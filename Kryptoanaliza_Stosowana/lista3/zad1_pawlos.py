#!/usr/bin/env python

from hashlib import sha256
from string import ascii_letters
from itertools import chain, combinations, product, dropwhile

match_prefix = '6c48427a19446954b7e2fbf0026106ef0494b231532e925b8c19d7926979f310'


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


def prefix_len(hash):
    sum = 0
    for s1, s2 in zip(hash, match_prefix):
        if s1 == s2:
            sum += 1
        else:
            break
    return sum


template = """<html>
<head>
<style type="text/css">
   p.{0} {{ font-style: italic; }}
</style>
</head>
<body>
<p class="{0}">Jest fatalnie!</p>
</body>
</html>
"""



log = open('sha256_script.log', 'a')
poww = dropwhile(lambda x: len(x) != 6, powerset(ascii_letters))
for alphabet in poww:
    log.write(f"al: {alphabet}\n")
    for variation in variations_with_rep(alphabet):
        sha_input = template.format(variation).encode()
        sha_out = sha256(sha_input)
        s = prefix_len(sha_out.hexdigest())
        if s >= 7:
            print(f'GOT A HIT: {variation} {s*4} bits')
            log.write(f'{variation} {s*4}\n')