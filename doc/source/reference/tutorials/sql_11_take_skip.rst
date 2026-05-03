.. _tutorial_sql_take_skip:

==========================================
SQL-11 --- ``take`` and ``skip``: Paging
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; take
    single: Tutorial; skip
    single: Tutorial; LIMIT
    single: Tutorial; OFFSET
    single: Tutorial; pagination

==============================================  =================================================
Source shape                                    Emitted SQL
==============================================  =================================================
``take(n)``                                     ``LIMIT ?``
``skip(n)`` (alone)                             ``LIMIT -1 OFFSET ?``
``skip(m) |> take(n)``                          ``LIMIT ? OFFSET ?`` (canonical pagination)
``take(n) |> skip(m)``                          nested SELECT (preserves linq positional semantics)
``take`` followed by ``_first()``               ``LIMIT 1`` (single-row terminal wins)
==============================================  =================================================

``take(n)`` --- LIMIT
=====================

.. code-block:: das

    let first_two <- _sql(db |> select_from(type<Car>) |> take(2))
    // SELECT "Id", "Name", "Price" FROM "Cars" LIMIT ?

``skip(n)`` --- OFFSET
======================

SQLite requires a ``LIMIT`` whenever ``OFFSET`` is present, so a
bare ``skip(n)`` emits ``LIMIT -1 OFFSET ?`` (``LIMIT -1`` is
SQLite's "no limit"):

.. code-block:: das

    let after_three <- _sql(db |> select_from(type<Car>) |> skip(3))
    // SELECT "Id", "Name", "Price" FROM "Cars" LIMIT -1 OFFSET ?

Combined --- a paging window
============================

Chain order matters --- ``_sql`` honors linq's positional
semantics. The canonical pagination shape is
``skip(m) |> take(n)`` (skip first, then take), which lowers to a
flat ``LIMIT n OFFSET m``. Standard paging is ``page k of size n``
= ``skip((k-1) * n) |> take(n)``:

.. code-block:: das

    let window <- _sql(db |> select_from(type<Car>) |> skip(2) |> take(2))
    // ... LIMIT ? OFFSET ?     (rows 3..4 = page 2 of size 2)

The reverse, ``take(n) |> skip(m)``, means "take first n rows,
then drop m of those" --- a strictly smaller result of
``max(0, n - m)`` rows. ``_sql`` preserves that meaning by lowering
to a nested SELECT:

.. code-block:: das

    let trimmed <- _sql(db |> select_from(type<Car>) |> take(2) |> skip(1))
    // SELECT ... FROM (SELECT ... LIMIT ?) AS "t0" LIMIT -1 OFFSET ?
    // result: 1 row (take 2, then drop 1)

Same chain, same answer in all three modes (``_sql`` over SQL
source, plain chain over SQL source, plain chain over
``array<T>``). Use the canonical ``skip|take`` form when you want
the flat fast-path SQL; the reverse is honest about its meaning
but pays the subquery cost.

Bind ordering
=============

LIMIT and OFFSET binds always go *after* WHERE binds in the
placeholder index sequence. A ``_where(...) |> take(n)`` chain
produces ``WHERE col > ? LIMIT ?`` with binds in
``[wherevalue, n]`` order:

.. code-block:: das

    let threshold = 100
    let cheap_two <- _sql(db |> select_from(type<Car>)
                            |> _where(_.Price > threshold)
                            |> take(2))
    // SELECT ... WHERE "Price" > ? LIMIT ?     binds: [100, 2]

Single-row terminals override ``take``
======================================

``_first`` and ``_first_opt`` always emit ``LIMIT 1``, ignoring any
earlier ``take``. The terminal's intent (one row) overrides the
chain's pagination hint:

.. code-block:: das

    let head = _sql(db |> select_from(type<Car>) |> take(5) |> _first())
    // ... LIMIT 1

Stable pagination needs ``_order_by``
=====================================

``LIMIT``/``OFFSET`` without ``ORDER BY`` isn't deterministic ---
SQLite can return rows in any order. Pair ``take``/``skip`` with
:ref:`tutorial_sql_order_by` whenever the page boundaries matter:

.. code-block:: das

    let stable_page <- _sql(db |> select_from(type<Car>)
                              |> _order_by(_.Price)
                              |> skip(1) |> take(2))
    // ... ORDER BY "Price" ASC LIMIT ? OFFSET ?

.. seealso::

    Full source: :download:`tutorials/sql/11-take_skip.das <../../../../tutorials/sql/11-take_skip.das>`

    Previous tutorial: :ref:`tutorial_sql_order_by`

    Next tutorial: :ref:`tutorial_sql_distinct`
