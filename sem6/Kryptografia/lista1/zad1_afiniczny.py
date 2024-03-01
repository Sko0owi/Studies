from math import gcd as bltin_gcd

toDecode = ('KQEREJEBCPPCJCRKIEACUZBKRVPKRBCIBQCARBJCVFCUP'
            'KRIOFKPACUZQEPBKRXPEIIEABDKPBCPFCDCCAFIEABDKP'
            'BCPFEQPKAZBKRHAIBKAPCCIBURCCDKDCCJCIDFUIXPAFF'
            'ERBICZDFKABICBBENEFCUPJCVKABPCYDCCDPKBCOCPERK'
            'IVKSCPICBRKIJPKABI')

toDecode2 = ('BNVSNSIHQCEELSSKKYERIFJKXUMBGYKAMQLJTYAVFBKVT'
            'DVBPVVRJYYLAOKYMPQSCGDLFSRLLPROYGESEBUUALRWXM'
            'MASAZLGLEDFJBZAVVPXWICGJXASCBYEHOSNMULKCEAHTQ'
            'OKMFLEBKFXLRRFDTZXCIWBJSICBGAWDVYDHAVFJXZIBKC'
            'GJIWEAHTTOEWTUHKRQVVRGZBXYIREMMASCSPBNLHJMBLR'
            'FFJELHWEYLWISTFVVYFJCMHYUYRUFSFMGESIGRLWALSWM'
            'NUHSIMYYITCCQPZSICEHBCCMZFEGVJYOCDEMMPGHVAAUM'
            'ELCMOEHVLTIPSUYILVGFLMVWDVYDBTHFRAYISYSGKVSUU'
            'HYHGGCKTMBLRX')


ANS = ('O CANADA TERRE DE NOS AIEUX TON FRONT EST CEINT DE FLEURONS GLORIEUX'
        'CAR TON BRAS SAIT PORTER LEPEE IL SAIT PORTER LA CROIX TON HISTOIRE EST '
        'UNE EPOPEE DES PLUS BRILLANTS EXPLOITS ET TA VALEUR DE FOI TREMPEE'
        'PROTEGERA NOS FOYERS ET NOS DROITS')


alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
alphabetSize = len(alphabet)
assert(alphabetSize == 26)

revMap = {}
for i,ch in enumerate(alphabet):
    revMap[ch] = i

possible_a = []
for i in range(alphabetSize):
    if bltin_gcd(i, alphabetSize) == 1:
        possible_a.append(i) 

print(possible_a)


def decode(znak, a, b, mod):
    a = pow(a, -1, mod)
    znak = revMap[znak]
    tmp = (a * (znak - b)) % mod 
    return alphabet[tmp]

for a in possible_a:
    for b in range(alphabetSize):
        ans = []
        for ch in toDecode2:
            ans.append(decode(ch,a,b,alphabetSize))

        print(f"for a: {a:02}, b: {b:02}, decoded: {''.join(ans)}")