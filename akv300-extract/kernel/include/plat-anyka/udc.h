#ifndef __UDC_H__
#define __UDC_H__

#include <asm/gpio.h>

#define USB_OP_MOD_REG			(AK_VA_SYSCTRL + 0x58)
#define USB_MODULE_RESET_REG	(AK_VA_SYSCTRL + 0x20)

/* Anyka usb register */
#define USB_FUNCTION_ADDR	0x0
#define USB_POWER_CTRL		0x1
#define USB_INTERRUPT_1		0x2
#define USB_INTERRUPT_2		0x4
#define USB_INTERRUPT_TX	0x6
#define USB_INTERRUPT_RX	0x8
#define USB_INTERRUPT_COMM	0xA
#define USB_INTERRUPT_USB	0xB
#define USB_FRAME_NUM		0xC
#define USB_EP_INDEX		0xE
#define USB_TEST_MODE		0xF
#define USB_TX_MAX			0x10
#define USB_CTRL_1			0x12
#define USB_CTRL_1_2		0x13
#define USB_RX_MAX			0x14
#define USB_CTRL_2			0x16
#define USB_CTRL_2_2		0x17
#define USB_EP_COUNT		0x18
#define USB_CFG_INFO		0x1F
#define USB_EP0_FIFO		0x20
#define USB_EP1_FIFO		0x24
#define USB_EP2_FIFO		0x28
#define USB_EP3_FIFO		0x2C
#define USB_EP4_FIFO		0x30
#define USB_EP5_FIFO		0x34
#define USB_DEVICE_CTRL		0x60
#define USB_DMA_INTR		0x200
#define USB_DMA_CTRL1		0x204
#define USB_DMA_CTRL2		0x214
#define USB_DMA_CTRL3		0x224
#define USB_DMA_CTRL4		0x234
#define USB_DMA_ADDR1		0x208
#define USB_DMA_ADDR2		0x218
#define USB_DMA_ADDR3		0x228
#define USB_DMA_ADDR4		0x238
#define USB_DMA_COUNT1		0x20C
#define USB_DMA_COUNT2		0x21C
#define USB_DMA_COUNT3		0x22C
#define USB_DMA_COUNT4		0x23C
#define USB_EP0_NUM			0x330
#define USB_EP2_NUM			0x334
#define USB_MODE_STATUS		0x344

#define USB_LINESTATE_WP		(3 << 15)
#define USB_DP_PU_EN			(1 << 14)
#define USB_ID_CFG				(3 << 12)
#define USB_PHY_CFG				(0x3f << 6)
#define USB_SUSPEND_EN			(1 << 2)
#define USB_ID_CLIENT			(3)
#define USB_ID_HOST				(1)
#define USB_PHY_CLIENT			(23)
#define USB_PHY_HOST			(31)
#define USB_ISO_UPDATE			(1 << 7)
#define USB_HSPEED_EN			(1 << 5)
#define USB_HSPEED_MODE			(1 << 4)
#define USB_RESUME_EN			(1 << 2)
#define USB_SUSPEND_MODE		(1 << 1)
#define USB_SUSPENDM_EN			(1 << 0)

#define USB_ENABLE_DMA              (1)
#define USB_DIRECTION_RX            (0<<1)
#define USB_DIRECTION_TX            (1<<1)
#define USB_DMA_MODE1               (1<<2)
#define USB_DMA_MODE0               (0<<2)
#define USB_DMA_INT_ENABLE          (1<<3)
#define USB_DMA_INT_DISABLE         (0<<3)
#define USB_DMA_BUS_ERROR           (1<<8)
#define USB_DMA_BUS_MODE0           (0<<9)
#define USB_DMA_BUS_MODE1           (1<<9)
#define USB_DMA_BUS_MODE2           (2<<9)
#define USB_DMA_BUS_MODE3           (3<<9)
#define DMA_CHANNEL1_INT          	(1)
#define DMA_CHANNEL2_INT          	(2)
#define DMA_CHANNEL3_INT          	(4)
#define DMA_CHANNEL4_INT          	(8)
#define USB_EP0_INDEX             	(0)
#define USB_EP1_INDEX             	(1 << 0)
#define USB_EP2_INDEX             	(1 << 1)
#define USB_EP3_INDEX             	((1 << 1)|(1 << 0))
#define USB_EP4_INDEX             	(1 << 2)
#define USB_EP5_INDEX             	((1 << 2)|(1 << 0))

// #define USB_11 [> usb1.1 <]   

#define EP0_FIFO_SIZE		64  /* control, not 64byte? */ 
#define EP1_FIFO_SIZE		64 /* interrupt */ 
#define EP2_FIFO_SIZE		512 /* ep2 in bulk */ 
#define EP3_FIFO_SIZE		512 /* ep3 out bulk */ 
#define EP4_FIFO_SIZE		512 /* ep4 in bulk */ 
#define EP5_FIFO_SIZE		512 /* ep5 out bulk */ 

#define USB_TXCSR_AUTOSET          	(0x80)
#define USB_TXCSR_ISO              	(0x40)
#define USB_TXCSR_MODE1            	(0x20)
#define USB_TXCSR_DMAREQENABLE     	(0x10)
#define USB_TXCSR_FRCDATATOG       	(0x8)
#define USB_TXCSR_DMAREQMODE1      	(0x4)
#define USB_TXCSR_DMAREQMODE0      	(0x0)

struct akudc_request;

/* the struct of endpoint for Anyka udc */
struct akudc_ep {
	struct usb_ep		ep; /* the basic endpoint */
	struct usb_gadget	*gadget; /* the gadget for udc */
	struct usb_endpoint_descriptor *desc; /* the descriptor for this endpoint */

	struct list_head	queue; /* the queue head for request of this endpoint */

	struct work_struct	work;
	struct akudc_request	*req; /* req be about to handle */ 

	struct akudc_udc	*udc; /* the udc struct */
	int			maxpacket;
	volatile int		done; /* whether the current request is complete */
	unsigned int		bufaddr;
	dma_addr_t		bufphys;

	unsigned		irq_enable; /* whether the endpoint cpu irq is enable */
	volatile unsigned	stopped; /*whether the endpoint is stopped */
	// unsigned		stopped:1;
	unsigned		is_in:1; /* whether the endpoint is in */
	unsigned		is_iso:1;
	// unsigned		fifo_bank:1;
	u8			l2_buf_id; /* the l2 buffer id for this endpoint */
};

struct akudc_request {
	struct list_head	queue;		/* ep's requests */
	struct usb_request	req;
	int			status;
};

/* the status flag for ep0 Control SETUP Transaction */
enum ep0_status {
        EP0_IDLE,
        EP0_IN_DATA_PHASE,
        EP0_OUT_DATA_PHASE,
        EP0_END_XFER,
        EP0_STALL,
};
#if 0
static const char *ep0states[]= {
        "EP0_IDLE",
        "EP0_IN_DATA_PHASE",
        "EP0_OUT_DATA_PHASE",
        "EP0_END_XFER",
        "EP0_STALL",
};
#endif
struct usb_l2 {
	void *buf;
	dma_addr_t phys;
};

static const char * const ep_name[] = {
	"ep0",
	"ep1-int",
	"ep2in-bulk",
	"ep3out-bulk",
	"ep4in-bulk",
	"ep5out-bulk",
};

#define ENDPOINTS_NUM	ARRAY_SIZE(ep_name)

struct akudc_udc {
	struct usb_gadget		gadget; /* the core struct for anyka udc */
	struct usb_gadget_driver	*driver; /* the core struct for function drivers */

	struct akudc_ep	ep[ENDPOINTS_NUM]; /* the endpoints for udc */
	enum ep0_status		ep0_status;

	void __iomem		*baseaddr;

	unsigned int	mcu_irq; /* the irq for cpu irq */
	unsigned int	dma_irq; /* the irq for l2 dma irq */
	struct clk	*clk; /* the clk for udc */
	char		addr;   /* assigned device address */ 
	u16			devstatus;
	int			ep0state; /* the status flag for ep0 Control SETUP Transaction */
	unsigned			req_std : 1;
	unsigned			req_config : 1;
	unsigned			req_pending : 1;
	unsigned			high_speed: 1;
	struct proc_dir_entry	*pde;
};


/* usb dosen't have vbus and id pin,so these bit must be set */
static inline void set_usb_as_slave(void)
{
	unsigned long con;

	con = __raw_readl(USB_OP_MOD_REG);
	con &= ~(0xff << 6);
	con |= ((0x3 << 12)|(0x17 << 6));
	__raw_writel(con, USB_OP_MOD_REG);
}

static inline void usb_vbus_wakeup(int enable)
{
#if defined (CONFIG_ARCH_AK37)
	/* open usb detect */
	if(enable) {
		REG32(AK_VA_SYSCTRL + 0xd8) &= ~(1 << 0);    //falling-trig 

		REG32(AK_VA_SYSCTRL + 0xd8) |= (1 << 2);     //clear usb det
		REG32(AK_VA_SYSCTRL + 0xd8) &= ~(1 << 2);

		REG32(AK_VA_SYSCTRL + 0xd8) |= (1 << 4);     //enable usb detect
	} else {
		REG32(AK_VA_SYSCTRL + 0xd8) |= (1 << 2);     //clear usb det
		REG32(AK_VA_SYSCTRL + 0xd8) &= ~(1 << 2);

		REG32(AK_VA_SYSCTRL + 0xd8) &= ~(1 << 4);    //disable usb detect
	}
#endif
}

static void udc_reg_writel(unsigned long __iomem *__reg, 
			unsigned long value, int bits, int index);
//static unsigned long udc_reg_readl(unsigned long __iomem *__reg, 
//			int bits, int index);

static void udc_reinit(struct akudc_udc *udc);
static void akudc_enable(struct akudc_udc *udc);
static void akudc_disable(struct akudc_udc *udc);
static void done(struct akudc_ep *ep, struct akudc_request *req, int status);

#endif	/* __UDC_H__ */
