from collections import Counter


def char_converter(alphabet: str):
    _alphalen = len(alphabet)
    _num2char = dict(enumerate(alphabet))
    _char2num = {_num2char[n]: n for n in _num2char}

    def convert(c: str, num: int) -> str:
        n = (_char2num[c] + num) % _alphalen
        return _num2char[n]

    return convert


def vigenere_decrypt(ciphertext, key, converter):
    decrypted_text = []
    key_length = len(key)
    for i in range(len(ciphertext)):
        # Calculate the shift value for the current character in the key
        key_shift = ord(key[i % key_length].upper()) - ord('A')

        decrypted_char = converter(ciphertext[i], -key_shift)
        decrypted_text.append(decrypted_char)

    return ''.join(decrypted_text)


def help_vigenere_key(ciphertext: str, known_plaintext: str, converter):
    key = ""
    key_length = len(known_plaintext)

    for i in range(key_length):
        n = ord(known_plaintext[i]) - ord('A')
        key_char = converter(ciphertext[i], -n)
        key += key_char
    return key


def get_letter_freq(_ciphertext: str) -> dict:
    res = Counter()
    for c in _ciphertext:
        res[c] += 1
    res = dict(res)
    n = len(_ciphertext)
    for k, v in res.items():
        res[k] /= n
        res[k] *= 100
    return res


def sort_dict(d: dict) -> dict:
    return dict(sorted(d.items(), key=lambda item: item[1], reverse=True))


def get_en_pair_freq_dict() -> dict:
    pair_num = [
        [1, 20, 33, 52, 0, 12, 18, 5, 39, 1, 12, 57, 26, 181, 1, 20, 1, 75, 95, 104, 9, 20, 13, 1, 26, 1],
        [11, 1, 0, 0, 47, 0, 0, 0, 6, 1, 0, 17, 0, 0, 19, 0, 0, 11, 2, 1, 21, 0, 0, 0, 11, 0],
        [31, 0, 4, 0, 38, 0, 0, 38, 10, 0, 18, 9, 0, 0, 45, 0, 1, 11, 1, 15, 7, 0, 0, 0, 1, 0],
        [48, 20, 9, 13, 57, 11, 7, 25, 50, 3, 1, 11, 14, 16, 41, 6, 0, 14, 35, 56, 10, 2, 19, 0, 10, 0],
        [110, 23, 45, 126, 48, 30, 15, 33, 41, 3, 5, 55, 47, 111, 33, 28, 2, 169, 115, 83, 6, 24, 50, 9, 26, 0],
        [25, 2, 3, 2, 20, 11, 1, 8, 23, 1, 0, 8, 5, 1, 40, 2, 0, 16, 5, 37, 8, 0, 3, 0, 2, 0],
        [24, 3, 2, 2, 28, 3, 4, 35, 18, 1, 0, 7, 3, 4, 23, 1, 0, 12, 9, 16, 7, 0, 5, 0, 1, 0],
        [114, 2, 2, 1, 302, 2, 1, 6, 97, 0, 0, 2, 3, 1, 49, 1, 0, 8, 5, 32, 8, 0, 4, 0, 4, 0],
        [10, 5, 32, 33, 23, 17, 25, 6, 1, 1, 8, 37, 37, 179, 24, 6, 0, 27, 86, 93, 1, 14, 7, 2, 0, 2],
        [2, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0],
        [6, 1, 1, 1, 29, 1, 0, 2, 14, 0, 0, 2, 1, 9, 4, 0, 0, 0, 5, 4, 1, 0, 2, 0, 2, 0],
        [40, 3, 2, 36, 64, 10, 1, 4, 47, 0, 3, 56, 4, 2, 41, 3, 0, 2, 11, 15, 8, 3, 5, 0, 31, 0],
        [44, 7, 1, 1, 68, 2, 1, 3, 25, 0, 0, 1, 5, 2, 29, 11, 0, 3, 10, 9, 8, 0, 4, 0, 18, 0],
        [40, 7, 25, 146, 66, 8, 92, 16, 33, 2, 8, 9, 7, 8, 60, 4, 1, 3, 33, 106, 6, 2, 12, 0, 11, 0],
        [16, 12, 13, 18, 5, 80, 7, 11, 12, 1, 13, 26, 48, 106, 36, 15, 0, 84, 28, 57, 115, 12, 46, 0, 5, 1],
        [23, 1, 0, 0, 30, 1, 0, 3, 12, 0, 0, 15, 1, 0, 21, 10, 0, 18, 5, 11, 6, 0, 1, 0, 1, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0],
        [50, 7, 10, 20, 133, 8, 10, 12, 50, 1, 8, 10, 14, 16, 55, 6, 0, 14, 37, 42, 12, 4, 11, 0, 21, 0],
        [67, 11, 17, 7, 74, 11, 4, 50, 49, 2, 6, 13, 12, 10, 57, 20, 2, 4, 43, 109, 20, 2, 24, 0, 4, 0],
        [59, 10, 11, 7, 75, 9, 3, 330, 76, 1, 2, 17, 11, 7, 115, 4, 0, 28, 34, 56, 17, 1, 31, 0, 16, 0],
        [7, 5, 12, 7, 7, 2, 14, 2, 8, 0, 1, 34, 8, 36, 1, 16, 0, 44, 35, 48, 0, 0, 2, 0, 1, 0],
        [5, 0, 0, 0, 65, 0, 0, 0, 11, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0],
        [66, 1, 1, 2, 39, 1, 0, 44, 39, 0, 0, 2, 1, 12, 29, 0, 0, 3, 4, 4, 1, 0, 2, 0, 1, 0],
        [1, 0, 2, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0],
        [18, 7, 6, 6, 14, 7, 3, 10, 11, 1, 1, 4, 6, 3, 36, 4, 0, 3, 19, 20, 1, 1, 12, 0, 2, 0],
        [1, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    ]
    n = sum([sum(x) for x in pair_num])
    pair_freq = [[x * 100 / n for x in l] for l in pair_num]

    en_pair_freq_dict = {}
    for i, l in enumerate(pair_freq):
        for j, el in enumerate(l):
            en_pair_freq_dict[alphabet[i] + alphabet[j]] = pair_freq[i][j]

    return sort_dict(en_pair_freq_dict)


def get_pair_freq_dict(_ciphertext: str) -> dict:
    res = Counter()
    n = len(_ciphertext)
    for i in range(n - 1):
        el = _ciphertext[i:i + 2]
        res[el] += 1
    res = dict(res)
    for k, v in res.items():
        res[k] /= n
        res[k] *= 100
    return sort_dict(dict(res))


alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'

english_freq_space = {
    '*': 15.,
    'A': 8.167,
    'N': 6.749,
    'B': 1.492,
    'O': 7.507,
    'C': 2.782,
    'P': 1.929,
    'D': 4.253,
    'Q': 0.095,
    'E': 12.702,
    'R': 5.987,
    'F': 2.228,
    'S': 6.327,
    'G': 2.015,
    'T': 9.056,
    'H': 6.094,
    'U': 2.758,
    'I': 6.966,
    'V': 0.978,
    'J': 0.153,
    'W': 2.360,
    'K': 0.772,
    'X': 0.150,
    'L': 4.025,
    'Y': 1.974,
    'M': 2.406,
    'Z': 0.074,
}
english_freq_space = sort_dict(english_freq_space)

english_freq = {
    'A': 8.167,
    'N': 6.749,
    'B': 1.492,
    'O': 7.507,
    'C': 2.782,
    'P': 1.929,
    'D': 4.253,
    'Q': 0.095,
    'E': 12.702,
    'R': 5.987,
    'F': 2.228,
    'S': 6.327,
    'G': 2.015,
    'T': 9.056,
    'H': 6.094,
    'U': 2.758,
    'I': 6.966,
    'V': 0.978,
    'J': 0.153,
    'W': 2.360,
    'K': 0.772,
    'X': 0.150,
    'L': 4.025,
    'Y': 1.974,
    'M': 2.406,
    'Z': 0.074,
}  # staty z prezentacji
english_freq = sort_dict(english_freq)

en_pair_freq = get_en_pair_freq_dict()