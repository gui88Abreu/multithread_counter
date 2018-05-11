'''
Testa o programa main multiplas vezes.
O numero de vezes deve ser passado pelo
terminal na linha de comando da seguinte forma:
python3 test.py n
onde n deve ser o numero de vezes desejado
'''

from os import system
import sys

num_tests = 0
try:
    if len(sys.argv) > 2:
        raise Exception()
    elif len(sys.argv) < 2:
        num_tests = 5
    else:
        num_tests = int(sys.argv[1])

except Exception:
    quit("It must be passed only one argument and it must be an integer")

system('make')
print(60*'-')
for i in range(num_tests):
    print("TESTE %d:" %(i+1))
    system('make test')
    print(60*'-')

system('make clean')
system('clear')