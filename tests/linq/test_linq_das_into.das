options gen2
options rtti
// decs group/orderby chains fall to the tier-2 materialize path (no dedicated decs splice arm) and emit a
// cosmetic `_fold` perf advisory — suppress it here (a decs-fold gap, not a linq_das issue).
options _no_linq_perf_warn = true

require dastest/testing_boost public
require daslib/linq_das
require daslib/decs_boost
require daslib/linq
require daslib/linq_boost
require daslib/linq_fold

// linq_das query continuation (`into`): `group c by k into g …` rebinds the group `g` (A2: `g.key` is the
// key, a bare `g` is the member collection), and `select p into n …` rebinds the projected value. Both
// continue on the SAME `_fold` chain. Array + decs sources here; SQL parity lives in tests/dasSQLITE.

struct Car {
    name  : string
    price : int
    brand : string
}

def private mk_cars() : array<Car> {
    return <- [Car(name = "cheap", price = 50,  brand = "eco"),
               Car(name = "mid",   price = 150, brand = "eco"),
               Car(name = "lux",   price = 300, brand = "lux")]
}

[decs_template(prefix = "ld_")]
struct CarComp {
    name  : string
    price : int
    brand : string
}

def private seed_decs() {
    restart()
    for (i in range(3)) {
        create_entity() @(eid, cmp) {
            cmp.eid := eid
            cmp.ld_name := "car{i}"
            cmp.ld_price := 100 * (i + 1)        // 100, 200, 300
            cmp.ld_brand := i < 2 ? "a" : "b"    // a, a, b
        }
    }
    commit()
}

// ===== group … into g … select  (grouped aggregation) =====

[test]
def test_group_into_aggregate(t : T?) {
    let cars <- mk_cars()
    let rep <- %linq! from c in cars
                      group c by c.brand into g
                      select (brand = g.key,
                              n     = g |> length,
                              total = g |> select($(u : Car) => u.price) |> sum,
                              lo    = g |> select($(u : Car) => u.price) |> min,
                              hi    = g |> select($(u : Car) => u.price) |> max) %%
    t |> equal(length(rep), 2, "two brand buckets")
    for (r in rep) {
        if (r.brand == "eco") {
            t |> equal(r.n, 2, "eco count")
            t |> equal(r.total, 200, "eco total 50+150")
            t |> equal(r.lo, 50)
            t |> equal(r.hi, 150)
        }
        if (r.brand == "lux") {
            t |> equal(r.n, 1, "lux count")
            t |> equal(r.total, 300)
            t |> equal(r.lo, 300)
            t |> equal(r.hi, 300)
        }
    }
}

[test]
def test_group_into_average(t : T?) {
    let cars <- mk_cars()
    let rep <- %linq! from c in cars
                      group c by c.brand into g
                      select (brand = g.key, avg = g |> select($(u : Car) => u.price) |> average) %%
    for (r in rep) {
        if (r.brand == "eco") { t |> equal(r.avg, 100.0lf, "eco avg (50+150)/2") }
        if (r.brand == "lux") { t |> equal(r.avg, 300.0lf, "lux avg") }
    }
}

[test]
def test_group_into_where_on_groups(t : T?) {
    let cars <- mk_cars()
    // keep only buckets with >1 member → just eco
    let rep <- %linq! from c in cars
                      group c by c.brand into g
                      where g |> length > 1
                      select (brand = g.key, n = g |> length) %%
    t |> equal(length(rep), 1, "only multi-member buckets survive")
    t |> equal(rep[0].brand, "eco")
    t |> equal(rep[0].n, 2)
}

[test]
def test_group_into_orderby_on_groups(t : T?) {
    let cars <- mk_cars()
    // order buckets by their total, descending → lux(300) before eco(200)
    let rep <- %linq! from c in cars
                      group c by c.brand into g
                      orderby g |> select($(u : Car) => u.price) |> sum descending
                      select (brand = g.key, total = g |> select($(u : Car) => u.price) |> sum) %%
    t |> equal(length(rep), 2)
    t |> equal(rep[0].brand, "lux", "highest total first")
    t |> equal(rep[1].brand, "eco")
}

[test]
def test_group_into_member_keeping_identity(t : T?) {
    let cars <- mk_cars()
    // `select g` is the identity continuation — yields the group tuples (key, members) unchanged.
    let rep <- %linq! from c in cars group c by c.brand into g select g %%
    t |> equal(length(rep), 2)
    for (g in rep) {
        if (g._0 == "eco") { t |> equal(length(g._1), 2) }
        if (g._0 == "lux") { t |> equal(length(g._1), 1) }
    }
}

[test]
def test_group_into_pre_where(t : T?) {
    let cars <- mk_cars()
    // a pre-group `where` filters rows before grouping; the continuation aggregates the survivors.
    let rep <- %linq! from c in cars
                      where c.price >= 150
                      group c by c.brand into g
                      select (brand = g.key, n = g |> length) %%
    t |> equal(length(rep), 2, "mid→eco, lux→lux: one each")
    for (r in rep) { t |> equal(r.n, 1) }
}

// ===== select … into n … (projection continuation) =====

[test]
def test_select_into_then_where(t : T?) {
    let cars <- mk_cars()
    let rep <- %linq! from c in cars
                      select c.price into p
                      where p > 100
                      select p %%
    t |> equal(length(rep), 2, "prices 150, 300 survive p>100")
    t |> equal(rep[0], 150)
    t |> equal(rep[1], 300)
}

[test]
def test_select_into_orderby(t : T?) {
    let cars <- mk_cars()
    let rep <- %linq! from c in cars
                      select c.price into p
                      orderby p descending
                      select p %%
    t |> equal(rep[0], 300, "descending by projected value")
    t |> equal(rep[2], 50)
}

[test]
def test_multi_stage_group_then_project_then_filter(t : T?) {
    let cars <- mk_cars()
    // 3 stages: group → aggregate → filter+order the aggregated rows.
    let rep <- %linq! from c in cars
                      group c by c.brand into g
                      select (brand = g.key, total = g |> select($(u : Car) => u.price) |> sum) into agg
                      where agg.total > 100
                      orderby agg.total descending
                      select agg.brand %%
    t |> equal(length(rep), 2)
    t |> equal(rep[0], "lux", "lux total 300 first")
    t |> equal(rep[1], "eco")
}

[test]
def test_group_into_iterator_output(t : T?) {
    let cars <- mk_cars()
    var n = 0
    for (_r in %linq! from c in cars group c by c.brand into g select (brand = g.key, n = g |> length) iterator %%) { n++ }
    t |> equal(n, 2, "iterator over the aggregated buckets")
}

// ===== decs source =====

[test]
def test_decs_group_into_aggregate(t : T?) {
    seed_decs()
    // count per brand bucket via the group continuation (a/a/b → a:2, b:1).
    let rep <- %linq! from c : CarComp in decs group c by c.brand into g select (brand = g.key, n = g |> length) %%
    t |> equal(length(rep), 2, "decs buckets a/b")
    for (r in rep) {
        if (r.brand == "a") { t |> equal(r.n, 2) }
        if (r.brand == "b") { t |> equal(r.n, 1) }
    }
}

[test]
def test_decs_select_into(t : T?) {
    seed_decs()
    let rep <- %linq! from c : CarComp in decs
                      select c.price into p
                      where p > 100
                      select p %%
    t |> equal(length(rep), 2, "decs prices 200,300 survive p>100")
}
