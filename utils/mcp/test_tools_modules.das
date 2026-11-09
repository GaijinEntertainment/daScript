options gen2
options rtti

require dastest/testing_boost public
require daslib/json
require daslib/json_boost
require strings
require _tools_common

require tools/list_functions public
require tools/list_types public
require tools/list_modules public
require tools/find_symbol public
require tools/list_module_api public
require tools/list_requires public
require tools/describe_type public

[test(description = "list_functions lists the user functions of a fixture.", timeout = 1200)]
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

[test(description = "list_types lists the structs, enums and classes of a fixture.", timeout = 1200)]
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

[test(description = "list_modules separates builtin from daslib modules and lists the registered require paths.", timeout = 1200)]
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
    t |> run("a module this program compiled from das is not a builtin, and the registered paths are a section") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_list_modules(true), text, is_error)
        var err : string
        var parsed = read_json(text, err)
        t |> success(parsed != null, "json parses: {err}")
        return if (parsed == null)
        t |> success(json_array_has(parsed?["builtin"], "math"), "math is a builtin")
        t |> success(!json_array_has(parsed?["builtin"], "json_boost"), "json_boost, which this program compiled, is not listed as a builtin")
        t |> success(json_array_has(parsed?["daslib"], "json_boost"), "json_boost is a daslib module")
        t |> success(parsed?["registered"] != null, "the registered require paths are listed")
        unsafe { delete parsed; }
    }
}

[test(description = "find_symbol finds builtins by substring, case-insensitively, exactly with an equals prefix, and finds typedef aliases.", timeout = 1200)]
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

[test(description = "list_module_api lists a module and rewrites an unqualified daslib name to its qualified one.", timeout = 1200)]
def test_list_module_api(t : T?) {
    t |> run("lists math module functions") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_list_module_api("math", "", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "sin") >= 0, "should list sin function")
        t |> success(find(text, "cos") >= 0, "should list cos function")
    }
    t |> run("rewrites unqualified daslib name") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_list_module_api("ast", "", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "[resolved 'ast' as 'daslib/ast']") >= 0, "should announce rewrite")
        t |> success(find(text, "AstVisitor") >= 0, "should list daslib/ast contents")
    }
    t |> run("no rewrite when name is already qualified") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_list_module_api("daslib/ast", "", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "[resolved") < 0, "should not announce rewrite")
    }
}

[test(description = "list_module_api in annotations mode lists function, structure, call-macro and reader-macro annotations and honors the filter.", timeout = 1200)]
def test_list_module_api_annotations(t : T?) {
    t |> run("lists function annotations from ast_boost") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_list_module_api("daslib/ast_boost", "", "annotations"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "[macro]") >= 0, "should list [macro] function annotation")
        t |> success(find(text, "Function annotations") >= 0, "should have Function annotations section")
    }
    t |> run("lists structure annotations from ast_boost") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_list_module_api("daslib/ast_boost", "", "annotations"), text, is_error)
        t |> success(find(text, "[function_macro]") >= 0, "should list [function_macro]")
        t |> success(find(text, "Structure annotations") >= 0, "should have Structure annotations section")
    }
    t |> run("lists call macros from ast") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_list_module_api("ast_core", "", "annotations"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "quote") >= 0, "should list quote call macro")
        t |> success(find(text, "Call macros") >= 0, "should have Call macros section")
    }
    t |> run("lists reader macros from regex_boost") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_list_module_api("daslib/regex_boost", "", "annotations"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "regex") >= 0, "should list regex reader macro")
        t |> success(find(text, "Reader macros") >= 0, "should have Reader macros section")
    }
    t |> run("filter works on annotations") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_list_module_api("daslib/ast_boost", "macro", "annotations"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "[macro]") >= 0, "should match macro")
        t |> success(find(text, "[contract]") < 0, "should not match contract")
    }
}

[test(description = "list_requires lists direct requires, marks public ones and annotates builtin modules.", timeout = 1200)]
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

[test(description = "describe_type describes handled types, enums and bitfields, reports an unknown type, and rewrites an unqualified daslib module.", timeout = 1200)]
def test_describe_type(t : T?) {
    t |> run("describes handled type fields") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_describe_type("TypeDecl", "ast_core"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "handled TypeDecl") >= 0, "should be handled type")
        t |> success(find(text, "baseType") >= 0, "should have baseType field")
        t |> success(find(text, "structType") >= 0, "should have structType field")
    }
    t |> run("describes enum values") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_describe_type("Type", "ast_core"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "enum Type") >= 0, "should be enum")
        t |> success(find(text, "tInt") >= 0, "should have tInt value")
    }
    t |> run("describes bitfield flags") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_describe_type("FunctionFlags", "ast_core"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "bitfield FunctionFlags") >= 0, "should be bitfield")
        t |> success(find(text, "builtIn") >= 0, "should have builtIn flag")
    }
    t |> run("type not found") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_describe_type("NonExistentType", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "not found") >= 0, "should report not found")
    }
    t |> run("rewrites unqualified daslib module") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_describe_type("AstFunctionAnnotation", "ast"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "[resolved 'ast' as 'daslib/ast']") >= 0, "should announce rewrite")
        t |> success(find(text, "class AstFunctionAnnotation") >= 0, "should describe class")
    }
}

[test(description = "list_modules json mode returns builtin and daslib arrays.", timeout = 1200)]
def test_list_modules_json(t : T?) {
    t |> run("json mode returns ModuleList") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_list_modules(true), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "\"builtin\":") >= 0, "should contain builtin field")
        t |> success(find(text, "\"daslib\":") >= 0, "should contain daslib field")
        t |> success(find(text, "\"math\"") >= 0, "should contain math module")
    }
}

[test(description = "list_requires json mode returns direct and transitive arrays.", timeout = 1200)]
def test_list_requires_json(t : T?) {
    t |> run("json mode returns RequiresResult") <| @(t : T?) {
        var text : string
        var is_error = false
        let ok = parse_result(do_list_requires(fixture_path("_fixture_valid.das"), "", "", true), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "\"direct\":") >= 0, "should contain direct field")
        t |> success(find(text, "\"transitive\":") >= 0, "should contain transitive field")
    }
}

[test(description = "find_symbol adds the C++ redirect line only when with_cpp_source is set.", timeout = 1200)]
def test_find_symbol_with_cpp_source(t : T?) {
    t |> run("redirect off (default) — no '→ cpp:' line") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_symbol("=print", "", "", "", "", false), text, is_error)
        t |> success(!is_error, "should not be error -- {cpp_diag(text)}")
        t |> success(find(text, "print") >= 0, "should still find print -- {cpp_diag(text)}")
        t |> success(find(text, "→ cpp:") < 0, "no redirect line when with_cpp_source=false -- {cpp_diag(text)}")
    }
    t |> run("redirect on — emits '→ cpp:' line for builtins") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_find_symbol("=print", "function", "", "", "", true, "src/builtin"), text, is_error)
        t |> success(!is_error, "should not be error -- {cpp_diag(text)}")
        t |> success(find(text, "→ cpp:") >= 0, "redirect line should appear when with_cpp_source=true -- {cpp_diag(text)}")
    }
}
