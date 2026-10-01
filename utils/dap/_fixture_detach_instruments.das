options gen2
options debugger
options optimize = false
require daslib/fio
require daslib/debugger
require daslib/rtti

var instrument_hits : int
class InstrumentCounter : DapiDebugAgent {
    def override onInstrument(var _ctx : Context; _at : LineInfo) { instrument_hits++ }
}
def install_counter(_ctx : Context) {
    install_new_debug_agent(new InstrumentCounter(), "detach_counter")
}
[export, pinvoke]
def read_counter(var count : int?) { *count = instrument_hits }

[export]
def main {
    fork_debug_agent_context(@@install_counter)
    var guard = 0
    while (guard < 8) {
        guard++
        sleep(1u)
    }
    var count = 0
    unsafe { invoke_in_context(get_debug_agent_context("detach_counter"), "read_counter", addr(count)) }
    print("INSTRUMENT_HITS={count}\n")
}
