 
IPCAM_MODEL=V380E2_C2

HASH=`./patchv380 write exshell_bfu.sh exshell_afu.sh -h ${IPCAM_MODEL}`

# Move path file to SD
mv ${HASH}.patch COPY_TO_SD/updatepatch

## Generate .conf file
cat <<EOT > COPY_TO_SD/local_update.conf
[PATCH]
patchmd5=${HASH}
EOT

# show
tree ./COPY_TO_SD

