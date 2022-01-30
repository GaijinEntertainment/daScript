options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

module random shared

require math

let LCG_RAND_MAX = 32767
let LCG_RAND_MAX_BIG = 32768*32768-1
let LCG_IRAND_MAX_FLT = 1.0f / float(LCG_RAND_MAX)

def random_seed ( seed:int )
    return int4(seed,seed+1,seed+2,seed+3)

def random_seed2D(var seed:int4&; co:int2; cf:int=0)
    let prnd = sin(dot(float2(co.xy),float2(12.9898,78.233))) * 43758.5453
    seed = random_seed(int(prnd)+cf)
    return random_float4(seed)

def random_int ( var seed:int4& )
    seed.x = 214013*seed.x+2531011
    return int((seed.x>>16) & LCG_RAND_MAX)

def random_big_int ( var seed:int4& )
    seed = int4(214013)*seed+int4(2531011)
    let i12 = (seed.xy>>16) & int2(LCG_RAND_MAX)
    return i12.x + i12.y*32768

def random_int4 ( var seed:int4& )
    seed = int4(214013)*seed+int4(2531011)
    return (seed>>16) & int4(LCG_RAND_MAX)

def random_float ( var seed:int4& )
    return float(random_int(seed)) * LCG_IRAND_MAX_FLT

def random_float4 ( var seed:int4& )
    return float4(random_int4(seed)) * float4(LCG_IRAND_MAX_FLT)

def random_unit_vector ( var seed:int4& )
    let zz = random_float4(seed)
    let z = zz.x * 2. - 1.
    let a = zz.y * ( 2. * PI )
    let r = sqrt(1. - z*z)
    var x, y : float
    sincos(a, x, y)
    return float3( r * x, r * y, z )

def random_in_unit_sphere ( var seed:int4& )
    while true
        let r = random_float4(seed).xyz
        let p = float3(2.)*r - float3(1.)
        if length_sq(p) <= 1.0
            return p
    return float3(0.)

def random_in_unit_disk ( var seed:int4& )
    while true
        let R = random_float4(seed).xy
        let r = float3(R.x,R.y,0.)
        let p = float3(2.)*r - float3(1.,1.,0.)
        if length_sq(p) <= 1.0
            return p
    return float3(0.)
