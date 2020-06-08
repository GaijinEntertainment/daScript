options log = true

[export]
def test
    var x = %_esc \{\}\w\x\y\\n%_esc
    print(x)
    return true
