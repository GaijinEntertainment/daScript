options log = true
options log_infer_passes = true
// options print_c_style = true

[export]
def test
    let gen = generator() <| $(var a:int &) : bool
        for i in range(0,10)
            yield i
        return false
    print("gen = {gen}\n")
    for x in gen
        print("x = {x}\n")
    print("gen = {gen}\n")
    return true
