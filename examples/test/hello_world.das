options log=true// , logOptimizationPasses=true

def add(a,b:int):int
    return a + b

[export]
def test
    let fn = @add
    let t = invoke(fn, 1, 2)
    assert(t==3)
    let qq : function
    assert(fn!=qq)
    return true


