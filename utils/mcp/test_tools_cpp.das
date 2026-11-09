options gen2
options rtti

require dastest/testing_boost public
require strings
require daslib/fio
require _tools_common

require tools/common public
require tools/cpp_common public
require tools/cpp_grep_usage public
require tools/cpp_find_symbol public
require tools/cpp_outline public
require tools/cpp_goto_definition public
require tools/cpp_compile_check public
require tools/cpp_format_file public
require tools/cpp_status public

let private CPP_OUTLINE_FIXTURE = "_fixture_cpp_outline.h"

def cpp_outline_text(mode : string) : string {
    var text : string
    var is_error = false
    parse_result(do_cpp_outline(fixture_path(CPP_OUTLINE_FIXTURE), "", mode), text, is_error)
    return text
}

def private has_compile_db(text : string) : bool => find(text, "No compile_commands.json") < 0

[test(description = "cpp_grep_usage finds a known C++ symbol, copes with a missing directory and rejects an empty symbol.", timeout = 1200)]
def test_cpp_grep_usage(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("finds known C++ symbol in src/ast") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_grep_usage("addExternFunc", "src/ast", "", ""), text, is_error)
        t |> success(!is_error, "should not be error -- {cpp_diag(text)}")
        t |> success(find(text, "ast_interop.cpp") >= 0, "should mention ast_interop.cpp -- {cpp_diag(text)}")
    }
    t |> run("missing dir reports cleanly") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_grep_usage("anything", "/no/such/dir/here", "", ""), text, is_error)
        t |> success(is_error || find(text, "No matches") >= 0, "should error or report no matches")
    }
    t |> run("missing symbol arg returns error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_grep_usage("", "src", "", ""), text, is_error)
        t |> success(is_error, "should be error")
    }
}

[test(description = "cpp_grep_usage finds a type used as a return type, parameter type, base class and qualified scope.", timeout = 1200)]
def test_cpp_grep_usage_type_position(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("finds Outer used as a return type") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_grep_usage("Outer", "utils/mcp/tests", "", ""), text, is_error)
        t |> success(!is_error, "should not be error -- {cpp_diag(text)}")
        t |> success(find(text, "Outer makeOuter()") >= 0,
            "must catch type_identifier in return position -- {cpp_diag(text)}")
    }
    t |> run("finds Outer used as a parameter type") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_grep_usage("Outer", "utils/mcp/tests", "", ""), text, is_error)
        t |> success(find(text, "void takeOuter(const Outer & o)") >= 0,
            "must catch type_identifier in parameter position -- {cpp_diag(text)}")
    }
    t |> run("finds Outer used as a base class") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_grep_usage("Outer", "utils/mcp/tests", "", ""), text, is_error)
        t |> success(find(text, "class GrepDerived : public Outer") >= 0,
            "must catch namespace_identifier in base_class_clause -- {cpp_diag(text)}")
    }
    t |> run("finds Outer used as qualified scope") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_grep_usage("Outer", "utils/mcp/tests", "", ""), text, is_error)
        t |> success(find(text, "Outer::Inner inner") >= 0,
            "must catch namespace_identifier in qualified type expression -- {cpp_diag(text)}")
    }
}

[test(description = "cpp_grep_usage lists a source line once even when several syntax nodes match it.", timeout = 1200)]
def test_cpp_grep_usage_dedup(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("a single source line appears at most once even if multiple AST nodes match") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_grep_usage("TextWriter", "src/misc/string_writer.cpp", "", ""), text, is_error)
        t |> success(!is_error, "should not be error -- {cpp_diag(text)}")
        var count = 0
        var pos = 0
        let needle = "TextWriter::~TextWriter()"
        while (true) {
            let i = find(text, needle, pos)
            if (i < 0) break
            count++
            pos = i + length(needle)
        }
        t |> equal(count, 1, "destructor line should appear exactly once after dedup")
    }
}

[test(description = "cpp_find_symbol finds classes in sources and headers, reports no match cleanly, honors the kind filter and rejects an empty query.", timeout = 1200)]
def test_cpp_find_symbol(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("finds class in src/ast") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_find_symbol("=Module", "class", "src/ast", ""), text, is_error)
        t |> success(!is_error, "should not be error -- {cpp_diag(text)}")
        t |> success(find(text, "Module") >= 0, "should find Module -- {cpp_diag(text)}")
    }
    t |> run(".h languageGlobs regression — finds class in header") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_find_symbol("=FsFileSystem", "class", "include", ""), text, is_error)
        t |> success(!is_error, "should not be error -- {cpp_diag(text)}")
        t |> success(find(text, "fs_file_info.h") >= 0, "should find FsFileSystem in fs_file_info.h (proves .h is treated as cpp) -- {cpp_diag(text)}")
    }
    t |> run("no-match returns clean empty result") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_find_symbol("zzznotarealsymbolzzz_xyzzy", "", "src/ast", ""), text, is_error)
        t |> success(!is_error, "should not be error on no-match -- {cpp_diag(text)}")
        t |> success(find(text, "No matches") >= 0, "should report no matches -- {cpp_diag(text)}")
    }
    t |> run("kind filter rejects wrong kinds") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_find_symbol("=addFunction", "class", "src/ast", ""), text, is_error)
        t |> success(!is_error, "should not be error -- {cpp_diag(text)}")
        t |> success(find(text, "No matches") >= 0, "kind=class should reject methods named addFunction -- {cpp_diag(text)}")
    }
    t |> run("missing query returns error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_find_symbol("", "", "src/ast", ""), text, is_error)
        t |> success(is_error, "should be error")
    }
}

[test(description = "cpp_outline lists declarations of a known file and rejects missing arguments and an unknown mode.", timeout = 1200)]
def test_cpp_outline(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("lists declarations in a known file") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_outline("src/ast/ast_interop.cpp", "", ""), text, is_error)
        t |> success(!is_error, "should not be error -- {cpp_diag(text)}")
        t |> success(find(text, "[function]") >= 0 || find(text, "[class]") >= 0 || find(text, "[struct]") >= 0, "should list at least one declaration -- {cpp_diag(text)}")
    }
    t |> run("missing both args returns error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_outline("", "", ""), text, is_error)
        t |> success(is_error, "should be error")
    }
    t |> run("invalid mode rejected") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_outline(fixture_path("_fixture_cpp_outline.h"), "", "verbose"), text, is_error)
        t |> success(is_error, "unknown mode should error")
    }
}

[test(description = "cpp_outline collapses a multi-line declaration into one signature and shows return type and parameters.", timeout = 1200)]
def test_cpp_outline_signatures(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("multi-line function declaration collapsed into a single signature") <| @(t : T?) {
        let text = cpp_outline_text("")
        t |> success(find(text, "void describe( const char * name, int count = 0, bool verbose = false ) const") >= 0,
            "expected collapsed signature")
    }
    t |> run("free function shows return type and params") <| @(t : T?) {
        let text = cpp_outline_text("")
        t |> success(find(text, "[function] void freeFunction(int x)") >= 0,
            "expected freeFunction signature")
    }
}

[test(description = "cpp_outline reports a class with a macro prefix as the class and never as a function.", timeout = 1200)]
def test_cpp_outline_class_misparse_filtered(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("class with macro prefix is reported as the class, not as a function") <| @(t : T?) {
        let text = cpp_outline_text("")
        t |> success(find(text, "[class] Outer") >= 0, "expected [class] Outer")
        t |> success(find(text, "[function] class MY_EXPORT") < 0,
            "must NOT emit a function entry for the misparsed class line")
        t |> success(find(text, "[class] MY_EXPORT") < 0,
            "name extractor must not pick the leading macro as the class name")
    }
}

[test(description = "cpp_outline drops anonymous unions, structs and enums and their enumerators.", timeout = 1200)]
def test_cpp_outline_anonymous_filtered(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("anonymous union/struct/enum entries are dropped") <| @(t : T?) {
        let text = cpp_outline_text("")
        t |> success(find(text, "[union] union \{") < 0, "anonymous union must be dropped")
        t |> success(find(text, "[union]\n") < 0, "no bare [union] entries")
        t |> success(find(text, "[enum] enum \{") < 0, "anonymous enum must be dropped")
        t |> success(find(text, "[enum] kFlagA") < 0, "anonymous enum's enumerators must not leak as the name")
    }
}

[test(description = "cpp_outline shows template specializations with their distinct arguments.", timeout = 1200)]
def test_cpp_outline_template_specializations(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("template specializations show distinct args") <| @(t : T?) {
        let text = cpp_outline_text("")
        t |> success(find(text, "[struct] Trait\n") >= 0, "primary Trait should appear")
        t |> success(find(text, "Trait<int>") >= 0, "Trait<int> should appear with args")
        t |> success(find(text, "Trait<float>") >= 0, "Trait<float> should appear with args")
    }
}

[test(description = "cpp_outline nests members under their class in tree mode and picks tree mode automatically for nested files.", timeout = 1200)]
def test_cpp_outline_nesting(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("methods nest under their enclosing class in tree mode") <| @(t : T?) {
        let text = cpp_outline_text("tree")
        t |> success(find(text, "    [class] Inner") >= 0, "Inner should be indented under Outer")
        t |> success(find(text, "      [function] void method()") >= 0,
            "Inner's method should be indented under Inner")
    }
    t |> run("auto mode picks tree when nesting exists") <| @(t : T?) {
        let auto_text = cpp_outline_text("")
        let tree_text = cpp_outline_text("tree")
        t |> success(auto_text == tree_text, "auto mode should match tree mode for a nested file")
    }
}

[test(description = "cpp_outline qualifies flat-mode names with their ancestor chain exactly once.", timeout = 1200)]
def test_cpp_outline_qualified_names(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("flat mode prefixes in-class methods with their ancestor chain") <| @(t : T?) {
        let text = cpp_outline_text("flat")
        t |> success(find(text, "fixns::Outer::describe") >= 0,
            "Outer::describe should be qualified with namespace + class")
        t |> success(find(text, "fixns::Outer::Inner::method") >= 0,
            "deeply-nested method should carry full qualifier chain")
    }
    t |> run("flat mode does not double-prefix already-qualified names") <| @(t : T?) {
        let text = cpp_outline_text("flat")
        t |> success(find(text, "fixns::Outer::describe::") < 0,
            "must not double-qualify out-of-class definition")
    }
}

[test(description = "cpp_goto_definition rejects missing arguments, non-positive positions and nonexistent files.")]
def test_cpp_goto_definition_errors(t : T?) {
    t |> run("missing args returns error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_goto_definition("", "", ""), text, is_error)
        t |> success(is_error, "should be error")
    }
    t |> run("non-positive line/column returns error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_goto_definition("src/ast/ast_interop.cpp", "0", "0"), text, is_error)
        t |> success(is_error, "should be error on 0/0 line/col")
    }
    t |> run("nonexistent file returns error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_goto_definition("/no/such/file.cpp", "1", "1"), text, is_error)
        t |> success(is_error, "should be error")
    }
}

[test(description = "The C++ search configuration lists the modules directory and at least three include globs.")]
def test_cpp_search_config(t : T?) {
    t |> run("CPP_SEARCH_DIRS contains expected defaults") <| @(t : T?) {
        var saw_modules = false
        for (d in CPP_SEARCH_DIRS) {
            if (d == "modules") {
                saw_modules = true
            }
        }
        t |> success(saw_modules, "CPP_SEARCH_DIRS should include 'modules'")
    }
    t |> run("CPP_SEARCH_INCLUDE_GLOBS locks to *.cpp/*.h/*.hpp") <| @(t : T?) {
        let n = length(CPP_SEARCH_INCLUDE_GLOBS)
        t |> success(n >= 3, "should list at least 3 include globs")
    }
}

[test(description = "The C++ index signature is non-empty and stable across consecutive builds with no file changes.", timeout = 1200)]
def test_cpp_index_signature(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("ensure_cpp_index produces a stable signature across consecutive calls (no edits)") <| @(t : T?) {
        ensure_cpp_index()
        let sig1 = cpp_index_signature
        ensure_cpp_index()
        let sig2 = cpp_index_signature
        t |> success(!empty(sig1), "first signature should not be empty after ensure_cpp_index()")
        t |> success(sig1 == sig2, "signature should be stable across consecutive ensure_cpp_index() calls (no fs changes)")
    }
}

[test(description = "The daspkg cache checkout is excluded from the C++ index automatically when present.")]
def test_cpp_collect_git_excludes(t : T?) {
    t |> run("daspkg cache .git is auto-excluded if present") <| @(t : T?) {
        let modules_dir = path_join(get_das_root(), "modules")
        let cache_git = path_join(modules_dir, ".daspkg_cache/index/.git")
        if (!stat(cache_git).is_valid) {
            return
        }
        var search_dirs : array<string>
        search_dirs |> push(modules_dir)
        let no_overrides : array<string>
        let excludes <- cpp_collect_git_excludes(search_dirs, no_overrides)
        var found = false
        for (g in excludes) {
            if (find(g, ".daspkg_cache/index") >= 0) {
                found = true
            }
        }
        t |> success(found, "modules/.daspkg_cache/index should be in the auto-exclude list")
    }
}

[test(description = "cpp_find_symbol finds a function declared in a header.", timeout = 1200)]
def test_cpp_function_declarations_visible(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("finds function declaration in ast.h header") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_find_symbol("=pull_all_auto_registered_modules", "function", "include", ""), text, is_error)
        t |> success(!is_error, "should not be error -- {cpp_diag(text)}")
        t |> success(find(text, "ast.h") >= 0, "should locate declaration in include/daScript/ast/ast.h -- {cpp_diag(text)}")
    }
}

[test(description = "cpp_find_symbol finds a using alias as a typedef.", timeout = 1200)]
def test_cpp_using_alias_visible(t : T?) {
    if (!require_ast_grep(t)) return
    t |> run("finds 'using' alias as typedef") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_find_symbol("module_pull_t", "typedef", "include", ""), text, is_error)
        t |> success(!is_error, "should not be error -- {cpp_diag(text)}")
        t |> success(find(text, "module_pull_t") >= 0, "should locate the using-alias -- {cpp_diag(text)}")
    }
}

[test(description = "cpp_compile_check rejects a missing file and a header, syntax-checks a translation unit, and returns the json shape, all tolerating a missing compile database.", timeout = 1200)]
def test_cpp_compile_check(t : T?) {
    t |> run("missing file is an error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_compile_check("src/zzz_nonexistent_xyz.cpp", "", false), text, is_error)
        t |> success(is_error, "missing file -> error -- text={text}")
        t |> success(find(text, "file not found") >= 0, "reports file not found -- text={text}")
    }
    t |> run("real TU syntax-checks (or graceful no-DB)") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_compile_check("src/misc/sysos.cpp", "", false), text, is_error)
        if (!has_compile_db(text)) {
            t->log("no compile_commands.json — skipping cpp_compile_check assertions")
            return
        }
        t |> success(find(text, "Compile check") >= 0 || find(text, "no compile DB entry") >= 0,
                     "ran a check or reported no entry -- text={text}")
    }
    t |> run("header is rejected with guidance") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_compile_check("src/misc/sysos.cpp", "", false), text, is_error)
        if (!has_compile_db(text)) return
        parse_result(do_cpp_compile_check("include/daScript/daScript.h", "", false), text, is_error)
        t |> success(find(text, "header") >= 0, "header gets a clear 'pass a .cpp' message -- text={text}")
    }
    t |> run("json mode returns a CppCompileResult shape") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_compile_check("src/misc/sysos.cpp", "", true), text, is_error)
        if (!has_compile_db(text) || find(text, "no compile DB entry") >= 0) return
        t |> success(find(text, "\"success\"") >= 0, "json has success field -- text={text}")
        t |> success(find(text, "\"file\"") >= 0, "json has file field -- text={text}")
    }
}

[test(description = "cpp_build_info shows the compiler and syntax-only command of a known translation unit and reports a file outside the database, tolerating a missing compile database.", timeout = 1200)]
def test_cpp_build_info(t : T?) {
    t |> run("known TU shows compiler + syntax-only command (or graceful no-DB)") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_build_info("src/misc/sysos.cpp", ""), text, is_error)
        if (!has_compile_db(text)) {
            t->log("no compile_commands.json — skipping cpp_build_info assertions")
            return
        }
        if (find(text, "no compile DB entry") >= 0) return
        t |> success(find(text, "compiler:") >= 0, "shows compiler -- text={text}")
        t |> success(find(text, "syntax-only command") >= 0, "shows derived syntax-only command -- text={text}")
        t |> success(find(text, "/Zs") >= 0 || find(text, "-fsyntax-only") >= 0, "appends a syntax-only flag -- text={text}")
    }
    t |> run("file not in the DB reports no entry") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_build_info("src/misc/sysos.cpp", ""), text, is_error)
        if (!has_compile_db(text)) return
        parse_result(do_cpp_build_info("utils/mcp/protocol_core.das", ""), text, is_error)
        t |> success(is_error, "non-cpp file -> error -- text={text}")
        t |> success(find(text, "no compile DB entry") >= 0, "reports no entry -- text={text}")
    }
}

[test(description = "cpp_format_file reports no_config for a tree without a clang-format file and an error for a missing file.", timeout = 1200)]
def test_cpp_format_file(t : T?) {
    t |> run("daScript tree has no .clang-format -> no_config") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_format_file("src/misc/sysos.cpp"), text, is_error)
        t |> success(!is_error, "no_config is not an error -- text={text}")
        t |> success(find(text, "no_config") >= 0, "reports no_config -- text={text}")
    }
    t |> run("missing file is an error") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_format_file("src/zzz_nonexistent_xyz.cpp"), text, is_error)
        t |> success(is_error, "missing file -> error -- text={text}")
        t |> success(find(text, "file not found") >= 0, "reports file not found -- text={text}")
    }
}

[test(description = "cpp_status reports ast-grep, the compile database and clang-format in text and json and honors an explicit build directory.", timeout = 1200)]
def test_cpp_status(t : T?) {
    t |> run("text report names each diagnosed dependency") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_status("", false), text, is_error)
        t |> success(!is_error, "status is not an error -- text={text}")
        t |> success(find(text, "cpp-mcp server status") >= 0, "has report header -- text={text}")
        t |> success(find(text, "ast-grep") >= 0, "reports ast-grep -- text={text}")
        t |> success(find(text, "compile DB") >= 0, "reports compile DB -- text={text}")
        t |> success(find(text, "clang-format") >= 0, "reports clang-format -- text={text}")
    }
    t |> run("json mode returns structured fields") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_status("", true), text, is_error)
        t |> success(!is_error, "json status is not an error -- text={text}")
        t |> success(find(text, "das_root") >= 0, "json has das_root -- text={text}")
        t |> success(find(text, "ast_grep_available") >= 0, "json has ast_grep_available -- text={text}")
    }
    t |> run("explicit build_dir is honored for the compile-DB probe") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_cpp_status("build-ninja", true), text, is_error)
        t |> success(!is_error, "build_dir status is not an error -- text={text}")
        t |> success(find(text, "compile_db") >= 0, "json reports compile_db field -- text={text}")
    }
}

[test(description = "Relative paths and C++ index roots resolve against the served tree, with the compile database looked up under it, and fall back to the daslang roots when it has no project marker.")]
def test_served_tree_paths(t : T?) {
    let here = getcwd()
    let dir = path_join(get_das_root(), "utils/mcp/tests/_served_tree_{ref_time_ticks()}")
    mkdir(dir)
    t |> success(chdir(dir), "enter the served tree")
    let served = to_generic_path(getcwd())
    t |> run("a relative path joins the served tree, not the daslang root") @(t : T?) {
        t |> equal(resolve_path("tests/x.das"), path_join(served, "tests/x.das"))
        t |> equal(resolve_path(""), served)
        t |> equal(resolve_path("."), served)
        let abs = path_join(to_generic_path(get_das_root()), "abs/x.das")   //! a rooted path on every platform (a bare `/abs` is drive-relative on Windows and resolves to `D:/abs`)
        t |> equal(resolve_path(abs), abs)
    }
    t |> run("a served tree with no project marker keeps the daslang C++ index roots") @(t : T?) {
        var inscope dirs <- cpp_default_search_dirs()
        t |> equal(length(dirs), length(CPP_SEARCH_DIRS))
        t |> equal(dirs[0], path_join(get_das_root(), CPP_SEARCH_DIRS[0]))
    }
    t |> run("the C++ index roots are the served tree once it carries a project marker") @(t : T?) {
        fwrite(path_join(served, "CMakeLists.txt"), "")
        var inscope dirs <- cpp_default_search_dirs()
        t |> equal(length(dirs), 1)
        t |> equal(dirs[0], served)
    }
    t |> run("the compile database is looked up under the served tree") @(t : T?) {
        mkdir(path_join(served, "build"))
        let db = path_join(served, "build/compile_commands.json")
        fwrite(db, "[]")
        t |> equal(cpp_locate_compile_db(""), db)
    }
    t |> success(chdir(here), "leave the served tree")
    rmdir_rec(dir)
    t |> run("at the daslang root the C++ index roots are CPP_SEARCH_DIRS") @(t : T?) {
        if (to_generic_path(getcwd()) != get_das_root()) {
            t |> skip("the suite runs outside the daslang root")
            return
        }
        var inscope dirs <- cpp_default_search_dirs()
        t |> equal(length(dirs), length(CPP_SEARCH_DIRS))
        t |> equal(dirs[0], path_join(get_das_root(), CPP_SEARCH_DIRS[0]))
    }
}
