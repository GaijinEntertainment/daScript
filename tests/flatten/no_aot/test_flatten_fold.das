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
                // honor the unit's own `options _flatten_no_fast_math` — gated rewrites are
                // skipped there by design, and the oracle must mirror that (no false flags)
                let fastMath = !(program._options |> find_arg("_flatten_no_fast_math") ?as tBool ?? false)
                program_for_each_module(program) $(mod) {
                    for_each_function(mod, "") $(func) {
                        if (func.body == null || func.flags.builtIn || !is_flatten_output(func)) {
                            return
                        }
                        checked ++
                        let r <- flatten_fold_residuals(func, fastMath)
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

// Compile `path` and return describe() of each `*_flat` twin body, keyed by twin name — the
// structural skip-proof half of the `_flatten_no_fast_math` tests (a skipped fold computes the
// same VALUE, so only the surviving shape proves the gate fired).
def private twin_bodies(t : T?; path : string) : table<string; string> {
    var bodies : table<string; string>
    var inscope access <- make_file_access("")
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
                        if (func.body != null && !func.flags.builtIn && func.name |> ends_with("_flat")) {
                            bodies["{func.name}"] = "{describe(func.body)}"
                        }
                    }
                }
            }
        }
    }
    return <- bodies
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
    t |> run("no-fast-math corpus: option-aware oracle clean") @(t : T?) {
        let n = check_unit(t, "", "{root}/tests/flatten/test_flatten_nofastmath.das")
        t |> success(n >= 9, "expected >=9 twins, checked {n}")
    }
    t |> run("no-fast-math corpus: gated shapes survive (skip-proof)") @(t : T?) {
        var bodies <- twin_bodies(t, "{root}/tests/flatten/test_flatten_nofastmath.das")
        let mul0 = bodies?["nf_mul0_flat"] ?? ""
        t |> success(mul0 |> find("* 0f") >= 0, "x*0 must survive: {mul0}")
        let vmul0 = bodies?["nf_vmul0_flat"] ?? ""
        t |> success(vmul0 |> find("float3(0") >= 0, "v*vec0 must survive: {vmul0}")
        let subself = bodies?["nf_subself_flat"] ?? ""
        t |> success(subself |> find(" - ") >= 0, "x-x must survive: {subself}")
        let reas = bodies?["nf_reassoc_flat"] ?? ""
        t |> success(reas |> find("0.75") < 0 && reas |> find("0.25") >= 0, "float consts must stay scattered: {reas}")
        let lrp = bodies?["nf_lerp0_flat"] ?? ""
        t |> success(lrp |> find("lerp") >= 0, "lerp(a,b,0) must round-trip back to lerp: {lrp}")
        let imul0 = bodies?["nf_imul0_flat"] ?? ""
        t |> success(imul0 |> find("* 0") < 0, "int n*0 must STILL fold: {imul0}")
        let ire = bodies?["nf_ireassoc_flat"] ?? ""
        t |> success(ire |> find("2 +") < 0, "int consts must gather+fold: {ire}")
        let pre = bodies?["nf_preshader_flat"] ?? ""
        t |> success(pre |> find("_preshader_0") >= 0, "preshader extraction must still run: {pre}")
        delete bodies
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
