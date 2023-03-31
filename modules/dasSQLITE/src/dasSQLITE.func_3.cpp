// this file is generated via daScript automatic C++ binder
// all user modifications will be lost after this file is re-generated

#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "daScript/simulate/bind_enum.h"
#include "dasSQLITE.h"
#include "need_dasSQLITE.h"
namespace das {
#include "dasSQLITE.func.aot.decl.inc"
void Module_dasSQLITE::initFunctions_3() {
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3764:26
	addExtern< sqlite3_int64 (*)(const char *,const char *,sqlite3_int64) , sqlite3_uri_int64 >(*this,lib,"sqlite3_uri_int64",SideEffects::worstDefault,"sqlite3_uri_int64")
		->args({"","",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3765:24
	addExtern< const char * (*)(const char *,int) , sqlite3_uri_key >(*this,lib,"sqlite3_uri_key",SideEffects::worstDefault,"sqlite3_uri_key")
		->args({"z","N"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3794:24
	addExtern< const char * (*)(const char *) , sqlite3_filename_database >(*this,lib,"sqlite3_filename_database",SideEffects::worstDefault,"sqlite3_filename_database")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3795:24
	addExtern< const char * (*)(const char *) , sqlite3_filename_journal >(*this,lib,"sqlite3_filename_journal",SideEffects::worstDefault,"sqlite3_filename_journal")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3796:24
	addExtern< const char * (*)(const char *) , sqlite3_filename_wal >(*this,lib,"sqlite3_filename_wal",SideEffects::worstDefault,"sqlite3_filename_wal")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3815:26
	addExtern< struct sqlite3_file * (*)(const char *) , sqlite3_database_file_object >(*this,lib,"sqlite3_database_file_object",SideEffects::worstDefault,"sqlite3_database_file_object")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3862:29
	addExtern< const char * (*)(const char *,const char *,const char *,int,const char **) , sqlite3_create_filename >(*this,lib,"sqlite3_create_filename",SideEffects::worstDefault,"sqlite3_create_filename")
		->args({"zDatabase","zJournal","zWal","nParam","azParam"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3869:17
	addExtern< void (*)(const char *) , sqlite3_free_filename >(*this,lib,"sqlite3_free_filename",SideEffects::worstDefault,"sqlite3_free_filename")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3931:16
	addExtern< int (*)(struct sqlite3 *) , sqlite3_errcode >(*this,lib,"sqlite3_errcode",SideEffects::worstDefault,"sqlite3_errcode")
		->args({"db"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3932:16
	addExtern< int (*)(struct sqlite3 *) , sqlite3_extended_errcode >(*this,lib,"sqlite3_extended_errcode",SideEffects::worstDefault,"sqlite3_extended_errcode")
		->args({"db"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3933:24
	addExtern< const char * (*)(struct sqlite3 *) , sqlite3_errmsg >(*this,lib,"sqlite3_errmsg",SideEffects::worstDefault,"sqlite3_errmsg")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3934:24
	addExtern< const void * (*)(struct sqlite3 *) , sqlite3_errmsg16 >(*this,lib,"sqlite3_errmsg16",SideEffects::worstDefault,"sqlite3_errmsg16")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3935:24
	addExtern< const char * (*)(int) , sqlite3_errstr >(*this,lib,"sqlite3_errstr",SideEffects::worstDefault,"sqlite3_errstr")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3936:16
	addExtern< int (*)(struct sqlite3 *) , sqlite3_error_offset >(*this,lib,"sqlite3_error_offset",SideEffects::worstDefault,"sqlite3_error_offset")
		->args({"db"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4004:16
	addExtern< int (*)(struct sqlite3 *,int,int) , sqlite3_limit >(*this,lib,"sqlite3_limit",SideEffects::worstDefault,"sqlite3_limit")
		->args({"","id","newVal"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4214:16
	addExtern< int (*)(struct sqlite3 *,const char *,int,struct sqlite3_stmt **,const char **) , sqlite3_prepare >(*this,lib,"sqlite3_prepare",SideEffects::worstDefault,"sqlite3_prepare")
		->args({"db","zSql","nByte","ppStmt","pzTail"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4221:16
	addExtern< int (*)(struct sqlite3 *,const char *,int,struct sqlite3_stmt **,const char **) , sqlite3_prepare_v2 >(*this,lib,"sqlite3_prepare_v2",SideEffects::worstDefault,"sqlite3_prepare_v2")
		->args({"db","zSql","nByte","ppStmt","pzTail"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4228:16
	addExtern< int (*)(struct sqlite3 *,const char *,int,unsigned int,struct sqlite3_stmt **,const char **) , sqlite3_prepare_v3 >(*this,lib,"sqlite3_prepare_v3",SideEffects::worstDefault,"sqlite3_prepare_v3")
		->args({"db","zSql","nByte","prepFlags","ppStmt","pzTail"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4236:16
	addExtern< int (*)(struct sqlite3 *,const void *,int,struct sqlite3_stmt **,const void **) , sqlite3_prepare16 >(*this,lib,"sqlite3_prepare16",SideEffects::worstDefault,"sqlite3_prepare16")
		->args({"db","zSql","nByte","ppStmt","pzTail"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4243:16
	addExtern< int (*)(struct sqlite3 *,const void *,int,struct sqlite3_stmt **,const void **) , sqlite3_prepare16_v2 >(*this,lib,"sqlite3_prepare16_v2",SideEffects::worstDefault,"sqlite3_prepare16_v2")
		->args({"db","zSql","nByte","ppStmt","pzTail"});
}
}

