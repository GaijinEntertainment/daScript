options logCpp=true, logNodes=false

def badd(ext:int;b:block<(arg:int):int>) : int
    return invoke(b,ext)


[export]
def test:bool
    let v1 = 3
    let res = badd(1000) <| $(a1:int):int
        return v1 + a1
    assert(res==1003)
    return true

