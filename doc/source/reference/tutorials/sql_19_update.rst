.. _tutorial_sql_update:

==========================================
SQL-19 --- UPDATE
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; UPDATE
    single: Tutorial; update
    single: Tutorial; _sql_update
    single: Tutorial; _sql_update_returning
    single: Tutorial; try_update
    single: Tutorial; RETURNING

Five flavours, mirroring the SELECT side:

==============================================  ===================================================
Form                                            When to use
==============================================  ===================================================
``db |> update(row)``                           by-PK whole-row replace from a loaded struct
``db |> _sql_update(type<T>, where, set)``      bulk: predicate + named-tuple set clause
``db |> _sql_update_returning(...)``            bulk + capture rows AFTER the update
``db |> exec(sql)``                             raw escape hatch for migrations / dynamic SQL
``db |> try_update(...)`` / ``_sql_try_*``      non-panic ``Result<int, string>`` variants
==============================================  ===================================================

All forms return ``int`` rows-affected (or ``array<T>`` for
RETURNING). 0 rows-affected is **not** an error --- the row simply
wasn't there. ``Err`` from the ``try_`` variants is reserved for
genuine SQL failures (constraint violation, IO, BUSY).

Naming
======

The function form is plain ``update`` / ``try_update``. The macro
form carries the ``_sql_`` prefix --- ``_sql_update`` /
``_sql_try_update`` / ``_sql_update_returning`` /
``_sql_try_update_returning`` --- to match the flagship
``_sql(...)`` macro and keep the SQL provenance visible at the call
site. RETURNING is macro-only; there is no plain
``update_returning`` function.

By-PK whole-row replace
=======================

.. code-block:: das

    let n1 = db |> update(User(Id = 1, Name = "alice", Email = "alice@new.com",
                                Active = true, LastSeen = 100l))
    // UPDATE "Users" SET "Name"=?, "Email"=?, "Active"=?, "LastSeen"=?
    //   WHERE "Id" = ?

A non-matching PK returns 0 rows-affected. Not an error.

Bulk update via the macro
=========================

``_sql_update(type<T>, where_expr, set_expr)`` translates the
predicate to SQL and binds captured locals automatically. The set
clause is a named-tuple literal ``(Col=expr, ...)`` whose RHS may
reference column refs (``_.Other``) and outer-scope locals.

.. code-block:: das

    let cutoff : int64 = 250l
    let stale = db |> _sql_update(type<User>, _.LastSeen < cutoff, (Active = false))
    // UPDATE "Users" SET "Active" = ? WHERE "LastSeen" < ?

Bulk RETURNING --- capture rows AFTER the update
================================================

.. code-block:: das

    let touch_at : int64 = 1000l
    let revived <- db |> _sql_update_returning(type<User>, _.Id == 4,
        (Active = true, LastSeen = touch_at))
    // UPDATE "Users" SET "Active" = ?, "LastSeen" = ? WHERE "Id" = ?
    //   RETURNING "Id", "Name", "Email", "Active", "LastSeen"

Useful for audit logs, "update-and-publish" pipelines, or any
caller that needs the post-update row state in one round-trip.

Raw-SQL escape hatch
====================

``exec`` runs arbitrary SQL with no parameter binding --- useful
for migrations or queries the macro can't (or shouldn't) translate.
For dynamic values in raw SQL, format them into the string yourself
(taking care to escape) or stick with the macro forms.

.. code-block:: das

    db |> exec("UPDATE Users SET Name = 'admin' WHERE Id = 1")
    // db |> changes() reports rows-affected from the most recent
    // INSERT / UPDATE / DELETE on this connection.

Non-panic ``try_`` variants
===========================

Return ``Result<int, string>``. 0 rows-affected is NOT an ``Err``
(reserved for constraint violation / IO / BUSY).

.. code-block:: das

    let attempt = db |> try_update(User(Id = 999, Name = "phantom",
                                         Email = "", Active = false, LastSeen = 0l))
    if (attempt |> is_err) {
        // "constraint failed: ..." or similar
    } else {
        let n = attempt |> unwrap   // 0 if PK didn't match
    }

.. seealso::

    Full source: :download:`tutorials/sql/19-update.das <../../../../tutorials/sql/19-update.das>`

    Previous tutorial: :ref:`tutorial_sql_null_handling`

    Next tutorial: :ref:`tutorial_sql_delete`
