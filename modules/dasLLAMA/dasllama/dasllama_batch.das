options gen2
options _comment_hygiene = true

module dasllama_batch shared public

require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_par
require dasllama/dasllama_convert   // quantize_q8_0_into — the per-row activation quant the batched attention feeds
require math

// ===== batched decode =====
// One decode step over B sessions through ONE pass of the weights: GEMVs become B-row GEMMs via
// the batched prefill blocks; attention stays per-(row, head) since each reads only ITS session's cache.

def create_batch_workspace_(model : Model) : BatchWorkspace {
    var ws : BatchWorkspace
    // the scratch Session never owns cache or sampler state — eval_batch reads/writes the real
    // sessions' caches through kbase/vbase (flat) or the batch's pool (paged)
    ws.scr <- make_run_state(model.config, KVDtype.f32, KVDtype.f32, false)
    delete ws.scr.sample_scratch
    return <- ws
}

// B-row slice of forward_prefill_alloc: exactly the buffers the batched-decode step touches (no
// prefill attention scratch — att_b sizes per layer in attention_batch_decode, fa_* never used).
def private batch_decode_alloc(t : Model; var s : Session; nrows : int64) {
    let c = t.config
    let dim = c.dim
    let hidden = c.hidden_dim
    let kv_dim = max_kv_dim(c)
    let head_size = max_head_size(c)
    let qd = c.n_heads * head_size
    let xdim = max(dim, qd)
    let maxn = max(xdim, hidden)
    s.x_b |> resize(nrows * dim)
    s.xb_b |> resize(nrows * xdim)
    s.xb2_b |> resize(nrows * dim)
    s.q_b |> resize(nrows * qd)
    s.k_b |> resize(nrows * kv_dim)
    s.v_b |> resize(nrows * kv_dim)
    s.hb_b |> resize(nrows * hidden)
    s.hb2_b |> resize(nrows * hidden)
    s.xqb |> resize(nrows * maxn)
    s.xsb |> resize(nrows * maxn / 32l)
    if (t.kquant_native) {   // kq batch tile: per-16 sums beside the batch quants
        s.xbsb |> resize(nrows * maxn / 16l)
    }
    if (s.kv_dtype_k == KVDtype.q8_0 || s.kv_dtype_k == KVDtype.tq4) {   // batch query-quant planes (dtypes snapshotted before this)
        s.attq |> resize(nrows * qd)
        s.atts |> resize(nrows * qd / 32l)
    }
    if (has_ple(c)) {   // gemma-4 E-series PLE scratch (one row per batch slot)
        let all = c.n_layers * c.n_embd_per_layer
        s.ple_inp |> resize(nrows * all)
        s.ple_proj |> resize(nrows * all)
        s.ple_gate |> resize(nrows * c.n_embd_per_layer)
    }
}

// One layer's batched-decode attention: shared prefix (norm → QKV GEMM → qk/v-norm → per-row RoPE
// via the positions table) → scatter-store each row's k/v into ITS session's cache → per-(row, head)
// decode attention through the codec seam worker → shared suffix (wo GEMM → post-norm → residual).
def private attention_batch_decode(t : Model; var ws : BatchWorkspace; sessions : array<Session?>; l, nrows : int64) { // nolint:STYLE037 — per-(row, head) attention over B sessions: cache-geometry arms are flat and share the hoisted row pointers
    let c = t.config
    let kv_dim = layer_kv_dim(c, l)
    let head_size = layer_head_size(c, l)
    let n_heads = c.n_heads
    let n_kv_heads = layer_n_kv_heads(c, l)
    let qd = n_heads * head_size
    let kv_mul = n_heads / n_kv_heads
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    let is_sliding = layer_is_sliding(c, l)
    let swin = c.sliding_window
    let kv_shared = t.kv_src[l] != l
    // flat rows address each session's slab; paged rows all address the batch's ONE pool for layer l
    let paged = ws.scr.kv_pool != null
    let kbb = paged ? 0l : kv_base_k(ws.scr, l, c.seq_len)   // layer slab base BYTES — uniform across the batch
    let vbb = paged ? 0l : kv_base_v(ws.scr, l, c.seq_len)   // (ws.scr carries the batch's dtypes + prefixes)
    var pkcl = paged ? kv_layer_kbase(ws.scr, l, c.seq_len) : null
    var pvcl = paged ? kv_layer_vbase(ws.scr, l, c.seq_len) : null
    let kdt = ws.scr.kv_dtype_k
    let vdt = ws.scr.kv_dtype_v
    // start_pos is only for the classic rope path; eval_batch_ never routes here (rows table bakes position)
    attn_qkv_rope_prefix(t, ws.scr, l, nrows, 0l)
    let ktq4 = kdt == KVDtype.tq4
    let vtq4 = vdt == KVDtype.tq4
    if (ktq4 || vtq4) {
        // tq4 basis change at the RoPE seam (see attention_std_decode); xb_b un-rotates below
        let ts_rot = ref_time_ticks()
        unsafe {
            let sgp = addr < float const? >(ws.scr.kv_signs[0])
            if (ktq4) {
                tq4_rotate_batch(ws.scr.q_b, nrows, qd, head_size, sgp)
                if (!kv_shared) {
                    tq4_rotate_batch(ws.scr.k_b, nrows, kv_dim, head_size, sgp)
                }
            }
            if (vtq4 && !kv_shared) {
                tq4_rotate_batch(ws.scr.v_b, nrows, kv_dim, head_size, sgp)
            }
        }
        prof_add("tq4_rot", ts_rot)
    }
    if (kdt == KVDtype.q8_0 || ktq4) {
        // q_b rows are contiguous and qd % 32 == 0, so per-32-block quants match the per-row form
        quantize_q8_0_into(ws.scr.q_b, nrows * qd, ws.scr.attq, ws.scr.atts, 0l, 0l)
    }
    if (!kv_shared) {
        let ts_kv = ref_time_ticks()
        unsafe {
            let kbp = addr < float const? >(ws.scr.k_b[0])
            let vbp = addr < float const? >(ws.scr.v_b[0])
            for (i in range64(nrows)) {
                let prow = kv_phys_row(*sessions[i], ws.positions[i])   // flat: identity
                kv_store_row(paged ? pkcl : ws.kbase[i] + kbb, kdt, kv_dim, prow, 0l, kv_dim, kbp + i * kv_dim)
                kv_store_row(paged ? pvcl : ws.vbase[i] + vbb, vdt, kv_dim, prow, 0l, kv_dim, vbp + i * kv_dim)
            }
        }
        prof_add("kv_store", ts_kv)
    }
    // per-(row, head): row i attends to ITS cache; lane count is work-proportional to KV traffic.
    let ts_attn = ref_time_ticks()
    var maxctx = 0l
    var work = 0l   // Σ 2·cnt_i·head_size MACs per head
    // runs_b row stride: a flat row is always one run; a paged row can span every page of the context
    let maxruns = paged ? (c.seq_len + ws.scr.kv_pool.page_rows - 1l) / ws.scr.kv_pool.page_rows + 1l : 1l
    ws.runs_b |> resize(nrows * maxruns)
    ws.nrun_b |> resize(nrows)
    for (i in range64(nrows)) {
        let pos = ws.positions[i]
        let kstart = (is_sliding && pos + 1l > swin) ? (pos + 1l - swin) : 0l
        maxctx = max(maxctx, pos + 1l - kstart)
        work += 2l * (pos + 1l - kstart) * head_size
        ws.nrun_b[i] = kv_runs(*sessions[i], kstart, pos + 1l, ws.runs_b, i * maxruns)   // flat: 1 identity run
    }
    ws.scr.att_b |> resize(nrows * n_heads * maxctx)
    let attn_softcap = c.attn_logit_softcap
    let attn_lanes = min(int(nrows * n_heads), lanes_for_work(work * n_heads, g_gemv_lane_cap))
    let sinksp = (c.attn_sinks
        ? unsafe(addr < float const? >(t.fblob[t.sinks_off + l * n_heads]))
        : null)
    unsafe {
        let qp = addr < float const? >(ws.scr.q_b[0])
        let kq8 = kdt == KVDtype.q8_0 || kdt == KVDtype.tq4
        let qqp0 = kq8 ? addr < int8 const? >(ws.scr.attq[0]) : null
        let qsp0 = kq8 ? addr < float const? >(ws.scr.atts[0]) : null
        var xbp = addr(ws.scr.xb_b[0])
        var attp = addr(ws.scr.att_b[0])
        let posp = addr < int64 const? >(ws.positions[0])
        let runsbp = addr < KVRun const? >(ws.runs_b[0])
        let nrunbp = addr < int64 const? >(ws.nrun_b[0])
        var kbpp = addr(ws.kbase[0])
        var vbpp = addr(ws.vbase[0])
        maybe_parallel_for(0, int(nrows * n_heads), attn_lanes) $(rb, re) {
            unsafe {
                for (idx in range(rb, re)) {
                    let ix = int64(idx)
                    let ii = ix / n_heads
                    let hh = ix % n_heads
                    let pos = posp[ii]
                    let kstart = (is_sliding && pos + 1l > swin) ? (pos + 1l - swin) : 0l
                    let cnt = pos + 1l - kstart
                    let qoff = hh * head_size
                    let kvhoff = (hh / kv_mul) * head_size
                    attn_head_decode_d(attp + ix * maxctx, xbp + ii * qd, qp + ii * qd,
                        kq8 ? qqp0 + ii * qd : null, kq8 ? qsp0 + ii * qd / 32l : null,
                        paged ? pkcl : kbpp[ii] + kbb, paged ? pvcl : vbpp[ii] + vbb, kdt, vdt,
                        runsbp + ii * maxruns, nrunbp[ii], qoff, kvhoff, kstart, cnt, kv_dim, head_size,
                        hh, scale, attn_softcap, sinksp)
                }
            }
        }
    }
    prof_add("attn", ts_attn)
    if (vtq4) {   // tq4 V: xb_b rows accumulated in the rotated basis — un-rotate per (row, head)
        unsafe {
            tq4_unrotate_batch(ws.scr.xb_b, nrows, qd, head_size, addr < float const? >(ws.scr.kv_signs[0]))
        }
    }
    attn_out_suffix(t, ws.scr, l, nrows)
}

//! One synchronous batched decode step: row i evals tokens[i] at sessions[i]'s position; each
//! session advances by one. Sessions must be distinct, same-geometry (one model, equal cache
//! sizes, uniform KV dtype); B == 1, q4_0, non-std attention, or classic rope fall back to forward().
def eval_batch_(t : Model; var ws : BatchWorkspace; var sessions : array<Session?>; tokens : array<int64>) {   // nolint:LINT014 — rows mutate THROUGH the pointers; a const array constifies the pointees / nolint:STYLE037 — one decode step: workspace grow, rope rows, override-or-loop, then the final classifier scatter; the phases share ws and must stay in order
    let c = t.config
    let nrows = long_length(sessions)
    if (nrows <= 0l) {
        return
    }
    if (long_length(tokens) != nrows) {
        panic("eval_batch: {long_length(sessions)} sessions but {long_length(tokens)} tokens — one token per session")
    }
    for (i in range64(nrows)) {
        if (sessions[i] == null) {
            panic("eval_batch: sessions[{i}] is null")
        }
        if (sessions[i].n_past + 1l > c.seq_len) {
            panic("eval_batch: sessions[{i}] is full (n_past={sessions[i].n_past}, seq_len={c.seq_len})")
        }
        for (j in range64(i)) {
            if (sessions[i] == sessions[j]) {
                panic("eval_batch: sessions[{j}] and sessions[{i}] are the same session — rows must be distinct")
            }
        }
    }
    var s0 = sessions[0]
    for (i in range64(1l, nrows)) {
        let sp = sessions[i]
        if (sp.kv_dtype_k != s0.kv_dtype_k || sp.kv_dtype_v != s0.kv_dtype_v) {
            panic("eval_batch: sessions[{i}] KV dtype differs from sessions[0] — the batch must be uniform")
        }
        if (sp.kv_pool != s0.kv_pool) {
            panic("eval_batch: sessions[{i}] KV pool differs from sessions[0] — one pool (or all-flat) per batch")
        }
        // long_length: a full-seq 1B's flat f16 cache is exactly 2^31 bytes — length() throws
        if (s0.kv_pool == null && (long_length(sp.key_cache) != long_length(s0.key_cache) || long_length(sp.value_cache) != long_length(s0.value_cache))) {
            panic("eval_batch: sessions[{i}] cache size differs from sessions[0] — same model + same seq_len required")
        }
    }
    // per-row fallback keeps every configuration correct: B == 1 keeps the fused decode chain, q4_0
    // has no batched kernels, non-std arches have no batch attention worker, and the classic rope
    // path can't express per-row positions
    if (nrows == 1l || t.quant == QuantMode.q4_0 || !t.blocks.attn_is_std || !g_use_rope_table) {
        for (i in range64(nrows)) {
            var sp = sessions[i]
            forward(t, *sp, tokens[i], sp.n_past)
            sp.n_past++
        }
        return
    }
    // snapshot cache geometry onto the scratch Session; kv_ensure runs for every row FIRST because
    // paged growth resizes pool blobs, so no pool pointer may be hoisted until all rows have pages.
    ws.scr.kv_dtype_k = s0.kv_dtype_k
    ws.scr.kv_dtype_v = s0.kv_dtype_v
    ws.scr.kv_signs := s0.kv_signs   // tq4: the rotation signs travel with the dtypes
    ws.scr.kv_bprefix_k := s0.kv_bprefix_k
    ws.scr.kv_bprefix_v := s0.kv_bprefix_v
    ws.scr.kv_pool = s0.kv_pool
    ws.positions |> resize(nrows)
    ws.kbase |> resize(nrows)
    ws.vbase |> resize(nrows)
    for (i in range64(nrows)) {
        var sp = sessions[i]
        kv_ensure(*sp, sp.n_past + 1l)
        ws.positions[i] = sp.n_past
        if (s0.kv_pool == null) {
            unsafe {
                ws.kbase[i] = addr(sp.key_cache[0])
                ws.vbase[i] = addr(sp.value_cache[0])
            }
        } else {
            ws.kbase[i] = null   // paged rows address the pool blobs, hoisted per layer
            ws.vbase[i] = null
        }
    }
    batch_decode_alloc(t, ws.scr, nrows)
    // embed each row's token into the residual stream
    let dim = c.dim
    let ts_embed = ref_time_ticks()
    for (i in range64(nrows)) {
        embed_row(t, tokens[i], unsafe(addr(ws.scr.x_b[i * dim])))
    }
    if (c.embed_scale != 1.0) {  // Gemma2: scale the embedding by sqrt(dim)
        scale_inplace(unsafe(addr(ws.scr.x_b[0])), c.embed_scale, nrows * dim)
    }
    prof_add("embed", ts_embed)
    if (has_ple(c)) {  // gemma-4 E-series: per-(row, layer) side input (token-indexed, position-free)
        let ts_ple = ref_time_ticks()
        ple_pre_prefill(t, ws.scr, tokens, nrows)
        prof_add("ple", ts_ple)
    }
    // per-row RoPE cos/sin table(s) — row i at sessions[i]'s own position
    let ts_rope_build = ref_time_ticks()
    build_rope_table_rows(ws.scr.rope_cos, ws.scr.rope_sin, t, c.rope_freq_base, c.rope_freq_scale,
        ws.positions, nrows, c.rope_dim > 0l ? c.rope_dim : c.head_size, true)
    if (has_dual_rope(c)) {
        let swa_hs = c.head_size_swa > 0l ? c.head_size_swa : c.head_size
        build_rope_table_rows(ws.scr.rope_cos_swa, ws.scr.rope_sin_swa, t, c.rope_freq_base_swa,
            c.rope_freq_scale_swa, ws.positions, nrows, swa_hs, !c.rope_ff_global_only)
    }
    prof_add("rope_build", ts_rope_build)
    // layer stack: batched attention + the arch's batched FFN. An active batch-decode override may
    // claim the whole stack (false = declined, fall back to the per-layer loop below).
    var stack_done = false
    batch_decode_logits_reset()
    if (batch_decode_override_armed()) {
        stack_done = batch_decode_override_invoke(t, ws, sessions, nrows)
    }
    if (!stack_done) {
        blob_only_panic(t, "batch-decode stack")
        for (l in range64(c.n_layers)) {
            attention_batch_decode(t, ws, sessions, l, nrows)
            t.blocks.ffn_prefill(t, ws.scr, l, nrows)
        }
    }
    // final rmsnorm + classifier over ALL rows, then per-row softcap/pins scattered per session
    let ts_final = ref_time_ticks()
    let vocab = c.vocab_size
    if (!(stack_done && batch_decode_logits_done())) {
        blob_only_panic(t, "batch-decode final-norm/classifier step")
        rms_batch(ws.scr.xb_b, ws.scr.x_b, t, t.rms_final_off, dim, nrows)
        ws.logits_b |> resize(nrows * vocab)
        let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
        if (c.shared_weights && !t.cls_q8 && cls_fmt == KqFmt.q8) {
            // tied weights (fp32/q4_0 loads): the classifier IS the fp32 embedding table
            mm_fblob_b(ws.logits_b, t, t.tok_emb_off, ws.scr.xb_b, dim, vocab, nrows)
        } elif (cls_fmt != KqFmt.q8) {
            mm_b_kq(ws.scr, ws.logits_b, t, cls_fmt, t.wcls_off, ws.scr.xb_b, dim, vocab, nrows)
        } else {
            mm_b(ws.logits_b, t, t.wcls_off, ws.scr.xb_b, ws.scr.xqb, ws.scr.xsb, dim, vocab, nrows)
        }
        // per-row softcap + scatter into each session's logits + suppress pins, ONE row-parallel
        // dispatch — the serial tail here was most of a ~6ms all-lane hole every step at B=8
        // (B x vocab ≈ 5MB of copy). Rows are disjoint (row i writes only sessions[i].logits).
        ws.lrow |> resize(int(nrows))
        unsafe {
            for (i in range64(nrows)) {
                ws.lrow[i] = addr(sessions[i].logits[0])
            }
            var lbp = addr(ws.logits_b[0])
            var lrp = addr(ws.lrow[0])
            let softcap = c.final_logit_softcap
            maybe_parallel_for(0, int(nrows), min(int(nrows), get_dispatch_lanes())) $(rb, re) {
                unsafe {
                    for (i in range(rb, re)) {
                        var src = lbp + int64(i) * vocab
                        if (softcap > 0.0) {  // Gemma2: soft-cap the output logits per row
                            softcap4(src, vocab, softcap)
                        }
                        copy_floats(lrp[i], src, vocab)
                    }
                }
            }
        }
    }
    for (i in range64(nrows)) {
        var sp = sessions[i]
        for (id in t.suppress) {  // gemma4: pin the checkpoint's suppressed token ids to -inf
            sp.logits[id] = -1.0e30
        }
        sp.n_past++
    }
    prof_add("final", ts_final)
}
