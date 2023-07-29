// based on https://zetcode.com/db/sqlitec/

require sqlite/sqlite_boost

[export]
def main
    var db : sqlite3?
    var rc = sqlite3_open("test.db", unsafe(addr(db)))
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "Cannot open database: sqlite3_errmsg(db)\n")
        sqlite3_close(db)
        return
    var err_msg : string
    rc = sqlite3_exec(db, "SELECT name FROM sqlite_master WHERE type='table'", unsafe(addr(err_msg))) <| $ ( values, columns )
        for v,c in values,columns
            print("{c} = {v}; ")
        print("\n")
        return SQLITE_OK
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "SQL error: {err_msg}\n")
        sqlite3_free(err_msg)
        sqlite3_close(db)
        return
    sqlite3_close(db)
