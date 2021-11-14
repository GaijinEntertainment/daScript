options debugger
require daslib/debug

require rtti
require debugapi
require daslib/strings_boost
require daslib/algorithm

class EvalWalker : DapiStackWalker
    W : StringBuilderWriter?
    def EvalWalker ( var wr:StringBuilderWriter )
        W = unsafe(addr(wr))
    def override onCallAOT ( pp:Prologue; fileName:string# ) : void
        *W |> write("// AOT {fileName}\n")
    def override onCallAt ( pp:Prologue; info:FuncInfo; at:LineInfo ) : void
        *W |> write("// {info.name} at {describe(at)}\n")
    def override onCall ( pp:Prologue; info:FuncInfo ) : void
        *W |> write("// {info.name}\n")
    def override onArgument ( info:FuncInfo; index:int; vinfo:VarInfo; arg:float4 ) : void
        if vinfo.flags.refType
            let parg = unsafe(reinterpret<void?> arg)
            *W |> write("\t\tvar {vinfo.name} : {describe(type_info(vinfo))} & = * reinterpret<{describe(type_info(vinfo))}?> {parg}ul // ref argument\n")
        else
            *W |> write("\t\tvar {vinfo.name} : {describe(type_info(vinfo))} = ")
            if vinfo.basicType==Type tString
                *W |> write("\"")
                *W |> write(sprint_data(arg,type_info(vinfo),print_flags humanReadable))
                *W |> write("\"")
            else
                *W |> write(sprint_data(arg,type_info(vinfo),print_flags humanReadable))
            *W |> write("\n")
    def override onVariable ( inf:FuncInfo; vinfo:LocalVariableInfo; arg:void?; inScope:bool ) : void
        if inScope
            if arg != null
                if vinfo.flags.ref
                    unsafe
                        let pref = unsafe(reinterpret<void??> arg)
                        var cvinfo = vinfo
                        cvinfo.flags &= ~ TypeInfoFlags ref
                        *W |> write("\t\tvar {vinfo.name} : {describe(type_info(vinfo))} = * reinterpret<{describe(type_info(cvinfo))}?> {*pref}ul // already ref\n")
                else
                    *W |> write("\t\tvar {vinfo.name} : {describe(type_info(vinfo))} & = * reinterpret<{describe(type_info(vinfo))}?> {arg}ul\n")

def getStackVariableDefinitions ( var ctx:Context;at:LineInfo )
    let text = build_string <| $ ( st )
        var walker = new EvalWalker(st)
        var walker_adapter <- make_stack_walker(walker)
        walk_stack(walker_adapter,ctx,at)
        walker_adapter := null
        unsafe
            delete walker
    var lines <- split(text,"\n")
    reverse(lines)
    let res = join(lines,"\n")
    delete lines
    return res

def getGlobalVariableDefinitions ( var ctx:Context )
    let text = build_string <| $ ( st )
        let tot = get_total_variables(ctx)
        for vi in range(tot)
            let vinfo = unsafe(addr(get_variable_info(ctx,vi)))
            var arg = unsafe(get_context_global_variable(ctx,vinfo.name))
            if arg != null
                if vinfo.flags.ref
                    unsafe
                        let pref = unsafe(reinterpret<void??> arg)
                        var cvinfo = vinfo
                        cvinfo.flags &= ~ TypeInfoFlags ref
                        st |> write("\t\tvar {vinfo.name} : {describe(type_info(*vinfo))} = * reinterpret<{describe(type_info(*cvinfo))}?> {*pref}ul // already ref\n")
                else
                    st |> write("\t\tvar {vinfo.name} : {describe(type_info(*vinfo))} & = * reinterpret<{describe(type_info(*vinfo))}?> {arg}ul\n")

    return text

def eval ( expr:string )
    let vars = getStackVariableDefinitions(this_context(),get_line_info())
    let gvars = getGlobalVariableDefinitions(this_context())
    let text = build_string <| $ ( st )
        st |> write("require rtti\n")       // TODO: list of 'require' for this eval
        st |> write("require strings\n")
        st |> write("\n")
        st |> write("[export]\n")
        st |> write("def main\n")
        st |> write("\tunsafe\n")
        st |> write(gvars)
        st |> write(vars)
        st |> write("// EVAL:\n")
        st |> write("\t\tlet EVAL = {expr}\n")
        st |> write("\t\tprint(\"\{EVAL\}\\n\")\n")
        st |> write("\n")
    print(text)
    compileAndRun(text)

def compileAndRun ( text:string )
    var failed = true
    var cop = CodeOfPolicies()
    cop.allow_local_variable_shadowing = true
    compile("project",text, cop, false) <| $ ( ok; prog; errors )
        if ok
            simulate(prog) <| $ ( sok; context; serrors )
                if sok
                    unsafe
                        context |> invoke_in_context("main")
                    failed = false
                else
                    print("failed to simulate\n{serrors}\n")
        else
            print("failed to compile\n{errors}\n")
    return failed

var GLOB = 13

[export]
def main
    unsafe
        var LOC = 13
        LOC ++
        var GLOC & = LOC
        print("testing {GLOB}+{LOC}={GLOB+LOC} // {GLOC}\n")
        eval("GLOB + LOC")
