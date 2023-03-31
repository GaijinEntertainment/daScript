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
void Module_dasSQLITE::initFunctions_1() {
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:186:24
	addExtern< const char * (*)() , sqlite3_libversion >(*this,lib,"sqlite3_libversion",SideEffects::worstDefault,"sqlite3_libversion");
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:187:24
	addExtern< const char * (*)() , sqlite3_sourceid >(*this,lib,"sqlite3_sourceid",SideEffects::worstDefault,"sqlite3_sourceid");
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:188:16
	addExtern< int (*)() , sqlite3_libversion_number >(*this,lib,"sqlite3_libversion_number",SideEffects::worstDefault,"sqlite3_libversion_number");
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:213:16
	addExtern< int (*)(const char *) , sqlite3_compileoption_used >(*this,lib,"sqlite3_compileoption_used",SideEffects::worstDefault,"sqlite3_compileoption_used")
		->args({"zOptName"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:214:24
	addExtern< const char * (*)(int) , sqlite3_compileoption_get >(*this,lib,"sqlite3_compileoption_get",SideEffects::worstDefault,"sqlite3_compileoption_get")
		->args({"N"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:256:16
	addExtern< int (*)() , sqlite3_threadsafe >(*this,lib,"sqlite3_threadsafe",SideEffects::worstDefault,"sqlite3_threadsafe");
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:353:16
	addExtern< int (*)(struct sqlite3 *) , sqlite3_close >(*this,lib,"sqlite3_close",SideEffects::worstDefault,"sqlite3_close")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:354:16
	addExtern< int (*)(struct sqlite3 *) , sqlite3_close_v2 >(*this,lib,"sqlite3_close_v2",SideEffects::worstDefault,"sqlite3_close_v2")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:1640:16
	addExtern< int (*)() , sqlite3_initialize >(*this,lib,"sqlite3_initialize",SideEffects::worstDefault,"sqlite3_initialize");
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:1641:16
	addExtern< int (*)() , sqlite3_shutdown >(*this,lib,"sqlite3_shutdown",SideEffects::worstDefault,"sqlite3_shutdown");
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:1642:16
	addExtern< int (*)() , sqlite3_os_init >(*this,lib,"sqlite3_os_init",SideEffects::worstDefault,"sqlite3_os_init");
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:1643:16
	addExtern< int (*)() , sqlite3_os_end >(*this,lib,"sqlite3_os_end",SideEffects::worstDefault,"sqlite3_os_end");
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:2469:16
	addExtern< int (*)(struct sqlite3 *,int) , sqlite3_extended_result_codes >(*this,lib,"sqlite3_extended_result_codes",SideEffects::worstDefault,"sqlite3_extended_result_codes")
		->args({"","onoff"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:2531:26
	addExtern< sqlite3_int64 (*)(struct sqlite3 *) , sqlite3_last_insert_rowid >(*this,lib,"sqlite3_last_insert_rowid",SideEffects::worstDefault,"sqlite3_last_insert_rowid")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:2541:17
	addExtern< void (*)(struct sqlite3 *,sqlite3_int64) , sqlite3_set_last_insert_rowid >(*this,lib,"sqlite3_set_last_insert_rowid",SideEffects::worstDefault,"sqlite3_set_last_insert_rowid")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:2602:16
	addExtern< int (*)(struct sqlite3 *) , sqlite3_changes >(*this,lib,"sqlite3_changes",SideEffects::worstDefault,"sqlite3_changes")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:2603:26
	addExtern< sqlite3_int64 (*)(struct sqlite3 *) , sqlite3_changes64 >(*this,lib,"sqlite3_changes64",SideEffects::worstDefault,"sqlite3_changes64")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:2644:16
	addExtern< int (*)(struct sqlite3 *) , sqlite3_total_changes >(*this,lib,"sqlite3_total_changes",SideEffects::worstDefault,"sqlite3_total_changes")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:2645:26
	addExtern< sqlite3_int64 (*)(struct sqlite3 *) , sqlite3_total_changes64 >(*this,lib,"sqlite3_total_changes64",SideEffects::worstDefault,"sqlite3_total_changes64")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:2685:17
	addExtern< void (*)(struct sqlite3 *) , sqlite3_interrupt >(*this,lib,"sqlite3_interrupt",SideEffects::worstDefault,"sqlite3_interrupt")
		->args({""});
}
}

