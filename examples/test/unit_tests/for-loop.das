[export]
def test:bool
    // 1 variable
    let(a:int[10];i:int=0)
        for l in a
            l = i
            i++
        assert(i==10)
        i = 0
        for l in a
            assert(l==i)
            i++
    // 1 variable array
    let(a:array<int>;i:int=0)
        resize(a,13)
        for l in a
            l = i++
        assert(i==13)
        i = 0
        for l in a
            assert(l==i)
            i++
    // 2 variable, array
    let(a,b:int[10];i:int=0)
        for l,r in a,b
            r = i*2-5
            l = i++
        assert(i==10)
        i = 0
        for l,r in a,b
            assert(r==i*2-5)
            assert(l==i++)
        i = 0
        for l,r in a,b
            if l>r
                assert(l>r)
                i++
        assert(i==5)
    // 2 variable, hybrid
    let(a:array<int>;b:int[10];i:int=0)
        resize(a,4)
        for l,r in a,b
            assert(i<4)
            r = i*2-5
            l = i++
        assert(i==4)
        i = 0
        for l,r in a,b
            assert(r==i*2-5)
            assert(l==i++)
        assert(i==4)
        i = 0
        for l,r in a,b
            if l>r
                assert(l>r)
                i++
        assert(i==4)
    // 1 variable, range
    let(i:int=0)
        for a in range(0,10)
            assert(a==i++)
        assert(i==10)
    // 2 variable, range hybrid
    let(b:int[10];i:int=0)
        for l,r in range(0,10),b    // note, this one is optimized out
            r = i++*2-5
        assert(i==10)
        i = 0
        for l,r in range(0,10),b
            assert(r==i*2-5)
            assert(l==i++)
        i = 0
        for l,r in range(0,10),b
            if l>r
                assert(l>r)
                i++
        assert(i==5)
    return true



