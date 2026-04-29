options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 29 — column metadata: how to ask "what's in this table?"
//
// Three bands cover the schema-introspection question, in increasing order
// of how dynamic the answer needs to be:
//
//   Band 1: `column_info(type<T>) : array<ColumnInfo>` — compile-time
//           walk over the [sql_table] struct. The data is fixed at
//           expansion time; iterate at runtime over what the daslang
//           type system already knows. EF Core's `context.Model`
//           equivalent. This is the 95% answer.
//
//   Band 3: `query("PRAGMA table_info(<name>)", type<PragmaColumn>)` —
//           raw-SQL fallback when you genuinely don't know the schema
//           ahead of time (admin UI inspecting an unknown DB, runtime
//           tooling). Falls out of tut 5's escape hatch + the
//           tut-26 adapter rail; no new API surface.
//
// (Band 2 — design-time scaffolding that reverse-engineers a DB into
//  daslang [sql_table] structs — is out of scope. Dev-time tooling, not
//  user API. EF Core only ships it as `dotnet ef dbcontext scaffold`.)

[sql_table(name = "Cars")]
struct Car {
    @sql_primary_key Id : int
    Name : string
    Price : float = 9.99
    @safe_when_uninitialized Tag : Option<string>
    @sql_default_fn = "CURRENT_TIMESTAMP"
    CreatedAt : string
}

// --- Band 3 — raw PRAGMA via the typed `query` family -----------------
// PragmaColumn shape mirrors what SQLite returns for `PRAGMA table_info`:
//   cid | name | type | notnull | dflt_value | pk
// `type` is a SQL keyword, so the column is renamed via `@sql_column`.
// The rename flows through DDL (if you ever `db |> create_table(type<…>)`),
// through `_sql(...)` predicates / projections / `_order_by` / `_group_by`,
// through `column_info(type<…>).name`, and through INSERT/UPDATE/DELETE/
// upsert SQL emission. `query(...)` materializes positionally, so this
// PRAGMA call would also work with the daslang field literally named
// `col_type` and no annotation — but the rename means the column shows up
// as `"type"` everywhere SQL identifiers are emitted, matching what's on
// disk. `notnull` and `pk` come back as integers (0/1).

// `[sql_table]` on a read-only row shape is the idiomatic way to opt in
// to the typed materializer without committing to a CREATE TABLE — the
// generated insert/update/delete helpers exist but you simply don't call
// them. The macro's value here is `_sql_read_row`, the per-struct shape
// the `query` family uses to materialize rows.
[sql_table(name = "pragma_columns")]
struct PragmaColumn {
    cid : int
    name : string
    @sql_column = "type"
    col_type : string
    notnull : int
    dflt_value : string
    pk : int
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        db |> create_table(type<Car>)

        // --- Band 1 — compile-time-known metadata ----------------------
        // `column_info(type<T>)` returns an `array<ColumnInfo>` baked at
        // [sql_table] expansion time. Iterating it does not touch the DB.
        for (c in column_info(type<Car>)) {
            let pk = c.is_pk ? " PK" : ""
            let nn = c.is_nullable ? "" : " NOT NULL"
            let df = empty(c.default_expr) ? "" : " DEFAULT {c.default_expr}"
            to_log(LOG_INFO, "{c.name} {sqlite_sql_type(c.data_type)}{pk}{nn}{df}\n")
        }

        // The same array composes with linq idioms — the columns are just
        // data. Filter, project, fold like any other `array<T>`.
        let pk_count = length([for (c in column_info(type<Car>)); c; where c.is_pk])
        to_log(LOG_INFO, "PK columns: {pk_count}\n")

        // `sqlite_sql_type` is the SQLite-dialect renderer for the
        // abstract `SqlType` enum. Future providers ship their own
        // (`postgres_sql_type`, `mysql_sql_type`, …) so the same
        // `ColumnInfo` round-trips across backends.
        to_log(LOG_INFO, "Integer renders as: {sqlite_sql_type(SqlType.Integer)}\n")

        // --- Band 3 — runtime PRAGMA (when the schema isn't statically known)
        // Useful when the DB came from somewhere else and there is no
        // [sql_table] struct in your code that describes it.
        for (col in db |> query("PRAGMA table_info(Cars)", type<PragmaColumn>)) {
            to_log(LOG_INFO, "[runtime] {col.name} : {col.col_type} pk={col.pk}\n")
        }
    }
}

// --- When to use which band -------------------------------------------
//
//   Band 1 (column_info) — you have a [sql_table] struct in your code
//   and want to drive UI, code generation, validation, DDL diffing, etc.
//   off the daslang-side schema. Free relative to declaring the struct;
//   no DB round-trip.
//
//   Band 3 (PRAGMA query) — the DB is the source of truth (admin tools,
//   migrations, reverse-engineering). Costs a `PRAGMA` per call; SQLite-
//   specific spelling.
//
// In practice most app code only needs Band 1. Band 3 shows up in
// migration scripts and inspection CLIs.

// --- Why no `db |> schema(name) : array<ColumnInfo>` runtime helper ---
//
// A name-keyed runtime helper that returns the SAME `ColumnInfo` shape
// — pulling data from `PRAGMA table_info` instead of the macro walk —
// is a tempting symmetry. Two reasons it does NOT ship:
//
//   1. `dflt_value` from PRAGMA comes back as the SQL-side default
//      AS STORED — not always the daslang field initializer that
//      created it. Subtle round-trip mismatch between Band 1 and Band 3.
//
//   2. Information loss. PRAGMA exposes a different subset than the
//      daslang macro can see. CHECK constraints, FOREIGN KEY referent
//      tables, expression-index columns, view bindings — different
//      backends spell these differently. A "uniform" ColumnInfo over
//      runtime PRAGMA would either lie or grow per-provider escape
//      hatches.
//
// The honest path: Band 1 stays compile-time-typed, Band 3 stays the
// raw-SQL escape hatch, and the boundary is explicit.
