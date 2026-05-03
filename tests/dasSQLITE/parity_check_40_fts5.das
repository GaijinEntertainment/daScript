options gen2

require dastest/testing_boost public

require daslib/sql
require daslib/linq_boost
require sqlite/sqlite_boost
require sqlite/sqlite_linq
require daslib/fts5_query

// Parity check for tutorial 40 — FTS5 full-text search.
//
// The interesting parity claim: `text_match(col, query)` produces the
// SAME hit set whether evaluated by SQLite's FTS5 engine (M1, lowered
// to `MATCH ?`) or by `daslib/fts5_query`'s in-memory parser (M2/M3,
// plain daslang call per-row).
//
// Chain shape: `_where(_.Body |> text_match(q))`. The Rank column is
// SQL-only (filled by SQLite from BM25) — it's left at default in M2/M3,
// so parity is on the set of matching Body strings, not on Rank values.
//
// Coverage:
//   - whitespace-AND  ("quick fox")
//   - prefix          ("quick*")
//   - phrase          ("\"quick brown\"")
//   - boolean OR      ("fox OR boot")
//   - NEAR            ("NEAR(quick fox, 3)")

[sql_fts5(name = "docs_idx")]
struct Doc {
    Body : string
    @sql_fts_rank Rank : float
}

def fixture_db(db : SqlRunner) {
    db |> create_table(type<Doc>)
    db |> insert(Doc(Body = "The quick brown fox jumps over the lazy dog"))
    db |> insert(Doc(Body = "A swift red fox escaped the hounds at dawn"))
    db |> insert(Doc(Body = "Quicksand swallowed the hiker's boot yesterday"))
    db |> insert(Doc(Body = "Code review is the most cost-effective bug filter"))
}

def fixture_array() : array<Doc> {
    var arr : array<Doc>
    arr |> emplace(Doc(Body = "The quick brown fox jumps over the lazy dog"))
    arr |> emplace(Doc(Body = "A swift red fox escaped the hounds at dawn"))
    arr |> emplace(Doc(Body = "Quicksand swallowed the hiker's boot yesterday"))
    arr |> emplace(Doc(Body = "Code review is the most cost-effective bug filter"))
    return <- arr
}

def bodies_of(rows : array<Doc>) : array<string> {
    var out : array<string>
    for (r in rows) {
        out |> push(r.Body)
    }
    out |> sort()
    return <- out
}

def strings_eq(a : array<string>; b : array<string>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (a[i] != b[i]) {
            return false
        }
    }
    return true
}

def assert_text_match_parity(t : T?; query : string; expected_count : int; tag : string) {
    with_sqlite(":memory:") $(db) {
        fixture_db(db)
        var arr <- fixture_array()

        var m1 <- _sql(db |> select_from(type<Doc>) |> _where(_.Body |> text_match(query)))
        var m2 <- (db |> select_from(type<Doc>) |> _where(_.Body |> text_match(query)))
        var m3 <- (arr |> _where(_.Body |> text_match(query)))

        let b1 <- bodies_of(m1)
        let b2 <- bodies_of(m2)
        let b3 <- bodies_of(m3)

        t |> equal(length(b1), expected_count, "{tag}: M1 hit count")
        t |> success(strings_eq(b1, b2), "{tag}: M1 ≡ M2 (bodies)")
        t |> success(strings_eq(b2, b3), "{tag}: M2 ≡ M3 (bodies)")
    }
}

[test]
def parity_text_match_and(t : T?) {
    // Whitespace = AND. "quick fox" needs both tokens. Hits doc 1
    // ("quick brown fox jumps") only — doc 3 has "quicksand" but no "fox".
    assert_text_match_parity(t, "quick fox", 1, "AND quick+fox")
}

[test]
def parity_text_match_prefix(t : T?) {
    // "quick*" matches "quick" + "quicksand". Docs 1 and 3.
    assert_text_match_parity(t, "quick*", 2, "prefix quick*")
}

[test]
def parity_text_match_phrase(t : T?) {
    // Quoted phrase = adjacent tokens in order. Only doc 1.
    assert_text_match_parity(t, "\"quick brown\"", 1, "phrase quick brown")
}

[test]
def parity_text_match_or(t : T?) {
    // "fox OR boot" — docs 1, 2 (fox), 3 (boot). Three hits.
    assert_text_match_parity(t, "fox OR boot", 3, "OR fox|boot")
}

[test]
def parity_text_match_near(t : T?) {
    // NEAR(quick fox, 3) — both tokens within 3 of each other. Doc 1
    // ("quick brown fox" — distance 2) qualifies. Doc 3 has only
    // "quicksand" (no "fox"), doesn't qualify.
    assert_text_match_parity(t, "NEAR(quick fox, 3)", 1, "NEAR quick fox 3")
}
