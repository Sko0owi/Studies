for package in $(dpkg-query -f '${Package}\n' -W );
do
    if [ ! -d "/usr/share/doc/$package" ]; then
        echo $package
    fi
done
