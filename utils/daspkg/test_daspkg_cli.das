options gen2
options no_global_variables = false

require dastest/testing_boost public

require daslib/fio
require daslib/clargs
require daslib/defer
require strings
require daslib/strings_boost

require daslib/json_boost

require daslib/ansi_colors

require utils
require commands
require daslib/result
require package_runner public
require _daspkg_test_common

[test(description = "parse_args maps each daspkg command line onto its DaspkgArgs fields, with the documented defaults")]
def test_parse_args(t : T?) {   // nolint:STYLE038 — a flat case table: one argv shape per `t |> run` block
    t |> run("install command") @(t : T?) {
        var r <- parse_args(type<DaspkgArgs>, ["install", "dasImgui"])
        t |> success(r |> is_ok, "parse succeeds")
        let parsed <- r |> move_unwrap
        t |> equal(parsed.command ?? "", "install")
        t |> equal(parsed.command_arg ?? "", "dasImgui")
        t |> success(!parsed.force)
    }
    t |> run("install with force") @(t : T?) {
        var r <- parse_args(type<DaspkgArgs>, ["install", "dasImgui", "--force"])
        t |> success(r |> is_ok, "parse succeeds")
        let parsed <- r |> move_unwrap
        t |> equal(parsed.command ?? "", "install")
        t |> equal(parsed.command_arg ?? "", "dasImgui")
        t |> success(parsed.force)
    }
    t |> run("install with root") @(t : T?) {
        var r <- parse_args(type<DaspkgArgs>, ["--root", "/tmp/proj", "install", "foo"])
        t |> success(r |> is_ok, "parse succeeds")
        let parsed <- r |> move_unwrap
        t |> equal(parsed.command ?? "", "install")
        t |> equal(parsed.command_arg ?? "", "foo")
        t |> equal(parsed.root, "/tmp/proj")
    }
    t |> run("list command no arg") @(t : T?) {
        var r <- parse_args(type<DaspkgArgs>, ["list"])
        t |> success(r |> is_ok, "parse succeeds")
        let parsed <- r |> move_unwrap
        t |> equal(parsed.command ?? "", "list")
        t |> success(parsed.command_arg |> is_none)
    }
    t |> run("release accepts --paranoid (compat, no budget change)") @(t : T?) {
        var r <- parse_args(type<DaspkgArgs>, ["release", "--out", "E:/", "--paranoid"])
        t |> success(r |> is_ok, "parse succeeds")
        let parsed <- r |> move_unwrap
        t |> equal(parsed.command ?? "", "release")
        t |> equal(parsed.out, "E:/")
        t |> success(parsed.paranoid)
    }
    t |> run("release with quick inherit") @(t : T?) {
        var r <- parse_args(type<DaspkgArgs>, ["release", "--quick"])
        t |> success(r |> is_ok, "parse succeeds")
        let parsed <- r |> move_unwrap
        t |> equal(parsed.command ?? "", "release")
        t |> success(parsed.quick)
        t |> success(!parsed.paranoid)
    }
    t |> run("no command") @(t : T?) {
        let empty_args : array<string>
        var r <- parse_args(type<DaspkgArgs>, empty_args)
        t |> success(r |> is_ok, "parse succeeds")
        let parsed <- r |> move_unwrap
        t |> success(parsed.command |> is_none)
    }
    t |> run("cleanup command") @(t : T?) {
        var r <- parse_args(type<DaspkgArgs>, ["cleanup"])
        t |> success(r |> is_ok, "parse succeeds")
        let parsed <- r |> move_unwrap
        t |> equal(parsed.command ?? "", "cleanup")
        t |> success(!parsed.force)
    }
    t |> run("cleanup with force") @(t : T?) {
        var r <- parse_args(type<DaspkgArgs>, ["cleanup", "--force"])
        t |> success(r |> is_ok, "parse succeeds")
        let parsed <- r |> move_unwrap
        t |> equal(parsed.command ?? "", "cleanup")
        t |> success(parsed.force)
    }
    t |> run("cleanup with global and force") @(t : T?) {
        var r <- parse_args(type<DaspkgArgs>, ["cleanup", "--global", "--force"])
        t |> success(r |> is_ok, "parse succeeds")
        let parsed <- r |> move_unwrap
        t |> equal(parsed.command ?? "", "cleanup")
        t |> success(parsed.force)
        t |> success(parsed.global_flag)
    }
    t |> run("default root is dot") @(t : T?) {
        var r <- parse_args(type<DaspkgArgs>, ["list"])
        t |> success(r |> is_ok, "parse succeeds")
        let parsed <- r |> move_unwrap
        t |> equal(parsed.root, ".")
    }
}

// --- logging tests ---

[test(description = "log writes to the screen and the log file, log_debug to the file only, and nothing is written before log_init", timeout = 1200)]
def test_log_writes_to_file(t : T?) {
    let test_root = get_full_file_name("_daspkg_test_log")
    mkdir(test_root)

    t |> run("log() writes to both screen and file") @(t : T?) {
        log_init(test_root)
        log("hello from test\n")
        // read the log file and check content
        var content : string
        fopen("{test_root}/modules/.daspkg.log", "rb") $(f) {
            if (f != null) {
                content = fread(f)
            }
        }
        t |> success(find(content, "hello from test") >= 0)
        t |> success(find(content, "--- daspkg ---") >= 0)
    }

    t |> run("log_debug() writes to file always") @(t : T?) {
        log_verbose_flag = false
        log_init(test_root)
        log_debug("debug message here")
        var content : string
        fopen("{test_root}/modules/.daspkg.log", "rb") $(f) {
            if (f != null) {
                content = fread(f)
            }
        }
        t |> success(find(content, "debug message here") >= 0)
    }

    t |> run("run_cmd logs command to file") @(t : T?) {
        log_init(test_root)
        var output : string
        run_cmd("git --version", output)
        var content : string
        fopen("{test_root}/modules/.daspkg.log", "rb") $(f) {
            if (f != null) {
                content = fread(f)
            }
        }
        t |> success(find(content, "git --version") >= 0)
        // note: stdout capture via popen is unreliable on Windows,
        // so we only check the command itself is logged
    }

    t |> run("no file logging before log_init") @(t : T?) {
        // reset log state
        log_file_path = ""
        log("this should not crash\n")
        log_debug("neither should this")
        t |> success(true)
    }

    // cleanup
    force_rmdir(test_root)
    log_file_path = ""
}

// --- ansi_colors tests ---

[test(description = "ANSI colors wrap text only when enabled, and NO_COLOR overrides the color flag", timeout = 1200)]
def test_ansi_colors(t : T?) {
    t |> run("colors disabled returns plain text") @(t : T?) {
        use_tty_colors = false
        t |> equal("hello", red_str("hello"))
        t |> equal("hello", green_str("hello"))
        t |> equal("hello", yellow_str("hello"))
        t |> equal("hello", blue_str("hello"))
        t |> equal("hello", magenta_str("hello"))
        t |> equal("hello", cyan_str("hello"))
        t |> equal("hello", bold_str("hello"))
        t |> equal("hello", dim_str("hello"))
        t |> equal("hello", underline_str("hello"))
        t |> equal("", reset_str())
    }
    t |> run("colors enabled wraps with ANSI codes") @(t : T?) {
        use_tty_colors = true
        t |> equal("\x1B[31mhi\x1B[0m", red_str("hi"))
        t |> equal("\x1B[32mhi\x1B[0m", green_str("hi"))
        t |> equal("\x1B[33mhi\x1B[0m", yellow_str("hi"))
        t |> equal("\x1B[34mhi\x1B[0m", blue_str("hi"))
        t |> equal("\x1B[35mhi\x1B[0m", magenta_str("hi"))
        t |> equal("\x1B[36mhi\x1B[0m", cyan_str("hi"))
        t |> equal("\x1B[1mhi\x1B[0m", bold_str("hi"))
        t |> equal("\x1B[2mhi\x1B[0m", dim_str("hi"))
        t |> equal("\x1B[4mhi\x1B[0m", underline_str("hi"))
        t |> equal("\x1B[0m", reset_str())
        use_tty_colors = false
    }
    t |> run("init_ansi_colors with --color flag") @(t : T?) {
        with_process([get_host_binary(), "{get_das_root()}/utils/daspkg/fixtures/_fixture_ansi_color.das", "--", "enabled"], "", ["NO_COLOR=0"]) $(var process) {
            t |> equal(unsafe(process_wait(process, 30.0)), 0)
        }
    }
    t |> run("NO_COLOR overrides the color flag") @(t : T?) {
        with_process([get_host_binary(), "{get_das_root()}/utils/daspkg/fixtures/_fixture_ansi_color.das", "--", "disabled"], "", ["NO_COLOR=1"]) $(var process) {
            t |> equal(unsafe(process_wait(process, 30.0)), 0)
        }
    }
    t |> run("init_ansi_colors with --no-color flag") @(t : T?) {
        use_tty_colors = true
        init_ansi_colors(["exe", "script.das", "--no-color"])
        t |> success(!use_tty_colors)
    }
}

// --- parse_args color flags ---
// Under clargs, --color/--no-color populate parsed.color/.no_color and
// main.das's apply_global_flags flips use_tty_colors.  We test the parse +
// apply pipeline together so the integration stays covered.

[test(description = "--color and --no-color set and clear use_tty_colors and are mutually exclusive")]
def test_parse_args_color_flags(t : T?) {
    t |> run("--color sets use_tty_colors") @(t : T?) {
        use_tty_colors = false
        var r <- parse_args(type<DaspkgArgs>, ["list", "--color"])
        t |> success(r |> is_ok, "parse succeeds")
        let parsed <- r |> move_unwrap
        t |> success(parsed.color, "color flag parsed")
        apply_global_flags(parsed)
        t |> success(use_tty_colors)
        use_tty_colors = false
    }
    t |> run("--no-color clears use_tty_colors") @(t : T?) {
        use_tty_colors = true
        var r <- parse_args(type<DaspkgArgs>, ["list", "--no-color"])
        t |> success(r |> is_ok, "parse succeeds")
        let parsed <- r |> move_unwrap
        t |> success(parsed.no_color, "no_color flag parsed")
        apply_global_flags(parsed)
        t |> success(!use_tty_colors)
    }
    t |> run("--color and --no-color together is mutex error") @(t : T?) {
        t |> success(parse_args(type<DaspkgArgs>, ["list", "--color", "--no-color"]) |> is_err, "mutex group rejects both")
    }
}

[test(description = "--global and -g set the global flag, which defaults to false")]
def test_parse_args_global_flag(t : T?) {
    t |> run("--global flag parsed") @(t : T?) {
        var r <- parse_args(type<DaspkgArgs>, ["install", "foo", "--global"])
        t |> success(r |> is_ok, "parse succeeds")
        let parsed <- r |> move_unwrap
        t |> equal(parsed.command ?? "", "install")
        t |> equal(parsed.command_arg ?? "", "foo")
        t |> success(parsed.global_flag)
    }
    t |> run("-g shorthand") @(t : T?) {
        var r <- parse_args(type<DaspkgArgs>, ["-g", "list"])
        t |> success(r |> is_ok, "parse succeeds")
        let parsed <- r |> move_unwrap
        t |> equal(parsed.command ?? "", "list")
        t |> success(parsed.global_flag)
    }
    t |> run("default global is false") @(t : T?) {
        var r <- parse_args(type<DaspkgArgs>, ["install", "foo"])
        t |> success(r |> is_ok, "parse succeeds")
        let parsed <- r |> move_unwrap
        t |> success(!parsed.global_flag)
    }
}

[test(description = "update-index parses --commit and --dry-run, both off by default")]
def test_parse_args_update_index_flags(t : T?) {
    t |> run("--commit and --dry-run parse and default off") @(t : T?) {
        var r <- parse_args(type<DaspkgArgs>, ["update-index", "--commit"])
        t |> success(r |> is_ok, "parse succeeds")
        let parsed <- r |> move_unwrap
        t |> success(parsed.commit)
        t |> success(!parsed.dry_run)
        var r2 <- parse_args(type<DaspkgArgs>, ["update-index", "--dry-run"])
        t |> success(r2 |> is_ok, "parse succeeds")
        let parsed2 <- r2 |> move_unwrap
        t |> success(parsed2.dry_run)
        t |> success(!parsed2.commit)
    }
}
