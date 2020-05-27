options log = true

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

[tag_function(get_hint_macro)]
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
    let hfnm = new GetHintFnMacro()
    let ann <- make_function_annotation("hint_function", hfnm)
    let this_module <- ast_this_module()
    ast_for_each_tag_function(this_module, "get_hint_macro") <| $ ( func )
        add_function_annotation(func, ann)
    add_function_annotation(this_module, ann)

