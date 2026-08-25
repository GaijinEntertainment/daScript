options gen2
options indenting = 4

require dastest/testing_boost public

require strings
require daslib/fio

// argv[0] is the running interpreter; dastest is invoked as
// `daslang(.exe) dastest/dastest.das ...`, so argv[0] is the daslang binary to spawn.
def das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}


def run_argv(args : array<string>; var output : string&) : int {
    return unsafe(popen_argv(args, 300.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
}


// A child that dies takes its diagnosis with it unless the parent prints what it said - the
// exit code alone turns a one-line answer into an exit-code hunt. Echo the child's output on
// any failure, and only then.
def report_child(t : T?; phase : string; rc : int; out : string; marker : string) : bool {
    let ok = rc == 0 && find(out, marker) >= 0
    if (!ok) {
        t |> failure("{phase} child: rc={rc}, marker '{marker}' {find(out, marker) >= 0 ? "found" : "MISSING"}")
        t |> failure("{phase} child output follows:\n{out}")
    }
    return ok
}


// A generic instance restored from the module cache must keep its origin generic
// (Function::fromGeneric). The child host restores its own module graph from the cache and
// then compiles a fresh program in the same process - a null origin on any restored
// instance disables generic-instance call recovery library-wide, and the fixture stops
// compiling on the warm run with error[30341] while the cold run is fine.
[test]
def test_generic_instance_origin(t : T?) {
    var terr : string
    let tmp = create_temp_directory("das_mc_origin", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let root = get_das_root()
    let argv <- [das_exe(), "-dasroot", root, "-module-cache", "{tmp}/mc.bin",
        "{root}/dastest/dastest.das", "--", "--test",
        "{root}/tests/module_cache/_fixtures/mc_generic_origin_drv.das"]
    var cold : string
    let coldRc = run_argv(argv, cold)
    let coldOk = report_child(t, "cold", coldRc, cold, "ser: wrote")
    t |> success(coldOk, "cold run wrote the cache and passed")
    var warm : string
    let warmRc = run_argv(argv, warm)
    // the verdict guards the vacuous pass: a cache that silently fell back to parsing would
    // pass this test without ever restoring a module
    var warmOk = report_child(t, "warm", warmRc, warm, "deser: clean")
    if (find(warm, "30341") >= 0) {
        t |> failure("warm run lost generic-instance recovery (error[30341])")
        warmOk = false
    }
    t |> success(warmOk, "warm run served every module from the cache and passed")
    var err : string
    rmdir_rec(tmp, err)
}
