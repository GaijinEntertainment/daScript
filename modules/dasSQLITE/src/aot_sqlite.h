#pragma once

#include "cb_dasSQLITE.h"
#include "need_dasSQLITE.h"

namespace das {
    int sqlite3_exec(sqlite3 * db, const char * sql, char ** errmsg);
    int sqlite3_exec_cb(sqlite3 * db, const char * sql, char ** errmsg,
        const TBlock<int,const TTemporary<TArray<char *>>,const TTemporary<TArray<char *>>> & block,
            Context * context, LineInfoArg * at );
    int sqlite3_bind_blob_ ( sqlite3_stmt * stmt, int index, void * data, int size );
}
