.. _tutorial_sql_delete:

==========================================
SQL-20 --- DELETE
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; DELETE
    single: Tutorial; delete_
    single: Tutorial; delete_by_id
    single: Tutorial; _sql_delete
    single: Tutorial; _sql_delete_returning
    single: Tutorial; try_delete_

Six flavours, mirroring UPDATE:

==============================================  ===================================================
Form                                            When to use
==============================================  ===================================================
``db |> delete_(row)``                          by-PK from a loaded struct (non-PK fields ignored)
``db |> delete_by_id(type<T>, id)``             by-PK when you only have the id
``db |> _sql_delete(type<T>, where)``           bulk via predicate macro
``db |> _sql_delete_returning(...)``            bulk + capture rows BEFORE they vanish
``db |> exec(sql)``                             raw escape hatch
``db |> try_delete_*`` / ``_sql_try_delete*``   non-panic ``Result`` variants
==============================================  ===================================================

Naming
======

The function form is ``delete_`` (trailing underscore) because
``delete`` is a daslang keyword. The macro form uses the ``_sql_``
prefix and skips the underscore (``_sql_delete``).
``delete_by_id`` and ``try_delete_by_id`` are separate helpers for
the "I just have the id" case so callers don't have to construct
a dummy struct just to identify the row.

As with UPDATE, 0 rows-affected is **not** an error --- the row
simply wasn't there. Reserve ``Err`` from ``try_`` variants for
SQL failures.

By-PK from a row
================

Useful when you already have the row loaded (e.g. from a SELECT).

.. code-block:: das

    let n1 = db |> delete_(User(Id = 2, Name = "", Active = false))
    // DELETE FROM "Users" WHERE "Id" = ?

By-PK from just an id
=====================

No dummy struct construction --- pass the id directly.

.. code-block:: das

    let n2 = db |> delete_by_id(type<User>, 4)
    // DELETE FROM "Users" WHERE "Id" = ?

Bulk delete via the macro
=========================

Captured-var binds and ``_.Col`` references both work.

.. code-block:: das

    let purged = db |> _sql_delete(type<User>, !_.Active)
    // DELETE FROM "Users" WHERE NOT ("Active")

Bulk RETURNING --- capture rows BEFORE they vanish
==================================================

Useful for audit logs, undo queues, "delete-and-publish" pipelines.

.. code-block:: das

    let small_threshold = 75
    let removed_orders <- db |> _sql_delete_returning(type<Order>,
                                                      _.Total < small_threshold)
    // DELETE FROM "Orders" WHERE "Total" < ?
    //   RETURNING "Id", "UserId", "Total"

Raw-SQL escape hatch
====================

``exec`` runs arbitrary SQL with no parameter binding. For dynamic
values stick to the macro forms above; this hatch is for
migrations / DDL / statements the macro can't translate.

.. code-block:: das

    db |> exec("DELETE FROM Users WHERE Name LIKE 'a%'")

Non-panic ``try_`` variants
===========================

.. code-block:: das

    let attempt = db |> try_delete_by_id(type<User>, 999)
    if (attempt |> is_err) {
        // SQL failure (constraint / IO / BUSY)
    } else {
        let n = attempt |> unwrap   // 0 if id didn't match
    }

.. seealso::

    Full source: :download:`tutorials/sql/20-delete.das <../../../../tutorials/sql/20-delete.das>`

    Previous tutorial: :ref:`tutorial_sql_update`

    Next tutorial: :ref:`tutorial_sql_transactions`
