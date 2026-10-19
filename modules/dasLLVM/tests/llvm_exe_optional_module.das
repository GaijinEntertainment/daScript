options gen2
options indenting = 4
require dastest/testing_boost public
require daslib/fio
require daslib/env_registry
require ../../../utils/daspkg/utils.das
require strings

[test]
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
