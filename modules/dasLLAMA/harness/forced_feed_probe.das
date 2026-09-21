options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

// The forced-feed probe: a CPU prefill of a counting prompt on a CPU session and on a GPU-decode
// session of the same model's blob twin, then N steps fed the CPU chain's greedy tokens on both,
// the per-step logits maxd and the argmax of each side - the instrument that separates a decode
// step's numerics from the drift a diverging chain hides.
//
//   daslang -jit modules/dasLLAMA/harness/forced_feed_probe.das -- <gguf> [--kv f16|f32|q8_0|tq4] [--steps N]

require dasllama/dasllama_transformer
require dasllama/dasllama_load
require dasllama/dasllama_layout
require dasllama/dasllama_math
require dasllama/dasllama_common
require dasllama/dasllama_tokenizer
require ?das_metal dasllama/dasllama_metal_decode
require daslib/jobque_boost
require daslib/clargs
require daslib/fio
require strings
require math

def greedy_top(lg : array<float>; vocab : int64) : int64 {
    var best = 0l
    var bestv = lg[0]
    for (i in range64(vocab)) {
        if (lg[i] > bestv) {
            bestv = lg[i]
            best = i
        }
    }
    return best
}

[CommandLineArgs]
struct Args {
    @clarg_positional
    @clarg_doc = "GGUF path"
    model : string

    @clarg_doc = "The KV codec: f16 | f32 | q8_0 | tq4"
    kv : KVDtype = KVDtype.f16

    @clarg_doc = "Forced steps after the prefill"
    steps : int = 8

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

[export]
def main() : int {
    allow_cpu_prefill()
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "forced_feed_probe")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help || empty(cfg.model)) {
        print_help(get_command_info(type<Args>), "forced_feed_probe")
        return cfg.help ? 0 : 1
    }
    let path = cfg.model
    let kdt = cfg.kv
    let nsteps = int64(cfg.steps)
    set_metal_mode(MetalMode.when_available)
    var tr <- load_model_(path, QuantMode.q8)
    tr.config.seq_len = min(tr.config.seq_len, 512l)
    var trg <- load_model_(path, QuantMode.q8)
    verify(convert_model_to_metal_blob(trg), "blob twin")
    trg.config.seq_len = min(trg.config.seq_len, 512l)
    let vocab = tr.config.vocab_size
    let text : string = build_string() $(var w) {
        for (i in range(1, 31)) {
            w |> write("{i}, ")
        }
    }
    let prompt <- encode(tr.tok, text, true, false)
    with_job_que() {
        setup_dasllama_jobque_()
        var sc = make_run_state(tr.config, kdt, kdt)
        var sg = make_run_state(trg.config, kdt, kdt)
        eval_(tr, sc, prompt)
        eval_(tr, sg, prompt)   //! the CPU prefill on both sessions: the decode step alone is the question
        verify(select_decode_override("metal"), "metal decode override")
        var tok = greedy_top(sc.logits, vocab)
        var cpu_ids : array<int64>
        var gpu_ids : array<int64>
        print("PROBE {base_name(path)} kv={kdt}: prompt {length(prompt)} tokens, {nsteps} forced steps\n")
        for (st in range64(nsteps)) {
            var one <- [tok]
            eval_(tr, sc, one)
            eval_(trg, sg, one)
            var maxd = 0.0
            for (v in range64(vocab)) {
                maxd = max(maxd, abs(sc.logits[v] - sg.logits[v]))
            }
            let ct = greedy_top(sc.logits, vocab)
            let gt = greedy_top(sg.logits, vocab)
            cpu_ids |> push(ct)
            gpu_ids |> push(gt)
            print("PROBE step {st}: fed {tok} cpu->{ct} gpu->{gt} maxd {maxd}{ct != gt ? "  <-- DIFF" : ""}\n")
            tok = ct
        }
        select_decode_override("")
        print("PROBE cpu {cpu_ids}\nPROBE gpu {gpu_ids}\n")
        delete sc
        delete sg
    }
    delete tr
    delete trg
    return 0
}
