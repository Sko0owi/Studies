for folder in linux-*;
do   
    mv $folder/$folder.tar.gz .
    rm -rf $folder 
    mkdir $folder
    mv $folder.tar.gz $folder
done