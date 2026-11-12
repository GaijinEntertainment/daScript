require dasbind public
options gen2
module llvm_host shared

//! the host queries the jit plan folds into its cache key - the only LLVM the DLL-cache hit path
//! touches, so the plan requires this and not the full llvm_func binding
let @extern_library llvm = "LLVM.dll"

[extern(cdecl, name="LLVMDisposeMessage", lib=llvm)]
def LLVMDisposeMessage(Message : string implicit) : void {}
[extern(cdecl, name="LLVMGetDefaultTargetTriple", lib=llvm)]
def LLVMGetDefaultTargetTriple() : string {}
[extern(cdecl, name="LLVMGetHostCPUName", lib=llvm)]
def LLVMGetHostCPUName() : string {}
[extern(cdecl, name="LLVMGetHostCPUFeatures", lib=llvm)]
def LLVMGetHostCPUFeatures() : string {}
[extern(cdecl, name="LLVMLoadLibraryPermanently", lib=llvm)]
def LLVMLoadLibraryPermanently(Filename : string implicit) : int {}
[extern(cdecl, name="LLVMSearchForAddressOfSymbol", lib=llvm)]
def LLVMSearchForAddressOfSymbol(symbolName : string implicit) : void? {}
