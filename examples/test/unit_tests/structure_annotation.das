// options logStack = true

require UnitTest

[IntFields]
struct Fields
    a,b,c,d:int

[export,noaot]
def testFields(x:Fields?):int
    let t = 0
    t += x?.a ?? 0
    t += x?.b ?? 0
    t += x?.c ?? 0
    t += x?.d ?? 0
    return t

[export]
def test:bool
    UnitTest::testFields()
    return true

