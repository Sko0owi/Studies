#!/bin/bash

[ -p /tmp/mylog.fifo ] || mkfifo /tmp/mylog.fifo
exec 0< /tmp/mylog.fifo

while true; do
    if read line; then
        echo $(date +"%Y-%m-%d %H-%M-%S") $line
    fi
done