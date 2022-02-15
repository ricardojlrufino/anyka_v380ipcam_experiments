 
#!/bin/sh
{
sleep 30
# revert files to allow run next time
mv /mnt/sdcard/local_update.bak /mnt/sdcard/local_update.conf
rm -f /mnt/sdcard/update_record
} &