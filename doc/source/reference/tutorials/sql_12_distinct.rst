.. _tutorial_sql_distinct:

==================================
SQL-12 --- ``distinct``
==================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; distinct
    single: Tutorial; DISTINCT

``distinct()`` flips a single bool flag on the analyzer's
``SqlQuery`` and emits ``SELECT DISTINCT`` instead of plain
``SELECT``. The rest of the chain (column list, ``WHERE``,
``ORDER BY``, ``LIMIT``) composes normally.

Full-row DISTINCT
=================

Without a ``_select``, every column is in the row --- DISTINCT
deduplicates whole rows:

.. code-block:: das

    let all_rows <- _sql(db |> select_from(type<Car>) |> distinct())
    // SELECT DISTINCT "Id", "Name", "Price" FROM "Cars"

Single-column DISTINCT
======================

Project one column with ``_select(_.Field)`` and dedupe it:

.. code-block:: das

    let names <- _sql(db |> select_from(type<Car>)
                        |> _select(_.Name)
                        |> distinct())
    // SELECT DISTINCT "Name" FROM "Cars"

Composes with ``_where``
========================

.. code-block:: das

    let names_over_100 <- _sql(db |> select_from(type<Car>)
                                 |> _where(_.Price > 100)
                                 |> _select(_.Name)
                                 |> distinct())
    // SELECT DISTINCT "Name" FROM "Cars" WHERE "Price" > ?

Distinct + first-row aggregates
===============================

``_distinct_by(_.K)`` followed by an aggregate terminator
(``_count(_.field > X)`` / ``_long_count(_.field > X)``, or
``_select(_.F) |> sum()`` / ``min()`` / ``max()`` / ``average()``)
lowers to a SQLite bare-aggregate subquery that picks the row with the
smallest ``@sql_primary_key`` per group:

.. code-block:: das

    // Count distinct brands whose first-encountered Car has Year > 2009
    let n = _sql(db |> select_from(type<Car>)
                    |> _distinct_by(_.Brand)
                    |> _count(_.Year > 2009))
    // SELECT COUNT(*) FROM (SELECT *, MIN("Id") FROM "Cars" GROUP BY "Brand") AS "t0" WHERE "Year" > ?

    // Same shape with long_count for int64 result
    let n64 = _sql(db |> select_from(type<Car>)
                      |> _distinct_by(_.Brand)
                      |> _long_count(_.Year > 2009))
    // (identical SQL; result type widens to int64)

    // Sum of price of the first-encountered Car per brand
    let total = _sql(db |> select_from(type<Car>)
                        |> _distinct_by(_.Brand)
                        |> _select(_.Price)
                        |> sum())
    // SELECT SUM("Price") FROM (SELECT *, MIN("Id") FROM "Cars" GROUP BY "Brand") AS "t0"

"First-encountered" = the row with the minimum ``@sql_primary_key`` value
per group. For tables where the PK is monotonic with insertion order (the
common case), this matches the array/decs ``_distinct_by`` semantics
("first row by source order"). Requires a single-column
``@sql_primary_key`` on the source table; composite keys reject.

Inside ``_sql(...)`` you must use the ``_count(_.field > X)`` /
``_long_count(_.field > X)`` linq_boost shorthand for the predicate form
— the explicit lambda form ``count($(c) => c.field > X)`` fails type
inference because ``_sql``'s macro boundary blocks the typer from
binding ``c`` through unexpanded chain operators (same constraint as
``_where(_.field > X)``).

For the no-predicate form ``_distinct_by(_.K) |> count()`` /
``long_count()``, ``_sql`` continues to emit the simpler
``COUNT(DISTINCT "K")`` (no subquery wrap); see
:ref:`tutorial_sql_aggregates`.

For ``UNION`` / ``INTERSECT`` / ``EXCEPT`` see :ref:`tutorial_sql_set_ops`.

.. seealso::

    Full source: :download:`tutorials/sql/12-distinct.das <../../../../tutorials/sql/12-distinct.das>`

    Previous tutorial: :ref:`tutorial_sql_take_skip`

    Next tutorial: :ref:`tutorial_sql_set_ops`
