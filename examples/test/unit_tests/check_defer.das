require daslib/defer

def check_def(var x:int&)
    defer() <|
        // 2nd
        assert(x==2)
        x *= 2
    defer() <|
        // 1st
        assert(x==3)
        x --
    x +=2

[export]
def test
    var x = 1
    check_def(x)
    assert(x==4)
    return true

