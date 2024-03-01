import random

def generate_random_permutation(n):
    permutation = list(range(n))
    for i in range(n - 1):
        k = 1
        while 2**k < n - i:
            k += 1

        while True:
            j = 0
            for _ in range(k):
                j = (j << 1) | random.randint(0, 1)

            if j < n - i:
                break

        permutation[i], permutation[i + j] = permutation[i + j], permutation[i]

    return permutation


n = int(input('Podaj n: '))

permutation = generate_random_permutation(n)
print("Losowa permutacja:", permutation)