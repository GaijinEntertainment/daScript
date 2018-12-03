struct Object
    position:float[3]
    velocity:float[3]

let
    objects:Object[10000]

def update(a:Object)
    a.position[0]+=a.velocity[0]
    a.position[1]+=a.velocity[1]
    a.position[2]+=a.velocity[2]

def ks_update(pos:float3 &;vel:float3)
    pos += vel

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
        a.position[0]=oi+0.1
        a.position[1]=oi+0.2
        a.position[2]=oi+0.3
        a.velocity[0]=ii+1.0
        a.velocity[1]=ii+2.0
        a.velocity[2]=ii+3.0

def init
    let ( index:int = 0 )
        while index < 10000
            initObj(index,objects[index++])
