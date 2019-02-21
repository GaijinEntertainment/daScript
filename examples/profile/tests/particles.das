// options log = true, logNodes =true, printRef = true, printVarAccess = true // , logOptimizationPasses = true

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
    resize(objects, 50000)
    let i = 0
    for obj in objects
        obj.position=float3(i++,i+1,i+2)
        obj.velocity=float3(1.0,2.0,3.0)
    assert(i==length(objects))

[export]
def test()
    let objects:array<NObject>
    init(objects)
    let(total=20)
        profile(total,"particles kinematics") <|
            testSim2(objects,100)
        profile(total,"particles kinematics, inlined") <|
            testSim2I(objects,100)
        profile(total,"particles kinematics, C++") <|
            testProfile::testParticles(100)
        profile(total,"particles kinematics, inlined C++") <|
            testProfile::testParticlesI(100)
    return true
