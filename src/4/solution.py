import re

def print_matches(text, match_strs):
    print(sum(re.match(s, text[N:], re.DOTALL) is not None 
              for N in range(len(text)) for s in match_strs))

with open("input") as f:
    text = f.read()
l = text.index('\n')
match_me = ["XMAS", "SAMX",
            "X.{%d}M.{%d}A.{%d}S" % (l, l, l),
            "X.{%d}M.{%d}A.{%d}S" % (l+1, l+1, l+1),
            "X.{%d}M.{%d}A.{%d}S" % (l-1, l-1, l-1),
            "S.{%d}A.{%d}M.{%d}X" % (l, l, l),
            "S.{%d}A.{%d}M.{%d}X" % (l+1, l+1, l+1),
            "S.{%d}A.{%d}M.{%d}X" % (l-1, l-1, l-1)]
match_me2 = ["M.S.{%d}A.{%d}M.S" % (l-1, l-1),
             "M.M.{%d}A.{%d}S.S" % (l-1, l-1),
             "S.S.{%d}A.{%d}M.M" % (l-1, l-1),
             "S.M.{%d}A.{%d}S.M" % (l-1, l-1)]

print_matches(text, match_me)
print_matches(text, match_me2)