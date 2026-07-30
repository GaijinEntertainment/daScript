options gen2
options _comment_hygiene = true
options indenting = 4
options strict_smart_pointers = true

module imgui_harness_lint shared private

//! Default-on lint for files that ``require imgui/imgui_harness``.
//!
//! Forbids direct calls into the windowed-backend modules so harness
//! consumers stay backend-agnostic — the harness dispatches windowed
//! vs headless internally, and any raw GLFW/GL/imgui_live call breaks
//! ``--headless`` execution.
//!
//! Code (macro_error severity, error code 50503):
//!   HARNESS001 — call into a forbidden backend module from a
//!                harness-using file. Opt-out per-file:
//!                ``options _allow_glfw_calls = true`` (scaffolding only,
//!                target end-state = no opt-out — same pattern as
//!                PR #33's ``_allow_imgui_legacy``).
//!
//! Forbidden modules:
//!   glfw_boost / opengl_boost / glfw_live / opengl_live / imgui_live
//!
//! Scoping: walks ``prog.getThisModule`` only — transitively-required
//! modules (including ``imgui_harness`` itself, which legitimately calls
//! into the backends as the wrapper) are skipped. Macro-generated bodies
//! and macro-synthesized call sites are filtered by ``fileInfo`` mismatch,
//! same convention as ``imgui_lint.das``.

require daslib/ast_boost

let private FORBIDDEN_MODULES : table<string> <- {
    "glfw_boost", "opengl_boost", "glfw_live", "opengl_live", "imgui_live"
}

class HarnessLintVisitor : AstVisitor {
    @do_not_delete current_function : Function?

    def override preVisitFunction(var fn : FunctionPtr) : void {
        current_function = fn
    }

    def override visitFunction(var fn : FunctionPtr) : FunctionPtr {
        current_function = null
        return <- fn
    }

    def override preVisitExprCall(var expr : ExprCall?) : void {
        if (expr.func == null || expr.func._module == null
                || (current_function != null
                    && (current_function.flags.generated
                        || (current_function.at.fileInfo != null
                            && expr.at.fileInfo != current_function.at.fileInfo)))) return
        let mname = string(expr.func._module.name)
        if (key_exists(FORBIDDEN_MODULES, mname)) {
            let fname = string(expr.func.name)
            compiling_program() |> macro_error(expr.at,
                "HARNESS001: {mname}::{fname} is forbidden in files that require imgui_harness — use the harness helpers (harness_init / harness_begin_frame / harness_new_frame / harness_end_frame / harness_shutdown) instead. Per-file escape: `options _allow_glfw_calls = true` (scaffolding only)")
        }
    }
}

[lint_macro]
class HarnessLintMacro : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        let opted_out = prog._options |> find_arg("_allow_glfw_calls") ?as tBool ?? false
        if (opted_out) return true
        var v = new HarnessLintVisitor()
        make_visitor(*v) $(adapter) {
            visit_module(prog, adapter, prog.getThisModule)
        }
        unsafe { delete v; }
        return true
    }
}
