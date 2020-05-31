require rtti
require ast

def is_json_ptr_value ( td : TypeDeclPtr )
    if td.baseType != Type tPointer
        return false
    if td.firstType == null
        return false
    if td.firstType.baseType != Type tStructure
        return false
    let st = td.firstType.structType
    if "{st.name}" != "JsonValue" & "{st.structureModule.name}" != "json"
        return false
    return true


// replacing ExprIsVariant(value,name) => ExprIsVariant(value.value,name)
// replacing ExprAsVariant(value,name) => ExprAsVariant(value.value,name)
// if value is json::JsonValue?
class BetterJsonVisitor : AstVisitor
    astChanged : bool = false
    def override visitExprIsVariant(expr:smart_ptr<ExprIsVariant>) : ExpressionPtr
        if is_json_ptr_value(expr.value.typeDecl)
            astChanged = true
            var vdr <- new [[ExprField() at=expr.at, name:="value", value <- clone_expression(expr.value)]]
            var isv <- new [[ExprIsVariant() at=expr.at, name:=expr.name, value <- ExpressionPtr(vdr) ]]
            return ExpressionPtr(isv)
        return ExpressionPtr(expr)
    def override visitExprAsVariant(expr:smart_ptr<ExprAsVariant>) : ExpressionPtr
        if is_json_ptr_value(expr.value.typeDecl)
            astChanged = true
            var vdr <- new [[ExprField() at=expr.at, name:="value", value <- clone_expression(expr.value)]]
            var isv <- new [[ExprAsVariant() at=expr.at, name:=expr.name, value <- ExpressionPtr(vdr) ]]
            return ExpressionPtr(isv)
        return ExpressionPtr(expr)

class BetterJson : AstPassMacro
    visitor : BetterJsonVisitor?
    adapter : smart_ptr<VisitorAdapter>
    [unsafe] def BetterJson
        visitor <- new BetterJsonVisitor()
        adapter <- make_visitor(*visitor)
    def override apply ( prog:ProgramPtr; mod:Module? ) : bool
        visitor.astChanged = false
        visit(prog, adapter)
        return visitor.astChanged

[_macro]
def setup
    if is_compiling_macros()
        this_module() |> add_dirty_infer_macro(
            make_pass_macro("better_json", new BetterJson()))
