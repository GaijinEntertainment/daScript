options gen2
options no_aot

require strings
require dastest/testing_boost
require daslib/defer
require daslib/fio
require daslib/strings_boost
require ../../dastest/worker_policy.das
require ../../utils/internal/preflight/config.das

[test(description = "dasfmt --verify honors --exclude-mask: a masked generated file is skipped and reported, a ./-prefixed mask normalizes to the same exclusion, and a mask that normalizes to empty excludes nothing", timeout = 1200)]
def test_dasfmt_exclude_mask(t : T?) {
    let exe = get_command_line_arguments()[0]
    let tool = path_join(get_das_root(), "utils/das-fmt/dasfmt.das")
    var terr : string
    let tmp = temp_directory(terr)
    t |> equal("", terr)
    let root = path_join(tmp, "dasfmt_mask_{ref_time_ticks()}")
    mkdir(root)
    mkdir(path_join(root, "build"))
    t |> success(fwrite(path_join(root, "clean.das"), "options gen2\n\n[export]\ndef main \{\n    var x = 1\n    print(\"\{x\}\\n\")\n\}\n"), "wrote formatted file")
    t |> success(fwrite(path_join(root, "build/gen.das"), "options gen2\n\n[export]\ndef main \{\n    var x=1\n    print(\"\{x\}\\n\")\n\}\n"), "wrote unformatted generated file")

    let log_level = get_env_variable("DAS_LOG_LEVEL")
    set_env_variable("DAS_LOG_LEVEL", "info")
    defer() {
        set_env_variable("DAS_LOG_LEVEL", log_level)
    }
    var out = ""
    let rcNoMask = run_and_capture([exe, tool, "--", "--path", root, "--verify"], out, 120.0)
    t |> success(rcNoMask != 0, "unmasked verify fails on the generated file")

    let rcMasked = run_and_capture([exe, tool, "--", "--path", root, "--verify", "--exclude-mask", "build/"], out, 120.0)
    t |> equal(0, rcMasked)
    t |> success(find(out, "Excluding file") >= 0, "the exclusion is reported, so the pass is not an accidental green")

    let rcDotMask = run_and_capture([exe, tool, "--", "--path", root, "--verify", "--exclude-mask", "./build/"], out, 120.0)
    t |> equal(0, rcDotMask)
    t |> success(find(out, "Excluding file") >= 0, "a ./-prefixed mask normalizes to the same exclusion")

    let rcEmptyMask = run_and_capture([exe, tool, "--", "--path", root, "--verify", "--exclude-mask", "./"], out, 120.0)
    t |> success(rcEmptyMask != 0, "a mask that normalizes to empty excludes nothing - verify stays red")

    var rmerr : string
    rmdir_rec(root, rmerr)
    t |> equal("", rmerr)
}

[test(description = "preflight's JIT settings: the worker default is conservative and capped, the max-file-time override accepts only its sentinel or non-negative values, the TOML loads, and an invalid TOML fails closed to the defaults")]
def test_preflight_config(tt : T?) {
    tt |> run("JIT worker default is conservative and capped") <| @(t : T?) {
        t |> equal(default_jit_isolated_threads(2), 4)
        t |> equal(default_jit_isolated_threads(16), 8)
        t |> equal(default_jit_isolated_threads(64), 32)
        t |> equal(default_jit_isolated_threads(128), 32)
    }

    tt |> run("JIT max-file-time override accepts only its sentinel or non-negative values") <| @(t : T?) {
        t |> success(valid_jit_max_file_time_override(-1.0))
        t |> success(valid_jit_max_file_time_override(0.0))
        t |> success(valid_jit_max_file_time_override(0.5))
        t |> success(!valid_jit_max_file_time_override(-0.5))
        t |> success(!valid_jit_max_file_time_override(-2.0))
    }

    tt |> run("preflight TOML loads JIT settings") <| @(t : T?) {
        let temp = create_temp_file_result("preflight_config", ".toml")
        t |> success(temp is value, "temporary config created")
        if (!(temp is value)) return
        let path = unsafe(temp.value)
        let wrote = fwrite(path, "[jit]\nthreads = 48\nmax_file_time = 75.5\n")
        t |> success(wrote, "temporary config written")
        var error : string
        let settings = load_preflight_settings(path, error)
        remove(path)
        t |> equal(error, "")
        t |> equal(settings.jit_threads, 48)
        t |> equal(settings.jit_max_file_time, 75.5)
    }

    tt |> run("invalid settings fail closed to defaults") <| @(t : T?) {
        let temp = create_temp_file_result("preflight_config_bad", ".toml")
        t |> success(temp is value, "temporary config created")
        if (!(temp is value)) return
        let path = unsafe(temp.value)
        let wrote = fwrite(path, "[jit]\nthreads = -2\n")
        t |> success(wrote, "temporary config written")
        var error : string
        let settings = load_preflight_settings(path, error)
        remove(path)
        t |> success(!empty(error), "invalid config reports an error")
        t |> equal(settings.jit_threads, 0)
        t |> equal(settings.jit_max_file_time, 120.0)
    }
}

def run_dastest_capturing(t : T?; mode_args : array<string>) : string {
    set_env_variable("DAS_LOG_LEVEL", "info")
    defer() {
        set_env_variable("DAS_LOG_LEVEL", "")
    }
    let das_root = get_das_root()
    var args <- [
        get_command_line_arguments()[0],
        "-dasroot", das_root,
        "{das_root}/dastest/dastest.das",
        "--",
        "--failures-only",
        "--test", "{das_root}/tests/dastest/_fixture_pass.das"
    ]
    args |> push_from(mode_args)
    var output : string
    let exit_code = run_and_capture(args, output)
    t |> equal(exit_code, 0, "the child dastest run must succeed; output:\n{output}")
    return output
}

[test(description = "a sequential sweep under --failures-only prints `run N/M: <file>` before the program runs, so a hung sweep still names it", timeout = 1200)]
def test_sequential_sweep_names_each_program(t : T?) {
    let output = run_dastest_capturing(t, [])
    let at = find(output, "run 1/1: ")
    t |> success(at >= 0, "the sequential sweep prints `run N/M: <file>` before the program")
    if (at >= 0) {
        let line = (split(slice(output, at), "\n"))[0]
        t |> success(find(line, "_fixture_pass.das") >= 0, "the start line names the program: {line}")
    }
}

[test(description = "an isolated sweep under --failures-only prints `worker K starts: <files>` before the batch runs, so a hung sweep still names it", timeout = 1200)]
def test_isolated_sweep_names_each_batch(t : T?) {
    let output = run_dastest_capturing(t, ["--isolated-mode", "--isolated-mode-threads", "1"])
    let at = find(output, "worker 1 starts: ")
    t |> success(at >= 0, "the isolated dispatcher prints `worker K starts: <files>` before the batch")
    if (at >= 0) {
        let line = (split(slice(output, at), "\n"))[0]
        t |> success(find(line, "_fixture_pass.das") >= 0, "the batch line names its files: {line}")
    }
}
