from cryptography.hazmat.primitives.asymmetric import rsa

from cryptography.hazmat.primitives.asymmetric import padding
from cryptography.hazmat.primitives.serialization import load_der_private_key


def int2bytes(n):
    return n.to_bytes((n.bit_length()+7)//8, 'big')
def int2str(n):
    return int2bytes(n).decode()
def bytes2int(b):
    return int.from_bytes(b, 'big')
def str2int(s):
    return bytes2int(s.encode())

private_key = None



with open("key.der", "rb") as f:
    private_key = load_der_private_key(f.read(),password=None)


n = private_key.private_numbers().public_numbers.n
e = private_key.private_numbers().public_numbers.e

p = private_key.private_numbers().p
q = private_key.private_numbers().q
d = private_key.private_numbers().d

d_p = private_key.private_numbers().dmp1
d_q = private_key.private_numbers().dmq1
q_inv = private_key.private_numbers().iqmp


assert(n == p*q)
assert((d * e)%((p-1)*(q-1)) == 1)
assert((d_p * e)%(p-1) == 1)
assert((d_q * e)%(q-1) == 1)
assert((q_inv * q)%(p) == 1)

cipher_text = (
'59dc3b0327e09b6b762385ac3fa1724a5c3761338018c1c69d691d563be751377119397a86bd0873'
'b9dd4259b94cdc7398fecbe9c252a4ef682bb09c17afad343e4c38f264b12abfff5aef54f830a08b'
'8c30e334bbd94a003310b5678a238135b1b89c324246129a6d8fcc34aa18386444ae3bf1d4bb7adb'
'603da50b44683e024e32dcee966531d1204d34a4cd51cf53431acb3044588a4de8ca124154308f55'
'7796873763fd625767b4296266e2c7ce07f891b8c6716deb78de0b84c6883fc42633a6ceee2b4118'
'e3c105c55de092a61fbca5cc4ec1f9a29c45a688d18d49ae5296363668e30c611e4ecbaacdfc4af9'
'458110a80ecab25a6e2b19745271c1be'
)



plain_text = pow(int(cipher_text,16),d,n)
print(int2bytes(plain_text).decode()) 
