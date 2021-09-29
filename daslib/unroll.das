options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module unroll shared private

require ast
require rtti
require daslib/ast_boost
require daslib/templates_boost

[tag_function(unroll_tag)]
def public unroll ( blk : block )
    invoke(blk)

class private RemoveDerefVisitor : AstVisitor
    vname : string
    def RemoveDerefVisitor ( n : string )
        vname = n
    def override visitExprRef2Value(expr:smart_ptr<ExprRef2Value>) : ExpressionPtr
        if expr.subexpr is ExprVar
            let vvar = expr.subexpr as ExprVar
            if "{vvar.name}"==vname
                return clone_expression(expr.subexpr)
        return expr

def remove_deref ( varname : string; var expr:smart_ptr<Expression> )
    var astVisitor = new RemoveDerefVisitor(varname)
    var astVisitorAdapter <- make_visitor(*astVisitor)
    expr |> visit(astVisitorAdapter)
    unsafe
        delete astVisitor
    astVisitorAdapter := null

[tag_function_macro(tag="unroll_tag")]
class UnrollMacro : AstFunctionAnnotation
    def override transform ( var call : smart_ptr<ExprCallFunc>; var errors : das_string ) : ExpressionPtr
        assert(call.arguments[0] is ExprMakeBlock)
        unsafe
            var mblk = reinterpret<ExprMakeBlock?> call.arguments[0]
            var blk = reinterpret<ExprBlock?> mblk._block
            if blk.finalList.length != 0
                errors := "not expecting finally section in the unroll"
                return [[ExpressionPtr]]
            if blk.list.length != 1 || !(blk.list[0] is ExprFor)
                errors := "expecting unroll <| for ..."
                return [[ExpressionPtr]]
            let efor = reinterpret<ExprFor?> blk.list[0]
            if !(efor.sources[0] is ExprConstRange)
                errors := "can only unroll for loop with range"
                return [[ExpressionPtr]]
            let euc = reinterpret<ExprConstRange?> efor.sources[0]
            let van = "{efor.iterators[0]}"
            var nblk <- new [[ExprBlock() at=call.at]]
            for i in euc.value
                var iblk <- clone_expression(efor.body)
                remove_deref(van, iblk)
                var rules : Template
                rules |> replaceVariable(van) <| new [[ExprConstInt() at=call.at, value=i]]
                rules |> apply_template(call.at,iblk,false)
                nblk.list |> emplace(iblk)
            return nblk

