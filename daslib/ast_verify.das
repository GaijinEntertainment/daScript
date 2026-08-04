options gen2
options indenting = 4
options strict_smart_pointers = true

module ast_verify shared private

//! AST invariant verifier.
//!
//! Reports a clean ``macro_error`` when an AST violates a structural invariant the
//! compiler relies on, instead of letting it segfault several passes later::
//!
//!     daslang --ast-verify foo.das
//!
//! or ``require daslib/ast_verify``, or call [[verify_module]] /
//! [[verify_expression]] from your own macro right after building AST.

require daslib/ast
require daslib/ast_boost
require strings

def private placeholder(at : LineInfo) : ExpressionPtr {
    return new ExprConstInt(at = at, value = 0)
}

def private placeholder_block(at : LineInfo) : ExpressionPtr {
    return new ExprBlock(at = at)
}

def private placeholder_bool(at : LineInfo) : ExpressionPtr {
    return new ExprConstBool(at = at, value = false)
}

let private STMT_ONLY_RTTI <- {
    "ExprBlock", "ExprLet", "ExprWhile", "ExprFor", "ExprIfThenElse", "ExprWith",
    "ExprUnsafe", "ExprTryCatch", "ExprReturn", "ExprBreak", "ExprContinue", "ExprGoto",
    "ExprLabel", "ExprYield", "ExprDelete", "ExprCopy", "ExprMove", "ExprClone",
    "ExprStaticAssert", "ExprAssume"
}

def private is_stmt_node(e : ExpressionPtr) : bool {
    return e != null && key_exists(STMT_ONLY_RTTI, e.__rtti)
}


def private repair_kind(want : string; at : LineInfo) : ExpressionPtr {
    if (want == "ExprMakeBlock") {
        return new ExprMakeBlock(at = at, _block = new ExprBlock(at = at))
    }
    if (want == "ExprFor") {
        return new ExprFor(at = at, body = new ExprBlock(at = at))
    }
    return new ExprBlock(at = at)
}


def private unqualified(nm : string) : string {
    let c = rfind(nm, "::")
    return c >= 0 ? slice(nm, c + 2) : nm
}

def private same_var_name(a, b : string) : bool {
    return unqualified(a) == unqualified(b)
}

def private is_enum_type(bt : Type) : bool {
    return (bt == Type.tEnumeration || bt == Type.tEnumeration8
        || bt == Type.tEnumeration16 || bt == Type.tEnumeration64)
}




class private AstVerifyVisitor : AstVisitor {
    error_count : int = 0
    @do_not_delete current_fn : Function?
    @do_not_delete fn_stack : array<Function?>
    seen_nodes : table<uint64; bool>
    check_unique : bool = false
    // `return` inside a closure block returns from the BLOCK, not the function, so
    // its null subexpr says nothing about the function's result type
    closure_depth : int = 0

    // Compiler-generated functions are filled in ACROSS passes, so mid-pass they
    // legitimately look half-built — checking them gave 19 false positives.
    def override canVisitFunction(fun : Function?) : bool {
        return !fun.flags.generated
    }

    def report(at : LineInfo; msg : string) : void {
        error_count++
        to_log(LOG_ERROR, "AST verify: {msg} at {describe(at)}\n")
        compiling_program() |> macro_error(at, "AST verify: {msg}")
    }

    def verify_elements(var list : dasvector`ptr`Expression; at : LineInfo; what : string) : void {
        for (i in range(length(list))) {
            need(list[i], "{what}[{i}]", at)
        }
    }

    def kind_slot(var slot : ExpressionPtr; want, what : string; at : LineInfo) : ExpressionPtr {
        if (slot == null) {
            report(at, "{what} is null (must be {want})")
            return repair_kind(want, at)
        }
        if (slot.__rtti != want) {
            report(at, "{what} is {slot.__rtti}, but inference static_casts it to {want}")
            return repair_kind(want, at)
        }
        return slot
    }

    def verify_type(var typ : TypeDeclPtr; what : string;
                    at : LineInfo) : void {
        if (typ == null) {
            return
        }
        if (typ.baseType == Type.tArray || typ.baseType == Type.tFixedArray
                || typ.baseType == Type.tIterator) {
            need_type(typ.firstType, "{what}: {typ.baseType} has no element type (firstType is null)", at)
        } elif (typ.baseType == Type.tTable) {
            need_type(typ.firstType, "{what}: table has no key type (firstType is null)", at)
            need_type(typ.secondType, "{what}: table has no value type (secondType is null)", at)
        }
        for (i in range(length(typ.argTypes))) {
            need_type(typ.argTypes[i], "{what}: {typ.baseType} argTypes[{i}] is null", at)
        }
        if (typ.baseType == Type.tStructure && typ.structType == null) {
            report(at, "{what}: tStructure with a null structType")
            typ.baseType = Type.tInt
        } elif (is_enum_type(typ.baseType) && typ.enumType == null) {
            report(at, "{what}: {typ.baseType} with a null enumType")
            typ.baseType = Type.tInt
        } elif (typ.baseType == Type.tHandle && typ.annotation == null) {
            report(at, "{what}: tHandle with a null annotation")
            typ.baseType = Type.tInt
        } elif (typ.baseType == Type.tDistinct && (typ.annotation == null || typ.firstType == null)) {
            // a distinct type IS its DistinctTypeAnnotation, dereferenced unguarded
            report(at, "{what}: tDistinct without a DistinctTypeAnnotation or underlying type")
            typ.baseType = Type.tInt
        }
    }

    def verify_variable(var v : VariablePtr; what : string) : void {
        if (v == null) {
            return
        }
        if (v._type == null) {
            need_type(v._type, "{what} '{v.name}' has no type (Variable._type is null); the parser uses autoinfer here",
                v.at, Type.autoinfer)
            return
        }
        verify_type(v._type, "{what} '{v.name}' type", v.at)
    }

    def verify_value_elements(var list : dasvector`ptr`Expression; at : LineInfo;
                              what : string) : void {
        for (i in range(length(list))) {
            value_slot(list[i], "{what}[{i}]", at)
        }
    }

    def override preVisitExpression(var expr : ExpressionPtr) : void {
        if (!check_unique || expr == null) {
            return
        }
        let key = intptr(expr)
        if (key_exists(seen_nodes, key)) {
            report(expr.at, "node {expr.__rtti} is aliased — reachable from two parents (missing clone_expression)")
            return
        }
        seen_nodes |> insert(key, true)
    }

    // Reporting alone is not enough: the visitor descends into the slot right after
    // preVisit, so a null has to be replaced or the verify pass segfaults itself.
    def need_type(var slot : TypeDeclPtr&; msg : string; at : LineInfo;
                  bt : Type = Type.tInt) : void {
        if (slot != null) {
            return
        }
        report(at, msg)
        slot = new TypeDecl(at = at, baseType = bt)
    }

    // Reporting alone is not enough: the visitor descends into the slot right after
    // preVisit, so a null has to be replaced or the verify pass segfaults itself.
    // `stand_in` clones a sibling instead, when one operand can stand for the other.
    def need(var slot : ExpressionPtr&; what : string; at : LineInfo;
             mk : function<(at : LineInfo) : ExpressionPtr> = @@placeholder;
             detail : string = ""; stand_in : ExpressionPtr = null) : void {
        if (slot != null) {
            return
        }
        report(at, empty(detail) ? "{what} is null" : "{what} is null ({detail})")
        slot = stand_in != null ? clone_expression(stand_in) : invoke(mk, at)
    }

    def value_slot(var slot : ExpressionPtr&; what : string; at : LineInfo) : void {
        if (is_stmt_node(slot)) {
            report(at, "{what} holds a statement ({slot.__rtti}) where a value is required")
            slot = placeholder(at)
        }
    }

    def override preVisitTypeDecl(var typ : TypeDeclPtr) : void {
        verify_type(typ, "TypeDecl", typ != null ? typ.at : LineInfo())
    }

    // The visitor dispatches on the CONCRETE kind, so preVisitExprAt never fires for
    // an ExprSafeAt — every derived kind needs its own override, delegating here.
    def check_at(var subexpr : ExpressionPtr&; var index : ExpressionPtr&; kind : string;
                 at : LineInfo) : void {
        need(subexpr, "{kind}.subexpr", at)
        need(index, "{kind}.index", at)
        value_slot(subexpr, "{kind}.subexpr", at)
        value_slot(index, "{kind}.index", at)
    }

    def check_field(var value : ExpressionPtr&; kind, name : string; at : LineInfo) : void {
        need(value, "{kind}.value", at, detail = "field '{name}'")
        value_slot(value, "{kind}.value", at)
    }

    def check_op2(var left : ExpressionPtr&; var right : ExpressionPtr&; kind : string;
                  at : LineInfo) : void {
        need(left, "{kind}.left", at)
        need(right, "{kind}.right", at)
        value_slot(left, "{kind}.left", at)
        value_slot(right, "{kind}.right", at)
    }

    def check_subexpr(var subexpr : ExpressionPtr&; kind : string; at : LineInfo) : void {
        need(subexpr, "{kind}.subexpr", at)
        value_slot(subexpr, "{kind}.subexpr", at)
    }

    def override preVisitExprField(var expr : ExprField?) : void {
        check_field(expr.value, "ExprField", string(expr.name), expr.at)
    }

    def override preVisitExprSafeField(var expr : ExprSafeField?) : void {
        check_field(expr.value, "ExprSafeField", string(expr.name), expr.at)
    }

    def override preVisitExprIsVariant(var expr : ExprIsVariant?) : void {
        check_field(expr.value, "ExprIsVariant", string(expr.name), expr.at)
    }

    def override preVisitExprAsVariant(var expr : ExprAsVariant?) : void {
        check_field(expr.value, "ExprAsVariant", string(expr.name), expr.at)
    }

    def override preVisitExprSafeAsVariant(var expr : ExprSafeAsVariant?) : void {
        check_field(expr.value, "ExprSafeAsVariant", string(expr.name), expr.at)
    }

    def override preVisitExprOp1(var expr : ExprOp1?) : void {
        need(expr.subexpr, "ExprOp1.subexpr", expr.at, detail = "op '{expr.op}'")
        value_slot(expr.subexpr, "ExprOp1.subexpr", expr.at)
    }

    def override preVisitExprOp2(var expr : ExprOp2?) : void {
        let op2 = "op '{expr.op}'"
        need(expr.left, "ExprOp2.left", expr.at, detail = op2, stand_in = expr.right)
        need(expr.right, "ExprOp2.right", expr.at, detail = op2, stand_in = expr.left)
        value_slot(expr.left, "ExprOp2.left", expr.at)
        value_slot(expr.right, "ExprOp2.right", expr.at)
    }

    def override preVisitExprCopy(var expr : ExprCopy?) : void {
        check_op2(expr.left, expr.right, "ExprCopy", expr.at)
    }

    def override preVisitExprMove(var expr : ExprMove?) : void {
        check_op2(expr.left, expr.right, "ExprMove", expr.at)
    }

    def override preVisitExprClone(var expr : ExprClone?) : void {
        check_op2(expr.left, expr.right, "ExprClone", expr.at)
    }

    def override preVisitExprRef2Ptr(var expr : ExprRef2Ptr?) : void {
        check_subexpr(expr.subexpr, "ExprRef2Ptr", expr.at)
    }

    def override preVisitExprYield(var expr : ExprYield?) : void {
        check_subexpr(expr.subexpr, "ExprYield", expr.at)
    }

    def override preVisitExprIs(var expr : ExprIs?) : void {
        check_subexpr(expr.subexpr, "ExprIs", expr.at)
        need_type(expr.typeexpr, "ExprIs.typeexpr is null", expr.at)
        verify_type(expr.typeexpr, "ExprIs.typeexpr", expr.at)
    }

    def override preVisitExprAt(var expr : ExprAt?) : void {
        check_at(expr.subexpr, expr.index, "ExprAt", expr.at)
    }

    def override preVisitExprSafeAt(var expr : ExprSafeAt?) : void {
        check_at(expr.subexpr, expr.index, "ExprSafeAt", expr.at)
    }

    def override preVisitExprWhile(var expr : ExprWhile?) : void {
        need(expr.cond, "ExprWhile.cond", expr.at)
        value_slot(expr.cond, "ExprWhile.cond", expr.at)
        need(expr.body, "ExprWhile.body", expr.at)
    }

    def override preVisitExprFor(var expr : ExprFor?) : void {
        // ExprFor.body is deliberately NOT null-checked: it is transiently null.
        verify_elements(expr.sources, expr.at, "ExprFor.sources")
        verify_value_elements(expr.sources, expr.at, "ExprFor.sources")
    }

    // finalList only: a null in `list` compiles clean, so it is not a crash shape
    def override preVisitExprBlock(var expr : ExprBlock?) : void {
        // ExprBlock::visit walks `arguments` only when isClosure; inference does not
        if (!expr.blockFlags.isClosure && !empty(expr.arguments)) {
            report(expr.at, "ExprBlock has arguments but is not marked isClosure")
            expr.blockFlags.isClosure = true
        }
        if (expr.blockFlags.isClosure) {
            closure_depth++
        }
        verify_elements(expr.finalList, expr.at, "ExprBlock.finalList")
    }

    def override visitExprBlock(var expr : ExprBlock?) : ExpressionPtr {
        if (expr.blockFlags.isClosure && closure_depth > 0) {
            closure_depth--
        }
        return expr
    }

    def override preVisitExprOp3(var expr : ExprOp3?) : void {
        let op3 = "op '{expr.op}'"
        need(expr.subexpr, "ExprOp3.subexpr", expr.at, @@placeholder_bool, op3)
        need(expr.left, "ExprOp3.left", expr.at, detail = op3, stand_in = expr.right)
        need(expr.right, "ExprOp3.right", expr.at, detail = op3, stand_in = expr.left)
        value_slot(expr.subexpr, "ExprOp3.subexpr", expr.at)
        value_slot(expr.left, "ExprOp3.left", expr.at)
        value_slot(expr.right, "ExprOp3.right", expr.at)
    }

    def override preVisitExprIfThenElse(var expr : ExprIfThenElse?) : void {
        need(expr.cond, "ExprIfThenElse.cond", expr.at, @@placeholder_bool)
        value_slot(expr.cond, "ExprIfThenElse.cond", expr.at)
        need(expr.if_true, "ExprIfThenElse.if_true", expr.at, @@placeholder_block)
        // if_false is legitimately null — an `if` without `else`.
    }

    def override preVisitExprTryCatch(var expr : ExprTryCatch?) : void {
        need(expr.try_block, "ExprTryCatch.try_block", expr.at, @@placeholder_block)
        need(expr.catch_block, "ExprTryCatch.catch_block", expr.at, @@placeholder_block)
    }

    def override preVisitExprWith(var expr : ExprWith?) : void {
        // `with (module x)` has no subject: ast_expressions.h documents with==nullptr
        let module_with = !empty(expr.moduleName)
        if (!module_with) {
            need(expr._with, "ExprWith.with", expr.at)
        }
        if (!module_with) {
            value_slot(expr._with, "ExprWith.with", expr.at)
        }
        need(expr.body, "ExprWith.body", expr.at, @@placeholder_block)
    }

    def override preVisitExprUnsafe(var expr : ExprUnsafe?) : void {
        need(expr.body, "ExprUnsafe.body", expr.at, @@placeholder_block)
    }

    def override preVisitExprRef2Value(var expr : ExprRef2Value?) : void {
        need(expr.subexpr, "ExprRef2Value.subexpr", expr.at)
    }

    def override preVisitExprPtr2Ref(var expr : ExprPtr2Ref?) : void {
        need(expr.subexpr, "ExprPtr2Ref.subexpr", expr.at)
    }

    def override preVisitExprCast(var expr : ExprCast?) : void {
        need(expr.subexpr, "ExprCast.subexpr", expr.at)
        if (expr.castType != null && (expr.castType.baseType == Type.tBlock
                || expr.castType.baseType == Type.tFunction
                || expr.castType.baseType == Type.tLambda)) {
            need_type(expr.castType.firstType,
                "ExprCast.castType is {expr.castType.baseType} with no return type (firstType is null)",
                expr.at, Type.tVoid)
        }
        value_slot(expr.subexpr, "ExprCast.subexpr", expr.at)
    }

    def override preVisitExprSwizzle(var expr : ExprSwizzle?) : void {
        need(expr.value, "ExprSwizzle.value", expr.at)
        value_slot(expr.value, "ExprSwizzle.value", expr.at)
    }

    def override preVisitExprNullCoalescing(var expr : ExprNullCoalescing?) : void {
        need(expr.defaultValue, "ExprNullCoalescing.defaultValue", expr.at)
        value_slot(expr.subexpr, "ExprNullCoalescing.subexpr", expr.at)
        value_slot(expr.defaultValue, "ExprNullCoalescing.defaultValue", expr.at)
    }

    def override preVisitExprNamedCall(var expr : ExprNamedCall?) : void {
        if (expr.arguments == null) {
            report(expr.at, "ExprNamedCall.arguments (named argument list) is null")
            expr.arguments = new MakeStruct(uninitialized)
        }
    }

    def override preVisitExprAscend(var expr : ExprAscend?) : void {
        need(expr.subexpr, "ExprAscend.subexpr", expr.at)
        verify_type(expr.ascType, "ExprAscend.ascType", expr.at)
    }

    def override preVisitExprMakeTuple(var expr : ExprMakeTuple?) : void {
        verify_type(expr.recordType, "ExprMakeTuple.recordType", expr.at)
        verify_type(expr.makeType, "ExprMakeTuple.makeType", expr.at)
    }

    def override preVisitExprMakeBlock(var expr : ExprMakeBlock?) : void {
        expr._block = kind_slot(expr._block, "ExprBlock", "ExprMakeBlock.block", expr.at)
        if (!expr.mmFlags.isLambda) {
            return
        }
        // kind_slot just guaranteed the rtti, so the cast is sound
        var blk = unsafe(reinterpret<ExprBlock?>(expr._block))
        if (!blk.blockFlags.isLambdaBlock) {
            // generateLambdaFunction copies *block->type unguarded
            report(expr.at, "ExprMakeBlock is isLambda but its block is not isLambdaBlock")
            expr.mmFlags.isLambda = false
        }
    }

    def override preVisitExprMakeStruct(var expr : ExprMakeStruct?) : void {
        if (expr._block != null) {
            expr._block = kind_slot(expr._block, "ExprMakeBlock", "ExprMakeStruct.block", expr.at)
        }
    }

    def override preVisitExprArrayComprehension(var expr : ExprArrayComprehension?) : void {
        expr.exprFor = kind_slot(expr.exprFor, "ExprFor", "ExprArrayComprehension.exprFor", expr.at)
        check_subexpr(expr.subexpr, "ExprArrayComprehension", expr.at)
    }

    def override preVisitExprLetVariable(var _expr : ExprLet?; var arg : VariablePtr;
                                         _lastArg : bool) : void {
        verify_variable(arg, "let variable")
    }

    def override preVisitExprBlockArgument(var _blk : ExprBlock?; var arg : VariablePtr;
                                           _lastArg : bool) : void {
        verify_variable(arg, "block argument")
    }

    def override preVisitExprStringBuilder(var expr : ExprStringBuilder?) : void {
        verify_elements(expr.elements, expr.at, "ExprStringBuilder.elements")
        verify_value_elements(expr.elements, expr.at, "ExprStringBuilder.elements")
    }

    def override preVisitExprMakeArray(var expr : ExprMakeArray?) : void {
        verify_type(expr.recordType, "ExprMakeArray.recordType", expr.at)
        verify_type(expr.makeType, "ExprMakeArray.makeType", expr.at)
        verify_elements(expr.values, expr.at, "ExprMakeArray.values")
        verify_value_elements(expr.values, expr.at, "ExprMakeArray.values")
    }

    def check_make_field(var decl : MakeFieldDeclPtr; kind : string) : void {
        if (decl == null) {
            return
        }
        need(decl.value, "{kind}.value", decl.at, detail = "field '{decl.name}'")
    }

    def override preVisitExprMakeStructField(var _expr : ExprMakeStruct?; _index : int; var decl : MakeFieldDeclPtr; _last : bool) : void {
        check_make_field(decl, "MakeFieldDecl")
    }

    def override preVisitExprMakeVariantField(var _expr : ExprMakeVariant?; _index : int; var decl : MakeFieldDeclPtr; _last : bool) : void {
        check_make_field(decl, "ExprMakeVariant field")
    }

    def override preVisitExprNamedCallArgument(var _expr : ExprNamedCall?; var arg : MakeFieldDeclPtr; _last : bool) : void {
        check_make_field(arg, "ExprNamedCall argument")
    }

    def override preVisitExprMakeTupleIndex(var expr : ExprMakeTuple?; index : int; var init : ExpressionPtr; _last : bool) : void {
        need(expr.values[index], "ExprMakeTuple.values[{index}]", expr.at)
    }

    def override visitExprVar(var expr : ExprVar?) : ExpressionPtr {
        let v = expr.variable
        // `var x aka X` gives the variable a legitimate second name
        var internal = false
        peek(expr.name) $(nm) {
            internal = find(nm, "`") >= 0
        }
        if (v != null && !expr.genFlags.generated
                && !same_var_name(string(expr.name), string(v.name))
                && !same_var_name(string(expr.name), string(v._aka)) && !internal) {
            report(expr.at, "ExprVar.name '{expr.name}' disagrees with its resolved variable '{v.name}'")
            expr.name := v.name
        }
        return expr
    }

    def override preVisitFunction(var fun : FunctionPtr) : void {
        fn_stack |> push(current_fn)
        current_fn = fun
        verify_type(fun.result, "function '{fun.name}' result type", fun.at)
    }

    def override preVisitFunctionArgument(var fun : FunctionPtr; var arg : VariablePtr; _last : bool) : void {
        verify_variable(arg, "function '{fun.name}' argument")
    }

    def override preVisitStructureField(var str : StructurePtr; var decl : FieldDeclaration; _last : bool) : void {
        verify_type(decl._type, "field '{str.name}.{decl.name}' type", decl.at)
    }

    def override visitFunction(var fun : FunctionPtr) : FunctionPtr {
        if (empty(fn_stack)) {
            current_fn = null
        } else {
            current_fn = fn_stack[length(fn_stack) - 1]
            fn_stack |> pop()
        }
        return fun
    }

    def override preVisitExprReturn(var expr : ExprReturn?) : void {
        if (expr.subexpr != null || current_fn == null || closure_depth > 0) {
            return
        }
        // compiler-internal functions carry a backtick in their mangled name
        var internal = false
        peek(current_fn.name) $(nm) {
            internal = find(nm, "`") >= 0
        }
        if (internal) {
            return
        }
        // an auto/alias result is still being inferred, so it says nothing yet
        if (current_fn.result != null && !current_fn.result.isAutoOrAlias
                && current_fn.result.baseType != Type.tVoid) {
            need(expr.subexpr, "ExprReturn.subexpr", expr.at,
                detail = "in '{current_fn.name}', which returns non-void")
        }
    }

    def override preVisitExprCall(var expr : ExprCall?) : void {
        verify_elements(expr.arguments, expr.at, "ExprCall.arguments")
        verify_value_elements(expr.arguments, expr.at, "ExprCall.arguments")
        // no arity check: the sort intrinsic renames calls in place, leaving `func`
    }
}

def public verify_module(prog : ProgramPtr; mod : Module?) : int {
    //! Verifies one module in place, emitting a ``macro_error`` per violation and
    //! returning the count. Call it from a macro's ``apply()`` right after building
    //! AST — see utils/ast-fuzz/README.md for when each entry point applies.
    var v = new AstVerifyVisitor(check_unique = prog._options |> find_arg("_ast_verify_unique") ?as tBool ?? false)
    make_visitor(*v) $(adapter) {
        visit_module(prog, adapter, mod)
    }
    let n = v.error_count
    unsafe {
        delete v
    }
    return n
}

[pre_infer_macro]
class AstVerifyMacro : AstPassMacro {
    //! Runs the AST verifier over each non-builtin module. Active whenever this
    //! module is loaded — via ``--ast-verify`` (force-include) or a source
    //! ``require daslib/ast_verify``.
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        if (mod == null || mod.moduleFlags.builtIn) {
            return true
        }
        verify_module(prog, mod)
        return true
    }
}

def public verify_function(var fn : FunctionPtr) : int {
    //! Verifies one function IN PLACE, signature included. For a macro that BUILDS a
    //! function: add_function mangles the signature at once, so a malformed result or
    //! argument type crashes there, before [[verify_module]] could see it.
    var v = new AstVerifyVisitor()
    make_visitor(*v) $(adapter) {
        visit(fn, adapter)
    }
    let n = v.error_count
    unsafe {
        delete v
    }
    return n
}

def public verify_expression(var expr : ExpressionPtr) : int {
    //! Verifies one expression SUBTREE, returning the violation count. For macros
    //! that build and return a tree (for-loop / call / annotation / reader), which
    //! run inside inference where [[verify_module]] cannot see the result yet.
    var v = new AstVerifyVisitor()
    make_visitor(*v) $(adapter) {
        visit(expr, adapter)
    }
    let n = v.error_count
    unsafe {
        delete v
    }
    return n
}

def public verify_program(prog : ProgramPtr) : int {
    //! Verifies every module of the program, returning the total violation
    //! count. Convenience wrapper over [[verify_module]] for callers that want
    //! to sweep the whole program.
    var total = 0
    program_for_each_module(prog) $(mod) {
        total += verify_module(prog, mod)
    }
    return total
}
