require math

let
    R_PI : float const = 3.14159265358979323846
    LCG_RAND_MAX : uint const = 0x7fff

def randSeed(seed:uint)
    return uint4(seed,seed+1u,seed+2u,seed+3u)

def randUint(seed:uint4&)
    seed.x = 214013u*seed.x+2531011u
    return (seed.x>>16u) & LCG_RAND_MAX

def randUint4(seed:uint4&)
    seed = uint4(214013u)*seed+uint4(2531011u)
    return (seed >> 16) & uint4(LCG_RAND_MAX)

def randomFloat(seed:uint4&)
    return float(randUint(seed)) * (1.0/float(LCG_RAND_MAX))

def randomFloat4(seed:uint4&)
    return float4(randUint4(seed)) * float4(1.0/float(LCG_RAND_MAX))

def randomUnitVector(state:uint4&)
    let zz = randomFloat4(state).xy
    let z = zz.x * 2. - 1.
    let a = zz.y * ( 2. * R_PI )
    let r = sqrt(1. - z * z)
    let x = sin(a)
    let y = cos(a)
    return float3(r * x, r * y, z)

def randomInUnitSphere(state:uint4&)
    while true
        let p = 2.0 * randomFloat4(state).xyz - float3(1)
        if lengthSq(p) <= 1.0
            return p

def randomInUnitDisk(state:uint4&)
    while true
        let p2 = 2.0 * randomFloat4(state).xy - float2(1)
        if lengthSq(p2) <= 1.0
            return float3(p2.x,p2.y,0.)

