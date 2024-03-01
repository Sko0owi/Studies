

alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
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


toDecode = "FDGEFYQUMYMODKBFASDMBTQD"

possibleplaintext = []
for k in alphabet:
    possibleplaintext.append((k,decode(toDecode, k)))
    #print(k, decode(toDecode, k))

print(possibleplaintext[ord('M')-ord('A')])
    

toDecode = "XHQPMFTFSJBHAMEHGIGHISHLPHLJAECWRVSRJWXNQECBSIQSCQSRHERWTWSVLVMRVLJAECWRVSRJWXNQECBSIFIHCPKS"

possibleplaintext = []
possibleKeys = [[],[],[],[],[]]

curr_key = ""
key_prefix = "ITMAY"

for k in alphabet:
    if(encode(key_prefix,k)[0] == toDecode[0]):
        possibleKeys[0].append(k)
    


for i in range(1, len(key_prefix)):
    for k in alphabet:
        if(encode(key_prefix,k)[i] == toDecode[i]):
            for key in possibleKeys[i-1]:
                possibleKeys[i].append(key + k)            
            

print(possibleKeys)

for keys in possibleKeys:
    for key in keys:
        print()
        print(key, decode(toDecode, key))


# print(possibleplaintext[ord('M')-ord('A')])
    
    