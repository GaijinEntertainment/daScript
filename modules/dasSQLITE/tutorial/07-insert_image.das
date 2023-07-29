// based on https://zetcode.com/db/sqlitec/

require sqlite/sqlite_boost
require fio

[export]
def main
    let image_file = "{get_das_root()}/examples/pathTracer/path_tracer.png"
    var image_data : array<uint8>
    fopen(image_file,"rb") <| $ ( f )
        if f != null
            fmap(f) <| $(data)
                image_data := data
        else
            panic("can't open {image_file}")
    var db : sqlite3?
    var rc = sqlite3_open("test.db", unsafe(addr(db)))
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "Cannot open database: sqlite3_errmsg(db)\n")
        sqlite3_close(db)
        return
    var res:sqlite3_stmt?
    rc = sqlite3_prepare_v2(db, "INSERT INTO Images(Data) VALUES(?)", -1, unsafe(addr(res)), null)
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "Failed to prepare statement: {sqlite3_errmsg(db)}\n")
        sqlite3_close(db)
        return
    sqlite3_bind_blob(res, 1, image_data)
    rc = sqlite3_step(res)
    if rc != SQLITE_DONE
        to_log(LOG_ERROR, "Failed to insert data: {sqlite3_errmsg(db)}\n")
        sqlite3_close(db)
        return
    sqlite3_finalize(res)
    sqlite3_close(db)
