from helpers import char_converter, vigenere_decrypt, help_vigenere_key

alphabet = "AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŻŹ"

ciphertext = "ŃABQYĆVLHĄOHQKĆNVODGQVŹŁFIXDWUJOZQNXŃNMRHZŹVSUJDFCWUFKRYĘSUÓLIWZEĆFHĄZNŚĘRKNYRZXCSIKQKĘVZZSHAMĆULNBSHJTÓHINQDOWŹKŁGSDATFŹCJDNMYAABCWXQOŻKĘYXTCXŹGZQMĆBZIĘZEXSŃLĄFŹPDVNYNVŹZCIOFSQŻZKGAPXÓŁRUŃŃIQRUÓŁNZSYHQPBLIMŻAOPĄWLNBCEŚXSZĄÓOCUAZXTXCUJUĆIHĄNAGGIUNMBCBDXRQLOBIQOBYPIPZXXĘZĘONOTŹQIULUXHRNAKSHBŹĘTFXWZSĘĘÓBVIBJRAÓĘWWQŹZEĆDTNŹNŚ"


known_plaintext = 'KULTURY'

converter = char_converter(alphabet)

possible_keys = [
    help_vigenere_key(ciphertext[i:], known_plaintext, converter) for i in range(len(ciphertext) -
                                                                                 len(known_plaintext) + 1)
]

print(possible_keys)  # jak widać - niewiele widać
print(
    len(possible_keys))  # narazie jedyne co mamy to 317 kluczy, ale to nam tak na prawdę daje 317 możliwych plaintextów

for key in possible_keys:
    print("plaintext:", vigenere_decrypt(ciphertext, key, converter))