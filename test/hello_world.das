def badd(ext:int;b:block<(arg:int):int>) : int
    return invoke(b,ext)

def radd(ext:int&;b:block<(arg:int&):int>):int
    return invoke(b,ext)

def test:bool
    let(res,v1:int)
        v1 = 1
        res = badd(1000,$(a1:int):int{ return v1++ + a1; })
        assert(res==1001)
        res = badd(1000) <| $(a1:int):int{ return v1 + a1; }
        assert(res==1002)
        v1 = 3
        res = badd(1000) <| $(a1:int):int
            return v1 + a1
        assert(res==1003)
        v1 = 4
        res = badd(1000) <| $(a2:int):int
            return badd(2000,$(a3:int):int { return a3 + a2 + v1; })
        assert(res==3004)
        v1 = 3
        res = radd(v1) <| $(a:int&):int
            return a++
        assert(res==3 & v1==4)
    return true

