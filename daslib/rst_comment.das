options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module rst_comment private

require daslib/ast_boost
require strings

enum ParserState
    none
    structure
    field
    func
    method

[comment_reader]
class RstComment : AstCommentReader
    chars : array<uint8>
    state : ParserState = ParserState none
    comment : string
    structure_comment : string
    field_comments : array<tuple<kind:string;name:string;comment:string>>
    def override open ( prog:ProgramPtr; mod:Module?; cpp:bool; info:LineInfo ) : void
        assert(length(chars)==0,"already open")
    def override accept ( prog:ProgramPtr; mod:Module?; ch:int; info:LineInfo ) : void
        chars |> push(uint8(ch))
    def override close ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        comment = string(chars)
        delete chars
        to_log(LOG_INFO,"\t-> comment = {comment} {state}")
    def override beforeStructure ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        to_log(LOG_INFO,"beforeStucture")
        if state != ParserState none
            panic("expecting none, got {state}")
        state = ParserState structure
    def override afterStructure ( st:StructurePtr; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        to_log(LOG_INFO,"afterStucture {st._module.name}::{st.name} // {structure_comment}")
        if state!=ParserState structure && state!=ParserState field
            panic("expecting structure or field, got {state}")
        state = ParserState none
        for fc in field_comments
            to_log(LOG_INFO,"    {fc.kind} {fc.name} // {fc.comment}")
        structure_comment = ""
        delete field_comments
    def override beforeFunction ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        to_log(LOG_INFO,"beforeFunction")
        if state!=ParserState none && state!=ParserState structure && state!=ParserState field
            panic("expecting none, structure, or field got {state}")
        state = state==ParserState none ? ParserState func : ParserState method
    def override afterFunction ( fn:FunctionPtr; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        to_log(LOG_INFO,"afterFunction {prog.thisModuleName}::{fn.name} // {comment}")
        if state!=ParserState func && state!=ParserState method
            panic("expecting func or method, got {state}")
        if state==ParserState func
            state = ParserState none
            if comment != ""
                to_log(LOG_INFO,"DEF {prog.thisModuleName}::{fn.name} // {comment}\n")
        else
            if comment != ""
                field_comments |> push([[auto "method",string(fn.name),comment]])
            state = ParserState structure
        comment = ""
    def override beforeStructureFields ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        to_log(LOG_INFO,"beforeStuctureFields")
        structure_comment <- comment
        if state!=ParserState structure
            panic("expecting structure, got {state}")
    def override afterStructureField ( name:string; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        to_log(LOG_INFO,"afterStuctureField {name}")
        if state!=ParserState structure && state!=ParserState field
            panic("expecting structure or field, got {state}")
        state = ParserState field
        if comment != ""
            to_log(LOG_INFO,"\t-> push {name} // {comment}")
            field_comments |> push([[auto "field",clone_string(name),comment]])
    def override afterStructureFields ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        to_log(LOG_INFO,"afterStuctureFields")
        if state!=ParserState structure && state!=ParserState field
            panic("expecting structure or field, got {state}")
        state = ParserState structure
        comment = ""
        to_log(LOG_INFO,"\t-> delete comment")







