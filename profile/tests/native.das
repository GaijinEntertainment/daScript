require testProfile

def testAdds():int
    let count:int = 0
    for i in range(0, 10000000)
        count = AddOne(count)
    return count

def test():bool
    profile(20,"native loop") <|
        testAdds()
    return true

