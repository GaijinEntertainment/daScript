options gen2
options persistent_heap

require _common public

let THRESHOLD = 500

// from_decs_template(...) |> count(P) — bare-chain count with predicate, no upstream where/select.
// Decs (m4): plan_decs_unroll's bare-chain 2-arg count(P) reach (Theme 4 root-cause fix
// to extract_decs_bridge). Previously bailed because `forExpr.iteratorVariables` was
// unpopulated when no chain op forces an inference pass over the bridge's inner for-loop;
// the bridge now recovers iter names from `mkTup.values` (peeling the ExprRef2Value wrap),
// so the emit_decs_accumulator 2-arg count(P) path is reachable on bare chains.
// No array (m3f) lane — array-side bare count(P) was always reachable. No SQL lane —
// covered by count_aggregate.das with a where shape.

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        unsafe {
            let c = _fold(from_decs_template(type<DecsCar>) |> count($(c) => c.price > THRESHOLD))
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def decs_count_bare_pred_m4(b : B?) {
    run_m4(b, 100000)
}
