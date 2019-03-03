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
    profile(20,"make t.x=1;...") <|
        test_1()
    profile(20,"make [[Foo x=1;...]]") <|
        test_2()
    return true

