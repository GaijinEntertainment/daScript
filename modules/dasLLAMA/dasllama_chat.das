options gen2

require dasllama_transformer
require dasllama_tokenizer
require dasllama_math
require daslib/jobque_boost
require daslib/random
require daslib/fio
require strings

// Interactive ChatML chat REPL over a real GGUF model (TinyLlama-1.1B-Chat). Loads the model
// + tokenizer once, quantizes to Q8 (1.63x faster, lossless at greedy), then loops: read a
// line, append a ChatML turn, sample-generate the reply (streaming), keep the whole
// conversation in the KV cache across turns. Type /quit (or Ctrl-D) to exit.
//
//   bin/daslang -jit modules/dasLLAMA/dasllama_chat.das
// Non-interactive demo: pipe lines in --
//   printf 'hi\nwrite a haiku\n/quit\n' | bin/daslang -jit modules/dasLLAMA/dasllama_chat.das

let MODEL = "/Users/borisbatkin/Work/llama.cpp/models/tinyllama-1.1b-chat-v1.0.Q8_0.gguf"
let SYSTEM = "You are a helpful, friendly assistant. Keep answers concise."
let EOS = 2l   // </s> — the Zephyr turn separator and stop token
let TEMP = 0.7
let TOP_K = 40l         // sample only from the K most-likely tokens (kills long-tail drift)
let PENALTY = 1.3       // repetition penalty on tokens already emitted this turn (kills loops)
let MAX_NEW = 220l      // max tokens per reply

// Sample the next token from s.logits with repetition penalty + temperature + top-k.
// `recent` are tokens already emitted this turn; `scratch` is a reusable vocab-sized buffer.
def sample(var s : RunState; vocab : int64; temp, penalty : float; top_k : int64;
           recent : array<int64>; var scratch : array<float>; var seed : int4&) : int64 {
    // repetition penalty: push down logits of tokens we already produced
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
    // top-k: find the k-th largest logit, then mask everything below it
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
    print("loading {MODEL} ...\n")
    let t <- load_gguf(MODEL, QuantMode.q8)  // load straight into Q8: Q8_0 weights transcode, norms+embedding stay fp32
    let tok <- load_tokenizer_gguf(MODEL)
    let c = t.config
    print("TinyLlama-1.1B-Chat-v1.0 ready (Q8, {c.n_layers} layers, ctx {c.seq_len}). ")
    print("Type your message; /quit to exit.\n\n")

    var s = make_run_state(c)
    var seed = random_seed(1234)
    var scratch : array<float>
    scratch |> resize(c.vocab_size)
    var pos = 0l
    var first = true

    with_job_que() {
    set_jobque_fork_pool(true, true)  // pool per-job fork contexts + skip their init (pure-data matmul jobs)
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

        // Build this turn's Zephyr prefill: role markers are plain text, </s>=EOS by id.
        // <|system|>\n{sys}</s>  <|user|>\n{msg}</s>  <|assistant|>\n
        var pre : array<int64>
        if (first) {
            pre |> push(1l)  // BOS, once
            pre |> push_from(encode(tok, "<|system|>\n{SYSTEM}", false, false))
            pre |> push(EOS)
            pre |> push_from(encode(tok, "\n<|user|>\n{line}", false, false))
            first = false
        } else {
            pre |> push_from(encode(tok, "\n<|user|>\n{line}", false, false))
        }
        pre |> push(EOS)
        pre |> push_from(encode(tok, "\n<|assistant|>\n", false, false))

        if (pos + int64(length(pre)) >= c.seq_len) {
            print("\n[context full — restart to continue]\n")
            break
        }
        pos = prefill(t, s, pre, pos, c.seq_len)

        // Stream the reply, sampling until <|im_end|> / EOS / budget.
        fprint(fstdout(), "Bot: ")
        var prev = EOS  // not BOS, so decode won't strip leading space spuriously
        var recent : array<int64>  // tokens emitted this turn, for repetition penalty
        var nout = 0l
        while (nout < MAX_NEW && pos < c.seq_len) {
            let next = sample(s, c.vocab_size, TEMP, PENALTY, TOP_K, recent, scratch, seed)
            if (next == EOS) {
                break
            }
            var piece = decode(tok, prev, next)
            if (nout == 0l && !empty(piece) && first_character(piece) == ' ') {
                piece = slice(piece, 1)  // drop the leading space of the first reply token
            }
            fprint(fstdout(), piece)
            fflush(fstdout())
            recent |> push(next)
            prev = next
            forward(t, s, next, pos)
            pos++
            nout++
        }
        print("\n\n")

        // Close the assistant turn in the KV context (</s>) so the next turn continues cleanly.
        pos = prefill(t, s, [EOS], pos, c.seq_len)
    }
    }
    print("bye\n")
}
