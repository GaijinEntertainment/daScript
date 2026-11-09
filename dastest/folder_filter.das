options gen2
module folder_filter shared

require daslib/ast_boost
require daslib/debugger
require daslib/fio
require log

//! compiles the `.das_test` at `root`, when there is one, and hands
//! `blk` a predicate answering its `can_visit_folder` for a folder
//! name; without a `.das_test` every folder is visitable. Returns
//! `blk`'s answer, or false when the `.das_test` did not compile
def with_folder_filter(root : string;
                       blk : block<(can_visit : block<(name : string) : bool>) : bool>) : bool {
    let das_test_path = path_join(root, ".das_test")
    if (!stat(das_test_path).is_reg) {
        return blk |> invoke($(_name : string) : bool {
            return true
        })
    }
    var success = false
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            compile_file(das_test_path, access, unsafe(addr(mg)), cop) $(ok, program, output) {
                if (!ok) {
                    log::error("Failed to compile {das_test_path}\n{output}")
                    return
                }
                simulate(program) $(sok, context, serrors) {
                    if (!sok) {
                        log::error("Failed to simulate {das_test_path}\n{serrors}")
                        return
                    }
                    success = blk |> invoke($(name : string) : bool {
                        var result = true
                        unsafe {
                            *context |> invoke_in_context("can_visit_folder", name, addr(result))
                        }
                        return result
                    })
                }
            }
        }
    }
    return success
}
