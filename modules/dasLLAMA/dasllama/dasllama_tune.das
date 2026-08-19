options gen2
options no_aot = true  // macro module — compile-time only, never AOT-compiled (like dasllama_par)
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_tune shared public

require dasllama/dasllama_lint public
require daslib/ast
require daslib/rtti
require daslib/ast_boost
require daslib/templates_boost
require daslib/json_boost  // read_json / read_json_field (re-exports daslib/json)
require llvm/daslib/llvm_tune   // the per-app tune sidecar (path, staleness, kernels read)

// ===== Tune spec =====

//! Per-loop hint spec, addressed to a `for [tune=idx] (...)` marker in the template body.
struct TuneHint {
    tune      : int    // which [tune=N] loop this targets
    vectorize : bool
    width     : int    // 0 => omit vectorize_width
    unroll    : int    // 0 => omit unroll_count
}

//! One named permutation: a name suffix + the per-loop hints to stamp on the clone.
struct TunePerm {
    suffix : string
    hints  : array<TuneHint>
}

def private build_grid() : array<TunePerm> {
    // unroll=1 OMITS unroll_count so each row stays byte-identical to the original hand hints (DEFAULT_PERM vec8_u2 included); width 4/32 rarely win on M1 but x64 AVX-512 picks them.
    let WIDTHS = fixed_array(4, 8, 16, 32)
    let UNROLLS = fixed_array(2, 4, 8)   // unroll-only rows; u1 == plain
    var grid : array<TunePerm>
    grid |> reserve(1 + length(UNROLLS) + length(WIDTHS) * (1 + length(UNROLLS)))
    grid |> emplace(TunePerm(suffix = "plain"))
    for (u in UNROLLS) {
        grid |> emplace(TunePerm(suffix = "u{u}", hints <- [TuneHint(tune = 1, unroll = u)]))
    }
    for (w in WIDTHS) {
        grid |> emplace(TunePerm(suffix = "vec{w}", hints <- [TuneHint(tune = 1, vectorize = true, width = w)]))
        for (u in UNROLLS) {
            grid |> emplace(TunePerm(suffix = "vec{w}_u{u}",
                hints <- [TuneHint(tune = 1, vectorize = true, width = w, unroll = u)]))
        }
    }
    return <- grid
}

// ===== [tune=N] marker helpers =====

def private has_tune_arg(loop : ExprFor?) : bool {
    for (a in loop.annotations) {
        if (a.name == "tune") return true
    }
    return false
}

def private tune_index_of(loop : ExprFor?) : int {
    for (a in loop.annotations) {
        if (a.name == "tune") return a.iValue
    }
    return -1
}

// The JIT's append_loop_hint_operand fails on an unknown hint, so the `tune`
// marker MUST be removed before the clone reaches codegen.
def private strip_tune_marker(var loop : ExprFor?) {
    var idx = -1
    for (a, i in loop.annotations, count()) {
        if (a.name == "tune") {
            idx = i
            break
        }
    }
    if (idx >= 0) loop.annotations |> erase(idx)
}

def private apply_hint(var loop : ExprFor?; h : TuneHint) {
    if (h.vectorize) loop |> append_vectorize(h.width)
    if (h.unroll > 0) loop |> append_unroll(h.unroll)
}

// ===== body walk =====
// preVisitExprFor yields a const node (detection only); visitExprFor yields a
// `var` node — the one place we can mutate a loop's annotations.

class private TuneScan : AstVisitor {
    hasTune : bool
    def override preVisitExprFor(expr : ExprFor?) : void {
        if (has_tune_arg(expr)) { hasTune = true }
    }
}

def private func_has_tune_loop(func : FunctionPtr) : bool {
    var vis = new TuneScan()
    make_visitor(*vis) $(adapter) {
        visit(func.body, adapter)
    }
    let r = vis.hasTune
    unsafe { delete vis }
    return r
}

class private ApplyPermVisitor : AstVisitor {
    hints : array<TuneHint>
    def override visitExprFor(var expr : ExprFor?) : ExpressionPtr {
        if (has_tune_arg(expr)) {
            let ti = tune_index_of(expr)
            strip_tune_marker(expr)
            for (h in hints) {
                if (h.tune == ti) apply_hint(expr, h)
            }
        }
        return <- expr
    }
}

def private apply_perm(var clone : FunctionPtr; perm : TunePerm) {
    var vis = new ApplyPermVisitor()
    vis.hints := perm.hints
    make_visitor(*vis) $(adapter) {
        visit(clone.body, adapter)
    }
    unsafe { delete vis }
}

// Clone `tmpl` as `<tmpl.name>__<perm.suffix>`, stamp the permutation's hints onto the
// [tune=N] loops, and register it in `mod`. Shared by [dasllama_tune] (in-module scan) and
// [dasllama_grid] (cross-module targeted emission). The clone name IS the registry key.
def private emit_variant(var mod : Module?; tmpl : FunctionPtr; perm : TunePerm; var errors : das_string) : bool {
    var clone = clone_function(tmpl)
    clone.name := "{tmpl.name}__{perm.suffix}"
    clone.moreFlags.isTemplate = false
    clone.flags.generated = true
    apply_perm(clone, perm)
    if (!(mod |> add_function(clone))) {
        errors := "dasllama grid: failed to add {clone.name}"
        return false
    }
    return true
}

// ===== the trigger =====

//! `[dasllama_tune]` on any (dummy) struct scans the module for `def template` functions with
//! a `for [tune=N] (...)` loop and emits one clone per grid permutation (`<name>__<suffix>`).
//! The template stays the single source of truth; profiling picks the winner.
[structure_macro(name="dasllama_tune")]
class private TuneMacro : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        var mod = compiling_module()
        var tunable : array<FunctionPtr>
        for_each_function(mod, "") $(func) {
            if (func.moreFlags.isTemplate && func_has_tune_loop(func)) {
                tunable |> push(func)
            }
        }
        let grid <- build_grid()
        for (func in tunable) {
            for (perm in grid) {
                if (!emit_variant(mod, func, perm, errors)) return false
            }
        }
        return true
    }
}

// ===== [dasllama_grid]: targeted grid emission + fn-ptr registry =====

// Build `<tmpl.name>_variants() : array<tuple<string; <sig>>>`, one row per grid perm:
// `(suffix, @@<tmpl.name>__<suffix>)`. `<sig>` is the template's own function type, so the
// registry is generic across kernels (no shared Variant type). The profiler benches these.
def private emit_registry(var mod : Module?; tmpl : FunctionPtr; grid : array<TunePerm>; var errors : das_string) : bool {
    let sig = function_to_type(tmpl)
    var body : array<ExpressionPtr>
    body |> reserve(length(grid) + 2)   // decl + one row per perm + return
    body |> push <| qmacro_expr() {
        var result : array<tuple<string; $t(sig)>>
    }
    for (perm in grid) {
        var fnAddr = new ExprAddr(at = tmpl.at, target := "{tmpl.name}__{perm.suffix}")
        body |> push <| qmacro_expr() {
            result |> push(($v(perm.suffix), $e(fnAddr)))
        }
    }
    body |> push <| qmacro_expr() {
        return <- result
    }
    var fn = qmacro_function("{tmpl.name}_variants") $() : array<tuple<string; $t(sig)>> {
        $b(body)
    }
    fn.flags.generated = true
    if (!(mod |> add_function(fn))) {
        errors := "dasllama_grid: failed to add {tmpl.name}_variants"
        return false
    }
    return true
}

// Emit `dasllama_tuned_fallbacks() : table<string; string>` from the bank — the harness reads a
// kernel's shipped default from here instead of restating it as a literal that a per-ISA chain
// would silently outdate.
def private emit_fallback_registry(var mod : Module?; var errors : das_string) : bool {
    var body : array<ExpressionPtr>
    body |> reserve(length(g_fallbacks) + 2)
    body |> push <| qmacro_expr() {
        var result : table<string; string>
    }
    for (k, v in keys(g_fallbacks), values(g_fallbacks)) {
        body |> push <| qmacro_expr() {
            result[$v(k)] = $v(v)
        }
    }
    body |> push <| qmacro_expr() {
        return <- result
    }
    var fn = qmacro_function("dasllama_tuned_fallbacks") $() : table<string; string> {
        $b(body)
    }
    fn.flags.generated = true
    if (!(mod |> add_function(fn))) {
        errors := "dasllama_fallbacks: failed to add dasllama_tuned_fallbacks"
        return false
    }
    return true
}

//! `[dasllama_fallbacks]` on any (dummy) struct emits `dasllama_tuned_fallbacks()`, mapping every
//! `[tuned]` kernel to the fallback perm it resolved to ON THIS BOX. Declare it AFTER the requires
//! that pull in the kernels, so every `[tuned]` apply has run by the time this one does.
[structure_macro(name="dasllama_fallbacks")]
class private FallbacksMacro : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        return emit_fallback_registry(compiling_module(), errors)
    }
}

//! `[dasllama_grid(src="dot_template")]` on any (dummy) struct emits the full grid of
//! `<src>__<suffix>` variant clones (cross-module) plus a `<src>_variants()` fn-ptr registry
//! the profiler harness benches. Replaces hand-written per-perm `[tuned]` stubs + `variants()`.
[structure_macro(name="dasllama_grid")]
class private GridMacro : AstStructureAnnotation {
    def override apply(var st : StructurePtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        let src = find_arg(args, "src")
        if (!(src is tString)) {
            errors := "dasllama_grid: requires src=\"<template_name>\""
            return false
        }
        var tmpl = find_template_function(src as tString)
        if (tmpl == null) {
            errors := "dasllama_grid: template function not found: {src as tString}"
            return false
        }
        var mod = compiling_module()
        let grid <- build_grid()
        for (perm in grid) {
            if (!emit_variant(mod, tmpl, perm, errors)) return false
        }
        return emit_registry(mod, tmpl, grid, errors)
    }
}

// ===== final assembly: [tuned] =====

let private DEFAULT_PERM = "vec8_u2"

def private tuned_template_name(func : FunctionPtr; args : AnnotationArgumentList) : string {
    let v = find_arg(args, "src")     // `src="foo_template"`; else convention below
    if (v is tString) return v as tString
    return "{func.name}_template"     // convention: dot -> dot_template
}

//! The ONE per-app tune sidecar (`<app>.tune.json` beside the root script/binary;
//! `DAS_TUNE_MANIFEST` env overrides) — shared by `[tuned]` compile-time reads, runtime apply,
//! `[tune]` generator winners, and the tuner harness write. Loop hints live under `"kernels"`, knobs under `"runtime"`.
def box_profile_path() : string {
    return tune_manifest_path()
}

//! The sidecar is older than the running binary — its measurements are dead (the runtime
//! apply skips it; the compile-time reads already treat it as absent).
def box_profile_stale() : bool {
    return tune_sidecar_stale(tune_manifest_path())
}

// Compile-time read of the sidecar's "kernels" section written by the tuner harness:
// function name -> winning perm suffix. "" when the sidecar is absent, stale, or lacks
// the key (macros can do fio).
def private config_perm_for(fname : string) : string {
    return tune_manifest_get(fname)
}

// Every [tuned] kernel's shipped fallback AS RESOLVED HERE — a `;`-chain picks per ISA, so this
// is box-specific and cannot be restated as a literal anywhere else. [dasllama_fallbacks] emits it.
var private g_fallbacks : table<string; string>

def private fallback_perm(args : AnnotationArgumentList) : string {
    let dv = find_arg(args, "fallback")
    if (dv is tString) {
        // ';'-chain of `suffix` or `suffix:requires` — one annotation, a per-ISA default
        let picked = tune_pick_fallback(dv as tString)
        if (!empty(picked)) return picked
    }
    return DEFAULT_PERM
}

def private report_perm(fname, perm, source : string) : string {
    if (tune_verbosity() == "verbose") {
        print("dasllama_tune: {fname} <- {perm} ({source})\n")
    }
    return perm
}

// Precedence: `perm=` pin > sidecar "kernels" entry > the fallback chain > DEFAULT_PERM.
// (`fallback`, not `default` — `default` is a grammar keyword, won't parse as an arg name.)
def private resolve_perm(func : FunctionPtr; args : AnnotationArgumentList) : string {
    let fname = "{func.name}"
    let fb = fallback_perm(args)
    g_fallbacks[fname] = fb
    let av = find_arg(args, "perm")
    if (av is tString) return av as tString
    let cfg = config_perm_for(fname)
    if (!empty(cfg)) {
        return report_perm(fname, cfg, box_profile_path())
    }
    let declared = find_arg(args, "fallback") is tString
    return report_perm(fname, fb, declared ? "fallback" : "default")
}

// Template lives in the compiling module for production `[tuned]`; the profiler harness
// is a separate module, so fall back to a program-wide scan.
def private find_template_function(name : string) : FunctionPtr {
    var fn = compiling_module() |> find_unique_function(name, true)
    if (fn != null) return fn
    var found : FunctionPtr
    program_for_each_module(compiling_program()) $(mod) {
        if (found == null) {
            var f = find_unique_function(mod, name, true)
            if (f != null) { found = f }
        }
    }
    return found
}

//! `[tuned]` on an empty-body function clones its source template (`<name>_template`, or
//! `src="..."`) and stamps the selected permutation's hints onto `[tune=N]` loops. The annotated
//! function IS the real symbol. Perm precedence: `perm=` pin > sidecar > `fallback=` > vec8_u2.
[function_macro(name="tuned")]
class private TunedMacro : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        let tmpl_name = tuned_template_name(func, args)
        var tmpl = find_template_function(tmpl_name)
        if (tmpl == null) {
            errors := "tuned: template function not found: {tmpl_name}"
            return false
        }
        func.body = clone_expression(tmpl.body)
        let suffix = resolve_perm(func, args)
        let grid <- build_grid()
        for (perm in grid) {
            if (perm.suffix == suffix) {
                apply_perm(func, perm)
                return true
            }
        }
        errors := "tuned: unknown perm '{suffix}' (not in grid)"
        return false
    }
}
