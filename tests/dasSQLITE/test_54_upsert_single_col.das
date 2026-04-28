options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_table(name = "WordHits")]
struct WordHit {
    @sql_primary_key Id : int
    Word : string
    Hits : int
    Last : int64
}

def private fixture(db : SqlRunner) {
    db |> create_table(type<WordHit>)
    db |> insert(WordHit(Id = 1, Word = "hello", Hits = 1, Last = 100l))
}

[test]
def test_upsert_inserts_when_no_conflict(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> _sql_upsert(
            WordHit(Id = 2, Word = "world", Hits = 1, Last = 200l),
            _.Id,
            (Hits = _.Hits + 1, Last = _excluded.Last))
        t |> equal(n, 1, "fresh insert returns 1")
        let world_hits = db |> query_scalar("SELECT Hits FROM WordHits WHERE Id = 2", type<int>)
        t |> equal(world_hits, 1, "fresh row used the INSERT values, not DO UPDATE")
    }
}

[test]
def test_upsert_merges_on_pk_collision(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let n = db |> _sql_upsert(
            WordHit(Id = 1, Word = "ignored", Hits = 99, Last = 300l),
            _.Id,
            (Hits = _.Hits + 1, Last = _excluded.Last))
        t |> equal(n, 1, "merge counts 1 row affected")
        let h = db |> query_scalar("SELECT Hits FROM WordHits WHERE Id = 1", type<int>)
        t |> equal(h, 2, "Hits incremented from existing 1 to 2")
        let last = db |> query_scalar("SELECT Last FROM WordHits WHERE Id = 1", type<int64>)
        t |> equal(last, 300l, "Last was updated from _excluded.Last (300)")
        let word = db |> query_scalar("SELECT Word FROM WordHits WHERE Id = 1", type<string>)
        t |> equal(word, "hello", "Word column unchanged because do_update doesn't touch it")
    }
}

[test]
def test_upsert_excluded_and_dot_underscore_compose(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        // Exercises both `_.Hits` (existing) and `_excluded.Hits` (proposed)
        // in the same DO UPDATE expression.
        db |> _sql_upsert(
            WordHit(Id = 1, Word = "x", Hits = 7, Last = 0l),
            _.Id,
            (Hits = _.Hits + _excluded.Hits, Last = _excluded.Last))
        let h = db |> query_scalar("SELECT Hits FROM WordHits WHERE Id = 1", type<int>)
        t |> equal(h, 8, "1 (existing) + 7 (proposed) = 8")
    }
}
