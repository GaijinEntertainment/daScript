options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 19 — UPDATE: change existing rows.
//
// Three flavours, mirroring the SELECT side:
//
//   (a) by-PK whole-row replace          db |> update(row)
//   (b) bulk via predicate macro         db |> _sql_update(type<T>, where, set)
//   (c) bulk RETURNING                   db |> _sql_update_returning(...)
//   (d) raw escape hatch                 db |> exec(sql)
//   (e) try_ non-panic variants          try_update / _sql_try_update / *_returning
//
// All forms return ``int`` rows-affected (or ``array<T>`` for RETURNING).
// 0 rows-affected is **not** an error — the row simply wasn't there.
// ``Err`` from the try_ variants is reserved for genuine SQL failures
// (constraint violation, IO, BUSY).
//
// Naming: the function form is plain ``update`` / ``try_update``. The macro
// form carries the ``_sql_`` prefix — ``_sql_update`` / ``_sql_try_update`` /
// ``_sql_update_returning`` / ``_sql_try_update_returning`` — to match the
// flagship ``_sql(...)`` macro and keep the SQL provenance visible. RETURNING
// is macro-only; there is no plain ``update_returning`` function.

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name     : string
    Email    : string
    Active   : bool
    LastSeen : int64
}

def fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> insert([
        User(Id = 1, Name = "alice", Email = "a@x.com", Active = true,  LastSeen = 100l),
        User(Id = 2, Name = "bob",   Email = "b@x.com", Active = true,  LastSeen = 200l),
        User(Id = 3, Name = "carol", Email = "c@x.com", Active = true,  LastSeen = 300l),
        User(Id = 4, Name = "dave",  Email = "d@x.com", Active = false, LastSeen = 400l)
    ])
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        fixture(db)

        // (a) by-PK whole-row replace ---------------------------------------
        // Pass a row; the PK identifies it, every other column is rewritten.
        // Emitted SQL: ``UPDATE "Users" SET "Name"=?, "Email"=?, "Active"=?,
        //                "LastSeen"=? WHERE "Id" = ?``.
        let n1 = db |> update(User(Id = 1, Name = "alice", Email = "alice@new.com",
                                    Active = true, LastSeen = 100l))
        to_log(LOG_INFO, "by-PK update affected {n1} row(s)\n")

        // 0 rows-affected when the PK doesn't match — not an error.
        let n_missing = db |> update(User(Id = 999, Name = "ghost", Email = "",
                                           Active = false, LastSeen = 0l))
        to_log(LOG_INFO, "by-PK update on missing row: {n_missing} (expect 0)\n")

        // (b) bulk update via the `_sql_update` macro -----------------------
        // ``_sql_update(type<T>, where_expr, set_expr)`` translates the
        // predicate to SQL and binds captured locals automatically. The set
        // clause is a named-tuple literal ``(Col=expr, ...)`` whose RHS may
        // reference column refs (``_.Other``) and outer-scope locals.
        // Emitted SQL: ``UPDATE "Users" SET "Active" = ? WHERE "LastSeen" < ?``.
        let cutoff : int64 = 250l
        let stale = db |> _sql_update(type<User>, _.LastSeen < cutoff, (Active = false))
        to_log(LOG_INFO, "marked {stale} stale users inactive\n")

        // (c) bulk RETURNING — capture the rows AFTER the update -----------
        // Emitted SQL: ``UPDATE "Users" SET "Active" = ?, "LastSeen" = ?
        //   WHERE "Id" = ? RETURNING "Id", "Name", "Email", "Active", "LastSeen"``.
        let touch_at : int64 = 1000l
        let revived <- db |> _sql_update_returning(type<User>, _.Id == 4,
            (Active = true, LastSeen = touch_at))
        for (u in revived) {
            to_log(LOG_INFO, "revived: {u.Name} (active={u.Active}, last={u.LastSeen})\n")
        }

        // (d) raw-SQL escape hatch -----------------------------------------
        // ``exec`` runs arbitrary SQL with no parameter binding — useful
        // for migrations or queries the macro can't (or shouldn't)
        // translate. For dynamic values in raw SQL, format them into the
        // string yourself (taking care to escape) or stick with the macro
        // forms above. ``changes()`` reports rows-affected from the most
        // recent INSERT / UPDATE / DELETE on this connection.
        db |> exec("UPDATE Users SET Name = 'admin' WHERE Id = 1")
        to_log(LOG_INFO, "raw-SQL update affected {db |> changes()} row(s)\n")

        // (e) try_ variants — non-panic flow -------------------------------
        // ``try_update`` / ``_sql_try_update`` return ``Result<int, string>``.
        // 0 rows-affected is NOT an Err (reserved for constraint / IO / BUSY).
        let attempt = db |> try_update(User(Id = 999, Name = "phantom",
                                             Email = "", Active = false, LastSeen = 0l))
        if (attempt |> is_err) {
            to_log(LOG_ERROR, "update failed: {attempt |> unwrap_err}\n")
        } else {
            to_log(LOG_INFO, "phantom-update touched {attempt |> unwrap} row(s)\n")
        }
    }
}
