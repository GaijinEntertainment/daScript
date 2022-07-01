options indenting = 2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module apply_in_context shared private

require ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/defer
require daslib/strings_boost

require rtti public
require debugapi public

[function_macro(name="apply_in_context")]
class AppendCondAnnotation : AstFunctionAnnotation
  //! [apply_in_context] function annotation.
  //! Function is modified, so that it is called in the debug agent context, specified in the annotation.
  //! If specified context is not insalled, panic is called.
  //!
  //! For example::
  //!  [apply_in_context(opengl_cache)]
  //!  def public cache_font(name:string implicit) : Font?
  //!      ...
  //!  ...
  //!  let font = cache_font("Arial") // call invoked in the "opengl_cache" debug agent context
  def override patch ( var func:FunctionPtr; var group:ModuleGroup; args,progArgs:AnnotationArgumentList; var errors : das_string; var astChanged:bool& ) : bool

    for ann in func.annotations
      if ann.annotation.name=="apply_in_context"
        if !(find_arg("patched",ann.arguments) is nothing)
          return true

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

    for arg in func.arguments
      if is_temp_type(arg._type,true) && !(arg._type.flags.temporary || arg._type.flags._implicit)
        errors := "argument {arg.name} needs to be temporary or implicit, i.e. {describe(arg._type)}# or {describe(arg._type)} implicit"
        return false

    for ann in func.annotations
      if ann.annotation.name=="apply_in_context"
        astChanged = true
        ann.arguments |> add_annotation_argument("patched",true)

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
        verify(has_debug_agent_context(CONTEXT_NAME),"debug agent is not installed")
        verify(addr(get_debug_agent_context(CONTEXT_NAME)) == addr(this_context()),"agent context mismatch")

    apply_template(at, ctxQblock) <| $ ( rules )
      rules |> replaceVariable("CONTEXT_NAME") <| new [[ExprConstString() at = at, value := contextName]]
    unsafe
      ctxFn.body <- move_unquote_block(ctxQblock)

    var ctxFnBlock = ctxFn.body as ExprBlock
    ctxFnBlock.blockFlags ^= ExprBlockFlags isClosure

    var callInCtx <- new [[ExprCall() at=at, name:=ctxCloneFnName]]

    for arg in func.arguments
      ctxFn.arguments |> emplace_new <| clone_variable(arg)
      callInCtx.arguments |> emplace_new <| new [[ExprVar() at=at, name:=arg.name]]

    if !func.result.isVoid
      var resType <- new [[TypeDecl() baseType=Type tPointer, firstType<-clone_type(func.result)]]
      ctxFn.arguments |> emplace_new <| new [[Variable() at=at, _type<-resType, name:=resName]]
      var ctxExprDeref <- new [[ExprPtr2Ref() at=at, subexpr <- new [[ExprVar() at=at, name:=resName]] ]]
      var ctxMovOp <- new [[ExprCopy() at=at, left<-ctxExprDeref, right<-callInCtx]]
      ctxFnBlock.list |> emplace(ctxMovOp)
    else
      ctxFnBlock.list |> emplace(callInCtx)

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
        verify(has_debug_agent_context(CONTEXT_NAME),"debug agent is not installed")
        verify(addr(get_debug_agent_context(CONTEXT_NAME)) != addr(this_context()),"agent context mismatch")

    apply_template(at, qblock) <| $ ( rules2 )
      rules2 |> replaceVariable("CONTEXT_NAME") <| new [[ExprConstString() at = at, value := contextName]]

    unsafe
      func.body <- move_unquote_block(qblock)

    var funcBlock: ExprBlock?
    unsafe
      funcBlock = reinterpret<ExprBlock?> func.body
    funcBlock.blockFlags ^= ExprBlockFlags isClosure

    if !func.result.isVoid
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
    pinvoke.arguments |> emplace_new <| new [[ExprAddr() at=at, target:=ctxFnName]]
    for arg in func.arguments
      if arg._type.isRef && !arg._type.isRefType
        pinvoke.arguments |> emplace_new <| new [[ExprRef2Ptr() at=at,
          subexpr <- new [[ExprVar() at=at, name:=arg.name]],
          genFlags = ExprGenFlags alwaysSafe
        ]]
      else
        pinvoke.arguments |> emplace_new <| qmacro($i(arg.name))
    if !func.result.isVoid
      pinvoke.arguments |> emplace_new <| qmacro(unsafe(addr($i(resName))))

    funcBlock.list |> emplace(pinvoke)

    if !func.result.isVoid
      funcBlock.list |> emplace_new() <| qmacro_expr(${return $i(resName);}) // new [[ExprReturn() at=at, subexpr<-new [[ExprVar() at=at, name:=resName]] ]]

    return true
