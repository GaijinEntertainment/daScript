options gen2
options persistent_heap

require _common public

// Fixture car ids run 1..n so id=42 is unique. `single` walks the full source
// (semantically — exactly-one-match), but the splice still fuses upstream where.
//
// SQL: SELECT * FROM Cars WHERE id = 42 LIMIT 2 — read at most 2 to assert uniqueness.
// m3/m3f traverse the array filtering and asserting one survivor.

let TARGET_ID = 42

def run_m1(b : B?; n : int) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db, n)
        b |> run("m1_sql/{n}", n) {
            let row = _sql(db |> select_from(type<Car>) |> _where(_.id == TARGET_ID) |> _first())
            if (row.id == 0) {
                b->failNow()
            }
        }
    }
}

def run_m3(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3_array/{n}", n) {
        let row = arr |> _where(_.id == TARGET_ID) |> single()
        if (row.id == 0) {
            b->failNow()
        }
    }
}
def run_m3f(b : B?; n : int) {
    let arr <- fixture_array(n)
    b |> run("m3f_array_fold/{n}", n) {
        let row = _fold(each(arr)._where(_.id == TARGET_ID).single())
        if (row.id == 0) {
            b->failNow()
        }
    }
}

[benchmark]
def single_match_m1(b : B?) {
    run_m1(b, 100000)
}

[benchmark]
def single_match_m3(b : B?) {
    run_m3(b, 100000)
}

[benchmark]
def single_match_m3f(b : B?) {
    run_m3f(b, 100000)
}
