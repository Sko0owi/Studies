

alphabet = 'AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŻŹ'
alphalen = len(alphabet)
num2char = dict(enumerate(alphabet))
print(num2char)
char2num = { num2char[n]:n for n in num2char }

def encodeChar(c, k):
    return num2char[(char2num[c]+char2num[k]) % alphalen]

def decodeChar(c, k):
    return num2char[(char2num[c] - char2num[k] + alphalen) % alphalen]

def encode(plaintext, key):
    keylen = len(key)
    ciphertext = [encodeChar(plaintext[n], key[n%keylen]) for n in range(len(plaintext))]
    return ''.join(ciphertext)


def decode(ciphertext, key):
    keylen = len(key)
    plaintext = [decodeChar(ciphertext[n], key[n%keylen]) for n in range(len(ciphertext))]
    return ''.join(plaintext)


toDecode = "ŃABQYĆVLHĄOHQKĆNVODGQVŹŁFIXDWUJOZQNXŃNMRHZŹVSUJDFCWUFKRYĘSUÓLIWZEĆFHĄZNŚĘRKNYRZXCSIKQKĘVZZSHAMĆULNBSHJTÓHINQDOWŹKŁGSDATFŹCJDNMYAABCWXQOŻKĘYXTCXŹGZQMĆBZIĘZEXSŃLĄFŹPDVNYNVŹZCIOFSQŻZKGAPXÓŁRUŃŃIQRUÓŁNZSYHQPBLIMŻAOPĄWLNBCEŚXSZĄÓOCUAZXTXCUJUĆIHĄNAGGIUNMBCBDXRQLOBIQOBYPIPZXXĘZĘONOTŹQIULUXHRNAKSHBŹĘTFXWZSĘĘÓBVIBJRAÓĘWWQŹZEĆDTNŹNŚ"


test =     "ŃABQYĆVLHĄOHQKĆNVODGQVŹŁFIXDWUJOZQNXŃNMRHZŹVSUJDFCWUFKRYĘSUÓLIWZEĆFHĄZNŚĘRKNYRZXCSIKQKĘVZZSHAMĆULNBSHJTÓHINQDOWŹKŁGSDATFŹCJDNMYAABCWXQOŻKĘYXTCXŹGZQMĆBZIĘZEXSŃLĄFŹPDVNYNVŹZCIOFSQŻZKGAPXÓŁRUŃŃIQRUÓŁNZSYHQPBLIMŻAOPĄWLNBCEŚXSZĄÓOCUAZXTXCUJUĆIHĄNAGGIUNMBCBDXRQLOBIQOBYPIPZXXĘZĘONOTŹQIULUXHRNAKSHBŹĘTFXWZSĘĘÓBVIBJRAÓĘWWQŹZEĆDTNŹNŚ"

print(test == toDecode)


possibleKeys = []
wow = []

key_prefix = "KULTURY"

print(len(toDecode))

key_len = 10

for startPos in range(0,len(toDecode) - len(key_prefix) + 1):
    key = ['A'] * key_len
    curr_key = ""
    for i in range(0, len(key_prefix)):
        for k in alphabet:
            if(encode(key_prefix,curr_key + k)[i] == toDecode[startPos + i]):
                curr_key += k
                break           
    possibleKeys.append(curr_key)

print(possibleKeys)


exit()

possibleKeys2 = [[],[],[],[],[],[]]

# for key in possibleKeys:
#     possibleKeys2[0].append(key)

for k1 in alphabet: 
    possibleKeys2[0].append(k1)
    for k2 in alphabet:
        possibleKeys2[1].append(k1 + k2)
        for k3 in alphabet:
            possibleKeys2[2].append(k1 + k2 + k3)
            for k4 in alphabet:
                possibleKeys2[3].append(k1 + k2 + k3 + k4)
                for k5 in alphabet:
                    possibleKeys2[4].append(k1 + k2 + k3 + k4 + k5)

# print(possibleKeys2)

plain_text = []


prev = ''
for i in range(0,4):
    print(i, len(possibleKeys2[i]))
    for key in possibleKeys2[i]:
        x = decode(toDecode, key)
        if key[0] != prev:
            print(i+1, key[0], len(plain_text))
            prev = key[0]
        
        if "KULTURY" in x:
            plain_text.append((key, x))

print("KONIEC")
print(plain_text)

# for i in range(len(possibleKeys)):
#     print(i, wow[i], possibleKeys[i][:20], decode(toDecode, possibleKeys[i])[:20])


# print(possibleplaintext[ord('M')-ord('A')])
    
    