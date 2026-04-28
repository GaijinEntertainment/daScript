options gen2

require daslib/sql
require sqlite/sqlite_boost
require sqlite/sqlite_linq

// Tutorial 20 — DELETE: remove rows.
//
// Mirrors the UPDATE shape:
//
//   (a) by-PK from a row              db |> delete_(row)
//   (b) by-PK from just an id         db |> delete_by_id(type<T>, id)
//   (c) bulk via predicate macro      db |> _sql_delete(type<T>, where)
//   (d) bulk RETURNING                db |> _sql_delete_returning(...)
//   (e) raw escape hatch              db |> exec("DELETE FROM ...")
//   (f) try_ non-panic variants
//
// Naming note: the function form is ``delete_`` (trailing underscore)
// because ``delete`` is a daslang keyword. The macro form uses the
// ``_sql_`` prefix and skips the underscore (``_sql_delete``).
// ``delete_by_id`` and ``try_delete_by_id`` are separate helpers for the
// "I just have the id" case so callers don't have to construct a dummy
// struct just to identify the row.
//
// Like UPDATE, 0 rows-affected is **not** an error — the row simply wasn't
// there. Reserve ``Err`` from try_ variants for SQL failures.

[sql_table(name = "Users")]
struct User {
    @sql_primary_key Id : int
    Name   : string
    Active : bool
}

[sql_table(name = "Orders")]
struct Order {
    @sql_primary_key Id : int
    UserId : int
    Total  : int
}

def fixture(db : SqlRunner) {
    db |> create_table(type<User>)
    db |> create_table(type<Order>)
    db |> insert([
        User(Id = 1, Name = "alice", Active = true),
        User(Id = 2, Name = "bob",   Active = true),
        User(Id = 3, Name = "carol", Active = false),
        User(Id = 4, Name = "dave",  Active = false)
    ])
    db |> insert([
        Order(Id = 10, UserId = 1, Total = 100),
        Order(Id = 11, UserId = 2, Total =  50)
    ])
}

[export]
def main {
    with_sqlite(":memory:") $(db) {
        fixture(db)

        // (a) by-PK from a row ---------------------------------------------
        // ``delete_`` reads only the PK; non-PK fields are ignored. Useful
        // when you already have the row loaded (e.g. from a SELECT).
        // Emitted SQL: ``DELETE FROM "Users" WHERE "Id" = ?``.
        let n1 = db |> delete_(User(Id = 2, Name = "", Active = false))
        to_log(LOG_INFO, "delete_(row): {n1} row(s)\n")

        // (b) by-PK from just an id ----------------------------------------
        // No dummy struct construction — pass the id directly.
        let n2 = db |> delete_by_id(type<User>, 4)
        to_log(LOG_INFO, "delete_by_id: {n2} row(s)\n")

        // (c) bulk delete via the macro ------------------------------------
        // Captured-var binds and ``_.Col`` references both work.
        // Emitted SQL: ``DELETE FROM "Users" WHERE NOT ("Active")``.
        let purged = db |> _sql_delete(type<User>, !_.Active)
        to_log(LOG_INFO, "purged {purged} inactive user(s)\n")

        // (d) bulk RETURNING — capture rows BEFORE they vanish -------------
        // Useful for audit logs, undo queues, "delete-and-publish" pipelines.
        // Emitted SQL: ``DELETE FROM "Orders" WHERE "Total" < ?
        //   RETURNING "Id", "UserId", "Total"``.
        let small_threshold = 75
        let removed_orders <- db |> _sql_delete_returning(type<Order>,
                                                          _.Total < small_threshold)
        for (o in removed_orders) {
            to_log(LOG_INFO, "removed order Id={o.Id} (Total={o.Total})\n")
        }

        // (e) raw-SQL escape hatch -----------------------------------------
        // ``exec`` runs arbitrary SQL with no parameter binding. For
        // dynamic values stick to the macro forms above; this hatch is
        // for migrations / DDL / statements the macro can't translate.
        db |> exec("DELETE FROM Users WHERE Name LIKE 'a%'")
        to_log(LOG_INFO, "raw-SQL delete affected {db |> changes()} row(s)\n")

        // (f) try_ variants — non-panic flow -------------------------------
        let attempt = db |> try_delete_by_id(type<User>, 999)
        if (attempt |> is_err) {
            to_log(LOG_ERROR, "delete failed: {attempt |> unwrap_err}\n")
        } else {
            to_log(LOG_INFO, "ghost-delete touched {attempt |> unwrap} row(s)\n")
        }
    }
}
