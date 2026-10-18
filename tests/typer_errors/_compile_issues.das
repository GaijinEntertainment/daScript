options gen2
options no_aot     // requires daslib/ast (no_aot) to invoke compile_file at runtime

module _compile_issues public

require daslib/ast


//! the issues text a compile of `file_path` leaves, under a
//! thread-locked context - the text an error-message test reads
def compile_issues(file_path : string) : string {
    var issuesText = ""
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            compile_file(file_path, access, unsafe(addr(mg)), cop) $(_ok, _program, issues) {
                issuesText := issues
            }
        }
    }
    return issuesText
}
