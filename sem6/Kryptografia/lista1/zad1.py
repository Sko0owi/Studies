import matplotlib.pyplot as plt




TEST =     ('CBAGDIHMEAMSEHILTIWYSIWETNMPHBGLATREDFTIRPUEN'
            'KPNHANBADGREARSEAVENISRIVETNOIENFCEUENIYTIFEA'
            'DRMPNOESNIYREARLTANNANADGMIRGNADRHIRAGCMIPLOH'
            'A WOEESMATTI WHIOESFCDUSEATCDLCHPFCOAVEASWAGNSI'
            'VERADRTENFEUHERHOE WOEESMATTI WCHCNHOEIDEWOEESE'
            'RVEOCUSEIYWOCUOCABFETYEUHBANHET')
# T,H,E,W
TEST =     ('I MAY NOT BE ABLE TO GROW FLOWERS BUT MY GARDEN PRODUCES'
            'JUST AS MANY DEAD LEAVES OLD OVERSHOES PIECES OF ROPE AND'
            'BUSHELS OF DEAD GRASS AS ANYBODYS AND TODAY I BOUGHT A WHEELBARROW'
            'TO HELP IN CLEARING IT UP I HAVE ALWAYS LOVED AND RESPECTED'
            'THE WHEEL BARROW IT IS THE ONE WHEELED VEHICLE OF WHICH I AM PERFECT MASTER')


toDecode = ('EMGLOSUDCGDNCUSWYSFHNSFCYKDPUMLWGYICOXYSIPJCK'
            'QPKUGKMGOLICGINCGACKSNISACYKZSCKXECJCKSHYSXCG'
            'OIDPKZCNKSHICGIWYGKKGKGOLDSILKGOIUSIGLEDSPWZU'
            'GFZCCNDGYYSFUSZCNXEOJNCGYEOWEUPXEZGACGNFGLKNS'
            'ACIGOIYCKXCJUCIUZCFZCCNDGYYSFEUEKUZCSOCFZCCNC'
            'IACZEJNCSHFZEJZEGMXCYHCJUMGKUCY')

charCnt = {}
for char in toDecode:
    if char in charCnt:
        charCnt[char] += 1
    else:
        charCnt[char] = 1

wzor = {'E': 220, 'A': 140, 'O': 139, 'S': 123, 'R': 122, 'D': 113, 'T': 106, 'L': 89, 'H': 87, 'I': 74, 'N': 62, 'W': 58, 'B': 58, 'Y': 39, 'P': 37, 'C': 37, 'U': 36, 'M': 30, 'G': 30, 'F': 21, 'V': 14, 'J': 8, 'X': 3, 'K': 2, 'Q': 2, 'Z': 1}
x = sorted(charCnt.items(), key = lambda x: x[1], reverse=True)

print(x)

names = []
values = []

names_wzor = []
values_wzor = []

for item in x:
    names.append(item[0])
    values.append(item[1])

y = sorted(wzor.items(), key = lambda x: x[1], reverse=True)

print(dict(y))

for item in y:
    names_wzor.append(item[0])
    values_wzor.append(item[1])

key = {}


for i in range(len(x)):
    if names[i] not in key:
        key[names[i]] = names_wzor[i]
    
print(key)

decoded = ""
for char in toDecode:
    decoded += key[char]

print(decoded)

plt.subplot(131)
plt.bar(names, values)
plt.subplot(132)
plt.bar(names_wzor, values_wzor)
plt.show()

print(sorted(charCnt.items(), key = lambda x: x[1], reverse=True))

print(toDecode)