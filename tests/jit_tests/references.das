options gen2

require dastest/testing_boost
require daslib/constant_expression

struct Foo {
    a : int
    b : float
}

def move_jit(var a, b) {
    a <- b
}

[test(description = "the move operator in JIT-compiled code leaves the source and destination as the interpreter does")]
def test_move(t : T?) {
    t |> run("move") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@ < (var a, b : Foo) : void > move_jit))
        var a : Foo
        var b = Foo(a = 1, b = 2.0)
        move_jit(a, b)
        t |> equal(1, a.a)
        t |> equal(2.0, a.b)
        t |> equal(0, b.a)
        t |> equal(0.0, b.b)
    }
}

var g_moved = 5

def move_global => (<- g_moved)

def move_through_ref(var a : int&) : int& {
    unsafe {
        return <- a
    }
}

[test(description = "a moved global read through a one-element tuple and a reference return under move semantics keep the argument's address")]
def test_move_reference_return(t : T?) {
    t |> run("a moved global through a one-element tuple") @(t : T?) {
        t |> equal(move_global(), 5)
    }
    t |> run("a reference return under move semantics is the argument's address") @(t : T?) {
        var x = 7
        move_through_ref(x) = 9
        t |> equal(x, 9)
        t |> success(!jit_enabled() || is_jit_function(@@move_through_ref))
    }
}

def inc_local_ref(var a : int&) {
    unsafe {
        var b & = a
        return b++
    }
}

def inc_arg_ref(var a : int&) => a++

var global_ref = 13

def get_global_ref : int& {
    unsafe {
        return global_ref
    }
}

[test(description = "JIT-compiled local, argument and global references read and write the same storage as the interpreter")]
def test_ref(t : T?) {
    t |> run("local ref") @(t : T?) {
        var a = 1
        let b = inc_local_ref(a)
        t |> equal(a, 2)
        t |> equal(b, 1)
        t |> success(!jit_enabled() || is_jit_function(@@inc_local_ref))
    }
    t |> run("arg ref") @(t : T?) {
        var a = 1
        let b = inc_arg_ref(a)
        t |> equal(a, 2)
        t |> equal(b, 1)
        t |> success(!jit_enabled() || is_jit_function(@@inc_arg_ref))
    }
    t |> run("global ref") @(t : T?) {
        t |> equal(13, global_ref)
        t |> equal(13, get_global_ref())
        get_global_ref() ++
        t |> equal(14, global_ref)
        t |> equal(14, get_global_ref())
        global_ref ++
        t |> equal(15, global_ref)
        t |> equal(15, get_global_ref())
        t |> success(!jit_enabled() || is_jit_function(@@get_global_ref))
    }
}

def get_back(a : array<int?>) : int? const& {
    let n = length(a)
    unsafe {
        return a[n - 1]
    }
}

[test(description = "a JIT-compiled function returning a reference through an array pointer returns the interpreter's value")]
def test_ptr_ref_return(t : T?) {
    t |> run("array ptr ref") @(t : T?) {
        var x = 42
        var ptrs : array<int?>
        var p = unsafe(addr(x))
        ptrs |> push(p)
        let q = get_back(ptrs)
        t |> equal(q, p)
        t |> success(!jit_enabled() || is_jit_function(@@get_back))
    }
}

def make13(var a : int?) {
    *a = 13
    return a
}

def makeAll13(var a : int?; count : int) {
    for (i in range(count)) {
        unsafe(a[i]) = 13
    }
}

def addAll(var a : int?; count : int) {
    var c = 0
    for (i in range(count)) {
        c += unsafe(a[i])
    }
    return c
}

def ptrAdd(var a : int?; count : int) {
    var c = 0
    for (_i in range(count)) {
        c += *a
        unsafe { ++ a }   // nolint:STYLE025 - unsafe(++ a) does not authorize the lowered i_das_ptr_inc call
    }
    return c
}

def ptrSub(var a : int?; count : int) {
    var c = 0
    for (_i in range(count)) {
        c += *a
        unsafe { -- a }   // nolint:STYLE025 - unsafe(-- a) does not authorize the lowered i_das_ptr_dec call
    }
    return c
}

def ptrIncPostfix(var a : uint8?; count : int) {
    var c = 0
    for (_i in range(count)) {
        c += int(*a)
        unsafe { a++ }   // nolint:STYLE025 - unsafe(a++) does not authorize the lowered i_das_ptr_inc call
    }
    return c
}

[test(description = "JIT-compiled pointer to reference conversion, indexing and ++/-- walk the element stride like the interpreter")]
def test_pointer_ref(t : T?) {
    t |> run("ptr2ref") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@make13))
        var x = -21
        var px = unsafe(addr(x))
        var ppx = make13(px)
        t |> equal(x, 13)
        t |> equal(px, ppx)
        t |> equal(*px, 13)
    }
    t |> run("ptr []") @(t : T?) {
        var x : int[10]
        var px = unsafe(addr(x[0]))
        t |> success(!jit_enabled() || is_jit_function(@@makeAll13))
        t |> success(!jit_enabled() || is_jit_function(@@addAll))
        makeAll13(px, 10)
        for (i in x) {
            t |> equal(i, 13)
        }
        t |> equal(addAll(px, 10), 130)
    }
    t |> run("ptr ++") @(t : T?) {
        var x : int[10]
        var px = unsafe(addr(x[0]))
        for (i, j in x, range(10)) {
            i = j
        }
        t |> success(!jit_enabled() || is_jit_function(@@addAll))
        t |> equal(addAll(px, 10), 45)
    }
    t |> run("ptr ++ and -- walk the element stride") @(t : T?) {
        var x : int[10]
        for (i, j in x, range(10)) {
            i = j
        }
        var px = unsafe(addr(x[0]))
        var plast = unsafe(addr(x[9]))
        t |> success(!jit_enabled() || is_jit_function(@@ptrAdd))
        t |> success(!jit_enabled() || is_jit_function(@@ptrSub))
        t |> equal(ptrAdd(px, 10), 45)
        t |> equal(ptrSub(plast, 10), 45)
        t |> equal(ptrAdd(px, 3), 3)
        t |> equal(ptrSub(plast, 3), 24)
        var bytes : uint8[8]
        for (b, j in bytes, range(8)) {
            b = uint8(j * 3)
        }
        var pb = unsafe(addr(bytes[0]))
        t |> success(!jit_enabled() || is_jit_function(@@ptrIncPostfix))
        t |> equal(ptrIncPostfix(pb, 8), 84)
    }
}

def fnmad(a, b, c : int) => a * b + c

[sideeffects]
def jit_addr => @@fnmad

[sideeffects]
def jit_pass_addr => take_fun(@@fnmad)

def take_fun(fn : function<(a, b, c : int) : int>) => invoke(fn, 1, 2, 3)

[test(description = "JIT-compiled addr and pass-through of a function address invoke to the right function and return the interpreter's value")]
def test_addr_of_function(t : T?) {
    t |> success(!jit_enabled() || is_jit_function(@@jit_addr))
    t |> success(!jit_enabled() || is_jit_function(@@jit_pass_addr))
    let res = invoke(jit_addr(), 1, 2, 3)
    t |> equal(5, res)
    let res_t = jit_pass_addr()
    t |> equal(5, res_t)
}

def low_bits(p : uint8?) : uint => unsafe(reinterpret<uint>(p))

def all_bits(p : uint8?) : uint64 => unsafe(reinterpret<uint64>(p))

[test(description = "reinterpreting a pointer to a 32-bit integer yields the low bits of its 64-bit value")]
def test_a_pointer_reinterpreted_to_a_32_bit_integer_is_its_low_bits(t : T?) {
    var data : uint8[64]
    let p = unsafe(addr(data[0]))
    let q : uint8? = unsafe(p + 4)
    t |> equal(all_bits(q) - all_bits(p), 4ul)
    t |> equal(low_bits(q) - low_bits(p), 4u)
    t |> equal(low_bits(p), uint(all_bits(p) & 0xfffffffful))
}

// Exported wrappers keep every length spelling observable in generated IR.
[export] def copy_i32(var dst : void ?; src : void ?; size : int){unsafe(memcpy(dst, src, size))}
[export] def copy_u32(var dst : void ?; src : void ?; size : uint){unsafe(memcpy(dst, src, size))}
[export] def copy_i64(var dst : void ?; src : void ?; size : int64){unsafe(memcpy(dst, src, size))}
[export] def copy_u64(var dst : void ?; src : void ?; size : uint64){unsafe(memcpy(dst, src, size))}
[export] def move_i32(var dst : void ?; src : void ?; size : int){unsafe(memmove(dst, src, size))}
[export] def move_u32(var dst : void ?; src : void ?; size : uint){unsafe(memmove(dst, src, size))}
[export] def move_i64(var dst : void ?; src : void ?; size : int64){unsafe(memmove(dst, src, size))}
[export] def move_u64(var dst : void ?; src : void ?; size : uint64){unsafe(memmove(dst, src, size))}

def private check_transfers(t : T?; copy, move : auto(FT); zero : auto(N)){
    t |> success(!jit_enabled() || is_jit_function(copy))
    t |> success(!jit_enabled() || is_jit_function(move))
    invoke(copy, null, null, zero); invoke(move, null, null, zero)
    for (size in fixed_array(0, 1, 3, 7, 31, 63, 129)){
        var source, destination : uint8[160]
        for (i in range(160)){source[i] = uint8(i); destination[i] = 0xa5u8}
        invoke(copy, unsafe(addr(destination[3])), unsafe(addr(source[1])), N(size))
        for (i in range(160)){t |> equal(destination[i], i >= 3 && i < size + 3 ? uint8(i - 2) : 0xa5u8); t |> equal(source[i], uint8(i))}
        invoke(move, unsafe(addr(source[3])), unsafe(addr(source[0])), N(size))
        for (i in range(160)){t |> equal(source[i], uint8(i >= 3 && i < size + 3 ? i - 3 : i))}
        for (i in range(160)){source[i] = uint8(i)}
        invoke(move, unsafe(addr(source[0])), unsafe(addr(source[3])), N(size))
        for (i in range(160)){t |> equal(source[i], uint8(i < size ? i + 3 : i))}
        invoke(move, unsafe(addr(source[1])), unsafe(addr(source[1])), N(size))
        for (i in range(160)){t |> equal(source[i], uint8(i < size ? i + 3 : i))}
    }
}
[test(description = "JIT-compiled copy and move of 32 and 64-bit signed and unsigned values transfer the value")]
def memory_copy_all_size_types(t : T?){
    check_transfers(t, @@copy_i32, @@move_i32, 0)
    check_transfers(t, @@copy_u32, @@move_u32, 0u)
    check_transfers(t, @@copy_i64, @@move_i64, 0l)
    check_transfers(t, @@copy_u64, @@move_u64, 0ul)
}

def jit_memset8(var data : void?; value : uint8; size : int) {
    unsafe(memset8(data, value, size))
}

def jit_memset16(var data : void?; value : uint16; size : int) {
    unsafe(memset16(data, value, size))
}

def jit_memset32(var data : void?; value : uint; size : int) {
    unsafe(memset32(data, value, size))
}

def jit_memset64(var data : void?; value : uint64; size : int) {
    unsafe(memset64(data, value, size))
}

def jit_memset128(var data : void?; value : uint4; size : int) {
    unsafe(memset128(data, value, size))
}

def jit_memmove(var dst : void?; src : void?; size : int) {
    unsafe(memmove(dst, src, size))
}

[constant_expression(funcname), sideeffects]
def test_any_memset(t : T?; value : auto(TT); funcname : string) {
    for (i in 0..128) {
        for (j in i..128) {
            let count = j - i
            var arr : TT[128]
            unsafe {
                invoke(funcname, addr(arr[i]), value, count)
            }
            for (V in 0..128) {
                if (V >= i && V < j) {
                    t |> equal(value, arr[V])
                } else {
                    t |> equal(TT(), arr[V])
                }
            }
        }
    }
}

[test(description = "JIT-compiled memset of 8, 16, 32, 64 and 128-bit elements fills every element with the pattern")]
def test_memset(t : T?) {
    t |> run("memset8") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@jit_memset8))
        var a : uint8[10]
        jit_memset8(unsafe(addr(a[2])), 0x55u8, 3)
        for (i in 0..10) {
            t |> equal((i >= 2 && i < 5) ? 0x55u8 : 0u8, a[i])
        }
        test_any_memset(t, 12u8, "jit_memset8")
    }
    t |> run("memset16") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@jit_memset16))
        var a : uint16[10]
        jit_memset16(unsafe(addr(a[2])), uint16(0x55), 3)
        for (i in 0..10) {
            t |> equal((i >= 2 && i < 5) ? uint16(0x55) : uint16(0), a[i])
        }
        test_any_memset(t, uint16(1234), "jit_memset16")
    }
    t |> run("memset32") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@jit_memset32))
        var a : uint[10]
        jit_memset32(unsafe(addr(a[2])), 0x55, 3)
        for (i in 0..10) {
            t |> equal((i >= 2 && i < 5) ? 0x55 : 0x00, a[i])
        }
        test_any_memset(t, 0x12345678, "jit_memset32")
    }
    t |> run("memset64") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@jit_memset64))
        var a : uint64[10]
        jit_memset64(unsafe(addr(a[2])), uint64(0x55), 3)
        for (i in 0..10) {
            t |> equal((i >= 2 && i < 5) ? uint64(0x55) : uint64(0), a[i])
        }
        test_any_memset(t, 0x12345678abcdef01ul, "jit_memset64")
    }
    t |> run("memset128") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@jit_memset128))
        var a : uint4[10]
        jit_memset128(unsafe(addr(a[2])), uint4(0x12, 0x34, 0x56, 0x78), 3)
        for (i in 0..10) {
            t |> equal((i >= 2 && i < 5) ? uint4(0x12, 0x34, 0x56, 0x78) : uint4(0), a[i])
        }
        test_any_memset(t, uint4(0x12345678, 0x9abcdef0, 0x1ee7c00d, 0xdeadbeef), "jit_memset128")
    }
}

[test(description = "an overlapping memmove shift in JIT-compiled code produces the correctly shifted contents")]
def test_memmove(t : T?) {
    t |> run("memmove overlapping shift") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@jit_memmove))
        var a : uint8[10]
        for (i in 0..10) {
            a[i] = uint8(i)
        }
        jit_memmove(unsafe(addr(a[2])), unsafe(addr(a[0])), 6)
        for (i in 0..10) {
            t |> equal(uint8(i >= 2 && i < 8 ? i - 2 : i), a[i])
        }
        jit_memmove(unsafe(addr(a[0])), unsafe(addr(a[2])), 6)
        for (i in 0..6) {
            t |> equal(uint8(i), a[i])
        }
    }
}

def to_byte(v : bool) : uint8 => unsafe(reinterpret<uint8>(v))

def to_bool(v : uint8) : bool => unsafe(reinterpret<bool>(v))

def int_to_bool(v : int) : bool => unsafe(reinterpret<bool>(v))

var g_byte_source = 0u8
var g_int_source = 0

def store(var x : auto(TT)&; v : TT) {
    unsafe {
        memset8(addr<uint8?>(x), reinterpret<uint8>(v), 1)
    }
}

[test(description = "reinterpreting a bool to uint8 and back round-trips 0 and 1 and stores a byte, not a wider word")]
def test_a_bool_reinterpreted_to_uint8_is_its_byte(t : T?) {
    t |> equal(1, int(to_byte(true)))
    t |> equal(0, int(to_byte(false)))
    t |> equal(true, to_bool(1u8))
    t |> equal(false, to_bool(0u8))
    var b = false
    store(b, true)
    t |> equal(true, b)
    var u = 0u8
    store(u, 7u8)
    t |> equal(7, int(u))
}

//! a bool's truth by branching on it, not by comparing its byte
def truth(b : bool) : int => b ? 1 : 0

//! a bool byte other than 0 or 1 is defined only under the JIT, which
//! compares the low byte with zero; the interpreter and AOT read it
//! as a C++ bool
[test(description = "reinterpreting a runtime integer to bool reads its low byte, so 256 is false and 1 is true")]
def test_a_runtime_integer_reinterpreted_to_bool_reads_its_low_byte(t : T?) {
    g_int_source = 256
    t |> equal(0, truth(int_to_bool(g_int_source)))
    g_int_source = 1
    t |> equal(1, truth(int_to_bool(g_int_source)))
    if (jit_enabled()) {
        g_byte_source = 2u8
        t |> equal(1, truth(to_bool(g_byte_source)))
        g_int_source = 2
        t |> equal(1, truth(int_to_bool(g_int_source)))
    }
}

[hint(noinline)]
def via_void(var p, q : void?) : float {
    unsafe {
        var f = reinterpret<float?>(p)
        var u = reinterpret<uint?>(q)
        *f = 2.0f
        *u = 0x3f800000u
        return *f
    }
}

[hint(noinline)]
def via_float(var f, q : float?) : float {
    var u = unsafe(reinterpret<uint?>(q))
    *f = 2.0f
    *u = 0x3f800000u
    return *f
}

[test(description = "two typed views of one float taken through void? parameters alias: the write through the uint view is seen by the read through the float view, as through typed source pointers")]
def test_a_view_taken_through_void_is_a_pun(t : T?) {
    var x = 0.0f
    var px = unsafe(addr(x))
    t |> equal(1.0f, via_float(px, px), "typed source pointer")
    t |> equal(1.0f, via_void(px, px), "void? source pointer")
}
