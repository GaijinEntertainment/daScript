options indenting = 4

module json_boost

require rtti
require ast
require daslib/json
require daslib/ast_boost

def is_json_ptr_value ( td : TypeDeclPtr )
    if td.baseType != Type tPointer
        return false
    if td.firstType == null
        return false
    if td.firstType.baseType != Type tStructure
        return false
    let st = td.firstType.structType
    if "{st.name}" != "JsonValue" & "{st._module.name}" != "json"
        return false
    return true


// replacing ExprIsVariant(value,name) => ExprIsVariant(value.value,name)
// replacing ExprAsVariant(value,name) => ExprAsVariant(value.value,name)
// if value is json::JsonValue?
class BetterJsonMacro : AstVariantMacro
    def override visitExprIsVariant(prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprIsVariant>) : ExpressionPtr
        if is_json_ptr_value(expr.value._type)
            var vdr <- new [[ExprField() at=expr.at, name:="value", value <- clone_expression(expr.value)]]
            var isv <- new [[ExprIsVariant() at=expr.at, name:=expr.name, value <- ExpressionPtr(vdr) ]]
            return ExpressionPtr(isv)
        return [[ExpressionPtr]]
    def override visitExprAsVariant(prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprAsVariant>) : ExpressionPtr
        if is_json_ptr_value(expr.value._type)
            var vdr <- new [[ExprField() at=expr.at, name:="value", value <- clone_expression(expr.value)]]
            var isv <- new [[ExprAsVariant() at=expr.at, name:=expr.name, value <- ExpressionPtr(vdr) ]]
            return ExpressionPtr(isv)
        return [[ExpressionPtr]]

[unsafe]
def make_json_embed ( value:JsonValue?; at:LineInfo ) : ExpressionPtr
    var jsv <- new [[ExprCall() name:="JV", at=at]]
    if value.value is _null
        push(jsv.arguments,ExpressionPtr(new [[ExprConstPtr() at=at]]))
    elif value.value is _bool
        push(jsv.arguments,ExpressionPtr(new [[ExprConstBool() at=at, value=value.value as _bool]]))
    elif value.value is _number
        push(jsv.arguments,ExpressionPtr(new [[ExprConstDouble() at=at, value=value.value as _number]]))
    elif value.value is _string
        push(jsv.arguments,ExpressionPtr(new [[ExprConstString() at=at, value:=value.value as _string]]))
    elif value.value is _array
        let total = length(value.value as _array)
        if total != 0
            var arrT <- clone_type(typeinfo(ast_typedecl type<json::JsonValue?>))
            push(arrT.dim,total)
            var mkArr <- new [[ExprMakeArray() at=at, makeType <- arrT]]
            for x in (value.value as _array)
                push(mkArr.values,make_json_embed(x,at))
            var mkToArrayMove <- new [[ExprCall() at=at, name:="to_array_move"]]
            push(mkToArrayMove.arguments,ExpressionPtr(mkArr))
            push(jsv.arguments,ExpressionPtr(mkToArrayMove))
        else
            jsv.name := "JVArr"
    elif value.value is _object
        let total = length(value.value as _object)
        if total != 0
            var tupT <- typeinfo(ast_typedecl type<tuple<string;JsonValue?>>)
            var arrT <- clone_type(tupT)
            push(arrT.dim,total)
            var mkArr <- new [[ExprMakeArray() at=at, makeType <- arrT]]
            for n,x in keys(value.value as _object),values(value.value as _object)
                var mkTup <- new [[ExprMakeTuple() at=at, recordType <- clone_type(tupT), isKeyValue=true]]
                push(mkTup.values,ExpressionPtr(new [[ExprConstString() at=at, value:=n]]))
                push(mkTup.values,make_json_embed(x,at))
                push(mkArr.values,ExpressionPtr(mkTup))
            var mkToTableMove <- new [[ExprCall() at=at, name:="to_table_move"]]
            push(mkToTableMove.arguments,ExpressionPtr(mkArr))
            push(jsv.arguments,ExpressionPtr(mkToTableMove))
        else
            jsv.name := "JVObj"
    else
        panic("unsupported json subtype")
    return ExpressionPtr(jsv)

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
            var jsv <- make_json_embed(json,expr.at)
            return ExpressionPtr(jsv)

[macro]
def setup
    add_new_variant_macro("better_json", new BetterJsonMacro())
    add_new_reader_macro("json", new JsonReader())
