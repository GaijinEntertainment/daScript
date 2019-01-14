options log=true, logStack=true, optimize=false

def dummy(a)
    return true

[export]
def test
    dummy(float4x4())
    return true
