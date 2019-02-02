require UnitTest

[export]
def test
    let t : TestObjectFoo
    t.fooData = 7
    let q = t.propAdd13
    print("t = {t}, q = {q}\n")
    return true
