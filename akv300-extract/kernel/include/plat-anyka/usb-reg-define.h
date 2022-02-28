#ifndef _ANYKA_USB_REG_DEFINE_H_
#define _ANYKA_USB_REG_DEFINE_H_

 /** Control Registers*/
#define USB_REG_FADDR           (0x0000)    // 8-bit
#define USB_REG_POWER           (0x0001)    // 8-bit
#define USB_REG_INTRTX          (0x0002)    // 16-bit, read clear
#define USB_REG_INTRRX          (0x0004)    // 16-bit, read clear
#define USB_REG_INTRTXE         (0x0006)    // 16-bit
#define USB_REG_INTRRXE         (0x0008)    // 16-bit
#define USB_REG_INTRUSB         (0x000A)    // 8-bit, read clear
#define USB_REG_INTRUSBE        (0x000B)    // 8-bit
#define USB_REG_FRAME           (0x000C)    // 16-bit
#define USB_REG_INDEX           (0x000E)    // 8-bit
#define USB_REG_TESEMODE        (0x000F)    // 8-bit
#define USB_REG_DEVCTL          (0x0060)    // 8-bit
	  
/** Endpoint Control/Status Registers */
#define USB_REG_TXMAXP          (0x0010)    // 16-bit, when index == 1~4
#define USB_REG_TXCSR1          (0x0012)    // 8-bit, when index == 1~4
#define USB_REG_TXCSR2          (0x0013)    // 8-bit, when index == 1~4
#define USB_REG_RXMAXP          (0x0014)    // 16-bit, when index == 1~4
#define USB_REG_RXCSR1          (0x0016)    // 8-bit, when index == 1~4
#define USB_REG_RXCSR2          (0x0017)    // 8-bit, when index == 1~4
#define USB_REG_COUNT0          (0x0018)    // 16-bit, when index == 0
#define USB_REG_RXCOUNT         (0x0018)    // 16-bit, when index == 1~4
#define USB_REG_TXTYPE          (0x001A)    // 8-bit, when index == 1~4,    host only
#define USB_REG_NAKLIMIT0       (0x001B)    // 8-bit, when index == 0,      host only
#define USB_REG_TXINTERVAL      (0x001B)    // 8-bit, when index == 1~4,    host only
#define USB_REG_RXTYPE          (0x001C)    // 8-bit, when index == 1~4,    host only
#define USB_REG_RXINTERVAL      (0x001D)    // 8-bit, when index == 1~4,    host only
#define USB_REG_CONFIGDATA      (0x001F)    // 8-bit, when index == 0
#define USB_REG_FIFOSIZE        (0x001F)    // 8-bit, when index == 1~4
 
/**  USB DMA */                                         
#define USB_DMA_INTR                                    (0x0200)
#define USB_DMA_CTRL_BASE                               (0x0204)
#define USB_DMA_ADDR_BASE                               (0x0208)
#define USB_DMA_COUNT_BASE                              (0x020c)
#define USB_DMA_CTRL(n)                                 (USB_DMA_CTRL_BASE+(n)*0x10)
#define USB_DMA_ADDR(n)                                 (USB_DMA_ADDR_BASE+(n)*0x10)
#define USB_DMA_COUNT(n)                                (USB_DMA_COUNT_BASE+(n)*0x10)

#define USB_REG_REQPKTCNT_BASE                           (0x0304)
#define USB_REG_REQPKTCNT(ep)                            (USB_REG_REQPKTCNT_BASE + (ep-1)*0x4)

/** FIFOs Entry */
#define USB_FIFO_EP0            (0x0020)    // 8- / 16- / 32-bit
#define USB_FIFO_EP1            (0x0024)    // 8- / 16- / 32-bit
#define USB_FIFO_EP2            (0x0028)    // 8- / 16- / 32-bit
#define USB_FIFO_EP3            (0x002C)    // 8- / 16- / 32-bit
#define USB_FIFO_EP4            (0x0030)    // 8- / 16- / 32-bit
#define USB_FIFO_EP5            (0x0034)    // 8- / 16- / 32-bit
 
/*DMA controler registers.*/
#define DMA_INTR_STAT	0x0		/*DMA interrupt status.*/

#define DMA_CTRL_REG1	0x04	/*DMA channel 1 control.*/
#define DMA_CTRL_REG2	0x14	/*DMA channel 2 control.*/ 
#define DMA_CTRL_REG3	0x24	/*DMA channel 3 control.*/
#define DMA_CTRL_REG4	0x34	/*DMA channel 4 control.*/

#define DMA_ADDR_REG1	0x08	/*DMA channel 1 AHB memory address.*/
#define DMA_ADDR_REG2	0x18	/*DMA channel 2 AHB memory address.*/
#define DMA_ADDR_REG3	0x28	/*DMA channel 3 AHB memory address.*/
#define DMA_ADDR_REG4	0x38	/*DMA channel 4 AHB memory address.*/

#define DMA_CUNT_REG1	0x0c	/*DMA channel 1 byte count.*/
#define DMA_CUNT_REG2	0x1c	/*DMA channel 2 byte count.*/
#define DMA_CUNT_REG3	0x2c	/*DMA channel 3 byte count.*/
#define DMA_CUNT_REG4	0x3c	/*DMA channel 4 byte count.*/


//********************** bit fields defs***********************

#define USB_ENABLE_DMA                                  (1)
#define USB_DIRECTION_RX                                (0<<1)
#define USB_DIRECTION_TX                                (1<<1)
#define USB_DMA_MODE1                                   (1<<2)
#define USB_DMA_MODE0                                   (0<<2)
#define USB_DMA_INT_ENABLE                              (1<<3)
#define USB_DMA_INT_DISABLE                             (0<<3)
#define USB_DMA_BUS_ERROR                               (1<<8)
#define USB_DMA_BUS_MODE0                               (0<<9)
#define USB_DMA_BUS_MODE1                               (1<<9)
#define USB_DMA_BUS_MODE2                               (2<<9)
#define USB_DMA_BUS_MODE3                               (3<<9)

// RTC_USB_CONFIG_REG
#define SESSEND             (1 << 31) // 0:above/1:below Session End threshold
#define AVALID              (1 << 30)
#define VBUSVALID           (1 << 29)
#define SESSEND_SEL         (1 << 28)
#define AVALID_SEL          (1 << 27)
#define VBUSVALID_SEL       (1 << 26)

 
/** POWER Control register  */
#define USB_POWER_ENSUSPEND     (1 << 0)
#define USB_POWER_SUSPENDM      (1 << 1)    // host/client
#define USB_POWER_RESUME        (1 << 2)    // host/client
#define USB_POWER_RESET         (1 << 3)
#define USB_HOSG_HIGH_SPEED		(1 << 5)
 
	  /** CSR01 register */
	  // mode-agnostic 
#define USB_CSR01_RXPKTRDY           (1 << 0)   // r / clear
#define USB_CSR01_TXPKTRDY           (1 << 1)   // r / set
	  // Client mode
#define USB_CSR01_P_SENTSTALL        (1 << 2)
#define USB_CSR01_P_DATAEND          (1 << 3)
#define USB_CSR01_P_SETUPEND         (1 << 4)
#define USB_CSR01_P_SENDSTALL        (1 << 5)
#define USB_CSR01_P_SVDRXPKTRDY      (1 << 6)
#define USB_CSR01_P_SVDSETUPEND      (1 << 7)
	  // Host mode
#define USB_CSR01_H_RXSTALL          (1 << 2)   // r / clear
#define USB_CSR01_H_SETUPPKT         (1 << 3)   // r / w
#define USB_CSR01_H_ERROR            (1 << 4)   // r / clear
#define USB_CSR01_H_REQPKT           (1 << 5)   // r / w
#define USB_CSR01_H_STATUSPKT        (1 << 6)   // r / w
#define USB_CSR01_H_NAKTIMEOUT       (1 << 7)   // r / clear
	  
	  /** CSR02 register */
	  // mode-agnostic 
#define USB_CSR02_FLUSHFIFO          (1 << 0)   // set
	  // Client mode (none)
	  // Host mode
#define USB_CSR02_H_DISPING          (1 << 3)   // r / w


	  /** TXCSR1 register */
	  // mode-agnostic
#define USB_TXCSR1_TXPKTRDY         (1 << 0)
#define USB_TXCSR1_FIFONOTEMPTY     (1 << 1)
#define USB_TXCSR1_FLUSHFIFO        (1 << 3)
#define USB_TXCSR1_CLRDATATOG       (1 << 6)
	  // Client mode
#define USB_TXCSR1_P_UNDERRUN       (1 << 2)
#define USB_TXCSR1_P_SENDSTALL      (1 << 4)
#define USB_TXCSR1_P_SENTSTALL      (1 << 5)
#define USB_TXCSR1_P_INCOMPTX       (1 << 7)
	  // Host MODE
#define USB_TXCSR1_H_ERROR          (1 << 2)
#define USB_TXCSR1_H_RXSTALL        (1 << 5)
#define USB_TXCSR1_H_NAKTIMEOUT     (1 << 7)    // for Bulk Endpoint
#define USB_TXCSR1_H_INCOMPTX       (1 << 7)    // for Interrupt Endpoint
	  
	  /** TXCSR2 register */
	  // mode-agnostic
#define USB_TXCSR2_DMAMODE1         (1 << 2)
#define USB_TXCSR2_FRCDATATOG       (1 << 3)
#define USB_TXCSR2_DMAENAB          (1 << 4)
#define USB_TXCSR2_MODE             (1 << 5)
	  // Host mode
#define USB_TXCSR2_MODE_TX          (1 << 5)
#define USB_TXCSR2_AUTOSET          (1 << 7)
	  // Client mode
#define USB_TXCSR2_P_ISO            (1 << 6)
 
	  /** RXCSR1 register */
	  // mode-agnostic
#define USB_RXCSR1_RXPKTRDY         (1 << 0)    // r / clear
#define USB_RXCSR1_FIFOFULL         (1 << 1)    // r
#define USB_RXCSR1_DATAERR          (1 << 3)    // r / clear(host), for ISO only
#define USB_RXCSR1_FLUSHFIFO        (1 << 4)    // set
#define USB_RXCSR1_CLRDATATOG       (1 << 7)    // set
	  // Client mode
#define USB_RXCSR1_P_OVERRUN        (1 << 2)    // r / clear, for ISO only
#define USB_RXCSR1_P_SENDSTALL      (1 << 5)    // r / w
#define USB_RXCSR1_P_SENTSTALL      (1 << 6)    // r / clear
	  // Host MODE 
#define USB_RXCSR1_H_ERROR          (1 << 2)    // r / clear
#define USB_RXCSR1_H_NAKTIMEOUT     (1 << 3)    // r / clear, for BULK
#define USB_RXCSR1_H_REQPKT         (1 << 5)    // r / w
#define USB_RXCSR1_H_RXSTALL        (1 << 6)    // r / clear
	  
	  /** RXCSR2 register */
	  // mode-agnostic
#define USB_RXCSR2_INCOMPRX         (1 << 0)    // r
#define USB_RXCSR2_DMAMODE0         (0 << 3)
#define USB_RXCSR2_DMAMODE1         (1 << 3)    // r / w
#define USB_RXCSR2_DMAREQENAB       (1 << 5)    // r / w
#define USB_RXCSR2_AUTOCLEAR        (1 << 7)    // r / w

#define USB_RXCSR2_AUTOREQ			(1 << 6)
#define USB_RXCSR2_DMAENAB			(1 << 5)
#define USB_RXCSR2_DISNYET			(1 << 4)
#define USB_RXCSR2_DMAMODE			(1 << 3)

	  // Client mode
#define USB_RXCSR2_P_DISNYET        (1 << 4) // for Bulk/Interrupt transaction
#define USB_RXCSR2_P_PIDERROR       (1 << 4) // for ISO transaction
#define USB_RXCSR2_P_ISO            (1 << 6)
	  // Host Mode
#define USB_RXCSR2_H_PIDERROR       (1 << 4)    // r / w, for ISO only
#define USB_RXCSR2_H_AUTOREQ        (1 << 6)    // r / w


	  /** IntrUSB/IntrUSBE register */
#define USB_INTR_SUSPEND        (1 << 0)    // client
#define USB_INTR_RESUME         (1 << 1)
#define USB_INTR_RESET          (1 << 2)    // client
#define USB_INTR_BABBLE         (1 << 2)    // host
#define USB_INTR_SOF            (1 << 3)
#define USB_INTR_CONNECT        (1 << 4)    // host
#define USB_INTR_DISCONNECT     (1 << 5)    // host/client
#define USB_INTR_SESSREQ        (1 << 6)    // A device
#define USB_INTR_VBUSERROR      (1 << 7)    // A device

 
 /** IntrTX register */
 /** IntrRX register */
 /** IntrTXE register */
 /** IntrRXE register */
#define USB_INTR_EP0          (1 << 0)
#define USB_INTR_EP1          (1 << 1)
#define USB_INTR_EP2          (1 << 2)
#define USB_INTR_EP3          (1 << 3)
#define USB_INTR_EP4          (1 << 4)
#define USB_INTR_EP5          (1 << 5)
#define USB_INTR_EPX_MASK     (USB_INTR_EP1|USB_INTR_EP2|USB_INTR_EP3|USB_INTR_EP4|USB_INTR_EP5)
 
 /** IntrUSB/IntrUSBE register */
#define USB_INTR_SUSPEND        (1 << 0)    // client
#define USB_INTR_RESUME         (1 << 1)
#define USB_INTR_RESET          (1 << 2)    // client
#define USB_INTR_BABBLE         (1 << 2)    // host
#define USB_INTR_SOF            (1 << 3)
#define USB_INTR_CONNECT        (1 << 4)    // host
#define USB_INTR_DISCONNECT     (1 << 5)    // host/client
#define USB_INTR_SESSREQ        (1 << 6)    // A device
#define USB_INTR_VBUSERROR      (1 << 7)    // A device
 
 /** RXMAXP register */
#define USB_RXMAXP_MASK             (0x07FF)
#define USB_RXMAXP(cnt)             ((cnt) & 0x07FF)
 
 /** TXTYPE register */
#define USB_TXTYPE_EPNUM_MASK       (0xf << 0)
#define USB_TXTYPE_EPNUM(ep)        (((ep)&0xf) << 0)
#define USB_TXTYPE_PROTOCOL_ILLEGAL (0 << 4)
#define USB_TXTYPE_PROTOCOL_ISO     (1 << 4)
#define USB_TXTYPE_PROTOCOL_BULK    (2 << 4)
#define USB_TXTYPE_PROTOCOL_INTR    (3 << 4)
 
 /** DevCtl register */
#define USB_DEVCTL_SESSION          (1 << 0)    // host/client
#define USB_DEVCTL_HOSTREQ          (1 << 1)    // B device
#define USB_DEVCTL_HOSTMODE_MASK    (1 << 2)
 
#define USB_DEVCTL_HOSTMODE_HOST    (1 << 2)
#define USB_DEVCTL_VBUS_MASK        (3 << 3)

#define USB_DEVCTL_LSDEV			(1 << 5)
#define USB_DEVCTL_FSDEV            (1 << 6)    // host
#define USB_DEVCTL_BDEVICE_MASK     (1 << 7)

#define USB_DEVCTL_BDEVICE_B        (1 << 7)

#endif	/* _ANYKA_USB_REG_DEFINE_H_ */

