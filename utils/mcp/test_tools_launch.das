options gen2
options rtti

require dastest/testing_boost public
require strings
require _tools_common

require tools/common public
require tools/live public

[test(description = "run_mcp_subtool passes its arguments to the subtool verbatim, including an empty list.", timeout = 1200)]
def test_run_mcp_subtool_passthrough(t : T?) {
    t |> run("argv round-trips through `--` separator") <| @(t : T?) {
        let raw = run_mcp_subtool("_test_passthrough", ["alpha", "beta", "gamma"])
        var text : string
        var is_error = false
        parse_result(raw, text, is_error)
        t |> success(!is_error, "should not be error -- text={text}")
        t |> success(find(text, "PASSTHROUGH: alpha beta gamma") >= 0,
                     "args should round-trip verbatim -- text={text}")
    }
    t |> run("empty args list still works") <| @(t : T?) {
        let raw = run_mcp_subtool("_test_passthrough", [])
        var text : string
        var is_error = false
        parse_result(raw, text, is_error)
        t |> success(!is_error, "should not be error -- text={text}")
        t |> success(find(text, "PASSTHROUGH:") >= 0,
                     "marker present even with no args -- text={text}")
    }
}

[test(description = "run_mcp_subtool reports a nonexistent subtool by name.", timeout = 1200)]
def test_run_mcp_subtool_missing(t : T?) {
    t |> run("nonexistent subtool surfaces as error") <| @(t : T?) {
        let raw = run_mcp_subtool("_does_not_exist_xyzzy", [])
        var text : string
        var is_error = false
        parse_result(raw, text, is_error)
        t |> success(is_error, "should be error -- text={text}")
        t |> success(find(text, "_does_not_exist_xyzzy") >= 0,
                     "error should name the missing subtool -- text={text}")
    }
}

[test(description = "run_mcp_subtool reports a subtool that outlives its timeout as timed out.", timeout = 1200)]
def test_run_mcp_subtool_timeout(t : T?) {
    t |> run("subtool exceeding timeout surfaces 'timed out'") <| @(t : T?) {
        let raw = run_mcp_subtool("_test_sleep", ["5000"], [timeout_sec = 0.5])
        var text : string
        var is_error = false
        parse_result(raw, text, is_error)
        t |> success(is_error, "should be error -- text={text}")
        t |> success(find(text, "timed out") >= 0,
                     "error should mention 'timed out' -- text={text}")
    }
}

[test(description = "run_mcp_subtool reports a panicking subtool with its exit code and the output it printed before the panic.", timeout = 1200)]
def test_run_mcp_subtool_nonzero_exit(t : T?) {
    t |> run("panicking subtool surfaces 'failed (exit ...)'") <| @(t : T?) {
        let raw = run_mcp_subtool("_test_panic", [])
        var text : string
        var is_error = false
        parse_result(raw, text, is_error)
        t |> success(is_error, "should be error -- text={text}")
        t |> success(find(text, "failed (exit") >= 0,
                     "error should report non-zero exit -- text={text}")
        t |> success(find(text, "STDOUT_BEFORE_PANIC") >= 0,
                     "captured stdout before the panic should be in the error payload -- text={text}")
    }
}

[test(description = "Live script arguments with no project and no project_root are just the working directory flag.")]
def test_live_args_neither(t : T?) {
    t |> run("no project, no project_root: just -cwd") <| @(t : T?) {
        let s = build_live_script_args("/path/to/foo.das", "", "", false)
        t |> equal(s, "-cwd \"/path/to/foo.das\"", "should be just -cwd")
    }
}

[test(description = "Live script arguments put the project flag before the working directory flag.")]
def test_live_args_project_only(t : T?) {
    t |> run("project set, project_root empty: -project before -cwd") <| @(t : T?) {
        let s = build_live_script_args("/path/foo.das", "/proj/P.das_project", "", false)
        t |> equal(s, "-project \"/proj/P.das_project\" -cwd \"/path/foo.das\"",
                   "should prepend -project before -cwd")
    }
}

[test(description = "Live script arguments put the project_root flag before the working directory flag.")]
def test_live_args_project_root_only(t : T?) {
    t |> run("project_root set, project empty: -project_root before -cwd") <| @(t : T?) {
        let s = build_live_script_args("/path/foo.das", "", "/proj_root", false)
        t |> equal(s, "-project_root \"/proj_root\" -cwd \"/path/foo.das\"",
                   "should prepend -project_root before -cwd")
    }
}

[test(description = "Live script arguments order project_root before project before the working directory.")]
def test_live_args_both(t : T?) {
    t |> run("both set: -project_root before -project before -cwd") <| @(t : T?) {
        let s = build_live_script_args("/path/foo.das", "/p.das_project", "/proj_root", false)
        t |> equal(s, "-project_root \"/proj_root\" -project \"/p.das_project\" -cwd \"/path/foo.das\"",
                   "project_root prepended last ends up leftmost")
    }
}

[test(description = "Live script arguments place a single load_module between the project_root slot and the working directory.")]
def test_live_args_load_module_only(t : T?) {
    t |> run("single load_module: -load_module before -cwd") <| @(t : T?) {
        var lm : array<string>
        lm |> push("/p1")
        let s = build_live_script_args("/path/foo.das", "", "", false, lm)
        t |> equal(s, "-load_module \"/p1\" -cwd \"/path/foo.das\"",
                   "single load_module pair between project_root slot and -cwd")
    }
}

[test(description = "Live script arguments keep several load_modules in source order.")]
def test_live_args_load_module_multi(t : T?) {
    t |> run("two load_modules: both pairs preserved in source order") <| @(t : T?) {
        let lm <- ["/p1", "/p2"]
        let s = build_live_script_args("/path/foo.das", "", "", false, lm)
        t |> equal(s, "-load_module \"/p1\" -load_module \"/p2\" -cwd \"/path/foo.das\"",
                   "two load_module pairs preserved in source order")
    }
}

[test(description = "Live script arguments order project_root, load_modules, project and working directory.")]
def test_live_args_all_set(t : T?) {
    t |> run("project_root + load_modules + project + cwd: ordered project_root → load_module → project → cwd") <| @(t : T?) {
        let lm <- ["/m1", "/m2"]
        let s = build_live_script_args("/path/foo.das", "/p.das_project", "/proj_root", false, lm)
        t |> equal(s,
                   "-project_root \"/proj_root\" -load_module \"/m1\" -load_module \"/m2\" -project \"/p.das_project\" -cwd \"/path/foo.das\"",
                   "full order: project_root, then load_modules, then project, then cwd")
    }
}

[test(description = "Live script arguments convert load_module paths to backslashes on Windows.")]
def test_live_args_load_module_windows_paths(t : T?) {
    t |> run("is_windows=true: load_module paths get slash-replaced too") <| @(t : T?) {
        var lm : array<string>
        lm |> push("D:/mods/dasImgui")
        let s = build_live_script_args("D:/path/foo.das", "", "", true, lm)
        t |> equal(s, "-load_module \"D:\\mods\\dasImgui\" -cwd \"D:\\path\\foo.das\"",
                   "windows: load_module paths slash-replaced like project_root")
    }
}

[test(description = "Live script arguments convert slashes to backslashes only on Windows.")]
def test_live_args_windows_paths(t : T?) {
    t |> run("is_windows=true: forward slashes become backslashes") <| @(t : T?) {
        let s = build_live_script_args("D:/path/foo.das", "D:/p.das_project", "D:/root", true)
        t |> equal(s, "-project_root \"D:\\root\" -project \"D:\\p.das_project\" -cwd \"D:\\path\\foo.das\"",
                   "windows: slashes replaced in all three quoted paths")
    }
    t |> run("is_windows=false: forward slashes preserved") <| @(t : T?) {
        let s = build_live_script_args("D:/path/foo.das", "", "", false)
        t |> success(find(s, "/path/foo.das") >= 0, "non-windows: keep forward slashes -- s={s}")
        t |> success(find(s, "\\path") < 0, "non-windows: no backslash conversion -- s={s}")
    }
}

[test(description = "resolve_load_modules turns an empty list into an empty list.")]
def test_resolve_load_modules_empty(t : T?) {
    t |> run("empty input yields empty output") <| @(t : T?) {
        let src : array<string>
        let out <- resolve_load_modules(src)
        t |> equal(length(out), 0, "empty in, empty out")
    }
}

[test(description = "resolve_load_modules makes relative paths absolute under the daslang root.")]
def test_resolve_load_modules_absolutizes_relative(t : T?) {
    t |> run("relative paths become absolute") <| @(t : T?) {
        let src <- ["foo", "./bar/baz"]
        let out <- resolve_load_modules(src)
        t |> equal(length(out), 2, "preserves entry count")
        let root = get_das_root()
        t |> success(find(out[0], root) >= 0,
                     "out[0] should contain das root prefix -- out[0]={out[0]} root={root}")
        t |> success(find(out[1], root) >= 0,
                     "out[1] should contain das root prefix -- out[1]={out[1]} root={root}")
    }
}

[test(description = "resolve_load_modules leaves absolute paths unchanged.")]
def test_resolve_load_modules_preserves_absolute(t : T?) {
    t |> run("already-absolute paths pass through unchanged") <| @(t : T?) {
        var src : array<string>
        let is_windows = get_platform_name() == "windows"
        let abs1 = is_windows ? "D:/already/abs/one" : "/already/abs/one"
        let abs2 = is_windows ? "D:/already/abs/two" : "/already/abs/two"
        src |> push(abs1)
        src |> push(abs2)
        let out <- resolve_load_modules(src)
        t |> equal(length(out), 2, "preserves entry count")
        t |> equal(out[0], abs1, "absolute path passes through unchanged")
        t |> equal(out[1], abs2, "absolute path passes through unchanged")
    }
}

[test(description = "The subtool argv without a project_root is the executable, the script, a separator and the arguments.")]
def test_build_subtool_argv_no_project_root(t : T?) {
    t |> run("argv without project_root: [exe, script, --, ...args]") <| @(t : T?) {
        let args <- ["alpha", "beta"]
        let argv <- build_subtool_argv("daslang.exe", "sub.das", args, "")
        t |> equal(length(argv), 5, "argv length should be 5")
        t |> equal(argv[0], "daslang.exe", "argv[0]=exe")
        t |> equal(argv[1], "sub.das", "argv[1]=script_path (no flag before it)")
        t |> equal(argv[2], "--", "argv[2]=--")
        t |> equal(argv[3], "alpha", "argv[3]=alpha")
        t |> equal(argv[4], "beta", "argv[4]=beta")
    }
}

[test(description = "The subtool argv puts the project_root flag before the script path.")]
def test_build_subtool_argv_with_project_root(t : T?) {
    t |> run("argv with project_root: flag inserted BEFORE script path") <| @(t : T?) {
        let args <- ["alpha"]
        let argv <- build_subtool_argv("daslang.exe", "sub.das", args, "/proj/root")
        t |> equal(length(argv), 6, "argv length should be 6")
        t |> equal(argv[0], "daslang.exe", "argv[0]=exe")
        t |> equal(argv[1], "-project_root", "argv[1]=-project_root flag")
        t |> equal(argv[2], "/proj/root", "argv[2]=project_root path")
        t |> equal(argv[3], "sub.das", "argv[3]=script_path (AFTER the flag)")
        t |> equal(argv[4], "--", "argv[4]=--")
        t |> equal(argv[5], "alpha", "argv[5]=alpha")
    }
}

[test(description = "The subtool argv stays valid and ends in the separator when there are no arguments.")]
def test_build_subtool_argv_empty_args(t : T?) {
    t |> run("empty args still yields valid argv ending in --") <| @(t : T?) {
        let args : array<string>
        let argv <- build_subtool_argv("daslang.exe", "sub.das", args, "")
        t |> equal(length(argv), 3, "argv length should be 3")
        t |> equal(argv[0], "daslang.exe", "argv[0]=exe")
        t |> equal(argv[1], "sub.das", "argv[1]=script")
        t |> equal(argv[2], "--", "argv[2]=--")
    }
    t |> run("empty args + project_root still yields valid argv") <| @(t : T?) {
        let args : array<string>
        let argv <- build_subtool_argv("daslang.exe", "sub.das", args, "/root")
        t |> equal(length(argv), 5, "argv length should be 5")
        t |> equal(argv[3], "sub.das", "script comes after flag")
        t |> equal(argv[4], "--", "trailing -- still present")
    }
}

[test(description = "The subtool argv puts a load_module pair before the script path.")]
def test_build_subtool_argv_with_load_module(t : T?) {
    t |> run("single load_module pair inserted before script path") <| @(t : T?) {
        let args <- ["alpha"]
        var lm : array<string>
        lm |> push("/mod1")
        let argv <- build_subtool_argv("daslang.exe", "sub.das", args, "", lm)
        t |> equal(length(argv), 6, "argv length should be 6")
        t |> equal(argv[0], "daslang.exe", "argv[0]=exe")
        t |> equal(argv[1], "-load_module", "argv[1]=-load_module flag")
        t |> equal(argv[2], "/mod1", "argv[2]=load_module path")
        t |> equal(argv[3], "sub.das", "argv[3]=script_path (AFTER load_module)")
        t |> equal(argv[4], "--", "argv[4]=--")
        t |> equal(argv[5], "alpha", "argv[5]=user arg")
    }
}

[test(description = "The subtool argv keeps several load_module pairs in source order before the script.")]
def test_build_subtool_argv_with_load_modules_multi(t : T?) {
    t |> run("two load_module pairs preserved in source order") <| @(t : T?) {
        let args : array<string>
        let lm <- ["/m1", "/m2"]
        let argv <- build_subtool_argv("daslang.exe", "sub.das", args, "", lm)
        t |> equal(length(argv), 7, "argv length should be 7")
        t |> equal(argv[1], "-load_module", "argv[1]=first -load_module flag")
        t |> equal(argv[2], "/m1", "argv[2]=first path")
        t |> equal(argv[3], "-load_module", "argv[3]=second -load_module flag")
        t |> equal(argv[4], "/m2", "argv[4]=second path")
        t |> equal(argv[5], "sub.das", "argv[5]=script (AFTER both load_module pairs)")
    }
}

[test(description = "The subtool argv orders project_root, then load_module, then the script.")]
def test_build_subtool_argv_with_project_root_and_load_module(t : T?) {
    t |> run("project_root + load_module: project_root first, then load_module, then script") <| @(t : T?) {
        let args : array<string>
        var lm : array<string>
        lm |> push("/m1")
        let argv <- build_subtool_argv("daslang.exe", "sub.das", args, "/proj_root", lm)
        t |> equal(length(argv), 7, "argv length should be 7")
        t |> equal(argv[1], "-project_root", "argv[1]=-project_root flag")
        t |> equal(argv[2], "/proj_root", "argv[2]=project_root path")
        t |> equal(argv[3], "-load_module", "argv[3]=-load_module flag (AFTER project_root)")
        t |> equal(argv[4], "/m1", "argv[4]=load_module path")
        t |> equal(argv[5], "sub.das", "argv[5]=script_path (AFTER all flags)")
        t |> equal(argv[6], "--", "argv[6]=--")
    }
}
