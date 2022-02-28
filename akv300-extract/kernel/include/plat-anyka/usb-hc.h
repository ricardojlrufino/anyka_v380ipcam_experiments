#ifndef _ANYKA_USB_HC_H_
#define _ANYKA_USB_HC_H_


#include <linux/usb.h>
#include <linux/platform_device.h>
#include <asm/io.h>
#include <plat/l2.h>

#include <mach/map.h>
#include <mach/gpio.h>
#include "usb-reg-define.h"

#include "otg-hshcd.h"

#define DYNAMIC_EPFIFO 
/*
 * IN V300, we have 5 epfifo. epfifo 0 using for Control transfer.
 * epfifo1-epfifi4 usnig for Interrupt/Bulk/Isochronous tranfer.
 * so, MAX_EP_NUM is 4 (EP4)
 */
#define MAX_EP_NUM				(4) 
#define USBDMA_CHANNEL_NUM		(4)

#define	LOG2_PERIODIC_SIZE		10	/* arbitrary; this matches OHCI */
#define	PERIODIC_SIZE			(1 << LOG2_PERIODIC_SIZE)

 /* usb 1.1 says max 90% of a frame is available for periodic transfers.
  * this driver doesn't promise that much since it's got to handle an
  * IRQ per packet; irq handling latencies also use up that time.
  *
  * NOTE:  the periodic schedule is a sparse tree, with the load for
  * each branch minimized.	see fig 3.5 in the OHCI spec for example.
  */
#define	MAX_PERIODIC_LOAD	500	/* out of 1000 usec */
 
 /*-------------------------------------------------------------------------*/
 
 /*Common Registers Access - Just use base address plus offset */

 static DEFINE_SPINLOCK(USB_HC_REG_LOCK);
 
#define hc_readb(reg)		__raw_readb(USB_HC_BASE_ADDR + (reg))
#define hc_writeb(val, reg)	__raw_writeb(val, USB_HC_BASE_ADDR + (reg))

#define hc_readw(reg)		__raw_readw(USB_HC_BASE_ADDR + (reg))
#define hc_writew(val, reg)	__raw_writew(val, USB_HC_BASE_ADDR + (reg))

#define hc_readl(reg)		__raw_readl(USB_HC_BASE_ADDR + (reg))
#define hc_writel(val, reg)	__raw_writel(val, USB_HC_BASE_ADDR + (reg))

 struct akotghc_ep {
	 struct usb_host_endpoint *hep;
	 struct usb_device	 *udev;
 
	 u16 		 maxpacket;
	 u8 		 epnum;
	 u8			 epfifo;
	 u8 		 nextpid;
 
	 u16		 error_count;
	 u16		 nak_count;
	 u16		 length;	 /* of current packet */

     bool        bdma;
 
	 /* periodic schedule */
	 u32		 period;
	 u32		 branch;
	 u32		 load;
	 struct akotghc_ep	 *next;
 
	 /* async schedule */
	 struct list_head	 schedule;
 };

/**
    struct for usb dma channel
 */
 struct akotg_dma
 {
    u8          channel;            //channel num
    u8          status;             //channel status
    u8          dir;                //transfer direction, 
    u8          epfifo;             //dma is bind to which usb ep fifo
    u8          l2buffer;           //l2 buffer which is bind to ep fifo
    u32         addr;               //inner addr for dma, start with 0x70000000
    u32         count;              //dma count
    u32         phyaddr;            //phycical address for dma
 };
 
  struct akotg_usbhc {
	 spinlock_t 	 lock;
	 
	 struct clk		*clk;
	 
	 /* sw model */
	 struct timer_list	 timer;
	 struct akotghc_ep	 *next_periodic;
	 struct akotghc_ep	 *next_async_ep0;
	 struct akotghc_ep	 *next_async_epx[MAX_EP_NUM];
 
	 struct akotghc_ep	 *active_ep0;
	 struct akotghc_ep	 *active_epx[MAX_EP_NUM];
	 unsigned long		 jiffies_ep0;
	 unsigned long		 jiffies_epx[MAX_EP_NUM];
	 
	 int 				mcu_irq;
	 
	 //dma
	 int                 dma_irq; //irq alloced for dma
	 struct akotg_dma    dma_channel[USBDMA_CHANNEL_NUM];
 
	 u32		 port_status;
	 u16		 frame;
 
	 /* async schedule: control, bulk */
	 struct list_head	 async_ep0;
	 struct list_head	 async_epx[MAX_EP_NUM];
 
	 /* periodic schedule: interrupt, iso */
	 u16		 load[PERIODIC_SIZE];
	 struct akotghc_ep	 *periodic[PERIODIC_SIZE];
	 unsigned		 periodic_count;
 };


 struct epfifo_mapping {
	int	epfifo;		
	int	used;		/* 0 - Unused, 1 - used */
	int	epnum;		/* USB Device endpoint number: 1 ~ 16 */
	int	direction;	/* 0 - In, 1 - Out */
};

struct akotghc_epfifo_mapping {
	spinlock_t lock;
	struct epfifo_mapping mapping[MAX_EP_NUM];
};

/**
    enum for DMA channel status
 */
enum
{
    USB_DMA_CHANNEL_IDLE,       //idle
    USB_DMA_CHANNEL_ALLOC,      //alloced for a specific ep fifo but not under tranfering
    USB_DMA_CHANNEL_TRANS       //there is a dma tranfering in this channel
};

irqreturn_t akotg_usbhc_irq(struct usb_hcd *hcd);
irqreturn_t akotg_dma_irq(int irqnum, void *__hcd);

int akotg_usbhc_urb_enqueue(struct usb_hcd	*hcd, struct urb *urb, gfp_t mem_flags);
int akotg_usbhc_urb_dequeue(struct usb_hcd *hcd, struct urb *urb, int status);
void akotg_usbhc_endpoint_reset(struct usb_hcd *hcd, struct usb_host_endpoint *hep);
void akotg_usbhc_endpoint_disable(struct usb_hcd *hcd, struct usb_host_endpoint *hep);
int akotg_usbhc_get_frame(struct usb_hcd *hcd);
int akotg_usbhc_hub_status_data(struct usb_hcd *hcd, char *buf);
void akotg_usbhc_hub_descriptor (struct akotg_usbhc *akotghc, struct usb_hub_descriptor	*desc);
void akotg_usbhc_timer(unsigned long _akotghs);
int akotg_usbhc_hub_control(
	struct usb_hcd	*hcd,
	u16		typeReq,
	u16		wValue,
	u16		wIndex,
	char		*buf,
	u16		wLength
);

int akotg_usbhc_bus_suspend(struct usb_hcd *hcd);
int akotg_usbhc_bus_resume(struct usb_hcd *hcd);
void akotg_usbhc_stop(struct usb_hcd *hcd);
int akotg_usbhc_start(struct usb_hcd *hcd);
int akotg_usbhc_reset(struct usb_hcd *hcd);


void port_power(struct akotg_usbhc *akotghc, int is_on);

static inline struct akotg_usbhc *hcd_to_akotg_usbhc(struct usb_hcd *hcd)
{
	return (struct akotg_usbhc *) (hcd->hcd_priv);
}

static inline struct usb_hcd *akotg_usbhc_to_hcd(struct akotg_usbhc *akotghc)
{
	return container_of((void *) akotghc, struct usb_hcd, hcd_priv);
}


/*
 * Part II: Index Registers Access - Spinlock+IRQ protection
 */
//static DEFINE_SPINLOCK(fsh_reg_lock);

static inline unsigned char hc_index_readb(int epindex, int reg)
{
	unsigned long flags;
	unsigned char val;

	spin_lock_irqsave(&USB_HC_REG_LOCK, flags);

	hc_writeb(epindex, USB_REG_INDEX);
	val = hc_readb(reg);

	spin_unlock_irqrestore(&USB_HC_REG_LOCK, flags);

	return val;
}

static inline void hc_index_writeb(int epindex, unsigned char val, int reg)
{
	unsigned long flags;
	
	spin_lock_irqsave(&USB_HC_REG_LOCK, flags);

	hc_writeb(epindex, USB_REG_INDEX);
	hc_writeb(val, reg);

	spin_unlock_irqrestore(&USB_HC_REG_LOCK, flags);
}

static inline unsigned short hc_index_readw(int epindex, int reg)
{
	unsigned long flags;
	unsigned short val;

	spin_lock_irqsave(&USB_HC_REG_LOCK, flags);

	hc_writeb(epindex, USB_REG_INDEX);
	val = hc_readw(reg);

	spin_unlock_irqrestore(&USB_HC_REG_LOCK, flags);

	return val;

}

static inline void hc_index_writew(int epindex, unsigned short val, int reg)
{
	unsigned long flags;

	spin_lock_irqsave(&USB_HC_REG_LOCK, flags);

	hc_writeb(epindex, USB_REG_INDEX);
	hc_writew(val, reg);

	spin_unlock_irqrestore(&USB_HC_REG_LOCK, flags);
}

static inline unsigned long hc_index_readl(int epindex, int reg)
{
	unsigned long flags;
	unsigned long val;

	spin_lock_irqsave(&USB_HC_REG_LOCK, flags);

	hc_writeb(epindex, USB_REG_INDEX);
	val = hc_readl(reg);

	spin_unlock_irqrestore(&USB_HC_REG_LOCK, flags);

	return val;
}

static inline void hc_index_writel(int epindex, unsigned long val, int reg)
{
	unsigned long flags;

	spin_lock_irqsave(&USB_HC_REG_LOCK, flags);

	hc_writeb(epindex, USB_REG_INDEX);
	hc_writel(val, reg);

	spin_unlock_irqrestore(&USB_HC_REG_LOCK, flags);

}
static inline void flush_ep0_fifo(void)
{
	unsigned long flags;

	spin_lock_irqsave(&USB_HC_REG_LOCK, flags);

	hc_writeb(0, USB_REG_INDEX);
	if (hc_readb(USB_REG_TXCSR1) & (USB_CSR01_RXPKTRDY | USB_CSR01_TXPKTRDY))
		hc_writeb(USB_CSR02_FLUSHFIFO, USB_REG_TXCSR2);

	spin_unlock_irqrestore(&USB_HC_REG_LOCK, flags);
}

static inline void flush_epx_tx_fifo(int i)
{
	unsigned long flags;

	spin_lock_irqsave(&USB_HC_REG_LOCK, flags);

	hc_writeb(i, USB_REG_INDEX);
	if (hc_readb(USB_REG_TXCSR1) & (USB_CSR01_RXPKTRDY))
		hc_writeb(USB_TXCSR1_FLUSHFIFO, USB_REG_TXCSR1);

	spin_unlock_irqrestore(&USB_HC_REG_LOCK, flags);

}

static inline void flush_epx_rx_fifo(int i)
{
	unsigned long flags;

	spin_lock_irqsave(&USB_HC_REG_LOCK, flags);

	hc_writeb(i, USB_REG_INDEX);
	if (hc_readb(USB_REG_RXCSR1) & (USB_RXCSR1_RXPKTRDY))
		hc_writeb(USB_RXCSR1_FLUSHFIFO, USB_REG_RXCSR1);

	spin_unlock_irqrestore(&USB_HC_REG_LOCK, flags);
}

static inline void flush_epx_fifo(int i)
{
	flush_epx_tx_fifo(i);
	flush_epx_rx_fifo(i);
}

static inline void flush_ep_fifo(int i)
{
	if (i == 0)
		flush_ep0_fifo();
	else
		flush_epx_fifo(i);
}

static inline void set_epx_rx_mode(int i)
{
	u8 regval;
	unsigned long flags;

	spin_lock_irqsave(&USB_HC_REG_LOCK, flags);

	hc_writeb(i, USB_REG_INDEX);
	regval = hc_readb(USB_REG_TXCSR2);
	regval &= ~USB_TXCSR2_MODE;
	hc_writeb(regval, USB_REG_TXCSR2);

	spin_unlock_irqrestore(&USB_HC_REG_LOCK, flags);
}

static inline void set_epx_tx_mode(int i)
{
	u8 regval;
	unsigned long flags;

	spin_lock_irqsave(&USB_HC_REG_LOCK, flags);

	hc_writeb(i, USB_REG_INDEX);
	regval = hc_readb(USB_REG_TXCSR2);
	regval |= USB_TXCSR2_MODE;
	hc_writeb(regval, USB_REG_TXCSR2);

	spin_unlock_irqrestore(&USB_HC_REG_LOCK, flags);
}

static inline void clear_epx_tx_data_toggle(int i)
{
	hc_index_writeb(i, USB_TXCSR1_CLRDATATOG, USB_REG_TXCSR1);
	//hc_index_writeb(i, USB_RXCSR2_DMAREQENAB, USB_REG_TXCSR2);  //???
}

static inline void clear_epx_rx_data_toggle(int i)
{
	hc_index_writeb(i, USB_RXCSR1_CLRDATATOG, USB_REG_RXCSR1);
}

/*
 * Valid types:
 *   1 - Isochronous
 *   2 - Bulk
 *   3 - Interrupt
 * Invalid type:
 *   0 - Illegal
 */
static inline void set_epx_tx_type(int i, int epnum, int type)
{
	BUG_ON(i < 0 || i > MAX_EP_NUM);
	BUG_ON(type < 0 || type > 3);

	hc_index_writeb(i, type << 4 | epnum, USB_REG_TXTYPE);
}

static inline void set_epx_rx_type(int i, int epnum, int type)
{
	BUG_ON(i < 0 || i > MAX_EP_NUM);
	BUG_ON(type < 0 || type > 3);

	hc_index_writeb(i, type << 4 | epnum, USB_REG_RXTYPE);
}

static inline void enable_ep0_interrupt(void)
{
	u8 regval;
	unsigned long flags;

	local_irq_save(flags);

	regval = hc_readb(USB_REG_INTRTXE);
	regval |= USB_INTR_EP0;
	hc_writeb(regval, USB_REG_INTRTXE);

	local_irq_restore(flags);
}

static inline void enable_epx_tx_interrupt(int i)
{
	u8 regval;
	unsigned long flags;

	local_irq_save(flags);

	regval = hc_readb(USB_REG_INTRTXE);
	regval |= (1 << i);
	hc_writeb(regval, USB_REG_INTRTXE);

	local_irq_restore(flags);
}

static inline void enable_epx_rx_interrupt(int i)
{
	u8 regval;
	unsigned long flags;

	local_irq_save(flags);

	regval = hc_readb(USB_REG_INTRRXE);
	regval |= (1 << i);
	hc_writeb(regval, USB_REG_INTRRXE);

	local_irq_restore(flags);
}

static inline void disable_ep0_interrupt(void)
{
	u8 regval;
	unsigned long flags;

	local_irq_save(flags);

	regval = hc_readb(USB_REG_INTRTXE);
	regval &= ~USB_INTR_EP0;
	hc_writeb(regval, USB_REG_INTRTXE);

	local_irq_restore(flags);
}

static inline void disable_epx_tx_interrupt(int i)
{
	u8 regval;
	unsigned long flags;

	local_irq_save(flags);

	regval = hc_readb(USB_REG_INTRTXE);
	regval &= ~(1 << i);
	hc_writeb(regval, USB_REG_INTRTXE);

	local_irq_restore(flags);
}

static inline void disable_epx_rx_interrupt(int i)
{
	u8 regval;
	unsigned long flags;

	local_irq_save(flags);

	regval = hc_readb(USB_REG_INTRRXE);
	regval &= ~(1 << i);
	hc_writeb(regval, USB_REG_INTRRXE);

	local_irq_restore(flags);
}

static inline void disable_epx_interrupt(int i)
{
	disable_epx_tx_interrupt(i);
	disable_epx_rx_interrupt(i);
}

static inline void disable_ep_interrupt(int i)
{
	BUG_ON(i < 0 || i > MAX_EP_NUM);

	if (i == 0) {
		disable_ep0_interrupt();
	} else {
		disable_epx_interrupt(i);
	}
}

static inline void clear_all_interrupts(void)
{
	hc_writeb(0x0, USB_REG_INTRUSBE);
	hc_writew(0x0, USB_REG_INTRTXE);
	hc_writew(0x0, USB_REG_INTRRXE);
	
	hc_readb(USB_REG_INTRUSB);
	hc_readw(USB_REG_INTRTX);
	hc_readw(USB_REG_INTRRX);
}

static inline void reset_endpoint(int i)
{
	BUG_ON(i < 0 || i > MAX_EP_NUM);

	disable_ep_interrupt(i);
	if (i == 0) {
		flush_ep0_fifo();
	} else {
		flush_epx_fifo(i);
		set_epx_rx_type(i, 0, 0);
		set_epx_tx_type(i, 0, 0);
	}
}

static inline void reset_endpoints(void)
{
	int i;

	for (i = 0; i < MAX_EP_NUM + 1; i++) {
		reset_endpoint(i);
	}
}


static inline u8 akotg_alloc_l2_buffer(int epfifo)
{
	l2_device_t l2addr[] = {ADDR_USB_EP1, ADDR_USB_EP2, ADDR_USB_EP3,ADDR_USB_EP4};

    //check param
    if((epfifo < 1) || (epfifo > MAX_EP_NUM))
        return BUF_NULL;

    //alloc l2 buffer
    return l2_alloc_nowait(l2addr[epfifo-1]);
}

static inline void akotg_free_l2_buffer(int epfifo)
{
	l2_device_t l2addr[] = {ADDR_USB_EP1, ADDR_USB_EP2, ADDR_USB_EP3,ADDR_USB_EP4};

    /*
     * check param. 
     * In V300, we have epfifo0、epfifo1、epfifo2、epfifo3、epfifo4.
     * But only epfifo1、epfifo2、epfifo3、epfifo4 can use L2 DMA.
     */
    if((epfifo < 1) || (epfifo > MAX_EP_NUM))
        return;

    //alloc l2 buffer
    l2_free(l2addr[epfifo-1]);
}



static inline void akotg_dma_init(struct akotg_usbhc *otg)
{
    int i;

    memset(otg->dma_channel, 0, sizeof(otg->dma_channel));
    
    for(i = 0; i < USBDMA_CHANNEL_NUM; i++)
    {
        otg->dma_channel[i].channel = i;
        otg->dma_channel[i].status = USB_DMA_CHANNEL_IDLE;
        otg->dma_channel[i].l2buffer = BUF_NULL;
    }
}


/** 
    config dma register
*/
static inline bool akotg_dma_config(struct akotg_dma *dma)
{
    u8 channel = dma->channel;
    u8 dir = dma->dir;
    u8 epfifo = dma->epfifo;

    u32 dmactrl;
    

    //config dma address and count
    hc_writel(dma->addr, USB_DMA_ADDR(channel));
    hc_writel(dma->count, USB_DMA_COUNT(channel));

    //config dma control
    dmactrl = (USB_ENABLE_DMA|dir|USB_DMA_MODE1|USB_DMA_INT_ENABLE|(epfifo <<4)|USB_DMA_BUS_MODE3);
	//pr_err("akotg_dma_config, dmactrl=%x\n", dmactrl);
    hc_writel(dmactrl, USB_DMA_CTRL(channel));

    dma->status = USB_DMA_CHANNEL_TRANS;

    return true;
}


/** 
    config dma struct
*/
static inline bool akotg_dma_set_struct(struct akotg_dma *dma, u8 dir, u8 epfifo, u8 l2buffer, u32 count, dma_addr_t phyaddr)
{
    if((epfifo < 1) || (epfifo > MAX_EP_NUM))
        return false; 
    
    dma->dir = dir;
    dma->count = count;
    dma->epfifo = epfifo;
    dma->l2buffer = l2buffer;

    //config dma addr
    dma->addr = 0x70000000 + 0x1000000*(epfifo-1);  
    dma->phyaddr = phyaddr;
    
    return true;
}



/** 
    alloc dma channel for epfifo
*/
static inline struct akotg_dma * akotg_dma_alloc(struct akotg_usbhc *otg, u8 epfifo)
{
    struct akotg_dma *dma;
    u8 l2buffer;
    
    if((epfifo < 1) || (epfifo > MAX_EP_NUM))
        return NULL;

    //alloc dma channel, do simple now,
    //we'll complete it later
    dma = &otg->dma_channel[epfifo-1];
    if(dma->status != USB_DMA_CHANNEL_IDLE){
        return NULL;
    }

    //alloc l2 buffer
    l2buffer = akotg_alloc_l2_buffer(epfifo);
    if(BUF_NULL == l2buffer){
        return NULL;
    }
    
    //printk("<%d:%d>\n", epfifo, l2buffer);

    dma->status = USB_DMA_CHANNEL_ALLOC;
    dma->l2buffer = l2buffer;

    return dma;
}

/** 
    get dma struct for channel i
*/
static inline struct akotg_dma *akotg_dma_get_struct(struct akotg_usbhc *otg, u8 channel)
{
    if(channel > USBDMA_CHANNEL_NUM) 
        return NULL;
        
    return &otg->dma_channel[channel];
}

/** 
    the usb dma trans will be stopped if a short packet is received,
    we can use this func to check the real number of data been received
*/
static inline u32 akotg_dma_get_trans_length(struct akotg_usbhc *otg, u8 epfifo)
{
    int i;
    struct akotg_dma *dma;
    u32 addr;

    for(i = 0; i < USBDMA_CHANNEL_NUM; i++)
    {
        dma = &otg->dma_channel[i];

        if((dma->epfifo == epfifo) && (dma->status == USB_DMA_CHANNEL_TRANS))
        {
            addr = hc_readl(USB_DMA_ADDR(dma->channel));
            return (addr - dma->addr);
        }
    }

    return 0;
}

/** 
    clear dma,
    epfifo: 0-clear all, >0 clear dma for epfifo
*/
static inline void akotg_dma_clear(struct akotg_usbhc *otg, u8 epfifo)
{
    int i;
    struct akotg_dma *dma;

    for(i = 0; i < USBDMA_CHANNEL_NUM; i++)
    {
        dma = &otg->dma_channel[i];

        if((epfifo != 0) && (dma->epfifo != epfifo)){
            continue;
        }

        if(dma->l2buffer != BUF_NULL)
        {
            //l2 free
            akotg_free_l2_buffer(dma->epfifo);
        }

        hc_writel(0, USB_DMA_CTRL(i));

        dma->status = USB_DMA_CHANNEL_IDLE;
        dma->l2buffer = BUF_NULL;
    }
    

}

static inline void init_epfifo_mapping(struct akotghc_epfifo_mapping *akotg_mapping)
{
	int i;
	struct epfifo_mapping *mapping;

	spin_lock_init(&akotg_mapping->lock);
	
	for (i = 0; i < MAX_EP_NUM; i++) {
		mapping = &akotg_mapping->mapping[i];
		mapping->epfifo = i + 1;	/* EPFIFO1 ~ MAX_EP_NUM(EPFIFO4) is mapping */
		mapping->used = 0;
		mapping->epnum = 0;
		mapping->direction = 0;
	}
}

static inline bool __is_epnum_mapped(
	struct akotghc_epfifo_mapping *akotg_mapping, int epnum, int direction)
{
	int i;
	struct epfifo_mapping *mapping;

	if(epnum == 0)
		return true;

	for (i = 0; i < MAX_EP_NUM; i++) {
		mapping = &akotg_mapping->mapping[i];
		if (mapping->used 
			&& (mapping->epnum == epnum) 
			&& (mapping->direction == direction)) {
			return true;
		}
	}

	return false;
}

static inline bool is_epnum_mapped(struct akotghc_epfifo_mapping *akotg_mapping,
	int epnum, int direction)
{
	bool ret;
	unsigned long flags;

	BUG_ON(akotg_mapping == NULL);

	if(epnum == 0)
		return true;

	spin_lock_irqsave(&akotg_mapping->lock, flags);

	ret = __is_epnum_mapped(akotg_mapping, epnum, direction);

	spin_unlock_irqrestore(&akotg_mapping->lock, flags);

	return ret;

}

static inline bool __map_epnum_to_epfifo(
	struct akotghc_epfifo_mapping *akotg_mapping,
	int epnum, int direction, int *epfifo)
{
	int i;
	struct epfifo_mapping *mapping;
	
	if (__is_epnum_mapped(akotg_mapping, epnum, direction))
		return false;

	//allocate 512(or 1024) byte size of filo to epnum
	for (i = 0; i < MAX_EP_NUM; i++) {	
		mapping = &akotg_mapping->mapping[i];
		if (!mapping->used) {
			mapping->used = 1;
			mapping->epnum = epnum;
			mapping->direction = direction;
			*epfifo = mapping->epfifo;
			return true;
		}
	}

	return false;
}

static inline bool map_epnum_to_epfifo(struct akotghc_epfifo_mapping *akotg_mapping,
	int epnum, int direction, int *epfifo)
{
	bool ret;
	unsigned long flags;

	BUG_ON(akotg_mapping == NULL);

	if (is_epnum_mapped(akotg_mapping, epnum, direction))
		return false;

	spin_lock_irqsave(&akotg_mapping->lock, flags);
	
	ret = __map_epnum_to_epfifo(akotg_mapping, epnum, direction, epfifo);

	spin_unlock_irqrestore(&akotg_mapping->lock, flags);

	return ret;
}

static inline bool epfifo_to_epnum(struct akotghc_epfifo_mapping *akotg_mapping, int epfifo, int *epnum, int *direction)
{
	int ret;
	unsigned long flags;
	struct epfifo_mapping *mapping;

	ret = false;

	spin_lock_irqsave(&akotg_mapping->lock, flags);

	mapping = &akotg_mapping->mapping[epfifo];
	if (mapping->used) {
		*epnum = mapping->epnum;
		*direction = mapping->direction;
		ret = true;
	} else {
		*epnum = 0;
		*direction = 0;
		ret = false;
	}

	spin_unlock_irqrestore(&akotg_mapping->lock, flags);

	return ret;
}

static inline bool epnum_to_epfifo(struct akotghc_epfifo_mapping *akotg_mapping, int epnum, int direction, int *epfifo)
{
	int i;
	unsigned long flags;
	struct epfifo_mapping *mapping;

	if (epnum == 0) {
		*epfifo = 0;
		return true;
	}

	spin_lock_irqsave(&akotg_mapping->lock, flags);
	
	for (i = 0; i < MAX_EP_NUM; i++) {
		mapping = &akotg_mapping->mapping[i];
		if (mapping->used && (mapping->epnum == epnum) && (mapping->direction == direction)) {
			*epfifo = mapping->epfifo;
			spin_unlock_irqrestore(&akotg_mapping->lock, flags);
			return true;
		}
	}

	spin_unlock_irqrestore(&akotg_mapping->lock, flags);

	return false;
}

static inline void set_usb_as_host(void)
{
	unsigned long con;

	con = __raw_readl(USB_OP_MOD_REG);
	con &= ~(0xff << 6);
	con |= (0x1 << 12)|(0x1f << 6);
	__raw_writel(con, USB_OP_MOD_REG);
}

static inline void usb_reset_phy(struct akotg_usbhc *otghc)
{
	unsigned long con;
	
	con = __raw_readl(USB_OP_MOD_REG);
	con &= ~(0x7 << 0);
	con |= (1 << 0);
	__raw_writel(con, USB_OP_MOD_REG);
}

/* power up and set usb suspend */
static inline void usb_power_up(struct akotg_usbhc *otghc)
{
	unsigned long con;
	
	con = __raw_readl(USB_OP_MOD_REG);
	con &= ~(0x7 << 0);
	con |= (3 << 1);
	__raw_writel(con, USB_OP_MOD_REG);
}

#endif
