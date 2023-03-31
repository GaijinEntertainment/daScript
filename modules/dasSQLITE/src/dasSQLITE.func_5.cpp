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
void Module_dasSQLITE::initFunctions_5() {
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4767:24
	addExtern< const char * (*)(struct sqlite3_stmt *,int) , sqlite3_column_database_name >(*this,lib,"sqlite3_column_database_name",SideEffects::worstDefault,"sqlite3_column_database_name")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4768:24
	addExtern< const void * (*)(struct sqlite3_stmt *,int) , sqlite3_column_database_name16 >(*this,lib,"sqlite3_column_database_name16",SideEffects::worstDefault,"sqlite3_column_database_name16")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4769:24
	addExtern< const char * (*)(struct sqlite3_stmt *,int) , sqlite3_column_table_name >(*this,lib,"sqlite3_column_table_name",SideEffects::worstDefault,"sqlite3_column_table_name")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4770:24
	addExtern< const void * (*)(struct sqlite3_stmt *,int) , sqlite3_column_table_name16 >(*this,lib,"sqlite3_column_table_name16",SideEffects::worstDefault,"sqlite3_column_table_name16")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4771:24
	addExtern< const char * (*)(struct sqlite3_stmt *,int) , sqlite3_column_origin_name >(*this,lib,"sqlite3_column_origin_name",SideEffects::worstDefault,"sqlite3_column_origin_name")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4772:24
	addExtern< const void * (*)(struct sqlite3_stmt *,int) , sqlite3_column_origin_name16 >(*this,lib,"sqlite3_column_origin_name16",SideEffects::worstDefault,"sqlite3_column_origin_name16")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4804:24
	addExtern< const char * (*)(struct sqlite3_stmt *,int) , sqlite3_column_decltype >(*this,lib,"sqlite3_column_decltype",SideEffects::worstDefault,"sqlite3_column_decltype")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4805:24
	addExtern< const void * (*)(struct sqlite3_stmt *,int) , sqlite3_column_decltype16 >(*this,lib,"sqlite3_column_decltype16",SideEffects::worstDefault,"sqlite3_column_decltype16")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4889:16
	addExtern< int (*)(struct sqlite3_stmt *) , sqlite3_step >(*this,lib,"sqlite3_step",SideEffects::worstDefault,"sqlite3_step")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:4910:16
	addExtern< int (*)(struct sqlite3_stmt *) , sqlite3_data_count >(*this,lib,"sqlite3_data_count",SideEffects::worstDefault,"sqlite3_data_count")
		->args({"pStmt"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5157:24
	addExtern< const void * (*)(struct sqlite3_stmt *,int) , sqlite3_column_blob >(*this,lib,"sqlite3_column_blob",SideEffects::worstDefault,"sqlite3_column_blob")
		->args({"","iCol"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5158:19
	addExtern< double (*)(struct sqlite3_stmt *,int) , sqlite3_column_double >(*this,lib,"sqlite3_column_double",SideEffects::worstDefault,"sqlite3_column_double")
		->args({"","iCol"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5159:16
	addExtern< int (*)(struct sqlite3_stmt *,int) , sqlite3_column_int >(*this,lib,"sqlite3_column_int",SideEffects::worstDefault,"sqlite3_column_int")
		->args({"","iCol"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5160:26
	addExtern< sqlite3_int64 (*)(struct sqlite3_stmt *,int) , sqlite3_column_int64 >(*this,lib,"sqlite3_column_int64",SideEffects::worstDefault,"sqlite3_column_int64")
		->args({"","iCol"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5161:33
	addExtern< const unsigned char * (*)(struct sqlite3_stmt *,int) , sqlite3_column_text >(*this,lib,"sqlite3_column_text",SideEffects::worstDefault,"sqlite3_column_text")
		->args({"","iCol"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5162:24
	addExtern< const void * (*)(struct sqlite3_stmt *,int) , sqlite3_column_text16 >(*this,lib,"sqlite3_column_text16",SideEffects::worstDefault,"sqlite3_column_text16")
		->args({"","iCol"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5163:27
	addExtern< struct sqlite3_value * (*)(struct sqlite3_stmt *,int) , sqlite3_column_value >(*this,lib,"sqlite3_column_value",SideEffects::worstDefault,"sqlite3_column_value")
		->args({"","iCol"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5164:16
	addExtern< int (*)(struct sqlite3_stmt *,int) , sqlite3_column_bytes >(*this,lib,"sqlite3_column_bytes",SideEffects::worstDefault,"sqlite3_column_bytes")
		->args({"","iCol"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5165:16
	addExtern< int (*)(struct sqlite3_stmt *,int) , sqlite3_column_bytes16 >(*this,lib,"sqlite3_column_bytes16",SideEffects::worstDefault,"sqlite3_column_bytes16")
		->args({"","iCol"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5166:16
	addExtern< int (*)(struct sqlite3_stmt *,int) , sqlite3_column_type >(*this,lib,"sqlite3_column_type",SideEffects::worstDefault,"sqlite3_column_type")
		->args({"","iCol"});
}
}

