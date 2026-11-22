options gen2
options persistent_heap   // the paged/flat A/Bs hold several Sessions (see test_forward.das)
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require _model_tier
require _kv_paged_cells   // logits_equal, cache_rows_equal, paged_vs_flat_cell

// Paged KV pool (E2) — pool accounting, block-table run math, and the paged/flat storage seam on
// synthetic Configs (no model files, never skips); then paged == flat BIT-EXACTNESS on stories15M
// (decode incl. forced fragmentation, prefill incl. cache bytes, eval_batch — env-gated skips,
// same contract as test_forward).

def private mk_config(layers, heads, kv_heads, hs, ctx : int64) : Config {
    return Config(dim = heads * hs, hidden_dim = 32l, n_layers = layers, n_heads = heads,
        n_kv_heads = kv_heads, vocab_size = 8l, seq_len = ctx, head_size = hs,
        kv_dim = kv_heads * hs, shared_weights = true, rope_freq_base = 10000.0)
}

// the E-series shape from test_kv_codec: layers 4 (sliding -> src 2) and 5 (global -> src 3) share
def private mk_shared_config() : Config {
    var c = mk_config(6l, 4l, 2l, 64l, 8l)
    c.sliding_window = 4l
    c.swa_pattern = 2l
    c.n_layer_kv_from_start = 4l
    return c
}

[test(description = "A paged KV pool on the E-series shared-KV Config maps shared layers to their source's blob, sizes each blob's K and V row bytes by its own dtype, and allocates nothing up front")]
def test_kv_pool_layout(t : T?) {
    t |> run("blob_of aliases shared-KV layers; row bytes track the dtype; blobs start empty") @(t : T?) {
        let c = mk_shared_config()
        var pool <- create_kv_pool_(c, 4l, KVDtype.f16, KVDtype.f32)
        t |> equal(long_length(pool.blob_of), c.n_layers)
        t |> equal(pool.nblobs, 4l)   // layers 0..3 own, 4/5 alias
        t |> equal(pool.blob_of[4], pool.blob_of[2])
        t |> equal(pool.blob_of[5], pool.blob_of[3])
        for (l in range64(4l)) {
            t |> equal(pool.krow_bytes[pool.blob_of[l]], layer_kv_dim(c, l) * 2l)   // f16 K
            t |> equal(pool.vrow_bytes[pool.blob_of[l]], layer_kv_dim(c, l) * 4l)   // f32 V
        }
        t |> equal(pool.capacity_groups, 0l)
        t |> equal(0, length(pool.kblobs[0]))   // nothing allocated until a session needs a page
        delete pool
    }
}

[test(description = "Pool groups allocate LIFO from a free list that grows geometrically (4 then 8 then 16), release returns a group at refcount 0, and growth inside a reserve moves no blob and keeps written rows")]
def test_kv_pool_alloc_release(t : T?) {
    t |> run("alloc pops LIFO from a geometrically grown free list; release returns at refcnt 0") @(t : T?) {
        let c = mk_config(2l, 4l, 2l, 16l, 32l)
        var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
        let g0 = kv_pool_alloc_group(pool)
        let g1 = kv_pool_alloc_group(pool)
        t |> equal(0, g0)   // first grow pushes high->low, so the lowest id pops first
        t |> equal(1, g1)
        t |> equal(pool.capacity_groups, 4l)
        t |> equal(long_length(pool.kblobs[0]), 4l * 4l * pool.krow_bytes[0])
        t |> equal(1, pool.refcnt[g0])
        kv_pool_release_group(pool, g0)
        t |> equal(0, pool.refcnt[g0])
        t |> equal(3, length(pool.free_groups))
        let g0b = kv_pool_alloc_group(pool)
        t |> equal(g0, g0b)   // LIFO: the just-freed group comes back first
        // exhaust the first grow, then force the geometric second one
        for (_i in range(2)) {
            kv_pool_alloc_group(pool)
        }
        t |> equal(pool.capacity_groups, 4l)
        let g4 = kv_pool_alloc_group(pool)
        t |> equal(4, g4)
        t |> equal(pool.capacity_groups, 8l)
        delete pool
    }
    t |> run("growth inside a reserve moves no blob and keeps the rows written; past it the pool still grows") @(t : T?) {
        let c = mk_config(2l, 4l, 2l, 16l, 32l)
        var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
        kv_pool_reserve(pool, 32l)
        t |> equal(long_capacity(pool.kblobs[0]), 8l * 4l * pool.krow_bytes[0])
        t |> equal(pool.capacity_groups, 0l)
        kv_pool_alloc_group(pool)
        pool.kblobs[0][0] = 0x5au8
        let home = unsafe(addr(pool.kblobs[0][0]))
        for (_i in range(7)) {
            kv_pool_alloc_group(pool)
        }
        t |> equal(pool.capacity_groups, 8l)
        t |> equal(home, unsafe(addr(pool.kblobs[0][0])))
        t |> equal(pool.kblobs[0][0], 0x5au8)
        t |> equal(8, kv_pool_alloc_group(pool))
        t |> equal(pool.capacity_groups, 16l)
        t |> equal(pool.kblobs[0][0], 0x5au8)
        delete pool
    }
}

[test(description = "`kv_ensure` grows a paged session's block table to ceil(upto / page) pages, idempotently and never shrinking, and releasing returns every page so the pool's free list is back to capacity")]
def test_kv_ensure_growth(t : T?) {
    t |> run("kv_ensure grows the block table to cover [0, upto); release returns every page") @(t : T?) {
        let c = mk_config(2l, 4l, 2l, 16l, 32l)
        var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
        var s <- make_run_state_paged(c, pool)
        t |> success(s.kv_pool != null, "paged session carries the pool")
        t |> equal(0, length(s.key_cache))    // flat slabs stay empty
        t |> equal(0, length(s.value_cache))
        kv_ensure(s, 1l)
        t |> equal(1, length(s.block_table))
        kv_ensure(s, 9l)                      // ceil(9/4) = 3 pages
        t |> equal(3, length(s.block_table))
        kv_ensure(s, 9l)                      // idempotent
        t |> equal(3, length(s.block_table))
        kv_ensure(s, 2l)                      // never shrinks
        t |> equal(3, length(s.block_table))
        release_kv_pages_(s)
        t |> equal(0, length(s.block_table))
        var live = 0
        for (g in range64(pool.capacity_groups)) {
            live += pool.refcnt[g]
        }
        t |> equal(0, live)
        t |> equal(long_length(pool.free_groups), pool.capacity_groups)   // accounting back to baseline
        release_kv_pages_(s)                  // no-op on an empty table
        delete s
        delete pool
    }
}

[test(description = "Paged `kv_runs` maps windows through the block table: physically consecutive pages merge into one run, shuffled pages split at every straddled boundary, and `kv_phys_row` follows the same table")]
def test_kv_runs_paged(t : T?) {
    t |> run("paged kv_runs: page-span runs through the block table, physically-consecutive spans merge") @(t : T?) {
        let c = mk_config(2l, 4l, 2l, 16l, 16l)
        var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
        var s <- make_run_state_paged(c, pool)
        t |> equal(5, length(s.kv_runs))   // ceil(16/4) + 1 worst-case scratch
        // identity table: pages are physically consecutive -> every window merges to ONE run
        s.block_table |> reserve(4)
        for (g in range(4)) {
            s.block_table |> push(g)
        }
        var nrun = kv_runs(s, 0l, 16l, s.kv_runs)
        t |> equal(1l, nrun)
        t |> equal(0l, s.kv_runs[0].j0)
        t |> equal(16l, s.kv_runs[0].j1)
        t |> equal(0l, s.kv_runs[0].row0)
        nrun = kv_runs(s, 5l, 12l, s.kv_runs)   // straddles pages 1..2, still consecutive
        t |> equal(1l, nrun)
        t |> equal(5l, s.kv_runs[0].row0)
        // shuffled table: every straddled boundary splits, row0 goes through the table
        s.block_table[0] = 2
        s.block_table[1] = 0
        s.block_table[2] = 3
        s.block_table[3] = 1
        nrun = kv_runs(s, 5l, 12l, s.kv_runs)   // [5,8) in page 1, [8,12) in page 2
        t |> equal(2l, nrun)
        t |> equal(5l, s.kv_runs[0].j0)
        t |> equal(8l, s.kv_runs[0].j1)
        t |> equal(1l, s.kv_runs[0].row0)       // group 0, in-page offset 1
        t |> equal(8l, s.kv_runs[1].j0)
        t |> equal(12l, s.kv_runs[1].j1)
        t |> equal(12l, s.kv_runs[1].row0)      // group 3 * 4
        nrun = kv_runs(s, 6l, 8l, s.kv_runs)    // inside one page
        t |> equal(1l, nrun)
        t |> equal(2l, s.kv_runs[0].row0)
        t |> equal(0l, kv_runs(s, 9l, 9l, s.kv_runs))   // empty window
        t |> equal(0l, kv_runs(s, 9l, 3l, s.kv_runs))   // inverted window
        // partial merge: pages 2 and 3 hold groups 3 and 1 -> [8,16) splits; pages 0 and 1 hold
        // groups 2 and 0 -> no merge either; but a table like [0,1,3,...] merges only the 0/1 seam
        s.block_table[0] = 0
        s.block_table[1] = 1
        s.block_table[2] = 3
        nrun = kv_runs(s, 0l, 12l, s.kv_runs)
        t |> equal(2l, nrun)
        t |> equal(8l, s.kv_runs[0].j1)   // pages 0+1 merged
        t |> equal(12l, s.kv_runs[1].row0)
        // kv_phys_row goes through the same table
        t |> equal(1l, kv_phys_row(s, 1l))
        t |> equal(5l, kv_phys_row(s, 5l))
        t |> equal(13l, kv_phys_row(s, 9l))   // page 2 = group 3: 3*4 + 1
        s.block_table |> clear()   // hand-built table, never pool-allocated: don't release
        delete s
        delete pool
    }
}

[test(description = "A paged pool's cache bytes at 10 positions are under half the flat slabs sized for seq_len 1024, and grow only as the context deepens to 600 positions")]
def test_kv_paged_memory_proportional(t : T?) {
    t |> run("paged cache bytes track actual context, not seq_len") @(t : T?) {
        let c = mk_config(4l, 4l, 2l, 64l, 1024l)
        var flat <- make_run_state(c)
        let flat_bytes = long_length(flat.key_cache) + long_length(flat.value_cache)
        delete flat
        var pool <- create_kv_pool_(c, 64l, KVDtype.f32, KVDtype.f32)
        var s <- make_run_state_paged(c, pool)
        kv_ensure(s, 10l)   // 10 positions -> 1 page -> first grow (4 groups)
        var paged_bytes = 0l
        for (b in range64(pool.nblobs)) {
            paged_bytes += long_length(pool.kblobs[b]) + long_length(pool.vblobs[b])
        }
        t |> success(paged_bytes * 2l < flat_bytes,
            "10-position paged pool ({paged_bytes}B) is far under the flat slabs ({flat_bytes}B)")
        kv_ensure(s, 600l)   // 10 pages -> grows toward the context, still bounded
        var grown = 0l
        for (b in range64(pool.nblobs)) {
            grown += long_length(pool.kblobs[b]) + long_length(pool.vblobs[b])
        }
        t |> success(grown > paged_bytes, "deeper context grows the pool")
        release_kv_pages_(s)
        delete s
        delete pool
    }
}

[test(description = "A `kv_store_row` through the paged layer base and `kv_phys_row` reads back exactly at the block-table row, and shared-KV layers' K and V bases alias their source layer's")]
def test_kv_paged_store_roundtrip(t : T?) {
    t |> run("kv_store_row through kv_layer_kbase + kv_phys_row round-trips; shared layers alias") @(t : T?) {
        let c = mk_shared_config()
        var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
        var s <- make_run_state_paged(c, pool)
        kv_ensure(s, 7l)   // 2 pages
        let kv_dim = layer_kv_dim(c, 1l)
        var src : array<float>
        src |> resize(int(kv_dim))
        for (i in range(length(src))) {
            src[i] = float(i) * 0.25 - 3.0
        }
        unsafe {
            var kcl = kv_layer_kbase(s, 1l, c.seq_len)
            let pos = 6l   // second page
            kv_store_row(kcl, s.kv_dtype_k, kv_dim, kv_phys_row(s, pos), 0l, kv_dim,
                addr<float const?>(src[0]))
            var d = reinterpret<float const?>(kcl)
            var ok = true
            for (i in range64(kv_dim)) {
                ok &&= d[kv_phys_row(s, pos) * kv_dim + i] == src[i]
            }
            t |> success(ok, "paged store reads back exactly at the block-table row")
            // shared-KV layers alias their source's blob — same base pointer
            t |> success(kv_layer_kbase(s, 4l, c.seq_len) == kv_layer_kbase(s, 2l, c.seq_len),
                "layer 4 K base aliases its source layer 2")
            t |> success(kv_layer_vbase(s, 5l, c.seq_len) == kv_layer_vbase(s, 3l, c.seq_len),
                "layer 5 V base aliases its source layer 3")
        }
        delete src
        release_kv_pages_(s)
        delete s
        delete pool
    }
}

// ===== paged == flat bit-exactness on a real model (env-gated, JIT-only) =====


def private mk_prompt(n : int64) : array<int64> => <- [for (i in range64(n)); 2l + i % 40l]

//! On stories15M, a 21-token prefill straddling three 8-row pages matches flat prefill bit for bit in logits and cache rows, and eight greedy decode steps on top stay bit-exact
def test_kv_paged_prefill(t : T?; tr : Model) {
    unsafe {
        t |> run("paged prefill == flat prefill: logits and cache rows bit-exact (needs stories15M.bin)") @capture(& tr)(t : T ?) {
            let c = tr.config
            with_threaded_jobque() {
                // 21 tokens straddles pages at P=8 (2 full + 1 partial)
                var prompt <- [for (i in range(21)); int64(i % 50 + 2)]
                let n = long_length(prompt)
                var pool <- create_kv_pool_(c, 8l, KVDtype.f32, KVDtype.f32)
                var sp <- make_run_state_paged(c, pool)
                forward_batch(tr, sp, prompt, n, 0l)
                var sf <- make_run_state(c)
                forward_batch(tr, sf, prompt, n, 0l)
                t |> success(logits_equal(sp, sf, c.vocab_size), "prefill logits bit-exact")
                t |> success(cache_rows_equal(sf, sp, c, n), "prefill cache rows byte-equal flat")
                // decode a few more on top of the paged prefill, against the flat reference
                var tok = argmax(sf, c.vocab_size)
                var bad = 0
                for (i in range64(8l)) {
                    forward(tr, sp, tok, n + i)
                    forward(tr, sf, tok, n + i)
                    if (!logits_equal(sp, sf, c.vocab_size)) {
                        bad ++
                    }
                    tok = argmax(sf, c.vocab_size)
                }
                t |> equal(0, bad)
                release_kv_pages_(sp)
                delete sp
                delete sf
                delete pool
                delete prompt
            }
        }
    }
}

//! On stories15M, `eval_batch` over three paged sessions of ragged depth on one pool, one crossing a page mid-decode, matches flat sessions' logits bit for bit for six steps and the deepest cache rows byte for byte
def test_kv_paged_eval_batch(t : T?; tr : Model) {
    unsafe {
        t |> run("paged eval_batch == flat eval_batch, ragged depths on one pool (needs stories15M.bin)") @capture(& tr)(t : T ?) {
            let c = tr.config
            with_threaded_jobque() {
                // ragged depths: 1, 2 and 3 pages at P=8; q0 crosses a page boundary MID-batch-decode
                // (kv_ensure growth inside eval_batch), and the sequential prefills interleave the
                // sessions' groups in the pool
                var p0 <- mk_prompt(5l)
                var p1 <- mk_prompt(9l)
                var p2 <- mk_prompt(17l)
                var pool <- create_kv_pool_(c, 8l, KVDtype.f32, KVDtype.f32)
                var q0 <- make_run_state_paged(c, pool)
                var q1 <- make_run_state_paged(c, pool)
                var q2 <- make_run_state_paged(c, pool)
                var f0 <- make_run_state(c)
                var f1 <- make_run_state(c)
                var f2 <- make_run_state(c)
                eval_(tr, q0, p0)
                eval_(tr, q1, p1)
                eval_(tr, q2, p2)
                eval_(tr, f0, p0)
                eval_(tr, f1, p1)
                eval_(tr, f2, p2)
                var ws <- create_batch_workspace_(tr)
                var qsess <- [unsafe(addr(q0)), unsafe(addr(q1)), unsafe(addr(q2))]
                var fsess <- [unsafe(addr(f0)), unsafe(addr(f1)), unsafe(addr(f2))]
                var toks : array<int64>
                toks |> resize(3)
                var bad = 0
                for (step in range64(6l)) {
                    for (i in range64(3l)) {
                        toks[i] = 100l + step * 3l + i
                    }
                    eval_batch_(tr, ws, qsess, toks)
                    eval_batch_(tr, ws, fsess, toks)
                    for (i in range64(3l)) {
                        if (!logits_equal(*qsess[i], *fsess[i], c.vocab_size)) {
                            bad ++
                        }
                    }
                }
                t |> equal(0, bad)
                t |> equal(q0.n_past, f0.n_past)
                t |> success(cache_rows_equal(f2, q2, c, q2.n_past), "deepest row cache rows byte-equal flat")
                release_kv_pages_(q0)
                release_kv_pages_(q1)
                release_kv_pages_(q2)
                qsess |> clear()   // drop the borrowed handles: delete of array<T?> cascades to the pointees
                fsess |> clear()
                unsafe {
                    delete ws
                    delete qsess
                    delete fsess
                }
                delete toks
                delete q0
                delete q1
                delete q2
                delete f0
                delete f1
                delete f2
                delete pool
                delete p0
                delete p1
                delete p2
            }
        }
    }
}

[test(description = "stories15M on one load, its weights quantized, runs the paged-decode, paged-prefill and paged eval_batch cells in turn, each on its own pool and sessions")]
def test_kv_paged_stories_cells(t : T?) {
    with_stories15m(t) $(var tr) {
        paged_vs_flat_cell(t, tr, "interleaved paged sessions == flat, bit-exact under fragmentation (needs stories15M.bin)", KVDtype.f32, "")
        test_kv_paged_prefill(t, tr)
        test_kv_paged_eval_batch(t, tr)
    }
}
