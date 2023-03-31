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
void Module_dasSQLITE::initFunctions_10() {
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7840:16
	addExtern< int (*)(struct sqlite3_mutex *) , sqlite3_mutex_try >(*this,lib,"sqlite3_mutex_try",SideEffects::worstDefault,"sqlite3_mutex_try")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7841:17
	addExtern< void (*)(struct sqlite3_mutex *) , sqlite3_mutex_leave >(*this,lib,"sqlite3_mutex_leave",SideEffects::worstDefault,"sqlite3_mutex_leave")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7996:27
	addExtern< struct sqlite3_mutex * (*)(struct sqlite3 *) , sqlite3_db_mutex >(*this,lib,"sqlite3_db_mutex",SideEffects::worstDefault,"sqlite3_db_mutex")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8039:16
	addExtern< int (*)(struct sqlite3 *,const char *,int,void *) , sqlite3_file_control >(*this,lib,"sqlite3_file_control",SideEffects::worstDefault,"sqlite3_file_control")
		->args({"","zDbName","op",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8152:16
	addExtern< int (*)() , sqlite3_keyword_count >(*this,lib,"sqlite3_keyword_count",SideEffects::worstDefault,"sqlite3_keyword_count");
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8153:16
	addExtern< int (*)(int,const char **,int *) , sqlite3_keyword_name >(*this,lib,"sqlite3_keyword_name",SideEffects::worstDefault,"sqlite3_keyword_name")
		->args({"","",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8154:16
	addExtern< int (*)(const char *,int) , sqlite3_keyword_check >(*this,lib,"sqlite3_keyword_check",SideEffects::worstDefault,"sqlite3_keyword_check")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8199:25
	addExtern< struct sqlite3_str * (*)(struct sqlite3 *) , sqlite3_str_new >(*this,lib,"sqlite3_str_new",SideEffects::worstDefault,"sqlite3_str_new")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8214:18
	addExtern< char * (*)(struct sqlite3_str *) , sqlite3_str_finish >(*this,lib,"sqlite3_str_finish",SideEffects::worstDefault,"sqlite3_str_finish")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8250:17
	addExtern< void (*)(struct sqlite3_str *,const char *,int) , sqlite3_str_append >(*this,lib,"sqlite3_str_append",SideEffects::worstDefault,"sqlite3_str_append")
		->args({"","zIn","N"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8251:17
	addExtern< void (*)(struct sqlite3_str *,const char *) , sqlite3_str_appendall >(*this,lib,"sqlite3_str_appendall",SideEffects::worstDefault,"sqlite3_str_appendall")
		->args({"","zIn"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8252:17
	addExtern< void (*)(struct sqlite3_str *,int,char) , sqlite3_str_appendchar >(*this,lib,"sqlite3_str_appendchar",SideEffects::worstDefault,"sqlite3_str_appendchar")
		->args({"","N","C"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8253:17
	addExtern< void (*)(struct sqlite3_str *) , sqlite3_str_reset >(*this,lib,"sqlite3_str_reset",SideEffects::worstDefault,"sqlite3_str_reset")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8284:16
	addExtern< int (*)(struct sqlite3_str *) , sqlite3_str_errcode >(*this,lib,"sqlite3_str_errcode",SideEffects::worstDefault,"sqlite3_str_errcode")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8285:16
	addExtern< int (*)(struct sqlite3_str *) , sqlite3_str_length >(*this,lib,"sqlite3_str_length",SideEffects::worstDefault,"sqlite3_str_length")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8286:18
	addExtern< char * (*)(struct sqlite3_str *) , sqlite3_str_value >(*this,lib,"sqlite3_str_value",SideEffects::worstDefault,"sqlite3_str_value")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8314:16
	addExtern< int (*)(int,int *,int *,int) , sqlite3_status >(*this,lib,"sqlite3_status",SideEffects::worstDefault,"sqlite3_status")
		->args({"op","pCurrent","pHighwater","resetFlag"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8315:16
	addExtern< int (*)(int,long long *,long long *,int) , sqlite3_status64 >(*this,lib,"sqlite3_status64",SideEffects::worstDefault,"sqlite3_status64")
		->args({"op","pCurrent","pHighwater","resetFlag"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8424:16
	addExtern< int (*)(struct sqlite3 *,int,int *,int *,int) , sqlite3_db_status >(*this,lib,"sqlite3_db_status",SideEffects::worstDefault,"sqlite3_db_status")
		->args({"","op","pCur","pHiwtr","resetFlg"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8577:16
	addExtern< int (*)(struct sqlite3_stmt *,int,int) , sqlite3_stmt_status >(*this,lib,"sqlite3_stmt_status",SideEffects::worstDefault,"sqlite3_stmt_status")
		->args({"","op","resetFlg"});
}
}

