#!/usr/bin/env python

import hashlib
import math

from Crypto.Cipher import PKCS1_OAEP
from Crypto.Hash import SHA256
from Crypto.PublicKey import RSA
from Crypto.Util.number import bytes_to_long, long_to_bytes, size

from wiener import wiener
from OAEP import bytes2int, decrypt_oaep, int2bytes, pad_oaep, unpad_oaep



def read_file(file):
    f = open(file, "r")
    return tuple(int(x.strip()) for x in f.read().splitlines())

if __name__ == "__main__":
    n, e, c = read_file("cipher_zad6.txt")
    maybe_d = wiener(n, e)
    if maybe_d:
        d = int(maybe_d)
        m = decrypt_oaep(c, d, n, h=hashlib.sha256)
        print(m.decode())
