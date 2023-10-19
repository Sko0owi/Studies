from random import randint


alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
alphasize = len(alphabet)
num2char = dict(enumerate(alphabet))
char2num = {num2char[n]: n for n in num2char}

def genKey(n: int):
    key = ""
    for _ in range(n):
        key += alphabet[randint(0, alphasize-1)]
    return key


def encodeChar(c, k):
    return num2char[(char2num[c] + char2num[k]) % alphasize]


def decodeChar(c, k):
    return num2char[(char2num[c] - char2num[k] + alphasize) % alphasize]


def encode(plaintext, key):
    ciphertext = [encodeChar(plaintext[n], key[n]) for n in range(len(plaintext))]
    return ''.join(ciphertext)


def decode(ciphertext, key):
    plaintext = [decodeChar(ciphertext[n], key[n]) for n in range(len(ciphertext))]
    return ''.join(plaintext)



# def fake_key(ct: str, fake_text: str):
#     if len(ct) != len(fake_text):
#         return {'error': 'bad size of one of {ciphertext, fake-text}'}
#     fake = decode(fake_text, ct)
#     return {
#         "fake_key": fake
#     }


while(1):
    print("1. Encode Message")
    print("2. Decode Message")
    print("3. Generate Fake Key")
    print("4. Exit")
    x = input("Choose your fighter: ")
    if x == "1":
        plaintext = input("input plaintext: ")
        key = genKey(len(plaintext))
        ciphertext = encode(plaintext,key)
        print("encoded message: ", ciphertext)
        print("key used for encryption: ", key)
    elif x == "2":
        ciphertext = input("input ciphertext: ")
        key = input("input key: ")
        plaintext = decode(ciphertext,key)
        print("decoded message: ", plaintext)
    elif x == "3":
        ciphertext = input("input ciphertext: ")
        fakemessage = input("input fake message: ")
        fakekey = decode(ciphertext,fakemessage)
        print("your fake key: ", fakekey)
    elif x == "4":
        exit()