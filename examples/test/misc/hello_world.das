
def add ( a : string )
    return 1 if a=="1"
    return 2 if a=="2"
    return -1

def print_it ( i : int )
    print("i is {i}\n") if i != 13

[export]
def main
    print("hello, world\n")
    var t = add("1")
    print("t = {t}\n")
    print_it(1)
    print_it(13)
