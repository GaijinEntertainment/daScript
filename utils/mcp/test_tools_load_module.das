options gen2
options rtti

require dastest/testing_boost public
require strings
require _tools_common

require tools/compile_check public
require tools/list_functions public
require tools/list_types public
require tools/run_script public
require tools/run_test public
require tools/ast_dump public
require tools/find_symbol public
require tools/list_module_api public
require tools/list_requires public
require tools/goto_definition public
require tools/type_of public
require tools/find_references public
require tools/eval_expression public
require tools/describe_type public
require tools/aot public
require tools/lint_tool public
require tools/program_log public

[test(description = "compile_check resolves a module only when it is passed as a load_module.", timeout = 1200)]
def test_load_module_compile_check(t : T?) {
    t |> run("compile_check honors load_module") <| @(t : T?) {
        assert_prereq_pair(t, "load_module",
            do_compile_check(fixture_path(PROOT_CONSUMER)),
            do_compile_check(fixture_path(PROOT_CONSUMER), "", "", false, make_lm_array(fixture_path(PRETEND_LM))))
    }
}

[test(description = "lint resolves a module only when it is passed as a load_module.", timeout = 1200)]
def test_load_module_lint(t : T?) {
    t |> run("lint honors load_module") <| @(t : T?) {
        assert_prereq_pair(t, "load_module",
            do_lint(fixture_path(PROOT_CONSUMER)),
            do_lint(fixture_path(PROOT_CONSUMER), "", "", make_lm_array(fixture_path(PRETEND_LM))))
    }
}

[test(description = "aot resolves a module only when it is passed as a load_module.", timeout = 1200)]
def test_load_module_aot(t : T?) {
    t |> run("aot honors load_module") <| @(t : T?) {
        assert_prereq_pair(t, "load_module",
            do_aot(fixture_path(PROOT_CONSUMER), ""),
            do_aot(fixture_path(PROOT_CONSUMER), "", "", "", make_lm_array(fixture_path(PRETEND_LM))))
    }
}

[test(description = "list_functions resolves a module only when it is passed as a load_module.", timeout = 1200)]
def test_load_module_list_functions(t : T?) {
    t |> run("list_functions honors load_module") <| @(t : T?) {
        assert_prereq_pair(t, "load_module",
            do_list_functions(fixture_path(PROOT_CONSUMER)),
            do_list_functions(fixture_path(PROOT_CONSUMER), "", "", make_lm_array(fixture_path(PRETEND_LM))))
    }
}

[test(description = "list_types resolves a module only when it is passed as a load_module.", timeout = 1200)]
def test_load_module_list_types(t : T?) {
    t |> run("list_types honors load_module") <| @(t : T?) {
        assert_prereq_pair(t, "load_module",
            do_list_types(fixture_path(PROOT_CONSUMER)),
            do_list_types(fixture_path(PROOT_CONSUMER), "", "", make_lm_array(fixture_path(PRETEND_LM))))
    }
}

[test(description = "list_requires resolves a module only when it is passed as a load_module.", timeout = 1200)]
def test_load_module_list_requires(t : T?) {
    t |> run("list_requires honors load_module") <| @(t : T?) {
        assert_prereq_pair(t, "load_module",
            do_list_requires(fixture_path(PROOT_CONSUMER)),
            do_list_requires(fixture_path(PROOT_CONSUMER), "", "", false, make_lm_array(fixture_path(PRETEND_LM))))
    }
}

[test(description = "program_log resolves a module only when it is passed as a load_module.", timeout = 1200)]
def test_load_module_program_log(t : T?) {
    t |> run("program_log honors load_module") <| @(t : T?) {
        assert_prereq_pair(t, "load_module",
            do_program_log(fixture_path(PROOT_CONSUMER), "main"),
            do_program_log(fixture_path(PROOT_CONSUMER), "main", "", "", make_lm_array(fixture_path(PRETEND_LM))))
    }
}

[test(description = "ast_dump resolves a module only when it is passed as a load_module.", timeout = 1200)]
def test_load_module_ast_dump(t : T?) {
    t |> run("ast_dump honors load_module") <| @(t : T?) {
        assert_prereq_pair(t, "load_module",
            do_ast_dump("", fixture_path(PROOT_CONSUMER), "main", "source", ""),
            do_ast_dump("", fixture_path(PROOT_CONSUMER), "main", "source", "", "", "", make_lm_array(fixture_path(PRETEND_LM))))
    }
}

[test(description = "find_references resolves a module only when it is passed as a load_module.", timeout = 1200)]
def test_load_module_find_references(t : T?) {
    t |> run("find_references honors load_module") <| @(t : T?) {
        assert_prereq_pair(t, "load_module",
            do_find_references(fixture_path(PROOT_CONSUMER), "7", "17", "file", ""),
            do_find_references(fixture_path(PROOT_CONSUMER), "7", "17", "file", "", "", "", make_lm_array(fixture_path(PRETEND_LM))))
    }
}

[test(description = "goto_definition resolves a module only when it is passed as a load_module.", timeout = 1200)]
def test_load_module_goto_definition(t : T?) {
    t |> run("goto_definition honors load_module") <| @(t : T?) {
        assert_prereq_pair(t, "load_module",
            do_goto_definition(fixture_path(PROOT_CONSUMER), "7", "17"),
            do_goto_definition(fixture_path(PROOT_CONSUMER), "7", "17", "", "", "", false, "", make_lm_array(fixture_path(PRETEND_LM))))
    }
}

[test(description = "type_of resolves a module only when it is passed as a load_module.", timeout = 1200)]
def test_load_module_type_of(t : T?) {
    t |> run("type_of honors load_module") <| @(t : T?) {
        assert_prereq_pair(t, "load_module",
            do_type_of(fixture_path(PROOT_CONSUMER), "7", "17"),
            do_type_of(fixture_path(PROOT_CONSUMER), "7", "17", "", "", "", make_lm_array(fixture_path(PRETEND_LM))))
    }
}

[test(description = "find_symbol resolves a module only when it is passed as a load_module.", timeout = 1200)]
def test_load_module_find_symbol(t : T?) {
    t |> run("find_symbol honors load_module") <| @(t : T?) {
        assert_prereq_pair(t, "load_module",
            do_find_symbol("hello", "", fixture_path(PROOT_CONSUMER)),
            do_find_symbol("hello", "", fixture_path(PROOT_CONSUMER), "", "", false, "", make_lm_array(fixture_path(PRETEND_LM))))
    }
}

[test(description = "list_module_api lists a module only when it is passed as a load_module.", timeout = 1200)]
def test_load_module_list_module_api(t : T?) {
    t |> run("list_module_api honors load_module") <| @(t : T?) {
        var t1 : string; var e1 = false
        parse_result(do_list_module_api("pretend_mod/greet"), t1, e1)
        t |> success(e1, "without load_module: should be error -- text={t1}")
        t |> success(find(t1, "missing prerequisite 'pretend_mod/greet") >= 0,
                     "without load_module: should mention missing prereq -- text={t1}")
        var t2 : string; var e2 = false
        parse_result(do_list_module_api("pretend_mod/greet", "", "", "", "", "", make_lm_array(fixture_path(PRETEND_LM))), t2, e2)
        t |> success(!e2, "with load_module: should NOT be error -- text={t2}")
        t |> success(find(t2, "hello") >= 0,
                     "with load_module: should list hello() -- text={t2}")
    }
}

[test(description = "describe_type resolves a module only when it is passed as a load_module.", timeout = 1200)]
def test_load_module_describe_type(t : T?) {
    t |> run("describe_type honors load_module") <| @(t : T?) {
        var t1 : string; var e1 = false
        parse_result(do_describe_type("FooBar", "pretend_mod/greet"), t1, e1)
        t |> success(e1, "without load_module: should be error -- text={t1}")
        t |> success(find(t1, "missing prerequisite 'pretend_mod/greet") >= 0,
                     "without load_module: should mention missing prereq -- text={t1}")
        var t2 : string; var e2 = false
        parse_result(do_describe_type("FooBar", "pretend_mod/greet", "", "", make_lm_array(fixture_path(PRETEND_LM))), t2, e2)
        t |> success(!e2, "with load_module: should NOT be error -- text={t2}")
    }
}

[test(description = "eval_expression resolves a module only when it is passed as a load_module.", timeout = 1200)]
def test_load_module_eval_expression(t : T?) {
    t |> run("eval_expression honors load_module") <| @(t : T?) {
        var t1 : string; var e1 = false
        parse_result(do_eval_expression("hello()", "pretend_mod/greet"), t1, e1)
        t |> success(e1, "without load_module: should be error -- text={t1}")
        t |> success(find(t1, "missing prerequisite 'pretend_mod/greet") >= 0,
                     "without load_module: should mention missing prereq -- text={t1}")
        var t2 : string; var e2 = false
        parse_result(do_eval_expression("hello()", "pretend_mod/greet", "", "", make_lm_array(fixture_path(PRETEND_LM))), t2, e2)
        t |> success(!e2, "with load_module: should NOT be error -- text={t2}")
    }
}

[test(description = "run_script resolves a module only when it is passed as a load_module.", timeout = 1200)]
def test_load_module_run_script(t : T?) {
    t |> run("run_script honors load_module") <| @(t : T?) {
        let inline_code = "options gen2\nrequire pretend_mod/greet\n[export]\ndef main() \{\n    print(\"\{hello()\}\\n\")\n\}\n"
        var t1 : string; var e1 = false
        parse_result(do_run_script("", inline_code), t1, e1)
        t |> success(e1, "without load_module: should be error -- text={t1}")
        t |> success(find(t1, PROOT_MISSING) >= 0,
                     "without load_module: should mention missing prereq -- text={t1}")
        var t2 : string; var e2 = false
        parse_result(do_run_script("", inline_code, "", false, "", "", make_lm_array(fixture_path(PRETEND_LM))), t2, e2)
        t |> success(!e2, "with load_module: should NOT be error -- text={t2}")
    }
}

[test(description = "run_test resolves a module only when it is passed as a load_module.", timeout = 1200)]
def test_load_module_run_test(t : T?) {
    t |> run("run_test honors load_module") <| @(t : T?) {
        var t1 : string; var e1 = false
        parse_result(do_run_test(fixture_path(PROOT_CONSUMER)), t1, e1)
        t |> success(e1, "without load_module: should be error -- text={t1}")
        t |> success(find(t1, PROOT_MISSING) >= 0,
                     "without load_module: should mention missing prereq -- text={t1}")
        var t2 : string; var e2 = false
        parse_result(do_run_test(fixture_path(PROOT_CONSUMER), "", "", "", false, false, make_lm_array(fixture_path(PRETEND_LM))), t2, e2)
        t |> success(find(t2, PROOT_MISSING) < 0,
                     "with load_module: should NOT mention missing prereq -- text={t2}")
    }
}
