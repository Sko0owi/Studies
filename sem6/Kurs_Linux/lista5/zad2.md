# ls
jaki jest ls każdy widzi uwu
Ale no listuje nam pliki w katalogu i dodatkowo może mieć jakieś basic info pokroju bity dostępu, kto stworzył, jaki to typ pliku, rozmiar.
# stat
dokładniejsze informacje o pliku. Fajny formater (coś jak w date). A informacje jakie dostajemy to bity dostępu, co to za typ pliku, jaki INODE. Ile bloków jest zaalokowanych. Daty modyfikacji, dostępu, powstania
# realpath
podaje nam dokładną ścieżkę do pliku (przy okazji rozwija linki symboliczne). Oczywiście możemy mu podać dokiedy może rozwijać
# readlink 
taki słabszy realpath ;pp tylko do symlinków i nawet w MAN'ie jest napisane że halo halo lepszy realpath
# dirname
stripuje ostatni komponent z nazwy pliku
(dokładniej to wypisze coś do pierwszego '/')
# basename
odwrotność dirname (zostawia tylko ostatnią część z ścieżki) oraz możemy podać by usuwał podany suffix 
