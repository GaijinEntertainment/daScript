.. _tutorial_linq_decs:

==========================================
LINQ over DECS
==========================================

.. index::
    single: Tutorial; LINQ over DECS
    single: Tutorial; from_decs_template
    single: Tutorial; _fold over decs
    single: Tutorial; decs splice

This tutorial covers ``daslib/linq_fold``'s decs lane — how a
``[decs_template]`` schema becomes a linq source via
``from_decs_template(type<T>)``, and how the ``_fold`` macro splices
``where`` / ``select`` / ``order_by`` / ``group_by`` chains into a
single per-archetype walk without materializing intermediate iterators.
The runtime component-name variant (``from_decs(...)``) is out of
scope here — see the ``decs_boost`` module reference.

It assumes you have read :ref:`tutorial_linq` (the linq surface and the
``_fold`` macro) and :ref:`tutorial_decs` (entities, components, queries,
``[decs_template]``).

Setup
=====

.. code-block:: das

    options gen2
    options persistent_heap

    require daslib/decs_boost
    require daslib/linq
    require daslib/linq_boost
    require daslib/linq_fold

    [decs_template(prefix = "car_")]
    struct Car {
        id    : int
        price : int
        brand : int
    }

The ``prefix`` argument controls the component-name prefix decs uses
internally — every ``Car`` field becomes a ``car_*`` component
(``car_id``, ``car_price``, ``car_brand``). The prefix is what lets
``from_decs_template`` find the right components on every archetype that
carries the template.

``from_decs_template`` — entry point
=====================================

``from_decs_template(type<T>)`` opens the live decs state as a linq
source whose element type is a tuple matching ``T``'s field shape. The
macro walks every archetype that carries the template's components — so
adding more components to some entities later still leaves the older
archetypes matchable.

.. code-block:: das

    let total = _fold(from_decs_template(type<Car>).count())
    print("total cars: {total}\n")
    // output: total cars: 20

``_fold`` splices the chain
============================

``_fold`` rewrites a chain like
``from_decs_template(...)._where(...).count()`` into a single
``for_each_archetype`` walk. There is no intermediate array, no
iterator allocation — the splice runs the per-archetype walk and
accumulates inline.

.. code-block:: das

    let pricy = _fold(from_decs_template(type<Car>)._where(_.price > 50).count())
    print("cars with price > 50: {pricy}\n")
    // output: cars with price > 50: 9

``order_by`` + ``first`` — streaming-min
========================================

``_order_by(KEY).first()`` is recognized as a *streaming-min*: the
splice keeps a single ``var best`` + ``var seen`` per walk, no buffer
at all. The dual shape ``_order_by(KEY).take(N)`` keeps a bounded heap
of size N.

.. code-block:: das

    let cheapest = _fold(from_decs_template(type<Car>)._order_by(_.price).first())
    print("cheapest car: id={cheapest.id} price={cheapest.price}\n")
    // output: cheapest car: id=1 price=0

.. code-block:: das

    let top3 <- _fold(from_decs_template(type<Car>)._order_by(_.price).take(3).to_array())
    for (c in top3) {
        print("  id={c.id} price={c.price}\n")
    }
    // output:
    //   id=1 price=0
    //   id=20 price=3
    //   id=12 price=7

``group_by`` + ``select``
==========================

``_group_by(KEY)`` produces one bucket per distinct key. A follow-up
``_select`` reads ``_._0`` (the key) and ``_._1`` (the bucket) — typical
SQL "GROUP BY brand, take one example row" shape.

.. code-block:: das

    let bybrand <- _fold(from_decs_template(type<Car>)
                          ._group_by(_.brand)
                          ._select((Brand = _._0, FirstCar = _._1 |> first()))
                          .to_array())
    for (g in bybrand) {
        print("  brand {g.Brand}: first id={g.FirstCar.id} price={g.FirstCar.price}\n")
    }
    // output (order may vary across hash iterations):
    //   brand 0: first id=1 price=0
    //   brand 4: first id=5 price=48
    //   brand 3: first id=4 price=11
    //   brand 2: first id=3 price=74
    //   brand 1: first id=2 price=37

When to reach for the decs lane
================================

Use ``from_decs_template`` (the decs lane) when the data already lives
as decs entities — you avoid the array copy. Use ``each(arr)`` (the
array lane) when you start from an ``array<T>`` you already built. The
fold splice is shape-aware: it picks the per-element representation
that matches the source, so the per-element cost is similar across the
two lanes for most chains.

The cross-lane numbers (SQL / Array / Decs) over the full chain catalog
live in ``benchmarks/sql/results.md`` in the source tree.

.. seealso::

   :doc:`/reference/linq_fold_patterns` — catalog of chain shapes
   that ``_fold`` recognizes, and the splice arm each one fires.

   :ref:`tutorial_linq` — the underlying linq surface and the
   ``_fold`` macro.

   :ref:`tutorial_decs` — entities, components, queries,
   ``[decs_template]``.

   Full source: :download:`tutorials/language/55_linq_decs.das <../../../../tutorials/language/55_linq_decs.das>`

   Previous tutorial: :ref:`tutorial_glob`
