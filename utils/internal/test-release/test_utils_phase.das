options gen2

require dastest/testing_boost public
require strings
require utils_phase

[test]
def test_substitute_tokens(t : T?) {
    var inscope argv <- substitute(["@DASLANG@", "@OUT@/x.cpp", "plain", "--flag=@OUT@"], "/b/bin/daslang", "/b", "/tmp/o")
    t |> equal("/b/bin/daslang", argv[0])
    t |> equal("/tmp/o/x.cpp", argv[1])
    t |> equal("plain", argv[2])
    t |> equal("--flag=/tmp/o", argv[3], "@OUT@ substitutes anywhere in the token")
}

[test]
def test_bundle_exe_missing_still_names_bin(t : T?) {
    // a tool the bundle lacks resolves to bin/<name> so the launch fails naming the tool
    let p = bundle_exe("/nonexistent-bundle", "nosuchtool")
    t |> success(p |> ends_with("bin/nosuchtool") || p |> ends_with("bin\\nosuchtool"), "resolved to bin/<name>: {p}")
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
