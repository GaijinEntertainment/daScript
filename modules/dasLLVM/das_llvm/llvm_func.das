require dasbind public
options gen2
module llvm_func shared
require llvm_struct
require llvm_enum
[extern(cdecl, name="LLVMVerifyModule", library="lib/LLVM.dll")]
def LLVMVerifyModule(M : LLVMOpaqueModule? implicit; Action : LLVMVerifierFailureAction; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMVerifyFunction", library="lib/LLVM.dll")]
def LLVMVerifyFunction(Fn : LLVMOpaqueValue? implicit; Action : LLVMVerifierFailureAction) : int {}
[extern(cdecl, name="LLVMViewFunctionCFG", library="lib/LLVM.dll")]
def LLVMViewFunctionCFG(Fn : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMViewFunctionCFGOnly", library="lib/LLVM.dll")]
def LLVMViewFunctionCFGOnly(Fn : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMParseBitcode", library="lib/LLVM.dll")]
def LLVMParseBitcode(MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutModule : LLVMOpaqueModule ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMParseBitcode2", library="lib/LLVM.dll")]
def LLVMParseBitcode2(MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutModule : LLVMOpaqueModule ?? implicit) : int {}
[extern(cdecl, name="LLVMParseBitcodeInContext", library="lib/LLVM.dll")]
def LLVMParseBitcodeInContext(ContextRef : LLVMOpaqueContext? implicit; MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutModule : LLVMOpaqueModule ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMParseBitcodeInContext2", library="lib/LLVM.dll")]
def LLVMParseBitcodeInContext2(ContextRef : LLVMOpaqueContext? implicit; MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutModule : LLVMOpaqueModule ?? implicit) : int {}
[extern(cdecl, name="LLVMGetBitcodeModuleInContext", library="lib/LLVM.dll")]
def LLVMGetBitcodeModuleInContext(ContextRef : LLVMOpaqueContext? implicit; MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutM : LLVMOpaqueModule ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMGetBitcodeModuleInContext2", library="lib/LLVM.dll")]
def LLVMGetBitcodeModuleInContext2(ContextRef : LLVMOpaqueContext? implicit; MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutM : LLVMOpaqueModule ?? implicit) : int {}
[extern(cdecl, name="LLVMGetBitcodeModule", library="lib/LLVM.dll")]
def LLVMGetBitcodeModule(MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutM : LLVMOpaqueModule ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMGetBitcodeModule2", library="lib/LLVM.dll")]
def LLVMGetBitcodeModule2(MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutM : LLVMOpaqueModule ?? implicit) : int {}
[extern(cdecl, name="LLVMWriteBitcodeToFile", library="lib/LLVM.dll")]
def LLVMWriteBitcodeToFile(M : LLVMOpaqueModule? implicit; Path : string implicit) : int {}
[extern(cdecl, name="LLVMWriteBitcodeToFD", library="lib/LLVM.dll")]
def LLVMWriteBitcodeToFD(M : LLVMOpaqueModule? implicit; FD : int; ShouldClose : int; Unbuffered : int) : int {}
[extern(cdecl, name="LLVMWriteBitcodeToFileHandle", library="lib/LLVM.dll")]
def LLVMWriteBitcodeToFileHandle(M : LLVMOpaqueModule? implicit; Handle : int) : int {}
[extern(cdecl, name="LLVMWriteBitcodeToMemoryBuffer", library="lib/LLVM.dll")]
def LLVMWriteBitcodeToMemoryBuffer(M : LLVMOpaqueModule? implicit) : LLVMOpaqueMemoryBuffer? {}
[extern(cdecl, name="LLVMGetOrInsertComdat", library="lib/LLVM.dll")]
def LLVMGetOrInsertComdat(M : LLVMOpaqueModule? implicit; Name : string implicit) : LLVMComdat? {}
[extern(cdecl, name="LLVMGetComdat", library="lib/LLVM.dll")]
def LLVMGetComdat(V : LLVMOpaqueValue? implicit) : LLVMComdat? {}
[extern(cdecl, name="LLVMSetComdat", library="lib/LLVM.dll")]
def LLVMSetComdat(V : LLVMOpaqueValue? implicit; C : LLVMComdat? implicit) : void {}
[extern(cdecl, name="LLVMGetComdatSelectionKind", library="lib/LLVM.dll")]
def LLVMGetComdatSelectionKind(C : LLVMComdat? implicit) : LLVMComdatSelectionKind {}
[extern(cdecl, name="LLVMSetComdatSelectionKind", library="lib/LLVM.dll")]
def LLVMSetComdatSelectionKind(C : LLVMComdat? implicit; Kind : LLVMComdatSelectionKind) : void {}
[extern(cdecl, name="LLVMResetFatalErrorHandler", library="lib/LLVM.dll")]
def LLVMResetFatalErrorHandler() : void {}
[extern(cdecl, name="LLVMEnablePrettyStackTrace", library="lib/LLVM.dll")]
def LLVMEnablePrettyStackTrace() : void {}
[extern(cdecl, name="LLVMShutdown", library="lib/LLVM.dll")]
def LLVMShutdown() : void {}
[extern(cdecl, name="LLVMGetVersion", library="lib/LLVM.dll")]
def LLVMGetVersion(Major : uint? implicit; Minor : uint? implicit; Patch : uint? implicit) : void {}
[extern(cdecl, name="LLVMCreateMessage", library="lib/LLVM.dll")]
def LLVMCreateMessage(Message : string implicit) : string {}
[extern(cdecl, name="LLVMDisposeMessage", library="lib/LLVM.dll")]
def LLVMDisposeMessage(Message : string implicit) : void {}
[extern(cdecl, name="LLVMContextCreate", library="lib/LLVM.dll")]
def LLVMContextCreate() : LLVMOpaqueContext? {}
[extern(cdecl, name="LLVMGetGlobalContext", library="lib/LLVM.dll")]
def LLVMGetGlobalContext() : LLVMOpaqueContext? {}
[extern(cdecl, name="LLVMContextGetDiagnosticContext", library="lib/LLVM.dll")]
def LLVMContextGetDiagnosticContext(C : LLVMOpaqueContext? implicit) : void? {}
[extern(cdecl, name="LLVMContextShouldDiscardValueNames", library="lib/LLVM.dll")]
def LLVMContextShouldDiscardValueNames(C : LLVMOpaqueContext? implicit) : int {}
[extern(cdecl, name="LLVMContextSetDiscardValueNames", library="lib/LLVM.dll")]
def LLVMContextSetDiscardValueNames(C : LLVMOpaqueContext? implicit; Discard : int) : void {}
[extern(cdecl, name="LLVMContextSetOpaquePointers", library="lib/LLVM.dll")]
def LLVMContextSetOpaquePointers(C : LLVMOpaqueContext? implicit; OpaquePointers : int) : void {}
[extern(cdecl, name="LLVMContextDispose", library="lib/LLVM.dll")]
def LLVMContextDispose(C : LLVMOpaqueContext? implicit) : void {}
[extern(cdecl, name="LLVMGetDiagInfoDescription", library="lib/LLVM.dll")]
def LLVMGetDiagInfoDescription(DI : LLVMOpaqueDiagnosticInfo? implicit) : string {}
[extern(cdecl, name="LLVMGetDiagInfoSeverity", library="lib/LLVM.dll")]
def LLVMGetDiagInfoSeverity(DI : LLVMOpaqueDiagnosticInfo? implicit) : LLVMDiagnosticSeverity {}
[extern(cdecl, name="LLVMGetMDKindIDInContext", library="lib/LLVM.dll")]
def LLVMGetMDKindIDInContext(C : LLVMOpaqueContext? implicit; Name : string implicit; SLen : uint) : uint {}
[extern(cdecl, name="LLVMGetMDKindID", library="lib/LLVM.dll")]
def LLVMGetMDKindID(Name : string implicit; SLen : uint) : uint {}
[extern(cdecl, name="LLVMGetEnumAttributeKindForName", library="lib/LLVM.dll")]
def LLVMGetEnumAttributeKindForName(Name : string implicit; SLen : uint64) : uint {}
[extern(cdecl, name="LLVMGetLastEnumAttributeKind", library="lib/LLVM.dll")]
def LLVMGetLastEnumAttributeKind() : uint {}
[extern(cdecl, name="LLVMCreateEnumAttribute", library="lib/LLVM.dll")]
def LLVMCreateEnumAttribute(C : LLVMOpaqueContext? implicit; KindID : uint; Val : uint64) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMGetEnumAttributeKind", library="lib/LLVM.dll")]
def LLVMGetEnumAttributeKind(A : LLVMOpaqueAttributeRef? implicit) : uint {}
[extern(cdecl, name="LLVMGetEnumAttributeValue", library="lib/LLVM.dll")]
def LLVMGetEnumAttributeValue(A : LLVMOpaqueAttributeRef? implicit) : uint64 {}
[extern(cdecl, name="LLVMCreateTypeAttribute", library="lib/LLVM.dll")]
def LLVMCreateTypeAttribute(C : LLVMOpaqueContext? implicit; KindID : uint; type_ref : LLVMOpaqueType? implicit) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMGetTypeAttributeValue", library="lib/LLVM.dll")]
def LLVMGetTypeAttributeValue(A : LLVMOpaqueAttributeRef? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMCreateStringAttribute", library="lib/LLVM.dll")]
def LLVMCreateStringAttribute(C : LLVMOpaqueContext? implicit; K : string implicit; KLength : uint; V : string implicit; VLength : uint) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMGetStringAttributeKind", library="lib/LLVM.dll")]
def LLVMGetStringAttributeKind(A : LLVMOpaqueAttributeRef? implicit; Length : uint? implicit) : string {}
[extern(cdecl, name="LLVMGetStringAttributeValue", library="lib/LLVM.dll")]
def LLVMGetStringAttributeValue(A : LLVMOpaqueAttributeRef? implicit; Length : uint? implicit) : string {}
[extern(cdecl, name="LLVMIsEnumAttribute", library="lib/LLVM.dll")]
def LLVMIsEnumAttribute(A : LLVMOpaqueAttributeRef? implicit) : int {}
[extern(cdecl, name="LLVMIsStringAttribute", library="lib/LLVM.dll")]
def LLVMIsStringAttribute(A : LLVMOpaqueAttributeRef? implicit) : int {}
[extern(cdecl, name="LLVMIsTypeAttribute", library="lib/LLVM.dll")]
def LLVMIsTypeAttribute(A : LLVMOpaqueAttributeRef? implicit) : int {}
[extern(cdecl, name="LLVMGetTypeByName2", library="lib/LLVM.dll")]
def LLVMGetTypeByName2(C : LLVMOpaqueContext? implicit; Name : string implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMModuleCreateWithName", library="lib/LLVM.dll")]
def LLVMModuleCreateWithName(ModuleID : string implicit) : LLVMOpaqueModule? {}
[extern(cdecl, name="LLVMModuleCreateWithNameInContext", library="lib/LLVM.dll")]
def LLVMModuleCreateWithNameInContext(ModuleID : string implicit; C : LLVMOpaqueContext? implicit) : LLVMOpaqueModule? {}
[extern(cdecl, name="LLVMCloneModule", library="lib/LLVM.dll")]
def LLVMCloneModule(M : LLVMOpaqueModule? implicit) : LLVMOpaqueModule? {}
[extern(cdecl, name="LLVMDisposeModule", library="lib/LLVM.dll")]
def LLVMDisposeModule(M : LLVMOpaqueModule? implicit) : void {}
[extern(cdecl, name="LLVMGetModuleIdentifier", library="lib/LLVM.dll")]
def LLVMGetModuleIdentifier(M : LLVMOpaqueModule? implicit; Len : uint64? implicit) : string {}
[extern(cdecl, name="LLVMSetModuleIdentifier", library="lib/LLVM.dll")]
def LLVMSetModuleIdentifier(M : LLVMOpaqueModule? implicit; Ident : string implicit; Len : uint64) : void {}
[extern(cdecl, name="LLVMGetSourceFileName", library="lib/LLVM.dll")]
def LLVMGetSourceFileName(M : LLVMOpaqueModule? implicit; Len : uint64? implicit) : string {}
[extern(cdecl, name="LLVMSetSourceFileName", library="lib/LLVM.dll")]
def LLVMSetSourceFileName(M : LLVMOpaqueModule? implicit; Name : string implicit; Len : uint64) : void {}
[extern(cdecl, name="LLVMGetDataLayoutStr", library="lib/LLVM.dll")]
def LLVMGetDataLayoutStr(M : LLVMOpaqueModule? implicit) : string {}
[extern(cdecl, name="LLVMGetDataLayout", library="lib/LLVM.dll")]
def LLVMGetDataLayout(M : LLVMOpaqueModule? implicit) : string {}
[extern(cdecl, name="LLVMSetDataLayout", library="lib/LLVM.dll")]
def LLVMSetDataLayout(M : LLVMOpaqueModule? implicit; DataLayoutStr : string implicit) : void {}
[extern(cdecl, name="LLVMGetTarget", library="lib/LLVM.dll")]
def LLVMGetTarget(M : LLVMOpaqueModule? implicit) : string {}
[extern(cdecl, name="LLVMSetTarget", library="lib/LLVM.dll")]
def LLVMSetTarget(M : LLVMOpaqueModule? implicit; Triple : string implicit) : void {}
[extern(cdecl, name="LLVMCopyModuleFlagsMetadata", library="lib/LLVM.dll")]
def LLVMCopyModuleFlagsMetadata(M : LLVMOpaqueModule? implicit; Len : uint64? implicit) : LLVMOpaqueModuleFlagEntry? {}
[extern(cdecl, name="LLVMDisposeModuleFlagsMetadata", library="lib/LLVM.dll")]
def LLVMDisposeModuleFlagsMetadata(Entries : LLVMOpaqueModuleFlagEntry? implicit) : void {}
[extern(cdecl, name="LLVMModuleFlagEntriesGetFlagBehavior", library="lib/LLVM.dll")]
def LLVMModuleFlagEntriesGetFlagBehavior(Entries : LLVMOpaqueModuleFlagEntry? implicit; Index : uint) : LLVMModuleFlagBehavior {}
[extern(cdecl, name="LLVMModuleFlagEntriesGetKey", library="lib/LLVM.dll")]
def LLVMModuleFlagEntriesGetKey(Entries : LLVMOpaqueModuleFlagEntry? implicit; Index : uint; Len : uint64? implicit) : string {}
[extern(cdecl, name="LLVMModuleFlagEntriesGetMetadata", library="lib/LLVM.dll")]
def LLVMModuleFlagEntriesGetMetadata(Entries : LLVMOpaqueModuleFlagEntry? implicit; Index : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMGetModuleFlag", library="lib/LLVM.dll")]
def LLVMGetModuleFlag(M : LLVMOpaqueModule? implicit; Key : string implicit; KeyLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMAddModuleFlag", library="lib/LLVM.dll")]
def LLVMAddModuleFlag(M : LLVMOpaqueModule? implicit; Behavior : LLVMModuleFlagBehavior; Key : string implicit; KeyLen : uint64; Val : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMDumpModule", library="lib/LLVM.dll")]
def LLVMDumpModule(M : LLVMOpaqueModule? implicit) : void {}
[extern(cdecl, name="LLVMPrintModuleToFile", library="lib/LLVM.dll")]
def LLVMPrintModuleToFile(M : LLVMOpaqueModule? implicit; Filename : string implicit; ErrorMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMPrintModuleToString", library="lib/LLVM.dll")]
def LLVMPrintModuleToString(M : LLVMOpaqueModule? implicit) : string {}
[extern(cdecl, name="LLVMGetModuleInlineAsm", library="lib/LLVM.dll")]
def LLVMGetModuleInlineAsm(M : LLVMOpaqueModule? implicit; Len : uint64? implicit) : string {}
[extern(cdecl, name="LLVMSetModuleInlineAsm2", library="lib/LLVM.dll")]
def LLVMSetModuleInlineAsm2(M : LLVMOpaqueModule? implicit; Asm : string implicit; Len : uint64) : void {}
[extern(cdecl, name="LLVMAppendModuleInlineAsm", library="lib/LLVM.dll")]
def LLVMAppendModuleInlineAsm(M : LLVMOpaqueModule? implicit; Asm : string implicit; Len : uint64) : void {}
[extern(cdecl, name="LLVMGetInlineAsm", library="lib/LLVM.dll")]
def LLVMGetInlineAsm(Ty : LLVMOpaqueType? implicit; AsmString : string implicit; AsmStringSize : uint64; Constraints : string implicit; ConstraintsSize : uint64; HasSideEffects : int; IsAlignStack : int; Dialect : LLVMInlineAsmDialect; CanThrow : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetModuleContext", library="lib/LLVM.dll")]
def LLVMGetModuleContext(M : LLVMOpaqueModule? implicit) : LLVMOpaqueContext? {}
[extern(cdecl, name="LLVMGetTypeByName", library="lib/LLVM.dll")]
def LLVMGetTypeByName(M : LLVMOpaqueModule? implicit; Name : string implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetFirstNamedMetadata", library="lib/LLVM.dll")]
def LLVMGetFirstNamedMetadata(M : LLVMOpaqueModule? implicit) : LLVMOpaqueNamedMDNode? {}
[extern(cdecl, name="LLVMGetLastNamedMetadata", library="lib/LLVM.dll")]
def LLVMGetLastNamedMetadata(M : LLVMOpaqueModule? implicit) : LLVMOpaqueNamedMDNode? {}
[extern(cdecl, name="LLVMGetNextNamedMetadata", library="lib/LLVM.dll")]
def LLVMGetNextNamedMetadata(NamedMDNode : LLVMOpaqueNamedMDNode? implicit) : LLVMOpaqueNamedMDNode? {}
[extern(cdecl, name="LLVMGetPreviousNamedMetadata", library="lib/LLVM.dll")]
def LLVMGetPreviousNamedMetadata(NamedMDNode : LLVMOpaqueNamedMDNode? implicit) : LLVMOpaqueNamedMDNode? {}
[extern(cdecl, name="LLVMGetNamedMetadata", library="lib/LLVM.dll")]
def LLVMGetNamedMetadata(M : LLVMOpaqueModule? implicit; Name : string implicit; NameLen : uint64) : LLVMOpaqueNamedMDNode? {}
[extern(cdecl, name="LLVMGetOrInsertNamedMetadata", library="lib/LLVM.dll")]
def LLVMGetOrInsertNamedMetadata(M : LLVMOpaqueModule? implicit; Name : string implicit; NameLen : uint64) : LLVMOpaqueNamedMDNode? {}
[extern(cdecl, name="LLVMGetNamedMetadataName", library="lib/LLVM.dll")]
def LLVMGetNamedMetadataName(NamedMD : LLVMOpaqueNamedMDNode? implicit; NameLen : uint64? implicit) : string {}
[extern(cdecl, name="LLVMGetNamedMetadataNumOperands", library="lib/LLVM.dll")]
def LLVMGetNamedMetadataNumOperands(M : LLVMOpaqueModule? implicit; Name : string implicit) : uint {}
[extern(cdecl, name="LLVMGetNamedMetadataOperands", library="lib/LLVM.dll")]
def LLVMGetNamedMetadataOperands(M : LLVMOpaqueModule? implicit; Name : string implicit; Dest : LLVMOpaqueValue ?? implicit) : void {}
[extern(cdecl, name="LLVMAddNamedMetadataOperand", library="lib/LLVM.dll")]
def LLVMAddNamedMetadataOperand(M : LLVMOpaqueModule? implicit; Name : string implicit; Val : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetDebugLocDirectory", library="lib/LLVM.dll")]
def LLVMGetDebugLocDirectory(Val : LLVMOpaqueValue? implicit; Length : uint? implicit) : string {}
[extern(cdecl, name="LLVMGetDebugLocFilename", library="lib/LLVM.dll")]
def LLVMGetDebugLocFilename(Val : LLVMOpaqueValue? implicit; Length : uint? implicit) : string {}
[extern(cdecl, name="LLVMGetDebugLocLine", library="lib/LLVM.dll")]
def LLVMGetDebugLocLine(Val : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetDebugLocColumn", library="lib/LLVM.dll")]
def LLVMGetDebugLocColumn(Val : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMAddFunction", library="lib/LLVM.dll")]
def LLVMAddFunction(M : LLVMOpaqueModule? implicit; Name : string implicit; FunctionTy : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNamedFunction", library="lib/LLVM.dll")]
def LLVMGetNamedFunction(M : LLVMOpaqueModule? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetFirstFunction", library="lib/LLVM.dll")]
def LLVMGetFirstFunction(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetLastFunction", library="lib/LLVM.dll")]
def LLVMGetLastFunction(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNextFunction", library="lib/LLVM.dll")]
def LLVMGetNextFunction(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPreviousFunction", library="lib/LLVM.dll")]
def LLVMGetPreviousFunction(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetModuleInlineAsm", library="lib/LLVM.dll")]
def LLVMSetModuleInlineAsm(M : LLVMOpaqueModule? implicit; Asm : string implicit) : void {}
[extern(cdecl, name="LLVMGetTypeKind", library="lib/LLVM.dll")]
def LLVMGetTypeKind(Ty : LLVMOpaqueType? implicit) : LLVMTypeKind {}
[extern(cdecl, name="LLVMTypeIsSized", library="lib/LLVM.dll")]
def LLVMTypeIsSized(Ty : LLVMOpaqueType? implicit) : int {}
[extern(cdecl, name="LLVMGetTypeContext", library="lib/LLVM.dll")]
def LLVMGetTypeContext(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueContext? {}
[extern(cdecl, name="LLVMDumpType", library="lib/LLVM.dll")]
def LLVMDumpType(Val : LLVMOpaqueType? implicit) : void {}
[extern(cdecl, name="LLVMPrintTypeToString", library="lib/LLVM.dll")]
def LLVMPrintTypeToString(Val : LLVMOpaqueType? implicit) : string {}
[extern(cdecl, name="LLVMInt1TypeInContext", library="lib/LLVM.dll")]
def LLVMInt1TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt8TypeInContext", library="lib/LLVM.dll")]
def LLVMInt8TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt16TypeInContext", library="lib/LLVM.dll")]
def LLVMInt16TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt32TypeInContext", library="lib/LLVM.dll")]
def LLVMInt32TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt64TypeInContext", library="lib/LLVM.dll")]
def LLVMInt64TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt128TypeInContext", library="lib/LLVM.dll")]
def LLVMInt128TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIntTypeInContext", library="lib/LLVM.dll")]
def LLVMIntTypeInContext(C : LLVMOpaqueContext? implicit; NumBits : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt1Type", library="lib/LLVM.dll")]
def LLVMInt1Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt8Type", library="lib/LLVM.dll")]
def LLVMInt8Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt16Type", library="lib/LLVM.dll")]
def LLVMInt16Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt32Type", library="lib/LLVM.dll")]
def LLVMInt32Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt64Type", library="lib/LLVM.dll")]
def LLVMInt64Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt128Type", library="lib/LLVM.dll")]
def LLVMInt128Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIntType", library="lib/LLVM.dll")]
def LLVMIntType(NumBits : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetIntTypeWidth", library="lib/LLVM.dll")]
def LLVMGetIntTypeWidth(IntegerTy : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMHalfTypeInContext", library="lib/LLVM.dll")]
def LLVMHalfTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMBFloatTypeInContext", library="lib/LLVM.dll")]
def LLVMBFloatTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMFloatTypeInContext", library="lib/LLVM.dll")]
def LLVMFloatTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMDoubleTypeInContext", library="lib/LLVM.dll")]
def LLVMDoubleTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMX86FP80TypeInContext", library="lib/LLVM.dll")]
def LLVMX86FP80TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMFP128TypeInContext", library="lib/LLVM.dll")]
def LLVMFP128TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMPPCFP128TypeInContext", library="lib/LLVM.dll")]
def LLVMPPCFP128TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMHalfType", library="lib/LLVM.dll")]
def LLVMHalfType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMBFloatType", library="lib/LLVM.dll")]
def LLVMBFloatType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMFloatType", library="lib/LLVM.dll")]
def LLVMFloatType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMDoubleType", library="lib/LLVM.dll")]
def LLVMDoubleType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMX86FP80Type", library="lib/LLVM.dll")]
def LLVMX86FP80Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMFP128Type", library="lib/LLVM.dll")]
def LLVMFP128Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMPPCFP128Type", library="lib/LLVM.dll")]
def LLVMPPCFP128Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMFunctionType", library="lib/LLVM.dll")]
def LLVMFunctionType(ReturnType : LLVMOpaqueType? implicit; ParamTypes : LLVMOpaqueType ?? implicit; ParamCount : uint; IsVarArg : int) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIsFunctionVarArg", library="lib/LLVM.dll")]
def LLVMIsFunctionVarArg(FunctionTy : LLVMOpaqueType? implicit) : int {}
[extern(cdecl, name="LLVMGetReturnType", library="lib/LLVM.dll")]
def LLVMGetReturnType(FunctionTy : LLVMOpaqueType? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMCountParamTypes", library="lib/LLVM.dll")]
def LLVMCountParamTypes(FunctionTy : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMGetParamTypes", library="lib/LLVM.dll")]
def LLVMGetParamTypes(FunctionTy : LLVMOpaqueType? implicit; Dest : LLVMOpaqueType ?? implicit) : void {}
[extern(cdecl, name="LLVMStructTypeInContext", library="lib/LLVM.dll")]
def LLVMStructTypeInContext(C : LLVMOpaqueContext? implicit; ElementTypes : LLVMOpaqueType ?? implicit; ElementCount : uint; Packed : int) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMStructType", library="lib/LLVM.dll")]
def LLVMStructType(ElementTypes : LLVMOpaqueType ?? implicit; ElementCount : uint; Packed : int) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMStructCreateNamed", library="lib/LLVM.dll")]
def LLVMStructCreateNamed(C : LLVMOpaqueContext? implicit; Name : string implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetStructName", library="lib/LLVM.dll")]
def LLVMGetStructName(Ty : LLVMOpaqueType? implicit) : string {}
[extern(cdecl, name="LLVMStructSetBody", library="lib/LLVM.dll")]
def LLVMStructSetBody(StructTy : LLVMOpaqueType? implicit; ElementTypes : LLVMOpaqueType ?? implicit; ElementCount : uint; Packed : int) : void {}
[extern(cdecl, name="LLVMCountStructElementTypes", library="lib/LLVM.dll")]
def LLVMCountStructElementTypes(StructTy : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMGetStructElementTypes", library="lib/LLVM.dll")]
def LLVMGetStructElementTypes(StructTy : LLVMOpaqueType? implicit; Dest : LLVMOpaqueType ?? implicit) : void {}
[extern(cdecl, name="LLVMStructGetTypeAtIndex", library="lib/LLVM.dll")]
def LLVMStructGetTypeAtIndex(StructTy : LLVMOpaqueType? implicit; i : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIsPackedStruct", library="lib/LLVM.dll")]
def LLVMIsPackedStruct(StructTy : LLVMOpaqueType? implicit) : int {}
[extern(cdecl, name="LLVMIsOpaqueStruct", library="lib/LLVM.dll")]
def LLVMIsOpaqueStruct(StructTy : LLVMOpaqueType? implicit) : int {}
[extern(cdecl, name="LLVMIsLiteralStruct", library="lib/LLVM.dll")]
def LLVMIsLiteralStruct(StructTy : LLVMOpaqueType? implicit) : int {}
[extern(cdecl, name="LLVMGetElementType", library="lib/LLVM.dll")]
def LLVMGetElementType(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetSubtypes", library="lib/LLVM.dll")]
def LLVMGetSubtypes(Tp : LLVMOpaqueType? implicit; Arr : LLVMOpaqueType ?? implicit) : void {}
[extern(cdecl, name="LLVMGetNumContainedTypes", library="lib/LLVM.dll")]
def LLVMGetNumContainedTypes(Tp : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMArrayType", library="lib/LLVM.dll")]
def LLVMArrayType(ElementType : LLVMOpaqueType? implicit; ElementCount : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetArrayLength", library="lib/LLVM.dll")]
def LLVMGetArrayLength(ArrayTy : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMPointerType", library="lib/LLVM.dll")]
def LLVMPointerType(ElementType : LLVMOpaqueType? implicit; AddressSpace : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMPointerTypeIsOpaque", library="lib/LLVM.dll")]
def LLVMPointerTypeIsOpaque(Ty : LLVMOpaqueType? implicit) : int {}
[extern(cdecl, name="LLVMPointerTypeInContext", library="lib/LLVM.dll")]
def LLVMPointerTypeInContext(C : LLVMOpaqueContext? implicit; AddressSpace : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetPointerAddressSpace", library="lib/LLVM.dll")]
def LLVMGetPointerAddressSpace(PointerTy : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMVectorType", library="lib/LLVM.dll")]
def LLVMVectorType(ElementType : LLVMOpaqueType? implicit; ElementCount : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMScalableVectorType", library="lib/LLVM.dll")]
def LLVMScalableVectorType(ElementType : LLVMOpaqueType? implicit; ElementCount : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetVectorSize", library="lib/LLVM.dll")]
def LLVMGetVectorSize(VectorTy : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMVoidTypeInContext", library="lib/LLVM.dll")]
def LLVMVoidTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMLabelTypeInContext", library="lib/LLVM.dll")]
def LLVMLabelTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMX86MMXTypeInContext", library="lib/LLVM.dll")]
def LLVMX86MMXTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMX86AMXTypeInContext", library="lib/LLVM.dll")]
def LLVMX86AMXTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMTokenTypeInContext", library="lib/LLVM.dll")]
def LLVMTokenTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMMetadataTypeInContext", library="lib/LLVM.dll")]
def LLVMMetadataTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMVoidType", library="lib/LLVM.dll")]
def LLVMVoidType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMLabelType", library="lib/LLVM.dll")]
def LLVMLabelType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMX86MMXType", library="lib/LLVM.dll")]
def LLVMX86MMXType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMX86AMXType", library="lib/LLVM.dll")]
def LLVMX86AMXType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMTargetExtTypeInContext", library="lib/LLVM.dll")]
def LLVMTargetExtTypeInContext(C : LLVMOpaqueContext? implicit; Name : string implicit; TypeParams : LLVMOpaqueType ?? implicit; TypeParamCount : uint; IntParams : uint? implicit; IntParamCount : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMTypeOf", library="lib/LLVM.dll")]
def LLVMTypeOf(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetValueKind", library="lib/LLVM.dll")]
def LLVMGetValueKind(Val : LLVMOpaqueValue? implicit) : LLVMValueKind {}
[extern(cdecl, name="LLVMGetValueName2", library="lib/LLVM.dll")]
def LLVMGetValueName2(Val : LLVMOpaqueValue? implicit; Length : uint64? implicit) : string {}
[extern(cdecl, name="LLVMSetValueName2", library="lib/LLVM.dll")]
def LLVMSetValueName2(Val : LLVMOpaqueValue? implicit; Name : string implicit; NameLen : uint64) : void {}
[extern(cdecl, name="LLVMDumpValue", library="lib/LLVM.dll")]
def LLVMDumpValue(Val : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMPrintValueToString", library="lib/LLVM.dll")]
def LLVMPrintValueToString(Val : LLVMOpaqueValue? implicit) : string {}
[extern(cdecl, name="LLVMReplaceAllUsesWith", library="lib/LLVM.dll")]
def LLVMReplaceAllUsesWith(OldVal : LLVMOpaqueValue? implicit; NewVal : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMIsConstant", library="lib/LLVM.dll")]
def LLVMIsConstant(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMIsUndef", library="lib/LLVM.dll")]
def LLVMIsUndef(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMIsPoison", library="lib/LLVM.dll")]
def LLVMIsPoison(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMIsAArgument", library="lib/LLVM.dll")]
def LLVMIsAArgument(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsABasicBlock", library="lib/LLVM.dll")]
def LLVMIsABasicBlock(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAInlineAsm", library="lib/LLVM.dll")]
def LLVMIsAInlineAsm(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAUser", library="lib/LLVM.dll")]
def LLVMIsAUser(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstant", library="lib/LLVM.dll")]
def LLVMIsAConstant(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsABlockAddress", library="lib/LLVM.dll")]
def LLVMIsABlockAddress(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantAggregateZero", library="lib/LLVM.dll")]
def LLVMIsAConstantAggregateZero(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantArray", library="lib/LLVM.dll")]
def LLVMIsAConstantArray(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantDataSequential", library="lib/LLVM.dll")]
def LLVMIsAConstantDataSequential(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantDataArray", library="lib/LLVM.dll")]
def LLVMIsAConstantDataArray(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantDataVector", library="lib/LLVM.dll")]
def LLVMIsAConstantDataVector(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantExpr", library="lib/LLVM.dll")]
def LLVMIsAConstantExpr(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantFP", library="lib/LLVM.dll")]
def LLVMIsAConstantFP(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantInt", library="lib/LLVM.dll")]
def LLVMIsAConstantInt(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantPointerNull", library="lib/LLVM.dll")]
def LLVMIsAConstantPointerNull(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantStruct", library="lib/LLVM.dll")]
def LLVMIsAConstantStruct(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantTokenNone", library="lib/LLVM.dll")]
def LLVMIsAConstantTokenNone(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantVector", library="lib/LLVM.dll")]
def LLVMIsAConstantVector(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAGlobalValue", library="lib/LLVM.dll")]
def LLVMIsAGlobalValue(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAGlobalAlias", library="lib/LLVM.dll")]
def LLVMIsAGlobalAlias(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAGlobalObject", library="lib/LLVM.dll")]
def LLVMIsAGlobalObject(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFunction", library="lib/LLVM.dll")]
def LLVMIsAFunction(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAGlobalVariable", library="lib/LLVM.dll")]
def LLVMIsAGlobalVariable(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAGlobalIFunc", library="lib/LLVM.dll")]
def LLVMIsAGlobalIFunc(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAUndefValue", library="lib/LLVM.dll")]
def LLVMIsAUndefValue(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAPoisonValue", library="lib/LLVM.dll")]
def LLVMIsAPoisonValue(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAInstruction", library="lib/LLVM.dll")]
def LLVMIsAInstruction(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAUnaryOperator", library="lib/LLVM.dll")]
def LLVMIsAUnaryOperator(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsABinaryOperator", library="lib/LLVM.dll")]
def LLVMIsABinaryOperator(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACallInst", library="lib/LLVM.dll")]
def LLVMIsACallInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAIntrinsicInst", library="lib/LLVM.dll")]
def LLVMIsAIntrinsicInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsADbgInfoIntrinsic", library="lib/LLVM.dll")]
def LLVMIsADbgInfoIntrinsic(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsADbgVariableIntrinsic", library="lib/LLVM.dll")]
def LLVMIsADbgVariableIntrinsic(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsADbgDeclareInst", library="lib/LLVM.dll")]
def LLVMIsADbgDeclareInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsADbgLabelInst", library="lib/LLVM.dll")]
def LLVMIsADbgLabelInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAMemIntrinsic", library="lib/LLVM.dll")]
def LLVMIsAMemIntrinsic(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAMemCpyInst", library="lib/LLVM.dll")]
def LLVMIsAMemCpyInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAMemMoveInst", library="lib/LLVM.dll")]
def LLVMIsAMemMoveInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAMemSetInst", library="lib/LLVM.dll")]
def LLVMIsAMemSetInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACmpInst", library="lib/LLVM.dll")]
def LLVMIsACmpInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFCmpInst", library="lib/LLVM.dll")]
def LLVMIsAFCmpInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAICmpInst", library="lib/LLVM.dll")]
def LLVMIsAICmpInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAExtractElementInst", library="lib/LLVM.dll")]
def LLVMIsAExtractElementInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAGetElementPtrInst", library="lib/LLVM.dll")]
def LLVMIsAGetElementPtrInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAInsertElementInst", library="lib/LLVM.dll")]
def LLVMIsAInsertElementInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAInsertValueInst", library="lib/LLVM.dll")]
def LLVMIsAInsertValueInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsALandingPadInst", library="lib/LLVM.dll")]
def LLVMIsALandingPadInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAPHINode", library="lib/LLVM.dll")]
def LLVMIsAPHINode(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsASelectInst", library="lib/LLVM.dll")]
def LLVMIsASelectInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAShuffleVectorInst", library="lib/LLVM.dll")]
def LLVMIsAShuffleVectorInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAStoreInst", library="lib/LLVM.dll")]
def LLVMIsAStoreInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsABranchInst", library="lib/LLVM.dll")]
def LLVMIsABranchInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAIndirectBrInst", library="lib/LLVM.dll")]
def LLVMIsAIndirectBrInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAInvokeInst", library="lib/LLVM.dll")]
def LLVMIsAInvokeInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAReturnInst", library="lib/LLVM.dll")]
def LLVMIsAReturnInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsASwitchInst", library="lib/LLVM.dll")]
def LLVMIsASwitchInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAUnreachableInst", library="lib/LLVM.dll")]
def LLVMIsAUnreachableInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAResumeInst", library="lib/LLVM.dll")]
def LLVMIsAResumeInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACleanupReturnInst", library="lib/LLVM.dll")]
def LLVMIsACleanupReturnInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACatchReturnInst", library="lib/LLVM.dll")]
def LLVMIsACatchReturnInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACatchSwitchInst", library="lib/LLVM.dll")]
def LLVMIsACatchSwitchInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACallBrInst", library="lib/LLVM.dll")]
def LLVMIsACallBrInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFuncletPadInst", library="lib/LLVM.dll")]
def LLVMIsAFuncletPadInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACatchPadInst", library="lib/LLVM.dll")]
def LLVMIsACatchPadInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACleanupPadInst", library="lib/LLVM.dll")]
def LLVMIsACleanupPadInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAUnaryInstruction", library="lib/LLVM.dll")]
def LLVMIsAUnaryInstruction(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAAllocaInst", library="lib/LLVM.dll")]
def LLVMIsAAllocaInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACastInst", library="lib/LLVM.dll")]
def LLVMIsACastInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAAddrSpaceCastInst", library="lib/LLVM.dll")]
def LLVMIsAAddrSpaceCastInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsABitCastInst", library="lib/LLVM.dll")]
def LLVMIsABitCastInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFPExtInst", library="lib/LLVM.dll")]
def LLVMIsAFPExtInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFPToSIInst", library="lib/LLVM.dll")]
def LLVMIsAFPToSIInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFPToUIInst", library="lib/LLVM.dll")]
def LLVMIsAFPToUIInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFPTruncInst", library="lib/LLVM.dll")]
def LLVMIsAFPTruncInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAIntToPtrInst", library="lib/LLVM.dll")]
def LLVMIsAIntToPtrInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAPtrToIntInst", library="lib/LLVM.dll")]
def LLVMIsAPtrToIntInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsASExtInst", library="lib/LLVM.dll")]
def LLVMIsASExtInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsASIToFPInst", library="lib/LLVM.dll")]
def LLVMIsASIToFPInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsATruncInst", library="lib/LLVM.dll")]
def LLVMIsATruncInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAUIToFPInst", library="lib/LLVM.dll")]
def LLVMIsAUIToFPInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAZExtInst", library="lib/LLVM.dll")]
def LLVMIsAZExtInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAExtractValueInst", library="lib/LLVM.dll")]
def LLVMIsAExtractValueInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsALoadInst", library="lib/LLVM.dll")]
def LLVMIsALoadInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAVAArgInst", library="lib/LLVM.dll")]
def LLVMIsAVAArgInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFreezeInst", library="lib/LLVM.dll")]
def LLVMIsAFreezeInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAAtomicCmpXchgInst", library="lib/LLVM.dll")]
def LLVMIsAAtomicCmpXchgInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAAtomicRMWInst", library="lib/LLVM.dll")]
def LLVMIsAAtomicRMWInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFenceInst", library="lib/LLVM.dll")]
def LLVMIsAFenceInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAMDNode", library="lib/LLVM.dll")]
def LLVMIsAMDNode(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAMDString", library="lib/LLVM.dll")]
def LLVMIsAMDString(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetValueName", library="lib/LLVM.dll")]
def LLVMGetValueName(Val : LLVMOpaqueValue? implicit) : string {}
[extern(cdecl, name="LLVMSetValueName", library="lib/LLVM.dll")]
def LLVMSetValueName(Val : LLVMOpaqueValue? implicit; Name : string implicit) : void {}
[extern(cdecl, name="LLVMGetFirstUse", library="lib/LLVM.dll")]
def LLVMGetFirstUse(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueUse? {}
[extern(cdecl, name="LLVMGetNextUse", library="lib/LLVM.dll")]
def LLVMGetNextUse(U : LLVMOpaqueUse? implicit) : LLVMOpaqueUse? {}
[extern(cdecl, name="LLVMGetUser", library="lib/LLVM.dll")]
def LLVMGetUser(U : LLVMOpaqueUse? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetUsedValue", library="lib/LLVM.dll")]
def LLVMGetUsedValue(U : LLVMOpaqueUse? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetOperand", library="lib/LLVM.dll")]
def LLVMGetOperand(Val : LLVMOpaqueValue? implicit; Index : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetOperandUse", library="lib/LLVM.dll")]
def LLVMGetOperandUse(Val : LLVMOpaqueValue? implicit; Index : uint) : LLVMOpaqueUse? {}
[extern(cdecl, name="LLVMSetOperand", library="lib/LLVM.dll")]
def LLVMSetOperand(User : LLVMOpaqueValue? implicit; Index : uint; Val : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetNumOperands", library="lib/LLVM.dll")]
def LLVMGetNumOperands(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMConstNull", library="lib/LLVM.dll")]
def LLVMConstNull(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstAllOnes", library="lib/LLVM.dll")]
def LLVMConstAllOnes(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetUndef", library="lib/LLVM.dll")]
def LLVMGetUndef(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPoison", library="lib/LLVM.dll")]
def LLVMGetPoison(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsNull", library="lib/LLVM.dll")]
def LLVMIsNull(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMConstPointerNull", library="lib/LLVM.dll")]
def LLVMConstPointerNull(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstInt", library="lib/LLVM.dll")]
def LLVMConstInt(IntTy : LLVMOpaqueType? implicit; N : uint64; SignExtend : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstIntOfArbitraryPrecision", library="lib/LLVM.dll")]
def LLVMConstIntOfArbitraryPrecision(IntTy : LLVMOpaqueType? implicit; NumWords : uint; Words : void? const) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstIntOfString", library="lib/LLVM.dll")]
def LLVMConstIntOfString(IntTy : LLVMOpaqueType? implicit; Text : string implicit; Radix : uint8) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstIntOfStringAndSize", library="lib/LLVM.dll")]
def LLVMConstIntOfStringAndSize(IntTy : LLVMOpaqueType? implicit; Text : string implicit; SLen : uint; Radix : uint8) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstReal", library="lib/LLVM.dll")]
def LLVMConstReal(RealTy : LLVMOpaqueType? implicit; N : double) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstRealOfString", library="lib/LLVM.dll")]
def LLVMConstRealOfString(RealTy : LLVMOpaqueType? implicit; Text : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstRealOfStringAndSize", library="lib/LLVM.dll")]
def LLVMConstRealOfStringAndSize(RealTy : LLVMOpaqueType? implicit; Text : string implicit; SLen : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstIntGetZExtValue", library="lib/LLVM.dll")]
def LLVMConstIntGetZExtValue(ConstantVal : LLVMOpaqueValue? implicit) : uint64 {}
[extern(cdecl, name="LLVMConstIntGetSExtValue", library="lib/LLVM.dll")]
def LLVMConstIntGetSExtValue(ConstantVal : LLVMOpaqueValue? implicit) : int64 {}
[extern(cdecl, name="LLVMConstRealGetDouble", library="lib/LLVM.dll")]
def LLVMConstRealGetDouble(ConstantVal : LLVMOpaqueValue? implicit; losesInfo : int? implicit) : double {}
[extern(cdecl, name="LLVMConstStringInContext", library="lib/LLVM.dll")]
def LLVMConstStringInContext(C : LLVMOpaqueContext? implicit; Str : string implicit; Length : uint; DontNullTerminate : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstString", library="lib/LLVM.dll")]
def LLVMConstString(Str : string implicit; Length : uint; DontNullTerminate : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsConstantString", library="lib/LLVM.dll")]
def LLVMIsConstantString(c : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMGetAsString", library="lib/LLVM.dll")]
def LLVMGetAsString(c : LLVMOpaqueValue? implicit; Length : uint64? implicit) : string {}
[extern(cdecl, name="LLVMConstStructInContext", library="lib/LLVM.dll")]
def LLVMConstStructInContext(C : LLVMOpaqueContext? implicit; ConstantVals : LLVMOpaqueValue ?? implicit; Count : uint; Packed : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstStruct", library="lib/LLVM.dll")]
def LLVMConstStruct(ConstantVals : LLVMOpaqueValue ?? implicit; Count : uint; Packed : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstArray", library="lib/LLVM.dll")]
def LLVMConstArray(ElementTy : LLVMOpaqueType? implicit; ConstantVals : LLVMOpaqueValue ?? implicit; Length : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNamedStruct", library="lib/LLVM.dll")]
def LLVMConstNamedStruct(StructTy : LLVMOpaqueType? implicit; ConstantVals : LLVMOpaqueValue ?? implicit; Count : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetAggregateElement", library="lib/LLVM.dll")]
def LLVMGetAggregateElement(C : LLVMOpaqueValue? implicit; Idx : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetElementAsConstant", library="lib/LLVM.dll")]
def LLVMGetElementAsConstant(C : LLVMOpaqueValue? implicit; idx : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstVector", library="lib/LLVM.dll")]
def LLVMConstVector(ScalarConstantVals : LLVMOpaqueValue ?? implicit; Size : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetConstOpcode", library="lib/LLVM.dll")]
def LLVMGetConstOpcode(ConstantVal : LLVMOpaqueValue? implicit) : LLVMOpcode {}
[extern(cdecl, name="LLVMAlignOf", library="lib/LLVM.dll")]
def LLVMAlignOf(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSizeOf", library="lib/LLVM.dll")]
def LLVMSizeOf(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNeg", library="lib/LLVM.dll")]
def LLVMConstNeg(ConstantVal : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNSWNeg", library="lib/LLVM.dll")]
def LLVMConstNSWNeg(ConstantVal : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNUWNeg", library="lib/LLVM.dll")]
def LLVMConstNUWNeg(ConstantVal : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNot", library="lib/LLVM.dll")]
def LLVMConstNot(ConstantVal : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstAdd", library="lib/LLVM.dll")]
def LLVMConstAdd(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNSWAdd", library="lib/LLVM.dll")]
def LLVMConstNSWAdd(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNUWAdd", library="lib/LLVM.dll")]
def LLVMConstNUWAdd(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstSub", library="lib/LLVM.dll")]
def LLVMConstSub(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNSWSub", library="lib/LLVM.dll")]
def LLVMConstNSWSub(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNUWSub", library="lib/LLVM.dll")]
def LLVMConstNUWSub(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstMul", library="lib/LLVM.dll")]
def LLVMConstMul(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNSWMul", library="lib/LLVM.dll")]
def LLVMConstNSWMul(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNUWMul", library="lib/LLVM.dll")]
def LLVMConstNUWMul(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstAnd", library="lib/LLVM.dll")]
def LLVMConstAnd(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstOr", library="lib/LLVM.dll")]
def LLVMConstOr(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstXor", library="lib/LLVM.dll")]
def LLVMConstXor(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstICmp", library="lib/LLVM.dll")]
def LLVMConstICmp(Predicate : LLVMIntPredicate; LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstFCmp", library="lib/LLVM.dll")]
def LLVMConstFCmp(Predicate : LLVMRealPredicate; LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstShl", library="lib/LLVM.dll")]
def LLVMConstShl(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstLShr", library="lib/LLVM.dll")]
def LLVMConstLShr(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstAShr", library="lib/LLVM.dll")]
def LLVMConstAShr(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstGEP2", library="lib/LLVM.dll")]
def LLVMConstGEP2(Ty : LLVMOpaqueType? implicit; ConstantVal : LLVMOpaqueValue? implicit; ConstantIndices : LLVMOpaqueValue ?? implicit; NumIndices : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstInBoundsGEP2", library="lib/LLVM.dll")]
def LLVMConstInBoundsGEP2(Ty : LLVMOpaqueType? implicit; ConstantVal : LLVMOpaqueValue? implicit; ConstantIndices : LLVMOpaqueValue ?? implicit; NumIndices : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstTrunc", library="lib/LLVM.dll")]
def LLVMConstTrunc(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstSExt", library="lib/LLVM.dll")]
def LLVMConstSExt(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstZExt", library="lib/LLVM.dll")]
def LLVMConstZExt(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstFPTrunc", library="lib/LLVM.dll")]
def LLVMConstFPTrunc(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstFPExt", library="lib/LLVM.dll")]
def LLVMConstFPExt(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstUIToFP", library="lib/LLVM.dll")]
def LLVMConstUIToFP(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstSIToFP", library="lib/LLVM.dll")]
def LLVMConstSIToFP(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstFPToUI", library="lib/LLVM.dll")]
def LLVMConstFPToUI(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstFPToSI", library="lib/LLVM.dll")]
def LLVMConstFPToSI(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstPtrToInt", library="lib/LLVM.dll")]
def LLVMConstPtrToInt(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstIntToPtr", library="lib/LLVM.dll")]
def LLVMConstIntToPtr(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstBitCast", library="lib/LLVM.dll")]
def LLVMConstBitCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstAddrSpaceCast", library="lib/LLVM.dll")]
def LLVMConstAddrSpaceCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstZExtOrBitCast", library="lib/LLVM.dll")]
def LLVMConstZExtOrBitCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstSExtOrBitCast", library="lib/LLVM.dll")]
def LLVMConstSExtOrBitCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstTruncOrBitCast", library="lib/LLVM.dll")]
def LLVMConstTruncOrBitCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstPointerCast", library="lib/LLVM.dll")]
def LLVMConstPointerCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstIntCast", library="lib/LLVM.dll")]
def LLVMConstIntCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit; isSigned : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstFPCast", library="lib/LLVM.dll")]
def LLVMConstFPCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstSelect", library="lib/LLVM.dll")]
def LLVMConstSelect(ConstantCondition : LLVMOpaqueValue? implicit; ConstantIfTrue : LLVMOpaqueValue? implicit; ConstantIfFalse : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstExtractElement", library="lib/LLVM.dll")]
def LLVMConstExtractElement(VectorConstant : LLVMOpaqueValue? implicit; IndexConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstInsertElement", library="lib/LLVM.dll")]
def LLVMConstInsertElement(VectorConstant : LLVMOpaqueValue? implicit; ElementValueConstant : LLVMOpaqueValue? implicit; IndexConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstShuffleVector", library="lib/LLVM.dll")]
def LLVMConstShuffleVector(VectorAConstant : LLVMOpaqueValue? implicit; VectorBConstant : LLVMOpaqueValue? implicit; MaskConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBlockAddress", library="lib/LLVM.dll")]
def LLVMBlockAddress(F : LLVMOpaqueValue? implicit; BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstInlineAsm", library="lib/LLVM.dll")]
def LLVMConstInlineAsm(Ty : LLVMOpaqueType? implicit; AsmString : string implicit; Constraints : string implicit; HasSideEffects : int; IsAlignStack : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetGlobalParent", library="lib/LLVM.dll")]
def LLVMGetGlobalParent(Global : LLVMOpaqueValue? implicit) : LLVMOpaqueModule? {}
[extern(cdecl, name="LLVMIsDeclaration", library="lib/LLVM.dll")]
def LLVMIsDeclaration(Global : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMGetLinkage", library="lib/LLVM.dll")]
def LLVMGetLinkage(Global : LLVMOpaqueValue? implicit) : LLVMLinkage {}
[extern(cdecl, name="LLVMSetLinkage", library="lib/LLVM.dll")]
def LLVMSetLinkage(Global : LLVMOpaqueValue? implicit; Linkage : LLVMLinkage) : void {}
[extern(cdecl, name="LLVMGetSection", library="lib/LLVM.dll")]
def LLVMGetSection(Global : LLVMOpaqueValue? implicit) : string {}
[extern(cdecl, name="LLVMSetSection", library="lib/LLVM.dll")]
def LLVMSetSection(Global : LLVMOpaqueValue? implicit; Section : string implicit) : void {}
[extern(cdecl, name="LLVMGetVisibility", library="lib/LLVM.dll")]
def LLVMGetVisibility(Global : LLVMOpaqueValue? implicit) : LLVMVisibility {}
[extern(cdecl, name="LLVMSetVisibility", library="lib/LLVM.dll")]
def LLVMSetVisibility(Global : LLVMOpaqueValue? implicit; Viz : LLVMVisibility) : void {}
[extern(cdecl, name="LLVMGetDLLStorageClass", library="lib/LLVM.dll")]
def LLVMGetDLLStorageClass(Global : LLVMOpaqueValue? implicit) : LLVMDLLStorageClass {}
[extern(cdecl, name="LLVMSetDLLStorageClass", library="lib/LLVM.dll")]
def LLVMSetDLLStorageClass(Global : LLVMOpaqueValue? implicit; Class : LLVMDLLStorageClass) : void {}
[extern(cdecl, name="LLVMGetUnnamedAddress", library="lib/LLVM.dll")]
def LLVMGetUnnamedAddress(Global : LLVMOpaqueValue? implicit) : LLVMUnnamedAddr {}
[extern(cdecl, name="LLVMSetUnnamedAddress", library="lib/LLVM.dll")]
def LLVMSetUnnamedAddress(Global : LLVMOpaqueValue? implicit; UnnamedAddr : LLVMUnnamedAddr) : void {}
[extern(cdecl, name="LLVMGlobalGetValueType", library="lib/LLVM.dll")]
def LLVMGlobalGetValueType(Global : LLVMOpaqueValue? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMHasUnnamedAddr", library="lib/LLVM.dll")]
def LLVMHasUnnamedAddr(Global : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetUnnamedAddr", library="lib/LLVM.dll")]
def LLVMSetUnnamedAddr(Global : LLVMOpaqueValue? implicit; HasUnnamedAddr : int) : void {}
[extern(cdecl, name="LLVMGetAlignment", library="lib/LLVM.dll")]
def LLVMGetAlignment(V : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMSetAlignment", library="lib/LLVM.dll")]
def LLVMSetAlignment(V : LLVMOpaqueValue? implicit; Bytes : uint) : void {}
[extern(cdecl, name="LLVMGlobalSetMetadata", library="lib/LLVM.dll")]
def LLVMGlobalSetMetadata(Global : LLVMOpaqueValue? implicit; Kind : uint; MD : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMGlobalEraseMetadata", library="lib/LLVM.dll")]
def LLVMGlobalEraseMetadata(Global : LLVMOpaqueValue? implicit; Kind : uint) : void {}
[extern(cdecl, name="LLVMGlobalClearMetadata", library="lib/LLVM.dll")]
def LLVMGlobalClearMetadata(Global : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGlobalCopyAllMetadata", library="lib/LLVM.dll")]
def LLVMGlobalCopyAllMetadata(Value : LLVMOpaqueValue? implicit; NumEntries : uint64? implicit) : LLVMOpaqueValueMetadataEntry? {}
[extern(cdecl, name="LLVMDisposeValueMetadataEntries", library="lib/LLVM.dll")]
def LLVMDisposeValueMetadataEntries(Entries : LLVMOpaqueValueMetadataEntry? implicit) : void {}
[extern(cdecl, name="LLVMValueMetadataEntriesGetKind", library="lib/LLVM.dll")]
def LLVMValueMetadataEntriesGetKind(Entries : LLVMOpaqueValueMetadataEntry? implicit; Index : uint) : uint {}
[extern(cdecl, name="LLVMValueMetadataEntriesGetMetadata", library="lib/LLVM.dll")]
def LLVMValueMetadataEntriesGetMetadata(Entries : LLVMOpaqueValueMetadataEntry? implicit; Index : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMAddGlobal", library="lib/LLVM.dll")]
def LLVMAddGlobal(M : LLVMOpaqueModule? implicit; Ty : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMAddGlobalInAddressSpace", library="lib/LLVM.dll")]
def LLVMAddGlobalInAddressSpace(M : LLVMOpaqueModule? implicit; Ty : LLVMOpaqueType? implicit; Name : string implicit; AddressSpace : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNamedGlobal", library="lib/LLVM.dll")]
def LLVMGetNamedGlobal(M : LLVMOpaqueModule? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetFirstGlobal", library="lib/LLVM.dll")]
def LLVMGetFirstGlobal(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetLastGlobal", library="lib/LLVM.dll")]
def LLVMGetLastGlobal(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNextGlobal", library="lib/LLVM.dll")]
def LLVMGetNextGlobal(GlobalVar : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPreviousGlobal", library="lib/LLVM.dll")]
def LLVMGetPreviousGlobal(GlobalVar : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMDeleteGlobal", library="lib/LLVM.dll")]
def LLVMDeleteGlobal(GlobalVar : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetInitializer", library="lib/LLVM.dll")]
def LLVMGetInitializer(GlobalVar : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetInitializer", library="lib/LLVM.dll")]
def LLVMSetInitializer(GlobalVar : LLVMOpaqueValue? implicit; ConstantVal : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMIsThreadLocal", library="lib/LLVM.dll")]
def LLVMIsThreadLocal(GlobalVar : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetThreadLocal", library="lib/LLVM.dll")]
def LLVMSetThreadLocal(GlobalVar : LLVMOpaqueValue? implicit; IsThreadLocal : int) : void {}
[extern(cdecl, name="LLVMIsGlobalConstant", library="lib/LLVM.dll")]
def LLVMIsGlobalConstant(GlobalVar : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetGlobalConstant", library="lib/LLVM.dll")]
def LLVMSetGlobalConstant(GlobalVar : LLVMOpaqueValue? implicit; IsConstant : int) : void {}
[extern(cdecl, name="LLVMGetThreadLocalMode", library="lib/LLVM.dll")]
def LLVMGetThreadLocalMode(GlobalVar : LLVMOpaqueValue? implicit) : LLVMThreadLocalMode {}
[extern(cdecl, name="LLVMSetThreadLocalMode", library="lib/LLVM.dll")]
def LLVMSetThreadLocalMode(GlobalVar : LLVMOpaqueValue? implicit; Mode : LLVMThreadLocalMode) : void {}
[extern(cdecl, name="LLVMIsExternallyInitialized", library="lib/LLVM.dll")]
def LLVMIsExternallyInitialized(GlobalVar : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetExternallyInitialized", library="lib/LLVM.dll")]
def LLVMSetExternallyInitialized(GlobalVar : LLVMOpaqueValue? implicit; IsExtInit : int) : void {}
[extern(cdecl, name="LLVMAddAlias2", library="lib/LLVM.dll")]
def LLVMAddAlias2(M : LLVMOpaqueModule? implicit; ValueTy : LLVMOpaqueType? implicit; AddrSpace : uint; Aliasee : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNamedGlobalAlias", library="lib/LLVM.dll")]
def LLVMGetNamedGlobalAlias(M : LLVMOpaqueModule? implicit; Name : string implicit; NameLen : uint64) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetFirstGlobalAlias", library="lib/LLVM.dll")]
def LLVMGetFirstGlobalAlias(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetLastGlobalAlias", library="lib/LLVM.dll")]
def LLVMGetLastGlobalAlias(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNextGlobalAlias", library="lib/LLVM.dll")]
def LLVMGetNextGlobalAlias(GA : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPreviousGlobalAlias", library="lib/LLVM.dll")]
def LLVMGetPreviousGlobalAlias(GA : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMAliasGetAliasee", library="lib/LLVM.dll")]
def LLVMAliasGetAliasee(Alias : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMAliasSetAliasee", library="lib/LLVM.dll")]
def LLVMAliasSetAliasee(Alias : LLVMOpaqueValue? implicit; Aliasee : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMDeleteFunction", library="lib/LLVM.dll")]
def LLVMDeleteFunction(Fn : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMHasPersonalityFn", library="lib/LLVM.dll")]
def LLVMHasPersonalityFn(Fn : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMGetPersonalityFn", library="lib/LLVM.dll")]
def LLVMGetPersonalityFn(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetPersonalityFn", library="lib/LLVM.dll")]
def LLVMSetPersonalityFn(Fn : LLVMOpaqueValue? implicit; PersonalityFn : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMLookupIntrinsicID", library="lib/LLVM.dll")]
def LLVMLookupIntrinsicID(Name : string implicit; NameLen : uint64) : uint {}
[extern(cdecl, name="LLVMGetIntrinsicID", library="lib/LLVM.dll")]
def LLVMGetIntrinsicID(Fn : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetIntrinsicDeclaration", library="lib/LLVM.dll")]
def LLVMGetIntrinsicDeclaration(Mod : LLVMOpaqueModule? implicit; ID : uint; ParamTypes : LLVMOpaqueType ?? implicit; ParamCount : uint64) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIntrinsicGetType", library="lib/LLVM.dll")]
def LLVMIntrinsicGetType(Ctx : LLVMOpaqueContext? implicit; ID : uint; ParamTypes : LLVMOpaqueType ?? implicit; ParamCount : uint64) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIntrinsicGetName", library="lib/LLVM.dll")]
def LLVMIntrinsicGetName(ID : uint; NameLength : uint64? implicit) : string {}
[extern(cdecl, name="LLVMIntrinsicCopyOverloadedName", library="lib/LLVM.dll")]
def LLVMIntrinsicCopyOverloadedName(ID : uint; ParamTypes : LLVMOpaqueType ?? implicit; ParamCount : uint64; NameLength : uint64? implicit) : string {}
[extern(cdecl, name="LLVMIntrinsicCopyOverloadedName2", library="lib/LLVM.dll")]
def LLVMIntrinsicCopyOverloadedName2(Mod : LLVMOpaqueModule? implicit; ID : uint; ParamTypes : LLVMOpaqueType ?? implicit; ParamCount : uint64; NameLength : uint64? implicit) : string {}
[extern(cdecl, name="LLVMIntrinsicIsOverloaded", library="lib/LLVM.dll")]
def LLVMIntrinsicIsOverloaded(ID : uint) : int {}
[extern(cdecl, name="LLVMGetFunctionCallConv", library="lib/LLVM.dll")]
def LLVMGetFunctionCallConv(Fn : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMSetFunctionCallConv", library="lib/LLVM.dll")]
def LLVMSetFunctionCallConv(Fn : LLVMOpaqueValue? implicit; CC : uint) : void {}
[extern(cdecl, name="LLVMGetGC", library="lib/LLVM.dll")]
def LLVMGetGC(Fn : LLVMOpaqueValue? implicit) : string {}
[extern(cdecl, name="LLVMSetGC", library="lib/LLVM.dll")]
def LLVMSetGC(Fn : LLVMOpaqueValue? implicit; Name : string implicit) : void {}
[extern(cdecl, name="LLVMAddAttributeAtIndex", library="lib/LLVM.dll")]
def LLVMAddAttributeAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint; A : LLVMOpaqueAttributeRef? implicit) : void {}
[extern(cdecl, name="LLVMGetAttributeCountAtIndex", library="lib/LLVM.dll")]
def LLVMGetAttributeCountAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint) : uint {}
[extern(cdecl, name="LLVMGetAttributesAtIndex", library="lib/LLVM.dll")]
def LLVMGetAttributesAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint; Attrs : LLVMOpaqueAttributeRef ?? implicit) : void {}
[extern(cdecl, name="LLVMGetEnumAttributeAtIndex", library="lib/LLVM.dll")]
def LLVMGetEnumAttributeAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint; KindID : uint) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMGetStringAttributeAtIndex", library="lib/LLVM.dll")]
def LLVMGetStringAttributeAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint; K : string implicit; KLen : uint) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMRemoveEnumAttributeAtIndex", library="lib/LLVM.dll")]
def LLVMRemoveEnumAttributeAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint; KindID : uint) : void {}
[extern(cdecl, name="LLVMRemoveStringAttributeAtIndex", library="lib/LLVM.dll")]
def LLVMRemoveStringAttributeAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint; K : string implicit; KLen : uint) : void {}
[extern(cdecl, name="LLVMAddTargetDependentFunctionAttr", library="lib/LLVM.dll")]
def LLVMAddTargetDependentFunctionAttr(Fn : LLVMOpaqueValue? implicit; A : string implicit; V : string implicit) : void {}
[extern(cdecl, name="LLVMCountParams", library="lib/LLVM.dll")]
def LLVMCountParams(Fn : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetParams", library="lib/LLVM.dll")]
def LLVMGetParams(Fn : LLVMOpaqueValue? implicit; Params : LLVMOpaqueValue ?? implicit) : void {}
[extern(cdecl, name="LLVMGetParam", library="lib/LLVM.dll")]
def LLVMGetParam(Fn : LLVMOpaqueValue? implicit; Index : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetParamParent", library="lib/LLVM.dll")]
def LLVMGetParamParent(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetFirstParam", library="lib/LLVM.dll")]
def LLVMGetFirstParam(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetLastParam", library="lib/LLVM.dll")]
def LLVMGetLastParam(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNextParam", library="lib/LLVM.dll")]
def LLVMGetNextParam(Arg : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPreviousParam", library="lib/LLVM.dll")]
def LLVMGetPreviousParam(Arg : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetParamAlignment", library="lib/LLVM.dll")]
def LLVMSetParamAlignment(Arg : LLVMOpaqueValue? implicit; Align : uint) : void {}
[extern(cdecl, name="LLVMAddGlobalIFunc", library="lib/LLVM.dll")]
def LLVMAddGlobalIFunc(M : LLVMOpaqueModule? implicit; Name : string implicit; NameLen : uint64; Ty : LLVMOpaqueType? implicit; AddrSpace : uint; Resolver : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNamedGlobalIFunc", library="lib/LLVM.dll")]
def LLVMGetNamedGlobalIFunc(M : LLVMOpaqueModule? implicit; Name : string implicit; NameLen : uint64) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetFirstGlobalIFunc", library="lib/LLVM.dll")]
def LLVMGetFirstGlobalIFunc(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetLastGlobalIFunc", library="lib/LLVM.dll")]
def LLVMGetLastGlobalIFunc(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNextGlobalIFunc", library="lib/LLVM.dll")]
def LLVMGetNextGlobalIFunc(IFunc : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPreviousGlobalIFunc", library="lib/LLVM.dll")]
def LLVMGetPreviousGlobalIFunc(IFunc : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetGlobalIFuncResolver", library="lib/LLVM.dll")]
def LLVMGetGlobalIFuncResolver(IFunc : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetGlobalIFuncResolver", library="lib/LLVM.dll")]
def LLVMSetGlobalIFuncResolver(IFunc : LLVMOpaqueValue? implicit; Resolver : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMEraseGlobalIFunc", library="lib/LLVM.dll")]
def LLVMEraseGlobalIFunc(IFunc : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMRemoveGlobalIFunc", library="lib/LLVM.dll")]
def LLVMRemoveGlobalIFunc(IFunc : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMMDStringInContext2", library="lib/LLVM.dll")]
def LLVMMDStringInContext2(C : LLVMOpaqueContext? implicit; Str : string implicit; SLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMMDNodeInContext2", library="lib/LLVM.dll")]
def LLVMMDNodeInContext2(C : LLVMOpaqueContext? implicit; MDs : LLVMOpaqueMetadata ?? implicit; Count : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMMetadataAsValue", library="lib/LLVM.dll")]
def LLVMMetadataAsValue(C : LLVMOpaqueContext? implicit; MD : LLVMOpaqueMetadata? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMValueAsMetadata", library="lib/LLVM.dll")]
def LLVMValueAsMetadata(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMGetMDString", library="lib/LLVM.dll")]
def LLVMGetMDString(V : LLVMOpaqueValue? implicit; Length : uint? implicit) : string {}
[extern(cdecl, name="LLVMGetMDNodeNumOperands", library="lib/LLVM.dll")]
def LLVMGetMDNodeNumOperands(V : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetMDNodeOperands", library="lib/LLVM.dll")]
def LLVMGetMDNodeOperands(V : LLVMOpaqueValue? implicit; Dest : LLVMOpaqueValue ?? implicit) : void {}
[extern(cdecl, name="LLVMMDStringInContext", library="lib/LLVM.dll")]
def LLVMMDStringInContext(C : LLVMOpaqueContext? implicit; Str : string implicit; SLen : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMMDString", library="lib/LLVM.dll")]
def LLVMMDString(Str : string implicit; SLen : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMMDNodeInContext", library="lib/LLVM.dll")]
def LLVMMDNodeInContext(C : LLVMOpaqueContext? implicit; Vals : LLVMOpaqueValue ?? implicit; Count : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMMDNode", library="lib/LLVM.dll")]
def LLVMMDNode(Vals : LLVMOpaqueValue ?? implicit; Count : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBasicBlockAsValue", library="lib/LLVM.dll")]
def LLVMBasicBlockAsValue(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMValueIsBasicBlock", library="lib/LLVM.dll")]
def LLVMValueIsBasicBlock(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMValueAsBasicBlock", library="lib/LLVM.dll")]
def LLVMValueAsBasicBlock(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetBasicBlockName", library="lib/LLVM.dll")]
def LLVMGetBasicBlockName(BB : LLVMOpaqueBasicBlock? implicit) : string {}
[extern(cdecl, name="LLVMGetBasicBlockParent", library="lib/LLVM.dll")]
def LLVMGetBasicBlockParent(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetBasicBlockTerminator", library="lib/LLVM.dll")]
def LLVMGetBasicBlockTerminator(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMCountBasicBlocks", library="lib/LLVM.dll")]
def LLVMCountBasicBlocks(Fn : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetBasicBlocks", library="lib/LLVM.dll")]
def LLVMGetBasicBlocks(Fn : LLVMOpaqueValue? implicit; BasicBlocks : LLVMOpaqueBasicBlock ?? implicit) : void {}
[extern(cdecl, name="LLVMGetFirstBasicBlock", library="lib/LLVM.dll")]
def LLVMGetFirstBasicBlock(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetLastBasicBlock", library="lib/LLVM.dll")]
def LLVMGetLastBasicBlock(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetNextBasicBlock", library="lib/LLVM.dll")]
def LLVMGetNextBasicBlock(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetPreviousBasicBlock", library="lib/LLVM.dll")]
def LLVMGetPreviousBasicBlock(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetEntryBasicBlock", library="lib/LLVM.dll")]
def LLVMGetEntryBasicBlock(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMInsertExistingBasicBlockAfterInsertBlock", library="lib/LLVM.dll")]
def LLVMInsertExistingBasicBlockAfterInsertBlock(Builder : LLVMOpaqueBuilder? implicit; BB : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMAppendExistingBasicBlock", library="lib/LLVM.dll")]
def LLVMAppendExistingBasicBlock(Fn : LLVMOpaqueValue? implicit; BB : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMCreateBasicBlockInContext", library="lib/LLVM.dll")]
def LLVMCreateBasicBlockInContext(C : LLVMOpaqueContext? implicit; Name : string implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMAppendBasicBlockInContext", library="lib/LLVM.dll")]
def LLVMAppendBasicBlockInContext(C : LLVMOpaqueContext? implicit; Fn : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMAppendBasicBlock", library="lib/LLVM.dll")]
def LLVMAppendBasicBlock(Fn : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMInsertBasicBlockInContext", library="lib/LLVM.dll")]
def LLVMInsertBasicBlockInContext(C : LLVMOpaqueContext? implicit; BB : LLVMOpaqueBasicBlock? implicit; Name : string implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMInsertBasicBlock", library="lib/LLVM.dll")]
def LLVMInsertBasicBlock(InsertBeforeBB : LLVMOpaqueBasicBlock? implicit; Name : string implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMDeleteBasicBlock", library="lib/LLVM.dll")]
def LLVMDeleteBasicBlock(BB : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMRemoveBasicBlockFromParent", library="lib/LLVM.dll")]
def LLVMRemoveBasicBlockFromParent(BB : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMMoveBasicBlockBefore", library="lib/LLVM.dll")]
def LLVMMoveBasicBlockBefore(BB : LLVMOpaqueBasicBlock? implicit; MovePos : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMMoveBasicBlockAfter", library="lib/LLVM.dll")]
def LLVMMoveBasicBlockAfter(BB : LLVMOpaqueBasicBlock? implicit; MovePos : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMGetFirstInstruction", library="lib/LLVM.dll")]
def LLVMGetFirstInstruction(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetLastInstruction", library="lib/LLVM.dll")]
def LLVMGetLastInstruction(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMHasMetadata", library="lib/LLVM.dll")]
def LLVMHasMetadata(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMGetMetadata", library="lib/LLVM.dll")]
def LLVMGetMetadata(Val : LLVMOpaqueValue? implicit; KindID : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetMetadata", library="lib/LLVM.dll")]
def LLVMSetMetadata(Val : LLVMOpaqueValue? implicit; KindID : uint; Node : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMInstructionGetAllMetadataOtherThanDebugLoc", library="lib/LLVM.dll")]
def LLVMInstructionGetAllMetadataOtherThanDebugLoc(Instr : LLVMOpaqueValue? implicit; NumEntries : uint64? implicit) : LLVMOpaqueValueMetadataEntry? {}
[extern(cdecl, name="LLVMGetInstructionParent", library="lib/LLVM.dll")]
def LLVMGetInstructionParent(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetNextInstruction", library="lib/LLVM.dll")]
def LLVMGetNextInstruction(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPreviousInstruction", library="lib/LLVM.dll")]
def LLVMGetPreviousInstruction(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMInstructionRemoveFromParent", library="lib/LLVM.dll")]
def LLVMInstructionRemoveFromParent(Inst : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMInstructionEraseFromParent", library="lib/LLVM.dll")]
def LLVMInstructionEraseFromParent(Inst : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMDeleteInstruction", library="lib/LLVM.dll")]
def LLVMDeleteInstruction(Inst : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetInstructionOpcode", library="lib/LLVM.dll")]
def LLVMGetInstructionOpcode(Inst : LLVMOpaqueValue? implicit) : LLVMOpcode {}
[extern(cdecl, name="LLVMGetICmpPredicate", library="lib/LLVM.dll")]
def LLVMGetICmpPredicate(Inst : LLVMOpaqueValue? implicit) : LLVMIntPredicate {}
[extern(cdecl, name="LLVMGetFCmpPredicate", library="lib/LLVM.dll")]
def LLVMGetFCmpPredicate(Inst : LLVMOpaqueValue? implicit) : LLVMRealPredicate {}
[extern(cdecl, name="LLVMInstructionClone", library="lib/LLVM.dll")]
def LLVMInstructionClone(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsATerminatorInst", library="lib/LLVM.dll")]
def LLVMIsATerminatorInst(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNumArgOperands", library="lib/LLVM.dll")]
def LLVMGetNumArgOperands(Instr : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMSetInstructionCallConv", library="lib/LLVM.dll")]
def LLVMSetInstructionCallConv(Instr : LLVMOpaqueValue? implicit; CC : uint) : void {}
[extern(cdecl, name="LLVMGetInstructionCallConv", library="lib/LLVM.dll")]
def LLVMGetInstructionCallConv(Instr : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMSetInstrParamAlignment", library="lib/LLVM.dll")]
def LLVMSetInstrParamAlignment(Instr : LLVMOpaqueValue? implicit; Idx : uint; Align : uint) : void {}
[extern(cdecl, name="LLVMAddCallSiteAttribute", library="lib/LLVM.dll")]
def LLVMAddCallSiteAttribute(C : LLVMOpaqueValue? implicit; Idx : uint; A : LLVMOpaqueAttributeRef? implicit) : void {}
[extern(cdecl, name="LLVMGetCallSiteAttributeCount", library="lib/LLVM.dll")]
def LLVMGetCallSiteAttributeCount(C : LLVMOpaqueValue? implicit; Idx : uint) : uint {}
[extern(cdecl, name="LLVMGetCallSiteAttributes", library="lib/LLVM.dll")]
def LLVMGetCallSiteAttributes(C : LLVMOpaqueValue? implicit; Idx : uint; Attrs : LLVMOpaqueAttributeRef ?? implicit) : void {}
[extern(cdecl, name="LLVMGetCallSiteEnumAttribute", library="lib/LLVM.dll")]
def LLVMGetCallSiteEnumAttribute(C : LLVMOpaqueValue? implicit; Idx : uint; KindID : uint) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMGetCallSiteStringAttribute", library="lib/LLVM.dll")]
def LLVMGetCallSiteStringAttribute(C : LLVMOpaqueValue? implicit; Idx : uint; K : string implicit; KLen : uint) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMRemoveCallSiteEnumAttribute", library="lib/LLVM.dll")]
def LLVMRemoveCallSiteEnumAttribute(C : LLVMOpaqueValue? implicit; Idx : uint; KindID : uint) : void {}
[extern(cdecl, name="LLVMRemoveCallSiteStringAttribute", library="lib/LLVM.dll")]
def LLVMRemoveCallSiteStringAttribute(C : LLVMOpaqueValue? implicit; Idx : uint; K : string implicit; KLen : uint) : void {}
[extern(cdecl, name="LLVMGetCalledFunctionType", library="lib/LLVM.dll")]
def LLVMGetCalledFunctionType(C : LLVMOpaqueValue? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetCalledValue", library="lib/LLVM.dll")]
def LLVMGetCalledValue(Instr : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsTailCall", library="lib/LLVM.dll")]
def LLVMIsTailCall(CallInst : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetTailCall", library="lib/LLVM.dll")]
def LLVMSetTailCall(CallInst : LLVMOpaqueValue? implicit; IsTailCall : int) : void {}
[extern(cdecl, name="LLVMGetNormalDest", library="lib/LLVM.dll")]
def LLVMGetNormalDest(InvokeInst : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetUnwindDest", library="lib/LLVM.dll")]
def LLVMGetUnwindDest(InvokeInst : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMSetNormalDest", library="lib/LLVM.dll")]
def LLVMSetNormalDest(InvokeInst : LLVMOpaqueValue? implicit; B : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMSetUnwindDest", library="lib/LLVM.dll")]
def LLVMSetUnwindDest(InvokeInst : LLVMOpaqueValue? implicit; B : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMGetNumSuccessors", library="lib/LLVM.dll")]
def LLVMGetNumSuccessors(Term : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetSuccessor", library="lib/LLVM.dll")]
def LLVMGetSuccessor(Term : LLVMOpaqueValue? implicit; i : uint) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMSetSuccessor", library="lib/LLVM.dll")]
def LLVMSetSuccessor(Term : LLVMOpaqueValue? implicit; i : uint; _block : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMIsConditional", library="lib/LLVM.dll")]
def LLVMIsConditional(Branch : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMGetCondition", library="lib/LLVM.dll")]
def LLVMGetCondition(Branch : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetCondition", library="lib/LLVM.dll")]
def LLVMSetCondition(Branch : LLVMOpaqueValue? implicit; Cond : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetSwitchDefaultDest", library="lib/LLVM.dll")]
def LLVMGetSwitchDefaultDest(SwitchInstr : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetAllocatedType", library="lib/LLVM.dll")]
def LLVMGetAllocatedType(Alloca : LLVMOpaqueValue? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIsInBounds", library="lib/LLVM.dll")]
def LLVMIsInBounds(GEP : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetIsInBounds", library="lib/LLVM.dll")]
def LLVMSetIsInBounds(GEP : LLVMOpaqueValue? implicit; InBounds : int) : void {}
[extern(cdecl, name="LLVMGetGEPSourceElementType", library="lib/LLVM.dll")]
def LLVMGetGEPSourceElementType(GEP : LLVMOpaqueValue? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMAddIncoming", library="lib/LLVM.dll")]
def LLVMAddIncoming(PhiNode : LLVMOpaqueValue? implicit; IncomingValues : LLVMOpaqueValue ?? implicit; IncomingBlocks : LLVMOpaqueBasicBlock ?? implicit; Count : uint) : void {}
[extern(cdecl, name="LLVMCountIncoming", library="lib/LLVM.dll")]
def LLVMCountIncoming(PhiNode : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetIncomingValue", library="lib/LLVM.dll")]
def LLVMGetIncomingValue(PhiNode : LLVMOpaqueValue? implicit; Index : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetIncomingBlock", library="lib/LLVM.dll")]
def LLVMGetIncomingBlock(PhiNode : LLVMOpaqueValue? implicit; Index : uint) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetNumIndices", library="lib/LLVM.dll")]
def LLVMGetNumIndices(Inst : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetIndices", library="lib/LLVM.dll")]
def LLVMGetIndices(Inst : LLVMOpaqueValue? implicit) : uint const? {}
[extern(cdecl, name="LLVMCreateBuilderInContext", library="lib/LLVM.dll")]
def LLVMCreateBuilderInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueBuilder? {}
[extern(cdecl, name="LLVMCreateBuilder", library="lib/LLVM.dll")]
def LLVMCreateBuilder() : LLVMOpaqueBuilder? {}
[extern(cdecl, name="LLVMPositionBuilder", library="lib/LLVM.dll")]
def LLVMPositionBuilder(Builder : LLVMOpaqueBuilder? implicit; Block : LLVMOpaqueBasicBlock? implicit; Instr : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMPositionBuilderBefore", library="lib/LLVM.dll")]
def LLVMPositionBuilderBefore(Builder : LLVMOpaqueBuilder? implicit; Instr : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMPositionBuilderAtEnd", library="lib/LLVM.dll")]
def LLVMPositionBuilderAtEnd(Builder : LLVMOpaqueBuilder? implicit; Block : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMGetInsertBlock", library="lib/LLVM.dll")]
def LLVMGetInsertBlock(Builder : LLVMOpaqueBuilder? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMClearInsertionPosition", library="lib/LLVM.dll")]
def LLVMClearInsertionPosition(Builder : LLVMOpaqueBuilder? implicit) : void {}
[extern(cdecl, name="LLVMInsertIntoBuilder", library="lib/LLVM.dll")]
def LLVMInsertIntoBuilder(Builder : LLVMOpaqueBuilder? implicit; Instr : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMInsertIntoBuilderWithName", library="lib/LLVM.dll")]
def LLVMInsertIntoBuilderWithName(Builder : LLVMOpaqueBuilder? implicit; Instr : LLVMOpaqueValue? implicit; Name : string implicit) : void {}
[extern(cdecl, name="LLVMDisposeBuilder", library="lib/LLVM.dll")]
def LLVMDisposeBuilder(Builder : LLVMOpaqueBuilder? implicit) : void {}
[extern(cdecl, name="LLVMGetCurrentDebugLocation2", library="lib/LLVM.dll")]
def LLVMGetCurrentDebugLocation2(Builder : LLVMOpaqueBuilder? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMSetCurrentDebugLocation2", library="lib/LLVM.dll")]
def LLVMSetCurrentDebugLocation2(Builder : LLVMOpaqueBuilder? implicit; Loc : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMSetInstDebugLocation", library="lib/LLVM.dll")]
def LLVMSetInstDebugLocation(Builder : LLVMOpaqueBuilder? implicit; Inst : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMAddMetadataToInst", library="lib/LLVM.dll")]
def LLVMAddMetadataToInst(Builder : LLVMOpaqueBuilder? implicit; Inst : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMBuilderGetDefaultFPMathTag", library="lib/LLVM.dll")]
def LLVMBuilderGetDefaultFPMathTag(Builder : LLVMOpaqueBuilder? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMBuilderSetDefaultFPMathTag", library="lib/LLVM.dll")]
def LLVMBuilderSetDefaultFPMathTag(Builder : LLVMOpaqueBuilder? implicit; FPMathTag : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMSetCurrentDebugLocation", library="lib/LLVM.dll")]
def LLVMSetCurrentDebugLocation(Builder : LLVMOpaqueBuilder? implicit; L : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetCurrentDebugLocation", library="lib/LLVM.dll")]
def LLVMGetCurrentDebugLocation(Builder : LLVMOpaqueBuilder? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildRetVoid", library="lib/LLVM.dll")]
def LLVMBuildRetVoid(_argument_0x0 : LLVMOpaqueBuilder? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildRet", library="lib/LLVM.dll")]
def LLVMBuildRet(_argument_0x0 : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAggregateRet", library="lib/LLVM.dll")]
def LLVMBuildAggregateRet(_argument_0x0 : LLVMOpaqueBuilder? implicit; RetVals : LLVMOpaqueValue ?? implicit; N : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildBr", library="lib/LLVM.dll")]
def LLVMBuildBr(_argument_0x0 : LLVMOpaqueBuilder? implicit; Dest : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCondBr", library="lib/LLVM.dll")]
def LLVMBuildCondBr(_argument_0x0 : LLVMOpaqueBuilder? implicit; If : LLVMOpaqueValue? implicit; Then : LLVMOpaqueBasicBlock? implicit; Else : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSwitch", library="lib/LLVM.dll")]
def LLVMBuildSwitch(_argument_0x0 : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit; Else : LLVMOpaqueBasicBlock? implicit; NumCases : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildIndirectBr", library="lib/LLVM.dll")]
def LLVMBuildIndirectBr(B : LLVMOpaqueBuilder? implicit; Addr : LLVMOpaqueValue? implicit; NumDests : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildInvoke2", library="lib/LLVM.dll")]
def LLVMBuildInvoke2(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Fn : LLVMOpaqueValue? implicit; Args : LLVMOpaqueValue ?? implicit; NumArgs : uint; Then : LLVMOpaqueBasicBlock? implicit; Catch : LLVMOpaqueBasicBlock? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildUnreachable", library="lib/LLVM.dll")]
def LLVMBuildUnreachable(_argument_0x0 : LLVMOpaqueBuilder? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildResume", library="lib/LLVM.dll")]
def LLVMBuildResume(B : LLVMOpaqueBuilder? implicit; Exn : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildLandingPad", library="lib/LLVM.dll")]
def LLVMBuildLandingPad(B : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; PersFn : LLVMOpaqueValue? implicit; NumClauses : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCleanupRet", library="lib/LLVM.dll")]
def LLVMBuildCleanupRet(B : LLVMOpaqueBuilder? implicit; CatchPad : LLVMOpaqueValue? implicit; BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCatchRet", library="lib/LLVM.dll")]
def LLVMBuildCatchRet(B : LLVMOpaqueBuilder? implicit; CatchPad : LLVMOpaqueValue? implicit; BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCatchPad", library="lib/LLVM.dll")]
def LLVMBuildCatchPad(B : LLVMOpaqueBuilder? implicit; ParentPad : LLVMOpaqueValue? implicit; Args : LLVMOpaqueValue ?? implicit; NumArgs : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCleanupPad", library="lib/LLVM.dll")]
def LLVMBuildCleanupPad(B : LLVMOpaqueBuilder? implicit; ParentPad : LLVMOpaqueValue? implicit; Args : LLVMOpaqueValue ?? implicit; NumArgs : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCatchSwitch", library="lib/LLVM.dll")]
def LLVMBuildCatchSwitch(B : LLVMOpaqueBuilder? implicit; ParentPad : LLVMOpaqueValue? implicit; UnwindBB : LLVMOpaqueBasicBlock? implicit; NumHandlers : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMAddCase", library="lib/LLVM.dll")]
def LLVMAddCase(Switch : LLVMOpaqueValue? implicit; OnVal : LLVMOpaqueValue? implicit; Dest : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMAddDestination", library="lib/LLVM.dll")]
def LLVMAddDestination(IndirectBr : LLVMOpaqueValue? implicit; Dest : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMGetNumClauses", library="lib/LLVM.dll")]
def LLVMGetNumClauses(LandingPad : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetClause", library="lib/LLVM.dll")]
def LLVMGetClause(LandingPad : LLVMOpaqueValue? implicit; Idx : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMAddClause", library="lib/LLVM.dll")]
def LLVMAddClause(LandingPad : LLVMOpaqueValue? implicit; ClauseVal : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMIsCleanup", library="lib/LLVM.dll")]
def LLVMIsCleanup(LandingPad : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetCleanup", library="lib/LLVM.dll")]
def LLVMSetCleanup(LandingPad : LLVMOpaqueValue? implicit; Val : int) : void {}
[extern(cdecl, name="LLVMAddHandler", library="lib/LLVM.dll")]
def LLVMAddHandler(CatchSwitch : LLVMOpaqueValue? implicit; Dest : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMGetNumHandlers", library="lib/LLVM.dll")]
def LLVMGetNumHandlers(CatchSwitch : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetHandlers", library="lib/LLVM.dll")]
def LLVMGetHandlers(CatchSwitch : LLVMOpaqueValue? implicit; Handlers : LLVMOpaqueBasicBlock ?? implicit) : void {}
[extern(cdecl, name="LLVMGetArgOperand", library="lib/LLVM.dll")]
def LLVMGetArgOperand(Funclet : LLVMOpaqueValue? implicit; i : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetArgOperand", library="lib/LLVM.dll")]
def LLVMSetArgOperand(Funclet : LLVMOpaqueValue? implicit; i : uint; value : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetParentCatchSwitch", library="lib/LLVM.dll")]
def LLVMGetParentCatchSwitch(CatchPad : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetParentCatchSwitch", library="lib/LLVM.dll")]
def LLVMSetParentCatchSwitch(CatchPad : LLVMOpaqueValue? implicit; CatchSwitch : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMBuildAdd", library="lib/LLVM.dll")]
def LLVMBuildAdd(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNSWAdd", library="lib/LLVM.dll")]
def LLVMBuildNSWAdd(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNUWAdd", library="lib/LLVM.dll")]
def LLVMBuildNUWAdd(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFAdd", library="lib/LLVM.dll")]
def LLVMBuildFAdd(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSub", library="lib/LLVM.dll")]
def LLVMBuildSub(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNSWSub", library="lib/LLVM.dll")]
def LLVMBuildNSWSub(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNUWSub", library="lib/LLVM.dll")]
def LLVMBuildNUWSub(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFSub", library="lib/LLVM.dll")]
def LLVMBuildFSub(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildMul", library="lib/LLVM.dll")]
def LLVMBuildMul(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNSWMul", library="lib/LLVM.dll")]
def LLVMBuildNSWMul(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNUWMul", library="lib/LLVM.dll")]
def LLVMBuildNUWMul(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFMul", library="lib/LLVM.dll")]
def LLVMBuildFMul(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildUDiv", library="lib/LLVM.dll")]
def LLVMBuildUDiv(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildExactUDiv", library="lib/LLVM.dll")]
def LLVMBuildExactUDiv(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSDiv", library="lib/LLVM.dll")]
def LLVMBuildSDiv(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildExactSDiv", library="lib/LLVM.dll")]
def LLVMBuildExactSDiv(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFDiv", library="lib/LLVM.dll")]
def LLVMBuildFDiv(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildURem", library="lib/LLVM.dll")]
def LLVMBuildURem(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSRem", library="lib/LLVM.dll")]
def LLVMBuildSRem(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFRem", library="lib/LLVM.dll")]
def LLVMBuildFRem(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildShl", library="lib/LLVM.dll")]
def LLVMBuildShl(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildLShr", library="lib/LLVM.dll")]
def LLVMBuildLShr(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAShr", library="lib/LLVM.dll")]
def LLVMBuildAShr(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAnd", library="lib/LLVM.dll")]
def LLVMBuildAnd(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildOr", library="lib/LLVM.dll")]
def LLVMBuildOr(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildXor", library="lib/LLVM.dll")]
def LLVMBuildXor(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildBinOp", library="lib/LLVM.dll")]
def LLVMBuildBinOp(B : LLVMOpaqueBuilder? implicit; Op : LLVMOpcode; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNeg", library="lib/LLVM.dll")]
def LLVMBuildNeg(_argument_0x0 : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNSWNeg", library="lib/LLVM.dll")]
def LLVMBuildNSWNeg(B : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNUWNeg", library="lib/LLVM.dll")]
def LLVMBuildNUWNeg(B : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFNeg", library="lib/LLVM.dll")]
def LLVMBuildFNeg(_argument_0x0 : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNot", library="lib/LLVM.dll")]
def LLVMBuildNot(_argument_0x0 : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildMalloc", library="lib/LLVM.dll")]
def LLVMBuildMalloc(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildArrayMalloc", library="lib/LLVM.dll")]
def LLVMBuildArrayMalloc(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Val : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildMemSet", library="lib/LLVM.dll")]
def LLVMBuildMemSet(B : LLVMOpaqueBuilder? implicit; Ptr : LLVMOpaqueValue? implicit; Val : LLVMOpaqueValue? implicit; Len : LLVMOpaqueValue? implicit; Align : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildMemCpy", library="lib/LLVM.dll")]
def LLVMBuildMemCpy(B : LLVMOpaqueBuilder? implicit; Dst : LLVMOpaqueValue? implicit; DstAlign : uint; Src : LLVMOpaqueValue? implicit; SrcAlign : uint; Size : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildMemMove", library="lib/LLVM.dll")]
def LLVMBuildMemMove(B : LLVMOpaqueBuilder? implicit; Dst : LLVMOpaqueValue? implicit; DstAlign : uint; Src : LLVMOpaqueValue? implicit; SrcAlign : uint; Size : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAlloca", library="lib/LLVM.dll")]
def LLVMBuildAlloca(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildArrayAlloca", library="lib/LLVM.dll")]
def LLVMBuildArrayAlloca(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Val : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFree", library="lib/LLVM.dll")]
def LLVMBuildFree(_argument_0x0 : LLVMOpaqueBuilder? implicit; PointerVal : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildLoad2", library="lib/LLVM.dll")]
def LLVMBuildLoad2(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; PointerVal : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildStore", library="lib/LLVM.dll")]
def LLVMBuildStore(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; Ptr : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildGEP2", library="lib/LLVM.dll")]
def LLVMBuildGEP2(B : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Pointer : LLVMOpaqueValue? implicit; Indices : LLVMOpaqueValue ?? implicit; NumIndices : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildInBoundsGEP2", library="lib/LLVM.dll")]
def LLVMBuildInBoundsGEP2(B : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Pointer : LLVMOpaqueValue? implicit; Indices : LLVMOpaqueValue ?? implicit; NumIndices : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildStructGEP2", library="lib/LLVM.dll")]
def LLVMBuildStructGEP2(B : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Pointer : LLVMOpaqueValue? implicit; Idx : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildGlobalString", library="lib/LLVM.dll")]
def LLVMBuildGlobalString(B : LLVMOpaqueBuilder? implicit; Str : string implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildGlobalStringPtr", library="lib/LLVM.dll")]
def LLVMBuildGlobalStringPtr(B : LLVMOpaqueBuilder? implicit; Str : string implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetVolatile", library="lib/LLVM.dll")]
def LLVMGetVolatile(MemoryAccessInst : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetVolatile", library="lib/LLVM.dll")]
def LLVMSetVolatile(MemoryAccessInst : LLVMOpaqueValue? implicit; IsVolatile : int) : void {}
[extern(cdecl, name="LLVMGetWeak", library="lib/LLVM.dll")]
def LLVMGetWeak(CmpXchgInst : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetWeak", library="lib/LLVM.dll")]
def LLVMSetWeak(CmpXchgInst : LLVMOpaqueValue? implicit; IsWeak : int) : void {}
[extern(cdecl, name="LLVMGetOrdering", library="lib/LLVM.dll")]
def LLVMGetOrdering(MemoryAccessInst : LLVMOpaqueValue? implicit) : LLVMAtomicOrdering {}
[extern(cdecl, name="LLVMSetOrdering", library="lib/LLVM.dll")]
def LLVMSetOrdering(MemoryAccessInst : LLVMOpaqueValue? implicit; Ordering : LLVMAtomicOrdering) : void {}
[extern(cdecl, name="LLVMGetAtomicRMWBinOp", library="lib/LLVM.dll")]
def LLVMGetAtomicRMWBinOp(AtomicRMWInst : LLVMOpaqueValue? implicit) : LLVMAtomicRMWBinOp {}
[extern(cdecl, name="LLVMSetAtomicRMWBinOp", library="lib/LLVM.dll")]
def LLVMSetAtomicRMWBinOp(AtomicRMWInst : LLVMOpaqueValue? implicit; BinOp : LLVMAtomicRMWBinOp) : void {}
[extern(cdecl, name="LLVMBuildTrunc", library="lib/LLVM.dll")]
def LLVMBuildTrunc(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildZExt", library="lib/LLVM.dll")]
def LLVMBuildZExt(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSExt", library="lib/LLVM.dll")]
def LLVMBuildSExt(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFPToUI", library="lib/LLVM.dll")]
def LLVMBuildFPToUI(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFPToSI", library="lib/LLVM.dll")]
def LLVMBuildFPToSI(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildUIToFP", library="lib/LLVM.dll")]
def LLVMBuildUIToFP(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSIToFP", library="lib/LLVM.dll")]
def LLVMBuildSIToFP(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFPTrunc", library="lib/LLVM.dll")]
def LLVMBuildFPTrunc(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFPExt", library="lib/LLVM.dll")]
def LLVMBuildFPExt(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildPtrToInt", library="lib/LLVM.dll")]
def LLVMBuildPtrToInt(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildIntToPtr", library="lib/LLVM.dll")]
def LLVMBuildIntToPtr(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildBitCast", library="lib/LLVM.dll")]
def LLVMBuildBitCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAddrSpaceCast", library="lib/LLVM.dll")]
def LLVMBuildAddrSpaceCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildZExtOrBitCast", library="lib/LLVM.dll")]
def LLVMBuildZExtOrBitCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSExtOrBitCast", library="lib/LLVM.dll")]
def LLVMBuildSExtOrBitCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildTruncOrBitCast", library="lib/LLVM.dll")]
def LLVMBuildTruncOrBitCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCast", library="lib/LLVM.dll")]
def LLVMBuildCast(B : LLVMOpaqueBuilder? implicit; Op : LLVMOpcode; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildPointerCast", library="lib/LLVM.dll")]
def LLVMBuildPointerCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildIntCast2", library="lib/LLVM.dll")]
def LLVMBuildIntCast2(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; IsSigned : int; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFPCast", library="lib/LLVM.dll")]
def LLVMBuildFPCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildIntCast", library="lib/LLVM.dll")]
def LLVMBuildIntCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetCastOpcode", library="lib/LLVM.dll")]
def LLVMGetCastOpcode(Src : LLVMOpaqueValue? implicit; SrcIsSigned : int; DestTy : LLVMOpaqueType? implicit; DestIsSigned : int) : LLVMOpcode {}
[extern(cdecl, name="LLVMBuildICmp", library="lib/LLVM.dll")]
def LLVMBuildICmp(_argument_0x0 : LLVMOpaqueBuilder? implicit; Op : LLVMIntPredicate; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFCmp", library="lib/LLVM.dll")]
def LLVMBuildFCmp(_argument_0x0 : LLVMOpaqueBuilder? implicit; Op : LLVMRealPredicate; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildPhi", library="lib/LLVM.dll")]
def LLVMBuildPhi(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCall2", library="lib/LLVM.dll")]
def LLVMBuildCall2(_argument_0x0 : LLVMOpaqueBuilder? implicit; _argument_0x1 : LLVMOpaqueType? implicit; Fn : LLVMOpaqueValue? implicit; Args : LLVMOpaqueValue ?? implicit; NumArgs : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSelect", library="lib/LLVM.dll")]
def LLVMBuildSelect(_argument_0x0 : LLVMOpaqueBuilder? implicit; If : LLVMOpaqueValue? implicit; Then : LLVMOpaqueValue? implicit; Else : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildVAArg", library="lib/LLVM.dll")]
def LLVMBuildVAArg(_argument_0x0 : LLVMOpaqueBuilder? implicit; List : LLVMOpaqueValue? implicit; Ty : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildExtractElement", library="lib/LLVM.dll")]
def LLVMBuildExtractElement(_argument_0x0 : LLVMOpaqueBuilder? implicit; VecVal : LLVMOpaqueValue? implicit; Index : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildInsertElement", library="lib/LLVM.dll")]
def LLVMBuildInsertElement(_argument_0x0 : LLVMOpaqueBuilder? implicit; VecVal : LLVMOpaqueValue? implicit; EltVal : LLVMOpaqueValue? implicit; Index : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildShuffleVector", library="lib/LLVM.dll")]
def LLVMBuildShuffleVector(_argument_0x0 : LLVMOpaqueBuilder? implicit; V1 : LLVMOpaqueValue? implicit; V2 : LLVMOpaqueValue? implicit; Mask : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildExtractValue", library="lib/LLVM.dll")]
def LLVMBuildExtractValue(_argument_0x0 : LLVMOpaqueBuilder? implicit; AggVal : LLVMOpaqueValue? implicit; Index : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildInsertValue", library="lib/LLVM.dll")]
def LLVMBuildInsertValue(_argument_0x0 : LLVMOpaqueBuilder? implicit; AggVal : LLVMOpaqueValue? implicit; EltVal : LLVMOpaqueValue? implicit; Index : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFreeze", library="lib/LLVM.dll")]
def LLVMBuildFreeze(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildIsNull", library="lib/LLVM.dll")]
def LLVMBuildIsNull(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildIsNotNull", library="lib/LLVM.dll")]
def LLVMBuildIsNotNull(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildPtrDiff2", library="lib/LLVM.dll")]
def LLVMBuildPtrDiff2(_argument_0x0 : LLVMOpaqueBuilder? implicit; ElemTy : LLVMOpaqueType? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFence", library="lib/LLVM.dll")]
def LLVMBuildFence(B : LLVMOpaqueBuilder? implicit; ordering : LLVMAtomicOrdering; singleThread : int; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAtomicRMW", library="lib/LLVM.dll")]
def LLVMBuildAtomicRMW(B : LLVMOpaqueBuilder? implicit; op : LLVMAtomicRMWBinOp; PTR : LLVMOpaqueValue? implicit; Val : LLVMOpaqueValue? implicit; ordering : LLVMAtomicOrdering; singleThread : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAtomicCmpXchg", library="lib/LLVM.dll")]
def LLVMBuildAtomicCmpXchg(B : LLVMOpaqueBuilder? implicit; Ptr : LLVMOpaqueValue? implicit; Cmp : LLVMOpaqueValue? implicit; New : LLVMOpaqueValue? implicit; SuccessOrdering : LLVMAtomicOrdering; FailureOrdering : LLVMAtomicOrdering; SingleThread : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNumMaskElements", library="lib/LLVM.dll")]
def LLVMGetNumMaskElements(ShuffleVectorInst : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetUndefMaskElem", library="lib/LLVM.dll")]
def LLVMGetUndefMaskElem() : int {}
[extern(cdecl, name="LLVMGetMaskValue", library="lib/LLVM.dll")]
def LLVMGetMaskValue(ShuffleVectorInst : LLVMOpaqueValue? implicit; Elt : uint) : int {}
[extern(cdecl, name="LLVMIsAtomicSingleThread", library="lib/LLVM.dll")]
def LLVMIsAtomicSingleThread(AtomicInst : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetAtomicSingleThread", library="lib/LLVM.dll")]
def LLVMSetAtomicSingleThread(AtomicInst : LLVMOpaqueValue? implicit; SingleThread : int) : void {}
[extern(cdecl, name="LLVMGetCmpXchgSuccessOrdering", library="lib/LLVM.dll")]
def LLVMGetCmpXchgSuccessOrdering(CmpXchgInst : LLVMOpaqueValue? implicit) : LLVMAtomicOrdering {}
[extern(cdecl, name="LLVMSetCmpXchgSuccessOrdering", library="lib/LLVM.dll")]
def LLVMSetCmpXchgSuccessOrdering(CmpXchgInst : LLVMOpaqueValue? implicit; Ordering : LLVMAtomicOrdering) : void {}
[extern(cdecl, name="LLVMGetCmpXchgFailureOrdering", library="lib/LLVM.dll")]
def LLVMGetCmpXchgFailureOrdering(CmpXchgInst : LLVMOpaqueValue? implicit) : LLVMAtomicOrdering {}
[extern(cdecl, name="LLVMSetCmpXchgFailureOrdering", library="lib/LLVM.dll")]
def LLVMSetCmpXchgFailureOrdering(CmpXchgInst : LLVMOpaqueValue? implicit; Ordering : LLVMAtomicOrdering) : void {}
[extern(cdecl, name="LLVMCreateModuleProviderForExistingModule", library="lib/LLVM.dll")]
def LLVMCreateModuleProviderForExistingModule(M : LLVMOpaqueModule? implicit) : LLVMOpaqueModuleProvider? {}
[extern(cdecl, name="LLVMDisposeModuleProvider", library="lib/LLVM.dll")]
def LLVMDisposeModuleProvider(M : LLVMOpaqueModuleProvider? implicit) : void {}
[extern(cdecl, name="LLVMCreateMemoryBufferWithContentsOfFile", library="lib/LLVM.dll")]
def LLVMCreateMemoryBufferWithContentsOfFile(Path : string implicit; OutMemBuf : LLVMOpaqueMemoryBuffer ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMCreateMemoryBufferWithSTDIN", library="lib/LLVM.dll")]
def LLVMCreateMemoryBufferWithSTDIN(OutMemBuf : LLVMOpaqueMemoryBuffer ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMCreateMemoryBufferWithMemoryRange", library="lib/LLVM.dll")]
def LLVMCreateMemoryBufferWithMemoryRange(InputData : string implicit; InputDataLength : uint64; BufferName : string implicit; RequiresNullTerminator : int) : LLVMOpaqueMemoryBuffer? {}
[extern(cdecl, name="LLVMCreateMemoryBufferWithMemoryRangeCopy", library="lib/LLVM.dll")]
def LLVMCreateMemoryBufferWithMemoryRangeCopy(InputData : string implicit; InputDataLength : uint64; BufferName : string implicit) : LLVMOpaqueMemoryBuffer? {}
[extern(cdecl, name="LLVMGetBufferStart", library="lib/LLVM.dll")]
def LLVMGetBufferStart(MemBuf : LLVMOpaqueMemoryBuffer? implicit) : string {}
[extern(cdecl, name="LLVMGetBufferSize", library="lib/LLVM.dll")]
def LLVMGetBufferSize(MemBuf : LLVMOpaqueMemoryBuffer? implicit) : uint64 {}
[extern(cdecl, name="LLVMDisposeMemoryBuffer", library="lib/LLVM.dll")]
def LLVMDisposeMemoryBuffer(MemBuf : LLVMOpaqueMemoryBuffer? implicit) : void {}
[extern(cdecl, name="LLVMGetGlobalPassRegistry", library="lib/LLVM.dll")]
def LLVMGetGlobalPassRegistry() : LLVMOpaquePassRegistry? {}
[extern(cdecl, name="LLVMCreatePassManager", library="lib/LLVM.dll")]
def LLVMCreatePassManager() : LLVMOpaquePassManager? {}
[extern(cdecl, name="LLVMCreateFunctionPassManagerForModule", library="lib/LLVM.dll")]
def LLVMCreateFunctionPassManagerForModule(M : LLVMOpaqueModule? implicit) : LLVMOpaquePassManager? {}
[extern(cdecl, name="LLVMCreateFunctionPassManager", library="lib/LLVM.dll")]
def LLVMCreateFunctionPassManager(MP : LLVMOpaqueModuleProvider? implicit) : LLVMOpaquePassManager? {}
[extern(cdecl, name="LLVMRunPassManager", library="lib/LLVM.dll")]
def LLVMRunPassManager(PM : LLVMOpaquePassManager? implicit; M : LLVMOpaqueModule? implicit) : int {}
[extern(cdecl, name="LLVMInitializeFunctionPassManager", library="lib/LLVM.dll")]
def LLVMInitializeFunctionPassManager(FPM : LLVMOpaquePassManager? implicit) : int {}
[extern(cdecl, name="LLVMRunFunctionPassManager", library="lib/LLVM.dll")]
def LLVMRunFunctionPassManager(FPM : LLVMOpaquePassManager? implicit; F : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMFinalizeFunctionPassManager", library="lib/LLVM.dll")]
def LLVMFinalizeFunctionPassManager(FPM : LLVMOpaquePassManager? implicit) : int {}
[extern(cdecl, name="LLVMDisposePassManager", library="lib/LLVM.dll")]
def LLVMDisposePassManager(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMStartMultithreaded", library="lib/LLVM.dll")]
def LLVMStartMultithreaded() : int {}
[extern(cdecl, name="LLVMStopMultithreaded", library="lib/LLVM.dll")]
def LLVMStopMultithreaded() : void {}
[extern(cdecl, name="LLVMIsMultithreaded", library="lib/LLVM.dll")]
def LLVMIsMultithreaded() : int {}
[extern(cdecl, name="LLVMDebugMetadataVersion", library="lib/LLVM.dll")]
def LLVMDebugMetadataVersion() : uint {}
[extern(cdecl, name="LLVMGetModuleDebugMetadataVersion", library="lib/LLVM.dll")]
def LLVMGetModuleDebugMetadataVersion(Module : LLVMOpaqueModule? implicit) : uint {}
[extern(cdecl, name="LLVMStripModuleDebugInfo", library="lib/LLVM.dll")]
def LLVMStripModuleDebugInfo(Module : LLVMOpaqueModule? implicit) : int {}
[extern(cdecl, name="LLVMCreateDIBuilderDisallowUnresolved", library="lib/LLVM.dll")]
def LLVMCreateDIBuilderDisallowUnresolved(M : LLVMOpaqueModule? implicit) : LLVMOpaqueDIBuilder? {}
[extern(cdecl, name="LLVMCreateDIBuilder", library="lib/LLVM.dll")]
def LLVMCreateDIBuilder(M : LLVMOpaqueModule? implicit) : LLVMOpaqueDIBuilder? {}
[extern(cdecl, name="LLVMDisposeDIBuilder", library="lib/LLVM.dll")]
def LLVMDisposeDIBuilder(Builder : LLVMOpaqueDIBuilder? implicit) : void {}
[extern(cdecl, name="LLVMDIBuilderFinalize", library="lib/LLVM.dll")]
def LLVMDIBuilderFinalize(Builder : LLVMOpaqueDIBuilder? implicit) : void {}
[extern(cdecl, name="LLVMDIBuilderFinalizeSubprogram", library="lib/LLVM.dll")]
def LLVMDIBuilderFinalizeSubprogram(Builder : LLVMOpaqueDIBuilder? implicit; Subprogram : LLVMOpaqueMetadata? implicit) : void {}
// Function `LLVMDIBuilderCreateCompileUnit` has too many arguments. Ignore.

[extern(cdecl, name="LLVMDIBuilderCreateFile", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateFile(Builder : LLVMOpaqueDIBuilder? implicit; Filename : string implicit; FilenameLen : uint64; Directory : string implicit; DirectoryLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateModule", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateModule(Builder : LLVMOpaqueDIBuilder? implicit; ParentScope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; ConfigMacros : string implicit; ConfigMacrosLen : uint64; IncludePath : string implicit; IncludePathLen : uint64; APINotesFile : string implicit; APINotesFileLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateNameSpace", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateNameSpace(Builder : LLVMOpaqueDIBuilder? implicit; ParentScope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; ExportSymbols : int) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateFunction", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateFunction(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; LinkageName : string implicit; LinkageNameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; Ty : LLVMOpaqueMetadata? implicit; IsLocalToUnit : int; IsDefinition : int; ScopeLine : uint; Flags : LLVMDIFlags; IsOptimized : int) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateLexicalBlock", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateLexicalBlock(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint; Column : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateLexicalBlockFile", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateLexicalBlockFile(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Discriminator : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateImportedModuleFromNamespace", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateImportedModuleFromNamespace(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; NS : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateImportedModuleFromAlias", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateImportedModuleFromAlias(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; ImportedEntity : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint; Elements : LLVMOpaqueMetadata ?? implicit; NumElements : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateImportedModuleFromModule", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateImportedModuleFromModule(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; M : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint; Elements : LLVMOpaqueMetadata ?? implicit; NumElements : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateImportedDeclaration", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateImportedDeclaration(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Decl : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint; Name : string implicit; NameLen : uint64; Elements : LLVMOpaqueMetadata ?? implicit; NumElements : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateDebugLocation", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateDebugLocation(Ctx : LLVMOpaqueContext? implicit; Line : uint; Column : uint; Scope : LLVMOpaqueMetadata? implicit; InlinedAt : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDILocationGetLine", library="lib/LLVM.dll")]
def LLVMDILocationGetLine(Location : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMDILocationGetColumn", library="lib/LLVM.dll")]
def LLVMDILocationGetColumn(Location : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMDILocationGetScope", library="lib/LLVM.dll")]
def LLVMDILocationGetScope(Location : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDILocationGetInlinedAt", library="lib/LLVM.dll")]
def LLVMDILocationGetInlinedAt(Location : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIScopeGetFile", library="lib/LLVM.dll")]
def LLVMDIScopeGetFile(Scope : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIFileGetDirectory", library="lib/LLVM.dll")]
def LLVMDIFileGetDirectory(File : LLVMOpaqueMetadata? implicit; Len : uint? implicit) : string {}
[extern(cdecl, name="LLVMDIFileGetFilename", library="lib/LLVM.dll")]
def LLVMDIFileGetFilename(File : LLVMOpaqueMetadata? implicit; Len : uint? implicit) : string {}
[extern(cdecl, name="LLVMDIFileGetSource", library="lib/LLVM.dll")]
def LLVMDIFileGetSource(File : LLVMOpaqueMetadata? implicit; Len : uint? implicit) : string {}
[extern(cdecl, name="LLVMDIBuilderGetOrCreateTypeArray", library="lib/LLVM.dll")]
def LLVMDIBuilderGetOrCreateTypeArray(Builder : LLVMOpaqueDIBuilder? implicit; Data : LLVMOpaqueMetadata ?? implicit; NumElements : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateSubroutineType", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateSubroutineType(Builder : LLVMOpaqueDIBuilder? implicit; File : LLVMOpaqueMetadata? implicit; ParameterTypes : LLVMOpaqueMetadata ?? implicit; NumParameterTypes : uint; Flags : LLVMDIFlags) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateMacro", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateMacro(Builder : LLVMOpaqueDIBuilder? implicit; ParentMacroFile : LLVMOpaqueMetadata? implicit; Line : uint; RecordType : LLVMDWARFMacinfoRecordType; Name : string implicit; NameLen : uint64; Value : string implicit; ValueLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateTempMacroFile", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateTempMacroFile(Builder : LLVMOpaqueDIBuilder? implicit; ParentMacroFile : LLVMOpaqueMetadata? implicit; Line : uint; File : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateEnumerator", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateEnumerator(Builder : LLVMOpaqueDIBuilder? implicit; Name : string implicit; NameLen : uint64; Value : int64; IsUnsigned : int) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateEnumerationType", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateEnumerationType(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNumber : uint; SizeInBits : uint64; AlignInBits : uint; Elements : LLVMOpaqueMetadata ?? implicit; NumElements : uint; ClassTy : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateUnionType", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateUnionType(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNumber : uint; SizeInBits : uint64; AlignInBits : uint; Flags : LLVMDIFlags; Elements : LLVMOpaqueMetadata ?? implicit; NumElements : uint; RunTimeLang : uint; UniqueId : string implicit; UniqueIdLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateArrayType", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateArrayType(Builder : LLVMOpaqueDIBuilder? implicit; Size : uint64; AlignInBits : uint; Ty : LLVMOpaqueMetadata? implicit; Subscripts : LLVMOpaqueMetadata ?? implicit; NumSubscripts : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateVectorType", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateVectorType(Builder : LLVMOpaqueDIBuilder? implicit; Size : uint64; AlignInBits : uint; Ty : LLVMOpaqueMetadata? implicit; Subscripts : LLVMOpaqueMetadata ?? implicit; NumSubscripts : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateUnspecifiedType", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateUnspecifiedType(Builder : LLVMOpaqueDIBuilder? implicit; Name : string implicit; NameLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateBasicType", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateBasicType(Builder : LLVMOpaqueDIBuilder? implicit; Name : string implicit; NameLen : uint64; SizeInBits : uint64; Encoding : uint; Flags : LLVMDIFlags) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreatePointerType", library="lib/LLVM.dll")]
def LLVMDIBuilderCreatePointerType(Builder : LLVMOpaqueDIBuilder? implicit; PointeeTy : LLVMOpaqueMetadata? implicit; SizeInBits : uint64; AlignInBits : uint; AddressSpace : uint; Name : string implicit; NameLen : uint64) : LLVMOpaqueMetadata? {}
// Function `LLVMDIBuilderCreateStructType` has too many arguments. Ignore.

[extern(cdecl, name="LLVMDIBuilderCreateMemberType", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateMemberType(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; SizeInBits : uint64; AlignInBits : uint; OffsetInBits : uint64; Flags : LLVMDIFlags; Ty : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateStaticMemberType", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateStaticMemberType(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNumber : uint; Type : LLVMOpaqueMetadata? implicit; Flags : LLVMDIFlags; ConstantVal : LLVMOpaqueValue? implicit; AlignInBits : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateMemberPointerType", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateMemberPointerType(Builder : LLVMOpaqueDIBuilder? implicit; PointeeType : LLVMOpaqueMetadata? implicit; ClassType : LLVMOpaqueMetadata? implicit; SizeInBits : uint64; AlignInBits : uint; Flags : LLVMDIFlags) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateObjCIVar", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateObjCIVar(Builder : LLVMOpaqueDIBuilder? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; SizeInBits : uint64; AlignInBits : uint; OffsetInBits : uint64; Flags : LLVMDIFlags; Ty : LLVMOpaqueMetadata? implicit; PropertyNode : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateObjCProperty", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateObjCProperty(Builder : LLVMOpaqueDIBuilder? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; GetterName : string implicit; GetterNameLen : uint64; SetterName : string implicit; SetterNameLen : uint64; PropertyAttributes : uint; Ty : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateObjectPointerType", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateObjectPointerType(Builder : LLVMOpaqueDIBuilder? implicit; Type : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateQualifiedType", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateQualifiedType(Builder : LLVMOpaqueDIBuilder? implicit; Tag : uint; Type : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateReferenceType", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateReferenceType(Builder : LLVMOpaqueDIBuilder? implicit; Tag : uint; Type : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateNullPtrType", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateNullPtrType(Builder : LLVMOpaqueDIBuilder? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateTypedef", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateTypedef(Builder : LLVMOpaqueDIBuilder? implicit; Type : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; Scope : LLVMOpaqueMetadata? implicit; AlignInBits : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateInheritance", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateInheritance(Builder : LLVMOpaqueDIBuilder? implicit; Ty : LLVMOpaqueMetadata? implicit; BaseTy : LLVMOpaqueMetadata? implicit; BaseOffset : uint64; VBPtrOffset : uint; Flags : LLVMDIFlags) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateForwardDecl", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateForwardDecl(Builder : LLVMOpaqueDIBuilder? implicit; Tag : uint; Name : string implicit; NameLen : uint64; Scope : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint; RuntimeLang : uint; SizeInBits : uint64; AlignInBits : uint; UniqueIdentifier : string implicit; UniqueIdentifierLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateReplaceableCompositeType", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateReplaceableCompositeType(Builder : LLVMOpaqueDIBuilder? implicit; Tag : uint; Name : string implicit; NameLen : uint64; Scope : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint; RuntimeLang : uint; SizeInBits : uint64; AlignInBits : uint; Flags : LLVMDIFlags; UniqueIdentifier : string implicit; UniqueIdentifierLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateBitFieldMemberType", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateBitFieldMemberType(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNumber : uint; SizeInBits : uint64; OffsetInBits : uint64; StorageOffsetInBits : uint64; Flags : LLVMDIFlags; Type : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
// Function `LLVMDIBuilderCreateClassType` has too many arguments. Ignore.

[extern(cdecl, name="LLVMDIBuilderCreateArtificialType", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateArtificialType(Builder : LLVMOpaqueDIBuilder? implicit; Type : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDITypeGetName", library="lib/LLVM.dll")]
def LLVMDITypeGetName(DType : LLVMOpaqueMetadata? implicit; Length : uint64? implicit) : string {}
[extern(cdecl, name="LLVMDITypeGetSizeInBits", library="lib/LLVM.dll")]
def LLVMDITypeGetSizeInBits(DType : LLVMOpaqueMetadata? implicit) : uint64 {}
[extern(cdecl, name="LLVMDITypeGetOffsetInBits", library="lib/LLVM.dll")]
def LLVMDITypeGetOffsetInBits(DType : LLVMOpaqueMetadata? implicit) : uint64 {}
[extern(cdecl, name="LLVMDITypeGetAlignInBits", library="lib/LLVM.dll")]
def LLVMDITypeGetAlignInBits(DType : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMDITypeGetLine", library="lib/LLVM.dll")]
def LLVMDITypeGetLine(DType : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMDITypeGetFlags", library="lib/LLVM.dll")]
def LLVMDITypeGetFlags(DType : LLVMOpaqueMetadata? implicit) : LLVMDIFlags {}
[extern(cdecl, name="LLVMDIBuilderGetOrCreateSubrange", library="lib/LLVM.dll")]
def LLVMDIBuilderGetOrCreateSubrange(Builder : LLVMOpaqueDIBuilder? implicit; LowerBound : int64; Count : int64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderGetOrCreateArray", library="lib/LLVM.dll")]
def LLVMDIBuilderGetOrCreateArray(Builder : LLVMOpaqueDIBuilder? implicit; Data : LLVMOpaqueMetadata ?? implicit; NumElements : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateExpression", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateExpression(Builder : LLVMOpaqueDIBuilder? implicit; Addr : uint64? implicit; Length : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateConstantValueExpression", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateConstantValueExpression(Builder : LLVMOpaqueDIBuilder? implicit; Value : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateGlobalVariableExpression", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateGlobalVariableExpression(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; Linkage : string implicit; LinkLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; Ty : LLVMOpaqueMetadata? implicit; LocalToUnit : int; Expr : LLVMOpaqueMetadata? implicit; Decl : LLVMOpaqueMetadata? implicit; AlignInBits : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIGlobalVariableExpressionGetVariable", library="lib/LLVM.dll")]
def LLVMDIGlobalVariableExpressionGetVariable(GVE : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIGlobalVariableExpressionGetExpression", library="lib/LLVM.dll")]
def LLVMDIGlobalVariableExpressionGetExpression(GVE : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIVariableGetFile", library="lib/LLVM.dll")]
def LLVMDIVariableGetFile(Var : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIVariableGetScope", library="lib/LLVM.dll")]
def LLVMDIVariableGetScope(Var : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIVariableGetLine", library="lib/LLVM.dll")]
def LLVMDIVariableGetLine(Var : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMTemporaryMDNode", library="lib/LLVM.dll")]
def LLVMTemporaryMDNode(Ctx : LLVMOpaqueContext? implicit; Data : LLVMOpaqueMetadata ?? implicit; NumElements : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDisposeTemporaryMDNode", library="lib/LLVM.dll")]
def LLVMDisposeTemporaryMDNode(TempNode : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMMetadataReplaceAllUsesWith", library="lib/LLVM.dll")]
def LLVMMetadataReplaceAllUsesWith(TempTargetMetadata : LLVMOpaqueMetadata? implicit; Replacement : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMDIBuilderCreateTempGlobalVariableFwdDecl", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateTempGlobalVariableFwdDecl(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; Linkage : string implicit; LnkLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; Ty : LLVMOpaqueMetadata? implicit; LocalToUnit : int; Decl : LLVMOpaqueMetadata? implicit; AlignInBits : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderInsertDeclareBefore", library="lib/LLVM.dll")]
def LLVMDIBuilderInsertDeclareBefore(Builder : LLVMOpaqueDIBuilder? implicit; Storage : LLVMOpaqueValue? implicit; VarInfo : LLVMOpaqueMetadata? implicit; Expr : LLVMOpaqueMetadata? implicit; DebugLoc : LLVMOpaqueMetadata? implicit; Instr : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMDIBuilderInsertDeclareAtEnd", library="lib/LLVM.dll")]
def LLVMDIBuilderInsertDeclareAtEnd(Builder : LLVMOpaqueDIBuilder? implicit; Storage : LLVMOpaqueValue? implicit; VarInfo : LLVMOpaqueMetadata? implicit; Expr : LLVMOpaqueMetadata? implicit; DebugLoc : LLVMOpaqueMetadata? implicit; Block : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMDIBuilderInsertDbgValueBefore", library="lib/LLVM.dll")]
def LLVMDIBuilderInsertDbgValueBefore(Builder : LLVMOpaqueDIBuilder? implicit; Val : LLVMOpaqueValue? implicit; VarInfo : LLVMOpaqueMetadata? implicit; Expr : LLVMOpaqueMetadata? implicit; DebugLoc : LLVMOpaqueMetadata? implicit; Instr : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMDIBuilderInsertDbgValueAtEnd", library="lib/LLVM.dll")]
def LLVMDIBuilderInsertDbgValueAtEnd(Builder : LLVMOpaqueDIBuilder? implicit; Val : LLVMOpaqueValue? implicit; VarInfo : LLVMOpaqueMetadata? implicit; Expr : LLVMOpaqueMetadata? implicit; DebugLoc : LLVMOpaqueMetadata? implicit; Block : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMDIBuilderCreateAutoVariable", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateAutoVariable(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; Ty : LLVMOpaqueMetadata? implicit; AlwaysPreserve : int; Flags : LLVMDIFlags; AlignInBits : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateParameterVariable", library="lib/LLVM.dll")]
def LLVMDIBuilderCreateParameterVariable(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; ArgNo : uint; File : LLVMOpaqueMetadata? implicit; LineNo : uint; Ty : LLVMOpaqueMetadata? implicit; AlwaysPreserve : int; Flags : LLVMDIFlags) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMGetSubprogram", library="lib/LLVM.dll")]
def LLVMGetSubprogram(Func : LLVMOpaqueValue? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMSetSubprogram", library="lib/LLVM.dll")]
def LLVMSetSubprogram(Func : LLVMOpaqueValue? implicit; SP : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMDISubprogramGetLine", library="lib/LLVM.dll")]
def LLVMDISubprogramGetLine(Subprogram : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMInstructionGetDebugLoc", library="lib/LLVM.dll")]
def LLVMInstructionGetDebugLoc(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMInstructionSetDebugLoc", library="lib/LLVM.dll")]
def LLVMInstructionSetDebugLoc(Inst : LLVMOpaqueValue? implicit; Loc : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMGetMetadataKind", library="lib/LLVM.dll")]
def LLVMGetMetadataKind(Metadata : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMSetDisasmOptions", library="lib/LLVM.dll")]
def LLVMSetDisasmOptions(DC : void? implicit; Options : uint64) : int {}
[extern(cdecl, name="LLVMDisasmDispose", library="lib/LLVM.dll")]
def LLVMDisasmDispose(DC : void? implicit) : void {}
[extern(cdecl, name="LLVMDisasmInstruction", library="lib/LLVM.dll")]
def LLVMDisasmInstruction(DC : void? implicit; Bytes : uint8? implicit; BytesSize : uint64; PC : uint64; OutString : string implicit; OutStringSize : uint64) : uint64 {}
[extern(cdecl, name="LLVMGetErrorTypeId", library="lib/LLVM.dll")]
def LLVMGetErrorTypeId(Err : LLVMOpaqueError? implicit) : void const? {}
[extern(cdecl, name="LLVMConsumeError", library="lib/LLVM.dll")]
def LLVMConsumeError(Err : LLVMOpaqueError? implicit) : void {}
[extern(cdecl, name="LLVMGetErrorMessage", library="lib/LLVM.dll")]
def LLVMGetErrorMessage(Err : LLVMOpaqueError? implicit) : string {}
[extern(cdecl, name="LLVMDisposeErrorMessage", library="lib/LLVM.dll")]
def LLVMDisposeErrorMessage(ErrMsg : string implicit) : void {}
[extern(cdecl, name="LLVMGetStringErrorTypeId", library="lib/LLVM.dll")]
def LLVMGetStringErrorTypeId() : void const? {}
[extern(cdecl, name="LLVMCreateStringError", library="lib/LLVM.dll")]
def LLVMCreateStringError(ErrMsg : string implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMInitializeAArch64TargetInfo", library="lib/LLVM.dll")]
def LLVMInitializeAArch64TargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeAMDGPUTargetInfo", library="lib/LLVM.dll")]
def LLVMInitializeAMDGPUTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeARMTargetInfo", library="lib/LLVM.dll")]
def LLVMInitializeARMTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeAVRTargetInfo", library="lib/LLVM.dll")]
def LLVMInitializeAVRTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeBPFTargetInfo", library="lib/LLVM.dll")]
def LLVMInitializeBPFTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeHexagonTargetInfo", library="lib/LLVM.dll")]
def LLVMInitializeHexagonTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeLanaiTargetInfo", library="lib/LLVM.dll")]
def LLVMInitializeLanaiTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeLoongArchTargetInfo", library="lib/LLVM.dll")]
def LLVMInitializeLoongArchTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeMipsTargetInfo", library="lib/LLVM.dll")]
def LLVMInitializeMipsTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeMSP430TargetInfo", library="lib/LLVM.dll")]
def LLVMInitializeMSP430TargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeNVPTXTargetInfo", library="lib/LLVM.dll")]
def LLVMInitializeNVPTXTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializePowerPCTargetInfo", library="lib/LLVM.dll")]
def LLVMInitializePowerPCTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeRISCVTargetInfo", library="lib/LLVM.dll")]
def LLVMInitializeRISCVTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeSparcTargetInfo", library="lib/LLVM.dll")]
def LLVMInitializeSparcTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeSystemZTargetInfo", library="lib/LLVM.dll")]
def LLVMInitializeSystemZTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeVETargetInfo", library="lib/LLVM.dll")]
def LLVMInitializeVETargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeWebAssemblyTargetInfo", library="lib/LLVM.dll")]
def LLVMInitializeWebAssemblyTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeX86TargetInfo", library="lib/LLVM.dll")]
def LLVMInitializeX86TargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeXCoreTargetInfo", library="lib/LLVM.dll")]
def LLVMInitializeXCoreTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeAArch64Target", library="lib/LLVM.dll")]
def LLVMInitializeAArch64Target() : void {}
[extern(cdecl, name="LLVMInitializeAMDGPUTarget", library="lib/LLVM.dll")]
def LLVMInitializeAMDGPUTarget() : void {}
[extern(cdecl, name="LLVMInitializeARMTarget", library="lib/LLVM.dll")]
def LLVMInitializeARMTarget() : void {}
[extern(cdecl, name="LLVMInitializeAVRTarget", library="lib/LLVM.dll")]
def LLVMInitializeAVRTarget() : void {}
[extern(cdecl, name="LLVMInitializeBPFTarget", library="lib/LLVM.dll")]
def LLVMInitializeBPFTarget() : void {}
[extern(cdecl, name="LLVMInitializeHexagonTarget", library="lib/LLVM.dll")]
def LLVMInitializeHexagonTarget() : void {}
[extern(cdecl, name="LLVMInitializeLanaiTarget", library="lib/LLVM.dll")]
def LLVMInitializeLanaiTarget() : void {}
[extern(cdecl, name="LLVMInitializeLoongArchTarget", library="lib/LLVM.dll")]
def LLVMInitializeLoongArchTarget() : void {}
[extern(cdecl, name="LLVMInitializeMipsTarget", library="lib/LLVM.dll")]
def LLVMInitializeMipsTarget() : void {}
[extern(cdecl, name="LLVMInitializeMSP430Target", library="lib/LLVM.dll")]
def LLVMInitializeMSP430Target() : void {}
[extern(cdecl, name="LLVMInitializeNVPTXTarget", library="lib/LLVM.dll")]
def LLVMInitializeNVPTXTarget() : void {}
[extern(cdecl, name="LLVMInitializePowerPCTarget", library="lib/LLVM.dll")]
def LLVMInitializePowerPCTarget() : void {}
[extern(cdecl, name="LLVMInitializeRISCVTarget", library="lib/LLVM.dll")]
def LLVMInitializeRISCVTarget() : void {}
[extern(cdecl, name="LLVMInitializeSparcTarget", library="lib/LLVM.dll")]
def LLVMInitializeSparcTarget() : void {}
[extern(cdecl, name="LLVMInitializeSystemZTarget", library="lib/LLVM.dll")]
def LLVMInitializeSystemZTarget() : void {}
[extern(cdecl, name="LLVMInitializeVETarget", library="lib/LLVM.dll")]
def LLVMInitializeVETarget() : void {}
[extern(cdecl, name="LLVMInitializeWebAssemblyTarget", library="lib/LLVM.dll")]
def LLVMInitializeWebAssemblyTarget() : void {}
[extern(cdecl, name="LLVMInitializeX86Target", library="lib/LLVM.dll")]
def LLVMInitializeX86Target() : void {}
[extern(cdecl, name="LLVMInitializeXCoreTarget", library="lib/LLVM.dll")]
def LLVMInitializeXCoreTarget() : void {}
[extern(cdecl, name="LLVMInitializeAArch64TargetMC", library="lib/LLVM.dll")]
def LLVMInitializeAArch64TargetMC() : void {}
[extern(cdecl, name="LLVMInitializeAMDGPUTargetMC", library="lib/LLVM.dll")]
def LLVMInitializeAMDGPUTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeARMTargetMC", library="lib/LLVM.dll")]
def LLVMInitializeARMTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeAVRTargetMC", library="lib/LLVM.dll")]
def LLVMInitializeAVRTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeBPFTargetMC", library="lib/LLVM.dll")]
def LLVMInitializeBPFTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeHexagonTargetMC", library="lib/LLVM.dll")]
def LLVMInitializeHexagonTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeLanaiTargetMC", library="lib/LLVM.dll")]
def LLVMInitializeLanaiTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeLoongArchTargetMC", library="lib/LLVM.dll")]
def LLVMInitializeLoongArchTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeMipsTargetMC", library="lib/LLVM.dll")]
def LLVMInitializeMipsTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeMSP430TargetMC", library="lib/LLVM.dll")]
def LLVMInitializeMSP430TargetMC() : void {}
[extern(cdecl, name="LLVMInitializeNVPTXTargetMC", library="lib/LLVM.dll")]
def LLVMInitializeNVPTXTargetMC() : void {}
[extern(cdecl, name="LLVMInitializePowerPCTargetMC", library="lib/LLVM.dll")]
def LLVMInitializePowerPCTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeRISCVTargetMC", library="lib/LLVM.dll")]
def LLVMInitializeRISCVTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeSparcTargetMC", library="lib/LLVM.dll")]
def LLVMInitializeSparcTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeSystemZTargetMC", library="lib/LLVM.dll")]
def LLVMInitializeSystemZTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeVETargetMC", library="lib/LLVM.dll")]
def LLVMInitializeVETargetMC() : void {}
[extern(cdecl, name="LLVMInitializeWebAssemblyTargetMC", library="lib/LLVM.dll")]
def LLVMInitializeWebAssemblyTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeX86TargetMC", library="lib/LLVM.dll")]
def LLVMInitializeX86TargetMC() : void {}
[extern(cdecl, name="LLVMInitializeXCoreTargetMC", library="lib/LLVM.dll")]
def LLVMInitializeXCoreTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeAArch64AsmPrinter", library="lib/LLVM.dll")]
def LLVMInitializeAArch64AsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeAMDGPUAsmPrinter", library="lib/LLVM.dll")]
def LLVMInitializeAMDGPUAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeARMAsmPrinter", library="lib/LLVM.dll")]
def LLVMInitializeARMAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeAVRAsmPrinter", library="lib/LLVM.dll")]
def LLVMInitializeAVRAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeBPFAsmPrinter", library="lib/LLVM.dll")]
def LLVMInitializeBPFAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeHexagonAsmPrinter", library="lib/LLVM.dll")]
def LLVMInitializeHexagonAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeLanaiAsmPrinter", library="lib/LLVM.dll")]
def LLVMInitializeLanaiAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeLoongArchAsmPrinter", library="lib/LLVM.dll")]
def LLVMInitializeLoongArchAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeMipsAsmPrinter", library="lib/LLVM.dll")]
def LLVMInitializeMipsAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeMSP430AsmPrinter", library="lib/LLVM.dll")]
def LLVMInitializeMSP430AsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeNVPTXAsmPrinter", library="lib/LLVM.dll")]
def LLVMInitializeNVPTXAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializePowerPCAsmPrinter", library="lib/LLVM.dll")]
def LLVMInitializePowerPCAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeRISCVAsmPrinter", library="lib/LLVM.dll")]
def LLVMInitializeRISCVAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeSparcAsmPrinter", library="lib/LLVM.dll")]
def LLVMInitializeSparcAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeSystemZAsmPrinter", library="lib/LLVM.dll")]
def LLVMInitializeSystemZAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeVEAsmPrinter", library="lib/LLVM.dll")]
def LLVMInitializeVEAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeWebAssemblyAsmPrinter", library="lib/LLVM.dll")]
def LLVMInitializeWebAssemblyAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeX86AsmPrinter", library="lib/LLVM.dll")]
def LLVMInitializeX86AsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeXCoreAsmPrinter", library="lib/LLVM.dll")]
def LLVMInitializeXCoreAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeAArch64AsmParser", library="lib/LLVM.dll")]
def LLVMInitializeAArch64AsmParser() : void {}
[extern(cdecl, name="LLVMInitializeAMDGPUAsmParser", library="lib/LLVM.dll")]
def LLVMInitializeAMDGPUAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeARMAsmParser", library="lib/LLVM.dll")]
def LLVMInitializeARMAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeAVRAsmParser", library="lib/LLVM.dll")]
def LLVMInitializeAVRAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeBPFAsmParser", library="lib/LLVM.dll")]
def LLVMInitializeBPFAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeHexagonAsmParser", library="lib/LLVM.dll")]
def LLVMInitializeHexagonAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeLanaiAsmParser", library="lib/LLVM.dll")]
def LLVMInitializeLanaiAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeLoongArchAsmParser", library="lib/LLVM.dll")]
def LLVMInitializeLoongArchAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeMipsAsmParser", library="lib/LLVM.dll")]
def LLVMInitializeMipsAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeMSP430AsmParser", library="lib/LLVM.dll")]
def LLVMInitializeMSP430AsmParser() : void {}
[extern(cdecl, name="LLVMInitializePowerPCAsmParser", library="lib/LLVM.dll")]
def LLVMInitializePowerPCAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeRISCVAsmParser", library="lib/LLVM.dll")]
def LLVMInitializeRISCVAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeSparcAsmParser", library="lib/LLVM.dll")]
def LLVMInitializeSparcAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeSystemZAsmParser", library="lib/LLVM.dll")]
def LLVMInitializeSystemZAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeVEAsmParser", library="lib/LLVM.dll")]
def LLVMInitializeVEAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeWebAssemblyAsmParser", library="lib/LLVM.dll")]
def LLVMInitializeWebAssemblyAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeX86AsmParser", library="lib/LLVM.dll")]
def LLVMInitializeX86AsmParser() : void {}
[extern(cdecl, name="LLVMInitializeAArch64Disassembler", library="lib/LLVM.dll")]
def LLVMInitializeAArch64Disassembler() : void {}
[extern(cdecl, name="LLVMInitializeAMDGPUDisassembler", library="lib/LLVM.dll")]
def LLVMInitializeAMDGPUDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeARMDisassembler", library="lib/LLVM.dll")]
def LLVMInitializeARMDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeAVRDisassembler", library="lib/LLVM.dll")]
def LLVMInitializeAVRDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeBPFDisassembler", library="lib/LLVM.dll")]
def LLVMInitializeBPFDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeHexagonDisassembler", library="lib/LLVM.dll")]
def LLVMInitializeHexagonDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeLanaiDisassembler", library="lib/LLVM.dll")]
def LLVMInitializeLanaiDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeLoongArchDisassembler", library="lib/LLVM.dll")]
def LLVMInitializeLoongArchDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeMipsDisassembler", library="lib/LLVM.dll")]
def LLVMInitializeMipsDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeMSP430Disassembler", library="lib/LLVM.dll")]
def LLVMInitializeMSP430Disassembler() : void {}
[extern(cdecl, name="LLVMInitializePowerPCDisassembler", library="lib/LLVM.dll")]
def LLVMInitializePowerPCDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeRISCVDisassembler", library="lib/LLVM.dll")]
def LLVMInitializeRISCVDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeSparcDisassembler", library="lib/LLVM.dll")]
def LLVMInitializeSparcDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeSystemZDisassembler", library="lib/LLVM.dll")]
def LLVMInitializeSystemZDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeVEDisassembler", library="lib/LLVM.dll")]
def LLVMInitializeVEDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeWebAssemblyDisassembler", library="lib/LLVM.dll")]
def LLVMInitializeWebAssemblyDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeX86Disassembler", library="lib/LLVM.dll")]
def LLVMInitializeX86Disassembler() : void {}
[extern(cdecl, name="LLVMInitializeXCoreDisassembler", library="lib/LLVM.dll")]
def LLVMInitializeXCoreDisassembler() : void {}
[extern(cdecl, name="LLVMGetModuleDataLayout", library="lib/LLVM.dll")]
def LLVMGetModuleDataLayout(M : LLVMOpaqueModule? implicit) : LLVMOpaqueTargetData? {}
[extern(cdecl, name="LLVMSetModuleDataLayout", library="lib/LLVM.dll")]
def LLVMSetModuleDataLayout(M : LLVMOpaqueModule? implicit; DL : LLVMOpaqueTargetData? implicit) : void {}
[extern(cdecl, name="LLVMCreateTargetData", library="lib/LLVM.dll")]
def LLVMCreateTargetData(StringRep : string implicit) : LLVMOpaqueTargetData? {}
[extern(cdecl, name="LLVMDisposeTargetData", library="lib/LLVM.dll")]
def LLVMDisposeTargetData(TD : LLVMOpaqueTargetData? implicit) : void {}
[extern(cdecl, name="LLVMAddTargetLibraryInfo", library="lib/LLVM.dll")]
def LLVMAddTargetLibraryInfo(TLI : LLVMOpaqueTargetLibraryInfotData? implicit; PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMCopyStringRepOfTargetData", library="lib/LLVM.dll")]
def LLVMCopyStringRepOfTargetData(TD : LLVMOpaqueTargetData? implicit) : string {}
[extern(cdecl, name="LLVMByteOrder", library="lib/LLVM.dll")]
def LLVMByteOrder(TD : LLVMOpaqueTargetData? implicit) : LLVMByteOrdering {}
[extern(cdecl, name="LLVMPointerSize", library="lib/LLVM.dll")]
def LLVMPointerSize(TD : LLVMOpaqueTargetData? implicit) : uint {}
[extern(cdecl, name="LLVMPointerSizeForAS", library="lib/LLVM.dll")]
def LLVMPointerSizeForAS(TD : LLVMOpaqueTargetData? implicit; AS : uint) : uint {}
[extern(cdecl, name="LLVMIntPtrType", library="lib/LLVM.dll")]
def LLVMIntPtrType(TD : LLVMOpaqueTargetData? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIntPtrTypeForAS", library="lib/LLVM.dll")]
def LLVMIntPtrTypeForAS(TD : LLVMOpaqueTargetData? implicit; AS : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIntPtrTypeInContext", library="lib/LLVM.dll")]
def LLVMIntPtrTypeInContext(C : LLVMOpaqueContext? implicit; TD : LLVMOpaqueTargetData? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIntPtrTypeForASInContext", library="lib/LLVM.dll")]
def LLVMIntPtrTypeForASInContext(C : LLVMOpaqueContext? implicit; TD : LLVMOpaqueTargetData? implicit; AS : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMSizeOfTypeInBits", library="lib/LLVM.dll")]
def LLVMSizeOfTypeInBits(TD : LLVMOpaqueTargetData? implicit; Ty : LLVMOpaqueType? implicit) : uint64 {}
[extern(cdecl, name="LLVMStoreSizeOfType", library="lib/LLVM.dll")]
def LLVMStoreSizeOfType(TD : LLVMOpaqueTargetData? implicit; Ty : LLVMOpaqueType? implicit) : uint64 {}
[extern(cdecl, name="LLVMABISizeOfType", library="lib/LLVM.dll")]
def LLVMABISizeOfType(TD : LLVMOpaqueTargetData? implicit; Ty : LLVMOpaqueType? implicit) : uint64 {}
[extern(cdecl, name="LLVMABIAlignmentOfType", library="lib/LLVM.dll")]
def LLVMABIAlignmentOfType(TD : LLVMOpaqueTargetData? implicit; Ty : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMCallFrameAlignmentOfType", library="lib/LLVM.dll")]
def LLVMCallFrameAlignmentOfType(TD : LLVMOpaqueTargetData? implicit; Ty : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMPreferredAlignmentOfType", library="lib/LLVM.dll")]
def LLVMPreferredAlignmentOfType(TD : LLVMOpaqueTargetData? implicit; Ty : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMPreferredAlignmentOfGlobal", library="lib/LLVM.dll")]
def LLVMPreferredAlignmentOfGlobal(TD : LLVMOpaqueTargetData? implicit; GlobalVar : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMElementAtOffset", library="lib/LLVM.dll")]
def LLVMElementAtOffset(TD : LLVMOpaqueTargetData? implicit; StructTy : LLVMOpaqueType? implicit; Offset : uint64) : uint {}
[extern(cdecl, name="LLVMOffsetOfElement", library="lib/LLVM.dll")]
def LLVMOffsetOfElement(TD : LLVMOpaqueTargetData? implicit; StructTy : LLVMOpaqueType? implicit; Element : uint) : uint64 {}
[extern(cdecl, name="LLVMGetFirstTarget", library="lib/LLVM.dll")]
def LLVMGetFirstTarget() : LLVMTarget? {}
[extern(cdecl, name="LLVMGetNextTarget", library="lib/LLVM.dll")]
def LLVMGetNextTarget(T : LLVMTarget? implicit) : LLVMTarget? {}
[extern(cdecl, name="LLVMGetTargetFromName", library="lib/LLVM.dll")]
def LLVMGetTargetFromName(Name : string implicit) : LLVMTarget? {}
[extern(cdecl, name="LLVMGetTargetFromTriple", library="lib/LLVM.dll")]
def LLVMGetTargetFromTriple(Triple : string implicit; T : LLVMTarget ?? implicit; ErrorMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMGetTargetName", library="lib/LLVM.dll")]
def LLVMGetTargetName(T : LLVMTarget? implicit) : string {}
[extern(cdecl, name="LLVMGetTargetDescription", library="lib/LLVM.dll")]
def LLVMGetTargetDescription(T : LLVMTarget? implicit) : string {}
[extern(cdecl, name="LLVMTargetHasJIT", library="lib/LLVM.dll")]
def LLVMTargetHasJIT(T : LLVMTarget? implicit) : int {}
[extern(cdecl, name="LLVMTargetHasTargetMachine", library="lib/LLVM.dll")]
def LLVMTargetHasTargetMachine(T : LLVMTarget? implicit) : int {}
[extern(cdecl, name="LLVMTargetHasAsmBackend", library="lib/LLVM.dll")]
def LLVMTargetHasAsmBackend(T : LLVMTarget? implicit) : int {}
[extern(cdecl, name="LLVMCreateTargetMachine", library="lib/LLVM.dll")]
def LLVMCreateTargetMachine(T : LLVMTarget? implicit; Triple : string implicit; CPU : string implicit; Features : string implicit; Level : LLVMCodeGenOptLevel; Reloc : LLVMRelocMode; CodeModel : LLVMCodeModel) : LLVMOpaqueTargetMachine? {}
[extern(cdecl, name="LLVMDisposeTargetMachine", library="lib/LLVM.dll")]
def LLVMDisposeTargetMachine(T : LLVMOpaqueTargetMachine? implicit) : void {}
[extern(cdecl, name="LLVMGetTargetMachineTarget", library="lib/LLVM.dll")]
def LLVMGetTargetMachineTarget(T : LLVMOpaqueTargetMachine? implicit) : LLVMTarget? {}
[extern(cdecl, name="LLVMGetTargetMachineTriple", library="lib/LLVM.dll")]
def LLVMGetTargetMachineTriple(T : LLVMOpaqueTargetMachine? implicit) : string {}
[extern(cdecl, name="LLVMGetTargetMachineCPU", library="lib/LLVM.dll")]
def LLVMGetTargetMachineCPU(T : LLVMOpaqueTargetMachine? implicit) : string {}
[extern(cdecl, name="LLVMGetTargetMachineFeatureString", library="lib/LLVM.dll")]
def LLVMGetTargetMachineFeatureString(T : LLVMOpaqueTargetMachine? implicit) : string {}
[extern(cdecl, name="LLVMCreateTargetDataLayout", library="lib/LLVM.dll")]
def LLVMCreateTargetDataLayout(T : LLVMOpaqueTargetMachine? implicit) : LLVMOpaqueTargetData? {}
[extern(cdecl, name="LLVMSetTargetMachineAsmVerbosity", library="lib/LLVM.dll")]
def LLVMSetTargetMachineAsmVerbosity(T : LLVMOpaqueTargetMachine? implicit; VerboseAsm : int) : void {}
[extern(cdecl, name="LLVMTargetMachineEmitToFile", library="lib/LLVM.dll")]
def LLVMTargetMachineEmitToFile(T : LLVMOpaqueTargetMachine? implicit; M : LLVMOpaqueModule? implicit; Filename : string implicit; codegen : LLVMCodeGenFileType; ErrorMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMTargetMachineEmitToMemoryBuffer", library="lib/LLVM.dll")]
def LLVMTargetMachineEmitToMemoryBuffer(T : LLVMOpaqueTargetMachine? implicit; M : LLVMOpaqueModule? implicit; codegen : LLVMCodeGenFileType; ErrorMessage : string? implicit; OutMemBuf : LLVMOpaqueMemoryBuffer ?? implicit) : int {}
[extern(cdecl, name="LLVMGetDefaultTargetTriple", library="lib/LLVM.dll")]
def LLVMGetDefaultTargetTriple() : string {}
[extern(cdecl, name="LLVMNormalizeTargetTriple", library="lib/LLVM.dll")]
def LLVMNormalizeTargetTriple(triple : string implicit) : string {}
[extern(cdecl, name="LLVMGetHostCPUName", library="lib/LLVM.dll")]
def LLVMGetHostCPUName() : string {}
[extern(cdecl, name="LLVMGetHostCPUFeatures", library="lib/LLVM.dll")]
def LLVMGetHostCPUFeatures() : string {}
[extern(cdecl, name="LLVMAddAnalysisPasses", library="lib/LLVM.dll")]
def LLVMAddAnalysisPasses(T : LLVMOpaqueTargetMachine? implicit; PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMLinkInMCJIT", library="lib/LLVM.dll")]
def LLVMLinkInMCJIT() : void {}
[extern(cdecl, name="LLVMLinkInInterpreter", library="lib/LLVM.dll")]
def LLVMLinkInInterpreter() : void {}
[extern(cdecl, name="LLVMCreateGenericValueOfInt", library="lib/LLVM.dll")]
def LLVMCreateGenericValueOfInt(Ty : LLVMOpaqueType? implicit; N : uint64; IsSigned : int) : LLVMOpaqueGenericValue? {}
[extern(cdecl, name="LLVMCreateGenericValueOfPointer", library="lib/LLVM.dll")]
def LLVMCreateGenericValueOfPointer(P : void? implicit) : LLVMOpaqueGenericValue? {}
[extern(cdecl, name="LLVMCreateGenericValueOfFloat", library="lib/LLVM.dll")]
def LLVMCreateGenericValueOfFloat(Ty : LLVMOpaqueType? implicit; N : double) : LLVMOpaqueGenericValue? {}
[extern(cdecl, name="LLVMGenericValueIntWidth", library="lib/LLVM.dll")]
def LLVMGenericValueIntWidth(GenValRef : LLVMOpaqueGenericValue? implicit) : uint {}
[extern(cdecl, name="LLVMGenericValueToInt", library="lib/LLVM.dll")]
def LLVMGenericValueToInt(GenVal : LLVMOpaqueGenericValue? implicit; IsSigned : int) : uint64 {}
[extern(cdecl, name="LLVMGenericValueToPointer", library="lib/LLVM.dll")]
def LLVMGenericValueToPointer(GenVal : LLVMOpaqueGenericValue? implicit) : void? {}
[extern(cdecl, name="LLVMGenericValueToFloat", library="lib/LLVM.dll")]
def LLVMGenericValueToFloat(TyRef : LLVMOpaqueType? implicit; GenVal : LLVMOpaqueGenericValue? implicit) : double {}
[extern(cdecl, name="LLVMDisposeGenericValue", library="lib/LLVM.dll")]
def LLVMDisposeGenericValue(GenVal : LLVMOpaqueGenericValue? implicit) : void {}
[extern(cdecl, name="LLVMCreateExecutionEngineForModule", library="lib/LLVM.dll")]
def LLVMCreateExecutionEngineForModule(OutEE : LLVMOpaqueExecutionEngine ?? implicit; M : LLVMOpaqueModule? implicit; OutError : string? implicit) : int {}
[extern(cdecl, name="LLVMCreateInterpreterForModule", library="lib/LLVM.dll")]
def LLVMCreateInterpreterForModule(OutInterp : LLVMOpaqueExecutionEngine ?? implicit; M : LLVMOpaqueModule? implicit; OutError : string? implicit) : int {}
[extern(cdecl, name="LLVMCreateJITCompilerForModule", library="lib/LLVM.dll")]
def LLVMCreateJITCompilerForModule(OutJIT : LLVMOpaqueExecutionEngine ?? implicit; M : LLVMOpaqueModule? implicit; OptLevel : uint; OutError : string? implicit) : int {}
[extern(cdecl, name="LLVMInitializeMCJITCompilerOptions", library="lib/LLVM.dll")]
def LLVMInitializeMCJITCompilerOptions(Options : LLVMMCJITCompilerOptions? implicit; SizeOfOptions : uint64) : void {}
[extern(cdecl, name="LLVMCreateMCJITCompilerForModule", library="lib/LLVM.dll")]
def LLVMCreateMCJITCompilerForModule(OutJIT : LLVMOpaqueExecutionEngine ?? implicit; M : LLVMOpaqueModule? implicit; Options : LLVMMCJITCompilerOptions? implicit; SizeOfOptions : uint64; OutError : string? implicit) : int {}
[extern(cdecl, name="LLVMDisposeExecutionEngine", library="lib/LLVM.dll")]
def LLVMDisposeExecutionEngine(EE : LLVMOpaqueExecutionEngine? implicit) : void {}
[extern(cdecl, name="LLVMRunStaticConstructors", library="lib/LLVM.dll")]
def LLVMRunStaticConstructors(EE : LLVMOpaqueExecutionEngine? implicit) : void {}
[extern(cdecl, name="LLVMRunStaticDestructors", library="lib/LLVM.dll")]
def LLVMRunStaticDestructors(EE : LLVMOpaqueExecutionEngine? implicit) : void {}
[extern(cdecl, name="LLVMRunFunctionAsMain", library="lib/LLVM.dll")]
def LLVMRunFunctionAsMain(EE : LLVMOpaqueExecutionEngine? implicit; F : LLVMOpaqueValue? implicit; ArgC : uint; ArgV : string const? implicit; EnvP : string const? implicit) : int {}
[extern(cdecl, name="LLVMRunFunction", library="lib/LLVM.dll")]
def LLVMRunFunction(EE : LLVMOpaqueExecutionEngine? implicit; F : LLVMOpaqueValue? implicit; NumArgs : uint; Args : LLVMOpaqueGenericValue ?? implicit) : LLVMOpaqueGenericValue? {}
[extern(cdecl, name="LLVMFreeMachineCodeForFunction", library="lib/LLVM.dll")]
def LLVMFreeMachineCodeForFunction(EE : LLVMOpaqueExecutionEngine? implicit; F : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMAddModule", library="lib/LLVM.dll")]
def LLVMAddModule(EE : LLVMOpaqueExecutionEngine? implicit; M : LLVMOpaqueModule? implicit) : void {}
[extern(cdecl, name="LLVMRemoveModule", library="lib/LLVM.dll")]
def LLVMRemoveModule(EE : LLVMOpaqueExecutionEngine? implicit; M : LLVMOpaqueModule? implicit; OutMod : LLVMOpaqueModule ?? implicit; OutError : string? implicit) : int {}
[extern(cdecl, name="LLVMFindFunction", library="lib/LLVM.dll")]
def LLVMFindFunction(EE : LLVMOpaqueExecutionEngine? implicit; Name : string implicit; OutFn : LLVMOpaqueValue ?? implicit) : int {}
[extern(cdecl, name="LLVMRecompileAndRelinkFunction", library="lib/LLVM.dll")]
def LLVMRecompileAndRelinkFunction(EE : LLVMOpaqueExecutionEngine? implicit; Fn : LLVMOpaqueValue? implicit) : void? {}
[extern(cdecl, name="LLVMGetExecutionEngineTargetData", library="lib/LLVM.dll")]
def LLVMGetExecutionEngineTargetData(EE : LLVMOpaqueExecutionEngine? implicit) : LLVMOpaqueTargetData? {}
[extern(cdecl, name="LLVMGetExecutionEngineTargetMachine", library="lib/LLVM.dll")]
def LLVMGetExecutionEngineTargetMachine(EE : LLVMOpaqueExecutionEngine? implicit) : LLVMOpaqueTargetMachine? {}
[extern(cdecl, name="LLVMAddGlobalMapping", library="lib/LLVM.dll")]
def LLVMAddGlobalMapping(EE : LLVMOpaqueExecutionEngine? implicit; Global : LLVMOpaqueValue? implicit; Addr : void? implicit) : void {}
[extern(cdecl, name="LLVMGetPointerToGlobal", library="lib/LLVM.dll")]
def LLVMGetPointerToGlobal(EE : LLVMOpaqueExecutionEngine? implicit; Global : LLVMOpaqueValue? implicit) : void? {}
[extern(cdecl, name="LLVMGetGlobalValueAddress", library="lib/LLVM.dll")]
def LLVMGetGlobalValueAddress(EE : LLVMOpaqueExecutionEngine? implicit; Name : string implicit) : uint64 {}
[extern(cdecl, name="LLVMGetFunctionAddress", library="lib/LLVM.dll")]
def LLVMGetFunctionAddress(EE : LLVMOpaqueExecutionEngine? implicit; Name : string implicit) : uint64 {}
[extern(cdecl, name="LLVMExecutionEngineGetErrMsg", library="lib/LLVM.dll")]
def LLVMExecutionEngineGetErrMsg(EE : LLVMOpaqueExecutionEngine? implicit; OutError : string? implicit) : int {}
[extern(cdecl, name="LLVMDisposeMCJITMemoryManager", library="lib/LLVM.dll")]
def LLVMDisposeMCJITMemoryManager(MM : LLVMOpaqueMCJITMemoryManager? implicit) : void {}
[extern(cdecl, name="LLVMCreateGDBRegistrationListener", library="lib/LLVM.dll")]
def LLVMCreateGDBRegistrationListener() : LLVMOpaqueJITEventListener? {}
[extern(cdecl, name="LLVMCreateIntelJITEventListener", library="lib/LLVM.dll")]
def LLVMCreateIntelJITEventListener() : LLVMOpaqueJITEventListener? {}
[extern(cdecl, name="LLVMCreateOProfileJITEventListener", library="lib/LLVM.dll")]
def LLVMCreateOProfileJITEventListener() : LLVMOpaqueJITEventListener? {}
[extern(cdecl, name="LLVMCreatePerfJITEventListener", library="lib/LLVM.dll")]
def LLVMCreatePerfJITEventListener() : LLVMOpaqueJITEventListener? {}
[extern(cdecl, name="LLVMInitializeTransformUtils", library="lib/LLVM.dll")]
def LLVMInitializeTransformUtils(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeScalarOpts", library="lib/LLVM.dll")]
def LLVMInitializeScalarOpts(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeVectorization", library="lib/LLVM.dll")]
def LLVMInitializeVectorization(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeInstCombine", library="lib/LLVM.dll")]
def LLVMInitializeInstCombine(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeIPO", library="lib/LLVM.dll")]
def LLVMInitializeIPO(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeAnalysis", library="lib/LLVM.dll")]
def LLVMInitializeAnalysis(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeIPA", library="lib/LLVM.dll")]
def LLVMInitializeIPA(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeCodeGen", library="lib/LLVM.dll")]
def LLVMInitializeCodeGen(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeTarget", library="lib/LLVM.dll")]
def LLVMInitializeTarget(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMParseIRInContext", library="lib/LLVM.dll")]
def LLVMParseIRInContext(ContextRef : LLVMOpaqueContext? implicit; MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutM : LLVMOpaqueModule ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMLinkModules2", library="lib/LLVM.dll")]
def LLVMLinkModules2(Dest : LLVMOpaqueModule? implicit; Src : LLVMOpaqueModule? implicit) : int {}
[extern(cdecl, name="LLVMOrcExecutionSessionGetSymbolStringPool", library="lib/LLVM.dll")]
def LLVMOrcExecutionSessionGetSymbolStringPool(ES : LLVMOrcOpaqueExecutionSession? implicit) : LLVMOrcOpaqueSymbolStringPool? {}
[extern(cdecl, name="LLVMOrcSymbolStringPoolClearDeadEntries", library="lib/LLVM.dll")]
def LLVMOrcSymbolStringPoolClearDeadEntries(SSP : LLVMOrcOpaqueSymbolStringPool? implicit) : void {}
[extern(cdecl, name="LLVMOrcExecutionSessionIntern", library="lib/LLVM.dll")]
def LLVMOrcExecutionSessionIntern(ES : LLVMOrcOpaqueExecutionSession? implicit; Name : string implicit) : LLVMOrcOpaqueSymbolStringPoolEntry? {}
[extern(cdecl, name="LLVMOrcRetainSymbolStringPoolEntry", library="lib/LLVM.dll")]
def LLVMOrcRetainSymbolStringPoolEntry(S : LLVMOrcOpaqueSymbolStringPoolEntry? implicit) : void {}
[extern(cdecl, name="LLVMOrcReleaseSymbolStringPoolEntry", library="lib/LLVM.dll")]
def LLVMOrcReleaseSymbolStringPoolEntry(S : LLVMOrcOpaqueSymbolStringPoolEntry? implicit) : void {}
[extern(cdecl, name="LLVMOrcSymbolStringPoolEntryStr", library="lib/LLVM.dll")]
def LLVMOrcSymbolStringPoolEntryStr(S : LLVMOrcOpaqueSymbolStringPoolEntry? implicit) : string {}
[extern(cdecl, name="LLVMOrcReleaseResourceTracker", library="lib/LLVM.dll")]
def LLVMOrcReleaseResourceTracker(RT : LLVMOrcOpaqueResourceTracker? implicit) : void {}
[extern(cdecl, name="LLVMOrcResourceTrackerTransferTo", library="lib/LLVM.dll")]
def LLVMOrcResourceTrackerTransferTo(SrcRT : LLVMOrcOpaqueResourceTracker? implicit; DstRT : LLVMOrcOpaqueResourceTracker? implicit) : void {}
[extern(cdecl, name="LLVMOrcResourceTrackerRemove", library="lib/LLVM.dll")]
def LLVMOrcResourceTrackerRemove(RT : LLVMOrcOpaqueResourceTracker? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcDisposeDefinitionGenerator", library="lib/LLVM.dll")]
def LLVMOrcDisposeDefinitionGenerator(DG : LLVMOrcOpaqueDefinitionGenerator? implicit) : void {}
[extern(cdecl, name="LLVMOrcDisposeMaterializationUnit", library="lib/LLVM.dll")]
def LLVMOrcDisposeMaterializationUnit(MU : LLVMOrcOpaqueMaterializationUnit? implicit) : void {}
[extern(cdecl, name="LLVMOrcAbsoluteSymbols", library="lib/LLVM.dll")]
def LLVMOrcAbsoluteSymbols(Syms : LLVMOrcCSymbolMapPair? implicit; NumPairs : uint64) : LLVMOrcOpaqueMaterializationUnit? {}
[extern(cdecl, name="LLVMOrcLazyReexports", library="lib/LLVM.dll")]
def LLVMOrcLazyReexports(LCTM : LLVMOrcOpaqueLazyCallThroughManager? implicit; ISM : LLVMOrcOpaqueIndirectStubsManager? implicit; SourceRef : LLVMOrcOpaqueJITDylib? implicit; CallableAliases : LLVMOrcCSymbolAliasMapPair? implicit; NumPairs : uint64) : LLVMOrcOpaqueMaterializationUnit? {}
[extern(cdecl, name="LLVMOrcDisposeMaterializationResponsibility", library="lib/LLVM.dll")]
def LLVMOrcDisposeMaterializationResponsibility(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit) : void {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityGetTargetDylib", library="lib/LLVM.dll")]
def LLVMOrcMaterializationResponsibilityGetTargetDylib(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit) : LLVMOrcOpaqueJITDylib? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityGetExecutionSession", library="lib/LLVM.dll")]
def LLVMOrcMaterializationResponsibilityGetExecutionSession(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit) : LLVMOrcOpaqueExecutionSession? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityGetSymbols", library="lib/LLVM.dll")]
def LLVMOrcMaterializationResponsibilityGetSymbols(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; NumPairs : uint64? implicit) : LLVMOrcCSymbolFlagsMapPair? {}
[extern(cdecl, name="LLVMOrcDisposeCSymbolFlagsMap", library="lib/LLVM.dll")]
def LLVMOrcDisposeCSymbolFlagsMap(Pairs : LLVMOrcCSymbolFlagsMapPair? implicit) : void {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityGetInitializerSymbol", library="lib/LLVM.dll")]
def LLVMOrcMaterializationResponsibilityGetInitializerSymbol(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit) : LLVMOrcOpaqueSymbolStringPoolEntry? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityGetRequestedSymbols", library="lib/LLVM.dll")]
def LLVMOrcMaterializationResponsibilityGetRequestedSymbols(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; NumSymbols : uint64? implicit) : LLVMOrcOpaqueSymbolStringPoolEntry ?? {}
[extern(cdecl, name="LLVMOrcDisposeSymbols", library="lib/LLVM.dll")]
def LLVMOrcDisposeSymbols(Symbols : LLVMOrcOpaqueSymbolStringPoolEntry ?? implicit) : void {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityNotifyResolved", library="lib/LLVM.dll")]
def LLVMOrcMaterializationResponsibilityNotifyResolved(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; Symbols : LLVMOrcCSymbolMapPair? implicit; NumPairs : uint64) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityNotifyEmitted", library="lib/LLVM.dll")]
def LLVMOrcMaterializationResponsibilityNotifyEmitted(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityDefineMaterializing", library="lib/LLVM.dll")]
def LLVMOrcMaterializationResponsibilityDefineMaterializing(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; Pairs : LLVMOrcCSymbolFlagsMapPair? implicit; NumPairs : uint64) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityFailMaterialization", library="lib/LLVM.dll")]
def LLVMOrcMaterializationResponsibilityFailMaterialization(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit) : void {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityReplace", library="lib/LLVM.dll")]
def LLVMOrcMaterializationResponsibilityReplace(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; MU : LLVMOrcOpaqueMaterializationUnit? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityDelegate", library="lib/LLVM.dll")]
def LLVMOrcMaterializationResponsibilityDelegate(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; Symbols : LLVMOrcOpaqueSymbolStringPoolEntry ?? implicit; NumSymbols : uint64; Result : LLVMOrcOpaqueMaterializationResponsibility ?? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityAddDependencies", library="lib/LLVM.dll")]
def LLVMOrcMaterializationResponsibilityAddDependencies(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; Name : LLVMOrcOpaqueSymbolStringPoolEntry? implicit; Dependencies : LLVMOrcCDependenceMapPair? implicit; NumPairs : uint64) : void {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityAddDependenciesForAll", library="lib/LLVM.dll")]
def LLVMOrcMaterializationResponsibilityAddDependenciesForAll(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; Dependencies : LLVMOrcCDependenceMapPair? implicit; NumPairs : uint64) : void {}
[extern(cdecl, name="LLVMOrcExecutionSessionCreateBareJITDylib", library="lib/LLVM.dll")]
def LLVMOrcExecutionSessionCreateBareJITDylib(ES : LLVMOrcOpaqueExecutionSession? implicit; Name : string implicit) : LLVMOrcOpaqueJITDylib? {}
[extern(cdecl, name="LLVMOrcExecutionSessionCreateJITDylib", library="lib/LLVM.dll")]
def LLVMOrcExecutionSessionCreateJITDylib(ES : LLVMOrcOpaqueExecutionSession? implicit; Result : LLVMOrcOpaqueJITDylib ?? implicit; Name : string implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcExecutionSessionGetJITDylibByName", library="lib/LLVM.dll")]
def LLVMOrcExecutionSessionGetJITDylibByName(ES : LLVMOrcOpaqueExecutionSession? implicit; Name : string implicit) : LLVMOrcOpaqueJITDylib? {}
[extern(cdecl, name="LLVMOrcJITDylibCreateResourceTracker", library="lib/LLVM.dll")]
def LLVMOrcJITDylibCreateResourceTracker(JD : LLVMOrcOpaqueJITDylib? implicit) : LLVMOrcOpaqueResourceTracker? {}
[extern(cdecl, name="LLVMOrcJITDylibGetDefaultResourceTracker", library="lib/LLVM.dll")]
def LLVMOrcJITDylibGetDefaultResourceTracker(JD : LLVMOrcOpaqueJITDylib? implicit) : LLVMOrcOpaqueResourceTracker? {}
[extern(cdecl, name="LLVMOrcJITDylibDefine", library="lib/LLVM.dll")]
def LLVMOrcJITDylibDefine(JD : LLVMOrcOpaqueJITDylib? implicit; MU : LLVMOrcOpaqueMaterializationUnit? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcJITDylibClear", library="lib/LLVM.dll")]
def LLVMOrcJITDylibClear(JD : LLVMOrcOpaqueJITDylib? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcJITDylibAddGenerator", library="lib/LLVM.dll")]
def LLVMOrcJITDylibAddGenerator(JD : LLVMOrcOpaqueJITDylib? implicit; DG : LLVMOrcOpaqueDefinitionGenerator? implicit) : void {}
[extern(cdecl, name="LLVMOrcLookupStateContinueLookup", library="lib/LLVM.dll")]
def LLVMOrcLookupStateContinueLookup(S : LLVMOrcOpaqueLookupState? implicit; Err : LLVMOpaqueError? implicit) : void {}
[extern(cdecl, name="LLVMOrcCreateStaticLibrarySearchGeneratorForPath", library="lib/LLVM.dll")]
def LLVMOrcCreateStaticLibrarySearchGeneratorForPath(Result : LLVMOrcOpaqueDefinitionGenerator ?? implicit; ObjLayer : LLVMOrcOpaqueObjectLayer? implicit; FileName : string implicit; TargetTriple : string implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcCreateNewThreadSafeContext", library="lib/LLVM.dll")]
def LLVMOrcCreateNewThreadSafeContext() : LLVMOrcOpaqueThreadSafeContext? {}
[extern(cdecl, name="LLVMOrcThreadSafeContextGetContext", library="lib/LLVM.dll")]
def LLVMOrcThreadSafeContextGetContext(TSCtx : LLVMOrcOpaqueThreadSafeContext? implicit) : LLVMOpaqueContext? {}
[extern(cdecl, name="LLVMOrcDisposeThreadSafeContext", library="lib/LLVM.dll")]
def LLVMOrcDisposeThreadSafeContext(TSCtx : LLVMOrcOpaqueThreadSafeContext? implicit) : void {}
[extern(cdecl, name="LLVMOrcCreateNewThreadSafeModule", library="lib/LLVM.dll")]
def LLVMOrcCreateNewThreadSafeModule(M : LLVMOpaqueModule? implicit; TSCtx : LLVMOrcOpaqueThreadSafeContext? implicit) : LLVMOrcOpaqueThreadSafeModule? {}
[extern(cdecl, name="LLVMOrcDisposeThreadSafeModule", library="lib/LLVM.dll")]
def LLVMOrcDisposeThreadSafeModule(TSM : LLVMOrcOpaqueThreadSafeModule? implicit) : void {}
[extern(cdecl, name="LLVMOrcJITTargetMachineBuilderDetectHost", library="lib/LLVM.dll")]
def LLVMOrcJITTargetMachineBuilderDetectHost(Result : LLVMOrcOpaqueJITTargetMachineBuilder ?? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcJITTargetMachineBuilderCreateFromTargetMachine", library="lib/LLVM.dll")]
def LLVMOrcJITTargetMachineBuilderCreateFromTargetMachine(TM : LLVMOpaqueTargetMachine? implicit) : LLVMOrcOpaqueJITTargetMachineBuilder? {}
[extern(cdecl, name="LLVMOrcDisposeJITTargetMachineBuilder", library="lib/LLVM.dll")]
def LLVMOrcDisposeJITTargetMachineBuilder(JTMB : LLVMOrcOpaqueJITTargetMachineBuilder? implicit) : void {}
[extern(cdecl, name="LLVMOrcJITTargetMachineBuilderGetTargetTriple", library="lib/LLVM.dll")]
def LLVMOrcJITTargetMachineBuilderGetTargetTriple(JTMB : LLVMOrcOpaqueJITTargetMachineBuilder? implicit) : string {}
[extern(cdecl, name="LLVMOrcJITTargetMachineBuilderSetTargetTriple", library="lib/LLVM.dll")]
def LLVMOrcJITTargetMachineBuilderSetTargetTriple(JTMB : LLVMOrcOpaqueJITTargetMachineBuilder? implicit; TargetTriple : string implicit) : void {}
[extern(cdecl, name="LLVMOrcObjectLayerAddObjectFile", library="lib/LLVM.dll")]
def LLVMOrcObjectLayerAddObjectFile(ObjLayer : LLVMOrcOpaqueObjectLayer? implicit; JD : LLVMOrcOpaqueJITDylib? implicit; ObjBuffer : LLVMOpaqueMemoryBuffer? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcObjectLayerAddObjectFileWithRT", library="lib/LLVM.dll")]
def LLVMOrcObjectLayerAddObjectFileWithRT(ObjLayer : LLVMOrcOpaqueObjectLayer? implicit; RT : LLVMOrcOpaqueResourceTracker? implicit; ObjBuffer : LLVMOpaqueMemoryBuffer? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcObjectLayerEmit", library="lib/LLVM.dll")]
def LLVMOrcObjectLayerEmit(ObjLayer : LLVMOrcOpaqueObjectLayer? implicit; R : LLVMOrcOpaqueMaterializationResponsibility? implicit; ObjBuffer : LLVMOpaqueMemoryBuffer? implicit) : void {}
[extern(cdecl, name="LLVMOrcDisposeObjectLayer", library="lib/LLVM.dll")]
def LLVMOrcDisposeObjectLayer(ObjLayer : LLVMOrcOpaqueObjectLayer? implicit) : void {}
[extern(cdecl, name="LLVMOrcIRTransformLayerEmit", library="lib/LLVM.dll")]
def LLVMOrcIRTransformLayerEmit(IRTransformLayer : LLVMOrcOpaqueIRTransformLayer? implicit; MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; TSM : LLVMOrcOpaqueThreadSafeModule? implicit) : void {}
[extern(cdecl, name="LLVMOrcCreateLocalIndirectStubsManager", library="lib/LLVM.dll")]
def LLVMOrcCreateLocalIndirectStubsManager(TargetTriple : string implicit) : LLVMOrcOpaqueIndirectStubsManager? {}
[extern(cdecl, name="LLVMOrcDisposeIndirectStubsManager", library="lib/LLVM.dll")]
def LLVMOrcDisposeIndirectStubsManager(ISM : LLVMOrcOpaqueIndirectStubsManager? implicit) : void {}
[extern(cdecl, name="LLVMOrcCreateLocalLazyCallThroughManager", library="lib/LLVM.dll")]
def LLVMOrcCreateLocalLazyCallThroughManager(TargetTriple : string implicit; ES : LLVMOrcOpaqueExecutionSession? implicit; ErrorHandlerAddr : uint64; LCTM : LLVMOrcOpaqueLazyCallThroughManager ?? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcDisposeLazyCallThroughManager", library="lib/LLVM.dll")]
def LLVMOrcDisposeLazyCallThroughManager(LCTM : LLVMOrcOpaqueLazyCallThroughManager? implicit) : void {}
[extern(cdecl, name="LLVMOrcCreateDumpObjects", library="lib/LLVM.dll")]
def LLVMOrcCreateDumpObjects(DumpDir : string implicit; IdentifierOverride : string implicit) : LLVMOrcOpaqueDumpObjects? {}
[extern(cdecl, name="LLVMOrcDisposeDumpObjects", library="lib/LLVM.dll")]
def LLVMOrcDisposeDumpObjects(DumpObjects : LLVMOrcOpaqueDumpObjects? implicit) : void {}
[extern(cdecl, name="LLVMOrcDumpObjects_CallOperator", library="lib/LLVM.dll")]
def LLVMOrcDumpObjects_CallOperator(DumpObjects : LLVMOrcOpaqueDumpObjects? implicit; ObjBuffer : LLVMOpaqueMemoryBuffer ?? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcCreateLLJITBuilder", library="lib/LLVM.dll")]
def LLVMOrcCreateLLJITBuilder() : LLVMOrcOpaqueLLJITBuilder? {}
[extern(cdecl, name="LLVMOrcDisposeLLJITBuilder", library="lib/LLVM.dll")]
def LLVMOrcDisposeLLJITBuilder(Builder : LLVMOrcOpaqueLLJITBuilder? implicit) : void {}
[extern(cdecl, name="LLVMOrcLLJITBuilderSetJITTargetMachineBuilder", library="lib/LLVM.dll")]
def LLVMOrcLLJITBuilderSetJITTargetMachineBuilder(Builder : LLVMOrcOpaqueLLJITBuilder? implicit; JTMB : LLVMOrcOpaqueJITTargetMachineBuilder? implicit) : void {}
[extern(cdecl, name="LLVMOrcCreateLLJIT", library="lib/LLVM.dll")]
def LLVMOrcCreateLLJIT(Result : LLVMOrcOpaqueLLJIT ?? implicit; Builder : LLVMOrcOpaqueLLJITBuilder? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcDisposeLLJIT", library="lib/LLVM.dll")]
def LLVMOrcDisposeLLJIT(J : LLVMOrcOpaqueLLJIT? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcLLJITGetExecutionSession", library="lib/LLVM.dll")]
def LLVMOrcLLJITGetExecutionSession(J : LLVMOrcOpaqueLLJIT? implicit) : LLVMOrcOpaqueExecutionSession? {}
[extern(cdecl, name="LLVMOrcLLJITGetMainJITDylib", library="lib/LLVM.dll")]
def LLVMOrcLLJITGetMainJITDylib(J : LLVMOrcOpaqueLLJIT? implicit) : LLVMOrcOpaqueJITDylib? {}
[extern(cdecl, name="LLVMOrcLLJITGetTripleString", library="lib/LLVM.dll")]
def LLVMOrcLLJITGetTripleString(J : LLVMOrcOpaqueLLJIT? implicit) : string {}
[extern(cdecl, name="LLVMOrcLLJITGetGlobalPrefix", library="lib/LLVM.dll")]
def LLVMOrcLLJITGetGlobalPrefix(J : LLVMOrcOpaqueLLJIT? implicit) : int8 {}
[extern(cdecl, name="LLVMOrcLLJITMangleAndIntern", library="lib/LLVM.dll")]
def LLVMOrcLLJITMangleAndIntern(J : LLVMOrcOpaqueLLJIT? implicit; UnmangledName : string implicit) : LLVMOrcOpaqueSymbolStringPoolEntry? {}
[extern(cdecl, name="LLVMOrcLLJITAddObjectFile", library="lib/LLVM.dll")]
def LLVMOrcLLJITAddObjectFile(J : LLVMOrcOpaqueLLJIT? implicit; JD : LLVMOrcOpaqueJITDylib? implicit; ObjBuffer : LLVMOpaqueMemoryBuffer? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcLLJITAddObjectFileWithRT", library="lib/LLVM.dll")]
def LLVMOrcLLJITAddObjectFileWithRT(J : LLVMOrcOpaqueLLJIT? implicit; RT : LLVMOrcOpaqueResourceTracker? implicit; ObjBuffer : LLVMOpaqueMemoryBuffer? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcLLJITAddLLVMIRModule", library="lib/LLVM.dll")]
def LLVMOrcLLJITAddLLVMIRModule(J : LLVMOrcOpaqueLLJIT? implicit; JD : LLVMOrcOpaqueJITDylib? implicit; TSM : LLVMOrcOpaqueThreadSafeModule? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcLLJITAddLLVMIRModuleWithRT", library="lib/LLVM.dll")]
def LLVMOrcLLJITAddLLVMIRModuleWithRT(J : LLVMOrcOpaqueLLJIT? implicit; JD : LLVMOrcOpaqueResourceTracker? implicit; TSM : LLVMOrcOpaqueThreadSafeModule? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcLLJITLookup", library="lib/LLVM.dll")]
def LLVMOrcLLJITLookup(J : LLVMOrcOpaqueLLJIT? implicit; Result : uint64? implicit; Name : string implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcLLJITGetObjLinkingLayer", library="lib/LLVM.dll")]
def LLVMOrcLLJITGetObjLinkingLayer(J : LLVMOrcOpaqueLLJIT? implicit) : LLVMOrcOpaqueObjectLayer? {}
[extern(cdecl, name="LLVMOrcLLJITGetObjTransformLayer", library="lib/LLVM.dll")]
def LLVMOrcLLJITGetObjTransformLayer(J : LLVMOrcOpaqueLLJIT? implicit) : LLVMOrcOpaqueObjectTransformLayer? {}
[extern(cdecl, name="LLVMOrcLLJITGetIRTransformLayer", library="lib/LLVM.dll")]
def LLVMOrcLLJITGetIRTransformLayer(J : LLVMOrcOpaqueLLJIT? implicit) : LLVMOrcOpaqueIRTransformLayer? {}
[extern(cdecl, name="LLVMOrcLLJITGetDataLayoutStr", library="lib/LLVM.dll")]
def LLVMOrcLLJITGetDataLayoutStr(J : LLVMOrcOpaqueLLJIT? implicit) : string {}
[extern(cdecl, name="lto_get_version", library="lib/LLVM.dll")]
def lto_get_version() : string {}
[extern(cdecl, name="lto_get_error_message", library="lib/LLVM.dll")]
def lto_get_error_message() : string {}
[extern(cdecl, name="lto_module_is_object_file", library="lib/LLVM.dll")]
def lto_module_is_object_file(path : string implicit) : bool {}
[extern(cdecl, name="lto_module_is_object_file_for_target", library="lib/LLVM.dll")]
def lto_module_is_object_file_for_target(path : string implicit; target_triple_prefix : string implicit) : bool {}
[extern(cdecl, name="lto_module_has_objc_category", library="lib/LLVM.dll")]
def lto_module_has_objc_category(mem : void const? implicit; length : uint64) : bool {}
[extern(cdecl, name="lto_module_is_object_file_in_memory", library="lib/LLVM.dll")]
def lto_module_is_object_file_in_memory(mem : void const? implicit; length : uint64) : bool {}
[extern(cdecl, name="lto_module_is_object_file_in_memory_for_target", library="lib/LLVM.dll")]
def lto_module_is_object_file_in_memory_for_target(mem : void const? implicit; length : uint64; target_triple_prefix : string implicit) : bool {}
[extern(cdecl, name="lto_module_create", library="lib/LLVM.dll")]
def lto_module_create(path : string implicit) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_create_from_memory", library="lib/LLVM.dll")]
def lto_module_create_from_memory(mem : void const? implicit; length : uint64) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_create_from_memory_with_path", library="lib/LLVM.dll")]
def lto_module_create_from_memory_with_path(mem : void const? implicit; length : uint64; path : string implicit) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_create_in_local_context", library="lib/LLVM.dll")]
def lto_module_create_in_local_context(mem : void const? implicit; length : uint64; path : string implicit) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_create_in_codegen_context", library="lib/LLVM.dll")]
def lto_module_create_in_codegen_context(mem : void const? implicit; length : uint64; path : string implicit; cg : LLVMOpaqueLTOCodeGenerator? implicit) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_create_from_fd", library="lib/LLVM.dll")]
def lto_module_create_from_fd(fd : int; path : string implicit; file_size : uint64) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_create_from_fd_at_offset", library="lib/LLVM.dll")]
def lto_module_create_from_fd_at_offset(fd : int; path : string implicit; file_size : uint64; map_size : uint64; offset : int64) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_dispose", library="lib/LLVM.dll")]
def lto_module_dispose(mod : LLVMOpaqueLTOModule? implicit) : void {}
[extern(cdecl, name="lto_module_get_target_triple", library="lib/LLVM.dll")]
def lto_module_get_target_triple(mod : LLVMOpaqueLTOModule? implicit) : string {}
[extern(cdecl, name="lto_module_set_target_triple", library="lib/LLVM.dll")]
def lto_module_set_target_triple(mod : LLVMOpaqueLTOModule? implicit; triple : string implicit) : void {}
[extern(cdecl, name="lto_module_get_num_symbols", library="lib/LLVM.dll")]
def lto_module_get_num_symbols(mod : LLVMOpaqueLTOModule? implicit) : uint {}
[extern(cdecl, name="lto_module_get_symbol_name", library="lib/LLVM.dll")]
def lto_module_get_symbol_name(mod : LLVMOpaqueLTOModule? implicit; index : uint) : string {}
[extern(cdecl, name="lto_module_get_symbol_attribute", library="lib/LLVM.dll")]
def lto_module_get_symbol_attribute(mod : LLVMOpaqueLTOModule? implicit; index : uint) : lto_symbol_attributes {}
[extern(cdecl, name="lto_module_get_linkeropts", library="lib/LLVM.dll")]
def lto_module_get_linkeropts(mod : LLVMOpaqueLTOModule? implicit) : string {}
[extern(cdecl, name="lto_module_get_macho_cputype", library="lib/LLVM.dll")]
def lto_module_get_macho_cputype(mod : LLVMOpaqueLTOModule? implicit; out_cputype : uint? implicit; out_cpusubtype : uint? implicit) : bool {}
[extern(cdecl, name="lto_module_has_ctor_dtor", library="lib/LLVM.dll")]
def lto_module_has_ctor_dtor(mod : LLVMOpaqueLTOModule? implicit) : bool {}
[extern(cdecl, name="lto_codegen_create", library="lib/LLVM.dll")]
def lto_codegen_create() : LLVMOpaqueLTOCodeGenerator? {}
[extern(cdecl, name="lto_codegen_create_in_local_context", library="lib/LLVM.dll")]
def lto_codegen_create_in_local_context() : LLVMOpaqueLTOCodeGenerator? {}
[extern(cdecl, name="lto_codegen_dispose", library="lib/LLVM.dll")]
def lto_codegen_dispose(_argument_0x0 : LLVMOpaqueLTOCodeGenerator? implicit) : void {}
[extern(cdecl, name="lto_codegen_add_module", library="lib/LLVM.dll")]
def lto_codegen_add_module(cg : LLVMOpaqueLTOCodeGenerator? implicit; mod : LLVMOpaqueLTOModule? implicit) : bool {}
[extern(cdecl, name="lto_codegen_set_module", library="lib/LLVM.dll")]
def lto_codegen_set_module(cg : LLVMOpaqueLTOCodeGenerator? implicit; mod : LLVMOpaqueLTOModule? implicit) : void {}
[extern(cdecl, name="lto_codegen_set_debug_model", library="lib/LLVM.dll")]
def lto_codegen_set_debug_model(cg : LLVMOpaqueLTOCodeGenerator? implicit; _argument_0x1 : lto_debug_model) : bool {}
[extern(cdecl, name="lto_codegen_set_pic_model", library="lib/LLVM.dll")]
def lto_codegen_set_pic_model(cg : LLVMOpaqueLTOCodeGenerator? implicit; _argument_0x1 : lto_codegen_model) : bool {}
[extern(cdecl, name="lto_codegen_set_cpu", library="lib/LLVM.dll")]
def lto_codegen_set_cpu(cg : LLVMOpaqueLTOCodeGenerator? implicit; cpu : string implicit) : void {}
[extern(cdecl, name="lto_codegen_set_assembler_path", library="lib/LLVM.dll")]
def lto_codegen_set_assembler_path(cg : LLVMOpaqueLTOCodeGenerator? implicit; path : string implicit) : void {}
[extern(cdecl, name="lto_codegen_set_assembler_args", library="lib/LLVM.dll")]
def lto_codegen_set_assembler_args(cg : LLVMOpaqueLTOCodeGenerator? implicit; args : string? implicit; nargs : int) : void {}
[extern(cdecl, name="lto_codegen_add_must_preserve_symbol", library="lib/LLVM.dll")]
def lto_codegen_add_must_preserve_symbol(cg : LLVMOpaqueLTOCodeGenerator? implicit; symbol : string implicit) : void {}
[extern(cdecl, name="lto_codegen_write_merged_modules", library="lib/LLVM.dll")]
def lto_codegen_write_merged_modules(cg : LLVMOpaqueLTOCodeGenerator? implicit; path : string implicit) : bool {}
[extern(cdecl, name="lto_codegen_compile", library="lib/LLVM.dll")]
def lto_codegen_compile(cg : LLVMOpaqueLTOCodeGenerator? implicit; length : uint64? implicit) : void const? {}
[extern(cdecl, name="lto_codegen_compile_to_file", library="lib/LLVM.dll")]
def lto_codegen_compile_to_file(cg : LLVMOpaqueLTOCodeGenerator? implicit; name : string? implicit) : bool {}
[extern(cdecl, name="lto_codegen_optimize", library="lib/LLVM.dll")]
def lto_codegen_optimize(cg : LLVMOpaqueLTOCodeGenerator? implicit) : bool {}
[extern(cdecl, name="lto_codegen_compile_optimized", library="lib/LLVM.dll")]
def lto_codegen_compile_optimized(cg : LLVMOpaqueLTOCodeGenerator? implicit; length : uint64? implicit) : void const? {}
[extern(cdecl, name="lto_api_version", library="lib/LLVM.dll")]
def lto_api_version() : uint {}
[extern(cdecl, name="lto_set_debug_options", library="lib/LLVM.dll")]
def lto_set_debug_options(_options : string const? implicit; number : int) : void {}
[extern(cdecl, name="lto_codegen_debug_options", library="lib/LLVM.dll")]
def lto_codegen_debug_options(cg : LLVMOpaqueLTOCodeGenerator? implicit; _argument_0x1 : string implicit) : void {}
[extern(cdecl, name="lto_codegen_debug_options_array", library="lib/LLVM.dll")]
def lto_codegen_debug_options_array(cg : LLVMOpaqueLTOCodeGenerator? implicit; _argument_0x1 : string const? implicit; number : int) : void {}
[extern(cdecl, name="lto_initialize_disassembler", library="lib/LLVM.dll")]
def lto_initialize_disassembler() : void {}
[extern(cdecl, name="lto_codegen_set_should_internalize", library="lib/LLVM.dll")]
def lto_codegen_set_should_internalize(cg : LLVMOpaqueLTOCodeGenerator? implicit; ShouldInternalize : bool) : void {}
[extern(cdecl, name="lto_codegen_set_should_embed_uselists", library="lib/LLVM.dll")]
def lto_codegen_set_should_embed_uselists(cg : LLVMOpaqueLTOCodeGenerator? implicit; ShouldEmbedUselists : bool) : void {}
[extern(cdecl, name="lto_input_create", library="lib/LLVM.dll")]
def lto_input_create(buffer : void const? implicit; buffer_size : uint64; path : string implicit) : LLVMOpaqueLTOInput? {}
[extern(cdecl, name="lto_input_dispose", library="lib/LLVM.dll")]
def lto_input_dispose(input : LLVMOpaqueLTOInput? implicit) : void {}
[extern(cdecl, name="lto_input_get_num_dependent_libraries", library="lib/LLVM.dll")]
def lto_input_get_num_dependent_libraries(input : LLVMOpaqueLTOInput? implicit) : uint {}
[extern(cdecl, name="lto_input_get_dependent_library", library="lib/LLVM.dll")]
def lto_input_get_dependent_library(input : LLVMOpaqueLTOInput? implicit; index : uint64; size : uint64? implicit) : string {}
[extern(cdecl, name="lto_runtime_lib_symbols_list", library="lib/LLVM.dll")]
def lto_runtime_lib_symbols_list(size : uint64? implicit) : string const? {}
[extern(cdecl, name="thinlto_create_codegen", library="lib/LLVM.dll")]
def thinlto_create_codegen() : LLVMOpaqueThinLTOCodeGenerator? {}
[extern(cdecl, name="thinlto_codegen_dispose", library="lib/LLVM.dll")]
def thinlto_codegen_dispose(cg : LLVMOpaqueThinLTOCodeGenerator? implicit) : void {}
[extern(cdecl, name="thinlto_codegen_add_module", library="lib/LLVM.dll")]
def thinlto_codegen_add_module(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; identifier : string implicit; data : string implicit; length : int) : void {}
[extern(cdecl, name="thinlto_codegen_process", library="lib/LLVM.dll")]
def thinlto_codegen_process(cg : LLVMOpaqueThinLTOCodeGenerator? implicit) : void {}
[extern(cdecl, name="thinlto_module_get_num_objects", library="lib/LLVM.dll")]
def thinlto_module_get_num_objects(cg : LLVMOpaqueThinLTOCodeGenerator? implicit) : uint {}
[extern(cdecl, name="thinlto_module_get_object", library="lib/LLVM.dll")]
def thinlto_module_get_object(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; index : uint) : LTOObjectBuffer {}
[extern(cdecl, name="thinlto_module_get_num_object_files", library="lib/LLVM.dll")]
def thinlto_module_get_num_object_files(cg : LLVMOpaqueThinLTOCodeGenerator? implicit) : uint {}
[extern(cdecl, name="thinlto_module_get_object_file", library="lib/LLVM.dll")]
def thinlto_module_get_object_file(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; index : uint) : string {}
[extern(cdecl, name="thinlto_codegen_set_pic_model", library="lib/LLVM.dll")]
def thinlto_codegen_set_pic_model(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; _argument_0x1 : lto_codegen_model) : bool {}
[extern(cdecl, name="thinlto_codegen_set_savetemps_dir", library="lib/LLVM.dll")]
def thinlto_codegen_set_savetemps_dir(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; save_temps_dir : string implicit) : void {}
[extern(cdecl, name="thinlto_set_generated_objects_dir", library="lib/LLVM.dll")]
def thinlto_set_generated_objects_dir(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; save_temps_dir : string implicit) : void {}
[extern(cdecl, name="thinlto_codegen_set_cpu", library="lib/LLVM.dll")]
def thinlto_codegen_set_cpu(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; cpu : string implicit) : void {}
[extern(cdecl, name="thinlto_codegen_disable_codegen", library="lib/LLVM.dll")]
def thinlto_codegen_disable_codegen(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; disable : bool) : void {}
[extern(cdecl, name="thinlto_codegen_set_codegen_only", library="lib/LLVM.dll")]
def thinlto_codegen_set_codegen_only(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; codegen_only : bool) : void {}
[extern(cdecl, name="thinlto_debug_options", library="lib/LLVM.dll")]
def thinlto_debug_options(_options : string const? implicit; number : int) : void {}
[extern(cdecl, name="lto_module_is_thinlto", library="lib/LLVM.dll")]
def lto_module_is_thinlto(mod : LLVMOpaqueLTOModule? implicit) : bool {}
[extern(cdecl, name="thinlto_codegen_add_must_preserve_symbol", library="lib/LLVM.dll")]
def thinlto_codegen_add_must_preserve_symbol(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; name : string implicit; length : int) : void {}
[extern(cdecl, name="thinlto_codegen_add_cross_referenced_symbol", library="lib/LLVM.dll")]
def thinlto_codegen_add_cross_referenced_symbol(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; name : string implicit; length : int) : void {}
[extern(cdecl, name="thinlto_codegen_set_cache_dir", library="lib/LLVM.dll")]
def thinlto_codegen_set_cache_dir(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; cache_dir : string implicit) : void {}
[extern(cdecl, name="thinlto_codegen_set_cache_pruning_interval", library="lib/LLVM.dll")]
def thinlto_codegen_set_cache_pruning_interval(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; interval : int) : void {}
[extern(cdecl, name="thinlto_codegen_set_final_cache_size_relative_to_available_space", library="lib/LLVM.dll")]
def thinlto_codegen_set_final_cache_size_relative_to_available_space(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; percentage : uint) : void {}
[extern(cdecl, name="thinlto_codegen_set_cache_entry_expiration", library="lib/LLVM.dll")]
def thinlto_codegen_set_cache_entry_expiration(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; expiration : uint) : void {}
[extern(cdecl, name="thinlto_codegen_set_cache_size_bytes", library="lib/LLVM.dll")]
def thinlto_codegen_set_cache_size_bytes(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; max_size_bytes : uint) : void {}
[extern(cdecl, name="thinlto_codegen_set_cache_size_megabytes", library="lib/LLVM.dll")]
def thinlto_codegen_set_cache_size_megabytes(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; max_size_megabytes : uint) : void {}
[extern(cdecl, name="thinlto_codegen_set_cache_size_files", library="lib/LLVM.dll")]
def thinlto_codegen_set_cache_size_files(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; max_size_files : uint) : void {}
[extern(cdecl, name="LLVMCreateBinary", library="lib/LLVM.dll")]
def LLVMCreateBinary(MemBuf : LLVMOpaqueMemoryBuffer? implicit; Context : LLVMOpaqueContext? implicit; ErrorMessage : string? implicit) : LLVMOpaqueBinary? {}
[extern(cdecl, name="LLVMDisposeBinary", library="lib/LLVM.dll")]
def LLVMDisposeBinary(BR : LLVMOpaqueBinary? implicit) : void {}
[extern(cdecl, name="LLVMBinaryCopyMemoryBuffer", library="lib/LLVM.dll")]
def LLVMBinaryCopyMemoryBuffer(BR : LLVMOpaqueBinary? implicit) : LLVMOpaqueMemoryBuffer? {}
[extern(cdecl, name="LLVMBinaryGetType", library="lib/LLVM.dll")]
def LLVMBinaryGetType(BR : LLVMOpaqueBinary? implicit) : LLVMBinaryType {}
[extern(cdecl, name="LLVMMachOUniversalBinaryCopyObjectForArch", library="lib/LLVM.dll")]
def LLVMMachOUniversalBinaryCopyObjectForArch(BR : LLVMOpaqueBinary? implicit; Arch : string implicit; ArchLen : uint64; ErrorMessage : string? implicit) : LLVMOpaqueBinary? {}
[extern(cdecl, name="LLVMObjectFileCopySectionIterator", library="lib/LLVM.dll")]
def LLVMObjectFileCopySectionIterator(BR : LLVMOpaqueBinary? implicit) : LLVMOpaqueSectionIterator? {}
[extern(cdecl, name="LLVMObjectFileIsSectionIteratorAtEnd", library="lib/LLVM.dll")]
def LLVMObjectFileIsSectionIteratorAtEnd(BR : LLVMOpaqueBinary? implicit; SI : LLVMOpaqueSectionIterator? implicit) : int {}
[extern(cdecl, name="LLVMObjectFileCopySymbolIterator", library="lib/LLVM.dll")]
def LLVMObjectFileCopySymbolIterator(BR : LLVMOpaqueBinary? implicit) : LLVMOpaqueSymbolIterator? {}
[extern(cdecl, name="LLVMObjectFileIsSymbolIteratorAtEnd", library="lib/LLVM.dll")]
def LLVMObjectFileIsSymbolIteratorAtEnd(BR : LLVMOpaqueBinary? implicit; SI : LLVMOpaqueSymbolIterator? implicit) : int {}
[extern(cdecl, name="LLVMDisposeSectionIterator", library="lib/LLVM.dll")]
def LLVMDisposeSectionIterator(SI : LLVMOpaqueSectionIterator? implicit) : void {}
[extern(cdecl, name="LLVMMoveToNextSection", library="lib/LLVM.dll")]
def LLVMMoveToNextSection(SI : LLVMOpaqueSectionIterator? implicit) : void {}
[extern(cdecl, name="LLVMMoveToContainingSection", library="lib/LLVM.dll")]
def LLVMMoveToContainingSection(Sect : LLVMOpaqueSectionIterator? implicit; Sym : LLVMOpaqueSymbolIterator? implicit) : void {}
[extern(cdecl, name="LLVMDisposeSymbolIterator", library="lib/LLVM.dll")]
def LLVMDisposeSymbolIterator(SI : LLVMOpaqueSymbolIterator? implicit) : void {}
[extern(cdecl, name="LLVMMoveToNextSymbol", library="lib/LLVM.dll")]
def LLVMMoveToNextSymbol(SI : LLVMOpaqueSymbolIterator? implicit) : void {}
[extern(cdecl, name="LLVMGetSectionName", library="lib/LLVM.dll")]
def LLVMGetSectionName(SI : LLVMOpaqueSectionIterator? implicit) : string {}
[extern(cdecl, name="LLVMGetSectionSize", library="lib/LLVM.dll")]
def LLVMGetSectionSize(SI : LLVMOpaqueSectionIterator? implicit) : uint64 {}
[extern(cdecl, name="LLVMGetSectionContents", library="lib/LLVM.dll")]
def LLVMGetSectionContents(SI : LLVMOpaqueSectionIterator? implicit) : string {}
[extern(cdecl, name="LLVMGetSectionAddress", library="lib/LLVM.dll")]
def LLVMGetSectionAddress(SI : LLVMOpaqueSectionIterator? implicit) : uint64 {}
[extern(cdecl, name="LLVMGetSectionContainsSymbol", library="lib/LLVM.dll")]
def LLVMGetSectionContainsSymbol(SI : LLVMOpaqueSectionIterator? implicit; Sym : LLVMOpaqueSymbolIterator? implicit) : int {}
[extern(cdecl, name="LLVMGetRelocations", library="lib/LLVM.dll")]
def LLVMGetRelocations(Section : LLVMOpaqueSectionIterator? implicit) : LLVMOpaqueRelocationIterator? {}
[extern(cdecl, name="LLVMDisposeRelocationIterator", library="lib/LLVM.dll")]
def LLVMDisposeRelocationIterator(RI : LLVMOpaqueRelocationIterator? implicit) : void {}
[extern(cdecl, name="LLVMIsRelocationIteratorAtEnd", library="lib/LLVM.dll")]
def LLVMIsRelocationIteratorAtEnd(Section : LLVMOpaqueSectionIterator? implicit; RI : LLVMOpaqueRelocationIterator? implicit) : int {}
[extern(cdecl, name="LLVMMoveToNextRelocation", library="lib/LLVM.dll")]
def LLVMMoveToNextRelocation(RI : LLVMOpaqueRelocationIterator? implicit) : void {}
[extern(cdecl, name="LLVMGetSymbolName", library="lib/LLVM.dll")]
def LLVMGetSymbolName(SI : LLVMOpaqueSymbolIterator? implicit) : string {}
[extern(cdecl, name="LLVMGetSymbolAddress", library="lib/LLVM.dll")]
def LLVMGetSymbolAddress(SI : LLVMOpaqueSymbolIterator? implicit) : uint64 {}
[extern(cdecl, name="LLVMGetSymbolSize", library="lib/LLVM.dll")]
def LLVMGetSymbolSize(SI : LLVMOpaqueSymbolIterator? implicit) : uint64 {}
[extern(cdecl, name="LLVMGetRelocationOffset", library="lib/LLVM.dll")]
def LLVMGetRelocationOffset(RI : LLVMOpaqueRelocationIterator? implicit) : uint64 {}
[extern(cdecl, name="LLVMGetRelocationSymbol", library="lib/LLVM.dll")]
def LLVMGetRelocationSymbol(RI : LLVMOpaqueRelocationIterator? implicit) : LLVMOpaqueSymbolIterator? {}
[extern(cdecl, name="LLVMGetRelocationType", library="lib/LLVM.dll")]
def LLVMGetRelocationType(RI : LLVMOpaqueRelocationIterator? implicit) : uint64 {}
[extern(cdecl, name="LLVMGetRelocationTypeName", library="lib/LLVM.dll")]
def LLVMGetRelocationTypeName(RI : LLVMOpaqueRelocationIterator? implicit) : string {}
[extern(cdecl, name="LLVMGetRelocationValueString", library="lib/LLVM.dll")]
def LLVMGetRelocationValueString(RI : LLVMOpaqueRelocationIterator? implicit) : string {}
[extern(cdecl, name="LLVMCreateObjectFile", library="lib/LLVM.dll")]
def LLVMCreateObjectFile(MemBuf : LLVMOpaqueMemoryBuffer? implicit) : LLVMOpaqueObjectFile? {}
[extern(cdecl, name="LLVMDisposeObjectFile", library="lib/LLVM.dll")]
def LLVMDisposeObjectFile(ObjectFile : LLVMOpaqueObjectFile? implicit) : void {}
[extern(cdecl, name="LLVMGetSections", library="lib/LLVM.dll")]
def LLVMGetSections(ObjectFile : LLVMOpaqueObjectFile? implicit) : LLVMOpaqueSectionIterator? {}
[extern(cdecl, name="LLVMIsSectionIteratorAtEnd", library="lib/LLVM.dll")]
def LLVMIsSectionIteratorAtEnd(ObjectFile : LLVMOpaqueObjectFile? implicit; SI : LLVMOpaqueSectionIterator? implicit) : int {}
[extern(cdecl, name="LLVMGetSymbols", library="lib/LLVM.dll")]
def LLVMGetSymbols(ObjectFile : LLVMOpaqueObjectFile? implicit) : LLVMOpaqueSymbolIterator? {}
[extern(cdecl, name="LLVMIsSymbolIteratorAtEnd", library="lib/LLVM.dll")]
def LLVMIsSymbolIteratorAtEnd(ObjectFile : LLVMOpaqueObjectFile? implicit; SI : LLVMOpaqueSymbolIterator? implicit) : int {}
[extern(cdecl, name="LLVMOrcCreateRTDyldObjectLinkingLayerWithSectionMemoryManager", library="lib/LLVM.dll")]
def LLVMOrcCreateRTDyldObjectLinkingLayerWithSectionMemoryManager(ES : LLVMOrcOpaqueExecutionSession? implicit) : LLVMOrcOpaqueObjectLayer? {}
[extern(cdecl, name="LLVMOrcRTDyldObjectLinkingLayerRegisterJITEventListener", library="lib/LLVM.dll")]
def LLVMOrcRTDyldObjectLinkingLayerRegisterJITEventListener(RTDyldObjLinkingLayer : LLVMOrcOpaqueObjectLayer? implicit; Listener : LLVMOpaqueJITEventListener? implicit) : void {}
[extern(cdecl, name="LLVMRemarkStringGetData", library="lib/LLVM.dll")]
def LLVMRemarkStringGetData(String : LLVMRemarkOpaqueString? implicit) : string {}
[extern(cdecl, name="LLVMRemarkStringGetLen", library="lib/LLVM.dll")]
def LLVMRemarkStringGetLen(String : LLVMRemarkOpaqueString? implicit) : uint {}
[extern(cdecl, name="LLVMRemarkDebugLocGetSourceFilePath", library="lib/LLVM.dll")]
def LLVMRemarkDebugLocGetSourceFilePath(DL : LLVMRemarkOpaqueDebugLoc? implicit) : LLVMRemarkOpaqueString? {}
[extern(cdecl, name="LLVMRemarkDebugLocGetSourceLine", library="lib/LLVM.dll")]
def LLVMRemarkDebugLocGetSourceLine(DL : LLVMRemarkOpaqueDebugLoc? implicit) : uint {}
[extern(cdecl, name="LLVMRemarkDebugLocGetSourceColumn", library="lib/LLVM.dll")]
def LLVMRemarkDebugLocGetSourceColumn(DL : LLVMRemarkOpaqueDebugLoc? implicit) : uint {}
[extern(cdecl, name="LLVMRemarkArgGetKey", library="lib/LLVM.dll")]
def LLVMRemarkArgGetKey(Arg : LLVMRemarkOpaqueArg? implicit) : LLVMRemarkOpaqueString? {}
[extern(cdecl, name="LLVMRemarkArgGetValue", library="lib/LLVM.dll")]
def LLVMRemarkArgGetValue(Arg : LLVMRemarkOpaqueArg? implicit) : LLVMRemarkOpaqueString? {}
[extern(cdecl, name="LLVMRemarkArgGetDebugLoc", library="lib/LLVM.dll")]
def LLVMRemarkArgGetDebugLoc(Arg : LLVMRemarkOpaqueArg? implicit) : LLVMRemarkOpaqueDebugLoc? {}
[extern(cdecl, name="LLVMRemarkEntryDispose", library="lib/LLVM.dll")]
def LLVMRemarkEntryDispose(Remark : LLVMRemarkOpaqueEntry? implicit) : void {}
[extern(cdecl, name="LLVMRemarkEntryGetType", library="lib/LLVM.dll")]
def LLVMRemarkEntryGetType(Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkType {}
[extern(cdecl, name="LLVMRemarkEntryGetPassName", library="lib/LLVM.dll")]
def LLVMRemarkEntryGetPassName(Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkOpaqueString? {}
[extern(cdecl, name="LLVMRemarkEntryGetRemarkName", library="lib/LLVM.dll")]
def LLVMRemarkEntryGetRemarkName(Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkOpaqueString? {}
[extern(cdecl, name="LLVMRemarkEntryGetFunctionName", library="lib/LLVM.dll")]
def LLVMRemarkEntryGetFunctionName(Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkOpaqueString? {}
[extern(cdecl, name="LLVMRemarkEntryGetDebugLoc", library="lib/LLVM.dll")]
def LLVMRemarkEntryGetDebugLoc(Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkOpaqueDebugLoc? {}
[extern(cdecl, name="LLVMRemarkEntryGetHotness", library="lib/LLVM.dll")]
def LLVMRemarkEntryGetHotness(Remark : LLVMRemarkOpaqueEntry? implicit) : uint64 {}
[extern(cdecl, name="LLVMRemarkEntryGetNumArgs", library="lib/LLVM.dll")]
def LLVMRemarkEntryGetNumArgs(Remark : LLVMRemarkOpaqueEntry? implicit) : uint {}
[extern(cdecl, name="LLVMRemarkEntryGetFirstArg", library="lib/LLVM.dll")]
def LLVMRemarkEntryGetFirstArg(Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkOpaqueArg? {}
[extern(cdecl, name="LLVMRemarkEntryGetNextArg", library="lib/LLVM.dll")]
def LLVMRemarkEntryGetNextArg(It : LLVMRemarkOpaqueArg? implicit; Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkOpaqueArg? {}
[extern(cdecl, name="LLVMRemarkParserCreateYAML", library="lib/LLVM.dll")]
def LLVMRemarkParserCreateYAML(Buf : void const? implicit; Size : uint64) : LLVMRemarkOpaqueParser? {}
[extern(cdecl, name="LLVMRemarkParserCreateBitstream", library="lib/LLVM.dll")]
def LLVMRemarkParserCreateBitstream(Buf : void const? implicit; Size : uint64) : LLVMRemarkOpaqueParser? {}
[extern(cdecl, name="LLVMRemarkParserGetNext", library="lib/LLVM.dll")]
def LLVMRemarkParserGetNext(Parser : LLVMRemarkOpaqueParser? implicit) : LLVMRemarkOpaqueEntry? {}
[extern(cdecl, name="LLVMRemarkParserHasError", library="lib/LLVM.dll")]
def LLVMRemarkParserHasError(Parser : LLVMRemarkOpaqueParser? implicit) : int {}
[extern(cdecl, name="LLVMRemarkParserGetErrorMessage", library="lib/LLVM.dll")]
def LLVMRemarkParserGetErrorMessage(Parser : LLVMRemarkOpaqueParser? implicit) : string {}
[extern(cdecl, name="LLVMRemarkParserDispose", library="lib/LLVM.dll")]
def LLVMRemarkParserDispose(Parser : LLVMRemarkOpaqueParser? implicit) : void {}
[extern(cdecl, name="LLVMRemarkVersion", library="lib/LLVM.dll")]
def LLVMRemarkVersion() : uint {}
[extern(cdecl, name="LLVMLoadLibraryPermanently", library="lib/LLVM.dll")]
def LLVMLoadLibraryPermanently(Filename : string implicit) : int {}
[extern(cdecl, name="LLVMParseCommandLineOptions", library="lib/LLVM.dll")]
def LLVMParseCommandLineOptions(argc : int; argv : string const? implicit; Overview : string implicit) : void {}
[extern(cdecl, name="LLVMSearchForAddressOfSymbol", library="lib/LLVM.dll")]
def LLVMSearchForAddressOfSymbol(symbolName : string implicit) : void? {}
[extern(cdecl, name="LLVMAddSymbol", library="lib/LLVM.dll")]
def LLVMAddSymbol(symbolName : string implicit; symbolValue : void? implicit) : void {}
[extern(cdecl, name="LLVMAddConstantMergePass", library="lib/LLVM.dll")]
def LLVMAddConstantMergePass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddMergeFunctionsPass", library="lib/LLVM.dll")]
def LLVMAddMergeFunctionsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddCalledValuePropagationPass", library="lib/LLVM.dll")]
def LLVMAddCalledValuePropagationPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddDeadArgEliminationPass", library="lib/LLVM.dll")]
def LLVMAddDeadArgEliminationPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddFunctionAttrsPass", library="lib/LLVM.dll")]
def LLVMAddFunctionAttrsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddFunctionInliningPass", library="lib/LLVM.dll")]
def LLVMAddFunctionInliningPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddAlwaysInlinerPass", library="lib/LLVM.dll")]
def LLVMAddAlwaysInlinerPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddGlobalDCEPass", library="lib/LLVM.dll")]
def LLVMAddGlobalDCEPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddGlobalOptimizerPass", library="lib/LLVM.dll")]
def LLVMAddGlobalOptimizerPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddIPSCCPPass", library="lib/LLVM.dll")]
def LLVMAddIPSCCPPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddInternalizePass", library="lib/LLVM.dll")]
def LLVMAddInternalizePass(_argument_0x0 : LLVMOpaquePassManager? implicit; AllButMain : uint) : void {}
[extern(cdecl, name="LLVMAddStripDeadPrototypesPass", library="lib/LLVM.dll")]
def LLVMAddStripDeadPrototypesPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddStripSymbolsPass", library="lib/LLVM.dll")]
def LLVMAddStripSymbolsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMRunPasses", library="lib/LLVM.dll")]
def LLVMRunPasses(M : LLVMOpaqueModule? implicit; Passes : string implicit; TM : LLVMOpaqueTargetMachine? implicit; Options : LLVMOpaquePassBuilderOptions? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMCreatePassBuilderOptions", library="lib/LLVM.dll")]
def LLVMCreatePassBuilderOptions() : LLVMOpaquePassBuilderOptions? {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetVerifyEach", library="lib/LLVM.dll")]
def LLVMPassBuilderOptionsSetVerifyEach(Options : LLVMOpaquePassBuilderOptions? implicit; VerifyEach : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetDebugLogging", library="lib/LLVM.dll")]
def LLVMPassBuilderOptionsSetDebugLogging(Options : LLVMOpaquePassBuilderOptions? implicit; DebugLogging : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetLoopInterleaving", library="lib/LLVM.dll")]
def LLVMPassBuilderOptionsSetLoopInterleaving(Options : LLVMOpaquePassBuilderOptions? implicit; LoopInterleaving : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetLoopVectorization", library="lib/LLVM.dll")]
def LLVMPassBuilderOptionsSetLoopVectorization(Options : LLVMOpaquePassBuilderOptions? implicit; LoopVectorization : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetSLPVectorization", library="lib/LLVM.dll")]
def LLVMPassBuilderOptionsSetSLPVectorization(Options : LLVMOpaquePassBuilderOptions? implicit; SLPVectorization : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetLoopUnrolling", library="lib/LLVM.dll")]
def LLVMPassBuilderOptionsSetLoopUnrolling(Options : LLVMOpaquePassBuilderOptions? implicit; LoopUnrolling : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetForgetAllSCEVInLoopUnroll", library="lib/LLVM.dll")]
def LLVMPassBuilderOptionsSetForgetAllSCEVInLoopUnroll(Options : LLVMOpaquePassBuilderOptions? implicit; ForgetAllSCEVInLoopUnroll : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetLicmMssaOptCap", library="lib/LLVM.dll")]
def LLVMPassBuilderOptionsSetLicmMssaOptCap(Options : LLVMOpaquePassBuilderOptions? implicit; LicmMssaOptCap : uint) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetLicmMssaNoAccForPromotionCap", library="lib/LLVM.dll")]
def LLVMPassBuilderOptionsSetLicmMssaNoAccForPromotionCap(Options : LLVMOpaquePassBuilderOptions? implicit; LicmMssaNoAccForPromotionCap : uint) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetCallGraphProfile", library="lib/LLVM.dll")]
def LLVMPassBuilderOptionsSetCallGraphProfile(Options : LLVMOpaquePassBuilderOptions? implicit; CallGraphProfile : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetMergeFunctions", library="lib/LLVM.dll")]
def LLVMPassBuilderOptionsSetMergeFunctions(Options : LLVMOpaquePassBuilderOptions? implicit; MergeFunctions : int) : void {}
[extern(cdecl, name="LLVMDisposePassBuilderOptions", library="lib/LLVM.dll")]
def LLVMDisposePassBuilderOptions(Options : LLVMOpaquePassBuilderOptions? implicit) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderCreate", library="lib/LLVM.dll")]
def LLVMPassManagerBuilderCreate() : LLVMOpaquePassManagerBuilder? {}
[extern(cdecl, name="LLVMPassManagerBuilderDispose", library="lib/LLVM.dll")]
def LLVMPassManagerBuilderDispose(PMB : LLVMOpaquePassManagerBuilder? implicit) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderSetOptLevel", library="lib/LLVM.dll")]
def LLVMPassManagerBuilderSetOptLevel(PMB : LLVMOpaquePassManagerBuilder? implicit; OptLevel : uint) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderSetSizeLevel", library="lib/LLVM.dll")]
def LLVMPassManagerBuilderSetSizeLevel(PMB : LLVMOpaquePassManagerBuilder? implicit; SizeLevel : uint) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderSetDisableUnitAtATime", library="lib/LLVM.dll")]
def LLVMPassManagerBuilderSetDisableUnitAtATime(PMB : LLVMOpaquePassManagerBuilder? implicit; Value : int) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderSetDisableUnrollLoops", library="lib/LLVM.dll")]
def LLVMPassManagerBuilderSetDisableUnrollLoops(PMB : LLVMOpaquePassManagerBuilder? implicit; Value : int) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderSetDisableSimplifyLibCalls", library="lib/LLVM.dll")]
def LLVMPassManagerBuilderSetDisableSimplifyLibCalls(PMB : LLVMOpaquePassManagerBuilder? implicit; Value : int) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderUseInlinerWithThreshold", library="lib/LLVM.dll")]
def LLVMPassManagerBuilderUseInlinerWithThreshold(PMB : LLVMOpaquePassManagerBuilder? implicit; Threshold : uint) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderPopulateFunctionPassManager", library="lib/LLVM.dll")]
def LLVMPassManagerBuilderPopulateFunctionPassManager(PMB : LLVMOpaquePassManagerBuilder? implicit; PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderPopulateModulePassManager", library="lib/LLVM.dll")]
def LLVMPassManagerBuilderPopulateModulePassManager(PMB : LLVMOpaquePassManagerBuilder? implicit; PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddAggressiveDCEPass", library="lib/LLVM.dll")]
def LLVMAddAggressiveDCEPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddDCEPass", library="lib/LLVM.dll")]
def LLVMAddDCEPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddBitTrackingDCEPass", library="lib/LLVM.dll")]
def LLVMAddBitTrackingDCEPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddAlignmentFromAssumptionsPass", library="lib/LLVM.dll")]
def LLVMAddAlignmentFromAssumptionsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddCFGSimplificationPass", library="lib/LLVM.dll")]
def LLVMAddCFGSimplificationPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddDeadStoreEliminationPass", library="lib/LLVM.dll")]
def LLVMAddDeadStoreEliminationPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddScalarizerPass", library="lib/LLVM.dll")]
def LLVMAddScalarizerPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddMergedLoadStoreMotionPass", library="lib/LLVM.dll")]
def LLVMAddMergedLoadStoreMotionPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddGVNPass", library="lib/LLVM.dll")]
def LLVMAddGVNPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddNewGVNPass", library="lib/LLVM.dll")]
def LLVMAddNewGVNPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddIndVarSimplifyPass", library="lib/LLVM.dll")]
def LLVMAddIndVarSimplifyPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddInstructionCombiningPass", library="lib/LLVM.dll")]
def LLVMAddInstructionCombiningPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddInstructionSimplifyPass", library="lib/LLVM.dll")]
def LLVMAddInstructionSimplifyPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddJumpThreadingPass", library="lib/LLVM.dll")]
def LLVMAddJumpThreadingPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLICMPass", library="lib/LLVM.dll")]
def LLVMAddLICMPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopDeletionPass", library="lib/LLVM.dll")]
def LLVMAddLoopDeletionPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopIdiomPass", library="lib/LLVM.dll")]
def LLVMAddLoopIdiomPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopRotatePass", library="lib/LLVM.dll")]
def LLVMAddLoopRotatePass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopRerollPass", library="lib/LLVM.dll")]
def LLVMAddLoopRerollPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopUnrollPass", library="lib/LLVM.dll")]
def LLVMAddLoopUnrollPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopUnrollAndJamPass", library="lib/LLVM.dll")]
def LLVMAddLoopUnrollAndJamPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLowerAtomicPass", library="lib/LLVM.dll")]
def LLVMAddLowerAtomicPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddMemCpyOptPass", library="lib/LLVM.dll")]
def LLVMAddMemCpyOptPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddPartiallyInlineLibCallsPass", library="lib/LLVM.dll")]
def LLVMAddPartiallyInlineLibCallsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddReassociatePass", library="lib/LLVM.dll")]
def LLVMAddReassociatePass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddSCCPPass", library="lib/LLVM.dll")]
def LLVMAddSCCPPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddScalarReplAggregatesPass", library="lib/LLVM.dll")]
def LLVMAddScalarReplAggregatesPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddScalarReplAggregatesPassSSA", library="lib/LLVM.dll")]
def LLVMAddScalarReplAggregatesPassSSA(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddScalarReplAggregatesPassWithThreshold", library="lib/LLVM.dll")]
def LLVMAddScalarReplAggregatesPassWithThreshold(PM : LLVMOpaquePassManager? implicit; Threshold : int) : void {}
[extern(cdecl, name="LLVMAddSimplifyLibCallsPass", library="lib/LLVM.dll")]
def LLVMAddSimplifyLibCallsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddTailCallEliminationPass", library="lib/LLVM.dll")]
def LLVMAddTailCallEliminationPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddDemoteMemoryToRegisterPass", library="lib/LLVM.dll")]
def LLVMAddDemoteMemoryToRegisterPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddVerifierPass", library="lib/LLVM.dll")]
def LLVMAddVerifierPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddCorrelatedValuePropagationPass", library="lib/LLVM.dll")]
def LLVMAddCorrelatedValuePropagationPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddEarlyCSEPass", library="lib/LLVM.dll")]
def LLVMAddEarlyCSEPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddEarlyCSEMemSSAPass", library="lib/LLVM.dll")]
def LLVMAddEarlyCSEMemSSAPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLowerExpectIntrinsicPass", library="lib/LLVM.dll")]
def LLVMAddLowerExpectIntrinsicPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLowerConstantIntrinsicsPass", library="lib/LLVM.dll")]
def LLVMAddLowerConstantIntrinsicsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddTypeBasedAliasAnalysisPass", library="lib/LLVM.dll")]
def LLVMAddTypeBasedAliasAnalysisPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddScopedNoAliasAAPass", library="lib/LLVM.dll")]
def LLVMAddScopedNoAliasAAPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddBasicAliasAnalysisPass", library="lib/LLVM.dll")]
def LLVMAddBasicAliasAnalysisPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddUnifyFunctionExitNodesPass", library="lib/LLVM.dll")]
def LLVMAddUnifyFunctionExitNodesPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLowerSwitchPass", library="lib/LLVM.dll")]
def LLVMAddLowerSwitchPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddPromoteMemoryToRegisterPass", library="lib/LLVM.dll")]
def LLVMAddPromoteMemoryToRegisterPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddAddDiscriminatorsPass", library="lib/LLVM.dll")]
def LLVMAddAddDiscriminatorsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopVectorizePass", library="lib/LLVM.dll")]
def LLVMAddLoopVectorizePass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddSLPVectorizePass", library="lib/LLVM.dll")]
def LLVMAddSLPVectorizePass(PM : LLVMOpaquePassManager? implicit) : void {}
