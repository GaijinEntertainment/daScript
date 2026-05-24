options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Brand : string
    Price : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Car>)
    for (i in range(20)) {
        db |> insert(Car(Id = i + 1, Brand = "B{i % 3}", Price = i * 10))
    }
}

[test]
def test_expr_key_modulo_emits_inline_constant(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>) |> _group_by(_.Price % 100) |> _select((K = _._0, N = _._1 |> length)))
    t |> equal(sql, "SELECT ((\"Price\") % (100)), COUNT(*) FROM \"Cars\" GROUP BY ((\"Price\") % (100))",
        "expr-key SELECT + GROUP BY share the same inlined fragment, no `?` binds")
}

[test]
def test_expr_key_modulo_runtime(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let groups <- _sql(db |> select_from(type<Car>) |> _group_by(_.Price % 100) |> _select((K = _._0, N = _._1 |> length)))
        t |> equal(length(groups), 10, "20 rows with Price 0,10,...,190 split by % 100 → 10 groups of 2")
        var seen = 0
        for (g in groups) {
            t |> equal(g.N, 2, "every group has 2 entries")
            seen++
        }
        t |> equal(seen, 10, "10 groups iterated")
    }
}

[test]
def test_expr_key_with_inner_select_sum(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let groups <- _sql(db |> select_from(type<Car>)
                              |> _group_by(_.Price % 100)
                              |> _select((K = _._0, S = _._1 |> select($(c : Car) => c.Price) |> sum())))
        t |> equal(length(groups), 10, "10 groups")
        for (g in groups) {
            t |> equal(g.S, g.K * 2 + 100, "K=0 → S=100, K=10 → S=120, etc.")
        }
    }
}

[test]
def test_expr_key_order_by_uses_same_fragment(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                           |> _group_by(_.Price % 100)
                           |> _select((K = _._0, N = _._1 |> length))
                           |> _order_by(_._0))
    t |> success(sql |> find("ORDER BY ((\"Price\") % (100)) ASC") >= 0,
        "ORDER BY on the group key re-uses the expression-key fragment verbatim")
}

[test]
def test_multi_key_mixed_field_and_expression(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Car>)
                           |> _group_by((_.Brand, _.Price % 100))
                           |> _select((B = _._0._0, K = _._0._1, N = _._1 |> length)))
    t |> equal(sql, "SELECT \"Brand\", ((\"Price\") % (100)), COUNT(*) FROM \"Cars\" GROUP BY \"Brand\", ((\"Price\") % (100))",
        "multi-key tuple mixes field key and expression key; both render in SELECT + GROUP BY in order")
}

[test]
def test_expr_key_runtime_runs_correctly(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let groups <- _sql(db |> select_from(type<Car>)
                              |> _group_by((_.Brand, _.Price % 100))
                              |> _select((B = _._0._0, K = _._0._1, N = _._1 |> length)))
        t |> success(!empty(groups), "non-empty multi-key result")
        var total = 0
        for (g in groups) {
            total += g.N
        }
        t |> equal(total, 20, "every row visits exactly one group")
    }
}
