for package in $(find /usr/share/doc/ -maxdepth 1 -type d);
do
    package_name=$(basename $package)
    if ! dpkg-query -f '${Package}\n' -W | grep -qi "^$package_name$"; then
        echo $package_name
    fi
    
done
