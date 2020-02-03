
options log = true
// options log_infer_passes = true
// options print_c_style = true

struct Foo
    a : int

/*
def test_for_pod
    let gen = generator<int>() <| $()
        for i in range(0,10)
            yield i
        return false
    for x,y in gen,range(0,10)
        assert(x==y)
*/

def test_for_ref
    var arr <- to_array(each(range(0,10)))
    let gen = generator<int&>() <| $()
        for i in arr
            yield i
        delete arr
        return false
    for x,y in gen,range(0,10)
        assert(x==y)

[export]
def test
    // test_for_pod()
    test_for_ref()
    return true

/*
    var ttt : Foo[10]
    var qq = 0
    for i in ttt
        i = [[Foo a=qq++]]
    var gen = generator<Foo&>() <| $() : bool
        for i in ttt
            yield i
        return false
    print("gen = {gen}\n")
    for x in gen
        debug(x,"x=")
        x.a = 1
    print("gen = {gen}\n")
    return true
*/
