require math

let
    R_PI : float const = 3.14159265358979323846
    LCG_RAND_MAX : int const = 32767

def randomSeed(seed:int)
    return int4(seed,seed+1,seed+2,seed+3)

def randomInt(seed:int4&)
    seed.x = 214013*seed.x+2531011
    return (seed.x>>16) & LCG_RAND_MAX

def randomInt4(seed:int4&)
    seed = int4(214013)*seed+int4(2531011)
    return (seed >> 16) & int4(LCG_RAND_MAX)

def randomFloat(seed:int4&)
    return float(randomInt(seed)) * (1.0/float(LCG_RAND_MAX))

def randomFloat4(seed:int4&)
    return float4(randomInt4(seed)) * float4(1.0/float(LCG_RAND_MAX))

def randomUnitVector(state:int4&)
    let zz = randomFloat4(state).xy
    let z = zz.x * 2. - 1.
    let a = zz.y * ( 2. * R_PI )
    let r = sqrt(1. - z * z)
    let x, y : float
    sincos(a, x, y)
    return float3(r * x, r * y, z)

def randomInUnitSphere(state:int4&)
    while true
        let p = 2.0 * randomFloat4(state).xyz - float3(1)
        if lengthSq(p) <= 1.0
            return p

def randomInUnitDisk(state:int4&)
    while true
        let p2 = 2.0 * randomFloat4(state).xy - float2(1)
        if lengthSq(p2) <= 1.0
            return float3(p2.x,p2.y,0.)

