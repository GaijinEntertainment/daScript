options gen2
options indenting = 4
options strict_smart_pointers = true

module dasllama_lint shared private

//! dasLLAMA facade lint - default-on, required by every dasllama engine module.
//! DASLLAMA001 (macro_error, code 50503): a consumer requires a dasLLAMA internal
//! directly - by name, or by a path that resolves into the module tree. The public entry
//! points are ``dasllama/dasllama`` (the facade), ``dasllama/dasllama_scheduler`` (serving),
//! and ``dasllama/dasllama_exchange`` (the tune-sidecar exchange). Engine files, in-module
//! tests and harnesses opt out per file via ``options _dasllama_internal = true``.

require daslib/ast_boost
require strings

let private ALLOWED_DASLLAMA : table<string> <- {
    "dasllama", "dasllama_scheduler", "dasllama_exchange", "dasllama_exchange_schema", "dasllama_lint"
}

[lint_macro]
class DasllamaLintMacro : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        let opted_out = prog._options |> find_arg("_dasllama_internal") ?as tBool ?? false
        if (opted_out) return true
        let tm = prog.getThisModule
        if (tm == null) return true
        // requires parsed from other source-compiled modules in the same program are theirs to answer for
        var module_files : table<string>
        let primary = string(tm.fileName)
        if (!empty(primary)) module_files |> insert(primary)
        for_each_function(tm, "") $(fn) {
            if (fn.at.fileInfo != null && fn._module == tm) {
                module_files |> insert(string(fn.at.fileInfo.name))
            }
        }
        prog |> for_each_require_declaration() $ [unused_argument(req_name, file, pub)] (rmod, req_name, file, pub, at) {
            if (rmod == null || at.fileInfo == null || !key_exists(module_files, string(at.fileInfo.name))) return
            let mname = string(rmod.name)
            if (key_exists(ALLOWED_DASLLAMA, mname)) return
            // the tree is the boundary, not the name: a path-require resolving into
            // modules/dasLLAMA is the same breach through the side door (case-folded: Windows
            // and macOS resolve a lowercased path, and a user's own dasllama_* module outside
            // the tree is theirs)
            let inside_tree = find(to_lower(replace(string(rmod.fileName), "\\", "/")), "modules/dasllama/") >= 0
            if (!inside_tree) return
            prog |> macro_error(at,
                "DASLLAMA001: require of dasLLAMA internal '{mname}' - the public surface is require dasllama/dasllama (+ dasllama/dasllama_scheduler for serving, dasllama/dasllama_exchange for the sidecar exchange). A symbol the facade lacks means the facade is missing it: add it to dasllama.das. Engine/test escape: options _dasllama_internal = true.")
        }
        return true
    }
}
