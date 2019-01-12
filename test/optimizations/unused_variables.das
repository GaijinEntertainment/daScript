//options logOptimizationPasses=true
//options log=true
//options optimize=false,log=true

[export]
def test:bool
    let(i:int)
        assert(true)
    let(i,j:int)
        assert(j==0)
    let(a1,a2:int[20])
        for v1,v2 in a1,a2
            v1 = 1              // only this loop and a1 stays
    let(a1,a2:int[20])
        for v1,v2 in a1,a2
            assert(true)			// nothing. note, assumed to have no side-effects and will be optimized out
    return true

