struct Object
    position:float3
    velocity:float3

let
    objects:Object[10000]

def ks_update(pos:float3 &;vel:float3)
    pos += vel

def update(a:Object)
    a.position+=a.velocity

def testSim
    for obj in objects
        update(obj)

def testInterop
    for obj in objects
        interopUpdate(obj)

def initObj(index:int;a:Object)
    let ( oi:float=float(index); ii:float=oi*2.0 )
        a.position=float3(oi+0.1,oi+0.2,oi+0.3)
        a.velocity=float3(ii+1.0,ii+2.0,ii+3.0)

def init
    let ( index:int = 0 )
        for obj in objects
            initObj(index++,obj)

def test:bool
    let(total:int=100;simT,cT,intT,manyT,manyKsT:float)
        init()
        simT = profile(total,"basic version") <|
            testSim()
        init()
        cT = profile(total,"c++ version") <|
            interopUpdateTest(objects)
        init()
        intT = profile(total,"interop version") <|
            testInterop()
        init()
        manyT = profile(total,"interop 10000 version") <|
            update10000(objects)
        manyKsT = profile(total,"interop 10000-ks version") <|
            update10000ks(objects)
        print("ratio sim/c++: "+string(simT/cT)+"\n");
        print("ratio interop/c++: "+string(intT/cT)+"\n");
        print("ratio interop-10000/c++: "+string(manyT/cT)+"\n");
        print("ratio interop-10000-ks/c++: "+string(manyKsT/cT)+"\n");
    return true
