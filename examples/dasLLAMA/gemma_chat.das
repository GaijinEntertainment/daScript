options gen2

require dasllama/dasllama_transformer
require dasllama/dasllama_tokenizer
require dasllama/dasllama_math
require daslib/jobque_boost
require daslib/random
require daslib/fio
require strings
require math

// Interactive chat over a Gemma-2-it GGUF (default gemma-2-2b-it). SentencePiece tokenizer
// (add_space_prefix=false) + the Gemma chat template — <bos>/<start_of_turn>/<end_of_turn>
// injected by id, the "{role}\n{content}" segments between them SPM-encoded. Gemma has no
// system role (the system line is folded into the first user turn). Reply stops on <end_of_turn>.
//
//   bin/daslang -jit examples/dasLLAMA/gemma_chat.das [-- <model.gguf>]
//   printf 'hi\n/quit\n' | bin/daslang -jit examples/dasLLAMA/gemma_chat.das

let DEFAULT_MODEL = "/Users/borisbatkin/Work/llama.cpp/models/gemma-2-2b-it-Q8_0.gguf"
let SYSTEM = "You are a helpful, friendly assistant. Keep answers concise."
let TEMP = 0.7
let TOP_K = 40l         // sample only from the K most-likely tokens (kills long-tail drift)
let PENALTY = 1.1       // repetition penalty on tokens already emitted this turn
let MAX_NEW = 256l      // max tokens per reply
let CTX = 4096l         // cap the KV-cache context (also Gemma-2's sliding-window size)

// Gemma chat special tokens (ids confirmed against the GGUF tokenizer metadata).
let BOS_TOK = 2l        // <bos>
let SOT_TOK = 106l      // <start_of_turn>
let EOT_TOK = 107l      // <end_of_turn>
let EOS_TOK = 1l        // <eos>

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
    print("Gemma-2 ready (Q8, {c.n_layers} layers, ctx {c.seq_len}). Type your message; /quit to exit.\n\n")

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

            // Build this turn's Gemma prefill. <bos> once; then a user turn and an empty model
            // turn. Gemma has no system role, so on the first turn the system line is folded
            // into the user content.
            var content = line
            var pre : array<int64>
            if (first) {
                pre |> push(BOS_TOK)
                if (!empty(SYSTEM)) {
                    content = "{SYSTEM}\n\n{line}"
                }
                first = false
            }
            pre |> push(SOT_TOK)
            pre |> push_from(encode(tok, "user\n{content}", false, false))
            pre |> push(EOT_TOK)
            pre |> push_from(encode(tok, "\n", false, false))
            pre |> push(SOT_TOK)
            pre |> push_from(encode(tok, "model\n", false, false))

            if (pos + int64(length(pre)) >= c.seq_len) {
                print("\n[context full — restart to continue]\n")
                break
            }
            pos = prefill(t, s, pre, pos, c.seq_len)

            // Stream the reply until <end_of_turn> / <eos> / budget.
            fprint(fstdout(), "Bot: ")
            var prev = EOT_TOK  // a non-BOS prev, so decode won't strip a leading space spuriously
            var recent : array<int64>
            var nout = 0l
            while (nout < MAX_NEW && pos < c.seq_len) {
                let next = sample(s, c.vocab_size, TEMP, PENALTY, TOP_K, recent, scratch, seed)
                if (next == EOT_TOK || next == EOS_TOK) {
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

            // Close the model turn in the KV context (<end_of_turn>\n) so the next turn continues cleanly.
            pos = prefill(t, s, [EOT_TOK], pos, c.seq_len)
            pos = prefill(t, s, encode(tok, "\n", false, false), pos, c.seq_len)
        }
    }
    print("bye\n")
}
