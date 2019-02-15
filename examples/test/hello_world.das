options log=true// , logOptimizationPasses=true

def add(a,b:int):int
    return a + b

[export]
def test
    let fn = @add
    let t = invoke(fn, 1, 2)
    assert(t==3)
    assert(fn!=null)
    assert(fn==@add)
    assert(!(fn==null))
    assert(!(fn!=@add))
    let qq : function
    assert(qq==null)
    return true


