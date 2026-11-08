options gen2

require UnitTest
require math
require dastest/testing_boost public
require daslib/math_boost

[jit]
def list_length(p : Float3x4List?) : int => length(*p)

[test(description = "length of a math-type list substituted from a handled type returns 10 under JIT")]
def test_jit_length_of_a_substituted_math_list(t : T?) {
    testFloat3x4List() $(list) {
        let p = unsafe(addr(list))
        t |> equal(list_length(p), 10)
        t |> success(!jit_enabled() || is_jit_function(@@list_length))
    }
}

//! A keyed handle (UnitTest::PtrSlots) indexes by string and yields `int?`, null for a missing key.
//! The jit's at`handle hook returns that pointer as the value of the expression: loading through it
//! hands back the pointee instead, and dereferences null on a miss. Every other annotation in tree
//! yields a ref from makeIndexType, so this is the only test that reaches the pointer arm.

def check_slots(t : T?; var slots : PtrSlots) {
    t |> success(!jit_enabled() || is_jit_function(@@ < (t : T?; var slots : PtrSlots) : void > check_slots))
    let pa = slots["a"]
    let pb = slots["b"]
    t |> success(pa != null)
    t |> success(pb != null)
    t |> equal(7, *pa)
    t |> equal(9, *pb)
    // a missing key is a null pointer, and evaluating the expression must not read through it
    t |> success(slots["zz"] == null)
}

[test(description = "indexing a handled type's pointer slots by string key returns the right slot under JIT")]
def test_handle_ptr_index(t : T?) {
    testPtrSlots() $(var slots : PtrSlots) {
        check_slots(t, slots)
    }
}

def handle_fun(var a : float4x4) => inverse(a)

def handle_ref_op1(a : float4x4) {
    let c = -a
    return c
}

def handle_ref_op2(a, b : float4x4) {
    let c = a * b
    return c
}

def handle_val(var num : int&) {
    let iid = make_invalid_id()
    num = int(iid)
    let iid2 = EntityId(13)
    return iid2
}

def handle_eid(l : lambda<(val : int) : EntityId>) => invoke(l, 13)

def handle_fields(var m : float4x4) {
    m.x = float4(1, 0, 0, 0)
    m.y = float4(0, 2, 0, 0)
    m.z = float4(0, 0, 3, 0)
    m.w = float4(0, 0, 0, 4)
    return m.y
}

def handle_fields_ptr(var m : float4x4?) {
    m.x = float4(1, 0, 0, 0)
    m.y = float4(0, 2, 0, 0)
    m.z = float4(0, 0, 3, 0)
    m.w = float4(0, 0, 0, 4)
    return m.y
}

def handle_index(var m : float4x4) {
    m[0] = float4(1, 0, 0, 0)
    m[1] = float4(0, 2, 0, 0)
    m[2] = float4(0, 0, 3, 0)
    m[3] = float4(0, 0, 0, 4)
    return m.y
}

def handle_vector_each(arr : Point3Array) {
    var n = 0
    var sum = 0.0
    for (p in arr) {
        n++
        sum += p.x + p.y + p.z
    }
    return int(sum) * 100 + n
}

def handle_vector_at(arr : Point3Array) {
    var sum = 0.0
    for (i in range(arr |> length)) {
        sum += arr[i].y * float(i + 1)
    }
    return int(sum)
}

[test(description = "JIT-compiled handled-type values, operators, functions, fields and indexing return the interpreter's results")]
def test_handle_type(t : T?) {
    t |> run("handle val") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@handle_val))
        var num = 0
        let iid = handle_val(num)
        t |> equal(13, int(iid))
        t |> equal(-1, num)
    }
    t |> run("handle eid") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@handle_eid))
        let l <- @(var val : int) : EntityId {
            return EntityId(val)
        }
        let eid = handle_eid(l)
        t |> equal(13, int(eid))
    }
    t |> run("handle op1") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@handle_ref_op1))
        var a, b, c : float4x4
        identity(a)
        b = handle_ref_op1(a)
        identity(c)
        t |> equal(-c, b)
    }
    t |> run("handle op2") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@handle_ref_op2))
        var a, b, c, d : float4x4
        identity(a)
        identity(b)
        c = handle_ref_op2(a, b)
        identity(d)
        t |> equal(d, c)
    }
    t |> run("handle fun") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@handle_fun))
        var a : float4x4
        identity(a)
        a[1][1] = 2.0
        a[2][2] = 4.0
        a[3][0] = 1.0
        a[3][1] = 2.0
        a[3][2] = 4.0
        let b = handle_fun(a)
        let c = inverse(a)
        t |> equal(c, b)
    }
    t |> run("handle fields") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@handle_fields))
        var m : float4x4
        let my = handle_fields(m)
        t |> equal(float4(1, 0, 0, 0), m.x)
        t |> equal(float4(0, 2, 0, 0), m.y)
        t |> equal(float4(0, 0, 3, 0), m.z)
        t |> equal(float4(0, 0, 0, 4), m.w)
        t |> equal(float4(0, 2, 0, 0), my)
    }
    t |> run("handle index") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@handle_index))
        var m : float4x4
        let my = handle_index(m)
        t |> equal(float4(1, 0, 0, 0), m.x)
        t |> equal(float4(0, 2, 0, 0), m.y)
        t |> equal(float4(0, 0, 3, 0), m.z)
        t |> equal(float4(0, 0, 0, 4), m.w)
        t |> equal(float4(0, 2, 0, 0), my)
    }
    t |> run("handle fields ptr") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@handle_fields_ptr))
        var m : float4x4
        let my = handle_fields_ptr(unsafe(addr(m)))
        t |> equal(float4(1, 0, 0, 0), m.x)
        t |> equal(float4(0, 2, 0, 0), m.y)
        t |> equal(float4(0, 0, 3, 0), m.z)
        t |> equal(float4(0, 0, 0, 4), m.w)
        t |> equal(float4(0, 2, 0, 0), my)
    }
}

[test(description = "each() and indexing over a handled vector type return the interpreter's results under JIT")]
def test_handle_vector(t : T?) {
    t |> run("handle vector each") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@handle_vector_each))
        testPoint3Array() $(arr) {
            t |> equal(6010, handle_vector_each(arr))
        }
    }
    t |> run("handle vector at") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@handle_vector_at))
        testPoint3Array() $(arr) {
            t |> equal(110, handle_vector_at(arr))
        }
    }
}

// regression for issue #2606: JIT codegen crashed on a move-return of a
// register-sized handled value type. the make-expression carries useCMRES while
// the enclosing function returns in a register (non-cmres), so there is no cmres
// param to write into — JIT used to read a non-existent parameter and segfault.
// the function must be [export] so it gets a register-return wrapper (the
// register-return ABI is what makes it non-cmres and triggers the bug).
[export]
def make_big : BigEntityId => <- BigEntityId()

// consume the result without string-interpolating it (interpolating a handled
// value type walks it, which is an unrelated path).
def sink(_b : BigEntityId) : int => 1

[test(description = "a register-sized handled value move-returned from a function is readable by the caller")]
def test_handle_value_move_return(t : T?) {
    t |> run("move-return register-sized handled value") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@make_big))
        let y = make_big()
        t |> equal(sink(y), 1)
    }
    // reading the moved-in value must match a freshly constructed one. before the
    // fix, the register-returned value was a bogus pointer, so this crashed.
    t |> run("move-returned value is readable") @(t : T?) {
        let y = make_big()
        t |> equal("{y}", "{BigEntityId()}")
    }
}
