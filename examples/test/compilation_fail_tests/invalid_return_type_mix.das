expect 30102

struct Foo
    a:int

def func
    return $(a:int):int { return a+1; } // 30102: not a valid function return type

def test:bool
    func()
    return true

