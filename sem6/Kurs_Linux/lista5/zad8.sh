exec 1>&4
while read -u 3 BUFFER
do
    case $BUFFER in
        d) date ;;
        q) break ;;
        sys) uname -a ;;
        *) echo 'What?'
    esac
done

# jak podejrzeć komunikacje?
# sudo socat tcp-listen:5050,fork,reuseaddr -
# tylko na to wpadłem, podajemy mu port na którym nasłuchujemy i jak akurat na niego przyjdzie wiadomość to słuchamy

# stworzenie socketu co odpala dany ten skrypt
# socat EXEC:"bash zad8.sh",fdin=3,fdout=4 UNIX-LISTEN:/tmp/mystat.socket

# połączenie się
# socat - UNIX-CONNECT:/tmp/mystat.socket