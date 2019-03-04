options logNodes = true

struct Foo
    x,y,z,w:int

[sideeffects]
def make_1
    let t:Foo
    t.x = 1
    t.y = 2
    t.z = 3
    t.w = 4
    return t

[sideeffects]
def make_2
    return [[Foo x=1, y=2, z=3, w=4]]

def test_1
    for x in range(0,100000)
        make_1()

def test_2
    for x in range(0,100000)
        make_2()

[export]
def test
    let i : int[10]
    for x in range(0,10)
        i[x] = x
    i[3] = 13
    print("i={i}\n")
    print("i[3]={i[3]}\n")
    let a : Foo[10]
    for x in range(0,10)
        a[x].y = x
    a[3].z = 13
    print("a={a}\n")
    print("a[3].z={a[3].z}\n")
    return true

