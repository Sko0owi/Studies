

alphabet = 'AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŻŹ'
alphalen = len(alphabet)
num2char = dict(enumerate(alphabet))
char2num = { num2char[n]:n for n in num2char }

def encodeChar(c, k):
    return num2char[(char2num[c]+char2num[k]) % alphalen]

def decodeChar(c, k):
    if k == '*':
        return '*'
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
key_len = 9

for startPos in range(0,len(toDecode) - len(key_prefix) + 1):
    key = ['*'] * key_len
    curr_key = ""
    for i in range(0, len(key_prefix)):
        for k in alphabet:
            if(encode(key_prefix,curr_key + k)[i] == toDecode[startPos + i]):
                curr_key += k
                key[(startPos + i) % key_len] = k
                break           
    possibleKeys.append(''.join(key))

print(possibleKeys)

print(len(possibleKeys))

cnt = {}

for key in possibleKeys:
    if "X" in decode(toDecode, key) or "V" in decode(toDecode, key) or "Q" in decode(toDecode, key):
        continue
    print(key, decode(toDecode,key))

print(decode(toDecode,"PAMIĘTNIK"))
# for key in possibleKeys:
#     backup_key = key
#     # x = decode(toDecode,key)
#     # if "KULTURY" in x:
#     #     if backup_key not in cnt:
#     #         cnt[backup_key] = 1        
#     #     print(len(key), x[:7])


#     x = decode(toDecode, "AA"+key)
#     if "KULTURY" in x:
#         if backup_key not in cnt:
#             cnt[backup_key] = 1    
#         print("AA*", x[2:9])    
#         print()
#     x = decode(toDecode, "A"+key+"A")
#     if "KULTURY" in x:
#         if backup_key not in cnt:
#             cnt[backup_key] = 1    
#         print("A*A",x[1:8])    
#         print()
#     x = decode(toDecode, key + "AA")
#     if "KULTURY" in x:
#         if backup_key not in cnt:
#             cnt[backup_key] = 1    
#         print("*AA",x[0:7])    
#         print()

# for key in possibleKeys:
    
#     backup_key = key
#     x = decode(toDecode, key + "AA")
#     if "KULTURY" in x:
#         if backup_key not in cnt:
#             cnt[backup_key] = 1    
#         print("*AA",x[0:16])    
#         print()

# for key in possibleKeys:
    
#     backup_key = key
#     x = decode(toDecode, "A"+key+"A")
#     if "KULTURY" in x:
#         if backup_key not in cnt:
#             cnt[backup_key] = 1    
#         print("A*A",x[1:16])    
#         print()

# for key in possibleKeys:
    
#     backup_key = key
#     x = decode(toDecode, "AA"+key)
#     if "KULTURY" in x:
#         if backup_key not in cnt:
#             cnt[backup_key] = 1    
#         print("AA*", key, x[2:16])    
#         print()

# print(len(cnt))


# print(decode(toDecode,"KULTURYAA"))
# possibleKeys2 = [[],[],[],[],[]]

# for key in possibleKeys:
#     possibleKeys2[0].append(key)

# for i in range(len(possibleKeys)):
#     x = decode(toDecode[i%7:],possibleKeys[i])
#     pos = int(i/7 * 7) - i%7
#     for j in range(0,len(x)):
#         print(x[pos+j : pos+7+j], end=" ")
#     print()                        
#     print()
#     #print(possibleKeys[i], x)


# print(possibleKeys2)


# for i in range(len(possibleKeys)):
#     print(i, wow[i], possibleKeys[i][:20], decode(toDecode, possibleKeys[i])[:20])


# print(possibleplaintext[ord('M')-ord('A')])
    
    