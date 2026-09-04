options gen2
options no_aot

require daslib/debugger

class CallbackWithoutThreadlockAgent : DapiDebugAgent {
    def override onTick() {
    }
}

def setup_callback_agent(_ctx : Context) {
    install_new_debug_agent(new CallbackWithoutThreadlockAgent(), "callback_without_threadlock")
}

[export]
def main() {
    fork_debug_agent_context(@@setup_callback_agent)
    tick_debug_agent("callback_without_threadlock")
    delete_debug_agent_context("callback_without_threadlock")
    print("DAP_CALLBACK_NO_THREADLOCK=1\n")
}
