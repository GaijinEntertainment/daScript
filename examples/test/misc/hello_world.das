struct Foo
    a : int

struct Bar
    a : float

def operator as Bar ( a : Foo )
    return [[Bar a = float(a.a)]]

def operator ?as Bar ( a : Foo? ) : Bar?
    if a == null
        return null
    return new [[Bar a = float(a.a)]]

def operator is Bar ( anything )
    return false

def operator is Bar ( a : Bar )
    return true

[export]
def main
    var f = [[Foo a= 13]]
    print("f = {f}\n")
    var b = f as Bar
    print("b = {b}\n")
    print("b is Bar = {b is Bar}\n")
    print("f is Bar = {f is Bar}\n")
    var pf = unsafe(addr(f))
    var pb = pf ?as Bar ?? [[Bar a = 0.0]]
    print("pb = {pb}\n")
    var nf : Foo?
    var nb = nf ?as Bar ?? [[Bar a = 0.0]]
    print("nb = {nb}\n")

