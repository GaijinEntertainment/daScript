options gen2
options no_aot

require dastest/testing_boost
require daslib/fio
require ../../dastest/worker_policy.das
require ../../utils/internal/preflight/config.das

[test]
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

    tt |> run("untracked_from_ls drops CR and blank lines") <| @(t : T?) {
        var inscope files <- untracked_from_ls("a.txt\r\nb/c.txt\n")
        t |> equal(2, length(files))
        t |> equal("a.txt", files[0])
        t |> equal("b/c.txt", files[1])

        var inscope none <- untracked_from_ls("")
        t |> equal(0, length(none))

        var inscope blanks <- untracked_from_ls("\n\r\n   \n")
        t |> equal(0, length(blanks))
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
        t |> equal(settings.jit_max_file_time, 60.0)
    }
}
