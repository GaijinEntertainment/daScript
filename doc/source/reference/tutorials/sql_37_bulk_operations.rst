.. _tutorial_sql_bulk:

==============================================
SQL-37 --- Bulk operations: making writes fast
==============================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; bulk
    single: Tutorial; transaction

Concept-only tutorial --- no new API. SQLite's per-statement
overhead is small but not free; per-row autocommit (the default
outside an explicit transaction) hits the WAL with ``fdatasync``
on every commit. Three tools, in order of how often you reach
for them:

#. ``with_transaction`` --- group N inserts into one ``fdatasync``;
#. ``insert(rows)`` (the ``array<T>`` overload) --- one prepared
   statement, reused across rows;
#. ``INSERT ... SELECT`` via raw ``exec`` --- copy rows DB-side
   without round-tripping through daslang.

Order matters: WAL ``fsync`` dominates per-row latency at small
batch sizes, then statement preparation, then column binding.

Transactions
============

The default autocommit makes every INSERT a separate transaction
--- one ``fsync`` per row. Wrap a batch in ``with_transaction``
and the whole batch is one ``fsync``. ~50x speedup on cold WAL
is typical:

.. code-block:: das

    db |> with_transaction <| $() {
        for (i in range(1000)) {
            db |> insert(Event(Id = i + 1, Kind = "tick", PayloadBytes = 32))
        }
    }

Bulk insert
===========

When the input is already an ``array<T>``, the ``insert(rows)``
overload uses a single prepared statement reused across rows
--- no per-row prepare/bind overhead in the daslang side:

.. code-block:: das

    var events : array<Event>
    for (i in range(500)) {
        events |> emplace(Event(Id = 2000 + i, Kind = "burst",
                                PayloadBytes = 16))
    }
    let inserted = db |> insert(events)

Combine ``with_transaction`` + ``insert(rows)`` for the absolute
fastest path: single transaction, single prepared statement.

``INSERT ... SELECT``
=====================

Filter-and-copy that never round-trips the rows to daslang.
Useful for archive flows: copy a subset out, delete it in the
same transaction:

.. code-block:: das

    db |> exec(
        "INSERT INTO \"Events\" (\"Id\", \"Kind\", \"PayloadBytes\")
         SELECT \"Id\" + 100000, 'archived', \"PayloadBytes\"
         FROM \"Events\" WHERE \"Kind\" = 'tick'")

When to reach for which
=======================

================================== =========================
Source                             Best path
================================== =========================
Already an ``array<T>``            ``insert(rows)``
Mixed kinds / row-by-row decisions ``with_transaction { for ... insert(...) }``
Filter-and-copy DB-side            ``exec("INSERT ... SELECT")``
Single row                         plain ``insert(row)`` --- autocommit is fine
================================== =========================

SQLite's hard ceiling is on the order of 1M rows/sec on
commodity SSD with WAL plus transaction-batched bulk insert.
Beyond that you're either CPU-bound in the daslang side or
paying for indexes --- drop indexes before bulk-load, recreate
after, then ``vacuum`` if free-space matters
(see :ref:`tut 34 <tutorial_sql_backup_vacuum>`).

.. seealso::

    Full source: :download:`tutorials/sql/37-bulk_operations.das <../../../../tutorials/sql/37-bulk_operations.das>`

    Previous tutorial: :ref:`tutorial_sql_attach`
