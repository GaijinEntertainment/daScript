.. _tutorial_sql_schema_evolution:

================================================
SQL-42 --- multi-version ETL with schema_from
================================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; schema evolution
    single: Tutorial; ETL
    single: Tutorial; schema_from
    single: Tutorial; typeinfo has_field

This tutorial uses :ref:`tutorial_sql_schema_from` as a building
block. Read it first if you haven't.

Imagine you've got ``Logs`` databases in the wild at v1 (no
``Severity`` column) and v2 (``Severity`` added). You want a
single program that reads from either, normalizes, and writes to
a fresh DB. With ``[sql_table(schema_from=...)]`` the v1/v2
structs come from the actual schemas; with ``typeinfo
has_field<...>`` + ``static_if`` one function body handles both
at compile time. Drift in either source DB stops the build at
the exact lines that need updating.

One struct per known historical shape
======================================

.. code-block:: das

    require daslib/sql
    require sqlite/sqlite_boost
    require sqlite/sqlite_linq

    [sql_table(name = "Logs",
               schema_from = "tests/dasSQLITE/test_data/schema_from_v1.db")]
    struct OldLogV1 {}

    [sql_table(name = "Logs",
               schema_from = "tests/dasSQLITE/test_data/schema_from_v2.db")]
    struct OldLogV2 {}

    // The current shape — hand-declared, lives in code, evolves with the app.
    [sql_table(name = "Logs")]
    struct LogEntry {
        @sql_primary_key Id : int64
        Severity : int64
        Message  : string
    }

Generic migration body with ``static_if`` branching
====================================================

``static_if (typeinfo has_field<Severity>(type<TT>))`` lets one
generic body handle both v1 and v2 sources. Compile-time
branching; no runtime cost. If a future version drops
``Severity``, the v1 branch kicks in automatically. If a version
removes a field ``LogEntry`` still expects (``Id`` or
``Message``), the typing fails to compile at the assignment
line.

.. code-block:: das

    def migrate_one_row(src : auto(TT)) : LogEntry {
        var dst : LogEntry
        dst.Id = src.Id
        dst.Message = src.Message
        static_if (typeinfo has_field<Severity>(type<TT>)) {
            dst.Severity = src.Severity
        } else {
            dst.Severity = 1l        // backfill: missing-Severity rows = info
        }
        return dst
    }

Streaming with ``_each_sql``
============================

For large old DBs, materialize-then-iterate uses memory
proportional to the result set. ``_each_sql`` is the streaming
iterator --- one row at a time, no full-array materialization.

.. code-block:: das

    [export]
    def main() {
        with_sqlite(":memory:") <| $(newDb) {
            newDb |> create_table(type<LogEntry>)

            with_sqlite("schema_from_v1.db") <| $(oldDb) {
                for (e in _each_sql(oldDb |> select_from(type<OldLogV1>))) {
                    newDb |> insert(migrate_one_row(e))
                }
            }
            with_sqlite("schema_from_v2.db") <| $(oldDb) {
                for (e in _each_sql(oldDb |> select_from(type<OldLogV2>))) {
                    newDb |> insert(migrate_one_row(e))
                }
            }
        }
    }

Same ``migrate_one_row``, different compile-time branch.

When this is the right tool
============================

``schema_from`` + ``typeinfo has_field`` is for ETL, archival
readers, one-shot data movement scripts. The script ships with
the historical .db schemas baked in. If a new historical version
surfaces, you add a third struct + a third loop and the type
system tells you exactly where to wire it up.

For "my app's schema evolves at runtime; I run migrations at
startup", a future ``daslib/sql_migrate`` module ships a
different shape: versioned ``[sql_migration(version=N)]``
functions applied in order, tracked in ``__schema_version``,
transactional per migration. The two patterns coexist:
``schema_from`` for *code-on-current-schema*, ``sql_migrate``
for the *schema-grows-over-time* runner.

.. seealso::

    Full source: :download:`tutorials/sql/42-schema_evolution.das <../../../../tutorials/sql/42-schema_evolution.das>`

    Background: :ref:`tutorial_sql_schema_from`

    Streaming iteration: :ref:`tutorial_sql_streaming`
