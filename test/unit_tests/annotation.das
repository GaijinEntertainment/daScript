require UnitTest

[es (pass="ks")]
def update(pos:float3&;vel:float3 const&)   // all by ref
    pos += vel


[es (pass="ks2")]
def update2(pos:float3&;vel:float3 const)   // deref
    pos += vel

[es (pass="ks3")]
def update3(pos:float3&;vel:float3 const &;dt:float = 1.0f)
    pos += vel * float3(dt,dt,dt)   // TODO: vector * scalar

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
    //  profile(1000,"es-update") <|
    //      UnitTest::testEsUpdate("ks")
    //  profile(1000,"es-update-ref") <|
    //      UnitTest::testEsUpdate("ks2")
    //  profile(1000,"es-update-ref-const") <|
    //      UnitTest::testEsUpdate("ks3")
    return true
