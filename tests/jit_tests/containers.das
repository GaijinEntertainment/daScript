options gen2

require dastest/testing_boost

def makeAll13(var a : array<int>) {
    for (v in a) {
        v = 13
    }
}

def addAll(var a : array<int>) {
    var c = 0
    for (v in a) {
        c += v
    }
    return c
}

def resizeLocked(var a : array<int>) {
    for (_a, i in a, count()) {
        if (i == 3) {
            a |> resize(20)
        }
    }
}

def outOfRange(var a : array<int>) => a[13]

def resizeUnreserved(var a : array<int>) {
    a |> resize(17000000)   // 68 MB: past the default 64 MB max_unreserved_size, no reserve
}

def check_array_float3(t : T?; a : array<float3>) {
    for (v, i in a, count()) {
        t |> equal(float3(i + 1, i + 2, i + 3), v)
    }
}

def check_dim_float3(t : T?; a : float3[3]) {
    for (v, i in a, count()) {
        t |> equal(float3(i + 1, i + 2, i + 3), v)
    }
}

[test(description = "JIT-compiled array and fixed-array access agrees with the interpreter, panics on out-of-range indices and on resizing a locked array")]
def test_array(t : T?) {
    t |> run("array float3") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@check_array_float3))
        let a <- array<float3>(
            float3(1, 2, 3),
            float3(2, 3, 4),
            float3(3, 4, 5)
        )
        check_array_float3(t, a)
    }
    t |> run("dim float3") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@check_dim_float3))
        let a <- fixed_array<float3>(
            float3(1, 2, 3),
            float3(2, 3, 4),
            float3(3, 4, 5)
        )
        check_dim_float3(t, a)
    }
    t |> run("array []") @(t : T?) {
        var x : array<int>
        resize(x, 10)
        t |> success(!jit_enabled() || is_jit_function(@@makeAll13))
        t |> success(!jit_enabled() || is_jit_function(@@addAll))
        makeAll13(x)
        for (i in x) {
            t |> equal(i, 13)
        }
        t |> equal(addAll(x), 130)
    }
    t |> run("out of range") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@outOfRange))
        var x : array<int>
        resize(x, 10)
        var failed = false
        try {
            let r = outOfRange(x)
            t |> success(r == 123456)// code never gets here
        } recover {
            failed = true
        }
        t |> success(failed, "out of range caused panic")
    }
    t |> run("resize locked") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@resizeLocked))
        var x : array<int>
        resize(x, 10)
        var failed = false
        try {
            resizeLocked(x)
        } recover {
            failed = true
        }
        t |> success(failed, "resize caused panic")
    }
    t |> run("unreserved huge resize") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@resizeUnreserved))
        var x : array<int>
        var failed = false
        try {
            resizeUnreserved(x)
        } recover {
            failed = true
        }
        t |> success(failed, "unreserved resize past max_unreserved_size caused panic")
    }
}

def push_range(n : int) : array<int> {
    var res : array<int>   // nolint:STYLE027 - the push loop IS the path under test
    for (i in range(n)) {
        res |> push(i * 3)   // nolint:PERF006 - growing from empty is the case under test
    }
    return <- res
}

def push_after_reserve(n : int) : int {
    var a : array<int>
    a |> reserve(n)
    for (i in range(n)) {
        a |> push(i)
    }
    var s = 0
    for (x in a) {
        s += x
    }
    delete a
    return s
}

def push_strings() : int {
    var a : array<string>   // nolint:STYLE012 - a run of pushes IS the path under test
    a |> push("x")
    a |> push("yy")
    a |> push("zzz")
    var n = 0
    for (s in a) {
        n += length(s)
    }
    delete a
    return n
}

def push_during_iteration(var a : array<int>) : bool {
    var threw = false
    try {
        for (_x in a) {
            a |> push(1)   // nolint:PERF006 - this push must throw, not grow
        }
    } recover {
        threw = true
    }
    return threw
}

[test(description = "push onto an array through the inline fast path grows across capacity steps, handles non-POD elements and still throws on a locked array")]
def test_array_push_fast(t : T?) {
    t |> run("grows from empty across capacity steps") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@push_range))
        var a <- push_range(100)
        t |> equal(length(a), 100)
        t |> equal(a[0], 0)
        t |> equal(a[99], 297)
        t |> success(capacity(a) >= 100)
        delete a
    }
    t |> run("reserved capacity takes the inline path") @(t : T?) {
        t |> equal(push_after_reserve(100), 4950)
    }
    t |> run("non-POD elements") @(t : T?) {
        t |> equal(push_strings(), 6)
    }
    t |> run("push into a locked array still throws") @(t : T?) {
        var a <- push_range(4)
        t |> success(push_during_iteration(a))
    }
}

def make_table {
    return <- {
        1 => "1",
        2 => "2",
        3 => "3"
    }
}

def test_table(var tab : table<int; string>) {
    unsafe {
        return length(tab) == 3 && tab[1] == "1" && tab[2] == "2" && tab[3] == "3"
    }
}

def make_table_jit {
    return <- {
        1 => "1",
        2 => "2",
        3 => "3"
    }
}

def test_table_jit(var tab : table<int; string>) {
    unsafe {
        return length(tab) == 3 && tab[1] == "1" && tab[2] == "2" && tab[3] == "3"
    }
}

def tab_erase(var tab : table<int; string>; key : int) {
    __builtin_table_erase(tab, key)
}

def tab_set_insert(var tab : table<int>; key : int) {
    __builtin_table_set_insert(tab, key)
}

def tab_find(var tab : table<int; string>; key : int) => __builtin_table_find(tab, key)

def tab_key_exist(var tab : table<int; string>; key : int) => __builtin_table_key_exists(tab, key)

[test(description = "JIT-compiled table construction, indexing, size, erase, insert, find and key_exists return the interpreter's results")]
def test_table(t : T?) {
    t |> run("make_table") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@make_table_jit))
        var tab <- make_table_jit()
        t |> success(test_table(tab))
    }
    t |> run("[], size") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@test_table_jit))
        var tab <- make_table()
        t |> success(test_table_jit(tab))
    }
    t |> run("erase") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@tab_erase))
        var tab <- {
            1 => "1",
            2 => "2",
            3 => "3"
        }
        tab_erase(tab, 2)
        t |> success(key_exists(tab, 1))
        t |> success(!key_exists(tab, 2))
        t |> success(key_exists(tab, 3))
    }
    t |> run("set insert") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@tab_set_insert))
        var tab <- { 1, 2, 4 }
        t |> success(key_exists(tab, 1))
        t |> success(key_exists(tab, 2))
        t |> success(!key_exists(tab, 3))
        t |> success(key_exists(tab, 4))
        tab_set_insert(tab, 3)
        t |> success(key_exists(tab, 1))
        t |> success(key_exists(tab, 2))
        t |> success(key_exists(tab, 3))
        t |> success(key_exists(tab, 4))
    }
    t |> run("find") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@tab_find))
        var tab <- {
            1 => "1",
            2 => "2",
            4 => "4"
        }
        t |> equal("1", *tab_find(tab, 1))
        t |> equal("2", *tab_find(tab, 2))
        t |> equal(null, tab_find(tab, 3))
        t |> equal("4", *tab_find(tab, 4))
    }
    t |> run("key exists") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@tab_key_exist))
        var tab <- {
            1 => "1",
            2 => "2",
            4 => "4"
        }
        t |> success(tab_key_exist(tab, 1))
        t |> success(tab_key_exist(tab, 2))
        t |> success(!tab_key_exist(tab, 3))
        t |> success(tab_key_exist(tab, 4))
    }
}

// Inline table slot-walk (keys/values for-loop sources over workhorse-keyed tables):
// the JIT scans the open-addressed ctrl bytes natively instead of calling the C++
// iterator per element. String keys keep the generic iterator path.

def kv_sums(t : table<int; int>) : tuple<n : int; ks : int; vs : int> {
    var n = 0
    var ks = 0
    var vs = 0
    for (k, v in keys(t), values(t)) {
        n++
        ks += k
        vs += v
    }
    return (n = n, ks = ks, vs = vs)
}

def keys_sum(t : table<int; int>) : int {
    var s = 0
    for (k in keys(t)) {
        s += k
    }
    return s
}

def values_bump(var t : table<int; int>) {
    for (v in values(t)) {
        v++
    }
}

def keys_break_at(t : table<int; int>; stop : int) : int {
    var n = 0
    for (_k in keys(t)) {
        n++
        break if (n == stop)
    }
    return n
}

def string_kv_sum(t : table<string; int>) : int {
    var s = 0
    for (_k, v in keys(t), values(t)) {
        s += v
    }
    return s
}

def keys64_sum(t : table<int64; int>) : int64 {
    var s = 0l
    for (k in keys(t)) {
        s += k
    }
    return s
}

def insert_during_iteration(var t : table<int; int>) : bool {
    var caught = false
    try {
        for (k in keys(t)) {
            t[100 + k] = 1
        }
    } recover {
        caught = true
    }
    return caught
}

[test(description = "JIT-compiled table walks visit keys and values correctly with tombstones, unlock on break, and panic on insert during locked iteration")]
def test_table_walk(t : T?) {
    t |> run("kv zip with tombstones") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@kv_sums))
        var tab <- {for (i in range(10)); i => i * 10}
        tab |> erase(3)
        tab |> erase(7)
        let r = kv_sums(tab)
        t |> equal(r.n, 8)
        t |> equal(r.ks, 35)
        t |> equal(r.vs, 350)
        delete tab
    }
    t |> run("keys-only walk") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@keys_sum))
        var tab <- {for (i in range(5)); i => i}
        t |> equal(keys_sum(tab), 10)
        var e : table<int; int>
        t |> equal(keys_sum(e), 0)
        delete tab
        delete e
    }
    t |> run("values walk mutates by ref") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@values_bump))
        var tab <- {for (i in range(4)); i => i * 10}
        values_bump(tab)
        let r = kv_sums(tab)
        t |> equal(r.vs, 60 + 4)
        delete tab
    }
    t |> run("break exits the walk and unlocks") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@keys_break_at))
        var tab <- {for (i in range(8)); i => i}
        t |> equal(keys_break_at(tab, 3), 3)
        tab[100] = 1   // table must be unlocked after the break
        t |> equal(length(tab), 9)
        delete tab
    }
    t |> run("string keys keep the generic path") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@string_kv_sum))
        var tab <- {"a" => 1, "b" => 2, "c" => 4}
        t |> equal(string_kv_sum(tab), 7)
        delete tab
    }
    t |> run("int64 keys ride the walk") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@keys64_sum))
        var tab <- {10l => 1, 20l => 2, 30l => 3}
        t |> equal(keys64_sum(tab), 60l)
        delete tab
    }
    t |> run("insert during locked iteration panics") @(t : T?) {
        // no delete: the panic leaves the table locked by design (panic is not resumable);
        // the context teardown reclaims it — same shape as array.das's locked-resize test
        var tab <- {for (i in range(5)); i => i}
        t |> success(insert_during_iteration(tab))
    }
}

let ROOM_SIZE_X = 32
let ROOM_SIZE_Y = 18
let ROOM_SIZE_Z = 5

var arr_xyz : int[ROOM_SIZE_X][ROOM_SIZE_Y][ROOM_SIZE_Z]
var arr_yzx : int[ROOM_SIZE_Y][ROOM_SIZE_Z][ROOM_SIZE_X]
var arr_xzy : int[ROOM_SIZE_X][ROOM_SIZE_Z][ROOM_SIZE_Y]

def walk {
    var pos = 0
    var arr : int[ROOM_SIZE_X + 2][ROOM_SIZE_Y + 2]
    for (y in 0 .. ROOM_SIZE_Y) {
        for (x in 0 .. ROOM_SIZE_X) {
            arr[x + 1][(ROOM_SIZE_Y - 1 - y) + 1] = pos++
        }
    }
    return pos
}

[jit, sideeffects]
def walk_xyz {
    for (x in range(ROOM_SIZE_X)) {
        for (y in range(ROOM_SIZE_Y)) {
            for (z in range(ROOM_SIZE_Z)) {
                arr_xyz[x][y][z] = 1
                arr_yzx[y][z][x] = 1
                arr_xzy[x][z][y] = 1
            }
        }
    }
    return true
}

var x : int[4][5]

[no_jit]
def init_no_jit {
    var val = 0
    for (i in range(4)) {
        for (j in range(5)) {
            x[i][j] = val++
        }
    }
}

def check_order_jit : bool {
    var val = 0
    for (i in range(4)) {
        for (j in range(5)) {
            return false if (x[i][j] != val++)
        }
    }
    return true
}

[test(description = "JIT-compiled multi-dimensional array access returns the interpreter's elements"), no_jit]
def test_dim(t : T?) {
    t |> run("multidim access") @(t : T?) {
        t |> success(!jit_enabled() || is_jit_function(@@ < () : bool > check_order_jit))
        init_no_jit()
        t |> success(check_order_jit())
        t |> equal(walk(),     576)
        t |> equal(walk_xyz(), true)
    }
}

def zero_through_reference(var a : uint[8]) {
    a = default<uint[8]>
}

[test(description = "assigning default<T[N]> zeroes every element of a fixed array held in a local, a var parameter, a tuple element and a nested dimension")]
def test_assigning_default_zeroes_the_whole_fixed_array(t : T?) {
    t |> run("a local") @(t : T?) {
        var a = fixed_array<int>(1, 2, 3, 4)
        a = default<int[4]>
        t |> equal(a[0], 0)
        t |> equal(a[1], 0, "every element, not only the first")
        t |> equal(a[3], 0)
    }
    t |> run("through a var parameter") @(t : T?) {
        var a : uint[8]
        for (w in a) {
            w = 0xffffffffu
        }
        zero_through_reference(a)
        t |> equal(a[0], 0u)
        t |> equal(a[7], 0u, "the last element too")
    }
    t |> run("a tuple element and a nested dimension") @(t : T?) {
        var ts : tuple<int; float>[2]
        ts[1]._0 = 3
        ts = default<tuple<int; float>[2]>
        t |> equal(ts[1]._0, 0)
        var ss : int[2][3]
        ss[1][2] = 9
        ss = default<int[2][3]>
        t |> equal(ss[1][2], 0)
    }
    t |> run("initializing from default is the control") @(t : T?) {
        let a = default<int[4]>
        t |> equal(a[3], 0)
    }
}

// a make-array literal whose elements are all numeric or 2-/4-wide vector constants lowers to constant
// data and one memcpy in the JIT; every element width, the 3-wide vector the lowering excludes, a literal
// under the four-element floor and one with a runtime element read back element for element

def sum_i8(a : int8[5]) : int {
    var s = 0
    for (v in a) {
        s += int(v)
    }
    return s
}

def sum_i16(a : int16[5]) : int {
    var s = 0
    for (v in a) {
        s += int(v)
    }
    return s
}

def sum_f4(a : float4[4]) : float {
    var s = 0.0
    for (v in a) {
        s += v.x + v.y + v.z + v.w
    }
    return s
}

def sum_f3(a : float3[4]) : float {
    var s = 0.0
    for (v in a) {
        s += v.x + v.y + v.z
    }
    return s
}

def sum_d(a : double[4]) : double {
    var s = 0.0lf
    for (v in a) {
        s += v
    }
    return s
}

def sum_i(a : int[3]) : int {
    var s = 0
    for (v in a) {
        s += v
    }
    return s
}

def sum_i4(a : int[4]) : int {
    var s = 0
    for (v in a) {
        s += v
    }
    return s
}

[test(description = "fixed_array literals of constant 1, 2, 4, 8 and 16-byte elements read back every element, and a literal with a runtime element among constants still works")]
def test_const_data_literal(t : T?) {
    t |> run("one-byte and two-byte elements") @(t : T?) {
        let bytes = fixed_array<int8>(int8(1), int8(-2), int8(3), int8(-4), int8(5))
        t |> equal(3, sum_i8(bytes))
        t |> equal(int8(-4), bytes[3])
        let halves = fixed_array<int16>(int16(1000), int16(-2000), int16(3000), int16(-4000), int16(5000))
        t |> equal(3000, sum_i16(halves))
        t |> equal(int16(5000), halves[4])
    }
    t |> run("16-byte vector elements, and the 3-wide vector the lowering excludes") @(t : T?) {
        let quads = fixed_array<float4>(float4(1, 2, 3, 4), float4(5, 6, 7, 8), float4(9, 10, 11, 12), float4(13, 14, 15, 16))
        t |> equal(136.0, sum_f4(quads))
        t |> equal(float4(13, 14, 15, 16), quads[3])
        let triples = fixed_array<float3>(float3(1, 2, 3), float3(4, 5, 6), float3(7, 8, 9), float3(10, 11, 12))
        t |> equal(78.0, sum_f3(triples))
        t |> equal(float3(10, 11, 12), triples[3])
    }
    t |> run("eight-byte elements") @(t : T?) {
        let wide = fixed_array<double>(1.5lf, -2.5lf, 3.5lf, -4.5lf)
        t |> equal(-2.0lf, sum_d(wide))
        t |> equal(-4.5lf, wide[3])
    }
    t |> run("under the floor, and a runtime element among constants") @(t : T?) {
        let three = fixed_array<int>(10, 20, 30)
        t |> equal(60, sum_i(three))
        let n = length(three)
        let mixed = fixed_array<int>(1, 2, n, 4)
        t |> equal(10, sum_i4(mixed))
        t |> equal(3, mixed[2])
    }
}
