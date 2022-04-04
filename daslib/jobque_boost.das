options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module jobque_boost shared public

require jobque public

require daslib/rtti
require daslib/ast
require daslib/ast_boost
require daslib/templates

[tag_function(new_job_tag)]
def new_job ( var l : lambda )
    invoke(l)   // note, this is never called if job-que is there

[tag_function(new_job_tag)]
def new_thread ( var l : lambda )
    invoke(l)   // note, this is never called if job-que is there

[tag_function_macro(tag="new_job_tag")]
class private NewJobMacro : AstFunctionAnnotation
    //! this macro handles `new_job` and `new_thread` calls.
    //! the call is replaced with `new_job_invoke` and `new_thread_invoke` accordingly.
    //! a cloning infastructure is generated for the lambda, which is invoked in the new context.
    def override transform ( var call : smart_ptr<ExprCallFunc>; var errors : das_string ) : ExpressionPtr
        if !call.arguments[0] is ExprAscend
            compiling_program() |>macro_error(call.at,"expecting lambda declaration, ExprAscend")
            return [[ExpressionPtr]]
        var asc = call.arguments[0] as ExprAscend
        if !asc.subexpr is ExprMakeStruct
            compiling_program() |>macro_error(call.at,"expecting lambda declaration, ExprMakeStruct")
            return [[ExpressionPtr]]
        var mks = asc.subexpr as ExprMakeStruct
        if mks._type==null || mks._type.baseType!=Type tStructure
            compiling_program() |>macro_error(call.at,"expecting lambda declaration, not a structure")
            return [[ExpressionPtr]]
        // clone structure type. make fields non-constant, so that they can be cloned
        var stype <- clone_structure(mks._type.structType)
        stype.name := "{stype.name}_new_job_clone"
        var stype_ptr = get_ptr(stype)
        var sttype <- new [[TypeDecl() at=call.at, baseType=Type tStructure, structType = stype_ptr]]
        for fld in stype.fields // TODO: verify field type here
            fld._type.flags &= ~(TypeDeclFlags constant)
            fld.flags &= ~(FieldDeclarationFlags capturedConstant)
        var pclone <- make_clone_structure(stype_ptr)
        compiling_module() |> add_function(pclone)
        compiling_module() |> add_structure(stype)
        // make an @@<function<(var L;L):void> type
        var ftype <- new [[TypeDecl() at=call.at, baseType=Type tFunction ]]
        ftype.firstType <- new [[TypeDecl() at=call.at, baseType=Type tVoid]]
        ftype.argTypes |> emplace_new <| clone_type(sttype)
        ftype.argTypes |> emplace <| sttype
        ftype.argTypes[1].flags |= TypeDeclFlags constant
        // make a new_job_invoke call
        var ncall <- new [[ExprCall() at=call.at, name:="{call.name}_invoke"]]
        ncall.arguments |> emplace_new <| clone_expression(call.arguments[0])
        ncall.arguments |> emplace_new <| new [[ExprAddr() at=call.at, target:="clone", funcType <- ftype]]
        ncall.arguments |> emplace_new <| new [[ExprConstInt() at=call.at, value=int(mks._type.sizeOf)]]
        return <- ncall

def for_each ( channel:Channel?; blk:block<(res:auto(TT)#):void> )
    while true
        let void_data = _builtin_channel_pop(channel)
        if void_data==null
            break
        unsafe
            let typed_data = reinterpret<TT?#> void_data
            invoke ( blk, *typed_data )

def push_clone ( channel:Channel?; data : auto(TT) )
    var heap_data = new TT
    *heap_data := data
    _builtin_channel_push(channel, heap_data)

def push ( channel:Channel?; data : auto? )
    _builtin_channel_push(channel, data)

[template (tinfo)]
def each ( channel:Channel?; tinfo : auto(TT) )
    unsafe
        return <- generator<TT&#> () <| $ ()
            while true
                let void_data = _builtin_channel_pop(channel)
                if void_data==null
                    return false
                unsafe
                    yield * reinterpret<TT?#> void_data
            return false

