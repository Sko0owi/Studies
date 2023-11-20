import os
import random
from multiprocessing import Process, Value, Manager

from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import padding

from cryptography.hazmat.primitives import hashes

STR_LEN = 6


PREFIX_GIT = 8

processes = [None] * 6

# checked_hashes = {}

def brute_force(str_list, index, found_one, checked_hashes):

    alphabet = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F']


    while(True):
        
        # print(index, len(checked_hashes))
        # if (len(checked_hashes) >= 1e3):
        #     # print("eee ja", len(checked_hashes))
        #     # print(checked_hashes)
        #     found_one.value = 1
        #     return True
        
        if bool(found_one.value) is True:
            # print("eee, ktos inny", len(checked_hashes))
            # print(checked_hashes)

            return True
        
        curr_colour = ""
        for i in range(STR_LEN):
            curr_colour += random.choice(alphabet)

        whole_document = (str_list[0] + curr_colour + str_list[1]).encode()
        
        digest = hashes.Hash(hashes.SHA256())
        digest.update(whole_document)
        brute_hex = digest.finalize().hex()

        hash = brute_hex[:PREFIX_GIT]

        if hash in checked_hashes and checked_hashes[hash] != curr_colour:
            print("WOWOWO")
            print(hash)
            print(curr_colour)
            print(checked_hashes[hash])
            print(len(checked_hashes))
            found_one.value = 1
            return True
        
        checked_hashes[hash] = curr_colour





with open('zad2a.html', 'rb') as f:
    contents = f.read()
    str_list = contents.decode("utf-8").split("02d5df")

    found_one = Value('i', 0)


    manager = Manager()
    checked_hashes = manager.dict()

    for i in range(len(processes)):
        processes[i] = Process(target=brute_force, args=(str_list,i, found_one,checked_hashes,))
        processes[i].start()

    for i in range(len(processes)):
        processes[i].join()

    print(len(checked_hashes))

