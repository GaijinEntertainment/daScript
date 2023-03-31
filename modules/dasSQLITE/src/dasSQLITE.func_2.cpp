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
void Module_dasSQLITE::initFunctions_2() {
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:2686:16
	addExtern< int (*)(struct sqlite3 *) , sqlite3_is_interrupted >(*this,lib,"sqlite3_is_interrupted",SideEffects::worstDefault,"sqlite3_is_interrupted")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:2721:16
	addExtern< int (*)(const char *) , sqlite3_complete >(*this,lib,"sqlite3_complete",SideEffects::worstDefault,"sqlite3_complete")
		->args({"sql"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:2722:16
	addExtern< int (*)(const void *) , sqlite3_complete16 >(*this,lib,"sqlite3_complete16",SideEffects::worstDefault,"sqlite3_complete16")
		->args({"sql"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:2806:16
	addExtern< int (*)(struct sqlite3 *,int) , sqlite3_busy_timeout >(*this,lib,"sqlite3_busy_timeout",SideEffects::worstDefault,"sqlite3_busy_timeout")
		->args({"","ms"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:2881:16
	addExtern< int (*)(struct sqlite3 *,const char *,char ***,int *,int *,char **) , sqlite3_get_table >(*this,lib,"sqlite3_get_table",SideEffects::worstDefault,"sqlite3_get_table")
		->args({"db","zSql","pazResult","pnRow","pnColumn","pzErrmsg"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:2889:17
	addExtern< void (*)(char **) , sqlite3_free_table >(*this,lib,"sqlite3_free_table",SideEffects::worstDefault,"sqlite3_free_table")
		->args({"result"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3011:18
	addExtern< void * (*)(int) , sqlite3_malloc >(*this,lib,"sqlite3_malloc",SideEffects::worstDefault,"sqlite3_malloc")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3012:18
	addExtern< void * (*)(sqlite3_uint64) , sqlite3_malloc64 >(*this,lib,"sqlite3_malloc64",SideEffects::worstDefault,"sqlite3_malloc64")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3013:18
	addExtern< void * (*)(void *,int) , sqlite3_realloc >(*this,lib,"sqlite3_realloc",SideEffects::worstDefault,"sqlite3_realloc")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3014:18
	addExtern< void * (*)(void *,sqlite3_uint64) , sqlite3_realloc64 >(*this,lib,"sqlite3_realloc64",SideEffects::worstDefault,"sqlite3_realloc64")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3015:17
	addExtern< void (*)(void *) , sqlite3_free >(*this,lib,"sqlite3_free",SideEffects::worstDefault,"sqlite3_free")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3016:27
	addExtern< sqlite3_uint64 (*)(void *) , sqlite3_msize >(*this,lib,"sqlite3_msize",SideEffects::worstDefault,"sqlite3_msize")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3041:26
	addExtern< sqlite3_int64 (*)() , sqlite3_memory_used >(*this,lib,"sqlite3_memory_used",SideEffects::worstDefault,"sqlite3_memory_used");
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3042:26
	addExtern< sqlite3_int64 (*)(int) , sqlite3_memory_highwater >(*this,lib,"sqlite3_memory_highwater",SideEffects::worstDefault,"sqlite3_memory_highwater")
		->args({"resetFlag"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3065:17
	addExtern< void (*)(int,void *) , sqlite3_randomness >(*this,lib,"sqlite3_randomness",SideEffects::worstDefault,"sqlite3_randomness")
		->args({"N","P"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3681:16
	addExtern< int (*)(const char *,struct sqlite3 **) , sqlite3_open >(*this,lib,"sqlite3_open",SideEffects::worstDefault,"sqlite3_open")
		->args({"filename","ppDb"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3685:16
	addExtern< int (*)(const void *,struct sqlite3 **) , sqlite3_open16 >(*this,lib,"sqlite3_open16",SideEffects::worstDefault,"sqlite3_open16")
		->args({"filename","ppDb"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3689:16
	addExtern< int (*)(const char *,struct sqlite3 **,int,const char *) , sqlite3_open_v2 >(*this,lib,"sqlite3_open_v2",SideEffects::worstDefault,"sqlite3_open_v2")
		->args({"filename","ppDb","flags","zVfs"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3762:24
	addExtern< const char * (*)(const char *,const char *) , sqlite3_uri_parameter >(*this,lib,"sqlite3_uri_parameter",SideEffects::worstDefault,"sqlite3_uri_parameter")
		->args({"z","zParam"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:3763:16
	addExtern< int (*)(const char *,const char *,int) , sqlite3_uri_boolean >(*this,lib,"sqlite3_uri_boolean",SideEffects::worstDefault,"sqlite3_uri_boolean")
		->args({"z","zParam","bDefault"});
}
}

