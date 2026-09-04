options gen2

module smt_macro shared

require smt/daslib/smt_expr public

require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost

//! `constrain` — natural logical syntax for assertions:
//!
//!     s |> constrain(x > 2 && (x < 10 || y % 2 == 0) && !flag)
//!
//! Rewrites `&&`/`||`/`!` (not overloadable in das) into `s_and`/`s_or`/`s_not`
//! before type inference; everything else resolves through the smt_expr
//! overloads, which also lift runtime ints/bools into numerals.

// quantifier call shapes whose trailing-lambda bodies are symbolic by
// contract — the rewriter descends into THESE calls only; any other call may
// legitimately take concrete das bools, so its arguments stay untouched
let private QUANT_NAMES <- {
    "all_of", "any_of", "all_pairs", "smt_forall", "smt_exists"
}

class private BoolOpRewriter : AstVisitor {
    def override canVisitCall(expr : ExprCall?) : bool {
        return key_exists(QUANT_NAMES, "{expr.name}")
    }
    def override visitExprOp2(var expr : ExprOp2?) : ExpressionPtr {
        if (expr.op == "&&") {
            return qmacro(s_and($e(expr.left), $e(expr.right)))
        } elif (expr.op == "||") {
            return qmacro(s_or($e(expr.left), $e(expr.right)))
        }
        return expr
    }
    def override visitExprOp1(var expr : ExprOp1?) : ExpressionPtr {
        if (expr.op == "!") {
            return qmacro(s_not($e(expr.subexpr)))
        }
        return expr
    }
}

def private rewrite_bool_ops(e : ExpressionPtr) : ExpressionPtr {
    var root = clone_expression(e)
    var v = new BoolOpRewriter()
    make_visitor(*v) $(adapter) {
        root = visit(root, adapter)
    }
    unsafe {
        delete v
    }
    return root
}

def private sort_fn_for(td : TypeDeclPtr) : string {
    if (td == null) {
        return ""
    }
    if (td.baseType == Type.tInt) {
        return "int_const"
    }
    if (td.baseType == Type.tFloat || td.baseType == Type.tDouble) {
        return "real_const"
    }
    if (td.baseType == Type.tBool) {
        return "bool_const"
    }
    if (td.baseType == Type.tString) {
        return "string_const"
    }
    if (td.baseType == Type.tUInt8) {
        return "bv8_const"
    }
    if (td.baseType == Type.tUInt16) {
        return "bv16_const"
    }
    if (td.baseType == Type.tUInt) {
        return "bv32_const"
    }
    if (td.baseType == Type.tUInt64) {
        return "bv64_const"
    }
    return ""
}

//! Shared statement lowering for the block forms. Each `var x : T` becomes a
//! fresh unknown named "x"; each bare expression statement becomes a constraint
//! handed to `emit` (an `add(...)` or an accumulator push).
def private lower_block_stmts(prog : ProgramPtr; blk : ExprBlock?; ctxName : string;
                              onSolver : bool; accName : string; var stmts : array<ExpressionPtr>) : bool {
    for (stmt in blk.list) {
        if (stmt is ExprLet) {
            let lt = stmt as ExprLet
            for (v in lt.variables) {
                let fname = sort_fn_for(v._type)
                if (empty(fname)) {
                    macro_error(prog, v.at,
                        "constrain block: unsupported unknown type for `{v.name}` - use int, uint, uint64, float, double, bool or string")
                    return false
                }
                let vname = string(v.name)
                stmts |> push <| qmacro_expr() {
                    let $i(v.name) = $c(fname)($i(ctxName), $v(vname))
                }
                if (v.init != null) {
                    if (onSolver) {
                        stmts |> push <| qmacro_expr() {
                            add($i(ctxName), $i(v.name) == $e(rewrite_bool_ops(v.init)))
                        }
                    } else {
                        stmts |> push <| qmacro_expr() {
                            $i(accName) |> push($i(v.name) == $e(rewrite_bool_ops(v.init)))
                        }
                    }
                }
            }
        } elif (stmt is ExprIfThenElse || stmt is ExprFor || stmt is ExprWhile || stmt is ExprReturn) {
            macro_error(prog, stmt.at,
                "constrain block: only unknown declarations and constraint expressions are supported, got {describe(stmt)}")
            return false
        } else {
            if (onSolver) {
                stmts |> push <| qmacro_expr() {
                    add($i(ctxName), $e(rewrite_bool_ops(stmt)))
                }
            } else {
                stmts |> push <| qmacro_expr() {
                    $i(accName) |> push($e(rewrite_bool_ops(stmt)))
                }
            }
        }
    }
    return true
}

[call_macro(name="constrain")]
class private ConstrainMacro : AstCallMacro {
    def override canVisitArgument(call : ExprCallMacro?; argIndex : int) : bool {
        // the constraint expression stays raw: `&&` over SBool cannot type before the rewrite
        return argIndex == 0
    }
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        macro_verify(call.arguments |> length == 2, prog, call.at,
            "expecting solver |> constrain(expression) or solver |> constrain() <| block")
        if (call.arguments[1] is ExprMakeBlock) {
            // block form: declarations become unknowns, statements become assertions
            let imb = call.arguments[1] as ExprMakeBlock
            let blk = imb._block as ExprBlock
            var stmts : array<ExpressionPtr>
            if (!lower_block_stmts(prog, blk, "smtSlv_", true, "", stmts)) {
                return null
            }
            return qmacro_block() {
                let smtSlv_ = $e(call.arguments[0])
                $b(stmts)
            }
        }
        return qmacro(add($e(call.arguments[0]), $e(rewrite_bool_ops(call.arguments[1]))))
    }
}

//! `let formula = s |> smt_formula() <| $() { ... }` — builds the conjunction
//! of the block's constraints on the solver's context without asserting it;
//! reusable across solvers sharing that context.
[call_macro(name="smt_formula")]
class private SmtFormulaMacro : AstCallMacro {
    def override canVisitArgument(call : ExprCallMacro?; argIndex : int) : bool {
        return argIndex == 0
    }
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        macro_verify(call.arguments |> length == 2 && call.arguments[1] is ExprMakeBlock, prog, call.at,
            "expecting solver |> smt_formula() with a block argument")
        let imb = call.arguments[1] as ExprMakeBlock
        let blk = imb._block as ExprBlock
        var stmts : array<ExpressionPtr>
        if (!lower_block_stmts(prog, blk, "smtCtx_", false, "smtAcc_", stmts)) {
            return null
        }
        return qmacro(invoke() $() : SBool {
            let smtCtx_ = $e(call.arguments[0]).ctx
            var smtAcc_ : array<SBool>
            $b(stmts)
            return s_all(smtAcc_)
        })
    }
}

// ===== [smt_fn] — lift an ordinary das function into a formula builder =====

var private g_smtfn_done : table<string>

def private lift_type_name(td : TypeDeclPtr) : string {
    if (td == null) {
        return ""
    }
    if (td.baseType == Type.tInt) {
        return "SInt"
    }
    if (td.baseType == Type.tBool) {
        return "SBool"
    }
    return ""
}

//! Statement-lowering visitor for [smt_fn]: `if` becomes guard updates, assignments merge via
//! `ite`, early returns accumulate first-return-wins, bounded `for` unrolls. Descent is explicit,
//! not AstVisitor, because the done-flag threads sequential state between sibling statements.
class private SmtFnEmitter {
    stmts : array<ExpressionPtr>
    counter : int
    err : string

    def emit_block(blk : ExprBlock?; g : string) : bool {
        for (stmt in blk.list) {
            if (!emit_stmt(stmt, g)) {
                return false
            }
        }
        return true
    }

    def emit_branch(e : ExpressionPtr; g : string) : bool {
        if (e is ExprBlock) {
            return emit_block(e as ExprBlock, g)
        }
        return emit_stmt(e, g)
    }

    def emit_stmt(stmt : ExpressionPtr; g : string) : bool {
        if (stmt is ExprLet) {
            return emit_let(stmt as ExprLet)
        } elif (stmt is ExprCopy) {
            return emit_copy(stmt as ExprCopy, g)
        } elif (stmt is ExprIfThenElse) {
            return emit_if(stmt as ExprIfThenElse, g)
        } elif (stmt is ExprReturn) {
            return emit_return(stmt as ExprReturn, g)
        } elif (stmt is ExprFor) {
            return emit_for(stmt as ExprFor, g)
        }
        err = "[smt_fn]: unsupported statement `{describe(stmt)}` - v1 supports initialized locals, assignments, if/else, return, and literal-range for loops"
        return false
    }

    def emit_let(lt : ExprLet?) : bool {
        for (v in lt.variables) {
            if (v.init == null) {
                err = "[smt_fn]: local `{v.name}` must be initialized"
                return false
            }
            stmts |> push <| qmacro_expr() {
                var $i(v.name) = s_lift(smtCtx_, $e(rewrite_bool_ops(v.init)))
            }
        }
        return true
    }

    def emit_copy(cp : ExprCopy?; g : string) : bool {
        if (!(cp.left is ExprVar)) {
            err = "[smt_fn]: only assignments to locals are supported"
            return false
        }
        let vname = "{(cp.left as ExprVar).name}"
        stmts |> push <| qmacro_expr() {
            $i(vname) = ite($i(g), s_lift(smtCtx_, $e(rewrite_bool_ops(cp.right))), $i(vname))
        }
        return true
    }

    def emit_if(ift : ExprIfThenElse?; g : string) : bool {
        counter ++
        let cname = "smtC{counter}_"
        let gtName = "smtT{counter}_"
        let gfName = "smtF{counter}_"
        stmts |> push <| qmacro_expr() { // nolint:STYLE012 — appending to the accumulated emission
            let $i(cname) = s_lift(smtCtx_, $e(rewrite_bool_ops(ift.cond)))
        }
        stmts |> push <| qmacro_expr() {
            let $i(gtName) = s_and($i(g), $i(cname))
        }
        if (!emit_branch(ift.if_true, gtName)) {
            return false
        }
        if (ift.if_false != null) {
            stmts |> push <| qmacro_expr() {
                let $i(gfName) = s_and($i(g), s_not($i(cname)))
            }
            if (!emit_branch(ift.if_false, gfName)) {
                return false
            }
        }
        return true
    }

    def emit_return(ret : ExprReturn?; g : string) : bool {
        if (ret.subexpr == null) {
            err = "[smt_fn]: `return` must carry a value"
            return false
        }
        stmts |> push <| qmacro_expr() { // nolint:STYLE012 — appending to the accumulated emission
            smtRet_ = ite(s_and($i(g), s_not(smtDone_)), s_lift(smtCtx_, $e(rewrite_bool_ops(ret.subexpr))), smtRet_)
        }
        stmts |> push <| qmacro_expr() {
            smtDone_ = s_or(smtDone_, $i(g))
        }
        return true
    }

    def emit_for(efor : ExprFor?; g : string) : bool {
        if (length(efor.sources) != 1 || length(efor.iterators) != 1) {
            err = "[smt_fn]: only single-source for loops are supported"
            return false
        }
        var lo = 0
        var hi = 0
        if (efor.sources[0] is ExprConstRange) {
            let rng = (efor.sources[0] as ExprConstRange).value
            lo = rng.x
            hi = rng.y
        } elif (efor.sources[0] is ExprCall) {
            let rcall = efor.sources[0] as ExprCall
            var boundsOk = rcall.name == "range"
            if (boundsOk) {
                for (a in rcall.arguments) {
                    boundsOk &&= a is ExprConstInt
                }
            }
            if (!boundsOk || empty(rcall.arguments) || length(rcall.arguments) > 2) {
                err = "[smt_fn]: for loop must iterate `range(<int literal>[, <int literal>])` so it can be unrolled"
                return false
            }
            if (length(rcall.arguments) == 1) {
                hi = (rcall.arguments[0] as ExprConstInt).value
            } else {
                lo = (rcall.arguments[0] as ExprConstInt).value
                hi = (rcall.arguments[1] as ExprConstInt).value
            }
        } else {
            err = "[smt_fn]: for loop must iterate a literal range so it can be unrolled"
            return false
        }
        let van = "{efor.iterators[0]}"
        for (k in range(lo, hi)) {
            var iterBody = clone_expression(efor.body)
            remove_deref(van, iterBody)
            var subst : Template
            subst |> replaceVariable(van) <| new ExprConstInt(at = efor.at, value = k, isConstLiteral = true)
            subst |> apply_template(efor.at, iterBody, false)
            var inner = new SmtFnEmitter(counter = counter)
            let ok = inner->emit_block(iterBody as ExprBlock, g)
            counter = inner.counter
            if (!ok) {
                err = inner.err
                unsafe {
                    delete inner
                }
                return false
            }
            stmts |> push <| qmacro_block() { // nolint:PERF006 — trip count is macro-time data, not worth a reserve
                $b(inner.stmts)
            }
            unsafe {
                delete inner
            }
        }
        return true
    }
}

//! `[smt_fn]` — generates an overload of the function taking `SInt`/`SBool`
//! parameters and returning the symbolic result, so `foo(sym_x, sym_y)` builds
//! a formula while `foo(1, 2)` keeps running concrete.
[function_macro(name="smt_fn")]
class private SmtFnMacro : AstFunctionAnnotation {
    def override apply(var func : FunctionPtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        let key = "{compiling_module().name}::{func.name}"
        if (key_exists(g_smtfn_done, key)) {
            return true
        }
        g_smtfn_done |> insert(key)
        if (empty(func.arguments)) {
            errors := "[smt_fn] function must have at least one parameter"
            return false
        }
        for (p in func.arguments) {
            if (empty(lift_type_name(p._type))) {
                errors := "[smt_fn] parameter `{p.name}` must be int or bool"
                return false
            }
        }
        let retLift = lift_type_name(func.result)
        if (empty(retLift)) {
            errors := "[smt_fn] function must declare an int or bool return type"
            return false
        }
        var twin <- clone_function(func)
        twin.flags.generated = true
        while (length(twin.annotations) != 0) {
            twin.annotations |> erase(0)
        }
        for (p in twin.arguments) {
            var td = clone_type(lift_type_name(p._type) == "SInt" ? qmacro_type(type<SInt>) : qmacro_type(type<SBool>))
            td.flags.constant = true
            p._type = td
        }
        twin.result = clone_type(retLift == "SInt" ? qmacro_type(type<SInt>) : qmacro_type(type<SBool>))
        let p0 = string(func.arguments[0].name)
        var stmts : array<ExpressionPtr>
        stmts |> push_from <| qmacro_block_to_array() {
            let smtCtx_ = $i(p0).ctx
            let smtG0_ = bool_val(smtCtx_, true)
            var smtDone_ = bool_val(smtCtx_, false)
        }
        if (retLift == "SInt") {
            stmts |> push <| qmacro_expr() {
                var smtRet_ = s_lift(smtCtx_, 0)
            }
        } else {
            stmts |> push <| qmacro_expr() {
                var smtRet_ = s_lift(smtCtx_, false)
            }
        }
        var em = new SmtFnEmitter()
        em.stmts <- stmts
        if (!em->emit_block(func.body as ExprBlock, "smtG0_")) {
            errors := em.err
            unsafe {
                delete em
            }
            return false
        }
        em.stmts |> push <| qmacro_expr() {
            return smtRet_
        }
        twin.body = qmacro_block() {
            $b(em.stmts)
        }
        unsafe {
            delete em
        }
        compiling_module() |> add_function(twin)
        return true
    }
}
