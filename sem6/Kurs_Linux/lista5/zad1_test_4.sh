exec 3< wielowierszowy.txt
exec 4> logs.txt
bash zad1.sh 1 <&3 1>&4 &
bash zad1.sh 1.7 <&3 1>&4