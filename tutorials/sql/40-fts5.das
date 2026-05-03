options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq
require daslib/fts5_query   // for in-memory text_match against the same query syntax

// Tutorial 40 — FTS5 full-text search.
//
// SQLite-only. The portable string predicates (`starts_with`,
// `ends_with`, `contains`) emit `LIKE ? ESCAPE '\'` patterns and work
// on any backend. They're fine for short columns, but `LIKE '%x%'`
// can't use a B-tree index — full scan at scale is painful. FTS5 is
// the specialized tool: a virtual table that builds an inverted index
// over text columns and exposes relevance ranking + Boolean / phrase
// / prefix / NEAR query operators.
//
// API surface:
//
//   [sql_fts5(name="...")]    — struct annotation: virtual table
//   @sql_fts_rank             — read-only float column → FTS5 `rank`
//   text_match(col, query)    — _where predicate → `col MATCH ?`
//
// The same `text_match` is also a `strings_boost` predicate (full
// FTS5-grammar parser in `daslib/fts5_query`); user code gets in-memory
// and SQL-side matching from one call site.

[sql_fts5(name = "docs_idx")]
struct Doc {
    Body : string
    @sql_fts_rank Rank : float
    // FTS5 fills `Rank` on SELECT (BM25 score; lower = more relevant).
    // INSERT skips this column — FTS5 has no user-writable rank.
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        db |> apply_recommended_pragmas()
        db |> create_table(type<Doc>)
        // => CREATE VIRTUAL TABLE IF NOT EXISTS "docs_idx"
        //        USING fts5("Body", tokenize='unicode61')

        // --- INSERT: just like a regular [sql_table] ----------------------
        db |> insert(Doc(Body = "The quick brown fox jumps over the lazy dog"))
        db |> insert(Doc(Body = "A swift red fox escaped the hounds at dawn"))
        db |> insert(Doc(Body = "Quicksand swallowed the hiker's boot yesterday"))
        db |> insert(Doc(Body = "Code review is the most cost-effective bug filter"))

        // --- text_match: simple keyword search ---------------------------
        // Whitespace-AND: both tokens must appear (any order, any
        // distance). Order by _.Rank → ascending = most relevant first.
        let foxes <- _sql(db |> select_from(type<Doc>)
                            |> _where(_.Body |> text_match("quick fox"))
                            |> _order_by(_.Rank))
        // => SELECT "Body", rank FROM "docs_idx"
        //    WHERE "Body" MATCH ? ORDER BY rank
        for (hit in foxes) {
            to_log(LOG_INFO, "[rank={hit.Rank}] {hit.Body}")
        }

        // --- Prefix match -------------------------------------------------
        // `quick*` matches `quick`, `quicksand`, `quicker`, …
        let quickish <- _sql(db |> select_from(type<Doc>)
                               |> _where(_.Body |> text_match("quick*")))
        to_log(LOG_INFO, "prefix `quick*`: {length(quickish)} hits")

        // --- Phrase match (adjacent tokens, in order) ---------------------
        // Quote the phrase. Without quotes it's whitespace-AND.
        let phrase <- _sql(db |> select_from(type<Doc>)
                             |> _where(_.Body |> text_match("\"quick brown\"")))
        to_log(LOG_INFO, "phrase `quick brown`: {length(phrase)} hit")

        // --- Boolean operators -------------------------------------------
        // AND / OR / NOT are FTS5 keywords (uppercase required in SQL).
        let either <- _sql(db |> select_from(type<Doc>)
                             |> _where(_.Body |> text_match("fox OR boot")))
        to_log(LOG_INFO, "fox OR boot: {length(either)} hits")

        // --- NEAR --- nolint:STYLE014
        // Proximity: `NEAR(quick fox, 3)` means both within 3 tokens of
        // each other. Default proximity is 10 if the trailing `, N` is
        // omitted.
        let near <- _sql(db |> select_from(type<Doc>)
                           |> _where(_.Body |> text_match("NEAR(quick fox, 3)")))
        to_log(LOG_INFO, "NEAR(quick fox, 3): {length(near)} hits")

        // --- LIMIT for top-N relevance results ---------------------------
        let top2 <- _sql(db |> select_from(type<Doc>)
                           |> _where(_.Body |> text_match("fox"))
                           |> _order_by(_.Rank)
                           |> take(2))
        to_log(LOG_INFO, "top 2 fox results: {length(top2)}")
    }

    // --- text_match outside SQL --- nolint:STYLE014
    // The exact same query syntax works in-memory via daslib/fts5_query.
    // Useful for: scanning log lines, filtering arrays before bulk insert,
    // implementing custom highlighters.
    let lines = ["The quick brown fox", "the lazy dog", "quicksand boot"]
    var hits : array<string>
    for (line in lines) {
        if (line |> text_match("quick OR fox")) {
            hits |> push_clone(line)
        }
    }
    to_log(LOG_INFO, "in-memory match: {length(hits)} of {length(lines)} lines")

    // --- text_match on a non-FTS5 [sql_table] is a COMPILE ERROR --- nolint:STYLE014
    // The translator refuses to silently emit MATCH against an indexed-
    // less column — it has no way to give useful behavior. The error
    // message points at `contains` (LIKE-based) or adding [sql_fts5].
    //
    //     [sql_table(name = "Articles")]
    //     struct Article { Title : string }
    //
    //     _sql(db |> select_from(type<Article>)
    //         |> _where(_.Title |> text_match("foo")))
    //     ^^^ compile error:
    //         _sql: text_match() requires an [sql_fts5] struct, got 'Article'.
    //         Options: use contains(_.Col, q) for LIKE-based substring match,
    //         or annotate the struct with [sql_fts5] for indexed full-text
    //         search.

    // --- v1 limitations --- nolint:STYLE014
    // Self-contained mode only — the FTS5 table holds both content and
    // index. External-content mode (FTS5 indexing rows of a separate
    // [sql_table]) ships when asked. Default tokenizer is unicode61
    // (case-fold + Unicode word boundaries). UPDATE / DELETE on FTS5
    // rows aren't typed in v1 — drop and re-INSERT, or use raw
    // `db |> exec("DELETE FROM docs_idx WHERE rowid = ?", id)`.
    // BM25 weighting, snippet/highlight helpers, and per-column query
    // filters (`Body:fox`) all work via the FTS5 query string but
    // don't have typed wrappers in v1.
}
