expect 30101:4, 30108:3    // invalid_type

def test(A:void&)       // 30101: can't be void reference
    let a:int[10] &     // 30108: local variable can't be reference
                        // 30101: can't be array of references
    debug(a)
    let b:int & &       // 30108: local variable can't be reference
    debug(b)
    let c:int & ?       // 30101: can't be pointer to reference
                        // 30108: local variable can't be reference
    debug(c)
    let d:int & [10]    // 30101: can't be array of references
    debug(d)
