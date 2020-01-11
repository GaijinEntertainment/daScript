options log = true

[export]
def test
    var count = 0
    let gen = generator() <| $(var a:int &) : bool
        yield count++
        yield count++
        yield count++
        return false
    for x in gen
        debug(x, "x=")
    debug(gen, "gen=")
    return true
