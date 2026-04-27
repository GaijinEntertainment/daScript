.. _tutorial_sql_left_join:

==============================================
SQL-16 --- ``_left_join`` with ``Option<TB>``
==============================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; _left_join
    single: Tutorial; LEFT JOIN
    single: Tutorial; outer join
    single: Tutorial; Option<TB>

``_left_join(other, on, into)`` has the same 4-arg shape as ``_join``
but the ``into`` lambda's right-side parameter is ``Option<TB>`` ---
``none`` when the left row had no matching right row, ``some(tb)``
otherwise.

Under ``_sql(...)`` the right-side columns become NULL on no-match
rows; ``Option<TB>`` is the daslang lift of that NULL semantics.

Probing the right-side option
=============================

The projection can probe the option:

==================================  ==========================================
Source shape                        Emitted SQL fragment
==================================  ==========================================
``o |> is_some``                    ``"t1"."<rightKey>" IS NOT NULL``
``o |> is_none``                    ``"t1"."<rightKey>" IS NULL``
==================================  ==========================================

The probe column is the right-side ON-key --- if THAT came back
NULL the row had no match.

.. code-block:: das

    let rows <- _sql(db |> select_from(type<User>)
                       |> _left_join(db |> select_from(type<Order>),
                                     $(u : User, o : Order) => u.Id == o.UserId,
                                     $(u : User, o : Option<Order>) => (Name = u.Name, HasOrder = o |> is_some)))
    // SELECT "t0"."Name", "t1"."UserId" IS NOT NULL
    //   FROM "Users" AS "t0"
    //   LEFT JOIN "Orders" AS "t1"
    //     ON "t0"."Id" = "t1"."UserId"

What's not yet wired
====================

* Per-column nullability inside ``Option<TB>`` --- ``o.Total``
  doesn't compile on Option, and ``o |> unwrap_or(default).Total``
  isn't translated yet. For per-column NULL handling drop to raw
  SQL or restructure with two queries.

What's not shipped (locked)
===========================

* ``_right_join`` --- trivially ``_left_join`` with swapped sources
* ``_full_outer_join`` --- rare
* ``_cross_join`` --- accidental footgun in macro form

.. seealso::

    Full source: :download:`tutorials/sql/16-left_join.das <../../../../tutorials/sql/16-left_join.das>`

    Previous tutorial: :ref:`tutorial_sql_join`

    Next tutorial: :ref:`tutorial_sql_subqueries`
