options log=true

def test
    let a:array<int>
    push(a,1)
    push(a,2)
    let b = a
    print("\na={a} b={b}\n")
    return true
