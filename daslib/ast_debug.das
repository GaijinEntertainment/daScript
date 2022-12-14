options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true

module ast_debug shared private

require debugapi
require daslib/ast_boost
require daslib/strings

def report_to_debugger ( var ctx:Context; category,name:string; value:auto(TT) )
    let tinfo = typeinfo(rtti_typeinfo value)
    let pdata = unsafe(addr(value))
    report_context_state(ctx, category, name, unsafe(addr(tinfo)), pdata)

def isExpressionType ( _vinfo )
    if _vinfo.basicType != Type tPointer || _vinfo.firstType == null
        return false
    let vinfo = _vinfo.firstType
    if vinfo.annotation._module.name!="ast"
        return false
    if !string(vinfo.annotation.name) |> starts_with("Expr")
        return false
    return true

class SampleStackWalker : DapiStackWalker
    ctxid : Context?
    def override onArgument ( info:FuncInfo; index:int; vinfo:VarInfo; arg:float4 ) : void
        if !isExpressionType(vinfo)
            return
        unsafe
            let pexpression = reinterpret<ExpressionPtr?> addr(arg)
            if *pexpression != null
                report_to_debugger(*ctxid, "ast_debug", vinfo.name, describe(*pexpression))
    def override onVariable ( inf:FuncInfo; vinfo:LocalVariableInfo; arg:void?; inScope:bool ) : void
        if !isExpressionType(vinfo)
            return
        unsafe
            let pexpression = reinterpret<ExpressionPtr?> arg
            if *pexpression != null
                report_to_debugger(*ctxid, "ast_debug", vinfo.name, describe(*pexpression))
    def override onAfterCall ( pp:Prologue ) : bool
        return false    // only stack top

class ContextStateAgent : DapiDebugAgent
    walker_adapter : smart_ptr<StackWalker>
    walker : SampleStackWalker?
    def ContextStateAgent
        walker = new SampleStackWalker()
        unsafe
            walker_adapter <- make_stack_walker(walker)
    def operator delete
        unsafe
            delete walker_adapter
            delete walker
    def override onCollect(var ctx:Context; at:LineInfo)
        walker.ctxid = unsafe(addr(ctx))
        walk_stack(walker_adapter,ctx,at)
    def override onBreakpoint ( var ctx:Context; at:LineInfo; reason,text:string ) : void
        walker.ctxid = unsafe(addr(ctx))
        walk_stack(walker_adapter,ctx,at)
    def override onSingleStep ( var ctx:Context; at:LineInfo ) : void
        walker.ctxid = unsafe(addr(ctx))
        walk_stack(walker_adapter,ctx,at)


def decs_state_agent ( ctx : Context )
    install_new_debug_agent(new ContextStateAgent(), "ast_debug")

[_macro]
def private auto_start_decs_state()
    if is_compiling_macros_in_module("ast_debug")
        if !is_in_debug_agent_creation()
            if !has_debug_agent_context("ast_debug")
                fork_debug_agent_context(@@decs_state_agent)