#!/usr/bin/env python

import os
from Crypto.PublicKey import RSA
from Crypto.Util.number import bytes_to_long, inverse
import time

NO_TESTS = 200


def rsa_decrypt(c, d, n):
    """classic rsa decryption"""
    return pow(c, d, n)


def rsa_decrypt_fast(dp, dq, q_inv, p, q, c):
    """Garner's algorithm for rsa decryption"""
    mp = pow(c, dp, p)
    mq = pow(c, dq, q)
    h = q_inv * ((mp - mq) % p) % p
    return mq + h * q


def init(rsa):
    message = os.urandom(128)
    m = bytes_to_long(message)
    c = pow(m, rsa.e, rsa.n)
    return m, c


def timeit(f):
    def wrap(*args, **kwargs):
        start = time.time()
        f(*args, **kwargs)
        end = time.time()
        print(f"Decrypted {NO_TESTS} in {end - start:2f} seconds")

    return wrap


@timeit
def test_decrypt():
    rsa = RSA.generate(2048)
    for i in range(NO_TESTS):
        m, c = init(rsa)
        dec = rsa_decrypt(c, rsa.d, rsa.n)
        assert dec == m


@timeit
def test_decrypt_fast():
    rsa = RSA.generate(2048)
    for i in range(NO_TESTS):
        m, c = init(rsa)
        p, q, e = rsa.p, rsa.q, rsa.e
        dp = inverse(e, p - 1)
        dq = inverse(e, q - 1)
        dec = rsa_decrypt_fast(dp, dq, rsa.invq, p, q, c)
        assert dec == m


test_decrypt()
test_decrypt_fast()
