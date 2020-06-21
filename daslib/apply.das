module apply

require ast
require daslib/ast_boost

[tag_function(apply_tag)]
def apply(anything;visitor_block)
    pass

/*
    def apply`Foo(self:Foo;arg_field1:block<(value:field1-type):void>;arg_field2:...)
        invoke(arg_field1,self.field1)
        invoke(arg_field2,self.field2)
        ...
*/
def generateApplyVisitStruct ( stype:TypeDeclPtr; fnname,visitor:string; at:LineInfo )
    assert(stype.baseType==Type tStructure)
    assert(stype.dim.length==0)
    var fn <- new [[Function() at=at, atDecl=at, name:=fnname]]                     // def apply`foo : void
    fn.flags |= FunctionFlags generated
    fn.result <- new [[TypeDecl() baseType=Type tVoid]]
    push(fn.arguments, new [[Variable() name:="self",at=at, _type := clone_type(stype)]])   // self:Foo
    var blk <- new [[ExprBlock() at=at]]
    for fld in stype.structType.fields
        var cfield <- new [[ExprCall() at=at, name:=visitor]]                       // visitor()
        var argn <- new [[ExprConstString() at=at, value:=fld.name]]                // "name"
        push(cfield.arguments,argn)
        var argvar <- new [[ExprVar() at=at, name:="self"]]                         // self
        var argfld <- new [[ExprField() at=at, name:=fld.name, value <- argvar]]    // self.name
        push(cfield.arguments,argfld)
        push(blk.list, cfield)
    fn.body <- blk
    compiling_module() |> add_function(fn)

[tag_function_macro(tag="apply_tag")]
class ApplyFunctionMacro : AstFunctionAnnotation
    def override transform ( var call : smart_ptr<ExprCall>; var errors : das_string ) : ExpressionPtr
        if call.arguments[0]._type!=null & call.arguments[1]._type!=null
            if call.arguments[0]._type.dim.length != 0
                // TODO: transform to apply_dim
                errors := "can't apply to dim"
                return [[ExpressionPtr]]
            if call.arguments[0]._type.baseType != Type tStructure
                // TODO: support apply to tuple / variant / table / array
                errors := "can only apply to structure"
                return [[ExpressionPtr]]
            if !call.arguments[1] is ExprConstString
                errors := "can only apply fixed visitor name"
                return [[ExpressionPtr]]
            var visitor = ""
            unsafe
                let arg2 = reinterpret<ExprConstString?>(call.arguments[1])
                visitor = "{arg2.value}"
            var callName = "apply`{visitor}"
            generateApplyVisitStruct(call.arguments[0]._type,callName,visitor,call.at)
            // swap call "in place"
            call.name := callName
            pop(call.arguments)
            return <- call
        return [[ExpressionPtr]]



