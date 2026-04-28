options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost

[sql_table(name = "Friends")]
struct Friend {
    @sql_primary_key Id : int
    Name : string
}

[test]
def test_with_transaction_commits_on_normal_exit(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Friend>)
        db |> with_transaction() {
            db |> insert(Friend(Id = 1, Name = "tom"))
            db |> insert(Friend(Id = 2, Name = "jerry"))
        }
        let rows <- db |> select_from(type<Friend>)
        t |> equal(length(rows), 2, "both inserts visible after commit")
    }
}

[test]
def test_in_transaction_status_flips(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Friend>)
        t |> equal(db |> in_transaction(), false, "default state is autocommit (not in txn)")
        var inside_flag = false
        db |> with_transaction() {
            inside_flag = db |> in_transaction()
            db |> insert(Friend(Id = 1, Name = "x"))
        }
        t |> equal(inside_flag, true, "in_transaction() returns true inside with_transaction")
        t |> equal(db |> in_transaction(), false, "reverts to autocommit after exit")
    }
}

[test]
def test_with_transaction_immediate_mode(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Friend>)
        db |> with_transaction(SqliteTxnMode.Immediate) {
            db |> insert(Friend(Id = 7, Name = "imm"))
        }
        t |> equal(length(db |> select_from(type<Friend>)), 1, "Immediate-mode txn commits cleanly")
    }
}

[test]
def test_with_transaction_nesting_via_savepoint(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Friend>)
        db |> with_transaction() {
            db |> insert(Friend(Id = 1, Name = "outer"))
            db |> with_transaction() {
                db |> insert(Friend(Id = 2, Name = "inner"))
                t |> equal(db |> in_transaction(), true, "nested call is still in-txn")
            }
            db |> insert(Friend(Id = 3, Name = "outer2"))
        }
        let rows <- db |> select_from(type<Friend>)
        t |> equal(length(rows), 3, "all 3 inserts (outer + inner savepoint + outer) commit")
    }
}
