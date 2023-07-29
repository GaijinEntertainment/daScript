// based on https://zetcode.com/db/sqlitec/

require sqlite/sqlite_boost
require daslib/stringify

[export]
def main
    var db : sqlite3?
    var rc = sqlite3_open("test.db", unsafe(addr(db)))
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "Cannot open database: sqlite3_errmsg(db)\n")
        sqlite3_close(db)
        return
    print("Autocommit: {sqlite3_get_autocommit_(db)}\n")
    sqlite3_close(db)
