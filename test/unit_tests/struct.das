struct Ball
    position:float3
    radius:float

def verify_default_ball(b:Ball)
    assert(b.position==float3(1.0,2.0,3.0))
    assert(b.radius==5.0)

def test_ref_ptr(a:Ball*&;b:Ball*)
    a = b
    assert(a==b)

def test_ret_ptr(a:Ball*):Ball*
    return a

def test:bool
    let ( b:Ball* = new Ball )
        b.position = float3(1.0,2.0,3.0)
        b.radius = 5.0
        verify_default_ball(b->)
        let ( xb, qb:Ball; qc:Ball* = b; qd:Ball* = new Ball )
            qb = qc->
            verify_default_ball(qb)
            verify_default_ball(qc->)
            qc = null
            assert(qc==null)
            assert(qd!=null)
            test_ref_ptr(qc,qd)
            assert(qc==qd)
            qd = test_ret_ptr(null)
            assert(qd==null)
            assert(qd!=qc)
            xb = qb
            verify_default_ball(xb)
    return true

