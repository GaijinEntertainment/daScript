options optimize=false

[export]
def test
    var i = 100500
    var li = 100500100500100500l
    var u = 14u
    var ul = 100500100500100500ul
    var uh = 0x1a
    var uhu = 0x12345678u
    var uhl = 0x123456789abcdefl
    var uhlu = 0x123456789abcdefl
    assert("{typeinfo(typename i)}"=="int")
    assert("{typeinfo(typename li)}"=="int64")
    assert("{typeinfo(typename u)}"=="uint")
    assert("{typeinfo(typename ul)}"=="uint64")
    assert("{typeinfo(typename uh)}"=="uint")
    assert("{typeinfo(typename uhu)}"=="uint")
    assert("{typeinfo(typename uhl)}"=="uint64")
    assert("{typeinfo(typename uhlu)}"=="uint64")
    let ch = 'a'
    assert(ch==97)
    return true
