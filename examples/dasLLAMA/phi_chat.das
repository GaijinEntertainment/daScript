options gen2
options _jit_fast_math = true   // ggml-parity FP laxity (non-bit-exact, ~+10%); tests stay bit-exact

require dasllama/dasllama_transformer
require dasllama/dasllama_tokenizer
require dasllama/dasllama_math
require daslib/jobque_boost
require daslib/random
require daslib/fio
require strings
require math

// Interactive chat over a Phi-3.5-mini-instruct GGUF. SentencePiece tokenizer + the Phi-3
// chat template — <|system|>/<|user|>/<|assistant|>/<|end|> injected by id, the "\n{content}"
// segments between them SPM-encoded (dummy-space prefix included, exactly as llama.cpp splits
// a templated prompt). The whole conversation stays in the KV cache; reply stops on <|end|>.
//
//   bin/daslang -jit examples/dasLLAMA/phi_chat.das [-- <model.gguf>]
//   printf 'hi\n/quit\n' | bin/daslang -jit examples/dasLLAMA/phi_chat.das

let DEFAULT_MODEL = "/Users/borisbatkin/Work/llama.cpp/models/Phi-3.5-mini-instruct-Q8_0.gguf"
let SYSTEM = "You are a helpful, friendly assistant. Keep answers concise."
let TEMP = 0.7
let TOP_K = 40l         // sample only from the K most-likely tokens (kills long-tail drift)
let PENALTY = 1.1       // repetition penalty on tokens already emitted this turn
let MAX_NEW = 256l      // max tokens per reply
let CTX = 4096l         // cap the KV-cache context (Phi-3.5's native 131072 is far too large)

// Phi-3 chat special tokens (ids confirmed against the GGUF tokenizer metadata).
let SYS_TOK = 32006l    // <|system|>
let USR_TOK = 32010l    // <|user|>
let ASST_TOK = 32001l   // <|assistant|>
let END_TOK = 32007l    // <|end|>
let EOD_TOK = 32000l    // <|endoftext|>

// Sample the next token from s.logits with repetition penalty + temperature + top-k.
def sample(var s : RunState; vocab : int64; temp, penalty : float; top_k : int64;
           recent : array<int64>; var scratch : array<float>; var seed : int4&) : int64 {
    // repetition penalty over only the last `pwin` tokens (bounded window, like llama.cpp's
    // penalty_last_n) — an unbounded window eventually suppresses frequent structural tokens
    // (newlines, spaces) and the reply collapses into a wall of text on long answers.
    let pwin = 64
    let pstart = length(recent) > pwin ? length(recent) - pwin : 0
    for (k in range(pstart, length(recent))) {
        let id = recent[k]
        let v = s.logits[id]
        s.logits[id] = v > 0.0 ? v / penalty : v * penalty
    }
    if (temp <= 0.0) {
        var best = 0l
        var bv = s.logits[0]
        for (i in range64(vocab)) {
            if (s.logits[i] > bv) {
                bv = s.logits[i]
                best = i
            }
        }
        return best
    }
    let inv = 1.0 / temp
    for (i in range64(vocab)) {
        s.logits[i] *= inv
    }
    for (i in range64(vocab)) {
        scratch[i] = s.logits[i]
    }
    var kth = -1.0e30
    for (_j in range64(top_k)) {
        var mx = -1.0e30
        var mi = 0l
        for (i in range64(vocab)) {
            if (scratch[i] > mx) {
                mx = scratch[i]
                mi = i
            }
        }
        kth = mx
        scratch[mi] = -1.0e30
    }
    for (i in range64(vocab)) {
        if (s.logits[i] < kth) {
            s.logits[i] = -1.0e30
        }
    }
    softmax(s.logits, vocab)
    let r = random_float(seed)
    var cdf = 0.0
    for (i in range64(vocab)) {
        cdf += s.logits[i]
        if (r < cdf) {
            return i
        }
    }
    return vocab - 1l
}

// Forward each token into the KV cache, advancing pos. Returns the new pos.
def prefill(t : Transformer; var s : RunState; toks : array<int64>; pos, seq_len : int64) : int64 {
    var p = pos
    for (tk in toks) {
        break if (p >= seq_len)
        forward(t, s, tk, p)
        p++
    }
    return p
}

[export]
def main {
    var model = DEFAULT_MODEL
    let cmd <- get_command_line_arguments()
    if (!empty(cmd) && ends_with(cmd[length(cmd) - 1], ".gguf")) {
        model = cmd[length(cmd) - 1]
    }
    print("loading {model} ...\n")
    var t <- load_gguf(model, QuantMode.q8)
    t.config.seq_len = min(t.config.seq_len, CTX)
    let tok <- load_tokenizer_gguf(model)
    let c = t.config
    print("Phi-3.5-mini ready (Q8, {c.n_layers} layers, ctx {c.seq_len}). The 3.8B is ~slow on CPU — be patient. /quit to exit.\n\n")

    var s = make_run_state(c)
    var seed = random_seed(1234)
    var scratch : array<float>
    scratch |> resize(c.vocab_size)
    var pos = 0l
    var first = true

    with_job_que() {
        set_jobque_fork_pool(true, true)
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

            // Build this turn's Phi-3 prefill (specials by id; "\n{content}" segments SPM-encoded).
            var pre : array<int64>
            if (first) {
                pre |> push(SYS_TOK)
                pre |> push_from(encode(tok, "\n{SYSTEM}", false, false))
                pre |> push(END_TOK)
                pre |> push_from(encode(tok, "\n", false, false))
                first = false
            }
            pre |> push(USR_TOK)
            pre |> push_from(encode(tok, "\n{line}", false, false))
            pre |> push(END_TOK)
            pre |> push_from(encode(tok, "\n", false, false))
            pre |> push(ASST_TOK)
            pre |> push_from(encode(tok, "\n", false, false))

            if (pos + int64(length(pre)) >= c.seq_len) {
                print("\n[context full — restart to continue]\n")
                break
            }
            pos = prefill(t, s, pre, pos, c.seq_len)

            // Stream the reply until <|end|> / <|endoftext|> / budget.
            fprint(fstdout(), "Bot: ")
            var prev = END_TOK  // != BOS(1), so decode won't strip a leading space spuriously
            var recent : array<int64>
            var nout = 0l
            while (nout < MAX_NEW && pos < c.seq_len) {
                let next = sample(s, c.vocab_size, TEMP, PENALTY, TOP_K, recent, scratch, seed)
                if (next == END_TOK || next == EOD_TOK) {
                    break
                }
                fprint(fstdout(), decode(tok, prev, next))
                fflush(fstdout())
                recent |> push(next)
                prev = next
                forward(t, s, next, pos)
                pos++
                nout++
            }
            print("\n\n")

            // Close the assistant turn in the KV context (<|end|>\n) so the next turn continues cleanly.
            pos = prefill(t, s, [END_TOK], pos, c.seq_len)
            pos = prefill(t, s, encode(tok, "\n", false, false), pos, c.seq_len)
        }
    }
    print("bye\n")
}
