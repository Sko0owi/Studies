import matplotlib.pyplot as plt

alphabet = '*ABCDEFGHIJKLMNOPQRSTUVWXYZ'
alphalen = len(alphabet)



def encode(plaintext, key):
    alphabet = list(key)
    alphabet.sort()
    encdic = dict(zip(alphabet, key))
    return ''.join(map(lambda c: encdic[c], plaintext))

# def decode(ciphertext, key):
#     keylen = len(key)
#     plaintext = [decodeChar(ciphertext[n], key[n%keylen]) for n in range(len(ciphertext))]
#     return ''.join(plaintext)

charCnt = {}

toDecode = ('LPMUQEMQMGKLWDPMCYRHMHQXMLBMQSKLRMQBHMPDVMCRYCJEMUVKVMEPKLJLBWMPDLKPVVBMULBEPYBM'
            'EILPDMDLEMCDLBMBAZZRVHMLBPYMDLEMGKVQEPMLBMQBMVOOYKPMPYMVECQSVMPDVM*LRVMULBHMERLS'
            'SVHMTALCJRXMPDKYAWDMPDVMWRQEEMHYYKEMYOM*LCPYKXMIQBELYBEMPDYAWDMBYPMTALCJRXMVBYAW'
            'DMPYMSKV*VBPMQMEULKRMYOMWKLPPXMHAEPMOKYIMVBPVKLBWMQRYBWMULPDMDLIMPDVMDQRRUQXMEIV'
            'RPMYOMGYLRVHMCQGGQWVMQBHMYRHMKQWMIQPEMQPMYBVMVBHMYOMLPMQMCYRYAKVHMSYEPVKMPYYMRQK'
            'WVMOYKMLBHYYKMHLESRQXMDQHMGVVBMPQCJVHMPYMPDVMUQRRMLPMHVSLCPVHMELISRXMQBMVBYKIYAE'
            'MOQCVMIYKVMPDQBMQMIVPKVMULHVMPDVMOQCVMYOMQMIQBMYOMQGYAPMOYKPXMOL*VMULPDMQMDVQ*XM'
            'GRQCJMIYAEPQCDVMQBHMKAWWVHRXMDQBHEYIVMOVQPAKVEMULBEPYBMIQHVMOYKMPDVMEPQLKEMLPMUQ'
            'EMBYMAEVMPKXLBWMPDVMRLOPMV*VBMQPMPDVMGVEPMYOMPLIVEMLPMUQEMEVRHYIMUYKJLBWMQBHMQPM'
            'SKVEVBPMPDVMVRVCPKLCMCAKKVBPMUQEMCAPMYOOMHAKLBWMHQXRLWDPMDYAKEMLPMUQEMSQKPMYOMPD'
            'VMVCYBYIXMHKL*VMLBMSKVSQKQPLYBMOYKMDQPVMUVVJMPDVMORQPMUQEMEV*VBMORLWDPEMASMQBHMU'
            'LBEPYBMUDYMUQEMPDLKPXMBLBVMQBHMDQHMQM*QKLCYEVMARCVKMQGY*VMDLEMKLWDPMQBJRVMUVBPME'
            'RYURXMKVEPLBWMEV*VKQRMPLIVEMYBMPDVMUQXMYBMVQCDMRQBHLBWMYSSYELPVMPDVMRLOPMEDQOPMP'
            'DVMSYEPVKMULPDMPDVMVBYKIYAEMOQCVMWQZVHMOKYIMPDVMUQRRMLPMUQEMYBVMYOMPDYEVMSLCPAKV'
            'EMUDLCDMQKVMEYMCYBPKL*VHMPDQPMPDVMVXVEMOYRRYUMXYAMQGYAPMUDVBMXYAMIY*VMGLWMGKYPDV'
            'KMLEMUQPCDLBWMXYAMPDVMCQSPLYBMGVBVQPDMLPMKQBM')

for char in toDecode:
    if char in charCnt:
        charCnt[char] += 1
    else:
        charCnt[char] = 1

wzor = {' ': 220, 'E': 196, 'T': 140, 'O': 139, 'A': 123, 'I': 122, 'N': 113, 'S': 106, 'H': 89, 'R': 87, 'L': 74, 'D': 62, 'W': 58, 'C': 58, 'F': 39, 'U': 37, 'G': 37, 'Y': 36, 'M': 30, 'P': 30, 'B': 21, 'V': 14, 'K': 8, 'Z': 3, 'Q': 2, 'J': 2, 'X': 1}
x = sorted(charCnt.items(), key = lambda x: x[1], reverse=True)

names = []
values = []

names_wzor = []
values_wzor = []

for item in x:
    names.append(item[0])
    values.append(item[1])

y = sorted(wzor.items(), key = lambda x: x[1], reverse=True)

print(dict(y))

for item in y:
    names_wzor.append(item[0])
    values_wzor.append(item[1])

key = {}


for i in range(len(x)):
    if names[i] not in key:
        key[names[i]] = names_wzor[i]
    
print(key)

decoded = ""
for char in toDecode:
    decoded += key[char]

print(decoded)

plt.subplot(131)
plt.bar(names, values)
plt.subplot(132)
plt.bar(names_wzor, values_wzor)
plt.show()

print(sorted(charCnt.items(), key = lambda x: x[1], reverse=True))

print(toDecode)

POG = "IT WAS A BRIGHT COLD DAY IN APRIL AND THE CLOCKS WERE STRIKING THIRTEEN WINSTON SMITH HIS CHIN NUZZLED INTO HIS BREAST IN AN EFFORT TO ESCAPE THE VILE WIND SLIPPED QUICKLY THROUGH THE GLASS DOORS OF VICTORY MANSIONS THOUGH NOT QUICKLY ENOUGH TO PREVENT A SWIRL OF GRITTY DUST FROM ENTERING ALONG WITH HIM THE HALLWAY SMELT OF BOILED CABBAGE AND OLD RAG MATS AT ONE END OF IT A COLOURED POSTER TOO LARGE FOR INDOOR DISPLAY HAD BEEN TACKED TO THE WALL IT DEPICTED SIMPLY AN ENORMOUS FACE MORE THAN A METRE WIDE THE FACE OF A MAN OF ABOUT FORTY FIVE WITH A HEAVY BLACK MOUSTACHE AND RUGGEDLY HANDSOME FEATURES WINSTON MADE FOR THE STAIRS IT WAS NO USE TRYING THE LIFT EVEN AT THE BEST OF TIMES IT WAS SELDOM WORKING AND AT PRESENT THE ELECTRIC CURRENT WAS CUT OFF DURING DAYLIGHT HOURS IT WAS PART OF THE ECONOMY DRIVE IN PREPARATION FOR HATE WEEK THE FLAT WAS SEVEN FLIGHTS UP AND WINSTON WHO WAS THIRTY NINE AND HAD A VARICOSE ULCER ABOVE HIS RIGHT ANKLE WENT SLOWLY RESTING SEVERAL TIMES ON THE WAY ON EACH LANDING OPPOSITE THE LIFT SHAFT THE POSTER WITH THE ENORMOUS FACE GAZED FROM THE WALL IT WAS ONE OF THOSE PICTURES WHICH ARE SO CONTRIVED THAT THE EYES FOLLOW YOU ABOUT WHEN YOU MOVE BIG BROTHER IS WATCHING YOU THE CAPTION BENEATH IT RAN"

POOOG = ('It was a bright cold day in April, and the clocks were striking thirteen. Winston Smith, his chin nuzzled into his '
'breast in an effort to escape the vile wind, slipped quickly through the glass doors of Victory Mansions, though '
'not quickly enough to prevent a swirl of gritty dust from entering along with him. '
'The hallway smelt of boiled cabbage and old rag mats. At one end of it a coloured poster, too large for indoor '
'display, had been tacked to the wall. It depicted simply an enormous face, more than a metre wide: the face of a '
'man of about forty five, with a heavy black moustache and ruggedly handsome features. Winston made for the '
'stairs. It was no use trying the lift. Even at the best of times it was seldom working, and at present the electric '
'current was cut off during daylight hours. It was part of the economy drive in preparation for Hate Week. The '
'flat was seven flights up, and Winston, who was thirty nine and had a varicose ulcer above his right ankle, went '
'slowly, resting several times on the way. On each landing, opposite the lift   shaft, the poster with the enormous '
'face gazed from the wall. It was one of those pictures which are so contrived that the eyes follow you about when '
'you move. big brother is watching you, the caption beneath it ran.')

POOOG = POOOG.upper()

POOOG2 =""
for char in POOOG:
    if char in alphabet or char == ' ' or char == '\n':
        if char == '\n': POOOG2 += ' '
        else : POOOG2 += char

print(POOOG2)

print(len(POOOG2), len(POG))

for i in range(len(POG)):
    if(POG[i] != POOOG2[i]):
        print(POG[:i+1])
        print(POOOG2[:i+1])
        break

print(POOOG2 == POG)
