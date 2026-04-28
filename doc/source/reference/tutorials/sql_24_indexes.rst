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
    single: Tutorial; UNIQUE INDEX
    single: Tutorial; composite index

``[sql_index]`` is a sibling annotation to ``[sql_table]``: each
``[sql_index]`` emits one ``CREATE [UNIQUE] INDEX`` statement, run
right after the ``CREATE TABLE`` inside the same transaction.

Sibling-annotation shape
========================

Both annotations live in the **same** bracket pair, comma-separated,
with ``[sql_table]`` first.

.. code-block:: das

    [sql_table(name = "Users"),
     sql_index(fields = "Email", unique = true),
     sql_index(fields = ("City", "LastSeen")),
     sql_index(fields = "LastSeen", name = "ix_users_lastseen")]
    struct User {
        @sql_primary_key Id : int
        Email : string
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
fields at macro-expansion time. Misspellings produce a compile error
listing the valid columns.

DDL emitted
===========

.. code-block:: text

    CREATE TABLE "Users" (...);
    CREATE UNIQUE INDEX "idx_Users_Email" ON "Users" ("Email");
    CREATE INDEX "idx_Users_City_LastSeen" ON "Users" ("City", "LastSeen");
    CREATE INDEX "ix_users_lastseen" ON "Users" ("LastSeen");

The DDL helper is callable directly --- handy for migration scripts
and asserts.

.. code-block:: das

    let idx_sql = _sql_create_indexes_sql(type<User>)

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
(emits ``UNIQUE`` directly in the column DDL) or
``[sql_index(unique = true, fields = "Col")]`` (separate
``CREATE UNIQUE INDEX``). Both work for the upsert path; the index
form is preferable when you also need to control the index name.

.. seealso::

    Full source: :download:`tutorials/sql/24-indexes.das <../../../../tutorials/sql/24-indexes.das>`

    Previous tutorial: :ref:`tutorial_sql_foreign_keys`

    Next tutorial: :ref:`tutorial_sql_defaults_computed`
