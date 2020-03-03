options log = true

/*
def filter ( var arr : array<auto(TT)>; var blk : lambda<(arg:TT):bool> )
    return <- generator<TT>() <| $()
        for x in arr
            if invoke(blk,x)
                yield x
        return false
    finally
        print("hello\n")

[export]
def test
    var arr <- [[for x in range(0,10); x]]
    var farr <- to_array ( filter(arr, @(t) => (t%2)==0 )  )
    print("farr = {farr}\n")
	return true
*/

[export]
def test
    var arr <- [{int 1;2;3}]
    var t = 2
    var lmb <- generator<int>() <| $ ( )
        for x in arr
            if x == t
                yield t
        return false
    finally
        delete arr
    print("lmb = {lmb}\n")
    return true

