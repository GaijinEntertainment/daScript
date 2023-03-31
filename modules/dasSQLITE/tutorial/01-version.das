// based on https://zetcode.com/db/sqlitec/

require sqlite/sqlite_boost

[export]
def main
    print("Hello, sqlite version {sqlite3_libversion()}\n")
    var db : sqlite3?
    var rc = sqlite3_open(":memory:", unsafe(addr(db)))
    if rc != SQLITE_OK
        print("Can't open database: {sqlite3_errmsg(db)}")
        sqlite3_close(db)
        return
    var res : sqlite3_stmt?
    rc = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, unsafe(addr(res)), null)
    if rc != SQLITE_OK
        print("Failed to fetch data: {sqlite3_errmsg(db)}\n")
        sqlite3_close(db)
        return
    rc = sqlite3_step(res)
    if rc == SQLITE_ROW
        print("sqlite3_column_text(res, 0) = {sqlite3_column_text_(res, 0)}\n")
    sqlite3_finalize(res)
    sqlite3_close(db)
