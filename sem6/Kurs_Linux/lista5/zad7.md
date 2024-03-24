# netstat i ss 
oba powstały do ogarniania socketów 
ss jest nowszy i ma więcej wyspecyfikowanych rzeczy od netstata
# netstat
możemy podejrzeć tablice routingu 
wszystkie interfejsy 
statystyczny bullshit

# ss 
głównie gniazdowe pitu pitu

# lsof -U 
wszystko co może lsof ale na socketach

# jak dowiedzieć się który proces utworzył gniazdo

netstat -a 
ss -e -a
???

# jak dowiedzieć się które gniazda należą do podanego procesu

lsof -U -p <PID>