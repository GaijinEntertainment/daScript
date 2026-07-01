options gen2

require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require dasllama/dasllama_tokenizer
require dasllama/dasllama_quant
require daslib/jobque_boost
require math

// Q4_0 quant-quality detector (curriculum step 6.3). Three numbers, same as the Q8 detector:
//   1. reconstruction error  — how far dequant(quant(w)) drifts from w (per weight)
//   2. divergence step        — first token where the real Q4 forward leaves fp32's output
//   3. coherence              — both stories printed, eyeball that Q4 still reads
let MODEL_PATH = "/Users/borisbatkin/Work/llama2.c/stories15M.bin"
let TOKENIZER_PATH = "/Users/borisbatkin/Work/llama2.c/tokenizer.bin"
let N_TOKENS = 64

def story(tk : SpmTokenizer; ids : array<int64>) {
    var prev = 1l
    for (id in ids) {
        print(decode(tk, prev, id))
        prev = id
    }
    print("\n")
}

[export]
def main {
    var t <- load_checkpoint(MODEL_PATH)
    let c = t.config
    let n = int64(length(t.wblob))   // the big 2D weights — the part Q4 actually quantizes
    let tk <- load_tokenizer(TOKENIZER_PATH, c.vocab_size)

    // 1) reconstruction error over the quantized weights (per-weight Q4 round-trip)
    let qt <- quantize_q4_0(t.wblob, n)
    var deq4 : array<float>
    deq4 |> resize(n)
    dequantize_q4_0(qt, deq4)
    let st = quant_error_stats(t.wblob, deq4, n)
    let fp32_bytes = n * 4l
    let q4_bytes = int64(length(qt.q)) + int64(length(qt.scales)) * 4l
    print("=== Q4_0 reconstruction ({n} weights) ===\n")
    print("  mean |err|   {st.mean_abs_err}\n")
    print("  max  |err|   {st.max_abs_err}\n")
    print("  SNR          {st.snr_db} dB\n")
    print("  footprint    {fp32_bytes} -> {q4_bytes} bytes ({double(fp32_bytes) / double(q4_bytes)}x)\n")

    // 2) divergence: fp32 greedy vs the REAL all-weights Q4 forward, same model
    with_job_que() {
        var sref <- make_run_state(c)
        let ref_ids <- generate(t, sref, [1l], N_TOKENS)  // gold: fp32 (quant defaults fp32)
        quantize_weights_q4(t)                            // sets t.quant = q4
        var sq <- make_run_state(c)
        let q_ids <- generate(t, sq, [1l], N_TOKENS)

        let m = min(length(ref_ids), length(q_ids))
        var diverge = -1
        for (i in range(m)) {
            if (ref_ids[i] != q_ids[i]) {
                diverge = i
                break
            }
        }
        print("=== divergence (greedy, {N_TOKENS} steps) ===\n")
        print("  fp32 emitted {length(ref_ids)}, Q4 emitted {length(q_ids)}\n")
        if (diverge < 0 && length(ref_ids) == length(q_ids)) {
            print("  Q4 matches fp32 token-for-token\n")
        } elif (diverge < 0) {
            print("  agree on first {m} tokens, then lengths differ\n")
        } else {
            print("  first divergence at token {diverge}\n")
        }

        // 3) coherence: print both stories
        print("=== fp32 ===\n")
        story(tk, ref_ids)
        print("=== Q4 ===\n")
        story(tk, q_ids)
    }
}
