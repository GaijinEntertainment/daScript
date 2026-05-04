.. _tutorial_sql_schema_from:

================================================
SQL-39 --- ``schema_from``: struct mirrors the DB
================================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; schema_from
    single: Tutorial; check_schema

``[sql_table(schema_from = "path.db")]`` opens the .db at compile
time, reads ``pragma_table_info``, and populates the struct's
fields from the actual schema. The struct mirrors the database ---
which means schema drift becomes a **compile error** at the exact
lines that need updating. No reflection, no migration metadata,
no model snapshots; the type system carries the contract.

Empty body
==========

.. code-block:: das

    require daslib/sql
    require sqlite/sqlite_boost

    [sql_table(name = "Logs",
               schema_from = "tests/dasSQLITE/test_data/schema_from_v2.db")]
    struct LogEntry {}

The macro reads schema_from_v2.db's ``Logs`` table at compile
time and synthesizes the struct as if you'd written:

.. code-block:: das

    [sql_table(name = "Logs")]
    struct LogEntry {
        @sql_primary_key Id : int64
        Severity : int64
        Message  : string
    }

Affinity to daslang type
========================

================== ============================================ ============================
SQLite affinity    Declared types                               daslang type
================== ============================================ ============================
INTEGER            INT/INTEGER/BIGINT/SMALLINT/...              ``int64``
REAL               REAL/DOUBLE/FLOAT                            ``double``
TEXT               TEXT/CHAR/VARCHAR/CLOB                       ``string``
BLOB               BLOB or no declared type                     ``array<uint8>``
NUMERIC            NUMERIC/DECIMAL/BOOLEAN/DATE/DATETIME        ``double``
================== ============================================ ============================

``NOT NULL`` columns synthesize as ``T``; otherwise ``Option<T>``.
``INTEGER PRIMARY KEY`` columns get ``@sql_primary_key`` synthesized.

NUMERIC affinity is the rough edge --- BOOLEAN/DATE columns stored
under NUMERIC end up as ``double`` by default. Override via the
partial-body form below.

Partial body --- hand-declared fields are CONTRACTS
====================================================

A hand-declared field is an *assertion* that the schema looks a
particular way, not an override of the schema. Same-affinity
narrowing is allowed (``int`` instead of synthesized ``int64``);
``@sql_json`` / ``@sql_blob`` overrides are accepted because they
route through the existing custom-types adapter rail.

.. code-block:: das

    // Same-affinity narrowing: int instead of int64 for the PK.
    [sql_table(name = "Logs",
               schema_from = "tests/dasSQLITE/test_data/schema_from_v2.db")]
    struct LogRefined {
        @sql_primary_key Id : int        // narrowed; Severity + Message synthesized
    }

    // Annotation override: @sql_json on a TEXT column tells the macro
    // to bind/extract via JSON encoding for a structured payload.
    struct Note {
        title : string
        rank  : int
    }
    [sql_table(name = "Items",
               schema_from = "items.db")]
    struct Item {
        @sql_json Meta : Option<Note>    // TEXT column on disk; JSON-encoded daslang side
    }

What you cannot do via partial body:

* Declare ``string`` against an INTEGER column --- type mismatch.
* Declare ``T`` (non-optional) against a nullable column --- nullability mismatch.
* Declare ``Option<T>`` against a NOT NULL column --- nullability mismatch.
* Declare ``@sql_primary_key`` on a non-PK column --- PK mismatch.
* Declare a field that doesn't exist in the .db schema.

Each fails at compile time with a message naming the field, the
column, and the source path.

Path resolution
===============

* Absolute path --- as-is.
* Relative path --- resolved under daslang's project root
  (``get_das_root()``).

For .db files alongside the source, pass an absolute path or
arrange the project so the relative form lands under
``get_das_root()``.

``check_schema`` --- runtime startup defense
=============================================

The companion runtime helper validates an open DB matches the
``[sql_table]`` struct. Useful even without ``schema_from`` ---
it's the recommended startup-defense pattern for any code that
opens a DB it didn't just create.

.. code-block:: das

    with_sqlite("logs.db") <| $(db) {
        db |> check_schema(type<LogEntry>)         // panics on mismatch
        // ... or for the Result form:
        // let r = db |> try_check_schema(type<LogEntry>)
        // if (r |> is_some) { panic(r |> unwrap) }
    }

Diagnostics name the column and the divergence (count, name,
SqlType, NOT NULL, PRIMARY KEY).

When NOT to use ``schema_from``
================================

``schema_from`` is for *code-on-current-schema*: the DB's shape
today is what the script reads/writes. ETL between two DBs,
archival readers, admin tooling --- all good fits.

For "the DB grows over time, run versioned schema migrations at
startup", a future ``daslib/sql_migrate`` module ships
``[sql_migration(version=N)]`` + a runtime runner. The two are
orthogonal: ``schema_from`` gives compile-time contract checks
against a known schema; ``sql_migrate`` evolves the schema over
time. See :ref:`tutorial_sql_schema_evolution` for the
multi-version ETL pattern.

AOT semantic-hash interaction
=============================

The macro's output is part of the struct's semantic hash. If the
.db schema changes between AOT-gen and runtime, the AOT link
fails and the runtime falls back to the interpreter --- no silent
miscompile.

.. seealso::

    Full source: :download:`tutorials/sql/39-schema_from.das <../../../../tutorials/sql/39-schema_from.das>`

    Next tutorial: :ref:`tutorial_sql_fts5`

    Multi-version ETL: :ref:`tutorial_sql_schema_evolution`
