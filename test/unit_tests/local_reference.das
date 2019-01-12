[export]
def test
    let a : int = 1
    let b : int& = a
    b = 2
    assert(a==2)
    let c : array<int>
    let d : array<int> & = c
    push(c,1)
    push(c,2)
    push(d,3)
    assert(length(c)==3 & length(d)==3)
    let e <- c
    assert(length(e)==3 & length(c)==0 & length(d)==0)
    return true
