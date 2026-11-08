options gen2
//! an fts5 struct must not compile with a renamed rank field or a
//! rank field that is also unindexed

// `[sql_fts5]` field-annotation rejections. Each struct below trips
// one validation paths in the FTS5 structure macro: 1. @sql_column on
// @sql_fts_rank — rank is hardcoded to FTS5's hidden id. 2.
// @sql_fts_rank combined with @sql_fts_unindexed.
//
// Both use 20800 ("macro [sql_fts5] failed to apply").
expect 20800:2

require daslib/sql
require sqlite/sqlite_boost

[sql_fts5(name = "F1_idx")]
struct F1 {
    Body : string
    @sql_fts_rank
    @sql_column = "score"        //!< @sql_column on @sql_fts_rank is rejected
    Rank : float
}

[sql_fts5(name = "F2_idx")]
struct F2 {
    Body : string
    @sql_fts_rank
    @sql_fts_unindexed
    Rank : float
}
