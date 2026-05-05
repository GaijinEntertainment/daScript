# test_data — fixtures for `[sql_table(schema_from=...)]` tests

Pre-built tiny SQLite databases consumed at compile time by the
`schema_from` test suite. Each `.db` has a sibling `.sql` showing the
DDL it was built from.

## Regenerate

The fixture builder is `_gen_fixtures.das`. To rebuild after editing it:

```
bin/Release/daslang tests/dasSQLITE/test_data/_gen_fixtures.das
```

The script removes existing `.db` files and recreates each from scratch
via `with_sqlite + exec(CREATE TABLE ...)`. Keep the SQL in
`_gen_fixtures.das` and the prose `.sql` files in lockstep so reviewers
can see the schema without opening the binary.
