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
void Module_dasSQLITE::initFunctions_6() {
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5194:16
	addExtern< int (*)(struct sqlite3_stmt *) , sqlite3_finalize >(*this,lib,"sqlite3_finalize",SideEffects::worstDefault,"sqlite3_finalize")
		->args({"pStmt"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5221:16
	addExtern< int (*)(struct sqlite3_stmt *) , sqlite3_reset >(*this,lib,"sqlite3_reset",SideEffects::worstDefault,"sqlite3_reset")
		->args({"pStmt"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5495:34
	addExtern< int (*)(struct sqlite3_context *) , sqlite3_aggregate_count >(*this,lib,"sqlite3_aggregate_count",SideEffects::worstDefault,"sqlite3_aggregate_count")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5496:34
	addExtern< int (*)(struct sqlite3_stmt *) , sqlite3_expired >(*this,lib,"sqlite3_expired",SideEffects::worstDefault,"sqlite3_expired")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5497:34
	addExtern< int (*)(struct sqlite3_stmt *,struct sqlite3_stmt *) , sqlite3_transfer_bindings >(*this,lib,"sqlite3_transfer_bindings",SideEffects::worstDefault,"sqlite3_transfer_bindings")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5498:34
	addExtern< int (*)() , sqlite3_global_recover >(*this,lib,"sqlite3_global_recover",SideEffects::worstDefault,"sqlite3_global_recover");
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5499:35
	addExtern< void (*)() , sqlite3_thread_cleanup >(*this,lib,"sqlite3_thread_cleanup",SideEffects::worstDefault,"sqlite3_thread_cleanup");
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5632:24
	addExtern< const void * (*)(struct sqlite3_value *) , sqlite3_value_blob >(*this,lib,"sqlite3_value_blob",SideEffects::worstDefault,"sqlite3_value_blob")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5633:19
	addExtern< double (*)(struct sqlite3_value *) , sqlite3_value_double >(*this,lib,"sqlite3_value_double",SideEffects::worstDefault,"sqlite3_value_double")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5634:16
	addExtern< int (*)(struct sqlite3_value *) , sqlite3_value_int >(*this,lib,"sqlite3_value_int",SideEffects::worstDefault,"sqlite3_value_int")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5635:26
	addExtern< sqlite3_int64 (*)(struct sqlite3_value *) , sqlite3_value_int64 >(*this,lib,"sqlite3_value_int64",SideEffects::worstDefault,"sqlite3_value_int64")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5636:18
	addExtern< void * (*)(struct sqlite3_value *,const char *) , sqlite3_value_pointer >(*this,lib,"sqlite3_value_pointer",SideEffects::worstDefault,"sqlite3_value_pointer")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5637:33
	addExtern< const unsigned char * (*)(struct sqlite3_value *) , sqlite3_value_text >(*this,lib,"sqlite3_value_text",SideEffects::worstDefault,"sqlite3_value_text")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5638:24
	addExtern< const void * (*)(struct sqlite3_value *) , sqlite3_value_text16 >(*this,lib,"sqlite3_value_text16",SideEffects::worstDefault,"sqlite3_value_text16")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5639:24
	addExtern< const void * (*)(struct sqlite3_value *) , sqlite3_value_text16le >(*this,lib,"sqlite3_value_text16le",SideEffects::worstDefault,"sqlite3_value_text16le")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5640:24
	addExtern< const void * (*)(struct sqlite3_value *) , sqlite3_value_text16be >(*this,lib,"sqlite3_value_text16be",SideEffects::worstDefault,"sqlite3_value_text16be")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5641:16
	addExtern< int (*)(struct sqlite3_value *) , sqlite3_value_bytes >(*this,lib,"sqlite3_value_bytes",SideEffects::worstDefault,"sqlite3_value_bytes")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5642:16
	addExtern< int (*)(struct sqlite3_value *) , sqlite3_value_bytes16 >(*this,lib,"sqlite3_value_bytes16",SideEffects::worstDefault,"sqlite3_value_bytes16")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5643:16
	addExtern< int (*)(struct sqlite3_value *) , sqlite3_value_type >(*this,lib,"sqlite3_value_type",SideEffects::worstDefault,"sqlite3_value_type")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:5644:16
	addExtern< int (*)(struct sqlite3_value *) , sqlite3_value_numeric_type >(*this,lib,"sqlite3_value_numeric_type",SideEffects::worstDefault,"sqlite3_value_numeric_type")
		->args({""});
}
}

