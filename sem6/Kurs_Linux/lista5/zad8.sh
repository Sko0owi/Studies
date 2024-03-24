while true; do
    if read line; then
        echo $(date +"%Y-%m-%d %H-%M-%S") $line
    fi
done | nc -lU -vv /var/tmp/dsocket