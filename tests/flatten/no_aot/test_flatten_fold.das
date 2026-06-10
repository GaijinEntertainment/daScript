options gen2
options indenting = 4

require dastest/testing_boost public
require daslib/flatten
require daslib/ast
require daslib/ast_boost
require daslib/rtti
require strings
require daslib/fio

//! Fold-completeness verification over the ACTUAL compiled output. It compiles REAL flatten
//! units and asserts no const-foldable residual (`x*1`/`x+0`/`x*-1`, an all-const call, a
//! const-condition `?:`, a const `!`) survives in the final post-flatten AST — using
//! `flatten_fold_residuals` (daslib/flatten) — the only fold-completeness check (there is no
//! compile-time flag). Running over compiled output, it covers the `[pixel_shader]` backend
//! path uniformly with `[flatten]` (the single-fold + re-infer loop that left the center-tap
//! `uv + float2(0,0)` residual). Three unit kinds:
//!   • every example shader (project-resolved) — the `@pixel_shader` entry function;
//!   • _fold_fixtures.das (this dir) — the `[flatten]` reveal-cascade corpus (`*_flat`);
//!   • test_flatten_const.das — the const-identity corpus. This is the structural FIRED
//!     half of that file's verification; the file itself carries only differential VALUE
//!     checks (a missed fold computes the same value, so it needs this residual proof).
//!
//! Lives in tests/flatten/no_aot/ (excluded from test_aot via tests/.das_test): it requires
//! daslib/flatten at runtime, pulling macro_boost generic instantiations that don't AOT-link
//! in the full-tree batch. Runs in interp + JIT; the residual check is tier-independent.

// A flatten OUTPUT to validate: a `[flatten]` twin (`*_flat`) or a `[pixel_shader]` entry.
def private is_flatten_output(func : FunctionPtr) : bool {
    if (func.name |> ends_with("_flat")) {
        return true
    }
    for (a in func.annotations) {
        if (a.annotation.name == "pixel_shader") {
            return true
        }
    }
    return false
}

// Compile `path` (project-aware when `proj` is non-empty) and assert every flatten output's
// body is residual-free. Returns the number of flatten outputs checked.
def private check_unit(t : T?; proj, path : string) : int {
    var checked = 0
    var inscope access <- make_file_access(proj)
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            cop.version_2_syntax = true
            compile_file(path, access, unsafe(addr(mg)), cop) $(ok; program; iss) {
                if (!ok) {
                    t |> failure("compile failed: {path}\n{iss}")
                    return
                }
                program_for_each_module(program) $(mod) {
                    for_each_function(mod, "") $(func) {
                        if (func.body == null || func.flags.builtIn || !is_flatten_output(func)) {
                            return
                        }
                        checked ++
                        let r <- flatten_fold_residuals(func)
                        t |> equal(length(r), 0, "{base_name(path)}::{func.name}: {r}")
                        let optr <- flatten_opt_residuals(func)
                        t |> equal(length(optr), 0, "{base_name(path)}::{func.name} (opt): {optr}")
                    }
                }
            }
        }
    }
    return checked
}

[test]
def test_flatten_fold(t : T?) {
    let root = get_das_root()
    t |> run("[flatten] reveal-cascade corpus folds clean") @(t : T?) {
        let n = check_unit(t, "", "{root}/tests/flatten/no_aot/_fold_fixtures.das")
        t |> success(n >= 3, "expected >=3 twins, checked {n}")
    }
    t |> run("const-identity corpus folds clean (test_flatten_const.das twins)") @(t : T?) {
        let n = check_unit(t, "", "{root}/tests/flatten/test_flatten_const.das")
        t |> success(n >= 70, "expected >=70 twins, checked {n}")
    }
    t |> run("optimize corpus folds clean (test_flatten_opt.das twins)") @(t : T?) {
        let n = check_unit(t, "", "{root}/tests/flatten/test_flatten_opt.das")
        t |> success(n >= 7, "expected >=7 twins, checked {n}")
    }
    t |> run("mad/lerp corpus fuses clean (test_flatten_mad.das twins)") @(t : T?) {
        let n = check_unit(t, "", "{root}/tests/flatten/test_flatten_mad.das")
        t |> success(n >= 20, "expected >=20 twins, checked {n}")
    }
    t |> run("every example shader folds clean (pixel_shader path)") @(t : T?) {
        let proj = "{root}/examples/flatten/flatten_shaders.das_project"
        let base = "{root}/examples/flatten/shaders"
        var total = 0
        fio::dir(base) $(dirname) {
            let sub = "{base}/{dirname}"
            if (dirname |> starts_with(".") || !stat(sub).is_dir) {
                return
            }
            fio::dir(sub) $(fn) {
                if (fn |> ends_with(".shader")) {
                    total += check_unit(t, proj, "{sub}/{fn}")
                }
            }
        }
        t |> success(total >= 60, "expected >=60 shaders, checked {total}")
    }
}
