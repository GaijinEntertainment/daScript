options log=true

[export]
def test
    let a : table<int,int>
    let t = 1000500
    // resize(a,t)
    for q in range(0,t)
        a[q] = q
    return true
