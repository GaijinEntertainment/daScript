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
//!
//! Two modes. Plain ``--ast-verify`` (or the source ``require``) is the debugging tool: it
//! re-checks before EVERY inference pass, so a break is caught on the pass right after it
//! happens, and each post-infer firing sweeps every module. ``--ast-verify-batch`` is the CI
//! gate: no pre-infer checks, and each post-infer firing checks the module being compiled
//! (every other module once per process) - the tree handed to lint/fold/codegen is valid,
//! at a fraction of the cost.

require daslib/ast
require daslib/ast_boost
require strings

def private batch_mode() : bool {
    let args <- get_command_line_arguments()
    return has_value(args, "--ast-verify-batch")
}

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

def private compact_nulls(var list : auto(LT)) : int {
    var removed = 0
    var i = 0
    while (i < length(list)) {
        if (list[i] == null) {
            list |> erase(i)
            removed++
        } else {
            i++
        }
    }
    return removed
}

// the grammar admits only these in a unary slot (ds2_parser.ypp), so any other name
// leaves ExprOp1.func null, which SimulateVisitor::visit(ExprOp1*) dereferences
let private UNARY_OPS <- { "-", "+", "~", "!", "++", "--", "+++", "---" }

// inc / dec need an l-value, so on a literal they can never resolve; infer reports that
// from source, but a macro-built one reaches the string-builder fold with func still null
let private LVALUE_OPS <- { "++", "--", "+++", "---" }

// a type registered from C++ reports at the shared <c++ binding> location
def private is_cpp_binding_at(at : LineInfo) : bool {
    return at.fileInfo != null && starts_with(string(at.fileInfo.name), "<c++ binding")
}

def private is_enum_type(bt : Type) : bool {
    return (bt == Type.tEnumeration || bt == Type.tEnumeration8
        || bt == Type.tEnumeration16 || bt == Type.tEnumeration64)
}

// TypeDecl::dimConst sentinel (ast_typedecl.h); TypeDecl::visit walks fixedDimExpr for it
let private DIM_CONST = -2

// A node reached twice has two parents, which breaks the unique-ownership rule every
// clone_* call exists to keep. Both verifier passes record into one node set: the
// infer-time one to report the alias, the gc-root one to know what it reached.
typedef private DupReport = block<(msg : string; at : LineInfo) : void>

// false when the node was already recorded, i.e. a walk must not descend into it again
def private note_node(var seen : table<void?>; node : void?; what : string; at : LineInfo;
                      report : DupReport) : bool {
    if (!key_exists(seen, node)) {
        seen |> insert(node)
        return true
    }
    invoke(report, "{what} is aliased - reachable from two parents (missing clone)", at)
    return false
}

// The walk descends into firstType / secondType / argTypes itself, so those reach
// preVisitTypeDecl; only a slot it never enters - an expression's inferred type, or one
// of the type fields the expression visits skip - has to be recorded as a whole tree.
def private note_type_tree(var seen : table<void?>; var typ : TypeDeclPtr; what : string;
                           report : DupReport) : void {
    if (typ == null
            || !note_node(seen, typ, "TypeDecl {describe(typ)} ({what})", typ.at, report)) {
        return
    }
    note_type_tree(seen, typ.firstType, "{what}.firstType", report)
    note_type_tree(seen, typ.secondType, "{what}.secondType", report)
    for (i in range(length(typ.argTypes))) {
        note_type_tree(seen, typ.argTypes[i], "{what}.argTypes[{i}]", report)
    }
    note_type_payload(seen, typ, what, report)
}

// TypeDecl::visit walks the payload only for a typeDecl / typeMacro node, and
// fixedDimExpr only for a dimConst array - the rest stay unvisited
def private note_type_payload(var seen : table<void?>; var typ : TypeDeclPtr; what : string;
                              report : DupReport) : void {
    if (typ.baseType != Type.typeDecl && typ.baseType != Type.typeMacro) {
        for (i in range(length(typ.typeMacroExpr))) {
            note_expr_node(seen, typ.typeMacroExpr[i], "{what}.typeMacroExpr[{i}]", report)
        }
    }
    if (typ.baseType == Type.tFixedArray && typ.fixedDim != DIM_CONST) {
        note_expr_node(seen, typ.fixedDimExpr, "{what}.fixedDimExpr", report)
    }
}

def private note_expr_node(var seen : table<void?>; var expr : ExpressionPtr; what : string;
                           report : DupReport) : void {
    if (expr == null
            || !note_node(seen, expr, "{expr.__rtti} ({what})", expr.at, report)) {
        return
    }
    note_type_tree(seen, expr._type, "{what}.type", report)
}




class private AstVerifyVisitor : AstVisitor {
    error_count : int = 0
    @do_not_delete current_fn : Function?
    @do_not_delete fn_stack : array<Function?>
    seen : table<void?>
    aliased_types : table<void?>
    check_unique : bool = true
    // slots the walk never reaches on its own (an expression's inferred type, a macro
    // payload) are visited from here, which needs the adapter the walk runs on
    @do_not_delete adapter : VisitorAdapter?
    // A node reachable from itself makes every walk — inference, codegen, ours —
    // recurse until the stack dies, so the cycle is cut, not only reported.
    path : table<void?>
    tpath : table<void?>
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
        // a detached synthetic tree has no location in it at all - do not trail an empty " at "
        let loc = describe(at)
        to_log(LOG_ERROR, empty(loc) ? "AST verify: {msg}\n" : "AST verify: {msg} at {loc}\n")
        compiling_program() |> macro_error(at, "AST verify: {msg}")
    }

    def track_node(node : void?; what : string; at : LineInfo) : bool {
        return note_node(seen, node, what, at) $(msg, msg_at) {
            report(msg_at, msg)
        }
    }

    def track_type_tree(var typ : TypeDeclPtr; what : string) : void {
        if (!check_unique) {
            return
        }
        note_type_tree(seen, typ, what) $(msg, msg_at) {
            report(msg_at, msg)
        }
    }

    def track_type_payload(var typ : TypeDeclPtr; what : string) : void {
        if (!check_unique) {
            return
        }
        note_type_payload(seen, typ, what) $(msg, msg_at) {
            report(msg_at, msg)
        }
    }

    // a slot the walk skips still has to be walked, or every node under it reads as
    // unreachable; visiting it through the adapter covers its children too
    def walk(var expr : ExpressionPtr) : void {
        if (expr != null && adapter != null) {
            visit(expr, adapter)
        }
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
        if (is_ancestor(slot)) {
            report(at, "{what} points at its own ancestor ({slot.__rtti}) - a cycle")
            return repair_kind(want, at)
        }
        if (slot.__rtti != want) {
            report(at, "{what} is {slot.__rtti}, but inference static_casts it to {want}")
            return repair_kind(want, at)
        }
        return slot
    }

    def cut_type_cycle(var slot : TypeDeclPtr&; what : string; at : LineInfo) : void {
        if (slot == null || !key_exists(tpath, slot)) {
            return
        }
        report(at, "{what} points at its own ancestor - a cycle")
        slot = new TypeDecl(at = at, baseType = Type.tInt)
    }

    def verify_type(var typ : TypeDeclPtr; what : string;
                    at : LineInfo) : void {
        if (typ == null) {
            return
        }
        cut_type_cycle(typ.firstType, "{what}: firstType", at)
        cut_type_cycle(typ.secondType, "{what}: secondType", at)
        if (typ.baseType == Type.tArray || typ.baseType == Type.tFixedArray
                || typ.baseType == Type.tIterator) {
            need_type(typ.firstType, "{what}: {typ.baseType} has no element type (firstType is null)", at)
        } elif (typ.baseType == Type.tTable) {
            need_type(typ.firstType, "{what}: table has no key type (firstType is null)", at)
            need_type(typ.secondType, "{what}: table has no value type (secondType is null)", at)
        }
        for (i in range(length(typ.argTypes))) {
            cut_type_cycle(typ.argTypes[i], "{what}: argTypes[{i}]", at)
            need_type(typ.argTypes[i], "{what}: {typ.baseType} argTypes[{i}] is null", at)
        }
        if (typ.baseType == Type.tStructure && typ.structType == null) {
            report(at, "{what}: tStructure with a null structType")
            typ.baseType = Type.tInt
        } elif (is_enum_type(typ.baseType) && typ.enumType == null) {
            report(at, "{what}: {typ.baseType} with a null enumType")
            typ.baseType = Type.tInt
        } elif (is_enum_type(typ.baseType) && typ.enumType._module == null) {
            // infer looks the enumerator up through its module
            report(at, "{what}: enumeration '{typ.enumType.name}' belongs to no module")
            typ.baseType = Type.tInt
            typ.enumType = null
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
        if (expr == null) {
            return
        }
        // folding sets this flag only alongside a type, and acts on the flag alone
        if (expr.flags.constexpression && expr._type == null) {
            report(expr.at, "{expr.__rtti} is marked constexpression but has no type")
            expr.flags.constexpression = false
        }
        // ExprConst dispatches preVisit twice (base, then derived) — not an alias
        let revisit = key_exists(path, expr)
        path |> insert(expr)
        if (!check_unique || revisit || !track_node(expr, "node {expr.__rtti}", expr.at)) {
            return
        }
        track_type_tree(expr._type, "{expr.__rtti}.type")
    }

    def leave(var expr : ExpressionPtr) : ExpressionPtr {
        path |> erase(expr)
        return expr
    }

    def override visitExpression(var expr : ExpressionPtr) : ExpressionPtr {
        return leave(expr)
    }

    def is_ancestor(slot : ExpressionPtr) : bool {
        return slot != null && key_exists(path, slot)
    }

    // Variable / MakeStruct entries are dereferenced by the visitor itself before any
    // per-entry hook runs, so a null one has to be dropped in preVisit, not reported.
    def dropped(n : int; what : string; at : LineInfo) : void {
        if (n > 0) {
            report(at, "{what} holds {n} null entry(s), which the visitor dereferences")
        }
    }

    def cycle_slot(var slot : ExpressionPtr&; what : string; at : LineInfo) : void {
        if (!is_ancestor(slot)) {
            return
        }
        report(at, "{what} points at its own ancestor ({slot.__rtti}) - a cycle")
        slot = placeholder(at)
    }

    def cut_list_cycles(var list : dasvector`ptr`Expression; at : LineInfo; what : string) : void {
        for (i in range(length(list))) {
            cycle_slot(list[i], "{what}[{i}]", at)
        }
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
            cycle_slot(slot, what, at)
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
        if (typ != null) {
            tpath |> insert(typ)
            if (check_unique) {
                if (!track_node(typ, "TypeDecl {describe(typ)}", typ.at)) {
                    aliased_types |> insert(typ)
                }
                track_type_payload(typ, "TypeDecl")
            }
        }
        verify_type(typ, "TypeDecl", typ != null ? typ.at : LineInfo())
    }

    // only the outermost slot of each declared type reaches visit (nested firstType /
    // argTypes never do), so that is where an alias can be unshared
    def override visitTypeDecl(var typ : TypeDeclPtr) : TypeDeclPtr {
        if (typ == null) {
            return typ
        }
        tpath |> erase(typ)
        if (key_exists(aliased_types, typ)) {
            aliased_types |> erase(typ)
            return clone_type(typ)
        }
        return typ
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
        if (!key_exists(UNARY_OPS, string(expr.op))) {
            report(expr.at, "ExprOp1 has op '{expr.op}', which is not a unary operator")
            expr.op := "-"
        }
        if (key_exists(LVALUE_OPS, string(expr.op)) && expr.subexpr != null
                && starts_with(expr.subexpr.__rtti, "ExprConst")) {
            report(expr.at, "ExprOp1 '{expr.op}' needs an l-value, but its subexpr is {expr.subexpr.__rtti}")
            expr.op := "+"
        }
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
        // ExprFor::visit walks sources and body, never the tags
        for (i in range(length(expr.iteratorsTags))) {
            walk(expr.iteratorsTags[i])
        }
    }

    // a quote body is AST data, not code, so the walk stops at it by default - it still
    // holds nodes that must be uniquely owned
    def override canVisitQuoteSubexpression(_expr : ExprQuote?) : bool {
        return true
    }

    // finalList only: a null in `list` compiles clean, so it is not a crash shape
    def override preVisitExprBlock(var expr : ExprBlock?) : void {
        dropped(compact_nulls(expr.arguments), "ExprBlock.arguments", expr.at)
        // ExprBlock::visit walks `arguments` only when isClosure; inference does not
        if (!expr.blockFlags.isClosure && !empty(expr.arguments)) {
            report(expr.at, "ExprBlock has arguments but is not marked isClosure")
            expr.blockFlags.isClosure = true
        }
        if (expr.blockFlags.isClosure) {
            closure_depth++
        }
        verify_elements(expr.finalList, expr.at, "ExprBlock.finalList")
        cut_list_cycles(expr.list, expr.at, "ExprBlock.list")
        // ExprBlock::visit walks returnType only for a closure
        if (!expr.blockFlags.isClosure) {
            track_type_tree(expr.returnType, "ExprBlock.returnType")
        }
    }

    def override visitExprBlock(var expr : ExprBlock?) : ExpressionPtr {
        if (expr.blockFlags.isClosure && closure_depth > 0) {
            closure_depth--
        }
        return leave(expr)
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
        track_type_tree(expr.ascType, "ExprAscend.ascType")
    }

    def override preVisitExprMakeTuple(var expr : ExprMakeTuple?) : void {
        verify_type(expr.recordType, "ExprMakeTuple.recordType", expr.at)
        verify_type(expr.makeType, "ExprMakeTuple.makeType", expr.at)
        // makeType is left alone: ExprMakeLocal::visit walks it, so it is tracked already
        track_type_tree(expr.recordType, "ExprMakeTuple.recordType")
    }

    def override preVisitExprConstBitfield(var expr : ExprConstBitfield?) : void {
        track_type_tree(expr.bitfieldType, "ExprConstBitfield.bitfieldType")
    }

    // ExprAssume::visit walks assumeType's children but never preVisits the node itself
    def override preVisitExprAssume(var expr : ExprAssume?) : void {
        if (expr.assumeType != null && check_unique) {
            let _dup = track_node(expr.assumeType, "TypeDecl {describe(expr.assumeType)} (ExprAssume.assumeType)",
                expr.assumeType.at)
        }
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

    def override preVisitExprMakeVariant(var expr : ExprMakeVariant?) : void {
        dropped(compact_nulls(expr.variants), "ExprMakeVariant.variants", expr.at)
    }

    def override preVisitExprLet(var expr : ExprLet?) : void {
        dropped(compact_nulls(expr.variables), "ExprLet.variables", expr.at)
    }

    def override preVisitExprMakeStruct(var expr : ExprMakeStruct?) : void {
        dropped(compact_nulls(expr.structs), "ExprMakeStruct.structs", expr.at)
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

    def payload_slot(var expr : ExpressionPtr; present : bool; what : string) : ExpressionPtr {
        path |> erase(expr)
        if (present) {
            return expr
        }
        // infer dereferences the payload unguarded, and there is nothing to put back
        report(expr.at, "{expr.__rtti} has a null {what}")
        return placeholder(expr.at)
    }

    def override visitExprReader(var expr : ExprReader?) : ExpressionPtr {
        return payload_slot(expr, expr.macro != null, "macro")
    }

    def override visitExprCallMacro(var expr : ExprCallMacro?) : ExpressionPtr {
        return payload_slot(expr, expr.macro != null, "macro")
    }

    def override visitExprConstEnumeration(var expr : ExprConstEnumeration?) : ExpressionPtr {
        return payload_slot(expr, expr.enumType != null, "enumType")
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
        return leave(expr)
    }

    def override preVisitFunction(var fun : FunctionPtr) : void {
        fn_stack |> push(current_fn)
        current_fn = fun
        dropped(compact_nulls(fun.arguments), "function '{fun.name}' arguments", fun.at)
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
    //! AST — the repo's ast-fuzz suite README covers when each entry point applies.
    var v = new AstVerifyVisitor(check_unique = prog._options |> find_arg("_ast_verify_unique") ?as tBool ?? true)
    make_visitor(*v) $(adapter) {
        v.adapter = adapter
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
    //! Verifies the module being compiled before every inference pass. Active whenever
    //! this module is loaded — via ``--ast-verify`` (force-include) or a source
    //! ``require daslib/ast_verify``; ``--ast-verify-batch`` turns these pre-infer checks off.
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        if (mod == null || mod.moduleFlags.builtIn) {
            return true
        }
        verify_module(prog, mod)
        return true
    }
    def override canVisitPass(prog : ProgramPtr; mod : Module?; index : int) : bool {
        return !batch_mode()
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

// `node` set means it carries its own `at` and names itself, so the walk stores one pointer per
// node; otherwise `at` and `phrase` are it. Not an Option<T> - LineInfo is no tuple element.
struct private Anchor {
    @do_not_delete node : Expression?
    @safe_when_uninitialized at : LineInfo
    phrase : string
}

// Runs for every node the walk touches - keep it a null test; `usable` does the rest, rarely.
def private located(e : Expression?) : bool {
    return e != null && e.at.fileInfo != null
}

def private usable(e : Expression?) : bool {
    return located(e) && !is_cpp_binding_at(e.at)
}

// Runs only for a node that turns out to have no `at`, so a well formed tree pays nothing. The two
// gates match AstPostInferVerifyVisitor: a location from a region that walk refuses to enter belongs
// to a spelling that is not part of this instantiation, and need_at stamps what it is given.
class private FirstLocatedVisitor : AstVisitor {
    @do_not_delete found : Expression?
    def override preVisitExpression(var expr : ExpressionPtr) : void {
        if (found == null && usable(expr)) {
            found = expr
        }
    }
    def override canVisitWithAliasSubexpression(_expr : ExprAssume?) : bool {
        return false
    }
    def override canVisitStructureFieldInit(st : StructurePtr) : bool {
        return !st.flags.isTemplate
    }
}

def private first_located_inside(var expr : ExpressionPtr) : Expression? {
    var v = new FirstLocatedVisitor()
    make_visitor(*v) $(adapter) {
        visit(expr, adapter)
    }
    var found = v.found
    unsafe {
        delete v
    }
    return found
}

// ===== post-inference verification =====
class private AstPostInferVerifyVisitor : AstVisitor {
    error_count : int = 0
    @do_not_delete current_fn : Function?
    closure_depth : int = 0
    // the last located thing the walk passed - a node, or a site with no expression around it
    @safe_when_uninitialized last_passed : Anchor


    def report(at : LineInfo; msg : string) : void {
        error_count++
        var text = msg
        var anchor = at
        if (at.fileInfo == null) {
            let guess = guess_at(null)
            anchor = guess.at
            if (!empty(guess.phrase)) {
                text = "{msg}; {guess.phrase}"
            }
        }
        let loc = describe(anchor)
        to_log(LOG_ERROR, empty(loc)
            ? "AST verify (post-infer): {text}\n"
            : "AST verify (post-infer): {text} at {loc}\n")
        compiling_program() |> macro_error(anchor, "AST verify (post-infer): {text}")
    }

    // Subtree first: a node begins where its subtree begins, which is the exact line for 91% of
    // nodes against 8% for the function head. What the walk passed last covers a synthetic subtree.
    def guess_at(var expr : ExpressionPtr) : Anchor {
        var inside = expr != null ? first_located_inside(expr) : null
        if (inside != null) {
            return Anchor(at = inside.at, phrase = "first located node inside it is {inside.__rtti}")
        }
        let passed = usable(last_passed.node) ? last_passed.node : null
        return Anchor(at = passed != null ? passed.at : last_passed.at,
            phrase = passed != null ? "preceded by {passed.__rtti}" : last_passed.phrase)
    }

    // A template body is expanded per instantiation and a stub body (dasbind [extern]) is
    // never inferred - InferTypes::canVisitFunction skips both, as does every pass this
    // verifier guards. A generated body (lambda, generator) IS codegen'd, so it is checked.
    def override canVisitFunction(fun : Function?) : bool {
        return !fun.moreFlags.isTemplate && !fun.moreFlags.stub
    }

    def override preVisitFunction(var fun : FunctionPtr) : void {
        current_fn = fun
        // walk state from the previously walked function is no vicinity for this one
        last_passed = Anchor()
        need_at(fun.at, "function '{fun.name}'")
        need_slot(fun.result, "function '{fun.name}' result type", fun.at)
        if (fun.body == null) {
            report(fun.at, "function '{fun.name}' has no body after inference")
            fun.body = placeholder_block(fun.at)
        }
    }

    def override visitExprBlock(var expr : ExprBlock?) : ExpressionPtr {
        if (expr.blockFlags.isClosure) {
            closure_depth--
        }
        return expr
    }

    // an `assume` alias is substituted at its use sites and never inferred, so nothing
    // under it is typed - daslib/quote.das and typemacro_boost.das rely on that
    def override canVisitWithAliasSubexpression(_expr : ExprAssume?) : bool {
        return false
    }

    // a template structure's field initializers stay AST data until it is instantiated
    def override canVisitStructureFieldInit(st : StructurePtr) : bool {
        return !st.flags.isTemplate
    }

    // a template structure is never inferred, only its instances are: its method fields keep their
    // pre-infer `auto` results (no type, no location) by design, so the post-infer walk skips it
    def override canVisitStructure(st : Structure?) : bool {
        return !st.flags.isTemplate
    }

    // Inference is done, so an operand with no type never got one, and lint / folding / codegen all
    // dereference it unguarded. An empty `what` names it after the node, only if it reports.
    def need_typed(var slot : ExpressionPtr; what : string; at : LineInfo) : void {
        // a statement is not a value - inference never gives it a type
        if (slot == null || slot._type != null || slot.genFlags.generated || is_stmt_node(slot)) {
            return
        }
        let named = empty(what) ? "{slot.__rtti}" : what
        report(at, "{named} ({slot.__rtti}) has no type after inference")
        slot._type = new TypeDecl(at = at, baseType = Type.tInt)
    }

    // ast_lint.cpp reports this for ExprCall; ast_unused.cpp getSideEffects segfaults
    def need_func(fn : Function?; what : string; at : LineInfo) : void {
        if (fn != null) {
            return
        }
        report(at, "{what} has an unresolved func after inference")
    }

    // A node with no location makes every later diagnostic over it point at nothing. No kind is
    // exempt: a value from a C++ binding gets the call site stamped by infer. The guess is stamped
    // in, so the passes after this one have a place too.
    def need_at(var at : LineInfo&; what : string; var expr : ExpressionPtr = null; track : bool = true) : void {
        if (at.fileInfo != null) {
            if (track && !is_cpp_binding_at(at)) {
                // a site has no node to name it, and clears the one the walk last stored
                last_passed = Anchor(at = at, phrase = "closest location the walk passed is {what}")
            }
            return
        }
        let ctx = current_fn != null ? " in '{current_fn.name}'" : ""
        let guess = guess_at(expr)
        let tail = empty(guess.phrase) ? guess.phrase : "; {guess.phrase}"
        report(guess.at, "{what} has no source location (at is unset){ctx}{tail}")
        if (guess.at.fileInfo != null) {
            at = guess.at
        }
    }

    // TypeDecl::visit preVisits firstType / secondType / argTypes, so one hook covers the tree. Not
    // tracked: a type's `at` is its declaration site, often another module, not this use.
    def need_type_at(var typ : TypeDeclPtr; what : string) : void {
        if (typ != null) {
            need_at(typ.at, "{what} {describe(typ)}", track = false)
        }
    }

    // `expr` is never null - Expression::visit hands its own `this` to every hook. Stamping the
    // guess is also what keeps ExprConst, pre-visited twice for one node, from reporting twice.
    def override preVisitExpression(var expr : ExpressionPtr) : void {
        if (located(expr)) {
            last_passed.node = expr
        } else {
            need_at(expr.at, "{expr.__rtti}", expr)
        }
        need_typed(expr, "", expr.at)
    }

    // Inference is done, so a null in a slot codegen dereferences is never filled in later.
    // A repaired slot keeps the remaining passes alive; the report already failed the compile.
    def need_slot(var slot : TypeDeclPtr&; what : string; at : LineInfo) : void {
        if (slot != null) {
            return
        }
        report(at, "{what} is null after inference")
        slot = new TypeDecl(at = at, baseType = Type.tInt)
    }

    def verify_type_slots(var typ : TypeDeclPtr) : void {
        if (is_cpp_binding_at(typ.at)) {
            return
        }
        let bt = typ.baseType
        let what = "TypeDecl {describe(typ)}"
        // only these lack an untyped spelling; bare Func / pointer / iterator carry no firstType
        if (bt == Type.tArray || bt == Type.tFixedArray) {
            need_slot(typ.firstType, "{what}: element type (firstType)", typ.at)
        } elif (bt == Type.tTable) {
            need_slot(typ.firstType, "{what}: key type (firstType)", typ.at)
            need_slot(typ.secondType, "{what}: value type (secondType)", typ.at)
        } elif (bt == Type.tDistinct) {
            need_slot(typ.firstType, "{what}: underlying type (firstType)", typ.at)
        }
        for (i in range(length(typ.argTypes))) {
            need_slot(typ.argTypes[i], "{what}: argTypes[{i}]", typ.at)
        }
        if (bt == Type.tStructure && typ.structType == null) {
            report(typ.at, "{what}: tStructure with a null structType")
        } elif (is_enum_type(bt) && typ.enumType == null) {
            report(typ.at, "{what}: {bt} with a null enumType")
        } elif ((bt == Type.tHandle || bt == Type.tDistinct) && typ.annotation == null) {
            report(typ.at, "{what}: {bt} with a null annotation")
        }
    }

    def override preVisitTypeDecl(var typ : TypeDeclPtr) : void {
        need_type_at(typ, "TypeDecl")
        if (typ != null) {
            verify_type_slots(typ)
        }
    }

    def override preVisitAlias(var typ : TypeDeclPtr; name : das_string) : void {
        need_type_at(typ, "alias '{name}'")
    }

    def override preVisitFunctionArgument(var fun : FunctionPtr; var arg : VariablePtr; _last : bool) : void {
        need_at(arg.at, "function '{fun.name}' argument '{arg.name}'")
        need_slot(arg._type, "function '{fun.name}' argument '{arg.name}' type", arg.at)
    }

    def override preVisitExprLetVariable(var _expr : ExprLet?; var arg : VariablePtr; _last : bool) : void {
        need_at(arg.at, "let variable '{arg.name}'")
        need_slot(arg._type, "let variable '{arg.name}' type", arg.at)
    }

    def override preVisitExprBlockArgument(var _blk : ExprBlock?; var arg : VariablePtr; _last : bool) : void {
        need_at(arg.at, "block argument '{arg.name}'")
        need_slot(arg._type, "block argument '{arg.name}' type", arg.at)
    }

    def override preVisitGlobalLetVariable(var arg : VariablePtr; _last : bool) : void {
        need_at(arg.at, "global '{arg.name}'")
        need_slot(arg._type, "global '{arg.name}' type", arg.at)
    }

    def override preVisitStructure(var str : StructurePtr) : void {
        need_at(str.at, "structure '{str.name}'")
    }

    def override preVisitStructureField(var str : StructurePtr; var decl : FieldDeclaration; _last : bool) : void {
        need_at(decl.at, "field '{str.name}.{decl.name}'")
        need_slot(decl._type, "field '{str.name}.{decl.name}' type", decl.at)
    }

    def override preVisitEnumeration(var enu : EnumerationPtr) : void {
        need_at(enu.at, "enumeration '{enu.name}'")
    }

    def override preVisitExprMakeStructField(var _expr : ExprMakeStruct?; _index : int; var decl : MakeFieldDeclPtr; _last : bool) : void {
        need_at(decl.at, "make-struct field '{decl.name}'")
    }

    def override preVisitExprMakeVariantField(var _expr : ExprMakeVariant?; _index : int; var decl : MakeFieldDeclPtr; _last : bool) : void {
        need_at(decl.at, "make-variant field '{decl.name}'")
    }

    def override preVisitExprNamedCallArgument(var _expr : ExprNamedCall?; var arg : MakeFieldDeclPtr; _last : bool) : void {
        need_at(arg.at, "named-call argument '{arg.name}'")
    }

    def override visitExprVar(var expr : ExprVar?) : ExpressionPtr {
        need_typed(expr, "ExprVar '{expr.name}'", expr.at)
        return expr
    }

    def override preVisitExprCall(var expr : ExprCall?) : void {
        need_func(expr.func, "ExprCall '{expr.name}'", expr.at)
    }

    // ast_unused.cpp reads arguments[i]->type for EVERY ExprLooksLikeCall subclass, and
    // dispatch is on the concrete kind - hence the generic hook, not preVisitExprCall
    def override preVisitExprLooksLikeCallArgument(var expr : ExprLooksLikeCall?; var arg : ExpressionPtr; _last : bool) : void {
        need_typed(arg, "{expr.__rtti} '{expr.name}' argument", expr.at)
    }

    def override preVisitExprAscend(var expr : ExprAscend?) : void {
        need_typed(expr.subexpr, "ExprAscend.subexpr", expr.at)
    }

    def override preVisitExprOp1(var expr : ExprOp1?) : void {
        need_func(expr.func, "ExprOp1 '{expr.op}'", expr.at)
        need_typed(expr.subexpr, "ExprOp1.subexpr", expr.at)
    }

    def override preVisitExprOp2(var expr : ExprOp2?) : void {
        need_func(expr.func, "ExprOp2 '{expr.op}'", expr.at)
        need_typed(expr.left, "ExprOp2.left", expr.at)
        need_typed(expr.right, "ExprOp2.right", expr.at)
    }

    def override preVisitExprOp3(var expr : ExprOp3?) : void {
        need_typed(expr.subexpr, "ExprOp3.subexpr", expr.at)
        need_typed(expr.left, "ExprOp3.left", expr.at)
        need_typed(expr.right, "ExprOp3.right", expr.at)
    }

    // ast_const_folding.cpp reads expr->type->baseType AND expr->value->type->baseType
    def override preVisitExprSwizzle(var expr : ExprSwizzle?) : void {
        need_typed(expr, "ExprSwizzle", expr.at)
        need_typed(expr.value, "ExprSwizzle.value", expr.at)
    }

    def override preVisitExprStringBuilderElement(var expr : ExprStringBuilder?; var el : ExpressionPtr; _last : bool) : void {
        need_typed(el, "ExprStringBuilder element", expr.at)
    }

    def override preVisitExprMakeArrayIndex(var expr : ExprMakeArray?; index : int; var init : ExpressionPtr; _last : bool) : void {
        need_typed(init, "ExprMakeArray.values[{index}]", expr.at)
    }

    // Null-slot rules mirror the pre-infer pass, which asserts the same invariants but skips
    // generated functions - so a null child inside a lambda / generator / class-generated body
    // was checked by nothing. This pass does visit them.
    def need_expr(var slot : ExpressionPtr&; what : string; at : LineInfo) : void {
        if (slot != null) {
            return
        }
        report(at, "{what} is null after inference")
        slot = placeholder(at)
    }

    def need_stmt(var slot : ExpressionPtr&; what : string; at : LineInfo) : void {
        if (slot != null) {
            return
        }
        report(at, "{what} is null after inference")
        slot = placeholder_block(at)
    }

    def need_elements(var list : dasvector`ptr`Expression; what : string; at : LineInfo) : void {
        for (i in range(length(list))) {
            need_expr(list[i], "{what}[{i}]", at)
        }
    }

    def override preVisitExprField(var expr : ExprField?) : void {
        need_expr(expr.value, "ExprField.value (field '{expr.name}')", expr.at)
    }

    def override preVisitExprSafeField(var expr : ExprSafeField?) : void {
        need_expr(expr.value, "ExprSafeField.value (field '{expr.name}')", expr.at)
    }

    def override preVisitExprIsVariant(var expr : ExprIsVariant?) : void {
        need_expr(expr.value, "ExprIsVariant.value (field '{expr.name}')", expr.at)
    }

    def override preVisitExprAsVariant(var expr : ExprAsVariant?) : void {
        need_expr(expr.value, "ExprAsVariant.value (field '{expr.name}')", expr.at)
    }

    def override preVisitExprSafeAsVariant(var expr : ExprSafeAsVariant?) : void {
        need_expr(expr.value, "ExprSafeAsVariant.value (field '{expr.name}')", expr.at)
    }

    def override preVisitExprCopy(var expr : ExprCopy?) : void {
        need_expr(expr.left, "ExprCopy.left", expr.at)
        need_expr(expr.right, "ExprCopy.right", expr.at)
    }

    def override preVisitExprMove(var expr : ExprMove?) : void {
        need_expr(expr.left, "ExprMove.left", expr.at)
        need_expr(expr.right, "ExprMove.right", expr.at)
    }

    def override preVisitExprClone(var expr : ExprClone?) : void {
        need_expr(expr.left, "ExprClone.left", expr.at)
        need_expr(expr.right, "ExprClone.right", expr.at)
    }

    def override preVisitExprRef2Ptr(var expr : ExprRef2Ptr?) : void {
        need_expr(expr.subexpr, "ExprRef2Ptr.subexpr", expr.at)
    }

    def override preVisitExprRef2Value(var expr : ExprRef2Value?) : void {
        need_expr(expr.subexpr, "ExprRef2Value.subexpr", expr.at)
    }

    def override preVisitExprPtr2Ref(var expr : ExprPtr2Ref?) : void {
        need_expr(expr.subexpr, "ExprPtr2Ref.subexpr", expr.at)
    }

    def override preVisitExprYield(var expr : ExprYield?) : void {
        need_expr(expr.subexpr, "ExprYield.subexpr", expr.at)
    }

    def override preVisitExprIs(var expr : ExprIs?) : void {
        need_expr(expr.subexpr, "ExprIs.subexpr", expr.at)
    }

    def override preVisitExprCast(var expr : ExprCast?) : void {
        need_expr(expr.subexpr, "ExprCast.subexpr", expr.at)
    }

    def override preVisitExprAt(var expr : ExprAt?) : void {
        need_expr(expr.subexpr, "ExprAt.subexpr", expr.at)
        need_expr(expr.index, "ExprAt.index", expr.at)
    }

    def override preVisitExprSafeAt(var expr : ExprSafeAt?) : void {
        need_expr(expr.subexpr, "ExprSafeAt.subexpr", expr.at)
        need_expr(expr.index, "ExprSafeAt.index", expr.at)
    }

    def override preVisitExprWhile(var expr : ExprWhile?) : void {
        need_expr(expr.cond, "ExprWhile.cond", expr.at)
        need_stmt(expr.body, "ExprWhile.body", expr.at)
    }

    def override preVisitExprIfThenElse(var expr : ExprIfThenElse?) : void {
        need_expr(expr.cond, "ExprIfThenElse.cond", expr.at)
        need_stmt(expr.if_true, "ExprIfThenElse.if_true", expr.at)
        // if_false is legitimately null - an `if` with no `else`
    }

    def override preVisitExprTryCatch(var expr : ExprTryCatch?) : void {
        need_stmt(expr.try_block, "ExprTryCatch.try_block", expr.at)
        need_stmt(expr.catch_block, "ExprTryCatch.catch_block", expr.at)
    }

    def override preVisitExprWith(var expr : ExprWith?) : void {
        // `with (module x)` has no subject - ast_expressions.h documents with==nullptr
        if (empty(expr.moduleName)) {
            need_expr(expr._with, "ExprWith.with", expr.at)
        }
        need_stmt(expr.body, "ExprWith.body", expr.at)
    }

    def override preVisitExprUnsafe(var expr : ExprUnsafe?) : void {
        need_stmt(expr.body, "ExprUnsafe.body", expr.at)
    }

    def override preVisitExprNullCoalescing(var expr : ExprNullCoalescing?) : void {
        need_expr(expr.subexpr, "ExprNullCoalescing.subexpr", expr.at)
        need_expr(expr.defaultValue, "ExprNullCoalescing.defaultValue", expr.at)
    }

    def override preVisitExprMakeBlock(var expr : ExprMakeBlock?) : void {
        need_stmt(expr._block, "ExprMakeBlock.block", expr.at)
    }

    def override preVisitExprStringBuilder(var expr : ExprStringBuilder?) : void {
        need_elements(expr.elements, "ExprStringBuilder.elements", expr.at)
    }

    def override preVisitExprMakeArray(var expr : ExprMakeArray?) : void {
        need_elements(expr.values, "ExprMakeArray.values", expr.at)
    }

    def override preVisitExprMakeTuple(var expr : ExprMakeTuple?) : void {
        need_elements(expr.values, "ExprMakeTuple.values", expr.at)
    }

    def override preVisitExprFor(var expr : ExprFor?) : void {
        need_elements(expr.sources, "ExprFor.sources", expr.at)
        need_stmt(expr.body, "ExprFor.body", expr.at)
    }

    def override preVisitExprBlock(var expr : ExprBlock?) : void {
        if (expr.blockFlags.isClosure) {
            closure_depth++
        }
        need_elements(expr.list, "ExprBlock.list", expr.at)
        need_elements(expr.finalList, "ExprBlock.finalList", expr.at)
    }

    def check_make_field_value(var decl : MakeFieldDeclPtr; kind : string) : void {
        if (decl != null) {
            need_expr(decl.value, "{kind}.value (field '{decl.name}')", decl.at)
        }
    }

    def override preVisitExprReturn(var expr : ExprReturn?) : void {
        if (expr.subexpr != null || current_fn == null || current_fn.result == null
                || closure_depth > 0) {
            return
        }
        if (current_fn.result.baseType != Type.tVoid) {
            report(expr.at, "ExprReturn.subexpr is null in '{current_fn.name}', which returns non-void")
        }
    }


}

def public verify_program_after_infer(prog : ProgramPtr; thisMod : Module?) : int {
    //! Verifies thisMod plus every other non-builtin module (a callee's tree is dereferenced
    //! too - getSideEffects recurses into CALLED functions). program_for_each_module yields
    //! thisMod as well, so it is walked once here and skipped in the loop.
    var total = verify_module_after_infer(prog, thisMod)
    program_for_each_module(prog) $(mod) {
        if (mod != thisMod && !mod.moduleFlags.builtIn) {
            total += verify_module_after_infer(prog, mod)
        }
    }
    return total
}

// Modules the batch sweep has verified once. Under --ast-verify-batch every module compiled after
// the force-include is verified as thisMod; the ones compiled BEFORE it (the verifier's own
// requires) and modules read from a module cache are not - each is swept once per process.
var private g_batch_swept : table<string>

def private verify_program_after_infer_batch(prog : ProgramPtr; thisMod : Module?) : int {
    var total = verify_module_after_infer(prog, thisMod)
    g_batch_swept |> insert("{thisMod.name}")
    program_for_each_module(prog) $(mod) {
        if (mod != thisMod && !mod.moduleFlags.builtIn && !key_exists(g_batch_swept, "{mod.name}")) {
            g_batch_swept |> insert("{mod.name}")
            total += verify_module_after_infer(prog, mod)
        }
    }
    return total
}

def public verify_module_after_infer(prog : ProgramPtr; mod : Module?) : int {
    //! Verifies one module is ready for codegen, returning the violation count.
    //! Separate from [[verify_module]]: a half-inferred tree legitimately violates
    //! these, so they can only be asserted here.
    var v = new AstPostInferVerifyVisitor()
    make_visitor(*v) $(adapter) {
        visit_module(prog, adapter, mod)
    }
    let n = v.error_count
    unsafe {
        delete v
    }
    return n
}

[post_infer_macro]
class AstPostInferVerifyMacro : AstPassMacro {
    //! Fires before every consumer of the finished tree (each infer leg, lint, every optimizer
    //! round) and walks the module being compiled plus every OTHER module - a source ``require``
    //! leaves the modules required before it unverified. Batch walks each other module once.
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        if (mod == null || mod.moduleFlags.builtIn) {
            return true
        }
        if (batch_mode()) {
            verify_program_after_infer_batch(prog, mod)
        } else {
            verify_program_after_infer(prog, mod)
        }
        return true
    }
}






// ===== gc-root reachability =====

// A gc-tracked node the module owns but no walk of its AST reaches is either orphaned or
// sits in a slot the visitor skips - so any pass driven by a visitor (lint, folding, a
// macro sweep) edits a tree it cannot see in full.
class private AstGcReachVisitor : AstVisitor {
    seen : table<void?>
    @do_not_delete adapter : VisitorAdapter?

    // recording twice is harmless here: only the reached set matters, not who reached it
    def reach_expr(var expr : ExpressionPtr; what : string) : void {
        note_expr_node(seen, expr, what) $(_msg, _at) {
            pass
        }
    }

    def reach_type(var typ : TypeDeclPtr; what : string) : void {
        note_type_tree(seen, typ, what) $(_msg, _at) {
            pass
        }
    }

    def override canVisitQuoteSubexpression(_expr : ExprQuote?) : bool {
        return true
    }

    def override preVisitExpression(var expr : ExpressionPtr) : void {
        reach_expr(expr, expr == null ? "expression" : "{expr.__rtti}")
    }

    def override preVisitTypeDecl(var typ : TypeDeclPtr) : void {
        reach_type(typ, "TypeDecl")
    }

    def override preVisitExprAscend(var expr : ExprAscend?) : void {
        reach_type(expr.ascType, "ExprAscend.ascType")
    }

    def override preVisitExprMakeArray(var expr : ExprMakeArray?) : void {
        reach_type(expr.recordType, "ExprMakeArray.recordType")
    }

    def override preVisitExprMakeTuple(var expr : ExprMakeTuple?) : void {
        reach_type(expr.recordType, "ExprMakeTuple.recordType")
    }

    def override preVisitExprConstBitfield(var expr : ExprConstBitfield?) : void {
        reach_type(expr.bitfieldType, "ExprConstBitfield.bitfieldType")
    }

    def override preVisitExprAssume(var expr : ExprAssume?) : void {
        reach_type(expr.assumeType, "ExprAssume.assumeType")
    }

    def override preVisitExprBlock(var expr : ExprBlock?) : void {
        reach_type(expr.returnType, "ExprBlock.returnType")
    }

    def override preVisitExprFor(var expr : ExprFor?) : void {
        for (i in range(length(expr.iteratorsTags))) {
            if (expr.iteratorsTags[i] != null && adapter != null) {
                visit(expr.iteratorsTags[i], adapter)
            }
        }
    }

    // set while a generic instantiates and left on the node; no visit walks it
    def override preVisitExprLooksLikeCall(var expr : ExprLooksLikeCall?) : void {
        reach_type(expr.aliasSubstitution, "{expr.__rtti}.aliasSubstitution")
    }

    def override preVisitExprReturn(var expr : ExprReturn?) : void {
        reach_type(expr.returnType, "ExprReturn.returnType")
    }
}

// the bound program is gone by post-compile time, so the error goes to `prog` directly
def private report_unreached(var prog : ProgramPtr; what : string; at : LineInfo) : void {
    to_log(LOG_ERROR, "AST verify (gc root): {what} is not reached by the visitor at {describe(at)}\n")
    prog |> macro_error(at, "AST verify (gc root): {what} is not reached by the visitor")
}

def public verify_module_gc_root(var prog : ProgramPtr; mod : Module?) : int {
    //! Reports every gc-tracked node the module owns that a full walk of its AST never
    //! reaches, returning the count. Meaningful only once the module root is collected,
    //! i.e. from a ``[post_compile_macro]``.
    var owned : table<void?>
    for_each_gc_typedecl(mod) $(t) {
        owned |> insert(t)
    }
    for_each_gc_expression(mod) $(e) {
        owned |> insert(e)
    }
    var v = new AstGcReachVisitor()
    // a shared module is promoted to builtin, which makes visitModule skip its functions
    let was_visiting_builtin = prog.visitBuiltinFunctions
    prog.visitBuiltinFunctions = true
    make_visitor(*v) $(adapter) {
        v.adapter = adapter
        visit_module(prog, adapter, mod)
        // visitModule walks generics only on request, and a generic keeps its own untyped
        // tree on the module root
        ast_core::for_each_generic(mod, "") $(fn) {
            visit(fn, adapter)
        }
    }
    prog.visitBuiltinFunctions = was_visiting_builtin
    // `owned` is the state before the walk, so a node this pass allocated is not reported
    var total = 0
    for_each_gc_typedecl(mod) $(t) {
        if (key_exists(owned, t) && !key_exists(v.seen, t)) {
            total++
            report_unreached(prog, "TypeDecl {describe(t)}", t.at)
        }
    }
    for_each_gc_expression(mod) $(e) {
        if (key_exists(owned, e) && !key_exists(v.seen, e)) {
            total++
            report_unreached(prog, "{e.__rtti}", e.at)
        }
    }
    unsafe {
        delete v
    }
    return total
}

[post_compile_macro]
class AstGcRootVerifyMacro : AstPassMacro {
    //! Runs the gc-root check over the module just compiled. Post-compile is the first
    //! point where its root holds only live nodes - before the collect, the garbage
    //! inference left behind is indistinguishable from a node nothing owns.
    def override apply(var prog : ProgramPtr; mod : Module?) : bool {
        // off by default: gc_collect keeps nodes no Visitor walks (see quote.das blacklist)
        if (mod != null && (prog._options |> find_arg("_ast_verify_gc_root") ?as tBool ?? false)) {
            verify_module_gc_root(prog, mod)
        }
        return true
    }
}
