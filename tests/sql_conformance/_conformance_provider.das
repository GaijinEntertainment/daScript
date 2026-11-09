options gen2
options indenting = 4

module _conformance_provider shared public

//! SQL-provider conformance shim — the ONE file a provider swaps to run the shared
//! suite (see README.md). Exposes a scoped open over the provider's runner plus
//! capability constants for self-skip of unsupported groups.

require sqlite/sqlite_boost public
require sqlite/sqlite_migrate public
require daslib/sql_linq public

// Capability constants — mirror the provider's SqlProviderCaps registry entry.
// Runtime-branch on these inside test bodies; a capability whose ABSENCE makes a
// whole file uncompilable (fts5, client UDFs) is skipped at file granularity instead
// (the provider drops the file from its copy of the suite — see README.md).
let public CONF_PROVIDER_NAME = "sqlite"
let public CONF_HAS_FTS5 = true
let public CONF_HAS_CLIENT_UDFS = true
let public CONF_HAS_RETURNING = true

def public with_conf_db(blk : block<(db : SqlRunner) : void>) : void {
    //! Scoped fresh empty database — every conformance test runs against one of these.
    with_sqlite(":memory:", blk)
}
