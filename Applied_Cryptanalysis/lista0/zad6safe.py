import matplotlib.pyplot as plt

alphabet = '*ABCDEFGHIJKLMNOPQRSTUVWXYZ'
alphalen = len(alphabet)



def encode(plaintext, key):
    alphabet = list(key)
    alphabet.sort()
    encdic = dict(zip(alphabet, key))
    return ''.join(map(lambda c: encdic[c], plaintext))

# def decode(ciphertext, key):
#     keylen = len(key)
#     plaintext = [decodeChar(ciphertext[n], key[n%keylen]) for n in range(len(ciphertext))]
#     return ''.join(plaintext)

charCnt = {}

toDecode = ('LPMUQEMQMGKLWDPMCYRHMHQXMLBMQSKLRMQBHMPDVMCRYCJEMUVKVMEPKLJLBWMPDLKPVVBMULBEPYBM'
            'EILPDMDLEMCDLBMBAZZRVHMLBPYMDLEMGKVQEPMLBMQBMVOOYKPMPYMVECQSVMPDVM*LRVMULBHMERLS'
            'SVHMTALCJRXMPDKYAWDMPDVMWRQEEMHYYKEMYOM*LCPYKXMIQBELYBEMPDYAWDMBYPMTALCJRXMVBYAW'
            'DMPYMSKV*VBPMQMEULKRMYOMWKLPPXMHAEPMOKYIMVBPVKLBWMQRYBWMULPDMDLIMPDVMDQRRUQXMEIV'
            'RPMYOMGYLRVHMCQGGQWVMQBHMYRHMKQWMIQPEMQPMYBVMVBHMYOMLPMQMCYRYAKVHMSYEPVKMPYYMRQK'
            'WVMOYKMLBHYYKMHLESRQXMDQHMGVVBMPQCJVHMPYMPDVMUQRRMLPMHVSLCPVHMELISRXMQBMVBYKIYAE'
            'MOQCVMIYKVMPDQBMQMIVPKVMULHVMPDVMOQCVMYOMQMIQBMYOMQGYAPMOYKPXMOL*VMULPDMQMDVQ*XM'
            'GRQCJMIYAEPQCDVMQBHMKAWWVHRXMDQBHEYIVMOVQPAKVEMULBEPYBMIQHVMOYKMPDVMEPQLKEMLPMUQ'
            'EMBYMAEVMPKXLBWMPDVMRLOPMV*VBMQPMPDVMGVEPMYOMPLIVEMLPMUQEMEVRHYIMUYKJLBWMQBHMQPM'
            'SKVEVBPMPDVMVRVCPKLCMCAKKVBPMUQEMCAPMYOOMHAKLBWMHQXRLWDPMDYAKEMLPMUQEMSQKPMYOMPD'
            'VMVCYBYIXMHKL*VMLBMSKVSQKQPLYBMOYKMDQPVMUVVJMPDVMORQPMUQEMEV*VBMORLWDPEMASMQBHMU'
            'LBEPYBMUDYMUQEMPDLKPXMBLBVMQBHMDQHMQM*QKLCYEVMARCVKMQGY*VMDLEMKLWDPMQBJRVMUVBPME'
            'RYURXMKVEPLBWMEV*VKQRMPLIVEMYBMPDVMUQXMYBMVQCDMRQBHLBWMYSSYELPVMPDVMRLOPMEDQOPMP'
            'DVMSYEPVKMULPDMPDVMVBYKIYAEMOQCVMWQZVHMOKYIMPDVMUQRRMLPMUQEMYBVMYOMPDYEVMSLCPAKV'
            'EMUDLCDMQKVMEYMCYBPKL*VHMPDQPMPDVMVXVEMOYRRYUMXYAMQGYAPMUDVBMXYAMIY*VMGLWMGKYPDV'
            'KMLEMUQPCDLBWMXYAMPDVMCQSPLYBMGVBVQPDMLPMKQBM')

for char in toDecode:
    if char in charCnt:
        charCnt[char] += 1
    else:
        charCnt[char] = 1

wzor = {' ': 220, 'E': 196, 'T': 140, 'I': 139, 'A': 123, 'N': 122, 'O': 113, 'R': 106, 'S': 89, 'H': 87, 'C': 74, 'L': 62, 'D': 58, 'M': 58, 'G': 39, 'F': 37, 'U': 37, 'P': 36, 'B': 30, 'W': 30, 'Y': 21, 'V': 14, 'K': 8, 'X': 3, 'J': 2, 'Q': 2, 'Z': 1}
x = sorted(charCnt.items(), key = lambda x: x[1], reverse=True)

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