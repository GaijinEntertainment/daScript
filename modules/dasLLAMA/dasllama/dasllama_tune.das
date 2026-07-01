options gen2
options no_aot = true  // macro module — compile-time only, never AOT-compiled (like dasllama_par)

module dasllama_tune shared public

require daslib/ast
require daslib/rtti
require daslib/ast_boost
require daslib/templates_boost

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
    // First-increment default grid over a single [tune=1] loop.
    return <- [
        TunePerm(suffix = "plain"),
        TunePerm(suffix = "u2", hints <- [TuneHint(tune = 1, unroll = 2)]),
        TunePerm(suffix = "vec8", hints <- [TuneHint(tune = 1, vectorize = true, width = 8)]),
        TunePerm(suffix = "vec8_u2", hints <- [TuneHint(tune = 1, vectorize = true, width = 8, unroll = 2)])
    ]
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
                var clone = clone_function(func)
                clone.name := "{func.name}__{perm.suffix}"
                clone.moreFlags.isTemplate = false
                clone.flags.generated = true
                apply_perm(clone, perm)
                if (!(mod |> add_function(clone))) {
                    errors := "dasllama_tune: failed to add {clone.name}"
                    return false
                }
            }
        }
        return true
    }
}

// ===== final assembly: [tuned] =====

let private DEFAULT_PERM = "vec8_u2"

def private tuned_template_name(func : FunctionPtr; args : AnnotationArgumentList) : string {
    let v = find_arg(args, "template")
    if (v is tString) return v as tString
    return "{func.name}_template"     // convention: dot -> dot_template
}

def private tuned_perm_suffix(args : AnnotationArgumentList) : string {
    let v = find_arg(args, "perm")
    if (v is tString) return v as tString
    return DEFAULT_PERM
}

//! `[tuned(perm="vec8_u2")]` on an empty-body function clones the body of its source
//! template (`<name>_template`, or `template="..."`) and stamps the selected permutation's
//! hints onto the `[tune=N]` loops. The annotated function IS the real symbol — callers are
//! untouched. The perm comes from the annotation today (default DEFAULT_PERM); reading it
//! from the profiler's box config is the next hook.
[function_macro(name="tuned")]
class private TunedMacro : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        let tmpl_name = tuned_template_name(func, args)
        var tmpl = compiling_module() |> find_unique_function(tmpl_name, true)
        if (tmpl == null) {
            errors := "tuned: template function not found: {tmpl_name}"
            return false
        }
        func.body = clone_expression(tmpl.body)
        let suffix = tuned_perm_suffix(args)
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
