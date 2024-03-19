for package in $(dpkg-query -f '${Package}\n' -W );
do
    if [ -d "/usr/share/doc/$package" ]; then
        if ! find "/usr/share/doc/$package/" -iname "changelog*" -print -quit | grep -q . ; then
            echo "$package"
        fi
    fi
done
