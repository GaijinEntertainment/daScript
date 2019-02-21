options log = true, logNodes =true, printRef = true, printVarAccess = true // , logOptimizationPasses = true

struct Color
    r, g, b, a : int

struct Particle
    pos : float3
    vel : float3
    col : Color

[sideeffects]
def update(p:Particle)
    p.vel.y = 13.
    p.col.a = 1

[export]
def test
    let p : Particle
    update(p)
    assert(p.col.a==1 & p.vel.y==13.)
    print("p={p}\n")
    return true
