options log=true

[export]
def counter()
    var x = 0
    return lambda() <| $ ()
        return ++x

[export]
def test
    let c = counter()
    let i0 = invoke(c)
    let i1 = invoke(c)
    let i2 = invoke(c)
    let i3 = invoke(c)
    print("{i0} {i1} {i2} {i3}\n")
    print("counter is {c}\n")
    debug(c)
	return true
