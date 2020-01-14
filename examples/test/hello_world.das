options log = true
options log_infer_passes = true

[export]
def test
    let gen = generator() <| $(var a:int &) : bool
        var count = 0
        var rcount : int & = count
        yield rcount++
        yield rcount++
        yield rcount++
        return false
    for x in gen
        debug(x, "x=")
    debug(gen, "gen=")
    return true
