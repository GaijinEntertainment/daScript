options gen2
options indenting = 4
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
require daslib/lint_config

[lint_macro]
class LintEverything : AstPassMacro {
    //! Pass macro that enables lint checking for modules.
    def override apply(prog : ProgramPtr; _mod : Module?) : bool {
        let disabled <- build_lint_macro_disabled(prog)
        paranoid(prog, true, disabled)
        return true
    }
}

struct private DeadStoreEntry {
    @do_not_delete v : Variable const?
    @do_not_delete pending_expr : Expression const?
    skip : bool
    has_pending : bool
    pending_pure : bool
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
    astVisitorAdapter : VisitorAdapter?
    exprForTerminator : array<uint64>
    compile_time_errors : bool
    noLint : bool = false
    genericDepth : int = 0
    // collection mode — when true, errors are appended to `errors` instead of error()
    collect_errors : bool = false
    errors : array<string>
    issues : array<LintIssue>       // structured twin of `errors`, same entries in the same order
    // Filters: disabled (denylist) and enabled (whitelist; empty == all).
    // Caller-populated via collect overload; applied alongside // nolint suppression.
    disabled_codes : table<string>
    enabled_codes : table<string>
    // LINT010 state — dead-store detection. Per-local tracking; one entry per
    // analyzed local in the current function. Reset on function entry.
    branch_depth : int = 0
    ds_entries : array<DeadStoreEntry>
    funcFile : string       // current function's source file, anchors lint_error's foreign-lineinfo gate
    def LintVisitor() {
        pass
    }
    def is_suppressed(text : string; at : LineInfo) : bool {
        let code = extract_lint_code(text)
        if (!empty(code)) {
            return true if (key_exists(disabled_codes, code)
                            || (!empty(enabled_codes) && !key_exists(enabled_codes, code)))
        }
        return is_lint_suppressed(at, code)
    }
    def lint_error(text : string; at : LineInfo) : void {
        // a node carrying another file's lineinfo (an inliner splice from a required module, macro-manufactured code) gets its diagnostics from that file's own compile — reporting it here would fail modules on idioms they cannot edit
        if (noLint || is_suppressed(text, at)
                || (at.fileInfo != null && !empty(funcFile) && at.fileInfo.name != funcFile)) return
        if (compile_time_errors) {
            compiling_program() |> macro_error(at, text)
        } elif (collect_errors) {
            errors |> push("{text} at {describe(at)}")
            issues |> push(make_lint_issue(text, at))
        } else {
            error("{text} at {describe(at)}\n")
        }
    }
    def override preVisitFunction(fun : FunctionPtr) : void {
        noLint = false
        funcFile = fun.at.fileInfo != null ? "{fun.at.fileInfo.name}" : ""
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
        // LINT010 — reset dead-store tracking
        branch_depth = 0
        ds_entries |> clear()
    }
    def override visitFunction(var fun : FunctionPtr) : FunctionPtr {
        // LINT010 — emit final dead-store warnings for any still-pending stores
        if (!noLint && genericDepth == 0) {
            for (e in ds_entries) {
                if (!e.skip && e.has_pending && e.pending_pure && e.pending_expr != null) {
                    let vname = "{e.v.name}"
                    lint_error("LINT010: dead store of '{vname}': value written but never read before scope exit", e.pending_expr.at)
                }
            }
        }
        noLint = false
        if (fun.fromGeneric != null || fun.flags.generated) {
            genericDepth--
        }
        return <- fun
    }
    def override preVisitExprBlock(blk : ExprBlock?) : void {
        exprForTerminator |> push(0ul)
        // LINT010: a block passed as a closure / lambda body / generator body executes deferred or conditionally — treat it as a branch so writes inside don't kill pending stores outside and reads inside don't keep an outside-init "live" (the callback may never fire).
        let bf = blk.blockFlags
        if (!noLint && genericDepth == 0 && (bf.isClosure || bf.isLambdaBlock || bf.isGeneratorBlock)) {
            branch_depth++
        }
    }
    def override visitExprBlock(var blk : ExprBlock?) : ExpressionPtr {
        if (!empty(exprForTerminator)) {
            exprForTerminator |> pop()
        }
        let bf = blk.blockFlags
        if (!noLint && genericDepth == 0 && (bf.isClosure || bf.isLambdaBlock || bf.isGeneratorBlock)) {
            branch_depth--
        }
        return <- blk
    }
    def override visitExprBlockExpression(_blk : ExprBlock?; var expr : ExpressionPtr) : ExpressionPtr {
        // Flag if a prior direct-child of THIS block already terminated control flow.
        let lb = exprForTerminator |> back()
        if (lb != 0ul) {
            lint_error("LINT001: unreachable code", expr.at)
        }
        // LINT015: a free-floating unary +/- subexpression used as a statement — a continuation line that lost its wrapping parens (`let x = a` ⏎ `+ b`, which re-parses as a unary `+b` whose value is discarded; if the operand is pure the optimizer drops the whole statement silently). Only `+`/`-` qualify: they are the only operators that are both binary and unary, so a split binary `a + b` orphans as a valid unary statement (`~`/`!` aren't binary, `*`/`|>`/etc. can't start a statement → loud error).
        if (!noLint && genericDepth == 0 && expr is ExprOp1 && !expr.genFlags.generated) {
            let op1 = expr as ExprOp1
            if (op1.op == "+" || op1.op == "-") {
                lint_error("LINT015: free-floating unary '{op1.op}' expression as a statement — its value is silently discarded; this is almost always a continuation line missing its parens, wrap the multi-line RHS in (...)", expr.at)
            }
        }
        // Mark this block as terminated only when `expr` IS a direct-child terminator.
        if (is_block_terminator(expr)) {
            exprForTerminator |> pop()
            exprForTerminator |> push(intptr(expr))
        }
        return <- expr
    }
    def override preVisitExprLabel(_expr : ExprLabel?) : void {
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
            lint_error("LINT005: redundant reinterpret cast; target type is the same as source type", expr.at)
        }
    }

    def override preVisitExprForVariable(expr : ExprFor?; v : VariablePtr; _last : bool) : void {
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
                lint_error("LINT004: variable '{v.name}' is used and should be named without underscore prefix", v.at)
                return
            }
            return
        }
        if (v.isAccessUnused) {
            lint_error("LINT002: unused variable {v.name}: {describe(v._type)} (add an underscore prefix if you really need it)", v.at)
            return
        }
        if (!v.access_flags.access_ref && !v.access_flags.access_pass && v.init != null && v.init |> is_expr_const() && !v.access_flags.access_fold) {
            lint_error("LINT002: unused variable {v.name}: {describe(v._type)} (add an underscore prefix if you really need it) {v.access_flags}", v.at)
            return
        }
        if (!v.access_flags.access_ref && !v.access_flags.access_pass && !v.access_flags.access_get && (v.init == null || v.init.flags.noSideEffects) && !v.access_flags.access_fold) {
            lint_error("LINT002: unused variable {v.name}: {describe(v._type)} (add an underscore prefix if you really need it)", v.at)
            return
        }
        if (!v.access_flags.access_get && !v.access_flags.access_ref && (v.init == null)) {
            let sideEffects = (v.init != null && !v.init.flags.noSideEffects)
            if (!sideEffects) {
                lint_error("LINT002: variable {v.name}: {describe(v._type)} is never used", v.at)
            } else {
                lint_error("LINT002: variable {v.name}: {describe(v._type)} is never used (be careful, initializer has side effects)", v.at)
            }
            return
        }
        // inScope variables can't be const — delete/finalize at scope exit requires mutability.
        if (can_make_const && (v.flags.inScope || v.access_info.access_info_pass_mutable)) return
        if (can_make_const && v._type.baseType != Type.tPointer && !v._type.flags.constant && !v.access_flags.access_ref && v._type.canCloneFromConst) {// && !v._type.flags.smartPtr
            lint_error("LINT003: variable {v.name}: {describe(v._type)} can be made const (declare with 'let')", v.at)
            return
        }
        if (can_make_const && v._type.baseType == Type.tPointer && !v._type.flags.constant && !(v.access_flags.access_ref || v.access_flags.access_pass) && v._type.canCloneFromConst) {
            lint_error("LINT003: variable {v.name}: {describe(v._type)} can be made const (declare with 'let')", v.at)
            return
        }
    }

    def validate_argument(v : VariablePtr; code, kind : string) {
        // skip template/generic/[no_lint] context, generated vars, and [unused_argument(name)] (marked_used)
        if (noLint || genericDepth > 0 || v.flags.generated || v.flags.marked_used) return
        let name = string(v.name)
        // A leading underscore on an argument means "leave it alone": intentionally unused, or dodging a reserved keyword / shadow (e.g. `_in`). Unlike locals, we do NOT raise LINT004 for a used `_arg` — a parameter name is often constrained and can't drop the underscore.
        if (name |> starts_with("_") || find(name, "`") >= 0) return
        if (v.isAccessUnused) {
            lint_error("{code}: unused {kind} {v.name}: {describe(v._type)} (add an underscore prefix or [unused_argument({v.name})] if intentional)", v.at)
        }
    }

    def validate_unwritten_var_argument(fun : FunctionPtr; v : VariablePtr) {
        if (noLint || genericDepth > 0 || v.flags.generated || v.flags.marked_used) return
        let name = string(v.name)
        // skips, in order: leave-it-alone names; `finalize` overloads (the delete protocol dictates var) and address-taken functions (signature conforms to a function-pointer type); by-value parameters (a mutable copy is not a caller burden); written params (access_ref) and params whose mutability a callee's signature demands transitively (access_info_pass_mutable — the leaf callee gets flagged first; fixing it re-exposes this one); entirely-unused params (LINT012's report)
        if ((name |> starts_with("_")) || find(name, "`") >= 0
                || fun.name == "finalize" || fun.moreFlags.addressTaken
                || v._type == null || v._type.flags.constant
                || !(v._type.flags.ref || v._type.isRefType)
                || v.access_flags.access_ref || v.access_info.access_info_pass_mutable
                || v.isAccessUnused) {
            return
        }
        lint_error("LINT014: mutable argument {v.name}: {describe(v._type)} is never written; declare it without 'var'", v.at)
    }

    def override preVisitFunctionArgument(fun : FunctionPtr; arg : VariablePtr; _lastArg : bool) : void {
        // class-method signatures are dictated by the base class / interface, and [extern] stubs by the bound C function — unused params there are structural, not a mistake
        if (fun.flags.isClassMethod) return
        for (ann in fun.annotations) {
            if (ann.annotation.name == "extern") return
        }
        validate_argument(arg, "LINT012", "function argument")
        validate_unwritten_var_argument(fun, arg)
    }

    def override preVisitExprBlockArgument(_blk : ExprBlock?; arg : VariablePtr; _lastArg : bool) : void {
        validate_argument(arg, "LINT013", "block argument")
    }

    // `assume name = <block>` keeps a template block at the alias-definition site; every use of
    // `name` is replaced with a *clone* of that block.
    // The template is never invoked directly, so its block arguments keep isAccessUnused == true and
    def override canVisitWithAliasSubexpression(_expr : ExprAssume?) : bool {
        return false
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
        // daslang AST nodes are not shared — each Expression has exactly one parent, so pointer-equality between sibling operands (left vs right) is effectively never true. Structural compare via describe() is the only correct option here.
        return describe(a) == describe(b)
    }

    def override preVisitExprOp2(expr : ExprOp2?) : void {
        if (noLint || genericDepth > 0) return
        let op = string(expr.op)
        if ((op == "/" || op == "%" || op == "/=" || op == "%=") && is_const_zero(expr.right)) {
            lint_error("LINT006: division by zero ({op})", expr.at)
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
        // identical constants are macro-splice residue (`$v(n) > 1` with n == 1), const-fold food rather than a copy-paste typo
        if (op_same_suspicious && expr_equal(expr.left, expr.right, false)
                && !(expr.left.__rtti |> starts_with("ExprConst"))) {
            lint_error("LINT007: left and right operands of '{op}' are the same", expr.at)
        }
    }

    // --- LINT008: ternary both branches equal ---

    def override preVisitExprOp3(expr : ExprOp3?) : void {
        if (noLint || genericDepth > 0) return
        if (expr_equal(expr.left, expr.right, false)) {
            lint_error("LINT008: both branches of ternary '?:' are equivalent", expr.at)
        }
    }

    // --- LINT009: if-then-else branches equal ---

    def override preVisitExprIfThenElse(expr : ExprIfThenElse?) : void {
        if (noLint || genericDepth > 0) return
        if (expr.if_false != null && expr_equal(expr.if_true, expr.if_false, false)) {
            lint_error("LINT009: 'then' branch is equivalent to 'else' branch", expr.at)
        }
        branch_depth++
    }
    def override visitExprIfThenElse(var expr : ExprIfThenElse?) : ExpressionPtr {
        if (!noLint && genericDepth == 0) {
            branch_depth--
        }
        return <- expr
    }

    // --- LINT010: dead store ---

    def private ds_find(v : VariablePtr) : int {
        // Linear scan — typical function has <10 tracked locals.
        let vc : Variable const? = v
        for (i in range(length(ds_entries))) {
            if (ds_entries[i].v == vc) return i
        }
        return -1
    }

    def override preVisitExprLetVariable(_let : ExprLet?; v : VariablePtr; _last : bool) : void {
        // Skip reference bindings (`var r & = x`) and inscope smart_ptr vars — these have aliasing or scope-exit semantics we don't reason about.
        if (noLint || genericDepth > 0
            || (v._type != null && v._type.flags.ref)
            || v.flags.inScope
            || v.access_flags.access_fold
            || !v.access_flags.access_get) return
        let has_init = v.init != null
        let init_pure = has_init && v.init.flags.noSideEffects
        ds_entries |> emplace(DeadStoreEntry(
            v = v,
            pending_expr = v.init,
            skip = false,
            has_pending = has_init,
            pending_pure = init_pure
        ))
    }

    def override preVisitExprVar(expr : ExprVar?) : void {
        if (noLint || genericDepth > 0) return
        let i = ds_find(expr.variable)
        if (i < 0 || ds_entries[i].skip) return
        let vf = expr.varFlags
        // Any reference inside a branch/loop/lambda body, OR a write under an aliasing context (addr-of, ref-bind, mutable-ref-arg, compound op), means we can't reason about this var — bail.
        if (branch_depth > 0 || (vf.write && !vf.under_clone)) {
            ds_entries[i].skip = true
            return
        }
        // Pure store — handled in postVisit for ExprCopy/Clone/Move so the RHS reads have been observed first. preVisit of the LHS fires BEFORE the RHS is walked, so we do nothing here.
        if (vf.write) return
        // Read — kills any pending dead store on this var.
        ds_entries[i].has_pending = false
    }

    def private ds_handle_store(lhs : ExpressionPtr; rhs : ExpressionPtr; var store_expr : ExpressionPtr) : void {
        if (lhs == null || !(lhs is ExprVar)) return
        let ev = lhs as ExprVar
        let i = ds_find(ev.variable)
        if (i < 0 || ds_entries[i].skip || branch_depth > 0) return
        if (ds_entries[i].has_pending && ds_entries[i].pending_pure && ds_entries[i].pending_expr != null) {
            let vname = "{ev.variable.name}"
            lint_error("LINT010: dead store of '{vname}': overwritten without intervening read", ds_entries[i].pending_expr.at)
        }
        ds_entries[i].has_pending = true
        ds_entries[i].pending_expr = store_expr
        ds_entries[i].pending_pure = rhs != null && rhs.flags.noSideEffects
    }

    def override visitExprCopy(var expr : ExprCopy?) : ExpressionPtr {
        ds_handle_store(expr.left, expr.right, expr)
        return <- expr
    }
    def override visitExprClone(var expr : ExprClone?) : ExpressionPtr {
        ds_handle_store(expr.left, expr.right, expr)
        return <- expr
    }
    def override visitExprMove(var expr : ExprMove?) : ExpressionPtr {
        ds_handle_store(expr.left, expr.right, expr)
        return <- expr
    }

    // Branch / loop / try / lambda — bumps branch_depth so any var referenced
    // inside is bailed-on.
    def override preVisitExprFor(_expr : ExprFor?) : void {
        if (!noLint && genericDepth == 0) branch_depth++
    }
    def override visitExprFor(var expr : ExprFor?) : ExpressionPtr {
        if (!noLint && genericDepth == 0) branch_depth--
        return <- expr
    }
    def override preVisitExprWhile(_expr : ExprWhile?) : void {
        if (!noLint && genericDepth == 0) branch_depth++
    }
    def override visitExprWhile(var expr : ExprWhile?) : ExpressionPtr {
        if (!noLint && genericDepth == 0) branch_depth--
        return <- expr
    }
    def override preVisitExprTryCatch(_expr : ExprTryCatch?) : void {
        if (!noLint && genericDepth == 0) branch_depth++
    }
    def override visitExprTryCatch(var expr : ExprTryCatch?) : ExpressionPtr {
        if (!noLint && genericDepth == 0) branch_depth--
        return <- expr
    }
    // NOTE on lambda capture: by-copy/clone captures (the daslang default) read

    // --- LINT011: int-literal-to-float precision loss ---
    def override visitExprConstFloat(var expr : ExprConstFloat?) : ExpressionPtr {
        if (!noLint && genericDepth == 0 && expr.inexactFloatPromotion) {
            lint_error("LINT011: int literal does not exactly represent as float", expr.at)
        }
        return <- expr
    }
    def override visitExprConstDouble(var expr : ExprConstDouble?) : ExpressionPtr {
        if (!noLint && genericDepth == 0 && expr.inexactFloatPromotion) {
            lint_error("LINT011: int literal does not exactly represent as double", expr.at)
        }
        return <- expr
    }
}

def public paranoid(prog : ProgramPtr; compile_time_errors : bool) {
    //! Runs the paranoid lint visitor on the program to check for common coding issues.
    let empty_set : table<string>
    paranoid(prog, compile_time_errors, empty_set)
}

def public paranoid(prog : ProgramPtr; compile_time_errors : bool; disabled_codes : table<string>) {
    //! Filter-aware compile-time variant. ``disabled_codes`` suppresses
    //! the listed rule IDs (e.g. "LINT003") alongside the usual
    //! ``// nolint:CODE`` directives.
    var astVisitor = new LintVisitor(compile_time_errors = compile_time_errors)
    astVisitor.disabled_codes := disabled_codes
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
    let empty_set : table<string>
    return paranoid_collect(prog, errors, empty_set, empty_set)
}

def public paranoid_collect(prog : ProgramPtr; var errors : array<string>;
                            disabled_codes, enabled_codes : table<string>) : int {
    //! Filter-aware overload. `disabled_codes` is a denylist; `enabled_codes`
    //! is a whitelist (empty == all). Codes use the bare form (e.g. "LINT002").
    var astVisitor = new LintVisitor(compile_time_errors = false, collect_errors = true)
    astVisitor.disabled_codes := disabled_codes
    astVisitor.enabled_codes := enabled_codes
    make_visitor(*astVisitor) $(adapter) {
        astVisitor.astVisitorAdapter = adapter
        visit(prog, astVisitor.astVisitorAdapter)
    }
    let count = length(astVisitor.errors)
    errors |> push_from(astVisitor.errors)
    unsafe {
        delete astVisitor
    }
    return count
}

def public paranoid_collect_issues(prog : ProgramPtr; var issues : array<LintIssue>;
                                   disabled_codes, enabled_codes : table<string>) : int {
    //! Structured twin of ``paranoid_collect`` — same filters, but emits
    //! ``LintIssue`` records (rule code + position) instead of display strings.
    var astVisitor = new LintVisitor(compile_time_errors = false, collect_errors = true)
    astVisitor.disabled_codes := disabled_codes
    astVisitor.enabled_codes := enabled_codes
    make_visitor(*astVisitor) $(adapter) {
        astVisitor.astVisitorAdapter = adapter
        visit(prog, astVisitor.astVisitorAdapter)
    }
    let count = length(astVisitor.issues)
    issues |> push_from(astVisitor.issues)
    unsafe {
        delete astVisitor
    }
    return count
}
