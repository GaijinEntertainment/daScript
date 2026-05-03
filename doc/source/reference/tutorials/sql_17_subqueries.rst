.. _tutorial_sql_subqueries:

==========================================
SQL-17 --- Subqueries
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; _in
    single: Tutorial; _not_in
    single: Tutorial; _any
    single: Tutorial; _none
    single: Tutorial; subquery
    single: Tutorial; IN
    single: Tutorial; EXISTS

Four ``daslib/linq_boost`` macros lower to SQL's IN / NOT IN /
EXISTS / NOT EXISTS forms when used inside a ``_where`` predicate
under ``_sql(...)``:

==========================================  =============================================
Source shape                                Emitted SQL
==========================================  =============================================
``x._in(subquery)``                         ``<x> IN (<subquery-sql>)``
``x._not_in(subquery)``                     ``<x> NOT IN (<subquery-sql>)``
``subquery._any()``                         ``EXISTS (<subquery-sql>)``
``subquery._any(predicate)``                ``EXISTS (... WHERE <predicate>)``
``subquery._none()``                        ``NOT EXISTS (<subquery-sql>)``
``subquery._none(predicate)``               ``NOT EXISTS (... WHERE <predicate>)``
==========================================  =============================================

Why explicit positive/negative names
====================================

Negated forms have **explicit names** (``_not_in``, ``_none``)
rather than ``!_in(...)`` / ``!_any(...)``. The ``_sql`` AST walker
pattern-matches on the macro's textual name; a leading ``!`` would
require deep AST walking that fights with daslang's expansion order.
Expect this convention to repeat for any future predicate primitive
with a negated form.

IN / NOT IN with a single-column subquery
==========================================

For IN-style subqueries, project a single column with
``_select(_.Col)`` so the IN list shape matches.

.. code-block:: das

    let with_orders <- _sql(db |> select_from(type<User>)
                              |> _where(_.Id._in(db |> select_from(type<Order>) |> _select(_.UserId))))
    // SELECT "Id", "Name", "Active" FROM "Users"
    //   WHERE "Id" IN (SELECT "UserId" FROM "Orders")

    let no_orders <- _sql(db |> select_from(type<User>)
                            |> _where(_.Id._not_in(db |> select_from(type<Order>) |> _select(_.UserId))))
    // ... WHERE "Id" NOT IN (...)

EXISTS / NOT EXISTS
===================

.. code-block:: das

    let some_big <- _sql(db |> select_from(type<User>)
                           |> _where((db |> select_from(type<Order>))._any(_.Total > 200)))
    // ... WHERE EXISTS (SELECT ... FROM "Orders" WHERE "Total" > ?)

    let no_huge <- _sql(db |> select_from(type<User>)
                          |> _where((db |> select_from(type<Order>))._none(_.Total > 1000)))
    // ... WHERE NOT EXISTS (SELECT ... FROM "Orders" WHERE "Total" > ?)

Uncorrelated only
=================

Only **uncorrelated** subqueries are supported --- the inner WHERE
references only the subquery's own row (``_``), not outer columns.
Correlated subqueries (referring to outer columns from inside the
subquery's WHERE) are not yet supported.

.. seealso::

    Full source: :download:`tutorials/sql/17-subqueries.das <../../../../tutorials/sql/17-subqueries.das>`

    Previous tutorial: :ref:`tutorial_sql_left_join`

    Next tutorial: :ref:`tutorial_sql_null_handling`
