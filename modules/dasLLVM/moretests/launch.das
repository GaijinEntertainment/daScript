options gen2
require fio
require strings
require rtti
require debugapi

[export]
def main {
    let testDir = "{get_das_root()}/examples/test/unit_tests"
    print("looking at {testDir}\n")
    var total = 0
    var failed = 0
    dir(testDir) <| $(fname) {
        if (!ends_with(fname, ".das") || starts_with(fname, "_")) {
            return
        }
        total ++
        let fileName = "{testDir}/{fname}"
        var inscope access <- make_file_access("")
        using <| $(var mg : ModuleGroup) {
            using <| $(var cop : CodeOfPolicies) {
                cop.threadlock_context = true
                cop.jit = true
                compile_file(fileName, access, unsafe(addr(mg)), cop) <| $(ok, program, issues) {
                    if (!ok) {
                        print("failed to compile {fileName}\n{issues}\n")
                        failed ++
                        return
                    }
                    simulate(program) <| $(sok; context; serrors) {   // no aot for now
                        if (!sok) {
                            print("failed to simulate {fileName}\n{serrors}\n")
                            failed ++
                            return
                        }
                        try {
                            unsafe(invoke_in_context(context, "test"))
                        } recover {
                            let ctx = unsafe(reinterpret<Context?> context)
                            print("failed to run {fileName}\n{ctx.exceptionAt}: {ctx.exception}\n")
                            failed ++
                        }
                    }
                }
            }
        }
    }
    if (failed != 0) {
        print("{failed} OF {total} TESTS FAILED!!!\n")
    } else {
        print("TESTS PASSED\n")
    }
}
