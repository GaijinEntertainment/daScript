options gen2
require dastest/testing_boost public
require sqlite/sqlite_boost

[test]
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

[test]
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

[test]
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

[test]
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

[test]
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
