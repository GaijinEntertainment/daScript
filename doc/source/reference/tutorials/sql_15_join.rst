.. _tutorial_sql_join:

==========================================
SQL-15 --- ``_join`` (inner equi-join)
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; _join
    single: Tutorial; INNER JOIN
    single: Tutorial; equi-join

``_join(other, on, into)`` is sugar over the existing
``daslib/linq.das::join`` function --- under ``_sql(...)`` it emits
SQL's ``INNER JOIN ... ON ...``.

Equi-join shape
===============

The ``on`` predicate is locked to the equi-join shape

.. code-block:: das

    $(l : TA, r : TB) => l.X == r.Y

--- a 2-arg lambda whose body is one ``==`` comparison between a
left-side field and a right-side field. Theta joins (``<``, ``>``,
``&&``-chained multi-key equality, function calls inside) emit a
compile-time ``macro_error`` pointing at raw SQL as the escape
hatch. The single-key restriction matches the backing ``join`` fn's
hash-based O(n+m) implementation; multi-key tuple joins land later.

Multi-source mode and table aliases
====================================

In multi-source mode the FROM clause aliases the root table as
``t0``, the joined table as ``t1``, and every column ref in the
WHERE, projection, and ON clauses qualifies with the matching alias.
Single-source chains keep the unqualified shape --- only multi-source
queries pay the alias-noise tax.

.. code-block:: das

    let rows <- _sql(db |> select_from(type<User>)
                       |> _join(db |> select_from(type<Order>),
                                $(u : User, o : Order) => u.Id == o.UserId,
                                $(u : User, o : Order) => (UserName = u.Name, Total = o.Total)))
    // SELECT "t0"."Name", "t1"."Total"
    //   FROM "Users" AS "t0"
    //   INNER JOIN "Orders" AS "t1"
    //     ON "t0"."Id" = "t1"."UserId"

Filtering before / inside / after the join
==========================================

Pre-join ``_where`` filters the LEFT source. Column refs qualify
with the left alias automatically:

.. code-block:: das

    db |> select_from(type<User>)
       |> _where(_.Active)
       |> _join(...)
    // ... WHERE "t0"."Active"

``_where`` can also live inside the right-side chain --- filters
there qualify with the right alias ``t1`` and emit into the JOIN's
``ON`` clause (not the outer ``WHERE``):

.. code-block:: das

    _join(db |> select_from(type<Order>) |> _where(_.Total > 75), ...)
    // ... INNER JOIN "Orders" AS "t1" ON ... AND ("t1"."Total" > ?)

This placement is required for ``_left_join`` --- a row-level filter
in the outer ``WHERE`` would drop ``NULL``-extended unmatched rows
and silently turn ``LEFT JOIN`` into ``INNER JOIN``. ``_join`` (INNER)
uses the same ``ON`` placement for consistency and planner stability.

The ``into`` projection
=======================

The ``into`` lambda has two args bound to the two sources. Naming
is up to you (``u``, ``o`` here). Refer to columns from each source
through the arg name (``u.Name``, ``o.Total``); the ``_sql``
translator rewrites them to alias-qualified column refs.

Single-column and named-tuple projections both work. Per-source
projections (``_select(...)`` on the right side) are rejected ---
the join's ``into`` lambda is the only projection.

What's not shipped
==================

* ``_right_join`` --- trivially ``_left_join`` with swapped sources
* ``_full_outer_join`` --- rare; raw SQL escape hatch
* ``_cross_join`` --- footgun; raw SQL escape hatch
* Three-table joins --- compose syntactically but produce verbose
  tuple chains; documented as "use 2-table joins idiomatically;
  3+ either chain with verbose tuple access, or drop to raw SQL"
* Multi-key equi-joins (``u.Id == o.UserId && u.Tenant == o.Tenant``)
  --- legitimate, but the backing ``join`` fn's keys are
  single-column; deferred to a later ``multi_key_join`` extension

.. seealso::

    Full source: :download:`tutorials/sql/15-join.das <../../../../tutorials/sql/15-join.das>`

    Previous tutorial: :ref:`tutorial_sql_group_by`

    Next tutorial: :ref:`tutorial_sql_left_join`
