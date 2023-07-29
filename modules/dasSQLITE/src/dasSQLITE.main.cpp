#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/ast/ast_handle.h"

#include "dasSQLITE.h"

#include "aot_sqlite.h"

namespace das {

#if 0
SQLITE_API int sqlite3_exec(
  sqlite3*,                                  /* An open database */
  const char *sql,                           /* SQL to be evaluated */
  int (*callback)(void*,int,char**,char**),  /* Callback function */
  void *,                                    /* 1st argument to callback */
  char **errmsg                              /* Error msg written here */
);
#endif

int sqlite3_exec(sqlite3 * db, const char * sql, char ** errmsg) {
    return sqlite3_exec(db, sql, nullptr, nullptr, errmsg);
}

struct ExecCallback {
    const Block * block = nullptr;
    Context * context = nullptr;
    LineInfoArg * at = nullptr;
};

int exec_callback ( void * arg, int argc, char ** argv, char ** colName ) {
    ExecCallback * cb = (ExecCallback *) arg;
    Array arrFields;
    arrFields.data = (char *) argv;
    arrFields.capacity = arrFields.size = argv ? argc : 0;
    arrFields.lock = 1;
    arrFields.flags = 0;
    Array arrColumns;
    arrColumns.data = (char *) colName;
    arrColumns.capacity = arrColumns.size = colName ? argc : 0;
    arrColumns.lock = 1;
    arrColumns.flags = 0;
    vec4f args[2] = {
        cast<Array *>::from(&arrFields),
        cast<Array *>::from(&arrColumns)
    };
    auto res = cb->context->invoke(*cb->block, args, nullptr, cb->at);
    return cast<int>::to(res);
}

int sqlite3_exec_cb(sqlite3 * db, const char * sql, char ** errmsg,
        const TBlock<int,const TTemporary<TArray<char *>>,const TTemporary<TArray<char *>>> & block,
            Context * context, LineInfoArg * at ) {
    ExecCallback cb;
    cb.block = &block;
    cb.context = context;
    cb.at = at;
    return sqlite3_exec(db, sql, exec_callback, &cb, errmsg);
}

int sqlite3_bind_blob_ ( sqlite3_stmt * stmt, int index, void * data, int size ) {
    return sqlite3_bind_blob(stmt, index, data, size, SQLITE_TRANSIENT);
}

int sqlite3_bind_text_ ( sqlite3_stmt * stmt, int index, const char * data ) {
    return sqlite3_bind_text(stmt, index, data, -1, SQLITE_TRANSIENT);
}

void Module_dasSQLITE::initMain() {

    addExtern<DAS_BIND_FUN(sqlite3_exec)>(*this,lib,"sqlite3_exec",
        SideEffects::worstDefault, "sqlite3_exec")
            ->args({"db","sql","errmsg"});
    addExtern<DAS_BIND_FUN(sqlite3_exec_cb)>(*this,lib,"sqlite3_exec",
        SideEffects::worstDefault, "sqlite3_exec_cb")
            ->args({"db","sql","errmsg","block","context","at"});
    addExtern<DAS_BIND_FUN(sqlite3_bind_blob_)>(*this,lib,"sqlite3_bind_blob",
        SideEffects::worstDefault, "sqlite3_bind_blob_")
            ->args({"stmt","sqlite3_bind_text_","data","size"});
    addExtern<DAS_BIND_FUN(sqlite3_bind_text_)>(*this,lib,"sqlite3_bind_text",
        SideEffects::worstDefault, "sqlite3_bind_text_")
            ->args({"stmt","index","data"});

    for ( auto & pfn : this->functions.each() ) {
        // ok, lets fix up everything returning uint8? into returning string# and make it unsafe operation
        if ( pfn->result->isPointer() && pfn->result->firstType &&
                pfn->result->firstType->baseType==Type::tUInt8 && pfn->result->firstType->dim.size()==0 ) {
            pfn->result = make_smart<TypeDecl>(Type::tString);
            pfn->result->constant = true;
            pfn->result->temporary = true;
            pfn->unsafeOperation = true;
        }
        // fixup module functions, so that there is a string cast
        bool anyString = false;
        for ( auto & arg : pfn->arguments ) {
            if ( arg->type->isString() && !arg->type->ref ) {
                anyString = true;
            }
        }
        if ( anyString ) {
            pfn->needStringCast = true;
        }
    }
}

ModuleAotType Module_dasSQLITE::aotRequire ( TextWriter & tw ) const {
    tw << "#include \"../modules/dasSQLITE/src/aot_sqlite.h\"\n";
    return ModuleAotType::cpp;
}

}


