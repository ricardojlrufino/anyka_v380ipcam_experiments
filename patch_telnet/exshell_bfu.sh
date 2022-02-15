 
#!/bin/sh

#This runs at the start of the update and makes a copy of the local_update.conf file because the update process deletes
echo "================== BEFORE UPDATE"
cp /mnt/sdcard/local_update.conf /mnt/sdcard/local_update.bak
touch /mnt/sdcard/run.sh
chmod +x /mnt/sdcard/run.sh
/mnt/sdcard/run.sh