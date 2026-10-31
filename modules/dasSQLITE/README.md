# dasSQLITE

SQLite bindings and typed query/transaction helpers. Import `sqlite/sqlite_boost`
to use `SqlRunner`, parameterized statements and transaction scopes. The
`tutorial/` directory contains runnable examples; `PROVIDER_CONTRACT.md` describes
integration with the SQL query macros.

## Result-returning transactions

`try_transaction(db, mode, block)` also accepts a block returning `SqlError`.
Return `none(type<string>)` to commit or `some(error)` to roll back. The returned
error is preserved; commit failures are returned and trigger rollback. Nested
calls use savepoints, so a returned inner error rolls back only that inner scope.

Expected application failures belong in the returned result. A panic is a fatal
shutdown condition, not a transaction error to catch and continue from. The host
closes the connection during graceful shutdown. No callback should perform an
external side effect under the assumption that SQLite can roll it back.

The existing void-block overload keeps its behavior. Tests in
`tests/dasSQLITE/test_transaction_result.das` cover explicit rejection, SQL failure,
nested savepoints and a deferred constraint that fails at commit.

## Composite primary keys

Mark every component with `@sql_primary_key`. Generated table creation declares
one composite constraint; row-based `update` and `delete_` match every component.
Zero is a valid integer component of a composite key, including bulk inserts and
`insert_or_replace`. The zero-as-generated-ID convention applies only to a single
integer primary key.

`delete_by_id` accepts only a single-column primary key. For composite keys, pass
a row with all key fields to `delete_` (or the result-returning `try_delete_`).

A component annotated `@sql_unique` remains individually unique, in addition to
the composite constraint. Composite key columns are non-null. `@sql_references`
infers a single parent key column and rejects parents with composite keys; it does
not silently select one component.
