Timeout=${1:-1}

while read Line; do
    echo $Line
    sleep $Timeout
done