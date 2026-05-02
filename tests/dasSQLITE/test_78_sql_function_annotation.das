options gen2

require dastest/testing_boost public

require math       // pulls math::normalize(float2/3/4) into scope — collision regression for `normalize` below
require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// [sql_function] auto-registers the function against every opened SQL connection
// and makes it visible to the `_sql` chain analyzer (rendered as `name(args)` SQL).

[sql_function]
def my_upper(s : string) : string => s |> to_upper

[sql_function]
def damage(base, mult : int) : int => base * mult

[sql_function(name="sql_log")]
def log_event(msg : string) : int {
    print("[log_event] {msg}\n")
    return length(msg)
}

[sql_function(deterministic=true)]
def double_it(x : double) : double => x * 2.0lf

[sql_function(directonly=true)]
def directonly_echo(s : string) : string => s

// Same SQL name as `math::normalize(float2)` — exercises the explicit
// `funcType` on `ExprAddr` in the macro emit. Without that disambiguation,
// the registration thunk would fail to resolve `@@normalize` (ambiguous).
[sql_function]
def normalize(s : string) : string => s |> to_upper

[sql_table(name="Items")]
struct Item {
    @sql_primary_key Id : int
    Name : string
    Qty  : int
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<Item>)
    db |> insert(Item(Id = 1, Name = "apple",  Qty = 3))
    db |> insert(Item(Id = 2, Name = "Banana", Qty = 5))
    db |> insert(Item(Id = 3, Name = "cherry", Qty = 7))
}

[test]
def test_auto_registered_raw_sql(t : T?) {
    // No manual register_function call — open_sqlite's installer hook does it.
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT my_upper('hello')", type<string>)
        t |> equal(v, "HELLO", "auto-registered my_upper via raw SQL")
    }
}

[test]
def test_chain_visibility(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<Item>) |> _where(my_upper(_.Name) == "BANANA") |> _to_array)
        t |> equal(length(rows), 1, "1 row matched WHERE my_upper(Name)='BANANA'")
        t |> equal(rows[0].Name, "Banana", "row 0 Name")
    }
}

[test]
def test_chain_visibility_two_args(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // damage(base, mult) emitted as SQL: WHERE damage("Qty", 2) >= 10
        let rows <- _sql(db |> select_from(type<Item>) |> _where(damage(_.Qty, 2) >= 10) |> _to_array)
        t |> equal(length(rows), 2, "Qty * 2 >= 10 → Banana(10), cherry(14)")
    }
}

[test]
def test_name_override(t : T?) {
    with_sqlite(":memory:") $(db) {
        // log_event was registered as "sql_log" via [sql_function(name="sql_log")]
        let v = db |> query_scalar("SELECT sql_log('msg-from-test')", type<int>)
        t |> equal(v, 13, "sql_log returns length('msg-from-test')")
    }
}

[test]
def test_name_override_in_chain(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // Calling log_event in a chain should emit sql_log(...) (the override name).
        let rows <- _sql(db |> select_from(type<Item>) |> _where(log_event(_.Name) >= 6) |> _to_array)
        t |> equal(length(rows), 2, "Banana(6) + cherry(6) → length >= 6")
    }
}

[test]
def test_deterministic_accepted(t : T?) {
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT double_it(3.5)", type<double>)
        t |> equal(v, 7.0lf, "deterministic=true accepted; double_it works")
    }
}

[test]
def test_directonly_works_in_raw_sql(t : T?) {
    // directonly=true is enforced by SQLite for views/triggers/CHECK only —
    // direct SELECT/UPDATE/etc. against the connection still work.
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT directonly_echo('hello')", type<string>)
        t |> equal(v, "hello", "directonly=true UDF callable from application SQL")
    }
}

[test]
def test_directonly_works_in_chain(t : T?) {
    // Chain context isn't `_create_view` — directonly UDF is fine here too.
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- _sql(db |> select_from(type<Item>) |> _where(directonly_echo(_.Name) == "Banana") |> _to_array)
        t |> equal(length(rows), 1, "directonly UDF usable in _sql chain")
    }
}

[test]
def test_name_collision_with_imported_module(t : T?) {
    // `normalize` is also exported by `math` (with float2/3/4 signatures). The macro
    // emits an `ExprAddr` with explicit `funcType` so the typer resolves to OUR
    // `normalize(string)` instead of erroring on ambiguity.
    with_sqlite(":memory:") $(db) {
        let v = db |> query_scalar("SELECT normalize('hi')", type<string>)
        t |> equal(v, "HI", "user [sql_function] normalize coexists with math::normalize")
    }
}

[test]
def test_projection_single_column(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // [sql_function] in single-column projection — emits `my_upper("Name")` SQL-side.
        let rows <- _sql(db |> select_from(type<Item>) |> _order_by(_.Id) |> _select(my_upper(_.Name)) |> _to_array)
        t |> equal(length(rows), 3, "3 rows projected through my_upper")
        t |> equal(rows[0], "APPLE",  "my_upper(apple)")
        t |> equal(rows[1], "BANANA", "my_upper(Banana)")
        t |> equal(rows[2], "CHERRY", "my_upper(cherry)")
    }
}

[test]
def test_projection_single_column_emits_call(t : T?) {
    var _db = SqlRunner()
    let sql = _sql_text(_db |> select_from(type<Item>) |> _select(my_upper(_.Name)))
    t |> equal(sql,
        "SELECT my_upper(\"Name\") FROM \"Items\"",
        "single-column [sql_function] projection emits the call verbatim")
}

[test]
def test_projection_named_tuple_mixed(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // Named tuple mixing a [sql_function] computed slot with a plain column ref.
        let rows <- _sql(db |> select_from(type<Item>)
                           |> _order_by(_.Id)
                           |> _select((Loud = my_upper(_.Name), Id = _.Id))
                           |> _to_array)
        t |> equal(length(rows), 3, "3 rows in mixed named tuple")
        t |> equal(rows[1].Loud, "BANANA", "computed Loud slot")
        t |> equal(rows[1].Id,   2,        "plain Id slot")
    }
}

[test]
def test_projection_named_tuple_two_args(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // Two-arg [sql_function] in tuple slot — args may themselves be column refs.
        let rows <- _sql(db |> select_from(type<Item>)
                           |> _order_by(_.Id)
                           |> _select((Id = _.Id, Boosted = damage(_.Qty, 3)))
                           |> _to_array)
        t |> equal(length(rows), 3, "3 rows")
        t |> equal(rows[0].Boosted, 9,  "damage(3, 3)")
        t |> equal(rows[1].Boosted, 15, "damage(5, 3)")
        t |> equal(rows[2].Boosted, 21, "damage(7, 3)")
    }
}

[test]
def test_multiple_opened_dbs(t : T?) {
    with_sqlite(":memory:") $(db1) {
        with_sqlite(":memory:") $(db2) {
            let v1 = db1 |> query_scalar("SELECT my_upper('one')", type<string>)
            let v2 = db2 |> query_scalar("SELECT my_upper('two')", type<string>)
            t |> equal(v1, "ONE", "db1 has my_upper")
            t |> equal(v2, "TWO", "db2 has my_upper")
        }
    }
}
