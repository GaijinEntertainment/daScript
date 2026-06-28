options gen2

require dasllama_transformer
require dasllama_bpe
require dasllama_math
require daslib/jobque_boost
require daslib/random
require daslib/fio
require strings
require math

// Interactive chat over a Llama-3 Instruct GGUF (default Llama-3.1-8B). Byte-level BPE
// tokenizer + the Llama-3 chat template — special tokens injected by id, the text segments
// between them BPE-encoded (exactly how llama.cpp splits a templated prompt). The whole
// conversation stays in the KV cache across turns; reply stops on <|eot_id|>/<|end_of_text|>.
//
//   bin/daslang -jit modules/dasLLAMA/dasllama_llama3_chat.das [-- <model.gguf>]
//   printf 'hi\n/quit\n' | bin/daslang -jit modules/dasLLAMA/dasllama_llama3_chat.das

let DEFAULT_MODEL = "/Users/borisbatkin/Work/llama.cpp/models/Meta-Llama-3.1-8B-Instruct-Q8_0.gguf"
let SYSTEM = "You are a helpful, friendly assistant. Keep answers concise."
let TEMP = 0.6
let TOP_K = 40l         // sample only from the K most-likely tokens (kills long-tail drift)
let PENALTY = 1.1       // repetition penalty on tokens already emitted this turn
let MAX_NEW = 256l      // max tokens per reply
let CTX = 8192l         // cap context: the KV cache is n_layers*ctx*kv_dim — Llama-3's full
                        // 131072 would be ~34GB for the 8B. 8192 is plenty for a chat session.

// Sample the next token from s.logits with repetition penalty + temperature + top-k.
def sample(var s : RunState; vocab : int64; temp, penalty : float; top_k : int64;
           recent : array<int64>; var scratch : array<float>; var seed : int4&) : int64 {
    for (id in recent) {
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

// Append a Llama-3 header: <|start_header_id|>{role}<|end_header_id|>. The "\n\n{content}"
// that follows is encoded separately by the caller (matches llama.cpp's special-split).
def push_header(var pre : array<int64>; tok : BPETokenizer; sh, eh : int64; role : string) {
    pre |> push(sh)
    pre |> push_from(bpe_encode(tok, role))
    pre |> push(eh)
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
    t.config.seq_len = min(t.config.seq_len, CTX)  // cap KV-cache context (forward + run-state agree on seq_len)
    let tok <- load_bpe_tokenizer_gguf(model)
    let c = t.config
    let sh = special_id(tok, "<|start_header_id|>")
    let eh = special_id(tok, "<|end_header_id|>")
    let eot = special_id(tok, "<|eot_id|>")
    let eod = special_id(tok, "<|end_of_text|>")
    if (sh < 0l || eh < 0l || eot < 0l) {
        panic("dasLLAMA: model is missing Llama-3 chat special tokens")
    }
    print("Llama-3 ready (Q8, {c.n_layers} layers, ctx {c.seq_len}). The 8B is ~slow on CPU — be patient. /quit to exit.\n\n")

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

            // Build this turn's prefill (specials by id; segments between them BPE-encoded).
            var pre : array<int64>
            if (first) {
                pre |> push(tok.bos_id)  // <|begin_of_text|>
                push_header(pre, tok, sh, eh, "system")
                pre |> push_from(bpe_encode(tok, "\n\n{SYSTEM}"))
                pre |> push(eot)
                first = false
            }
            push_header(pre, tok, sh, eh, "user")
            pre |> push_from(bpe_encode(tok, "\n\n{line}"))
            pre |> push(eot)
            push_header(pre, tok, sh, eh, "assistant")
            pre |> push_from(bpe_encode(tok, "\n\n"))

            if (pos + int64(length(pre)) >= c.seq_len) {
                print("\n[context full — restart to continue]\n")
                break
            }
            pos = prefill(t, s, pre, pos, c.seq_len)

            // Stream the reply until <|eot_id|> / <|end_of_text|> / budget.
            fprint(fstdout(), "Bot: ")
            var recent : array<int64>
            var nout = 0l
            while (nout < MAX_NEW && pos < c.seq_len) {
                let next = sample(s, c.vocab_size, TEMP, PENALTY, TOP_K, recent, scratch, seed)
                if (next == eot || next == eod) {
                    break
                }
                fprint(fstdout(), bpe_decode(tok, next))
                fflush(fstdout())
                recent |> push(next)
                forward(t, s, next, pos)
                pos++
                nout++
            }
            print("\n\n")

            // Close the assistant turn in the KV context so the next turn continues cleanly.
            pos = prefill(t, s, [eot], pos, c.seq_len)
        }
    }
    print("bye\n")
}
