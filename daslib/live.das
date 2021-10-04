options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

module live shared public

require daslib/rtti
require debugapi
require fio

var private appPtr : smart_ptr<Context>
var private appFile = "app.das"
var private appDir  = get_das_root() + "/modules/dasGlfw/framework"

var private appTime : table<string;clock>
var private watchTime = get_clock()

var private liveFunctionLookup : table<string; bool>

def public is_live
    return appPtr != null

def public go_main
    unsafe
        var ctx <- reinterpret<smart_ptr<Context>> addr(this_context())
        appPtr <- ctx

def public go_live ( appf, appd : string )
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
            print("exception during '{fn}'\n{appPtr.exception}\n{describe(appPtr.exceptionAt)}\n")
            appPtr := null

def public invoke_live ( fn:string; a1 )
    if live_lookup(fn)
        try
            unsafe
                invoke_in_context(appPtr, fn, a1)
        recover
            print("exception during '{fn}'\n{appPtr.exception}\n{describe(appPtr.exceptionAt)}\n")
            appPtr := null

def public invoke_live ( fn:string; a1; a2 )
    if live_lookup(fn)
        try
            unsafe
                invoke_in_context(appPtr, fn, a1, a2)
        recover
            print("exception during '{fn}'\n{appPtr.exception}\n{describe(appPtr.exceptionAt)}\n")
            appPtr := null

def public invoke_live ( fn:string; a1; a2; a3 )
    if live_lookup(fn)
        try
            unsafe
                invoke_in_context(appPtr, fn, a1, a2, a3)
        recover
            print("exception during '{fn}'\n{appPtr.exception}\n{describe(appPtr.exceptionAt)}\n")
            appPtr := null

def public invoke_live ( fn:string; a1; a2; a3; a4 )
    if live_lookup(fn)
        try
            unsafe
                invoke_in_context(appPtr, fn, a1, a2, a3, a4)
        recover
            print("exception during '{fn}'\n{appPtr.exception}\n{describe(appPtr.exceptionAt)}\n")
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

def private set_new_context ( var ptr : smart_ptr<Context> )
    invoke_live("shutdown")
    appPtr := ptr
    delete liveFunctionLookup
    if appPtr != null
        appPtr.alwaysStackWalkOnException = true
    invoke_live("initialize")

def public recompile
    var access := make_file_access("")
    using <| $(var mg:ModuleGroup#)
        using <| $(var cop:CodeOfPolicies#)
            compile_file("{appDir}/{appFile}",access,addr(mg),cop) <| $(ok,program,issues)
                if ok
                    simulate(program,false) <| $ ( sok; context; serrors )
                        if sok
                            // TODO: beep print("reloaded...\n")
                            set_new_context(context)
                        else
                            print("{appFile} failed to simulate:\n{issues}\n")
                            appPtr := null
                else
                    print("{appFile} failed to compile:\n{issues}\n")
                    appPtr := null

def public live_keypressed ( keycode, scancode, action, mods : int )
    // controls_keypressed(....)
    invoke_live("keypressed", keycode, scancode, action, mods )
