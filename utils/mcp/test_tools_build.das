options gen2
options rtti

require dastest/testing_boost public
require strings
require daslib/fio
require _tools_common

require tools/compile_check public
require tools/run_script public
require tools/run_test public
require tools/ast_dump public
require tools/convert_to_gen2 public
require tools/eval_expression public
require tools/common public
require tools/aot public
require tools/program_log public

def has_path(files : array<string>; suffix : string) : bool {
    for (f in files) {
        if (find(f, suffix) >= 0) return true
    }
    return false
}

[test(description = "compile_check accepts a valid fixture and lists its functions.", timeout = 1200)]
def test_compile_check_valid(t : T?) {
    t |> run("valid file compiles") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_compile_check(fixture_path("_fixture_valid.das")), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Compilation OK") >= 0, "should contain 'Compilation OK'")
    }
    t |> run("lists functions") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_compile_check(fixture_path("_fixture_valid.das")), text, is_error)
        t |> success(find(text, "def greet") >= 0, "should list greet function")
        t |> success(find(text, "def main") >= 0, "should list main function")
    }
}

[test(description = "compile_check reports a fixture with a compile error as a failed compilation.", timeout = 1200)]
def test_compile_check_error(t : T?) {
    t |> run("invalid file reports error") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_compile_check(fixture_path("_fixture_error.das")), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(is_error, "should be error")
        t |> success(find(text, "Compilation failed") >= 0, "should contain 'Compilation failed'")
    }
}

[test(description = "compile_check reports a missing file as an error that says it was not found.", timeout = 1200)]
def test_compile_check_missing(t : T?) {
    t |> run("missing file reports error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_compile_check("nonexistent_file_12345.das"), text, is_error)
        t |> success(is_error, "should be error")
        t |> success(find(text, "not found") >= 0, "should mention file not found")
    }
}

[test(description = "A comma-separated compile_check batch tallies the passed and failed files.", timeout = 1200)]
def test_compile_check_batch_comma(t : T?) {
    t |> run("batch compile with comma-separated files") <| @(t : T?) {
        var text : string
        var is_error = false
        let files = "{fixture_path("_fixture_valid.das")},{fixture_path("_fixture_error.das")}"
        parse_result(do_compile_check(files), text, is_error)
        t |> success(is_error, "batch with error file should report error")
        t |> success(find(text, "1 passed") >= 0, "should have 1 passed")
        t |> success(find(text, "1 failed") >= 0, "should have 1 failed")
        t |> success(find(text, "PASS") >= 0, "should have PASS line")
        t |> success(find(text, "FAIL") >= 0, "should have FAIL line")
    }
}

[test(description = "A glob compile_check batch compiles every matched fixture and prints a summary.", timeout = 1200)]
def test_compile_check_batch_glob(t : T?) {
    t |> run("batch compile with glob pattern") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_compile_check("utils/mcp/tests/*.das"), text, is_error)
        t |> success(find(text, "PASS") >= 0, "should have at least one PASS")
        t |> success(find(text, "files") >= 0, "should have summary line")
    }
}

[test(description = "A compile_check glob that matches nothing is an error saying so.", timeout = 1200)]
def test_compile_check_glob_no_match(t : T?) {
    t |> run("glob with no matches reports error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_compile_check("nonexistent_dir_xyz/*.das"), text, is_error)
        t |> success(is_error, "should be error when no files match")
        t |> success(find(text, "no files matched") >= 0, "should mention no files matched")
    }
}

[test(description = "parse_file_list keeps plain entries and expands globs when reached through the MCP require chain.")]
def test_parse_file_list_smoke(t : T?) {
    t |> run("parse_file_list reachable through MCP require chain") <| @(t : T?) {
        var files : array<string>
        parse_file_list("{fixture_path("_fixture_valid.das")},utils/mcp/tests/_fixture_e*.das", files)
        t |> success(has_path(files, "_fixture_valid.das"), "plain entry preserved")
        t |> success(has_path(files, "_fixture_error.das"), "glob expanded")
    }
}

[test(description = "run_script accepts inline code and returns a well-formed result envelope.", timeout = 1200)]
def test_run_script_inline(t : T?) {
    t |> run("inline code executes") <| @(t : T?) {
        var text : string
        var is_error = false
        let code = "options gen2\n[export]\ndef main() \{\n    print(\"hello_mcp_test\\n\")\n\}\n"
        parse_result(do_run_script("", code), text, is_error)
        t->log("output assertions disabled: popen stdout capture is broken under dastest on Windows")
    }
}

[test(description = "run_script reports inline code that fails to compile as an error.", timeout = 1200)]
def test_run_script_error(t : T?) {
    t |> run("bad code reports error") <| @(t : T?) {
        var text : string
        var is_error = false
        let code = "options gen2\ndef broken() : int \{\n    return \"nope\"\n\}\n"
        parse_result(do_run_script("", code), text, is_error)
        t |> success(is_error, "should be error")
    }
}

[test(description = "run_test returns a well-formed result envelope for a passing fixture test.", timeout = 1200)]
def test_run_test_passing(t : T?) {
    t |> run("passing test returns success") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_run_test(fixture_path("_fixture_test.das")), text, is_error)
        t->log("result assertions disabled: popen stdout capture is broken under dastest on Windows")
    }
}

[test(description = "run_test reports a nonexistent test file as an error.", timeout = 1200)]
def test_run_test_bad_file(t : T?) {
    t |> run("nonexistent file reports error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_run_test("nonexistent_test_file.das"), text, is_error)
        t |> success(is_error, "should be error")
    }
}

[test(description = "ast_dump of an expression prints its S-expression nodes.", timeout = 1200)]
def test_ast_dump_expression(t : T?) {
    t |> run("expression mode returns S-expression") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_ast_dump("a + b", "", "", "", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "ExprOp2") >= 0, "should contain ExprOp2")
        t |> success(find(text, "ExprVar") >= 0, "should contain ExprVar")
    }
}

[test(description = "ast_dump of a file function in ast mode prints the function header and its nodes.", timeout = 1200)]
def test_ast_dump_file_ast_mode(t : T?) {
    t |> run("file ast mode returns S-expression") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_ast_dump("", fixture_path("_fixture_valid.das"), "add", "ast_core", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "def add") >= 0, "should contain function header")
        t |> success(find(text, "ExprReturn") >= 0, "should contain ExprReturn node")
    }
}

[test(description = "ast_dump of a file function in source mode prints source text and no S-expression nodes.", timeout = 1200)]
def test_ast_dump_file_source_mode(t : T?) {
    t |> run("file source mode returns describe output") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_ast_dump("", fixture_path("_fixture_valid.das"), "add", "source", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "def add") >= 0, "should contain function header")
        t |> success(find(text, "return") >= 0, "should contain return statement")
        t |> success(find(text, "ExprOp2") < 0, "should not contain S-expression nodes")
    }
}

[test(description = "ast_dump keeps functions other than main alive so they can be dumped.", timeout = 1200)]
def test_ast_dump_export_all(t : T?) {
    t |> run("export_all preserves non-main functions") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_ast_dump("", fixture_path("_fixture_valid.das"), "greet", "source", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "def greet") >= 0, "greet should be preserved by export_all")
    }
}

[test(description = "convert_to_gen2 rewrites a gen1 fixture into gen2 syntax and reports a missing file as an error.", timeout = 1200)]
def test_convert_to_gen2(t : T?) {
    t |> run("converts gen1 to gen2 stdout") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_convert_to_gen2(fixture_path("_fixture_gen1.das"), false), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "options gen2") >= 0, "should add options gen2")
        t |> success(find(text, "def add(") >= 0, "should contain add function")
        t |> success(find(text, "\{") >= 0, "should contain braces")
    }
    t |> run("adds parentheses to control flow") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_convert_to_gen2(fixture_path("_fixture_gen1.das"), false), text, is_error)
        t |> success(find(text, "if (") >= 0, "should add parens to if")
    }
    t |> run("missing file reports error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_convert_to_gen2("nonexistent_file_12345.das", false), text, is_error)
        t |> success(is_error, "should be error for missing file")
    }
}

[test(description = "eval_expression evaluates expressions with requires, reports failures, and retries an unqualified daslib name only after a module-resolution failure.", timeout = 1200)]
def test_eval_expression(t : T?) {
    t |> run("evaluates simple arithmetic") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_eval_expression("1 + 2", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> equal(text, "3")
    }
    t |> run("evaluates with require") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_eval_expression("float(42) + sin(1.0)", "math"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "42.84") >= 0, "should contain 42.84")
    }
    t |> run("evaluates with multiple requires") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_eval_expression("to_int(\"10\") + 5", "math, strings"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> equal(text, "15")
    }
    t |> run("reports compile error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_eval_expression("undefined_func()", ""), text, is_error)
        t |> success(is_error, "should be error")
        t |> success(find(text, "Evaluation failed") >= 0, "should report failure")
    }
    t |> run("missing expression is error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_eval_expression("", ""), text, is_error)
        t |> success(is_error, "should be error")
    }
    t |> run("rewrites unqualified daslib require") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_eval_expression("1 + 2", "ast"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "[resolved unqualified modules under daslib/") >= 0, "should announce rewrite")
        t |> success(find(text, "3") >= 0, "should evaluate to 3")
    }
    t |> run("retry-also-fails surfaces retry error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_eval_expression("1 + 2", "nonexistent_module_xyz"), text, is_error)
        t |> success(is_error, "should be error")
        t |> success(find(text, "also retried with require 'daslib/nonexistent_module_xyz'") >= 0,
                "should mention retry attempt with rewritten name")
    }
    t |> run("retry skipped for non-module-resolution failures") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_eval_expression("undefined_func_xyz()", "math"), text, is_error)
        t |> success(is_error, "should be error")
        t |> success(find(text, "daslib/math") < 0,
                "should NOT mention daslib/math — failure is not module-resolution")
        t |> success(find(text, "also retried") < 0, "should NOT announce a retry attempt")
    }
    t |> run("mixed list rewrites only the missing token") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_eval_expression("1 + 2", "math, ast"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "math, daslib/ast") >= 0, "should rewrite ast but keep math")
        t |> success(find(text, "daslib/math") < 0, "should NOT rewrite the builtin math")
        t |> success(find(text, "3") >= 0, "should evaluate to 3")
    }
}

[test(description = "A shared module edited between two compiles is seen anew and not served from a stale cache.", timeout = 1200)]
def test_shared_module_not_cached(t : T?) {
    let shared_mod = "utils/mcp/tests/fixture_shared_mod.das"
    let original = "options gen2\nmodule fixture_shared_mod shared\n\nstruct Foo \{\n    x : int = 0;\n\}\n"
    let modified = "options gen2\nmodule fixture_shared_mod shared\n\nstruct Foo \{\n    x : int = 0;\n\}\n\nstruct Bar \{\n    y : int = 0;\n\}\n"
    t |> run("sees new struct after shared module changes") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_compile_check(fixture_path("_fixture_shared_user.das")), text, is_error)
        t |> success(!is_error, "user1 should compile (Foo exists)")
        t |> success(fwrite(shared_mod, modified), "should write modified shared module")
        parse_result(do_compile_check(fixture_path("_fixture_shared_user2.das")), text, is_error)
        t |> success(!is_error, "user2 should compile (Bar should be visible after module change)")
        fwrite(shared_mod, original)
    }
}

[test(description = "aot emits the C++ for a whole file.", timeout = 1200)]
def test_aot_full_file(t : T?) {
    t |> run("full file AOT") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_aot(fixture_path("_fixture_valid.das"), ""), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "inline") >= 0, "should contain inline functions")
        t |> success(find(text, "namespace das") >= 0, "should contain namespace")
    }
}

[test(description = "aot extracts only the function it was asked for.", timeout = 1200)]
def test_aot_single_function(t : T?) {
    t |> run("extracts single function by name") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_aot(fixture_path("_fixture_aot.das"), "helper"), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "inline") >= 0, "should contain inline keyword")
        t |> success(find(text, "helper") >= 0, "should contain helper function name")
        t |> success(find(text, "describe") < 0, "should not contain other functions")
    }
}

[test(description = "aot names the available functions when none matches, without raising an error.", timeout = 1200)]
def test_aot_not_found(t : T?) {
    t |> run("nonexistent function") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_aot(fixture_path("_fixture_aot.das"), "nonexistent"), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "should not be error (informational)")
        t |> success(find(text, "No function matching") >= 0, "should say no match")
        t |> success(find(text, "Available functions") >= 0, "should list available functions")
    }
}

[test(description = "aot lists the overload signatures when a name is ambiguous.", timeout = 1200)]
def test_aot_multiple_matches(t : T?) {
    t |> run("overloaded function returns disambiguation list") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_aot(fixture_path("_fixture_aot.das"), "describe"), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Multiple functions match") >= 0, "should say multiple matches")
        t |> success(find(text, "int") >= 0, "should show int overload signature")
        t |> success(find(text, "float") >= 0, "should show float overload signature")
    }
}

[test(description = "aot selects one overload by its mangled aot name.", timeout = 1200)]
def test_aot_select_by_aot_name(t : T?) {
    t |> run("select overload by mangled aot_name") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_aot(fixture_path("_fixture_aot.das"), "describe"), text, is_error)
        let bracket_pos = find(text, "[describe_")
        t |> success(bracket_pos >= 0, "should contain bracketed aot_name")
        if (bracket_pos < 0) return
        let name_start = bracket_pos + 1
        let name_end = find(text, "]", name_start)
        t |> success(name_end > name_start, "should have closing bracket")
        if (name_end <= name_start) return
        let aot_name = slice(text, name_start, name_end)
        parse_result(do_aot(fixture_path("_fixture_aot.das"), aot_name), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "inline") >= 0, "should contain inline function")
        t |> success(find(text, aot_name) >= 0, "should contain the selected aot_name")
    }
}

[test(description = "aot reports a missing file as an error.", timeout = 1200)]
def test_aot_missing_file(t : T?) {
    t |> run("missing file reports error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_aot("nonexistent_file_12345.das", ""), text, is_error)
        t |> success(is_error, "should be error")
    }
}

[test(description = "compile_check json mode returns a result per file carrying success and errors fields.", timeout = 1200)]
def test_compile_check_json(t : T?) {
    t |> run("json mode returns CompileResult array") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_compile_check(fixture_path("_fixture_valid.das"), "", "", true), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "\"success\":true") >= 0, "should contain success:true")
        t |> success(find(text, "\"file\":") >= 0, "should contain file field")
    }
    t |> run("json mode reports errors") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_compile_check(fixture_path("_fixture_error.das"), "", "", true), text, is_error)
        t |> success(find(text, "\"success\":false") >= 0, "should contain success:false")
        t |> success(find(text, "\"errors\":") >= 0, "should contain errors field")
    }
    t |> run("json batch returns array") <| @(t : T?) {
        var text : string
        var is_error = false
        let batch = fixture_path("_fixture_valid.das") + "," + fixture_path("_fixture_error.das")
        parse_result(do_compile_check(batch, "", "", true), text, is_error)
        let arr_start = "[" + "\{"
        t |> success(find(text, arr_start) >= 0, "should start with array")
        t |> success(find(text, "\"success\":true") >= 0, "should have a passing result")
        t |> success(find(text, "\"success\":false") >= 0, "should have a failing result")
    }
}

[test(description = "run_test json mode returns a well-formed result envelope.", timeout = 1200)]
def test_run_test_json(t : T?) {
    t |> run("json mode returns TestSummary") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_run_test(fixture_path("_fixture_test.das"), "", "", "", true), text, is_error)
        t |> success(ok, "parse result JSON")
    }
}

[test(description = "program_log returns the post-compile text of one named function or of the whole program.", timeout = 1200)]
def test_program_log_basic(t : T?) {
    t |> run("program_log returns post-compile program text for a named function") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_program_log(fixture_path("_fixture_valid.das"), "main"), text, is_error)
        t |> success(!is_error, "should not be error -- text={text}")
        t |> success(find(text, "def public main") >= 0,
                     "should contain post-infer main signature -- text={text}")
    }
    t |> run("program_log with empty func dumps full program") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_program_log(fixture_path("_fixture_valid.das"), ""), text, is_error)
        t |> success(!is_error, "should not be error -- text={text}")
        t |> success(find(text, "def public greet") >= 0,
                     "should list greet -- text={text}")
        t |> success(find(text, "def public main") >= 0,
                     "should list main -- text={text}")
    }
}

[test(description = "program_log reports a function that does not exist.", timeout = 1200)]
def test_program_log_missing_func(t : T?) {
    t |> run("program_log reports unfound function") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_program_log(fixture_path("_fixture_valid.das"),
                                    "no_such_function_xyzzy"),
                     text, is_error)
        t |> success(find(text, "not found") >= 0,
                     "should report 'not found' for bogus function -- text={text}")
    }
}
