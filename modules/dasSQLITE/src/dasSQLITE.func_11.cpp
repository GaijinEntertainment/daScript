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
void Module_dasSQLITE::initFunctions_11() {
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9079:28
	addExtern< struct sqlite3_backup * (*)(struct sqlite3 *,const char *,struct sqlite3 *,const char *) , sqlite3_backup_init >(*this,lib,"sqlite3_backup_init",SideEffects::worstDefault,"sqlite3_backup_init")
		->args({"pDest","zDestName","pSource","zSourceName"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9085:16
	addExtern< int (*)(struct sqlite3_backup *,int) , sqlite3_backup_step >(*this,lib,"sqlite3_backup_step",SideEffects::worstDefault,"sqlite3_backup_step")
		->args({"p","nPage"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9086:16
	addExtern< int (*)(struct sqlite3_backup *) , sqlite3_backup_finish >(*this,lib,"sqlite3_backup_finish",SideEffects::worstDefault,"sqlite3_backup_finish")
		->args({"p"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9087:16
	addExtern< int (*)(struct sqlite3_backup *) , sqlite3_backup_remaining >(*this,lib,"sqlite3_backup_remaining",SideEffects::worstDefault,"sqlite3_backup_remaining")
		->args({"p"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9088:16
	addExtern< int (*)(struct sqlite3_backup *) , sqlite3_backup_pagecount >(*this,lib,"sqlite3_backup_pagecount",SideEffects::worstDefault,"sqlite3_backup_pagecount")
		->args({"p"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9220:16
	addExtern< int (*)(const char *,const char *) , sqlite3_stricmp >(*this,lib,"sqlite3_stricmp",SideEffects::worstDefault,"sqlite3_stricmp")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9221:16
	addExtern< int (*)(const char *,const char *,int) , sqlite3_strnicmp >(*this,lib,"sqlite3_strnicmp",SideEffects::worstDefault,"sqlite3_strnicmp")
		->args({"","",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9238:16
	addExtern< int (*)(const char *,const char *) , sqlite3_strglob >(*this,lib,"sqlite3_strglob",SideEffects::worstDefault,"sqlite3_strglob")
		->args({"zGlob","zStr"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9261:16
	addExtern< int (*)(const char *,const char *,unsigned int) , sqlite3_strlike >(*this,lib,"sqlite3_strlike",SideEffects::worstDefault,"sqlite3_strlike")
		->args({"zGlob","zStr","cEsc"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9356:16
	addExtern< int (*)(struct sqlite3 *,int) , sqlite3_wal_autocheckpoint >(*this,lib,"sqlite3_wal_autocheckpoint",SideEffects::worstDefault,"sqlite3_wal_autocheckpoint")
		->args({"db","N"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9378:16
	addExtern< int (*)(struct sqlite3 *,const char *) , sqlite3_wal_checkpoint >(*this,lib,"sqlite3_wal_checkpoint",SideEffects::worstDefault,"sqlite3_wal_checkpoint")
		->args({"db","zDb"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9472:16
	addExtern< int (*)(struct sqlite3 *,const char *,int,int *,int *) , sqlite3_wal_checkpoint_v2 >(*this,lib,"sqlite3_wal_checkpoint_v2",SideEffects::worstDefault,"sqlite3_wal_checkpoint_v2")
		->args({"db","zDb","eMode","pnLog","pnCkpt"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9590:16
	addExtern< int (*)(struct sqlite3 *) , sqlite3_vtab_on_conflict >(*this,lib,"sqlite3_vtab_on_conflict",SideEffects::worstDefault,"sqlite3_vtab_on_conflict")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9616:16
	addExtern< int (*)(struct sqlite3_context *) , sqlite3_vtab_nochange >(*this,lib,"sqlite3_vtab_nochange",SideEffects::worstDefault,"sqlite3_vtab_nochange")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9651:24
	addExtern< const char * (*)(struct sqlite3_index_info *,int) , sqlite3_vtab_collation >(*this,lib,"sqlite3_vtab_collation",SideEffects::worstDefault,"sqlite3_vtab_collation")
		->args({"",""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9724:16
	addExtern< int (*)(struct sqlite3_index_info *) , sqlite3_vtab_distinct >(*this,lib,"sqlite3_vtab_distinct",SideEffects::worstDefault,"sqlite3_vtab_distinct")
		->args({""});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9797:16
	addExtern< int (*)(struct sqlite3_index_info *,int,int) , sqlite3_vtab_in >(*this,lib,"sqlite3_vtab_in",SideEffects::worstDefault,"sqlite3_vtab_in")
		->args({"","iCons","bHandle"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9844:16
	addExtern< int (*)(struct sqlite3_value *,struct sqlite3_value **) , sqlite3_vtab_in_first >(*this,lib,"sqlite3_vtab_in_first",SideEffects::worstDefault,"sqlite3_vtab_in_first")
		->args({"pVal","ppOut"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9845:16
	addExtern< int (*)(struct sqlite3_value *,struct sqlite3_value **) , sqlite3_vtab_in_next >(*this,lib,"sqlite3_vtab_in_next",SideEffects::worstDefault,"sqlite3_vtab_in_next")
		->args({"pVal","ppOut"});
// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:9887:16
	addExtern< int (*)(struct sqlite3_index_info *,int,struct sqlite3_value **) , sqlite3_vtab_rhs_value >(*this,lib,"sqlite3_vtab_rhs_value",SideEffects::worstDefault,"sqlite3_vtab_rhs_value")
		->args({"","","ppVal"});
}
}

