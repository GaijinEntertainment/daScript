expect 30503:4    // cant_get_field
expect 30504      // cant_write_to_const

struct L3
    a:int

struct L2
    l3:L3?

struct L1
    l2:L2?

def test_safe_navigation
    let l:L1?
    let cl:L1? const
    let dummy:int
    l?.l2?.l3?.a ?? dummy = 0
    l.l2.l3.a = 0
    cl?.l2?.l3?.a ?? dummy = 0  // can't write to constant value(30504)

def test
    let a:L3
    let pa:L3?
    let b:L3[10]
    let c:int
    let pca:L3? const
    a.a = 0
    b.a = 0         // can't get field of array                 (30503)
    pa.a = 0
    c.a = 0         // can't get field of some random type      (30503)
    a?.a = 0        // can't safe get field of non-pointer      (30503)
    deref(pa?.a) = 0
    c?.a = 0        // can't get field of some random type      (30503)



