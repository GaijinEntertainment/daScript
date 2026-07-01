options gen2
options no_aot = true  // macro module — compile-time only, never AOT-compiled (like dasllama_par)

module dasllama_tune shared public

require daslib/ast
require daslib/rtti
require daslib/ast_boost
require daslib/templates_boost
require daslib/fio         // [tuned] reads the per-box profile at compile time
require daslib/json_boost  // read_json / read_json_field (re-exports daslib/json)

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
    // Full sweep over a single [tune=1] loop: {plain, unroll-only u2/u4/u8} ∪
    // (width {4,8,16,32} × unroll {1,2,4,8}). The unroll=1 column OMITS unroll_count (TuneHint.unroll=0)
    // so each `vecW` row is byte-identical to the original hand-written `[vectorize, vectorize_width=W]`
    // loop — and the DEFAULT_PERM `vec8_u2` is in the grid, so migrated kernels stay bit-identical.
    // width 4/32 rarely win on M1 (1 NEON reg / reg pressure) but must be present: x64 AVX-512 picks them.
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

//! `[dasllama_tune]` on any (dummy) struct scans the compiling module for `def template`
//! functions carrying a `for [tune=N] (...)` loop and emits one clone per grid permutation
//! (`<name>__<suffix>`), stamping that permutation's hints onto the marked loops. The template
//! stays the single source of truth; profiling picks the winner (see profile_* harness).
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

//! `[dasllama_grid(src="dot_template")]` on any (dummy) struct emits the full grid of
//! `<src>__<suffix>` variant clones (cross-module: `src` may live in a required module) plus
//! a `<src>_variants()` fn-ptr registry the profiler harness benches. Replaces the hand-written
//! per-perm `[tuned]` stubs + `variants()` array.
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

let private BOX_PROFILE_PATH = "box_profile.json"

// Compile-time read of the per-box profile written by the tuner harness:
// function name -> winning perm suffix. Returns "" if the file is absent or lacks
// the key (macros can do fio; fread returns "" on a missing file).
def private config_perm_for(fname : string) : string {
    let text = fread(BOX_PROFILE_PATH)
    if (empty(text)) return ""
    var err = ""
    let jv = read_json(text, err)
    if (jv == null) return ""
    return read_json_field(jv, fname, "")
}

// Perm precedence: explicit `perm=` pin (the profiler's per-variant stubs) >
// per-box `box_profile.json` (keyed by the emitted function name) > DEFAULT_PERM
// (the bit-identical fallback when no profile exists).
def private resolve_perm(func : FunctionPtr; args : AnnotationArgumentList) : string {
    let av = find_arg(args, "perm")
    if (av is tString) return av as tString
    let cfg = config_perm_for("{func.name}")
    if (!empty(cfg)) {
        print("dasllama_tune: {func.name} <- {cfg} (box_profile.json)\n")
        return cfg
    }
    return DEFAULT_PERM
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

//! `[tuned(perm="vec8_u2")]` on an empty-body function clones the body of its source
//! template (`<name>_template`, or `src="..."`) and stamps the selected permutation's
//! hints onto the `[tune=N]` loops. The annotated function IS the real symbol — callers are
//! untouched. The perm comes from the annotation today (default DEFAULT_PERM); reading it
//! from the profiler's box config is the next hook.
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
