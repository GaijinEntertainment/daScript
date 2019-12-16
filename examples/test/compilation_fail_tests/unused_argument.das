expect 40206:2

[sideeffects]
def haha(a,b,c)                 // 40206: unused function argument b
                                // 40206: unused function argument c
    return a * 2

[export]
def test
    let t = haha(1,2.0,"blah")
    assert(t==2)
    return true

