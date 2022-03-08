options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module rst_comment private

require daslib/ast_boost
require strings

[comment_reader]
class RstComment : AstCommentReader
    def override open ( prog:ProgramPtr; mod:Module?; cpp:bool; info:LineInfo ) : void
        print("/* cpp={cpp}\n")
    def override accept ( prog:ProgramPtr; mod:Module?; ch:int; info:LineInfo ) : void
        print("{to_char(ch)}")
    def override close ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        print("*/\n")
    def override beforeStructure ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        print("!!! beforeStructure\n")
    def override afterStructure ( st:StructurePtr; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        print("!!! afterStructure {st.name}\n")
    def override beforeFunction ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        print("!!! beforeFunction\n")
    def override afterFunction ( fn:FunctionPtr; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        print("!!! afterFunction {fn.name}\n")
    def override beforeStructureFields ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        print("!!! beforeSturctureFields\n")
    def override afterStructureField ( name:string; prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        print("!!! afterSturctureField {name}\n")
    def override afterStructureFields ( prog:ProgramPtr; mod:Module?; info:LineInfo ) : void
        print("!!! afterSturctureFields\n")






