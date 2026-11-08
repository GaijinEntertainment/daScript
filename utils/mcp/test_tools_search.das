options gen2
options rtti

require dastest/testing_boost public
require strings
require _tools_common

require tools/grep_usage public
require tools/outline public

[test(description = "grep_usage finds a symbol with real line numbers, treats an unmatched symbol as clean, rejects an empty one, and keeps slash-anchored globs from zeroing the result.", timeout = 1200)]
def test_grep_usage(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("finds symbol in directory") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_grep_usage("compile_program", "utils/mcp/tools", "", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "compile_program") >= 0, "should contain symbol name")
        t |> success(find(text, "common.das") >= 0, "should find in common.das")
        t |> success(find(text, "Found") >= 0, "should have Found header")
    }
    t |> run("returns correct line numbers") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_grep_usage("compile_program", "utils/mcp/tools", "", ""), text, is_error)
        t |> success(find(text, "  1:") < 0 || find(text, "  38:") >= 0 || find(text, "  41:") >= 0, "should have real line numbers")
    }
    t |> run("no matches returns clean message") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_grep_usage("xyzzy_nonexistent_symbol_12345", "utils/mcp/tools", "", ""), text, is_error)
        t |> success(!is_error, "should not be error")
    }
    t |> run("missing symbol is error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_grep_usage("", "", "", ""), text, is_error)
        t |> success(is_error, "should be error")
    }
    t |> run("slash-anchored glob filters rather than zeroing") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_grep_usage("compile_program", "", "", "utils/mcp/tools/*.das"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "common.das") >= 0, "root-anchored glob should match")
        t |> success(find(text, "Found 0 match") < 0, "glob must not silently zero the result")
    }
    t |> run("explicit '.' directory behaves like the default root") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_grep_usage("compile_program", ".", "", "utils/mcp/tools/*.das"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "common.das") >= 0, "'.' root must not append a trailing /.")
        t |> success(find(text, "Found 0 match") < 0, "glob must not silently zero the result")
    }
}

[test(description = "grep_usage finds the defining use of a symbol in the file that declares it.", timeout = 1200)]
def test_grep_usage_parse_aware(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("skips comments and strings") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_grep_usage("compile_program", "utils/mcp/tools", "", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "common.das") >= 0, "should find in common.das where it's defined")
    }
}

[test(description = "grep_usage honors an exclude glob and limits itself to an include glob.", timeout = 1200)]
def test_grep_usage_glob_exclude(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("exclude glob filters out files") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_grep_usage("compile_program", "utils/mcp/tools", "", "!**/common.das"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "common.das") < 0, "common.das should be excluded")
    }
    t |> run("include glob limits to matching files") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_grep_usage("compile_program", "utils/mcp/tools", "", "common.das"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "common.das") >= 0, "should find in common.das")
        t |> success(find(text, "compile_check.das") < 0, "should not find in other files")
    }
}

[test(description = "outline lists functions, structs, enums, bitfields, variants, typedefs, globals and classes in line order and rejects missing arguments.", timeout = 1200)]
def test_outline(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("lists functions and structs") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_outline("utils/mcp/tests/_fixture_valid.das", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "[function]") >= 0, "should find functions")
        t |> success(find(text, "[struct]") >= 0, "should find structs")
        t |> success(find(text, "add") >= 0, "should find 'add' function")
        t |> success(find(text, "Point") >= 0, "should find 'Point' struct")
    }
    t |> run("lists enums") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_outline("utils/mcp/tests/_fixture_valid.das", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "[enum]") >= 0, "should find enums")
        t |> success(find(text, "Color") >= 0, "should find 'Color' enum")
    }
    t |> run("lists bitfields and variants") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_outline("utils/mcp/tests/_fixture_valid.das", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "[bitfield]") >= 0, "should find bitfields")
        t |> success(find(text, "Access") >= 0, "should find 'Access' bitfield")
        t |> success(find(text, "[variant]") >= 0, "should find variants")
        t |> success(find(text, "Value") >= 0, "should find 'Value' variant")
    }
    t |> run("lists typedefs, globals and classes") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_outline("utils/mcp/tests/_fixture_outline.das", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "[typedef] typedef Coord") >= 0, "should find the 'Coord' typedef")
        t |> success(find(text, "Local") < 0, "should not list a typedef local to a function")
        t |> success(find(text, "[global] let ORIGIN") >= 0, "should find the 'ORIGIN' global")
        t |> success(find(text, "[struct] class Shape") >= 0, "should find the 'Shape' class")
        t |> success(find(text, "get_area") < 0, "should not list a method of a class")
        t |> success(find(text, "Inner") < 0, "should not list a typedef of a class")
    }
    t |> run("sorted by line number") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_outline("utils/mcp/tests/_fixture_valid.das", ""), text, is_error)
        t |> success(!is_error, "should not be error")
        let point_pos = find(text, "Point")
        let color_pos = find(text, "Color")
        let add_pos = find(text, "add")
        t |> success(point_pos < color_pos, "Point should come before Color")
        t |> success(color_pos < add_pos, "Color should come before add")
    }
    t |> run("missing args is error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_outline("", ""), text, is_error)
        t |> success(is_error, "should be error")
    }
}

[test(description = "outline scans every file a glob matches.", timeout = 1200)]
def test_outline_glob(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("glob scans multiple files") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_outline("", "utils/mcp/tests/_fixture_*.das"), text, is_error)
        t |> success(!is_error, "should not be error")
        t |> success(find(text, "_fixture_valid.das") >= 0, "should include _fixture_valid.das")
    }
}

[test(description = "ast-grep is detected in the test environment.", timeout = 1200)]
def test_grep_usage_detect(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("detects ast-grep") <| @(t : T?) {
        let sg = detect_ast_grep()
        t |> success(!empty(sg), "ast-grep should be detected in test environment")
    }
}
