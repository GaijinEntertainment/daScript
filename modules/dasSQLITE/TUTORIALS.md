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
   iterator result, `_to_array()` terminal. First exposure to `_sql(...)`
   — deliberate: the macro is *the* read-side story and appears in every
   subsequent read tutorial. Mockup: [04-select_all.das](tutorial-mockup/04-select_all.das.mockup).

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
   full translation-failure policy. Mockup: **none yet — write new**.
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
10. **`_order_by` / `_then_by`** — single- and multi-column ordering,
    ASC via `_order_by(expr)` / DESC via `_order_by_descending(expr)`.
    Covers API_MISSING §20. Mockup: **none yet — write new**.
11. **`_take` / `_skip` + keyset pagination** — offset/limit pagination
    and the faster keyset variant. Covers API_MISSING §21. Mockup:
    **none yet — write new**.
12. **`_distinct` + set operations** — DISTINCT, UNION / INTERSECT /
    EXCEPT. Covers API_MISSING §22. Mockup: **none yet — write new**.
13. **Aggregates — `_count` / `_sum` / `_avg` / `_min` / `_max`** — terminal
    aggregates on the whole source. Establishes the primitives group-by
    builds on. Covers API_MISSING §18. Mockup: **none yet — write new**.
14. **`_group_by` + `_having`** — aggregate per bucket, post-aggregate
    filter. Introduces: `_group_by` / `_group_by_lazy`, fusion with the
    next `_select`, HAVING emission. Mockup: [19-group_by.das](tutorial-mockup/19-group_by.das.mockup).
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
    instead of nullable pointers for SQL columns. Mockup: [25-null_handling.das](tutorial-mockup/25-null_handling.das.mockup).

## Part 4 — Writes

The read story is complete. Now the write side.

19. **UPDATE** — `db |> update(row)` for whole-row,
    `_sql(..._update(...))` for partial/filtered updates, the
    `_update_returning` / `update_returning` variants for RETURNING
    clauses, `db |> changes()` for rows-affected counts. Introduces
    the strict / `try_update` / `try_update_returning` fan-out on the
    write side. Mockup: [15-update.das](tutorial-mockup/15-update.das.mockup).
20. **DELETE** — `db |> delete_(row)` by PK,
    `db |> delete_by_id(type<T>, id)` for the PK-only shortcut,
    `_sql(..._delete())` for filtered deletes, `_delete_returning` /
    `delete_returning` for RETURNING clauses, `db |> changes()` for
    rows-affected counts. Strict / `try_delete_` /
    `try_delete_by_id` / `try_delete_returning` fan-out. Mockup:
    [16-delete.das](tutorial-mockup/16-delete.das.mockup).
21. **UPSERT — INSERT ON CONFLICT** — `db |> upsert(row, on_conflict=…,
    do_update=…)` and the `array<T>` overload; `insert_or_ignore` /
    `insert_or_replace` as the no-update / clobber shortcuts;
    `upsert_returning` / `_upsert_returning` for RETURNING; the
    `@sql_unique` field annotation for declaring conflict keys without
    a full `[sql_index(unique=true)]`; strict / `try_insert_or_ignore`
    / `try_insert_or_replace` / `try_upsert` / `try_upsert_returning`
    fan-out. Mockup: [17-upsert.das](tutorial-mockup/17-upsert.das.mockup).
22. **Transactions** — `db |> with_transaction() <| $ { … }` block,
    `with_transaction(mode=…)` for IMMEDIATE / EXCLUSIVE, rollback on
    panic/early return, nested transaction behavior, autocommit
    default, `db |> in_transaction()` introspection, `try_transaction`
    for explicit error handling. Inherited tutorials 12/13/14 are
    merged into one conceptual tutorial per API_REWORK decision.
    Mockup: [14-transaction.das](tutorial-mockup/14-transaction.das.mockup).

## Part 5 — Schema richness

Once CRUD and `_sql` feel natural, richer schema annotations fit in without
new syntax to teach.

23. **Foreign keys** — `@sql_references(type="T", on_delete=…)`. DDL-only
    FK constraints + CASCADE; navigation properties deliberately not
    shipped (per API_REWORK tut 26 decision). Mockup:
    [26-foreign_keys.das](tutorial-mockup/26-foreign_keys.das.mockup).
24. **Indexes** — stackable struct-level `[sql_index(fields=…,
    unique=…, name=…)]`. Emits `CREATE [UNIQUE] INDEX` per annotation;
    query side is transparent (SQLite picks indexes automatically).
    Note: `@sql_unique` (tut 21) is the field-level shortcut for a
    single-column UNIQUE constraint — use `[sql_index]` when the
    uniqueness spans multiple columns or when you want a non-unique
    index for query perf. Mockup: [27-indexes.das](tutorial-mockup/27-indexes.das.mockup).
25. **Defaults + computed columns** — `@sql_default(value=…)` /
    `@sql_default(sql_fn="CURRENT_TIMESTAMP")` / `@sql_computed(sql=…)`.
    Bind-code excludes computed fields from INSERT/UPDATE. Mockup:
    [28-defaults_computed.das](tutorial-mockup/28-defaults_computed.das.mockup).
26. **Custom type adapters** — name-based `bind_X` / `extract_X` pair;
    no registration step; `DateTime`, enums, GUIDs via this rail.
    Mockup: [29-custom_types.das](tutorial-mockup/29-custom_types.das.mockup).

## Part 6 — Non-scalar columns

BLOB and JSON are schema-rich enough to be their own section — they land
here because by now the reader understands custom-type adapters.

27. **BLOB round-trip** — insert a binary file, read it back. Merges
    inherited tutorials 07 + 08 since the API makes them a single
    round-trip story. Introduces the `@sql_blob` field annotation (for
    byte-sequence columns that shouldn't go through the custom-type
    adapter rail). Mockup: **none yet — write new** (derived from
    `tutorial/07-insert_image.das` + `tutorial/08-read_image.das`).
28. **JSON columns** — `@sql_json` field annotation, `_sql` JSON-path
    walker rule, round-tripping daslang types through SQLite JSON1.
    Mockup: [37-json.das](tutorial-mockup/37-json.das.mockup).

## Part 7 — Introspection

Schema inspection is late on purpose: most app code doesn't need it, and it
makes more sense once you know what a `[sql_table]` is.

29. **Column names** — `db |> schema("Cars")` returns a typed schema
    description. Mockup: **none yet — write new** (derived from
    `tutorial/09-column_names.das`).
30. **Listing tables** — `db |> tables()` returns the set of user tables.
    Mockup: **none yet — write new** (derived from
    `tutorial/10-list_tables.das`).

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
| 1 | Hello dasSQLITE | `01-version.das` | Has mockup |
| 2 | Declare a table, insert rows | `02-insert_data.das` | Has mockup |
| 3 | Auto-increment primary keys | `03-last_row_id.das` | Has mockup |
| 4 | Read every row | `04-select_all.das` | Has mockup |
| 5 | Parameters — positional + named | `05-parametrized.das` | Has mockup |
| 6 | Error handling — `try_` / `_opt` | `_error_handling.das` | Has mockup |
| 7 | Anatomy of `_sql` | — | **Needs mockup** |
| 8 | `_where` — predicates | — | **Needs mockup** |
| 9 | `_select` — projections | — | **Needs mockup** |
| 10 | `_order_by` / `_then_by` | — | **Needs mockup** |
| 11 | `_take` / `_skip` + keyset | — | **Needs mockup** |
| 12 | `_distinct` + set ops | — | **Needs mockup** |
| 13 | Aggregates | — | **Needs mockup** |
| 14 | `_group_by` + `_having` | `19-group_by.das` | Has mockup |
| 15 | `_join` — inner equi-join | `23-joins.das` | Has mockup |
| 16 | `_left_join` — outer | `23-joins.das` | Has mockup (shared) |
| 17 | Subqueries | `24-subqueries.das` | Has mockup |
| 18 | NULL handling — `Option<T>` | `25-null_handling.das` | Has mockup |
| 19 | UPDATE | `15-update.das` | Has mockup |
| 20 | DELETE | `16-delete.das` | Has mockup |
| 21 | UPSERT | `17-upsert.das` | Has mockup |
| 22 | Transactions | `14-transaction.das` | Has mockup |
| 23 | Foreign keys | `26-foreign_keys.das` | Has mockup |
| 24 | Indexes | `27-indexes.das` | Has mockup |
| 25 | Defaults + computed | `28-defaults_computed.das` | Has mockup |
| 26 | Custom type adapters | `29-custom_types.das` | Has mockup |
| 27 | BLOB round-trip | — | **Needs mockup** (merges inherited 07+08) |
| 28 | JSON columns | `37-json.das` | Has mockup |
| 29 | Column names | — | **Needs mockup** (from inherited 09) |
| 30 | Listing tables | — | **Needs mockup** (from inherited 10) |
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
