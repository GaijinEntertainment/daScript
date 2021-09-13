require rtti
require debugapi
require jobque
require daslib/apply_in_context

// debug server

var value = 0

[export]
def debug_agent ( ctx : Context )
    print("from debugger\n")
    install_new_debug_agent(new DapiDebugAgent(), "sample") // note - no install = no category
    value = 13

[apply_in_context(sample)]
def get_value() : int
    return value

[apply_in_context(sample)]
def cache_resource_server ( ctx:Context; fun:function ) : bool
    print("cache: in the server on {this_context()}\n")
    unsafe
        ctx |> invoke_in_context(fun)
    return true

// client

[export]
def main
    fork_debug_agent_context(@@debug_agent)
    cache_resource_server(this_context()) <| @@ ()
        print("it called me back on {this_context()}\n")
    print("value = {get_value()}\n")
