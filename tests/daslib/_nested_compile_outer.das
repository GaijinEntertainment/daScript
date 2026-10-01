options gen2

require daslib/ast
require _nested_compile_macro

[init]
def compile_nested() {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            compile_file("{get_das_root()}/tests/daslib/_nested_compile_inner.das", access, unsafe(addr(mg)), cop) $(ok; _program; issues) {
                if (!ok) {
                    panic("the nested compile failed: {issues}")
                }
            }
        }
    }
}

[export]
def main() {
    feint("outer\n")
}
