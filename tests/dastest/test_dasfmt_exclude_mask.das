options gen2
options no_aot

require strings
require dastest/testing_boost
require daslib/fio

[test]
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
