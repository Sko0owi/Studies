for package in $(dpkg-query -f '${Package}\n' -W );
do
    if [ -d "/usr/share/doc/$package" ]; then
        for changelog in $(find /usr/share/doc/$package -maxdepth 1 -iname "changelog.Debian.gz" -print);
        do
            if [ $(zgrep -ivc "Initial release" $changelog) -le 3 ]; then
                echo $changelog
            fi
        done 
        # find /usr/share/doc/$package -maxdepth 1 -iname "changelog.Debian.gz" -exec zgrep -ivc "Initial release" {} \; -print
    fi
done
