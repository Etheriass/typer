words = open ('./en.txt', 'r').read().split()
print(max(words, key=len))
# fire-extinguisher