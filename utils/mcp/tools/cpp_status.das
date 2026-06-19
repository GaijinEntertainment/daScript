options gen2
options rtti

module cpp_status

require common public
require grep_usage
require cpp_compile_check
require cpp_format_file
require daslib/fio
require daslib/json
require daslib/json_boost
require strings

// Self-diagnosis for a freshly-installed bundle: report which external
// dependencies the cpp/agnostic tools need (ast-grep, a compile_commands.json,
// clang-format, and the MSVC vcvars INCLUDE env) and whether each is present,
// so it's obvious why a tool is missing or failing. Pure detection — no
// compilation, no side effects.
def do_cpp_status(as_json : bool) : string {
    let root = get_das_root()
    let sg = detect_ast_grep()
    let db = cpp_locate_compile_db("")
    let cf = detect_clang_format()
    let has_include = has_env_variable("INCLUDE")   // MSVC: cpp_compile_check needs vcvars' INCLUDE

    if (as_json) {
        return make_tool_result(sprint_json(JV((
            das_root = root,
            ast_grep = sg,
            ast_grep_available = !empty(sg),
            compile_db = db,
            compile_db_found = !empty(db),
            clang_format = cf,
            clang_format_available = !empty(cf),
            msvc_include_env = has_include
        )), false))
    }

    let report = build_string() $(var w) {
        w |> write("cpp-mcp server status (cpp/agnostic tool surface)\n\n")
        w |> write("  das root:      {root}\n")
        if (!empty(sg)) {
            w |> write("  ast-grep:      {sg}\n")
            w |> write("                 -> grep_usage / outline / cpp_grep_usage / cpp_find_symbol / cpp_outline / cpp_goto_definition ENABLED\n")
        } else {
            w |> write("  ast-grep:      NOT FOUND\n")
            w |> write("                 -> those 6 tools are disabled; install ast-grep (sg) on PATH and restart\n")
        }
        if (!empty(db)) {
            w |> write("  compile DB:    {db}\n")
            w |> write("                 -> cpp_compile_check / cpp_build_info ready\n")
        } else {
            w |> write("  compile DB:    NOT FOUND in build/ build-ninja/ build*/\n")
            w |> write("                 -> configure CMake with CMAKE_EXPORT_COMPILE_COMMANDS=ON; the VS generator does not emit it -- use a Ninja build dir\n")
        }
        if (!empty(cf)) {
            w |> write("  clang-format:  {cf}\n")
            w |> write("                 -> cpp_format_file ready (formats only where a .clang-format is present)\n")
        } else {
            w |> write("  clang-format:  NOT FOUND  -> cpp_format_file will no-op\n")
        }
        if (has_include) {
            w |> write("  MSVC INCLUDE:  set  -> cpp_compile_check can spawn cl and find system headers\n")
        } else {
            w |> write("  MSVC INCLUDE:  not set  -> on Windows+MSVC, launch via the vcvars wrapper or cpp_compile_check fails on <vcruntime.h>; ignored for clang/gcc\n")
        }
    }
    return make_tool_result(report)
}
