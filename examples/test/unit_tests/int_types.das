options optimize=false

[export]
def test
    let i = 100500
    let li = 100500100500100500l
    let u = 14u
    let ul = 100500100500100500ul
    let uh = 0x1a
    let uhu = 0x12345678u
    let uhl = 0x123456789abcdefl
    let uhlu = 0x123456789abcdefl
    assert("{typename(i)}"=="int&")
    assert("{typename(li)}"=="int64&")
    assert("{typename(u)}"=="uint&")
    assert("{typename(ul)}"=="uint64&")
    assert("{typename(uh)}"=="uint&")
    assert("{typename(uhu)}"=="uint&")
    assert("{typename(uhl)}"=="uint64&")
    assert("{typename(uhlu)}"=="uint64&")
    return true
