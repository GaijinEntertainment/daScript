struct Ball
    position:float3
    radius:float

def test_sizeof
    // verifying if sizeof accepts expression, not just type
    let
        a:int
    in
        assert(sizeof(a)==4,"integer size must be 4, sizeof understands expressions and types")
    // verifying basic type sizes
    assert(sizeof(type bool)==1,"bool size must be 1")
    assert(sizeof(type int)==4,"int size must be 4")
    assert(sizeof(type int2)==8,"int2 size must be 8")
    assert(sizeof(type int3)==12,"int3 size must be 12")
    assert(sizeof(type uint4)==16,"int4 size must be 16")
    assert(sizeof(type uint)==4,"uint size must be 4")
    assert(sizeof(type uint2)==8,"uint2 size must be 8")
    assert(sizeof(type uint3)==12,"uint3 size must be 12")
    assert(sizeof(type uint4)==16,"uint4 size must be 16")
    assert(sizeof(type float)==4,"float size must be 4")
    assert(sizeof(type float2)==8,"float2 size must be 8")
    assert(sizeof(type float3)==12,"float3 size must be 12")
    assert(sizeof(type float4)==16,"float4 size must be 16")
    assert(sizeof(type string)==8,"string is 8 bytes")
    // verifying array size
    assert(sizeof(type int[2][3])==24,"int[2][3] size must be 24")
    // verifying structure sizes
    assert(sizeof(type Ball*)==8,"ball pointer is 8 bytes")
    assert(sizeof(type Ball)==16,"ball is float3+float")

def verify_default_ball(b:Ball)
    debug(b,"verifying ball")
    assert(b.position==float3(1.0,2.0,3.0),"default ball position is 1,2,3")
    assert(b.radius==5.0,"default ball radius is 5")

def test_new
    let
        b:Ball* = new Ball
    in
        debug(b,"before")
        b.position = float3(1.0,2.0,3.0)
        b.radius = 5.0
        verify_default_ball(debug(b,"ball is")->)
// TODO: support assigning of the structures and default values for the structures
//        let
//            qb:Ball
//            qc:Ball = b->
//        in
//            qb = b->
//            verify_default_ball(qb)
//            verify_default_ball(qc)

def test_array
    let
        arr:int[10]
        idx:int = 0
        val:int
    in
        while idx!=10
            arr[idx++] = idx
        idx = 0
        foreach val in arr
            assert(arr[idx++]==val,"we just set it that way")

def test_prefix_postfix
    let
        a:int = 1
        b:int = 2
    in
        assert(++a==b,"++1==2")
        assert(a==2,"2==2")
        assert(a++==2,"2++==2")
        assert(a==3,"3==3")
        ++b=a
        assert(b==3,"3==3")
        a = ++b+b++
        assert(a==8,"8==8")

def stack_depth_3(a:int;b:float)
    assert(debug(a,"a")==2)
    assert(1.0==debug(b,"b"))
    stackwalk()

def stack_depth_2(b:float&;a:int&;c:string)
    stack_depth_3(debug(a,"a&"),debug(b,"b&"))

def test_stackwalk
    let
        a:float=1.0
        b:int=2
    in
        stack_depth_2(debug(a,"a"),debug(b,"b"),"and string")

// this is where unit tests are
def test
    assert(true,"this should pass")
    test_sizeof()
    test_new()
    test_prefix_postfix()
    test_array()
    test_stackwalk()



