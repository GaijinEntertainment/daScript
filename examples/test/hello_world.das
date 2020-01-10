options log = true


struct Foo
    count : int

[unsafe,export]
def test
    var count = 0
    let gen = generator() <| $(var a:int &) : bool
        if count < 10
            a = count++
            return true
        else
            return false
    for x in gen
        debug(x, "x=")
    debug(gen, "gen=")

    var foo : Foo
    count = 0
    let genf = generator() <| $ ( var a : Foo ) : bool
        if count < 10
            foo.count = count++
            a = foo
            return true
        else
            return false
    for x in genf
        debug ( x, "x=" )
    return true
