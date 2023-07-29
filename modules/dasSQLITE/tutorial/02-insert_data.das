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
    let sql = %stringify~
        DROP TABLE IF EXISTS Cars;
        CREATE TABLE Cars(Id INT, Name TEXT, Price INT);
        INSERT INTO Cars VALUES(1, 'Audi', 52642);
        INSERT INTO Cars VALUES(2, 'Mercedes', 57127);
        INSERT INTO Cars VALUES(3, 'Skoda', 9000);
        INSERT INTO Cars VALUES(4, 'Volvo', 29000);
        INSERT INTO Cars VALUES(5, 'Bentley', 350000);
        INSERT INTO Cars VALUES(6, 'Citroen', 21000);
        INSERT INTO Cars VALUES(7, 'Hummer', 41400);
        INSERT INTO Cars VALUES(8, 'Volkswagen', 21600);
    %%
    var err_msg : string
    rc = sqlite3_exec(db, sql, unsafe(addr(err_msg)))
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "SQL error: {err_msg}\n")
        sqlite3_free(err_msg)
        sqlite3_close(db)
        return
    sqlite3_close(db)
