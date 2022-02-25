import os
from glob import glob


class Color:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    WHITE = '\033[37m'


os.system("g++ tokenizer-en.cpp -o tokenizer-en.exe")
print("Compiling")
for i in range(1, 5):
    os.system(f"cp test/text{i}.in text.in")
    os.system(f"cp test/vocab{i}.in vocab.in")
    os.system("tokenizer-en.exe")
    result = os.system(f"diff result.out test/result{i}.out")
    if result == 0:
        print(Color.WHITE+f"Testcase{i}: "+Color.OKGREEN+"Accepted")
    else:
        print(Color.WHITE+f"Testcase{i}: "+Color.FAIL+"Wrong Answer")
