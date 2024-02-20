require fio
require strings
require rtti
require debugapi

[export]
def main
    let appDir = "{get_das_root()}/examples/profile/hash_test"
    var failed = 0
    dir(appDir) <| $ ( fileName )
        if fileName |> starts_with("test")
            let appfile = "{appDir}/{fileName}"
            var inscope access <- make_file_access("")
            using <| $(var mg:ModuleGroup)
                using <| $(var cop:CodeOfPolicies)
                    cop.aot_module = true
                    cop.threadlock_context = true
                    cop.jit = jit_enabled()
                    cop.jit_module := "{get_das_root()}/daslib/just_in_time.das"
                    compile_file(appfile,access,unsafe(addr(mg)),cop) <| $(ok,program,issues)
                        if !ok
                            print("failed to compile {fileName}\n{issues}\n")
                            failed ++
                            return
                        simulate(program) <| $ ( sok; context; serrors ) // no aot for now
                            if !sok
                                print("failed to simulate {fileName}\n{serrors}\n")
                                failed ++
                                return
                            try
                                unsafe(invoke_in_context(context,"main"))
                            recover
                                let ctx = unsafe(reinterpret<Context?> context)
                                print("failed to run {fileName}\n{ctx.exceptionAt}: {ctx.exception}\n")
                                failed ++
