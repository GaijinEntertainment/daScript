require UnitTest

typedef
    TestObjectSmartPtr = smart_ptr<TestObjectSmart>

[export]
def mks ()
    return [[TestObjectSmartPtr]]

[export,unsafe]
def test
    let p <- mks()
    verify((reinterpret<TestObjectSmart?> p)==null)
    return true
