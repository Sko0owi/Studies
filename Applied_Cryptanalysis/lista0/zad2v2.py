alphabet = "AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŻŹ"
alphasize = len(alphabet)
num2char = dict(enumerate(alphabet))
char2num = { num2char[n]:n for n in num2char }

def encodeChar(c, k):
    return num2char[(char2num[c]+char2num[k]) % alphasize]
def decodeChar(c, k):
    return num2char[(char2num[c] - char2num[k] + alphasize) % alphasize]
def encode(plaintext, key):
    ciphertext = [encodeChar(plaintext[n], key[n]) for n in range(len(plaintext))]
    return ''.join(ciphertext)

toDecode1 = "ĘĘKGCATDUJXNYYVXWÓWUĄVŹGJĘCŃŁQŃJAŁGEJWXNYĆŃKXŁMJMOWŃAVVCUBWLŃARJHÓŁVBIONKFQJMEĄŃŹJAEHĆQEŁRIŁŚŹFMŹNŚŁFAĘXTOPŚŹUĆŃPBUŚAGWYZGHUHRWUGKFŚKĄYV"
toDecode2 = "ŁUXBUQLĄNUAÓĘCŃŁĆŚDNŁVSSGĘGKĆPLIRDWVÓDŃŚŁÓĘŃNWWOŚZFĆŚSŹBŻUSIKUĆHHUĆUĘŁVILCWRŹBAYMYBKŹŁFRKŃŃCIŹUŻDSTHEOOLLOÓIŁZVPJŁŻWGDŃĆFŹFQKOAĄSZAVĆŻŁŚ"
toDecode2reszta = "JADLCŹDWEŁHBĆQZXPĘINLĘYXYAŻTLQMNĄĆŻWU"

decode1Xordecode2 = ""

slowa = [""]

for i in range(len(toDecode1)):
    decode1Xordecode2 += decodeChar(toDecode1[i], toDecode2[i])

print(decode1Xordecode2)


while True:
    first_word1 = input("First word:")
    fw_2 = ""
    for i in range(len(first_word1)):
        fw_2 += decodeChar(first_word1[i].upper(), decode1Xordecode2[i])
    print(fw_2)