

alphabet = 'AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŻŹ'
alphalen = len(alphabet)
num2char = dict(enumerate(alphabet))
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

possibleKeys = []
wow = []

key_prefix = "KULTURY"


ICtable = []

for lenKey in range(2,40):
    
    frequency = []
    for i in range(lenKey):
        frequency.append({})
    
    print(frequency)
    
    for i in range(len(toDecode)):
        if toDecode[i] in frequency[i%lenKey]:
            frequency[i%lenKey][toDecode[i]] += 1
        else:
            frequency[i%lenKey][toDecode[i]] = 1

    IC = 0.0

    
    for zbior in range(lenKey):
        numerator = 0.0
        N = 0
        for letter in frequency[zbior].items():
            numerator += letter[1]*(letter[1] - 1)
            N += letter[1]
            # print(letter)
        ICrow = numerator / (N* (N-1))
        # print(ICrow)
        IC += ICrow

    # print("KeyLen:", lenKey, "IC:", IC/lenKey)
    ICtable.append((IC/lenKey, lenKey))
    # print(frequency)

ICtable = sorted(ICtable, key= lambda x : x[0], reverse=True)
for ICval in ICtable:
    print("KeyLen:", ICval[1], "IC:", ICval[0])


    