struct CXString_GeneratedAnnotation : ManagedStructureAnnotation<CXString> {
	CXString_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXString", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(data)>("data","data");
		addField<DAS_BIND_MANAGED_FIELD(private_flags)>("private_flags","private_flags");
	}
};
template <> struct cast_arg<CXString> {
	static __forceinline CXString to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXString *>::to(res);
	}
};

struct CXStringSet_GeneratedAnnotation : ManagedStructureAnnotation<CXStringSet> {
	CXStringSet_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXStringSet", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Strings)>("Strings","Strings");
		addField<DAS_BIND_MANAGED_FIELD(Count)>("Count","Count");
	}
};
template <> struct cast_arg<CXStringSet> {
	static __forceinline CXStringSet to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXStringSet *>::to(res);
	}
};

struct CXUnsavedFile_GeneratedAnnotation : ManagedStructureAnnotation<CXUnsavedFile> {
	CXUnsavedFile_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXUnsavedFile", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Filename)>("Filename","Filename");
		addField<DAS_BIND_MANAGED_FIELD(Contents)>("Contents","Contents");
		addField<DAS_BIND_MANAGED_FIELD(Length)>("Length","Length");
	}
};
template <> struct cast_arg<CXUnsavedFile> {
	static __forceinline CXUnsavedFile to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXUnsavedFile *>::to(res);
	}
};

struct CXVersion_GeneratedAnnotation : ManagedStructureAnnotation<CXVersion> {
	CXVersion_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXVersion", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Major)>("Major","Major");
		addField<DAS_BIND_MANAGED_FIELD(Minor)>("Minor","Minor");
		addField<DAS_BIND_MANAGED_FIELD(Subminor)>("Subminor","Subminor");
	}
};
template <> struct cast_arg<CXVersion> {
	static __forceinline CXVersion to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXVersion *>::to(res);
	}
};

struct CXFileUniqueID_GeneratedAnnotation : ManagedStructureAnnotation<CXFileUniqueID> {
	CXFileUniqueID_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXFileUniqueID", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(data)>("data","data");
	}
};
template <> struct cast_arg<CXFileUniqueID> {
	static __forceinline CXFileUniqueID to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXFileUniqueID *>::to(res);
	}
};

struct CXSourceLocation_GeneratedAnnotation : ManagedStructureAnnotation<CXSourceLocation> {
	CXSourceLocation_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXSourceLocation", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(ptr_data)>("ptr_data","ptr_data");
		addField<DAS_BIND_MANAGED_FIELD(int_data)>("int_data","int_data");
	}
};
template <> struct cast_arg<CXSourceLocation> {
	static __forceinline CXSourceLocation to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXSourceLocation *>::to(res);
	}
};

struct CXSourceRange_GeneratedAnnotation : ManagedStructureAnnotation<CXSourceRange> {
	CXSourceRange_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXSourceRange", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(ptr_data)>("ptr_data","ptr_data");
		addField<DAS_BIND_MANAGED_FIELD(begin_int_data)>("begin_int_data","begin_int_data");
		addField<DAS_BIND_MANAGED_FIELD(end_int_data)>("end_int_data","end_int_data");
	}
};
template <> struct cast_arg<CXSourceRange> {
	static __forceinline CXSourceRange to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXSourceRange *>::to(res);
	}
};

struct CXSourceRangeList_GeneratedAnnotation : ManagedStructureAnnotation<CXSourceRangeList> {
	CXSourceRangeList_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXSourceRangeList", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(count)>("count","count");
		addField<DAS_BIND_MANAGED_FIELD(ranges)>("ranges","ranges");
	}
};
template <> struct cast_arg<CXSourceRangeList> {
	static __forceinline CXSourceRangeList to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXSourceRangeList *>::to(res);
	}
};

struct CXTUResourceUsageEntry_GeneratedAnnotation : ManagedStructureAnnotation<CXTUResourceUsageEntry> {
	CXTUResourceUsageEntry_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXTUResourceUsageEntry", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(kind)>("kind","kind");
		addField<DAS_BIND_MANAGED_FIELD(amount)>("amount","amount");
	}
};
template <> struct cast_arg<CXTUResourceUsageEntry> {
	static __forceinline CXTUResourceUsageEntry to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXTUResourceUsageEntry *>::to(res);
	}
};

struct CXTUResourceUsage_GeneratedAnnotation : ManagedStructureAnnotation<CXTUResourceUsage> {
	CXTUResourceUsage_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXTUResourceUsage", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(data)>("data","data");
		addField<DAS_BIND_MANAGED_FIELD(numEntries)>("numEntries","numEntries");
		addField<DAS_BIND_MANAGED_FIELD(entries)>("entries","entries");
	}
};
template <> struct cast_arg<CXTUResourceUsage> {
	static __forceinline CXTUResourceUsage to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXTUResourceUsage *>::to(res);
	}
};

struct CXCursor_GeneratedAnnotation : ManagedStructureAnnotation<CXCursor> {
	CXCursor_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXCursor", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(kind)>("kind","kind");
		addField<DAS_BIND_MANAGED_FIELD(xdata)>("xdata","xdata");
		addField<DAS_BIND_MANAGED_FIELD(data)>("data","data");
	}
};
template <> struct cast_arg<CXCursor> {
	static __forceinline CXCursor to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXCursor *>::to(res);
	}
};

struct CXPlatformAvailability_GeneratedAnnotation : ManagedStructureAnnotation<CXPlatformAvailability> {
	CXPlatformAvailability_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXPlatformAvailability", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Platform)>("Platform","Platform");
		addField<DAS_BIND_MANAGED_FIELD(Introduced)>("Introduced","Introduced");
		addField<DAS_BIND_MANAGED_FIELD(Deprecated)>("Deprecated","Deprecated");
		addField<DAS_BIND_MANAGED_FIELD(Obsoleted)>("Obsoleted","Obsoleted");
		addField<DAS_BIND_MANAGED_FIELD(Unavailable)>("Unavailable","Unavailable");
		addField<DAS_BIND_MANAGED_FIELD(Message)>("Message","Message");
	}
};
template <> struct cast_arg<CXPlatformAvailability> {
	static __forceinline CXPlatformAvailability to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXPlatformAvailability *>::to(res);
	}
};

struct CXType_GeneratedAnnotation : ManagedStructureAnnotation<CXType> {
	CXType_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXType", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(kind)>("kind","kind");
		addField<DAS_BIND_MANAGED_FIELD(data)>("data","data");
	}
};
template <> struct cast_arg<CXType> {
	static __forceinline CXType to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXType *>::to(res);
	}
};

struct CXToken_GeneratedAnnotation : ManagedStructureAnnotation<CXToken> {
	CXToken_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXToken", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(int_data)>("int_data","int_data");
		addField<DAS_BIND_MANAGED_FIELD(ptr_data)>("ptr_data","ptr_data");
	}
};
template <> struct cast_arg<CXToken> {
	static __forceinline CXToken to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXToken *>::to(res);
	}
};

struct CXCompletionResult_GeneratedAnnotation : ManagedStructureAnnotation<CXCompletionResult> {
	CXCompletionResult_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXCompletionResult", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(CursorKind)>("CursorKind","CursorKind");
		addField<DAS_BIND_MANAGED_FIELD(CompletionString)>("CompletionString","CompletionString");
	}
};
template <> struct cast_arg<CXCompletionResult> {
	static __forceinline CXCompletionResult to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXCompletionResult *>::to(res);
	}
};

struct CXCodeCompleteResults_GeneratedAnnotation : ManagedStructureAnnotation<CXCodeCompleteResults> {
	CXCodeCompleteResults_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXCodeCompleteResults", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Results)>("Results","Results");
		addField<DAS_BIND_MANAGED_FIELD(NumResults)>("NumResults","NumResults");
	}
};
template <> struct cast_arg<CXCodeCompleteResults> {
	static __forceinline CXCodeCompleteResults to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXCodeCompleteResults *>::to(res);
	}
};

struct CXCursorAndRangeVisitor_GeneratedAnnotation : ManagedStructureAnnotation<CXCursorAndRangeVisitor> {
	CXCursorAndRangeVisitor_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXCursorAndRangeVisitor", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(context)>("context","context");
		addField<DAS_BIND_MANAGED_FIELD(visit)>("visit","visit");
	}
};
template <> struct cast_arg<CXCursorAndRangeVisitor> {
	static __forceinline CXCursorAndRangeVisitor to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXCursorAndRangeVisitor *>::to(res);
	}
};

struct CXIdxLoc_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxLoc> {
	CXIdxLoc_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxLoc", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(ptr_data)>("ptr_data","ptr_data");
		addField<DAS_BIND_MANAGED_FIELD(int_data)>("int_data","int_data");
	}
};
template <> struct cast_arg<CXIdxLoc> {
	static __forceinline CXIdxLoc to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXIdxLoc *>::to(res);
	}
};

struct CXIdxIncludedFileInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxIncludedFileInfo> {
	CXIdxIncludedFileInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxIncludedFileInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(hashLoc)>("hashLoc","hashLoc");
		addField<DAS_BIND_MANAGED_FIELD(filename)>("filename","filename");
		addField<DAS_BIND_MANAGED_FIELD(file)>("file","file");
		addField<DAS_BIND_MANAGED_FIELD(isImport)>("isImport","isImport");
		addField<DAS_BIND_MANAGED_FIELD(isAngled)>("isAngled","isAngled");
		addField<DAS_BIND_MANAGED_FIELD(isModuleImport)>("isModuleImport","isModuleImport");
	}
};
template <> struct cast_arg<CXIdxIncludedFileInfo> {
	static __forceinline CXIdxIncludedFileInfo to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXIdxIncludedFileInfo *>::to(res);
	}
};

struct CXIdxImportedASTFileInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxImportedASTFileInfo> {
	CXIdxImportedASTFileInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxImportedASTFileInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(file)>("file","file");
		addField<DAS_BIND_MANAGED_FIELD(module)>("module","module");
		addField<DAS_BIND_MANAGED_FIELD(loc)>("loc","loc");
		addField<DAS_BIND_MANAGED_FIELD(isImplicit)>("isImplicit","isImplicit");
	}
};
template <> struct cast_arg<CXIdxImportedASTFileInfo> {
	static __forceinline CXIdxImportedASTFileInfo to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXIdxImportedASTFileInfo *>::to(res);
	}
};

struct CXIdxAttrInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxAttrInfo> {
	CXIdxAttrInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxAttrInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(kind)>("kind","kind");
		addField<DAS_BIND_MANAGED_FIELD(cursor)>("cursor","cursor");
		addField<DAS_BIND_MANAGED_FIELD(loc)>("loc","loc");
	}
};
template <> struct cast_arg<CXIdxAttrInfo> {
	static __forceinline CXIdxAttrInfo to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXIdxAttrInfo *>::to(res);
	}
};

struct CXIdxEntityInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxEntityInfo> {
	CXIdxEntityInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxEntityInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(kind)>("kind","kind");
		addField<DAS_BIND_MANAGED_FIELD(templateKind)>("templateKind","templateKind");
		addField<DAS_BIND_MANAGED_FIELD(lang)>("lang","lang");
		addField<DAS_BIND_MANAGED_FIELD(name)>("name","name");
		addField<DAS_BIND_MANAGED_FIELD(USR)>("USR","USR");
		addField<DAS_BIND_MANAGED_FIELD(cursor)>("cursor","cursor");
		addField<DAS_BIND_MANAGED_FIELD(attributes)>("attributes","attributes");
		addField<DAS_BIND_MANAGED_FIELD(numAttributes)>("numAttributes","numAttributes");
	}
};
template <> struct cast_arg<CXIdxEntityInfo> {
	static __forceinline CXIdxEntityInfo to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXIdxEntityInfo *>::to(res);
	}
};

struct CXIdxContainerInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxContainerInfo> {
	CXIdxContainerInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxContainerInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(cursor)>("cursor","cursor");
	}
};
template <> struct cast_arg<CXIdxContainerInfo> {
	static __forceinline CXIdxContainerInfo to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXIdxContainerInfo *>::to(res);
	}
};

struct CXIdxIBOutletCollectionAttrInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxIBOutletCollectionAttrInfo> {
	CXIdxIBOutletCollectionAttrInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxIBOutletCollectionAttrInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(attrInfo)>("attrInfo","attrInfo");
		addField<DAS_BIND_MANAGED_FIELD(objcClass)>("objcClass","objcClass");
		addField<DAS_BIND_MANAGED_FIELD(classCursor)>("classCursor","classCursor");
		addField<DAS_BIND_MANAGED_FIELD(classLoc)>("classLoc","classLoc");
	}
};
template <> struct cast_arg<CXIdxIBOutletCollectionAttrInfo> {
	static __forceinline CXIdxIBOutletCollectionAttrInfo to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXIdxIBOutletCollectionAttrInfo *>::to(res);
	}
};

struct CXIdxDeclInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxDeclInfo> {
	CXIdxDeclInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxDeclInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(entityInfo)>("entityInfo","entityInfo");
		addField<DAS_BIND_MANAGED_FIELD(cursor)>("cursor","cursor");
		addField<DAS_BIND_MANAGED_FIELD(loc)>("loc","loc");
		addField<DAS_BIND_MANAGED_FIELD(semanticContainer)>("semanticContainer","semanticContainer");
		addField<DAS_BIND_MANAGED_FIELD(lexicalContainer)>("lexicalContainer","lexicalContainer");
		addField<DAS_BIND_MANAGED_FIELD(isRedeclaration)>("isRedeclaration","isRedeclaration");
		addField<DAS_BIND_MANAGED_FIELD(isDefinition)>("isDefinition","isDefinition");
		addField<DAS_BIND_MANAGED_FIELD(isContainer)>("isContainer","isContainer");
		addField<DAS_BIND_MANAGED_FIELD(declAsContainer)>("declAsContainer","declAsContainer");
		addField<DAS_BIND_MANAGED_FIELD(isImplicit)>("isImplicit","isImplicit");
		addField<DAS_BIND_MANAGED_FIELD(attributes)>("attributes","attributes");
		addField<DAS_BIND_MANAGED_FIELD(numAttributes)>("numAttributes","numAttributes");
		addField<DAS_BIND_MANAGED_FIELD(flags)>("flags","flags");
	}
};
template <> struct cast_arg<CXIdxDeclInfo> {
	static __forceinline CXIdxDeclInfo to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXIdxDeclInfo *>::to(res);
	}
};

struct CXIdxObjCContainerDeclInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxObjCContainerDeclInfo> {
	CXIdxObjCContainerDeclInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxObjCContainerDeclInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(declInfo)>("declInfo","declInfo");
		addField<DAS_BIND_MANAGED_FIELD(kind)>("kind","kind");
	}
};
template <> struct cast_arg<CXIdxObjCContainerDeclInfo> {
	static __forceinline CXIdxObjCContainerDeclInfo to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXIdxObjCContainerDeclInfo *>::to(res);
	}
};

struct CXIdxBaseClassInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxBaseClassInfo> {
	CXIdxBaseClassInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxBaseClassInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(base)>("base","base");
		addField<DAS_BIND_MANAGED_FIELD(cursor)>("cursor","cursor");
		addField<DAS_BIND_MANAGED_FIELD(loc)>("loc","loc");
	}
};
template <> struct cast_arg<CXIdxBaseClassInfo> {
	static __forceinline CXIdxBaseClassInfo to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXIdxBaseClassInfo *>::to(res);
	}
};

struct CXIdxObjCProtocolRefInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxObjCProtocolRefInfo> {
	CXIdxObjCProtocolRefInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxObjCProtocolRefInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(protocol)>("protocol","protocol");
		addField<DAS_BIND_MANAGED_FIELD(cursor)>("cursor","cursor");
		addField<DAS_BIND_MANAGED_FIELD(loc)>("loc","loc");
	}
};
template <> struct cast_arg<CXIdxObjCProtocolRefInfo> {
	static __forceinline CXIdxObjCProtocolRefInfo to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXIdxObjCProtocolRefInfo *>::to(res);
	}
};

struct CXIdxObjCProtocolRefListInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxObjCProtocolRefListInfo> {
	CXIdxObjCProtocolRefListInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxObjCProtocolRefListInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(protocols)>("protocols","protocols");
		addField<DAS_BIND_MANAGED_FIELD(numProtocols)>("numProtocols","numProtocols");
	}
};
template <> struct cast_arg<CXIdxObjCProtocolRefListInfo> {
	static __forceinline CXIdxObjCProtocolRefListInfo to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXIdxObjCProtocolRefListInfo *>::to(res);
	}
};

struct CXIdxObjCInterfaceDeclInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxObjCInterfaceDeclInfo> {
	CXIdxObjCInterfaceDeclInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxObjCInterfaceDeclInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(containerInfo)>("containerInfo","containerInfo");
		addField<DAS_BIND_MANAGED_FIELD(superInfo)>("superInfo","superInfo");
		addField<DAS_BIND_MANAGED_FIELD(protocols)>("protocols","protocols");
	}
};
template <> struct cast_arg<CXIdxObjCInterfaceDeclInfo> {
	static __forceinline CXIdxObjCInterfaceDeclInfo to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXIdxObjCInterfaceDeclInfo *>::to(res);
	}
};

struct CXIdxObjCCategoryDeclInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxObjCCategoryDeclInfo> {
	CXIdxObjCCategoryDeclInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxObjCCategoryDeclInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(containerInfo)>("containerInfo","containerInfo");
		addField<DAS_BIND_MANAGED_FIELD(objcClass)>("objcClass","objcClass");
		addField<DAS_BIND_MANAGED_FIELD(classCursor)>("classCursor","classCursor");
		addField<DAS_BIND_MANAGED_FIELD(classLoc)>("classLoc","classLoc");
		addField<DAS_BIND_MANAGED_FIELD(protocols)>("protocols","protocols");
	}
};
template <> struct cast_arg<CXIdxObjCCategoryDeclInfo> {
	static __forceinline CXIdxObjCCategoryDeclInfo to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXIdxObjCCategoryDeclInfo *>::to(res);
	}
};

struct CXIdxObjCPropertyDeclInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxObjCPropertyDeclInfo> {
	CXIdxObjCPropertyDeclInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxObjCPropertyDeclInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(declInfo)>("declInfo","declInfo");
		addField<DAS_BIND_MANAGED_FIELD(getter)>("getter","getter");
		addField<DAS_BIND_MANAGED_FIELD(setter)>("setter","setter");
	}
};
template <> struct cast_arg<CXIdxObjCPropertyDeclInfo> {
	static __forceinline CXIdxObjCPropertyDeclInfo to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXIdxObjCPropertyDeclInfo *>::to(res);
	}
};

struct CXIdxCXXClassDeclInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxCXXClassDeclInfo> {
	CXIdxCXXClassDeclInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxCXXClassDeclInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(declInfo)>("declInfo","declInfo");
		addField<DAS_BIND_MANAGED_FIELD(bases)>("bases","bases");
		addField<DAS_BIND_MANAGED_FIELD(numBases)>("numBases","numBases");
	}
};
template <> struct cast_arg<CXIdxCXXClassDeclInfo> {
	static __forceinline CXIdxCXXClassDeclInfo to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXIdxCXXClassDeclInfo *>::to(res);
	}
};

struct CXIdxEntityRefInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxEntityRefInfo> {
	CXIdxEntityRefInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxEntityRefInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(kind)>("kind","kind");
		addField<DAS_BIND_MANAGED_FIELD(cursor)>("cursor","cursor");
		addField<DAS_BIND_MANAGED_FIELD(loc)>("loc","loc");
		addField<DAS_BIND_MANAGED_FIELD(referencedEntity)>("referencedEntity","referencedEntity");
		addField<DAS_BIND_MANAGED_FIELD(parentEntity)>("parentEntity","parentEntity");
		addField<DAS_BIND_MANAGED_FIELD(container)>("container","container");
		addField<DAS_BIND_MANAGED_FIELD(role)>("role","role");
	}
};
template <> struct cast_arg<CXIdxEntityRefInfo> {
	static __forceinline CXIdxEntityRefInfo to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<CXIdxEntityRefInfo *>::to(res);
	}
};

struct IndexerCallbacks_GeneratedAnnotation : ManagedStructureAnnotation<IndexerCallbacks> {
	IndexerCallbacks_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("IndexerCallbacks", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(abortQuery)>("abortQuery","abortQuery");
		addField<DAS_BIND_MANAGED_FIELD(diagnostic)>("diagnostic","diagnostic");
		addField<DAS_BIND_MANAGED_FIELD(enteredMainFile)>("enteredMainFile","enteredMainFile");
		addField<DAS_BIND_MANAGED_FIELD(ppIncludedFile)>("ppIncludedFile","ppIncludedFile");
		addField<DAS_BIND_MANAGED_FIELD(importedASTFile)>("importedASTFile","importedASTFile");
		addField<DAS_BIND_MANAGED_FIELD(startedTranslationUnit)>("startedTranslationUnit","startedTranslationUnit");
		addField<DAS_BIND_MANAGED_FIELD(indexDeclaration)>("indexDeclaration","indexDeclaration");
		addField<DAS_BIND_MANAGED_FIELD(indexEntityReference)>("indexEntityReference","indexEntityReference");
	}
};
template <> struct cast_arg<IndexerCallbacks> {
	static __forceinline IndexerCallbacks to ( Context & ctx, SimNode * node ) {
		vec4f res = node->eval(ctx);
		return * cast<IndexerCallbacks *>::to(res);
	}
};

