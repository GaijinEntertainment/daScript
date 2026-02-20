options gen2
options no_aot

// Tutorial macro module: pass macro (AstPassMacro).
//
// AstPassMacro is a whole-program macro invoked during compilation.
// It has a single method:
//   apply(prog : ProgramPtr; mod : Module?) : bool
// Five annotations control WHEN the macro runs:
//   [infer_macro]        — after clean inference (returning true re-infers)
//   [dirty_infer_macro]  — during each dirty inference pass
//   [lint_macro]         — after successful compilation, read-only analysis
//   [global_lint_macro]  — same as lint_macro but for ALL modules
//   [optimization_macro] — during optimization passes
//
// Section 1 — [lint_macro]: runs after inference, prints a compile-time
//   summary.  Lint macros are for read-only analysis; they cannot modify
//   the AST.  `mod` is the module that registered the macro.
//
// Section 2 — [infer_macro]: instruments every function body with a
//   _trace_enter() call, following the heartbeat.das pattern.
//   Returns true when changes are made so the compiler re-infers.

module pass_macro_mod

require ast
require daslib/ast_boost
require daslib/templates_boost

// ---- Section 1: lint_macro — compile-time analysis -------------------------

[lint_macro]
class CodeStatsLint : AstPassMacro {
    //! Lint pass that reports module statistics at compile time.
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        // `mod` is the module that owns this macro (pass_macro_mod).
        // for_each_function(mod, "") iterates its functions.
        var nFunctions = 0
        for_each_function(mod, "") <| $(func) {
            nFunctions++
        }
        print("[lint] pass_macro_mod defines {nFunctions} function(s)\n")
        return true
    }
}

// ---- Section 2: infer_macro — AST transformation ---------------------------

def public _trace_enter(name : string) {
    //! Prints a trace message when entering a function.
    print(">>> {name}\n")
}

class TraceCallsVisitor : AstVisitor {
    //! AST visitor that inserts _trace_enter() at the start of every function.
    astChanged : bool = false
    @do_not_delete func : Function?
    def override preVisitFunction(var fun : FunctionPtr) {
        func = get_ptr(fun)
    }
    def override visitFunction(var fun : FunctionPtr) : FunctionPtr {
        // Skip our own helper to avoid infinite recursion at runtime.
        if (string(fun.name) == "_trace_enter") {
            func = null
            return <- fun
        }
        if (fun.body == null || !(fun.body is ExprBlock)) {
            func = null
            return <- fun
        }
        var body = fun.body as ExprBlock
        if (length(body.list) == 0) {
            func = null
            return <- fun
        }
        // Idempotency: skip if already instrumented.
        if ((body.list[0] is ExprCall) && (body.list[0] as ExprCall).name == "_trace_enter") {
            func = null
            return <- fun
        }
        // Insert _trace_enter("function_name") at the beginning of the body.
        let fname = string(fun.name)
        var inscope expr <- qmacro(_trace_enter($v(fname)))
        body.list |> emplace(expr, 0)
        astChanged = true
        func.not_inferred()
        func = null
        return <- fun
    }
}

[infer_macro]
class TraceCallsPass : AstPassMacro {
    //! Pass macro that inserts trace calls at every function entry.
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        var astVisitor = new TraceCallsVisitor()
        var inscope astVisitorAdapter <- make_visitor(*astVisitor)
        visit(prog, astVisitorAdapter)
        var result = astVisitor.astChanged
        unsafe {
            delete astVisitor
        }
        return result
    }
}
