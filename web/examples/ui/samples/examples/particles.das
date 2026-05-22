// Particles kinematics benchmark — ported from dasProfile/tests/particles.das.
// 50,000 objects × 100 sim steps × 10 samples.
options gen2

struct NObject {
    position : float3
    velocity : float3
}

[hint(noalias=objects, vec3_ldu)]
def testSimI(var objects : array<NObject>) {
    for (obj in objects) {
        obj.position += obj.velocity
    }
}

[hint(noalias=objects, vec3_ldu)]
def testSim2I(var objects : array<NObject>; count : int) {
    for (_i in range(count)) {
        testSimI(objects)
    }
}

def init(var objects : array<NObject>) {
    resize(objects, 50000)
    var i = 0
    for (obj in objects) {
        obj.position = float3(i++, i + 1, i + 2)
        obj.velocity = float3(1.0, 2.0, 3.0)
    }
    assert(i == length(objects))
}

[export]
def main {
    var objects : array<NObject>
    init(objects)
    profile(10, "particles kinematics") {
        testSim2I(objects, 100)
    }
}
