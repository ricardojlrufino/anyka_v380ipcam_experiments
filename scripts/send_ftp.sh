 #!/bin/sh


function send_ftp (){
TARGET_DEVICE=$1
TARGET_BIN=$2
TARGET_PATH=$3

### SEND USING FTP
ftp ${TARGET_DEVICE} <<EOF 
pwd
cd ${TARGET_PATH}
ls
put ${TARGET_BIN}
chmod +x ${TARGET_BIN}
ls
EOF
###
}


