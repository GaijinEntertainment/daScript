options indenting = 4
options remove_unused_symbols = false

module json_boost shared

require rtti
require ast
require strings
require daslib/json public
require daslib/ast_boost
require daslib/contracts
require daslib/templates

[private]
def is_json_ptr_value ( td : TypeDeclPtr )
    if td.baseType != Type tPointer
        return false
    if td.firstType == null
        return false
    if td.firstType.baseType != Type tStructure
        return false
    let st = td.firstType.structType
    if "{st.name}" != "JsonValue" && "{st._module.name}" != "json"
        return false
    return true


// replacing ExprIsVariant(value,name) => ExprIsVariant(value.value,name)
// replacing ExprAsVariant(value,name) => ExprAsVariant(value.value,name)
// if value is json::JsonValue?
[variant_macro(name="better_json")]
class BetterJsonMacro : AstVariantMacro
    def override visitExprIsVariant(prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprIsVariant>) : ExpressionPtr
        if is_json_ptr_value(expr.value._type)
            var vdr <- new [[ExprField() at=expr.at, name:="value", value <- clone_expression(expr.value)]]
            var isv <- new [[ExprIsVariant() at=expr.at, name:=expr.name, value <- vdr ]]
            return isv
        return [[ExpressionPtr]]
    def override visitExprAsVariant(prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprAsVariant>) : ExpressionPtr
        if is_json_ptr_value(expr.value._type)
            var vdr <- new [[ExprField() at=expr.at, name:="value", value <- clone_expression(expr.value)]]
            var isv <- new [[ExprAsVariant() at=expr.at, name:=expr.name, value <- vdr ]]
            return isv
        return [[ExpressionPtr]]

[reader_macro(name="json")]
class JsonReader : AstReaderMacro
    def override accept ( prog:ProgramPtr; mod:Module?; var expr:ExprReader?; ch:int; info:LineInfo ) : bool
        append(expr.sequence,ch)
        if ends_with(expr.sequence,"%%")
            let len = length(expr.sequence)
            resize(expr.sequence,len-2)
            return false
        else
            return true
    def override visit ( prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprReader> ) : ExpressionPtr
        let seqStr = string(expr.sequence)
        var error = ""
        var json = read_json(seqStr, error)
        if json == null
            macro_error(prog,expr.at,"json did not parse ({error})")
            return [[ExpressionPtr]]
        else
            var exx <- convert_to_expression(json,expr.at)
            return exx

[expect_any_enum(ent)]
def JV ( ent )
    return JV("{ent}")

[template(ent)]
def from_JV ( jv:JsonValue?; ent : auto(EnumT) ) : EnumT
    let name = jv.value as _string
    var res : auto(EnumTT) = [[EnumT]]
    var ti = typeinfo(rtti_typeinfo type<EnumT>)
    for ef in *ti.enumType
        if name==ef.name
            unsafe
                res = reinterpret<EnumTT> (ef.value)
            return res
    panic("not a valid enumeration {name} in {typeinfo(typename type<EnumT>)}")
    return res

def JV ( v : float )
    return new [[JsonValue value <- [[JsValue _number = double(v)]]]]

def JV ( v : int )
    return new [[JsonValue value <- [[JsValue _number = double(v)]]]]

[expect_any_vector_type(v)]
def JV ( v : auto(VecT) )
    static_if typeinfo(vector_dim type<VecT>)==2
        return JV({{"x" => JV(v.x); "y" => JV(v.y)}})
    static_elif typeinfo(vector_dim type<VecT>)==3
        return JV({{"x" => JV(v.x); "y" => JV(v.y); "z" => JV(v.z)}})
    static_elif typeinfo(vector_dim type<VecT>)==4
        return JV({{"x" => JV(v.x); "y" => JV(v.y); "z" => JV(v.z); "w" => JV(v.w)}})

[expect_any_vector_type(ent), template(ent)]
def from_JV ( var v : JsonValue?; ent: auto(VecT)  )
    let VecV : auto(VecTT) = VecT()
    var vo & = v.value as _object
    static_if typeinfo(vector_dim VecV)==2
        return VecTT(
            vo["x"].value as _number,
            vo["y"].value as _number)
    static_elif typeinfo(vector_dim VecV)==3
        return VecTT(
            vo["x"].value as _number,
            vo["y"].value as _number,
            vo["z"].value as _number)
    static_elif typeinfo(vector_dim VecV)==4
        return VecTT(
            vo["x"].value as _number,
            vo["y"].value as _number,
            vo["z"].value as _number,
            vo["w"].value as _number)
