#!/usr/bin/env python

import math

import gmpy2
from Crypto.Util.number import bytes_to_long, inverse, long_to_bytes

gmpy2.set_context(gmpy2.context())
gmpy2.get_context().precision = 128


def eve(c1, c2, c3, n1, n2, n3):
    g1 = math.gcd(n1, n2)
    g2 = math.gcd(n1, n3)
    g3 = math.gcd(n2, n3)
    crt = 1
    if g1 > 1 or g2 > 1:
        p = g1 if g1 > 1 else g2
        q = n1 // p
        n = n1
        c = c1
        crt = 0
    elif g3 > 1:
        p = g3
        q = n2 // p
        n = n2
        c = c2
        crt = 0
    if crt == 1:
        M = n1 * n2 * n3
        m_3 = (
            (c1 * (M // n1) * inverse(M // n1, n1))
            + (c2 * (M // n2) * inverse(M // n2, n2))
            + (c3 * (M // n3) * inverse(M // n3, n3))
        ) % M
        m = gmpy2.mpz(gmpy2.cbrt(m_3))
        print(f"scenario one: {long_to_bytes(m).decode()}")
    else:
        phi = math.lcm(p - 1, q - 1)
        d = inverse(3, phi)
        m = pow(c, d, n)
        print(f"scenario two: {long_to_bytes(m).decode()}")


def one():
    """Scenario when n1, n2, n3 are coprime."""
    e = 3

    # klucze 3 osób z zadania
    n1 = 145791506507802235152159488881964349614137635732219716168243748065243988068407777772385344319271170517847657730133759743597904267617831092951280753517183415068810803667549550296693427218562409698096101486375936844000731773719962334747525780017996556976595971714998133765892294760885060288321087687053667113561
    n2 = 57489787321652718974660095677270976973897274015625597819518684591343202275437327100337189390897526814140494254636538667298998577279477063540904937125762370801297573121090241032941184183983369727021576327981476540576405001150098183088971095499223217782468323030343094545795588464591264190982726369745412207609
    n3 = 81452311910111311009150887832748908486577799320033778624107381419306465755638952772535784843382705543014637258806309213081040408588923404694106188557275441521815810812436910174454792718359545189728309797916008227370970152096244900575419591630302647514649216058901720256677776126979627771049836430099655887847

    assert math.gcd(n1, n2) == 1 and math.gcd(n1, n3) == 1 and math.gcd(n2, n3) == 1

    m = bytes_to_long("hidden message".encode())
    c1 = pow(m, e, n1)
    c2 = pow(m, e, n2)
    c3 = pow(m, e, n3)

    eve(c1, c2, c3, n1, n2, n3)


def two():
    """Scenario when gcd of mods > 1"""
    e = 3
    n1 = 103436092830245214784313749434007551756120293641676256502649726765979993389387899055976812713845906098048285620100482098930884290569558690317531061953812287902645393398835010711727915623417230708108941980362985953678698119827138123008975068235766328879086222120392047715753631589352667233554839224572119410801
    n2 = 88568823861066139647996395393936483076521581494893428429346459398149283728048092746283224036741294351364240930223666146994648682116505023664194733340619563464213239095152715655436162950599201040743411392645297822383135931916325224283269454856096355879659792009039935464721732793436616019798495865297171491519
    n3 = 130838294861799661702097025882913222347808581186350305960496433359350531985795339713163418312775480059515767765133143748933019306643235373179750466693608647654839693960421421609603121816086307275136613423612819052164677135095868667529364981822726702080482595981090723660008856902612706878379906451660208604473
    m = bytes_to_long("hidden message".encode())
    c1 = pow(m, e, n1)
    c2 = pow(m, e, n2)
    c3 = pow(m, e, n3)

    eve(c1, c2, c3, n1, n2, n3)


if __name__ == "__main__":
    one()
    two()