options indenting = 4
options remove_unused_symbols = false

module json_boost shared

require rtti
require ast
require strings
require daslib/json
require daslib/ast_boost

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
            return <- convert_to_expression(json,expr.at)

