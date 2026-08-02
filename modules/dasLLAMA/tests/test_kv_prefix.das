options gen2
options persistent_heap   // several Sessions live at once (see test_forward.das)
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_prefix
require dasllama/dasllama_math   // setup_dasllama_jobque_ + target_chunk_work
require daslib/jobque_boost
require daslib/fio

// Prefix cache (S5) — accounting, the attach cap, LRU budget, and token verification on a tiny
// synthetic Config (no model files, never skips); then warm == cold BIT-EXACT reuse on stories15M
// (classic prefill is chunk-invariant, so attached pages + a tail eval reproduce the cold logits
// byte for byte).

def private mk_config(layers, heads, kv_heads, hs, ctx : int64) : Config {
    return Config(dim = heads * hs, hidden_dim = 32l, n_layers = layers, n_heads = heads,
        n_kv_heads = kv_heads, vocab_size = 8l, seq_len = ctx, head_size = hs,
        kv_dim = kv_heads * hs, shared_weights = true, rope_freq_base = 10000.0)
}

[test]
def test_prefix_accounting(t : T?) {
    t |> run("insert holds pages past session release; attach shares them; release returns all") @(t : T?) {
        let c = mk_config(2l, 4l, 2l, 16l, 32l)
        var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
        var cache <- create_prefix_cache_()
        var s <- make_run_state_paged(c, pool)
        kv_ensure(s, 8l)          // 2 pages at P=4
        s.n_past = 8l             // structural test: pretend both pages are evaled
        var toks <- [for (i in range64(8l)); 100l + i]
        prefix_insert_(cache, pool, s, toks)
        t |> equal(2l, prefix_held_groups_(cache))
        let g0 = s.block_table[0]
        let g1 = s.block_table[1]
        t |> equal(2, pool.refcnt[g0])   // session + cache
        release_kv_pages_(s)
        t |> equal(1, pool.refcnt[g0])   // the cache keeps donated pages alive
        t |> equal(1, pool.refcnt[g1])
        // np == 8 -> pages_max = (8-1)/4 = 1: the cap leaves a tail to eval for logits
        var w <- make_run_state_paged(c, pool)
        t |> equal(4l, prefix_attach_(cache, pool, w, toks))
        t |> equal(4l, w.n_past)
        t |> equal(1, length(w.block_table))
        t |> equal(g0, w.block_table[0])
        t |> equal(2, pool.refcnt[g0])
        // a longer prompt with the same 8-token prefix attaches BOTH pages
        var toks12 <- [for (i in range64(12l)); i < 8l ? 100l + i : 900l + i]
        var w2 <- make_run_state_paged(c, pool)
        t |> equal(8l, prefix_attach_(cache, pool, w2, toks12))
        t |> equal(3, pool.refcnt[g0])
        // divergence in page 0 -> no match, nothing attached
        var other <- [for (i in range64(8l)); 500l + i]
        var w3 <- make_run_state_paged(c, pool)
        t |> equal(0l, prefix_attach_(cache, pool, w3, other))
        t |> equal(0, length(w3.block_table))
        t |> equal(0l, w3.n_past)
        release_kv_pages_(w)
        release_kv_pages_(w2)
        prefix_release_(cache, pool)
        t |> equal(long_length(pool.free_groups), pool.capacity_groups)   // accounting to baseline
        var live = 0
        for (g in range64(pool.capacity_groups)) {
            live += pool.refcnt[g]
        }
        t |> equal(0, live)
        delete other
        delete toks12
        delete toks
        delete w
        delete w2
        delete w3
        delete s
        delete cache
        delete pool
    }
}

[test]
def test_prefix_lru_budget(t : T?) {
    t |> run("max_groups LRU-drops the oldest page; a dropped page a session still uses stays alive") @(t : T?) {
        let c = mk_config(2l, 4l, 2l, 16l, 32l)
        var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
        var cache <- create_prefix_cache_(2l)   // hold at most 2 pages
        var s <- make_run_state_paged(c, pool)
        kv_ensure(s, 12l)   // 3 pages
        s.n_past = 12l
        var toks <- [for (i in range64(12l)); 100l + i]
        prefix_insert_(cache, pool, s, toks)   // 3 donated -> the oldest (page 0) LRU-drops
        t |> equal(2l, prefix_held_groups_(cache))
        t |> equal(1l, cache.n_evicted_pages)
        t |> equal(1, pool.refcnt[s.block_table[0]])   // dropped: only the session holds it
        t |> equal(2, pool.refcnt[s.block_table[1]])
        // page 0 is gone and the chain starts there -> attach misses at once
        var w <- make_run_state_paged(c, pool)
        t |> equal(0l, prefix_attach_(cache, pool, w, toks))
        release_kv_pages_(s)
        prefix_release_(cache, pool)
        t |> equal(long_length(pool.free_groups), pool.capacity_groups)
        delete toks
        delete w
        delete s
        delete cache
        delete pool
    }
}

[test]
def test_prefix_verify_tokens(t : T?) {
    t |> run("hashes route, tokens verify: a corrupted entry refuses to attach") @(t : T?) {
        let c = mk_config(2l, 4l, 2l, 16l, 32l)
        var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
        var cache <- create_prefix_cache_()
        var s <- make_run_state_paged(c, pool)
        kv_ensure(s, 8l)
        s.n_past = 8l
        var toks <- [for (i in range64(8l)); 100l + i]
        prefix_insert_(cache, pool, s, toks)
        for (e in values(cache.entries)) {   // simulate a hash collision: key routes, tokens differ
            e.tokens[0] = 424242l
        }
        var w <- make_run_state_paged(c, pool)
        t |> equal(0l, prefix_attach_(cache, pool, w, toks))
        t |> equal(0, length(w.block_table))
        release_kv_pages_(s)
        prefix_release_(cache, pool)
        delete toks
        delete w
        delete s
        delete cache
        delete pool
    }
}

// ===== warm == cold bit-exactness on a real model (env-gated, JIT-only) =====

def private llama2c_dir() : string {
    let env = get_env_variable("DASLLAMA_LLAMA2C_DIR")
    return empty(env) ? "/Users/borisbatkin/Work/llama2.c" : env
}
let MODEL_PATH = path_join(llama2c_dir(), "stories15M.bin")

def private argmax_of(s : Session; vocab : int64) : int64 {
    var best = 0l
    for (i in range64(1l, vocab)) {
        if (s.logits[i] > s.logits[best]) {
            best = i
        }
    }
    return best
}

def private logits_match(s : Session; all : array<float>; snap, vocab : int64) : bool {
    for (i in range64(vocab)) {
        if (s.logits[i] != all[snap * vocab + i]) {
            return false
        }
    }
    return true
}

[test]
def test_prefix_warm_equals_cold(t : T?) {   // nolint:STYLE038 — cold/warm/divergent phases share one pool + cache and one flat teardown
    t |> run("attached pages + tail eval == cold prefill, bit-exact (needs stories15M.bin)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!stat(MODEL_PATH).is_valid) {
            t |> skip("stories15M.bin not present")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        let c = tr.config
        quantize_weights(tr)
        let prev_thresh = get_target_chunk_work()
        let saved_mode = get_prefill_attn_mode()
        with_job_que() {
            setup_dasllama_jobque_()
            set_target_chunk_work(1l)
            // classic prefill is bit-exact under ANY chunking (test_prefill's contract), so the
            // warm tail eval must reproduce the cold single-shot prefill byte for byte
            set_prefill_attn_mode(AttnPrefillMode.classic)
            var prompt <- [for (i in range64(21l)); 2l + i % 40l]
            let np = long_length(prompt)
            let nsteps = 8l
            var pool <- create_kv_pool_(c, 8l, KVDtype.f32, KVDtype.f32)
            var cache <- create_prefix_cache_()
            // COLD: single-shot prefill + greedy decode, snapshotting every logits vector
            var cold <- make_run_state_paged(c, pool)
            eval_(tr, cold, prompt)
            var cold_all : array<float>
            cold_all |> push_from(cold.logits)
            var cold_toks : array<int64>
            var one : array<int64>
            one |> resize(1)
            var tok = argmax_of(cold, c.vocab_size)
            for (_i in range64(nsteps)) {
                cold_toks |> push(tok)
                one[0] = tok
                eval_(tr, cold, one)
                cold_all |> push_from(cold.logits)
                tok = argmax_of(cold, c.vocab_size)
            }
            // donate: full evaled history = prompt + sampled tokens, floor(29/8) = 3 pages
            var full : array<int64>
            full |> push_from(prompt)
            full |> push_from(cold_toks)
            prefix_insert_(cache, pool, cold, full)
            t |> equal(3l, prefix_held_groups_(cache))
            let g0 = cold.block_table[0]
            let g1 = cold.block_table[1]
            release_kv_pages_(cold)
            // WARM: attach 2 pages ((21-1)/8), classic-eval only the 5-token tail
            var warm <- make_run_state_paged(c, pool)
            t |> equal(16l, prefix_attach_(cache, pool, warm, prompt))
            t |> equal(g0, warm.block_table[0])   // the pages are SHARED, not copied
            t |> equal(g1, warm.block_table[1])
            t |> equal(2, pool.refcnt[g0])        // cache + warm
            var tail <- [for (j in range64(16l, np)); prompt[j]]
            eval_(tr, warm, tail)
            t |> success(logits_match(warm, cold_all, 0l, c.vocab_size), "post-prefill logits bit-exact")
            var wtok = argmax_of(warm, c.vocab_size)
            var bad = 0
            for (i in range64(nsteps)) {
                if (wtok != cold_toks[i]) {
                    bad ++
                }
                one[0] = wtok
                eval_(tr, warm, one)
                if (!logits_match(warm, cold_all, 1l + i, c.vocab_size)) {
                    bad ++
                }
                wtok = argmax_of(warm, c.vocab_size)
            }
            t |> equal(0, bad)
            // DIVERGENT suffix: same 16-token prefix, different tail — warm attach == its own cold
            var prompt2 : array<int64>
            for (i in range64(24l)) {
                prompt2 |> push(i < 16l ? prompt[i] : 3l + i % 30l)
            }
            var cold2 <- make_run_state_paged(c, pool)
            eval_(tr, cold2, prompt2)
            var warm2 <- make_run_state_paged(c, pool)
            t |> equal(16l, prefix_attach_(cache, pool, warm2, prompt2))
            var tail2 <- [for (j in range64(16l, 24l)); prompt2[j]]
            eval_(tr, warm2, tail2)
            var bad2 = 0
            for (i in range64(c.vocab_size)) {
                if (warm2.logits[i] != cold2.logits[i]) {
                    bad2 ++
                }
            }
            t |> equal(0, bad2)
            // teardown: everything returns to the pool
            release_kv_pages_(warm)
            release_kv_pages_(warm2)
            release_kv_pages_(cold2)
            prefix_release_(cache, pool)
            t |> equal(long_length(pool.free_groups), pool.capacity_groups)
            delete tail2
            delete warm2
            delete cold2
            delete prompt2
            delete tail
            delete warm
            delete full
            delete one
            delete cold_toks
            delete cold_all
            delete cold
            delete cache
            delete pool
            delete prompt
        }
        set_prefill_attn_mode(saved_mode)
        set_target_chunk_work(prev_thresh)
        delete tr
    }
}
