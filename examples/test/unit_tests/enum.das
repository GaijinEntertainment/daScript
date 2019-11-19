require UnitTest

enum Numbers
    zero
    one
    two
    ten = 10
    sixteen = 0x10
    neg_ten = -10
    neg_one = 0xffffffff
    neg_one_i = -1

enum Zen
    zero
    not_zero

[export]
def test
    assert ( int(Numbers zero)==0 )
    assert ( int(Numbers two)==2 )
    assert ( int(Numbers ten)==10 )
    assert ( int(Numbers sixteen)==16 )
    assert ( int(Numbers neg_ten)==-10 )
    assert ( int(Numbers neg_one)==-1 )
    assert ( int(Numbers neg_one_i)==-1 )
    let t : Numbers
    assert ( t == Numbers zero )
    var q = Numbers one
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
    assert ( free_takeOne_giveTwo(SomeEnum one)==SomeEnum two )
    assert ( free_takeOne_giveTwo(SomeEnum two)==SomeEnum zero )
	verify ( efn_takeOne_giveTwo(SomeEnum one)==SomeEnum two )
	verify ( efn_takeOne_giveTwo_98(SomeEnum98 SomeEnum98_one)==SomeEnum98 SomeEnum98_two )
    var goo : GooEnum
    goo = GooEnum regular
    verify ( efn_flip(goo)==GooEnum hazardous )
    return true
