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
	var qq = foo.hit
	assert(qq.x==0.0 & qq.y==1.0 & qq.z==2.0)
	var qq1 = UnitTest::getSamplePoint3()
	assert(qq1.x==0.0 & qq1.y==1.0 & qq1.z==2.0)
    let qq2 = foo.hitPos
    assert(qq2.x==0.0 & qq2.y==1.0 & qq2.z==2.0)
	var qq3 = foo.hitPos
	assert(qq3.x==0.0 & qq3.y==1.0 & qq3.z==2.0)
    return true
