alphabet = "AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŻŹ"
alphasize = len(alphabet)
num2char = dict(enumerate(alphabet))
char2num = { num2char[n]:n for n in num2char }

def convert(c, num):
    n = (char2num[c] + num) % alphasize
    return num2char[n]

def encodeChar(c, k):
    return num2char[(char2num[c]+char2num[k]) % alphasize]

def encode(plaintext, key):
    keylen = len(key)
    ciphertext = [encodeChar(plaintext[n], key[n%keylen]) for n in range(len(plaintext))]
    return ''.join(ciphertext)

def decode(ciphertext, key):
    keylen = len(key)
    plaintext = [decodeChar(ciphertext[n], key[n%keylen]) for n in range(len(ciphertext))]
    return ''.join(plaintext)

def decode2(ciphertext):
    plaintext = [decodeChar2(ciphertext[n], 35) for n in range(len(ciphertext))]
    return ''.join(plaintext)

def decodeChar(c, k):
    return num2char[(char2num[c] - char2num[k] + alphasize) % alphasize]

def decodeChar2(c, k):
    return num2char[(-(char2num[c] - k)) % alphasize]

toDecode1 = encode('ALA', 'ABCD')
toDecode2 = encode('KOTA', 'ABCD')

print(toDecode1, toDecode2)

# toDecode1 = "ĘĘKGCATDUJXNYYVXWÓWUĄVŹGJĘCŃŁQŃJAŁGEJWXNYĆŃKXŁMJMOWŃAVVCUBWLŃARJHÓŁVBIONKFQJMEĄŃŹJAEHĆQEŁRIŁŚŹFMŹNŚŁFAĘXTOPŚŹUĆŃPBUŚAGWYZGHUHRWUGKFŚKĄYV"
# toDecode2 = "ŁUXBUQLĄNUAÓĘCŃŁĆŚDNŁVSSGĘGKĆPLIRDWVÓDŃŚŁÓĘŃNWWOŚZFĆŚSŹBŻUSIKUĆHHUĆUĘŁVILCWRŹBAYMYBKŹŁFRKŃŃCIŹUŻDSTHEOOLLOÓIŁZVPJŁŻWGDŃĆFŹFQKOAĄSZAVĆŻŁŚJADLCŹDWEŁHBĆQZXPĘINLĘYXYAŻTLQMNĄĆŻWU"

toDecode1LCM = ""
toDecode2LCM = ""

dlugosc1 = len(toDecode1)
dlugosc2 = len(toDecode2)

print(char2num)

for i in range(dlugosc2):
    toDecode1LCM += "ALA"

for i in range(dlugosc1):
    toDecode2LCM += "KOTA"

toDecode1LCM = encode(toDecode1LCM, "ABCD")
toDecode2LCM = encode(toDecode2LCM, "ABCD")

print(len(toDecode1LCM), len(toDecode2LCM), dlugosc1, dlugosc2)

decode1Xordecode2 = ""
print(toDecode1LCM)
print(toDecode2LCM)
for i in range(len(toDecode1LCM)):
    # decode1Xordecode2 += encodeChar(toDecode1LCM[i], toDecode2LCM[i])
    decode1Xordecode2 += decodeChar(toDecode1LCM[i], toDecode2LCM[i])

print(decode1Xordecode2)

debugDecode1Xordecode2 = decode(toDecode1LCM, toDecode2LCM)

print("DD: ",debugDecode1Xordecode2)
print()

test = decode("A",debugDecode1Xordecode2)
test2 = ""
for i in range(len(test)):
    test2 += decodeChar2(test[i],35)
print("POG: ", test)
print()

print("POG: ", test2)
print()

test_alphabet = ['A']

for letter in test_alphabet:
    wyn1 = [0] * dlugosc1
    wyn2 = [0] * dlugosc2

    for i in range(0, len(decode1Xordecode2), dlugosc1):
        # if wyn2[i % dlugosc2] != 0:
        #     if wyn2[i % dlugosc2] != decodeChar(decode1Xordecode2[i], letter):
        #         print("o cie chuj")
        wyn2[i % dlugosc2] = decodeChar(letter, decode1Xordecode2[i])
        print(wyn2, i, i%dlugosc2)

    wyn1[0] = letter
    for i in range(1, dlugosc1):
        if wyn2[i] == 0:
            print("wow")
        wyn1[i] = encodeChar(decode1Xordecode2[i], wyn2[i])

    wyn1 = "".join(wyn1)
    wyn2 = "".join(wyn2)

    print("W1: ", wyn1)
    print("W2: ", wyn2)
    print()