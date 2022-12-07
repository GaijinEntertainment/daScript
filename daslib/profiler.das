options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module profiler shared private

require debugapi
require fio
require strings

var g_out : FILE const?
var g_firstRecord = true

def dump(text)
    g_out |> fprint(text)

def dump_prefix
    g_out = fopen("perf.json", "wb")
    dump("[")

def dump_suffix
    dump("]")
    g_out |> fclose()

def dump_event(ev:PerfEvent; tid:uint64)
    if g_firstRecord
        g_firstRecord = false
    else
        dump(",\n")
    let phase = ev.entering ? "B" : "E"
    let fnName = ev.fun.mangledName |> escape()
    dump("\{\"name\": \"{fnName}\", \"cat\": \"PERF\", \"ph\": \"{phase}\", \"pid\": 0, \"tid\": {int64(tid)}, \"ts\": {ev.ts/1000l}\}")

[skip_field_lock_check]
struct PerfNode
    fun : SimFunction?
    count : uint64
    total_time : int64
    enter_time : int64
    children : table<SimFunction?;PerfNode?>

def dump_node ( node:PerfNode?; tab:int = -1 )
    if node==null
        return
    if node.fun!=null
        let tabs = repeat("  ",tab)
        to_log(LOG_INFO,"{tabs}{node.fun.mangledName} {int64(node.count)} {node.total_time}ns\n")
    for ch in values(node.children)
        dump_node(ch, tab+1)

def dump_context_stack ( var ctx:Context; tid:uint64 )
    // print("context: {tid} // {ctx.name} {ctx.category}\n")
    var root = new PerfNode()
    var stack : array<PerfNode?>
    var sp = root
    for ev in g_events[tid].events
        if ev.entering
            stack |> push(sp)
            if !sp.children |> key_exists(ev.fun)
                sp.children[ev.fun] = new [[PerfNode fun=ev.fun]]
            sp = sp.children[ev.fun]
            sp.count ++
            sp.enter_time = ev.ts
        else
            stack |> pop()
            sp = length(stack)!=0 ? stack |> back() : root
            sp.total_time += ev.ts - sp.enter_time
    dump_node(root)
    unsafe
        delete root

def dump_context ( var ctx:Context; tid:uint64 )
    dump_context_stack(ctx, tid)
    for ev in g_events[tid].events
        dump_event(ev, tid)
    delete g_events[tid].events
    g_events |> erase(tid)

var g_us0 = 0l

struct PerfEvent
    fun : SimFunction?
    ts : int64
    entering : bool

[skip_field_lock_check]
struct PerfContext
    events : array<PerfEvent>

var g_events : table<uint64;PerfContext>

class ProfilerDebugAgent : DapiDebugAgent
    def override onCreateContext ( var ctx:Context ) : void
        if ctx.category.debug_context || ctx.category.macro_context
            return
        instrument_all_functions(ctx)
    def override onDestroyContext ( var ctx:Context ) : void
        let tid = intptr(unsafe(addr(ctx)))
        dump_context(ctx, tid)
        delete g_events[tid]
    def override onInstrumentFunction ( var ctx:Context; fun:SimFunction?; entering:bool; userData:uint64 ) : void
        let tid = intptr(unsafe(addr(ctx)))
        g_events[tid].events |> push([[PerfEvent
            fun = fun,
            entering = entering,
            ts = get_delta_time_nsec(ref_time_ticks(),g_us0)]])

def debug_agent ( ctx : Context )
    assert(this_context().category.debug_context)
    dump_prefix()
    g_us0 = ref_time_ticks()
    install_new_debug_agent(new ProfilerDebugAgent(), "profiler")

[finalize]
def shutdown_profiler
    if g_out!=null
        dump_suffix()

[_macro]
def installing
    if is_compiling_macros_in_module("profiler")
        fork_debug_agent_context(@@debug_agent)

