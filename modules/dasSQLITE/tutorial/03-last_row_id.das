require sqlite/sqlite_boost
require daslib/stringify

[export]
def main
    var db : sqlite3?
    var rc = sqlite3_open(":memory:", unsafe(addr(db)))
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "Cannot open database: sqlite3_errmsg(db)\n")
        sqlite3_close(db)
        return
    let sql = %stringify~
        CREATE TABLE Friends(Id INTEGER PRIMARY KEY, Name TEXT);
        INSERT INTO Friends(Name) VALUES ('Tom');
        INSERT INTO Friends(Name) VALUES ('Rebecca');
        INSERT INTO Friends(Name) VALUES ('Jim');
        INSERT INTO Friends(Name) VALUES ('Roger');
        INSERT INTO Friends(Name) VALUES ('Robert');
    %%
    var err_msg : string
    rc = sqlite3_exec(db, sql, unsafe(addr(err_msg)))
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "SQL error: {err_msg}\n")
        sqlite3_free(err_msg)
        sqlite3_close(db)
        return
    let last_id = sqlite3_last_insert_rowid(db)
    to_log(LOG_INFO, "The last Id of the inserted row is {last_id}\n")
    sqlite3_close(db)
