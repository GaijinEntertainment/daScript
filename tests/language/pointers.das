options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require UnitTest
require dastest/testing_boost public
require daslib/safe_addr

struct TestStruct {
    x : int = 10
    y : int = 20
}

[test(description = "new with named fields or with defaults builds the struct, and field reads and writes through the pointer auto-dereference it")]
def test_new_and_fields(t : T?) {
    t |> run("new with fields") @(t : T?) {
        var p = new TestStruct(x = 5, y = 7)
        t |> equal(p.x, 5)
        t |> equal(p.y, 7)
        unsafe {
            delete p
        }
    }
    t |> run("new default") @(t : T?) {
        var p = new TestStruct()
        t |> equal(p.x, 10)
        t |> equal(p.y, 20)
        unsafe {
            delete p
        }
    }
    t |> run("auto-deref field access") @(t : T?) {
        var inscope p = new TestStruct(x = 42, y = 99)
        // p.x auto-dereferences the pointer — no -> needed
        t |> equal(p.x, 42)
        t |> equal(p.y, 99)
    }
    t |> run("modify through pointer") @(t : T?) {
        var inscope p = new TestStruct(x = 1, y = 2)
        p.x = 100
        p.y = 200
        t |> equal(p.x, 100)
        t |> equal(p.y, 200)
    }
}

[test(description = "addr of a local or of a struct field yields a pointer whose * and deref read and write the original variable")]
def test_addr_and_deref(t : T?) {
    t |> run("addr and deref") @(t : T?) {
        var x = 42
        var p = unsafe(addr(x))
        t |> equal(*p, 42)
        t |> equal(deref(p), 42)
    }
    t |> run("modify through addr") @(t : T?) {
        var x = 10
        var p = unsafe(addr(x))
        *p = 99
        t |> equal(x, 99)
    }
    t |> run("addr of struct field") @(t : T?) {
        var s = TestStruct(x = 7, y = 8)
        var px = unsafe(addr(s.x))
        t |> equal(*px, 7)
        *px = 77
        t |> equal(s.x, 77)
    }
}

[test(description = "safe_addr of a local yields a temporary pointer that reads and writes the local")]
def test_safe_addr(t : T?) {
    t |> run("safe_addr local") @(t : T?) {
        var a = 55
        var pa = safe_addr(a)       // returns int?# (temporary pointer)
        t |> equal(*pa, 55)
    }
    t |> run("safe_addr modify") @(t : T?) {
        var b = 10
        var pb = safe_addr(b)
        *pb = 20
        t |> equal(b, 20)
    }
}

[test(description = "a default-initialized pointer equals null and the address of a local does not")]
def test_null_pointer(t : T?) {
    t |> run("null default") @(t : T?) {
        var p : int?
        t |> equal(p == null, true)
        t |> equal(p != null, false)
    }
    t |> run("non-null check") @(t : T?) {
        var x = 42
        var p = unsafe(addr(x))
        t |> equal(p != null, true)
        t |> equal(p == null, false)
    }
    t |> run("null deref requires unsafe") @(t : T?) {
        // Dereferencing null requires unsafe — we just verify null state
        var p : int?
        t |> equal(p == null, true)
    }
}

[test(description = "?. with ?? reads the field through a valid pointer and yields the fallback through a null pointer")]
def test_safe_navigation(t : T?) {
    t |> run("?. on valid pointer") @(t : T?) {
        var inscope p = new TestStruct(x = 42, y = 99)
        t |> equal(p?.x ?? -1, 42)
        t |> equal(p?.y ?? -1, 99)
    }
    t |> run("?. on null pointer") @(t : T?) {
        var p : TestStruct?
        t |> equal(p?.x ?? -1, -1)
        t |> equal(p?.y ?? -1, -1)
    }
    t |> run("?? null coalescing on int?") @(t : T?) {
        var x = 42
        var p = unsafe(addr(x))
        var q : int?
        if (p != null) {
            t |> equal(*p, 42)
        }
        t |> equal(q == null, true)
    }
}

[test(description = "indexing a pointer to an array element reads and writes the elements at each offset")]
def test_pointer_indexing(t : T?) {
    t |> run("index into array via pointer") @(t : T?) {
        var arr <- [10, 20, 30, 40, 50]
        unsafe {
            var p = addr(arr[0])
            t |> equal(p[0], 10)
            t |> equal(p[1], 20)
            t |> equal(p[2], 30)
            t |> equal(p[3], 40)
            t |> equal(p[4], 50)
        }
    }
    t |> run("write through pointer index") @(t : T?) {
        var arr <- [1, 2, 3]
        unsafe {
            var p = addr(arr[0])
            p[1] = 99
        }
        t |> equal(arr[1], 99)
    }
}

[test(description = "++ and += on a pointer advance it over array elements one element per step")]
def test_pointer_arithmetic(t : T?) {
    t |> run("pointer increment") @(t : T?) {
        var arr <- [10, 20, 30, 40, 50]
        unsafe {
            var p = addr(arr[0])
            t |> equal(*p, 10)
            ++ p
            t |> equal(*p, 20)
            ++ p
            t |> equal(*p, 30)
        }
    }
    t |> run("pointer += N") @(t : T?) {
        var arr <- [10, 20, 30, 40, 50]
        unsafe {
            var p = addr(arr[0])
            p += 3
            t |> equal(*p, 40)
            p += 1
            t |> equal(*p, 50)
        }
    }
}

[test(description = "intptr is non-zero and stable for one pointer and differs between two, and for a function or a lambda it is the handle a void pointer reinterpret reads")]
def test_intptr(t : T?) {
    t |> run("intptr non-zero") @(t : T?) {
        var x = 42
        var p = unsafe(addr(x))
        let address = intptr(p)
        t |> equal(address != uint64(0), true)
    }
    t |> run("intptr same pointer same value") @(t : T?) {
        var x = 42
        var p = unsafe(addr(x))
        t |> equal(intptr(p), intptr(p))
    }
    t |> run("intptr different pointers differ") @(t : T?) {
        var x = 1
        var y = 2
        unsafe {
            var px = addr(x)
            var py = addr(y)
            t |> equal(intptr(px) != intptr(py), true)
        }
    }
    t |> run("intptr of a function value is its handle, whatever spelling carries it") @(t : T?) {
        let typed_slot : function<(a : int) : int> = @@ip_twice
        t |> equal(intptr(typed_slot) != 0ul, true, "a function handle is not null")
        t |> equal(intptr(@@ip_twice), intptr(typed_slot),
                   "the direct and the typed spelling agree")
        unsafe {
            t |> equal(intptr(@@ip_twice), intptr(reinterpret<void?>(@@ip_twice)),
                       "the handle is the address the pointer overload reads")
        }
        t |> equal(intptr(@@ip_twice) != intptr(@@ip_thrice), true,
                   "two functions do not share a handle")
    }
    t |> run("intptr of a lambda value is its capture address") @(t : T?) {
        let lam <- @(a : int) : int { return a + 1 }
        let other_lam <- @(a : int) : int { return a + 2 }
        t |> equal(intptr(lam) != 0ul, true, "a lambda handle is not null")
        t |> equal(intptr(lam), intptr(unsafe(reinterpret<void?>(lam))),
                   "the handle is the capture pointer")
        t |> equal(intptr(lam) != intptr(other_lam), true, "two lambdas do not share a handle")
    }
}

def ip_twice(a : int) : int => a * 2

def ip_thrice(a : int) : int => a * 3

[test(description = "reinterpret of float 1.0 to int gives the IEEE-754 bits 0x3F800000, and reinterpret back restores 1.0")]
def test_reinterpret(t : T?) {
    t |> run("float to int roundtrip") @(t : T?) {
        unsafe {
            let f = 1.0
            let bits = reinterpret<int>(f)
            t |> equal(bits, int(0x3F800000))       // IEEE 754 for 1.0
            let back = reinterpret<float>(bits)
            t |> equal(back, 1.0)
        }
    }
}

[test(description = "an int pointer reinterpreted to void? and back still reads the original value")]
def test_void_pointer(t : T?) {
    t |> run("roundtrip through void?") @(t : T?) {
        var x = 42
        unsafe {
            var p = addr(x)
            var vp : void? = reinterpret<void?>(p)
            var p2 = reinterpret<int?>(vp)
            t |> equal(*p2, 42)
        }
    }
}

[test(description = "delete frees a pointer and sets it to null, and a var inscope pointer is freed at scope exit")]
def test_delete(t : T?) {
    t |> run("delete sets null") @(t : T?) {
        var p = new TestStruct(x = 1, y = 2)
        t |> equal(p != null, true)
        unsafe {
            delete p
        }
        t |> equal(p == null, true)
    }
    t |> run("var inscope auto-deletes") @(t : T?) {
        // var inscope p = new ... — p is deleted at scope exit
        // We just verify it works without crash
        var inscope p = new TestStruct(x = 5, y = 6)
        t |> equal(p.x, 5)
    }
}

def pa_test_signed(t : T?; dummy : auto(TT)) {
    unsafe {
        var zero = TT(0); zero--; zero++
        var one = TT(1); one--; one++
        var two = TT(2); two--; two++
        var three = TT(3); three--; three++
        var neg_one = TT(-1); neg_one--; neg_one++
        var a <- fixed_array<int>(0, 1, 2, 3)
        var pA = addr(a[1])
        t |> equal(*pA, 1)
        pA++
        t |> equal(*pA, 2)
        pA--
        t |> equal(*pA, 1)
        t |> equal(pA[zero], 1)
        t |> equal(pA[one], 2)
        t |> equal(pA[neg_one], 0)
        pA += two
        t |> equal(*pA, 3)
        pA -= three
        t |> equal(*pA, 0)
        t |> equal(*(pA + two), 2)
        pA += 2
        t |> equal(*(pA - one), 1)
    }
}

def pa_test_unsigned(t : T?; dummy : auto(TT)) {
    unsafe {
        var zero = TT(0); zero--; zero++
        var one = TT(1); one--; one++
        var two = TT(2); two--; two++
        var three = TT(3); three--; three++
        var a <- fixed_array<int>(0, 1, 2, 3)
        var pA = addr(a[1])
        t |> equal(*pA, 1)
        pA++
        t |> equal(*pA, 2)
        pA--
        t |> equal(*pA, 1)
        t |> equal(pA[zero], 1)
        t |> equal(pA[one], 2)
        pA += two
        t |> equal(*pA, 3)
        pA -= three
        t |> equal(*pA, 0)
        t |> equal(*(pA + two), 2)
        pA += 2
        t |> equal(*(pA - one), 1)
    }
}

[test(description = "pointer ++, --, +=, -=, + and - and indexing step by whole elements for int, int64, uint and uint64 offsets alike, negative signed offsets included")]
def test_ptr_arithmetic(t : T?) {
    t |> run("signed int pointer arithmetic") @(t : T?) {
        pa_test_signed(t, type<int>)
    }
    t |> run("signed int64 pointer arithmetic") @(t : T?) {
        pa_test_signed(t, type<int64>)
    }
    t |> run("unsigned uint pointer arithmetic") @(t : T?) {
        pa_test_unsigned(t, type<uint>)
    }
    t |> run("unsigned uint64 pointer arithmetic") @(t : T?) {
        pa_test_unsigned(t, type<uint64>)
    }
}

def pi_fn_with_default_null_ptr(var a : int? implicit = default<int?>) {
    var b : int
    a ?? b = 2
}

[test(description = "a pointer to a fixed array element derefs and indexes every element, and a null default pointer argument takes the ?? fallback without a crash")]
def test_ptr_index(t : T?) {
    t |> run("pointer deref and index") @(t : T?) {
        let tf = fixed_array<int>(1, 2, 3, 4, 5)
        unsafe {
            let pt = addr(tf[0])
            t |> equal(deref(pt), 1)
            t |> equal(pt[0], 1)
            t |> equal(pt[1], 2)
            t |> equal(pt[2], 3)
            t |> equal(pt[3], 4)
            t |> equal(pt[4], 5)
        }
    }
    t |> run("default null pointer argument") @(t : T?) {
        pi_fn_with_default_null_ptr()
        t |> success(true)
    }
}

struct SpaFoo {
    a, b : int
}

[test(description = "?[] on an int pointer writes and reads the element at the offset when the pointer is non-null and takes the ?? fallback when it is null")]
def test_ptr_safe_at_int(t : T?) {
    t |> run("int pointer safe at - non null") @(t : T?) {
        var ar = fixed_array(10, 20, 30)
        var p : int? = unsafe(addr(ar[0]))
        var dummy = 0
        unsafe(p?[0]) ?? dummy = 1
        t |> equal(ar[0], 1)
        t |> equal(dummy, 0)
    }
    t |> run("int pointer safe at - offset") @(t : T?) {
        var ar = fixed_array(10, 20, 30)
        var p : int? = unsafe(addr(ar[0]))
        var dummy = 0
        unsafe(p?[2]) ?? dummy = 42
        t |> equal(ar[2], 42)
        t |> equal(dummy, 0)
    }
    t |> run("int pointer safe at - null") @(t : T?) {
        var p : int?
        var dummy = 0
        unsafe(p?[0]) ?? dummy = 99
        t |> equal(dummy, 99)
    }
    t |> run("int pointer safe at - null with offset") @(t : T?) {
        var p : int?
        var dummy = 0
        unsafe(p?[5]) ?? dummy = 77
        t |> equal(dummy, 77)
    }
    t |> run("int pointer safe at - read value") @(t : T?) {
        var ar = fixed_array(10, 20, 30)
        var p : int? = unsafe(addr(ar[0]))
        let val = unsafe(p?[1]) ?? 0
        t |> equal(val, 20)
    }
    t |> run("int pointer safe at - read null") @(t : T?) {
        var p : int?
        let val = unsafe(p?[0]) ?? 42
        t |> equal(val, 42)
    }
}

[test(description = "?[] on a pointer with a uint index reads the element when non-null and the ?? fallback when null")]
def test_ptr_safe_at_uint_index(t : T?) {
    t |> run("pointer safe at uint index - non null") @(t : T?) {
        var ar = fixed_array(10, 20, 30)
        var p : int? = unsafe(addr(ar[0]))
        let val = unsafe(p?[1u]) ?? 0
        t |> equal(val, 20)
    }
    t |> run("pointer safe at uint index - null") @(t : T?) {
        var p : int?
        let val = unsafe(p?[0u]) ?? 42
        t |> equal(val, 42)
    }
}

[test(description = "?[] on a pointer with an int64 index reads the element when non-null and the ?? fallback when null")]
def test_ptr_safe_at_int64_index(t : T?) {
    t |> run("pointer safe at int64 index - non null") @(t : T?) {
        var ar = fixed_array(10, 20, 30)
        var p : int? = unsafe(addr(ar[0]))
        let val = unsafe(p?[2l]) ?? 0
        t |> equal(val, 30)
    }
    t |> run("pointer safe at int64 index - null") @(t : T?) {
        var p : int?
        let val = unsafe(p?[0l]) ?? 42
        t |> equal(val, 42)
    }
}

[test(description = "?[] on a pointer with a uint64 index reads the element when non-null and the ?? fallback when null")]
def test_ptr_safe_at_uint64_index(t : T?) {
    t |> run("pointer safe at uint64 index - non null") @(t : T?) {
        var ar = fixed_array(10, 20, 30)
        var p : int? = unsafe(addr(ar[0]))
        let val = unsafe(p?[1ul]) ?? 0
        t |> equal(val, 20)
    }
    t |> run("pointer safe at uint64 index - null") @(t : T?) {
        var p : int?
        let val = unsafe(p?[0ul]) ?? 42
        t |> equal(val, 42)
    }
}

[test(description = "?[] on a struct pointer returns the whole element when non-null and the fallback struct when null")]
def test_ptr_safe_at_struct(t : T?) {
    t |> run("struct pointer safe at - non null") @(t : T?) {
        var ar = fixed_array(SpaFoo(a = 1, b = 2), SpaFoo(a = 3, b = 4))
        var p : SpaFoo? = unsafe(addr(ar[0]))
        let dummy = SpaFoo()
        let val = unsafe(p?[1]) ?? dummy
        t |> equal(val.a, 3)
        t |> equal(val.b, 4)
    }
    t |> run("struct pointer safe at - null") @(t : T?) {
        var p : SpaFoo?
        let dummy = SpaFoo(a = 99, b = 88)
        let val = unsafe(p?[0]) ?? dummy
        t |> equal(val.a, 99)
        t |> equal(val.b, 88)
    }
}

[test(description = "?[] on a pointer to pointers returns the stored pointer when non-null and the null fallback when null")]
def test_ptr_safe_at_ptr_to_ptr(t : T?) {
    t |> run("pointer to pointer safe at - non null") @(t : T?) {
        var a = 10
        var b = 20
        var ptrs = fixed_array(unsafe(addr(a)), unsafe(addr(b)))
        var pp : int ?? = unsafe(addr(ptrs[0]))
        var dummy : int?
        let val = unsafe(pp?[1]) ?? dummy
        t |> success(val != null)
        t |> equal(deref(val), 20)
    }
    t |> run("pointer to pointer safe at - null") @(t : T?) {
        var pp : int ??
        var dummy : int?
        let val = unsafe(pp?[0]) ?? dummy
        t |> success(val == null)
    }
}

[test(description = "deref of a pointer returned by a bound C++ function prints its pointee inside string interpolation")]
def test_deref_ptr_fun(t : T?) {
    feint("{deref(getPtr())}")
    t |> equal("{deref(getPtr())}", "0")
}

[test(description = "addr<float?> of a float3 reinterprets its address, so indexing the result reads the components")]
def test_addr_cast_basic(t : T?) {
    var v = float3(1., 2., 3.)
    unsafe {
        t |> equal(addr<float?>(v)[1], 2.0)
    }
}

[test(description = "one expression-form unsafe covers both the addr and the reinterpret of addr<uint8?>, and a write through the result lands in the buffer")]
def test_addr_cast_single_unsafe(t : T?) {
    var buf : array<uint8>
    buf |> resize(4)
    var p = unsafe(addr<uint8?>(buf[0]))
    *(unsafe(p + 2)) = uint8(0xAB)
    t |> equal(uint(buf[2]), 0xABu)
}

[test(description = "addr<int?> of an int is a plain addr whose writes reach the variable")]
def test_addr_cast_exact_type(t : T?) {
    // exact-type addr<T?> folds to plain addr(x); unsafe still required
    // (see cant_addr_cast_unsafe)
    var i = 42
    var pi = unsafe(addr<int?>(i))
    *pi = 43
    t |> equal(i, 43)
}

[test(description = "a postfix index after addr<T?>(x) applies to the cast pointer, not to x")]
def test_addr_cast_self_delimiting(t : T?) {
    var v = float3(1., 2., 3.)
    unsafe {
        let q = addr<float?>(v)[2]
        t |> equal(q, 3.0)
    }
}

// A callee that writes its output pointer ONLY through a helper call
// that receives the pointer OFFSET (`cpy2(d + k*2, ...)`) inside a
// loop is inferred [modify_argument]: the offset desugars to
// `i_das_ptr_add(d, ...)`, a firstArgReturnType builtin whose result
// aliases the base pointer's pointee, and the side-effect analyzer
// recurses through it - else the call is dead-code-eliminated.

def cpy2(var d : float?; s : float const?; n : int64) {
    for (j in range64(n)) { unsafe { d[j] = s[j] + 100.0 } }
}

def pk(var d : float?; s : float const?) {
    unsafe { for (k in range64(2l)) { cpy2(d + k * 2l, s + k * 2l, 2l) } }
}

def pk_direct(var d : float?; s : float const?) {
    unsafe { for (k in range64(4l)) { d[k] = s[k] + 100.0 } }
}

def pk_unoffset(var d : float?; s : float const?) {
    cpy2(d, s, 4l)
}

def fill(var a : float[4]; base : float) {
    for (i in range(4)) { a[i] = base + float(i) }
}

[test(description = "a callee writing its output pointer only through a helper called with an offset pointer keeps the write, exactly like a direct write or an unoffset helper call")]
def test_offset_pointer_write_through_helper(t : T?) {
    t |> run("offset-pointer helper write") @(t : T?) {
        var a : float[4]; fill(a, -9.0)
        var b : float[4]; fill(b, 0.0)
        unsafe { pk(addr(a[0]), addr(b[0])) }
        t |> equal(a[0], 100.0)
        t |> equal(a[1], 101.0)
        t |> equal(a[2], 102.0)
        t |> equal(a[3], 103.0)
    }
    t |> run("direct write control") @(t : T?) {
        var a : float[4]; fill(a, -9.0)
        var b : float[4]; fill(b, 0.0)
        unsafe { pk_direct(addr(a[0]), addr(b[0])) }
        t |> equal(a[0], 100.0)
        t |> equal(a[3], 103.0)
    }
    t |> run("unoffset helper control") @(t : T?) {
        var a : float[4]; fill(a, -9.0)
        var b : float[4]; fill(b, 0.0)
        unsafe { pk_unoffset(addr(a[0]), addr(b[0])) }
        t |> equal(a[0], 100.0)
        t |> equal(a[3], 103.0)
    }
}

def offset(p : uint8?; n : int; zero : auto(TT)) : TT =>
    unsafe(reinterpret<TT>(unsafe(p + n))) - unsafe(reinterpret<TT>(p))

def offset_via_variable(p : uint8?; n : int) : uint64 {
    let q : uint8? = unsafe(p + n)
    return unsafe(reinterpret<uint64>(q)) - unsafe(reinterpret<uint64>(p))
}

[test(description = "reinterpret to a pointer of an extern call's uint64 result agrees whether applied directly or through a local")]
def test_reinterpret_of_an_extern_uint64_result(t : T?) {
    let bits = heap_bytes_allocated()
    let via_local = unsafe(reinterpret<int?>(bits))
    let direct = unsafe(reinterpret<int?>(heap_bytes_allocated()))
    t |> equal(via_local == null, direct == null, "the direct and the two-step reinterpret agree")
}

[test(description = "reinterpreting two pointers to integers and subtracting gives their byte offset, inline in a generic and through a local alike")]
def test_reinterpret_of_an_extern_pointer_result(t : T?) {
    var data : uint8[64]
    let p = unsafe(addr(data[0]))
    if (typeinfo sizeof(p) == 8) {
        t |> equal(offset_via_variable(p, 4), 4ul)
        t |> equal(offset(p, 4, 0ul), 4ul)
    }
    t |> equal(offset(p, 4, 0u), 4u)
}

class IdClass {
    def test_method() {
        pass
    }
}

[test(description = "a class pointer copied out of an array, by inferred type or by explicit pointer type, stays callable with ->")]
def test_ignore_deref(t : T?) {
    t |> run("deref pointer from array by value") @(t : T?) {
        var arr <- [new IdClass(), new IdClass()]
        var a = arr[0]
        a->test_method()
        t |> success(true)
    }
    t |> run("explicit pointer type from array") @(t : T?) {
        var arr <- [new IdClass(), new IdClass()]
        var b : IdClass? = arr[0]
        b->test_method()
        t |> success(true)
    }
}

def inside_scope_test(t : T?) {
    unsafe {
        var sptr <- new TestObjectSmart
        t |> equal(getTotalTestObjectSmart(), 1)
        return get_ptr(sptr)
    }
}

def scope_test(t : T?) {
    t |> equal(getTotalTestObjectSmart(), 0)
    // we loose one in there, return regular pointer, and cast it to
    // smart_ptr again
    let lostPtr = inside_scope_test(t)
    t |> equal(getTotalTestObjectSmart(), 1)
    unsafe {
        var sptr : smart_ptr<TestObjectSmart> <- reinterpret< smart_ptr<TestObjectSmart> >(lostPtr)
        delete sptr
    }
    t |> equal(getTotalTestObjectSmart(), 0)
}

def inside_move_test(t : T?) {
    t |> equal(getTotalTestObjectSmart(), 0)
    var inscope t1 <- new TestObjectSmart
    var inscope t2 <- t1                            // unsafe operation
    t |> equal(smart_ptr_use_count(t1), 0u)
    t |> equal(smart_ptr_use_count(t2), 1u)
    t |> equal(getTotalTestObjectSmart(), 1)
    return <- t2
}

def move_test(t : T?) {
    t |> equal(getTotalTestObjectSmart(), 0)
    {
        var inscope sptr <- inside_move_test(t)
        t |> equal(getTotalTestObjectSmart(), 1)
    }
    t |> equal(getTotalTestObjectSmart(), 0)
}

def ref_count_test(t : T?) {
    t |> equal(getTotalTestObjectSmart(), 0)
    var inscope sptr <- new TestObjectSmart         // ref_count = 1
    t |> equal(getTotalTestObjectSmart(), 1)
    t |> equal(smart_ptr_use_count(sptr), 1u)
    var ptr = get_ptr(sptr)
    var pptr : TestObjectSmart?
    pptr = get_ptr(sptr)                    // nolint:STYLE011 - assignment (not init) from get_ptr is the tested construct
    t |> equal(sptr, ptr)
    t |> equal(get_const_ptr(sptr), ptr)
    var inscope qptr : smart_ptr<TestObjectSmart>
    qptr := get_ptr(sptr)                   // ref_count = 2
    t |> equal(smart_ptr_use_count(sptr), 2u)
    t |> equal(qptr, ptr)
    t |> equal(get_const_ptr(qptr), ptr)
    t |> equal(qptr, sptr)
    var inscope fptr := qptr                        // ref_count = 3
    t |> equal(smart_ptr_use_count(sptr), 3u)
    t |> success(fptr != null)
    t |> equal(get_const_ptr(fptr), sptr)
    t |> equal(fptr, qptr)
    t |> equal(fptr, sptr)
    t |> equal(smart_ptr_use_count(sptr), 3u)
    t |> equal(getTotalTestObjectSmart(), 1)
    delete sptr                             // ref_count = 2
    t |> equal(getTotalTestObjectSmart(), 1)
    t |> equal(smart_ptr_use_count(sptr), 0u)
    t |> equal(smart_ptr_use_count(qptr), 2u)
    delete qptr                             // ref_count = 1
    t |> equal(getTotalTestObjectSmart(), 1)
    t |> equal(smart_ptr_use_count(qptr), 0u)
    t |> equal(smart_ptr_use_count(fptr), 1u)
    delete fptr                             // physical delete
    t |> equal(getTotalTestObjectSmart(), 0)
    t |> equal(smart_ptr_use_count(fptr), 0u)
}

def access_test(t : T?) {
    t |> equal(getTotalTestObjectSmart(), 0)
    var inscope ptr <- new TestObjectSmart
    t |> equal(ptr.fooData, 1234)     //  .
    var val = ptr ?. fooData ?? 1     // ?? rvalue
    t |> equal(val, 1234)
    ptr ?. fooData ?? val = 13        // ?? lvalue
    t |> equal(ptr.fooData, 13)
    delete ptr
    val = ptr ?. fooData ?? 2
    t |> equal(val, 2)
    unsafe {
        ptr <- new TestObjectSmart
        ptr.first <- new TestObjectSmart
    }
    t |> equal(ptr.first.fooData, 1234)
    t |> equal(ptr ?. first ?. fooData ?? 13, 1234)
    delete ptr.first
    delete ptr
    t |> equal(getTotalTestObjectSmart(), 0)
}

def fn_test(t : T?) {
    t |> equal(getTotalTestObjectSmart(), 0)
    var inscope ptr <- makeTestObjectSmart()
    t |> equal(countTestObjectSmart(ptr), 1u)
    t |> equal(getTotalTestObjectSmart(), 1)
    delete ptr
    t |> equal(getTotalTestObjectSmart(), 0)
}

[test(description = "smart_ptr use counts rise on := and fall on delete with the object freed at zero, a move transfers ownership, a raw pointer reinterpreted back to smart_ptr frees the object, and ?. with ?? reads and writes through it")]
def test_smart_ptr(t : T?) {
    ref_count_test(t)
    move_test(t)
    scope_test(t)
    access_test(t)
    fn_test(t)
}

typedef TestObjectSmartPtr = smart_ptr<TestObjectSmart>

[export]
def mks() => <- default<TestObjectSmartPtr>

[test(description = "the default value of a smart_ptr type, returned by move, holds a null pointer")]
def test_mksmart_zero(t : T?) {
    var inscope p <- mks()
    t |> equal(get_ptr(p), null)
}

[test(description = "memset8, memset16, memset32, memset64 and memset128 fill exactly the counted elements from the start address and leave the rest zero")]
def test_memset(t : T?) {
    t |> run("memset8") @(t : T?) {
        var a : uint8[10]
        unsafe {
            memset8(addr(a[2]), 0x55u8, 3)
        }
        for (i in 0..10) {
            t |> equal((i >= 2 && i < 5) ? 0x55u8 : 0u8, a[i])
        }
    }
    t |> run("memset16") @(t : T?) {
        var a : uint16[10]
        unsafe {
            memset16(addr(a[2]), uint16(0x55), 3)
        }
        for (i in 0..10) {
            t |> equal((i >= 2 && i < 5) ? uint16(0x55) : uint16(0), a[i])
        }
    }
    t |> run("memset32") @(t : T?) {
        var a : uint[10]
        unsafe {
            memset32(addr(a[2]), 0x55, 3)
        }
        for (i in 0..10) {
            t |> equal((i >= 2 && i < 5) ? 0x55 : 0x00, a[i])
        }
    }
    t |> run("memset64") @(t : T?) {
        var a : uint64[10]
        unsafe {
            memset64(addr(a[2]), uint64(0x55), 3)
        }
        for (i in 0..10) {
            t |> equal((i >= 2 && i < 5) ? uint64(0x55) : uint64(0), a[i])
        }
    }
    t |> run("memset128") @(t : T?) {
        var a : uint4[10]
        unsafe {
            memset128(addr(a[2]), uint4(0x12, 0x34, 0x56, 0x78), 3)
        }
        for (i in 0..10) {
            t |> equal((i >= 2 && i < 5) ? uint4(0x12, 0x34, 0x56, 0x78) : uint4(0), a[i])
        }
    }
}

[test(description = "memmove copies an overlapping range in both directions with an int, uint, int64 or uint64 size, and copies between distinct buffers")]
def test_memmove(t : T?) {
    t |> run("memmove overlapping ranges, every size spelling") @(t : T?) {
        // shifting a[0..6] right by 2 overlaps itself: memmove keeps the
        // source bytes, unlike memcpy
        var a : uint8[10]
        for (i in 0..10) {
            a[i] = uint8(i)
        }
        unsafe {
            memmove(addr(a[2]), addr(a[0]), 6)
        }
        for (i in 0..10) {
            t |> equal(uint8(i >= 2 && i < 8 ? i - 2 : i), a[i])
        }
        unsafe {
            memmove(addr(a[0]), addr(a[2]), 6u)
        }
        for (i in 0..6) {
            t |> equal(uint8(i), a[i])
        }
        unsafe {
            memmove(addr(a[2]), addr(a[0]), 6l)
        }
        t |> equal(0u8, a[2])
        t |> equal(5u8, a[7])
        unsafe {
            memmove(addr(a[0]), addr(a[2]), 6ul)
        }
        for (i in 0..6) {
            t |> equal(uint8(i), a[i])
        }
    }
    t |> run("memmove between distinct buffers") @(t : T?) {
        var src, dst : int[4]
        for (i in 0..4) {
            src[i] = i * 11
        }
        unsafe {
            memmove(addr(dst[0]), addr(src[0]), 4 * 4)
        }
        for (i in 0..4) {
            t |> equal(i * 11, dst[i])
        }
    }
}

[test(description = "memzero clears a scalar and every element of a fixed array")]
def test_memzero(t : T?) {
    t |> run("memzero float") @(t : T?) {
        var x = 1.0f
        memzero(x)
        t |> equal(x, 0.0f)
    }
    t |> run("memzero fixed_array") @(t : T?) {
        var tf = fixed_array<int>(1, 2)
        memzero(tf)
        t |> equal(tf[0], 0)
        t |> equal(tf[1], 0)
    }
}

// A write through a const-stripped (reinterpret to mutable) `float
// const?` parameter is a side effect of every caller that forwards
// the pointer - through `float const?` pass-throughs or let aliases -
// so no caller is judged pure and dead-code-eliminated.

def leaf(p : float const?; n : int) { // nolint:LINT031 - the const-stripped write is under test
    unsafe {
        var w = reinterpret<float?>(p)
        for (i in range(n)) { w[i] = 7.0 }
    }
}

def passthrough(p : float const?; n : int) {
    leaf(p, n)
}

def passthrough2(p : float const?; n : int) {
    passthrough(p, n)
}

def leaf_mut(var p : float?; n : int) {
    for (i in range(n)) { unsafe(p[i]) = 7.0 }
}
def passthrough_mut(var p : float?; n : int) {
    leaf_mut(p, n)
}

def passthrough_alias(p : float const?; n : int) {
    let q = p
    leaf(q, n)
}

def passthrough_alias2(p : float const?; n : int) {
    let q1 = p
    let q2 = q1
    leaf(q2, n)
}

def writer_via_copy(var p : float?; n : int) {
    var w = p
    for (i in range(n)) { unsafe(w[i]) = 7.0 }
}

def fill_zero(var a : float[4]) {
    for (i in range(4)) { a[i] = 0.0 }
}

[test(description = "a write through a const-stripped pointer parameter stays a side effect when the pointer reaches it through one or two const pass-throughs or let aliases, so every forwarding call keeps the write")]
def test_const_strip_write_through_passthrough(t : T?) {
    t |> run("direct leaf write") @(t : T?) {
        var a : float[4]
        fill_zero(a)
        leaf(unsafe(addr(a[0])), 4)
        t |> equal(a[0], 7.0)
    }
    t |> run("write forwarded through const? pass-through") @(t : T?) {
        var b : float[4]
        fill_zero(b)
        passthrough(unsafe(addr(b[0])), 4)
        t |> equal(b[0], 7.0)
    }
    t |> run("write forwarded through two const? hops") @(t : T?) {
        var c : float[4]
        fill_zero(c)
        passthrough2(unsafe(addr(c[0])), 4)
        t |> equal(c[0], 7.0)
    }
    t |> run("mutable-param pass-through control") @(t : T?) {
        var d : float[4]
        fill_zero(d)
        passthrough_mut(unsafe(addr(d[0])), 4)
        t |> equal(d[0], 7.0)
    }
    t |> run("write through let-alias of const? param") @(t : T?) {
        var e : float[4]
        fill_zero(e)
        passthrough_alias(unsafe(addr(e[0])), 4)
        t |> equal(e[0], 7.0)
    }
    t |> run("write through chained let-aliases") @(t : T?) {
        var g : float[4]
        fill_zero(g)
        passthrough_alias2(unsafe(addr(g[0])), 4)
        t |> equal(g[0], 7.0)
    }
    t |> run("write through var-copy of mutable param") @(t : T?) {
        var h : float[4]
        fill_zero(h)
        writer_via_copy(unsafe(addr(h[0])), 4)
        t |> equal(h[0], 7.0)
    }
}
