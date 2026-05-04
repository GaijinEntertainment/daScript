.. _tutorial_sql_left_join:

==============================================
SQL-16 --- All Join Shapes
==============================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; _join
    single: Tutorial; _left_join
    single: Tutorial; _right_join
    single: Tutorial; _full_outer_join
    single: Tutorial; _cross_join
    single: Tutorial; LEFT JOIN
    single: Tutorial; RIGHT JOIN
    single: Tutorial; FULL OUTER JOIN
    single: Tutorial; CROSS JOIN
    single: Tutorial; outer join
    single: Tutorial; Option<T>

Five join shapes, one surface
=============================

All four equi-joins share the same 4-arg shape ``srca |> _<kind>_join(srcb, on, into)``.
``_cross_join`` is the exception --- no ``on`` clause, 3-arg form.

The ``into`` projection lambda's argument types reveal which side may
be missing per join kind:

============================  ===========================================  ==================================
Operator                      ``into`` lambda signature                    Cardinality vs. inputs
============================  ===========================================  ==================================
``_join``                     ``(a : TA, b : TB)``                         Matched pairs only
``_left_join``                ``(a : TA, b : Option<TB>)``                 Every TA, plus matched right
``_right_join``               ``(a : Option<TA>, b : TB)``                 Every TB, plus matched left
``_full_outer_join``          ``(a : Option<TA>, b : Option<TB>)``         Every TA + every TB
``_cross_join``               ``(a : TA, b : TB)``                         Cartesian (TA × TB)
============================  ===========================================  ==================================

Probing the Option arg
======================

``is_some`` / ``is_none`` on a join arg lowers to a SQL NULL probe on
that side's join key. The probe column is the matching ON-key on the
side being checked --- if THAT came back NULL, the row had no match.

==========================================  ===========================  ==========================================
Probe in projection                         Allowed on join kind         Emitted SQL fragment
==========================================  ===========================  ==========================================
``o |> is_some`` (RHS arg)                  Left, Full Outer             ``"t1"."<rightKey>" IS NOT NULL``
``o |> is_none``                            Left, Full Outer             ``"t1"."<rightKey>" IS NULL``
``u |> is_some`` (LHS arg)                  Right, Full Outer            ``"t0"."<leftKey>" IS NOT NULL``
``u |> is_none``                            Right, Full Outer            ``"t0"."<leftKey>" IS NULL``
==========================================  ===========================  ==========================================

LEFT JOIN
=========

.. code-block:: das

    let rows <- _sql(db |> select_from(type<User>)
                       |> _left_join(db |> select_from(type<Order>),
                                     $(u : User, o : Order) => u.Id == o.UserId,
                                     $(u : User, o : Option<Order>) => (Name = u.Name, HasOrder = o |> is_some)))
    // SELECT "t0"."Name", "t1"."UserId" IS NOT NULL
    //   FROM "Users" AS "t0"
    //   LEFT JOIN "Orders" AS "t1"
    //     ON "t0"."Id" = "t1"."UserId"

RIGHT JOIN
==========

Mirror of LEFT --- every right row surfaces, left side is ``Option<TA>``.

.. code-block:: das

    let rows <- _sql(db |> select_from(type<User>)
                       |> _right_join(db |> select_from(type<Order>),
                                      $(u : User, o : Order) => u.Id == o.UserId,
                                      $(u : Option<User>, o : Order) => (HasUser = u |> is_some, OrderId = o.Id)))
    // SELECT "t0"."Id" IS NOT NULL, "t1"."Id"
    //   FROM "Users" AS "t0"
    //   RIGHT JOIN "Orders" AS "t1"
    //     ON "t0"."Id" = "t1"."UserId"

Bundled SQLite is 3.41.2; ``RIGHT JOIN`` is native since 3.39.

FULL OUTER JOIN
===============

Both sides are ``Option<T>``. Probe either arg; the analyzer routes the
``IS [NOT] NULL`` to the correct side's join key.

.. code-block:: das

    let rows <- _sql(db |> select_from(type<User>)
                       |> _full_outer_join(db |> select_from(type<Order>),
                                           $(u : User, o : Order) => u.Id == o.UserId,
                                           $(u : Option<User>, o : Option<Order>) => (HasUser = u |> is_some, HasOrder = o |> is_some)))
    // SELECT "t0"."Id" IS NOT NULL, "t1"."UserId" IS NOT NULL
    //   FROM "Users" AS "t0"
    //   FULL OUTER JOIN "Orders" AS "t1"
    //     ON "t0"."Id" = "t1"."UserId"

CROSS JOIN
==========

Cartesian product. No ``on`` clause; both args are non-Option (every
pair surfaces). Filter post-cross with ``_where`` to recover an
inner-join-shaped result.

.. code-block:: das

    let rows <- _sql(db |> select_from(type<User>)
                       |> _cross_join(db |> select_from(type<Order>),
                                      $(u : User, o : Order) => (UserName = u.Name, OrderId = o.Id)))
    // SELECT "t0"."Name", "t1"."Id"
    //   FROM "Users" AS "t0"
    //   CROSS JOIN "Orders" AS "t1"

What's not yet wired
====================

* Per-column nullability inside ``Option<T>`` --- ``o.Total`` doesn't
  compile on ``o : Option<Order>``, and ``o |> unwrap_or(default).Total``
  isn't translated yet. For per-column NULL handling drop to raw SQL
  or restructure with two queries.

.. seealso::

    Full source: :download:`tutorials/sql/16-left_join.das <../../../../tutorials/sql/16-left_join.das>`

    Previous tutorial: :ref:`tutorial_sql_join`

    Next tutorial: :ref:`tutorial_sql_subqueries`
