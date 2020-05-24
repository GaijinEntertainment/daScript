require UnitTest

options log = true

[export,unsafe]
def test
    var t : TestObjectFoo
    t.foo_loop = addr(t)
    debug(t,"t=")
    return true
