require dasbind public
options gen2
module llvm_func shared
require llvm_struct
require llvm_enum
[extern(cdecl, name="LLVMVerifyModule", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMVerifyModule(M : LLVMOpaqueModule? implicit; Action : LLVMVerifierFailureAction; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMVerifyFunction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMVerifyFunction(Fn : LLVMOpaqueValue? implicit; Action : LLVMVerifierFailureAction) : int {}
[extern(cdecl, name="LLVMViewFunctionCFG", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMViewFunctionCFG(Fn : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMViewFunctionCFGOnly", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMViewFunctionCFGOnly(Fn : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMParseBitcode", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMParseBitcode(MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutModule : LLVMOpaqueModule ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMParseBitcode2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMParseBitcode2(MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutModule : LLVMOpaqueModule ?? implicit) : int {}
[extern(cdecl, name="LLVMParseBitcodeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMParseBitcodeInContext(ContextRef : LLVMOpaqueContext? implicit; MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutModule : LLVMOpaqueModule ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMParseBitcodeInContext2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMParseBitcodeInContext2(ContextRef : LLVMOpaqueContext? implicit; MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutModule : LLVMOpaqueModule ?? implicit) : int {}
[extern(cdecl, name="LLVMGetBitcodeModuleInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetBitcodeModuleInContext(ContextRef : LLVMOpaqueContext? implicit; MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutM : LLVMOpaqueModule ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMGetBitcodeModuleInContext2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetBitcodeModuleInContext2(ContextRef : LLVMOpaqueContext? implicit; MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutM : LLVMOpaqueModule ?? implicit) : int {}
[extern(cdecl, name="LLVMGetBitcodeModule", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetBitcodeModule(MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutM : LLVMOpaqueModule ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMGetBitcodeModule2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetBitcodeModule2(MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutM : LLVMOpaqueModule ?? implicit) : int {}
[extern(cdecl, name="LLVMWriteBitcodeToFile", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMWriteBitcodeToFile(M : LLVMOpaqueModule? implicit; Path : string implicit) : int {}
[extern(cdecl, name="LLVMWriteBitcodeToFD", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMWriteBitcodeToFD(M : LLVMOpaqueModule? implicit; FD : int; ShouldClose : int; Unbuffered : int) : int {}
[extern(cdecl, name="LLVMWriteBitcodeToFileHandle", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMWriteBitcodeToFileHandle(M : LLVMOpaqueModule? implicit; Handle : int) : int {}
[extern(cdecl, name="LLVMWriteBitcodeToMemoryBuffer", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMWriteBitcodeToMemoryBuffer(M : LLVMOpaqueModule? implicit) : LLVMOpaqueMemoryBuffer? {}
[extern(cdecl, name="LLVMGetOrInsertComdat", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetOrInsertComdat(M : LLVMOpaqueModule? implicit; Name : string implicit) : LLVMComdat? {}
[extern(cdecl, name="LLVMGetComdat", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetComdat(V : LLVMOpaqueValue? implicit) : LLVMComdat? {}
[extern(cdecl, name="LLVMSetComdat", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetComdat(V : LLVMOpaqueValue? implicit; C : LLVMComdat? implicit) : void {}
[extern(cdecl, name="LLVMGetComdatSelectionKind", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetComdatSelectionKind(C : LLVMComdat? implicit) : LLVMComdatSelectionKind {}
[extern(cdecl, name="LLVMSetComdatSelectionKind", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetComdatSelectionKind(C : LLVMComdat? implicit; Kind : LLVMComdatSelectionKind) : void {}
[extern(cdecl, name="LLVMResetFatalErrorHandler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMResetFatalErrorHandler() : void {}
[extern(cdecl, name="LLVMEnablePrettyStackTrace", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMEnablePrettyStackTrace() : void {}
[extern(cdecl, name="LLVMShutdown", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMShutdown() : void {}
[extern(cdecl, name="LLVMGetVersion", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetVersion(Major : uint? implicit; Minor : uint? implicit; Patch : uint? implicit) : void {}
[extern(cdecl, name="LLVMCreateMessage", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateMessage(Message : string implicit) : string {}
[extern(cdecl, name="LLVMDisposeMessage", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeMessage(Message : string implicit) : void {}
[extern(cdecl, name="LLVMContextCreate", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMContextCreate() : LLVMOpaqueContext? {}
[extern(cdecl, name="LLVMGetGlobalContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetGlobalContext() : LLVMOpaqueContext? {}
[extern(cdecl, name="LLVMContextGetDiagnosticContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMContextGetDiagnosticContext(C : LLVMOpaqueContext? implicit) : void? {}
[extern(cdecl, name="LLVMContextShouldDiscardValueNames", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMContextShouldDiscardValueNames(C : LLVMOpaqueContext? implicit) : int {}
[extern(cdecl, name="LLVMContextSetDiscardValueNames", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMContextSetDiscardValueNames(C : LLVMOpaqueContext? implicit; Discard : int) : void {}
[extern(cdecl, name="LLVMContextSetOpaquePointers", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMContextSetOpaquePointers(C : LLVMOpaqueContext? implicit; OpaquePointers : int) : void {}
[extern(cdecl, name="LLVMContextDispose", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMContextDispose(C : LLVMOpaqueContext? implicit) : void {}
[extern(cdecl, name="LLVMGetDiagInfoDescription", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetDiagInfoDescription(DI : LLVMOpaqueDiagnosticInfo? implicit) : string {}
[extern(cdecl, name="LLVMGetDiagInfoSeverity", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetDiagInfoSeverity(DI : LLVMOpaqueDiagnosticInfo? implicit) : LLVMDiagnosticSeverity {}
[extern(cdecl, name="LLVMGetMDKindIDInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetMDKindIDInContext(C : LLVMOpaqueContext? implicit; Name : string implicit; SLen : uint) : uint {}
[extern(cdecl, name="LLVMGetMDKindID", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetMDKindID(Name : string implicit; SLen : uint) : uint {}
[extern(cdecl, name="LLVMGetEnumAttributeKindForName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetEnumAttributeKindForName(Name : string implicit; SLen : uint64) : uint {}
[extern(cdecl, name="LLVMGetLastEnumAttributeKind", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetLastEnumAttributeKind() : uint {}
[extern(cdecl, name="LLVMCreateEnumAttribute", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateEnumAttribute(C : LLVMOpaqueContext? implicit; KindID : uint; Val : uint64) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMGetEnumAttributeKind", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetEnumAttributeKind(A : LLVMOpaqueAttributeRef? implicit) : uint {}
[extern(cdecl, name="LLVMGetEnumAttributeValue", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetEnumAttributeValue(A : LLVMOpaqueAttributeRef? implicit) : uint64 {}
[extern(cdecl, name="LLVMCreateTypeAttribute", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateTypeAttribute(C : LLVMOpaqueContext? implicit; KindID : uint; type_ref : LLVMOpaqueType? implicit) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMGetTypeAttributeValue", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetTypeAttributeValue(A : LLVMOpaqueAttributeRef? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMCreateStringAttribute", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateStringAttribute(C : LLVMOpaqueContext? implicit; K : string implicit; KLength : uint; V : string implicit; VLength : uint) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMGetStringAttributeKind", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetStringAttributeKind(A : LLVMOpaqueAttributeRef? implicit; Length : uint? implicit) : string {}
[extern(cdecl, name="LLVMGetStringAttributeValue", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetStringAttributeValue(A : LLVMOpaqueAttributeRef? implicit; Length : uint? implicit) : string {}
[extern(cdecl, name="LLVMIsEnumAttribute", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsEnumAttribute(A : LLVMOpaqueAttributeRef? implicit) : int {}
[extern(cdecl, name="LLVMIsStringAttribute", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsStringAttribute(A : LLVMOpaqueAttributeRef? implicit) : int {}
[extern(cdecl, name="LLVMIsTypeAttribute", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsTypeAttribute(A : LLVMOpaqueAttributeRef? implicit) : int {}
[extern(cdecl, name="LLVMGetTypeByName2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetTypeByName2(C : LLVMOpaqueContext? implicit; Name : string implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMModuleCreateWithName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMModuleCreateWithName(ModuleID : string implicit) : LLVMOpaqueModule? {}
[extern(cdecl, name="LLVMModuleCreateWithNameInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMModuleCreateWithNameInContext(ModuleID : string implicit; C : LLVMOpaqueContext? implicit) : LLVMOpaqueModule? {}
[extern(cdecl, name="LLVMCloneModule", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCloneModule(M : LLVMOpaqueModule? implicit) : LLVMOpaqueModule? {}
[extern(cdecl, name="LLVMDisposeModule", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeModule(M : LLVMOpaqueModule? implicit) : void {}
[extern(cdecl, name="LLVMGetModuleIdentifier", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetModuleIdentifier(M : LLVMOpaqueModule? implicit; Len : uint64? implicit) : string {}
[extern(cdecl, name="LLVMSetModuleIdentifier", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetModuleIdentifier(M : LLVMOpaqueModule? implicit; Ident : string implicit; Len : uint64) : void {}
[extern(cdecl, name="LLVMGetSourceFileName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetSourceFileName(M : LLVMOpaqueModule? implicit; Len : uint64? implicit) : string {}
[extern(cdecl, name="LLVMSetSourceFileName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetSourceFileName(M : LLVMOpaqueModule? implicit; Name : string implicit; Len : uint64) : void {}
[extern(cdecl, name="LLVMGetDataLayoutStr", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetDataLayoutStr(M : LLVMOpaqueModule? implicit) : string {}
[extern(cdecl, name="LLVMGetDataLayout", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetDataLayout(M : LLVMOpaqueModule? implicit) : string {}
[extern(cdecl, name="LLVMSetDataLayout", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetDataLayout(M : LLVMOpaqueModule? implicit; DataLayoutStr : string implicit) : void {}
[extern(cdecl, name="LLVMGetTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetTarget(M : LLVMOpaqueModule? implicit) : string {}
[extern(cdecl, name="LLVMSetTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetTarget(M : LLVMOpaqueModule? implicit; Triple : string implicit) : void {}
[extern(cdecl, name="LLVMCopyModuleFlagsMetadata", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCopyModuleFlagsMetadata(M : LLVMOpaqueModule? implicit; Len : uint64? implicit) : LLVMOpaqueModuleFlagEntry? {}
[extern(cdecl, name="LLVMDisposeModuleFlagsMetadata", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeModuleFlagsMetadata(Entries : LLVMOpaqueModuleFlagEntry? implicit) : void {}
[extern(cdecl, name="LLVMModuleFlagEntriesGetFlagBehavior", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMModuleFlagEntriesGetFlagBehavior(Entries : LLVMOpaqueModuleFlagEntry? implicit; Index : uint) : LLVMModuleFlagBehavior {}
[extern(cdecl, name="LLVMModuleFlagEntriesGetKey", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMModuleFlagEntriesGetKey(Entries : LLVMOpaqueModuleFlagEntry? implicit; Index : uint; Len : uint64? implicit) : string {}
[extern(cdecl, name="LLVMModuleFlagEntriesGetMetadata", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMModuleFlagEntriesGetMetadata(Entries : LLVMOpaqueModuleFlagEntry? implicit; Index : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMGetModuleFlag", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetModuleFlag(M : LLVMOpaqueModule? implicit; Key : string implicit; KeyLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMAddModuleFlag", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddModuleFlag(M : LLVMOpaqueModule? implicit; Behavior : LLVMModuleFlagBehavior; Key : string implicit; KeyLen : uint64; Val : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMDumpModule", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDumpModule(M : LLVMOpaqueModule? implicit) : void {}
[extern(cdecl, name="LLVMPrintModuleToFile", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPrintModuleToFile(M : LLVMOpaqueModule? implicit; Filename : string implicit; ErrorMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMPrintModuleToString", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPrintModuleToString(M : LLVMOpaqueModule? implicit) : string {}
[extern(cdecl, name="LLVMGetModuleInlineAsm", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetModuleInlineAsm(M : LLVMOpaqueModule? implicit; Len : uint64? implicit) : string {}
[extern(cdecl, name="LLVMSetModuleInlineAsm2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetModuleInlineAsm2(M : LLVMOpaqueModule? implicit; Asm : string implicit; Len : uint64) : void {}
[extern(cdecl, name="LLVMAppendModuleInlineAsm", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAppendModuleInlineAsm(M : LLVMOpaqueModule? implicit; Asm : string implicit; Len : uint64) : void {}
[extern(cdecl, name="LLVMGetInlineAsm", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetInlineAsm(Ty : LLVMOpaqueType? implicit; AsmString : string implicit; AsmStringSize : uint64; Constraints : string implicit; ConstraintsSize : uint64; HasSideEffects : int; IsAlignStack : int; Dialect : LLVMInlineAsmDialect; CanThrow : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetModuleContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetModuleContext(M : LLVMOpaqueModule? implicit) : LLVMOpaqueContext? {}
[extern(cdecl, name="LLVMGetTypeByName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetTypeByName(M : LLVMOpaqueModule? implicit; Name : string implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetFirstNamedMetadata", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetFirstNamedMetadata(M : LLVMOpaqueModule? implicit) : LLVMOpaqueNamedMDNode? {}
[extern(cdecl, name="LLVMGetLastNamedMetadata", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetLastNamedMetadata(M : LLVMOpaqueModule? implicit) : LLVMOpaqueNamedMDNode? {}
[extern(cdecl, name="LLVMGetNextNamedMetadata", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNextNamedMetadata(NamedMDNode : LLVMOpaqueNamedMDNode? implicit) : LLVMOpaqueNamedMDNode? {}
[extern(cdecl, name="LLVMGetPreviousNamedMetadata", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetPreviousNamedMetadata(NamedMDNode : LLVMOpaqueNamedMDNode? implicit) : LLVMOpaqueNamedMDNode? {}
[extern(cdecl, name="LLVMGetNamedMetadata", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNamedMetadata(M : LLVMOpaqueModule? implicit; Name : string implicit; NameLen : uint64) : LLVMOpaqueNamedMDNode? {}
[extern(cdecl, name="LLVMGetOrInsertNamedMetadata", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetOrInsertNamedMetadata(M : LLVMOpaqueModule? implicit; Name : string implicit; NameLen : uint64) : LLVMOpaqueNamedMDNode? {}
[extern(cdecl, name="LLVMGetNamedMetadataName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNamedMetadataName(NamedMD : LLVMOpaqueNamedMDNode? implicit; NameLen : uint64? implicit) : string {}
[extern(cdecl, name="LLVMGetNamedMetadataNumOperands", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNamedMetadataNumOperands(M : LLVMOpaqueModule? implicit; Name : string implicit) : uint {}
[extern(cdecl, name="LLVMGetNamedMetadataOperands", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNamedMetadataOperands(M : LLVMOpaqueModule? implicit; Name : string implicit; Dest : LLVMOpaqueValue ?? implicit) : void {}
[extern(cdecl, name="LLVMAddNamedMetadataOperand", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddNamedMetadataOperand(M : LLVMOpaqueModule? implicit; Name : string implicit; Val : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetDebugLocDirectory", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetDebugLocDirectory(Val : LLVMOpaqueValue? implicit; Length : uint? implicit) : string {}
[extern(cdecl, name="LLVMGetDebugLocFilename", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetDebugLocFilename(Val : LLVMOpaqueValue? implicit; Length : uint? implicit) : string {}
[extern(cdecl, name="LLVMGetDebugLocLine", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetDebugLocLine(Val : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetDebugLocColumn", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetDebugLocColumn(Val : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMAddFunction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddFunction(M : LLVMOpaqueModule? implicit; Name : string implicit; FunctionTy : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNamedFunction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNamedFunction(M : LLVMOpaqueModule? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetFirstFunction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetFirstFunction(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetLastFunction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetLastFunction(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNextFunction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNextFunction(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPreviousFunction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetPreviousFunction(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetModuleInlineAsm", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetModuleInlineAsm(M : LLVMOpaqueModule? implicit; Asm : string implicit) : void {}
[extern(cdecl, name="LLVMGetTypeKind", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetTypeKind(Ty : LLVMOpaqueType? implicit) : LLVMTypeKind {}
[extern(cdecl, name="LLVMTypeIsSized", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMTypeIsSized(Ty : LLVMOpaqueType? implicit) : int {}
[extern(cdecl, name="LLVMGetTypeContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetTypeContext(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueContext? {}
[extern(cdecl, name="LLVMDumpType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDumpType(Val : LLVMOpaqueType? implicit) : void {}
[extern(cdecl, name="LLVMPrintTypeToString", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPrintTypeToString(Val : LLVMOpaqueType? implicit) : string {}
[extern(cdecl, name="LLVMInt1TypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInt1TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt8TypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInt8TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt16TypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInt16TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt32TypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInt32TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt64TypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInt64TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt128TypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInt128TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIntTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIntTypeInContext(C : LLVMOpaqueContext? implicit; NumBits : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt1Type", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInt1Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt8Type", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInt8Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt16Type", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInt16Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt32Type", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInt32Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt64Type", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInt64Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt128Type", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInt128Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIntType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIntType(NumBits : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetIntTypeWidth", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetIntTypeWidth(IntegerTy : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMHalfTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMHalfTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMBFloatTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBFloatTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMFloatTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMFloatTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMDoubleTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDoubleTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMX86FP80TypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMX86FP80TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMFP128TypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMFP128TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMPPCFP128TypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPPCFP128TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMHalfType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMHalfType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMBFloatType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBFloatType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMFloatType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMFloatType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMDoubleType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDoubleType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMX86FP80Type", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMX86FP80Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMFP128Type", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMFP128Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMPPCFP128Type", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPPCFP128Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMFunctionType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMFunctionType(ReturnType : LLVMOpaqueType? implicit; ParamTypes : LLVMOpaqueType ?? implicit; ParamCount : uint; IsVarArg : int) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIsFunctionVarArg", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsFunctionVarArg(FunctionTy : LLVMOpaqueType? implicit) : int {}
[extern(cdecl, name="LLVMGetReturnType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetReturnType(FunctionTy : LLVMOpaqueType? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMCountParamTypes", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCountParamTypes(FunctionTy : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMGetParamTypes", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetParamTypes(FunctionTy : LLVMOpaqueType? implicit; Dest : LLVMOpaqueType ?? implicit) : void {}
[extern(cdecl, name="LLVMStructTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMStructTypeInContext(C : LLVMOpaqueContext? implicit; ElementTypes : LLVMOpaqueType ?? implicit; ElementCount : uint; Packed : int) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMStructType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMStructType(ElementTypes : LLVMOpaqueType ?? implicit; ElementCount : uint; Packed : int) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMStructCreateNamed", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMStructCreateNamed(C : LLVMOpaqueContext? implicit; Name : string implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetStructName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetStructName(Ty : LLVMOpaqueType? implicit) : string {}
[extern(cdecl, name="LLVMStructSetBody", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMStructSetBody(StructTy : LLVMOpaqueType? implicit; ElementTypes : LLVMOpaqueType ?? implicit; ElementCount : uint; Packed : int) : void {}
[extern(cdecl, name="LLVMCountStructElementTypes", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCountStructElementTypes(StructTy : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMGetStructElementTypes", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetStructElementTypes(StructTy : LLVMOpaqueType? implicit; Dest : LLVMOpaqueType ?? implicit) : void {}
[extern(cdecl, name="LLVMStructGetTypeAtIndex", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMStructGetTypeAtIndex(StructTy : LLVMOpaqueType? implicit; i : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIsPackedStruct", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsPackedStruct(StructTy : LLVMOpaqueType? implicit) : int {}
[extern(cdecl, name="LLVMIsOpaqueStruct", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsOpaqueStruct(StructTy : LLVMOpaqueType? implicit) : int {}
[extern(cdecl, name="LLVMIsLiteralStruct", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsLiteralStruct(StructTy : LLVMOpaqueType? implicit) : int {}
[extern(cdecl, name="LLVMGetElementType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetElementType(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetSubtypes", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetSubtypes(Tp : LLVMOpaqueType? implicit; Arr : LLVMOpaqueType ?? implicit) : void {}
[extern(cdecl, name="LLVMGetNumContainedTypes", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNumContainedTypes(Tp : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMArrayType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMArrayType(ElementType : LLVMOpaqueType? implicit; ElementCount : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetArrayLength", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetArrayLength(ArrayTy : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMPointerType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPointerType(ElementType : LLVMOpaqueType? implicit; AddressSpace : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMPointerTypeIsOpaque", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPointerTypeIsOpaque(Ty : LLVMOpaqueType? implicit) : int {}
[extern(cdecl, name="LLVMPointerTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPointerTypeInContext(C : LLVMOpaqueContext? implicit; AddressSpace : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetPointerAddressSpace", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetPointerAddressSpace(PointerTy : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMVectorType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMVectorType(ElementType : LLVMOpaqueType? implicit; ElementCount : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMScalableVectorType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMScalableVectorType(ElementType : LLVMOpaqueType? implicit; ElementCount : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetVectorSize", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetVectorSize(VectorTy : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMVoidTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMVoidTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMLabelTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMLabelTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMX86MMXTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMX86MMXTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMX86AMXTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMX86AMXTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMTokenTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMTokenTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMMetadataTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMMetadataTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMVoidType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMVoidType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMLabelType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMLabelType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMX86MMXType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMX86MMXType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMX86AMXType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMX86AMXType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMTargetExtTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMTargetExtTypeInContext(C : LLVMOpaqueContext? implicit; Name : string implicit; TypeParams : LLVMOpaqueType ?? implicit; TypeParamCount : uint; IntParams : uint? implicit; IntParamCount : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMTypeOf", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMTypeOf(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetValueKind", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetValueKind(Val : LLVMOpaqueValue? implicit) : LLVMValueKind {}
[extern(cdecl, name="LLVMGetValueName2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetValueName2(Val : LLVMOpaqueValue? implicit; Length : uint64? implicit) : string {}
[extern(cdecl, name="LLVMSetValueName2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetValueName2(Val : LLVMOpaqueValue? implicit; Name : string implicit; NameLen : uint64) : void {}
[extern(cdecl, name="LLVMDumpValue", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDumpValue(Val : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMPrintValueToString", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPrintValueToString(Val : LLVMOpaqueValue? implicit) : string {}
[extern(cdecl, name="LLVMReplaceAllUsesWith", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMReplaceAllUsesWith(OldVal : LLVMOpaqueValue? implicit; NewVal : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMIsConstant", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsConstant(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMIsUndef", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsUndef(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMIsPoison", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsPoison(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMIsAArgument", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAArgument(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsABasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsABasicBlock(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAInlineAsm", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAInlineAsm(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAUser", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAUser(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstant", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAConstant(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsABlockAddress", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsABlockAddress(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantAggregateZero", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAConstantAggregateZero(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantArray", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAConstantArray(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantDataSequential", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAConstantDataSequential(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantDataArray", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAConstantDataArray(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantDataVector", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAConstantDataVector(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantExpr", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAConstantExpr(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantFP", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAConstantFP(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantInt", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAConstantInt(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantPointerNull", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAConstantPointerNull(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantStruct", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAConstantStruct(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantTokenNone", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAConstantTokenNone(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantVector", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAConstantVector(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAGlobalValue", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAGlobalValue(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAGlobalAlias", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAGlobalAlias(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAGlobalObject", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAGlobalObject(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFunction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAFunction(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAGlobalVariable", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAGlobalVariable(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAGlobalIFunc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAGlobalIFunc(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAUndefValue", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAUndefValue(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAPoisonValue", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAPoisonValue(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAInstruction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAInstruction(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAUnaryOperator", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAUnaryOperator(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsABinaryOperator", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsABinaryOperator(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACallInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsACallInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAIntrinsicInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAIntrinsicInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsADbgInfoIntrinsic", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsADbgInfoIntrinsic(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsADbgVariableIntrinsic", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsADbgVariableIntrinsic(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsADbgDeclareInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsADbgDeclareInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsADbgLabelInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsADbgLabelInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAMemIntrinsic", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAMemIntrinsic(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAMemCpyInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAMemCpyInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAMemMoveInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAMemMoveInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAMemSetInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAMemSetInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACmpInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsACmpInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFCmpInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAFCmpInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAICmpInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAICmpInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAExtractElementInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAExtractElementInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAGetElementPtrInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAGetElementPtrInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAInsertElementInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAInsertElementInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAInsertValueInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAInsertValueInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsALandingPadInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsALandingPadInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAPHINode", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAPHINode(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsASelectInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsASelectInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAShuffleVectorInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAShuffleVectorInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAStoreInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAStoreInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsABranchInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsABranchInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAIndirectBrInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAIndirectBrInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAInvokeInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAInvokeInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAReturnInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAReturnInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsASwitchInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsASwitchInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAUnreachableInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAUnreachableInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAResumeInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAResumeInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACleanupReturnInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsACleanupReturnInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACatchReturnInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsACatchReturnInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACatchSwitchInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsACatchSwitchInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACallBrInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsACallBrInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFuncletPadInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAFuncletPadInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACatchPadInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsACatchPadInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACleanupPadInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsACleanupPadInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAUnaryInstruction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAUnaryInstruction(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAAllocaInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAAllocaInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACastInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsACastInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAAddrSpaceCastInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAAddrSpaceCastInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsABitCastInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsABitCastInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFPExtInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAFPExtInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFPToSIInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAFPToSIInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFPToUIInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAFPToUIInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFPTruncInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAFPTruncInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAIntToPtrInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAIntToPtrInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAPtrToIntInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAPtrToIntInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsASExtInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsASExtInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsASIToFPInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsASIToFPInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsATruncInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsATruncInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAUIToFPInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAUIToFPInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAZExtInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAZExtInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAExtractValueInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAExtractValueInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsALoadInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsALoadInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAVAArgInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAVAArgInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFreezeInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAFreezeInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAAtomicCmpXchgInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAAtomicCmpXchgInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAAtomicRMWInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAAtomicRMWInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFenceInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAFenceInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAMDNode", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAMDNode(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAMDString", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAMDString(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetValueName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetValueName(Val : LLVMOpaqueValue? implicit) : string {}
[extern(cdecl, name="LLVMSetValueName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetValueName(Val : LLVMOpaqueValue? implicit; Name : string implicit) : void {}
[extern(cdecl, name="LLVMGetFirstUse", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetFirstUse(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueUse? {}
[extern(cdecl, name="LLVMGetNextUse", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNextUse(U : LLVMOpaqueUse? implicit) : LLVMOpaqueUse? {}
[extern(cdecl, name="LLVMGetUser", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetUser(U : LLVMOpaqueUse? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetUsedValue", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetUsedValue(U : LLVMOpaqueUse? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetOperand", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetOperand(Val : LLVMOpaqueValue? implicit; Index : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetOperandUse", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetOperandUse(Val : LLVMOpaqueValue? implicit; Index : uint) : LLVMOpaqueUse? {}
[extern(cdecl, name="LLVMSetOperand", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetOperand(User : LLVMOpaqueValue? implicit; Index : uint; Val : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetNumOperands", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNumOperands(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMConstNull", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstNull(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstAllOnes", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstAllOnes(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetUndef", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetUndef(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPoison", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetPoison(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsNull", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsNull(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMConstPointerNull", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstPointerNull(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstInt", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstInt(IntTy : LLVMOpaqueType? implicit; N : uint64; SignExtend : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstIntOfArbitraryPrecision", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstIntOfArbitraryPrecision(IntTy : LLVMOpaqueType? implicit; NumWords : uint; Words : void? const) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstIntOfString", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstIntOfString(IntTy : LLVMOpaqueType? implicit; Text : string implicit; Radix : uint8) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstIntOfStringAndSize", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstIntOfStringAndSize(IntTy : LLVMOpaqueType? implicit; Text : string implicit; SLen : uint; Radix : uint8) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstReal", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstReal(RealTy : LLVMOpaqueType? implicit; N : double) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstRealOfString", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstRealOfString(RealTy : LLVMOpaqueType? implicit; Text : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstRealOfStringAndSize", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstRealOfStringAndSize(RealTy : LLVMOpaqueType? implicit; Text : string implicit; SLen : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstIntGetZExtValue", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstIntGetZExtValue(ConstantVal : LLVMOpaqueValue? implicit) : uint64 {}
[extern(cdecl, name="LLVMConstIntGetSExtValue", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstIntGetSExtValue(ConstantVal : LLVMOpaqueValue? implicit) : int64 {}
[extern(cdecl, name="LLVMConstRealGetDouble", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstRealGetDouble(ConstantVal : LLVMOpaqueValue? implicit; losesInfo : int? implicit) : double {}
[extern(cdecl, name="LLVMConstStringInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstStringInContext(C : LLVMOpaqueContext? implicit; Str : string implicit; Length : uint; DontNullTerminate : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstString", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstString(Str : string implicit; Length : uint; DontNullTerminate : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsConstantString", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsConstantString(c : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMGetAsString", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetAsString(c : LLVMOpaqueValue? implicit; Length : uint64? implicit) : string {}
[extern(cdecl, name="LLVMConstStructInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstStructInContext(C : LLVMOpaqueContext? implicit; ConstantVals : LLVMOpaqueValue ?? implicit; Count : uint; Packed : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstStruct", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstStruct(ConstantVals : LLVMOpaqueValue ?? implicit; Count : uint; Packed : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstArray", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstArray(ElementTy : LLVMOpaqueType? implicit; ConstantVals : LLVMOpaqueValue ?? implicit; Length : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNamedStruct", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstNamedStruct(StructTy : LLVMOpaqueType? implicit; ConstantVals : LLVMOpaqueValue ?? implicit; Count : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetAggregateElement", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetAggregateElement(C : LLVMOpaqueValue? implicit; Idx : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetElementAsConstant", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetElementAsConstant(C : LLVMOpaqueValue? implicit; idx : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstVector", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstVector(ScalarConstantVals : LLVMOpaqueValue ?? implicit; Size : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetConstOpcode", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetConstOpcode(ConstantVal : LLVMOpaqueValue? implicit) : LLVMOpcode {}
[extern(cdecl, name="LLVMAlignOf", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAlignOf(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSizeOf", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSizeOf(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNeg", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstNeg(ConstantVal : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNSWNeg", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstNSWNeg(ConstantVal : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNUWNeg", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstNUWNeg(ConstantVal : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNot", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstNot(ConstantVal : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstAdd", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstAdd(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNSWAdd", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstNSWAdd(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNUWAdd", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstNUWAdd(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstSub", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstSub(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNSWSub", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstNSWSub(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNUWSub", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstNUWSub(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstMul", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstMul(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNSWMul", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstNSWMul(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNUWMul", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstNUWMul(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstAnd", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstAnd(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstOr", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstOr(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstXor", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstXor(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstICmp", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstICmp(Predicate : LLVMIntPredicate; LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstFCmp", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstFCmp(Predicate : LLVMRealPredicate; LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstShl", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstShl(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstLShr", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstLShr(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstAShr", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstAShr(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstGEP2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstGEP2(Ty : LLVMOpaqueType? implicit; ConstantVal : LLVMOpaqueValue? implicit; ConstantIndices : LLVMOpaqueValue ?? implicit; NumIndices : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstInBoundsGEP2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstInBoundsGEP2(Ty : LLVMOpaqueType? implicit; ConstantVal : LLVMOpaqueValue? implicit; ConstantIndices : LLVMOpaqueValue ?? implicit; NumIndices : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstTrunc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstTrunc(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstSExt", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstSExt(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstZExt", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstZExt(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstFPTrunc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstFPTrunc(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstFPExt", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstFPExt(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstUIToFP", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstUIToFP(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstSIToFP", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstSIToFP(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstFPToUI", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstFPToUI(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstFPToSI", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstFPToSI(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstPtrToInt", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstPtrToInt(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstIntToPtr", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstIntToPtr(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstBitCast", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstBitCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstAddrSpaceCast", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstAddrSpaceCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstZExtOrBitCast", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstZExtOrBitCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstSExtOrBitCast", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstSExtOrBitCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstTruncOrBitCast", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstTruncOrBitCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstPointerCast", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstPointerCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstIntCast", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstIntCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit; isSigned : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstFPCast", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstFPCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstSelect", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstSelect(ConstantCondition : LLVMOpaqueValue? implicit; ConstantIfTrue : LLVMOpaqueValue? implicit; ConstantIfFalse : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstExtractElement", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstExtractElement(VectorConstant : LLVMOpaqueValue? implicit; IndexConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstInsertElement", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstInsertElement(VectorConstant : LLVMOpaqueValue? implicit; ElementValueConstant : LLVMOpaqueValue? implicit; IndexConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstShuffleVector", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstShuffleVector(VectorAConstant : LLVMOpaqueValue? implicit; VectorBConstant : LLVMOpaqueValue? implicit; MaskConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBlockAddress", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBlockAddress(F : LLVMOpaqueValue? implicit; BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstInlineAsm", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConstInlineAsm(Ty : LLVMOpaqueType? implicit; AsmString : string implicit; Constraints : string implicit; HasSideEffects : int; IsAlignStack : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetGlobalParent", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetGlobalParent(Global : LLVMOpaqueValue? implicit) : LLVMOpaqueModule? {}
[extern(cdecl, name="LLVMIsDeclaration", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsDeclaration(Global : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMGetLinkage", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetLinkage(Global : LLVMOpaqueValue? implicit) : LLVMLinkage {}
[extern(cdecl, name="LLVMSetLinkage", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetLinkage(Global : LLVMOpaqueValue? implicit; Linkage : LLVMLinkage) : void {}
[extern(cdecl, name="LLVMGetSection", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetSection(Global : LLVMOpaqueValue? implicit) : string {}
[extern(cdecl, name="LLVMSetSection", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetSection(Global : LLVMOpaqueValue? implicit; Section : string implicit) : void {}
[extern(cdecl, name="LLVMGetVisibility", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetVisibility(Global : LLVMOpaqueValue? implicit) : LLVMVisibility {}
[extern(cdecl, name="LLVMSetVisibility", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetVisibility(Global : LLVMOpaqueValue? implicit; Viz : LLVMVisibility) : void {}
[extern(cdecl, name="LLVMGetDLLStorageClass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetDLLStorageClass(Global : LLVMOpaqueValue? implicit) : LLVMDLLStorageClass {}
[extern(cdecl, name="LLVMSetDLLStorageClass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetDLLStorageClass(Global : LLVMOpaqueValue? implicit; Class : LLVMDLLStorageClass) : void {}
[extern(cdecl, name="LLVMGetUnnamedAddress", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetUnnamedAddress(Global : LLVMOpaqueValue? implicit) : LLVMUnnamedAddr {}
[extern(cdecl, name="LLVMSetUnnamedAddress", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetUnnamedAddress(Global : LLVMOpaqueValue? implicit; UnnamedAddr : LLVMUnnamedAddr) : void {}
[extern(cdecl, name="LLVMGlobalGetValueType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGlobalGetValueType(Global : LLVMOpaqueValue? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMHasUnnamedAddr", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMHasUnnamedAddr(Global : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetUnnamedAddr", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetUnnamedAddr(Global : LLVMOpaqueValue? implicit; HasUnnamedAddr : int) : void {}
[extern(cdecl, name="LLVMGetAlignment", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetAlignment(V : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMSetAlignment", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetAlignment(V : LLVMOpaqueValue? implicit; Bytes : uint) : void {}
[extern(cdecl, name="LLVMGlobalSetMetadata", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGlobalSetMetadata(Global : LLVMOpaqueValue? implicit; Kind : uint; MD : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMGlobalEraseMetadata", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGlobalEraseMetadata(Global : LLVMOpaqueValue? implicit; Kind : uint) : void {}
[extern(cdecl, name="LLVMGlobalClearMetadata", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGlobalClearMetadata(Global : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGlobalCopyAllMetadata", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGlobalCopyAllMetadata(Value : LLVMOpaqueValue? implicit; NumEntries : uint64? implicit) : LLVMOpaqueValueMetadataEntry? {}
[extern(cdecl, name="LLVMDisposeValueMetadataEntries", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeValueMetadataEntries(Entries : LLVMOpaqueValueMetadataEntry? implicit) : void {}
[extern(cdecl, name="LLVMValueMetadataEntriesGetKind", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMValueMetadataEntriesGetKind(Entries : LLVMOpaqueValueMetadataEntry? implicit; Index : uint) : uint {}
[extern(cdecl, name="LLVMValueMetadataEntriesGetMetadata", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMValueMetadataEntriesGetMetadata(Entries : LLVMOpaqueValueMetadataEntry? implicit; Index : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMAddGlobal", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddGlobal(M : LLVMOpaqueModule? implicit; Ty : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMAddGlobalInAddressSpace", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddGlobalInAddressSpace(M : LLVMOpaqueModule? implicit; Ty : LLVMOpaqueType? implicit; Name : string implicit; AddressSpace : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNamedGlobal", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNamedGlobal(M : LLVMOpaqueModule? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetFirstGlobal", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetFirstGlobal(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetLastGlobal", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetLastGlobal(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNextGlobal", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNextGlobal(GlobalVar : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPreviousGlobal", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetPreviousGlobal(GlobalVar : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMDeleteGlobal", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDeleteGlobal(GlobalVar : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetInitializer", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetInitializer(GlobalVar : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetInitializer", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetInitializer(GlobalVar : LLVMOpaqueValue? implicit; ConstantVal : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMIsThreadLocal", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsThreadLocal(GlobalVar : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetThreadLocal", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetThreadLocal(GlobalVar : LLVMOpaqueValue? implicit; IsThreadLocal : int) : void {}
[extern(cdecl, name="LLVMIsGlobalConstant", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsGlobalConstant(GlobalVar : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetGlobalConstant", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetGlobalConstant(GlobalVar : LLVMOpaqueValue? implicit; IsConstant : int) : void {}
[extern(cdecl, name="LLVMGetThreadLocalMode", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetThreadLocalMode(GlobalVar : LLVMOpaqueValue? implicit) : LLVMThreadLocalMode {}
[extern(cdecl, name="LLVMSetThreadLocalMode", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetThreadLocalMode(GlobalVar : LLVMOpaqueValue? implicit; Mode : LLVMThreadLocalMode) : void {}
[extern(cdecl, name="LLVMIsExternallyInitialized", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsExternallyInitialized(GlobalVar : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetExternallyInitialized", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetExternallyInitialized(GlobalVar : LLVMOpaqueValue? implicit; IsExtInit : int) : void {}
[extern(cdecl, name="LLVMAddAlias2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddAlias2(M : LLVMOpaqueModule? implicit; ValueTy : LLVMOpaqueType? implicit; AddrSpace : uint; Aliasee : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNamedGlobalAlias", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNamedGlobalAlias(M : LLVMOpaqueModule? implicit; Name : string implicit; NameLen : uint64) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetFirstGlobalAlias", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetFirstGlobalAlias(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetLastGlobalAlias", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetLastGlobalAlias(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNextGlobalAlias", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNextGlobalAlias(GA : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPreviousGlobalAlias", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetPreviousGlobalAlias(GA : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMAliasGetAliasee", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAliasGetAliasee(Alias : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMAliasSetAliasee", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAliasSetAliasee(Alias : LLVMOpaqueValue? implicit; Aliasee : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMDeleteFunction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDeleteFunction(Fn : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMHasPersonalityFn", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMHasPersonalityFn(Fn : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMGetPersonalityFn", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetPersonalityFn(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetPersonalityFn", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetPersonalityFn(Fn : LLVMOpaqueValue? implicit; PersonalityFn : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMLookupIntrinsicID", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMLookupIntrinsicID(Name : string implicit; NameLen : uint64) : uint {}
[extern(cdecl, name="LLVMGetIntrinsicID", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetIntrinsicID(Fn : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetIntrinsicDeclaration", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetIntrinsicDeclaration(Mod : LLVMOpaqueModule? implicit; ID : uint; ParamTypes : LLVMOpaqueType ?? implicit; ParamCount : uint64) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIntrinsicGetType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIntrinsicGetType(Ctx : LLVMOpaqueContext? implicit; ID : uint; ParamTypes : LLVMOpaqueType ?? implicit; ParamCount : uint64) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIntrinsicGetName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIntrinsicGetName(ID : uint; NameLength : uint64? implicit) : string {}
[extern(cdecl, name="LLVMIntrinsicCopyOverloadedName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIntrinsicCopyOverloadedName(ID : uint; ParamTypes : LLVMOpaqueType ?? implicit; ParamCount : uint64; NameLength : uint64? implicit) : string {}
[extern(cdecl, name="LLVMIntrinsicCopyOverloadedName2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIntrinsicCopyOverloadedName2(Mod : LLVMOpaqueModule? implicit; ID : uint; ParamTypes : LLVMOpaqueType ?? implicit; ParamCount : uint64; NameLength : uint64? implicit) : string {}
[extern(cdecl, name="LLVMIntrinsicIsOverloaded", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIntrinsicIsOverloaded(ID : uint) : int {}
[extern(cdecl, name="LLVMGetFunctionCallConv", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetFunctionCallConv(Fn : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMSetFunctionCallConv", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetFunctionCallConv(Fn : LLVMOpaqueValue? implicit; CC : uint) : void {}
[extern(cdecl, name="LLVMGetGC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetGC(Fn : LLVMOpaqueValue? implicit) : string {}
[extern(cdecl, name="LLVMSetGC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetGC(Fn : LLVMOpaqueValue? implicit; Name : string implicit) : void {}
[extern(cdecl, name="LLVMAddAttributeAtIndex", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddAttributeAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint; A : LLVMOpaqueAttributeRef? implicit) : void {}
[extern(cdecl, name="LLVMGetAttributeCountAtIndex", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetAttributeCountAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint) : uint {}
[extern(cdecl, name="LLVMGetAttributesAtIndex", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetAttributesAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint; Attrs : LLVMOpaqueAttributeRef ?? implicit) : void {}
[extern(cdecl, name="LLVMGetEnumAttributeAtIndex", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetEnumAttributeAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint; KindID : uint) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMGetStringAttributeAtIndex", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetStringAttributeAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint; K : string implicit; KLen : uint) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMRemoveEnumAttributeAtIndex", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemoveEnumAttributeAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint; KindID : uint) : void {}
[extern(cdecl, name="LLVMRemoveStringAttributeAtIndex", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemoveStringAttributeAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint; K : string implicit; KLen : uint) : void {}
[extern(cdecl, name="LLVMAddTargetDependentFunctionAttr", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddTargetDependentFunctionAttr(Fn : LLVMOpaqueValue? implicit; A : string implicit; V : string implicit) : void {}
[extern(cdecl, name="LLVMCountParams", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCountParams(Fn : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetParams", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetParams(Fn : LLVMOpaqueValue? implicit; Params : LLVMOpaqueValue ?? implicit) : void {}
[extern(cdecl, name="LLVMGetParam", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetParam(Fn : LLVMOpaqueValue? implicit; Index : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetParamParent", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetParamParent(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetFirstParam", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetFirstParam(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetLastParam", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetLastParam(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNextParam", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNextParam(Arg : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPreviousParam", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetPreviousParam(Arg : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetParamAlignment", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetParamAlignment(Arg : LLVMOpaqueValue? implicit; Align : uint) : void {}
[extern(cdecl, name="LLVMAddGlobalIFunc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddGlobalIFunc(M : LLVMOpaqueModule? implicit; Name : string implicit; NameLen : uint64; Ty : LLVMOpaqueType? implicit; AddrSpace : uint; Resolver : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNamedGlobalIFunc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNamedGlobalIFunc(M : LLVMOpaqueModule? implicit; Name : string implicit; NameLen : uint64) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetFirstGlobalIFunc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetFirstGlobalIFunc(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetLastGlobalIFunc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetLastGlobalIFunc(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNextGlobalIFunc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNextGlobalIFunc(IFunc : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPreviousGlobalIFunc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetPreviousGlobalIFunc(IFunc : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetGlobalIFuncResolver", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetGlobalIFuncResolver(IFunc : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetGlobalIFuncResolver", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetGlobalIFuncResolver(IFunc : LLVMOpaqueValue? implicit; Resolver : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMEraseGlobalIFunc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMEraseGlobalIFunc(IFunc : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMRemoveGlobalIFunc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemoveGlobalIFunc(IFunc : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMMDStringInContext2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMMDStringInContext2(C : LLVMOpaqueContext? implicit; Str : string implicit; SLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMMDNodeInContext2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMMDNodeInContext2(C : LLVMOpaqueContext? implicit; MDs : LLVMOpaqueMetadata ?? implicit; Count : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMMetadataAsValue", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMMetadataAsValue(C : LLVMOpaqueContext? implicit; MD : LLVMOpaqueMetadata? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMValueAsMetadata", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMValueAsMetadata(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMGetMDString", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetMDString(V : LLVMOpaqueValue? implicit; Length : uint? implicit) : string {}
[extern(cdecl, name="LLVMGetMDNodeNumOperands", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetMDNodeNumOperands(V : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetMDNodeOperands", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetMDNodeOperands(V : LLVMOpaqueValue? implicit; Dest : LLVMOpaqueValue ?? implicit) : void {}
[extern(cdecl, name="LLVMMDStringInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMMDStringInContext(C : LLVMOpaqueContext? implicit; Str : string implicit; SLen : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMMDString", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMMDString(Str : string implicit; SLen : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMMDNodeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMMDNodeInContext(C : LLVMOpaqueContext? implicit; Vals : LLVMOpaqueValue ?? implicit; Count : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMMDNode", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMMDNode(Vals : LLVMOpaqueValue ?? implicit; Count : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBasicBlockAsValue", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBasicBlockAsValue(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMValueIsBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMValueIsBasicBlock(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMValueAsBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMValueAsBasicBlock(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetBasicBlockName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetBasicBlockName(BB : LLVMOpaqueBasicBlock? implicit) : string {}
[extern(cdecl, name="LLVMGetBasicBlockParent", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetBasicBlockParent(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetBasicBlockTerminator", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetBasicBlockTerminator(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMCountBasicBlocks", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCountBasicBlocks(Fn : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetBasicBlocks", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetBasicBlocks(Fn : LLVMOpaqueValue? implicit; BasicBlocks : LLVMOpaqueBasicBlock ?? implicit) : void {}
[extern(cdecl, name="LLVMGetFirstBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetFirstBasicBlock(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetLastBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetLastBasicBlock(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetNextBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNextBasicBlock(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetPreviousBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetPreviousBasicBlock(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetEntryBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetEntryBasicBlock(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMInsertExistingBasicBlockAfterInsertBlock", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInsertExistingBasicBlockAfterInsertBlock(Builder : LLVMOpaqueBuilder? implicit; BB : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMAppendExistingBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAppendExistingBasicBlock(Fn : LLVMOpaqueValue? implicit; BB : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMCreateBasicBlockInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateBasicBlockInContext(C : LLVMOpaqueContext? implicit; Name : string implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMAppendBasicBlockInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAppendBasicBlockInContext(C : LLVMOpaqueContext? implicit; Fn : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMAppendBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAppendBasicBlock(Fn : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMInsertBasicBlockInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInsertBasicBlockInContext(C : LLVMOpaqueContext? implicit; BB : LLVMOpaqueBasicBlock? implicit; Name : string implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMInsertBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInsertBasicBlock(InsertBeforeBB : LLVMOpaqueBasicBlock? implicit; Name : string implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMDeleteBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDeleteBasicBlock(BB : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMRemoveBasicBlockFromParent", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemoveBasicBlockFromParent(BB : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMMoveBasicBlockBefore", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMMoveBasicBlockBefore(BB : LLVMOpaqueBasicBlock? implicit; MovePos : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMMoveBasicBlockAfter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMMoveBasicBlockAfter(BB : LLVMOpaqueBasicBlock? implicit; MovePos : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMGetFirstInstruction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetFirstInstruction(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetLastInstruction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetLastInstruction(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMHasMetadata", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMHasMetadata(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMGetMetadata", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetMetadata(Val : LLVMOpaqueValue? implicit; KindID : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetMetadata", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetMetadata(Val : LLVMOpaqueValue? implicit; KindID : uint; Node : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMInstructionGetAllMetadataOtherThanDebugLoc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInstructionGetAllMetadataOtherThanDebugLoc(Instr : LLVMOpaqueValue? implicit; NumEntries : uint64? implicit) : LLVMOpaqueValueMetadataEntry? {}
[extern(cdecl, name="LLVMGetInstructionParent", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetInstructionParent(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetNextInstruction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNextInstruction(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPreviousInstruction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetPreviousInstruction(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMInstructionRemoveFromParent", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInstructionRemoveFromParent(Inst : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMInstructionEraseFromParent", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInstructionEraseFromParent(Inst : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMDeleteInstruction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDeleteInstruction(Inst : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetInstructionOpcode", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetInstructionOpcode(Inst : LLVMOpaqueValue? implicit) : LLVMOpcode {}
[extern(cdecl, name="LLVMGetICmpPredicate", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetICmpPredicate(Inst : LLVMOpaqueValue? implicit) : LLVMIntPredicate {}
[extern(cdecl, name="LLVMGetFCmpPredicate", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetFCmpPredicate(Inst : LLVMOpaqueValue? implicit) : LLVMRealPredicate {}
[extern(cdecl, name="LLVMInstructionClone", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInstructionClone(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsATerminatorInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsATerminatorInst(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNumArgOperands", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNumArgOperands(Instr : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMSetInstructionCallConv", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetInstructionCallConv(Instr : LLVMOpaqueValue? implicit; CC : uint) : void {}
[extern(cdecl, name="LLVMGetInstructionCallConv", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetInstructionCallConv(Instr : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMSetInstrParamAlignment", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetInstrParamAlignment(Instr : LLVMOpaqueValue? implicit; Idx : uint; Align : uint) : void {}
[extern(cdecl, name="LLVMAddCallSiteAttribute", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddCallSiteAttribute(C : LLVMOpaqueValue? implicit; Idx : uint; A : LLVMOpaqueAttributeRef? implicit) : void {}
[extern(cdecl, name="LLVMGetCallSiteAttributeCount", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetCallSiteAttributeCount(C : LLVMOpaqueValue? implicit; Idx : uint) : uint {}
[extern(cdecl, name="LLVMGetCallSiteAttributes", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetCallSiteAttributes(C : LLVMOpaqueValue? implicit; Idx : uint; Attrs : LLVMOpaqueAttributeRef ?? implicit) : void {}
[extern(cdecl, name="LLVMGetCallSiteEnumAttribute", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetCallSiteEnumAttribute(C : LLVMOpaqueValue? implicit; Idx : uint; KindID : uint) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMGetCallSiteStringAttribute", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetCallSiteStringAttribute(C : LLVMOpaqueValue? implicit; Idx : uint; K : string implicit; KLen : uint) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMRemoveCallSiteEnumAttribute", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemoveCallSiteEnumAttribute(C : LLVMOpaqueValue? implicit; Idx : uint; KindID : uint) : void {}
[extern(cdecl, name="LLVMRemoveCallSiteStringAttribute", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemoveCallSiteStringAttribute(C : LLVMOpaqueValue? implicit; Idx : uint; K : string implicit; KLen : uint) : void {}
[extern(cdecl, name="LLVMGetCalledFunctionType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetCalledFunctionType(C : LLVMOpaqueValue? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetCalledValue", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetCalledValue(Instr : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsTailCall", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsTailCall(CallInst : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetTailCall", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetTailCall(CallInst : LLVMOpaqueValue? implicit; IsTailCall : int) : void {}
[extern(cdecl, name="LLVMGetNormalDest", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNormalDest(InvokeInst : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetUnwindDest", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetUnwindDest(InvokeInst : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMSetNormalDest", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetNormalDest(InvokeInst : LLVMOpaqueValue? implicit; B : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMSetUnwindDest", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetUnwindDest(InvokeInst : LLVMOpaqueValue? implicit; B : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMGetNumSuccessors", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNumSuccessors(Term : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetSuccessor", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetSuccessor(Term : LLVMOpaqueValue? implicit; i : uint) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMSetSuccessor", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetSuccessor(Term : LLVMOpaqueValue? implicit; i : uint; _block : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMIsConditional", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsConditional(Branch : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMGetCondition", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetCondition(Branch : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetCondition", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetCondition(Branch : LLVMOpaqueValue? implicit; Cond : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetSwitchDefaultDest", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetSwitchDefaultDest(SwitchInstr : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetAllocatedType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetAllocatedType(Alloca : LLVMOpaqueValue? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIsInBounds", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsInBounds(GEP : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetIsInBounds", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetIsInBounds(GEP : LLVMOpaqueValue? implicit; InBounds : int) : void {}
[extern(cdecl, name="LLVMGetGEPSourceElementType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetGEPSourceElementType(GEP : LLVMOpaqueValue? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMAddIncoming", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddIncoming(PhiNode : LLVMOpaqueValue? implicit; IncomingValues : LLVMOpaqueValue ?? implicit; IncomingBlocks : LLVMOpaqueBasicBlock ?? implicit; Count : uint) : void {}
[extern(cdecl, name="LLVMCountIncoming", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCountIncoming(PhiNode : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetIncomingValue", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetIncomingValue(PhiNode : LLVMOpaqueValue? implicit; Index : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetIncomingBlock", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetIncomingBlock(PhiNode : LLVMOpaqueValue? implicit; Index : uint) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetNumIndices", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNumIndices(Inst : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetIndices", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetIndices(Inst : LLVMOpaqueValue? implicit) : uint const? {}
[extern(cdecl, name="LLVMCreateBuilderInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateBuilderInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueBuilder? {}
[extern(cdecl, name="LLVMCreateBuilder", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateBuilder() : LLVMOpaqueBuilder? {}
[extern(cdecl, name="LLVMPositionBuilder", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPositionBuilder(Builder : LLVMOpaqueBuilder? implicit; Block : LLVMOpaqueBasicBlock? implicit; Instr : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMPositionBuilderBefore", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPositionBuilderBefore(Builder : LLVMOpaqueBuilder? implicit; Instr : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMPositionBuilderAtEnd", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPositionBuilderAtEnd(Builder : LLVMOpaqueBuilder? implicit; Block : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMGetInsertBlock", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetInsertBlock(Builder : LLVMOpaqueBuilder? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMClearInsertionPosition", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMClearInsertionPosition(Builder : LLVMOpaqueBuilder? implicit) : void {}
[extern(cdecl, name="LLVMInsertIntoBuilder", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInsertIntoBuilder(Builder : LLVMOpaqueBuilder? implicit; Instr : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMInsertIntoBuilderWithName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInsertIntoBuilderWithName(Builder : LLVMOpaqueBuilder? implicit; Instr : LLVMOpaqueValue? implicit; Name : string implicit) : void {}
[extern(cdecl, name="LLVMDisposeBuilder", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeBuilder(Builder : LLVMOpaqueBuilder? implicit) : void {}
[extern(cdecl, name="LLVMGetCurrentDebugLocation2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetCurrentDebugLocation2(Builder : LLVMOpaqueBuilder? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMSetCurrentDebugLocation2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetCurrentDebugLocation2(Builder : LLVMOpaqueBuilder? implicit; Loc : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMSetInstDebugLocation", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetInstDebugLocation(Builder : LLVMOpaqueBuilder? implicit; Inst : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMAddMetadataToInst", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddMetadataToInst(Builder : LLVMOpaqueBuilder? implicit; Inst : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMBuilderGetDefaultFPMathTag", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuilderGetDefaultFPMathTag(Builder : LLVMOpaqueBuilder? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMBuilderSetDefaultFPMathTag", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuilderSetDefaultFPMathTag(Builder : LLVMOpaqueBuilder? implicit; FPMathTag : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMSetCurrentDebugLocation", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetCurrentDebugLocation(Builder : LLVMOpaqueBuilder? implicit; L : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetCurrentDebugLocation", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetCurrentDebugLocation(Builder : LLVMOpaqueBuilder? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildRetVoid", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildRetVoid(_argument_0x0 : LLVMOpaqueBuilder? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildRet", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildRet(_argument_0x0 : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAggregateRet", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildAggregateRet(_argument_0x0 : LLVMOpaqueBuilder? implicit; RetVals : LLVMOpaqueValue ?? implicit; N : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildBr", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildBr(_argument_0x0 : LLVMOpaqueBuilder? implicit; Dest : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCondBr", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildCondBr(_argument_0x0 : LLVMOpaqueBuilder? implicit; If : LLVMOpaqueValue? implicit; Then : LLVMOpaqueBasicBlock? implicit; Else : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSwitch", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildSwitch(_argument_0x0 : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit; Else : LLVMOpaqueBasicBlock? implicit; NumCases : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildIndirectBr", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildIndirectBr(B : LLVMOpaqueBuilder? implicit; Addr : LLVMOpaqueValue? implicit; NumDests : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildInvoke2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildInvoke2(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Fn : LLVMOpaqueValue? implicit; Args : LLVMOpaqueValue ?? implicit; NumArgs : uint; Then : LLVMOpaqueBasicBlock? implicit; Catch : LLVMOpaqueBasicBlock? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildUnreachable", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildUnreachable(_argument_0x0 : LLVMOpaqueBuilder? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildResume", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildResume(B : LLVMOpaqueBuilder? implicit; Exn : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildLandingPad", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildLandingPad(B : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; PersFn : LLVMOpaqueValue? implicit; NumClauses : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCleanupRet", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildCleanupRet(B : LLVMOpaqueBuilder? implicit; CatchPad : LLVMOpaqueValue? implicit; BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCatchRet", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildCatchRet(B : LLVMOpaqueBuilder? implicit; CatchPad : LLVMOpaqueValue? implicit; BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCatchPad", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildCatchPad(B : LLVMOpaqueBuilder? implicit; ParentPad : LLVMOpaqueValue? implicit; Args : LLVMOpaqueValue ?? implicit; NumArgs : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCleanupPad", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildCleanupPad(B : LLVMOpaqueBuilder? implicit; ParentPad : LLVMOpaqueValue? implicit; Args : LLVMOpaqueValue ?? implicit; NumArgs : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCatchSwitch", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildCatchSwitch(B : LLVMOpaqueBuilder? implicit; ParentPad : LLVMOpaqueValue? implicit; UnwindBB : LLVMOpaqueBasicBlock? implicit; NumHandlers : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMAddCase", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddCase(Switch : LLVMOpaqueValue? implicit; OnVal : LLVMOpaqueValue? implicit; Dest : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMAddDestination", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddDestination(IndirectBr : LLVMOpaqueValue? implicit; Dest : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMGetNumClauses", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNumClauses(LandingPad : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetClause", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetClause(LandingPad : LLVMOpaqueValue? implicit; Idx : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMAddClause", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddClause(LandingPad : LLVMOpaqueValue? implicit; ClauseVal : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMIsCleanup", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsCleanup(LandingPad : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetCleanup", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetCleanup(LandingPad : LLVMOpaqueValue? implicit; Val : int) : void {}
[extern(cdecl, name="LLVMAddHandler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddHandler(CatchSwitch : LLVMOpaqueValue? implicit; Dest : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMGetNumHandlers", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNumHandlers(CatchSwitch : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetHandlers", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetHandlers(CatchSwitch : LLVMOpaqueValue? implicit; Handlers : LLVMOpaqueBasicBlock ?? implicit) : void {}
[extern(cdecl, name="LLVMGetArgOperand", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetArgOperand(Funclet : LLVMOpaqueValue? implicit; i : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetArgOperand", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetArgOperand(Funclet : LLVMOpaqueValue? implicit; i : uint; value : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetParentCatchSwitch", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetParentCatchSwitch(CatchPad : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetParentCatchSwitch", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetParentCatchSwitch(CatchPad : LLVMOpaqueValue? implicit; CatchSwitch : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMBuildAdd", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildAdd(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNSWAdd", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildNSWAdd(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNUWAdd", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildNUWAdd(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFAdd", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildFAdd(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSub", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildSub(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNSWSub", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildNSWSub(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNUWSub", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildNUWSub(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFSub", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildFSub(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildMul", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildMul(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNSWMul", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildNSWMul(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNUWMul", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildNUWMul(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFMul", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildFMul(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildUDiv", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildUDiv(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildExactUDiv", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildExactUDiv(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSDiv", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildSDiv(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildExactSDiv", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildExactSDiv(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFDiv", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildFDiv(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildURem", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildURem(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSRem", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildSRem(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFRem", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildFRem(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildShl", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildShl(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildLShr", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildLShr(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAShr", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildAShr(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAnd", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildAnd(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildOr", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildOr(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildXor", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildXor(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildBinOp", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildBinOp(B : LLVMOpaqueBuilder? implicit; Op : LLVMOpcode; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNeg", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildNeg(_argument_0x0 : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNSWNeg", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildNSWNeg(B : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNUWNeg", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildNUWNeg(B : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFNeg", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildFNeg(_argument_0x0 : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNot", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildNot(_argument_0x0 : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildMalloc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildMalloc(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildArrayMalloc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildArrayMalloc(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Val : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildMemSet", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildMemSet(B : LLVMOpaqueBuilder? implicit; Ptr : LLVMOpaqueValue? implicit; Val : LLVMOpaqueValue? implicit; Len : LLVMOpaqueValue? implicit; Align : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildMemCpy", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildMemCpy(B : LLVMOpaqueBuilder? implicit; Dst : LLVMOpaqueValue? implicit; DstAlign : uint; Src : LLVMOpaqueValue? implicit; SrcAlign : uint; Size : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildMemMove", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildMemMove(B : LLVMOpaqueBuilder? implicit; Dst : LLVMOpaqueValue? implicit; DstAlign : uint; Src : LLVMOpaqueValue? implicit; SrcAlign : uint; Size : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAlloca", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildAlloca(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildArrayAlloca", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildArrayAlloca(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Val : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFree", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildFree(_argument_0x0 : LLVMOpaqueBuilder? implicit; PointerVal : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildLoad2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildLoad2(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; PointerVal : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildStore", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildStore(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; Ptr : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildGEP2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildGEP2(B : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Pointer : LLVMOpaqueValue? implicit; Indices : LLVMOpaqueValue ?? implicit; NumIndices : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildInBoundsGEP2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildInBoundsGEP2(B : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Pointer : LLVMOpaqueValue? implicit; Indices : LLVMOpaqueValue ?? implicit; NumIndices : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildStructGEP2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildStructGEP2(B : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Pointer : LLVMOpaqueValue? implicit; Idx : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildGlobalString", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildGlobalString(B : LLVMOpaqueBuilder? implicit; Str : string implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildGlobalStringPtr", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildGlobalStringPtr(B : LLVMOpaqueBuilder? implicit; Str : string implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetVolatile", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetVolatile(MemoryAccessInst : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetVolatile", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetVolatile(MemoryAccessInst : LLVMOpaqueValue? implicit; IsVolatile : int) : void {}
[extern(cdecl, name="LLVMGetWeak", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetWeak(CmpXchgInst : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetWeak", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetWeak(CmpXchgInst : LLVMOpaqueValue? implicit; IsWeak : int) : void {}
[extern(cdecl, name="LLVMGetOrdering", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetOrdering(MemoryAccessInst : LLVMOpaqueValue? implicit) : LLVMAtomicOrdering {}
[extern(cdecl, name="LLVMSetOrdering", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetOrdering(MemoryAccessInst : LLVMOpaqueValue? implicit; Ordering : LLVMAtomicOrdering) : void {}
[extern(cdecl, name="LLVMGetAtomicRMWBinOp", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetAtomicRMWBinOp(AtomicRMWInst : LLVMOpaqueValue? implicit) : LLVMAtomicRMWBinOp {}
[extern(cdecl, name="LLVMSetAtomicRMWBinOp", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetAtomicRMWBinOp(AtomicRMWInst : LLVMOpaqueValue? implicit; BinOp : LLVMAtomicRMWBinOp) : void {}
[extern(cdecl, name="LLVMBuildTrunc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildTrunc(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildZExt", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildZExt(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSExt", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildSExt(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFPToUI", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildFPToUI(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFPToSI", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildFPToSI(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildUIToFP", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildUIToFP(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSIToFP", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildSIToFP(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFPTrunc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildFPTrunc(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFPExt", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildFPExt(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildPtrToInt", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildPtrToInt(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildIntToPtr", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildIntToPtr(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildBitCast", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildBitCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAddrSpaceCast", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildAddrSpaceCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildZExtOrBitCast", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildZExtOrBitCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSExtOrBitCast", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildSExtOrBitCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildTruncOrBitCast", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildTruncOrBitCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCast", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildCast(B : LLVMOpaqueBuilder? implicit; Op : LLVMOpcode; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildPointerCast", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildPointerCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildIntCast2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildIntCast2(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; IsSigned : int; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFPCast", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildFPCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildIntCast", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildIntCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetCastOpcode", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetCastOpcode(Src : LLVMOpaqueValue? implicit; SrcIsSigned : int; DestTy : LLVMOpaqueType? implicit; DestIsSigned : int) : LLVMOpcode {}
[extern(cdecl, name="LLVMBuildICmp", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildICmp(_argument_0x0 : LLVMOpaqueBuilder? implicit; Op : LLVMIntPredicate; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFCmp", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildFCmp(_argument_0x0 : LLVMOpaqueBuilder? implicit; Op : LLVMRealPredicate; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildPhi", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildPhi(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCall2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildCall2(_argument_0x0 : LLVMOpaqueBuilder? implicit; _argument_0x1 : LLVMOpaqueType? implicit; Fn : LLVMOpaqueValue? implicit; Args : LLVMOpaqueValue ?? implicit; NumArgs : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSelect", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildSelect(_argument_0x0 : LLVMOpaqueBuilder? implicit; If : LLVMOpaqueValue? implicit; Then : LLVMOpaqueValue? implicit; Else : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildVAArg", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildVAArg(_argument_0x0 : LLVMOpaqueBuilder? implicit; List : LLVMOpaqueValue? implicit; Ty : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildExtractElement", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildExtractElement(_argument_0x0 : LLVMOpaqueBuilder? implicit; VecVal : LLVMOpaqueValue? implicit; Index : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildInsertElement", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildInsertElement(_argument_0x0 : LLVMOpaqueBuilder? implicit; VecVal : LLVMOpaqueValue? implicit; EltVal : LLVMOpaqueValue? implicit; Index : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildShuffleVector", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildShuffleVector(_argument_0x0 : LLVMOpaqueBuilder? implicit; V1 : LLVMOpaqueValue? implicit; V2 : LLVMOpaqueValue? implicit; Mask : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildExtractValue", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildExtractValue(_argument_0x0 : LLVMOpaqueBuilder? implicit; AggVal : LLVMOpaqueValue? implicit; Index : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildInsertValue", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildInsertValue(_argument_0x0 : LLVMOpaqueBuilder? implicit; AggVal : LLVMOpaqueValue? implicit; EltVal : LLVMOpaqueValue? implicit; Index : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFreeze", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildFreeze(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildIsNull", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildIsNull(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildIsNotNull", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildIsNotNull(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildPtrDiff2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildPtrDiff2(_argument_0x0 : LLVMOpaqueBuilder? implicit; ElemTy : LLVMOpaqueType? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFence", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildFence(B : LLVMOpaqueBuilder? implicit; ordering : LLVMAtomicOrdering; singleThread : int; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAtomicRMW", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildAtomicRMW(B : LLVMOpaqueBuilder? implicit; op : LLVMAtomicRMWBinOp; PTR : LLVMOpaqueValue? implicit; Val : LLVMOpaqueValue? implicit; ordering : LLVMAtomicOrdering; singleThread : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAtomicCmpXchg", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBuildAtomicCmpXchg(B : LLVMOpaqueBuilder? implicit; Ptr : LLVMOpaqueValue? implicit; Cmp : LLVMOpaqueValue? implicit; New : LLVMOpaqueValue? implicit; SuccessOrdering : LLVMAtomicOrdering; FailureOrdering : LLVMAtomicOrdering; SingleThread : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNumMaskElements", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNumMaskElements(ShuffleVectorInst : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetUndefMaskElem", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetUndefMaskElem() : int {}
[extern(cdecl, name="LLVMGetMaskValue", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetMaskValue(ShuffleVectorInst : LLVMOpaqueValue? implicit; Elt : uint) : int {}
[extern(cdecl, name="LLVMIsAtomicSingleThread", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsAtomicSingleThread(AtomicInst : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetAtomicSingleThread", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetAtomicSingleThread(AtomicInst : LLVMOpaqueValue? implicit; SingleThread : int) : void {}
[extern(cdecl, name="LLVMGetCmpXchgSuccessOrdering", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetCmpXchgSuccessOrdering(CmpXchgInst : LLVMOpaqueValue? implicit) : LLVMAtomicOrdering {}
[extern(cdecl, name="LLVMSetCmpXchgSuccessOrdering", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetCmpXchgSuccessOrdering(CmpXchgInst : LLVMOpaqueValue? implicit; Ordering : LLVMAtomicOrdering) : void {}
[extern(cdecl, name="LLVMGetCmpXchgFailureOrdering", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetCmpXchgFailureOrdering(CmpXchgInst : LLVMOpaqueValue? implicit) : LLVMAtomicOrdering {}
[extern(cdecl, name="LLVMSetCmpXchgFailureOrdering", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetCmpXchgFailureOrdering(CmpXchgInst : LLVMOpaqueValue? implicit; Ordering : LLVMAtomicOrdering) : void {}
[extern(cdecl, name="LLVMCreateModuleProviderForExistingModule", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateModuleProviderForExistingModule(M : LLVMOpaqueModule? implicit) : LLVMOpaqueModuleProvider? {}
[extern(cdecl, name="LLVMDisposeModuleProvider", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeModuleProvider(M : LLVMOpaqueModuleProvider? implicit) : void {}
[extern(cdecl, name="LLVMCreateMemoryBufferWithContentsOfFile", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateMemoryBufferWithContentsOfFile(Path : string implicit; OutMemBuf : LLVMOpaqueMemoryBuffer ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMCreateMemoryBufferWithSTDIN", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateMemoryBufferWithSTDIN(OutMemBuf : LLVMOpaqueMemoryBuffer ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMCreateMemoryBufferWithMemoryRange", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateMemoryBufferWithMemoryRange(InputData : string implicit; InputDataLength : uint64; BufferName : string implicit; RequiresNullTerminator : int) : LLVMOpaqueMemoryBuffer? {}
[extern(cdecl, name="LLVMCreateMemoryBufferWithMemoryRangeCopy", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateMemoryBufferWithMemoryRangeCopy(InputData : string implicit; InputDataLength : uint64; BufferName : string implicit) : LLVMOpaqueMemoryBuffer? {}
[extern(cdecl, name="LLVMGetBufferStart", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetBufferStart(MemBuf : LLVMOpaqueMemoryBuffer? implicit) : string {}
[extern(cdecl, name="LLVMGetBufferSize", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetBufferSize(MemBuf : LLVMOpaqueMemoryBuffer? implicit) : uint64 {}
[extern(cdecl, name="LLVMDisposeMemoryBuffer", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeMemoryBuffer(MemBuf : LLVMOpaqueMemoryBuffer? implicit) : void {}
[extern(cdecl, name="LLVMGetGlobalPassRegistry", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetGlobalPassRegistry() : LLVMOpaquePassRegistry? {}
[extern(cdecl, name="LLVMCreatePassManager", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreatePassManager() : LLVMOpaquePassManager? {}
[extern(cdecl, name="LLVMCreateFunctionPassManagerForModule", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateFunctionPassManagerForModule(M : LLVMOpaqueModule? implicit) : LLVMOpaquePassManager? {}
[extern(cdecl, name="LLVMCreateFunctionPassManager", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateFunctionPassManager(MP : LLVMOpaqueModuleProvider? implicit) : LLVMOpaquePassManager? {}
[extern(cdecl, name="LLVMRunPassManager", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRunPassManager(PM : LLVMOpaquePassManager? implicit; M : LLVMOpaqueModule? implicit) : int {}
[extern(cdecl, name="LLVMInitializeFunctionPassManager", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeFunctionPassManager(FPM : LLVMOpaquePassManager? implicit) : int {}
[extern(cdecl, name="LLVMRunFunctionPassManager", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRunFunctionPassManager(FPM : LLVMOpaquePassManager? implicit; F : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMFinalizeFunctionPassManager", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMFinalizeFunctionPassManager(FPM : LLVMOpaquePassManager? implicit) : int {}
[extern(cdecl, name="LLVMDisposePassManager", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposePassManager(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMStartMultithreaded", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMStartMultithreaded() : int {}
[extern(cdecl, name="LLVMStopMultithreaded", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMStopMultithreaded() : void {}
[extern(cdecl, name="LLVMIsMultithreaded", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsMultithreaded() : int {}
[extern(cdecl, name="LLVMDebugMetadataVersion", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDebugMetadataVersion() : uint {}
[extern(cdecl, name="LLVMGetModuleDebugMetadataVersion", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetModuleDebugMetadataVersion(Module : LLVMOpaqueModule? implicit) : uint {}
[extern(cdecl, name="LLVMStripModuleDebugInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMStripModuleDebugInfo(Module : LLVMOpaqueModule? implicit) : int {}
[extern(cdecl, name="LLVMCreateDIBuilderDisallowUnresolved", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateDIBuilderDisallowUnresolved(M : LLVMOpaqueModule? implicit) : LLVMOpaqueDIBuilder? {}
[extern(cdecl, name="LLVMCreateDIBuilder", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateDIBuilder(M : LLVMOpaqueModule? implicit) : LLVMOpaqueDIBuilder? {}
[extern(cdecl, name="LLVMDisposeDIBuilder", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeDIBuilder(Builder : LLVMOpaqueDIBuilder? implicit) : void {}
[extern(cdecl, name="LLVMDIBuilderFinalize", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderFinalize(Builder : LLVMOpaqueDIBuilder? implicit) : void {}
[extern(cdecl, name="LLVMDIBuilderFinalizeSubprogram", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderFinalizeSubprogram(Builder : LLVMOpaqueDIBuilder? implicit; Subprogram : LLVMOpaqueMetadata? implicit) : void {}
// Function `LLVMDIBuilderCreateCompileUnit` has too many arguments. Ignore.

[extern(cdecl, name="LLVMDIBuilderCreateFile", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateFile(Builder : LLVMOpaqueDIBuilder? implicit; Filename : string implicit; FilenameLen : uint64; Directory : string implicit; DirectoryLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateModule", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateModule(Builder : LLVMOpaqueDIBuilder? implicit; ParentScope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; ConfigMacros : string implicit; ConfigMacrosLen : uint64; IncludePath : string implicit; IncludePathLen : uint64; APINotesFile : string implicit; APINotesFileLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateNameSpace", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateNameSpace(Builder : LLVMOpaqueDIBuilder? implicit; ParentScope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; ExportSymbols : int) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateFunction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateFunction(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; LinkageName : string implicit; LinkageNameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; Ty : LLVMOpaqueMetadata? implicit; IsLocalToUnit : int; IsDefinition : int; ScopeLine : uint; Flags : LLVMDIFlags; IsOptimized : int) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateLexicalBlock", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateLexicalBlock(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint; Column : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateLexicalBlockFile", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateLexicalBlockFile(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Discriminator : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateImportedModuleFromNamespace", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateImportedModuleFromNamespace(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; NS : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateImportedModuleFromAlias", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateImportedModuleFromAlias(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; ImportedEntity : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint; Elements : LLVMOpaqueMetadata ?? implicit; NumElements : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateImportedModuleFromModule", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateImportedModuleFromModule(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; M : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint; Elements : LLVMOpaqueMetadata ?? implicit; NumElements : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateImportedDeclaration", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateImportedDeclaration(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Decl : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint; Name : string implicit; NameLen : uint64; Elements : LLVMOpaqueMetadata ?? implicit; NumElements : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateDebugLocation", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateDebugLocation(Ctx : LLVMOpaqueContext? implicit; Line : uint; Column : uint; Scope : LLVMOpaqueMetadata? implicit; InlinedAt : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDILocationGetLine", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDILocationGetLine(Location : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMDILocationGetColumn", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDILocationGetColumn(Location : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMDILocationGetScope", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDILocationGetScope(Location : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDILocationGetInlinedAt", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDILocationGetInlinedAt(Location : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIScopeGetFile", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIScopeGetFile(Scope : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIFileGetDirectory", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIFileGetDirectory(File : LLVMOpaqueMetadata? implicit; Len : uint? implicit) : string {}
[extern(cdecl, name="LLVMDIFileGetFilename", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIFileGetFilename(File : LLVMOpaqueMetadata? implicit; Len : uint? implicit) : string {}
[extern(cdecl, name="LLVMDIFileGetSource", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIFileGetSource(File : LLVMOpaqueMetadata? implicit; Len : uint? implicit) : string {}
[extern(cdecl, name="LLVMDIBuilderGetOrCreateTypeArray", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderGetOrCreateTypeArray(Builder : LLVMOpaqueDIBuilder? implicit; Data : LLVMOpaqueMetadata ?? implicit; NumElements : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateSubroutineType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateSubroutineType(Builder : LLVMOpaqueDIBuilder? implicit; File : LLVMOpaqueMetadata? implicit; ParameterTypes : LLVMOpaqueMetadata ?? implicit; NumParameterTypes : uint; Flags : LLVMDIFlags) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateMacro", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateMacro(Builder : LLVMOpaqueDIBuilder? implicit; ParentMacroFile : LLVMOpaqueMetadata? implicit; Line : uint; RecordType : LLVMDWARFMacinfoRecordType; Name : string implicit; NameLen : uint64; Value : string implicit; ValueLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateTempMacroFile", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateTempMacroFile(Builder : LLVMOpaqueDIBuilder? implicit; ParentMacroFile : LLVMOpaqueMetadata? implicit; Line : uint; File : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateEnumerator", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateEnumerator(Builder : LLVMOpaqueDIBuilder? implicit; Name : string implicit; NameLen : uint64; Value : int64; IsUnsigned : int) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateEnumerationType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateEnumerationType(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNumber : uint; SizeInBits : uint64; AlignInBits : uint; Elements : LLVMOpaqueMetadata ?? implicit; NumElements : uint; ClassTy : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateUnionType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateUnionType(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNumber : uint; SizeInBits : uint64; AlignInBits : uint; Flags : LLVMDIFlags; Elements : LLVMOpaqueMetadata ?? implicit; NumElements : uint; RunTimeLang : uint; UniqueId : string implicit; UniqueIdLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateArrayType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateArrayType(Builder : LLVMOpaqueDIBuilder? implicit; Size : uint64; AlignInBits : uint; Ty : LLVMOpaqueMetadata? implicit; Subscripts : LLVMOpaqueMetadata ?? implicit; NumSubscripts : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateVectorType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateVectorType(Builder : LLVMOpaqueDIBuilder? implicit; Size : uint64; AlignInBits : uint; Ty : LLVMOpaqueMetadata? implicit; Subscripts : LLVMOpaqueMetadata ?? implicit; NumSubscripts : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateUnspecifiedType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateUnspecifiedType(Builder : LLVMOpaqueDIBuilder? implicit; Name : string implicit; NameLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateBasicType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateBasicType(Builder : LLVMOpaqueDIBuilder? implicit; Name : string implicit; NameLen : uint64; SizeInBits : uint64; Encoding : uint; Flags : LLVMDIFlags) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreatePointerType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreatePointerType(Builder : LLVMOpaqueDIBuilder? implicit; PointeeTy : LLVMOpaqueMetadata? implicit; SizeInBits : uint64; AlignInBits : uint; AddressSpace : uint; Name : string implicit; NameLen : uint64) : LLVMOpaqueMetadata? {}
// Function `LLVMDIBuilderCreateStructType` has too many arguments. Ignore.

[extern(cdecl, name="LLVMDIBuilderCreateMemberType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateMemberType(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; SizeInBits : uint64; AlignInBits : uint; OffsetInBits : uint64; Flags : LLVMDIFlags; Ty : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateStaticMemberType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateStaticMemberType(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNumber : uint; Type : LLVMOpaqueMetadata? implicit; Flags : LLVMDIFlags; ConstantVal : LLVMOpaqueValue? implicit; AlignInBits : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateMemberPointerType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateMemberPointerType(Builder : LLVMOpaqueDIBuilder? implicit; PointeeType : LLVMOpaqueMetadata? implicit; ClassType : LLVMOpaqueMetadata? implicit; SizeInBits : uint64; AlignInBits : uint; Flags : LLVMDIFlags) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateObjCIVar", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateObjCIVar(Builder : LLVMOpaqueDIBuilder? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; SizeInBits : uint64; AlignInBits : uint; OffsetInBits : uint64; Flags : LLVMDIFlags; Ty : LLVMOpaqueMetadata? implicit; PropertyNode : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateObjCProperty", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateObjCProperty(Builder : LLVMOpaqueDIBuilder? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; GetterName : string implicit; GetterNameLen : uint64; SetterName : string implicit; SetterNameLen : uint64; PropertyAttributes : uint; Ty : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateObjectPointerType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateObjectPointerType(Builder : LLVMOpaqueDIBuilder? implicit; Type : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateQualifiedType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateQualifiedType(Builder : LLVMOpaqueDIBuilder? implicit; Tag : uint; Type : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateReferenceType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateReferenceType(Builder : LLVMOpaqueDIBuilder? implicit; Tag : uint; Type : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateNullPtrType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateNullPtrType(Builder : LLVMOpaqueDIBuilder? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateTypedef", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateTypedef(Builder : LLVMOpaqueDIBuilder? implicit; Type : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; Scope : LLVMOpaqueMetadata? implicit; AlignInBits : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateInheritance", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateInheritance(Builder : LLVMOpaqueDIBuilder? implicit; Ty : LLVMOpaqueMetadata? implicit; BaseTy : LLVMOpaqueMetadata? implicit; BaseOffset : uint64; VBPtrOffset : uint; Flags : LLVMDIFlags) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateForwardDecl", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateForwardDecl(Builder : LLVMOpaqueDIBuilder? implicit; Tag : uint; Name : string implicit; NameLen : uint64; Scope : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint; RuntimeLang : uint; SizeInBits : uint64; AlignInBits : uint; UniqueIdentifier : string implicit; UniqueIdentifierLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateReplaceableCompositeType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateReplaceableCompositeType(Builder : LLVMOpaqueDIBuilder? implicit; Tag : uint; Name : string implicit; NameLen : uint64; Scope : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint; RuntimeLang : uint; SizeInBits : uint64; AlignInBits : uint; Flags : LLVMDIFlags; UniqueIdentifier : string implicit; UniqueIdentifierLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateBitFieldMemberType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateBitFieldMemberType(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNumber : uint; SizeInBits : uint64; OffsetInBits : uint64; StorageOffsetInBits : uint64; Flags : LLVMDIFlags; Type : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
// Function `LLVMDIBuilderCreateClassType` has too many arguments. Ignore.

[extern(cdecl, name="LLVMDIBuilderCreateArtificialType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateArtificialType(Builder : LLVMOpaqueDIBuilder? implicit; Type : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDITypeGetName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDITypeGetName(DType : LLVMOpaqueMetadata? implicit; Length : uint64? implicit) : string {}
[extern(cdecl, name="LLVMDITypeGetSizeInBits", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDITypeGetSizeInBits(DType : LLVMOpaqueMetadata? implicit) : uint64 {}
[extern(cdecl, name="LLVMDITypeGetOffsetInBits", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDITypeGetOffsetInBits(DType : LLVMOpaqueMetadata? implicit) : uint64 {}
[extern(cdecl, name="LLVMDITypeGetAlignInBits", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDITypeGetAlignInBits(DType : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMDITypeGetLine", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDITypeGetLine(DType : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMDITypeGetFlags", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDITypeGetFlags(DType : LLVMOpaqueMetadata? implicit) : LLVMDIFlags {}
[extern(cdecl, name="LLVMDIBuilderGetOrCreateSubrange", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderGetOrCreateSubrange(Builder : LLVMOpaqueDIBuilder? implicit; LowerBound : int64; Count : int64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderGetOrCreateArray", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderGetOrCreateArray(Builder : LLVMOpaqueDIBuilder? implicit; Data : LLVMOpaqueMetadata ?? implicit; NumElements : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateExpression", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateExpression(Builder : LLVMOpaqueDIBuilder? implicit; Addr : uint64? implicit; Length : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateConstantValueExpression", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateConstantValueExpression(Builder : LLVMOpaqueDIBuilder? implicit; Value : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateGlobalVariableExpression", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateGlobalVariableExpression(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; Linkage : string implicit; LinkLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; Ty : LLVMOpaqueMetadata? implicit; LocalToUnit : int; Expr : LLVMOpaqueMetadata? implicit; Decl : LLVMOpaqueMetadata? implicit; AlignInBits : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIGlobalVariableExpressionGetVariable", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIGlobalVariableExpressionGetVariable(GVE : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIGlobalVariableExpressionGetExpression", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIGlobalVariableExpressionGetExpression(GVE : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIVariableGetFile", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIVariableGetFile(Var : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIVariableGetScope", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIVariableGetScope(Var : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIVariableGetLine", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIVariableGetLine(Var : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMTemporaryMDNode", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMTemporaryMDNode(Ctx : LLVMOpaqueContext? implicit; Data : LLVMOpaqueMetadata ?? implicit; NumElements : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDisposeTemporaryMDNode", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeTemporaryMDNode(TempNode : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMMetadataReplaceAllUsesWith", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMMetadataReplaceAllUsesWith(TempTargetMetadata : LLVMOpaqueMetadata? implicit; Replacement : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMDIBuilderCreateTempGlobalVariableFwdDecl", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateTempGlobalVariableFwdDecl(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; Linkage : string implicit; LnkLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; Ty : LLVMOpaqueMetadata? implicit; LocalToUnit : int; Decl : LLVMOpaqueMetadata? implicit; AlignInBits : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderInsertDeclareBefore", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderInsertDeclareBefore(Builder : LLVMOpaqueDIBuilder? implicit; Storage : LLVMOpaqueValue? implicit; VarInfo : LLVMOpaqueMetadata? implicit; Expr : LLVMOpaqueMetadata? implicit; DebugLoc : LLVMOpaqueMetadata? implicit; Instr : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMDIBuilderInsertDeclareAtEnd", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderInsertDeclareAtEnd(Builder : LLVMOpaqueDIBuilder? implicit; Storage : LLVMOpaqueValue? implicit; VarInfo : LLVMOpaqueMetadata? implicit; Expr : LLVMOpaqueMetadata? implicit; DebugLoc : LLVMOpaqueMetadata? implicit; Block : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMDIBuilderInsertDbgValueBefore", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderInsertDbgValueBefore(Builder : LLVMOpaqueDIBuilder? implicit; Val : LLVMOpaqueValue? implicit; VarInfo : LLVMOpaqueMetadata? implicit; Expr : LLVMOpaqueMetadata? implicit; DebugLoc : LLVMOpaqueMetadata? implicit; Instr : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMDIBuilderInsertDbgValueAtEnd", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderInsertDbgValueAtEnd(Builder : LLVMOpaqueDIBuilder? implicit; Val : LLVMOpaqueValue? implicit; VarInfo : LLVMOpaqueMetadata? implicit; Expr : LLVMOpaqueMetadata? implicit; DebugLoc : LLVMOpaqueMetadata? implicit; Block : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMDIBuilderCreateAutoVariable", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateAutoVariable(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; Ty : LLVMOpaqueMetadata? implicit; AlwaysPreserve : int; Flags : LLVMDIFlags; AlignInBits : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateParameterVariable", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDIBuilderCreateParameterVariable(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; ArgNo : uint; File : LLVMOpaqueMetadata? implicit; LineNo : uint; Ty : LLVMOpaqueMetadata? implicit; AlwaysPreserve : int; Flags : LLVMDIFlags) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMGetSubprogram", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetSubprogram(Func : LLVMOpaqueValue? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMSetSubprogram", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetSubprogram(Func : LLVMOpaqueValue? implicit; SP : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMDISubprogramGetLine", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDISubprogramGetLine(Subprogram : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMInstructionGetDebugLoc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInstructionGetDebugLoc(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMInstructionSetDebugLoc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInstructionSetDebugLoc(Inst : LLVMOpaqueValue? implicit; Loc : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMGetMetadataKind", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetMetadataKind(Metadata : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMSetDisasmOptions", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetDisasmOptions(DC : void? implicit; Options : uint64) : int {}
[extern(cdecl, name="LLVMDisasmDispose", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisasmDispose(DC : void? implicit) : void {}
[extern(cdecl, name="LLVMDisasmInstruction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisasmInstruction(DC : void? implicit; Bytes : uint8? implicit; BytesSize : uint64; PC : uint64; OutString : string implicit; OutStringSize : uint64) : uint64 {}
[extern(cdecl, name="LLVMGetErrorTypeId", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetErrorTypeId(Err : LLVMOpaqueError? implicit) : void const? {}
[extern(cdecl, name="LLVMConsumeError", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMConsumeError(Err : LLVMOpaqueError? implicit) : void {}
[extern(cdecl, name="LLVMGetErrorMessage", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetErrorMessage(Err : LLVMOpaqueError? implicit) : string {}
[extern(cdecl, name="LLVMDisposeErrorMessage", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeErrorMessage(ErrMsg : string implicit) : void {}
[extern(cdecl, name="LLVMGetStringErrorTypeId", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetStringErrorTypeId() : void const? {}
[extern(cdecl, name="LLVMCreateStringError", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateStringError(ErrMsg : string implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMInitializeAArch64TargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeAArch64TargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeAMDGPUTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeAMDGPUTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeARMTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeARMTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeAVRTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeAVRTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeBPFTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeBPFTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeHexagonTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeHexagonTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeLanaiTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeLanaiTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeLoongArchTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeLoongArchTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeMipsTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeMipsTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeMSP430TargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeMSP430TargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeNVPTXTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeNVPTXTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializePowerPCTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializePowerPCTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeRISCVTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeRISCVTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeSparcTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeSparcTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeSystemZTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeSystemZTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeVETargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeVETargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeWebAssemblyTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeWebAssemblyTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeX86TargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeX86TargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeXCoreTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeXCoreTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeAArch64Target", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeAArch64Target() : void {}
[extern(cdecl, name="LLVMInitializeAMDGPUTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeAMDGPUTarget() : void {}
[extern(cdecl, name="LLVMInitializeARMTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeARMTarget() : void {}
[extern(cdecl, name="LLVMInitializeAVRTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeAVRTarget() : void {}
[extern(cdecl, name="LLVMInitializeBPFTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeBPFTarget() : void {}
[extern(cdecl, name="LLVMInitializeHexagonTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeHexagonTarget() : void {}
[extern(cdecl, name="LLVMInitializeLanaiTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeLanaiTarget() : void {}
[extern(cdecl, name="LLVMInitializeLoongArchTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeLoongArchTarget() : void {}
[extern(cdecl, name="LLVMInitializeMipsTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeMipsTarget() : void {}
[extern(cdecl, name="LLVMInitializeMSP430Target", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeMSP430Target() : void {}
[extern(cdecl, name="LLVMInitializeNVPTXTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeNVPTXTarget() : void {}
[extern(cdecl, name="LLVMInitializePowerPCTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializePowerPCTarget() : void {}
[extern(cdecl, name="LLVMInitializeRISCVTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeRISCVTarget() : void {}
[extern(cdecl, name="LLVMInitializeSparcTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeSparcTarget() : void {}
[extern(cdecl, name="LLVMInitializeSystemZTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeSystemZTarget() : void {}
[extern(cdecl, name="LLVMInitializeVETarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeVETarget() : void {}
[extern(cdecl, name="LLVMInitializeWebAssemblyTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeWebAssemblyTarget() : void {}
[extern(cdecl, name="LLVMInitializeX86Target", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeX86Target() : void {}
[extern(cdecl, name="LLVMInitializeXCoreTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeXCoreTarget() : void {}
[extern(cdecl, name="LLVMInitializeAArch64TargetMC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeAArch64TargetMC() : void {}
[extern(cdecl, name="LLVMInitializeAMDGPUTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeAMDGPUTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeARMTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeARMTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeAVRTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeAVRTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeBPFTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeBPFTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeHexagonTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeHexagonTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeLanaiTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeLanaiTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeLoongArchTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeLoongArchTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeMipsTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeMipsTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeMSP430TargetMC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeMSP430TargetMC() : void {}
[extern(cdecl, name="LLVMInitializeNVPTXTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeNVPTXTargetMC() : void {}
[extern(cdecl, name="LLVMInitializePowerPCTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializePowerPCTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeRISCVTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeRISCVTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeSparcTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeSparcTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeSystemZTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeSystemZTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeVETargetMC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeVETargetMC() : void {}
[extern(cdecl, name="LLVMInitializeWebAssemblyTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeWebAssemblyTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeX86TargetMC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeX86TargetMC() : void {}
[extern(cdecl, name="LLVMInitializeXCoreTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeXCoreTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeAArch64AsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeAArch64AsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeAMDGPUAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeAMDGPUAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeARMAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeARMAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeAVRAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeAVRAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeBPFAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeBPFAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeHexagonAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeHexagonAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeLanaiAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeLanaiAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeLoongArchAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeLoongArchAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeMipsAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeMipsAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeMSP430AsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeMSP430AsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeNVPTXAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeNVPTXAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializePowerPCAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializePowerPCAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeRISCVAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeRISCVAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeSparcAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeSparcAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeSystemZAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeSystemZAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeVEAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeVEAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeWebAssemblyAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeWebAssemblyAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeX86AsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeX86AsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeXCoreAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeXCoreAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeAArch64AsmParser", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeAArch64AsmParser() : void {}
[extern(cdecl, name="LLVMInitializeAMDGPUAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeAMDGPUAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeARMAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeARMAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeAVRAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeAVRAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeBPFAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeBPFAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeHexagonAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeHexagonAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeLanaiAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeLanaiAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeLoongArchAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeLoongArchAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeMipsAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeMipsAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeMSP430AsmParser", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeMSP430AsmParser() : void {}
[extern(cdecl, name="LLVMInitializePowerPCAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializePowerPCAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeRISCVAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeRISCVAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeSparcAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeSparcAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeSystemZAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeSystemZAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeVEAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeVEAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeWebAssemblyAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeWebAssemblyAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeX86AsmParser", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeX86AsmParser() : void {}
[extern(cdecl, name="LLVMInitializeAArch64Disassembler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeAArch64Disassembler() : void {}
[extern(cdecl, name="LLVMInitializeAMDGPUDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeAMDGPUDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeARMDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeARMDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeAVRDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeAVRDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeBPFDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeBPFDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeHexagonDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeHexagonDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeLanaiDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeLanaiDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeLoongArchDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeLoongArchDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeMipsDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeMipsDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeMSP430Disassembler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeMSP430Disassembler() : void {}
[extern(cdecl, name="LLVMInitializePowerPCDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializePowerPCDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeRISCVDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeRISCVDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeSparcDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeSparcDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeSystemZDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeSystemZDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeVEDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeVEDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeWebAssemblyDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeWebAssemblyDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeX86Disassembler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeX86Disassembler() : void {}
[extern(cdecl, name="LLVMInitializeXCoreDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeXCoreDisassembler() : void {}
[extern(cdecl, name="LLVMGetModuleDataLayout", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetModuleDataLayout(M : LLVMOpaqueModule? implicit) : LLVMOpaqueTargetData? {}
[extern(cdecl, name="LLVMSetModuleDataLayout", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetModuleDataLayout(M : LLVMOpaqueModule? implicit; DL : LLVMOpaqueTargetData? implicit) : void {}
[extern(cdecl, name="LLVMCreateTargetData", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateTargetData(StringRep : string implicit) : LLVMOpaqueTargetData? {}
[extern(cdecl, name="LLVMDisposeTargetData", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeTargetData(TD : LLVMOpaqueTargetData? implicit) : void {}
[extern(cdecl, name="LLVMAddTargetLibraryInfo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddTargetLibraryInfo(TLI : LLVMOpaqueTargetLibraryInfotData? implicit; PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMCopyStringRepOfTargetData", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCopyStringRepOfTargetData(TD : LLVMOpaqueTargetData? implicit) : string {}
[extern(cdecl, name="LLVMByteOrder", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMByteOrder(TD : LLVMOpaqueTargetData? implicit) : LLVMByteOrdering {}
[extern(cdecl, name="LLVMPointerSize", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPointerSize(TD : LLVMOpaqueTargetData? implicit) : uint {}
[extern(cdecl, name="LLVMPointerSizeForAS", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPointerSizeForAS(TD : LLVMOpaqueTargetData? implicit; AS : uint) : uint {}
[extern(cdecl, name="LLVMIntPtrType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIntPtrType(TD : LLVMOpaqueTargetData? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIntPtrTypeForAS", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIntPtrTypeForAS(TD : LLVMOpaqueTargetData? implicit; AS : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIntPtrTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIntPtrTypeInContext(C : LLVMOpaqueContext? implicit; TD : LLVMOpaqueTargetData? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIntPtrTypeForASInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIntPtrTypeForASInContext(C : LLVMOpaqueContext? implicit; TD : LLVMOpaqueTargetData? implicit; AS : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMSizeOfTypeInBits", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSizeOfTypeInBits(TD : LLVMOpaqueTargetData? implicit; Ty : LLVMOpaqueType? implicit) : uint64 {}
[extern(cdecl, name="LLVMStoreSizeOfType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMStoreSizeOfType(TD : LLVMOpaqueTargetData? implicit; Ty : LLVMOpaqueType? implicit) : uint64 {}
[extern(cdecl, name="LLVMABISizeOfType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMABISizeOfType(TD : LLVMOpaqueTargetData? implicit; Ty : LLVMOpaqueType? implicit) : uint64 {}
[extern(cdecl, name="LLVMABIAlignmentOfType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMABIAlignmentOfType(TD : LLVMOpaqueTargetData? implicit; Ty : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMCallFrameAlignmentOfType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCallFrameAlignmentOfType(TD : LLVMOpaqueTargetData? implicit; Ty : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMPreferredAlignmentOfType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPreferredAlignmentOfType(TD : LLVMOpaqueTargetData? implicit; Ty : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMPreferredAlignmentOfGlobal", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPreferredAlignmentOfGlobal(TD : LLVMOpaqueTargetData? implicit; GlobalVar : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMElementAtOffset", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMElementAtOffset(TD : LLVMOpaqueTargetData? implicit; StructTy : LLVMOpaqueType? implicit; Offset : uint64) : uint {}
[extern(cdecl, name="LLVMOffsetOfElement", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOffsetOfElement(TD : LLVMOpaqueTargetData? implicit; StructTy : LLVMOpaqueType? implicit; Element : uint) : uint64 {}
[extern(cdecl, name="LLVMGetFirstTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetFirstTarget() : LLVMTarget? {}
[extern(cdecl, name="LLVMGetNextTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetNextTarget(T : LLVMTarget? implicit) : LLVMTarget? {}
[extern(cdecl, name="LLVMGetTargetFromName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetTargetFromName(Name : string implicit) : LLVMTarget? {}
[extern(cdecl, name="LLVMGetTargetFromTriple", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetTargetFromTriple(Triple : string implicit; T : LLVMTarget ?? implicit; ErrorMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMGetTargetName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetTargetName(T : LLVMTarget? implicit) : string {}
[extern(cdecl, name="LLVMGetTargetDescription", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetTargetDescription(T : LLVMTarget? implicit) : string {}
[extern(cdecl, name="LLVMTargetHasJIT", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMTargetHasJIT(T : LLVMTarget? implicit) : int {}
[extern(cdecl, name="LLVMTargetHasTargetMachine", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMTargetHasTargetMachine(T : LLVMTarget? implicit) : int {}
[extern(cdecl, name="LLVMTargetHasAsmBackend", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMTargetHasAsmBackend(T : LLVMTarget? implicit) : int {}
[extern(cdecl, name="LLVMCreateTargetMachine", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateTargetMachine(T : LLVMTarget? implicit; Triple : string implicit; CPU : string implicit; Features : string implicit; Level : LLVMCodeGenOptLevel; Reloc : LLVMRelocMode; CodeModel : LLVMCodeModel) : LLVMOpaqueTargetMachine? {}
[extern(cdecl, name="LLVMDisposeTargetMachine", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeTargetMachine(T : LLVMOpaqueTargetMachine? implicit) : void {}
[extern(cdecl, name="LLVMGetTargetMachineTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetTargetMachineTarget(T : LLVMOpaqueTargetMachine? implicit) : LLVMTarget? {}
[extern(cdecl, name="LLVMGetTargetMachineTriple", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetTargetMachineTriple(T : LLVMOpaqueTargetMachine? implicit) : string {}
[extern(cdecl, name="LLVMGetTargetMachineCPU", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetTargetMachineCPU(T : LLVMOpaqueTargetMachine? implicit) : string {}
[extern(cdecl, name="LLVMGetTargetMachineFeatureString", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetTargetMachineFeatureString(T : LLVMOpaqueTargetMachine? implicit) : string {}
[extern(cdecl, name="LLVMCreateTargetDataLayout", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateTargetDataLayout(T : LLVMOpaqueTargetMachine? implicit) : LLVMOpaqueTargetData? {}
[extern(cdecl, name="LLVMSetTargetMachineAsmVerbosity", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSetTargetMachineAsmVerbosity(T : LLVMOpaqueTargetMachine? implicit; VerboseAsm : int) : void {}
[extern(cdecl, name="LLVMTargetMachineEmitToFile", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMTargetMachineEmitToFile(T : LLVMOpaqueTargetMachine? implicit; M : LLVMOpaqueModule? implicit; Filename : string implicit; codegen : LLVMCodeGenFileType; ErrorMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMTargetMachineEmitToMemoryBuffer", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMTargetMachineEmitToMemoryBuffer(T : LLVMOpaqueTargetMachine? implicit; M : LLVMOpaqueModule? implicit; codegen : LLVMCodeGenFileType; ErrorMessage : string? implicit; OutMemBuf : LLVMOpaqueMemoryBuffer ?? implicit) : int {}
[extern(cdecl, name="LLVMGetDefaultTargetTriple", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetDefaultTargetTriple() : string {}
[extern(cdecl, name="LLVMNormalizeTargetTriple", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMNormalizeTargetTriple(triple : string implicit) : string {}
[extern(cdecl, name="LLVMGetHostCPUName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetHostCPUName() : string {}
[extern(cdecl, name="LLVMGetHostCPUFeatures", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetHostCPUFeatures() : string {}
[extern(cdecl, name="LLVMAddAnalysisPasses", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddAnalysisPasses(T : LLVMOpaqueTargetMachine? implicit; PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMLinkInMCJIT", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMLinkInMCJIT() : void {}
[extern(cdecl, name="LLVMLinkInInterpreter", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMLinkInInterpreter() : void {}
[extern(cdecl, name="LLVMCreateGenericValueOfInt", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateGenericValueOfInt(Ty : LLVMOpaqueType? implicit; N : uint64; IsSigned : int) : LLVMOpaqueGenericValue? {}
[extern(cdecl, name="LLVMCreateGenericValueOfPointer", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateGenericValueOfPointer(P : void? implicit) : LLVMOpaqueGenericValue? {}
[extern(cdecl, name="LLVMCreateGenericValueOfFloat", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateGenericValueOfFloat(Ty : LLVMOpaqueType? implicit; N : double) : LLVMOpaqueGenericValue? {}
[extern(cdecl, name="LLVMGenericValueIntWidth", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGenericValueIntWidth(GenValRef : LLVMOpaqueGenericValue? implicit) : uint {}
[extern(cdecl, name="LLVMGenericValueToInt", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGenericValueToInt(GenVal : LLVMOpaqueGenericValue? implicit; IsSigned : int) : uint64 {}
[extern(cdecl, name="LLVMGenericValueToPointer", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGenericValueToPointer(GenVal : LLVMOpaqueGenericValue? implicit) : void? {}
[extern(cdecl, name="LLVMGenericValueToFloat", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGenericValueToFloat(TyRef : LLVMOpaqueType? implicit; GenVal : LLVMOpaqueGenericValue? implicit) : double {}
[extern(cdecl, name="LLVMDisposeGenericValue", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeGenericValue(GenVal : LLVMOpaqueGenericValue? implicit) : void {}
[extern(cdecl, name="LLVMCreateExecutionEngineForModule", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateExecutionEngineForModule(OutEE : LLVMOpaqueExecutionEngine ?? implicit; M : LLVMOpaqueModule? implicit; OutError : string? implicit) : int {}
[extern(cdecl, name="LLVMCreateInterpreterForModule", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateInterpreterForModule(OutInterp : LLVMOpaqueExecutionEngine ?? implicit; M : LLVMOpaqueModule? implicit; OutError : string? implicit) : int {}
[extern(cdecl, name="LLVMCreateJITCompilerForModule", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateJITCompilerForModule(OutJIT : LLVMOpaqueExecutionEngine ?? implicit; M : LLVMOpaqueModule? implicit; OptLevel : uint; OutError : string? implicit) : int {}
[extern(cdecl, name="LLVMInitializeMCJITCompilerOptions", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeMCJITCompilerOptions(Options : LLVMMCJITCompilerOptions? implicit; SizeOfOptions : uint64) : void {}
[extern(cdecl, name="LLVMCreateMCJITCompilerForModule", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateMCJITCompilerForModule(OutJIT : LLVMOpaqueExecutionEngine ?? implicit; M : LLVMOpaqueModule? implicit; Options : LLVMMCJITCompilerOptions? implicit; SizeOfOptions : uint64; OutError : string? implicit) : int {}
[extern(cdecl, name="LLVMDisposeExecutionEngine", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeExecutionEngine(EE : LLVMOpaqueExecutionEngine? implicit) : void {}
[extern(cdecl, name="LLVMRunStaticConstructors", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRunStaticConstructors(EE : LLVMOpaqueExecutionEngine? implicit) : void {}
[extern(cdecl, name="LLVMRunStaticDestructors", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRunStaticDestructors(EE : LLVMOpaqueExecutionEngine? implicit) : void {}
[extern(cdecl, name="LLVMRunFunctionAsMain", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRunFunctionAsMain(EE : LLVMOpaqueExecutionEngine? implicit; F : LLVMOpaqueValue? implicit; ArgC : uint; ArgV : string const? implicit; EnvP : string const? implicit) : int {}
[extern(cdecl, name="LLVMRunFunction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRunFunction(EE : LLVMOpaqueExecutionEngine? implicit; F : LLVMOpaqueValue? implicit; NumArgs : uint; Args : LLVMOpaqueGenericValue ?? implicit) : LLVMOpaqueGenericValue? {}
[extern(cdecl, name="LLVMFreeMachineCodeForFunction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMFreeMachineCodeForFunction(EE : LLVMOpaqueExecutionEngine? implicit; F : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMAddModule", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddModule(EE : LLVMOpaqueExecutionEngine? implicit; M : LLVMOpaqueModule? implicit) : void {}
[extern(cdecl, name="LLVMRemoveModule", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemoveModule(EE : LLVMOpaqueExecutionEngine? implicit; M : LLVMOpaqueModule? implicit; OutMod : LLVMOpaqueModule ?? implicit; OutError : string? implicit) : int {}
[extern(cdecl, name="LLVMFindFunction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMFindFunction(EE : LLVMOpaqueExecutionEngine? implicit; Name : string implicit; OutFn : LLVMOpaqueValue ?? implicit) : int {}
[extern(cdecl, name="LLVMRecompileAndRelinkFunction", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRecompileAndRelinkFunction(EE : LLVMOpaqueExecutionEngine? implicit; Fn : LLVMOpaqueValue? implicit) : void? {}
[extern(cdecl, name="LLVMGetExecutionEngineTargetData", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetExecutionEngineTargetData(EE : LLVMOpaqueExecutionEngine? implicit) : LLVMOpaqueTargetData? {}
[extern(cdecl, name="LLVMGetExecutionEngineTargetMachine", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetExecutionEngineTargetMachine(EE : LLVMOpaqueExecutionEngine? implicit) : LLVMOpaqueTargetMachine? {}
[extern(cdecl, name="LLVMAddGlobalMapping", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddGlobalMapping(EE : LLVMOpaqueExecutionEngine? implicit; Global : LLVMOpaqueValue? implicit; Addr : void? implicit) : void {}
[extern(cdecl, name="LLVMGetPointerToGlobal", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetPointerToGlobal(EE : LLVMOpaqueExecutionEngine? implicit; Global : LLVMOpaqueValue? implicit) : void? {}
[extern(cdecl, name="LLVMGetGlobalValueAddress", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetGlobalValueAddress(EE : LLVMOpaqueExecutionEngine? implicit; Name : string implicit) : uint64 {}
[extern(cdecl, name="LLVMGetFunctionAddress", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetFunctionAddress(EE : LLVMOpaqueExecutionEngine? implicit; Name : string implicit) : uint64 {}
[extern(cdecl, name="LLVMExecutionEngineGetErrMsg", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMExecutionEngineGetErrMsg(EE : LLVMOpaqueExecutionEngine? implicit; OutError : string? implicit) : int {}
[extern(cdecl, name="LLVMDisposeMCJITMemoryManager", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeMCJITMemoryManager(MM : LLVMOpaqueMCJITMemoryManager? implicit) : void {}
[extern(cdecl, name="LLVMCreateGDBRegistrationListener", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateGDBRegistrationListener() : LLVMOpaqueJITEventListener? {}
[extern(cdecl, name="LLVMCreateIntelJITEventListener", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateIntelJITEventListener() : LLVMOpaqueJITEventListener? {}
[extern(cdecl, name="LLVMCreateOProfileJITEventListener", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateOProfileJITEventListener() : LLVMOpaqueJITEventListener? {}
[extern(cdecl, name="LLVMCreatePerfJITEventListener", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreatePerfJITEventListener() : LLVMOpaqueJITEventListener? {}
[extern(cdecl, name="LLVMInitializeTransformUtils", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeTransformUtils(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeScalarOpts", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeScalarOpts(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeVectorization", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeVectorization(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeInstCombine", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeInstCombine(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeIPO", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeIPO(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeAnalysis", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeAnalysis(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeIPA", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeIPA(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeCodeGen", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeCodeGen(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeTarget", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMInitializeTarget(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMParseIRInContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMParseIRInContext(ContextRef : LLVMOpaqueContext? implicit; MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutM : LLVMOpaqueModule ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMLinkModules2", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMLinkModules2(Dest : LLVMOpaqueModule? implicit; Src : LLVMOpaqueModule? implicit) : int {}
[extern(cdecl, name="LLVMOrcExecutionSessionGetSymbolStringPool", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcExecutionSessionGetSymbolStringPool(ES : LLVMOrcOpaqueExecutionSession? implicit) : LLVMOrcOpaqueSymbolStringPool? {}
[extern(cdecl, name="LLVMOrcSymbolStringPoolClearDeadEntries", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcSymbolStringPoolClearDeadEntries(SSP : LLVMOrcOpaqueSymbolStringPool? implicit) : void {}
[extern(cdecl, name="LLVMOrcExecutionSessionIntern", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcExecutionSessionIntern(ES : LLVMOrcOpaqueExecutionSession? implicit; Name : string implicit) : LLVMOrcOpaqueSymbolStringPoolEntry? {}
[extern(cdecl, name="LLVMOrcRetainSymbolStringPoolEntry", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcRetainSymbolStringPoolEntry(S : LLVMOrcOpaqueSymbolStringPoolEntry? implicit) : void {}
[extern(cdecl, name="LLVMOrcReleaseSymbolStringPoolEntry", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcReleaseSymbolStringPoolEntry(S : LLVMOrcOpaqueSymbolStringPoolEntry? implicit) : void {}
[extern(cdecl, name="LLVMOrcSymbolStringPoolEntryStr", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcSymbolStringPoolEntryStr(S : LLVMOrcOpaqueSymbolStringPoolEntry? implicit) : string {}
[extern(cdecl, name="LLVMOrcReleaseResourceTracker", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcReleaseResourceTracker(RT : LLVMOrcOpaqueResourceTracker? implicit) : void {}
[extern(cdecl, name="LLVMOrcResourceTrackerTransferTo", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcResourceTrackerTransferTo(SrcRT : LLVMOrcOpaqueResourceTracker? implicit; DstRT : LLVMOrcOpaqueResourceTracker? implicit) : void {}
[extern(cdecl, name="LLVMOrcResourceTrackerRemove", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcResourceTrackerRemove(RT : LLVMOrcOpaqueResourceTracker? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcDisposeDefinitionGenerator", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcDisposeDefinitionGenerator(DG : LLVMOrcOpaqueDefinitionGenerator? implicit) : void {}
[extern(cdecl, name="LLVMOrcDisposeMaterializationUnit", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcDisposeMaterializationUnit(MU : LLVMOrcOpaqueMaterializationUnit? implicit) : void {}
[extern(cdecl, name="LLVMOrcAbsoluteSymbols", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcAbsoluteSymbols(Syms : LLVMOrcCSymbolMapPair? implicit; NumPairs : uint64) : LLVMOrcOpaqueMaterializationUnit? {}
[extern(cdecl, name="LLVMOrcLazyReexports", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcLazyReexports(LCTM : LLVMOrcOpaqueLazyCallThroughManager? implicit; ISM : LLVMOrcOpaqueIndirectStubsManager? implicit; SourceRef : LLVMOrcOpaqueJITDylib? implicit; CallableAliases : LLVMOrcCSymbolAliasMapPair? implicit; NumPairs : uint64) : LLVMOrcOpaqueMaterializationUnit? {}
[extern(cdecl, name="LLVMOrcDisposeMaterializationResponsibility", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcDisposeMaterializationResponsibility(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit) : void {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityGetTargetDylib", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcMaterializationResponsibilityGetTargetDylib(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit) : LLVMOrcOpaqueJITDylib? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityGetExecutionSession", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcMaterializationResponsibilityGetExecutionSession(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit) : LLVMOrcOpaqueExecutionSession? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityGetSymbols", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcMaterializationResponsibilityGetSymbols(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; NumPairs : uint64? implicit) : LLVMOrcCSymbolFlagsMapPair? {}
[extern(cdecl, name="LLVMOrcDisposeCSymbolFlagsMap", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcDisposeCSymbolFlagsMap(Pairs : LLVMOrcCSymbolFlagsMapPair? implicit) : void {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityGetInitializerSymbol", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcMaterializationResponsibilityGetInitializerSymbol(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit) : LLVMOrcOpaqueSymbolStringPoolEntry? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityGetRequestedSymbols", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcMaterializationResponsibilityGetRequestedSymbols(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; NumSymbols : uint64? implicit) : LLVMOrcOpaqueSymbolStringPoolEntry ?? {}
[extern(cdecl, name="LLVMOrcDisposeSymbols", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcDisposeSymbols(Symbols : LLVMOrcOpaqueSymbolStringPoolEntry ?? implicit) : void {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityNotifyResolved", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcMaterializationResponsibilityNotifyResolved(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; Symbols : LLVMOrcCSymbolMapPair? implicit; NumPairs : uint64) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityNotifyEmitted", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcMaterializationResponsibilityNotifyEmitted(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityDefineMaterializing", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcMaterializationResponsibilityDefineMaterializing(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; Pairs : LLVMOrcCSymbolFlagsMapPair? implicit; NumPairs : uint64) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityFailMaterialization", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcMaterializationResponsibilityFailMaterialization(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit) : void {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityReplace", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcMaterializationResponsibilityReplace(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; MU : LLVMOrcOpaqueMaterializationUnit? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityDelegate", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcMaterializationResponsibilityDelegate(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; Symbols : LLVMOrcOpaqueSymbolStringPoolEntry ?? implicit; NumSymbols : uint64; Result : LLVMOrcOpaqueMaterializationResponsibility ?? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityAddDependencies", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcMaterializationResponsibilityAddDependencies(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; Name : LLVMOrcOpaqueSymbolStringPoolEntry? implicit; Dependencies : LLVMOrcCDependenceMapPair? implicit; NumPairs : uint64) : void {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityAddDependenciesForAll", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcMaterializationResponsibilityAddDependenciesForAll(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; Dependencies : LLVMOrcCDependenceMapPair? implicit; NumPairs : uint64) : void {}
[extern(cdecl, name="LLVMOrcExecutionSessionCreateBareJITDylib", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcExecutionSessionCreateBareJITDylib(ES : LLVMOrcOpaqueExecutionSession? implicit; Name : string implicit) : LLVMOrcOpaqueJITDylib? {}
[extern(cdecl, name="LLVMOrcExecutionSessionCreateJITDylib", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcExecutionSessionCreateJITDylib(ES : LLVMOrcOpaqueExecutionSession? implicit; Result : LLVMOrcOpaqueJITDylib ?? implicit; Name : string implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcExecutionSessionGetJITDylibByName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcExecutionSessionGetJITDylibByName(ES : LLVMOrcOpaqueExecutionSession? implicit; Name : string implicit) : LLVMOrcOpaqueJITDylib? {}
[extern(cdecl, name="LLVMOrcJITDylibCreateResourceTracker", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcJITDylibCreateResourceTracker(JD : LLVMOrcOpaqueJITDylib? implicit) : LLVMOrcOpaqueResourceTracker? {}
[extern(cdecl, name="LLVMOrcJITDylibGetDefaultResourceTracker", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcJITDylibGetDefaultResourceTracker(JD : LLVMOrcOpaqueJITDylib? implicit) : LLVMOrcOpaqueResourceTracker? {}
[extern(cdecl, name="LLVMOrcJITDylibDefine", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcJITDylibDefine(JD : LLVMOrcOpaqueJITDylib? implicit; MU : LLVMOrcOpaqueMaterializationUnit? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcJITDylibClear", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcJITDylibClear(JD : LLVMOrcOpaqueJITDylib? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcJITDylibAddGenerator", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcJITDylibAddGenerator(JD : LLVMOrcOpaqueJITDylib? implicit; DG : LLVMOrcOpaqueDefinitionGenerator? implicit) : void {}
[extern(cdecl, name="LLVMOrcLookupStateContinueLookup", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcLookupStateContinueLookup(S : LLVMOrcOpaqueLookupState? implicit; Err : LLVMOpaqueError? implicit) : void {}
[extern(cdecl, name="LLVMOrcCreateStaticLibrarySearchGeneratorForPath", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcCreateStaticLibrarySearchGeneratorForPath(Result : LLVMOrcOpaqueDefinitionGenerator ?? implicit; ObjLayer : LLVMOrcOpaqueObjectLayer? implicit; FileName : string implicit; TargetTriple : string implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcCreateNewThreadSafeContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcCreateNewThreadSafeContext() : LLVMOrcOpaqueThreadSafeContext? {}
[extern(cdecl, name="LLVMOrcThreadSafeContextGetContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcThreadSafeContextGetContext(TSCtx : LLVMOrcOpaqueThreadSafeContext? implicit) : LLVMOpaqueContext? {}
[extern(cdecl, name="LLVMOrcDisposeThreadSafeContext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcDisposeThreadSafeContext(TSCtx : LLVMOrcOpaqueThreadSafeContext? implicit) : void {}
[extern(cdecl, name="LLVMOrcCreateNewThreadSafeModule", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcCreateNewThreadSafeModule(M : LLVMOpaqueModule? implicit; TSCtx : LLVMOrcOpaqueThreadSafeContext? implicit) : LLVMOrcOpaqueThreadSafeModule? {}
[extern(cdecl, name="LLVMOrcDisposeThreadSafeModule", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcDisposeThreadSafeModule(TSM : LLVMOrcOpaqueThreadSafeModule? implicit) : void {}
[extern(cdecl, name="LLVMOrcJITTargetMachineBuilderDetectHost", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcJITTargetMachineBuilderDetectHost(Result : LLVMOrcOpaqueJITTargetMachineBuilder ?? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcJITTargetMachineBuilderCreateFromTargetMachine", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcJITTargetMachineBuilderCreateFromTargetMachine(TM : LLVMOpaqueTargetMachine? implicit) : LLVMOrcOpaqueJITTargetMachineBuilder? {}
[extern(cdecl, name="LLVMOrcDisposeJITTargetMachineBuilder", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcDisposeJITTargetMachineBuilder(JTMB : LLVMOrcOpaqueJITTargetMachineBuilder? implicit) : void {}
[extern(cdecl, name="LLVMOrcJITTargetMachineBuilderGetTargetTriple", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcJITTargetMachineBuilderGetTargetTriple(JTMB : LLVMOrcOpaqueJITTargetMachineBuilder? implicit) : string {}
[extern(cdecl, name="LLVMOrcJITTargetMachineBuilderSetTargetTriple", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcJITTargetMachineBuilderSetTargetTriple(JTMB : LLVMOrcOpaqueJITTargetMachineBuilder? implicit; TargetTriple : string implicit) : void {}
[extern(cdecl, name="LLVMOrcObjectLayerAddObjectFile", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcObjectLayerAddObjectFile(ObjLayer : LLVMOrcOpaqueObjectLayer? implicit; JD : LLVMOrcOpaqueJITDylib? implicit; ObjBuffer : LLVMOpaqueMemoryBuffer? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcObjectLayerAddObjectFileWithRT", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcObjectLayerAddObjectFileWithRT(ObjLayer : LLVMOrcOpaqueObjectLayer? implicit; RT : LLVMOrcOpaqueResourceTracker? implicit; ObjBuffer : LLVMOpaqueMemoryBuffer? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcObjectLayerEmit", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcObjectLayerEmit(ObjLayer : LLVMOrcOpaqueObjectLayer? implicit; R : LLVMOrcOpaqueMaterializationResponsibility? implicit; ObjBuffer : LLVMOpaqueMemoryBuffer? implicit) : void {}
[extern(cdecl, name="LLVMOrcDisposeObjectLayer", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcDisposeObjectLayer(ObjLayer : LLVMOrcOpaqueObjectLayer? implicit) : void {}
[extern(cdecl, name="LLVMOrcIRTransformLayerEmit", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcIRTransformLayerEmit(IRTransformLayer : LLVMOrcOpaqueIRTransformLayer? implicit; MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; TSM : LLVMOrcOpaqueThreadSafeModule? implicit) : void {}
[extern(cdecl, name="LLVMOrcCreateLocalIndirectStubsManager", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcCreateLocalIndirectStubsManager(TargetTriple : string implicit) : LLVMOrcOpaqueIndirectStubsManager? {}
[extern(cdecl, name="LLVMOrcDisposeIndirectStubsManager", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcDisposeIndirectStubsManager(ISM : LLVMOrcOpaqueIndirectStubsManager? implicit) : void {}
[extern(cdecl, name="LLVMOrcCreateLocalLazyCallThroughManager", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcCreateLocalLazyCallThroughManager(TargetTriple : string implicit; ES : LLVMOrcOpaqueExecutionSession? implicit; ErrorHandlerAddr : uint64; LCTM : LLVMOrcOpaqueLazyCallThroughManager ?? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcDisposeLazyCallThroughManager", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcDisposeLazyCallThroughManager(LCTM : LLVMOrcOpaqueLazyCallThroughManager? implicit) : void {}
[extern(cdecl, name="LLVMOrcCreateDumpObjects", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcCreateDumpObjects(DumpDir : string implicit; IdentifierOverride : string implicit) : LLVMOrcOpaqueDumpObjects? {}
[extern(cdecl, name="LLVMOrcDisposeDumpObjects", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcDisposeDumpObjects(DumpObjects : LLVMOrcOpaqueDumpObjects? implicit) : void {}
[extern(cdecl, name="LLVMOrcDumpObjects_CallOperator", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcDumpObjects_CallOperator(DumpObjects : LLVMOrcOpaqueDumpObjects? implicit; ObjBuffer : LLVMOpaqueMemoryBuffer ?? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcCreateLLJITBuilder", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcCreateLLJITBuilder() : LLVMOrcOpaqueLLJITBuilder? {}
[extern(cdecl, name="LLVMOrcDisposeLLJITBuilder", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcDisposeLLJITBuilder(Builder : LLVMOrcOpaqueLLJITBuilder? implicit) : void {}
[extern(cdecl, name="LLVMOrcLLJITBuilderSetJITTargetMachineBuilder", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcLLJITBuilderSetJITTargetMachineBuilder(Builder : LLVMOrcOpaqueLLJITBuilder? implicit; JTMB : LLVMOrcOpaqueJITTargetMachineBuilder? implicit) : void {}
[extern(cdecl, name="LLVMOrcCreateLLJIT", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcCreateLLJIT(Result : LLVMOrcOpaqueLLJIT ?? implicit; Builder : LLVMOrcOpaqueLLJITBuilder? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcDisposeLLJIT", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcDisposeLLJIT(J : LLVMOrcOpaqueLLJIT? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcLLJITGetExecutionSession", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcLLJITGetExecutionSession(J : LLVMOrcOpaqueLLJIT? implicit) : LLVMOrcOpaqueExecutionSession? {}
[extern(cdecl, name="LLVMOrcLLJITGetMainJITDylib", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcLLJITGetMainJITDylib(J : LLVMOrcOpaqueLLJIT? implicit) : LLVMOrcOpaqueJITDylib? {}
[extern(cdecl, name="LLVMOrcLLJITGetTripleString", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcLLJITGetTripleString(J : LLVMOrcOpaqueLLJIT? implicit) : string {}
[extern(cdecl, name="LLVMOrcLLJITGetGlobalPrefix", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcLLJITGetGlobalPrefix(J : LLVMOrcOpaqueLLJIT? implicit) : int8 {}
[extern(cdecl, name="LLVMOrcLLJITMangleAndIntern", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcLLJITMangleAndIntern(J : LLVMOrcOpaqueLLJIT? implicit; UnmangledName : string implicit) : LLVMOrcOpaqueSymbolStringPoolEntry? {}
[extern(cdecl, name="LLVMOrcLLJITAddObjectFile", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcLLJITAddObjectFile(J : LLVMOrcOpaqueLLJIT? implicit; JD : LLVMOrcOpaqueJITDylib? implicit; ObjBuffer : LLVMOpaqueMemoryBuffer? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcLLJITAddObjectFileWithRT", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcLLJITAddObjectFileWithRT(J : LLVMOrcOpaqueLLJIT? implicit; RT : LLVMOrcOpaqueResourceTracker? implicit; ObjBuffer : LLVMOpaqueMemoryBuffer? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcLLJITAddLLVMIRModule", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcLLJITAddLLVMIRModule(J : LLVMOrcOpaqueLLJIT? implicit; JD : LLVMOrcOpaqueJITDylib? implicit; TSM : LLVMOrcOpaqueThreadSafeModule? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcLLJITAddLLVMIRModuleWithRT", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcLLJITAddLLVMIRModuleWithRT(J : LLVMOrcOpaqueLLJIT? implicit; JD : LLVMOrcOpaqueResourceTracker? implicit; TSM : LLVMOrcOpaqueThreadSafeModule? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcLLJITLookup", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcLLJITLookup(J : LLVMOrcOpaqueLLJIT? implicit; Result : uint64? implicit; Name : string implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcLLJITGetObjLinkingLayer", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcLLJITGetObjLinkingLayer(J : LLVMOrcOpaqueLLJIT? implicit) : LLVMOrcOpaqueObjectLayer? {}
[extern(cdecl, name="LLVMOrcLLJITGetObjTransformLayer", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcLLJITGetObjTransformLayer(J : LLVMOrcOpaqueLLJIT? implicit) : LLVMOrcOpaqueObjectTransformLayer? {}
[extern(cdecl, name="LLVMOrcLLJITGetIRTransformLayer", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcLLJITGetIRTransformLayer(J : LLVMOrcOpaqueLLJIT? implicit) : LLVMOrcOpaqueIRTransformLayer? {}
[extern(cdecl, name="LLVMOrcLLJITGetDataLayoutStr", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcLLJITGetDataLayoutStr(J : LLVMOrcOpaqueLLJIT? implicit) : string {}
[extern(cdecl, name="lto_get_version", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_get_version() : string {}
[extern(cdecl, name="lto_get_error_message", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_get_error_message() : string {}
[extern(cdecl, name="lto_module_is_object_file", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_is_object_file(path : string implicit) : bool {}
[extern(cdecl, name="lto_module_is_object_file_for_target", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_is_object_file_for_target(path : string implicit; target_triple_prefix : string implicit) : bool {}
[extern(cdecl, name="lto_module_has_objc_category", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_has_objc_category(mem : void const? implicit; length : uint64) : bool {}
[extern(cdecl, name="lto_module_is_object_file_in_memory", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_is_object_file_in_memory(mem : void const? implicit; length : uint64) : bool {}
[extern(cdecl, name="lto_module_is_object_file_in_memory_for_target", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_is_object_file_in_memory_for_target(mem : void const? implicit; length : uint64; target_triple_prefix : string implicit) : bool {}
[extern(cdecl, name="lto_module_create", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_create(path : string implicit) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_create_from_memory", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_create_from_memory(mem : void const? implicit; length : uint64) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_create_from_memory_with_path", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_create_from_memory_with_path(mem : void const? implicit; length : uint64; path : string implicit) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_create_in_local_context", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_create_in_local_context(mem : void const? implicit; length : uint64; path : string implicit) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_create_in_codegen_context", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_create_in_codegen_context(mem : void const? implicit; length : uint64; path : string implicit; cg : LLVMOpaqueLTOCodeGenerator? implicit) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_create_from_fd", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_create_from_fd(fd : int; path : string implicit; file_size : uint64) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_create_from_fd_at_offset", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_create_from_fd_at_offset(fd : int; path : string implicit; file_size : uint64; map_size : uint64; offset : int64) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_dispose", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_dispose(mod : LLVMOpaqueLTOModule? implicit) : void {}
[extern(cdecl, name="lto_module_get_target_triple", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_get_target_triple(mod : LLVMOpaqueLTOModule? implicit) : string {}
[extern(cdecl, name="lto_module_set_target_triple", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_set_target_triple(mod : LLVMOpaqueLTOModule? implicit; triple : string implicit) : void {}
[extern(cdecl, name="lto_module_get_num_symbols", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_get_num_symbols(mod : LLVMOpaqueLTOModule? implicit) : uint {}
[extern(cdecl, name="lto_module_get_symbol_name", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_get_symbol_name(mod : LLVMOpaqueLTOModule? implicit; index : uint) : string {}
[extern(cdecl, name="lto_module_get_symbol_attribute", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_get_symbol_attribute(mod : LLVMOpaqueLTOModule? implicit; index : uint) : lto_symbol_attributes {}
[extern(cdecl, name="lto_module_get_linkeropts", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_get_linkeropts(mod : LLVMOpaqueLTOModule? implicit) : string {}
[extern(cdecl, name="lto_module_get_macho_cputype", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_get_macho_cputype(mod : LLVMOpaqueLTOModule? implicit; out_cputype : uint? implicit; out_cpusubtype : uint? implicit) : bool {}
[extern(cdecl, name="lto_module_has_ctor_dtor", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_has_ctor_dtor(mod : LLVMOpaqueLTOModule? implicit) : bool {}
[extern(cdecl, name="lto_codegen_create", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_create() : LLVMOpaqueLTOCodeGenerator? {}
[extern(cdecl, name="lto_codegen_create_in_local_context", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_create_in_local_context() : LLVMOpaqueLTOCodeGenerator? {}
[extern(cdecl, name="lto_codegen_dispose", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_dispose(_argument_0x0 : LLVMOpaqueLTOCodeGenerator? implicit) : void {}
[extern(cdecl, name="lto_codegen_add_module", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_add_module(cg : LLVMOpaqueLTOCodeGenerator? implicit; mod : LLVMOpaqueLTOModule? implicit) : bool {}
[extern(cdecl, name="lto_codegen_set_module", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_set_module(cg : LLVMOpaqueLTOCodeGenerator? implicit; mod : LLVMOpaqueLTOModule? implicit) : void {}
[extern(cdecl, name="lto_codegen_set_debug_model", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_set_debug_model(cg : LLVMOpaqueLTOCodeGenerator? implicit; _argument_0x1 : lto_debug_model) : bool {}
[extern(cdecl, name="lto_codegen_set_pic_model", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_set_pic_model(cg : LLVMOpaqueLTOCodeGenerator? implicit; _argument_0x1 : lto_codegen_model) : bool {}
[extern(cdecl, name="lto_codegen_set_cpu", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_set_cpu(cg : LLVMOpaqueLTOCodeGenerator? implicit; cpu : string implicit) : void {}
[extern(cdecl, name="lto_codegen_set_assembler_path", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_set_assembler_path(cg : LLVMOpaqueLTOCodeGenerator? implicit; path : string implicit) : void {}
[extern(cdecl, name="lto_codegen_set_assembler_args", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_set_assembler_args(cg : LLVMOpaqueLTOCodeGenerator? implicit; args : string? implicit; nargs : int) : void {}
[extern(cdecl, name="lto_codegen_add_must_preserve_symbol", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_add_must_preserve_symbol(cg : LLVMOpaqueLTOCodeGenerator? implicit; symbol : string implicit) : void {}
[extern(cdecl, name="lto_codegen_write_merged_modules", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_write_merged_modules(cg : LLVMOpaqueLTOCodeGenerator? implicit; path : string implicit) : bool {}
[extern(cdecl, name="lto_codegen_compile", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_compile(cg : LLVMOpaqueLTOCodeGenerator? implicit; length : uint64? implicit) : void const? {}
[extern(cdecl, name="lto_codegen_compile_to_file", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_compile_to_file(cg : LLVMOpaqueLTOCodeGenerator? implicit; name : string? implicit) : bool {}
[extern(cdecl, name="lto_codegen_optimize", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_optimize(cg : LLVMOpaqueLTOCodeGenerator? implicit) : bool {}
[extern(cdecl, name="lto_codegen_compile_optimized", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_compile_optimized(cg : LLVMOpaqueLTOCodeGenerator? implicit; length : uint64? implicit) : void const? {}
[extern(cdecl, name="lto_api_version", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_api_version() : uint {}
[extern(cdecl, name="lto_set_debug_options", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_set_debug_options(_options : string const? implicit; number : int) : void {}
[extern(cdecl, name="lto_codegen_debug_options", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_debug_options(cg : LLVMOpaqueLTOCodeGenerator? implicit; _argument_0x1 : string implicit) : void {}
[extern(cdecl, name="lto_codegen_debug_options_array", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_debug_options_array(cg : LLVMOpaqueLTOCodeGenerator? implicit; _argument_0x1 : string const? implicit; number : int) : void {}
[extern(cdecl, name="lto_initialize_disassembler", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_initialize_disassembler() : void {}
[extern(cdecl, name="lto_codegen_set_should_internalize", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_set_should_internalize(cg : LLVMOpaqueLTOCodeGenerator? implicit; ShouldInternalize : bool) : void {}
[extern(cdecl, name="lto_codegen_set_should_embed_uselists", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_codegen_set_should_embed_uselists(cg : LLVMOpaqueLTOCodeGenerator? implicit; ShouldEmbedUselists : bool) : void {}
[extern(cdecl, name="lto_input_create", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_input_create(buffer : void const? implicit; buffer_size : uint64; path : string implicit) : LLVMOpaqueLTOInput? {}
[extern(cdecl, name="lto_input_dispose", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_input_dispose(input : LLVMOpaqueLTOInput? implicit) : void {}
[extern(cdecl, name="lto_input_get_num_dependent_libraries", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_input_get_num_dependent_libraries(input : LLVMOpaqueLTOInput? implicit) : uint {}
[extern(cdecl, name="lto_input_get_dependent_library", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_input_get_dependent_library(input : LLVMOpaqueLTOInput? implicit; index : uint64; size : uint64? implicit) : string {}
[extern(cdecl, name="lto_runtime_lib_symbols_list", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_runtime_lib_symbols_list(size : uint64? implicit) : string const? {}
[extern(cdecl, name="thinlto_create_codegen", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_create_codegen() : LLVMOpaqueThinLTOCodeGenerator? {}
[extern(cdecl, name="thinlto_codegen_dispose", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_codegen_dispose(cg : LLVMOpaqueThinLTOCodeGenerator? implicit) : void {}
[extern(cdecl, name="thinlto_codegen_add_module", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_codegen_add_module(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; identifier : string implicit; data : string implicit; length : int) : void {}
[extern(cdecl, name="thinlto_codegen_process", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_codegen_process(cg : LLVMOpaqueThinLTOCodeGenerator? implicit) : void {}
[extern(cdecl, name="thinlto_module_get_num_objects", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_module_get_num_objects(cg : LLVMOpaqueThinLTOCodeGenerator? implicit) : uint {}
[extern(cdecl, name="thinlto_module_get_object", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_module_get_object(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; index : uint) : LTOObjectBuffer {}
[extern(cdecl, name="thinlto_module_get_num_object_files", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_module_get_num_object_files(cg : LLVMOpaqueThinLTOCodeGenerator? implicit) : uint {}
[extern(cdecl, name="thinlto_module_get_object_file", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_module_get_object_file(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; index : uint) : string {}
[extern(cdecl, name="thinlto_codegen_set_pic_model", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_codegen_set_pic_model(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; _argument_0x1 : lto_codegen_model) : bool {}
[extern(cdecl, name="thinlto_codegen_set_savetemps_dir", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_codegen_set_savetemps_dir(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; save_temps_dir : string implicit) : void {}
[extern(cdecl, name="thinlto_set_generated_objects_dir", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_set_generated_objects_dir(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; save_temps_dir : string implicit) : void {}
[extern(cdecl, name="thinlto_codegen_set_cpu", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_codegen_set_cpu(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; cpu : string implicit) : void {}
[extern(cdecl, name="thinlto_codegen_disable_codegen", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_codegen_disable_codegen(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; disable : bool) : void {}
[extern(cdecl, name="thinlto_codegen_set_codegen_only", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_codegen_set_codegen_only(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; codegen_only : bool) : void {}
[extern(cdecl, name="thinlto_debug_options", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_debug_options(_options : string const? implicit; number : int) : void {}
[extern(cdecl, name="lto_module_is_thinlto", linux_library="libLLVM.so", windows_library="LLVM-C")]
def lto_module_is_thinlto(mod : LLVMOpaqueLTOModule? implicit) : bool {}
[extern(cdecl, name="thinlto_codegen_add_must_preserve_symbol", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_codegen_add_must_preserve_symbol(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; name : string implicit; length : int) : void {}
[extern(cdecl, name="thinlto_codegen_add_cross_referenced_symbol", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_codegen_add_cross_referenced_symbol(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; name : string implicit; length : int) : void {}
[extern(cdecl, name="thinlto_codegen_set_cache_dir", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_codegen_set_cache_dir(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; cache_dir : string implicit) : void {}
[extern(cdecl, name="thinlto_codegen_set_cache_pruning_interval", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_codegen_set_cache_pruning_interval(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; interval : int) : void {}
[extern(cdecl, name="thinlto_codegen_set_final_cache_size_relative_to_available_space", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_codegen_set_final_cache_size_relative_to_available_space(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; percentage : uint) : void {}
[extern(cdecl, name="thinlto_codegen_set_cache_entry_expiration", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_codegen_set_cache_entry_expiration(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; expiration : uint) : void {}
[extern(cdecl, name="thinlto_codegen_set_cache_size_bytes", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_codegen_set_cache_size_bytes(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; max_size_bytes : uint) : void {}
[extern(cdecl, name="thinlto_codegen_set_cache_size_megabytes", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_codegen_set_cache_size_megabytes(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; max_size_megabytes : uint) : void {}
[extern(cdecl, name="thinlto_codegen_set_cache_size_files", linux_library="libLLVM.so", windows_library="LLVM-C")]
def thinlto_codegen_set_cache_size_files(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; max_size_files : uint) : void {}
[extern(cdecl, name="LLVMCreateBinary", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateBinary(MemBuf : LLVMOpaqueMemoryBuffer? implicit; Context : LLVMOpaqueContext? implicit; ErrorMessage : string? implicit) : LLVMOpaqueBinary? {}
[extern(cdecl, name="LLVMDisposeBinary", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeBinary(BR : LLVMOpaqueBinary? implicit) : void {}
[extern(cdecl, name="LLVMBinaryCopyMemoryBuffer", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBinaryCopyMemoryBuffer(BR : LLVMOpaqueBinary? implicit) : LLVMOpaqueMemoryBuffer? {}
[extern(cdecl, name="LLVMBinaryGetType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMBinaryGetType(BR : LLVMOpaqueBinary? implicit) : LLVMBinaryType {}
[extern(cdecl, name="LLVMMachOUniversalBinaryCopyObjectForArch", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMMachOUniversalBinaryCopyObjectForArch(BR : LLVMOpaqueBinary? implicit; Arch : string implicit; ArchLen : uint64; ErrorMessage : string? implicit) : LLVMOpaqueBinary? {}
[extern(cdecl, name="LLVMObjectFileCopySectionIterator", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMObjectFileCopySectionIterator(BR : LLVMOpaqueBinary? implicit) : LLVMOpaqueSectionIterator? {}
[extern(cdecl, name="LLVMObjectFileIsSectionIteratorAtEnd", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMObjectFileIsSectionIteratorAtEnd(BR : LLVMOpaqueBinary? implicit; SI : LLVMOpaqueSectionIterator? implicit) : int {}
[extern(cdecl, name="LLVMObjectFileCopySymbolIterator", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMObjectFileCopySymbolIterator(BR : LLVMOpaqueBinary? implicit) : LLVMOpaqueSymbolIterator? {}
[extern(cdecl, name="LLVMObjectFileIsSymbolIteratorAtEnd", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMObjectFileIsSymbolIteratorAtEnd(BR : LLVMOpaqueBinary? implicit; SI : LLVMOpaqueSymbolIterator? implicit) : int {}
[extern(cdecl, name="LLVMDisposeSectionIterator", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeSectionIterator(SI : LLVMOpaqueSectionIterator? implicit) : void {}
[extern(cdecl, name="LLVMMoveToNextSection", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMMoveToNextSection(SI : LLVMOpaqueSectionIterator? implicit) : void {}
[extern(cdecl, name="LLVMMoveToContainingSection", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMMoveToContainingSection(Sect : LLVMOpaqueSectionIterator? implicit; Sym : LLVMOpaqueSymbolIterator? implicit) : void {}
[extern(cdecl, name="LLVMDisposeSymbolIterator", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeSymbolIterator(SI : LLVMOpaqueSymbolIterator? implicit) : void {}
[extern(cdecl, name="LLVMMoveToNextSymbol", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMMoveToNextSymbol(SI : LLVMOpaqueSymbolIterator? implicit) : void {}
[extern(cdecl, name="LLVMGetSectionName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetSectionName(SI : LLVMOpaqueSectionIterator? implicit) : string {}
[extern(cdecl, name="LLVMGetSectionSize", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetSectionSize(SI : LLVMOpaqueSectionIterator? implicit) : uint64 {}
[extern(cdecl, name="LLVMGetSectionContents", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetSectionContents(SI : LLVMOpaqueSectionIterator? implicit) : string {}
[extern(cdecl, name="LLVMGetSectionAddress", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetSectionAddress(SI : LLVMOpaqueSectionIterator? implicit) : uint64 {}
[extern(cdecl, name="LLVMGetSectionContainsSymbol", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetSectionContainsSymbol(SI : LLVMOpaqueSectionIterator? implicit; Sym : LLVMOpaqueSymbolIterator? implicit) : int {}
[extern(cdecl, name="LLVMGetRelocations", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetRelocations(Section : LLVMOpaqueSectionIterator? implicit) : LLVMOpaqueRelocationIterator? {}
[extern(cdecl, name="LLVMDisposeRelocationIterator", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeRelocationIterator(RI : LLVMOpaqueRelocationIterator? implicit) : void {}
[extern(cdecl, name="LLVMIsRelocationIteratorAtEnd", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsRelocationIteratorAtEnd(Section : LLVMOpaqueSectionIterator? implicit; RI : LLVMOpaqueRelocationIterator? implicit) : int {}
[extern(cdecl, name="LLVMMoveToNextRelocation", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMMoveToNextRelocation(RI : LLVMOpaqueRelocationIterator? implicit) : void {}
[extern(cdecl, name="LLVMGetSymbolName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetSymbolName(SI : LLVMOpaqueSymbolIterator? implicit) : string {}
[extern(cdecl, name="LLVMGetSymbolAddress", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetSymbolAddress(SI : LLVMOpaqueSymbolIterator? implicit) : uint64 {}
[extern(cdecl, name="LLVMGetSymbolSize", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetSymbolSize(SI : LLVMOpaqueSymbolIterator? implicit) : uint64 {}
[extern(cdecl, name="LLVMGetRelocationOffset", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetRelocationOffset(RI : LLVMOpaqueRelocationIterator? implicit) : uint64 {}
[extern(cdecl, name="LLVMGetRelocationSymbol", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetRelocationSymbol(RI : LLVMOpaqueRelocationIterator? implicit) : LLVMOpaqueSymbolIterator? {}
[extern(cdecl, name="LLVMGetRelocationType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetRelocationType(RI : LLVMOpaqueRelocationIterator? implicit) : uint64 {}
[extern(cdecl, name="LLVMGetRelocationTypeName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetRelocationTypeName(RI : LLVMOpaqueRelocationIterator? implicit) : string {}
[extern(cdecl, name="LLVMGetRelocationValueString", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetRelocationValueString(RI : LLVMOpaqueRelocationIterator? implicit) : string {}
[extern(cdecl, name="LLVMCreateObjectFile", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreateObjectFile(MemBuf : LLVMOpaqueMemoryBuffer? implicit) : LLVMOpaqueObjectFile? {}
[extern(cdecl, name="LLVMDisposeObjectFile", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposeObjectFile(ObjectFile : LLVMOpaqueObjectFile? implicit) : void {}
[extern(cdecl, name="LLVMGetSections", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetSections(ObjectFile : LLVMOpaqueObjectFile? implicit) : LLVMOpaqueSectionIterator? {}
[extern(cdecl, name="LLVMIsSectionIteratorAtEnd", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsSectionIteratorAtEnd(ObjectFile : LLVMOpaqueObjectFile? implicit; SI : LLVMOpaqueSectionIterator? implicit) : int {}
[extern(cdecl, name="LLVMGetSymbols", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMGetSymbols(ObjectFile : LLVMOpaqueObjectFile? implicit) : LLVMOpaqueSymbolIterator? {}
[extern(cdecl, name="LLVMIsSymbolIteratorAtEnd", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMIsSymbolIteratorAtEnd(ObjectFile : LLVMOpaqueObjectFile? implicit; SI : LLVMOpaqueSymbolIterator? implicit) : int {}
[extern(cdecl, name="LLVMOrcCreateRTDyldObjectLinkingLayerWithSectionMemoryManager", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcCreateRTDyldObjectLinkingLayerWithSectionMemoryManager(ES : LLVMOrcOpaqueExecutionSession? implicit) : LLVMOrcOpaqueObjectLayer? {}
[extern(cdecl, name="LLVMOrcRTDyldObjectLinkingLayerRegisterJITEventListener", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMOrcRTDyldObjectLinkingLayerRegisterJITEventListener(RTDyldObjLinkingLayer : LLVMOrcOpaqueObjectLayer? implicit; Listener : LLVMOpaqueJITEventListener? implicit) : void {}
[extern(cdecl, name="LLVMRemarkStringGetData", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkStringGetData(String : LLVMRemarkOpaqueString? implicit) : string {}
[extern(cdecl, name="LLVMRemarkStringGetLen", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkStringGetLen(String : LLVMRemarkOpaqueString? implicit) : uint {}
[extern(cdecl, name="LLVMRemarkDebugLocGetSourceFilePath", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkDebugLocGetSourceFilePath(DL : LLVMRemarkOpaqueDebugLoc? implicit) : LLVMRemarkOpaqueString? {}
[extern(cdecl, name="LLVMRemarkDebugLocGetSourceLine", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkDebugLocGetSourceLine(DL : LLVMRemarkOpaqueDebugLoc? implicit) : uint {}
[extern(cdecl, name="LLVMRemarkDebugLocGetSourceColumn", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkDebugLocGetSourceColumn(DL : LLVMRemarkOpaqueDebugLoc? implicit) : uint {}
[extern(cdecl, name="LLVMRemarkArgGetKey", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkArgGetKey(Arg : LLVMRemarkOpaqueArg? implicit) : LLVMRemarkOpaqueString? {}
[extern(cdecl, name="LLVMRemarkArgGetValue", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkArgGetValue(Arg : LLVMRemarkOpaqueArg? implicit) : LLVMRemarkOpaqueString? {}
[extern(cdecl, name="LLVMRemarkArgGetDebugLoc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkArgGetDebugLoc(Arg : LLVMRemarkOpaqueArg? implicit) : LLVMRemarkOpaqueDebugLoc? {}
[extern(cdecl, name="LLVMRemarkEntryDispose", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkEntryDispose(Remark : LLVMRemarkOpaqueEntry? implicit) : void {}
[extern(cdecl, name="LLVMRemarkEntryGetType", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkEntryGetType(Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkType {}
[extern(cdecl, name="LLVMRemarkEntryGetPassName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkEntryGetPassName(Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkOpaqueString? {}
[extern(cdecl, name="LLVMRemarkEntryGetRemarkName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkEntryGetRemarkName(Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkOpaqueString? {}
[extern(cdecl, name="LLVMRemarkEntryGetFunctionName", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkEntryGetFunctionName(Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkOpaqueString? {}
[extern(cdecl, name="LLVMRemarkEntryGetDebugLoc", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkEntryGetDebugLoc(Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkOpaqueDebugLoc? {}
[extern(cdecl, name="LLVMRemarkEntryGetHotness", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkEntryGetHotness(Remark : LLVMRemarkOpaqueEntry? implicit) : uint64 {}
[extern(cdecl, name="LLVMRemarkEntryGetNumArgs", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkEntryGetNumArgs(Remark : LLVMRemarkOpaqueEntry? implicit) : uint {}
[extern(cdecl, name="LLVMRemarkEntryGetFirstArg", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkEntryGetFirstArg(Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkOpaqueArg? {}
[extern(cdecl, name="LLVMRemarkEntryGetNextArg", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkEntryGetNextArg(It : LLVMRemarkOpaqueArg? implicit; Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkOpaqueArg? {}
[extern(cdecl, name="LLVMRemarkParserCreateYAML", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkParserCreateYAML(Buf : void const? implicit; Size : uint64) : LLVMRemarkOpaqueParser? {}
[extern(cdecl, name="LLVMRemarkParserCreateBitstream", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkParserCreateBitstream(Buf : void const? implicit; Size : uint64) : LLVMRemarkOpaqueParser? {}
[extern(cdecl, name="LLVMRemarkParserGetNext", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkParserGetNext(Parser : LLVMRemarkOpaqueParser? implicit) : LLVMRemarkOpaqueEntry? {}
[extern(cdecl, name="LLVMRemarkParserHasError", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkParserHasError(Parser : LLVMRemarkOpaqueParser? implicit) : int {}
[extern(cdecl, name="LLVMRemarkParserGetErrorMessage", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkParserGetErrorMessage(Parser : LLVMRemarkOpaqueParser? implicit) : string {}
[extern(cdecl, name="LLVMRemarkParserDispose", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkParserDispose(Parser : LLVMRemarkOpaqueParser? implicit) : void {}
[extern(cdecl, name="LLVMRemarkVersion", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRemarkVersion() : uint {}
[extern(cdecl, name="LLVMLoadLibraryPermanently", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMLoadLibraryPermanently(Filename : string implicit) : int {}
[extern(cdecl, name="LLVMParseCommandLineOptions", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMParseCommandLineOptions(argc : int; argv : string const? implicit; Overview : string implicit) : void {}
[extern(cdecl, name="LLVMSearchForAddressOfSymbol", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMSearchForAddressOfSymbol(symbolName : string implicit) : void? {}
[extern(cdecl, name="LLVMAddSymbol", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddSymbol(symbolName : string implicit; symbolValue : void? implicit) : void {}
[extern(cdecl, name="LLVMAddConstantMergePass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddConstantMergePass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddMergeFunctionsPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddMergeFunctionsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddCalledValuePropagationPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddCalledValuePropagationPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddDeadArgEliminationPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddDeadArgEliminationPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddFunctionAttrsPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddFunctionAttrsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddFunctionInliningPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddFunctionInliningPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddAlwaysInlinerPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddAlwaysInlinerPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddGlobalDCEPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddGlobalDCEPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddGlobalOptimizerPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddGlobalOptimizerPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddIPSCCPPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddIPSCCPPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddInternalizePass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddInternalizePass(_argument_0x0 : LLVMOpaquePassManager? implicit; AllButMain : uint) : void {}
[extern(cdecl, name="LLVMAddStripDeadPrototypesPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddStripDeadPrototypesPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddStripSymbolsPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddStripSymbolsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMRunPasses", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMRunPasses(M : LLVMOpaqueModule? implicit; Passes : string implicit; TM : LLVMOpaqueTargetMachine? implicit; Options : LLVMOpaquePassBuilderOptions? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMCreatePassBuilderOptions", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMCreatePassBuilderOptions() : LLVMOpaquePassBuilderOptions? {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetVerifyEach", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassBuilderOptionsSetVerifyEach(Options : LLVMOpaquePassBuilderOptions? implicit; VerifyEach : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetDebugLogging", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassBuilderOptionsSetDebugLogging(Options : LLVMOpaquePassBuilderOptions? implicit; DebugLogging : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetLoopInterleaving", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassBuilderOptionsSetLoopInterleaving(Options : LLVMOpaquePassBuilderOptions? implicit; LoopInterleaving : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetLoopVectorization", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassBuilderOptionsSetLoopVectorization(Options : LLVMOpaquePassBuilderOptions? implicit; LoopVectorization : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetSLPVectorization", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassBuilderOptionsSetSLPVectorization(Options : LLVMOpaquePassBuilderOptions? implicit; SLPVectorization : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetLoopUnrolling", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassBuilderOptionsSetLoopUnrolling(Options : LLVMOpaquePassBuilderOptions? implicit; LoopUnrolling : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetForgetAllSCEVInLoopUnroll", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassBuilderOptionsSetForgetAllSCEVInLoopUnroll(Options : LLVMOpaquePassBuilderOptions? implicit; ForgetAllSCEVInLoopUnroll : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetLicmMssaOptCap", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassBuilderOptionsSetLicmMssaOptCap(Options : LLVMOpaquePassBuilderOptions? implicit; LicmMssaOptCap : uint) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetLicmMssaNoAccForPromotionCap", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassBuilderOptionsSetLicmMssaNoAccForPromotionCap(Options : LLVMOpaquePassBuilderOptions? implicit; LicmMssaNoAccForPromotionCap : uint) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetCallGraphProfile", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassBuilderOptionsSetCallGraphProfile(Options : LLVMOpaquePassBuilderOptions? implicit; CallGraphProfile : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetMergeFunctions", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassBuilderOptionsSetMergeFunctions(Options : LLVMOpaquePassBuilderOptions? implicit; MergeFunctions : int) : void {}
[extern(cdecl, name="LLVMDisposePassBuilderOptions", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMDisposePassBuilderOptions(Options : LLVMOpaquePassBuilderOptions? implicit) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderCreate", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassManagerBuilderCreate() : LLVMOpaquePassManagerBuilder? {}
[extern(cdecl, name="LLVMPassManagerBuilderDispose", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassManagerBuilderDispose(PMB : LLVMOpaquePassManagerBuilder? implicit) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderSetOptLevel", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassManagerBuilderSetOptLevel(PMB : LLVMOpaquePassManagerBuilder? implicit; OptLevel : uint) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderSetSizeLevel", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassManagerBuilderSetSizeLevel(PMB : LLVMOpaquePassManagerBuilder? implicit; SizeLevel : uint) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderSetDisableUnitAtATime", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassManagerBuilderSetDisableUnitAtATime(PMB : LLVMOpaquePassManagerBuilder? implicit; Value : int) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderSetDisableUnrollLoops", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassManagerBuilderSetDisableUnrollLoops(PMB : LLVMOpaquePassManagerBuilder? implicit; Value : int) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderSetDisableSimplifyLibCalls", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassManagerBuilderSetDisableSimplifyLibCalls(PMB : LLVMOpaquePassManagerBuilder? implicit; Value : int) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderUseInlinerWithThreshold", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassManagerBuilderUseInlinerWithThreshold(PMB : LLVMOpaquePassManagerBuilder? implicit; Threshold : uint) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderPopulateFunctionPassManager", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassManagerBuilderPopulateFunctionPassManager(PMB : LLVMOpaquePassManagerBuilder? implicit; PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderPopulateModulePassManager", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMPassManagerBuilderPopulateModulePassManager(PMB : LLVMOpaquePassManagerBuilder? implicit; PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddAggressiveDCEPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddAggressiveDCEPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddDCEPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddDCEPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddBitTrackingDCEPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddBitTrackingDCEPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddAlignmentFromAssumptionsPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddAlignmentFromAssumptionsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddCFGSimplificationPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddCFGSimplificationPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddDeadStoreEliminationPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddDeadStoreEliminationPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddScalarizerPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddScalarizerPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddMergedLoadStoreMotionPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddMergedLoadStoreMotionPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddGVNPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddGVNPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddNewGVNPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddNewGVNPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddIndVarSimplifyPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddIndVarSimplifyPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddInstructionCombiningPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddInstructionCombiningPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddInstructionSimplifyPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddInstructionSimplifyPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddJumpThreadingPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddJumpThreadingPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLICMPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddLICMPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopDeletionPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddLoopDeletionPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopIdiomPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddLoopIdiomPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopRotatePass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddLoopRotatePass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopRerollPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddLoopRerollPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopUnrollPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddLoopUnrollPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopUnrollAndJamPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddLoopUnrollAndJamPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLowerAtomicPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddLowerAtomicPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddMemCpyOptPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddMemCpyOptPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddPartiallyInlineLibCallsPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddPartiallyInlineLibCallsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddReassociatePass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddReassociatePass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddSCCPPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddSCCPPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddScalarReplAggregatesPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddScalarReplAggregatesPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddScalarReplAggregatesPassSSA", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddScalarReplAggregatesPassSSA(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddScalarReplAggregatesPassWithThreshold", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddScalarReplAggregatesPassWithThreshold(PM : LLVMOpaquePassManager? implicit; Threshold : int) : void {}
[extern(cdecl, name="LLVMAddSimplifyLibCallsPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddSimplifyLibCallsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddTailCallEliminationPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddTailCallEliminationPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddDemoteMemoryToRegisterPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddDemoteMemoryToRegisterPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddVerifierPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddVerifierPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddCorrelatedValuePropagationPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddCorrelatedValuePropagationPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddEarlyCSEPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddEarlyCSEPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddEarlyCSEMemSSAPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddEarlyCSEMemSSAPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLowerExpectIntrinsicPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddLowerExpectIntrinsicPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLowerConstantIntrinsicsPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddLowerConstantIntrinsicsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddTypeBasedAliasAnalysisPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddTypeBasedAliasAnalysisPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddScopedNoAliasAAPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddScopedNoAliasAAPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddBasicAliasAnalysisPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddBasicAliasAnalysisPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddUnifyFunctionExitNodesPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddUnifyFunctionExitNodesPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLowerSwitchPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddLowerSwitchPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddPromoteMemoryToRegisterPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddPromoteMemoryToRegisterPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddAddDiscriminatorsPass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddAddDiscriminatorsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopVectorizePass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddLoopVectorizePass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddSLPVectorizePass", linux_library="libLLVM.so", windows_library="LLVM-C")]
def LLVMAddSLPVectorizePass(PM : LLVMOpaquePassManager? implicit) : void {}
