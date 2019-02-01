require math

options optimize=false

[export]
def test
    assert(uint32_hash(1U) != 1U)
    let seed:uint = 1U
    let position:int = 1
    assert(uint_noise1D(position, seed) != seed)
    assert(uint_noise2D(int2(100,100), seed) != seed)
    assert(uint_noise3D(int3(100,100,100), seed) != seed)
    assert(dot(float2(0,1), float2(1,0))==0.)
    assert(dot(float3(0,1,0), float3(1,0,1))==0.)
    assert(dot(float4(0,1,0,0), float4(1,0,1,1))==0.)

    assert(length(float2(0,1))==1.)
    assert(length(float3(0,1,0))==1.)
    assert(length(float4(0,0,0,1))==1.)
    assert(lengthSq(float2(0,1))==1.)
    assert(lengthSq(float3(1,0,0))==1.)
    assert(lengthSq(float4(0,1,0,0))==1.)
    assert(cross(float3(0,1,0), float3(0,0,1))==float3(1,0,0))

    assert(sin(0.)==0.)
    assert(cos(0.)==1.)
    assert(tan(0.)==0.)
    assert(asin(0.)==0.)
    assert(acos(1.)==0.)
    assert(abs(asin(sin(0.5)) - 0.5) < 0.0001)
    assert(abs(acos(cos(0.5)) - 0.5) < 0.0001)
    assert(sin(0.1)*sin(0.1) + cos(0.1)*cos(0.1) == 1.)//cos^2 + sin^2 == 1

    assert(atan2(0.,0.)==0.)
    assert(sqrt(4.)==2.)
    assert(pow(2.,2.)==4.)
    assert(ceil(1.1)==2.)
    assert(floor(1.1)==1.)
    assert(ceil(-1.1)==-1.)
    assert(floor(-1.1)==-2.)
    assert(exp(0.)==1.)
    assert(abs(log(exp(1.)) - 1.) < 1e-6)
    assert(pow(0., 0.)==1.)
    assert(pow(2., 2.)==4.)
    assert(exp2(2.)==4.)
    assert(log2(4.)==2.)
    assert(abs(exp(1.) - 2.718281828459045f) < 1e-6)
    assert(abs(log(2.718281828459045f) - 1.) < 1e-6)
    assert(ceili(float2(0.5,-0.1)) == int2(1,0))
    assert(floori(float2(0.5,-0.1)) == int2(0,-1))
    assert(roundi(float2(0.501,-0.1)) == int2(1,0))
    assert(trunci(float2(0.501,-0.1)) == int2(0,0))
    return true
