# dasSQLITE Migrations — Design Notes

Working document for the migrations chunk. Sibling of
[API_REWORK.md](API_REWORK.md) (master plan), [API_MISSING.md](API_MISSING.md)
(strawman workshop), and [API_CHECKED.md](API_CHECKED.md). Decisions reached
here graduate to API_REWORK §30; loose ends stay in API_MISSING §30.

We walk one real-world migration scenario at a time, write down what the user
hits, and decide what the API should do. Tutorial 30 is the natural rendering
of these decisions.

Mockup carried over from phase 0: [tutorial-mockup/30-migrations.das.mockup](tutorial-mockup/30-migrations.das.mockup).
Locked design preserved in [API_REWORK.md §30-migrations](API_REWORK.md#L3630).

---

## Conceptual model (locked, 2026-05-04)

Captured from the discussion that opens this doc, before walking scenarios.

### The problem migrations solve

A DB file holds *schema* (tables, columns, indexes) and *data* (rows). When
v1 of the app ships, it creates the schema; users start producing data. When
v2 ships with a new `Email` column, the user's `app.db` on disk still has the
v1 schema. v2's code that queries `Email` panics on the very first call —
"no such column."

You can't unconditionally `CREATE TABLE` (table already exists for upgrades),
can't unconditionally `ALTER TABLE … ADD COLUMN Email` (no `users` table
at all for users skipping straight to v3), and can't sprinkle version
checks through app code. You need an **ordered, recorded, file-local**
record of which schema steps this particular DB has gone through, so the
runtime knows exactly which steps remain.

### What `migrate_to_latest()` does

Treats schema evolution as an ordered sequence of integer-numbered
migrations. Each call:

1. Opens the tracking table (`__schema_version`) — creates it if missing.
2. Reads the highest applied version.
3. Runs every migration with `version > current`, in ascending order, each
   in its own transaction.
4. Records each applied one in `__schema_version`.

After it returns, the DB matches what the current code expects, regardless
of where it started. Three properties make this tractable:

- **The tracking table lives inside the DB itself.** Version state travels
  with the file — copy the DB to another machine and the version comes with
  it. Nothing else has to be in sync.
- **Migrations are append-only and ordered by integer.** Once shipped, a
  migration is immutable. To change anything, write a new one with a higher
  version number.
- **Each migration body is just code** — raw SQL, daslang APIs, or both.
  "ADD COLUMN", "CREATE INDEX", "UPDATE rows", whatever it takes to advance
  one step.

### The call site

`migrate_to_latest()` runs unconditionally on every app startup, before any
other DB work. Cost on a fully-up-to-date DB is one cheap query
(`SELECT MAX(version) FROM __schema_version`) — microseconds. Safe to call
every time.

The mockup spells out the canonical form:

```das
with_sqlite("app.db") <| $(db) {
    db |> migrate_to_latest()
    // … rest of the app …
}
```

**Convenience wrapper (decided 2026-05-04):** ship `with_latest_sqlite(path)`
in `daslib/sql_migrate` that combines `with_sqlite` + `migrate_to_latest`
into one call. Less ceremony, harder to ship a binary that opens the DB
without bringing it up to date. Preferred form for app code; the explicit
two-step form remains for tools/CI that may want to inspect before
migrating.

### Append-only — the rule that makes this work

You never edit a migration after it's been released. Once `migration_002`
ships, it is frozen for life. If you decide later that `Email` should be
`UNIQUE`, write `migration_003` that does `CREATE UNIQUE INDEX …`.

This isn't aesthetic — it's mechanical. A user whose DB is already at v2
will never re-execute migration_002 on their machine. Edits to v2 won't
reach existing users; fresh installs would get the new v2 body, existing
users would have the old v2's effects baked in. The two DBs would diverge
silently.

### The struct-vs-migration disconnect

Unique to daslang's `[sql_table]` model. Your `struct User` describes the
*current* schema (e.g. v3 with `Email`). When you write `migration_001`,
you cannot say `db |> create_table(type<User>)` — that would emit the
v3 shape with `Email`, not the v1 shape without it. So `migration_001`
uses raw SQL: `db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT)")`,
hand-written, frozen forever.

EF avoids this by saving a *model snapshot* on disk per migration — a
generated file describing what the model looked like at each version.
**We deliberately don't ship that.** It would require frozen-struct tooling
daslang doesn't have, plus a code generator. The trade: migrations are
slightly more verbose (raw SQL for old shapes), but the tooling stays small.

### Struct as a partial projection (decided 2026-05-04)

A named principle, not just a side-effect:

> **The `[sql_table]` struct is a partial projection of the table, not a
> mirror of it.** The macro generates SQL from struct fields, not from
> `SELECT *`, so columns that exist in the DB but not in the struct are
> silently ignored on read and untouched on write.

Consequences:

1. **Not every migration requires a struct change.** Adding `LastLoginAt`
   for an analytics dashboard means: write the migration, ship the
   new binary (struct unchanged), the column accumulates data. Months
   later when code wants to *read* `LastLoginAt`, that's when the
   struct gets the field — by which time every DB has the column.
2. **Compile-time "struct must match latest migration" would fight the
   user.** Earlier sketch had `[sql_table(schema_from=…)]` auto-deriving
   the struct from the latest migration's DDL — that would close the
   partial-projection door. Posture: leave them decoupled. `schema_from`
   stays opt-in for users who *want* the strict pairing.
3. **Zero-downtime / rolling-deploy patterns fall out for free.** The
   standard expand-contract recipe (add nullable column → deploy code
   that uses it → tighten constraints later) just works — no special
   API needed.
4. **Destructive migrations (DROP COLUMN, NOT NULL without default,
   RENAME) break old binaries that are still running.** Same constraint
   everywhere; mention in tut 30, document the additive-only rule for
   safe rolling deploys.

Matches EF Core's entity-class-as-partial-projection model 1:1.

---

## Open questions

Held over for discussion or future work.

### Q1 — Audit table semantics: log failures or only successes? ✅ RESOLVED

Closed in Scenario 1 — option A (success-only rows). Audit table is
3 columns; the original mockup's `success` column is dropped.

### Q2 — Separate migration tool (`dasMigrate` CLI)?

EF has `dotnet ef database update`; Flyway has the Flyway CLI. The argument
for separation: CI/prod deploys want schema changes to land *before* app
processes start consuming the new schema, especially for multi-instance
rollouts (rolling deploys, blue-green). Argument against: extra moving
piece, dev workflow gets clumsier.

**Posture:** ship in-app first (the mockup's shape). Open `dasMigrate` as
future work — also dovetails with the eventual `dasSQL` abstraction layer
where a provider-neutral migration runner makes more sense.

### Q3 — Compile-time DDL emission (export migrations as plain SQL)?

What `dotnet ef migrations script` does: write migration bodies to an
`app_migrations.sql` file next to the binary so a separate tool (or a DBA)
can apply them without daslang in the loop. Useful for ops handing SQL to
DBAs and for review pipelines.

**Posture:** future work. Doesn't block v1.

### Q4 — Baseline / pre-migrations adoption ✅ RESOLVED

Closed in Scenario 5 — `baseline(db, version)` ships, plus Layer-1
proactive warning + Layer-2 enriched panic for the naive
`with_sqlite → with_latest_sqlite` swap UX.

### Q5 — Concurrent runners ✅ RESOLVED

Closed in Scenario 8 — α-shape (one transaction wraps the whole call).
Coordination via SQLite's RESERVED lock + `busy_timeout`. β
(per-migration txn + re-check) was considered and rejected.

---

## Scenario walk

Each scenario is a real situation a developer hits. We walk one at a time,
agree on the API behavior, capture it here. Test cases are planned alongside
each scenario — the **Test cases** sub-block under a scenario maps 1:1 to
files we'll add under `tests/sqlite/migrations/` when the chunk is built.

Status legend: 🟢 decided · 🟡 in discussion · ⚪ pending

### Scenario list (working order)

| # | Title | Status |
|---|-------|--------|
| 1 | Fresh DB — first ever app run | 🟢 |
| 2 | Add a column on the next release | 🟢 |
| 3 | Add an index + backfill data | 🟢 |
| 4 | Inspect, don't migrate (CLI / CI) | 🟢 |
| 5 | Existing pre-migrations DB ("baseline") | 🟢 |
| 6 | Two devs pick the same version number | 🟢 |
| 7 | Migration N fails midway | 🟢 |
| 8 | Two processes call `migrate_to_latest` at once | 🟢 |
| 9 | Rollback request from a non-DB user | 🟢 |
| 10 | DROP / RENAME column on old SQLite | 🟢 |
| 11 | Struct-to-struct convert + table rebuild | 🟢 |
| 12 | Squash / baseline old migrations (3 years in) | 🟢 |

---

### Scenario 1 — Fresh DB 🟢

**Setup.** First ever app run. `app.db` doesn't exist on disk. User starts
the binary. `with_latest_sqlite("app.db") <| $(db) { … }` opens the path
(SQLite creates the empty file), then runs `migrate_to_latest`.

**Behavior (locked 2026-05-04, amended for α-shape concurrency 2026-05-04):**

1. Eagerly create the audit table via
   `CREATE TABLE IF NOT EXISTS __schema_version (…)` — autocommit,
   outside any transaction. Idempotent across concurrent runners.
2. `BEGIN IMMEDIATE` — acquires the file-level RESERVED lock (with
   `busy_timeout`); blocks if another process is migrating.
3. Read the highest applied version from `__schema_version` (empty → 0).
4. Walk pending migrations in ascending version order.
5. For each, inside the big transaction:
   - run body (`db |> exec("CREATE TABLE users (…)")`)
   - `INSERT INTO __schema_version VALUES (N, desc, unixepoch())`
   - emit `to_log(LOG_INFO, "applying migration v{N}: {desc}")`
     (progress; "applying" tense, since the txn hasn't committed yet)
6. `COMMIT` — releases the lock; everything in steps 3–5 is durable
   atomically. The whole `migrate_to_latest` call is a single
   transaction (α-shape).
7. (Per Scenario 3) If any migration in the pending list set
   `vacuum=true` or `analyze=true`, run those AFTER COMMIT, in order
   (ANALYZE then VACUUM if both).
8. Emit `to_log(LOG_INFO, "migrate_to_latest: applied {count} migration(s)")`.
9. Return `int` count of applied migrations (`Result<int, string>` for
   `try_migrate_to_latest`).

**α vs β.** The runner uses one transaction wrapping the whole pending
list (α), not per-migration transactions (β was considered and rejected).
Scenario 8's lock has the full reasoning. Implication: failure rolls
back every migration applied in this call; re-run replays the whole
pending list. Per-migration body+audit-insert atomicity is preserved as
a consequence (both inside the same big transaction); concurrent runners
see all-or-nothing per call, which is simpler to reason about and test.

**Audit table shape (locked):**

```sql
CREATE TABLE __schema_version (
    version     INTEGER PRIMARY KEY,
    description TEXT    NOT NULL DEFAULT '',
    applied_at  INTEGER NOT NULL          -- unix epoch seconds
)
```

Success-only rows. The `success` column from the original mockup is
**dropped** — every row is a success by construction (failed migrations
roll back the audit insert atomically with their own body). Failed
migrations surface through panic / `Result::Err`, not through audit-table
state.

**Decision rationale:**

- *Eager audit table.* The tracking table is metadata, not application data;
  not worth atomicity-coupling with migration #1. Also gives inspection
  helpers (Scenario 4) a stable invariant: after `migrate_to_latest`
  returns, `__schema_version` always exists.
- *Loud-by-default logging.* Schema evolution is rare and loud-worthy; a
  hung migration is much easier to diagnose when the prior log line says
  "applied v6, starting v7."
- *Drop the `success` column.* Audit table answers "what's the current
  state"; failures are developer events for logs, not durable state.

**Test cases:**

| File | What it asserts |
|---|---|
| `01_fresh_creates_audit_table.das` | After `migrate_to_latest` on a `:memory:` DB, `__schema_version` exists with the locked 3-column shape (verify via `pragma_table_info`). |
| `02_fresh_runs_all_migrations.das` | Three test migrations registered (v1/v2/v3); after the call, `__schema_version` has exactly 3 rows with matching versions + descriptions. |
| `03_fresh_returns_count.das` | Return value equals number of applied migrations (3 here, 0 on a no-op rerun). |
| `04_fresh_runs_bodies.das` | Each migration body's effect is observable (e.g. v1 creates `users`, v2 adds `Email`, table-info reflects both). |
| `05_audit_table_eager.das` | Register zero migrations; run `migrate_to_latest`; assert `__schema_version` is created anyway. Locks "metadata, not migration-coupled." |
| `06_idempotent_rerun.das` | Call `migrate_to_latest` twice in a row; second call returns 0, audit table unchanged, no extra rows. |
| `07_applied_at_is_now.das` | `applied_at` of each inserted row falls in `[test_start, test_end]` (unix seconds; 5-sec window). |
| `08_log_lines_per_migration.das` | (If `to_log` capture is available in dastest) — assert one log line per applied migration with matching version + description. Skip if capture isn't available; the production observation suffices. |

---

### Scenario 2 — Add a column on the next release 🟢

**Setup.** User ran v1 of the app for a while; their DB has v1 applied
(`__schema_version` row `(1, '', T1)`). Developer ships v2 with one new
migration:

```das
[sql_migration(version=2, description="add email column")]
def migration_002(db : SqlRunner) {
    db |> exec("ALTER TABLE users ADD COLUMN Email TEXT NOT NULL DEFAULT ''")
}
```

User installs v2, app starts.

**Behavior.** Inherits Scenario 1's contract. The audit table already
exists, the runner reads `MAX(version) = 1`, finds v2 in the registry,
runs it inside `BEGIN/COMMIT` with the audit insert, logs, returns `1`.
Steady-state.

**Decision (locked 2026-05-04) — pending detection: strict-forward.**

> **Pending = registered versions where `version > MAX(applied)`.**

Set-difference (run anything registered-but-not-applied, EF-style) is
**not** how `migrate_to_latest` works. Reasons:

1. Mechanical match with the append-only rule. Once a DB has moved past
   version K, anything ≤ K is frozen — no retroactive migrations.
2. Predictable. `MAX(applied)` only ever monotonically increases. No
   "this column appeared even though I never ran a migration that adds
   it" surprises.
3. Branch-merge across devs is solved at the version-control layer:
   two devs writing `version=2` in parallel branches **must** conflict
   when merged (same annotation argument, same registry entry, compile
   error from the duplicate-version check). VC handles ordering; the
   runner doesn't have to.
4. Cheaper: one comparison, no set-diff edge cases.

The cost: parallel-branch dev workflow needs discipline. If Dev A's DB
ran `v3` before Dev B's `v2` got merged, that DB is in a state with
applied = `{1, 3}` and v2 registered. The runner skips v2 silently.
**Recovery is "blow away the dev DB and rebuild."** Production is
unaffected — only one migration sequence ever ships.

**Decision (locked 2026-05-04) — no `migrate_to(version=N)` in v1.**

Migrating *backward* is meaningless without rewinding the binary too —
the struct describes the latest schema; running with v=2 against a
binary expecting v=3 means code that doesn't compile or runs against a
column that's not there. Defer indefinitely; `migrate_to_version` ships
when (if) a concrete use case shows up.

**Future tool (deferred to v2) — semantic-dupe detection as a lint, not a build gate.**

`detect_duplicates` from chunk 13 is fuzzy — produces false positives.
Wrong shape for a compile-time hard error. Right shape for a PR-review
helper: a `daslib/sql_migrate_lint` (or similar) tool that scans the
migration registry and flags suspiciously-similar bodies for human
review. Useful for catching copy-paste mistakes during merges, but
the dev keeps the final call. Out of scope for v1; revisit when the
ecosystem matures.

**Decision (locked 2026-05-04) — ship typed ALTER surface for the
additive cases.**

Migration bodies don't have to stay raw-SQL-only. The cases where
daslang has enough information to validate against the current struct
ship as typed macros; everything else stays raw `db |> exec(…)`.

| Form | Shipped surface | Emits |
|---|---|---|
| ADD COLUMN | `db \|> add_column(type<T>, .Field [, default = expr])` | `ALTER TABLE t ADD COLUMN Field <SQL_TYPE> [NOT NULL] [DEFAULT …]`. SQL type from `_::sql_bind` adapter rail; NOT NULL derived from absence of `Option<>` wrapping; default from the expression via `to_sql_literal`. |
| CREATE INDEX | `db \|> create_index(type<T>, fields = (.A, .B), unique = true [, name = "ix_…"])` | `CREATE [UNIQUE] INDEX <name> ON t(A, B)`. Name auto-derived as `idx_<table>_<col1>_<col2>` to match the `[sql_index]` annotation convention from tut 24. |
| DROP INDEX | `db \|> drop_index_if_exists("ix_users_email")` | `DROP INDEX IF EXISTS ix_users_email`. Just a name string — no struct typing. |

**Reasoning:**

1. ADD COLUMN is by far the most common migration; making it as
   ergonomic as `create_table(type<T>)` is high leverage.
2. Type-driven NOT NULL / nullable derivation prevents a real class of
   bug — forgetting NOT NULL + DEFAULT, then existing rows panic on
   read after the migration.
3. Forward-compat with the future `dasSQL` abstraction: provider-
   specific syntax stays centralized inside the macro.
4. Implementation cost is moderate — `add_column` reuses the per-field
   DDL chunk from `create_table`; `create_index` reuses `[sql_index]`
   emission; `drop_index_if_exists` is a string wrapper. No new codegen
   path of significance.

**Stays raw SQL** (the macros are deliberately *not* shipped, the
escape is `db |> exec(…)`):

- DROP COLUMN — column may not exist in the current struct (you're
  dropping it!).
- RENAME COLUMN / RENAME TABLE — old name not in the current struct.
- ADD/DROP CHECK constraints — too many shapes; predicate language
  doesn't map cleanly.
- 12-step ALTER for old SQLite — already deferred from API_REWORK §30.

The reader's mental model is consistent: *typed when daslang has enough
info to validate, raw when it doesn't.*

**Test cases (typed ALTER):**

| File | What it asserts |
|---|---|
| `15_add_column_simple.das` | `add_column(type<User>, .Email, default = "")` emits the expected DDL; column appears in `pragma_table_info` with TEXT NOT NULL DEFAULT ''. |
| `16_add_column_optional.das` | `add_column(type<User>, .LastLoginAt)` for `Option<int64>` field emits INTEGER (no NOT NULL, no default). |
| `17_create_index_default_name.das` | `create_index(type<User>, fields = (.Email,))` emits `idx_users_email`; visible in `sqlite_master`. |
| `18_create_index_unique_composite.das` | `create_index(type<User>, fields = (.Email, .LastLoginAt), unique = true, name = "ix_lookup")` emits `CREATE UNIQUE INDEX ix_lookup ON users(Email, LastLoginAt)`. |
| `19_drop_index_if_exists.das` | Drop nonexistent index → no error; create + drop → second pragma shows it gone. |
| `20_typed_and_raw_mix.das` | Migration body using both `add_column` (typed) and `db \|> exec("DROP COLUMN OldField")` (raw) compiles and applies cleanly. Locks the "mix freely" claim. |

---

### Scenario 3 — Add an index + backfill data 🟢

**Setup.** User's DB at v2 (table `users`, `Email` column, half the rows
have `Email = ''`). Developer ships v3 with a migration that adds an index
*and* backfills the empties:

```das
[sql_migration(version=3, description="index email; backfill unknowns")]
def migration_003(db : SqlRunner) {
    db |> create_index(type<User>, fields = (.Email,))
    db |> _sql_update(type<User>,
        where = $(u) => u.Email == "",
        set   = (Email = "unknown@example.com"))
}
```

(Equivalent raw form: `db |> exec("CREATE INDEX …")` + `db |> exec("UPDATE
users SET …")`. Typed form leads in the tutorial; raw shown as the escape.)

**Decision (locked 2026-05-04, amended for α 2026-05-04) — body atomicity.**

> All statements in a migration body run inside one transaction. The
> body's audit INSERT, and any other migrations in the same
> `migrate_to_latest` call, share that same transaction (α-shape from
> Scenario 8): `BEGIN IMMEDIATE; <body of v1> INSERT v1 audit; <body of
> v2> INSERT v2 audit; …; COMMIT;`.

DDL and DML mix freely; SQLite supports transactional DDL. If any
statement in any body fails, **every effect rolls back** — including
prior migrations applied earlier in the same call. The next
`migrate_to_latest` retries the whole pending list cleanly.

(Forward note for the eventual `dasSQL` abstraction: MySQL does *not*
support transactional DDL — implicit COMMIT after each DDL statement.
Per-provider concern; SQLite-only v1 uses transactional DDL freely.)

**Decision (locked 2026-05-04) — migration body is plain daslang.**

Restating API_REWORK §30 lock #10 because users coming from
Flyway/EF/Alembic think of migrations as *files of SQL statements*. In
dasSQLITE they're plain daslang functions taking `SqlRunner`. Inside
the body anything goes:

- raw `exec`
- typed DDL: `create_index`, `add_column`, `drop_index_if_exists`,
  `create_table`, `drop_table_if_exists`
- typed DML: `_sql_update`, `_sql_delete`, `insert`, `_sql_upsert`,
  `insert_or_replace`, `insert_or_ignore`
- typed reads: `_sql(select_from(…))` to inspect data before deciding
  what to do
- arbitrary daslang control flow: `for`, `if`, helper function calls

Tutorial example: a migration that reads existing data, decides which
backfill values to use, then writes them. That's a daslang function;
no special migration sub-DSL.

**Decision (locked 2026-05-04) — VACUUM / ANALYZE as opt-in annotation parameters.**

Neither runs automatically. Both are user opt-in via parameters on the
`[sql_migration]` annotation:

```das
[sql_migration(version=3, description="add index", analyze=true)]
def migration_003(db : SqlRunner) {
    db |> create_index(type<User>, fields = (.Email,))
}

[sql_migration(version=4, description="reorganize storage", vacuum=true)]
def migration_004(db : SqlRunner) { … }
```

**Semantics:**

- Both parameters default to `false`. Absence means no extra step.
- After the **whole `migrate_to_latest` call commits** (α-shape from
  Scenario 8), the runner inspects the just-applied set: if any
  migration set `analyze=true`, runner emits `db |> exec("ANALYZE")`
  once; if any set `vacuum=true`, runner emits `db |> exec("VACUUM")`
  once. **Once per call, not once per migration** — the per-migration
  COMMIT boundary disappeared with α, so the natural unit is the call.
- If both flags are present anywhere in the batch: ANALYZE runs first,
  then VACUUM (cheap-then-expensive order).
- Both can fail (disk full, locked, etc.). Failure is logged via
  `to_log(LOG_WARNING, …)` but does **not** roll back any migration —
  the schema changes are already committed and durable. `migrate_to_latest`
  still returns success; `try_migrate_to_latest` still returns `Ok`.
  Rationale: VACUUM/ANALYZE are *optimizations*, not *correctness*.
- ANALYZE/VACUUM cannot run inside a transaction in SQLite — that's
  why they're post-COMMIT. The big-txn shape forces this naturally.

**Decision (locked 2026-05-04) — long-running backfills are a documented limitation.**

A migration that does `UPDATE big_table SET col = …` over 10M rows holds
the write-lock for the entire body's transaction duration. Tut 30
documents this limitation. Escape: chunk the UPDATE manually inside the
body, possibly with intermediate `with_transaction { … }` savepoints
(at the cost of losing all-or-nothing semantics). No new helper for v1.

**Future polish (not blocking v1) — `chunked_update` helper.**

A `daslib/sql_migrate_chunk` follow-up could ship `db |>
chunked_update(type<T>, where, set, chunk_size = 10000)` for large data
backfills with controlled lock contention. Logged for if real demand
surfaces.

**Test cases:**

| File | What it asserts |
|---|---|
| `21_multi_statement_atomic_commit.das` | Migration body with three statements: all three effects observable after the call returns. |
| `22_multi_statement_atomic_rollback.das` | Migration body where the second statement panics: first statement's effect is rolled back; audit row not inserted; pending list still shows this migration version. |
| `23_typed_and_raw_mix.das` | Body mixing `create_index` (typed), `_sql_update` (typed), and `exec("UPDATE … WHERE …")` (raw): all apply atomically. |
| `24_vacuum_runs_after_commit.das` | Migration with `vacuum=true`: DB freelist shrinks (or comparable observable). Audit row present. Runner returns success. |
| `25_analyze_runs_after_commit.das` | Migration with `analyze=true`: `sqlite_stat1` table populated for the affected table. Audit row present. |
| `26_vacuum_and_analyze_both.das` | Both flags true; ANALYZE-then-VACUUM order observable (e.g. via log capture or by checking that stats exist after the call). |
| `27_vacuum_failure_is_warning_not_error.das` | Inject a VACUUM failure (e.g. concurrent reader holding shared lock); migration still returns `Ok` with audit row present; warning logged. |
| `28_no_vacuum_no_analyze_default.das` | Migration without either flag: `sqlite_stat1` not created (assuming no prior ANALYZE); freelist unchanged. Locks "default = no extra steps." |

---

### Scenario 4 — Inspect, don't migrate 🟢

**Setup.** Three real situations want to *look* at migration state without
running anything:

1. **CI gate.** Pipeline fails the job if production DB has unapplied
   migrations not yet covered by a deploy step.
2. **Operator CLI.** `mytool db status` prints "current version: 4,
   pending: 2 (v5, v6), last applied 2025-12-01".
3. **Health endpoint.** Long-running service exposes `/healthz` with the
   current schema version.

**Surface (locked):**

```das
// Returns highest applied version, or 0 if __schema_version
// doesn't exist yet. Pure read; no side effects.
def current_schema_version(db : SqlRunner) : int

struct PendingMigration {
    version     : int
    description : string       // from the annotation (current code)
}
def pending_migrations(db : SqlRunner) : array<PendingMigration>

struct MigrationRecord {
    version     : int
    description : string       // frozen at apply-time, from the DB
    applied_at  : int64        // unix epoch seconds
}
def migration_history(db : SqlRunner) : array<MigrationRecord>
```

Plus `try_` siblings for tools that handle errors structurally.

**Decision (locked 2026-05-04) — pure read, no side effects.**

Inspection functions never create the audit table, never write a row,
never run a migration. If `__schema_version` doesn't exist:

- `current_schema_version` → `0`
- `pending_migrations` → all registered versions (since nothing's
  applied)
- `migration_history` → `[]`

Important for the CLI scenario: running `db status` against a brand-new
DB must not silently materialize `__schema_version` as a side effect.

**Decision (locked 2026-05-04) — `MigrationRecord` is three fields.**

The `success` column is gone (Q1=A lock from Scenario 1). All audit rows
are successes by construction.

**Decision (locked 2026-05-04) — description sourcing differs by function.**

Annotations live in code and can be edited at any time; the audit row
captures the description *at the moment the migration applied* and is
the authoritative historical record from then on. So:

- **`migration_history`** reads description from the DB row (frozen at
  apply time; future annotation edits don't propagate). Authoritative
  for what *was*.
- **`pending_migrations`** reads description from the annotation
  (compile-time; nothing else exists for never-applied migrations).
  Reflects what *will be*.

At apply time, `migrate_to_latest` reads the *current* annotation's
description and writes it into the audit row. From then on the DB is
the source of truth for that version. This makes the append-only rule
gentle on description text — editing a description on an already-shipped
migration is a no-op for any DB that's already past it (the audit row
already has the old text); a fresh DB picks up the new text on first
apply.

Tutorial 30 mentions this explicitly: "annotation descriptions can drift
without harm; treat the audit table as the historical record, not your
source code's annotations."

**Decision (locked 2026-05-04) — no `MigrationStatus` aggregate struct in v1.**

The three functions cover everything; aggregating them into a single
struct is a CLI-author convenience, not a missing capability. Punt to
deferred list; ship if real demand surfaces.

**Decision (locked 2026-05-04) — downgrade detection: warn from `migrate_to_latest`, no predicate.**

If `current_schema_version > MAX(registered_version)` — i.e. the audit
table has rows for versions the running binary doesn't know about —
`migrate_to_latest` emits a `to_log(LOG_WARNING, …)` and returns 0
(nothing to migrate forward; the DB is already past the current
binary's knowledge).

Warning text shape (illustrative, single line):

```
WARNING: DB at schema v9 but this binary only knows up to v3 — DB
appears to be running ahead of the binary. Common causes: rolling-
deploy mismatch, stale process, two apps sharing one DB, test
running against a prod snapshot. No migrations applied.
```

No `is_downgraded(db) : bool` predicate ships in v1. Tools that want
the boolean compute it from `current_schema_version(db)` plus the
registered max (which the runner already exposes through
`pending_migrations` ↔ empty + history non-empty).

Inspection functions themselves do **not** warn — they're pure read; a
diagnostic call shouldn't emit logs. The warning belongs at the
attempted-write boundary (`migrate_to_latest`).

**Test cases:**

| File | What it asserts |
|---|---|
| `29_current_version_empty_db.das` | Fresh DB, no `__schema_version`; `current_schema_version` returns 0; assert table still doesn't exist (no side effect). |
| `30_current_version_after_migrate.das` | Apply v1/v2/v3; `current_schema_version` returns 3. |
| `31_pending_empty_db.das` | Fresh DB; `pending_migrations` returns the full registered list with descriptions intact. |
| `32_pending_partial.das` | Applied = {1}, registered = {1,2,3}; `pending_migrations` returns 2 records (v2/v3) with annotation-side descriptions. |
| `33_pending_when_current.das` | Applied = {1,2,3}, registered = {1,2,3}; `pending_migrations` returns `[]`. |
| `34_history_empty_db.das` | Fresh DB; `migration_history` returns `[]`. |
| `35_history_after_migrate.das` | Apply v1/v2/v3; `migration_history` returns 3 records in version order; descriptions match annotations; `applied_at` falls in test window. |
| `36_no_side_effect_on_inspect.das` | Fresh DB, call all three inspectors in sequence; assert `__schema_version` still doesn't exist via `pragma_table_info`. |
| `37_description_frozen_in_db.das` | Manually INSERT a row into `__schema_version` with description="OLD"; have a registered annotation for the same version with description="NEW"; assert `migration_history` returns "OLD" (DB wins) and `pending_migrations` does not include that version (already in DB). |
| `38_pending_description_from_annotation.das` | Registered v=2 with description="from annotation" (not applied); `pending_migrations` row for v=2 has description="from annotation". |
| `39_downgrade_warning.das` | Manually INSERT row at v=9; registered max=v=3; call `migrate_to_latest`; assert return value = 0 + audit table unchanged + warning logged (capture `to_log` if dastest supports). |
| `40_compose_status_from_three_calls.das` | Smoke test that all three functions can be called in one breath against the same DB and produce mutually-consistent answers (current ≤ max(history.version) when history non-empty; pending versions all > current). |

---

### Scenario 5 — Existing pre-migrations DB ("baseline") 🟢

Closes Q4 from the open-questions list.

**Setup.** User has been shipping the app for a year *without*
`daslib/sql_migrate`. Their `app.db` already has `users`, `orders`, etc.
Dev pulls in `daslib/sql_migrate`, writes `[sql_migration(version=1)]`
that does `db |> exec("CREATE TABLE users (…)")` representing the v1
shape, swaps `with_sqlite` → `with_latest_sqlite`. Boom — runtime crashes
with "table users already exists."

The user's DB is *semantically* at v1 — same schema as what
migration_001 would create — but the audit table records v0, and the
runner's only contract is "run the body."

**Decision (locked 2026-05-04) — ship `baseline(db, version)`.**

Flyway's well-known pattern. Cleaner than asking users to write raw
`INSERT INTO __schema_version` statements. Surface:

```das
// Stamp v1..N as "already applied" without running their bodies.
def baseline(db : SqlRunner, version : int)
def try_baseline(db : SqlRunner, version : int) : Result<int, string>
```

**Semantics:**

1. **Stamp v1..N, not just N.** Audit table looks complete in
   `migration_history` after baseline. `MAX(version)` is unchanged
   either way; honest history wins.
2. **Idempotent.** Stamping a version that's already in the audit is a
   no-op. Safe to leave the baseline block in `main()` permanently
   (though tut 30 recommends removing it after one release cycle).
3. **Description sourcing per stamped row:**
   - From the registered annotation if `[sql_migration(version=v)]`
     exists for `v`.
   - `"[baseline]"` sentinel otherwise (un-registered version stamped
     for retroactive history).
4. **`applied_at`** = stamping time, not the historical apply time
   (we don't know the historical time). Document this in tut 30.
5. **Reject `baseline(db, version=N)` where `N > registered_max`** —
   almost certainly a typo. `try_baseline` returns `Result::Err`;
   `baseline` panics.

**Decision (locked 2026-05-04) — Tutorial 30 leads with first-change-as-first-migration.**

> "Prefer writing your first migration as the first *change* you need to
> make to the existing schema, not a recreation of v0 state. Existing
> tables stay; migration_001 does `ALTER TABLE … ADD COLUMN …` (or
> whatever the real first change is) and applies cleanly to the existing
> DB. Reach for `baseline()` only when you specifically want a
> CREATE-TABLE-shaped first migration for retroactive history."

Both shapes documented; first-change-as-first-migration is the
recommended-default; `baseline()` is the explicit Flyway-style escape.

**Decision (locked 2026-05-04) — proactive warning at adoption-shape detection (Layer 1).**

Right after eagerly creating `__schema_version`, the runner queries
`sqlite_master` for any user objects (excluding `sqlite_*` and
`__schema_version`). If yes AND the audit table was just created from
empty:

```
WARNING: this DB has existing tables/views/indexes but no migration
history (__schema_version was just created). If your first migration
creates tables that already exist, it will fail. To adopt migrations
on an existing DB, call `db |> baseline(version = N)` before
`migrate_to_latest()` to mark v1..vN as already applied. See tut 30
§ "Adopting migrations on an existing DB".
```

Emitted via `to_log(LOG_WARNING, …)`. Then the runner proceeds with
migrations as normal — the warning is informational, not a stop.

False-positive cost: one warning the first time a user runs an app
that built tables outside the migration system on a fresh DB. Acceptable
noise — they see it once, decide whether to act, never see it again.

**Decision (locked 2026-05-04) — enriched panic on first-migration failure (Layer 2).**

If a migration body fails AND it is the smallest pending version AND
the audit table was just created from empty in this `migrate_to_latest`
call (heuristic for "this was an adoption attempt"), the panic message
appends:

```
note: This is the first migration on this DB and __schema_version
was empty before this call. Looks like an adoption attempt on an
existing schema. Call `db |> baseline(version = N)` once before
`migrate_to_latest()` to mark v1..vN as already applied.
See tut 30 § "Adopting migrations on an existing DB".
```

Same message for `try_migrate_to_latest`'s `Result::Err` value.

Together with Layer 1, the worst-case UX of the naive swap is:

1. App boots → warning logged.
2. Migration v1 fails → panic with actionable hint.
3. User reads hint → adds `baseline(version=1)` block → restarts → works.

No silent skip, no "warn and continue." Schema mismatch stays fail-fast;
we improve only the *quality of failure*, not the shape.

**Test cases:**

| File | What it asserts |
|---|---|
| `41_baseline_fresh_db.das` | Fresh `:memory:`, no audit table; `baseline(version=3)` creates audit table + stamps v1/v2/v3. `migration_history` returns 3 rows in order. |
| `42_baseline_idempotent.das` | Call `baseline(version=3)` twice; second call is a no-op; audit unchanged. |
| `43_baseline_descriptions_mixed.das` | Register annotation for v1 (description="initial") only. `baseline(version=3)` stamps v1 with "initial", v2/v3 with "[baseline]". |
| `44_baseline_above_registered_max.das` | Register up to v3; `try_baseline(version=5)` returns `Result::Err`; audit unchanged. Strict `baseline(version=5)` panics. |
| `45_baseline_then_migrate.das` | `baseline(version=2)`, then `migrate_to_latest`; only v3+ run; audit ends with v1/v2 (baselined) + v3+ (applied). |
| `46_layer1_warning_existing_tables.das` | Pre-create a `users` table directly; call `migrate_to_latest` (no baseline); assert WARNING logged BEFORE migration body runs. |
| `47_layer1_no_warning_fresh_db.das` | Fresh `:memory:` with no tables; call `migrate_to_latest`; assert NO Layer-1 warning logged. |
| `48_layer2_enriched_panic.das` | Pre-create `users`; register migration_001 that does `CREATE TABLE users`; call `try_migrate_to_latest`; assert `Result::Err` value contains the baseline hint substring. |
| `49_layer2_no_hint_on_later_failure.das` | Apply v1/v2/v3 successfully; pre-create blocking state for v4; call `try_migrate_to_latest`; `Result::Err` does NOT contain the baseline hint (this isn't an adoption case). |
| `50_swap_with_sqlite_to_with_latest.das` | Integration test mirroring the user-facing scenario: open existing DB with `with_sqlite`, build user tables, close. Re-open with `with_latest_sqlite` w/ no baseline → panic with hint. Re-open with baseline + `with_latest_sqlite` → success. |

---

### Scenario 6 — Two devs pick the same version number 🟢

**Setup.** Dev A on branch `feature/email` writes
`[sql_migration(version=2)] def migration_002_email`; Dev B on
`feature/avatar` writes `[sql_migration(version=2)] def
migration_002_avatar`. Different function names, different files,
possibly different modules — **git sees no textual conflict**. Both
branches merge into `main`. Only the macro can catch the `version=2`
collision.

**Decision (locked 2026-05-04) — duplicate-version detection is GLOBAL (program-scoped).**

Amends API_REWORK §30 lock #5 (originally module-scoped). The migration
registry is logically a program-level concept — every `[sql_migration]`
in the build feeds into the same `migrate_to_latest` runner, so the
version space is shared. The macro uses a global gc_node-tracked
compile-time table; duplicates across any modules error at compile time.

Implementation pattern: same gc_node compile-time-state mechanism that
`detect_duplicates` and `[sql_table]` adapter registry use. Cost is
small.

**Decision (locked 2026-05-04) — compile-error message names both functions AND both modules.**

```
error: duplicate [sql_migration(version=2)]
  registered: migration_002_email  in module `app/migrations_email.das`
  duplicate:  migration_002_avatar in module `app/migrations_avatar.das`
hint: pick a different version number for one of them, or merge the bodies
```

The module name is critical for cross-module duplicates — without it
the dev can't easily locate the conflicting function.

**Decision (locked 2026-05-04) — runtime belt-and-suspenders check.**

If for any reason two registrations slip past compile-time (e.g. dynamic
plugin loading, future scenarios), `migrate_to_latest` walks the
registry once before running and panics if it sees a duplicate, with
the same loud message shape as the compile error. Cheap, defensive,
runs once per call.

**Decision (locked 2026-05-04) — semantic-dupe detection is a lint tool, not a compile-time error.**

Catching two distinct version numbers with semantically-similar bodies
(copy-paste merge mistakes) is fuzzy — `detect_duplicates` produces
false positives. Wrong shape for a build gate. Right shape for a
`daslib/sql_migrate_lint` (or similar) PR-review helper that flags
suspect bodies for human review. Out of scope for v1; revisit when the
ecosystem matures.

**Documented merge-resolution workflow (one paragraph in tut 30):**

> "When git silently accepts two branches that both used `version=2`,
> the build will fail at the `[sql_migration]` macro with both function
> names + both module paths. Resolution: renumber the LATER-merged
> branch's migration to the next free version (e.g. v3 if v2 conflicts),
> push, re-merge. Gap-allowed versioning means it's fine if the
> resulting sequence has gaps you don't care about — `1, 2, 3, 4, 5`
> and `1, 2, 5, 6, 7` are both valid."

**Test cases:**

| File | What it asserts |
|---|---|
| `51_dup_version_same_module.das` | Two `[sql_migration(version=2)]` annotations in the same file → compile error. Use dastest's expect-compile-failure pattern. |
| `52_dup_version_cross_module.das` | Two `[sql_migration(version=2)]` annotations in two different `.das` files → compile error. Locks the global-scope amendment. |
| `53_dup_error_names_both.das` | Compile-error output contains both function names AND both module paths. (Capture compiler output; substring assert.) |
| `54_renumbered_resolution.das` | Same setup as 52, but second migration uses `version=3` → compiles cleanly; runner applies both. |
| `55_runtime_belt_and_suspenders.das` | Manually inject duplicate registry entries (test-only API into the registry) → `migrate_to_latest` panics with the duplicate message. Locks defense-in-depth. |
| `56_gap_allowed_versions.das` | Register `version=1`, `version=5`, `version=10` (gappy); `migrate_to_latest` runs all three in order; pending detection works on gaps. Locks gap-allowed numbering. |

---

### Scenario 7 — Migration N fails midway 🟢

**Setup.** DB at v3. Dev ships a release with v4, v5, v6. v5's body has
a bug; some statement inside it panics (NOT NULL violation, unknown SQL
function, disk full, FK constraint, etc.). Atomicity story locked from
Scenarios 1–3 + α-shape from Scenario 8: **whole-call txn rolls back —
v4's effects, v5's partial effects, audit inserts for v4 + v5 — all
gone**. DB stays at v3 (the version it was at before this
`migrate_to_latest` call), fail-fast (v6 doesn't run), re-run on next
launch retries the whole pending list cleanly (v4/v5/v6 all re-applied
once v5 is fixed).

**Decision (locked 2026-05-04) — LOG_ERROR before panic.**

Before propagating the failure, the runner emits a structured error
line via `to_log(LOG_ERROR, …)`:

```
ERROR: migration v5 'add score + backfill' FAILED — rolled back call
  underlying error: SQLite no such function: unknown_fn
  rolled back: v4, v5 (this call)
```

Names the failing migration AND the migrations rolled back as collateral
(v4 succeeded body-wise but its commit was inside the same big txn).
The "this call" qualifier distinguishes from anything previously
committed (v1/v2/v3 stay).

Helps the operator see *what failed* even if the panic handler swallows
the message somewhere upstream. Same line format whether the call form
is `migrate_to_latest` (panic-bound) or `try_migrate_to_latest`
(Result-bound) — emitted in both paths.

**Decision (locked 2026-05-04) — enriched panic + matching `Result::Err` text.**

```
panic: migration v5 'add score + backfill' failed:
  SQLite error: no such function: unknown_fn

note: this migrate_to_latest call's transaction was rolled back — v4
and v5 are NOT applied. The DB is still at v3 (unchanged from before
the call). Fix the migration body and re-run; the corrected migration
set (v4, v5, v6) will replay on the next call. The DB does not need
to be reset.
```

`try_migrate_to_latest` returns the same enriched message in
`Result::Err`. The "DB does not need to be reset" line is high-value —
devs who haven't seen this before often think a failed migration leaves
the DB in a corrupt half-applied state and start nuking files. Saying
so explicitly heads off the panic.

**Implication of α-shape worth restating:** prior migrations *in this
call* (v4 here) get rolled back too, even though their bodies succeeded.
This is by design (Scenario 8); cost is "redo prior body work on
re-run", which is bounded for the typical case (small migrations).
Long-running backfills are flagged in Scenario 3's "long backfill"
note — keep them in their own `migrate_to_latest` invocation if
re-execution cost matters.

**Decision (locked 2026-05-04) — tut 30 paragraph: migration bodies should be DB-only.**

Discipline note, no enforcement (we have no way to enforce it):

> "Keep migration bodies DB-only. The transactional rollback covers
> everything inside SQLite (DDL + DML), but anything that touches the
> outside world — HTTP calls, file writes, `system(…)` — is NOT rolled
> back when the migration fails. If the migration retries, those side
> effects re-execute. If you genuinely need a cross-system migration,
> sequence it outside the migration runner."

**Edge case — audit-row-only failure:** if every body statement succeeds
but the final `INSERT INTO __schema_version` fails (disk full mid-commit,
etc.), SQLite rolls back the body too — same effective outcome as
body-failure. No special handling.

**Idempotent-body note (tutorial, not an API):** for purely-DML
migrations doing data backfills, `WHERE NOT EXISTS …` / `INSERT OR
IGNORE` / `try_exec` patterns let the body re-run safely if a previous
attempt half-committed (which can't happen under our atomicity, but
disk corruption / partial recovery scenarios are real). Not enforced;
mentioned as a defensive option for high-value migrations.

**Test cases:**

| File | What it asserts |
|---|---|
| `57_failure_rolls_back_body.das` | v5 body has 3 statements; statement 3 panics; assert effects of statements 1+2 are NOT visible after the call returns. |
| `58_failure_rolls_back_prior_migrations.das` | Pending = [v4, v5]; v4 body succeeds, v5 panics; assert v4's effects are ALSO rolled back (α-shape collateral). DB still at v3. Locks the α contract. |
| `59_failure_audit_clean.das` | v5 fails; `__schema_version` MAX still 3; `migration_history` does NOT contain v4 OR v5 rows. |
| `60_subsequent_skipped.das` | Pending = [v4, v5, v6]; v5 panics; assert v6 body did NOT run. |
| `61_rerun_after_fix.das` | Pending = [v4, v5]; v5 broken; first rerun fails; fix v5; second rerun applies v4+v5 cleanly; audit ends with v4 + v5. |
| `62_log_error_emitted.das` | v5 fails; capture `to_log` output; assert one LOG_ERROR line with version+description+underlying error+rolled-back list. |
| `63_panic_message_contents.das` | v5 fails (strict form); panic message contains version, description, "v4 and v5 are NOT applied", "DB is still at v3", "does not need to be reset". |
| `64_try_form_returns_result_err.das` | v5 fails (try_ form); `Result::Err` value contains the same text as the panic. |
| `65_audit_only_failure.das` | Simulate audit INSERT failure (e.g. via tampering with __schema_version permissions/lock); body effects NOT visible (txn rolled back). |
| `66_idempotent_body_pattern.das` | Migration body uses `INSERT OR IGNORE` for data backfill; force a synthetic mid-fail and re-run; final state matches single-run state (no double-apply). Locks the documented defensive pattern. |

---

### Scenario 8 — Two processes call `migrate_to_latest` at once 🟢

**Setup.** Real situations:
- Kubernetes rolling deploy spins up 3 new app pods; all three boot,
  all three call `migrate_to_latest` against the same shared `app.db`.
- Dev double-clicks the app icon; two processes start.
- Multi-worker app architecture; all workers boot in parallel.

Without protection, the race goes: A reads MAX=0, computes pending,
starts running v1's CREATE TABLE; B reads MAX=0 too (A hasn't
committed); A commits; B unblocks and runs v1 → "table already exists"
panic.

**Decision (locked 2026-05-04) — α: one transaction wraps the whole call.**

> The runner uses `BEGIN IMMEDIATE` once at the start of the call,
> applies all pending migrations + their audit inserts inside that
> single transaction, `COMMIT` at the end. If any migration fails,
> the whole transaction rolls back; re-run replays the entire pending
> list.

β (per-migration transaction + per-iteration re-check) was considered
and rejected. β buys partial-success-on-failure (prior migrations
remain committed if a later one fails), but at the cost of:

- ~2x the test surface, much of which is hard-to-write multi-process
  race tests.
- A correctness invariant inside the loop body (the per-iteration
  MAX re-check) that future maintainers can silently break by
  refactoring without any test failing on single-process runs.
- ~30 lines of concurrency-aware loop logic vs ~10 lines for α.

α's "lose intermediate work on failure" cost is bounded — re-running
re-applies. The narrow case it hurts is "a migration set with a
long-running backfill, where a later migration fails": the backfill
replays. We document this as a known cost (Scenario 3's long-backfill
note) and recommend keeping long backfills in their own
`migrate_to_latest` invocation if replay cost matters.

**Lock semantics:**

1. **OS-level coordination via SQLite's RESERVED lock.** `BEGIN
   IMMEDIATE` acquires the file-level RESERVED lock through the OS
   (`fcntl()` on Unix, `LockFileEx` on Windows). Only one process
   holds RESERVED at a time. Other processes' `BEGIN IMMEDIATE` calls
   block (up to `busy_timeout`) until release. We don't implement an
   advisory lock; SQLite + the filesystem do it.
2. **`with_latest_sqlite` auto-applies `apply_recommended_pragmas`** —
   from chunk 10's tut 33. Sets `busy_timeout = 5000ms` (5-second
   wait), `journal_mode = WAL`, etc. Concurrent runners get a sane
   default wait window before erroring; user can override via explicit
   `set_pragma` calls before `migrate_to_latest`.
3. **busy_timeout exhaustion → `Result::Err` / panic.** If B can't
   acquire the lock within the timeout, `try_migrate_to_latest`
   returns `Result::Err("SQLITE_BUSY: another migration is in
   progress")`; strict form panics. Caller can retry with a longer
   timeout or back off externally.
4. **Concurrent-runner outcome (correct case):** A acquires lock,
   applies all pending, commits. B blocks until A releases; B re-reads
   MAX inside its own `BEGIN IMMEDIATE`, sees nothing pending,
   COMMITs trivially, returns 0. **Each migration body runs exactly
   once across all processes.**
5. **Network filesystem caveat — documented in tut 30.**
   OS file locking is unreliable on NFS, SMB, GlusterFS, EFS. SQLite
   docs explicitly warn against this; our concurrency contract
   degrades to "mostly works, occasionally not" on shared volumes.
   Tut 30 paragraph:

   > "For deployments where multiple processes share the DB file via
   > a network filesystem (NFS, SMB, k8s shared volumes), do NOT rely
   > on `migrate_to_latest`'s concurrency safety. OS-level file
   > locking is unreliable on these filesystems. Run migrations as a
   > separate explicit step before launching app instances."

6. **Death-during-call recovery.** If A is kernel-killed mid-call
   without committing, SQLite's recovery journal rolls back A's whole
   transaction on next open. B (or A's restart) finds MAX unchanged
   from before the call and re-applies the pending list. Same outcome
   as the panic-on-bad-code case — never partial, never duplicate.

**Test cases:**

| File | What it asserts |
|---|---|
| `67_concurrent_a_applies_b_blocks.das` | Spawn two processes/contexts on the same DB file; A applies v1+v2+v3; B blocks at `BEGIN IMMEDIATE` until A commits; B then sees nothing pending, returns 0. |
| `68_concurrent_each_migration_once.das` | Two racing processes against pending=[v1,v2,v3]; assert each body executed exactly once across both processes (e.g. v1's CREATE TABLE doesn't fire twice with "already exists"). |
| `69_busy_timeout_exhausted.das` | A holds the lock artificially long (e.g. injected sleep); B's `try_migrate_to_latest` with low busy_timeout returns `Result::Err` containing SQLITE_BUSY hint. |
| `70_kernel_kill_recovery.das` | Simulate process death mid-transaction (close connection without commit); reopen; assert MAX unchanged; rerun applies pending cleanly. |
| `71_with_latest_sets_busy_timeout.das` | Open via `with_latest_sqlite`; query `PRAGMA busy_timeout` before any migration runs; assert the recommended-default value is set. |
| `72_network_fs_caveat.das` | Documentation-only — N/A. (No reliable test for unreliable file locking; the caveat is in tut 30 prose.) |
| `73_failure_rolls_back_call_visible_to_concurrent.das` | A's v5 fails; A's whole call rolls back; concurrent B (started slightly later) acquires lock after A's rollback; B sees MAX still at original; B retries the same pending list (will fail too if same code, but the retry visibility is what we're testing). |

---

### Scenario 9 — Rollback request from a non-DB user 🟢

**Setup.** Operator deploys v7. v7 adds a column AND backfills it via a
heuristic that turns out wrong — half the existing rows get corrupted
values. Operator opens an incident: **"undo v7."**

EF Core has `dotnet ef database update <PrevMigration>`. Alembic has
`alembic downgrade -1`. Flyway charges money for `flyway undo`. Users
coming from those tools assume we have it too. This scenario is the
answer they get when they ask.

**Decision (locked 2026-05-04) — no rollback API ships, ever. The door is closed.**

API_REWORK §30 deferred-list item #1 originally left the door slightly
ajar ("Future: if shipped, live as a separate function macro"). The
discussion in this scenario closes it. Down-migrations are a *design
anti-pattern*, not a missing feature. Future versions / `daslib/sql_migrate_*`
follow-ups should not ship them either. If a real use case forces our
hand, revisit then; default posture is permanent "no."

The mechanism is well-understood (paired up/down bodies, runner walks
backward, deletes audit rows) — see API_MIGRATION's discussion log for
the explainer. The reasons it doesn't work in practice:

1. **Implicit data loss.** Up `ADD COLUMN`; down `DROP COLUMN`; rolling
   back silently deletes the data accumulated in that column.
2. **Schema-evolution rot.** v7 adds a column; v8 references it via FK;
   v7's `down` body says `DROP COLUMN` — fails because of v8's FK.
   Down bodies have to be retroactively updated as later migrations
   land. Nobody does this.
3. **Multi-step bodies aren't invertible.** Lossy backfills, hashed
   columns, collapsed enums — the up was lossy, the down is a lie.
4. **Production reality ≠ local dev.** Down body written six months
   ago against empty tables; runs against 100M rows + live writes +
   FKs the dev didn't anticipate.
5. **Rot.** Down bodies never tested in production conditions; first
   run during an incident — exactly the wrong moment.
6. **The thing people actually want is "go back to before I deployed
   v7" — that's a backup-restore concern**, not a migration one.
   Yesterday's DB file *is* the v6 state with data intact.

Industry has consolidated: SQLAlchemy/Alembic's creator has publicly
regretted shipping downgrades; DBT rejects them by design; Flyway adds
caveats every year. Modern best practice: **backup, forward-fix,
restore-when-needed.**

**Decision (locked 2026-05-04) — forward-fix is the recommended response.**

Tut 30 paragraph + worked example:

> "If a deployed migration broke things: write the next migration to
> fix it. v7 added a bad backfill column? v8 reads the correct values
> from some other source, runs UPDATE to overwrite v7's mistakes. The
> audit trail shows v7 ('add Score column') + v8 ('correct Score
> backfill') in chronological order — permanent record of what
> happened, including the screwup. Operators run `migrate_to_latest`
> v8 in the same flow they migrate anything else; no special rollback
> mode."

Works for *most* incidents. The bad migration's effects are corrected
by a follow-up.

**Decision (locked 2026-05-04) — backup-and-restore is the fallback when forward-fix can't recover data.**

Some incidents lose information that can't be reconstructed (v7
dropped a column, or backfilled with a hash that's irreversible). For
those:

> "Restore the DB file from a backup taken before v7. Use the Online
> Backup API (tut 34) or just stop the app and copy the file. Then
> rerun migrations against the restored DB. If you want to skip the
> bad v7 entirely (because you've fixed it forward as v8): manually
> INSERT a row into `__schema_version` for v7 to mark it 'applied' —
> v8's logic now runs against the restored-pre-v7 state and gets the
> right outcome. This is operator-intervention territory, not a
> command we ship."

The "manually INSERT" step is a one-liner the operator runs in
`sqlite3` directly. We don't wrap it; this is a rare, careful operation
that belongs in human hands.

**Decision (locked 2026-05-04) — explicit EF/Flyway-comparison callout in tut 30.**

> "If you're used to `dotnet ef database update <PrevMigration>` or
> `alembic downgrade -1`, that command intentionally doesn't exist in
> dasSQLITE. We deliberately ship neither down-migrations nor
> migrate-to-version. The reasoning is at the top of this section;
> the recommended response is fix-forward."

**Test cases:**

| File | What it asserts |
|---|---|
| `74_no_migration_down_macro.das` | `[sql_migration_down(version=N)]` is NOT a recognized annotation; using it is a compile error. Locks the "door is closed" stance. |
| `75_forward_fix_recipe.das` | End-to-end: apply v7 (intentionally bad), apply v8 (corrects v7), assert final state matches the desired-correct state; assert audit shows both v7 and v8 in chronological order. Locks the worked recipe. |
| `76_manual_skip_via_audit_insert.das` | After the user manually INSERTs `__schema_version` for v7 (simulating "skip-this-bad-migration" recovery), the next `migrate_to_latest` does NOT re-run v7. Locks the operator-side escape hatch. |

---

### Scenario 10 — DROP / RENAME column on old SQLite 🟢

**Setup, considered:** dev writes `db |> exec("ALTER TABLE users DROP
COLUMN OldField")` in a migration; user runs it against an SQLite that
predates the feature. SQLite errors with "near DROP: syntax error".
12-step rebuild recipe ensues (manually create new table, copy data,
drop indexes/triggers, drop old, rename new, recreate
indexes/triggers, FK check, commit).

**Why this scenario doesn't apply to dasSQLITE.**

dasSQLITE bundles SQLite via `sqlite_boost` at version **3.41.2**
(verified `modules/dasSQLITE/sqlite/sqlite3.h:149`). Bundled means
users get our SQLite, not the OS's — version is pinned. 3.41.2 covers
every ALTER form a normal app needs:

| ALTER form | First supported | Bundled (3.41.2)? |
|---|---|---|
| `ADD COLUMN` (with constraints) | always | ✓ |
| `RENAME TABLE` | always | ✓ |
| `RENAME COLUMN` | 3.25.0 (Sep 2018) | ✓ |
| `DROP COLUMN` | 3.35.0 (Mar 2021) | ✓ |

Migration bodies can write `db |> exec("ALTER TABLE … DROP COLUMN …")`
or `… RENAME COLUMN … TO …` directly; SQLite handles them without
a recipe.

**Decision (locked 2026-05-04) — withdraw API_REWORK §30 deferred-list item #4.**

The "SQLite 12-step ALTER TABLE helper (`recreate_table`)" was deferred
on the assumption that some users would run against pre-3.35 SQLite.
We don't ship that scenario — bundled `sqlite_boost` is 3.41.2.
Deferred-list item is withdrawn outright (not "deferred to v2"); no
need to revisit.

If a future contributor wants `recreate_table(db, type<T>, copy_from=…)`
for *other* reasons — e.g. type-system-driven schema reshuffling, or
because someone exposes a future provider whose ALTER is more limited —
that's a separate design conversation, not a residue of this one.

**Decision (locked 2026-05-04) — tut 30 has a one-liner on SQLite version baseline.**

> "dasSQLITE bundles SQLite 3.41.2. All `ALTER TABLE` forms — including
> DROP COLUMN and RENAME COLUMN — work directly via `db |> exec(…)` in
> migration bodies. No version checks or 12-step recipes required."

That's it. No recipe section, no version-check helper, no future-work
callout. Closes a class of "what about older SQLite?" worry that's
genuinely not relevant for dasSQLITE deployments.

**Decision (locked 2026-05-04) — DROP COLUMN / RENAME COLUMN / RENAME TABLE stay raw `exec`.**

Reconfirms scenario 2's typed-ALTER lock. The shippable typed forms
are `add_column` + `create_index` + `drop_index_if_exists`; the
destructive/historical-name forms (where the current struct doesn't
have the old name) stay as raw `exec`. Bundled-3.41.2 doesn't change
the design rationale for that split.

**Test cases:**

| File | What it asserts |
|---|---|
| `77_drop_column_works_natively.das` | Migration body does `db \|> exec("ALTER TABLE users DROP COLUMN OldField")`; after apply, `pragma_table_info` confirms the column is gone. Locks "no recipe needed." |
| `78_rename_column_works_natively.das` | Migration body does `db \|> exec("ALTER TABLE users RENAME COLUMN OldName TO NewName")`; after apply, column is renamed; existing data preserved. |
| `79_sqlite_version_meets_minimum.das` | Smoke test — query `SELECT sqlite_version()`; assert ≥ 3.35.0. Tripwire: if a future bundled-version downgrade slips in, this fails. |

---

### Scenario 11 — Struct-to-struct convert + table rebuild 🟢

(Originally framed as "Schema autogenerate from struct diff (EF-style)";
the discussion reframed it. The original "no autogen in v1" stance is
preserved for the *EF-style diff-emitter* approach; what we ship instead
is below.)

**Setup.** Dev wants to evolve a struct's shape — rename a column,
change a type, drop a column AND backfill some new field — in a way
SQLite's narrow ALTER vocabulary can't handle. The standard escape is
the **12-step rebuild**: `CREATE TABLE T_new` with the new shape,
`INSERT … SELECT` from `T` with mapped columns, drop indexes/triggers,
`DROP TABLE T`, rename `T_new → T`. Tedious to write by hand, easy to
get wrong, but the only path on SQLite for non-trivial schema
evolution.

**Decision (locked 2026-05-04) — ship `struct_convert` + name overloads in v1.**

The 12-step rebuild becomes one short migration body when daslang
macros do the field-by-field translation work. Two changes to existing
primitives, one new macro:

```das
// 1. struct-to-struct converter — auto-derived where trivial,
//    dev override block for non-trivial fields.
let convert = struct_convert(type<UserV6>, type<User>) $(old, var new) {
    // Macro auto-fills trivial mappings (same-name same-type,
    // Option flips, primitive casts). Dev only writes overrides:
    new.Email = (old.LegacyEmail != "") ? old.LegacyEmail : "unknown@example.com"
}

// 2. existing primitives gain optional `name=` overrides:
db |> create_table(type<User>, name = "users_new")
db |> insert_to (type<User>, new_row, name = "users_new")
```

**There is no `recreate_table` runtime.** The dev writes the rebuild
pattern directly — 10 lines, all using existing primitives:

```das
[sql_table(name="users", legacy=true)]    // historical reference
struct UserV6 { @sql_primary_key Id : int; Name : string; LegacyEmail : string }

[sql_table(name="users")]                 // current
struct User { @sql_primary_key Id : int; Name : string; Email : string }

[sql_migration(version=7, description="restructure users")]
def migration_007(db : SqlRunner) {
    let convert = struct_convert(type<UserV6>, type<User>) $(old, var new) {
        new.Email = (old.LegacyEmail != "") ? old.LegacyEmail : "unknown@example.com"
    }

    db |> create_table(type<User>, name = "users_new")
    for (old in db |> select_from(type<UserV6>)) {
        var new : User = default
        convert(old, new)
        db |> insert_to(type<User>, new, name = "users_new")
    }
    db |> exec("DROP TABLE users")
    db |> exec("ALTER TABLE users_new RENAME TO users")
}
```

Indexes auto-recreate via `[sql_index]` annotations on `User` (handled
by `create_table`'s existing DDL emission). FKs auto-recreate via
`@sql_references` annotations. Triggers/views the dev manages
explicitly if they had any. The whole thing runs inside
`migrate_to_latest`'s big α-shape transaction (Scenario 8), so
failure rolls everything back.

**`struct_convert` auto-rules (locked):**

| Field shape diff | Auto-derived emission |
|---|---|
| Same name + same type | `new.X = old.X` |
| Same name + `T → Option<T>` | `new.X = Some(old.X)` |
| Same name + `Option<T> → T` | **macro_error** — needs override (existing rows might be NULL) |
| Same name + INT ↔ STRING ↔ FLOAT | `new.X = to_string(old.X)` / `to_int(old.X)` / `float(old.X)` |
| New field with daslang default initializer | uses the default |
| New field of type `Option<T>` | `new.X = none()` |
| New field, no default, not Option | **macro_error** — needs override |
| Field gone from target | omitted (no assignment) |
| `@sql_renamed_from = "OldName"` on new field | reads `old.OldName`, writes `new.NewName` |
| Anything else (struct → primitive, complex type changes, lossy conversions without `confirm_lossy=true`) | **macro_error** — needs override |

The override block is *additive* — auto-derived assignments run first,
then the dev's overrides (which can replace any auto-derived line by
re-assigning). Dev only writes lines for non-trivial fields.

**`[sql_table(legacy=true)]` semantics (locked):**

- Allows the same `name=` as a non-legacy `[sql_table]` (the duplicate-
  name check skips `legacy=true` registrations).
- Struct is **readable** via `select_from(type<S>)` — useful for the
  rebuild SELECT phase.
- Struct is **excluded** from the current-schema registry: not eligible
  for `create_table` / `insert` / `_sql_update` / `_sql_delete` /
  `_sql_upsert` / column-info / etc. Trying any of these is a
  macro_error: *"`UserV6` is `[sql_table(legacy=true)]` — read-only;
  use the current `[sql_table]` struct for writes."*
- Macro_error message names the current non-legacy struct that
  matches the same `name=`, so the dev knows which to use.

**Decision (locked 2026-05-04) — user philosophy: opt-in convenience, not enforced discipline.**

> "Users keeping a `[sql_table(legacy=true)]` historical struct get
> `struct_convert` for trivial mappings; only write override blocks for
> non-trivial fields. Users who don't keep historical structs hand-write
> the rebuild via raw `db |> exec("INSERT INTO users_new (…) SELECT (…)
> FROM users")` with explicit column lists. Both paths work; the macro
> is convenience, not requirement."

This frames `struct_convert` honestly — "if you decide to not keep the
old schema around, you don't want auto-conversion." Doesn't impose a
maintenance burden on users who'd rather hand-write SQL.

**Why this is the v1 answer (replacing the rejected diff-emitter approach):**

The earlier sketch was an EF-style schema-diff macro that emits ALTER
statements. Rejected because:

1. SQLite's narrow `ALTER` vocabulary makes most non-trivial schema
   changes unreachable through ALTER (PK/FK/DEFAULT/COMPUTED/CHECK
   modifications all kaboom; see Scenario 10's "kaboom" table).
2. Hidden auto-emitted SQL inside the migration body is opaque — hard
   to review, hard to trust.
3. Implementation would have been ~1000 lines (snapshot system + diff
   engine + CLI tool + macro + emission).

`struct_convert` + name overloads + legacy flag sidesteps all three:

1. The rebuild path doesn't use ALTER for the hard cases; it uses
   `CREATE TABLE T_new` + `INSERT … SELECT`. SQLite's vocabulary
   limitations stop being relevant.
2. The migration body is plain daslang code visible at the call site;
   the only macro-emitted code is the `convert` function (one auto-
   generated function literal), which the dev can inspect via
   `options log` if curious.
3. Implementation is ~150 lines (`struct_convert` macro + name
   overloads + legacy flag).

**Macro-as-quantum-advantage check.** Field-by-field struct conversion
is exactly what daslang macros excel at: full struct-shape AST access
at compile time, qmacro for synthesizing the converter body, no runtime
reflection or codegen ceremony. The C# / EF equivalent of `struct_convert`
would require source generators or a runtime IL emitter — much heavier.
Macros let us collapse what would be a CLI tool + generated files in
other ecosystems into one declarative call site.

**LOC budget:**

| Component | Size |
|---|---|
| `struct_convert` macro (field walk, auto-rules, override-block merge) | ~120 lines |
| `create_table(type<T>, name? : string)` overload | ~5 lines |
| `insert_to(type<T>, row, name? : string)` overload | ~5 lines |
| `[sql_table(legacy=true)]` flag (dup-name allowance + write-path exclusion) | ~15 lines |
| Tests | 14 cases (below) |

**~150 lines of net-new code** — ships in v1 alongside the
`baseline` / `migrate_to_latest` / inspection trio.

**Test cases:**

| File | What it asserts |
|---|---|
| `80_struct_convert_trivial.das` | All same-name-same-type fields; auto-converter produces identical `NewUser` values; no override block needed. |
| `81_struct_convert_option_flip.das` | `T → Option<T>` auto-wraps with `Some`; `Option<T> → T` without override is a macro_error. |
| `82_struct_convert_primitive_cast.das` | `int → string` emits `to_string`; `string → int` emits `to_int`; round-trips through a fixture without data loss. |
| `83_struct_convert_new_field_with_default.das` | New field `Score : int = 0` auto-fills with 0 from the daslang field initializer. |
| `84_struct_convert_new_option_field.das` | New field `LastLoginAt : Option<int64>` auto-fills with `none()`. |
| `85_struct_convert_new_field_kaboom.das` | New non-Option field with no default → macro_error names the field; override block resolves it cleanly. |
| `86_struct_convert_renamed_from.das` | `@sql_renamed_from = "OldName"` reads from old's `OldName`, writes to new's `NewName`. |
| `87_struct_convert_override_merges.das` | Override block + auto-derived fields combine correctly; dev's overrides take precedence over auto-mappings (re-assignment). |
| `88_create_table_name_override.das` | `create_table(type<T>, name="users_new")` emits `CREATE TABLE users_new` with the struct's full DDL (PKs, indexes via `[sql_index]`, FKs via `@sql_references`). |
| `89_insert_to_name_override.das` | `insert_to(type<T>, row, name="users_new")` writes into `users_new`, not `users`. |
| `90_sql_table_legacy_marker.das` | `[sql_table(name="users", legacy=true)]` allows duplicate name with current `[sql_table(name="users")]`; legacy struct usable with `select_from` but rejected at compile time by `create_table` / `insert` / `_sql_update` (each is a macro_error naming the current non-legacy struct). |
| `91_full_rebuild_end_to_end.das` | The 10-line migration above: insert fixture rows under v6 schema; apply v7; assert all rows preserved with mapped fields; old table gone; new table has FKs + indexes from current struct's annotations. |
| `92_rebuild_without_legacy_struct.das` | Dev who doesn't keep `UserV6`: hand-written `INSERT … SELECT` rebuild also works; `struct_convert` not used. Path-2 of the philosophy validated. |
| `93_rebuild_inside_migrate_to_latest_txn.das` | The rebuild runs inside the big α-shape transaction; if a later migration in the same call fails, the rebuild rolls back too; DB returns to pre-call state. |

---

### Scenario 12 — Squash / baseline old migrations (3 years in) 🟢

**Setup.** Project has been running for 3 years; 200 migrations shipped.
Most users are on v200. New devs / new test environments still have to
apply all 200 from scratch. The standard answer is **squash**: collapse
migrations 1..N into one baseline migration whose body creates the v=N
schema directly.

**Decision (locked 2026-05-04) — no squash tool in v1.**

Three reasons:
1. Demand surfaces only at long timescales (3+ years). v1's audience
   is "first 6 months of dasSQLITE adoption." Years before this hits.
2. The squash recipe is hand-doable today with no tool (below).
3. The real value-add tool is a CLI overlapping significantly with
   `daspkg sql-migrate snapshot` from Scenario 11's diff-emitter
   alternative. If/when that ships, squash falls out as one operation.

**Decision (locked 2026-05-04) — tut 30 documents the hand-squash recipe.**

> "If your project has accumulated more migrations than is comfortable
> (e.g. 100+ over several years), squash old ones into a baseline by
> hand:
>
> 1. Pick a squash point N. Every deployed user must already be past
>    v=N (else they'll hit the baseline problem from Scenario 5).
> 2. Spin up `:memory:` SQLite; apply migrations 1..N; dump the schema
>    (`sqlite3 :memory: '.schema'`).
> 3. Hand-write `[sql_migration(version=N, description="baseline;
>    merges v1..v(N-1)")]` whose body emits that schema via `db |>
>    exec(…)`.
> 4. Delete migrations 1..(N-1) from the codebase.
> 5. Existing users' DBs already have v1..v(N-1) rows; strict-forward
>    pending detection finds nothing; runner is a no-op.
> 6. New users boot against the squashed registry; v=N runs first as
>    the baseline.
>
> One-time operation per squash point. Don't squash unless you need to."

**Decision (locked 2026-05-04) — future tool on the radar, not committed.**

A `daspkg sql-migrate squash --through N` CLI automating steps 2-3
above is plausible future work; lives alongside the snapshot tooling
that Scenario 11 also deferred. Revisit when demand surfaces.

**Test cases:**

| File | What it asserts |
|---|---|
| `94_squash_recipe_doc.das` | Apply migrations v1..v5 to a `:memory:` DB; dump the schema; build a "v5 baseline" migration; verify it produces the same end state on a fresh DB. Locks the recipe in tut 30 as runnable. |
| `95_squash_existing_user_unaffected.das` | DB at v10 (audit has v1..v10); registry switches to "baseline at v5 + v6..v10"; `migrate_to_latest` is a no-op (strict-forward, MAX=10 > registered max=10); audit unchanged. Existing users invisible to the squash. |

---

## Walk complete

All 12 scenarios decided 2026-05-04. The locks below cross-reference
back to scenarios; the **API surface to ship for chunk 14** is the
union of:

- **Core runtime:** `[sql_migration(version=N, description=…, vacuum=…, analyze=…)]`
  annotation; `migrate_to_latest` / `try_migrate_to_latest`;
  `current_schema_version` / `pending_migrations` / `migration_history`
  + `try_*` siblings; `MigrationRecord` + `PendingMigration` structs;
  `with_latest_sqlite(path)` wrapper.
- **Adoption helpers:** `baseline(db, version)` / `try_baseline`;
  Layer-1 warning + Layer-2 enriched-panic logic in
  `migrate_to_latest`.
- **Schema rebuild support:** `struct_convert(type<S>, type<T>)`
  macro; `[sql_table(legacy=true)]` flag; `name=` overload on
  existing `create_table` / `insert_to`.
- **Typed ALTER:** `add_column(type<T>, .Field, default=…)`;
  `create_index(type<T>, fields=…, unique=…, name=…)`;
  `drop_index_if_exists(name)`.
- **Module:** all of the above lives in `daslib/sql_migrate`
  (separate from core `daslib/sql`); `[sql_migration]` annotation
  is the only thing that registers without `require daslib/sql_migrate`
  if convenient (TBD during impl).
- **Audit table:** `__schema_version` 3-column shape (`version`,
  `description`, `applied_at`).
- **Concurrency:** α-shape (one big transaction wraps the whole
  call); coordination via SQLite's RESERVED lock + `busy_timeout`.

**Test surface:** ~95 cases enumerated across the 12 scenarios.

**Tutorial 30 prose** must cover: append-only rule, struct-vs-migration
disconnect, partial-projection principle, migration-bodies-are-DB-only
discipline, expand-contract for safe rolling deploys, baseline pattern
for adoption, fix-forward for rollback, hand-squash recipe, the
EF-comparison callout for rollback. SQLite-3.41 baseline note.

**Mockup (`tutorial-mockup/30-migrations.das.mockup`)** stays as the
v1-spirit reference; it predates several locks and will need a refresh
during chunk implementation to reflect the locked surface (e.g.
3-column audit table, no `success` field in MigrationRecord).

---

---

---

---

---

---

---

---

---

**Test cases:**

| File | What it asserts |
|---|---|
| `09_steady_state_increment.das` | Applied = {1}; registered = {1, 2}; `migrate_to_latest` returns 1; audit = {1, 2}; column from migration_002 visible via `pragma_table_info`. |
| `10_strict_forward_skips_gap.das` | Applied = {1, 3} (via direct INSERT for setup); registered = {1, 2, 3}; `migrate_to_latest` returns 0; audit unchanged; v2 NOT run. Locks the strict-forward decision against silent regression. |
| `11_idempotent_when_current.das` | Applied = {1, 2}; registered = {1, 2}; `migrate_to_latest` returns 0; audit unchanged; no log lines emitted. |
| `12_description_persists.das` | Migration registered with `description="add email column"`; after run, `__schema_version` row's description column matches; logs include the description. |
| `13_data_preserved_across_alter.das` | Insert rows under v1 schema; run v2 (ADD COLUMN with default); verify rows still exist + new column has the default value. The "real users have data" assertion. |
| `14_multi_step_advance.das` | Applied = {1}; registered = {1, 2, 3, 4}; `migrate_to_latest` returns 3; audit ends with `{1, 2, 3, 4}`; log lines count = 3 in version order. |

---
