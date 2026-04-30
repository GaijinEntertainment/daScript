.. _tutorial_sql_concurrency:

==============================================
SQL-38 --- Concurrency: threads, contention
==============================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; concurrency
    single: Tutorial; WAL
    single: Tutorial; busy_timeout

Concept-only tutorial --- no new API. SQLite's concurrency
model is blunt but predictable: writers serialize, readers run
in parallel (with WAL), and the connection is the unit of
locking.

Three rules
===========

#. **One connection per thread.** Never share a ``SqlRunner``
   across threads. The handle is connection-state on the C side;
   SQLite itself is thread-safe at the *library* level (built
   with ``SQLITE_THREADSAFE=1``) but a single connection's
   prepared statements + transaction state are not.

#. **WAL + busy_timeout is not optional.** WAL lets readers and
   one writer coexist; ``busy_timeout`` makes contended writes
   wait instead of failing immediately. ``apply_recommended_pragmas``
   sets both. (See :ref:`tut 33 <tutorial_sql_pragma>`.)

#. **Short write transactions win.** Long-running writers block
   every other writer until commit. Compute outside the
   transaction; batch the actual rows; commit fast.

Connection per thread
=====================

Each worker opens its own ``with_sqlite(...)``. The underlying
file is shared; the handle is per-thread. WAL guarantees readers
see a consistent snapshot while another thread writes:

.. code-block:: das

    def worker_read(path : string) {
        with_sqlite(path) $(db) {
            // separate handle, separate prepared-statement cache
            let n = _sql(db |> select_from(type<Counter>) |> count)
            to_log(LOG_INFO, "thread {get_thread_id()} sees n={n}")
        }
    }

Recommended-pragma defaults
===========================

``apply_recommended_pragmas`` sets:

* ``journal_mode = WAL`` (concurrent reads + one writer)
* ``synchronous = NORMAL`` (durable on commit, faster than FULL)
* ``foreign_keys = ON`` (constraint enforcement)
* ``busy_timeout = 5000`` (5s wait on lock contention)

This is the right starting point for any multi-threaded /
multi-process workload.

Busy handling
=============

When a writer holds the write lock and another connection tries
to write, SQLite returns ``SQLITE_BUSY``. With ``busy_timeout=5000``,
libsqlite3 retries internally for up to 5s before surfacing the
error. dasSQLITE turns this into a panic in the strict path, or
``Result::Err`` in the ``try_*`` path. Three patterns:

* **Default (recommended):** ``apply_recommended_pragmas`` plus
  short transactions. Most workloads never hit the 5s ceiling.
* **Manual retry:** ``try_with_transaction`` returns
  ``SqlError``; on ``some(err)``, check the error string for
  ``"database is locked"`` / ``"BUSY"``, sleep with jitter,
  retry up to a cap.
* **Tune the timeout:** ``db |> set_pragma("busy_timeout", 30000)``
  if you have legitimate load spikes longer than 5s.

What breaks
===========

* **Sharing one** ``SqlRunner`` **across threads.** Prepared
  statements held by thread A are silently visible to thread B;
  concurrent ``step`` calls on the same ``sqlite3_stmt*`` is
  undefined behavior. Always open per-thread.
* **Long-running transactions.** A ``with_transaction`` block
  that reads a million rows + computes blocks every other writer
  until commit. Either run the read outside the transaction and
  commit only the writes, or use a snapshot read connection in
  WAL mode (no lock acquired).
* **journal_mode=DELETE (the legacy default).**
  ``apply_recommended_pragmas`` uses WAL. Verify with
  ``query_scalar("PRAGMA journal_mode")`` if you inherit a DB
  built elsewhere.

Out of scope
============

Connection pools, MVCC across connections, SQLite's session
extension, distributed-write coordination --- these are
use-case specific. The 80% case is "one connection per thread,
WAL, busy_timeout, short transactions" and that's what this
tutorial teaches.

.. seealso::

    Full source: :download:`tutorials/sql/38-concurrency.das <../../../../tutorials/sql/38-concurrency.das>`

    Previous tutorial: :ref:`tutorial_sql_bulk`
