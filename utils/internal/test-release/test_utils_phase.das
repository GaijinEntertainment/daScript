options gen2

require dastest/testing_boost public
require strings
require daslib/fio
require utils_phase

// a throwaway bundle root with bin/<names...> as files; "" when the temp dir cannot be made
def stage_bundle(names : array<string>) : string {
    let r = create_temp_directory_result("test_release_bundle")
    return "" if (!(r is value))
    let root = to_generic_path(r as value)
    mkdir(path_join(root, "bin"))
    for (n in names) {
        fwrite(path_join(root, "bin/{n}"), "x")
    }
    return root
}

[test]
def test_substitute_tokens(t : T?) {
    let root = stage_bundle(["lint.exe"])
    t |> success(!empty(root), "staged a bundle")
    return if (empty(root))
    var inscope argv <- substitute_tokens(["@DASLANG@", "@OUT@/x.cpp", "plain", "--flag=@OUT@", "@EXE:lint@"], "/b/bin/daslang", root, "/tmp/o")
    t |> equal("/b/bin/daslang", argv[0])
    t |> equal("/tmp/o/x.cpp", argv[1])
    t |> equal("plain", argv[2])
    t |> equal("--flag=/tmp/o", argv[3], "@OUT@ substitutes anywhere in the token")
    t |> equal(to_generic_path(get_full_file_name(path_join(root, "bin/lint.exe"))), to_generic_path(argv[4]), "@EXE:x@ resolves through bundle_exe, trailing @ stripped")
    rmdir_rec(root)
}

[test]
def test_bundle_exe_prefers_exe_then_bare_then_names_bin(t : T?) {
    let root = stage_bundle(["both.exe", "both", "bare"])
    t |> success(!empty(root), "staged a bundle")
    return if (empty(root))
    t |> success(to_generic_path(bundle_exe(root, "both")) |> ends_with("bin/both.exe"), ".exe wins when both exist")
    t |> success(to_generic_path(bundle_exe(root, "bare")) |> ends_with("bin/bare"), "bare name when no .exe")
    t |> success(to_generic_path(bundle_exe(root, "nosuchtool")) |> ends_with("bin/nosuchtool"), "missing tool still names bin/<name> so the launch fails naming it")
    rmdir_rec(root)
}

[test]
def test_judged_rc_and_head(t : T?) {
    let row <- ToolCheck(name = "x", want <- ["Verified!"])
    t |> equal(0, judged_rc(row, 0, "Verified! 3 files"), "rc 0 with the oracle met stays 0")
    t |> equal(1, judged_rc(row, 0, "Complete! 3 files"), "rc 0 with the oracle unmet becomes 1")
    t |> equal(2, judged_rc(row, 2, "Verified! 3 files"), "a non-zero exit keeps its code even if the words appear")
    t |> equal("expected `Verified!` in output", judged_head(row, 0, "Complete! 3 files"))
    t |> equal("Complete! 3 files", judged_head(row, 2, "Complete! 3 files"), "a real failure shows the child's output")
    t |> equal("Verified! 3 files", judged_head(row, 0, "Verified! 3 files"))
}

[test]
def test_unmet_wants_and_forbids(t : T?) {
    let row <- ToolCheck(name = "x", want <- ["0 failed, 0 errors"], forbid <- ["\n0 tests,"])
    t |> equal("", unmet(row, "17 tests, 17 passed, 0 failed, 0 errors, 0 skipped\nSUCCESS!"))
    t |> equal("expected `0 failed, 0 errors` in output", unmet(row, "17 tests, 16 passed, 1 failed, 0 errors"))
    t |> equal("unexpected `0 tests,` in output", unmet(row, "0 tests, 0 passed, 0 failed, 0 errors, 0 skipped"), "the forbid anchors at line start")
    t |> equal("", unmet(row, "10 tests, 10 passed, 0 failed, 0 errors, 0 skipped"), "'10 tests' must not trip the anchored '0 tests'")
    let lint <- ToolCheck(name = "l", want <- ["0 issue(s), 0 error(s)"], forbid <- ["skipped"])
    t |> equal("unexpected `skipped` in output", unmet(lint, "148 files, 0 issue(s), 0 error(s), 1 skipped"))
    t |> equal("", unmet(lint, "148 files, 0 issue(s), 0 error(s)"))
}

[test]
def test_rows_are_well_formed(t : T?) {
    var inscope rows <- utils_checks()
    t |> success(length(rows) > 20, "{length(rows)} rows")
    var names : table<string>
    for (r in rows) {
        t |> success(!empty(r.name), "row has a name")
        t |> success(!key_exists(names, r.name), "row name unique: {r.name}")
        names |> insert(r.name)
        t |> success(!empty(r.argv), "{r.name} has argv")
        t |> success(r.timeout > 0.0, "{r.name} has a timeout")
    }
}
