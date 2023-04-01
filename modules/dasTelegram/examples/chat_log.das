options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module chat_log shared private

require sqlite/sqlite_boost

var private g_db : sqlite3? = null

[finalize]
def private close_chat_log
    if g_db != null
        sqlite3_close(g_db)
        g_db = null

def private create_chat_table
    var sql = "CREATE TABLE IF NOT EXISTS chat_log (
                chat_id INTEGER,
                message_id INTEGER,
                date INTEGER,
                from_id INTEGER,
                reply_to_message INTEGER,
                text TEXT,
                transcription TEXT);"
    var rc = sqlite3_exec(g_db, sql, null)
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "Error creating table chat_log: {sqlite3_errmsg(g_db)}\n")
        return false
    sql = "CREATE TABLE IF NOT EXISTS users (
                user_id INTEGER UNIQUE,
                is_bot INTEGER,
                first_name TEXT,
                last_name TEXT,
                user_name TEXT);"
    rc = sqlite3_exec(g_db, sql, null)
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "Error creating table users: {sqlite3_errmsg(g_db)}\n")
        return false
    return true

def public create_chat_log ( dbname:string )
    var rc = sqlite3_open(dbname, g_db)
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "Cannot open database: {sqlite3_errmsg(g_db)}\n")
        sqlite3_close(g_db)
        g_db = null
        return false
    rc = sqlite3_exec(g_db, "PRAGMA journal_mode=WAL",null)
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "Cannot set journal mode: {sqlite3_errmsg(g_db)}\n")
        sqlite3_close(g_db)
        g_db = null
        return false
    if !create_chat_table()
        sqlite3_close(g_db)
        g_db = null
        return false
    return true

struct public UserEntry
    user_id : int64
    is_bot : bool
    first_name : string
    last_name : string
    user_name : string

struct public ChatLogEntry
    chat_id : int64
    message_id : int64
    date : int64
    from_id : int64
    reply_to_message : int64
    text : string
    transcription : string

def public write_to_log ( entry:ChatLogEntry )
    var sql = "INSERT INTO chat_log (chat_id, message_id, date, from_id, reply_to_message, text, transcription) VALUES (?, ?, ?, ?, ?, ?, ?)"
    var stmt : sqlite3_stmt?
    var rc = sqlite3_prepare_v2(g_db, sql, -1, unsafe(addr(stmt)), null)
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "Error preparing statement: {sqlite3_errmsg(g_db)}\n")
        return false
    sqlite3_bind_int64(stmt, 1, entry.chat_id)
    sqlite3_bind_int64(stmt, 2, entry.message_id)
    sqlite3_bind_int64(stmt, 3, entry.date)
    sqlite3_bind_int64(stmt, 4, entry.from_id)
    sqlite3_bind_int64(stmt, 5, entry.reply_to_message)
    sqlite3_bind_text(stmt, 6, entry.text)
    sqlite3_bind_text(stmt, 7, entry.transcription)
    rc = sqlite3_step(stmt)
    if rc != SQLITE_DONE
        to_log(LOG_ERROR, "Error inserting data: {sqlite3_errmsg(g_db)}\n")
        return false
    return true
finally
    sqlite3_finalize(stmt)

def public write_user ( user:UserEntry )
    var sql = "INSERT OR IGNORE INTO users (user_id, is_bot, first_name, last_name, user_name) VALUES (?, ?, ?, ?, ?)"
    var stmt : sqlite3_stmt?
    var rc = sqlite3_prepare_v2(g_db, sql, -1, unsafe(addr(stmt)), null)
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "Error preparing statement: {sqlite3_errmsg(g_db)}\n")
        return false
    sqlite3_bind_int64(stmt, 1, user.user_id)
    sqlite3_bind_int(stmt, 2, user.is_bot ? 1 : 0)
    sqlite3_bind_text(stmt, 3, user.first_name)
    sqlite3_bind_text(stmt, 4, user.last_name)
    sqlite3_bind_text(stmt, 5, user.user_name)
    rc = sqlite3_step(stmt)
    if rc != SQLITE_DONE
        to_log(LOG_ERROR, "Error inserting data: {sqlite3_errmsg(g_db)}\n")
        return false
    return true
finally
    sqlite3_finalize(stmt)

def public list_unique_users ( chat_id:int64 ) : array<int64>
    var sql = "SELECT DISTINCT from_id FROM chat_log WHERE chat_id = ?"
    var stmt : sqlite3_stmt?
    var rc = sqlite3_prepare_v2(g_db, sql, -1, unsafe(addr(stmt)), null)
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "Error preparing statement: {sqlite3_errmsg(g_db)}\n")
        return <- [[array<int64>]]
    sqlite3_bind_int64(stmt, 1, chat_id)
    var users : array<int64>
    while true
        rc = sqlite3_step(stmt)
        if rc == SQLITE_DONE
            break
        if rc != SQLITE_ROW
            to_log(LOG_ERROR, "Error reading data: {sqlite3_errmsg(g_db)}\n")
            return <- [[array<int64>]]
        users |> push(sqlite3_column_int64(stmt, 0))
    return <- users
finally
    sqlite3_finalize(stmt)

def public list_chat_users ( chat_id:int64; blk:block<(entry:UserEntry):void>)
    var stmt : sqlite3_stmt?
    var inscope users <- list_unique_users(chat_id)
    if users |> empty
        return false
    var inscope user_table : table<int64>
    for u in users
        user_table |> insert(u)
    var sql = "SELECT user_id, is_bot, first_name, last_name, user_name FROM users"
    var rc = sqlite3_prepare_v2(g_db, sql, -1, unsafe(addr(stmt)), null)
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "Error preparing statement: {sqlite3_errmsg(g_db)}\n")
        return false
    while true
        rc = sqlite3_step(stmt)
        if rc == SQLITE_DONE
            break
        if rc != SQLITE_ROW
            to_log(LOG_ERROR, "Error reading data: {sqlite3_errmsg(g_db)}\n")
            return false
        let user_id = sqlite3_column_int64(stmt, 0)
        if user_table |> key_exists(user_id)
            var entry = [[UserEntry
                user_id = user_id,
                is_bot = sqlite3_column_int(stmt, 1) != 0,
                first_name = sqlite3_column_text_(stmt, 2),
                last_name = sqlite3_column_text_(stmt, 3),
                user_name = sqlite3_column_text_(stmt, 4)
            ]]
            invoke(blk,entry)
    return true
finally
    sqlite3_finalize(stmt)

def public list_chat_log ( chat_id:int64; last_n_entries:int; blk:block<(entry:ChatLogEntry):void>)
    var sql = "SELECT message_id, date, from_id, reply_to_message, text, transcription FROM chat_log WHERE chat_id = ? ORDER BY date DESC LIMIT ?"
    var stmt : sqlite3_stmt?
    var rc = sqlite3_prepare_v2(g_db, sql, -1, unsafe(addr(stmt)), null)
    if rc != SQLITE_OK
        to_log(LOG_ERROR, "Error preparing statement: {sqlite3_errmsg(g_db)}\n")
        return false
    sqlite3_bind_int64(stmt, 1, chat_id)
    sqlite3_bind_int(stmt, 2, last_n_entries)
    while true
        rc = sqlite3_step(stmt)
        if rc == SQLITE_DONE
            break
        if rc != SQLITE_ROW
            to_log(LOG_ERROR, "Error reading data: {sqlite3_errmsg(g_db)}\n")
            return false
        var entry <- [[ChatLogEntry
            chat_id = chat_id,
            message_id = sqlite3_column_int64(stmt, 0),
            date = sqlite3_column_int64(stmt, 1),
            from_id = sqlite3_column_int64(stmt, 2),
            reply_to_message = sqlite3_column_int64(stmt, 3),
            text = sqlite3_column_text_(stmt, 4),
            transcription = sqlite3_column_text_(stmt, 5)
        ]]
        invoke(blk,entry)
    return true
finally
    sqlite3_finalize(stmt)
