options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

// Greedy continuation probe: renders one user turn through the model's chat template (thinking
// off), prints the rendered prompt and the greedy continuation - the text two engines must agree on
// before their speculative acceptance rates can be compared.
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_common
require dasllama/dasllama_math   // setup_dasllama_jobque_
require dasllama/dasllama_image   // load_model_cached - the .dlim rail
require dasllama/dasllama_tokenizer
require dasllama/dasllama_chat   // create_chat_renderer_ / set_thinking_ / add_user_ / render_turn_
require ?das_metal dasllama/dasllama_metal_common   // finish_pending_step
require ?das_metal dasllama/dasllama_metal_decode
require ?das_metal dasllama/dasllama_metal_prefill
require ../performance/profile_common.das   // apply_box_profile_runtime
require daslib/jobque_boost
require daslib/clargs
require daslib/fio
require strings
require math

[CommandLineArgs]
struct Args {
    @clarg_short = "m"
    @clarg_doc = "GGUF model path"
    model : string

    @clarg_doc = "Prompt text file (one user turn)"
    prompt : string

    @clarg_short = "n"
    @clarg_doc = "Tokens to generate"
    ngen : int = 128

    @clarg_doc = "Chat template thinking on"
    think : bool

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def private argmax_of(s : Session; vocab : int64) : int64 {
    var best = 0l
    var bv = s.logits[0]
    for (v in range64(1l, vocab)) {
        if (s.logits[v] > bv) {
            bv = s.logits[v]
            best = v
        }
    }
    return best
}

[export]
def main() {
    if (!jit_enabled()) {
        print("This probe is JIT-only - run with: daslang -jit <file>\n")
        return
    }
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "gen_probe")
        return
    }
    let cfg <- r |> move_unwrap
    if (cfg.help || empty(cfg.model) || empty(cfg.prompt)) {
        print_help(get_command_info(type<Args>), "gen_probe")
        return
    }
    var text = ""
    fopen(cfg.prompt, "rb") $(f) {
        if (f != null) {
            text = fread(f)
        }
    }
    apply_box_profile_runtime()
    set_metal_mode(MetalMode.required)
    let tk <- load_tokenizer_auto(cfg.model)
    with_job_que() {
        setup_dasllama_jobque_()
        var t <- load_model_cached(cfg.model, QuantMode.q8)
        t.config.seq_len = min(t.config.seq_len, 2048l)
        var cs <- create_chat_renderer_(t, "", 0l)
        set_thinking_(cs, cfg.think)
        add_user_(cs, strip(text))
        var ids <- render_turn_(t, cs)
        finalize(cs)
        print("== rendered prompt ({long_length(ids)} tokens)\n")
        for (id in ids) {
            print("{piece(tk, id)}")
        }
        print("\n== continuation ({cfg.ngen} tokens, greedy)\n")
        var s <- make_run_state(t.config, KVDtype.f16, KVDtype.f16)
        static_if (typeinfo builtin_module_exists(das_metal)) {
            select_prefill_override("metal")
            set_metal_prefill_min_npos(1l)
            select_decode_override("metal")
        }
        eval_(t, s, ids)
        var tok = argmax_of(s, t.config.vocab_size)
        var one <- [tok]
        for (_i in range(cfg.ngen)) {
            print("{piece(tk, tok)}")
            one[0] = tok
            eval_(t, s, one)
            static_if (typeinfo builtin_module_exists(das_metal)) {
                finish_pending_step()
            }
            tok = argmax_of(s, t.config.vocab_size)
        }
        print("\n== end\n")
        static_if (typeinfo builtin_module_exists(das_metal)) {
            select_decode_override("")
            select_prefill_override("")
            metal_decode_shutdown()
            metal_prefill_shutdown()
        }
        delete one
        delete s
        delete ids
        delete t
    }
}
