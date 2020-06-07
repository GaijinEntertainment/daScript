options log = true

[export]
def test

    var x = %_esc \{\}\w\x\y\\n%
    print(x)
    return true
