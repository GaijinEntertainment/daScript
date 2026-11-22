options gen2

require dastest/testing_boost public
require daslib/fio
require strings

// `diagnostics_to_file` points the process-wide diagnostics sink at a file, so a
// program whose stdout carries a protocol and whose launcher keeps no stderr still
// has somewhere to leave a leak dump or a fatal. It is fail-closed: a path it
// cannot open returns false and leaves the sink where it was.

[test(description = "Diagnostics redirect to a file refuses an unopenable path and accepts a writable one")]
def test_diagnostics_to_file(t : T?) {
    t |> run("a path that cannot be opened is refused") @(t : T?) {
        t |> success(!diagnostics_to_file("/no/such/directory/diagnostics.log"))
    }
    t |> run("a writable path is taken") @(t : T?) {
        var terr = ""
        let root = create_temp_directory("das_diag_", terr)
        t |> success(!empty(root), "temp dir created: {terr}")
        if (!empty(root)) {
            t |> success(diagnostics_to_file(path_join(root, "diag.log")))
            // moving the sink off the file closes it, and a removal that succeeds
            // is the proof: on Windows an open handle holds a file against deletion
            diagnostics_to_stderr()
            t |> success(rmdir_rec(root), "the sink held its file open")
        }
    }
}

// `format_time(clock, fmt)` renders a clock as local-time text via strftime.
// Assertions stay timezone/DST-independent: at 09:22 the date part is stable (the
// largest DST normalization is one hour, which never carries 09:22 across midnight),
// and the formatting mechanism (default pattern, literals, %% escaping) is tz-agnostic.

[test(description = "format_time formats the date part as YYYY-MM-DD, falls back to a default pattern, and passes plain text and %% through")]
def test_format_time(t : T?) {
    let c = mktime(2026, 6, 13, 9, 22, 45)
    t |> run("date part formats as YYYY-MM-DD") @(t : T?) {
        t |> success(format_time(c, "%Y-%m-%d") == "2026-06-13")
        t |> success(format_time(c, "%Y") == "2026")
    }
    t |> run("empty format falls back to the default pattern") @(t : T?) {
        t |> success(format_time(c, "") == format_time(c, "%Y-%m-%d %H:%M"))
    }
    t |> run("non-specifier text and %% pass through literally") @(t : T?) {
        t |> success(format_time(c, "hello %% world") == "hello % world")
    }
}

//! the AOT build of this file is the proof: generated C++ calls builtin_exit_now, which the public
//! header must declare; the call itself never runs, since argv is never empty under the CLI

[test(description = "exit_now compiles into generated C++ against the public header")]
def test_exit_now_binds_under_aot(t : T?) {
    t |> run("exit_now compiles into generated C++ against the public header") @(t : T?) {
        let args <- get_command_line_arguments()
        if (empty(args)) {
            unsafe(exit_now(3))
        }
        t |> success(!empty(args), "the process is still here: argv carried the interpreter")
    }
}

[test(description = "unset_env_variable removes a variable that was set")]
def test_unset_env_variable(t : T?) {
    set_env_variable("DAS_T_UNSET_ME", "here")
    unset_env_variable("DAS_T_UNSET_ME")
    t |> success(!has_env_variable("DAS_T_UNSET_ME"), "the variable is gone")
}

//! how many mappings the child `_fixture_hugepage_child.das` reports
//! as huge-page advised under `env`, -1 where it printed no count
def private advised_mappings(env : string; var output : string&) : int {
    let argv <- [get_command_line_arguments()[0], "-dasroot", get_das_root(), "-no-module-cache",
        "{get_das_root()}/tests/fio/_fixture_hugepage_child.das"]
    let rc = run_and_capture(argv, "", [env], output, 120.0)
    let at = find(output, "ADVISED=")
    return -1 if (rc != 0 || at < 0)
    return to_int(slice(output, at + length("ADVISED=")))
}

[test(description = "On Linux an allocation of 2 MB or more carries the huge-page advice with DAS_NO_HUGEPAGE_HINT empty or 0, and any other value of it drops the advice", timeout = 1200)]
def test_no_hugepage_hint(t : T?) {
    if (get_platform_name() != "linux") {
        t |> skip("the huge-page hint is Linux's: {get_platform_name()} allocates without one")
        return
    }
    var modes = ""
    fopen("/sys/kernel/mm/transparent_hugepage/enabled", "rb") $(f) {
        if (f != null) {
            modes = fgets(f)
        }
    }
    if (find(modes, "[always]") < 0 && find(modes, "[madvise]") < 0) {
        t |> skip("this kernel takes no huge-page advice (its modes: '{strip(modes)}')")
        return
    }
    var output : string
    for (on in ["DAS_NO_HUGEPAGE_HINT=0", "DAS_NO_HUGEPAGE_HINT="]) {
        let hinted = advised_mappings(on, output)
        t |> success(hinted > 0, "under `{on}` the array's mapping carries the advice: {output}")
    }
    let plain = advised_mappings("DAS_NO_HUGEPAGE_HINT=1", output)
    t |> equal(plain, 0, "under the knob no mapping carries the advice: {output}")
}
