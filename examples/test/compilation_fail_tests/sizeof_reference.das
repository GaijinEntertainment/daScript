expect 39901:2, 39902:2

def blah(a:auto(numt)&)
    return typeinfo(sizeof type numt)    // 39901: typeinfo(sizeof ref) is prohibited

def test
    typeinfo(sizeof type int&)           // 39901: typeinfo(sizeof ref) is prohibited
    let t = 1
    blah(t)
    typeinfo(sizeof type auto)           // 39902: typeinfo(sizeof auto) is undefined
    typeinfo(typename type auto)         // 39902: typeinfo(typename auto) is undefined

