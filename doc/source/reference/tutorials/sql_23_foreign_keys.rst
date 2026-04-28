.. _tutorial_sql_foreign_keys:

==========================================
SQL-23 --- Foreign keys
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; foreign key
    single: Tutorial; FOREIGN KEY
    single: Tutorial; CASCADE
    single: Tutorial; sql_references
    single: Tutorial; sql_on_delete
    single: Tutorial; sql_on_update

DDL-only foreign-key support: a child column names its parent struct,
optional ``on_delete`` / ``on_update`` actions pick the integrity
behavior. dasSQLITE deliberately ships **no navigation properties**
--- to query across a relationship use ``_join`` (:ref:`tutorial_sql_join`),
``_left_join`` (:ref:`tutorial_sql_left_join`), or ``_any`` / ``_in``
correlated subqueries (:ref:`tutorial_sql_subqueries`).

Annotation shape
================

==============================================  ===================================================
Annotation                                      Meaning
==============================================  ===================================================
``@sql_references = "ParentTypeName"``          parent struct (must have ``@sql_primary_key``)
``@sql_on_delete = "<action>"``                 CASCADE / SET NULL / SET DEFAULT / RESTRICT / NO ACTION
``@sql_on_update = "<action>"``                 same value set
==============================================  ===================================================

Each decorator is a separate ``@name = value`` line --- daslang field
annotations are scalar key/value pairs, not function-call shapes.
``@sql_references`` resolves at ``[sql_table]`` expansion time: it
finds the named struct in the same module, reads its ``[sql_table(name
= ...)]``, and finds the ``@sql_primary_key`` field. Defaults when
omitted: ``on_delete = "no_action"``, ``on_update = "no_action"``.

PRAGMA foreign_keys = ON
========================

``with_sqlite`` enables the pragma on every connection. SQLite ships
it OFF for back-compat with pre-3.6.19 databases; without it FK
clauses are parsed and stored but never enforced --- orphans silently
appear and CASCADE silently no-ops. dasSQLITE turns it on always.

CASCADE delete
==============

.. code-block:: das

    [sql_table(name = "Users")]
    struct User {
        @sql_primary_key Id : int
        Name : string
    }

    [sql_table(name = "Orders")]
    struct Order {
        @sql_primary_key Id : int

        @sql_references = "User"
        @sql_on_delete = "cascade"
        UserId : int

        Total : float
    }

    db |> _sql_delete(type<User>, _.Id == 1)
    // DELETE FROM "Users" WHERE "Id" = ?
    // (SQLite cascades internally to DELETE FROM "Orders"
    //  WHERE "UserId" = 1)

SET NULL requires Option<T>
===========================

The FK column must be ``Option<T>`` so SQLite can write NULL when
the parent goes away. ``[sql_table]`` rejects the combination at
compile time if the column is plain ``T``.

.. code-block:: das

    [sql_table(name = "Comments")]
    struct Comment {
        @sql_primary_key Id : int

        @sql_references = "User"
        @sql_on_delete = "set_null"
        @safe_when_uninitialized AuthorId : Option<int>

        Body : string
    }

Action reference
================

==============================================  ===================================================
Annotation value                                ``ON DELETE`` / ``ON UPDATE`` clause
==============================================  ===================================================
``"cascade"``                                   CASCADE  --- propagate the change to children
``"set_null"``                                  SET NULL --- requires ``Option<T>``
``"set_default"``                               SET DEFAULT --- requires field-init or @sql_default_fn
``"restrict"``                                  RESTRICT --- reject the change immediately
``"no_action"``                                 NO ACTION --- the SQL standard default
==============================================  ===================================================

FK violations through ``try_insert``
====================================

Constraint violations surface as SQLite errors at insert/update
time. ``try_insert`` wraps them as ``Result``.

.. code-block:: das

    let res = db |> try_insert(Order(Id = 999, UserId = 9999, Total = 1.0f))
    if (res |> is_err) {
        // "FOREIGN KEY constraint failed"
    }

Drop / create order
===================

Parent before child on create; child before parent on drop. The FK
clause names the parent table by name, so the parent must exist when
the child is created.

.. seealso::

    Full source: :download:`tutorials/sql/23-foreign_keys.das <../../../../tutorials/sql/23-foreign_keys.das>`

    Previous tutorial: :ref:`tutorial_sql_transactions`

    Next tutorial: :ref:`tutorial_sql_indexes`
