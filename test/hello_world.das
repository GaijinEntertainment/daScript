options log=true, logInferPasses=false, optimize=true


def add2(bl:block<int&>)
    invoke(bl) += 2

def test
    let x = 1
    add2() <| $():auto&
        return x
    assert(x==3)
    return true
