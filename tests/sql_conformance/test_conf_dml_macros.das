options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false


// Contract area: predicate-form write macros — _sql_update / _sql_delete / _sql_upsert,
// their returning variants (CONF_HAS_RETURNING), and rows-affected reporting.

require dastest/testing_boost public

require _conformance_provider

[sql_table(name = "ConfHits"),
 sql_index(fields = "Word", unique = true)]

struct ConfHit {
    @sql_primary_key Id : int
    Word : string
    Hits : int
    Active : bool
}

def private seed(db) {
    db |> create_table(type<ConfHit>)
    db |> insert([
        ConfHit(Id = 1, Word = "hello", Hits = 1, Active = true),
        ConfHit(Id = 2, Word = "world", Hits = 5, Active = true),
        ConfHit(Id = 3, Word = "stale", Hits = 0, Active = true)])
}

[test(description = "predicate update and delete report rows affected and an update can reference the existing row")]
def test_conf_sql_update_delete(t : T?) {
    t |> run("_sql_update by predicate reports rows affected") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let n = db |> _sql_update(type<ConfHit>, _.Hits == 0, (Active = false))
            t |> equal(n, 1)
            let inactive <- _sql(db |> select_from(type<ConfHit>) |> _where(!_.Active))
            t |> equal(length(inactive), 1)
            t |> equal(inactive[0].Word, "stale")
        }
    }
    t |> run("_sql_update SET can reference the existing row") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let n = db |> _sql_update(type<ConfHit>, _.Word == "world", (Hits = _.Hits + 10))
            t |> equal(n, 1)
            let w = _sql(db |> select_from(type<ConfHit>) |> _where(_.Word == "world") |> _first())
            t |> equal(w.Hits, 15)
        }
    }
    t |> run("_sql_delete by predicate") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let n = db |> _sql_delete(type<ConfHit>, _.Hits < 2)
            t |> equal(n, 2)
            t |> equal(_sql(db |> select_from(type<ConfHit>) |> count()), 1)
        }
    }
}

[test(description = "upsert inserts on a miss, merges on a conflict and auto-assigns an unset integer primary key")]
def test_conf_sql_upsert(t : T?) {
    t |> run("insert-on-miss, merge-on-conflict") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            // Conflict: Id=1 exists — SET merges old row (_) and proposed row (_excluded).
            let n1 = db |> _sql_upsert(
                ConfHit(Id = 1, Word = "hello", Hits = 1, Active = true),
                _.Id,
                (Hits = _.Hits + 1))
            t |> equal(n1, 1)
            let bumped = _sql(db |> select_from(type<ConfHit>) |> _where(_.Id == 1) |> _first())
            t |> equal(bumped.Hits, 2)
            // Miss: Id=9 inserts fresh.
            db |> _sql_upsert(
                ConfHit(Id = 9, Word = "new", Hits = 7, Active = true),
                _.Id,
                (Hits = _excluded.Hits))
            t |> equal(_sql(db |> select_from(type<ConfHit>) |> count()), 4)
        }
    }
    t |> run("unset integer primary key is auto-assigned") @(t : T?) {
        with_conf_db() $(db) {
            db |> create_table(type<ConfHit>)
            db |> _sql_upsert(
                ConfHit(Id = 0, Word = "first", Hits = 1, Active = true),
                _.Word,
                (Hits = _excluded.Hits))
            db |> _sql_upsert(
                ConfHit(Id = 0, Word = "second", Hits = 2, Active = true),
                _.Word,
                (Hits = _excluded.Hits))
            db |> _sql_upsert(
                ConfHit(Id = 0, Word = "first", Hits = 7, Active = true),
                _.Word,
                (Hits = _excluded.Hits))
            let tried = db |> _sql_try_upsert(
                ConfHit(Id = 0, Word = "third", Hits = 3, Active = true),
                _.Word,
                (Hits = _excluded.Hits))
            t |> success(tried |> is_ok)
            t |> equal(tried |> unwrap, 1)
            let rows <- _sql(db |> select_from(type<ConfHit>) |> _order_by(_.Id))
            t |> equal(length(rows), 3)
            t |> success(rows[0].Id != 0 && rows[1].Id != 0 && rows[2].Id != 0)
            t |> success(rows[0].Id != rows[1].Id && rows[1].Id != rows[2].Id)
            t |> equal(rows[0].Hits, 7)
            t |> equal(rows[1].Hits, 2)
            t |> equal(rows[2].Hits, 3)
        }
    }
}

[test(description = "update, delete and upsert with returning give back the affected rows and the assigned key")]
def test_conf_returning(t : T?) {
    if (!CONF_HAS_RETURNING) {
        return
    }
    t |> run("_sql_update_returning captures post-update rows") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let revived <- db |> _sql_update_returning(type<ConfHit>,
                _.Word == "stale",
                (Hits = 100))
            t |> equal(length(revived), 1)
            t |> equal(revived[0].Hits, 100)
        }
    }
    t |> run("_sql_delete_returning captures the deleted rows") @(t : T?) {
        with_conf_db() $(db) {
            seed(db)
            let gone <- db |> _sql_delete_returning(type<ConfHit>, _.Hits >= 5)
            t |> equal(length(gone), 1)
            t |> equal(gone[0].Word, "world")
        }
    }
    t |> run("upsert returning auto-assigns an unset integer primary key") @(t : T?) {
        with_conf_db() $(db) {
            db |> create_table(type<ConfHit>)
            let inserted <- db |> _sql_upsert_returning(
                ConfHit(Id = 0, Word = "returning", Hits = 4, Active = true),
                _.Word,
                (Hits = _excluded.Hits))
            t |> equal(length(inserted), 1)
            t |> success(inserted[0].Id != 0)
            let tried <- db |> _sql_try_upsert_returning(
                ConfHit(Id = 0, Word = "try-returning", Hits = 5, Active = true),
                _.Word,
                (Hits = _excluded.Hits))
            t |> success(tried |> is_ok)
            let tried_rows <- tried |> unwrap
            t |> equal(length(tried_rows), 1)
            t |> success(tried_rows[0].Id != 0)
            t |> success(tried_rows[0].Id != inserted[0].Id)
        }
    }
}
