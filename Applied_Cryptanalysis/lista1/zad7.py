import os

from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import padding


key = os.urandom(16)

iv = os.urandom(16)

plain_text = bytes("Głucha noc, Miasto płacze znowu, czy wysłucha ktoś? Cichosza, cichosza Jest za cicho, także puszczaj to na blok",'utf-8')

plain_text_my = bytes("HELLO YOUR COMPUTER HAS A VIRUS PLEASE GIB ME MONEY", 'utf-8')

padder = padding.PKCS7(128).padder()
pt = padder.update(plain_text) + padder.finalize()

padder = padding.PKCS7(128).padder()
pt_my = padder.update(plain_text_my) + padder.finalize()

pt_hex = pt.hex()
pt_my_hex = pt_my.hex()

blocks_pt_hex = [bytes.fromhex(pt_hex[i:i+32]) for i in range(0,len(pt_hex),32)]
blocks_pt_my_hex = [bytes.fromhex(pt_my_hex[i:i+32]) for i in range(0,len(pt_my_hex),32)]

# print(blocks_pt_hex)
# print(blocks_pt_my_hex)



# for i in blocks_pt_hex:
#     print(bytes.fromhex(i).decode('utf-8'))
# for i in blocks_pt_my_hex:
#     print(bytes.fromhex(i).decode('utf-8'))

cipher = Cipher(algorithms.AES128(key), modes.CBC(iv))

encryptor = cipher.encryptor()
ct = encryptor.update(pt) + encryptor.finalize()

ct_hex = ct.hex()
blocks_ct_hex = [bytes.fromhex(ct_hex[i:i+32]) for i in range(0,len(ct_hex),32)]

decryptor = cipher.decryptor()
pt_check = decryptor.update(ct) + decryptor.finalize()

padder = padding.PKCS7(128).unpadder()
pt_check = padder.update(pt_check) + padder.finalize()

print("TEST: ", pt_check.decode('utf-8'))

# print("CT: ",blocks_ct_hex)
# print("MY_PT: ",blocks_pt_my_hex)
# print("PT: ",blocks_pt_hex)

# print("BEFORE:",blocks_ct_hex)


for i in range(0,len(blocks_ct_hex),2):
    # print(i)
    blocks_ct_hex[i] = bytes(a ^ b ^ c for a, b, c in zip(blocks_ct_hex[i], blocks_pt_my_hex[int(i/2)], blocks_pt_hex[i+1]))

# print("PRZEROBKA:", blocks_ct_hex)

new_ct = b''.join(blocks_ct_hex)

decryptor = cipher.decryptor()
new_pt = decryptor.update(new_ct) + decryptor.finalize()

padder = padding.PKCS7(128).unpadder()
new_pt = padder.update(new_pt) + padder.finalize()
print("KONIEC: ",new_pt)