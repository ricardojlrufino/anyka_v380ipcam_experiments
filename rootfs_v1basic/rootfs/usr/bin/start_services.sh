 #!/bin/sh

echo "###### Start network services ..."

echo "Start telnet"
busybox telnetd -p 23 -l /bin/sh &


echo "Start FTP"
/usr/bin/tcpsvd 0 21 ftpd -w / -t 600 &
