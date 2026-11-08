options gen2
//! a MATCH on an UNINDEXED fts5 column must not compile

// UNINDEXED metadata is stored and filterable, but cannot be a MATCH target.
expect 50503

require daslib/sql
require sqlite/sqlite_boost
require daslib/sql_linq
require daslib/fts5_query

[sql_fts5(name = "docs_idx")]
struct Doc {
    @sql_fts_unindexed Kind : string
    Body : string
}

[export]
def main() {
    var db = SqlRunner()
    let invalid = _sql(db |> select_from(type<Doc>)
                          |> _where(_.Kind |> text_match("metadata")))
}
