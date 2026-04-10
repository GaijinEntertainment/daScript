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

class LintVisitor : AstVisitor {
    //! AST visitor that performs lint checks.
    //!
    //! Rule codes (for ``// nolint:LINTxxx`` suppression):
    //!   - LINT001: unreachable code
    //!   - LINT002: unused variable
    //!   - LINT003: variable can be made const (var→let)
    //!   - LINT004: underscore-prefixed variable is actually used
    astVisitorAdapter : smart_ptr<ast::VisitorAdapter>
    exprForTerminator : array<uint64>
    compile_time_errors : bool
    noLint : bool = false
    // collection mode — when true, errors are appended to `errors` instead of error()
    collect_errors : bool = false
    errors : array<string>
    def LintVisitor() {
        pass
    }
    def is_suppressed(text : string; at : LineInfo) : bool {
        if (at.fileInfo == null || at.line == 0u) {
            return false
        }
        let colon = find(text, ":")
        if (colon < 0) {
            return false
        }
        let code = slice(text, 0, colon)
        var found = false
        get_file_source_line(at.fileInfo, at.line) $(line) {
            let nolint_pos = find(line, "nolint")
            if (nolint_pos >= 0) {
                found = find(line, code, nolint_pos) >= 0
            }
        }
        return found
    }
    def lint_error(text : string; at : LineInfo) : void {
        if (noLint) {
            return
        }
        if (self->is_suppressed(text, at)) {
            return
        }
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
        return <- fun
    }
    def override preVisitExprBlock(blk : ExprBlock?) : void {
        exprForTerminator |> push(0ul)
    }
    def override visitExprBlock(var blk : ExprBlock?) : ExpressionPtr {
        if (length(exprForTerminator) > 0) {
            exprForTerminator |> pop()
        }
        return <- blk
    }
    def override visitExprBlockExpression(blk : ExprBlock?; var expr : ExpressionPtr) : ExpressionPtr {
        let lb = exprForTerminator |> back()
        if (lb != 0ul) {
            let eb = intptr(expr)
            if (lb != eb) {
                self->lint_error("LINT001: unreachable code", expr.at)
            }
        }
        return <- expr
    }
    def override preVisitExprLabel(expr : ExprLabel?) : void {
        if (length(exprForTerminator) > 0) {
            exprForTerminator |> pop()
        }
        exprForTerminator |> push(0ul)
    }
    def override preVisitExprReturn(expr : ExprReturn?) : void {
        if (length(exprForTerminator) > 0) {
            exprForTerminator |> pop()
        }
        exprForTerminator |> push(intptr(expr))
    }
    def override preVisitExprCall(expr : ExprCall?) : void {
        if (expr.func == null) {
            return
        }
        if (expr.name |> eq <| "panic" && expr.func._module.name |> eq <| "builtin") {
            if (length(exprForTerminator) > 0) {
                exprForTerminator |> pop()
            }
            exprForTerminator |> push(intptr(expr))
        }
    }
    def override preVisitExprForVariable(expr : ExprFor?; v : VariablePtr; last : bool) : void {
        if (expr.canShadow) {
            return
        }
        validate_var(v, false)
    }

    def validate_var(v : VariablePtr; can_make_const : bool) {
        if (v.flags.generated) {
            return
        }
        let name = string(v.name)
        if (name |> starts_with("__")) {// system variables
            return
        }
        if (find(name, "`") >= 0) {// compiler-generated internal name (e.g. tuple destructuring)
            return
        }
        if (name |> starts_with("_")) {
            if (name |> ends_with("_")) {// _foo_ - ignore too
                return
            }
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
        if (can_make_const && v.flags.inScope) {
            return // inscope variables can't be const — delete/finalize at scope exit requires mutability
        }
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
}

def public paranoid(prog : ProgramPtr; compile_time_errors : bool) {
    //! Runs the paranoid lint visitor on the program to check for common coding issues.
    var astVisitor = new LintVisitor(compile_time_errors = compile_time_errors)
    unsafe {
        astVisitor.astVisitorAdapter <- make_visitor(*astVisitor)
    }
    visit(prog, astVisitor.astVisitorAdapter)
    astVisitor.astVisitorAdapter := null
    unsafe {
        delete astVisitor
    }
}

def public paranoid_collect(prog : ProgramPtr; var errors : array<string>) : int {
    //! Runs the paranoid lint visitor and collects errors as strings.
    //! Returns the number of lint issues found.
    var astVisitor = new LintVisitor(compile_time_errors = false, collect_errors = true)
    unsafe {
        astVisitor.astVisitorAdapter <- make_visitor(*astVisitor)
    }
    visit(prog, astVisitor.astVisitorAdapter)
    let count = length(astVisitor.errors)
    errors |> reserve(count)
    for (e in astVisitor.errors) {
        errors |> push(e)
    }
    astVisitor.astVisitorAdapter := null
    unsafe {
        delete astVisitor
    }
    return count
}
