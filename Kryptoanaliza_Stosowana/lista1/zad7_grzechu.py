from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes
from Crypto.Util.Padding import pad


def get_blocks(s):
        return [s[i : i + AES.block_size] for i in range(0, len(s), AES.block_size)]

def xor(a,b,c):
    return bytes([x ^ y ^ z for x, y,z in zip(a, b,c)])

iv = get_random_bytes(16)
key = get_random_bytes(16)

plaintext = "Daleko, daleko, na nie objętych mapami peryferiach niemodnegoo".encode()

plaintext = pad(plaintext, 16)
pt = get_blocks(plaintext)

cipher = AES.new(key, AES.MODE_CBC, iv=iv).encrypt(plaintext)


cipher = get_blocks(cipher)
print("Cipher: ", cipher)

ourtext = "Szedl Sasza sucha szosa".encode()
ourtext = get_blocks(pad(ourtext, 16))
print("Ourtext: ", ourtext)

for i in range (0, len(cipher), 2):
        cipher[i] = xor(ourtext[(i+1)//2], cipher[i], pt[i+1])

print("Cipher: ", cipher)

print("Po odszyfrowaniu mamy\n")


print(AES.new(key, AES.MODE_CBC, iv=iv).decrypt(b''.join(cipher)))