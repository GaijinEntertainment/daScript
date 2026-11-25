options gen2

module test_compile_dag_fork public

require debugapi

[unused_argument(ctx)]
def private fork_probe_agent(ctx : Context) {
    pass
}

[_macro]
def private fork_probe_installing {
    if (is_compiling_macros_in_module("never_compiled_module")) {
        fork_debug_agent_context(@@fork_probe_agent)
    }
}

def public fork_probe_value() => 1
