require UnitTest

options log = true
options log_infer_passes = true

[export]
def sample (a : UnitTest::dasvector`TestObjectFoo; var b : UnitTest::dasvector`TestObjectFoo )
    debug(each(a))
    debug(each(b))

[export]
def test
    return true
