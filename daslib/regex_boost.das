module regex_boost

require ast
require daslib/ast_boost
require daslib/regex

class RegexReader : AstReaderMacro
    def override accept ( prog:ProgramPtr; mod:Module?; var expr:ExprReader?; ch:int; info:LineInfo ) : bool
        append(expr.sequence,ch)
        if ends_with(expr.sequence,"%%")
            let len = length(expr.sequence)
            resize(expr.sequence,len-2)
            return false
        else
            return true
    def override visit ( prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprReader> ) : ExpressionPtr
        var regex:Regex
        peek(expr.sequence) <| $ ( seqStr )
            regex_compile(regex,seqStr)
        if !is_valid(regex)
            delete regex
            macro_error(prog,expr.at,"regular expression did not compile")
            return [[ExpressionPtr]]
        // TODO: turn into AOT?
        delete regex
        var creg <- new [[ExprCall() at=expr.at, name:="regex::regex_compile"]]
        var ex <- new [[ExprConstString() at=expr.at, value:=expr.sequence]]
        push(creg.arguments,ExpressionPtr(ex))
        return ExpressionPtr(creg)

[macro]
def init_macros
    add_new_reader_macro("regex", new RegexReader())
