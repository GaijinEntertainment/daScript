[export,unsafe]
def test:bool
    let t = [[ int 1; 2; 3; 4; 5 ]]
    let pt = addr(t[0])
    assert(deref(pt)==1)
    assert(pt[0]==1 & pt[1]==2 & pt[2]==3 & pt[3]==4 & pt[4]==5)
    return true

