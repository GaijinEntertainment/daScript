options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require daslib/jobque_boost
require strings
require daslib/strings_boost
require daslib/clargs
require daslib/fio   // has_env_variable / get_env_variable (the DASLLAMA_BATCH_GRID_2D A/B rail)
require math
require dasllama/dasllama_math  // setup_dasllama_jobque_ + the runtime knob setters (engine-level, not re-exported)
require dasllama/dasllama_layout  // convert_model_to_metal_blob — the blob-flavor transform

// dasLLAMA parity harness — the dasLLAMA side of the token-for-token oracle check.
// Feed the prompt token IDs the reference (harness/oracle/simple_ids) produced, greedily generate N
// continuation tokens, and print them as `GEN_IDS: ...` (the exact format simple_ids prints, so the
// two streams diff directly). Arch-agnostic: load_gguf + generate route through the per-arch
// dispatch, so this validates any supported model with no harness change.
//
//   bin/daslang -jit modules/dasLLAMA/harness/parity.das -- \
//     -m <model.gguf> -n 40 --quant q8 --kv f16 --ids 1,9038,2501,263,931

[CommandLineArgs]
struct Args {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "Model GGUF path"
    model : string

    @clarg_required
    @clarg_doc = "Comma-separated prompt token ids (as the oracle tokenized them)"
    ids : string

    @clarg_short = "n"
    @clarg_doc = "Continuation tokens to generate (default: 40)"
    ngen : int = 40

    @clarg_doc = "Weight quant mode (default: q8)"
    quant : QuantMode = QuantMode.q8

    @clarg_doc = "KV cache dtype (default: f32)"
    kv : KVDtype = KVDtype.f32

    @clarg_doc = "Native K-quant planes A/B: 1 = keep Q4_K/Q5_K/Q6_K packed at load, 0 = q8-requant fallback, -1 = keep default"
    kquant_native : int = -1

    @clarg_doc = "GPU layers, llama-bench spelling: 0 = CPU, anything else = the whole graph on Metal, required (default: 0)"
    ngl : int = 0

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def private parse_ids(s : string) : array<int64> {
    var ids <- [for (part in split(s, ",")); int64(to_int(part)); where !empty(part)]
    return <- ids
}

[export]
def main {
    var res <- parse_args(type<Args>)
    if (res |> is_err) {
        print("error: {res |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "parity")
        return
    }
    let cfg <- res |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "parity")
        return
    }
    let n_gen = cfg.ngen
    let prompt <- parse_ids(cfg.ids)
    if (empty(prompt) || n_gen <= 0) {
        print_help(get_command_info(type<Args>), "parity")
        return
    }

    if (cfg.kquant_native >= 0) {   // load-time layout decision, must precede load_gguf
        set_kquant_native(cfg.kquant_native != 0)
    }
    if (cfg.ngl != 0) {   // before load_model — pins the portable backend, activates the metal overrides
        set_metal_mode(MetalMode.required)
    }
    var t <- load_gguf(cfg.model, cfg.quant)
    if (cfg.ngl != 0 && !convert_model_to_metal_blob(t)) {   // the Metal drivers serve blob-form models only
        panic("dasLLAMA parity: metal-blob transform declined — cannot serve --ngl on this load")
    }
    // Cap the KV context: only ~(prompt + n_gen) positions are used, and native seq_len (Llama-3 /
    // Phi / gemma4: 131072) would waste multi-GB KV allocations. Same cap test_parity.das applies.
    if (t.config.seq_len > 2048l) {
        t.config.seq_len = 2048l
    }
    let c = t.config
    var s = make_run_state(c, cfg.kv, cfg.kv)
    with_job_que() {
        setup_dasllama_jobque_()
        // A/B rail: force the 2-D batch chunk grid (0 = 1-D, 1 = fine, 2 = aligned) — the
        // knob-on GEN_IDS gate; parity stays raw-defaults otherwise
        if (has_env_variable("DASLLAMA_BATCH_GRID_2D")) {
            set_batch_grid_2d(to_int(get_env_variable("DASLLAMA_BATCH_GRID_2D")))
            print("batch grid 2d override: {get_batch_grid_2d()}\n")
        }
        let ids <- generate(t, s, prompt, length(prompt) + n_gen)
        // generate echoes prompt[1..] then the continuation, so the first generated token is at
        // index length(prompt)-1 (same indexing the runners use)
        let echo = length(prompt) - 1
        let avail = length(ids) - echo
        let count = min(n_gen, avail)
        let line = build_string() $(var w) {
            w |> write("GEN_IDS:")
            for (i in range(count)) {
                w |> write(" ")
                w |> write(ids[echo + i])
            }
        }
        print("{line}\n")
        if (count < n_gen) {
            print("NOTE: generate stopped early ({count}/{n_gen}) — emitted BOS\n")
        }
    }
}
