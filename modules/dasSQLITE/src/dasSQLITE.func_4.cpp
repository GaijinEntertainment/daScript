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
void Module_dasSQLITE::initFunctions_4() {
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4250:16
	addExtern< int (*)(struct sqlite3 *,const void *,int,unsigned int,struct sqlite3_stmt **,const void **) , sqlite3_prepare16_v3 >(*this,lib,"sqlite3_prepare16_v3",SideEffects::worstDefault,"sqlite3_prepare16_v3")
		->args({"db","zSql","nByte","prepFlags","ppStmt","pzTail"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4300:24
	addExtern< const char * (*)(struct sqlite3_stmt *) , sqlite3_sql >(*this,lib,"sqlite3_sql",SideEffects::worstDefault,"sqlite3_sql")
		->args({"pStmt"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4301:18
	addExtern< char * (*)(struct sqlite3_stmt *) , sqlite3_expanded_sql >(*this,lib,"sqlite3_expanded_sql",SideEffects::worstDefault,"sqlite3_expanded_sql")
		->args({"pStmt"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4353:16
	addExtern< int (*)(struct sqlite3_stmt *) , sqlite3_stmt_readonly >(*this,lib,"sqlite3_stmt_readonly",SideEffects::worstDefault,"sqlite3_stmt_readonly")
		->args({"pStmt"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4365:16
	addExtern< int (*)(struct sqlite3_stmt *) , sqlite3_stmt_isexplain >(*this,lib,"sqlite3_stmt_isexplain",SideEffects::worstDefault,"sqlite3_stmt_isexplain")
		->args({"pStmt"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4386:16
	addExtern< int (*)(struct sqlite3_stmt *) , sqlite3_stmt_busy >(*this,lib,"sqlite3_stmt_busy",SideEffects::worstDefault,"sqlite3_stmt_busy")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4589:16
	addExtern< int (*)(struct sqlite3_stmt *,int,double) , sqlite3_bind_double >(*this,lib,"sqlite3_bind_double",SideEffects::worstDefault,"sqlite3_bind_double")
		->args({"","",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4590:16
	addExtern< int (*)(struct sqlite3_stmt *,int,int) , sqlite3_bind_int >(*this,lib,"sqlite3_bind_int",SideEffects::worstDefault,"sqlite3_bind_int")
		->args({"","",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4591:16
	addExtern< int (*)(struct sqlite3_stmt *,int,sqlite3_int64) , sqlite3_bind_int64 >(*this,lib,"sqlite3_bind_int64",SideEffects::worstDefault,"sqlite3_bind_int64")
		->args({"","",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4592:16
	addExtern< int (*)(struct sqlite3_stmt *,int) , sqlite3_bind_null >(*this,lib,"sqlite3_bind_null",SideEffects::worstDefault,"sqlite3_bind_null")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4597:16
	addExtern< int (*)(struct sqlite3_stmt *,int,const struct sqlite3_value *) , sqlite3_bind_value >(*this,lib,"sqlite3_bind_value",SideEffects::worstDefault,"sqlite3_bind_value")
		->args({"","",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4599:16
	addExtern< int (*)(struct sqlite3_stmt *,int,int) , sqlite3_bind_zeroblob >(*this,lib,"sqlite3_bind_zeroblob",SideEffects::worstDefault,"sqlite3_bind_zeroblob")
		->args({"","","n"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4600:16
	addExtern< int (*)(struct sqlite3_stmt *,int,sqlite3_uint64) , sqlite3_bind_zeroblob64 >(*this,lib,"sqlite3_bind_zeroblob64",SideEffects::worstDefault,"sqlite3_bind_zeroblob64")
		->args({"","",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4621:16
	addExtern< int (*)(struct sqlite3_stmt *) , sqlite3_bind_parameter_count >(*this,lib,"sqlite3_bind_parameter_count",SideEffects::worstDefault,"sqlite3_bind_parameter_count")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4649:24
	addExtern< const char * (*)(struct sqlite3_stmt *,int) , sqlite3_bind_parameter_name >(*this,lib,"sqlite3_bind_parameter_name",SideEffects::worstDefault,"sqlite3_bind_parameter_name")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4667:16
	addExtern< int (*)(struct sqlite3_stmt *,const char *) , sqlite3_bind_parameter_index >(*this,lib,"sqlite3_bind_parameter_index",SideEffects::worstDefault,"sqlite3_bind_parameter_index")
		->args({"","zName"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4677:16
	addExtern< int (*)(struct sqlite3_stmt *) , sqlite3_clear_bindings >(*this,lib,"sqlite3_clear_bindings",SideEffects::worstDefault,"sqlite3_clear_bindings")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4693:16
	addExtern< int (*)(struct sqlite3_stmt *) , sqlite3_column_count >(*this,lib,"sqlite3_column_count",SideEffects::worstDefault,"sqlite3_column_count")
		->args({"pStmt"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4722:24
	addExtern< const char * (*)(struct sqlite3_stmt *,int) , sqlite3_column_name >(*this,lib,"sqlite3_column_name",SideEffects::worstDefault,"sqlite3_column_name")
		->args({"","N"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4723:24
	addExtern< const void * (*)(struct sqlite3_stmt *,int) , sqlite3_column_name16 >(*this,lib,"sqlite3_column_name16",SideEffects::worstDefault,"sqlite3_column_name16")
		->args({"","N"});
}
}

