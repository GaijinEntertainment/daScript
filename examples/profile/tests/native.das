// options log=true, logNodes=true, printVarAccess=true, printRef=true
// options logCpp=true
require testProfile

[sideeffects]
def testAdds()
    var count = 0
    for i in range(0, 10000000)
        count = AddOne(count)
    return count

[export]
def test()
	var count = 0
    profile(20,"native loop") <|
        count = testAdds()
	assert(10000000==count)
    return true

