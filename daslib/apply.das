options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module apply shared private

require ast
require daslib/ast_boost
require daslib/defer

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

/*
    def apply`Foo(self:Foo;arg_field1:block<(name:string,value:field1-type):void>;arg_field2:...)
        invoke(arg_field1,"field1",self.field1)
        invoke(arg_field2,"field2",self.field2)
        ...
*/
[macro_function]
def generateApplyVisitTuple ( stype:TypeDeclPtr; fnname:string; at:LineInfo )
    assert(stype.baseType==Type tTuple)
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
    for flda,fldi in stype.argTypes,range(100500)
        var argT <- new [[TypeDecl() baseType=Type tBlock, at=at]]                         // block<(name:string;x:field1type)>
        emplace_new(argT.argTypes, new [[TypeDecl() baseType=Type tString, at=at]])
        var fldT <- clone_type(flda)
        if stype.flags.constant
            fldT.flags |= TypeDeclFlags constant
        if !fldT.isRefType
            fldT.flags |= TypeDeclFlags ref
        emplace(argT.argTypes, fldT)
        argT.firstType <- new [[TypeDecl() baseType=Type tVoid,at=at]]
        argT.flags = TypeDeclFlags constant
        let fldname = length(stype.argNames)==length(stype.argTypes) ? string(stype.argNames[fldi]) : "_{fldi}"
        var argF <- new [[Variable() name:="__arg_{fldname}",at=at,_type <- argT]]   // arg_field1:block...
        emplace(fn.arguments,argF)
        var cfield <- new [[ExprInvoke() at=at]]                                    // invoke()
        var argb <- new [[ExprVar() at=at, name:="__arg_{fldname}"]]                 // arg_field1
        emplace(cfield.arguments,argb)
        var argn <- new [[ExprConstString() at=at, value:=fldname]]                // "name"
        emplace(cfield.arguments,argn)
        var argvar <- new [[ExprVar() at=at, name:="__self"]]                       // self
        var argfld <- new [[ExprField() at=at, name:=fldname, value <- argvar]]    // self.name
        emplace(cfield.arguments,argfld)
        emplace(blk.list, cfield)
    fn.body <- blk
    compiling_module() |> add_function(fn)

/*
    def apply`Foo(self:Foo;arg_field1:block<(name:string,value:field1-type):void>;arg_field2:...)
        if variant_index(self)==0
            invoke(arg_field1,"field1",self.field1)
            return
        if variant_idnex(self)==2
            invoke(arg_field2,"field2",self.field2)
            return
        ...
*/
[macro_function]
def generateApplyVisitVariant ( stype:TypeDeclPtr; fnname:string; at:LineInfo )
    assert(stype.baseType==Type tVariant)
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
    for flda,fldi in stype.argTypes,range(100500)
        var argT <- new [[TypeDecl() baseType=Type tBlock, at=at]]                         // block<(name:string;x:field1type)>
        emplace_new(argT.argTypes, new [[TypeDecl() baseType=Type tString, at=at]])
        var fldT <- clone_type(flda)
        if stype.flags.constant
            fldT.flags |= TypeDeclFlags constant
        if !fldT.isRefType
            fldT.flags |= TypeDeclFlags ref
        emplace(argT.argTypes, fldT)
        argT.firstType <- new [[TypeDecl() baseType=Type tVoid,at=at]]
        argT.flags = TypeDeclFlags constant
        let fldname = length(stype.argNames)==length(stype.argTypes) ? string(stype.argNames[fldi]) : "_{fldi}"
        var argF <- new [[Variable() name:="__arg_{fldname}",at=at,_type <- argT]]   // arg_field1:block...
        emplace(fn.arguments,argF)
        var cfield <- new [[ExprInvoke() at=at]]                                    // invoke()
        var argb <- new [[ExprVar() at=at, name:="__arg_{fldname}"]]                 // arg_field1
        emplace(cfield.arguments,argb)
        var argn <- new [[ExprConstString() at=at, value:=fldname]]                     // "name"
        emplace(cfield.arguments,argn)
        var argvar <- new [[ExprVar() at=at, name:="__self"]]                           // self
        var argfld <- new [[ExprAsVariant() at=at, name:=fldname, value <- argvar]]    // self as name
        emplace(cfield.arguments,argfld)
        // variant_index(self)==fld
        var vvidx <- new [[ExprCall() at=at, name:="variant_index"]]
        vvidx.arguments |> emplace_new <| new [[ExprVar() at=at, name:="__self"]]
        var vcond <- new [[ExprOp2() at=at, op:="==",
            left <- vvidx,
            right <- new [[ExprConstInt() at=at, value=fldi]]
        ]]
        // invoke(); return
        var vblk <- new [[ExprBlock() at=at]]
        vblk.list |> emplace(cfield)
        vblk.list |> emplace_new <| new [[ExprReturn() at=at]]
        // if
        blk.list |> emplace_new <| new [[ExprIfThenElse() at=at,
            cond <- vcond,
            if_true <- vblk
        ]]
    fn.body <- blk
    compiling_module() |> add_function(fn)


[call_macro(name="apply")]  // apply(value, block)
class ApplyMacro : AstCallMacro
    //! This macro implements the apply() pattern. The idea is that for each entry in the structure, variant, or tuple,
    //! the block will be invoked. Both element name, and element value are passed to the block.
    //! For example::
    //!     struct Bar
	//!         x, y : float
	//!     apply([[Bar x=1.,y=2.]]) <| $ ( name:string; field )
    //!         print("{name} = {field} ")
    //! Would print x = 1.0 y = 2.0
    def override visit ( prog:ProgramPtr; mod:Module?; var expr:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        if expr.arguments.length != 2
            macro_error(prog,expr.at,"expecting apply(value, block)")
            return [[ExpressionPtr]]
        if expr.arguments[0]._type!=null    // need value inferred
            var argT := expr.arguments[0]._type
            defer <|
                argT := null
            if argT.dim.length != 0
                macro_error(prog,expr.at,"can't apply to dim")
                return [[ExpressionPtr]]
            if argT.baseType==Type tStructure || argT.baseType==Type tTuple || argT.baseType==Type tVariant
                pass
            else
                macro_error(prog,expr.at,"can only apply to {describe(expr.arguments[0]._type)}")
                return [[ExpressionPtr]]
            if !(expr.arguments[1] is ExprMakeBlock)
                macro_error(prog,expr.at,"expecting make block, i.e. $(..)")
                return [[ExpressionPtr]]
            var callName : string
            var nfields : int
            if argT.baseType == Type tStructure
                callName = "apply`struct`{argT.structType.name}`{expr.at.line}"
                nfields = argT.structType.fields.length
                generateApplyVisitStruct(expr.arguments[0]._type,callName,expr.at)
            elif argT.baseType == Type tTuple
                callName = "apply`tuple`{intptr(expr)}`{expr.at.line}"
                nfields = length(argT.argTypes)
                generateApplyVisitTuple(expr.arguments[0]._type,callName,expr.at)
            elif argT.baseType == Type tVariant
                callName = "apply`variant`{intptr(expr)}`{expr.at.line}"
                nfields = length(argT.argTypes)
                generateApplyVisitVariant(expr.arguments[0]._type,callName,expr.at)
            else
                panic("internal error. usupported apply")
            // make a call
            var call <- new [[ExprCall() name:="_::" + callName, at=expr.at]]
            emplace_new(call.arguments,clone_expression(expr.arguments[0]))
            for i in range(nfields)
                emplace_new(call.arguments,clone_expression(expr.arguments[1]))
            return <- call
        return [[ExpressionPtr]]
