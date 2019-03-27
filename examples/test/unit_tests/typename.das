// options log=true,logInferPasses=false,optimize=true

struct Foo
    bar : int

def testInfT(a:auto(numT))
    return typeinfo(typename type numT)

def testVarRefT(a)
    return typeinfo(typename a)

def testVarValT(a)
    return typeinfo(typename a+0)

def arrType(a:array<auto(arrT)>)
    return typeinfo(typename type arrT)

def tabTypes(a:table<auto(key),auto(value)>)
    return typeinfo(typename type key) + ":" + typeinfo(typename type value)

[export]
def test:bool
    let x : auto(numT)  = 3
    assert(typeinfo(typename type numT)=="int")
    assert(testInfT(3)=="int")
    assert(testVarRefT(3)=="int&")
    assert(testVarValT(3)=="int")
    let y : table<string,int>
    assert(tabTypes(y)=="string:int")
    let z : array<float>
    let ZZ : array<Foo>
    assert(arrType(z)=="float")
    assert(arrType(ZZ)=="Foo")
    return true

