.. _tutorial_sql_distinct:

==================================
SQL-12 --- ``distinct``
==================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; distinct
    single: Tutorial; DISTINCT
    single: Tutorial; UNION

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

Set operations --- deferred
===========================

``UNION`` / ``INTERSECT`` / ``EXCEPT`` are **out of chunk-4 scope**.
The chain analyzer is single-source today: every ``SELECT`` comes
from one ``select_from(type<T>)``. Set operations bring in a second
``SqlQuery`` and need a multi-stage emitter --- the same engine work
joins and subqueries need.

Until that lands, work around set ops via the raw-SQL escape hatch
(``db |> exec(...)`` for DDL plus ``CREATE TEMP VIEW``, then
``select_from`` the view; or drive the SQLite C API directly for
multi-row reads of arbitrary SQL).

.. seealso::

    Full source: :download:`tutorials/sql/12-distinct.das <../../../../tutorials/sql/12-distinct.das>`

    Previous tutorial: :ref:`tutorial_sql_take_skip`

    Next tutorial: :ref:`tutorial_sql_set_ops`
