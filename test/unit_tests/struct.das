struct Ball
    position:float3
    radius:float

def set_default_ball(b:Ball)
    b.position=float3(1.0,2.0,3.0)
    b.radius=5.0

def make_default_ball:Ball?
    let(b:Ball?=new Ball)
        set_default_ball(b->)
        return b

def verify_default_ball(b:Ball)
    assert(b.position==float3(1.0,2.0,3.0))
    assert(b.radius==5.0)

def test_ref_ptr(a:Ball?&;b:Ball?)
    a = b
    assert(a==b)

def test_ret_ptr(a:Ball?):Ball?
    return a

struct L3
    a:int

struct L2
    l3:L3?

struct L1
    l2:L2?

let
    glob : L1? = null

def test:bool
    let ( b:Ball? = new Ball )
        b.position = float3(1.0,2.0,3.0)
        b.radius = 5.0
        verify_default_ball(b->)
        let ( xb, qb:Ball; qc:Ball? = b; qd:Ball? = new Ball )
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
    let ( b:Ball? = make_default_ball() )
        verify_default_ball(b->)
    let(dummy:int)
        glob?.l2?.l3?.a ?? dummy = 1
        assert(dummy==1)
        glob = new L1
        glob?.l2?.l3?.a ?? dummy = 2
        assert(dummy==2)
        glob.l2 = new L2
        glob?.l2?.l3?.a ?? dummy = 3
        assert(dummy==3)
        glob.l2.l3 = new L3
        glob?.l2?.l3?.a ?? dummy = 4
        assert(dummy==3)
        assert(glob?.l2?.l3?.a ?? 5==4)
    return true

