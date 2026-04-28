options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost

[sql_table(name = "Acct")]
struct Acct {
    @sql_primary_key Id : int
    Balance : int
}

[test]
def test_try_transaction_ok_path(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Acct>)
        let r = db |> try_transaction() {
            db |> insert(Acct(Id = 1, Balance = 100))
            db |> insert(Acct(Id = 2, Balance = 200))
        }
        t |> equal(r |> is_none(), true, "try_transaction returns none on success")
        t |> equal(length(db |> select_from(type<Acct>)), 2, "both rows committed")
    }
}

[test]
def test_try_transaction_pk_conflict_returns_err(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Acct>)
        db |> insert(Acct(Id = 1, Balance = 100))
        // Use try_insert inside the txn so a PK clash surfaces as an Err
        // *inside the block* rather than panicking. The block returns
        // normally, so try_transaction COMMITs whatever the block did
        // (which was to fail try_insert and not insert anything).
        let r = db |> try_transaction() {
            let ins = db |> try_insert(Acct(Id = 1, Balance = 999))
            // ignore the err; the transaction itself succeeds
            t |> equal(ins |> is_err(), true, "PK collision is caught by try_insert inside txn")
        }
        t |> equal(r |> is_none(), true, "try_transaction commits even when block-internal SQL failed")
        let rows <- db |> select_from(type<Acct>)
        t |> equal(length(rows), 1, "row count unchanged (collision didn't insert)")
        t |> equal(rows[0].Balance, 100, "original Balance preserved")
    }
}

[test]
def test_try_transaction_immediate_mode(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Acct>)
        let r = db |> try_transaction(SqliteTxnMode.Immediate) {
            db |> insert(Acct(Id = 1, Balance = 50))
        }
        t |> equal(r |> is_none(), true, "Immediate-mode try_transaction commits cleanly")
        t |> equal(length(db |> select_from(type<Acct>)), 1, "row inserted")
    }
}
