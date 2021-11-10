options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module apply shared private

require ast
require daslib/ast_boost

/*
    def apply`Foo(self:Foo;arg_field1:block<(name:string,value:field1-type):void>;arg_field2:...)
        invoke(arg_field1,"field1",self.field1)
        invoke(arg_field2,"field2",self.field2)
        ...
*/
[macro_function]
def generateApplyVisitStruct ( stype:TypeDeclPtr; fnname:string; at:LineInfo )
    assert(stype.baseType==Type tStructure)
    assert(stype.dim.length==0)
    var fn <- new [[Function() at=at, atDecl=at, name:=fnname]]                     // def apply`foo : void
    fn.flags |= FunctionFlags generated
    fn.flags |= FunctionFlags privateFunction
    fn.result <- new [[TypeDecl() baseType=Type autoinfer, at=at]]
    var selfT <- clone_type(stype)
    selfT.flags |= TypeDeclFlags isExplicit
    selfT.flags |= TypeDeclFlags explicitConst
    emplace_new(fn.arguments, new [[Variable() name:="__self",at=at, _type <- selfT]])   // self:Foo
    var blk <- new [[ExprBlock() at=at]]
    for fld in stype.structType.fields
        var argT <- new [[TypeDecl() baseType=Type tBlock, at=at]]                         // block<(name:string;x:field1type)>
        emplace_new(argT.argTypes, new [[TypeDecl() baseType=Type tString, at=at]])
        var fldT <- clone_type(fld._type)
        if stype.flags.constant
            fldT.flags |= TypeDeclFlags constant
        if !fldT.isRefType
            fldT.flags |= TypeDeclFlags ref
        emplace(argT.argTypes, fldT)
        argT.firstType <- new [[TypeDecl() baseType=Type tVoid,at=at]]
        argT.flags = TypeDeclFlags constant
        var argF <- new [[Variable() name:="__arg_{fld.name}",at=at,_type <- argT]]   // arg_field1:block...
        emplace(fn.arguments,argF)
        var cfield <- new [[ExprInvoke() at=at]]                                    // invoke()
        var argb <- new [[ExprVar() at=at, name:="__arg_{fld.name}"]]                 // arg_field1
        emplace(cfield.arguments,argb)
        var argn <- new [[ExprConstString() at=at, value:=fld.name]]                // "name"
        emplace(cfield.arguments,argn)
        var argvar <- new [[ExprVar() at=at, name:="__self"]]                       // self
        var argfld <- new [[ExprField() at=at, name:=fld.name, value <- argvar]]    // self.name
        emplace(cfield.arguments,argfld)
        emplace(blk.list, cfield)
    fn.body <- blk
    compiling_module() |> add_function(fn)

[call_macro(name="apply")]  // apply(value, block)
class ApplyMacro : AstCallMacro
    def override visit ( prog:ProgramPtr; mod:Module?; var expr:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        if expr.arguments.length != 2
            macro_error(prog,expr.at,"expecting apply(value, block)")
            return [[ExpressionPtr]]
        if expr.arguments[0]._type!=null    // need value inferred
            if expr.arguments[0]._type.dim.length != 0
                macro_error(prog,expr.at,"can't apply to dim")
                return [[ExpressionPtr]]
            if expr.arguments[0]._type.baseType != Type tStructure
                macro_error(prog,expr.at,"can only apply to structure")
                return [[ExpressionPtr]]
            if !(expr.arguments[1] is ExprMakeBlock)
                macro_error(prog,expr.at,"expecting make block, i.e. $(..)")
            var callName = "apply`{expr.arguments[0]._type.structType.name}"
            generateApplyVisitStruct(expr.arguments[0]._type,callName,expr.at)
            // make a call
            var call <- new [[ExprCall() name:="_::" + callName, at=expr.at]]
            emplace_new(call.arguments,clone_expression(expr.arguments[0]))
            let nfields = expr.arguments[0]._type.structType.fields.length
            for i in range(nfields)
                emplace_new(call.arguments,clone_expression(expr.arguments[1]))
            return <- call
        return [[ExpressionPtr]]




