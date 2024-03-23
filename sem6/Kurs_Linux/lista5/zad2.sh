for IMG in *.jpg # weź wszystkie pliki .jpg
do
    convert $IMG $(basename $IMG .jpg).png 
    # zamień $IMG na $IMG  bez suffixu .jpg i dodaj suffix .png
done