import base64
import os

from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import padding

from PIL import Image


image1 = ""
image2 = ""
image3 = ""

with open("tekst.rgb", "rb") as imageFile:
    image1 = base64.b64encode(imageFile.read())
    print(len(image1))


with open("gwiazda.rgb", "rb") as imageFile:
    image2 = base64.b64encode(imageFile.read())
    print(len(image2))

with open("kocham.rgb", "rb") as imageFile:
    image3 = base64.b64encode(imageFile.read())
    print(len(image3))


test = ""
with open("test.rgb", "rb") as imageFile:
    test = base64.b64encode(imageFile.read())
    print(len(test))
test2 = ""
with open("test2.rgb", "rb") as imageFile:
    test2 = base64.b64encode(imageFile.read())
    print(len(test2))


key = os.urandom(16)
iv = os.urandom(16)

# CTR
cipher = Cipher(algorithms.AES128(key), modes.CTR(iv))

encryptor = cipher.encryptor()
ci1 = encryptor.update(image1) + encryptor.finalize()

encryptor = cipher.encryptor()
ci2 = encryptor.update(image2) + encryptor.finalize()

ci1xorci2 = bytes(a ^ b for a, b in zip(ci1, ci2))

im = Image.frombuffer('RGBA', (256, 256), ci1, 'raw', 'RGBA', 0, 1)
im.save('CTR_IM1.png')

im = Image.frombuffer('RGBA', (256, 256), ci2, 'raw', 'RGBA', 0, 1) 
im.save('CTR_IM2.png')

im = Image.frombuffer('RGBA', (256, 256), ci1xorci2, 'raw', 'RGBA', 0, 1)
im.save('CTR_IM1XORIM2.png')


# CBC
cipher = Cipher(algorithms.AES128(key), modes.CBC(iv))

encryptor = cipher.encryptor()
ci1 = encryptor.update(image1) + encryptor.finalize()

encryptor = cipher.encryptor()
ci2 = encryptor.update(image2) + encryptor.finalize()

ci1xorci2 = bytes(a ^ b for a, b in zip(ci1, ci2))

im = Image.frombuffer('RGBA', (256, 256), ci1, 'raw', 'RGBA', 0, 1)
im.save('CBC_IM1.png')

im = Image.frombuffer('RGBA', (256, 256), ci2, 'raw', 'RGBA', 0, 1) 
im.save('CBC_IM2.png')

im = Image.frombuffer('RGBA', (256, 256), ci1xorci2, 'raw', 'RGBA', 0, 1)
im.save('CBC_IM1XORIM2.png')

# ECB
cipher = Cipher(algorithms.AES128(key), modes.ECB())

encryptor = cipher.encryptor()
ci1 = encryptor.update(image1) + encryptor.finalize()

encryptor = cipher.encryptor()
ci2 = encryptor.update(image2) + encryptor.finalize()

ci1xorci2 = bytes(a ^ b for a, b in zip(ci1, ci2))

im = Image.frombuffer('RGBA', (256, 256), ci1, 'raw', 'RGBA', 0, 1)
im.save('ECB_IM1.png')

im = Image.frombuffer('RGBA', (256, 256), ci2, 'raw', 'RGBA', 0, 1) 
im.save('ECB_IM2.png')

im = Image.frombuffer('RGBA', (256, 256), ci1xorci2, 'raw', 'RGBA', 0, 1)
im.save('ECB_IM1XORIM2.png')

# Modyfikacja tekstu

cipher = Cipher(algorithms.AES128(key), modes.CTR(iv))


encryptor = cipher.encryptor()
ci1 = encryptor.update(image3) + encryptor.finalize()


decryptor = cipher.decryptor()
ptoriginal = decryptor.update(ci1) + decryptor.finalize()
im = Image.frombuffer('RGBA', (256, 256), ptoriginal, 'raw', 'RGBA', 0, 1)
im.save('CTRBEFOREMOD_IM1XORIM2.png')

ci1xorci2 = bytes(a ^ b for a, b in zip(ci1, image3))
ci1xorci2 = bytes(a ^ b for a, b in zip(image1, ci1xorci2))

decryptor = cipher.decryptor()
pt1 = decryptor.update(ci1xorci2) + decryptor.finalize()

im = Image.frombuffer('RGBA', (256, 256), pt1, 'raw', 'RGBA', 0, 1)
im.save('CTRAFTERMOD_IM1XORIM2.png')

# JEST KOCHAM DISA :OOOOOO


# # for i in range(256):
# #     for j in range(0,1024,4):
# #         print(ci1xorci2[i*256+4*j:i*256+4*j + 4], end=" ")
# #     print()

# for i in range(30):
#     for j in range(0,120,4):
#         print(test[i*120+j:i*120+j + 4], end=" ")
#     print()

# encryptor = cipher.encryptor()
# ci3 = encryptor.update(test) + encryptor.finalize()

# encryptor = cipher.encryptor()
# ci4 = encryptor.update(test2) + encryptor.finalize()

# ci3xorci4 = bytes(a ^ b for a, b in zip(ci3, ci4))

# for i in range(30):
#     for j in range(0,120,4):
#         # encoded_msg = ci3xorci4[i*120+j:i*120+j + 4].decode('utf-8') # len(encoded_msg) is 2 bytes
#         # encoded_prefix = '{:0>5d}'.format(len(encoded_msg)).encode('utf-8')
#         # full_message = encoded_prefix + encoded_msg # both are bytes, so we can concat
#         print('%s' % ci3xorci4[i*120+j:i*120+j + 4].hex(), end=" ")
#     print()


# ci33 = bytes(a ^ b for a, b in zip(ci3xorci4, test2))

# im = Image.frombuffer('RGBA', (30, 30), ci3xorci4, 'raw', 'RGBA', 0, 1)
# im.save('CTRMOD_IM3XORIM4.png')

# im = Image.frombuffer('RGBA', (30, 30), ci33, 'raw', 'RGBA', 0, 1)
# im.save('TEST.png')

# # print(ci1xorci2)