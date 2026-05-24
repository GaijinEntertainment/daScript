options gen2
options persistent_heap

require _common public

let THRESHOLD = 500

// SQL pushes COUNT(*) to the engine returning one row.
// Array and Decs lanes fold where+count into a single fused pass
// (no intermediate filter array).

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let c = _sql(db |> select_from(type<Car>) |> _where(_.price > THRESHOLD) |> count())
            b |> accept(c)
            if (c == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let c = _fold(each(arr)._where(_.price > THRESHOLD).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

def run_m4(b : B?; n : int) {
    fixture_decs(n)
    b |> run("m4_decs_fold/{n}", n) {
        let c = _fold(from_decs_template(type<DecsCar>)._where(_.price > THRESHOLD).count())
        b |> accept(c)
        if (c == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def count_aggregate_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def count_aggregate_m3f(b : B?) {
    run_m3f(b, 100000)
}

[benchmark]
def count_aggregate_m4(b : B?) {
    run_m4(b, 100000)
}
