[export]
def test
    let arr <- [{for x in range(10); x}]
    lock_data(arr) <| $(data, size)
        for j in range(10)
            unsafe
                assert(data[j]==j)
    var arr2 <- [{for x in range(10); x}]
    lock_data(arr2) <| $(data, size)
        for j in range(10)
            unsafe
                data[j] = j*2
    for j in range(10)
        assert(arr2[j]==j*2)
    return true

