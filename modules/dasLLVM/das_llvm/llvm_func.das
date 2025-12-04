require dasbind public
options gen2
module llvm_func shared
require llvm_struct
require llvm_enum
[extern(cdecl, name="LLVMVerifyModule", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMVerifyModule(M : LLVMOpaqueModule? implicit; Action : LLVMVerifierFailureAction; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMVerifyFunction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMVerifyFunction(Fn : LLVMOpaqueValue? implicit; Action : LLVMVerifierFailureAction) : int {}
[extern(cdecl, name="LLVMViewFunctionCFG", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMViewFunctionCFG(Fn : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMViewFunctionCFGOnly", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMViewFunctionCFGOnly(Fn : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMParseBitcode", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMParseBitcode(MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutModule : LLVMOpaqueModule ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMParseBitcode2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMParseBitcode2(MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutModule : LLVMOpaqueModule ?? implicit) : int {}
[extern(cdecl, name="LLVMParseBitcodeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMParseBitcodeInContext(ContextRef : LLVMOpaqueContext? implicit; MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutModule : LLVMOpaqueModule ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMParseBitcodeInContext2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMParseBitcodeInContext2(ContextRef : LLVMOpaqueContext? implicit; MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutModule : LLVMOpaqueModule ?? implicit) : int {}
[extern(cdecl, name="LLVMGetBitcodeModuleInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetBitcodeModuleInContext(ContextRef : LLVMOpaqueContext? implicit; MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutM : LLVMOpaqueModule ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMGetBitcodeModuleInContext2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetBitcodeModuleInContext2(ContextRef : LLVMOpaqueContext? implicit; MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutM : LLVMOpaqueModule ?? implicit) : int {}
[extern(cdecl, name="LLVMGetBitcodeModule", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetBitcodeModule(MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutM : LLVMOpaqueModule ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMGetBitcodeModule2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetBitcodeModule2(MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutM : LLVMOpaqueModule ?? implicit) : int {}
[extern(cdecl, name="LLVMWriteBitcodeToFile", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMWriteBitcodeToFile(M : LLVMOpaqueModule? implicit; Path : string implicit) : int {}
[extern(cdecl, name="LLVMWriteBitcodeToFD", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMWriteBitcodeToFD(M : LLVMOpaqueModule? implicit; FD : int; ShouldClose : int; Unbuffered : int) : int {}
[extern(cdecl, name="LLVMWriteBitcodeToFileHandle", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMWriteBitcodeToFileHandle(M : LLVMOpaqueModule? implicit; Handle : int) : int {}
[extern(cdecl, name="LLVMWriteBitcodeToMemoryBuffer", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMWriteBitcodeToMemoryBuffer(M : LLVMOpaqueModule? implicit) : LLVMOpaqueMemoryBuffer? {}
[extern(cdecl, name="LLVMGetOrInsertComdat", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetOrInsertComdat(M : LLVMOpaqueModule? implicit; Name : string implicit) : LLVMComdat? {}
[extern(cdecl, name="LLVMGetComdat", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetComdat(V : LLVMOpaqueValue? implicit) : LLVMComdat? {}
[extern(cdecl, name="LLVMSetComdat", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetComdat(V : LLVMOpaqueValue? implicit; C : LLVMComdat? implicit) : void {}
[extern(cdecl, name="LLVMGetComdatSelectionKind", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetComdatSelectionKind(C : LLVMComdat? implicit) : LLVMComdatSelectionKind {}
[extern(cdecl, name="LLVMSetComdatSelectionKind", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetComdatSelectionKind(C : LLVMComdat? implicit; Kind : LLVMComdatSelectionKind) : void {}
[extern(cdecl, name="LLVMResetFatalErrorHandler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMResetFatalErrorHandler() : void {}
[extern(cdecl, name="LLVMEnablePrettyStackTrace", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMEnablePrettyStackTrace() : void {}
[extern(cdecl, name="LLVMShutdown", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMShutdown() : void {}
[extern(cdecl, name="LLVMGetVersion", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetVersion(Major : uint? implicit; Minor : uint? implicit; Patch : uint? implicit) : void {}
[extern(cdecl, name="LLVMCreateMessage", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateMessage(Message : string implicit) : string {}
[extern(cdecl, name="LLVMDisposeMessage", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeMessage(Message : string implicit) : void {}
[extern(cdecl, name="LLVMContextCreate", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMContextCreate() : LLVMOpaqueContext? {}
[extern(cdecl, name="LLVMGetGlobalContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetGlobalContext() : LLVMOpaqueContext? {}
[extern(cdecl, name="LLVMContextGetDiagnosticContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMContextGetDiagnosticContext(C : LLVMOpaqueContext? implicit) : void? {}
[extern(cdecl, name="LLVMContextShouldDiscardValueNames", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMContextShouldDiscardValueNames(C : LLVMOpaqueContext? implicit) : int {}
[extern(cdecl, name="LLVMContextSetDiscardValueNames", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMContextSetDiscardValueNames(C : LLVMOpaqueContext? implicit; Discard : int) : void {}
[extern(cdecl, name="LLVMContextSetOpaquePointers", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMContextSetOpaquePointers(C : LLVMOpaqueContext? implicit; OpaquePointers : int) : void {}
[extern(cdecl, name="LLVMContextDispose", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMContextDispose(C : LLVMOpaqueContext? implicit) : void {}
[extern(cdecl, name="LLVMGetDiagInfoDescription", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetDiagInfoDescription(DI : LLVMOpaqueDiagnosticInfo? implicit) : string {}
[extern(cdecl, name="LLVMGetDiagInfoSeverity", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetDiagInfoSeverity(DI : LLVMOpaqueDiagnosticInfo? implicit) : LLVMDiagnosticSeverity {}
[extern(cdecl, name="LLVMGetMDKindIDInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetMDKindIDInContext(C : LLVMOpaqueContext? implicit; Name : string implicit; SLen : uint) : uint {}
[extern(cdecl, name="LLVMGetMDKindID", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetMDKindID(Name : string implicit; SLen : uint) : uint {}
[extern(cdecl, name="LLVMGetEnumAttributeKindForName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetEnumAttributeKindForName(Name : string implicit; SLen : uint64) : uint {}
[extern(cdecl, name="LLVMGetLastEnumAttributeKind", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetLastEnumAttributeKind() : uint {}
[extern(cdecl, name="LLVMCreateEnumAttribute", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateEnumAttribute(C : LLVMOpaqueContext? implicit; KindID : uint; Val : uint64) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMGetEnumAttributeKind", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetEnumAttributeKind(A : LLVMOpaqueAttributeRef? implicit) : uint {}
[extern(cdecl, name="LLVMGetEnumAttributeValue", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetEnumAttributeValue(A : LLVMOpaqueAttributeRef? implicit) : uint64 {}
[extern(cdecl, name="LLVMCreateTypeAttribute", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateTypeAttribute(C : LLVMOpaqueContext? implicit; KindID : uint; type_ref : LLVMOpaqueType? implicit) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMGetTypeAttributeValue", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetTypeAttributeValue(A : LLVMOpaqueAttributeRef? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMCreateStringAttribute", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateStringAttribute(C : LLVMOpaqueContext? implicit; K : string implicit; KLength : uint; V : string implicit; VLength : uint) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMGetStringAttributeKind", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetStringAttributeKind(A : LLVMOpaqueAttributeRef? implicit; Length : uint? implicit) : string {}
[extern(cdecl, name="LLVMGetStringAttributeValue", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetStringAttributeValue(A : LLVMOpaqueAttributeRef? implicit; Length : uint? implicit) : string {}
[extern(cdecl, name="LLVMIsEnumAttribute", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsEnumAttribute(A : LLVMOpaqueAttributeRef? implicit) : int {}
[extern(cdecl, name="LLVMIsStringAttribute", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsStringAttribute(A : LLVMOpaqueAttributeRef? implicit) : int {}
[extern(cdecl, name="LLVMIsTypeAttribute", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsTypeAttribute(A : LLVMOpaqueAttributeRef? implicit) : int {}
[extern(cdecl, name="LLVMGetTypeByName2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetTypeByName2(C : LLVMOpaqueContext? implicit; Name : string implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMModuleCreateWithName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMModuleCreateWithName(ModuleID : string implicit) : LLVMOpaqueModule? {}
[extern(cdecl, name="LLVMModuleCreateWithNameInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMModuleCreateWithNameInContext(ModuleID : string implicit; C : LLVMOpaqueContext? implicit) : LLVMOpaqueModule? {}
[extern(cdecl, name="LLVMCloneModule", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCloneModule(M : LLVMOpaqueModule? implicit) : LLVMOpaqueModule? {}
[extern(cdecl, name="LLVMDisposeModule", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeModule(M : LLVMOpaqueModule? implicit) : void {}
[extern(cdecl, name="LLVMGetModuleIdentifier", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetModuleIdentifier(M : LLVMOpaqueModule? implicit; Len : uint64? implicit) : string {}
[extern(cdecl, name="LLVMSetModuleIdentifier", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetModuleIdentifier(M : LLVMOpaqueModule? implicit; Ident : string implicit; Len : uint64) : void {}
[extern(cdecl, name="LLVMGetSourceFileName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetSourceFileName(M : LLVMOpaqueModule? implicit; Len : uint64? implicit) : string {}
[extern(cdecl, name="LLVMSetSourceFileName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetSourceFileName(M : LLVMOpaqueModule? implicit; Name : string implicit; Len : uint64) : void {}
[extern(cdecl, name="LLVMGetDataLayoutStr", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetDataLayoutStr(M : LLVMOpaqueModule? implicit) : string {}
[extern(cdecl, name="LLVMGetDataLayout", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetDataLayout(M : LLVMOpaqueModule? implicit) : string {}
[extern(cdecl, name="LLVMSetDataLayout", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetDataLayout(M : LLVMOpaqueModule? implicit; DataLayoutStr : string implicit) : void {}
[extern(cdecl, name="LLVMGetTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetTarget(M : LLVMOpaqueModule? implicit) : string {}
[extern(cdecl, name="LLVMSetTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetTarget(M : LLVMOpaqueModule? implicit; Triple : string implicit) : void {}
[extern(cdecl, name="LLVMCopyModuleFlagsMetadata", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCopyModuleFlagsMetadata(M : LLVMOpaqueModule? implicit; Len : uint64? implicit) : LLVMOpaqueModuleFlagEntry? {}
[extern(cdecl, name="LLVMDisposeModuleFlagsMetadata", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeModuleFlagsMetadata(Entries : LLVMOpaqueModuleFlagEntry? implicit) : void {}
[extern(cdecl, name="LLVMModuleFlagEntriesGetFlagBehavior", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMModuleFlagEntriesGetFlagBehavior(Entries : LLVMOpaqueModuleFlagEntry? implicit; Index : uint) : LLVMModuleFlagBehavior {}
[extern(cdecl, name="LLVMModuleFlagEntriesGetKey", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMModuleFlagEntriesGetKey(Entries : LLVMOpaqueModuleFlagEntry? implicit; Index : uint; Len : uint64? implicit) : string {}
[extern(cdecl, name="LLVMModuleFlagEntriesGetMetadata", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMModuleFlagEntriesGetMetadata(Entries : LLVMOpaqueModuleFlagEntry? implicit; Index : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMGetModuleFlag", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetModuleFlag(M : LLVMOpaqueModule? implicit; Key : string implicit; KeyLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMAddModuleFlag", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddModuleFlag(M : LLVMOpaqueModule? implicit; Behavior : LLVMModuleFlagBehavior; Key : string implicit; KeyLen : uint64; Val : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMDumpModule", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDumpModule(M : LLVMOpaqueModule? implicit) : void {}
[extern(cdecl, name="LLVMPrintModuleToFile", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPrintModuleToFile(M : LLVMOpaqueModule? implicit; Filename : string implicit; ErrorMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMPrintModuleToString", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPrintModuleToString(M : LLVMOpaqueModule? implicit) : string {}
[extern(cdecl, name="LLVMGetModuleInlineAsm", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetModuleInlineAsm(M : LLVMOpaqueModule? implicit; Len : uint64? implicit) : string {}
[extern(cdecl, name="LLVMSetModuleInlineAsm2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetModuleInlineAsm2(M : LLVMOpaqueModule? implicit; Asm : string implicit; Len : uint64) : void {}
[extern(cdecl, name="LLVMAppendModuleInlineAsm", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAppendModuleInlineAsm(M : LLVMOpaqueModule? implicit; Asm : string implicit; Len : uint64) : void {}
[extern(cdecl, name="LLVMGetInlineAsm", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetInlineAsm(Ty : LLVMOpaqueType? implicit; AsmString : string implicit; AsmStringSize : uint64; Constraints : string implicit; ConstraintsSize : uint64; HasSideEffects : int; IsAlignStack : int; Dialect : LLVMInlineAsmDialect; CanThrow : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetModuleContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetModuleContext(M : LLVMOpaqueModule? implicit) : LLVMOpaqueContext? {}
[extern(cdecl, name="LLVMGetTypeByName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetTypeByName(M : LLVMOpaqueModule? implicit; Name : string implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetFirstNamedMetadata", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetFirstNamedMetadata(M : LLVMOpaqueModule? implicit) : LLVMOpaqueNamedMDNode? {}
[extern(cdecl, name="LLVMGetLastNamedMetadata", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetLastNamedMetadata(M : LLVMOpaqueModule? implicit) : LLVMOpaqueNamedMDNode? {}
[extern(cdecl, name="LLVMGetNextNamedMetadata", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNextNamedMetadata(NamedMDNode : LLVMOpaqueNamedMDNode? implicit) : LLVMOpaqueNamedMDNode? {}
[extern(cdecl, name="LLVMGetPreviousNamedMetadata", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetPreviousNamedMetadata(NamedMDNode : LLVMOpaqueNamedMDNode? implicit) : LLVMOpaqueNamedMDNode? {}
[extern(cdecl, name="LLVMGetNamedMetadata", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNamedMetadata(M : LLVMOpaqueModule? implicit; Name : string implicit; NameLen : uint64) : LLVMOpaqueNamedMDNode? {}
[extern(cdecl, name="LLVMGetOrInsertNamedMetadata", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetOrInsertNamedMetadata(M : LLVMOpaqueModule? implicit; Name : string implicit; NameLen : uint64) : LLVMOpaqueNamedMDNode? {}
[extern(cdecl, name="LLVMGetNamedMetadataName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNamedMetadataName(NamedMD : LLVMOpaqueNamedMDNode? implicit; NameLen : uint64? implicit) : string {}
[extern(cdecl, name="LLVMGetNamedMetadataNumOperands", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNamedMetadataNumOperands(M : LLVMOpaqueModule? implicit; Name : string implicit) : uint {}
[extern(cdecl, name="LLVMGetNamedMetadataOperands", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNamedMetadataOperands(M : LLVMOpaqueModule? implicit; Name : string implicit; Dest : LLVMOpaqueValue ?? implicit) : void {}
[extern(cdecl, name="LLVMAddNamedMetadataOperand", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddNamedMetadataOperand(M : LLVMOpaqueModule? implicit; Name : string implicit; Val : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetDebugLocDirectory", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetDebugLocDirectory(Val : LLVMOpaqueValue? implicit; Length : uint? implicit) : string {}
[extern(cdecl, name="LLVMGetDebugLocFilename", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetDebugLocFilename(Val : LLVMOpaqueValue? implicit; Length : uint? implicit) : string {}
[extern(cdecl, name="LLVMGetDebugLocLine", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetDebugLocLine(Val : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetDebugLocColumn", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetDebugLocColumn(Val : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMAddFunction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddFunction(M : LLVMOpaqueModule? implicit; Name : string implicit; FunctionTy : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNamedFunction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNamedFunction(M : LLVMOpaqueModule? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetFirstFunction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetFirstFunction(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetLastFunction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetLastFunction(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNextFunction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNextFunction(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPreviousFunction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetPreviousFunction(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetModuleInlineAsm", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetModuleInlineAsm(M : LLVMOpaqueModule? implicit; Asm : string implicit) : void {}
[extern(cdecl, name="LLVMGetTypeKind", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetTypeKind(Ty : LLVMOpaqueType? implicit) : LLVMTypeKind {}
[extern(cdecl, name="LLVMTypeIsSized", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMTypeIsSized(Ty : LLVMOpaqueType? implicit) : int {}
[extern(cdecl, name="LLVMGetTypeContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetTypeContext(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueContext? {}
[extern(cdecl, name="LLVMDumpType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDumpType(Val : LLVMOpaqueType? implicit) : void {}
[extern(cdecl, name="LLVMPrintTypeToString", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPrintTypeToString(Val : LLVMOpaqueType? implicit) : string {}
[extern(cdecl, name="LLVMInt1TypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInt1TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt8TypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInt8TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt16TypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInt16TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt32TypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInt32TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt64TypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInt64TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt128TypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInt128TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIntTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIntTypeInContext(C : LLVMOpaqueContext? implicit; NumBits : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt1Type", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInt1Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt8Type", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInt8Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt16Type", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInt16Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt32Type", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInt32Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt64Type", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInt64Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMInt128Type", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInt128Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIntType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIntType(NumBits : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetIntTypeWidth", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetIntTypeWidth(IntegerTy : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMHalfTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMHalfTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMBFloatTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBFloatTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMFloatTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMFloatTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMDoubleTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDoubleTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMX86FP80TypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMX86FP80TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMFP128TypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMFP128TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMPPCFP128TypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPPCFP128TypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMHalfType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMHalfType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMBFloatType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBFloatType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMFloatType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMFloatType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMDoubleType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDoubleType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMX86FP80Type", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMX86FP80Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMFP128Type", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMFP128Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMPPCFP128Type", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPPCFP128Type() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMFunctionType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMFunctionType(ReturnType : LLVMOpaqueType? implicit; ParamTypes : LLVMOpaqueType ?? implicit; ParamCount : uint; IsVarArg : int) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIsFunctionVarArg", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsFunctionVarArg(FunctionTy : LLVMOpaqueType? implicit) : int {}
[extern(cdecl, name="LLVMGetReturnType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetReturnType(FunctionTy : LLVMOpaqueType? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMCountParamTypes", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCountParamTypes(FunctionTy : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMGetParamTypes", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetParamTypes(FunctionTy : LLVMOpaqueType? implicit; Dest : LLVMOpaqueType ?? implicit) : void {}
[extern(cdecl, name="LLVMStructTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMStructTypeInContext(C : LLVMOpaqueContext? implicit; ElementTypes : LLVMOpaqueType ?? implicit; ElementCount : uint; Packed : int) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMStructType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMStructType(ElementTypes : LLVMOpaqueType ?? implicit; ElementCount : uint; Packed : int) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMStructCreateNamed", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMStructCreateNamed(C : LLVMOpaqueContext? implicit; Name : string implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetStructName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetStructName(Ty : LLVMOpaqueType? implicit) : string {}
[extern(cdecl, name="LLVMStructSetBody", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMStructSetBody(StructTy : LLVMOpaqueType? implicit; ElementTypes : LLVMOpaqueType ?? implicit; ElementCount : uint; Packed : int) : void {}
[extern(cdecl, name="LLVMCountStructElementTypes", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCountStructElementTypes(StructTy : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMGetStructElementTypes", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetStructElementTypes(StructTy : LLVMOpaqueType? implicit; Dest : LLVMOpaqueType ?? implicit) : void {}
[extern(cdecl, name="LLVMStructGetTypeAtIndex", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMStructGetTypeAtIndex(StructTy : LLVMOpaqueType? implicit; i : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIsPackedStruct", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsPackedStruct(StructTy : LLVMOpaqueType? implicit) : int {}
[extern(cdecl, name="LLVMIsOpaqueStruct", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsOpaqueStruct(StructTy : LLVMOpaqueType? implicit) : int {}
[extern(cdecl, name="LLVMIsLiteralStruct", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsLiteralStruct(StructTy : LLVMOpaqueType? implicit) : int {}
[extern(cdecl, name="LLVMGetElementType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetElementType(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetSubtypes", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetSubtypes(Tp : LLVMOpaqueType? implicit; Arr : LLVMOpaqueType ?? implicit) : void {}
[extern(cdecl, name="LLVMGetNumContainedTypes", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNumContainedTypes(Tp : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMArrayType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMArrayType(ElementType : LLVMOpaqueType? implicit; ElementCount : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetArrayLength", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetArrayLength(ArrayTy : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMPointerType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPointerType(ElementType : LLVMOpaqueType? implicit; AddressSpace : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMPointerTypeIsOpaque", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPointerTypeIsOpaque(Ty : LLVMOpaqueType? implicit) : int {}
[extern(cdecl, name="LLVMPointerTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPointerTypeInContext(C : LLVMOpaqueContext? implicit; AddressSpace : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetPointerAddressSpace", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetPointerAddressSpace(PointerTy : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMVectorType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMVectorType(ElementType : LLVMOpaqueType? implicit; ElementCount : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMScalableVectorType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMScalableVectorType(ElementType : LLVMOpaqueType? implicit; ElementCount : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetVectorSize", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetVectorSize(VectorTy : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMVoidTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMVoidTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMLabelTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMLabelTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMX86MMXTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMX86MMXTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMX86AMXTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMX86AMXTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMTokenTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMTokenTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMMetadataTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMMetadataTypeInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMVoidType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMVoidType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMLabelType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMLabelType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMX86MMXType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMX86MMXType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMX86AMXType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMX86AMXType() : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMTargetExtTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMTargetExtTypeInContext(C : LLVMOpaqueContext? implicit; Name : string implicit; TypeParams : LLVMOpaqueType ?? implicit; TypeParamCount : uint; IntParams : uint? implicit; IntParamCount : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMTypeOf", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMTypeOf(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetValueKind", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetValueKind(Val : LLVMOpaqueValue? implicit) : LLVMValueKind {}
[extern(cdecl, name="LLVMGetValueName2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetValueName2(Val : LLVMOpaqueValue? implicit; Length : uint64? implicit) : string {}
[extern(cdecl, name="LLVMSetValueName2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetValueName2(Val : LLVMOpaqueValue? implicit; Name : string implicit; NameLen : uint64) : void {}
[extern(cdecl, name="LLVMDumpValue", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDumpValue(Val : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMPrintValueToString", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPrintValueToString(Val : LLVMOpaqueValue? implicit) : string {}
[extern(cdecl, name="LLVMReplaceAllUsesWith", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMReplaceAllUsesWith(OldVal : LLVMOpaqueValue? implicit; NewVal : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMIsConstant", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsConstant(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMIsUndef", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsUndef(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMIsPoison", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsPoison(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMIsAArgument", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAArgument(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsABasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsABasicBlock(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAInlineAsm", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAInlineAsm(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAUser", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAUser(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstant", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAConstant(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsABlockAddress", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsABlockAddress(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantAggregateZero", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAConstantAggregateZero(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantArray", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAConstantArray(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantDataSequential", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAConstantDataSequential(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantDataArray", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAConstantDataArray(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantDataVector", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAConstantDataVector(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantExpr", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAConstantExpr(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantFP", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAConstantFP(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantInt", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAConstantInt(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantPointerNull", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAConstantPointerNull(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantStruct", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAConstantStruct(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantTokenNone", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAConstantTokenNone(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAConstantVector", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAConstantVector(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAGlobalValue", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAGlobalValue(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAGlobalAlias", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAGlobalAlias(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAGlobalObject", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAGlobalObject(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFunction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAFunction(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAGlobalVariable", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAGlobalVariable(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAGlobalIFunc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAGlobalIFunc(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAUndefValue", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAUndefValue(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAPoisonValue", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAPoisonValue(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAInstruction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAInstruction(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAUnaryOperator", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAUnaryOperator(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsABinaryOperator", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsABinaryOperator(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACallInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsACallInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAIntrinsicInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAIntrinsicInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsADbgInfoIntrinsic", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsADbgInfoIntrinsic(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsADbgVariableIntrinsic", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsADbgVariableIntrinsic(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsADbgDeclareInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsADbgDeclareInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsADbgLabelInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsADbgLabelInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAMemIntrinsic", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAMemIntrinsic(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAMemCpyInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAMemCpyInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAMemMoveInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAMemMoveInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAMemSetInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAMemSetInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACmpInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsACmpInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFCmpInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAFCmpInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAICmpInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAICmpInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAExtractElementInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAExtractElementInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAGetElementPtrInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAGetElementPtrInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAInsertElementInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAInsertElementInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAInsertValueInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAInsertValueInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsALandingPadInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsALandingPadInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAPHINode", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAPHINode(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsASelectInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsASelectInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAShuffleVectorInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAShuffleVectorInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAStoreInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAStoreInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsABranchInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsABranchInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAIndirectBrInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAIndirectBrInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAInvokeInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAInvokeInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAReturnInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAReturnInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsASwitchInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsASwitchInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAUnreachableInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAUnreachableInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAResumeInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAResumeInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACleanupReturnInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsACleanupReturnInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACatchReturnInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsACatchReturnInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACatchSwitchInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsACatchSwitchInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACallBrInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsACallBrInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFuncletPadInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAFuncletPadInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACatchPadInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsACatchPadInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACleanupPadInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsACleanupPadInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAUnaryInstruction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAUnaryInstruction(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAAllocaInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAAllocaInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsACastInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsACastInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAAddrSpaceCastInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAAddrSpaceCastInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsABitCastInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsABitCastInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFPExtInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAFPExtInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFPToSIInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAFPToSIInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFPToUIInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAFPToUIInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFPTruncInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAFPTruncInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAIntToPtrInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAIntToPtrInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAPtrToIntInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAPtrToIntInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsASExtInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsASExtInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsASIToFPInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsASIToFPInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsATruncInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsATruncInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAUIToFPInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAUIToFPInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAZExtInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAZExtInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAExtractValueInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAExtractValueInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsALoadInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsALoadInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAVAArgInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAVAArgInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFreezeInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAFreezeInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAAtomicCmpXchgInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAAtomicCmpXchgInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAAtomicRMWInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAAtomicRMWInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAFenceInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAFenceInst(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAMDNode", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAMDNode(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsAMDString", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAMDString(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetValueName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetValueName(Val : LLVMOpaqueValue? implicit) : string {}
[extern(cdecl, name="LLVMSetValueName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetValueName(Val : LLVMOpaqueValue? implicit; Name : string implicit) : void {}
[extern(cdecl, name="LLVMGetFirstUse", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetFirstUse(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueUse? {}
[extern(cdecl, name="LLVMGetNextUse", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNextUse(U : LLVMOpaqueUse? implicit) : LLVMOpaqueUse? {}
[extern(cdecl, name="LLVMGetUser", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetUser(U : LLVMOpaqueUse? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetUsedValue", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetUsedValue(U : LLVMOpaqueUse? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetOperand", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetOperand(Val : LLVMOpaqueValue? implicit; Index : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetOperandUse", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetOperandUse(Val : LLVMOpaqueValue? implicit; Index : uint) : LLVMOpaqueUse? {}
[extern(cdecl, name="LLVMSetOperand", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetOperand(User : LLVMOpaqueValue? implicit; Index : uint; Val : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetNumOperands", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNumOperands(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMConstNull", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstNull(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstAllOnes", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstAllOnes(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetUndef", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetUndef(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPoison", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetPoison(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsNull", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsNull(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMConstPointerNull", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstPointerNull(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstInt", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstInt(IntTy : LLVMOpaqueType? implicit; N : uint64; SignExtend : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstIntOfArbitraryPrecision", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstIntOfArbitraryPrecision(IntTy : LLVMOpaqueType? implicit; NumWords : uint; Words : void? const) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstIntOfString", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstIntOfString(IntTy : LLVMOpaqueType? implicit; Text : string implicit; Radix : uint8) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstIntOfStringAndSize", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstIntOfStringAndSize(IntTy : LLVMOpaqueType? implicit; Text : string implicit; SLen : uint; Radix : uint8) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstReal", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstReal(RealTy : LLVMOpaqueType? implicit; N : double) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstRealOfString", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstRealOfString(RealTy : LLVMOpaqueType? implicit; Text : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstRealOfStringAndSize", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstRealOfStringAndSize(RealTy : LLVMOpaqueType? implicit; Text : string implicit; SLen : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstIntGetZExtValue", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstIntGetZExtValue(ConstantVal : LLVMOpaqueValue? implicit) : uint64 {}
[extern(cdecl, name="LLVMConstIntGetSExtValue", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstIntGetSExtValue(ConstantVal : LLVMOpaqueValue? implicit) : int64 {}
[extern(cdecl, name="LLVMConstRealGetDouble", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstRealGetDouble(ConstantVal : LLVMOpaqueValue? implicit; losesInfo : int? implicit) : double {}
[extern(cdecl, name="LLVMConstStringInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstStringInContext(C : LLVMOpaqueContext? implicit; Str : string implicit; Length : uint; DontNullTerminate : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstString", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstString(Str : string implicit; Length : uint; DontNullTerminate : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsConstantString", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsConstantString(c : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMGetAsString", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetAsString(c : LLVMOpaqueValue? implicit; Length : uint64? implicit) : string {}
[extern(cdecl, name="LLVMConstStructInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstStructInContext(C : LLVMOpaqueContext? implicit; ConstantVals : LLVMOpaqueValue ?? implicit; Count : uint; Packed : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstStruct", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstStruct(ConstantVals : LLVMOpaqueValue ?? implicit; Count : uint; Packed : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstArray", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstArray(ElementTy : LLVMOpaqueType? implicit; ConstantVals : LLVMOpaqueValue ?? implicit; Length : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNamedStruct", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstNamedStruct(StructTy : LLVMOpaqueType? implicit; ConstantVals : LLVMOpaqueValue ?? implicit; Count : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetAggregateElement", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetAggregateElement(C : LLVMOpaqueValue? implicit; Idx : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetElementAsConstant", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetElementAsConstant(C : LLVMOpaqueValue? implicit; idx : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstVector", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstVector(ScalarConstantVals : LLVMOpaqueValue ?? implicit; Size : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetConstOpcode", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetConstOpcode(ConstantVal : LLVMOpaqueValue? implicit) : LLVMOpcode {}
[extern(cdecl, name="LLVMAlignOf", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAlignOf(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSizeOf", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSizeOf(Ty : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNeg", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstNeg(ConstantVal : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNSWNeg", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstNSWNeg(ConstantVal : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNUWNeg", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstNUWNeg(ConstantVal : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNot", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstNot(ConstantVal : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstAdd", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstAdd(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNSWAdd", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstNSWAdd(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNUWAdd", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstNUWAdd(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstSub", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstSub(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNSWSub", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstNSWSub(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNUWSub", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstNUWSub(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstMul", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstMul(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNSWMul", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstNSWMul(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstNUWMul", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstNUWMul(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstAnd", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstAnd(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstOr", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstOr(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstXor", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstXor(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstICmp", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstICmp(Predicate : LLVMIntPredicate; LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstFCmp", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstFCmp(Predicate : LLVMRealPredicate; LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstShl", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstShl(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstLShr", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstLShr(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstAShr", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstAShr(LHSConstant : LLVMOpaqueValue? implicit; RHSConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstGEP2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstGEP2(Ty : LLVMOpaqueType? implicit; ConstantVal : LLVMOpaqueValue? implicit; ConstantIndices : LLVMOpaqueValue ?? implicit; NumIndices : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstInBoundsGEP2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstInBoundsGEP2(Ty : LLVMOpaqueType? implicit; ConstantVal : LLVMOpaqueValue? implicit; ConstantIndices : LLVMOpaqueValue ?? implicit; NumIndices : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstTrunc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstTrunc(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstSExt", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstSExt(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstZExt", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstZExt(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstFPTrunc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstFPTrunc(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstFPExt", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstFPExt(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstUIToFP", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstUIToFP(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstSIToFP", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstSIToFP(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstFPToUI", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstFPToUI(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstFPToSI", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstFPToSI(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstPtrToInt", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstPtrToInt(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstIntToPtr", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstIntToPtr(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstBitCast", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstBitCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstAddrSpaceCast", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstAddrSpaceCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstZExtOrBitCast", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstZExtOrBitCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstSExtOrBitCast", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstSExtOrBitCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstTruncOrBitCast", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstTruncOrBitCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstPointerCast", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstPointerCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstIntCast", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstIntCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit; isSigned : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstFPCast", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstFPCast(ConstantVal : LLVMOpaqueValue? implicit; ToType : LLVMOpaqueType? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstSelect", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstSelect(ConstantCondition : LLVMOpaqueValue? implicit; ConstantIfTrue : LLVMOpaqueValue? implicit; ConstantIfFalse : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstExtractElement", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstExtractElement(VectorConstant : LLVMOpaqueValue? implicit; IndexConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstInsertElement", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstInsertElement(VectorConstant : LLVMOpaqueValue? implicit; ElementValueConstant : LLVMOpaqueValue? implicit; IndexConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstShuffleVector", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstShuffleVector(VectorAConstant : LLVMOpaqueValue? implicit; VectorBConstant : LLVMOpaqueValue? implicit; MaskConstant : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBlockAddress", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBlockAddress(F : LLVMOpaqueValue? implicit; BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMConstInlineAsm", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConstInlineAsm(Ty : LLVMOpaqueType? implicit; AsmString : string implicit; Constraints : string implicit; HasSideEffects : int; IsAlignStack : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetGlobalParent", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetGlobalParent(Global : LLVMOpaqueValue? implicit) : LLVMOpaqueModule? {}
[extern(cdecl, name="LLVMIsDeclaration", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsDeclaration(Global : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMGetLinkage", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetLinkage(Global : LLVMOpaqueValue? implicit) : LLVMLinkage {}
[extern(cdecl, name="LLVMSetLinkage", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetLinkage(Global : LLVMOpaqueValue? implicit; Linkage : LLVMLinkage) : void {}
[extern(cdecl, name="LLVMGetSection", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetSection(Global : LLVMOpaqueValue? implicit) : string {}
[extern(cdecl, name="LLVMSetSection", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetSection(Global : LLVMOpaqueValue? implicit; Section : string implicit) : void {}
[extern(cdecl, name="LLVMGetVisibility", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetVisibility(Global : LLVMOpaqueValue? implicit) : LLVMVisibility {}
[extern(cdecl, name="LLVMSetVisibility", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetVisibility(Global : LLVMOpaqueValue? implicit; Viz : LLVMVisibility) : void {}
[extern(cdecl, name="LLVMGetDLLStorageClass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetDLLStorageClass(Global : LLVMOpaqueValue? implicit) : LLVMDLLStorageClass {}
[extern(cdecl, name="LLVMSetDLLStorageClass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetDLLStorageClass(Global : LLVMOpaqueValue? implicit; Class : LLVMDLLStorageClass) : void {}
[extern(cdecl, name="LLVMGetUnnamedAddress", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetUnnamedAddress(Global : LLVMOpaqueValue? implicit) : LLVMUnnamedAddr {}
[extern(cdecl, name="LLVMSetUnnamedAddress", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetUnnamedAddress(Global : LLVMOpaqueValue? implicit; UnnamedAddr : LLVMUnnamedAddr) : void {}
[extern(cdecl, name="LLVMGlobalGetValueType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGlobalGetValueType(Global : LLVMOpaqueValue? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMHasUnnamedAddr", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMHasUnnamedAddr(Global : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetUnnamedAddr", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetUnnamedAddr(Global : LLVMOpaqueValue? implicit; HasUnnamedAddr : int) : void {}
[extern(cdecl, name="LLVMGetAlignment", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetAlignment(V : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMSetAlignment", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetAlignment(V : LLVMOpaqueValue? implicit; Bytes : uint) : void {}
[extern(cdecl, name="LLVMGlobalSetMetadata", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGlobalSetMetadata(Global : LLVMOpaqueValue? implicit; Kind : uint; MD : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMGlobalEraseMetadata", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGlobalEraseMetadata(Global : LLVMOpaqueValue? implicit; Kind : uint) : void {}
[extern(cdecl, name="LLVMGlobalClearMetadata", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGlobalClearMetadata(Global : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGlobalCopyAllMetadata", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGlobalCopyAllMetadata(Value : LLVMOpaqueValue? implicit; NumEntries : uint64? implicit) : LLVMOpaqueValueMetadataEntry? {}
[extern(cdecl, name="LLVMDisposeValueMetadataEntries", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeValueMetadataEntries(Entries : LLVMOpaqueValueMetadataEntry? implicit) : void {}
[extern(cdecl, name="LLVMValueMetadataEntriesGetKind", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMValueMetadataEntriesGetKind(Entries : LLVMOpaqueValueMetadataEntry? implicit; Index : uint) : uint {}
[extern(cdecl, name="LLVMValueMetadataEntriesGetMetadata", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMValueMetadataEntriesGetMetadata(Entries : LLVMOpaqueValueMetadataEntry? implicit; Index : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMAddGlobal", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddGlobal(M : LLVMOpaqueModule? implicit; Ty : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMAddGlobalInAddressSpace", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddGlobalInAddressSpace(M : LLVMOpaqueModule? implicit; Ty : LLVMOpaqueType? implicit; Name : string implicit; AddressSpace : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNamedGlobal", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNamedGlobal(M : LLVMOpaqueModule? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetFirstGlobal", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetFirstGlobal(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetLastGlobal", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetLastGlobal(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNextGlobal", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNextGlobal(GlobalVar : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPreviousGlobal", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetPreviousGlobal(GlobalVar : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMDeleteGlobal", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDeleteGlobal(GlobalVar : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetInitializer", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetInitializer(GlobalVar : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetInitializer", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetInitializer(GlobalVar : LLVMOpaqueValue? implicit; ConstantVal : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMIsThreadLocal", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsThreadLocal(GlobalVar : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetThreadLocal", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetThreadLocal(GlobalVar : LLVMOpaqueValue? implicit; IsThreadLocal : int) : void {}
[extern(cdecl, name="LLVMIsGlobalConstant", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsGlobalConstant(GlobalVar : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetGlobalConstant", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetGlobalConstant(GlobalVar : LLVMOpaqueValue? implicit; IsConstant : int) : void {}
[extern(cdecl, name="LLVMGetThreadLocalMode", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetThreadLocalMode(GlobalVar : LLVMOpaqueValue? implicit) : LLVMThreadLocalMode {}
[extern(cdecl, name="LLVMSetThreadLocalMode", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetThreadLocalMode(GlobalVar : LLVMOpaqueValue? implicit; Mode : LLVMThreadLocalMode) : void {}
[extern(cdecl, name="LLVMIsExternallyInitialized", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsExternallyInitialized(GlobalVar : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetExternallyInitialized", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetExternallyInitialized(GlobalVar : LLVMOpaqueValue? implicit; IsExtInit : int) : void {}
[extern(cdecl, name="LLVMAddAlias2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddAlias2(M : LLVMOpaqueModule? implicit; ValueTy : LLVMOpaqueType? implicit; AddrSpace : uint; Aliasee : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNamedGlobalAlias", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNamedGlobalAlias(M : LLVMOpaqueModule? implicit; Name : string implicit; NameLen : uint64) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetFirstGlobalAlias", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetFirstGlobalAlias(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetLastGlobalAlias", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetLastGlobalAlias(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNextGlobalAlias", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNextGlobalAlias(GA : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPreviousGlobalAlias", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetPreviousGlobalAlias(GA : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMAliasGetAliasee", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAliasGetAliasee(Alias : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMAliasSetAliasee", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAliasSetAliasee(Alias : LLVMOpaqueValue? implicit; Aliasee : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMDeleteFunction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDeleteFunction(Fn : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMHasPersonalityFn", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMHasPersonalityFn(Fn : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMGetPersonalityFn", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetPersonalityFn(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetPersonalityFn", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetPersonalityFn(Fn : LLVMOpaqueValue? implicit; PersonalityFn : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMLookupIntrinsicID", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMLookupIntrinsicID(Name : string implicit; NameLen : uint64) : uint {}
[extern(cdecl, name="LLVMGetIntrinsicID", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetIntrinsicID(Fn : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetIntrinsicDeclaration", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetIntrinsicDeclaration(Mod : LLVMOpaqueModule? implicit; ID : uint; ParamTypes : LLVMOpaqueType ?? implicit; ParamCount : uint64) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIntrinsicGetType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIntrinsicGetType(Ctx : LLVMOpaqueContext? implicit; ID : uint; ParamTypes : LLVMOpaqueType ?? implicit; ParamCount : uint64) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIntrinsicGetName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIntrinsicGetName(ID : uint; NameLength : uint64? implicit) : string {}
[extern(cdecl, name="LLVMIntrinsicCopyOverloadedName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIntrinsicCopyOverloadedName(ID : uint; ParamTypes : LLVMOpaqueType ?? implicit; ParamCount : uint64; NameLength : uint64? implicit) : string {}
[extern(cdecl, name="LLVMIntrinsicCopyOverloadedName2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIntrinsicCopyOverloadedName2(Mod : LLVMOpaqueModule? implicit; ID : uint; ParamTypes : LLVMOpaqueType ?? implicit; ParamCount : uint64; NameLength : uint64? implicit) : string {}
[extern(cdecl, name="LLVMIntrinsicIsOverloaded", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIntrinsicIsOverloaded(ID : uint) : int {}
[extern(cdecl, name="LLVMGetFunctionCallConv", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetFunctionCallConv(Fn : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMSetFunctionCallConv", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetFunctionCallConv(Fn : LLVMOpaqueValue? implicit; CC : uint) : void {}
[extern(cdecl, name="LLVMGetGC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetGC(Fn : LLVMOpaqueValue? implicit) : string {}
[extern(cdecl, name="LLVMSetGC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetGC(Fn : LLVMOpaqueValue? implicit; Name : string implicit) : void {}
[extern(cdecl, name="LLVMAddAttributeAtIndex", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddAttributeAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint; A : LLVMOpaqueAttributeRef? implicit) : void {}
[extern(cdecl, name="LLVMGetAttributeCountAtIndex", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetAttributeCountAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint) : uint {}
[extern(cdecl, name="LLVMGetAttributesAtIndex", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetAttributesAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint; Attrs : LLVMOpaqueAttributeRef ?? implicit) : void {}
[extern(cdecl, name="LLVMGetEnumAttributeAtIndex", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetEnumAttributeAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint; KindID : uint) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMGetStringAttributeAtIndex", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetStringAttributeAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint; K : string implicit; KLen : uint) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMRemoveEnumAttributeAtIndex", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemoveEnumAttributeAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint; KindID : uint) : void {}
[extern(cdecl, name="LLVMRemoveStringAttributeAtIndex", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemoveStringAttributeAtIndex(F : LLVMOpaqueValue? implicit; Idx : uint; K : string implicit; KLen : uint) : void {}
[extern(cdecl, name="LLVMAddTargetDependentFunctionAttr", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddTargetDependentFunctionAttr(Fn : LLVMOpaqueValue? implicit; A : string implicit; V : string implicit) : void {}
[extern(cdecl, name="LLVMCountParams", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCountParams(Fn : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetParams", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetParams(Fn : LLVMOpaqueValue? implicit; Params : LLVMOpaqueValue ?? implicit) : void {}
[extern(cdecl, name="LLVMGetParam", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetParam(Fn : LLVMOpaqueValue? implicit; Index : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetParamParent", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetParamParent(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetFirstParam", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetFirstParam(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetLastParam", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetLastParam(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNextParam", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNextParam(Arg : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPreviousParam", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetPreviousParam(Arg : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetParamAlignment", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetParamAlignment(Arg : LLVMOpaqueValue? implicit; Align : uint) : void {}
[extern(cdecl, name="LLVMAddGlobalIFunc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddGlobalIFunc(M : LLVMOpaqueModule? implicit; Name : string implicit; NameLen : uint64; Ty : LLVMOpaqueType? implicit; AddrSpace : uint; Resolver : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNamedGlobalIFunc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNamedGlobalIFunc(M : LLVMOpaqueModule? implicit; Name : string implicit; NameLen : uint64) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetFirstGlobalIFunc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetFirstGlobalIFunc(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetLastGlobalIFunc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetLastGlobalIFunc(M : LLVMOpaqueModule? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNextGlobalIFunc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNextGlobalIFunc(IFunc : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPreviousGlobalIFunc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetPreviousGlobalIFunc(IFunc : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetGlobalIFuncResolver", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetGlobalIFuncResolver(IFunc : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetGlobalIFuncResolver", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetGlobalIFuncResolver(IFunc : LLVMOpaqueValue? implicit; Resolver : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMEraseGlobalIFunc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMEraseGlobalIFunc(IFunc : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMRemoveGlobalIFunc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemoveGlobalIFunc(IFunc : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMMDStringInContext2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMMDStringInContext2(C : LLVMOpaqueContext? implicit; Str : string implicit; SLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMMDNodeInContext2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMMDNodeInContext2(C : LLVMOpaqueContext? implicit; MDs : LLVMOpaqueMetadata ?? implicit; Count : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMMetadataAsValue", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMMetadataAsValue(C : LLVMOpaqueContext? implicit; MD : LLVMOpaqueMetadata? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMValueAsMetadata", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMValueAsMetadata(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMGetMDString", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetMDString(V : LLVMOpaqueValue? implicit; Length : uint? implicit) : string {}
[extern(cdecl, name="LLVMGetMDNodeNumOperands", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetMDNodeNumOperands(V : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetMDNodeOperands", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetMDNodeOperands(V : LLVMOpaqueValue? implicit; Dest : LLVMOpaqueValue ?? implicit) : void {}
[extern(cdecl, name="LLVMMDStringInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMMDStringInContext(C : LLVMOpaqueContext? implicit; Str : string implicit; SLen : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMMDString", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMMDString(Str : string implicit; SLen : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMMDNodeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMMDNodeInContext(C : LLVMOpaqueContext? implicit; Vals : LLVMOpaqueValue ?? implicit; Count : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMMDNode", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMMDNode(Vals : LLVMOpaqueValue ?? implicit; Count : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBasicBlockAsValue", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBasicBlockAsValue(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMValueIsBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMValueIsBasicBlock(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMValueAsBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMValueAsBasicBlock(Val : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetBasicBlockName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetBasicBlockName(BB : LLVMOpaqueBasicBlock? implicit) : string {}
[extern(cdecl, name="LLVMGetBasicBlockParent", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetBasicBlockParent(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetBasicBlockTerminator", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetBasicBlockTerminator(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMCountBasicBlocks", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCountBasicBlocks(Fn : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetBasicBlocks", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetBasicBlocks(Fn : LLVMOpaqueValue? implicit; BasicBlocks : LLVMOpaqueBasicBlock ?? implicit) : void {}
[extern(cdecl, name="LLVMGetFirstBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetFirstBasicBlock(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetLastBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetLastBasicBlock(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetNextBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNextBasicBlock(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetPreviousBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetPreviousBasicBlock(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetEntryBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetEntryBasicBlock(Fn : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMInsertExistingBasicBlockAfterInsertBlock", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInsertExistingBasicBlockAfterInsertBlock(Builder : LLVMOpaqueBuilder? implicit; BB : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMAppendExistingBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAppendExistingBasicBlock(Fn : LLVMOpaqueValue? implicit; BB : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMCreateBasicBlockInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateBasicBlockInContext(C : LLVMOpaqueContext? implicit; Name : string implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMAppendBasicBlockInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAppendBasicBlockInContext(C : LLVMOpaqueContext? implicit; Fn : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMAppendBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAppendBasicBlock(Fn : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMInsertBasicBlockInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInsertBasicBlockInContext(C : LLVMOpaqueContext? implicit; BB : LLVMOpaqueBasicBlock? implicit; Name : string implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMInsertBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInsertBasicBlock(InsertBeforeBB : LLVMOpaqueBasicBlock? implicit; Name : string implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMDeleteBasicBlock", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDeleteBasicBlock(BB : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMRemoveBasicBlockFromParent", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemoveBasicBlockFromParent(BB : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMMoveBasicBlockBefore", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMMoveBasicBlockBefore(BB : LLVMOpaqueBasicBlock? implicit; MovePos : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMMoveBasicBlockAfter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMMoveBasicBlockAfter(BB : LLVMOpaqueBasicBlock? implicit; MovePos : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMGetFirstInstruction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetFirstInstruction(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetLastInstruction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetLastInstruction(BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMHasMetadata", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMHasMetadata(Val : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMGetMetadata", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetMetadata(Val : LLVMOpaqueValue? implicit; KindID : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetMetadata", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetMetadata(Val : LLVMOpaqueValue? implicit; KindID : uint; Node : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMInstructionGetAllMetadataOtherThanDebugLoc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInstructionGetAllMetadataOtherThanDebugLoc(Instr : LLVMOpaqueValue? implicit; NumEntries : uint64? implicit) : LLVMOpaqueValueMetadataEntry? {}
[extern(cdecl, name="LLVMGetInstructionParent", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetInstructionParent(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetNextInstruction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNextInstruction(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetPreviousInstruction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetPreviousInstruction(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMInstructionRemoveFromParent", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInstructionRemoveFromParent(Inst : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMInstructionEraseFromParent", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInstructionEraseFromParent(Inst : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMDeleteInstruction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDeleteInstruction(Inst : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetInstructionOpcode", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetInstructionOpcode(Inst : LLVMOpaqueValue? implicit) : LLVMOpcode {}
[extern(cdecl, name="LLVMGetICmpPredicate", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetICmpPredicate(Inst : LLVMOpaqueValue? implicit) : LLVMIntPredicate {}
[extern(cdecl, name="LLVMGetFCmpPredicate", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetFCmpPredicate(Inst : LLVMOpaqueValue? implicit) : LLVMRealPredicate {}
[extern(cdecl, name="LLVMInstructionClone", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInstructionClone(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsATerminatorInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsATerminatorInst(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNumArgOperands", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNumArgOperands(Instr : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMSetInstructionCallConv", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetInstructionCallConv(Instr : LLVMOpaqueValue? implicit; CC : uint) : void {}
[extern(cdecl, name="LLVMGetInstructionCallConv", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetInstructionCallConv(Instr : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMSetInstrParamAlignment", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetInstrParamAlignment(Instr : LLVMOpaqueValue? implicit; Idx : uint; Align : uint) : void {}
[extern(cdecl, name="LLVMAddCallSiteAttribute", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddCallSiteAttribute(C : LLVMOpaqueValue? implicit; Idx : uint; A : LLVMOpaqueAttributeRef? implicit) : void {}
[extern(cdecl, name="LLVMGetCallSiteAttributeCount", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetCallSiteAttributeCount(C : LLVMOpaqueValue? implicit; Idx : uint) : uint {}
[extern(cdecl, name="LLVMGetCallSiteAttributes", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetCallSiteAttributes(C : LLVMOpaqueValue? implicit; Idx : uint; Attrs : LLVMOpaqueAttributeRef ?? implicit) : void {}
[extern(cdecl, name="LLVMGetCallSiteEnumAttribute", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetCallSiteEnumAttribute(C : LLVMOpaqueValue? implicit; Idx : uint; KindID : uint) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMGetCallSiteStringAttribute", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetCallSiteStringAttribute(C : LLVMOpaqueValue? implicit; Idx : uint; K : string implicit; KLen : uint) : LLVMOpaqueAttributeRef? {}
[extern(cdecl, name="LLVMRemoveCallSiteEnumAttribute", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemoveCallSiteEnumAttribute(C : LLVMOpaqueValue? implicit; Idx : uint; KindID : uint) : void {}
[extern(cdecl, name="LLVMRemoveCallSiteStringAttribute", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemoveCallSiteStringAttribute(C : LLVMOpaqueValue? implicit; Idx : uint; K : string implicit; KLen : uint) : void {}
[extern(cdecl, name="LLVMGetCalledFunctionType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetCalledFunctionType(C : LLVMOpaqueValue? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMGetCalledValue", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetCalledValue(Instr : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMIsTailCall", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsTailCall(CallInst : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetTailCall", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetTailCall(CallInst : LLVMOpaqueValue? implicit; IsTailCall : int) : void {}
[extern(cdecl, name="LLVMGetNormalDest", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNormalDest(InvokeInst : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetUnwindDest", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetUnwindDest(InvokeInst : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMSetNormalDest", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetNormalDest(InvokeInst : LLVMOpaqueValue? implicit; B : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMSetUnwindDest", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetUnwindDest(InvokeInst : LLVMOpaqueValue? implicit; B : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMGetNumSuccessors", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNumSuccessors(Term : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetSuccessor", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetSuccessor(Term : LLVMOpaqueValue? implicit; i : uint) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMSetSuccessor", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetSuccessor(Term : LLVMOpaqueValue? implicit; i : uint; _block : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMIsConditional", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsConditional(Branch : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMGetCondition", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetCondition(Branch : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetCondition", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetCondition(Branch : LLVMOpaqueValue? implicit; Cond : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetSwitchDefaultDest", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetSwitchDefaultDest(SwitchInstr : LLVMOpaqueValue? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetAllocatedType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetAllocatedType(Alloca : LLVMOpaqueValue? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIsInBounds", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsInBounds(GEP : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetIsInBounds", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetIsInBounds(GEP : LLVMOpaqueValue? implicit; InBounds : int) : void {}
[extern(cdecl, name="LLVMGetGEPSourceElementType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetGEPSourceElementType(GEP : LLVMOpaqueValue? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMAddIncoming", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddIncoming(PhiNode : LLVMOpaqueValue? implicit; IncomingValues : LLVMOpaqueValue ?? implicit; IncomingBlocks : LLVMOpaqueBasicBlock ?? implicit; Count : uint) : void {}
[extern(cdecl, name="LLVMCountIncoming", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCountIncoming(PhiNode : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetIncomingValue", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetIncomingValue(PhiNode : LLVMOpaqueValue? implicit; Index : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetIncomingBlock", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetIncomingBlock(PhiNode : LLVMOpaqueValue? implicit; Index : uint) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMGetNumIndices", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNumIndices(Inst : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetIndices", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetIndices(Inst : LLVMOpaqueValue? implicit) : uint const? {}
[extern(cdecl, name="LLVMCreateBuilderInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateBuilderInContext(C : LLVMOpaqueContext? implicit) : LLVMOpaqueBuilder? {}
[extern(cdecl, name="LLVMCreateBuilder", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateBuilder() : LLVMOpaqueBuilder? {}
[extern(cdecl, name="LLVMPositionBuilder", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPositionBuilder(Builder : LLVMOpaqueBuilder? implicit; Block : LLVMOpaqueBasicBlock? implicit; Instr : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMPositionBuilderBefore", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPositionBuilderBefore(Builder : LLVMOpaqueBuilder? implicit; Instr : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMPositionBuilderAtEnd", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPositionBuilderAtEnd(Builder : LLVMOpaqueBuilder? implicit; Block : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMGetInsertBlock", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetInsertBlock(Builder : LLVMOpaqueBuilder? implicit) : LLVMOpaqueBasicBlock? {}
[extern(cdecl, name="LLVMClearInsertionPosition", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMClearInsertionPosition(Builder : LLVMOpaqueBuilder? implicit) : void {}
[extern(cdecl, name="LLVMInsertIntoBuilder", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInsertIntoBuilder(Builder : LLVMOpaqueBuilder? implicit; Instr : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMInsertIntoBuilderWithName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInsertIntoBuilderWithName(Builder : LLVMOpaqueBuilder? implicit; Instr : LLVMOpaqueValue? implicit; Name : string implicit) : void {}
[extern(cdecl, name="LLVMDisposeBuilder", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeBuilder(Builder : LLVMOpaqueBuilder? implicit) : void {}
[extern(cdecl, name="LLVMGetCurrentDebugLocation2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetCurrentDebugLocation2(Builder : LLVMOpaqueBuilder? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMSetCurrentDebugLocation2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetCurrentDebugLocation2(Builder : LLVMOpaqueBuilder? implicit; Loc : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMSetInstDebugLocation", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetInstDebugLocation(Builder : LLVMOpaqueBuilder? implicit; Inst : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMAddMetadataToInst", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddMetadataToInst(Builder : LLVMOpaqueBuilder? implicit; Inst : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMBuilderGetDefaultFPMathTag", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuilderGetDefaultFPMathTag(Builder : LLVMOpaqueBuilder? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMBuilderSetDefaultFPMathTag", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuilderSetDefaultFPMathTag(Builder : LLVMOpaqueBuilder? implicit; FPMathTag : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMSetCurrentDebugLocation", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetCurrentDebugLocation(Builder : LLVMOpaqueBuilder? implicit; L : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetCurrentDebugLocation", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetCurrentDebugLocation(Builder : LLVMOpaqueBuilder? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildRetVoid", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildRetVoid(_argument_0x0 : LLVMOpaqueBuilder? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildRet", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildRet(_argument_0x0 : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAggregateRet", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildAggregateRet(_argument_0x0 : LLVMOpaqueBuilder? implicit; RetVals : LLVMOpaqueValue ?? implicit; N : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildBr", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildBr(_argument_0x0 : LLVMOpaqueBuilder? implicit; Dest : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCondBr", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildCondBr(_argument_0x0 : LLVMOpaqueBuilder? implicit; If : LLVMOpaqueValue? implicit; Then : LLVMOpaqueBasicBlock? implicit; Else : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSwitch", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildSwitch(_argument_0x0 : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit; Else : LLVMOpaqueBasicBlock? implicit; NumCases : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildIndirectBr", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildIndirectBr(B : LLVMOpaqueBuilder? implicit; Addr : LLVMOpaqueValue? implicit; NumDests : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildInvoke2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildInvoke2(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Fn : LLVMOpaqueValue? implicit; Args : LLVMOpaqueValue ?? implicit; NumArgs : uint; Then : LLVMOpaqueBasicBlock? implicit; Catch : LLVMOpaqueBasicBlock? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildUnreachable", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildUnreachable(_argument_0x0 : LLVMOpaqueBuilder? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildResume", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildResume(B : LLVMOpaqueBuilder? implicit; Exn : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildLandingPad", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildLandingPad(B : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; PersFn : LLVMOpaqueValue? implicit; NumClauses : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCleanupRet", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildCleanupRet(B : LLVMOpaqueBuilder? implicit; CatchPad : LLVMOpaqueValue? implicit; BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCatchRet", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildCatchRet(B : LLVMOpaqueBuilder? implicit; CatchPad : LLVMOpaqueValue? implicit; BB : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCatchPad", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildCatchPad(B : LLVMOpaqueBuilder? implicit; ParentPad : LLVMOpaqueValue? implicit; Args : LLVMOpaqueValue ?? implicit; NumArgs : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCleanupPad", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildCleanupPad(B : LLVMOpaqueBuilder? implicit; ParentPad : LLVMOpaqueValue? implicit; Args : LLVMOpaqueValue ?? implicit; NumArgs : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCatchSwitch", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildCatchSwitch(B : LLVMOpaqueBuilder? implicit; ParentPad : LLVMOpaqueValue? implicit; UnwindBB : LLVMOpaqueBasicBlock? implicit; NumHandlers : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMAddCase", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddCase(Switch : LLVMOpaqueValue? implicit; OnVal : LLVMOpaqueValue? implicit; Dest : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMAddDestination", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddDestination(IndirectBr : LLVMOpaqueValue? implicit; Dest : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMGetNumClauses", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNumClauses(LandingPad : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetClause", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetClause(LandingPad : LLVMOpaqueValue? implicit; Idx : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMAddClause", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddClause(LandingPad : LLVMOpaqueValue? implicit; ClauseVal : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMIsCleanup", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsCleanup(LandingPad : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetCleanup", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetCleanup(LandingPad : LLVMOpaqueValue? implicit; Val : int) : void {}
[extern(cdecl, name="LLVMAddHandler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddHandler(CatchSwitch : LLVMOpaqueValue? implicit; Dest : LLVMOpaqueBasicBlock? implicit) : void {}
[extern(cdecl, name="LLVMGetNumHandlers", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNumHandlers(CatchSwitch : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetHandlers", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetHandlers(CatchSwitch : LLVMOpaqueValue? implicit; Handlers : LLVMOpaqueBasicBlock ?? implicit) : void {}
[extern(cdecl, name="LLVMGetArgOperand", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetArgOperand(Funclet : LLVMOpaqueValue? implicit; i : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetArgOperand", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetArgOperand(Funclet : LLVMOpaqueValue? implicit; i : uint; value : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMGetParentCatchSwitch", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetParentCatchSwitch(CatchPad : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMSetParentCatchSwitch", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetParentCatchSwitch(CatchPad : LLVMOpaqueValue? implicit; CatchSwitch : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMBuildAdd", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildAdd(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNSWAdd", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildNSWAdd(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNUWAdd", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildNUWAdd(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFAdd", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildFAdd(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSub", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildSub(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNSWSub", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildNSWSub(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNUWSub", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildNUWSub(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFSub", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildFSub(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildMul", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildMul(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNSWMul", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildNSWMul(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNUWMul", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildNUWMul(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFMul", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildFMul(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildUDiv", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildUDiv(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildExactUDiv", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildExactUDiv(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSDiv", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildSDiv(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildExactSDiv", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildExactSDiv(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFDiv", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildFDiv(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildURem", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildURem(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSRem", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildSRem(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFRem", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildFRem(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildShl", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildShl(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildLShr", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildLShr(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAShr", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildAShr(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAnd", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildAnd(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildOr", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildOr(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildXor", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildXor(_argument_0x0 : LLVMOpaqueBuilder? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildBinOp", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildBinOp(B : LLVMOpaqueBuilder? implicit; Op : LLVMOpcode; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNeg", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildNeg(_argument_0x0 : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNSWNeg", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildNSWNeg(B : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNUWNeg", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildNUWNeg(B : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFNeg", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildFNeg(_argument_0x0 : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildNot", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildNot(_argument_0x0 : LLVMOpaqueBuilder? implicit; V : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildMalloc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildMalloc(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildArrayMalloc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildArrayMalloc(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Val : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildMemSet", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildMemSet(B : LLVMOpaqueBuilder? implicit; Ptr : LLVMOpaqueValue? implicit; Val : LLVMOpaqueValue? implicit; Len : LLVMOpaqueValue? implicit; Align : uint) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildMemCpy", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildMemCpy(B : LLVMOpaqueBuilder? implicit; Dst : LLVMOpaqueValue? implicit; DstAlign : uint; Src : LLVMOpaqueValue? implicit; SrcAlign : uint; Size : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildMemMove", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildMemMove(B : LLVMOpaqueBuilder? implicit; Dst : LLVMOpaqueValue? implicit; DstAlign : uint; Src : LLVMOpaqueValue? implicit; SrcAlign : uint; Size : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAlloca", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildAlloca(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildArrayAlloca", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildArrayAlloca(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Val : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFree", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildFree(_argument_0x0 : LLVMOpaqueBuilder? implicit; PointerVal : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildLoad2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildLoad2(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; PointerVal : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildStore", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildStore(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; Ptr : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildGEP2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildGEP2(B : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Pointer : LLVMOpaqueValue? implicit; Indices : LLVMOpaqueValue ?? implicit; NumIndices : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildInBoundsGEP2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildInBoundsGEP2(B : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Pointer : LLVMOpaqueValue? implicit; Indices : LLVMOpaqueValue ?? implicit; NumIndices : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildStructGEP2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildStructGEP2(B : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Pointer : LLVMOpaqueValue? implicit; Idx : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildGlobalString", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildGlobalString(B : LLVMOpaqueBuilder? implicit; Str : string implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildGlobalStringPtr", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildGlobalStringPtr(B : LLVMOpaqueBuilder? implicit; Str : string implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetVolatile", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetVolatile(MemoryAccessInst : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetVolatile", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetVolatile(MemoryAccessInst : LLVMOpaqueValue? implicit; IsVolatile : int) : void {}
[extern(cdecl, name="LLVMGetWeak", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetWeak(CmpXchgInst : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetWeak", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetWeak(CmpXchgInst : LLVMOpaqueValue? implicit; IsWeak : int) : void {}
[extern(cdecl, name="LLVMGetOrdering", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetOrdering(MemoryAccessInst : LLVMOpaqueValue? implicit) : LLVMAtomicOrdering {}
[extern(cdecl, name="LLVMSetOrdering", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetOrdering(MemoryAccessInst : LLVMOpaqueValue? implicit; Ordering : LLVMAtomicOrdering) : void {}
[extern(cdecl, name="LLVMGetAtomicRMWBinOp", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetAtomicRMWBinOp(AtomicRMWInst : LLVMOpaqueValue? implicit) : LLVMAtomicRMWBinOp {}
[extern(cdecl, name="LLVMSetAtomicRMWBinOp", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetAtomicRMWBinOp(AtomicRMWInst : LLVMOpaqueValue? implicit; BinOp : LLVMAtomicRMWBinOp) : void {}
[extern(cdecl, name="LLVMBuildTrunc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildTrunc(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildZExt", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildZExt(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSExt", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildSExt(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFPToUI", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildFPToUI(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFPToSI", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildFPToSI(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildUIToFP", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildUIToFP(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSIToFP", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildSIToFP(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFPTrunc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildFPTrunc(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFPExt", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildFPExt(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildPtrToInt", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildPtrToInt(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildIntToPtr", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildIntToPtr(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildBitCast", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildBitCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAddrSpaceCast", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildAddrSpaceCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildZExtOrBitCast", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildZExtOrBitCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSExtOrBitCast", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildSExtOrBitCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildTruncOrBitCast", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildTruncOrBitCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCast", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildCast(B : LLVMOpaqueBuilder? implicit; Op : LLVMOpcode; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildPointerCast", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildPointerCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildIntCast2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildIntCast2(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; IsSigned : int; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFPCast", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildFPCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildIntCast", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildIntCast(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; DestTy : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetCastOpcode", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetCastOpcode(Src : LLVMOpaqueValue? implicit; SrcIsSigned : int; DestTy : LLVMOpaqueType? implicit; DestIsSigned : int) : LLVMOpcode {}
[extern(cdecl, name="LLVMBuildICmp", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildICmp(_argument_0x0 : LLVMOpaqueBuilder? implicit; Op : LLVMIntPredicate; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFCmp", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildFCmp(_argument_0x0 : LLVMOpaqueBuilder? implicit; Op : LLVMRealPredicate; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildPhi", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildPhi(_argument_0x0 : LLVMOpaqueBuilder? implicit; Ty : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildCall2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildCall2(_argument_0x0 : LLVMOpaqueBuilder? implicit; _argument_0x1 : LLVMOpaqueType? implicit; Fn : LLVMOpaqueValue? implicit; Args : LLVMOpaqueValue ?? implicit; NumArgs : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildSelect", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildSelect(_argument_0x0 : LLVMOpaqueBuilder? implicit; If : LLVMOpaqueValue? implicit; Then : LLVMOpaqueValue? implicit; Else : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildVAArg", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildVAArg(_argument_0x0 : LLVMOpaqueBuilder? implicit; List : LLVMOpaqueValue? implicit; Ty : LLVMOpaqueType? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildExtractElement", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildExtractElement(_argument_0x0 : LLVMOpaqueBuilder? implicit; VecVal : LLVMOpaqueValue? implicit; Index : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildInsertElement", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildInsertElement(_argument_0x0 : LLVMOpaqueBuilder? implicit; VecVal : LLVMOpaqueValue? implicit; EltVal : LLVMOpaqueValue? implicit; Index : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildShuffleVector", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildShuffleVector(_argument_0x0 : LLVMOpaqueBuilder? implicit; V1 : LLVMOpaqueValue? implicit; V2 : LLVMOpaqueValue? implicit; Mask : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildExtractValue", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildExtractValue(_argument_0x0 : LLVMOpaqueBuilder? implicit; AggVal : LLVMOpaqueValue? implicit; Index : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildInsertValue", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildInsertValue(_argument_0x0 : LLVMOpaqueBuilder? implicit; AggVal : LLVMOpaqueValue? implicit; EltVal : LLVMOpaqueValue? implicit; Index : uint; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFreeze", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildFreeze(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildIsNull", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildIsNull(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildIsNotNull", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildIsNotNull(_argument_0x0 : LLVMOpaqueBuilder? implicit; Val : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildPtrDiff2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildPtrDiff2(_argument_0x0 : LLVMOpaqueBuilder? implicit; ElemTy : LLVMOpaqueType? implicit; LHS : LLVMOpaqueValue? implicit; RHS : LLVMOpaqueValue? implicit; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildFence", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildFence(B : LLVMOpaqueBuilder? implicit; ordering : LLVMAtomicOrdering; singleThread : int; Name : string implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAtomicRMW", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildAtomicRMW(B : LLVMOpaqueBuilder? implicit; op : LLVMAtomicRMWBinOp; PTR : LLVMOpaqueValue? implicit; Val : LLVMOpaqueValue? implicit; ordering : LLVMAtomicOrdering; singleThread : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMBuildAtomicCmpXchg", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBuildAtomicCmpXchg(B : LLVMOpaqueBuilder? implicit; Ptr : LLVMOpaqueValue? implicit; Cmp : LLVMOpaqueValue? implicit; New : LLVMOpaqueValue? implicit; SuccessOrdering : LLVMAtomicOrdering; FailureOrdering : LLVMAtomicOrdering; SingleThread : int) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMGetNumMaskElements", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNumMaskElements(ShuffleVectorInst : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMGetUndefMaskElem", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetUndefMaskElem() : int {}
[extern(cdecl, name="LLVMGetMaskValue", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetMaskValue(ShuffleVectorInst : LLVMOpaqueValue? implicit; Elt : uint) : int {}
[extern(cdecl, name="LLVMIsAtomicSingleThread", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsAtomicSingleThread(AtomicInst : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMSetAtomicSingleThread", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetAtomicSingleThread(AtomicInst : LLVMOpaqueValue? implicit; SingleThread : int) : void {}
[extern(cdecl, name="LLVMGetCmpXchgSuccessOrdering", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetCmpXchgSuccessOrdering(CmpXchgInst : LLVMOpaqueValue? implicit) : LLVMAtomicOrdering {}
[extern(cdecl, name="LLVMSetCmpXchgSuccessOrdering", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetCmpXchgSuccessOrdering(CmpXchgInst : LLVMOpaqueValue? implicit; Ordering : LLVMAtomicOrdering) : void {}
[extern(cdecl, name="LLVMGetCmpXchgFailureOrdering", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetCmpXchgFailureOrdering(CmpXchgInst : LLVMOpaqueValue? implicit) : LLVMAtomicOrdering {}
[extern(cdecl, name="LLVMSetCmpXchgFailureOrdering", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetCmpXchgFailureOrdering(CmpXchgInst : LLVMOpaqueValue? implicit; Ordering : LLVMAtomicOrdering) : void {}
[extern(cdecl, name="LLVMCreateModuleProviderForExistingModule", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateModuleProviderForExistingModule(M : LLVMOpaqueModule? implicit) : LLVMOpaqueModuleProvider? {}
[extern(cdecl, name="LLVMDisposeModuleProvider", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeModuleProvider(M : LLVMOpaqueModuleProvider? implicit) : void {}
[extern(cdecl, name="LLVMCreateMemoryBufferWithContentsOfFile", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateMemoryBufferWithContentsOfFile(Path : string implicit; OutMemBuf : LLVMOpaqueMemoryBuffer ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMCreateMemoryBufferWithSTDIN", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateMemoryBufferWithSTDIN(OutMemBuf : LLVMOpaqueMemoryBuffer ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMCreateMemoryBufferWithMemoryRange", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateMemoryBufferWithMemoryRange(InputData : string implicit; InputDataLength : uint64; BufferName : string implicit; RequiresNullTerminator : int) : LLVMOpaqueMemoryBuffer? {}
[extern(cdecl, name="LLVMCreateMemoryBufferWithMemoryRangeCopy", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateMemoryBufferWithMemoryRangeCopy(InputData : string implicit; InputDataLength : uint64; BufferName : string implicit) : LLVMOpaqueMemoryBuffer? {}
[extern(cdecl, name="LLVMGetBufferStart", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetBufferStart(MemBuf : LLVMOpaqueMemoryBuffer? implicit) : string {}
[extern(cdecl, name="LLVMGetBufferSize", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetBufferSize(MemBuf : LLVMOpaqueMemoryBuffer? implicit) : uint64 {}
[extern(cdecl, name="LLVMDisposeMemoryBuffer", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeMemoryBuffer(MemBuf : LLVMOpaqueMemoryBuffer? implicit) : void {}
[extern(cdecl, name="LLVMGetGlobalPassRegistry", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetGlobalPassRegistry() : LLVMOpaquePassRegistry? {}
[extern(cdecl, name="LLVMCreatePassManager", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreatePassManager() : LLVMOpaquePassManager? {}
[extern(cdecl, name="LLVMCreateFunctionPassManagerForModule", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateFunctionPassManagerForModule(M : LLVMOpaqueModule? implicit) : LLVMOpaquePassManager? {}
[extern(cdecl, name="LLVMCreateFunctionPassManager", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateFunctionPassManager(MP : LLVMOpaqueModuleProvider? implicit) : LLVMOpaquePassManager? {}
[extern(cdecl, name="LLVMRunPassManager", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRunPassManager(PM : LLVMOpaquePassManager? implicit; M : LLVMOpaqueModule? implicit) : int {}
[extern(cdecl, name="LLVMInitializeFunctionPassManager", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeFunctionPassManager(FPM : LLVMOpaquePassManager? implicit) : int {}
[extern(cdecl, name="LLVMRunFunctionPassManager", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRunFunctionPassManager(FPM : LLVMOpaquePassManager? implicit; F : LLVMOpaqueValue? implicit) : int {}
[extern(cdecl, name="LLVMFinalizeFunctionPassManager", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMFinalizeFunctionPassManager(FPM : LLVMOpaquePassManager? implicit) : int {}
[extern(cdecl, name="LLVMDisposePassManager", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposePassManager(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMStartMultithreaded", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMStartMultithreaded() : int {}
[extern(cdecl, name="LLVMStopMultithreaded", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMStopMultithreaded() : void {}
[extern(cdecl, name="LLVMIsMultithreaded", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsMultithreaded() : int {}
[extern(cdecl, name="LLVMDebugMetadataVersion", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDebugMetadataVersion() : uint {}
[extern(cdecl, name="LLVMGetModuleDebugMetadataVersion", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetModuleDebugMetadataVersion(Module : LLVMOpaqueModule? implicit) : uint {}
[extern(cdecl, name="LLVMStripModuleDebugInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMStripModuleDebugInfo(Module : LLVMOpaqueModule? implicit) : int {}
[extern(cdecl, name="LLVMCreateDIBuilderDisallowUnresolved", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateDIBuilderDisallowUnresolved(M : LLVMOpaqueModule? implicit) : LLVMOpaqueDIBuilder? {}
[extern(cdecl, name="LLVMCreateDIBuilder", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateDIBuilder(M : LLVMOpaqueModule? implicit) : LLVMOpaqueDIBuilder? {}
[extern(cdecl, name="LLVMDisposeDIBuilder", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeDIBuilder(Builder : LLVMOpaqueDIBuilder? implicit) : void {}
[extern(cdecl, name="LLVMDIBuilderFinalize", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderFinalize(Builder : LLVMOpaqueDIBuilder? implicit) : void {}
[extern(cdecl, name="LLVMDIBuilderFinalizeSubprogram", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderFinalizeSubprogram(Builder : LLVMOpaqueDIBuilder? implicit; Subprogram : LLVMOpaqueMetadata? implicit) : void {}
// Function `LLVMDIBuilderCreateCompileUnit` has too many arguments. Ignore.

[extern(cdecl, name="LLVMDIBuilderCreateFile", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateFile(Builder : LLVMOpaqueDIBuilder? implicit; Filename : string implicit; FilenameLen : uint64; Directory : string implicit; DirectoryLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateModule", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateModule(Builder : LLVMOpaqueDIBuilder? implicit; ParentScope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; ConfigMacros : string implicit; ConfigMacrosLen : uint64; IncludePath : string implicit; IncludePathLen : uint64; APINotesFile : string implicit; APINotesFileLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateNameSpace", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateNameSpace(Builder : LLVMOpaqueDIBuilder? implicit; ParentScope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; ExportSymbols : int) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateFunction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateFunction(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; LinkageName : string implicit; LinkageNameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; Ty : LLVMOpaqueMetadata? implicit; IsLocalToUnit : int; IsDefinition : int; ScopeLine : uint; Flags : LLVMDIFlags; IsOptimized : int) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateLexicalBlock", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateLexicalBlock(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint; Column : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateLexicalBlockFile", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateLexicalBlockFile(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Discriminator : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateImportedModuleFromNamespace", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateImportedModuleFromNamespace(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; NS : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateImportedModuleFromAlias", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateImportedModuleFromAlias(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; ImportedEntity : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint; Elements : LLVMOpaqueMetadata ?? implicit; NumElements : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateImportedModuleFromModule", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateImportedModuleFromModule(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; M : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint; Elements : LLVMOpaqueMetadata ?? implicit; NumElements : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateImportedDeclaration", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateImportedDeclaration(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Decl : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint; Name : string implicit; NameLen : uint64; Elements : LLVMOpaqueMetadata ?? implicit; NumElements : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateDebugLocation", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateDebugLocation(Ctx : LLVMOpaqueContext? implicit; Line : uint; Column : uint; Scope : LLVMOpaqueMetadata? implicit; InlinedAt : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDILocationGetLine", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDILocationGetLine(Location : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMDILocationGetColumn", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDILocationGetColumn(Location : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMDILocationGetScope", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDILocationGetScope(Location : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDILocationGetInlinedAt", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDILocationGetInlinedAt(Location : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIScopeGetFile", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIScopeGetFile(Scope : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIFileGetDirectory", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIFileGetDirectory(File : LLVMOpaqueMetadata? implicit; Len : uint? implicit) : string {}
[extern(cdecl, name="LLVMDIFileGetFilename", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIFileGetFilename(File : LLVMOpaqueMetadata? implicit; Len : uint? implicit) : string {}
[extern(cdecl, name="LLVMDIFileGetSource", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIFileGetSource(File : LLVMOpaqueMetadata? implicit; Len : uint? implicit) : string {}
[extern(cdecl, name="LLVMDIBuilderGetOrCreateTypeArray", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderGetOrCreateTypeArray(Builder : LLVMOpaqueDIBuilder? implicit; Data : LLVMOpaqueMetadata ?? implicit; NumElements : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateSubroutineType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateSubroutineType(Builder : LLVMOpaqueDIBuilder? implicit; File : LLVMOpaqueMetadata? implicit; ParameterTypes : LLVMOpaqueMetadata ?? implicit; NumParameterTypes : uint; Flags : LLVMDIFlags) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateMacro", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateMacro(Builder : LLVMOpaqueDIBuilder? implicit; ParentMacroFile : LLVMOpaqueMetadata? implicit; Line : uint; RecordType : LLVMDWARFMacinfoRecordType; Name : string implicit; NameLen : uint64; Value : string implicit; ValueLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateTempMacroFile", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateTempMacroFile(Builder : LLVMOpaqueDIBuilder? implicit; ParentMacroFile : LLVMOpaqueMetadata? implicit; Line : uint; File : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateEnumerator", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateEnumerator(Builder : LLVMOpaqueDIBuilder? implicit; Name : string implicit; NameLen : uint64; Value : int64; IsUnsigned : int) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateEnumerationType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateEnumerationType(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNumber : uint; SizeInBits : uint64; AlignInBits : uint; Elements : LLVMOpaqueMetadata ?? implicit; NumElements : uint; ClassTy : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateUnionType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateUnionType(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNumber : uint; SizeInBits : uint64; AlignInBits : uint; Flags : LLVMDIFlags; Elements : LLVMOpaqueMetadata ?? implicit; NumElements : uint; RunTimeLang : uint; UniqueId : string implicit; UniqueIdLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateArrayType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateArrayType(Builder : LLVMOpaqueDIBuilder? implicit; Size : uint64; AlignInBits : uint; Ty : LLVMOpaqueMetadata? implicit; Subscripts : LLVMOpaqueMetadata ?? implicit; NumSubscripts : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateVectorType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateVectorType(Builder : LLVMOpaqueDIBuilder? implicit; Size : uint64; AlignInBits : uint; Ty : LLVMOpaqueMetadata? implicit; Subscripts : LLVMOpaqueMetadata ?? implicit; NumSubscripts : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateUnspecifiedType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateUnspecifiedType(Builder : LLVMOpaqueDIBuilder? implicit; Name : string implicit; NameLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateBasicType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateBasicType(Builder : LLVMOpaqueDIBuilder? implicit; Name : string implicit; NameLen : uint64; SizeInBits : uint64; Encoding : uint; Flags : LLVMDIFlags) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreatePointerType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreatePointerType(Builder : LLVMOpaqueDIBuilder? implicit; PointeeTy : LLVMOpaqueMetadata? implicit; SizeInBits : uint64; AlignInBits : uint; AddressSpace : uint; Name : string implicit; NameLen : uint64) : LLVMOpaqueMetadata? {}
// Function `LLVMDIBuilderCreateStructType` has too many arguments. Ignore.

[extern(cdecl, name="LLVMDIBuilderCreateMemberType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateMemberType(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; SizeInBits : uint64; AlignInBits : uint; OffsetInBits : uint64; Flags : LLVMDIFlags; Ty : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateStaticMemberType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateStaticMemberType(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNumber : uint; Type : LLVMOpaqueMetadata? implicit; Flags : LLVMDIFlags; ConstantVal : LLVMOpaqueValue? implicit; AlignInBits : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateMemberPointerType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateMemberPointerType(Builder : LLVMOpaqueDIBuilder? implicit; PointeeType : LLVMOpaqueMetadata? implicit; ClassType : LLVMOpaqueMetadata? implicit; SizeInBits : uint64; AlignInBits : uint; Flags : LLVMDIFlags) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateObjCIVar", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateObjCIVar(Builder : LLVMOpaqueDIBuilder? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; SizeInBits : uint64; AlignInBits : uint; OffsetInBits : uint64; Flags : LLVMDIFlags; Ty : LLVMOpaqueMetadata? implicit; PropertyNode : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateObjCProperty", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateObjCProperty(Builder : LLVMOpaqueDIBuilder? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; GetterName : string implicit; GetterNameLen : uint64; SetterName : string implicit; SetterNameLen : uint64; PropertyAttributes : uint; Ty : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateObjectPointerType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateObjectPointerType(Builder : LLVMOpaqueDIBuilder? implicit; Type : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateQualifiedType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateQualifiedType(Builder : LLVMOpaqueDIBuilder? implicit; Tag : uint; Type : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateReferenceType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateReferenceType(Builder : LLVMOpaqueDIBuilder? implicit; Tag : uint; Type : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateNullPtrType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateNullPtrType(Builder : LLVMOpaqueDIBuilder? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateTypedef", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateTypedef(Builder : LLVMOpaqueDIBuilder? implicit; Type : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; Scope : LLVMOpaqueMetadata? implicit; AlignInBits : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateInheritance", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateInheritance(Builder : LLVMOpaqueDIBuilder? implicit; Ty : LLVMOpaqueMetadata? implicit; BaseTy : LLVMOpaqueMetadata? implicit; BaseOffset : uint64; VBPtrOffset : uint; Flags : LLVMDIFlags) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateForwardDecl", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateForwardDecl(Builder : LLVMOpaqueDIBuilder? implicit; Tag : uint; Name : string implicit; NameLen : uint64; Scope : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint; RuntimeLang : uint; SizeInBits : uint64; AlignInBits : uint; UniqueIdentifier : string implicit; UniqueIdentifierLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateReplaceableCompositeType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateReplaceableCompositeType(Builder : LLVMOpaqueDIBuilder? implicit; Tag : uint; Name : string implicit; NameLen : uint64; Scope : LLVMOpaqueMetadata? implicit; File : LLVMOpaqueMetadata? implicit; Line : uint; RuntimeLang : uint; SizeInBits : uint64; AlignInBits : uint; Flags : LLVMDIFlags; UniqueIdentifier : string implicit; UniqueIdentifierLen : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateBitFieldMemberType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateBitFieldMemberType(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNumber : uint; SizeInBits : uint64; OffsetInBits : uint64; StorageOffsetInBits : uint64; Flags : LLVMDIFlags; Type : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
// Function `LLVMDIBuilderCreateClassType` has too many arguments. Ignore.

[extern(cdecl, name="LLVMDIBuilderCreateArtificialType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateArtificialType(Builder : LLVMOpaqueDIBuilder? implicit; Type : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDITypeGetName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDITypeGetName(DType : LLVMOpaqueMetadata? implicit; Length : uint64? implicit) : string {}
[extern(cdecl, name="LLVMDITypeGetSizeInBits", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDITypeGetSizeInBits(DType : LLVMOpaqueMetadata? implicit) : uint64 {}
[extern(cdecl, name="LLVMDITypeGetOffsetInBits", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDITypeGetOffsetInBits(DType : LLVMOpaqueMetadata? implicit) : uint64 {}
[extern(cdecl, name="LLVMDITypeGetAlignInBits", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDITypeGetAlignInBits(DType : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMDITypeGetLine", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDITypeGetLine(DType : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMDITypeGetFlags", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDITypeGetFlags(DType : LLVMOpaqueMetadata? implicit) : LLVMDIFlags {}
[extern(cdecl, name="LLVMDIBuilderGetOrCreateSubrange", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderGetOrCreateSubrange(Builder : LLVMOpaqueDIBuilder? implicit; LowerBound : int64; Count : int64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderGetOrCreateArray", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderGetOrCreateArray(Builder : LLVMOpaqueDIBuilder? implicit; Data : LLVMOpaqueMetadata ?? implicit; NumElements : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateExpression", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateExpression(Builder : LLVMOpaqueDIBuilder? implicit; Addr : uint64? implicit; Length : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateConstantValueExpression", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateConstantValueExpression(Builder : LLVMOpaqueDIBuilder? implicit; Value : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateGlobalVariableExpression", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateGlobalVariableExpression(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; Linkage : string implicit; LinkLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; Ty : LLVMOpaqueMetadata? implicit; LocalToUnit : int; Expr : LLVMOpaqueMetadata? implicit; Decl : LLVMOpaqueMetadata? implicit; AlignInBits : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIGlobalVariableExpressionGetVariable", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIGlobalVariableExpressionGetVariable(GVE : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIGlobalVariableExpressionGetExpression", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIGlobalVariableExpressionGetExpression(GVE : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIVariableGetFile", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIVariableGetFile(Var : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIVariableGetScope", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIVariableGetScope(Var : LLVMOpaqueMetadata? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIVariableGetLine", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIVariableGetLine(Var : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMTemporaryMDNode", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMTemporaryMDNode(Ctx : LLVMOpaqueContext? implicit; Data : LLVMOpaqueMetadata ?? implicit; NumElements : uint64) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDisposeTemporaryMDNode", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeTemporaryMDNode(TempNode : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMMetadataReplaceAllUsesWith", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMMetadataReplaceAllUsesWith(TempTargetMetadata : LLVMOpaqueMetadata? implicit; Replacement : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMDIBuilderCreateTempGlobalVariableFwdDecl", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateTempGlobalVariableFwdDecl(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; Linkage : string implicit; LnkLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; Ty : LLVMOpaqueMetadata? implicit; LocalToUnit : int; Decl : LLVMOpaqueMetadata? implicit; AlignInBits : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderInsertDeclareBefore", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderInsertDeclareBefore(Builder : LLVMOpaqueDIBuilder? implicit; Storage : LLVMOpaqueValue? implicit; VarInfo : LLVMOpaqueMetadata? implicit; Expr : LLVMOpaqueMetadata? implicit; DebugLoc : LLVMOpaqueMetadata? implicit; Instr : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMDIBuilderInsertDeclareAtEnd", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderInsertDeclareAtEnd(Builder : LLVMOpaqueDIBuilder? implicit; Storage : LLVMOpaqueValue? implicit; VarInfo : LLVMOpaqueMetadata? implicit; Expr : LLVMOpaqueMetadata? implicit; DebugLoc : LLVMOpaqueMetadata? implicit; Block : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMDIBuilderInsertDbgValueBefore", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderInsertDbgValueBefore(Builder : LLVMOpaqueDIBuilder? implicit; Val : LLVMOpaqueValue? implicit; VarInfo : LLVMOpaqueMetadata? implicit; Expr : LLVMOpaqueMetadata? implicit; DebugLoc : LLVMOpaqueMetadata? implicit; Instr : LLVMOpaqueValue? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMDIBuilderInsertDbgValueAtEnd", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderInsertDbgValueAtEnd(Builder : LLVMOpaqueDIBuilder? implicit; Val : LLVMOpaqueValue? implicit; VarInfo : LLVMOpaqueMetadata? implicit; Expr : LLVMOpaqueMetadata? implicit; DebugLoc : LLVMOpaqueMetadata? implicit; Block : LLVMOpaqueBasicBlock? implicit) : LLVMOpaqueValue? {}
[extern(cdecl, name="LLVMDIBuilderCreateAutoVariable", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateAutoVariable(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; File : LLVMOpaqueMetadata? implicit; LineNo : uint; Ty : LLVMOpaqueMetadata? implicit; AlwaysPreserve : int; Flags : LLVMDIFlags; AlignInBits : uint) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMDIBuilderCreateParameterVariable", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDIBuilderCreateParameterVariable(Builder : LLVMOpaqueDIBuilder? implicit; Scope : LLVMOpaqueMetadata? implicit; Name : string implicit; NameLen : uint64; ArgNo : uint; File : LLVMOpaqueMetadata? implicit; LineNo : uint; Ty : LLVMOpaqueMetadata? implicit; AlwaysPreserve : int; Flags : LLVMDIFlags) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMGetSubprogram", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetSubprogram(Func : LLVMOpaqueValue? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMSetSubprogram", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetSubprogram(Func : LLVMOpaqueValue? implicit; SP : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMDISubprogramGetLine", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDISubprogramGetLine(Subprogram : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMInstructionGetDebugLoc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInstructionGetDebugLoc(Inst : LLVMOpaqueValue? implicit) : LLVMOpaqueMetadata? {}
[extern(cdecl, name="LLVMInstructionSetDebugLoc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInstructionSetDebugLoc(Inst : LLVMOpaqueValue? implicit; Loc : LLVMOpaqueMetadata? implicit) : void {}
[extern(cdecl, name="LLVMGetMetadataKind", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetMetadataKind(Metadata : LLVMOpaqueMetadata? implicit) : uint {}
[extern(cdecl, name="LLVMSetDisasmOptions", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetDisasmOptions(DC : void? implicit; Options : uint64) : int {}
[extern(cdecl, name="LLVMDisasmDispose", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisasmDispose(DC : void? implicit) : void {}
[extern(cdecl, name="LLVMDisasmInstruction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisasmInstruction(DC : void? implicit; Bytes : uint8? implicit; BytesSize : uint64; PC : uint64; OutString : string implicit; OutStringSize : uint64) : uint64 {}
[extern(cdecl, name="LLVMGetErrorTypeId", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetErrorTypeId(Err : LLVMOpaqueError? implicit) : void const? {}
[extern(cdecl, name="LLVMConsumeError", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMConsumeError(Err : LLVMOpaqueError? implicit) : void {}
[extern(cdecl, name="LLVMGetErrorMessage", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetErrorMessage(Err : LLVMOpaqueError? implicit) : string {}
[extern(cdecl, name="LLVMDisposeErrorMessage", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeErrorMessage(ErrMsg : string implicit) : void {}
[extern(cdecl, name="LLVMGetStringErrorTypeId", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetStringErrorTypeId() : void const? {}
[extern(cdecl, name="LLVMCreateStringError", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateStringError(ErrMsg : string implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMInitializeAArch64TargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeAArch64TargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeAMDGPUTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeAMDGPUTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeARMTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeARMTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeAVRTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeAVRTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeBPFTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeBPFTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeHexagonTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeHexagonTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeLanaiTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeLanaiTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeLoongArchTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeLoongArchTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeMipsTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeMipsTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeMSP430TargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeMSP430TargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeNVPTXTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeNVPTXTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializePowerPCTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializePowerPCTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeRISCVTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeRISCVTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeSparcTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeSparcTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeSystemZTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeSystemZTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeVETargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeVETargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeWebAssemblyTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeWebAssemblyTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeX86TargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeX86TargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeXCoreTargetInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeXCoreTargetInfo() : void {}
[extern(cdecl, name="LLVMInitializeAArch64Target", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeAArch64Target() : void {}
[extern(cdecl, name="LLVMInitializeAMDGPUTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeAMDGPUTarget() : void {}
[extern(cdecl, name="LLVMInitializeARMTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeARMTarget() : void {}
[extern(cdecl, name="LLVMInitializeAVRTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeAVRTarget() : void {}
[extern(cdecl, name="LLVMInitializeBPFTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeBPFTarget() : void {}
[extern(cdecl, name="LLVMInitializeHexagonTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeHexagonTarget() : void {}
[extern(cdecl, name="LLVMInitializeLanaiTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeLanaiTarget() : void {}
[extern(cdecl, name="LLVMInitializeLoongArchTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeLoongArchTarget() : void {}
[extern(cdecl, name="LLVMInitializeMipsTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeMipsTarget() : void {}
[extern(cdecl, name="LLVMInitializeMSP430Target", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeMSP430Target() : void {}
[extern(cdecl, name="LLVMInitializeNVPTXTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeNVPTXTarget() : void {}
[extern(cdecl, name="LLVMInitializePowerPCTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializePowerPCTarget() : void {}
[extern(cdecl, name="LLVMInitializeRISCVTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeRISCVTarget() : void {}
[extern(cdecl, name="LLVMInitializeSparcTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeSparcTarget() : void {}
[extern(cdecl, name="LLVMInitializeSystemZTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeSystemZTarget() : void {}
[extern(cdecl, name="LLVMInitializeVETarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeVETarget() : void {}
[extern(cdecl, name="LLVMInitializeWebAssemblyTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeWebAssemblyTarget() : void {}
[extern(cdecl, name="LLVMInitializeX86Target", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeX86Target() : void {}
[extern(cdecl, name="LLVMInitializeXCoreTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeXCoreTarget() : void {}
[extern(cdecl, name="LLVMInitializeAArch64TargetMC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeAArch64TargetMC() : void {}
[extern(cdecl, name="LLVMInitializeAMDGPUTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeAMDGPUTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeARMTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeARMTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeAVRTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeAVRTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeBPFTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeBPFTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeHexagonTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeHexagonTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeLanaiTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeLanaiTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeLoongArchTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeLoongArchTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeMipsTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeMipsTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeMSP430TargetMC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeMSP430TargetMC() : void {}
[extern(cdecl, name="LLVMInitializeNVPTXTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeNVPTXTargetMC() : void {}
[extern(cdecl, name="LLVMInitializePowerPCTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializePowerPCTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeRISCVTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeRISCVTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeSparcTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeSparcTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeSystemZTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeSystemZTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeVETargetMC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeVETargetMC() : void {}
[extern(cdecl, name="LLVMInitializeWebAssemblyTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeWebAssemblyTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeX86TargetMC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeX86TargetMC() : void {}
[extern(cdecl, name="LLVMInitializeXCoreTargetMC", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeXCoreTargetMC() : void {}
[extern(cdecl, name="LLVMInitializeAArch64AsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeAArch64AsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeAMDGPUAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeAMDGPUAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeARMAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeARMAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeAVRAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeAVRAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeBPFAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeBPFAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeHexagonAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeHexagonAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeLanaiAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeLanaiAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeLoongArchAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeLoongArchAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeMipsAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeMipsAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeMSP430AsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeMSP430AsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeNVPTXAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeNVPTXAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializePowerPCAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializePowerPCAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeRISCVAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeRISCVAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeSparcAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeSparcAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeSystemZAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeSystemZAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeVEAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeVEAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeWebAssemblyAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeWebAssemblyAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeX86AsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeX86AsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeXCoreAsmPrinter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeXCoreAsmPrinter() : void {}
[extern(cdecl, name="LLVMInitializeAArch64AsmParser", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeAArch64AsmParser() : void {}
[extern(cdecl, name="LLVMInitializeAMDGPUAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeAMDGPUAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeARMAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeARMAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeAVRAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeAVRAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeBPFAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeBPFAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeHexagonAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeHexagonAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeLanaiAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeLanaiAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeLoongArchAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeLoongArchAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeMipsAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeMipsAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeMSP430AsmParser", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeMSP430AsmParser() : void {}
[extern(cdecl, name="LLVMInitializePowerPCAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializePowerPCAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeRISCVAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeRISCVAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeSparcAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeSparcAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeSystemZAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeSystemZAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeVEAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeVEAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeWebAssemblyAsmParser", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeWebAssemblyAsmParser() : void {}
[extern(cdecl, name="LLVMInitializeX86AsmParser", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeX86AsmParser() : void {}
[extern(cdecl, name="LLVMInitializeAArch64Disassembler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeAArch64Disassembler() : void {}
[extern(cdecl, name="LLVMInitializeAMDGPUDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeAMDGPUDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeARMDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeARMDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeAVRDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeAVRDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeBPFDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeBPFDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeHexagonDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeHexagonDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeLanaiDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeLanaiDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeLoongArchDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeLoongArchDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeMipsDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeMipsDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeMSP430Disassembler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeMSP430Disassembler() : void {}
[extern(cdecl, name="LLVMInitializePowerPCDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializePowerPCDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeRISCVDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeRISCVDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeSparcDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeSparcDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeSystemZDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeSystemZDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeVEDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeVEDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeWebAssemblyDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeWebAssemblyDisassembler() : void {}
[extern(cdecl, name="LLVMInitializeX86Disassembler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeX86Disassembler() : void {}
[extern(cdecl, name="LLVMInitializeXCoreDisassembler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeXCoreDisassembler() : void {}
[extern(cdecl, name="LLVMGetModuleDataLayout", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetModuleDataLayout(M : LLVMOpaqueModule? implicit) : LLVMOpaqueTargetData? {}
[extern(cdecl, name="LLVMSetModuleDataLayout", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetModuleDataLayout(M : LLVMOpaqueModule? implicit; DL : LLVMOpaqueTargetData? implicit) : void {}
[extern(cdecl, name="LLVMCreateTargetData", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateTargetData(StringRep : string implicit) : LLVMOpaqueTargetData? {}
[extern(cdecl, name="LLVMDisposeTargetData", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeTargetData(TD : LLVMOpaqueTargetData? implicit) : void {}
[extern(cdecl, name="LLVMAddTargetLibraryInfo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddTargetLibraryInfo(TLI : LLVMOpaqueTargetLibraryInfotData? implicit; PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMCopyStringRepOfTargetData", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCopyStringRepOfTargetData(TD : LLVMOpaqueTargetData? implicit) : string {}
[extern(cdecl, name="LLVMByteOrder", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMByteOrder(TD : LLVMOpaqueTargetData? implicit) : LLVMByteOrdering {}
[extern(cdecl, name="LLVMPointerSize", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPointerSize(TD : LLVMOpaqueTargetData? implicit) : uint {}
[extern(cdecl, name="LLVMPointerSizeForAS", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPointerSizeForAS(TD : LLVMOpaqueTargetData? implicit; AS : uint) : uint {}
[extern(cdecl, name="LLVMIntPtrType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIntPtrType(TD : LLVMOpaqueTargetData? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIntPtrTypeForAS", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIntPtrTypeForAS(TD : LLVMOpaqueTargetData? implicit; AS : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIntPtrTypeInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIntPtrTypeInContext(C : LLVMOpaqueContext? implicit; TD : LLVMOpaqueTargetData? implicit) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMIntPtrTypeForASInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIntPtrTypeForASInContext(C : LLVMOpaqueContext? implicit; TD : LLVMOpaqueTargetData? implicit; AS : uint) : LLVMOpaqueType? {}
[extern(cdecl, name="LLVMSizeOfTypeInBits", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSizeOfTypeInBits(TD : LLVMOpaqueTargetData? implicit; Ty : LLVMOpaqueType? implicit) : uint64 {}
[extern(cdecl, name="LLVMStoreSizeOfType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMStoreSizeOfType(TD : LLVMOpaqueTargetData? implicit; Ty : LLVMOpaqueType? implicit) : uint64 {}
[extern(cdecl, name="LLVMABISizeOfType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMABISizeOfType(TD : LLVMOpaqueTargetData? implicit; Ty : LLVMOpaqueType? implicit) : uint64 {}
[extern(cdecl, name="LLVMABIAlignmentOfType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMABIAlignmentOfType(TD : LLVMOpaqueTargetData? implicit; Ty : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMCallFrameAlignmentOfType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCallFrameAlignmentOfType(TD : LLVMOpaqueTargetData? implicit; Ty : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMPreferredAlignmentOfType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPreferredAlignmentOfType(TD : LLVMOpaqueTargetData? implicit; Ty : LLVMOpaqueType? implicit) : uint {}
[extern(cdecl, name="LLVMPreferredAlignmentOfGlobal", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPreferredAlignmentOfGlobal(TD : LLVMOpaqueTargetData? implicit; GlobalVar : LLVMOpaqueValue? implicit) : uint {}
[extern(cdecl, name="LLVMElementAtOffset", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMElementAtOffset(TD : LLVMOpaqueTargetData? implicit; StructTy : LLVMOpaqueType? implicit; Offset : uint64) : uint {}
[extern(cdecl, name="LLVMOffsetOfElement", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOffsetOfElement(TD : LLVMOpaqueTargetData? implicit; StructTy : LLVMOpaqueType? implicit; Element : uint) : uint64 {}
[extern(cdecl, name="LLVMGetFirstTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetFirstTarget() : LLVMTarget? {}
[extern(cdecl, name="LLVMGetNextTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetNextTarget(T : LLVMTarget? implicit) : LLVMTarget? {}
[extern(cdecl, name="LLVMGetTargetFromName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetTargetFromName(Name : string implicit) : LLVMTarget? {}
[extern(cdecl, name="LLVMGetTargetFromTriple", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetTargetFromTriple(Triple : string implicit; T : LLVMTarget ?? implicit; ErrorMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMGetTargetName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetTargetName(T : LLVMTarget? implicit) : string {}
[extern(cdecl, name="LLVMGetTargetDescription", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetTargetDescription(T : LLVMTarget? implicit) : string {}
[extern(cdecl, name="LLVMTargetHasJIT", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMTargetHasJIT(T : LLVMTarget? implicit) : int {}
[extern(cdecl, name="LLVMTargetHasTargetMachine", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMTargetHasTargetMachine(T : LLVMTarget? implicit) : int {}
[extern(cdecl, name="LLVMTargetHasAsmBackend", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMTargetHasAsmBackend(T : LLVMTarget? implicit) : int {}
[extern(cdecl, name="LLVMCreateTargetMachine", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateTargetMachine(T : LLVMTarget? implicit; Triple : string implicit; CPU : string implicit; Features : string implicit; Level : LLVMCodeGenOptLevel; Reloc : LLVMRelocMode; CodeModel : LLVMCodeModel) : LLVMOpaqueTargetMachine? {}
[extern(cdecl, name="LLVMDisposeTargetMachine", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeTargetMachine(T : LLVMOpaqueTargetMachine? implicit) : void {}
[extern(cdecl, name="LLVMGetTargetMachineTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetTargetMachineTarget(T : LLVMOpaqueTargetMachine? implicit) : LLVMTarget? {}
[extern(cdecl, name="LLVMGetTargetMachineTriple", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetTargetMachineTriple(T : LLVMOpaqueTargetMachine? implicit) : string {}
[extern(cdecl, name="LLVMGetTargetMachineCPU", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetTargetMachineCPU(T : LLVMOpaqueTargetMachine? implicit) : string {}
[extern(cdecl, name="LLVMGetTargetMachineFeatureString", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetTargetMachineFeatureString(T : LLVMOpaqueTargetMachine? implicit) : string {}
[extern(cdecl, name="LLVMCreateTargetDataLayout", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateTargetDataLayout(T : LLVMOpaqueTargetMachine? implicit) : LLVMOpaqueTargetData? {}
[extern(cdecl, name="LLVMSetTargetMachineAsmVerbosity", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSetTargetMachineAsmVerbosity(T : LLVMOpaqueTargetMachine? implicit; VerboseAsm : int) : void {}
[extern(cdecl, name="LLVMTargetMachineEmitToFile", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMTargetMachineEmitToFile(T : LLVMOpaqueTargetMachine? implicit; M : LLVMOpaqueModule? implicit; Filename : string implicit; codegen : LLVMCodeGenFileType; ErrorMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMTargetMachineEmitToMemoryBuffer", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMTargetMachineEmitToMemoryBuffer(T : LLVMOpaqueTargetMachine? implicit; M : LLVMOpaqueModule? implicit; codegen : LLVMCodeGenFileType; ErrorMessage : string? implicit; OutMemBuf : LLVMOpaqueMemoryBuffer ?? implicit) : int {}
[extern(cdecl, name="LLVMGetDefaultTargetTriple", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetDefaultTargetTriple() : string {}
[extern(cdecl, name="LLVMNormalizeTargetTriple", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMNormalizeTargetTriple(triple : string implicit) : string {}
[extern(cdecl, name="LLVMGetHostCPUName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetHostCPUName() : string {}
[extern(cdecl, name="LLVMGetHostCPUFeatures", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetHostCPUFeatures() : string {}
[extern(cdecl, name="LLVMAddAnalysisPasses", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddAnalysisPasses(T : LLVMOpaqueTargetMachine? implicit; PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMLinkInMCJIT", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMLinkInMCJIT() : void {}
[extern(cdecl, name="LLVMLinkInInterpreter", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMLinkInInterpreter() : void {}
[extern(cdecl, name="LLVMCreateGenericValueOfInt", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateGenericValueOfInt(Ty : LLVMOpaqueType? implicit; N : uint64; IsSigned : int) : LLVMOpaqueGenericValue? {}
[extern(cdecl, name="LLVMCreateGenericValueOfPointer", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateGenericValueOfPointer(P : void? implicit) : LLVMOpaqueGenericValue? {}
[extern(cdecl, name="LLVMCreateGenericValueOfFloat", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateGenericValueOfFloat(Ty : LLVMOpaqueType? implicit; N : double) : LLVMOpaqueGenericValue? {}
[extern(cdecl, name="LLVMGenericValueIntWidth", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGenericValueIntWidth(GenValRef : LLVMOpaqueGenericValue? implicit) : uint {}
[extern(cdecl, name="LLVMGenericValueToInt", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGenericValueToInt(GenVal : LLVMOpaqueGenericValue? implicit; IsSigned : int) : uint64 {}
[extern(cdecl, name="LLVMGenericValueToPointer", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGenericValueToPointer(GenVal : LLVMOpaqueGenericValue? implicit) : void? {}
[extern(cdecl, name="LLVMGenericValueToFloat", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGenericValueToFloat(TyRef : LLVMOpaqueType? implicit; GenVal : LLVMOpaqueGenericValue? implicit) : double {}
[extern(cdecl, name="LLVMDisposeGenericValue", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeGenericValue(GenVal : LLVMOpaqueGenericValue? implicit) : void {}
[extern(cdecl, name="LLVMCreateExecutionEngineForModule", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateExecutionEngineForModule(OutEE : LLVMOpaqueExecutionEngine ?? implicit; M : LLVMOpaqueModule? implicit; OutError : string? implicit) : int {}
[extern(cdecl, name="LLVMCreateInterpreterForModule", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateInterpreterForModule(OutInterp : LLVMOpaqueExecutionEngine ?? implicit; M : LLVMOpaqueModule? implicit; OutError : string? implicit) : int {}
[extern(cdecl, name="LLVMCreateJITCompilerForModule", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateJITCompilerForModule(OutJIT : LLVMOpaqueExecutionEngine ?? implicit; M : LLVMOpaqueModule? implicit; OptLevel : uint; OutError : string? implicit) : int {}
[extern(cdecl, name="LLVMInitializeMCJITCompilerOptions", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeMCJITCompilerOptions(Options : LLVMMCJITCompilerOptions? implicit; SizeOfOptions : uint64) : void {}
[extern(cdecl, name="LLVMCreateMCJITCompilerForModule", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateMCJITCompilerForModule(OutJIT : LLVMOpaqueExecutionEngine ?? implicit; M : LLVMOpaqueModule? implicit; Options : LLVMMCJITCompilerOptions? implicit; SizeOfOptions : uint64; OutError : string? implicit) : int {}
[extern(cdecl, name="LLVMDisposeExecutionEngine", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeExecutionEngine(EE : LLVMOpaqueExecutionEngine? implicit) : void {}
[extern(cdecl, name="LLVMRunStaticConstructors", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRunStaticConstructors(EE : LLVMOpaqueExecutionEngine? implicit) : void {}
[extern(cdecl, name="LLVMRunStaticDestructors", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRunStaticDestructors(EE : LLVMOpaqueExecutionEngine? implicit) : void {}
[extern(cdecl, name="LLVMRunFunctionAsMain", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRunFunctionAsMain(EE : LLVMOpaqueExecutionEngine? implicit; F : LLVMOpaqueValue? implicit; ArgC : uint; ArgV : string const? implicit; EnvP : string const? implicit) : int {}
[extern(cdecl, name="LLVMRunFunction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRunFunction(EE : LLVMOpaqueExecutionEngine? implicit; F : LLVMOpaqueValue? implicit; NumArgs : uint; Args : LLVMOpaqueGenericValue ?? implicit) : LLVMOpaqueGenericValue? {}
[extern(cdecl, name="LLVMFreeMachineCodeForFunction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMFreeMachineCodeForFunction(EE : LLVMOpaqueExecutionEngine? implicit; F : LLVMOpaqueValue? implicit) : void {}
[extern(cdecl, name="LLVMAddModule", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddModule(EE : LLVMOpaqueExecutionEngine? implicit; M : LLVMOpaqueModule? implicit) : void {}
[extern(cdecl, name="LLVMRemoveModule", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemoveModule(EE : LLVMOpaqueExecutionEngine? implicit; M : LLVMOpaqueModule? implicit; OutMod : LLVMOpaqueModule ?? implicit; OutError : string? implicit) : int {}
[extern(cdecl, name="LLVMFindFunction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMFindFunction(EE : LLVMOpaqueExecutionEngine? implicit; Name : string implicit; OutFn : LLVMOpaqueValue ?? implicit) : int {}
[extern(cdecl, name="LLVMRecompileAndRelinkFunction", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRecompileAndRelinkFunction(EE : LLVMOpaqueExecutionEngine? implicit; Fn : LLVMOpaqueValue? implicit) : void? {}
[extern(cdecl, name="LLVMGetExecutionEngineTargetData", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetExecutionEngineTargetData(EE : LLVMOpaqueExecutionEngine? implicit) : LLVMOpaqueTargetData? {}
[extern(cdecl, name="LLVMGetExecutionEngineTargetMachine", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetExecutionEngineTargetMachine(EE : LLVMOpaqueExecutionEngine? implicit) : LLVMOpaqueTargetMachine? {}
[extern(cdecl, name="LLVMAddGlobalMapping", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddGlobalMapping(EE : LLVMOpaqueExecutionEngine? implicit; Global : LLVMOpaqueValue? implicit; Addr : void? implicit) : void {}
[extern(cdecl, name="LLVMGetPointerToGlobal", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetPointerToGlobal(EE : LLVMOpaqueExecutionEngine? implicit; Global : LLVMOpaqueValue? implicit) : void? {}
[extern(cdecl, name="LLVMGetGlobalValueAddress", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetGlobalValueAddress(EE : LLVMOpaqueExecutionEngine? implicit; Name : string implicit) : uint64 {}
[extern(cdecl, name="LLVMGetFunctionAddress", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetFunctionAddress(EE : LLVMOpaqueExecutionEngine? implicit; Name : string implicit) : uint64 {}
[extern(cdecl, name="LLVMExecutionEngineGetErrMsg", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMExecutionEngineGetErrMsg(EE : LLVMOpaqueExecutionEngine? implicit; OutError : string? implicit) : int {}
[extern(cdecl, name="LLVMDisposeMCJITMemoryManager", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeMCJITMemoryManager(MM : LLVMOpaqueMCJITMemoryManager? implicit) : void {}
[extern(cdecl, name="LLVMCreateGDBRegistrationListener", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateGDBRegistrationListener() : LLVMOpaqueJITEventListener? {}
[extern(cdecl, name="LLVMCreateIntelJITEventListener", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateIntelJITEventListener() : LLVMOpaqueJITEventListener? {}
[extern(cdecl, name="LLVMCreateOProfileJITEventListener", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateOProfileJITEventListener() : LLVMOpaqueJITEventListener? {}
[extern(cdecl, name="LLVMCreatePerfJITEventListener", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreatePerfJITEventListener() : LLVMOpaqueJITEventListener? {}
[extern(cdecl, name="LLVMInitializeTransformUtils", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeTransformUtils(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeScalarOpts", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeScalarOpts(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeVectorization", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeVectorization(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeInstCombine", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeInstCombine(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeIPO", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeIPO(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeAnalysis", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeAnalysis(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeIPA", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeIPA(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeCodeGen", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeCodeGen(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMInitializeTarget", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMInitializeTarget(R : LLVMOpaquePassRegistry? implicit) : void {}
[extern(cdecl, name="LLVMParseIRInContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMParseIRInContext(ContextRef : LLVMOpaqueContext? implicit; MemBuf : LLVMOpaqueMemoryBuffer? implicit; OutM : LLVMOpaqueModule ?? implicit; OutMessage : string? implicit) : int {}
[extern(cdecl, name="LLVMLinkModules2", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMLinkModules2(Dest : LLVMOpaqueModule? implicit; Src : LLVMOpaqueModule? implicit) : int {}
[extern(cdecl, name="LLVMOrcExecutionSessionGetSymbolStringPool", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcExecutionSessionGetSymbolStringPool(ES : LLVMOrcOpaqueExecutionSession? implicit) : LLVMOrcOpaqueSymbolStringPool? {}
[extern(cdecl, name="LLVMOrcSymbolStringPoolClearDeadEntries", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcSymbolStringPoolClearDeadEntries(SSP : LLVMOrcOpaqueSymbolStringPool? implicit) : void {}
[extern(cdecl, name="LLVMOrcExecutionSessionIntern", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcExecutionSessionIntern(ES : LLVMOrcOpaqueExecutionSession? implicit; Name : string implicit) : LLVMOrcOpaqueSymbolStringPoolEntry? {}
[extern(cdecl, name="LLVMOrcRetainSymbolStringPoolEntry", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcRetainSymbolStringPoolEntry(S : LLVMOrcOpaqueSymbolStringPoolEntry? implicit) : void {}
[extern(cdecl, name="LLVMOrcReleaseSymbolStringPoolEntry", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcReleaseSymbolStringPoolEntry(S : LLVMOrcOpaqueSymbolStringPoolEntry? implicit) : void {}
[extern(cdecl, name="LLVMOrcSymbolStringPoolEntryStr", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcSymbolStringPoolEntryStr(S : LLVMOrcOpaqueSymbolStringPoolEntry? implicit) : string {}
[extern(cdecl, name="LLVMOrcReleaseResourceTracker", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcReleaseResourceTracker(RT : LLVMOrcOpaqueResourceTracker? implicit) : void {}
[extern(cdecl, name="LLVMOrcResourceTrackerTransferTo", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcResourceTrackerTransferTo(SrcRT : LLVMOrcOpaqueResourceTracker? implicit; DstRT : LLVMOrcOpaqueResourceTracker? implicit) : void {}
[extern(cdecl, name="LLVMOrcResourceTrackerRemove", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcResourceTrackerRemove(RT : LLVMOrcOpaqueResourceTracker? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcDisposeDefinitionGenerator", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcDisposeDefinitionGenerator(DG : LLVMOrcOpaqueDefinitionGenerator? implicit) : void {}
[extern(cdecl, name="LLVMOrcDisposeMaterializationUnit", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcDisposeMaterializationUnit(MU : LLVMOrcOpaqueMaterializationUnit? implicit) : void {}
[extern(cdecl, name="LLVMOrcAbsoluteSymbols", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcAbsoluteSymbols(Syms : LLVMOrcCSymbolMapPair? implicit; NumPairs : uint64) : LLVMOrcOpaqueMaterializationUnit? {}
[extern(cdecl, name="LLVMOrcLazyReexports", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcLazyReexports(LCTM : LLVMOrcOpaqueLazyCallThroughManager? implicit; ISM : LLVMOrcOpaqueIndirectStubsManager? implicit; SourceRef : LLVMOrcOpaqueJITDylib? implicit; CallableAliases : LLVMOrcCSymbolAliasMapPair? implicit; NumPairs : uint64) : LLVMOrcOpaqueMaterializationUnit? {}
[extern(cdecl, name="LLVMOrcDisposeMaterializationResponsibility", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcDisposeMaterializationResponsibility(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit) : void {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityGetTargetDylib", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcMaterializationResponsibilityGetTargetDylib(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit) : LLVMOrcOpaqueJITDylib? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityGetExecutionSession", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcMaterializationResponsibilityGetExecutionSession(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit) : LLVMOrcOpaqueExecutionSession? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityGetSymbols", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcMaterializationResponsibilityGetSymbols(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; NumPairs : uint64? implicit) : LLVMOrcCSymbolFlagsMapPair? {}
[extern(cdecl, name="LLVMOrcDisposeCSymbolFlagsMap", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcDisposeCSymbolFlagsMap(Pairs : LLVMOrcCSymbolFlagsMapPair? implicit) : void {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityGetInitializerSymbol", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcMaterializationResponsibilityGetInitializerSymbol(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit) : LLVMOrcOpaqueSymbolStringPoolEntry? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityGetRequestedSymbols", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcMaterializationResponsibilityGetRequestedSymbols(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; NumSymbols : uint64? implicit) : LLVMOrcOpaqueSymbolStringPoolEntry ?? {}
[extern(cdecl, name="LLVMOrcDisposeSymbols", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcDisposeSymbols(Symbols : LLVMOrcOpaqueSymbolStringPoolEntry ?? implicit) : void {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityNotifyResolved", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcMaterializationResponsibilityNotifyResolved(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; Symbols : LLVMOrcCSymbolMapPair? implicit; NumPairs : uint64) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityNotifyEmitted", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcMaterializationResponsibilityNotifyEmitted(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityDefineMaterializing", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcMaterializationResponsibilityDefineMaterializing(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; Pairs : LLVMOrcCSymbolFlagsMapPair? implicit; NumPairs : uint64) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityFailMaterialization", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcMaterializationResponsibilityFailMaterialization(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit) : void {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityReplace", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcMaterializationResponsibilityReplace(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; MU : LLVMOrcOpaqueMaterializationUnit? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityDelegate", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcMaterializationResponsibilityDelegate(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; Symbols : LLVMOrcOpaqueSymbolStringPoolEntry ?? implicit; NumSymbols : uint64; Result : LLVMOrcOpaqueMaterializationResponsibility ?? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityAddDependencies", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcMaterializationResponsibilityAddDependencies(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; Name : LLVMOrcOpaqueSymbolStringPoolEntry? implicit; Dependencies : LLVMOrcCDependenceMapPair? implicit; NumPairs : uint64) : void {}
[extern(cdecl, name="LLVMOrcMaterializationResponsibilityAddDependenciesForAll", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcMaterializationResponsibilityAddDependenciesForAll(MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; Dependencies : LLVMOrcCDependenceMapPair? implicit; NumPairs : uint64) : void {}
[extern(cdecl, name="LLVMOrcExecutionSessionCreateBareJITDylib", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcExecutionSessionCreateBareJITDylib(ES : LLVMOrcOpaqueExecutionSession? implicit; Name : string implicit) : LLVMOrcOpaqueJITDylib? {}
[extern(cdecl, name="LLVMOrcExecutionSessionCreateJITDylib", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcExecutionSessionCreateJITDylib(ES : LLVMOrcOpaqueExecutionSession? implicit; Result : LLVMOrcOpaqueJITDylib ?? implicit; Name : string implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcExecutionSessionGetJITDylibByName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcExecutionSessionGetJITDylibByName(ES : LLVMOrcOpaqueExecutionSession? implicit; Name : string implicit) : LLVMOrcOpaqueJITDylib? {}
[extern(cdecl, name="LLVMOrcJITDylibCreateResourceTracker", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcJITDylibCreateResourceTracker(JD : LLVMOrcOpaqueJITDylib? implicit) : LLVMOrcOpaqueResourceTracker? {}
[extern(cdecl, name="LLVMOrcJITDylibGetDefaultResourceTracker", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcJITDylibGetDefaultResourceTracker(JD : LLVMOrcOpaqueJITDylib? implicit) : LLVMOrcOpaqueResourceTracker? {}
[extern(cdecl, name="LLVMOrcJITDylibDefine", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcJITDylibDefine(JD : LLVMOrcOpaqueJITDylib? implicit; MU : LLVMOrcOpaqueMaterializationUnit? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcJITDylibClear", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcJITDylibClear(JD : LLVMOrcOpaqueJITDylib? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcJITDylibAddGenerator", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcJITDylibAddGenerator(JD : LLVMOrcOpaqueJITDylib? implicit; DG : LLVMOrcOpaqueDefinitionGenerator? implicit) : void {}
[extern(cdecl, name="LLVMOrcLookupStateContinueLookup", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcLookupStateContinueLookup(S : LLVMOrcOpaqueLookupState? implicit; Err : LLVMOpaqueError? implicit) : void {}
[extern(cdecl, name="LLVMOrcCreateStaticLibrarySearchGeneratorForPath", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcCreateStaticLibrarySearchGeneratorForPath(Result : LLVMOrcOpaqueDefinitionGenerator ?? implicit; ObjLayer : LLVMOrcOpaqueObjectLayer? implicit; FileName : string implicit; TargetTriple : string implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcCreateNewThreadSafeContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcCreateNewThreadSafeContext() : LLVMOrcOpaqueThreadSafeContext? {}
[extern(cdecl, name="LLVMOrcThreadSafeContextGetContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcThreadSafeContextGetContext(TSCtx : LLVMOrcOpaqueThreadSafeContext? implicit) : LLVMOpaqueContext? {}
[extern(cdecl, name="LLVMOrcDisposeThreadSafeContext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcDisposeThreadSafeContext(TSCtx : LLVMOrcOpaqueThreadSafeContext? implicit) : void {}
[extern(cdecl, name="LLVMOrcCreateNewThreadSafeModule", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcCreateNewThreadSafeModule(M : LLVMOpaqueModule? implicit; TSCtx : LLVMOrcOpaqueThreadSafeContext? implicit) : LLVMOrcOpaqueThreadSafeModule? {}
[extern(cdecl, name="LLVMOrcDisposeThreadSafeModule", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcDisposeThreadSafeModule(TSM : LLVMOrcOpaqueThreadSafeModule? implicit) : void {}
[extern(cdecl, name="LLVMOrcJITTargetMachineBuilderDetectHost", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcJITTargetMachineBuilderDetectHost(Result : LLVMOrcOpaqueJITTargetMachineBuilder ?? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcJITTargetMachineBuilderCreateFromTargetMachine", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcJITTargetMachineBuilderCreateFromTargetMachine(TM : LLVMOpaqueTargetMachine? implicit) : LLVMOrcOpaqueJITTargetMachineBuilder? {}
[extern(cdecl, name="LLVMOrcDisposeJITTargetMachineBuilder", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcDisposeJITTargetMachineBuilder(JTMB : LLVMOrcOpaqueJITTargetMachineBuilder? implicit) : void {}
[extern(cdecl, name="LLVMOrcJITTargetMachineBuilderGetTargetTriple", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcJITTargetMachineBuilderGetTargetTriple(JTMB : LLVMOrcOpaqueJITTargetMachineBuilder? implicit) : string {}
[extern(cdecl, name="LLVMOrcJITTargetMachineBuilderSetTargetTriple", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcJITTargetMachineBuilderSetTargetTriple(JTMB : LLVMOrcOpaqueJITTargetMachineBuilder? implicit; TargetTriple : string implicit) : void {}
[extern(cdecl, name="LLVMOrcObjectLayerAddObjectFile", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcObjectLayerAddObjectFile(ObjLayer : LLVMOrcOpaqueObjectLayer? implicit; JD : LLVMOrcOpaqueJITDylib? implicit; ObjBuffer : LLVMOpaqueMemoryBuffer? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcObjectLayerAddObjectFileWithRT", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcObjectLayerAddObjectFileWithRT(ObjLayer : LLVMOrcOpaqueObjectLayer? implicit; RT : LLVMOrcOpaqueResourceTracker? implicit; ObjBuffer : LLVMOpaqueMemoryBuffer? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcObjectLayerEmit", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcObjectLayerEmit(ObjLayer : LLVMOrcOpaqueObjectLayer? implicit; R : LLVMOrcOpaqueMaterializationResponsibility? implicit; ObjBuffer : LLVMOpaqueMemoryBuffer? implicit) : void {}
[extern(cdecl, name="LLVMOrcDisposeObjectLayer", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcDisposeObjectLayer(ObjLayer : LLVMOrcOpaqueObjectLayer? implicit) : void {}
[extern(cdecl, name="LLVMOrcIRTransformLayerEmit", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcIRTransformLayerEmit(IRTransformLayer : LLVMOrcOpaqueIRTransformLayer? implicit; MR : LLVMOrcOpaqueMaterializationResponsibility? implicit; TSM : LLVMOrcOpaqueThreadSafeModule? implicit) : void {}
[extern(cdecl, name="LLVMOrcCreateLocalIndirectStubsManager", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcCreateLocalIndirectStubsManager(TargetTriple : string implicit) : LLVMOrcOpaqueIndirectStubsManager? {}
[extern(cdecl, name="LLVMOrcDisposeIndirectStubsManager", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcDisposeIndirectStubsManager(ISM : LLVMOrcOpaqueIndirectStubsManager? implicit) : void {}
[extern(cdecl, name="LLVMOrcCreateLocalLazyCallThroughManager", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcCreateLocalLazyCallThroughManager(TargetTriple : string implicit; ES : LLVMOrcOpaqueExecutionSession? implicit; ErrorHandlerAddr : uint64; LCTM : LLVMOrcOpaqueLazyCallThroughManager ?? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcDisposeLazyCallThroughManager", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcDisposeLazyCallThroughManager(LCTM : LLVMOrcOpaqueLazyCallThroughManager? implicit) : void {}
[extern(cdecl, name="LLVMOrcCreateDumpObjects", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcCreateDumpObjects(DumpDir : string implicit; IdentifierOverride : string implicit) : LLVMOrcOpaqueDumpObjects? {}
[extern(cdecl, name="LLVMOrcDisposeDumpObjects", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcDisposeDumpObjects(DumpObjects : LLVMOrcOpaqueDumpObjects? implicit) : void {}
[extern(cdecl, name="LLVMOrcDumpObjects_CallOperator", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcDumpObjects_CallOperator(DumpObjects : LLVMOrcOpaqueDumpObjects? implicit; ObjBuffer : LLVMOpaqueMemoryBuffer ?? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcCreateLLJITBuilder", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcCreateLLJITBuilder() : LLVMOrcOpaqueLLJITBuilder? {}
[extern(cdecl, name="LLVMOrcDisposeLLJITBuilder", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcDisposeLLJITBuilder(Builder : LLVMOrcOpaqueLLJITBuilder? implicit) : void {}
[extern(cdecl, name="LLVMOrcLLJITBuilderSetJITTargetMachineBuilder", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcLLJITBuilderSetJITTargetMachineBuilder(Builder : LLVMOrcOpaqueLLJITBuilder? implicit; JTMB : LLVMOrcOpaqueJITTargetMachineBuilder? implicit) : void {}
[extern(cdecl, name="LLVMOrcCreateLLJIT", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcCreateLLJIT(Result : LLVMOrcOpaqueLLJIT ?? implicit; Builder : LLVMOrcOpaqueLLJITBuilder? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcDisposeLLJIT", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcDisposeLLJIT(J : LLVMOrcOpaqueLLJIT? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcLLJITGetExecutionSession", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcLLJITGetExecutionSession(J : LLVMOrcOpaqueLLJIT? implicit) : LLVMOrcOpaqueExecutionSession? {}
[extern(cdecl, name="LLVMOrcLLJITGetMainJITDylib", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcLLJITGetMainJITDylib(J : LLVMOrcOpaqueLLJIT? implicit) : LLVMOrcOpaqueJITDylib? {}
[extern(cdecl, name="LLVMOrcLLJITGetTripleString", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcLLJITGetTripleString(J : LLVMOrcOpaqueLLJIT? implicit) : string {}
[extern(cdecl, name="LLVMOrcLLJITGetGlobalPrefix", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcLLJITGetGlobalPrefix(J : LLVMOrcOpaqueLLJIT? implicit) : int8 {}
[extern(cdecl, name="LLVMOrcLLJITMangleAndIntern", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcLLJITMangleAndIntern(J : LLVMOrcOpaqueLLJIT? implicit; UnmangledName : string implicit) : LLVMOrcOpaqueSymbolStringPoolEntry? {}
[extern(cdecl, name="LLVMOrcLLJITAddObjectFile", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcLLJITAddObjectFile(J : LLVMOrcOpaqueLLJIT? implicit; JD : LLVMOrcOpaqueJITDylib? implicit; ObjBuffer : LLVMOpaqueMemoryBuffer? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcLLJITAddObjectFileWithRT", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcLLJITAddObjectFileWithRT(J : LLVMOrcOpaqueLLJIT? implicit; RT : LLVMOrcOpaqueResourceTracker? implicit; ObjBuffer : LLVMOpaqueMemoryBuffer? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcLLJITAddLLVMIRModule", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcLLJITAddLLVMIRModule(J : LLVMOrcOpaqueLLJIT? implicit; JD : LLVMOrcOpaqueJITDylib? implicit; TSM : LLVMOrcOpaqueThreadSafeModule? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcLLJITAddLLVMIRModuleWithRT", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcLLJITAddLLVMIRModuleWithRT(J : LLVMOrcOpaqueLLJIT? implicit; JD : LLVMOrcOpaqueResourceTracker? implicit; TSM : LLVMOrcOpaqueThreadSafeModule? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcLLJITLookup", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcLLJITLookup(J : LLVMOrcOpaqueLLJIT? implicit; Result : uint64? implicit; Name : string implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMOrcLLJITGetObjLinkingLayer", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcLLJITGetObjLinkingLayer(J : LLVMOrcOpaqueLLJIT? implicit) : LLVMOrcOpaqueObjectLayer? {}
[extern(cdecl, name="LLVMOrcLLJITGetObjTransformLayer", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcLLJITGetObjTransformLayer(J : LLVMOrcOpaqueLLJIT? implicit) : LLVMOrcOpaqueObjectTransformLayer? {}
[extern(cdecl, name="LLVMOrcLLJITGetIRTransformLayer", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcLLJITGetIRTransformLayer(J : LLVMOrcOpaqueLLJIT? implicit) : LLVMOrcOpaqueIRTransformLayer? {}
[extern(cdecl, name="LLVMOrcLLJITGetDataLayoutStr", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcLLJITGetDataLayoutStr(J : LLVMOrcOpaqueLLJIT? implicit) : string {}
[extern(cdecl, name="lto_get_version", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_get_version() : string {}
[extern(cdecl, name="lto_get_error_message", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_get_error_message() : string {}
[extern(cdecl, name="lto_module_is_object_file", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_is_object_file(path : string implicit) : bool {}
[extern(cdecl, name="lto_module_is_object_file_for_target", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_is_object_file_for_target(path : string implicit; target_triple_prefix : string implicit) : bool {}
[extern(cdecl, name="lto_module_has_objc_category", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_has_objc_category(mem : void const? implicit; length : uint64) : bool {}
[extern(cdecl, name="lto_module_is_object_file_in_memory", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_is_object_file_in_memory(mem : void const? implicit; length : uint64) : bool {}
[extern(cdecl, name="lto_module_is_object_file_in_memory_for_target", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_is_object_file_in_memory_for_target(mem : void const? implicit; length : uint64; target_triple_prefix : string implicit) : bool {}
[extern(cdecl, name="lto_module_create", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_create(path : string implicit) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_create_from_memory", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_create_from_memory(mem : void const? implicit; length : uint64) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_create_from_memory_with_path", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_create_from_memory_with_path(mem : void const? implicit; length : uint64; path : string implicit) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_create_in_local_context", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_create_in_local_context(mem : void const? implicit; length : uint64; path : string implicit) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_create_in_codegen_context", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_create_in_codegen_context(mem : void const? implicit; length : uint64; path : string implicit; cg : LLVMOpaqueLTOCodeGenerator? implicit) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_create_from_fd", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_create_from_fd(fd : int; path : string implicit; file_size : uint64) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_create_from_fd_at_offset", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_create_from_fd_at_offset(fd : int; path : string implicit; file_size : uint64; map_size : uint64; offset : int64) : LLVMOpaqueLTOModule? {}
[extern(cdecl, name="lto_module_dispose", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_dispose(mod : LLVMOpaqueLTOModule? implicit) : void {}
[extern(cdecl, name="lto_module_get_target_triple", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_get_target_triple(mod : LLVMOpaqueLTOModule? implicit) : string {}
[extern(cdecl, name="lto_module_set_target_triple", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_set_target_triple(mod : LLVMOpaqueLTOModule? implicit; triple : string implicit) : void {}
[extern(cdecl, name="lto_module_get_num_symbols", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_get_num_symbols(mod : LLVMOpaqueLTOModule? implicit) : uint {}
[extern(cdecl, name="lto_module_get_symbol_name", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_get_symbol_name(mod : LLVMOpaqueLTOModule? implicit; index : uint) : string {}
[extern(cdecl, name="lto_module_get_symbol_attribute", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_get_symbol_attribute(mod : LLVMOpaqueLTOModule? implicit; index : uint) : lto_symbol_attributes {}
[extern(cdecl, name="lto_module_get_linkeropts", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_get_linkeropts(mod : LLVMOpaqueLTOModule? implicit) : string {}
[extern(cdecl, name="lto_module_get_macho_cputype", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_get_macho_cputype(mod : LLVMOpaqueLTOModule? implicit; out_cputype : uint? implicit; out_cpusubtype : uint? implicit) : bool {}
[extern(cdecl, name="lto_module_has_ctor_dtor", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_has_ctor_dtor(mod : LLVMOpaqueLTOModule? implicit) : bool {}
[extern(cdecl, name="lto_codegen_create", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_create() : LLVMOpaqueLTOCodeGenerator? {}
[extern(cdecl, name="lto_codegen_create_in_local_context", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_create_in_local_context() : LLVMOpaqueLTOCodeGenerator? {}
[extern(cdecl, name="lto_codegen_dispose", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_dispose(_argument_0x0 : LLVMOpaqueLTOCodeGenerator? implicit) : void {}
[extern(cdecl, name="lto_codegen_add_module", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_add_module(cg : LLVMOpaqueLTOCodeGenerator? implicit; mod : LLVMOpaqueLTOModule? implicit) : bool {}
[extern(cdecl, name="lto_codegen_set_module", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_set_module(cg : LLVMOpaqueLTOCodeGenerator? implicit; mod : LLVMOpaqueLTOModule? implicit) : void {}
[extern(cdecl, name="lto_codegen_set_debug_model", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_set_debug_model(cg : LLVMOpaqueLTOCodeGenerator? implicit; _argument_0x1 : lto_debug_model) : bool {}
[extern(cdecl, name="lto_codegen_set_pic_model", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_set_pic_model(cg : LLVMOpaqueLTOCodeGenerator? implicit; _argument_0x1 : lto_codegen_model) : bool {}
[extern(cdecl, name="lto_codegen_set_cpu", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_set_cpu(cg : LLVMOpaqueLTOCodeGenerator? implicit; cpu : string implicit) : void {}
[extern(cdecl, name="lto_codegen_set_assembler_path", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_set_assembler_path(cg : LLVMOpaqueLTOCodeGenerator? implicit; path : string implicit) : void {}
[extern(cdecl, name="lto_codegen_set_assembler_args", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_set_assembler_args(cg : LLVMOpaqueLTOCodeGenerator? implicit; args : string? implicit; nargs : int) : void {}
[extern(cdecl, name="lto_codegen_add_must_preserve_symbol", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_add_must_preserve_symbol(cg : LLVMOpaqueLTOCodeGenerator? implicit; symbol : string implicit) : void {}
[extern(cdecl, name="lto_codegen_write_merged_modules", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_write_merged_modules(cg : LLVMOpaqueLTOCodeGenerator? implicit; path : string implicit) : bool {}
[extern(cdecl, name="lto_codegen_compile", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_compile(cg : LLVMOpaqueLTOCodeGenerator? implicit; length : uint64? implicit) : void const? {}
[extern(cdecl, name="lto_codegen_compile_to_file", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_compile_to_file(cg : LLVMOpaqueLTOCodeGenerator? implicit; name : string? implicit) : bool {}
[extern(cdecl, name="lto_codegen_optimize", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_optimize(cg : LLVMOpaqueLTOCodeGenerator? implicit) : bool {}
[extern(cdecl, name="lto_codegen_compile_optimized", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_compile_optimized(cg : LLVMOpaqueLTOCodeGenerator? implicit; length : uint64? implicit) : void const? {}
[extern(cdecl, name="lto_api_version", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_api_version() : uint {}
[extern(cdecl, name="lto_set_debug_options", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_set_debug_options(_options : string const? implicit; number : int) : void {}
[extern(cdecl, name="lto_codegen_debug_options", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_debug_options(cg : LLVMOpaqueLTOCodeGenerator? implicit; _argument_0x1 : string implicit) : void {}
[extern(cdecl, name="lto_codegen_debug_options_array", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_debug_options_array(cg : LLVMOpaqueLTOCodeGenerator? implicit; _argument_0x1 : string const? implicit; number : int) : void {}
[extern(cdecl, name="lto_initialize_disassembler", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_initialize_disassembler() : void {}
[extern(cdecl, name="lto_codegen_set_should_internalize", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_set_should_internalize(cg : LLVMOpaqueLTOCodeGenerator? implicit; ShouldInternalize : bool) : void {}
[extern(cdecl, name="lto_codegen_set_should_embed_uselists", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_codegen_set_should_embed_uselists(cg : LLVMOpaqueLTOCodeGenerator? implicit; ShouldEmbedUselists : bool) : void {}
[extern(cdecl, name="lto_input_create", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_input_create(buffer : void const? implicit; buffer_size : uint64; path : string implicit) : LLVMOpaqueLTOInput? {}
[extern(cdecl, name="lto_input_dispose", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_input_dispose(input : LLVMOpaqueLTOInput? implicit) : void {}
[extern(cdecl, name="lto_input_get_num_dependent_libraries", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_input_get_num_dependent_libraries(input : LLVMOpaqueLTOInput? implicit) : uint {}
[extern(cdecl, name="lto_input_get_dependent_library", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_input_get_dependent_library(input : LLVMOpaqueLTOInput? implicit; index : uint64; size : uint64? implicit) : string {}
[extern(cdecl, name="lto_runtime_lib_symbols_list", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_runtime_lib_symbols_list(size : uint64? implicit) : string const? {}
[extern(cdecl, name="thinlto_create_codegen", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_create_codegen() : LLVMOpaqueThinLTOCodeGenerator? {}
[extern(cdecl, name="thinlto_codegen_dispose", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_codegen_dispose(cg : LLVMOpaqueThinLTOCodeGenerator? implicit) : void {}
[extern(cdecl, name="thinlto_codegen_add_module", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_codegen_add_module(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; identifier : string implicit; data : string implicit; length : int) : void {}
[extern(cdecl, name="thinlto_codegen_process", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_codegen_process(cg : LLVMOpaqueThinLTOCodeGenerator? implicit) : void {}
[extern(cdecl, name="thinlto_module_get_num_objects", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_module_get_num_objects(cg : LLVMOpaqueThinLTOCodeGenerator? implicit) : uint {}
[extern(cdecl, name="thinlto_module_get_object", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_module_get_object(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; index : uint) : LTOObjectBuffer {}
[extern(cdecl, name="thinlto_module_get_num_object_files", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_module_get_num_object_files(cg : LLVMOpaqueThinLTOCodeGenerator? implicit) : uint {}
[extern(cdecl, name="thinlto_module_get_object_file", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_module_get_object_file(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; index : uint) : string {}
[extern(cdecl, name="thinlto_codegen_set_pic_model", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_codegen_set_pic_model(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; _argument_0x1 : lto_codegen_model) : bool {}
[extern(cdecl, name="thinlto_codegen_set_savetemps_dir", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_codegen_set_savetemps_dir(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; save_temps_dir : string implicit) : void {}
[extern(cdecl, name="thinlto_set_generated_objects_dir", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_set_generated_objects_dir(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; save_temps_dir : string implicit) : void {}
[extern(cdecl, name="thinlto_codegen_set_cpu", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_codegen_set_cpu(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; cpu : string implicit) : void {}
[extern(cdecl, name="thinlto_codegen_disable_codegen", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_codegen_disable_codegen(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; disable : bool) : void {}
[extern(cdecl, name="thinlto_codegen_set_codegen_only", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_codegen_set_codegen_only(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; codegen_only : bool) : void {}
[extern(cdecl, name="thinlto_debug_options", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_debug_options(_options : string const? implicit; number : int) : void {}
[extern(cdecl, name="lto_module_is_thinlto", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def lto_module_is_thinlto(mod : LLVMOpaqueLTOModule? implicit) : bool {}
[extern(cdecl, name="thinlto_codegen_add_must_preserve_symbol", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_codegen_add_must_preserve_symbol(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; name : string implicit; length : int) : void {}
[extern(cdecl, name="thinlto_codegen_add_cross_referenced_symbol", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_codegen_add_cross_referenced_symbol(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; name : string implicit; length : int) : void {}
[extern(cdecl, name="thinlto_codegen_set_cache_dir", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_codegen_set_cache_dir(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; cache_dir : string implicit) : void {}
[extern(cdecl, name="thinlto_codegen_set_cache_pruning_interval", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_codegen_set_cache_pruning_interval(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; interval : int) : void {}
[extern(cdecl, name="thinlto_codegen_set_final_cache_size_relative_to_available_space", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_codegen_set_final_cache_size_relative_to_available_space(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; percentage : uint) : void {}
[extern(cdecl, name="thinlto_codegen_set_cache_entry_expiration", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_codegen_set_cache_entry_expiration(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; expiration : uint) : void {}
[extern(cdecl, name="thinlto_codegen_set_cache_size_bytes", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_codegen_set_cache_size_bytes(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; max_size_bytes : uint) : void {}
[extern(cdecl, name="thinlto_codegen_set_cache_size_megabytes", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_codegen_set_cache_size_megabytes(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; max_size_megabytes : uint) : void {}
[extern(cdecl, name="thinlto_codegen_set_cache_size_files", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def thinlto_codegen_set_cache_size_files(cg : LLVMOpaqueThinLTOCodeGenerator? implicit; max_size_files : uint) : void {}
[extern(cdecl, name="LLVMCreateBinary", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateBinary(MemBuf : LLVMOpaqueMemoryBuffer? implicit; Context : LLVMOpaqueContext? implicit; ErrorMessage : string? implicit) : LLVMOpaqueBinary? {}
[extern(cdecl, name="LLVMDisposeBinary", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeBinary(BR : LLVMOpaqueBinary? implicit) : void {}
[extern(cdecl, name="LLVMBinaryCopyMemoryBuffer", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBinaryCopyMemoryBuffer(BR : LLVMOpaqueBinary? implicit) : LLVMOpaqueMemoryBuffer? {}
[extern(cdecl, name="LLVMBinaryGetType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMBinaryGetType(BR : LLVMOpaqueBinary? implicit) : LLVMBinaryType {}
[extern(cdecl, name="LLVMMachOUniversalBinaryCopyObjectForArch", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMMachOUniversalBinaryCopyObjectForArch(BR : LLVMOpaqueBinary? implicit; Arch : string implicit; ArchLen : uint64; ErrorMessage : string? implicit) : LLVMOpaqueBinary? {}
[extern(cdecl, name="LLVMObjectFileCopySectionIterator", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMObjectFileCopySectionIterator(BR : LLVMOpaqueBinary? implicit) : LLVMOpaqueSectionIterator? {}
[extern(cdecl, name="LLVMObjectFileIsSectionIteratorAtEnd", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMObjectFileIsSectionIteratorAtEnd(BR : LLVMOpaqueBinary? implicit; SI : LLVMOpaqueSectionIterator? implicit) : int {}
[extern(cdecl, name="LLVMObjectFileCopySymbolIterator", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMObjectFileCopySymbolIterator(BR : LLVMOpaqueBinary? implicit) : LLVMOpaqueSymbolIterator? {}
[extern(cdecl, name="LLVMObjectFileIsSymbolIteratorAtEnd", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMObjectFileIsSymbolIteratorAtEnd(BR : LLVMOpaqueBinary? implicit; SI : LLVMOpaqueSymbolIterator? implicit) : int {}
[extern(cdecl, name="LLVMDisposeSectionIterator", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeSectionIterator(SI : LLVMOpaqueSectionIterator? implicit) : void {}
[extern(cdecl, name="LLVMMoveToNextSection", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMMoveToNextSection(SI : LLVMOpaqueSectionIterator? implicit) : void {}
[extern(cdecl, name="LLVMMoveToContainingSection", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMMoveToContainingSection(Sect : LLVMOpaqueSectionIterator? implicit; Sym : LLVMOpaqueSymbolIterator? implicit) : void {}
[extern(cdecl, name="LLVMDisposeSymbolIterator", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeSymbolIterator(SI : LLVMOpaqueSymbolIterator? implicit) : void {}
[extern(cdecl, name="LLVMMoveToNextSymbol", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMMoveToNextSymbol(SI : LLVMOpaqueSymbolIterator? implicit) : void {}
[extern(cdecl, name="LLVMGetSectionName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetSectionName(SI : LLVMOpaqueSectionIterator? implicit) : string {}
[extern(cdecl, name="LLVMGetSectionSize", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetSectionSize(SI : LLVMOpaqueSectionIterator? implicit) : uint64 {}
[extern(cdecl, name="LLVMGetSectionContents", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetSectionContents(SI : LLVMOpaqueSectionIterator? implicit) : string {}
[extern(cdecl, name="LLVMGetSectionAddress", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetSectionAddress(SI : LLVMOpaqueSectionIterator? implicit) : uint64 {}
[extern(cdecl, name="LLVMGetSectionContainsSymbol", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetSectionContainsSymbol(SI : LLVMOpaqueSectionIterator? implicit; Sym : LLVMOpaqueSymbolIterator? implicit) : int {}
[extern(cdecl, name="LLVMGetRelocations", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetRelocations(Section : LLVMOpaqueSectionIterator? implicit) : LLVMOpaqueRelocationIterator? {}
[extern(cdecl, name="LLVMDisposeRelocationIterator", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeRelocationIterator(RI : LLVMOpaqueRelocationIterator? implicit) : void {}
[extern(cdecl, name="LLVMIsRelocationIteratorAtEnd", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsRelocationIteratorAtEnd(Section : LLVMOpaqueSectionIterator? implicit; RI : LLVMOpaqueRelocationIterator? implicit) : int {}
[extern(cdecl, name="LLVMMoveToNextRelocation", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMMoveToNextRelocation(RI : LLVMOpaqueRelocationIterator? implicit) : void {}
[extern(cdecl, name="LLVMGetSymbolName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetSymbolName(SI : LLVMOpaqueSymbolIterator? implicit) : string {}
[extern(cdecl, name="LLVMGetSymbolAddress", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetSymbolAddress(SI : LLVMOpaqueSymbolIterator? implicit) : uint64 {}
[extern(cdecl, name="LLVMGetSymbolSize", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetSymbolSize(SI : LLVMOpaqueSymbolIterator? implicit) : uint64 {}
[extern(cdecl, name="LLVMGetRelocationOffset", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetRelocationOffset(RI : LLVMOpaqueRelocationIterator? implicit) : uint64 {}
[extern(cdecl, name="LLVMGetRelocationSymbol", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetRelocationSymbol(RI : LLVMOpaqueRelocationIterator? implicit) : LLVMOpaqueSymbolIterator? {}
[extern(cdecl, name="LLVMGetRelocationType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetRelocationType(RI : LLVMOpaqueRelocationIterator? implicit) : uint64 {}
[extern(cdecl, name="LLVMGetRelocationTypeName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetRelocationTypeName(RI : LLVMOpaqueRelocationIterator? implicit) : string {}
[extern(cdecl, name="LLVMGetRelocationValueString", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetRelocationValueString(RI : LLVMOpaqueRelocationIterator? implicit) : string {}
[extern(cdecl, name="LLVMCreateObjectFile", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreateObjectFile(MemBuf : LLVMOpaqueMemoryBuffer? implicit) : LLVMOpaqueObjectFile? {}
[extern(cdecl, name="LLVMDisposeObjectFile", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposeObjectFile(ObjectFile : LLVMOpaqueObjectFile? implicit) : void {}
[extern(cdecl, name="LLVMGetSections", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetSections(ObjectFile : LLVMOpaqueObjectFile? implicit) : LLVMOpaqueSectionIterator? {}
[extern(cdecl, name="LLVMIsSectionIteratorAtEnd", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsSectionIteratorAtEnd(ObjectFile : LLVMOpaqueObjectFile? implicit; SI : LLVMOpaqueSectionIterator? implicit) : int {}
[extern(cdecl, name="LLVMGetSymbols", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMGetSymbols(ObjectFile : LLVMOpaqueObjectFile? implicit) : LLVMOpaqueSymbolIterator? {}
[extern(cdecl, name="LLVMIsSymbolIteratorAtEnd", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMIsSymbolIteratorAtEnd(ObjectFile : LLVMOpaqueObjectFile? implicit; SI : LLVMOpaqueSymbolIterator? implicit) : int {}
[extern(cdecl, name="LLVMOrcCreateRTDyldObjectLinkingLayerWithSectionMemoryManager", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcCreateRTDyldObjectLinkingLayerWithSectionMemoryManager(ES : LLVMOrcOpaqueExecutionSession? implicit) : LLVMOrcOpaqueObjectLayer? {}
[extern(cdecl, name="LLVMOrcRTDyldObjectLinkingLayerRegisterJITEventListener", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMOrcRTDyldObjectLinkingLayerRegisterJITEventListener(RTDyldObjLinkingLayer : LLVMOrcOpaqueObjectLayer? implicit; Listener : LLVMOpaqueJITEventListener? implicit) : void {}
[extern(cdecl, name="LLVMRemarkStringGetData", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkStringGetData(String : LLVMRemarkOpaqueString? implicit) : string {}
[extern(cdecl, name="LLVMRemarkStringGetLen", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkStringGetLen(String : LLVMRemarkOpaqueString? implicit) : uint {}
[extern(cdecl, name="LLVMRemarkDebugLocGetSourceFilePath", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkDebugLocGetSourceFilePath(DL : LLVMRemarkOpaqueDebugLoc? implicit) : LLVMRemarkOpaqueString? {}
[extern(cdecl, name="LLVMRemarkDebugLocGetSourceLine", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkDebugLocGetSourceLine(DL : LLVMRemarkOpaqueDebugLoc? implicit) : uint {}
[extern(cdecl, name="LLVMRemarkDebugLocGetSourceColumn", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkDebugLocGetSourceColumn(DL : LLVMRemarkOpaqueDebugLoc? implicit) : uint {}
[extern(cdecl, name="LLVMRemarkArgGetKey", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkArgGetKey(Arg : LLVMRemarkOpaqueArg? implicit) : LLVMRemarkOpaqueString? {}
[extern(cdecl, name="LLVMRemarkArgGetValue", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkArgGetValue(Arg : LLVMRemarkOpaqueArg? implicit) : LLVMRemarkOpaqueString? {}
[extern(cdecl, name="LLVMRemarkArgGetDebugLoc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkArgGetDebugLoc(Arg : LLVMRemarkOpaqueArg? implicit) : LLVMRemarkOpaqueDebugLoc? {}
[extern(cdecl, name="LLVMRemarkEntryDispose", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkEntryDispose(Remark : LLVMRemarkOpaqueEntry? implicit) : void {}
[extern(cdecl, name="LLVMRemarkEntryGetType", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkEntryGetType(Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkType {}
[extern(cdecl, name="LLVMRemarkEntryGetPassName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkEntryGetPassName(Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkOpaqueString? {}
[extern(cdecl, name="LLVMRemarkEntryGetRemarkName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkEntryGetRemarkName(Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkOpaqueString? {}
[extern(cdecl, name="LLVMRemarkEntryGetFunctionName", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkEntryGetFunctionName(Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkOpaqueString? {}
[extern(cdecl, name="LLVMRemarkEntryGetDebugLoc", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkEntryGetDebugLoc(Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkOpaqueDebugLoc? {}
[extern(cdecl, name="LLVMRemarkEntryGetHotness", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkEntryGetHotness(Remark : LLVMRemarkOpaqueEntry? implicit) : uint64 {}
[extern(cdecl, name="LLVMRemarkEntryGetNumArgs", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkEntryGetNumArgs(Remark : LLVMRemarkOpaqueEntry? implicit) : uint {}
[extern(cdecl, name="LLVMRemarkEntryGetFirstArg", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkEntryGetFirstArg(Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkOpaqueArg? {}
[extern(cdecl, name="LLVMRemarkEntryGetNextArg", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkEntryGetNextArg(It : LLVMRemarkOpaqueArg? implicit; Remark : LLVMRemarkOpaqueEntry? implicit) : LLVMRemarkOpaqueArg? {}
[extern(cdecl, name="LLVMRemarkParserCreateYAML", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkParserCreateYAML(Buf : void const? implicit; Size : uint64) : LLVMRemarkOpaqueParser? {}
[extern(cdecl, name="LLVMRemarkParserCreateBitstream", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkParserCreateBitstream(Buf : void const? implicit; Size : uint64) : LLVMRemarkOpaqueParser? {}
[extern(cdecl, name="LLVMRemarkParserGetNext", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkParserGetNext(Parser : LLVMRemarkOpaqueParser? implicit) : LLVMRemarkOpaqueEntry? {}
[extern(cdecl, name="LLVMRemarkParserHasError", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkParserHasError(Parser : LLVMRemarkOpaqueParser? implicit) : int {}
[extern(cdecl, name="LLVMRemarkParserGetErrorMessage", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkParserGetErrorMessage(Parser : LLVMRemarkOpaqueParser? implicit) : string {}
[extern(cdecl, name="LLVMRemarkParserDispose", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkParserDispose(Parser : LLVMRemarkOpaqueParser? implicit) : void {}
[extern(cdecl, name="LLVMRemarkVersion", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRemarkVersion() : uint {}
[extern(cdecl, name="LLVMLoadLibraryPermanently", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMLoadLibraryPermanently(Filename : string implicit) : int {}
[extern(cdecl, name="LLVMParseCommandLineOptions", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMParseCommandLineOptions(argc : int; argv : string const? implicit; Overview : string implicit) : void {}
[extern(cdecl, name="LLVMSearchForAddressOfSymbol", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMSearchForAddressOfSymbol(symbolName : string implicit) : void? {}
[extern(cdecl, name="LLVMAddSymbol", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddSymbol(symbolName : string implicit; symbolValue : void? implicit) : void {}
[extern(cdecl, name="LLVMAddConstantMergePass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddConstantMergePass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddMergeFunctionsPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddMergeFunctionsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddCalledValuePropagationPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddCalledValuePropagationPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddDeadArgEliminationPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddDeadArgEliminationPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddFunctionAttrsPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddFunctionAttrsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddFunctionInliningPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddFunctionInliningPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddAlwaysInlinerPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddAlwaysInlinerPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddGlobalDCEPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddGlobalDCEPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddGlobalOptimizerPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddGlobalOptimizerPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddIPSCCPPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddIPSCCPPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddInternalizePass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddInternalizePass(_argument_0x0 : LLVMOpaquePassManager? implicit; AllButMain : uint) : void {}
[extern(cdecl, name="LLVMAddStripDeadPrototypesPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddStripDeadPrototypesPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddStripSymbolsPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddStripSymbolsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMRunPasses", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMRunPasses(M : LLVMOpaqueModule? implicit; Passes : string implicit; TM : LLVMOpaqueTargetMachine? implicit; Options : LLVMOpaquePassBuilderOptions? implicit) : LLVMOpaqueError? {}
[extern(cdecl, name="LLVMCreatePassBuilderOptions", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMCreatePassBuilderOptions() : LLVMOpaquePassBuilderOptions? {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetVerifyEach", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassBuilderOptionsSetVerifyEach(Options : LLVMOpaquePassBuilderOptions? implicit; VerifyEach : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetDebugLogging", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassBuilderOptionsSetDebugLogging(Options : LLVMOpaquePassBuilderOptions? implicit; DebugLogging : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetLoopInterleaving", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassBuilderOptionsSetLoopInterleaving(Options : LLVMOpaquePassBuilderOptions? implicit; LoopInterleaving : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetLoopVectorization", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassBuilderOptionsSetLoopVectorization(Options : LLVMOpaquePassBuilderOptions? implicit; LoopVectorization : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetSLPVectorization", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassBuilderOptionsSetSLPVectorization(Options : LLVMOpaquePassBuilderOptions? implicit; SLPVectorization : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetLoopUnrolling", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassBuilderOptionsSetLoopUnrolling(Options : LLVMOpaquePassBuilderOptions? implicit; LoopUnrolling : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetForgetAllSCEVInLoopUnroll", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassBuilderOptionsSetForgetAllSCEVInLoopUnroll(Options : LLVMOpaquePassBuilderOptions? implicit; ForgetAllSCEVInLoopUnroll : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetLicmMssaOptCap", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassBuilderOptionsSetLicmMssaOptCap(Options : LLVMOpaquePassBuilderOptions? implicit; LicmMssaOptCap : uint) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetLicmMssaNoAccForPromotionCap", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassBuilderOptionsSetLicmMssaNoAccForPromotionCap(Options : LLVMOpaquePassBuilderOptions? implicit; LicmMssaNoAccForPromotionCap : uint) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetCallGraphProfile", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassBuilderOptionsSetCallGraphProfile(Options : LLVMOpaquePassBuilderOptions? implicit; CallGraphProfile : int) : void {}
[extern(cdecl, name="LLVMPassBuilderOptionsSetMergeFunctions", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassBuilderOptionsSetMergeFunctions(Options : LLVMOpaquePassBuilderOptions? implicit; MergeFunctions : int) : void {}
[extern(cdecl, name="LLVMDisposePassBuilderOptions", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMDisposePassBuilderOptions(Options : LLVMOpaquePassBuilderOptions? implicit) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderCreate", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassManagerBuilderCreate() : LLVMOpaquePassManagerBuilder? {}
[extern(cdecl, name="LLVMPassManagerBuilderDispose", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassManagerBuilderDispose(PMB : LLVMOpaquePassManagerBuilder? implicit) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderSetOptLevel", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassManagerBuilderSetOptLevel(PMB : LLVMOpaquePassManagerBuilder? implicit; OptLevel : uint) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderSetSizeLevel", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassManagerBuilderSetSizeLevel(PMB : LLVMOpaquePassManagerBuilder? implicit; SizeLevel : uint) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderSetDisableUnitAtATime", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassManagerBuilderSetDisableUnitAtATime(PMB : LLVMOpaquePassManagerBuilder? implicit; Value : int) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderSetDisableUnrollLoops", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassManagerBuilderSetDisableUnrollLoops(PMB : LLVMOpaquePassManagerBuilder? implicit; Value : int) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderSetDisableSimplifyLibCalls", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassManagerBuilderSetDisableSimplifyLibCalls(PMB : LLVMOpaquePassManagerBuilder? implicit; Value : int) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderUseInlinerWithThreshold", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassManagerBuilderUseInlinerWithThreshold(PMB : LLVMOpaquePassManagerBuilder? implicit; Threshold : uint) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderPopulateFunctionPassManager", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassManagerBuilderPopulateFunctionPassManager(PMB : LLVMOpaquePassManagerBuilder? implicit; PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMPassManagerBuilderPopulateModulePassManager", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMPassManagerBuilderPopulateModulePassManager(PMB : LLVMOpaquePassManagerBuilder? implicit; PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddAggressiveDCEPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddAggressiveDCEPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddDCEPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddDCEPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddBitTrackingDCEPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddBitTrackingDCEPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddAlignmentFromAssumptionsPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddAlignmentFromAssumptionsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddCFGSimplificationPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddCFGSimplificationPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddDeadStoreEliminationPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddDeadStoreEliminationPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddScalarizerPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddScalarizerPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddMergedLoadStoreMotionPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddMergedLoadStoreMotionPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddGVNPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddGVNPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddNewGVNPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddNewGVNPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddIndVarSimplifyPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddIndVarSimplifyPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddInstructionCombiningPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddInstructionCombiningPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddInstructionSimplifyPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddInstructionSimplifyPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddJumpThreadingPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddJumpThreadingPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLICMPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddLICMPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopDeletionPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddLoopDeletionPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopIdiomPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddLoopIdiomPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopRotatePass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddLoopRotatePass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopRerollPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddLoopRerollPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopUnrollPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddLoopUnrollPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopUnrollAndJamPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddLoopUnrollAndJamPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLowerAtomicPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddLowerAtomicPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddMemCpyOptPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddMemCpyOptPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddPartiallyInlineLibCallsPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddPartiallyInlineLibCallsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddReassociatePass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddReassociatePass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddSCCPPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddSCCPPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddScalarReplAggregatesPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddScalarReplAggregatesPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddScalarReplAggregatesPassSSA", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddScalarReplAggregatesPassSSA(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddScalarReplAggregatesPassWithThreshold", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddScalarReplAggregatesPassWithThreshold(PM : LLVMOpaquePassManager? implicit; Threshold : int) : void {}
[extern(cdecl, name="LLVMAddSimplifyLibCallsPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddSimplifyLibCallsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddTailCallEliminationPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddTailCallEliminationPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddDemoteMemoryToRegisterPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddDemoteMemoryToRegisterPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddVerifierPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddVerifierPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddCorrelatedValuePropagationPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddCorrelatedValuePropagationPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddEarlyCSEPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddEarlyCSEPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddEarlyCSEMemSSAPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddEarlyCSEMemSSAPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLowerExpectIntrinsicPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddLowerExpectIntrinsicPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLowerConstantIntrinsicsPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddLowerConstantIntrinsicsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddTypeBasedAliasAnalysisPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddTypeBasedAliasAnalysisPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddScopedNoAliasAAPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddScopedNoAliasAAPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddBasicAliasAnalysisPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddBasicAliasAnalysisPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddUnifyFunctionExitNodesPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddUnifyFunctionExitNodesPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLowerSwitchPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddLowerSwitchPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddPromoteMemoryToRegisterPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddPromoteMemoryToRegisterPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddAddDiscriminatorsPass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddAddDiscriminatorsPass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddLoopVectorizePass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddLoopVectorizePass(PM : LLVMOpaquePassManager? implicit) : void {}
[extern(cdecl, name="LLVMAddSLPVectorizePass", linux_library="libLLVM.so", windows_library="LLVM-C", macos_library="libLLVM-C.dylib")]
def LLVMAddSLPVectorizePass(PM : LLVMOpaquePassManager? implicit) : void {}
