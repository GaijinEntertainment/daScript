options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget: the past-the-context cell prefills 2048 rows
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE030,LINT019 - the facade's embed / encode, inside the model-gated cells
require dasllama/dasllama_common   // POOLING_* and the Model the cells read
require daslib/fio   // nolint:STYLE030,LINT019 - path_join, inside the model-gated cells
require math
require _model_tier

// The embeddings read per model: an embedding file names its pooling (`{arch}.pooling_type`) and ends
// every text with EOS (`add_eos_token`), and the vector it was trained to be read at is the last row;
// a chat model names neither and is read by the mean. Qwen3-Embedding-0.6B's own model-card example is
// the oracle: its published cosine matrix over two queries and two passages, which the last-token read
// lands within 0.02 on the Q8_0 file while the mean read does not. Stocked suite; the cells skip
// off the JIT and without their carrier.

//! Qwen3-Embedding's model-card example: the instruction-prefixed queries, the two passages, and the
//! published cosine matrix for the 0.6B model (rows = queries, columns = passages)
let private EMB_TASK = "Instruct: Given a web search query, retrieve relevant passages that answer the query\nQuery: "
let private EMB_QUERIES = fixed_array("What is the capital of China?", "Explain gravity")
let private EMB_DOCS = fixed_array("The capital of China is Beijing.",
    "Gravity is a force that attracts two bodies towards each other. It gives weight to physical objects and is responsible for the movement of planets around the sun.")
let private EMB_REF = fixed_array(0.7646, 0.1414, 0.1355, 0.6000)
let private EMB_BAR = 0.02   //! the Q8_0 file against the card's bf16 numbers (reads 0.005 at most on the M1 Max)

def private cosine(a, b : array<float>) : float {
    var s = 0.0
    for (x, y in a, b) {
        s += x * y
    }
    return s   // both vectors are unit length by the facade's contract
}

//! the four cosines of the card's example under one pooling, row-major
def private cosine_matrix(m : Model; pooling : int64) : array<float> {
    var out : array<float>
    out |> reserve(4)
    for (q in EMB_QUERIES) {
        var inscope qv <- embed(m, "{EMB_TASK}{q}", pooling)
        for (d in EMB_DOCS) {
            var inscope dv <- embed(m, d, pooling)
            out |> push(cosine(qv, dv))
            delete dv
        }
        delete qv
    }
    return <- out
}

[test]
def test_embeddings_qwen3_embedding(t : T?) {
    t |> run("Qwen3-Embedding reads its pooling and its EOS off the file, and lands the model card's cosine matrix (0.6B Q8_0)") <| @(tt : T?) {
        let path = path_join(models_dir(), "Qwen3-Embedding-0.6B-Q8_0.gguf")
        return if (!jit_model_available(tt, path))
        var m <- load_model_(path, QuantMode.q8)
        tt |> equal(POOLING_LAST, m.config.pooling, "the file's pooling_type reads last-token")
        tt |> success(m.tok.bpe.add_eos, "the file asks for an appended EOS")
        var ids <- encode(m, EMB_DOCS[0], true, false)
        tt |> success(length(ids) > 1 && ids[length(ids) - 1] == m.tok.bpe.eos_id, "encode ends every text with the EOS the last row is read at ({ids})")
        var last <- cosine_matrix(m, -1l)   // the file's own pooling
        var mean <- cosine_matrix(m, POOLING_MEAN)
        var misses = 0
        for (c, ref in last, EMB_REF) {
            misses += !(abs(c - ref) <= EMB_BAR) ? 1 : 0   // a NaN cosine is a miss, never dropped by a running max
        }
        to_log(LOG_INFO, "qwen3-embedding last-token cosines {last} (card {EMB_REF}, {misses} outside {EMB_BAR}); mean-pooled {mean}\n")
        tt |> equal(0, misses, "the last-token read lands the card's matrix within {EMB_BAR} ({misses} of 4 outside)")
        //! the control: the mean read parts the relevant passage from the other by less than a tenth where the card parts them by 0.6
        tt |> success(mean[0] - mean[1] < 0.1, "the mean read cannot tell the capital passage from the gravity one ({mean[0]} against {mean[1]})")
        tt |> success(last[0] - last[1] > 0.5, "the last-token read does ({last[0]} against {last[1]})")
        delete last
        delete mean
        delete ids
        delete m
    }
}

[test]
def test_embeddings_past_the_context(t : T?) {
    t |> run("a text past the model's context embeds its leading seq_len tokens: a unit vector, no panic (tinyllama Q8)") <| @(tt : T?) {
        let path = path_join(models_dir(), "tinyllama-1.1b-chat-v1.0.Q8_0.gguf")
        return if (!jit_model_available(tt, path))
        var m <- load_model_(path, QuantMode.q8)
        // one token a word: a text of exactly the context's token count, then the same text with a thousand other words past it
        let cut_text = strip(build_string() $(var w) {
            for (_i in range64(m.config.seq_len - 1l)) {   // BOS takes the first slot; the trailing space is stripped below
                w |> write("a ")
            }
        })
        let long_text = build_string() $(var w) {
            w |> write(cut_text)
            for (_i in range(1000)) {
                w |> write(" zebra")
            }
        }
        let n = long_length(encode(m, long_text, true, false))
        tt |> success(n > m.config.seq_len, "the input runs past the context ({n} tokens, context {m.config.seq_len})")
        var vec <- embed(m, long_text)
        tt |> equal(m.config.dim, long_length(vec), "dim floats")
        var ss = 0.0
        var finite = true
        for (v in vec) {
            ss += v * v
            finite = finite && v == v
        }
        tt |> success(finite && abs(ss - 1.0) < 1e-3, "a finite unit vector (|v|^2 = {ss})")
        // the vector is the one the leading seq_len tokens give: the text cut at that token embeds to the same floats (an
        // uncut read would embed the zebras too and differ)
        tt |> equal(m.config.seq_len, long_length(encode(m, cut_text, true, false)), "the cut text encodes to the context's token count")
        var head <- embed(m, cut_text)
        var same = 0.0
        for (a, b in vec, head) {
            same += a * b
        }
        tt |> success(same > 0.9999, "the long text embeds as its first seq_len tokens: unit vectors at cosine {same} (the kernels' threaded sums move an element by 1e-4 between two reads)")
        // the control: the zebras alone embed elsewhere, so a read that took them in would miss the bar
        let zebras = build_string() $(var w) {
            for (_i in range(1000)) {
                w |> write(" zebra")
            }
        }
        var tail <- embed(m, zebras)
        var other = 0.0
        for (a, b in vec, tail) {
            other += a * b
        }
        tt |> success(other < 0.999, "... where the text past the cut embeds at cosine {other}")
        delete tail
        delete head
        delete vec
        delete m
    }
}

[test]
def test_embeddings_chat_model_default_mean(t : T?) {
    t |> run("a chat model's file names no pooling: the mean is the default, the last row an override (tinyllama Q8)") <| @(tt : T?) {
        let path = path_join(models_dir(), "tinyllama-1.1b-chat-v1.0.Q8_0.gguf")
        return if (!jit_model_available(tt, path))
        var m <- load_model_(path, QuantMode.q8)
        tt |> equal(POOLING_MEAN, m.config.pooling, "no pooling_type in the file: the mean")
        tt |> success(!m.tok.bpe.add_eos && !m.tok.spm.add_eos, "a chat model appends no EOS (tinyllama is SentencePiece, the other backend)")
        var dflt <- embed(m, EMB_DOCS[0])
        var mean <- embed(m, EMB_DOCS[0], POOLING_MEAN)
        var last <- embed(m, EMB_DOCS[0], POOLING_LAST)
        var cls <- embed(m, EMB_DOCS[0], POOLING_CLS)
        tt |> equal(m.config.dim, long_length(dflt), "dim floats")
        var same_as_mean = true
        var same_as_last = true
        var same_as_cls = true
        for (d, mn, ls, cl in dflt, mean, last, cls) {
            same_as_mean = same_as_mean && d == mn
            same_as_last = same_as_last && d == ls
            same_as_cls = same_as_cls && d == cl
        }
        tt |> success(same_as_mean, "the default read is the mean read, float for float")
        tt |> success(!same_as_last && !same_as_cls, "... and neither the last-token nor the first-row read")
        delete dflt
        delete mean
        delete last
        delete cls
        delete m
    }
}
