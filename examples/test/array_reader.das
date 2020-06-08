module array_reader

require rtti
require ast

class Uint8ArrayReader : AstReaderMacro
    sequence : array<uint8>
    def override accept ( var expr:ExprReader?; ch:int ) : bool
        if ch==-1
            clear(sequence)
            return false
        push(sequence,uint8(ch))
        let sz = length(sequence)
        if sz >= 2
            if int(sequence[sz-2])=='%' & int(sequence[sz-1])=='%'
                resize(sequence,sz-2)
                expr.sequence := string(sequence)
                clear(sequence)
                return false
        return true
    def override visit ( prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprReader> ) : ExpressionPtr
        let seqStr = string(expr.sequence)
        var arrT <- new [[TypeDecl() baseType=Type tInt]]
        push(arrT.dim,length(seqStr))
        var mkArr <- new [[ExprMakeArray() at = expr.at, makeType <- arrT]]
        for x in seqStr
            var mkC <- new [[ExprConstInt() at=expr.at, value=x]]
            push(mkArr.values,ExpressionPtr(mkC))
        return ExpressionPtr(mkArr)

[macro]
def init_macros
    add_new_reader_macro("arr", new Uint8ArrayReader())
