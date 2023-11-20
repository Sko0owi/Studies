import os
import random

from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import padding

from cryptography.hazmat.primitives import hashes

STR_LEN = 6


PREFIX_GIT = 12

first_hex = 0

str_list = []



alphabet = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f']

checked_hashes = {}


def brute_force():

    while(True):

        if (len(checked_hashes) >= 1e6):
            # found_one.set()
            return True
        
        curr_colour = ""
        for i in range(STR_LEN):
            curr_colour += random.choice(alphabet)

        whole_document = (str_list[0] + curr_colour + str_list[1]).encode()
        
        digest = hashes.Hash(hashes.SHA256())
        # digest.update(whole_document)
        # brute_hex = digest.finalize().hex()
        brute_hex = curr_colour

        hash = brute_hex[:PREFIX_GIT]

        if hash in checked_hashes and checked_hashes[hash] != curr_colour:
            print("WOWOWO")
            print(hash)
            print(curr_colour)
            print(checked_hashes[hash])
            print(len(checked_hashes))

            return True
        
        checked_hashes[hash] = curr_colour



with open('zad2a.html', 'rb') as f:
    contents = f.read()
    str_list = contents.decode("utf-8").split("02d5df")
    brute_force()


