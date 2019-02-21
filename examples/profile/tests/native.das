// options log=true, logNodes=true, printVarAccess=true, printRef=true

require testProfile

[sideeffects]
def testAdds()
    let count = 0
    for i in range(0, 10000000)
        count = AddOne(count)
    return count

[export]
def test()
    profile(20,"native loop") <|
        testAdds()
    return true

