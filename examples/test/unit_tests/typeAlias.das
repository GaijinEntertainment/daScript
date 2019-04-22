require UnitTest

[export]
def test:bool
    var a = UnitTest::getSamplePoint3()
    assert(typeinfo(typename a) == "float3&")
    assert(UnitTest::doubleSamplePoint3(a) == float3(0,2,4))
    return true
