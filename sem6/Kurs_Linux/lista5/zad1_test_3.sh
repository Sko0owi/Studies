exec 4> logs.txt
bash zad1.sh 1 < wielowierszowy.txt 1>&4 &
bash zad1.sh 1.7 < wielowierszowy.txt 1>&4