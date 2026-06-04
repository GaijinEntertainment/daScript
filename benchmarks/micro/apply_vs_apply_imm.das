options gen2
options persistent_heap

require daslib/apply
require dastest/testing_boost
require strings

// Profiling micro-bench: the inline `apply` (ref-local field binding `let/var field & = unsafe(self.field)`
// + a single `__self` bind) vs `apply_imm` (the same body, but each param aliased via `ExprAssume`, and
// `self` re-read per field). Both inline the per-field body — no helper fn, no per-field block invoke — so
// this isolates ref-local-vs-assume + the `__self` single-bind cost. Decides whether `apply_imm` earns its
// keep: per the plan it survives only if it is >=5% faster than `apply` (else it is deleted, `apply`
// covering every case). Self-contained (no _common) so it stays out of the results.md sweep.

let N = 200000

struct Rec {
    a : int
    b : float
    c : int
    d : string
    e : bool
    f : int
}

// Identical per-field accumulation for both lanes — the only difference is the macro that walks the fields.
def walk_apply(r : Rec) : int {
    var acc = 0
    apply(r) $(name : string; field) {
        static_if (typeinfo stripped_typename(field) == "int") {
            acc += field
        } static_elif (typeinfo stripped_typename(field) == "float") {
            acc += int(field)
        } static_elif (typeinfo stripped_typename(field) == "string") {
            acc += length(field)
        } static_elif (typeinfo stripped_typename(field) == "bool") {
            acc += field ? 1 : 0
        }
    }
    return acc
}

def walk_apply_imm(r : Rec) : int {
    var acc = 0
    apply_imm(r) $(name : string; field) {
        static_if (typeinfo stripped_typename(field) == "int") {
            acc += field
        } static_elif (typeinfo stripped_typename(field) == "float") {
            acc += int(field)
        } static_elif (typeinfo stripped_typename(field) == "string") {
            acc += length(field)
        } static_elif (typeinfo stripped_typename(field) == "bool") {
            acc += field ? 1 : 0
        }
    }
    return acc
}

[benchmark]
def probe_apply(b : B?) {
    let r = Rec(a = 1, b = 2.5, c = 3, d = "hello", e = true, f = 7)
    b |> run("apply/{N}", N) {
        var s = 0
        for (_i in range(N)) {
            s += walk_apply(r)
        }
        if (s == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def probe_apply_imm(b : B?) {
    let r = Rec(a = 1, b = 2.5, c = 3, d = "hello", e = true, f = 7)
    b |> run("apply_imm/{N}", N) {
        var s = 0
        for (_i in range(N)) {
            s += walk_apply_imm(r)
        }
        if (s == 0) {
            b->failNow()
        }
    }
}
