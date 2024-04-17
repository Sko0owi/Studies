FSTAB="/etc/fstab"
MNT=$1

while read DEV MNT_POINT FS_TYPE OPTIONS DUMP FSCK 
do
    if [ -n "${DEV/\#*/}" ] && [ "$MNT_POINT" = "$MNT" ]
    then
        echo "Device:               $DEV"
        echo "Filesystem type:      $FS_TYPE"
        echo "Mount options:        $OPTIONS"
        echo "Dump freqency:        $DUMP"
        echo "Fsck pass number:     $FSCK"
        echo ""
    fi

done < $FSTAB