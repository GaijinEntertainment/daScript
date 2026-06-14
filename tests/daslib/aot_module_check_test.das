options gen2
options no_aot

require dastest/testing_boost public
require daslib/ast_boost
require daslib/rtti
require daslib/fio
require strings

def write_fixtures(dir : string) {
    fwrite(path_join(dir, "helper.das"),
        "options gen2\ndef helper_add(a, b : int) : int \{\n    return a + b\n\}\n")
    fwrite(path_join(dir, "main.das"),
        "options gen2\nrequire helper\n[export]\ndef do_add(a, b : int) : int \{\n    return helper_add(a, b)\n\}\n")
}

def compile_main_for_aot(dir : string; fail_on_lack_of_aot_export : bool; var ok : bool&; var issues : string&) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.aot = false
            cop.aot_module = true
            cop.fail_on_lack_of_aot_export = fail_on_lack_of_aot_export
            cop.ignore_shared_modules = false
            compile_file(path_join(dir, "main.das"), access, unsafe(addr(mg)), cop) $(cok; _program; cissues) {
                ok = cok
                issues := cissues
            }
        }
    }
}

[test]
def test_require_module_less_file_fails_aot(t : T?) {
    t |> run("AOT compile errors when a required file is not a module") <| @(t2 : T?) {
        var error : string
        let dir = create_temp_directory("das_aot_modcheck", error)
        write_fixtures(dir)

        var ok = true
        var issues = ""
        compile_main_for_aot(dir, true, ok, issues)
        t2 |> success(!ok, "compile fails for a module-less required file")
        t2 |> success(find(issues, "not setup correctly for AOT") >= 0,
            "diagnostic mentions AOT setup: {issues}")

        var ok2 = false
        var issues2 = ""
        compile_main_for_aot(dir, false, ok2, issues2)
        t2 |> success(ok2, "same file compiles when fail_on_lack_of_aot_export is off: {issues2}")

        rmdir_rec_result(dir)
    }
}
