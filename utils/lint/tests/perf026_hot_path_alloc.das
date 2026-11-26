//! PERF026 is reported for: heap traffic on a hot path.
options gen2
options auto_inline_functions = false   // lint fixtures assert SOURCE shapes; splices rewrite them
// PERF026: heap traffic on a hot path
//
// Problem:
//   Allocating or freeing inside a per-frame / per-token loop is the classic
//   hidden cost — an array resize in the middle of a decode step, a table
//   insert per layer. The cost is invisible at the call site because the
//   allocation happens several calls deep, inside daslib.
//
// Bad pattern:
//   [hot_path]
//   def step(var s : Session) {
//       s.stages |> resize(4)
//   }
//
// Good pattern:
//   size the buffer once at setup; declare a reused buffer @scratch at its
//   field (or at the parameter of a helper that sizes one by reference); or
//   put a genuinely one-time growth behind a [cold_path] helper.
//
// The rule is annotation-gated: nothing is checked until a function carries
// [hot_path] (all three contracts) or the narrow [no_alloc] / [no_env] /
// [no_io]. From each root the scan follows DIRECT calls transitively, so a
// sink several frames deep is still reported — anchored on the deepest line
// written in the root's own file, not on the daslib internal that allocates.

expect 31208:34

require daslib/perf_lint
require strings

struct Blob {
    v : int
}

// A reused buffer says so once, at the declaration. Sizing it to the current step's geometry is
// the owner's allocation strategy, not an accident at each call site — and unlike a nolint per
// line, the statement survives the code moving.
struct Scratch {
    @scratch reused : array<float>
    fresh : array<float>
}

// 1. allocation written directly in the annotated function
[no_alloc]
def bad_direct(var a : array<int>) {
    a |> push(1)
}

// 2. allocation reached through an ordinary (unannotated) helper — the helper is
//    scanned because it is reachable, and the warning lands on the push line
def alloc_helper(var a : array<int>) {
    a |> reserve(64)
}

[no_alloc]
def bad_transitive(var a : array<int>) {
    alloc_helper(a)
}

// 3. [cold_path] prunes the walk: one-time setup reached from a hot function is fine
[cold_path]
def cold_init(var a : array<int>) {
    a |> reserve(1024)
}

[no_alloc]
def good_cold(var a : array<int>) {
    cold_init(a)
}

// 4. per-line suppression still works
[no_alloc]
def good_nolint(var a : array<int>) {
    a |> push(2)   // nolint:PERF026
}

// 5. an unannotated function is never scanned
def good_unannotated(var a : array<int>) {
    a |> push(3)
}

// 6. a @scratch destination is declared intent and stays silent; its unmarked neighbour does not
[no_alloc]
def bad_only_the_unmarked_buffer(var sc : Scratch; n : int) {
    sc.reused |> resize(n)
    sc.fresh |> resize(n)
}

// 7. a helper that sizes a caller's buffer BY REFERENCE marks the parameter — the call site
//    cannot see which field arrived
def ensure_room(@scratch var a : array<float>; n : int) {
    a |> resize(n)
}

[no_alloc]
def good_scratch_helper(var sc : Scratch; n : int) {
    ensure_room(sc.reused, n)
}

// 8. new and delete are heap traffic too
[no_alloc]
def bad_new_delete() : int {
    var p = new Blob(v = 7)
    let v = p.v
    unsafe {
        delete p
    }
    return v
}

// 9. a @scratch module GLOBAL is declared intent too — the capture-rail shape: a
//    clear()-recycled array that grows once and is reused; its unmarked neighbour still reports
var @scratch g_reused : array<int>
var g_fresh : array<int>

[no_alloc]
def bad_only_the_unmarked_global() {
    g_reused |> push(1)
    g_reused |> pop()   // pop rides the escape too (shrink of a reused buffer is a store)
    g_fresh |> push(2)
}

// 10. indexing a @scratch TABLE is the declared pool / residency-cache shape (insert-on-first-
//     touch, steady-state lookup); an unmarked table's [] still reports its insert-on-read
var @scratch g_pool : table<int; int>
var g_plain : table<int; int>

[no_alloc]
def bad_only_the_unmarked_table(k : int) : int {
    unsafe {
        return g_pool[k] + g_plain[k]
    }
}

// 11. a local REFERENCE binding carries its destination's scratch-ness (the pool free-list
//     idiom: `var lst & = pool.buckets[b]`)
var @scratch g_buckets : table<int; array<int>>

[no_alloc]
def good_scratch_through_ref(b : int) : int {
    var lst & = unsafe(g_buckets[b])
    lst |> push(1)
    let v = lst[length(lst) - 1]
    lst |> pop()
    return v
}

// 12. a nolint ANYWHERE along the chain suppresses — the sink may bottom in daslib, so the
//     honest line is often an intermediate call site in a required module
def deep_alloc12(var a : array<int>) {
    a |> reserve(64)
}

def mid_hop12(var a : array<int>) {
    deep_alloc12(a)   // nolint:PERF026 — chain-middle suppression is honored
}

[no_alloc]
def good_chain_middle_nolint(var a : array<int>) {
    mid_hop12(a)
}

// 13. a temp-string builtin on a hot path is heap traffic
[no_alloc]
def bad_temp_string_builtin(c : int) : bool {
    return strings::to_char(c) == "a"                       // PERF026
}

// 14. the sizing builtins that reserve for a @scratch destination are the declared strategy
//     too; on an unmarked buffer each one is the reserve it wraps
[no_alloc]
def bad_sizing_builtins_on_the_unmarked_buffer(var sc : Scratch; n : int) {
    sc.reused |> ensure_capacity(n)
    unsafe(sc.reused |> scratch_reserve(n))
    unsafe(sc.reused |> scratch_ensure_capacity(n))
    unsafe(sc.reused |> scratch_resize(n))
    sc.fresh |> ensure_capacity(n)                          // PERF026
    unsafe(sc.fresh |> scratch_resize(n))                   // PERF026
}

// 15. a function value stored in a module table and invoked on a hot path: the walk follows every function the
//     program stores under that function type, so a table-driven dispatch ladder is no blind spot
struct Forms {
    go : function<(var a : array<int>) : void>
}

def alloc_through_value(var a : array<int>) {
    a |> push(4)                                            // PERF026, reached through the stored value
}

var g_forms : table<int; Forms>

[init]
def forms_boot {
    g_forms[0] = Forms(go = @@alloc_through_value)
}

[no_alloc]
def bad_table_dispatch(var a : array<int>) {
    let f = unsafe(g_forms?[0])
    invoke(f.go, a)
}

// 16. the same, with the stored function's parameter named otherwise than the field's type names it: the key is the
//     argument types, never their names, so the walk still follows the value
struct Hooks {
    run : function<(var buf : array<int>) : void>
}

def alloc_through_renamed(var items : array<int>) {
    items |> push(5)                                        // PERF026, reached through the stored value
}

var g_hooks : table<int; Hooks>

[init]
def hooks_boot {
    g_hooks[0] = Hooks(run = @@alloc_through_renamed)
}

[no_alloc]
def bad_renamed_dispatch(var a : array<int>) {
    let h = unsafe(g_hooks?[0])
    invoke(h.run, a)
}

// 17. @pool (a free list, a residency cache) carries the
//     @scratch sizing exemption: a field, a table indexed on a
//     miss, and a by-ref helper parameter
struct FreeList {
    @pool free : array<int>
    @pool free_f : array<float>
}

var @pool g_resident : table<int; int>

def pool_put(@pool var lst : array<int>; v : int) {
    lst |> push(v)
}

[no_alloc]
def good_pool_shapes(var fl : FreeList; k : int) : int {
    fl.free |> push(k)
    pool_put(fl.free, k)
    unsafe {
        return g_resident[k]
    }
}

// 18. a callee with a @scratch parameter is still walked:
//     only sizing of a reused destination is exempt in it, so
//     a pure sizing helper (delete-then-reserve included) stays
//     quiet and a working function's own allocation is reported
def overwrite_room(@scratch var a : array<float>; need : int64) {
    if (long_length(a) < need) {
        delete a
        a |> reserve(need)
    }
    a |> resize_no_init(need)
}

[no_alloc]
def good_pure_sizing_helper(var sc : Scratch; n : int64) {
    overwrite_room(sc.reused, n)
}

def fill_out(x : array<float>; @scratch var out : array<float>) {
    out |> resize(length(x))
    var tmp : array<float>
    tmp |> push(1.0)                    // PERF026, in a @scratch-param callee
    out[0] = tmp[0]
}

[no_alloc]
def bad_working_function_with_scratch_param(var sc : Scratch) {
    fill_out(sc.fresh, sc.reused)
}

// 19. an explicit delete of a @scratch field is sizing too:
//     it frees the buffer the owner regrows - in every form
//     delete lowers to: an array or table (builtin`finalize`),
//     a structure (finalize), a fixed array (builtin`finalize_dim`)
struct Rows {
    v : array<int>
}

struct DeleteForms {
    @scratch tab : table<int; int>
    @scratch rows : Rows
    @scratch fixed : array<int>[2]
}

[no_alloc]
def good_delete_of_scratch_field(var sc : Scratch) {
    delete sc.reused
}

[no_alloc]
def good_delete_of_scratch_table(var d : DeleteForms) {
    delete d.tab
}

[no_alloc]
def good_delete_of_scratch_struct(var d : DeleteForms) {
    delete d.rows
}

[no_alloc]
def good_delete_of_scratch_fixed_array(var d : DeleteForms) {
    delete d.fixed
}

[no_alloc]
def bad_delete_of_unmarked_field(var sc : Scratch) {
    delete sc.fresh                     // PERF026
}

// a finalizer the user wrote is still walked: freeing its own
// fields is the delete, anything else in it is held to the root
struct OwnFields {
    v : array<int>
}

def finalize(var x : OwnFields) {
    delete x.v
}

struct Grows {
    v : array<int>
}

def finalize(var x : Grows) {
    var tmp : array<int>
    tmp |> push(1)                      // PERF026, in a user finalizer
    delete x.v
}

struct Leaf {
    v : int
}

struct OwnsPointer {
    p : Leaf?
}

def finalize(var x : OwnsPointer) {
    unsafe {
        delete x.p
    }
}

struct UserFinalized {
    @scratch own : OwnFields
    @scratch grows : Grows
    @scratch ptr : OwnsPointer
}

[no_alloc]
def good_delete_through_own_field_finalizer(var u : UserFinalized) {
    delete u.own
}

[no_alloc]
def good_delete_through_pointer_field_finalizer(var u : UserFinalized) {
    delete u.ptr
}

[no_alloc]
def bad_delete_through_allocating_finalizer(var u : UserFinalized) {
    delete u.grows
}

struct InnerGrows {
    v : array<int>
}

def finalize(var x : InnerGrows) {
    var tmp : array<int>
    tmp |> push(1)                      // PERF026, under a generated finalizer
    delete x.v
}

struct OuterGrows {
    inner : InnerGrows
}

struct ThroughRef {
    items : array<int>
}

def finalize(var x : ThroughRef) {
    var p & = unsafe(x.items)
    delete p
}

struct Chain {
    v : array<int>
    next : Chain?
}

struct Reused {
    @scratch outer : OuterGrows
    @scratch through_ref : ThroughRef
    @scratch chain : Chain
    @scratch rows : array<OwnFields>
    @scratch by_key : table<int; OwnFields>
}

[no_alloc]
def bad_alloc_under_generated_finalizer(var r : Reused) {
    delete r.outer
}

[no_alloc]
def good_self_free_through_reference(var r : Reused) {
    delete r.through_ref
}

[no_alloc]
def good_elements_free_themselves(var r : Reused) {
    delete r.rows
    delete r.by_key
}

[no_alloc]
def good_recursive_type_frees_itself(var r : Reused) {
    unsafe {
        delete r.chain
    }
}

// 20. a reference bound through a ternary carries the mark
//     only when both branches do
struct TwoScratch {
    @scratch a : array<float>
    @scratch b : array<float>
    @pool p : array<float>
    plain : array<float>
}

[no_alloc]
def good_ternary_ref(var s : TwoScratch; c : bool; n : int) {
    var x & = c ? s.a : s.b
    x |> resize(n)
}

[no_alloc]
def good_ternary_ref_scratch_or_pool(var s : TwoScratch; c : bool; n : int) {
    var x & = c ? s.a : s.p
    x |> resize(n)
}

[no_alloc]
def bad_ternary_ref_one_unmarked(var s : TwoScratch; c : bool; n : int) {
    var x & = c ? s.a : s.plain
    x |> resize(n)                                          // PERF026
}

[no_alloc]
def bad_ternary_ref_unmarked_left(var s : TwoScratch; c : bool; n : int) {
    var x & = c ? s.plain : s.a
    x |> resize(n)                                          // PERF026
}

// 23. a class pointer frees through its class finalizer, reached
//     by the class's dispatch: its own fields are the delete,
//     a derived override included; anything else is reported
class CleansItself {
    items : array<int>
    def operator delete {
        delete self.items
    }
}

class CleansMore : CleansItself {
    more : array<int>
    def operator delete {
        delete self.more
        delete super.self
    }
}

class GrowsOnDelete {
    items : array<int>
    def operator delete {
        var tmp : array<int>
        tmp |> push(1)                  // PERF026, in a class finalizer
        delete self.items
    }
}

struct ClassHolder {
    @scratch own : CleansItself?
    @scratch derived : CleansMore?
    @scratch grows : GrowsOnDelete?
}

[no_alloc]
def good_delete_class_pointer(var h : ClassHolder) {
    unsafe {
        delete h.own
    }
}

[no_alloc]
def good_delete_derived_class_pointer(var h : ClassHolder) {
    unsafe {
        delete h.derived
    }
}

[no_alloc]
def bad_delete_class_pointer_that_allocates(var h : ClassHolder) {
    unsafe {
        delete h.grows
    }
}

// 24. a function value in a class's data field is followed like a
//     struct's: only declared methods match by member name
def alloc_through_hook(var a : array<float>) {
    a |> push(6.)                       // PERF026, through the class field
}

class HasHook {
    hook : function<(var a : array<float>) : void>
}

var g_hooked : HasHook?

[init]
def hooked_boot {
    g_hooked = new HasHook()
    g_hooked.hook = @@alloc_through_hook
}

[no_alloc]
def bad_class_data_field_dispatch(var a : array<float>) {
    invoke(g_hooked.hook, a)
}

// 25. the call site: a buffer bound to a @scratch / @pool
//     parameter must itself be declared reused, since the
//     helper's sizing of it is exempt only on that promise
def size_pool(@pool var a : array<float>; n : int) {
    a |> resize(n)
}

[cold_path]
def size_room_once(@scratch var a : array<float>; n : int) {
    a |> resize(n)
}

var @scratch g_room : array<float>

def forward_room(@scratch var a : array<float>; n : int) {
    ensure_room(a, n)
}

[no_alloc]
def bad_unmarked_into_scratch_param(var sc : Scratch; var plain : array<float>; n : int) {
    ensure_room(sc.fresh, n)                                  // PERF026: an unmarked field
    var local : array<float>
    ensure_room(local, n)                                     // PERF026: an unmarked local
    ensure_room(plain, n)                                     // PERF026: an unmarked parameter
}

[no_alloc]
def good_reused_into_scratch_param(var sc : Scratch; var two : TwoScratch;
                                   var fl : FreeList; c : bool; n : int) {
    ensure_room(sc.reused, n)
    ensure_room(g_room, n)
    forward_room(sc.reused, n)
    var r & = unsafe(sc.reused)
    ensure_room(r, n)
    ensure_room(c ? two.a : two.b, n)
    ensure_room(c ? two.a : two.p, n)
    size_pool(fl.free_f, n)
    size_room_once(sc.fresh, n)
}

// 26. a generic helper's instance keeps its parameter's mark
def size_any(@scratch var a : array<auto(numT)>; n : int) {
    a |> resize(n)
}

[no_alloc]
def bad_unmarked_into_generic_scratch_param(var sc : Scratch; n : int) {
    size_any(sc.reused, n)
    size_any(sc.fresh, n)               // PERF026: unmarked, via a generic
}

// 27. a function's own body block is generated, a `=>` body's
//     included, but the statements in it are the author's
[no_alloc]
def bad_interpolation_in_expression_body(k : int) : string => "k={k}"   // PERF026

[no_alloc]
def bad_table_index_in_expression_body(k : int) : int => unsafe(g_plain[k])   // PERF026

// 28. an error message's interpolation on a hot path is an
//     accepted suppression: a nolint at the text, with its reason
def unserved_reason(ok : bool; k : int) : string
    => ok ? "" : "k {k} is unserved"    // nolint:PERF026 - an error message

var g_last_text : string

def unserved_with_side_text(ok : bool; k : int) : string {
    g_last_text = "checked {k}"         // PERF026, not the error message
    return ok ? "" : "k {k} is unserved"    // nolint:PERF026 - an error message
}

[no_alloc]
def good_error_message_text(k : int) : bool => empty(unserved_reason(k > 0, k))

[no_alloc]
def bad_text_beside_an_error_message(k : int) : bool => empty(unserved_with_side_text(k > 0, k))

// 29. the call site through an invoke: a class method's @scratch
//     parameter, a function value's, and a @pool parameter
class Sizer {
    v : int
    def size_into(@scratch var a : array<float>; n : int) {
        a |> resize(n)
    }
}

var g_sizer : Sizer?

def size_through_value(@scratch var a : array<float>; n : int64) {
    a |> resize(n)
}

struct SizerHook {
    fill : function<(var a : array<float>; n : int64) : void>
}

var g_sizer_hook : SizerHook

[init]
def sizer_boot {
    g_sizer = new Sizer()
    g_sizer_hook.fill = @@size_through_value
}

[no_alloc]
def bad_unmarked_into_invoked_params(var sc : Scratch; var fl : FreeList; n : int) {
    g_sizer->size_into(sc.fresh, n)     // PERF026: a class method's @scratch parameter
    invoke(g_sizer_hook.fill, sc.fresh, int64(n))   // PERF026: a function value's
    size_pool(sc.fresh, n)              // PERF026: a @pool parameter
}

[no_alloc]
def good_reused_into_invoked_params(var sc : Scratch; n : int) {
    g_sizer->size_into(sc.reused, n)
    invoke(g_sizer_hook.fill, sc.reused, int64(n))
}

// 30. text under a panic is on the abort path
[no_alloc]
def good_interpolation_under_panic(k : int) {
    if (k < 0) {
        panic("negative k {k}")
    }
}

// 31. a by-value local is made on every call: a reuse mark on it, or
//     on a field of it, is not a reused buffer
[no_alloc]
def bad_marks_on_per_call_locals(n : int) {
    var @scratch local : array<float>
    ensure_room(local, n)                 // PERF026: a @scratch local of this call
    var fresh_sc : Scratch
    ensure_room(fresh_sc.reused, n)       // PERF026: a marked field of a local
    fresh_sc.reused |> resize(n)        // PERF026: its sizing too
}

var g_scratch_at : Scratch?

[no_alloc]
def good_marks_through_a_local_pointer(n : int) {
    var at = g_scratch_at
    ensure_room(at.reused, n)
}

struct ScratchHolder {
    at : Scratch?
}

// 32. a path through a pointer a by-value local holds reaches the
//     long-lived pointee, whose marks hold
[no_alloc]
def good_marks_through_a_held_pointer(n : int) {
    var h = ScratchHolder(at = g_scratch_at)
    ensure_room(h.at.reused, n)
    ensure_room((*h.at).reused, n)
}

// 33. a loop variable over a reused container is a reused buffer
var @scratch g_rooms : array<array<float>>
var g_plain_rooms : array<array<float>>

[no_alloc]
def good_loop_alias_of_reused_rooms(n : int) {
    for (a in g_rooms) {
        ensure_room(a, n)
        a |> resize(n)
    }
}

[no_alloc]
def bad_loop_alias_of_plain_rooms(n : int) {
    for (a in g_plain_rooms) {
        ensure_room(a, n)               // PERF026: the loop runs over an unmarked container
    }
}
