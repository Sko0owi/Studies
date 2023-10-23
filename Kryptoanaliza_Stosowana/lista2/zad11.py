import os
import random

from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import padding

from cryptography.hazmat.primitives import hashes

wersy = []

PREFIX_LENGTH = 6

DEBUG = 1

def genSpaces():

    nowe_wersy = []
    for wers in wersy:
        if(random.random() <= 0.5):
            wers += ' '
        wers += '\n'
        nowe_wersy.append(wers)
    
    
    wiersz = ''.join(nowe_wersy).encode('utf-8')

    digest = hashes.Hash(hashes.SHA256())
    digest.update(wiersz)
    brute_hex = digest.finalize().hex()
    if brute_hex[:PREFIX_LENGTH] == "0"*PREFIX_LENGTH:
        print(wiersz.decode('utf-8'), end="")
        return True
    else:
        return False
    

def rng_jesus():

    while(True):
        x = genSpaces()
        if(x == True):
            return True
        

if (not DEBUG):
    with open('zad11.txt', 'rb') as f:
        contents = f.read()
        wersy = contents.decode('utf-8').split('\n')

        rng_jesus()
else:

    with open('zad11_2.txt', 'rb') as f:
        contents = f.read()
        digest = hashes.Hash(hashes.SHA256())
        digest.update(contents)
        print(digest.finalize().hex())

