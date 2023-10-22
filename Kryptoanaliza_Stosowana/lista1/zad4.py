from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes
import random

def decide(s):
    if s == 1:
        return "AES"
    return "random"

class AdamPlain(): 
    def __init__(self) -> None:
        self.key = get_random_bytes(16)
        self.alg = random.choice([1, 2]) 
        self.map = {}

    def random_perm(self, plaintext, iv):
        for block in plaintext:
            key = (block, iv)
            if key not in self.map:
                self.map[key] = get_random_bytes(len(plaintext))
        return self.map[key]

    def encrypt_plain(self, plaintext, iv):
        if self.alg == 1:
            return self.get_blocks(AES.new(self.key, AES.MODE_CBC, iv=iv).encrypt(plaintext))
        else:
            return self.get_blocks(self.random_perm(plaintext, iv))

    @staticmethod
    def get_blocks(s):
        return [s[i : i + AES.block_size] for i in range(0, len(s), AES.block_size)]
    
class AdamEncrypted(AdamPlain): 
    def encrypt_enc(self, plaintext, iv):
        if self.alg == 1:
            enc_iv = super().encrypt_plain(iv, b'\x00'*16)[0]
            return self.get_blocks(AES.new(self.key, AES.MODE_CBC, iv = enc_iv).encrypt(plaintext))
        else:
            return self.get_blocks(self.random_perm(plaintext, iv))

if __name__ == '__main__':
    mode = input("Choose mode p/e\n")

    # Plain Test
    if mode == 'p':
        print("Plain AES")
        adam_plain = AdamPlain()
        plaintext1 = b'\x00'*32
        plaintext2 = b'\x00'*16
        iv = b'\x00'*16
        
        c1 = adam_plain.encrypt_plain(plaintext1, iv)

        c2 = adam_plain.encrypt_plain(plaintext2, c1[0])

        # print(c1[1])
        # print(c2[0])
        print("For alg: ", decide(adam_plain.alg))
        if c1[1] == c2[0]:
            print("Used AES algorithm")
        else:
            print("It's random algo")

    # Encrypted Test
    elif mode == 'e':
        print("Encrypted AES")
        adam_enc = AdamEncrypted()
        plaintext1 = b'\x00'*32
        plaintext2 = b'\x00'*16
        iv = b'\x00'*16
        
        c1 = adam_enc.encrypt_enc(plaintext1, iv)
        # print("C1 :", c1)
        c2 = adam_enc.encrypt_enc(plaintext2+c1[0], iv)
        # print("C2 :", c2)

        print("For alg: ", decide(adam_enc.alg))
        if c1[0] == c2[0]:
            print("Used AES algorithm")
        else:
            print("It's random algo")