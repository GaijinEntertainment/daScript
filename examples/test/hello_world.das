options debugger = true

def funC(y:string)
    var q = y
    q += " before"
    stackwalk()
    q += " after"

def invBlock(z,blk)
    invoke(blk,z)

def funB(x:string)
    invBlock(x) <| $ (copyX : string)
        funC("from B {copyX}")

def funA(a:int;b:string)
    funB("{a} is {b}")

[export]
def test
    funA(1,"one")
    return true
