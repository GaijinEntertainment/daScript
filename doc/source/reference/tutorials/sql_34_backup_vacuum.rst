.. _tutorial_sql_backup_vacuum:

==========================================
SQL-34 --- Backup, VACUUM, integrity check
==========================================

.. index::
    single: Tutorial; SQL
    single: Tutorial; SQLite
    single: Tutorial; vacuum
    single: Tutorial; backup_to
    single: Tutorial; integrity_check
    single: Tutorial; optimize

This tutorial covers the maintenance / hygiene side of SQLite. Each
helper has a strict variant (panics on failure) and a ``try_*`` sibling
returning ``SqlError`` / ``Result<...>`` for non-panic recovery.

.. list-table::
   :header-rows: 1
   :widths: 26 74

   * - Helper
     - What it does
   * - ``vacuum``
     - Defragment the file, reclaim deleted-row space.
   * - ``vacuum_into("path")``
     - Snapshot the live DB to a new file (one-off copy, no source
       mutation).
   * - ``optimize``
     - ``PRAGMA optimize``: refresh query-planner statistics.
   * - ``integrity_check``
     - Full corruption scan. Returns ``["ok"]`` on a healthy DB; one
       row per detected issue otherwise.
   * - ``quick_check``
     - Cheaper sibling of ``integrity_check`` (skips index/table
       cross-walk).
   * - ``backup_to(dest)`` / ``backup_to("path")``
     - Online hot backup. Two overloads: into another open runner, or
       directly to a path on disk.

End-to-end
==========

.. code-block:: das

    require daslib/sql
    require sqlite/sqlite_boost

    [sql_table(name = "Logs")]
    struct LogEntry {
        @sql_primary_key Id : int
        Severity : int
        Message  : string
    }

    [export]
    def main {
        with_sqlite(":memory:") $(db) {
            db |> create_table(type<LogEntry>)
            for (i in range(100)) {
                db |> insert(LogEntry(
                    Id = i + 1,
                    Severity = (i % 4),
                    Message = "msg #{i}"))
            }

            // VACUUM ----------------------------------------------------
            // Rewrites the file from scratch. After large DELETE
            // workloads this reclaims free pages and packs surviving
            // rows densely. Cannot run inside an active transaction ---
            // the helper rejects that case before reaching SQLite.
            db |> exec("DELETE FROM Logs WHERE Severity = 0")
            db |> vacuum()

            // OPTIMIZE -------------------------------------------------
            // Cheap; recommended at connection close on long-lived
            // connections. No-op when nothing has changed.
            db |> optimize()

            // INTEGRITY CHECK -----------------------------------------
            let issues <- db |> integrity_check()
            if (length(issues) == 1 && issues[0] == "ok") {
                to_log(LOG_INFO, "integrity_check: db is healthy\n")
            } else {
                for (msg in issues) {
                    to_log(LOG_ERROR, "  - {msg}\n")
                }
            }
        }
    }

VACUUM in a transaction returns ``Err``
=======================================

The strict form would panic mid-transaction; ``try_vacuum`` surfaces
the error so the caller can recover:

.. code-block:: das

    db |> with_transaction() {
        let err = db |> try_vacuum()
        if (err |> is_some) {
            to_log(LOG_INFO, "try_vacuum mid-tx: {err |> unwrap}\n")
        }
    }

VACUUM INTO --- file snapshot
=============================

``VACUUM INTO '<path>'`` writes a one-off copy of the live DB to a new
file without touching the source. The path is single-quoted with
``''`` doubling --- SQLite doesn't accept ``?`` binds for VACUUM:

.. code-block:: das

    db |> vacuum_into("/tmp/snapshot.sqlite")
    db |> vacuum_into("with'apostrophe.sqlite")  // quotes are escaped

Use ``vacuum_into`` when you want a defragmented snapshot. For a hot
copy of the live DB without re-packing, use ``backup_to`` (below).

Online hot backup --- ``backup_to``
===================================

The Backup API copies a live database into another open connection or
directly to disk. The destination receives the full state of the
source; transient ``SQLITE_BUSY`` / ``SQLITE_LOCKED`` responses
(concurrent writers on the source) are retried automatically with a
50 ms backoff.

Snapshot into another runner (e.g. an in-memory clone):

.. code-block:: das

    with_sqlite(":memory:") $(snapshot) {
        db |> backup_to(snapshot)
        // ... query `snapshot` to inspect a frozen view ...
    }

Snapshot directly to disk (the file is opened, populated, and closed
for you):

.. code-block:: das

    db |> backup_to("/tmp/hot_snapshot.sqlite")

The strict form panics on any non-completion; the ``try_`` form
returns ``SqlError``. ``backup_to`` replaces the destination
wholesale --- pre-existing tables on the destination are gone after
the call.

Integrity check return shape
============================

``integrity_check`` returns ``array<string>``:

* ``["ok"]`` --- the database is healthy.
* One row per detected issue otherwise (truncated file, missing index
  entry, dangling page chain, ...).

The helper returns rows verbatim; the caller decides how to react.
``quick_check`` has the same return shape but skips the per-index
cross-table walk --- useful as a fast pre-flight before running the
full ``integrity_check``.

Deferred
========

* **Backup progress callback.** ``sqlite3_backup_step(bp, N)``
  returning remaining + pagecount, surfaced to a daslang block.
  Useful for long-running backups under a "saving..." progress bar.
* **First-N rows for** ``integrity_check`` / ``quick_check``.
  SQLite's pragma accepts a row-cap argument
  (``PRAGMA integrity_check(10)`` stops after 10 issues). The current
  helper always runs the full scan; add an optional ``N`` once a real
  consumer asks.

.. seealso::

    Full source: :download:`tutorials/sql/34-backup_vacuum.das <../../../../tutorials/sql/34-backup_vacuum.das>`

    Previous tutorial: :ref:`tutorial_sql_pragma`

    Next tutorial: :ref:`tutorial_sql_streaming`
