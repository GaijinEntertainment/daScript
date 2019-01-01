require UnitTest

[es (pass="ks")]
def update(pos:float3&;vel:float3 const&)   // all by ref
    pos += vel


[es (pass="ks2")]
def update2(pos:float3&;vel:float3 const)   // deref
    pos += vel

[es (pass="ks3")]
def update3(pos:float3&;vel:float3 const &;dt:float = 1.0f)
    pos += vel * dt

def queryOne(dt:float=1.0f)
    UnitTest::queryEs() <| $ [es] (pos:float3&;vel:float3 const)
        pos += vel * dt

def queryTwo(dt:float=0.5f)
    UnitTest::queryEs() <| $ [es] (pos:float3&;vel:float3 const;defT:float=0.5f)
        pos += vel * (defT + dt)

def queryThreeFast()
    let count : int = 0
    try
        UnitTest::queryEs() <| $ [es] (pos:float3& const)
            let position : float3 = pos
            UnitTest::queryEs() <| $ [es] (pos:float3& const)
                if position==pos
                    count ++
                    if count == 100
                        throw("already a 100")  // note, this is here to speed up default test
    catch
        assert(count==100)

def queryThree()
    let count : int = 0
    UnitTest::queryEs() <| $ [es] (pos:float3& const)
        let position : float3 = pos
        UnitTest::queryEs() <| $ [es] (pos:float3& const)
            if position==pos
                count ++
    assert(count==10000)

def test:bool
    UnitTest::initEsComponents()
    UnitTest::testEsUpdate("ks")
    UnitTest::verifyEsComponents()
    UnitTest::initEsComponents()
    UnitTest::testEsUpdate("ks2")
    UnitTest::verifyEsComponents()
    UnitTest::initEsComponents()
    UnitTest::testEsUpdate("ks3")
    UnitTest::verifyEsComponents()
    UnitTest::initEsComponents()
    queryOne()
    UnitTest::verifyEsComponents()
    UnitTest::initEsComponents()
    queryTwo()
    UnitTest::verifyEsComponents()
    UnitTest::initEsComponents()
    queryThreeFast()
    // profile(1000,"es-update") <|
    //     UnitTest::testEsUpdate("ks")
    // profile(1000,"es-update-ref") <|
    //     UnitTest::testEsUpdate("ks2")
    // profile(1000,"es-update-ref-const") <|
    //     UnitTest::testEsUpdate("ks3")
    // profile(1000,"query-1") <|
    //     queryOne()
    // profile(1000,"query-2") <|
    //     queryTwo()
    // profile(1,"query-3") <|
    //     queryThree()
    return true
