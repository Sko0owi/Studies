for file in $(find *.tar.gz)
do    
    folder_name=${file%.tar.gz}
    mkdir $folder_name
    mv $file ./$folder_name/$file

    tar -xf ./$folder_name/$file -C ./$folder_name
    echo $folder_name
    echo $file
done