expect 30503:4    // cant_get_field
expect 30504      // cant_write_to_const

struct L3
    a:int

struct L2
    l3:L3?

struct L1
    l2:L2?

def test_safe_navigation
    let(l:L1?;cl:const L1?;dummy:int)
        l?.l2?.l3?.a ?? dummy = 0   // this is fine
        l.l2.l3.a = 0               // this is fine too, but may throw
        cl?.l2?.l3?.a ?? dummy = 0  // can't write to constant value

def test
    let(a:L3;pa:L3?;b:L3[10];c:int;pca:const L3?)
        a.a = 0         // structure ok
        b.a = 0         // can't get field of array
        pa.a = 0        // pointer to structure ok
        c.a = 0         // can't get field of some random type
        a?.a = 0        // can't safe get field of non-pointer
        pa?.a-> = 0     // pointer to structure ok
        c?.a = 0        // can't get field of some random type



