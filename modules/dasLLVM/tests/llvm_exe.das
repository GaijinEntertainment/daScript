options gen2
require dastest/testing_boost public
require daslib/fio
require daslib/clargs
require daslib/env_registry
require daslib/strings_boost
require strings
require ../../../utils/daspkg/utils.das
require _exe_test_common

// A standalone exe registers its functions and globals one at a time and seals the lookup tables
// before its init script runs; by-name lookup - has_function, invoke by name, a global by name -
// then answers from the sealed tables, the same way it does in the interpreter.

let private NAME_LOOKUP_ROOT = "modules/dasLLVM/tests/_name_lookup_root.das"

def private expect_name_lookup_lines(t : T?; out : string; what : string) {
    t |> success(find(out, "HAS_FOO true") >= 0, "{what}: has_function finds an overloaded name: {tail(out)}")
    t |> success(find(out, "HAS_BUMP true") >= 0, "{what}: has_function finds a unique name: {tail(out)}")
    t |> success(find(out, "HAS_NOPE false") >= 0, "{what}: has_function misses an unknown name: {tail(out)}")
    t |> success(find(out, "BUMP 8") >= 0, "{what}: invoke by name reaches the unique function: {tail(out)}")
    t |> success(find(out, "G_COUNTER 8") >= 0, "{what}: a global resolves by name after the bump: {tail(out)}")
    t |> success(find(out, "G_NOPE true") >= 0, "{what}: an unknown global name answers null: {tail(out)}")
}

[test(description = "has_function, invoke by name and a global by name answer the same in a standalone exe as interpreted", timeout = 1200)]
def test_exe_name_lookup(t : T?) {
    let dir_r = create_temp_directory_result("llvm_exe_name_lookup")
    if (!(dir_r is value)) {
        t |> failure("temp dir failed")
        return
    }
    let dir = unsafe(dir_r.value)
    t |> run("the fixture answers the same by-name lookups interpreted and as a standalone exe") @(t : T?) {
        var hout = ""
        let hrc = run_and_capture([das_exe(), "-no-module-cache", NAME_LOOKUP_ROOT], hout, 300.0)
        t |> equal(0, hrc, "the fixture runs interpreted on the host: {tail(hout)}")
        expect_name_lookup_lines(t, hout, "interpreted")
        let out = build_and_run(t, dir, "lookup", NAME_LOOKUP_ROOT, [], [])
        expect_name_lookup_lines(t, out, "exe")
    }
    rmdir_rec_result(dir)
}

// A standalone exe carries the host's require-resolver rows (register_native_path) only when it
// links the whole compiler library - the resolver they feed lives there. A runtime-only exe has
// no compiler, so it registers none; a --jit-register-all-modules exe (the jit.exe shape,
// which compiles at run time) still carries every row.

let private NATIVE_PATHS_ROOT = "modules/dasLLVM/tests/_exe_native_paths_root.das"

def private native_path_rows(out : string) : int {
    let at = find(out, "NATIVE_PATHS ")
    if (at < 0) {
        return -1
    }
    let rest = slice(out, at + length("NATIVE_PATHS "))
    let eol = find(rest, "\n")
    return to_int(eol < 0 ? rest : slice(rest, 0, eol))
}

[test(description = "a runtime-only exe registers no require-resolver rows, and a --jit-register-all-modules exe registers the host's rows exactly once each", timeout = 1200)]
def test_exe_native_paths(t : T?) {
    let dir_r = create_temp_directory_result("llvm_exe_native_paths")
    if (!(dir_r is value)) {
        t |> failure("temp dir failed")
        return
    }
    let dir = unsafe(dir_r.value)
    t |> run("a runtime-only exe registers no require-resolver rows") @(t : T?) {
        let out = build_and_run(t, dir, "plain", NATIVE_PATHS_ROOT, [], [])
        t |> equal(0, native_path_rows(out), "no register_native_path row at startup: {tail(out)}")
    }
    t |> run("a whole-lib exe (--jit-register-all-modules) carries the host's rows, each once") @(t : T?) {
        let out = build_and_run(t, dir, "whole", NATIVE_PATHS_ROOT,
            ["--jit-register-all-modules"], [])
        var hout = ""
        let hrc = run_and_capture([das_exe(), "-no-module-cache", NATIVE_PATHS_ROOT], hout, 300.0)
        t |> equal(0, hrc, "the fixture runs interpreted on the host: {tail(hout)}")
        t |> success(native_path_rows(hout) > 0, "the host scan registered rows: {tail(hout)}")
        t |> equal(native_path_rows(hout), native_path_rows(out), "the exe registers the host's row count - two emission sites would double it: {tail(out)}")
    }
    rmdir_rec_result(dir)
}

[test(description = "a register-all exe started beside an unrelated descriptor whose module is missing still runs, and runs again once the descriptor is gone", timeout = 1200)]
def register_all_executable_keeps_unrelated_missing_modules_optional(t : T?) {
    let made = create_temp_directory_result("llvm_optional_module")
    if (!(made is value)) { t |> failure("could not create temporary SDK"); return }
    let root = made as value
    {
        let source = path_join(get_das_root(), "modules/dasLLVM/tests/_exe_optional_module_root.das")
        let output = path_join(root, "driver")
        var log : string
        let compiled = run_and_capture([get_executable_path(), "-no-module-cache", "-jit",
            "-jit-no-cache", "-exe", "-output", "{output}.exe", source, "--",
            "--jit-register-all-modules"], log, 180.0)
        t |> equal(compiled, 0, log)
        if (compiled != 0) return
        t |> success(copy_dir_rec(path_join(get_das_root(), "daslib"), path_join(root, "daslib")))
        let optional = path_join(root, "modules/OptionalAbsent")
        t |> success(mkdir_rec(optional))
        let copied = copy_file_result(path_join(get_das_root(), "modules/dasLLVM/tests/_exe_optional_descriptor.das"), path_join(optional, ".das_module"), false)
        t |> success(copied is value && copied as value)
        // The temporary executable still links the producing SDK's runtime DLLs.
        let path_separator = get_platform_name() == "windows" ? ";" : ":"
        let runtime_env <- ["PATH={dir_name(get_executable_path())}{path_separator}{env_value_of("PATH")}"]
        let first = run_and_capture(["{output}.exe"], root, runtime_env, log, 30.0)
        t |> equal(first, 0, "an unrelated Quiet descriptor must not abort a register-all tool: {log}")
        t |> success(find(log, "VALID") >= 0)
        rmdir_rec(optional)
        let control = run_and_capture(["{output}.exe"], root, runtime_env, log, 30.0)
        t |> equal(control, 0, log)
        t |> success(find(log, "VALID") >= 0)
    } finally { rmdir_rec_result(root) }
}

// A standalone exe whose main panics while a job is parked on a
// channel exits with the panic code -2: jit_run_main_guarded asks the
// que to quiesce and exits without the teardown that would join the
// parked worker forever. The fixture is the interpreter test's, under
// tests/jobque, built here as an exe.

[test(description = "a standalone exe whose main panics exits with the panic code -2, with a job parked on a channel (no teardown) and with nothing running (regular teardown) alike", timeout = 1200)]
def test_exe_panic_with_parked_job_exits(t : T?) {
    let dir_r = create_temp_directory_result("llvm_exe_panic_teardown")
    if (!(dir_r is value)) {
        t |> failure("temp dir failed")
        return
    }
    let dir = unsafe(dir_r.value)
    let panicCode = get_running_platform_name() == "windows" ? -2 : 254
    let root = "tests/jobque/_fixture_panic_parked_job.das"
    let exe = path_join(dir, "panic_teardown.exe")
    var bout = ""
    let brc = run_and_capture([das_exe(), "-jit", "-exe", "-output", exe, root], bout, 300.0)
    t |> equal(0, brc, "the -exe build succeeds: {tail(bout)}")
    if (brc == 0 && stat(exe).is_valid) {
        t |> run("a parked job: the exe prints the panic and exits -2 within the bound") @(t : T?) {
            var out = ""
            let rc = run_standalone(exe, "", out, 60.0)
            t |> equal(panicCode, rc, "the exe exits with the panic code: {tail(out)}")
            t |> success(find(out, "EXCEPTION: boom") >= 0, "the panic is printed: {tail(out)}")
            t |> success(find(out, "exiting without teardown") >= 0,
                         "and the host says why it skipped teardown: {tail(out)}")
        }
        t |> run("nothing running: the exe tears down and still exits -2") @(t : T?) {
            var out = ""
            let rc = run_standalone(exe, "-- clean", out, 60.0)
            t |> equal(panicCode, rc, "the exe exits with the panic code: {tail(out)}")
            t |> success(find(out, "EXCEPTION: boom") >= 0, "the panic is printed: {tail(out)}")
            t |> success(find(out, "exiting without teardown") < 0,
                         "the regular teardown ran: {tail(out)}")
        }
    }
    rmdir_rec_result(dir)
}

// A shared module's global initializer that takes a function's address (`@@ fn`) fills the
// same Func in a standalone exe as it does interpreted: the exe's startup walk over global
// initializers covers a promoted das module, so the address global the emitter left null is
// stored before the init script runs.

let private SHARED_ADDR_ROOT = "modules/dasLLVM/tests/_exe_shared_addr_root.das"

def private expect_shared_addr_lines(t : T?; out : string; what : string) {
    t |> success(find(out, "SHARED_ADDR_TABLE 11") >= 0, "{what}: the catalog's addresses dispatch through the table: {tail(out)}")
    t |> success(find(out, "SHARED_ADDR_DIRECT 21") >= 0, "{what}: the bare function-pointer global invokes: {tail(out)}")
}

[test(description = "function addresses a shared module's global initializers take dispatch the same in a standalone exe as interpreted", timeout = 1200)]
def test_exe_shared_module_function_address(t : T?) {
    let dir_r = create_temp_directory_result("llvm_exe_shared_addr")
    if (!(dir_r is value)) {
        t |> failure("temp dir failed")
        return
    }
    let dir = unsafe(dir_r.value)
    t |> run("a shared module's global function addresses answer the same interpreted and as a standalone exe") @(t : T?) {
        var hout = ""
        let hrc = run_and_capture([das_exe(), "-no-module-cache", SHARED_ADDR_ROOT], hout, 300.0)
        t |> equal(0, hrc, "the fixture runs interpreted on the host: {tail(hout)}")
        expect_shared_addr_lines(t, hout, "interpreted")
        let out = build_and_run(t, dir, "shared_addr", SHARED_ADDR_ROOT, [], [])
        expect_shared_addr_lines(t, out, "exe")
    }
    rmdir_rec_result(dir)
}

// A standalone exe built split (`--jit-split-modules`: one partition a das module, the entry in a
// partition of its own) and split-plus-LTO (`--jit-lto`: bitcode partitions, the link inlines across
// them) runs the fixture the way the one-unit exe does, and leaves no partition object beside itself.
// Off Windows the flags build also links the runtime archives in
// (`--jit-static`), which the link announces as an executable rather
// than a library.

let private SPLIT_LTO_ROOT = "modules/dasLLVM/tests/_exe_split_lto_root.das"

//! an empty `out` is a build or run that already failed above; its lines are not checked again
def private expect_split_lines(t : T?; out : string; tag : string) {
    if (empty(out)) return
    t |> success(find(out, "WORDS 3") >= 0, "{tag}: the strings_boost partition serves the split: {tail(out)}")
    t |> success(find(out, "MIX 4901") >= 0, "{tag}: the fixture's own partition folds the word lengths: {tail(out)}")
    t |> success(find(out, "SQRT 12") >= 0, "{tag}: the math partition answers: {tail(out)}")
    t |> success(find(out, "ADDR 42") >= 0, "{tag}: a function address taken in a body resolves through its partition's slot: {tail(out)}")
    t |> success(find(out, "TABLE 8") >= 0, "{tag}: the entry's global table literal reaches the accessor slot a partition collected first: {tail(out)}")
    t |> success(find(out, "VEC 211") >= 0, "{tag}: vector-constant literals at, past and below the constant-data floor read back whole: {tail(out)}")
}

//! a split build leaves its exe and the linker's map alone: the partition objects, bitcode and response file go with the link, and an exe that exports nothing gets no import library or export file
def private expect_no_partition_files(t : T?; dir : string; tag : string) {
    let link_only = [".o", ".bc", ".rsp", ".lib", ".exp"]
    var left : array<string>
    fio::dir(dir) $(name) {
        if (name |> starts_with("{tag}.")) {
            for (suffix in link_only) {
                if (name |> ends_with(suffix)) {
                    left |> push(name)
                }
            }
        }
    }
    t |> equal(0, length(left), "the split build ({tag}) removed its partition files and exported nothing: {join(left, ", ")}")
}

[test(description = "a split -exe (one partition per das module) runs the fixture like the one-unit exe and leaves no partition files beside it", timeout = 1200)]
def test_exe_split(t : T?) {
    let dir_r = create_temp_directory_result("llvm_exe_split")
    if (!(dir_r is value)) {
        t |> failure("temp dir failed")
        return
    }
    let dir = unsafe(dir_r.value)
    t |> run("a split -exe (one partition a das module, the entry in its own) runs the fixture like the one-unit exe") @(t : T?) {
        let one = build_and_run(t, dir, "one", SPLIT_LTO_ROOT, [], [])
        expect_split_lines(t, one, "one unit")
        let split = build_and_run(t, dir, "split", SPLIT_LTO_ROOT, ["--jit-split-modules=-1"],
            ["split codegen (--jit-split-modules)"])
        expect_split_lines(t, split, "split")
        expect_no_partition_files(t, dir, "split")
    }
    rmdir_rec_result(dir)
}

[test(description = "--jit-lto and --jit-obj-cache without the split announce that they are ignored, and the one-unit exe still runs", timeout = 1200)]
def test_exe_flags_outside_their_rail(t : T?) {
    let dir_r = create_temp_directory_result("llvm_exe_flags")
    if (!(dir_r is value)) {
        t |> failure("temp dir failed")
        return
    }
    let dir = unsafe(dir_r.value)
    t |> run("--jit-lto and --jit-obj-cache without the split say they are ignored, and the one-unit exe still runs") @(t : T?) {
        var jit_args <- ["--jit-lto", "--jit-obj-cache=1"]
        var says <- ["--jit-lto applies to a split -exe",
                     "--jit-obj-cache applies under --jit-split-modules"]
        if (get_platform_name() != "windows") {
            jit_args |> push("--jit-static")
            says |> push("Executable {path_join(dir, "flags")}.exe linked - ok")
        }
        let out = build_and_run(t, dir, "flags", SPLIT_LTO_ROOT, jit_args, says)
        expect_split_lines(t, out, "flags outside their rail")
    }
    rmdir_rec_result(dir)
}

[test(description = "the LTO driver gate reads a clang driver's major version, 0 for a non-clang driver, and the bundled LLVM's major from the CMake pin")]
def test_lto_driver_gate_parsing(t : T?) {
    t |> run("the LTO driver gate reads a driver's major and the bundled LLVM's on every platform") @(t : T?) {
        t |> equal(18, driver_major("Ubuntu clang version 18.1.3 (1ubuntu1)\nTarget: x86_64-pc-linux-gnu\n"))
        t |> equal(22, driver_major("clang version 22.1.5\n"))
        t |> equal(0, driver_major("g++ (GCC) 13.2.0\n"))
        t |> success(bundled_llvm_major() >= 16, "the CMake pin names the bundled LLVM's major: {bundled_llvm_major()}")
    }
}

[test(description = "a split -exe linked with LTO runs the fixture like the one-unit exe and leaves no partition files beside it", timeout = 1200)]
def test_exe_split_lto(t : T?) {
    let why_not = lto_driver_missing()
    if (!empty(why_not)) {
        t |> skip(why_not)
        return
    }
    let dir_r = create_temp_directory_result("llvm_exe_split_lto")
    if (!(dir_r is value)) {
        t |> failure("temp dir failed")
        return
    }
    let dir = unsafe(dir_r.value)
    t |> run("a split -exe linked with LTO runs the fixture like the one-unit exe") @(t : T?) {
        let out = build_and_run(t, dir, "lto", SPLIT_LTO_ROOT,
            ["--jit-split-modules=-1", "--jit-lto"], ["the link runs LTO"])
        expect_split_lines(t, out, "split + LTO")
        expect_no_partition_files(t, dir, "lto")
    }
    rmdir_rec_result(dir)
}

// A standalone exe registers its functions with no argument types, so a job or thread lambda's
// capture block used to get a null type in its header and the first collect the thread ran
// walked it: the exe hands its context the capture type of every lambda body at init, and the
// thread's collect keeps the captured array.

let private THREAD_COLLECT_ROOT = "modules/dasLLVM/tests/_exe_thread_collect_root.das"

def private expect_thread_collect_lines(t : T?; out : string; what : string) {
    t |> success(find(out, "THREAD_COLLECT_SUM 2016") >= 0, "{what}: the captured array survives the thread's collect: {tail(out)}")
    t |> success(find(out, "JOB_COLLECT_SUM 992") >= 0, "{what}: the captured array survives the job's collect: {tail(out)}")
    t |> success(find(out, "THREAD_COLLECT_DONE") >= 0, "{what}: the thread is joined: {tail(out)}")
}

[test(description = "an array a thread or job lambda captures survives the collect that thread runs, in a standalone exe as interpreted", timeout = 1200)]
def test_exe_thread_lambda_collect(t : T?) {
    let dir_r = create_temp_directory_result("llvm_exe_thread_collect")
    if (!(dir_r is value)) {
        t |> failure("temp dir failed")
        return
    }
    let dir = unsafe(dir_r.value)
    t |> run("a thread lambda's capture survives the collect it runs, interpreted and as a standalone exe") @(t : T?) {
        var hout = ""
        let hrc = run_and_capture([das_exe(), "-no-module-cache", THREAD_COLLECT_ROOT], hout, 300.0)
        t |> equal(0, hrc, "the fixture runs interpreted on the host: {tail(hout)}")
        expect_thread_collect_lines(t, hout, "interpreted")
        let out = build_and_run(t, dir, "thread_collect", THREAD_COLLECT_ROOT, [], [])
        expect_thread_collect_lines(t, out, "exe")
    }
    rmdir_rec_result(dir)
}

[test(description = "a thread loop collects between its steps and keeps its captures' ownership, interpreted and in a standalone exe", timeout = 1200)]
def thread_loop_collects_after_steps_and_preserves_capture_ownership(t : T?) {
    let root = get_das_root()
    let fixture = "{root}/tests/jobque/_fixture_thread_loop_collect.das"
    var output : string
    let interpreted = run_and_capture([get_host_binary(), "-no-module-cache", fixture], output, 60.0)
    t |> equal(interpreted, 0, output)
    t |> success(contains(output, "THREAD_LOOP_COLLECT_OK"), output)
    let temporary = create_temp_directory_result("thread_loop_collect")
    t |> success(temporary is value)
    if (!(temporary is value)) { return }
    let dir = temporary as value
    let built = run_and_capture([get_host_binary(), "-jit", "-exe", "-output", "{dir}/worker.exe",
        fixture], output, 180.0)
    t |> equal(built, 0, output)
    if (built == 0 && fexist("{dir}/worker.exe")) {
        let ran = run_standalone("{dir}/worker.exe", "", output, 60.0)
        t |> equal(ran, 0, output)
        t |> success(contains(output, "THREAD_LOOP_COLLECT_OK"), output)
    } else { t |> failure("standalone artifact missing") }
    let removed = rmdir_rec_result(dir)
    t |> success(removed is value && removed as value)
}

[test(description = "a thread loop that ends without channel release, or a nested one missing its release, fails by name interpreted and in a standalone exe", timeout = 1200)]
def thread_loop_requires_release_at_finish_and_keeps_nested_checks(t : T?) {
    let temporary = create_temp_directory_result("thread_loop_release")
    t |> success(temporary is value)
    if (!(temporary is value)) { return }
    let dir = temporary as value
    let fixture = "{get_das_root()}/tests/jobque/_fixture_thread_loop_release.das"
    var output : string
    let built = run_and_capture([get_host_binary(), "-jit", "-exe", "-output", "{dir}/worker.exe",
        fixture], output, 180.0)
    t |> equal(built, 0, output)
    for (mode in ["final", "nested"]) {
        let result = run_and_capture([get_host_binary(), "-no-module-cache", fixture, "--", mode], output, 20.0)
        t |> success(result != 0, output)
        t |> success(contains(output, mode == "final" ? "thread loop's final step" : "missing channel|>release"), output)
        if (built == 0 && fexist("{dir}/worker.exe")) {
            let native_result = run_standalone("{dir}/worker.exe", "-- {mode}", output, 20.0)
            t |> success(native_result != 0, output)
            t |> success(contains(output, mode == "final" ? "thread loop's final step" : "missing channel|>release"), output)
        } else { t |> failure("standalone artifact missing") }
    }
    let removed = rmdir_rec_result(dir)
    t |> success(removed is value && removed as value)
}

// `options threadlock_context` reaches a standalone exe: the exe's context is built with the
// mutex invoke_in_context needs, where only a [pinvoke] function or the policy used to buy it.

let private THREADLOCK_ROOT = "modules/dasLLVM/tests/_exe_threadlock_root.das"

def private expect_threadlock_lines(t : T?; out : string; what : string) {
    t |> success(find(out, "THREADLOCK_PING") >= 0, "{what}: invoke_in_context on the program's own context ran the function: {tail(out)}")
    t |> success(find(out, "THREADLOCK_DONE") >= 0, "{what}: the program ran to its end: {tail(out)}")
}

[test(description = "options threadlock_context gives a standalone exe's context the mutex invoke_in_context needs, as it does interpreted", timeout = 1200)]
def test_exe_threadlock_option(t : T?) {
    let dir_r = create_temp_directory_result("llvm_exe_threadlock")
    if (!(dir_r is value)) {
        t |> failure("temp dir failed")
        return
    }
    let dir = unsafe(dir_r.value)
    t |> run("options threadlock_context gives the context its mutex, interpreted and as a standalone exe") @(t : T?) {
        var hout = ""
        let hrc = run_and_capture([das_exe(), "-no-module-cache", THREADLOCK_ROOT], hout, 300.0)
        t |> equal(0, hrc, "the fixture runs interpreted on the host: {tail(hout)}")
        expect_threadlock_lines(t, hout, "interpreted")
        let out = build_and_run(t, dir, "threadlock", THREADLOCK_ROOT, [], [])
        expect_threadlock_lines(t, out, "exe")
    }
    rmdir_rec_result(dir)
}
