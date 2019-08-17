// options log = true, logNodes =true, printRef = true, printVarAccess = true // , logOptimizationPasses = true
// options logCpp = true
// options logNodes=true

require testProfile

struct NObject
    position : float3
    velocity : float3

def update(var a:NObject)
    a.position+=a.velocity

def testSim(var objects:array<NObject>)
    for obj in objects
        update(obj)

def testSim2(var objects:array<NObject>; count:int)
    for i in range(0,count)
        testSim(objects)

def testSimI(var objects:array<NObject>)
    for obj in objects
        obj.position += obj.velocity

def testSim2I(var objects:array<NObject>; count:int)
    for i in range(0,count)
        testSimI(objects)

def init(var objects:array<NObject>)
    resize(objects, 50000)
    var i = 0
    for obj in objects
        obj.position=float3(i++,i+1,i+2)
        obj.velocity=float3(1.0,2.0,3.0)
    assert(i==length(objects))

[export]
def test()
    var objects:array<NObject>
    init(objects)
    let total=20
    profile(total,"particles kinematics") <|
        testSim2(objects,100)
    profile(total,"particles kinematics, inlined") <|
        testSim2I(objects,100)
    profile(total,"particles kinematics, C++") <|
        testProfile::testParticles(100)
    profile(total,"particles kinematics, inlined C++") <|
        testProfile::testParticlesI(100)
    return true
