options gen2
options stack = 65536

require daslib/fio
require daslib/array_boost
require math
require strings

// Tutorial dasLLAMA-00: Problem Statement
//
// This file does not use dasLLAMA. It is a complete, if deliberately tiny,
// Llama-2 inference engine: map a real checkpoint, run the transformer, greedily
// choose the next token, and complete a prompt.
//
// It also tells another story. The first dasLLAMA commit was authored on
// 2026-06-27. It began with the naive matmul below; about a day later PR #3297
// contained Llama-2/3, GGUF, quantization, threading, prefill, tests, and chat.
// That was possible because all the steps from readable script to real machine
// code could stay in one programming environment.
//
// This is the seed, before it became a library. The design — checkpoint
// layout, tokenizer format, shape of the forward pass — follows Andrej
// Karpathy's llama2.c (run.c); credit where credit is due.
//
// Get the two llama2.c files:
//   https://huggingface.co/karpathy/tinyllamas/resolve/main/stories15M.bin
//   https://raw.githubusercontent.com/karpathy/llama2.c/master/tokenizer.bin
//
// Run:
//   daslang.exe -jit tutorials/dasLLAMA/00_problem_statement.das -- stories15M.bin tokenizer.bin "Once upon a time"
//
// The same source runs unchanged in either tier: the interpreter tells this
// story at ~6 tokens per second on a fast desktop, JIT at ~75. Try both.

// -----------------------------------------------------------------------------
// The five operations used by a Llama transformer
// -----------------------------------------------------------------------------

// y = W*x. W has d rows and n columns. Most inference time is spent here.
def matmul(var y : array<float>; w : array<float>#; x : array<float>; n, d : int) {
    for (row in range(d)) {
        var sum = 0.0
        let offset = row * n
        for (col in range(n)) {
            sum += w[offset + col] * x[col]
        }
        y[row] = sum
    }
}

// RMSNorm keeps the residual stream at a useful scale.
def rmsnorm(var out : array<float>; x : array<float>; weight : array<float>#; size : int) {
    var sum = 0.0
    for (i in range(size)) {
        let xi = x[i]
        sum += xi * xi
    }
    let scale = 1.0 / sqrt(sum / float(size) + 1e-5)
    for (i in range(size)) {
        out[i] = weight[i] * x[i] * scale
    }
}

// Softmax turns attention scores into weights which sum to one.
def softmax(var x : array<float>; size : int) {
    var largest = x[0]
    for (i in range(1, size)) {
        largest = max(largest, x[i])
    }
    var sum = 0.0
    for (i in range(size)) {
        x[i] = exp(x[i] - largest)
        sum += x[i]
    }
    for (i in range(size)) {
        x[i] /= sum
    }
}

// SiLU is the non-linearity in the feed-forward gate.
def silu(var x : array<float>; size : int) {
    for (i in range(size)) {
        x[i] /= 1.0 + exp(-x[i])
    }
}

// RoPE encodes position by rotating pairs of query/key coordinates.
def rope(var x : array<float>; position, head_size, size : int) {
    for (pair in range(size / 2)) {
        let i = pair * 2
        let frequency = 1.0 / pow(10000.0, float(i % head_size) / float(head_size))
        let angle = float(position) * frequency
        let a = x[i]
        let b = x[i + 1]
        let sin_angle = sin(angle)
        let cos_angle = cos(angle)
        x[i] = a * cos_angle - b * sin_angle
        x[i + 1] = a * sin_angle + b * cos_angle
    }
}

// -----------------------------------------------------------------------------
// What is in a llama2.c checkpoint
// -----------------------------------------------------------------------------

struct Config {
    dim : int
    hidden_dim : int
    layers : int
    heads : int
    kv_heads : int
    vocab : int
    sequence : int
    head_size : int
    kv_dim : int
    shared_classifier : bool
}

// The checkpoint is one flat fp32 array. These are element offsets, not copies.
struct Weights {
    token_embedding : int
    attention_norm : int
    query : int
    key : int
    value : int
    attention_output : int
    ffn_norm : int
    ffn_gate : int
    ffn_down : int
    ffn_up : int
    final_norm : int
    classifier : int
    end : int
}

def take(var cursor : int&; count : int) : int {
    let result = cursor
    cursor += count
    return result
}

def weight_layout(c : Config) : Weights {
    var at = 0
    var w = Weights(
        token_embedding = take(at, c.vocab * c.dim),
        attention_norm = take(at, c.layers * c.dim),
        query = take(at, c.layers * c.dim * c.dim),
        key = take(at, c.layers * c.dim * c.kv_dim),
        value = take(at, c.layers * c.dim * c.kv_dim),
        attention_output = take(at, c.layers * c.dim * c.dim),
        ffn_norm = take(at, c.layers * c.dim),
        ffn_gate = take(at, c.layers * c.dim * c.hidden_dim),
        ffn_down = take(at, c.layers * c.hidden_dim * c.dim),
        ffn_up = take(at, c.layers * c.dim * c.hidden_dim),
        final_norm = take(at, c.dim)
    )
    at += c.sequence * c.head_size   // old precomputed RoPE tables; this program computes them
    w.classifier = c.shared_classifier ? w.token_embedding : take(at, c.vocab * c.dim)
    w.end = at
    return w
}

// Scratch memory is allocated once and reused for every generated token.
struct State {
    x : array<float>
    xb : array<float>
    xb2 : array<float>
    gate : array<float>
    up : array<float>
    query : array<float>
    key : array<float>
    value : array<float>
    attention : array<float>
    logits : array<float>
    key_cache : array<float>
    value_cache : array<float>
}

def zeros(size : int) : array<float> {
    var result : array<float>
    result |> resize(size)
    return <- result
}

def make_state(c : Config) : State {
    return <- State(
        x <- zeros(c.dim),
        xb <- zeros(c.dim),
        xb2 <- zeros(c.dim),
        gate <- zeros(c.hidden_dim),
        up <- zeros(c.hidden_dim),
        query <- zeros(c.dim),
        key <- zeros(c.kv_dim),
        value <- zeros(c.kv_dim),
        attention <- zeros(c.sequence),
        logits <- zeros(c.vocab),
        key_cache <- zeros(c.layers * c.sequence * c.kv_dim),
        value_cache <- zeros(c.layers * c.sequence * c.kv_dim)
    )
}

// Present a matrix or norm vector inside the flat checkpoint as a borrowed view.
def project(
            var out : array<float>;
            model : array<float>#;
            offset : int;
            input : array<float>;
            n, d : int
            ) {
    array_view(model, offset, n * d) $(matrix : array<float>#) {
        matmul(out, matrix, input, n, d)
    }
}

def normalize(
              var out : array<float>;
              model : array<float>#;
              offset : int;
              input : array<float>;
              size : int
              ) {
    array_view(model, offset, size) $(weight : array<float>#) {
        rmsnorm(out, input, weight, size)
    }
}

// -----------------------------------------------------------------------------
// One token through the entire transformer
// -----------------------------------------------------------------------------

def forward( // nolint:STYLE038 — tutorial: one token through the whole transformer in one place is the lesson
            c : Config;
            w : Weights;
            model : array<float>#;
            var s : State;
            token, position : int
            ) {
    let dim = c.dim
    let kv_multiple = c.heads / c.kv_heads   // 1 for stories15M; larger models share one cached K/V head between several query heads to save memory
    let attention_scale = 1.0 / sqrt(float(c.head_size))
    // embedding: copy the token's learned row into x, the residual stream
    array_view(model, w.token_embedding + token * dim, dim) $(embedding : array<float>#) {
        for (i in range(dim)) {
            s.x[i] = embedding[i]
        }
    }

    for (layer in range(c.layers)) {
        let cache_layer = layer * c.sequence * c.kv_dim
        // attention: normalize x, then project it into query, key, value
        normalize(s.xb, model, w.attention_norm + layer * dim, s.x, dim)
        project(s.query, model, w.query + layer * dim * dim, s.xb, dim, dim)
        project(s.key, model, w.key + layer * dim * c.kv_dim, s.xb, dim, c.kv_dim)
        project(s.value, model, w.value + layer * dim * c.kv_dim, s.xb, dim, c.kv_dim)
        // rotate Q and K so their scores depend on relative position (RoPE)
        rope(s.query, position, c.head_size, dim)
        rope(s.key, position, c.head_size, c.kv_dim)
        // append this position's K and V to the layer's cache
        let cache_row = cache_layer + position * c.kv_dim
        for (i in range(c.kv_dim)) {
            s.key_cache[cache_row + i] = s.key[i]
            s.value_cache[cache_row + i] = s.value[i]
        }
        // per-head: score cached keys, softmax, blend cached values into s.xb (spent by now, safe to reuse)
        for (head in range(c.heads)) {
            let query_row = head * c.head_size
            let cache_head = (head / kv_multiple) * c.head_size

            for (past in range(position + 1)) {
                let key_row = cache_layer + past * c.kv_dim + cache_head
                var score = 0.0
                for (i in range(c.head_size)) {
                    score += s.query[query_row + i] * s.key_cache[key_row + i]
                }
                s.attention[past] = score * attention_scale
            }
            softmax(s.attention, position + 1)

            for (i in range(c.head_size)) {
                s.xb[query_row + i] = 0.0
            }
            for (past in range(position + 1)) {
                let value_row = cache_layer + past * c.kv_dim + cache_head
                let score = s.attention[past]
                for (i in range(c.head_size)) {
                    s.xb[query_row + i] += score * s.value_cache[value_row + i]
                }
            }
        }
        // mix the head outputs into one correction and add it to x
        project(s.xb2, model, w.attention_output + layer * dim * dim, s.xb, dim, dim)
        for (i in range(dim)) {
            s.x[i] += s.xb2[i]
        }
        // feed-forward: expand, SiLU-gate, project back down, add to x
        normalize(s.xb, model, w.ffn_norm + layer * dim, s.x, dim)
        project(s.gate, model, w.ffn_gate + layer * dim * c.hidden_dim, s.xb, dim, c.hidden_dim)
        project(s.up, model, w.ffn_up + layer * dim * c.hidden_dim, s.xb, dim, c.hidden_dim)
        silu(s.gate, c.hidden_dim)
        for (i in range(c.hidden_dim)) {
            s.gate[i] *= s.up[i]
        }
        project(s.xb, model, w.ffn_down + layer * c.hidden_dim * dim, s.gate, c.hidden_dim, dim)
        for (i in range(dim)) {
            s.x[i] += s.xb[i]
        }
    }
    // final norm, then score x against every vocabulary row
    normalize(s.xb, model, w.final_norm, s.x, dim)
    project(s.logits, model, w.classifier, s.xb, dim, c.vocab)
}

def most_likely(logits : array<float>) : int {
    var best = 0
    for (i in range(1, length(logits))) {
        if (logits[i] > logits[best]) {
            best = i
        }
    }
    return best
}

// -----------------------------------------------------------------------------
// A minimal llama2.c tokenizer: text to token ids and back
// -----------------------------------------------------------------------------

struct Tokenizer {
    pieces : array<string>
    scores : array<float>
    lookup : table<string; int>
}

def load_tokenizer(path : string; count : int) : Tokenizer {
    var tk = Tokenizer()
    let file = fopen(path, "rb")
    if (file == null) {
        panic("cannot open tokenizer '{path}'")
    }
    fmap(file) $(var data : array<uint8>#) {
        var cursor = 4   // max_token_length, unused by decoding
        for (_ in range(count)) {
            // records are unaligned (variable piece length), so pod_view rather than a typed array_view
            tk.scores |> push(pod_view(data, cursor, type<float>))
            let size = pod_view(data, cursor + 4, type<int>)
            array_view(data, cursor + 8, size) $(piece : array<uint8>#) {
                let text = string(piece)
                tk.lookup |> insert(text, length(tk.pieces))
                tk.pieces |> push(text)
            }
            cursor += 8 + size
        }
    }
    fclose(file)
    return <- tk
}

def encode(tk : Tokenizer; text : string) : array<int> {
    var tokens = [1] // BOS
    let dummy_prefix = tk.lookup?[" "] ?? -1 // SentencePiece marks word starts with a space
    if (!empty(text) && dummy_prefix >= 0) {
        tokens |> push(dummy_prefix)
    }
    // one byte view for the whole prompt: the UTF-8 walk and every piece lookup below read the
    // length it carries, so no call re-measures `text`
    peek_data(text) $(bytes : array<uint8> const#) {
        var begin = 0
        while (begin < length(bytes)) {
            var end = begin + 1
            while (end < length(bytes) && (int(bytes[end]) & int(0xC0)) == int(0x80)) {
                end++
            }
            let id = tk.lookup?[slice(bytes, begin, end)] ?? -1
            if (id >= 0) {
                tokens |> push(id)
            } else {
                for (i in range(begin, end)) {
                    tokens |> push(int(bytes[i]) + 3) // llama2.c byte-token fallback
                }
            }
            begin = end
        }
    }
    while (true) {
        var best_score = -1.0e10
        var best_id = -1
        var best_at = -1
        for (i in range(length(tokens) - 1)) {
            let id = tk.lookup?[tk.pieces[tokens[i]] + tk.pieces[tokens[i + 1]]] ?? -1
            if (id >= 0 && tk.scores[id] > best_score) {
                best_score = tk.scores[id]
                best_id = id
                best_at = i
            }
        }
        if (best_at < 0) {
            break
        }
        tokens[best_at] = best_id
        tokens |> erase(best_at + 1)
    }
    return <- tokens
}

def token_text(tk : Tokenizer; previous, token : int) : string {
    let text = tk.pieces[token]
    if (length(text) == 6 && starts_with(text, "<0x") && ends_with(text, ">")) {
        return to_char(to_int(slice(text, 1, 5), true))
    }
    if (previous == 1 && !empty(text) && first_character(text) == ' ') {
        return slice(text, 1)
    }
    return text
}

// -----------------------------------------------------------------------------
// Repeated forward calls prefill the prompt; then each result becomes the next input
// -----------------------------------------------------------------------------

def prefill(
            c : Config;
            w : Weights;
            model : array<float>#;
            var s : State;
            tokens : array<int>
            ) {
    for (position, token in range(length(tokens)), tokens) {
        forward(c, w, model, s, token, position)
    }
}

def complete(model_path, tokenizer_path, prompt : string) {
    let file = fopen(model_path, "rb")
    if (file == null) {
        panic("cannot open model '{model_path}'")
    }

    fmap(file) $(var bytes : array<uint8>#) {
        if (length(bytes) < 28) {
            panic("checkpoint is smaller than its seven-int header")
        }
        array_view(bytes, 0, 7, type<int>) $(header : array<int>#) {
            let signed_vocab = header[5]
            let c = Config(
                dim = header[0],
                hidden_dim = header[1],
                layers = header[2],
                heads = header[3],
                kv_heads = header[4],
                vocab = abs(signed_vocab),
                sequence = header[6],
                head_size = header[0] / header[3],
                kv_dim = header[0] * header[4] / header[3],
                shared_classifier = signed_vocab > 0   // llama2.c negates vocab to flag a separate classifier matrix
            )
            let w = weight_layout(c)
            if (length(bytes) < 28 + w.end * 4) {
                panic("checkpoint ends before its declared weights")
            }
            array_view(bytes, 28, (length(bytes) - 28) / 4, type<float>) $(model : array<float>#) {
                let tk <- load_tokenizer(tokenizer_path, c.vocab)
                var state <- make_state(c)
                let prompt_tokens <- encode(tk, prompt)
                if (length(prompt_tokens) > c.sequence) {
                    panic("prompt has {length(prompt_tokens)} tokens; model context is {c.sequence}")
                }

                print("dim={c.dim}, layers={c.layers}, heads={c.heads}, parameters≈{w.end}\n\n")
                let delimiter = 1
                var token = prompt_tokens[length(prompt_tokens) - 1]
                var position = length(prompt_tokens)
                prefill(c, w, model, state, prompt_tokens)
                fprint(fstdout(), prompt)
                while (true) {
                    let next = most_likely(state.logits)
                    if (next == delimiter) {
                        break
                    }
                    fprint(fstdout(), token_text(tk, token, next))
                    fflush(fstdout())
                    token = next
                    if (position >= c.sequence) {
                        break
                    }
                    forward(c, w, model, state, token, position)
                    position++
                }
                print("\n")
            }
        }
    }
    fclose(file)
}

[export]
def main() {
    let args <- get_command_line_arguments()
    let paths <- [for (arg in args); arg; where ends_with(arg, ".bin")]
    if (length(paths) < 2) {
        print("usage: daslang -jit tutorials/dasLLAMA/00_problem_statement.das -- stories15M.bin tokenizer.bin [\"prompt\"]\n")
        return
    }
    let prompt = ends_with(args[length(args) - 1], ".bin") ? "Once upon a time" : args[length(args) - 1]
    complete(paths[length(paths) - 2], paths[length(paths) - 1], prompt)
}
