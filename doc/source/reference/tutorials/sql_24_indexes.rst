.. _tutorial_sql_indexes:

==========================================
SQL-24 --- Indexes
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; index
    single: Tutorial; CREATE INDEX
    single: Tutorial; sql_index
    single: Tutorial; sql_unique
    single: Tutorial; UNIQUE INDEX
    single: Tutorial; composite index

``@sql_unique`` is the concise field-level form for one-column
uniqueness. ``[sql_index]`` is the configurable sibling annotation for
named, non-unique, unique, and composite indexes. Both emit portable
``CREATE [UNIQUE] INDEX`` statements after ``CREATE TABLE``.

Sibling-annotation shape
========================

Both annotations live in the **same** bracket pair, comma-separated,
with ``[sql_table]`` first.

.. das-doc: given var inscope db = open_sqlite(":memory:")

.. code-block:: das

    [sql_table(name = "Users"),
     sql_index(fields = "Email", unique = true),
     sql_index(fields = ("City", "LastSeen")),
     sql_index(fields = "LastSeen", name = "ix_users_lastseen")]
    struct User {
        @sql_primary_key Id : int
        Email : string
        @sql_unique
        Name : string
        City : string
        LastSeen : int64
    }

Argument reference
==================

==============================================  ===================================================
Argument                                        Meaning / default
==============================================  ===================================================
``fields = "Col"`` or ``fields = ("A", "B")``   single column (string) or composite (tuple)
``unique = true``                               UNIQUE INDEX (default ``false``)
``name = "..."``                                explicit index name (default ``idx_<table>_<col1>_<col2>``)
==============================================  ===================================================

``[sql_table]`` validates every field name against the struct's
fields at macro-expansion time. A misspelling fails the macro with
``error[20800]`` naming the offending field and the struct.

DDL emitted
===========

.. code-block:: text

    CREATE TABLE "Users" (...);
    CREATE UNIQUE INDEX "uq_Users_Name" ON "Users" ("Name");
    CREATE UNIQUE INDEX "idx_Users_Email" ON "Users" ("Email");
    CREATE INDEX "idx_Users_City_LastSeen" ON "Users" ("City", "LastSeen");
    CREATE INDEX "ix_users_lastseen" ON "Users" ("LastSeen");

The DDL helper is callable directly --- handy for migration scripts
and asserts.

.. code-block:: das

    let idx_sql = _sql_create_indexes_sql(type<User>)

Field-level single-column uniqueness
====================================

``@sql_unique`` creates a provider-neutral UNIQUE INDEX with the
generated name ``uq_<table>_<column>``. It goes through
``_sql_create_indexes_sql`` instead of being embedded in SQLite-style
column DDL, so the same annotation is enforced by SQLite, DuckDB, and
PostgreSQL.

.. das-doc: fragment

.. code-block:: das

    struct User {
        @sql_primary_key Id : int
        @sql_unique
        Name : string
    }

    // CREATE UNIQUE INDEX "uq_Users_Name" ON "Users" ("Name")

Use ``[sql_index(fields = "Name", unique = true, name = "...")]``
instead when the index name must be controlled explicitly.

Indexes are transparent at the query side
=========================================

User queries don't change shape; the SQLite planner picks the index
automatically.

.. code-block:: das

    let alice = _sql(db |> select_from(type<User>)
                        |> _where(_.Email == "alice@x.com") |> _first())
    // SELECT ... FROM "Users" WHERE "Email" = ? LIMIT 1
    // (uses idx_Users_Email)

UNIQUE-INDEX violations through ``try_insert``
==============================================

.. code-block:: das

    let dup = User(Id = 4, Email = "alice@x.com", Name = "alice2",
                   City = "LA", LastSeen = 0l)
    let res = db |> try_insert(dup)
    if (res |> is_err) {
        // "UNIQUE constraint failed: Users.Email"
    }

Composite UNIQUE for upsert composite-conflict targets
======================================================

``_sql_upsert(row, tuple(_.A, _.B), do_update)``
(:ref:`tutorial_sql_upsert`) requires a UNIQUE constraint covering
the same column set. ``[sql_index(unique = true, fields = ("A", "B"))]``
is how you declare one --- there is no separate
``[sql_unique]`` table-level annotation.

Single-column uniqueness can use either ``@sql_unique`` on the field
or ``[sql_index(unique = true, fields = "Col")]``. Both work on
SQLite, DuckDB, and PostgreSQL and both satisfy the upsert conflict
target. The index form is preferable when you need to control its name.

.. seealso::

    Full source: :download:`tutorials/sql/24-indexes.das <../../../../tutorials/sql/24-indexes.das>`

    Previous tutorial: :ref:`tutorial_sql_foreign_keys`

    Next tutorial: :ref:`tutorial_sql_defaults_computed`
