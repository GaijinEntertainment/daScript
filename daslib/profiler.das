options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module profiler shared private

require debugapi
require fio
require strings
require uriparser
require ast

[skip_field_lock_check]
struct PerfNode
    fun : SimFunction?
    count : uint64
    total_time : int64
    enter_time : int64
    children : table<SimFunction?;PerfNode?>

struct PerfEvent
    fun : SimFunction?
    ts : int64
    entering : bool

[skip_field_lock_check]
struct PerfContext
    events : array<PerfEvent>

class ProfilerDebugAgent : DapiDebugAgent
    g_us0 : int64 = 0l
    g_events : table<uint64;PerfContext>
    g_out : FILE const?
    g_firstRecord : bool = true
    def override onInstall ( agent:DebugAgent? ) : void
        var args <- get_command_line_arguments()
        for argv,i in args,range(100500)
            if argv=="--das-profiler-log-file"
                if i+1<length(args)
                    g_out = fopen(args[i+1], "wb")
                    break
                break
        write("[")
        g_us0 = ref_time_ticks()
    def override onUninstall ( agent:DebugAgent? ) : void
        if g_out!=null
            write("]")
            g_out |> fclose()
            g_out = null
    def isProfileable ( var ctx:Context )
        return !(ctx.category.debug_context || ctx.category.macro_context || ctx.category.folding_context ||
            ctx.category.debugger_tick || ctx.category.debugger_attached)
    def override onCreateContext ( var ctx:Context ) : void
        if !isProfileable(ctx)
            return
        instrument_all_functions(ctx)
    def override onDestroyContext ( var ctx:Context ) : void
        if !isProfileable(ctx)
            return
        let tid = intptr(unsafe(addr(ctx)))
        if g_events |> key_exists(tid)
            dump_context(ctx, tid)
            delete g_events[tid]
    def override onInstrumentFunction ( var ctx:Context; fun:SimFunction?; entering:bool; userData:uint64 ) : void
        let tid = intptr(unsafe(addr(ctx)))
        g_events[tid].events |> push([[PerfEvent
            fun = fun,
            entering = entering,
            ts = get_time_nsec(g_us0)]])
    def dump(text:string)
        if g_firstRecord
            g_firstRecord = false
        else
            write(",\n")
        write(text)
    def write(text:string)
        if g_out != null
            g_out |> fprint(text)
    def dump_event(ev:PerfEvent; tid:uint64)
        let phase = ev.entering ? "B" : "E"
        let fnName = ev.fun.mangledName |> escape()
        dump("\{\"name\":\"{fnName}\",\"cat\": \"PERF\",\"ph\":\"{phase}\",\"pid\": 0,\"tid\":{int64(tid)},\"ts\":{ev.ts/1000l}\}")
    def dump_node ( node:PerfNode?; tab:int = -1 )
        if node==null
            return
        if node.fun!=null
            let tabs = repeat("  ",tab)
            to_log(LOG_INFO,"{tabs}{node.fun.mangledName} {int64(node.count)} {node.total_time}ns\n")
        for ch in values(node.children)
            dump_node(ch, tab+1)
    def dump_context_stack ( tid:uint64 )
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
                sp.total_time += ev.ts - sp.enter_time
                stack |> pop()
                sp = length(stack)!=0 ? stack |> back() : root
        dump_node(root)
        unsafe
            delete root
    def dump_meta ( var ctx:Context; tid:uint64 )
        let ctxName = empty(ctx.name) ? "" : " '{ctx.name}'"
        let wasDead = (ctx.category & context_category_flags dead) == context_category_flags dead
        ctx.category &= ~context_category_flags dead
        let ctxDesc = "{tid}{ctxName} cat:{ctx.category}" |> escape()
        dump("\{\"args\": \{ \"name\": \"{ctxDesc}\" \}, \"name\": \"thread_name\", \"cat\": \"__metadata\", \"ph\": \"M\", \"pid\": 0, \"tid\": {int64(tid)}, \"ts\": 0 \}")
        if wasDead { ctx.category |= context_category_flags dead; }
        // dump("\{\"args\": \{ \"value\" : \"ns\" \}, \"name\": \"displayTimeUnit\", \"cat\": \"__metadata\", \"ph\": \"M\", \"pid\": 0, \"tid\": {int64(tid)}, \"ts\": 0 \}")
    def dump_events ( tid:uint64 )
        for ev in g_events[tid].events
            dump_event(ev, tid)
    def dump_context ( var ctx:Context; tid:uint64 )
        dump_context_stack(tid)
        dump_meta(ctx, tid)
        dump_events(tid)
        delete g_events[tid].events
        g_events |> erase(tid)



def debug_agent ( ctx : Context )
    assert(this_context().category.debug_context)
    install_new_debug_agent(new ProfilerDebugAgent(), "profiler")

[_macro]
def installing
    if is_compiling_macros_in_module("profiler")
        fork_debug_agent_context(@@debug_agent)

