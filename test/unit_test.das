struct Ball
    position:float3
    radius:float

def test_sizeof
    print("test sizeof:\n")
    // verifying if sizeof accepts expression, not just type
    let ( a:int )
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
    print("ok\n")

def inc(a:int&)
    a ++

def inc_inc(a:int&)
    a ++
    return
    a++
    assert(false,"should never get here")

def madd(a,b,c:int):int
    return a*b+c

def same_value(i:int&):int
    return i
    ++i
    assert(false,"should never get here")

def test_return
    print("test return:\n")
    let ( a:int=2; b:int=3; c:int=3 )
        inc(c)
        assert(madd(2,3,4)==10,"2*3+4==10")
        inc_inc(c)
        assert(c==5)
    print("ok\n")

def verify_default_ball(b:Ball)
    debug(b,"verifying ball")
    assert(b.position==float3(1.0,2.0,3.0),"default ball position is 1,2,3")
    assert(b.radius==5.0,"default ball radius is 5")

def test_ref_ptr(a:Ball*&;b:Ball*)
    a = b
    assert(a==b,"now same pointer")

def test_ret_ptr(a:Ball*):Ball*
    return a

def test_new
    print("test new:\n")
    let ( b:Ball* = new Ball )
        debug(b,"before")
        b.position = float3(1.0,2.0,3.0)
        b.radius = 5.0
        verify_default_ball(debug(b,"ball is")->)
        let ( qb:Ball; qc:Ball* = b; qd:Ball* = new Ball )
            debug(qc,"qc")
            qb <- qc->
            verify_default_ball(qb)
            verify_default_ball(qc->)
            qc = null
            assert(qc==null,"null")
            assert(qd!=null,"not null")
            test_ref_ptr(qc,qd)
            assert(qc==qd,"now same pointer")
            qd = test_ret_ptr(null)
            assert(qd==null,"null")
            assert(qd!=qc,"now one is null")
    print("ok\n")

def test_array
    print("test array:\n")
    let ( arr:int[10]; idx:int = 0; val:int )
        while idx!=10
            arr[idx++] = idx
        idx = 0
        foreach val in arr
            assert(arr[idx++]==val,"we just set it that way")
    print("ok\n")

def test_prefix_postfix
    print("test prefix postfix:\n")
    let ( a,b:int = 1 )
        assert(a==1 & b==1)
        b = 2
        assert(++a==b,"++1==2")
        assert(a==2,"2==2")
        assert(a++==2,"2++==2")
        assert(a==3,"3==3")
        ++b=a
        assert(b==3,"3==3")
        a = ++b+b++
        assert(a==8,"8==8")
    print("ok\n")

def stack_depth_5(a:string)
    assert(a=="from c++")
    stackwalk()

def stack_depth_3(a:int;b:float)
    assert(debug(a,"a")==2)
    assert(1.0==debug(b,"b"))
    stack_depth_4("to c++")

def stack_depth_2(b:float&;a:int&;c:string)
    stack_depth_3(debug(a,"a&"),debug(b,"b&"))

def test_stackwalk
    print("test stackwalk:\n")
    let ( a:float=1.0; b:int=2 )
        stack_depth_2(debug(a,"a"),debug(b,"b"),"and string")
    print("ok\n")

def inc_true(x:int &):bool
    x ++
    return true

def inc_false(x:int &):bool
    x ++
    return false

def test_and_or
    print("test and or:\n")
    let ( x:int=1 )
        assert((inc_false(x) & inc_true(x))==false, "false & true = false")
        assert(x==2,"right did not happen")
        assert((inc_true(x) & inc_false(x))==false, "true & false = false")
        assert(x==4,"both happen")
        assert((inc_false(x) | inc_true(x))==true, "false | true = true")
        assert(x==6,"both happen")
        assert((inc_true(x) | inc_false(x))==true, "true | false = true")
        assert(x==7,"right did not happen")
    print("ok\n")

def test_if_then_else
    print("test if-then-else:\n")
    let ( x:int=1 )
        if x==1
            x=2
        assert(x==2)
        if x==2
            x=3
        else
            x=0
        assert(x==3)
        if x==2
            x=0
        elif x==3
            x=4
        else
            x=0
        assert(x==4)
        if (x=4)==4
            x=5
        elif x==5       // this elif won't happen
            x=0
        elif x==4       // this elif won't happen either
            x =0
        else
            x=0
        assert(x==5)
    print("ok\n")

let
    global_x:int=1

def test_globals
    print("test globals:\n")
    debug(global_x)
    assert(global_x==1,"global variable init")
    let ( y:int=global_x=5 )
        assert(global_x==5 & y==5)
    print("ok\n")

def test_copy
    print("test copy:\n")
    let ( a,aCopy:int[10]; b,bCopy:Ball; idx:int )
        idx = 0
        while idx!=10
            a[idx++] = idx
        aCopy <- a
        debug(a,"a=")
        debug(aCopy,"aCopy=")
        idx = 0
        while idx!=10
            assert(aCopy[idx++]==idx)
        b.position = float3(1.0,2.0,3.0)
        b.radius = 5.0
        bCopy <- b
        verify_default_ball(b)
    print("ok")

def test_break
    print("test break:\n")
    let ( i,j,k:int=0 )
        while i < 20
            i ++
            if i == 10
                break
            j++
        assert(j==9)
        i = 0
        while i < 10
            i ++
            j = 0
            while j < 10
                j ++
                if j > i
                    break
                k++
            if k>=20
                break
        assert(i==6 & j==7 & k==21)
    print("ok\n")

def test_string
    print("test string:\n")
    let ( a:string="foo"; b:string="bar"; c:string=a+b; d:string )
        assert(c=="foobar")
        d = a + b
        assert(d=="foobar")
        b += a
        assert(b=="barfoo")
    print("ok\n")

def pass_array ( arr:array<int> )
    push(arr,4)

def test_dynamic_array
    print("test array:\n")
    let ( arr:array<int>; arr2:array<int> )
        assert(length(arr)==0,"array starts at 0 length")
        push(arr,1)
        push(arr,2)
        push(arr,3)
        assert(length(arr)==3,"array length is 3")
        assert(capacity(arr)==16,"its 2x, but no less than 16")
        debug(arr)
        pass_array(arr)
        assert(length(arr)==4,"array length is 4")
        debug(arr)
        resize(arr,6)
        assert(length(arr)==6,"array length is now 6")
        debug(arr)
        resize(arr,17)
        debug(arr)
        assert(capacity(arr)==32,"now bigger")
        reserve(arr,33)
        debug(arr)
        assert(capacity(arr)==33,"now even bigger")
        resize(arr,4)
        assert(capacity(arr)==33,"did not grow smaller")
        assert(length(arr)==4,"resized properly")
        debug(arr, "array 1 is")
        arr2 <- arr // todo: make this work properly
        debug(arr2, "array 2 is")
        let ( index:int = 0 )
            while index < 4
                debug(arr[index++]++)
        debug(arr)
    print("ok\n")

// this is where unit tests are
def test
    print("unit tests started\n")
    assert(true,"this should pass")
    test_globals()
    test_sizeof()
    test_return()
    test_new()
    test_prefix_postfix()
    test_array()
    test_and_or()
    test_if_then_else()
    test_copy()
    test_stackwalk()
    test_break()
    test_string()
    test_dynamic_array()
    print("unit tests finished\n")



