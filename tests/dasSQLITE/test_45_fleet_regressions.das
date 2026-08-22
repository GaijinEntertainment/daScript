options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require sqlite/sqlite_boost
require daslib/sql_linq

[sql_table(name = "FleetThings")]
struct FleetThing {
    @sql_primary_key Id : int
    Price : int
}

[sql_table(name = "FleetSales")]
struct FleetSale {
    @sql_primary_key Id : int
    @sql_column = "amount_cents" Amount : int
    Region : string
}

[sql_table(name = "FleetKV")]
struct FleetKV {
    @sql_primary_key Id : int
    K : string
    V : int
}

def private fixture_things(db : SqlRunner) {
    db |> create_table(type<FleetThing>)
    db |> insert(FleetThing(Id = 1, Price = 90))
    db |> insert(FleetThing(Id = 2, Price = 190))
    db |> insert(FleetThing(Id = 3, Price = 290))
}

[test]
def test_skip_then_first_keeps_offset(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_things(db)
        let f = _sql(db |> select_from(type<FleetThing>) |> _order_by(_.Id) |> skip(1) |> _first())
        t |> equal(f.Id, 2, "skip(1) |> _first() emits LIMIT 1 OFFSET 1")
        let f2 = _sql(db |> select_from(type<FleetThing>) |> _order_by(_.Id) |> skip(2) |> _first())
        t |> equal(f2.Id, 3, "skip(2) |> _first() emits LIMIT 1 OFFSET 2")
    }
}

[test]
def test_not_in_computed_lhs_binds_once(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_things(db)
        let base = 10
        let bad = [100, 300]
        let rows <- _sql(db |> select_from(type<FleetThing>) |> _where((_.Price + base)._not_in(bad)))
        t |> equal(length(rows), 1, "computed-LHS _not_in filters instead of being neutered")
        t |> equal(rows[0].Id, 2)
    }
}

[test]
def test_grouped_aggregate_honors_sql_column_rename(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<FleetSale>)
        db |> insert(FleetSale(Id = 1, Amount = 100, Region = "n"))
        db |> insert(FleetSale(Id = 2, Amount = 250, Region = "n"))
        db |> insert(FleetSale(Id = 3, Amount = 50, Region = "s"))
        var rows <- _sql(db |> select_from(type<FleetSale>)
            |> _group_by(_.Region)
            |> _select((R = _._0, S = _._1 |> _select(_.Amount) |> sum())))
        var total = 0
        for (r in rows) {
            total += r.S
        }
        t |> equal(total, 400, "grouped sum reads the renamed column, not a string literal")
    }
}

[test]
def test_reversed_distinct_by_aggregates_keep_last(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<FleetKV>)
        db |> insert(FleetKV(Id = 1, K = "a", V = 10))
        db |> insert(FleetKV(Id = 2, K = "a", V = 20))
        db |> insert(FleetKV(Id = 3, K = "b", V = 30))
        let s = _sql(db |> select_from(type<FleetKV>) |> reverse() |> _distinct_by(_.K) |> _select(_.V) |> sum())
        t |> equal(s, 50, "reversed distinct_by sum keeps the MAX(pk) representative")
        let n = _sql(db |> select_from(type<FleetKV>) |> reverse() |> _distinct_by(_.K) |> _count(_.V > 15))
        t |> equal(n, 2, "reversed distinct_by count evaluates the predicate on the kept row")
    }
}

[test]
def test_set_op_post_operators_divert(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<FleetKV>)
        db |> insert(FleetKV(Id = 1, K = "a", V = 30))
        db |> insert(FleetKV(Id = 2, K = "b", V = 10))
        db |> insert(FleetKV(Id = 3, K = "c", V = 20))
        let filtered <- _sql((db |> select_from(type<FleetKV>) |> _where(_.V < 25))
            |> union(db |> select_from(type<FleetKV>) |> _where(_.V > 15))
            |> _where(_.Id > 1))
        t |> equal(length(filtered), 2, "post-union where filters the combined rows, not just the LHS")
        let ordered <- _sql((db |> select_from(type<FleetKV>) |> _where(_.V < 25))
            |> union(db |> select_from(type<FleetKV>) |> _where(_.V > 15))
            |> _order_by(_.V))
        t |> equal(length(ordered), 3)
        t |> equal(ordered[0].V, 10, "post-union order_by sorts the combined rows")
        t |> equal(ordered[2].V, 30)
        let taken <- _sql((db |> select_from(type<FleetKV>) |> _where(_.V < 25))
            |> union(db |> select_from(type<FleetKV>) |> _where(_.V > 15))
            |> _order_by(_.V) |> take(2))
        t |> equal(length(taken), 2, "take after a set-op is honored, not rejected")
        let f = _sql((db |> select_from(type<FleetKV>) |> _where(_.V < 25))
            |> union(db |> select_from(type<FleetKV>) |> _where(_.V > 15))
            |> _order_by(_.V) |> _first())
        t |> equal(f.V, 10, "first after a set-op reads the combined minimum")
    }
}

[sql_table(name = "FleetNames")]
struct FleetName {
    @sql_primary_key Id : int
    Name : string
}

[test]
def test_string_ops_case_sensitive_rail(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<FleetName>)
        db |> insert(FleetName(Id = 1, Name = "Lux"))
        db |> insert(FleetName(Id = 2, Name = "lux"))
        db |> insert(FleetName(Id = 3, Name = "de[lux]e*?"))
        let c1 <- _sql(db |> select_from(type<FleetName>) |> _where(contains(_.Name, "lux")))
        t |> equal(length(c1), 2, "contains is case-sensitive like compat mode")
        let c2 <- _sql(db |> select_from(type<FleetName>) |> _where(starts_with(_.Name, "Lux")))
        t |> equal(length(c2), 1, "starts_with is case-sensitive like compat mode")
        let c3 <- _sql(db |> select_from(type<FleetName>) |> _where(ends_with(_.Name, "e*?")))
        t |> equal(length(c3), 1, "GLOB metacharacters in the needle are escaped")
        let c4 <- _sql(db |> select_from(type<FleetName>) |> _where(contains(_.Name, "[lux]")))
        t |> equal(length(c4), 1, "bracket needle matches literally")
        let c5 <- _sql(db |> select_from(type<FleetName>) |> _where(contains(_.Name, "")))
        t |> equal(length(c5), 3, "empty needle matches every row")
    }
}

[sql_table(name = "FleetJoinB")]
struct FleetJoinB {
    @sql_primary_key Id : int
    K : int
    W : int
}

[sql_table(name = "FleetJoinA")]
struct FleetJoinA {
    @sql_primary_key Id : int
    K : int
    V : int
}

[test]
def test_join_take_aggregate_projects_the_join(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<FleetJoinA>)
        db |> create_table(type<FleetJoinB>)
        for (i in range(4)) {
            db |> insert(FleetJoinA(Id = i + 1, K = i + 1, V = 10 * (i + 1)))
            db |> insert(FleetJoinB(Id = 10 + i, K = i + 1, W = 100 * (i + 1)))
        }
        let n = _sql(db |> select_from(type<FleetJoinA>)
            |> _join(db |> select_from(type<FleetJoinB>), $(a, b) => a.K == b.K, $(a, b) => (V = a.V, W = b.W))
            |> take(2)
            |> count())
        t |> equal(n, 2, "the inner subquery keeps the join projection, so shared column names stay unambiguous")
    }
}

[test]
def test_take_zero_then_first_matches_plain(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture_things(db)
        let none = _sql(db |> select_from(type<FleetThing>) |> _order_by(_.Id) |> take(0) |> _first_opt())
        t |> success(none |> is_none, "take(0) |> _first_opt() yields none like plain")
        let one = _sql(db |> select_from(type<FleetThing>) |> _order_by(_.Id) |> take(1) |> _first_opt())
        t |> success(one |> is_some, "take(1) |> _first_opt() still finds the row")
        var didPanic = false
        try {
            let f = _sql(db |> select_from(type<FleetThing>) |> _order_by(_.Id) |> take(0) |> _first())
            t |> success(false, "expected panic; got Id {f.Id}")
        } recover {
            didPanic = true
        }
        t |> success(didPanic, "take(0) |> _first() panics like plain")
    }
}
