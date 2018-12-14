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

def test:bool
    let(a,b:int=0)
        twice({ a++; })
        assert(a==2)
        b = twice_good({ a++; })
        assert(a==4 & b==(2+3))
    let(x:int=0)
        func1({
            x++;
            func2({
                x++;
            });
            func4({
                x++;
            });
        })
        assert(x==3)
    return true

