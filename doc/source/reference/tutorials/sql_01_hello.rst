.. _tutorial_sql_hello:

===========================
SQL-01 --- Hello dasSQLITE
===========================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; dasSQLITE
    single: Tutorial; with_sqlite
    single: Tutorial; query_scalar

This tutorial introduces the dasSQLITE high-level API: opening a database
with RAII, reading the linked SQLite version, and running a one-shot scalar
query.

Setup
=====

Two requires::

    require daslib/sql
    require sqlite/sqlite_boost

``daslib/sql`` is the abstract layer — it re-exports ``Option<T>`` and
``Result<T, string>`` so user code only needs one import for those types.
``sqlite/sqlite_boost`` is the SQLite provider; it ships ``SqlRunner``,
``with_sqlite``, and the CRUD helpers used in every tutorial.

Library version
===============

``sqlite_version()`` reads the linked libsqlite3 version. It needs no
database connection:

.. code-block:: das

    print("sqlite version: {sqlite_version()}\n")
    // sqlite version: 3.45.0

Opening a database
==================

The canonical form is ``with_sqlite(path) <| $(db) { ... }``. It opens the
database, passes a ``SqlRunner`` handle to the block, and closes the
connection on block exit — including on panic and early return:

.. code-block:: das

    with_sqlite(":memory:") <| $(db) {
        // db is a SqlRunner
    }

``:memory:`` opens an in-memory database. Use a filesystem path to open or
create a database file. Failure (path not writable, disk full, etc.) panics
with the libsqlite3 ``errmsg`` text. ``with_sqlite`` is the strict form;
under the hood it calls ``open_sqlite``, which itself wraps
``try_open_sqlite(path) : Result<SqlRunner, string>`` (``ok(runner)`` on
success, ``err(errmsg)`` on failure). Tutorial 6 walks the full ``try_`` /
``_opt`` rail; for now the strict form is the right default.

Each future SQL provider ships its own ``with_<provider>`` helper —
``with_postgres``, ``with_mysql``, etc. The block body is the same shape
across providers.

Declaration form
================

If the database lifetime is the function body or a struct field, use
``var inscope`` directly — ``finalize`` on ``SqlRunner`` runs at scope
exit::

    var inscope db = open_sqlite(":memory:")
    // ... use db ...
    // sqlite3_close_v2 fires here

The ``with_sqlite`` form is preferred when the lifetime is a single
block; it's harder to forget the ``inscope``.

Running a scalar query
======================

``query_scalar`` prepares a statement, steps once, reads column 0,
finalizes, and returns the value:

.. code-block:: das

    with_sqlite(":memory:") <| $(db) {
        let v = db |> query_scalar("SELECT SQLITE_VERSION()", type<string>)
        print("server reports: {v}\n")
    }

The result type is passed as a ``type<...>`` witness — daslang's gen2
convention for type-parameterized functions. ``query_scalar`` is generic
over the daslang reader-adapter rail (tutorial 26), so any type with an
``sql_extract`` adapter works: ``string``, ``int``, ``int64``, ``double``,
``float``, ``bool``, ``array<uint8>`` (BLOB), and user-defined custom
types all read through the same call.

``query_scalar`` panics on prepare/step error and on zero rows. The
``try_query_scalar(db, sql, type<T>) : Result<T, string>`` sibling
returns the libsqlite3 ``errmsg`` on any failure; tutorial 6 covers it
alongside the rest of the ``try_`` rail.

.. seealso::

    Full source: :download:`tutorials/sql/01-version.das <../../../../tutorials/sql/01-version.das>`

    Next tutorial: :ref:`tutorial_sql_insert_data`
