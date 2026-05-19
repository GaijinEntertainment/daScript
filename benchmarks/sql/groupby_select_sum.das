options gen2
options persistent_heap

require _common public

// _select(c => c.price) |> _group_by(price % 100) |> _select((K=key, S=sum)).
// _sql LINQ-to-SQL requires `_group_by(_.Field)` (no expression keys) → m1 omitted; m3 vs m3f
// is the headline comparison. Splice (PR-B) fuses the upstream Car→price projection into an
// intermediate var bind so the key + sum acc reference the projected int (no per-element Car re-access).

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let groups <- (arr._select(_.price)._group_by(_ % 100)._select((K = _._0, S = _._1 |> sum())))
        if (empty(groups)) {
            b->failNow()
        }
    }
}
def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let groups <- _fold(each(arr)
                            ._select(_.price)
                            ._group_by(_ % 100)
                            ._select((K = _._0, S = _._1 |> sum()))
                            .to_array())
        if (empty(groups)) {
            b->failNow()
        }
    }
}

[benchmark]
def groupby_select_sum_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def groupby_select_sum_m3f(b : B?) {
    run_m3f(b, 100000)
}
