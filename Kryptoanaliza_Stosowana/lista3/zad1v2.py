import os
import random

from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import padding

from cryptography.hazmat.primitives import hashes

STR_LEN = 6 


PREFIX_GIT = 6

first_hex = 0

str_list = []
class_name = []
found = ""


def test(class_name):
    whole_document = (str_list[0] + class_name + str_list[1] + class_name + str_list[2]).encode()
    # print(whole_document)
    digest = hashes.Hash(hashes.SHA256())
    digest.update(whole_document)
    brute_hex = digest.finalize().hex()

    print(brute_hex)

    if first_hex[:PREFIX_GIT] == brute_hex[:PREFIX_GIT]:
        print(first_hex[:PREFIX_GIT], brute_hex[:PREFIX_GIT])
        print(class_name)
        return True
    return False




with open('zad1a.html', 'rb') as f:
    contents = f.read()

    digest = hashes.Hash(hashes.SHA256())
    digest.update(contents)
    first_hex = digest.finalize().hex()

    print(first_hex)


with open('zad1b.html', 'rb') as f:
    contents = f.read()
    str_list = contents.decode("utf-8").split("iesfec")
print(test("iesfec"))
exit()

with open('6_perm.txt','rb') as f:
    contents = f.read()
    contents = contents.decode('utf-8').split('\n')
    i = 0
    
    for name in contents:
        i += 1
        print(i)
        if test(name):
            print("TAAAK")
            break
    

with open('zad1_28.html', 'rb') as f:
    contents = f.read()
    digest = hashes.Hash(hashes.SHA256())
    digest.update(contents)
    my_hex = digest.finalize().hex()

    print(my_hex)