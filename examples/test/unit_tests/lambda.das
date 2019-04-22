// options log = true

var
    network : string

struct Message
    cnt : lambda<(arg:int):int>

def send(t:lambda<(arg:int):int>)
    var msg = [[Message cnt=t]]
    binary_save(msg) <| $(data)
        network = data

def recv
    var t : Message
    binary_load(t, network)
    return t.cnt

[export]
def test
    var CNT = 0
    let counter = lambda() <| $ (extra:int) : int
        return CNT++ + extra
    var x = invoke(counter,13)
    CNT = 100500
    let y = invoke(counter,13)
	assert(x==13 & y==14)
    send(counter)
    let net_counter = recv()
    let z = invoke(net_counter,13)
    assert(z==15)
    x = invoke(counter,13)
    assert(x==15)
    return true

