options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module apply shared private

require ast
require daslib/ast_boost
require daslib/defer
require daslib/templates_boost

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
    fn.flags |= FunctionFlags generated | FunctionFlags privateFunction
    fn.result <- new [[TypeDecl() baseType=Type autoinfer, at=at]]
    var selfT <- clone_type(stype)
    selfT.flags |= TypeDeclFlags isExplicit | TypeDeclFlags explicitConst
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
        blk.list |> emplace_new <|  qmacro(invoke($i("__arg_{fld.name}"),$v(string(fld.name)),__self.$f(fld.name)))
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
    fn.flags |= FunctionFlags generated | FunctionFlags privateFunction
    fn.result <- new [[TypeDecl() baseType=Type autoinfer, at=at]]
    var selfT <- clone_type(stype)
    selfT.flags |= TypeDeclFlags isExplicit | TypeDeclFlags explicitConst
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
        blk.list |> emplace_new <|  qmacro(invoke($i("__arg_{fldname}"),$v(fldname),__self.$f(fldname)))
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
    fn.flags |= FunctionFlags generated | FunctionFlags privateFunction
    fn.result <- new [[TypeDecl() baseType=Type autoinfer, at=at]]
    var selfT <- clone_type(stype)
    selfT.flags |= TypeDeclFlags isExplicit | TypeDeclFlags explicitConst
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
        var vexpr <- qmacro_block <|
            if variant_index(__self)==$v(fldi)
                invoke($i("__arg_{fldname}"),$v(fldname),__self as $f(fldname))
                return
        blk.list |> emplace(vexpr)
    fn.body <- blk
    compiling_module() |> add_function(fn)


[call_macro(name="apply")]  // apply(value, block)
class ApplyMacro : AstCallMacro
    //! This macro implements the apply() pattern. The idea is that for each entry in the structure, variant, or tuple,
    //! the block will be invoked. Both element name, and element value are passed to the block.
    //! For example
    //!
    //!     struct Bar
	//!         x, y : float
	//!     apply([[Bar x=1.,y=2.]]) <| $ ( name:string; field )
    //!         print("{name} = {field} ")
    //!
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
