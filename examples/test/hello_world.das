options debugger = true

require UnitTest

// options log = true

def funC(y:string)
    var j = 13
    j ++
    stackwalk()
    var q = y
    q += " before {j}"
    // stackwalk()
    q += " after"

def invBlock(z,blk)
    invoke(blk,z)

def funB(x:string)
    if length(x) > 13
        var brr <- [{int 1;2;3;4}]
        push(brr,13)
        return <- brr
    else
        var arr <- [{int 1;2;3;4}]
        push(arr,length(x))
        invBlock(x) <| $ (copyX : string)
            funC("from B {copyX}")
        return <- arr

def funA(a:int;b:string)
    funB("{a} is {b}")

[export]
def test
    funA(1,"one")
    return true
