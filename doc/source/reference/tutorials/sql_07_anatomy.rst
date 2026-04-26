.. _tutorial_sql_anatomy:

================================
SQL-07 --- Anatomy of ``_sql``
================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; _sql
    single: Tutorial; _sql_text
    single: Tutorial; macro
    single: Tutorial; bind parameter

``_sql(chain)`` is a compile-time macro. It walks a daslib/linq-shaped
chain, classifies each operator, and emits a SQL string plus a list
of bind expressions. By the time the program runs, the chain is gone
--- only the SQL and the binds remain. There is no runtime
LINQ-to-SQL inspection.

Three moving parts
==================

==============================================  ================================================================
Source shape                                    Compile-time translation
==============================================  ================================================================
``_.Field``                                     quoted column identifier (``"Field"``)
free variable, literal                          ``?`` bind parameter
recognized operator / function call             SQL operator (``=``, ``<``, ``AND``, ``LIKE``, ``COALESCE``, ...)
==============================================  ================================================================

Anything outside that surface raises a compile-time ``macro_error``
pointing at the offending node.

Inspecting the SQL with ``_sql_text``
=====================================

``_sql_text`` shares the analyzer with ``_sql`` and returns the SQL
string instead of running it. The ``?`` placeholders show where each
bind goes:

.. code-block:: das

    let sql1 = _sql_text(db |> select_from(type<Car>)
                           |> _where(_.Price > 100))
    // SELECT "Id", "Name", "Price" FROM "Cars" WHERE "Price" > ?

This is the primary debugging tool for ``_sql`` chains --- when the
chain grows, ``_sql_text`` makes the macro's view of it visible
without a database round-trip.

Captured-vs-literal binding
===========================

A captured local becomes a bind. A literal *also* becomes a bind ---
the analyzer doesn't bother distinguishing, because keeping the SQL
parameterized either way is the safe-by-default behavior:

.. code-block:: das

    let cutoff = 150
    let sql2 = _sql_text(db |> select_from(type<Car>)
                           |> _where(_.Price > cutoff))
    // SELECT "Id", "Name", "Price" FROM "Cars" WHERE "Price" > ?

Column-side detection is symmetric --- the analyzer recognizes
``_.Field`` as a column on whichever side of an operator it appears:

.. code-block:: das

    _where(_.Price > cutoff)    // WHERE "Price" > ?
    _where(cutoff < _.Price)    // WHERE ? < "Price"

Composing ``_where``
====================

Each ``_where`` adds another ``AND`` conjunct:

.. code-block:: das

    _sql(db |> select_from(type<Car>)
           |> _where(_.Price > 100)
           |> _where(_.Name |> starts_with("T")))
    // ... WHERE ("Name" LIKE ? || '%') AND ("Price" > ?)

The full ``_where`` translation surface lives in
:ref:`tutorial_sql_where`.

The raw-SQL escape hatch
========================

For SQL the macro can't or shouldn't translate (DDL, vendor-specific
statements, dynamic SQL), drop down to the raw-SQL helpers covered in
:ref:`tutorial_sql_parametrized`:

==========================================================  =====================================================
Helper                                                      Purpose
==========================================================  =====================================================
``db |> exec(sql)``                                         run a statement, no result
``db |> query_scalar(sql, type<T>)``                        SELECT one column from one row
``db |> query_one(sql, type<T>, args...)``                  SELECT one row, build a struct
``db |> query_one_opt(sql, type<T>, args...)``              same, but ``Option<T>`` if no row
==========================================================  =====================================================

Bind parameters are positional ``?`` --- pass values as trailing
arguments.

The translation surface in one glance
=====================================

Tutorials 7-18 cover the full read-side translation. Each one shows
its operator's recognized shapes and the SQL the macro emits:

================================================  =========================================================
Tutorial                                          Surface
================================================  =========================================================
:ref:`tutorial_sql_where`                         predicates: ``==``, ``<``, ``&&``, ``starts_with``, ...
:ref:`tutorial_sql_select`                        projections: ``_.Field``, named tuples
:ref:`tutorial_sql_order_by`                      ``_order_by``, ``_order_by_descending``
:ref:`tutorial_sql_take_skip`                     ``take`` / ``skip`` (LIMIT / OFFSET)
:ref:`tutorial_sql_distinct`                      ``distinct``
:ref:`tutorial_sql_aggregates`                    ``count`` / ``sum`` / ``average`` / ``min`` / ``max``
:ref:`tutorial_sql_group_by`                      ``_group_by`` + ``_having``
:ref:`tutorial_sql_null_handling`                 ``Option<T>`` columns + ``is_some`` / ``is_none`` / ``unwrap_or``
================================================  =========================================================

What ``_sql`` refuses
=====================

If you write a shape outside the translation table --- an arbitrary
user-defined function in ``_where``, an unsupported math op, a regex
--- compile fails with ``macro_error`` pointing at the offending
node. Two responses:

- Add the rule to the analyzer (fork-and-extend) if the pattern is
  reusable across queries.
- Use the raw-SQL escape hatch above for one-offs.

.. seealso::

    Full source: :download:`tutorials/sql/07-anatomy.das <../../../../tutorials/sql/07-anatomy.das>`

    Previous tutorial: :ref:`tutorial_sql_error_handling`

    Next tutorial: :ref:`tutorial_sql_where`
