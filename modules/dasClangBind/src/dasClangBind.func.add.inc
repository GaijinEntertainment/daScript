	addExtern<const char * (*)(CXString),clang_getCString>(*this,lib,"clang_getCString",SideEffects::worstDefault,"clang_getCString")
		->args({"string"});
	addExtern<void (*)(CXString),clang_disposeString>(*this,lib,"clang_disposeString",SideEffects::worstDefault,"clang_disposeString")
		->args({"string"});
	addExtern<void (*)(CXStringSet *),clang_disposeStringSet>(*this,lib,"clang_disposeStringSet",SideEffects::worstDefault,"clang_disposeStringSet")
		->args({"set"});
	addExtern<void * (*)(int,int),clang_createIndex>(*this,lib,"clang_createIndex",SideEffects::worstDefault,"clang_createIndex")
		->args({"excludeDeclarationsFromPCH","displayDiagnostics"});
	addExtern<void (*)(void *),clang_disposeIndex>(*this,lib,"clang_disposeIndex",SideEffects::worstDefault,"clang_disposeIndex")
		->args({"index"});
	addExtern<void (*)(void *,unsigned int),clang_CXIndex_setGlobalOptions>(*this,lib,"clang_CXIndex_setGlobalOptions",SideEffects::worstDefault,"clang_CXIndex_setGlobalOptions")
		->args({"","options"});
	addExtern<unsigned int (*)(void *),clang_CXIndex_getGlobalOptions>(*this,lib,"clang_CXIndex_getGlobalOptions",SideEffects::worstDefault,"clang_CXIndex_getGlobalOptions")
		->args({""});
	addExtern<void (*)(void *,const char *),clang_CXIndex_setInvocationEmissionPathOption>(*this,lib,"clang_CXIndex_setInvocationEmissionPathOption",SideEffects::worstDefault,"clang_CXIndex_setInvocationEmissionPathOption")
		->args({"","Path"});
	addExtern<CXString (*)(void *),clang_getFileName,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getFileName",SideEffects::worstDefault,"clang_getFileName")
		->args({"SFile"});
	addExtern<long long (*)(void *),clang_getFileTime>(*this,lib,"clang_getFileTime",SideEffects::worstDefault,"clang_getFileTime")
		->args({"SFile"});
	addExtern<int (*)(void *,CXFileUniqueID *),clang_getFileUniqueID>(*this,lib,"clang_getFileUniqueID",SideEffects::worstDefault,"clang_getFileUniqueID")
		->args({"file","outID"});
	addExtern<unsigned int (*)(CXTranslationUnitImpl *,void *),clang_isFileMultipleIncludeGuarded>(*this,lib,"clang_isFileMultipleIncludeGuarded",SideEffects::worstDefault,"clang_isFileMultipleIncludeGuarded")
		->args({"tu","file"});
	addExtern<void * (*)(CXTranslationUnitImpl *,const char *),clang_getFile>(*this,lib,"clang_getFile",SideEffects::worstDefault,"clang_getFile")
		->args({"tu","file_name"});
	addExtern<const char * (*)(CXTranslationUnitImpl *,void *,unsigned long long *),clang_getFileContents>(*this,lib,"clang_getFileContents",SideEffects::worstDefault,"clang_getFileContents")
		->args({"tu","file","size"});
	addExtern<int (*)(void *,void *),clang_File_isEqual>(*this,lib,"clang_File_isEqual",SideEffects::worstDefault,"clang_File_isEqual")
		->args({"file1","file2"});
	addExtern<CXString (*)(void *),clang_File_tryGetRealPathName,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_File_tryGetRealPathName",SideEffects::worstDefault,"clang_File_tryGetRealPathName")
		->args({"file"});
	addExtern<CXSourceLocation (*)(),clang_getNullLocation,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getNullLocation",SideEffects::worstDefault,"clang_getNullLocation");
	addExtern<unsigned int (*)(CXSourceLocation,CXSourceLocation),clang_equalLocations>(*this,lib,"clang_equalLocations",SideEffects::worstDefault,"clang_equalLocations")
		->args({"loc1","loc2"});
	addExtern<CXSourceLocation (*)(CXTranslationUnitImpl *,void *,unsigned int,unsigned int),clang_getLocation,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getLocation",SideEffects::worstDefault,"clang_getLocation")
		->args({"tu","file","line","column"});
	addExtern<CXSourceLocation (*)(CXTranslationUnitImpl *,void *,unsigned int),clang_getLocationForOffset,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getLocationForOffset",SideEffects::worstDefault,"clang_getLocationForOffset")
		->args({"tu","file","offset"});
	addExtern<int (*)(CXSourceLocation),clang_Location_isInSystemHeader>(*this,lib,"clang_Location_isInSystemHeader",SideEffects::worstDefault,"clang_Location_isInSystemHeader")
		->args({"location"});
	addExtern<int (*)(CXSourceLocation),clang_Location_isFromMainFile>(*this,lib,"clang_Location_isFromMainFile",SideEffects::worstDefault,"clang_Location_isFromMainFile")
		->args({"location"});
	addExtern<CXSourceRange (*)(),clang_getNullRange,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getNullRange",SideEffects::worstDefault,"clang_getNullRange");
	addExtern<CXSourceRange (*)(CXSourceLocation,CXSourceLocation),clang_getRange,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getRange",SideEffects::worstDefault,"clang_getRange")
		->args({"begin","end"});
	addExtern<unsigned int (*)(CXSourceRange,CXSourceRange),clang_equalRanges>(*this,lib,"clang_equalRanges",SideEffects::worstDefault,"clang_equalRanges")
		->args({"range1","range2"});
	addExtern<int (*)(CXSourceRange),clang_Range_isNull>(*this,lib,"clang_Range_isNull",SideEffects::worstDefault,"clang_Range_isNull")
		->args({"range"});
	addExtern<void (*)(CXSourceLocation,void **,unsigned int *,unsigned int *,unsigned int *),clang_getExpansionLocation>(*this,lib,"clang_getExpansionLocation",SideEffects::worstDefault,"clang_getExpansionLocation")
		->args({"location","file","line","column","offset"});
	addExtern<void (*)(CXSourceLocation,CXString *,unsigned int *,unsigned int *),clang_getPresumedLocation>(*this,lib,"clang_getPresumedLocation",SideEffects::worstDefault,"clang_getPresumedLocation")
		->args({"location","filename","line","column"});
	addExtern<void (*)(CXSourceLocation,void **,unsigned int *,unsigned int *,unsigned int *),clang_getInstantiationLocation>(*this,lib,"clang_getInstantiationLocation",SideEffects::worstDefault,"clang_getInstantiationLocation")
		->args({"location","file","line","column","offset"});
	addExtern<void (*)(CXSourceLocation,void **,unsigned int *,unsigned int *,unsigned int *),clang_getSpellingLocation>(*this,lib,"clang_getSpellingLocation",SideEffects::worstDefault,"clang_getSpellingLocation")
		->args({"location","file","line","column","offset"});
	addExtern<void (*)(CXSourceLocation,void **,unsigned int *,unsigned int *,unsigned int *),clang_getFileLocation>(*this,lib,"clang_getFileLocation",SideEffects::worstDefault,"clang_getFileLocation")
		->args({"location","file","line","column","offset"});
	addExtern<CXSourceLocation (*)(CXSourceRange),clang_getRangeStart,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getRangeStart",SideEffects::worstDefault,"clang_getRangeStart")
		->args({"range"});
	addExtern<CXSourceLocation (*)(CXSourceRange),clang_getRangeEnd,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getRangeEnd",SideEffects::worstDefault,"clang_getRangeEnd")
		->args({"range"});
	addExtern<CXSourceRangeList * (*)(CXTranslationUnitImpl *,void *),clang_getSkippedRanges>(*this,lib,"clang_getSkippedRanges",SideEffects::worstDefault,"clang_getSkippedRanges")
		->args({"tu","file"});
	addExtern<CXSourceRangeList * (*)(CXTranslationUnitImpl *),clang_getAllSkippedRanges>(*this,lib,"clang_getAllSkippedRanges",SideEffects::worstDefault,"clang_getAllSkippedRanges")
		->args({"tu"});
	addExtern<void (*)(CXSourceRangeList *),clang_disposeSourceRangeList>(*this,lib,"clang_disposeSourceRangeList",SideEffects::worstDefault,"clang_disposeSourceRangeList")
		->args({"ranges"});
	addExtern<unsigned int (*)(void *),clang_getNumDiagnosticsInSet>(*this,lib,"clang_getNumDiagnosticsInSet",SideEffects::worstDefault,"clang_getNumDiagnosticsInSet")
		->args({"Diags"});
	addExtern<void * (*)(void *,unsigned int),clang_getDiagnosticInSet>(*this,lib,"clang_getDiagnosticInSet",SideEffects::worstDefault,"clang_getDiagnosticInSet")
		->args({"Diags","Index"});
	addExtern<void * (*)(const char *,CXLoadDiag_Error *,CXString *),clang_loadDiagnostics>(*this,lib,"clang_loadDiagnostics",SideEffects::worstDefault,"clang_loadDiagnostics")
		->args({"file","error","errorString"});
	addExtern<void (*)(void *),clang_disposeDiagnosticSet>(*this,lib,"clang_disposeDiagnosticSet",SideEffects::worstDefault,"clang_disposeDiagnosticSet")
		->args({"Diags"});
	addExtern<void * (*)(void *),clang_getChildDiagnostics>(*this,lib,"clang_getChildDiagnostics",SideEffects::worstDefault,"clang_getChildDiagnostics")
		->args({"D"});
	addExtern<unsigned int (*)(CXTranslationUnitImpl *),clang_getNumDiagnostics>(*this,lib,"clang_getNumDiagnostics",SideEffects::worstDefault,"clang_getNumDiagnostics")
		->args({"Unit"});
	addExtern<void * (*)(CXTranslationUnitImpl *,unsigned int),clang_getDiagnostic>(*this,lib,"clang_getDiagnostic",SideEffects::worstDefault,"clang_getDiagnostic")
		->args({"Unit","Index"});
	addExtern<void * (*)(CXTranslationUnitImpl *),clang_getDiagnosticSetFromTU>(*this,lib,"clang_getDiagnosticSetFromTU",SideEffects::worstDefault,"clang_getDiagnosticSetFromTU")
		->args({"Unit"});
	addExtern<void (*)(void *),clang_disposeDiagnostic>(*this,lib,"clang_disposeDiagnostic",SideEffects::worstDefault,"clang_disposeDiagnostic")
		->args({"Diagnostic"});
	addExtern<CXString (*)(void *,unsigned int),clang_formatDiagnostic,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_formatDiagnostic",SideEffects::worstDefault,"clang_formatDiagnostic")
		->args({"Diagnostic","Options"});
	addExtern<unsigned int (*)(),clang_defaultDiagnosticDisplayOptions>(*this,lib,"clang_defaultDiagnosticDisplayOptions",SideEffects::worstDefault,"clang_defaultDiagnosticDisplayOptions");
	addExtern<CXDiagnosticSeverity (*)(void *),clang_getDiagnosticSeverity>(*this,lib,"clang_getDiagnosticSeverity",SideEffects::worstDefault,"clang_getDiagnosticSeverity")
		->args({""});
	addExtern<CXSourceLocation (*)(void *),clang_getDiagnosticLocation,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticLocation",SideEffects::worstDefault,"clang_getDiagnosticLocation")
		->args({""});
	addExtern<CXString (*)(void *),clang_getDiagnosticSpelling,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticSpelling",SideEffects::worstDefault,"clang_getDiagnosticSpelling")
		->args({""});
	addExtern<CXString (*)(void *,CXString *),clang_getDiagnosticOption,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticOption",SideEffects::worstDefault,"clang_getDiagnosticOption")
		->args({"Diag","Disable"});
	addExtern<unsigned int (*)(void *),clang_getDiagnosticCategory>(*this,lib,"clang_getDiagnosticCategory",SideEffects::worstDefault,"clang_getDiagnosticCategory")
		->args({""});
	addExtern<CXString (*)(unsigned int),clang_getDiagnosticCategoryName,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticCategoryName",SideEffects::worstDefault,"clang_getDiagnosticCategoryName")
		->args({"Category"});
	addExtern<CXString (*)(void *),clang_getDiagnosticCategoryText,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticCategoryText",SideEffects::worstDefault,"clang_getDiagnosticCategoryText")
		->args({""});
	addExtern<unsigned int (*)(void *),clang_getDiagnosticNumRanges>(*this,lib,"clang_getDiagnosticNumRanges",SideEffects::worstDefault,"clang_getDiagnosticNumRanges")
		->args({""});
	addExtern<CXSourceRange (*)(void *,unsigned int),clang_getDiagnosticRange,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticRange",SideEffects::worstDefault,"clang_getDiagnosticRange")
		->args({"Diagnostic","Range"});
	addExtern<unsigned int (*)(void *),clang_getDiagnosticNumFixIts>(*this,lib,"clang_getDiagnosticNumFixIts",SideEffects::worstDefault,"clang_getDiagnosticNumFixIts")
		->args({"Diagnostic"});
	addExtern<CXString (*)(void *,unsigned int,CXSourceRange *),clang_getDiagnosticFixIt,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDiagnosticFixIt",SideEffects::worstDefault,"clang_getDiagnosticFixIt")
		->args({"Diagnostic","FixIt","ReplacementRange"});
	addExtern<CXString (*)(CXTranslationUnitImpl *),clang_getTranslationUnitSpelling,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTranslationUnitSpelling",SideEffects::worstDefault,"clang_getTranslationUnitSpelling")
		->args({"CTUnit"});
	addExtern<CXTranslationUnitImpl * (*)(void *,const char *,int,const char *const *,unsigned int,CXUnsavedFile *),clang_createTranslationUnitFromSourceFile>(*this,lib,"clang_createTranslationUnitFromSourceFile",SideEffects::worstDefault,"clang_createTranslationUnitFromSourceFile")
		->args({"CIdx","source_filename","num_clang_command_line_args","clang_command_line_args","num_unsaved_files","unsaved_files"});
	addExtern<CXTranslationUnitImpl * (*)(void *,const char *),clang_createTranslationUnit>(*this,lib,"clang_createTranslationUnit",SideEffects::worstDefault,"clang_createTranslationUnit")
		->args({"CIdx","ast_filename"});
	addExtern<CXErrorCode (*)(void *,const char *,CXTranslationUnitImpl **),clang_createTranslationUnit2>(*this,lib,"clang_createTranslationUnit2",SideEffects::worstDefault,"clang_createTranslationUnit2")
		->args({"CIdx","ast_filename","out_TU"});
	addExtern<unsigned int (*)(),clang_defaultEditingTranslationUnitOptions>(*this,lib,"clang_defaultEditingTranslationUnitOptions",SideEffects::worstDefault,"clang_defaultEditingTranslationUnitOptions");
	addExtern<CXTranslationUnitImpl * (*)(void *,const char *,const char *const *,int,CXUnsavedFile *,unsigned int,unsigned int),clang_parseTranslationUnit>(*this,lib,"clang_parseTranslationUnit",SideEffects::worstDefault,"clang_parseTranslationUnit")
		->args({"CIdx","source_filename","command_line_args","num_command_line_args","unsaved_files","num_unsaved_files","options"});
	addExtern<CXErrorCode (*)(void *,const char *,const char *const *,int,CXUnsavedFile *,unsigned int,unsigned int,CXTranslationUnitImpl **),clang_parseTranslationUnit2>(*this,lib,"clang_parseTranslationUnit2",SideEffects::worstDefault,"clang_parseTranslationUnit2")
		->args({"CIdx","source_filename","command_line_args","num_command_line_args","unsaved_files","num_unsaved_files","options","out_TU"});
	addExtern<CXErrorCode (*)(void *,const char *,const char *const *,int,CXUnsavedFile *,unsigned int,unsigned int,CXTranslationUnitImpl **),clang_parseTranslationUnit2FullArgv>(*this,lib,"clang_parseTranslationUnit2FullArgv",SideEffects::worstDefault,"clang_parseTranslationUnit2FullArgv")
		->args({"CIdx","source_filename","command_line_args","num_command_line_args","unsaved_files","num_unsaved_files","options","out_TU"});
	addExtern<unsigned int (*)(CXTranslationUnitImpl *),clang_defaultSaveOptions>(*this,lib,"clang_defaultSaveOptions",SideEffects::worstDefault,"clang_defaultSaveOptions")
		->args({"TU"});
	addExtern<int (*)(CXTranslationUnitImpl *,const char *,unsigned int),clang_saveTranslationUnit>(*this,lib,"clang_saveTranslationUnit",SideEffects::worstDefault,"clang_saveTranslationUnit")
		->args({"TU","FileName","options"});
	addExtern<unsigned int (*)(CXTranslationUnitImpl *),clang_suspendTranslationUnit>(*this,lib,"clang_suspendTranslationUnit",SideEffects::worstDefault,"clang_suspendTranslationUnit")
		->args({""});
	addExtern<void (*)(CXTranslationUnitImpl *),clang_disposeTranslationUnit>(*this,lib,"clang_disposeTranslationUnit",SideEffects::worstDefault,"clang_disposeTranslationUnit")
		->args({""});
	addExtern<unsigned int (*)(CXTranslationUnitImpl *),clang_defaultReparseOptions>(*this,lib,"clang_defaultReparseOptions",SideEffects::worstDefault,"clang_defaultReparseOptions")
		->args({"TU"});
	addExtern<int (*)(CXTranslationUnitImpl *,unsigned int,CXUnsavedFile *,unsigned int),clang_reparseTranslationUnit>(*this,lib,"clang_reparseTranslationUnit",SideEffects::worstDefault,"clang_reparseTranslationUnit")
		->args({"TU","num_unsaved_files","unsaved_files","options"});
	addExtern<const char * (*)(CXTUResourceUsageKind),clang_getTUResourceUsageName>(*this,lib,"clang_getTUResourceUsageName",SideEffects::worstDefault,"clang_getTUResourceUsageName")
		->args({"kind"});
	addExtern<CXTUResourceUsage (*)(CXTranslationUnitImpl *),clang_getCXTUResourceUsage,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCXTUResourceUsage",SideEffects::worstDefault,"clang_getCXTUResourceUsage")
		->args({"TU"});
	addExtern<void (*)(CXTUResourceUsage),clang_disposeCXTUResourceUsage>(*this,lib,"clang_disposeCXTUResourceUsage",SideEffects::worstDefault,"clang_disposeCXTUResourceUsage")
		->args({"usage"});
	addExtern<CXTargetInfoImpl * (*)(CXTranslationUnitImpl *),clang_getTranslationUnitTargetInfo>(*this,lib,"clang_getTranslationUnitTargetInfo",SideEffects::worstDefault,"clang_getTranslationUnitTargetInfo")
		->args({"CTUnit"});
	addExtern<void (*)(CXTargetInfoImpl *),clang_TargetInfo_dispose>(*this,lib,"clang_TargetInfo_dispose",SideEffects::worstDefault,"clang_TargetInfo_dispose")
		->args({"Info"});
	addExtern<CXString (*)(CXTargetInfoImpl *),clang_TargetInfo_getTriple,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_TargetInfo_getTriple",SideEffects::worstDefault,"clang_TargetInfo_getTriple")
		->args({"Info"});
	addExtern<int (*)(CXTargetInfoImpl *),clang_TargetInfo_getPointerWidth>(*this,lib,"clang_TargetInfo_getPointerWidth",SideEffects::worstDefault,"clang_TargetInfo_getPointerWidth")
		->args({"Info"});
	addExtern<CXCursor (*)(),clang_getNullCursor,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getNullCursor",SideEffects::worstDefault,"clang_getNullCursor");
	addExtern<CXCursor (*)(CXTranslationUnitImpl *),clang_getTranslationUnitCursor,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTranslationUnitCursor",SideEffects::worstDefault,"clang_getTranslationUnitCursor")
		->args({""});
	addExtern<unsigned int (*)(CXCursor,CXCursor),clang_equalCursors>(*this,lib,"clang_equalCursors",SideEffects::worstDefault,"clang_equalCursors")
		->args({"",""});
	addExtern<int (*)(CXCursor),clang_Cursor_isNull>(*this,lib,"clang_Cursor_isNull",SideEffects::worstDefault,"clang_Cursor_isNull")
		->args({"cursor"});
	addExtern<unsigned int (*)(CXCursor),clang_hashCursor>(*this,lib,"clang_hashCursor",SideEffects::worstDefault,"clang_hashCursor")
		->args({""});
	addExtern<CXCursorKind (*)(CXCursor),clang_getCursorKind>(*this,lib,"clang_getCursorKind",SideEffects::worstDefault,"clang_getCursorKind")
		->args({""});
	addExtern<unsigned int (*)(CXCursorKind),clang_isDeclaration>(*this,lib,"clang_isDeclaration",SideEffects::worstDefault,"clang_isDeclaration")
		->args({""});
	addExtern<unsigned int (*)(CXCursor),clang_isInvalidDeclaration>(*this,lib,"clang_isInvalidDeclaration",SideEffects::worstDefault,"clang_isInvalidDeclaration")
		->args({""});
	addExtern<unsigned int (*)(CXCursorKind),clang_isReference>(*this,lib,"clang_isReference",SideEffects::worstDefault,"clang_isReference")
		->args({""});
	addExtern<unsigned int (*)(CXCursorKind),clang_isExpression>(*this,lib,"clang_isExpression",SideEffects::worstDefault,"clang_isExpression")
		->args({""});
	addExtern<unsigned int (*)(CXCursorKind),clang_isStatement>(*this,lib,"clang_isStatement",SideEffects::worstDefault,"clang_isStatement")
		->args({""});
	addExtern<unsigned int (*)(CXCursorKind),clang_isAttribute>(*this,lib,"clang_isAttribute",SideEffects::worstDefault,"clang_isAttribute")
		->args({""});
	addExtern<unsigned int (*)(CXCursor),clang_Cursor_hasAttrs>(*this,lib,"clang_Cursor_hasAttrs",SideEffects::worstDefault,"clang_Cursor_hasAttrs")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursorKind),clang_isInvalid>(*this,lib,"clang_isInvalid",SideEffects::worstDefault,"clang_isInvalid")
		->args({""});
	addExtern<unsigned int (*)(CXCursorKind),clang_isTranslationUnit>(*this,lib,"clang_isTranslationUnit",SideEffects::worstDefault,"clang_isTranslationUnit")
		->args({""});
	addExtern<unsigned int (*)(CXCursorKind),clang_isPreprocessing>(*this,lib,"clang_isPreprocessing",SideEffects::worstDefault,"clang_isPreprocessing")
		->args({""});
	addExtern<unsigned int (*)(CXCursorKind),clang_isUnexposed>(*this,lib,"clang_isUnexposed",SideEffects::worstDefault,"clang_isUnexposed")
		->args({""});
	addExtern<CXLinkageKind (*)(CXCursor),clang_getCursorLinkage>(*this,lib,"clang_getCursorLinkage",SideEffects::worstDefault,"clang_getCursorLinkage")
		->args({"cursor"});
	addExtern<CXVisibilityKind (*)(CXCursor),clang_getCursorVisibility>(*this,lib,"clang_getCursorVisibility",SideEffects::worstDefault,"clang_getCursorVisibility")
		->args({"cursor"});
	addExtern<CXAvailabilityKind (*)(CXCursor),clang_getCursorAvailability>(*this,lib,"clang_getCursorAvailability",SideEffects::worstDefault,"clang_getCursorAvailability")
		->args({"cursor"});
	addExtern<int (*)(CXCursor,int *,CXString *,int *,CXString *,CXPlatformAvailability *,int),clang_getCursorPlatformAvailability>(*this,lib,"clang_getCursorPlatformAvailability",SideEffects::worstDefault,"clang_getCursorPlatformAvailability")
		->args({"cursor","always_deprecated","deprecated_message","always_unavailable","unavailable_message","availability","availability_size"});
	addExtern<void (*)(CXPlatformAvailability *),clang_disposeCXPlatformAvailability>(*this,lib,"clang_disposeCXPlatformAvailability",SideEffects::worstDefault,"clang_disposeCXPlatformAvailability")
		->args({"availability"});
	addExtern<CXCursor (*)(CXCursor),clang_Cursor_getVarDeclInitializer,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getVarDeclInitializer",SideEffects::worstDefault,"clang_Cursor_getVarDeclInitializer")
		->args({"cursor"});
	addExtern<int (*)(CXCursor),clang_Cursor_hasVarDeclGlobalStorage>(*this,lib,"clang_Cursor_hasVarDeclGlobalStorage",SideEffects::worstDefault,"clang_Cursor_hasVarDeclGlobalStorage")
		->args({"cursor"});
	addExtern<int (*)(CXCursor),clang_Cursor_hasVarDeclExternalStorage>(*this,lib,"clang_Cursor_hasVarDeclExternalStorage",SideEffects::worstDefault,"clang_Cursor_hasVarDeclExternalStorage")
		->args({"cursor"});
	addExtern<CXLanguageKind (*)(CXCursor),clang_getCursorLanguage>(*this,lib,"clang_getCursorLanguage",SideEffects::worstDefault,"clang_getCursorLanguage")
		->args({"cursor"});
	addExtern<CXTLSKind (*)(CXCursor),clang_getCursorTLSKind>(*this,lib,"clang_getCursorTLSKind",SideEffects::worstDefault,"clang_getCursorTLSKind")
		->args({"cursor"});
	addExtern<CXTranslationUnitImpl * (*)(CXCursor),clang_Cursor_getTranslationUnit>(*this,lib,"clang_Cursor_getTranslationUnit",SideEffects::worstDefault,"clang_Cursor_getTranslationUnit")
		->args({""});
	addExtern<CXCursorSetImpl * (*)(),clang_createCXCursorSet>(*this,lib,"clang_createCXCursorSet",SideEffects::worstDefault,"clang_createCXCursorSet");
	addExtern<void (*)(CXCursorSetImpl *),clang_disposeCXCursorSet>(*this,lib,"clang_disposeCXCursorSet",SideEffects::worstDefault,"clang_disposeCXCursorSet")
		->args({"cset"});
	addExtern<unsigned int (*)(CXCursorSetImpl *,CXCursor),clang_CXCursorSet_contains>(*this,lib,"clang_CXCursorSet_contains",SideEffects::worstDefault,"clang_CXCursorSet_contains")
		->args({"cset","cursor"});
	addExtern<unsigned int (*)(CXCursorSetImpl *,CXCursor),clang_CXCursorSet_insert>(*this,lib,"clang_CXCursorSet_insert",SideEffects::worstDefault,"clang_CXCursorSet_insert")
		->args({"cset","cursor"});
	addExtern<CXCursor (*)(CXCursor),clang_getCursorSemanticParent,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorSemanticParent",SideEffects::worstDefault,"clang_getCursorSemanticParent")
		->args({"cursor"});
	addExtern<CXCursor (*)(CXCursor),clang_getCursorLexicalParent,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorLexicalParent",SideEffects::worstDefault,"clang_getCursorLexicalParent")
		->args({"cursor"});
	addExtern<void (*)(CXCursor,CXCursor **,unsigned int *),clang_getOverriddenCursors>(*this,lib,"clang_getOverriddenCursors",SideEffects::worstDefault,"clang_getOverriddenCursors")
		->args({"cursor","overridden","num_overridden"});
	addExtern<void (*)(CXCursor *),clang_disposeOverriddenCursors>(*this,lib,"clang_disposeOverriddenCursors",SideEffects::worstDefault,"clang_disposeOverriddenCursors")
		->args({"overridden"});
	addExtern<void * (*)(CXCursor),clang_getIncludedFile>(*this,lib,"clang_getIncludedFile",SideEffects::worstDefault,"clang_getIncludedFile")
		->args({"cursor"});
	addExtern<CXCursor (*)(CXTranslationUnitImpl *,CXSourceLocation),clang_getCursor,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursor",SideEffects::worstDefault,"clang_getCursor")
		->args({"",""});
	addExtern<CXSourceLocation (*)(CXCursor),clang_getCursorLocation,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorLocation",SideEffects::worstDefault,"clang_getCursorLocation")
		->args({""});
	addExtern<CXSourceRange (*)(CXCursor),clang_getCursorExtent,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorExtent",SideEffects::worstDefault,"clang_getCursorExtent")
		->args({""});
	addExtern<CXType (*)(CXCursor),clang_getCursorType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorType",SideEffects::worstDefault,"clang_getCursorType")
		->args({"C"});
	addExtern<CXString (*)(CXType),clang_getTypeSpelling,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTypeSpelling",SideEffects::worstDefault,"clang_getTypeSpelling")
		->args({"CT"});
	addExtern<CXType (*)(CXCursor),clang_getTypedefDeclUnderlyingType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTypedefDeclUnderlyingType",SideEffects::worstDefault,"clang_getTypedefDeclUnderlyingType")
		->args({"C"});
	addExtern<CXType (*)(CXCursor),clang_getEnumDeclIntegerType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getEnumDeclIntegerType",SideEffects::worstDefault,"clang_getEnumDeclIntegerType")
		->args({"C"});
	addExtern<long long (*)(CXCursor),clang_getEnumConstantDeclValue>(*this,lib,"clang_getEnumConstantDeclValue",SideEffects::worstDefault,"clang_getEnumConstantDeclValue")
		->args({"C"});
	addExtern<unsigned long long (*)(CXCursor),clang_getEnumConstantDeclUnsignedValue>(*this,lib,"clang_getEnumConstantDeclUnsignedValue",SideEffects::worstDefault,"clang_getEnumConstantDeclUnsignedValue")
		->args({"C"});
	addExtern<int (*)(CXCursor),clang_getFieldDeclBitWidth>(*this,lib,"clang_getFieldDeclBitWidth",SideEffects::worstDefault,"clang_getFieldDeclBitWidth")
		->args({"C"});
	addExtern<int (*)(CXCursor),clang_Cursor_getNumArguments>(*this,lib,"clang_Cursor_getNumArguments",SideEffects::worstDefault,"clang_Cursor_getNumArguments")
		->args({"C"});
	addExtern<CXCursor (*)(CXCursor,unsigned int),clang_Cursor_getArgument,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getArgument",SideEffects::worstDefault,"clang_Cursor_getArgument")
		->args({"C","i"});
	addExtern<int (*)(CXCursor),clang_Cursor_getNumTemplateArguments>(*this,lib,"clang_Cursor_getNumTemplateArguments",SideEffects::worstDefault,"clang_Cursor_getNumTemplateArguments")
		->args({"C"});
	addExtern<CXTemplateArgumentKind (*)(CXCursor,unsigned int),clang_Cursor_getTemplateArgumentKind>(*this,lib,"clang_Cursor_getTemplateArgumentKind",SideEffects::worstDefault,"clang_Cursor_getTemplateArgumentKind")
		->args({"C","I"});
	addExtern<CXType (*)(CXCursor,unsigned int),clang_Cursor_getTemplateArgumentType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getTemplateArgumentType",SideEffects::worstDefault,"clang_Cursor_getTemplateArgumentType")
		->args({"C","I"});
	addExtern<long long (*)(CXCursor,unsigned int),clang_Cursor_getTemplateArgumentValue>(*this,lib,"clang_Cursor_getTemplateArgumentValue",SideEffects::worstDefault,"clang_Cursor_getTemplateArgumentValue")
		->args({"C","I"});
	addExtern<unsigned long long (*)(CXCursor,unsigned int),clang_Cursor_getTemplateArgumentUnsignedValue>(*this,lib,"clang_Cursor_getTemplateArgumentUnsignedValue",SideEffects::worstDefault,"clang_Cursor_getTemplateArgumentUnsignedValue")
		->args({"C","I"});
	addExtern<unsigned int (*)(CXType,CXType),clang_equalTypes>(*this,lib,"clang_equalTypes",SideEffects::worstDefault,"clang_equalTypes")
		->args({"A","B"});
	addExtern<CXType (*)(CXType),clang_getCanonicalType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCanonicalType",SideEffects::worstDefault,"clang_getCanonicalType")
		->args({"T"});
	addExtern<unsigned int (*)(CXType),clang_isConstQualifiedType>(*this,lib,"clang_isConstQualifiedType",SideEffects::worstDefault,"clang_isConstQualifiedType")
		->args({"T"});
	addExtern<unsigned int (*)(CXCursor),clang_Cursor_isMacroFunctionLike>(*this,lib,"clang_Cursor_isMacroFunctionLike",SideEffects::worstDefault,"clang_Cursor_isMacroFunctionLike")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_Cursor_isMacroBuiltin>(*this,lib,"clang_Cursor_isMacroBuiltin",SideEffects::worstDefault,"clang_Cursor_isMacroBuiltin")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_Cursor_isFunctionInlined>(*this,lib,"clang_Cursor_isFunctionInlined",SideEffects::worstDefault,"clang_Cursor_isFunctionInlined")
		->args({"C"});
	addExtern<unsigned int (*)(CXType),clang_isVolatileQualifiedType>(*this,lib,"clang_isVolatileQualifiedType",SideEffects::worstDefault,"clang_isVolatileQualifiedType")
		->args({"T"});
	addExtern<unsigned int (*)(CXType),clang_isRestrictQualifiedType>(*this,lib,"clang_isRestrictQualifiedType",SideEffects::worstDefault,"clang_isRestrictQualifiedType")
		->args({"T"});
	addExtern<unsigned int (*)(CXType),clang_getAddressSpace>(*this,lib,"clang_getAddressSpace",SideEffects::worstDefault,"clang_getAddressSpace")
		->args({"T"});
	addExtern<CXString (*)(CXType),clang_getTypedefName,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTypedefName",SideEffects::worstDefault,"clang_getTypedefName")
		->args({"CT"});
	addExtern<CXType (*)(CXType),clang_getPointeeType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getPointeeType",SideEffects::worstDefault,"clang_getPointeeType")
		->args({"T"});
	addExtern<CXCursor (*)(CXType),clang_getTypeDeclaration,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTypeDeclaration",SideEffects::worstDefault,"clang_getTypeDeclaration")
		->args({"T"});
	addExtern<CXString (*)(CXCursor),clang_getDeclObjCTypeEncoding,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getDeclObjCTypeEncoding",SideEffects::worstDefault,"clang_getDeclObjCTypeEncoding")
		->args({"C"});
	addExtern<CXString (*)(CXType),clang_Type_getObjCEncoding,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getObjCEncoding",SideEffects::worstDefault,"clang_Type_getObjCEncoding")
		->args({"type"});
	addExtern<CXString (*)(CXTypeKind),clang_getTypeKindSpelling,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTypeKindSpelling",SideEffects::worstDefault,"clang_getTypeKindSpelling")
		->args({"K"});
	addExtern<CXCallingConv (*)(CXType),clang_getFunctionTypeCallingConv>(*this,lib,"clang_getFunctionTypeCallingConv",SideEffects::worstDefault,"clang_getFunctionTypeCallingConv")
		->args({"T"});
	addExtern<CXType (*)(CXType),clang_getResultType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getResultType",SideEffects::worstDefault,"clang_getResultType")
		->args({"T"});
	addExtern<int (*)(CXType),clang_getExceptionSpecificationType>(*this,lib,"clang_getExceptionSpecificationType",SideEffects::worstDefault,"clang_getExceptionSpecificationType")
		->args({"T"});
	addExtern<int (*)(CXType),clang_getNumArgTypes>(*this,lib,"clang_getNumArgTypes",SideEffects::worstDefault,"clang_getNumArgTypes")
		->args({"T"});
	addExtern<CXType (*)(CXType,unsigned int),clang_getArgType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getArgType",SideEffects::worstDefault,"clang_getArgType")
		->args({"T","i"});
	addExtern<CXType (*)(CXType),clang_Type_getObjCObjectBaseType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getObjCObjectBaseType",SideEffects::worstDefault,"clang_Type_getObjCObjectBaseType")
		->args({"T"});
	addExtern<unsigned int (*)(CXType),clang_Type_getNumObjCProtocolRefs>(*this,lib,"clang_Type_getNumObjCProtocolRefs",SideEffects::worstDefault,"clang_Type_getNumObjCProtocolRefs")
		->args({"T"});
	addExtern<CXCursor (*)(CXType,unsigned int),clang_Type_getObjCProtocolDecl,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getObjCProtocolDecl",SideEffects::worstDefault,"clang_Type_getObjCProtocolDecl")
		->args({"T","i"});
	addExtern<unsigned int (*)(CXType),clang_Type_getNumObjCTypeArgs>(*this,lib,"clang_Type_getNumObjCTypeArgs",SideEffects::worstDefault,"clang_Type_getNumObjCTypeArgs")
		->args({"T"});
	addExtern<CXType (*)(CXType,unsigned int),clang_Type_getObjCTypeArg,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getObjCTypeArg",SideEffects::worstDefault,"clang_Type_getObjCTypeArg")
		->args({"T","i"});
	addExtern<unsigned int (*)(CXType),clang_isFunctionTypeVariadic>(*this,lib,"clang_isFunctionTypeVariadic",SideEffects::worstDefault,"clang_isFunctionTypeVariadic")
		->args({"T"});
	addExtern<CXType (*)(CXCursor),clang_getCursorResultType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorResultType",SideEffects::worstDefault,"clang_getCursorResultType")
		->args({"C"});
	addExtern<int (*)(CXCursor),clang_getCursorExceptionSpecificationType>(*this,lib,"clang_getCursorExceptionSpecificationType",SideEffects::worstDefault,"clang_getCursorExceptionSpecificationType")
		->args({"C"});
	addExtern<unsigned int (*)(CXType),clang_isPODType>(*this,lib,"clang_isPODType",SideEffects::worstDefault,"clang_isPODType")
		->args({"T"});
	addExtern<CXType (*)(CXType),clang_getElementType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getElementType",SideEffects::worstDefault,"clang_getElementType")
		->args({"T"});
	addExtern<long long (*)(CXType),clang_getNumElements>(*this,lib,"clang_getNumElements",SideEffects::worstDefault,"clang_getNumElements")
		->args({"T"});
	addExtern<CXType (*)(CXType),clang_getArrayElementType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getArrayElementType",SideEffects::worstDefault,"clang_getArrayElementType")
		->args({"T"});
	addExtern<long long (*)(CXType),clang_getArraySize>(*this,lib,"clang_getArraySize",SideEffects::worstDefault,"clang_getArraySize")
		->args({"T"});
	addExtern<CXType (*)(CXType),clang_Type_getNamedType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getNamedType",SideEffects::worstDefault,"clang_Type_getNamedType")
		->args({"T"});
	addExtern<unsigned int (*)(CXType),clang_Type_isTransparentTagTypedef>(*this,lib,"clang_Type_isTransparentTagTypedef",SideEffects::worstDefault,"clang_Type_isTransparentTagTypedef")
		->args({"T"});
	addExtern<CXTypeNullabilityKind (*)(CXType),clang_Type_getNullability>(*this,lib,"clang_Type_getNullability",SideEffects::worstDefault,"clang_Type_getNullability")
		->args({"T"});
	addExtern<long long (*)(CXType),clang_Type_getAlignOf>(*this,lib,"clang_Type_getAlignOf",SideEffects::worstDefault,"clang_Type_getAlignOf")
		->args({"T"});
	addExtern<CXType (*)(CXType),clang_Type_getClassType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getClassType",SideEffects::worstDefault,"clang_Type_getClassType")
		->args({"T"});
	addExtern<long long (*)(CXType),clang_Type_getSizeOf>(*this,lib,"clang_Type_getSizeOf",SideEffects::worstDefault,"clang_Type_getSizeOf")
		->args({"T"});
	addExtern<long long (*)(CXType,const char *),clang_Type_getOffsetOf>(*this,lib,"clang_Type_getOffsetOf",SideEffects::worstDefault,"clang_Type_getOffsetOf")
		->args({"T","S"});
	addExtern<CXType (*)(CXType),clang_Type_getModifiedType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getModifiedType",SideEffects::worstDefault,"clang_Type_getModifiedType")
		->args({"T"});
	addExtern<CXType (*)(CXType),clang_Type_getValueType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getValueType",SideEffects::worstDefault,"clang_Type_getValueType")
		->args({"CT"});
	addExtern<long long (*)(CXCursor),clang_Cursor_getOffsetOfField>(*this,lib,"clang_Cursor_getOffsetOfField",SideEffects::worstDefault,"clang_Cursor_getOffsetOfField")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_Cursor_isAnonymous>(*this,lib,"clang_Cursor_isAnonymous",SideEffects::worstDefault,"clang_Cursor_isAnonymous")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_Cursor_isAnonymousRecordDecl>(*this,lib,"clang_Cursor_isAnonymousRecordDecl",SideEffects::worstDefault,"clang_Cursor_isAnonymousRecordDecl")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_Cursor_isInlineNamespace>(*this,lib,"clang_Cursor_isInlineNamespace",SideEffects::worstDefault,"clang_Cursor_isInlineNamespace")
		->args({"C"});
	addExtern<int (*)(CXType),clang_Type_getNumTemplateArguments>(*this,lib,"clang_Type_getNumTemplateArguments",SideEffects::worstDefault,"clang_Type_getNumTemplateArguments")
		->args({"T"});
	addExtern<CXType (*)(CXType,unsigned int),clang_Type_getTemplateArgumentAsType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Type_getTemplateArgumentAsType",SideEffects::worstDefault,"clang_Type_getTemplateArgumentAsType")
		->args({"T","i"});
	addExtern<CXRefQualifierKind (*)(CXType),clang_Type_getCXXRefQualifier>(*this,lib,"clang_Type_getCXXRefQualifier",SideEffects::worstDefault,"clang_Type_getCXXRefQualifier")
		->args({"T"});
	addExtern<unsigned int (*)(CXCursor),clang_Cursor_isBitField>(*this,lib,"clang_Cursor_isBitField",SideEffects::worstDefault,"clang_Cursor_isBitField")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_isVirtualBase>(*this,lib,"clang_isVirtualBase",SideEffects::worstDefault,"clang_isVirtualBase")
		->args({""});
	addExtern<CX_CXXAccessSpecifier (*)(CXCursor),clang_getCXXAccessSpecifier>(*this,lib,"clang_getCXXAccessSpecifier",SideEffects::worstDefault,"clang_getCXXAccessSpecifier")
		->args({""});
	addExtern<CX_StorageClass (*)(CXCursor),clang_Cursor_getStorageClass>(*this,lib,"clang_Cursor_getStorageClass",SideEffects::worstDefault,"clang_Cursor_getStorageClass")
		->args({""});
	addExtern<unsigned int (*)(CXCursor),clang_getNumOverloadedDecls>(*this,lib,"clang_getNumOverloadedDecls",SideEffects::worstDefault,"clang_getNumOverloadedDecls")
		->args({"cursor"});
	addExtern<CXCursor (*)(CXCursor,unsigned int),clang_getOverloadedDecl,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getOverloadedDecl",SideEffects::worstDefault,"clang_getOverloadedDecl")
		->args({"cursor","index"});
	addExtern<CXType (*)(CXCursor),clang_getIBOutletCollectionType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getIBOutletCollectionType",SideEffects::worstDefault,"clang_getIBOutletCollectionType")
		->args({""});
	addExtern<unsigned int (*)(CXCursor,CXChildVisitResult (*)(CXCursor, CXCursor, void *),void *),clang_visitChildren>(*this,lib,"clang_visitChildren",SideEffects::worstDefault,"clang_visitChildren")
		->args({"parent","visitor","client_data"});
	addExtern<CXString (*)(CXCursor),clang_getCursorUSR,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorUSR",SideEffects::worstDefault,"clang_getCursorUSR")
		->args({""});
	addExtern<CXString (*)(const char *),clang_constructUSR_ObjCClass,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_constructUSR_ObjCClass",SideEffects::worstDefault,"clang_constructUSR_ObjCClass")
		->args({"class_name"});
	addExtern<CXString (*)(const char *,const char *),clang_constructUSR_ObjCCategory,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_constructUSR_ObjCCategory",SideEffects::worstDefault,"clang_constructUSR_ObjCCategory")
		->args({"class_name","category_name"});
	addExtern<CXString (*)(const char *),clang_constructUSR_ObjCProtocol,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_constructUSR_ObjCProtocol",SideEffects::worstDefault,"clang_constructUSR_ObjCProtocol")
		->args({"protocol_name"});
	addExtern<CXString (*)(const char *,CXString),clang_constructUSR_ObjCIvar,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_constructUSR_ObjCIvar",SideEffects::worstDefault,"clang_constructUSR_ObjCIvar")
		->args({"name","classUSR"});
	addExtern<CXString (*)(const char *,unsigned int,CXString),clang_constructUSR_ObjCMethod,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_constructUSR_ObjCMethod",SideEffects::worstDefault,"clang_constructUSR_ObjCMethod")
		->args({"name","isInstanceMethod","classUSR"});
	addExtern<CXString (*)(const char *,CXString),clang_constructUSR_ObjCProperty,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_constructUSR_ObjCProperty",SideEffects::worstDefault,"clang_constructUSR_ObjCProperty")
		->args({"property","classUSR"});
	addExtern<CXString (*)(CXCursor),clang_getCursorSpelling,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorSpelling",SideEffects::worstDefault,"clang_getCursorSpelling")
		->args({""});
	addExtern<CXSourceRange (*)(CXCursor,unsigned int,unsigned int),clang_Cursor_getSpellingNameRange,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getSpellingNameRange",SideEffects::worstDefault,"clang_Cursor_getSpellingNameRange")
		->args({"","pieceIndex","options"});
	addExtern<unsigned int (*)(void *,CXPrintingPolicyProperty),clang_PrintingPolicy_getProperty>(*this,lib,"clang_PrintingPolicy_getProperty",SideEffects::worstDefault,"clang_PrintingPolicy_getProperty")
		->args({"Policy","Property"});
	addExtern<void (*)(void *,CXPrintingPolicyProperty,unsigned int),clang_PrintingPolicy_setProperty>(*this,lib,"clang_PrintingPolicy_setProperty",SideEffects::worstDefault,"clang_PrintingPolicy_setProperty")
		->args({"Policy","Property","Value"});
	addExtern<void * (*)(CXCursor),clang_getCursorPrintingPolicy>(*this,lib,"clang_getCursorPrintingPolicy",SideEffects::worstDefault,"clang_getCursorPrintingPolicy")
		->args({""});
	addExtern<void (*)(void *),clang_PrintingPolicy_dispose>(*this,lib,"clang_PrintingPolicy_dispose",SideEffects::worstDefault,"clang_PrintingPolicy_dispose")
		->args({"Policy"});
	addExtern<CXString (*)(CXCursor,void *),clang_getCursorPrettyPrinted,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorPrettyPrinted",SideEffects::worstDefault,"clang_getCursorPrettyPrinted")
		->args({"Cursor","Policy"});
	addExtern<CXString (*)(CXCursor),clang_getCursorDisplayName,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorDisplayName",SideEffects::worstDefault,"clang_getCursorDisplayName")
		->args({""});
	addExtern<CXCursor (*)(CXCursor),clang_getCursorReferenced,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorReferenced",SideEffects::worstDefault,"clang_getCursorReferenced")
		->args({""});
	addExtern<CXCursor (*)(CXCursor),clang_getCursorDefinition,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorDefinition",SideEffects::worstDefault,"clang_getCursorDefinition")
		->args({""});
	addExtern<unsigned int (*)(CXCursor),clang_isCursorDefinition>(*this,lib,"clang_isCursorDefinition",SideEffects::worstDefault,"clang_isCursorDefinition")
		->args({""});
	addExtern<CXCursor (*)(CXCursor),clang_getCanonicalCursor,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCanonicalCursor",SideEffects::worstDefault,"clang_getCanonicalCursor")
		->args({""});
	addExtern<int (*)(CXCursor),clang_Cursor_getObjCSelectorIndex>(*this,lib,"clang_Cursor_getObjCSelectorIndex",SideEffects::worstDefault,"clang_Cursor_getObjCSelectorIndex")
		->args({""});
	addExtern<int (*)(CXCursor),clang_Cursor_isDynamicCall>(*this,lib,"clang_Cursor_isDynamicCall",SideEffects::worstDefault,"clang_Cursor_isDynamicCall")
		->args({"C"});
	addExtern<CXType (*)(CXCursor),clang_Cursor_getReceiverType,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getReceiverType",SideEffects::worstDefault,"clang_Cursor_getReceiverType")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor,unsigned int),clang_Cursor_getObjCPropertyAttributes>(*this,lib,"clang_Cursor_getObjCPropertyAttributes",SideEffects::worstDefault,"clang_Cursor_getObjCPropertyAttributes")
		->args({"C","reserved"});
	addExtern<CXString (*)(CXCursor),clang_Cursor_getObjCPropertyGetterName,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getObjCPropertyGetterName",SideEffects::worstDefault,"clang_Cursor_getObjCPropertyGetterName")
		->args({"C"});
	addExtern<CXString (*)(CXCursor),clang_Cursor_getObjCPropertySetterName,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getObjCPropertySetterName",SideEffects::worstDefault,"clang_Cursor_getObjCPropertySetterName")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_Cursor_getObjCDeclQualifiers>(*this,lib,"clang_Cursor_getObjCDeclQualifiers",SideEffects::worstDefault,"clang_Cursor_getObjCDeclQualifiers")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_Cursor_isObjCOptional>(*this,lib,"clang_Cursor_isObjCOptional",SideEffects::worstDefault,"clang_Cursor_isObjCOptional")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_Cursor_isVariadic>(*this,lib,"clang_Cursor_isVariadic",SideEffects::worstDefault,"clang_Cursor_isVariadic")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor,CXString *,CXString *,unsigned int *),clang_Cursor_isExternalSymbol>(*this,lib,"clang_Cursor_isExternalSymbol",SideEffects::worstDefault,"clang_Cursor_isExternalSymbol")
		->args({"C","language","definedIn","isGenerated"});
	addExtern<CXSourceRange (*)(CXCursor),clang_Cursor_getCommentRange,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getCommentRange",SideEffects::worstDefault,"clang_Cursor_getCommentRange")
		->args({"C"});
	addExtern<CXString (*)(CXCursor),clang_Cursor_getRawCommentText,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getRawCommentText",SideEffects::worstDefault,"clang_Cursor_getRawCommentText")
		->args({"C"});
	addExtern<CXString (*)(CXCursor),clang_Cursor_getBriefCommentText,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getBriefCommentText",SideEffects::worstDefault,"clang_Cursor_getBriefCommentText")
		->args({"C"});
	addExtern<CXString (*)(CXCursor),clang_Cursor_getMangling,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Cursor_getMangling",SideEffects::worstDefault,"clang_Cursor_getMangling")
		->args({""});
	addExtern<CXStringSet * (*)(CXCursor),clang_Cursor_getCXXManglings>(*this,lib,"clang_Cursor_getCXXManglings",SideEffects::worstDefault,"clang_Cursor_getCXXManglings")
		->args({""});
	addExtern<CXStringSet * (*)(CXCursor),clang_Cursor_getObjCManglings>(*this,lib,"clang_Cursor_getObjCManglings",SideEffects::worstDefault,"clang_Cursor_getObjCManglings")
		->args({""});
	addExtern<void * (*)(CXCursor),clang_Cursor_getModule>(*this,lib,"clang_Cursor_getModule",SideEffects::worstDefault,"clang_Cursor_getModule")
		->args({"C"});
	addExtern<void * (*)(CXTranslationUnitImpl *,void *),clang_getModuleForFile>(*this,lib,"clang_getModuleForFile",SideEffects::worstDefault,"clang_getModuleForFile")
		->args({"",""});
	addExtern<void * (*)(void *),clang_Module_getASTFile>(*this,lib,"clang_Module_getASTFile",SideEffects::worstDefault,"clang_Module_getASTFile")
		->args({"Module"});
	addExtern<void * (*)(void *),clang_Module_getParent>(*this,lib,"clang_Module_getParent",SideEffects::worstDefault,"clang_Module_getParent")
		->args({"Module"});
	addExtern<CXString (*)(void *),clang_Module_getName,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Module_getName",SideEffects::worstDefault,"clang_Module_getName")
		->args({"Module"});
	addExtern<CXString (*)(void *),clang_Module_getFullName,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_Module_getFullName",SideEffects::worstDefault,"clang_Module_getFullName")
		->args({"Module"});
	addExtern<int (*)(void *),clang_Module_isSystem>(*this,lib,"clang_Module_isSystem",SideEffects::worstDefault,"clang_Module_isSystem")
		->args({"Module"});
	addExtern<unsigned int (*)(CXTranslationUnitImpl *,void *),clang_Module_getNumTopLevelHeaders>(*this,lib,"clang_Module_getNumTopLevelHeaders",SideEffects::worstDefault,"clang_Module_getNumTopLevelHeaders")
		->args({"","Module"});
	addExtern<void * (*)(CXTranslationUnitImpl *,void *,unsigned int),clang_Module_getTopLevelHeader>(*this,lib,"clang_Module_getTopLevelHeader",SideEffects::worstDefault,"clang_Module_getTopLevelHeader")
		->args({"","Module","Index"});
	addExtern<unsigned int (*)(CXCursor),clang_CXXConstructor_isConvertingConstructor>(*this,lib,"clang_CXXConstructor_isConvertingConstructor",SideEffects::worstDefault,"clang_CXXConstructor_isConvertingConstructor")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_CXXConstructor_isCopyConstructor>(*this,lib,"clang_CXXConstructor_isCopyConstructor",SideEffects::worstDefault,"clang_CXXConstructor_isCopyConstructor")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_CXXConstructor_isDefaultConstructor>(*this,lib,"clang_CXXConstructor_isDefaultConstructor",SideEffects::worstDefault,"clang_CXXConstructor_isDefaultConstructor")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_CXXConstructor_isMoveConstructor>(*this,lib,"clang_CXXConstructor_isMoveConstructor",SideEffects::worstDefault,"clang_CXXConstructor_isMoveConstructor")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_CXXField_isMutable>(*this,lib,"clang_CXXField_isMutable",SideEffects::worstDefault,"clang_CXXField_isMutable")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_CXXMethod_isDefaulted>(*this,lib,"clang_CXXMethod_isDefaulted",SideEffects::worstDefault,"clang_CXXMethod_isDefaulted")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_CXXMethod_isPureVirtual>(*this,lib,"clang_CXXMethod_isPureVirtual",SideEffects::worstDefault,"clang_CXXMethod_isPureVirtual")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_CXXMethod_isStatic>(*this,lib,"clang_CXXMethod_isStatic",SideEffects::worstDefault,"clang_CXXMethod_isStatic")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_CXXMethod_isVirtual>(*this,lib,"clang_CXXMethod_isVirtual",SideEffects::worstDefault,"clang_CXXMethod_isVirtual")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_CXXRecord_isAbstract>(*this,lib,"clang_CXXRecord_isAbstract",SideEffects::worstDefault,"clang_CXXRecord_isAbstract")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_EnumDecl_isScoped>(*this,lib,"clang_EnumDecl_isScoped",SideEffects::worstDefault,"clang_EnumDecl_isScoped")
		->args({"C"});
	addExtern<unsigned int (*)(CXCursor),clang_CXXMethod_isConst>(*this,lib,"clang_CXXMethod_isConst",SideEffects::worstDefault,"clang_CXXMethod_isConst")
		->args({"C"});
	addExtern<CXCursorKind (*)(CXCursor),clang_getTemplateCursorKind>(*this,lib,"clang_getTemplateCursorKind",SideEffects::worstDefault,"clang_getTemplateCursorKind")
		->args({"C"});
	addExtern<CXCursor (*)(CXCursor),clang_getSpecializedCursorTemplate,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getSpecializedCursorTemplate",SideEffects::worstDefault,"clang_getSpecializedCursorTemplate")
		->args({"C"});
	addExtern<CXSourceRange (*)(CXCursor,unsigned int,unsigned int),clang_getCursorReferenceNameRange,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorReferenceNameRange",SideEffects::worstDefault,"clang_getCursorReferenceNameRange")
		->args({"C","NameFlags","PieceIndex"});
	addExtern<CXToken * (*)(CXTranslationUnitImpl *,CXSourceLocation),clang_getToken>(*this,lib,"clang_getToken",SideEffects::worstDefault,"clang_getToken")
		->args({"TU","Location"});
	addExtern<CXTokenKind (*)(CXToken),clang_getTokenKind>(*this,lib,"clang_getTokenKind",SideEffects::worstDefault,"clang_getTokenKind")
		->args({""});
	addExtern<CXString (*)(CXTranslationUnitImpl *,CXToken),clang_getTokenSpelling,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTokenSpelling",SideEffects::worstDefault,"clang_getTokenSpelling")
		->args({"",""});
	addExtern<CXSourceLocation (*)(CXTranslationUnitImpl *,CXToken),clang_getTokenLocation,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTokenLocation",SideEffects::worstDefault,"clang_getTokenLocation")
		->args({"",""});
	addExtern<CXSourceRange (*)(CXTranslationUnitImpl *,CXToken),clang_getTokenExtent,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getTokenExtent",SideEffects::worstDefault,"clang_getTokenExtent")
		->args({"",""});
	addExtern<void (*)(CXTranslationUnitImpl *,CXSourceRange,CXToken **,unsigned int *),clang_tokenize>(*this,lib,"clang_tokenize",SideEffects::worstDefault,"clang_tokenize")
		->args({"TU","Range","Tokens","NumTokens"});
	addExtern<void (*)(CXTranslationUnitImpl *,CXToken *,unsigned int,CXCursor *),clang_annotateTokens>(*this,lib,"clang_annotateTokens",SideEffects::worstDefault,"clang_annotateTokens")
		->args({"TU","Tokens","NumTokens","Cursors"});
	addExtern<void (*)(CXTranslationUnitImpl *,CXToken *,unsigned int),clang_disposeTokens>(*this,lib,"clang_disposeTokens",SideEffects::worstDefault,"clang_disposeTokens")
		->args({"TU","Tokens","NumTokens"});
	addExtern<CXString (*)(CXCursorKind),clang_getCursorKindSpelling,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCursorKindSpelling",SideEffects::worstDefault,"clang_getCursorKindSpelling")
		->args({"Kind"});
	addExtern<void (*)(CXCursor,const char **,const char **,unsigned int *,unsigned int *,unsigned int *,unsigned int *),clang_getDefinitionSpellingAndExtent>(*this,lib,"clang_getDefinitionSpellingAndExtent",SideEffects::worstDefault,"clang_getDefinitionSpellingAndExtent")
		->args({"","startBuf","endBuf","startLine","startColumn","endLine","endColumn"});
	addExtern<void (*)(),clang_enableStackTraces>(*this,lib,"clang_enableStackTraces",SideEffects::worstDefault,"clang_enableStackTraces");
	addExtern<void (*)(void (*)(void *),void *,unsigned int),clang_executeOnThread>(*this,lib,"clang_executeOnThread",SideEffects::worstDefault,"clang_executeOnThread")
		->args({"fn","user_data","stack_size"});
	addExtern<CXCompletionChunkKind (*)(void *,unsigned int),clang_getCompletionChunkKind>(*this,lib,"clang_getCompletionChunkKind",SideEffects::worstDefault,"clang_getCompletionChunkKind")
		->args({"completion_string","chunk_number"});
	addExtern<CXString (*)(void *,unsigned int),clang_getCompletionChunkText,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCompletionChunkText",SideEffects::worstDefault,"clang_getCompletionChunkText")
		->args({"completion_string","chunk_number"});
	addExtern<void * (*)(void *,unsigned int),clang_getCompletionChunkCompletionString>(*this,lib,"clang_getCompletionChunkCompletionString",SideEffects::worstDefault,"clang_getCompletionChunkCompletionString")
		->args({"completion_string","chunk_number"});
	addExtern<unsigned int (*)(void *),clang_getNumCompletionChunks>(*this,lib,"clang_getNumCompletionChunks",SideEffects::worstDefault,"clang_getNumCompletionChunks")
		->args({"completion_string"});
	addExtern<unsigned int (*)(void *),clang_getCompletionPriority>(*this,lib,"clang_getCompletionPriority",SideEffects::worstDefault,"clang_getCompletionPriority")
		->args({"completion_string"});
	addExtern<CXAvailabilityKind (*)(void *),clang_getCompletionAvailability>(*this,lib,"clang_getCompletionAvailability",SideEffects::worstDefault,"clang_getCompletionAvailability")
		->args({"completion_string"});
	addExtern<unsigned int (*)(void *),clang_getCompletionNumAnnotations>(*this,lib,"clang_getCompletionNumAnnotations",SideEffects::worstDefault,"clang_getCompletionNumAnnotations")
		->args({"completion_string"});
	addExtern<CXString (*)(void *,unsigned int),clang_getCompletionAnnotation,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCompletionAnnotation",SideEffects::worstDefault,"clang_getCompletionAnnotation")
		->args({"completion_string","annotation_number"});
	addExtern<CXString (*)(void *,CXCursorKind *),clang_getCompletionParent,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCompletionParent",SideEffects::worstDefault,"clang_getCompletionParent")
		->args({"completion_string","kind"});
	addExtern<CXString (*)(void *),clang_getCompletionBriefComment,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCompletionBriefComment",SideEffects::worstDefault,"clang_getCompletionBriefComment")
		->args({"completion_string"});
	addExtern<void * (*)(CXCursor),clang_getCursorCompletionString>(*this,lib,"clang_getCursorCompletionString",SideEffects::worstDefault,"clang_getCursorCompletionString")
		->args({"cursor"});
	addExtern<unsigned int (*)(CXCodeCompleteResults *,unsigned int),clang_getCompletionNumFixIts>(*this,lib,"clang_getCompletionNumFixIts",SideEffects::worstDefault,"clang_getCompletionNumFixIts")
		->args({"results","completion_index"});
	addExtern<CXString (*)(CXCodeCompleteResults *,unsigned int,unsigned int,CXSourceRange *),clang_getCompletionFixIt,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getCompletionFixIt",SideEffects::worstDefault,"clang_getCompletionFixIt")
		->args({"results","completion_index","fixit_index","replacement_range"});
	addExtern<unsigned int (*)(),clang_defaultCodeCompleteOptions>(*this,lib,"clang_defaultCodeCompleteOptions",SideEffects::worstDefault,"clang_defaultCodeCompleteOptions");
	addExtern<CXCodeCompleteResults * (*)(CXTranslationUnitImpl *,const char *,unsigned int,unsigned int,CXUnsavedFile *,unsigned int,unsigned int),clang_codeCompleteAt>(*this,lib,"clang_codeCompleteAt",SideEffects::worstDefault,"clang_codeCompleteAt")
		->args({"TU","complete_filename","complete_line","complete_column","unsaved_files","num_unsaved_files","options"});
	addExtern<void (*)(CXCompletionResult *,unsigned int),clang_sortCodeCompletionResults>(*this,lib,"clang_sortCodeCompletionResults",SideEffects::worstDefault,"clang_sortCodeCompletionResults")
		->args({"Results","NumResults"});
	addExtern<void (*)(CXCodeCompleteResults *),clang_disposeCodeCompleteResults>(*this,lib,"clang_disposeCodeCompleteResults",SideEffects::worstDefault,"clang_disposeCodeCompleteResults")
		->args({"Results"});
	addExtern<unsigned int (*)(CXCodeCompleteResults *),clang_codeCompleteGetNumDiagnostics>(*this,lib,"clang_codeCompleteGetNumDiagnostics",SideEffects::worstDefault,"clang_codeCompleteGetNumDiagnostics")
		->args({"Results"});
	addExtern<void * (*)(CXCodeCompleteResults *,unsigned int),clang_codeCompleteGetDiagnostic>(*this,lib,"clang_codeCompleteGetDiagnostic",SideEffects::worstDefault,"clang_codeCompleteGetDiagnostic")
		->args({"Results","Index"});
	addExtern<unsigned long long (*)(CXCodeCompleteResults *),clang_codeCompleteGetContexts>(*this,lib,"clang_codeCompleteGetContexts",SideEffects::worstDefault,"clang_codeCompleteGetContexts")
		->args({"Results"});
	addExtern<CXCursorKind (*)(CXCodeCompleteResults *,unsigned int *),clang_codeCompleteGetContainerKind>(*this,lib,"clang_codeCompleteGetContainerKind",SideEffects::worstDefault,"clang_codeCompleteGetContainerKind")
		->args({"Results","IsIncomplete"});
	addExtern<CXString (*)(CXCodeCompleteResults *),clang_codeCompleteGetContainerUSR,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_codeCompleteGetContainerUSR",SideEffects::worstDefault,"clang_codeCompleteGetContainerUSR")
		->args({"Results"});
	addExtern<CXString (*)(CXCodeCompleteResults *),clang_codeCompleteGetObjCSelector,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_codeCompleteGetObjCSelector",SideEffects::worstDefault,"clang_codeCompleteGetObjCSelector")
		->args({"Results"});
	addExtern<CXString (*)(),clang_getClangVersion,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_getClangVersion",SideEffects::worstDefault,"clang_getClangVersion");
	addExtern<void (*)(unsigned int),clang_toggleCrashRecovery>(*this,lib,"clang_toggleCrashRecovery",SideEffects::worstDefault,"clang_toggleCrashRecovery")
		->args({"isEnabled"});
	addExtern<void (*)(CXTranslationUnitImpl *,void (*)(void *, CXSourceLocation *, unsigned int, void *),void *),clang_getInclusions>(*this,lib,"clang_getInclusions",SideEffects::worstDefault,"clang_getInclusions")
		->args({"tu","visitor","client_data"});
	addExtern<void * (*)(CXCursor),clang_Cursor_Evaluate>(*this,lib,"clang_Cursor_Evaluate",SideEffects::worstDefault,"clang_Cursor_Evaluate")
		->args({"C"});
	addExtern<CXEvalResultKind (*)(void *),clang_EvalResult_getKind>(*this,lib,"clang_EvalResult_getKind",SideEffects::worstDefault,"clang_EvalResult_getKind")
		->args({"E"});
	addExtern<int (*)(void *),clang_EvalResult_getAsInt>(*this,lib,"clang_EvalResult_getAsInt",SideEffects::worstDefault,"clang_EvalResult_getAsInt")
		->args({"E"});
	addExtern<long long (*)(void *),clang_EvalResult_getAsLongLong>(*this,lib,"clang_EvalResult_getAsLongLong",SideEffects::worstDefault,"clang_EvalResult_getAsLongLong")
		->args({"E"});
	addExtern<unsigned int (*)(void *),clang_EvalResult_isUnsignedInt>(*this,lib,"clang_EvalResult_isUnsignedInt",SideEffects::worstDefault,"clang_EvalResult_isUnsignedInt")
		->args({"E"});
	addExtern<unsigned long long (*)(void *),clang_EvalResult_getAsUnsigned>(*this,lib,"clang_EvalResult_getAsUnsigned",SideEffects::worstDefault,"clang_EvalResult_getAsUnsigned")
		->args({"E"});
	addExtern<double (*)(void *),clang_EvalResult_getAsDouble>(*this,lib,"clang_EvalResult_getAsDouble",SideEffects::worstDefault,"clang_EvalResult_getAsDouble")
		->args({"E"});
	addExtern<const char * (*)(void *),clang_EvalResult_getAsStr>(*this,lib,"clang_EvalResult_getAsStr",SideEffects::worstDefault,"clang_EvalResult_getAsStr")
		->args({"E"});
	addExtern<void (*)(void *),clang_EvalResult_dispose>(*this,lib,"clang_EvalResult_dispose",SideEffects::worstDefault,"clang_EvalResult_dispose")
		->args({"E"});
	addExtern<void * (*)(const char *),clang_getRemappings>(*this,lib,"clang_getRemappings",SideEffects::worstDefault,"clang_getRemappings")
		->args({"path"});
	addExtern<void * (*)(const char **,unsigned int),clang_getRemappingsFromFileList>(*this,lib,"clang_getRemappingsFromFileList",SideEffects::worstDefault,"clang_getRemappingsFromFileList")
		->args({"filePaths","numFiles"});
	addExtern<unsigned int (*)(void *),clang_remap_getNumFiles>(*this,lib,"clang_remap_getNumFiles",SideEffects::worstDefault,"clang_remap_getNumFiles")
		->args({""});
	addExtern<void (*)(void *,unsigned int,CXString *,CXString *),clang_remap_getFilenames>(*this,lib,"clang_remap_getFilenames",SideEffects::worstDefault,"clang_remap_getFilenames")
		->args({"","index","original","transformed"});
	addExtern<void (*)(void *),clang_remap_dispose>(*this,lib,"clang_remap_dispose",SideEffects::worstDefault,"clang_remap_dispose")
		->args({""});
	addExtern<CXResult (*)(CXCursor,void *,CXCursorAndRangeVisitor),clang_findReferencesInFile>(*this,lib,"clang_findReferencesInFile",SideEffects::worstDefault,"clang_findReferencesInFile")
		->args({"cursor","file","visitor"});
	addExtern<CXResult (*)(CXTranslationUnitImpl *,void *,CXCursorAndRangeVisitor),clang_findIncludesInFile>(*this,lib,"clang_findIncludesInFile",SideEffects::worstDefault,"clang_findIncludesInFile")
		->args({"TU","file","visitor"});
	addExtern<int (*)(CXIdxEntityKind),clang_index_isEntityObjCContainerKind>(*this,lib,"clang_index_isEntityObjCContainerKind",SideEffects::worstDefault,"clang_index_isEntityObjCContainerKind")
		->args({""});
	addExtern<const CXIdxObjCContainerDeclInfo * (*)(const CXIdxDeclInfo *),clang_index_getObjCContainerDeclInfo>(*this,lib,"clang_index_getObjCContainerDeclInfo",SideEffects::worstDefault,"clang_index_getObjCContainerDeclInfo")
		->args({""});
	addExtern<const CXIdxObjCInterfaceDeclInfo * (*)(const CXIdxDeclInfo *),clang_index_getObjCInterfaceDeclInfo>(*this,lib,"clang_index_getObjCInterfaceDeclInfo",SideEffects::worstDefault,"clang_index_getObjCInterfaceDeclInfo")
		->args({""});
	addExtern<const CXIdxObjCCategoryDeclInfo * (*)(const CXIdxDeclInfo *),clang_index_getObjCCategoryDeclInfo>(*this,lib,"clang_index_getObjCCategoryDeclInfo",SideEffects::worstDefault,"clang_index_getObjCCategoryDeclInfo")
		->args({""});
	addExtern<const CXIdxObjCProtocolRefListInfo * (*)(const CXIdxDeclInfo *),clang_index_getObjCProtocolRefListInfo>(*this,lib,"clang_index_getObjCProtocolRefListInfo",SideEffects::worstDefault,"clang_index_getObjCProtocolRefListInfo")
		->args({""});
	addExtern<const CXIdxObjCPropertyDeclInfo * (*)(const CXIdxDeclInfo *),clang_index_getObjCPropertyDeclInfo>(*this,lib,"clang_index_getObjCPropertyDeclInfo",SideEffects::worstDefault,"clang_index_getObjCPropertyDeclInfo")
		->args({""});
	addExtern<const CXIdxIBOutletCollectionAttrInfo * (*)(const CXIdxAttrInfo *),clang_index_getIBOutletCollectionAttrInfo>(*this,lib,"clang_index_getIBOutletCollectionAttrInfo",SideEffects::worstDefault,"clang_index_getIBOutletCollectionAttrInfo")
		->args({""});
	addExtern<const CXIdxCXXClassDeclInfo * (*)(const CXIdxDeclInfo *),clang_index_getCXXClassDeclInfo>(*this,lib,"clang_index_getCXXClassDeclInfo",SideEffects::worstDefault,"clang_index_getCXXClassDeclInfo")
		->args({""});
	addExtern<void * (*)(const CXIdxContainerInfo *),clang_index_getClientContainer>(*this,lib,"clang_index_getClientContainer",SideEffects::worstDefault,"clang_index_getClientContainer")
		->args({""});
	addExtern<void (*)(const CXIdxContainerInfo *,void *),clang_index_setClientContainer>(*this,lib,"clang_index_setClientContainer",SideEffects::worstDefault,"clang_index_setClientContainer")
		->args({"",""});
	addExtern<void * (*)(const CXIdxEntityInfo *),clang_index_getClientEntity>(*this,lib,"clang_index_getClientEntity",SideEffects::worstDefault,"clang_index_getClientEntity")
		->args({""});
	addExtern<void (*)(const CXIdxEntityInfo *,void *),clang_index_setClientEntity>(*this,lib,"clang_index_setClientEntity",SideEffects::worstDefault,"clang_index_setClientEntity")
		->args({"",""});
	addExtern<void * (*)(void *),clang_IndexAction_create>(*this,lib,"clang_IndexAction_create",SideEffects::worstDefault,"clang_IndexAction_create")
		->args({"CIdx"});
	addExtern<void (*)(void *),clang_IndexAction_dispose>(*this,lib,"clang_IndexAction_dispose",SideEffects::worstDefault,"clang_IndexAction_dispose")
		->args({""});
	addExtern<int (*)(void *,void *,IndexerCallbacks *,unsigned int,unsigned int,const char *,const char *const *,int,CXUnsavedFile *,unsigned int,CXTranslationUnitImpl **,unsigned int),clang_indexSourceFile>(*this,lib,"clang_indexSourceFile",SideEffects::worstDefault,"clang_indexSourceFile")
		->args({"","client_data","index_callbacks","index_callbacks_size","index_options","source_filename","command_line_args","num_command_line_args","unsaved_files","num_unsaved_files","out_TU","TU_options"});
	addExtern<int (*)(void *,void *,IndexerCallbacks *,unsigned int,unsigned int,const char *,const char *const *,int,CXUnsavedFile *,unsigned int,CXTranslationUnitImpl **,unsigned int),clang_indexSourceFileFullArgv>(*this,lib,"clang_indexSourceFileFullArgv",SideEffects::worstDefault,"clang_indexSourceFileFullArgv")
		->args({"","client_data","index_callbacks","index_callbacks_size","index_options","source_filename","command_line_args","num_command_line_args","unsaved_files","num_unsaved_files","out_TU","TU_options"});
	addExtern<int (*)(void *,void *,IndexerCallbacks *,unsigned int,unsigned int,CXTranslationUnitImpl *),clang_indexTranslationUnit>(*this,lib,"clang_indexTranslationUnit",SideEffects::worstDefault,"clang_indexTranslationUnit")
		->args({"","client_data","index_callbacks","index_callbacks_size","index_options",""});
	addExtern<void (*)(CXIdxLoc,void **,void **,unsigned int *,unsigned int *,unsigned int *),clang_indexLoc_getFileLocation>(*this,lib,"clang_indexLoc_getFileLocation",SideEffects::worstDefault,"clang_indexLoc_getFileLocation")
		->args({"loc","indexFile","file","line","column","offset"});
	addExtern<CXSourceLocation (*)(CXIdxLoc),clang_indexLoc_getCXSourceLocation,SimNode_ExtFuncCallAndCopyOrMove>(*this,lib,"clang_indexLoc_getCXSourceLocation",SideEffects::worstDefault,"clang_indexLoc_getCXSourceLocation")
		->args({"loc"});
	addExtern<unsigned int (*)(CXType,CXVisitorResult (*)(CXCursor, void *),void *),clang_Type_visitFields>(*this,lib,"clang_Type_visitFields",SideEffects::worstDefault,"clang_Type_visitFields")
		->args({"T","visitor","client_data"});
