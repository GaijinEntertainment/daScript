options gen2

// Regression: a NAMED-param `_where($(c) => …)` BEFORE a `_join` previously leaked the lambda var
// `c` into the emitted bind list (`error[30838]: can't locate variable 'c'`). The column-ref resolver
// rescued an unbound param to source 0 only for the `_` placeholder; the LHS `_where` runs before the
// join's `into` populates argNames (and before seenJoin is set), so a named param never resolved. Fixed
// at three mirror sites (column-ref / JSON-path / nullable-probe) by resolving any param to t0 while no
// join is seen yet. These tests exercise all three predicate shapes with a named lambda parameter.

require dastest/testing_boost public

require strings
require daslib/sql
require daslib/linq_boost
require daslib/linq_fold
require sqlite/sqlite_boost
require sqlite/sqlite_linq

struct CarMeta {
    color : string
}

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key id : int
    name : string
    did  : int
    @sql_json meta : CarMeta
}

[sql_table(name = "Dealers")]
struct Dealer {
    @sql_primary_key id : int
    city : string
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    db |> create_table(type<Dealer>)
    db |> insert(Car(id = 1, name = "a", did = 10, meta = CarMeta(color = "red")))
    db |> insert(Car(id = 2, name = "c", did = 20, meta = CarMeta(color = "blue")))
    db |> insert(Car(id = 3, name = "d", did = 30, meta = CarMeta(color = "red")))
    db |> insert(Dealer(id = 10, city = "NY"))
    db |> insert(Dealer(id = 20, city = "LA"))
    db |> insert(Dealer(id = 30, city = "NY"))
}

[test]
def test_prejoin_where_named_emits_t0_col(t : T?) {
    // Core: the named-param pre-join where must qualify as `t0.did` (identical to the `_` placeholder form).
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _where($(c : Car) => c.did >= 20)
                          |> _join(_db |> select_from(type<Dealer>),
                                   $(c : Car, d : Dealer) => c.did == d.id,
                                   $(c : Car, d : Dealer) => (N = c.name, City = d.city)))
    t |> equal(sql,
        "SELECT \"t0\".\"name\", \"t1\".\"city\" FROM \"Cars\" AS \"t0\" INNER JOIN \"Dealers\" AS \"t1\" ON \"t0\".\"did\" = \"t1\".\"id\" WHERE \"t0\".\"did\" >= ?",
        "named-param _where before _join resolves to t0.did (was error[30838])")
}

[test]
def test_prejoin_where_named_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<Car>)
                           |> _where($(c : Car) => c.did >= 20)
                           |> _join(db |> select_from(type<Dealer>),
                                    $(c : Car, d : Dealer) => c.did == d.id,
                                    $(c : Car, d : Dealer) => (N = c.name, City = d.city)))
        // did >= 20 keeps c(20→LA) and d(30→NY); a(10) filtered before the join.
        t |> equal(length(rows), 2, "pre-join where filters srcA before the join")
        var cities = ""
        for (r in rows) {
            cities += r.City
        }
        t |> equal(cities, "LANY", "remaining rows join to LA and NY")
    }
}

[test]
def test_prejoin_where_named_jsonpath(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // JSON-path predicate on the named param (mirror site 2) before the join.
        let rows <- _sql(db |> select_from(type<Car>)
                           |> _where($(c : Car) => c.meta.color == "red")
                           |> _join(db |> select_from(type<Dealer>),
                                    $(c : Car, d : Dealer) => c.did == d.id,
                                    $(c : Car, d : Dealer) => (N = c.name, City = d.city)))
        // red = a(10→NY) and d(30→NY); blue c(20) filtered.
        t |> equal(length(rows), 2, "json-path pre-join where filters by nested field")
        for (r in rows) {
            t |> equal(r.City, "NY", "both red cars join to NY dealers")
        }
    }
}

[test]
def test_prejoin_order_by_named_string_key_emits_computed(t : T?) {
    // Mirror site 4 (mentions_source_arg): a row-referencing STRING key in a pre-join `_order_by` with a
    // named param must route to the computed-key path (`LOWER("t0"."name")`), NOT the dynamic-column-NAME
    // path. Pre-fix, mentions_source_arg missed the named receiver `c` → mis-routed to `ORDER BY <runtime>`.
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                          |> _order_by($(c : Car) => to_lower(c.name))
                          |> _join(_db |> select_from(type<Dealer>),
                                   $(c : Car, d : Dealer) => c.did == d.id,
                                   $(c : Car, d : Dealer) => (N = c.name, City = d.city)))
    t |> equal(sql,
        "SELECT \"t0\".\"name\", \"t1\".\"city\" FROM \"Cars\" AS \"t0\" INNER JOIN \"Dealers\" AS \"t1\" ON \"t0\".\"did\" = \"t1\".\"id\" ORDER BY LOWER(\"t0\".\"name\") ASC",
        "named-param pre-join _order_by(to_lower(c.name)) lowers to ORDER BY LOWER(t0.name)")
}

[test]
def test_prejoin_where_named_not_in(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // _not_in subquery in the named-param pre-join where (mirror site 3 — nullable-probe).
        let rows <- _sql(db |> select_from(type<Car>)
                           |> _where($(c : Car) => c.did |> _not_in(db |> select_from(type<Dealer>)
                                                                       |> _where(_.city == "LA")
                                                                       |> _select(_.id)))
                           |> _join(db |> select_from(type<Dealer>),
                                    $(c : Car, d : Dealer) => c.did == d.id,
                                    $(c : Car, d : Dealer) => (N = c.name, City = d.city)))
        // LA dealer id = 20, so did NOT IN {20} keeps a(10→NY) and d(30→NY).
        t |> equal(length(rows), 2, "not_in pre-join where excludes LA-dealer cars")
    }
}
