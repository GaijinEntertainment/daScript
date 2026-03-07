options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require daslib/json
require daslib/json_boost
require strings

require tools/compile_check public
require tools/list_functions public
require tools/list_types public
require tools/run_script public
require tools/format_file public
require tools/ast_dump public
require tools/list_modules public
require tools/find_symbol public
require tools/list_module_api public
require tools/list_requires public
require tools/convert_to_gen2 public
require tools/goto_definition public
require tools/type_of public
require tools/find_references public

// helper: parse tool result JSON, return (text, isError)
def parse_result(result : string; var text : string&; var is_error : bool&) : bool {
    var err : string
    var parsed = read_json(result, err)
    if (parsed == null) {
        return false
    }
    // result is {"content":[{"type":"text","text":"..."}],"isError":false}
    // access content[0].text via JSON path
    let content = parsed?["content"]
    if (content == null) {
        unsafe { delete parsed; }
        return false
    }
    let first = content?[0]
    if (first == null) {
        unsafe { delete parsed; }
        return false
    }
    let text_val = first?["text"]
    if (text_val != null) {
        text = text_val.value as _string
    }
    let err_val = parsed?["isError"]
    if (err_val != null) {
        is_error = err_val.value as _bool
    }
    unsafe { delete parsed; }
    return true
}

def fixture_path(name : string) : string {
    return "utils/mcp/tests/{name}"
}

// ── compile_check ────────────────────────────────────────────────────

[test]
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

[test]
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

[test]
def test_compile_check_missing(t : T?) {
    t |> run("missing file reports error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_compile_check("nonexistent_file_12345.das"), text, is_error)
        t |> success(is_error, "should be error")
        t |> success(find(text, "not found") >= 0, "should mention file not found")
    }
}

// ── list_functions ───────────────────────────────────────────────────

[test]
def test_list_functions(t : T?) {
    t |> run("lists user functions") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_list_functions(fixture_path("_fixture_valid.das")), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "def greet") >= 0, "should list greet")
        t |> success(find(text, "def main") >= 0, "should list main")
    }
}

// ── list_types ───────────────────────────────────────────────────────

[test]
def test_list_types(t : T?) {
    t |> run("lists structs, enums, classes") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_list_types(fixture_path("_fixture_valid.das")), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Point") >= 0, "should list Point struct")
        t |> success(find(text, "Color") >= 0, "should list Color enum")
        t |> success(find(text, "Animal") >= 0, "should list Animal class")
    }
}

// ── run_script ───────────────────────────────────────────────────────

[test]
def test_run_script_inline(t : T?) {
    t |> run("inline code executes") <| @(t : T?) {
        var text : string
        var is_error = false
        let code = "options gen2\n[export]\ndef main() \{\n    print(\"hello_mcp_test\\n\")\n\}\n"
        parse_result(do_run_script("", code), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "hello_mcp_test") >= 0, "should contain output")
    }
}

[test]
def test_run_script_error(t : T?) {
    t |> run("bad code reports error") <| @(t : T?) {
        var text : string
        var is_error = false
        let code = "options gen2\ndef broken() : int \{\n    return \"nope\"\n\}\n"
        parse_result(do_run_script("", code), text, is_error)
        t |> success(is_error, "should be error")
    }
}

// ── ast_dump ─────────────────────────────────────────────────────────

[test]
def test_ast_dump_expression(t : T?) {
    t |> run("expression mode returns S-expression") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_ast_dump("a + b", "", "", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "ExprOp2") >= 0, "should contain ExprOp2")
        t |> success(find(text, "ExprVar") >= 0, "should contain ExprVar")
    }
}

[test]
def test_ast_dump_file_ast_mode(t : T?) {
    t |> run("file ast mode returns S-expression") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_ast_dump("", fixture_path("_fixture_valid.das"), "add", "ast"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "def add") >= 0, "should contain function header")
        t |> success(find(text, "ExprReturn") >= 0, "should contain ExprReturn node")
    }
}

[test]
def test_ast_dump_file_source_mode(t : T?) {
    t |> run("file source mode returns describe output") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_ast_dump("", fixture_path("_fixture_valid.das"), "add", "source"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "def add") >= 0, "should contain function header")
        t |> success(find(text, "return") >= 0, "should contain return statement")
        // source mode should NOT have ExprOp2 etc.
        t |> success(find(text, "ExprOp2") < 0, "should not contain S-expression nodes")
    }
}

[test]
def test_ast_dump_export_all(t : T?) {
    t |> run("export_all preserves non-main functions") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_ast_dump("", fixture_path("_fixture_valid.das"), "greet", "source"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "def greet") >= 0, "greet should be preserved by export_all")
    }
}

// ── list_modules ─────────────────────────────────────────────────────

[test]
def test_list_modules(t : T?) {
    t |> run("lists builtin modules") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_list_modules(), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "math") >= 0, "should list math module")
        t |> success(find(text, "strings") >= 0, "should list strings module")
    }
    t |> run("lists daslib modules") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_list_modules(), text, is_error)
        t |> success(find(text, "json") >= 0, "should list json module")
    }
}

// ── find_symbol ──────────────────────────────────────────────────────

[test]
def test_find_symbol(t : T?) {
    t |> run("finds builtin function") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_symbol("print", "", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "print") >= 0, "should find print")
    }
    t |> run("case-insensitive search") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_symbol("PRINT", "", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "print") >= 0, "should find print case-insensitively")
    }
    t |> run("exact match with = prefix") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_symbol("=print", "", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "print") >= 0, "should find exact 'print'")
    }
    t |> run("finds typedef aliases") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_symbol("Flags", "typedef", fixture_path("_fixture_refs.das")), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "typedef") >= 0, "should find typedef")
        t |> success(find(text, "Flags") >= 0, "should name Flags")
        t |> success(find(text, "bitfield") >= 0, "should show bitfield type")
    }
}

// ── list_module_api ──────────────────────────────────────────────────

[test]
def test_list_module_api(t : T?) {
    t |> run("lists math module functions") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_list_module_api("math", "", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "sin") >= 0, "should list sin function")
        t |> success(find(text, "cos") >= 0, "should list cos function")
    }
}

// ── format_file ──────────────────────────────────────────────────────

[test]
def test_format_file(t : T?) {
    t |> run("formats already-formatted file") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_format_file(fixture_path("_fixture_valid.das")), text, is_error)
        t |> success(!is_error, "should not be error")
        // might be "already formatted" or "Formatted" depending on state
        t |> success(find(text, "ormatted") >= 0, "should mention formatting result")
    }
}

// ── list_requires ────────────────────────────────────────────────────

[test]
def test_list_requires(t : T?) {
    t |> run("lists direct requires") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_list_requires(fixture_path("_fixture_requires.das")), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Direct requires") >= 0, "should have Direct requires section")
        t |> success(find(text, "require math") >= 0, "should list math")
        t |> success(find(text, "require strings") >= 0, "should list strings")
    }
    t |> run("marks public requires") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_list_requires(fixture_path("_fixture_requires.das")), text, is_error)
        t |> success(find(text, "strings public") >= 0, "should mark strings as public")
    }
    t |> run("annotates builtin modules") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_list_requires(fixture_path("_fixture_requires.das")), text, is_error)
        t |> success(find(text, "// builtin") >= 0, "should annotate builtin modules")
    }
    t |> run("file with no explicit requires") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_list_requires(fixture_path("_fixture_valid.das")), text, is_error)
        t |> success(!is_error, "should not be error")
    }
}

// ── convert_to_gen2 ─────────────────────────────────────────────────

[test]
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

// ── goto_definition ──────────────────────────────────────────────────

[test]
def test_goto_definition_function(t : T?) {
    t |> run("resolves function call to definition") <| @(t : T?) {
        var text : string
        var is_error = false
        // distance() call at line 33, col 13
        parse_result(do_goto_definition(fixture_path("_fixture_goto.das"), "33", "13"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Kind: function") >= 0, "should be function kind")
        t |> success(find(text, "Symbol: distance") >= 0, "should resolve to distance")
        t |> success(find(text, "Line: 14") >= 0, "should point to line 14")
    }
}

[test]
def test_goto_definition_variable(t : T?) {
    t |> run("resolves variable reference to declaration") <| @(t : T?) {
        var text : string
        var is_error = false
        // p1 variable at line 33, col 22
        parse_result(do_goto_definition(fixture_path("_fixture_goto.das"), "33", "22"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Kind: variable") >= 0, "should be variable kind")
        t |> success(find(text, "Symbol: p1") >= 0, "should resolve to p1")
        t |> success(find(text, "Line: 31") >= 0, "should point to line 31")
    }
}

[test]
def test_goto_definition_field(t : T?) {
    t |> run("resolves field access to struct field") <| @(t : T?) {
        var text : string
        var is_error = false
        // a.x field at line 15, col 14
        parse_result(do_goto_definition(fixture_path("_fixture_goto.das"), "15", "14"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Kind: field") >= 0, "should be field kind")
        t |> success(find(text, "Point.x") >= 0, "should resolve to Point.x")
        t |> success(find(text, "Line: 10") >= 0, "should point to line 10")
    }
}

[test]
def test_goto_definition_builtin(t : T?) {
    t |> run("resolves builtin function") <| @(t : T?) {
        var text : string
        var is_error = false
        // print() at line 34, col 5
        parse_result(do_goto_definition(fixture_path("_fixture_goto.das"), "34", "5"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Kind: builtin") >= 0, "should be builtin kind")
        t |> success(find(text, "print") >= 0, "should resolve to print")
    }
}

[test]
def test_goto_definition_no_expression(t : T?) {
    t |> run("reports no expression at empty position") <| @(t : T?) {
        var text : string
        var is_error = false
        // line 1 col 1 — options gen2, no expression
        parse_result(do_goto_definition(fixture_path("_fixture_goto.das"), "1", "1"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "No expression found") >= 0, "should report no expression")
    }
}

[test]
def test_goto_definition_snippet(t : T?) {
    t |> run("includes source snippet") <| @(t : T?) {
        var text : string
        var is_error = false
        // distance() at line 33
        parse_result(do_goto_definition(fixture_path("_fixture_goto.das"), "33", "13"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "def distance") >= 0, "snippet should contain function definition")
    }
}

// ── type_of ──────────────────────────────────────────────────────────

[test]
def test_type_of_call(t : T?) {
    t |> run("function call returns result type") <| @(t : T?) {
        var text : string
        var is_error = false
        // distance() call at line 33 col 13
        parse_result(do_type_of(fixture_path("_fixture_goto.das"), "33", "13"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "ExprCall(distance)") >= 0, "should show ExprCall")
        t |> success(find(text, "float") >= 0, "should return float type")
    }
}

[test]
def test_type_of_variable(t : T?) {
    t |> run("variable reference shows struct type with location") <| @(t : T?) {
        var text : string
        var is_error = false
        // p1 at line 33 col 22
        parse_result(do_type_of(fixture_path("_fixture_goto.das"), "33", "22"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "ExprVar(p1)") >= 0, "should show ExprVar")
        t |> success(find(text, "Point") >= 0, "should show Point type")
    }
}

[test]
def test_type_of_field(t : T?) {
    t |> run("field access shows field type") <| @(t : T?) {
        var text : string
        var is_error = false
        // a.x at line 15 col 14
        parse_result(do_type_of(fixture_path("_fixture_goto.das"), "15", "14"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "ExprField(x)") >= 0, "should show ExprField")
        t |> success(find(text, "float") >= 0, "should return float type")
    }
}

[test]
def test_type_of_no_expression(t : T?) {
    t |> run("reports no expression at empty position") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_type_of(fixture_path("_fixture_goto.das"), "1", "1"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "No expression found") >= 0, "should report no expression")
    }
}

// ── find_references ──────────────────────────────────────────────────

[test]
def test_find_references_function(t : T?) {
    t |> run("finds function call and addr references") <| @(t : T?) {
        var text : string
        var is_error = false
        // cursor on add_points call at line 44 col 14
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "44", "14", "file"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "func") >= 0, "should identify as function")
        t |> success(find(text, "add_points") >= 0, "should name add_points")
        t |> success(find(text, "[call]") >= 0, "should find call reference")
        t |> success(find(text, "[addr]") >= 0, "should find addr reference (@@add_points)")
    }
}

[test]
def test_find_references_variable(t : T?) {
    t |> run("finds variable references") <| @(t : T?) {
        var text : string
        var is_error = false
        // cursor on p1 variable usage at line 44 col 25
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "44", "25", "file"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "variable") >= 0, "should identify as variable")
        t |> success(find(text, "p1") >= 0, "should name p1")
        t |> success(find(text, "Found 2 reference") >= 0, "should find 2 references")
    }
}

[test]
def test_find_references_field(t : T?) {
    t |> run("finds field references including safe field") <| @(t : T?) {
        var text : string
        var is_error = false
        // cursor on a.x field at line 15 col 22
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "15", "22", "file"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "Point.x") >= 0, "should identify as Point.x field")
        t |> success(find(text, "[field]") >= 0, "should find field references")
        t |> success(find(text, "[safe field]") >= 0, "should find safe field reference (?.x)")
        t |> success(find(text, "Found 5 reference") >= 0, "should find 5 references")
    }
}

[test]
def test_find_references_addr(t : T?) {
    t |> run("finds function from @@addr expression") <| @(t : T?) {
        var text : string
        var is_error = false
        // cursor on @@add_points at line 46 col 30
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "46", "30", "file"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "func") >= 0, "should identify as function")
        t |> success(find(text, "add_points") >= 0, "should name add_points")
        t |> success(find(text, "[call]") >= 0, "should find call reference")
        t |> success(find(text, "[addr]") >= 0, "should find addr reference")
    }
}

[test]
def test_find_references_invoke(t : T?) {
    t |> run("invoke resolves to function pointer variable") <| @(t : T?) {
        var text : string
        var is_error = false
        // cursor on invoke(f, a, b) at line 23 col 12
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "23", "12", "file"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "variable") >= 0, "should identify as variable")
        t |> success(find(text, "'f'") >= 0, "should name f")
    }
}

[test]
def test_find_references_from_declaration(t : T?) {
    t |> run("finds references from function definition") <| @(t : T?) {
        var text : string
        var is_error = false
        // cursor on 'def add_points' at line 14
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "14", "5", "file"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "func") >= 0, "should identify as function")
        t |> success(find(text, "add_points") >= 0, "should name add_points")
        t |> success(find(text, "[call]") >= 0, "should find call reference")
    }
    t |> run("finds references from struct definition") <| @(t : T?) {
        var text : string
        var is_error = false
        // cursor on 'struct Point' at line 9
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "9", "1", "file"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "structure") >= 0, "should identify as structure")
        t |> success(find(text, "Point") >= 0, "should name Point")
        t |> success(find(text, "[type ref]") >= 0, "should find type references")
    }
}

[test]
def test_find_references_alias(t : T?) {
    t |> run("finds alias references from bitfield declaration") <| @(t : T?) {
        var text : string
        var is_error = false
        // cursor on 'bitfield Flags' declaration at line 26
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "26", "1", "file"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "alias") >= 0, "should identify as alias")
        t |> success(find(text, "Flags") >= 0, "should name Flags")
        t |> success(find(text, "[alias ref]") >= 0, "should find alias references")
    }
    t |> run("finds alias references from variant usage") <| @(t : T?) {
        var text : string
        var is_error = false
        // cursor on Value(i = 10) at line 49 col 13
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "49", "13", "file"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "alias") >= 0, "should identify as alias")
        t |> success(find(text, "Value") >= 0, "should name Value")
    }
}

[test]
def test_find_references_no_expression(t : T?) {
    t |> run("reports no expression at empty position") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_references(fixture_path("_fixture_refs.das"), "2", "1", "file"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "No expression or declaration found") >= 0, "should report nothing found")
    }
}
