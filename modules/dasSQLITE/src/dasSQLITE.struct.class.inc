// this file is generated via daScript automatic C++ binder
// all user modifications will be lost after this file is re-generated

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:728:8
struct sqlite3_file_GeneratedAnnotation : ManagedStructureAnnotation<sqlite3_file> {
	sqlite3_file_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("sqlite3_file", ml, "sqlite3_file") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(pMethods)>("pMethods","pMethods");
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:834:8
struct sqlite3_io_methods_GeneratedAnnotation : ManagedStructureAnnotation<sqlite3_io_methods> {
	sqlite3_io_methods_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("sqlite3_io_methods", ml, "sqlite3_io_methods") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(iVersion)>("iVersion","iVersion");
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:1464:8
struct sqlite3_vfs_GeneratedAnnotation : ManagedStructureAnnotation<sqlite3_vfs> {
	sqlite3_vfs_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("sqlite3_vfs", ml, "sqlite3_vfs") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(iVersion)>("iVersion","iVersion");
		addField<DAS_BIND_MANAGED_FIELD(szOsFile)>("szOsFile","szOsFile");
		addField<DAS_BIND_MANAGED_FIELD(mxPathname)>("mxPathname","mxPathname");
		addField<DAS_BIND_MANAGED_FIELD(pNext)>("pNext","pNext");
		addField<DAS_BIND_MANAGED_FIELD(zName)>("zName","zName");
		addField<DAS_BIND_MANAGED_FIELD(pAppData)>("pAppData","pAppData");
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:1761:8
struct sqlite3_mem_methods_GeneratedAnnotation : ManagedStructureAnnotation<sqlite3_mem_methods> {
	sqlite3_mem_methods_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("sqlite3_mem_methods", ml, "sqlite3_mem_methods") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(pAppData)>("pAppData","pAppData");
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7072:8
struct sqlite3_module_GeneratedAnnotation : ManagedStructureAnnotation<sqlite3_module> {
	sqlite3_module_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("sqlite3_module", ml, "sqlite3_module") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(iVersion)>("iVersion","iVersion");
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7215:10
struct sqlite3_index_info_sqlite3_index_constraint_GeneratedAnnotation : ManagedStructureAnnotation<sqlite3_index_info::sqlite3_index_constraint> {
	sqlite3_index_info_sqlite3_index_constraint_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("sqlite3_index_constraint", ml, "sqlite3_index_info::sqlite3_index_constraint") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(iColumn)>("iColumn","iColumn");
		addField<DAS_BIND_MANAGED_FIELD(op)>("op","op");
		addField<DAS_BIND_MANAGED_FIELD(usable)>("usable","usable");
		addField<DAS_BIND_MANAGED_FIELD(iTermOffset)>("iTermOffset","iTermOffset");
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7222:10
struct sqlite3_index_info_sqlite3_index_orderby_GeneratedAnnotation : ManagedStructureAnnotation<sqlite3_index_info::sqlite3_index_orderby> {
	sqlite3_index_info_sqlite3_index_orderby_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("sqlite3_index_orderby", ml, "sqlite3_index_info::sqlite3_index_orderby") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(iColumn)>("iColumn","iColumn");
		addField<DAS_BIND_MANAGED_FIELD(desc)>("desc","desc");
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7227:10
struct sqlite3_index_info_sqlite3_index_constraint_usage_GeneratedAnnotation : ManagedStructureAnnotation<sqlite3_index_info::sqlite3_index_constraint_usage> {
	sqlite3_index_info_sqlite3_index_constraint_usage_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("sqlite3_index_constraint_usage", ml, "sqlite3_index_info::sqlite3_index_constraint_usage") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(argvIndex)>("argvIndex","argvIndex");
		addField<DAS_BIND_MANAGED_FIELD(omit)>("omit","omit");
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7212:8
struct sqlite3_index_info_GeneratedAnnotation : ManagedStructureAnnotation<sqlite3_index_info> {
	sqlite3_index_info_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("sqlite3_index_info", ml, "sqlite3_index_info") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(nConstraint)>("nConstraint","nConstraint");
		addField<DAS_BIND_MANAGED_FIELD(aConstraint)>("aConstraint","aConstraint");
		addField<DAS_BIND_MANAGED_FIELD(nOrderBy)>("nOrderBy","nOrderBy");
		addField<DAS_BIND_MANAGED_FIELD(aOrderBy)>("aOrderBy","aOrderBy");
		addField<DAS_BIND_MANAGED_FIELD(aConstraintUsage)>("aConstraintUsage","aConstraintUsage");
		addField<DAS_BIND_MANAGED_FIELD(idxNum)>("idxNum","idxNum");
		addField<DAS_BIND_MANAGED_FIELD(idxStr)>("idxStr","idxStr");
		addField<DAS_BIND_MANAGED_FIELD(needToFreeIdxStr)>("needToFreeIdxStr","needToFreeIdxStr");
		addField<DAS_BIND_MANAGED_FIELD(orderByConsumed)>("orderByConsumed","orderByConsumed");
		addField<DAS_BIND_MANAGED_FIELD(estimatedCost)>("estimatedCost","estimatedCost");
		addField<DAS_BIND_MANAGED_FIELD(estimatedRows)>("estimatedRows","estimatedRows");
		addField<DAS_BIND_MANAGED_FIELD(idxFlags)>("idxFlags","idxFlags");
		addField<DAS_BIND_MANAGED_FIELD(colUsed)>("colUsed","colUsed");
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7390:8
struct sqlite3_vtab_GeneratedAnnotation : ManagedStructureAnnotation<sqlite3_vtab> {
	sqlite3_vtab_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("sqlite3_vtab", ml, "sqlite3_vtab") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(pModule)>("pModule","pModule");
		addField<DAS_BIND_MANAGED_FIELD(nRef)>("nRef","nRef");
		addField<DAS_BIND_MANAGED_FIELD(zErrMsg)>("zErrMsg","zErrMsg");
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7414:8
struct sqlite3_vtab_cursor_GeneratedAnnotation : ManagedStructureAnnotation<sqlite3_vtab_cursor> {
	sqlite3_vtab_cursor_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("sqlite3_vtab_cursor", ml, "sqlite3_vtab_cursor") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(pVtab)>("pVtab","pVtab");
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:7909:8
struct sqlite3_mutex_methods_GeneratedAnnotation : ManagedStructureAnnotation<sqlite3_mutex_methods> {
	sqlite3_mutex_methods_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("sqlite3_mutex_methods", ml, "sqlite3_mutex_methods") {
	}
	void init () {
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8678:8
struct sqlite3_pcache_page_GeneratedAnnotation : ManagedStructureAnnotation<sqlite3_pcache_page> {
	sqlite3_pcache_page_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("sqlite3_pcache_page", ml, "sqlite3_pcache_page") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(pBuf)>("pBuf","pBuf");
		addField<DAS_BIND_MANAGED_FIELD(pExtra)>("pExtra","pExtra");
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8843:8
struct sqlite3_pcache_methods2_GeneratedAnnotation : ManagedStructureAnnotation<sqlite3_pcache_methods2> {
	sqlite3_pcache_methods2_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("sqlite3_pcache_methods2", ml, "sqlite3_pcache_methods2") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(iVersion)>("iVersion","iVersion");
		addField<DAS_BIND_MANAGED_FIELD(pArg)>("pArg","pArg");
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:8866:8
struct sqlite3_pcache_methods_GeneratedAnnotation : ManagedStructureAnnotation<sqlite3_pcache_methods> {
	sqlite3_pcache_methods_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("sqlite3_pcache_methods", ml, "sqlite3_pcache_methods") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(pArg)>("pArg","pArg");
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:10230:16
struct sqlite3_snapshot_GeneratedAnnotation : ManagedStructureAnnotation<sqlite3_snapshot> {
	sqlite3_snapshot_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("sqlite3_snapshot", ml, "sqlite3_snapshot") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(hidden)>("hidden","hidden");
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:10604:8
struct sqlite3_rtree_geometry_GeneratedAnnotation : ManagedStructureAnnotation<sqlite3_rtree_geometry> {
	sqlite3_rtree_geometry_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("sqlite3_rtree_geometry", ml, "sqlite3_rtree_geometry") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(pContext)>("pContext","pContext");
		addField<DAS_BIND_MANAGED_FIELD(nParam)>("nParam","nParam");
		addField<DAS_BIND_MANAGED_FIELD(aParam)>("aParam","aParam");
		addField<DAS_BIND_MANAGED_FIELD(pUser)>("pUser","pUser");
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:10636:8
struct sqlite3_rtree_query_info_GeneratedAnnotation : ManagedStructureAnnotation<sqlite3_rtree_query_info> {
	sqlite3_rtree_query_info_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("sqlite3_rtree_query_info", ml, "sqlite3_rtree_query_info") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(pContext)>("pContext","pContext");
		addField<DAS_BIND_MANAGED_FIELD(nParam)>("nParam","nParam");
		addField<DAS_BIND_MANAGED_FIELD(aParam)>("aParam","aParam");
		addField<DAS_BIND_MANAGED_FIELD(pUser)>("pUser","pUser");
		addField<DAS_BIND_MANAGED_FIELD(aCoord)>("aCoord","aCoord");
		addField<DAS_BIND_MANAGED_FIELD(anQueue)>("anQueue","anQueue");
		addField<DAS_BIND_MANAGED_FIELD(nCoord)>("nCoord","nCoord");
		addField<DAS_BIND_MANAGED_FIELD(iLevel)>("iLevel","iLevel");
		addField<DAS_BIND_MANAGED_FIELD(mxLevel)>("mxLevel","mxLevel");
		addField<DAS_BIND_MANAGED_FIELD(iRowid)>("iRowid","iRowid");
		addField<DAS_BIND_MANAGED_FIELD(rParentScore)>("rParentScore","rParentScore");
		addField<DAS_BIND_MANAGED_FIELD(eParentWithin)>("eParentWithin","eParentWithin");
		addField<DAS_BIND_MANAGED_FIELD(eWithin)>("eWithin","eWithin");
		addField<DAS_BIND_MANAGED_FIELD(rScore)>("rScore","rScore");
		addField<DAS_BIND_MANAGED_FIELD(apSqlParam)>("apSqlParam","apSqlParam");
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:12442:8
struct Fts5PhraseIter_GeneratedAnnotation : ManagedStructureAnnotation<Fts5PhraseIter> {
	Fts5PhraseIter_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("Fts5PhraseIter", ml, "Fts5PhraseIter") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(a)>("a","a");
		addField<DAS_BIND_MANAGED_FIELD(b)>("b","b");
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:12658:8
struct Fts5ExtensionApi_GeneratedAnnotation : ManagedStructureAnnotation<Fts5ExtensionApi> {
	Fts5ExtensionApi_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("Fts5ExtensionApi", ml, "Fts5ExtensionApi") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(iVersion)>("iVersion","iVersion");
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:12894:8
struct fts5_tokenizer_GeneratedAnnotation : ManagedStructureAnnotation<fts5_tokenizer> {
	fts5_tokenizer_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("fts5_tokenizer", ml, "fts5_tokenizer") {
	}
	void init () {
	}
};

// from D:\Work\daScript\Modules\dasSQLITE\sqlite\sqlite3.h:12930:8
struct fts5_api_GeneratedAnnotation : ManagedStructureAnnotation<fts5_api> {
	fts5_api_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("fts5_api", ml, "fts5_api") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(iVersion)>("iVersion","iVersion");
	}
};

