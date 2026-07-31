options gen2
options indenting = 4
options no_aot     // invokes the compiler at runtime (compile_file) — not AOT-linkable

require dastest/testing_boost public
require daslib/ast
require strings

// The @role derivation's whole-buffer rule: a tracked buffer handed WHOLE to a same-module
// helper takes its direction from the callee's PARAMETER — `var` means the callee may store
// through it, anything else is read-only, which is exactly what the emitters lower it to
// (`device` vs `device const`). Both directions are gated here: the read fixture compiles
// clean against its declared roles, the write fixture must be REFUSED because the declared
// @role contradicts what the `var` parameter derives. Fixtures live in a non-globbed subdir.

def private compile_issues(file_path : string) : tuple<ok : bool; issues : string> {
    var okFlag = false
    var issuesText = ""
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            compile_file(file_path, access, unsafe(addr(mg)), cop) $(ok, _program, issues) {
                okFlag = ok
                issuesText := issues
            }
        }
    }
    return (ok = okFlag, issues = issuesText)
}

def private fixture(name : string) : string => "{get_das_root()}/modules/dasLLAMA/tests/_role_fixtures/{name}.das"

[test]
def test_role_whole_buffer_rule(t : T?) {
    t |> run("a buffer passed whole to a helper takes the callee parameter's direction") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let good = compile_issues(fixture("_rf_read_through"))
            t |> success(good.ok, "read-only helper parameter derives 'read': {good.issues}")
            let bad = compile_issues(fixture("_rf_write_through"))
            t |> success(!bad.ok, "a `var` helper parameter derives 'write' — the declared 'read' must be refused")
            t |> success(find(bad.issues, "declares @role='read' but the body derives 'write'") >= 0,
                "the refusal names the contradiction")
            if (bad.ok) {
                to_log(LOG_ERROR, "ROLE-INFER: write-through fixture compiled clean\n")
            }
        } else {
            feint("das_metal is not built on this platform; the lens does not load\n")
        }
    }
    t |> run("a kernel body that delegates to methods derives roles through the chain") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            // the family shape: base loop + per-format override + a body that only calls run().
            // Before the classifier followed methods this derived NOTHING — every field looked
            // unused, so a class without declared roles would have been staged with no hazards.
            let good = compile_issues(fixture("_rf_method_chain"))
            t |> success(good.ok, "base member and override-only member both derived: {good.issues}")
            let bad = compile_issues(fixture("_rf_method_wrong"))
            t |> success(!bad.ok, "a read-only member declared 'write' must be refused through the chain")
            t |> success(find(bad.issues, "declares @role='write' but the body derives 'read'") >= 0,
                "the refusal names the contradiction")
            if (bad.ok) {
                to_log(LOG_ERROR, "ROLE-INFER: method-chain wrong-role fixture compiled clean\n")
            }
        } else {
            feint("das_metal is not built on this platform; the lens does not load\n")
        }
    }
}
