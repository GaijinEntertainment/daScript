require testProfile

def ks_update(pos:float3 &;vel:float3)
    pos += vel

def update(a:Object)
    a.position+=a.velocity

def testSim(objects:ObjectArray)
    for obj in objects
        update(obj)

def testInterop(objects:ObjectArray)
    for obj in objects
        interopUpdate(obj)

def initObj(index:int;a:Object)
    let ( oi:float=float(index); ii:float=oi*2.0 )
        a.position=float3(oi+0.1,oi+0.2,oi+0.3)
        a.velocity=float3(ii+1.0,ii+2.0,ii+3.0)

def init(objects:ObjectArray)
    let index:int = 0
    for obj in objects
        initObj(index++,obj)
    assert(index==objects.length)

def verify(total:int;objects:ObjectArray)
    // print("\n")
    let index:int = 0
    let t:float=float(total)
    let tt:float3 = float3(t,t,t)
    for obj in objects
        let ( oi:float=float(index); ii:float=oi*2.0 )
            let apos:float3=float3(oi+0.1,oi+0.2,oi+0.3)
            let avel:float3=float3(ii+1.0,ii+2.0,ii+3.0)
            let npos:float3 = apos + avel*tt
            // debug(npos,"npos=")
            // debug(obj.position,"obj.position=")
            assert(obj.position==npos)
            index ++

def test(objects:ObjectArray):bool
    let(total:int=100;simT,cT,intT,manyT,manyKsT:float)
        init(objects)
        simT = profile(total,"basic version") <|
            testSim(objects)
        verify(total,objects)
        init(objects)
        cT = profile(total,"c++ version") <|
            interopUpdateTest(objects)
        verify(total,objects)
        init(objects)
        intT = profile(total,"interop version") <|
            testInterop(objects)
        verify(total,objects)
        init(objects)
        manyT = profile(total,"interop 10000 version") <|
            update10000(objects)
        verify(total,objects)
        init(objects)
        manyKsT = profile(total,"interop 10000-ks version") <|
            update10000ks(objects)
        verify(total,objects)
        print("ratio sim/c++: "+string(simT/cT)+"\n");
        print("ratio interop/c++: "+string(intT/cT)+"\n");
        print("ratio interop-10000/c++: "+string(manyT/cT)+"\n");
        print("ratio interop-10000-ks/c++: "+string(manyKsT/cT)+"\n");
    return true
