expect 40211:19
options no_aliasing

require daslib/random

var seed : int4

let FULL_TEST = true

struct Foo
    a : int
    b : float4

// potential alias - cmres vs argument a
def set ( a : Foo )
    var b = [[Foo a = 13]]
    b.a += a.a
    return b

// potential alias - cmres.a vs argument inc
def set2 ( inc : int& )
    var b = [[Foo a = 13]]
    b.a += inc
    return b

// no aliasing ever
def set2_ok ( inc : int )
    var b = [[Foo a = 13]]
    b.a += inc
    return b

// potential alias - cmres.b.{xyzw} vs argument incf
def set3 ( incf : float& )
    var b = [[Foo b = float4(13.) ]]
    b.b += float4(incf)
    return b

// potential alias - cmres.a vs argument inc via *inc
def set4 ( inc : int? )
    var b = [[Foo a = 13]]
    b.a += *inc
    return b

var g_a, g_b : Foo

var g_p_a = new Foo()
var g_p_b = g_p_a

// potential alias - global variable cmres vs global variable g_a
def set5
    var b = [[Foo a = 13]]
    b.a += g_a.a
    return b

def get_a : Foo
    return g_a

// potential alias - global variable cmres vs global variable g_a via get_a function
def set6
    var b = [[Foo a = 13]]
    b.a += get_a().a
    return b

// this function ALWAYS aliases its result, becase it reads g_p_a pointer
def set7
    var b = [[Foo a = 13]]
    b.a += g_p_a?.a ?? 100500
    return b

def pass_a ( var a:Foo )
    return a

def pass_ref_a ( var a:Foo ) : Foo&
    unsafe
        return a

[export]
def main
    if FULL_TEST // straight up aliasing
        var a = [[Foo a = 1]]
        var b : Foo
        a = set(a)                  // (1) 40211: function set result aliases argument a
        b = set(a)                  // this one ok
    if FULL_TEST // field aliasing
        var a = [[Foo a = 1]]
        var b : Foo
        a = set2(a.a)               // (2) 40211: function set result aliases argument inc
        a = set2_ok(a.a)            // this one ok
        b = set2(a.a)               // this one ok
    if FULL_TEST // swizzle + vector subtype aliasing
        var a = [[Foo b = float4(1.)]]
        var b : Foo
        a = set3(a.b.x)             // (3) 40211: function set result aliases argument incf
        b = set3(a.b.y)             // this one ok
    if FULL_TEST // array [] aliasing
        var a,b : Foo[2]
        a[0] = set(a[1])            // (4) 40211: function set result aliases argument a
                                        // note - false aliasing
        b[0] = set(a[1])            // this one ok
    if FULL_TEST  // array ?[] aliasing, ?? aliasing
        var a,b : Foo[2]
        var defv : Foo
        a?[0] ?? defv = set(a[0])   // (5) 40211: function set result aliases argument a
        a?[1] ?? defv = set(defv)   // (6) 40211: function set result aliases argument a
        b?[0] ?? defv = set(a[0])   // this one ok
    if FULL_TEST // cast aliasing
        var a = [[Foo a = 1]]
        var b : Foo
        unsafe
            a = set2(reinterpret<int&> a.b) // (7) 40211: function set result aliases argument inc
            a = set2(reinterpret<int&> b.b) // this one ok
    if FULL_TEST // op3 aliasing
        var a = [[Foo a = 1]]
        var b : Foo
        var cond = (random_int(seed) & 1) != 0
        cond ? a : b = set(a)       // (8) 40211: function set result aliases argument a
        a = set ( cond ? a : b )    // (9) 40211: function set result aliases argument a
        a = set ( cond ? b : b )    // this one ok
    if FULL_TEST // ref2ptr
        var a = [[Foo a = 1]]
        var b : Foo
        a = set4(unsafe(addr(a.a))) // (10) 40211: function set result aliases argument a
        b = set4(unsafe(addr(a.a))) // this one ok
        a = set4(unsafe(addr(b.a))) // this one ok
    if FULL_TEST // ptr2ref, ref2value
        var a = [[Foo a = 1]]
        var pa = unsafe(addr(a))
        var paa = unsafe(addr(a.a))
        var b : Foo
        a = set2(*paa)      // (11) 40211: function set result aliases argument a
        *pa = set2(a.a)     // (12) 40211: function set result aliases argument a
        b = set2(*paa)      // (13) 40211: function set result aliases argument a
                                // note - false aliasing
        set2(*paa)          // this oke ok
    if FULL_TEST
        g_a = [[Foo a = 1]]
        g_a = set5()        // (14) 40211: function set5 result aliases global variable ::g_a S<::Foo> through function set5
        g_a = [[Foo a = 1]]
        g_b = set5()        // this one ok
    if FULL_TEST
        g_a = [[Foo a = 1]]
        g_a = set6()        // (15) 40211: function set6 result aliases global variable ::g_a S<::Foo> through function get_a
        g_a = [[Foo a = 1]]
        g_b = set6()        // this one ok
    if FULL_TEST
        var a = new [[Foo a = 1]]
        var b : Foo
        b = set(*a)         // (16) 40211: function set result aliases argument a
                                // note - false aliasing
    if FULL_TEST
        var a = [[Foo a = 1]]
        var b = new [[Foo]]
        *b = set(a)     // (17) 40211: function set result always aliases
                                // note - false aliasing
    if FULL_TEST
        g_b = set7()    // (18) 40211: function set7 result aliases global variable ::g_p_a 1<S<::Foo>>? through function set7
                                // note - false aliasing
    if FULL_TEST
        var a = [[Foo a = 1]]
        a = set(pass_ref_a(a))  // (19) 40211: function set result aliases argument a
        a = [[Foo a = 1]]
        a = set(pass_a(a))      // this one ok
