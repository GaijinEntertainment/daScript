options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

options multiple_contexts

module live shared public

require daslib/rtti
require daslib/decs
require daslib/static_let
require debugapi
require fio

var private appPtr : smart_ptr<Context>
var private appFile = "app.das"
var private appDir  = get_das_root() + "/modules/dasGlfw/framework"
var private appLive = false

var private appTime : table<string;clock>
var private watchTime = get_clock()

var private liveFunctionLookup : table<string; bool>

var private liveContext : smart_ptr<Context>

def public get_live_context : smart_ptr<Context>
    return liveContext

[finalize]
def private go_offline
    appPtr := null

def public is_live
    return appLive

def public go_main
    appLive = false
    unsafe
        var ctx <- reinterpret<smart_ptr<Context>> addr(this_context())
        appPtr := ctx

def public go_live ( appf, appd : string )
    appLive = true
    this_context().name := "Live context"
    appFile = appf
    appDir = appd
    recompile()

def private live_lookup(fn)
    if appPtr==null
        return false
    if !liveFunctionLookup |> key_exists(fn)
        liveFunctionLookup[fn] = *appPtr |> has_function(fn)
    return liveFunctionLookup[fn]

def public invoke_live ( fn:string )
    if live_lookup(fn)
        try
            unsafe
                invoke_in_context(appPtr, fn)
        recover
            print("exception during '{fn}'\n{appPtr.last_exception}\n{describe(appPtr.exceptionAt)}\n")
            appPtr := null

def public invoke_live ( fn:string; a1 )
    if live_lookup(fn)
        try
            unsafe
                invoke_in_context(appPtr, fn, a1)
        recover
            print("exception during '{fn}'\n{appPtr.last_exception}\n{describe(appPtr.exceptionAt)}\n")
            appPtr := null

def public invoke_live ( fn:string; a1; a2 )
    if live_lookup(fn)
        try
            unsafe
                invoke_in_context(appPtr, fn, a1, a2)
        recover
            print("exception during '{fn}'\n{appPtr.last_exception}\n{describe(appPtr.exceptionAt)}\n")
            appPtr := null

def public invoke_live ( fn:string; a1; a2; a3 )
    if live_lookup(fn)
        try
            unsafe
                invoke_in_context(appPtr, fn, a1, a2, a3)
        recover
            print("exception during '{fn}'\n{appPtr.last_exception}\n{describe(appPtr.exceptionAt)}\n")
            appPtr := null

def public invoke_live ( fn:string; a1; a2; a3; a4 )
    if live_lookup(fn)
        try
            unsafe
                invoke_in_context(appPtr, fn, a1, a2, a3, a4)
        recover
            print("exception during '{fn}'\n{appPtr.last_exception}\n{describe(appPtr.exceptionAt)}\n")
            appPtr := null

def public watch_files
    var clk = get_clock()
    if (clk - watchTime) <= 0.
        return false
    watchTime = clk
    var any = false
    dir(appDir) <| $ (fname)
        let fileName = "{appDir}\/{fname}"
        var st : FStat
        if stat(fileName,st)
            if st.is_reg
                if appTime |> key_exists(fname)
                    if appTime[fname] != st.mtime
                        any = true
                appTime[fname] = st.mtime
    return any

def private set_new_context ( var ptr : smart_ptr<Context>; full_restart:bool = false )
    print("set new context {intptr(get_ptr(ptr))}\n")
    invoke_live("shutdown")
    if appPtr != null
        unsafe
            var pvar = get_context_global_variable(appPtr, "decsState")
            if pvar != null
                decsState := *(reinterpret<DecsState ?> pvar)
    appPtr := ptr
    delete liveFunctionLookup
    if appPtr != null
        appPtr.alwaysStackWalkOnException = true
        unsafe
            // fixup decs if its there
            var pvar = get_context_global_variable(appPtr, "decsState")
            if pvar != null
                var css <- @ <| [[&decsState]] ( state : DecsState? )
                    decsState := *state
                invoke_in_context(appPtr, css, addr(decsState))
                delete css
            // fix-up live context, if its there
            var pctx_var = get_context_global_variable(appPtr, "liveContext")
            if pctx_var != null
                var lContext <- reinterpret<smart_ptr<Context>> addr(this_context())
                var css <- @ <| [[&decsState]] ( state : DecsState? )
                    unsafe
                        liveContext <- lContext
                invoke_in_context(appPtr, css, addr(decsState))
                delete css
        invoke_live("initialize", full_restart)

def public recompile(full_restart:bool=false)
    var access := make_file_access("")
    using <| $(var mg:ModuleGroup#)
        using <| $(var cop:CodeOfPolicies#)
            compile_file("{appDir}/{appFile}",access,addr(mg),cop) <| $(ok,program,issues)
                if ok
                    simulate(program) <| $ ( sok; context; serrors )
                        if sok
                            // TODO: beep print("reloaded...\n")
                            set_new_context(context,full_restart)
                        else
                            print("{appFile} failed to simulate:\n{issues}\n")
                            set_new_context([[smart_ptr<Context>]],full_restart)
                else
                    print("{appFile} failed to compile:\n{issues}\n")
                    set_new_context([[smart_ptr<Context>]],full_restart)

def public live_keypressed ( keycode, scancode, action, mods : int )
    invoke_live("keypressed", keycode, scancode, action, mods )
    if is_live()
        if scancode==19 && action==1 && mods==2
            recompile(true)

def public live_charpressed ( charcode : uint )
    invoke_live("charpressed", charcode )

def public live_mousemoved ( x, y : double )
    invoke_live("mousemoved", x, y )

def public live_mouseenter ( yes : bool )
    invoke_live("mouseenter", yes)

def public live_mousebutton ( button, action, mods : int )
    invoke_live("mousebutton", button, action, mods )

def public live_mousescroll ( x, y : double )
    invoke_live("mousescroll", x, y )

def public live_update ( appTime : double )
    static_let <|
        var t = 0.0lf
    let dt = float(appTime - t)
    t = appTime
    invoke_live("act", dt)
