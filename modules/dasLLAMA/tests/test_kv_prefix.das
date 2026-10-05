options gen2
options persistent_heap   // several Sessions live at once (see test_forward.das)
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require _model_tier
require dasllama/dasllama_prefix
require dasllama/dasllama_math   // with_dasllama_jobque_ + target_chunk_work
require daslib/jobque_boost
require daslib/fio
require daslib/linq

// Prefix cache (S5) — accounting, the attach cap, LRU budget, token verification, and a recurrent
// session's checkpoints (sharing, the copied page, the budget, where a prefill stops) on a tiny
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
        // np == 8 -> pages_max = (8-1)/4 = 1 whole page shared, then three rows of the second as a COPY: the cap leaves one token to eval for logits
        var w <- make_run_state_paged(c, pool)
        t |> equal(7l, prefix_match_len_(cache, pool, toks), "the probe reads what the attach will attach")
        t |> equal(7l, prefix_attach_(cache, pool, w, toks))
        t |> equal(7l, w.n_past)
        t |> equal(2, length(w.block_table))
        t |> equal(g0, w.block_table[0])
        t |> success(w.block_table[1] != g1, "the partly shared page is a copy")
        t |> equal(2, pool.refcnt[g0])
        t |> equal(1, pool.refcnt[g1])
        t |> equal(1l, cache.n_hit_partial)
        // a longer prompt with the same 8-token prefix attaches BOTH pages
        var toks12 <- [for (i in range64(12l)); i < 8l ? 100l + i : 900l + i]
        var w2 <- make_run_state_paged(c, pool)
        t |> equal(8l, prefix_match_len_(cache, pool, toks12), "the probe reads what the attach will attach")
        t |> equal(8l, prefix_attach_(cache, pool, w2, toks12))
        t |> equal(3, pool.refcnt[g0])
        // divergence in page 0 -> no match, nothing attached
        var other <- [for (i in range64(8l)); 500l + i]
        var w3 <- make_run_state_paged(c, pool)
        t |> equal(0l, prefix_match_len_(cache, pool, other), "the probe reads what the attach will attach")
        t |> equal(0l, prefix_attach_(cache, pool, w3, other))
        t |> equal(0, length(w3.block_table))
        t |> equal(0l, w3.n_past)
        release_kv_pages_(w)
        release_kv_pages_(w2)
        prefix_release_(cache, pool)
        t |> equal(long_length(pool.free_groups), pool.capacity_groups)   // accounting to baseline
        t |> equal(0, live_refs(pool))
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
        t |> equal(0l, prefix_match_len_(cache, pool, toks), "the probe reads what the attach will attach")
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
def test_prefix_tail_edges(t : T?) {
    t |> run("the page past the whole hits: a prompt sharing none of its rows copies nothing, each donation's page past the hit is a candidate, a partial take is an LRU touch") @(t : T?) {
        let c = mk_config(2l, 4l, 2l, 16l, 32l)
        var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
        var cache <- create_prefix_cache_()
        var sa <- make_run_state_paged(c, pool)
        kv_ensure(sa, 12l)
        sa.n_past = 12l
        var ta <- [for (i in range64(12l)); 100l + i]
        prefix_insert_(cache, pool, sa, ta)   // pages A0 A1 A2
        let ga1 = sa.block_table[1]
        let ga2 = sa.block_table[2]
        release_kv_pages_(sa)
        var sb <- make_run_state_paged(c, pool)
        kv_ensure(sb, 12l)
        sb.n_past = 12l
        var tb <- [for (i in range64(12l)); i < 4l ? 100l + i : 700l + i]
        prefix_insert_(cache, pool, sb, tb)   // A0 again, then B1 B2: A0 links A1 and B1
        let gb1 = sb.block_table[1]
        release_kv_pages_(sb)
        t |> equal(5l, prefix_held_groups_(cache))
        // one whole hit, then a first row the page past it does not share: no copy, nothing taken
        var none <- [100l, 101l, 102l, 103l, 999l]
        var w0 <- make_run_state_paged(c, pool)
        t |> equal(4l, prefix_attach_(cache, pool, w0, none))
        t |> equal(1, length(w0.block_table), "a page sharing no row is not copied")
        t |> equal(0l, cache.n_hit_partial)
        // one whole hit, then two rows of A1 - of the two pages past A0, the one the prompt shares rows with
        var two <- [100l, 101l, 102l, 103l, 104l, 105l, 999l, 998l]
        var w1 <- make_run_state_paged(c, pool)
        t |> equal(6l, prefix_match_len_(cache, pool, two), "the probe reads what the attach will attach")
        t |> equal(6l, prefix_attach_(cache, pool, w1, two), "A1 follows A0 and the prompt shares its two rows")
        t |> equal(1l, cache.n_hit_partial)
        t |> success(w1.block_table[1] != ga1 && w1.block_table[1] != gb1, "the partly shared page is a copy")
        // the same hit, then three rows of B1: the second donation's page past A0 serves a prompt that follows it
        var like_b <- [100l, 101l, 102l, 103l, 704l, 705l, 706l, 999l, 998l]
        var w2 <- make_run_state_paged(c, pool)
        t |> equal(7l, prefix_match_len_(cache, pool, like_b), "the probe reads what the attach will attach")
        t |> equal(7l, prefix_attach_(cache, pool, w2, like_b), "B1 follows A0 too, and the prompt shares three of its rows")
        t |> success(w2.block_table[1] != ga1 && w2.block_table[1] != gb1, "and its page is a copy as well")
        release_kv_pages_(w2)
        delete w2
        delete like_b
        // the partial take touched A1: with room for five of the six pages a sixth donation evicts A2, the one left untouched
        cache.max_groups = 5l
        var sc <- make_run_state_paged(c, pool)
        kv_ensure(sc, 4l)
        sc.n_past = 4l
        var tc <- [for (i in range64(4l)); 300l + i]
        prefix_insert_(cache, pool, sc, tc)
        release_kv_pages_(sc)
        t |> equal(1l, cache.n_evicted_pages)
        t |> equal(1, pool.refcnt[ga1], "A1, touched by the partial take, stays held by the cache")
        t |> equal(0, pool.refcnt[ga2], "A2, untouched since its donation, is the page evicted")
        // A1 still links A2, whose page the cache dropped: a prompt running on into it stops at the two whole pages
        var into_gone <- [for (i in range64(11l)); 100l + i]
        var w3 <- make_run_state_paged(c, pool)
        t |> equal(8l, prefix_attach_(cache, pool, w3, into_gone), "a link to a dropped page attaches nothing past the whole hits")
        t |> equal(2, length(w3.block_table))
        release_kv_pages_(w3)
        delete w3
        delete into_gone
        release_kv_pages_(w0)
        release_kv_pages_(w1)
        prefix_release_(cache, pool)
        t |> equal(long_length(pool.free_groups), pool.capacity_groups)
        delete ta
        delete tb
        delete tc
        delete none
        delete two
        delete w0
        delete w1
        delete sa
        delete sb
        delete sc
        delete cache
        delete pool
    }
}

[test]
def test_prefix_first_page_edges(t : T?) {
    t |> run("no whole hit: the first page sharing the most leading rows is copied, a prompt sharing none takes nothing, an evicted first page is no candidate") @(t : T?) {
        let c = mk_config(2l, 4l, 2l, 16l, 32l)
        var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
        var cache <- create_prefix_cache_()
        var sa <- make_run_state_paged(c, pool)
        kv_ensure(sa, 3l)
        sa.n_past = 3l
        var ta <- [100l, 101l, 102l]
        prefix_insert_(cache, pool, sa, ta)   // a first page of three rows
        let ga = sa.block_table[0]
        release_kv_pages_(sa)
        var sb <- make_run_state_paged(c, pool)
        kv_ensure(sb, 8l)
        sb.n_past = 8l
        var tb <- [for (i in range64(8l)); i < 2l ? 100l + i : 500l + i]
        prefix_insert_(cache, pool, sb, tb)   // a whole first page opening on the same two tokens, and a second page
        let gb = sb.block_table[0]
        release_kv_pages_(sb)
        t |> equal(2, length(cache.roots), "each donation's first page is a candidate")
        var like_a <- [100l, 101l, 102l, 900l, 901l]
        var w0 <- make_run_state_paged(c, pool)
        t |> equal(3l, prefix_match_len_(cache, pool, like_a), "the probe reads what the attach will attach")
        t |> equal(3l, prefix_attach_(cache, pool, w0, like_a), "three rows of A's page beat two of B's")
        var chains <- prefix_chain_list_(cache)
        t |> equal(1l, [for (ch in chains); ch.hits] |> sum(), "the chain whose first page was copied is credited the hit")
        delete chains
        t |> success(length(w0.block_table) == 1 && w0.block_table[0] != ga && w0.block_table[0] != gb, "the page is a copy")
        var like_b <- [100l, 101l, 502l, 900l, 901l]
        var w1 <- make_run_state_paged(c, pool)
        t |> equal(3l, prefix_match_len_(cache, pool, like_b), "the probe reads what the attach will attach")
        t |> equal(3l, prefix_attach_(cache, pool, w1, like_b), "three leading rows of B's whole page")
        var same_a <- [100l, 101l, 102l]
        var w2 <- make_run_state_paged(c, pool)
        t |> equal(2l, prefix_match_len_(cache, pool, same_a), "the probe reads what the attach will attach")
        t |> equal(2l, prefix_attach_(cache, pool, w2, same_a), "the same prompt again stops a token short, so the tail eval lands logits")
        var none <- [777l, 100l, 101l]
        var w3 <- make_run_state_paged(c, pool)
        t |> equal(0l, prefix_match_len_(cache, pool, none), "the probe reads what the attach will attach")
        t |> equal(0l, prefix_attach_(cache, pool, w3, none))
        t |> success(empty(w3.block_table), "a prompt sharing no first row takes no page")
        t |> equal(3l, cache.n_hit_partial)
        // B's two pages were touched after A's: a budget of two pages evicts A's first page, and its candidate with it
        cache.tick ++
        cache.entries[cache.roots[1]].last_use = cache.tick
        cache.max_groups = 2l
        var sc <- make_run_state_paged(c, pool)
        kv_ensure(sc, 1l)
        sc.n_past = 1l
        var tc <- [300l]
        prefix_insert_(cache, pool, sc, tc)
        release_kv_pages_(sc)
        t |> success(find_index(cache.roots, 0ul) < 0 && length(cache.roots) == length([for (k in cache.roots); k; where key_exists(cache.entries, k)]), "every candidate is a held page: {cache.roots}")
        for (w in [w0, w1, w2, w3]) {
            release_kv_pages_(w)
        }
        prefix_release_(cache, pool)
        t |> equal(long_length(pool.free_groups), pool.capacity_groups, "every page is back in the pool")
        delete ta
        delete tb
        delete tc
        delete like_a
        delete like_b
        delete same_a
        delete none
        delete w0
        delete w1
        delete w2
        delete w3
        delete sa
        delete sb
        delete sc
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
        t |> equal(0l, prefix_match_len_(cache, pool, toks), "the probe reads what the attach will attach")
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

//! a paged session that reads as recurrent to the cache, standing at `n` evaled rows with its state at the same position
def private mk_recurrent(c : Config; var pool : KVPool; n : int64) : Session {
    var s <- make_run_state_paged(c, pool)
    s.dn_state_prefix |> push(0l)
    s.dn_conv_prefix |> push(0l)
    s.dn_state <- [for (i in range64(8l)); float(n * 10l + i)]   // one recurrent layer's state, a value a position
    s.dn_conv_state <- [for (i in range64(4l)); float(n * 100l + i)]
    if (n > 0l) {
        kv_ensure(s, n)
    }
    s.n_past = n
    s.dn_pos = n
    return <- s
}

def private live_refs(pool : KVPool) : int {
    var live = 0
    for (g in range64(pool.capacity_groups)) {
        live += pool.refcnt[g]
    }
    return live
}

[test]
def test_prefix_checkpoint_accounting(t : T?) {
    t |> run("a recurrent session's donation is one checkpoint: whole pages shared, the page of the last row copied, every page back at release") @(t : T?) {
        let c = mk_config(2l, 4l, 2l, 16l, 32l)
        var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
        var cache <- create_prefix_cache_()
        var s <- mk_recurrent(c, pool, 6l)   // a page and a half at P=4
        var toks <- [for (i in range64(9l)); 100l + i]
        prefix_insert_(cache, pool, s, toks)
        t |> equal(1, length(cache.states), "one checkpoint")
        t |> equal(6l, long_length(cache.states[0].tokens), "it holds the evaled tokens, not the whole list")
        t |> equal(0, length(cache.entries), "a recurrent model donates no page chain")
        t |> equal(2l, prefix_held_groups_(cache), "the cache counts the checkpoint's two pages as held")
        let g0 = s.block_table[0]
        let g1 = s.block_table[1]
        t |> equal(2, pool.refcnt[g0], "the checkpoint holds the whole page")
        t |> equal(2, pool.refcnt[g1], "and the partial one")
        prefix_insert_(cache, pool, s, toks)
        t |> equal(1, length(cache.states), "the same tokens leave no second checkpoint")
        var w <- mk_recurrent(c, pool, 0l)
        t |> equal(0l, prefix_match_len_(cache, pool, toks), "a recurrent cache attaches at checkpoints: the probe answers nothing held")
        t |> equal(6l, prefix_attach_(cache, pool, w, toks), "a prompt past the checkpoint attaches all of it")
        t |> equal(6l, w.n_past)
        t |> equal(6l, w.dn_pos, "the recurrent state stands at the checkpoint")
        t |> success(sequence_equal(w.dn_state, s.dn_state) && sequence_equal(w.dn_conv_state, s.dn_conv_state), "and is the donor's, where the fresh session held its own")
        t |> equal(60.0, w.dn_state[0])
        t |> equal(2, length(w.block_table))
        t |> equal(g0, w.block_table[0], "the whole page is shared")
        t |> success(w.block_table[1] != g1, "the partial page is the session's own copy")
        t |> equal(3, pool.refcnt[g0])
        t |> equal(1, pool.refcnt[w.block_table[1]])
        var same <- [for (i in range64(6l)); 100l + i]
        var w2 <- mk_recurrent(c, pool, 0l)
        t |> equal(0l, prefix_match_len_(cache, pool, same), "the probe reads what the attach will attach")
        t |> equal(0l, prefix_attach_(cache, pool, w2, same), "a prompt the checkpoint covers whole attaches nothing: its last token must be evaled")
        var other <- [for (i in range64(9l)); i < 5l ? 100l + i : 700l + i]
        t |> equal(0l, prefix_match_len_(cache, pool, other), "the probe reads what the attach will attach")
        t |> equal(0l, prefix_attach_(cache, pool, w2, other), "a prompt leaving the checkpoint's tokens early attaches nothing: the state is at one position only")
        release_kv_pages_(s)
        release_kv_pages_(w)
        prefix_release_(cache, pool)
        t |> equal(long_length(pool.free_groups), pool.capacity_groups, "every page is back")
        t |> equal(0, live_refs(pool))
        delete other
        delete same
        delete toks
        delete w
        delete w2
        delete s
        delete cache
        delete pool
    }
    t |> run("a checkpoint on a page boundary still copies the page of its last row; a state off the session's position is not kept") @(t : T?) {
        let c = mk_config(2l, 4l, 2l, 16l, 32l)
        var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
        var cache <- create_prefix_cache_()
        var toks <- [for (i in range64(12l)); 100l + i]
        var off <- mk_recurrent(c, pool, 8l)
        off.dn_pos = 7l
        prefix_insert_(cache, pool, off, toks)
        t |> equal(0, length(cache.states), "a state one position behind its rows leaves no checkpoint")
        off.dn_pos = 8l
        prefix_insert_(cache, pool, off, toks)
        t |> equal(1, length(cache.states))
        var w <- mk_recurrent(c, pool, 0l)
        t |> equal(0l, prefix_match_len_(cache, pool, toks), "a recurrent cache attaches at checkpoints: the probe answers nothing held")
        t |> equal(8l, prefix_attach_(cache, pool, w, toks))
        t |> equal(off.block_table[0], w.block_table[0], "the first page is shared")
        t |> success(w.block_table[1] != off.block_table[1], "the page of row 7 is copied though it is whole: the draft head rewrites that row")
        release_kv_pages_(off)
        release_kv_pages_(w)
        prefix_release_(cache, pool)
        t |> equal(long_length(pool.free_groups), pool.capacity_groups, "every page is back")
        delete toks
        delete w
        delete off
        delete cache
        delete pool
    }
}

[test]
def test_prefix_checkpoint_budget(t : T?) {
    t |> run("max_states drops the checkpoint used longest ago, and a budget of none keeps nothing") @(t : T?) {
        let c = mk_config(2l, 4l, 2l, 16l, 32l)
        var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
        var cache <- create_prefix_cache_(0l, 2l)
        var lists : array<array<int64>>
        lists |> reserve(3)
        for (k in range64(3l)) {
            var s <- mk_recurrent(c, pool, 5l)
            var toks <- [for (i in range64(8l)); 100l * (k + 1l) + i]
            if (k == 2l) {   // the first checkpoint is used again before the third lands, so the second is the oldest
                var w <- mk_recurrent(c, pool, 0l)
                t |> equal(5l, prefix_attach_(cache, pool, w, lists[0]))
                release_kv_pages_(w)
                delete w
            }
            prefix_insert_(cache, pool, s, toks)
            release_kv_pages_(s)
            lists |> emplace(toks)
            delete s
        }
        t |> equal(2, length(cache.states), "two checkpoints stand")
        var hits : array<int64>
        hits |> reserve(3)
        for (toks in lists) {
            var w <- mk_recurrent(c, pool, 0l)
            hits |> push(prefix_attach_(cache, pool, w, toks))
            release_kv_pages_(w)
            delete w
        }
        t |> equal(5l, hits[0], "the checkpoint used since stands")
        t |> equal(0l, hits[1], "the one used longest ago is gone")
        t |> equal(5l, hits[2], "the newest stands")
        prefix_release_(cache, pool)
        t |> equal(long_length(pool.free_groups), pool.capacity_groups, "the dropped checkpoint's pages went back with the rest")
        var none <- create_prefix_cache_(0l, 0l)
        var s <- mk_recurrent(c, pool, 5l)
        prefix_insert_(none, pool, s, lists[0])
        t |> equal(0, length(none.states), "a budget of none keeps no checkpoint")
        release_kv_pages_(s)
        delete s
        delete none
        delete hits
        delete lists
        delete cache
        delete pool
    }
}

[test]
def test_prefix_checkpoint_at(t : T?) {
    t |> run("the prefill stops at the caller's stable opening, else at an opening an earlier checkpoint shares, and nowhere on a model with no recurrent state") @(t : T?) {
        let c = mk_config(2l, 4l, 2l, 16l, 64l)
        var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
        var cache <- create_prefix_cache_()
        var prompt <- [for (i in range64(20l)); 100l + i]
        var s <- mk_recurrent(c, pool, 0l)
        t |> equal(0l, prefix_checkpoint_at_(cache, pool, s, prompt, 0l, 0l), "no hint and no checkpoint: nowhere")
        t |> equal(12l, prefix_checkpoint_at_(cache, pool, s, prompt, 0l, 12l), "the stable opening")
        t |> equal(0l, prefix_checkpoint_at_(cache, pool, s, prompt, 9l, 12l), "under a page past the hit it is not worth a window")
        t |> equal(0l, prefix_checkpoint_at_(cache, pool, s, prompt, 0l, 20l), "an opening that is the whole prompt leaves nothing to eval")
        var plain <- make_run_state_paged(c, pool)
        t |> equal(0l, prefix_checkpoint_at_(cache, pool, plain, prompt, 0l, 12l), "a model with no recurrent state checkpoints nowhere: its pages are the cache")
        var donor <- mk_recurrent(c, pool, 14l)
        var earlier <- [for (i in range64(14l)); i < 10l ? 100l + i : 900l + i]   // shares the prompt's first 10 tokens
        prefix_insert_(cache, pool, donor, earlier)
        t |> equal(10l, prefix_checkpoint_at_(cache, pool, s, prompt, 0l, 0l), "with no hint, the opening the earlier prompt shares")
        t |> equal(12l, prefix_checkpoint_at_(cache, pool, s, prompt, 0l, 12l), "the hint wins over it")
        t |> equal(0l, prefix_checkpoint_at_(cache, pool, s, prompt, 8l, 0l), "the shared opening under a page past the hit is nowhere too")
        var none <- create_prefix_cache_(0l, 0l)
        t |> equal(0l, prefix_checkpoint_at_(none, pool, s, prompt, 0l, 12l), "a cache that keeps no checkpoint asks for none")
        var longer <- mk_recurrent(c, pool, 24l)
        var past <- [for (i in range64(24l)); 100l + i]   // the prompt whole, and four tokens more
        prefix_insert_(cache, pool, longer, past)
        t |> equal(19l, prefix_checkpoint_at_(cache, pool, s, prompt, 0l, 0l), "an earlier prompt that holds this one whole stops it a token short: the last is evaled")
        release_kv_pages_(longer)
        release_kv_pages_(donor)
        prefix_release_(cache, pool)
        delete past
        delete longer
        delete none
        delete earlier
        delete donor
        delete plain
        delete s
        delete prompt
        delete cache
        delete pool
    }
}

def private restore_panics(var s : Session; snap : DnSnapshot) : bool {
    var tripped = false
    try {
        dn_snapshot_restore(s, snap)
    } recover {
        tripped = true
    }
    return tripped
}

[test]
def test_prefix_checkpoint_side_state(t : T?) {
    t |> run("a checkpoint carries the n-gram side input's history with the recurrent state, and a state of another geometry is refused") @(t : T?) {
        let c = mk_config(2l, 4l, 2l, 16l, 32l)
        var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
        var cache <- create_prefix_cache_()
        var s <- mk_recurrent(c, pool, 6l)
        s.ple_prev <- [7l, 8l]
        s.ple_conv_hist <- [for (i in range64(6l)); float(6000l + i)]
        var toks <- [for (i in range64(9l)); 100l + i]
        prefix_insert_(cache, pool, s, toks)
        s.ple_prev[0] = 99l   // the donor moves on; the checkpoint keeps what it took
        s.ple_conv_hist[0] = -1.0
        var w <- mk_recurrent(c, pool, 0l)
        w.ple_prev <- [-1l, -1l]
        w.ple_conv_hist |> reserve_resize(6l)
        t |> equal(0l, prefix_match_len_(cache, pool, toks), "a recurrent cache attaches at checkpoints: the probe answers nothing held")
        t |> equal(6l, prefix_attach_(cache, pool, w, toks))
        t |> success(w.ple_prev[0] == 7l && w.ple_prev[1] == 8l, "the attached session holds the checkpoint's last tokens ({w.ple_prev[0]}, {w.ple_prev[1]})")
        t |> success(w.ple_conv_hist[0] == 6000.0 && w.ple_conv_hist[5] == 6005.0, "and its conv history ({w.ple_conv_hist[0]} .. {w.ple_conv_hist[5]})")
        var snap <- dn_snapshot_take(s)
        var wide <- mk_recurrent(c, pool, 0l)
        wide.dn_state |> reserve_resize(16l)
        t |> success(restore_panics(wide, snap), "a session of another model's state size refuses the snapshot")
        t |> equal(1.0, wide.dn_state[1], "and is left as it stood")
        t |> success(!restore_panics(w, snap), "a session of the snapshot's own geometry takes it")
        release_kv_pages_(s)
        release_kv_pages_(w)
        prefix_release_(cache, pool)
        t |> equal(long_length(pool.free_groups), pool.capacity_groups, "every page is back")
        delete snap
        delete wide
        delete toks
        delete w
        delete s
        delete cache
        delete pool
    }
}

// ===== warm == cold bit-exactness on a real model (env-gated, JIT-only) =====

let MODEL_PATH = path_join(llama2c_dir(), "stories15M.bin")

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
        return if (!jit_only(t))
        if (!stat(MODEL_PATH).is_valid) {
            t |> skip("stories15M.bin not present")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        let c = tr.config
        quantize_weights(tr)
        let prev_thresh = get_target_chunk_work()
        let saved_mode = get_prefill_attn_mode()
        with_dasllama_jobque_() {
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
            var cold_all := cold.logits
            var cold_toks : array<int64>
            var one : array<int64>
            one |> resize(1)
            var tok = argmax(cold, c.vocab_size)
            for (_i in range64(nsteps)) {
                cold_toks |> push(tok)
                one[0] = tok
                eval_(tr, cold, one)
                cold_all |> push_from(cold.logits)
                tok = argmax(cold, c.vocab_size)
            }
            // donate: full evaled history = prompt + sampled tokens, floor(29/8) = 3 pages and a 5-row tail
            var full <- concat(prompt, cold_toks)
            prefix_insert_(cache, pool, cold, full)
            t |> equal(4l, prefix_held_groups_(cache))
            let g0 = cold.block_table[0]
            let g1 = cold.block_table[1]
            release_kv_pages_(cold)
            // WARM: attach 2 pages ((21-1)/8) shared and four rows of the third copied, classic-eval only the last token
            var warm <- make_run_state_paged(c, pool)
            t |> equal(20l, prefix_match_len_(cache, pool, prompt), "the probe reads what the attach will attach")
            t |> equal(20l, prefix_attach_(cache, pool, warm, prompt))
            t |> equal(g0, warm.block_table[0])   // the whole pages are SHARED, not copied
            t |> equal(g1, warm.block_table[1])
            t |> equal(2, pool.refcnt[g0])        // cache + warm
            var tail <- [for (j in range64(20l, np)); prompt[j]]
            eval_(tr, warm, tail)
            t |> success(logits_match(warm, cold_all, 0l, c.vocab_size), "post-prefill logits bit-exact")
            var wtok = argmax(warm, c.vocab_size)
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
                wtok = argmax(warm, c.vocab_size)
            }
            t |> equal(0, bad)
            // DIVERGENT suffix: same 16-token prefix, different tail — warm attach == its own cold
            var prompt2 <- [for (i in range64(24l)); i < 16l ? prompt[i] : 3l + i % 30l]
            var cold2 <- make_run_state_paged(c, pool)
            eval_(tr, cold2, prompt2)
            var warm2 <- make_run_state_paged(c, pool)
            t |> equal(16l, prefix_match_len_(cache, pool, prompt2), "the probe reads what the attach will attach")
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

//! a fresh paged session attaches `rows` of `prompt` - the last page a COPY, never `donor_page` -
//! prefills the rest and reads the cold prefill's logits bit for bit
def private attach_as_cold(t : T?; tr : Model; var cache : PrefixCache; var pool : KVPool; prompt : array<int64>; rows : int64; donor_page : int; what : string) {
    let c = tr.config
    var cold <- make_run_state_paged(c, pool)
    eval_(tr, cold, prompt)
    var warm <- make_run_state_paged(c, pool)
    t |> equal(rows, prefix_attach_(cache, pool, warm, prompt), "{what}: the rows attach")
    t |> success(warm.block_table[length(warm.block_table) - 1] != donor_page, "{what}: the partly shared page is a copy, not the donor's")
    var rest <- [for (j in range64(rows, long_length(prompt))); prompt[j]]
    eval_(tr, warm, rest)
    to_log(LOG_INFO, "{what}: argmax {argmax_of(warm.logits)} after the attach, {argmax_of(cold.logits)} cold, {mismatch_exact(warm.logits, cold.logits)} of {length(cold.logits)} logits differ\n")
    t |> equal(0, mismatch_exact(warm.logits, cold.logits), "{what}: the logits are the cold prefill's")
    release_kv_pages_(warm)
    release_kv_pages_(cold)
    delete rest
    delete warm
    delete cold
}

//! The page after the last whole hit serves the rows a prompt shares with it: the taker gets a COPY
//! of the page (the donor's stays the cache's), prefills the rest from there and lands the cold
//! prefill's logits bit for bit - the donation's partial tail whole, a whole page's leading rows, a tail's leading rows.
[test]
def test_prefix_tail_copy(t : T?) {
    t |> run("the page past the whole hits is copied for the rows the prompt shares with it, bit-exact against the cold prefill (needs stories15M.bin)") @(t : T?) {
        return if (!jit_only(t))
        if (!stat(MODEL_PATH).is_valid) {
            t |> skip("stories15M.bin not present")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        let c = tr.config
        quantize_weights(tr)
        let prev_thresh = get_target_chunk_work()
        let saved_mode = get_prefill_attn_mode()
        with_dasllama_jobque_() {
            set_target_chunk_work(1l)
            set_prefill_attn_mode(AttnPrefillMode.classic)
            var history <- [for (i in range64(29l)); 2l + i % 40l]
            var pool <- create_kv_pool_(c, 8l, KVDtype.f32, KVDtype.f32)
            var cache <- create_prefix_cache_()
            var donor <- make_run_state_paged(c, pool)
            eval_(tr, donor, history)
            prefix_insert_(cache, pool, donor, history)
            let tail_group = donor.block_table[3]
            let page2_group = donor.block_table[2]
            let page0_group = donor.block_table[0]
            t |> equal(4l, cache.n_insert_pages, "the donation's partial page is an entry beside its three whole ones")
            release_kv_pages_(donor)
            t |> equal(1, pool.refcnt[tail_group], "the cache alone holds the tail page")
            var prompt <- [for (i in range64(33l)); i < 29l ? history[i] : 7l + i]
            attach_as_cold(t, tr, cache, pool, prompt, 29l, tail_group, "the whole history and four new tokens")
            t |> equal(1l, cache.n_hit_partial, "the tail was taken as the page past the whole hits")
            t |> equal(1, pool.refcnt[tail_group], "the donor's page stays the cache's alone")
            var shorter <- [for (i in range64(31l)); i < 27l ? history[i] : 5l + i]
            attach_as_cold(t, tr, cache, pool, shorter, 27l, tail_group, "three of the tail's five rows")
            var inner <- [for (i in range64(24l)); i < 20l ? history[i] : 9l + i]
            attach_as_cold(t, tr, cache, pool, inner, 20l, page2_group, "two whole pages and four rows of the third")
            t |> equal(3l, cache.n_hit_partial, "three partial takes")
            var opening <- [for (i in range64(12l)); i < 5l ? history[i] : 11l + i]
            attach_as_cold(t, tr, cache, pool, opening, 5l, page0_group, "five rows of the first page, no whole page shared")
            t |> equal(4l, cache.n_hit_partial, "the first page was copied for its leading rows")
            delete opening
            prefix_release_(cache, pool)
            t |> equal(long_length(pool.free_groups), pool.capacity_groups, "every page is back in the pool")
            delete inner
            delete shorter
            delete prompt
            delete donor
            delete cache
            delete pool
            delete history
        }
        set_prefill_attn_mode(saved_mode)
        set_target_chunk_work(prev_thresh)
        delete tr
    }
}
