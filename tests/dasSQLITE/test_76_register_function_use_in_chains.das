options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Registered functions should be usable in WHERE / ORDER BY / projection
// just like a built-in scalar. We verify that a deterministic UDF embeds
// into common chain shapes.

[sql_table(name = "Items")]
struct Item {
    @sql_primary_key Id : int
    Name : string
    Score : int
}

def fixture(db : SqlRunner) {
    db |> create_table(type<Item>)
    db |> insert(Item(Id = 1, Name = "alpha", Score = 10))
    db |> insert(Item(Id = 2, Name = "beta",  Score = 30))
    db |> insert(Item(Id = 3, Name = "gamma", Score = 20))
    db |> insert(Item(Id = 4, Name = "delta", Score = 50))
}

def doubler(x : int) : int {
    return x * 2
}

def add_prefix(s : string) : string {
    return "X_" + s
}

[test]
def test_udf_in_where(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        db |> register_function("doubler", @@doubler, true)
        // doubler(Score) > 30 means Score > 15 → Items 2,3,4 match.
        let n = db |> query_scalar("SELECT COUNT(*) FROM Items WHERE doubler(Score) > 30", type<int>)
        t |> equal(n, 3, "doubler(Score) > 30 — 3 rows")
    }
}

[test]
def test_udf_in_order_by(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        db |> register_function("doubler", @@doubler, true)
        // ORDER BY doubler(Score) DESC ⇒ items by Score desc: 50, 30, 20, 10
        let top = db |> query_scalar(
            "SELECT Name FROM Items ORDER BY doubler(Score) DESC LIMIT 1",
            type<string>)
        t |> equal(top, "delta", "highest score is delta")
    }
}

[test]
def test_udf_string_in_projection(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        db |> register_function("add_prefix", @@add_prefix, true)
        let v = db |> query_scalar("SELECT add_prefix(Name) FROM Items WHERE Id = 1", type<string>)
        t |> equal(v, "X_alpha", "prefix prepended")
    }
}
