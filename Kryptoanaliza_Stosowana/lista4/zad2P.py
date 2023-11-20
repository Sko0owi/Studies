#!/usr/bin/env python3

from Crypto.Util import asn1
from Crypto.Util.number import long_to_bytes, inverse
import pathlib
from zad3 import rsa_decrypt_fast, rsa_decrypt

der_data = pathlib.Path("key.der").read_bytes()
seq = asn1.DerSequence()
seq.decode(der_data)

ver, n, e, d, p, q, dp, dq, crt_q = seq

phi = (p - 1) * (q - 1)
assert n == p * q
assert d == inverse(e, phi)
assert dp == inverse(e, p - 1)
assert dq == inverse(e, q - 1)
assert crt_q == inverse(q, p)

ciphertext = 0x59DC3B0327E09B6B762385AC3FA1724A5C3761338018C1C69D691D563BE751377119397A86BD0873B9DD4259B94CDC7398FECBE9C252A4EF682BB09C17AFAD343E4C38F264B12ABFFF5AEF54F830A08B8C30E334BBD94A003310B5678A238135B1B89C324246129A6D8FCC34AA18386444AE3BF1D4BB7ADB603DA50B44683E024E32DCEE966531D1204D34A4CD51CF53431ACB3044588A4DE8CA124154308F557796873763FD625767B4296266E2C7CE07F891B8C6716DEB78DE0B84C6883FC42633A6CEEE2B4118E3C105C55DE092A61FBCA5CC4EC1F9A29C45A688D18D49AE5296363668E30C611E4ECBAACDFC4AF9458110A80ECAB25A6E2B19745271C1BE

message1 = rsa_decrypt(ciphertext, d, n)
message2 = rsa_decrypt_fast(dp, dq, crt_q, p, q, ciphertext)
print(long_to_bytes(message2).decode())
