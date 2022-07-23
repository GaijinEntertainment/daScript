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
require daslib/macro_boost

[tag_function(new_job_tag)]
def new_job ( var l : lambda )
    //! Create a new job.
    //!     * new context is cloned from the current context.
    //!     * lambda is cloned to the new context.
    //!     * new job is added to the job queue.
    //!     * once new job is invoked, lambda is invoked on the new context on the job thread.
    invoke(l)   // note, this is never called if job-que is there.

[tag_function(new_job_tag)]
def new_thread ( var l : lambda )
    //! Create a new thread
    //!     * new context is cloned from the current context.
    //!     * lambda is cloned to the new context.
    //!     * new thread is created.
    //!     * lambda is invoked on the new context on the new thread.
    invoke(l)   // note, this is never called if job-que is there

[tag_function_macro(tag="new_job_tag")]
class private NewJobMacro : AstFunctionAnnotation
    //! this macro handles `new_job` and `new_thread` calls.
    //! the call is replaced with `new_job_invoke` and `new_thread_invoke` accordingly.
    //! a cloning infastructure is generated for the lambda, which is invoked in the new context.
    def override transform ( var call : smart_ptr<ExprCallFunc>; var errors : das_string ) : ExpressionPtr
        macro_verify(call.arguments[0] is ExprAscend,compiling_program(),call.at,"expecting lambda declaration, ExprAscend")
        var asc = call.arguments[0] as ExprAscend
        macro_verify(asc.subexpr is ExprMakeStruct,compiling_program(),call.at,"expecting lambda declaration, ExprMakeStruct")
        var mks = asc.subexpr as ExprMakeStruct
        macro_verify(!(mks._type==null || mks._type.baseType!=Type tStructure),compiling_program(),call.at,"expecting lambda declaration, not a structure")
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
    //! reads input from the channel (in order it was pushed) and invokes the block on each input.
    //! stops once channel is depleted (internal entry counter is 0)
    //! this can happen on multiple threads or jobs at the same time.
    while true
        let void_data = _builtin_channel_pop(channel)
        if void_data==null
            break
        unsafe
            let typed_data = reinterpret<TT?#> void_data
            invoke ( blk, *typed_data )

def push_clone ( channel:Channel?; data : auto(TT) )
    //! clones data and pushed value to the channel (at the end)
    var heap_data = new TT
    *heap_data := data
    _builtin_channel_push(channel, heap_data)

def push ( channel:Channel?; data : auto? )
    //! pushes value to the channel (at the end)
    _builtin_channel_push(channel, data)

[template (tinfo)]
def each ( var channel:Channel?; tinfo : auto(TT) )
    //! this iterator is used to iterate over the channel in order it was pushed.
    //! iterator stops once channel is depleted (internal entry counter is 0)
    //! iteration can happen on multiple threads or jobs at the same time.
    unsafe
        return <- generator<TT&#> () <| $ ()
            while true
                let void_data = _builtin_channel_pop(channel)
                if void_data==null
                    break
                unsafe
                    yield * reinterpret<TT?#> void_data
            channel |> release
            return false

def public capture_jobque_channel ( ch:Channel? ) : Channel?
    //! this function is used to capture a channel that is used by the jobque.
    ch |> add_ref
    return ch

def public capture_jobque_job_status ( js:JobStatus? ) : JobStatus?
    //! this function is used to capture a job status that is used by the jobque.
    js |> add_ref
    return js

def public release_capture_jobque_channel ( ch:Channel? )
    //! this function is used to release a channel that is used by the jobque.
    if ch != null
        panic("Channel has not been released. missing channel|>release or channel|>notify_and_release")

def public release_capture_jobque_job_status ( js:JobStatus? )
    //! this function is used to release a job status that is used by the jobque.
    if js != null
        panic("JobStatus has not been released. missing status|>release or status|>notify_and_release")

[macro_function]
def private isPtrToJQ ( typ:TypeDeclPtr; jqt:string ) : bool
    if !typ.isPointer
        return false
    if typ.firstType==null
        return false
    if !typ.firstType.isHandle
        return false
    if typ.firstType.annotation._module.name!="jobque"
        return false
    if typ.firstType.annotation.name!=jqt
        return false
    return true

[capture_macro(name=channel_and_status_capture_macro)]
class private ChannelAndStatusCapture : AstCaptureMacro
    //! This macro implements capturing of the `jobque::Channel` and `jobque::JobStatus` types.
    //! When captured reference counts are increased. When lambda is destroyed, reference counts are decreased.
    def override captureExpression ( prog:Program?; mod:Module?; expr:ExpressionPtr; typ:TypeDeclPtr ) : ExpressionPtr
        //! Implementation details for the capture macro.
        if typ |> isPtrToJQ("Channel")
            var pCall <- new [[ExprCall() at=expr.at, name:="jobque_boost::capture_jobque_channel"]]
            pCall.arguments |> emplace_new <| clone_expression(expr)
            return pCall
        elif typ |> isPtrToJQ("JobStatus")
            var pCall <- new [[ExprCall() at=expr.at, name:="jobque_boost::capture_jobque_job_status"]]
            pCall.arguments |> emplace_new <| clone_expression(expr)
            return pCall
        return [[ExpressionPtr]]
    def override captureFunction ( prog:Program?; mod:Module?; var lcs:Structure?; var fun:FunctionPtr ) : void
        //! Implementation details for the capture macro.
        if fun.flags._generator // generator gets finally section called every single time. nothing we can do
            return
        for fld in lcs.fields
            if fld._type |> isPtrToJQ("Channel")
                var pCall <- new [[ExprCall() at=fld.at, name:="jobque_boost::release_capture_jobque_channel"]]
                pCall.arguments |> emplace_new <| new [[ExprField() at=fld.at,
                    value <- new [[ExprVar() at=fld.at, name:="__this"]],
                    name := fld.name,
                    derefFlags = ExprFieldDerefFlags ignoreCaptureConst
                ]]
                (fun.body as ExprBlock).finalList |> emplace(pCall)
            elif fld._type |> isPtrToJQ("JobStatus")
                var pCall <- new [[ExprCall() at=fld.at, name:="jobque_boost::release_capture_jobque_job_status"]]
                pCall.arguments |> emplace_new <| new [[ExprField() at=fld.at,
                    value <- new [[ExprVar() at=fld.at, name:="__this"]],
                    name := fld.name,
                    derefFlags = ExprFieldDerefFlags ignoreCaptureConst
                ]]
                (fun.body as ExprBlock).finalList |> emplace(pCall)

