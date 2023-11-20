import os
import random

from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import padding

from cryptography.hazmat.primitives import hashes

STR_LEN = 6 


PREFIX_GIT = 8

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

    if first_hex[:PREFIX_GIT] == brute_hex[:PREFIX_GIT]:
        print(first_hex[:PREFIX_GIT], brute_hex[:PREFIX_GIT])
        print(class_name)
        return True
    return False


alphabet = []
# for a in range(32, 127 + 1):
#     alphabet.append(chr(a))
for a in range(ord('a'), ord('z') + 1):
    alphabet.append(chr(a))
for a in range(ord('A'), ord('Z') + 1):
    alphabet.append(chr(a))
# for a in range(ord('1'), ord('9') + 1):
#     alphabet.append(chr(a))

checked_names = {}

checkpoints = []

foo = 10
for i in range(9):
    checkpoints.append(foo)
    foo *= 10

debug = 0
def genName():
    
    global debug
    if debug < len(checkpoints) and len(checked_names) > checkpoints[debug]:
        print(len(checked_names), debug+1)
        debug += 1

    # if len(checked_names) > 2*1e7:
    #     exit(1)
    # curr_name = "fgfgfgfg"

    curr_name = ""
    for i in range(STR_LEN):
        curr_name += random.choice(alphabet)

    if curr_name not in checked_names:
        # curr_name = "fgfgfgfg"
        # print(curr_name)
        checked_names[curr_name] = True
        if test(curr_name):
            print(curr_name, len(checked_names))
            return True
    return False
def brute_force():

    while(True):
        x = genName()
        if(x == True):
            return True



with open('zad1a.html', 'rb') as f:
    contents = f.read()

    digest = hashes.Hash(hashes.SHA256())
    digest.update(contents)
    first_hex = digest.finalize().hex()

    print(first_hex)


with open('zad1b.html', 'rb') as f:
    contents = f.read()
    str_list = contents.decode("utf-8").split("iesfec")
    brute_force()

with open('zad1_28.html', 'rb') as f:
    contents = f.read()
    digest = hashes.Hash(hashes.SHA256())
    digest.update(contents)
    my_hex = digest.finalize().hex()

    print(my_hex)