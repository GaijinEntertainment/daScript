options log = true, logNodes = true, logStack = true

struct SomeEvent
    pos:float3

def query(b:block<(eid:int;pos:float3&):void>)
    let f : float3
    f = float3(1)
    invoke(b,1,f)
    f = float3(2)
    invoke(b,2,f)
    f = float3(3)
    invoke(b,3,f)

def some_es2()
    let e:SomeEvent
    query() <| $ (eid:int; pos:float3&)
        e = [[SomeEvent pos=pos]]
        print("send to {eid} {e.pos}\n")
    print("{e}\n")

[export]
def test
    some_es2()
    return true
