options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module profiler shared private

require debugapi
require fio

var g_out : FILE const?
var g_firstRecord = true

def dump(text)
    g_out |> fprint(text)

def dump_prefix
    g_out = fopen("perf.json", "wb")
    dump("
\{
  \"traceEvents\": [
")

def dump_suffix
    dump("
  ],
  \"displayTimeUnit\": \"ns\",
  \"systemTraceEvents\": \"SystemTraceData\",
  \"otherData\": \{
    \"version\": \"daScript (wip)\"
  \}
\}")
    g_out |> fclose()

def dump_event(ev:PerfEvent; tid:uint64)
    if g_firstRecord
        g_firstRecord = false
    else
        dump(",\n")
    let phase = ev.entering ? "B" : "E"
    dump("    \{\"name\": \"{ev.fun.mangledName}\", \"cat\": \"PERF\", \"ph\": \"{phase}\", \"pid\": 0, \"tid\": {int(tid)}, \"ts\": {ev.ts}\}")

def dump_context ( tid:uint64 )
    for ev in g_events[tid].events
        dump_event(ev, tid)
    delete g_events[tid].events
    g_events |> erase(tid)

var g_us0 = 0l

struct PerfEvent
    fun : SimFunction?
    entering : bool
    ts : int64

[skip_field_lock_check]
struct PerfContext
    events : array<PerfEvent>

var g_events : table<uint64;PerfContext>

var [[do_not_delete]] g_debug_agent : ProfilerDebugAgent?

class ProfilerDebugAgent : DapiDebugAgent
    def override onCreateContext ( var ctx:Context ) : void
        if ctx.category.debug_context || ctx.category.macro_context
            return
        instrument_all_functions(ctx)
    def override onDestroyContext ( var ctx:Context ) : void
        let tid = intptr(unsafe(addr(ctx)))
        dump_context(tid)
        delete g_events[tid]
    def override onInstrumentFunction ( var ctx:Context; fun:SimFunction?; entering:bool; userData:uint64 ) : void
        let tid = intptr(unsafe(addr(ctx)))
        if tid==intptr(unsafe(addr(this_context())))
            return
        g_events[tid].events |> push([[PerfEvent
            fun = fun,
            entering = entering,
            ts = ref_time_ticks() - g_us0]])

def debug_agent ( ctx : Context )
    assert(this_context().category.debug_context)
    dump_prefix()
    g_us0 = ref_time_ticks()
    assert(g_debug_agent==null)
    g_debug_agent = new ProfilerDebugAgent()
    install_new_debug_agent(g_debug_agent, "profiler")

[finalize]
def shutdown_profiler
    if g_out!=null
        dump_suffix()

[_macro]
def installing
    if is_compiling_macros_in_module("profiler")
        fork_debug_agent_context(@@debug_agent)



