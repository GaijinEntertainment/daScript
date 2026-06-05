.. _tutorial_sql_insert_data:

==================================
SQL-02 --- Declare a Table, Insert
==================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; sql_table
    single: Tutorial; create_table
    single: Tutorial; insert
    single: Tutorial; @sql_primary_key

This tutorial introduces ``[sql_table]``, the structure macro that turns a
plain daslang struct into a typed SQL table, plus the user-facing CRUD
helpers ``create_table``, ``drop_table_if_exists``, and ``insert``.

Declaring a table
=================

A table is a daslang ``struct`` annotated with ``[sql_table]``. Field
metadata (``@sql_primary_key`` for now) goes on individual fields:

.. code-block:: das

    [sql_table(name="Cars")]
    struct Car {
        @sql_primary_key Id : int
        Name : string
        Price : int
    }

The ``name="Cars"`` argument sets the on-disk table name. When the
struct name already matches the desired table name, omit it.

What ``[sql_table]`` generates
==============================

At compile time, ``[sql_table]`` reads the struct's fields and field
annotations, then generates a fixed family of helpers attached to the
struct's namespace:

- ``_sql_table_name(type<T>) : string`` — the on-disk name.
- ``_sql_create_table_sql(type<T>) : string`` — the ``CREATE TABLE`` DDL.
- ``_sql_drop_table_if_exists_sql(type<T>) : string`` — the ``DROP`` DDL.
- ``_sql_insert_with_pk_sql(type<T>)`` /
  ``_sql_insert_no_pk_sql(type<T>)`` — the two ``INSERT`` shapes.
- ``_sql_pk_is_unset(row : T) : bool`` — detects an uninitialised PK.
- ``_sql_bind_row(stmt, row : T, include_pk : bool)`` — binds each
  field to a prepared statement.

User code rarely calls these directly — the user-facing CRUD generics
below dispatch to them.

Each field is stored through a bidirectional adapter pair defined
next to its type: ``sql_bind`` maps the value to a SQLite primitive,
``sql_extract`` maps it back. Built-in primitives (every
``int``/``uint`` width, ``float``, ``double``, ``string``,
``array<uint8>``, ``bool``, and enums) ship with pairs. To support a
custom type, add your own pair — no registration required:

.. code-block:: das

    def sql_bind(v : MyType) : string { return ... }
    [unused_argument(t)]
    def sql_extract(v : string; t : type<MyType>) : MyType { return ... }

The primitive ``P`` returned by ``sql_bind`` is one of
``int64`` / ``double`` / ``string`` / ``array<uint8>``, and that
return type alone picks the column's storage class — there is no
separate DDL-type override. Tutorial 26 covers the custom-type
adapter rail in full.

CREATE / DROP
=============

``create_table`` and ``drop_table_if_exists`` are generics
parameterized by ``type<T>``. Each forwards to the macro-emitted DDL
helper and runs the result through ``exec``:

.. code-block:: das

    with_sqlite("test.db") <| $(db) {
        db |> drop_table_if_exists(type<Car>)
        db |> create_table(type<Car>)
    }

Both panic on libsqlite3 error. The non-panicking siblings —
``try_create_table``, ``try_drop_table_if_exists`` — return
``SqlError`` (``= Option<string>``, ``none`` on success).

``check_schema`` --- runtime startup defense
=============================================

After opening a DB you didn't just create, validate that the
on-disk schema matches the ``[sql_table]`` struct on (name,
SqlType, NOT NULL, PRIMARY KEY). Catches column renames, type
changes, missing columns at the startup site instead of letting
them surface as silent wrong-results later.

.. code-block:: das

    with_sqlite("test.db") <| $(db) {
        db |> check_schema(type<Car>)            // panics with a column-named diagnostic on mismatch
        // ... or use try_check_schema for the non-panicking Option form
        // (SqlError = Option<string>: none on match, some(errmsg) on mismatch):
        // let r = db |> try_check_schema(type<Car>)
        // if (r |> is_some) { panic(r |> unwrap) }
    }

Works on any ``[sql_table]`` struct, not just ones derived via
``schema_from`` (:ref:`tutorial_sql_schema_from`). For code where
the struct is the source of truth and the DB might drift,
``check_schema`` is the recommended startup-defense pattern.

INSERT — single row
===================

``insert(db, row)`` prepares the right statement, binds every field via
the convention dispatch, steps once, and returns the assigned rowid:

.. code-block:: das

    with_sqlite("test.db") <| $(db) {
        let new_id = db |> insert(Car(Id=42, Name="Audi", Price=52642))
        // new_id == 42 because Id was specified
    }

When the integer primary-key field is zero, the macro-generated
``_sql_pk_is_unset`` recognises it and the ``insert`` overload picks
the no-PK INSERT form, letting SQLite assign the rowid (covered in
tutorial 3).

INSERT — array of rows (batched)
================================

The ``array<T>`` overload runs every insert inside a single
``BEGIN IMMEDIATE`` / ``COMMIT`` transaction — eight rows = one fsync,
not eight:

.. code-block:: das

    db |> insert([
        Car(Id=1, Name="Audi", Price=52642),
        Car(Id=2, Name="Mercedes", Price=57127),
        Car(Id=3, Name="Skoda", Price=9000),
        Car(Id=4, Name="Volvo", Price=29000),
        Car(Id=5, Name="Bentley", Price=350000),
        Car(Id=6, Name="Citroen", Price=21000),
        Car(Id=7, Name="Hummer", Price=41400),
        Car(Id=8, Name="Volkswagen", Price=21600)
    ])

All rows must agree on PK presence: the first row decides which
``INSERT`` shape to prepare. The transaction rolls back on any
prepare/step error before the panic surfaces.

The non-panicking ``try_insert`` returns
``Result<int64, string>`` — Ok(last rowid) or Err(errmsg).

Quick reference
===============

====================================================  ================================================
Helper                                                Description
====================================================  ================================================
``[sql_table(name="Cars")]``                          Structure macro on a daslang struct
``@sql_primary_key Id : int``                         Field annotation for the integer PK column
``db |> create_table(type<T>)``                       ``CREATE TABLE`` from the struct layout
``db |> drop_table_if_exists(type<T>)``               ``DROP TABLE IF EXISTS``
``db |> insert(row)``                                 Single-row INSERT, returns assigned rowid
``db |> insert(array<row>)``                          Batched INSERT in one transaction
``db |> try_insert(row) : Result<int64, string>``     Non-panic single-row INSERT
``def sql_bind(v : T) : P``                           Adapter: value → SQLite primitive ``P``
``def sql_extract(v : P; t : type<T>) : T``           Adapter: ``P`` → value (``P`` picks storage)
====================================================  ================================================

.. seealso::

    Full source: :download:`tutorials/sql/02-insert_data.das <../../../../tutorials/sql/02-insert_data.das>`

    Previous tutorial: :ref:`tutorial_sql_hello`

    Next tutorial: :ref:`tutorial_sql_last_row_id`
