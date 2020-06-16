module _hint_macros

require ast
require rtti
require daslib/ast_boost public

struct HintLookup
    tab : table<uint; string>

var
    g_non_hints = 0

def get_hint ( lookup : HintLookup; id, defv : string; hint : uint )
    return lookup.tab ?[ hint ] ?? defv

[tag_function(get_hint_tag)]
def get_hint ( lookup : HintLookup; id, defv : string )
    g_non_hints ++
    return get_hint(lookup,id,defv,hash(id))

[tag_function_macro(tag="get_hint_tag")]
class GetHintFnMacro : AstFunctionAnnotation
    def override transform ( var call : smart_ptr<ExprCall>; var errors : das_string ) : ExpressionPtr
        if call.arguments[1] is ExprConstString
            unsafe
                let arg2 = reinterpret<ExprConstString?>(call.arguments[1])
                let hint = hash("{arg2.value}")
                var mkc <- new [[ExprConstUInt() at=arg2.at, value=hint]]
                push(call.arguments, mkc)
                return <- call
        return [[ExpressionPtr]]


