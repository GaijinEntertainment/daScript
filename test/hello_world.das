def foo
    assert(true)

def foo(a:int)
    debug(a)

def add(a,b:int):int
    return a+b

def add(a:int&;b:int):int
    return a+b

def test:bool
    let a:int = 1;
    foo(a,1)
    add(a,1)
    return true
