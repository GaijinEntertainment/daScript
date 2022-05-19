require daslib/soa

struct Particle
    pos : float3
    color : float4

// this is what annotation generates
struct Particle_SOA
    pos : array<float3>
    color : array<float4>

// this is what annotation also generates
def operator [] ( var src : Particle_SOA; index:int )
    return [[SOA_INDEX]]    // note SOA_INDEX

[export]
def main
    var soa : Particle_SOA
    let t = soa[11].pos     // macro replaces this with soa.pos[11]
    print("t = {t}\n")
