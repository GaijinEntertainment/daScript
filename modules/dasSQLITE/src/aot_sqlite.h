#pragma once

#include "cb_dasSQLITE.h"
#include "need_dasSQLITE.h"

namespace das {
    int sqlite3_exec(sqlite3 * db, const char * sql, char ** errmsg);
    int sqlite3_exec_cb(sqlite3 * db, const char * sql, char ** errmsg,
        const TBlock<int,const TTemporary<TArray<char *>>,const TTemporary<TArray<char *>>> & block,
            Context * context, LineInfoArg * at );
    int sqlite3_bind_blob_ ( sqlite3_stmt * stmt, int index, void * data, int size );
    int sqlite3_bind_text_ ( sqlite3_stmt * stmt, int index, const char * data );
    int sqlite3_prepare_v2_no_tail ( sqlite3 * db, const char * sql, sqlite3_stmt ** stmt );
    int sqlite3_open_v2_no_vfs ( const char * filename, sqlite3 ** ppDb, int flags );
    int sqlite3_register_function (
        sqlite3 * db, const char * name, Func fn, int nArgs,
        uint8_t tag0, uint8_t tag1, uint8_t tag2, uint8_t tag3,
        uint8_t retTag, bool deterministic, bool directonly,
        Context * context, LineInfoArg * at );
    int sqlite3_backup_run ( sqlite3 * src, sqlite3 * dst );
}
