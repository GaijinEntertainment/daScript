options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

require dasllama/dasllama   // nolint:STYLE029,LINT019 - the facade's Metal entries, used only inside the das_metal half
require dasllama/dasllama_common   // apply_box_profile_runtime, set_metal_mode, sel_selections
require dasllama/dasllama_scheduler
require ?das_metal dasllama/dasllama_metal_common   // nolint:STYLE030,LINT019 - metal_decode_spec_stats, inside the das_metal half
require ../performance/profile_common.das   // nolint:STYLE029,LINT019 - tune_gate, inside the das_metal half
require daslib/clargs

// The served path's decode chains: one scheduler stream a sampler (T 0.6 / top-k 20 / top-p 0.95 twice, then
// greedy) over a short prompt, each stream's tok/s with the chain's hits and misses, the selections landed and
// the device decodes counted - what dasllama-server does a request, without the wire.
//
//   bin/daslang -jit modules/dasLLAMA/harness/sampled_chain_probe.das -- -m <model.gguf> [-n 128] [--streams 4]

[CommandLineArgs]
struct Args {
    @clarg_short = "m"
    @clarg_doc = "Model GGUF path"
    model : string
    @clarg_short = "n"
    @clarg_doc = "tokens a stream generates (default 128)"
    ngen : int = 128
    @clarg_doc = "the scheduler's stream slots (default 4, the server's)"
    streams : int = 4
    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def private run_stream(m : Model; var sch : Scheduler; id : int64; prompt : array<int64>; params : SamplingParams; ngen : int64; tag : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var events : array<SchedEvent>
        let sp0 = metal_decode_spec_stats()
        let sel0 = sel_selections()
        let dec0 = metal_decode_stats().decodes
        var req <- PendingReq(id = id, params = params, max_new = ngen, seed = 7)
        req.prompt := prompt
        if (!submit(m, sch, req)) {
            print("{tag}: submit refused\n")
            return
        }
        var steps = 0
        while (scheduler_step(m, sch, events)) {
            steps++
            if (steps > 4096) {
                panic("the scheduler did not go idle")
            }
        }
        let sp1 = metal_decode_spec_stats()
        var fin = SchedEvent(stream = -1l)
        var toks : array<int64>
        for (e in events) {
            if (e.stream == id && e.kind == SchedEventKind.finished) {
                fin = e
            } elif (e.stream == id && e.kind == SchedEventKind.piece) {
                toks |> push(e.token)
            }
        }
        let tps = fin.gen_us > 0l ? double(fin.n_gen) * 1.0e6lf / double(fin.gen_us) : 0.0lf
        print("{tag}: gen {fin.n_gen} tok {tps} tok/s | chain hits {sp1.hits - sp0.hits} misses {sp1.misses - sp0.misses} | selections {sel_selections() - sel0} | device decodes {metal_decode_stats().decodes - dec0}\n")
        print("{tag}: {decode(m, toks)}\n")
    }
}

def private run(a : Args) : int {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_mode(MetalMode.required)
        apply_box_profile_runtime()
        return 1 if (!tune_gate())
        with_dasllama_jobque() {
            var m <- load_model(a.model, QuantMode.q8)
            var sch <- create_scheduler(m, int64(a.streams), 32l, 0l, 64l)
            let prompt <- encode(m, "The kitchen light is on, the bedroom lights are off, and the sprinkler runs at seven in the morning. Describe the house's evening routine in detail.", true)
            run_stream(m, sch, 1l, prompt, SamplingParams(temp = 0.6, top_k = 20l, top_p = 0.95), int64(a.ngen), "sampled T0.6")
            run_stream(m, sch, 2l, prompt, SamplingParams(temp = 0.6, top_k = 20l, top_p = 0.95), int64(a.ngen), "sampled T0.6 again")
            run_stream(m, sch, 3l, prompt, SamplingParams(), int64(a.ngen), "greedy")
            delete sch
            delete m
        }
        return 0
    } else {
        print("das_metal is not built into this binary\n")
        return 1
    }
}

[export]
def main : int {
    var parsed <- parse_args(type<Args>)
    if (parsed |> is_err) {
        print("error: {parsed |> unwrap_err}\n")
        print_help(get_command_info(type<Args>), "sampled_chain_probe")
        return 2
    }
    let a <- parsed |> move_unwrap
    if (a.help || empty(a.model)) {
        print_help(get_command_info(type<Args>), "sampled_chain_probe")
        return a.help ? 0 : 2
    }
    return run(a)
}
