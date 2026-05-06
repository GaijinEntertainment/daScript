options gen2

require dastest/testing_boost public
require sqlite/sqlite_migrate

// Identifier injection guard: every runtime ``name : string`` overload
// (create_table / insert / select_from / drop / update / delete + the
// migrate path) must route through sql_quote_id so embedded `"` is
// doubled. Without escaping, a `"` in name would close the identifier
// and execute attacker-controlled SQL through sqlite3_exec.

[sql_table(name = "users")]
struct User {
    @sql_primary_key Id : int
    Name : string
}

[test]
def test_hostile_name_does_not_inject(t : T?) {
    var inscope db = open_sqlite(":memory:")
    let hostile = "evil\" (Injected INTEGER); CREATE TABLE hacked(z TEXT); --"
    db |> create_table(type<User>, hostile)
    let landed = User(Name = "alice")
    let id = db |> insert(landed, hostile)
    t |> success(id > 0l, "insert into hostile-named table succeeded")
    let n_landed = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name={sql_quote_lit(hostile)}",
        type<int>)
    t |> equal(n_landed, 1, "hostile name landed verbatim as a table identifier")
    let n_hacked = db |> query_scalar(
        "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='hacked'",
        type<int>)
    t |> equal(n_hacked, 0, "no injected `hacked` table was created")
}
