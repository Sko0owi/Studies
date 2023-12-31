
from cryptography.hazmat.primitives.asymmetric import rsa, padding
import math

from gmpy2 import (
    mpz,
    sqrt,
    floor,
    ceil,
    floor_div,
    mul,
    sub,
    add,
    is_divisible,
    square,
    gcd,
    is_square,
    powmod,
    cbrt,
    lcm,
    isqrt,
    is_prime,
    invert,
    iroot,
    is_integer
)

def int2bytes(n):
    n = int(n)
    return n.to_bytes((n.bit_length()+7)//8, 'big')
def int2str(n):
    return int2bytes(n).decode()
def bytes2int(b):
    return int.from_bytes(b, 'big')
def str2int(s):
    return bytes2int(s.encode())



def read_file(file):
    f = open(file, "r")
    return tuple(int(x.strip()) for x in f.read().splitlines())

def brut(n,e,c):
    for p in range(2,int(math.sqrt(n)) + 2):
        if not is_divisible(n,p): continue
        q = floor_div(n,p)
        assert(p * q == n)

        if is_prime(p) and is_prime(q):
            d = invert(e,mul((p-1),(q-1)))
            print(hex(int(powmod(c,d,n))))
            print(int2bytes(powmod(c,d,n)).decode())
            break

def better_brut(n,e,c):
    x = add(isqrt(n), mpz(1))
    y = isqrt(sub(square(x), n))
    while square(y) != sub(square(x), n):
        x = add(x,mpz(1))
        y = isqrt(sub(square(x), n))
    p = x + y
    q = x - y

    d = invert(e,mul((p-1),(q-1)))

    
    print(int2bytes(powmod(c,d,n)).decode())
    


# hope = "Niniejszym zaświadcza się, że liczba punktów przyznawanych za to zadanie wynosi 2+5."

# def hope_brut(n,e,c):

#     message = str2int(hope)
#     if powmod(message,e,n) == c:
#         print("POOOG")
    
    

if __name__ == "__main__":
    n, e, c = read_file("test4.txt")

    brut(n,e,c)

    # better_brut(n,e,c)

    # hope_brut(n,e,c)
