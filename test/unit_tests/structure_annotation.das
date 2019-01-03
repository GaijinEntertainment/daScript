// options log = true

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
        return t

def test:bool
    UnitTest::testFields()
    return true

