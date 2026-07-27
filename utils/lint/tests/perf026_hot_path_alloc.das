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
//   size the buffer once at setup, or put the one-time growth behind a
//   [cold_path] helper so the contract scan stops there.
//
// The rule is annotation-gated: nothing is checked until a function carries
// [hot_path] (all three contracts) or the narrow [no_alloc] / [no_env] /
// [no_io]. From each root the scan follows DIRECT calls transitively, so a
// sink several frames deep is still reported — anchored on the deepest line
// written in the root's own file, not on the daslib internal that allocates.

expect 31208:4

require daslib/perf_lint

struct Blob {
    v : int
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

// 6. new and delete are heap traffic too
[no_alloc]
def bad_new_delete() : int {
    var p = new Blob(v = 7)
    let v = p.v
    unsafe {
        delete p
    }
    return v
}
