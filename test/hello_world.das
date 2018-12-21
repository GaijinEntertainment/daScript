require UnitTest

[IntFields]
struct Fields
    a,b,c,d:int

def testFields(x:Fields?):int
    let(t:int)
        t = 0
        t += x?.a ?? 0
        t += x?.b ?? 0
        t += x?.c ?? 0
        t += x?.d ?? 0
        // debug(x)
        return t

[es (pass="ks")]
def update(pos:float3&;vel:float3 const &)
    pos += vel

def test:bool
    UnitTest::testFields()
    UnitTest::initEsComponents()
    UnitTest::testEsUpdate("ks")
    UnitTest::verifyEsComponents()
    profile(1000,"es-update") <|
        UnitTest::testEsUpdate("ks")
    return true


