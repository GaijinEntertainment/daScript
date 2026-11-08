options gen2
options unit_test = 1.23 // this is here to test custom option

require UnitTest
require dastest/testing_boost public

var {
    foo : TestObjectFoo
}

[sideeffects]
def GetGoo(arg : string) => CheckEid(foo, arg)

[sideeffects]
def GetGoo2(arg : string) => CheckEid2(foo, arg)

[test(description = "a C++-bound function called directly and through a daslang wrapper returns the same result, in a program carrying a custom module option")]
def test_check_eid(t : T?) {
    let a = CheckEid(foo, "goo")
    let b = GetGoo("goo")
    t |> equal(a, b)
    let a2 = CheckEid2(foo, "goo")
    let b2 = GetGoo2("goo")
    t |> equal(a2, b2)
}

