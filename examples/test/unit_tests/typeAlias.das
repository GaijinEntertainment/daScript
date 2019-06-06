require UnitTest

[export]
def test:bool
    var a = UnitTest::getSamplePoint3()
    assert(typeinfo(typename a) == "float3")
    verify(UnitTest::doubleSamplePoint3(a) == float3(0,2,4))
    return true
