# SQL provider contract

What a database provider implements to plug into the `_sql` LINQ-to-SQL machinery
(`daslib/sql_linq.das`) and the `[sql_table]` macro layer (`daslib/sql_boost.das`).
SQLite (`sqlite_boost.das` + the registration shim `sqlite_provider.das`) is the
reference implementation; [dasDuckDB](https://github.com/borisbat/dasDuckDB) and
[dasPostgreSQL](https://github.com/borisbat/dasPostgreSQL) are the external consumers. This
document is the contract; the registry core in `daslib/sql_provider.das` is its
executable form.

The rework arc's design records — `API_REWORK.md`, `API_MISSING.md`, `API_CHECKED.md`,
`API_MIGRATION.md`, `TUTORIALS.md`, and the `tutorial-mockup/` design artifacts — are
archived under `history/dasSQLITE/` (2026-07-24).

## Architecture in one paragraph

`_sql(...)` and its siblings analyze a LINQ-shaped chain at **compile time** and emit
(a) a SQL string with positional placeholders, (b) a bind block, (c) a row-reader
block/lambda, all passed to a **runtime runner helper** (`run_select`,
`run_each_select`, ...). Nothing provider-specific survives to runtime except the
helper call — dispatch is by ordinary overload resolution on the provider's runner
type. The macro learns which provider it is emitting for from the **provider
registry**: a compile-time (macro-context) table keyed by the runner's type name,
carrying the provider's statement type, dialect hooks, and capability flags.

## What a provider ships

### 1. Runtime types

| Piece | SQLite reference | Notes |
|---|---|---|
| Runner struct | `SqlRunner { sqlite_handle, schema_name }` | The value user code threads through every call. Must be a struct (registry keys on its type name) |
| Statement handle | `sqlite3_stmt?` | Spliced by the macro into emitted bind/reader blocks; opaque to the analyzer |
| `finalize(var db)` | closes the connection | enables `var inscope db` RAII |
| `with_<provider>(path) <\| $(db)` | `with_sqlite` | scoped open/close |
| open family | `open_sqlite` / `try_open_sqlite` | strict + `try_` forms |

### 2. Runtime runner helpers (overloads on the runner type)

The macro emits calls to these **unqualified** names; overload resolution on the
first argument picks the provider. Signatures are generic over the row type
(`auto(TT)`) and take the provider's concrete stmt type in block/lambda positions:

| Helper | Shape | Used by |
|---|---|---|
| `run_select` | `(db, sql, bind_blk, row_blk) : array<TT>` | `_sql` (default terminal) |
| `run_select_one` | `... : TT` (panics on empty) | `_sql(... \|> _first())` |
| `run_select_one_opt` | `... : Option<TT>` | `_first_opt()` |
| `try_run_select` / `_one` / `_one_opt` | `... : Result<..., string>` | `_try_sql` |
| `run_each_select` | `(db, sql, bind_blk, row_lam) : iterator<TT>` | `_each_sql`; finalize in generator `finally` |
| `run_exec_binds` | `(db, sql, bind_blk) : int` (rows affected) | `_sql_update` / `_sql_delete` |
| `run_returning` variants | rows-after-write | `_sql_*_returning` |
| `sql_bind_to_stmt` | `(var stmt, idx, v)` per storage class + adapter fallthrough | emitted bind blocks; generated `_sql_bind_row` |
| `sql_read_witness` / `read_via_adapter` | `(stmt, col, ...)` per storage class | row readers; generated `_sql_read_row` |
| `exec` / `query*` raw-SQL family | see skills/sql.md | not macro-emitted; provider-specific surface |

The generated per-table helpers (`_sql_bind_row`, `_sql_read_row`, from
`[sql_table]`) are **generic over the stmt parameter** — they monomorphize per
provider, and their interior `sql_bind_to_stmt` / `read_via_adapter` calls
overload-resolve against whichever provider's stmt type instantiated them.

### 3. Registry entry (compile-time)

Registries live per macro context (consumer-requires-contributor; see
skills/das_macros.md "Macro modules each compile into their own context"), so
registration is pulled, not pushed: each consulting macro module (`daslib/sql_boost`,
`daslib/sql_linq`) has a `[_macro]` twin calling `sql_register_present_providers`
(in `daslib/sql_boost`), which optionally-requires each provider's **registration
shim** (`require ?sqlite sqlite/sqlite_provider`) and calls its registration
function under `static_if (typeinfo builtin_module_exists(<mod>))`. A new provider
therefore ships a lean shim module (stmt factories + dialect hooks + caps; it must
NOT require the provider's boost — that would cycle through `daslib/sql_boost`) and
adds one `require ?<mod>` line plus one `static_if` branch to
`sql_register_present_providers`. Entry fields:

| Field | SQLite value | Consulted by |
|---|---|---|
| `name` | `"sqlite"` | diagnostics |
| `runnerModule` + `runnerStruct` | `"sqlite_boost"` + `"SqlRunner"` | registry lookup key, matched against `q.dbExpr._type`'s struct + owning module |
| `makeStmtType` | `qmacro_type(type<sqlite3_stmt?>)` | bind-block parameter splices (mutable flavor) |
| `makeStmtTypeConst` | `qmacro_type(type<sqlite3_stmt? const>)` | row-reader block/lambda parameter splices — qualifiers are spelled in the type expression; `$t` substitution replaces the placeholder type wholesale, so the parser's const-append on non-`var` params does not apply to spliced types |
| `renderPlaceholder(ordinal)` | `"?"` | `fold_to_builder` — the single render point for bind frags (`?` inside `build_sql_string` output is internal IR that `sql_to_frags` re-splits). Ordinal is 1-based among macro-enumerated binds; upsert row-binds are emitted as literal `?` text (bound by the generated `_sql_bind_row`, not enumerated), so a numbered-placeholder provider must also handle row-binder placeholders at port time. dasPostgreSQL's resolution: register `"?"` here and renumber every statement's `?` markers to `$n` in one quote-aware runtime pass at execute time — text order equals bind-index order by construction across chain SQL, upsert, and generated CRUD |
| `noLimitSpelling` | `"LIMIT -1"` | OFFSET-without-LIMIT rendering in `build_sql_string` |
| `renderJsonExtract(target, path)` | `json_extract({target}, '$....')` | `@sql_json` column descent (`render_json_extract`) |
| `quoteId(name)` | `"name"` (double quotes) | registered, NOT yet routed — double-quote identifier quoting is portable across SQLite/DuckDB/PostgreSQL; the renderers keep inline quoting until a diverging provider (e.g. MySQL backticks) forces the sweep |
| `sqlTypeFor(SqlType)` | `sqlite_sql_type` | routed for diagnostics (schema_from type-mismatch labels); DDL column-type emission still flows through the `sql_storage_type_for` witness rail with the standard INTEGER/REAL/TEXT/BLOB spellings — per-provider DDL routing (selected by the runner at the `create_table` call site) lands with the first diverging provider (DuckDB: BIGINT) |
| `identityDdl` | `PRIMARY KEY` (INTEGER PK aliases rowid) | registered, NOT yet routed — same story as `sqlTypeFor`'s DDL half |
| `readTableSchema(path, tbl)` | readonly open + `PRAGMA table_info` + affinity rules | `[sql_table(schema_from=...)]` compile-time introspection. Returns `Result<array<SchemaFromCol>, string>` with provider-normalized `data_type : SqlType`; `null` when the provider has no file-based schema source. Annotation time has no runner, so exactly one registered provider may supply the hook (ambiguity is a macro error until a `provider=` annotation arg exists) |
| capability flags | see below | macro_error gates |

### 4. Capability flags — `SqlProviderCaps` bitfield + `pkReport`

| Field | SQLite | Gate |
|---|---|---|
| `caps.fts5` | set | `[sql_fts5]` / `text_match` → compile error when absent |
| `caps.client_udfs` | set | `[sql_function]` → compile error when absent (PG has no client-side UDFs) |
| `caps.returning` | set | `_sql_*_returning` |
| `caps.distinct_on` | clear | `_distinct_by` lowering selector, routed in `build_distinct_by_inner_subquery`: clear → SQLite's bare-aggregate `SELECT *, MIN/MAX(pk) FROM t GROUP BY K` (bare columns beside GROUP BY are SQLite-only semantics); set → PostgreSQL-style `SELECT DISTINCT ON (K) * FROM t ORDER BY K, pk [DESC]` (a PG extension, also implemented by DuckDB). Not a macro_error gate — every provider gets one of the two lowerings |
| `pkReport : SqlPkReport` | `LastInsertRowid` | `insert(...) : id` — `ReturningPk` for providers without rowid |

The gates wire up as the second provider lands; in this PR the flags are
registered data only.

A missing capability is a **compile-time** `macro_error` naming the provider and the
feature — never a runtime failure.

## Dialect notes (what is already portable)

`ON CONFLICT` upsert, `RETURNING`, double-quote identifier quoting, and
`LIKE ... ESCAPE '\'` are portable across SQLite / DuckDB / PostgreSQL. DuckDB even
shares the `json_extract` spelling. The known divergence points are exactly the
registry hooks above plus placeholder style (`?` vs `$n`).

## Conformance

A provider passes the shared conformance suite (`tests/sql_conformance/`) by shipping
a `conformance_provider.das` shim exposing its runner + capability flags for self-skip
of unsupported groups. SQLite's shim is the in-tree reference; an external provider
repo copies the suite directory and swaps the shim (see the suite's README.md).

## Migrations

`[sql_migration]`, the `migrate_to_latest` runner family, `baseline`, and the
`[struct_convert]` / `convert_and_rename` rebuild rail live in the neutral
`daslib/sql_migrate`. The registry is **partitioned into per-provider streams keyed by
the migration body's runner parameter type** — the annotation derives the stream key
`"<runnerModule>::<RunnerStruct>"` from the parameter, and validates the type against
the provider registry. Version uniqueness is checked **per stream** (a PostgreSQL v2
and a SQLite v2 coexist), both at compile time (cross-module annotation scan, grouped
by stream) and at runtime (per-stream dup panic). Bodies are stored type-erased
(`MigrationBodyCarrier`); the glue re-types them on invocation.

A provider ships a small glue module (`sqlite/sqlite_migrate` is the in-tree
reference; `duckdb/duckdb_migrate`, `postgres/postgres_migrate` external) that
`require daslib/sql_migrate public` (re-export keeps the one-require surface) and
implements the hook contract, dispatched on the runner type from the engine's
generics via `_::`:

| Hook | SQLite reference | Notes |
|---|---|---|
| `_migration_stream_key(db)` | `"sqlite_boost::SqlRunner"` | MUST equal the annotation's derived key — registry `(runnerModule, runnerStruct)` identity |
| `_migration_begin(db, blocking)` | `BEGIN IMMEDIATE` | takes the provider's migration lock + opens the txn, **all-or-nothing**: on `err` the glue must hold NOTHING — a glue that locks and then fails to `BEGIN` releases its own lock before returning (the engine's error return never calls `_migration_end`). PostgreSQL: session advisory lock (`pg_advisory_lock` when `blocking`, `pg_try_advisory_lock` fail-fast otherwise — `migrate_to_latest` blocks, `try_migrate_to_latest` fails fast), then `BEGIN`, unlocking internally if `BEGIN` fails. DuckDB: plain `BEGIN` (single-writer engine locks for us) |
| `_migration_end(db)` | no-op | releases the lock after COMMIT/ROLLBACK (PostgreSQL: `pg_advisory_unlock`; session locks auto-release on crash/disconnect) |
| `_migration_query_int(db, sql)` | `try_query_scalar` | `Result<int, string>` one-int-result rail for the engine's COUNT/MAX probes (the raw `query*` family is provider-specific, so the engine can't call it) |
| `_migration_audit_exists(db)` | `sqlite_master` probe | `Result<bool, string>` — `information_schema.tables` on PostgreSQL/DuckDB |
| `_migration_has_user_objects(db)` | `sqlite_master` count | drives the adoption hint |
| `_migration_history_rows(db)` | native stmt rail | reads `__schema_version` into `array<MigrationRecord>` |
| `_migration_invoke_body(db, body)` | reinterpret + invoke | re-types the erased body to `function<(db : <Runner>) : void>` |

Engine SQL is portable by construction: the audit table uses `BIGINT` for
`applied_at` and the INSERT binds the epoch client-side (`int64(get_clock())` — no
`unixepoch()` / `EXTRACT(EPOCH ...)` divergence), and COMMIT/ROLLBACK/COALESCE/COUNT
are universal. Post-commit `VACUUM` / `ANALYZE` remain warnings-only.
`tests/sql_conformance/test_conf_migrations.das` exercises the contract.
