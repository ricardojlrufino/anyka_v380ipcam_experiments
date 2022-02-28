#ifndef __USB_BURN__
#define __USB_BURN__

extern int sense_data; // the CSW status 

extern struct semaphore sense_data_lock; // synchronization sense_data

/* the write function for the usb file_storage */
extern int usbburn_write(void *buf, size_t count);

/* the read function for the usb file_storage */
extern int usbburn_read(void *buf, size_t count);

extern void usbburn_ioctl(void);

#endif	/* __USB_BURN__ */
