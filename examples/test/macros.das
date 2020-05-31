module macros

// require limbo
require ast
require rtti

struct HintLookup
    tab : table<uint; string>

def get_hint ( lookup : HintLookup; id, defv : string; hint : uint )
    return lookup.tab ?[ hint ] ?? defv

[tag_function(get_hint_tag)]
def get_hint ( lookup : HintLookup; id, defv : string )
    return get_hint(lookup,id,defv,hash(id))

[tag_function_macro(tag="get_hint_tag")]
class GetHintFnMacro : AstFunctionAnnotation
    [unsafe] def override transform ( var call : smart_ptr<ExprCall>; var errors : das_string ) : ExpressionPtr
        if call.arguments[1] is ExprConstString
            let arg2 = reinterpret<ExprConstString?>(call.arguments[1])
            let hint = hash("{arg2.value}")
            var mkc <- new [[ExprConstUInt() at=arg2.at, value=hint]]
            push_back(call.arguments, mkc)
            return <- ExpressionPtr(call)
        return [[ExpressionPtr]]


