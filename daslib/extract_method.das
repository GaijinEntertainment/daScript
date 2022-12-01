options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module extract_method shared public

require daslib/ast_boost
require daslib/templates_boost
require daslib/capture_block
require strings

let LOG_REFACTOR = 100500

[tag_function(extract_method_tag),sideeffects]
def public extract_method ( method_name:string; blk:block )
    invoke(blk)

def log_range ( at:LineInfo )
    return "{describe(at)}-{int(at.last_line)}:{int(at.last_column)}"

[tag_function_macro(tag="extract_method_tag")]
class AssertOnceMacro : AstFunctionAnnotation
    def override verifyCall ( var call : smart_ptr<ExprCallFunc>; args,progArgs:AnnotationArgumentList; var errors : das_string ) : bool
        if !(call.arguments[0] is ExprConstString)
            print("extract_method: first argument must be a string literal")
            return false
        var capture <- capture_block(call.arguments[1])
        sort(capture) <| $ ( a,b ) => string(a.variable.name) < string(b.variable.name)
        var call_name = string((call.arguments[0] as ExprConstString).value)
        let function_declaration = build_string <| $ ( writer )
            writer |> write ( "def ")
            writer |> write ( call_name )
            if length(capture) != 0
                writer |> write ( " ( " )
                var first = true
                for v in capture
                    if first
                        first = false
                    else
                        writer |> write("; ")
                    if !v.variable._type.flags.constant && v.variable.access_flags.access_ref
                        writer |> write ( "var " )
                    writer |> write ( v.variable.name )
                    writer |> write ( ":")
                    writer |> write ( describe(v.variable._type) )
                    if v.variable._type.isWorkhorseType && v.variable.access_flags.access_ref
                        writer |> write("&")
                writer |> write ( " ) " )

        let call_expression = build_string <| $ ( writer )
            writer |> write ( call_name )
            writer |> write ( "(" )
            if length(capture) != 0
                var first = true
                for v in capture
                    if first
                        first = false
                    else
                        writer |> write(",")
                    writer |> write ( v.variable.name )
            writer |> write ( ")" )
        assume atCall = call.atEnclosure
        assume atBlock = call.arguments[1].at
        to_log(LOG_REFACTOR,
            "EXTREACT METHOD {call_name}\n{function_declaration}\n{call_expression}\n{log_range(atCall)}\n{log_range(atBlock)}\n")
        return true

