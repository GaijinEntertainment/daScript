require math

options optimize=false

[export]
def test
    assert(sin(0.)==0.)
    assert(cos(0.)==1.)
    assert(tan(0.)==0.)
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
