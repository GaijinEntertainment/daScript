require fio
require strings
require rtti
require debugapi
require math

var failed = 0

[no_jit]
def compile_and_run ( fileName:string; useAot:bool; useJit:bool )
    var t0 = ref_time_ticks()
    var inscope access <- make_file_access("")
    using <| $(var mg:ModuleGroup)
        using <| $(var cop:CodeOfPolicies)
            cop.aot_module = true
            cop.threadlock_context = true
            if useJit
                print("JIT: ")
                cop.jit = jit_enabled()
                cop.jit_module := "{get_das_root()}/daslib/just_in_time.das"
            elif useAot
                print("AOT: ")
                cop.aot = true
            else
                print("INTERPRETED: ")
                cop.jit = false
                cop.aot = false
            compile_file(fileName,access,unsafe(addr(mg)),cop) <| $(ok,program,issues)
                if !ok
                    print("failed to compile {fileName}\n{issues}\n")
                    failed ++
                    return
                simulate(program) <| $ ( sok; context; serrors ) // no aot for now
                    if !sok
                        print("failed to simulate {fileName}\n{serrors}\n")
                        failed ++
                        return
                    var dt = get_time_usec(t0)
                    var sec = double(dt)/1000000.0lf
                    print("compiled in {sec} sec\n")
                    try
                        unsafe(invoke_in_context(context,"main"))
                    recover
                        let ctx = unsafe(reinterpret<Context?> context)
                        print("failed to run {fileName}\n{ctx.exceptionAt}: {ctx.exception}\n")
                        failed ++


[no_jit]
def test_single_file ( fileName:string )
    print("{fileName}\n")
    compile_and_run(fileName, false, false)
    compile_and_run(fileName, true, false)
    compile_and_run(fileName, false, true)
    print("\n\n")

[no_jit]
def run_dir ( appDir : string )
    dir(appDir) <| $ ( fileName )
        if (!fileName |> starts_with("_")) && fileName |> ends_with(".das")
            let appfile = "{appDir}/{fileName}"
            test_single_file(appfile)

[export,no_jit]
def main
    print("RUNNING TESTS...\n\n")

    if false // uncomment for single test
        let appfile = "{get_das_root()}/examples/profile/tests/annotation.das"
        test_single_file(appfile)
        return

    run_dir("{get_das_root()}/examples/profile/hash_test")
    run_dir("{get_das_root()}/examples/profile/tests")

    if failed != 0
        print("TOTAL FAILURES: {failed}\n")
    else
        print("ALL TESTS PASSED\n")


