options gen2
options no_aot

// Tutorial macro module: custom module options via add_module_option.
//
// A module can register its own `options` flag so that any file requiring
// it may write `options <name> = <value>` without the parser rejecting it
// as unknown. Macros in the module then read that flag to change behavior —
// this is the mechanism behind built-in module options like `shader_like`
// in daslib/validate_code.
//
// Two pieces:
//   1. register_options — a macro_function that calls add_module_option
//      while this module's macros compile, declaring the option name + type.
//   2. TraceCompileLint — a lint pass that reads the flag off the program
//      being compiled and, when set, prints a per-module compile-time note.

module add_module_option_mod

require daslib/ast
require daslib/ast_boost

// ---- Section 1: register the option ----------------------------------------
//
// add_module_option(module, name, type) records an option name and its type
// on a module. It must run while THIS module's macros are being compiled,
// hence the is_compiling_macros_in_module guard. `Type.tBool` declares the
// option as boolean; `Type.tInt` / `Type.tString` register other shapes.

[_macro, macro_function]
def register_options {
    if (is_compiling_macros_in_module("add_module_option_mod")) {
        this_module() |> add_module_option("trace_compile", Type.tBool)
    }
}

// ---- Section 2: read the option from a lint pass ---------------------------
//
// A [lint_macro] runs once per module compiled after this one. It reads the
// flag off the program options with find_arg; `?as tBool` extracts the bool
// and `?? false` supplies the default when the option was never set. When
// the flag is on, it prints the module name and its function count — at
// compile time, so the note appears before the program's own output.

[lint_macro]
class TraceCompileLint : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        let on = prog._options |> find_arg("trace_compile") ?as tBool ?? false
        if (!on) return false
        let cm = compiling_module()
        var nfun = 0
        cm |> for_each_function("") $(var _func : FunctionPtr) {
            nfun++
        }
        let name = empty(cm.name) ? "<main>" : string(cm.name)
        print("[trace_compile] module '{name}' — {nfun} function(s)\n")
        return false   // lint passes never modify the AST
    }
}
