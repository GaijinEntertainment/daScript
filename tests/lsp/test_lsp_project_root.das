options gen2
options no_unused_block_arguments = false

require dastest/testing_boost
require daslib/fio
require daslib/command_line
require strings

//! the validate subtool spawned with -project_root sees a project's descriptors: the pretend
//! root's module registers a C++ module and a require path, and the consumer validates clean
//! only through them
[test]
def test_validate_project_root(t : T?) {
    let exe = get_full_file_name(get_das_exe())
    let root = path_join(get_das_root(), "utils/mcp/tests/_pretend_root")
    let script = path_join(root, "probe_consumer.das")
    let validate = path_join(get_das_root(), "utils/lsp/subtools/validate.das")
    t |> run("with -project_root the consumer validates clean") @(t : T?) {
        var out : string
        let rc = run_and_capture([exe, "-project_root", root, validate, "--", script], out, 120.0)
        t |> equal(rc, 0, "validate exits 0: {out}")
        t |> success(find(out, "\"ok\":true") >= 0 && find(out, "\"diagnostics\":[]") >= 0, "no diagnostics: {out}")
    }
    t |> run("without it the same file reports the require it cannot resolve") @(t : T?) {
        var out : string
        run_and_capture([exe, validate, "--", script], out, 120.0)
        t |> success(find(out, "unit_probe/hello") >= 0, "the project's require path is what goes missing: {out}")
    }
}
