options gen2

require strings
require daslib/strings_boost
require daslib/fio
require classify

// Phase `utils`: every shipped tool exercised in its capacity, from the bundle alone -
// the shipped suites through the shipped runner, one real invocation per exe with an
// oracle a machine can check, and (with --network) daspkg installing the packages the
// gated examples need. Rows are the verified command lines from the RC1 utils audit.

struct ToolCheck {
    name : string           //! verdict key, e.g. "lint.exe/daslib"
    argv : array<string>    //! @DASLANG@ = bundle daslang, @EXE:x@ = bin/x(.exe), @OUT@ = scratch dir
    want : array<string>    //! every substring must appear in the merged output
    forbid : array<string>  //! none may appear (checked against "\n" + output, so "\n0 tests" anchors a line)
    timeout : float = 120.0
    network : bool = false  //! only with --network: fetches packages, mutates the bundle's example dirs
    copy_from : string      //! bundle-relative file copied into @OUT@ before the run (formatter round-trips)
}

let private DASTEST_OK <- ["0 failed, 0 errors, 0 skipped"]
let private DASTEST_EMPTY <- ["\n0 tests,"]

//! The bundle-relative suites the in-tree run_utils_tests target runs - through the
//! script-form runner, which the exe-form row (dastest.exe) then has to match.
def private suite_row(name, dir : string; extra_argv : array<string>) : ToolCheck {
    var argv <- ["@DASLANG@"]
    argv |> push_from(extra_argv, ["dastest/dastest.das", "--", "--test", dir])
    return <- ToolCheck(name = name, argv <- argv, want := DASTEST_OK, forbid := DASTEST_EMPTY, timeout = 300.0)
}

def public utils_checks() : array<ToolCheck> {
    var rows <- [
        // --- shipped suites through the shipped runner ---------------------------------
        <- suite_row("dastest/common-suite", "utils/common/tests", []),
        <- suite_row("dastest/lint-suite", "utils/lint/tests", []),
        <- suite_row("dastest/dascov-suite", "utils/dascov/tests", []),
        <- suite_row("dastest/find-dupe-suite", "utils/find-dupe/tests", []),
        <- suite_row("dastest/jobque-timeline-suite", "utils/jobque-timeline", ["-project_root", "utils/jobque-timeline"]),
        // the exe form must be able to compile a test at all (RC1: frozen build-machine paths)
        ToolCheck(name = "dastest.exe/common-suite", argv <- ["@EXE:dastest@", "--test", "utils/common/tests"],
            want := DASTEST_OK, forbid := DASTEST_EMPTY, timeout = 300.0),
        ToolCheck(name = "gen1_to_gen2/self-tests", argv <- ["@EXE:gen1_to_gen2@", "--tests"]),
        ToolCheck(name = "gen1_to_gen2/convert", argv <- ["@EXE:gen1_to_gen2@", "-v2", "utils/mcp/tests/_fixture_gen1.das"],
            want <- ["options gen2", "struct Point \{"]),
        // --- lint / format over the shipped daslib (the tree is at zero, and stays so) -----
        ToolCheck(name = "lint.exe/help", argv <- ["@EXE:lint@", "--help"], want <- ["Unified lint checker for daslang files."]),
        ToolCheck(name = "lint.exe/daslib", argv <- ["@EXE:lint@", "daslib"],
            want <- ["0 issue(s), 0 error(s)"], forbid <- ["skipped"], timeout = 300.0),
        ToolCheck(name = "das-fmt.exe/verify-daslib", argv <- ["@EXE:das-fmt@", "--path", "daslib", "--verify"],
            want <- ["Verified!"], forbid <- ["Verification failed"]),
        ToolCheck(name = "das-fmt.exe/format-copy", argv <- ["@EXE:das-fmt@", "--path", "@OUT@/strings_boost.das"],
            want <- ["Complete! 1 files"], copy_from = "daslib/strings_boost.das"),
        // --- analysis tools on their shipped fixtures -----------------------------------
        ToolCheck(name = "dascov.exe/report", argv <- ["@EXE:dascov@", "--", "-", "utils/dascov/tests/simple_branch.das"],
            want <- ["simple_branch.das", "TOTAL"]),
        ToolCheck(name = "detect-dupe.exe/fixture", argv <- ["@EXE:detect-dupe@", "-p", "utils/detect-dupe/fixture/synth.das"],
            want <- ["functions analyzed: 9", "exact clusters    : 2", "fuzzy pairs       : 1"]),
        ToolCheck(name = "benchctl.exe/help", argv <- ["@EXE:benchctl@", "--help"], want <- ["benchctl - benchmark database management tool"]),
        ToolCheck(name = "benchctl.exe/reset", argv <- ["@EXE:benchctl@", "--", "reset", "--db", "@OUT@/bench.db"]),
        ToolCheck(name = "benchctl.exe/query", argv <- ["@EXE:benchctl@", "--", "query", "--db", "@OUT@/bench.db"], want <- ["no results"]),
        ToolCheck(name = "aot/emit", argv <- ["@DASLANG@", "utils/aot/main.das", "--", "-aot", "examples/hello_world.das", "@OUT@/hello.cpp"],
            want <- ["Aot to "]),
        ToolCheck(name = "fix-lint-errors/help", argv <- ["@DASLANG@", "utils/fix-lint-errors/main.das", "--", "-?"],
            want <- ["Auto-fixer for mechanical lint rules"]),
        // --- agent-facing tooling ---------------------------------------------------------
        ToolCheck(name = "lsp/validate", argv <- ["@DASLANG@", "utils/lsp/subtools/validate.das", "--", "utils/mcp/tests/_fixture_error.das"],
            want <- ["\"ok\":false", "\"code\":\"30343\""]),
        ToolCheck(name = "lsp/nav", argv <- ["@DASLANG@", "utils/lsp/subtools/nav.das", "--", "documentSymbol", "utils/mcp/tests/_fixture_valid.das"],
            want <- ["\"ok\":true", "\"name\":\"Point\""]),
        ToolCheck(name = "mcp/setup-help", argv <- ["@DASLANG@", "utils/mcp/setup.das", "--", "--show-help"], want <- ["--no-build"]),
        ToolCheck(name = "daslang-live/test_api", argv <- ["@EXE:daslang-live@", "examples/daslive/test_api/main.das", "--live-port", "19099"],
            want <- ["PASS: 2 commands registered"], timeout = 60.0),
        // --- daspkg: launch offline; the installs need the network and mutate example dirs --
        ToolCheck(name = "daspkg.exe/help", argv <- ["@EXE:daspkg@", "--help"], want <- ["daslang package manager"]),
        ToolCheck(name = "daspkg.exe/install-sequence", argv <- ["@EXE:daspkg@", "install", "--root", "examples/games/sequence", "--no-color"],
            want <- ["Installed 'das-cards'"], network = true),
        ToolCheck(name = "daspkg.exe/unlock-sequence", argv <- ["@DASLANG@", "-compile-only", "examples/games/sequence/main.das"], network = true),
        ToolCheck(name = "daspkg.exe/install-daspkg-example", argv <- ["@EXE:daspkg@", "install", "--root", "examples/daspkg/daspkg-example", "--no-color"],
            want <- ["Installed 'daspkg-test-pure'", "Installed 'daspkg-test-deps'"], network = true),
        ToolCheck(name = "daspkg.exe/run-daspkg-example", argv <- ["@DASLANG@", "examples/daspkg/daspkg-example/main.das"],
            want <- ["Hello, daspkg!", "2 + 3 = 5"], network = true),
        ToolCheck(name = "daspkg.exe/install-telegram", argv <- ["@EXE:daspkg@", "install", "--root", "examples/telegram/echo-bot", "--no-color"],
            want <- ["Installed 'dasTelegram'"], network = true),
        ToolCheck(name = "daspkg.exe/unlock-telegram", argv <- ["@DASLANG@", "-compile-only", "examples/telegram/echo-bot/echo_bot.das"], network = true),
        ToolCheck(name = "daspkg.exe/install-build-example", argv <- ["@EXE:daspkg@", "install", "--root", "examples/daspkg/daspkg-build-example", "--no-color"],
            want <- ["Installed 'daspkg-example-c' (local)", "Installed 'daspkg-example-cpp' (local)"], network = true, timeout = 600.0),
        ToolCheck(name = "daspkg.exe/run-build-example", argv <- ["@DASLANG@", "examples/daspkg/daspkg-build-example/main.das"],
            want <- ["fast_inv_sqrt(4.0) = 0.49915358", "counter(step=5), 2x increment = 10"], network = true),
    ]
    // teardown - the example dirs go back to shipped state
    rows |> reserve(length(rows) + 4)
    for (root in ["examples/games/sequence", "examples/daspkg/daspkg-example", "examples/telegram/echo-bot", "examples/daspkg/daspkg-build-example"]) {
        rows |> emplace(ToolCheck(name = "daspkg.exe/cleanup:{root}", argv <- ["@EXE:daspkg@", "cleanup", "--force", "--root", root, "--no-color"], network = true))
    }
    return <- rows
}

def public substitute(argv : array<string>; daslang, bundle, out_dir : string) : array<string> {
    var res : array<string>
    res |> reserve(length(argv))
    for (a in argv) {
        if (a == "@DASLANG@") {
            res |> push(daslang)
        } elif (a |> starts_with("@EXE:")) {
            let name = slice(a, 5, length(a) - 1)
            res |> push(bundle_exe(bundle, name))
        } else {
            res |> push(replace(a, "@OUT@", out_dir))
        }
    }
    return <- res
}

//! bin/<name>.exe if present (every `daslang -exe` tool, and all Windows exes), else bin/<name>.
def public bundle_exe(bundle, name : string) : string {
    for (candidate in ["bin/{name}.exe", "bin/{name}"]) {
        let p = path_join(bundle, candidate)
        if (fexist(p)) {
            return get_full_file_name(p)
        }
    }
    return get_full_file_name(path_join(bundle, "bin/{name}"))   // missing: the launch fails loudly
}

//! "" when the output satisfies the row, else the first unmet expectation - the FAIL head.
def public unmet(check : ToolCheck; out : string) : string {
    for (e in check.want) {
        if (find(out, e) < 0) {
            return "expected `{e}` in output"
        }
    }
    let anchored = "\n{out}"
    for (r in check.forbid) {
        if (find(anchored, r) >= 0) {
            return "unexpected `{strip(r)}` in output"
        }
    }
    return ""
}

//! Runs the rows in order from the bundle root; verdict lines in classify's shape.
def public run_utils_checks(bundle, daslang : string; network : bool; only : string) : array<string> {
    var verdicts : array<string>
    let tmp = create_temp_directory_result("test_release_utils")
    let out_dir = tmp is value ? to_generic_path(tmp as value) : ""
    if (empty(out_dir)) {
        verdicts |> push("FAIL utils/scratch rc=2 cannot create a scratch directory")
        return <- verdicts
    }
    var inscope rows <- utils_checks()
    for (check in rows) {
        continue if ((!empty(only) && find(check.name, only) < 0) || (check.network && !network))
        if (!empty(check.copy_from)) {
            let src = path_join(bundle, check.copy_from)
            fwrite(path_join(out_dir, base_name(src)), fread(src))
        }
        var inscope argv <- substitute(check.argv, daslang, bundle, out_dir)
        let t0 = ref_time_ticks()
        var out : string
        var rc = run_and_capture(argv, out, check.timeout)
        let sec = float(get_time_usec(t0)) / 1000000.0
        var head = out
        if (rc == 0) {
            let miss = unmet(check, out)
            if (!empty(miss)) {
                rc = 1
                head = miss
            }
        }
        verdicts |> push(verdict_line(check.name, rc, sec, check.timeout, head))
    }
    rmdir_rec(out_dir)
    return <- verdicts
}
