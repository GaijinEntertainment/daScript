require cbind/cbind_boost
require daslib/safe_addr
require daslib/strings

[export]
def main
    let pfn = "sample.h"
    let pfp = "{get_das_root()}/modules/dasClangBind/examples/"
    clang_reportAst(pfn,pfp,DEFAULT_CLANG_ARGUMENTS)
    var cgb = new CppGenBind(pfn,pfp,DEFAULT_CLANG_ARGUMENTS)
    cgb.generate_lsp = true
    cgb->generate()
    unsafe
        delete cgb
