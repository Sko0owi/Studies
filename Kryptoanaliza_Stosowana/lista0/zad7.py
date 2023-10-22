import matplotlib.pyplot as plt

alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
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

toDecode = ('KMFXLZMKDZVOXLZPXKYIPBKRRZFSVYDSVVFKMFXLZXLIYFAKYXVOXLZSKXZYPQZBKDZSVYDSVVFKMFDK'
            'MCDZMFIZFVOXLZSKXZYPQZBKNPZXLZCSZYZDKFZQIXXZY')

for char in toDecode:
    if char in charCnt:
        charCnt[char] += 1
    else:
        charCnt[char] = 1

wzor = {'E': 196, 'A': 140, 'T': 139, 'D': 123, 'O': 122, 'R': 113, 'H': 106, 'M': 89, 'W': 87, 'N': 74, 'S': 62, 'I': 58, 'F': 58, 'C': 39, 'B': 37, 'G': 37, 'Y': 36, 'P': 30, 'U': 30, 'L': 21, 'V': 14, 'K': 8, 'Z': 3, 'Q': 2, 'J': 2, 'X': 1}
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

WOW = "AND THE NAME OF THE STAR IS CAGGED WORMWOOD AND THE THIRD PART OF THE WATERS BECAME WORMWOOD AND MANY MEN DIED OF THE WATERS BECALSE THEY WERE MADE BITTER"
print(decoded)


decoded_with_spaces = ""
j = 0
for i in range(len(WOW)):
    if WOW[i] == ' ':
        decoded_with_spaces += ' '
        continue
    decoded_with_spaces += decoded[j]
    j += 1

print(decoded_with_spaces)

# plt.subplot(131)
# plt.bar(names, values)
# plt.subplot(132)
# plt.bar(names_wzor, values_wzor)
# plt.show()


