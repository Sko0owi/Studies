find / \
    -type f \
    -regextype posix-extended \
    -regex '.*\.so(\.[0-9]+)*$' \
    -print \
    2>/dev/null