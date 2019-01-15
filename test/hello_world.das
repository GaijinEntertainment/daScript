options log=true, optimize=true

struct Foo
    a:bool
    b:int
    c:bool

struct Bar
    a:bool
    m:float3x4
    c:bool

def info(t:auto(SType))
    print("info on {typename(type SType)}\n")
    t.a = false
    t.c = true
    debug(t,"debug t=")
    print("t={t}\n")
    print("sizeof(t)={sizeof(type SType)}\n")
    stackwalk()

[export]
def test
    let foo : Foo
    let bar : Bar
    info(foo)
    info(bar)
    return true
