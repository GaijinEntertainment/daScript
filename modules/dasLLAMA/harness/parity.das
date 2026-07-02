options gen2
options persistent_heap   // >4GB single-array weight blobs (Mistral-7B Q8 7.1GB, gpt-oss-20b Q8 ~21GB) exceed LinearChunkAllocator's per-chunk uint32 cap

require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require daslib/jobque_boost
require strings
require daslib/strings_boost
require math

// dasLLAMA parity harness — the dasLLAMA side of the token-for-token oracle check.
// Feed the prompt token IDs the reference (harness/oracle/simple_ids) produced, greedily generate N
// continuation tokens, and print them as `GEN_IDS: ...` (the exact format simple_ids prints, so the
// two streams diff directly). Arch-agnostic: load_gguf + generate route through the per-arch
// dispatch, so this validates any supported model with no harness change.
//
//   bin/daslang -jit modules/dasLLAMA/harness/parity.das -- \
//     -m <model.gguf> -n 40 --quant q8 --ids 1,9038,2501,263,931

def private parse_quant(s : string) : QuantMode {
    if (s == "fp32") {
        return QuantMode.fp32
    }
    if (s == "q4") {
        return QuantMode.q4
    }
    return QuantMode.q8
}

def private parse_ids(s : string) : array<int64> {
    var ids <- [for (part in split(s, ",")); int64(to_int(part)); where !empty(part)]
    return <- ids
}

[export]
def main {
    var model = ""
    var n_gen = 40
    var quant = QuantMode.q8
    var prompt : array<int64>
    let args = get_command_line_arguments()
    let na = length(args)
    for (i in range(na)) {
        if (args[i] == "-m" && i + 1 < na) {
            model = args[i + 1]
        } elif (args[i] == "-n" && i + 1 < na) {
            n_gen = to_int(args[i + 1])
        } elif (args[i] == "--quant" && i + 1 < na) {
            quant = parse_quant(args[i + 1])
        } elif (args[i] == "--ids" && i + 1 < na) {
            prompt <- parse_ids(args[i + 1])
        }
    }
    if (empty(model) || empty(prompt) || n_gen <= 0) {
        print("usage: parity.das -- -m <model.gguf> -n <N> --quant <fp32|q8|q4> --ids <i0,i1,...>\n")
        return
    }

    var t <- load_gguf(model, quant)
    // Cap the KV context: only ~(prompt + n_gen) positions are used, and native seq_len (Llama-3 /
    // Phi / gemma4: 131072) would put the KV arrays over the 4GB uint32 chunk cap (see the wave-6
    // allocator item). Same cap test_parity.das applies.
    if (t.config.seq_len > 2048l) {
        t.config.seq_len = 2048l
    }
    let c = t.config
    var s = make_run_state(c)
    with_job_que() {
        set_jobque_fork_pool(true, true)
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
