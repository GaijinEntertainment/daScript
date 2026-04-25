options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers = true

module sql shared public

// Abstract layer for daslang's SQL story. This module is intentionally
// thin in the first chunk of the dasSQLITE rework — its purpose is to
// be the canonical require path that user code points at, regardless of
// the underlying SQL provider (`sqlite/sqlite_boost`, future
// `postgres/postgres_boost`, etc.).
//
// For now it re-exports Option<T> and Result<T, string> so user code
// only needs `require daslib/sql` to use the read-side / write-side
// optional and fallible return types. Provider-specific symbols
// (SqlRunner, with_<provider>, [sql_table], etc.) currently live in
// the provider's own boost module; they migrate up here when a second
// provider lands.

require daslib/option public
require daslib/result public
