expect 10008

[export]
def test()
    var a = "ab\\xy"
    assert(a=="abxy")   // 10008: error, invalid escape sequence
    return true
