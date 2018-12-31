require testProfile

def testAdds():int
    let count:int = 0
    for i in range(0, 100000000)
        count = AddOne(count)
    return count

def test():bool
    profile(1,"native loop") <|
        assert(testAdds()==100000000)
    return true

