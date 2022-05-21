require daslib/soa

[soa]
struct Particle
    pos : float3
    color : float4

[export]
def main
    var soa : Particle`SOA
    let t = soa[11].pos     // macro replaces this with soa.pos[11]
    let len = length(soa)
    print("len={len}, t = {t}\n")

options log_infer_passes