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
def test_upsert_returning_yields_post_merge_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- db |> _sql_upsert_returning(
            WordHit(Id = 1, Word = "x", Hits = 7, Last = 999l),
            _.Id,
            (Hits = _.Hits + 1, Last = _excluded.Last))
        t |> equal(length(rows), 1, "RETURNING yields one row")
        t |> equal(rows[0].Hits, 2, "post-merge Hits = existing 1 + 1")
        t |> equal(rows[0].Last, 999l, "post-merge Last = _excluded.Last")
        t |> equal(rows[0].Word, "hello", "Word unchanged")
    }
}

[test]
def test_upsert_returning_for_fresh_row(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let rows <- db |> _sql_upsert_returning(
            WordHit(Id = 2, Word = "world", Hits = 5, Last = 0l),
            _.Id,
            (Hits = _.Hits + 1, Last = _excluded.Last))
        t |> equal(length(rows), 1, "RETURNING yields the just-inserted row")
        t |> equal(rows[0].Hits, 5, "fresh insert kept the proposed Hits=5")
    }
}

[test]
def test_try_upsert_returning_ok_path(t : T?) {
    with_sqlite(":memory:") $(db) {
        fixture(db)
        let r <- db |> _sql_try_upsert_returning(
            WordHit(Id = 1, Word = "x", Hits = 0, Last = 0l),
            _.Id,
            (Hits = _.Hits + 10, Last = _excluded.Last))
        t |> success(r |> is_ok, "try-returning succeeded")
        let rows <- r |> unwrap
        t |> equal(length(rows), 1, "one row returned")
        t |> equal(rows[0].Hits, 11, "existing 1 + 10 = 11")
    }
}
