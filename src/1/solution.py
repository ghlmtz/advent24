import re

with open("input") as f:
    lines = [list(map(int, re.findall(r'\d+', line))) for line in f.readlines()]
    first = sorted(line[0] for line in lines)
    second = sorted(line[1] for line in lines)
    print(sum(abs(x - y) for (x, y) in zip(first, second)))
    print(sum(second.count(x)*x for x in first))