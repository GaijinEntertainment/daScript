options gen2

require daslib/sql
require sqlite/sqlite_boost

[export]
def main() {
    // Library-level helper — no DB needed to read the linked libsqlite version.
    print("sqlite version: {sqlite_version()}\n")

    // RAII, callback form. `with_sqlite` opens the DB, passes the runner
    // handle to the block, and calls sqlite3_close_v2 on block exit
    // (including on panic / early return). Canonical form for short-lived
    // DB use. Per-provider name: each future provider ships its own
    // `with_<provider>` — `with_postgres`, `with_mysql`, etc.
    with_sqlite(":memory:") $(db) {
        // query_scalar prepares, steps, reads column 0, and finalizes in one
        // call. Panics (via sqlite3_errmsg) on prepare/step error or on 0
        // rows — appropriate here since SELECT SQLITE_VERSION() always
        // returns a row.
        print("server reports: {db |> query_scalar("SELECT SQLITE_VERSION()", type<string>)}\n")
    }
}
