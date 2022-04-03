/*
 * Driver for the SSD1306 OLED controller
 *
 * Copyright 2016 Martin K. Schröder <mkschreder.uk@gmail.com>
 * - making it work with ssd1306 based on u8g code 
 * - make it use a work queue to blit data to the display 
 * 
 * Based on code by: 
 * Copyright 2012 Free Electrons
 *
 * Licensed under the GPLv2 or later.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/fb.h>
#include <linux/uaccess.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/pwm.h>
#include <linux/delay.h>


#define SSD1306_DATA			0x40
#define SSD1306_COMMAND		0x00
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_SETSTARTPAGE 0xb0

// ssd1306 full list of definitions (not used by this driver)
#if 0
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTPAGE 0xb0
#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A
	
#define SSD1306_SET_ADDRESS_MODE	0x20
#define SSD1306_SET_ADDRESS_MODE_HORIZONTAL	(0x00)
#define SSD1306_SET_ADDRESS_MODE_VERTICAL	(0x01)
#define SSD1306_SET_ADDRESS_MODE_PAGE		(0x02)
#define SSD1306_SET_COL_RANGE		0x21
#define SSD1306_SET_PAGE_RANGE	0x22
#define SSD1306_CONTRAST		0x81
#define	SSD1306_CHARGE_PUMP		0x8d
#define SSD1306_SEG_REMAP_ON		0xa1
#define SSD1306_DISPLAY_OFF		0xae
#define SSD1306_SET_MULTIPLEX_RATIO	0xa8
#define SSD1306_DISPLAY_ON		0xaf
#define SSD1306_START_PAGE_ADDRESS	0xb0
#define SSD1306_SET_DISPLAY_OFFSET	0xd3
#define	SSD1306_SET_CLOCK_FREQ	0xd5
#define	SSD1306_SET_PRECHARGE_PERIOD	0xd9
#define	SSD1306_SET_COM_PINS_CONFIG	0xda
#define	SSD1306_SET_VCOMH		0xdb
#endif

struct ssd1306fb_par;

struct ssd1306fb_ops {
	int (*init)(struct ssd1306fb_par *);
	int (*remove)(struct ssd1306fb_par *);
};

struct ssd1306fb_par {
	struct i2c_client *client;
	u32 height;
	struct fb_info *info;
	struct ssd1306fb_ops *ops;
	u32 page_offset;
	struct pwm_device *pwm;
	u32 pwm_period;
	u32 width;

	// used for temporary data storage
	struct ssd1306fb_array *cmd1; 
	struct ssd1306fb_array *data16;

	struct work_struct work; 
};

struct ssd1306fb_array {
	u8	type;
	u8	data[0];
};

static struct fb_fix_screeninfo ssd1306fb_fix = {
	.id		= "SSD1306 OLED",
	.type		= FB_TYPE_PACKED_PIXELS,
	.visual		= FB_VISUAL_MONO10,
	.xpanstep	= 0,
	.ypanstep	= 0,
	.ywrapstep	= 0,
	.accel		= FB_ACCEL_NONE,
};

static struct fb_var_screeninfo ssd1306fb_var = {
	.bits_per_pixel	= 1,
};

static struct ssd1306fb_array *ssd1306fb_alloc_array(u32 len, u8 type){
	struct ssd1306fb_array *array;

	array = kzalloc(sizeof(struct ssd1306fb_array) + len, GFP_KERNEL);
	if (!array)
		return NULL;

	array->type = type;

	return array;
}

static int ssd1306fb_write_array(struct ssd1306fb_par *par,
				 struct ssd1306fb_array *array, u32 len){
	int ret;

	len += sizeof(struct ssd1306fb_array);

	ret = i2c_master_send(par->client, (u8 *)array, len);
	if (ret != len) {
		dev_err(&par->client->dev, "Couldn't send I2C command.\n");
		return ret;
	}

	return 0;
}

static inline int ssd1306fb_write_cmd(struct ssd1306fb_par *par, u8 cmd){
	par->cmd1->data[0] = cmd;
	return ssd1306fb_write_array(par, par->cmd1, 1);
}

static inline int ssd1306fb_write_data(struct ssd1306fb_par *par, u8 data){
	par->data16->data[0] = data; 
	return ssd1306fb_write_array(par, par->data16, 1);
}

static void ssd1306fb_update_display(struct ssd1306fb_par *par){
	u8 *vmem = par->info->screen_base;
	int i;

	// go to first character
	ssd1306fb_write_cmd(par, SSD1306_SETLOWCOLUMN); 
	ssd1306fb_write_cmd(par, SSD1306_SETHIGHCOLUMN); 
	ssd1306fb_write_cmd(par, SSD1306_SETSTARTPAGE); 

	ssd1306fb_write_cmd(par, SSD1306_COLUMNADDR);
	ssd1306fb_write_cmd(par, 0);   // Column start address (0 = reset)
	ssd1306fb_write_cmd(par, 127); // Column end address (127 = reset)

	ssd1306fb_write_cmd(par, SSD1306_PAGEADDR);
	ssd1306fb_write_cmd(par, 0); // Page start address (0 = reset)
	ssd1306fb_write_cmd(par, 7); // Page end address	


	// write in chunks of 16 bytes (can not send more over i2c right now)
	for(i = 0; i < 64; i++){
		memcpy(par->data16->data, vmem + i * 16, 16); 
		ssd1306fb_write_array(par, par->data16, 16);
	}
}


static ssize_t ssd1306fb_write(struct fb_info *info, const char __user *buf,
		size_t count, loff_t *ppos){
	unsigned long total_size;
	unsigned long p = *ppos;
	struct ssd1306fb_par *par = info->par; 
	u8 __iomem *dst;

	//printk("ssd1306: write %d at %lu\n", count, p); 

	total_size = info->fix.smem_len;

	if (p > total_size)
		return -EINVAL;

	if (count + p > total_size)
		count = total_size - p;

	if (!count)
		return -EINVAL;

	dst = (void __force *) (info->screen_base + p);

	if (copy_from_user(dst, buf, count))
		return -EFAULT;
	
	schedule_work(&par->work); 

	*ppos += count;

	return count;
}

static void ssd1306fb_fillrect(struct fb_info *info, const struct fb_fillrect *rect)
{
	struct ssd1306fb_par *par = info->par;
	cfb_fillrect(info, rect);
	schedule_work(&par->work); 
}

static void ssd1306fb_copyarea(struct fb_info *info, const struct fb_copyarea *area)
{
	struct ssd1306fb_par *par = info->par;
	cfb_copyarea(info, area);
	schedule_work(&par->work); 
}

static void ssd1306fb_imageblit(struct fb_info *info, const struct fb_image *image)
{
	struct ssd1306fb_par *par = info->par;
	cfb_imageblit(info, image);
	schedule_work(&par->work); 
}

static struct fb_ops ssd1306fb_ops = {
	.owner		= THIS_MODULE,
	.fb_write	= ssd1306fb_write,
	.fb_fillrect	= ssd1306fb_fillrect,
	.fb_copyarea	= ssd1306fb_copyarea,
	.fb_imageblit	= ssd1306fb_imageblit,
};

static void ssd1306fb_update_task(struct work_struct *work){
	struct ssd1306fb_par *par = container_of(work, struct ssd1306fb_par, work);  
	//printk("ssd1306: update task\n"); 
	ssd1306fb_update_display(par); 
	//printk("ssd1306: update done\n"); 
}

static int ssd1306fb_ssd1306_init(struct ssd1306fb_par *par)
{
	int ret, c;

	#define U8G_ESC_CS(x) 255, (0xd0 | ((x)&0x0f))
	#define U8G_ESC_ADR(x) 255, (0xe0 | ((x)&0x0f))
	#define U8G_ESC_RST(x) 255, (0xc0 | ((x)&0x0f))
	#define U8G_ESC_END 255, 254

	static const uint8_t cmd_init[] = {
		U8G_ESC_CS(0),             //disable chip
		U8G_ESC_ADR(0),           /* instruction mode */
		U8G_ESC_RST(1),           /* do reset low pulse with (1*16)+2 milliseconds */
		U8G_ESC_CS(1),             /* enable chip */
		
		0x0ae,				/* display off, sleep mode */
		0x0d5, 0x081,		/* clock divide ratio (0x00=1) and oscillator frequency (0x8) */
		0x0a8, 0x03f,		/* multiplex ratio */
		0x0d3, 0x000,	0x00,	/* display offset */
		//0x040,				/* start line */
		0x08d, 0x14,		/* charge pump setting (p62): 0x014 enable, 0x010 disable */
		0x020, 0x00, // memory addr mode
		0x0a1,				/* segment remap a0/a1*/
		0x0a5, // display on
		0x0c8,				/* c0: scan dir normal, c8: reverse */
		0x0da, 0x012,		/* com pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5) */
		0x081, 0x09f,		/* set contrast control */
		0x0d9, 0x011,		/* pre-charge period */
		0x0db, 0x020,		/* vcomh deselect level */
		0x021, 0x00, 0x7f, // (7d for 125 col screen!) column addressing mode
		0x022, 0x00, 0x07,		/* page addressing mode WRONG: 3 byte cmd! */
		0x0a4,				/* output ram to display */
		0x0a6,				/* none inverted normal display mode */
		0x0af,				/* display on */

		U8G_ESC_CS(0),             /* disable chip */
		U8G_ESC_END                /* end of sequence */
	};	
	
	for(c = 0; c < sizeof(cmd_init); c++){
		ret = ssd1306fb_write_cmd(par, cmd_init[c]);
		if (ret < 0){
			printk("ssd1306: i2c write failed!\n"); 
			return ret;
		}
		mdelay(10); 
	}
	
	schedule_work(&par->work); 
	//ssd1306fb_update_display(par); 
	
	printk("ssd1306: display initialized\n"); 

	return 0;
}

static struct ssd1306fb_ops ssd1306fb_ssd1306_ops = {
	.init	= ssd1306fb_ssd1306_init,
};


static int ssd1306fb_probe(struct i2c_client *client,
			   const struct i2c_device_id *id){
	struct fb_info *info;
	struct ssd1306fb_par *par;
	u8 __iomem *vmem; 
	u32 vmem_size;
	int ret;

	dev_info(&client->dev, "Probing ssd1306fb module..\n"); 

	info = framebuffer_alloc(sizeof(struct ssd1306fb_par), &client->dev);
	if (!info) {
		dev_err(&client->dev, "Couldn't allocate framebuffer.\n");
		return -ENOMEM;
	}

	par = info->par;
	par->info = info;
	par->client = client;

	par->ops = &ssd1306fb_ssd1306_ops; 

	par->width = 128; 
	par->height = 64; 
	par->page_offset = 1; 
	
	par->data16 = ssd1306fb_alloc_array(16, SSD1306_DATA); 
	par->cmd1 = ssd1306fb_alloc_array(1, SSD1306_COMMAND); 

	INIT_WORK(&par->work, ssd1306fb_update_task); 
	
	vmem_size = par->width * par->height / 8;

	vmem = (u8 __force __iomem*) devm_kzalloc(&client->dev, vmem_size, GFP_KERNEL);
	info->screen_base = vmem;

	if (!info->screen_base) {
		dev_err(&client->dev, "Couldn't allocate graphical memory.\n");
		ret = -ENOMEM;
		goto fb_alloc_error;
	}

	info->fbops = &ssd1306fb_ops;
	info->fix = ssd1306fb_fix;
	info->fix.line_length = par->width / 8;
	//info->fbdefio = &ssd1306fb_defio;

	info->var = ssd1306fb_var;
	info->var.xres = par->width;
	info->var.xres_virtual = par->width;
	info->var.yres = par->height;
	info->var.yres_virtual = par->height;

	info->var.red.length = 1;
	info->var.red.offset = 0;
	info->var.green.length = 1;
	info->var.green.offset = 0;
	info->var.blue.length = 1;
	info->var.blue.offset = 0;

	info->fix.smem_start = (unsigned long)info->screen_base;
	info->fix.smem_len = vmem_size;

	i2c_set_clientdata(client, info);

	if (par->ops->init) {
		printk("ssd1306: initializing display\n"); 
		ret = par->ops->init(par);
		if (ret){
			printk("ssd1306: failed to initialize screen!\n"); 
			goto reset_oled_error;
		}
	}

	ret = register_framebuffer(info);
	if (ret) {
		dev_err(&client->dev, "Couldn't register the framebuffer\n");
		goto panel_init_error;
	}
	
	dev_info(&client->dev, "fb%d: %s framebuffer device registered, using %d bytes of video memory\n", info->node, info->fix.id, vmem_size);

	return 0;

panel_init_error:
	if (par->ops->remove)
		par->ops->remove(par);
reset_oled_error:
	//fb_deferred_io_cleanup(info);
fb_alloc_error:
	framebuffer_release(info);
	return ret;
}

static int ssd1306fb_remove(struct i2c_client *client)
{
	struct fb_info *info = i2c_get_clientdata(client);
	struct ssd1306fb_par *par = info->par;

	dev_info(&client->dev, "removing ssd1307 driver\n"); 

	kfree(par->data16); 
	kfree(par->cmd1); 

	unregister_framebuffer(info);
	if (par->ops->remove)
		par->ops->remove(par);
	framebuffer_release(info);

	return 0;
}

static const struct i2c_device_id ssd1306fb_i2c_id[] = {
	{ "ssd1306fb", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, ssd1306fb_i2c_id);

static struct i2c_driver ssd1306fb_driver = {
	.probe = ssd1306fb_probe,
	.remove = ssd1306fb_remove,
	.id_table = ssd1306fb_i2c_id,
	.driver = {
		.name = "ssd1306fb",
		.owner = THIS_MODULE,
	},
};

module_i2c_driver(ssd1306fb_driver);

MODULE_DESCRIPTION("FB driver for SSD1306 OLED I2C display");
MODULE_AUTHOR("Maxime Ripard <maxime.ripard@free-electrons.com>");
MODULE_LICENSE("GPL");
