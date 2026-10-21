options gen2
options indenting = 4

module mc_shared_typename_macro

require daslib/ast
require daslib/ast_boost

[call_macro(name = "shared_typename")]
class SharedTypenameMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?;
                       var expr : ExprCallMacro?) : ExpressionPtr {
        var element = new TypeDecl(at = expr.at, baseType = Type.tFloat)
        var compound = new TypeDecl(at = expr.at, baseType = Type.tTuple)
        compound.argTypes |> resize(3)
        compound.argNames |> resize(3)
        for (name, i in ["a", "b", "c"], range(3)) {
            compound.argTypes[i] = element
            compound.argNames[i] := name
        }
        return new ExprTypeInfo(at = expr.at, trait := "typename",
            typeexpr = compound)
    }
}
