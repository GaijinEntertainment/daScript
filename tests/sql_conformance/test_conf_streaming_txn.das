options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false


// Contract area: _each_sql streaming (iterator lifecycle incl. early break, then the
// connection stays usable) and transactions (commit, savepoint nesting, try_transaction).

require dastest/testing_boost public

require _conformance_provider

[sql_table(name = "ConfLog")]
struct ConfLogRow {
    @sql_primary_key Id : int
    Severity : int
}

def private seed(db) {
    db |> create_table(type<ConfLogRow>)
    var rows : array<ConfLogRow>
    rows |> reserve(10)
    for (i in range(10)) {
        rows |> push(ConfLogRow(Id = 0, Severity = i))
    }
    db |> insert(rows)
}

[test(description = "each_sql streams filtered rows in order and an early break leaves the connection writable", standalone = "a provider copies this directory and drops files per capability; the file is the unit")]
def test_conf_each_sql(t : T?) {
    t |> run("streams filtered rows in order") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            var seen = 0
            var last = -1
            for (e in _each_sql(db |> select_from(type<ConfLogRow>)
                    |> _where(_.Severity >= 5) |> _order_by(_.Severity))) {
                seen++
                last = e.Severity
            }
            t |> equal(seen, 5)
            t |> equal(last, 9)
        }
    }
    t |> run("early break finalizes; connection stays writable") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            var seen = 0
            for (_e in _each_sql(db |> select_from(type<ConfLogRow>))) {
                seen++
                break if (seen == 3)
            }
            t |> equal(seen, 3)
            db |> insert(ConfLogRow(Id = 0, Severity = 99))   // must not be blocked by a leaked cursor
            t |> equal(_sql(db |> select_from(type<ConfLogRow>) |> count()), 11)
        }
    }
}

[test(description = "with_transaction commits, nested transactions compose as savepoints and try_transaction returns none on success")]
def test_conf_transactions(t : T?) {
    t |> run("with_transaction commits on normal exit") @(t : T?) {
        with_conf_db() $(db) {
            db |> create_table(type<ConfLogRow>)
            db |> with_transaction() {
                db |> insert(ConfLogRow(Id = 0, Severity = 1))
                db |> insert(ConfLogRow(Id = 0, Severity = 2))
            }
            t |> equal(_sql(db |> select_from(type<ConfLogRow>) |> count()), 2)
        }
    }
    t |> run("nested with_transaction composes (savepoint)") @(t : T?) {
        with_conf_db() $(db) {
            db |> create_table(type<ConfLogRow>)
            db |> with_transaction() {
                db |> insert(ConfLogRow(Id = 0, Severity = 1))
                db |> with_transaction() {
                    db |> insert(ConfLogRow(Id = 0, Severity = 2))
                }
                db |> insert(ConfLogRow(Id = 0, Severity = 3))
            }
            t |> equal(_sql(db |> select_from(type<ConfLogRow>) |> count()), 3)
        }
    }
    t |> run("try_transaction returns none on success") @(t : T?) {
        with_conf_db() $(db) {
            db |> create_table(type<ConfLogRow>)
            let r = db |> try_transaction() {
                db |> insert(ConfLogRow(Id = 0, Severity = 3))
            }
            t |> success(r |> is_none, "clean txn reports no error")
            t |> equal(_sql(db |> select_from(type<ConfLogRow>) |> count()), 1)
        }
    }
}
