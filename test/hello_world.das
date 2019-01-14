// options log=true, logStack=true, optimize=false

struct Foo
    bar : int

def testRefArray
    let a : array<Foo>
    let x : Foo
    push(a,x)

def pass_array ( arr:array<int> )
    push(arr,4)

def verifyIntArray(a:array<int>;s,c,tail:int)
    for t in range(0,c)
        assert(a[t]==s++)
    for t in range(c,c+tail)
        assert(a[t]==0)

def testIntArray
    let arr,arr2:array<int>
    let val2 = 2
    assert(length(arr)==0,"array starts at 0 length")
    push(arr,1)
    push(arr,val2)
    push(arr,3)
    assert(length(arr)==3,"array length is 3")
    assert(capacity(arr)==16,"its 2x, but no less than 16")
    verifyIntArray(arr,1,3,0)
    pass_array(arr)
    assert(length(arr)==4,"array length is 4")
    verifyIntArray(arr,1,4,0)
    resize(arr,6)
    assert(length(arr)==6,"array length is now 6")
    verifyIntArray(arr,1,4,2)
    resize(arr,17)
    verifyIntArray(arr,1,4,13)
    assert(capacity(arr)==32,"now bigger")
    reserve(arr,33)
    verifyIntArray(arr,1,4,13)
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
    verifyIntArray(arr2,2,4,0)
    arr <- arr2
    verifyIntArray(arr,2,4,0)
    resize(arr,3)
    verifyIntArray(arr,2,3,0)
    resize(arr,4)
    assert(arr[3]==0,"new element after resize is 0")
    verifyIntArray(arr,2,3,1)
    push(arr,1,0)
    verifyIntArray(arr,1,4,1)
    resize(arr,4)
    push(arr,5,4)
    verifyIntArray(arr,1,5,0)
    push(arr,7,2)
    erase(arr,2)
    verifyIntArray(arr,1,5,0)
    erase(arr,0)
    erase(arr,3)
    verifyIntArray(arr,2,3,0)

[export]
def test
    testRefArray()
    testIntArray()
    return true
