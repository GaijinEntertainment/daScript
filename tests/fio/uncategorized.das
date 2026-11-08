options gen2

require dastest/testing_boost public
require daslib/fio

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
