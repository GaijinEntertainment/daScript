require UnitTest

require math

[sideeffects]
def take3(t:float3)
    print("t={t}")

[export]
def indexFail(arr:Point3Array)
    take3(arr[0])
    let t & = arr[1]
    take3(t)

[export]
def swizzleFail(battleAreaPoints:Point3Array)
    var pointStart : float2
    pointStart.x = battleAreaPoints[0].x
    print("{pointStart}")

[export]
def test:bool
    var a = UnitTest::getSamplePoint3()
    assert(typeinfo(typename a) == "float3")
    verify(UnitTest::doubleSamplePoint3(a) == float3(0,2,4))
    // also swizzle
    var foo : TestObjectFoo
    foo.hit = UnitTest::getSamplePoint3()
    assert(foo.hit.x==0.0 && foo.hit.y==1.0 && foo.hit.z==2.0)
    var qq = foo.hit
    assert(qq.x==0.0 && qq.y==1.0 && qq.z==2.0)
    var qq1 = UnitTest::getSamplePoint3()
    assert(qq1.x==0.0 && qq1.y==1.0 && qq1.z==2.0)
    let qq2 = foo.hitPos
    assert(qq2.x==0.0 && qq2.y==1.0 && qq2.z==2.0)
    var qq3 = foo.hitPos
    assert(qq3.x==0.0 && qq3.y==1.0 && qq3.z==2.0)
    var hitDir = normalize(-foo.hit |> UnitTest::doubleSamplePoint3())
    let nhd = normalize(-float3(0,2,4))
    assert(hitDir.x==nhd.x && hitDir.y==nhd.y && hitDir.z==nhd.z)
    hitDir = normalize(-foo.hit)
    let nhdh = normalize(-float3(0,1,2))
    assert(hitDir.x==nhdh.x && hitDir.y==nhdh.y && hitDir.z==nhdh.z)

    testPoint3Array() <| $(arr)
        assert(arr.length==10)
        for p in arr
            assert(p.x==1.0 && p.y==2.0 && p.z==3.0)

    return true
