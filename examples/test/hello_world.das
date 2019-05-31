// options logInferPasses = true
options logCpp=true

/*
require UnitTest

struct CoreEvent
    eventName:string
    eventType:uint

[export, unsafe]
def zone_captured(evt:TestObjectFoo)
    print("{(upcast<CoreEvent const&> evt).eventType}\n")
*/

struct Int2Float
    iv:uint
    fv:float

struct Float2Int
    fv:float
    iv:uint

[export,unsafe]
def test:bool
    let f2i = [[Int2Float iv=0x3f400000,fv=1.0]]
    let i2f = reinterpret<Float2Int>(f2i)
    print("f2i={f2i} i2f={i2f}\n")
    assert(i2f.fv==0.75 & i2f.iv==0x3f800000)
    return true

/*
[export,unsafe]
def test:bool
    let t : tuple < int; float; string >
    debug(t,"\nt=")
    print("t={t}\n")
    print("sizeof t = {typeinfo(sizeof type tuple <int;float;string>)}\n")
    let q = t
    print("q={q}\n")
    return true
*/

