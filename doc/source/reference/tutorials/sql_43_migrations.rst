.. _tutorial_sql_migrations:

================================================
SQL-43 --- versioned schema migrations
================================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; migrations
    single: Tutorial; sql_migration
    single: Tutorial; baseline
    single: Tutorial; with_latest_sqlite

A DB file holds *schema* (tables, columns, indexes) and *data*
(rows). When v1 of an app ships, users start filling tables.
When v2 ships with a new column, the user's existing ``app.db``
still has the v1 shape --- v2 code that queries the new column
panics on the first call. Migrations solve this: each schema
change ships as a numbered, frozen step, and the runtime applies
whatever is missing on every startup.

Three rules make this work:

1. **Migrations are append-only.** Once shipped, a migration is
   FROZEN --- to change anything, write a new one with a higher
   version number. A user whose DB is already past version K
   never re-runs anything ``<= K``; edits to old migrations are
   invisible to existing DBs and silent divergence between fresh
   and existing DBs is the result. So: don't edit, add.

2. **Versions are unique across the whole program.** Two devs
   picking ``version=N`` on parallel branches is a compile-time
   error --- the macro scans every loaded module's
   ``[sql_migration]`` annotations and refuses the dup. The
   error message names both function names + both module paths,
   and a runtime belt-and-suspenders check guards against any
   case that slips through (dynamic plugin loading, etc.).

3. **The runner uses one transaction per call.** All pending
   migrations + their audit-table inserts run inside a single
   ``BEGIN IMMEDIATE`` / ``COMMIT``. If any migration fails,
   every effect rolls back --- including prior migrations that
   succeeded earlier in the same call. Re-run replays the whole
   pending list cleanly.

The struct describes the CURRENT schema. Old shapes live ONLY in
the migration body's raw SQL --- daslang has no model-snapshot
system on disk per migration. Migrations 1 through N-1 use raw
``db |> exec(...)`` for whatever the schema looked like at that
point.

Define a schema and write migrations
=====================================

.. code-block:: das

    require sqlite/sqlite_migrate

    [sql_table(name = "users")]
    struct User {
        @sql_primary_key Id : int
        Name : string
        Email : string                 // added in migration 2
        Score : int = 0                // added in migration 3
    }

    [sql_migration(version = 1, description = "create users")]
    def migration_001(db : SqlRunner) {
        db |> exec("CREATE TABLE users (Id INTEGER PRIMARY KEY, Name TEXT NOT NULL)")
    }

    [sql_migration(version = 2, description = "add email column")]
    def migration_002(db : SqlRunner) {
        db |> add_column(type<User>, "Email", "")
    }

    [sql_migration(version = 3, description = "add score; backfill", analyze = true)]
    def migration_003(db : SqlRunner) {
        db |> add_column(type<User>, "Score", 0)
        db |> exec("UPDATE users SET Score = 100 WHERE Email != ''")
    }

A migration body is just a daslang function. Inside it: raw
``exec``, parameterized ``exec``, ``_sql_update``, helper
function calls, ``for``/``if`` --- whatever it takes to advance
one step. Multiple statements run inside the same transaction;
DDL and DML mix freely (SQLite supports transactional DDL).

Annotation parameters:

- ``version=N`` --- required, positive integer, unique across
  the program.
- ``description=...`` --- optional. Shown in ``LOG_INFO``
  and stored in ``__schema_version``.
- ``vacuum=true`` --- optional. Runs ``VACUUM`` once after the
  call commits, regardless of how many migrations in the batch
  set the flag.
- ``analyze=true`` --- same shape, runs ``ANALYZE``. If both
  appear in a batch, ANALYZE runs first, then VACUUM.

App startup
============

Preferred form combines ``with_sqlite`` +
``apply_recommended_pragmas`` + ``migrate_to_latest`` into one
call:

.. code-block:: das

    [export]
    def main() {
        with_latest_sqlite("app.db") <| $(db) {
            // DB is now at the latest schema. App-level logic goes here.
            db |> exec("INSERT INTO users (Name, Email) VALUES ('alice', 'alice@x')")
        }
    }

The explicit two-step form (``with_sqlite`` then
``db |> migrate_to_latest()``) stays available for tools / CI
that want to inspect state before migrating.

Cost on a fully-up-to-date DB is a few cheap reads ---
``CREATE TABLE IF NOT EXISTS`` is a no-op on the existing audit table,
plus a ``COUNT(*)`` adoption probe and a ``MAX(version)`` lookup.
Microseconds total. Safe to call every startup.

Inspection: pure reads, no side effects
========================================

Three functions answer "what's the migration state?" without
running anything:

.. code-block:: das

    db |> current_schema_version()    // int (0 if no audit table yet)
    db |> pending_migrations()        // array<PendingMigration>
    db |> migration_history()         // array<MigrationRecord>

All three never create the audit table, never write a row, never
run a migration. Safe to call from ``mytool db status`` against a
brand-new DB. ``Result``-bound ``try_*`` siblings exist for tools
that want explicit error handling.

Description sourcing differs by call:

- ``pending_migrations`` reads from the **annotation** (current
  code; reflects what *will be*).
- ``migration_history`` reads from the **audit row** (frozen
  at apply-time; reflects what *was*).

So editing a description on an already-shipped migration is a
no-op for any DB that's already past it --- the audit row keeps
the old text. Fresh installs pick up the new text on first
apply.

Typed ALTER: when daslang has enough info to validate
======================================================

Migrations 2 and 3 above use the typed surface in
``daslib/sqlite_boost``:

.. code-block:: das

    db |> add_column(type<T>, "Field" [, defaultLit])
    db |> create_index(type<T>, "Field" | ("A","B") [, "name"])
    db |> create_unique_index(type<T>, ... same shape ...)
    db |> drop_index_if_exists("name")

Field selectors are **string literals**, not ``.Field`` syntax:
gen2's parser only accepts ``.Field`` inside an ``_sql {...}``
block. Plain call sites pass explicit string field names ---
the same convention ``[sql_index(fields="A")]`` already uses.

What the typed forms buy you:

1. **Compile-time field-name checks.**
   ``add_column(type<T>, "Emial")`` fails at compile time, not
   on the user's DB at startup.

2. **Type-derived NOT NULL.** Forgetting ``NOT NULL + DEFAULT``
   used to mean existing rows panic on the next read after the
   migration. The macro derives nullability from
   ``Option<>`` wrapping and refuses to ADD a non-nullable
   column without a literal default.

3. **Annotation reuse.** ``@sql_column`` rename + ``@sql_json``
   / ``@sql_blob`` storage are honored automatically --- same
   conventions as ``[sql_table]`` / ``[sql_index]``, no
   double-bookkeeping.

A second migration that adds an index and a unique composite,
with an idempotent rebuild pattern via
``drop_index_if_exists``:

.. code-block:: das

    [sql_migration(version = 4, description = "index Email")]
    def migration_004(db : SqlRunner) {
        db |> create_index(type<User>, "Email")
    }

    [sql_migration(version = 5, description = "unique (Email, Name)")]
    def migration_005(db : SqlRunner) {
        db |> drop_index_if_exists("ux_email_name")
        db |> create_unique_index(type<User>, ("Email", "Name"), "ux_email_name")
    }

What stays on raw ``db |> exec(...)``:

- DROP COLUMN / RENAME COLUMN --- old names aren't in the
  current struct, so daslang has nothing to validate against.
- PK / UNIQUE inline / generated columns added post-hoc ---
  SQLite can't ALTER these in place; the rebuild path below
  handles them.
- ``CHECK`` constraints, FK ADD/DROP, column type changes.
- Anything ad-hoc that doesn't map to a struct field.

Schema rebuild via ``struct_convert``
======================================

SQLite's ``ALTER`` vocabulary handles ADD COLUMN, RENAME COLUMN,
RENAME TABLE, and DROP COLUMN cleanly. Everything else --- PK
changes, type narrowing, FK alterations, ``CHECK`` changes ---
needs a *rebuild*: build a new table with the desired shape, copy
the rows through a converter, drop the old, rename. SQLite docs
call this the "12-step recipe"; daslang collapses it to three
lines of user code.

Three pieces work together:

1. ``[sql_table(name=..., legacy=true)]`` keeps the OLD shape as
   a *historical* struct. Read-only --- usable with
   ``select_from`` and ``drop_table_if_exists``, but the compiler
   refuses ``create_table`` / ``insert`` / ``update`` / ``delete``
   on it (the write-side helpers are not emitted; the natural
   unresolved-overload error fires at the call site).

2. ``[struct_convert] def my_v1_to_v2(old : S; var dst : T) {...}``
   is a function annotation that walks T's fields. For each one
   it emits a single dispatch call:
   ``_::struct_convert_field(tgt.X, src.X_or_renamed)``.

   The conversion itself lives in an overloaded
   ``struct_convert_field`` set. dasSQLITE ships:

   - **Identity** (``T -> T``) --- ``dst = src``.
   - **``S -> Option<T>`` wrap** --- recurses on the inner
     ``S -> T``, then wraps with ``some()``. So ``int -> Option<string>``
     works because the inner ``int -> string`` overload fires.
   - **``Option<S> -> T`` unwrap** --- NULL collapses to
     ``default<S>``, then recurses on the inner ``S -> T``.
   - **``Option<S> -> Option<T>`` cross-payload** --- unwraps,
     dispatches the inner conversion, re-wraps (NULL stays NULL).
   - **Primitive targets** --- ``int`` / ``int64`` / ``float`` /
     ``double`` / ``string`` from any source via the type's
     constructor (``int(src)``, ``int64(src)``, ...) or string
     interp.

   To extend, drop your own overload in your module --- the
   macro emits ``_::struct_convert_field(...)`` so the call
   resolves at the user-side overload set::

       def struct_convert_field(var dst : MyEnum&; src : int) : void {
           dst = MyEnum(src)
       }

   Now ``int -> MyEnum`` auto-derives. The ``Option<MyEnum> -> int``
   path also picks up your overload via the recursive dispatch.

   The macro itself still handles: ``@sql_renamed_from = "OldName"``
   lookup (read from old's renamed field), default-init for fields
   absent from S (uses ``T``'s field initializer or ``none()`` for
   ``Option<T>``), body validation (must be a brace block), and
   user-override detection. An explicit ``dst.X = ...`` (or ``<-``,
   ``:=``) on the LHS in the body suppresses the auto-fill for X.

3. ``db |> convert_and_rename(type<S>, type<T>)`` runs the
   whole rebuild: CREATE staging table, copy rows through the
   converter, DROP original, ALTER ... RENAME staging -> target.
   Lower-level ``db |> convert(type<S>, type<T>, name=...)`` is
   available if you only need the staging step (and want to
   manage the swap separately).

The rebuild runs inside ``migrate_to_latest``'s big transaction
--- a later migration failing rolls the rebuild back too.

.. code-block:: das

    [sql_table(name = "users", legacy = true)]
    struct UserV6 {
        @sql_primary_key Id : int
        Name : string
        LegacyEmail : string
    }

    [sql_table(name = "users")]
    struct User {
        @sql_primary_key Id : int
        Name : string
        Email : string
    }

    [struct_convert]
    def my_v6_to_v7(old : UserV6; var dst : User) {
        dst.Email = (old.LegacyEmail != "")
            ? old.LegacyEmail
            : "unknown@example.com"
    }

    [sql_migration(version = 7, description = "restructure users")]
    def migration_007(db : SqlRunner) {
        db |> convert_and_rename(type<UserV6>, type<User>)
    }

The auto-rules pick up ``Id`` and ``Name`` (same-name same-type);
the user body's one line handles ``Email`` (the new field).
``LegacyEmail`` is in S but not in T --- it gets dropped on
conversion. After the rebuild, the table named ``users`` has the
new shape with rows preserved.

Path 2 (no legacy struct): hand-written
``db |> exec("INSERT INTO users_new (...) SELECT (...) FROM users")``
also works. The legacy struct is convenience, not a requirement.

Adopting migrations on an existing DB
======================================

Common case: the app has been shipping without migrations for a
year. Existing user DBs already have ``users`` from hand-rolled
DDL. A naive swap to ``with_latest_sqlite`` would crash ---
``migration_001``'s ``CREATE TABLE`` fails because the table
already exists.

``baseline(db, version=N)`` stamps v1..vN as already-applied
without running their bodies. Idempotent (versions already in
the audit are skipped); safe to leave the call permanently
in startup code, though removing it after one release cycle
keeps things tidy.

.. code-block:: das

    [export]
    def main() {
        with_sqlite("app.db") <| $(db) {
            db |> baseline(1)             // mark v1 as already applied
            db |> migrate_to_latest()     // applies v2 + v3 only
        }
    }

The runner gives two layers of UX safety net for the naive
adoption shape:

- **Layer 1 (proactive warning).** When ``migrate_to_latest``
  detects user objects in a freshly-empty audit table, it logs a
  ``LOG_WARNING`` with the baseline recipe before running any
  migration body.

- **Layer 2 (enriched panic).** If the first migration body
  fails AND the audit was empty before the call, the panic
  message appends the same baseline hint. Same hint reaches
  ``try_migrate_to_latest``'s ``Result::Err``.

Recommended default in new code: prefer first-change-as-first-
migration --- your first new migration ALTERs the existing
schema rather than recreating it. ``baseline`` is the explicit
escape for when you want CREATE-TABLE-shaped retroactive
history.

What does NOT ship
===================

- **Down migrations / rollback.** No ``[sql_migration_down]``,
  no ``migrate_to(version=N-1)``. Down migrations are a design
  anti-pattern --- implicit data loss, retroactive maintenance
  burden, never tested in production conditions, the thing
  people actually want is "go back to before I deployed v7"
  which is a backup-restore concern. Recommended response to a
  bad migration: write the next migration to fix it forward.

- **Concurrent runners across NFS / shared volumes.** OS file
  locking is unreliable on NFS, SMB, GlusterFS, EFS. SQLite
  docs explicitly warn against this; ``migrate_to_latest``'s
  concurrency safety degrades to "mostly works." Run migrations
  as a separate explicit step before launching app instances on
  shared storage.

- **Compile-time DDL emission to a .sql file.** ``dotnet ef
  migrations script`` analogue. Future work; not blocking.

- **Squash tool.** Hand-recipe in API_MIGRATION.md when a
  project accumulates 100+ migrations and needs collapsing.
  Future ``daspkg sql-migrate squash`` is plausible but
  deferred.

- **A separate CLI** for running migrations outside the app.
  Future work, also dovetails with the eventual ``dasSQL``
  abstraction layer.

.. seealso::

    Full source: :download:`tutorials/sql/43-migrations.das <../../../../tutorials/sql/43-migrations.das>`

    Read-side multi-version ETL: :ref:`tutorial_sql_schema_evolution`

    Schema introspection from existing DBs: :ref:`tutorial_sql_schema_from`
