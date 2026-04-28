.. _tutorial_sql_upsert:

==========================================
SQL-21 --- UPSERT
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; UPSERT
    single: Tutorial; ON CONFLICT
    single: Tutorial; insert_or_ignore
    single: Tutorial; insert_or_replace
    single: Tutorial; _sql_upsert
    single: Tutorial; _sql_upsert_returning
    single: Tutorial; _excluded

Three SQL shapes for "INSERT but, if it collides with a UNIQUE / PK
constraint, do something instead":

==============================================  ===================================================
Form                                            Behavior
==============================================  ===================================================
``db |> insert_or_ignore(row)``                 silent no-op on conflict (returns 0 rows affected)
``db |> insert_or_replace(row)``                wipe-and-reinsert on conflict (loses the old row)
``db |> _sql_upsert(row, on_conflict, ...)``    proper merge --- keep old row, update chosen cols
``db |> _sql_upsert_returning(...)``            same, but capture the post-merge row
``db |> _sql_try_upsert(...)``                  non-panic ``Result<int, string>``
==============================================  ===================================================

``insert_or_ignore`` / ``insert_or_replace`` also have ``try_``
siblings and ``array<T>`` bulk overloads.

INSERT OR IGNORE
================

Silent no-op on PK / UNIQUE conflict. Returns rows-affected: 1 if
inserted, 0 if ignored.

.. code-block:: das

    let n = db |> insert_or_ignore(WordHit(Id = 2, Word = "world", Hits = 1, Last = 200l))
    // INSERT OR IGNORE INTO "WordHits" (...) VALUES (?,?,?,?)

INSERT OR REPLACE
=================

On conflict, the existing row is **deleted** and a fresh one is
inserted from the new values. Columns not in the new row reset to
their defaults --- rarely what you want for partial updates. Use
``_sql_upsert`` for proper merging.

.. code-block:: das

    db |> insert_or_replace(WordHit(Id = 1, Word = "hello", Hits = 42, Last = 999l))
    // INSERT OR REPLACE INTO "WordHits" (...) VALUES (?,?,?,?)

ON CONFLICT ... DO UPDATE --- the proper merge
==============================================

``_sql_upsert(row, on_conflict, do_update)``:

* ``row`` --- the values to ``INSERT``.
* ``on_conflict`` --- ``_.Col`` for a single-column conflict target,
  or ``tuple(_.A, _.B)`` for a composite target. Validated at
  macro-expansion time against the struct's fields.
* ``do_update`` --- a named-tuple ``(Col = expr, ...)``. ``_`` refers
  to the existing row; ``_excluded`` refers to the row we tried to
  ``INSERT``.

.. code-block:: das

    let bumped = db |> _sql_upsert(
        WordHit(Id = 1, Word = "hello", Hits = 1, Last = 1234l),
        _.Id,
        (Hits = _.Hits + 1, Last = _excluded.Last))
    // INSERT INTO "WordHits" (...) VALUES (?,?,?,?)
    //   ON CONFLICT("Id") DO UPDATE SET
    //     "Hits" = "WordHits"."Hits" + 1,
    //     "Last" = "excluded"."Last"

Composite conflict targets
==========================

``tuple(_.Email, _.Tenant)`` resolves against a UNIQUE constraint on
the same column set. Declare the constraint via
``[sql_index(unique = true, fields = (...))]`` (:ref:`tutorial_sql_indexes`).
SQLite throws *"ON CONFLICT clause does not match any PRIMARY KEY or
UNIQUE constraint"* at runtime if no constraint covers the column
list.

.. code-block:: das

    [sql_table(name = "UserAccts"),
     sql_index(fields = ("Email", "Tenant"), unique = true)]
    struct UserAcct {
        @sql_primary_key Id : int
        Email : string
        Name : string
        Tenant : string
    }

    db |> _sql_upsert(
        UserAcct(Id = 999, Email = "x@y.com", Name = "alice2", Tenant = "acme"),
        tuple(_.Email, _.Tenant),
        (Name = _excluded.Name))
    // INSERT INTO "UserAccts" (...) VALUES (?,?,?,?)
    //   ON CONFLICT("Email","Tenant") DO UPDATE SET
    //     "Name" = "excluded"."Name"

UPSERT RETURNING
================

Capture the post-merge row. Returns ``array<T>`` (always one row in
practice; the array shape mirrors ``_sql_update_returning``).

.. code-block:: das

    let after <- db |> _sql_upsert_returning(
        WordHit(Id = 1, Word = "hello", Hits = 1, Last = 0l),
        _.Id,
        (Hits = _.Hits + 1))
    // INSERT INTO "WordHits" (...) VALUES (?,?,?,?)
    //   ON CONFLICT("Id") DO UPDATE SET "Hits" = "WordHits"."Hits" + 1
    //   RETURNING "Id","Word","Hits","Last"

Non-panic ``try_`` variants
===========================

Each upsert has a ``try_`` sibling returning ``Result<int, string>``
(or ``Result<array<T>, string>`` for ``_returning``). Useful for
constraint-violation handling without a panic.

.. code-block:: das

    let attempt = db |> _sql_try_upsert(
        WordHit(Id = 1, Word = "hello", Hits = 1, Last = 0l),
        _.Id,
        (Hits = _.Hits + 1))
    if (attempt |> is_err) {
        // "constraint failed: ..." or similar
    } else {
        let n = attempt |> unwrap
    }

``try_insert_or_ignore`` and ``try_insert_or_replace`` cover the
non-panic shapes for the simpler forms.

.. seealso::

    Full source: :download:`tutorials/sql/21-upsert.das <../../../../tutorials/sql/21-upsert.das>`

    Previous tutorial: :ref:`tutorial_sql_delete`

    Next tutorial: :ref:`tutorial_sql_transactions`
