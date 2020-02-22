from random import randint

def get_matrix(i, j):
    m = []
    for _ in range(0, i):
        r = []
        for _ in range(0,j):
            r.append(randint(0, 2000000))
        m.append(r)
    return m

def get_thresholds():
    l = []
    for _ in range(0,100):
        l.append(randint(100000,2000000))
    return l
