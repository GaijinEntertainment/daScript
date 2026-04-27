.. _tutorial_sql_group_by:

==========================================
SQL-14 --- ``_group_by`` and ``_having``
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; _group_by
    single: Tutorial; _having
    single: Tutorial; GROUP BY
    single: Tutorial; HAVING
    single: Tutorial; IGrouping
    single: Tutorial; aggregate

``_group_by(key)`` partitions rows into buckets. After grouping, the
chain element is a (key, group-rows) tuple shaped like C# LINQ's
``IGrouping<K, T>``:

==========================================  =============================================
Group-tuple shape                           Meaning
==========================================  =============================================
``_._0``                                    group key (single-key form)
``_._0._N``                                 N-th key column (multi-key form)
``_._1``                                    array of rows in this bucket
==========================================  =============================================

Aggregating over the group
==========================

The macro recognizes a fixed set of pipe shapes on ``_._1``:

==================================================================  ==============================
Source shape                                                        Emitted SQL
==================================================================  ==============================
``_._1 |> length``                                                  ``COUNT(*)``
``_._1 |> count``                                                   ``COUNT(*)`` (alias)
``_._1 |> select($(u : T) => u.Field) |> sum``                      ``SUM("Field")``
``_._1 |> select($(u : T) => u.Field) |> average``                  ``AVG("Field")``
``_._1 |> select($(u : T) => u.Field) |> min``                      ``MIN("Field")``
``_._1 |> select($(u : T) => u.Field) |> max``                      ``MAX("Field")``
==================================================================  ==============================

The inner ``select`` lambda parameter is intentionally **named**
(``$(u : User)``), not ``_``. The outer ``_select`` lambda already
binds ``_`` to the group tuple, and daslang flags a same-name-
different-type lambda nested inside as a shadowing error. Naming the
inner parameter sidesteps the conflict; the macro recognizes the
body as ``<param>.<field>`` for any parameter name.

Single-key grouping
===================

.. code-block:: das

    let by_city <- _sql(db |> select_from(type<User>)
                          |> _group_by(_.City)
                          |> _order_by(_._0)
                          |> _select((City = _._0, N = _._1 |> length)))
    // SELECT "City", COUNT(*) FROM "Users" GROUP BY "City"

Multiple aggregates in one projection
=====================================

.. code-block:: das

    let stats <- _sql(db |> select_from(type<User>)
                        |> _group_by(_.City)
                        |> _select((
                            City   = _._0,
                            N      = _._1 |> length,
                            MinAge = _._1 |> select($(u : User) => u.Age)    |> min,
                            MaxAge = _._1 |> select($(u : User) => u.Age)    |> max,
                            Total  = _._1 |> select($(u : User) => u.Salary) |> sum)))

Pre-aggregate filter --- ``_where``
===================================

``_where`` before ``_group_by`` filters input rows --- it lowers to
``WHERE`` (pre-aggregate). The macro routes binds correctly: WHERE
binds first, then HAVING, then LIMIT.

.. code-block:: das

    _sql(db |> select_from(type<User>)
           |> _where(_.Age >= 30)
           |> _group_by(_.City)
           |> _select((City = _._0, N = _._1 |> length)))
    // ... WHERE "Age" >= ? GROUP BY "City"

Post-aggregate filter --- ``_having``
=====================================

``_having(predicate)`` filters on the aggregated buckets. It sits
between ``_group_by`` and the projection. Aggregate expressions
inside ``_having`` translate the same way as inside ``_select``:

.. code-block:: das

    _sql(db |> select_from(type<User>)
           |> _group_by(_.City)
           |> _having(_._1 |> length > 1)
           |> _select((City = _._0, N = _._1 |> length)))
    // SELECT "City", COUNT(*) FROM "Users"
    // GROUP BY "City" HAVING COUNT(*) > ?

Multi-key grouping
==================

Pass a tuple to ``_group_by``. Each tuple field becomes its own
``GROUP BY`` column. Project the keys with ``_._0._N``:

.. code-block:: das

    _sql(db |> select_from(type<User>)
           |> _group_by((_.City, _.Age))
           |> _select((City = _._0._0, Age = _._0._1, N = _._1 |> length)))
    // SELECT "City", "Age", COUNT(*) FROM "Users"
    // GROUP BY "City", "Age"

The full reporting query
========================

The chain mirrors SQL clause order one-for-one:

.. code-block:: das

    let report <- _sql(db |> select_from(type<User>)
                         |> _where(_.Age >= min_age)
                         |> _group_by(_.City)
                         |> _having(_._1 |> length >= min_count)
                         |> _order_by(_._0)
                         |> _select((
                             City      = _._0,
                             N         = _._1 |> length,
                             AvgSalary = _._1 |> select($(u : User) => u.Salary) |> average))
                         |> take(10))

    // SELECT "City", COUNT(*), AVG("Salary") FROM "Users"
    // WHERE "Age" >= ? GROUP BY "City" HAVING COUNT(*) >= ?
    // ORDER BY "City" ASC LIMIT ?

.. seealso::

    Full source: :download:`tutorials/sql/14-group_by.das <../../../../tutorials/sql/14-group_by.das>`

    Previous tutorial: :ref:`tutorial_sql_aggregates`

    Next tutorial: :ref:`tutorial_sql_null_handling`
