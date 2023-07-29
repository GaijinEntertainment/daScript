def hello(): bool
    print("hello\n")
    return false

def printVar(i: int)
    print("{i}\n")

def printVarRef(i: int&)
    print("{i}\n")

def setVar(var i: int&)
    i = i + 2

[export]
def main
    var b = hello()
    print(b ? "Y" : "N")
    print("\n")
    printVar(4)