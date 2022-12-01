options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module extract_method shared private

require daslib/ast_boost
require daslib/templates_boost
require daslib/capture_block
require daslib/json_boost
require daslib/defer
require strings

let LOG_REFACTOR = 100500

[tag_function(extract_method_tag),sideeffects]
def public extract_method ( method_name:string; blk:block )
    invoke(blk)

def log_range ( at:LineInfo )
    return "{describe(at)}-{int(at.last_line)}:{int(at.last_column)}"

struct ExtractMethodDesc
    call_name : string
    function_declaration : string
    call_expression : string
    call_at : LineInfo
    function_body_at : LineInfo

def JV ( li:LineInfo )
    let fname = li.fileInfo!=null ? string(li.fileInfo.name) : ""
    let tabSize = li.fileInfo?.tabSize ?? 4
    var kv <- {{
        "line" => "{int(li.line)}";
        "column" => "{int(li.column)}";
        "last_line" => "{int(li.last_line)}";
        "last_column" => "{int(li.last_column)}";
        "file" => fname;
        "tab" => "{tabSize}"
    }}; defer_delete(kv)
    return JV(kv)

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
                writer |> write ( " )" )

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
        var jv <- JV([[ExtractMethodDesc
            call_name = call_name,
            function_declaration = function_declaration,
            call_expression = call_expression,
            call_at = atCall,
            function_body_at = atBlock
        ]])
        unsafe
            defer_delete(jv)
        to_compilation_log("!!!extract_method\n{write_json(jv)}\n!!!\n")
        return true

