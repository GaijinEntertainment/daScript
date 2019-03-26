// options log=true, logGenerics=false, optimize=true

def logT(x:auto(numT)&)
    return typename(type numT)

def logTNoRef(x:auto(numT)delete &)
    return typename(type numT)

def logTNoRefNoConst(x:auto(numT) delete const&)
    return typename(type numT)

def aliasLogT(x:auto(numT)&;y:numT)
    return typename(type numT)

def aliasLogTNoRef(x:auto(numT);y:numT delete &)
    let z : auto(passT) = y
    return typename(type passT)

def aliasLogTNoRefNoConst(x:auto(numT);y:numT delete const&)
    let z : auto(passT) = y
    return typename(type passT)

[export]
def test
    let x : auto const = 1
    assert(logT(x)=="int const&")
    assert(logTNoRef(x)=="int const")
    assert(logTNoRefNoConst(x)=="int")
    assert(aliasLogT(x,x)=="int const&")
    assert(aliasLogTNoRef(x,x)=="int const")
    assert(aliasLogTNoRefNoConst(x,x)=="int")
    return true
