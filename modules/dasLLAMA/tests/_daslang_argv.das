options gen2
options _dasllama_internal = true

module _daslang_argv shared public

require dasllama/dasllama_env   // g_env_test.disable_modules: the modules a child leaves out
require strings
require daslib/strings_boost   // split: the comma list

// the arguments every daslang child of this folder takes right after the binary: the dynamic modules
// DASLLAMA_DISABLE_MODULES names left out, so a child compiles the world its parent compiles

//! the daslang arguments that leave out each dynamic module of the comma list `names` (`DASLLAMA_DISABLE_MODULES`):
//! `--disable-module <name>` a name, none for an empty list
def disable_module_args(names : string) : array<string> {
    var out : array<string>
    for (name in split(names, ",")) {
        let n = strip(name)
        continue if (empty(n))
        out |> push("--disable-module")
        out |> push(n)
    }
    return <- out
}

//! `disable_module_args` as a command line's text, each argument after a space: what follows the binary
def disable_module_flags(names : string) : string {
    var inscope args <- disable_module_args(names)
    return empty(args) ? "" : " {join(args, " ")}"
}

//! the dynamic modules this process's children leave out: `DASLLAMA_DISABLE_MODULES`, a comma list
def child_disabled_modules() : string => g_env_test.disable_modules

//! the flags this process's children take after the binary: the modules `DASLLAMA_DISABLE_MODULES` names left out
def child_disable_flags() : string => disable_module_flags(child_disabled_modules())

//! a daslang child's argv: the binary `exe`, the flags that leave out the modules `DASLLAMA_DISABLE_MODULES` names, then
//! `rest`
def daslang_argv(exe : string; rest : array<string>) : array<string>
    => <- daslang_argv_leaving(exe, child_disabled_modules(), rest)

//! `daslang_argv` leaving out the modules of the comma list `disabled`
def daslang_argv_leaving(exe, disabled : string; rest : array<string>) : array<string> {
    var argv <- [exe]
    argv |> push_from(disable_module_args(disabled), rest)
    return <- argv
}
