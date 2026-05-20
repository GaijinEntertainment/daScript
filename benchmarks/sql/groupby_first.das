options gen2
options persistent_heap

require _common public

// _group_by(_.brand) |> _select((Brand=key, FirstCar=first source elem per group)).
// No direct SQL aggregator for "first source-order row per group" → m1 omitted; m3 vs m3f
// is the headline comparison. Splice (PR-A2) emits miss-branch-only loop (no hit update).

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let groups <- (arr._group_by(_.brand)._select((Brand = _._0,
                                                       FirstCar = _._1 |> first())))
        b |> accept(groups)
        if (empty(groups)) {
            b->failNow()
        }
    }
}
def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let groups <- _fold(each(arr)
                            ._group_by(_.brand)
                            ._select((Brand = _._0,
                                      FirstCar = _._1 |> first()))
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
                            ._group_by(_.brand)
                            ._select((Brand = _._0,
                                      FirstCar = _._1 |> first()))
                            .to_array())
        b |> accept(groups)
        if (empty(groups)) {
            b->failNow()
        }
    }
}

[benchmark]
def groupby_first_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def groupby_first_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def groupby_first_m4(b : B?) {
    run_m4(b, 100000)
}
