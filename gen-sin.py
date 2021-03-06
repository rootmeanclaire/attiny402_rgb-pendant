from sys import argv
from math import sin, pi

LEN_TAB = 32

if len(argv) > 1:
    LEN_TAB = int(argv[1])

print("\n{", end="")

for i in range(LEN_TAB):
    print("{}".format(round(127 * (sin(i / LEN_TAB * 2 * pi) + 1))), end=("" if (i == LEN_TAB - 1) else ", "))

print("}\n")

