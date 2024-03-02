require fio

[no_jit,no_aot]
def cpp_label
    print("\"C++\", ")

[no_jit,no_aot]
def is_cpp_time
    return !jit_enabled() && !aot_enabled()

struct TestFile
    luaFile : string // lua
    luaJitFile : string // lua jit
    nutFile : string // squirrel
    nutModifiedFile : string // squirrel modified
    asFile  : string // angel script
    jsFile  : string // javascript
    monoFile  : string // csharp

[no_jit,no_aot]
def run_and_gun ( lang,cmdLine:string )
    unsafe
        print("\"{lang}\", ")
        let exitCode = popen(cmdLine) <| $ ( f )
            while !feof(f)
                let st = fgets(f)
                print(st)
        if exitCode != 0
            print("Error: {exitCode}\n")

[no_jit,no_aot]
def run_and_gun_silent ( cmdLine:string )
    unsafe
        var problem = ""
        var exitCode = popen(cmdLine) <| $ ( f )
            while !feof(f)
                let st = fgets(f)
                problem = "{problem}\n{st}"
        if exitCode != 0
            print("Error: {exitCode}\n{problem}\n")
            return

[no_jit,no_aot]
def run_files ( tf:TestFile )
    if !empty(tf.jsFile)
        // javascript
        let jsRoot = "{get_das_root()}/examples/profile/tests/js"
        let jsCompiler = "{jsRoot}/qjs.exe"
        let cwd = getcwd()
        chdir(jsRoot)
        run_and_gun("QUICKJS","{jsCompiler} {jsRoot}/{tf.jsFile}")
        chdir(cwd)
    if !empty(tf.monoFile)
        // csharp
        let monoRoot = "{get_das_root()}/examples/profile/tests/cs"
        let monoCompiler = "mcs"
        let monoRunner = "mono"
        let cwd = getcwd()
        chdir(monoRoot)
        run_and_gun_silent("{monoCompiler} -optimize+ {monoRoot}/{tf.monoFile} -out:test.exe")
        run_and_gun("MONO --interpreter","{monoRunner} --interpreter test.exe")
        run_and_gun("MONO","{monoRunner} test.exe")
        run_and_gun(".NET","test.exe")
        chdir(cwd)
    if !empty(tf.luaFile)
        // lua
        let luaRoot = "{get_das_root()}/examples/profile/tests/lua"
        let luaCompiler = "{luaRoot}/lua.exe"
        let cwd = getcwd()
        chdir(luaRoot)
        run_and_gun("LUA","{luaCompiler} {luaRoot}/{tf.luaFile}")
        chdir(cwd)
    if !empty(tf.luaJitFile)
        // lua
        let luaRoot = "{get_das_root()}/examples/profile/tests/lua"
        let luaJitCompiler = "{luaRoot}/luajit2_1_0beta3/luajit.exe"
        let cwd = getcwd()
        chdir(luaRoot)
        run_and_gun("LUAJIT -joff","{luaJitCompiler} -joff {luaRoot}/{tf.luaJitFile}")
        run_and_gun("LUAJIT","{luaJitCompiler} {luaRoot}/{tf.luaJitFile}")
        chdir(cwd)
    if !empty(tf.nutFile)
        // squirrel
        let nutRoot = "{get_das_root()}/examples/profile/tests/squirrel"
        let nutCompiler = "{nutRoot}/sq3-64.exe"
        let cwd = getcwd()
        chdir(nutRoot)
        run_and_gun("SQ3-64","{nutCompiler} {nutRoot}/{tf.nutFile}")
        chdir(cwd)
    if !empty(tf.nutModifiedFile)
        // squirrel
        let nutRoot = "{get_das_root()}/examples/profile/tests/squirrel"
        let numModifiedCompiler = "{nutRoot}/sq3-modified-64.exe"
        let cwd = getcwd()
        chdir(nutRoot)
        run_and_gun("SQ3-MODIFIED-64","{numModifiedCompiler} {nutRoot}/{tf.nutModifiedFile}")
        chdir(cwd)
    if !empty(tf.asFile)
        // angel script
        let asRoot = "{get_das_root()}/examples/profile/tests/angelscript"
        let asCompiler = "{asRoot}/as.exe"
        let asJitCompiler = "{asRoot}/asjit.exe"
        let cwd = getcwd()
        chdir(asRoot)
        run_and_gun("ANGELSCRIPT","{asCompiler} {asRoot}/{tf.asFile}")
        run_and_gun("ANGELSCRIPT-JIT","{asJitCompiler} {asRoot}/{tf.asFile}")
        chdir(cwd)
