require llvm

def add ( a : string )
    return 1 if a=="1"
    return 2 if a=="2"
    return -1

[export]
def main
    print("hello, world\n")
    var t = add("1")
    print("t = {t}\n")
