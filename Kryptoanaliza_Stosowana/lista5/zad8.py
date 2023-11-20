#!/usr/bin/env python

import math

import gmpy2
from Crypto.Util.number import bytes_to_long, inverse, long_to_bytes

import primes


class Adam:
    def __init__(self):
        self.n, self.e, self.d, _, _ = primes.gen_rsa(1024)

    def decrypt(self, c):
        return pow(c, self.d, self.n)


adam = Adam()


def eve(c):
    r = pow(2, adam.e, adam.n)
    k = adam.decrypt((c * r) % adam.n)
    print(long_to_bytes(k // 2).decode())


def bob():
    m = bytes_to_long("tajna wiadomość".encode())
    # eve przechwytuje wiadomość
    eve(pow(m, adam.e, adam.n))


if __name__ == "__main__":
    bob()
