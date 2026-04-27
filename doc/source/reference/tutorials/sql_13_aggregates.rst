.. _tutorial_sql_aggregates:

==========================================
SQL-13 --- Aggregates: ``sum``/``avg``/...
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; aggregate
    single: Tutorial; SUM
    single: Tutorial; AVG
    single: Tutorial; MIN
    single: Tutorial; MAX
    single: Tutorial; COUNT

==============================================  ============================  ========================
Source shape                                    Emitted SQL                   Result type
==============================================  ============================  ========================
``... |> count()``                              ``SELECT COUNT(*) ...``       ``int``
``... |> _select(_.Col) |> sum()``              ``SELECT SUM("Col") ...``     column type
``... |> _select(_.Col) |> average()``          ``SELECT AVG("Col") ...``     ``double``
``... |> _select(_.Col) |> min()``              ``SELECT MIN("Col") ...``     column type
``... |> _select(_.Col) |> max()``              ``SELECT MAX("Col") ...``     column type
==============================================  ============================  ========================

All five are *terminals* --- they end the chain and produce a
scalar, not an array. The four column-driven aggregates need a
``_select(_.Col)`` to pick the column; ``count`` operates on rows,
not values, and stands alone.

``count`` --- whole-source row count
====================================

.. code-block:: das

    let n = _sql(db |> select_from(type<Car>) |> count())
    // SELECT COUNT(*) FROM "Cars"

``count`` returns ``int`` --- matching daslang's ``length(arr)``
return type --- not ``int64``. SQLite's underlying ``COUNT`` returns
``int64``; the column-reader narrows to ``int``.

``sum`` / ``min`` / ``max`` --- inherit column type
===================================================

.. code-block:: das

    let total    = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> sum())
    let cheapest = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> min())
    let priciest = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> max())

The result element type matches the projected column. ``Price`` is
``int``, so ``total`` is ``int``; ``Price`` of type ``int64`` would
produce ``int64``.

``average`` --- always promotes to ``double``
=============================================

.. code-block:: das

    let mean = _sql(db |> select_from(type<Car>) |> _select(_.Price) |> average())
    // SELECT AVG("Price") FROM "Cars"     -- result type: double

Composing with ``_where``
=========================

A ``_where`` predicate filters the input rows the aggregate sees:

.. code-block:: das

    let cutoff = 100
    let total_over_100 = _sql(db |> select_from(type<Car>)
                                |> _where(_.Price > cutoff)
                                |> _select(_.Price)
                                |> sum())
    // SELECT SUM("Price") FROM "Cars" WHERE "Price" > ?

Per-bucket aggregates --- ``_group_by``
=======================================

Aggregates inside ``_group_by`` are covered in
:ref:`tutorial_sql_group_by`. There the same primitives
(``length`` / ``sum`` / ``average`` / ``min`` / ``max``) appear
inside an inner ``select`` lambda over the group elements.

.. seealso::

    Full source: :download:`tutorials/sql/13-aggregates.das <../../../../tutorials/sql/13-aggregates.das>`

    Previous tutorial: :ref:`tutorial_sql_distinct`

    Next tutorial: :ref:`tutorial_sql_group_by`
