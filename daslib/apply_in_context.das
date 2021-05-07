options indenting = 2
options remove_unused_symbols = false
options no_unused_function_arguments = false
options no_aot = true
module apply_in_context shared

require ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/defer


[function_macro(name="apply_in_context")]
class AppendCondAnnotation : AstFunctionAnnotation
  def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
    if args.length != 1
      errors := "expecting one argument"
      return false

    var contextName = ""
    for argv in args
      let val = get_annotation_argument_value(argv)
      if val is tBool
        contextName = "{argv.name}"
      else
        errors := "invalid argument type {argv.name}"
        return false

    let resName = "__res__"
    let ctxCloneFnName = "CONTEXT_CLONE`{func.name}"
    let at = func.at
    let ctxFnName = "CONTEXT`{func.name}"
    var ctxFn <- new [[Function() at = at, atDecl = at, name := ctxFnName]]
    ctxFn.flags |= FunctionFlags generated
    ctxFn.flags |= FunctionFlags exports
    ctxFn.result <- new [[TypeDecl() baseType=Type tVoid, at=at]]

    var ctxQblock <- quote() <|
      unsafe
        verify(has_debug_agent_context(CONTEXT_NAME))
        verify(addr(get_debug_agent_context(CONTEXT_NAME)) == addr(this_context()))

    var rules : Template
    defer_delete(rules)
    rules |> replaceVariable("CONTEXT_NAME") <| new [[ExprConstString() at = at, value := contextName]]
    apply_template(rules, at, ctxQblock)
    unsafe
      ctxFn.body <- move_unquote_block(ctxQblock)

    var ctxFnBlock: ExprBlock?
    unsafe
      ctxFnBlock = reinterpret<ExprBlock?> ctxFn.body
    ctxFnBlock.blockFlags ^= ExprBlockFlags isClosure

    var callInCtx <- new [[ExprCall() at=at, name:=ctxCloneFnName]]

    for arg in func.arguments
      ctxFn.arguments |> emplace_new <| clone_variable(arg)
      callInCtx.arguments |> emplace_new <| new [[ExprVar() at=at, name:=arg.name]]

    var resType <- new [[TypeDecl() baseType=Type tPointer, firstType<-clone_type(func.result)]]
    ctxFn.arguments |> emplace_new <| new [[Variable() at=at, _type<-resType, name:=resName]]

    var ctxExprDeref <- new [[ExprPtr2Ref() at=at, subexpr <- new [[ExprVar() at=at, name:=resName]] ]]
    var ctxMovOp <- new [[ExprMove() at=at, left<-ctxExprDeref, right<-callInCtx]]
    ctxFnBlock.list |> emplace(ctxMovOp)

    compiling_module() |> add_function(ctxFn)


    var fn <- clone_function(func)
    fn.name := ctxCloneFnName
    fn.flags |= FunctionFlags generated
    fn.flags |= FunctionFlags privateFunction
    fn.flags |= FunctionFlags exports
    compiling_module() |> add_function(fn)

    func.body := null

    var qblock <- quote() <|
      unsafe
        verify(has_debug_agent_context(CONTEXT_NAME))
        verify(addr(get_debug_agent_context(CONTEXT_NAME)) != addr(this_context()))

    var rules2 : Template
    defer_delete(rules2)
    rules2 |> replaceVariable("CONTEXT_NAME") <| new [[ExprConstString() at = at, value := contextName]]
    apply_template(rules2, at, qblock)

    unsafe
      func.body <- move_unquote_block(qblock)

    var funcBlock: ExprBlock?
    unsafe
      funcBlock = reinterpret<ExprBlock?> func.body
    funcBlock.blockFlags ^= ExprBlockFlags isClosure

    var vlet <- new [[ExprLet() at=at, atInit=at ]]
    vlet.variables |> emplace_new() <| new [[Variable() at = at,
      name := resName,
      _type <- clone_type(func.result)
    ]]

    funcBlock.list |> emplace(vlet)

    var pinvoke <- new [[ExprCall() at=at, name:="invoke_in_context"]]
    pinvoke.genFlags |= ExprGenFlags alwaysSafe
    var getCtx <- new [[ExprCall() at=at, name:="get_debug_agent_context"]]
    getCtx.arguments |> emplace_new <| new [[ExprConstString() at=at, value:=contextName]]
    pinvoke.arguments |> emplace(getCtx)
    pinvoke.arguments |> emplace_new <| new [[ExprConstString() at=at, value:=ctxFnName]]
    for arg in func.arguments
      pinvoke.arguments |> emplace_new <| new [[ExprVar() at=at, name:=arg.name]]
    var resAddr <- new [[ExprRef2Ptr() at=at, subexpr<-new [[ExprVar() at=at, name:=resName]] ]]
    resAddr.genFlags |= ExprGenFlags alwaysSafe
    pinvoke.arguments |> emplace_new <| resAddr

    funcBlock.list |> emplace(pinvoke)

    funcBlock.list |> emplace_new() <| new [[ExprReturn() at=at, subexpr<-new [[ExprVar() at=at, name:=resName]] ]]

    return true