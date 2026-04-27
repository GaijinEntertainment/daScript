.. _tutorial_sql_set_ops:

==========================================
SQL-12b --- Set operations
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; UNION
    single: Tutorial; INTERSECT
    single: Tutorial; EXCEPT
    single: Tutorial; set operations

Set operations stack two SELECT queries with compatible projections
and dedupe / intersect / subtract their result rows. Under
``_sql(...)`` the existing ``daslib/linq.das`` array/iterator
functions ``union``, ``intersect``, and ``except`` are recognized
by the emitter and lowered to the matching SQL set-op:

==========================================  =============================================
Source shape                                Emitted SQL
==========================================  =============================================
``lhs |> union(rhs)``                       ``<lhs-sql> UNION <rhs-sql>``
``lhs |> intersect(rhs)``                   ``<lhs-sql> INTERSECT <rhs-sql>``
``lhs |> except(rhs)``                      ``<lhs-sql> EXCEPT <rhs-sql>``
==========================================  =============================================

Both sides must project the same column shape (same column count
and types). Use ``_select(_.Col)`` to pin the shape explicitly when
the sources have different schemas.

Distinct tags from either table
===============================

.. code-block:: das

    let all_tiers <- _sql((db |> select_from(type<Customer>) |> _select(_.Tier))
                          |> union(db |> select_from(type<Prospect>) |> _select(_.Tier)))
    // SELECT "Tier" FROM "Customers" UNION SELECT "Tier" FROM "Prospects"

Tags present in both tables
===========================

.. code-block:: das

    let shared <- _sql((db |> select_from(type<Customer>) |> _select(_.Tier))
                       |> intersect(db |> select_from(type<Prospect>) |> _select(_.Tier)))

Tags present only on the LHS
============================

.. code-block:: das

    let exclusive <- _sql((db |> select_from(type<Customer>) |> _select(_.Tier))
                          |> except(db |> select_from(type<Prospect>) |> _select(_.Tier)))

.. seealso::

    Full source: :download:`tutorials/sql/12b-set_ops.das <../../../../tutorials/sql/12b-set_ops.das>`

    Previous tutorial: :ref:`tutorial_sql_distinct`

    Next tutorial: :ref:`tutorial_sql_aggregates`
