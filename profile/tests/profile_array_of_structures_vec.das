require testProfile

struct NObject
    position, velocity : float3

let
    nobjects:NObject[10000]

def update(a:NObject)
    a.position+=a.velocity

def testSim(objects:NObject[10000])
    for obj in objects
        update(obj)

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
    let ( oi = float(index); ii=oi*2.0 )
        a.position=float3(oi+0.1,oi+0.2,oi+0.3)
        a.velocity=float3(1.0,2.0,3.0)

def initObj(index:int;a:NObject)
    let ( oi=float(index); ii=oi*2.0 )
        a.position=float3(oi+0.1,oi+0.2,oi+0.3)
        a.velocity=float3(1.0,2.0,3.0)

def init(objects:ObjectArray)
    let index = 0
    for obj in objects
        initObj(index++,obj)
    assert(index==objects.length)

def init(objects:NObject[10000])
    let index = 0
    for obj in objects
        initObj(index++,obj)
    assert(index==10000)

def verify(total:int;objects:ObjectArray)
    // print("\n")
    let index = 0
    let t = float(total)
    for obj in objects
        let ( oi=float(index); ii=oi*2.0 )
            let apos = float3(oi+0.1,oi+0.2,oi+0.3)
            let avel = float3(1.0,2.0,3.0)
            let npos = apos + avel*t
            // debug(npos,"npos=")
            // debug(obj.position,"obj.position=")
            assert(obj.position==npos)
            index ++

def verify(total:int;objects:NObject[10000])
    // print("\n")
    let index = 0
    let t = float(total)
    for obj in objects
        let ( oi=float(index); ii=oi*2.0 )
            let apos = float3(oi+0.1,oi+0.2,oi+0.3)
            let avel = float3(1.0,2.0,3.0)
            let npos = apos + avel*t
            // debug(npos,"npos=")
            // debug(obj.position,"obj.position=")
            assert(obj.position==npos)
            index ++

def test(objects:ObjectArray):bool
    let(simTN,simT,cT,intT,manyT,manyKsT:float)
        // verify
        init(nobjects)
        testSim(nobjects)
        verify(1,nobjects)
        init(objects)
        testSim(objects)
        verify(1,objects)
        init(objects)
        interopUpdateTest(objects)
        verify(1,objects)
        init(objects)
        testInterop(objects)
        verify(1,objects)
        init(objects)
        update10000(objects)
        verify(1,objects)
        init(objects)
        update10000ks(objects)
        verify(1,objects)
        // profile
        simTN = profile(20,"native basic version") <|
            for i in range(0,1000)
                testSim(nobjects)
        simT = profile(20,"basic version") <|
            for i in range(0,1000)
                testSim(objects)
        cT = profile(20,"c++ version") <|
            for i in range(0,1000)
                interopUpdateTest(objects)
        intT = profile(20,"interop version") <|
            for i in range(0,1000)
                testInterop(objects)
        manyT = profile(20,"interop 10000 version") <|
            for i in range(0,1000)
                update10000(objects)
        manyKsT = profile(20,"interop 10000-ks version") <|
            for i in range(0,1000)
                update10000ks(objects)
        print("ratio nsim/c++ {simTN/cT}\n")
        print("ratio sim/c++: {simT/cT}\n")
        print("ratio interop/c++: {intT/cT}\n")
        print("ratio interop-10000/c++: {manyT/cT}\n");
        print("ratio interop-10000-ks/c++: {manyKsT/cT}\n")
    return true
