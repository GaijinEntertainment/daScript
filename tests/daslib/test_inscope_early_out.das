options gen2
options no_aot

require dastest/testing_boost public
require daslib/fio
require strings

[test]
def test_inscope_after_nested_early_out(t : T?) {
    t |> run("break and continue before a var inscope leave it zeroed for the finalizer") @(t : T?) {
        var output : string
        let root = get_das_root()
        let rc = unsafe(popen_argv([get_command_line_arguments()[0], "-dasroot", root,
                                    path_join(root, "tests/daslib/_inscope_early_out_probe.das")], 0.0, $(f) {
            if (f != null) {
                output = fread(f)
            }
        }))
        t |> equal(rc, 0, "the probe runs: {output}")
        t |> equal(strip(output), "6 3")
    }
}
