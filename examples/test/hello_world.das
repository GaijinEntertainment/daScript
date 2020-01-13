options log = true

[export]
def test
    let gen = generator() <| $(var a:int &) : bool
        var count = 0
        yield count++
        yield count++
        yield count++
        return false
    for x in gen
        debug(x, "x=")
    debug(gen, "gen=")
    return true
