find / \
    -type l \
    -exec file {} + \
    2>/dev/null \
| grep -E 'symbolic link to .*\.so(\.[0-9]+)*$' \
| awk -F: '{print $1}'