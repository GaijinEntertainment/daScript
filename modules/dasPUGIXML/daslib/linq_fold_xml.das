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
        let bindName = qn("it", at)
        let chName = qn("xch", at)
        let nxName = qn("xnx", at)
        let sn = srcName
        var declT = strip_const_ref(clone_type(elemType))
        if (nameArg == null) {
            return <- qmacro_block() {
                var $i(chName) = $i(sn).first_child
                while ($i(chName).ok) {
                    let $i(nxName) = $i(chName).next_sibling
                    if ($i(chName)._type == xml_node_type.node_element) {
                        var $i(bindName) = default<$t(declT)>
                        build_xml_row($i(chName), $i(bindName))
                        $e(body)
                    }
                    $i(chName) = $i(nxName)
                }
            }
        }
        let nmName = qn("xname", at)
        return <- qmacro_block() {
            let $i(nmName) = $e(nameArg)
            var $i(chName) = child($i(sn), $i(nmName))
            while ($i(chName).ok) {
                let $i(nxName) = next_sibling($i(chName), $i(nmName))
                var $i(bindName) = default<$t(declT)>
                build_xml_row($i(chName), $i(bindName))
                $e(body)
                $i(chName) = $i(nxName)
            }
        }
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
