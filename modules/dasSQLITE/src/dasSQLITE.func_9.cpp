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
void Module_dasSQLITE::initFunctions_9() {
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7046:17
	addExtern< void (*)() , sqlite3_reset_auto_extension >(*this,lib,"sqlite3_reset_auto_extension",SideEffects::worstDefault,"sqlite3_reset_auto_extension");
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7341:16
	addExtern< int (*)(struct sqlite3 *,const char *,const struct sqlite3_module *,void *) , sqlite3_create_module >(*this,lib,"sqlite3_create_module",SideEffects::worstDefault,"sqlite3_create_module")
		->args({"db","zName","p","pClientData"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7367:16
	addExtern< int (*)(struct sqlite3 *,const char **) , sqlite3_drop_modules >(*this,lib,"sqlite3_drop_modules",SideEffects::worstDefault,"sqlite3_drop_modules")
		->args({"db","azKeep"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7427:16
	addExtern< int (*)(struct sqlite3 *,const char *) , sqlite3_declare_vtab >(*this,lib,"sqlite3_declare_vtab",SideEffects::worstDefault,"sqlite3_declare_vtab")
		->args({"","zSQL"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7446:16
	addExtern< int (*)(struct sqlite3 *,const char *,int) , sqlite3_overload_function >(*this,lib,"sqlite3_overload_function",SideEffects::worstDefault,"sqlite3_overload_function")
		->args({"","zFuncName","nArg"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7545:16
	addExtern< int (*)(struct sqlite3 *,const char *,const char *,const char *,sqlite3_int64,int,struct sqlite3_blob **) , sqlite3_blob_open >(*this,lib,"sqlite3_blob_open",SideEffects::worstDefault,"sqlite3_blob_open")
		->args({"","zDb","zTable","zColumn","iRow","flags","ppBlob"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7578:16
	addExtern< int (*)(struct sqlite3_blob *,sqlite3_int64) , sqlite3_blob_reopen >(*this,lib,"sqlite3_blob_reopen",SideEffects::worstDefault,"sqlite3_blob_reopen")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7601:16
	addExtern< int (*)(struct sqlite3_blob *) , sqlite3_blob_close >(*this,lib,"sqlite3_blob_close",SideEffects::worstDefault,"sqlite3_blob_close")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7617:16
	addExtern< int (*)(struct sqlite3_blob *) , sqlite3_blob_bytes >(*this,lib,"sqlite3_blob_bytes",SideEffects::worstDefault,"sqlite3_blob_bytes")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7646:16
	addExtern< int (*)(struct sqlite3_blob *,void *,int,int) , sqlite3_blob_read >(*this,lib,"sqlite3_blob_read",SideEffects::worstDefault,"sqlite3_blob_read")
		->args({"","Z","N","iOffset"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7688:16
	addExtern< int (*)(struct sqlite3_blob *,const void *,int,int) , sqlite3_blob_write >(*this,lib,"sqlite3_blob_write",SideEffects::worstDefault,"sqlite3_blob_write")
		->args({"","z","n","iOffset"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7719:25
	addExtern< struct sqlite3_vfs * (*)(const char *) , sqlite3_vfs_find >(*this,lib,"sqlite3_vfs_find",SideEffects::worstDefault,"sqlite3_vfs_find")
		->args({"zVfsName"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7720:16
	addExtern< int (*)(struct sqlite3_vfs *,int) , sqlite3_vfs_register >(*this,lib,"sqlite3_vfs_register",SideEffects::worstDefault,"sqlite3_vfs_register")
		->args({"","makeDflt"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7721:16
	addExtern< int (*)(struct sqlite3_vfs *) , sqlite3_vfs_unregister >(*this,lib,"sqlite3_vfs_unregister",SideEffects::worstDefault,"sqlite3_vfs_unregister")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7837:27
	addExtern< struct sqlite3_mutex * (*)(int) , sqlite3_mutex_alloc >(*this,lib,"sqlite3_mutex_alloc",SideEffects::worstDefault,"sqlite3_mutex_alloc")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7838:17
	addExtern< void (*)(struct sqlite3_mutex *) , sqlite3_mutex_free >(*this,lib,"sqlite3_mutex_free",SideEffects::worstDefault,"sqlite3_mutex_free")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7839:17
	addExtern< void (*)(struct sqlite3_mutex *) , sqlite3_mutex_enter >(*this,lib,"sqlite3_mutex_enter",SideEffects::worstDefault,"sqlite3_mutex_enter")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7840:16
	addExtern< int (*)(struct sqlite3_mutex *) , sqlite3_mutex_try >(*this,lib,"sqlite3_mutex_try",SideEffects::worstDefault,"sqlite3_mutex_try")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7841:17
	addExtern< void (*)(struct sqlite3_mutex *) , sqlite3_mutex_leave >(*this,lib,"sqlite3_mutex_leave",SideEffects::worstDefault,"sqlite3_mutex_leave")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7996:27
	addExtern< struct sqlite3_mutex * (*)(struct sqlite3 *) , sqlite3_db_mutex >(*this,lib,"sqlite3_db_mutex",SideEffects::worstDefault,"sqlite3_db_mutex")
		->args({""});
}
}

