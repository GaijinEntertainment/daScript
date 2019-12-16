options unit_test = 1.23 // this is here to test custom option

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

