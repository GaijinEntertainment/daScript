options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

module spoof shared private

require daslib/ast_boost
require strings

let SPOOF_SPEFICAL = '%'

enum SpoofError
    none
    expecting_open_paren
    invalid_argument_character
    invalid_argument_count
    invalid_spoof_character
    expecting_var_name
    eof

[macro_function]
def next_not_whitespace ( it:iterator<int>; var Ch:int& ) : bool
    Ch = -1
    while next(it,Ch)
        if !is_white_space(Ch)
            return true
    return false

[macro_function]
def next_ident ( it:iterator<int>; var Ch:int&; var ident:string& ) : bool
    var id : array<uint8>
    Ch = -1
    while next(it,Ch)
        if is_alpha(Ch) || is_number(Ch) || Ch=='_'
            id |> push(uint8(Ch))
        else
            break
    return false if id |> length == 0
    ident = string(id)
    return true

[macro_function]
def instance_spoof ( temp:string; instance_arguments:array<string> ) : tuple<result:string;error:SpoofError>
    var it <- unsafe(each(temp))
    var Ch:int
    // read template arguments
    if !next_not_whitespace(it,Ch)
        return [[auto "", SpoofError expecting_open_paren]]
    if Ch!='(' || Ch==-1
        return [[auto "", SpoofError expecting_open_paren]]
    var args : array<string>
    while true
        var ident : string
        if next_ident(it,Ch,ident)
            args |> push(ident)
            if Ch==','
                continue
            elif Ch==')'
                break
            else
                return [[auto "invalid character {Ch}", SpoofError invalid_argument_character]]
        elif is_white_space(Ch)
            continue
    // compare argument counts
    if length(args) != length(instance_arguments)
        return [[auto "argument count mismatch", SpoofError invalid_argument_count]]
    // skip to next line
    Ch = -1
    while next(it,Ch)
        if Ch==' ' || Ch=='\t'
            continue
        elif Ch=='\n'
            break
        elif Ch=='\r'
            pass
        else
            return [[auto "invalid character {Ch}", SpoofError invalid_argument_character]]
    if Ch==-1
        return [[auto "unexpected end of file", SpoofError eof]]
    // now, lets apply the arguments
    var result : array<uint8>
    var pendingCh : int
    Ch = 0
    while Ch==-1 || next(it,Ch)
        if Ch==-1                   // we have pending character
            Ch = pendingCh
        if Ch==SPOOF_SPEFICAL
            var ident : string
            if next_ident(it,Ch,ident)
                var index = args |> find_index(ident)
                if index != -1
                    for c in instance_arguments[index]
                        result |> push(uint8(c))
                if Ch!=-1           // pending after ident
                    pendingCh = Ch
                    Ch = -1
            elif Ch==SPOOF_SPEFICAL
                result |> push(uint8(SPOOF_SPEFICAL))
                result |> push(uint8(SPOOF_SPEFICAL))
            elif Ch==-1
                break   // eof
            else
                return [[auto "invalid sequence after %", SpoofError invalid_spoof_character]]
        else
            result |> push(uint8(Ch))

    return [[auto string(result), SpoofError none]]

def instance_args ( temp:string ) : tuple<varName:string;args:array<string>;error:SpoofError>
    var it <- unsafe(each(temp))
    var Ch:int
    var args : array<string>
    var varName : string
    if !next_ident(it,Ch,varName)
        return <- [[auto "expecting template name", args, SpoofError expecting_var_name]]
    while true
        var ident : string
        if next_ident(it,Ch,ident)
            args |> push(ident)
            if Ch==','
                continue
            elif Ch==')'
                break
            else
                return <- [[auto "invalid character {Ch}", args, SpoofError invalid_argument_character]]
        elif is_white_space(Ch)
            continue
    return <- [[auto varName, args, SpoofError none]]

[reader_macro(name="spoof_template")]
class SpoofTemplateReader : AstReaderMacro
    def override accept ( prog:ProgramPtr; mod:Module?; var expr:ExprReader?; ch:int; info:LineInfo ) : bool
        append(expr.sequence,ch)
        if ends_with(expr.sequence,"%%")
            let len = length(expr.sequence)
            resize(expr.sequence,len-2)
            return false
        else
            return true
    def override visit ( prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprReader> ) : ExpressionPtr
        return <- new [[ExprConstString() at=expr.at, value:=expr.sequence]]


[reader_macro(name="spoof_instance")]
class SpoofInstanceReader : AstReaderMacro
    def override accept ( prog:ProgramPtr; mod:Module?; var expr:ExprReader?; ch:int; info:LineInfo ) : bool
        append(expr.sequence,ch)
        if ends_with(expr.sequence,"%%")
            let len = length(expr.sequence)
            resize(expr.sequence,len-2)
            return false
        else
            return true
    def override suffix ( prog:ProgramPtr; mod:Module?; var expr:ExprReader?; info:LineInfo; var outLine:int& ) : string
        outLine = int(info.line)
        var iargs <- instance_args(string(expr.sequence))
        if iargs.error != SpoofError none
            prog |> macro_error(info, "syntax error ({iargs.error}): {iargs.varName}")
            return ""
        // lets find the variable for the template
        var inscope vptr : VariablePtr
        find_matching_variable(get_ptr(prog),null,iargs.varName,false) <| $ ( vars )
            if vars |> length == 0
                prog |> macro_error(info, "no matching variable found for {iargs.varName}")
                return
            elif vars |> length > 1
                prog |> macro_error(info, "multiple matching variables found for {iargs.varName}")
                return
            else
                vptr := vars[0]
        if vptr == null
            return ""
        if vptr.init == null
            prog |> macro_error(info, "variable {iargs.varName} has no initializer")
            return ""
        var vstr = ""
        if vptr.init is ExprConstString
            vstr = string((vptr.init as ExprConstString).value)
        elif vptr.init is ExprReader
            vstr = string((vptr.init as ExprReader).sequence)
        else
            prog |> macro_error(info, "variable {iargs.varName} is not a string")
            return ""
        // now, lets spoof the instance
        outLine = int(vptr.init.at.line) + 1
        var result <- instance_spoof(string(vstr),iargs.args)
        if result.error != SpoofError none
            prog |> macro_error(info, "spoof instance error({result.error}): {result.result}")
            return ""
        return result.result

