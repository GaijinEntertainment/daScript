# dasSQLITE Tutorials — Ordering & Coverage

Sibling to [API_REWORK.md](API_REWORK.md) and [API_MISSING.md](API_MISSING.md).

The numeric `NN-topic.das` filenames in `tutorial/` and `tutorial-mockup/` are
inherited from the zetcode C-API tutorial set. That ordering is low-level-
first (BLOB and schema introspection show up inside the basic-CRUD run, write
ops land after query power, NULLs come after JOIN). It is **not** the order a
reader should learn the rewritten API in.

This file is the **teaching order** for the rewritten surface. Each entry
describes the lesson, lists necessary concepts it introduces, and points at a
mockup file if one exists. Mockups are design artifacts under
`tutorial-mockup/` — they reference APIs that don't exist yet and don't
compile. See [API_REWORK.md](API_REWORK.md) for per-mockup decisions.

**Filename-vs-order note.** The mockup files keep their inherited `NN-` prefix
(so `git log` history tracks cleanly against the original `tutorial/` files).
When the real tutorials are written, they can be renumbered to match this
ordering — or left alone and referenced only through this index. TBD.

**Coverage guarantee.** Every decision locked in API_REWORK has a home here;
every cross-cutting concept from API_MISSING § A-E is covered in the
appendices. Nothing in the final API should land without at least one
tutorial pointing at it.

---

## Part 1 — Opening: the API shape

The first four tutorials establish the mental model: a `SqlRunner` is a
scoped resource, a table is a daslang struct with `[sql_table]`, CRUD uses
one-line helpers. Everything after this assumes this vocabulary.

1. **Hello dasSQLITE** — open the DB, read `sqlite_version()`, run one
   `query_scalar`. Introduces: `with_sqlite(path) <| $(db) { … }` RAII
   form, the `SqlRunner` handle, `daslib/sql` + `sqlite/sqlite_boost`
   require pair, `:memory:` databases. Mockup: [01-version.das](tutorial-mockup/01-version.das.mockup).
2. **Declare a table, insert rows** — define a struct with
   `[sql_table(name=…)]`, insert 8 rows with `db |> insert(row)`.
   Introduces: the `[sql_table]` structure macro, field annotations
   (`@sql_primary_key`), CREATE TABLE emission via
   `db |> create_table(type<T>)` / `try_create_table`, tear-down via
   `drop_table_if_exists(type<T>)`, one-call insert for a row and for
   an `array<T>`. Mockup: [02-insert_data.das](tutorial-mockup/02-insert_data.das.mockup).
3. **Auto-increment primary keys** — insert, read `last_insert_rowid()`,
   show INTEGER PRIMARY KEY → AUTOINCREMENT DDL mapping. Introduces:
   `db |> last_insert_rowid()`, how `@sql_primary_key` on `int` maps to
   AUTOINCREMENT. Mockup: [03-last_row_id.das](tutorial-mockup/03-last_row_id.das.mockup).
4. **Read every row** — iterate a table with
   `_sql(db |> select_from(type<Car>))`. Introduces: `select_from`, the
   `array<T>` result, `_to_array()` no-op terminal, single-column
   projection via `_select(_.Field)`, filtering with `_where(predicate)`
   (with bind capture for free variables and literals), and the
   companion `_sql_text(chain)` for inspecting the emitted SQL. First
   exposure to `_sql(...)` — deliberate: the macro is *the* read-side
   story and appears in every subsequent read tutorial. **Shipped:**
   [tutorials/sql/04-select_all.das](../../tutorials/sql/04-select_all.das)
   (chunk 2 introduced; relocated from `modules/dasSQLITE/tutorial/`
   to the project-wide `tutorials/sql/` home in chunk 2.5). Original
   mockup:
   [04-select_all.das.mockup](tutorial-mockup/04-select_all.das.mockup).

## Part 2 — Parameters + error handling rail

Everything up to here panics on failure. Before we load the reader up with
more API surface, the "what do I do when it might fail" answer needs to be
on the table.

5. **Parameters — positional and named** — bind values via variadic `?`
   args, bind by field name via named-tuple literal
   `(target=lookupId)`. Introduces: the positional/named split, the
   raw-SQL escape hatch, the named-tuple literal (already in the
   language). Mockup: [05-parametrized.das](tutorial-mockup/05-parametrized.das.mockup)
   covers both forms. (Note: inherited tutorials split these across
   05 + 06, the mockup merges them.)
6. **Error handling — strict / `try_` / `_opt`** — every panicking call
   has a non-panicking sibling: `try_foo` returns `Result<T, string>`
   for write-side errors, `foo_opt` returns `Option<T>` for read-side
   empty. Introduces: the naming convention, `daslib/option` +
   `daslib/result`, Result / Option methods (`is_ok` / `is_err` /
   `unwrap` / `unwrap_err` / `is_some` / `is_none` / `unwrap_or` /
   `map` / `and_then` / `if_ok` / `if_err` / `if_some` /
   `to_option`), the `_try_sql(...)` chain variant that returns
   `Result<T, string>` instead of panicking (used in tuts 14/15/17/18),
   when to reach for each form. Mockup:
   [_error_handling.das](tutorial-mockup/_error_handling.das.mockup).

## Part 3 — The `_sql(...)` LINQ-to-SQL chain

This is the flagship. `_sql` takes a daslib/linq-shaped chain and emits SQL
+ bind params. Every subsection is a chain stage; every mockup inside this
part re-uses the same [sql_table] from Part 1.

7. **Anatomy of `_sql`** — what it translates, what it refuses, what the
   generated SQL looks like. Introduces: the translation boundary
   (captured vars → bind params, column access → identifiers,
   untranslatable → compile error), how to inspect the emitted SQL,
   the `options log` story for debugging, the raw-SQL escape hatch
   via `db |> exec(sql, …)` / `try_exec` for DDL and for statements
   the macro can't (or shouldn't) translate. Appendix C documents the
   full translation-failure policy. **Shipped:**
   [tutorials/sql/07-anatomy.das](../../tutorials/sql/07-anatomy.das)
   (chunk 4).
8. **`_where` — predicates** — filtering with captured-variable bind
   params. Introduces: the `_where` block macro, the captured-var-to-
   bind rewriter, `_.Col` column sugar, translatable string predicates
   (`|> starts_with(x)`, `|> ends_with(x)`, `|> contains(x)`) from
   appendix A. Covers API_MISSING §5's filtering story in LINQ terms.
   Mockup: **none yet — write new**.
9. **`_select` — projections** — project to a named-tuple literal
   `(Name=_.Name, Price=_.Price)` or to an existing struct type.
   Introduces: named-tuple projections, how `_sql` passes tuple field
   names through to column aliases. Mockup: **none yet — write new**.
10. **`_order_by` / `_order_by_descending`** — single-column ordering
    via `_.Field`, multi-column via tuple-key `(_.k1, _.k2)`. Mixed
    ASC/DESC across columns and a multi-key `_then_by` protocol are
    deferred (D1/D2 in the chunk-4 plan). Covers API_MISSING §20.
    **Shipped:** [tutorials/sql/10-order_by.das](../../tutorials/sql/10-order_by.das) (chunk 4).
11. **`take` / `skip` (LIMIT / OFFSET)** — offset/limit pagination via
    the linq.das primitives, no `_`-prefixed wrappers needed. Bind
    ordering: WHERE binds first, then HAVING, then LIMIT/OFFSET.
    Single-row terminals (`_first`/`_first_opt`) override `take`.
    Keyset pagination is a future concept tut. Covers API_MISSING §21.
    **Shipped:** [tutorials/sql/11-take_skip.das](../../tutorials/sql/11-take_skip.das) (chunk 4).
12. **`distinct`** — DISTINCT row dedupe. Set operations (UNION /
    INTERSECT / EXCEPT) **deferred to chunk 5** alongside joins and
    subqueries — they need a multi-source emitter. Covers API_MISSING §22.
    **Shipped:** [tutorials/sql/12-distinct.das](../../tutorials/sql/12-distinct.das) (chunk 4).
13. **Aggregates — `count` / `sum` / `average` / `min` / `max`** —
    terminal aggregates on the whole source. `count` stands alone;
    the four column-driven aggregates compose with `_select(_.Col)`
    upstream. AVG promotes to `double`; SUM/MIN/MAX inherit the
    column type. Covers API_MISSING §18.
    **Shipped:** [tutorials/sql/13-aggregates.das](../../tutorials/sql/13-aggregates.das) (chunk 4).
14. **`_group_by` + `_having`** — aggregate per bucket, post-aggregate
    filter. Group rows surface as IGrouping-shaped tuples (`_._0` =
    key, `_._1` = group rows); aggregates over the group use
    `_._1 |> length` / `_._1 |> select($(u : T) => u.X) |> sum`
    (etc.). The chunk-4 mockup `19-group_by.das.mockup` was based on
    aspirational `_count_all()` / `_sum(_.X)` / `_avg(_.X)` syntax
    that does not typecheck after `_group_by` lowers — see
    API_REWORK §19 for the IGrouping-shape lock-in. **Shipped:**
    [tutorials/sql/14-group_by.das](../../tutorials/sql/14-group_by.das) (chunk 4).
15. **`_join` — inner equi-join** — join two tables on `(l, r) => l.X == r.Y`.
    Introduces: the equi-join-only constraint, `into` projection. Mockup:
    [23-joins.das](tutorial-mockup/23-joins.das.mockup) (first half).
16. **`_left_join` — outer with `Option<TB>`** — left outer join; missing
    right side comes back as `Option::none`. Introduces: `Option<TB>`
    flowing through `into` projection, why we don't ship full/cross/right
    joins. Mockup: [23-joins.das](tutorial-mockup/23-joins.das.mockup) (second half).
17. **Subqueries — `_in` / `_not_in` / `_any` / `_none`** — EXISTS /
    NOT EXISTS / IN / NOT IN, correlated and uncorrelated. Introduces:
    why negated names exist (`!expr` isn't AST-walkable — see
    API_REWORK pattern note). Mockup: [24-subqueries.das](tutorial-mockup/24-subqueries.das.mockup).
18. **NULL handling — `Option<T>` everywhere** — nullable columns typed
    as `Option<T>` in the struct, `_.Col |> is_some()` / `is_none()` in
    predicates emit IS NOT NULL / IS NULL, `|> unwrap_or(x)` emits
    COALESCE, `Option<T>` carries through projections to result tuples.
    Introduces: three-valued logic, why daslang uses `Option<T>`
    instead of nullable pointers for SQL columns. The original mockup
    `25-null_handling.das.mockup` shipped largely as designed; the
    `_.Col == none()` diagnostic and projection-side `unwrap_or` are
    deferred (D4 in the chunk-4 plan). **Shipped:**
    [tutorials/sql/18-null_handling.das](../../tutorials/sql/18-null_handling.das) (chunk 4).

## Part 4 — Writes

The read story is complete. Now the write side.

19. **UPDATE** — `db |> update(row)` for whole-row by-PK,
    `_sql_update(type<T>, where, set)` for predicate-based bulk updates,
    `_sql_update_returning` for RETURNING, `db |> changes()` for
    rows-affected counts. Macro form carries the `_sql_` prefix so SQL
    provenance stays visible at the call site. Strict / `try_update` /
    `_sql_try_update` / `*_returning` / `_sql_try_*_returning` fan-out
    on the write side. **Shipped:**
    [tutorials/sql/19-update.das](../../tutorials/sql/19-update.das)
    (chunk 6). Original mockup:
    [15-update.das.mockup](tutorial-mockup/15-update.das.mockup).
20. **DELETE** — `db |> delete_(row)` by PK (trailing underscore: `delete`
    is a daslang keyword), `db |> delete_by_id(type<T>, id)` for the
    PK-only shortcut, `_sql_delete(type<T>, where)` for predicate-based
    bulk deletes, `_sql_delete_returning` for RETURNING, `db |> changes()`
    for rows-affected counts. Strict / `try_delete_` / `try_delete_by_id`
    / `_sql_try_delete` / `*_returning` / `_sql_try_*_returning` fan-out.
    **Shipped:**
    [tutorials/sql/20-delete.das](../../tutorials/sql/20-delete.das)
    (chunk 6). Original mockup:
    [16-delete.das.mockup](tutorial-mockup/16-delete.das.mockup).
21. **UPSERT — INSERT ON CONFLICT** — `_sql_upsert(row, on_conflict,
    do_update)` macro for the proper merge form (`ON CONFLICT(...) DO
    UPDATE SET ...`); `_excluded.Col` for the proposed-row sentinel;
    single-column (`_.Id`) and composite (`tuple(_.A, _.B)`) conflict
    targets; `_sql_upsert_returning` for capturing the post-merge row;
    `try_` siblings for non-panic flow. Plain functions
    `insert_or_ignore` / `insert_or_replace` (single + bulk + `try_`)
    for the simpler "ignore conflict" / "wipe-and-reinsert" cases. The
    `@sql_unique` field annotation declares a single-column UNIQUE for
    the conflict key; composite uniqueness uses `[sql_index(unique =
    true, fields = (...))]` (tut 24). **Shipped:**
    [tutorials/sql/21-upsert.das](../../tutorials/sql/21-upsert.das)
    (chunk 7). Original mockup:
    [17-upsert.das.mockup](tutorial-mockup/17-upsert.das.mockup).
22. **Transactions** — `db |> with_transaction() { … }` block,
    `with_transaction(mode=…)` for IMMEDIATE / EXCLUSIVE, rollback on
    panic/early return, nested transaction behavior (automatic
    SAVEPOINT fallback via `das_sp`), autocommit default,
    `db |> in_transaction()` introspection, `try_transaction` for
    explicit error handling. Inherited tutorials 12/13/14 are merged
    into one conceptual tutorial per API_REWORK decision. **Shipped:**
    [tutorials/sql/22-transactions.das](../../tutorials/sql/22-transactions.das)
    (chunk 6). Original mockup:
    [14-transaction.das.mockup](tutorial-mockup/14-transaction.das.mockup).

## Part 5 — Schema richness

Once CRUD and `_sql` feel natural, richer schema annotations fit in without
new syntax to teach.

23. **Foreign keys** — per-field decorators `@sql_references = "Parent"`
    + optional `@sql_on_delete` / `@sql_on_update` (cascade / set_null /
    set_default / restrict / no_action). DDL-only FK constraints +
    CASCADE; navigation properties deliberately not shipped (per
    API_REWORK tut 26 decision). `with_sqlite` enables `PRAGMA
    foreign_keys = ON` on every connection so the constraints actually
    fire. **Shipped:**
    [tutorials/sql/23-foreign_keys.das](../../tutorials/sql/23-foreign_keys.das)
    (chunk 7). Original mockup:
    [26-foreign_keys.das.mockup](tutorial-mockup/26-foreign_keys.das.mockup).
24. **Indexes** — sibling annotation `[sql_table, sql_index(fields = ...,
    unique = ..., name = ...)]` (must live in the same bracket as
    `[sql_table]`, with `[sql_table]` first). Emits `CREATE [UNIQUE]
    INDEX` per annotation; query side is transparent (SQLite picks
    indexes automatically). Auto-naming is `idx_<table>_<col1>_<col2>`
    when `name` is omitted. Composite UNIQUE via `[sql_index(unique =
    true, fields = (...))]` is the prerequisite for upsert composite-
    conflict targets. **Shipped:**
    [tutorials/sql/24-indexes.das](../../tutorials/sql/24-indexes.das)
    (chunk 7). Original mockup:
    [27-indexes.das.mockup](tutorial-mockup/27-indexes.das.mockup).
25. **Defaults + computed columns** — three default sources: native
    daslang field initializer (`Active : bool = true` becomes
    `DEFAULT 1`), `@sql_default_fn = "CURRENT_TIMESTAMP"` for SQLite
    built-ins (CURRENT_TIMESTAMP / CURRENT_DATE / CURRENT_TIME), and
    `@sql_computed = "expression"` for generated columns (default
    VIRTUAL; add `@sql_stored = true` for STORED). Bind-code excludes
    computed fields from INSERT/UPDATE; SELECT reads them as ordinary
    columns. **Shipped:**
    [tutorials/sql/25-defaults_computed.das](../../tutorials/sql/25-defaults_computed.das)
    (chunk 7). Original mockup:
    [28-defaults_computed.das.mockup](tutorial-mockup/28-defaults_computed.das.mockup).
26. **Custom type adapters** — name-based `_::sql_bind` / `_::sql_extract`
    overload pair, no registration step; four primitive storage types
    (INTEGER / REAL / TEXT / BLOB) selected by the `sql_bind` return
    type; `DateTime`, enums, GUIDs via this rail. Tutorial:
    [26-custom_types.das](../../tutorials/sql/26-custom_types.das).
    Original mockup: [29-custom_types.das](tutorial-mockup/29-custom_types.das.mockup).

## Part 6 — Non-scalar columns

BLOB and JSON are schema-rich enough to be their own section — they land
here because by now the reader understands custom-type adapters.

27. **BLOB round-trip** — insert a binary file, read it back. Merges
    inherited tutorials 07 + 08 since the API makes them a single
    round-trip story. `array<uint8>` is one of the four primitive
    storage types from the tut-26 adapter rail, so a `[sql_table]`
    field of that type round-trips through a BLOB column with no
    extra annotation. Tutorial:
    [27-blob.das](../../tutorials/sql/27-blob.das). The
    `@sql_blob` annotation for opaque struct serialization is a
    different concept and lands with tut 28 (JSON / serialization).
28. **JSON columns** — `@sql_json` and `@sql_blob` field annotations,
    `_sql` walker descent into JSON paths (pred-side and projection-
    side, arbitrary depth), opaque archive round-trip for `@sql_blob`.
    **Shipped:** [tutorials/sql/28-json.das](../../tutorials/sql/28-json.das).
    Original mockup: [37-json.das](tutorial-mockup/37-json.das.mockup).

## Part 7 — Introspection

Schema inspection is late on purpose: most app code doesn't need it, and it
makes more sense once you know what a `[sql_table]` is.

29. **Column metadata** — Band 1 `column_info(type<T>)` returns
    `array<ColumnInfo>` baked at `[sql_table]` expansion time;
    `sqlite_sql_type` renders the abstract `SqlType` enum to the
    SQLite dialect. Band 3 covers raw `PRAGMA table_info` via the
    typed `query` family for genuinely-dynamic introspection.
    **Shipped:** [tutorials/sql/29-column_names.das](../../tutorials/sql/29-column_names.das).
30. **Listing tables** — raw `query("SELECT … FROM sqlite_master …",
    type<MasterRow>)` for catalog inspection. No new abstract API
    (catalog spelling is genuinely provider-specific). **Shipped:**
    [tutorials/sql/30-list_tables.das](../../tutorials/sql/30-list_tables.das).

## Part 8 — Operations

Everything a real app needs once it leaves the developer's machine.

31. **Views** — `[sql_view]` struct + SQL string, read-only `[sql_table]`;
    DDL managed by migrations. Mockup: [31-views.das](tutorial-mockup/31-views.das.mockup).
32. **Migrations** — `[sql_migration(version=N)]` + `migrate_to_latest(db)`;
    up-only, versioned, tracked in `__schema_version`. Ships in
    **optional** `daslib/sql_migrate` sub-module. Mockup:
    [30-migrations.das](tutorial-mockup/30-migrations.das.mockup).
33. **PRAGMA tuning** — WAL mode, `busy_timeout`, `foreign_keys`,
    `synchronous`. Ad-hoc `db |> set_pragma(name, value)` /
    `try_set_pragma`, the batch shortcut
    `db |> apply_recommended_pragmas()` /
    `try_apply_recommended_pragmas` that sets a sensible app default,
    `query_scalar` on `PRAGMA foo` for read-side inspection. Mockup:
    [33-pragma.das](tutorial-mockup/33-pragma.das.mockup).
34. **Backup + VACUUM + integrity check** — online backup to another
    runner, `VACUUM`, `PRAGMA integrity_check`. Mockup:
    [34-backup_vacuum.das](tutorial-mockup/34-backup_vacuum.das.mockup).
35. **ATTACH DATABASE** — cross-DB queries with runner-wrapper schema
    qualification. Introduces `db |> attach(path, as_name)` /
    `try_attach`, `db |> detach(name)` / `try_detach`, the scoped
    `db |> with_attached(path, as_name) <| $(db2) { … }` block for
    attach-then-auto-detach, and `db |> with_schema(name)` to return a
    runner wrapper that qualifies all subsequent queries against the
    named attached schema. Mockup: [36-attach.das](tutorial-mockup/36-attach.das.mockup).

## Part 9 — Performance

The "my dataset got big" chapter. Every tutorial here was marked "no API
impact" during the design walk — they're purely concept + existing API.

36. **Streaming results** — iterate large result sets without
    materializing into `array<T>`, using the bare iterator that
    `_sql(...)` returns before a terminal. Concept-only; no new API.
    Per API_REWORK §35. Mockup: **no API impact — concept tutorial**.
37. **Bulk operations** — large-batch insert, `INSERT … SELECT`, bulk
    update. Concept + `insert_many` / transaction patterns. Per
    API_REWORK §38. Mockup: **no API impact — concept tutorial**.
38. **Concurrency** — threading, per-context runners, busy retry,
    connection-per-thread policy. Per API_REWORK §40. Mockup: **no API
    impact — concept tutorial**.

## Part 10 — Advanced

Niche, but tractable once the base is down.

39. **User-defined SQL scalar functions** — register a daslang fn as a
    SQL fn via `sqlite3_create_function`. SQLite-provider-specific
    (other backends need their own registration rail). Mockup:
    [32-sql_functions.das](tutorial-mockup/32-sql_functions.das.mockup).
40. **FTS5 full-text search** — `[sql_fts5]` table, `text_match(col, q)`
    in predicates → `col MATCH ?`. Covers the shared in-memory
    `text_match` from Phase 0.4. Mockup: [39-fts5.das](tutorial-mockup/39-fts5.das.mockup).
41. **Triggers** — DB-level callbacks via raw SQL inside migrations.
    Deliberately *no* daslang-side trigger DSL. Per API_REWORK §41.
    Mockup: **no API impact — concept tutorial**.

---

## Concept appendices

Not tutorials (no `.das` file). Reference docs that cross-cut the above.
These live here rather than in API_REWORK / API_MISSING because they're
reader-facing, not design-facing.

A. **The SQL function table** — which daslang / daslib calls the `_sql`
   emitter knows how to translate: `|> starts_with(x)`, `|> contains(x)`,
   `|> to_lower()`, `length(s)`, `|> abs()`, `floor`, `min`, `max`,
   date/time functions. Per-function: daslang signature, emitted SQL,
   per-provider notes. Source: API_MISSING § A.
B. **Cross-provider portability of annotations** — which `@sql_*`
   annotations are standard across providers vs. SQLite-only (e.g.
   `@sql_computed` needs a per-provider shape for generated-columns
   support). Source: API_MISSING § B.
C. **The translation-failure boundary** — when `_sql(...)` refuses to
   translate and what the user does: the compile error points at the
   offending expression, the fix is either rewrite the LINQ in a
   translatable shape or drop to raw SQL via `query_one` / `query_many`
   / `exec`. Source: API_MISSING § C.
D. **Module layout — `daslib/sql` vs. `sqlite/sqlite_boost`** — the
   abstract-layer / provider split that lets user code stay the same
   when a future provider (`postgres/postgres_boost` etc.) lands.
   Source: API_REWORK § Module layout.
E. **Forward-looking: `dasSQL` abstraction layer** — the roadmap beyond
   this rework. Not deliverable in this PR; documented so the current
   design doesn't bake in SQLite-specific choices that a future
   abstraction can't escape. Source: API_REWORK § Forward-looking.

---

## Tutorial status summary

| # | Title | Mockup | Status |
|---|---|---|---|
| 1 | Hello dasSQLITE | `01-version.das` | **Shipped** (chunk 1) |
| 2 | Declare a table, insert rows | `02-insert_data.das` | **Shipped** (chunk 1) |
| 3 | Auto-increment primary keys | `03-last_row_id.das` | **Shipped** (chunk 1) |
| 4 | Read every row | `04-select_all.das` | **Shipped** (chunk 2) |
| 5 | Parameters — positional | `05-parametrized.das` | **Shipped** (chunk 3); named-tuple bind for `:name` placeholders deferred to chunk 4 |
| 6 | Error handling — `try_` / `_opt` / `_try_sql` | `_error_handling.das` | **Shipped** (chunk 3) |
| 7 | Anatomy of `_sql` | `07-anatomy.das` | **Shipped** (chunk 4) |
| 8 | `_where` — predicates + appendix-A operators | direct | **Shipped** (chunk 3) |
| 9 | `_select` — single-column + named-tuple | direct | **Shipped** (chunk 3); struct-type projection (`_select(type<T2>)`) deferred to chunk 5 |
| 10 | `_order_by` / `_order_by_descending` (+ tuple key) | `10-order_by.das` | **Shipped** (chunk 4); `_then_by` and per-column ASC/DESC mix deferred |
| 11 | `take` / `skip` (LIMIT / OFFSET) | `11-take_skip.das` | **Shipped** (chunk 4); keyset pagination is a future concept tut |
| 12 | `distinct` (set ops deferred) | `12-distinct.das` | **Shipped** (chunk 4); UNION/INTERSECT/EXCEPT punted to chunk 5 alongside joins/subqueries |
| 13 | Aggregates — `count` / `sum` / `average` / `min` / `max` | `13-aggregates.das` | **Shipped** (chunk 4) |
| 14 | `_group_by` + `_having` | `14-group_by.das` (`19-group_by.das.mockup` superseded — IGrouping shape, see API_REWORK §19) | **Shipped** (chunk 4) |
| 15 | `_join` — inner equi-join | `23-joins.das` | Has mockup |
| 16 | `_left_join` — outer | `23-joins.das` | Has mockup (shared) |
| 17 | Subqueries | `24-subqueries.das` | Has mockup |
| 18 | NULL handling — `Option<T>` | `18-null_handling.das` (`25-null_handling.das.mockup` shipped largely as designed; `_.Col == none()` diagnostic shipped chunk 8; projection-side `unwrap_or` deferred) | **Shipped** (chunk 4); `_.Col == none()` fixit added in chunk 8 |
| 19 | UPDATE | `15-update.das` | **Shipped** (chunk 6); macros named `_sql_update` / `_sql_try_update` / `_sql_update_returning` / `_sql_try_update_returning`; raw `exec` parameter binding shipped in chunk 8 |
| 20 | DELETE | `16-delete.das` | **Shipped** (chunk 6); macros named `_sql_delete` / `_sql_try_delete` / `_sql_delete_returning` / `_sql_try_delete_returning`; CASCADE FK example deferred to tut 23 |
| 21 | UPSERT | `21-upsert.das` | **Shipped** (chunk 7); `_sql_upsert` / `_sql_try_upsert` / `_sql_upsert_returning` / `_sql_try_upsert_returning`, `_excluded` sentinel, single + composite conflict targets via `tuple(_.A, _.B)`, plain-fn `insert_or_ignore` / `insert_or_replace` (single + bulk + `try_` for both) |
| 22 | Transactions | `14-transaction.das` | **Shipped** (chunk 6); two-arity overload, `SqliteTxnMode` enum (Deferred/Immediate/Exclusive), savepoint nesting via `das_sp` name |
| 23 | Foreign keys | `23-foreign_keys.das` | **Shipped** (chunk 7); `@sql_references = "Parent"` + optional `@sql_on_delete` / `@sql_on_update` (cascade / set_null / set_default / restrict / no_action); `with_sqlite` enables `PRAGMA foreign_keys = ON` |
| 24 | Indexes | `24-indexes.das` | **Shipped** (chunk 7); `[sql_table, sql_index(fields = ..., unique = ..., name = ...)]` sibling annotation, single + composite, auto-name `idx_<table>_<col1>_<col2>` |
| 25 | Defaults + computed | `25-defaults_computed.das` | **Shipped** (chunk 7); native field init for literal defaults (`Active : bool = true`), `@sql_default_fn` for SQL built-ins (CURRENT_TIMESTAMP / CURRENT_DATE / CURRENT_TIME), `@sql_computed = "expr"` (+ optional `@sql_stored = true`) |
| 26 | Custom type adapters | `26-custom_types.das` | **Shipped** (chunk 8); `_::sql_bind` / `_::sql_extract` adapter pair, four primitive storage types (INTEGER / REAL / TEXT / BLOB), enum auto-roundtrip, return-type-driven DDL, `Option<T>` over a custom adapter; per-field `@sql_as(type<P>)` override deferred |
| 27 | BLOB round-trip | `27-blob.das` | **Shipped** (chunk 8); `array<uint8>` is one of the four adapter primitives — falls out of tut 26 |
| 28 | JSON + BLOB columns | `28-json.das` | **Shipped** (chunk 9); `@sql_json` (TEXT via daslib/json + json_boost) + `@sql_blob` (BLOB via daslib/archive); `_sql` walker JSON-path descent (pred + projection, arbitrary depth) emits `json_extract`; `@sql_blob` opaque to walker (compile error on descent); adapter dedup across tables; mockup was 37-json |
| 29 | Column metadata | `29-column_names.das` | **Shipped** (chunk 9); Band 1 `column_info(type<T>) : array<ColumnInfo>` (compile-time walk) + abstract `SqlType` enum + `sqlite_sql_type` dialect renderer; Band 3 raw `PRAGMA table_info` via the typed `query` family |
| 30 | Listing tables | `30-list_tables.das` | **Shipped** (chunk 9); raw `query` against `sqlite_master`; no abstract `list_tables` helper (catalog spelling diverges per backend) |
| 31 | Views | `31-views.das` | Has mockup |
| 32 | Migrations | `30-migrations.das` | Has mockup |
| 33 | PRAGMA tuning | `33-pragma.das` | Has mockup |
| 34 | Backup + VACUUM | `34-backup_vacuum.das` | Has mockup |
| 35 | ATTACH DATABASE | `36-attach.das` | Has mockup |
| 36 | Streaming results | — | No-API — concept tutorial |
| 37 | Bulk operations | — | No-API — concept tutorial |
| 38 | Concurrency | — | No-API — concept tutorial |
| 39 | UD SQL functions | `32-sql_functions.das` | Has mockup |
| 40 | FTS5 | `39-fts5.das` | Has mockup |
| 41 | Triggers | — | No-API — concept tutorial |

**Mockup coverage:** 27 tutorials have a mockup already. 7 need a new mockup
(mostly in Part 3 — the `_sql` chain deep-dives — plus BLOB round-trip and
two introspection tutorials). 4 are no-API concept tutorials and don't get
a mockup by design. 5 concept appendices (A-E) are prose-only reference
docs.

**Inherited-file cross-reference:** see [API_REWORK.md](API_REWORK.md)'s
per-section headers (`### NN-topic` prefix) for the decision log behind
each mockup. The inherited numbering is preserved in the mockup filenames
so the decision log remains grep-able.
