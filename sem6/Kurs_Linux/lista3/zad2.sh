#!/bin/bash

mount_point=$1

if [[ -z $mount_point ]]; then
    mount_point="."
fi 

IFS=$'\n'
result=$(grep -E "^UUID=[0-9a-zA-Z\-]+[ ]*$mount_point([/]*)[ ]+.*$" /etc/fstab)

for line in "$result"
do 
    IFS=$' '
    parse=($line)
    echo "Device:               ${parse[ 0 ]}"
    echo "Filesystem type:      ${parse[ 2 ]}"
    echo "Mount options:        ${parse[ 3 ]}"
    echo "Dump freqency:        ${parse[ 4 ]}"
    echo "Fsck pass number:     ${parse[ 5 ]}"
    echo ""
done


