# rm wyniki.txt
for folder in linux-*;
do   
    echo -n "$folder:" >> wyniki.txt
    cloc --json $folder/$folder.tar.gz | jq -c >> wyniki.txt
    echo $folder
done