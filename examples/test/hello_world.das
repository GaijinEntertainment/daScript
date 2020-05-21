// options log_cpp = true

require UnitTest

[export, unsafe]
def test

    var ascend_t <- new [[TestObjectFoo() fooData=1]]
    assert(ascend_t.fooData==1)
    delete ascend_t

    var ascend_smart_t <- new [[TestObjectSmart() fooData=1]]
    assert(ascend_smart_t.fooData==1)
    delete ascend_smart_t

    return true
