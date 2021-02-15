options indenting = 4
options no_aot = true

module static shared

require ast
require rtti
require daslib/ast_boost
require daslib/template_boost

[tag_function(static_let_tag)]
def static_let ( blk : block ) {}

[tag_function_macro(tag="static_let_tag")]
class StaticLetMacro : AstFunctionAnnotation
    def override transform ( var call : smart_ptr<ExprCallFunc>; var errors : das_string ) : ExpressionPtr
        assert(call.arguments[0] is ExprMakeBlock)
        unsafe
            var mblk = reinterpret<ExprMakeBlock?> call.arguments[0]
            var blk = reinterpret<ExprBlock?> mblk._block
            if blk.finalList.length != 0
                errors := "not expecting finally section in the static_let"
                return [[ExpressionPtr]]
            for ex in blk.list
                if !(ex is ExprLet)
                    errors := "expecting let ... expressions in the static_let"
                    return [[ExpressionPtr]]
                var lexpr = reinterpret<ExprLet?> ex
                for vvar in lexpr.variables
                    var vclone <- clone_variable(vvar)
                    vclone.flags |= VariableFlags private_variable
                    if !(compiling_module() |> add_variable(vclone))
                        panic("can't add global variable {vclone.name}")
        return quote <|
            pass

