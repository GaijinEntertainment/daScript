// this file is generated via daScript automatic C++ binder
// all user modifications will be lost after this file is re-generated

class Enumeration_CXErrorCode : public das::Enumeration {
public:
	Enumeration_CXErrorCode() : das::Enumeration("CXErrorCode") {
		external = true;
		cppName = "CXErrorCode";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXErrorCode >::type >::type;
		addIEx("CXError_Success", "CXError_Success", int64_t(CXErrorCode::CXError_Success), das::LineInfo());
		addIEx("CXError_Failure", "CXError_Failure", int64_t(CXErrorCode::CXError_Failure), das::LineInfo());
		addIEx("CXError_Crashed", "CXError_Crashed", int64_t(CXErrorCode::CXError_Crashed), das::LineInfo());
		addIEx("CXError_InvalidArguments", "CXError_InvalidArguments", int64_t(CXErrorCode::CXError_InvalidArguments), das::LineInfo());
		addIEx("CXError_ASTReadError", "CXError_ASTReadError", int64_t(CXErrorCode::CXError_ASTReadError), das::LineInfo());
	}
};

class Enumeration_CXAvailabilityKind : public das::Enumeration {
public:
	Enumeration_CXAvailabilityKind() : das::Enumeration("CXAvailabilityKind") {
		external = true;
		cppName = "CXAvailabilityKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXAvailabilityKind >::type >::type;
		addIEx("Available", "CXAvailability_Available", int64_t(CXAvailabilityKind::CXAvailability_Available), das::LineInfo());
		addIEx("Deprecated", "CXAvailability_Deprecated", int64_t(CXAvailabilityKind::CXAvailability_Deprecated), das::LineInfo());
		addIEx("NotAvailable", "CXAvailability_NotAvailable", int64_t(CXAvailabilityKind::CXAvailability_NotAvailable), das::LineInfo());
		addIEx("NotAccessible", "CXAvailability_NotAccessible", int64_t(CXAvailabilityKind::CXAvailability_NotAccessible), das::LineInfo());
	}
};

class Enumeration_CXCursor_ExceptionSpecificationKind : public das::Enumeration {
public:
	Enumeration_CXCursor_ExceptionSpecificationKind() : das::Enumeration("CXCursor_ExceptionSpecificationKind") {
		external = true;
		cppName = "CXCursor_ExceptionSpecificationKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXCursor_ExceptionSpecificationKind >::type >::type;
		addIEx("None", "CXCursor_ExceptionSpecificationKind_None", int64_t(CXCursor_ExceptionSpecificationKind::CXCursor_ExceptionSpecificationKind_None), das::LineInfo());
		addIEx("DynamicNone", "CXCursor_ExceptionSpecificationKind_DynamicNone", int64_t(CXCursor_ExceptionSpecificationKind::CXCursor_ExceptionSpecificationKind_DynamicNone), das::LineInfo());
		addIEx("Dynamic", "CXCursor_ExceptionSpecificationKind_Dynamic", int64_t(CXCursor_ExceptionSpecificationKind::CXCursor_ExceptionSpecificationKind_Dynamic), das::LineInfo());
		addIEx("MSAny", "CXCursor_ExceptionSpecificationKind_MSAny", int64_t(CXCursor_ExceptionSpecificationKind::CXCursor_ExceptionSpecificationKind_MSAny), das::LineInfo());
		addIEx("BasicNoexcept", "CXCursor_ExceptionSpecificationKind_BasicNoexcept", int64_t(CXCursor_ExceptionSpecificationKind::CXCursor_ExceptionSpecificationKind_BasicNoexcept), das::LineInfo());
		addIEx("ComputedNoexcept", "CXCursor_ExceptionSpecificationKind_ComputedNoexcept", int64_t(CXCursor_ExceptionSpecificationKind::CXCursor_ExceptionSpecificationKind_ComputedNoexcept), das::LineInfo());
		addIEx("Unevaluated", "CXCursor_ExceptionSpecificationKind_Unevaluated", int64_t(CXCursor_ExceptionSpecificationKind::CXCursor_ExceptionSpecificationKind_Unevaluated), das::LineInfo());
		addIEx("Uninstantiated", "CXCursor_ExceptionSpecificationKind_Uninstantiated", int64_t(CXCursor_ExceptionSpecificationKind::CXCursor_ExceptionSpecificationKind_Uninstantiated), das::LineInfo());
		addIEx("Unparsed", "CXCursor_ExceptionSpecificationKind_Unparsed", int64_t(CXCursor_ExceptionSpecificationKind::CXCursor_ExceptionSpecificationKind_Unparsed), das::LineInfo());
		addIEx("NoThrow", "CXCursor_ExceptionSpecificationKind_NoThrow", int64_t(CXCursor_ExceptionSpecificationKind::CXCursor_ExceptionSpecificationKind_NoThrow), das::LineInfo());
	}
};

class Enumeration_CXGlobalOptFlags : public das::Enumeration {
public:
	Enumeration_CXGlobalOptFlags() : das::Enumeration("CXGlobalOptFlags") {
		external = true;
		cppName = "CXGlobalOptFlags";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXGlobalOptFlags >::type >::type;
		addIEx("CXGlobalOpt_None", "CXGlobalOpt_None", int64_t(CXGlobalOptFlags::CXGlobalOpt_None), das::LineInfo());
		addIEx("CXGlobalOpt_ThreadBackgroundPriorityForIndexing", "CXGlobalOpt_ThreadBackgroundPriorityForIndexing", int64_t(CXGlobalOptFlags::CXGlobalOpt_ThreadBackgroundPriorityForIndexing), das::LineInfo());
		addIEx("CXGlobalOpt_ThreadBackgroundPriorityForEditing", "CXGlobalOpt_ThreadBackgroundPriorityForEditing", int64_t(CXGlobalOptFlags::CXGlobalOpt_ThreadBackgroundPriorityForEditing), das::LineInfo());
		addIEx("CXGlobalOpt_ThreadBackgroundPriorityForAll", "CXGlobalOpt_ThreadBackgroundPriorityForAll", int64_t(CXGlobalOptFlags::CXGlobalOpt_ThreadBackgroundPriorityForAll), das::LineInfo());
	}
};

class Enumeration_CXDiagnosticSeverity : public das::Enumeration {
public:
	Enumeration_CXDiagnosticSeverity() : das::Enumeration("CXDiagnosticSeverity") {
		external = true;
		cppName = "CXDiagnosticSeverity";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXDiagnosticSeverity >::type >::type;
		addIEx("Ignored", "CXDiagnostic_Ignored", int64_t(CXDiagnosticSeverity::CXDiagnostic_Ignored), das::LineInfo());
		addIEx("Note", "CXDiagnostic_Note", int64_t(CXDiagnosticSeverity::CXDiagnostic_Note), das::LineInfo());
		addIEx("Warning", "CXDiagnostic_Warning", int64_t(CXDiagnosticSeverity::CXDiagnostic_Warning), das::LineInfo());
		addIEx("Error", "CXDiagnostic_Error", int64_t(CXDiagnosticSeverity::CXDiagnostic_Error), das::LineInfo());
		addIEx("Fatal", "CXDiagnostic_Fatal", int64_t(CXDiagnosticSeverity::CXDiagnostic_Fatal), das::LineInfo());
	}
};

class Enumeration_CXLoadDiag_Error : public das::Enumeration {
public:
	Enumeration_CXLoadDiag_Error() : das::Enumeration("CXLoadDiag_Error") {
		external = true;
		cppName = "CXLoadDiag_Error";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXLoadDiag_Error >::type >::type;
		addIEx("None", "CXLoadDiag_None", int64_t(CXLoadDiag_Error::CXLoadDiag_None), das::LineInfo());
		addIEx("Unknown", "CXLoadDiag_Unknown", int64_t(CXLoadDiag_Error::CXLoadDiag_Unknown), das::LineInfo());
		addIEx("CannotLoad", "CXLoadDiag_CannotLoad", int64_t(CXLoadDiag_Error::CXLoadDiag_CannotLoad), das::LineInfo());
		addIEx("InvalidFile", "CXLoadDiag_InvalidFile", int64_t(CXLoadDiag_Error::CXLoadDiag_InvalidFile), das::LineInfo());
	}
};

class Enumeration_CXDiagnosticDisplayOptions : public das::Enumeration {
public:
	Enumeration_CXDiagnosticDisplayOptions() : das::Enumeration("CXDiagnosticDisplayOptions") {
		external = true;
		cppName = "CXDiagnosticDisplayOptions";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXDiagnosticDisplayOptions >::type >::type;
		addIEx("SourceLocation", "CXDiagnostic_DisplaySourceLocation", int64_t(CXDiagnosticDisplayOptions::CXDiagnostic_DisplaySourceLocation), das::LineInfo());
		addIEx("Column", "CXDiagnostic_DisplayColumn", int64_t(CXDiagnosticDisplayOptions::CXDiagnostic_DisplayColumn), das::LineInfo());
		addIEx("SourceRanges", "CXDiagnostic_DisplaySourceRanges", int64_t(CXDiagnosticDisplayOptions::CXDiagnostic_DisplaySourceRanges), das::LineInfo());
		addIEx("Option", "CXDiagnostic_DisplayOption", int64_t(CXDiagnosticDisplayOptions::CXDiagnostic_DisplayOption), das::LineInfo());
		addIEx("CategoryId", "CXDiagnostic_DisplayCategoryId", int64_t(CXDiagnosticDisplayOptions::CXDiagnostic_DisplayCategoryId), das::LineInfo());
		addIEx("CategoryName", "CXDiagnostic_DisplayCategoryName", int64_t(CXDiagnosticDisplayOptions::CXDiagnostic_DisplayCategoryName), das::LineInfo());
	}
};

class Enumeration_CXTranslationUnit_Flags : public das::Enumeration {
public:
	Enumeration_CXTranslationUnit_Flags() : das::Enumeration("CXTranslationUnit_Flags") {
		external = true;
		cppName = "CXTranslationUnit_Flags";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXTranslationUnit_Flags >::type >::type;
		addIEx("CXTranslationUnit_None", "CXTranslationUnit_None", int64_t(CXTranslationUnit_Flags::CXTranslationUnit_None), das::LineInfo());
		addIEx("CXTranslationUnit_DetailedPreprocessingRecord", "CXTranslationUnit_DetailedPreprocessingRecord", int64_t(CXTranslationUnit_Flags::CXTranslationUnit_DetailedPreprocessingRecord), das::LineInfo());
		addIEx("CXTranslationUnit_Incomplete", "CXTranslationUnit_Incomplete", int64_t(CXTranslationUnit_Flags::CXTranslationUnit_Incomplete), das::LineInfo());
		addIEx("CXTranslationUnit_PrecompiledPreamble", "CXTranslationUnit_PrecompiledPreamble", int64_t(CXTranslationUnit_Flags::CXTranslationUnit_PrecompiledPreamble), das::LineInfo());
		addIEx("CXTranslationUnit_CacheCompletionResults", "CXTranslationUnit_CacheCompletionResults", int64_t(CXTranslationUnit_Flags::CXTranslationUnit_CacheCompletionResults), das::LineInfo());
		addIEx("CXTranslationUnit_ForSerialization", "CXTranslationUnit_ForSerialization", int64_t(CXTranslationUnit_Flags::CXTranslationUnit_ForSerialization), das::LineInfo());
		addIEx("CXTranslationUnit_CXXChainedPCH", "CXTranslationUnit_CXXChainedPCH", int64_t(CXTranslationUnit_Flags::CXTranslationUnit_CXXChainedPCH), das::LineInfo());
		addIEx("CXTranslationUnit_SkipFunctionBodies", "CXTranslationUnit_SkipFunctionBodies", int64_t(CXTranslationUnit_Flags::CXTranslationUnit_SkipFunctionBodies), das::LineInfo());
		addIEx("CXTranslationUnit_IncludeBriefCommentsInCodeCompletion", "CXTranslationUnit_IncludeBriefCommentsInCodeCompletion", int64_t(CXTranslationUnit_Flags::CXTranslationUnit_IncludeBriefCommentsInCodeCompletion), das::LineInfo());
		addIEx("CXTranslationUnit_CreatePreambleOnFirstParse", "CXTranslationUnit_CreatePreambleOnFirstParse", int64_t(CXTranslationUnit_Flags::CXTranslationUnit_CreatePreambleOnFirstParse), das::LineInfo());
		addIEx("CXTranslationUnit_KeepGoing", "CXTranslationUnit_KeepGoing", int64_t(CXTranslationUnit_Flags::CXTranslationUnit_KeepGoing), das::LineInfo());
		addIEx("CXTranslationUnit_SingleFileParse", "CXTranslationUnit_SingleFileParse", int64_t(CXTranslationUnit_Flags::CXTranslationUnit_SingleFileParse), das::LineInfo());
		addIEx("CXTranslationUnit_LimitSkipFunctionBodiesToPreamble", "CXTranslationUnit_LimitSkipFunctionBodiesToPreamble", int64_t(CXTranslationUnit_Flags::CXTranslationUnit_LimitSkipFunctionBodiesToPreamble), das::LineInfo());
		addIEx("CXTranslationUnit_IncludeAttributedTypes", "CXTranslationUnit_IncludeAttributedTypes", int64_t(CXTranslationUnit_Flags::CXTranslationUnit_IncludeAttributedTypes), das::LineInfo());
		addIEx("CXTranslationUnit_VisitImplicitAttributes", "CXTranslationUnit_VisitImplicitAttributes", int64_t(CXTranslationUnit_Flags::CXTranslationUnit_VisitImplicitAttributes), das::LineInfo());
		addIEx("CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles", "CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles", int64_t(CXTranslationUnit_Flags::CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles), das::LineInfo());
		addIEx("CXTranslationUnit_RetainExcludedConditionalBlocks", "CXTranslationUnit_RetainExcludedConditionalBlocks", int64_t(CXTranslationUnit_Flags::CXTranslationUnit_RetainExcludedConditionalBlocks), das::LineInfo());
	}
};

class Enumeration_CXSaveTranslationUnit_Flags : public das::Enumeration {
public:
	Enumeration_CXSaveTranslationUnit_Flags() : das::Enumeration("CXSaveTranslationUnit_Flags") {
		external = true;
		cppName = "CXSaveTranslationUnit_Flags";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXSaveTranslationUnit_Flags >::type >::type;
		addIEx("CXSaveTranslationUnit_None", "CXSaveTranslationUnit_None", int64_t(CXSaveTranslationUnit_Flags::CXSaveTranslationUnit_None), das::LineInfo());
	}
};

class Enumeration_CXSaveError : public das::Enumeration {
public:
	Enumeration_CXSaveError() : das::Enumeration("CXSaveError") {
		external = true;
		cppName = "CXSaveError";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXSaveError >::type >::type;
		addIEx("None", "CXSaveError_None", int64_t(CXSaveError::CXSaveError_None), das::LineInfo());
		addIEx("Unknown", "CXSaveError_Unknown", int64_t(CXSaveError::CXSaveError_Unknown), das::LineInfo());
		addIEx("TranslationErrors", "CXSaveError_TranslationErrors", int64_t(CXSaveError::CXSaveError_TranslationErrors), das::LineInfo());
		addIEx("InvalidTU", "CXSaveError_InvalidTU", int64_t(CXSaveError::CXSaveError_InvalidTU), das::LineInfo());
	}
};

class Enumeration_CXReparse_Flags : public das::Enumeration {
public:
	Enumeration_CXReparse_Flags() : das::Enumeration("CXReparse_Flags") {
		external = true;
		cppName = "CXReparse_Flags";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXReparse_Flags >::type >::type;
		addIEx("CXReparse_None", "CXReparse_None", int64_t(CXReparse_Flags::CXReparse_None), das::LineInfo());
	}
};

class Enumeration_CXTUResourceUsageKind : public das::Enumeration {
public:
	Enumeration_CXTUResourceUsageKind() : das::Enumeration("CXTUResourceUsageKind") {
		external = true;
		cppName = "CXTUResourceUsageKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXTUResourceUsageKind >::type >::type;
		addIEx("CXTUResourceUsage_AST", "CXTUResourceUsage_AST", int64_t(CXTUResourceUsageKind::CXTUResourceUsage_AST), das::LineInfo());
		addIEx("CXTUResourceUsage_Identifiers", "CXTUResourceUsage_Identifiers", int64_t(CXTUResourceUsageKind::CXTUResourceUsage_Identifiers), das::LineInfo());
		addIEx("CXTUResourceUsage_Selectors", "CXTUResourceUsage_Selectors", int64_t(CXTUResourceUsageKind::CXTUResourceUsage_Selectors), das::LineInfo());
		addIEx("CXTUResourceUsage_GlobalCompletionResults", "CXTUResourceUsage_GlobalCompletionResults", int64_t(CXTUResourceUsageKind::CXTUResourceUsage_GlobalCompletionResults), das::LineInfo());
		addIEx("CXTUResourceUsage_SourceManagerContentCache", "CXTUResourceUsage_SourceManagerContentCache", int64_t(CXTUResourceUsageKind::CXTUResourceUsage_SourceManagerContentCache), das::LineInfo());
		addIEx("CXTUResourceUsage_AST_SideTables", "CXTUResourceUsage_AST_SideTables", int64_t(CXTUResourceUsageKind::CXTUResourceUsage_AST_SideTables), das::LineInfo());
		addIEx("CXTUResourceUsage_SourceManager_Membuffer_Malloc", "CXTUResourceUsage_SourceManager_Membuffer_Malloc", int64_t(CXTUResourceUsageKind::CXTUResourceUsage_SourceManager_Membuffer_Malloc), das::LineInfo());
		addIEx("CXTUResourceUsage_SourceManager_Membuffer_MMap", "CXTUResourceUsage_SourceManager_Membuffer_MMap", int64_t(CXTUResourceUsageKind::CXTUResourceUsage_SourceManager_Membuffer_MMap), das::LineInfo());
		addIEx("CXTUResourceUsage_ExternalASTSource_Membuffer_Malloc", "CXTUResourceUsage_ExternalASTSource_Membuffer_Malloc", int64_t(CXTUResourceUsageKind::CXTUResourceUsage_ExternalASTSource_Membuffer_Malloc), das::LineInfo());
		addIEx("CXTUResourceUsage_ExternalASTSource_Membuffer_MMap", "CXTUResourceUsage_ExternalASTSource_Membuffer_MMap", int64_t(CXTUResourceUsageKind::CXTUResourceUsage_ExternalASTSource_Membuffer_MMap), das::LineInfo());
		addIEx("CXTUResourceUsage_Preprocessor", "CXTUResourceUsage_Preprocessor", int64_t(CXTUResourceUsageKind::CXTUResourceUsage_Preprocessor), das::LineInfo());
		addIEx("CXTUResourceUsage_PreprocessingRecord", "CXTUResourceUsage_PreprocessingRecord", int64_t(CXTUResourceUsageKind::CXTUResourceUsage_PreprocessingRecord), das::LineInfo());
		addIEx("CXTUResourceUsage_SourceManager_DataStructures", "CXTUResourceUsage_SourceManager_DataStructures", int64_t(CXTUResourceUsageKind::CXTUResourceUsage_SourceManager_DataStructures), das::LineInfo());
		addIEx("CXTUResourceUsage_Preprocessor_HeaderSearch", "CXTUResourceUsage_Preprocessor_HeaderSearch", int64_t(CXTUResourceUsageKind::CXTUResourceUsage_Preprocessor_HeaderSearch), das::LineInfo());
		addIEx("CXTUResourceUsage_MEMORY_IN_BYTES_BEGIN", "CXTUResourceUsage_MEMORY_IN_BYTES_BEGIN", int64_t(CXTUResourceUsageKind::CXTUResourceUsage_MEMORY_IN_BYTES_BEGIN), das::LineInfo());
		addIEx("CXTUResourceUsage_MEMORY_IN_BYTES_END", "CXTUResourceUsage_MEMORY_IN_BYTES_END", int64_t(CXTUResourceUsageKind::CXTUResourceUsage_MEMORY_IN_BYTES_END), das::LineInfo());
		addIEx("CXTUResourceUsage_First", "CXTUResourceUsage_First", int64_t(CXTUResourceUsageKind::CXTUResourceUsage_First), das::LineInfo());
		addIEx("CXTUResourceUsage_Last", "CXTUResourceUsage_Last", int64_t(CXTUResourceUsageKind::CXTUResourceUsage_Last), das::LineInfo());
	}
};

class Enumeration_CXCursorKind : public das::Enumeration {
public:
	Enumeration_CXCursorKind() : das::Enumeration("CXCursorKind") {
		external = true;
		cppName = "CXCursorKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXCursorKind >::type >::type;
		addIEx("UnexposedDecl", "CXCursor_UnexposedDecl", int64_t(CXCursorKind::CXCursor_UnexposedDecl), das::LineInfo());
		addIEx("StructDecl", "CXCursor_StructDecl", int64_t(CXCursorKind::CXCursor_StructDecl), das::LineInfo());
		addIEx("UnionDecl", "CXCursor_UnionDecl", int64_t(CXCursorKind::CXCursor_UnionDecl), das::LineInfo());
		addIEx("ClassDecl", "CXCursor_ClassDecl", int64_t(CXCursorKind::CXCursor_ClassDecl), das::LineInfo());
		addIEx("EnumDecl", "CXCursor_EnumDecl", int64_t(CXCursorKind::CXCursor_EnumDecl), das::LineInfo());
		addIEx("FieldDecl", "CXCursor_FieldDecl", int64_t(CXCursorKind::CXCursor_FieldDecl), das::LineInfo());
		addIEx("EnumConstantDecl", "CXCursor_EnumConstantDecl", int64_t(CXCursorKind::CXCursor_EnumConstantDecl), das::LineInfo());
		addIEx("FunctionDecl", "CXCursor_FunctionDecl", int64_t(CXCursorKind::CXCursor_FunctionDecl), das::LineInfo());
		addIEx("VarDecl", "CXCursor_VarDecl", int64_t(CXCursorKind::CXCursor_VarDecl), das::LineInfo());
		addIEx("ParmDecl", "CXCursor_ParmDecl", int64_t(CXCursorKind::CXCursor_ParmDecl), das::LineInfo());
		addIEx("ObjCInterfaceDecl", "CXCursor_ObjCInterfaceDecl", int64_t(CXCursorKind::CXCursor_ObjCInterfaceDecl), das::LineInfo());
		addIEx("ObjCCategoryDecl", "CXCursor_ObjCCategoryDecl", int64_t(CXCursorKind::CXCursor_ObjCCategoryDecl), das::LineInfo());
		addIEx("ObjCProtocolDecl", "CXCursor_ObjCProtocolDecl", int64_t(CXCursorKind::CXCursor_ObjCProtocolDecl), das::LineInfo());
		addIEx("ObjCPropertyDecl", "CXCursor_ObjCPropertyDecl", int64_t(CXCursorKind::CXCursor_ObjCPropertyDecl), das::LineInfo());
		addIEx("ObjCIvarDecl", "CXCursor_ObjCIvarDecl", int64_t(CXCursorKind::CXCursor_ObjCIvarDecl), das::LineInfo());
		addIEx("ObjCInstanceMethodDecl", "CXCursor_ObjCInstanceMethodDecl", int64_t(CXCursorKind::CXCursor_ObjCInstanceMethodDecl), das::LineInfo());
		addIEx("ObjCClassMethodDecl", "CXCursor_ObjCClassMethodDecl", int64_t(CXCursorKind::CXCursor_ObjCClassMethodDecl), das::LineInfo());
		addIEx("ObjCImplementationDecl", "CXCursor_ObjCImplementationDecl", int64_t(CXCursorKind::CXCursor_ObjCImplementationDecl), das::LineInfo());
		addIEx("ObjCCategoryImplDecl", "CXCursor_ObjCCategoryImplDecl", int64_t(CXCursorKind::CXCursor_ObjCCategoryImplDecl), das::LineInfo());
		addIEx("TypedefDecl", "CXCursor_TypedefDecl", int64_t(CXCursorKind::CXCursor_TypedefDecl), das::LineInfo());
		addIEx("CXXMethod", "CXCursor_CXXMethod", int64_t(CXCursorKind::CXCursor_CXXMethod), das::LineInfo());
		addIEx("Namespace", "CXCursor_Namespace", int64_t(CXCursorKind::CXCursor_Namespace), das::LineInfo());
		addIEx("LinkageSpec", "CXCursor_LinkageSpec", int64_t(CXCursorKind::CXCursor_LinkageSpec), das::LineInfo());
		addIEx("Constructor", "CXCursor_Constructor", int64_t(CXCursorKind::CXCursor_Constructor), das::LineInfo());
		addIEx("Destructor", "CXCursor_Destructor", int64_t(CXCursorKind::CXCursor_Destructor), das::LineInfo());
		addIEx("ConversionFunction", "CXCursor_ConversionFunction", int64_t(CXCursorKind::CXCursor_ConversionFunction), das::LineInfo());
		addIEx("TemplateTypeParameter", "CXCursor_TemplateTypeParameter", int64_t(CXCursorKind::CXCursor_TemplateTypeParameter), das::LineInfo());
		addIEx("NonTypeTemplateParameter", "CXCursor_NonTypeTemplateParameter", int64_t(CXCursorKind::CXCursor_NonTypeTemplateParameter), das::LineInfo());
		addIEx("TemplateTemplateParameter", "CXCursor_TemplateTemplateParameter", int64_t(CXCursorKind::CXCursor_TemplateTemplateParameter), das::LineInfo());
		addIEx("FunctionTemplate", "CXCursor_FunctionTemplate", int64_t(CXCursorKind::CXCursor_FunctionTemplate), das::LineInfo());
		addIEx("ClassTemplate", "CXCursor_ClassTemplate", int64_t(CXCursorKind::CXCursor_ClassTemplate), das::LineInfo());
		addIEx("ClassTemplatePartialSpecialization", "CXCursor_ClassTemplatePartialSpecialization", int64_t(CXCursorKind::CXCursor_ClassTemplatePartialSpecialization), das::LineInfo());
		addIEx("NamespaceAlias", "CXCursor_NamespaceAlias", int64_t(CXCursorKind::CXCursor_NamespaceAlias), das::LineInfo());
		addIEx("UsingDirective", "CXCursor_UsingDirective", int64_t(CXCursorKind::CXCursor_UsingDirective), das::LineInfo());
		addIEx("UsingDeclaration", "CXCursor_UsingDeclaration", int64_t(CXCursorKind::CXCursor_UsingDeclaration), das::LineInfo());
		addIEx("TypeAliasDecl", "CXCursor_TypeAliasDecl", int64_t(CXCursorKind::CXCursor_TypeAliasDecl), das::LineInfo());
		addIEx("ObjCSynthesizeDecl", "CXCursor_ObjCSynthesizeDecl", int64_t(CXCursorKind::CXCursor_ObjCSynthesizeDecl), das::LineInfo());
		addIEx("ObjCDynamicDecl", "CXCursor_ObjCDynamicDecl", int64_t(CXCursorKind::CXCursor_ObjCDynamicDecl), das::LineInfo());
		addIEx("CXXAccessSpecifier", "CXCursor_CXXAccessSpecifier", int64_t(CXCursorKind::CXCursor_CXXAccessSpecifier), das::LineInfo());
		addIEx("FirstDecl", "CXCursor_FirstDecl", int64_t(CXCursorKind::CXCursor_FirstDecl), das::LineInfo());
		addIEx("LastDecl", "CXCursor_LastDecl", int64_t(CXCursorKind::CXCursor_LastDecl), das::LineInfo());
		addIEx("FirstRef", "CXCursor_FirstRef", int64_t(CXCursorKind::CXCursor_FirstRef), das::LineInfo());
		addIEx("ObjCSuperClassRef", "CXCursor_ObjCSuperClassRef", int64_t(CXCursorKind::CXCursor_ObjCSuperClassRef), das::LineInfo());
		addIEx("ObjCProtocolRef", "CXCursor_ObjCProtocolRef", int64_t(CXCursorKind::CXCursor_ObjCProtocolRef), das::LineInfo());
		addIEx("ObjCClassRef", "CXCursor_ObjCClassRef", int64_t(CXCursorKind::CXCursor_ObjCClassRef), das::LineInfo());
		addIEx("TypeRef", "CXCursor_TypeRef", int64_t(CXCursorKind::CXCursor_TypeRef), das::LineInfo());
		addIEx("CXXBaseSpecifier", "CXCursor_CXXBaseSpecifier", int64_t(CXCursorKind::CXCursor_CXXBaseSpecifier), das::LineInfo());
		addIEx("TemplateRef", "CXCursor_TemplateRef", int64_t(CXCursorKind::CXCursor_TemplateRef), das::LineInfo());
		addIEx("NamespaceRef", "CXCursor_NamespaceRef", int64_t(CXCursorKind::CXCursor_NamespaceRef), das::LineInfo());
		addIEx("MemberRef", "CXCursor_MemberRef", int64_t(CXCursorKind::CXCursor_MemberRef), das::LineInfo());
		addIEx("LabelRef", "CXCursor_LabelRef", int64_t(CXCursorKind::CXCursor_LabelRef), das::LineInfo());
		addIEx("OverloadedDeclRef", "CXCursor_OverloadedDeclRef", int64_t(CXCursorKind::CXCursor_OverloadedDeclRef), das::LineInfo());
		addIEx("VariableRef", "CXCursor_VariableRef", int64_t(CXCursorKind::CXCursor_VariableRef), das::LineInfo());
		addIEx("LastRef", "CXCursor_LastRef", int64_t(CXCursorKind::CXCursor_LastRef), das::LineInfo());
		addIEx("FirstInvalid", "CXCursor_FirstInvalid", int64_t(CXCursorKind::CXCursor_FirstInvalid), das::LineInfo());
		addIEx("InvalidFile", "CXCursor_InvalidFile", int64_t(CXCursorKind::CXCursor_InvalidFile), das::LineInfo());
		addIEx("NoDeclFound", "CXCursor_NoDeclFound", int64_t(CXCursorKind::CXCursor_NoDeclFound), das::LineInfo());
		addIEx("NotImplemented", "CXCursor_NotImplemented", int64_t(CXCursorKind::CXCursor_NotImplemented), das::LineInfo());
		addIEx("InvalidCode", "CXCursor_InvalidCode", int64_t(CXCursorKind::CXCursor_InvalidCode), das::LineInfo());
		addIEx("LastInvalid", "CXCursor_LastInvalid", int64_t(CXCursorKind::CXCursor_LastInvalid), das::LineInfo());
		addIEx("FirstExpr", "CXCursor_FirstExpr", int64_t(CXCursorKind::CXCursor_FirstExpr), das::LineInfo());
		addIEx("UnexposedExpr", "CXCursor_UnexposedExpr", int64_t(CXCursorKind::CXCursor_UnexposedExpr), das::LineInfo());
		addIEx("DeclRefExpr", "CXCursor_DeclRefExpr", int64_t(CXCursorKind::CXCursor_DeclRefExpr), das::LineInfo());
		addIEx("MemberRefExpr", "CXCursor_MemberRefExpr", int64_t(CXCursorKind::CXCursor_MemberRefExpr), das::LineInfo());
		addIEx("CallExpr", "CXCursor_CallExpr", int64_t(CXCursorKind::CXCursor_CallExpr), das::LineInfo());
		addIEx("ObjCMessageExpr", "CXCursor_ObjCMessageExpr", int64_t(CXCursorKind::CXCursor_ObjCMessageExpr), das::LineInfo());
		addIEx("BlockExpr", "CXCursor_BlockExpr", int64_t(CXCursorKind::CXCursor_BlockExpr), das::LineInfo());
		addIEx("IntegerLiteral", "CXCursor_IntegerLiteral", int64_t(CXCursorKind::CXCursor_IntegerLiteral), das::LineInfo());
		addIEx("FloatingLiteral", "CXCursor_FloatingLiteral", int64_t(CXCursorKind::CXCursor_FloatingLiteral), das::LineInfo());
		addIEx("ImaginaryLiteral", "CXCursor_ImaginaryLiteral", int64_t(CXCursorKind::CXCursor_ImaginaryLiteral), das::LineInfo());
		addIEx("StringLiteral", "CXCursor_StringLiteral", int64_t(CXCursorKind::CXCursor_StringLiteral), das::LineInfo());
		addIEx("CharacterLiteral", "CXCursor_CharacterLiteral", int64_t(CXCursorKind::CXCursor_CharacterLiteral), das::LineInfo());
		addIEx("ParenExpr", "CXCursor_ParenExpr", int64_t(CXCursorKind::CXCursor_ParenExpr), das::LineInfo());
		addIEx("UnaryOperator", "CXCursor_UnaryOperator", int64_t(CXCursorKind::CXCursor_UnaryOperator), das::LineInfo());
		addIEx("ArraySubscriptExpr", "CXCursor_ArraySubscriptExpr", int64_t(CXCursorKind::CXCursor_ArraySubscriptExpr), das::LineInfo());
		addIEx("BinaryOperator", "CXCursor_BinaryOperator", int64_t(CXCursorKind::CXCursor_BinaryOperator), das::LineInfo());
		addIEx("CompoundAssignOperator", "CXCursor_CompoundAssignOperator", int64_t(CXCursorKind::CXCursor_CompoundAssignOperator), das::LineInfo());
		addIEx("ConditionalOperator", "CXCursor_ConditionalOperator", int64_t(CXCursorKind::CXCursor_ConditionalOperator), das::LineInfo());
		addIEx("CStyleCastExpr", "CXCursor_CStyleCastExpr", int64_t(CXCursorKind::CXCursor_CStyleCastExpr), das::LineInfo());
		addIEx("CompoundLiteralExpr", "CXCursor_CompoundLiteralExpr", int64_t(CXCursorKind::CXCursor_CompoundLiteralExpr), das::LineInfo());
		addIEx("InitListExpr", "CXCursor_InitListExpr", int64_t(CXCursorKind::CXCursor_InitListExpr), das::LineInfo());
		addIEx("AddrLabelExpr", "CXCursor_AddrLabelExpr", int64_t(CXCursorKind::CXCursor_AddrLabelExpr), das::LineInfo());
		addIEx("StmtExpr", "CXCursor_StmtExpr", int64_t(CXCursorKind::CXCursor_StmtExpr), das::LineInfo());
		addIEx("GenericSelectionExpr", "CXCursor_GenericSelectionExpr", int64_t(CXCursorKind::CXCursor_GenericSelectionExpr), das::LineInfo());
		addIEx("GNUNullExpr", "CXCursor_GNUNullExpr", int64_t(CXCursorKind::CXCursor_GNUNullExpr), das::LineInfo());
		addIEx("CXXStaticCastExpr", "CXCursor_CXXStaticCastExpr", int64_t(CXCursorKind::CXCursor_CXXStaticCastExpr), das::LineInfo());
		addIEx("CXXDynamicCastExpr", "CXCursor_CXXDynamicCastExpr", int64_t(CXCursorKind::CXCursor_CXXDynamicCastExpr), das::LineInfo());
		addIEx("CXXReinterpretCastExpr", "CXCursor_CXXReinterpretCastExpr", int64_t(CXCursorKind::CXCursor_CXXReinterpretCastExpr), das::LineInfo());
		addIEx("CXXConstCastExpr", "CXCursor_CXXConstCastExpr", int64_t(CXCursorKind::CXCursor_CXXConstCastExpr), das::LineInfo());
		addIEx("CXXFunctionalCastExpr", "CXCursor_CXXFunctionalCastExpr", int64_t(CXCursorKind::CXCursor_CXXFunctionalCastExpr), das::LineInfo());
		addIEx("CXXTypeidExpr", "CXCursor_CXXTypeidExpr", int64_t(CXCursorKind::CXCursor_CXXTypeidExpr), das::LineInfo());
		addIEx("CXXBoolLiteralExpr", "CXCursor_CXXBoolLiteralExpr", int64_t(CXCursorKind::CXCursor_CXXBoolLiteralExpr), das::LineInfo());
		addIEx("CXXNullPtrLiteralExpr", "CXCursor_CXXNullPtrLiteralExpr", int64_t(CXCursorKind::CXCursor_CXXNullPtrLiteralExpr), das::LineInfo());
		addIEx("CXXThisExpr", "CXCursor_CXXThisExpr", int64_t(CXCursorKind::CXCursor_CXXThisExpr), das::LineInfo());
		addIEx("CXXThrowExpr", "CXCursor_CXXThrowExpr", int64_t(CXCursorKind::CXCursor_CXXThrowExpr), das::LineInfo());
		addIEx("CXXNewExpr", "CXCursor_CXXNewExpr", int64_t(CXCursorKind::CXCursor_CXXNewExpr), das::LineInfo());
		addIEx("CXXDeleteExpr", "CXCursor_CXXDeleteExpr", int64_t(CXCursorKind::CXCursor_CXXDeleteExpr), das::LineInfo());
		addIEx("UnaryExpr", "CXCursor_UnaryExpr", int64_t(CXCursorKind::CXCursor_UnaryExpr), das::LineInfo());
		addIEx("ObjCStringLiteral", "CXCursor_ObjCStringLiteral", int64_t(CXCursorKind::CXCursor_ObjCStringLiteral), das::LineInfo());
		addIEx("ObjCEncodeExpr", "CXCursor_ObjCEncodeExpr", int64_t(CXCursorKind::CXCursor_ObjCEncodeExpr), das::LineInfo());
		addIEx("ObjCSelectorExpr", "CXCursor_ObjCSelectorExpr", int64_t(CXCursorKind::CXCursor_ObjCSelectorExpr), das::LineInfo());
		addIEx("ObjCProtocolExpr", "CXCursor_ObjCProtocolExpr", int64_t(CXCursorKind::CXCursor_ObjCProtocolExpr), das::LineInfo());
		addIEx("ObjCBridgedCastExpr", "CXCursor_ObjCBridgedCastExpr", int64_t(CXCursorKind::CXCursor_ObjCBridgedCastExpr), das::LineInfo());
		addIEx("PackExpansionExpr", "CXCursor_PackExpansionExpr", int64_t(CXCursorKind::CXCursor_PackExpansionExpr), das::LineInfo());
		addIEx("SizeOfPackExpr", "CXCursor_SizeOfPackExpr", int64_t(CXCursorKind::CXCursor_SizeOfPackExpr), das::LineInfo());
		addIEx("LambdaExpr", "CXCursor_LambdaExpr", int64_t(CXCursorKind::CXCursor_LambdaExpr), das::LineInfo());
		addIEx("ObjCBoolLiteralExpr", "CXCursor_ObjCBoolLiteralExpr", int64_t(CXCursorKind::CXCursor_ObjCBoolLiteralExpr), das::LineInfo());
		addIEx("ObjCSelfExpr", "CXCursor_ObjCSelfExpr", int64_t(CXCursorKind::CXCursor_ObjCSelfExpr), das::LineInfo());
		addIEx("OMPArraySectionExpr", "CXCursor_OMPArraySectionExpr", int64_t(CXCursorKind::CXCursor_OMPArraySectionExpr), das::LineInfo());
		addIEx("ObjCAvailabilityCheckExpr", "CXCursor_ObjCAvailabilityCheckExpr", int64_t(CXCursorKind::CXCursor_ObjCAvailabilityCheckExpr), das::LineInfo());
		addIEx("FixedPointLiteral", "CXCursor_FixedPointLiteral", int64_t(CXCursorKind::CXCursor_FixedPointLiteral), das::LineInfo());
		addIEx("OMPArrayShapingExpr", "CXCursor_OMPArrayShapingExpr", int64_t(CXCursorKind::CXCursor_OMPArrayShapingExpr), das::LineInfo());
		addIEx("OMPIteratorExpr", "CXCursor_OMPIteratorExpr", int64_t(CXCursorKind::CXCursor_OMPIteratorExpr), das::LineInfo());
		addIEx("CXXAddrspaceCastExpr", "CXCursor_CXXAddrspaceCastExpr", int64_t(CXCursorKind::CXCursor_CXXAddrspaceCastExpr), das::LineInfo());
		addIEx("LastExpr", "CXCursor_LastExpr", int64_t(CXCursorKind::CXCursor_LastExpr), das::LineInfo());
		addIEx("FirstStmt", "CXCursor_FirstStmt", int64_t(CXCursorKind::CXCursor_FirstStmt), das::LineInfo());
		addIEx("UnexposedStmt", "CXCursor_UnexposedStmt", int64_t(CXCursorKind::CXCursor_UnexposedStmt), das::LineInfo());
		addIEx("LabelStmt", "CXCursor_LabelStmt", int64_t(CXCursorKind::CXCursor_LabelStmt), das::LineInfo());
		addIEx("CompoundStmt", "CXCursor_CompoundStmt", int64_t(CXCursorKind::CXCursor_CompoundStmt), das::LineInfo());
		addIEx("CaseStmt", "CXCursor_CaseStmt", int64_t(CXCursorKind::CXCursor_CaseStmt), das::LineInfo());
		addIEx("DefaultStmt", "CXCursor_DefaultStmt", int64_t(CXCursorKind::CXCursor_DefaultStmt), das::LineInfo());
		addIEx("IfStmt", "CXCursor_IfStmt", int64_t(CXCursorKind::CXCursor_IfStmt), das::LineInfo());
		addIEx("SwitchStmt", "CXCursor_SwitchStmt", int64_t(CXCursorKind::CXCursor_SwitchStmt), das::LineInfo());
		addIEx("WhileStmt", "CXCursor_WhileStmt", int64_t(CXCursorKind::CXCursor_WhileStmt), das::LineInfo());
		addIEx("DoStmt", "CXCursor_DoStmt", int64_t(CXCursorKind::CXCursor_DoStmt), das::LineInfo());
		addIEx("ForStmt", "CXCursor_ForStmt", int64_t(CXCursorKind::CXCursor_ForStmt), das::LineInfo());
		addIEx("GotoStmt", "CXCursor_GotoStmt", int64_t(CXCursorKind::CXCursor_GotoStmt), das::LineInfo());
		addIEx("IndirectGotoStmt", "CXCursor_IndirectGotoStmt", int64_t(CXCursorKind::CXCursor_IndirectGotoStmt), das::LineInfo());
		addIEx("ContinueStmt", "CXCursor_ContinueStmt", int64_t(CXCursorKind::CXCursor_ContinueStmt), das::LineInfo());
		addIEx("BreakStmt", "CXCursor_BreakStmt", int64_t(CXCursorKind::CXCursor_BreakStmt), das::LineInfo());
		addIEx("ReturnStmt", "CXCursor_ReturnStmt", int64_t(CXCursorKind::CXCursor_ReturnStmt), das::LineInfo());
		addIEx("GCCAsmStmt", "CXCursor_GCCAsmStmt", int64_t(CXCursorKind::CXCursor_GCCAsmStmt), das::LineInfo());
		addIEx("AsmStmt", "CXCursor_AsmStmt", int64_t(CXCursorKind::CXCursor_AsmStmt), das::LineInfo());
		addIEx("ObjCAtTryStmt", "CXCursor_ObjCAtTryStmt", int64_t(CXCursorKind::CXCursor_ObjCAtTryStmt), das::LineInfo());
		addIEx("ObjCAtCatchStmt", "CXCursor_ObjCAtCatchStmt", int64_t(CXCursorKind::CXCursor_ObjCAtCatchStmt), das::LineInfo());
		addIEx("ObjCAtFinallyStmt", "CXCursor_ObjCAtFinallyStmt", int64_t(CXCursorKind::CXCursor_ObjCAtFinallyStmt), das::LineInfo());
		addIEx("ObjCAtThrowStmt", "CXCursor_ObjCAtThrowStmt", int64_t(CXCursorKind::CXCursor_ObjCAtThrowStmt), das::LineInfo());
		addIEx("ObjCAtSynchronizedStmt", "CXCursor_ObjCAtSynchronizedStmt", int64_t(CXCursorKind::CXCursor_ObjCAtSynchronizedStmt), das::LineInfo());
		addIEx("ObjCAutoreleasePoolStmt", "CXCursor_ObjCAutoreleasePoolStmt", int64_t(CXCursorKind::CXCursor_ObjCAutoreleasePoolStmt), das::LineInfo());
		addIEx("ObjCForCollectionStmt", "CXCursor_ObjCForCollectionStmt", int64_t(CXCursorKind::CXCursor_ObjCForCollectionStmt), das::LineInfo());
		addIEx("CXXCatchStmt", "CXCursor_CXXCatchStmt", int64_t(CXCursorKind::CXCursor_CXXCatchStmt), das::LineInfo());
		addIEx("CXXTryStmt", "CXCursor_CXXTryStmt", int64_t(CXCursorKind::CXCursor_CXXTryStmt), das::LineInfo());
		addIEx("CXXForRangeStmt", "CXCursor_CXXForRangeStmt", int64_t(CXCursorKind::CXCursor_CXXForRangeStmt), das::LineInfo());
		addIEx("SEHTryStmt", "CXCursor_SEHTryStmt", int64_t(CXCursorKind::CXCursor_SEHTryStmt), das::LineInfo());
		addIEx("SEHExceptStmt", "CXCursor_SEHExceptStmt", int64_t(CXCursorKind::CXCursor_SEHExceptStmt), das::LineInfo());
		addIEx("SEHFinallyStmt", "CXCursor_SEHFinallyStmt", int64_t(CXCursorKind::CXCursor_SEHFinallyStmt), das::LineInfo());
		addIEx("MSAsmStmt", "CXCursor_MSAsmStmt", int64_t(CXCursorKind::CXCursor_MSAsmStmt), das::LineInfo());
		addIEx("NullStmt", "CXCursor_NullStmt", int64_t(CXCursorKind::CXCursor_NullStmt), das::LineInfo());
		addIEx("DeclStmt", "CXCursor_DeclStmt", int64_t(CXCursorKind::CXCursor_DeclStmt), das::LineInfo());
		addIEx("OMPParallelDirective", "CXCursor_OMPParallelDirective", int64_t(CXCursorKind::CXCursor_OMPParallelDirective), das::LineInfo());
		addIEx("OMPSimdDirective", "CXCursor_OMPSimdDirective", int64_t(CXCursorKind::CXCursor_OMPSimdDirective), das::LineInfo());
		addIEx("OMPForDirective", "CXCursor_OMPForDirective", int64_t(CXCursorKind::CXCursor_OMPForDirective), das::LineInfo());
		addIEx("OMPSectionsDirective", "CXCursor_OMPSectionsDirective", int64_t(CXCursorKind::CXCursor_OMPSectionsDirective), das::LineInfo());
		addIEx("OMPSectionDirective", "CXCursor_OMPSectionDirective", int64_t(CXCursorKind::CXCursor_OMPSectionDirective), das::LineInfo());
		addIEx("OMPSingleDirective", "CXCursor_OMPSingleDirective", int64_t(CXCursorKind::CXCursor_OMPSingleDirective), das::LineInfo());
		addIEx("OMPParallelForDirective", "CXCursor_OMPParallelForDirective", int64_t(CXCursorKind::CXCursor_OMPParallelForDirective), das::LineInfo());
		addIEx("OMPParallelSectionsDirective", "CXCursor_OMPParallelSectionsDirective", int64_t(CXCursorKind::CXCursor_OMPParallelSectionsDirective), das::LineInfo());
		addIEx("OMPTaskDirective", "CXCursor_OMPTaskDirective", int64_t(CXCursorKind::CXCursor_OMPTaskDirective), das::LineInfo());
		addIEx("OMPMasterDirective", "CXCursor_OMPMasterDirective", int64_t(CXCursorKind::CXCursor_OMPMasterDirective), das::LineInfo());
		addIEx("OMPCriticalDirective", "CXCursor_OMPCriticalDirective", int64_t(CXCursorKind::CXCursor_OMPCriticalDirective), das::LineInfo());
		addIEx("OMPTaskyieldDirective", "CXCursor_OMPTaskyieldDirective", int64_t(CXCursorKind::CXCursor_OMPTaskyieldDirective), das::LineInfo());
		addIEx("OMPBarrierDirective", "CXCursor_OMPBarrierDirective", int64_t(CXCursorKind::CXCursor_OMPBarrierDirective), das::LineInfo());
		addIEx("OMPTaskwaitDirective", "CXCursor_OMPTaskwaitDirective", int64_t(CXCursorKind::CXCursor_OMPTaskwaitDirective), das::LineInfo());
		addIEx("OMPFlushDirective", "CXCursor_OMPFlushDirective", int64_t(CXCursorKind::CXCursor_OMPFlushDirective), das::LineInfo());
		addIEx("SEHLeaveStmt", "CXCursor_SEHLeaveStmt", int64_t(CXCursorKind::CXCursor_SEHLeaveStmt), das::LineInfo());
		addIEx("OMPOrderedDirective", "CXCursor_OMPOrderedDirective", int64_t(CXCursorKind::CXCursor_OMPOrderedDirective), das::LineInfo());
		addIEx("OMPAtomicDirective", "CXCursor_OMPAtomicDirective", int64_t(CXCursorKind::CXCursor_OMPAtomicDirective), das::LineInfo());
		addIEx("OMPForSimdDirective", "CXCursor_OMPForSimdDirective", int64_t(CXCursorKind::CXCursor_OMPForSimdDirective), das::LineInfo());
		addIEx("OMPParallelForSimdDirective", "CXCursor_OMPParallelForSimdDirective", int64_t(CXCursorKind::CXCursor_OMPParallelForSimdDirective), das::LineInfo());
		addIEx("OMPTargetDirective", "CXCursor_OMPTargetDirective", int64_t(CXCursorKind::CXCursor_OMPTargetDirective), das::LineInfo());
		addIEx("OMPTeamsDirective", "CXCursor_OMPTeamsDirective", int64_t(CXCursorKind::CXCursor_OMPTeamsDirective), das::LineInfo());
		addIEx("OMPTaskgroupDirective", "CXCursor_OMPTaskgroupDirective", int64_t(CXCursorKind::CXCursor_OMPTaskgroupDirective), das::LineInfo());
		addIEx("OMPCancellationPointDirective", "CXCursor_OMPCancellationPointDirective", int64_t(CXCursorKind::CXCursor_OMPCancellationPointDirective), das::LineInfo());
		addIEx("OMPCancelDirective", "CXCursor_OMPCancelDirective", int64_t(CXCursorKind::CXCursor_OMPCancelDirective), das::LineInfo());
		addIEx("OMPTargetDataDirective", "CXCursor_OMPTargetDataDirective", int64_t(CXCursorKind::CXCursor_OMPTargetDataDirective), das::LineInfo());
		addIEx("OMPTaskLoopDirective", "CXCursor_OMPTaskLoopDirective", int64_t(CXCursorKind::CXCursor_OMPTaskLoopDirective), das::LineInfo());
		addIEx("OMPTaskLoopSimdDirective", "CXCursor_OMPTaskLoopSimdDirective", int64_t(CXCursorKind::CXCursor_OMPTaskLoopSimdDirective), das::LineInfo());
		addIEx("OMPDistributeDirective", "CXCursor_OMPDistributeDirective", int64_t(CXCursorKind::CXCursor_OMPDistributeDirective), das::LineInfo());
		addIEx("OMPTargetEnterDataDirective", "CXCursor_OMPTargetEnterDataDirective", int64_t(CXCursorKind::CXCursor_OMPTargetEnterDataDirective), das::LineInfo());
		addIEx("OMPTargetExitDataDirective", "CXCursor_OMPTargetExitDataDirective", int64_t(CXCursorKind::CXCursor_OMPTargetExitDataDirective), das::LineInfo());
		addIEx("OMPTargetParallelDirective", "CXCursor_OMPTargetParallelDirective", int64_t(CXCursorKind::CXCursor_OMPTargetParallelDirective), das::LineInfo());
		addIEx("OMPTargetParallelForDirective", "CXCursor_OMPTargetParallelForDirective", int64_t(CXCursorKind::CXCursor_OMPTargetParallelForDirective), das::LineInfo());
		addIEx("OMPTargetUpdateDirective", "CXCursor_OMPTargetUpdateDirective", int64_t(CXCursorKind::CXCursor_OMPTargetUpdateDirective), das::LineInfo());
		addIEx("OMPDistributeParallelForDirective", "CXCursor_OMPDistributeParallelForDirective", int64_t(CXCursorKind::CXCursor_OMPDistributeParallelForDirective), das::LineInfo());
		addIEx("OMPDistributeParallelForSimdDirective", "CXCursor_OMPDistributeParallelForSimdDirective", int64_t(CXCursorKind::CXCursor_OMPDistributeParallelForSimdDirective), das::LineInfo());
		addIEx("OMPDistributeSimdDirective", "CXCursor_OMPDistributeSimdDirective", int64_t(CXCursorKind::CXCursor_OMPDistributeSimdDirective), das::LineInfo());
		addIEx("OMPTargetParallelForSimdDirective", "CXCursor_OMPTargetParallelForSimdDirective", int64_t(CXCursorKind::CXCursor_OMPTargetParallelForSimdDirective), das::LineInfo());
		addIEx("OMPTargetSimdDirective", "CXCursor_OMPTargetSimdDirective", int64_t(CXCursorKind::CXCursor_OMPTargetSimdDirective), das::LineInfo());
		addIEx("OMPTeamsDistributeDirective", "CXCursor_OMPTeamsDistributeDirective", int64_t(CXCursorKind::CXCursor_OMPTeamsDistributeDirective), das::LineInfo());
		addIEx("OMPTeamsDistributeSimdDirective", "CXCursor_OMPTeamsDistributeSimdDirective", int64_t(CXCursorKind::CXCursor_OMPTeamsDistributeSimdDirective), das::LineInfo());
		addIEx("OMPTeamsDistributeParallelForSimdDirective", "CXCursor_OMPTeamsDistributeParallelForSimdDirective", int64_t(CXCursorKind::CXCursor_OMPTeamsDistributeParallelForSimdDirective), das::LineInfo());
		addIEx("OMPTeamsDistributeParallelForDirective", "CXCursor_OMPTeamsDistributeParallelForDirective", int64_t(CXCursorKind::CXCursor_OMPTeamsDistributeParallelForDirective), das::LineInfo());
		addIEx("OMPTargetTeamsDirective", "CXCursor_OMPTargetTeamsDirective", int64_t(CXCursorKind::CXCursor_OMPTargetTeamsDirective), das::LineInfo());
		addIEx("OMPTargetTeamsDistributeDirective", "CXCursor_OMPTargetTeamsDistributeDirective", int64_t(CXCursorKind::CXCursor_OMPTargetTeamsDistributeDirective), das::LineInfo());
		addIEx("OMPTargetTeamsDistributeParallelForDirective", "CXCursor_OMPTargetTeamsDistributeParallelForDirective", int64_t(CXCursorKind::CXCursor_OMPTargetTeamsDistributeParallelForDirective), das::LineInfo());
		addIEx("OMPTargetTeamsDistributeParallelForSimdDirective", "CXCursor_OMPTargetTeamsDistributeParallelForSimdDirective", int64_t(CXCursorKind::CXCursor_OMPTargetTeamsDistributeParallelForSimdDirective), das::LineInfo());
		addIEx("OMPTargetTeamsDistributeSimdDirective", "CXCursor_OMPTargetTeamsDistributeSimdDirective", int64_t(CXCursorKind::CXCursor_OMPTargetTeamsDistributeSimdDirective), das::LineInfo());
		addIEx("BuiltinBitCastExpr", "CXCursor_BuiltinBitCastExpr", int64_t(CXCursorKind::CXCursor_BuiltinBitCastExpr), das::LineInfo());
		addIEx("OMPMasterTaskLoopDirective", "CXCursor_OMPMasterTaskLoopDirective", int64_t(CXCursorKind::CXCursor_OMPMasterTaskLoopDirective), das::LineInfo());
		addIEx("OMPParallelMasterTaskLoopDirective", "CXCursor_OMPParallelMasterTaskLoopDirective", int64_t(CXCursorKind::CXCursor_OMPParallelMasterTaskLoopDirective), das::LineInfo());
		addIEx("OMPMasterTaskLoopSimdDirective", "CXCursor_OMPMasterTaskLoopSimdDirective", int64_t(CXCursorKind::CXCursor_OMPMasterTaskLoopSimdDirective), das::LineInfo());
		addIEx("OMPParallelMasterTaskLoopSimdDirective", "CXCursor_OMPParallelMasterTaskLoopSimdDirective", int64_t(CXCursorKind::CXCursor_OMPParallelMasterTaskLoopSimdDirective), das::LineInfo());
		addIEx("OMPParallelMasterDirective", "CXCursor_OMPParallelMasterDirective", int64_t(CXCursorKind::CXCursor_OMPParallelMasterDirective), das::LineInfo());
		addIEx("OMPDepobjDirective", "CXCursor_OMPDepobjDirective", int64_t(CXCursorKind::CXCursor_OMPDepobjDirective), das::LineInfo());
		addIEx("OMPScanDirective", "CXCursor_OMPScanDirective", int64_t(CXCursorKind::CXCursor_OMPScanDirective), das::LineInfo());
		addIEx("OMPTileDirective", "CXCursor_OMPTileDirective", int64_t(CXCursorKind::CXCursor_OMPTileDirective), das::LineInfo());
		addIEx("OMPCanonicalLoop", "CXCursor_OMPCanonicalLoop", int64_t(CXCursorKind::CXCursor_OMPCanonicalLoop), das::LineInfo());
		addIEx("OMPInteropDirective", "CXCursor_OMPInteropDirective", int64_t(CXCursorKind::CXCursor_OMPInteropDirective), das::LineInfo());
		addIEx("OMPDispatchDirective", "CXCursor_OMPDispatchDirective", int64_t(CXCursorKind::CXCursor_OMPDispatchDirective), das::LineInfo());
		addIEx("OMPMaskedDirective", "CXCursor_OMPMaskedDirective", int64_t(CXCursorKind::CXCursor_OMPMaskedDirective), das::LineInfo());
		addIEx("OMPUnrollDirective", "CXCursor_OMPUnrollDirective", int64_t(CXCursorKind::CXCursor_OMPUnrollDirective), das::LineInfo());
		addIEx("LastStmt", "CXCursor_LastStmt", int64_t(CXCursorKind::CXCursor_LastStmt), das::LineInfo());
		addIEx("TranslationUnit", "CXCursor_TranslationUnit", int64_t(CXCursorKind::CXCursor_TranslationUnit), das::LineInfo());
		addIEx("FirstAttr", "CXCursor_FirstAttr", int64_t(CXCursorKind::CXCursor_FirstAttr), das::LineInfo());
		addIEx("UnexposedAttr", "CXCursor_UnexposedAttr", int64_t(CXCursorKind::CXCursor_UnexposedAttr), das::LineInfo());
		addIEx("IBActionAttr", "CXCursor_IBActionAttr", int64_t(CXCursorKind::CXCursor_IBActionAttr), das::LineInfo());
		addIEx("IBOutletAttr", "CXCursor_IBOutletAttr", int64_t(CXCursorKind::CXCursor_IBOutletAttr), das::LineInfo());
		addIEx("IBOutletCollectionAttr", "CXCursor_IBOutletCollectionAttr", int64_t(CXCursorKind::CXCursor_IBOutletCollectionAttr), das::LineInfo());
		addIEx("CXXFinalAttr", "CXCursor_CXXFinalAttr", int64_t(CXCursorKind::CXCursor_CXXFinalAttr), das::LineInfo());
		addIEx("CXXOverrideAttr", "CXCursor_CXXOverrideAttr", int64_t(CXCursorKind::CXCursor_CXXOverrideAttr), das::LineInfo());
		addIEx("AnnotateAttr", "CXCursor_AnnotateAttr", int64_t(CXCursorKind::CXCursor_AnnotateAttr), das::LineInfo());
		addIEx("AsmLabelAttr", "CXCursor_AsmLabelAttr", int64_t(CXCursorKind::CXCursor_AsmLabelAttr), das::LineInfo());
		addIEx("PackedAttr", "CXCursor_PackedAttr", int64_t(CXCursorKind::CXCursor_PackedAttr), das::LineInfo());
		addIEx("PureAttr", "CXCursor_PureAttr", int64_t(CXCursorKind::CXCursor_PureAttr), das::LineInfo());
		addIEx("ConstAttr", "CXCursor_ConstAttr", int64_t(CXCursorKind::CXCursor_ConstAttr), das::LineInfo());
		addIEx("NoDuplicateAttr", "CXCursor_NoDuplicateAttr", int64_t(CXCursorKind::CXCursor_NoDuplicateAttr), das::LineInfo());
		addIEx("CUDAConstantAttr", "CXCursor_CUDAConstantAttr", int64_t(CXCursorKind::CXCursor_CUDAConstantAttr), das::LineInfo());
		addIEx("CUDADeviceAttr", "CXCursor_CUDADeviceAttr", int64_t(CXCursorKind::CXCursor_CUDADeviceAttr), das::LineInfo());
		addIEx("CUDAGlobalAttr", "CXCursor_CUDAGlobalAttr", int64_t(CXCursorKind::CXCursor_CUDAGlobalAttr), das::LineInfo());
		addIEx("CUDAHostAttr", "CXCursor_CUDAHostAttr", int64_t(CXCursorKind::CXCursor_CUDAHostAttr), das::LineInfo());
		addIEx("CUDASharedAttr", "CXCursor_CUDASharedAttr", int64_t(CXCursorKind::CXCursor_CUDASharedAttr), das::LineInfo());
		addIEx("VisibilityAttr", "CXCursor_VisibilityAttr", int64_t(CXCursorKind::CXCursor_VisibilityAttr), das::LineInfo());
		addIEx("DLLExport", "CXCursor_DLLExport", int64_t(CXCursorKind::CXCursor_DLLExport), das::LineInfo());
		addIEx("DLLImport", "CXCursor_DLLImport", int64_t(CXCursorKind::CXCursor_DLLImport), das::LineInfo());
		addIEx("NSReturnsRetained", "CXCursor_NSReturnsRetained", int64_t(CXCursorKind::CXCursor_NSReturnsRetained), das::LineInfo());
		addIEx("NSReturnsNotRetained", "CXCursor_NSReturnsNotRetained", int64_t(CXCursorKind::CXCursor_NSReturnsNotRetained), das::LineInfo());
		addIEx("NSReturnsAutoreleased", "CXCursor_NSReturnsAutoreleased", int64_t(CXCursorKind::CXCursor_NSReturnsAutoreleased), das::LineInfo());
		addIEx("NSConsumesSelf", "CXCursor_NSConsumesSelf", int64_t(CXCursorKind::CXCursor_NSConsumesSelf), das::LineInfo());
		addIEx("NSConsumed", "CXCursor_NSConsumed", int64_t(CXCursorKind::CXCursor_NSConsumed), das::LineInfo());
		addIEx("ObjCException", "CXCursor_ObjCException", int64_t(CXCursorKind::CXCursor_ObjCException), das::LineInfo());
		addIEx("ObjCNSObject", "CXCursor_ObjCNSObject", int64_t(CXCursorKind::CXCursor_ObjCNSObject), das::LineInfo());
		addIEx("ObjCIndependentClass", "CXCursor_ObjCIndependentClass", int64_t(CXCursorKind::CXCursor_ObjCIndependentClass), das::LineInfo());
		addIEx("ObjCPreciseLifetime", "CXCursor_ObjCPreciseLifetime", int64_t(CXCursorKind::CXCursor_ObjCPreciseLifetime), das::LineInfo());
		addIEx("ObjCReturnsInnerPointer", "CXCursor_ObjCReturnsInnerPointer", int64_t(CXCursorKind::CXCursor_ObjCReturnsInnerPointer), das::LineInfo());
		addIEx("ObjCRequiresSuper", "CXCursor_ObjCRequiresSuper", int64_t(CXCursorKind::CXCursor_ObjCRequiresSuper), das::LineInfo());
		addIEx("ObjCRootClass", "CXCursor_ObjCRootClass", int64_t(CXCursorKind::CXCursor_ObjCRootClass), das::LineInfo());
		addIEx("ObjCSubclassingRestricted", "CXCursor_ObjCSubclassingRestricted", int64_t(CXCursorKind::CXCursor_ObjCSubclassingRestricted), das::LineInfo());
		addIEx("ObjCExplicitProtocolImpl", "CXCursor_ObjCExplicitProtocolImpl", int64_t(CXCursorKind::CXCursor_ObjCExplicitProtocolImpl), das::LineInfo());
		addIEx("ObjCDesignatedInitializer", "CXCursor_ObjCDesignatedInitializer", int64_t(CXCursorKind::CXCursor_ObjCDesignatedInitializer), das::LineInfo());
		addIEx("ObjCRuntimeVisible", "CXCursor_ObjCRuntimeVisible", int64_t(CXCursorKind::CXCursor_ObjCRuntimeVisible), das::LineInfo());
		addIEx("ObjCBoxable", "CXCursor_ObjCBoxable", int64_t(CXCursorKind::CXCursor_ObjCBoxable), das::LineInfo());
		addIEx("FlagEnum", "CXCursor_FlagEnum", int64_t(CXCursorKind::CXCursor_FlagEnum), das::LineInfo());
		addIEx("ConvergentAttr", "CXCursor_ConvergentAttr", int64_t(CXCursorKind::CXCursor_ConvergentAttr), das::LineInfo());
		addIEx("WarnUnusedAttr", "CXCursor_WarnUnusedAttr", int64_t(CXCursorKind::CXCursor_WarnUnusedAttr), das::LineInfo());
		addIEx("WarnUnusedResultAttr", "CXCursor_WarnUnusedResultAttr", int64_t(CXCursorKind::CXCursor_WarnUnusedResultAttr), das::LineInfo());
		addIEx("AlignedAttr", "CXCursor_AlignedAttr", int64_t(CXCursorKind::CXCursor_AlignedAttr), das::LineInfo());
		addIEx("LastAttr", "CXCursor_LastAttr", int64_t(CXCursorKind::CXCursor_LastAttr), das::LineInfo());
		addIEx("PreprocessingDirective", "CXCursor_PreprocessingDirective", int64_t(CXCursorKind::CXCursor_PreprocessingDirective), das::LineInfo());
		addIEx("MacroDefinition", "CXCursor_MacroDefinition", int64_t(CXCursorKind::CXCursor_MacroDefinition), das::LineInfo());
		addIEx("MacroExpansion", "CXCursor_MacroExpansion", int64_t(CXCursorKind::CXCursor_MacroExpansion), das::LineInfo());
		addIEx("MacroInstantiation", "CXCursor_MacroInstantiation", int64_t(CXCursorKind::CXCursor_MacroInstantiation), das::LineInfo());
		addIEx("InclusionDirective", "CXCursor_InclusionDirective", int64_t(CXCursorKind::CXCursor_InclusionDirective), das::LineInfo());
		addIEx("FirstPreprocessing", "CXCursor_FirstPreprocessing", int64_t(CXCursorKind::CXCursor_FirstPreprocessing), das::LineInfo());
		addIEx("LastPreprocessing", "CXCursor_LastPreprocessing", int64_t(CXCursorKind::CXCursor_LastPreprocessing), das::LineInfo());
		addIEx("ModuleImportDecl", "CXCursor_ModuleImportDecl", int64_t(CXCursorKind::CXCursor_ModuleImportDecl), das::LineInfo());
		addIEx("TypeAliasTemplateDecl", "CXCursor_TypeAliasTemplateDecl", int64_t(CXCursorKind::CXCursor_TypeAliasTemplateDecl), das::LineInfo());
		addIEx("StaticAssert", "CXCursor_StaticAssert", int64_t(CXCursorKind::CXCursor_StaticAssert), das::LineInfo());
		addIEx("FriendDecl", "CXCursor_FriendDecl", int64_t(CXCursorKind::CXCursor_FriendDecl), das::LineInfo());
		addIEx("FirstExtraDecl", "CXCursor_FirstExtraDecl", int64_t(CXCursorKind::CXCursor_FirstExtraDecl), das::LineInfo());
		addIEx("LastExtraDecl", "CXCursor_LastExtraDecl", int64_t(CXCursorKind::CXCursor_LastExtraDecl), das::LineInfo());
		addIEx("OverloadCandidate", "CXCursor_OverloadCandidate", int64_t(CXCursorKind::CXCursor_OverloadCandidate), das::LineInfo());
	}
};

class Enumeration_CXLinkageKind : public das::Enumeration {
public:
	Enumeration_CXLinkageKind() : das::Enumeration("CXLinkageKind") {
		external = true;
		cppName = "CXLinkageKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXLinkageKind >::type >::type;
		addIEx("CXLinkage_Invalid", "CXLinkage_Invalid", int64_t(CXLinkageKind::CXLinkage_Invalid), das::LineInfo());
		addIEx("CXLinkage_NoLinkage", "CXLinkage_NoLinkage", int64_t(CXLinkageKind::CXLinkage_NoLinkage), das::LineInfo());
		addIEx("CXLinkage_Internal", "CXLinkage_Internal", int64_t(CXLinkageKind::CXLinkage_Internal), das::LineInfo());
		addIEx("CXLinkage_UniqueExternal", "CXLinkage_UniqueExternal", int64_t(CXLinkageKind::CXLinkage_UniqueExternal), das::LineInfo());
		addIEx("CXLinkage_External", "CXLinkage_External", int64_t(CXLinkageKind::CXLinkage_External), das::LineInfo());
	}
};

class Enumeration_CXVisibilityKind : public das::Enumeration {
public:
	Enumeration_CXVisibilityKind() : das::Enumeration("CXVisibilityKind") {
		external = true;
		cppName = "CXVisibilityKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXVisibilityKind >::type >::type;
		addIEx("CXVisibility_Invalid", "CXVisibility_Invalid", int64_t(CXVisibilityKind::CXVisibility_Invalid), das::LineInfo());
		addIEx("CXVisibility_Hidden", "CXVisibility_Hidden", int64_t(CXVisibilityKind::CXVisibility_Hidden), das::LineInfo());
		addIEx("CXVisibility_Protected", "CXVisibility_Protected", int64_t(CXVisibilityKind::CXVisibility_Protected), das::LineInfo());
		addIEx("CXVisibility_Default", "CXVisibility_Default", int64_t(CXVisibilityKind::CXVisibility_Default), das::LineInfo());
	}
};

class Enumeration_CXLanguageKind : public das::Enumeration {
public:
	Enumeration_CXLanguageKind() : das::Enumeration("CXLanguageKind") {
		external = true;
		cppName = "CXLanguageKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXLanguageKind >::type >::type;
		addIEx("CXLanguage_Invalid", "CXLanguage_Invalid", int64_t(CXLanguageKind::CXLanguage_Invalid), das::LineInfo());
		addIEx("CXLanguage_C", "CXLanguage_C", int64_t(CXLanguageKind::CXLanguage_C), das::LineInfo());
		addIEx("CXLanguage_ObjC", "CXLanguage_ObjC", int64_t(CXLanguageKind::CXLanguage_ObjC), das::LineInfo());
		addIEx("CXLanguage_CPlusPlus", "CXLanguage_CPlusPlus", int64_t(CXLanguageKind::CXLanguage_CPlusPlus), das::LineInfo());
	}
};

class Enumeration_CXTLSKind : public das::Enumeration {
public:
	Enumeration_CXTLSKind() : das::Enumeration("CXTLSKind") {
		external = true;
		cppName = "CXTLSKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXTLSKind >::type >::type;
		addIEx("CXTLS_None", "CXTLS_None", int64_t(CXTLSKind::CXTLS_None), das::LineInfo());
		addIEx("CXTLS_Dynamic", "CXTLS_Dynamic", int64_t(CXTLSKind::CXTLS_Dynamic), das::LineInfo());
		addIEx("CXTLS_Static", "CXTLS_Static", int64_t(CXTLSKind::CXTLS_Static), das::LineInfo());
	}
};

class Enumeration_CXTypeKind : public das::Enumeration {
public:
	Enumeration_CXTypeKind() : das::Enumeration("CXTypeKind") {
		external = true;
		cppName = "CXTypeKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXTypeKind >::type >::type;
		addIEx("Invalid", "CXType_Invalid", int64_t(CXTypeKind::CXType_Invalid), das::LineInfo());
		addIEx("Unexposed", "CXType_Unexposed", int64_t(CXTypeKind::CXType_Unexposed), das::LineInfo());
		addIEx("Void", "CXType_Void", int64_t(CXTypeKind::CXType_Void), das::LineInfo());
		addIEx("Bool", "CXType_Bool", int64_t(CXTypeKind::CXType_Bool), das::LineInfo());
		addIEx("Char_U", "CXType_Char_U", int64_t(CXTypeKind::CXType_Char_U), das::LineInfo());
		addIEx("UChar", "CXType_UChar", int64_t(CXTypeKind::CXType_UChar), das::LineInfo());
		addIEx("Char16", "CXType_Char16", int64_t(CXTypeKind::CXType_Char16), das::LineInfo());
		addIEx("Char32", "CXType_Char32", int64_t(CXTypeKind::CXType_Char32), das::LineInfo());
		addIEx("UShort", "CXType_UShort", int64_t(CXTypeKind::CXType_UShort), das::LineInfo());
		addIEx("UInt", "CXType_UInt", int64_t(CXTypeKind::CXType_UInt), das::LineInfo());
		addIEx("ULong", "CXType_ULong", int64_t(CXTypeKind::CXType_ULong), das::LineInfo());
		addIEx("ULongLong", "CXType_ULongLong", int64_t(CXTypeKind::CXType_ULongLong), das::LineInfo());
		addIEx("UInt128", "CXType_UInt128", int64_t(CXTypeKind::CXType_UInt128), das::LineInfo());
		addIEx("Char_S", "CXType_Char_S", int64_t(CXTypeKind::CXType_Char_S), das::LineInfo());
		addIEx("SChar", "CXType_SChar", int64_t(CXTypeKind::CXType_SChar), das::LineInfo());
		addIEx("WChar", "CXType_WChar", int64_t(CXTypeKind::CXType_WChar), das::LineInfo());
		addIEx("Short", "CXType_Short", int64_t(CXTypeKind::CXType_Short), das::LineInfo());
		addIEx("Int", "CXType_Int", int64_t(CXTypeKind::CXType_Int), das::LineInfo());
		addIEx("Long", "CXType_Long", int64_t(CXTypeKind::CXType_Long), das::LineInfo());
		addIEx("LongLong", "CXType_LongLong", int64_t(CXTypeKind::CXType_LongLong), das::LineInfo());
		addIEx("Int128", "CXType_Int128", int64_t(CXTypeKind::CXType_Int128), das::LineInfo());
		addIEx("Float", "CXType_Float", int64_t(CXTypeKind::CXType_Float), das::LineInfo());
		addIEx("Double", "CXType_Double", int64_t(CXTypeKind::CXType_Double), das::LineInfo());
		addIEx("LongDouble", "CXType_LongDouble", int64_t(CXTypeKind::CXType_LongDouble), das::LineInfo());
		addIEx("NullPtr", "CXType_NullPtr", int64_t(CXTypeKind::CXType_NullPtr), das::LineInfo());
		addIEx("Overload", "CXType_Overload", int64_t(CXTypeKind::CXType_Overload), das::LineInfo());
		addIEx("Dependent", "CXType_Dependent", int64_t(CXTypeKind::CXType_Dependent), das::LineInfo());
		addIEx("ObjCId", "CXType_ObjCId", int64_t(CXTypeKind::CXType_ObjCId), das::LineInfo());
		addIEx("ObjCClass", "CXType_ObjCClass", int64_t(CXTypeKind::CXType_ObjCClass), das::LineInfo());
		addIEx("ObjCSel", "CXType_ObjCSel", int64_t(CXTypeKind::CXType_ObjCSel), das::LineInfo());
		addIEx("Float128", "CXType_Float128", int64_t(CXTypeKind::CXType_Float128), das::LineInfo());
		addIEx("Half", "CXType_Half", int64_t(CXTypeKind::CXType_Half), das::LineInfo());
		addIEx("Float16", "CXType_Float16", int64_t(CXTypeKind::CXType_Float16), das::LineInfo());
		addIEx("ShortAccum", "CXType_ShortAccum", int64_t(CXTypeKind::CXType_ShortAccum), das::LineInfo());
		addIEx("Accum", "CXType_Accum", int64_t(CXTypeKind::CXType_Accum), das::LineInfo());
		addIEx("LongAccum", "CXType_LongAccum", int64_t(CXTypeKind::CXType_LongAccum), das::LineInfo());
		addIEx("UShortAccum", "CXType_UShortAccum", int64_t(CXTypeKind::CXType_UShortAccum), das::LineInfo());
		addIEx("UAccum", "CXType_UAccum", int64_t(CXTypeKind::CXType_UAccum), das::LineInfo());
		addIEx("ULongAccum", "CXType_ULongAccum", int64_t(CXTypeKind::CXType_ULongAccum), das::LineInfo());
		addIEx("BFloat16", "CXType_BFloat16", int64_t(CXTypeKind::CXType_BFloat16), das::LineInfo());
		addIEx("FirstBuiltin", "CXType_FirstBuiltin", int64_t(CXTypeKind::CXType_FirstBuiltin), das::LineInfo());
		addIEx("LastBuiltin", "CXType_LastBuiltin", int64_t(CXTypeKind::CXType_LastBuiltin), das::LineInfo());
		addIEx("Complex", "CXType_Complex", int64_t(CXTypeKind::CXType_Complex), das::LineInfo());
		addIEx("Pointer", "CXType_Pointer", int64_t(CXTypeKind::CXType_Pointer), das::LineInfo());
		addIEx("BlockPointer", "CXType_BlockPointer", int64_t(CXTypeKind::CXType_BlockPointer), das::LineInfo());
		addIEx("LValueReference", "CXType_LValueReference", int64_t(CXTypeKind::CXType_LValueReference), das::LineInfo());
		addIEx("RValueReference", "CXType_RValueReference", int64_t(CXTypeKind::CXType_RValueReference), das::LineInfo());
		addIEx("Record", "CXType_Record", int64_t(CXTypeKind::CXType_Record), das::LineInfo());
		addIEx("Enum", "CXType_Enum", int64_t(CXTypeKind::CXType_Enum), das::LineInfo());
		addIEx("Typedef", "CXType_Typedef", int64_t(CXTypeKind::CXType_Typedef), das::LineInfo());
		addIEx("ObjCInterface", "CXType_ObjCInterface", int64_t(CXTypeKind::CXType_ObjCInterface), das::LineInfo());
		addIEx("ObjCObjectPointer", "CXType_ObjCObjectPointer", int64_t(CXTypeKind::CXType_ObjCObjectPointer), das::LineInfo());
		addIEx("FunctionNoProto", "CXType_FunctionNoProto", int64_t(CXTypeKind::CXType_FunctionNoProto), das::LineInfo());
		addIEx("FunctionProto", "CXType_FunctionProto", int64_t(CXTypeKind::CXType_FunctionProto), das::LineInfo());
		addIEx("ConstantArray", "CXType_ConstantArray", int64_t(CXTypeKind::CXType_ConstantArray), das::LineInfo());
		addIEx("Vector", "CXType_Vector", int64_t(CXTypeKind::CXType_Vector), das::LineInfo());
		addIEx("IncompleteArray", "CXType_IncompleteArray", int64_t(CXTypeKind::CXType_IncompleteArray), das::LineInfo());
		addIEx("VariableArray", "CXType_VariableArray", int64_t(CXTypeKind::CXType_VariableArray), das::LineInfo());
		addIEx("DependentSizedArray", "CXType_DependentSizedArray", int64_t(CXTypeKind::CXType_DependentSizedArray), das::LineInfo());
		addIEx("MemberPointer", "CXType_MemberPointer", int64_t(CXTypeKind::CXType_MemberPointer), das::LineInfo());
		addIEx("Auto", "CXType_Auto", int64_t(CXTypeKind::CXType_Auto), das::LineInfo());
		addIEx("Elaborated", "CXType_Elaborated", int64_t(CXTypeKind::CXType_Elaborated), das::LineInfo());
		addIEx("Pipe", "CXType_Pipe", int64_t(CXTypeKind::CXType_Pipe), das::LineInfo());
		addIEx("OCLImage1dRO", "CXType_OCLImage1dRO", int64_t(CXTypeKind::CXType_OCLImage1dRO), das::LineInfo());
		addIEx("OCLImage1dArrayRO", "CXType_OCLImage1dArrayRO", int64_t(CXTypeKind::CXType_OCLImage1dArrayRO), das::LineInfo());
		addIEx("OCLImage1dBufferRO", "CXType_OCLImage1dBufferRO", int64_t(CXTypeKind::CXType_OCLImage1dBufferRO), das::LineInfo());
		addIEx("OCLImage2dRO", "CXType_OCLImage2dRO", int64_t(CXTypeKind::CXType_OCLImage2dRO), das::LineInfo());
		addIEx("OCLImage2dArrayRO", "CXType_OCLImage2dArrayRO", int64_t(CXTypeKind::CXType_OCLImage2dArrayRO), das::LineInfo());
		addIEx("OCLImage2dDepthRO", "CXType_OCLImage2dDepthRO", int64_t(CXTypeKind::CXType_OCLImage2dDepthRO), das::LineInfo());
		addIEx("OCLImage2dArrayDepthRO", "CXType_OCLImage2dArrayDepthRO", int64_t(CXTypeKind::CXType_OCLImage2dArrayDepthRO), das::LineInfo());
		addIEx("OCLImage2dMSAARO", "CXType_OCLImage2dMSAARO", int64_t(CXTypeKind::CXType_OCLImage2dMSAARO), das::LineInfo());
		addIEx("OCLImage2dArrayMSAARO", "CXType_OCLImage2dArrayMSAARO", int64_t(CXTypeKind::CXType_OCLImage2dArrayMSAARO), das::LineInfo());
		addIEx("OCLImage2dMSAADepthRO", "CXType_OCLImage2dMSAADepthRO", int64_t(CXTypeKind::CXType_OCLImage2dMSAADepthRO), das::LineInfo());
		addIEx("OCLImage2dArrayMSAADepthRO", "CXType_OCLImage2dArrayMSAADepthRO", int64_t(CXTypeKind::CXType_OCLImage2dArrayMSAADepthRO), das::LineInfo());
		addIEx("OCLImage3dRO", "CXType_OCLImage3dRO", int64_t(CXTypeKind::CXType_OCLImage3dRO), das::LineInfo());
		addIEx("OCLImage1dWO", "CXType_OCLImage1dWO", int64_t(CXTypeKind::CXType_OCLImage1dWO), das::LineInfo());
		addIEx("OCLImage1dArrayWO", "CXType_OCLImage1dArrayWO", int64_t(CXTypeKind::CXType_OCLImage1dArrayWO), das::LineInfo());
		addIEx("OCLImage1dBufferWO", "CXType_OCLImage1dBufferWO", int64_t(CXTypeKind::CXType_OCLImage1dBufferWO), das::LineInfo());
		addIEx("OCLImage2dWO", "CXType_OCLImage2dWO", int64_t(CXTypeKind::CXType_OCLImage2dWO), das::LineInfo());
		addIEx("OCLImage2dArrayWO", "CXType_OCLImage2dArrayWO", int64_t(CXTypeKind::CXType_OCLImage2dArrayWO), das::LineInfo());
		addIEx("OCLImage2dDepthWO", "CXType_OCLImage2dDepthWO", int64_t(CXTypeKind::CXType_OCLImage2dDepthWO), das::LineInfo());
		addIEx("OCLImage2dArrayDepthWO", "CXType_OCLImage2dArrayDepthWO", int64_t(CXTypeKind::CXType_OCLImage2dArrayDepthWO), das::LineInfo());
		addIEx("OCLImage2dMSAAWO", "CXType_OCLImage2dMSAAWO", int64_t(CXTypeKind::CXType_OCLImage2dMSAAWO), das::LineInfo());
		addIEx("OCLImage2dArrayMSAAWO", "CXType_OCLImage2dArrayMSAAWO", int64_t(CXTypeKind::CXType_OCLImage2dArrayMSAAWO), das::LineInfo());
		addIEx("OCLImage2dMSAADepthWO", "CXType_OCLImage2dMSAADepthWO", int64_t(CXTypeKind::CXType_OCLImage2dMSAADepthWO), das::LineInfo());
		addIEx("OCLImage2dArrayMSAADepthWO", "CXType_OCLImage2dArrayMSAADepthWO", int64_t(CXTypeKind::CXType_OCLImage2dArrayMSAADepthWO), das::LineInfo());
		addIEx("OCLImage3dWO", "CXType_OCLImage3dWO", int64_t(CXTypeKind::CXType_OCLImage3dWO), das::LineInfo());
		addIEx("OCLImage1dRW", "CXType_OCLImage1dRW", int64_t(CXTypeKind::CXType_OCLImage1dRW), das::LineInfo());
		addIEx("OCLImage1dArrayRW", "CXType_OCLImage1dArrayRW", int64_t(CXTypeKind::CXType_OCLImage1dArrayRW), das::LineInfo());
		addIEx("OCLImage1dBufferRW", "CXType_OCLImage1dBufferRW", int64_t(CXTypeKind::CXType_OCLImage1dBufferRW), das::LineInfo());
		addIEx("OCLImage2dRW", "CXType_OCLImage2dRW", int64_t(CXTypeKind::CXType_OCLImage2dRW), das::LineInfo());
		addIEx("OCLImage2dArrayRW", "CXType_OCLImage2dArrayRW", int64_t(CXTypeKind::CXType_OCLImage2dArrayRW), das::LineInfo());
		addIEx("OCLImage2dDepthRW", "CXType_OCLImage2dDepthRW", int64_t(CXTypeKind::CXType_OCLImage2dDepthRW), das::LineInfo());
		addIEx("OCLImage2dArrayDepthRW", "CXType_OCLImage2dArrayDepthRW", int64_t(CXTypeKind::CXType_OCLImage2dArrayDepthRW), das::LineInfo());
		addIEx("OCLImage2dMSAARW", "CXType_OCLImage2dMSAARW", int64_t(CXTypeKind::CXType_OCLImage2dMSAARW), das::LineInfo());
		addIEx("OCLImage2dArrayMSAARW", "CXType_OCLImage2dArrayMSAARW", int64_t(CXTypeKind::CXType_OCLImage2dArrayMSAARW), das::LineInfo());
		addIEx("OCLImage2dMSAADepthRW", "CXType_OCLImage2dMSAADepthRW", int64_t(CXTypeKind::CXType_OCLImage2dMSAADepthRW), das::LineInfo());
		addIEx("OCLImage2dArrayMSAADepthRW", "CXType_OCLImage2dArrayMSAADepthRW", int64_t(CXTypeKind::CXType_OCLImage2dArrayMSAADepthRW), das::LineInfo());
		addIEx("OCLImage3dRW", "CXType_OCLImage3dRW", int64_t(CXTypeKind::CXType_OCLImage3dRW), das::LineInfo());
		addIEx("OCLSampler", "CXType_OCLSampler", int64_t(CXTypeKind::CXType_OCLSampler), das::LineInfo());
		addIEx("OCLEvent", "CXType_OCLEvent", int64_t(CXTypeKind::CXType_OCLEvent), das::LineInfo());
		addIEx("OCLQueue", "CXType_OCLQueue", int64_t(CXTypeKind::CXType_OCLQueue), das::LineInfo());
		addIEx("OCLReserveID", "CXType_OCLReserveID", int64_t(CXTypeKind::CXType_OCLReserveID), das::LineInfo());
		addIEx("ObjCObject", "CXType_ObjCObject", int64_t(CXTypeKind::CXType_ObjCObject), das::LineInfo());
		addIEx("ObjCTypeParam", "CXType_ObjCTypeParam", int64_t(CXTypeKind::CXType_ObjCTypeParam), das::LineInfo());
		addIEx("Attributed", "CXType_Attributed", int64_t(CXTypeKind::CXType_Attributed), das::LineInfo());
		addIEx("OCLIntelSubgroupAVCMcePayload", "CXType_OCLIntelSubgroupAVCMcePayload", int64_t(CXTypeKind::CXType_OCLIntelSubgroupAVCMcePayload), das::LineInfo());
		addIEx("OCLIntelSubgroupAVCImePayload", "CXType_OCLIntelSubgroupAVCImePayload", int64_t(CXTypeKind::CXType_OCLIntelSubgroupAVCImePayload), das::LineInfo());
		addIEx("OCLIntelSubgroupAVCRefPayload", "CXType_OCLIntelSubgroupAVCRefPayload", int64_t(CXTypeKind::CXType_OCLIntelSubgroupAVCRefPayload), das::LineInfo());
		addIEx("OCLIntelSubgroupAVCSicPayload", "CXType_OCLIntelSubgroupAVCSicPayload", int64_t(CXTypeKind::CXType_OCLIntelSubgroupAVCSicPayload), das::LineInfo());
		addIEx("OCLIntelSubgroupAVCMceResult", "CXType_OCLIntelSubgroupAVCMceResult", int64_t(CXTypeKind::CXType_OCLIntelSubgroupAVCMceResult), das::LineInfo());
		addIEx("OCLIntelSubgroupAVCImeResult", "CXType_OCLIntelSubgroupAVCImeResult", int64_t(CXTypeKind::CXType_OCLIntelSubgroupAVCImeResult), das::LineInfo());
		addIEx("OCLIntelSubgroupAVCRefResult", "CXType_OCLIntelSubgroupAVCRefResult", int64_t(CXTypeKind::CXType_OCLIntelSubgroupAVCRefResult), das::LineInfo());
		addIEx("OCLIntelSubgroupAVCSicResult", "CXType_OCLIntelSubgroupAVCSicResult", int64_t(CXTypeKind::CXType_OCLIntelSubgroupAVCSicResult), das::LineInfo());
		addIEx("OCLIntelSubgroupAVCImeResultSingleRefStreamout", "CXType_OCLIntelSubgroupAVCImeResultSingleRefStreamout", int64_t(CXTypeKind::CXType_OCLIntelSubgroupAVCImeResultSingleRefStreamout), das::LineInfo());
		addIEx("OCLIntelSubgroupAVCImeResultDualRefStreamout", "CXType_OCLIntelSubgroupAVCImeResultDualRefStreamout", int64_t(CXTypeKind::CXType_OCLIntelSubgroupAVCImeResultDualRefStreamout), das::LineInfo());
		addIEx("OCLIntelSubgroupAVCImeSingleRefStreamin", "CXType_OCLIntelSubgroupAVCImeSingleRefStreamin", int64_t(CXTypeKind::CXType_OCLIntelSubgroupAVCImeSingleRefStreamin), das::LineInfo());
		addIEx("OCLIntelSubgroupAVCImeDualRefStreamin", "CXType_OCLIntelSubgroupAVCImeDualRefStreamin", int64_t(CXTypeKind::CXType_OCLIntelSubgroupAVCImeDualRefStreamin), das::LineInfo());
		addIEx("ExtVector", "CXType_ExtVector", int64_t(CXTypeKind::CXType_ExtVector), das::LineInfo());
		addIEx("Atomic", "CXType_Atomic", int64_t(CXTypeKind::CXType_Atomic), das::LineInfo());
	}
};

class Enumeration_CXCallingConv : public das::Enumeration {
public:
	Enumeration_CXCallingConv() : das::Enumeration("CXCallingConv") {
		external = true;
		cppName = "CXCallingConv";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXCallingConv >::type >::type;
		addIEx("Default", "CXCallingConv_Default", int64_t(CXCallingConv::CXCallingConv_Default), das::LineInfo());
		addIEx("C", "CXCallingConv_C", int64_t(CXCallingConv::CXCallingConv_C), das::LineInfo());
		addIEx("X86StdCall", "CXCallingConv_X86StdCall", int64_t(CXCallingConv::CXCallingConv_X86StdCall), das::LineInfo());
		addIEx("X86FastCall", "CXCallingConv_X86FastCall", int64_t(CXCallingConv::CXCallingConv_X86FastCall), das::LineInfo());
		addIEx("X86ThisCall", "CXCallingConv_X86ThisCall", int64_t(CXCallingConv::CXCallingConv_X86ThisCall), das::LineInfo());
		addIEx("X86Pascal", "CXCallingConv_X86Pascal", int64_t(CXCallingConv::CXCallingConv_X86Pascal), das::LineInfo());
		addIEx("AAPCS", "CXCallingConv_AAPCS", int64_t(CXCallingConv::CXCallingConv_AAPCS), das::LineInfo());
		addIEx("AAPCS_VFP", "CXCallingConv_AAPCS_VFP", int64_t(CXCallingConv::CXCallingConv_AAPCS_VFP), das::LineInfo());
		addIEx("X86RegCall", "CXCallingConv_X86RegCall", int64_t(CXCallingConv::CXCallingConv_X86RegCall), das::LineInfo());
		addIEx("IntelOclBicc", "CXCallingConv_IntelOclBicc", int64_t(CXCallingConv::CXCallingConv_IntelOclBicc), das::LineInfo());
		addIEx("Win64", "CXCallingConv_Win64", int64_t(CXCallingConv::CXCallingConv_Win64), das::LineInfo());
		addIEx("X86_64Win64", "CXCallingConv_X86_64Win64", int64_t(CXCallingConv::CXCallingConv_X86_64Win64), das::LineInfo());
		addIEx("X86_64SysV", "CXCallingConv_X86_64SysV", int64_t(CXCallingConv::CXCallingConv_X86_64SysV), das::LineInfo());
		addIEx("X86VectorCall", "CXCallingConv_X86VectorCall", int64_t(CXCallingConv::CXCallingConv_X86VectorCall), das::LineInfo());
		addIEx("Swift", "CXCallingConv_Swift", int64_t(CXCallingConv::CXCallingConv_Swift), das::LineInfo());
		addIEx("PreserveMost", "CXCallingConv_PreserveMost", int64_t(CXCallingConv::CXCallingConv_PreserveMost), das::LineInfo());
		addIEx("PreserveAll", "CXCallingConv_PreserveAll", int64_t(CXCallingConv::CXCallingConv_PreserveAll), das::LineInfo());
		addIEx("AArch64VectorCall", "CXCallingConv_AArch64VectorCall", int64_t(CXCallingConv::CXCallingConv_AArch64VectorCall), das::LineInfo());
		addIEx("SwiftAsync", "CXCallingConv_SwiftAsync", int64_t(CXCallingConv::CXCallingConv_SwiftAsync), das::LineInfo());
		addIEx("Invalid", "CXCallingConv_Invalid", int64_t(CXCallingConv::CXCallingConv_Invalid), das::LineInfo());
		addIEx("Unexposed", "CXCallingConv_Unexposed", int64_t(CXCallingConv::CXCallingConv_Unexposed), das::LineInfo());
	}
};

class Enumeration_CXTemplateArgumentKind : public das::Enumeration {
public:
	Enumeration_CXTemplateArgumentKind() : das::Enumeration("CXTemplateArgumentKind") {
		external = true;
		cppName = "CXTemplateArgumentKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXTemplateArgumentKind >::type >::type;
		addIEx("Null", "CXTemplateArgumentKind_Null", int64_t(CXTemplateArgumentKind::CXTemplateArgumentKind_Null), das::LineInfo());
		addIEx("Type", "CXTemplateArgumentKind_Type", int64_t(CXTemplateArgumentKind::CXTemplateArgumentKind_Type), das::LineInfo());
		addIEx("Declaration", "CXTemplateArgumentKind_Declaration", int64_t(CXTemplateArgumentKind::CXTemplateArgumentKind_Declaration), das::LineInfo());
		addIEx("NullPtr", "CXTemplateArgumentKind_NullPtr", int64_t(CXTemplateArgumentKind::CXTemplateArgumentKind_NullPtr), das::LineInfo());
		addIEx("Integral", "CXTemplateArgumentKind_Integral", int64_t(CXTemplateArgumentKind::CXTemplateArgumentKind_Integral), das::LineInfo());
		addIEx("Template", "CXTemplateArgumentKind_Template", int64_t(CXTemplateArgumentKind::CXTemplateArgumentKind_Template), das::LineInfo());
		addIEx("TemplateExpansion", "CXTemplateArgumentKind_TemplateExpansion", int64_t(CXTemplateArgumentKind::CXTemplateArgumentKind_TemplateExpansion), das::LineInfo());
		addIEx("Expression", "CXTemplateArgumentKind_Expression", int64_t(CXTemplateArgumentKind::CXTemplateArgumentKind_Expression), das::LineInfo());
		addIEx("Pack", "CXTemplateArgumentKind_Pack", int64_t(CXTemplateArgumentKind::CXTemplateArgumentKind_Pack), das::LineInfo());
		addIEx("Invalid", "CXTemplateArgumentKind_Invalid", int64_t(CXTemplateArgumentKind::CXTemplateArgumentKind_Invalid), das::LineInfo());
	}
};

class Enumeration_CXTypeNullabilityKind : public das::Enumeration {
public:
	Enumeration_CXTypeNullabilityKind() : das::Enumeration("CXTypeNullabilityKind") {
		external = true;
		cppName = "CXTypeNullabilityKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXTypeNullabilityKind >::type >::type;
		addIEx("CXTypeNullability_NonNull", "CXTypeNullability_NonNull", int64_t(CXTypeNullabilityKind::CXTypeNullability_NonNull), das::LineInfo());
		addIEx("CXTypeNullability_Nullable", "CXTypeNullability_Nullable", int64_t(CXTypeNullabilityKind::CXTypeNullability_Nullable), das::LineInfo());
		addIEx("CXTypeNullability_Unspecified", "CXTypeNullability_Unspecified", int64_t(CXTypeNullabilityKind::CXTypeNullability_Unspecified), das::LineInfo());
		addIEx("CXTypeNullability_Invalid", "CXTypeNullability_Invalid", int64_t(CXTypeNullabilityKind::CXTypeNullability_Invalid), das::LineInfo());
		addIEx("CXTypeNullability_NullableResult", "CXTypeNullability_NullableResult", int64_t(CXTypeNullabilityKind::CXTypeNullability_NullableResult), das::LineInfo());
	}
};

class Enumeration_CXTypeLayoutError : public das::Enumeration {
public:
	Enumeration_CXTypeLayoutError() : das::Enumeration("CXTypeLayoutError") {
		external = true;
		cppName = "CXTypeLayoutError";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXTypeLayoutError >::type >::type;
		addIEx("Invalid", "CXTypeLayoutError_Invalid", int64_t(CXTypeLayoutError::CXTypeLayoutError_Invalid), das::LineInfo());
		addIEx("Incomplete", "CXTypeLayoutError_Incomplete", int64_t(CXTypeLayoutError::CXTypeLayoutError_Incomplete), das::LineInfo());
		addIEx("Dependent", "CXTypeLayoutError_Dependent", int64_t(CXTypeLayoutError::CXTypeLayoutError_Dependent), das::LineInfo());
		addIEx("NotConstantSize", "CXTypeLayoutError_NotConstantSize", int64_t(CXTypeLayoutError::CXTypeLayoutError_NotConstantSize), das::LineInfo());
		addIEx("InvalidFieldName", "CXTypeLayoutError_InvalidFieldName", int64_t(CXTypeLayoutError::CXTypeLayoutError_InvalidFieldName), das::LineInfo());
		addIEx("Undeduced", "CXTypeLayoutError_Undeduced", int64_t(CXTypeLayoutError::CXTypeLayoutError_Undeduced), das::LineInfo());
	}
};

class Enumeration_CXRefQualifierKind : public das::Enumeration {
public:
	Enumeration_CXRefQualifierKind() : das::Enumeration("CXRefQualifierKind") {
		external = true;
		cppName = "CXRefQualifierKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXRefQualifierKind >::type >::type;
		addIEx("CXRefQualifier_None", "CXRefQualifier_None", int64_t(CXRefQualifierKind::CXRefQualifier_None), das::LineInfo());
		addIEx("CXRefQualifier_LValue", "CXRefQualifier_LValue", int64_t(CXRefQualifierKind::CXRefQualifier_LValue), das::LineInfo());
		addIEx("CXRefQualifier_RValue", "CXRefQualifier_RValue", int64_t(CXRefQualifierKind::CXRefQualifier_RValue), das::LineInfo());
	}
};

class Enumeration_CX_CXXAccessSpecifier : public das::Enumeration {
public:
	Enumeration_CX_CXXAccessSpecifier() : das::Enumeration("CX_CXXAccessSpecifier") {
		external = true;
		cppName = "CX_CXXAccessSpecifier";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CX_CXXAccessSpecifier >::type >::type;
		addIEx("CX_CXXInvalidAccessSpecifier", "CX_CXXInvalidAccessSpecifier", int64_t(CX_CXXAccessSpecifier::CX_CXXInvalidAccessSpecifier), das::LineInfo());
		addIEx("CX_CXXPublic", "CX_CXXPublic", int64_t(CX_CXXAccessSpecifier::CX_CXXPublic), das::LineInfo());
		addIEx("CX_CXXProtected", "CX_CXXProtected", int64_t(CX_CXXAccessSpecifier::CX_CXXProtected), das::LineInfo());
		addIEx("CX_CXXPrivate", "CX_CXXPrivate", int64_t(CX_CXXAccessSpecifier::CX_CXXPrivate), das::LineInfo());
	}
};

class Enumeration_CX_StorageClass : public das::Enumeration {
public:
	Enumeration_CX_StorageClass() : das::Enumeration("CX_StorageClass") {
		external = true;
		cppName = "CX_StorageClass";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CX_StorageClass >::type >::type;
		addIEx("CX_SC_Invalid", "CX_SC_Invalid", int64_t(CX_StorageClass::CX_SC_Invalid), das::LineInfo());
		addIEx("CX_SC_None", "CX_SC_None", int64_t(CX_StorageClass::CX_SC_None), das::LineInfo());
		addIEx("CX_SC_Extern", "CX_SC_Extern", int64_t(CX_StorageClass::CX_SC_Extern), das::LineInfo());
		addIEx("CX_SC_Static", "CX_SC_Static", int64_t(CX_StorageClass::CX_SC_Static), das::LineInfo());
		addIEx("CX_SC_PrivateExtern", "CX_SC_PrivateExtern", int64_t(CX_StorageClass::CX_SC_PrivateExtern), das::LineInfo());
		addIEx("CX_SC_OpenCLWorkGroupLocal", "CX_SC_OpenCLWorkGroupLocal", int64_t(CX_StorageClass::CX_SC_OpenCLWorkGroupLocal), das::LineInfo());
		addIEx("CX_SC_Auto", "CX_SC_Auto", int64_t(CX_StorageClass::CX_SC_Auto), das::LineInfo());
		addIEx("CX_SC_Register", "CX_SC_Register", int64_t(CX_StorageClass::CX_SC_Register), das::LineInfo());
	}
};

class Enumeration_CXChildVisitResult : public das::Enumeration {
public:
	Enumeration_CXChildVisitResult() : das::Enumeration("CXChildVisitResult") {
		external = true;
		cppName = "CXChildVisitResult";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXChildVisitResult >::type >::type;
		addIEx("Break", "CXChildVisit_Break", int64_t(CXChildVisitResult::CXChildVisit_Break), das::LineInfo());
		addIEx("Continue", "CXChildVisit_Continue", int64_t(CXChildVisitResult::CXChildVisit_Continue), das::LineInfo());
		addIEx("Recurse", "CXChildVisit_Recurse", int64_t(CXChildVisitResult::CXChildVisit_Recurse), das::LineInfo());
	}
};

class Enumeration_CXPrintingPolicyProperty : public das::Enumeration {
public:
	Enumeration_CXPrintingPolicyProperty() : das::Enumeration("CXPrintingPolicyProperty") {
		external = true;
		cppName = "CXPrintingPolicyProperty";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXPrintingPolicyProperty >::type >::type;
		addIEx("CXPrintingPolicy_Indentation", "CXPrintingPolicy_Indentation", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_Indentation), das::LineInfo());
		addIEx("CXPrintingPolicy_SuppressSpecifiers", "CXPrintingPolicy_SuppressSpecifiers", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_SuppressSpecifiers), das::LineInfo());
		addIEx("CXPrintingPolicy_SuppressTagKeyword", "CXPrintingPolicy_SuppressTagKeyword", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_SuppressTagKeyword), das::LineInfo());
		addIEx("CXPrintingPolicy_IncludeTagDefinition", "CXPrintingPolicy_IncludeTagDefinition", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_IncludeTagDefinition), das::LineInfo());
		addIEx("CXPrintingPolicy_SuppressScope", "CXPrintingPolicy_SuppressScope", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_SuppressScope), das::LineInfo());
		addIEx("CXPrintingPolicy_SuppressUnwrittenScope", "CXPrintingPolicy_SuppressUnwrittenScope", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_SuppressUnwrittenScope), das::LineInfo());
		addIEx("CXPrintingPolicy_SuppressInitializers", "CXPrintingPolicy_SuppressInitializers", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_SuppressInitializers), das::LineInfo());
		addIEx("CXPrintingPolicy_ConstantArraySizeAsWritten", "CXPrintingPolicy_ConstantArraySizeAsWritten", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_ConstantArraySizeAsWritten), das::LineInfo());
		addIEx("CXPrintingPolicy_AnonymousTagLocations", "CXPrintingPolicy_AnonymousTagLocations", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_AnonymousTagLocations), das::LineInfo());
		addIEx("CXPrintingPolicy_SuppressStrongLifetime", "CXPrintingPolicy_SuppressStrongLifetime", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_SuppressStrongLifetime), das::LineInfo());
		addIEx("CXPrintingPolicy_SuppressLifetimeQualifiers", "CXPrintingPolicy_SuppressLifetimeQualifiers", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_SuppressLifetimeQualifiers), das::LineInfo());
		addIEx("CXPrintingPolicy_SuppressTemplateArgsInCXXConstructors", "CXPrintingPolicy_SuppressTemplateArgsInCXXConstructors", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_SuppressTemplateArgsInCXXConstructors), das::LineInfo());
		addIEx("CXPrintingPolicy_Bool", "CXPrintingPolicy_Bool", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_Bool), das::LineInfo());
		addIEx("CXPrintingPolicy_Restrict", "CXPrintingPolicy_Restrict", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_Restrict), das::LineInfo());
		addIEx("CXPrintingPolicy_Alignof", "CXPrintingPolicy_Alignof", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_Alignof), das::LineInfo());
		addIEx("CXPrintingPolicy_UnderscoreAlignof", "CXPrintingPolicy_UnderscoreAlignof", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_UnderscoreAlignof), das::LineInfo());
		addIEx("CXPrintingPolicy_UseVoidForZeroParams", "CXPrintingPolicy_UseVoidForZeroParams", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_UseVoidForZeroParams), das::LineInfo());
		addIEx("CXPrintingPolicy_TerseOutput", "CXPrintingPolicy_TerseOutput", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_TerseOutput), das::LineInfo());
		addIEx("CXPrintingPolicy_PolishForDeclaration", "CXPrintingPolicy_PolishForDeclaration", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_PolishForDeclaration), das::LineInfo());
		addIEx("CXPrintingPolicy_Half", "CXPrintingPolicy_Half", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_Half), das::LineInfo());
		addIEx("CXPrintingPolicy_MSWChar", "CXPrintingPolicy_MSWChar", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_MSWChar), das::LineInfo());
		addIEx("CXPrintingPolicy_IncludeNewlines", "CXPrintingPolicy_IncludeNewlines", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_IncludeNewlines), das::LineInfo());
		addIEx("CXPrintingPolicy_MSVCFormatting", "CXPrintingPolicy_MSVCFormatting", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_MSVCFormatting), das::LineInfo());
		addIEx("CXPrintingPolicy_ConstantsAsWritten", "CXPrintingPolicy_ConstantsAsWritten", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_ConstantsAsWritten), das::LineInfo());
		addIEx("CXPrintingPolicy_SuppressImplicitBase", "CXPrintingPolicy_SuppressImplicitBase", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_SuppressImplicitBase), das::LineInfo());
		addIEx("CXPrintingPolicy_FullyQualifiedName", "CXPrintingPolicy_FullyQualifiedName", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_FullyQualifiedName), das::LineInfo());
		addIEx("CXPrintingPolicy_LastProperty", "CXPrintingPolicy_LastProperty", int64_t(CXPrintingPolicyProperty::CXPrintingPolicy_LastProperty), das::LineInfo());
	}
};

class Enumeration_CXObjCPropertyAttrKind : public das::Enumeration {
public:
	Enumeration_CXObjCPropertyAttrKind() : das::Enumeration("CXObjCPropertyAttrKind") {
		external = true;
		cppName = "CXObjCPropertyAttrKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXObjCPropertyAttrKind >::type >::type;
		addIEx("CXObjCPropertyAttr_noattr", "CXObjCPropertyAttr_noattr", int64_t(CXObjCPropertyAttrKind::CXObjCPropertyAttr_noattr), das::LineInfo());
		addIEx("CXObjCPropertyAttr_readonly", "CXObjCPropertyAttr_readonly", int64_t(CXObjCPropertyAttrKind::CXObjCPropertyAttr_readonly), das::LineInfo());
		addIEx("CXObjCPropertyAttr_getter", "CXObjCPropertyAttr_getter", int64_t(CXObjCPropertyAttrKind::CXObjCPropertyAttr_getter), das::LineInfo());
		addIEx("CXObjCPropertyAttr_assign", "CXObjCPropertyAttr_assign", int64_t(CXObjCPropertyAttrKind::CXObjCPropertyAttr_assign), das::LineInfo());
		addIEx("CXObjCPropertyAttr_readwrite", "CXObjCPropertyAttr_readwrite", int64_t(CXObjCPropertyAttrKind::CXObjCPropertyAttr_readwrite), das::LineInfo());
		addIEx("CXObjCPropertyAttr_retain", "CXObjCPropertyAttr_retain", int64_t(CXObjCPropertyAttrKind::CXObjCPropertyAttr_retain), das::LineInfo());
		addIEx("CXObjCPropertyAttr_copy", "CXObjCPropertyAttr_copy", int64_t(CXObjCPropertyAttrKind::CXObjCPropertyAttr_copy), das::LineInfo());
		addIEx("CXObjCPropertyAttr_nonatomic", "CXObjCPropertyAttr_nonatomic", int64_t(CXObjCPropertyAttrKind::CXObjCPropertyAttr_nonatomic), das::LineInfo());
		addIEx("CXObjCPropertyAttr_setter", "CXObjCPropertyAttr_setter", int64_t(CXObjCPropertyAttrKind::CXObjCPropertyAttr_setter), das::LineInfo());
		addIEx("CXObjCPropertyAttr_atomic", "CXObjCPropertyAttr_atomic", int64_t(CXObjCPropertyAttrKind::CXObjCPropertyAttr_atomic), das::LineInfo());
		addIEx("CXObjCPropertyAttr_weak", "CXObjCPropertyAttr_weak", int64_t(CXObjCPropertyAttrKind::CXObjCPropertyAttr_weak), das::LineInfo());
		addIEx("CXObjCPropertyAttr_strong", "CXObjCPropertyAttr_strong", int64_t(CXObjCPropertyAttrKind::CXObjCPropertyAttr_strong), das::LineInfo());
		addIEx("CXObjCPropertyAttr_unsafe_unretained", "CXObjCPropertyAttr_unsafe_unretained", int64_t(CXObjCPropertyAttrKind::CXObjCPropertyAttr_unsafe_unretained), das::LineInfo());
		addIEx("CXObjCPropertyAttr_class", "CXObjCPropertyAttr_class", int64_t(CXObjCPropertyAttrKind::CXObjCPropertyAttr_class), das::LineInfo());
	}
};

class Enumeration_CXObjCDeclQualifierKind : public das::Enumeration {
public:
	Enumeration_CXObjCDeclQualifierKind() : das::Enumeration("CXObjCDeclQualifierKind") {
		external = true;
		cppName = "CXObjCDeclQualifierKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXObjCDeclQualifierKind >::type >::type;
		addIEx("CXObjCDeclQualifier_None", "CXObjCDeclQualifier_None", int64_t(CXObjCDeclQualifierKind::CXObjCDeclQualifier_None), das::LineInfo());
		addIEx("CXObjCDeclQualifier_In", "CXObjCDeclQualifier_In", int64_t(CXObjCDeclQualifierKind::CXObjCDeclQualifier_In), das::LineInfo());
		addIEx("CXObjCDeclQualifier_Inout", "CXObjCDeclQualifier_Inout", int64_t(CXObjCDeclQualifierKind::CXObjCDeclQualifier_Inout), das::LineInfo());
		addIEx("CXObjCDeclQualifier_Out", "CXObjCDeclQualifier_Out", int64_t(CXObjCDeclQualifierKind::CXObjCDeclQualifier_Out), das::LineInfo());
		addIEx("CXObjCDeclQualifier_Bycopy", "CXObjCDeclQualifier_Bycopy", int64_t(CXObjCDeclQualifierKind::CXObjCDeclQualifier_Bycopy), das::LineInfo());
		addIEx("CXObjCDeclQualifier_Byref", "CXObjCDeclQualifier_Byref", int64_t(CXObjCDeclQualifierKind::CXObjCDeclQualifier_Byref), das::LineInfo());
		addIEx("CXObjCDeclQualifier_Oneway", "CXObjCDeclQualifier_Oneway", int64_t(CXObjCDeclQualifierKind::CXObjCDeclQualifier_Oneway), das::LineInfo());
	}
};

class Enumeration_CXNameRefFlags : public das::Enumeration {
public:
	Enumeration_CXNameRefFlags() : das::Enumeration("CXNameRefFlags") {
		external = true;
		cppName = "CXNameRefFlags";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXNameRefFlags >::type >::type;
		addIEx("CXNameRange_WantQualifier", "CXNameRange_WantQualifier", int64_t(CXNameRefFlags::CXNameRange_WantQualifier), das::LineInfo());
		addIEx("CXNameRange_WantTemplateArgs", "CXNameRange_WantTemplateArgs", int64_t(CXNameRefFlags::CXNameRange_WantTemplateArgs), das::LineInfo());
		addIEx("CXNameRange_WantSinglePiece", "CXNameRange_WantSinglePiece", int64_t(CXNameRefFlags::CXNameRange_WantSinglePiece), das::LineInfo());
	}
};

class Enumeration_CXTokenKind : public das::Enumeration {
public:
	Enumeration_CXTokenKind() : das::Enumeration("CXTokenKind") {
		external = true;
		cppName = "CXTokenKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXTokenKind >::type >::type;
		addIEx("CXToken_Punctuation", "CXToken_Punctuation", int64_t(CXTokenKind::CXToken_Punctuation), das::LineInfo());
		addIEx("CXToken_Keyword", "CXToken_Keyword", int64_t(CXTokenKind::CXToken_Keyword), das::LineInfo());
		addIEx("CXToken_Identifier", "CXToken_Identifier", int64_t(CXTokenKind::CXToken_Identifier), das::LineInfo());
		addIEx("CXToken_Literal", "CXToken_Literal", int64_t(CXTokenKind::CXToken_Literal), das::LineInfo());
		addIEx("CXToken_Comment", "CXToken_Comment", int64_t(CXTokenKind::CXToken_Comment), das::LineInfo());
	}
};

class Enumeration_CXCompletionChunkKind : public das::Enumeration {
public:
	Enumeration_CXCompletionChunkKind() : das::Enumeration("CXCompletionChunkKind") {
		external = true;
		cppName = "CXCompletionChunkKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXCompletionChunkKind >::type >::type;
		addIEx("CXCompletionChunk_Optional", "CXCompletionChunk_Optional", int64_t(CXCompletionChunkKind::CXCompletionChunk_Optional), das::LineInfo());
		addIEx("CXCompletionChunk_TypedText", "CXCompletionChunk_TypedText", int64_t(CXCompletionChunkKind::CXCompletionChunk_TypedText), das::LineInfo());
		addIEx("CXCompletionChunk_Text", "CXCompletionChunk_Text", int64_t(CXCompletionChunkKind::CXCompletionChunk_Text), das::LineInfo());
		addIEx("CXCompletionChunk_Placeholder", "CXCompletionChunk_Placeholder", int64_t(CXCompletionChunkKind::CXCompletionChunk_Placeholder), das::LineInfo());
		addIEx("CXCompletionChunk_Informative", "CXCompletionChunk_Informative", int64_t(CXCompletionChunkKind::CXCompletionChunk_Informative), das::LineInfo());
		addIEx("CXCompletionChunk_CurrentParameter", "CXCompletionChunk_CurrentParameter", int64_t(CXCompletionChunkKind::CXCompletionChunk_CurrentParameter), das::LineInfo());
		addIEx("CXCompletionChunk_LeftParen", "CXCompletionChunk_LeftParen", int64_t(CXCompletionChunkKind::CXCompletionChunk_LeftParen), das::LineInfo());
		addIEx("CXCompletionChunk_RightParen", "CXCompletionChunk_RightParen", int64_t(CXCompletionChunkKind::CXCompletionChunk_RightParen), das::LineInfo());
		addIEx("CXCompletionChunk_LeftBracket", "CXCompletionChunk_LeftBracket", int64_t(CXCompletionChunkKind::CXCompletionChunk_LeftBracket), das::LineInfo());
		addIEx("CXCompletionChunk_RightBracket", "CXCompletionChunk_RightBracket", int64_t(CXCompletionChunkKind::CXCompletionChunk_RightBracket), das::LineInfo());
		addIEx("CXCompletionChunk_LeftBrace", "CXCompletionChunk_LeftBrace", int64_t(CXCompletionChunkKind::CXCompletionChunk_LeftBrace), das::LineInfo());
		addIEx("CXCompletionChunk_RightBrace", "CXCompletionChunk_RightBrace", int64_t(CXCompletionChunkKind::CXCompletionChunk_RightBrace), das::LineInfo());
		addIEx("CXCompletionChunk_LeftAngle", "CXCompletionChunk_LeftAngle", int64_t(CXCompletionChunkKind::CXCompletionChunk_LeftAngle), das::LineInfo());
		addIEx("CXCompletionChunk_RightAngle", "CXCompletionChunk_RightAngle", int64_t(CXCompletionChunkKind::CXCompletionChunk_RightAngle), das::LineInfo());
		addIEx("CXCompletionChunk_Comma", "CXCompletionChunk_Comma", int64_t(CXCompletionChunkKind::CXCompletionChunk_Comma), das::LineInfo());
		addIEx("CXCompletionChunk_ResultType", "CXCompletionChunk_ResultType", int64_t(CXCompletionChunkKind::CXCompletionChunk_ResultType), das::LineInfo());
		addIEx("CXCompletionChunk_Colon", "CXCompletionChunk_Colon", int64_t(CXCompletionChunkKind::CXCompletionChunk_Colon), das::LineInfo());
		addIEx("CXCompletionChunk_SemiColon", "CXCompletionChunk_SemiColon", int64_t(CXCompletionChunkKind::CXCompletionChunk_SemiColon), das::LineInfo());
		addIEx("CXCompletionChunk_Equal", "CXCompletionChunk_Equal", int64_t(CXCompletionChunkKind::CXCompletionChunk_Equal), das::LineInfo());
		addIEx("CXCompletionChunk_HorizontalSpace", "CXCompletionChunk_HorizontalSpace", int64_t(CXCompletionChunkKind::CXCompletionChunk_HorizontalSpace), das::LineInfo());
		addIEx("CXCompletionChunk_VerticalSpace", "CXCompletionChunk_VerticalSpace", int64_t(CXCompletionChunkKind::CXCompletionChunk_VerticalSpace), das::LineInfo());
	}
};

class Enumeration_CXCodeComplete_Flags : public das::Enumeration {
public:
	Enumeration_CXCodeComplete_Flags() : das::Enumeration("CXCodeComplete_Flags") {
		external = true;
		cppName = "CXCodeComplete_Flags";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXCodeComplete_Flags >::type >::type;
		addIEx("CXCodeComplete_IncludeMacros", "CXCodeComplete_IncludeMacros", int64_t(CXCodeComplete_Flags::CXCodeComplete_IncludeMacros), das::LineInfo());
		addIEx("CXCodeComplete_IncludeCodePatterns", "CXCodeComplete_IncludeCodePatterns", int64_t(CXCodeComplete_Flags::CXCodeComplete_IncludeCodePatterns), das::LineInfo());
		addIEx("CXCodeComplete_IncludeBriefComments", "CXCodeComplete_IncludeBriefComments", int64_t(CXCodeComplete_Flags::CXCodeComplete_IncludeBriefComments), das::LineInfo());
		addIEx("CXCodeComplete_SkipPreamble", "CXCodeComplete_SkipPreamble", int64_t(CXCodeComplete_Flags::CXCodeComplete_SkipPreamble), das::LineInfo());
		addIEx("CXCodeComplete_IncludeCompletionsWithFixIts", "CXCodeComplete_IncludeCompletionsWithFixIts", int64_t(CXCodeComplete_Flags::CXCodeComplete_IncludeCompletionsWithFixIts), das::LineInfo());
	}
};

class Enumeration_CXCompletionContext : public das::Enumeration {
public:
	Enumeration_CXCompletionContext() : das::Enumeration("CXCompletionContext") {
		external = true;
		cppName = "CXCompletionContext";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXCompletionContext >::type >::type;
		addIEx("Unexposed", "CXCompletionContext_Unexposed", int64_t(CXCompletionContext::CXCompletionContext_Unexposed), das::LineInfo());
		addIEx("AnyType", "CXCompletionContext_AnyType", int64_t(CXCompletionContext::CXCompletionContext_AnyType), das::LineInfo());
		addIEx("AnyValue", "CXCompletionContext_AnyValue", int64_t(CXCompletionContext::CXCompletionContext_AnyValue), das::LineInfo());
		addIEx("ObjCObjectValue", "CXCompletionContext_ObjCObjectValue", int64_t(CXCompletionContext::CXCompletionContext_ObjCObjectValue), das::LineInfo());
		addIEx("ObjCSelectorValue", "CXCompletionContext_ObjCSelectorValue", int64_t(CXCompletionContext::CXCompletionContext_ObjCSelectorValue), das::LineInfo());
		addIEx("CXXClassTypeValue", "CXCompletionContext_CXXClassTypeValue", int64_t(CXCompletionContext::CXCompletionContext_CXXClassTypeValue), das::LineInfo());
		addIEx("DotMemberAccess", "CXCompletionContext_DotMemberAccess", int64_t(CXCompletionContext::CXCompletionContext_DotMemberAccess), das::LineInfo());
		addIEx("ArrowMemberAccess", "CXCompletionContext_ArrowMemberAccess", int64_t(CXCompletionContext::CXCompletionContext_ArrowMemberAccess), das::LineInfo());
		addIEx("ObjCPropertyAccess", "CXCompletionContext_ObjCPropertyAccess", int64_t(CXCompletionContext::CXCompletionContext_ObjCPropertyAccess), das::LineInfo());
		addIEx("EnumTag", "CXCompletionContext_EnumTag", int64_t(CXCompletionContext::CXCompletionContext_EnumTag), das::LineInfo());
		addIEx("UnionTag", "CXCompletionContext_UnionTag", int64_t(CXCompletionContext::CXCompletionContext_UnionTag), das::LineInfo());
		addIEx("StructTag", "CXCompletionContext_StructTag", int64_t(CXCompletionContext::CXCompletionContext_StructTag), das::LineInfo());
		addIEx("ClassTag", "CXCompletionContext_ClassTag", int64_t(CXCompletionContext::CXCompletionContext_ClassTag), das::LineInfo());
		addIEx("Namespace", "CXCompletionContext_Namespace", int64_t(CXCompletionContext::CXCompletionContext_Namespace), das::LineInfo());
		addIEx("NestedNameSpecifier", "CXCompletionContext_NestedNameSpecifier", int64_t(CXCompletionContext::CXCompletionContext_NestedNameSpecifier), das::LineInfo());
		addIEx("ObjCInterface", "CXCompletionContext_ObjCInterface", int64_t(CXCompletionContext::CXCompletionContext_ObjCInterface), das::LineInfo());
		addIEx("ObjCProtocol", "CXCompletionContext_ObjCProtocol", int64_t(CXCompletionContext::CXCompletionContext_ObjCProtocol), das::LineInfo());
		addIEx("ObjCCategory", "CXCompletionContext_ObjCCategory", int64_t(CXCompletionContext::CXCompletionContext_ObjCCategory), das::LineInfo());
		addIEx("ObjCInstanceMessage", "CXCompletionContext_ObjCInstanceMessage", int64_t(CXCompletionContext::CXCompletionContext_ObjCInstanceMessage), das::LineInfo());
		addIEx("ObjCClassMessage", "CXCompletionContext_ObjCClassMessage", int64_t(CXCompletionContext::CXCompletionContext_ObjCClassMessage), das::LineInfo());
		addIEx("ObjCSelectorName", "CXCompletionContext_ObjCSelectorName", int64_t(CXCompletionContext::CXCompletionContext_ObjCSelectorName), das::LineInfo());
		addIEx("MacroName", "CXCompletionContext_MacroName", int64_t(CXCompletionContext::CXCompletionContext_MacroName), das::LineInfo());
		addIEx("NaturalLanguage", "CXCompletionContext_NaturalLanguage", int64_t(CXCompletionContext::CXCompletionContext_NaturalLanguage), das::LineInfo());
		addIEx("IncludedFile", "CXCompletionContext_IncludedFile", int64_t(CXCompletionContext::CXCompletionContext_IncludedFile), das::LineInfo());
		addIEx("Unknown", "CXCompletionContext_Unknown", int64_t(CXCompletionContext::CXCompletionContext_Unknown), das::LineInfo());
	}
};

class Enumeration_CXEvalResultKind : public das::Enumeration {
public:
	Enumeration_CXEvalResultKind() : das::Enumeration("CXEvalResultKind") {
		external = true;
		cppName = "CXEvalResultKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXEvalResultKind >::type >::type;
		addIEx("CXEval_Int", "CXEval_Int", int64_t(CXEvalResultKind::CXEval_Int), das::LineInfo());
		addIEx("CXEval_Float", "CXEval_Float", int64_t(CXEvalResultKind::CXEval_Float), das::LineInfo());
		addIEx("CXEval_ObjCStrLiteral", "CXEval_ObjCStrLiteral", int64_t(CXEvalResultKind::CXEval_ObjCStrLiteral), das::LineInfo());
		addIEx("CXEval_StrLiteral", "CXEval_StrLiteral", int64_t(CXEvalResultKind::CXEval_StrLiteral), das::LineInfo());
		addIEx("CXEval_CFStr", "CXEval_CFStr", int64_t(CXEvalResultKind::CXEval_CFStr), das::LineInfo());
		addIEx("CXEval_Other", "CXEval_Other", int64_t(CXEvalResultKind::CXEval_Other), das::LineInfo());
		addIEx("CXEval_UnExposed", "CXEval_UnExposed", int64_t(CXEvalResultKind::CXEval_UnExposed), das::LineInfo());
	}
};

class Enumeration_CXVisitorResult : public das::Enumeration {
public:
	Enumeration_CXVisitorResult() : das::Enumeration("CXVisitorResult") {
		external = true;
		cppName = "CXVisitorResult";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXVisitorResult >::type >::type;
		addIEx("CXVisit_Break", "CXVisit_Break", int64_t(CXVisitorResult::CXVisit_Break), das::LineInfo());
		addIEx("CXVisit_Continue", "CXVisit_Continue", int64_t(CXVisitorResult::CXVisit_Continue), das::LineInfo());
	}
};

class Enumeration_CXResult : public das::Enumeration {
public:
	Enumeration_CXResult() : das::Enumeration("CXResult") {
		external = true;
		cppName = "CXResult";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXResult >::type >::type;
		addIEx("Success", "CXResult_Success", int64_t(CXResult::CXResult_Success), das::LineInfo());
		addIEx("Invalid", "CXResult_Invalid", int64_t(CXResult::CXResult_Invalid), das::LineInfo());
		addIEx("VisitBreak", "CXResult_VisitBreak", int64_t(CXResult::CXResult_VisitBreak), das::LineInfo());
	}
};

class Enumeration_CXIdxEntityKind : public das::Enumeration {
public:
	Enumeration_CXIdxEntityKind() : das::Enumeration("CXIdxEntityKind") {
		external = true;
		cppName = "CXIdxEntityKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXIdxEntityKind >::type >::type;
		addIEx("CXIdxEntity_Unexposed", "CXIdxEntity_Unexposed", int64_t(CXIdxEntityKind::CXIdxEntity_Unexposed), das::LineInfo());
		addIEx("CXIdxEntity_Typedef", "CXIdxEntity_Typedef", int64_t(CXIdxEntityKind::CXIdxEntity_Typedef), das::LineInfo());
		addIEx("CXIdxEntity_Function", "CXIdxEntity_Function", int64_t(CXIdxEntityKind::CXIdxEntity_Function), das::LineInfo());
		addIEx("CXIdxEntity_Variable", "CXIdxEntity_Variable", int64_t(CXIdxEntityKind::CXIdxEntity_Variable), das::LineInfo());
		addIEx("CXIdxEntity_Field", "CXIdxEntity_Field", int64_t(CXIdxEntityKind::CXIdxEntity_Field), das::LineInfo());
		addIEx("CXIdxEntity_EnumConstant", "CXIdxEntity_EnumConstant", int64_t(CXIdxEntityKind::CXIdxEntity_EnumConstant), das::LineInfo());
		addIEx("CXIdxEntity_ObjCClass", "CXIdxEntity_ObjCClass", int64_t(CXIdxEntityKind::CXIdxEntity_ObjCClass), das::LineInfo());
		addIEx("CXIdxEntity_ObjCProtocol", "CXIdxEntity_ObjCProtocol", int64_t(CXIdxEntityKind::CXIdxEntity_ObjCProtocol), das::LineInfo());
		addIEx("CXIdxEntity_ObjCCategory", "CXIdxEntity_ObjCCategory", int64_t(CXIdxEntityKind::CXIdxEntity_ObjCCategory), das::LineInfo());
		addIEx("CXIdxEntity_ObjCInstanceMethod", "CXIdxEntity_ObjCInstanceMethod", int64_t(CXIdxEntityKind::CXIdxEntity_ObjCInstanceMethod), das::LineInfo());
		addIEx("CXIdxEntity_ObjCClassMethod", "CXIdxEntity_ObjCClassMethod", int64_t(CXIdxEntityKind::CXIdxEntity_ObjCClassMethod), das::LineInfo());
		addIEx("CXIdxEntity_ObjCProperty", "CXIdxEntity_ObjCProperty", int64_t(CXIdxEntityKind::CXIdxEntity_ObjCProperty), das::LineInfo());
		addIEx("CXIdxEntity_ObjCIvar", "CXIdxEntity_ObjCIvar", int64_t(CXIdxEntityKind::CXIdxEntity_ObjCIvar), das::LineInfo());
		addIEx("CXIdxEntity_Enum", "CXIdxEntity_Enum", int64_t(CXIdxEntityKind::CXIdxEntity_Enum), das::LineInfo());
		addIEx("CXIdxEntity_Struct", "CXIdxEntity_Struct", int64_t(CXIdxEntityKind::CXIdxEntity_Struct), das::LineInfo());
		addIEx("CXIdxEntity_Union", "CXIdxEntity_Union", int64_t(CXIdxEntityKind::CXIdxEntity_Union), das::LineInfo());
		addIEx("CXIdxEntity_CXXClass", "CXIdxEntity_CXXClass", int64_t(CXIdxEntityKind::CXIdxEntity_CXXClass), das::LineInfo());
		addIEx("CXIdxEntity_CXXNamespace", "CXIdxEntity_CXXNamespace", int64_t(CXIdxEntityKind::CXIdxEntity_CXXNamespace), das::LineInfo());
		addIEx("CXIdxEntity_CXXNamespaceAlias", "CXIdxEntity_CXXNamespaceAlias", int64_t(CXIdxEntityKind::CXIdxEntity_CXXNamespaceAlias), das::LineInfo());
		addIEx("CXIdxEntity_CXXStaticVariable", "CXIdxEntity_CXXStaticVariable", int64_t(CXIdxEntityKind::CXIdxEntity_CXXStaticVariable), das::LineInfo());
		addIEx("CXIdxEntity_CXXStaticMethod", "CXIdxEntity_CXXStaticMethod", int64_t(CXIdxEntityKind::CXIdxEntity_CXXStaticMethod), das::LineInfo());
		addIEx("CXIdxEntity_CXXInstanceMethod", "CXIdxEntity_CXXInstanceMethod", int64_t(CXIdxEntityKind::CXIdxEntity_CXXInstanceMethod), das::LineInfo());
		addIEx("CXIdxEntity_CXXConstructor", "CXIdxEntity_CXXConstructor", int64_t(CXIdxEntityKind::CXIdxEntity_CXXConstructor), das::LineInfo());
		addIEx("CXIdxEntity_CXXDestructor", "CXIdxEntity_CXXDestructor", int64_t(CXIdxEntityKind::CXIdxEntity_CXXDestructor), das::LineInfo());
		addIEx("CXIdxEntity_CXXConversionFunction", "CXIdxEntity_CXXConversionFunction", int64_t(CXIdxEntityKind::CXIdxEntity_CXXConversionFunction), das::LineInfo());
		addIEx("CXIdxEntity_CXXTypeAlias", "CXIdxEntity_CXXTypeAlias", int64_t(CXIdxEntityKind::CXIdxEntity_CXXTypeAlias), das::LineInfo());
		addIEx("CXIdxEntity_CXXInterface", "CXIdxEntity_CXXInterface", int64_t(CXIdxEntityKind::CXIdxEntity_CXXInterface), das::LineInfo());
	}
};

class Enumeration_CXIdxEntityLanguage : public das::Enumeration {
public:
	Enumeration_CXIdxEntityLanguage() : das::Enumeration("CXIdxEntityLanguage") {
		external = true;
		cppName = "CXIdxEntityLanguage";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXIdxEntityLanguage >::type >::type;
		addIEx("CXIdxEntityLang_None", "CXIdxEntityLang_None", int64_t(CXIdxEntityLanguage::CXIdxEntityLang_None), das::LineInfo());
		addIEx("CXIdxEntityLang_C", "CXIdxEntityLang_C", int64_t(CXIdxEntityLanguage::CXIdxEntityLang_C), das::LineInfo());
		addIEx("CXIdxEntityLang_ObjC", "CXIdxEntityLang_ObjC", int64_t(CXIdxEntityLanguage::CXIdxEntityLang_ObjC), das::LineInfo());
		addIEx("CXIdxEntityLang_CXX", "CXIdxEntityLang_CXX", int64_t(CXIdxEntityLanguage::CXIdxEntityLang_CXX), das::LineInfo());
		addIEx("CXIdxEntityLang_Swift", "CXIdxEntityLang_Swift", int64_t(CXIdxEntityLanguage::CXIdxEntityLang_Swift), das::LineInfo());
	}
};

class Enumeration_CXIdxEntityCXXTemplateKind : public das::Enumeration {
public:
	Enumeration_CXIdxEntityCXXTemplateKind() : das::Enumeration("CXIdxEntityCXXTemplateKind") {
		external = true;
		cppName = "CXIdxEntityCXXTemplateKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXIdxEntityCXXTemplateKind >::type >::type;
		addIEx("CXIdxEntity_NonTemplate", "CXIdxEntity_NonTemplate", int64_t(CXIdxEntityCXXTemplateKind::CXIdxEntity_NonTemplate), das::LineInfo());
		addIEx("CXIdxEntity_Template", "CXIdxEntity_Template", int64_t(CXIdxEntityCXXTemplateKind::CXIdxEntity_Template), das::LineInfo());
		addIEx("CXIdxEntity_TemplatePartialSpecialization", "CXIdxEntity_TemplatePartialSpecialization", int64_t(CXIdxEntityCXXTemplateKind::CXIdxEntity_TemplatePartialSpecialization), das::LineInfo());
		addIEx("CXIdxEntity_TemplateSpecialization", "CXIdxEntity_TemplateSpecialization", int64_t(CXIdxEntityCXXTemplateKind::CXIdxEntity_TemplateSpecialization), das::LineInfo());
	}
};

class Enumeration_CXIdxAttrKind : public das::Enumeration {
public:
	Enumeration_CXIdxAttrKind() : das::Enumeration("CXIdxAttrKind") {
		external = true;
		cppName = "CXIdxAttrKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXIdxAttrKind >::type >::type;
		addIEx("CXIdxAttr_Unexposed", "CXIdxAttr_Unexposed", int64_t(CXIdxAttrKind::CXIdxAttr_Unexposed), das::LineInfo());
		addIEx("CXIdxAttr_IBAction", "CXIdxAttr_IBAction", int64_t(CXIdxAttrKind::CXIdxAttr_IBAction), das::LineInfo());
		addIEx("CXIdxAttr_IBOutlet", "CXIdxAttr_IBOutlet", int64_t(CXIdxAttrKind::CXIdxAttr_IBOutlet), das::LineInfo());
		addIEx("CXIdxAttr_IBOutletCollection", "CXIdxAttr_IBOutletCollection", int64_t(CXIdxAttrKind::CXIdxAttr_IBOutletCollection), das::LineInfo());
	}
};

class Enumeration_CXIdxDeclInfoFlags : public das::Enumeration {
public:
	Enumeration_CXIdxDeclInfoFlags() : das::Enumeration("CXIdxDeclInfoFlags") {
		external = true;
		cppName = "CXIdxDeclInfoFlags";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXIdxDeclInfoFlags >::type >::type;
		addIEx("CXIdxDeclFlag_Skipped", "CXIdxDeclFlag_Skipped", int64_t(CXIdxDeclInfoFlags::CXIdxDeclFlag_Skipped), das::LineInfo());
	}
};

class Enumeration_CXIdxObjCContainerKind : public das::Enumeration {
public:
	Enumeration_CXIdxObjCContainerKind() : das::Enumeration("CXIdxObjCContainerKind") {
		external = true;
		cppName = "CXIdxObjCContainerKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXIdxObjCContainerKind >::type >::type;
		addIEx("CXIdxObjCContainer_ForwardRef", "CXIdxObjCContainer_ForwardRef", int64_t(CXIdxObjCContainerKind::CXIdxObjCContainer_ForwardRef), das::LineInfo());
		addIEx("CXIdxObjCContainer_Interface", "CXIdxObjCContainer_Interface", int64_t(CXIdxObjCContainerKind::CXIdxObjCContainer_Interface), das::LineInfo());
		addIEx("CXIdxObjCContainer_Implementation", "CXIdxObjCContainer_Implementation", int64_t(CXIdxObjCContainerKind::CXIdxObjCContainer_Implementation), das::LineInfo());
	}
};

class Enumeration_CXIdxEntityRefKind : public das::Enumeration {
public:
	Enumeration_CXIdxEntityRefKind() : das::Enumeration("CXIdxEntityRefKind") {
		external = true;
		cppName = "CXIdxEntityRefKind";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXIdxEntityRefKind >::type >::type;
		addIEx("CXIdxEntityRef_Direct", "CXIdxEntityRef_Direct", int64_t(CXIdxEntityRefKind::CXIdxEntityRef_Direct), das::LineInfo());
		addIEx("CXIdxEntityRef_Implicit", "CXIdxEntityRef_Implicit", int64_t(CXIdxEntityRefKind::CXIdxEntityRef_Implicit), das::LineInfo());
	}
};

class Enumeration_CXSymbolRole : public das::Enumeration {
public:
	Enumeration_CXSymbolRole() : das::Enumeration("CXSymbolRole") {
		external = true;
		cppName = "CXSymbolRole";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXSymbolRole >::type >::type;
		addIEx("None", "CXSymbolRole_None", int64_t(CXSymbolRole::CXSymbolRole_None), das::LineInfo());
		addIEx("Declaration", "CXSymbolRole_Declaration", int64_t(CXSymbolRole::CXSymbolRole_Declaration), das::LineInfo());
		addIEx("Definition", "CXSymbolRole_Definition", int64_t(CXSymbolRole::CXSymbolRole_Definition), das::LineInfo());
		addIEx("Reference", "CXSymbolRole_Reference", int64_t(CXSymbolRole::CXSymbolRole_Reference), das::LineInfo());
		addIEx("Read", "CXSymbolRole_Read", int64_t(CXSymbolRole::CXSymbolRole_Read), das::LineInfo());
		addIEx("Write", "CXSymbolRole_Write", int64_t(CXSymbolRole::CXSymbolRole_Write), das::LineInfo());
		addIEx("Call", "CXSymbolRole_Call", int64_t(CXSymbolRole::CXSymbolRole_Call), das::LineInfo());
		addIEx("Dynamic", "CXSymbolRole_Dynamic", int64_t(CXSymbolRole::CXSymbolRole_Dynamic), das::LineInfo());
		addIEx("AddressOf", "CXSymbolRole_AddressOf", int64_t(CXSymbolRole::CXSymbolRole_AddressOf), das::LineInfo());
		addIEx("Implicit", "CXSymbolRole_Implicit", int64_t(CXSymbolRole::CXSymbolRole_Implicit), das::LineInfo());
	}
};

class Enumeration_CXIndexOptFlags : public das::Enumeration {
public:
	Enumeration_CXIndexOptFlags() : das::Enumeration("CXIndexOptFlags") {
		external = true;
		cppName = "CXIndexOptFlags";
		baseType = (das::Type) das::ToBasicType< das::underlying_type< CXIndexOptFlags >::type >::type;
		addIEx("CXIndexOpt_None", "CXIndexOpt_None", int64_t(CXIndexOptFlags::CXIndexOpt_None), das::LineInfo());
		addIEx("CXIndexOpt_SuppressRedundantRefs", "CXIndexOpt_SuppressRedundantRefs", int64_t(CXIndexOptFlags::CXIndexOpt_SuppressRedundantRefs), das::LineInfo());
		addIEx("CXIndexOpt_IndexFunctionLocalSymbols", "CXIndexOpt_IndexFunctionLocalSymbols", int64_t(CXIndexOptFlags::CXIndexOpt_IndexFunctionLocalSymbols), das::LineInfo());
		addIEx("CXIndexOpt_IndexImplicitTemplateInstantiations", "CXIndexOpt_IndexImplicitTemplateInstantiations", int64_t(CXIndexOptFlags::CXIndexOpt_IndexImplicitTemplateInstantiations), das::LineInfo());
		addIEx("CXIndexOpt_SuppressWarnings", "CXIndexOpt_SuppressWarnings", int64_t(CXIndexOptFlags::CXIndexOpt_SuppressWarnings), das::LineInfo());
		addIEx("CXIndexOpt_SkipParsedBodiesInSession", "CXIndexOpt_SkipParsedBodiesInSession", int64_t(CXIndexOptFlags::CXIndexOpt_SkipParsedBodiesInSession), das::LineInfo());
	}
};

