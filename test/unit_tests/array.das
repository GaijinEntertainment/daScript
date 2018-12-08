def make_test_array(a:array<int>;extra:int=0)
    let (i:int=0)
        resize(a,10)
        for x in a
            x = i++ + extra

def verify_test_array(a:array<int>;extra:int=0)
    assert(length(a)==10)
    let(i:int=0)
        for x in a
            assert(x==(extra+i++))

def test:bool
    let(a:array<int>)
        make_test_array(a)
        verify_test_array(a)
        let(b:array<int> = a)
            assert(length(a)==0)
            verify_test_array(b)
            a <- b
        verify_test_array(a)
    let(a,b:array<int>;i:int=0)
        resize(a,10)
        resize(b,10)
        for l,r in a,b
            l = i
            r = i*2-5
            i++
        verify_test_array(a)
        i = 0
        for l,r in a,b
            assert(l==i)
            assert(r==i*2-5)
            i++
        i = 0
        for l,r in a,b where l>r
            assert(l>r)
            i++
        assert(i==5)
    let(a:array<int>[10]; i:int=0)
        for b in a
            make_test_array(b,i++)
        i = 0
        for b in a
            verify_test_array(b,i++)
    return true




