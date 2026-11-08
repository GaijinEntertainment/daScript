options gen2

require dastest/testing_boost public

struct ThreeInts {
    a : int
    b : int
    c : int
}

// typeinfo leaves this dimension unresolved in the first pass
let UNRESOLVED_DIM = typeinfo sizeof(type<ThreeInts>) / 4

variant HeapVariant {
    p : int
    q : array<int>
}

variant PodVariant {
    i : int
    f : float
}

def operator :=(var a : PodVariant; _b : PodVariant) {
    a = PodVariant(i = 42)
}

[test(description = "a user clone operator on a variant is called for clone between inferred locals")]
def test_user_clone_operator_with_inferred_locals(t : T?) {
    let src = PodVariant(f = 1.5)
    var dst = PodVariant(i = 2)
    dst := src
    t |> success(dst is i)
    t |> equal(dst as i, 42)
}

struct IndexTarget {
    assign_hit : int
    clone_hit : int
    add_hit : int
    move_hit : int
}

def operator [] =(var self : IndexTarget; _i : int; _v : int[3]) {
    self.assign_hit = 1
}

def operator [] =(var self : IndexTarget; _i : int; _v : auto) {
    self.assign_hit = 2
}

def operator [] :=(var self : IndexTarget; _i : int; _v : int[3]) {
    self.clone_hit = 1
}

def operator [] :=(var self : IndexTarget; _i : int; _v : auto) {
    self.clone_hit = 2
}

def operator [] +=(var self : IndexTarget; _i : int; _v : int[3]) {
    self.add_hit = 1
}

def operator [] +=(var self : IndexTarget; _i : int; _v : auto) {
    self.add_hit = 2
}

def operator [] <-(var self : IndexTarget; _i : int; var _v : int[3]) {
    self.move_hit = 1
}

def operator [] <-(var self : IndexTarget; _i : int; var _v : auto) {
    self.move_hit = 2
}

def index_operators_on_unresolved_dim(var a : int[UNRESOLVED_DIM]) : IndexTarget {
    var target : IndexTarget
    target[0] = a
    target[0] := a
    target[0] += a
    target[0] <- a
    return target
}

[test(description = "index-assign operators whose value is a fixed array of an unresolved dimension bind the concrete int[3] overload, not the auto one")]
def test_index_operators_bind_the_concrete_overload(t : T?) {
    var x : int[3]
    let target = index_operators_on_unresolved_dim(x)
    t |> equal(target.assign_hit, 1)
    t |> equal(target.clone_hit, 1)
    t |> equal(target.add_hit, 1)
    t |> equal(target.move_hit, 1)
}

var g_index_self_hit = 0

def operator [] =(var _self : int[3]; _i : int; _v : int) {
    g_index_self_hit = 1
}

def operator [] =(var _self : auto; _i : int; _v : int) {
    g_index_self_hit = 2
}

def index_assign_into_unresolved_dim(var a : int[UNRESOLVED_DIM]) {
    a[0] = 1
}

[test(description = "an index assignment into a fixed array of an unresolved dimension binds the concrete int[3] self overload")]
def test_index_assign_on_unresolved_self_binds_the_concrete_overload(t : T?) {
    var x : int[3]
    index_assign_into_unresolved_dim(x)
    t |> equal(g_index_self_hit, 1)
}

struct IndexSink {
    hit : int
}

def operator [] =(var self : IndexSink; _i : int[3]; _v : int) {
    self.hit = 1
}

def operator [] =(var self : IndexSink; _i : auto; _v : int) {
    self.hit = 2
}

def index_assign_with_unresolved_index(a : int[UNRESOLVED_DIM]) : int {
    var sink : IndexSink
    sink[a] = 1
    return sink.hit
}

[test(description = "an index assignment whose index is a fixed array of an unresolved dimension binds the concrete int[3] index overload")]
def test_index_assign_with_unresolved_index_binds_the_concrete_overload(t : T?) {
    let x : int[3] = fixed_array(1, 2, 3)
    t |> equal(index_assign_with_unresolved_index(x), 1)
}

def is_three_ints(a : int[UNRESOLVED_DIM]) : bool => a is type<int[3]> && a[0] == 1

[test(description = "is type<int[3]> on a fixed array declared with an unresolved dimension answers true once the dimension resolves")]
def test_is_type_on_unresolved_dim(t : T?) {
    let x : int[3] = fixed_array(1, 2, 3)
    t |> success(is_three_ints(x))
}

typedef HeapTargetFn = function<(v : HeapVariant) : int>

def heap_target(_v : HeapVariant) : int => 1

def heap_target_impostor(_v : HeapVariant) : int => 2

def call_with_inferred_local() : int {
    var heap_target : HeapTargetFn = @@heap_target_impostor  // nolint:LINT002 - must not bind
    let src = HeapVariant(p = 1)
    return heap_target(src)
}

class HeapTargetHolder {
    heap_target : HeapTargetFn = @@heap_target_impostor
    def call_with_inferred_local() : int {
        let src = HeapVariant(p = 1)
        return heap_target(src)
    }
}

struct WithInferredField {
    v = HeapVariant(p = 1)
}

def call_with_inferred_field(s : WithInferredField) : int {
    var heap_target : HeapTargetFn = @@heap_target_impostor  // nolint:LINT002 - must not bind
    return heap_target(s.v)
}

struct VariantMaker {
    x : int
}

def call_with_method_result() : int {
    var heap_target : HeapTargetFn = @@heap_target_impostor  // nolint:LINT002 - must not bind
    let maker = VariantMaker(x = 1)
    return heap_target(maker.make_variant())
}

def make_variant(maker : VariantMaker) => <- HeapVariant(p = maker.x)

typedef HeapPair = tuple<a : int; v : HeapVariant>

def HeapPair(a : int; _v : HeapVariant) : HeapPair => <- (a = a + 100, v = HeapVariant(p = 7))

[test(description = "a call whose argument type is still being inferred - a local, a class member, a field, a method result - binds the intended function, not a same-signature impostor")]
def test_call_with_inferred_argument_binds_the_function(t : T?) {
    t |> equal(call_with_inferred_local(), 1)
    var holder = new HeapTargetHolder()
    t |> equal(holder->call_with_inferred_local(), 1)
    unsafe {
        delete holder
    }
    t |> equal(call_with_inferred_field(default<WithInferredField>), 1)
    t |> equal(call_with_method_result(), 1)
    let src = HeapVariant(p = 1)
    let pair <- HeapPair(1, src)
    t |> equal(pair.a, 101)
}

def pass_through(var x : auto(TT)) : TT => <- x

struct ZeroTarget {
    x : int
}

def memzero(var s : ZeroTarget; _v : HeapVariant) {
    s.x = 42
}

[test(description = "a dot method call whose argument comes through nested generic pass-throughs binds the user function over the builtin of the same name")]
def test_method_call_with_inferred_argument_binds_the_function(t : T?) {
    var s = ZeroTarget(x = 5)
    let src <- pass_through(pass_through(pass_through(HeapVariant(p = 1))))
    s.memzero(src)
    t |> equal(s.x, 42)
}

var g_receiver_hit = 0

def memzero(var _v : HeapVariant) {
    g_receiver_hit = 42
}

[test(description = "a dot method call on a receiver inferred through nested generic pass-throughs binds the user function and leaves the receiver intact")]
def test_method_call_on_inferred_receiver_binds_the_function(t : T?) {
    var src <- pass_through(pass_through(pass_through(HeapVariant(p = 1))))
    src.memzero()
    t |> equal(g_receiver_hit, 42)
    t |> success(src is p)
}

struct WithUnresolvedDim {
    b : int[UNRESOLVED_DIM]
    x : int
}

[test(description = "typeinfo offsetof of a field after a fixed array with an unresolved dimension counts the resolved size")]
def test_offsetof_after_unresolved_dim(t : T?) {
    t |> equal(typeinfo offsetof < x > (type<WithUnresolvedDim>), 12)
}

[test(description = "pointer + 1, ++ and pointer difference over a struct holding an unresolved-dimension array stride by the resolved struct size")]
def test_pointer_stride_over_unresolved_dim(t : T?) {
    var arr : WithUnresolvedDim[4]
    unsafe {
        var p = addr(arr[0])
        var q = p + 1
        var r = p
        r++
        t |> equal(intptr(q) - intptr(p), uint64(typeinfo sizeof(type<WithUnresolvedDim>)))
        t |> equal(intptr(r) - intptr(p), uint64(typeinfo sizeof(type<WithUnresolvedDim>)))
        t |> equal(q - p, 1l)
    }
}

variant StrideVariant {
    p : int
    w : int4[8]
}

struct WithInferredFieldType {
    v = StrideVariant(p = 1)
    x : int
}

[test(description = "pointer arithmetic over a struct whose field type is inferred strides by the struct's final size")]
def test_pointer_stride_over_inferred_field_type(t : T?) {
    unsafe {
        var arr : WithInferredFieldType[4]
        var p = addr(arr[0])
        var q = p + 1
        var r = p
        r++
        t |> equal(intptr(q) - intptr(p), uint64(typeinfo sizeof(type<WithInferredFieldType>)))
        t |> equal(intptr(r) - intptr(p), uint64(typeinfo sizeof(type<WithInferredFieldType>)))
        t |> equal(q - p, 1l)
    }
}

def ptr_diff(a, b : WithInferredFieldType?) : int64 => a - b

[test(description = "the difference of two pointers to a struct with an inferred field type counts elements, not bytes")]
def test_pointer_diff_over_inferred_field_type(t : T?) {
    unsafe {
        var arr : WithInferredFieldType[4]
        t |> equal(ptr_diff(addr(arr[1]), addr(arr[0])), 1l)
    }
}

struct SelfLinked {
    next : SelfLinked?
    x : int
}

[test(description = "pointer + 1 over a self-referential struct strides by the struct's size")]
def test_pointer_stride_over_self_referential_struct(t : T?) {
    unsafe {
        var arr : SelfLinked[4]
        var p = addr(arr[0])
        var q = p + 1
        t |> equal(intptr(q) - intptr(p), uint64(typeinfo sizeof(type<SelfLinked>)))
    }
}

typedef InferredFieldPair = WithInferredFieldType[2]

[test(description = "pointer + 1 over a fixed array of structs with an inferred field type strides by the whole array's size")]
def test_pointer_stride_over_array_of_inferred_struct(t : T?) {
    unsafe {
        var arr : InferredFieldPair[4]
        var p = addr(arr[0])
        var q = p + 1
        t |> equal(intptr(q) - intptr(p), uint64(typeinfo sizeof(type<InferredFieldPair>)))
    }
}

def count_down(n : int) {
    if (n > 0) {
        let x = count_down(n - 1)
        return x + 1
    }
    return 0
}

[test(description = "a recursive function whose result type is read through a local still pending inference infers and returns the right count")]
def test_recursive_result_read_through_a_pending_local(t : T?) {
    t |> equal(count_down(3), 3)
}
