options gen2
options persistent_heap

require _common public

let THRESHOLD = 2009

// _distinct_by(_.brand) |> count(P) — distinct brands whose first-occurrence Car
// satisfies P (Theme 4, audit 3c).
// SQL: no clean form — sqlite_linq's `_distinct_by + count(P)` 2-arg shape isn't
// surfaced through `_sql` (count with predicate after distinct lowers as
// `COUNT(* FILTER WHERE ...)` which the lowerer doesn't emit). By design.
// Array/Decs splice through plan_distinct / plan_decs_distinct's 2-arg-count extension:
// dedup table built unconditionally so `distinct_by` semantics keeps FIRST occurrence
// per key; a separate counter increments only when P matches that first occurrence.
// Companion to distinct_by_count.das which has no predicate.

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        unsafe {
            let c = _fold(each(arr) |> _distinct_by(_.brand) |> count($(c) => c.year > THRESHOLD))
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        unsafe {
            let c = _fold(from_decs_template(type<DecsCar>) |> _distinct_by(_.brand) |> count($(c) => c.year > THRESHOLD))
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def distinct_count_pred_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def distinct_count_pred_m4(b : B?) {
    run_m4(b, 100000)
}
