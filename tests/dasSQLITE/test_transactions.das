options gen2

require dastest/testing_boost public
require daslib/sql
require sqlite/sqlite_boost

[sql_table(name = "Friends")]
struct Friend {
    @sql_primary_key Id : int
    Name : string
}

[test(description = "with_transaction commits every statement of its block when the block exits normally")]
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

[test(description = "in_transaction is false in autocommit, true inside with_transaction and false again after it")]
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

[test(description = "with_transaction in immediate mode commits cleanly")]
def test_with_transaction_immediate_mode(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Friend>)
        db |> with_transaction(SqliteTxnMode.Immediate) {
            db |> insert(Friend(Id = 7, Name = "imm"))
        }
        t |> equal(length(db |> select_from(type<Friend>)), 1, "Immediate-mode txn commits cleanly")
    }
}

[test(description = "a nested with_transaction runs as a savepoint inside the outer transaction and all work commits together")]
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

[sql_table(name = "Acct")]
struct Acct {
    @sql_primary_key Id : int
    Balance : int
}

[test(description = "try_transaction returns none and commits when the block succeeds")]
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

[test(description = "a try_insert that fails inside try_transaction surfaces as an error in the block while the transaction itself still commits")]
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

[test(description = "try_transaction in immediate mode commits cleanly")]
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

[test(description = "a transaction block that returns an error rolls back and surfaces that error, and one that returns none commits")]
def transaction_result_commits_or_rolls_back(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> exec("CREATE TABLE values_table (id INTEGER PRIMARY KEY)")
        let rejected = db |> try_transaction() $() : SqlError {
            let write_error = db |> try_exec("INSERT INTO values_table VALUES (1)")
            if (write_error |> is_some) { return write_error }
            return some("explicit rejection")
        }
        t |> equal(rejected |> unwrap, "explicit rejection")
        t |> equal(db |> query_scalar("SELECT count(*) FROM values_table", type<int>), 0)
        t |> success(!in_transaction(db))
        let committed = db |> try_transaction(SqliteTxnMode.Immediate) $() : SqlError {
            return db |> try_exec("INSERT INTO values_table VALUES (2)")
        }
        t |> success(committed |> is_none)
        t |> equal(db |> query_scalar("SELECT id FROM values_table", type<int>), 2)
    }
}

[test(description = "a SQL failure inside a result transaction is returned as an error and leaves no partial commit")]
def transaction_result_propagates_sql_failure_without_partial_commit(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> exec("CREATE TABLE values_table (id INTEGER PRIMARY KEY)")
        let failed = db |> try_transaction() $() : SqlError {
            let first = db |> try_exec("INSERT INTO values_table VALUES (1)")
            if (first |> is_some) { return first }
            return db |> try_exec("INSERT INTO values_table VALUES (1)")
        }
        t |> success(failed |> is_some)
        t |> equal(db |> query_scalar("SELECT count(*) FROM values_table", type<int>), 0)
        t |> success(!in_transaction(db))
    }
}

[test(description = "rejecting a nested result transaction rolls back only the inner work and the outer transaction still commits")]
def transaction_result_nested_rejection_preserves_outer_work(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> exec("CREATE TABLE values_table (id INTEGER PRIMARY KEY)")
        let outer = db |> try_transaction() $() : SqlError {
            let first = db |> try_exec("INSERT INTO values_table VALUES (1)")
            if (first |> is_some) { return first }
            let inner = db |> try_transaction() $() : SqlError {
                let written = db |> try_exec("INSERT INTO values_table VALUES (2)")
                if (written |> is_some) { return written }
                return some("reject inner")
            }
            t |> equal(inner |> unwrap, "reject inner")
            t |> success(in_transaction(db))
            return db |> try_exec("INSERT INTO values_table VALUES (3)")
        }
        t |> success(outer |> is_none)
        t |> equal(db |> query_scalar("SELECT count(*) FROM values_table", type<int>), 2)
        t |> equal(db |> query_scalar("SELECT count(*) FROM values_table WHERE id=2", type<int>), 0)
        t |> success(!in_transaction(db))
    }
}

[test(description = "a deferred constraint that fails at commit is returned as an error and rolls the whole transaction back")]
def transaction_result_commit_constraint_failure_rolls_back(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> exec("PRAGMA foreign_keys=ON")
        db |> exec("CREATE TABLE parent (id INTEGER PRIMARY KEY)")
        db |> exec("CREATE TABLE child (parent_id INTEGER REFERENCES parent(id) DEFERRABLE INITIALLY DEFERRED)")
        let failed = db |> try_transaction() $() : SqlError {
            return db |> try_exec("INSERT INTO child VALUES (42)")
        }
        t |> success(failed |> is_some, "constraint is checked at commit, after callback succeeded")
        t |> success(!in_transaction(db))
        t |> equal(db |> query_scalar("SELECT count(*) FROM child", type<int>), 0)
        let next = db |> try_transaction() $() : SqlError {
            return db |> try_exec("INSERT INTO parent VALUES (42)")
        }
        t |> success(next |> is_none)
        t |> equal(db |> query_scalar("SELECT count(*) FROM parent", type<int>), 1)
    }
}

[test(description = "a transaction callback that returns early still commits what ran before the return")]
def void_transaction_callback_can_return_early(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> exec("CREATE TABLE values_table (id INTEGER PRIMARY KEY)")
        var after_return = false
        let result = db |> try_transaction() $ {
            db |> exec("INSERT INTO values_table VALUES (1)")
            if (db |> query_scalar("SELECT count(*) FROM values_table", type<int>) == 1) { return }
            after_return = true
        }
        t |> success(result |> is_none)
        t |> success(!after_return)
        t |> success(!in_transaction(db))
        t |> equal(db |> query_scalar("SELECT count(*) FROM values_table", type<int>), 1)
    }
}
