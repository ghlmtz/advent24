import re

with open("input") as f:
    text = f.read()

def print_mul(text):
    print(sum(int(a) * int(b) for a,b in re.findall(r'mul\((\d+),(\d+)\)', text)))

print_mul(text)
text = re.sub(r'(.*?)don\'t\(\).*?do\(\)', r'\1', text, flags=re.DOTALL)
text = re.sub(r'don\'t\(\).*$', '', text, flags=re.DOTALL)
print_mul(text)