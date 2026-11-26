//! PERF028 is reported for: console / file I/O on a hot path.
options gen2
options auto_inline_functions = false   // lint fixtures assert SOURCE shapes; splices rewrite them
// PERF028: console / file I/O on a hot path
//
// Problem:
//   A print or to_log left in a per-frame path is the debug line that never
//   got removed. It serializes on the console handle and, with an
//   interpolated message, allocates as well (that half is PERF026).
//
// Bad pattern:
//   [hot_path]
//   def step(i : int) {
//       to_log(LOG_INFO, "step {i}\n")
//   }
//
// Good pattern:
//   accumulate into a counter and report once outside the loop, or put the
//   logging behind a [cold_path] helper guarded by a debug flag.
//
// This is the rule to reach for DAS_LINT_DISABLE=PERF028 on: adding a log
// line while chasing a bug should not require editing the annotation.
//
// See perf026_hot_path_alloc.das for how the annotation-gated scan works.

expect 31208:10

require daslib/perf_lint
require _hot_path_macro

// 1. I/O written directly in the annotated function
[no_io]
def bad_direct() {
    print("tick\n")
}

// 2. I/O reached through an ordinary helper
def io_helper() {
    to_log(LOG_INFO, "tick\n")
}

[no_io]
def bad_transitive() {
    io_helper()
}

// 3. [cold_path] prunes the walk
[cold_path]
def cold_report() {
    print("summary\n")
}

[no_io]
def good_cold() {
    cold_report()
}

// 4. per-line suppression still works
[no_io]
def good_nolint() {
    print("tick\n")   // nolint:PERF028
}

// 5. an unannotated function is never scanned
def good_unannotated() {
    print("tick\n")
}

// 6. [no_io] ignores allocation and env reads — those are PERF026 / PERF027
[no_io]
def good_other_contracts(var a : array<int>) {
    a |> push(1)
}

// 7. a callee with a @scratch parameter is still walked for I/O
struct Frame {
    @scratch rows : array<float>
}

def log_into(@scratch var out : array<float>) {
    out |> resize(1)
    print("rows\n")                     // PERF028, in a @scratch-param callee
}

[no_io]
def bad_io_under_scratch_callee(var fr : Frame) {
    log_into(fr.rows)
}

// 8. a finalizer the user wrote is walked when a reused buffer
//    is deleted on a hot path
struct Logged {
    v : array<int>
}

def finalize(var x : Logged) {
    print("bye\n")                      // PERF028, in a user finalizer
    delete x.v
}

struct Owner {
    @scratch logged : Logged
}

[no_io]
def bad_io_in_user_finalizer(var o : Owner) {
    delete o.logged
}

// 9. a reused delete follows every finalizer it reaches: through a
//    generated one, an array, a table, and a chain three deep
struct InnerLogged {
    v : int
}

def finalize(var x : InnerLogged) {
    print("inner\n")                    // PERF028, under a generated finalizer
}

struct OuterPlain {
    inner : InnerLogged
}

struct ElemLogged {
    v : int
}

def finalize(var x : ElemLogged) {
    print("elem\n")                     // PERF028, per array element
}

struct ValueLogged {
    v : int
}

def finalize(var x : ValueLogged) {
    print("value\n")                    // PERF028, per table value
}

struct DeepLogged {
    v : int
}

def finalize(var x : DeepLogged) {
    print("deep\n")                     // PERF028, three finalizers down
}

struct DeepMid {
    leaf : DeepLogged
}

struct DeepTop {
    mid : DeepMid
}

struct Reused {
    @scratch outer : OuterPlain
    @scratch elems : array<ElemLogged>
    @scratch values : table<int; ValueLogged>
    @scratch deep : DeepTop
}

[no_io]
def bad_io_under_generated_finalizer(var r : Reused) {
    delete r.outer
}

[no_io]
def bad_io_per_array_element(var r : Reused) {
    delete r.elems
}

[no_io]
def bad_io_per_table_value(var r : Reused) {
    delete r.values
}

[no_io]
def bad_io_three_finalizers_down(var r : Reused) {
    delete r.deep
}

// 10. a class finalizer reached through the class's dispatch is
//     walked for I/O too
class LogsOnDelete {
    v : int
    def operator delete {
        print("class bye\n")            // PERF028, in a class finalizer
    }
}

struct ClassOwner {
    @scratch logged : LogsOnDelete?
}

[no_io]
def bad_io_in_class_finalizer(var o : ClassOwner) {
    unsafe {
        delete o.logged
    }
}

// 11. the class dispatch reaches the finalizer only, not another
//     method of the same type
class TalksElsewhere {
    v : int
    def operator delete {
        v = 0
    }
    def talk {
        print("hi\n")
    }
}

struct TalkerOwner {
    @scratch talker : TalksElsewhere?
}

[no_io]
def good_delete_does_not_reach_other_methods(var o : TalkerOwner) {
    unsafe {
        delete o.talker
    }
}

// 12. a method call enters only that method, and a macro's own
//     generated block in a method stays shielded while the
//     method's own statements are read
class Speaks {
    v : int
    def quiet_step {
        v++
    }
    def noisy_step {
        print("noisy\n")
    }
    def mixed_step {
        macro_logged_block()
        print("own\n")                  // PERF028, the method's own statement
    }
}

var g_speaks : Speaks?

[no_io]
def good_method_call_enters_only_that_method() {
    g_speaks->quiet_step()
}

[no_io]
def bad_own_statement_beside_macro_block() {
    g_speaks->mixed_step()
}

// 13. a plain function whose whole body a macro rewrote into a
//     `with` keeps it hidden: only a class method's `with` is read
struct WithSubject {
    v : int
}

[no_io]
def good_macro_with_in_plain_function(var ws : WithSubject) {
    macro_logged_with(ws)
}
