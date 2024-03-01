#!/usr/bin/env python

from OAEP import bytes2int, decrypt_oaep, int2bytes, pad_oaep, unpad_oaep
import hashlib


def read_file(file):
    f = open(file, "r")
    return tuple(int(x.strip()) for x in f.read().splitlines())

if __name__ == "__main__":
    n, e, d, c = read_file("cipher_zad5.txt")
    m = decrypt_oaep(c, d, n, h=hashlib.sha256)
    print(m.decode())
