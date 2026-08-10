.. _tutorial_sql_transactions:

==========================================
SQL-22 --- Transactions
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; transaction
    single: Tutorial; with_transaction
    single: Tutorial; try_transaction
    single: Tutorial; in_transaction
    single: Tutorial; SAVEPOINT
    single: Tutorial; BEGIN IMMEDIATE

Atomic groups of statements:

==============================================  ===================================================
Form                                            Behavior
==============================================  ===================================================
``db |> with_transaction() { ... }``            ``BEGIN`` / ``COMMIT`` (rolls back on early return)
``db |> with_transaction(mode) { ... }``        same, with SQLite-specific BEGIN modifier
``db |> try_transaction() { ... }``             non-panic; returns ``SqlError = Option<string>``
``db |> in_transaction()``                      status query: ``true`` if a txn is active
==============================================  ===================================================

``with_transaction`` emits ``BEGIN`` on entry, ``COMMIT`` on
normal exit, and ``ROLLBACK`` on early return (a block ``return``
unwinds non-locally through ``finally``). A panic does NOT roll
back --- daslang deliberately skips ``finally`` on panic; the open
transaction's writes are discarded when the connection closes.
When nested inside an existing transaction it falls back to
``SAVEPOINT`` / ``RELEASE`` / ``ROLLBACK TO`` so user code can
compose freely without SQLite's "no nested transactions" rule
biting.

Why two overloads instead of one optional parameter
===================================================

Two overloads --- not one optional-parameter function --- because
the trailing-block convention puts the block last; an optional
middle ``mode`` parameter wouldn't resolve from a no-mode call
site. Same shape for ``try_transaction``.

Canonical form
==============

.. das-doc: given [sql_table(name="Friends")] struct Friend { @sql_primary_key Id : int; Name : string }
.. das-doc: given var inscope db = open_sqlite(":memory:")

.. code-block:: das

    db |> with_transaction() {
        db |> insert([
            Friend(Id = 1, Name = "tom"),
            Friend(Id = 2, Name = "jerry")
        ])
    }
    // The INSERTs commit as one unit.
    // The inner array-insert nests via SAVEPOINT.

BEGIN IMMEDIATE
===============

The ``Immediate`` mode takes the RESERVED lock at ``BEGIN``, so
concurrent writers block at ``BEGIN`` time rather than on first
write --- avoids the "another writer raced us between read and
write" trap.

.. code-block:: das

    db |> with_transaction(SqliteTxnMode.Immediate) {
        db |> insert(Friend(Id = 4, Name = "jasmine"))
    }

Available modes: ``Deferred`` (default), ``Immediate``,
``Exclusive``.

Status check
============

``in_transaction()`` wraps SQLite's autocommit flag. Mostly useful
for library code that wants "join an ambient transaction if one
is active, else start one".

.. code-block:: das

    if (db |> in_transaction()) {
        // ...
    }

Nested transactions via SAVEPOINT
=================================

Nested calls fall back to ``SAVEPOINT`` --- proper LIFO
composition, no need for the runner to track depth itself.

.. code-block:: das

    db |> with_transaction() {
        db |> insert(Friend(Id = 5, Name = "max"))
        db |> with_transaction() {
            db |> insert(Friend(Id = 6, Name = "rover"))
        }
    }

Non-panic ``try_transaction``
=============================

Returns ``SqlError = Option<string>``: ``some(errmsg)`` on SQL
failure, ``none`` on success. Useful for retry-on-``SQLITE_BUSY``
loops.

.. code-block:: das

    let r = db |> try_transaction() {
        db |> insert(Friend(Id = 7, Name = "lola"))
    }
    if (r |> is_some) {
        // SQL failure (BEGIN / COMMIT errors)
    }

The ``Option``-based ``try_transaction`` only converts SQL
failures (``BEGIN`` / ``COMMIT`` errors) into ``some(errmsg)``. A
panic from inside the block propagates without rolling back
(``finally`` is skipped on panic) --- fallible steps inside a
transaction should use the ``try_`` API forms.

Why ``Option<string>`` instead of ``Result<void, string>``
==========================================================

``Result<void, E>`` isn't supported yet --- ``void`` can't be a
struct field. ``SqlError = Option<string>`` is the workaround:
``none`` means success, ``some(msg)`` means failure.

.. seealso::

    Full source: :download:`tutorials/sql/22-transactions.das <../../../../tutorials/sql/22-transactions.das>`

    Previous tutorial: :ref:`tutorial_sql_upsert`

    Next tutorial: :ref:`tutorial_sql_foreign_keys`
