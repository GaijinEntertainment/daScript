options unit_test = 1.23 // this is here to test custom option

require UnitTest

[sideeffects]
def GetGoo(arg:string)
    return CheckEid(arg)

[sideeffects]
def GetGoo2(arg:string)
    return CheckEid2(arg)

[export]
def test
    let a = CheckEid("goo")
    let b = GetGoo("goo")
    assert(a==b)
    let a2 = CheckEid2("goo")
    let b2 = GetGoo2("goo")
    assert(a2==b2)
    return true

