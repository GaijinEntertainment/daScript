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
