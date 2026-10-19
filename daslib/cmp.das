options gen2
options indenting = 4

module cmp shared public

//! Comparisons: lexicographic orderings from a list of keys.
//!
//! ``[ordered]`` on a structure generates its ``operator <``, so
//! ``sort``, the heap and partial-sort functions of ``sort_boost``,
//! and anything else that compares with ``<`` order it by default.
//!
//! Every field, in declaration order, ascending::
//!
//!     [ordered]
//!     struct Span { from : int; to : int }
//!
//! Only the listed fields, in this order::
//!
//!     [ordered(score = greater, name)]
//!     struct Player { name : string; score : int; tag : string }
//!
//! ``key`` names a function of the structure::
//!
//!     [ordered(key = name_length, name)]
//!     struct Word { name : string }
//!
//! ``cmp_by`` writes a comparator block for one call. Each argument
//! is a key over ``_``, the element; a leading ``-`` sorts that key
//! descending::
//!
//!     sort(players) <| cmp_by(-_.score, _.name)
//!     sort(words) <| cmp_by(length(_.name), _.name)
//!
//! The first key that differs decides; elements equal on every key
//! compare as not less.

require daslib/ast_boost
require daslib/templates_boost
require strings

struct private OrderKey {
    expr : Expression?
    greater : bool
}

//! A field or constant-index chain over a variable reads cheaply
//! twice; any other key is bound once per side before its ``!=``
//! test.
def private key_is_plain(expr : Expression?) : bool {
    if (expr is ExprVar) return true
    if (expr is ExprField) return key_is_plain((expr as ExprField).value)
    if (expr is ExprSafeField) return key_is_plain((expr as ExprSafeField).value)
    if (expr is ExprAt) {
        return key_is_plain((expr as ExprAt).subexpr) && is_expr_const((expr as ExprAt).index)
    }
    return false
}

//! The statements comparing ``cmp_lhs`` with ``cmp_rhs`` over
//! ``keys``: the first key that differs decides.
def private order_statements(keys : array<OrderKey>) : array<Expression?> {
    var body : array<Expression?>
    for (key, i in keys, count()) {
        var lhs = clone_expression(key.expr)
        lhs = apply_template(key.expr.at, lhs) $(var rules) {
            rules |> replaceVariable("_") <| new ExprVar(at = key.expr.at, name := "cmp_lhs")
        }
        var rhs = clone_expression(key.expr)
        rhs = apply_template(key.expr.at, rhs) $(var rules) {
            rules |> replaceVariable("_") <| new ExprVar(at = key.expr.at, name := "cmp_rhs")
        }
        let last = i + 1 == length(keys)
        if (!last && !key_is_plain(key.expr)) {
            let lname = "cmp_key{i}_lhs"
            let rname = "cmp_key{i}_rhs"
            body |> push <| qmacro_expr() {
                let $i(lname) = $e(lhs)
            }
            body |> push <| qmacro_expr() {
                let $i(rname) = $e(rhs)
            }
            lhs = new ExprVar(at = key.expr.at, name := lname)
            rhs = new ExprVar(at = key.expr.at, name := rname)
        }
        if (key.greater && last) {
            body |> push <| qmacro_expr() {
                return $e(lhs) > $e(rhs)
            }
        } elif (last) {
            body |> push <| qmacro_expr() {
                return $e(lhs) < $e(rhs)
            }
        } elif (key.greater) {
            body |> push <| qmacro_expr() {
                if ($e(lhs) != $e(rhs)) {
                    return $e(lhs) > $e(rhs)
                }
            }
        } else {
            body |> push <| qmacro_expr() {
                if ($e(lhs) != $e(rhs)) {
                    return $e(lhs) < $e(rhs)
                }
            }
        }
    }
    return <- body
}

[structure_macro(name="ordered")]
class OrderedMacro : AstStructureAnnotation {
    //! ``[ordered]`` / ``[ordered(field, field = greater, key = fn)]``
    //! - generates the structure's ``operator <``.
    def override apply(var st : StructurePtr; var group : ModuleGroup;
                       var args : AnnotationArgumentList; var errors : das_string) : bool {
        if (st.flags.isClass) {
            errors := ("ordered: '{st.name}' is a class - a class is handled by pointer, "
                + "so its '<' would compare addresses")
            return false
        }
        var keys : array<OrderKey>
        var seen : table<string>
        for (arg in args) {
            let name = string(arg.name)
            if (name == "key") {
                if (arg.basicType != Type.tString) {
                    errors := "ordered: 'key' names a function of the structure - key = <function>"
                    return false
                }
                var call = new ExprCall(at = st.at, name := arg.sValue)
                call.arguments |> emplace_new <| new ExprVar(at = st.at, name := "_")
                keys |> push(OrderKey(expr = call))
                continue
            }
            var known = false
            for (f in st.fields) {
                known ||= f.name == name
            }
            if (!known || key_exists(seen, name)) {
                errors := (known ? "ordered: field '{name}' is listed twice"
                    : "ordered: '{st.name}' has no field '{name}'")
                return false
            }
            seen |> insert(name)
            let flag = arg.basicType == Type.tBool && arg.bValue ? "less" : ""
            let dir = arg.basicType == Type.tString ? string(arg.sValue) : flag
            if (dir != "less" && dir != "greater") {
                errors := "ordered: '{name}' takes no value, or 'less' / 'greater'"
                return false
            }
            var elem = new ExprVar(at = st.at, name := "_")
            var field = new ExprField(at = st.at, name := name, value = elem)
            keys |> push(OrderKey(expr = field, greater = dir == "greater"))
        }
        if (empty(args)) {
            for (f in st.fields) {
                var elem = new ExprVar(at = st.at, name := "_")
                var field = new ExprField(at = st.at, name := f.name, value = elem)
                keys |> push(OrderKey(expr = field))
            }
        }
        if (empty(keys)) {
            errors := "ordered: '{st.name}' has no fields to order by"
            return false
        }
        var body <- order_statements(keys)
        var fn = qmacro_function("<") $(cmp_lhs, cmp_rhs : $t(st)) : bool {
            $b(body)
        }
        fn.flags.generated = true
        fn.flags.privateFunction = st.flags.privateStructure
        fn.body |> force_at(st.at)
        compiling_module() |> add_function(fn)
        return true
    }
}

[call_macro(name="cmp_by")]
class CmpByMacro : AstCallMacro {
    //! ``cmp_by(key, -key, ...)`` - a comparator block ordering by
    //! keys over ``_``, the element; ``-`` sorts a key descending.
    def override canVisitArgument(expr : ExprCallMacro?; argIndex : int) : bool => false
    def override visit(prog : ProgramPtr; mod : Module?;
                       var expr : ExprCallMacro?) : ExpressionPtr {
        if (empty(expr.arguments)) {
            macro_error(prog, expr.at, "cmp_by: list at least one key over '_', the element")
            return <- default<ExpressionPtr>
        }
        var keys : array<OrderKey>
        for (arg in expr.arguments) {
            let neg = arg is ExprOp1 && (arg as ExprOp1).op == "-"
            var key = neg ? (arg as ExprOp1).subexpr : arg
            if (empty(find_var_refs(key, "_"))) {
                macro_error(prog, arg.at, "cmp_by: the key '{describe(arg)}' does not read '_'")
                return <- default<ExpressionPtr>
            }
            keys |> push(OrderKey(expr = key, greater = neg))
        }
        var body <- order_statements(keys)
        return <- qmacro($(cmp_lhs, cmp_rhs) {
            $b(body)
        })
    }
}
