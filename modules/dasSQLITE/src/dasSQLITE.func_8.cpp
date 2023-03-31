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
void Module_dasSQLITE::initFunctions_8() {
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6018:17
	addExtern< void (*)(struct sqlite3_context *,int) , sqlite3_result_zeroblob >(*this,lib,"sqlite3_result_zeroblob",SideEffects::worstDefault,"sqlite3_result_zeroblob")
		->args({"","n"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6019:16
	addExtern< int (*)(struct sqlite3_context *,sqlite3_uint64) , sqlite3_result_zeroblob64 >(*this,lib,"sqlite3_result_zeroblob64",SideEffects::worstDefault,"sqlite3_result_zeroblob64")
		->args({"","n"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6034:17
	addExtern< void (*)(struct sqlite3_context *,unsigned int) , sqlite3_result_subtype >(*this,lib,"sqlite3_result_subtype",SideEffects::worstDefault,"sqlite3_result_subtype")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6205:16
	addExtern< int (*)(int) , sqlite3_sleep >(*this,lib,"sqlite3_sleep",SideEffects::worstDefault,"sqlite3_sleep")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6321:16
	addExtern< int (*)(unsigned long,void *) , sqlite3_win32_set_directory >(*this,lib,"sqlite3_win32_set_directory",SideEffects::worstDefault,"sqlite3_win32_set_directory")
		->args({"type","zValue"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6325:16
	addExtern< int (*)(unsigned long,const char *) , sqlite3_win32_set_directory8 >(*this,lib,"sqlite3_win32_set_directory8",SideEffects::worstDefault,"sqlite3_win32_set_directory8")
		->args({"type","zValue"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6326:16
	addExtern< int (*)(unsigned long,const void *) , sqlite3_win32_set_directory16 >(*this,lib,"sqlite3_win32_set_directory16",SideEffects::worstDefault,"sqlite3_win32_set_directory16")
		->args({"type","zValue"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6359:16
	addExtern< int (*)(struct sqlite3 *) , sqlite3_get_autocommit >(*this,lib,"sqlite3_get_autocommit",SideEffects::worstDefault,"sqlite3_get_autocommit")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6372:21
	addExtern< struct sqlite3 * (*)(struct sqlite3_stmt *) , sqlite3_db_handle >(*this,lib,"sqlite3_db_handle",SideEffects::worstDefault,"sqlite3_db_handle")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6394:24
	addExtern< const char * (*)(struct sqlite3 *,int) , sqlite3_db_name >(*this,lib,"sqlite3_db_name",SideEffects::worstDefault,"sqlite3_db_name")
		->args({"db","N"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6426:29
	addExtern< const char * (*)(struct sqlite3 *,const char *) , sqlite3_db_filename >(*this,lib,"sqlite3_db_filename",SideEffects::worstDefault,"sqlite3_db_filename")
		->args({"db","zDbName"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6436:16
	addExtern< int (*)(struct sqlite3 *,const char *) , sqlite3_db_readonly >(*this,lib,"sqlite3_db_readonly",SideEffects::worstDefault,"sqlite3_db_readonly")
		->args({"db","zDbName"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6454:16
	addExtern< int (*)(struct sqlite3 *,const char *) , sqlite3_txn_state >(*this,lib,"sqlite3_txn_state",SideEffects::worstDefault,"sqlite3_txn_state")
		->args({"","zSchema"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6503:26
	addExtern< struct sqlite3_stmt * (*)(struct sqlite3 *,struct sqlite3_stmt *) , sqlite3_next_stmt >(*this,lib,"sqlite3_next_stmt",SideEffects::worstDefault,"sqlite3_next_stmt")
		->args({"pDb","pStmt"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6720:16
	addExtern< int (*)(int) , sqlite3_enable_shared_cache >(*this,lib,"sqlite3_enable_shared_cache",SideEffects::worstDefault,"sqlite3_enable_shared_cache")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6736:16
	addExtern< int (*)(int) , sqlite3_release_memory >(*this,lib,"sqlite3_release_memory",SideEffects::worstDefault,"sqlite3_release_memory")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6750:16
	addExtern< int (*)(struct sqlite3 *) , sqlite3_db_release_memory >(*this,lib,"sqlite3_db_release_memory",SideEffects::worstDefault,"sqlite3_db_release_memory")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6816:26
	addExtern< sqlite3_int64 (*)(sqlite3_int64) , sqlite3_soft_heap_limit64 >(*this,lib,"sqlite3_soft_heap_limit64",SideEffects::worstDefault,"sqlite3_soft_heap_limit64")
		->args({"N"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6817:26
	addExtern< sqlite3_int64 (*)(sqlite3_int64) , sqlite3_hard_heap_limit64 >(*this,lib,"sqlite3_hard_heap_limit64",SideEffects::worstDefault,"sqlite3_hard_heap_limit64")
		->args({"N"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6828:35
	addExtern< void (*)(int) , sqlite3_soft_heap_limit >(*this,lib,"sqlite3_soft_heap_limit",SideEffects::worstDefault,"sqlite3_soft_heap_limit")
		->args({"N"});
}
}

