options log = true
// options log_infer_passes = true
// options print_c_style = true

[export]
def test
    let gen = generator() <| $(var a:int &) : bool
        var count = 0
        var rcount : int & = count
        yield rcount++
        yield rcount++
        if rcount > 0
            var zount = count++
            yield zount
        if count < 0
            var zount = rcount++
            yield zount
        else
            yield -1
        if count > 0
            if count > 1
                yield count
        return false
    print("gen = {gen}\n")
    for x in gen
        print("x = {x}\n")
    print("gen = {gen}\n")
    return true
