struct Object
    position:float3
    velocity:float3

let
    objects:Object[10000]

def ks_update(pos:float3 &;vel:float3)
    pos += vel

def update(a:Object)
    a.position+=a.velocity

def test
    let ( index:int = 0 )
        while index < 10000
            update(objects[index++])

def foreachTest
    let ( obj:Object )
        foreach obj in objects
            update(obj)

def foreachIteropTest
    let ( obj:Object )
        foreach obj in objects
            interopUpdate(obj)

def interopTest
    let ( index:int = 0 )
        while index < 10000
            interopUpdate(objects[index++])

def initObj(index:int;a:Object)
    let ( oi:float=float(index); ii:float=oi*2.0 )
        a.position=float3(oi+0.1,oi+0.2,oi+0.3)
        a.velocity=float3(ii+1.0,ii+2.0,ii+3.0)

def init
    let ( index:int = 0 )
        while index < 10000
            initObj(index,objects[index++])
