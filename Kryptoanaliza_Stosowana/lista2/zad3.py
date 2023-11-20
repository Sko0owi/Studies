import os

from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import padding

key = os.urandom(16)


iv = bytes("giga_tajny_vec12",'ascii')
plain_text = bytes("Send $100 to Bob from my account.               ",'ascii')
print(len(plain_text))
print(len(iv))
print(plain_text.hex())
print(iv.hex())

cipher = Cipher(algorithms.AES128(key), modes.CBC(iv))

encryptor = cipher.encryptor()
ct = encryptor.update(plain_text) + encryptor.finalize()
print("CT:",ct)
print("CT HEX:", ct.hex())
ct_to_hack = ct.hex()
ct_to_hack = [ ct_to_hack[i:i+2] for i in range(0, len(ct_to_hack), 2) ] # bytes are standalone

print(ct_to_hack    )

iv_better_hex = iv.hex()
iv_better_hex = [ iv_better_hex[i:i+2] for i in range(0, len(iv_better_hex), 2)]


iv_hex_last = iv_better_hex[13:16]
ct_hex_last = ct_to_hack[13:16]
pt_hex_last = [ int(bytes("Bob",'ascii').hex()[i:i+2],16) for i in range(0, len(bytes("Bob",'ascii').hex()), 2)]

newpt_hex_last = [ int(bytes("Eve",'ascii').hex()[i:i+2],16) for i in range(0, len(bytes("Eve",'ascii').hex()), 2)]

print(iv_hex_last)
print(ct_hex_last)
print(pt_hex_last)

IP = [0]*3

for i in range(3):
    IP[i] = int(iv_hex_last[i],16) ^ pt_hex_last[i]
    
print(IP)

newIV = [0]*3
for i in range(3):
    newIV[i] = "{:02x}".format(IP[i] ^ newpt_hex_last[i])

print(newIV)



decryptor = cipher.decryptor()
plain_text = decryptor.update(ct) + decryptor.finalize()
print(plain_text)



hackedIV = iv_better_hex[0:13] + newIV
print(len(''.join(hackedIV)))
hackedIV = bytes.fromhex(''.join(hackedIV))
print(hackedIV)
cipherEVE = Cipher(algorithms.AES128(key), modes.CBC(hackedIV))
decryptor = cipherEVE.decryptor()
hacked_pt = decryptor.update(ct) + decryptor.finalize()
print(hacked_pt)
