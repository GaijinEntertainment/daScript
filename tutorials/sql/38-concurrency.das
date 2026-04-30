options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 38 — concurrency: threads, contention, recovery.
//
// Concept-only tutorial — no new API. SQLite's concurrency model is
// blunt but predictable: writers serialize, readers run in parallel
// (with WAL), and the connection is the unit of locking.
//
// Three rules in one screen:
//
//   1. ONE CONNECTION PER THREAD. Never share a SqlRunner across
//      threads. The handle is connection-state on the C side; SQLite
//      itself is thread-safe at the *library* level (built with
//      SQLITE_THREADSAFE=1) but a single connection's prepared
//      statements + transaction state are not.
//
//   2. WAL + BUSY TIMEOUT IS NOT OPTIONAL. WAL lets readers and one
//      writer coexist; busy_timeout makes contended writes wait
//      instead of failing. `apply_recommended_pragmas` sets both.
//
//   3. SHORT WRITE TRANSACTIONS WIN. Long-running writers block
//      every other writer. Compute outside the transaction, batch
//      the actual rows, commit fast.

[sql_table(name = "Counters")]
struct Counter {
    @sql_primary_key Id : int
    Tally               : int
}

[export]
def main {
    // --- Pattern 1: one connection per thread --- nolint:STYLE014
    // The shape, not a runnable demo (job_que threads aren't required to
    // walk through the tutorial). Each worker opens its OWN
    // `with_sqlite(...)`; the underlying file is shared, the handle is
    // per-thread. WAL guarantees readers see a consistent snapshot
    // while another thread writes.
    //
    //     def worker_read(path : string) {
    //         with_sqlite(path) $(db) {
    //             // separate handle, separate prepared-statement cache
    //             let n = _sql(db |> select_from(type<Counter>) |> count)
    //             to_log(LOG_INFO, "thread {get_thread_id()} sees n={n}")
    //         }
    //     }

    // --- Pattern 2: WAL + busy_timeout (the prod default) --- nolint:STYLE014
    // `apply_recommended_pragmas` sets WAL mode + busy_timeout=5000ms +
    // foreign_keys=ON + synchronous=NORMAL. This is the right starting
    // point for any multi-threaded / multi-process workload.
    with_sqlite(":memory:") $(db) {
        db |> apply_recommended_pragmas()
        db |> create_table(type<Counter>)
        db |> insert(Counter(Id = 1, Tally = 0))

        // Per-thread reader/writer cycles look like this — each call is
        // its own transaction, so each releases the write lock fast:
        for (_round in range(3)) {
            db |> with_transaction() {
                let cur = _sql(db |> select_from(type<Counter>)
                                 |> _where(_.Id == 1)
                                 |> _select(_.Tally)
                                 |> first)
                db |> exec("UPDATE \"Counters\" SET \"Tally\" = ? WHERE \"Id\" = ?", cur + 1, 1)
            }
        }
        let total = _sql(db |> select_from(type<Counter>)
                           |> _where(_.Id == 1)
                           |> _select(_.Tally)
                           |> first)
        to_log(LOG_INFO, "final tally: {total}")
    }
}

// --- BUSY HANDLING --- nolint:STYLE014
//
// When a writer holds the write lock and another connection tries to
// write, SQLite returns SQLITE_BUSY. With `busy_timeout=5000`, libsqlite3
// retries internally for up to 5 s before surfacing SQLITE_BUSY. dasSQLITE
// turns this into a panic in the strict path, or `Result::Err` in the
// `try_*` path. Three patterns for handling:
//
//   - Default (recommended): `apply_recommended_pragmas` + short
//     transactions. Most workloads never hit the 5 s ceiling.
//   - Manual retry: `try_with_transaction` (returns SqlError); on Err,
//     check err substring `"database is locked"` or `"BUSY"`, sleep
//     with jitter, retry up to a cap.
//   - PRAGMA `busy_timeout = N`: tune the wait if you have load
//     spikes longer than 5 s. Ten-second jobs need ten-second timeouts.
//
// --- WHAT BREAKS --------------------------------------------------------
//
//   - SHARING ONE SqlRunner ACROSS THREADS. Prepared statements
//     held by thread A are silently visible to thread B; concurrent
//     `step` calls on the same `sqlite3_stmt*` is undefined behavior.
//     ALWAYS open per-thread.
//   - LONG-RUNNING TRANSACTIONS. A `with_transaction` block that
//     reads a million rows + computes blocks every other writer
//     until commit. Either run the read outside the transaction
//     and commit only the writes, or use a snapshot read connection
//     in WAL mode (no lock acquired).
//   - JOURNAL_MODE=DELETE (the legacy default). `apply_recommended_pragmas`
//     uses WAL. Verify with `query_scalar("PRAGMA journal_mode")` if you
//     inherit a DB built elsewhere.
//
// --- WHAT'S OUT OF SCOPE FOR THIS TUTORIAL -----------------------------
//
// Connection pools, MVCC across connections, SQLite's session
// extension, distributed-write coordination — these are use-case
// specific. The 80% case is "one connection per thread, WAL,
// busy_timeout, short transactions" and that's what this tutorial
// teaches.
