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

def testSimI(objects:NObject[10000])
    for obj in objects
        obj.position += obj.velocity

[export]
def ks_update(pos:float3 &;vel:float3)
    pos += vel

[export]
def update(a:Object)
    a.position+=a.velocity

def testSim(objects:ObjectArray)
    for obj in objects
        update(obj)

def testSimI(objects:ObjectArray)
    for obj in objects
        obj.position += obj.velocity

def testInterop(objects:ObjectArray)
    for obj in objects
        interopUpdate(obj)

def initObj(i:int;a:Object)
    a.position=float3(i++,i+1,i+2)
    a.velocity=float3(1.0,2.0,3.0)

def initObj(i:int;a:NObject)
    a.position=float3(i++,i+1,i+2)
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
    let i = 0
    let t = float(total)
    for obj in objects
        let apos = float3(i++,i+1,i+2)
        let avel = float3(1.0,2.0,3.0)
        let npos = apos + avel*t
        assert(obj.position==npos)

def verify(total:int;objects:NObject[10000])
    let i = 0
    let t = float(total)
    for obj in objects
        let apos = float3(i++,i+1,i+2)
        let avel = float3(1.0,2.0,3.0)
        let npos = apos + avel*t
        assert(obj.position==npos)

[export]
def test(objects:ObjectArray)
    // verify
    init(nobjects)
    testSim(nobjects)
    verify(1,nobjects)
    init(nobjects)
    testSimI(nobjects)
    verify(1,nobjects)
    init(objects)
    testSim(objects)
    verify(1,objects)
    init(objects)
    testSimI(objects)
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
    let simTN = profile(20,"native basic version") <|
        for i in range(0,1000)
            testSim(nobjects)
    let simTN_I = profile(20,"native basic version, inline") <|
        for i in range(0,1000)
            testSimI(nobjects)
    let simT = profile(20,"basic version") <|
        for i in range(0,1000)
            testSim(objects)
    let simT_I = profile(20,"basic version, inline") <|
        for i in range(0,1000)
            testSimI(objects)
    let cT = profile(20,"c++ version") <|
        for i in range(0,1000)
            interopUpdateTest(objects)
    let intT = profile(20,"interop version") <|
        for i in range(0,1000)
            testInterop(objects)
    let manyT = profile(20,"interop 10000 version") <|
        for i in range(0,1000)
            update10000(objects)
    let manyKsT = profile(20,"interop 10000-ks version") <|
        for i in range(0,1000)
            update10000ks(objects)
    print("ratio nsim/c++ {simTN/cT}\n")
    print("ratio sim/c++: {simT/cT}\n")
    print("ratio nsim,inline/c++ {simTN_I/cT}\n")
    print("ratio sim,inline/c++: {simT_I/cT}\n")
    print("ratio interop/c++: {intT/cT}\n")
    print("ratio interop-10000/c++: {manyT/cT}\n");
    print("ratio interop-10000-ks/c++: {manyKsT/cT}\n")
    return true
