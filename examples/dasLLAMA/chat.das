options gen2
options _jit_fast_math = true   // ggml-parity FP laxity (non-bit-exact, ~+10%); tests stay bit-exact

require dasllama/dasllama   // the public facade: engine + every arch registration + chat layer
require daslib/jobque_boost
require daslib/fio
require strings
require math

// One chat REPL for every supported model. load_model auto-selects the architecture, tokenizer, and
// chat template from the GGUF; the chat engine (dasllama_chat) renders each turn per the arch's
// registered template and keeps the whole conversation in the KV cache. Works for Llama-2/3,
// Qwen2.5/3, Phi-3.5, and Gemma-2/3 — the only per-model difference is data in the arch registry.
//
//   bin/daslang -jit examples/dasLLAMA/chat.das -- <model.gguf>
//   printf 'hi\n/quit\n' | bin/daslang -jit examples/dasLLAMA/chat.das -- <model.gguf>

let DEFAULT_MODEL = "/Users/borisbatkin/Work/llama.cpp/models/tinyllama-1.1b-chat-v1.0.Q8_0.gguf"
let SYSTEM = "You are a helpful, friendly assistant. Keep answers concise."
let CTX = 4096l   // cap the KV-cache context (the session's KV cache is sized to config.seq_len)

[export]
def main {
    var model = DEFAULT_MODEL
    let cmd <- get_command_line_arguments()
    if (!empty(cmd) && ends_with(cmd[length(cmd) - 1], ".gguf")) {
        model = cmd[length(cmd) - 1]
    }
    print("loading {model} ...\n")
    var m <- load_model(model, QuantMode.q8)
    m.config.seq_len = min(m.config.seq_len, CTX)
    let c = m.config
    print("{m.arch} ready (Q8, {c.n_layers} layers, ctx {c.seq_len}). Type your message; /quit to exit.\n\n")

    let params = SamplingParams(temp = 0.7, top_k = 40l, penalty = 1.1)

    with_job_que() {
        set_jobque_fork_pool(true, true)  // pool per-job fork contexts + skip their init (pure-data matmul jobs)
        var chat = create_chat(m, SYSTEM)
        while (true) {
            fprint(fstdout(), "You: ")
            fflush(fstdout())
            var line = fgets(fstdin())
            break if (empty(line))  // EOF / Ctrl-D
            line = strip(line)
            continue if (empty(line))
            if (line == "/quit" || line == "/exit") {
                break
            }
            add_user(chat, line)
            fprint(fstdout(), "Bot: ")
            respond(m, chat, params) $(piece) {
                fprint(fstdout(), piece)
                fflush(fstdout())
                return true
            }
            let st = stats(chat.session)
            print("\n\n[{st.n_gen} tok | ttft {int(st.ttft_s * 1000.0lf)}ms | prefill {int(st.prefill_tps)} t/s | gen {int(st.gen_tps)} t/s]\n")
        }
    }
    print("bye\n")
}
