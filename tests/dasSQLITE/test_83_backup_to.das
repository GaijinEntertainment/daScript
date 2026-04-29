options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

require daslib/fio

[sql_table(name = "Items")]
struct Item {
    @sql_primary_key Id : int
    Name : string
    Qty : int
}

def fixture(db : SqlRunner) {
    db |> create_table(type<Item>)
    db |> insert(Item(Id = 1, Name = "alpha", Qty = 10))
    db |> insert(Item(Id = 2, Name = "beta",  Qty = 20))
    db |> insert(Item(Id = 3, Name = "gamma", Qty = 30))
}

[test]
def test_backup_to_memory_runner(t : T?) {
    with_sqlite(":memory:") $(src) {
        fixture(src)
        with_sqlite(":memory:") $(dst) {
            src |> backup_to(dst)
            let n = dst |> query_scalar("SELECT COUNT(*) FROM Items", type<int>)
            t |> equal(n, 3, "snapshot row count")
            let names <- dst |> select_from(type<Item>)
            t |> equal(length(names), 3, "snapshot rows materialize")
            t |> equal(names[1].Name, "beta", "row content preserved")
        }
    }
}

[test]
def test_backup_to_path_round_trip(t : T?) {
    let r = create_temp_file_result("dastest_backup", ".db")
    t |> equal(r is value, true, "temp file path obtained")
    let path = unsafe(r.value)
    // create_temp_file_result already created the file as empty; remove
    // it so sqlite3_open creates a fresh database for the backup.
    remove(path)
    with_sqlite(":memory:") $(src) {
        fixture(src)
        src |> backup_to(path)
    }
    t |> equal(fexist(path), true, "snapshot file written")
    {
        var inscope reopened = open_sqlite(path)
        let qty_total = reopened |> query_scalar("SELECT SUM(Qty) FROM Items", type<int>)
        t |> equal(qty_total, 60, "10 + 20 + 30 = 60")
        let r3 = reopened |> query_one("SELECT * FROM Items WHERE Id = 3", type<Item>)
        t |> equal(r3.Name, "gamma", "row 3 round-tripped")
    }
    remove(path)
}

[test]
def test_try_backup_to_unwritable_path_returns_err(t : T?) {
    // try_backup_to(string) used to call open_sqlite(...) which panics on
    // a path SQLite can't open; that broke the try_* contract. After the
    // fix it threads through try_open_sqlite and surfaces the error as
    // some(errmsg) for the caller to recover from.
    with_sqlite(":memory:") $(src) {
        fixture(src)
        // A directory that doesn't exist — sqlite3_open returns an error
        // rather than creating intermediate dirs.
        let bogus = "/this/path/should/not/exist/snapshot.db"
        let err = src |> try_backup_to(bogus)
        t |> equal(err |> is_some, true, "open failure surfaces as Err, not panic")
    }
}

[test]
def test_backup_overwrites_existing_destination_runner(t : T?) {
    // sqlite3_backup_run replaces the destination wholesale.
    with_sqlite(":memory:") $(src) {
        fixture(src)
        with_sqlite(":memory:") $(dst) {
            dst |> create_table(type<Item>)
            dst |> insert(Item(Id = 99, Name = "stale", Qty = 0))
            src |> backup_to(dst)
            let n = dst |> query_scalar("SELECT COUNT(*) FROM Items", type<int>)
            t |> equal(n, 3, "stale row gone, src rows now present")
            let stale <- dst |> try_query_one("SELECT * FROM Items WHERE Id = 99", type<Item>)
            t |> equal(stale |> is_err, true, "stale row 99 not in snapshot")
        }
    }
}
