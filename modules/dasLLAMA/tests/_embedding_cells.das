options gen2
options _dasllama_internal = true

module _embedding_cells shared public

//! the embedding reads per model, on the load its model's block makes; no entry loads a model

require dastest/testing_boost public
require dasllama/dasllama   // the facade's embed / embed_batch / encode
require dasllama/dasllama_common   // POOLING_* and the Model the cells read
require dasllama/dasllama_blocks   // embed_batch_ and EMBED_WINDOW_ROWS: the batch cells' window split
require dasllama/dasllama_gpu_tier   // moe_gpu_bake_tag: the device cells' armed-tier gate
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active / gpu_resident_prefills_: the device cells' served witness
require daslib/strings_boost
require _cpu_gpu_model   // with_ctx_cut: the past-the-context cell's cut
require math

// The embeddings read per model, on the loads their models' blocks make: an embedding file names its pooling
// (`{arch}.pooling_type`) and ends every text with EOS (`add_eos_token`), and the vector it was trained to be read at is the
// last row; a chat model names neither and is read by the mean. Qwen3-Embedding-0.6B's own model-card example is the oracle:
// its published cosine matrix over two queries and two passages, which the last-token read lands within 0.02 on the Q8_0 file
// while the mean read does not. The CPU cells run on Qwen3-Embedding (q8 uncached, q8, q4_0) and tinyllama (q8 uncached); the
// device cells on a load the whole-model Vulkan or CUDA driver serves (Qwen3-Embedding, gemma-3-1b, gemma-4 E2B), each skipping
// where no tier is armed or the driver declines the model.

//! Qwen3-Embedding's model-card example: the instruction-prefixed queries, the two passages, and the
//! published cosine matrix for the 0.6B model (rows = queries, columns = passages)
let EMB_TASK = "Instruct: Given a web search query, retrieve relevant passages that answer the query\nQuery: "
let EMB_QUERIES = fixed_array("What is the capital of China?", "Explain gravity")
let EMB_DOCS = fixed_array("The capital of China is Beijing.",
    "Gravity is a force that attracts two bodies towards each other. It gives weight to physical objects and is responsible for the movement of planets around the sun.")
let EMB_REF = fixed_array(0.7646, 0.1414, 0.1355, 0.6000)
let EMB_MEAN_BAR = 0.999   //! the device's mean read (f16 mirror, GPU sums) against the CPU's (f32 cache) on one text
let EMB_BAR = 0.02 //! the Q8_0 file against the card's bf16 numbers (reads 0.005 at most on the M1 Max)
let EMB_REGION_ROWS = 256l   //! the window-rows cell's region cap: its floor, far under EMBED_WINDOW_ROWS, so the two bounds differ
let private PAST_CONTEXT_SEQ = 256l   //! the context the past-the-context cell cuts to: the cut is what it reads, at any context

def cosine(a, b : array<float>) : float {
    var s = 0.0
    for (x, y in a, b) {
        s += x * y
    }
    return s   // both vectors are unit length by the facade's contract
}

//! the four cosines of the card's example under one pooling, row-major
def cosine_matrix(m : Model; pooling : int64) : array<float> {
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
def card_misses(got : array<float>) : int {
    var misses = 0
    for (c, ref in got, EMB_REF) {
        misses += !(abs(c - ref) <= EMB_BAR) ? 1 : 0
    }
    return misses
}

//! the least cosine between each text's batch vector and the single `embed` vector of the text `shift` places on - 0 its own,
//! 1 the control (another text's) - with -1 for a count or width mismatch and for a NaN cosine
def batch_vs_single(m : Model; texts : array<string>; pooling, dims, window_rows : int64; shift : int = 0) : float {
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
def batch_texts() : array<string> {
    var texts <- [for (q in EMB_QUERIES); "{EMB_TASK}{q}"]
    texts |> push_from([for (d in EMB_DOCS); d])
    texts |> push("x")
    texts |> push("Turn on the kitchen lights.")
    texts |> push("What will the weather be tomorrow morning, and should I take an umbrella when I go out?")
    return <- texts
}

//! Qwen3-Embedding `m` (q8, uncached) reads last-token pooling and an appended EOS off the file, and its last-token read lands
//! the card's matrix within EMB_BAR while the mean read cannot separate the passages
def embeddings_card_cell(t : T?; m : Model) {
    unsafe {
        t |> run("Qwen3-Embedding reads its pooling and its EOS off the file, and lands the model card's cosine matrix (0.6B Q8_0)") @capture(& m)(tt : T ?) {
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
        }
    }
}

//! `embed_batch` on Qwen3-Embedding `m` (q8, uncached) gives each text its single vector in one window, in 40-row split windows
//! and under the 256-dim Matryoshka cut, in input order
def embeddings_batch_matches_single_cell(t : T?; m : Model) {
    unsafe {
        t |> run("embed_batch gives each text the vector embed gives it alone, in input order - one window, split windows, the Matryoshka cut (0.6B Q8_0)") @capture(& m)(tt : T ?) {
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
        }
    }
}

//! in a host batch on Qwen3-Embedding `m` (q8, its device state dropped) exactly the one-row text runs `embed_forward` alone
//! while multi-row texts share the window, each landing its single vector, a shifted-text control missing
def embeddings_lone_forwards_cell(t : T?; m : Model) {
    unsafe {
        t |> run("a batch's one-row text runs embed_forward alone, outside the shared window, and lands the vector embed gives it (0.6B Q8_0)") @capture(& m)(tt : T ?) {
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
            delete texts
        }
    }
}

//! every text of Qwen3-Embedding `q4` (loaded as q4_0, its device state dropped) runs `embed_forward` alone and lands its single
//! vector, a shifted-text control missing
def embeddings_lone_forwards_q4_0_cell(t : T?; q4 : Model) {
    unsafe {
        t |> run("every text of a q4_0 model runs embed_forward alone, outside the shared window, and lands the vector embed gives it (0.6B Q8_0 loaded as q4_0)") @capture(& q4)(tt : T ?) {
            moe_gpu_drop_model()
            var texts <- ["", EMB_DOCS[0], EMB_DOCS[1]]
            tt |> equal(q4.quant, QuantMode.q4_0, "the model serves its q4_0 planes")
            let lone2 = embed_lone_forwards()
            let q4_batch = batch_vs_single(q4, texts, -1l, 0l, EMBED_WINDOW_ROWS)
            tt |> equal(embed_lone_forwards() - lone2, length(texts), "every text of the q4_0 batch ran embed_forward alone")
            tt |> success(q4_batch > 0.9999, "the q4_0 model's batch against its singles: the least cosine is {q4_batch}")
            let q4_ctrl = batch_vs_single(q4, texts, -1l, 0l, EMBED_WINDOW_ROWS, 1)
            tt |> success(q4_ctrl < 0.9999, "the control: against another text's single the bar misses ({q4_ctrl})")
            delete texts
        }
    }
}

//! on tinyllama `m` (q8, uncached), its context cut to PAST_CONTEXT_SEQ for the cell, a text running past the context embeds to
//! a finite unit vector at cosine above 0.9999 to its leading seq_len tokens, the overflow text alone elsewhere
def embeddings_past_the_context_cell(t : T?; var m : Model) {
    with_ctx_cut(m, PAST_CONTEXT_SEQ) {
        unsafe {
            t |> run("a text past the model's context embeds its leading seq_len tokens: a unit vector, no panic (tinyllama Q8)") @capture(& m)(tt : T ?) {
                past_the_context_rows(tt, m)
            }
        }
    }
}

def private past_the_context_rows(tt : T?; m : Model) {
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
        finite = finite && !is_nan(v)
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
}

//! tinyllama `m` (q8, uncached), a chat model file naming no pooling, defaults to mean pooling float for float, appends no EOS,
//! and differs from both the last-token and the first-row reads
def embeddings_chat_model_default_mean_cell(t : T?; m : Model) {
    unsafe {
        t |> run("a chat model's file names no pooling: the mean is the default, the last row an override (tinyllama Q8)") @capture(& m)(tt : T ?) {
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
        }
    }
}

//! whether the whole-model driver serves the last load, or a skip naming why not: no armed tier, or the driver declined the
//! model (its device state dropped)
def private served(tt : T?) : bool {
    if (empty(moe_gpu_bake_tag())) {
        tt |> skip("no armed vulkan tier (DASLLAMA_GPU=1 on a box with a device)")
        return false
    }
    if (!moe_gpu_resident_active()) {
        moe_gpu_drop_model()
        tt |> skip("the whole-model driver did not take the model on this box (the decline line above)")
        return false
    }
    return true
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

//! the long text the resident-driver cell's mean read takes, past the last layer's 32-row FFN tail
def private resident_long_text() : string => "{EMB_DOCS[1]} {EMB_DOCS[1]}"

//! the whole-model driver serving Qwen3-Embedding `m` (q8) whole from the device: the card matrix within EMB_BAR, a batch packed
//! in one scratch window matching its singles, no call passed to the CPU rails; answers the device's mean read of the long text
//! (`embeddings_resident_mean_within` holds it to the CPU's), empty where the cell skipped
def embeddings_on_the_resident_driver_cell(t : T?; var m : Model) : array<float> {
    var dev_mean : array<float>
    unsafe {
        t |> run("on an armed Vulkan tier the driver serves Qwen3-Embedding whole from the device: embed and embed_batch prefill on it with no call passed to the CPU rails, and land the card's matrix (0.6B Q8_0)") @capture(& m, & dev_mean)(tt : T ?) {
            return if (!served(tt))
            resident_driver_rows(tt, m)
            resident_many_rows(tt, m)
            //! a mean read past the last layer's 32-row FFN tail: every row the window lands must have run the whole stack
            let long_text = resident_long_text()
            var inscope long_ids <- encode(m, long_text)
            tt |> success(length(long_ids) > 32, "the mean read's text runs past the last layer's 32-row tail ({length(long_ids)} tokens)")
            dev_mean <- embed(m, long_text, POOLING_MEAN)
            var inscope passes <- gpu_cpu_passes()
            tt |> equal(length(passes), 0, "no call passed to the CPU rails ({passes})")
            let served2 = gpu_resident_prefills_()
            let scratch2 = moe_gpu_resident_scratch_windows()
            host_route_off(m, EMB_DOCS[0])
            tt |> equal(gpu_resident_prefills_(), served2, "the control: a host-cached text with the resident prefill pinned off prefills nothing on the device")
            tt |> equal(moe_gpu_resident_scratch_windows(), scratch2, "... and takes no scratch window")
        }
    }
    return <- dev_mean
}

def private resident_driver_rows(tt : T?; m : Model) {
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
    delete texts
}

def private resident_many_rows(tt : T?; m : Model) {
    // more texts than the N-row command's rows (RDEC_NROW_MAX): a hidden window seats every segment, the logits slots bound nothing
    var many <- [for (i in range(int(RDEC_NROW_MAX) + 3)); "{EMB_DOCS[i % length(EMB_DOCS)]} ({i})"]
    let served_many = gpu_resident_prefills_()
    let scratch_many = moe_gpu_resident_scratch_windows()
    var inscope pooled_many <- embed_batch(m, many)
    tt |> equal(moe_gpu_resident_scratch_windows() - scratch_many, 1l, "{length(many)} texts - more than the {RDEC_NROW_MAX} rows a token command steps - sit in one packed window")
    tt |> equal(gpu_resident_prefills_() - served_many, long_length(many), "... every text a prefill on the device")
    let worst_many = batch_vs_single(m, many, -1l, 0l, EMBED_WINDOW_ROWS)
    tt |> success(worst_many > 0.9999, "... the batch against its singles: least cosine {worst_many}")
    delete many
}

//! the CPU's mean reads the resident-driver cell's device read is held to: the long text's and another text's
struct EmbMeanRows {
    mean : array<float>
    other : array<float>
}

//! the resident-driver cell's CPU twin's reads on Qwen3-Embedding `cpu` (q8, its device state dropped), collected for
//! `embeddings_resident_mean_within` after the block
def embeddings_resident_mean_rows(cpu : Model) : EmbMeanRows {
    moe_gpu_drop_model()   //! the device tier arms each load: dropped, the model reads on the CPU rails
    return <- EmbMeanRows(mean <- embed_(cpu, resident_long_text(), POOLING_MEAN, 0l, KVDtype.f32),
        other <- embed_(cpu, EMB_DOCS[0], POOLING_MEAN, 0l, KVDtype.f32))
}

//! the device's mean read `dev_mean` of the long text is the CPU's (`rows`) within EMB_MEAN_BAR, another text's CPU mean read
//! misses it
def embeddings_resident_mean_within(t : T?; dev_mean : array<float>; rows : EmbMeanRows) {
    t |> success(cosine(dev_mean, rows.mean) > EMB_MEAN_BAR, "the device's mean read is the CPU's ({cosine(dev_mean, rows.mean)})")
    t |> success(cosine(dev_mean, rows.other) < EMB_MEAN_BAR, "the control: against another text's CPU mean read it misses the bar ({cosine(dev_mean, rows.other)})")
}

//! the packed window's seat on gemma-3-1b `m` (q8, served), a model with sliding layers: neither discrete home seats segments
//! back to back over a ring layer (`cud_rdec_segs_packed`, its Vulkan twin), so a batch's texts take a window each, no packed
//! window is counted, and the batch's vectors are still the singles'; the packed form's own witness is the resident-driver cell
def embeddings_ring_layers_cell(t : T?; var m : Model) {
    unsafe {
        t |> run("gemma-3-1b Q8_0 on the whole-model driver: the packed seat declines a sliding layer's ring on either home, so the batch's texts take a window each and still match their singles") @capture(& m)(tt : T ?) {
            return if (!served(tt))
            var texts <- batch_texts()
            let n = long_length(texts)
            let served1 = gpu_resident_prefills_()
            let scratch1 = moe_gpu_resident_scratch_windows()
            var inscope pooled <- embed_batch(m, texts)
            tt |> equal(gpu_resident_prefills_() - served1, n, "every text a prefill on the device")
            let worst = batch_vs_single(m, texts, -1l, 0l, EMBED_WINDOW_ROWS)
            tt |> equal(rdec_segs_packed(), false, "the {gpu_want_cuda() ? "CUDA" : "Vulkan"} home seats no segments back to back over a sliding layer's ring")
            tt |> equal(moe_gpu_resident_scratch_windows() - scratch1, 0l, "... so the batch took no packed window: a window a text")
            tt |> success(worst > 0.9999, "the batch against its singles on this home: least cosine {worst}")
            delete texts
        }
    }
}

//! `embed_window_rows` on Qwen3-Embedding `m` (q8, served under a region capped at EMB_REGION_ROWS) is the region's context, so
//! a batch past it takes the predicted count of scratch windows and matches its singles; true where the cell ran past its served
//! gate (`embeddings_unserved_window_rows` holds the unserved model's bound)
def embeddings_window_rows_cell(t : T?; var m : Model) : bool {
    var ran = false
    unsafe {
        t |> run("embed_window_rows: the armed driver's scratch region bounds a batch window where it serves the model - a batch past the region splits into windows and still matches its singles - and EMBED_WINDOW_ROWS bounds it elsewhere (0.6B Q8_0)") @capture(& m, & ran)(tt : T ?) {
            return if (!served(tt))
            ran = true
            let region = EMB_REGION_ROWS
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
            delete lens
            delete texts
        }
    }
    return ran
}

//! the window-rows cell's CPU twin on Qwen3-Embedding `cpu` (q8, its device state dropped): an unserved model's window is
//! EMBED_WINDOW_ROWS
def embeddings_unserved_window_rows(t : T?; cpu : Model) {
    moe_gpu_drop_model()   //! the device tier arms each load: dropped, the model reads on the CPU rails
    t |> equal(embed_window_rows(cpu), EMBED_WINDOW_ROWS, "an unserved model's window is EMBED_WINDOW_ROWS")
}

//! with every device region of the driver serving Qwen3-Embedding `m` (q8) held by a live session, `embed_batch` takes
//! host-cached sessions and no scratch window yet reads the free region's vectors, and parking the streams frees every region
def embeddings_every_region_held_cell(t : T?; var m : Model) {
    unsafe {
        t |> run("with every region held by a live device-home session the embedding takes host-cached sessions, not the scratch window, and reads the vectors the free region gives (0.6B Q8_0)") @capture(& m)(tt : T ?) {
            return if (!served(tt))
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
        }
    }
}

//! on the driver serving gemma-4 E2B `m` (q8), with per-layer embeddings and sliding layers, no scratch window is promised or
//! run, and its mean-pooled batch still matches its singles
def embeddings_ple_no_scratch_cell(t : T?; var m : Model) {
    unsafe {
        t |> run("a per-layer-embedding model with sliding layers on the armed driver is promised no scratch window - the packed seat seats no ring - takes none, and its batch still matches its singles (gemma-4 E2B Q8_0)") @capture(& m)(tt : T ?) {
            return if (!served(tt))
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
        }
    }
}
