#!/usr/bin/env python3

import sys
from subprocess import Popen, PIPE, STDOUT


cmd = ['cryptsetup', 'luksOpen', '--test-passphrase', 'headers/hdr8.img', 'asd']

def main():
    try:
        wordlist = sys.argv[1]
    except Exception:
        print('daj wordliste bałwanie')

    # zakładam ze jest to w takim formacie jak hashcat wypluł
    with open(wordlist,'r') as handle:
        password_candidates = [x.split(':')[1] for x in handle.read().splitlines()]
    for passw in password_candidates:
        p = Popen(cmd, stdout=PIPE, stdin=PIPE, stderr=STDOUT)
        grep_stdout = p.communicate(input=passw.encode('utf-8'))
        if grep_stdout[0] != b'No key available with this passphrase.\n':
            print(f'WE GOT A HIT: {passw}')
            print(grep_stdout)
            break
        else:
            print(passw)


if __name__ == '__main__':
    main()
