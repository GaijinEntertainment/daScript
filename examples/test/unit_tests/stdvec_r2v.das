require UnitTest

[export]
def test
    testFooArray() <| $(arr)
        for i in range(0,arr.length)
            if arr[i].fooData != i
                panic("bad")
    return true
