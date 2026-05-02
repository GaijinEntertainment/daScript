# dasSQLITE API Rework — Design Notes

Working document. We walk the tutorials one at a time, take notes on what the
current low-level C-binding style makes the user write, and what an idiomatic
daslang API *should* let them write. At the end we synthesize a plan.

**Mockups:** for each tutorial we agree on, a file in
`modules/dasSQLITE/tutorial-mockup/` shows the target API shape. Mockups are
design artifacts — they reference APIs that don't exist yet and do not
compile. They are the source of truth for "what we want this to look like";
the rework plan must be able to produce them.

## Language prerequisites (orthogonal to dasSQLITE)

dasSQLITE rework depends on two language-level additions, both useful far
beyond SQL:

1. **PR #2471 — `daslib/option` + `daslib/result`.** ✅ **Merged
   2026-04-24** (commit `56934f491`). `Option<T>` for scalar-or-empty
   reads, `Result<T, string>` for write-side fallibility carrying an
   errmsg. Replaces the wrong `T?` nullable-pointer shape for value-
   type optionals. Available in-tree — the rework can rely on these
   types being present.
2. **Named-tuple literal — `(Name=val, Price=val)`.** ✅ **Already in the
   language.** ds2_parser.ypp's `'(' make_struct_single ')'` production
   (around line 2203) lowers `(Name=a, Price=b)` to `ExprMakeTuple` with
   `recordNames` populated — the bare parenthesized form is what ships.
   Field-by-name access works (`t.Name`), `:=` for clone works
   (`(x:=1, y:=2)`), single-field and nested forms both parse. No bison
   conflicts. The verbose `(Name=a, Price=b)` form does **not** work
   and is not needed — the bare form is the sanctioned shape.
   Closes the "projection without declaring a type" gap and removes the
   need for SQL-specific ident-capture macros in dasSQLITE's projection
   story. Throughout this doc, examples written as `(Name=…, …)`
   refer to this bare `(Name=…, …)` form.

Both prereqs are now done. The rework is unblocked on design **and**
language impl; only the dasSQLITE-specific phases remain.

## Shipped — chunk 2: `_sql` macro foundation (branch `dassqlite-chunk2-sql-macro`)

- New module file `modules/dasSQLITE/daslib/sqlite_linq.das` housing `_sql(...)`
  and the companion `_sql_text(...)` (SQL-text-debug macro — name decision
  locked to `_sql_text`, returns `string`). Module registered in CMake via
  `ADD_MODULE_DAS(sqlite daslib sqlite_linq)` and in `.das_module` via a new
  `register_native_path` block that also backfills `sqlite_boost`.
- Runtime shim: `sqlite_boost` adds the read-direction dispatch rail
  `sqlite_read(stmt, col, type<T>) : T` (mirrors `sqlite_bind`), the
  `select_from(db, type<T>) : array<T>` runtime entry, and the
  `try_run_select` / `run_select` helpers the macro emits calls to. The
  `[sql_table]` structure macro now also emits `_sql_select_all_sql` (full-row
  SELECT string) and `_sql_read_row` (per-struct row materializer) for each
  user table.
- **Operator surface in chunk 2:** `select_from(type<T>)` (root) +
  `_to_array()` (no-op terminal) + `_select(_.Field)` (single-column
  projection) + `_where(predicate)` with bind capture (literals and free
  variables → `?` placeholders; supports `==`, `!=`, `<`, `<=`, `>`, `>=`,
  `&&`, `||`, `!`). Multiple `_where` calls in a chain compose AND-wise.

### Architecture: Mode 2 inner-first expansion

`_sql` (and `_sql_text`) run as plain `[call_macro]` decls with the
**default `canVisitArgument=true`**. That contract — confirmed during
chunk-2 development — means daslang's macro pass expands inner
call_macros bottom-up *before* the outer `visit()` fires. By the time
`_sql.visit` walks the chain, `_where(it, expr)` has already become
`where_(it, $(_:T) => expr)`, `_select(it, expr)` has become
`select(it, $(_:T) => expr)`, and any user-defined wrapper macro has
cascaded into the same canonical `where_` / `select` shape.

This makes the chain analyzer trivially small: it pattern-matches
`select_from(...)` at the root, `where_` / `select` calls in the body
(via `match_linq_call(node, "where_" | "select")` + `peel_lambda_body`),
and `_to_array(...)` as the only outermost terminal. No per-operator
macro plumbing — every chain stage is just a `linq` call by the time
the analyzer sees it.

The Mode 2 contract was nearly missed during development. An initial
Mode-1 attempt added `canVisitArgument → false` overrides on `_sql` /
`_sql_text` to guarantee raw-AST inputs, after a synthetic test seemed
to show inner expansion not firing. The diagnosis was wrong: the real
problem was that `sqlite_linq.das` didn't `require daslib/linq_boost
public`, so `_where` / `_select` were unavailable as call_macros at
user-file scope and produced unbound-`_` cascades. Adding the public
require made Mode 2 work; the override is gone.

**Composable user wrappers (proven by `test_07_sql_composability.das`):**
a user can write `[call_macro(name="when_price_lt")]` that expands to
`_where(it, _.Price < val)` and call `_sql(db |> select_from(type<Car>)
|> when_price_lt(200))` — the wrapper expansion cascades through `_where`
into `where_`, and the analyzer sees the same `where_` shape it would
for a user-written `_where(_.Price < 200)`. The 10-test composability
suite covers the baked-field form, the generic form
(`when_lt(it, _.Field, val)` passing the field-ref through as an
argument), wrapper-then-`_where` AND-composition, wrapper-then-`_select`
projection composition, and two-wrapper AND chains.

### Hardening / diagnostics

- **Entry guards on `_sql` and `_sql_text`** use `macro_verify` (loud
  failure with `{describe(arg)}`) rather than soft `return null`, so a
  null `_type` on the chain argument — which means an inner call_macro
  didn't expand because of a missing-require — surfaces immediately at
  the call site instead of cascading into unrelated infer errors.
- **`ExprRef2Value` peel** at the top of `analyze_projection`,
  `pred_to_sql`, and `is_const_or_captured_var`: the typer wraps field
  reads in `ExprRef2Value` after Mode-2 inner expansion, so the analyzer
  unwraps it before pattern-matching shapes like `_.Field` or comparing
  to `ExprVar`. (TODO in `daslib/ast_match.das`: add auto-peel inside
  `qmatch` so callers don't need to do this manually.)

### AOT status

`sqlite_linq.das` is registered in `tests/aot/CMakeLists.txt`'s
`AOT_DASSQLITE_MODULE_FILES` and AOT-codegens cleanly into
`test_aot_dassqlite_modules_sqlite_linq.das.cpp`. Runtime AOT for
dasSQLITE inherits whatever sqlite_boost AOT delivers — chunk-2 doesn't
gate or worsen anything in that path. `failed_sql_macro.das` is excluded
from the AOT glob via the same `list(FILTER ... EXCLUDE REGEX
"failed_")` pattern used by `tests/decs/`, since expect-error tests
intentionally fail to compile and would break AOT codegen.

### Deferred to chunk 3+

Struct-constructor and named-tuple `_select` projections; `_order_by`,
`_take`, `_skip`, `_distinct`; aggregates (`_count`, `_sum`, `_avg`,
`_min`, `_max`); `_first` / `_first_or_default`; `_group_by`, `_having`;
`_join`, `_left_join`; `_in`, `_not_in`, `_any`, `_none`; the
panic-free `_try_sql` variant. All planned, none in this PR.

## Shipped — chunk 3: aggregate framework + _try_sql + tuts 5/6/8/9 (branch `dassqlite-chunk3-sql-framework`)

Builds the discriminator infrastructure that aggregates (chunk 4's tut 13)
will plug into, and ships the `_sql`-side surface for tutorials 5/6/8/9.
The framework is proven end-to-end by a `_count` canary; chunk 4 broadens
to `_sum`/`_avg`/`_min`/`_max` plus tutorials 10–13.

### Framework rework — three discriminators in `analyze_chain`

- **`Materializer`** enum threaded through `SqlQuery`:
  - `Array` — default, returns `array<T>` via `run_select`
  - `One` — single row, panics on empty (covers `_first`, `count()`)
  - `OneOpt` — `Option<T>`, none on empty (covers `_first_opt`)
- **`ProjectionShape`** enum:
  - `FullRow` — default, `SELECT cols-of(rootT)`
  - `SingleColumn` — `_select(_.Field)` (chunk 2)
  - `NamedTuple` — `_select((Name=_.Name, Price=_.Price))` (new)
  - `Aggregate` — `SELECT COUNT(*)` (canary) — chunk 4 broadens
- **Function-call dispatch** in `pred_to_sql`: `(name, arity) -> SQL_template`.
  Chunk 3 entries: `starts_with`/`ends_with`/`contains` (LIKE patterns),
  `to_lower`/`to_upper` (case folding), `length`, `abs`. Chunk 4 adds
  date/time + the aggregate row (`sum`, `avg`, etc.).

### New chain operators

- **`_first()`** — terminal. Plain function in `sqlite_linq.das`; recognized
  in `analyze_chain`; emits ` LIMIT 1` and uses Materializer.One.
- **`_first_opt()`** — terminal. Plain function in `sqlite_linq.das`; emits
  ` LIMIT 1` and uses Materializer.OneOpt.
- **`count()`** (no underscore — `_count` is reserved by linq_boost as a
  predicate-form macro). Recognized in `analyze_chain`; sets
  ProjectionShape.Aggregate and Materializer.One. **Canary** for the
  framework — chunk 4 will land `_sum` / `_avg` / `_min` / `_max` in the
  same shape.

### `_try_sql(chain)` macro — non-panicking sibling of `_sql`

Shares the analyzer with `_sql` (the visit body is factored into
`emit_sql_macro_body(prog, call, isTry : bool)`). Difference: emits
`try_run_*` runtime helpers wrapping the result in `Result<T, string>`
(or `Result<Option<T>, string>` for OneOpt). Tut 6 demonstrates the full
matrix.

### Runtime additions in `sqlite_boost.das`

- **`run_select_one` + `try_run_select_one`** — single-row materializer
  (errors on 0 rows; one row -> T or Result<T,string>).
- **`run_select_one_opt` + `try_run_select_one_opt`** — single-row Option
  variant (0 rows = none).
- **`query_one` + `try_query_one` + `query_one_opt`** — full-row read
  with positional bind. 0/1/2/3-arg overloads; 4+ positional args is a
  follow-up. Pattern mirrors chunk-1's `try_query_scalar` — `try_*` does
  the work and returns `Result<T, string>`; strict panics on err; `_opt`
  returns `Option<T>`.
- **`query_scalar` overloads** for `int`, `int64`, `float`, `double`,
  `bool`. (Chunk 1 shipped `string` only.) Each with `try_*` and `_opt`
  siblings.

### Deferred to chunk 4

- **Named-tuple bind for `query_one(sql, type<T>, (id=…, name=…))`.**
  Needs a `[call_macro]` to walk the trailing tuple's `argNames` at
  compile time and emit unrolled `sqlite3_bind_parameter_index +
  sqlite_bind` calls. Pattern is the `qmacro_function`-per-instantiation
  used by chunk-1's `_sql_bind_row` for `[sql_table]` rows. Tut 5 ships
  positional-bind only this chunk; the named-tuple form is a follow-up.
- **Struct-type `_select(type<T2>)` projection.** Project a subset of
  source fields into a different `[sql_table]` struct. Today users can
  use named-tuple projection (`_select((A=_.A, B=_.B))`) to get the same
  result with a tuple instead of a struct.
- **4+ positional args for `query_one`.** Add overloads or a call_macro
  in chunk 4 if a real use case shows up (rare in practice).
- **Tutorial 7 ("Anatomy of `_sql`")** is folded into tuts 8/9 inline
  comments + Appendix C below — no standalone tutorial.

### Surface in this PR

- **Tutorials 5, 6, 8, 9** under `tutorials/sql/`.
- **22 new test files** under `tests/dasSQLITE/`:
  - test_08 / test_09 — `_first`, `_first_opt`
  - test_10 / test_11 — `query_one` positional (1/2/3-arg)
  - test_13 — `query_one_opt`
  - test_15..test_20 — `query_scalar` int/int64/double/bool + `_opt` + `try_*`
  - test_21 — `_try_sql` (Result/Option/Result<Option> matrix)
  - test_22 — `_select` named-tuple projection
  - test_24..test_29 — `_where` operators (starts_with, ends_with,
    contains, to_lower/to_upper, length, abs)
  - test_30 / test_31 — `_count` canary (basic + with `_where`)
- **151 dasSQLITE tests pass** interpreted (81 chunk-2 + 70 chunk-3).
- AOT path stays green (CMake glob auto-picks new tests; reconfigure
  required after adding the first new test of a chunk).

## Shipped — chunk 6: write side — UPDATE / DELETE / Transactions (branch `dassqlite-chunk6-update-delete-tx`)

Chunk 6 ships **tutorials 19 (UPDATE), 20 (DELETE), 22 (Transactions)** —
the write-side counterpart to chunks 2–5's read-side `_sql(...)` flagship.
Tut 21 (UPSERT) is deferred to chunk 7 because it carries its own
substantial new surface (the `_excluded` AST sentinel, multi-column
conflict targets, `@sql_unique` annotation, four "INSERT OR X" variants,
bulk overload).

### Naming decision (override of the original mockups)

Macro forms use the `_sql_` prefix:
`_sql_update`, `_sql_try_update`, `_sql_update_returning`,
`_sql_try_update_returning`, and the parallel `_sql_delete` set.
The mockups (15-update.das.mockup / 16-delete.das.mockup) had the bare
`_update` / `_delete` names — overridden during plan walkthrough so that
SQL provenance stays visible at the call site and there's no name
collision with hypothetical future non-SQL macros named `_update` /
`_delete`. Function siblings stay unprefixed (`update`, `delete_`,
`delete_by_id`, plus the `try_*` variants). RETURNING is macro-only
in this chunk — `_sql_update_returning` / `_sql_delete_returning`
(and the `_sql_try_*_returning` non-panic variants) — backed by the
generic `run_dml_returning` / `try_run_dml_returning` runtime
helpers; there are no plain `update_returning` / `delete_returning`
function wrappers.

### New `[sql_table]` generated helpers

- `_sql_update_by_pk_sql(typ : T) : string` —
  `UPDATE "T" SET "c1"=?, "c2"=?, ... WHERE "pk"=?`. PK-only or PK-less
  structs emit a stub that panics at call time with a clear message
  (concept_assert fires unconditionally during structure-macro apply,
  so a runtime panic is the workable shape).
- `_sql_delete_by_pk_sql(typ : T) : string` —
  `DELETE FROM "T" WHERE "pk"=?`.
- `_sql_bind_row_for_update(stmt, row)` — binds non-PK columns first
  (positional 1..N), then the PK column at index N+1 — matches the SET-
  then-WHERE order in the generated SQL.
- `_sql_bind_row_pk_only(stmt, row)` — binds just the PK column at
  index 1, used by `delete_(row)` to execute the by-PK DELETE.

### Runtime additions in `sqlite_boost.das`

- **`changes(db) : int`** — wraps `sqlite3_changes`. Used by raw `exec`
  callers and by tests for "what did the last DML touch".
- **By-PK mutators:** `update(row)` / `try_update(row)`,
  `delete_(row)` / `try_delete_(row)`,
  `delete_by_id(type<T>, id)` / `try_delete_by_id(...)`.
  All return `int` rows-affected (1 or 0); `try_*` returns
  `Result<int, string>`.
- **Generic DML runners:** `try_run_dml` / `run_dml` (returns int),
  `try_run_dml_returning` / `run_dml_returning` (returns `array<T>`).
  Used by the `_sql_update` / `_sql_delete` macros — they take a SQL
  string + bind block + (for RETURNING) a row-builder block.
- **Transaction suite:**
  - `enum SqliteTxnMode { Deferred, Immediate, Exclusive }`.
  - `with_transaction(db, blk)` and
    `with_transaction(db, mode, blk)` — two distinct overloads (per
    tut 22 plan; trailing-block convention forbids an optional middle
    parameter).
  - `try_transaction(db, blk)` and `try_transaction(db, mode, blk)` —
    return `SqlError` (`Option<string>`) rather than `Result<void, E>`
    because daslang's `Result<T, E>` does not yet support `void`
    payloads. `none = success`; `some(errmsg) = SQL failure at BEGIN
    or COMMIT`. Block panics still ROLLBACK and re-propagate.
  - `in_transaction(db) : bool` — wraps the inverse of
    `sqlite3_get_autocommit`.
  - **Savepoint nesting:** when entered while already in a
    transaction, emits `SAVEPOINT das_sp` / `RELEASE das_sp` /
    `ROLLBACK TO das_sp; RELEASE das_sp` instead of BEGIN/COMMIT/
    ROLLBACK. SQLite resolves RELEASE / ROLLBACK TO against the
    most-recent matching savepoint name, so proper LIFO nesting
    composes correctly without the runner tracking depth itself.
- **Bulk insert composability fix:** `try_insert(rows : array<T>)`
  now branches on `in_transaction()` — uses `BEGIN IMMEDIATE` /
  `COMMIT` when standalone, `SAVEPOINT das_ins_sp` / `RELEASE` when
  nested inside an outer `with_transaction`. Without this fix SQLite
  errored "cannot start a transaction within a transaction" when
  bulk-insert was nested.

### New macros in `sqlite_linq.das`

Eight new `[call_macro]`s — four for UPDATE and four for DELETE.
Each pair is `_sql_{verb}` / `_sql_try_{verb}` (with optional
`_returning` suffix for the four RETURNING variants). All eight set
`canVisitArgument = false` for everything except `db`, so the WHERE /
SET expressions arrive as raw AST. Analysis runs on the AST shape
directly: `_.Col` references → SQL column refs, captured locals /
literals → `?` placeholders + bind expressions. Bind ordering is SET
binds first, then WHERE binds (matches placeholder order in
`UPDATE ... SET ... WHERE ...`).

The named-tuple SET clause `(Col1=val1, Col2=val2)` carries its
column names in `ExprMakeTuple.recordNames`, populated by the parser
(no type inference required). Chunk 6 also exposes `recordNames` to
daslang reflection via a one-line C++ change in
`module_builtin_ast_annotations_2.cpp` so the macro can read it
directly without forcing premature type inference on the SET arg.

### Tutorials

- [tutorials/sql/19-update.das](../../tutorials/sql/19-update.das) — by-PK,
  bulk macro, RETURNING, raw escape hatch, try_ variants.
- [tutorials/sql/20-delete.das](../../tutorials/sql/20-delete.das) — by-PK
  via `delete_(row)` / `delete_by_id`, bulk macro, RETURNING, raw escape
  hatch, try_ variants.
- [tutorials/sql/22-transactions.das](../../tutorials/sql/22-transactions.das)
  — 2-arg / 3-arg `with_transaction`, savepoint nesting,
  `in_transaction()`, `try_transaction`.

### Tests

9 new test files: `test_43_update_by_pk.das`, `test_44_update_bulk.das`,
`test_45_update_returning.das`, `test_46_delete_by_pk.das`,
`test_47_delete_bulk.das`, `test_48_delete_returning.das`,
`test_49_changes.das`, `test_50_with_transaction.das`,
`test_51_try_transaction.das`. Plus `failed_sql_update_delete.das` for
malformed-call diagnostics. Total dasSQLITE suite: **271 tests passing**
(238 pre-chunk-6 + 33 new).

### Deferred to chunk 7+

- **`exec` parameter binding** — `db |> exec(sql, args...)` doesn't
  exist; users must inline values into the SQL string for raw escape
  hatch with dynamic data, or stick to the macro forms. Adding
  parameterized `exec` is a small follow-up that mirrors `query_one`'s
  variadic shape.
- **Optimistic concurrency** (`@sql_concurrency_token`) — open question
  in API_MISSING § 15; defer until a real ask.
- **Multi-table DELETE** (PG `DELETE … USING` / MySQL JOIN-DELETE) —
  dialect-divergent, deliberately skipped per API_MISSING § 16.
- **Projection-side `_select` after `_sql_update_returning`** — for
  RETURNING column projection, post-process the result with a chain
  `|> _select(...)` instead of inventing a `update_returning_select`.

## Shipped — chunk 7: UPSERT + schema annotations (branch `dassqlite-chunk7-upsert-schema`)

Chunk 7 ships **tutorials 21 (UPSERT), 23 (foreign keys), 24 (indexes),
25 (defaults + computed columns)**. UPSERT was deferred from chunk 6
because it carries its own substantial macro surface; the three schema
tutorials were already at the front of "Part 5 — schema richness" in the
plan and naturally co-shipped because the upsert composite-conflict path
needs `[sql_index(unique = true, ...)]` from tut 24.

### UPSERT macros (`sqlite_linq.das`)

- `_sql_upsert(row, on_conflict, do_update)` — INSERT … ON CONFLICT …
  DO UPDATE SET …, returns rows-affected `int`.
- `_sql_try_upsert(...)` — same, returns `Result<int, string>`.
- `_sql_upsert_returning(...)` / `_sql_try_upsert_returning(...)` —
  capture the post-merge row(s) as `array<T>` (always one row in
  practice; the array shape mirrors `_sql_update_returning`).
- `on_conflict` accepts `_.Col` (single) or `tuple(_.A, _.B, ...)`
  (composite); validated against the struct's fields at macro-expansion.
- `do_update` is a named-tuple `(Col = expr, ...)`. `_` is the existing
  row; `_excluded` is the proposed-row sentinel (added to `pred_to_sql`
  via the new `SqlQuery.upsertMode` flag). Computed columns and unknown
  column names are rejected with macro_error; duplicate column names too.
- Arithmetic operators (`+ - * / %`) added to `pred_to_sql` so the do-
  update SET clause can express `Hits = _.Hits + 1`.

### Plain "INSERT OR X" functions (`sqlite_boost.das`)

- `insert_or_ignore(row)` / `insert_or_ignore(rows)` — single + bulk;
  `try_` siblings return `Result<int, string>`.
- `insert_or_replace(row)` / `insert_or_replace(rows)` — same fan-out.
- All implemented via runtime substitution of `OR IGNORE` / `OR
  REPLACE` against the existing `_sql_insert_with_pk_sql` /
  `_sql_insert_no_pk_sql` strings — no new generated helpers needed.

### Schema annotations on `[sql_table]` (`sqlite_boost.das`)

Per-field decorators (every `@xxx = value` is a separate scalar line):

- **`@sql_unique`** — single-column UNIQUE in the column DDL.
- **`@sql_default_fn = "FN"`** — whitelisted SQL built-ins
  (`CURRENT_TIMESTAMP`, `CURRENT_DATE`, `CURRENT_TIME`); emits
  ` DEFAULT FN`.
- **Native field initializer for literal defaults** — `Active : bool =
  true` becomes ` DEFAULT 1`. Supported literals: bool / int / int64 /
  float / double / string. Non-literal initializers are silently dropped
  from the DDL but still construct in daslang.
- **`@sql_computed = "expression"`** — generated column. Default storage
  VIRTUAL; add **`@sql_stored = true`** for STORED. Computed columns are
  excluded from INSERT and UPDATE bind paths automatically; SELECT reads
  them as ordinary columns.
- **`@sql_references = "Parent"`** + optional **`@sql_on_delete`** /
  **`@sql_on_update`** — one of `cascade` / `set_null` / `set_default` /
  `restrict` / `no_action`. Resolves the parent struct's table name and
  PK column at macro-expansion. `with_sqlite` enables `PRAGMA
  foreign_keys = ON` so the constraints actually fire.

Validation rules enforced at `[sql_table]` apply (each emits
`error[30111]`):

- `@sql_primary_key` ≠ `@sql_computed` (SQLite rejects PK on a generated
  column).
- `@sql_computed` ≠ field initializer (the SQL expression provides the
  value).
- `@sql_computed` ≠ `@sql_default_fn`.
- field initializer ≠ `@sql_default_fn` (pick one).
- `@sql_default_fn` value must be in the whitelist.
- `@sql_on_delete` / `@sql_on_update` value must be in the whitelist.
- `@sql_on_delete` / `@sql_on_update` without `@sql_references`.
- `on_delete = "set_null"` requires `Option<T>` column.
- `on_delete = "set_default"` requires a field initializer or
  `@sql_default_fn`.
- `@sql_references` referencing a struct that doesn't exist or has no
  `@sql_primary_key`.

### `[sql_index]` sibling annotation

- Shape: `[sql_table(name = "..."), sql_index(fields = ..., unique =
  ..., name = ...)]` (must live in the same bracket as `[sql_table]`,
  comma-separated, with `[sql_table]` first).
- `fields` accepts a single string or a tuple of strings. `unique`
  defaults to `false`. `name` defaults to `idx_<table>_<col1>_<col2>`.
- `[sql_index]` rewrites the `_sql_create_indexes_sql` helper that
  `[sql_table]` emits (per Boris's body-rewrite suggestion) — each
  `[sql_index].apply()` parses the const-string return of the existing
  helper, appends a new `CREATE INDEX` statement, and replaces the
  body. This forces source order: `[sql_table]` must precede every
  `[sql_index]` because the body-rewrite needs the helper to already
  exist.
- Unknown field names and missing `fields=` are rejected with
  `error[30111]`.

### Tutorials

- [tutorials/sql/21-upsert.das](../../tutorials/sql/21-upsert.das) —
  `insert_or_ignore` / `_or_replace`, single + composite `_sql_upsert`,
  `_sql_upsert_returning`, `_sql_try_upsert`.
- [tutorials/sql/23-foreign_keys.das](../../tutorials/sql/23-foreign_keys.das)
  — CASCADE delete, SET NULL with `Option<T>`, FK violation through
  `try_insert`.
- [tutorials/sql/24-indexes.das](../../tutorials/sql/24-indexes.das) —
  unique + composite + named indexes, query-side transparency, UNIQUE
  violation through `try_insert`.
- [tutorials/sql/25-defaults_computed.das](../../tutorials/sql/25-defaults_computed.das)
  — native init defaults, `@sql_default_fn`, VIRTUAL + STORED computed
  columns.

### Tests

9 new test files: `test_52_insert_or_ignore.das`,
`test_53_insert_or_replace.das`, `test_54_upsert_single_col.das`,
`test_55_upsert_composite.das`, `test_56_upsert_returning.das`,
`test_57_sql_unique.das`, `test_58_sql_references.das`,
`test_59_sql_index.das`, `test_60_defaults_computed.das`. Plus three
new failed-test files: `failed_sql_table_schema.das` (12 [sql_table]
validation rules), `failed_sql_index.das` (4 [sql_index] validation
rules), `failed_sql_upsert.das` (10 _sql_upsert macro_errors). Total
dasSQLITE suite: **309 tests passing** (271 pre-chunk-7 + 38 new).

### Deferred to chunk 8+

- **Bulk `array<T>` upsert** — `_sql_upsert` rejects `array<T>` as the
  row argument with macro_error; pass single rows in a transaction loop
  for now. Adding the bulk overload is straightforward but each row
  needs its own bind cycle plus the same SQL prepared statement reused.
- **Composite foreign keys** — `@sql_references` resolves a single PK
  column. Composite PK targets would need a `fields = ("A", "B")`
  syntax extension. Rare in greenfield SQLite designs.
- **Partial / expression indexes** — SQLite supports `CREATE INDEX …
  WHERE …` and `CREATE INDEX … ON T(lower(Email))`. Both are SQLite-
  specific; users needing them run raw `db |> exec("CREATE INDEX …")`.
- **Function-reference form for `@sql_computed`** — current shape passes
  the SQL expression as a string; SQLite validates at CREATE TABLE
  time. The principled long-term shape uses `@sql_computed(func =
  @@compute_total)` so a regular daslang function with daslang
  compile-time validation provides the expression. Punted; SQLite's
  runtime error message is actionable enough as MVP.
- **DEFAULT-firing from the macro INSERT path** — the macro INSERT
  always names every non-computed column, so the SQL DEFAULT clause
  only fires from raw `exec`. Detecting "user wants the default" from
  a struct field value is fragile (zero-detection) or verbose (explicit
  field list). Raw SQL is the escape hatch.
- **Pre-existing duplicate cleanup pass** — `detect_duplicates` over
  `sqlite_boost.das` + `sqlite_linq.das` flagged five real merge
  candidates carried over from chunks 3 / 6, none of which are chunk-7
  surface but all of which would land cleanly together:
    1. `validate_outer_update_args` (`sqlite_linq.das:3353`) ≡
       `validate_outer_delete_args` (`:3367`) — exact dupe; differs
       only in `length(call.arguments) != 4` vs `!= 3` and the arg-
       name list in the error message. Collapse to
       `validate_outer_args(prog, call, rootT, expectedArity, sigText)`.
    2. `make_insert_with_pk_sql_fn` (`sqlite_boost.das:1951`) vs
       `make_insert_no_pk_sql_fn` (`:1995`) — 0.96 fuzzy. Both emit
       `INSERT INTO "T" (...) VALUES (?,?,...)`; the only structural
       difference is whether the PK column is in the column list.
       Single helper taking an `include_pk : bool`.
    3. `find_bool_annotation` (`sqlite_boost.das:1353`) ≡
       `find_string_annotation` (`:1361`) — identical loops differing
       only in the typed accessor literal (`bValue` vs `sValue`).
       Generic `find_annotation<T>(args, name, default)`.
    4. `try_run_dml_returning` (`sqlite_boost.das:1213`) ≡
       `try_run_select` (`:1270`) — identical bodies. One should
       call the other (or share a `run_step_collect` helper).
    5. **12-way `canVisitArgument` cluster** across
       `Sql{Update,TryUpdate,UpdateReturning,TryUpdateReturning,
       Delete,TryDelete,DeleteReturning,TryDeleteReturning,
       Upsert,TryUpsert,UpsertReturning,TryUpsertReturning}Macro` —
       every override returns `argIndex == 0`. Class-hierarchy
       fan-out can't share without a parent class. Introduce
       `class abstract OuterArgZeroMacro : AstCallMacro` (or
       similar) and re-parent all 12.
  Cleanup is independent of any new feature work and can be its own
  small chunk; targeting it together with chunk 7's `bulk array<T>
  upsert` (`Sql{Upsert,…}Macro` would change anyway) is one
  reasonable bundle.

## Shipped — chunk 8: custom-types adapter rail + BLOB + backlog drain (branch `dassqlite-chunk8-custom-types`)

Chunk 8 ships **tutorial 26 (custom type adapters)** and **tutorial 27
(BLOB round-trip)**, plus two small deferred features and the dupe
cleanup carried over from chunk 7. Tut 28 (JSON) was cut from this
chunk — its `_sql` JSON-path walker rule deserves its own attention.

### Custom-types adapter rail (`sqlite_boost.das`)

- **Two-function name-based pair** — `_::sql_bind(value : T) : P` and
  `_::sql_extract(stored : P; type<T>) : T`, where `P` is one of four
  primitive storage types (`int64`, `double`, `string`, `array<uint8>`)
  selected at macro-expansion from `_::sql_bind`'s return type.
- **Module-scope overloads** for the four primitives (passthrough),
  stdlib widenings (`int`/`int8`/`int16`/`uint`/`uint8`/`uint16`/
  `uint64`/`float`/`bool` → primitives), and a single enum generic
  guarded by `static_if (typeinfo is_enum(default<TT>))`. Users add
  their own overloads at any module scope to register `DateTime`,
  `Guid`, etc.
- **Macro emission migrated** — the `[sql_table]` codegen routes
  through the new `sql_bind_to_stmt(stmt, idx, v)` and
  `read_via_adapter(stmt, col, type<T>)` helpers, plus a
  `sql_storage_type_for(type<T>)` DDL emitter that derives the
  column type from `_::sql_bind`'s return type via `typedecl`. Runtime
  paths (`query_one` / `query_scalar` / raw `_sql` positional binds)
  also route through these so user-defined adapters work uniformly.
- **`Option<T>` over a custom adapter** — the bind/read sites unwrap
  `Option<T>` to `T` and recurse, so `Option<DateTime>` Just Works as
  long as `_::sql_bind`/`_::sql_extract` are defined for `DateTime`.
- **Legacy rail removed** — `sqlite_bind` / `sqlite_read` /
  `sqlite_sql_type` overload tables are gone.

### Tut 27 — BLOB round-trip

`array<uint8>` is one of the four adapter primitives, so a struct
field of that type round-trips through a BLOB column with no extra
annotation. No new core macro work was needed.

### Backlog drain — small features

- **`exec` / `try_exec` parameter binding** — 0/1/2/3-arg overloads
  mirror the `query_one` shape (chunk 6 deferred). Routes through a
  new private `try_exec_with_binds` helper.
- **`_.Col == none()` → fixit `macro_error`** — `pred_to_sql`'s
  equality handler detects `none()` on either side (both pre-typer
  qmatch and post-typer `operator==(...)` shapes) and emits a
  `macro_error` pointing the user at `is_none()` (chunk 4 D4 deferred).

### Side bug fixes

- **`sqlite3_bind_blob` wrapper had a hardcoded `index = 1`** —
  pre-existing; never noticed because nothing tested BLOB from
  `[sql_table]` before. Now uses the caller's `index` parameter.
- **Empty `array<uint8>` bind** — guarded with
  `length == 0 → sqlite3_bind_zeroblob(stmt, index, 0)` to avoid
  `addr(data[0])` panic on zero-length arrays.
- **`is_const_or_captured_var` missed enum / bitfield literals** —
  `_where(_.Status == OrderStatus.Paid)` was emitting incomplete SQL
  (`WHERE "Status" = ` with no bind). Added `ExprConstEnumeration`,
  `ExprConstBitfield`, and the missing int8/16 / uint8/16 forms to
  the recognizer.

### Pre-existing dupe cleanup (5 items, all from chunk-7 deferred list)

1. **`validate_outer_args(prog, call, rootT, expected_arity, sig_text)`**
   — collapses `validate_outer_update_args` /
   `validate_outer_delete_args`. All 8 callers updated.
2. **`make_insert_sql_fn(st, name, fields, include_pk : bool)`** —
   collapses `make_insert_with_pk_sql_fn` / `make_insert_no_pk_sql_fn`.
   Emitted helper names stay stable
   (`_sql_insert_with_pk_sql` / `_sql_insert_no_pk_sql`).
3. **`find_annotation(args, argn, default_value : auto(TT)) : TT`** —
   collapses `find_bool_annotation` / `find_string_annotation` via
   `static_if (typeinfo is_string(default_value))` dispatch on
   `tString` / `tBool`. ~20 call sites updated.
4. **`try_run_dml_returning` ≡ `try_run_select`** — flagged by the
   dupe agent but already factored: both are 1-line wrappers around
   `try_collect_rows` with different `err_prefix`. The existing
   factoring IS the dedupe; no code change needed.
5. **`OuterArgZeroMacro` / `OuterArgTwoMacro` parents** —
   `class private OuterArgZeroMacro : AstCallMacro` (overrides
   `canVisitArgument` → `argIndex == 0`) and
   `class private OuterArgTwoMacro : AstCallMacro`
   (`argIndex < 2`). The 8 update/delete macros and 4 upsert macros
   re-parent to these and drop their local `canVisitArgument`
   overrides; each subclass keeps its own `[call_macro(name=...)]`
   annotation since annotations don't inherit. Net: 12 × 3-line
   override deletions, 2 × 4-line parent-class additions.

### Tutorials

- [tutorials/sql/26-custom_types.das](../../tutorials/sql/26-custom_types.das)
  — `DateTime` via INTEGER, `Guid` via BLOB, enum auto-roundtrip,
  `query_scalar` through adapters, `Option<DateTime>`, enum-in-where
  predicate, DDL storage-type derivation.
- [tutorials/sql/27-blob.das](../../tutorials/sql/27-blob.das) —
  `array<uint8>` round-trip; derived from inherited
  `tutorial/07-insert_image.das` + `08-read_image.das`.

### Tests

3 new test files: `test_61_custom_types.das` (7 tests),
`test_62_blob.das` (6 tests), `test_63_exec_params.das` (6 tests).
Plus 2 new entries in `failed_sql_macro.das` (`bad20` / `bad21` —
the `_.Col == none()` fixit). Total dasSQLITE suite: **328 tests
passing** (309 pre-chunk-8 + 19 new).

### Deferred to chunk 9+

- **Tut 28 — JSON columns** (`@sql_json` / `@sql_blob` annotations
  + `_sql` JSON-path walker rule) — was cut from this chunk; ships
  as its own headline.
- **Per-field `@sql_as(type<P>)` override** — pick a non-default
  storage primitive for a custom-type column when multiple
  `_::sql_bind` overloads exist. Locked shape; awaiting user demand.
- **Struct-type `_select(type<T2>)` projection** — needs a
  `[sql_table]`-emitted compile-time field-list metadata helper.
- **Bulk `array<T>` upsert** — carried from chunk 7 deferred.
- **Composite foreign keys**, **partial / expression indexes**,
  **function-reference `@sql_computed`**, **DEFAULT-firing from the
  macro INSERT path** — all carried from chunk 7 deferred.
- **Optimistic concurrency token, multi-table DELETE, named-tuple
  bind for `query_one` / `exec`** — carried from earlier chunks.

## Shipped — chunk 10: operational SQLite (branch `dassqlite-chunk10-operational-sqlite`)

Chunk 10 ships **tut 31 (views)**, **tut 33 (PRAGMA tuning)**, **tut 34
(backup + VACUUM)**, **tut 35 (streaming `_each_sql`)**, and **tut 39
(user-defined SQL scalar functions)** — five tutorials covering the
"operational" surface of SQLite that wasn't yet in the daslang rail.
Migrations (originally listed as tut 30) deferred to the last chunk. No
new language prerequisites; chunk 10 sits entirely on the chunk-2..9
foundation.

### Tut 31 — `[sql_view]` + `_create_view`

- **`[sql_view(name="...")]` structure annotation.** Read-only sibling
  of `[sql_table]`. Generates the same `_sql_table_name` /
  `_sql_select_all_sql` / `_sql_read_row` / `_sql_column_info` /
  `_sql_drop_view_if_exists_sql` helpers under the same names so
  `select_from(type<V>)` and the `_sql(...)` rail dispatch on view
  types without case-splitting.
- **Field annotations:** `@sql_column` (rename), `@sql_json`,
  `@sql_blob` accepted; six DDL-flavored annotations rejected
  (`@sql_primary_key`, `@sql_unique`, `@sql_computed`/`@sql_stored`,
  `@sql_default_fn`, `@sql_references`/`@sql_on_delete`/`@sql_on_update`).
  Sibling `[sql_index]` rejected too.
- **JSON/BLOB adapter codegen factored** out of `[sql_table]`'s body
  into a shared `emit_json_blob_adapters_for_struct(st, fields, tag,
  errors)` helper — `[sql_view]` reuses the exact same adapter
  machinery for transparent JSON/BLOB-typed view columns.
- **Mutation-path rejection:** predicate-form (`_sql_update` /
  `_sql_delete` / `_sql_upsert` and try / returning siblings) caught at
  compile time inside `validate_outer_args`'s view check; row-form
  (`insert(viewRow)` / `update(viewRow)` / `delete_(viewRow)` /
  `upsert(viewRow)`) caught at compile time via macro-emitted
  `concept_assert(false, ...)` stubs that fire on first reference.
- **`_create_view(db, type<V>, chain)` call macro.** Validates: `type<V>`
  carries `[sql_view]`; chain projection's column count matches V's
  field count; per-position type matches V's field type (compared via
  `describe()`). Emits `db |> exec(...)` with the view's column list
  using V's field names (and `@sql_column` renames) authoritative.
- **Runtime-stringifier inlining** for `_create_view`: each bind expression
  goes through `_::to_sql_literal(<expr>)` at view-creation time, with the
  resulting literal concatenated into the DDL stored in `sqlite_schema`.
  Default overload set covers all numeric primitives, `bool`, and `string`
  (the `string` arm uses `sql_quote_lit`); float/double use round-trip-safe
  `%.9g` / `%.17g`. Captured locals, struct fields, and function calls all
  work; the value is frozen at view-creation time. User types extend with
  a one-line overload (`def to_sql_literal(s : Status) : string =>
  "{int(s)}"`); resolution at the call site picks them up via `_::`.
  Types without an overload hit the `to_sql_literal(auto(TT))` catch-all
  in `sqlite_linq.das`, which emits enums as `int(v)` and otherwise
  `concept_assert`s with a "define a one-line overload in YourType's
  module" pointer (40103).
- **`drop_view_if_exists(type<V>)` / `try_drop_view_if_exists`** template
  wrappers.
- Tutorial: [tutorials/sql/31-views.das](../../tutorials/sql/31-views.das).
  Tests: `test_72_view_basic.das` (5 positive),
  `failed_sql_view_schema.das` (9 errors),
  `failed_sql_view_mutations.das` (8 errors),
  `failed_create_view.das` (6 errors).

### Tut 33 — Pragma tuning

- **`set_pragma` / `try_set_pragma`** with three typed overloads
  (string / int64 / bool with ON/OFF). Inlined into `PRAGMA "..." = ...`
  since SQLite parses pragma values at prepare time (no `?` binds).
- **`apply_recommended_pragmas` / `try_apply_recommended_pragmas`**:
  WAL + busy_timeout=5000 + foreign_keys=ON + synchronous=NORMAL.
  Errors short-circuit through `Result`-style early return so the
  caller learns which pragma failed.
- Tutorial: [tutorials/sql/33-pragma.das](../../tutorials/sql/33-pragma.das).
  Tests: `test_80_pragma_basic.das` (6 cases incl. PRAGMA-in-tx
  rejection).

### Tut 34 — Vacuum / optimize / integrity / backup

- **`vacuum` / `try_vacuum`** — rejects active transactions before
  reaching SQLite (the strict form would surface SQLite's error
  message; the wrapper-side preflight is friendlier).
- **`vacuum_into(path)` / `try_vacuum_into`** — `''`-doubling
  single-quote escape on `path` (SQLite rejects `?` binds for VACUUM).
- **`optimize` / `try_optimize`** — thin `PRAGMA optimize` wrapper.
- **`integrity_check` / `quick_check` / `try_*`** — `array<string>`
  return; healthy DB returns `["ok"]`, corruption returns one row per
  detected issue. Backed by a private `try_check_impl` using
  `try_run_select` with `read_via_adapter` (NOT `try_query`, which
  expects a struct + `_sql_read_row`).
- **`backup_to(dest)` / `backup_to(path)` + `try_*` siblings** — online
  Backup API (`sqlite3_backup_init` / `_step` / `_finish`). C++ shim
  `sqlite3_backup_run` lives in `dasSQLITE.backup.cpp`; loops with
  100-page steps and falls back to a 50 ms `sqlite3_sleep` on
  transient SQLITE_BUSY/LOCKED responses. Path overload opens the
  destination internally and finalizes via `var inscope`. The C++ shim
  returns `SQLITE_OK` on a fully-completed backup and any other rc on
  failure; the daslang side maps that to `SqlError`.
- Tutorial: [tutorials/sql/34-backup_vacuum.das](../../tutorials/sql/34-backup_vacuum.das).
  Tests: `test_85_vacuum.das` (5 cases) +
  `test_83_backup_to.das` (3 cases — memory-runner snapshot, on-disk
  round-trip, destination overwrite).

### Tut 35 — Streaming (`_each_sql`)

- **`_each_sql(chain)` call macro** returning `iterator<T>` via
  `run_each_select` runtime helper. Same chain shape as `_sql`;
  rejects materializing terminals (`_to_array`, `_first`, `_first_opt`,
  aggregates) at compile time, pointing at `_sql` for the
  materializing form.
- **Generator pattern that worked** (after debugging "can't yield from
  inside the block" + "implicit capture by move"): explicit
  `capture(<- name1, <- name2)` with `<-` on every captured name;
  `var row_lam : lambda<...>` in the runtime helper signature;
  `for (_ in range(0x7FFFFFFF))` with `break` on `SQLITE_DONE`
  (daslang generators reject `while`); yield as the LAST statement in
  the for body.
- **Cleanup via `finally`:** the generator's `finally { sqlite3_finalize(stmt) }`
  runs on normal exhaustion, early `break`, outer return, and panic —
  no leaked stmt blocks subsequent writes on the same connection.
  Captured-local binds work (CREATE-VIEW-style inlining is NOT needed
  since `?` placeholders are honored at run time).
- **API_REWORK §35 revision:** §35 originally said streaming was a
  "behavior of existing iterator-based `for (row in _sql(...))`" — but
  that iterator form was never shipped. Streaming is therefore real
  new API: a new `_each_sql` macro returning `iterator<T>`.
- Tutorial: [tutorials/sql/35-streaming.das](../../tutorials/sql/35-streaming.das).
  Tests: `test_90_each_sql_basic.das` (5 positive incl. break +
  stmt-cleanup proof), `failed_each_sql_terminals.das` (4 errors).

### Tut 39 — `register_function`

- **`register_function(db, name, @@fn[, deterministic[, directonly]])`
  call macro.** Inspects the function pointer's static type at compile
  time (`_type.argTypes` + `_type.firstType`), derives an `SqlFnTag`
  per arg + return, and emits a call to a C++ trampoline-registration
  shim with per-position tags. Up to 4 arguments supported in v1.
- **Supported scalar set** (compile error otherwise, with per-position
  diagnostic naming the offender): `int`, `int64`, `float`, `double`,
  `bool`, `string`. Pointer types, structs, arrays, lambdas, classes
  rejected.
- **C++ trampoline** in `dasSQLITE.userfn.cpp`. `RegisteredScalarFn`
  user-data struct stores `(Context*, Func, retTag, nArgs, argTags[4])`;
  freed by SQLite-managed `xDestroy` callback (registered via
  `sqlite3_create_function_v2`'s last param) on connection close /
  function replacement / explicit drop.
- **NULL handling (v1):** any SQLITE_NULL argument short-circuits to
  `sqlite3_result_null` without invoking the daslang function — the
  idiomatic SQL behavior of built-in scalars (`abs(NULL)` → NULL).
  Explicit `Option<T>` arg types for in-function NULL handling are
  deferred to a follow-up.
- **Panic recovery:** `Context::runWithCatch` wraps the daslang
  invocation. On panic, the message is read via `getException()`, the
  context's `exception` / `stopFlags` are cleared (matching the
  recovery shape from `simulate_exceptions.cpp:217`), and the panic is
  surfaced via `sqlite3_result_error`. The connection itself is
  unaffected — subsequent statements on the same connection run
  normally.
- **`deterministic=true`** maps to `SQLITE_DETERMINISTIC` (allows the
  fn in `CREATE INDEX … ON tbl(myfn(col))` and lets the planner
  factor it out of inner loops). **`directonly=true`** maps to
  `SQLITE_DIRECTONLY` (blocks invocation from triggers / views /
  CHECK constraints).
- **Tag transport:** four scalar `uint8_t` slots (`tag0..tag3`) on the
  C++ binding instead of a `TArray<uint8_t>`, sidestepping the
  array-construction overhead in the macro emit. Unused slots
  ignored according to nArgs.
- Tutorial: [tutorials/sql/32-sql_functions.das](../../tutorials/sql/32-sql_functions.das).
  Tests: `test_74_register_function_basic.das` (10 — every supported
  type and arity), `test_75_register_function_null_panic.das`
  (3 — NULL short-circuit, panic recovery, post-panic connection
  health), `test_76_register_function_use_in_chains.das`
  (3 — UDF in WHERE / ORDER BY / projection),
  `test_77_register_function_lifetime.das` (3 — replacement,
  per-connection scope, no-survive-reopen),
  `failed_register_function.das` (5 errors — struct arg, struct
  return, pointer arg, lambda instead of `@@fn`, > 4 args).

### Tut 39 follow-up — `[sql_function]` auto-registration + chain visibility

`register_function` is per-connection: open a second DB and you have to
remember to repeat the call. The `[sql_function]` annotation removes
that ceremony AND makes the function visible inside `_sql(...)` chain
analysis (so it can be used as a SQL predicate / projection just like
a built-in scalar, instead of falling through to a per-row daslang
bind).

- **`[sql_function]` function annotation** in `sqlite_boost.das`. Same
  validation rails as `register_function` (≤4 args, scalar types only,
  `sql_fn_tag_for_type`); the auto-registration path uses the soft
  variant `_try_register_function_check_rc` which returns `SqlError`
  rather than panicking, so `try_open_sqlite` can surface a failed
  registration as `err(...)` instead of an unwound stack. Optional args:
  `name`, `deterministic`, `directonly`.
- **Compile-time-built lambda registry.** The macro emits a thunk
  (`@(db : SqlRunner) : SqlError { return _try_register_function_check_rc(
  db, "name", @@fn, ...) }`) and pipes it into the private
  `sql_function_registry` array (of `SqlFunctionThunk` entries carrying
  SQL `name` + `nArgs` alongside the install closure) via the public
  `_add_sql_function_thunk(name, nArgs, thunk)` adder, all inside an
  `[init]` block built by `setup_call_list("register`sqlite`functions",
  at, true, true)` — same mechanism `[live_command]` and `[decs]` use.
  The registry itself stays module-private; user-module init blocks reach
  it only through the adder. The install loop deduplicates `(name, arity)`
  across modules — silent SQLite override is a footgun, surface as err.
- **Auto-installation hook in `try_open_sqlite`.** After
  `sqlite3_open` returns OK, `_install_sql_function_registry(db)`
  walks the registry and registers every thunk against the new
  connection. Single hook covers `try_open_sqlite`, `open_sqlite`,
  and `with_sqlite`. ATTACH DATABASE shares the same `sqlite3*`
  handle so functions stay visible — no second hook needed.
- **Chain visibility in `fn_call_to_sql` + `analyze_projection`.** New
  branch at the end of the predicate dispatch table checks
  `find_annotation(call.func, "sql_function")`; if tagged, emits
  `<name>(args...)` SQL. The SQL name is taken from the annotation's
  `name` arg (defaults to the function's daslang name). The same
  recognizer (`try_recognize_sql_function_proj`) also fires from the
  single-column and named-tuple branches of `analyze_projection`, so
  `_select(my_upper(_.Name))` and `_select((Loud = my_upper(_.Name),
  Id = _.Id))` both push computed-fragment slots through the shared
  `push_computed_proj_slot` helper (also used by the JSON-path and
  LEFT JOIN `is_some/is_none` projection paths).
- **Function-pointer disambiguation via explicit `funcType`.** The
  emitted thunk uses `new ExprAddr(target, funcType)` rather than the
  qmacro `@@$c(name)` shape so the typer resolves to the user's
  `func` even when an imported module exports a same-named function
  with a different signature (e.g. user `normalize(string)` colliding
  with `math::normalize(float2)`).
- **Coexists with manual `register_function`.** Both go through
  `_register_function_check_rc`. Registry installs first at open;
  later manual calls override. Use `register_function` for
  per-connection / one-off; `[sql_function]` for ambient SQL helpers
  visible to chain analysis.
- **Failure modes:** generic functions rejected (no concrete name to
  emit); unsupported arg/return types rejected per-position; >4 args
  rejected — all surface as 30111 macro-apply failures with the
  per-position diagnostic from the underlying tag derivation.
- Tutorial: [tutorials/sql/32-sql_functions.das](../../tutorials/sql/32-sql_functions.das)
  (extended with `[sql_function]` examples after the manual
  `register_function` section).
  Tests: `test_78_sql_function_annotation.das` (14 positive — auto-reg
  raw SQL, chain visibility 1-arg + 2-arg, `name=` override raw +
  chain, `deterministic=true` accepted, multiple opened DBs see same
  function, `directonly=true` works in raw SQL + non-view chain,
  collision with `math::normalize` resolves via `ExprAddr.funcType`,
  single-column projection `_select(my_upper(_.Name))` + emit-shape
  pin, named-tuple projection mixing computed and column-ref slots,
  named-tuple with two-arg `[sql_function]`),
  `test_79_sql_function_dup.das` (1 — install-time detection of
  duplicate `(name, arity)` registrations, isolated file because the
  dup poisons the registry for the whole context),
  `failed_sql_function_annotation.das` (9 errors — struct arg,
  pointer arg, > 4 args, struct return, empty `name=`,
  invalid-identifier `name=`, non-string `name=`, non-bool
  `deterministic=`, non-bool `directonly=`),
  `failed_sql_function_in_view.das` (3 cases — `[sql_function(directonly=true)]`
  rejected inside `_create_view` body via `pred_fail`: outer WHERE,
  nested `_in(...)` subquery WHERE, and `_select` projection slot).

### Cumulative state after chunk 10

381 (chunk 9) + 21 (chunk 10) = ~402 dasSQLITE tests passing.

### Carried / deferred to chunk 11+

- **SQL-fragment refactor** — SHIPPED in chunk 11 (`SqlFrag` variant +
  `fold_to_string` / `fold_to_builder` consumers in sqlite_linq.das).
  All 13 macro emissions route through `fold_to_builder`; const-folding
  collapses all-text/all-bind chains to a single `ExprConstString` so
  AOT codegen is byte-identical to the previous `$v(sql)` baking path.
  PR-B follow-on adds `_sql_pragma(db, name, value)` and
  `_sql_vacuum_into(db, path)` macros that exercise the
  `inline_id` / `inline_lit` SqlFrag kinds — both fold to literal SQL
  when args are compile-time constants, falling back to a builder
  with `sql_quote_id` / `sql_quote_lit` calls otherwise. PR-C extends
  `_order_by` / `_order_by_descending` to accept a `string`-typed
  expression (single col or tuple element) — the SQL build emits a
  `\x01` placeholder consumed by `sql_to_frags_ex`, which routes the
  expression through `fold_to_builder` as an `inline_id` frag. Const
  strings still fold to compile-time-quoted SQL; runtime variables emit
  `sql_quote_id(<expr>)` so embedded `"` is doubled safely. Mixed
  tuples (e.g. `(_.Price, runtimeName)`) work — each entry independently
  picks the compile-time `_.Field` or runtime-string path. `_create_view`
  rejects runtime ORDER BY (DDL is run-once; runtime column would bake at
  view-creation time).
- **Migrations (last chunk)** — `daslib/sql_migrate` module,
  `[sql_migration]` annotation collected across translation units,
  `migrate_to_latest` runner, `__schema_version` table, multi-row
  audit semantics.
- **Aggregate / window UDFs** — `xStep` / `xFinal` / `xValue` /
  `xInverse`. v1's `register_function` is scalar-only.
- **`[sql_function]` custom-type adapter composition** — base
  `[sql_function]` shipped (auto-registration on connect + chain
  visibility, see Tut 39 follow-up above); custom-type adapter
  composition for non-scalar return / arg types still deferred.
- **Updatable views via INSTEAD OF triggers.**
- **ATTACH DATABASE** (tut 36 — independent SQLite extension).
- **`_try_each_sql`** — Result-yielding iterator variant.
- **Backup progress callback** — `sqlite3_backup_step(bp, N)` +
  `remaining` / `pagecount` surfaced to a daslang block.
- **Compile-time enum surface for pragmas** — `set_journal_mode(JournalMode.WAL)`
  rather than the stringly-typed name.
- **Open-time named-tuple pragma bundle** — `with_sqlite(path,
  pragmas=(...))`.
- **Option<T> args / blob args / blob return** for `register_function`.

## Shipped — chunk 9: JSON / BLOB columns + column metadata + raw `query` family (branch `dassqlite-chunk9-json-introspection`)

Chunk 9 ships the headline JSON / opaque-blob feature plus the
schema-introspection cluster: tuts 28 (JSON / BLOB columns), 29 (column
metadata), 30 (listing tables). All three sit on top of the chunk-8
adapter rail and the existing `[sql_table]` field-walking machinery.
No new architecture decisions — designs locked in §37-json (8 locks),
§09-column_names, and §10-list_tables.

### Pre-step refactor: factor the executor functions, add `query` family

- **A1.** Factored `try_one_row` / `try_one_row_opt` / `try_step_dml` /
  `try_collect_rows` into shared `try_with_stmt` helper. ~80 lines of
  duplicated prepare/finalize boilerplate → ~20.
- **A2.** Added `try_query` / `query` (4 overloads each, 0/1/2/3 bind
  args). The missing fourth member of the `query_*` family — multi-row
  typed raw-SQL read. Backs tut 29 Band 3 + tut 30.

### Tut 28 — `@sql_json` + `@sql_blob`

- **`@sql_json` field annotation** — TEXT-backed via `daslib/json` +
  `daslib/json_boost`. `[sql_table]` generates the `sql_bind` /
  `sql_extract` adapter pair at module scope: `write_json(JV(v))` /
  `from_JV(read_json(...))`. DDL emits `TEXT`.
- **`@sql_blob` field annotation** — BLOB-backed via `daslib/archive`.
  Adapter codegen: `mem_archive_save` / `mem_archive_load`. DDL emits
  `BLOB`.
- **Validation** — `[sql_table]` rejects: both annotations on one
  field; either annotation on `@sql_primary_key`; either on
  `@sql_computed`. Adapter dedup is per-`(T, kind)`, keyed off the
  return type of any existing `sql_bind(T)` (string for `@sql_json`,
  `array<uint8>` for `@sql_blob`) so two `[sql_table]`s sharing a
  JSON-typed field don't double-generate. Using the same `T` with
  different kinds (across structs in the module, or across fields in
  one struct) is rejected at compile time — silent miscompile would
  otherwise route a BLOB column through the JSON adapter or vice
  versa. Wrap in a typedef when two storage views of the same payload
  are genuinely needed.
- **`_sql` walker JSON-path descent (pred-side AND projection-side)**
  — `_.<json_col>.<path…>` lowers to `json_extract("<col>",
  '$.<path…>')`. Arbitrary depth. Multi-source SELECT (joins) emits
  `json_extract("<alias>"."<col>", '$.…')`. `@sql_blob` columns are a
  compile error on descent — the walker reads field annotations off
  the source struct, not off the type.
- Tutorial: [tutorials/sql/28-json.das](../../tutorials/sql/28-json.das).
  Tests: `test_64_json_columns.das` (18 tests — round-trip, DDL,
  adapter dedup, pred-side and projection-side walker descent),
  `test_65_blob_struct.das` (5 tests — multiple `@sql_blob` fields,
  array/table/nested archive payloads, mixed plain + JSON + BLOB).

### Tut 29 — `column_info(type<T>)` + `sqlite_sql_type`

- **Abstract types** in `daslib/sql`: `enum SqlType { Integer; Real;
  Text; Blob; Null }` and `struct ColumnInfo { name; data_type; is_pk;
  is_nullable; default_expr }`. `SqlType` lives in the abstract layer
  so the same `ColumnInfo` round-trips across providers.
- **Witness machinery** in `sqlite_boost`: `sql_storage_enum_witness`
  (mirror of `sql_storage_type_witness`) + `sql_storage_enum_for(type<T>)`
  resolves the enum at infer time via `_::sql_bind`'s return type.
- **Provider helper** `sqlite_sql_type(t : SqlType) : string` —
  `INTEGER`/`REAL`/`TEXT`/`BLOB`/`NULL`. Future
  `postgres_sql_type`, `mysql_sql_type` ship next to their respective
  boost modules.
- **`[sql_table]` codegen** — `make_column_info_fn` synthesizes a
  per-struct `_sql_column_info(typ : T) : array<ColumnInfo>` helper
  that pushes one `ColumnInfo` per field. `@sql_json` / `@sql_blob`
  fields short-circuit to `SqlType.Text` / `SqlType.Blob`; other
  fields call `sql_storage_enum_for` for infer-time resolution.
  `default_expr` is the SQL-form expression with the leading
  `" DEFAULT "` prefix stripped.
- **User-facing dispatcher** `column_info(t : type<auto(TT)>) :
  array<ColumnInfo>` — calls `_::_sql_column_info(default<TT>)`.
- Tutorial: [tutorials/sql/29-column_names.das](../../tutorials/sql/29-column_names.das).
  Tests: `test_66_column_info.das` (21 tests — cardinality, ordering,
  PK/nullable/data_type/default_expr per column, computed inclusion,
  JSON+BLOB short-circuit, `sqlite_sql_type` for all 5 enum values,
  composability with linq comprehensions).

### Tut 30 — Listing tables

No new API. Tutorial-only — falls out of `query("SELECT … FROM
sqlite_master …", type<MasterRow>)`. Decision (locked § 10-list_tables):
no abstract `list_tables` helper; catalog spelling is genuinely
provider-specific. Tutorial:
[tutorials/sql/30-list_tables.das](../../tutorials/sql/30-list_tables.das).
The existing `test_67_query_raw.das` `test_query_sqlite_master_lists_user_tables`
test already covers the path end-to-end.

### Cumulative state after chunk 9

381 dasSQLITE tests passing (328 pre-chunk-8 + 27 chunk-8 +
26 chunk-9). New tests: `test_64_json_columns.das` (+18),
`test_65_blob_struct.das` (+5), `test_66_column_info.das` (+21).

### Deferred to chunk 10+

- **`@sql_json_index("$.path")`** — typed expression index on a JSON
  path. Cross-provider DDL diverges; raw `CREATE INDEX` until a second
  backend lands.
- **`_json_extract<T>(col, path)`** — explicit escape for paths the
  walker can't reach (array subscripts, custom JSON operators).
- **`@sql_json(fast)`** — `json_sprint` / `json_sscan` fast path.
- **JSON patch / merge inside `_update`** — `json_set` / `json_patch`
  in chain-form updates.
- **JSONB binary storage** (SQLite 3.45+) via `@sql_json(binary)`.
- **`@sql_blob` compression** (zstd / lz4) and versioned-format header.
- **Cross-provider JSON-path emitter** — PG `->>`, MSSQL `JSON_VALUE`.
- **`@sql_blob` field-descent compile-error negative test** — first
  attempt hit an unrelated archive codegen issue for string-only
  inner structs. Track separately.
- **Carried from chunk 8+:** per-field `@sql_as(type<P>)`, struct-type
  `_select(type<T2>)`, bulk `array<T>` upsert, composite FKs, partial /
  expression indexes, function-reference `@sql_computed`, DEFAULT-firing
  from the macro INSERT path, optimistic concurrency token, multi-table
  DELETE, named-tuple bind for `query_one` / `exec`.

## Shipped — chunk 4: read-side depth — distinct/take/skip/order_by/aggregates/group_by/NULL (branch `dassqlite-chunk4-read-depth`)

Chunk 4 broadens the chunk-3 framework with the rest of the read-side
operators and ships tutorials 7, 10, 11, 12, 13, 14, 18. Set
operations (UNION/INTERSECT/EXCEPT — tut 12 second half) and joins +
subqueries (tuts 15–17) are deferred to chunk 5: they all need the
same multi-source / multi-stage SqlQuery extension.

### New chain operators (all in `analyze_chain` / `pred_to_sql`)

- **`distinct()`** — sets `q.distinctFlag`; emits `SELECT DISTINCT`.
- **`take(n)` / `skip(n)`** — bind expressions stashed in
  `q.limitBindExpr` / `q.offsetBindExpr`; pushed *after* WHERE binds
  in `emit_sql_macro_body`. Solo `skip(n)` emits `LIMIT -1 OFFSET ?`
  to satisfy SQLite's OFFSET-requires-LIMIT rule. Single-row
  terminals (`_first` / `_first_opt`) override `take`'s `LIMIT`.
- **`_order_by(_.Col)` / `_order_by_descending(_.Col)`** — single-key
  ASC/DESC. Tuple-key form `_order_by((_.k1, _.k2))` emits each
  tuple field as its own `ORDER BY` column. Recurse-first on the
  peel so downstream `seenGroupBy` is known when analyzing the key.
  Mixed ASC/DESC across columns is **deferred** (D2).
- **`sum` / `average` / `min` / `max`** — terminal column aggregates
  via `peel_column_aggregate`. Recurse-first: peel the aggregate,
  let the inner chain populate `selectCols`, then wrap
  `selectCols[0]` in the SQL aggregate. AVG promotes to `double`;
  SUM/MIN/MAX inherit the column type. LIMIT 1 suppressed for
  aggregate Materializer.
- **`_group_by(_.Col)` / `_group_by((_.k1, _.k2))`** — single- and
  multi-key grouping via the existing linq.das `group_by_lazy`
  lowering. Adds `ProjectionShape.GroupedNamedTuple` with parallel
  `q.groupedSelectExprs` / `q.groupedSelectTypes`. Macro-side
  recognition of the IGrouping shape: `_._0` for the group key
  (single-key), `_._0._N` for multi-key, `_._1` for group rows;
  `_._1 |> length` / `_._1 |> count` → `COUNT(*)` (result type
  `int` not `int64` — matches daslang's `length`/`count` returns);
  `_._1 |> select($(u : T) => u.Field) |> sum/average/min/max` →
  `SUM("Field")` / etc.
- **`_having(predicate)`** — recurse-first; rejected if
  `!seenGroupBy` or wrong order. Bind exprs route to
  `q.havingBindExprs` and slot in between WHERE and LIMIT in the
  placeholder index sequence.
- **NULL handling — Option<T> in `_where` predicates:**
  `_.Col |> is_some` → `Col IS NOT NULL`;
  `_.Col |> is_none` → `Col IS NULL`;
  `_.Col |> unwrap_or(default)` → `COALESCE(Col, ?)` (default routed
  through `pred_to_sql` so both captured locals and literals are
  emitted as `?` binds — same default-parameterize behavior as the
  rest of `_where`).

### `[sql_table]` runtime extensions for `Option<T>`

- `is_option_field_type(td)` recognizes both pre-resolution
  (`Type.typeMacro` with `dimExpr[0]` ExprConstString = "Option")
  and post-resolution (`Type.tStructure` with module=="option")
  forms — structure macro runs before the `[template_structure]`
  expansion, so the typemacro form is what actually shows up.
- `sqlite_sql_type` / `sqlite_bind` / `sqlite_read` overloads for
  `$Option<auto(TT)>`. Bind branches on `_has_value` and binds
  `sqlite3_bind_null` for `none`. Read branches on
  `sqlite3_column_type == SQLITE_NULL` and wraps in `some(v)` /
  `none()`.
- DDL emitter omits `NOT NULL` for `Option<T>` columns; rejects
  `@sql_primary_key` on `Option<T>` via `errors:=`.
- `_sql_read_row` body uses `var row = default<$t(st)>` so
  Option-bearing user structs default-init cleanly under
  `strict_smart_pointers`.

### IGrouping shape vs the original mockup (locked 2026-04-26)

The original `19-group_by.das.mockup` used aspirational syntax
(`_count_all()`, `_sum(_.X)`, `_avg(_.X)`) inside the post-group
`_select`. After `_group_by` lowers to `group_by_lazy`, the chain
element is `tuple<KeyType; array<RowType>>`, so the next `_select`
sees `_:tuple<...>` — `_.X` doesn't typecheck (the tuple has
`_0`/`_1`, not `X`).

Chunk 4 ships the **IGrouping shape** (matches existing
`tests/linq/test_linq_group_by.das`):

- `_._0` for the group key (single-key), `_._0._N` for multi-key
- `_._1` for the group element array
- `_._1 |> length` / `_._1 |> count` → `COUNT(*)`
- `_._1 |> select($(u : RowType) => u.X) |> sum/average/min/max` →
  the matching SQL aggregate

The inner `select` lambda parameter is intentionally **named**
(`$(u : User)` etc.), not `_` — the outer `_select` lambda already
binds `_` to the group tuple, and daslang flags a same-name-
different-type lambda nested inside as a shadowing error. Naming
the inner parameter sidesteps it; the macro recognizes the body as
`<param>.<field>` for any parameter name. This aligns with EF Core
/ C# LINQ shape (`g.Key` ↔ `_._0`, `g.Count()` ↔ `_._1 |> length`,
`g.Average(u => u.Age)` ↔ `_._1 |> select($(u) => u.Age) |> average`).

### Deferred to chunk 5+

- **Set operations (UNION / INTERSECT / EXCEPT)** — tut 12 second
  half. Single-source `SqlQuery` today; needs `setOpKind` +
  `setOpRhsQuery` plus the multi-stage emitter. Bundled with joins
  and subqueries.
- **`_join` / `_left_join`** (tuts 15-16) — multi-source FROM,
  equi-join predicate extraction.
- **Subqueries — `_in` / `_not_in` / `_any` / `_none`** (tut 17) —
  nested SqlQuery walker, outer-lambda-param classifier for
  correlated columns, scalar-subquery embedding in predicates.
- **`_then_by` and multi-key ordering protocol (D1)** — needs an
  `IOrderedEnumerable<T>`-equivalent in daslib/linq. Chunk-4
  workaround: tuple-key `_order_by((_.k1, _.k2))` emits each tuple
  field as its own `ORDER BY` column.
- **Mixed ASC/DESC across columns (D2)** — `ORDER BY a ASC, b DESC`
  in one chain step. Possible future syntax: marker-in-tuple
  `_order_by(@(x) => (x.a, desc(x.b)))`. Out of chunk 4 scope; raw
  SQL escape hatch handles it today.
- **`Option<T> == none()` in `_sql` predicates (D4)** — currently
  not translated. Lock decision (macro_error with fixit vs silent
  rewrite to `IS NULL`) when a real test forces it; leaning
  fixit-with-macro_error for explicit-over-magic.
- **Projection-side `_.Col |> unwrap_or(d)` in `_select`** — the
  predicate-side rule lands this chunk; the projection-side
  COALESCE case requires extending `analyze_projection` beyond
  plain `_.Field` shape and is deferred.

### Surface in this PR

- **Tutorials 7, 10, 11, 12, 13, 14, 18** under `tutorials/sql/`.
- **6 new test files** under `tests/dasSQLITE/`:
  - test_32 — `distinct`
  - test_33 — `take` / `skip`
  - test_34 — `_order_by` (single-key, descending, tuple-key)
  - test_35 — column aggregates (sum/average/min/max)
  - test_36 — `_group_by` + `_having` (single/multi-key, full reporting query)
  - test_37 — NULL handling (DDL, round-trip, is_some / is_none /
    unwrap_or in `_where`, full-row Option pass-through)
- `failed_sql_macro.das` extended with bad10 (`_having` without
  `_group_by`) and bad11 (bare scalar projection after `_group_by`).
- **205/205 dasSQLITE tests pass** interpreted; chunk-2 (81),
  chunk-3 (70), chunk-4 (54). AOT path stays green; lint clean.
- New RST: `sql_07_anatomy.rst`, `sql_10_order_by.rst`,
  `sql_11_take_skip.rst`, `sql_12_distinct.rst`,
  `sql_13_aggregates.rst`, `sql_14_group_by.rst`,
  `sql_18_null_handling.rst`. Sphinx `-W` clean.

## Appendix C — `_sql` translation boundary (cross-reference for tutorial 7)

What `_sql(chain)` translates and what it refuses, in one place. The
macro walks the chain bottom-up after Mode-2 expansion has unfolded
inner call_macros (`_where` → `where_(it, $(_:T) => body)`, etc.).

**Translates:**

- **Chain root:** `select_from(db, type<T>)` where `T` is a `[sql_table]`
  struct.
- **Chain operators:** `_to_array()`, `_first()`, `_first_opt()`,
  `count()` (chunk 3); `_sum`/`_avg`/`_min`/`_max`/`_order_by`/`_take`/
  `_skip`/`_distinct`/`_group_by`/`_having`/`_join`/`_left_join`/
  subqueries (chunk 4+).
- **Projections (`_select`):** `_.Field` (single column),
  `(Name=_.Name, Price=_.Price)` (named tuple). Struct-type projection
  is chunk 4.
- **Predicates (`_where`):** column refs (`_.Field`), captured vars and
  literals (auto-bound as `?`), comparisons (`==`, `!=`, `<`, `<=`, `>`,
  `>=`), logic (`&&`, `||`, `!`), and the appendix-A function calls
  (`starts_with`, `ends_with`, `contains`, `to_lower`, `to_upper`,
  `length`, `abs`).

**Refuses with a compile-time `macro_error` pointing at the offending
node:**

- Unknown function calls in `_where` (anything not in the dispatch table).
- `_select` projections that aren't `_.Field` or named-tuple.
- Multiple `_select` calls in one chain.
- Multiple terminals in one chain (`_to_array() |> _first()` etc.).

**Workarounds:**

- Use `db |> exec(sql)` / `try_exec(sql)` for DDL and arbitrary
  statements that don't fit the chain.
- Use `db |> query_one(sql, type<T>, args…)` /
  `db |> query_scalar(sql, type<T>)` for hand-written SELECT statements
  that return rows.
- The `_sql_text(chain)` companion macro returns the exact SQL string
  that `_sql(chain)` would emit (with `?` placeholders) — useful for
  inspecting generated SQL while debugging.

### Surface in this PR

- **Tutorial 04** is now real (`tutorials/sql/04-select_all.das` — relocated
  from `modules/dasSQLITE/tutorial/` to the project-wide tutorials home in
  chunk 2.5) — first end-to-end exposure to `_sql`.
- **Tests:** `tests/dasSQLITE/test_05_sql_macro.das` (17 execution cases),
  `tests/dasSQLITE/test_06_sql_text.das` (19 SQL-string-emission cases),
  `tests/dasSQLITE/test_07_sql_composability.das` (10 user-wrapper cases),
  `tests/dasSQLITE/test_07_macros.das` (companion module defining
  `when_price_lt` and `when_lt` wrapper call_macros),
  `tests/dasSQLITE/failed_sql_macro.das` (`expect`-based macro-error cases).
  `tests/dasSQLITE/test_04_user_types.das` extended with a `select_from`
  round-trip exercising the user-side `sqlite_read` overload.

## SQL read-side prerequisites (daslib/linq + daslib/linq_boost rework)

The `_sql(...)` chain is a thin SQL-emitting translator over daslib/linq.
Every read-side chain stage in dasSQLITE (`_where`, `_select`, `_order_by`,
`_group_by`, `_having`, `_take`, `_skip`, `_count`, `_sum`, `_avg`, `_join`,
etc.) lowers to an existing daslib/linq function; the `_sql` macro recognizes
the chain shape and emits SQL instead of an in-memory iterator.

This means **daslib/linq / daslib/linq_boost get extended first — before
dasSQLITE's `_sql` emitter can rely on the stage being there.** These
extensions are useful on their own (in-memory pipelines, non-SQL data
sources), but the dasSQLITE plan treats them as mandatory prerequisites.

**Principle:** never bake a SQL-only macro into dasSQLITE if the same chain
stage would also make sense for plain `array<T>` / `iterator<T>`. Ship it in
`daslib/linq_boost` (or `daslib/linq` when a backing fn is needed) and let
`_sql` be one consumer among several. Only the `_sql(...)` call macro itself,
the `[sql_table]` structure macro, and the per-provider emitter / bind-param
plumbing are dasSQLITE-specific.

**Known additions required (accumulates as tutorial audits land):**

| Added | To | Scope | Covered by tutorial |
|---|---|---|---|
| `_group_by(key)` block-macro (1-arg key, **fuses** with next `_select` to recognize group-scope `_count()` / `_sum(_.X)` / `_avg(_.X)` aggregates) | `daslib/linq_boost` | Macro-only (backing `group_by` already at `linq.das:1575-1610`) | 19 |
| `_group_by_lazy(key)` block-macro (1-arg key, no fusion — yields `tuple<Key; iterator<T>>` IGrouping shape; `macro_error` inside `_sql(...)`) | `daslib/linq_boost` | Macro-only | 19 |
| `_having(predicate)` block-macro (post-aggregate filter; in-memory lowers to `where` on grouped iter, emits HAVING under `_sql`; captured-var-to-bind reuses `_where`'s rewriter) | `daslib/linq_boost` | Macro-only | 19 |
| Group-scope aggregate recognition inside post-`_group_by` `_select` (`_count()` / `_sum(_.X)` / `_avg(_.X)` / `_min(_.X)` / `_max(_.X)` become aggregate calls on the group iterator, not row-projections) | `daslib/linq_boost` | Macro-only | 19 (building on 18's primitives) |
| `_join(other, on, into)` block-macro — inner join, equi-join only (`on` predicate must be `(l,r) => l.X == r.Y`; theta joins `macro_error` with a pointer at raw SQL). Lowers to linq.das's existing `join(srca, srcb, keya, keyb, result)` by splitting the `==` predicate into the two key selectors. | `daslib/linq_boost` | Macro-only (backing `join` at `linq.das:1521-1534`) | 23 |
| **`left_join(srca, srcb, keya, keyb, result)` fn** — thin wrapper over `group_join`: for each TA with matching array-of-TBs, emit one row per TB wrapped in `Option::some`; for each TA with no match, emit one row with `Option::none`. 5-10 lines. **First `daslib/linq` addition — all prior bucket-B items were macro-only.** | `daslib/linq` | New fn | 23 |
| `_left_join(other, on, into)` block-macro — sugar over the new `left_join` fn. Same equi-join constraint as `_join`. Right-side projection parameter is `Option<TB>`. Under `_sql` emits `LEFT JOIN`. | `daslib/linq_boost` | Macro-only | 23 |
| ~~`_select_many(nav_key, into)` block-macro~~ — **DROPPED per tut 26 decision** (2026-04-24). Was a forward-reference to navigation-property metadata; when tut 26 decided not to ship nav properties, this macro lost its reason to exist. The backing `select_many(src, coll, result)` fn stays in `linq.das` as an in-memory array primitive — just no SQL-side sugar macro. | — | Dropped | 23 (forward-ref, now dead) |
| `_in(x, subquery)` block-macro — sugars over existing `contains(subquery, x)` with arg order flipped for SQL-native reading. Under `_sql` emits `x IN (…)`. | `daslib/linq_boost` | Macro-only (backing `contains` at `linq.das:1927-1934`) | 24 |
| `_not_in(x, subquery)` block-macro — separate name, NOT `!x._in(...)`. Under `_sql` emits `x NOT IN (…)`. | `daslib/linq_boost` | Macro-only | 24 |
| **`none(src)` / `none(src, predicate)` fn** — mirror of `any`: returns true iff zero elements match. ~10 lines. Independently useful for in-memory code (`none(xs, _ < 0)` reads clearer than `!any(xs, _ < 0)`). | `daslib/linq` | New fn | 24 |
| `_none(predicate)` call-macro — same `AstCallMacro_LinqPred2` pattern as `_any` / `_all` at `linq_boost.das:161-169`. Under `_sql` emits `NOT EXISTS (…)`. Explicit name (not `!_any(...)`) because the `_sql` AST walker cannot reliably pattern-match `!expr` across intermediate transforms. | `daslib/linq_boost` | Macro-only | 24 |
| (NULL handling — `Option<T>` methods in predicates and projections. `_.Col \|> is_some()` / `is_none()` emit IS NOT NULL / IS NULL; `\|> unwrap_or(default)` emits COALESCE; `Option<T>` carries through projections to result tuple fields.) | — | **Zero linq prereqs.** All work is in the `_sql` emitter recognizing existing `Option<T>` methods from PR #2471. | 25 |
| (Foreign keys — `@sql_references(type="T", on_delete=..., on_update=...)` field annotation. DDL-only FK constraints + CASCADE; navigation properties deliberately NOT shipped.) | — | **Zero linq prereqs.** All work is in the `[sql_table]` structure macro's DDL emitter + `with_sqlite` setting `PRAGMA foreign_keys=ON`. | 26 |
| (Indexes — stackable struct-level `[sql_index(fields=..., unique=..., name=...)]` annotation. Emits one `CREATE [UNIQUE] INDEX` per annotation after the table.) | — | **Zero linq prereqs.** All work is in the `[sql_table]` structure macro's DDL emitter. Query side is transparent — SQLite picks indexes automatically. | 27 |
| (Defaults + computed — `@sql_default(value=...)` / `@sql_default(sql_fn="CURRENT_TIMESTAMP")` and `@sql_computed(sql="...", stored=false)` field annotations. DDL-only + bind-code excludes computed fields from INSERT/UPDATE.) | — | **Zero linq prereqs.** All work is in the `[sql_table]` structure macro's DDL emitter + bind-code generator. Expression validation runtime-deferred to SQLite; lambda/`@@func` form flagged for later. | 28 |

Table rows marked `?` are pending the per-tut audit. Finalized rows
correspond to locked tutorials (see per-tutorial sections below).

**Pattern observation** (2026-04-24): the "`!expr` isn't AST-walkable"
constraint means every negated SQL form needs its own explicit name —
`_none` for NOT EXISTS, `_not_in` for NOT IN. Expect similar explicit
positive/negative pairs for any future predicate primitives (is_null /
is_not_null come up in tut 25). Naming convention: negative form gets
the `_not_` or `_no` prefix on the positive name.

**Not shipped in this rework** (per tut 23 decision):
`full_outer_join`, `right_join`, `cross_join`. `RIGHT JOIN` = `_left_join`
with swapped args; `CROSS JOIN` reads as an accidental-footgun and belongs
in raw SQL; `FULL OUTER JOIN` is rare. Add on real demand.

**Placement open question (not blocking design):** should SQL-only concerns
like "emit HAVING instead of WHERE" live in `daslib/linq_boost` (simpler,
works everywhere) or in a new SQL-scoped `daslib/sql_boost` sibling
(cleaner separation, but duplicates chain machinery)? Current lean: ship in
`daslib/linq_boost`, since `_having` and `_group_by` *are* in-memory useful
(they're just more primitive in SQL land). Revisit if the macro table
balloons with SQL-smelling entries.

### Plan phase ordering (locked 2026-04-24)

The dasSQLITE implementation plan lists these as **Phase 0 — prerequisites**,
ahead of any dasSQLITE-specific phase. The order:

1. **Phase 0.1** — PR #2471 `daslib/option` + `daslib/result` ✅ **merged 2026-04-24** (commit `56934f491`).
2. **Phase 0.2** — Named-tuple literal `(Name=val, …)` ✅ **already in the parser** (ds2_parser.ypp `'(' make_struct_single ')'`, verified 2026-04-24). No work needed.
3. **Phase 0.3** — daslib/linq_boost macro additions (table above).
   Per-tut audit fills rows; lands per-tut or batched — no dasSQLITE
   dependency, so order doesn't matter relative to 0.1 / 0.2. Each
   row ships with its own tests using plain `array<T>` / `iterator<T>`
   inputs (no SQL dependency in the tests).
4. **Phase 0.4** — `daslib/strings_boost::text_match(text, query) : bool`.
   In-memory FTS5-query-syntax subset (whitespace-AND, `*` prefix,
   Unicode word-boundary + ASCII case-fold). Unsupported subset
   (NEAR, AND/OR/NOT, phrases, column filters) → panic pointing at
   SQL backend. Prerequisite for tut 39's `_sql` translator rule:
   `text_match` on an `[sql_fts5]`-indexed column emits `col MATCH ?`;
   on a regular `[sql_table]` column → compile error suggesting
   `contains` or `[sql_fts5]`. Useful outside SQL for log/config/doc
   filtering — the function is a general daslang feature, not an
   SQL adapter. Added 2026-04-24 alongside the tut 39 lock.

Only once 0.1 / 0.2 / 0.3 / 0.4 are all in can the `_sql(...)` emitter in
dasSQLITE ship — because `_sql` translates `_group_by` + `_having` + named-
tuple projections + `Option<T>` returns + `Result<T,string>` failure paths
+ `text_match`/`starts_with`/`ends_with`/`contains` string predicates,
and each of those is a prerequisite.

Files named `NN-topic.das` correspond 1:1 to `tutorial/NN-topic.das` and
demonstrate the happy-path (strict, panic-on-error) shape. Files prefixed
`_topic.das` (underscore, sorts first) cover cross-cutting concerns that
don't map to a specific tutorial — e.g. `_error_handling.das` walks the
full `try_`/`_opt` surface in one focused file.

**Inspiration:** C# LINQ-to-SQL / Dapper, Rust `rusqlite`, Python `sqlite3`,
Kotlin Exposed. Common theme: connection as a scoped resource, one-liner
queries that return rows as typed sequences, parameters by name or position
without manual binding, errors by exception rather than by return-code check
after every call.

## Current state (as of 2026-04-23)

- `modules/dasSQLITE/src/` — auto-generated thin C binding over libsqlite3.
  Functions appear 1:1 with the C API (`sqlite3_open`, `sqlite3_prepare_v2`,
  `sqlite3_step`, `sqlite3_column_text`, `sqlite3_finalize`, `sqlite3_close`,
  …). Pointers to pointers (`sqlite3**`) are exposed as `unsafe(addr(db))`.
- `modules/dasSQLITE/daslib/sqlite_boost.das` — 47 lines, four helpers. Name
  "boost" is aspirational; it offers essentially nothing on top of the raw
  binding. Contents:
  - `sqlite3_column_text_` — clones result to daslang-owned `string`
  - `sqlite3_free(string)` — untyped-ptr reinterpret
  - `sqlite3_bind_blob` overload taking any array
  - `sqlite3_get_autocommit_` — `int` → `bool`
  - `sqlite3_open(file, var db)` — hides the `addr(db)` cast
  - `sqlite3_prepare_v2(db, sql, var stmt)` — hides the `addr(stmt)` cast
- Tutorials under `modules/dasSQLITE/tutorial/` translate the zetcode C
  examples almost verbatim.

## Tutorial notes

### 01-version — "print SQLite version, run one SELECT"

**What the user writes today** (29 lines):

```das
require sqlite/sqlite_boost

[export]
def main {
    print("Hello, sqlite version {sqlite3_libversion()}\n")
    var db : sqlite3?
    var rc = sqlite3_open(":memory:", unsafe(addr(db)))
    if (rc != SQLITE_OK) {
        print("Can't open database: {sqlite3_errmsg(db)}")
        sqlite3_close(db)
        return
    }
    var res : sqlite3_stmt?
    rc = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, unsafe(addr(res)), null)
    if (rc != SQLITE_OK) {
        print("Failed to fetch data: {sqlite3_errmsg(db)}\n")
        sqlite3_close(db)
        return
    }
    rc = sqlite3_step(res)
    if (rc == SQLITE_ROW) {
        print("sqlite3_column_text(res, 0) = {sqlite3_column_text_(res, 0)}\n")
    }
    sqlite3_finalize(res)
    sqlite3_close(db)
}
```

**Observations:**

1. **Wrong require path.** `require sqlite/sqlite_boost` — the file lives at
   `modules/dasSQLITE/daslib/sqlite_boost.das`. Works only because of how the
   module is laid out; for everyone else reading this tutorial it's a riddle.
   Naming/layout should be predictable: `require daslib/sqlite` or
   `require dasSQLITE/sqlite` — one canonical path, not two.
2. **Manual `addr(var)` even though the boost exists.** `sqlite_boost` already
   has `sqlite3_open(file, var db)` and `sqlite3_prepare_v2(db, sql, var stmt)`
   overloads that hide the unsafe `addr`. The tutorial ignores them. Either
   the boost wins and raw C calls disappear from tutorials, or the boost goes
   away — having both is the worst outcome.
3. **Three repetitions of the same error-check/close/return dance.** Opening a
   DB, preparing a statement, and stepping are all `int`-returning calls where
   a non-OK code means "print errmsg, manually close, bail". That's not an
   API, that's a template. The API must hoist this into *one* decision point
   (exception / panic / `Result<T>`), not three.
4. **`sqlite3_close` even on the open-failure path.** SQLite's own contract
   requires this — `sqlite3_open` allocates the handle even on failure — and
   the tutorial does it, but a user who forgets leaks. Resource lifetime
   belongs to the library, not the tutorial reader.
5. **Trailing-underscore wart: `sqlite3_column_text_`.** Exists only because
   the C function returns `const unsigned char*` and the daslang binding
   refuses to auto-string it. Name is user-hostile.
6. **No RAII / scope-based cleanup.** `inscope` + a `[finalize]` on a wrapper
   type closes the handle automatically. A LINQ-style
   `using_db(path) <| $(db) { … }` is even friendlier (block-scoped, can't
   forget, no leak on early return).
7. **The entire example is "one SELECT returning one scalar".** That should
   be a one-liner. Dapper: `conn.ExecuteScalar<string>("SELECT …")`. Rust
   rusqlite: `conn.query_row("SELECT …", [], |r| r.get(0))`. Target here:
   `db |> query_scalar("SELECT SQLITE_VERSION()", type<string>)` or, since no DB
   is needed at all for the version, just `sqlite_version()` as a module-
   level function.
8. **`sqlite3_libversion()` already works as a string** in the interpolated
   string, so we know `const char*` → `string` conversion works at the bind
   layer. The `_text_` wart is specifically about `const unsigned char*`.
   Fixable in the binder (cast `unsigned char*` → `char*` on text columns).
9. **`def main` (no parens) is fine** but tutorials should use `def main()`
   consistently — `def main { … }` is also legal in gen2 but less discovered.

**What we want 01 to look like (straw-man target — see mockup at
`tutorial-mockup/01-version.das.mockup`):**

```das
require daslib/sql
require sqlite/sqlite_boost

[export]
def main() {
    print("sqlite version: {sqlite_version()}\n")
    with_sqlite(":memory:") <| $(db) {
        print("server reports: {db |> query_scalar("SELECT SQLITE_VERSION()", type<string>)}\n")
    }
}
```

Five lines of body, no raw `sqlite3_*`, no `addr`, no `unsafe`, no `rc`
checks, no close/finalize, no error-path branching (errors panic or `try`).

**Why no `_sql` macro here:** tutorial 01 has no table, no predicates,
nothing to translate. The `_sql` macro only earns its keep from tutorial 04
onward (typed SELECT). `query_scalar<T>` stays the right shape for
"arbitrary one-off SQL returning a scalar" and is also what `_sql` expands
to internally for its scalar-terminal case.

**Implied API surface (first pass — will refine as tutorials accumulate):**

- `sqlite_version() : string` — module-level convenience wrapping `sqlite3_libversion` (lives under `sqlite/sqlite_boost` — SQLite-specific).
- `with_sqlite(path : string) <| $(db : SqlRunner) { … }` — scoped resource.
  Canonical form. Opens, passes owned runner handle to block, finalizes on
  block exit (including on panic). Lives under `sqlite/sqlite_boost`.
- `SqlRunner` — the abstract runner handle, lives under `daslib/sql`.
  The SQLite provider wraps `sqlite3?` plus its dialect table behind this
  type. `[finalize]` calls `sqlite3_close_v2` so plain
  `var inscope db = open_sqlite(path)` works for users who want explicit
  lifetimes.
- `db |> query_scalar(sql : string; T : type<$T>) : $T` — prepare / step /
  column-0 / finalize in one call. Panics on prepare/step error and on 0
  rows. Typed by the `type<T>` argument (gen2 rule).
- `db |> query_scalar_opt(sql, type<$T>) : $T?` — scalar-or-null variant
  for expected-empty lookups.

**Decisions locked 2026-04-23:**

- **Module layout:** keep `sqlite/sqlite_boost` separate-namespace convention
  (same as `dashv/dashv_boost`). `sqlite/sqlite` stays the auto-generated
  thin C binding; `sqlite/sqlite_boost` grows into the real user-facing API.
- **RAII:** ship *both* flavors, `with_sqlite` is the **canonical** form:
  - `with_sqlite(path) <| $(db) { … }` — canonical. Matches daslang's
    `with_*` idiom for scoped resources. Opens, passes the runner handle
    to the block, calls `sqlite3_close_v2` on block exit (including on
    panic / early return). Per-provider name — future backends ship their
    own `with_<provider>`.
  - `var inscope db = open_sqlite(path)` — also supported, for cases
    where the DB lifetime is the function body or a struct field. Backed
    by `[finalize]` on `SqlRunner`.
- **Generic-call syntax:** gen2 daslang passes types as *arguments*, not as
  angle-bracket type parameters. So `db |> query_scalar(sql, type<string>)`
  — **not** `db |> query_scalar<string>(sql)`. (CLAUDE.md Keywords
  Reference: *"Type function call: `take(type<int>, 1, 2)` — NOT
  `take < int > (1, 2)`"*.) This threads through every typed query helper
  below.
- **Error model:** panic = real crash, not recoverable flow control.
  daslang's `try/recover` is *not* suited for "this SQL might fail, handle
  it" — it is for genuine unwinding. Therefore:
  - **Programmer errors → panic.** Malformed SQL, missing table, column
    type mismatch, 0 rows from a scalar query that must return a row,
    `_sql` operator that can't translate.
  - **Expected operational failures → explicit return value.** File-not-
    found on open, unique-constraint violation on insert, `SQLITE_BUSY`,
    locked DB, 0 rows from a lookup-by-id. These get `T?`-returning
    variants (e.g. `try_open_sqlite`, `query_scalar_opt`, `try_insert`) that
    either return `null` with an out-param `var err : string&`, or return
    `T?` on a well-known empty case.

  The panicking form is the default; the `_opt` / `try_` form is the
  escape hatch. No `try/recover` in the user-facing API.

- **Resolved at tut 04:** Row representation is **named tuple**
  (`tuple<Name:string; Price:int>`). No synthetic per-query structs, no
  Dapper-style reflection. See the `_sql` / `select_from` section below.

### Strict vs. try_ variant family (API-wide convention)

**Requires PR #2471** (`daslib/option` + `daslib/result`, aleksisch,
open 2026-04-23). dasSQLITE rework depends on that PR landing / API
stabilizing. Not blocking design; blocking implementation.

Every write or read helper that can fail has two call shapes:

- **Strict form** — panics on any error (`sqlite3_errmsg` in the panic
  message), returns the success value directly. Default choice for
  tutorials and app code where a failure is a bug.
- **`try_` / `_opt` form** — returns `Option<T>` (when the only failure
  mode is "empty result") or `Result<T, string>` (when there are real
  error modes with useful errmsg). Composes through
  `map`/`and_then`/`??`/etc.

`T?` (nullable pointer) is **not** used for these returns — it's a
pointer-nullable, which is wrong for value types (no natural `int64?`)
and forces boxing. `Option<T>` is the right value-level optional.

| Strict (panic on error) | Optional / Result variant | Returns |
|---|---|---|
| `open_sqlite(path) : SqlRunner` | `try_open_sqlite(path) : Result<SqlRunner, string>` | handle or errmsg |
| `db \|> insert(row) : int64` | `db \|> try_insert(row) : Result<int64, string>` | new rowid or errmsg |
| `db \|> insert(array<T>) : int64` | `db \|> try_insert(array<T>) : Result<int64, string>` | last rowid or errmsg (txn rolled back) |
| `db \|> exec(sql [, args])` | `db \|> try_exec(sql [, args]) : Result<void, string>` | success or errmsg (pending `Result<void,E>` viability) |
| `db \|> query_scalar(sql, type<T>)` | `db \|> query_scalar_opt(sql, type<T>) : Option<T>` | value or none (0 rows) |
| `db \|> query_one(sql, type<T>)` | `db \|> query_one_opt(sql, type<T>) : Option<T>` | row or none (0 rows) |

Distinction in meaning:
- **`Option<T>`** when the only failure is "0 rows" — scalar/one-row
  reads where empty is an expected outcome. No errmsg to carry.
- **`Result<T, string>`** when real error modes exist (IO, constraint
  violation, BUSY, locked DB) and the caller might want to log *why*.

Naming: `_opt` for the read-side `Option`-returning helpers (reads well
as "optional value"); `try_` for the write-side `Result`-returning
helpers (reads well as "attempt the write, tell me if it failed").

**Dependencies on PR #2471 review items that matter here:**

- `opt is T` / `opt as T` / `opt ?as T` — nice for writing
  `if (row is Friend)` post-query. Not critical; unwrap-style also works.
- `opt == value` / `value == opt` — lets `if (id == some(1))`-style
  comparisons work without unwrap. Nice-to-have.
- Tutorials and docs for `Option` / `Result` — we'll lean on those
  landing before the dasSQLITE tutorials rely on them.

### Target: `_sql` macro (LINQ-to-SQL level, not Dapper level)

**Decided 2026-04-23.** The north star is a `_sql` call-macro that operates
like `_fold` in `daslib/linq_boost`: takes a daslang expression chain at AST
level, pattern-matches on the known operators (`_where`, `_select_new`,
`_order_by`, `_group_by`, `_count`, `_first`, …), and emits:

1. A **compile-time SQL string** built from the chain.
2. A **parameter vector** extracted from free variables captured in
   predicates (every non-column reference in `u.age >= minAge` becomes a
   bind parameter, not a string-concatenated literal).
3. A **runtime materialization**: `sqlite3_prepare_v2` → bind params →
   `sqlite3_step` loop → row-to-struct mapping generated from the projected
   type's field layout → `array<Row>` (or `Row`, or scalar, depending on
   the terminal operator).

This is the C# LINQ-to-SQL / EF model — full expression-tree translation —
implemented via `qmacro` / AST visitors the same way `_fold` already does
for daslib/linq.

**Schema side (straw-man):**

```das
[sql_table(name="users")]
struct User {
    id   : int64
    name : string
    age  : int
    city : string
}
```

**Use side:**

```das
var inscope db = open_sqlite(":memory:")
db |> create_table(type<User>)
db |> insert(User(name="alice", age=30, city="Berlin"))

let minAge = 18
var adults <- _sql(db |> select_from(type<User>)
    ._where    (_.age >= minAge)
    ._order_by (_.name)
    ._select   ((name=_.name, city=_.city)))
// => array<tuple<name:string;city:string>>
// => runtime SQL: "SELECT name, city FROM users WHERE age >= ? ORDER BY name"
// => binds: [minAge]
```

**Row representation: named tuple.** `select_from(type<T>)` returns
`iterator<T>` (the struct itself — full row). Projection is just the
existing daslib/linq `_select` with a named-tuple literal:
`._select((a=_.a, b=_.b))` yields
`iterator<tuple<a:A; b:B>>`. Field names come from the `(Name=…)`
literal's argument names; types are inferred from values. Named tuples are the
universal row type across dasSQLITE — no per-query synthetic structs,
typeinfo-readable, compose with daslib/linq unchanged.

**`select_from` is a regular function** — no ident-capture, no AST
magic. It takes the db + a `type<T>`, returns `iterator<T>`. The only
macro in the chain is `_sql(...)` (whole-chain translator to compile-
time SQL + runtime bind/step/materialize). Predicates in `._where(…)` /
`._order_by(…)` see the full row type `T`; projection happens via
`._select(…)` and decides what comes out. Same three operators mirror
SQL's WHERE / ORDER BY / SELECT.

**Why `_sql` (not Dapper-equivalent runtime helpers):**

- SQL is built at compile time — zero string-building per call.
- Parameters auto-extracted from captured vars — no
  `{{ "minAge" => 18 }}` table to maintain.
- Invalid column name / misspelled field → **compile error**, not a
  runtime "no such column" from SQLite.
- Result-row → struct mapping generated by the macro from the projected
  type — no reflection, no runtime column-name → field-name lookup.
- Composes with existing `_fold`: unsupported operators stay client-side
  over the result `array`.

**Low-level escape hatches remain** (for raw SQL, migrations, DDL).
All typed helpers take the result type as a `type<T>` argument (gen2 rule):

- `db |> exec(sql [, args])` — statement with no result.
- `db |> query_scalar(sql, type<T> [, args])` — one scalar; panics on 0 rows.
- `db |> query_scalar_opt(sql, type<T> [, args])` — scalar or `null` on 0 rows.
- `db |> query_one(sql, type<T> [, args])` — one typed row; panics on 0/≥2 rows.
- `db |> query_one_opt(sql, type<T> [, args])` — one row or `null`.
- `db |> query(sql, type<T> [, args])` — iterator/array of typed rows.

These are what the low-level tutorials port to, *and* what `_sql` itself
expands to internally. Pipeline: `_sql` → one of these → C binding.

**Design challenges to settle as we walk tutorials:**

- **Non-translatable predicates.** EF famously falls back to client-side
  evaluation when a predicate can't be SQL-ified. **Default: `macro_error`
  at compile time.** Fallback to client-side eval opt-in only (user
  writes `_where_client(...)` or the chain has a transition point).
- **Joins.** Need a shape; probably `._join(db.orders, u => u.id,
  o => o.user_id, (u, o) => …)` borrowing directly from daslib/linq's
  join.
- **Aggregates** (`_count`, `_sum`, `_min`, `_max`, `_group_by`) — all
  translatable; mirror daslib/linq terminals.
- **Sub-queries / `IN` / `EXISTS`** — later.
- **Prepared-statement cache** — the SQL string is compile-time-constant
  per call site; macro can emit a call-site-local `static` `sqlite3_stmt*`
  + reset-on-each-use. Big win for hot loops.
- **NULL.** SQL NULL maps to `T?` (optional) on the daslang side. Columns
  declared non-optional must be `NOT NULL` in the emitted schema.

### C# reference points (kept for historical context)

The three C# bands for reference:

- **Raw ADO.NET** — what today's tutorials look like; not our target.
- **Dapper** (micro-ORM) — runtime SQL + runtime column-mapping.
  Shape of our low-level escape hatches (`query_scalar` / `query` etc.).
- **LINQ-to-SQL / EF** — compile-time expression-tree translation.
  Shape of our `_sql` macro and the primary user-facing API.

### 02-insert_data — "create table Cars, insert 8 rows"

**What the user writes today** (36 lines): opens `test.db` with raw
`sqlite3_open` + `addr`, checks rc, builds a multi-statement SQL blob with
`%stringify` containing DROP + CREATE + 8 × INSERT, calls `sqlite3_exec`,
checks rc, frees errmsg, closes DB. Two rc-check/close/return cycles.

**Observations:**

1. **Schema lives in a string literal.** `CREATE TABLE Cars(Id INT, Name
   TEXT, Price INT)` and the 8 INSERT statements are hand-written SQL. If
   the schema changes, every site that inserts/reads must be updated
   manually. EF solves this by declaring tables as classes with
   attributes; we want the same.
2. **8 inserts, 1 `sqlite3_exec`, no transaction.** `sqlite3_exec` will
   implicitly commit each INSERT (autocommit mode is ON by default), so
   this is 8 fsyncs in a tutorial meant to demonstrate "insert some
   rows". The idiomatic API should batch automatically when given an
   array.
3. **Multi-statement SQL via `sqlite3_exec`** is a footgun for params —
   `sqlite3_exec` doesn't support bind params. Any user-supplied value
   has to be interpolated into the string (SQL injection, type errors).
4. **Zero reuse** — nothing stops you from typing `Naem` in both the
   INSERT SQL and the downstream SELECT, and only noticing at runtime.
   Types on the struct would catch this at compile time.

**Decisions locked 2026-04-23:**

- **Schema as struct:** tables are declared as regular daslang `struct`s.
  `[sql_table]` is a **structure macro** applied above the struct; it
  reads field metadata and generates the full per-type API surface at
  compile time.
- **Field metadata syntax: `@name` only.** Per ds2 grammar, field
  annotations use `@sql_primary_key`, `@sql_default(expr)`,
  `@sql_column = "<name>"`, etc. — not `[...]`. Struct/function-level
  still `[...]` (`[sql_table(name="Cars")]`). The shipped form for
  `@sql_column` is the bare-string form (`@sql_column = "type"`),
  matching `@sql_default_fn = "FN"` / `@sql_references = "Parent"` /
  `@sql_on_delete = "cascade"` precedent.
- **What `[sql_table]` generates** (aspirational, grows as tutorials
  demand):
  - `create_table(db, type<T>)` / `drop_table(db, type<T>)` /
    `drop_table_if_exists(db, type<T>)` — DDL from the struct layout.
  - `insert(db : SqlRunner; row : T)` — single-row INSERT.
  - `insert(db : SqlRunner; rows : array<T>)` — batched INSERT inside a
    single transaction (BEGIN / … / COMMIT).
  - Per-type row reader/writer helpers (struct → bind, column → struct).
  - A LINQ-style `each(...)` iterator for fallback queries not routed
    through `_sql`.
  - Whatever else `_sql` terminals need for typed materialization.
- **`insert` is polymorphic** on `T` vs `array<T>`, not split into
  `insert` / `insert_all`. Name is self-documenting either way; fewer
  names is less to remember.
- **Table name explicit, never auto-pluralized.** `[sql_table(name="X")]`
  overrides the struct name when you want a different on-disk table.
  Default: use the struct name verbatim. No EF-style pluralization magic.
- **Foreign keys deferred.** Revisit once join / multi-table tutorials
  come up. Likely shape: `@sql_foreign_key(ref=type<Owner>, field="id")`
  on the referencing field, but the concept isn't pinned down yet.

**What we want 02 to look like** (see mockup
`tutorial-mockup/02-insert_data.das.mockup`):

```das
[sql_table(name="Cars")]
struct Car {
    @sql_primary_key Id : int
    Name  : string
    Price : int
}

[export]
def main() {
    with_sqlite("test.db") <| $(db) {
        db |> drop_table_if_exists(type<Car>)
        db |> create_table(type<Car>)
        db |> insert([
            Car(Id=1, Name="Audi",       Price=52642),
            // …
            Car(Id=8, Name="Volkswagen", Price=21600)
        ])
    }
}
```

~20 lines incl. struct + 8 rows. Zero raw `sqlite3_*`, zero SQL strings,
zero `rc` checks. Schema is the struct; any typo (`Pice` instead of
`Price`) is a compile error.

**Open questions surfaced but not settled by 02:**

- **`@sql_not_null` default polarity.** daslang types are non-nullable by
  default (string is never null), so the natural mapping is `T` →
  `NOT NULL` column, `T?` → nullable column. `@sql_not_null` probably
  isn't needed at all — the type does the job. Confirm at tut 04+ when
  we see SELECT of nullable columns.
- **Auto-incrementing primary key.** If `@sql_primary_key Id : int` and
  the user constructs `Car(Name="x", Price=0)` omitting `Id`, should the
  macro emit `INSERT … DEFAULT VALUES`-style SQL and pull the row-id
  back? Tutorial 03 (last_row_id) will force this decision.
- **Duplicate PK on insert.** `INSERT … VALUES(1, 'Audi', …)` will fail
  on second run of this tutorial because `drop_table_if_exists` fires.
  But in general a PK violation is an expected failure (vs. programmer
  error) — that's `try_insert(...) : bool` or similar. Decide at tut 03
  or wherever conflict shows up.
- **Table migration.** `drop + create` is fine for the tutorial but
  terrible for real data. Out of scope for the rework; flag as future
  work (Alembic-style migrations or EF `EnsureCreated` / `MigrateAsync`
  equivalent — separate module).

### 03-last_row_id — "auto-increment PK, read last rowid"

Same flow as 02 plus one `sqlite3_last_insert_rowid` call. The table uses
`INTEGER PRIMARY KEY` (SQLite's ROWID alias — auto-increment on omitted-Id
inserts). Tutorial inserts 5 `Friends(Name)` rows without Ids, then logs
the last assigned rowid.

**Decisions locked 2026-04-23:**

- **`@sql_primary_key` on `int`/`int64` → `INTEGER PRIMARY KEY`.** No
  separate `@sql_auto_increment` attribute. SQLite's INTEGER PRIMARY KEY
  is already the ROWID alias; any `int`/`int64` PK gets auto-increment
  for free. (If we ever need `AUTOINCREMENT` — SQLite's stricter
  never-reuse-rowid variant — a separate `@sql_strict_autoincrement`
  attribute can be added, but it's almost never wanted.)
- **Construction with PK omitted is legal when PK is auto-increment.**
  `Friend(Name="Tom")` with Id unspecified defaults Id to 0. The
  `[sql_table]`-generated `insert` detects "PK == 0 on an auto-increment
  column" and emits SQL without the Id column (`INSERT INTO Friends(Name)
  VALUES(?)` instead of `INSERT INTO Friends VALUES(?, ?)`). SQLite
  assigns the rowid.
- **`insert` returns `int64` = assigned rowid.**
  - `insert(T) : int64` — the new row's rowid.
  - `insert(array<T>) : int64` — the *last* inserted rowid (consistent
    with `sqlite3_last_insert_rowid` semantics). Returning
    `array<int64>` of every rowid is possible but more expensive for the
    common case of "I just want the last one"; if a caller wants every
    id they can insert in a loop over the single-row form (also inside
    a manual transaction).
- **`db |> last_insert_rowid() : int64`** also available — mirrors the C
  API and gives the "just query whatever the last write was" shape.
  Expands to `sqlite3_last_insert_rowid` directly.
- **Mutation-of-`var`-row form deferred.** EF mutates the entity's Id in
  place. We could generate this too (if user passes `var friend`,
  writeback). Not adding yet — no tutorial demands it; adds a third way
  to get the id when two suffice. Revisit if a later tutorial wants
  "construct, insert, pass to downstream queries" in one object.

**What we want 03 to look like** (see mockup
`tutorial-mockup/03-last_row_id.das.mockup`):

```das
[sql_table(name="Friends")]
struct Friend {
    @sql_primary_key Id : int
    Name : string
}

[export]
def main() {
    with_sqlite(":memory:") <| $(db) {
        db |> create_table(type<Friend>)
        let last_id = db |> insert([
            Friend(Name="Tom"),
            Friend(Name="Rebecca"),
            Friend(Name="Jim"),
            Friend(Name="Roger"),
            Friend(Name="Robert")
        ])
        to_log(LOG_INFO, "The last Id of the inserted row is {last_id}\n")
    }
}
```

**Open questions surfaced by 03:**

- **PK-collision on explicit Id.** If a user *does* supply an Id
  (`Friend(Id=3, Name="Tom")`) that collides, `INSERT` fails with
  `SQLITE_CONSTRAINT`. This is an expected operational failure — should
  return `try_insert(db, row) : int64?` (null on conflict). Mirror
  `insert` with a `try_` variant at the same call site. Settle at the
  first tutorial that actually writes with explicit Ids.
- **`INSERT OR REPLACE` / `ON CONFLICT`.** Useful but adds surface. Add
  later via `[sql_table]` config attribute (`@sql_on_conflict_replace`?)
  or via a distinct `upsert(...)` function. Deferred until a tutorial
  calls for it.

### 04-select_all — "iterate every row of a table"

**What the user writes today**: opens `test.db`, calls `sqlite3_exec`
with `"SELECT * FROM Cars"` and a callback that receives `values` +
`columns` arrays and prints them raw. No typing, no struct mapping —
just parallel string arrays per row.

**Observations:**

1. **Untyped rows.** `values, columns` are string arrays. Number as
   string, blob as string, NULL as empty string — and the user has to
   know the column order. Idiomatic daslang must give typed rows.
2. **Callback-shaped iteration.** `sqlite3_exec` with a block is a
   push model. daslang prefers pull (`for ... in iterator`) because it
   composes with `break` / early return / linq pipelines.
3. **No projection, no filter, no sort.** Tutorial 04 is "read
   everything back out". But the real-world evolution is filter-then-
   sort-then-project — and the API should make that a chain, not a
   rewrite.

**Decisions locked 2026-04-23:**

- **Entry point: `db |> select_from(type<T>) : iterator<T>`** — a
  regular function, single-shape. No ident-capture, no AST magic. In
  compatibility mode it prepares+steps at runtime and yields rows;
  wrapped in `_sql(...)` with a chain of daslib/linq operators, the
  whole chain is pushed down to one SQL statement.
- **Projection goes through existing daslib/linq `_select`.** No
  SQL-specific projection macro. `_select((Name=_.Name,
  Price=_.Price))` yields `iterator<tuple<Name:string; Price:int>>`
  directly — the named-tuple literal carries the field names (see
  Language prerequisites at top of doc). Single-column:
  `._select(_.Name)`. Computed struct: `._select(CarView(Name=_.Name,
  Price=_.Price*2))`. All three already compile through
  `LinqSelect`/`_fold`; `_sql` just adds the recognize-and-emit-SQL
  pattern.
- **Named tuple is the universal row type.** No per-query synthetic
  struct declarations, no Dapper-style reflection. The `(Name=…)`
  literal is an orthogonal language win — useful for any ad-hoc
  row-shape anywhere in daslang.
- **`_where` / `_order_by` see the full source-table type**, not the
  narrowed projection — matches SQL (WHERE/ORDER BY reference any
  column, SELECT decides output). So `select_from(type<Car>)
  ._where(_.Price > 20000) ._select((Name=_.Name))` is valid:
  `_.Price` is visible in the predicate even though Price isn't in
  the output.
- **Compatibility mode is not a perf promise.** Without `_sql`, every
  client-side operator fetches the full table first. That's a
  correctness fallback, not a production path; real use wraps in
  `_sql` to push filters / projection / ordering down.

**What we want 04 to look like** (see mockup
`tutorial-mockup/04-select_all.das.mockup`):

```das
with_sqlite("test.db") <| $(db) {
    for (car in db |> select_from(type<Car>)) {
        print("Id={car.Id} Name={car.Name} Price={car.Price}\n")
    }
    for (row in db |> select_from(type<Car>)
                   ._select((Name=_.Name, Price=_.Price))) {
        print("{row.Name}: {row.Price}\n")
    }
}
```

**Open questions surfaced by 04:**

- **Keyword / invalid-ident column names.** If a column name is a
  daslang keyword (`for`, `if`, `var`), the daslang struct field
  renames via `@sql_column = "for"` — the field carries a legal
  ident. Projection via `(If=_.for_column)` uses the daslang field
  name; DB name is never typed in user code. **SHIPPED** in chunk 9
  round 4: rename flows through DDL, `_sql` predicates / projections /
  `_order_by` / `_group_by`, `column_info(type<T>).name`, INSERT /
  UPDATE / DELETE / upsert SQL, RETURNING clauses, `[sql_index]`, and
  `@sql_references` to a renamed parent PK. `[sql_table]` rejects
  empty values, embedded `"`/`\` (SQL identifier hygiene), and
  cross-field collisions where two fields produce the same SQL
  column name.
- **Column aliasing in projection.** Trivial under the new design:
  `._select((CarName=_.Name, Cost=_.Price))` — the tuple field
  name IS the alias. Removes this as an open issue.
- **Aggregates.** `COUNT(*)` / `SUM(price)` / `GROUP BY` belong as
  chained terminal operators (`._count()`, `._sum(_ .Price)`,
  `._group_by(_.City)`) — already the daslib/linq shape. `_sql` adds
  SQL translation for them. Settle when an aggregate-heavy tutorial
  comes up (real aggregates don't appear in the existing tutorials).

### 05-parametrized — "SELECT … WHERE Id = ? with a bound value"

Tutorial: prepare `SELECT Id, Name FROM Cars WHERE Id = ?`, bind position
1 to `3`, step, read columns 0 and 1, print.

**The LINQ form eats the whole thing.** Under `_sql`, a captured variable
in the `_where` lambda becomes a bind parameter automatically — exactly
EF Core's behavior where `c => c.Id == targetId` translates to
`WHERE Id = @p0` with `targetId`'s current value bound. The user never
types a placeholder character.

```das
let target = 3
let car = _sql(db |> select_from(type<Car>)
                 ._where(_.Id == target)
                 ._first())
```

No new design work needed on the LINQ side — this is exactly the
`select_from + _where + _first` pattern 04 already committed to, with
the `_sql` macro's expression-walker classifying `_.Id` as column and
`target` as bind param. Same machinery handles any `_where` predicate
with captured vars.

**Decisions locked 2026-04-23 (raw-SQL escape-hatch parameter syntax):**

For queries that don't round-trip through `_sql` (migrations, ad-hoc SQL,
queries where `_sql` gives up on translating), the `exec` / `query_*` /
`query_*_opt` family takes parameters as a **trailing argument**:

| SQLite placeholder style | daslang argument shape |
|---|---|
| Positional `?` / `?N` | Variadic trailing args: `db \|> query_one(sql, type<T>, 3, "alice", 42.0)` |
| Named `:name` / `@name` / `$name` | Named-tuple literal: `db \|> query_one(sql, type<T>, (id=3, name="alice"))` |

Why this pair:
- **Variadic positional** is cleanest for the common `?`-placeholder case
  and handles mixed types trivially (no `array<any>` boxing).
- **Named tuple** (enabled by the language prerequisite) matches SQLite's
  named placeholders 1:1 with zero new machinery. Field names in the
  tuple literal become placeholder names; values bind by index via
  `sqlite3_bind_parameter_index`.
- Overload selection on trailing-arg type (values… vs. one named-tuple
  literal `(Name=…, …)`) disambiguates without a separate function name.

**What we want 05 to look like** (see mockup
`tutorial-mockup/05-parametrized.das.mockup`):

```das
let target = 3
with_sqlite("test.db") <| $(db) {
    let car = _sql(db |> select_from(type<Car>)
                     ._where(_.Id == target)
                     ._first())
    print("ID = {car.Id}, name = {car.Name}\n")
}
```

**Observations / carryovers:**

- **Tutorial 05's entire purpose (parametrization) is invisible in the
  LINQ form.** That's the design win — explicit `?` + explicit bind is
  raw-SQL machinery, not a conceptual primitive. Parameters exist only
  because SQL itself distinguishes literal from bound. The LINQ form
  erases the distinction.
- **Raw-SQL form still gets a tutorial mention.** Users porting from C
  SQLite or writing migration scripts need the placeholder shape; the
  mockup shows both `?` variadic and `:name` named-tuple forms beside
  the LINQ one.
- **Closes the open question** flagged in `_error_handling.das`
  ("Parameter-binding syntax TBD"). Updated that file's header too.

### 06-named_placeholders — "WHERE Id = @id with a bound value"

**Collapsed into 05. No mockup file.**

Tutorial 06 is tutorial 05 with `@id` instead of `?`. SQLite accepts three
interchangeable named-placeholder sigils (`:name`, `@name`, `$name`); the
raw-SQL tutorial swaps `?` for `@id` and `sqlite3_bind_int(stmt, 1, v)` for
`sqlite3_bind_parameter_index(stmt, "@id")` + `sqlite3_bind_int(stmt, idx, v)`.
Nothing else differs.

**Under `_sql`:** the LINQ form is byte-for-byte tutorial 05 —
`_sql(db |> select_from(type<Car>)._where(_.Id == target)._first())`.
Placeholder style is a codegen detail the user never sees. EF Core has no
user-facing knob for this either; the provider picks `@p0` / `?` / `$1`
based on the backend, LINQ translation is identical.

**Raw-SQL equivalent:** `(id=lookupId)` works against
`"... WHERE Id = :id"`, `"... WHERE Id = @id"`, or `"... WHERE Id = $id"`
identically — `sqlite3_bind_parameter_index` accepts the full key (sigil
included), matcher logic is one line of macro, no dialect branch.

**No tutorial mockup for 06.** Mockup `05-parametrized.das` already covers
the conceptual surface completely.

### 07-insert_image + 08-read_image — BLOB column round-trip

**Collapsed together. No mockup files.**

Tutorials 07 and 08 are the first-ever exercise of the type-mapping
table (daslang type ↔ SQL type). 07 writes an `array<uint8>` into a
`Data blob` column, 08 reads it back. The user-level code is identical
to 02/03 (insert) and 04/05 (select) — `[sql_table]` + `insert(...)` for
the write, `_sql(...)` + `select_from(...)._where(...)._first()` for the
read. The only new content is one row in the provider-side type table.

**Write side (what 07 reduces to):**

```das
[sql_table(name="Images")]
struct Image {
    @sql_primary_key Id : int
    Data : array<uint8>
}

// ... fopen/fmap image_data as array<uint8> ...
with_sqlite("test.db") <| $(db) {
    db |> drop_table_if_exists(type<Image>)
    db |> create_table(type<Image>)
    db |> insert(Image(Id=1, Data <- image_data))
}
```

**Read side (what 08 reduces to):**

```das
with_sqlite("test.db") <| $(db) {
    let image = _sql(db |> select_from(type<Image>)
                       ._where(_.Id == 1)
                       ._first())
    fopen("test.png", "wb") $(f) {
        assert(f != null, "can't open test.png")
        f |> _builtin_write(image.Data)
    }
}
```

**EF Core equivalent (for reference):**

```csharp
public class Image { public int Id { get; set; } public byte[] Data { get; set; } }
db.Images.Add(new Image { Id = 1, Data = imageBytes }); db.SaveChanges();
var img = db.Images.First(i => i.Id == 1);
File.WriteAllBytes("test.png", img.Data);
```

Same shape — `byte[]` property, add, save, query, serve bytes. EF handles
BLOB mapping internally. No blob-specific user-facing API surface.

**Locked 2026-04-23 (provider-side only — no user-visible API change):**

- **Type-table entry for BLOB:** `array<uint8>` ↔ `BLOB`.
  - **DDL emission:** `array<uint8>` field → `BLOB [NOT NULL]` column.
    Non-nullable field → `NOT NULL` (consistent with the string/int rule).
  - **Bind:** `sqlite3_bind_blob(stmt, idx, arr.data, arr.length,
    SQLITE_TRANSIENT)`. `SQLITE_TRANSIENT` tells SQLite to copy the
    bytes into its own buffer immediately — the daslang array's lifetime
    after the bind call doesn't matter, the `_sql` / `insert` machinery
    doesn't have to hold a reference through `step`. Small extra copy,
    big correctness win.
  - **Column read:** `let n = sqlite3_column_bytes(stmt, col)` → allocate
    fresh `array<uint8>` of size `n` on daslang's heap → `memcpy` from
    `sqlite3_column_blob(stmt, col)` → assign to struct field. Must copy
    — SQLite's blob pointer is only valid until the next `step`/`reset`.
  - **Existing plumbing:** `modules/dasSQLITE/daslib/sqlite_boost.das`
    already has a `sqlite3_bind_blob` overload accepting any array;
    that's the underlying primitive. The new work is routing
    [sql_table]'s generated bind code to it.

**Non-user-visible but worth flagging:**

- **Nullable blob columns.** If a future schema wants `Data : array<uint8>?`-
  equivalent (SQL NULL), the right daslang shape is
  `Data : Option<array<uint8>>` (same `Option<T>` pattern used for
  nullable scalar reads). `array<T>?` (nullable pointer) is the wrong
  shape for the same reason `int?` is wrong for nullable ints — no
  natural value-level null, forces pointer semantics where none was
  asked for. 07/08 don't exercise this; pin down when a tutorial /
  real use does.
- **Memory spike on large blobs.** Reading a 10 MB blob means a 10 MB
  allocation on daslang's heap, copied from SQLite's buffer. Fine for
  small blobs (icons, saves). For large assets, see future work below.

**Future work (not in this rework, but design must not preclude):**

1. **Chunked / streaming blob I/O.** SQLite offers
   `sqlite3_blob_open` / `sqlite3_blob_read` / `sqlite3_blob_write` for
   incremental access without materializing the whole blob. Likely shape:
   `db |> open_blob(type<Image>, rowid, "Data") <| $(blob) { ... }` with
   `blob |> read(offset, buf)` / `blob |> write(offset, buf)` /
   `blob |> length()`. EF Core doesn't expose this; game-asset users
   probably want it.
2. **Zero-copy column peek.** A `peek_blob` borrow API that hands the
   caller a `#` ref into SQLite's internal buffer — valid for the
   duration of a callback, not allowed to escape. Saves the copy for
   read-only workloads. Precedent: daslang's `peek_data(str) $(arr) { ... }`
   for strings (see CLAUDE.md § "String access functions").
3. **`sqlite3_bind_zeroblob(stmt, idx, n)`.** Reserves `n` zero bytes
   for a later `blob_write`. Useful paired with (1) for pre-allocating
   a row before filling it incrementally.

All three ride on top of the type-mapping; none changes the
`array<uint8>` ↔ `BLOB` user-facing story.

**Observations / carryovers:**

- **`drop + create` pattern** (current tutorial) is terrible for real
  data — flagged already at tut 02 under "Table migration" future work.
  07's use of multi-statement SQL via `sqlite3_exec` collapses into
  two `drop_table_if_exists` + `create_table` calls — consistent with
  02.
- **`sqlite3_bind_blob` array overload** in the existing
  `sqlite_boost.das` is keep-as-is infrastructure. Not shipped API that
  rotates out; the [sql_table] macro uses it under the hood.

### 09-column_names — schema introspection

**No mockup file.** Tutorial 09 runs `PRAGMA table_info(Cars)` with the
callback form of `sqlite3_exec` and prints per-column `(cid, name, type,
notnull, dflt_value, pk)` — i.e. "at runtime, ask the DB what columns
this table has."

**EF Core's answer: you mostly don't.** Three bands:

1. **Compile-time (common case).** Schema IS the type. `Car` class +
   `[Column]`/`[Key]` attributes ARE the column list. EF also exposes a
   `context.Model.FindEntityType(typeof(Car)).GetProperties()` API that
   walks the static model with zero DB round-trips.
2. **Design-time scaffolding** (`dotnet ef dbcontext scaffold`). Reads
   `sqlite_master` / `PRAGMA table_info` to generate model classes from
   an existing DB. Developer tool, not runtime code.
3. **Raw ADO.NET fallback** — `SqliteConnection.GetSchema("Columns")`
   for genuinely-dynamic runtime introspection of an unknown DB. Rarely
   used in EF codebases.

**Our analog — locked 2026-04-23:**

**Band 1: `column_info(type<T>) : array<ColumnInfo>`** — compile-time
macro over a `[sql_table]` struct, emitted as a runtime array whose
contents are fixed at compile time. EF's `context.Model` equivalent.

```das
enum SqlType {
    Integer      // -> INTEGER (SQLite) / INT (MySQL, MSSQL) / INT4 (PG)
    Real         // -> REAL    / DOUBLE / FLOAT / FLOAT8
    Text         // -> TEXT    / VARCHAR / NVARCHAR / TEXT
    Blob         // -> BLOB    / BLOB    / VARBINARY / BYTEA
    Null
}

struct ColumnInfo {
    name         : string
    data_type    : SqlType       // abstract; provider renders to dialect
    is_pk        : bool
    is_nullable  : bool
    default_expr : string        // "" if none
}
```

- **`SqlType` is abstract** (lives in `daslib/sql`). Keeps
  `ColumnInfo` cross-provider — PG, MySQL, MSSQL emit different strings
  from the same `SqlType.Integer`.
- **Per-provider helper** renders the dialect-specific name:
  `def sqlite_sql_type(t : SqlType) : string` lives in
  `sqlite/sqlite_boost`; future `postgres_sql_type(...)` in
  `postgres/postgres_boost`; etc.
- **Usage:**
  ```das
  for (c in column_info(type<Car>)) {
      print("{c.name} : {c |> sqlite_sql_type(c.data_type)}")
      print(c.is_pk ? " PK" : "")
      print(c.is_nullable ? "" : " NOT NULL")
      print("\n")
  }
  ```
- **Why Shape A (runtime array) over Shape B (apply-style
  per-field block):** column metadata is uniform data — all strings +
  bools + an enum — no per-field type variance. `for`-in composes with
  every daslang idiom (linq, filters, etc.) and doesn't require the
  caller to learn an apply-style block convention. Per-field-typed
  iteration is what `apply(instance)` (daslib/apply.das) already
  handles, and `[sql_table]` uses raw AST internally for bind/column
  codegen — neither needs a user-facing typed-iterator API here.

**Band 3: raw PRAGMA via normal `query(...)`** — zero new API. Falls
out of the raw-SQL escape hatch already specified for 05:

```das
struct PragmaColumn {
    cid        : int
    name       : string
    @sql_column = "type"
    col_type   : string                          // "type" is a keyword
    notnull    : int
    dflt_value : string
    pk         : int
}
for (col in db |> query("PRAGMA table_info(Cars)", type<PragmaColumn>)) {
    print("{col.name} : {col.col_type}\n")
}
```

Same machinery as any other typed read. No new surface. Useful for
genuinely-dynamic tooling (an admin UI inspecting an unknown DB).

**Band 2: scaffolding** (reverse-engineer DB → `[sql_table]` struct)
— out of scope. Dev-time tooling, separate module, not user API.

**Observations / carryovers:**

- **`column_info` is the first typeinfo-over-`[sql_table]` user-facing
  API.** `[sql_table]` already has to walk struct fields at compile
  time to emit DDL and bind/column code; exposing the walked result as
  `array<ColumnInfo>` is nearly free. Both DDL generation and
  `column_info` are views over the same compile-time walk.
- **`sqlite3_exec` callback form disappears** from user code. Any
  shape of "step a query and run code per row" is either `for`-in
  against `select_from(...)` / `query(...)` or the
  `_sql(...)` terminal that naturally matches.

### 10-list_tables — list tables in a DB

**No mockup file, no new API.** Tutorial 10 is the Band-3-only cousin of
09, one level up (tables instead of columns) and more SQLite-specific.
It runs `SELECT name FROM sqlite_master WHERE type='table'` via
`sqlite3_exec` + callback. `sqlite_master` / `sqlite_schema` is SQLite's
catalog; every other backend has a different one (PG `pg_class` /
`information_schema.tables`, MySQL `information_schema.tables`, MSSQL
`sys.tables`).

**EF Core's answer:** no runtime user-facing equivalent.
`context.Model.GetEntityTypes()` lists the tables *declared in your
code* at compile time; genuine "what physical tables exist in this DB"
drops to design-time scaffolding or raw ADO.NET. Rare enough that EF
Core doesn't surface it.

**Three bands for 10:**

1. **Compile-time-over-declared.** No analog — we deliberately don't
   have a `DbContext` that registers `[sql_table]` types (ceremony
   without payoff for daslang's style). The honest answer is "look at
   your source code."
2. **Scaffolding.** Out of scope, same as 09.
3. **Raw escape hatch.** Free via existing `query(...)`:

   ```das
   struct MasterRow {
       @sql_column = "type"
       row_type    : string
       name     : string
       tbl_name : string
       rootpage : int
       sql      : string
   }
   for (t in db |> query("SELECT * FROM sqlite_master WHERE type='table'", type<MasterRow>)) {
       print("{t.name} -- {t.sql}\n")
   }
   ```

**Why no `list_tables(runner) : array<string>` abstract helper:**
- Genuinely provider-specific at the SQL level; a uniform-subset
  contract ("which columns do we surface?") locks design decisions
  about views, virtual tables, system tables, schemas, partitions,
  temp tables forever — with little payoff.
- Users who runtime-list tables are almost always writing admin /
  migration tooling — they're already provider-aware, so
  `query(provider_specific_sql, type<T>)` is more honest than hiding
  the catalog name behind a pretend-portable helper.
- `daslib/sql`'s abstract surface stays focused on the
  LINQ-translation path, not a grab-bag of schema-ops helpers.

**Decision 2026-04-23:** ship nothing new. 10 is fully covered by the
raw-SQL escape hatch already specified at 05, applied to the SQLite
catalog table. Contrast with 09, where `column_info(type<T>)` was a
real win because it was a view over a compile-time walk already done
by `[sql_table]` — no equivalent exists for "runtime catalog listing."

### 12 + 13 + 14 — transactions (combined)

Tutorials 12, 13, and 14 are one conceptual unit on SQLite transaction
semantics. Addressed together:

- **12** — `sqlite3_get_autocommit(db)` → bool. "Am I currently in a
  transaction?" Default after open: `true` (not in a txn; autocommit
  on).
- **13** — runs DROP + CREATE + 4 INSERTs via `sqlite3_exec` with **no**
  `BEGIN`/`COMMIT`. Each statement auto-commits individually. Tutorial
  name "auto_commit" = "observe the default behavior."
- **14** — same statements wrapped in explicit `BEGIN TRANSACTION` /
  `COMMIT`. All-or-nothing.

**EF Core equivalents:**

```csharp
// 12's question:
bool inTxn = context.Database.CurrentTransaction != null;

// 14's explicit txn (classic EF pattern):
using var tx = context.Database.BeginTransaction();
// work
tx.Commit();   // or tx.Rollback()
// Dispose() rolls back if neither was called.
```

13's implicit auto-commit has no explicit EF user-facing equivalent — EF
users either `Add/Update/Remove` + `SaveChanges()` (wraps in an implicit
transaction automatically) or start an explicit one.

**Decisions locked 2026-04-23:**

- **Canonical form is a `with_*` block.** `with_transaction(db) <| $() { … }`
  emits `BEGIN` on entry, `COMMIT` on normal exit, and `ROLLBACK` on
  panic / early-return via scope-exit cleanup. Any failure inside the
  block rolls back the whole thing and re-raises the panic. Matches
  daslang's `with_*` idiom (same family as `with_sqlite`).
- **Two overloads, not one optional-parameter function.**
  - `with_transaction(db : SqlRunner, blk : block<():void>)` —
    abstract, lives in `daslib/sql`.
  - `with_transaction(db : SqlRunner, mode : SqliteTxnMode, blk :
    block<():void>)` — SQLite-specific, lives in `sqlite/sqlite_boost`.

  Rationale: the block must be the *last* argument (daslang's
  trailing-block convention), and piping inserts `db` as the *first*
  argument. So `db |> with_transaction(Mode.X) <| blk` expands to
  `with_transaction(db, mode, blk)` — block is arg 3. The no-arg form
  expands to `with_transaction(db, blk)` — block is arg 2. A single
  signature `with_transaction(db, mode=Default, blk)` with optional
  middle-position `mode` would require positional skipping (caller
  supplies arg 1 and arg 3 but not arg 2), which daslang's overload
  resolution doesn't do. Two distinct overloads resolve cleanly by
  arity.

  Same shape for the non-panic variant: `try_transaction` has both
  2-arg (abstract) and 3-arg (SQLite-specific) overloads.

- **`SqliteTxnMode` enum is provider-specific.** SQLite's BEGIN
  lock-acquisition modes don't fit onto the cross-provider
  isolation-level dimension:
  ```das
  enum SqliteTxnMode {
      Deferred    // default; lock deferred to first write
      Immediate   // RESERVED lock at BEGIN; blocks other writers
      Exclusive   // EXCLUSIVE lock; blocks all readers too
  }
  ```
  Lives in `sqlite/sqlite_boost` alongside the 3-arg overload. Other
  providers (PG, MySQL, MSSQL) will ship their own enums for *their*
  BEGIN modifiers (ISOLATION LEVEL / READ ONLY / DEFERRABLE / etc.) —
  deliberately *not* pre-pollinated into the abstract layer, because
  guessing the cross-provider contract before having the second
  provider in hand is exactly the kind of mistake the forward-looking
  section warns against.

- **Nesting → automatic SAVEPOINT.** `with_transaction` inside an
  already-open transaction emits `SAVEPOINT sp_N` /
  `RELEASE sp_N` / `ROLLBACK TO sp_N` rather than a nested BEGIN
  (which SQLite doesn't support). Matches EF's nested
  `BeginTransaction` behavior — keeps the user API uniform; the
  machinery picks BEGIN vs SAVEPOINT based on ambient state.

- **`try_transaction` returns `Result<void, string>`** — non-panic
  variant, for retry-on-SQLITE_BUSY and similar recoverable flows.
  Pending the `Result<void, E>` shape settling in PR #2471
  (see `_error_handling.das` header), mirrors the other `try_` family
  members.

- **`db |> in_transaction() : bool`** — one-liner wrapping
  `!sqlite3_get_autocommit(db)`. Ships. Marginally useful for library
  code that wants "participate in ambient txn if one exists, else
  start one." EF equivalent:
  `context.Database.CurrentTransaction != null`. Trivially cheap to
  implement; trivially removable if we decide it's noise. Lives under
  `sqlite/sqlite_boost` (thin wrapper over the SQLite C API — an
  abstract-layer `in_transaction()` could be added later if a second
  provider has a useful equivalent).

- **13 needs no special API.** Running a series of statements without
  wrapping IS the default behavior — each auto-commits individually.
  Users who want atomicity write `with_transaction` or pass an array
  to `insert` (which auto-wraps in a single transaction — tut 02
  decision). No tutorial-mockup file for 13.

- **12 needs no mockup file either.** `db |> in_transaction() : bool`
  is a one-liner; the 14 mockup demonstrates it at the bottom for
  completeness. No separate mockup.

**Mockup:** `tutorial-mockup/14-transaction.das.mockup` — demonstrates the
canonical `with_transaction`, the SQLite-specific 3-arg overload, the
`try_transaction` non-panic variant in both arities, and the
`in_transaction()` status helper.

**Observations / carryovers:**

- **Scope-exit cleanup mechanism** (`finally` vs `inscope` `[finalize]`)
  — must run on panic as well as normal exit for ROLLBACK to fire
  correctly. Daslang's `finally` does run on panic; verify at impl
  time that the specific mechanism chosen preserves this. If panic
  turns out not to run `finally` for some reason, the fallback is
  internal `try/recover` — which is allowed inside `with_transaction`
  (not user-facing) to implement the rollback guarantee.
- **Execution strategies / retry loops.** EF Core has
  `CreateExecutionStrategy()` for automatic retry-on-transient-failure
  (useful under cloud DB load). Out of scope for this rework; the
  `try_transaction` + user-level retry loop covers the common case.
  Flag as a future-work idea once we see a second provider (PG, MySQL)
  where transient failures are more material.

### 19-group_by — "aggregate per bucket" (decided 2026-04-24)

**Verdict:** bucket B. **No `daslib/linq.das` edits needed** — the backing
`group_by(source, key, element_selector, result_selector)` at
`linq.das:1575-1610` already handles tuple keys, element selection, and
per-group iterator access. All the work is `daslib/linq_boost` macro-side.
Full prerequisite list in § "SQL read-side prerequisites" above.

**Two shapes ship** (reflected in mockup
`tutorial-mockup/19-group_by.das.mockup`):

- `_group_by(key)` — flat SQL shape. 1-arg key selector. **Fuses with the
  next `_select`** — unlike every other LinqCall entry, the macro has to
  treat `_group_by` + `_select` as one compound stage so that aggregate
  primitives in the projection (`_count()`, `_avg(_.X)`, `_sum(_.X)`,
  `_min(_.X)`, `_max(_.X)`) can be recognized as group-scope rather than
  row-scope. Lowers to linq.das's 3-arg `group_by(src, key, elem,
  result)` by synthesizing the `element_selector` (identity) and
  `result_selector` (tuple builder with aggregate calls) from the
  downstream `_select` body. Under `_sql` emits `GROUP BY` + aggregate
  columns in the SELECT clause.

- `_group_by_lazy(key)` — IGrouping shape. 1-arg key. **No fusion** —
  yields `tuple<Key; iterator<T>>` per group, leaving the per-bucket
  processing to the caller. For use cases that don't map to SQL
  (top-N-per-group, group-scoped filtering, grouped JSON emission).
  **`macro_error` inside `_sql(...)`** — we don't try to translate
  per-group windowing to SQL; that's a future window-function story
  (tut 32+).

**`_having(predicate)`** ships alongside. 1-arg boolean block.
In-memory lowers to `where` on the grouped iterator; under `_sql` emits
a HAVING clause (the macro knows it came from `_having`, not `_where`,
and routes accordingly). Captured-var-to-bind-param rewriter is
shared with `_where` — no new plumbing.

**Pre- vs. post-aggregate filtering is explicit in the chain position:**
`_where` placed before `_group_by` emits WHERE; `_having` placed after
emits HAVING. The macro does not try to be clever about a single filter
form that auto-splits.

**Multi-key grouping via named-tuple key:** `._group_by((City=_.City,
Year=year(_.SignupDate)))` works natively — linq.das's `group_by` already
accepts any hashable key type, and the named-tuple literal prerequisite
makes the multi-key shape ergonomic. SQL emits `GROUP BY City,
year(SignupDate)`. `year(...)` and similar scalar transforms must be in
the SQL-translatable function table (tut 32).

**Aggregate recognition is macro context-dependent, not a global rule:**
`_count()` / `_avg(_.X)` called at the top of a chain (no preceding
`_group_by`) are tut-18 flat aggregates. Called inside a `_select` tuple
*after* `_group_by`, they're group-scope. The macro's
`_group_by` → `_select` fusion handles the context switch; the tut-18
form is unaffected.

**Placement:** ships in `daslib/linq_boost` (not a new `daslib/sql_boost`
sibling). Rationale: both macros are in-memory useful independent of SQL
— `_group_by` is a shorter-and-clearer form of linq.das's 3-arg
`group_by` call; `_having` is a clarity win over `where` immediately
after `group_by`. `_sql` is one consumer of these, not the only one.

**Open questions resolved:**

- *What does `_` refer to inside `_select` after `_group_by`?* After
  `_group_by`, `_` in the projection resolves to the group key (scalar
  or tuple). Aggregate primitives (`_count()`, `_avg(_.X)`) iterate the
  group members internally. `_.Name` (a non-group, non-aggregate column
  reference) is a **compile-time `macro_error`** under `_sql` — mirrors
  SQL's rule and preserves the daslang ethos of rejecting at compile
  time. Outside `_sql`, the same rule applies because the fused
  `result_selector` synthesized only has access to the key and the
  group iterator — a non-grouped field reference simply has no binding
  to resolve to.
- *LINQ's IGrouping form.* Ship both (`_group_by` flat + `_group_by_lazy`
  IGrouping). Two macros is simpler than one with auto-detection, and
  matches the clear use-case split.
- *`HAVING` with captured vars.* Reuse `_where`'s rewriter — confirmed
  trivial since the AST path is identical; only the emission-site
  keyword differs (`WHERE` vs `HAVING`).

**Mockup:** `tutorial-mockup/19-group_by.das.mockup`. Covers flat SQL,
pre-filter `_where`, `_having`, multi-key grouping, IGrouping shape,
combined reporting query, and `_try_sql` non-panic variant.

### 23-joins — "one-to-many across tables" (decided 2026-04-24)

**Verdict:** bucket B. **First tut that grows `daslib/linq` itself** —
one new fn, 5-10 lines on top of `group_join`. Everything else is
`daslib/linq_boost` macro-only. Full additions recorded in § "SQL
read-side prerequisites" above.

**Two shapes ship** (reflected in mockup
`tutorial-mockup/23-joins.das.mockup`):

- `_join(other, on, into)` — inner join. `other` is any `_sql`-eligible
  chain (another `select_from`, possibly with `_where` etc.). `on` is
  a 2-arg lambda; `into` is a 2-arg projection. Lowers to linq.das's
  existing `join(srca, srcb, keya, keyb, result)` at
  `linq.das:1521-1534`.

- `_left_join(other, on, into)` — left outer join. Same three args;
  `into`'s right-side parameter has type `Option<TB>` (none if no
  match, some(tb) otherwise). Lowers to a new `left_join` fn in
  `linq.das` that unfolds `group_join`'s per-key array into
  `(TA, Option<TB>)` rows.

**Equi-join-only constraint on `on`** (locked): the macro requires
the `on` predicate to be of the shape `(l, r) => l.X == r.Y` — a
single `==` between one left-field reference and one right-field
reference. The macro extracts the two key selectors at compile time
and synthesizes `keya = $(l) => l.X` + `keyb = $(r) => r.Y`.
Anything else (theta joins, `AND`-combined multi-key equality,
function calls inside the comparison) is a **compile-time
`macro_error`** pointing at raw SQL as the escape hatch.

**Why:** the backing `join` / `left_join` fns build a hash table on
the right-side keys for O(n+m). Theta joins force O(n·m) nested-loop
— accepting arbitrary predicates would silently hand users the perf
cliff and also lose SQLite's planner hint for the common equi case.
Multi-key equi-joins (`u.Id == o.UserId && u.Tenant == o.Tenant`)
are legitimate but don't fit the current `keya`/`keyb` shape of
linq.das's `join`; defer as a later extension (would need a
`multi_key_join` fn with tuple keys, or key_selector returning a
tuple). Ship single-key equi first; flag multi-key as follow-up when
a real use case surfaces.

**LEFT JOIN right side is `Option<TB>`, not per-column nullable**
(locked): SQL's strict LEFT JOIN semantics are per-column nullable
on the right — each column individually NULL on no-match. Our
`Option<TB>` is a whole-row optional, which is *more restrictive*
than SQL but matches what users want 99% of the time. Converts
directly to `NULL` for each right-side column under `_sql`; the
fidelity loss only shows up in rare projections that want to
distinguish "no matching row" from "matching row with NULL
columns". Those cases: raw SQL.

**`_select_many(_.Orders, into)` (relationship-aware)** — deferred
to tut 26. Backing `select_many` already exists at
`linq.das:2629+`; the macro + the `_.Orders` nav-property metadata
that makes it resolve are a tut 26 concern. Mockup shows the shape
as a forward-reference.

**Three-table joins** — compose syntactically (`LinqCall(recursive=[1])`
on `join` / `left_join` makes the inner chains chainable), but the
output after join-1 is `tuple<TA;TB>` and join-2's keys must index
into that (`_._0.Id`). Named tuples help. Writes ugly past 2
tables. Documented as "use 2-table joins idiomatically; 3+ either
chain with verbose tuple access, or drop to raw SQL / split the
query". Query-comprehension syntax (LINQ's `from u in users, o in
orders, p in products where … select …`) has no daslang analog
yet — future ergonomics pass, not blocking this rework.

**Not shipped** (locked): `_full_outer_join`, `_right_join`,
`_cross_join`. Rationale under the prerequisites table above.
`_right_join` in particular: trivially `_left_join` with swapped
arguments. `_cross_join` is an accidental-footgun if it's a
macro — user who writes `._join(other, on=…, into=…)` without
realizing they left `on` blank would get cartesian product;
forcing raw SQL for cross join is safer.

**Open questions resolved:**

- *Lambda parameter naming — match across `on` / `into` or
  independent?* Independent. Each lambda binds its own names; the
  macro matches positionally (first param = left, second = right).
  Matches daslang lambda conventions and removes a cross-lambda
  invariant the user has to remember.
- *LEFT JOIN nullable shape.* `Option<TB>` (whole-row). Justified
  above.
- *3-table joins.* Deferred — ergonomics, not a linq gap.
- *INNER / LEFT / RIGHT / FULL OUTER / CROSS.* INNER + LEFT ship;
  rest deferred.

**Mockup:** `tutorial-mockup/23-joins.das.mockup`. Covers inner join,
equi-join constraint diagnostic, `_where` on either side of the
join, LEFT JOIN with `unwrap_or`, LEFT JOIN preserving `Option<T>`
in the output tuple, three-table join (commented-out as
discouraged), relationship-aware navigation (commented-out as
forward-reference to tut 26), and `_try_sql` non-panic variant.

### 24-subqueries — "EXISTS, IN, correlated" (decided 2026-04-24)

**Verdict:** bucket B. **Second tut that grows `daslib/linq` itself**
— one new fn (`none`, mirror of `any`, ~10 lines). Two new
`linq_boost` macros (`_in`, `_not_in`) plus one `AstCallMacro_LinqPred2`
call-macro (`_none`). See § "SQL read-side prerequisites" above for
the full table entries.

**Core constraint driving the design: no leading-`!` AST walkability.**
The `_sql` macro walks the AST of `_where` predicates to classify
references (column / bind / correlated) and recognize sub-chains that
should emit as subqueries. Detecting `!expr` and conditionally
negating the emitted SQL is fragile — any transform between `!` and
the target call (parentheses, local binding, `if` expression)
breaks the match. **Explicit function names are AST-identifiable
with zero ambiguity.** This drives the API shape:

| Positive | Negative | SQL |
|---|---|---|
| `_any(...)` / `_any(pred)` | `_none(...)` / `_none(pred)` | EXISTS / NOT EXISTS |
| `x._in(subquery)` | `x._not_in(subquery)` | IN / NOT IN |

Expect this pattern to repeat — any future SQL primitive that has a
negated form will ship as a matched pair under the same naming
convention (`_not_<positive>` or `_no_<positive>`).

**Three SQL patterns lock:**

- **EXISTS / NOT EXISTS** — `_any` and `_none`. `_any` already exists
  at `linq_boost.das:161-169`. `_none` is a new
  `AstCallMacro_LinqPred2` with the same structure. The backing
  `none` fn is new in `linq.das` (mirror of `any`). Under `_sql`,
  nested `select_from(...)._any(pred)` / `._none(pred)` chains
  inside a `_where` predicate emit `EXISTS` / `NOT EXISTS` with
  correlation handled automatically (see below).

- **IN / NOT IN** — `_in` and `_not_in`. Both are `linq_boost` macros.
  `_in(x, subquery)` sugars over linq.das's existing
  `contains(subquery, x)` at `linq.das:1927-1934` (arg order flipped
  for SQL-native reading "x is in collection"). `_not_in` is a
  distinct macro, not a negation wrapper.

- **Correlated subqueries** — pure `_sql` emitter work, **no linq or
  linq_boost addition needed**. The classification rule:
  - lambda param referenced inside its own body → column ref
    with current table alias
  - outer lambda param referenced inside a nested lambda →
    CORRELATED column ref, qualified with outer table alias
  - free identifier (local `let`, closure var, function arg) →
    BIND parameter

  Resolved at macro expansion by walking lambda-param scopes. Extends
  the `_where` captured-var machinery one nesting level deeper — no
  new infrastructure, same mechanism.

**`none` ships for in-memory users too** (not SQL-only). `none(xs,
pred)` reads clearly in linq pipelines (`none(errors, _.Fatal)` ↔ "no
fatal errors"), better than `!any(xs, pred)`. The SQL translator is
just one consumer. Placement: `daslib/linq` (fn) + `daslib/linq_boost`
(macro), same as `any` / `all`.

**Scope-limited for this rework:**

- `_in(x, literal_array)` — `x._in([1, 2, 3])` emitting `x IN (1,2,3)`
  — would be nice, but the first-class form is `x._in(subquery)`.
  Defer the literal-list form; it can be added later as an overload
  that takes an array value instead of a chain. Until then, literal
  lists go through raw SQL.
- `NOT IN with NULLs` — SQL-semantics gotcha (`x NOT IN (1, NULL)`
  is always NULL, never TRUE). Documented in the tutorial comment;
  not an API issue. User responsibility to filter NULLs from the
  subquery projection when that matters.
- Nested `_sql(...)` inside `_sql(...)` — the outer macro recurses
  and treats any inner `_sql`'s chain argument as a subquery. **No
  separate `_subquery(...)` sentinel needed** — the inner `_sql`
  macro name is the recognition token, and nesting is idiomatic.

**Open questions resolved:**

- *Nested `_sql` vs. dedicated `_subquery` sentinel.* Nested `_sql`
  wins. Simpler; user doesn't learn a second macro name; the outer
  macro special-cases its own name during AST walk.
- *`_in` vs. `contains` — ship both?* Yes. `_in` is SQL-native
  reading; `contains` is LINQ-idiomatic. Both lower to the same
  backing `contains` fn; under `_sql` both emit `IN`. Removing
  `contains` would break existing callers.
- *Correlated-column classification.* Lambda-scope walk at macro
  expansion. Outer-lambda-bound params → correlated column refs
  with outer table alias; free identifiers → bind params. Same
  classifier extends to arbitrary nesting depth.
- *`!_any(...)` vs. `_none(...)` for NOT EXISTS.* Explicit `_none`
  (and parallel `_not_in`). AST walkability requires it.

**Mockup:** `tutorial-mockup/24-subqueries.das.mockup`. Covers EXISTS, NOT
EXISTS, IN, NOT IN (with the NULL gotcha comment), correlated
subquery with outer-column reference, mixed correlated + captured-
local bind parameter, and `_try_sql` non-panic variant.

**Interesting phase-0.3 observation:** tut 24 is the **second tut
that grows `daslib/linq`** (after tut 23's `left_join`). The pattern
across the three bucket-B tuts:

| Tut | linq.das fn adds | linq_boost macro adds | `_sql` emitter work |
|---|---|---|---|
| 19 group_by | 0 | 3 (`_group_by`, `_group_by_lazy`, `_having`) + aggregate context recognition | `_group_by` fusion with `_select` |
| 23 joins | 1 (`left_join`) | 4 (`_join`, `_left_join`, equi-join constraint, `_select_many` deferred) | moderate |
| 24 subqueries | 1 (`none`) | 3 (`_in`, `_not_in`, `_none`) | **heavy** — nested-chain walker, outer-lambda classifier |

The linq.das growth is modest (2 fns total), but tut 24 concentrates
the real weight in the `_sql` emitter itself. This matters for plan
ordering: the emitter is dasSQLITE-phase work (not Phase 0.3), and
its expression walker has to be robust enough for tut 24's
correlated + nested cases before tut 24's SQL tests can pass.

### 25-null_handling — "Option<T>, IS NULL, COALESCE" (decided 2026-04-24)

**Verdict:** first post-bucket-B tut with **zero linq prereqs.** All
NULL-handling work is internal to the `_sql` emitter and to the
`[sql_table]` structure macro's DDL/bind/reader code generation.
`Option<T>` is already a regular container from Phase 0.1 (PR #2471);
dasSQLITE just teaches its macros to recognize existing Option API
calls in SQL contexts.

**Core design decision — nullability is type-driven, not annotation-
driven:**

| Field type | DDL | INSERT/UPDATE bind | SELECT read |
|---|---|---|---|
| `T` (non-optional) | `… NOT NULL` | always bind value | panic if column is NULL (row shape violation) |
| `Option<T>` | `…` (nullable — SQLite's default) | `is_some()` → value, `is_none()` → NULL | `some(v)` or `none()` based on `sqlite3_column_type == SQLITE_NULL` |

The field's daslang type is the **single source of truth** for
column nullability. No `@sql_nullable` annotation ships — would
duplicate the type-level info and risk drift. This mirrors C# 8's
nullable-reference-types approach (type system carries nullability,
not attributes).

**Structural invariant:** `[sql_table]` is a **read-only** structure
macro. It introspects field types to generate DDL / bind / reader
helpers around the struct, but does NOT rewrite fields. A `User`
struct with `Age : Option<int>` is a plain daslang struct usable
wherever any other struct is; nothing SQL-flavored leaks into the
type.

**Consequences:**

- **Non-nullable-in-DB but `Option<T>` in daslang** — wasted wrapper,
  but harmless. Consider flagging with a lint (warning, not error)
  as a future polish. Not worth a macro_error — maybe the user is
  about to make the column nullable and has the struct updated
  first.
- **Nullable-in-DB but `T` in daslang** — **unsound, forbidden by
  construction.** There's no way for the user to declare this
  shape: the struct field is either `T` or `Option<T>`, full stop.
  The DDL emission follows the type. Legacy databases with schemas
  that don't match would surface as read-side panics (column
  returns NULL, daslang struct expects `T`) — acceptable since the
  user's choice of `T` is an explicit "this cannot be NULL" claim.
- **`Option<T>` on a primary key → `macro_error`.** Primary keys
  can't be NULL in SQL. `[sql_table]` rejects the combination at
  compile time with a pointer to drop the `Option` wrapper.

**Translation table** — `_sql` emitter recognition rules for Option
methods in predicates and projections:

| User writes | `_sql` emits |
|---|---|
| `_.Col \|> is_some()` | `Col IS NOT NULL` |
| `_.Col \|> is_none()` | `Col IS NULL` |
| `_.Col \|> unwrap_or(default)` | `COALESCE(Col, default)` / `IFNULL(Col, default)` |
| `_.Col \|> unwrap()` inside `_where` | **macro_error** — panics in-memory on NULL; won't translate. Use `unwrap_or` or guard with `is_some` + combined predicate. |
| Projection tuple field typed `Option<T>` | column passes through nullably — result tuple field is `Option<T>` |

Both positive and negative names (`is_some` / `is_none`) are
explicit — no `!is_some()` pattern detection needed. Convention
from tut 24.

**One open question deliberately deferred:**

- *`_.Col == none()` vs `_.Col |> is_none()`.* The first compiles
  (`Option<T> == Option<T>` is a standard container equality) but
  under `_sql` would translate to `Col = NULL` — which is
  semantically wrong (SQL three-valued logic: `x = NULL` is NULL,
  never TRUE). Two plausible routes:
  - **(a) macro_error on `== none()` in predicates**, with a fixit
    pointing at `|> is_none()`. Explicit-is-better-than-magic.
  - **(b) Auto-translate `_.Col == none()` to `Col IS NULL`.** User-
    friendly; hides the SQL-vs-daslang gotcha.
  - **Lean (a).** Discovery matters more than convenience — forcing
    the explicit spelling teaches the user about three-valued logic
    instead of silently doing the right thing and leaving them
    surprised the first time it matters.
- *`Option<T> == T` overload for SQL-native three-valued
  comparisons.* Out of scope for dasSQLITE; belongs in PR #2471
  follow-up. Until/unless it lands, user writes explicit `unwrap_or`
  + compare. Not blocking tut 25.

**Mockup:** `tutorial-mockup/25-null_handling.das.mockup`. Covers type-
driven DDL emission, INSERT/UPDATE binding of `Option<T>`, reading
nullable columns, IS NULL / IS NOT NULL via `is_some`/`is_none`,
COALESCE via `unwrap_or`, Option<T> carrying through projections,
UPDATE setting a column to NULL (via `none()`), the three-valued-
logic gotcha warning with deferred open question, and `_try_sql`.

**Phase-0.3 impact:** none. Zero linq or linq_boost additions.
Advances dasSQLITE-phase scope (`[sql_table]` DDL generation,
Option-aware bind/reader generation, `_sql` emitter recognition
rules) — all internal macro work, no language or daslib changes.

**Running totals after tut 25:**

| Tut | linq.das | linq_boost | dasSQLITE macros |
|---|---|---|---|
| 19 group_by | 0 | 3 | `_group_by` + `_select` fusion |
| 23 joins | 1 | 4 | moderate emitter work |
| 24 subqueries | 1 | 3 | nested-chain + outer-lambda classifier |
| 25 null_handling | 0 | 0 | type-driven DDL; Option recognition |
| **Total** | **2 fns** | **10 macros** | (emitter complexity accumulates) |

### 26-foreign_keys — "DDL-only FKs; nav properties deliberately skipped" (decided 2026-04-24)

**Verdict:** second consecutive tut with **zero linq prereqs.** All
work is internal to the `[sql_table]` structure macro's DDL emitter
and to `with_sqlite` setting `PRAGMA foreign_keys=ON` on connect.
**Negative delta on prereqs:** drops tut 23's `_select_many` macro
from the backlog — it was a forward-reference to nav-property
metadata, which this tut decides not to ship.

**Core scope decision — (a) only, skip (b):** the API_MISSING
strawman split FK into "(a) DDL-only constraint" vs "(b) nav
properties / `_include` / dot-chain traversal." Lock: ship (a),
skip (b).

**Why skip nav properties:**

1. **Query side already works.** Tut 23's `_join` / `_left_join`
   and tut 24's `_any` / `_in` cover every relational pattern
   you'd express with a nav property. User writes the key
   predicate explicitly. More keystrokes, zero macro magic, zero
   runtime ambiguity.
2. **Violates tut 25's `[sql_table]` read-only invariant.** Nav
   properties would need either synthetic struct fields (breaks
   "struct IS the row") or phantom fields resolved by macro
   (confusing — `u.Orders` means "field" or "query"?). Tut 25 just
   locked the invariant; breaking it one tut later is bad
   sequencing.
3. **No object-graph layer.** EF navs are load-bearing because
   C# apps pass `User` objects around and traverse `.Orders`
   lazily. daslang code writes queries; there is no graph worth
   navigating. Without a graph consumer, nav properties are
   sugar over joins — and the join syntax is already fine.
4. **Massive scope.** `Include`, lazy loading, loaded-vs-not-
   loaded state, change tracking — the single biggest feature in
   EF, and the source of most of its complexity. Cutting it
   cleanly avoids dragging dasSQLITE's scope into ORM territory.
   "If you want ORM ergonomics, write a macro on top of dasSQLITE"
   is a cleaner story than "ORM ergonomics built in, but
   half-working."

**Annotation syntax (what ships):**

```das
@sql_references(type="TargetTypeName",
                on_delete="cascade" | "set_null" | "set_default"
                           | "restrict" | "no_action",
                on_update="...")     // same value set
FieldName : ColumnType
```

`type` takes a **string** (not `type<T>`) because daslang
annotations accept only literal values — no type expressions.
The macro resolves the string at expansion time: finds the
struct with that name, reads its `[sql_table(name="...")]` for
the table name, reads its `@sql_primary_key` field for the
column name. Emits `REFERENCES Users(Id) ON DELETE CASCADE`.

Defaults when `on_delete` / `on_update` omitted: `"no_action"`
(SQL standard — immediate error on orphan).

**Emitted DDL example:**

```sql
CREATE TABLE Orders (
  Id INTEGER PRIMARY KEY,
  UserId INTEGER NOT NULL REFERENCES Users(Id) ON DELETE CASCADE,
  Total REAL NOT NULL
)
```

**`on_delete` / `on_update` value table:**

| String | SQL emitted | Notes |
|---|---|---|
| `"cascade"` | `CASCADE` | Most common — delete-parent = delete-children |
| `"set_null"` | `SET NULL` | **Requires `Option<T>` column type** — `macro_error` otherwise (tut 25 row-shape rule) |
| `"set_default"` | `SET DEFAULT` | Requires `@sql_default(...)` annotation on the same field; wires up when tut 28 lands |
| `"restrict"` | `RESTRICT` | Immediate constraint check; parent delete errors if any child exists |
| `"no_action"` | `NO ACTION` | Deferred check (SQL default) |

Unknown string → `macro_error` at annotation parse time with
the valid list.

**Edge cases:**

- **`on_delete="set_null"` on plain `T` column → `macro_error`.**
  SET NULL would try to write NULL into a NOT NULL column. Fix:
  change the column type to `Option<T>` or pick a different
  action. Same invariant that powers tut 25's `Option<T>`-on-
  primary-key rejection.
- **Self-referential FK.** `@sql_references(type="NodeTree", …)`
  inside `struct NodeTree { … }` resolves to the containing
  type; single `create_table` emits both the table and the
  self-reference. Column is usually `Option<int>` because the
  root has no parent.
- **Ordering constraint on `create_table`.** Parent before
  child; drop in reverse. `drop_table_if_exists` + recreate is
  the safe pattern. No automatic ordering helper — explicit is
  fine.

**`PRAGMA foreign_keys=ON` is default.** `with_sqlite` opens
the connection and sets the pragma on every connect. SQLite
ships it OFF for back-compat with pre-3.6.19 databases; dasSQLITE
turns it on always. Users needing it off can raw-exec the
pragma. Corollary: our FK constraints are enforced end-to-end
(orphan inserts error, CASCADE actually cascades). Documented
in the `with_sqlite` helper's doc-comment.

**Dropped from tut 23 backlog:** `_select_many(nav_key, into)`
macro was flagged as a forward-reference "once tut 26 ships nav
metadata." Since nav metadata is not shipping, the macro loses
its motivation and drops. The backing `select_many` fn stays in
`linq.das` — it's still useful for plain `array<T>` pipelines
that want flat-map over nested collections, just not as a SQL
sugar. Net effect on running totals: `-1` linq_boost macro.

**Deferred / out of scope:**

- Composite foreign keys (multi-column FK referencing a
  composite PK). Shape would be
  `@sql_references(type="X", fields=["A", "B"])` — falls out
  naturally. Not common; add on real demand.
- `DEFERRABLE INITIALLY DEFERRED` constraints. SQLite-only
  feature (most providers ignore it). Useful for bulk imports
  that need transient inconsistency. Add when a use case
  surfaces.
- Future integration with tut 28's `@sql_default` for the
  `"set_default"` action.

**Mockup:** `tutorial-mockup/26-foreign_keys.das.mockup`. Covers annotation
syntax, emitted DDL, CASCADE delete demo, SET NULL / `Option<T>`
pairing, value table, self-referential FK, explicit-join replacement
for nav properties, `try_insert` for FK violations, `PRAGMA`
behavior, and the full deferred list.

**Phase-0.3 impact:** none. Zero additions; one subtraction
(`_select_many` macro drops). Advances dasSQLITE-phase scope
(`[sql_table]` DDL emitter extension for REFERENCES clauses,
`with_sqlite` pragma-on-connect).

**Running totals after tut 26:**

| Tut | linq.das | linq_boost | dasSQLITE macros |
|---|---|---|---|
| 19 group_by | 0 | 3 | `_group_by` + `_select` fusion |
| 23 joins | 1 | 3 (−1, drops `_select_many`) | moderate emitter work |
| 24 subqueries | 1 | 3 | nested-chain + outer-lambda classifier |
| 25 null_handling | 0 | 0 | type-driven DDL; Option recognition |
| 26 foreign_keys | 0 | 0 | `@sql_references` DDL; pragma-on-connect |
| **Total** | **2 fns** | **9 macros** | (emitter complexity accumulates) |

### 27-indexes — "CREATE INDEX, stackable annotation" (decided 2026-04-24)

**Verdict:** third consecutive tut with **zero linq prereqs.**
Pure `[sql_table]` structure-macro extension — read stackable
`[sql_index(...)]` annotations, emit one `CREATE [UNIQUE]
INDEX` per annotation after `CREATE TABLE`, inside the same
transaction.

**Annotation shape (locked):**

```das
[sql_index(fields="ColName",
           unique=true | false,     // default false
           name="explicit_name")]   // default auto-generated
```

Composite index: `fields=["ColA", "ColB"]`. Stackable —
multiple `[sql_index]` attributes on one struct each emit a
separate CREATE INDEX.

**Six small locks:**

1. **Index naming** — auto-generate `idx_<table>_<col1>[_<col2>…]`
   when `name=` omitted; accept explicit `name="..."` override
   for DB-migration compatibility with an existing hand-named
   schema. Auto-gen keeps DDL deterministic for schema-diffing.
2. **`unique=true` on a single column emits external `CREATE
   UNIQUE INDEX`**, not inline `UNIQUE` in the column
   definition. Two spellings for the same constraint = drift
   risk; one uniform spelling.
3. **Field validation.** `fields=["NonExistent"]` → `macro_error`
   at `[sql_table]` expansion with a list of valid field names.
   Falls out of the field-introspection the macro already does
   for column-name emission.
4. **Partial indexes (`WHERE …`) — skip.** SQLite-specific, EF
   doesn't expose them portably, rare. Raw `exec("CREATE INDEX
   … WHERE …")` for users who need them.
5. **Expression indexes (`lower(Email)`) — skip.** Would require
   an expression parser inside the annotation — annotations only
   accept literal values. Same raw-SQL escape hatch.
6. **Emission order.** `CREATE TABLE` first, then one `CREATE
   INDEX` per annotation, all in one transaction. SQLite only
   allows inline `UNIQUE` at the column level, which we reject
   per lock #2 — everything goes through separate index
   statements.

**Consequence for query side: transparent.** User's `_sql(...)`
chains don't change; SQLite's planner picks indexes
automatically. Indexes are a performance concern, never a query-
shape or semantics concern. No new query surface in this tut.

**Unique-constraint violations** surface as SQLite's `UNIQUE
constraint failed: Users.Email` runtime error. `try_insert`
captures it as `Result<int, string>` via the existing tut-15
try-variant machinery.

**Deferred / out of scope:**

- Partial indexes (SQLite `WHERE` clause on CREATE INDEX).
- Expression indexes (`ON Users(lower(Email))`).
- Covering indexes / `INCLUDE` columns (PostgreSQL feature;
  SQLite doesn't support it).
- DROP INDEX — belongs to schema-migration tut 30.

**Mockup:** `tutorial-mockup/27-indexes.das.mockup`. Covers composite,
single-column, UNIQUE, explicit-name, transparent query side,
`try_insert` on duplicate, and the full deferred list.

**Phase-0.3 impact:** none. Zero additions. Advances dasSQLITE-
phase scope only (`[sql_table]` DDL emitter grows a second
stackable-annotation reader — same shape as tut 26's field-
level annotation reader, just at struct level).

**Running totals after tut 27:**

| Tut | linq.das | linq_boost | dasSQLITE macros |
|---|---|---|---|
| 19 group_by | 0 | 3 | `_group_by` + `_select` fusion |
| 23 joins | 1 | 3 (−1, drops `_select_many`) | moderate emitter work |
| 24 subqueries | 1 | 3 | nested-chain + outer-lambda classifier |
| 25 null_handling | 0 | 0 | type-driven DDL; Option recognition |
| 26 foreign_keys | 0 | 0 | `@sql_references` DDL; pragma-on-connect |
| 27 indexes | 0 | 0 | `[sql_index]` stackable DDL |
| **Total** | **2 fns** | **9 macros** | (emitter complexity accumulates) |

### 28-defaults_computed — "`@sql_default` + `@sql_computed`" (decided 2026-04-24)

**Verdict:** fourth consecutive tut with **zero linq prereqs.**
Pure `[sql_table]` work — extends the DDL emitter (DEFAULT /
GENERATED ALWAYS clauses) and the bind-code generator (exclude
computed fields from INSERT/UPDATE). No query-side changes.

**Annotation shapes (locked):**

```das
@sql_default(value=<literal>)                 // daslang literal
@sql_default(sql_fn="CURRENT_TIMESTAMP")      // whitelisted built-in
@sql_computed(sql="<expr>", stored=false)     // generated column
```

**Five locks + one consistency check:**

1. **`@sql_default(value=...)` takes daslang LITERALS only.** Ints,
   floats, strings, booleans. No expressions — annotation parser
   doesn't evaluate.
2. **`@sql_default(sql_fn="...")` is a whitelist.** Allowed
   strings: `"CURRENT_TIMESTAMP"`, `"CURRENT_DATE"`,
   `"CURRENT_TIME"`. Unknown string → `macro_error` at
   expansion with valid list. No user-defined functions (they
   can't reference the struct anyway).
3. **`@sql_computed(sql="...")` is a string passed verbatim to
   SQLite.** Expression validation is runtime-deferred to
   SQLite's CREATE TABLE parser — typos surface as
   `create_table` failures with the SQLite error (e.g. "no
   such column: Qnty"). `try_create_table` captures as
   `Result<void, string>`.
4. **STORED vs VIRTUAL.** Default `stored=false` (matches
   SQLite's default: VIRTUAL, computed on read). `stored=true`
   materializes on write for expensive expressions.
5. **Computed columns excluded from INSERT/UPDATE bind code.**
   Macro walks struct fields for binding; fields tagged
   `@sql_computed` are skipped entirely. Explicit
   `_update(..., set_expr=(Total=x))` on a computed
   column → `macro_error` with a pointer to update source
   columns instead. Symmetric on the struct-value update path
   (silently excluded — no runtime footgun).

**Consistency check across tuts:** `on_delete="set_default"` (tut
26) now gated on `@sql_default` presence. `[sql_table]` rejects
`@sql_references(..., on_delete="set_default")` on a column
without `@sql_default` — same error style as tut 25's
`Option<T>`+PK rejection.

Additional forbidden combinations rejected at expansion:

- `@sql_computed` + `@sql_primary_key` → `macro_error` (SQLite
  restriction — generated columns can't be PK).
- `@sql_computed` + `@sql_default` → `macro_error` (SQLite
  rejects; generated columns define their own value).

**Expression validation tradeoff (locked string form, flagged
follow-up):**

The string-form `@sql_computed(sql="Price * Qty")` ships with
**runtime validation** — SQLite parses the expression at CREATE
TABLE and errors on bad column refs, bad function names, bad
types. Acceptable because errors surface immediately on first
`create_table` call (not silent), and the SQLite error messages
are actionable.

The **principled alternative** (deferred) is a
`@@func_name` reference form:

```das
def compute_total(r : Order) : float {
    return r.Price * float(r.Qty)
}

@sql_computed(func=@@compute_total)
Total : float
```

Three wins over the string form:

1. **Daslang compile-time validation** — `r.Price` resolves
   against the struct, types compose under `*`, return type
   must match column.
2. **In-memory callable** — same function works for local
   compute, bulk-insert pre-calculation, unit tests. One
   spelling, two consumers.
3. **`_sql` translator reuse** — walks the function body and
   emits SQL. Same infrastructure being built for tuts
   19/23/24 handles annotation-position expressions for
   defaults, computed columns, future expression indexes
   (tut 27 deferral), etc.

**Deferred until a user files an issue or asks in person.**
Annotation-parser extension to accept `@@func` plus
`_sql`-translator-applied-to-function-bodies is real work.
String form is a fine MVP; SQLite errors are good enough.

**Other deferred / out of scope:**

- Default-firing from the macro-generated INSERT path.
  Considered skip-zero-value detection and explicit
  `insert_with_defaults(fields=...)` variant — both rejected
  (fragile / verbose). Raw SQL is the escape.
- SQLite 3.31+ requirement for generated columns (2019-10-10).
  Documented as minimum version; not back-porting via triggers.
- Changing VIRTUAL/STORED or the expression itself requires
  drop + recreate — schema-migration (tut 30) territory.

**Mockup:** `tutorial-mockup/28-defaults_computed.das.mockup`. Covers
both annotations in action, emitted DDL, macro-INSERT-doesn't-
fire-defaults quirk, raw-SQL workaround, SELECT reading computed
columns, UPDATE-of-computed rejected at compile time, STORED vs
VIRTUAL, `on_delete="set_default"` consistency check, full
deferred list including `@@func` future form.

**Phase-0.3 impact:** none. Zero additions. Advances dasSQLITE-
phase scope (`[sql_table]` DDL emitter learns DEFAULT /
GENERATED clauses; bind-code generator learns to skip
`@sql_computed` fields).

**Running totals after tut 28:**

| Tut | linq.das | linq_boost | dasSQLITE macros |
|---|---|---|---|
| 19 group_by | 0 | 3 | `_group_by` + `_select` fusion |
| 23 joins | 1 | 3 (−1, drops `_select_many`) | moderate emitter work |
| 24 subqueries | 1 | 3 | nested-chain + outer-lambda classifier |
| 25 null_handling | 0 | 0 | type-driven DDL; Option recognition |
| 26 foreign_keys | 0 | 0 | `@sql_references` DDL; pragma-on-connect |
| 27 indexes | 0 | 0 | `[sql_index]` stackable DDL |
| 28 defaults_computed | 0 | 0 | `@sql_default` / `@sql_computed` DDL + bind-exclusion |
| **Total** | **2 fns** | **9 macros** | (emitter complexity accumulates) |

### 29-custom_types — "name-based bind/extract pair; no registration" (decided 2026-04-24)

**Verdict:** fifth consecutive tut with **zero linq prereqs.**
Pure dasSQLITE work: adopt a convention-by-name type-adapter
mechanism (no registration API, no runtime lookup table). Stdlib
types ship with trivial passthrough overloads inside the module;
users add overloads for domain types (`DateTime`, `Guid`,
JSON-backed structs) in their own modules and `require` them.

**Core shape (locked):** every SQL-addressable type `T` has a
bidirectional pair of named functions.

```das
def sql_bind    (v : T) : P                     // T → primitive
def sql_extract (v : P; type<T>) : T            // primitive → T
```

`P` is the **primitive storage type** — one of four. SQLite's
four storage classes map to four daslang primitives:

| Return type of `sql_bind` | SQLite column | sqlite3 API |
|---|---|---|
| `int64` | INTEGER | `sqlite3_bind_int64` / `sqlite3_column_int64` |
| `double` | REAL | `sqlite3_bind_double` / `sqlite3_column_double` |
| `string` | TEXT | `sqlite3_bind_text` / `sqlite3_column_text` |
| `array<uint8>` | BLOB | `sqlite3_bind_blob` / `sqlite3_column_blob` |

(NULL is handled orthogonally by tut 25's `Option<T>` — not a
fifth storage type at this layer.)

**Example — DateTime as integer (unix seconds):**

```das
def sql_bind    (dt : DateTime) : int64 { return dt.unix_seconds }
def sql_extract (v  : int64; type<DateTime>) : DateTime {
    return DateTime(unix_seconds=v)
}

[sql_table(name="events")]
struct Event {
    @sql_primary_key Id : int
    At : DateTime                       // round-trips via the adapter
}
```

**Same DateTime as TEXT (ISO8601):** different body, same
signature shape. The *return type* carries the storage choice.

```das
def sql_bind    (dt : DateTime) : string { return to_iso8601(dt) }
def sql_extract (v  : string; type<DateTime>) : DateTime {
    return parse_iso8601(v)
}
```

**Why name-based, not `register_sql_type(...)`:** the earlier
runtime-registration strawman (see API_MISSING.md § 29) hit
an ordering contradiction — `[sql_table]` runs at compile time
and needs to emit type-correct bind/column code, so a runtime
registration can't reach it. Name-based dispatch sidesteps the
problem entirely: the definition IS the registration, overload
resolution happens naturally at macro expansion, and the mechanism
mirrors daslang's existing `_::clone` / `_::finalize` convention.

**Eight locks:**

1. **Name-based dispatch, no registration.** No
   `register_sql_type(...)` call, no runtime lookup table, no
   ordering question ("what if the user's adapter is defined
   after `create_table` runs?"). If the pair exists when the
   `[sql_table]` macro expands, the type is SQL-addressable; if
   not, the user gets an overload-resolution error that names
   the exact struct field.
2. **Return type of `sql_bind` IS the storage type.** The
   macro reads it via `typedecl(_::sql_bind(obj.field))` at
   expansion and emits the matching `sqlite3_bind_*` / column
   type. Single source of truth — no separate
   `sql_column_type(type<T>)` registration function.
3. **Four primitive storage types:** `int64`, `double`,
   `string`, `array<uint8>`. Any other return type from
   `sql_bind` → `macro_error` at `[sql_table]` expansion with
   the valid list. Matches SQLite's four storage classes.
4. **`_::`-dispatched at emission.** The `[sql_table]` macro
   emits `_::sql_bind(obj.field)` and
   `_::sql_extract(..., type<T>)` so overloads defined in the
   **caller's** module are visible — same mechanism as
   `_::clone` / `_::finalize`. A domain type's adapters live
   next to the type, not inside `daslib/sql`.
5. **Standard-type overloads ship in the dasSQLITE module.**
   `int`, `int8`, `int16`, `int64`, `uint`/`uint8`/`uint16`/
   `uint64`, `float`, `double`, `bool`, `string`, `array<uint8>`.
   Trivial passthroughs (`def sql_bind(x : int) : int64 = int64(x)`).
   `bool` → `int64` (0/1). `float` → `double` (widening). No
   user work required for primitives.
6. **Enums auto-handled via one generic overload.**
   ```das
   def sql_bind $T (e : T) : int64 where T : enum { return int64(e) }
   def sql_extract $T (v : int64; type<T>) : T where T : enum {
       return T(v)
   }
   ```
   Any `enum`-kinded type round-trips through INTEGER
   automatically. Rename-safe text storage
   (`@sql_enum_as_string`) deferred — enum-reorder silent
   corruption is the tradeoff a user opts into by picking
   int-backed storage.
7. **Missing adapter → compile error at macro expansion.**
   Because the macro emits a literal `_::sql_bind(obj.field)`
   call, a type without the pair fails normal overload
   resolution. Error message names the struct, the field, and
   the type — actionable without a bespoke "type is not
   SQL-compatible" diagnostic path.
8. **Mismatched pair → compile error.** If `sql_bind(T) : int64`
   exists but `sql_extract(int64; type<T>) : T` doesn't (or
   returns the wrong type), the select-path emits an unresolved
   call and errors. Symmetric enforcement; no runtime surprise.

**Per-field override (locked shape, deferred implementation):**

```das
@sql_as(type<string>)                 // pick the string-returning overload
HumanAt : DateTime
```

`@sql_as(type<P>)` tells the `[sql_table]` macro to route this
column's bind/extract through a specific primitive instead of
the default. Requires the user to have defined both storage
forms of the pair. Useful when one schema wants `DateTime` as
INTEGER for indexability and another column wants TEXT for
human-readable `sqlite3` shell output. **Ships when a user
files an issue** — the core case (one adapter per type) covers
the 95% path.

**Edge cases:**

- **`Option<T>` on adapter types.** Handled by tut 25's
  invariant: `Option<DateTime>` is supported iff `DateTime`
  has the pair. The `[sql_table]` macro unwraps `Option<T>` to
  `T`, looks up adapters for `T`, emits NULL-aware bind/read.
  Zero extra work in tut 29.
- **BLOB-backed user types.** `Guid` → `array<uint8>` works
  the same way: user writes `sql_bind(g : Guid) : array<uint8>`
  plus matching extract, macro emits `sqlite3_bind_blob`.
- **Generic adapters** (`Option<T>` for any `T` with an adapter,
  `array<T>` → JSON, `tuple<…>` → JSON). These fall out of the
  `[sql_table]` walker's recursion on struct fields rather than
  being user-defined. Not a tut-29 deliverable; opportunistic
  extensions.
- **JSON columns** — tut 37 territory. `@sql_json Foo : tuple<…>`
  rides on top of the string-primitive adapter; dasSQLITE's
  only addition there is a `CHECK(json_valid(Foo))` constraint
  and routing through daslang's JSON module.

**Deliberately not shipped:**

- **No registration API.** Removed from the original strawman.
- **No runtime `SqlType` enum** (was `SqlType.Integer |
  SqlType.Real | SqlType.Text | SqlType.Blob` in API_MISSING's
  strawman). The return type of `sql_bind` replaces it —
  daslang's type system is the enum.
- **No third function `sql_column_type(type<T>)`.** Folded into
  lock 2 (storage type read from `sql_bind`'s return type).
- **`@sql_enum_as_string`** rename-safe enum storage.
- **Adapter validators** (`def sql_validate(v : T) : bool` for
  CHECK constraints). Column-level CHECK stays in tut 28's
  string form.
- **Fully-generic user-defined adapters** (`$T` templated on a
  trait other than `enum`). Lock 6's enum overload is the one
  concession; further user-defined genericity deferred until
  a concrete need surfaces.

**Mockup:** `tutorial-mockup/29-custom_types.das.mockup`. Covers
`DateTime` via INTEGER (default) and via TEXT (same struct,
two schemas), `Guid` via BLOB, enum auto-round-trip, standard-
type passthrough listing, `Option<T>` + adapter composition,
missing-adapter compile error demo, `@sql_as` hook
(commented-out "future work"), full deferred list.

**Phase-0.3 impact:** none. Zero linq additions. Advances
dasSQLITE-phase scope: `[sql_table]` macro emits
`_::sql_bind` / `_::sql_extract` calls, reads storage type via
`typedecl`, rejects non-primitive return types; module ships
stdlib-type passthrough overloads plus the enum generic.

**Running totals after tut 29:**

| Tut | linq.das | linq_boost | dasSQLITE macros |
|---|---|---|---|
| 19 group_by | 0 | 3 | `_group_by` + `_select` fusion |
| 23 joins | 1 | 3 (−1, drops `_select_many`) | moderate emitter work |
| 24 subqueries | 1 | 3 | nested-chain + outer-lambda classifier |
| 25 null_handling | 0 | 0 | type-driven DDL; Option recognition |
| 26 foreign_keys | 0 | 0 | `@sql_references` DDL; pragma-on-connect |
| 27 indexes | 0 | 0 | `[sql_index]` stackable DDL |
| 28 defaults_computed | 0 | 0 | `@sql_default` / `@sql_computed` DDL + bind-exclusion |
| 29 custom_types | 0 | 0 | `_::sql_bind`/`_::sql_extract` emission + stdlib overloads |
| **Total** | **2 fns** | **9 macros** | (emitter complexity accumulates) |

### 30-migrations — "up-only versioned migrations; MVP in a separate module" (decided 2026-04-24)

**Verdict:** sixth consecutive tut with **zero linq prereqs.**
Ships as a **separate, optional module** — `daslib/sql_migrate`
— so users who don't need migrations don't pay for them, and
the migration story evolves on its own schedule without
destabilizing core dasSQLITE. Tutorial opens with an explicit
"this is MVP; full migration frameworks are a huge topic; here's
what we ship and here's what we don't" scope statement.

**Scope stance (locked):** migrations are one of the biggest
topics in database tooling — Alembic, EF Core Migrations,
Flyway, Liquibase, Rails AR, Diesel, sqitch each pick a
different cutoff. Writing that story for daslang is **far out
of scope for v1.** The MVP below gives production code an
honest minimum (schema evolution is tracked, versioned, and
transactional) and explicitly defers everything else.

**Core shape (locked):**

```das
require daslib/sql
require daslib/sql_migrate
require sqlite/sqlite_boost

[sql_migration(version=1)]
def migration_001(db : SqlRunner) {
    db |> create_table(type<User>)
}

[sql_migration(version=2, description="add email")]
def migration_002(db : SqlRunner) {
    db |> exec("ALTER TABLE users ADD COLUMN Email TEXT")
}

[sql_migration(version=3)]
def migration_003(db : SqlRunner) {
    db |> exec("CREATE INDEX ix_users_email ON users(email)")
    db |> exec("UPDATE users SET email='unknown@example.com' WHERE email IS NULL")
}

[export] def main() {
    with_sqlite("app.db") <| $(db) {
        db |> migrate_to_latest()       // panics on failure
        // … or, for tools / CI:
        // match (db |> try_migrate_to_latest()) { … }
        // … app code runs against the up-to-date schema …
    }
}
```

**Public surface (exhaustive):**

| Name | Shape | Role |
|---|---|---|
| `[sql_migration(version=N, description="…")]` | function-level annotation | registers the function as a migration at compile time |
| `migrate_to_latest(db)` | `(db : SqlRunner) : int` | runs pending migrations in version order, returns count applied; **panics on failure** |
| `try_migrate_to_latest(db)` | `(db : SqlRunner) : Result<int, string>` | same, Result form for tools/CI |
| `current_schema_version(db)` | `(db : SqlRunner) : int` | highest applied version (0 if none) |
| `pending_migrations(db)` | `(db : SqlRunner) : array<int>` | versions that would run if `migrate_to_latest` were called now |
| `migration_history(db)` | `(db : SqlRunner) : array<MigrationRecord>` | full audit trail from `__schema_version` |
| `MigrationRecord` | `struct { version : int; description : string; applied_at : int64; success : bool }` | one row from the audit table |

**`__schema_version` table (fixed schema, auto-created on first run):**

```sql
CREATE TABLE __schema_version (
    version     INTEGER PRIMARY KEY,
    description TEXT    NOT NULL DEFAULT '',
    applied_at  INTEGER NOT NULL,          -- unix seconds
    success     INTEGER NOT NULL DEFAULT 1
)
```

One row per successfully-applied migration. Multi-row (not
single-row-with-current-version) so the audit trail survives —
matches Flyway's ergonomics at zero extra cost.

**Ten locks:**

1. **Separate module `daslib/sql_migrate`.** Sibling of
   `daslib/sql`, not merged into it. Users who don't need
   migrations don't `require` it; the migration API never
   appears in their symbol table.
2. **Up-only.** No down migrations, no rollback. Reasoning:
   down migrations in every framework that ships them are
   written, never run, and rot. Raw SQL in a new up-migration
   is the escape hatch.
3. **Raw SQL inside migration bodies.** No model-snapshot
   system, no struct-diff codegen. The struct always describes
   the *current* schema; migrations describe the *path* from
   prior states. This is the Flyway philosophy — EF's model-
   snapshot approach would require inventing a daslang-specific
   frozen-struct mechanism and is out of scope for v1.
4. **Version numbers are ints; gaps allowed.** `1, 2, 5, 10`
   runs fine — the runner sorts ascending and applies what's
   pending. Gaps make branch merges less painful (team A uses
   1-5, team B uses 100+, merge without renumbering).
5. **Duplicate `version=N` values → compile error** at
   `[sql_migration]` expansion. The annotation macro maintains
   a module-scoped sorted table; same-version collision is
   rejected with both function names named.
6. **Per-migration transaction.** Each migration body runs
   inside `BEGIN; … INSERT INTO __schema_version …; COMMIT;`.
   Any error inside rolls back the whole migration (SQL + the
   audit insert). Atomicity at the migration granularity, not
   the session granularity.
7. **Fail-fast runner.** Error in migration N aborts the runner
   — migrations N+1…last do NOT run. `migrate_to_latest` panics;
   `try_migrate_to_latest` returns `Result::Err`. No "skip bad
   migration and keep going" mode.
8. **Compile-time collection.** The `[sql_migration]` function
   macro appends `(version, function_ptr, description)` to a
   module-scoped sorted table. The runner iterates it — no
   RTTI walk at startup, no runtime registration.
9. **`__schema_version` auto-created.** First call to
   `migrate_to_latest` on a fresh DB creates the tracking table,
   then runs all migrations in order. Subsequent calls find
   the table and only run pending versions. No separate
   `initialize_migrations(db)` bootstrap step.
10. **Migration functions are plain daslang** — a migration is
    just a function taking `SqlRunner`. No special sub-DSL.
    Users can call any dasSQLITE API inside (`create_table`,
    `drop_table_if_exists`, `insert`, `_sql` queries for data
    migration, `exec` for raw SQL). The daslang compiler
    type-checks them like any other function.

**Explicitly deferred / out of scope (the MVP boundary):**

This is the meat of the tutorial — what users need to know
*isn't* shipping:

- **Down migrations / rollback.** A user writing `migrate_down(db, target_version)` is on their own. Future: if shipped, live as a separate function macro `[sql_migration_down(version=N)]` pairing with the up one.
- **Autogenerate from struct diff.** EF's `dotnet ef migrations add Foo` diffs the current model against a saved snapshot and emits the migration body automatically. Requires a model-snapshot system daslang doesn't have. Future: a separate `daslib/sql_migrate_gen` module (name TBD) with its own design discussion.
- **Model snapshots** (v1's `User` shape preserved after v4 rewrites it). EF saves snapshot C# files per migration; daslang would need frozen-struct tooling. Not attempting.
- **SQLite 12-step `ALTER TABLE` helper** (`recreate_table(db, type<T>, copy_from=...)` for drop/rename column on pre-3.25 SQLite). Highest-value migration helper and the single biggest pain point that distinguishes SQLite migrations from Postgres. Deliberately *not* shipped in v1 because getting it right is a mini-framework. Users writing `ALTER TABLE ... DROP COLUMN` on old SQLite versions write the 12-step by hand or bump the SQLite version requirement.
- **Zero-downtime / expand-contract pattern.** The multi-deploy pattern where schema changes stay compatible with both old and new app versions running simultaneously. Needs migration-pairing, dry-run validation, and a deploy contract. Out of scope; users who need this are running live services and can wire it up externally.
- **Data migrations vs schema migrations.** Most frameworks treat them separately (schema migrations run in DDL phase, data migrations in a post-phase). MVP conflates them — both happen inside the same transactional migration body. Fine for small schemas, will hit limits at scale.
- **Squashing / baselining.** After 3 years and 200 migrations, collapsing 1-100 into a single baseline + keeping 101-200 as-is. Future: a separate `squash(db, through=N)` tool that rewrites `__schema_version` after verifying the collapsed schema matches.
- **Advisory locks for concurrent runners.** If two processes call `migrate_to_latest` simultaneously, the MVP has no protection — both may try to apply the same migration. SQLite's single-writer model makes this mostly-harmless (the second transaction fails), but a proper framework takes an advisory lock. Future.
- **Migration testing harness.** EF has `dotnet ef database update --dry-run`; Flyway has `info`, `validate`, `migrate` phases. MVP ships `pending_migrations(db)` as the closest analog — just "what would run". A full dry-run (apply to a scratch DB, verify, discard) is future.
- **Branch/merge across developers.** Git-style merges of concurrently-written migration files. MVP punts via "use gap-allowed version numbers and coordinate on Slack". Real solution is a DAG of migrations with explicit dependencies (sqitch-style).
- **Repeatable migrations** (Flyway concept: re-runnable migrations for views, functions, seed data that may change every release). MVP ships only versioned one-shot migrations.
- **Multi-database / multi-attach migrations.** A migration body operating on `ATTACH`-ed databases. Raw SQL works but there's no support for "migrate database A to vN, then database B to vM" atomically.

The tutorial's tone is "here's the 100-line MVP that makes
dasSQLITE honest for production; the real migration framework
is a future effort — likely a separate `daslib/sql_migrate_*`
module or at the future `dasSQL` abstraction layer." Users
who need more than MVP know that on page one.

**Mockup:** `tutorial-mockup/30-migrations.das.mockup`. Covers the
three-migration example above, `__schema_version` auto-creation,
inspect/audit helpers, `try_` variant with error handling,
duplicate-version compile-error demo, full deferred-list with
per-item "if you need this in v1, here's your escape."

**Phase-0.3 impact:** zero. Zero linq additions. Advances
dasSQLITE-phase scope: a **new module file**
(`daslib/sql_migrate.das` — physical location TBD per the
locked module-layout table), a function-level annotation macro
(`[sql_migration]`), six public functions, one struct, one
auto-created table.

**Running totals after tut 30:**

| Tut | linq.das | linq_boost | dasSQLITE macros |
|---|---|---|---|
| 19 group_by | 0 | 3 | `_group_by` + `_select` fusion |
| 23 joins | 1 | 3 (−1, drops `_select_many`) | moderate emitter work |
| 24 subqueries | 1 | 3 | nested-chain + outer-lambda classifier |
| 25 null_handling | 0 | 0 | type-driven DDL; Option recognition |
| 26 foreign_keys | 0 | 0 | `@sql_references` DDL; pragma-on-connect |
| 27 indexes | 0 | 0 | `[sql_index]` stackable DDL |
| 28 defaults_computed | 0 | 0 | `@sql_default` / `@sql_computed` DDL + bind-exclusion |
| 29 custom_types | 0 | 0 | `_::sql_bind`/`_::sql_extract` emission + stdlib overloads |
| 30 migrations | 0 | 0 | new `daslib/sql_migrate` module; `[sql_migration]` + runner |
| **Total** | **2 fns** | **9 macros** | (emitter complexity accumulates) |

### 31-views — "`[sql_view]` as read-only `[sql_table]`; DDL in migrations" (decided 2026-04-24)

**Verdict:** seventh consecutive tut with **zero linq prereqs.**
Smallest tut in the batch — ~80% of the design is "it's just
`[sql_table]` with the mutation path cut out." Matches EF Core's
view story almost 1:1 (keyless entity + `ToView(name)`, views not
managed by migrations, DDL lives as raw SQL in migration bodies).

**Design anchor — where we diverge from EF (updated 2026-04-24):**
EF maps views as keyless entities (`[Keyless]` + `ToView("v_name")`)
and we take that half verbatim. Where we **diverge**: EF deliberately
doesn't ship a LINQ-to-DDL view helper — users write
`migrationBuilder.Sql("CREATE VIEW ...")` strings. EF's stated
reason is that view bodies want provider-specific features
(CTEs, window functions, hints) a LINQ pipeline would fight.
**That reasoning doesn't survive the daslang ethos argument.**
A view body IS a SELECT by definition; our `_sql` translator is
the whole design investment from tuts 19/23/24; forcing users
to drop to raw SQL strings for a SELECT they could have written
typed-and-checked is inconsistent with the rest of the API and
gives up compile-time column/type validation. Therefore v1
ships `_create_view(type<V>, <pipeline>)` as the **primary
path**; raw `exec("CREATE VIEW ...")` remains the escape for
features the translator doesn't cover (same escape as
`_sql` itself).

**Core shape (locked):**

```das
[sql_view(name="v_order_summary")]
struct OrderSummary {
    CustomerName : string
    Total        : float
    NumOrders    : int
}

[sql_migration(version=5, description="add order summary view")]
def migration_005(db : SqlRunner) {
    // Primary path — typed, column-checked, refactor-safe.
    db |> _create_view(type<OrderSummary>,
        select_from(type<Order>)
            ._join(type<Customer>) $(o, c) => o.CustomerId == c.Id
            ._group_by($(o, c) => c.Id)
            ._select($(o, c) => (
                CustomerName = c.Name,
                Total        = _sum(o.Total),
                NumOrders    = _count()
            ))
    )
}

// transparent querying — same `_sql` pipeline as tables:
let big = db |> _sql(
    select_from(type<OrderSummary>)._where($ => _.Total > 100.0)
)
```

**Eight locks:**

1. **`[sql_view(name="...")]` is a read-only sibling of
   `[sql_table]`.** Same field-introspection machinery (names,
   types, custom-type adapters per tut 29). Cuts out DDL
   generation, `@sql_primary_key`, `@sql_references`,
   `@sql_default`, `@sql_computed`, `@sql_unique`,
   `[sql_index]`. Applying any of those to a field under
   `[sql_view]` → `macro_error` at expansion with "not
   applicable to views; see tut 31."
2. **Query pipeline identical to `[sql_table]`.** Zero new
   query surface. `select_from(type<V>)`, `_where`,
   `_order_by`, `_select`, `_join` all work exactly the same.
   `type<V>` threads through the `_sql` translator wherever
   `type<T : sql_table>` would go.
3. **Mutations against a `[sql_view]` → compile error at
   macro expansion.** `insert(view_row)`, `update(view_row)`,
   `delete_(view_row)`, `upsert(...)`, `_update(type<V>, ...)`,
   `_delete(type<V>, ...)` all rejected. Error message names
   the view and points at "mutate underlying tables and
   re-query."
4. **`_create_view(type<V>, <pipeline>)` is the primary
   view-DDL path** — call macro that walks the `_sql`-style
   pipeline, generates the SELECT string at macro expansion,
   and emits `db |> exec("CREATE VIEW v_name AS <sql>")`. The
   column names and types in the final `_select` tuple are
   matched against the `[sql_view]` struct at expansion;
   mismatch → `macro_error` naming the offending column.
5. **Raw SQL remains the escape.** `db |> exec("CREATE VIEW
   ...")` still works — same escape as `_try_sql` for live
   queries that outgrow the translator. Features outside the
   `_sql` translator's grammar (window functions, recursive
   CTEs, SQLite-specific extensions) go this way.
6. **`drop_view_if_exists(type<V>)` ships.** Trivial wrapper
   over `exec("DROP VIEW IF EXISTS v_name")` — name read from
   the annotation, same ergonomics as `drop_table_if_exists`.
   No `create_view_if_not_exists` variant (silent skip on
   schema drift is a footgun; if users genuinely want that,
   wrap in a pragma check or drop-then-create).
7. **No connect-time existence check.** EF doesn't validate
   view presence at context creation — errors surface at
   query time. We match: `select_from(type<V>)` against a
   missing view fails with SQLite's `no such table: v_name`
   at query runtime.
8. **Views are keyless.** No `@sql_primary_key` allowed on a
   `[sql_view]` field (→ `macro_error`). EF's `[Keyless]` is
   the default for views; we make it mandatory. Natural-key
   columns stay as regular fields — queries and join
   predicates still work, just no PK metadata.

**`_create_view` failure modes at macro expansion:**

- **Column count mismatch.** Tuple has N fields; struct has M
  — error names both counts + both sides.
- **Column name mismatch.** Tuple field `CustomerName` but
  struct field `Name` — error names the offender.
- **Column type mismatch.** Tuple field emits `int64` but
  struct field is `string` — error names both types.
- **Pipeline uses a feature `_sql` can't translate.** Falls
  through to `_sql`'s normal translation-failure path;
  suggests the raw-SQL escape in the error message.
- **Bind expression has no `to_sql_literal` overload.** The
  macro emits `_::to_sql_literal(<bind>)`; if the bind's type
  isn't covered by the default set (numeric / bool / string)
  and the user has no overload in scope, the
  `to_sql_literal(auto(TT))` catch-all in `sqlite_linq.das`
  emits enums via `int(v)` and otherwise `concept_assert`s
  (40103) with: *to_sql_literal: unsupported type for
  `_create_view` body inlining. Define `def to_sql_literal(v
  : YourType) : string` in YourType's module.* Fix: add the
  one-line overload in the user's module.

**Deferred / out of scope:**

- **Updatable views via INSTEAD OF triggers.** Tut 41.
- **Materialized views.** No SQLite support; table+triggers
  simulation is a mini-framework; out of scope.
- **Schema-qualified view names.** SQLite uses `ATTACH` for
  multi-database (tut 36), not schemas — irrelevant here.
- **Runtime view-existence validation at `with_sqlite` /
  `open_sqlite`.** Would walk `[sql_view]`-registered types
  and query `sqlite_master`. Nice-to-have; costs startup
  time; defer.
- **`alter_view` helper.** SQLite doesn't support ALTER VIEW
  at all — the standard pattern is drop + recreate. A helper
  over that pair is ~2 lines; users write it inline.
- **`create_view_if_not_exists`.** Drift-hiding footgun; raw
  SQL if genuinely needed.

**Mockup:** `tutorial-mockup/31-views.das.mockup`. Covers the
`[sql_view]` annotation, `_create_view` as primary with the
JOIN/GROUP/aggregate example, raw-SQL escape for a
window-function example, `drop_view_if_exists` in a later
migration, transparent querying, mutation compile-error demo,
joining a view with a table, column-mismatch compile-error
shape, deferred list.

**Phase-0.3 impact:** zero. No linq additions. Advances
`daslib/sql` scope: one new structure macro (`[sql_view]`,
~80% shared with `[sql_table]`); `_sql` translator learns to
accept `type<V>` where `type<T : sql_table>` would go;
mutation-path macros add the "is this a view?" check and
reject if so; one new call macro `_create_view` (~50 lines —
wraps `_sql`'s SELECT-string emitter with a `CREATE VIEW name
AS` prefix + column-match check against the `[sql_view]`
struct); one new function `drop_view_if_exists(type<V>)`
(~5 lines).

**Running totals after tut 31:**

| Tut | linq.das | linq_boost | dasSQLITE macros |
|---|---|---|---|
| 19 group_by | 0 | 3 | `_group_by` + `_select` fusion |
| 23 joins | 1 | 3 (−1, drops `_select_many`) | moderate emitter work |
| 24 subqueries | 1 | 3 | nested-chain + outer-lambda classifier |
| 25 null_handling | 0 | 0 | type-driven DDL; Option recognition |
| 26 foreign_keys | 0 | 0 | `@sql_references` DDL; pragma-on-connect |
| 27 indexes | 0 | 0 | `[sql_index]` stackable DDL |
| 28 defaults_computed | 0 | 0 | `@sql_default` / `@sql_computed` DDL + bind-exclusion |
| 29 custom_types | 0 | 0 | `_::sql_bind`/`_::sql_extract` emission + stdlib overloads |
| 30 migrations | 0 | 0 | new `daslib/sql_migrate` module; `[sql_migration]` + runner |
| 31 views | 0 | 0 | `[sql_view]` + `_create_view` (linq-based DDL, raw-SQL escape) + `drop_view_if_exists` |
| **Total** | **2 fns** | **9 macros** | (emitter complexity accumulates) |

### 32-sql_functions — "user-defined scalar functions via sqlite3_create_function (SQLite-provider-specific)" (decided 2026-04-24)

**Verdict:** eighth consecutive tut with **zero linq prereqs.**
First tut to add surface **only** to the provider-specific
module (`sqlite/sqlite_boost`), not to `daslib/sql`. Reason:
SQLite's in-process C callback model (`sqlite3_create_function`)
has no cross-provider analog — PG/MySQL/MSSQL require
server-side stored-function DDL in a provider-specific
procedural dialect, and any abstract cross-provider API would
be a false promise.

**SQLite has no stored-function DDL at all** — no
`CREATE FUNCTION ... LANGUAGE plpgsql`, no T-SQL procedures,
no procedural dialect. Every user function is runtime-
registered via the C API. `register_function` in
`sqlite/sqlite_boost` isn't one option among many — it's the
*only* option.

**Design anchor — `Microsoft.Data.Sqlite.CreateFunction`:**

```csharp
conn.CreateFunction("regex_match",
    (string s, string pattern) => Regex.IsMatch(s, pattern),
    isDeterministic: true);
```

One line per function, type-generic, per-connection lifetime,
`isDeterministic` planner hint. The daslang equivalent takes
the same shape minus the lambda verbosity.

**Core shape (locked):**

```das
require sqlite/sqlite_boost

// v1 user-side: write a plain daslang function taking primitives.
def damage_formula(base : float; armor : int; crit_mult : float) : float {
    let effective_armor = max(0, armor - 5)
    let after_armor = max(1.0, base - float(effective_armor) * 0.8)
    return after_armor * crit_mult
}

[export] def main() {
    with_sqlite("game.db") <| $(db) {
        // Register on every connection — per-connection lifetime.
        db |> register_function("damage_formula", @@damage_formula,
                                deterministic=true)

        // Usable from raw SQL (v1 path — no `_sql` integration yet):
        let avg = db |> query_scalar<float>(
            "SELECT AVG(damage_formula(base_damage, target_armor, 1.5))
             FROM battles
             JOIN weapons ON weapons.id = battles.weapon_id
             JOIN targets ON targets.id = battles.target_id")
        to_log(LOG_INFO, "avg expected damage: {avg}")
    }
}
```

**Public surface (exhaustive, all in `sqlite/sqlite_boost`):**

| Name | Shape | Role |
|---|---|---|
| `register_function` | `(db : SqlRunner; name : string; fn : <function_ptr>; deterministic : bool = false; directonly : bool = false) : void` | wraps `sqlite3_create_function_v2`; arg count read from `fn`'s type |
| (C++ side: `sqlite3_register_function`) | C++ binding matching the SQLite C API | trampoline owner + lifetime mgmt |

One function, one C++ binding. That's the whole v1 surface.

**Supported primitive argument/return types (v1):** `int64`,
`double`, `string`, `array<uint8>`, `Option<int64>`,
`Option<double>`, `Option<string>`, `Option<array<uint8>>`.
`bool` sugar over `int64` (0/1) ships too for ergonomics.
Also: `int` / `float` widen to `int64` / `double` respectively
on input and output, same as tut 29's stdlib passthroughs.

Custom types from tut 29 (DateTime, Guid, domain enums) are
**not** auto-marshaled in v1. The user converts by hand inside
the function body (`let dt = DateTime(unix_seconds=i)`). See
lock 6 + the "future `[sql_function]` extension" note below.

**Nine locks:**

1. **Lives in `sqlite/sqlite_boost`, not `daslib/sql`.** No
   cross-provider abstraction. The function, its C++ binding,
   and the trampoline are all provider-local. Tut opens with
   an explicit "this feature doesn't port to PG/MySQL/MSSQL —
   those providers need server-side DDL in their own
   procedural dialects" paragraph.
2. **Scalar functions only in v1.** No aggregates
   (`xStep`/`xFinal` + per-group state), no window functions
   (`xValue`/`xInverse` for frame maintenance). Aggregates
   flagged as the first logical follow-up; window functions
   deferred indefinitely.
3. **`sqlite3_create_function_v2` under the hood, not v1.**
   v1 leaks `pApp` on replace/close; v2 adds the destructor
   callback we need for cleaning up the daslang-fn binding.
   No reason to use v1 in new code.
4. **Per-connection lifetime — re-register on every
   `with_sqlite` / `open_sqlite`.** Tutorial opens with this
   caveat in the first paragraph. SQLite's native semantics;
   no persistence in the DB file; a file containing data
   written by a custom function can't be read by a process
   that doesn't also register the same function.
5. **NULL handling via `Option<T>` in the signature.**
   Non-Option arg + SQL NULL input → function is **not
   called**; result is NULL (standard SQL NULL-propagation,
   matches built-ins like `LENGTH(NULL) = NULL`). Option arg
   + SQL NULL input → function is called with `none()`. Option
   return `none()` → `sqlite3_result_null`. Opt-in NULL
   awareness; non-Option signatures stay clean.
6. **Primitives-only argument types in v1.** `int64`, `double`,
   `string`, `array<uint8>` (+ their `Option<_>` forms,
   `bool`/`int`/`float` widening sugar). Custom types from
   tut 29 (DateTime, Guid, domain enums) are not
   auto-marshaled. Users convert by hand in the function body.
   **Clear extension spot flagged (see below) — a future
   `[sql_function]` macro will walk the fn signature and
   inject `_::sql_bind` / `_::sql_extract` calls
   automatically.**
7. **Arg count read from the function pointer's type.** The
   trampoline registers with `nArg = <type-derived count>`;
   SQLite rejects wrong-arity calls at parse time. No
   variadic support (`nArg=-1`) in v1.
8. **Panic inside the callback → `sqlite3_result_error`.**
   The trampoline wraps the daslang invocation in panic
   recovery; the panic message becomes the SQL error text
   returned to `sqlite3_step`, which bubbles up to the
   dasSQLITE caller as a normal error (panic for strict
   APIs, `Result::Err` for `try_` variants).
9. **`SQLITE_DETERMINISTIC` + `SQLITE_DIRECTONLY` exposed
   as bool params.** Both default to `false` matching
   SQLite's defaults; users opt in. `deterministic=true` is
   the common performance case (memoization + usability in
   index expressions). `directonly=true` is the security
   case (prevents the function from being smuggled into
   triggers / views / index expressions — relevant when the
   function has side effects or reads filesystem state).

**Future extension spot — `[sql_function]` macro (v2+):**

```das
// NOT SHIPPED in v1 — documented as the clear extension
// spot for the existing manual registration path.

[sql_function(name="age_at", deterministic=true)]
def age_at(birthday : DateTime; now : DateTime) : int {
    return int((now.unix_seconds - birthday.unix_seconds) / 31557600l)
}

// Macro would:
// 1. Walk the signature; for each non-primitive param type,
//    compose with tut 29's `_::sql_extract(primitive, type<T>)`.
// 2. Generate a primitive-typed wrapper + register it.
// 3. Emit a module-init hook so every `with_sqlite` registers
//    annotated functions automatically (addresses lock 4's
//    per-connection re-registration caveat).
```

Three wins over the v1 manual path:

1. **Custom-type adapter composition.** Users write `DateTime`,
   `Guid`, domain enums directly in the fn signature; the
   macro handles marshaling using tut 29's infrastructure.
2. **Auto-registration on connect.** No manual
   `register_function` call per `with_sqlite`; the macro
   threads the registration into connection setup.
3. **Single source of truth.** The function annotation
   declares the SQL name, determinism, direct-only flag —
   no drift between the definition and the registration site.

**Ships when a user asks, or when aggregates follow-up lands
and a broader redesign makes sense.** v1 manual path is fine
MVP.

**Deferred / out of scope for v1:**

- **Aggregate functions.** `xStep` + `xFinal` pair, per-group
  state via `sqlite3_aggregate_context`. Accumulator-pattern
  API (following EF's `CreateAggregate<TAccumulate, TResult>`)
  would be ~40 lines of additional C++ trampoline. First
  logical follow-up tut.
- **Window functions.** Five callbacks (`xStep`, `xFinal`,
  `xValue`, `xInverse`, + optional `xDestroy`). SQLite
  3.25+. Rare enough that deferring is easy; tut 31's
  window-function escape-hatch example covers the "use raw
  SQL" pattern.
- **Variadic functions (`nArg=-1`).** Daslang function
  pointers have fixed arity; exposing variadic needs either
  `array<sql_value>` parameter or explicit overload
  registration. Deferred.
- **`[sql_function]` annotation macro.** The extension spot
  described above. User writes `[sql_function]` on a fn,
  macro composes adapters, auto-registers on connect.
- **LINQ-integrated user functions (EF's `HasDbFunction`
  layer).** Teaching `_sql` to recognize registered
  function names and emit typed calls. Independent of this
  tut's layer-1 work; abstract-layer feature; future tut.
- **Loadable extensions** (`.load libfoo.so`). SQLite's
  C-extension mechanism; orthogonal to application-defined
  functions. Users needing FTS5 / R-Tree / JSON1 extensions
  call `sqlite3_load_extension` via the raw binding.

**Mockup:** `tutorial-mockup/32-sql_functions.das.mockup`. Covers
`damage_formula` with realistic game-analytics query, the
per-connection re-registration pattern, `deterministic` flag,
`Option<T>` NULL-propagation demo (function handling nullable
target_armor), panic → SQL error round-trip, v2
`[sql_function]` annotation as commented-out future work, full
deferred list.

**Phase-0.3 impact:** zero. No linq additions, no
`daslib/sql` changes. Advances `sqlite/sqlite_boost` scope:
one new function (`register_function`), one new C++ binding
(`sqlite3_register_function` — ~100 lines wrapping
`sqlite3_create_function_v2` + trampoline + destructor).

**Running totals after tut 32:**

| Tut | linq.das | linq_boost | dasSQLITE macros |
|---|---|---|---|
| 19 group_by | 0 | 3 | `_group_by` + `_select` fusion |
| 23 joins | 1 | 3 (−1, drops `_select_many`) | moderate emitter work |
| 24 subqueries | 1 | 3 | nested-chain + outer-lambda classifier |
| 25 null_handling | 0 | 0 | type-driven DDL; Option recognition |
| 26 foreign_keys | 0 | 0 | `@sql_references` DDL; pragma-on-connect |
| 27 indexes | 0 | 0 | `[sql_index]` stackable DDL |
| 28 defaults_computed | 0 | 0 | `@sql_default` / `@sql_computed` DDL + bind-exclusion |
| 29 custom_types | 0 | 0 | `_::sql_bind`/`_::sql_extract` emission + stdlib overloads |
| 30 migrations | 0 | 0 | new `daslib/sql_migrate` module; `[sql_migration]` + runner |
| 31 views | 0 | 0 | `[sql_view]` + `_create_view` (linq-based DDL, raw-SQL escape) + `drop_view_if_exists` |
| 32 sql_functions | 0 | 0 | `sqlite/sqlite_boost::register_function` (SQLite-only, scalar-only) |
| **Total** | **2 fns** | **9 macros** | (emitter complexity accumulates) |

### 33-pragma — "SQLite runtime knobs: WAL / busy_timeout / foreign_keys / synchronous" (decided 2026-04-24)

**Verdict:** ninth consecutive tut with **zero linq prereqs.** Second
consecutive provider-specific tut — all surface lives in
`sqlite/sqlite_boost`, not `daslib/sql`. Pragmas aren't portable
(PG uses `SET`, MySQL uses `SET GLOBAL`, MSSQL uses `sp_configure`;
names, types, and scopes all diverge), so any cross-provider
"set tuning parameter" abstraction would be a false promise. A
future `daslib/sql`-level hook can graft on when a second backend
ships and a real shared surface can be extracted from practice.

**Strawman problem (API_MISSING.md § 33):** every value was a
string.

```das
// STRAWMAN — wrong
db |> set_pragma("busy_timeout", "5000")    // 5000 is a number
db |> set_pragma("foreign_keys",  "ON")     // ON is a boolean
```

Values carry type information. The API should reflect that.

**Core shape (locked) — three overloads on value type:**

```das
def set_pragma(db : SqlRunner; name : string; value : string) : void
def set_pragma(db : SqlRunner; name : string; value : int64)  : void
def set_pragma(db : SqlRunner; name : string; value : bool)   : void

def try_set_pragma(db : SqlRunner; name : string; value : string) : Result<void, string>
def try_set_pragma(db : SqlRunner; name : string; value : int64)  : Result<void, string>
def try_set_pragma(db : SqlRunner; name : string; value : bool)   : Result<void, string>
```

Callers write the natural form per value kind:

```das
db |> set_pragma("journal_mode", "WAL")       // string — enum-ish mode
db |> set_pragma("busy_timeout", 5000)        // int64  — milliseconds (int widens)
db |> set_pragma("foreign_keys", true)        // bool   — ON/OFF flag
db |> set_pragma("synchronous", "NORMAL")     // string — durability level
```

**Booleans emit as `ON` / `OFF` on the wire.** SQLite accepts
0/1, TRUE/FALSE, YES/NO, ON/OFF interchangeably; ON/OFF reads
best in `options log` output and in panic-on-error text.

**`int` widens to `int64` at the call site** (same convention as
tut 32's primitive-widening rule). Users don't need to write
`5000l`.

**One-shot convenience (locked):**

```das
def apply_recommended_pragmas(db : SqlRunner) : void
def try_apply_recommended_pragmas(db : SqlRunner) : Result<void, string>
```

Sets the four defaults most apps want and logs each at `LOG_INFO`:

| pragma         | recommended | why |
|---|---|---|
| `journal_mode` | `WAL`       | readers don't block the writer; writer doesn't block readers |
| `busy_timeout` | `5000`      | wait 5s on lock contention instead of erroring immediately |
| `foreign_keys` | `ON`        | enable declared FK enforcement (SQLite default is OFF) |
| `synchronous`  | `NORMAL`    | WAL-safe durability; `FULL` default is fsync-heavy |

Tut source block is copy-pasteable if the user wants to set only
a subset or different values; `apply_recommended_pragmas` is a
shortcut, not a gate.

**Six locks:**

1. **Lives in `sqlite/sqlite_boost`.** Pragmas are provider-
   specific; no pretense of cross-backend abstraction in v1.
   Future `daslib/sql` "tuning hook" can dispatch per-provider
   if/when a second backend surfaces a sensible shared surface.
2. **Three value-typed overloads** (string / int64 / bool); no
   stringly-typed `any`-accepting fallback. The three-overload
   shape makes the right choice the obvious one.
3. **Boolean emits as `ON` / `OFF`.** Readable logs, matches how
   SQLite's docs talk about these pragmas.
4. **`apply_recommended_pragmas` ships bundled** with the four
   defaults above; logs each at `LOG_INFO`. Users who want
   different values call the four overloads explicitly.
5. **No typed enum surface in v1**
   (`set_journal_mode(JournalMode.WAL)`). Adds maintenance cost
   (one enum + one wrapper per well-known pragma, ~a dozen) for
   marginal benefit over the three typed overloads. Typos in
   pragma names surface at runtime via SQLite behavior (see
   gotcha below); typos in mode strings don't — but the enum
   alternative punishes the long tail of less-common pragmas
   equally. Ships as follow-up if users ask.
6. **No open-time named-tuple bundle in v1**
   (`with_sqlite(path, pragmas = (journal_mode="WAL", ...))`).
   Needs phase 0.2 (named-tuple literal) and a demonstrated
   need. Post-open form is already one line each; add if/when
   users ask.

**Read side not needed — skip `get_pragma` overloads.** `PRAGMA
foo` (no `=`) returns a one-column result set; tut 04's
`query_scalar<T>` covers it directly:

```das
let mode = db |> query_scalar<string>("PRAGMA journal_mode")
let wait = db |> query_scalar<int>   ("PRAGMA busy_timeout")
let fks  = db |> query_scalar<bool>  ("PRAGMA foreign_keys")
```

Multi-row pragmas (`PRAGMA table_info(x)`, `PRAGMA integrity_check`)
use `query<T>` normally (tut 09, tut 34). No bespoke getter
overloads.

**Transactional gotchas (tut warns; API does not enforce):**

- **`journal_mode` cannot change inside a transaction.** SQLite
  returns SQLITE_BUSY; strict form panics, `try_` form returns
  Err. Set at connection setup, outside `with_transaction`.
- **`foreign_keys` cannot change inside a transaction.** Unlike
  `journal_mode`, SQLite silently no-ops here rather than
  erroring. Same fix: set at connection setup.
- **Unknown pragma names are silently ignored by SQLite.** No
  error code. A typo like `set_pragma("forein_keys", true)` does
  nothing and returns success. The API cannot catch this at the
  binding layer — SQLite doesn't tell us. Only
  [sqlite.org/pragma.html](https://sqlite.org/pragma.html) is
  authoritative; read back with `query_scalar` if you suspect a
  pragma isn't taking effect.
- **Per-connection scope for most pragmas.** `foreign_keys`,
  `busy_timeout`, `synchronous`, `cache_size` apply to the
  current connection only; a fresh `open_sqlite` starts with
  SQLite's defaults. `journal_mode` is the exception — it's
  written into the DB header and persists across connections.

**Tut-26 consistency.** Tut 26 locked `PRAGMA foreign_keys=ON` as
a `with_sqlite` connect-time default. Tut 33 keeps that, and
makes it explicit that `apply_recommended_pragmas` is what tut 26
referenced — the four-pragma bundle, not just foreign_keys.

**Mockup:** `tutorial-mockup/33-pragma.das.mockup`. Opens with the
connection-setup pattern, shows all three typed overloads with
correct SQLite pragma names, demoes `apply_recommended_pragmas`,
shows `try_` + recovery, covers the transactional + unknown-
pragma gotchas, closes with the deferred list.

**Phase-0.3 impact:** zero. No linq additions, no `daslib/sql`
changes. Advances `sqlite/sqlite_boost` scope: three
`set_pragma` overloads, three `try_set_pragma` overloads,
`apply_recommended_pragmas`, `try_apply_recommended_pragmas`.
All thin wrappers over `exec` / `sqlite3_exec`.

**Deferred / out of scope for v1:**

- **Typed enum surface** (`set_journal_mode(JournalMode.WAL)`,
  `set_synchronous(Sync.NORMAL)`, …). ~a dozen enums + wrappers.
  Follow-up if users ask.
- **Open-time named-tuple bundle**
  (`with_sqlite(path, pragmas=(journal_mode="WAL", ...))`). Needs a
  demonstrated need (phase 0.2 already shipped).
- **Cross-provider "tuning hook"** in `daslib/sql`. Not worth
  designing before a second backend exists and a shared naming
  scheme can be extracted from real usage.
- **`PRAGMA optimize`** (SQLite 3.18+). Candidate for inclusion
  in `apply_recommended_pragmas` at CLOSE time (not at connection
  setup). Deferred to tut 34's operational-hygiene scope.
- **`[sql_pragma]` annotation** for module-scoped preferences.
  Parallel to tut 32's `[sql_function]` future macro. Defer —
  runtime API is the MVP.

**Running totals after tut 33:**

| Tut | linq.das | linq_boost | dasSQLITE macros |
|---|---|---|---|
| 19 group_by | 0 | 3 | `_group_by` + `_select` fusion |
| 23 joins | 1 | 3 (−1, drops `_select_many`) | moderate emitter work |
| 24 subqueries | 1 | 3 | nested-chain + outer-lambda classifier |
| 25 null_handling | 0 | 0 | type-driven DDL; Option recognition |
| 26 foreign_keys | 0 | 0 | `@sql_references` DDL; pragma-on-connect |
| 27 indexes | 0 | 0 | `[sql_index]` stackable DDL |
| 28 defaults_computed | 0 | 0 | `@sql_default` / `@sql_computed` DDL + bind-exclusion |
| 29 custom_types | 0 | 0 | `_::sql_bind`/`_::sql_extract` emission + stdlib overloads |
| 30 migrations | 0 | 0 | new `daslib/sql_migrate` module; `[sql_migration]` + runner |
| 31 views | 0 | 0 | `[sql_view]` + `_create_view` (linq-based DDL, raw-SQL escape) + `drop_view_if_exists` |
| 32 sql_functions | 0 | 0 | `sqlite/sqlite_boost::register_function` (SQLite-only, scalar-only) |
| 33 pragma | 0 | 0 | `sqlite/sqlite_boost::set_pragma` × 3 + `apply_recommended_pragmas` (SQLite-only) |
| **Total** | **2 fns** | **9 macros** | (emitter complexity accumulates) |

### 34-backup_vacuum — "online backup, VACUUM, integrity check" (decided 2026-04-24)

**Verdict:** tenth consecutive tut with **zero linq prereqs**.
Third consecutive provider-specific tut — all surface in
`sqlite/sqlite_boost`. Three primitives, all SQLite-proprietary:
online backup API (`sqlite3_backup_*`) has no cross-provider
analog; VACUUM semantics diverge (PG's VACUUM is
dead-tuple-reclaim-inside-live-table, not whole-file rebuild);
`PRAGMA integrity_check` is SQLite-only.

**Core teaching points.**

- **Backup is not a file copy.** The online backup API reads
  pages through SQLite's locking + transaction machinery so the
  destination stays transactionally consistent even with active
  writers. An OS-level copy (`cp app.db backup.db`) of a live DB
  can produce a torn file. Tut opens with this distinction.
- **VACUUM rebuilds the file** — reclaims free pages,
  defragments. Long operation, takes a write lock, needs up to
  2× the DB size in free disk space.
- **Integrity check** answers "is my DB corrupted?" — relevant
  after a crash, on startup, before a migration.

**Core shape (locked):**

```das
// online backup — safe on a live DB
def backup_to(db : SqlRunner; path : string) : void
def backup_to(db : SqlRunner; dest : SqlRunner) : void
def try_backup_to(db : SqlRunner; path : string) : Result<void, string>
def try_backup_to(db : SqlRunner; dest : SqlRunner) : Result<void, string>

// VACUUM — runtime-guarded against in-transaction use
def vacuum(db : SqlRunner) : void
def try_vacuum(db : SqlRunner) : Result<void, string>

// VACUUM INTO — compact snapshot; briefly takes write lock on source
def vacuum_into(db : SqlRunner; path : string) : void
def try_vacuum_into(db : SqlRunner; path : string) : Result<void, string>

// integrity — empty array == healthy
def integrity_check(db : SqlRunner) : array<string>
def quick_check(db : SqlRunner) : array<string>
def try_integrity_check(db : SqlRunner) : Result<array<string>, string>
def try_quick_check(db : SqlRunner) : Result<array<string>, string>

// close-time hygiene — tut 33 punted this here
def optimize(db : SqlRunner) : void
def try_optimize(db : SqlRunner) : Result<void, string>
```

**Seven locks:**

1. **Lives in `sqlite/sqlite_boost`.** All three primitives are
   SQLite-proprietary; no cross-provider abstraction makes
   honest sense.
2. **`backup_to` ships both path and runner forms.** Path is the
   common case; runner form supports backup to `:memory:`
   (in-memory snapshot for RO queries) and already-open
   destinations. Path form is a thin open-then-copy-then-close
   wrapper over the runner form.
3. **`vacuum` is runtime-guarded against in-transaction use.**
   Strict form panics with a clear message ("VACUUM cannot run
   inside an active transaction; commit or rollback first");
   `try_vacuum` returns Err. Compile-time check deferred —
   `with_transaction` lexical-scope awareness in the macro is
   too brittle across lambdas / forwarding.
4. **`vacuum_into(db, path)` ships alongside** for the compact-
   snapshot case (SQLite 3.27+). Briefly takes a write lock on
   source (unlike `backup_to` which allows concurrent writers);
   output is defragged / minimal-size. Tut explains when each
   applies:
   - `backup_to` — zero-downtime, allows writers, output may
     have free pages.
   - `vacuum_into` — briefly blocks writers, output defragged.
5. **`integrity_check` and `quick_check` ship as separate
   functions**, not one with a flag. `PRAGMA integrity_check`
   and `PRAGMA quick_check` have different SQLite semantics
   (quick_check skips UNIQUE/FK/NOT NULL constraint checks); a
   flag would hide the distinction. Both return
   `array<string>` — empty = healthy, non-empty = list of
   SQLite-reported issues.
6. **`optimize(db)` ships as a standalone function** — NOT
   auto-bundled into `apply_recommended_pragmas` (tut 33).
   `PRAGMA optimize` needs to run AFTER queries have built
   planner stats, so the natural call site is close time —
   opposite of setup-time pragmas. User opts in; no
   paternalistic auto-run.
7. **Backup progress callback deferred.** The C API supports
   stepwise `sqlite3_backup_step(bp, N)` with
   `sqlite3_backup_remaining` / `sqlite3_backup_pagecount` for
   UI progress. v1 runs backup in one call with a sensible
   internal chunk size; progress callback ships when users ask
   (big-DB progress bars are a real use case but rare).

**Transactional gotchas (tut warns, API does not enforce):**

- **VACUUM cannot run inside a transaction** — runtime panic
  with clear message; `try_vacuum` returns Err.
- **`backup_to` is safe with concurrent writers.** If a writer
  commits mid-backup, the internal step call returns
  SQLITE_BUSY and the backup transparently restarts its page
  copy.
- **`vacuum_into` takes a write lock** on source — other
  writers block. Not zero-downtime (unlike `backup_to`).
- **`integrity_check` can be slow** on big DBs (full table +
  index scan). `quick_check` is 5-10× faster for page-level
  issues at the cost of skipping constraint verification.

**Deferred / out of scope for v1:**

1. **Backup progress callback.** Stepwise API with
   remaining/pagecount telemetry. Ships when asked.
2. **ATTACH-database integration.** `backup_to` / `vacuum` /
   `integrity_check` default to `"main"` schema only.
   Multi-schema variants are tut 36 territory.
3. **`PRAGMA foreign_key_check`.** FK-specific integrity check.
   Users call `exec("PRAGMA foreign_key_check")` directly;
   rare.
4. **Compile-time VACUUM-in-transaction check.** Would need
   whole-program lexical-scope analysis that breaks on lambdas
   / forwarding. Runtime panic is sufficient.
5. **Automatic periodic `optimize`.** No daslang-level
   scheduler; users wire their own cadence (cron, job queue,
   daemon loop).
6. **Backup chunk size tuning.** v1 uses a sensible internal
   chunk (~100 pages/step). Exposed if progress callback ships.

**Mockup:** `tutorial-mockup/34-backup_vacuum.das.mockup`. Opens with
"backup ≠ file copy" explanation, shows path + runner backup
forms + in-memory snapshot use case, `try_backup_to` recovery,
`vacuum` + in-transaction panic shape + `try_vacuum`,
`vacuum_into` as compact-snapshot alternative,
`integrity_check` / `quick_check` tradeoff, startup-recovery
`try_integrity_check` flow, close-time `optimize` pattern,
6-item deferred list.

**Phase-0.3 impact:** zero. No linq additions, no `daslib/sql`
changes. Advances `sqlite/sqlite_boost` scope: 7 new runtime
functions + matching `try_` variants. All thin wrappers:
`backup_to` over `sqlite3_backup_*`; `vacuum` / `vacuum_into`
/ `optimize` over `exec`; `integrity_check` / `quick_check`
over `query<string>`.

**Running totals after tut 34:**

| Tut | linq.das | linq_boost | dasSQLITE macros |
|---|---|---|---|
| 19 group_by | 0 | 3 | `_group_by` + `_select` fusion |
| 23 joins | 1 | 3 (−1, drops `_select_many`) | moderate emitter work |
| 24 subqueries | 1 | 3 | nested-chain + outer-lambda classifier |
| 25 null_handling | 0 | 0 | type-driven DDL; Option recognition |
| 26 foreign_keys | 0 | 0 | `@sql_references` DDL; pragma-on-connect |
| 27 indexes | 0 | 0 | `[sql_index]` stackable DDL |
| 28 defaults_computed | 0 | 0 | `@sql_default` / `@sql_computed` DDL + bind-exclusion |
| 29 custom_types | 0 | 0 | `_::sql_bind`/`_::sql_extract` emission + stdlib overloads |
| 30 migrations | 0 | 0 | new `daslib/sql_migrate` module; `[sql_migration]` + runner |
| 31 views | 0 | 0 | `[sql_view]` + `_create_view` (linq-based DDL, raw-SQL escape) + `drop_view_if_exists` |
| 32 sql_functions | 0 | 0 | `sqlite/sqlite_boost::register_function` (SQLite-only, scalar-only) |
| 33 pragma | 0 | 0 | `sqlite/sqlite_boost::set_pragma` × 3 + `apply_recommended_pragmas` (SQLite-only) |
| 34 backup_vacuum | 0 | 0 | `sqlite/sqlite_boost::backup_to` / `vacuum` / `vacuum_into` / `integrity_check` / `quick_check` / `optimize` (SQLite-only) |
| **Total** | **2 fns** | **9 macros** | (emitter complexity accumulates) |

### 35-streaming — noted, no API impact (2026-04-24)

Streaming is a behavior of the existing tut-04 iterator-based
`for (row in _sql(...))`, not new API surface. Worth writing as a
user-facing tutorial (inoculates against "I called `to_array` on
a 10M-row query" perf reports), but **out of scope for the API
rework**. Row iterators are already `[unsafe_outside_of_for]`, so
the lifetime concern of returning one past `with_sqlite` scope is
handled by existing iterator safety — no new API.

Three strawman open questions resolve as doc concerns, not API:

- `sqlite3_stmt` lifetime on early `break` — per-iter `finally`
  cleanup calls `sqlite3_finalize` (verify at impl time).
- Re-iteration of a consumed iterator — SQLite-native one-shot
  semantics; document.
- Concurrent read/write iterators on same DB — SQLite's native
  locking behavior; tutorial warns.

**No mockup, no decision section, running totals unchanged.**
Flagged as a post-rework tutorial-writing task.

### 36-attach — "ATTACH DATABASE with runner-wrapper schema qualification" (decided 2026-04-24)

**Verdict:** eleventh consecutive tut with **zero linq prereqs**.
Fourth consecutive provider-specific tut — all surface in
`sqlite/sqlite_boost`. ATTACH has no cross-provider analog:
PG's foreign data wrappers, MSSQL's linked servers, MySQL's
FEDERATED engine share the *use case* (cross-DB query) but
diverge completely in API + semantics. Honest placement in the
provider namespace.

**Core shape (locked) — option (d): runner-wrapper schema qualification.**

```das
// attach / detach primitives
def attach(db : SqlRunner; path : string; as_name : string) : void
def detach(db : SqlRunner; as_name : string) : void
def try_attach(db : SqlRunner; path : string; as_name : string) : Result<void, string>
def try_detach(db : SqlRunner; as_name : string) : Result<void, string>

// block-scoped attach with auto-detach; block receives qualified runner
def with_attached(db : SqlRunner; path : string; as_name : string;
                  block : block<(scoped : SqlRunner) : void>) : void

// schema-qualified runner wrapper
def with_schema(db : SqlRunner; schema_name : string) : SqlRunner
```

`SqlRunner` gains a `schema_name : string` field (default
`"main"`). `with_schema` returns a copy of the runner with the
field set. All typed-query methods (`select_from`, `insert`,
`update`, `delete_`, `upsert`, `_sql(...)` translator) read the
runner's `schema_name` at emission and qualify table names
accordingly. The wrapper is a plain struct copy — shares the
underlying connection, no extra handle.

**Usage shapes:**

```das
// Path 1: explicit attach + qualified runner
with_sqlite("main.db") <| $(db) {
    db |> attach("archive.db", as_name = "archive")
    let arch = db |> with_schema("archive")
    for (u in arch |> select_from(type<User>)._where(_.Active)) {
        // _sql emits: SELECT ... FROM archive.Users WHERE Active = 1
    }
    db |> detach("archive")
}

// Path 2: block-scoped (preferred) — auto-detach, block receives qualified runner
with_sqlite("main.db") <| $(db) {
    db |> with_attached("archive.db", as_name = "archive") <| $(arch) {
        for (u in arch |> select_from(type<User>)) { … }
    }
}

// Path 3: cross-schema via raw SQL (v1 escape for cross-schema joins)
let merged <- db |> query<User>(
    "SELECT * FROM main.Users UNION SELECT * FROM archive.Users")
```

**Seven locks:**

1. **Lives in `sqlite/sqlite_boost`.** No cross-provider
   abstraction — PG/MySQL/MSSQL cross-DB mechanisms diverge
   completely in API + semantics.
2. **`attach` / `detach` minimal pair** + `try_` variants.
   Thin wrappers over SQLite's `ATTACH` / `DETACH` DDL.
3. **`with_attached(...) <| $(scoped)` is the preferred form.**
   Block body receives the schema-qualified runner; auto-
   detaches on normal and exceptional exit (try/finally-
   wrapped body). Can't forget; can't leak the attachment.
4. **Schema qualification via runner-wrapper (option d).**
   `SqlRunner` carries a `schema_name` field (default
   `"main"`); `with_schema(db, name)` returns a copy with
   the field set. All typed-query methods respect the
   runner's schema at emit time. Single extension point;
   no per-method surface bloat.
5. **One-schema-per-`_sql`-chain in v1.** Each `_sql(...)`
   call operates on a single runner with a single schema
   qualifier. Cross-schema typed joins deferred — users
   UNION / JOIN across schemas via raw SQL with qualified
   names.
6. **Runner wrapper is cheap** — plain struct copy with
   different `schema_name`. Shares the underlying DB
   connection.
7. **No validation at `with_schema` call.** Schema existence
   is checked by SQLite at query time (runtime
   `SQLITE_ERROR` for nonexistent schema). Wrapper creation
   is pure — no side effects, no round-trip.

**Why (d) beat the alternatives:**

- **(a) Raw SQL only.** Minimal, but surrenders the type
  safety `[sql_table]` + `_sql` exist for. Users with real
  ATTACH use cases (per-user DBs, archive splits) would
  hand-write qualified SQL — losing everything the typed
  layer gives them.
- **(b) Static `[sql_table(schema="archive")]`.** Wrong
  fit. ATTACH's `as_name` is chosen at runtime (often
  user-id-dependent, e.g. `attach("/users/{uid}.db",
  as_name="user_{uid}")`); static annotation can't express
  that.
- **(c) `select_from(type<User>, schema="archive")`
  dynamic param.** Honest about dynamic schema but bloats
  every typed-query method (`select_from`, `insert`,
  `update`, `delete_`, `upsert`, ...) with a schema
  parameter. Boilerplate per call site.
- **(d) Runner-wrapper.** Single extension point;
  transparent to every query method; decomposes well (save
  qualified runner in a local, pass it around); matches
  how users think ("this runner is scoped to schema X").

**Open-question resolutions (no API change):**

- **Cross-DB transactions.** SQLite handles them natively in
  WAL mode. Tut notes the WAL caveat; not an API concern.
- **`[sql_table]` schema annotation.** Explicitly rejected
  (alt-b above). Schema is runtime-dynamic; `[sql_table]`
  stays schema-agnostic.

**Deferred / out of scope for v1:**

1. **Cross-schema typed joins.** `_sql(main |> select_from
   (type<User>)._join(arch |> select_from(type<Order>),
   ...))` emitting `FROM main.Users JOIN archive.Orders`.
   Moderate `_sql` translator work (subquery-runner
   qualification). Raw SQL covers v1.
2. **Multi-attach in a single call.**
   `with_multi_attached(db, [("a.db","a"), ("b.db","b")])
   <| $(runners) { … }`. Users nest `with_attached` calls
   in v1.
3. **PRAGMA-per-attached-schema** (`PRAGMA archive.table_info
   (x)`) typed wrapper. Users run raw `exec` / `query`.
4. **ATTACH with custom open flags** — read-only attach,
   URI-style paths (`"file:archive.db?mode=ro"`). v1's
   `path` is a plain filesystem path. Ships if users ask.
5. **Schema listing** — `list_schemas(db) : array<string>`
   over `PRAGMA database_list`. Users run the pragma
   directly.

**Mockup:** `tutorial-mockup/36-attach.das.mockup`. Opens with
"why ATTACH" (per-user DBs, archive splits, import flows),
shows `attach`/`detach` pair, `with_attached` block form
(preferred), `with_schema` runner-wrapper decomposed use,
cross-schema via raw SQL, runtime error on nonexistent
schema, 5-item deferred list.

**Phase-0.3 impact:** zero. No linq additions, no
`daslib/sql` changes. Advances `sqlite/sqlite_boost`:
`attach` / `detach` / `with_attached` / `with_schema` +
`try_` variants. Minor `SqlRunner` field addition
(`schema_name : string` with default `"main"`).

**Running totals after tut 36:**

| Tut | linq.das | linq_boost | dasSQLITE macros |
|---|---|---|---|
| 19 group_by | 0 | 3 | `_group_by` + `_select` fusion |
| 23 joins | 1 | 3 (−1, drops `_select_many`) | moderate emitter work |
| 24 subqueries | 1 | 3 | nested-chain + outer-lambda classifier |
| 25 null_handling | 0 | 0 | type-driven DDL; Option recognition |
| 26 foreign_keys | 0 | 0 | `@sql_references` DDL; pragma-on-connect |
| 27 indexes | 0 | 0 | `[sql_index]` stackable DDL |
| 28 defaults_computed | 0 | 0 | `@sql_default` / `@sql_computed` DDL + bind-exclusion |
| 29 custom_types | 0 | 0 | `_::sql_bind`/`_::sql_extract` emission + stdlib overloads |
| 30 migrations | 0 | 0 | new `daslib/sql_migrate` module; `[sql_migration]` + runner |
| 31 views | 0 | 0 | `[sql_view]` + `_create_view` (linq-based DDL, raw-SQL escape) + `drop_view_if_exists` |
| 32 sql_functions | 0 | 0 | `sqlite/sqlite_boost::register_function` (SQLite-only, scalar-only) |
| 33 pragma | 0 | 0 | `sqlite/sqlite_boost::set_pragma` × 3 + `apply_recommended_pragmas` (SQLite-only) |
| 34 backup_vacuum | 0 | 0 | `sqlite/sqlite_boost::backup_to` / `vacuum` / `vacuum_into` / `integrity_check` / `quick_check` / `optimize` (SQLite-only) |
| 36 attach | 0 | 0 | `sqlite/sqlite_boost::attach` / `detach` / `with_attached` / `with_schema` + `SqlRunner.schema_name` (SQLite-only) |
| **Total** | **2 fns** | **9 macros** | (emitter complexity accumulates) |

### 37-json — "JSON and BLOB columns via tut-29 rail + `_sql` JSON-path walker rule" (decided 2026-04-24)

**Verdict:** twelfth consecutive tut with **zero linq prereqs.**
Pure dasSQLITE work in the ABSTRACT layer (`daslib/sql`): two new
per-field annotations (`@sql_json`, `@sql_blob`) that trigger
`[sql_table]` to auto-generate tut 29's `sql_bind` / `sql_extract`
pair at the table-struct's module scope; `_sql` walker gains a
rule for "field descent on a `@sql_json`-tagged column →
`json_extract(col, '$.<path>')`". Per-provider emitter renders
the SQL-dialect spelling (`json_extract` for SQLite; `->>` for
PG; `JSON_VALUE` for MSSQL — v1 ships SQLite only).

**Core shape (locked):**

```das
[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name                : string
    @sql_json Prefs     : tuple<theme : string; lang : string; notify : bool>
    @sql_blob State     : SessionSnapshot        // opaque BLOB
}
```

`[sql_table]` expansion emits, at the table-struct's module scope
(idempotent — once per distinct type):

```das
// for @sql_json T:
def sql_bind    (v : T) : string          { return write_json(JV(v)) }
def sql_extract (v : string; type<T>) : T {
    var err = ""
    let jv = read_json(v, err)
    if (err != "") { panic(err) }
    return from_JV(jv, default<T>)
}

// for @sql_blob T (const param + clone_to_move because the chunk-8
// catch-all binder passes `v` non-`var`, and mem_archive_* need a
// mutable reference):
def sql_bind    (v : T) : array<uint8> {
    var v_local <- clone_to_move(v)
    return <- mem_archive_save(v_local)
}
def sql_extract (v : array<uint8>; type<T>) : T {
    var r = default<T>
    var v_local <- clone_to_move(v)
    mem_archive_load(v_local, r)           // panics on failure
    return <- r
}
```

**Eight locks:**

1. **Explicit annotation, no silent defaults.** `@sql_json` or
   `@sql_blob` per field. Without either, a struct/tuple-typed
   column stays a compile error (tut 29 lock 7 — no adapter).
   No generic `sql_bind $T where T : struct_or_tuple` ships.
   Rationale: "JSON or BLOB?" is load-bearing (schema drift +
   SQL queryability); a default would lock in the wrong answer
   half the time.
2. **`[sql_table]` generates the tut-29 pair at expansion time.**
   The adapter lives in the caller's module (same scope as the
   table struct), visible to `_::sql_bind` / `_::sql_extract`
   dispatch. Generated once per distinct type — a second
   annotation on the same type reuses the existing pair.
3. **`@sql_json` uses `daslib/json` + `daslib/json_boost`.**
   Bind: `write_json(JV(v))`. Extract:
   `from_JV(read_json(v, err), default<T>)`. Type breadth =
   whatever `JV`/`from_JV` round-trips (primitives, enums,
   tuples, structs, arrays, nested composites).
4. **`@sql_blob` uses `daslib/archive`.** Bind:
   `mem_archive_save(v)`. Extract: `mem_archive_load(v, r)`
   with `canfail = false` (strict panic). Archive covers a
   wider type surface than `json_boost` — smart_ptr, variant,
   matrix types also work.
5. **`_sql` walker descends inside `@sql_json` columns only.**
   `[sql_table]` records per-field "@sql_json was here" in the
   table metadata; `_sql` reads the flag. `_.<col>.<path…>` on
   a `@sql_json` column → `json_extract(<col>, '$.<path…>')`
   (recursive on nested struct/tuple). Subfield's declared
   daslang type drives result-side extraction via tut 29's
   adapters.
6. **`_sql` field descent on a `@sql_blob` column is a compile
   error.** Binary blobs are opaque to SQL — no sensible
   emission. Error names the field and says to filter in
   daslang after round-trip, or use raw SQL.
7. **Strict panic on bad stored data; no CHECK-constraint DDL.**
   Malformed JSON → `read_json` fails → `sql_extract` panics.
   Corrupt / schema-mismatched blob → `mem_archive_load` fails
   → panic. `try_select_from` / `try_query<T>` surface either
   as Err. No auto `CHECK(json_valid(col))` — the strict
   extract is the gate.
8. **Provider-specific JSON-path dialect stays in the emitter.**
   The walker rule is universal ("field descent on @sql_json
   column = JSON path"); per-provider emitter renders the
   call. `daslib/sql` hosts the walker; `sqlite/sqlite_boost`
   renders `json_extract`. PG (`col->>'path'`), MySQL, MSSQL
   emitters ship with their providers.

**Deliberately not shipped:**

- **Generic `sql_bind $T where T : struct_or_tuple`** — explicit
  annotation is the design call. Tuple/struct columns without
  `@sql_json` or `@sql_blob` remain tut 29 lock 7 compile errors.
- **`CHECK(json_valid(col))` auto-DDL** — see lock 7.
- **`@sql_json_index("$.path")` typed expression index** —
  dialect-divergent; ships after a second provider lands.
- **`@sql_json(fast)` fast-path using `json_sprint` /
  `json_sscan`** — profile-driven opt-in; ships when asked.
- **`@sql_json(binary)` JSONB storage (SQLite 3.45+)** — ships
  when 3.45+ is broadly deployable.
- **`@sql_blob(zstd)` / `@sql_blob(lz4)` compression** — ships
  when asked.
- **`@sql_blob` versioned-format header** — needs a
  `daslib/archive` schema-evolution story first.
- **JSON mutation in `_update` chains (`json_set` / `json_patch`
  / `json_remove`)** — raw `exec` covers v1.
- **Array subscript inside JSON path (`_.Tags[0]` → `'$[0]'`)**
  — deferred `_json_extract<T>(col, path)` escape (below).
- **`_json_extract<T>(col, path)` explicit escape** — for paths
  the walker can't reach. Ships when a wall is hit.
- **`@sql_json_raw`** — TEXT-JSON column whose declared type is
  `string`, user-managed serialization. Niche.
- **Cross-provider JSON path emitter** — v1 emits SQLite syntax
  only; per-provider dialect when a second backend ships.

**Mockup:** `tutorial-mockup/37-json.das.mockup`. Covers both
annotations, side-by-side contrast table (storage /
queryability / speed / size / schema drift), walker descent on
nested structs, compile-error demo for `@sql_blob` field
descent, panic-on-malformed-data semantics, 7 gotchas (type
fidelity, schema-fragile blob, JSON1 availability, expression
indexes, JSON mutation, array subscript, @sql_blob type limits),
10-item deferred list.

**Phase-0.3 impact:** none. Zero linq additions. Advances
dasSQLITE-phase scope: two annotations in `daslib/sql`,
`[sql_table]` adapter-pair codegen at macro expansion, `_sql`
walker JSON-path rule, per-provider `json_extract` emission in
`sqlite/sqlite_boost`.

**Running totals after tut 37:**

| Tut | linq.das | linq_boost | dasSQLITE macros |
|---|---|---|---|
| 19 group_by | 0 | 3 | `_group_by` + `_select` fusion |
| 23 joins | 1 | 3 (−1, drops `_select_many`) | moderate emitter work |
| 24 subqueries | 1 | 3 | nested-chain + outer-lambda classifier |
| 25 null_handling | 0 | 0 | type-driven DDL; Option recognition |
| 26 foreign_keys | 0 | 0 | `@sql_references` DDL; pragma-on-connect |
| 27 indexes | 0 | 0 | `[sql_index]` stackable DDL |
| 28 defaults_computed | 0 | 0 | `@sql_default` / `@sql_computed` DDL + bind-exclusion |
| 29 custom_types | 0 | 0 | `_::sql_bind`/`_::sql_extract` emission + stdlib overloads |
| 30 migrations | 0 | 0 | new `daslib/sql_migrate` module; `[sql_migration]` + runner |
| 31 views | 0 | 0 | `[sql_view]` + `_create_view` (linq-based DDL, raw-SQL escape) + `drop_view_if_exists` |
| 32 sql_functions | 0 | 0 | `sqlite/sqlite_boost::register_function` (SQLite-only, scalar-only) |
| 33 pragma | 0 | 0 | `sqlite/sqlite_boost::set_pragma` × 3 + `apply_recommended_pragmas` (SQLite-only) |
| 34 backup_vacuum | 0 | 0 | `sqlite/sqlite_boost::backup_to` / `vacuum` / `vacuum_into` / `integrity_check` / `quick_check` / `optimize` (SQLite-only) |
| 36 attach | 0 | 0 | `sqlite/sqlite_boost::attach` / `detach` / `with_attached` / `with_schema` + `SqlRunner.schema_name` (SQLite-only) |
| 37 json | 0 | 0 | `@sql_json` / `@sql_blob` annotations + `[sql_table]` adapter-pair codegen + `_sql` JSON-path walker rule (abstract layer) |
| **Total** | **2 fns** | **9 macros** | (emitter complexity accumulates) |

### 38-bulk_ops — noted, no API impact (2026-04-24)

Bulk operations are a re-presentation of existing locks, not new
API surface. Strawman (API_MISSING.md § 38) predates the tut 15
naming revision that dropped the `_sql_` prefix and moved
mutations to standalone functions. Mapping each strawman bullet
to current locks:

- **`insert(array<T>)` auto-txn** — tut 02 ships this.
- **Bulk `_update` with predicate** — tut 15's
  `_update(type<T>, where, set)` / `update(db, type<T>, where,
  set)` covers it. `_sql_update` in the strawman is the pre-
  revision name.
- **Bulk `_delete` with predicate** — tut 16's `_delete` /
  `delete_(db, type<T>, where)` covers it.
- **Streaming insert (`insert_stream <| $(yield) { … }`)** — not
  a primitive; equivalent to `with_transaction + for + insert`.
  Prepared-statement cache reuse across the loop is a dasSQLITE
  internal (statement-cache design challenge, flagged in this
  file), not user-facing API.
- **`INSERT … SELECT` server-side** — covered by raw
  `exec("INSERT INTO … SELECT … WHERE …")`. A typed
  `insert_into(type<Target>; from = _select_from(...)...)`
  helper was considered and deliberately not shipped in v1: the
  use case (imports / archival) is rare, and any typed surface
  either duplicates the `_sql` translator or needs its own
  walker rule for the combined SELECT+INSERT terminal. If a
  user files an issue, reconsider.

Transaction sizing for million-row inserts (auto-chunking knob)
is a user's `for` loop + periodic `COMMIT; BEGIN` — not a new
API. `apply_recommended_pragmas` (tut 33) already covers the
`journal_mode = WAL` + `synchronous = NORMAL` durability-vs-
throughput knobs.

**No mockup, no decision section, running totals unchanged.**
Flagged as a post-rework tutorial-writing task — the "how to
insert 8 million rows correctly" story is worth a dedicated tut
even though the API is already there.

### 39-fts5 — "string predicates inside _where; FTS5 via predicate + annotation" (decided 2026-04-24)

**Verdict:** thirteenth consecutive tut with **zero linq prereqs.**
ONE new strings_boost function (`text_match`) lifted into a new
Phase 0.4 row; the rest is pure dasSQLITE translator-and-DDL
work. `starts_with` / `ends_with` / `contains` (already in
`daslib/strings_boost`) gain `_sql` translator rules that emit
`LIKE` patterns with automatic escaping — portable across SQL
backends. `text_match(col, query)` gets recognized on
`[sql_fts5]`-indexed string columns and emits `col MATCH ?`; on
regular `[sql_table]` columns it's a compile error. `@sql_fts_rank`
marks the auto-populated relevance-rank field.

**Key insight:** MATCH is a PREDICATE, not a chain operator. It
filters; it doesn't change result shape (no group / join /
pagination). So it lives inside `_where(...)` alongside `==`
and `<` — not as a new chain op. The same insight unifies the
portable-trio `LIKE` translation: any string-predicate function
called inside `_where` gets an emission rule.

**Core shape (locked):**

```das
// portable — every SQL backend, also in-memory:
_where(_.Name  |> starts_with(prefix))       // → LIKE 'prefix%'
_where(_.Email |> ends_with("@gaijin.team"))  // → LIKE '%@gaijin.team'
_where(_.Name  |> contains(part))             // → LIKE '%part%'

// SQLite FTS5 — requires [sql_fts5]-annotated struct:
[sql_fts5(name = "docs_idx")]
struct Doc {
    Body : string
    @sql_fts_rank Rank : float
}

_sql(db |> select_from(type<Doc>)
    ._where(_.Body |> text_match("quick* AND fox"))
    ._order_by(_.Rank)                // lower rank = more relevant
    ._take(20))
// => SELECT Body, rank FROM docs_idx
//    WHERE Body MATCH ? ORDER BY rank LIMIT 20
```

**Nine locks:**

1. **Predicates live inside `_where`, not as chain ops.**
   `MATCH` and `LIKE` filter; they don't change shape. No new
   chain operator — translator pattern-matches known
   string-predicate function calls inside `_where` bodies.

2. **Portable trio uses existing `daslib/strings_boost` functions.**
   `starts_with` / `ends_with` / `contains` are already defined
   for in-memory use. Translator rules emit `LIKE` with
   automatic escaping (`%`, `_`, `\` in user input → `ESCAPE '\\'`).
   Zero new functions in strings_boost for the trio; pure
   translator work.

3. **`text_match(text, query) : bool` ships in `daslib/strings_boost`
   (Phase 0.4).** In-memory implementation covers the FTS5
   query-syntax subset: whitespace-AND, `*` prefix, Unicode
   word-boundary + ASCII case-fold. Unsupported subset (NEAR,
   AND/OR/NOT keywords, phrases, column filters) → panic with
   "unsupported FTS5 query syntax in-memory; use SQL backend
   for the full grammar." Useful standalone for log / config /
   doc filtering.

4. **`text_match` on an `[sql_fts5]`-indexed column emits
   `MATCH ?`.** Translator recognizes the call inside `_where`,
   keys on the `[sql_fts5]` flag from `[sql_table]` metadata,
   emits `col MATCH ?` with full FTS5 query syntax passed
   through to the DB.

5. **`text_match` on a non-`[sql_fts5]` struct = compile error.**
   Translator rejects at macro expansion with a message pointing
   at `contains` (for LIKE-based substring) or adding `[sql_fts5]`
   (for indexed full-text). No silent LIKE fallback — FTS5 query
   syntax doesn't cleanly map to LIKE.

6. **`[sql_fts5(name="docs_idx")]` = self-contained FTS5 virtual
   table.** `create_table(type<Doc>)` emits
   `CREATE VIRTUAL TABLE IF NOT EXISTS docs_idx USING fts5(...)`.
   All `string` fields become FTS5 content columns. Default
   tokenizer = `unicode61`. v1 ships self-contained mode only;
   external-content-table mode (`content_table=`, `content_column=`)
   deferred.

7. **`@sql_fts_rank Rank : float` auto-maps to FTS5's hidden
   `rank` column on SELECT.** Typed as `float`; accessed via
   normal field syntax. `_order_by(_.Rank)` emits `ORDER BY rank`
   (ascending — FTS5 rank is a positive cost, lower = more
   relevant). On INSERT, `@sql_fts_rank` fields are ignored
   (read-only).

8. **LIKE escaping is automatic.** Translator emits
   `ESCAPE '\\'` and escapes `%`, `_`, `\` in the bound value.
   User input `"50%"` as a `starts_with` argument matches paths
   starting with literal `"50%"`, not `"50"` + anything.

9. **SQLite-only for v1; provider-per-backend pattern locked
   for later.** `text_match` translator rule lives in
   `sqlite/sqlite_boost` emitter (emits FTS5 MATCH). PG's
   `tsvector`/`to_tsquery`, MySQL's `MATCH AGAINST`, MSSQL's
   `CONTAINS` all map to the same `text_match` predicate via
   per-backend emitter when a second provider lands. The
   portable trio's `LIKE` emission is already cross-provider.

**Deliberately not shipped:**

- **`_match` as a chain operator** — not needed; predicates
  live in `_where`.
- **Silent LIKE fallback for `text_match` on non-FTS5 columns**
  — compile error is the design call; FTS5 syntax and LIKE
  semantics don't align.
- **External-content-table FTS5 mode** (`content_table=` etc.)
  — ships when asked; self-contained covers v1.
- **Tokenizer config in the annotation** (`tokenize="porter"`,
  `"ascii"`, custom) — raw DDL in v1.
- **Prefix index (`prefix='2 3 4'` option)** — niche; raw DDL.
- **Highlight / snippet / offsets typed wrappers** — raw `exec`
  covers v1.
- **BM25 custom weights** (`rank='bm25(10.0, 5.0)'`) — raw DDL.
- **Typed NEAR / phrase DSL** — users pass the FTS5 query
  string raw in v1.
- **Typed rebuild wrapper** — one-line raw `exec`; trivial sugar
  if asked.
- **Cross-provider full-text emitter** — v1 SQLite-only; pattern
  scales per-provider when second backend lands.
- **ILIKE / Unicode case-insensitive** — SQLite LIKE is ASCII-CI
  only; Unicode CI needs a collation (raw SQL).
- **Regex predicate** (`regex_match` → SQLite's optional
  REGEXP operator) — not always built-in; ships via explicit
  extension-load if requested.
- **In-memory full FTS5 grammar** — NEAR, AND/OR/NOT, phrases,
  column filters in the strings_boost implementation. Query
  syntax parser would balloon 10×; the subset covers 90% of
  standalone use.

**Mockup:** `tutorial-mockup/39-fts5.das.mockup`. Covers the portable
trio with LIKE emission + escape, FTS5 table declaration,
`text_match` usage (SQL + in-memory), compile-error demo on
non-FTS5 struct, panic demo on unsupported in-memory query
syntax, 5-row choice table, 7 gotchas, 10-item deferred list.

**Phase-0 impact:** adds **Phase 0.4** —
`daslib/strings_boost::text_match`. Useful standalone; not
SQL-specific. Zero linq.das/linq_boost additions.

**Phase-0.3 impact:** zero.

**dasSQLITE-phase impact:** `_sql` translator string-predicate
rules for `starts_with`/`ends_with`/`contains`/`text_match`
(emit `LIKE` with `ESCAPE '\\'` or `MATCH` depending on annotation),
`[sql_fts5]` DDL annotation, `@sql_fts_rank` extraction,
`sqlite/sqlite_boost` emitter renders SQLite-specific FTS5 syntax.

**Running totals after tut 39:**

| Tut | linq.das | linq_boost | dasSQLITE macros |
|---|---|---|---|
| 19 group_by | 0 | 3 | `_group_by` + `_select` fusion |
| 23 joins | 1 | 3 (−1, drops `_select_many`) | moderate emitter work |
| 24 subqueries | 1 | 3 | nested-chain + outer-lambda classifier |
| 25 null_handling | 0 | 0 | type-driven DDL; Option recognition |
| 26 foreign_keys | 0 | 0 | `@sql_references` DDL; pragma-on-connect |
| 27 indexes | 0 | 0 | `[sql_index]` stackable DDL |
| 28 defaults_computed | 0 | 0 | `@sql_default` / `@sql_computed` DDL + bind-exclusion |
| 29 custom_types | 0 | 0 | `_::sql_bind`/`_::sql_extract` emission + stdlib overloads |
| 30 migrations | 0 | 0 | new `daslib/sql_migrate` module; `[sql_migration]` + runner |
| 31 views | 0 | 0 | `[sql_view]` + `_create_view` (linq-based DDL, raw-SQL escape) + `drop_view_if_exists` |
| 32 sql_functions | 0 | 0 | `sqlite/sqlite_boost::register_function` (SQLite-only, scalar-only) |
| 33 pragma | 0 | 0 | `sqlite/sqlite_boost::set_pragma` × 3 + `apply_recommended_pragmas` (SQLite-only) |
| 34 backup_vacuum | 0 | 0 | `sqlite/sqlite_boost::backup_to` / `vacuum` / `vacuum_into` / `integrity_check` / `quick_check` / `optimize` (SQLite-only) |
| 36 attach | 0 | 0 | `sqlite/sqlite_boost::attach` / `detach` / `with_attached` / `with_schema` + `SqlRunner.schema_name` (SQLite-only) |
| 37 json | 0 | 0 | `@sql_json` / `@sql_blob` annotations + `[sql_table]` adapter-pair codegen + `_sql` JSON-path walker rule (abstract layer) |
| 39 fts5 | 0 | 0 | `_sql` string-predicate rules (LIKE + MATCH + escape) + `[sql_fts5]` DDL + `@sql_fts_rank` (FTS5 in `sqlite/sqlite_boost`); **Phase 0.4:** `text_match` in strings_boost |
| **Total** | **2 fns** | **9 macros** | (emitter complexity accumulates; strings_boost gains `text_match`) |

### 40-concurrency — noted, no API impact (2026-04-24)

Concurrency is a **tutorial-writing task**, not an API surface.
Every bullet in the API_MISSING.md § 40 strawman maps back to an
existing lock or a language-level rule — retry logic in
particular doesn't belong in any specific module, and isolating
it as a helper isn't worth the surface area.

Mapping each strawman concern to current state:

- **Per-thread runners.** `new_thread() <| @ { with_sqlite(...)
  <| $(db) { … } }` is the pattern — each thread opens its own
  connection via the already-locked `with_sqlite` (tut 15+). No
  new API.
- **WAL mode for read concurrency.** Covered by tut 33's
  `apply_recommended_pragmas` (`journal_mode = WAL`).
- **`busy_timeout` knob.** Covered by tut 33's
  `set_pragma("busy_timeout", "5000")` — same surface as the
  other SQLite pragmas.
- **SQLITE_BUSY retry.** Deliberately not a shipped helper. A
  retry loop is five lines of user code around
  `try_transaction` (tut 12-14); the right backoff, jitter, and
  max-attempts values are project-specific. A
  `with_retry_on_busy(...)` helper would either lock users into
  one policy or grow a parameter zoo. If a pattern emerges in
  practice we add it; until then, users write the loop. Retry
  doesn't obviously belong to `sqlite/sqlite_boost`, `daslib/sql`,
  or even dasSQLITE — its natural home is closer to
  `daslib/jobque` or user code.
- **`SqlRunner` cross-context safety.** This is a **language-level
  rule**, not a new API: `SqlRunner` holds heap-resident state
  (statement cache, bind buffers, C handle) and follows the
  standard daslang rule that context-bound data cannot cross
  `new_thread` boundaries. Tutorial spells it out with a
  don't-do-this example; no runtime-check surface is added
  because the existing context isolation already makes the
  misuse fail at the first heap dereference.
- **Connection pooling.** Defer to networked providers. SQLite
  `open` is cheap, runner holds little state — pooling is a
  PG/MySQL/MSSQL concern flagged in the forward-looking section
  below.
- **Async story.** Already flagged under "Sync vs. async" in the
  forward-looking section. No shape commitment until a second
  backend prototype exists.

**No mockup, no decision section, running totals unchanged.**
Flagged as a post-rework tutorial-writing task — concurrency is
a great tutorial (WAL + per-thread runners + retry pattern +
cross-context rule, all in one place) even though the API is
already there.

### 41-triggers — noted, no API impact (2026-04-24)

Triggers are DB-level — `CREATE TRIGGER` lives in the database,
fires transparently on INSERT / UPDATE / DELETE, and has no
daslang-side hook. Fourth consecutive noted-no-API (35 / 38 /
40 / 41).

**What the tutorial teaches:**

- Raw `exec("CREATE TRIGGER …")` is the surface; no LINQ-style
  DDL is planned or useful. Triggers are by-definition
  SQL-only constructs.
- Triggers fire **transparently** through `[sql_table]`-
  generated mutations — `db |> delete(User(Id=42))` honours any
  trigger on the `Users` table. This is the question users
  actually want answered.
- `OLD` / `NEW` pseudo-tables are SQLite-specific spelling; a
  brief mention so porters from other DBs know their dialect
  may differ.

**Why no API.** Any typed-over-trigger surface would either
duplicate raw `exec` (just prettier) or require the `_sql`
translator to model trigger bodies — which means modelling
OLD/NEW pseudo-tables, per-row vs. per-statement semantics, and
the BEGIN … END action list. Well outside dasSQLITE v1 scope,
and the cost-benefit is poor: legacy-schema porting is the
primary use case, and those schemas already have the trigger
SQL written.

**No mockup, no decision section, running totals unchanged.**
Final tutorial of the pre-plan walkthrough. Post-rework
tutorial-writing task: "yes, your triggers fire correctly
through the daslang API" in one short page, with `OLD`/`NEW`
+ audit-log example from API_MISSING § 41.

## Forward-looking: SQL runner / DB abstraction layer

**Flagged 2026-04-23.** Scope for *this* rework is dasSQLITE only, but the
LINQ-translation half of the design (`[sql_table]` structure macro, `_sql`
call macro, `select_from` / `_where` / `_order_by` / `_select`, the
`query_one` / `query_scalar` / `insert` / `exec` family) is intentionally
backend-agnostic. Only the emitter below the macros and the thin C binding
at the bottom are SQLite-specific.

**EF Core is the model.** `DbContext` + `DbSet<T>` is the user-facing
abstraction; `UseSqlite()` / `UseNpgsql()` / `UseSqlServer()` plugs in a
provider. Tables, LINQ, queries, transactions are the same API across
backends — the provider handles dialect, placeholder style, type mapping.
dasSQLITE's `_sql` macro is already structured like this: it builds a SQL
string + bind-param vector from the expression tree; only the final
emission step cares which DB is on the other end.

**What the abstraction layer would look like** (not for this PR):

- `modules/dasSQL/` (or similar) — the abstract layer: `SqlRunner` handle,
  the `_sql` macro, `[sql_table]`, all query helpers.
- `modules/dasSQLITE/` — becomes a **provider**: exposes
  `open_sqlite(path) : SqlRunner`, implements the runner interface
  against the libsqlite3 C binding.
- Future providers — `dasPGSQL`, `dasMSSQL`, `dasMySQL` — same interface,
  different emitter + wire protocol + dialect table.

**Dialect surface that needs abstraction:**

- Placeholder style (`?` / `:name` / `@name` / `$1`) — already handled by
  the macro emitting whatever the provider declares.
- SQL type names (`INTEGER` / `INT`, `TEXT` / `VARCHAR(n)`, `REAL` /
  `DOUBLE`, `BLOB` / `BYTEA`) — per-provider type table keyed on daslang
  type.
- `AUTOINCREMENT` keyword shape (SQLite `INTEGER PRIMARY KEY`, PG
  `SERIAL` / `GENERATED ALWAYS AS IDENTITY`, MySQL `AUTO_INCREMENT`,
  MSSQL `IDENTITY(1,1)`) — per-provider DDL fragment.
- Identifier quoting (`"..."` ANSI / \`...\` MySQL / `[...]` MSSQL) — per-
  provider quoter fn.
- `LIMIT n` vs `TOP n` vs `FETCH NEXT n ROWS ONLY` — per-provider
  terminal-operator emission.
- `RETURNING` support (SQLite 3.35+, PG always, MySQL never) — per-
  provider capability bit; `insert` returning rowid works via
  `last_insert_rowid()` on backends without RETURNING.

**Sync vs. async.** SQLite embedded is synchronous (no I/O wait beyond
disk). Networked providers (PG, MySQL, MSSQL) are inherently async.
daslang has channels / jobque / streams; the async story probably looks
like `runner |> query(...) |> future.get()` or the runner exposes a
`stream<T>` directly. Out of scope to pin down until we have a second
backend in hand.

**Not blocking this rework.** dasSQLITE lands first, exercises the macro
machinery, ships. Abstraction layer is a subsequent pass — once we've
lived with the API for a bit and know which seams matter in practice.
Flagging it now so the current design doesn't bake in SQLite-specific
choices that a future abstraction can't escape (e.g. per-provider
dialect table has to be a real thing the macro reads, not a bunch of
`"?"` literals scattered through `_sql`'s emission code).

**Likely memory pointer:** keep this aspirational; no API surface here
yet, just a shape commitment.

### Module layout (locked 2026-04-23)

Decision: **physical files stay in `modules/dasSQLITE/` for now; the
`require` namespace already reflects the future split.** Users write
`require` lines against the final taxonomy today — when we eventually
move files to `modules/dasSQL/`, user code is unchanged.

Three namespaces:

| require path | Contents | Future physical location |
|---|---|---|
| `daslib/sql` (or similar — TBD) | Abstract layer: `SqlRunner` handle type, `_sql` call macro, `[sql_table]` structure macro, `select_from` / `_where` / `_order_by` / `_select`, typed query helpers (`query_one` / `query_scalar` / `insert` / `exec` + their `_opt` / `try_` variants), `Option`/`Result` integration, provider registration API. | `modules/dasSQL/` |
| `daslib/sql_migrate` | **Optional** migration MVP (tut 30): `[sql_migration(version=N)]` annotation, `migrate_to_latest(db)` / `try_migrate_to_latest(db)` runners, `current_schema_version` / `pending_migrations` / `migration_history` inspectors, `__schema_version` tracking table. Up-only, raw-SQL-inside. Not required unless the user wants migrations. | `modules/dasSQL/migrate/` |
| `sqlite/sqlite_boost` | SQLite-specific provider: `open_sqlite(path) : SqlRunner`, `with_sqlite(path) <| $(runner) { … }` (tbd — see naming Q below), SQLite dialect table, runner interface impl, `sqlite_version()`. Plus unchanged: `sqlite/sqlite` raw C binding. | `modules/dasSQLITE/` (stays) |

A tutorial's typical top:

```das
require daslib/sql
require sqlite/sqlite_boost
```

The abstract-layer `require` is universal; the provider one swaps per
backend. Future: `require postgres/postgres_boost` etc., same upper
require.

**Naming locked 2026-04-23:** per-provider entry-point names,
`with_sqlite(path) <| $(db : SqlRunner) { … }` and
`open_sqlite(path) : SqlRunner` (plus `try_open_sqlite(...) :
Result<SqlRunner, string>`). No generic `with_db` / `open_db` — each
provider ships its own `with_<name>` / `open_<name>` (future
`with_postgres`, `with_mysql`, etc.). Variable name in the block
callback stays colloquial (`$(db)`) — that's just a local name, the
type is `SqlRunner`.

Mockups swept 2026-04-23 to reflect the chosen names and the
`require daslib/sql + require sqlite/sqlite_boost` pair. Diagnostics
about `daslib/sql` not being found are expected — these files are
design artifacts, not compilable.

## Polishing phase / post-MVP follow-ups

Items deliberately deferred past the MVP cut. Pick up once the read /
write paths and the runner abstraction are stable.

### Expand `text_match` to the full FTS5 grammar

[strings_boost.das `text_match`](../../daslib/strings_boost.das) ships
the FTS5 subset described in [lock 3](#L3606-L3613) — whitespace-AND,
`*` prefix, ASCII case-fold, byte-level word boundaries. Anything
beyond that panics at the user with "use SQL backend for the full
grammar". That keeps the daslib piece small and the SQL routing rule
simple, but it leaves a usability hole: any chain that runs without
`_sql` (debug, in-memory test, non-SQLite provider) hits the panic on
real-world FTS5 queries.

The polish step is to grow the in-memory matcher to cover the full
FTS5 query language — quoted phrases (`"a b"`), explicit
`AND`/`OR`/`NOT` (with precedence), `NEAR(... , k)` proximity, column
filters (`col:term`, `{col1 col2}:term`), and parentheses for grouping.
At that size the implementation no longer fits cleanly next to
`glob_match` — it's a small parser plus a query evaluator. Likely
shape:

- **Extract to its own module:** `daslib/fts5_query` (or `daslib/text_match`
  if the name reads better) exposing the public `text_match` plus a
  parsed-query type and a `compile_text_match(query) : FTS5Query`
  helper for callers that match the same query against many texts.
- **`strings_boost.text_match` becomes a thin re-export** to keep
  existing `require daslib/strings_boost` callers working.
- **Tokenizer:** ICU-style Unicode word boundaries instead of the
  current byte-level approximation. The existing implementation passes
  through high-bit bytes; a polished one handles CJK / combining
  marks correctly.
- **Tests:** port + extend the FTS5 query-grammar conformance suite
  from SQLite's own tests as a starting set.

The `_sql` translator side stays unchanged — `text_match` on an
`[sql_fts5]` column still emits `col MATCH ?` and ships the user's
query string to SQLite verbatim. The full-grammar in-memory matcher
just removes the panic when those same chains run without `_sql`.

## Plan (to be written after all tutorials are reviewed)

_TBD — this section is filled in once we have notes from every tutorial._
