options gen2
options persistent_heap   // the paged/flat A/Bs hold several Sessions (see test_forward.das)
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_math   // setup_dasllama_jobque_ + target_chunk_work
require daslib/jobque_boost
require daslib/fio

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

[test]
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

[test]
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
}

[test]
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

[test]
def test_kv_runs_paged(t : T?) {
    t |> run("paged kv_runs: page-span runs through the block table, physically-consecutive spans merge") @(t : T?) {
        let c = mk_config(2l, 4l, 2l, 16l, 16l)
        var pool <- create_kv_pool_(c, 4l, KVDtype.f32, KVDtype.f32)
        var s <- make_run_state_paged(c, pool)
        t |> equal(5, length(s.kv_runs))   // ceil(16/4) + 1 worst-case scratch
        // identity table: pages are physically consecutive -> every window merges to ONE run
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

[test]
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

[test]
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

def private llama2c_dir() : string {
    let env = get_env_variable("DASLLAMA_LLAMA2C_DIR")
    return empty(env) ? "/Users/borisbatkin/Work/llama2.c" : env
}
let MODEL_PATH = path_join(llama2c_dir(), "stories15M.bin")

def private mk_prompt(n : int64) : array<int64> {
    return <- [for (i in range64(n)); 2l + i % 40l]
}

def private argmax_of(s : Session; vocab : int64) : int64 {
    var best = 0l
    for (i in range64(1l, vocab)) {
        if (s.logits[i] > s.logits[best]) {
            best = i
        }
    }
    return best
}

def private logits_equal(a : Session; b : Session; vocab : int64) : bool {
    for (i in range64(vocab)) {
        if (a.logits[i] != b.logits[i]) {
            return false
        }
    }
    return true
}

// per-(layer, position) cache-row byte compare through the storage seam — flat rows at identity,
// paged rows through the block table
def private cache_rows_equal(var sf : Session; var sp : Session; c : Config; npos : int64) : bool {
    unsafe {
        for (l in range64(c.n_layers)) {
            let kvd = layer_kv_dim(c, l)
            let krb = kv_row_bytes(sf.kv_dtype_k, kvd)
            let vrb = kv_row_bytes(sf.kv_dtype_v, kvd)
            var fk = kv_layer_kbase(sf, l, c.seq_len)
            var pk = kv_layer_kbase(sp, l, c.seq_len)
            var fv = kv_layer_vbase(sf, l, c.seq_len)
            var pv = kv_layer_vbase(sp, l, c.seq_len)
            for (pos in range64(npos)) {
                let pr = kv_phys_row(sp, pos)
                for (b in range64(krb)) {
                    if (fk[pos * krb + b] != pk[pr * krb + b]) {
                        return false
                    }
                }
                for (b in range64(vrb)) {
                    if (fv[pos * vrb + b] != pv[pr * vrb + b]) {
                        return false
                    }
                }
            }
        }
    }
    return true
}

[test]
def test_kv_paged_decode(t : T?) {
    t |> run("interleaved paged sessions == flat, bit-exact under fragmentation (needs stories15M.bin)") @(t : T?) {
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
        with_job_que() {
            setup_dasllama_jobque_()
            set_target_chunk_work(1l)
            // two paged sessions interleave allocations on ONE pool (P=16), so their pages
            // FRAGMENT: [0,2,4] vs [1,3,5]. Each decodes a deterministic token stream and must
            // stay bit-exact against its own flat reference, every step.
            var pool <- create_kv_pool_(c, 16l, KVDtype.f32, KVDtype.f32)
            var pa <- make_run_state_paged(c, pool)
            var pb <- make_run_state_paged(c, pool)
            var fa <- make_run_state(c)
            var fb <- make_run_state(c)
            let nsteps = 40l
            var bad = 0
            for (i in range64(nsteps)) {
                let ta = 2l + (i * 7l) % (c.vocab_size - 2l)
                let tb = 2l + (i * 11l) % (c.vocab_size - 2l)
                forward(tr, pa, ta, i)
                forward(tr, pb, tb, i)
                forward(tr, fa, ta, i)
                forward(tr, fb, tb, i)
                if (!logits_equal(pa, fa, c.vocab_size) || !logits_equal(pb, fb, c.vocab_size)) {
                    bad ++
                }
            }
            t |> equal(0, bad)
            t |> equal(3, length(pa.block_table))
            t |> success(pa.block_table[1] != pa.block_table[0] + 1,
                "interleaved sessions actually fragment the pool")
            t |> success(cache_rows_equal(fa, pa, c, nsteps), "session A cache rows byte-equal flat")
            t |> success(cache_rows_equal(fb, pb, c, nsteps), "session B cache rows byte-equal flat")
            release_kv_pages_(pa)
            release_kv_pages_(pb)
            delete pa
            delete pb
            delete fa
            delete fb
            delete pool
        }
        set_target_chunk_work(prev_thresh)
        delete tr
    }
}

[test]
def test_kv_paged_q8(t : T?) {
    t |> run("paged q8_0 sessions == flat q8_0, bit-exact under fragmentation (needs SmolLM2-135M)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        // q8_0 needs a head_size-64 carrier (stories15M's 48 is rejected by the 32-block check)
        let env = get_env_variable("DASLLAMA_MODELS_DIR")
        let mdir = empty(env) ? "/Users/borisbatkin/Work/llama.cpp/models/" : env
        let path = path_join(mdir, "SmolLM2-135M-Instruct-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var tr <- load_gguf(path, QuantMode.q8)
        if (tr.config.seq_len > 64l) {   // cap the KV allocation (no math require here)
            tr.config.seq_len = 64l
        }
        let c = tr.config
        let prev_thresh = get_target_chunk_work()
        with_job_que() {
            setup_dasllama_jobque_()
            set_target_chunk_work(1l)
            // two q8 paged sessions interleave on ONE pool (P=16): fragmented BYTE-strided runs
            var pool <- create_kv_pool_(c, 16l, KVDtype.q8_0, KVDtype.q8_0)
            var pa <- make_run_state_paged(c, pool)
            var pb <- make_run_state_paged(c, pool)
            var fa <- make_run_state(c, KVDtype.q8_0, KVDtype.q8_0)
            var fb <- make_run_state(c, KVDtype.q8_0, KVDtype.q8_0)
            let nsteps = 40l
            var bad = 0
            for (i in range64(nsteps)) {
                let ta = 2l + (i * 7l) % (c.vocab_size - 2l)
                let tb = 2l + (i * 11l) % (c.vocab_size - 2l)
                forward(tr, pa, ta, i)
                forward(tr, pb, tb, i)
                forward(tr, fa, ta, i)
                forward(tr, fb, tb, i)
                if (!logits_equal(pa, fa, c.vocab_size) || !logits_equal(pb, fb, c.vocab_size)) {
                    bad ++
                }
            }
            t |> equal(0, bad)
            t |> success(pa.block_table[1] != pa.block_table[0] + 1,
                "interleaved sessions actually fragment the pool")
            t |> success(cache_rows_equal(fa, pa, c, nsteps), "session A q8 cache rows byte-equal flat")
            t |> success(cache_rows_equal(fb, pb, c, nsteps), "session B q8 cache rows byte-equal flat")
            release_kv_pages_(pa)
            release_kv_pages_(pb)
            delete pa
            delete pb
            delete fa
            delete fb
            delete pool
        }
        set_target_chunk_work(prev_thresh)
        delete tr
    }
}

[test]
def test_kv_paged_prefill(t : T?) {
    t |> run("paged prefill == flat prefill: logits and cache rows bit-exact (needs stories15M.bin)") @(t : T?) {
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
        with_job_que() {
            setup_dasllama_jobque_()
            set_target_chunk_work(1l)
            var prompt : array<int64>
            for (i in range(21)) {   // straddles pages at P=8 (2 full + 1 partial)
                prompt |> push(int64(i % 50 + 2))
            }
            let n = long_length(prompt)
            var pool <- create_kv_pool_(c, 8l, KVDtype.f32, KVDtype.f32)
            var sp <- make_run_state_paged(c, pool)
            forward_batch(tr, sp, prompt, n, 0l)
            var sf <- make_run_state(c)
            forward_batch(tr, sf, prompt, n, 0l)
            t |> success(logits_equal(sp, sf, c.vocab_size), "prefill logits bit-exact")
            t |> success(cache_rows_equal(sf, sp, c, n), "prefill cache rows byte-equal flat")
            // decode a few more on top of the paged prefill, against the flat reference
            var tok = argmax_of(sf, c.vocab_size)
            var bad = 0
            for (i in range64(8l)) {
                forward(tr, sp, tok, n + i)
                forward(tr, sf, tok, n + i)
                if (!logits_equal(sp, sf, c.vocab_size)) {
                    bad ++
                }
                tok = argmax_of(sf, c.vocab_size)
            }
            t |> equal(0, bad)
            release_kv_pages_(sp)
            delete sp
            delete sf
            delete pool
            delete prompt
        }
        set_target_chunk_work(prev_thresh)
        delete tr
    }
}

[test]
def test_kv_paged_eval_batch(t : T?) {   // nolint:STYLE038 — flat 3-session paged/flat A/B setup + explicit per-object teardown
    t |> run("paged eval_batch == flat eval_batch, ragged depths on one pool (needs stories15M.bin)") @(t : T?) {
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
        with_job_que() {
            setup_dasllama_jobque_()
            set_target_chunk_work(1l)
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
        set_target_chunk_work(prev_thresh)
        delete tr
    }
}
