// options log=true, logStack=true, logNodes=true
// options logInferPasses=true
// options printVarAccess=true

def toHex(hash:uint[8])
    let st = build_string() <| $(var writer)
        for i in hash
            format(writer,"%08x",i)
    return st

[export]
def test
    let hash:uint[8]
    toHex(hash)
    return true

