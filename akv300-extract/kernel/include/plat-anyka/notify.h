/*
 * include/plat-anyka/notify.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef __NOTIFY_H_
#define __NOTIFY_H_

#define POWER_EVENT_AC_PLUGIN		0x01
#define POWER_EVENT_AC_PLUGOUT		0x02
#define POWER_EVENT_USB_PLUGIN		0x03
#define POWER_EVENT_USB_PLUGOUT		0x04

#define ADDEDECT_DEV1_PLUGIN		0x10
#define ADDEDECT_DEV1_PLUGOUT		0x11
#define ADDEDECT_DEV2_PLUGIN		0x12
#define ADDEDECT_DEV2_PLUGOUT		0x13
#define ADDEDECT_DEV3_PLUGIN		0x14
#define ADDEDECT_DEV3_PLUGOUT		0x15
#define ADDEDECT_DEV4_PLUGIN		0x16
#define ADDEDECT_DEV4_PLUGOUT		0x17

int power_register_client(struct notifier_block *nb);
int power_unregister_client(struct notifier_block *nb);
int power_notifier_call_chain(unsigned long val, void *v);

int addetect_register_client(struct notifier_block *nb);
int addetect_unregister_client(struct notifier_block *nb);
int addetect_notifier_call_chain(unsigned long val, void *v);

#endif /* __NOTIFY_H_ */

