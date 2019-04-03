require UnitTest
[export]
def test():bool
    print("{deref(getPtr())}")
    assert("{deref(getPtr())}"=="0")
    return true

