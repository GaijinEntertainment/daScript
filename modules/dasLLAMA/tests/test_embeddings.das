options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget: the past-the-context cell prefills 2048 rows
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // nolint:STYLE030,LINT019 - the facade's embed / encode, inside the model-gated cells
require dasllama/dasllama_common   // POOLING_* and the Model the cells read
require dasllama/dasllama_blocks   // embed_batch_ and EMBED_WINDOW_ROWS: the batch cells' window split
require dasllama/dasllama_gpu_tier   // moe_gpu_bake_tag: the device cell's armed-tier gate
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active / gpu_resident_prefills_: the device cell's served witness
require daslib/fio   // nolint:STYLE030,LINT019 - path_join, inside the model-gated cells
require ?das_metal dasllama/dasllama_metal_prefill   // metal_prefill_window_forms: the hidden-only window's witness
require _metal_blob_twin   // blob_twin: the Metal cell's blob-flavor model, minted in memory
require math
require _model_tier

// The embeddings read per model: an embedding file names its pooling (`{arch}.pooling_type`) and ends
// every text with EOS (`add_eos_token`), and the vector it was trained to be read at is the last row;
// a chat model names neither and is read by the mean. Qwen3-Embedding-0.6B's own model-card example is
// the oracle: its published cosine matrix over two queries and two passages, which the last-token read
// lands within 0.02 on the Q8_0 file while the mean read does not. Stocked suite; the cells skip off the JIT and without
// their carrier, the Metal cell on a build without dasMetal, and the Vulkan device cells with no armed Vulkan tier or where
// the whole-model driver declines the model (its decline line says why). Every load is in memory: no `.dlim` is minted.

//! Qwen3-Embedding's model-card example: the instruction-prefixed queries, the two passages, and the
//! published cosine matrix for the 0.6B model (rows = queries, columns = passages)
let private EMB_TASK = "Instruct: Given a web search query, retrieve relevant passages that answer the query\nQuery: "
let private EMB_QUERIES = fixed_array("What is the capital of China?", "Explain gravity")
let private EMB_DOCS = fixed_array("The capital of China is Beijing.",
    "Gravity is a force that attracts two bodies towards each other. It gives weight to physical objects and is responsible for the movement of planets around the sun.")
let private EMB_REF = fixed_array(0.7646, 0.1414, 0.1355, 0.6000)
let private EMB_MEAN_BAR = 0.999   //! the device's mean read (f16 mirror, GPU sums) against the CPU's (f32 cache) on one text
let private EMB_F32_BAR = 0.9999   //! the Metal window's vector against the CPU f32 model's, one row or many (reads 0.9999991 to 1.0 on the M5 Max)
let private EMB_BAR = 0.02 //! the Q8_0 file against the card's bf16 numbers (reads 0.005 at most on the M1 Max)

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

//! how many of `got`'s four cosines miss the card's matrix by more than EMB_BAR - a NaN is a miss
def private card_misses(got : array<float>) : int {
    var misses = 0
    for (c, ref in got, EMB_REF) {
        misses += !(abs(c - ref) <= EMB_BAR) ? 1 : 0
    }
    return misses
}

//! the q8 blob's split-K stamps' dispatches since the census reset: the 32- and 16-row stamps, half and f32 activations
def private q8_split_dispatches() : int64 {
    var n = 0l
    for (v in fixed_array("THSk", "TSk", "THSk16", "TSk16")) {
        n += metal_kernel_count_compiled("MetalQ8MulMm{v}_metal_q8_mulmm_t")
    }
    return n
}

//! the short window's q8 split-K form on the blob twin `m`: the embedding query of a few rows splits its sites' K, split-K off
//! walks it whole; the lever put back
def private q8_split_leg(tt : T?; m : Model) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prior_split = get_metal_prefill_split_k()
        set_metal_prefill_split_k(true)
        let sites0 = metal_prefill_split_k_sites()
        let q8sk0 = q8_split_dispatches()
        var inscope short_v <- embed(m, EMB_DOCS[0])
        let split_sites = metal_prefill_split_k_sites() - sites0
        let q8sk = q8_split_dispatches() - q8sk0
        tt |> success(split_sites > 0l && q8sk > 0l, "the short window's q8 sites took the split-K form ({split_sites} sites, {q8sk} q8 split dispatches)")
        set_metal_prefill_split_k(false)
        let sites1 = metal_prefill_split_k_sites()
        let q8sk1 = q8_split_dispatches()
        var inscope whole_v <- embed(m, EMB_DOCS[0])
        tt |> equal(metal_prefill_split_k_sites(), sites1, "the control: split-K off, no site split")
        tt |> equal(q8_split_dispatches(), q8sk1, "... and no q8 split stamp dispatched")
        set_metal_prefill_split_k(prior_split)
    }
}

[test]
def test_embeddings_hidden_only_window(t : T?) {
    t |> run("an embedding's Metal window runs no classifier and feeds no decode mirror, a chat window does both, and the card's matrix holds (0.6B Q8_0)") <| @(tt : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let path = path_join(models_dir(), "Qwen3-Embedding-0.6B-Q8_0.gguf")
            return if (!jit_model_available(tt, path))
            let prior_mode = get_metal_mode()
            let prior_mirror = get_metal_prefill_mirror()
            set_metal_mode(MetalMode.when_available)
            set_metal_prefill_mirror(true)   //! the chat control's mirror leg reads it
            var m <- blob_twin(tt, path, 4096l)   //! the Metal flavor in memory, its prefill override
            let served0 = metal_prefill_stats().prefills
            var inscope qv <- embed(m, "{EMB_TASK}{EMB_QUERIES[0]}")
            tt |> equal(metal_prefill_stats().prefills, served0 + 1l, "the embedding's forward rode the Metal window, one window")
            let fe = metal_prefill_window_forms()
            tt |> equal(fe.logits, "none", "the embedding's window runs no classifier (logits={fe.logits})")
            tt |> equal(fe.kv, "panels", "... and converts no K/V row into a decode mirror (kv={fe.kv})")
            var s <- create_session(m)
            var inscope toks <- encode(m, EMB_DOCS[1])
            eval(m, s, toks)
            let fc = metal_prefill_window_forms()
            tt |> equal(fc.logits, "gpu", "the control: a chat forward's window lands its logits on the GPU (logits={fc.logits})")
            tt |> success(fc.kv != "panels", "... and feeds the decode mirror (kv={fc.kv})")
            delete s
            q8_split_leg(tt, m)
            //! a one-row text - the EOS alone - is a one-row window too
            var inscope eos_ids <- encode(m, "", true, false)
            tt |> equal(long_length(eos_ids), 1l, "the empty text encodes to its EOS alone ({eos_ids})")
            let served_one = metal_prefill_stats().prefills
            var inscope one_row <- embed(m, "")
            tt |> equal(metal_prefill_stats().prefills, served_one + 1l, "the one-row embedding rode the Metal window")
            var inscope pair <- embed_batch(m, ["", EMB_DOCS[0]])
            let pair_c = cosine(pair[0], one_row)
            to_log(LOG_INFO, "one-row text in a batch on the Metal window: cosine {pair_c} to its single\n")
            tt |> success(pair_c > 0.9999, "the batch's one-row text reads its single's vector (cosine {pair_c})")
            let pair_other = cosine(pair[1], one_row)
            tt |> success(is_finite(pair_other) && pair_other < 0.9999, "the control: the batch's other text misses the bar ({pair_other})")
            var inscope got <- cosine_matrix(m, POOLING_LAST)
            for (g, w in got, EMB_REF) {
                tt |> success(abs(g - w) <= EMB_BAR, "the card's cosine {w} on the Metal window reads {g}")
            }
            var inscope mean <- cosine_matrix(m, POOLING_MEAN)
            tt |> success(card_misses(mean) > 0, "the control: the mean read on the Metal window misses the card's matrix ({mean})")
            delete m
            set_metal_mode(MetalMode.off)
            var cpu <- load_model_(path, QuantMode.q8)
            let served1 = metal_prefill_stats().prefills
            var inscope cpu_v <- embed(cpu, EMB_DOCS[0])
            tt |> equal(metal_prefill_stats().prefills, served1, "the control: with Metal off the embedding rides no Metal window")
            var inscope q8_one <- embed(cpu, "")
            delete cpu
            //! the reference is the f32 model: the CPU q8 chain quantizes its activations to Q8_0 blocks, which a first row's
            //! outlier activations cost about a percent of cosine (the q8 read logged beside)
            var f32 <- load_model_(path, QuantMode.fp32)
            var inscope cpu_one <- embed(f32, "")
            var inscope cpu_x <- embed(f32, "x")
            let one_c = cosine(one_row, cpu_one)
            let other_c = cosine(one_row, cpu_x)
            to_log(LOG_INFO, "one-row embedding: Metal against the CPU f32 model cosine {one_c}; against its \"x\" {other_c}; the CPU q8 chain against f32 {cosine(q8_one, cpu_one)}\n")
            tt |> success(one_c >= EMB_F32_BAR, "the one-row window's vector is the CPU f32 model's (cosine {one_c})")
            tt |> success(is_finite(other_c) && other_c < EMB_F32_BAR, "the control: against another text's f32 vector it misses the bar ({other_c})")
            delete f32
            set_metal_prefill_mirror(prior_mirror)
            set_metal_mode(prior_mode)
        } else {
            tt |> skip("dasMetal not built")
        }
    }
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
        let misses = card_misses(last)
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

//! the least cosine between each text's batch vector and the single `embed` vector of the text `shift` places on - 0 its own,
//! 1 the control (another text's) - with -1 for a count or width mismatch and for a NaN cosine
def private batch_vs_single(m : Model; texts : array<string>; pooling, dims, window_rows : int64; shift : int = 0) : float {
    var inscope batch <- embed_batch_(m, texts, pooling < 0l ? m.config.pooling : pooling, dims, window_rows, embed_session_dtype(m),
        moe_gpu_resident_scratch_serves(m))
    let n = length(texts)
    return -1.0 if (length(batch) != n)
    var worst = 1.0
    for (b, i in batch, count()) {
        var inscope one <- embed(m, texts[(i + shift) % n], pooling, dims)
        return -1.0 if (length(one) != length(b))
        let c = cosine(one, b)
        worst = is_nan(c) ? -1.0 : min(worst, c)
    }
    return worst
}

//! the batch cells' texts: the card's queries and passages, a one-token text, and two lines of different lengths
def private batch_texts() : array<string> {
    var texts <- [for (q in EMB_QUERIES); "{EMB_TASK}{q}"]
    texts |> push_from([for (d in EMB_DOCS); d])
    texts |> push("x")
    texts |> push("Turn on the kitchen lights.")
    texts |> push("What will the weather be tomorrow morning, and should I take an umbrella when I go out?")
    return <- texts
}

[test]
def test_embeddings_batch_matches_single(t : T?) {
    t |> run("embed_batch gives each text the vector embed gives it alone, in input order - one window, split windows, the Matryoshka cut (0.6B Q8_0)") <| @(tt : T?) {
        let path = path_join(models_dir(), "Qwen3-Embedding-0.6B-Q8_0.gguf")
        return if (!jit_model_available(tt, path))
        var m <- load_model_(path, QuantMode.q8)
        var texts <- batch_texts()
        let one_window = batch_vs_single(m, texts, -1l, 0l, EMBED_WINDOW_ROWS)
        tt |> success(one_window > 0.9999, "one window: the least cosine to the single vectors is {one_window}")
        let split = batch_vs_single(m, texts, -1l, 0l, 40l)
        tt |> success(split > 0.9999, "windows of 40 rows (the batch split across several): the least cosine is {split}")
        let cut = batch_vs_single(m, texts, -1l, 256l, EMBED_WINDOW_ROWS)
        tt |> success(cut > 0.9999, "the leading 256 components, renormalized: the least cosine is {cut}")
        var inscope order <- embed_batch(m, [texts[3], texts[0]])
        var inscope first <- embed(m, texts[3])
        tt |> success(cosine(order[0], first) > 0.9999 && cosine(order[1], first) < 0.9, "the vectors come back in input order")
        delete texts
        delete m
    }
}

[test]
def test_embeddings_lone_forwards(t : T?) {
    t |> run("a batch's one-row text and every text of a q4_0 model run embed_forward alone, outside the shared window, and land the vector embed gives them (0.6B Q8_0, and loaded as q4_0)") <| @(tt : T?) {
        let path = path_join(models_dir(), "Qwen3-Embedding-0.6B-Q8_0.gguf")
        return if (!jit_model_available(tt, path))
        var m <- load_model_(path, QuantMode.q8)
        moe_gpu_drop_model()   //! a device tier arms each load: dropped, the batch takes the host windows this cell is about
        var texts <- ["", EMB_DOCS[0], EMB_DOCS[1]]
        tt |> equal(length(encode(m, texts[0])), 1, "the empty text is one row - its EOS")
        var inscope rows_only <- [EMB_DOCS[0], EMB_DOCS[1]]
        let lone0 = embed_lone_forwards()
        var inscope windowed <- embed_batch_(m, rows_only, m.config.pooling, 0l, EMBED_WINDOW_ROWS)
        tt |> equal(embed_lone_forwards(), lone0, "the control: two texts of several rows share the window, none runs alone")
        let lone1 = embed_lone_forwards()
        let one_row = batch_vs_single(m, texts, -1l, 0l, EMBED_WINDOW_ROWS)
        tt |> equal(embed_lone_forwards() - lone1, 1, "the one-row text ran embed_forward alone, the other two shared the window")
        tt |> success(one_row > 0.9999, "the one-row text beside two others: the least cosine to the single vectors is {one_row}")
        let one_ctrl = batch_vs_single(m, texts, -1l, 0l, EMBED_WINDOW_ROWS, 1)
        tt |> success(one_ctrl < 0.9999, "the control: against another text's single the bar misses ({one_ctrl})")
        delete m
        var q4 <- load_model_(path, QuantMode.q4_0)
        moe_gpu_drop_model()
        tt |> equal(q4.quant, QuantMode.q4_0, "the model serves its q4_0 planes")
        let lone2 = embed_lone_forwards()
        let q4_batch = batch_vs_single(q4, texts, -1l, 0l, EMBED_WINDOW_ROWS)
        tt |> equal(embed_lone_forwards() - lone2, length(texts), "every text of the q4_0 batch ran embed_forward alone")
        tt |> success(q4_batch > 0.9999, "the q4_0 model's batch against its singles: the least cosine is {q4_batch}")
        let q4_ctrl = batch_vs_single(q4, texts, -1l, 0l, EMBED_WINDOW_ROWS, 1)
        tt |> success(q4_ctrl < 0.9999, "the control: against another text's single the bar misses ({q4_ctrl})")
        delete q4
        delete texts
    }
}

//! `t`'s levers for a Vulkan device cell, as they stood before it: the whole-model route and the region context pin
struct private DeviceLevers {
    route : bool
    ctx_max : int64
}

//! the device cells' levers pinned - the whole-model route on, the region context at `ctx_max` - and what they were
def private device_levers(ctx_max : int64) : DeviceLevers {
    let was = DeviceLevers(route = gpu_resident_route_lever(), ctx_max = gpu_ctx_pins_().ctx_max)
    set_gpu_resident_route(true)
    set_gpu_ctx_max(ctx_max)
    return was
}

def private restore_levers(was : DeviceLevers) {
    set_gpu_resident_route(was.route)
    set_gpu_ctx_max(was.ctx_max)
}

//! `path` loaded in memory with the whole-model driver armed on it, or a skip naming why not (the levers restored either way
//! by the caller)
def private served_load(tt : T?; path : string; var m : Model) : bool {
    if (empty(moe_gpu_bake_tag())) {
        tt |> skip("no armed vulkan tier (DASLLAMA_GPU=1 on a box with a device)")
        return false
    }
    delete m
    m <- load_model_(path, QuantMode.q8)
    if (!moe_gpu_resident_active()) {
        moe_gpu_drop_model()
        tt |> skip("the whole-model driver did not take the model on this box (the decline line above)")
        return false
    }
    return true
}

[test]
def test_embeddings_on_the_resident_driver(t : T?) {
    t |> run("on an armed Vulkan tier the driver serves Qwen3-Embedding whole from the device: embed and embed_batch prefill on it with no call passed to the CPU rails, and land the card's matrix (0.6B Q8_0)") <| @(tt : T?) {
        let path = path_join(models_dir(), "Qwen3-Embedding-0.6B-Q8_0.gguf")
        return if (!jit_model_available(tt, path))
        let was = device_levers(0l)
        var m = Model()
        if (!served_load(tt, path, m)) {
            restore_levers(was)
            return
        }
        tt |> equal(embed_session_dtype(m), gpu_device_session_dtype(), "the embed sessions take the armed mirror codec")
        let served0 = gpu_resident_prefills_()
        var inscope got <- cosine_matrix(m, POOLING_LAST)
        tt |> equal(gpu_resident_prefills_() - served0, 6l, "each of the six texts prefilled on the device, one window each")
        for (g, w in got, EMB_REF) {
            tt |> success(abs(g - w) <= EMB_BAR, "the card's cosine {w} on the device reads {g}")
        }
        var inscope mean <- cosine_matrix(m, POOLING_MEAN)
        tt |> success(card_misses(mean) > 0, "the control: the mean read on the device misses the card's matrix ({mean})")
        var texts <- batch_texts()
        let n = long_length(texts)
        let served1 = gpu_resident_prefills_()
        let scratch1 = moe_gpu_resident_scratch_windows()
        var inscope pooled <- embed_batch(m, texts)
        tt |> equal(moe_gpu_resident_scratch_windows() - scratch1, 1l, "the batch's texts sat back to back in one scratch region, one window")
        tt |> equal(gpu_resident_prefills_() - served1, n, "... every text a prefill on the device")
        tt |> equal(int64(embed_pool_device_sessions()), n, "the facade's batch rides device-home sessions - no host K/V a text")
        let worst = batch_vs_single(m, texts, -1l, 0l, EMBED_WINDOW_ROWS)
        tt |> success(worst > 0.9999, "the device's batch against its singles: least cosine {worst}")
        let other = batch_vs_single(m, texts, -1l, 0l, EMBED_WINDOW_ROWS, 1)
        tt |> success(other < 0.9999, "the control: each batch vector against another text's single misses the bar ({other})")
        //! a mean read past the last layer's 32-row FFN tail: every row the window lands must have run the whole stack
        let long_text = "{EMB_DOCS[1]} {EMB_DOCS[1]}"
        var inscope long_ids <- encode(m, long_text)
        tt |> success(length(long_ids) > 32, "the mean read's text runs past the last layer's 32-row tail ({length(long_ids)} tokens)")
        var inscope dev_mean <- embed(m, long_text, POOLING_MEAN)
        var inscope passes <- gpu_cpu_passes()
        tt |> equal(length(passes), 0, "no call passed to the CPU rails ({passes})")
        let served2 = gpu_resident_prefills_()
        let scratch2 = moe_gpu_resident_scratch_windows()
        host_route_off(m, EMB_DOCS[0])
        tt |> equal(gpu_resident_prefills_(), served2, "the control: a host-cached text with the resident prefill pinned off prefills nothing on the device")
        tt |> equal(moe_gpu_resident_scratch_windows(), scratch2, "... and takes no scratch window")
        delete texts
        delete m
        moe_gpu_drop_model()
        var cpu <- load_model_(path, QuantMode.q8)
        moe_gpu_drop_model()   //! the device tier arms each load: dropped, the model reads on the CPU rails
        var inscope cpu_mean <- embed_(cpu, long_text, POOLING_MEAN, 0l, KVDtype.f32)
        tt |> success(cosine(dev_mean, cpu_mean) > EMB_MEAN_BAR, "the device's mean read is the CPU's ({cosine(dev_mean, cpu_mean)})")
        var inscope cpu_other <- embed_(cpu, EMB_DOCS[0], POOLING_MEAN, 0l, KVDtype.f32)
        tt |> success(cosine(dev_mean, cpu_other) < EMB_MEAN_BAR, "the control: against another text's CPU mean read it misses the bar ({cosine(dev_mean, cpu_other)})")
        delete cpu
        restore_levers(was)
    }
}

//! the route-off leg of a device cell: `text` on a host-cached session in the driver's codec with the resident prefill pinned
//! off - the driver passes it to the CPU rails, so no device counter moves; the pin is put back
def private host_route_off(m : Model; text : string) {
    let was = resident_prefill_allowed()
    set_resident_prefill_allowed(false)
    var inscope v <- embed_batch_(m, [text], m.config.pooling, 0l, EMBED_WINDOW_ROWS, gpu_device_session_dtype(), false)
    set_resident_prefill_allowed(was)
}

//! the windows `embed_batch_` cuts `lens` into under `window_rows`: a window takes texts while their rows fit, and always one
def private windows_for(lens : array<int64>; window_rows : int64) : int64 {
    var windows = 0l
    var rows = 0l
    for (len, i in lens, count()) {
        if (i == 0 || rows + len > window_rows) {
            windows++
            rows = 0l
        }
        rows += len
    }
    return windows
}

[test]
def test_embeddings_window_rows_follow_the_region(t : T?) {
    t |> run("embed_window_rows: the armed driver's scratch region bounds a batch window where it serves the model - a batch past the region splits into windows and still matches its singles - and EMBED_WINDOW_ROWS bounds it elsewhere (0.6B Q8_0)") <| @(tt : T?) {
        let path = path_join(models_dir(), "Qwen3-Embedding-0.6B-Q8_0.gguf")
        return if (!jit_model_available(tt, path))
        let region = 256l   //! the cap's floor: a region far under EMBED_WINDOW_ROWS, so the two bounds differ
        let was = device_levers(region)
        var m = Model()
        if (!served_load(tt, path, m)) {
            restore_levers(was)
            return
        }
        tt |> equal(gpu_device_session_ctx(), region, "the driver armed the capped region")
        tt |> equal(embed_window_rows(m), region, "a served model's window is the region's context")
        var texts <- [for (i in range(12)); "{i}: {EMB_DOCS[1]}"]
        var lens <- [for (text in texts); long_length(encode(m, text))]
        let want = windows_for(lens, region)
        tt |> success(want >= 2l, "the batch's texts ({lens} tokens) take {want} windows of {region} rows")
        let windows0 = moe_gpu_resident_scratch_windows()
        var inscope pooled <- embed_batch(m, texts)
        tt |> equal(moe_gpu_resident_scratch_windows() - windows0, want, "the batch took one scratch window a {region}-row cut")
        var inscope passes <- gpu_cpu_passes()
        tt |> equal(length(passes), 0, "no call passed to the CPU rails ({passes})")
        let worst = batch_vs_single(m, texts, -1l, 0l, embed_window_rows(m))
        tt |> success(worst > 0.9999, "the split batch against its singles: least cosine {worst}")
        let other = batch_vs_single(m, texts, -1l, 0l, embed_window_rows(m), 1)
        tt |> success(other < 0.9999, "the control: each batch vector against another text's single misses the bar ({other})")
        let windows1 = moe_gpu_resident_scratch_windows()
        host_route_off(m, texts[0])
        tt |> equal(moe_gpu_resident_scratch_windows(), windows1, "the control: a host-cached text with the resident prefill pinned off takes no scratch window")
        delete m
        moe_gpu_drop_model()
        var cpu <- load_model_(path, QuantMode.q8)
        moe_gpu_drop_model()   //! the device tier arms each load: dropped, the model reads on the CPU rails
        tt |> equal(embed_window_rows(cpu), EMBED_WINDOW_ROWS, "an unserved model's window is EMBED_WINDOW_ROWS")
        delete lens
        delete texts
        delete cpu
        restore_levers(was)
    }
}

[test]
def test_embeddings_every_region_held(t : T?) {
    t |> run("with every region held by a live device-home session the embedding takes host-cached sessions, not the scratch window, and reads the vectors the free region gives (0.6B Q8_0)") <| @(tt : T?) {
        let path = path_join(models_dir(), "Qwen3-Embedding-0.6B-Q8_0.gguf")
        return if (!jit_model_available(tt, path))
        let was = device_levers(0l)
        var m = Model()
        if (!served_load(tt, path, m)) {
            restore_levers(was)
            return
        }
        var texts <- [for (d in EMB_DOCS); d]   //! two texts: with no region free each takes a host cache of the model's context
        var inscope free_v <- embed_batch(m, texts)
        tt |> equal(embed_pool_device_sessions(), length(texts), "the control: with a region free the batch rides device-home sessions")
        var held : array<Session>
        held |> reserve(gpu_device_sessions())
        for (_ in range64(gpu_device_sessions())) {
            held |> emplace(create_device_session(m, gpu_device_session_dtype()))
            var inscope ids <- encode(m, EMB_DOCS[1])
            eval(m, held[length(held) - 1], ids)
        }
        tt |> equal(gpu_free_regions(), 0l, "every region holds a live stream")
        let scratch0 = moe_gpu_resident_scratch_windows()
        var inscope held_v <- embed_batch(m, texts)
        tt |> equal(moe_gpu_resident_scratch_windows(), scratch0, "no scratch window with every region held")
        tt |> equal(embed_pool_device_sessions(), 0, "... the texts took host-cached sessions")
        var worst = 1.0
        for (a, b in free_v, held_v) {
            let c = cosine(a, b)
            worst = is_nan(c) ? -1.0 : min(worst, c)
        }
        tt |> success(worst > EMB_MEAN_BAR, "the held run reads the free run's vectors (least cosine {worst})")
        tt |> success(cosine(free_v[0], held_v[1]) < EMB_MEAN_BAR, "the control: another text's vector misses the bar ({cosine(free_v[0], held_v[1])})")
        for (h in held) {
            gpu_device_kv_park(h)   //! a finished device-home stream lets go of its region
        }
        delete held
        tt |> equal(gpu_free_regions(), gpu_device_sessions(), "the streams' end frees their regions")
        delete texts
        delete m
        moe_gpu_drop_model()
        restore_levers(was)
    }
}

[test]
def test_embeddings_ple_model_takes_no_scratch(t : T?) {
    t |> run("a per-layer-embedding model with sliding layers on the armed driver is promised no scratch window - the packed seat seats no ring - takes none, and its batch still matches its singles (gemma-4 E2B Q8_0)") <| @(tt : T?) {
        let path = path_join(models_dir(), "gemma-4-E2B-it-Q8_0.gguf")
        return if (!jit_model_available(tt, path))
        let was = device_levers(0l)
        var m = Model()
        if (!served_load(tt, path, m)) {
            restore_levers(was)
            return
        }
        tt |> success(has_ple(m.config) && session_keeps_rows_only(m.config), "the carrier has per-layer embeddings and keeps nothing past its rows")
        tt |> success(!moe_gpu_resident_scratch_serves(m), "no scratch window is promised a model with sliding layers")
        var texts <- [for (d in EMB_DOCS); d]
        let scratch0 = moe_gpu_resident_scratch_windows()
        let worst = batch_vs_single(m, texts, POOLING_MEAN, 0l, EMBED_WINDOW_ROWS)
        tt |> equal(moe_gpu_resident_scratch_windows(), scratch0, "... and none ran")
        tt |> success(worst > 0.9999, "the batch against its singles: least cosine {worst}")
        let other = batch_vs_single(m, texts, POOLING_MEAN, 0l, EMBED_WINDOW_ROWS, 1)
        tt |> success(other < 0.9999, "the control: against another text's single the bar misses ({other})")
        delete texts
        delete m
        moe_gpu_drop_model()
        restore_levers(was)
    }
}

[test]
def test_embeddings_session_rules(t : T?) {
    t |> run("session_keeps_rows_only reads the session state a forward from 0 does not rewrite - recurrent state, a NextN head, the n-gram ring - and nothing else") @(tt : T?) {
        tt |> success(session_keeps_rows_only(Config()), "a plain decoder keeps only its rows")
        tt |> success(!session_keeps_rows_only(Config(recr_mask = 1ul)), "recurrent state outlives the rows")
        tt |> success(!session_keeps_rows_only(Config(n_layer_nextn = 1l)), "so does a NextN head's row")
        tt |> success(!session_keeps_rows_only(Config(ple_layer = 0l)), "so does the per-layer-embedding n-gram ring")
        tt |> success(session_keeps_rows_only(Config(n_embd_per_layer = 256l)), "per-layer embeddings alone are per row")
        tt |> success(session_keeps_rows_only(Config(hyper_conn = true)), "hyper-connection streams are per row")
    }
    t |> run("the embed pool's key tells two loaded models apart by their planes - a mapped or trimmed model's empty fblob included") @(tt : T?) {
        var a = Model()
        var b = Model()
        a.qblob |> resize(64)
        b.qblob |> resize(64)
        tt |> success(embed_pool_key(a, KVDtype.f32, false) != embed_pool_key(b, KVDtype.f32, false), "two models of one geometry, no fblob: two keys")
        tt |> equal(embed_pool_key(a, KVDtype.f32, false), embed_pool_key(a, KVDtype.f32, false), "the control: one model reads one key")
        delete a
        delete b
    }
}

[test]
def test_embeddings_batch_fresh_sessions(t : T?) {
    t |> run("a model whose session carries state past its rows (Qwen3.5 deltanet) takes fresh sessions: batch matches single; a pooled model of another geometry between two reads leaves the second read right (Qwen3.5 0.8B, tinyllama, 0.6B Q8_0)") <| @(tt : T?) {
        let path = path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf")
        let emb_path = path_join(models_dir(), "Qwen3-Embedding-0.6B-Q8_0.gguf")
        let other_path = path_join(models_dir(), "tinyllama-1.1b-chat-v1.0.Q8_0.gguf")
        return if (!jit_model_available(tt, path) || !jit_model_available(tt, emb_path) || !jit_model_available(tt, other_path))
        var emb <- load_model_(emb_path, QuantMode.q8)
        moe_gpu_drop_model()   //! a device tier arms the next load: the embedding model reads on the CPU both times, so the two reads compare
        var inscope before <- embed(emb, EMB_DOCS[0])
        var m <- load_model_(path, QuantMode.q8)
        tt |> success(!session_keeps_rows_only(m.config), "the carrier keeps recurrent state past its rows - the fresh-session arm")
        var texts <- batch_texts()
        let fresh_builds = embed_pool_builds()
        let worst = batch_vs_single(m, texts, POOLING_MEAN, 0l, EMBED_WINDOW_ROWS)
        tt |> equal(embed_pool_builds(), fresh_builds, "the recurrent model's batch and singles took no pooled session - the pool, keyed to the embedding model, was never rebuilt")
        tt |> success(worst > 0.9999, "the recurrent model's batch against its singles: least cosine {worst}")
        let shifted = batch_vs_single(m, texts, POOLING_MEAN, 0l, EMBED_WINDOW_ROWS, 1)
        tt |> success(shifted < 0.9999, "the control: against another text's single the bar misses ({shifted})")
        var other <- load_model_(other_path, QuantMode.q8)
        let builds = embed_pool_builds()
        var inscope other_v <- embed(other, EMB_DOCS[0])
        tt |> equal(builds + 1, embed_pool_builds(), "another pooled model's geometry rebuilds the pool")
        var inscope other_again <- embed(other, EMB_DOCS[1])
        tt |> equal(builds + 1, embed_pool_builds(), "the control: the same model's next call keeps the pool")
        tt |> equal(other.config.dim, long_length(other_v), "tinyllama reads its own width")
        delete other
        var inscope after <- embed(emb, EMB_DOCS[0])
        tt |> equal(builds + 2, embed_pool_builds(), "... and the embedding model's next call rebuilds it back")
        tt |> success(cosine(before, after) > 0.99999, "the embedding model's pooled session reads the same vector after another model ran ({cosine(before, after)})")
        var inscope after_other <- embed(emb, EMB_DOCS[1])
        tt |> success(cosine(before, after_other) < 0.99999, "the control: another text's read misses the bar ({cosine(before, after_other)})")
        delete texts
        delete m
        delete emb
    }
}
