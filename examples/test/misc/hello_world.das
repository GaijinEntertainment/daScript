require daslib/random

var seed : int4

let FULL_TEST = true

struct Foo
    a : int
    b : float4

def set ( a : Foo )
    var b = [[Foo a = 13]]
    b.a += a.a
    return b

def set2 ( inc : int& )
    var b = [[Foo a = 13]]
    b.a += inc
    return b

def set2_ok ( inc : int )
    var b = [[Foo a = 13]]
    b.a += inc
    return b

def set3 ( incf : float& )
    var b = [[Foo b = float4(13.) ]]
    b.b += float4(incf)
    return b

def set4 ( inc : int? )
    var b = [[Foo a = 13]]
    b.a += *inc
    return b

[export]
def main
    if FULL_TEST // straight up aliasing
        var a = [[Foo a = 1]]
        var b : Foo
        a = set(a)                  // 40211: function set result aliases argument a
        b = set(a)                  // this one ok
    if FULL_TEST // field aliasing
        var a = [[Foo a = 1]]
        var b : Foo
        a = set2(a.a)               // 40211: function set result aliases argument inc
        a = set2_ok(a.a)            // this one ok
        b = set2(a.a)               // this one ok
    if FULL_TEST // swizzle + vector subtype aliasing
        var a = [[Foo b = float4(1.)]]
        var b : Foo
        a = set3(a.b.x)             // 40211: function set result aliases argument incf
        b = set3(a.b.y)             // this one ok
    if FULL_TEST // array [] aliasing
        var a,b : Foo[2]
        a[0] = set(a[1])            // 40211: function set result aliases argument a
                                        // note - false aliasing
        b[0] = set(a[1])            // this one ok
    if FULL_TEST  // array ?[] aliasing, ?? aliasing
        var a,b : Foo[2]
        var defv : Foo
        a?[0] ?? defv = set(a[0])   // 40211: function set result aliases argument a
        a?[1] ?? defv = set(defv)   // 40211: function set result aliases argument a
        b?[0] ?? defv = set(a[0])   // this one ok
    if FULL_TEST // cast aliasing
        var a = [[Foo a = 1]]
        var b : Foo
        unsafe
            a = set2(reinterpret<int&> a.b) // 40211: function set result aliases argument inc
            a = set2(reinterpret<int&> b.b) // this one ok
    if FULL_TEST // op3 aliasing
        var a = [[Foo a = 1]]
        var b : Foo
        var cond = (random_int(seed) & 1) != 0
        cond ? a : b = set(a)       // 40211: function set result aliases argument a
        a = set ( cond ? a : b )    // 40211: function set result aliases argument a
        a = set ( cond ? b : b )    // this one ok
    if FULL_TEST // ref2ptr
        var a = [[Foo a = 1]]
        var b : Foo
        a = set4(unsafe(addr(a.a))) // 40211: function set result aliases argument a
        b = set4(unsafe(addr(a.a))) // this one ok
        a = set4(unsafe(addr(b.a))) // this one ok
    if FULL_TEST // ptr2ref, ref2value
        var a = [[Foo a = 1]]
        var pa = unsafe(addr(a))
        var paa = unsafe(addr(a.a))
        var b : Foo
        a = set2(*paa)      // 40211: function set result aliases argument a
        *pa = set2(a.a)     // 40211: function set result aliases argument a
        b = set2(*paa)      // 40211: function set result aliases argument a
                                // note - false aliasing
        set2(*paa)          // this oke ok














