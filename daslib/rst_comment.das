options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module rst_comment private

require daslib/ast_boost
require fio
require strings

enum ParserState
    none
    structure
    field
    func
    method

def private debug_rst ( s : string )
    // to_log(LOG_INFO,s)
    pass

[comment_reader]
class RstComment : AstCommentReader
    chars : array<uint8>
    state : ParserState = ParserState none
    comment : string
    structure_comment : string
    function_comment : string
    field_comment : string
    field_comments : array<tuple<kind:string;name:string;comment:string>>
    def override open ( prog:ProgramPtr; mod:Module?; cpp:bool; info:LineInfo ) : void
        assert(length(chars)==0,"already open")
    def override accept ( prog:ProgramPtr; mod:Module?; ch:int; info:LineInfo ) : void
        chars |> push(uint8(ch))
    def override close ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        if length(chars) <2 || int(chars[0]) != '!' || int(chars[1]) != ' '
            delete chars
            return
        chars |> erase(1)
        chars |> erase(0)
        chars |> push(uint8('\n'))
        comment = string(chars)
        delete chars
        if state==ParserState structure
            structure_comment += comment
        elif state==ParserState func || state==ParserState method
            function_comment += comment
        elif state==ParserState field
            field_comment += comment
        comment = ""
    def override beforeStructure ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        debug_rst("beforeStucture")
        if state != ParserState none
            panic("expecting none, got {state}")
        state = ParserState structure
    def override afterStructure ( st:StructurePtr; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        debug_rst("afterStucture {st._module.name}::{st.name} // {structure_comment}")
        if state!=ParserState structure && state!=ParserState field
            panic("expecting structure or field, got {state}")
        state = ParserState none
        var is_annotation = false
        if structure_comment != ""
            for ann in st.annotations
                if ann.annotation.name=="tag_function_macro"
                    write_to_detail("function_annotation-{prog.thisModuleName}-{st.name}", structure_comment)
                    is_annotation = true
                    break
                else
                    to_log(LOG_WARNING, "WARNING: unsupported structure-annotation = {ann.annotation.name} in {st.name}\n")
        // to_log(LOG_WARNING, "WARNING: unsupported structure-annotation = {ann.annotation.name} in {st.name}\n")
        if !is_annotation
            write_to_detail("structure-{prog.thisModuleName}-{st.name}", structure_comment)
        for fc in field_comments
            debug_rst("    {fc.kind} {fc.name} // {fc.comment}")
        structure_comment = ""
        delete field_comments
    def override beforeFunction ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        debug_rst("beforeFunction")
        if state!=ParserState none && state!=ParserState structure && state!=ParserState field
            panic("expecting none, structure, or field got {state}")
        state = state==ParserState none ? ParserState func : ParserState method
    def override afterFunction ( fn:FunctionPtr; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        debug_rst("afterFunction {state} {prog.thisModuleName}::{fn.name} // {function_comment}")
        if state!=ParserState func && state!=ParserState method
            panic("expecting func or method, got {state}")
        if state==ParserState func
            state = ParserState none
            if function_comment != ""
                write_to_detail("function-{prog.thisModuleName}-{fn.name}", function_comment)
        else
            if function_comment != ""
                field_comments |> push([[auto "method",string(fn.name),function_comment]])
            state = ParserState structure
        function_comment = ""
    def override beforeStructureFields ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        debug_rst("beforeStuctureFields")
        if state!=ParserState structure
            panic("expecting structure, got {state}")
        state = ParserState field
    def override afterStructureField ( name:string; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        debug_rst("afterStuctureField {name} // {field_comment}")
        if state!=ParserState structure && state!=ParserState field
            panic("expecting structure or field, got {state}")
        state = ParserState field
        if field_comment != ""
            debug_rst("\t-> push {name} // {field_comment}")
            field_comments |> push([[auto "field",clone_string(name),field_comment]])
    def override afterStructureFields ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        debug_rst("afterStuctureFields")
        if state!=ParserState structure && state!=ParserState field
            panic("expecting structure or field, got {state}")
        state = ParserState structure
        field_comment = ""
    def write_to_detail ( name, text : string ) : void
        let fname = get_das_root() + "/doc/source/stdlib/detail/{name}.rst"
        fopen(fname,"wb") <| $ ( f )
            if f==null
                panic("can't open {fname}")
            else
                f |> fwrite(text)






