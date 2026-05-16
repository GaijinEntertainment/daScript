options gen2
options persistent_heap

require _common public

// SQL: SELECT AVG(price) FROM Cars.
// m3/m3f_old/m3f sum and divide on a single pass.

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let a = _sql(db |> select_from(type<Car>) |> _select(_.price) |> average())
            if (a == 0.0lf) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let a = arr |> _select(double(_.price)) |> average()
        if (a == 0.0lf) {
            b->failNow()
        }
    }
}

def run_m3f_old(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_old_array_fold/{n}", n) {
        let a = _old_fold(each(arr)._select(double(_.price)).average())
        if (a == 0.0lf) {
            b->failNow()
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let a = _fold(each(arr)._select(double(_.price)).average())
        if (a == 0.0lf) {
            b->failNow()
        }
    }
}

[benchmark]
def average_aggregate_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def average_aggregate_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def average_aggregate_m3f_old(b : B?) {
    run_m3f_old(b, 100000)
}

[benchmark]
def average_aggregate_m3f(b : B?) {
    run_m3f(b, 100000)
}
