options gen2

module test_compile_dag_reader public !inscope

require daslib/ast_boost

var sibling_comment_chars = 0
var all_comment_chars = 0

[comment_reader]
class SiblingCommentCounter : AstCommentReader {
    def override open(prog : ProgramPtr; mod : Module?; cpp : bool; info : LineInfo) : void {
        pass
    }
    def override accept(prog : ProgramPtr; mod : Module?; ch : int; info : LineInfo) : void {
        all_comment_chars ++
        if (mod != null && mod.name == "test_compile_dag_sibling") {
            sibling_comment_chars ++
        }
    }
    def override close(prog : ProgramPtr; mod : Module?; info : LineInfo) : void {
        pass
    }
    def override beforeStructure(prog : ProgramPtr; mod : Module?; info : LineInfo) : void {
        pass
    }
    def override afterStructure(st : StructurePtr; prog : ProgramPtr; mod : Module?;
                                info : LineInfo) : void {
        pass
    }
    def override beforeStructureFields(prog : ProgramPtr; mod : Module?; info : LineInfo) : void {
        pass
    }
    def override afterStructureField(name : string; prog : ProgramPtr; mod : Module?;
                                     info : LineInfo) : void {
        pass
    }
    def override afterStructureFields(prog : ProgramPtr; mod : Module?; info : LineInfo) : void {
        pass
    }
    def override beforeFunction(prog : ProgramPtr; mod : Module?; info : LineInfo) : void {
        pass
    }
    def override afterFunction(fn : FunctionPtr; prog : ProgramPtr; mod : Module?;
                               info : LineInfo) : void {
        pass
    }
    def override beforeGlobalVariables(prog : ProgramPtr; mod : Module?; info : LineInfo) : void {
        pass
    }
    def override afterGlobalVariable(name : string; prog : ProgramPtr; mod : Module?;
                                     info : LineInfo) : void {
        pass
    }
    def override afterGlobalVariables(prog : ProgramPtr; mod : Module?; info : LineInfo) : void {
        pass
    }
    def override beforeVariant(prog : ProgramPtr; mod : Module?; info : LineInfo) : void {
        pass
    }
    def override beforeVariantEntries(prog : ProgramPtr; mod : Module?; info : LineInfo) : void {
        pass
    }
    def override afterVariantEntry(name : string; prog : ProgramPtr; mod : Module?;
                                   info : LineInfo) : void {
        pass
    }
    def override afterVariantEntries(prog : ProgramPtr; mod : Module?; info : LineInfo) : void {
        pass
    }
    def override afterVariant(name : string; prog : ProgramPtr; mod : Module?;
                              info : LineInfo) : void {
        pass
    }
    def override beforeTuple(prog : ProgramPtr; mod : Module?; info : LineInfo) : void {
        pass
    }
    def override beforeTupleEntries(prog : ProgramPtr; mod : Module?; info : LineInfo) : void {
        pass
    }
    def override afterTupleEntry(name : string; prog : ProgramPtr; mod : Module?;
                                 info : LineInfo) : void {
        pass
    }
    def override afterTupleEntries(prog : ProgramPtr; mod : Module?; info : LineInfo) : void {
        pass
    }
    def override afterTuple(name : string; prog : ProgramPtr; mod : Module?;
                            info : LineInfo) : void {
        pass
    }
    def override beforeBitfield(prog : ProgramPtr; mod : Module?; info : LineInfo) : void {
        pass
    }
    def override beforeBitfieldEntries(prog : ProgramPtr; mod : Module?; info : LineInfo) : void {
        pass
    }
    def override afterBitfieldEntry(name : string; prog : ProgramPtr; mod : Module?;
                                    info : LineInfo) : void {
        pass
    }
    def override afterBitfieldEntries(prog : ProgramPtr; mod : Module?; info : LineInfo) : void {
        pass
    }
    def override afterBitfield(name : string; prog : ProgramPtr; mod : Module?;
                               info : LineInfo) : void {
        pass
    }
    def override beforeEnumeration(prog : ProgramPtr; mod : Module?; info : LineInfo) : void {
        pass
    }
    def override beforeEnumerationEntries(prog : ProgramPtr; mod : Module?;
                                          info : LineInfo) : void {
        pass
    }
    def override afterEnumerationEntry(name : string; prog : ProgramPtr; mod : Module?;
                                       info : LineInfo) : void {
        pass
    }
    def override afterEnumerationEntries(prog : ProgramPtr; mod : Module?; info : LineInfo) : void {
        pass
    }
    def override afterEnumeration(name : string; prog : ProgramPtr; mod : Module?;
                                  info : LineInfo) : void {
        pass
    }
    def override beforeAlias(prog : ProgramPtr; mod : Module?; info : LineInfo) : void {
        pass
    }
    def override afterAlias(name : string; prog : ProgramPtr; mod : Module?;
                            info : LineInfo) : void {
        pass
    }
}

def public compile_dag_reader_probe() => 1
