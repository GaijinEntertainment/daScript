# SQL provider conformance suite

Provider-neutral tests for the `daslib/sql_boost` + `daslib/sql_linq` contract
(see [modules/dasSQLITE/PROVIDER_CONTRACT.md](https://github.com/GaijinEntertainment/daScript/blob/master/modules/dasSQLITE/PROVIDER_CONTRACT.md)).
Every test goes through the shim in [`_conformance_provider.das`](_conformance_provider.das) -
the ONE file a provider swaps to run the suite against its own runner. In-tree the shim
is SQLite; an external provider repo (dasDuckDB, dasPostgreSQL) copies this directory and
rewrites only the shim:

- `with_conf_db(blk)` - scoped fresh empty database over the provider's runner type
- `CONF_PROVIDER_NAME` / `CONF_HAS_*` capability constants mirroring the provider's
  `SqlProviderCaps` registry entry
- a public require of the provider's migration glue module (in-tree:
  `sqlite/sqlite_migrate`) so `test_conf_migrations.das` sees the runner's
  `[sql_migration]` stream and the `migrate_to_latest` family

## Capability skip policy

- **Runtime-branchable capabilities** (e.g. `CONF_HAS_RETURNING`) are checked inside test
  bodies - both branches compile everywhere.
- **Compile-blocking capabilities** are skipped at FILE granularity: a provider that lacks
  the feature drops the file from its copy (the annotations inside wouldn't compile).
  Currently file-gated: `test_conf_fts5.das` (`caps.fts5`), `test_conf_sql_functions.das`
  (`caps.client_udfs`).

## Contract areas -> files

| File | Contract area |
|---|---|
| `test_conf_table_crud.das` | `[sql_table]` DDL round-trip, check_schema, column_info, insert single+bulk, update/delete_/delete_by_id |
| `test_conf_sql_select.das` | `_sql` reads: _where + captured binds, _order_by, take/skip, distinct, _first/_first_opt, _sql_text shape |
| `test_conf_projection_agg.das` | _select forms (column / named-tuple / computed), scalar aggregates, one-scan `_aggregate`, _group_by + _having |
| `test_conf_joins_subq.das` | _join (named/scalar and exact whole-source projections), `_aggregate` over whole-source joins, _left_join (Option right side), _in/_not_in, _any/_none |
| `test_conf_nullability.das` | Option<T> columns, is_some/is_none/unwrap_or translations |
| `test_conf_adapters.das` | sql_bind/sql_extract rail: custom type, enum, Option-over-custom, @sql_json (+path descent) |
| `test_conf_dml_macros.das` | _sql_update/_sql_delete/_sql_upsert (+ returning, capability-checked) |
| `test_conf_streaming_txn.das` | _each_sql lifecycle (early break), transactions (commit, nesting, try_transaction) |
| `test_conf_views.das` | [sql_view] + _create_view (captured locals inline via to_sql_literal) |
| `test_conf_fts5.das` | [sql_fts5] + text_match - file-gated on caps.fts5 |
| `test_conf_sql_functions.das` | [sql_function] UDF visible to chains - file-gated on caps.client_udfs |
| `test_conf_migrations.das` | [sql_migration] per-stream apply/idempotence/pending/history, baseline adoption, failure rollback, [struct_convert] + convert_and_rename rebuild |

Deep provider-specific behavior stays in the provider's own suite (`tests/dasSQLITE/`,
904 tests for SQLite) - this suite asserts the shared contract, not exhaustive behavior.
