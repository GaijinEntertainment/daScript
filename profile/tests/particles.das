require testProfile

struct NObject
    position, velocity : float3

def update(a:NObject)
    a.position+=a.velocity

def testSim(objects:array<NObject>)
    for obj in objects
        update(obj)

def testSim2(objects:array<NObject>; count:int)
    for i in range(0,count)
        testSim(objects)

def testSimI(objects:array<NObject>)
    for obj in objects
        obj.position += obj.velocity

def testSim2I(objects:array<NObject>; count:int)
    for i in range(0,count)
        testSimI(objects)

def init(objects:array<NObject>)
    resize(objects, 1000000)
    let index:int = 0
    for obj in objects
        let ( oi:float=float(index++); ii:float=oi*2.0 )
            obj.position=float3(oi+0.1,oi+0.2,oi+0.3)
            obj.velocity=float3(1.0,2.0,3.0)
    assert(index==length(objects))

def test():bool
    let objects:array<NObject>
    init(objects)
    let(total:int=2)
        profile(total,"array of nobjects") <|
            testSim2(objects,100)
        profile(total,"array of nobjects, inlined") <|
            testSim2I(objects,100)
    return true
