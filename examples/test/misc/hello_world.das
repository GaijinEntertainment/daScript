
def add ( a : string )
    return 1 if a=="1"
    return 2 if a=="2"
    return 3 if a=="3" else print("not 1,2,3\n")
    return 4 if a=="4" else return 100500
    return -1

def print_it ( i : int )
    print("i is {i}\n") if i != 13
    print("i is 13\n") if i == 13 else print("i is not 13\n")
    print("i is BOO\n") if i == 13 else return

[export]
def main
    print("hello, world\n")
    var t = add("1")
    print("t = {t}\n")
    print_it(1)
    print_it(13)
