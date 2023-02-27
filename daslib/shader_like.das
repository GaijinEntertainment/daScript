options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers = true

module shader_like private

require daslib/ast_boost
require daslib/rtti

[macro]
class ValidateShaderVisitor : AstVisitor
    def override preVisitTypeDecl(typ:TypeDeclPtr) : void
        if !typ.isNoHeapType
            macro_error(compiling_program(), typ.at, "not shader-like, type requires heap {describe(typ)}")
    def override preVisitExprAscend(expr:smart_ptr<ExprAscend>) : void
        macro_error(compiling_program(), expr.at, "can't ascend in shader-like code (new [[...]])")
    def override preVisitExprNew(expr:smart_ptr<ExprNew>): void
        macro_error(compiling_program(), expr.at, "can't new in shader-like code (new ...)")

[simulate_macro(name="shader_like")]
class JIT_LLVM : AstSimulateMacro
    def override simulate ( prog:Program?; var ctx:Context? ) : bool
        return true if is_in_completion() || is_compiling_macros()
        return true if ctx.category.debug_context || ctx.category.macro_context || ctx.category.folding_context || ctx.category.debugger_tick || ctx.category.debugger_attached
        var astVisitor = new ValidateShaderVisitor()
        var inscope adapter <- make_visitor(*astVisitor)
        prog |> for_each_module <| $ ( mod )
            mod |> for_each_function("") <| $ ( f )
                if f.flags.used
                    visit(f, adapter)
            mod |> for_each_global() <| $ ( g )
                if g.flags.used
                    if !g._type.isNoHeapType
                        macro_error(compiling_program(), g.at, "global variable {g.name} requires heap")
                    elif g.init!=null
                        visit(g.init, adapter)
        unsafe
            delete astVisitor
        return true
