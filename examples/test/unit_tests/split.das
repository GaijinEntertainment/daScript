[sideeffects]
def expect0(a)
    assert(length(a)==0)

[sideeffects]
def expect1(a)
    assert(length(a)==1)
    assert(a[0]=="one")

[sideeffects]
def expect2(a)
    assert(length(a)==2)
    assert(a[0]=="one")
    assert(a[1]=="two")

[sideeffects]
def expect3(a)
    assert(length(a)==3)
    assert(a[0]=="one")
    assert(a[1]=="two")
    assert(a[2]=="three")

[export]
def test
    split("","") <| $ ( a )
        expect0(a)
    split("one",",") <| $ ( a )
        expect1(a)
    split("one,two",",") <| $ ( a )
        expect2(a)
    split("one,two.three",",.") <| $ ( a )
        expect3(a)
    expect0(split("",""))
    expect1(split("one",","))
    expect2(split("one,two",","))
    expect3(split("one,two.three",",."))
    return true


