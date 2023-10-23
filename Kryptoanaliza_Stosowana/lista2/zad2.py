import os
import random

from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import padding

from cryptography.hazmat.primitives import hashes

STR_LEN = 6


PREFIX_GIT = 16

first_hex = 0

str_list = []



def genHash(colour):
    whole_document = (str_list[0] + colour + str_list[1]).encode()

    digest = hashes.Hash(hashes.SHA256())
    digest.update(whole_document)
    brute_hex = digest.finalize().hex()

    return brute_hex[:PREFIX_GIT]


alphabet = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F']

checked_hashes = {}
checked_colours = {}

checkpoints = []

foo = 10
for i in range(10):
    checkpoints.append(foo)
    foo *= 10

debug = 0

def genColour():
    global debug
    if debug < len(checkpoints) and len(checked_hashes) > checkpoints[debug]:
        print(len(checked_hashes), debug+1)
        debug += 1
    
    # if len(checked_hashes) > 2*1e7:
    #     exit(1)

    curr_colour = ""
    for i in range(STR_LEN):

        curr_colour += random.choice(alphabet)

    if curr_colour in checked_colours:
        return False
    
    checked_colours[curr_colour] = True
    hash = genHash(curr_colour)
    if hash in checked_hashes:
        print("WOWOWO")
        print(hash)
        print(curr_colour)
        print(checked_hashes[hash])
        print(len(checked_hashes))

        return True
    
    checked_hashes[hash] = curr_colour
    
    return False
def brute_force():

    while(True):
        x = genColour()
        if(x == True):
            return True



with open('zad2a.html', 'rb') as f:
    contents = f.read()
    str_list = contents.decode("utf-8").split("02d5df")
    brute_force()


