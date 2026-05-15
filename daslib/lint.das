options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers = true

module lint shared private

//! Lint pass that enables paranoid compilation checks.
//!
//! When this module is required, the ``paranoid`` lint flag is turned on
//! for the current module, enabling stricter compiler diagnostics.
//! See also ``lint_everything`` for applying lint to all modules.

require daslib/strings_boost
require daslib/ast_boost
require daslib/strings_boost

[lint_macro]
class LintEverything : AstPassMacro {
    //! Pass macro that enables lint checking for modules.
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        paranoid(prog, true)
        return true
    }
}

def private is_block_terminator(expr : ExpressionPtr) : bool {
    // A direct-child block statement that always exits the enclosing function.
    if (expr is ExprReturn) return true
    if (expr is ExprCall) {
        let call = expr as ExprCall
        if (call.func == null) return false
        return call.name |> eq <| "panic" && call.func._module.name |> eq <| "builtin"
    }
    return false
}

class LintVisitor : AstVisitor {
    //! AST visitor that performs lint checks.
    //!
    //! Rule codes (for ``// nolint:LINTxxx`` suppression):
    //!   - LINT001: unreachable code
    //!   - LINT002: unused variable
    //!   - LINT003: variable can be made const (var→let)
    //!   - LINT004: underscore-prefixed variable is actually used
    //!   - LINT005: redundant reinterpret<T>(x) where T matches source type
    //!   - LINT006: division by zero (constant 0 divisor)
    //!   - LINT007: left and right operands of binary operator are identical
    //!   - LINT008: both branches of ternary ``?:`` are equivalent
    //!   - LINT009: ``then`` branch equals ``else`` branch of ``if``
    astVisitorAdapter : VisitorAdapter?
    exprForTerminator : array<uint64>
    compile_time_errors : bool
    noLint : bool = false
    genericDepth : int = 0
    // collection mode — when true, errors are appended to `errors` instead of error()
    collect_errors : bool = false
    errors : array<string>
    def LintVisitor() {
        pass
    }
    def is_suppressed(text : string; at : LineInfo) : bool {
        return is_lint_suppressed(at, extract_lint_code(text))
    }
    def lint_error(text : string; at : LineInfo) : void {
        if (noLint || self->is_suppressed(text, at)) return
        if (compile_time_errors) {
            compiling_program() |> macro_error(at, text)
        } elif (collect_errors) {
            errors |> push("{text} at {describe(at)}")
        } else {
            error("{text} at {describe(at)}\n")
        }
    }
    def override preVisitFunction(fun : FunctionPtr) : void {
        noLint = false
        if (fun.fromGeneric != null || fun.flags.generated) {
            genericDepth++
        }
        if (fun.moreFlags.isTemplate) {
            noLint = true
            return
        }
        for (ann in fun.annotations) {
            if (ann.annotation.name == "no_lint") {
                noLint = true
                break
            }
        }
    }
    def override visitFunction(var fun : FunctionPtr) : FunctionPtr {
        noLint = false
        if (fun.fromGeneric != null || fun.flags.generated) {
            genericDepth--
        }
        return <- fun
    }
    def override preVisitExprBlock(blk : ExprBlock?) : void {
        exprForTerminator |> push(0ul)
    }
    def override visitExprBlock(var blk : ExprBlock?) : ExpressionPtr {
        if (!empty(exprForTerminator)) {
            exprForTerminator |> pop()
        }
        return <- blk
    }
    def override visitExprBlockExpression(blk : ExprBlock?; var expr : ExpressionPtr) : ExpressionPtr {
        // Flag if a prior direct-child of THIS block already terminated control flow.
        let lb = exprForTerminator |> back()
        if (lb != 0ul) {
            self->lint_error("LINT001: unreachable code", expr.at)
        }
        // Mark this block as terminated only when `expr` IS a direct-child terminator.
        // Tracking from preVisitExprReturn/preVisitExprCall(panic) was incorrect: it
        // fired for returns nested in braceless `if/elif <terminator>` whose then-body
        // is an ExprReturn directly under ExprIf (no synthetic block), poisoning the
        // ENCLOSING block's slot.
        if (is_block_terminator(expr)) {
            exprForTerminator |> pop()
            exprForTerminator |> push(intptr(expr))
        }
        return <- expr
    }
    def override preVisitExprLabel(expr : ExprLabel?) : void {
        if (!empty(exprForTerminator)) {
            exprForTerminator |> pop()
        }
        exprForTerminator |> push(0ul)
    }
    // --- LINT005: redundant reinterpret cast ---

    def override preVisitExprCast(expr : ExprCast?) : void {
        if (noLint || genericDepth > 0
            || !expr.castFlags.reinterpretCast
            || expr.subexpr == null || expr.subexpr._type == null || expr.castType == null
            || expr.castType.isVoidPointer != expr.subexpr._type.isVoidPointer) return
        if (is_same_type(expr.castType, expr.subexpr._type, false, true, true, false)) {
            self->lint_error("LINT005: redundant reinterpret cast; target type is the same as source type", expr.at)
        }
    }

    def override preVisitExprForVariable(expr : ExprFor?; v : VariablePtr; last : bool) : void {
        if (expr.canShadow) return
        validate_var(v, false)
    }

    def validate_var(v : VariablePtr; can_make_const : bool) {
        if (v.flags.generated) return
        let name = string(v.name)
        // skip system vars (`__name`) and compiler-generated names (backtick-mangled, e.g. tuple destructuring)
        if (name |> starts_with("__") || find(name, "`") >= 0) return
        if (name |> starts_with("_")) {
            if (name |> ends_with("_")) return  // _foo_ - ignore too
            if (v.access_flags.access_ref || v.access_flags.access_pass || v.access_flags.access_get || v.access_flags.access_fold) {
                self->lint_error("LINT004: variable '{v.name}' is used and should be named without underscore prefix", v.at)
                return
            }
            return
        }
        if (v.isAccessUnused) {
            self->lint_error("LINT002: unused variable {v.name}: {describe(v._type)} (add an underscore prefix if you really need it)", v.at)
            return
        }
        if (!v.access_flags.access_ref && !v.access_flags.access_pass && v.init != null && v.init |> is_expr_const() && !v.access_flags.access_fold) {
            self->lint_error("LINT002: unused variable {v.name}: {describe(v._type)} (add an underscore prefix if you really need it) {v.access_flags}", v.at)
            return
        }
        if (!v.access_flags.access_ref && !v.access_flags.access_pass && !v.access_flags.access_get && (v.init == null || v.init.flags.noSideEffects) && !v.access_flags.access_fold) {
            self->lint_error("LINT002: unused variable {v.name}: {describe(v._type)} (add an underscore prefix if you really need it)", v.at)
            return
        }
        if (!v.access_flags.access_get && !v.access_flags.access_ref && (v.init == null)) {
            let sideEffects = (v.init != null && !v.init.flags.noSideEffects)
            if (!sideEffects) {
                self->lint_error("LINT002: variable {v.name}: {describe(v._type)} is never used", v.at)
            } else {
                self->lint_error("LINT002: variable {v.name}: {describe(v._type)} is never used (be careful, initializer has side effects)", v.at)
            }
            return
        }
        if (can_make_const && v.flags.inScope) return // inscope variables can't be const — delete/finalize at scope exit requires mutability
        if (can_make_const && v._type.baseType != Type.tPointer && !v._type.flags.constant && !v.access_flags.access_ref && v._type.canCloneFromConst) {// && !v._type.flags.smartPtr
            self->lint_error("LINT003: variable {v.name}: {describe(v._type)} can be made const (declare with 'let')", v.at)
            return
        }
        if (can_make_const && v._type.baseType == Type.tPointer && !v._type.flags.constant && !(v.access_flags.access_ref || v.access_flags.access_pass) && v._type.canCloneFromConst) {
            self->lint_error("LINT003: variable {v.name}: {describe(v._type)} can be made const (declare with 'let')", v.at)
            return
        }
    }

    def override preVisitExprLet(expr : ExprLet?) : void {
        for (v in expr.variables) {
            validate_var(v, true)
        }
    }

    // --- LINT006: division by zero, LINT007: same operands ---

    def is_const_zero(expr : ExpressionPtr) : bool {
        if (expr == null) return false
        if (expr is ExprConstInt) return (expr as ExprConstInt).value == 0
        if (expr is ExprConstUInt) return (expr as ExprConstUInt).value == 0u
        if (expr is ExprConstInt64) return (expr as ExprConstInt64).value == 0l
        if (expr is ExprConstUInt64) return (expr as ExprConstUInt64).value == 0ul
        if (expr is ExprConstFloat) return (expr as ExprConstFloat).value == 0.0f
        if (expr is ExprConstDouble) return (expr as ExprConstDouble).value == 0.0lf
        return false
    }

    def expr_equal(a : ExpressionPtr; b : ExpressionPtr; require_pure : bool = true) : bool {
        if (a == null || b == null
            || (require_pure && (!a.flags.noSideEffects || !b.flags.noSideEffects))) return false
        // daslang AST nodes are not shared — each Expression has exactly one parent, so
        // pointer-equality between sibling operands (left vs right) is effectively never
        // true. Structural compare via describe() is the only correct option here.
        return describe(a) == describe(b)
    }

    def override preVisitExprOp2(expr : ExprOp2?) : void {
        if (noLint || genericDepth > 0) return
        let op = string(expr.op)
        if ((op == "/" || op == "%" || op == "/=" || op == "%=") && is_const_zero(expr.right)) {
            self->lint_error("LINT006: division by zero ({op})", expr.at)
            return
        }
        let op_same_suspicious = (
            op == "==" || op == "!=" ||
            op == "<"  || op == ">"  ||
            op == "<=" || op == ">=" ||
            op == "-"  || op == "/"  || op == "%" ||
            op == "&&" || op == "||" ||
            op == "&"  || op == "|"  || op == "^" ||
            op == "-=" || op == "/=" || op == "%="
        )
        if (op_same_suspicious && expr_equal(expr.left, expr.right, false)) {
            self->lint_error("LINT007: left and right operands of '{op}' are the same", expr.at)
        }
    }

    // --- LINT008: ternary both branches equal ---

    def override preVisitExprOp3(expr : ExprOp3?) : void {
        if (noLint || genericDepth > 0) return
        if (expr_equal(expr.left, expr.right, false)) {
            self->lint_error("LINT008: both branches of ternary '?:' are equivalent", expr.at)
        }
    }

    // --- LINT009: if-then-else branches equal ---

    def override preVisitExprIfThenElse(expr : ExprIfThenElse?) : void {
        if (noLint || genericDepth > 0) return
        if (expr.if_false != null && expr_equal(expr.if_true, expr.if_false, false)) {
            self->lint_error("LINT009: 'then' branch is equivalent to 'else' branch", expr.at)
        }
    }
}

def public paranoid(prog : ProgramPtr; compile_time_errors : bool) {
    //! Runs the paranoid lint visitor on the program to check for common coding issues.
    var astVisitor = new LintVisitor(compile_time_errors = compile_time_errors)
    make_visitor(*astVisitor) $(adapter) {
        astVisitor.astVisitorAdapter = adapter
        visit(prog, astVisitor.astVisitorAdapter)
    }
    unsafe {
        delete astVisitor
    }
}

def public paranoid_collect(prog : ProgramPtr; var errors : array<string>) : int {
    //! Runs the paranoid lint visitor and collects errors as strings.
    //! Returns the number of lint issues found.
    var astVisitor = new LintVisitor(compile_time_errors = false, collect_errors = true)
    make_visitor(*astVisitor) $(adapter) {
        astVisitor.astVisitorAdapter = adapter
        visit(prog, astVisitor.astVisitorAdapter)
    }
    let count = length(astVisitor.errors)
    errors |> reserve(count)
    for (e in astVisitor.errors) {
        errors |> push(e)
    }
    unsafe {
        delete astVisitor
    }
    return count
}
