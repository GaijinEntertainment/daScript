options gen2
options persistent_heap
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

def private l2(v : array<float>) : float {
    var s = 0.0
    for (x in v) {
        s += x * x
    }
    return sqrt(s)
}

//! the four cosines of the card's example under one pooling, row-major
def private cosine_matrix(m : Model; pooling : int64) : array<float> {
    var out : array<float>
    out |> reserve(4)
    for (q in EMB_QUERIES) {
        var qv <- embed(m, "{EMB_TASK}{q}", pooling)
        for (d in EMB_DOCS) {
            var dv <- embed(m, d, pooling)
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
        var worst = 0.0
        for (i in range(4)) {
            worst = max(worst, abs(last[i] - EMB_REF[i]))
        }
        to_log(LOG_INFO, "qwen3-embedding last-token cosines {last} (card {EMB_REF}, widest miss {worst}); mean-pooled {mean}\n")
        tt |> success(worst <= EMB_BAR, "the last-token read lands the card's matrix within {EMB_BAR} (widest miss {worst})")
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
def test_embeddings_chat_model_default_mean(t : T?) {
    t |> run("a chat model's file names no pooling: the mean is the default, the last row an override (tinyllama Q8)") <| @(tt : T?) {
        let path = path_join(models_dir(), "tinyllama-1.1b-chat-v1.0.Q8_0.gguf")
        return if (!jit_model_available(tt, path))
        var m <- load_model_(path, QuantMode.q8)
        tt |> equal(POOLING_MEAN, m.config.pooling, "no pooling_type in the file: the mean")
        tt |> success(!m.tok.bpe.add_eos, "a chat model appends no EOS")
        var dflt <- embed(m, EMB_DOCS[0])
        var mean <- embed(m, EMB_DOCS[0], POOLING_MEAN)
        var last <- embed(m, EMB_DOCS[0], POOLING_LAST)
        tt |> equal(m.config.dim, long_length(dflt), "dim floats")
        tt |> success(abs(l2(dflt) - 1.0) < 1.0e-4 && abs(l2(last) - 1.0) < 1.0e-4, "both reads are unit vectors")
        var same_as_mean = true
        var same_as_last = true
        for (d, mn, ls in dflt, mean, last) {
            same_as_mean = same_as_mean && d == mn
            same_as_last = same_as_last && d == ls
        }
        tt |> success(same_as_mean, "the default read is the mean read, float for float")
        tt |> success(!same_as_last, "... and not the last-token read")
        delete dflt
        delete mean
        delete last
        delete m
    }
}
