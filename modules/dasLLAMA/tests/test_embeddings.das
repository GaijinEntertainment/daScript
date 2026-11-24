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
require _model_tier
require _cpu_gpu_model   // own_load: the fresh-session cell's loads
require _embedding_cells   // the embedding cells on a loaded model, and the card's example they share

// The embedding rules no model file carries, and the one cell that holds several models at once. An embedding file names its
// pooling (`{arch}.pooling_type`) and ends every text with EOS (`add_eos_token`); a chat model names neither and is read by the
// mean. Each model file's embedding cells run on its block's forms (`_embedding_cells.das`, the Qwen3-Embedding Metal cell
// among them in `test_gpu_resident_qwen3.das`); the fresh-session cell's loads are its own. It skips off the JIT and without
// its carriers.

[test(description = "`session_keeps_rows_only` is false exactly for recurrent state, a NextN head and the per-layer n-gram ring")]
def test_embeddings_session_rules(t : T?) {
    t |> run("session_keeps_rows_only reads the session state a forward from 0 does not rewrite - recurrent state, a NextN head, the n-gram ring - and nothing else") @(tt : T?) {
        tt |> success(session_keeps_rows_only(Config()), "a plain decoder keeps only its rows")
        tt |> success(!session_keeps_rows_only(Config(recr_mask = 1ul)), "recurrent state outlives the rows")
        tt |> success(!session_keeps_rows_only(Config(n_layer_nextn = 1l)), "so does a NextN head's row")
        tt |> success(!session_keeps_rows_only(Config(ple_layer = 0l)), "so does the per-layer-embedding n-gram ring")
        tt |> success(session_keeps_rows_only(Config(n_embd_per_layer = 256l)), "per-layer embeddings alone are per row")
        tt |> success(session_keeps_rows_only(Config(hyper_conn = true)), "hyper-connection streams are per row")
    }
}

[test(description = "`embed_pool_key` tells two same-geometry models with empty fblobs apart while one model reads one key")]
def test_embeddings_pool_key(t : T?) {
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
