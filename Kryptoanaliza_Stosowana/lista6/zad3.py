#!/usr/bin/env python

from primes import inverse
from Crypto.Util.number import bytes_to_long,long_to_bytes, getPrime, getRandomInteger
from math import gcd

def inverses(a, b):
    x, y = 1, 0
    x1, y1, a1, b1 = 0, 1, a, b
    while b1 != 0:
        q = a1 // b1
        x, x1 = x1, x - q * x1
        y, y1 = y1, y - q * y1
        a1, b1 = b1, a1 - q * b1
    return x,y

def eve(c1,c2,n,e1,e2):
    r,s = inverses(e1,e2)
    if s < 0:
        r,s = s,r
        c1,c2 = c2,c1
        e1,e2 = e2,e1
    c1_inv = inverse(c1,n)
    r = abs(r)
    g1 = pow(c1_inv,r,n)
    g2 = pow(c2,s,n)
    m = (g1*g2) % n
    print(f'msg: {long_to_bytes(m).decode()}')


if __name__ == "__main__":
    m = bytes_to_long(b'tata poszedl po mleko')
    p,q = getPrime(512),getPrime(512)
    n = p*q
    while True:
        e1 = getRandomInteger(64)
        e2 = getRandomInteger(64)
        if gcd(e1,e2) == 1:
            break
    c1 = pow(m,e1,n)
    c2 = pow(m,e2,n)
    eve(c1,c2,n,e1,e2)
    



