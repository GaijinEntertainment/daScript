require sqlite/sqlite_boost

[export]
def main
    var db : sqlite3?
    var rc = sqlite3_open("test.db", unsafe(addr(db)))
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "Cannot open database: sqlite3_errmsg(db)\n")
        sqlite3_close(db)
        return
    var res:sqlite3_stmt?
    rc = sqlite3_prepare_v2(db, "SELECT Id, Name FROM Cars WHERE Id = ?", -1, unsafe(addr(res)), null)
    if rc == SQLITE_OK
        sqlite3_bind_int(res, 1, 3)
    else
        to_log(LOG_ERROR, "Failed to execute statement: {sqlite3_errmsg(db)}\n")
    let step = sqlite3_step(res)
    if step == SQLITE_ROW
        let id = sqlite3_column_int(res, 0)
        let name = sqlite3_column_text_(res, 1)
        print("ID = {id}, name = {name}\n")
    if rc != SQLITE_OK
        sqlite3_close(db)
        return
    sqlite3_close(db)

