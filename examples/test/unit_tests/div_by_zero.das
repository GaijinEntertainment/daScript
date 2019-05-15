[sideeffects]
def div_by_zero(num:auto(numT))
    var ex = false
    try
        return num / numT(0)
    recover
        ex = true
    assert(ex)
    return num

[sideeffects]
def mod_by_zero(num:auto(numT))
    var ex = false
    try
        return num % numT(0)
    recover
        ex = true
    assert(ex)
    return num

[export]
def test()
    // int
    assert(div_by_zero(1)==1)
    assert(mod_by_zero(1)==1)
    // uint
    assert(div_by_zero(1u)==1u)
    assert(mod_by_zero(1u)==1u)
    // int64
    assert(div_by_zero(1l)==1l)
    assert(mod_by_zero(1l)==1l)
    // uint64
    assert(div_by_zero(1ul)==1ul)
    assert(mod_by_zero(1ul)==1ul)
    // float
    assert(div_by_zero(1.0f)==1.0f)
    assert(mod_by_zero(1.0f)==1.0f)
    // double
    assert(div_by_zero(1.0lf)==1.0lf)
    assert(mod_by_zero(1.0lf)==1.0lf)
    return true

