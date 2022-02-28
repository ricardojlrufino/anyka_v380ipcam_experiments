/*
 *  include/plat-anyka/akmci.h - Anyka MMC/SD driver
 *
 *  Copyright (C) 2010 Anyka, Ltd, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __AK_MCI_H__
#define __AK_MCI_H__

#define MCI_CLK_REG			0x004
#define MMC_CLK_DIVL(x)		((x) & 0xff)
#define MMC_CLK_DIVH(x)		(((x) & 0xff) << 8)
#define MCI_CLK_ENABLE		(1 << 16)
#define MCI_CLK_PWRSAVE		(1 << 17)
#define MCI_FAIL_TRIGGER	(1 << 19)
#define MCI_ENABLE			(1 << 20)

#define MCI_ARGUMENT_REG	0x008
#define MCI_COMMAND_REG		0x00c
#define MCI_CPSM_ENABLE		(1 << 0)
#define MCI_CPSM_CMD(x)		(((x) & 0x3f) << 1)
#define MCI_CPSM_RESPONSE	(1 << 7)
#define MCI_CPSM_LONGRSP	(1 << 8)
#define MCI_CPSM_PENDING	(1 << 9)
#define MCI_CPSM_RSPCRC_NOCHK	(1 << 10)
#define MCI_CPSM_WITHDATA		(1 << 11)

#define MCI_RESPCMD_REG		0x010
#define MCI_RESPONSE0_REG	0x014
#define MCI_RESPONSE1_REG	0x018
#define MCI_RESPONSE2_REG	0x01c
#define MCI_RESPONSE3_REG	0x020
#define MCI_DATATIMER_REG	0x024
#define MCI_DATALENGTH_REG	0x028
#define MCI_DATACTRL_REG	0x02c
#define MCI_DPSM_ENABLE		(1 << 0)
#define MCI_DPSM_DIRECTION	(1 << 1)
#define MCI_DPSM_STREAM		(1 << 2)
#define MCI_DPSM_BUSMODE(x)	(((x) & 0x3) << 3)
#define MCI_DPSM_BLOCKSIZE(x)	(((x) & 0xfff) << 16)

#define MCI_DATACNT_REG		0x030
#define MCI_STATUS_REG		0x034
#define MCI_RESPCRCFAIL		(1 << 0)
#define MCI_DATACRCFAIL		(1 << 1)
#define MCI_RESPTIMEOUT		(1 << 2)
#define MCI_DATATIMEOUT		(1 << 3)
#define MCI_RESPEND			(1 << 4)
#define MCI_CMDSENT			(1 << 5)
#define MCI_DATAEND			(1 << 6)
#define MCI_DATABLOCKEND	(1 << 7)
#define MCI_STARTBIT_ERR	(1 << 8)
#define MCI_CMDACTIVE		(1 << 9)
#define MCI_TXACTIVE		(1 << 10)
#define MCI_RXACTIVE		(1 << 11)
#define MCI_FIFOFULL		(1 << 12)
#define MCI_FIFOEMPTY		(1 << 13)
#define MCI_FIFOHALFFULL	(1 << 14)
#define MCI_FIFOHALFEMPTY	(1 << 15)
#define MCI_DATATRANS_FINISH	(1 << 16)
#define MCI_SDIOINT				(1 << 17)

#define MCI_MASK_REG		0x038
#define MCI_RESPCRCFAILMASK	(1 << 0)
#define MCI_DATACRCFAILMASK	(1 << 1)
#define MCI_RESPTIMEOUTMASK	(1 << 2)
#define MCI_DATATIMEOUTMASK	(1 << 3)
#define MCI_RESPENDMASK		(1 << 4)
#define MCI_CMDSENTMASK		(1 << 5)
#define MCI_DATAENDMASK		(1 << 6)
#define MCI_DATABLOCKENDMASK	(1 << 7)
#define MCI_STARTBIT_ERRMASK	(1 << 8)
#define MCI_CMDACTIVEMASK	(1 << 9)
#define MCI_TXACTIVEMASK	(1 << 10)
#define MCI_RXACTIVEMASK	(1 << 11)
#define MCI_FIFOFULLMASK	(1 << 12)
#define MCI_FIFOEMPTYMASK	(1 << 13)
#define MCI_FIFOHALFFULLMASK	(1 << 14)
#define MCI_FIFOHALFEMPTYMASK	(1 << 15)
#define MCI_DATATRANS_FINISHMASK	(1 << 16)
#define MCI_SDIOINTMASK				(1 << 17)

#define MCI_DMACTRL_REG		0x03c
#define MCI_DMA_BUFEN		(1 << 0)
#define MCI_DMA_ADDR(x)		(((x) & 0x7fff) << 1)
#define MCI_DMA_EN			(1 << 16)
#define MCI_DMA_SIZE(x)		(((x) & 0x7fff) << 17)

#define MCI_FIFO_REG		0x040

#define SDIO_INTRCTR_REG		0x000
#define SDIO_INTR_CTR_ENABLE	(1 << 8)
#define SDIO_INTR_ENABLE		(1 << 17)


#define MCI_CMDIRQMASKS \
	(MCI_CMDSENTMASK|MCI_RESPENDMASK|		\
	 MCI_RESPCRCFAILMASK|MCI_RESPTIMEOUTMASK)

#define MCI_DATAIRQMASKS \
	(MCI_DATAEND|		\
	 MCI_DATACRCFAILMASK|MCI_DATATIMEOUTMASK)

/*|	\
	 MCI_STARTBIT_ERRMASK)
*/

/*
 * The size of the FIFO in bytes.
 */
#define MCI_FIFOSIZE	4
#define MCI_FIFOHALFSIZE (MCI_FIFOSIZE / 2)

#define NR_SG		16

#define L2BASE		0x2002c000
#define L2FIFO_DMACONF	0x80
#define L2FIFO_CONF1	0x88
#define L2FIFO_ASSIGN1	0x90
#define L2FIFO_INTEN	0x9c

#define L2FIFOBASE	0x48000000
#define L2ADDR(n)	(L2FIFOBASE + 512 * (n))
#define MCI_L2FIFO_NUM	2	/* #6 l2fifo */
#define MCI_L2FIFO_SIZE	512

#define L2DMA_MAX_SIZE	(64*255)

#define L2_DMA_ALIGN	(512)

enum akmci_detect_mode {
	AKMCI_PLUGIN_ALWAY,
	AKMCI_DETECT_MODE_GPIO,
	AKMCI_DETECT_MODE_AD,
};

enum akmci_xfer_mode {
	AKMCI_XFER_UNKNOWN,
	AKMCI_XFER_L2DMA,
	AKMCI_XFER_L2PIO,
	AKMCI_XFER_INNERPIO,
	
};

struct ak_mci_platform_data {
	int  irq_cd_type;
	int  cap_highspeed;
	int  detect_mode;
	int  xfer_mode;
	int  mci_mode;
	u32	 max_speed_hz;
	int  bus_width;
    void (* gpio_init) (const struct gpio_info *);
    struct gpio_info gpio_cd;       /* card detect pin */
    struct gpio_info gpio_wp;       /* write protect pin */
	struct gpio_info gpio_pwr;      ///< Power Controller
};

struct clk;

#define MAX_STATUS_COUNT	(1<<10)
#define MAX_STATUS_MASK 	(MAX_STATUS_COUNT - 1)

#define MCI_BUS_WIDTH_1		0
#define MCI_BUS_WIDTH_4		1

#define MCI_MODE_MMC_SD		0
#define MCI_MODE_SDIO		1


/* the 0x800000 value is reference RTOS platform, 
 *	timeout is 419ms */
//#define TRANS_DATA_TIMEOUT	0x800000 //0xffffffff /*0x800000*/
#define TRANS_DATA_TIMEOUT      0x17D7840  //1s 25M


#define MAX_MCI_REQ_SIZE	(65536)
/* as l2 fifo limit to 512 bytes */
#define MAX_MCI_BLOCK_SIZE	(512)

struct power_gpio_attribute {
	struct kobj_attribute k_attr;
	unsigned int gpio;
	unsigned int value;
};

struct akmci_host {
	struct platform_device	*pdev;
	struct ak_mci_platform_data *plat;
	struct mmc_request	*mrq;
	struct mmc_host		*mmc;
	struct mmc_data		*data;
	struct mmc_command	*cmd;
	void __iomem		*base;
	struct resource		*mem;	
	struct clk			*clk;
	unsigned long		asic_clk;
	unsigned char		bus_mode;
	unsigned char		bus_width;
	unsigned long		bus_clock;
	unsigned long		clkreg;
	
	int 		mci_mode;
	int			irq_mci;
	int			irq_cd;
	int			irq_cd_type;
	int			gpio_cd;
	int			gpio_wp;
	int 		detect_mode;
	int			xfer_mode;
	int 		data_err_flag;	
	u8			l2buf_id;
	spinlock_t	lock;
	int			card_status;
	unsigned int		data_xfered;
	unsigned int		sg_len;
	struct scatterlist	*sg_ptr;
	unsigned int		sg_off;
	unsigned int		size;
	
	struct timer_list		detect_timer;
	struct notifier_block	detect_nb;
	int 	plugin_flag;
	wait_queue_head_t	intr_data_wait;
	unsigned int irq_status;
	unsigned long		pending_events;
#ifdef CONFIG_CPU_FREQ
	struct notifier_block	freq_transition;
	struct semaphore freq_lock; 
#endif

	struct power_gpio_attribute power;
};



#endif	/* end __AK_MCI_H__ */

