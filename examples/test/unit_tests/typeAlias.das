require UnitTest

[export]
def test:bool
    var a = UnitTest::getSamplePoint3()
    assert(typeinfo(typename a) == "float3")
    verify(UnitTest::doubleSamplePoint3(a) == float3(0,2,4))
    // also swizzle
    var foo : TestObjectFoo
    foo.hit = UnitTest::getSamplePoint3()
    assert(foo.hit.x==0.0 & foo.hit.y==1.0 & foo.hit.z==2.0)
    return true
