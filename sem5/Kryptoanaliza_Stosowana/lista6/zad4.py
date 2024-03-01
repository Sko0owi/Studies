#!/usr/bin/env python
from primes import gen_rsa
from Crypto.Util.number import bytes_to_long, long_to_bytes
from dataclasses import dataclass
import gmpy2 as gm
from tqdm import tqdm
n, e, d, p, q = gen_rsa(1024)

gm.set_context(gm.context())
gm.get_context().precision = 128


@dataclass
class Pair:
    fst: gm.mpfr
    snd: gm.mpfr


def half(c):
    m = gm.powmod(c, d, n)
    return m >= n/2


def parity(c):
    m = gm.powmod(c, d, n)
    return False if m % 2 == 0 else True


def oracle(c, function, start_s):
    s = start_s
    j_n = Pair(gm.mpfr(0), gm.mpfr(n))
    for i in tqdm(range(gm.mpz(gm.floor(gm.log2(n)))+1)):
        cp = gm.mul(c, gm.powmod(s, e, n)) % n
        ans = function(cp)
        r = gm.div(gm.add(j_n.fst, j_n.snd), 2)
        if ans:
            j_n.fst = r
        else:
            j_n.snd = r
        s = gm.mul(s, 2)
    m = gm.mpz(gm.floor(j_n.snd))
    print(long_to_bytes(m).decode())



if __name__ == '__main__':
    m = bytes_to_long(b'skowi to ja pog')
    c = pow(m, e, n)
    print(f'{n=}\n{d=}\n{p=}\n{q=}\n{m=}')
    oracle(c, half,1)
    oracle(c, parity, 2)
