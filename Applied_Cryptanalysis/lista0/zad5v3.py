





wzor = {
    'A' :8.91,  
    'Ą' :0.79, 
    'B' :1.93,  
    'C' :3.89,  
    'Ć' :0.60,  
    'D' :3.35,  
    'E' :8.68,  
    'Ę' :1.13,  
    'F' :0.26,  
    'G' :1.46,  
    'H' :1.25,  
    'I' :8.83,  
    'J' :2.28,  
    'K' :3.01,  
    'L' :2.24,  
    'Ł' :2.38,  
    'M' :2.81,  
    'N' :5.69,  
    'Ń' :0.16,  
    'O' :7.75,  
    'Ó' :0.79,  
    'P' :2.87,  
    'R' :4.15,  
    'S' :4.13,  
    'Ś' :0.72,  
    'T' :3.85,  
    'U' :2.06,  
    'W' :4.11,  
    'Y' :4.03,  
    'Z' :5.33, 
    'Ź' :0.08,  
    'Ż' :0.93,  
}

ppb = 0

for ee in wzor.items():
    print(ee)
    ppb += (ee[1]/100)*(ee[1]/100)

print(len(wzor))
print(ppb*len(wzor))
