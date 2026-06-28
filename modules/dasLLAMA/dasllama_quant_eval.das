options gen2

require dasllama_transformer
require dasllama_tokenizer
require dasllama_quant
require daslib/jobque_boost
require math

// Quant-quality detector (curriculum step 6, position 0: measure before optimizing).
// Q8 is lossy, so "looks fine" is not a metric. Three numbers:
//   1. reconstruction error  — how far dequant(quant(w)) drifts from w (per weight)
//   2. divergence step        — first token where Q8 greedy output leaves fp32's
//   3. coherence              — both stories printed, eyeball that Q8 still reads
// The trick: dequantize the Q8 weights and run the EXISTING fp32 forward on them. No new
// kernel needed yet — this isolates "is Q8 good enough" from "is the fast kernel correct".
let MODEL_PATH = "/Users/borisbatkin/Work/llama2.c/stories15M.bin"
let TOKENIZER_PATH = "/Users/borisbatkin/Work/llama2.c/tokenizer.bin"
let N_TOKENS = 64

def story(tk : Tokenizer; ids : array<int64>) {
    var prev = 1l
    for (id in ids) {
        print(decode(tk, prev, id))
        prev = id
    }
    print("\n")
}

[export]
def main {
    // fp32 reference, and a second copy we quantize-then-dequantize in place
    let t <- load_checkpoint(MODEL_PATH)
    let c = t.config
    let n = int64(length(t.blob))
    var tq <- load_checkpoint(MODEL_PATH)
    let qt <- quantize_q8_0(tq.blob, n)
    dequantize_q8_0(qt, tq.blob)

    let tk <- load_tokenizer(TOKENIZER_PATH, c.vocab_size)

    // 1) reconstruction error over the whole model
    let st = quant_error_stats(t.blob, tq.blob, n)
    let fp32_bytes = n * 4l
    let q8_bytes = int64(length(qt.q)) + int64(length(qt.scales)) * 4l
    print("=== Q8_0 reconstruction ({n} weights) ===\n")
    print("  mean |err|   {st.mean_abs_err}\n")
    print("  max  |err|   {st.max_abs_err}\n")
    print("  rms signal   {st.rms_signal}\n")
    print("  rms error    {st.rms_error}\n")
    print("  SNR          {st.snr_db} dB\n")
    print("  footprint    {fp32_bytes} -> {q8_bytes} bytes ({double(fp32_bytes) / double(q8_bytes)}x)\n")

    // 2) divergence: fp32 greedy vs Q8 greedy, both from BOS
    with_job_que() {
        var sref <- make_run_state(c)
        let ref_ids <- generate(t, sref, [1l], N_TOKENS)
        var sq <- make_run_state(c)
        let q_ids <- generate(tq, sq, [1l], N_TOKENS)

        let m = min(length(ref_ids), length(q_ids))
        var diverge = -1
        for (i in range(m)) {
            if (ref_ids[i] != q_ids[i]) {
                diverge = i
                break
            }
        }
        print("=== divergence (greedy, {N_TOKENS} steps) ===\n")
        print("  fp32 emitted {length(ref_ids)}, Q8 emitted {length(q_ids)}\n")
        if (diverge < 0 && length(ref_ids) == length(q_ids)) {
            print("  Q8 matches fp32 token-for-token\n")
        } elif (diverge < 0) {
            print("  agree on first {m} tokens, then lengths differ\n")
        } else {
            print("  first divergence at token {diverge}\n")
        }

        // 3) coherence: print both stories
        print("=== fp32 ===\n")
        story(tk, ref_ids)
        print("=== Q8 ===\n")
        story(tk, q_ids)
    }
}
