.. _tutorial_sql_order_by:

=======================================================
SQL-10 --- ``_order_by`` and ``_order_by_descending``
=======================================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; _order_by
    single: Tutorial; ORDER BY
    single: Tutorial; tuple key

``_order_by(key)`` adds an ``ORDER BY`` clause. The key shape decides
what the macro emits:

==============================================  ===========================================
Key shape                                       Emitted SQL
==============================================  ===========================================
``_.Field``                                     ``ORDER BY "Field" ASC``
``(_.k1, _.k2, ...)``                           ``ORDER BY "k1" ASC, "k2" ASC, ...``
``_order_by_descending(_.Field)``               ``ORDER BY "Field" DESC``
==============================================  ===========================================

Single-column order
===================

.. code-block:: das

    let by_price <- _sql(db |> select_from(type<Car>)
                           |> _order_by(_.Price))
    // SELECT "Id", "Name", "Price" FROM "Cars" ORDER BY "Price" ASC

``_order_by_descending`` flips the direction:

.. code-block:: das

    let by_price_desc <- _sql(db |> select_from(type<Car>)
                                |> _order_by_descending(_.Price))
    // ... ORDER BY "Price" DESC

Multi-column tuple key
======================

A tuple-shaped key emits each tuple field as its own ``ORDER BY``
column. Useful for primary-and-tie-breaker ordering:

.. code-block:: das

    let by_price_then_name <- _sql(db |> select_from(type<Car>)
                                     |> _order_by((_.Price, _.Name)))
    // ... ORDER BY "Price" ASC, "Name" ASC

If two rows tie on ``Price``, the second column breaks the tie.

Mixed ASC/DESC
==============

A single ``_order_by`` step emits all columns in the same direction.
Mixed ASC/DESC across columns (e.g. ``ORDER BY a ASC, b DESC``) is
**out of chunk-4 scope**. Until a marker syntax lands, drop down to
the raw-SQL escape hatch (``query_one`` / ``query_scalar`` /
``exec``) when you need it.

Composes with ``_where`` and ``take``
=====================================

``_order_by`` is just another chain stage --- it composes with
filter, project, paginate, and aggregate operators in the natural
SQL order:

.. code-block:: das

    let cheap_top3 <- _sql(db |> select_from(type<Car>)
                             |> _where(_.Price > 50)
                             |> _order_by(_.Price)
                             |> take(3))
    // ... WHERE "Price" > ? ORDER BY "Price" ASC LIMIT ?

Note the SQL clause order: ``WHERE`` first, ``ORDER BY`` next,
``LIMIT`` last --- same as the chain order.

Stable pagination
=================

``LIMIT``/``OFFSET`` without ``ORDER BY`` is non-deterministic ---
SQLite is free to return rows in any order. Pair ``take``/``skip``
with ``_order_by`` whenever you actually care about page boundaries.
See :ref:`tutorial_sql_take_skip`.

.. seealso::

    Full source: :download:`tutorials/sql/10-order_by.das <../../../../tutorials/sql/10-order_by.das>`

    Previous tutorial: :ref:`tutorial_sql_select`

    Next tutorial: :ref:`tutorial_sql_take_skip`
