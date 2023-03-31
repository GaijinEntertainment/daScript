options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers

module sqlite_boost shared public

require sqlite public

def sqlite3_column_text_ ( stmt : sqlite3_stmt?; col : int ) : string
    //! Returns the text value of a column in the current row of a result set of a SELECT statement.
    unsafe
        return clone_string(sqlite3_column_text(stmt,col))

def sqlite3_free ( std:string )
    //! Frees memory previously allocated by SQLite.
    sqlite3_free(unsafe(reinterpret<void?> std))
