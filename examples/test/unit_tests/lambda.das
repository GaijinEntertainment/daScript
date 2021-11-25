// options log = true

var
    network : array<uint8>

struct Message
    cnt : lambda<(arg:int):int>

def send(var t:lambda<(arg:int):int>)
    var msg <- [[Message cnt <- t]]
    binary_save(msg) <| $(data)
        network := data

def recv
    var t : Message
    binary_load(t, network)
    return <- t.cnt

[sideeffects]
def onlyNull ( bar : lambda<(a:int):int> )
    if bar != null
        assert(false,"has to be null")

def addX(a:int)
    return @ <| (b:int)
        return a + b

[export]
def test
    var CNT = 0
    let counter <- @ <| (extra:int) : int
        return CNT++ + extra
    var x = invoke(counter,13)
    CNT = 100500
    let y = invoke(counter,13)
    assert(x==13 && y==14)
    send(counter)
    let net_counter <- recv()
    let z = invoke(net_counter,13)
    assert(z==15)
    x = invoke(counter,13)
    assert(x==15)
    let l : lambda<(a:int):int>
    onlyNull(l)
    let t <- addX(1)
    let q = invoke(t,2)
    assert(q==3)
    return true

[export]
def main
    test()

