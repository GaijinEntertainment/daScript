options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 33 — PRAGMA: tuning the connection.
//
// PRAGMA is SQLite's per-connection control surface. It covers:
//   - Performance toggles (journal_mode, synchronous, cache_size, mmap_size)
//   - Behavior switches (foreign_keys, recursive_triggers, trusted_schema)
//   - Diagnostics (integrity_check, quick_check — see tut 34)
//
// Some PRAGMAs are connection-scoped (`foreign_keys`, `busy_timeout`).
// Others are database-scoped and persist on disk (`journal_mode`,
// `page_size` after VACUUM). dasSQLITE's helpers make no distinction —
// they just emit `PRAGMA "<name>" = <value>` and let SQLite handle it.
//
// Three typed `value` overloads cover the common shapes:
//   set_pragma(db; name; value : string)   // 'WAL', 'NORMAL', 'utf8'
//   set_pragma(db; name; value : int64)    // 5000, 50000, 4096
//   set_pragma(db; name; value : bool)     // ON / OFF
//
// PRAGMA values can't be bound with `?` — SQLite parses them at prepare
// time. The helpers format the value into the SQL inline; string values
// are single-quoted with `''` doubling for safety. PRAGMA names + values
// are admin-controlled (not user input), so this is fine.

[export]
def main {
    with_sqlite(":memory:") $(db) {
        // --- Strict variants — panic on a malformed pragma --------------
        db |> set_pragma("journal_mode", "WAL")     // string
        db |> set_pragma("busy_timeout", 5000l)     // int64 (note `l` suffix)
        db |> set_pragma("foreign_keys", true)      // bool

        // --- Read pragmas back through the typed query rail ------------
        // PRAGMA `<name>` is a SELECT-shaped statement returning a single
        // column row, so `query_scalar(sql, type<T>)` (tut 13) reads it.
        let mode = db |> query_scalar("PRAGMA journal_mode", type<string>)
        to_log(LOG_INFO, "journal_mode = {mode}\n")
        let busy = db |> query_scalar("PRAGMA busy_timeout", type<int>)
        to_log(LOG_INFO, "busy_timeout = {busy}\n")
        let fks = db |> query_scalar("PRAGMA foreign_keys", type<int>)
        to_log(LOG_INFO, "foreign_keys = {fks}\n")

        // --- try_ variant — non-panic recovery -------------------------
        // SQLite rejects `journal_mode` changes inside an open transaction;
        // the strict form would panic, the try form returns SqlError.
        db |> with_transaction() {
            let err = db |> try_set_pragma("journal_mode", "DELETE")
            // :memory: ignores journal mode at the disk level so this may
            // succeed depending on build flags. The point of the try form
            // is recovery — a real disk DB would surface SQLITE_BUSY here.
            if (err |> is_some) {
                to_log(LOG_INFO, "journal_mode change rejected mid-transaction: {err |> unwrap}\n")
            }
        }

        // --- apply_recommended_pragmas — connection-setup curated set --
        // Call this right after `with_sqlite` opens the connection. The
        // exact contents are documented in `sqlite_boost.das`; today they
        // are: WAL + busy_timeout=5s + foreign_keys=ON + synchronous=NORMAL.
        // If your workload demands different defaults (durability-first
        // servers → synchronous=FULL; embedded ROM-like data → journal_mode
        // = OFF) call `set_pragma` directly.
        let setup_err = db |> try_apply_recommended_pragmas()
        if (setup_err |> is_some) {
            to_log(LOG_ERROR, "failed to apply pragmas: {setup_err |> unwrap}\n")
        }

        // --- Unknown-pragma quirk --------------------------------------
        // SQLite is famously permissive with PRAGMA: typos succeed silently
        // (no error), and the corresponding `query_scalar` read returns 0
        // / "" / row count 0 depending on shape. The lookup table is
        // closed; sqlite3 simply ignores names it doesn't recognize.
        // Verify your spelling by reading back via `query_scalar`.
    }
}

// --- Reading PRAGMAs ---------------------------------------------------
//
// Always-set pragmas (booleans / ints):
//   PRAGMA foreign_keys                      -> 0 or 1   query_scalar(sql, type<int>)
//   PRAGMA busy_timeout                       -> ms      query_scalar(sql, type<int>)
//   PRAGMA cache_size                         -> pages   query_scalar(sql, type<int>)
//   PRAGMA journal_mode                       -> string  query_scalar(sql, type<string>)
//
// Multi-row pragmas (use `query` family):
//   PRAGMA table_info('Cars')                 -> rows    query(sql, type<TableInfoRow>)
//   PRAGMA index_list('Cars')                 -> rows    ...
//   PRAGMA integrity_check                    -> rows    (see tut 34)

// --- DEFERRED ---------------------------------------------------------
//
//   1. Compile-time enum surface for booleans / well-known string pragmas
//      (e.g. set_journal_mode(JournalMode.WAL)) — adds a layer of typo
//      protection at the cost of needing maintenance whenever SQLite ships
//      a new mode. Defer until the surface stabilizes.
//
//   2. Open-time pragma bundle on `with_sqlite`
//      (`with_sqlite(path, pragmas=(journal_mode="WAL", ...))`) — would
//      shave a few lines at every call site. Defer until a real consumer
//      asks for it.
