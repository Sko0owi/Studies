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
    ciphertext = [encodeChar(plaintext[n], key[n]) for n in range(len(plaintext))]
    return ''.join(ciphertext)

def decodeChar(c, k):
    return num2char[(char2num[c] - char2num[k] + alphasize) % alphasize]


for a in alphabet:
    klucz = a+a+a+a
    toDecode1 = encode('OLE', klucz)
    toDecode2 = encode('KOTA', klucz)

    # print(toDecode1, toDecode2)

    # toDecode1 = "ĘĘKGCATDUJXNYYVXWÓWUĄVŹGJĘCŃŁQŃJAŁGEJWXNYĆŃKXŁMJMOWŃAVVCUBWLŃARJHÓŁVBIONKFQJMEĄŃŹJAEHĆQEŁRIŁŚŹFMŹNŚŁFAĘXTOPŚŹUĆŃPBUŚAGWYZGHUHRWUGKFŚKĄYV"
    # toDecode2 = "ŁUXBUQLĄNUAÓĘCŃŁĆŚDNŁVSSGĘGKĆPLIRDWVÓDŃŚŁÓĘŃNWWOŚZFĆŚSŹBŻUSIKUĆHHUĆUĘŁVILCWRŹBAYMYBKŹŁFRKŃŃCIŹUŻDSTHEOOLLOÓIŁZVPJŁŻWGDŃĆFŹFQKOAĄSZAVĆŻŁŚJADLCŹDWEŁHBĆQZXPĘINLĘYXYAŻTLQMNĄĆŻWU"

    toDecode1LCM = ""
    toDecode2LCM = ""

    dlugosc1 = len(toDecode1)
    dlugosc2 = len(toDecode2)

    # print(char2num)

    # Rozszerz do NWW
    for i in range(dlugosc2):
        toDecode1LCM += toDecode1

    for i in range(dlugosc1):
        toDecode2LCM += toDecode2

    decode1Xordecode2 = ""

    for i in range(len(toDecode1LCM)):
        decode1Xordecode2 += decodeChar(toDecode2LCM[i], toDecode1LCM[i])
        # decode1Xordecode2 += encodeChar(toDecode1LCM[i], toDecode2LCM[i])

    # print(decode1Xordecode2)

    for letter in "K":
        wyn11 = [0] * dlugosc1
        wyn12 = [0] * dlugosc1
        wyn2 = [0] * dlugosc2

        for i in range(0, len(decode1Xordecode2), dlugosc2):
            wyn11[i % dlugosc1] = decodeChar(letter, decode1Xordecode2[i])
            wyn12[i % dlugosc1] = num2char[(alphasize - (alphasize + char2num[decodeChar(letter, decode1Xordecode2[i])]))%alphasize] 

        for i in range(0, dlugosc2):
            wyn2[i] = encode(wyn11[i%dlugosc1], decode1Xordecode2[i])

        wyn11 = "".join(wyn11)
        wyn12 = "".join(wyn12)
        wyn2 = "".join(wyn2)

        print(a)
        print("W1: ", wyn11)
        print("W2: ", wyn12)
        print("W3: ", wyn2)