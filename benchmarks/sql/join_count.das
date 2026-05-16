options gen2
options persistent_heap

require _common public

// _join(cars, dealers, on=c.dealer_id == d.id, into=...) |> count.
// SQL form (`_sql(... |> _join(select_from(type<Dealer>), ...))`) requires the
// inner select_from to bind the db handle inside the _sql analyzer; that wiring
// is not exposed for direct authoring here, so m1 is omitted for this benchmark.
// 3-way comparison (m3 / m3f_old / m3f) focuses on array-side join cost.

def run_m3(b : B?; n : int) {
    let cars <- fixture_array(n)
    let dealers <- fixture_dealers_array()
    b |> run("m3_array/{n}", n) {
        let c = (cars |> _join(dealers,
                               $(c : Car, d : Dealer) => c.dealer_id == d.id,
                               $(c : Car, d : Dealer) => (c.name, d.name))
                      |> count())
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let cars <- fixture_array(n)
    let dealers <- fixture_dealers_array()
    b |> run("m3f_old_array_fold/{n}", n) {
        let c = _old_fold(cars |> _join(dealers,
                                        $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                        $(c : Car, d : Dealer) => (c.name, d.name))
                               |> count())
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let cars <- fixture_array(n)
    let dealers <- fixture_dealers_array()
    b |> run("m3f_array_fold/{n}", n) {
        let c = _fold(cars |> _join(dealers,
                                    $(c : Car, d : Dealer) => c.dealer_id == d.id,
                                    $(c : Car, d : Dealer) => (c.name, d.name))
                           |> count())
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def join_count_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def join_count_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def join_count_m3f(b : B?) {
    run_m3f(b, 100000)
}
