options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require daslib/fio
require math
require daslib/strings_boost
require strings
require daslib/env_registry
require _model_tier

// The standalone-exe context gate: build `_exe_smoke_root.das` with `-jit -exe` and run the
// artifact - the one rail where globals restore as DATA, so a function-typed global with no
// boot-restore [init] dies on its first invoke while every -jit suite stays green. One tiny
// model, one 4-token generate, ~90 s; skips honestly without the model.

def private self_daslang() : string {
    let argv <- get_command_line_arguments()
    return find(to_generic_path(argv[0]), "/") < 0 ? argv[0] : get_full_file_name(normalize(argv[0]))
}

[test]
def test_exe_smoke(t : T?) {
    t |> run("a -jit -exe build of the facade generates on the serialized-globals rail") @(t : T?) {
        let model = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
        if (!model_available(t, model)) {
            return
        }
        let dir_r = create_temp_directory_result("dasllama_exe_smoke")
        if (!(dir_r is value)) {
            t |> failure("temp dir failed")
            return
        }
        let dir = unsafe(dir_r.value)
        let exe = path_join(dir, "smoke.exe")   //! daslang -exe appends `.exe` to -output itself
        var bout = ""
        let brc = run_and_capture([self_daslang(), "-jit", "-exe", "-output", path_join(dir, "smoke"),
                                   "modules/dasLLAMA/tests/_exe_smoke_root.das"], bout, 600.0)
        t |> equal(0, brc, "the -exe build succeeds: {slice(bout, max(0, length(bout) - 400))}")
        if (brc != 0 || !stat(exe).is_valid) {
            t |> success(stat(exe).is_valid, "the exe artifact exists at -output")
            rmdir_rec_result(dir)
            return
        }
        var out = ""
        let prev_untuned = env_value_of("DASLLAMA_ALLOW_UNTUNED")
        set_env_variable("DASLLAMA_ALLOW_UNTUNED", "1")   //! inherited by the child; /usr/bin/env does not exist on Windows
        //! the artifact links the producing daslang's runtime DLLs, which resolve through PATH and never from the
        //! temp dir - so the run prepends that daslang's dir through the shell; the command opens with `set` or an
        //! assignment, never a quote, which cmd.exe would strip
        let bin_dir = dir_name(self_daslang())
        var cmd = "PATH=\"{bin_dir}:$PATH\" \"{exe}\" -- \"{model}\" 2>&1"
        if (get_platform_name() == "windows") {
            cmd = "set \"PATH={bin_dir};%PATH%\"&& \"{exe}\" -- \"{model}\" 2>&1"
        }
        let rc = unsafe(popen_timeout(cmd, 300.0, $(f) {
            if (f != null) {
                out = fread(f)
            }
        }))
        set_env_variable("DASLLAMA_ALLOW_UNTUNED", prev_untuned)
        t |> equal(0, rc, "the exe runs clean: {slice(out, max(0, length(out) - 400))}")
        t |> success(find(out, "EXE_SMOKE_OK") >= 0, "the generate completed in the exe: {slice(out, max(0, length(out) - 400))}")
        rmdir_rec_result(dir)
    }
}
