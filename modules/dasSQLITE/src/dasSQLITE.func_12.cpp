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
void Module_dasSQLITE::initFunctions_12() {
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:10078:16
	addExtern< int (*)(struct sqlite3 *) , sqlite3_db_cacheflush >(*this,lib,"sqlite3_db_cacheflush",SideEffects::worstDefault,"sqlite3_db_cacheflush")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:10208:16
	addExtern< int (*)(struct sqlite3 *) , sqlite3_system_errno >(*this,lib,"sqlite3_system_errno",SideEffects::worstDefault,"sqlite3_system_errno")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:10277:36
	addExtern< int (*)(struct sqlite3 *,const char *,struct sqlite3_snapshot **) , sqlite3_snapshot_get >(*this,lib,"sqlite3_snapshot_get",SideEffects::worstDefault,"sqlite3_snapshot_get")
		->args({"db","zSchema","ppSnapshot"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:10326:36
	addExtern< int (*)(struct sqlite3 *,const char *,struct sqlite3_snapshot *) , sqlite3_snapshot_open >(*this,lib,"sqlite3_snapshot_open",SideEffects::worstDefault,"sqlite3_snapshot_open")
		->args({"db","zSchema","pSnapshot"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:10343:37
	addExtern< void (*)(struct sqlite3_snapshot *) , sqlite3_snapshot_free >(*this,lib,"sqlite3_snapshot_free",SideEffects::worstDefault,"sqlite3_snapshot_free")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:10370:36
	addExtern< int (*)(struct sqlite3_snapshot *,struct sqlite3_snapshot *) , sqlite3_snapshot_cmp >(*this,lib,"sqlite3_snapshot_cmp",SideEffects::worstDefault,"sqlite3_snapshot_cmp")
		->args({"p1","p2"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:10398:36
	addExtern< int (*)(struct sqlite3 *,const char *) , sqlite3_snapshot_recover >(*this,lib,"sqlite3_snapshot_recover",SideEffects::worstDefault,"sqlite3_snapshot_recover")
		->args({"db","zDb"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:10436:27
	addExtern< unsigned char * (*)(struct sqlite3 *,const char *,long long *,unsigned int) , sqlite3_serialize >(*this,lib,"sqlite3_serialize",SideEffects::worstDefault,"sqlite3_serialize")
		->args({"db","zSchema","piSize","mFlags"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:10492:16
	addExtern< int (*)(struct sqlite3 *,const char *,unsigned char *,sqlite3_int64,sqlite3_int64,unsigned int) , sqlite3_deserialize >(*this,lib,"sqlite3_deserialize",SideEffects::worstDefault,"sqlite3_deserialize")
		->args({"db","zSchema","pData","szDb","szBuf","mFlags"});
}
}

