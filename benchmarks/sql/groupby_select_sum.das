options gen2
options persistent_heap

require _common public

// _select(c => c.price) |> _group_by(price % 100) |> _select((K=key, S=sum)).
// SQL omitted: sqlite_linq's `_group_by` requires `_.Field` or a tuple of `_.Field`s.
// The expression key (`_ % 100`) is rejected. [Follow-up TODO 2026-05-23: error[50503]
// `_sql: _group_by: key must be \`_.Field\` or a tuple of \`_.Field\`s; got: (_ % 100)`
// — would need expression-key support in sqlite_linq lowering.]
// Array vs Decs is the headline comparison. Splice (PR-B) fuses the upstream Car→price
// projection into an intermediate var bind so the key + sum acc reference the projected
// int (no per-element Car re-access).

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let groups <- _fold(each(arr)
                            ._select(_.price)
                            ._group_by(_ % 100)
                            ._select((K = _._0, S = _._1 |> sum()))
                            .to_array())
        b |> accept(groups)
        if (empty(groups)) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let groups <- _fold(from_decs_template(type<DecsCar>)
                            ._select(_.price)
                            ._group_by(_ % 100)
                            ._select((K = _._0, S = _._1 |> sum()))
                            .to_array())
        b |> accept(groups)
        if (empty(groups)) {
            b->failNow()
        }
    }
}

[benchmark]
def groupby_select_sum_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def groupby_select_sum_m4(b : B?) {
    run_m4(b, 100000)
}
