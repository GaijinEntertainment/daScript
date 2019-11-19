require UnitTest

[sideeffects]
def GetGoo(arg:string)
    return CheckEid(arg)

[export]
def test
    let a = CheckEid("goo")
    let b = GetGoo("goo")
    assert(a==b)
    return true

