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

def pass_array ( arr:array<int> )
    push(arr,4)

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
    let ( arr:array<int>; arr2:array<int>; val2:int=2 )
        assert(length(arr)==0,"array starts at 0 length")
        push(arr,1)
        push(arr,val2)
        push(arr,3)
        assert(length(arr)==3,"array length is 3")
        assert(capacity(arr)==16,"its 2x, but no less than 16")
        pass_array(arr)
        assert(length(arr)==4,"array length is 4")
        resize(arr,6)
        assert(length(arr)==6,"array length is now 6")
        resize(arr,17)
        assert(capacity(arr)==32,"now bigger")
        reserve(arr,33)
        assert(capacity(arr)==33,"now even bigger")
        resize(arr,4)
        assert(capacity(arr)==33,"did not grow smaller")
        assert(length(arr)==4,"resized properly")
        arr2 <- arr
        assert(length(arr)==0)
        assert(length(arr2)==4)
        let ( index:int = 0 )
            while index < 4
                arr2[index++]++
        arr <- arr2
        resize(arr,3)
        resize(arr,4)
        assert(arr[3]==0,"new element after resize is 0")
        push(arr,1,0)
        resize(arr,4)
        push(arr,5,4)
        push(arr,7,2)
        assert(length(arr)==6)
        assert(arr[2]==7 & arr[5]==5)
        erase(arr,2)
        erase(arr,0)
        erase(arr,3)
        assert(length(arr)==3)
        assert(arr[0]==2 & arr[1]==3 & arr[2]==4)
    return true




