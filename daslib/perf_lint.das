options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers = true

module perf_lint shared private

//! Performance lint module.
//!
//! Detects common performance anti-patterns in daslang code at compile time.
//! When this module is required, a lint pass runs after compilation and reports
//! warnings as ``CompilationError::performance_lint`` (error code 40217).
//!
//! Rules:
//!   PERF001 — string += in loop (O(n²))
//!   PERF002 — character_at in loop with loop variable index (O(n) per call)
//!   PERF003 — character_at anywhere (info: O(n) per call)
//!   PERF004 — string interpolation reassignment in loop (O(n²))
//!   PERF005 — length(string) in while condition (strlen each iteration)

require daslib/ast_boost

// ---------------------------------------------------------------------------
// Visitor
// ---------------------------------------------------------------------------

struct VarStackEntry {
    @do_not_delete v : Variable?
    depth : int
    is_iter : bool
}

class PerfLintVisitor : AstVisitor {
    astVisitorAdapter : smart_ptr<ast::VisitorAdapter>
    compile_time_errors : bool
    // variable + loop tracking
    loop_depth : int = 0
    in_closure : int = 0
    var_stack : array<VarStackEntry>
    scope_stack : array<int>
    // while tracking
    in_while_cond : bool = false
    // counter-based detection state
    in_character_at_call : int = 0
    @do_not_delete current_character_at : ExprCall?
    in_length_while_call : int = 0
    in_string_builder_check : int = 0
    @do_not_delete perf004_target_var : Variable?
    @do_not_delete perf004_save_stack : array<Variable?>
    // reported character_at locations (to avoid duplicate PERF002+PERF003)
    @do_not_delete reported_character_at : array<ExprCall?>
    warning_count : int = 0

    def PerfLintVisitor() {
        pass
    }

    def perf_warning(text : string; at : LineInfo) : void {
        warning_count ++
        if (compile_time_errors) {
            compiling_program() |> macro_performance_warning(at, text)
        } else {
            print("performance warning: {text} at {describe(at)}\n")
        }
    }

    // --- variable scope helpers ---

    def is_loop_variable(v : Variable?) : bool {
        if (v == null) {
            return false
        }
        for (entry in var_stack) {
            if (entry.v == v && entry.is_iter) {
                return true
            }
        }
        return false
    }

    def is_defined_outside_loop(v : Variable?) : bool {
        if (v == null || loop_depth == 0) {
            return false
        }
        if (self->is_loop_variable(v)) {
            return false
        }
        for (entry in var_stack) {
            if (entry.v == v) {
                return entry.depth < loop_depth
            }
        }
        // not on stack — function argument or global — outside any loop
        return true
    }

    // --- expression helpers ---

    def find_string_var_from_expr(expr : Expression?) : Variable? {
        if (expr == null) {
            return null
        }
        // Unwrap ExprRef2Value (compiler inserts these for value-type reads)
        var inner = expr
        if (inner is ExprRef2Value) {
            inner = get_ptr((inner as ExprRef2Value).subexpr)
        }
        if (inner != null && (inner is ExprVar)) {
            var evar = inner as ExprVar
            if (evar.variable != null && evar.variable._type != null) {
                if (evar.variable._type.baseType == Type.tString) {
                    return get_ptr(evar.variable)
                }
            }
        }
        return null
    }

    // --- scope tracking ---

    def override preVisitExprBlock(blk : smart_ptr<ExprBlock>) : void {
        if (blk.blockFlags.isClosure) {
            in_closure ++
            perf004_save_stack |> push(perf004_target_var)
            perf004_target_var = null
        }
        scope_stack |> push(length(var_stack))
    }

    def override visitExprBlock(var blk : smart_ptr<ExprBlock>) : ExpressionPtr {
        if (blk.blockFlags.isClosure) {
            in_closure --
            if (length(perf004_save_stack) > 0) {
                perf004_target_var = perf004_save_stack |> back()
                perf004_save_stack |> pop()
            }
        }
        if (length(scope_stack) > 0) {
            var_stack |> resize(scope_stack |> back())
            scope_stack |> pop()
        }
        return <- blk
    }

    // --- variable declaration tracking ---

    def override preVisitExprLetVariable(expr : smart_ptr<ExprLet>; var v : VariablePtr; last : bool) : void {
        if (in_closure == 0) {
            var_stack |> push(VarStackEntry(v = get_ptr(v), depth = loop_depth, is_iter = false))
        }
    }

    // --- loop tracking ---

    def override preVisitExprFor(expr : smart_ptr<ExprFor>) : void {
        if (in_closure == 0) {
            scope_stack |> push(length(var_stack))
            loop_depth ++
        }
    }

    def override preVisitExprForVariable(expr : smart_ptr<ExprFor>; var v : VariablePtr; last : bool) : void {
        if (in_closure == 0) {
            var_stack |> push(VarStackEntry(v = get_ptr(v), depth = loop_depth - 1, is_iter = true))
        }
    }

    def override visitExprFor(var expr : smart_ptr<ExprFor>) : ExpressionPtr {
        if (in_closure == 0) {
            loop_depth --
            if (length(scope_stack) > 0) {
                var_stack |> resize(scope_stack |> back())
                scope_stack |> pop()
            }
        }
        return <- expr
    }

    def override preVisitExprWhile(expr : smart_ptr<ExprWhile>) : void {
        if (in_closure == 0) {
            scope_stack |> push(length(var_stack))
            loop_depth ++
            in_while_cond = true
        }
    }

    def override preVisitExprWhileBody(expr : smart_ptr<ExprWhile>; body : ExpressionPtr) : void {
        in_while_cond = false
    }

    def override visitExprWhile(var expr : smart_ptr<ExprWhile>) : ExpressionPtr {
        if (in_closure == 0) {
            loop_depth --
            if (length(scope_stack) > 0) {
                var_stack |> resize(scope_stack |> back())
                scope_stack |> pop()
            }
        }
        in_while_cond = false
        return <- expr
    }

    // --- PERF001: string += in loop ---

    def override preVisitExprOp2(expr : smart_ptr<ExprOp2>) : void {
        if (in_closure > 0) {
            return
        }
        if (loop_depth > 0 && expr.op == "+=") {
            let v = self->find_string_var_from_expr(get_ptr(expr.left))
            if (v != null && self->is_defined_outside_loop(v)) {
                self->perf_warning("PERF001: string += in loop creates O(n^2) allocations; use build_string() instead", expr.at)
            }
        }
    }

    // --- PERF002 + PERF003: character_at (counter-based) ---

    def override preVisitExprCall(var expr : smart_ptr<ExprCall>) : void {
        if (in_closure > 0) {
            return
        }
        if (expr.func.name == "character_at" && expr.func._module.name == "strings") {
            var ecall = get_ptr(expr)
            // PERF002: start tracking — any loop var inside this call triggers warning
            if (loop_depth > 0) {
                in_character_at_call ++
                current_character_at = ecall
            }
        }
        // PERF005: length(string) in while condition — start tracking
        if (in_while_cond && expr.func.name == "length" && expr.func._module.name == "strings") {
            in_length_while_call ++
        }
    }

    def override visitExprCall(var expr : smart_ptr<ExprCall>) : ExpressionPtr {
        if (in_closure == 0) {
            if (expr.func.name == "character_at" && expr.func._module.name == "strings") {
                var ecall = get_ptr(expr)
                if (loop_depth > 0 && in_character_at_call > 0) {
                    in_character_at_call --
                    current_character_at = null
                }
                // PERF003: character_at anywhere (info) — deferred so PERF002 can claim it first
                if (!reported_character_at |> has_value(ecall)) {
                    self->perf_warning("PERF003: character_at is O(n) due to strlen; consider peek_data() for hot paths", expr.at)
                }
            }
            if (expr.func.name == "length" && expr.func._module.name == "strings") {
                if (in_length_while_call > 0) {
                    in_length_while_call --
                }
            }
        }
        return <- expr
    }

    // --- PERF004: string builder reassignment in loop (counter-based) ---

    def override preVisitExprCopy(expr : smart_ptr<ExprCopy>) : void {
        if (in_closure > 0 || loop_depth == 0) {
            return
        }
        var v = self->find_string_var_from_expr(get_ptr(expr.left))
        if (v != null && self->is_defined_outside_loop(v) && get_ptr(expr.right) is ExprStringBuilder) {
            perf004_target_var = v
        }
    }

    def override visitExprCopy(var expr : smart_ptr<ExprCopy>) : ExpressionPtr {
        perf004_target_var = null
        return <- expr
    }

    def override preVisitExprMove(expr : smart_ptr<ExprMove>) : void {
        if (in_closure > 0 || loop_depth == 0) {
            return
        }
        var v = self->find_string_var_from_expr(get_ptr(expr.left))
        if (v != null && self->is_defined_outside_loop(v) && get_ptr(expr.right) is ExprStringBuilder) {
            perf004_target_var = v
        }
    }

    def override visitExprMove(var expr : smart_ptr<ExprMove>) : ExpressionPtr {
        perf004_target_var = null
        return <- expr
    }

    def override preVisitExprStringBuilder(expr : smart_ptr<ExprStringBuilder>) : void {
        if (perf004_target_var != null) {
            in_string_builder_check ++
        }
    }

    def override visitExprStringBuilder(var expr : smart_ptr<ExprStringBuilder>) : ExpressionPtr {
        if (in_string_builder_check > 0) {
            in_string_builder_check --
        }
        return <- expr
    }

    // --- Central ExprVar handler (PERF002, PERF004, PERF005) ---

    def override preVisitExprVar(expr : smart_ptr<ExprVar>) : void {
        if (in_closure > 0) {
            return
        }
        let v = get_ptr(expr.variable)
        // PERF002: loop variable inside character_at call
        if (in_character_at_call > 0 && self->is_loop_variable(v)) {
            if (current_character_at != null && !reported_character_at |> has_value(current_character_at)) {
                self->perf_warning("PERF002: character_at(s, i) in loop is O(n) per call (strlen + bounds check each time); use peek_data(s) to access characters as array<uint8>", expr.at)
                reported_character_at |> push(current_character_at)
            }
        }
        // PERF004: target var inside string builder
        if (in_string_builder_check > 0 && perf004_target_var != null && v == perf004_target_var) {
            self->perf_warning("PERF004: rebuilding string via interpolation in loop creates O(n^2) allocations; use build_string() instead", expr.at)
        }
        // PERF005: string var inside length() in while condition
        if (in_length_while_call > 0 && v != null) {
            if (expr.variable._type != null && expr.variable._type.baseType == Type.tString) {
                if (!expr.variable.access_flags.access_ref) {
                    self->perf_warning("PERF005: length(string) in while condition recomputes strlen each iteration; cache in a local variable", expr.at)
                }
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

def public perf_lint(prog : ProgramPtr; compile_time_errors : bool) : int {
    //! Runs the performance lint visitor on the compiled program.
    //! Returns the number of warnings found.
    var astVisitor = new PerfLintVisitor(compile_time_errors = compile_time_errors)
    unsafe {
        astVisitor.astVisitorAdapter <- make_visitor(*astVisitor)
    }
    visit(prog, astVisitor.astVisitorAdapter)
    let count = astVisitor.warning_count
    astVisitor.astVisitorAdapter := null
    unsafe {
        delete astVisitor
    }
    return count
}

// ---------------------------------------------------------------------------
// Lint macro (auto-runs when module is required)
// ---------------------------------------------------------------------------

[lint_macro]
class PerfLintMacro : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        perf_lint(prog, true)
        return true
    }
}
