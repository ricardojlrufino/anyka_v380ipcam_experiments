#ifndef SRC_APP_SETTINGS_H_
#define SRC_APP_SETTINGS_H_

#define LED_CHANGE 1 // Change led on detect QRCODE

// /oldcam/usr/isp
const char *cfg = "/mnt/sdcard/CAM/isp";			//isp config file storage path
const char *yuv_path = "/mnt/sdcard/CAM/record/";	//yuv save path

#define SNAPSHOT_ON_ERROR 1
#define SNAPSHOT_DEFAULT_DIR "/mnt/sdcard/CAM/record"

#define DEBUG_SNAPSHOT 1  			// save snapshot
#define DEBUG_SNAPSHOT_MULTIPLE 0  // if need capture snapshot as series of images.



#endif /* SRC_APP_SETTINGS_H_ */
