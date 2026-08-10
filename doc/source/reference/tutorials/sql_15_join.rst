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

.. das-doc: fragment

.. code-block:: das

    $(l : TA, r : TB) => l.X == r.Y

--- a 2-arg lambda whose body is one ``==`` comparison between a
left-side field and a right-side field. Theta joins (``<``, ``>``,
``&&``-chained multi-key equality, function calls inside) emit a
compile-time ``error[50503]`` pointing at raw SQL as the escape
hatch. The single-key restriction matches the backing ``join`` fn's
hash-based O(n+m) implementation; for composite-key joins, use raw
SQL.

Multi-source mode and table aliases
====================================

In multi-source mode the FROM clause aliases the root table as
``t0``, the joined table as ``t1``, and every column ref in the
WHERE, projection, and ON clauses qualifies with the matching alias.
Single-source chains keep the unqualified shape --- only multi-source
queries pay the alias-noise tax.

.. das-doc: given [sql_table(name="Users")] struct User { @sql_primary_key Id : int; Name : string; Active : bool }
.. das-doc: given [sql_table(name="Orders")] struct Order { @sql_primary_key Id : int; UserId : int; Total : int }
.. das-doc: given [sql_table(name="Cars")] struct Car { @sql_primary_key id : int; brand : string; price : int; dealer_id : int }
.. das-doc: given [sql_table(name="Dealers")] struct Dealer { @sql_primary_key id : int; name : string }
.. das-doc: given var inscope db = open_sqlite(":memory:")

.. code-block:: das

    let rows <- _sql(db |> select_from(type<User>)
                       |> _join(db |> select_from(type<Order>),
                                $(u : User, o : Order) => u.Id == o.UserId,
                                $(u : User, o : Order) => (UserName = u.Name, Total = o.Total)))
    // SELECT "t0"."Name" AS "UserName", "t1"."Total"
    //   FROM "Users" AS "t0"
    //   INNER JOIN "Orders" AS "t1"
    //     ON "t0"."Id" = "t1"."UserId"

Filtering before / inside / after the join
==========================================

Pre-join ``_where`` filters the LEFT source. Column refs qualify
with the left alias automatically:

.. das-doc: fragment

.. code-block:: das

    db |> select_from(type<User>)
       |> _where(_.Active)
       |> _join(...)
    // ... WHERE ("t0"."Active" <> 0)

``_where`` can also live inside the right-side chain --- filters
there qualify with the right alias ``t1`` and emit into the JOIN's
``ON`` clause (not the outer ``WHERE``):

.. das-doc: fragment

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

Returning one source row
========================

When the result should be one of the joined table rows, return that
source argument directly from ``into``. ``_sql`` expands the source
to its qualified columns and reconstructs the exact nominal struct:

.. code-block:: das

    let large_orders : array<Order> <- _sql(
        db |> select_from(type<User>)
           |> _join(db |> select_from(type<Order>),
                    $(u : User, o : Order) => u.Id == o.UserId,
                    $(u : User, o : Order) => o)
           |> _where(_.Total >= 100)
           |> _order_by(_.Total))
    // SELECT "t1"."Id", "t1"."UserId", "t1"."Total"
    //   FROM "Users" AS "t0"
    //   INNER JOIN "Orders" AS "t1"
    //     ON "t0"."Id" = "t1"."UserId"
    //  WHERE "t1"."Total" >= ?
    //  ORDER BY "t1"."Total" ASC

The result type is ``array<Order>``, not a generated tuple. This form
is useful when the other source only supplies filtering, ranking, or
existence context. A nullable side of an outer join cannot be returned
as a bare struct; project named scalar fields (including an
``is_some`` probe) when unmatched rows must be represented.

Joining + grouping
==================

``_group_by`` / ``_having`` / ``_order_by`` after ``_join`` accept the
join's ``into``-projection alias names as keys, not base-table fields.
The translator resolves each alias through the join's projection
registry and emits the underlying qualified column (``"t0"."brand"``,
``"t1"."Id"``) in the GROUP BY / HAVING / ORDER BY clauses, so the
SQL composes cleanly with the JOIN:

.. code-block:: das

    let groups <- _sql(db |> select_from(type<Car>)
        |> _join(db |> select_from(type<Dealer>),
                 $(c : Car, d : Dealer) => c.dealer_id == d.id,
                 $(c : Car, d : Dealer) => (Brand = c.brand, Price = c.price))
        |> _group_by(_.Brand)
        |> _select((Brand = _._0, N = _._1 |> count())))
    // SELECT ("t0"."brand") AS "Brand", COUNT(*) AS "N"
    //   FROM "Cars" AS "t0" INNER JOIN "Dealers" AS "t1"
    //     ON "t0"."dealer_id" = "t1"."id"
    //   GROUP BY ("t0"."brand")

Per-group aggregates over a projection alias resolve the same way ---
``_._1 |> _select(_.Price) |> sum()`` becomes ``SUM("t0"."price")``,
not the bare alias:

.. code-block:: das

    let totals <- _sql(db |> select_from(type<Car>)
        |> _join(db |> select_from(type<Dealer>),
                 $(c : Car, d : Dealer) => c.dealer_id == d.id,
                 $(c : Car, d : Dealer) => (Brand = c.brand, Price = c.price))
        |> _group_by(_.Brand)
        |> _select((Brand = _._0,
                    Total = _._1 |> _select(_.Price) |> sum())))
    // ... SUM("t0"."price") ... GROUP BY ("t0"."brand")

``_order_by`` and computed-expression group keys (``_group_by(_.Price / 100)``)
read aliases through the same registry. Aliases that don't appear in
the join's ``into`` projection reject with a clear macro_error listing
the valid alias names.

Other join shapes
=================

``_left_join`` / ``_right_join`` / ``_full_outer_join`` /
``_cross_join`` all ship --- see :ref:`tutorial_sql_left_join`, which
covers every outer-join shape and the WHERE-on-preserved-side rules.

Three-table joins chain cleanly: each ``_join``'s named ``into``
projection is referenced by alias in the next join's lambdas (e.g.
``uo.OrderId``). The one constraint is that each join's RIGHT side
cannot itself be a join --- chain from the left.

What's not shipped
==================

* Multi-key equi-joins (``u.Id == o.UserId && u.Tenant == o.Tenant``)
  --- the backing ``join`` fn's keys are single-column, so a
  ``&&``-chained ``ON`` predicate errors with ``error[50503]``; use
  raw SQL for composite-key joins.

.. seealso::

    Full source: :download:`tutorials/sql/15-join.das <../../../../tutorials/sql/15-join.das>`

    Previous tutorial: :ref:`tutorial_sql_group_by`

    Next tutorial: :ref:`tutorial_sql_left_join`
