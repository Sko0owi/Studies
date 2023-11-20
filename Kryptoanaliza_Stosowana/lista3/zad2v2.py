#!/usr/bin/env python

from hashlib import sha256
from string import ascii_letters
from itertools import chain, combinations, product, dropwhile


def join_str(strlist):
    return "".join(strlist)


def powerset(lst):
    """string powerset"""
    s = list(lst)
    return map(join_str, chain.from_iterable(combinations(s, r) for r in range(len(s)+1)))


def variations_with_rep(lst):
    """variations with repetition of list of strings"""
    repeat = len(lst)
    return map(join_str, product(lst, repeat=repeat))


# def prefix_len(hash):
#     sum = 0
#     for s1, s2 in zip(hash, match_prefix):
#         if s1 == s2:
#             sum += 1
#         else:
#             break
#     return sum


template = """<html>
<head>
<style type="text/css">
   p {{ color: {0}#{1}; }}
</style>
</head>
<body>
<p>Jest wspaniale!</p>
</body>
</html>
"""

calculated_hashes = {}

set_char = "0123456789abcdef"

log = open('sha256_script.log', 'a')
poww = dropwhile(lambda x: len(x) != 6, powerset(set_char))
for alphabet in poww:
    log.write(f"al: {alphabet} hash_size: {len(calculated_hashes)}\n")
    for variation in variations_with_rep(alphabet):
        for wow in range(5):
            for pog in range(5):
                sha_input = template.format(" "*wow, variation).encode()
                # print(sha_input)
                sha_out = sha256(sha_input)
                sha_out_pref = sha_out.hexdigest()[:16]
                if sha_out_pref in calculated_hashes and calculated_hashes[sha_out_pref] != (" "*wow, variation):
                    print(sha_out_pref)
                    print(calculated_hashes[sha_out_pref], variation)
                    exit()
                calculated_hashes[sha_out_pref] = (" "*wow, variation)
                variation += ' '
        # # s = prefix_len(sha_out.hexdigest())
        # if s >= 7:
        #     print(f'GOT A HIT: {variation} {s*4} bits')
        #     log.write(f'{variation} {s*4}\n')