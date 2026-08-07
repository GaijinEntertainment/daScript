options gen2
options indenting = 4
options no_aot     // invokes the compiler at runtime (compile_file) — not AOT-linkable

require dastest/testing_boost public
require daslib/ast
require strings   // nolint:STYLE030,LINT019 — used only inside the das_metal static_if half

// The @role derivation's whole-buffer rule: a tracked buffer handed WHOLE to a same-module
// helper takes its direction from the callee's PARAMETER — `var` means the callee may store
// through it, anything else is read-only, which is exactly what the emitters lower it to
// (`device` vs `device const`). Both directions are gated here: the read fixture compiles
// clean against its declared roles, the write fixture must be REFUSED because the declared
// @role contradicts what the `var` parameter derives. Fixtures live in a non-globbed subdir.

def private compile_issues(file_path : string; lint : bool = false) : tuple<ok : bool; issues : string> {
    var okFlag = false
    var issuesText = ""
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            cop.lint_check = lint   // the un-lensed census is a [lint_macro] — off on a plain compile
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
def test_grid_micro_grammar(t : T?) {
    t |> run("the grid micro-grammar folds integer literals and refuses malformed tokens") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            // the shared mk_grid_dim (dasllama_kernel_access) folds ANY digit token — the metal
            // lens folded only "1" before the hoist; "4l" must fall to the name path and fail
            let good = compile_issues(fixture("_rf_grid_literal"))
            t |> success(good.ok, "grid = \"4, n/4\" compiles (literal dim + literal coverage): {good.issues}")
            let bad = compile_issues(fixture("_rf_grid_badtok"))
            t |> success(!bad.ok, "grid = \"4l\" must be refused, not truncated through to_int")
            t |> success(find(bad.issues, "4l") >= 0, "the refusal names the bad token")
            if (bad.ok) {
                to_log(LOG_ERROR, "ROLE-INFER: bad grid token compiled clean\n")
            }
        } else {
            feint("das_metal is not built on this platform; the lens does not load\n")
        }
    }
}

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
    t |> run("a POINTER into a tracked buffer takes the callee's pointee const") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            // an advancing `var p : T const?` is still a read; only a mutable POINTEE is a write.
            // Keying on the pointer's own const would call the read side a write and miss this.
            let good = compile_issues(fixture("_rf_ptr_through"))
            t |> success(good.ok, "pointer local reads, inline addr() writes: {good.issues}")
            let bad = compile_issues(fixture("_rf_ptr_wrong"))
            t |> success(!bad.ok, "a mutable-pointee pointer derives 'write' — the declared 'read' must be refused")
            t |> success(find(bad.issues, "declares @role='read' but the body derives 'write'") >= 0,
                "the refusal names the contradiction")
            if (bad.ok) {
                to_log(LOG_ERROR, "ROLE-INFER: pointer write-through fixture compiled clean\n")
            }
        } else {
            feint("das_metal is not built on this platform; the lens does not load\n")
        }
    }
    t |> run("a GENERIC helper resolves by base name, not by its mangled instance") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            // lensed: the classifier runs PRE-infer, so the call still carries its bare name
            let bad = compile_issues(fixture("_rf_generic_wrong"))
            t |> success(!bad.ok, "a `var` parameter on a generic still derives 'write'")
            t |> success(find(bad.issues, "declares @role='read' but the body derives 'write'") >= 0,
                "the refusal names the contradiction, not an unresolvable callee")
            // un-lensed: the lint-time census runs POST-infer, where the call is mangled
            let ubad = compile_issues(fixture("dasllama_metal_rf_unlensed"), true)
            t |> success(!ubad.ok, "the un-lensed census resolves the mangled instance name too")
            t |> success(find(ubad.issues, "derives 'write' — fix the stale declaration") >= 0,
                "the census names the stale declaration")
            if (bad.ok || ubad.ok) {
                to_log(LOG_ERROR, "ROLE-INFER: generic-callee fixture compiled clean\n")
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
