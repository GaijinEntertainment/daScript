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
            var prefix = "`static`{call.at.line}`"
            var rules : Template
            for ex in blk.list
                if !(ex is ExprLet)
                    errors := "expecting let ... expressions in the static_let"
                    return [[ExpressionPtr]]
                var lexpr = reinterpret<ExprLet?> ex
                for vvar in lexpr.variables
                    var vclone <- clone_variable(vvar)
                    vclone.flags |= VariableFlags private_variable
                    let old_name = "{vclone.name}"
                    let new_name = "{prefix}{vclone.name}"
                    vclone.name := new_name
                    rules |> renameVariable(old_name) <| new_name
                    if !(compiling_module() |> add_variable(vclone))
                        panic("can't add global variable {vclone.name}")
            var success = true
            compiling_program() |> get_ast_context(call) <| $(valid, astc)
                if !valid
                    errors := "can't get valid program context"
                    success = false
                else
                    apply_template(rules, call.at, astc.scopes[astc.scopes.length-1],false)
            delete rules
            if !success
                return [[ExpressionPtr]]
        return quote <|
            pass

