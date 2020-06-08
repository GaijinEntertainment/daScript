options log = true

require array_reader

[export]
def test
    var x = %arr \{\}\w\x\y\\n%%
    print("x = {x}\n")
    return true
