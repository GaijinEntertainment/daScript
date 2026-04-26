.. _tutorial_sql_select_all:

==========================================
SQL-04 --- Reading Rows with ``_sql``
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; _sql
    single: Tutorial; select_from
    single: Tutorial; _select
    single: Tutorial; _where

Tutorials 1-3 covered opening a database, declaring a table, and writing
rows. This tutorial introduces ``_sql(...)`` --- the call macro that
translates a daslib/linq-shaped chain into SQL at compile time --- and
the ``select_from`` / ``_where`` / ``_select`` chain stages.

Setup
=====

Three requires::

    require daslib/sql
    require sqlite/sqlite_boost
    require sqlite/sqlite_linq

The first two are the standard pair from tutorial 1. The third,
``sqlite/sqlite_linq``, brings in the ``_sql`` and ``_sql_text`` call
macros plus the ``select_from`` runtime entry. The abstract layer
``daslib/sql`` deliberately does *not* re-export sqlite-specific
symbols --- each provider keeps its linq adapter in its own boost
module so a future ``postgres/postgres_linq`` lands cleanly alongside.

Reading every row
=================

The minimal chain ``select_from(type<T>)`` reads every row of the table
into ``array<T>``:

.. code-block:: das

    let cars <- _sql(db |> select_from(type<Car>))
    for (car in cars) {
        to_log(LOG_INFO, "  {car.Name}: {car.Price}\n")
    }

The macro emits the SQL ``SELECT "Id", "Name", "Price" FROM "Cars"``
and the runtime materializes each row into a ``Car`` via the
per-field ``sqlite_read`` overloads ``[sql_table]`` generates. The
move-assign ``<-`` is required because ``array<Car>`` is non-copyable.

Filtering with ``_where`` (bind capture)
========================================

``_where(predicate)`` pipes after the chain root and adds a ``WHERE``
clause. Free variables in the predicate are *captured* and lowered to
``?`` bind parameters --- so the SQL is parameterized, not
string-interpolated:

.. code-block:: das

    let threshold = 200
    let pricey <- _sql(
        db |> select_from(type<Car>) |> _where(_.Price > threshold))
    // emits:  SELECT "Id", "Name", "Price" FROM "Cars" WHERE "Price" > ?
    // binds:  [threshold]

The ``_.Field`` shorthand inside the predicate refers to a column of
the row source. The macro classifies each name in the predicate as
either a column reference (``_.X``) or a captured value (everything
else), and routes them to the SQL identifier vs. bind list
accordingly. Multiple ``_where`` calls compose AND-wise.

Supported operators inside ``_where``: ``==``, ``!=``, ``<``, ``<=``,
``>``, ``>=``, ``&&``, ``||``, ``!``. The captured side may be a
literal or a free variable from any enclosing scope.

Projecting a single column with ``_select``
===========================================

``_select(_.Field)`` projects a single column, producing
``array<FieldType>`` instead of ``array<T>``:

.. code-block:: das

    let names <- _sql(db |> select_from(type<Car>) |> _select(_.Name))
    // emits:  SELECT "Name" FROM "Cars"
    // result: array<string>

This chunk supports the single-column ``_.Field`` form. Multi-column
projections (struct constructors, named-tuple literals) ship in a
later chunk.

Inspecting the emitted SQL with ``_sql_text``
=============================================

``_sql_text`` is the debug companion: same chain, but instead of
running it the macro returns the SQL string that ``_sql`` would have
prepared. ``?`` placeholders stay literal --- the bind values are not
substituted into the text:

.. code-block:: das

    let sql = _sql_text(
        db |> select_from(type<Car>) |> _where(_.Id == 5))
    to_log(LOG_INFO, "{sql}\n")
    // SELECT "Id", "Name", "Price" FROM "Cars" WHERE "Id" = ?

Use this in tests to lock the emission shape, and during development
to confirm the macro is translating what you expect.

Composability
=============

``_sql`` runs as a default ``[call_macro]`` (``canVisitArgument =
true``), so any user-defined call macro that expands to one of the
recognised chain stages composes transparently. A wrapper macro that
expands to ``_where(it, _.Price < threshold)`` slots into a ``_sql``
chain at the same place a literal ``_where`` would, and the analyzer
walks the post-expansion shape uniformly. Tests in
``tests/dasSQLITE/test_07_sql_composability.das`` pin the contract.

Quick reference
===============

==========================================================  =========================================================
Helper                                                      Description
==========================================================  =========================================================
``_sql(chain) -> array<T>``                                 Translate a linq-shaped chain to SQL and materialize
``_sql_text(chain) -> string``                              Same translation, return the SQL string instead
``db |> select_from(type<T>)``                              Chain root --- iterate every row of table T
``... |> _where(predicate)``                                Filter; captured variables become ``?`` binds
``... |> _select(_.Field)``                                 Single-column projection
``... |> _to_array()``                                      No-op terminal --- materializes into ``array<T>``
==========================================================  =========================================================

.. seealso::

    Full source: :download:`tutorials/sql/04-select_all.das <../../../../tutorials/sql/04-select_all.das>`

    Previous tutorial: :ref:`tutorial_sql_last_row_id`
