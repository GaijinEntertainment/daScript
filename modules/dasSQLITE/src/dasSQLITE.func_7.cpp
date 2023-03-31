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
void Module_dasSQLITE::initFunctions_7() {
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5645:16
	addExtern< int (*)(struct sqlite3_value *) , sqlite3_value_nochange >(*this,lib,"sqlite3_value_nochange",SideEffects::worstDefault,"sqlite3_value_nochange")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5646:16
	addExtern< int (*)(struct sqlite3_value *) , sqlite3_value_frombind >(*this,lib,"sqlite3_value_frombind",SideEffects::worstDefault,"sqlite3_value_frombind")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5668:16
	addExtern< int (*)(struct sqlite3_value *) , sqlite3_value_encoding >(*this,lib,"sqlite3_value_encoding",SideEffects::worstDefault,"sqlite3_value_encoding")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5680:25
	addExtern< unsigned int (*)(struct sqlite3_value *) , sqlite3_value_subtype >(*this,lib,"sqlite3_value_subtype",SideEffects::worstDefault,"sqlite3_value_subtype")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5697:27
	addExtern< struct sqlite3_value * (*)(const struct sqlite3_value *) , sqlite3_value_dup >(*this,lib,"sqlite3_value_dup",SideEffects::worstDefault,"sqlite3_value_dup")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5698:17
	addExtern< void (*)(struct sqlite3_value *) , sqlite3_value_free >(*this,lib,"sqlite3_value_free",SideEffects::worstDefault,"sqlite3_value_free")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5743:18
	addExtern< void * (*)(struct sqlite3_context *,int) , sqlite3_aggregate_context >(*this,lib,"sqlite3_aggregate_context",SideEffects::worstDefault,"sqlite3_aggregate_context")
		->args({"","nBytes"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5758:18
	addExtern< void * (*)(struct sqlite3_context *) , sqlite3_user_data >(*this,lib,"sqlite3_user_data",SideEffects::worstDefault,"sqlite3_user_data")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5770:21
	addExtern< struct sqlite3 * (*)(struct sqlite3_context *) , sqlite3_context_db_handle >(*this,lib,"sqlite3_context_db_handle",SideEffects::worstDefault,"sqlite3_context_db_handle")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5829:18
	addExtern< void * (*)(struct sqlite3_context *,int) , sqlite3_get_auxdata >(*this,lib,"sqlite3_get_auxdata",SideEffects::worstDefault,"sqlite3_get_auxdata")
		->args({"","N"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6001:17
	addExtern< void (*)(struct sqlite3_context *,double) , sqlite3_result_double >(*this,lib,"sqlite3_result_double",SideEffects::worstDefault,"sqlite3_result_double")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6002:17
	addExtern< void (*)(struct sqlite3_context *,const char *,int) , sqlite3_result_error >(*this,lib,"sqlite3_result_error",SideEffects::worstDefault,"sqlite3_result_error")
		->args({"","",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6003:17
	addExtern< void (*)(struct sqlite3_context *,const void *,int) , sqlite3_result_error16 >(*this,lib,"sqlite3_result_error16",SideEffects::worstDefault,"sqlite3_result_error16")
		->args({"","",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6004:17
	addExtern< void (*)(struct sqlite3_context *) , sqlite3_result_error_toobig >(*this,lib,"sqlite3_result_error_toobig",SideEffects::worstDefault,"sqlite3_result_error_toobig")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6005:17
	addExtern< void (*)(struct sqlite3_context *) , sqlite3_result_error_nomem >(*this,lib,"sqlite3_result_error_nomem",SideEffects::worstDefault,"sqlite3_result_error_nomem")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6006:17
	addExtern< void (*)(struct sqlite3_context *,int) , sqlite3_result_error_code >(*this,lib,"sqlite3_result_error_code",SideEffects::worstDefault,"sqlite3_result_error_code")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6007:17
	addExtern< void (*)(struct sqlite3_context *,int) , sqlite3_result_int >(*this,lib,"sqlite3_result_int",SideEffects::worstDefault,"sqlite3_result_int")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6008:17
	addExtern< void (*)(struct sqlite3_context *,sqlite3_int64) , sqlite3_result_int64 >(*this,lib,"sqlite3_result_int64",SideEffects::worstDefault,"sqlite3_result_int64")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6009:17
	addExtern< void (*)(struct sqlite3_context *) , sqlite3_result_null >(*this,lib,"sqlite3_result_null",SideEffects::worstDefault,"sqlite3_result_null")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:6016:17
	addExtern< void (*)(struct sqlite3_context *,struct sqlite3_value *) , sqlite3_result_value >(*this,lib,"sqlite3_result_value",SideEffects::worstDefault,"sqlite3_result_value")
		->args({"",""});
}
}

