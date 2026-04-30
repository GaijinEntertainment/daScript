options gen2

require dastest/testing_boost public

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

[sql_fts5(name="docs_idx")]
struct Doc {
    Body : string
    @sql_fts_rank Rank : float
}

[test]
def test_fts5_emits_create_virtual_table(t : T?) {
    t |> equal(_::_sql_create_table_sql(type<Doc>),
        "CREATE VIRTUAL TABLE IF NOT EXISTS \"docs_idx\" USING fts5(\"Body\", tokenize='unicode61')",
        "[sql_fts5] emits CREATE VIRTUAL TABLE … USING fts5")
}

[test]
def test_fts5_emits_select_with_rank_column(t : T?) {
    t |> equal(_::_sql_select_all_sql(type<Doc>),
        "SELECT \"Body\", rank FROM \"docs_idx\"",
        "@sql_fts_rank field maps to FTS5 hidden `rank` column on SELECT")
}

[test]
def test_fts5_insert_skips_rank_column(t : T?) {
    t |> equal(_::_sql_insert_no_pk_sql(Doc(Body = "x")),
        "INSERT INTO \"docs_idx\" (\"Body\") VALUES (?)",
        "@sql_fts_rank column is skipped on INSERT (FTS5 populates rank automatically)")
}

[test]
def test_fts5_column_info_exposes_rank_as_rank(t : T?) {
    // `@sql_fts_rank` fields surface in column_info as name="rank" regardless of
    // the daslang field name — that's the SQL-side identifier callers see.
    let cols <- column_info(type<Doc>)
    t |> equal(length(cols), 2)
    t |> equal(cols[0].name, "Body")
    t |> equal(cols[1].name, "rank", "rank field surfaces as FTS5's hidden `rank`, not the daslang field name")
    t |> equal(cols[1].is_nullable, false, "rank is non-nullable (Option<float> rejected at macro time)")
}

[test]
def test_fts5_text_match_emits_match_predicate(t : T?) {
    var _db = SqlRunner()
    let _q = "fox"
    let sql = _sql_text(_db |> select_from(type<Doc>)
                          |> _where(_.Body |> text_match(_q)))
    t |> equal(sql,
        "SELECT \"Body\", rank FROM \"docs_idx\" WHERE \"Body\" MATCH ?",
        "text_match on [sql_fts5] column emits MATCH ?")
}

[test]
def test_fts5_runtime_insert_select_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Doc>)
        db |> insert(Doc(Body = "the quick brown fox jumps over the lazy dog"))
        db |> insert(Doc(Body = "a swift red fox escaped at dawn"))
        db |> insert(Doc(Body = "quicksand swallowed the boot"))

        let hits <- _sql(db |> select_from(type<Doc>)
                           |> _where(_.Body |> text_match("fox"))
                           |> _order_by(_.Rank))
        t |> equal(length(hits), 2, "two rows mention 'fox'")
        // BM25 ranks are negative; lower (more-negative) = more relevant. Both rows had one hit.
        t |> success(hits[0].Rank <= hits[1].Rank, "rank order is ascending (more-relevant first)")
    }
}

[test]
def test_fts5_runtime_prefix_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Doc>)
        db |> insert(Doc(Body = "quick brown fox"))
        db |> insert(Doc(Body = "quicksand boot"))
        db |> insert(Doc(Body = "slow turtle"))

        let hits <- _sql(db |> select_from(type<Doc>)
                           |> _where(_.Body |> text_match("quick*")))
        t |> equal(length(hits), 2, "two rows match the 'quick*' prefix")
    }
}

[test]
def test_fts5_runtime_phrase_match(t : T?) {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Doc>)
        db |> insert(Doc(Body = "quick brown fox"))
        db |> insert(Doc(Body = "fox quick"))   // different word order

        let hits <- _sql(db |> select_from(type<Doc>)
                           |> _where(_.Body |> text_match("\"quick brown\"")))
        t |> equal(length(hits), 1, "phrase match only finds adjacent tokens in order")
    }
}
