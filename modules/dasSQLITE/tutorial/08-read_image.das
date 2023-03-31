// based on https://zetcode.com/db/sqlitec/

require sqlite/sqlite_boost
require fio

[export]
def main
    var db : sqlite3?
    var rc = sqlite3_open("test.db", unsafe(addr(db)))
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "Cannot open database: sqlite3_errmsg(db)\n")
        sqlite3_close(db)
        return
    var res:sqlite3_stmt?
    rc = sqlite3_prepare_v2(db, "SELECT Data FROM Images WHERE Id = 1", -1, unsafe(addr(res)), null)
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "Failed to prepare statement: {sqlite3_errmsg(db)}\n")
        sqlite3_close(db)
        return
    rc = sqlite3_step(res)
    if rc != SQLITE_ROW
        to_log(LOG_ERROR, "Failed to step: {sqlite3_errmsg(db)}\n")
        sqlite3_close(db)
        return
    let bytes = sqlite3_column_bytes(res, 0)
    fopen("test.png","wb") <| $ ( f )
        if f != null
            f |> _builtin_write(sqlite3_column_blob(res, 0), bytes)
        else
            panic("can't open image.png")
    sqlite3_finalize(res)
    sqlite3_close(db)

