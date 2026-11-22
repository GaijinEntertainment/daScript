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
require dasllama/dasllama_image   // load_model_uncached: the product's load with no image
require daslib/fio   // nolint:STYLE030,LINT019 - path_join, inside the model-gated cells
require ?das_metal dasllama/dasllama_metal_prefill   // metal_prefill_window_forms: the hidden-only window's witness
require _metal_blob_twin   // blob_twin: the Metal cell's blob-flavor model, minted in memory
require math   // nolint:STYLE030,LINT019 - read only inside the das_metal static_if half
require _model_tier
require _cpu_gpu_model   // with_cpu_and_gpu_model, cpu_cells_form: the one-load blocks
require _embedding_cells   // the embedding cells on a loaded model, and the card's example they share

// The embeddings read per model: an embedding file names its pooling (`{arch}.pooling_type`) and ends
// every text with EOS (`add_eos_token`), and the vector it was trained to be read at is the last row;
// a chat model names neither and is read by the mean. Qwen3-Embedding-0.6B's own model-card example is
// the oracle: its published cosine matrix over two queries and two passages, which the last-token read
// lands within 0.02 on the Q8_0 file while the mean read does not. Stocked suite; the cells skip off the JIT and without
// their carrier, the Metal cell on a build without dasMetal, and the Vulkan device cells with no armed Vulkan tier or where
// the whole-model driver declines the model (its decline line says why). Every load is in memory: no `.dlim` is minted.
// Each CPU model loads once a form (`with_cpu_and_gpu_model`); the cells' bodies and the card's example are
// `_embedding_cells.das`, the Metal cell's and the fresh-session cell's loads their own.

let private EMB_F32_BAR = 0.9999   //! the Metal window's vector against the CPU f32 model's, one row or many (reads 0.9999991 to 1.0 on the M5 Max)
let private EMB_FILE_CTX = 32768l   //! Qwen3-Embedding's own context: its CPU cells read at it
let private TINYLLAMA_CTX = 2048l   //! tinyllama's own context: its cells read at it, the past-the-context cell cutting it

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

[test(description = "On Metal, Qwen3-Embedding 0.6B's embed rides one prefill window with no classifier and no decode mirror (a chat forward does both), takes split-K on q8 sites, matches the card matrix within 0.02, and its one-row vector is the CPU f32 model's")]
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

[test(description = "`session_keeps_rows_only` is false exactly for recurrent state, a NextN head and the per-layer n-gram ring, and `embed_pool_key` tells two same-geometry models with empty fblobs apart while one model reads one key")]
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

[test(description = "A Qwen3.5 deltanet batch takes fresh sessions without rebuilding the embed pool and matches its singles, and a pooled model of another geometry rebuilds the pool once while the embedding model's next read returns its earlier vector")]
def test_embeddings_batch_fresh_sessions(t : T?) {
    own_load("the cell holds several models loaded at once: the embedding model stays loaded across a Qwen3.5 load and a tinyllama load, its pool keyed to it, so its second read checks the pool rebuilt for another geometry")
    t |> run("a model whose session carries state past its rows (Qwen3.5 deltanet) takes fresh sessions: batch matches single; a pooled model of another geometry between two reads leaves the second read right (Qwen3.5 0.8B, tinyllama, 0.6B Q8_0)") <| @(tt : T?) {
        let path = path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf")
        let emb_path = path_join(models_dir(), "Qwen3-Embedding-0.6B-Q8_0.gguf")
        let other_path = path_join(models_dir(), "tinyllama-1.1b-chat-v1.0.Q8_0.gguf")
        return if (!jit_model_available(tt, path) || !jit_model_available(tt, emb_path) || !jit_model_available(tt, other_path))
        var emb <- load_model_(emb_path, QuantMode.q8)
        moe_gpu_drop_model()   //! a device tier arms the next load: the embedding model reads on the CPU both times, so the two reads compare
        var inscope before <- embed(emb, EMB_DOCS[0])
        var m <- load_model_uncached(path, QuantMode.q8)
        tt |> success(!session_keeps_rows_only(m.config), "the carrier keeps recurrent state past its rows - the fresh-session arm")
        var texts <- batch_texts()
        let fresh_builds = embed_pool_builds()
        let worst = batch_vs_single(m, texts, POOLING_MEAN, 0l, EMBED_WINDOW_ROWS)
        tt |> equal(embed_pool_builds(), fresh_builds, "the recurrent model's batch and singles took no pooled session - the pool, keyed to the embedding model, was never rebuilt")
        tt |> success(worst > 0.9999, "the recurrent model's batch against its singles: least cosine {worst}")
        let shifted = batch_vs_single(m, texts, POOLING_MEAN, 0l, EMBED_WINDOW_ROWS, 1)
        tt |> success(shifted < 0.9999, "the control: against another text's single the bar misses ({shifted})")
        var other <- load_model_uncached(other_path, QuantMode.q8)
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
