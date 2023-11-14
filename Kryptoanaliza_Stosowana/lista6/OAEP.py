#!/usr/bin/env python

import hashlib
import math
import os
from functools import reduce
from itertools import dropwhile


def int2bytes(n, k=1):
    r = n.to_bytes((n.bit_length() + 7) // 8, "big")
    p = k - (len(r) % k)
    p = 0 if p == k else p
    return bytes(p) + r


def bytes2int(b):
    return int.from_bytes(b, "big")


def xor(*args):
    return b"".join((int2bytes(reduce(lambda a, b: a ^ b, x)) for x in zip(*args)))


def mgf(seed, len, h=hashlib.sha1):
    if len > 1 << 32:
        raise ValueError("len parameter cannot be > 2^32")
    hlen = h().digest_size
    mask = b""
    for cnt in range(math.ceil(len / hlen)):
        c = cnt.to_bytes(4, "big")
        mask += h(seed + c).digest()
    return mask[:len]


def pad_oaep(m, n, h=hashlib.sha1, mgf=mgf):
    m = int2bytes(msg)
    modulus = int2bytes(n)
    hlen = h().digest_size
    mlen = len(m)
    k = n.bit_length() // 8
    label = b""
    if mlen > k - 2 * hlen - 2:
        raise ValueError("Message too long")
    lhash = h(label).digest()
    ps = bytes(k - mlen - 2 * hlen - 2)
    db = lhash + ps + b"\x01" + m
    seed = os.urandom(hlen)
    db_mask = mgf(seed, k - hlen - 1, h=h)
    masked_db = xor(db, db_mask)
    seed_mask = mgf(masked_db, hlen, h=h)
    masked_seed = xor(seed, seed_mask)
    return b"\x00" + masked_seed + masked_db


def unpad_oaep(msg, n, h=hashlib.sha1, mgf=mgf):
    if msg[0] != 0:
        raise ValueError("Message corrupted")
    k = n.bit_length() // 8 # ile bajtów
    hlen = h().digest_size # |h|
    masked_seed = msg[1 : 1 + hlen] # seed xor Y_1
    masked_X = msg[1 + hlen :] # X
    seed = xor(masked_seed, mgf(masked_X, hlen, h=h)) # Y_1 = MGF(X,|h|)
    db = xor(masked_X, mgf(seed, k - hlen - 1, h=h)) # X = X_1 ^ X_2
    db_stripped = db[hlen:]
    message = bytes(dropwhile(lambda x: x != 1, db_stripped))[1:]
    return message


def decrypt_oaep(c, d, n, h=hashlib.sha1, mgf=mgf):
    k = math.ceil(n.bit_length() / 8)
    m_int = pow(c, d, n)
    em = int2bytes(m_int, k)
    print(em,k)
    unpadded = unpad_oaep(em, n, h=h, mgf=mgf)
    return unpadded
