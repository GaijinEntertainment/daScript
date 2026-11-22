options gen2
options _dasllama_internal = true

module _kv_paged_cells shared public

//! the paged K/V pool's cells and flat-vs-paged compares, on a model its caller loaded; none loads one

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require _model_tier   // with_threaded_jobque
require _cpu_gpu_model   // with_ctx_cut: the cell's 64-position context

// The paged K/V pool's cells on a loaded model, and the flat-vs-paged compares they and `test_kv_paged.das`'s stories15M
// cells read: two paged sessions interleaved on one pool against flat references, under f32 on stories15M and under q8_0 on
// SmolLM2-135M (the head_size-64 carrier - stories15M's 48 is rejected by the 32-block check).

//! whether two sessions' logit rows are bit-identical over the vocabulary
def logits_equal(a : Session; b : Session; vocab : int64) : bool {
    for (i in range64(vocab)) {
        if (a.logits[i] != b.logits[i]) {
            return false
        }
    }
    return true
}

//! per-(layer, position) cache-row byte compare through the storage seam - flat rows at identity, paged rows through the
//! block table
def cache_rows_equal(var sf : Session; var sp : Session; c : Config; npos : int64) : bool {
    unsafe {
        for (l in range64(c.n_layers)) {
            let kvd = layer_kv_dim(c, l)
            let krb = kv_row_bytes(sf.kv_dtype_k, kvd)
            let vrb = kv_row_bytes(sf.kv_dtype_v, kvd)
            let fk = kv_layer_kbase(sf, l, c.seq_len)
            let pk = kv_layer_kbase(sp, l, c.seq_len)
            let fv = kv_layer_vbase(sf, l, c.seq_len)
            let pv = kv_layer_vbase(sp, l, c.seq_len)
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

//! the run `title`: two paged `kdt` sessions interleaved on one pool (16-row pages) fragment its pages yet match their flat `kdt`
//! references bit for bit in logits over 40 decode steps and in every cache row, each session on three pages; `what` names the
//! codec in the cache-row asserts, dispatch forced threaded
def paged_vs_flat_cell(t : T?; tr : Model; title : string; kdt : KVDtype; what : string) {
    unsafe {
        t |> run(title) @capture(& tr)(t : T ?) {
            let c = tr.config
            with_threaded_jobque() {
                //! the two sessions interleave allocations on ONE pool, so their pages FRAGMENT: [0,2,4] vs [1,3,5]
                var pool <- create_kv_pool_(c, 16l, kdt, kdt)
                var pa <- make_run_state_paged(c, pool)
                var pb <- make_run_state_paged(c, pool)
                var fa <- make_run_state(c, kdt, kdt)
                var fb <- make_run_state(c, kdt, kdt)
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
                t |> success(cache_rows_equal(fa, pa, c, nsteps), "session A{what} cache rows byte-equal flat")
                t |> success(cache_rows_equal(fb, pb, c, nsteps), "session B{what} cache rows byte-equal flat")
                release_kv_pages_(pa)
                release_kv_pages_(pb)
                delete pa
                delete pb
                delete fa
                delete fb
                delete pool
            }
        }
    }
}

//! On SmolLM2-135M `tr`, its context cut to 64: the paged-vs-flat cell under q8_0 (fragmented BYTE-strided runs)
def kv_paged_q8_cell(t : T?; var tr : Model) {
    with_ctx_cut(tr, 64l) {
        paged_vs_flat_cell(t, tr, "paged q8_0 sessions == flat q8_0, bit-exact under fragmentation (needs SmolLM2-135M)", KVDtype.q8_0, " q8")
    }
}
