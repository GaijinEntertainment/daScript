options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options _comment_hygiene = true

module linq_fold_xml shared public

//! linq_fold XML source adapter: ``XmlAdapter`` + ``extract_xml_source``. Lets ``_fold`` over a
//! ``from_xml_node(node[, name], type<Row>)`` chain emit an inlined DOM child-element walk (mirroring
//! ``PUGIXML_boost``'s generator body) instead of driving the generator + an intermediate array.
//! Optional companion to ``daslib/linq_fold`` — pulled in via ``require ?pugixml`` and gated by
//! ``static_if (typeinfo builtin_module_exists(pugixml))``. See daslib/linq_fold.md.

require pugixml public
require pugixml/PUGIXML_boost
require daslib/ast_boost
require daslib/ast_match
require daslib/templates_boost
require strings
require daslib/linq_fold_common public

// ===== Field-pruning (pass 2b) =====
// Walk the chain body to learn which Row fields it reads, so the per-element materialization can read
// only those attributes — skipping `clone_string` for unread string fields (zero allocs when no kept
// field is a string). Mirrors linq_fold_decs's DecsTupUsageScanner / DecsTupFieldFlattener, specialized
// to a single struct bind (no iter-var reverse-map; no whole-tuple rewriter — a whole-`it` escape just
// routes to full build_xml_row).

class private XmlRowUsageScanner : AstVisitor {
    bindName      : string
    usedFields    : table<string>
    allFieldsUsed : bool = false
    inTargetField : bool = false
    def XmlRowUsageScanner(n : string) {
        bindName = n
    }
    def override preVisitExprField(expr : ExprField?) : void {
        var v = expr.value
        if (v is ExprRef2Value) {
            v = (v as ExprRef2Value).subexpr
        }
        if (v is ExprVar && (v as ExprVar).name == bindName) {
            usedFields |> insert(string(expr.name))
            inTargetField = true
        }
    }
    def override visitExprField(var expr : ExprField?) : ExpressionPtr {
        inTargetField = false
        return expr
    }
    def override preVisitExprVar(expr : ExprVar?) : void {
        // A whole-var `it` ref (not the inner value of a matched field access) means the whole row
        // escapes (to_array push_clone, identity select, pass-to-user-fn) — fall back to full materialization.
        if (expr.name == bindName && !inTargetField) {
            allFieldsUsed = true
        }
    }
}

[macro_function]
def private collect_xml_row_usage(var body : Expression?; bindName : string) : tuple<bool; array<string>> {
    var allUsed = false
    var used : array<string>
    if (body == null) return (allUsed, used)
    var sc = new XmlRowUsageScanner(bindName)
    make_visitor(*sc) $(astVisitorAdapter) {
        visit_expression(body, astVisitorAdapter)
    }
    allUsed = sc.allFieldsUsed
    for (k in keys(sc.usedFields)) {
        used |> push(k)
    }
    unsafe {
        delete sc
    }
    return (allUsed, used)
}

class private XmlRowFieldFlattener : AstVisitor {
    bindName     : string
    fieldToLocal : table<string; string>
    def XmlRowFieldFlattener(b : string; var f : table<string; string>) {
        bindName = b
        fieldToLocal <- f
    }
    def override visitExprField(var expr : ExprField?) : ExpressionPtr {
        var v = expr.value
        if (v is ExprRef2Value) {
            v = (v as ExprRef2Value).subexpr
        }
        if (v is ExprVar && (v as ExprVar).name == bindName) {
            let fname = string(expr.name)
            if (key_exists(fieldToLocal, fname)) return new ExprVar(at = expr.at, name := fieldToLocal[fname])
        }
        return expr
    }
}

[macro_function]
def private flatten_xml_row_to_locals(var body : Expression?; bindName : string; var fieldToLocal : table<string; string>) : void {
    if (body == null) return
    var fl = new XmlRowFieldFlattener(bindName, fieldToLocal)
    make_visitor(*fl) $(astVisitorAdapter) {
        visit_expression(body, astVisitorAdapter)
    }
    unsafe {
        delete fl
    }
}

// Fills `prelude` (the hoisted default-row decl, flatten path only) + `inner` (per-element, ends with
// `body`): allUsed -> full build_xml_row; no field refs -> body only; pruned -> one `let xf_<f>` read
// per referenced field with body's `it.<f>` rewritten to it (struct dropped). See linq_fold.md.
[macro_function]
def private build_xml_materializer(var body : Expression?; bindName : string; declT : TypeDeclPtr;
                                   defName, chName : string; at : LineInfo;
                                   var prelude : array<Expression?>; var inner : array<Expression?>) {
    var (allUsed, usedFields) = collect_xml_row_usage(body, bindName)
    if (allUsed) {
        inner |> push <| qmacro_expr() {
            var $i(bindName) = default<$t(declT)>
        }
        inner |> push <| qmacro_expr() {
            build_xml_row($i(chName), $i(bindName))
        }
        inner |> push(body)
        return
    }
    if (empty(usedFields)) {
        inner |> push(body)
        return
    }
    sort(usedFields)   // deterministic emission (AOT hash / test stability)
    var fieldToLocal <- {for (f in usedFields); f => qn("xf_{f}", at)}
    prelude |> push <| qmacro_expr() {
        let $i(defName) = default<$t(declT)>
    }
    inner |> reserve(length(inner) + length(usedFields) + 1)
    for (f in usedFields) {
        let local = fieldToLocal[f]
        inner |> push <| qmacro_expr() {
            let $i(local) = read_xml_field($i(chName), $v(f), $i(defName).$f(f))
        }
    }
    flatten_xml_row_to_locals(body, bindName, fieldToLocal)
    inner |> push(body)
}

// Single flat loop, like ArrayAdapter (unlike decs's 2-level archetype walk). nameArg null = the 2-arg
// `from_xml_node(node, type<Row>)` overload (walk all child elements); non-null = the 3-arg named overload
// (walk child elements whose tag matches the name).
class XmlAdapter : SourceAdapter {
    nodeExpr : Expression?
    srcName  : string
    elemType : TypeDeclPtr
    nameArg  : Expression?
    def override kind() : AdapterKind {
        return AdapterKind.k_xml
    }
    def override arraySrcName() : string {
        return srcName
    }
    def override bind_name(at : LineInfo) : string {
        return qn("it", at)
    }
    def override element_type() : TypeDeclPtr {
        return clone_type(elemType)
    }
    def override wrap_source_loop(var body : Expression?; at : LineInfo) : Expression? {
        let chName = qn("xch", at)
        let curName = qn("xcur", at)
        let defName = qn("xdef", at)
        let sn = srcName
        var declT = strip_const_ref(clone_type(elemType))
        var prelude : array<Expression?>
        var inner : array<Expression?>
        // `xch` advances to the next sibling BEFORE the body runs, so a `continue` inside the body
        // (emitted by skip / skip_while) lands on the advanced cursor, not the same element; the body
        // reads attributes from the captured current element `xcur`.
        build_xml_materializer(body, bind_name(at), declT, defName, curName, at, prelude, inner)
        var loop : Expression?
        if (nameArg == null) {
            loop = qmacro_block() {
                var $i(chName) = $i(sn).first_child
                while ($i(chName).ok) {
                    let $i(curName) = $i(chName)
                    $i(chName) = $i(chName).next_sibling
                    if ($i(curName)._type == xml_node_type.node_element) {
                        $b(inner)
                    }
                }
            }
        } else {
            let nmName = qn("xname", at)
            loop = qmacro_block() {
                let $i(nmName) = $e(nameArg)
                var $i(chName) = child($i(sn), $i(nmName))
                while ($i(chName).ok) {
                    let $i(curName) = $i(chName)
                    $i(chName) = next_sibling($i(chName), $i(nmName))
                    $b(inner)
                }
            }
        }
        if (empty(prelude)) return loop
        // Flatten path: hoist `let xdef` before the walk as a FLAT sibling of the loop statements
        // (a `$b(prelude)` splice would nest it in its own scope, hiding it from the per-element reads).
        var outBlk = new ExprBlock(at = at)
        for (s in prelude) {
            outBlk.list |> push(s)
        }
        for (s in (loop as ExprBlock).list) {
            outBlk.list |> push(s)
        }
        return outBlk
    }
    def override wrap_invoke(var stmts : array<Expression?>; retType : TypeDeclPtr; wrapIter : bool; at : LineInfo) : Expression? {
        var nodeType = strip_const_ref(clone_type(nodeExpr._type))
        var nodeClone = clone_expression(nodeExpr)
        nodeClone.genFlags.alwaysSafe = true
        let sn = srcName
        var emission : Expression?
        if (retType != null) {
            emission = qmacro(invoke($($i(sn) : $t(nodeType)) : $t(retType) {
                $b(stmts)
            }, $e(nodeClone)))
        } else {
            emission = qmacro(invoke($($i(sn) : $t(nodeType)) {
                $b(stmts)
            }, $e(nodeClone)))
        }
        emission = finalize_invoke(emission, at)
        if (wrapIter) {
            emission = qmacro($e(emission).to_sequence_move())
            emission.force_generated(true)
        }
        return emission
    }
}

// Recognize a `from_xml_node(node[, name], type<Row>)` chain top. Returns the call (caller reads
// arguments[0] = node, arguments[1] = name for the 3-arg form, `_type.firstType` = Row); null otherwise.
[macro_function]
def extract_xml_source(var top : Expression?) : ExprCall? {
    if (top == null || !(top is ExprCall)) return null
    var c = top as ExprCall
    if (get_call_short_name(c) != "from_xml_node" || c._type == null || !c._type.isIterator || c._type.firstType == null) return null
    let na = length(c.arguments)
    if (na != 2 && na != 3) return null
    return c
}
