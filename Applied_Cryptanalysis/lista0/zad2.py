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


# alphabetStart = "CDEFGHIKLMNOTŻŹ"
# test_dict = {}
# for char in alphabetStart:
#     fw_2 = decodeChar(char,decode1Xordecode2[0])
#     test_dict[char] = fw_2

# print(test_dict)

polish_words_dict = {}
with open('zad2.txt', 'r') as f:
    polish_words = [x.strip() for x in f.readlines()][1:]
    for x in polish_words:
        bad = False
        for char in x:
            # print(char, end="")
            if char.upper() not in alphabet:
                bad = True
        # print(bad)
        if bad == False and len(x) >= 1:
            polish_words_dict[x.upper()] = True

# for word in polish_words_dict:
#     print(word)
#     next = input("next?:")
#     if next == "y":
#         continue
#     else:
#         break
print("KONIEC WCZYTANIA")

for word in polish_words_dict:
    fw_2 = ""
    for i in range(len(word)):
        fw_2 += decodeChar(word[i].upper(), decode1Xordecode2[i])
    
    # print(word)
    # print(fw_2)
    # print()
    
    good = False
    ile = 0
    matches = []
    for i in range(len(word)):
        if fw_2[:i] in polish_words_dict:
            good = True
            ile += 1
            matches.append(fw_2[:i])
            
            
    
    if good == False or "Q" in fw_2 or "V" in fw_2 or "X" in fw_2:
        continue
    match_max_len = 0
    for match in matches:
        match_max_len = max(len(match), match_max_len)
        print("MATCH: ", match)
    print("MATCH MAX LEN: L", match_max_len)
    print("MATCH TYLE BYL: C", ile)
    print(word)
    print(fw_2)
    print()

    # next = input("next?:")
    # if next == "n":

    #     break
    # else:
    continue
    #     first_word1 = input("First word:")
    # fw_2 = ""
    # for i in range(len(first_word1)):
    #     fw_2 += decodeChar(decode1Xordecode2[i], first_word1[i].upper())
    # print(fw_2)