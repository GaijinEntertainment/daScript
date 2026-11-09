options gen2
options rtti

require dastest/testing_boost public
require daslib/json
require daslib/json_boost
require strings
require daslib/fio
require _tools_common

require tools/compile_check public
require tools/list_functions public
require tools/list_types public
require tools/run_script public
require tools/run_test public
require tools/ast_dump public
require tools/list_modules public
require tools/find_symbol public
require tools/list_module_api public
require tools/list_requires public
require tools/goto_definition public
require tools/type_of public
require tools/find_references public
require tools/eval_expression public
require tools/describe_type public
require tools/common public
require tools/aot public
require tools/lint_tool public
require tools/program_log public

def private project_root_fixture() : string =>
    path_join(get_das_root(), "utils/mcp/tests/_pretend_root")

[test(description = "compile_check reports a missing project file and a non-project extension each with a focused error.", timeout = 1200)]
def test_compile_check_invalid_project(t : T?) {
    t |> run("missing project file reports focused error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_compile_check(fixture_path("_fixture_valid.das"), "nonexistent_xyz.das_project"), text, is_error)
        t |> success(is_error, "should be error")
        t |> success(find(text, "'project' file not found") >= 0, "should mention project file not found")
    }
    t |> run("non-.das_project extension reports focused error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_compile_check(fixture_path("_fixture_valid.das"), fixture_path("_fixture_valid.das")), text, is_error)
        t |> success(is_error, "should be error")
        t |> success(find(text, "expected a path to a .das_project file") >= 0, "should mention expected extension")
    }
}

[test(description = "run_script reports a missing project file and a non-project extension each with a focused error.", timeout = 1200)]
def test_run_script_invalid_project(t : T?) {
    t |> run("missing project file reports focused error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_run_script("", "print(\"hi\")", "", false, "nonexistent_xyz.das_project"), text, is_error)
        t |> success(is_error, "should be error")
        t |> success(find(text, "'project' file not found") >= 0, "should mention project file not found")
    }
    t |> run("non-.das_project extension reports focused error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_run_script("", "print(\"hi\")", "", false, fixture_path("_fixture_valid.das")), text, is_error)
        t |> success(is_error, "should be error")
        t |> success(find(text, "expected a path to a .das_project file") >= 0, "should mention expected extension")
    }
}

[test(description = "eval_expression reports a missing project file and a non-project extension each with a focused error.", timeout = 1200)]
def test_eval_expression_invalid_project(t : T?) {
    t |> run("missing project file reports focused error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_eval_expression("1 + 1", "", "nonexistent_xyz.das_project"), text, is_error)
        t |> success(is_error, "should be error")
        t |> success(find(text, "'project' file not found") >= 0, "should mention project file not found")
    }
    t |> run("non-.das_project extension reports focused error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_eval_expression("1 + 1", "", fixture_path("_fixture_valid.das")), text, is_error)
        t |> success(is_error, "should be error")
        t |> success(find(text, "expected a path to a .das_project file") >= 0, "should mention expected extension")
    }
}

//! the pretend root's second module folder: its descriptor registers the tree's UnitTest C++ module
//! and a require path of its own, and every tool asked with project_root sees both - as does a plain run
[test(description = "A project_root with a descriptor makes its C++ module and require path visible to list_modules, find_symbol, compile_check and a plain run.", timeout = 1200)]
def test_project_root_fixture(t : T?) {
    let root = project_root_fixture()
    let script = path_join(root, "probe_consumer.das")
    t |> run("list_modules with project_root lists the descriptor's C++ module and require path") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_list_modules(true, root), text, is_error)
        t |> success(!is_error, "should not be error: {text}")
        var err : string
        var parsed = read_json(text, err)
        t |> success(parsed != null, "json parses: {err}")
        return if (parsed == null)
        t |> success(json_array_has(parsed?["builtin"], "UnitTest"), "the C++ module the descriptor registered is a builtin")
        t |> success(json_array_has(parsed?["registered"], "unit_probe/hello"), "the require path the descriptor registered is listed")
        unsafe { delete parsed; }
    }
    t |> run("find_symbol with project_root finds a symbol from the project's own require path") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_symbol("probe_hello_value", "", script, "", root), text, is_error)
        t |> success(!is_error, "should not be error: {text}")
        t |> success(find(text, "probe_hello_value") >= 0, "should find the project's function")
    }
    t |> run("compile_check with project_root compiles a file requiring both") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_compile_check(script, "", root), text, is_error)
        t |> success(!is_error, "should not be error: {text}")
        t |> success(find(text, "Compilation OK") >= 0, "should compile: {text}")
    }
    t |> run("a plain run and an eager run of the file both reach the C++ module and the require path") <| @(t : T?) {
        let exe = get_daslang_exe()
        t |> success(!empty(exe), "daslang executable is known")
        return if (empty(exe))
        var plain_out : string
        let plain_rc = run_and_capture([exe, "-project_root", root, script], plain_out, 120.0)
        t |> equal(plain_rc, 0, "plain run exits 0: {plain_out}")
        t |> success(find(plain_out, "probe 4242 1234") >= 0, "plain run prints the probe: {plain_out}")
        var eager_out : string
        let eager_rc = run_and_capture([exe, "-ignore-manifest", "-project_root", root, script], eager_out, 120.0)
        t |> equal(eager_rc, 0, "eager run exits 0: {eager_out}")
        t |> success(find(eager_out, "probe 4242 1234") >= 0, "eager run prints the probe: {eager_out}")
    }
}

[test(description = "lint reports a missing project file and a non-project extension each with a focused error.", timeout = 1200)]
def test_lint_invalid_project(t : T?) {
    t |> run("missing project file reports focused error") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_lint(fixture_path("_fixture_valid.das"), "nonexistent_xyz.das_project"), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(is_error, "should be error")
        t |> success(find(text, "'project' file not found") >= 0, "should mention project file not found")
    }
    t |> run("non-.das_project extension reports focused error") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_lint(fixture_path("_fixture_valid.das"), fixture_path("_fixture_valid.das")), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(is_error, "should be error")
        t |> success(find(text, "expected a path to a .das_project file") >= 0, "should mention expected extension")
    }
}

[test(description = "compile_check resolves a project's own module only when project_root is given.", timeout = 1200)]
def test_project_root_compile_check(t : T?) {
    t |> run("compile_check honors project_root") <| @(t : T?) {
        assert_prereq_pair(t, "project_root",
            do_compile_check(fixture_path(PROOT_CONSUMER)),
            do_compile_check(fixture_path(PROOT_CONSUMER), "", fixture_path(PROOT)))
    }
}

[test(description = "lint resolves a project's own module only when project_root is given.", timeout = 1200)]
def test_project_root_lint(t : T?) {
    t |> run("lint honors project_root") <| @(t : T?) {
        assert_prereq_pair(t, "project_root",
            do_lint(fixture_path(PROOT_CONSUMER)),
            do_lint(fixture_path(PROOT_CONSUMER), "", fixture_path(PROOT)))
    }
}

[test(description = "aot resolves a project's own module only when project_root is given.", timeout = 1200)]
def test_project_root_aot(t : T?) {
    t |> run("aot honors project_root") <| @(t : T?) {
        assert_prereq_pair(t, "project_root",
            do_aot(fixture_path(PROOT_CONSUMER), ""),
            do_aot(fixture_path(PROOT_CONSUMER), "", "", fixture_path(PROOT)))
    }
}

[test(description = "list_functions resolves a project's own module only when project_root is given.", timeout = 1200)]
def test_project_root_list_functions(t : T?) {
    t |> run("list_functions honors project_root") <| @(t : T?) {
        assert_prereq_pair(t, "project_root",
            do_list_functions(fixture_path(PROOT_CONSUMER)),
            do_list_functions(fixture_path(PROOT_CONSUMER), "", fixture_path(PROOT)))
    }
}

[test(description = "list_types resolves a project's own module only when project_root is given.", timeout = 1200)]
def test_project_root_list_types(t : T?) {
    t |> run("list_types honors project_root") <| @(t : T?) {
        assert_prereq_pair(t, "project_root",
            do_list_types(fixture_path(PROOT_CONSUMER)),
            do_list_types(fixture_path(PROOT_CONSUMER), "", fixture_path(PROOT)))
    }
}

[test(description = "list_requires resolves a project's own module only when project_root is given.", timeout = 1200)]
def test_project_root_list_requires(t : T?) {
    t |> run("list_requires honors project_root") <| @(t : T?) {
        assert_prereq_pair(t, "project_root",
            do_list_requires(fixture_path(PROOT_CONSUMER)),
            do_list_requires(fixture_path(PROOT_CONSUMER), "", fixture_path(PROOT)))
    }
}

[test(description = "program_log resolves a project's own module only when project_root is given.", timeout = 1200)]
def test_project_root_program_log(t : T?) {
    t |> run("program_log honors project_root") <| @(t : T?) {
        assert_prereq_pair(t, "project_root",
            do_program_log(fixture_path(PROOT_CONSUMER), "main"),
            do_program_log(fixture_path(PROOT_CONSUMER), "main", "", fixture_path(PROOT)))
    }
}

[test(description = "ast_dump resolves a project's own module only when project_root is given.", timeout = 1200)]
def test_project_root_ast_dump(t : T?) {
    t |> run("ast_dump honors project_root") <| @(t : T?) {
        assert_prereq_pair(t, "project_root",
            do_ast_dump("", fixture_path(PROOT_CONSUMER), "main", "source", ""),
            do_ast_dump("", fixture_path(PROOT_CONSUMER), "main", "source", "", "", fixture_path(PROOT)))
    }
}

[test(description = "find_references resolves a project's own module only when project_root is given.", timeout = 1200)]
def test_project_root_find_references(t : T?) {
    t |> run("find_references honors project_root") <| @(t : T?) {
        assert_prereq_pair(t, "project_root",
            do_find_references(fixture_path(PROOT_CONSUMER), "7", "17", "file", ""),
            do_find_references(fixture_path(PROOT_CONSUMER), "7", "17", "file", "", "", fixture_path(PROOT)))
    }
}

[test(description = "goto_definition resolves a project's own module only when project_root is given.", timeout = 1200)]
def test_project_root_goto_definition(t : T?) {
    t |> run("goto_definition honors project_root") <| @(t : T?) {
        assert_prereq_pair(t, "project_root",
            do_goto_definition(fixture_path(PROOT_CONSUMER), "7", "17"),
            do_goto_definition(fixture_path(PROOT_CONSUMER), "7", "17", "", "", fixture_path(PROOT)))
    }
}

[test(description = "type_of resolves a project's own module only when project_root is given.", timeout = 1200)]
def test_project_root_type_of(t : T?) {
    t |> run("type_of honors project_root") <| @(t : T?) {
        assert_prereq_pair(t, "project_root",
            do_type_of(fixture_path(PROOT_CONSUMER), "7", "17"),
            do_type_of(fixture_path(PROOT_CONSUMER), "7", "17", "", "", fixture_path(PROOT)))
    }
}

[test(description = "find_symbol resolves a project's own module only when project_root is given.", timeout = 1200)]
def test_project_root_find_symbol(t : T?) {
    t |> run("find_symbol honors project_root") <| @(t : T?) {
        assert_prereq_pair(t, "project_root",
            do_find_symbol("hello", "", fixture_path(PROOT_CONSUMER)),
            do_find_symbol("hello", "", fixture_path(PROOT_CONSUMER), "", fixture_path(PROOT)))
    }
}

[test(description = "list_module_api lists a project's own module only when project_root is given.", timeout = 1200)]
def test_project_root_list_module_api(t : T?) {
    t |> run("list_module_api honors project_root") <| @(t : T?) {
        var t1 : string; var e1 = false
        parse_result(do_list_module_api("pretend_mod/greet"), t1, e1)
        t |> success(e1, "without project_root: should be error -- text={t1}")
        t |> success(find(t1, "missing prerequisite 'pretend_mod/greet") >= 0,
                     "without project_root: should mention missing prereq -- text={t1}")
        var t2 : string; var e2 = false
        parse_result(do_list_module_api("pretend_mod/greet", "", "", "", "", fixture_path(PROOT)), t2, e2)
        t |> success(!e2, "with project_root: should NOT be error -- text={t2}")
        t |> success(find(t2, "hello") >= 0,
                     "with project_root: should list hello() -- text={t2}")
    }
}

[test(description = "describe_type resolves a project's own module only when project_root is given.", timeout = 1200)]
def test_project_root_describe_type(t : T?) {
    t |> run("describe_type honors project_root") <| @(t : T?) {
        var t1 : string; var e1 = false
        parse_result(do_describe_type("FooBar", "pretend_mod/greet"), t1, e1)
        t |> success(e1, "without project_root: should be error -- text={t1}")
        t |> success(find(t1, "missing prerequisite 'pretend_mod/greet") >= 0,
                     "without project_root: should mention missing prereq -- text={t1}")
        var t2 : string; var e2 = false
        parse_result(do_describe_type("FooBar", "pretend_mod/greet", "", fixture_path(PROOT)), t2, e2)
        t |> success(!e2, "with project_root: should NOT be error -- text={t2}")
    }
}

[test(description = "eval_expression resolves a project's own module only when project_root is given.", timeout = 1200)]
def test_project_root_eval_expression(t : T?) {
    t |> run("eval_expression honors project_root") <| @(t : T?) {
        var t1 : string; var e1 = false
        parse_result(do_eval_expression("hello()", "pretend_mod/greet"), t1, e1)
        t |> success(e1, "without project_root: should be error -- text={t1}")
        t |> success(find(t1, "missing prerequisite 'pretend_mod/greet") >= 0,
                     "without project_root: should mention missing prereq -- text={t1}")
        var t2 : string; var e2 = false
        parse_result(do_eval_expression("hello()", "pretend_mod/greet", "", fixture_path(PROOT)), t2, e2)
        t |> success(!e2, "with project_root: should NOT be error -- text={t2}")
    }
}

[test(description = "run_script resolves a project's own module only when project_root is given.", timeout = 1200)]
def test_project_root_run_script(t : T?) {
    t |> run("run_script honors project_root") <| @(t : T?) {
        let inline_code = "options gen2\nrequire pretend_mod/greet\n[export]\ndef main() \{\n    print(\"\{hello()\}\\n\")\n\}\n"
        var t1 : string; var e1 = false
        parse_result(do_run_script("", inline_code), t1, e1)
        t |> success(e1, "without project_root: should be error -- text={t1}")
        t |> success(find(t1, PROOT_MISSING) >= 0,
                     "without project_root: should mention missing prereq -- text={t1}")
        var t2 : string; var e2 = false
        parse_result(do_run_script("", inline_code, "", false, "", fixture_path(PROOT)), t2, e2)
        t |> success(!e2, "with project_root: should NOT be error -- text={t2}")
    }
}

[test(description = "run_test resolves a project's own module only when project_root is given.", timeout = 1200)]
def test_project_root_run_test(t : T?) {
    t |> run("run_test honors project_root") <| @(t : T?) {
        var t1 : string; var e1 = false
        parse_result(do_run_test(fixture_path(PROOT_CONSUMER)), t1, e1)
        t |> success(e1, "without project_root: should be error -- text={t1}")
        t |> success(find(t1, PROOT_MISSING) >= 0,
                     "without project_root: should mention missing prereq -- text={t1}")
        var t2 : string; var e2 = false
        parse_result(do_run_test(fixture_path(PROOT_CONSUMER), "", fixture_path(PROOT)), t2, e2)
        t |> success(find(t2, PROOT_MISSING) < 0,
                     "with project_root: should NOT mention missing prereq -- text={t2}")
    }
}

[test(description = "A project_root that does not exist leaves the project's module unresolved and says so.", timeout = 1200)]
def test_project_root_invalid_dir(t : T?) {
    t |> run("invalid project_root: pins current daslang behavior") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_compile_check(fixture_path(PROOT_CONSUMER), "",
                                      fixture_path("nonexistent_xyz_pretend_root")),
                     text, is_error)
        t |> success(is_error, "should be error -- text={text}")
        t |> success(find(text, PROOT_MISSING) >= 0,
                     "should still report missing prereq -- text={text}")
    }
}
