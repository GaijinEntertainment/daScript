require debugapi

class MyDebugAgent : DapiDebugAgent
    def add_ii ( var res:int&; a,b:int )
        print("add {a} {b}\n")
        res = a + b

def debug_agent ( ctx : Context )
    install_new_debug_agent(new MyDebugAgent(), "sample")

[export]
def main
    fork_debug_agent_context(@@debug_agent)
    var t : int
    unsafe
        invoke_debug_agent_function("sample", "add_ii", addr(t), 1, 2)
    print("t = {t}\n")
