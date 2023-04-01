options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers

module sqlite_boost shared public

require sqlite public

require daslib/contracts

def sqlite3_column_text_ ( stmt : sqlite3_stmt?; col : int ) : string
    //! Returns the text value of a column in the current row of a result set of a SELECT statement.
    unsafe
        return clone_string(sqlite3_column_text(stmt,col))

def sqlite3_free ( std:string )
    //! Frees memory previously allocated by SQLite.
    sqlite3_free(unsafe(reinterpret<void?> std))

[expect_any_array(data)]
def sqlite3_bind_blob ( stmt : sqlite3_stmt?; index : int; data )
    //! Binds a blob value to a prepared statement.
    unsafe
        return sqlite3_bind_blob(stmt, 1, unsafe(addr(data[0])), data |> length)

def sqlite3_get_autocommit_ ( db : sqlite3? ) : bool
    //! Returns true if the database connection is in autocommit mode.
    return sqlite3_get_autocommit(db) != 0

def sqlite3_open ( filename : string; var db : sqlite3?& ) : int
    //! Opens a new database connection to a SQLite database file.
    return sqlite3_open(filename, unsafe(addr(db)))
