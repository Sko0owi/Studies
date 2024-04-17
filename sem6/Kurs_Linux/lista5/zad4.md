# fuser
fuser pokazuje pidy procesów które obecnie korzystają z danego pliku albo filesystemu. Możemy też np zabić procesy które spełniają nasz warunek

# lsof 
pokazuje nam wszystkie otwarte pliki przez podane procesy 

# wszystkie otwarte pliki przez podany proces
lsof -p <PID>

# który proces otworzył dany plik
fuser -m <file_name>

# wszystkie otwarte pliki przez podanego użytkownika

lsof -u <USER>