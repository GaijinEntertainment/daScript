// this file is generated via daScript automatic C++ binder
// all user modifications will be lost after this file is re-generated

// from C:\Users\andrei_orazov\libclang\include\clang-c/CXString.h:37:9
struct CXString_GeneratedAnnotation : ManagedStructureAnnotation<CXString> {
	CXString_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXString", ml, "CXString") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(data)>("data","data");
		addField<DAS_BIND_MANAGED_FIELD(private_flags)>("private_flags","private_flags");
	}
};

// from C:\Users\andrei_orazov\libclang\include\clang-c/CXString.h:42:9
struct CXStringSet_GeneratedAnnotation : ManagedStructureAnnotation<CXStringSet> {
	CXStringSet_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXStringSet", ml, "CXStringSet") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Strings)>("Strings","Strings");
		addField<DAS_BIND_MANAGED_FIELD(Count)>("Count","Count");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:102:8
struct CXUnsavedFile_GeneratedAnnotation : ManagedStructureAnnotation<CXUnsavedFile> {
	CXUnsavedFile_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXUnsavedFile", ml, "CXUnsavedFile") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Filename)>("Filename","Filename");
		addField<DAS_BIND_MANAGED_FIELD(Contents)>("Contents","Contents");
		addField<DAS_BIND_MANAGED_FIELD(Length)>("Length","Length");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:150:16
struct CXVersion_GeneratedAnnotation : ManagedStructureAnnotation<CXVersion> {
	CXVersion_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXVersion", ml, "CXVersion") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Major)>("Major","Major");
		addField<DAS_BIND_MANAGED_FIELD(Minor)>("Minor","Minor");
		addField<DAS_BIND_MANAGED_FIELD(Subminor)>("Subminor","Subminor");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:971:16
struct CXFileUniqueID_GeneratedAnnotation : ManagedStructureAnnotation<CXFileUniqueID> {
	CXFileUniqueID_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXFileUniqueID", ml, "CXFileUniqueID") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(data)>("data","data");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:456:9
struct CXSourceLocation_GeneratedAnnotation : ManagedStructureAnnotation<CXSourceLocation> {
	CXSourceLocation_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXSourceLocation", ml, "CXSourceLocation") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(ptr_data)>("ptr_data","ptr_data");
		addField<DAS_BIND_MANAGED_FIELD(int_data)>("int_data","int_data");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:467:9
struct CXSourceRange_GeneratedAnnotation : ManagedStructureAnnotation<CXSourceRange> {
	CXSourceRange_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXSourceRange", ml, "CXSourceRange") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(ptr_data)>("ptr_data","ptr_data");
		addField<DAS_BIND_MANAGED_FIELD(begin_int_data)>("begin_int_data","begin_int_data");
		addField<DAS_BIND_MANAGED_FIELD(end_int_data)>("end_int_data","end_int_data");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:693:9
struct CXSourceRangeList_GeneratedAnnotation : ManagedStructureAnnotation<CXSourceRangeList> {
	CXSourceRangeList_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXSourceRangeList", ml, "CXSourceRangeList") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(count)>("count","count");
		addField<DAS_BIND_MANAGED_FIELD(ranges)>("ranges","ranges");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:1639:16
struct CXTUResourceUsageEntry_GeneratedAnnotation : ManagedStructureAnnotation<CXTUResourceUsageEntry> {
	CXTUResourceUsageEntry_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXTUResourceUsageEntry", ml, "CXTUResourceUsageEntry") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(kind)>("kind","kind");
		addField<DAS_BIND_MANAGED_FIELD(amount)>("amount","amount");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:982:16
struct CXTUResourceUsage_GeneratedAnnotation : ManagedStructureAnnotation<CXTUResourceUsage> {
	CXTUResourceUsage_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXTUResourceUsage", ml, "CXTUResourceUsage") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(data)>("data","data");
		addField<DAS_BIND_MANAGED_FIELD(numEntries)>("numEntries","numEntries");
		addField<DAS_BIND_MANAGED_FIELD(entries)>("entries","entries");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2107:9
struct CXCursor_GeneratedAnnotation : ManagedStructureAnnotation<CXCursor> {
	CXCursor_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXCursor", ml, "CXCursor") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(kind)>("kind","kind");
		addField<DAS_BIND_MANAGED_FIELD(xdata)>("xdata","xdata");
		addField<DAS_BIND_MANAGED_FIELD(data)>("data","data");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2288:16
struct CXPlatformAvailability_GeneratedAnnotation : ManagedStructureAnnotation<CXPlatformAvailability> {
	CXPlatformAvailability_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXPlatformAvailability", ml, "CXPlatformAvailability") {
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

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:2835:9
struct CXType_GeneratedAnnotation : ManagedStructureAnnotation<CXType> {
	CXType_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXType", ml, "CXType") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(kind)>("kind","kind");
		addField<DAS_BIND_MANAGED_FIELD(data)>("data","data");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4506:9
struct CXToken_GeneratedAnnotation : ManagedStructureAnnotation<CXToken> {
	CXToken_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXToken", ml, "CXToken") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(int_data)>("int_data","int_data");
		addField<DAS_BIND_MANAGED_FIELD(ptr_data)>("ptr_data","ptr_data");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4664:9
struct CXCompletionResult_GeneratedAnnotation : ManagedStructureAnnotation<CXCompletionResult> {
	CXCompletionResult_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXCompletionResult", ml, "CXCompletionResult") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(CursorKind)>("CursorKind","CursorKind");
		addField<DAS_BIND_MANAGED_FIELD(CompletionString)>("CompletionString","CompletionString");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:4998:9
struct CXCodeCompleteResults_GeneratedAnnotation : ManagedStructureAnnotation<CXCodeCompleteResults> {
	CXCodeCompleteResults_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXCodeCompleteResults", ml, "CXCodeCompleteResults") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Results)>("Results","Results");
		addField<DAS_BIND_MANAGED_FIELD(NumResults)>("NumResults","NumResults");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5624:16
struct CXCursorAndRangeVisitor_GeneratedAnnotation : ManagedStructureAnnotation<CXCursorAndRangeVisitor> {
	CXCursorAndRangeVisitor_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXCursorAndRangeVisitor", ml, "CXCursorAndRangeVisitor") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(context)>("context","context");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5720:9
struct CXIdxLoc_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxLoc> {
	CXIdxLoc_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxLoc", ml, "CXIdxLoc") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(ptr_data)>("ptr_data","ptr_data");
		addField<DAS_BIND_MANAGED_FIELD(int_data)>("int_data","int_data");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5728:9
struct CXIdxIncludedFileInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxIncludedFileInfo> {
	CXIdxIncludedFileInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxIncludedFileInfo", ml, "CXIdxIncludedFileInfo") {
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

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5753:9
struct CXIdxImportedASTFileInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxImportedASTFileInfo> {
	CXIdxImportedASTFileInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxImportedASTFileInfo", ml, "CXIdxImportedASTFileInfo") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(file)>("file","file");
		addField<DAS_BIND_MANAGED_FIELD(module)>("module","module");
		addField<DAS_BIND_MANAGED_FIELD(loc)>("loc","loc");
		addField<DAS_BIND_MANAGED_FIELD(isImplicit)>("isImplicit","isImplicit");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5842:9
struct CXIdxAttrInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxAttrInfo> {
	CXIdxAttrInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxAttrInfo", ml, "CXIdxAttrInfo") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(kind)>("kind","kind");
		addField<DAS_BIND_MANAGED_FIELD(cursor)>("cursor","cursor");
		addField<DAS_BIND_MANAGED_FIELD(loc)>("loc","loc");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5848:9
struct CXIdxEntityInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxEntityInfo> {
	CXIdxEntityInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxEntityInfo", ml, "CXIdxEntityInfo") {
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

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5859:9
struct CXIdxContainerInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxContainerInfo> {
	CXIdxContainerInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxContainerInfo", ml, "CXIdxContainerInfo") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(cursor)>("cursor","cursor");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5863:9
struct CXIdxIBOutletCollectionAttrInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxIBOutletCollectionAttrInfo> {
	CXIdxIBOutletCollectionAttrInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxIBOutletCollectionAttrInfo", ml, "CXIdxIBOutletCollectionAttrInfo") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(attrInfo)>("attrInfo","attrInfo");
		addField<DAS_BIND_MANAGED_FIELD(objcClass)>("objcClass","objcClass");
		addField<DAS_BIND_MANAGED_FIELD(classCursor)>("classCursor","classCursor");
		addField<DAS_BIND_MANAGED_FIELD(classLoc)>("classLoc","classLoc");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5872:9
struct CXIdxDeclInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxDeclInfo> {
	CXIdxDeclInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxDeclInfo", ml, "CXIdxDeclInfo") {
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

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5904:9
struct CXIdxObjCContainerDeclInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxObjCContainerDeclInfo> {
	CXIdxObjCContainerDeclInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxObjCContainerDeclInfo", ml, "CXIdxObjCContainerDeclInfo") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(declInfo)>("declInfo","declInfo");
		addField<DAS_BIND_MANAGED_FIELD(kind)>("kind","kind");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5909:9
struct CXIdxBaseClassInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxBaseClassInfo> {
	CXIdxBaseClassInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxBaseClassInfo", ml, "CXIdxBaseClassInfo") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(base)>("base","base");
		addField<DAS_BIND_MANAGED_FIELD(cursor)>("cursor","cursor");
		addField<DAS_BIND_MANAGED_FIELD(loc)>("loc","loc");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5915:9
struct CXIdxObjCProtocolRefInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxObjCProtocolRefInfo> {
	CXIdxObjCProtocolRefInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxObjCProtocolRefInfo", ml, "CXIdxObjCProtocolRefInfo") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(protocol)>("protocol","protocol");
		addField<DAS_BIND_MANAGED_FIELD(cursor)>("cursor","cursor");
		addField<DAS_BIND_MANAGED_FIELD(loc)>("loc","loc");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5921:9
struct CXIdxObjCProtocolRefListInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxObjCProtocolRefListInfo> {
	CXIdxObjCProtocolRefListInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxObjCProtocolRefListInfo", ml, "CXIdxObjCProtocolRefListInfo") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(protocols)>("protocols","protocols");
		addField<DAS_BIND_MANAGED_FIELD(numProtocols)>("numProtocols","numProtocols");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5926:9
struct CXIdxObjCInterfaceDeclInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxObjCInterfaceDeclInfo> {
	CXIdxObjCInterfaceDeclInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxObjCInterfaceDeclInfo", ml, "CXIdxObjCInterfaceDeclInfo") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(containerInfo)>("containerInfo","containerInfo");
		addField<DAS_BIND_MANAGED_FIELD(superInfo)>("superInfo","superInfo");
		addField<DAS_BIND_MANAGED_FIELD(protocols)>("protocols","protocols");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5932:9
struct CXIdxObjCCategoryDeclInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxObjCCategoryDeclInfo> {
	CXIdxObjCCategoryDeclInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxObjCCategoryDeclInfo", ml, "CXIdxObjCCategoryDeclInfo") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(containerInfo)>("containerInfo","containerInfo");
		addField<DAS_BIND_MANAGED_FIELD(objcClass)>("objcClass","objcClass");
		addField<DAS_BIND_MANAGED_FIELD(classCursor)>("classCursor","classCursor");
		addField<DAS_BIND_MANAGED_FIELD(classLoc)>("classLoc","classLoc");
		addField<DAS_BIND_MANAGED_FIELD(protocols)>("protocols","protocols");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5940:9
struct CXIdxObjCPropertyDeclInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxObjCPropertyDeclInfo> {
	CXIdxObjCPropertyDeclInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxObjCPropertyDeclInfo", ml, "CXIdxObjCPropertyDeclInfo") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(declInfo)>("declInfo","declInfo");
		addField<DAS_BIND_MANAGED_FIELD(getter)>("getter","getter");
		addField<DAS_BIND_MANAGED_FIELD(setter)>("setter","setter");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5946:9
struct CXIdxCXXClassDeclInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxCXXClassDeclInfo> {
	CXIdxCXXClassDeclInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxCXXClassDeclInfo", ml, "CXIdxCXXClassDeclInfo") {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(declInfo)>("declInfo","declInfo");
		addField<DAS_BIND_MANAGED_FIELD(bases)>("bases","bases");
		addField<DAS_BIND_MANAGED_FIELD(numBases)>("numBases","numBases");
	}
};

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:5992:9
struct CXIdxEntityRefInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxEntityRefInfo> {
	CXIdxEntityRefInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxEntityRefInfo", ml, "CXIdxEntityRefInfo") {
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

// from D:\Work\daScript/../libclang/include/clang-c/Index.h:6029:9
struct IndexerCallbacks_GeneratedAnnotation : ManagedStructureAnnotation<IndexerCallbacks> {
	IndexerCallbacks_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("IndexerCallbacks", ml, "IndexerCallbacks") {
	}
	void init () {
	}
};

