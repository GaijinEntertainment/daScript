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
    assert(fabs(log(exp(1.)) - 1.) < 1e-6)
    return true
