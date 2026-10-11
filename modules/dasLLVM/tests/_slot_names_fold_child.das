options gen2

require llvm/daslib/llvm_jit_plan
require daslib/rtti
require daslib/ast_boost

//! prints the slot-name fold of the fixture's `other_slots` in a fresh process, for the parent test to compare
[export]
def main() : int {
    let path = "{get_das_root()}/modules/dasLLVM/tests/_slot_names_fixture.das"
    var fold = 0ul
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            compile_file(path, access, unsafe(addr(mg)), cop) $(compiled; program; _issues) {
                return if (!compiled)
                for_each_function(get_this_module(program), "other_slots") $(fn) {
                    fold = fold_extern_slot_names(get_ptr(program), [fn], 0ul)
                }
            }
        }
    }
    print("fold={fold}\n")
    return fold == 0ul ? 1 : 0
}
