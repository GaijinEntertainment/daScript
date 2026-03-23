options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers = true

module ast_match shared public

//! AST pattern matching — reverse reification.
//!
//! ``qmatch`` matches an expression against a structural pattern and extracts
//! sub-expressions, constant values, and identifier names using the same tag
//! system as ``qmacro`` but in reverse: ``$e`` clones a matched expression,
//! ``$v`` extracts a constant value, ``$i`` extracts an identifier name.

require ast public
require rtti
require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost
require strings

// ── Result types ────────────────────────────────────────────────────────

enum QMatchError : int {
    ok
    rtti_mismatch
    field_mismatch
    const_type_mismatch
    type_mismatch
    list_length
    wildcard_not_found
    null_expression
}

struct QMatchResult {
    matched : bool
    error   : QMatchError
    expr    : Expression const?
}

// ── Runtime helpers ─────────────────────────────────────────────────────

def qm_run(expr : ExpressionPtr; blk : block<(var arg : ExpressionPtr) : QMatchResult>) : QMatchResult {
    var inscope copy <- clone_expression(expr)
    return invoke(blk, copy)
}

def qm_run_function(var func : FunctionPtr&; blk : block<(var body : ExpressionPtr; var func_ptr : Function?) : QMatchResult>) : QMatchResult {
    //! Run matching block on a function, passing both body and Function pointer.
    if (func == null) {
        return qm_fail_null()
    }
    var inscope body <- clone_expression(func.body)
    return invoke(blk, body, get_ptr(func))
}

def qm_ok() : QMatchResult {
    return QMatchResult(matched = true)
}

def qm_fail_rtti(var expr : ExpressionPtr) : QMatchResult {
    return QMatchResult(error = QMatchError.rtti_mismatch, expr = get_ptr(expr))
}

def qm_fail_field(var expr : ExpressionPtr) : QMatchResult {
    return QMatchResult(error = QMatchError.field_mismatch, expr = get_ptr(expr))
}

def qm_fail_null() : QMatchResult {
    return QMatchResult(error = QMatchError.null_expression)
}

def qm_fail_list(var expr : ExpressionPtr) : QMatchResult {
    return QMatchResult(error = QMatchError.list_length, expr = get_ptr(expr))
}

def qm_fail_const(var expr : ExpressionPtr) : QMatchResult {
    return QMatchResult(error = QMatchError.const_type_mismatch, expr = get_ptr(expr))
}

def qm_fail_type(var expr : ExpressionPtr) : QMatchResult {
    return QMatchResult(error = QMatchError.type_mismatch, expr = get_ptr(expr))
}

def qm_fail_wildcard(var expr : ExpressionPtr) : QMatchResult {
    return QMatchResult(error = QMatchError.wildcard_not_found, expr = get_ptr(expr))
}

def qm_scan(var blk_expr : ExpressionPtr&; var pos : int&; len : int; matcher : block<(var elem : ExpressionPtr) : QMatchResult>) : QMatchResult {
    //! Scan block statements from pos to len, trying matcher at each position.
    //! On first success, update pos to the position after the match and return ok.
    var inscope blk_copy <- clone_expression(blk_expr)
    var blk = blk_copy as ExprBlock
    for (scan_pos in range(pos, len)) {
        {
            var inscope elem <- clone_expression(blk.list[scan_pos])
            let r = invoke(matcher, elem)
            if (r.matched) {
                pos = scan_pos + 1
                return qm_ok()
            }
        }
    }
    return qm_fail_wildcard(clone_expression(blk_expr))
}

def qm_extract(var expr : ExpressionPtr; var val : int&) : QMatchResult {
    //! Extract int from ExprConstInt. Fails if wrong type.
    if (expr is ExprConstInt) {
        val = (expr as ExprConstInt).value
        return qm_ok()
    }
    return qm_fail_const(expr)
}

def qm_extract(var expr : ExpressionPtr; var val : float&) : QMatchResult {
    if (expr is ExprConstFloat) {
        val = (expr as ExprConstFloat).value
        return qm_ok()
    }
    return qm_fail_const(expr)
}

def qm_extract(var expr : ExpressionPtr; var val : bool&) : QMatchResult {
    if (expr is ExprConstBool) {
        val = (expr as ExprConstBool).value
        return qm_ok()
    }
    return qm_fail_const(expr)
}

def qm_extract(var expr : ExpressionPtr; var val : string&) : QMatchResult {
    if (expr is ExprConstString) {
        val = string((expr as ExprConstString).value)
        return qm_ok()
    }
    return qm_fail_const(expr)
}

def qm_extract(var expr : ExpressionPtr; var val : int64&) : QMatchResult {
    if (expr is ExprConstInt64) {
        val = (expr as ExprConstInt64).value
        return qm_ok()
    }
    return qm_fail_const(expr)
}

def qm_extract(var expr : ExpressionPtr; var val : uint&) : QMatchResult {
    if (expr is ExprConstUInt) {
        val = (expr as ExprConstUInt).value
        return qm_ok()
    }
    return qm_fail_const(expr)
}

def qm_extract(var expr : ExpressionPtr; var val : double&) : QMatchResult {
    if (expr is ExprConstDouble) {
        val = (expr as ExprConstDouble).value
        return qm_ok()
    }
    return qm_fail_const(expr)
}

def qm_extract_name(var expr : ExpressionPtr) : string {
    //! Extract the "name" field from common expression types.
    //! Works for ExprVar, ExprCall, ExprField, ExprSafeField, ExprLet variables, etc.
    if (expr is ExprVar) {
        return string((expr as ExprVar).name)
    } elif (expr is ExprField || expr is ExprSafeField) {
        return string((expr as ExprField).name)
    } elif (expr is ExprCall) {
        return string((expr as ExprCall).name)
    } elif (expr is ExprIsVariant || expr is ExprAsVariant || expr is ExprSafeAsVariant) {
        return string((expr as ExprIsVariant).name)
    }
    return ""
}

def qm_count_real_args(args : dasvector`smart_ptr`Variable) : int {
    //! Count arguments excluding fakeContext and fakeLineInfo.
    var count = 0
    for (arg in args) {
        if (arg._type != null) {
            let bt = arg._type.baseType
            if (bt != Type.fakeContext && bt != Type.fakeLineInfo) {
                count ++
            }
        } else {
            count ++
        }
    }
    return count
}

def qm_real_arg_index(args : dasvector`smart_ptr`Variable; logical_idx : int) : int {
    //! Map logical argument index (skipping fakeContext/fakeLineInfo) to physical index.
    var logical = 0
    for (arg, phys in args, range(length(args))) {
        if (arg._type != null) {
            let bt = arg._type.baseType
            if (bt == Type.fakeContext || bt == Type.fakeLineInfo) {
                continue
            }
        }
        if (logical == logical_idx) {
            return phys
        }
        logical ++
    }
    return -1
}

def qm_extract_remaining_args(args : dasvector`smart_ptr`Variable; from_logical : int; var result : array<VariablePtr>&) : void {
    //! Collect remaining real arguments (skipping fakeContext/fakeLineInfo) starting from logical index.
    var logical = 0
    for (arg in args) {
        if (arg._type != null) {
            let bt = arg._type.baseType
            if (bt == Type.fakeContext || bt == Type.fakeLineInfo) {
                continue
            }
        }
        if (logical >= from_logical) {
            result |> emplace_new <| clone_variable(arg)
        }
        logical ++
    }
}

def qm_extract_stmts(var blk_expr : ExpressionPtr; from_pos : int; to_pos : int; var result : array<ExpressionPtr>&) : void {
    //! Extract statements [from_pos, to_pos) from a block into an array (cloned).
    var blk = blk_expr as ExprBlock
    for (i in range(from_pos, to_pos)) {
        result |> emplace_new <| clone_expression(blk.list[i])
    }
}

def qm_extract_call_name(var expr : ExpressionPtr) : string {
    //! Extract call name from ExprCall or similar.
    if (expr is ExprCall) {
        return string((expr as ExprCall).name)
    }
    return ""
}

def qm_extract_field_name(var expr : ExpressionPtr) : string {
    //! Extract field name from ExprField, ExprSafeField, or variant expressions.
    if (expr is ExprField || expr is ExprSafeField) {
        return string((expr as ExprField).name)
    } elif (expr is ExprIsVariant || expr is ExprAsVariant || expr is ExprSafeAsVariant) {
        return string((expr as ExprIsVariant).name)
    }
    return ""
}

def qm_extract_type(var expr : ExpressionPtr; var out_type : TypeDeclPtr&) : QMatchResult {
    //! Extract TypeDeclPtr from ExprTypeDecl.
    if (!(expr is ExprTypeDecl)) {
        return qm_fail_type(expr)
    }
    out_type |> move_new <| clone_type((expr as ExprTypeDecl).typeexpr)
    return qm_ok()
}

def qm_rtti(var expr : ExpressionPtr) : string {
    return string(get_ptr(expr).__rtti)
}

def qm_rtti(expr : Expression const?) : string {
    return string(expr.__rtti)
}

// ── Compile-time AST builders ───────────────────────────────────────────

[macro_function]
def private make_is_check(at : LineInfo; var expr : ExpressionPtr&; rtti_name : string) : ExpressionPtr {
    return <- new ExprIsVariant(at = at, value <- expr, name := rtti_name)
}

[macro_function]
def private make_not(at : LineInfo; var expr : ExpressionPtr&) : ExpressionPtr {
    return <- new ExprOp1(at = at, op := "!", subexpr <- expr)
}

[macro_function]
def private make_var_ref(at : LineInfo; name : string) : ExpressionPtr {
    return <- new ExprVar(at = at, name := name)
}

[macro_function]
def private qm_guard(at : LineInfo; var cond : ExpressionPtr&; var fail_call : ExpressionPtr&) : ExpressionPtr {
    //! Build: if (cond) { return fail_call }
    var inscope fail_ret <- new ExprReturn(at = at, subexpr <- fail_call)
    var inscope fail_blk <- new ExprBlock(at = at)
    unsafe {
        (reinterpret<ExprBlock?> fail_blk).list |> emplace(fail_ret)
    }
    return <- new ExprIfThenElse(at = at, cond <- cond, if_true <- fail_blk)
}

[macro_function]
def private qm_rtti_guard(at : LineInfo; actual_var : string; rtti_name : string) : ExpressionPtr {
    var inscope cond <- qmacro(qm_rtti($i(actual_var)) != $v(rtti_name))
    var inscope fail <- qmacro(qm_fail_rtti($i(actual_var)))
    return <- qm_guard(at, cond, fail)
}

[macro_function]
def private qm_null_guard(at : LineInfo; actual_var : string) : ExpressionPtr {
    var inscope cond <- qmacro($i(actual_var) == null)
    var inscope fail <- qmacro(qm_fail_null())
    return <- qm_guard(at, cond, fail)
}

[macro_function]
def private next_var(var index : int&) : string {
    let name = "_qm_{index}"
    index++
    return name
}

[macro_function]
def private qm_skip_field(rtti_name : string; field_name : string) : bool {
    //! Returns true if the field should be skipped during pattern matching.
    //! Compiler-internal fields are populated during compilation but null/zero
    //! in raw patterns from qmacro/quote.

    // Fields on all expressions (inherited from Expression base)
    if (field_name == "_type" || field_name == "flags" || field_name == "genFlags"
        || field_name == "printFlags" || field_name == "at") {
        return true
    }
    // Stack/annotation internals
    if (field_name == "stackTop" || field_name == "stackVarBottom" || field_name == "stackVarTop"
        || field_name == "annotationDataSid" || field_name == "annotationData"
        || field_name == "maxLabelIndex" || field_name == "refStackTop"
        || field_name == "initStackSize" || field_name == "argStackTop") {
        return true
    }
    // ExprVar — resolved variable reference
    if (rtti_name == "ExprVar") {
        if (field_name == "variable" || field_name == "argumentIndex" || field_name == "pBlock"
            || field_name == "varFlags") {
            return true
        }
    }
    // ExprCall/ExprOp*/ExprLooksLikeCall — resolved function reference
    if (field_name == "func" || field_name == "atEnclosure" || field_name == "stackTop"
        || field_name == "doesNotNeedSp" || field_name == "cmresAlias"
        || field_name == "argumentsFailedToInfer") {
        return true
    }
    // ExprReturn
    if (field_name == "returnInBlock") {
        return true
    }
    return false
}

// ── TypeDecl matching code generation ────────────────────────────────────
// Hardcoded because TypeDecl is not an Expression — no get_expression_annotation.
// Handles $t capture, dimExpr (contains Expressions), and recursive child types.

[macro_function]
def private qm_type_guard(at : LineInfo; actual_var : string; var cond : ExpressionPtr&) : ExpressionPtr {
    //! Build: if (cond) { return qm_fail_type(expr_var) }
    //! actual_var is used for error context (the original ExpressionPtr)
    var inscope fail <- qmacro(qm_fail_type($i(actual_var)))
    return <- qm_guard(at, cond, fail)
}

[macro_function]
def private generate_type_match(pat_type : TypeDecl?; td_var : string; expr_var : string; var body : array<ExpressionPtr>&; var index : int&; at : LineInfo) {
    //! Generate code to match a TypeDecl at runtime.
    //! td_var — name of a TypeDeclPtr variable holding the actual type
    //! expr_var — name of the ExpressionPtr variable for error reporting
    //! pat_type — compile-time pattern TypeDecl

    if (pat_type == null) {
        return
    }

    // ── $t tag — capture TypeDeclPtr ────────────────────────
    if (pat_type.baseType == Type.alias && pat_type.flags.isTag) {
        if (pat_type.firstType != null && length(pat_type.firstType.dimExpr) == 1) {
            let tag_expr = get_ptr(pat_type.firstType.dimExpr[0])
            if (tag_expr != null && tag_expr is ExprVar) {
                let vname = string((tag_expr as ExprVar).name)
                body |> emplace_new <| qmacro_expr(${ $i(vname) |> move_new <| clone_type($i(td_var)); })
            }
        }
        return
    }

    // ── baseType ────────────────────────────────────────────
    var bt_int = int(pat_type.baseType)
    var inscope bt_cond <- qmacro(int($i(td_var).baseType) != $v(bt_int))
    body |> emplace_new <| qm_type_guard(at, expr_var, bt_cond)

    // ── flags — match structurally relevant flags ────────────
    if (pat_type.flags.constant) {
        var inscope c <- qmacro(!$i(td_var).flags.constant)
        body |> emplace_new <| qm_type_guard(at, expr_var, c)
    }
    if (pat_type.flags.ref) {
        var inscope c <- qmacro(!$i(td_var).flags.ref)
        body |> emplace_new <| qm_type_guard(at, expr_var, c)
    }
    if (pat_type.flags.temporary) {
        var inscope c <- qmacro(!$i(td_var).flags.temporary)
        body |> emplace_new <| qm_type_guard(at, expr_var, c)
    }
    if (pat_type.flags.smartPtr) {
        var inscope c <- qmacro(!$i(td_var).flags.smartPtr)
        body |> emplace_new <| qm_type_guard(at, expr_var, c)
    }
    if (pat_type.flags.isExplicit) {
        var inscope c <- qmacro(!$i(td_var).flags.isExplicit)
        body |> emplace_new <| qm_type_guard(at, expr_var, c)
    }

    // ── structType — compare by name ────────────────────────
    if (pat_type.structType != null) {
        let sname = string(pat_type.structType.name)
        var inscope c <- qmacro($i(td_var).structType == null || string($i(td_var).structType.name) != $v(sname))
        body |> emplace_new <| qm_type_guard(at, expr_var, c)
    }

    // ── enumType — compare by name ──────────────────────────
    if (pat_type.enumType != null) {
        let ename = string(pat_type.enumType.name)
        var inscope c <- qmacro($i(td_var).enumType == null || string($i(td_var).enumType.name) != $v(ename))
        body |> emplace_new <| qm_type_guard(at, expr_var, c)
    }

    // ── annotation — compare by name (handled types) ────────
    if (pat_type.annotation != null) {
        let aname = string(pat_type.annotation.name)
        var inscope c <- qmacro($i(td_var).annotation == null || string($i(td_var).annotation.name) != $v(aname))
        body |> emplace_new <| qm_type_guard(at, expr_var, c)
    }

    // ── alias — compare string ──────────────────────────────
    if (!empty(pat_type.alias)) {
        let aval = string(pat_type.alias)
        var inscope c <- qmacro(string($i(td_var).alias) != $v(aval))
        body |> emplace_new <| qm_type_guard(at, expr_var, c)
    }

    // ── firstType — recursive ───────────────────────────────
    if (pat_type.firstType != null) {
        let child_var = next_var(index)
        var inscope nc <- qmacro($i(td_var).firstType == null)
        body |> emplace_new <| qm_type_guard(at, expr_var, nc)
        body |> emplace_new <| qmacro_expr(${ var inscope $i(child_var) <- clone_type($i(td_var).firstType); })
        generate_type_match(get_ptr(pat_type.firstType), child_var, expr_var, body, index, at)
    }

    // ── secondType — recursive ──────────────────────────────
    if (pat_type.secondType != null) {
        let child_var = next_var(index)
        var inscope nc <- qmacro($i(td_var).secondType == null)
        body |> emplace_new <| qm_type_guard(at, expr_var, nc)
        body |> emplace_new <| qmacro_expr(${ var inscope $i(child_var) <- clone_type($i(td_var).secondType); })
        generate_type_match(get_ptr(pat_type.secondType), child_var, expr_var, body, index, at)
    }

    // ── argTypes — element-wise recursive ────────────────────
    let n_args = length(pat_type.argTypes)
    if (n_args > 0) {
        var inscope lc <- qmacro(length($i(td_var).argTypes) != $v(n_args))
        body |> emplace_new <| qm_type_guard(at, expr_var, lc)
        for (arg, i in pat_type.argTypes, range(n_args)) {
            let pat_arg = get_ptr(arg)
            if (pat_arg != null) {
                let child_var = next_var(index)
                body |> emplace_new <| qmacro_expr(${ var inscope $i(child_var) <- clone_type($i(td_var).argTypes[$v(i)]); })
                generate_type_match(pat_arg, child_var, expr_var, body, index, at)
            }
        }
    }

    // ── dim — compare fixed dimensions ──────────────────────
    let n_dim = length(pat_type.dim)
    if (n_dim > 0) {
        var inscope dc <- qmacro(length($i(td_var).dim) != $v(n_dim))
        body |> emplace_new <| qm_type_guard(at, expr_var, dc)
        for (d, i in pat_type.dim, range(n_dim)) {
            {
                var inscope c <- qmacro($i(td_var).dim[$v(i)] != $v(d))
                body |> emplace_new <| qm_type_guard(at, expr_var, c)
            }
        }
    }

    // ── dimExpr — these are Expressions, use generate_match ──
    let n_dimexpr = length(pat_type.dimExpr)
    if (n_dimexpr > 0) {
        var inscope dc <- qmacro(length($i(td_var).dimExpr) != $v(n_dimexpr))
        body |> emplace_new <| qm_type_guard(at, expr_var, dc)
        for (dexpr, i in pat_type.dimExpr, range(n_dimexpr)) {
            let pat_expr = get_ptr(dexpr)
            if (pat_expr != null) {
                let child_var = next_var(index)
                body |> emplace_new <| qmacro_expr(${ var inscope $i(child_var) <- clone_expression($i(td_var).dimExpr[$v(i)]); })
                generate_match(pat_expr, child_var, body, index, at)
            }
        }
    }
}

// ── Argument and return type matching ────────────────────────────────────

[macro_function]
def private generate_pattern_args_match(pat_block : ExprBlock?; target_var : string; ret_field : string; expr_var : string; var body : array<ExpressionPtr>&; var index : int&; at : LineInfo) {
    //! Generate code to match argument names, types, and return type.
    //! target_var — runtime variable with .arguments and return type field
    //! ret_field — "returnType" for ExprBlock, "result" for Function
    //! If pattern has no arguments, skip argument checks.
    //! If pattern has no return type (null), skip return type check.

    let n_args = length(pat_block.arguments)
    if (n_args > 0) {
        // Detect $a(var) tag on the last argument — captures remaining args
        var has_a_tag = false
        var a_tag_vname = ""
        {
            let last_arg = get_ptr(pat_block.arguments[n_args - 1])
            if (last_arg._type != null && last_arg._type.flags.isTag && last_arg._type.baseType == Type.none) {
                has_a_tag = true
                if (last_arg.init != null && last_arg.init is ExprVar) {
                    a_tag_vname = string((last_arg.init as ExprVar).name)
                }
            }
        }
        let n_fixed = has_a_tag ? n_args - 1 : n_args

        // Check argument count (excluding fakeContext/fakeLineInfo)
        // With $a: at least n_fixed args. Without: exactly n_args.
        if (has_a_tag) {
            var inscope cond <- qmacro(qm_count_real_args($i(target_var).arguments) < $v(n_fixed))
            var inscope fail <- qmacro(qm_fail_list($i(expr_var)))
            body |> emplace_new <| qm_guard(at, cond, fail)
        } else {
            var inscope cond <- qmacro(qm_count_real_args($i(target_var).arguments) != $v(n_args))
            var inscope fail <- qmacro(qm_fail_list($i(expr_var)))
            body |> emplace_new <| qm_guard(at, cond, fail)
        }

        // Match each fixed argument's name and type (using real arg index mapping)
        for (arg, i in pat_block.arguments, range(n_fixed)) {
            {
                var pat_arg = get_ptr(arg)
                let arg_name = string(pat_arg.name)

                // Get physical index at runtime (skipping fake args)
                let phys_var = next_var(index)
                body |> emplace_new <| qmacro_expr(${ var $i(phys_var) = qm_real_arg_index($i(target_var).arguments, $v(i)); })

                // Name matching
                if (!empty(arg_name)) {
                    var inscope nc <- qmacro(string($i(target_var).arguments[$i(phys_var)].name) != $v(arg_name))
                    var inscope nf <- qmacro(qm_fail_field($i(expr_var)))
                    body |> emplace_new <| qm_guard(at, nc, nf)
                }

                // Type matching — if argument has a type, match it
                if (pat_arg._type != null) {
                    var pat_td = unsafe(reinterpret<TypeDecl?> get_ptr(pat_arg._type))
                    // Skip if type is autoinfer (unspecified)
                    if (pat_td.baseType != Type.autoinfer) {
                        let td_var = next_var(index)
                        body |> emplace_new <| qmacro_expr(${ var inscope $i(td_var) <- clone_type($i(target_var).arguments[$i(phys_var)]._type); })
                        generate_type_match(pat_td, td_var, expr_var, body, index, at)
                    }
                }
            }
        }

        // $a(var) — capture remaining arguments
        if (has_a_tag && !empty(a_tag_vname)) {
            body |> emplace_new <| qmacro_expr(${ qm_extract_remaining_args($i(target_var).arguments, $v(n_fixed), $i(a_tag_vname)); })
        }
    }

    // Return type matching
    if (pat_block.returnType != null) {
        var pat_ret = unsafe(reinterpret<TypeDecl?> get_ptr(pat_block.returnType))
        // Skip if return type is autoinfer or void (unspecified)
        if (pat_ret.baseType != Type.autoinfer && pat_ret.baseType != Type.tVoid) {
            let ret_var = next_var(index)
            body |> emplace_new <| qmacro_expr(${ var inscope $i(ret_var) <- clone_type($i(target_var).$f(ret_field)); })
            generate_type_match(pat_ret, ret_var, expr_var, body, index, at)
        }
    }
}

[macro_function]
def private is_wildcard_call(pattern : Expression?) : bool {
    if (!(pattern is ExprCall)) {
        return false
    }
    let call = pattern as ExprCall
    let cname = string(call.name)
    return cname == "_wildcard" || cname == "_wildcard1" || cname == "_optional" || cname == "_any"
}

[macro_function]
def private get_wildcard_b_tag(pattern : Expression?) : string {
    //! If wildcard has a $b(var) argument, return the variable name. Otherwise "".
    if (!(pattern is ExprCall)) {
        return ""
    }
    let call = pattern as ExprCall
    if (length(call.arguments) == 0) {
        return ""
    }
    let arg = get_ptr(call.arguments[0])
    if (arg is ExprTag) {
        let tag = arg as ExprTag
        if (string(tag.name) == "b") {
            let var_expr = tag.subexpr as ExprVar
            if (var_expr != null) {
                return string(var_expr.name)
            }
        }
    }
    return ""
}

// ── Core code generator ─────────────────────────────────────────────────
// Generated code uses var (not let) for all variables so as-cast returns
// non-const and field access gives non-const smart_ptr<Expression>.

[macro_function]
def private generate_match(pattern : Expression?; actual_var : string; var body : array<ExpressionPtr>&; var index : int&; at : LineInfo) {
    if (pattern == null) {
        return
    }

    let rtti = string(pattern.__rtti)

    // ── ExprTag — captures ($e, $v, $i, $t, $c, $f) ────────
    if (pattern is ExprTag) {
        let tag = pattern as ExprTag
        let tag_name = string(tag.name)
        if (tag_name == "e" || tag_name == "") {
            let var_expr = tag.subexpr as ExprVar
            if (var_expr != null) {
                let vname = string(var_expr.name)
                body |> emplace_new <| qmacro_expr(${ $i(vname) |> move_new <| clone_expression($i(actual_var)); })
            }
            return
        } elif (tag_name == "v") {
            // $v(var) — extract constant value. Overload resolved by variable type.
            // Generates: var _qm_r = qm_extract(actual, vname); if (!_qm_r.matched) return _qm_r
            let var_expr = tag.subexpr as ExprVar
            if (var_expr != null) {
                let vname = string(var_expr.name)
                let r_var = next_var(index)
                body |> emplace_new <| qmacro_expr(${ var $i(r_var) = qm_extract($i(actual_var), $i(vname)); })
                var inscope cond <- qmacro($i(r_var).matched == false)
                var inscope fail <- make_var_ref(at, r_var)
                body |> emplace_new <| qm_guard(at, cond, fail)
            }
            return
        } elif (tag_name == "i") {
            let var_expr = tag.subexpr as ExprVar
            if (var_expr != null) {
                let vname = string(var_expr.name)
                // Check if $i wraps a declaration (ExprLet, ExprFor, ExprAddr)
                if (tag.value != null) {
                    let wrapped = get_ptr(tag.value)
                    if (wrapped is ExprLet) {
                        // var $i(name) = expr — match ExprLet, capture variable name
                        // Clone the wrapped expression so we don't modify the pattern AST
                        var inscope wrapped_clone <- clone_expression(tag.value)
                        var wrapped_clone_let = unsafe(reinterpret<ExprLet?> get_ptr(wrapped_clone))
                        if (length(wrapped_clone_let.variables) > 0) {
                            get_ptr(wrapped_clone_let.variables[0]).name := ""
                        }
                        // Generate ExprLet matching for the cloned expression
                        generate_match(get_ptr(wrapped_clone), actual_var, body, index, at)
                        // After matching, extract variable name
                        let cast_let = next_var(index)
                        var inscope asv <- make_var_ref(at, actual_var)
                        var inscope asc <- new ExprAsVariant(at = at, value <- asv, name := "ExprLet")
                        body |> emplace_new <| qmacro_expr(${ var $i(cast_let) = $e(asc); })
                        body |> emplace_new <| qmacro_expr(${ $i(vname) = string($i(cast_let).variables[0].name); })
                    } else {
                        // Other wrapped types — just match and extract name
                        generate_match(wrapped, actual_var, body, index, at)
                        body |> emplace_new <| qmacro_expr(${ $i(vname) = qm_extract_name($i(actual_var)); })
                    }
                } else {
                    // Standalone $i(name) — extract name from any expression
                    body |> emplace_new <| qmacro_expr(${ $i(vname) = qm_extract_name($i(actual_var)); })
                }
            }
            return
        } elif (tag_name == "t") {
            // $t(var) — extract TypeDeclPtr from ExprTypeDecl
            let var_expr = tag.subexpr as ExprVar
            if (var_expr != null) {
                let vname = string(var_expr.name)
                let r_var = next_var(index)
                body |> emplace_new <| qmacro_expr(${ var $i(r_var) = qm_extract_type($i(actual_var), $i(vname)); })
                var inscope cond <- qmacro($i(r_var).matched == false)
                var inscope fail <- make_var_ref(at, r_var)
                body |> emplace_new <| qm_guard(at, cond, fail)
            }
            return
        } elif (tag_name == "c") {
            // $c(var)(args) — match call arguments, capture call name
            let var_expr = tag.subexpr as ExprVar
            if (var_expr != null) {
                let vname = string(var_expr.name)
                if (tag.value != null) {
                    // Clone wrapped ExprCall, clear placeholder name so field match skips it
                    var inscope wrapped_clone <- clone_expression(tag.value)
                    var wrapped_call = unsafe(reinterpret<ExprCall?> get_ptr(wrapped_clone))
                    wrapped_call.name := ""
                    generate_match(get_ptr(wrapped_clone), actual_var, body, index, at)
                }
                // Extract call name
                body |> emplace_new <| qmacro_expr(${ $i(vname) = qm_extract_call_name($i(actual_var)); })
            }
            return
        } elif (tag_name == "f") {
            // a.$f(var) — match field value, capture field name
            let var_expr = tag.subexpr as ExprVar
            if (var_expr != null) {
                let vname = string(var_expr.name)
                if (tag.value != null) {
                    // Clone wrapped ExprField, clear placeholder name so field match skips it
                    var inscope wrapped_clone <- clone_expression(tag.value)
                    if (wrapped_clone is ExprField) {
                        var wf = unsafe(reinterpret<ExprField?> get_ptr(wrapped_clone))
                        wf.name := ""
                    } elif (wrapped_clone is ExprSafeField) {
                        var wf = unsafe(reinterpret<ExprSafeField?> get_ptr(wrapped_clone))
                        wf.name := ""
                    }
                    generate_match(get_ptr(wrapped_clone), actual_var, body, index, at)
                }
                // Extract field name
                body |> emplace_new <| qmacro_expr(${ $i(vname) = qm_extract_field_name($i(actual_var)); })
            }
            return
        }
        return
    }

    // ── _any() wildcard ────────────────────────────────────────
    if (is_wildcard_call(pattern)) {
        return
    }

    // ── ExprTypeDecl — type matching with generate_type_match ──
    if (pattern is ExprTypeDecl) {
        body |> emplace_new <| qm_rtti_guard(at, actual_var, "ExprTypeDecl")
        let cast_var = next_var(index)
        var inscope as_val <- make_var_ref(at, actual_var)
        var inscope as_cast <- new ExprAsVariant(at = at, value <- as_val, name := "ExprTypeDecl")
        body |> emplace_new <| qmacro_expr(${ var $i(cast_var) = $e(as_cast); })
        // Clone the typeexpr for matching
        let td_var = next_var(index)
        body |> emplace_new <| qmacro_expr(${ var inscope $i(td_var) <- clone_type($i(cast_var).typeexpr); })
        generate_type_match(get_ptr(unsafe(reinterpret<ExprTypeDecl?> pattern).typeexpr), td_var, actual_var, body, index, at)
        return
    }

    // ── ExprFor — handle iteratorsTags for $i capture ─────────
    if (pattern is ExprFor) {
        var pat_for = unsafe(reinterpret<ExprFor?> pattern)
        // Process iteratorsTags: clear tagged iterator names (so generic matching skips them)
        // and record which indices need $i capture
        var has_tags = false
        for (tag in pat_for.iteratorsTags) {
            if (tag != null) {
                has_tags = true
                break
            }
        }
        body |> emplace_new <| qm_rtti_guard(at, actual_var, rtti)
        let cast_var = next_var(index)
        var inscope as_val_f <- make_var_ref(at, actual_var)
        var inscope as_cast_f <- new ExprAsVariant(at = at, value <- as_val_f, name := rtti)
        body |> emplace_new <| qmacro_expr(${ var $i(cast_var) = $e(as_cast_f); })
        // Run annotation reflection for fields
        var ann = unsafe(reinterpret<BasicStructureAnnotation?> get_expression_annotation(pattern))
        if (ann != null) {
            for_each_field(*ann) $(name, cppName, xtype, offset) {
                if (offset == 0xFFFFFFFFu) {
                    return
                }
                if (qm_skip_field(rtti, string(name))) {
                    return
                }
                if (string(name) == "iteratorsTags") {
                    return
                }
                if (string(name) == "iteratorsAt" || string(name) == "iteratorVariables" || string(name) == "iteratorsTupleExpansion" || string(name) == "iteratorsAka") {
                    return
                }
                if (string(name) == "visibility" || string(name) == "allowIteratorOptimization" || string(name) == "canShadow") {
                    return
                }
                if (length(xtype.dim) != 0) {
                    return
                }
                var p8 : int8?
                unsafe {
                    p8 = (reinterpret<int8?> pattern) + int(offset)
                }
                let bt = xtype.baseType
                // Reuse the same field matching logic from the generic handler
                if (bt == Type.tHandle && string(xtype.annotation.name) == "dasvector`das_string") {
                    let pv = unsafe(reinterpret<dasvector`das_string?> p8)
                    let pat_len = length(*pv)
                    if (pat_len > 0) {
                        let fname = string(name)
                        var inscope cond_s <- qmacro(length($i(cast_var).$f(fname)) != $v(pat_len))
                        var inscope fail_s <- qmacro(qm_fail_list($i(actual_var)))
                        body |> emplace_new <| qm_guard(at, cond_s, fail_s)
                        for (selem, si in *pv, range(pat_len)) {
                            {
                                // Skip tagged iterators ($i capture) — they'll be handled later
                                let is_tagged = has_tags && si < length(pat_for.iteratorsTags) && pat_for.iteratorsTags[si] != null
                                if (!is_tagged) {
                                    let sval = string(selem)
                                    if (!empty(sval)) {
                                                    let fname2 = string(name)
                                        var inscope sc <- qmacro(string($i(cast_var).$f(fname2)[$v(si)]) != $v(sval))
                                        var inscope sf <- qmacro(qm_fail_field($i(actual_var)))
                                        body |> emplace_new <| qm_guard(at, sc, sf)
                                    }
                                }
                            }
                        }
                    }
                } elif (xtype.isSmartPointer) {
                    let inner_ann_name = xtype.firstType != null && xtype.firstType.annotation != null ? string(xtype.firstType.annotation.name) : ""
                    if (inner_ann_name == "TypeDecl") {
                        // skip TypeDecl smart_ptr fields for ExprFor
                        pass
                    } else {
                        let pv = unsafe(reinterpret<ExpressionPtr?> p8)
                        if (*pv != null) {
                            let child_pattern = get_ptr(*pv)
                            let child_var = next_var(index)
                            let fname = string(name)
                            body |> emplace_new <| qmacro_expr(${ var inscope $i(child_var) <- clone_expression($i(cast_var).$f(fname)); })
                            if (!is_wildcard_call(child_pattern) && !(child_pattern is ExprTag)) {
                                var inscope ncond <- qmacro($i(child_var) == null)
                                var inscope nfail <- qmacro(qm_fail_null())
                                body |> emplace_new <| qm_guard(at, ncond, nfail)
                            }
                            generate_match(child_pattern, child_var, body, index, at)
                        }
                    }
                } elif (bt == Type.tHandle && string(xtype.annotation.name) == "dasvector`smart_ptr`Expression") {
                    let pv = unsafe(reinterpret<dasvector`smart_ptr`Expression?> p8)
                    let pat_len = length(*pv)
                    if (pat_len > 0) {
                        let fname = string(name)
                        var inscope cond_e <- qmacro(length($i(cast_var).$f(fname)) != $v(pat_len))
                        var inscope fail_e <- qmacro(qm_fail_list($i(actual_var)))
                        body |> emplace_new <| qm_guard(at, cond_e, fail_e)
                        for (elem, i in *pv, range(pat_len)) {
                            let child_pattern = get_ptr(elem)
                            if (child_pattern != null) {
                                let child_var = next_var(index)
                                let fname2 = string(name)
                                body |> emplace_new <| qmacro_expr(${ var inscope $i(child_var) <- clone_expression($i(cast_var).$f(fname2)[$v(i)]); })
                                generate_match(child_pattern, child_var, body, index, at)
                            }
                        }
                    }
                }
            }
        }
        // Generate $i capture code for tagged iterators
        if (has_tags) {
            for (tag, ti in pat_for.iteratorsTags, range(length(pat_for.iteratorsTags))) {
                if (tag != null) {
                    let tag_expr = get_ptr(tag)
                    if (tag_expr is ExprVar) {
                        let cap_name = string((tag_expr as ExprVar).name)
                        var tidx = ti
                        body |> emplace_new <| qmacro_expr(${ $i(cap_name) = string($i(cast_var).iterators[$v(tidx)]); })
                    }
                }
            }
        }
        return
    }

    // ── ExprConst* — RTTI check + value comparison ────────────
    if (rtti |> starts_with("ExprConst")) {
        body |> emplace_new <| qm_rtti_guard(at, actual_var, rtti)
        // Generate: var cast = actual as ExprConstXxx
        let cast_var = next_var(index)
        var inscope as_val <- make_var_ref(at, actual_var)
        var inscope as_cast <- new ExprAsVariant(at = at, value <- as_val, name := rtti)
        body |> emplace_new <| qmacro_expr(${ var $i(cast_var) = $e(as_cast); })
        if (rtti == "ExprConstInt") {
            let cv = unsafe(reinterpret<ExprConstInt?> pattern)
            var val = cv.value
            var inscope cond <- qmacro($i(cast_var).value != $v(val))
            var inscope fail <- qmacro(qm_fail_field($i(actual_var)))
            body |> emplace_new <| qm_guard(at, cond, fail)
        } elif (rtti == "ExprConstFloat") {
            let cv = unsafe(reinterpret<ExprConstFloat?> pattern)
            var val = cv.value
            var inscope cond <- qmacro($i(cast_var).value != $v(val))
            var inscope fail <- qmacro(qm_fail_field($i(actual_var)))
            body |> emplace_new <| qm_guard(at, cond, fail)
        } elif (rtti == "ExprConstBool") {
            let cv = unsafe(reinterpret<ExprConstBool?> pattern)
            var val = cv.value
            var inscope cond <- qmacro($i(cast_var).value != $v(val))
            var inscope fail <- qmacro(qm_fail_field($i(actual_var)))
            body |> emplace_new <| qm_guard(at, cond, fail)
        } elif (rtti == "ExprConstString") {
            let cv = unsafe(reinterpret<ExprConstString?> pattern)
            var val = string(cv.value)
            var inscope cond <- qmacro(string($i(cast_var).value) != $v(val))
            var inscope fail <- qmacro(qm_fail_field($i(actual_var)))
            body |> emplace_new <| qm_guard(at, cond, fail)
        }
        return
    }

    // ── Generic expression — RTTI check + field-by-field ──────
    body |> emplace_new <| qm_rtti_guard(at, actual_var, rtti)

    // Generate: var cast = actual as ExprXxx
    let cast_var = next_var(index)
    var inscope as_val2 <- make_var_ref(at, actual_var)
    var inscope as_cast2 <- new ExprAsVariant(at = at, value <- as_val2, name := rtti)
    body |> emplace_new <| qmacro_expr(${ var $i(cast_var) = $e(as_cast2); })

    // Iterate fields via annotation reflection
    var ann = unsafe(reinterpret<BasicStructureAnnotation?> get_expression_annotation(pattern))
    if (ann == null) {
        return
    }

    // Field-by-field matching via annotation reflection
    for_each_field(*ann) $(name, cppName, xtype, offset) {
        if (offset == 0xFFFFFFFFu) {
            return
        }
        if (qm_skip_field(rtti, string(name))) {
            return
        }
        if (length(xtype.dim) != 0) {
            panic("qmatch: fixed-size array field '{name}' not supported")
        }
        var p8 : int8?
        unsafe {
            p8 = (reinterpret<int8?> pattern) + int(offset)
        }
        let bt = xtype.baseType

        if (xtype.isSmartPointer) {
            let inner_ann_name = xtype.firstType != null && xtype.firstType.annotation != null ? string(xtype.firstType.annotation.name) : ""
            if (inner_ann_name == "TypeDecl") {
                // smart_ptr<TypeDecl> field — use generate_type_match
                let pv = unsafe(reinterpret<TypeDeclPtr?> p8)
                if (*pv != null) {
                    let child_td = get_ptr(*pv)
                    let child_var = next_var(index)
                    let fname = string(name)
                    // Check not null
                    var inscope ncond <- qmacro($i(cast_var).$f(fname) == null)
                    var inscope nfail <- qmacro(qm_fail_null())
                    body |> emplace_new <| qm_guard(at, ncond, nfail)
                    body |> emplace_new <| qmacro_expr(${ var inscope $i(child_var) <- clone_type($i(cast_var).$f(fname)); })
                    generate_type_match(child_td, child_var, actual_var, body, index, at)
                }
                // If pattern's TypeDecl field is null, skip (don't require actual's to be null)
            } else {
                // smart_ptr<Expression> child — recurse or check null
                let pv = unsafe(reinterpret<ExpressionPtr?> p8)
                if (*pv != null) {
                    let child_pattern = get_ptr(*pv)
                    let child_var = next_var(index)
                    let fname = string(name)
                    body |> emplace_new <| qmacro_expr(${ var inscope $i(child_var) <- clone_expression($i(cast_var).$f(fname)); })
                    if (!is_wildcard_call(child_pattern) && !(child_pattern is ExprTag)) {
                        var inscope ncond <- qmacro($i(child_var) == null)
                        var inscope nfail <- qmacro(qm_fail_null())
                        body |> emplace_new <| qm_guard(at, ncond, nfail)
                    }
                    generate_match(child_pattern, child_var, body, index, at)
                } else {
                    // Pattern's child is null — require actual's child is also null
                    let fname = string(name)
                    var inscope ncond <- qmacro($i(cast_var).$f(fname) != null)
                    var inscope nfail <- qmacro(qm_fail_field($i(actual_var)))
                    body |> emplace_new <| qm_guard(at, ncond, nfail)
                }
            }
        } elif (bt == Type.tHandle && string(xtype.annotation.name) == "das_string") {
            // das_string field
            let pv = unsafe(reinterpret<das_string?> p8)
            let sval = string(*pv)
            if (!empty(sval)) {
                let fname = string(name)
                var inscope cond <- qmacro(string($i(cast_var).$f(fname)) != $v(sval))
                var inscope fail <- qmacro(qm_fail_field($i(actual_var)))
                body |> emplace_new <| qm_guard(at, cond, fail)
            }
        } elif (bt == Type.tHandle && string(xtype.annotation.name) == "dasvector`smart_ptr`Expression") {
            // Expression vector field (block.list, call.arguments)
            let pv = unsafe(reinterpret<dasvector`smart_ptr`Expression?> p8)
            let pat_len = length(*pv)
            if (pat_len > 0) {
                let fname = string(name)
                var inscope cond <- qmacro(length($i(cast_var).$f(fname)) != $v(pat_len))
                var inscope fail <- qmacro(qm_fail_list($i(actual_var)))
                body |> emplace_new <| qm_guard(at, cond, fail)
                for (elem, i in *pv, range(pat_len)) {
                    let child_pattern = get_ptr(elem)
                    if (child_pattern != null) {
                        let child_var = next_var(index)
                        var fidx = i
                        let fname2 = string(name)
                        body |> emplace_new <| qmacro_expr(${ var inscope $i(child_var) <- clone_expression($i(cast_var).$f(fname2)[$v(i)]); })
                        generate_match(child_pattern, child_var, body, index, at)
                    }
                }
            }
        } elif (bt == Type.tHandle && string(xtype.annotation.name) == "dasvector`smart_ptr`Variable") {
            // Variable vector (ExprLet.variables) — match name, type, init
            let pv = unsafe(reinterpret<dasvector`smart_ptr`Variable?> p8)
            let pat_len = length(*pv)
            if (pat_len > 0) {
                let fname = string(name)
                var inscope cond <- qmacro(length($i(cast_var).$f(fname)) != $v(pat_len))
                var inscope fail <- qmacro(qm_fail_list($i(actual_var)))
                body |> emplace_new <| qm_guard(at, cond, fail)
                for (velem, vi in *pv, range(pat_len)) {
                    {
                        var pat_v = get_ptr(velem)
                        let fname2 = string(name)
                        // Name matching
                        if (pat_v.flags.tag && pat_v.source != null) {
                            // $i capture — extract variable name
                            let src = get_ptr(pat_v.source)
                            if (src is ExprVar) {
                                let vname = string((src as ExprVar).name)
                                body |> emplace_new <| qmacro_expr(${ $i(vname) = string($i(cast_var).$f(fname2)[$v(vi)].name); })
                            }
                        } else {
                            let vn = string(pat_v.name)
                            if (!empty(vn)) {
                                var vname = vn
                                var inscope nc <- qmacro(string($i(cast_var).$f(fname2)[$v(vi)].name) != $v(vname))
                                var inscope nf <- qmacro(qm_fail_field($i(actual_var)))
                                body |> emplace_new <| qm_guard(at, nc, nf)
                            }
                        }
                        // Init expression matching
                        if (pat_v.init != null) {
                            let init_pat = get_ptr(pat_v.init)
                            let init_var = next_var(index)
                            let fname3 = string(name)
                            body |> emplace_new <| qmacro_expr(${ var inscope $i(init_var) <- clone_expression($i(cast_var).$f(fname3)[$v(vi)].init); })
                            generate_match(init_pat, init_var, body, index, at)
                        }
                        // Type matching (skip autoinfer)
                        if (pat_v._type != null) {
                            var pat_td = unsafe(reinterpret<TypeDecl?> get_ptr(pat_v._type))
                            if (pat_td.baseType != Type.autoinfer) {
                                let td_var = next_var(index)
                                let fname4 = string(name)
                                body |> emplace_new <| qmacro_expr(${ var inscope $i(td_var) <- clone_type($i(cast_var).$f(fname4)[$v(vi)]._type); })
                                generate_type_match(pat_td, td_var, actual_var, body, index, at)
                            }
                        }
                    }
                }
            }
        } elif (bt == Type.tHandle && string(xtype.annotation.name) == "dasvector`das_string") {
            // String vector (ExprFor.iterators) — match name strings
            let pv = unsafe(reinterpret<dasvector`das_string?> p8)
            let pat_len = length(*pv)
            if (pat_len > 0) {
                let fname = string(name)
                var inscope cond <- qmacro(length($i(cast_var).$f(fname)) != $v(pat_len))
                var inscope fail <- qmacro(qm_fail_list($i(actual_var)))
                body |> emplace_new <| qm_guard(at, cond, fail)
                for (selem, si in *pv, range(pat_len)) {
                    {
                        let sval = string(selem)
                        if (!empty(sval)) {
                            var sidx = si
                            let fname2 = string(name)
                            var inscope sc <- qmacro(string($i(cast_var).$f(fname2)[$v(si)]) != $v(sval))
                            var inscope sf <- qmacro(qm_fail_field($i(actual_var)))
                            body |> emplace_new <| qm_guard(at, sc, sf)
                        }
                    }
                }
            }
        } elif (bt == Type.tInt) {
            let pv = unsafe(reinterpret<int?> p8)
            if (*pv != 0) {
                var fval = *pv
                let fname = string(name)
                var inscope cond <- qmacro($i(cast_var).$f(fname) != $v(fval))
                var inscope fail <- qmacro(qm_fail_field($i(actual_var)))
                body |> emplace_new <| qm_guard(at, cond, fail)
            }
        } elif (bt == Type.tUInt) {
            let pv = unsafe(reinterpret<uint?> p8)
            if (*pv != 0u) {
                var fval = *pv
                let fname = string(name)
                var inscope cond <- qmacro($i(cast_var).$f(fname) != $v(fval))
                var inscope fail <- qmacro(qm_fail_field($i(actual_var)))
                body |> emplace_new <| qm_guard(at, cond, fail)
            }
        } elif (bt == Type.tBool) {
            let pv = unsafe(reinterpret<bool?> p8)
            if (*pv) {
                let fname = string(name)
                var inscope cond <- qmacro($i(cast_var).$f(fname) == false)
                var inscope fail <- qmacro(qm_fail_field($i(actual_var)))
                body |> emplace_new <| qm_guard(at, cond, fail)
            }
        }
        // Skip: tBitfield, tHandle(LineInfo), tString(__rtti), tPointer(non-smart)
    }
}

// ── Block matching code generation ───────────────────────────────────────

[macro_function]
def private get_wildcard_name(pattern : Expression?) : string {
    //! Get the wildcard type name, or "" if not a wildcard.
    if (!(pattern is ExprCall)) {
        return ""
    }
    let call = pattern as ExprCall
    let cname = string(call.name)
    if (cname == "_wildcard" || cname == "_wildcard1" || cname == "_optional" || cname == "_any") {
        return cname
    }
    return ""
}

[macro_function]
def private count_fixed_stmts(pat_block : ExprBlock?) : int {
    //! Count non-wildcard statements in a pattern block.
    var count = 0
    for (stmt in pat_block.list) {
        if (empty(get_wildcard_name(get_ptr(stmt)))) {
            count ++
        }
    }
    return count
}

[macro_function]
def private get_single_fixed_stmt(var pat_block : ExprBlock?) : Expression? {
    //! If pattern has exactly one non-wildcard statement (possibly surrounded by wildcards),
    //! return it. Otherwise return null.
    for (stmt in pat_block.list) {
        if (empty(get_wildcard_name(get_ptr(stmt)))) {
            return get_ptr(stmt)
        }
    }
    return null
}

[macro_function]
def private generate_block_match(var pat_block : ExprBlock?; actual_var : string; var body : array<ExpressionPtr>&; var index : int&; at : LineInfo; check_args : bool = true) {
    //! Generate code to match a block's statement list with wildcard support.
    //! If the optimizer folded a single-statement block into a bare expression,
    //! and the pattern has exactly one fixed statement (with optional wildcards),
    //! match the bare expression directly against that statement.

    // Check if pattern is a single fixed statement (possibly with wildcards).
    // If actual is not ExprBlock, try matching the single statement directly.
    // Skip this optimization if pattern has arguments or return type — those require a real block.
    let has_pat_args = length(pat_block.arguments) > 0
    let has_pat_ret = pat_block.returnType != null && pat_block.returnType.baseType != Type.autoinfer && pat_block.returnType.baseType != Type.tVoid
    let n_fixed = count_fixed_stmts(pat_block)
    if (n_fixed == 1 && !has_pat_args && !has_pat_ret) {
        var single_pat = get_single_fixed_stmt(pat_block)
        if (single_pat != null) {
            // Generate: if (!(actual is ExprBlock) && !(actual is ExprMakeBlock))
            //              match single_pat against actual directly
            //           else
            //              fall through to normal block matching
            // For simplicity: try block match first. If RTTI fails (not a block),
            // fall back to direct match. We use a runtime helper for this.
            let direct_var = next_var(index)
            body |> emplace_new <| qmacro_expr(${ var $i(direct_var) = qm_rtti($i(actual_var)) != "ExprBlock" && qm_rtti($i(actual_var)) != "ExprMakeBlock"; })
            // If it's not a block, match directly
            var inscope direct_body : array<ExpressionPtr>
            generate_match(single_pat, actual_var, direct_body, index, at)
            direct_body |> emplace_new <| qmacro_expr(${return qm_ok(); })
            // Build: if (direct_var) { direct match + return }
            var inscope direct_blk <- new ExprBlock(at = at)
            unsafe {
                var dp = reinterpret<ExprBlock?> direct_blk
                for (stmt in direct_body) {
                    dp.list |> emplace(stmt)
                }
            }
            var inscope direct_cond <- make_var_ref(at, direct_var)
            var inscope direct_if <- new ExprIfThenElse(at = at, cond <- direct_cond, if_true <- direct_blk)
            body |> emplace(direct_if)
            // Fall through to block matching if it IS a block
        }
    }

    // Navigate to ExprBlock — actual may be ExprMakeBlock (wraps ExprBlock) or ExprBlock directly.
    let blk_var = next_var(index)
    body |> emplace_new <| qmacro_expr(${ var inscope $i(blk_var) <- clone_expression($i(actual_var)); })
    // Unwrap ExprMakeBlock if present
    var inscope vref_blk <- make_var_ref(at, blk_var)
    var inscope is_mkblk <- make_is_check(at, vref_blk, "ExprMakeBlock")
    var inscope unwrap <- qmacro_expr(${ $i(blk_var) |> move_new <| clone_expression(($i(blk_var) as ExprMakeBlock)._block); })
    var inscope unwrap_blk <- new ExprBlock(at = at)
    unsafe {
        (reinterpret<ExprBlock?> unwrap_blk).list |> emplace(unwrap)
    }
    var inscope unwrap_if <- new ExprIfThenElse(at = at, cond <- is_mkblk, if_true <- unwrap_blk)
    body |> emplace(unwrap_if)
    // Now verify it's ExprBlock
    body |> emplace_new <| qm_rtti_guard(at, blk_var, "ExprBlock")
    let cast_var = next_var(index)
    var inscope as_val <- make_var_ref(at, blk_var)
    var inscope as_cast <- new ExprAsVariant(at = at, value <- as_val, name := "ExprBlock")
    body |> emplace_new <| qmacro_expr(${ var $i(cast_var) = $e(as_cast); })

    // Match block arguments and return type (if requested and pattern specifies them)
    if (check_args) {
        generate_pattern_args_match(pat_block, cast_var, "returnType", actual_var, body, index, at)
    }

    // Declare position cursor: var _qm_pos = 0
    let pos_var = next_var(index)
    body |> emplace_new <| qmacro_expr(${ var $i(pos_var) = 0; })

    // Get the total length of the actual block for bounds checking
    let len_var = next_var(index)
    body |> emplace_new <| qmacro_expr(${ var $i(len_var) = length($i(cast_var).list); })

    // Walk through pattern statements
    // Track pending $b capture: save_var holds start position, b_vname holds user variable name
    var pending_b_save = ""   // name of the var holding wildcard start position
    var pending_b_vname = ""  // name of the user's $b capture variable

    let pat_len = length(pat_block.list)
    for (pat_stmt, pat_idx in pat_block.list, range(pat_len)) {
        let pat = get_ptr(pat_stmt)
        let wc_name = get_wildcard_name(pat)

        if (wc_name == "_wildcard") {
            // Zero or more — just skip. The next fixed pattern will scan forward.
            // Save start position for $b capture
            let b_vname = get_wildcard_b_tag(pat)
            if (!empty(b_vname)) {
                let save_var = next_var(index)
                body |> emplace_new <| qmacro_expr(${ var $i(save_var) = $i(pos_var); })
                pending_b_save = save_var
                pending_b_vname = b_vname
            }
            if (pat_idx == pat_len - 1) {
                // Trailing wildcard — matches rest, done
                // Emit $b capture: [save_pos, len)
                if (!empty(pending_b_vname)) {
                    body |> emplace_new <| qmacro_expr(${ qm_extract_stmts($i(blk_var), $i(pending_b_save), $i(len_var), $i(pending_b_vname)); })
                    pending_b_save = ""
                    pending_b_vname = ""
                }
            }
            // Otherwise, the next fixed statement will handle scanning
        } elif (wc_name == "_wildcard1") {
            // One or more — advance pos by at least 1
            let b_vname = get_wildcard_b_tag(pat)
            if (!empty(b_vname)) {
                let save_var = next_var(index)
                body |> emplace_new <| qmacro_expr(${ var $i(save_var) = $i(pos_var); })
                pending_b_save = save_var
                pending_b_vname = b_vname
            }
            var inscope cond <- qmacro($i(pos_var) >= $i(len_var))
            var inscope fail <- qmacro(qm_fail_wildcard($i(actual_var)))
            body |> emplace_new <| qm_guard(at, cond, fail)
            body |> emplace_new <| qmacro_expr(${ $i(pos_var) ++; })
            if (pat_idx == pat_len - 1) {
                // Trailing _wildcard1 — capture [save_pos, len)
                if (!empty(pending_b_vname)) {
                    body |> emplace_new <| qmacro_expr(${ qm_extract_stmts($i(blk_var), $i(pending_b_save), $i(len_var), $i(pending_b_vname)); })
                    pending_b_save = ""
                    pending_b_vname = ""
                }
            }
        } elif (wc_name == "_optional") {
            // Zero or one — similar to _wildcard for $b
            let b_vname = get_wildcard_b_tag(pat)
            if (!empty(b_vname)) {
                let save_var = next_var(index)
                body |> emplace_new <| qmacro_expr(${ var $i(save_var) = $i(pos_var); })
                pending_b_save = save_var
                pending_b_vname = b_vname
            }
            pass
        } elif (wc_name == "_any") {
            // Exactly one — advance pos by exactly 1
            let b_vname = get_wildcard_b_tag(pat)
            var inscope cond <- qmacro($i(pos_var) >= $i(len_var))
            var inscope fail <- qmacro(qm_fail_wildcard($i(actual_var)))
            body |> emplace_new <| qm_guard(at, cond, fail)
            if (!empty(b_vname)) {
                // _any captures exactly one statement
                body |> emplace_new <| qmacro_expr(${ qm_extract_stmts($i(blk_var), $i(pos_var), $i(pos_var) + 1, $i(b_vname)); })
            }
            body |> emplace_new <| qmacro_expr(${ $i(pos_var) ++; })
        } else {
            // Fixed statement — must match at current position
            // Check if the previous statement was a wildcard (need to scan)
            let prev_is_wildcard = pat_idx > 0 && !empty(get_wildcard_name(get_ptr(pat_block.list[pat_idx - 1])))
            let prev_wc = prev_is_wildcard ? get_wildcard_name(get_ptr(pat_block.list[pat_idx - 1])) : ""

            if (prev_wc == "_wildcard" || prev_wc == "_optional") {
                // Scan forward: build an inner matcher block and call qm_scan
                let elem_var = next_var(index)

                // Generate match code for the fixed pattern
                var inscope match_body : array<ExpressionPtr>
                generate_match(pat, elem_var, match_body, index, at)
                match_body |> emplace_new <| qmacro_expr(${return qm_ok(); })

                // Build inner block: $(var elem : ExpressionPtr) : QMatchResult { match_code }
                var inscope inner_blk <- new ExprBlock(at = at)
                unsafe {
                    var ip = reinterpret<ExprBlock?> inner_blk
                    ip.blockFlags |= ExprBlockFlags.isClosure
                    ip.returnType |> move_new <| typeinfo ast_typedecl(type<QMatchResult>)
                    var inscope inner_arg <- new Variable(at = at, name := elem_var)
                    inner_arg._type |> move_new <| typeinfo ast_typedecl(type<ExpressionPtr>)
                    inner_arg.access_flags |= VariableAccessFlags.access_ref
                    ip.arguments |> emplace(inner_arg)
                    for (stmt in match_body) {
                        ip.list |> emplace(stmt)
                    }
                }
                var inscope inner_mkblk <- new ExprMakeBlock(at = at, _block <- inner_blk)

                // Generate: var scan_r = qm_scan(blk_var, pos_var, len_var, inner_block)
                let scan_r_var = next_var(index)
                body |> emplace_new <| qmacro_expr(${ var $i(scan_r_var) = qm_scan($i(blk_var), $i(pos_var), $i(len_var), $e(inner_mkblk)); })
                // Guard on scan result
                var inscope scan_cond <- qmacro($i(scan_r_var).matched == false)
                var inscope scan_fail <- qmacro($i(scan_r_var))
                body |> emplace_new <| qm_guard(at, scan_cond, scan_fail)

                // Emit pending $b capture: [save_pos, pos - 1) — pos was updated by qm_scan to after the match
                if (!empty(pending_b_vname)) {
                    body |> emplace_new <| qmacro_expr(${ qm_extract_stmts($i(blk_var), $i(pending_b_save), $i(pos_var) - 1, $i(pending_b_vname)); })
                    pending_b_save = ""
                    pending_b_vname = ""
                }
            } else {
                // No preceding wildcard — must match at exact position
                // Emit pending $b capture before this fixed match (for _wildcard1 followed by fixed)
                if (!empty(pending_b_vname)) {
                    body |> emplace_new <| qmacro_expr(${ qm_extract_stmts($i(blk_var), $i(pending_b_save), $i(pos_var), $i(pending_b_vname)); })
                    pending_b_save = ""
                    pending_b_vname = ""
                }
                // Bounds check
                var inscope cond <- qmacro($i(pos_var) >= $i(len_var))
                var inscope fail <- qmacro(qm_fail_wildcard($i(actual_var)))
                body |> emplace_new <| qm_guard(at, cond, fail)
                // Get element at position
                let elem_var = next_var(index)
                body |> emplace_new <| qmacro_expr(${ var inscope $i(elem_var) <- clone_expression($i(cast_var).list[$i(pos_var)]); })
                // Match it
                generate_match(pat, elem_var, body, index, at)
                // Advance
                body |> emplace_new <| qmacro_expr(${ $i(pos_var) ++; })
            }
        }
    }

    // After all patterns, check that we consumed everything (unless trailing wildcard)
    let last_is_wildcard = pat_len > 0 && !empty(get_wildcard_name(get_ptr(pat_block.list[pat_len - 1])))
    if (!last_is_wildcard) {
        var inscope cond <- qmacro($i(pos_var) != $i(len_var))
        var inscope fail <- qmacro(qm_fail_list($i(actual_var)))
        body |> emplace_new <| qm_guard(at, cond, fail)
    }
}

// ── Argument validation ─────────────────────────────────────────────────

[macro_function]
def private qm_validate_expr_arg(prog : ProgramPtr; arg : ExpressionPtr; at : LineInfo; var result : ExpressionPtr&) : bool {
    if (arg._type == null) {
        macro_error(prog, at, "qmatch: first argument has no type")
        return false
    }
    if (arg._type.isSmartPointer) {
        result |> move_new <| clone_expression(arg)
        return true
    } elif (arg._type.baseType == Type.tPointer) {
        var inscope cloned <- clone_expression(arg)
        result |> move_new <| qmacro(add_ptr_ref($e(cloned)))
        return true
    } else {
        macro_error(prog, at, "qmatch: first argument must be Expression? or ExpressionPtr, got {describe(arg._type)}")
        return false
    }
}

// ── Call macros ─────────────────────────────────────────────────────────

[call_macro(name="qmatch")]
class private QMatch : AstCallMacro {
    def override canVisitArgument(call : smart_ptr<ExprCallMacro>; argIndex : int) : bool {
        return argIndex == 0
    }
    def override visit(prog : ProgramPtr; mod : Module?; var call : smart_ptr<ExprCallMacro>) : ExpressionPtr {
        if (call.arguments |> length != 2) {
            macro_error(prog, call.at, "expecting qmatch(expression, pattern)")
            return <- default<ExpressionPtr>
        }
        let at = call.at
        let pattern = get_ptr(call.arguments[1])

        // Build block: $(var _qm_arg : ExpressionPtr) : QMatchResult { ... }
        var inscope blk <- new ExprBlock(at = at)
        unsafe {
            var blk_ptr = reinterpret<ExprBlock?> blk
            blk_ptr.blockFlags |= ExprBlockFlags.isClosure
            blk_ptr.returnType |> move_new <| typeinfo ast_typedecl(type<QMatchResult>)
            var inscope arg_var <- new Variable(at = at, name := "_qm_arg")
            arg_var._type |> move_new <| typeinfo ast_typedecl(type<ExpressionPtr>)
            arg_var.access_flags |= VariableAccessFlags.access_ref
            blk_ptr.arguments |> emplace(arg_var)

            // Null check
            blk_ptr.list |> emplace_new <| qm_null_guard(at, "_qm_arg")
            // Pattern matching
            var index = 0
            var inscope body : array<ExpressionPtr>
            generate_match(pattern, "_qm_arg", body, index, at)
            for (stmt in body) {
                blk_ptr.list |> emplace(stmt)
            }
            blk_ptr.list |> emplace_new <| qmacro_expr(${return qm_ok(); })
        }

        var inscope expr_arg : ExpressionPtr
        if (!qm_validate_expr_arg(prog, call.arguments[0], at, expr_arg)) {
            return <- default<ExpressionPtr>
        }

        var inscope mkblk <- new ExprMakeBlock(at = at, _block <- blk)
        return <- new ExprCall(at = at,
            name := "qm_run",
            arguments := array<ExpressionPtr>(expr_arg, mkblk)
        )
    }
}

[call_macro(name="qmatch_block")]
class private QMatchBlock : AstCallMacro {
    def override canVisitArgument(call : smart_ptr<ExprCallMacro>; argIndex : int) : bool {
        return argIndex == 0  // QMatchBlock
    }
    def override visit(prog : ProgramPtr; mod : Module?; var call : smart_ptr<ExprCallMacro>) : ExpressionPtr {
        if (call.arguments |> length != 2) {
            macro_error(prog, call.at, "expecting qmatch_block(expression, block_pattern)")
            return <- default<ExpressionPtr>
        }
        let at = call.at

        // arg[1] is ExprMakeBlock containing ExprBlock
        let pat_arg = get_ptr(call.arguments[1])
        if (!(pat_arg is ExprMakeBlock)) {
            macro_error(prog, at, "qmatch_block: second argument must be a block")
            return <- default<ExpressionPtr>
        }
        let pat_mkblk = pat_arg as ExprMakeBlock
        var pat_block = get_ptr(pat_mkblk._block) as ExprBlock
        if (pat_block == null) {
            macro_error(prog, at, "qmatch_block: could not extract block from pattern")
            return <- default<ExpressionPtr>
        }

        // Build block: $(var _qm_arg : ExpressionPtr) : QMatchResult { ... }
        var inscope blk <- new ExprBlock(at = at)
        unsafe {
            var blk_ptr = reinterpret<ExprBlock?> blk
            blk_ptr.blockFlags |= ExprBlockFlags.isClosure
            blk_ptr.returnType |> move_new <| typeinfo ast_typedecl(type<QMatchResult>)
            var inscope arg_var <- new Variable(at = at, name := "_qm_arg")
            arg_var._type |> move_new <| typeinfo ast_typedecl(type<ExpressionPtr>)
            arg_var.access_flags |= VariableAccessFlags.access_ref
            blk_ptr.arguments |> emplace(arg_var)

            // Null check
            blk_ptr.list |> emplace_new <| qm_null_guard(at, "_qm_arg")
            // Block matching
            var index = 0
            var inscope body : array<ExpressionPtr>
            generate_block_match(pat_block, "_qm_arg", body, index, at)
            for (stmt in body) {
                blk_ptr.list |> emplace(stmt)
            }
            blk_ptr.list |> emplace_new <| qmacro_expr(${return qm_ok(); })
        }

        var inscope expr_arg : ExpressionPtr
        if (!qm_validate_expr_arg(prog, call.arguments[0], at, expr_arg)) {
            return <- default<ExpressionPtr>
        }

        var inscope mkblk <- new ExprMakeBlock(at = at, _block <- blk)
        return <- new ExprCall(at = at,
            name := "qm_run",
            arguments := array<ExpressionPtr>(expr_arg, mkblk)
        )
    }
}

[call_macro(name="qmatch_function")]
class private QMatchFunction : AstCallMacro {
    //! ``qmatch_function(func, block_pattern)`` — match function body against block pattern.
    def override canVisitArgument(call : smart_ptr<ExprCallMacro>; argIndex : int) : bool {
        return argIndex == 0
    }
    def override visit(prog : ProgramPtr; mod : Module?; var call : smart_ptr<ExprCallMacro>) : ExpressionPtr {
        if (call.arguments |> length != 2) {
            macro_error(prog, call.at, "expecting qmatch_function(func, block_pattern)")
            return <- default<ExpressionPtr>
        }
        let at = call.at

        let pat_arg = get_ptr(call.arguments[1])
        if (!(pat_arg is ExprMakeBlock)) {
            macro_error(prog, at, "qmatch_function: second argument must be a block")
            return <- default<ExpressionPtr>
        }
        let pat_mkblk = pat_arg as ExprMakeBlock
        var pat_block = get_ptr(pat_mkblk._block) as ExprBlock
        if (pat_block == null) {
            macro_error(prog, at, "qmatch_function: could not extract block from pattern")
            return <- default<ExpressionPtr>
        }

        // Build block: $(var _qm_arg : ExpressionPtr; _qm_func : Function?) : QMatchResult { ... }
        var inscope blk <- new ExprBlock(at = at)
        unsafe {
            var blk_ptr = reinterpret<ExprBlock?> blk
            blk_ptr.blockFlags |= ExprBlockFlags.isClosure
            blk_ptr.returnType |> move_new <| typeinfo ast_typedecl(type<QMatchResult>)
            // First param: body (ExpressionPtr)
            var inscope arg_body <- new Variable(at = at, name := "_qm_arg")
            arg_body._type |> move_new <| typeinfo ast_typedecl(type<ExpressionPtr>)
            arg_body.access_flags |= VariableAccessFlags.access_ref
            blk_ptr.arguments |> emplace(arg_body)
            // Second param: function pointer (Function?)
            var inscope arg_func <- new Variable(at = at, name := "_qm_func")
            arg_func._type |> move_new <| typeinfo ast_typedecl(type<Function?>)
            blk_ptr.arguments |> emplace(arg_func)

            blk_ptr.list |> emplace_new <| qm_null_guard(at, "_qm_arg")

            var index = 0
            var inscope body : array<ExpressionPtr>

            // Match function arguments and return type against _qm_func
            generate_pattern_args_match(pat_block, "_qm_func", "result", "_qm_arg", body, index, at)

            // Match function body (skip arg check — already done against _qm_func)
            generate_block_match(pat_block, "_qm_arg", body, index, at, false)
            for (stmt in body) {
                blk_ptr.list |> emplace(stmt)
            }
            blk_ptr.list |> emplace_new <| qmacro_expr(${return qm_ok(); })
        }

        var inscope func_arg <- clone_expression(call.arguments[0])
        var inscope mkblk_f <- new ExprMakeBlock(at = at, _block <- blk)
        return <- new ExprCall(at = at,
            name := "qm_run_function",
            arguments := array<ExpressionPtr>(func_arg, mkblk_f)
        )
    }
}
