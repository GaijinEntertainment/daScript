// this file is generated via dasClangBind
// all user modifications will be lost after this file is re-generated

struct CXString_GeneratedAnnotation : ManagedStructureAnnotation<CXString> {
	CXString_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXString", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(data)>("data","data");
		addField<DAS_BIND_MANAGED_FIELD(private_flags)>("private_flags","private_flags");
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

struct CXUnsavedFile_GeneratedAnnotation : ManagedStructureAnnotation<CXUnsavedFile> {
	CXUnsavedFile_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXUnsavedFile", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(Filename)>("Filename","Filename");
		addField<DAS_BIND_MANAGED_FIELD(Contents)>("Contents","Contents");
		addField<DAS_BIND_MANAGED_FIELD(Length)>("Length","Length");
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

struct CXFileUniqueID_GeneratedAnnotation : ManagedStructureAnnotation<CXFileUniqueID> {
	CXFileUniqueID_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXFileUniqueID", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(data)>("data","data");
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

struct CXSourceRange_GeneratedAnnotation : ManagedStructureAnnotation<CXSourceRange> {
	CXSourceRange_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXSourceRange", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(ptr_data)>("ptr_data","ptr_data");
		addField<DAS_BIND_MANAGED_FIELD(begin_int_data)>("begin_int_data","begin_int_data");
		addField<DAS_BIND_MANAGED_FIELD(end_int_data)>("end_int_data","end_int_data");
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

struct CXTUResourceUsageEntry_GeneratedAnnotation : ManagedStructureAnnotation<CXTUResourceUsageEntry> {
	CXTUResourceUsageEntry_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXTUResourceUsageEntry", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(kind)>("kind","kind");
		addField<DAS_BIND_MANAGED_FIELD(amount)>("amount","amount");
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

struct CXCursor_GeneratedAnnotation : ManagedStructureAnnotation<CXCursor> {
	CXCursor_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXCursor", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(kind)>("kind","kind");
		addField<DAS_BIND_MANAGED_FIELD(xdata)>("xdata","xdata");
		addField<DAS_BIND_MANAGED_FIELD(data)>("data","data");
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

struct CXType_GeneratedAnnotation : ManagedStructureAnnotation<CXType> {
	CXType_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXType", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(kind)>("kind","kind");
		addField<DAS_BIND_MANAGED_FIELD(data)>("data","data");
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

struct CXCompletionResult_GeneratedAnnotation : ManagedStructureAnnotation<CXCompletionResult> {
	CXCompletionResult_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXCompletionResult", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(CursorKind)>("CursorKind","CursorKind");
		addField<DAS_BIND_MANAGED_FIELD(CompletionString)>("CompletionString","CompletionString");
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

struct CXCursorAndRangeVisitor_GeneratedAnnotation : ManagedStructureAnnotation<CXCursorAndRangeVisitor> {
	CXCursorAndRangeVisitor_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXCursorAndRangeVisitor", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(context)>("context","context");
		addField<DAS_BIND_MANAGED_FIELD(visit)>("visit","visit");
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

struct CXIdxAttrInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxAttrInfo> {
	CXIdxAttrInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxAttrInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(kind)>("kind","kind");
		addField<DAS_BIND_MANAGED_FIELD(cursor)>("cursor","cursor");
		addField<DAS_BIND_MANAGED_FIELD(loc)>("loc","loc");
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

struct CXIdxContainerInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxContainerInfo> {
	CXIdxContainerInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxContainerInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(cursor)>("cursor","cursor");
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

struct CXIdxObjCContainerDeclInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxObjCContainerDeclInfo> {
	CXIdxObjCContainerDeclInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxObjCContainerDeclInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(declInfo)>("declInfo","declInfo");
		addField<DAS_BIND_MANAGED_FIELD(kind)>("kind","kind");
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

struct CXIdxObjCProtocolRefInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxObjCProtocolRefInfo> {
	CXIdxObjCProtocolRefInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxObjCProtocolRefInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(protocol)>("protocol","protocol");
		addField<DAS_BIND_MANAGED_FIELD(cursor)>("cursor","cursor");
		addField<DAS_BIND_MANAGED_FIELD(loc)>("loc","loc");
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

struct CXIdxObjCInterfaceDeclInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxObjCInterfaceDeclInfo> {
	CXIdxObjCInterfaceDeclInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxObjCInterfaceDeclInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(containerInfo)>("containerInfo","containerInfo");
		addField<DAS_BIND_MANAGED_FIELD(superInfo)>("superInfo","superInfo");
		addField<DAS_BIND_MANAGED_FIELD(protocols)>("protocols","protocols");
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

struct CXIdxObjCPropertyDeclInfo_GeneratedAnnotation : ManagedStructureAnnotation<CXIdxObjCPropertyDeclInfo> {
	CXIdxObjCPropertyDeclInfo_GeneratedAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("CXIdxObjCPropertyDeclInfo", ml) {
	}
	void init () {
		addField<DAS_BIND_MANAGED_FIELD(declInfo)>("declInfo","declInfo");
		addField<DAS_BIND_MANAGED_FIELD(getter)>("getter","getter");
		addField<DAS_BIND_MANAGED_FIELD(setter)>("setter","setter");
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

