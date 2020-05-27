module macros

require ast
require rtti

struct HintLookup
    tab : table<uint; string>

def get_hint ( lookup : HintLookup; id, defv : string; hint : uint )
    var result  = defv
    find_if_exists(lookup.tab, hint) <| $ ( res : string? )
        result = *res
    return result

def get_hint ( lookup : HintLookup; id, defv : string )
    let hint = hash(id)
    return get_hint(lookup,id,defv,hint)

class GetHintFnMacro : AstFunctionAnnotation
    [unsafe] def override transform ( call : smart_ptr<ExprCall>; errors : das_string ) : ExpressionPtr
        if call.arguments[1].__rtti=="ExprConstString"
            var newCall := call
            let arg2 = reinterpret<ExprConstString?>(call.arguments[1])
            let hint = hash("{arg2.value}")
            var mkc <- new [[ExprConstUInt() at=arg2.at, value=hint]]
            push(newCall.arguments, ExpressionPtr(mkc))
            return <- ExpressionPtr(newCall)
        return [[ExpressionPtr]]

[macro]
def setup
    if is_compiling_macros()
        let hfnm = new GetHintFnMacro()
        let ann <- make_function_annotation("hint_function", hfnm)
        let this_module <- ast_this_module()
        // add annotation to get_hit with 3 args
        // todo: come up with easier way to look-up functions (by tag?)
        ast_for_each_function(this_module, "get_hint") <| $ ( func )
            if func.arguments.length == 3
                add_function_annotation(func, ann)
        add_function_annotation(this_module, ann)

