# /etc/hostname
przechowuje nazwe lokalną naszego systemu
# /etc/machine-id
unikalne ID maszyny nadane po pierwszym bootcie. Jest to ID po którym możemy zostać rozpoznani i nie powinniśmy go rozpowszechniać. 
# /etc/os-release
jaką mamy wersję systemu operacyjnego i jakiś bs o tym skąd pobrane itp
# /etc/debian_version
jaka wersja debiana
# hostname
jak nie podamy argumentu to podamy to co jest w /etc/hostname. Możemy też podając jako argument nazwę zmienić hostname'a. Możemy też podejrzeć fsqn ALE nie zmienić
# hostnamectl
Dodatkowe narzędzie do modyfikacji hostname'a. Są jakieś dodatkowe opcje pokroju chassis i icon-name
# uuid
Universally Unique Identifier. Identyfikator dla naszego urządzenia. U mnie każde z kolejnych wywołań tworzy nowy (nie mam uuidgen)
# dbus-uuidgen
Totalnie inne uuid od tego wyżej, ono ma się zmieniać tylko po reboocie i jest wykorzystywane do innych celów. Ma jakiś rodzaj identyfikacji 
# uname
Printuje informacje o systemie.
Nazwa kernela
wersja kernela
jaki mamy system, procesor itp
uname -a for the win
# lsb-release
print distribution-specific information
Informacje o systemie dla starszych aplikacji.
