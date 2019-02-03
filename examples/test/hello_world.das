// options log = true, optimize = true // , logInferPasses = true, logOptimizationPasses = true

require UnitTest

enum Numbers
    zero
    one
    two
    ten = 10

enum Zen
    zero
    not_zero

[export]
def test
    assert ( int(Numbers ten)==10 )
    let t : Numbers
    assert ( t == Numbers zero )
    let q = Numbers one
    assert ( int(q)==1 )
    assert ( q == Numbers one )
    assert ( t != q )
    q = Numbers zero
    assert ( t == q )
    assert ( t != Numbers one )
    assert ( Zen zero != Zen not_zero )
    assert ( Zen zero == Zen zero )
    assert ( int(t)==0 )
    assert ( int(Zen not_zero)==1 )
	assert ( efn_takeOne_giveTwo(SomeEnum one)==SomeEnum two )
	assert ( efn_takeOne_giveTwo_98(SomeEnum_98 one)==SomeEnum_98 two )
    return true
