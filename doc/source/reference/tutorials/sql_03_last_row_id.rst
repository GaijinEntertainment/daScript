.. _tutorial_sql_last_row_id:

=====================================
SQL-03 --- Auto-Increment Primary Key
=====================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; AUTOINCREMENT
    single: Tutorial; last_insert_rowid
    single: Tutorial; @sql_primary_key

Tutorial 2 inserted ``Car`` rows with explicit ``Id`` values. This
tutorial shows what happens when you omit the integer primary key —
SQLite assigns the rowid for you, and ``insert`` returns it.

INTEGER PRIMARY KEY ↔ ROWID
===========================

In SQLite, ``INTEGER PRIMARY KEY`` is a special case: the column is
the table's ROWID alias and gets auto-assigned on insert when omitted.
``[sql_table]`` maps any ``int`` or ``int64`` field marked
``@sql_primary_key`` to ``INTEGER PRIMARY KEY`` for exactly this
behaviour:

.. code-block:: das

    [sql_table(name="Friends")]
    struct Friend {
        @sql_primary_key Id : int
        Name : string
    }

The generated DDL is
``CREATE TABLE "Friends"("Id" INTEGER PRIMARY KEY, "Name" TEXT NOT NULL)``.

Inserting without an Id
=======================

Construct a row with the PK field unset (zero) and the macro-generated
``_sql_pk_is_unset(row)`` returns ``true``. The ``insert`` overload
then picks the no-PK INSERT shape
(``INSERT INTO "Friends" ("Name") VALUES (?)``), letting SQLite
assign the rowid. The returned ``int64`` is the assigned id:

.. code-block:: das

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
        // The last Id of the inserted row is 5
    }

For an array insert, the returned ``int64`` is the rowid of the *last*
inserted row (consistent with ``sqlite3_last_insert_rowid``
semantics).

Reading the last rowid directly
===============================

``last_insert_rowid`` is the direct wrapper:

.. code-block:: das

    let id = db |> last_insert_rowid()

Use this when you need the most recent rowid in a context where the
``insert`` return value is no longer in scope.

Mixing explicit and assigned PKs
================================

If you supply ``Friend(Id=42, Name="X")``, the macro routes through
the with-PK INSERT shape and uses your value verbatim — provided no
existing row has the same Id. A duplicate Id raises
``SQLITE_CONSTRAINT`` and panics; the non-panicking ``try_insert``
returns ``Err(errmsg)`` instead. Tutorial 6 walks the
constraint-violation handling story.

Quick reference
===============

==========================================  ================================================
Helper                                      Description
==========================================  ================================================
``@sql_primary_key Id : int``               Maps to ``INTEGER PRIMARY KEY``
``Friend(Name="Tom")``                      Construct with PK = 0 → SQLite assigns rowid
``db |> insert(row) : int64``               Single-row INSERT, returns assigned rowid
``db |> insert(array<row>) : int64``        Batched INSERT, returns *last* assigned rowid
``db |> last_insert_rowid() : int64``       Wraps ``sqlite3_last_insert_rowid``
==========================================  ================================================

.. seealso::

    Full source: :download:`tutorials/sql/03-last_row_id.das <../../../../tutorials/sql/03-last_row_id.das>`

    Previous tutorial: :ref:`tutorial_sql_insert_data`

    Next tutorial: :ref:`tutorial_sql_select_all`
