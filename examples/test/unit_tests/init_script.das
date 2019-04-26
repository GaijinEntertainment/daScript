var
    value = 0

[init]
def c1
    value++

[init,noaot]
def c2
    value+=2

[export]
def test
    assert(value==3)
    return true


