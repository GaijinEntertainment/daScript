#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/ast/ast_handle.h"

#include "dasSQLITE.h"
#include "aot_sqlite.h"

namespace das {

// ============================================================================
// sqlite3_backup_run — copy the contents of `src` into `dst` using SQLite's
// online backup API. Loops until the entire database has been copied;
// transient SQLITE_BUSY/SQLITE_LOCKED responses fall back to a 50 ms sleep
// before retrying, matching SQLite's documented recommendation.
//
// Returns the result of sqlite3_backup_finish() once the step loop ends in
// SQLITE_DONE -- i.e. SQLITE_OK on success, or any non-SQLITE_OK rc the
// finish call surfaces. If the step loop exits with a non-SQLITE_DONE rc
// (e.g. SQLITE_IOERR / SQLITE_FULL), that rc is returned directly. The
// caller (daslang `backup_to`) maps SQLITE_OK -> success and anything else
// -> SqlError carrying `sqlite3_errmsg(dst)` (or, if dst-side failed to
// even initialize the backup, `sqlite3_errmsg(dst)` from before the backup
// started).
// ============================================================================

int sqlite3_backup_run ( sqlite3 * src, sqlite3 * dst ) {
    if ( !src || !dst ) return SQLITE_MISUSE;
    sqlite3_backup * bp = sqlite3_backup_init(dst, "main", src, "main");
    if ( !bp ) return sqlite3_errcode(dst);
    int rc;
    do {
        rc = sqlite3_backup_step(bp, 100);  // 100 pages per step
        if ( rc == SQLITE_BUSY || rc == SQLITE_LOCKED ) {
            sqlite3_sleep(50);
        }
    } while ( rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED );
    int frc = sqlite3_backup_finish(bp);
    return rc == SQLITE_DONE ? frc : rc;
}

}
