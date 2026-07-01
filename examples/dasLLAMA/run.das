options gen2
options _jit_fast_math = true   // ggml-parity FP laxity (non-bit-exact, ~+10%); tests stay bit-exact

require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require daslib/jobque_boost
require daslib/fio
require strings
require math

// Greedy text completion over any GGUF model, with throughput stats. Collapses the old
// run/gguf_run/llama3_run trio into one path: load_model auto-selects the arch + tokenizer, encode
// the prompt, stream the greedy continuation, and print prefill / generation tok/s. Token-for-token
// oracle validation lives in the test suite (tests/dasLLAMA/test_forward.das + test_parity.das).
//
//   bin/daslang -jit examples/dasLLAMA/run.das -- <model.gguf>

let DEFAULT_MODEL = "/Users/borisbatkin/Work/llama.cpp/models/tinyllama-1.1b-v0.3-f16.gguf"
let PROMPT = "Once upon a time"
let N_GEN = 64l
let CTX = 4096l

[export]
def main {
    var model = DEFAULT_MODEL
    let cmd <- get_command_line_arguments()
    if (!empty(cmd) && ends_with(cmd[length(cmd) - 1], ".gguf")) {
        model = cmd[length(cmd) - 1]
    }
    print("loading {model}\n")
    var m <- load_model(model, QuantMode.q8)
    m.config.seq_len = min(m.config.seq_len, CTX)
    let c = m.config
    print("config: dim={c.dim} layers={c.n_layers} heads={c.n_heads}/{c.n_kv_heads} vocab={c.vocab_size} arch={m.arch}\n")

    let prompt <- encode(m, PROMPT, true, false)
    print("{PROMPT}")
    with_job_que() {
        set_jobque_fork_pool(true, true)  // pool per-job fork contexts + skip their init (pure-data matmul jobs)
        var s = create_session(m)
        generate(m, s, prompt, SamplingParams(), N_GEN) $(_id, piece) {
            fprint(fstdout(), piece)
            fflush(fstdout())
            return true
        }
        let st = stats(s)
        print("\n\n[prompt {st.n_prompt} tok | gen {st.n_gen} tok | ttft {int(st.ttft_s * 1000.0lf)}ms | ")
        print("prefill {int(st.prefill_tps)} t/s | gen {int(st.gen_tps)} t/s]\n")
    }
}
