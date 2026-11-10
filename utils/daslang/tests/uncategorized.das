options gen2

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost
require _host_fixture

//! argv[0] is the running interpreter: dastest is launched as
//! `daslang dastest/dastest.das ...`
def das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}

[test(description = "An uncaught runtime exception in main ends the host with -2, apart from a compile error's 1", timeout = 1200)]
def test_panic_carries_its_own_code(t : T?) {
    t |> run("a panic out of main exits -2 (254 on POSIX) and prints EXCEPTION") @(t : T?) {
        let fixture = path_join(tests_dir(), "_fixture_panic_child.das")
        let res = run_daslang([das_exe(), "-dasroot", get_das_root(), "-no-module-cache", fixture])
        let expected = get_platform_name() == "windows" ? -2 : 254
        t |> equal(res.rc, expected, "the host's exit code, got:\n{res.out}")
        t |> success(res.out |> contains("EXCEPTION: a fixture panic"),
                     "the EXCEPTION line reached the parent, got:\n{res.out}")
    }
}

[test(description = "A GC COMPILE LEAK report ends the host with 1 even when main runs and returns 0", timeout = 1200)]
def test_compile_leak_holds_its_exit_code(t : T?) {
    t |> run("a gc_node left on the thread root at [init] exits 1 past a clean main") @(t : T?) {
        let fixture = path_join(tests_dir(), "_fixture_compile_leak_child.das")
        let res = run_daslang([das_exe(), "-dasroot", get_das_root(), "-no-module-cache", fixture])
        t |> equal(res.rc, 1, "the host's exit code, got:\n{res.out}")
        t |> success(res.out |> contains("GC COMPILE LEAK"),
                     "the compile-leak report reached the parent, got:\n{res.out}")
    }
}
