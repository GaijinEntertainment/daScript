options gen2

require dasllama/dasllama
require daslib/jobque_boost
require daslib/fio
require strings

// Tutorial dasLLAMA-01: Hello, Generation
//
// This tutorial covers:
//   - Loading a GGUF model with load_model (architecture + tokenizer auto-detected)
//   - Tokenization: encode / decode / piece
//   - Streaming text generation with generate()
//   - Throughput statistics with stats()
//
// Prerequisites: a GGUF model file on disk (models are not shipped with the repo).
// These tutorials use SmolLM2-135M-Instruct Q8_0 (~145MB, runs fast on any machine):
//   https://huggingface.co/bartowski/SmolLM2-135M-Instruct-GGUF
//
// Run: daslang.exe -jit tutorials/dasLLAMA/01_hello_generate.das -- <path/to/model.gguf>
// (always pass -jit — interpreted inference is far too slow for model work)

let PROMPT = "Once upon a time"
let N_GEN = 48l

// Resolve the model path: last command-line argument ending in .gguf, else $DASLLAMA_MODEL.
def model_path() : string {
    let cmd <- get_command_line_arguments()
    if (!empty(cmd) && ends_with(cmd[length(cmd) - 1], ".gguf")) {
        return cmd[length(cmd) - 1]
    }
    return get_env_variable("DASLLAMA_MODEL")
}

[export]
def main() {
    let path = model_path()
    if (empty(path)) {
        print("usage: daslang -jit tutorials/dasLLAMA/01_hello_generate.das -- <model.gguf>\n")
        print("(or set DASLLAMA_MODEL — a good tiny model is SmolLM2-135M-Instruct-Q8_0.gguf,\n")
        print(" https://huggingface.co/bartowski/SmolLM2-135M-Instruct-GGUF)\n")
        return
    }
    if (!jit_enabled()) {
        print("NOTE: running interpreted — re-run with -jit, inference is far too slow without it\n")
    }

    // ──────────────────────────────────────────────────────────────────────────
    // Section 1 — Load the model
    // ──────────────────────────────────────────────────────────────────────────
    //
    // load_model is the one entry point: it reads the GGUF, picks the architecture and
    // tokenizer from the file's metadata, and loads the weights at the precision you ask
    // for. QuantMode.q8 (int8) is the fast everyday choice; QuantMode.fp32 is the
    // token-exact reference the test suite validates against llama.cpp.

    print("loading {path}\n")
    var m <- load_model(path, QuantMode.q8)
    let c = m.config
    print("arch={m.arch} dim={c.dim} layers={c.n_layers} heads={c.n_heads}/{c.n_kv_heads} vocab={c.vocab_size} ctx={c.seq_len}\n\n")

    // ──────────────────────────────────────────────────────────────────────────
    // Section 2 — Tokens: encode, decode, piece
    // ──────────────────────────────────────────────────────────────────────────
    //
    // Models consume token ids, not text. encode() uses the model's own tokenizer
    // (BPE or SentencePiece, chosen at load); decode() is the inverse; piece() decodes
    // a single token — that's what streaming callbacks receive.

    let ids <- encode(m, PROMPT)
    print("encode(\"{PROMPT}\") -> {length(ids)} tokens: {ids}\n")
    print("decode back        -> \"{decode(m, ids)}\"\n")
    print("one piece at a time:")
    for (id in ids) {
        print(" [{piece(m, id)}]")
    }
    print("\n\n")

    // ──────────────────────────────────────────────────────────────────────────
    // Section 3 — Generate
    // ──────────────────────────────────────────────────────────────────────────
    //
    // A Session holds the KV cache and scratch buffers for one generation stream.
    // generate() prefills the prompt, then samples one token at a time, calling the
    // trailing block with (id, piece) — return false to stop early. SamplingParams()
    // defaults are greedy (always the most likely token); tutorial 03 covers the knobs.
    //
    // The kernels split their work across the job queue and REQUIRE one — model code
    // outside with_job_que() panics. Tutorial 05 measures what the threads buy.

    with_job_que() {
        set_jobque_fork_pool(true, true)   // pool per-job fork contexts (pure-data matmul jobs)
        var s = create_session(m)
        print("{PROMPT}")
        generate(m, s, ids, SamplingParams(), N_GEN) $(_id, piece) {
            fprint(fstdout(), piece)
            fflush(fstdout())
            return true
        }
        print("\n\n")

        // ──────────────────────────────────────────────────────────────────────
        // Section 4 — Stats
        // ──────────────────────────────────────────────────────────────────────
        //
        // stats() reports the last generate/respond call on the session: token counts,
        // time to first token, and prefill / generation throughput.

        let st = stats(s)
        print("prompt {st.n_prompt} tok | gen {st.n_gen} tok | ttft {int(st.ttft_s * 1000.0lf)}ms | ")
        print("prefill {int(st.prefill_tps)} t/s | gen {int(st.gen_tps)} t/s\n")
    }

    // (Greedy 135M output loops quickly — that's the model, not a bug; tutorial 03 shows
    // how the repetition penalty breaks the loop.)
}
