#!/usr/bin/env python

# assume all p's are prime
def primitive_roots(p):
    roots = []
    phi = p - 1
    for i in range(2, p):
        k = 0
        for j in range(1, p):
            k = i ** j % p
            if k == 1 and j != phi:
                break
            elif k == 1 and j == phi:
                roots.append(i)

    return roots


roots = primitive_roots(13)
print("primitive roots = {}".format(roots))
print("dlog 2, 13 (11) = {}".format(2 ** 11 % 13))
