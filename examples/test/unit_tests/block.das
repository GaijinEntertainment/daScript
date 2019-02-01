// options plot=true,plotFile="../../test.dot",plotReadWrite=true
// options log=true, logOptimizationPasses=false

def twice(b:block)
    invoke(b)
    invoke(b)

def twice_good(b:block<int>):int
    let(t:int)
        t += invoke(b)
        t += invoke(b)
        return t

def crapOnStack
    let(a:int[20])
        for b in a
            b = 13

def func1(b:block)
    crapOnStack()
    invoke(b)
    crapOnStack()

def func2(b:block)
    crapOnStack()
    func3(b)
    crapOnStack()

def func3(b:block)
    crapOnStack()
    invoke(b)
    crapOnStack()

def func4(b:block)
    crapOnStack()
    invoke(b)
    crapOnStack()

def badd(ext:int;b:block<(arg:int):int>) : int
    return invoke(b,ext)

def radd(ext:int&;b:block<(arg:int&):int>):int
    return invoke(b,ext)

[export]
def test:bool
    let(a,b:int=0)
        twice({ a++; })
        assert(a==2)
        b = twice_good() <| { return a++; }
        assert(a==4 & b==(2+3))
        b = twice_good() <|
            return a++
        assert(b==(4+5))
        b = twice_good({ return ++a; })
        assert(a==8)
        assert(b==(7+8))
    let(x,y:int=0)
        func1() <|
            x += 1
            y += 2
            func2() <| // here
                x++
                y += 3
            func4() <|
                x++
                y += 4
        assert(x==3 & y==9)
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

