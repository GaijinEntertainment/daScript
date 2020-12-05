struct A
    a : int

struct AB
    a : int
    b : int

def finalize ( a : A )
    print("a = {a}\n")

def finalize ( a : AB )
    print("ab = {a}\n")

[export]
def test
    var a : A
    var ab : AB
    delete a
    delete ab
    return true
