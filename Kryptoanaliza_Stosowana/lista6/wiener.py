import gmpy2
from Crypto.Util.number import bytes_to_long


def _cf_exp(n, d):
    e = []

    q = n // d
    r = n % d
    e.append(q)

    while r != 0:
        n, d = d, r
        q = n // d
        r = n % d
        e.append(q)

    return e


def _convergents(e):
    n = []  # Nominators
    d = []  # Denominators

    for i in range(len(e)):
        if i == 0:
            ni = e[i]
            di = 1
        elif i == 1:
            ni = e[i] * e[i - 1] + 1
            di = e[i]
        else:  # i > 1
            ni = e[i] * n[i - 1] + n[i - 2]
            di = e[i] * d[i - 1] + d[i - 2]

        n.append(ni)
        d.append(di)
        yield (ni, di)


def _cf_exp_gmpy2(n, d):
    e = []

    q = gmpy2.floor_div(n, d)
    r = gmpy2.f_mod(n, d)
    e.append(q)

    while not gmpy2.is_zero(r):
        n, d = d, r
        q = gmpy2.floor_div(n, d)
        r = gmpy2.f_mod(n, d)
        e.append(q)

    return e


def _convergents_gmpy2(e):
    n = []  # Nominators
    d = []  # Denominators

    for i in range(len(e)):
        if i == 0:
            ni = e[i]
            di = 1
        elif i == 1:
            # ni = e[i] * e[i - 1] + 1
            ni = gmpy2.add(gmpy2.mul(e[i], e[i - 1]), 1)
            di = e[i]
        else:  # i > 1
            # ni = e[i] * n[i - 1] + n[i - 2]
            # di = e[i] * d[i - 1] + d[i - 2]
            ni = gmpy2.add(gmpy2.mul(e[i], n[i - 1]), n[i - 2])
            di = gmpy2.add(gmpy2.mul(e[i], d[i - 1]), d[i - 2])

        n.append(ni)
        d.append(di)
        yield (ni, di)


def wiener(n, e):
    """Wiener's Attack, assuming d is small"""

    def maybe_return_key(maybe_d):
        msg = bytes_to_long("asd".encode("utf-8"))
        if gmpy2.powmod(gmpy2.powmod(msg, e, n), maybe_d, n) == msg:
            return maybe_d
        return None

    for m_k, m_d in _convergents_gmpy2(_cf_exp_gmpy2(e, n)):
        if m_k == 0:
            continue
        if maybe_return_key(m_d) == m_d:
            return m_d
    return None
