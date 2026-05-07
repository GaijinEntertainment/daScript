options gen2

// `[sql_fts5]` field-annotation rejections. Each struct below trips one
// validation path in the FTS5 macro:
//   1. @sql_column on @sql_fts_rank — rank is hardcoded to FTS5's hidden id.
//
// All fail with 30111 ("macro [sql_fts5] failed to apply").
expect 20800

require daslib/sql
require sqlite/sqlite_boost

[sql_fts5(name = "F1_idx")]
struct F1 {
    Body : string
    @sql_fts_rank
    @sql_column = "score"        //!< @sql_column on @sql_fts_rank is rejected
    Rank : float
}
