options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

require dasllama/dasllama
require dasllama/dasllama_common   // set_metal_mode, allow_cpu_prefill, eval_
require dasllama/dasllama_moe      // set_expert_reuse, expert_reuse_report
require dasllama/dasllama_math   // parallel_argmax
require daslib/clargs

// The routed experts a k+1-row verify reads as a union: the CPU chain walks a prompt and a greedy continuation with the
// expert-reuse instrument armed, and the report counts the distinct experts over windows of 2, 4 and 8 consecutive
// tokens against the slots they route - the bytes a verify that reads each expert once would stream.
//
//   bin/daslang -jit modules/dasLLAMA/harness/expert_overlap_probe.das -- -m <model.gguf> [--steps 96]

[CommandLineArgs]
struct Args {
    @clarg_short = "m"
    @clarg_doc = "Model GGUF path"
    model : string
    @clarg_doc = "greedy continuation steps after the prompt (default 96)"
    steps : int = 96
    @clarg_doc = "per-layer report lines"
    verbose : bool
    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

let PROMPT = "Write a complete, well-commented Python module that implements a token-bucket rate limiter with per-key buckets, a sliding-window variant, a thread-safe wrapper, and a pytest test suite. Write at least 600 lines. Do not stop early."

def private argmax_row(v : array<float>) : int64 => parallel_argmax(v, long_length(v))

def private run(a : Args) : int {
    set_metal_mode(MetalMode.off)
    allow_cpu_prefill(true)
    set_expert_reuse(true)
    with_dasllama_jobque() {
        var m <- load_model(a.model, QuantMode.q8)
        let prompt <- encode(m, PROMPT, true)
        var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        eval_(m, s, prompt)
        var one <- [0l]
        var ids : array<int64>
        ids |> reserve(a.steps)
        for (_i in range(a.steps)) {
            one[0] = argmax_row(s.logits)
            ids |> push(one[0])
            eval_(m, s, one)
        }
        print("prompt {length(prompt)} tokens; continuation ids {ids}\ncontinuation: {decode(m, ids)}\n")
        delete ids
        expert_reuse_report(a.verbose)
        delete one
        delete s
        delete m
    }
    return 0
}

[export]
def main : int {
    var parsed <- parse_args(type<Args>)
    if (parsed |> is_err) {
        print("error: {parsed |> unwrap_err}\n")
        print_help(get_command_info(type<Args>), "expert_overlap_probe")
        return 2
    }
    let a <- parsed |> move_unwrap
    if (a.help || empty(a.model)) {
        print_help(get_command_info(type<Args>), "expert_overlap_probe")
        return a.help ? 0 : 2
    }
    return run(a)
}
