# SQL — daslib/sql_linq + providers (dasSQLITE / dasDuckDB / dasPostgreSQL)

Read this skill before writing or editing any `.das` code that talks to a SQL database. The companion tutorials live under [tutorials/sql/](../tutorials/sql/) (45 files, numbered by teaching order — `01-version.das` through `44-in_not_in_collections.das`, plus `12b-set_ops.das`); read the relevant ones for runnable examples of every pattern below. Implementation: the provider-neutral core is [daslib/sql_boost.das](../daslib/sql_boost.das) (`[sql_table]` / `[sql_view]` / `[sql_index]` macros, `add_column` / `create_index` ALTER macros, the `sql_bind`/`sql_extract` adapter rail) and [daslib/sql_linq.das](../daslib/sql_linq.das) (the `_sql(...)` family of call macros), routed through the provider registry in [daslib/sql_provider.das](../daslib/sql_provider.das). The SQLite provider is [modules/dasSQLITE/daslib/sqlite_boost.das](../modules/dasSQLITE/daslib/sqlite_boost.das) (`SqlRunner`, exec/query runtime, runner helpers, `[sql_fts5]`) plus the registration shim [sqlite_provider.das](../modules/dasSQLITE/daslib/sqlite_provider.das); the `[sql_function]` annotation is provider-neutral (in `daslib/sql_boost`), and migrations (`[sql_migration]` + the `migrate_to_latest` runner + `[struct_convert]`) live in the neutral [daslib/sql_migrate.das](../daslib/sql_migrate.das) with per-provider glue ([sqlite_migrate.das](../modules/dasSQLITE/daslib/sqlite_migrate.das) re-exports it). The living provider contract is `modules/dasSQLITE/PROVIDER_CONTRACT.md` (repo-only); the rework arc's design notes, decision logs, and deferred-feature list (`API_REWORK.md`, `TUTORIALS.md`, `API_MIGRATION.md`, `API_MISSING.md`) are archived in the daslang repo under `history/dasSQLITE/`.

Three backends implement the provider contract: in-tree **SQLite** (the reference), and the external **[dasDuckDB](https://github.com/borisbat/dasDuckDB)** and **[dasPostgreSQL](https://github.com/borisbat/dasPostgreSQL)** repos (each carries its own CLAUDE.md with dialect divergences). The macro layer (`daslib/sql_boost` + `daslib/sql_linq`) is provider-neutral; everything runner- or dialect-specific lives in the provider's module and its registry entry. This skill's examples are written against SQLite; the `_sql` chain surface is identical across providers — swap the runner (`with_sqlite` / `with_duckdb` / `with_postgres`) and the require lines.

## Providers at a glance

| | SQLite (in-tree) | DuckDB (external) | PostgreSQL (external) |
|---|---|---|---|
| Require | `sqlite/sqlite_boost` | `duckdb/duckdb_boost` | `postgres/postgres_boost` |
| Scoped open | `with_sqlite(path)` | `with_duckdb(path)` | `with_postgres(conninfo)` |
| `[sql_fts5]` / `text_match` | ✓ | compile error | compile error |
| `[sql_function]` UDFs | ✓ | ✓ | compile error (no client-side UDFs) |
| `_distinct_by` lowering | bare-aggregate `GROUP BY` | `DISTINCT ON` | `DISTINCT ON` |
| Migrations glue | `sqlite/sqlite_migrate` | `duckdb/duckdb_migrate` | `postgres/postgres_migrate` (advisory-lock coordination) |
| PRAGMA / VACUUM / ATTACH / backup | ✓ (SQLite-specific surface) | engine-specific via `exec` | engine-specific via `exec` |

Capability gates are **compile-time** `macro_error`s naming the provider (`caps` bitfield in the registry — see `modules/dasSQLITE/PROVIDER_CONTRACT.md` (repo-only)). Everything else — `[sql_table]` DDL, insert-returns-pk, upsert, transactions (nested composes on all three), placeholder style — is handled per-provider under the same surface; user code doesn't change.

## `require`

```das
require sqlite/sqlite_boost         // provider: SqlRunner, exec/query runtime, [sql_fts5], [sql_function]
require daslib/sql_linq             // _sql / _try_sql / _each_sql / _aggregate / _sql_update / _sql_delete / _sql_upsert / _create_view / _sql_text
require daslib/linq_das             // OPTIONAL: `%linq! from row : T in db ... %%`; lowers through the same SQL analyzer
require sqlite/sqlite_migrate       // OPTIONAL — [sql_migration], migrate_to_latest, with_latest_sqlite, baseline
```

`sqlite_boost` re-exports the raw `sqlite` C binding, `daslib/sql` (`SqlType`, `ColumnInfo`, `SqlError`, `Option`/`Result`), and `daslib/sql_boost` (`[sql_table]` / `[sql_view]` / `[sql_index]`, the adapter rail) publicly — one require covers the whole write-side surface. Add `daslib/sql_linq` for the `_sql` chain macros. **Never `require sqlite` directly** — it's the auto-generated thin C binding (`sqlite3_open`, `sqlite3_prepare_v2`, `sqlite3_step`, …). Going through `sqlite_boost` gets you `SqlRunner` + the typed surface; the raw functions are still reachable when you genuinely need them, but reaching for them is almost always wrong.

The old spellings `require sqlite/sqlite_linq` and `require sqlite/sql_provider` are **hard compile errors** (deprecation stubs) — they moved to `daslib/sql_linq` / `daslib/sql_provider`.

`sqlite/sqlite_migrate` is the only optional sub-module — only require it when the app uses versioned migrations. It transitively brings in `sqlite_boost`, so don't double-require.

The path uses **forward slash** (`require sqlite/sqlite_boost` — not backslash). The `sqlite/*` paths are wired through the [.das_module](../modules/dasSQLITE/.das_module) descriptor.

## Pick the right tool first

When writing or reading SQL, the order of preference is fixed. Reach for the simplest tool that fits.

| Situation | Use | Why |
|---|---|---|
| Open / close a DB for a scoped block | **`with_sqlite(path) <\| $(db) { ... }`** (or `with_latest_sqlite` if migrations) | RAII; closes on panic / early return. Per-provider: `with_duckdb(path)` / `with_postgres(conninfo)` in the external repos |
| Declare a row shape | **`[sql_table(name="...")]` struct** | Generates CREATE / DROP / INSERT / SELECT-row helpers + adapter dispatch. The struct is the single source of truth |
| Query a table you own | **`_sql(db \|> select_from(type<T>) \|> _where(...) \|> _select(...))`** | Compile-time SQL emission, captured-vars auto-bound, type-checked column refs. The flagship; every read-side tutorial uses it |
| Write a declarative reader query | **`%linq! from row : T in db where ... orderby ... select ... %%`** | C#-style reader syntax from `daslib/linq_das`; a typed SQL source lowers through `_fold` into the same `_sql` analyzer, so supported filters, projections, ordering, grouping, and joins remain one SQL statement |
| Stream millions of rows | **`for (row in _each_sql(...))`** | Generator; `sqlite3_finalize` runs in `finally` on break/exhaustion/panic. Same chain shape as `_sql`, rejects materializing terminals |
| Bulk write by predicate | **`_sql_update` / `_sql_delete` / `_sql_upsert`** | Predicate + named-tuple SET in one macro; mirrors `_sql` on the write side |
| Single row by PK | **`db \|> update(row)` / `db \|> delete_(row)` / `db \|> delete_by_id(type<T>, id)`** | Plain functions; `delete_` carries trailing underscore because `delete` is a daslang keyword |
| Insert one or many | **`db \|> insert(row)` / `db \|> insert(arrayOfRows)`** | Bulk overload wraps in a transaction (or savepoint when nested) — 1 fsync, not N |
| One scalar / one row from raw SQL | **`db \|> query_scalar(sql, type<T>, args...)` / `query_one(...)`** | Escape hatch for SQL the macro doesn't translate; positional `?` binds (0..3 args) |
| Set of rows from raw SQL | **`db \|> query(sql, type<T>, args...)`** | Multi-row typed read — same family, returns `array<T>` |
| Statement that returns nothing | **`db \|> exec(sql, args...)`** | DDL, vendor-specific statements, anything `_sql` can't (or shouldn't) translate |
| Inspect emitted SQL while debugging | **`_sql_text(chain)`** | Same analyzer as `_sql`, returns the SQL string with `?` placeholders. Pure debugging aid |
| Schema evolution | **`[sql_migration(version=N)]` + `with_latest_sqlite(path)`** | Versioned, append-only, file-local. See "Migrations" below |

If you find yourself hand-writing `sqlite3_prepare_v2`/`sqlite3_step`/`sqlite3_finalize`, stop — you're below the abstraction line. The boost handles every step. Reach for raw `sqlite3_*` only inside provider-internal code (the boost itself, custom adapters that need streaming BLOB I/O, etc.).

## Strict vs `try_` vs `_opt` — the variant family

Every fallible call has up to two siblings. The naming is consistent across the whole module:

| Form | Returns | When |
|---|---|---|
| `foo(...)` | `T` | Strict; **panics** on error. Default choice for tutorials and app code where a failure is a bug |
| `try_foo(...)` | `Result<T, string>` | Real error modes (constraint violation, BUSY, IO, malformed SQL). Caller wants to log / retry / recover |
| `foo_opt(...)` | `Option<T>` | The only failure mode is "0 rows" — empty is an expected outcome, no errmsg to carry |

Examples: `open_sqlite` / `try_open_sqlite`; `insert` / `try_insert`; `query_scalar` / `try_query_scalar` / `query_scalar_opt`; `query_one` / `try_query_one` / `query_one_opt`; `update` / `try_update`; `delete_` / `try_delete_`; `vacuum` / `try_vacuum`. The LINQ side has `_sql(chain)` (strict) / `_try_sql(chain)` (returns `Result<T, string>` — same chain, panic-free).

Special case for void-returning ops: `try_exec`, `try_attach`, `try_set_pragma`, `try_apply_recommended_pragmas`, `try_vacuum`, `try_backup_to`, `try_create_table`, `try_drop_table_if_exists`, `try_drop_view_if_exists`, `try_check_schema`, `try_transaction` all return `SqlError = Option<string>` (`none = success`, `some(errmsg) = failure`). This is because daslang's `Result<T, E>` doesn't yet support `void` payloads.

`T?` (nullable pointer) is **not** used for these returns — it's a pointer-nullable, wrong for value types. `Option<T>` is the right value-level optional and ships in `daslib/option`.

## Opening a DB — `SqlRunner`

```das
// Canonical scoped form — closes on block exit (incl. panic / early return)
with_sqlite(":memory:") $(db) {
    db |> create_table(type<Car>)
    // ...
}

// Result-returning open — for user-supplied paths
var open_result <- try_open_sqlite("data/app.db")
if (open_result |> is_err) {
    to_log(LOG_ERROR, "could not open: {open_result |> unwrap_err}")
    return
}
var inscope db <- open_result._value     // SqlRunner has [finalize]; must be inscope + bound by move
db |> create_table(type<Car>)            // ... use freely; closes when `db` goes out of scope
```

`SqlRunner` is a `smart_ptr`-style scoped resource (one of the residual types still using refcount semantics — see `skills/gc_migration.md`). Variables holding it need `var inscope` and move-binding (`<-`), not plain `=`. Inside an existing transaction or a `with_sqlite` block, the runner is just passed by value to helpers — no extra ceremony.

`open_sqlite(path)` is the strict form and panics on failure. The third form `with_latest_sqlite(path)` (from `sqlite/sqlite_migrate`) combines `with_sqlite` + `apply_recommended_pragmas` + `migrate_to_latest` into one call — the preferred app-startup form when migrations are in play.

## `[sql_table]` — declare a row shape

```das
[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int                       // INTEGER PRIMARY KEY (rowid alias)
    Name : string                                   // TEXT NOT NULL
    Email : string                                  // TEXT NOT NULL
    @sql_unique
    @safe_when_uninitialized Nick : Option<string>  // TEXT (nullable, since Option<string>)
    Active : bool = true                            // INTEGER NOT NULL DEFAULT 1 (literal field initializer)
    @sql_default_fn = "CURRENT_TIMESTAMP"
    CreatedAt : int64                               // INTEGER NOT NULL DEFAULT CURRENT_TIMESTAMP
    @sql_computed = "length(Name)"
    NameLen : int                                   // GENERATED ALWAYS AS (length(Name)) VIRTUAL
}
```

The struct is the single source of truth. The `[sql_table]` macro reads each field at compile time and generates, attached to type `T`:
- per-type CREATE / DROP / INSERT SQL emitters
- a row binder that binds each field to the right `?` placeholder
- a row reader that materializes a `T` from a step result
- `_sql_table_name`, `_sql_select_all_sql`, `_sql_create_indexes_sql`, `_sql_column_info`

Field annotations:

| Annotation | Effect |
|---|---|
| `@sql_primary_key` | `INTEGER PRIMARY KEY` — the rowid alias, so an unset `int` PK is auto-assigned on insert. Not SQLite's `AUTOINCREMENT` (never emitted). Rejected on `Option<T>` and `@sql_computed` |
| `@sql_column = "..."` | Rename the on-disk column (escape hatch for daslang field names that clash with SQL keywords or the existing schema) |
| `@sql_unique` | Single-column UNIQUE via portable generated index `uq_<table>_<column>` |
| `@sql_references = "Parent"` | Foreign key to another `[sql_table]` struct's primary key |
| `@sql_on_delete` / `@sql_on_update` | One of `cascade` / `set_null` / `set_default` / `restrict` / `no_action`. Only valid with `@sql_references` |
| `@sql_default_fn = "FN"` | Whitelisted SQL built-in (`CURRENT_TIMESTAMP` / `CURRENT_DATE` / `CURRENT_TIME`); emits ` DEFAULT FN` |
| field initializer (`x : T = literal`) | Becomes ` DEFAULT <literal>` for bool / int / float / string. Mutually exclusive with `@sql_default_fn` |
| `@sql_computed = "expr"` | Generated column (DEFAULT VIRTUAL); add `@sql_stored = true` for STORED. Excluded from INSERT/UPDATE bind paths |
| `@sql_json` | TEXT-backed; serialized via `daslib/json` + `daslib/json_boost`. **Queryable**: `_sql` walker descends `_.JsonCol.path.subpath` into `json_extract(...)`. See [tutorials/sql/28-json.das](../tutorials/sql/28-json.das) and `skills/json.md` |
| `@sql_blob` | BLOB-backed; serialized via `daslib/archive`. **Opaque** to `_sql` — descending into a `@sql_blob` column is a compile error |
| `@safe_when_uninitialized` | Required on `Option<T>` fields and on raw `array<uint8>` BLOB fields under `strict_smart_pointers`; lets the row-reader build a default-init row |

Sibling annotation `[sql_index(fields = ..., unique = ..., name = ...)]` lives in the same bracket as `[sql_table]`, comma-separated, with `[sql_table]` first:

```das
[sql_table(name = "UserAccts"),
 sql_index(fields = ("Email", "Tenant"), unique = true)]
struct UserAcct { ... }
```

`fields` is a single string or a tuple of strings. `unique` defaults to `false`. `name` defaults to `idx_<table>_<col1>_<col2>`. Multiple `[sql_index]` lines are stackable. `@sql_unique` uses the same provider-neutral index rail for the one-column case; `[sql_index]` adds explicit naming and composite keys. Composite UNIQUE via `[sql_index(unique = true, fields = (...))]` is the prerequisite for `_sql_upsert` composite-conflict targets.

`db |> create_table(type<T>)` issues the CREATE TABLE + every `[sql_index]` DDL. `db |> drop_table_if_exists(type<T>)` is the idempotent teardown. `db |> check_schema(type<T>)` validates the open DB matches the struct on (name, type, NOT NULL, PRIMARY KEY) — recommended startup pattern for code that opens a DB it didn't just create. **`check_schema` fails on a struct carrying `@sql_computed`**: it compares against `PRAGMA table_info`, which does not list generated columns, so it reports a column-count mismatch. Skip the check for computed-column tables.

## Nullability — `Option<T>` everywhere, never `T?`

Type-driven, no `@sql_nullable` annotation:

- `T` field → column declared `NOT NULL`
- `Option<T>` field → column allows `NULL`

The `[sql_table]` macro inspects each field at compile time:
- DDL emits `NOT NULL` for `T`, omits it for `Option<T>`
- INSERT/UPDATE bind: `is_some(field)` → bind underlying value; `is_none()` → bind SQL NULL
- SELECT reader: checks `sqlite3_column_type == SQLITE_NULL` and wraps in `some(v)` / `none()`

Inside `_sql(...)` predicates, three Option methods translate to SQL null-comparison operators:

| daslang | SQL |
|---|---|
| `_.Col \|> is_some` | `"Col" IS NOT NULL` |
| `_.Col \|> is_none` | `"Col" IS NULL` |
| `_.Col \|> unwrap_or(d)` | `COALESCE("Col", ?)` (d binds) |

Three-valued-logic gotcha: in SQL, `NULL = NULL` is NULL, never TRUE. The Option API steers users away from that footgun. **Never write `_.Col == none()`** — the macro emits a `macro_error` with a fixit pointing at `is_none()`.

Construction at insert time: `none(type<T>)` — the type witness is required. `some(v)` is just `some(v)`. See [tutorials/sql/18-null_handling.das](../tutorials/sql/18-null_handling.das).

## `_sql(chain)` — the LINQ-to-SQL flagship

`_sql(...)` walks a daslib/linq-shaped chain at compile time, classifies each operator, and emits a SQL string + a list of bind expressions. The runtime helper preps, binds, steps, materializes. There is no runtime LINQ→SQL inspection — by program-run-time only the SQL and binds remain.

The `%linq! … %%` reader macro from `daslib/linq_das` is an equal front door for declarative reads. A typed SQL range source such as `from m : Message in db` lowers to `select_from(db, type<Message>)`, then through `_fold` into this same `_sql` analyzer. Prefer the reader form when it makes the query clearer; prefer the explicit `_sql` chain for dynamic composition, SQL-only terminals such as `_first_opt`, or operators not represented by the reader grammar. Do not replace either form with raw `query(...)` for an ordinary typed table read.

```das
let rows <- %linq! from m : Message in db
                   where m.ChatId == chat_id && !m.Deleted
                   orderby m.MessageDate descending
                   select m %%
```

```das
let cutoff = 100
let cars <- _sql(db |> select_from(type<Car>)
                    |> _where(_.Price > cutoff && _.Name |> starts_with("T"))
                    |> _order_by(_.Name)
                    |> _select((Name = _.Name, Price = _.Price))
                    |> take(5))
// SQL: SELECT "Name", "Price" FROM "Cars"
//      WHERE ("Price" > ?) AND ("Name" LIKE ? ESCAPE '\') ORDER BY "Name" ASC LIMIT ?
// Binds: [cutoff, "T%", 5]
```

### What `_sql` translates

| Surface | Examples |
|---|---|
| **Source** | `select_from(db, type<T>)` where `T` carries `[sql_table]` or `[sql_view]` |
| **Filter** | `_where(p)` — multiple calls compose with AND |
| **Project** | `_select(_.Field)` (single column), `_select((A=_.A, B=_.B))` (named-tuple), `_select(_.A + _.B)` (computed scalar → the arithmetic renders into the projection/aggregate), default = full row. Renamed/computed named-tuple entries emit `AS "<name>"`, so result columns carry your record names and a later `_order_by(_.Name)` over a projection alias orders correctly on every provider |
| **Order** | `_order_by(_.Field)`, `_order_by_descending(_.Field)`, tuple-key `_order_by((_.k1, _.k2))` |
| **Group** | `_group_by(_.Key)`, `_group_by((_.k1, _.k2))`; post-group `_having(p)` |
| **Page** | `take(n)`, `skip(m)` — canonical fast form is `skip(m) \|> take(n)`. `take`/`skip` BEFORE an aggregate (`take(n) \|> count()`, `_select(_.X) \|> take(n) \|> sum()`) wraps the bounded rows into an inner subquery so the LIMIT applies pre-aggregate |
| **Distinct** | `distinct()` |
| **Distinct-by** | `_distinct_by(_.K)` — one row per key: first in pk order; `reverse() \|> _distinct_by(_.K)` picks the last. Terminators: row-returning (`to_array` / `_first`, trailing `_order_by`/`take`/`skip` OK), `count()` (→ `COUNT(DISTINCT K)`), `_count(p)`, `_select(_.X) \|> sum/min/max/average()`. Provider-lowered via `caps.distinct_on`: `DISTINCT ON (K)` (PG, DuckDB) or SQLite's bare-aggregate `GROUP BY`. Requires a single `@sql_primary_key`; composing with `_where`/`_join`/`_group_by`/set ops is a compile error (v1) |
| **Aggregate** | Scalar terminals: `count()`, `sum`, `average`, `min`, `max`. One-row summary: `_aggregate($(rows) => (N = rows \|> count, Total = rows \|> _select(_.X) \|> sum, ...))` lowers all named slots into one SELECT |
| **Joins** | `_join(other, $(l, r) => l.X == r.Y, $(l, r) => projection)`, including exact whole-source results (`$(l, r) => r` returns `array<TRight>`); `_left_join(...)` flows the right side as `Option<TB>` through `into` |
| **Subqueries** | `x._in(subq)`, `x._not_in(subq)`, `subq._any()`, `subq._any(p)`, `subq._none()`, `subq._none(p)` |
| **Terminals** | `_to_array()` (default), `_first()` (panic on empty), `_first_opt()` (Option<T>) |
| **String predicates** | `\|> starts_with(s)`, `\|> ends_with(s)`, `\|> contains(s)` (LIKE patterns); `\|> to_lower()`, `\|> to_upper()` (LOWER/UPPER); `length(s)` (LENGTH) |
| **Numeric** | `\|> abs()`, arithmetic (`+ - * / %`), comparisons, `&&` / `\|\|` / `!` |
| **Option** | `\|> is_some`, `\|> is_none`, `\|> unwrap_or(d)` (COALESCE) |
| **JSON descent** | `_.JsonCol.field.subfield` on `@sql_json` columns → `json_extract("JsonCol", '$.field.subfield')` (predicate AND projection) |
| **FTS5** | `_.Col \|> text_match(query)` on an `[sql_fts5]` column → `MATCH ?` |
| **`[sql_function]` UDFs** | A `[sql_function]`-tagged daslang function visible to chain analysis |

### What `_sql` refuses

Compile-time `macro_error` pointing at the offending node:

- Unknown function calls in `_where` / `_select`
- Single-source identity `_select($(x) => x)` remains unsupported (omit `_select` to return the full row). A join's `into` may return one non-nullable source struct directly (`$(l, r) => r`); `_.Field`, named-tuple, and computed-scalar `_.A + _.B` projections are also supported, including workhorse casts (`int64(_.A) * int64(_.B)` lowers to `CAST(...)`)
- Multiple `_select` calls in one chain
- Multiple terminals in one chain (`_to_array() |> _first()`)
- `text_match` on a non-`[sql_fts5]` column (suggests `contains` or `[sql_fts5]`)
- Field descent into `@sql_blob` (opaque; round-trip and filter in daslang, or use raw SQL)
- `_.Col == none()` (fixit → `is_none()`)
- `_each_sql` with a materializing terminal (`_to_array`, `_first`, aggregates) → use `_sql` instead
- `_sql_update` / `_sql_delete` / `_sql_upsert` against a `[sql_view]`

### Inspecting emitted SQL

`_sql_text(chain)` shares the analyzer with `_sql` but returns the SQL string instead of running it. Use freely while debugging:

```das
to_log(LOG_INFO, "SQL: {_sql_text(db |> select_from(type<Car>) |> _where(_.Price > 100))}")
```

`options log` on the file dumps the post-macro daslang code — useful for seeing the runtime helper call the analyzer emits.

### Several global aggregates in one scan

Use `_aggregate` when one answer needs several facts about the same
filtered source:

```das
let stats = _sql(
    db |> select_from(type<Order>)
       |> _where(_.State == wanted)
       |> _aggregate($(rows) => (
            N = rows |> count,
            Total = rows |> _select(_.Amount) |> sum,
            First = rows |> _select(_.CreatedAt) |> min,
            Last = rows |> _select(_.CreatedAt) |> max)))
```

The result is a typed named tuple and the provider executes one query
with one result row. The same expression works on an in-memory array.
V1 accepts filtered table or join sources and field selectors; it
compile-errors on pre-projection, grouping, distinct/set operations,
ordering, or paging until explicit subquery composition exists.

### Composability

Inner `_where` / `_select` compose freely. User-defined `[call_macro]` wrappers cascade through the analyzer — write `[call_macro(name="when_price_lt")]` that expands to `_where(it, _.Price < val)` and use `_sql(db |> select_from(type<Car>) |> when_price_lt(200))`. The wrapper's body becomes the same `where_` shape the analyzer sees for a hand-written `_where`.

## `_each_sql` — streaming

```das
for (e in _each_sql(db |> select_from(type<LogEntry>) |> _where(_.Severity >= 2))) {
    process(e)
    break if (budget_exhausted)
}
```

Same chain shape as `_sql`, but returns `iterator<T>` instead of materializing `array<T>`. Lifecycle: `prepare → bind → step+yield → … → finalize`. The `sqlite3_finalize` runs in the generator's `finally`, so the statement is released on:
- normal exhaustion
- early `break`
- panic from inside the loop body

Subsequent writes on the same connection are **not** blocked by a leaked read transaction — verified by [tutorials/sql/35-streaming.das](../tutorials/sql/35-streaming.das).

Pick `_each_sql` when the result set is large, you `break` early, or downstream is iterator-shaped. Pick `_sql` when the set is small, you need to index/count, or the downstream takes `array<T>`. Concurrent-writer note: while the iterator is stepping, SQLite holds a read transaction. Default journal mode (`DELETE`) blocks writes from the same connection until the cursor finalizes — switch to WAL (`db |> set_pragma("journal_mode", "WAL")` or `apply_recommended_pragmas`) for reader+writer coexistence on the same connection.

A daslang generator is **consumed once**. To re-run, write the call site again — each evaluation builds a fresh prepared statement. Don't capture into a variable then iterate twice; the second pass yields zero rows.

## Writes — `insert` / `update` / `delete_` / upsert

### Plain functions (per-row by PK)

```das
let new_id = db |> insert(Car(Id = 0, Name = "Tesla", Price = 60000))
db |> insert([Car(...), Car(...)])             // bulk; one BEGIN/COMMIT (or SAVEPOINT if nested)
let n_updated = db |> update(Car(Id = 1, Name = "Tesla", Price = 70000))
let n_deleted = db |> delete_(Car(Id = 1, ...))     // trailing _ — `delete` is a daslang keyword
let n_by_id   = db |> delete_by_id(type<Car>, 1)
```

Each has a `try_` sibling returning `Result<int, string>` (`Result<int64, string>` for `try_insert` — the rowid). Bulk insert on `array<T>` is a single transactional unit: 8 rows = 1 fsync, not 8.

`db |> changes() : int` reports rows-affected from the most recent INSERT / UPDATE / DELETE on the connection — useful after raw `exec` for dynamic SQL.

### Predicate-based bulk macros

```das
// UPDATE
let stale = db |> _sql_update(type<User>,
    _.LastSeen < cutoff,
    (Active = false))

// UPDATE … RETURNING — capture rows AFTER the update
let revived <- db |> _sql_update_returning(type<User>,
    _.Id == 4,
    (Active = true, LastSeen = touch_at))

// DELETE
let n = db |> _sql_delete(type<User>, _.Email == "")

// UPSERT — INSERT … ON CONFLICT(...) DO UPDATE SET ...
//   _      = the existing row
//   _excluded = the row we tried to INSERT
let bumped = db |> _sql_upsert(
    WordHit(Id = 1, Word = "hello", Hits = 1, Last = 1234l),
    _.Id,                                        // single-key conflict
    (Hits = _.Hits + 1, Last = _excluded.Last))

// Composite-conflict — needs a [sql_index(unique = true, fields = (...))]
db |> _sql_upsert(
    UserAcct(Id = 999, Email = "x@y.com", Name = "alice2", Tenant = "acme"),
    tuple(_.Email, _.Tenant),
    (Name = _excluded.Name))

// Default integer PK is unset: omit it so the provider auto-assigns it.
// Useful when conflict targets another UNIQUE column.
let inserted <- db |> _sql_upsert_returning(
    WordHit(Id = 0, Word = "new", Hits = 1, Last = 1234l),
    _.Word,
    (Hits = _.Hits + _excluded.Hits))
```

Each macro has the four-way fan-out: `_sql_update` / `_sql_try_update` / `_sql_update_returning` / `_sql_try_update_returning`, parallel for `_sql_delete` and `_sql_upsert`.

For `_sql_upsert`, a default-valued integer `@sql_primary_key` is treated as unset just like `insert`: the macro omits that column so SQLite can assign rowid, DuckDB can draw from its sequence, and PostgreSQL can use its identity column. Explicit non-default primary keys remain in the INSERT. This applies to strict/`try_` and returning/non-returning variants.

The `_sql_` prefix is deliberate — it carries SQL provenance to the call site and avoids name collisions with hypothetical future non-SQL `_update` / `_delete` macros. Function siblings stay unprefixed (`update` / `delete_` / `delete_by_id`).

`insert_or_ignore(row)` and `insert_or_replace(row)` are plain functions for the simpler "ignore conflict" / "wipe-and-reinsert" cases — both have single + bulk + `try_` overloads. Reach for `_sql_upsert` when you actually want to merge rather than replace.

## Transactions

```das
db |> with_transaction() {
    db |> insert([Friend(Id = 1, Name = "tom"), Friend(Id = 2, Name = "jerry")])
}

db |> with_transaction(SqliteTxnMode.Immediate) {
    db |> insert(Friend(Id = 4, Name = "jasmine"))
}

let r = db |> try_transaction() {
    db |> insert(Friend(Id = 7, Name = "lola"))
}
if (r |> is_some) {
    to_log(LOG_ERROR, "txn failed: {r |> unwrap}")
}
```

`with_transaction` emits `BEGIN` on entry, `COMMIT` on normal exit, `ROLLBACK` on early return (a block `return` unwinds non-locally through `finally`). **A panic does NOT roll back** — daslang deliberately skips `finally` on panic (see "Error handling" in CLAUDE.md), so the transaction is left open and its writes are discarded when the connection closes; that's consistent with panic-is-fatal (print diagnostics and exit — don't continue after `recover`). Nested calls fall back to `SAVEPOINT` / `RELEASE` / `ROLLBACK TO` (savepoint name `das_sp`) so user code composes freely without SQLite's "no nested transactions" rule biting. Bulk `insert(array<T>)` likewise auto-uses a savepoint when nested.

Two distinct overloads (one with `mode`, one without) — not one with an optional middle parameter, because the trailing-block convention puts the block last and an optional middle wouldn't resolve from a no-mode call site. Same for `try_transaction`.

`SqliteTxnMode`: `Deferred` (default; lock acquired on first write), `Immediate` (RESERVED lock at BEGIN — avoids the "another writer raced us between read and write" trap), `Exclusive`. The mode overload is SQLite-specific; DuckDB / PostgreSQL providers ship the mode-less `with_transaction` / `try_transaction` only (nesting still composes — savepoints on PG, depth tracking on DuckDB).

`db |> in_transaction() : bool` wraps SQLite's autocommit flag — useful for library code that wants "join an ambient transaction if one is active, else start one".

`try_transaction` only converts SQL failures (BEGIN / COMMIT errors) into `some(errmsg)` and returns `none` on success. A panic from inside the block propagates without rolling back (same `finally`-skipped-on-panic rule as `with_transaction`) — a fallible step inside a transaction should use the `try_` API forms and early-return, not panic-and-recover.

## Custom types — `sql_bind` / `sql_extract`

Every SQL-addressable type `T` has a bidirectional pair of named functions:

```das
def sql_bind    (v : T) : P                 // T → primitive
def sql_extract (v : P; t : type<T>) : T    // primitive → T
```

where `P` is one of `int64`, `double`, `string`, `array<uint8>` — the four SQLite storage classes. The **return type** of `sql_bind` IS the storage type; the `[sql_table]` macro reads it and emits the matching `sqlite3_bind_*` / `sqlite3_column_*` call.

```das
struct DateTime { unix_seconds : int64 }

def sql_bind(dt : DateTime) : int64 { return dt.unix_seconds }

[unused_argument(t)]
def sql_extract(v : int64; t : type<DateTime>) : DateTime {
    return DateTime(unix_seconds = v)
}
```

Convention dispatch — no `register_sql_type()` call, no runtime lookup. Defining the pair next to the type IS the registration. The `[sql_table]` macro emits `_::sql_bind(...)` so the user's-module overloads are visible at expansion (same mechanism as `_::clone` / `_::finalize`).

dasSQLITE ships passthrough overloads for `int64`/`double`/`string`/`array<uint8>` and widening overloads for the rest of the integer / float family + `bool`. Enums round-trip through INTEGER via a generic overload guarded by `static_if (typeinfo is_enum(default<TT>))`. `Option<T>` over a custom adapter just works — the bind/read sites unwrap to `T` and recurse.

If a `[sql_table]` field has a type with no `sql_bind` / `sql_extract` pair in scope, the macro emits a literal `_::sql_bind(field)` call and overload resolution falls through to a catch-all generic that compile-errors with the missing-adapter signature. No runtime "type not registered" — all compile-time.

[tutorials/sql/26-custom_types.das](../tutorials/sql/26-custom_types.das) walks the rail end-to-end including `Guid` over `array<uint8>` (BLOB) and `OrderStatus` enum (auto-handled).

## JSON / BLOB columns

Two complementary annotations for shapes that don't belong in their own columns:

```das
[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name                : string
    @sql_json
    Prefs : tuple<theme : string; lang : string; notify : bool>
}

struct SessionSnapshot { last_scene : string; unlocks : array<int>; last_tick_ms : int64 }

[sql_table(name = "Sessions")]
struct SessionRow {
    @sql_primary_key UserId : int
    StartedAt : int64
    @sql_blob State : SessionSnapshot
}
```

| Annotation | Storage | `_sql` walker descent | Encode/decode | Schema-drift |
|---|---|---|---|---|
| `@sql_json` | TEXT (via `daslib/json` + `daslib/json_boost`) | YES — `_.Col.path.subpath` → `json_extract(...)`, predicate AND projection, arbitrary depth | Slower | Tolerant — missing fields default, extras ignored |
| `@sql_blob` | BLOB (via `daslib/archive`) | NO — compile error on descent | Faster, smaller | Fragile — positional binary; adding a field breaks old rows |

Both ride the custom-types adapter rail: when `[sql_table]` sees the annotation, it generates the matching `sql_bind` / `sql_extract` pair at module scope. EXPLICIT OPT-IN: a struct/tuple-typed column without `@sql_json` / `@sql_blob` is a compile error from the catch-all generic.

The walker reads the annotation off the **field**, not off the type. A user who writes a custom string-returning `sql_bind(MyStruct)` (CSV, protobuf-text, etc.) gets TEXT storage but NOT walker descent — they use raw SQL for any structure-aware query.

JSON-path descent extends through nested struct fields:

```das
[sql_table(name = "Accounts")]
struct Account {
    @sql_primary_key Id : int
    @sql_json Profile : Profile        // struct Profile { Addr : Address; Bio : string }
}

let in_berlin <- _sql(db |> select_from(type<Account>)
                        |> _where(_.Profile.Addr.City == "Berlin"))
// SQL: SELECT "Id", "Profile" FROM "Accounts" WHERE json_extract("Profile", '$.Addr.City') = ?
```

Both forms panic on bad stored data — malformed JSON trips `read_json`, corrupt archive bytes trip `mem_archive_load`. `try_select_from` / `try_query` only catch sqlite3-level prepare/step errors; they do NOT convert adapter panics to `Err`. See `skills/json.md` for JSON value modeling. See [tutorials/sql/28-json.das](../tutorials/sql/28-json.das) for the full surface.

## Views — `[sql_view]` + `_create_view`

A view is a saved SELECT — a virtual read-only table whose rows are produced on demand by re-running its body. Same struct-driven story as a table:

```das
[sql_view(name = "BigOrders")]
struct BigOrder {
    Id : int
    CustomerId : int
    Amount : int
    Status : string
}

db |> _create_view(type<BigOrder>,
    db |> select_from(type<Order>) |> _where(_.Amount >= 100))

// Same read surface as a table:
for (b in db |> select_from(type<BigOrder>)) { ... }
let big <- _sql(db |> select_from(type<BigOrder>) |> _where(_.Status == "shipped"))
```

`[sql_view]` accepts the read-only subset of field annotations: `@sql_column`, `@sql_json`, `@sql_blob`. Anything affecting underlying-table DDL is rejected (keys, uniqueness, computed, defaults, foreign keys, `[sql_index]`).

Mutation paths are blocked:
- Predicate-form (`_sql_update(type<V>, ...)`, `_sql_delete(...)`, `_sql_upsert(viewRow, ...)`) — compile-time error
- Row-form (`insert(viewRow)` / `update(viewRow)` / `delete_(viewRow)`) — runtime panic with "views are read-only"

`db |> drop_view_if_exists(type<V>)` is the idempotent teardown.

**Captured locals via `to_sql_literal`**: SQLite stores view bodies as TEXT in `sqlite_schema` and rejects `?` inside DDL, so `_create_view` cannot use `?` binds. Instead it stringifies each bind expression once at view-creation time and inlines the literal. All numeric primitives, `bool`, and `string` work out of the box. User types extend with a one-line overload:

```das
enum Status { Pending = 1, Shipped = 2 }
def to_sql_literal(s : Status) : string => "{int(s)}"
```

Types without an overload hit the catch-all in `sql_linq.das` (enums emit as `int(v)`, otherwise compile error pointing at "define a one-line overload in YourType's module"). The value is **frozen at view-creation time** — changing the captured local later does not update the view; drop and re-create. Runtime `_sql(...)` queries against the view still honor `?` binds normally — it's only DDL that can't.

For window functions, recursive CTEs, custom helpers the chain doesn't translate, drop to raw `db |> exec("CREATE VIEW … AS …")` and declare a matching `[sql_view]` struct for the read side.

## FTS5 — full-text search (SQLite-only, `caps.fts5`)

```das
[sql_fts5(name = "docs_idx")]
struct Doc {
    @sql_fts_unindexed Id : int64  // stored/filterable metadata, excluded from MATCH
    Body : string
    @sql_fts_rank Rank : float       // FTS5 fills on SELECT (BM25); INSERT skips
}

db |> create_table(type<Doc>)                    // CREATE VIRTUAL TABLE … USING fts5(...)
db |> insert(Doc(Id = 42l, Body = "The quick brown fox jumps"))

let foxes <- _sql(db |> select_from(type<Doc>)
                     |> _where(_.Id == 42l && (_.Body |> text_match("quick fox")))
                     |> _order_by(_.Rank))
// SELECT "Id", "Body", rank FROM "docs_idx" WHERE ("Id" = ?) AND ("Body" MATCH ?) ORDER BY "Rank" ASC

let removed = db |> _sql_delete(type<Doc>, _.Body |> text_match("obsolete"))
```

`@sql_fts_unindexed` emits FTS5's `UNINDEXED` column suffix. Use it for typed IDs, timestamps, kinds, or tenant/chat metadata that must round-trip with hits and participate in ordinary comparisons without being tokenized. The normal `sql_bind` / `sql_extract` adapter rail supplies its storage type. `text_match` on an unindexed field is a compile error.

`text_match(col, query)` lowers to `col MATCH ?`. The same predicate is also a `daslib/strings_boost` (via `daslib/fts5_query`) in-memory matcher — one call site for both SQL and in-memory filtering. `text_match` on a non-`[sql_fts5]` column is a compile error pointing at `contains` (LIKE-based) or adding `[sql_fts5]`.

Query-string syntax: whitespace-AND, `*` prefix (`quick*` matches `quicksand`), `"phrase match"` (quoted), Boolean `AND` / `OR` / `NOT` (uppercase), `NEAR(a b, N)` proximity. Default tokenizer is `unicode61` (Unicode word boundaries + ASCII case-fold).

Limitations: self-contained mode only (FTS5 holds both content and index); row-shaped UPDATE / DELETE-by-PK helpers are unavailable because the virtual table has no declared daslang PK. Predicate DELETE is typed through `_sql_delete(type<T>, predicate)`; update by deleting and re-inserting, or use raw SQL. BM25 weighting / snippet helpers / per-column query filters work via the FTS5 query string but lack typed wrappers.

## User-defined SQL functions — `register_function` / `[sql_function]` (`caps.client_udfs`)

SQLite and DuckDB only — PostgreSQL has no client-side UDFs, so `[sql_function]` use in a chain against a PG runner is a compile-time error (`caps.client_udfs` gate). The annotation itself is provider-neutral (ONE `[sql_function]` in `daslib/sql_boost`): a tagged function lands in a type-erased queue that every UDF-capable provider replays against each connection it opens — tag once, visible on SQLite and DuckDB connections alike.

Two flavors. Per-connection `register_function` for explicit one-off registration; `[sql_function]` annotation for auto-registration on every open + chain visibility.

```das
def damage(base : float; armor : int; mult : float) : float => base * (1.0f - float(armor) / 100.0f) * mult

// Per-connection
db |> register_function("damage", @@damage, true /* deterministic */)
let n = db |> query_scalar("SELECT COUNT(*) FROM Enemies WHERE damage(100.0, Armor, 1.5) > 50", type<int>)

// Or auto-register everywhere + visible to _sql chains
[sql_function(deterministic=true)]
def score(hp : float; armor : int) : double => double(hp) * (1.0lf - double(armor) / 100.0lf)

let strong <- _sql(db |> select_from(type<Enemy>)
                      |> _where(score(_.Hp, _.Armor) >= 100.0lf))
```

**Supported scalar set**: `int`, `int64`, `float`, `double`, `bool`, `string`. Up to 4 args. Structs / arrays / pointers / lambdas / classes are rejected at compile time with a per-position diagnostic. `@@fn` (function pointer literal) — not lambdas; lambdas are rejected.

**NULL handling (v1)**: any SQLITE_NULL argument short-circuits to `sqlite3_result_null` without invoking the daslang function. Idiomatic SQL behavior of built-in scalars (`abs(NULL) → NULL`).

**Panic recovery**: a panic inside the daslang function is caught (`Context::runWithCatch`), surfaced via `sqlite3_result_error`. The connection itself is unaffected; subsequent statements run normally.

**Flags**:
- `deterministic=true` → `SQLITE_DETERMINISTIC` (allows the fn in `CREATE INDEX … ON tbl(myfn(col))` and lets the planner factor it out of inner loops)
- `directonly=true` → `SQLITE_DIRECTONLY` (blocks invocation from triggers / views / CHECK constraints)

**`[sql_function]` install**: the annotation queues an entry into the neutral `daslib/sql_boost` registry; every UDF-capable provider's open path (`try_open_sqlite` / `with_sqlite`, `with_duckdb`, …) installs every entry against the new connection. Duplicate `(name, arity)` registration across modules → install-time `SqlError`. Manual `register_function` after open overrides the registry's binding.

`[sql_function]` is the right shape for ambient SQL helpers that should be visible to chain analysis everywhere; `register_function` is for one-off / per-connection registrations.

## Migrations — `daslib/sql_migrate` + per-provider glue

Versioned, append-only schema evolution, provider-neutral. The engine and the
`[sql_migration]` / `[struct_convert]` annotations live in `daslib/sql_migrate`; each
provider ships a glue module that re-exports it — require the glue, never the engine
directly: `sqlite/sqlite_migrate`, `duckdb/duckdb_migrate`, or `postgres/postgres_migrate`
(see PROVIDER_CONTRACT.md §Migrations for the glue hook contract). Optional — only require
it when the app uses versioned migrations. The migration body's runner parameter type
selects the provider **stream**: versions are unique per stream, so SQLite and PostgreSQL
migration sets number independently in one program.

```das
require sqlite/sqlite_migrate

[sql_table(name = "users")]
struct User {
    @sql_primary_key Id : int
    Name : string
    Email : string                   // added in migration 2
    Score : int = 0                  // added in migration 3 with default
}

[sql_migration(version = 1, description = "create users")]
def migration_001(db : SqlRunner) {
    db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL)")
}

[sql_migration(version = 2, description = "add email column")]
def migration_002(db : SqlRunner) {
    db |> exec("ALTER TABLE users ADD COLUMN Email TEXT NOT NULL DEFAULT ''")
}

[sql_migration(version = 3, description = "add score; backfill", analyze = true)]
def migration_003(db : SqlRunner) {
    db |> exec("ALTER TABLE users ADD COLUMN Score INTEGER NOT NULL DEFAULT 0")
    db |> exec("UPDATE users SET Score = 100 WHERE Email != ''")
}

[export]
def main() : int {
    with_latest_sqlite(":memory:") $(db) {
        // DB is now at the latest schema
        db |> exec("INSERT INTO users (Name, Email) VALUES ('alice', 'alice@x')")
    }
    return 0
}
```

### The five rules

1. **Append-only.** Once shipped, a migration is FROZEN. Never edit `migration_002` after release — write `migration_003` that does the additional change. The mechanism: a user whose DB is already at v2 will never re-execute v2 on their machine; edits to v2 wouldn't reach existing users.
2. **Versions are unique per provider stream.** Two devs picking `version=2` on parallel branches (for the same runner type) is a **compile-time error** — the macro scans the `[sql_migration]` registry across modules, grouped by the body's runner type. Resolution: renumber the later branch. Migrations for *different* providers (e.g. `db : SqlRunner` from sqlite_boost vs postgres_boost) live in separate streams and may share version numbers.
3. **Strict-forward pending detection.** Pending = registered versions where `version > MAX(applied)`. Set-difference (EF-style) is **not** how this works.
4. **One transaction per call (α-shape).** All pending migrations + their audit inserts share one transaction (SQLite: `BEGIN IMMEDIATE`; PostgreSQL: session advisory lock + `BEGIN`; DuckDB: `BEGIN`). If any migration fails, the whole call rolls back — re-run replays the entire pending list. Implication: prior successful migrations in the same call get rolled back too. Long-running backfills should live in their own `migrate_to_latest` invocation if replay cost matters.
5. **Body is plain daslang.** Raw `db |> exec(...)`, typed DDL (`add_column` / `create_index` / `drop_index_if_exists` / `create_table` / `drop_table_if_exists`), typed DML (`_sql_update` / `insert` / `_sql_upsert` / etc.), reads (`_sql(...)`), arbitrary control flow. No special migration sub-DSL.

### The struct-vs-migration disconnect

Your `struct User` describes the **current** schema. You cannot say `db |> create_table(type<User>)` inside `migration_001` — it would emit the latest shape, not the v1 shape. So early migration bodies use raw SQL. There is **no** model snapshot system (deliberately — would require frozen-struct tooling daslang doesn't have).

The `[sql_table]` struct is a **partial projection** of the table, not a mirror: columns that exist in the DB but not in the struct are silently ignored on read and untouched on write. Consequence: not every migration requires a struct change. Adding `LastLoginAt` for an analytics dashboard means: write the migration, ship the new binary (struct unchanged), the column accumulates data. Months later when code wants to read `LastLoginAt`, that's when the struct gets the field. Matches EF Core's entity-as-partial-projection model.

### Surface

```das
def with_latest_sqlite(path, blk)            // open + apply_recommended_pragmas + migrate_to_latest, RAII
def migrate_to_latest(db) : int              // applied count (panics on fail)
def try_migrate_to_latest(db) : Result<int, string>

def baseline(db, version : int)              // stamp v1..vN as already-applied without running bodies
def try_baseline(db, version : int) : Result<int, string>

// Pure reads — never create the audit table or write rows
def current_schema_version(db) : int                  // 0 if __schema_version doesn't exist yet
def pending_migrations(db) : array<PendingMigration>  // {version, description (from annotation)}
def migration_history(db) : array<MigrationRecord>   // {version, description (frozen at apply), applied_at}
```

### Audit table

`__schema_version (version INTEGER PRIMARY KEY, description TEXT, applied_at BIGINT)`. Eagerly created; `applied_at` is stamped client-side (epoch seconds) so the insert is portable across engines. Success-only rows — failed migrations roll back the audit insert atomically with their own body. `description` in `migration_history` is **frozen at apply time** (DB wins); `pending_migrations` reads from the annotation. Editing a description on an already-shipped migration is a no-op for any DB past it.

### Annotation parameters

- `version=N` — required, integer
- `description="..."` — optional; lands in `__schema_version` + `to_log` lines
- `vacuum=true` — runs `VACUUM` once after the call commits
- `analyze=true` — runs `ANALYZE` once after the call commits (cheap-then-expensive: ANALYZE first, then VACUUM if both)

VACUUM/ANALYZE failures log a `LOG_WARNING` but do **not** roll back — the schema changes are already durable, and these are optimizations not correctness.

### Adoption — pre-existing DB

Real-world story: app shipped without migrations, existing user DBs already have tables. Naive swap to `with_latest_sqlite` would crash because `migration_001`'s `CREATE TABLE` fails on the existing table.

Two layers of help:
- **Layer 1**: when `migrate_to_latest` detects user objects in a freshly-empty audit table, it logs a `LOG_WARNING` with the baseline recipe. Informational, not a stop.
- **Layer 2**: if the first migration fails AND the audit table was just created from empty, the panic / `Result::Err` message is enriched with the `baseline(version=N)` hint and a "DB does not need to be reset" line.

`baseline(db, version=N)` stamps `v1..vN` as already-applied (with `[baseline]` description if no annotation exists for that version). Idempotent. **Recommended pattern**: prefer first-change-as-first-migration — make `migration_001` an `ALTER TABLE` that fits both fresh and existing DBs, rather than recreating v0 state. Reach for `baseline()` only when you specifically want CREATE-TABLE-shaped retroactive history.

### Concurrent runners

Per-provider coordination behind the same surface. SQLite: the RESERVED lock — `apply_recommended_pragmas` (called by `with_latest_sqlite`) sets `busy_timeout = 5000ms`, so concurrent migrators block on `BEGIN IMMEDIATE` for 5 seconds before erroring. PostgreSQL: a session advisory lock keyed on the current database — `migrate_to_latest` / `baseline` **block indefinitely** waiting for a concurrent migrator (the session lock auto-releases if the holder crashes or disconnects, so there's no stuck-lock failure mode); `try_migrate_to_latest` / `try_baseline` use `pg_try_advisory_lock` and **fail fast** with `err` instead. DuckDB: the engine's single-writer lock covers it. Three pods racing on startup → one wins, the others wait, all end up at the latest version with no special application logic; a re-check under the lock skips work another runner already did.

### Failure semantics

If migration v5 fails midway through `[v4, v5, v6]`:
1. `LOG_ERROR` line names the failing migration + collateral rollbacks
2. Panic / `Result::Err` enriched: "v4 and v5 are NOT applied. The DB is still at v3. Re-run the corrected migration set."
3. v6 doesn't run

Body discipline: **migration bodies should be DB-only**. Anything that touches the outside world (HTTP, file writes, `system(...)`) is NOT rolled back when the migration fails; if the migration retries, those side effects re-execute. For genuinely cross-system migrations, sequence them outside the runner.

## PRAGMA, VACUUM, backup (SQLite-specific surface)

```das
db |> set_pragma("journal_mode", "WAL")          // string / int64 / bool overloads
db |> apply_recommended_pragmas()                // WAL + busy_timeout=5000 + foreign_keys=ON + synchronous=NORMAL

db |> vacuum()                                   // rejects active txn before reaching SQLite
db |> vacuum_into("snapshot.db")
db |> optimize()
let issues <- db |> integrity_check()            // ["ok"] healthy, otherwise per-issue rows
let quick  <- db |> quick_check()

db |> backup_to(dest_runner)                     // online Backup API; auto-retries on SQLITE_BUSY
db |> backup_to("backup.db")                     // path overload opens internally + finalizes
```

All have `try_` siblings returning `SqlError` (or `Result<array<string>, string>` for the `*_check` family). VACUUM cannot run inside a transaction (SQLite restriction); the wrapper catches that with a friendlier preflight before SQLite errors. PRAGMA values are inlined into `PRAGMA "name" = value` — SQLite parses them at prepare time and rejects `?` binds for pragmas.

`apply_recommended_pragmas` is the canonical "sane defaults" call — WAL enables reader+writer coexistence, busy_timeout gives concurrent runners a wait window, foreign_keys=ON makes `@sql_references` constraints actually fire, synchronous=NORMAL is the WAL-appropriate durability/perf tradeoff.

## ATTACH (SQLite-specific surface)

```das
db |> attach("other.db", "ext")                  // try_attach for non-panic
db |> with_schema("ext") $(arch) {
    let rows <- _sql(arch |> select_from(type<RemoteRow>))
}
db |> detach("ext")

// attach + detach in one — the block param is already schema-qualified
db |> with_attached("other.db", "ext") $(tenant) {
    let rows <- _sql(tenant |> select_from(type<RemoteRow>))
}
```

`with_schema(name) $(scoped) { ... }` invokes the block with a runner that qualifies every query as `"<schema>"."<table>"`. It is block-shaped and returns `void` deliberately: the qualified runner aliases `db`'s `sqlite3*` handle and must not outlive it, so it can't be `inscope`'d. `with_attached` attaches, hands you exactly such a runner, and detaches in `finally` — don't call `with_schema` again inside it. Because the attached DB shares the same `sqlite3*` handle, `[sql_function]`-registered functions stay visible without a second install hook.

## Raw-SQL escape hatch

When `_sql` won't translate (window functions, recursive CTEs, dialect-specific syntax, dynamic SQL), drop down to:

```das
db |> exec("INSERT INTO Users (Name) VALUES (?)", "alice")          // 0/1/2/3 positional binds
let v = db |> query_scalar("SELECT COUNT(*) FROM Users", type<int>)
let row = db |> query_one("SELECT Id, Name FROM Users WHERE Id = ?", type<User>, 42)
let opt = db |> query_one_opt("SELECT Id, Name FROM Users WHERE Email = ?", type<User>, email)
let many <- db |> query("SELECT type, name FROM sqlite_master WHERE type=?", type<MasterTable>, "table")
```

The row struct of a raw `query` / `query_one` **must carry `[sql_table]`** — a plain struct fails deep inside the boost with `error[30341] … _::_sql_read_row(sqlite3_stmt? const&, MasterTable)`. And `type` is a daslang keyword, so a column named `type` needs a renamed field:

```das
[sql_table(name = "sqlite_master_rows")]
struct MasterTable {
    @sql_column = "type"
    row_type : string
    name : string
}
```

All have `try_` and (for the read family) `_opt` siblings. 4+ positional binds isn't shipped — if you need them, build the SQL with the values inlined for trusted constants, or stick with `_sql` macros for user-controlled values.

`query_scalar` ships overloads for `int`, `int64`, `float`, `double`, `bool`, `string`. Other types come through the typed `query_one` family with a `[sql_table]`-shaped struct.

For listing tables, columns, etc., catalog spelling is genuinely provider-specific — there's no abstract `list_tables` helper. Use `query("SELECT type, name FROM sqlite_master WHERE type='table'", type<MasterTable>)` with the annotated struct above. See [tutorials/sql/30-list_tables.das](../tutorials/sql/30-list_tables.das).

## Threading and contexts

Each context has its own heap. `SqlRunner` is bound to the context that opened it — **don't share a runner across threads**. Either:
- Open a separate `SqlRunner` per thread (preferred — SQLite's WAL mode handles concurrent readers + one writer well)
- Funnel all DB calls through one thread via a channel

Strings produced by `query` / `_sql` are allocated on the calling context's heap — if you ship them to another context, clone (`clone_string(s)` or `:=` for fresh allocation in the destination's heap; `=` just copies the pointer, unsafe across contexts).

## Patterns and anti-patterns

| Don't write | Write instead | Why |
|---|---|---|
| `_.Col == none()` | `_.Col \|> is_none` | Three-valued logic; the macro emits a fixit `macro_error` |
| `if (length(arr) == 0) ...` after `_sql` | `_sql(... \|> _first_opt())` then check `is_some` | Single-row materializer avoids building the full array |
| `if (try_op() \|> is_err) panic(...)` | `op()` (strict form) | Strict already panics with the SQLite errmsg |
| `db \|> exec("SELECT ...")` | `query` / `query_one` / `query_scalar` | `exec` is for statements with no result |
| `db \|> exec(format(...))` with user data | `_sql(db \|> select_from(type<T>) \|> _where(_.X == val))` | Captured-var bind avoids SQL injection; the macro emits `?` |
| `var inscope db = ...; db |> ...; db.close()` (manual) | `with_sqlite(path) <\| $(db) { ... }` | RAII; closes on panic / early return |
| Raw `sqlite3_prepare_v2` + `sqlite3_step` loop | `_each_sql(...)` or `query(...)` | The boost handles binding, materialization, finalize |
| Manual rollback after error in a `with_transaction` block | Just `panic` or `return` | `finally` rolls back automatically |
| Editing `migration_002` after release | Write `migration_003` | Append-only — existing DBs never re-run an old migration |

## Common gotchas

- **`delete_(row)` has a trailing underscore** — `delete` is a daslang keyword. `delete_by_id(type<T>, id)` does not (different name).
- **Hex literals are `uint`** — `_.Status == 0x01` doesn't typecheck against `int`. Use `int(0x01)` or `Status.Foo` enum.
- **`_.Col == none()` doesn't translate** — use `is_none()` / `is_some()`. The macro emits a fixit pointing at the right form.
- **Generator one-shot** — `_each_sql` returns an `iterator<T>` that's consumed once. Re-running the same query rebuilds the prepared statement; don't capture and iterate twice.
- **PRAGMA can't use `?` binds** — values are inlined into the SQL string by `set_pragma`; same for VACUUM INTO's path. `_sql(...)` `?` binds are fine in regular SELECTs.
- **Views can't use `?` binds in their body** — `_create_view` inlines literals via `to_sql_literal`; values are frozen at creation time.
- **`@safe_when_uninitialized` on `Option<T>` fields** — under `strict_smart_pointers` the `[sql_table]` row reader needs it on Option-bearing structs and on raw `array<uint8>` BLOB fields.
- **`check_schema` and `@sql_computed` don't mix** — it reads `PRAGMA table_info`, which omits generated columns, so it reports a column-count mismatch on a table that is in fact correct.
- **A raw-`query` row struct needs `[sql_table]`** — and a column whose name is a daslang keyword (`type`, `class`) needs `@sql_column = "type"` on a renamed field.
- **`Result::Err` from `try_select_from` only catches sqlite3-level errors** — adapter panics (malformed JSON, corrupt archive) are NOT converted to Err. Validate JSON shape externally if you need defensive reads.
- **Bulk insert composability** — `try_insert(rows)` auto-uses `BEGIN IMMEDIATE/COMMIT` standalone, `SAVEPOINT/RELEASE` when nested. Don't wrap a bulk insert in a manual `with_transaction` block expecting to skip the inner txn — the inner already does the right thing.
- **String predicate operators emit LIKE patterns** — `starts_with(s)`, `ends_with(s)`, `contains(s)` use `LIKE ? ESCAPE '\'` with `%` / `_` escaped via the helpers `like_escape_prefix` / `like_escape_suffix` / `like_escape_substring`. They can't use a B-tree index for substring search — for large columns prefer `[sql_fts5]` with `text_match`.
- **Inner-lambda parameter naming in `_group_by` aggregates** — write `$(u : User) => u.Salary`, not `$(_) => _.Salary`. The outer `_select` already binds `_` to the group tuple; daslang flags same-name shadowing inside.
- **Dot-on-`Option<T>`** — `result.is_some()` doesn't work because `Result` / `Option` are class-style with method calls; use the pipe form `result \|> is_some` everywhere.
- **No `print` in tests / daslib / utils** — use `to_log(LOG_INFO, ...)`. App code (under `tutorials/sql/`, real apps) is fine with `print`.

## Reference

- Tutorials — every shipped feature has a runnable file under `tutorials/sql/` (45 files).
- Implementation — `daslib/sql_boost.das`, `daslib/sql_linq.das`, `daslib/sql_provider.das` (neutral core); `modules/dasSQLITE/daslib/sqlite_boost.das`, `sqlite_provider.das`, `sqlite_migrate.das` (SQLite provider).
- Related skills — `skills/json.md` (`@sql_json` columns), `skills/linq.md` (`_sql` is LINQ-shaped), `skills/das_macros.md` (`[sql_table]` / `_sql` are macros), `skills/gc_migration.md` (`SqlRunner` is one of the residual smart_ptr types).
