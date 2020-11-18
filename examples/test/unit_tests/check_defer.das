require daslib/defer

def check_def(var x:int&)
    defer() <|
        assert(x==3)
        x --
    defer() <|
        assert(x==2)
        x *= 2
    x +=2

[export]
def test
    var x = 1
    check_def(x)
    assert(x==4)
    return true

