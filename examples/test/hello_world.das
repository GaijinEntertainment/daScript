require UnitTest

options log = true

[export,unsafe]
def test
    /*
    TestObjectFoo t
    t.foo_loop = addr(t)
    print("t = {t}\n")
     */

    var b = 1.0
    b = 0.0
    print("{testGetDiv(0.0,b)}\n")
    return true
