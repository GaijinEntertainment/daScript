require cbind/cbind_boost
require daslib/safe_addr
require daslib/strings

[export]
def main
    let pfn = "clang-c/Index.h"
    let pfp = "{get_das_root()}/../libclang/include/"
    var cgb = new CppGenBind(pfn,pfp,DEFAULT_CLANG_ARGUMENTS)
    cgb->generate()
    unsafe
        delete cgb
