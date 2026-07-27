options gen2
options _comment_hygiene = true

module dasllama_metal_llama shared public

require math
require daslib/perf_lint  // PERF026-028: the hot-path contracts below
require daslib/fio
require dasllama/dasllama_env  // env_int/env_flag/env_str: the shared typed readers + the knob registry
require metal/das_metal_boost
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_metal_prefill
require dasllama/dasllama_metal_common public
require dasllama/dasllama_metal_kernels public

// The dasMetal GPU-resident DECODE path (the emission arc's P2 driver + P3 chase shape) — the
// mirror image of dasllama_metal_prefill: one command buffer per token step runs the whole llama
// layer stack on the GPU. Decode is GEMV-bandwidth-bound (one token streams every weight once)
// AND launch-gap-bound (~4.3us per serial dispatch), so the layer runs 9 fused dispatches:
//   fused QKV / W1|W3 GEMVs   the P1-lab-proven single-simdgroup-per-row Q8 GEMV
//                             (metal_q8_gemv_msl, f32 activations) over CONCATENATED weight
//                             regions (upload_region_cat3) into one output buffer each —
//                             downstream dispatches bind at byte offsets
//   metal_sq_attn16/32        single-query attention over the KV mirror, one threadgroup per
//                             (stream, head), simdgroup-cooperative coalesced K scoring; depth
//                             > 128 rows takes metal_sq_attn_part16/32 + metal_sq_attn_comb
//                             (chunked split-K: occupancy scales with depth)
//   metal_rope_store16/32     fused rope + KV-row store: q ropes in place, k ropes straight
//                             into the mirror with cvt_f32_to_f16's exact ±65504 clamp (CPU
//                             fallback steps read byte-identical KV), v copies alongside
//   metal_add_rms             fused residual add + rmsnorm (the trailing one feeds the NEXT
//                             layer's attention norm; layer 0 gets a prologue rms)
// rmsnorm / add / swiglu / the classifier GEMV reuse the prefill module's MSL sources verbatim
// (own PSOs, zero copy drift).
//
// GPU-resident KV = a lazy LOGICAL mirror per session, keyed by Session.uid (address keying is
// unsound: scheduler churn ABAs same-geometry sessions, paged sessions have no flat base,
// kv_pool_grow reallocs blobs in place, Stream erase memmoves Sessions). The CPU session cache
// stays the source of truth: uploads gather logical rows [watermark, pos) through kv_runs,
// pos < watermark = rewind detected (the ASR release_kv_pages + n_past = 0 pattern) → re-upload
// from 0, and after each step the GPU's row bytes memcpy VERBATIM back into the CPU cache (the
// CPU never re-encodes — mirror dtype == session dtype by the f16/f32 gate). Flat and paged
// sessions are the same driver code: the GPU kernels only ever see a dense logical slab.


//! Release every GPU object the driver family holds — mirrors, weight regions, pools, PSOs,
//! queue, device (the tests' leak-gate rail, same contract as metal_prefill_shutdown). Lazily
//! re-initializes on the next served step; all mirrors rebuild from the CPU caches on demand.
def public metal_decode_shutdown {
    discard_pre()
    finish_pending_step()   // land any in-flight pipelined step before tearing down
    g_lp_err = false
    metal_kernels_release()
    metal_common_release()
}

// what each decode path's kernels implement beyond the base llama dense block — grows wave by wave.
let DECODE_NEEDS_OK = (MetalNeed.neox | MetalNeed.qkv_bias | MetalNeed.qk_norm | MetalNeed.qd_neq_dim |
    MetalNeed.softcap | MetalNeed.sliding | MetalNeed.pre_post_norm | MetalNeed.geglu |
    MetalNeed.v_norm | MetalNeed.attn_scale | MetalNeed.out_scale |
    MetalNeed.sinks | MetalNeed.out_bias | MetalNeed.partial_rope | MetalNeed.q_gated |
    MetalNeed.ple)
// q_gated/partial_rope only occur on hybrid models (attn_is_std false), which batch graph-declines;
// ple (E-series) has no batch arm either — batched steps decline `feature` and fall back per-row
let BATCH_NEEDS_OK = DECODE_NEEDS_OK & ~MetalNeed.ple   // clear ple explicitly (XOR would re-add it if DECODE ever drops it)

def private decode_shape_decline(t : Model; needs_ok : MetalNeed; allow_moe : bool = false; allow_kv_share : bool = false) : MetalDecodeDecline {
    if (t.quant != QuantMode.q8) {
        return MetalDecodeDecline.quant_mode
    }
    if (!kq_load_gpu_supported(t)) {   // a fmt without a GPU kernel (the q4_0 QAT tier, ...) must not dispatch
        return MetalDecodeDecline.kquant_native
    }
    // non-std graphs decline; MoE (Wave C) + deltanet hybrid (Wave D) serve when their kernels cover it
    if ((!t.blocks.attn_is_std && !(t.config.hybrid_deltanet && dn_metal_ok(t))) ||
            (!t.blocks.ffn_is_dense && !(allow_moe && moe_metal_ok(t)))) {
        return MetalDecodeDecline.graph
    }
    if (kernel_backend_needs_repack(active_kernel_backend())) {
        return MetalDecodeDecline.backend_repack
    }
    if (!get_rope_table_mode()) {
        return MetalDecodeDecline.rope_mode
    }
    if (metal_missing_needs(t, needs_ok) != default<MetalNeed>) {
        return MetalDecodeDecline.feature
    }
    let c = t.config
    let hidden = layer_hidden(t, 0l)
    // sliding masking lives on the CHUNKED path only — window >= 128 (the single-form depth ceiling's clamp max) guarantees the single form never sees a masked row; decline tighter
    if (((c.dim % 32l) != 0l || (hidden % 32l) != 0l) ||
            (c.sliding_window > 0l && c.sliding_window < 128l)) {
        return MetalDecodeDecline.shape
    }
    // per-CLASS: whole simdgroups; red[16] + the epl <= 16 q-register hoist cap the threadgroup width at 512 (gemma4's global class)
    for (l in range64(c.n_layers)) {
        if (layer_is_recurrent(c, l)) {   // no KV, no attention tensors — only the FFN uniformity applies
            if (layer_hidden(t, l) != hidden) {
                return MetalDecodeDecline.layers
            }
            continue
        }
        let hs_l = layer_head_size(c, l)
        if ((hs_l % 32l) != 0l || hs_l > 512l || (layer_kv_dim(c, l) % 32l) != 0l) {
            return MetalDecodeDecline.shape
        }
        // E-series KV sharing: only the single-stream driver has the Q-only arm (allow_kv_share)
        if ((t.kv_src[l] != l && !(allow_kv_share && kv_share_ok(t, l))) || layer_hidden(t, l) != hidden) {
            return MetalDecodeDecline.layers
        }
        // a missing attn_v (V = the pre-norm K projection) serves — raw copy without v_norm; both encoded (the CPU mm_qkv shapes)
    }
    return MetalDecodeDecline.none
}

// ... plus the decode-specific gates (uniform f16/f32 KV, attention depth, a real session uid)
[unused_argument(pos)]   // depth retired (M2.4) — pos stays in the gate signature for future clauses
def private decode_decline(t : Model; s : Session; pos : int64) : MetalDecodeDecline {
    // mirror + writeback move raw row bytes, so the session codec must equal the mirror dtype; depth is capacity-bounded by the mirror arena, not gated here. Partial rope has no q8_0/tq4 rope-store arm yet (f16/f32 mirrors only)
    let partial_rope = t.config.rope_dim > 0l && t.config.rope_dim != t.config.head_size
    if (s.kv_dtype_k != s.kv_dtype_v ||
            (s.kv_dtype_k != KVDtype.f16 && s.kv_dtype_k != KVDtype.f32 &&
             s.kv_dtype_k != KVDtype.q8_0 && s.kv_dtype_k != KVDtype.tq4) ||
            (partial_rope && s.kv_dtype_k != KVDtype.f16 && s.kv_dtype_k != KVDtype.f32)) {
        return MetalDecodeDecline.kv_dtype
    }
    if (s.uid == 0ul) {
        return MetalDecodeDecline.no_uid
    }
    let shape = decode_shape_decline(t, DECODE_NEEDS_OK, true, true)
    if (shape != MetalDecodeDecline.none) {
        return shape
    }
    // AFTER the capability gates (the support matrix asserts those) — only a fully-servable planar load declines `planar`
    if (!t.metal_blob) {
        return MetalDecodeDecline.planar
    }
    // forward() built this position's rope row(s) before the hook (dual-rope both classes; partial rope rot/2 wide)
    let rot = t.config.rope_dim > 0l ? t.config.rope_dim : t.config.head_size
    let swa_hs = t.config.head_size_swa > 0l ? t.config.head_size_swa : t.config.head_size
    if (int64(length(s.rope_cos)) < rot / 2l ||
            (has_dual_rope(t.config) && int64(length(s.rope_cos_swa)) < swa_hs / 2l)) {
        return MetalDecodeDecline.rope_rows
    }
    return MetalDecodeDecline.none
}

def private kq_fmt_at(a : array<KqFmt>; l : int64) : KqFmt => empty(a) ? KqFmt.q8 : a[l]

// mint-time servability (register_metal_servable): shape/graph/feature gates only — the planar
// check is a RUNTIME gate. Tied-fp32 classifiers decline: the blob drivers force GPU logits
// and have no fp32-table GEMV.
def private metal_model_servable_impl(t : Model) : bool {
    let cls_fmt = t.config.shared_weights ? t.emb_fmt : t.wcls_fmt
    // tied-fp32 classifier declines (no fp32-table GEMV); NextN serves — prefill stashes mtp_h + warms the slab
    if (t.config.shared_weights && !t.cls_q8 && cls_fmt == KqFmt.q8) {
        return false
    }
    return decode_shape_decline(t, DECODE_NEEDS_OK, true, true) == MetalDecodeDecline.none
}

// Can the speculative chain feed itself on this model? It needs a GPU embed gather for the
// argmax winner: the q8 linear copy (cls_q8), or a tied k6 classifier (every K-quant file on
// the current boards ties a Q6_K token_embd — MetalEmbedK6 gathers straight off the kq planes).
def private spec_cls_capable(t : Model) : bool {
    // recurrent models never chain: a mispredicted spec step already advanced the GPU dn state (no repair)
    return (t.cls_q8 || (t.config.shared_weights && t.emb_fmt == KqFmt.k6)) && t.config.recr_mask == 0ul
}

// One weight site's GEMV — a q8-tagged tensor rides the planar q8 kernels, a kq tensor the
// plane GEMVs (kq_quants_of/kq_scales_of + enc_kq_gemv). yoff places the k/v/w3 output segments.
def private enc_site_gemv(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows, n : int64; bx, by, bn, bd : MetalBuffer?; yoff : uint64 = 0ul) {
    if (fmt == KqFmt.q8) {
        let bw = blob_of(g_dev, t, woff)
        enc_gemv(enc, bw.buf, bw.boff, bx, by, bn, bd, rows, yoff)
    } else {
        let bq = kq_quants_of(g_dev, t, fmt, woff)
        let bs = kq_scales_of(g_dev, t, fmt, woff)
        enc_kq_gemv(enc, fmt, bq.buf, bq.qoff, bs.buf, bs.soff, bs.doff, bx, by, bn, bd, rows, n, yoff)
    }
}

// One kq weight site in a batched step. B <= 8: nrows separate single-stream plane GEMVs (the
// kq GEMV is ALU-bound, so this matches the lcpp kq batch wall). B >= 9: the kq mul_mm twin over
// the M-padded x panel. Disjoint y rows/columns: the caller's one barrier covers the whole group.
def private enc_kq_site_b(enc : MetalComputeEncoder?; t : Model; fmt : KqFmt; woff, rows, n : int64;
                          bx : MetalBuffer?; xstride : int64; by : MetalBuffer?; ystride, ycol : int64;
                          bn, bd, bys, bnr : MetalBuffer?; nrows, mp : int64) {
    let bq = kq_quants_of(g_dev, t, fmt, woff)
    let bs = kq_scales_of(g_dev, t, fmt, woff)
    if (nrows > 8l) {
        enc_kq_gemm_mm_b(enc, fmt, bq.buf, bq.qoff, bs.buf, bs.soff, bs.doff, bx, by, uint64(ycol * 4l), bn, bys, mp, rows)
        return
    }
    // B = 2..8: the ext small-batch twins — one weight pass per R-column group instead of B per-stream passes (-25..-40% at the 3B sites)
    if (nrows >= 2l && bnr != null) {
        assert(xstride == n)
        enc_kq_mvb(enc, fmt, bq.buf, bq.qoff, bs.buf, bs.soff, bs.doff, bx, by, uint64(ycol * 4l), bn, bd, bys, bnr, rows, nrows)
        return
    }
    for (b in range64(nrows)) {
        enc_kq_gemv(enc, fmt, bq.buf, bq.qoff, bs.buf, bs.soff, bs.doff, bx, by, bn, bd, rows, n,
            uint64((b * ystride + ycol) * 4l), uint64(b * xstride * 4l))
    }
}

// Evict least-recently-used mirrors (never `keep`, never a pinned batch row) until the resident
// bytes fit the cap.

// ===== the per-step resource bundle + encode-ahead pipelining =====

// Everything one step's command buffer owns. The encode-ahead rail (g_pre) keeps ONE pre-encoded
// for the predicted next position; forward() pokes the token-dependent bytes (embed row + rope
// row) and COMMITS — encode cost overlaps the PREVIOUS step's GPU run instead of the per-token wall.
struct private StepRes {
    valid : bool
    pos : int64
    uid : uint64
    kdt : KVDtype         // the session/mirror KV codec (kdt == vdt gated)
    s16 : bool
    gpu_logits : bool
    chunked : bool
    spec : bool           // opened with GPU argmax + embed gather — no CPU poke, runs immediately
    committed : bool      // spec pres commit at encode; poked pres commit at poke time (a cb
                          // parked committed across an idle gap trips the GPU watchdog)
    trace_mode : int64    // serving path for the step trace (0 slow / 1 poked / 2 spec)
    t_commit : int64      // trace-clock stamp at this step's cb commit (armed traces only)
    cb : MetalCommandBuffer?
    btok : MetalBuffer?   // the argmax winner (spec steps; read back to verify the prediction)
    koff : uint64         // mirror arena offsets captured at encode time; arena_epoch pins the
    voff : uint64         // BUFFER those offsets index (a grow-rebuild frees it — stale pre-steps discard)
    arena_epoch : int64
    cap : int64
    bx : MetalBuffer?
    bxb : MetalBuffer?
    bxb2 : MetalBuffer?
    bqkv : MetalBuffer?
    bh12 : MetalBuffer?
    bcos : MetalBuffer?
    bsin : MetalBuffer?
    bcos_swa : MetalBuffer?   // the sliding class's rope row (dual-rope configs — gemma3/4)
    bsin_swa : MetalBuffer?
    bpart : MetalBuffer?
    bxf : MetalBuffer?
    blog : MetalBuffer?   // pooled staging — only when bnc is unavailable
    bnc : MetalBuffer?    // zero-copy logits: the session's wrapped storage (registry-owned, never released here)
    draft : bool          // MTP draft-head step (metal_mtp_draft_forward) — bcat/u_dim2 live
    bcat : MetalBuffer?   // draft eh_proj input: [enorm(embed) ; hnorm(mtp_h)] (2*dim)
    u_dim2 : MetalBuffer?
    nrows : int64         // activation-row count (1 normal, 2 = MTP verify); byte sizes pre-scaled
    verify : bool         // B=2 same-slab MTP verify step — the u_two32/total uniforms live
    u_two32 : MetalBuffer?
    u_total_dim : MetalBuffer?
    u_total_h : MetalBuffer?
    u_total_qd : MetalBuffer?
    u_dim : MetalBuffer?
    u_qd : MetalBuffer?
    u_kvd : MetalBuffer?
    u_qkvd : MetalBuffer?
    u_hidden : MetalBuffer?
    u_hs : MetalBuffer?
    u_kvm : MetalBuffer?
    u_nh : MetalBuffer?
    u_cnt : MetalBuffer?
    u_npq : MetalBuffer?
    u_npall : MetalBuffer?
    u_scale : MetalBuffer?
    u_eps : MetalBuffer?
    u_vocab : MetalBuffer?
    u_nch : MetalBuffer?
    u_nblk : MetalBuffer?  // kv_dim/32 — the block-codec kernels' row-width uniform (q8_0 only)
    u_neox : MetalBuffer?  // rope pairing mode (0 = NORM adjacent, 1 = NEOX j/j+half)
    u_hasb : MetalBuffer?  // QKV bias present (the rope-store/qkv_rs epilogue reads the bias plane)
    u_hasn : MetalBuffer?  // QK-norm present (the fused H-form rope-store's reduce gate)
    u_softcap : MetalBuffer?  // attention-logit soft cap (0 = off) — every attention dispatch binds it
    u_zero32 : MetalBuffer?   // 0u — the window/chlo bind for global (non-sliding) layers
    u_winsl : MetalBuffer?    // sliding span (sliding models only; sliding layers bind it as `window`)
    u_chlo : MetalBuffer?     // first live chunk = wstart/64 (sliding models only; the comb skip)
    u_flcap : MetalBuffer?    // final-logit soft cap (gpu_logits x softcap models only)
    // the SLIDING-class uniform twins (hetero models — gemma4's 16q/8kv/256 vs 16q/1kv/512;
    // uniform models leave these null and every layer binds the base set)
    hetero : bool
    u_hs_sw : MetalBuffer?
    u_kvd_sw : MetalBuffer?
    u_qd_sw : MetalBuffer?
    u_qkvd_sw : MetalBuffer?
    u_kvm_sw : MetalBuffer?
    u_nblk_sw : MetalBuffer?
    u_scale_sw : MetalBuffer?
    u_npq_sw : MetalBuffer?
    u_npall_sw : MetalBuffer?
    u_voffe : MetalBuffer?     // v segment element offset (qd + kvd) — the v_norm/V-from-K sites
    u_voffe_sw : MetalBuffer?
    u_embscale : MetalBuffer?  // sqrt(dim) embed scale on the spec chain's GPU gather (gemma)
    bsupp : MetalBuffer?       // suppressed token ids (u32) + count — the gemma4 logits pin
    u_nsupp : MetalBuffer?
    // MoE routing (Wave C) — moe steps only; bh12 sizes to max(2*hidden, 2*k*nfe)
    moe : bool
    bmoe_lg : MetalBuffer?     // [ne] router logits
    bmoe_sel : MetalBuffer?    // [k idx u32][k w f32] — the select kernel's output pair
    bmoe_dn : MetalBuffer?     // [k x dim] per-slot down outputs (combine's input)
    u_moe_ne : MetalBuffer?
    u_moe_k : MetalBuffer?
    u_moe_nfe : MetalBuffer?
    u_moe_knfe : MetalBuffer?  // k * n_ff_exp — the swiglu span over the slot rows
    u_moe_ss : MetalBuffer?    // sel stride per stream (2k) — the verify's two-stream forms
    u_moe_kdim : MetalBuffer?  // k * dim — per-stream bmoe_dn stride
    u_moe_tot : MetalBuffer?   // nrows * knfe — the two-stream swiglu span
    u_moe_eblk : MetalBuffer?  // q8 blocks per expert plane (ege / 32)
    u_moe_esb : MetalBuffer?   // kq superblocks per expert plane (ege / 256)
    u_moe_xs_dn : MetalBuffer? // the down site's per-slot x stride (nfe)
    bh12s : MetalBuffer?       // the dense-shared/shexp branch's OWN gate|up panel — off bh12 so
                               // the schedule can run the shared chain under the routed chain
    bytes_h12s : uint64
    bmoe_rt : MetalBuffer?     // gemma4: router-input row, reused as the routed-branch acc
    u_moe_invd : MetalBuffer?  // gemma4: 1/sqrt(dim) router scale
    u_moe_gmode : MetalBuffer? // select gating mode (1 = gpt-oss softmax_weight)
    u_moe_nsh : MetalBuffer?   // shexp width (qwen35moe; null = no shared expert)
    u_hass : MetalBuffer?      // attention sinks present (binds hass on the attn dispatches)
    // gemma-4 E-series per-layer embeddings — ple steps only; the CPU pre-step's side input
    // (s.ple_inp, [layers x ple]) pokes into bple each step alongside the embed row
    ple : bool
    bple : MetalBuffer?        // the token's per-layer side input rows (poked, GPU-read-only)
    u_ple : MetalBuffer?       // n_embd_per_layer
    bytes_ple : uint64
    // deltanet (Wave D) — recurrent-layer scratch + the gated-attention twins; dn steps only
    dn : bool
    bdn_state : MetalBuffer?   // the session's DnMirror buffer (mirror-owned, never released here)
    dn_state_base : uint64     // live region's state base inside bdn_state
    dn_conv_base : uint64      // live region's conv-history base inside bdn_state
    dn_state_alt : uint64      // shadow region bases — the MTP verify's dual-store targets
    dn_conv_alt : uint64
    bdn_qkv : MetalBuffer?     // [cd] fused qkv projection row
    bdn_cv : MetalBuffer?      // [cd] conv output (l2-normed in place)
    bdn_z : MetalBuffer?       // [di] z gate row
    bdn_b : MetalBuffer?       // [nvh] raw beta projections
    bdn_g : MetalBuffer?       // [nvh] raw alpha projections
    bdn_o : MetalBuffer?       // [di] delta-rule output (gated in place)
    bdn_qg : MetalBuffer?      // [2*qd] packed [q|gate] projection (gated-attention layers)
    bdn_gate : MetalBuffer?    // [qd] deinterleaved sigmoid-gate rows
    u_cd : MetalBuffer?
    u_dn_kd : MetalBuffer?
    u_ds : MetalBuffer?
    u_nvh : MetalBuffer?
    u_nkh : MetalBuffer?
    u_di : MetalBuffer?
    u_dconv : MetalBuffer?
    u_qscale : MetalBuffer?
    u_one32 : MetalBuffer?     // constant 1 (dn npos, the shexp gate's 1-row GEMV) — unconditional
    u_dn_qd2 : MetalBuffer?    // 2*qd — the packed [q|gate] GEMV width
    u_rot : MetalBuffer?       // partial rotary width (null = full; rope binds default to hs)
    bytes_dn_cd : uint64
    bytes_dn_di : uint64
    bytes_dn_h : uint64        // nvh floats (β / α rows)
    bytes_moe_lg : uint64
    bytes_moe_sel : uint64
    bytes_moe_dn : uint64
    bytes_supp : uint64
    bytes_tab_sw : uint64      // the swa rope pair's row bytes (hetero: hs_sw/2 floats != bytes_tab)
    bytes_x : uint64
    bytes_q : uint64
    bytes_xb : uint64      // max(dim, qd) — bxb holds dim-wide norm outs AND the qd-wide attention out
    bytes_qkv : uint64
    bytes_h2 : uint64
    bytes_tab : uint64
    bytes_part : uint64
    bytes_log : uint64
}

var private g_pre : StepRes                    // the pre-encoded next step, when valid
// the driver-built rope row for the PREDICTED next position (bit-identical to forward()'s)
var private g_rope_cos_next : array<float>
var private g_rope_sin_next : array<float>
var private g_rope_cos_next_swa : array<float>   // the sliding class's twin (dual-rope configs)
var private g_rope_sin_next_swa : array<float>

// geometry + pooled buffers + uniforms for one step at `pos` — no input bytes yet (poke_step).
// Hetero models (gemma4) carry TWO attention classes (global = config fields, sliding = _swa);
// buffers size to the class maxima and each layer binds its class's uniform set.
def private acquire_step(t : Model; s : Session; pos : int64; koff, voff : uint64; cap : int64; draft : bool = false; nrows : int64 = 1l) : StepRes {
    let c = t.config
    let dim = c.dim
    let head_size = c.head_size
    let n_heads = c.n_heads
    let qd = n_heads * head_size
    let kv_dim = c.n_kv_heads * head_size
    let hidden = layer_hidden(t, 0l)
    let qkvd = qd + 2l * kv_dim
    let hs_sw = c.head_size_swa > 0l ? c.head_size_swa : head_size
    let kvh_sw = c.n_kv_heads_swa > 0l ? c.n_kv_heads_swa : c.n_kv_heads
    let qd_sw = n_heads * hs_sw
    let kvd_sw = kvh_sw * hs_sw
    let qkvd_sw = qd_sw + 2l * kvd_sw
    let hetero = hs_sw != head_size || kvh_sw != c.n_kv_heads
    let hs_mx = max(head_size, hs_sw)
    let qd_mx = max(qd, qd_sw)
    let qkvd_mx = max(qkvd, qkvd_sw)
    let moe = !t.blocks.ffn_is_dense
    let knfe = moe ? c.n_expert_used * c.n_ff_exp : 0l
    let nsh = moe ? c.n_ff_shexp : 0l
    let h2 = max(2l * hidden, max(2l * knfe, 2l * nsh))   // moe: bh12 holds k gate rows | k up rows (+ the shexp pair)
    let rot = c.rope_dim > 0l ? c.rope_dim : head_size   // partial-rope tables are rot/2 wide
    var r = StepRes(valid = true, pos = pos, uid = s.uid, nrows = nrows,
        kdt = s.kv_dtype_k, s16 = t.wscale_f16, hetero = hetero, moe = moe,
        dn = c.recr_mask != 0ul,
        koff = koff, voff = voff, cap = cap, arena_epoch = g_arena_epoch,
        bytes_x = uint64(nrows * dim * 4l), bytes_q = uint64(nrows * qd_mx * 4l), bytes_xb = uint64(nrows * max(dim, qd_mx) * 4l),
        bytes_qkv = uint64(nrows * qkvd_mx * 4l),
        bytes_h2 = uint64(nrows * h2 * 4l), bytes_tab = uint64(nrows * (rot / 2l) * 4l),
        bytes_tab_sw = uint64(nrows * (hs_sw / 2l) * 4l),
        bytes_log = uint64(nrows * c.vocab_size * 4l))
    r.bx = pool_acquire(g_pool, g_dev, r.bytes_x)
    r.bxb = pool_acquire(g_pool, g_dev, r.bytes_xb)   // rms out (dim) AND attention out (qd)
    r.bxb2 = pool_acquire(g_pool, g_dev, r.bytes_x)
    r.bqkv = pool_acquire(g_pool, g_dev, r.bytes_qkv)   // q | roped-in-place q, k, raw v
    r.bh12 = pool_acquire(g_pool, g_dev, r.bytes_h2)    // w1 gate | swiglu in place, w3 up
    r.bcos = pool_acquire_untracked(g_upool, g_dev, r.bytes_tab)
    r.bsin = pool_acquire_untracked(g_upool, g_dev, r.bytes_tab)
    if (has_dual_rope(c)) {
        r.bcos_swa = pool_acquire_untracked(g_upool, g_dev, r.bytes_tab_sw)
        r.bsin_swa = pool_acquire_untracked(g_upool, g_dev, r.bytes_tab_sw)
    }
    r.u_dim = uniform_u32(uint(dim))
    r.u_qd = uniform_u32(uint(qd))
    r.u_kvd = uniform_u32(uint(kv_dim))
    r.u_qkvd = uniform_u32(uint(qkvd))
    r.u_hidden = uniform_u32(uint(hidden))
    r.u_hs = uniform_u32(uint(head_size))
    r.u_kvm = uniform_u32(uint(n_heads / c.n_kv_heads))
    r.u_nh = uniform_u32(uint(n_heads))
    r.u_cnt = uniform_u32(uint(pos + 1l))
    r.u_npq = uniform_u32(uint(qd / 2l))
    r.u_npall = uniform_u32(uint((qd + kv_dim) / 2l))
    // attn_scale overrides the 1/sqrt(hs) score scale (gemma4: 1.0 — QK-norm replaces it)
    r.u_scale = uniform_f32(c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size)))
    r.u_eps = uniform_f32(c.norm_eps)
    if (hetero) {
        r.u_hs_sw = uniform_u32(uint(hs_sw))
        r.u_kvd_sw = uniform_u32(uint(kvd_sw))
        r.u_qd_sw = uniform_u32(uint(qd_sw))
        r.u_qkvd_sw = uniform_u32(uint(qkvd_sw))
        r.u_kvm_sw = uniform_u32(uint(n_heads / kvh_sw))
        r.u_scale_sw = uniform_f32(c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(hs_sw)))
        r.u_npq_sw = uniform_u32(uint(qd_sw / 2l))
        r.u_npall_sw = uniform_u32(uint((qd_sw + kvd_sw) / 2l))
    }
    if (c.v_norm || c.v_from_k_mask != 0ul) {
        r.u_voffe = uniform_u32(uint(qd + kv_dim))
        if (hetero) {
            r.u_voffe_sw = uniform_u32(uint(qd_sw + kvd_sw))
        }
    }
    r.u_neox = uniform_u32(c.rope_neox ? 1u : 0u)
    r.u_hasb = uniform_u32(c.attn_qkv_bias ? 1u : 0u)
    r.u_hasn = uniform_u32(c.qk_norm ? 1u : 0u)
    r.u_softcap = uniform_f32(c.attn_logit_softcap)
    r.u_zero32 = uniform_u32(0u)
    r.u_one32 = uniform_u32(1u)
    if (c.sliding_window > 0l) {
        let wstart = pos + 1l > c.sliding_window ? pos + 1l - c.sliding_window : 0l
        r.u_winsl = uniform_u32(uint(c.sliding_window))
        r.u_chlo = uniform_u32(uint(wstart / ATTN_CHUNK_ROWS))
    }
    if (r.kdt == KVDtype.q8_0 || r.kdt == KVDtype.tq4) {
        r.u_nblk = uniform_u32(uint(kv_dim / 32l))
        if (hetero) {
            r.u_nblk_sw = uniform_u32(uint(kvd_sw / 32l))
        }
    }
    if (moe) {
        let ege = dim * c.n_ff_exp
        r.bytes_moe_lg = uint64(nrows * c.n_expert * 4l)
        r.bytes_moe_sel = uint64(nrows) * 128ul
        r.bytes_moe_dn = uint64(nrows * c.n_expert_used * dim * 4l)
        r.bmoe_lg = pool_acquire(g_pool, g_dev, r.bytes_moe_lg)
        r.bmoe_sel = pool_acquire(g_pool, g_dev, r.bytes_moe_sel)   // k <= 16: [k idx][k w] per stream
        r.bmoe_dn = pool_acquire(g_pool, g_dev, r.bytes_moe_dn)
        r.u_moe_ne = uniform_u32(uint(c.n_expert))
        r.u_moe_k = uniform_u32(uint(c.n_expert_used))
        r.u_moe_nfe = uniform_u32(uint(c.n_ff_exp))
        r.u_moe_knfe = uniform_u32(uint(knfe))
        r.u_moe_ss = uniform_u32(uint(2l * c.n_expert_used))
        r.u_moe_kdim = uniform_u32(uint(c.n_expert_used * dim))
        r.u_moe_tot = uniform_u32(uint(nrows * knfe))
        r.u_moe_eblk = uniform_u32(uint(ege / 32l))
        r.u_moe_esb = uniform_u32(uint(ege / 256l))
        r.u_moe_xs_dn = uniform_u32(uint(c.n_ff_exp))
        if (c.moe_dense_shexp) {   // gemma4: router-input scratch + the 1/sqrt(dim) scale
            r.bmoe_rt = pool_acquire(g_pool, g_dev, r.bytes_x)
            r.u_moe_invd = uniform_f32(1.0 / sqrt(float(dim)))
        }
        if (nsh > 0l) {
            r.u_moe_nsh = uniform_u32(uint(nsh))
        }
        if (c.moe_dense_shexp || nsh > 0l) {
            r.bytes_h12s = uint64(nrows * max(2l * hidden, 2l * nsh) * 4l)
            r.bh12s = pool_acquire(g_pool, g_dev, r.bytes_h12s)
        }
        r.u_moe_gmode = uniform_u32(c.moe_gate == MoeGate.softmax_weight ? 1u : (c.norm_topk_prob ? 0u : 2u))
    }
    if (c.attn_sinks) {
        r.u_hass = uniform_u32(1u)
    }
    if (r.dn) {
        let cd = dn_conv_dim(c)
        let di = c.ssm_d_inner
        r.bytes_dn_cd = uint64(nrows * cd * 4l)
        r.bytes_dn_di = uint64(nrows * di * 4l)
        r.bytes_dn_h = uint64(nrows * c.ssm_dt_rank * 4l)
        r.bdn_qkv = pool_acquire(g_pool, g_dev, r.bytes_dn_cd)
        r.bdn_cv = pool_acquire(g_pool, g_dev, r.bytes_dn_cd)
        r.bdn_z = pool_acquire(g_pool, g_dev, r.bytes_dn_di)
        r.bdn_o = pool_acquire(g_pool, g_dev, r.bytes_dn_di)
        r.bdn_b = pool_acquire(g_pool, g_dev, r.bytes_dn_h)
        r.bdn_g = pool_acquire(g_pool, g_dev, r.bytes_dn_h)
        r.u_cd = uniform_u32(uint(cd))
        r.u_dn_kd = uniform_u32(uint(dn_key_dim(c)))
        r.u_ds = uniform_u32(uint(c.ssm_d_state))
        r.u_nvh = uniform_u32(uint(c.ssm_dt_rank))
        r.u_nkh = uniform_u32(uint(c.ssm_n_group))
        r.u_di = uniform_u32(uint(di))
        r.u_dconv = uniform_u32(uint(c.ssm_d_conv))
        r.u_qscale = uniform_f32(1.0 / sqrt(float(c.ssm_d_state)))
    }
    if (c.q_gated) {
        r.bdn_qg = pool_acquire(g_pool, g_dev, 2ul * r.bytes_q)
        r.bdn_gate = pool_acquire(g_pool, g_dev, r.bytes_q)
        r.u_dn_qd2 = uniform_u32(uint(2l * qd))
    }
    if (has_ple(c)) {   // E-series: the poked side-input rows (CPU pre-step) + the ple width uniform
        r.ple = true
        r.bytes_ple = uint64(c.n_layers * c.n_embd_per_layer * 4l)
        r.bple = pool_acquire_untracked(g_upool, g_dev, r.bytes_ple)
        r.u_ple = uniform_u32(uint(c.n_embd_per_layer))
    }
    if (rot != head_size) {
        r.u_rot = uniform_u32(uint(rot))
    }
    // logits-on-GPU (default ON, DASLLAMA_METAL_LOGITS=0 pins the CPU classifier) mirrors the prefill gates
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    let tied_f32 = c.shared_weights && !t.cls_q8 && cls_fmt == KqFmt.q8
    // tied-fp32 stays CPU; blob models FORCE GPU logits (no CPU planes); MTP serves — finish_step stashes mtp_h from bxf
    r.gpu_logits = ((t.metal_blob || (!g_logits_cpu && g_env_logits && empty(g_skip))) &&
        !tied_f32 &&
        t.wcls_off >= 0l)
    if (r.gpu_logits) {
        if (c.final_logit_softcap > 0.0) {   // cap*tanh epilogue rides after the classifier GEMV
            r.u_flcap = uniform_f32(c.final_logit_softcap)
        }
        if (!empty(t.suppress)) {   // gemma4: pin the suppressed ids to -1e30 after the cap
            r.bytes_supp = uint64(int64(length(t.suppress)) * 4l)
            r.bsupp = pool_acquire_untracked(g_upool, g_dev, r.bytes_supp)
            unsafe {
                var sp = reinterpret<uint?>(metal_buffer_contents(r.bsupp))
                for (i in range(length(t.suppress))) {
                    sp[i] = uint(t.suppress[i])
                }
            }
            r.u_nsupp = uniform_u32(uint(length(t.suppress)))
        }
        if (c.embed_scale != 1.0) {   // the spec gather's escale bind (gemma; identity rides g_one)
            r.u_embscale = uniform_f32(c.embed_scale)
        }
        r.bxf = pool_acquire(g_pool, g_dev, r.bytes_x)
        // zero-copy logits write straight into s.logits; the pooled staging is fallback (spec-capable models keep staging too — the speculative chain must only ever touch pooled memory). Multi-row steps (verify) always stage.
        r.bnc = nrows == 1l ? nc_logits_for(g_dev, s) : null
        if (r.bnc == null || spec_cls_capable(t)) {
            r.blog = pool_acquire(g_pool, g_dev, r.bytes_log)
        }
        r.u_vocab = uniform_u32(uint(c.vocab_size))
    }
    // chunked split-K attention above the single-tg depth (128-row chunks, cross-chunk combine)
    r.chunked = pos + 1l > g_attn_single_max
    let nchunks = (pos + 1l + ATTN_CHUNK_ROWS - 1l) / ATTN_CHUNK_ROWS
    r.bytes_part = uint64(nrows * n_heads * nchunks * (hs_mx + 2l) * 4l)
    if (r.chunked) {
        r.bpart = pool_acquire(g_pool, g_dev, r.bytes_part)
        r.u_nch = uniform_u32(uint(nchunks))
    }
    if (draft || nrows > 1l) {   // the verify's warm sub-pass shares the draft's cat/eh_proj plumbing
        r.draft = draft
        r.bcat = pool_acquire(g_pool, g_dev, 2ul * r.bytes_x)
        r.u_dim2 = uniform_u32(uint(2l * dim))
    }
    if (nrows > 1l) {
        r.verify = true
        r.u_two32 = uniform_u32(uint(nrows))
        r.u_total_dim = uniform_u32(uint(nrows * dim))
        r.u_total_h = uniform_u32(uint(nrows * hidden))
        r.u_total_qd = uniform_u32(uint(nrows * qd))
    }
    return r
}

// the token-dependent input bytes — the ONLY step state not known at encode time
def private poke_step(s : Session; r : StepRes) {
    unsafe {
        memcpy(metal_buffer_contents(r.bx), addr < void? >(s.x[0]), int(r.bytes_x))
        memcpy(metal_buffer_contents(r.bcos), addr < void? >(s.rope_cos[0]), int(r.bytes_tab))
        memcpy(metal_buffer_contents(r.bsin), addr < void? >(s.rope_sin[0]), int(r.bytes_tab))
        if (r.bcos_swa != null) {
            memcpy(metal_buffer_contents(r.bcos_swa), addr < void? >(s.rope_cos_swa[0]), int(r.bytes_tab_sw))
            memcpy(metal_buffer_contents(r.bsin_swa), addr < void? >(s.rope_sin_swa[0]), int(r.bytes_tab_sw))
        }
        if (r.ple) {   // forward()'s ple_pre_decode filled s.ple_inp before the hook
            memcpy(metal_buffer_contents(r.bple), addr < void? >(s.ple_inp[0]), int(r.bytes_ple))
        }
    }
}

// one trunk-shaped fused layer (attention/deltanet + ffn/moe) — the decode step's loop body,
// shared with the MTP draft arm (which runs it at l == n_layers). Returns the advanced
// recurrent-slice cursor
def private encode_layer(t : Model; var r : StepRes; enc : MetalComputeEncoder?; l : int64; var dnli : int64&) {
    let c = t.config
    let dim = c.dim
    let n_heads = c.n_heads
    let hidden = layer_hidden(t, 0l)
    let pos = r.pos
    let cap = r.cap
    let f16v = r.kdt == KVDtype.f16
    let scalar_kv = r.kdt == KVDtype.f16 || r.kdt == KVDtype.f32   // block codecs take their own kernels
    let geglu = c.ffn_act != FfnAct.silu     // gemma: gelu gate — the swiglu-fused forms stand down
    let nchunks = (pos + 1l + ATTN_CHUNK_ROWS - 1l) / ATTN_CHUNK_ROWS
    unsafe {
        let recr = layer_is_recurrent(c, l)
        // attention: fused QKV GEMV -> single-query attention -> out-proj -> fused add+norm; K-quant sites (mixed PER TENSOR formats) split into per-tensor plane GEMVs
        let fq = kq_fmt_at(t.wq_fmt, l)
        let fk = kq_fmt_at(t.wk_fmt, l)
        let fv = kq_fmt_at(t.wv_fmt, l)
        let qkv_q8 = fq == KqFmt.q8 && fk == KqFmt.q8 && fv == KqFmt.q8
        // sliding layers: the swa rope row + the window/chlo binds below; hetero models (gemma4) also swap the whole class geometry
        let lsl = layer_is_sliding(c, l)
        let het = r.hetero && lsl
        var bcos_l = lsl && r.bcos_swa != null ? r.bcos_swa : r.bcos
        var bsin_l = lsl && r.bsin_swa != null ? r.bsin_swa : r.bsin
        let head_size = layer_head_size(c, l)
        let kv_dim = layer_kv_dim(c, l)
        let qd = n_heads * head_size
        let qkvd = qd + 2l * kv_dim
        let rowb = kv_row_bytes(r.kdt, kv_dim)
        let lkoff = r.koff + uint64(mirror_lbase(t, r.kdt, cap, l))
        let lvoff = r.voff + uint64(mirror_lbase(t, r.kdt, cap, l))
        var u_hs_l = het ? r.u_hs_sw : r.u_hs
        var u_kvd_l = het ? r.u_kvd_sw : r.u_kvd
        var u_qd_l = het ? r.u_qd_sw : r.u_qd
        var u_qkvd_l = het ? r.u_qkvd_sw : r.u_qkvd
        var u_kvm_l = het ? r.u_kvm_sw : r.u_kvm
        var u_nblk_l = het ? r.u_nblk_sw : r.u_nblk
        var u_scale_l = het ? r.u_scale_sw : r.u_scale
        var u_npq_l = het ? r.u_npq_sw : r.u_npq
        var u_npall_l = het ? r.u_npall_sw : r.u_npall
        // E-series shared-KV layer: Q-only — lkoff/lvoff alias the source slab via kv_row_prefix
        let kvsh = kv_shared_layer(t, l)
        // qk_norm needs the head-wide prepass between GEMV and rope — the fused form stands down (hetero too: its row map bakes one class geometry); q_gated packs [q|gate] (deint first)
        let fused_qkv = scalar_kv && qkv_q8 && !c.qk_norm && !r.hetero && !c.q_gated && !kvsh
        if (recr) {
            // deltanet layer (Wave D): projections -> conv+SiLU -> q/k L2 -> delta rule -> gated out-norm -> out-proj
            let cd = dn_conv_dim(c)
            let di = c.ssm_d_inner
            let nvh = c.ssm_dt_rank
            if (g_skip != "gemv") {
                enc_site_gemv(enc, t, KqFmt.q8, t.dnqkv_offs[l], cd, dim, r.bxb, r.bdn_qkv, r.u_dim, r.u_cd)
                enc_site_gemv(enc, t, KqFmt.q8, t.dngate_offs[l], di, dim, r.bxb, r.bdn_z, r.u_dim, r.u_di)
                if (t.dn_ba_f32) {
                    // F32-on-disk β/α (35B): fblob slabs ride the router f32 GEMV
                    var bw_bt = upload_region(addr < void? >(t.fblob[t.dn_betaf_off + l * dim * nvh]), uint64(dim * nvh * 4l))
                    enc_moe_router(enc, bw_bt, r.bxb, r.bdn_b, r.u_dim, r.u_nvh, r.u_dim, g_one, g_zero, nvh, 1l)
                    var bw_al = upload_region(addr < void? >(t.fblob[t.dn_alphaf_off + l * dim * nvh]), uint64(dim * nvh * 4l))
                    enc_moe_router(enc, bw_al, r.bxb, r.bdn_g, r.u_dim, r.u_nvh, r.u_dim, g_one, g_zero, nvh, 1l)
                } else {
                    enc_site_gemv(enc, t, KqFmt.q8, t.dnbeta_offs[l], nvh, dim, r.bxb, r.bdn_b, r.u_dim, r.u_nvh)
                    enc_site_gemv(enc, t, KqFmt.q8, t.dnalpha_offs[l], nvh, dim, r.bxb, r.bdn_g, r.u_dim, r.u_nvh)
                }
            }
            if (g_skip != "attn") {
                let soff = r.dn_state_base + uint64(dnli * nvh * c.ssm_d_state * c.ssm_d_state * 4l)
                let hoff = r.dn_conv_base + uint64(dnli * (c.ssm_d_conv - 1l) * cd * 4l)
                var bw_cv = upload_region(addr < void? >(t.fblob[t.dn_conv_off + l * c.ssm_d_conv * cd]), uint64(c.ssm_d_conv * cd * 4l))
                enc_dn_conv(enc, r.bdn_qkv, r.bdn_state, hoff, bw_cv, r.bdn_cv, r.u_cd, r.u_dconv, r.u_one32, cd, 1l)
                enc_dn_conv_hist(enc, r.bdn_qkv, r.bdn_state, hoff, r.u_cd, r.u_dconv, r.u_one32, r.bdn_state, hoff, r.u_zero32, cd)
                enc_dn_l2norm(enc, r.bdn_cv, r.u_cd, r.u_dn_kd, r.u_ds, r.u_eps, r.u_qscale, 2l * c.ssm_n_group, 1l)
                var baa = upload_region(addr < void? >(t.fblob[t.dn_a_off + l * nvh]), uint64(nvh * 4l))
                var bdt = upload_region(addr < void? >(t.fblob[t.dn_dt_off + l * nvh]), uint64(nvh * 4l))
                enc_dn_scan(enc, r.bdn_cv, r.bdn_state, soff, r.bdn_b, r.bdn_g, baa, bdt, r.bdn_o,
                    r.u_cd, r.u_dn_kd, r.u_ds, r.u_nvh, r.u_nkh, r.u_di, r.u_one32, r.bdn_state, soff, r.u_zero32, c.ssm_d_state, nvh)
                var bwn = upload_region(addr < void? >(t.fblob[t.dn_norm_off + l * c.ssm_d_state]), uint64(c.ssm_d_state * 4l))
                enc_dn_gate(enc, r.bdn_o, r.bdn_z, bwn, r.u_ds, r.u_di, r.u_eps, nvh, 1l)
            }
            if (g_skip != "gemv") {
                enc_site_gemv(enc, t, KqFmt.q8, t.dnout_offs[l], dim, di, r.bdn_o, r.bxb2, r.u_di, r.u_dim)
            }
            dnli++
        }
        // the [bq | bk | bv] bias plane matches the fused QKV index space, added pre-rope; no bias -> the qkv buffer dummy-binds the slot
        var bbias = r.bqkv
        if (c.attn_qkv_bias) {
            bbias = upload_region_cat3(
                addr < void? >(t.bq[l * qd]), uint64(qd * 4l),
                addr < void? >(t.bk[l * kv_dim]), uint64(kv_dim * 4l),
                addr < void? >(t.bv[l * kv_dim]), uint64(kv_dim * 4l))
        }
        if (!recr && g_skip != "gemv" && g_skip != "qkv") {   // qkv knockout: the attention input GEMVs only
            if (fused_qkv && t.wv_offs[l] >= 0l) {
                // one dispatch over all three segments — the kernel binds each tensor's blob slice separately (kind-major layout keeps q/k/v apart)
                let bwq_ = blob_of(g_dev, t, t.wq_offs[l])
                enc_gemv_qkv_rs(enc, f16v, bwq_.buf, bwq_.boff,
                    uint64((t.wk_offs[l] / 32l) * 34l), uint64((t.wv_offs[l] / 32l) * 34l),
                    r.bxb, r.bqkv, g_arena, g_arena,
                    lkoff + uint64(pos * rowb), lvoff + uint64(pos * rowb),
                    bcos_l, bsin_l, r.u_dim, r.u_qkvd, r.u_qd, r.u_kvd, r.u_hs, r.u_neox,
                    bbias, r.u_hasb, qkvd)
            } else {
                if (c.q_gated) {
                    // 2x-wide packed [q|gate] projection, deinterleaved into the qkv q segment + gate rows
                    enc_site_gemv(enc, t, fq, t.wq_offs[l], 2l * qd, dim, r.bxb, r.bdn_qg, r.u_dim, r.u_dn_qd2)
                    enc_dn_deint(enc, r.bdn_qg, r.bqkv, r.bdn_gate, u_hs_l, u_qd_l, u_qd_l, qd, 1l)
                } else {
                    enc_site_gemv(enc, t, fq, t.wq_offs[l], qd, dim, r.bxb, r.bqkv, r.u_dim, u_qd_l)
                }
                if (!kvsh) {
                    enc_site_gemv(enc, t, fk, t.wk_offs[l], kv_dim, dim, r.bxb, r.bqkv, r.u_dim, u_kvd_l, uint64(qd * 4l))
                    if (t.wv_offs[l] >= 0l) {
                        enc_site_gemv(enc, t, fv, t.wv_offs[l], kv_dim, dim, r.bxb, r.bqkv, r.u_dim, u_kvd_l, uint64((qd + kv_dim) * 4l))
                    }
                }
            }
        }
        if (!recr && !kvsh && g_skip != "ew" && (t.wv_offs[l] < 0l || c.v_norm)) {
            // gemma4 V: no-wv layers fill the v segment from the PRE-norm/PRE-rope K projection (the CPU mm_qkv order, BEFORE qk_norm touches k); with v_norm the copy FUSES into the ones-RMS
            if (t.wv_offs[l] < 0l && !c.v_norm) {
                enc_copy_row(enc, r.bqkv, uint64(qd * 4l), r.bqkv, uint64((qd + kv_dim) * 4l), u_kvd_l, kv_dim)
            }
            if (c.v_norm) {
                // MAX-width upload: the region cache is address-keyed and both classes read this one plane — a narrower first upload starved the hs-512 class (Metal OOB reads are ZEROS)
                var bones = upload_region(addr < void? >(t.fblob[t.rms_ones_off]), uint64(max_head_size(c) * 4l))
                enc_qk_norm(enc, r.bqkv, bones, r.u_zero32, het ? r.u_voffe_sw : r.u_voffe,
                    u_qkvd_l, u_hs_l, r.u_zero32, r.u_eps,
                    t.wv_offs[l] < 0l ? u_qd_l : (het ? r.u_voffe_sw : r.u_voffe),
                    kv_dim / head_size, 1l, head_size)
            }
        }
        if (!recr && c.qk_norm && g_skip != "ew" && !f16v) {
            // per-head QK RMSNorm before rope (qwen3) — the f16 mirror rides the fused H-form rope-store instead of this prepass
            var bqkw = upload_region_cat3(
                addr < void? >(t.fblob[t.rms_q_offs[l]]), uint64(head_size * 4l),
                addr < void? >(t.fblob[t.rms_k_offs[l]]), uint64(head_size * 4l), null, 0ul)
            enc_qk_norm(enc, r.bqkv, bqkw, r.u_nh, u_qd_l, u_qkvd_l, u_hs_l, u_hs_l, r.u_eps,
                u_qd_l, kvsh ? n_heads : n_heads + kv_dim / head_size, 1l, head_size)
        }
        if (!recr && g_skip != "ew" && !fused_qkv) {
            // shared-KV layers dispatch the Q rows only — no K rope, no mirror row store
            if (r.kdt == KVDtype.q8_0) {
                enc_rope_store_q8(enc, r.bqkv, uint64((qd + kv_dim) * 4l), g_arena, g_arena,
                    lkoff + uint64(pos * rowb), lvoff + uint64(pos * rowb),
                    bcos_l, bsin_l, u_qd_l, u_hs_l, u_npq_l, u_nblk_l, r.u_neox, bbias, r.u_hasb,
                    kvsh ? qd / 2l : qd / 2l + 2l * (kv_dim / 32l))
            } elif (r.kdt == KVDtype.tq4) {
                enc_rope_store_tq4(enc, r.bqkv, uint64((qd + kv_dim) * 4l), g_arena, g_arena,
                    lkoff + uint64(pos * rowb), lvoff + uint64(pos * rowb),
                    bcos_l, bsin_l, u_qd_l, u_hs_l, r.u_nh, u_kvm_l, r.u_neox, bbias, r.u_hasb,
                    kvsh ? n_heads : n_heads + 2l * (kv_dim / head_size), head_size)
            } elif (c.qk_norm && f16v) {
                // QK-norm x f16 mirror: bias + per-head RMS + rope + store fold into ONE head-cooperative dispatch
                var bqkw = upload_region_cat3(
                    addr < void? >(t.fblob[t.rms_q_offs[l]]), uint64(head_size * 4l),
                    addr < void? >(t.fblob[t.rms_k_offs[l]]), uint64(head_size * 4l), null, 0ul)
                enc_rope_store_h16(enc, r.bqkv, uint64((qd + kv_dim) * 4l), g_arena, g_arena,
                    lkoff + uint64(pos * rowb), lvoff + uint64(pos * rowb),
                    bcos_l, bsin_l, bqkw, bbias, u_qd_l, u_hs_l, r.u_nh, u_kvm_l, r.u_neox,
                    r.u_hasb, r.u_hasn, r.u_eps, kvsh ? n_heads : n_heads + 2l * (kv_dim / head_size), head_size,
                    [brot = r.u_rot])
            } else {
                enc_rope_store(enc, f16v, r.bqkv, uint64((qd + kv_dim) * 4l), g_arena, g_arena,
                    lkoff + uint64(pos * rowb), lvoff + uint64(pos * rowb),
                    bcos_l, bsin_l, u_qd_l, u_hs_l, u_npq_l, u_npall_l, r.u_neox, bbias, r.u_hasb,
                    kvsh ? qd / 2l : (qd + kv_dim) / 2l, [brot = r.u_rot])
            }
        }
        if (!recr && g_skip != "attn") {
            // sliding layers bind `window` (wstart = cnt - window) + the first-live-chunk comb start, global bind 0; sliding only surfaces chunked (the gate pins window >= 128 >= the single-form ceiling)
            var bwin = lsl ? r.u_winsl : r.u_zero32
            var bchlo = lsl ? r.u_chlo : r.u_zero32
            var bsnk : MetalBuffer?   // null = the enc defaults bind the identity
            if (c.attn_sinks) {
                bsnk = upload_region(addr < void? >(t.fblob[t.sinks_off + l * n_heads]), uint64(n_heads * 4l))
            }
            if (r.chunked) {
                if (r.kdt == KVDtype.q8_0) {
                    enc_attn_part_q8(enc, g_arena, g_arena, lkoff, lvoff,
                        r.bqkv, r.bpart, r.u_cnt, u_nblk_l, r.u_nh, u_kvm_l, u_hs_l, u_scale_l, r.u_nch, r.u_softcap, bwin, n_heads * nchunks, head_size)
                } elif (r.kdt == KVDtype.tq4) {
                    enc_attn_part_tq4(enc, g_arena, g_arena, lkoff, lvoff,
                        r.bqkv, r.bpart, r.u_cnt, u_nblk_l, r.u_nh, u_kvm_l, u_hs_l, u_scale_l, r.u_nch, r.u_softcap, bwin, n_heads * nchunks, head_size)
                } else {
                    enc_attn_part(enc, f16v, g_arena, g_arena, lkoff, lvoff,
                        r.bqkv, r.bpart, r.u_cnt, u_kvd_l, r.u_nh, u_kvm_l, u_hs_l, u_scale_l, r.u_nch, r.u_softcap, bwin, n_heads * nchunks, head_size)
                }
                enc_attn_comb(enc, r.bpart, r.bxb, r.u_nch, r.u_nh, u_hs_l, bchlo, n_heads, head_size, bsnk, r.u_hass)
            } else {
                if (r.kdt == KVDtype.q8_0) {
                    enc_attn_q8(enc, g_arena, g_arena, lkoff, lvoff,
                        r.bqkv, r.bxb, r.u_cnt, u_nblk_l, r.u_nh, u_kvm_l, u_hs_l, u_scale_l, r.u_softcap, n_heads, head_size, bsnk, r.u_hass)
                } elif (r.kdt == KVDtype.tq4) {
                    enc_attn_tq4(enc, g_arena, g_arena, lkoff, lvoff,
                        r.bqkv, r.bxb, r.u_cnt, u_nblk_l, r.u_nh, u_kvm_l, u_hs_l, u_scale_l, r.u_softcap, n_heads, head_size, bsnk, r.u_hass)
                } else {
                    enc_attn(enc, f16v, g_arena, g_arena, lkoff, lvoff,
                        r.bqkv, r.bxb, r.u_cnt, u_kvd_l, r.u_nh, u_kvm_l, u_hs_l, u_scale_l, r.u_softcap, n_heads, head_size, bsnk, r.u_hass)
                }
            }
            if (r.kdt == KVDtype.tq4) {
                // the attention out is rotated-basis per head — un-rotate before the wo projection
                enc_fwht_unsign(enc, r.bxb, u_hs_l, u_qd_l, n_heads, 1l, head_size)
            }
        }
        if (!recr && c.q_gated && g_skip != "ew") {
            // gated-attention epilogue: out ⊙ σ(gate) between the attention output and wo (the CPU order)
            enc_ew2(enc, g_pso_sigmul, r.bxb, 0ul, r.bdn_gate, 0ul, u_qd_l, qd)
        }
        if (!recr && g_skip != "gemv" && g_skip != "wo") {   // recurrent layers have no wo — ssm_out (already in bxb2) is their out projection
            enc_site_gemv(enc, t, kq_fmt_at(t.wo_fmt, l), t.wo_offs[l], dim, qd, r.bxb, r.bxb2, u_qd_l, r.u_dim)
            if (c.attn_out_bias) {
                var bwo_b = upload_region(addr < void? >(t.bo[l * dim]), uint64(dim * 4l))
                enc_ew2(enc, g_pso_add, r.bxb2, 0ul, bwo_b, 0ul, r.u_dim, dim)
            }
        }
        // ffn: [post-attn norm ->] fused add+norm -> W1|W3 -> swiglu/geglu -> W2 [-> post-ffn norm] -> fused add + next attn norm (pre_post_norm norms IN PLACE — the CPU order)
        if (g_skip != "ew") {
            var bw_ffn = upload_region(addr < void? >(t.fblob[t.rms_ffn_off + l * dim]), uint64(dim * 4l))
            if (c.pre_post_norm) {
                var bw_pa = upload_region(addr < void? >(t.fblob[t.rms_post_att_off + l * dim]), uint64(dim * 4l))
                if (decode_fuse_pre()) {
                    // post_attn_rms + add_rms collapse to one dispatch (R1 chain-depth fusion)
                    enc_pre_add_rms_site(enc, r.bx, r.bxb2, bw_pa, bw_ffn, r.bxb, r.u_dim, r.u_eps, dim)
                } else {
                    enc_rms(enc, r.bxb2, bw_pa, r.bxb2, r.u_dim, r.u_eps)
                    enc_add_rms_site(enc, r.bx, r.bxb2, bw_ffn, r.bxb, r.u_dim, r.u_eps, dim)
                }
            } else {
                enc_add_rms_site(enc, r.bx, r.bxb2, bw_ffn, r.bxb, r.u_dim, r.u_eps, dim)
            }
        }
        if (r.moe) {
            // router -> select -> experts -> combine; g4 = gemma4's parallel-dense form
            let knfe = c.n_expert_used * c.n_ff_exp
            let g4 = c.moe_dense_shexp
            let fe1 = kq_fmt_at(t.we1_fmt, l)
            let fe3 = kq_fmt_at(t.we3_fmt, l)
            let fe2 = kq_fmt_at(t.we2_fmt, l)
            var bmx = r.bxb   // expert input: plain = the rms_ffn norm-out; g4 = pre_ffn2
            if (g4 && g_skip != "ew") {
                var bw_p2 = upload_region(addr < void? >(t.fblob[t.rms_pre_ffn2_off + l * dim]), uint64(dim * 4l))
                enc_rms(enc, r.bx, bw_p2, r.bxb2, r.u_dim, r.u_eps)
                bmx = r.bxb2
            }
            if (g_skip != "gemv") {
                var bw_rt = upload_region(addr < void? >(t.fblob[t.router_off + l * c.n_expert * dim]), uint64(c.n_expert * dim * 4l))
                var brb : MetalBuffer? = g_one
                var bhrb = r.u_zero32
                if (c.moe_router_bias) {
                    brb = upload_region(addr < void? >(t.fblob[t.router_b_off + l * c.n_expert]), uint64(c.n_expert * 4l))
                    bhrb = r.u_moe_gmode   // gpt-oss: gmode == 1 doubles as the has-bias flag
                }
                if (g4) {
                    var bw_rs = upload_region(addr < void? >(t.fblob[t.moe_router_scale_off + l * dim]), uint64(dim * 4l))
                    enc_g4_router_norm(enc, r.bx, bw_rs, r.bmoe_rt, r.u_dim, r.u_eps, r.u_moe_invd, 1l)
                    enc_moe_router(enc, bw_rt, r.bmoe_rt, r.bmoe_lg, r.u_dim, r.u_moe_ne, r.u_dim, brb, bhrb, c.n_expert, 1l)
                } else {
                    enc_moe_router(enc, bw_rt, r.bxb, r.bmoe_lg, r.u_dim, r.u_moe_ne, r.u_dim, brb, bhrb, c.n_expert, 1l)
                }
                enc_moe_select(enc, r.bmoe_lg, r.bmoe_sel, r.bmoe_sel, r.u_moe_ne, r.u_moe_k, r.u_moe_gmode, 1l)
                if (g4) {
                    var bw_ds = upload_region(addr < void? >(t.fblob[t.moe_down_scale_off + l * c.n_expert]), uint64(c.n_expert * 4l))
                    enc_moe_wscale(enc, r.bmoe_sel, r.bmoe_sel, bw_ds, r.u_moe_k, 1l)
                }
                if (r.u_moe_nsh != null) {
                    // shexp raw gate dot (σ in the combine) — bmoe_lg[0] is free once select consumed it
                    var bw_shg = upload_region(addr < void? >(t.fblob[t.shexp_gate_off + l * dim]), uint64(dim * 4l))
                    enc_moe_router(enc, bw_shg, r.bxb, r.bmoe_lg, r.u_dim, r.u_one32, r.u_dim, g_one, g_zero, 1l, 1l)
                }
                if (g_skip != "moe_rt") {   // moe_rt knockout: the routed expert GEMVs only (router/select stay live)
                    if (t.experts_mx4) {
                        // gpt-oss: mx4 expert planes + per-expert biases fold into the twins
                        var bwb1 = upload_region(addr < void? >(t.fblob[t.web1_off + l * c.n_expert * c.n_ff_exp]), uint64(c.n_expert * c.n_ff_exp * 4l))
                        var bwb3 = upload_region(addr < void? >(t.fblob[t.web3_off + l * c.n_expert * c.n_ff_exp]), uint64(c.n_expert * c.n_ff_exp * 4l))
                        enc_moe_gemv_mx4(enc, t, t.we1_offs[l], c.n_ff_exp, c.n_expert_used, 1l, bmx, r.bh12,
                            r.u_dim, r.u_moe_nfe, r.bmoe_sel, r.u_moe_eblk, r.u_zero32,
                            r.u_zero32, r.u_zero32, r.u_zero32, bwb1, r.u_moe_gmode)
                        enc_moe_gemv_mx4(enc, t, t.we3_offs[l], c.n_ff_exp, c.n_expert_used, 1l, bmx, r.bh12,
                            r.u_dim, r.u_moe_nfe, r.bmoe_sel, r.u_moe_eblk, r.u_zero32,
                            r.u_zero32, r.u_zero32, r.u_zero32, bwb3, r.u_moe_gmode, uint64(knfe * 4l))
                    } else {
                        enc_moe_gemv(enc, t, fe1, t.we1_offs[l], c.n_ff_exp, c.n_expert_used, 1l, bmx, r.bh12,
                            r.u_dim, r.u_moe_nfe, r.bmoe_sel, fe1 == KqFmt.q8 || fe1 == KqFmt.q51 ? r.u_moe_eblk : r.u_moe_esb, r.u_zero32,
                            r.u_zero32, r.u_zero32, r.u_zero32)
                        enc_moe_gemv(enc, t, fe3, t.we3_offs[l], c.n_ff_exp, c.n_expert_used, 1l, bmx, r.bh12,
                            r.u_dim, r.u_moe_nfe, r.bmoe_sel, fe3 == KqFmt.q8 || fe3 == KqFmt.q51 ? r.u_moe_eblk : r.u_moe_esb, r.u_zero32,
                            r.u_zero32, r.u_zero32, r.u_zero32, uint64(knfe * 4l))
                    }
                }
            }
            if (g_skip != "ew") {
                let pso_act = c.ffn_act == FfnAct.swiglu_oai ? g_pso_swiglu_oai : (g4 ? g_pso_geglu : g_pso_swiglu)
                enc_ew2(enc, pso_act, r.bh12, 0ul, r.bh12, uint64(knfe * 4l), r.u_moe_knfe, knfe)
            }
            if (g_skip != "gemv") {
                if (g_skip != "moe_rt") {
                    if (t.experts_mx4) {
                        var bwb2 = upload_region(addr < void? >(t.fblob[t.web2_off + l * c.n_expert * dim]), uint64(c.n_expert * dim * 4l))
                        enc_moe_gemv_mx4(enc, t, t.we2_offs[l], dim, c.n_expert_used, 1l, r.bh12, r.bmoe_dn,
                            r.u_moe_nfe, r.u_dim, r.bmoe_sel, r.u_moe_eblk, r.u_moe_xs_dn,
                            r.u_zero32, r.u_zero32, r.u_zero32, bwb2, r.u_moe_gmode)
                    } else {
                        enc_moe_gemv(enc, t, fe2, t.we2_offs[l], dim, c.n_expert_used, 1l, r.bh12, r.bmoe_dn,
                            r.u_moe_nfe, r.u_dim, r.bmoe_sel, fe2 == KqFmt.q8 || fe2 == KqFmt.q51 ? r.u_moe_eblk : r.u_moe_esb, r.u_moe_xs_dn,
                            r.u_zero32, r.u_zero32, r.u_zero32)
                    }
                }
                if (r.u_moe_nsh != null && g_skip != "moe_sh") {
                    // shexp reads bxb BEFORE combine overwrites it; own bh12s panel = schedulable under the routed chain
                    let nsh = c.n_ff_shexp
                    let she = dim * nsh
                    let bwsh1 = blob_of(g_dev, t, t.wsh1_off + l * she)
                    enc_gemv_w13sw(enc, bwsh1.buf, bwsh1.boff, uint64(((t.wsh3_off + l * she) / 32l) * 34l),
                        r.bxb, r.bh12s, r.u_dim, r.u_moe_nsh, nsh, r.u_zero32)
                    enc_site_gemv(enc, t, KqFmt.q8, t.wsh2_off + l * she, dim, nsh, r.bh12s, r.bxb2, r.u_moe_nsh, r.u_dim)
                }
                enc_moe_combine(enc, r.bmoe_dn, r.bmoe_sel, g4 ? r.bmoe_rt : r.bxb, r.u_dim, r.u_moe_k, dim, 1l)
                if (r.u_moe_nsh != null && g_skip != "ew") {
                    // routed sum lands in bxb first — the shexp branch adds σ(gate)-scaled on top
                    enc_axpy_sig(enc, r.bxb, r.bxb2, r.bmoe_lg, r.u_dim, r.u_dim, dim)
                }
            }
            if (g4) {
                // routed post-norm + the dense shared branch, summed into bxb for the epilogue
                if (g_skip != "ew") {
                    var bw_pf2 = upload_region(addr < void? >(t.fblob[t.rms_post_ffn2_off + l * dim]), uint64(dim * 4l))
                    enc_rms(enc, r.bmoe_rt, bw_pf2, r.bmoe_rt, r.u_dim, r.u_eps)
                }
                let sh_w13_q8 = kq_fmt_at(t.w1_fmt, l) == KqFmt.q8 && kq_fmt_at(t.w3_fmt, l) == KqFmt.q8
                if (g_skip != "gemv" && g_skip != "moe_sh") {
                    if (sh_w13_q8) {
                        // fused shared-branch gate+up + gelu (extends the dense w13sw fusion to the g4 shexp)
                        let bw1 = blob_of(g_dev, t, t.w1_offs[l])
                        enc_gemv_w13sw(enc, bw1.buf, bw1.boff, uint64((t.w3_offs[l] / 32l) * 34l),
                            r.bxb, r.bh12s, r.u_dim, r.u_hidden, hidden, geglu ? r.u_one32 : r.u_zero32)
                    } else {
                        enc_site_gemv(enc, t, kq_fmt_at(t.w1_fmt, l), t.w1_offs[l], hidden, dim, r.bxb, r.bh12s, r.u_dim, r.u_hidden)
                        enc_site_gemv(enc, t, kq_fmt_at(t.w3_fmt, l), t.w3_offs[l], hidden, dim, r.bxb, r.bh12s, r.u_dim, r.u_hidden, uint64(hidden * 4l))
                    }
                }
                if (g_skip != "ew" && !sh_w13_q8) {
                    enc_ew2(enc, g_pso_geglu, r.bh12s, 0ul, r.bh12s, uint64(hidden * 4l), r.u_hidden, hidden)
                }
                if (g_skip != "gemv" && g_skip != "moe_sh") {
                    enc_site_gemv(enc, t, kq_fmt_at(t.w2_fmt, l), t.w2_offs[l], dim, hidden, r.bh12s, r.bxb, r.u_hidden, r.u_dim)
                }
                if (g_skip != "ew") {
                    var bw_pf1 = upload_region(addr < void? >(t.fblob[t.rms_post_ffn1_off + l * dim]), uint64(dim * 4l))
                    enc_rms(enc, r.bxb, bw_pf1, r.bxb, r.u_dim, r.u_eps)
                    enc_ew2(enc, g_pso_add, r.bxb, 0ul, r.bmoe_rt, 0ul, r.u_dim, dim)
                }
            }
        } else {
            let f1 = kq_fmt_at(t.w1_fmt, l)
            let f3 = kq_fmt_at(t.w3_fmt, l)
            let w13_q8 = f1 == KqFmt.q8 && f3 == KqFmt.q8
            if (g_skip != "gemv") {
                if (w13_q8) {
                    // fused gate+up dot + silu/gelu epilogue — gemma geglu rides the act uniform, no separate ew pass
                    let bw1 = blob_of(g_dev, t, t.w1_offs[l])
                    enc_gemv_w13sw(enc, bw1.buf, bw1.boff, uint64((t.w3_offs[l] / 32l) * 34l),
                        r.bxb, r.bh12, r.u_dim, r.u_hidden, hidden, geglu ? r.u_one32 : r.u_zero32)
                } else {
                    enc_site_gemv(enc, t, f1, t.w1_offs[l], hidden, dim, r.bxb, r.bh12, r.u_dim, r.u_hidden)
                    enc_site_gemv(enc, t, f3, t.w3_offs[l], hidden, dim, r.bxb, r.bh12, r.u_dim, r.u_hidden, uint64(hidden * 4l))
                }
            }
            if (g_skip != "ew" && !w13_q8) {
                enc_ew2(enc, geglu ? g_pso_geglu : g_pso_swiglu, r.bh12, 0ul, r.bh12, uint64(hidden * 4l), r.u_hidden, hidden)
            }
            if (g_skip != "gemv") {
                enc_site_gemv(enc, t, kq_fmt_at(t.w2_fmt, l), t.w2_offs[l], dim, hidden, r.bh12, r.bxb, r.u_hidden, r.u_dim)
            }
        }
        if (g_skip != "ew") {
            if (c.pre_post_norm) {
                var bw_pf = upload_region(addr < void? >(t.fblob[t.rms_post_ffn_off + l * dim]), uint64(dim * 4l))
                enc_rms(enc, r.bxb, bw_pf, r.bxb, r.u_dim, r.u_eps)
            }
        }
        if (r.ple) {
            // PLE between FFN residual and out_scale: branch = post_norm(proj·(gelu(gate·x) ⊙ side[l]))
            let ple = c.n_embd_per_layer
            if (g_skip != "ew") {
                enc_ew2(enc, g_pso_add, r.bx, 0ul, r.bxb, 0ul, r.u_dim, dim)
            }
            if (g_skip != "gemv") {
                enc_site_gemv(enc, t, KqFmt.q8, t.ple_gate_off + l * dim * ple, ple, dim, r.bx, r.bh12, r.u_dim, r.u_ple)
            }
            if (g_skip != "ew") {
                enc_ew2(enc, g_pso_geglu, r.bh12, 0ul, r.bple, uint64(l * ple * 4l), r.u_ple, ple)
            }
            if (g_skip != "gemv") {
                enc_site_gemv(enc, t, KqFmt.q8, t.ple_proj_off + l * ple * dim, dim, ple, r.bh12, r.bxb, r.u_ple, r.u_dim)
            }
            if (g_skip != "ew") {
                var bw_pp = upload_region(addr < void? >(t.fblob[t.ple_post_off + l * dim]), uint64(dim * 4l))
                enc_rms(enc, r.bxb, bw_pp, r.bxb, r.u_dim, r.u_eps)
            }
        }
        if (g_skip != "ew") {
            // gemma4 layer_out_scale: (x + branch) * s folds into the residual add (a 4-byte view into the weights blob)
            var bosc : MetalBuffer?
            if (c.layer_out_scale) {
                bosc = upload_region(addr < void? >(t.fblob[t.out_scale_off + l]), 4ul)
            }
            if (l + 1l < c.n_layers) {
                var bw_attn = upload_region(addr < void? >(t.fblob[t.rms_att_off + (l + 1l) * dim]), uint64(dim * 4l))
                enc_add_rms_site(enc, r.bx, r.bxb, bw_attn, r.bxb, r.u_dim, r.u_eps, dim, bosc)
            } else {
                enc_ew2(enc, g_pso_add, r.bx, 0ul, r.bxb, 0ul, r.u_dim, dim, bosc)
            }
        }
    }
}

// final rmsnorm + classifier GEMV (+ softcap/suppress epilogues); norm_off parameterized for
// the MTP draft head (shared_head_norm when shipped, else output_norm)
def private encode_cls_tail(t : Model; var r : StepRes; enc : MetalComputeEncoder?; norm_off : int64) {
    let c = t.config
    let dim = c.dim
    // final rmsnorm (one row) + classifier GEMV — the caller samples straight from s.logits
    unsafe {
        var bwfin = upload_region(addr < void? >(t.fblob[norm_off]), uint64(dim * 4l))
        enc_rms(enc, r.bx, bwfin, r.bxf, r.u_dim, r.u_eps)
    }
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    var bcls = r.bnc != null ? r.bnc : r.blog
    enc_site_gemv(enc, t, cls_fmt, t.wcls_off, c.vocab_size, dim, r.bxf, bcls, r.u_dim, r.u_vocab)
    if (r.u_flcap != null) {
        // final-logit soft cap (gemma2/gemma4): cap*tanh in place — monotonic, so the spec argmax is unaffected
        enc_softcap_row(enc, bcls, 0ul, r.u_vocab, r.u_flcap, c.vocab_size)
    }
    if (r.bsupp != null) {
        // gemma4 suppressed ids pin to -1e30 AFTER the cap (the CPU order) — the spec argmax reads pinned logits
        enc_suppress_row(enc, bcls, 0ul, r.bsupp, r.u_nsupp, int64(r.bytes_supp / 4ul))
    }
    if (r.bnc != null && r.blog != null) {
        // spec-capable step: mirror zero-copy logits into pooled staging — the next step's speculative argmax reads that, never session memory
        enc_copy_row(enc, r.bnc, 0ul, r.blog, 0ul, r.u_vocab, c.vocab_size)
    }
}

// encode the MTP draft-head step: [enorm(embed) ; hnorm(mtp_h)] -> eh_proj -> the trunk-shaped
// block at l == n_layers -> shared-head norm + classifier (skipped for the seam-row variant).
// The caller poked bx = raw embed row, bxb2 = mtp_h, bcos/bsin = the DRAFT row's rope table
def private encode_draft_step(t : Model; var r : StepRes; with_cls : bool = true) {
    let c = t.config
    let dim = c.dim
    var cb = metal_new_command_buffer_unretained(g_queue)
    let conc = decode_concurrent() && empty(g_skip)
    var enc = conc ? metal_new_compute_encoder_concurrent(cb) : metal_new_compute_encoder(cb)
    gr_arm(conc)   // the draft step rides the same capture rail as the main step
    unsafe {
        var bw_en = upload_region(addr < void? >(t.fblob[t.mtp_enorm_off]), uint64(dim * 4l))
        enc_rms(enc, r.bx, bw_en, r.bxb, r.u_dim, r.u_eps)
        enc_copy_row(enc, r.bxb, 0ul, r.bcat, 0ul, r.u_dim, dim)
        var bw_hn = upload_region(addr < void? >(t.fblob[t.mtp_hnorm_off]), uint64(dim * 4l))
        enc_rms(enc, r.bxb2, bw_hn, r.bxb, r.u_dim, r.u_eps)
        enc_copy_row(enc, r.bxb, 0ul, r.bcat, uint64(dim * 4l), r.u_dim, dim)
        enc_site_gemv(enc, t, t.mtp_ehproj_fmt, t.mtp_ehproj_off, dim, 2l * dim, r.bcat, r.bx, r.u_dim2, r.u_dim)
        var bw_att = upload_region(addr < void? >(t.fblob[t.rms_att_off + c.n_layers * dim]), uint64(dim * 4l))
        enc_rms(enc, r.bx, bw_att, r.bxb, r.u_dim, r.u_eps)
    }
    var dn0 = 0l
    encode_layer(t, r, enc, c.n_layers, dn0)
    if (with_cls) {
        encode_cls_tail(t, r, enc, t.mtp_headnorm_off >= 0l ? t.mtp_headnorm_off : t.rms_final_off)
    }
    if (conc) {
        graph_flush_sched(enc)
    }
    metal_end_encoding(enc)
    metal_release(enc)
    metal_commit(cb)
    r.committed = true
    r.cb = cb
}

// land the draft step: the draft-slab KV row back to the CPU cache, logits + mtp_h handoff
// (the seam variant lands the KV row ONLY). NO watermark/dn-cursor touch — the draft rewrites
// a row BELOW the watermark in its OWN slab and never runs the recurrent branch
def private finish_draft_step(t : Model; var s : Session; r : StepRes; seam : bool = false) : bool {
    metal_wait_until_completed(r.cb)
    let err = metal_command_buffer_error(r.cb)
    let ok = empty(err)
    if (ok) {
        let c = t.config
        let l = c.n_layers
        let dpos = r.pos
        unsafe {
            let rowb = kv_row_bytes(s.kv_dtype_k, layer_kv_dim(c, l))
            let lb = mirror_lbase(t, s.kv_dtype_k, r.cap, l)
            let prow = kv_phys_row(s, dpos)
            var kmp = reinterpret<uint8?>(metal_buffer_contents(g_arena)) + r.koff
            var vmp = reinterpret<uint8?>(metal_buffer_contents(g_arena)) + r.voff
            memcpy(reinterpret<void?>(kv_layer_kbase(s, l, c.seq_len) + prow * rowb),
                reinterpret<void?>(kmp + lb + dpos * rowb), int(rowb))
            memcpy(reinterpret<void?>(kv_layer_vbase(s, l, c.seq_len) + prow * rowb),
                reinterpret<void?>(vmp + lb + dpos * rowb), int(rowb))
            if (!seam) {
                if (r.bnc == null) {
                    memcpy(addr < void? >(s.logits[0]), metal_buffer_contents(r.blog), int(r.bytes_log))
                }
                // the head's own h_nextn — recursive drafting handoff (forward_mtp's contract)
                memcpy(addr < void? >(s.mtp_h[0]), metal_buffer_contents(r.bxf), int(r.bytes_x))
            }
        }
        if (!seam) {
            s.mtp_h_pos1 = 0l   // NOT a trunk hidden: the warm's seam row must not consume it
        }
    } else {
        to_log(LOG_ERROR, "dasLLAMA metal MTP draft: dispatch failed ({err})\n")   // nolint:PERF028,PERF026 — error path on a failed draft dispatch
        decline(MetalDecodeDecline.gpu_error)
    }
    return ok
}

//! forward_mtp's GPU twin: the draft head against a resident blob model. `pos` is the trunk
//! position (n_past); the draft row lands at pos-1 (rotary is delta-invariant). Needs a
//! GPU-current session (KV mirror holds rows [0, pos)) and GPU logits. false = declined.
def metal_mtp_draft_forward(t : Model; var s : Session; token, pos : int64; seam : bool = false) : bool {
    let c = t.config
    if (g_dev == null || !t.metal_blob || c.n_layer_nextn <= 0l || pos < 1l ||
            weight_caches_stale() || !key_exists(g_mirrors, s.uid)) {
        return false
    }
    finish_pending_step()
    let mr = g_mirrors[s.uid]
    let dpos = pos - 1l
    if (mr.watermark < pos || mr.cap_rows < pos || mr.kdt != s.kv_dtype_k || mr.vdt != s.kv_dtype_v) {
        return false
    }
    var r = acquire_step(t, s, dpos, mr.koff, mr.voff, mr.cap_rows, [draft = true])
    if (!r.gpu_logits) {
        release_step(r)
        return false
    }
    mtp_draft_prep(t, s, token, dpos)
    unsafe {
        memcpy(metal_buffer_contents(r.bx), addr < void? >(s.x[0]), int(r.bytes_x))
        memcpy(metal_buffer_contents(r.bxb2), addr < void? >(s.mtp_h[0]), int(r.bytes_x))
        memcpy(metal_buffer_contents(r.bcos), addr < void? >(s.rope_cos[0]), int(r.bytes_tab))
        memcpy(metal_buffer_contents(r.bsin), addr < void? >(s.rope_sin[0]), int(r.bytes_tab))
    }
    encode_draft_step(t, r, !seam)
    let ok = finish_draft_step(t, s, r, seam)
    release_step(r)
    return ok
}

//! forward_mtp(..., false)'s GPU twin — the continuation-window SEAM: write the draft head's
//! KV row pos-1 from (embed(token), the pre-window mtp_h) without touching logits or mtp_h.
def metal_mtp_seam_row(t : Model; var s : Session; token, pos : int64) : bool {
    return metal_mtp_draft_forward(t, s, token, pos, true)
}

// ===== MTP B=2 same-slab verify (the spec step's trunk forward) =====

var private g_vrt : array<uint>   // per-layer 2-row route entries: uint4(koff_el+lbase, voff_el+lbase, cap, cnt_i)

// verify shape gate: f16 KV, silu FFN (dense or routed MoE ± shexp), no epilogue exotics.
// Weight sites are fmt-generic (enc_verify_site / enc_moe_gemv — the load gate vets kernel
// coverage); a missing attn_v and the g4/mx4 MoE forms have no verify arm yet
def private metal_mtp_verify_ok(t : Model; s : Session) : bool {
    let c = t.config
    if (s.kv_dtype_k != KVDtype.f16 || s.kv_dtype_v != KVDtype.f16 ||
            has_dual_rope(c) || c.attn_sinks || c.v_norm || c.pre_post_norm || c.layer_out_scale ||
            c.final_logit_softcap > 0.0 || !empty(t.suppress) ||
            (!t.blocks.ffn_is_dense && (c.moe_dense_shexp || t.experts_mx4)) ||
            c.ffn_act != FfnAct.silu ||
            t.wcls_off < 0l || (c.shared_weights && !t.cls_q8 && t.emb_fmt == KqFmt.q8)) {
        return false
    }
    for (l in range64(c.n_layers)) {
        return false if (layer_is_sliding(c, l))
        continue if (layer_is_recurrent(c, l))
        return false if (t.wv_offs[l] < 0l)
    }
    return true
}

// one verify-B=2 weight site: a q8 plane rides the batched q8 GEMV, a kq plane the small-batch
// mvb twins (the batch rail's dispatch at nrows=2). ycol = flat element offset into by
def private enc_verify_site(t : Model; var r : StepRes; enc : MetalComputeEncoder?; fmt : KqFmt;
                            woff, rows, n, ys : int64; bx, by, bn, bd, bys : MetalBuffer?; ycol : int64 = 0l) {
    if (fmt == KqFmt.q8) {
        let bw = blob_of(g_dev, t, woff)
        enc_gemv_b(enc, false, bw.buf, bw.boff, bx, by, bn, bd, bys, rows, uint64(ycol * 4l))
    } else {
        enc_kq_site_b(enc, t, fmt, woff, rows, n, bx, n, by, ys, ycol, bn, bd, bys, r.u_two32, 2l, 2l)
    }
}

// one verify-shaped B=2 layer (batch-form sites, same-slab brt row at l*32) — the trunk
// loop body, shared with the draft-slab warm (which runs it at l == n_layers)
def private encode_verify_layer(t : Model; var r : StepRes; enc : MetalComputeEncoder?; brt : MetalBuffer?; l, pos : int64; var dnli : int64&) {
    let c = t.config
    let dim = c.dim
    let n_heads = c.n_heads
    let hidden = layer_hidden(t, 0l)
    let head_size = c.head_size
    let qd = n_heads * head_size
    let kv_dim = c.n_kv_heads * head_size
    let qkvd = qd + 2l * kv_dim
    let nchunks = (pos + 2l + ATTN_CHUNK_ROWS - 1l) / ATTN_CHUNK_ROWS
    unsafe {
        let rtoff = uint64(l * 32l)
        if (layer_is_recurrent(c, l)) {
            let cd = dn_conv_dim(c)
            let di = c.ssm_d_inner
            let nvh = c.ssm_dt_rank
            let bdnq = blob_of(g_dev, t, t.dnqkv_offs[l])
            enc_gemv_b(enc, false, bdnq.buf, bdnq.boff, r.bxb, r.bdn_qkv, r.u_dim, r.u_cd, r.u_cd, cd)
            let bdng = blob_of(g_dev, t, t.dngate_offs[l])
            enc_gemv_b(enc, false, bdng.buf, bdng.boff, r.bxb, r.bdn_z, r.u_dim, r.u_di, r.u_di, di)
            if (t.dn_ba_f32) {
                // F32-on-disk β/α: the router-slab f32 GEMV, one stream per verify row
                var bw_bt = upload_region(addr < void? >(t.fblob[t.dn_betaf_off + l * dim * nvh]), uint64(dim * nvh * 4l))
                enc_moe_router(enc, bw_bt, r.bxb, r.bdn_b, r.u_dim, r.u_nvh, r.u_dim, g_one, g_zero, nvh, 2l)
                var bw_al = upload_region(addr < void? >(t.fblob[t.dn_alphaf_off + l * dim * nvh]), uint64(dim * nvh * 4l))
                enc_moe_router(enc, bw_al, r.bxb, r.bdn_g, r.u_dim, r.u_nvh, r.u_dim, g_one, g_zero, nvh, 2l)
            } else {
                let bdnb = blob_of(g_dev, t, t.dnbeta_offs[l])
                enc_gemv_b(enc, false, bdnb.buf, bdnb.boff, r.bxb, r.bdn_b, r.u_dim, r.u_nvh, r.u_nvh, nvh)
                let bdna = blob_of(g_dev, t, t.dnalpha_offs[l])
                enc_gemv_b(enc, false, bdna.buf, bdna.boff, r.bxb, r.bdn_g, r.u_dim, r.u_nvh, r.u_nvh, nvh)
            }
            // dual-store: post-row0 state/hist land in the shadow region — reject flips to it
            let sslice = uint64(dnli * nvh * c.ssm_d_state * c.ssm_d_state * 4l)
            let hslice = uint64(dnli * (c.ssm_d_conv - 1l) * cd * 4l)
            let soff = r.dn_state_base + sslice
            let hoff = r.dn_conv_base + hslice
            var bw_cv = upload_region(addr < void? >(t.fblob[t.dn_conv_off + l * c.ssm_d_conv * cd]), uint64(c.ssm_d_conv * cd * 4l))
            enc_dn_conv(enc, r.bdn_qkv, r.bdn_state, hoff, bw_cv, r.bdn_cv, r.u_cd, r.u_dconv, r.u_two32, cd, 2l)
            enc_dn_conv_hist(enc, r.bdn_qkv, r.bdn_state, hoff, r.u_cd, r.u_dconv, r.u_two32, r.bdn_state, r.dn_conv_alt + hslice, r.u_one32, cd)
            enc_dn_l2norm(enc, r.bdn_cv, r.u_cd, r.u_dn_kd, r.u_ds, r.u_eps, r.u_qscale, 2l * c.ssm_n_group, 2l)
            var baa = upload_region(addr < void? >(t.fblob[t.dn_a_off + l * nvh]), uint64(nvh * 4l))
            var bdt = upload_region(addr < void? >(t.fblob[t.dn_dt_off + l * nvh]), uint64(nvh * 4l))
            enc_dn_scan(enc, r.bdn_cv, r.bdn_state, soff, r.bdn_b, r.bdn_g, baa, bdt, r.bdn_o,
                r.u_cd, r.u_dn_kd, r.u_ds, r.u_nvh, r.u_nkh, r.u_di, r.u_two32, r.bdn_state,
                r.dn_state_alt + sslice, r.u_one32, c.ssm_d_state, nvh)
            var bwn = upload_region(addr < void? >(t.fblob[t.dn_norm_off + l * c.ssm_d_state]), uint64(c.ssm_d_state * 4l))
            enc_dn_gate(enc, r.bdn_o, r.bdn_z, bwn, r.u_ds, r.u_di, r.u_eps, nvh, 2l)
            let bdno = blob_of(g_dev, t, t.dnout_offs[l])
            enc_gemv_b(enc, false, bdno.buf, bdno.boff, r.bdn_o, r.bxb2, r.u_di, r.u_dim, r.u_dim, dim)
            dnli++
        } else {
            var bbias = r.bqkv
            if (c.attn_qkv_bias) {
                bbias = upload_region_cat3(
                    addr < void? >(t.bq[l * qd]), uint64(qd * 4l),
                    addr < void? >(t.bk[l * kv_dim]), uint64(kv_dim * 4l),
                    addr < void? >(t.bv[l * kv_dim]), uint64(kv_dim * 4l))
            }
            if (c.q_gated) {
                enc_verify_site(t, r, enc, kq_fmt_at(t.wq_fmt, l), t.wq_offs[l], 2l * qd, dim, 2l * qd,
                    r.bxb, r.bdn_qg, r.u_dim, r.u_dn_qd2, r.u_dn_qd2)
                enc_dn_deint(enc, r.bdn_qg, r.bqkv, r.bdn_gate, r.u_hs, r.u_qd, r.u_qkvd, qd, 2l)
            } else {
                enc_verify_site(t, r, enc, kq_fmt_at(t.wq_fmt, l), t.wq_offs[l], qd, dim, qkvd,
                    r.bxb, r.bqkv, r.u_dim, r.u_qd, r.u_qkvd)
            }
            enc_verify_site(t, r, enc, kq_fmt_at(t.wk_fmt, l), t.wk_offs[l], kv_dim, dim, qkvd,
                r.bxb, r.bqkv, r.u_dim, r.u_kvd, r.u_qkvd, [ycol = qd])
            enc_verify_site(t, r, enc, kq_fmt_at(t.wv_fmt, l), t.wv_offs[l], kv_dim, dim, qkvd,
                r.bxb, r.bqkv, r.u_dim, r.u_kvd, r.u_qkvd, [ycol = qd + kv_dim])
            if (c.qk_norm) {
                var bqkw = upload_region_cat3(
                    addr < void? >(t.fblob[t.rms_q_offs[l]]), uint64(head_size * 4l),
                    addr < void? >(t.fblob[t.rms_k_offs[l]]), uint64(head_size * 4l), null, 0ul)
                enc_qk_norm(enc, r.bqkv, bqkw, r.u_nh, r.u_qd, r.u_qkvd, r.u_hs, r.u_hs, r.u_eps,
                    r.u_qd, n_heads + kv_dim / head_size, 2l, head_size)
            }
            enc_rope_store_b(enc, true, r.bqkv, r.bcos, r.bsin, brt, r.u_zero32, r.u_kvd, r.u_qd, r.u_hs, r.u_npq, r.u_npall, r.u_qkvd, r.u_neox,
                bbias, r.u_hasb, (qd + kv_dim) / 2l, 2l, [rtoff = rtoff, brot = r.u_rot])
            if (r.chunked) {
                enc_attn_part_b(enc, true, r.bqkv, r.bpart, brt, r.u_zero32, r.u_kvd, r.u_nh, r.u_kvm, r.u_hs, r.u_scale, r.u_qkvd, r.u_nch,
                    r.u_softcap, r.u_zero32, 2l, nchunks, n_heads, head_size, [rtoff = rtoff])
                enc_attn_comb_b(enc, r.bpart, r.bxb, brt, r.u_nch, r.u_nh, r.u_hs, r.u_zero32, 2l, n_heads, head_size, [rtoff = rtoff])
            } else {
                enc_attn_b(enc, true, r.bqkv, r.bxb, brt, r.u_zero32, r.u_kvd, r.u_nh, r.u_kvm, r.u_hs, r.u_scale, r.u_qkvd,
                    r.u_softcap, 2l, n_heads, head_size, [rtoff = rtoff])
            }
            if (c.q_gated) {
                enc_ew2(enc, g_pso_sigmul, r.bxb, 0ul, r.bdn_gate, 0ul, r.u_total_qd, 2l * qd)
            }
            enc_verify_site(t, r, enc, kq_fmt_at(t.wo_fmt, l), t.wo_offs[l], dim, qd, dim,
                r.bxb, r.bxb2, r.u_qd, r.u_dim, r.u_dim)
            if (c.attn_out_bias) {
                var bwo_b = upload_region(addr < void? >(t.bo[l * dim]), uint64(dim * 4l))
                enc_ew2(enc, g_pso_add, r.bxb2, 0ul, bwo_b, 0ul, r.u_total_dim, 2l * dim)
            }
        }
        var bw_ffn = upload_region(addr < void? >(t.fblob[t.rms_ffn_off + l * dim]), uint64(dim * 4l))
        if (dim <= ADD_RMS_MAX_DIM) {
            enc_add_rms_b(enc, r.bx, r.bxb2, bw_ffn, r.bxb, r.u_dim, r.u_eps, 2l)
        } else {
            enc_ew2(enc, g_pso_add, r.bx, 0ul, r.bxb2, 0ul, r.u_total_dim, 2l * dim)
            enc_rms_b(enc, r.bx, bw_ffn, r.bxb, r.u_dim, r.u_eps, 2l)
        }
        if (r.moe) {
            // router -> select -> experts -> combine at two streams; shexp rides the fixed-B2 q8 forms
            let knfe = c.n_expert_used * c.n_ff_exp
            let fe1 = kq_fmt_at(t.we1_fmt, l)
            let fe3 = kq_fmt_at(t.we3_fmt, l)
            let fe2 = kq_fmt_at(t.we2_fmt, l)
            var bw_rt = upload_region(addr < void? >(t.fblob[t.router_off + l * c.n_expert * dim]), uint64(c.n_expert * dim * 4l))
            var brb : MetalBuffer? = g_one
            var bhrb = r.u_zero32
            if (c.moe_router_bias) {
                brb = upload_region(addr < void? >(t.fblob[t.router_b_off + l * c.n_expert]), uint64(c.n_expert * 4l))
                bhrb = r.u_moe_gmode
            }
            enc_moe_router(enc, bw_rt, r.bxb, r.bmoe_lg, r.u_dim, r.u_moe_ne, r.u_dim, brb, bhrb, c.n_expert, 2l)
            enc_moe_select(enc, r.bmoe_lg, r.bmoe_sel, r.bmoe_sel, r.u_moe_ne, r.u_moe_k, r.u_moe_gmode, 2l)
            if (r.u_moe_nsh != null) {
                var bw_shg = upload_region(addr < void? >(t.fblob[t.shexp_gate_off + l * dim]), uint64(dim * 4l))
                enc_moe_router(enc, bw_shg, r.bxb, r.bmoe_lg, r.u_dim, r.u_one32, r.u_dim, g_one, g_zero, 1l, 2l)
            }
            enc_moe_gemv(enc, t, fe1, t.we1_offs[l], c.n_ff_exp, c.n_expert_used, 2l, r.bxb, r.bh12,
                r.u_dim, r.u_moe_nfe, r.bmoe_sel, fe1 == KqFmt.q8 || fe1 == KqFmt.q51 ? r.u_moe_eblk : r.u_moe_esb, r.u_zero32,
                r.u_moe_ss, r.u_dim, r.u_moe_knfe)
            enc_moe_gemv(enc, t, fe3, t.we3_offs[l], c.n_ff_exp, c.n_expert_used, 2l, r.bxb, r.bh12,
                r.u_dim, r.u_moe_nfe, r.bmoe_sel, fe3 == KqFmt.q8 || fe3 == KqFmt.q51 ? r.u_moe_eblk : r.u_moe_esb, r.u_zero32,
                r.u_moe_ss, r.u_dim, r.u_moe_knfe, uint64(2l * knfe * 4l))
            enc_ew2(enc, g_pso_swiglu, r.bh12, 0ul, r.bh12, uint64(2l * knfe * 4l), r.u_moe_tot, 2l * knfe)
            enc_moe_gemv(enc, t, fe2, t.we2_offs[l], dim, c.n_expert_used, 2l, r.bh12, r.bmoe_dn,
                r.u_moe_nfe, r.u_dim, r.bmoe_sel, fe2 == KqFmt.q8 || fe2 == KqFmt.q51 ? r.u_moe_eblk : r.u_moe_esb, r.u_moe_xs_dn,
                r.u_moe_ss, r.u_moe_knfe, r.u_moe_kdim)
            if (r.u_moe_nsh != null) {
                // shexp FFN reads bxb BEFORE combine overwrites it; bh12 is free after expert W2
                let nsh = c.n_ff_shexp
                let she = dim * nsh
                let bwsh1 = blob_of(g_dev, t, t.wsh1_off + l * she)
                enc_gemv_w13sw_b(enc, false, bwsh1.buf, bwsh1.boff, uint64(((t.wsh3_off + l * she) / 32l) * 34l),
                    r.bxb, r.bh12, r.u_dim, r.u_moe_nsh, nsh)
                let bwsh2 = blob_of(g_dev, t, t.wsh2_off + l * she)
                enc_gemv_b(enc, false, bwsh2.buf, bwsh2.boff, r.bh12, r.bxb2, r.u_moe_nsh, r.u_dim, r.u_dim, dim)
            }
            enc_moe_combine(enc, r.bmoe_dn, r.bmoe_sel, r.bxb, r.u_dim, r.u_moe_k, dim, 2l)
            if (r.u_moe_nsh != null) {
                enc_axpy_sig(enc, r.bxb, r.bxb2, r.bmoe_lg, r.u_total_dim, r.u_dim, 2l * dim)
            }
        } else {
            let f1 = kq_fmt_at(t.w1_fmt, l)
            let f2 = kq_fmt_at(t.w2_fmt, l)
            let f3 = kq_fmt_at(t.w3_fmt, l)
            if (f1 == KqFmt.q8 && f2 == KqFmt.q8 && f3 == KqFmt.q8) {
                let bw1 = blob_of(g_dev, t, t.w1_offs[l])
                let bw3 = blob_of(g_dev, t, t.w3_offs[l])
                enc_gemv_w13sw_b(enc, false, bw1.buf, bw1.boff, bw3.boff, r.bxb, r.bh12, r.u_dim, r.u_hidden, hidden)
                let bw2 = blob_of(g_dev, t, t.w2_offs[l])
                enc_gemv_b(enc, false, bw2.buf, bw2.boff, r.bh12, r.bxb, r.u_hidden, r.u_dim, r.u_dim, dim)
            } else {
                // gate rows land at 0, up rows at 2*hidden (bh12 is nrows * 2*hidden); swiglu fuses in place
                enc_verify_site(t, r, enc, f1, t.w1_offs[l], hidden, dim, hidden, r.bxb, r.bh12, r.u_dim, r.u_hidden, r.u_hidden)
                enc_verify_site(t, r, enc, f3, t.w3_offs[l], hidden, dim, hidden, r.bxb, r.bh12, r.u_dim, r.u_hidden, r.u_hidden, [ycol = 2l * hidden])
                enc_ew2(enc, g_pso_swiglu, r.bh12, 0ul, r.bh12, uint64(2l * hidden * 4l), r.u_total_h, 2l * hidden)
                enc_verify_site(t, r, enc, f2, t.w2_offs[l], dim, hidden, dim, r.bh12, r.bxb, r.u_hidden, r.u_dim, r.u_dim)
            }
        }
        if (l + 1l < c.n_layers) {
            var bw_attn = upload_region(addr < void? >(t.fblob[t.rms_att_off + (l + 1l) * dim]), uint64(dim * 4l))
            if (dim <= ADD_RMS_MAX_DIM) {
                enc_add_rms_b(enc, r.bx, r.bxb, bw_attn, r.bxb, r.u_dim, r.u_eps, 2l)
            } else {
                enc_ew2(enc, g_pso_add, r.bx, 0ul, r.bxb, 0ul, r.u_total_dim, 2l * dim)
                enc_rms_b(enc, r.bx, bw_attn, r.bxb, r.u_dim, r.u_eps, 2l)
            }
        } else {
            enc_ew2(enc, g_pso_add, r.bx, 0ul, r.bxb, 0ul, r.u_total_dim, 2l * dim)
        }
    }
}

// the 2-row verify [tok@pos, d@pos+1]: batch-form sites at nrows=2, SAME-SLAB route table
// (cnt pos+1/pos+2 — the serial encoder orders row0's KV write before row1's read); dn scans
// npos=2 in place on the decode DnMirror. Caller poked bx (embed pair) + 2-row rope tables.
def private encode_verify_step(t : Model; var r : StepRes; pos : int64) {
    let c = t.config
    let dim = c.dim
    let esz = uint64(kv_row_bytes(r.kdt, 1l))
    // per-layer same-slab route entries (elements); the +nextn entry = the warm's draft slab
    g_vrt |> resize(int((c.n_layers + c.n_layer_nextn) * 8l))
    for (l in range64(c.n_layers + c.n_layer_nextn)) {
        let lb = uint((r.koff + uint64(mirror_lbase(t, r.kdt, r.cap, l))) / esz)
        let vb = uint((r.voff + uint64(mirror_lbase(t, r.kdt, r.cap, l))) / esz)
        g_vrt[l * 8l] = lb
        g_vrt[l * 8l + 1l] = vb
        g_vrt[l * 8l + 2l] = uint(r.cap)
        g_vrt[l * 8l + 3l] = uint(pos + 1l)
        g_vrt[l * 8l + 4l] = lb
        g_vrt[l * 8l + 5l] = vb
        g_vrt[l * 8l + 6l] = uint(r.cap)
        g_vrt[l * 8l + 7l] = uint(pos + 2l)
    }
    let bytes_rt = uint64((c.n_layers + c.n_layer_nextn) * 32l)
    var brt = pool_acquire_untracked(g_upool, g_dev, bytes_rt)
    unsafe {
        memcpy(metal_buffer_contents(brt), addr < void? >(g_vrt[0]), int(bytes_rt))
    }
    var cb = metal_new_command_buffer_unretained(g_queue)
    var enc = metal_new_compute_encoder(cb)
    unsafe {
        var bw_att0 = upload_region(addr < void? >(t.fblob[t.rms_att_off]), uint64(dim * 4l))
        enc_rms_b(enc, r.bx, bw_att0, r.bxb, r.u_dim, r.u_eps, 2l)
    }
    var dnli = 0l
    for (l in range64(c.n_layers)) {
        encode_verify_layer(t, r, enc, brt, l, pos, dnli)
    }
    // both rows norm+classify: row0 = the verify truth, row1 = the accept-path continuation
    unsafe {
        var bwfin = upload_region(addr < void? >(t.fblob[t.rms_final_off]), uint64(dim * 4l))
        enc_rms_b(enc, r.bx, bwfin, r.bxf, r.u_dim, r.u_eps, 2l)
    }
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    enc_verify_site(t, r, enc, cls_fmt, t.wcls_off, c.vocab_size, dim, c.vocab_size,
        r.bxf, r.blog, r.u_dim, r.u_vocab, r.u_vocab)
    // draft-slab warm: the draft block at B=2, no cls — future drafts need contiguous slab rows
    unsafe {
        enc_copy_row(enc, r.bcat, uint64(2l * dim * 4l), r.bxb, 0ul, r.u_dim, dim)
        enc_copy_row(enc, r.bxf, 0ul, r.bxb, uint64(dim * 4l), r.u_dim, dim)
        var bw_hn = upload_region(addr < void? >(t.fblob[t.mtp_hnorm_off]), uint64(dim * 4l))
        enc_rms_b(enc, r.bxb, bw_hn, r.bxb2, r.u_dim, r.u_eps, 2l)
        var bw_en = upload_region(addr < void? >(t.fblob[t.mtp_enorm_off]), uint64(dim * 4l))
        enc_rms_b(enc, r.bcat, bw_en, r.bh12, r.u_dim, r.u_eps, 2l)
        enc_copy_row(enc, r.bh12, 0ul, r.bcat, 0ul, r.u_dim, dim)
        enc_copy_row(enc, r.bxb2, 0ul, r.bcat, uint64(dim * 4l), r.u_dim, dim)
        enc_copy_row(enc, r.bh12, uint64(dim * 4l), r.bcat, uint64(2l * dim * 4l), r.u_dim, dim)
        enc_copy_row(enc, r.bxb2, uint64(dim * 4l), r.bcat, uint64(3l * dim * 4l), r.u_dim, dim)
        enc_verify_site(t, r, enc, t.mtp_ehproj_fmt, t.mtp_ehproj_off, dim, 2l * dim, dim,
            r.bcat, r.bx, r.u_dim2, r.u_dim, r.u_dim)
        var bw_att = upload_region(addr < void? >(t.fblob[t.rms_att_off + c.n_layers * dim]), uint64(dim * 4l))
        enc_rms_b(enc, r.bx, bw_att, r.bxb, r.u_dim, r.u_eps, 2l)
    }
    var dnw = 0l
    encode_verify_layer(t, r, enc, brt, c.n_layers, pos, dnw)
    metal_end_encoding(enc)
    metal_release(enc)
    metal_commit(cb)
    r.committed = true
    r.cb = cb
    pool_release(g_upool, brt, bytes_rt)
}

// land the verify: KV rows + row0 logits -> s.mtp_logits, row1 -> s.logits; accept/reject bookkeeping is the caller's
def private finish_verify_step(t : Model; var s : Session; r : StepRes; pos : int64) : bool {
    metal_wait_until_completed(r.cb)
    let err = metal_command_buffer_error(r.cb)
    let ok = empty(err)
    if (ok) {
        let c = t.config
        unsafe {
            var kmp = reinterpret<uint8?>(metal_buffer_contents(g_arena)) + r.koff
            var vmp = reinterpret<uint8?>(metal_buffer_contents(g_arena)) + r.voff
            for (l in range64(c.n_layers + c.n_layer_nextn)) {   // + the warmed draft-slab rows
                let rowb = kv_row_bytes(s.kv_dtype_k, layer_kv_dim(c, l))
                let lb = mirror_lbase(t, s.kv_dtype_k, r.cap, l)
                for (i in range64(2l)) {
                    let prow = kv_phys_row(s, pos + i)
                    memcpy(reinterpret<void?>(kv_layer_kbase(s, l, c.seq_len) + prow * rowb),
                        reinterpret<void?>(kmp + lb + (pos + i) * rowb), int(rowb))
                    memcpy(reinterpret<void?>(kv_layer_vbase(s, l, c.seq_len) + prow * rowb),
                        reinterpret<void?>(vmp + lb + (pos + i) * rowb), int(rowb))
                }
            }
            var lg = reinterpret<uint8?>(metal_buffer_contents(r.blog))
            memcpy(addr < void? >(s.mtp_logits[0]), reinterpret<void?>(lg), int(c.vocab_size * 4l))
            memcpy(addr < void? >(s.logits[0]), reinterpret<void?>(lg + c.vocab_size * 4l), int(c.vocab_size * 4l))
        }
    } else {
        to_log(LOG_ERROR, "dasLLAMA metal MTP verify: dispatch failed ({err})\n")   // nolint:PERF028,PERF026 — error path, and the CPU fallback dwarfs the log
        decline(MetalDecodeDecline.gpu_error)
    }
    return ok
}

//! mtp_spec_eval's GPU twin: draft + B=2 same-slab verify, wholly on the metal decode path.
//! Cold/decline paths fall back to one plain (GPU) forward — output-invariant either way.
//! Reject is ~free: row0 IS the truth decode, the dn shadow region flips in — no re-forward.
[no_env, no_io]
def metal_mtp_spec_eval(t : Model; var s : Session; tok : int64; var accepted : int64&) : bool {
    let c = t.config
    let pos = s.n_past
    var cold = (g_dev == null || !t.metal_blob || c.n_layer_nextn <= 0l || pos < 1l ||
        s.mtp_h_pos1 != pos || pos + 2l > c.seq_len || !metal_mtp_verify_ok(t, s))
    if (!cold && c.recr_mask != 0ul) {
        // the verify scans the decode DnMirror's live region — it must be resident and current
        cold = !key_exists(g_dn_mirrors, s.uid) || g_dn_mirrors[s.uid].pos != pos
    }
    if (!cold) {
        if (!key_exists(g_mirrors, s.uid)) {
            cold = true
        } else {
            let kmr = g_mirrors[s.uid]
            cold = kmr.watermark < pos || kmr.cap_rows < pos + 2l
        }
    }
    if (cold && g_env_mtp_debug == "cold") {   // bring-up bisect: log the failing gate
        let vok_g = metal_mtp_verify_ok(t, s)
        let dnp = c.recr_mask != 0ul && key_exists(g_dn_mirrors, s.uid) ? g_dn_mirrors[s.uid].pos : -1l
        let wm = key_exists(g_mirrors, s.uid) ? g_mirrors[s.uid].watermark : -1l
        to_log(LOG_INFO, "mtp cold: pos={pos} h_pos1={s.mtp_h_pos1} seq={c.seq_len} verify_ok={vok_g} dn_pos={dnp} watermark={wm}\n")   // nolint:PERF028,PERF026 — behind the DASLLAMA_MTP_DEBUG=cold bring-up gate
    }
    if (!cold) {
        // the warm's row-pos h input is the PRE-draft trunk hidden — the draft overwrites mtp_h
        s.mtp_xb_save |> resize(int(c.dim))
        unsafe {
            memcpy(addr < void? >(s.mtp_xb_save[0]), addr < void? >(s.mtp_h[0]), int(c.dim * 4l))
        }
        cold = !metal_mtp_draft_forward(t, s, tok, pos)
        if (cold && g_env_mtp_debug == "cold") {
            to_log(LOG_INFO, "mtp cold: draft declined at pos={pos}\n")   // nolint:PERF028,PERF026 — behind the DASLLAMA_MTP_DEBUG=cold bring-up gate
        }
    }
    if (cold) {
        forward(t, s, tok, pos)
        s.n_past = pos + 1l
        return false
    }
    var d = parallel_argmax(s.logits, c.vocab_size)
    if (g_env_mtp_debug == "reject") {   // bring-up bisect: force the reject path
        d = 0l
    }
    s.mtp_drafted++
    mtp_verify_prep(t, s, tok, d, pos)
    finish_pending_step()
    let mr = g_mirrors[s.uid]
    var r = acquire_step(t, s, pos + 1l, mr.koff, mr.voff, mr.cap_rows, [nrows = 2l])
    if (!r.gpu_logits) {   // verify has no CPU classifier fallback (blog/bxf live under gpu_logits)
        release_step(r)
        forward(t, s, tok, pos)
        s.n_past = pos + 1l
        return false
    }
    if (r.dn) {
        var dm = dn_mirror_prepare(s, pos, dn_mirror_regions(t))
        if (!dm.ok) {
            release_step(r)
            forward(t, s, tok, pos)
            s.n_past = pos + 1l
            return false
        }
        r.bdn_state = dm.buf
        r.dn_state_base = dm.state_off
        r.dn_conv_base = dm.conv_off
        r.dn_state_alt = dm.state_alt
        r.dn_conv_alt = dm.conv_alt
    }
    unsafe {
        memcpy(metal_buffer_contents(r.bx), addr < void? >(s.mtp_cat[0]), int(r.bytes_x))
        memcpy(metal_buffer_contents(r.bcos), addr < void? >(s.rope_cos[0]), int(r.bytes_tab))
        memcpy(metal_buffer_contents(r.bsin), addr < void? >(s.rope_sin[0]), int(r.bytes_tab))
        // warm inputs: embed pair + the saved pre-draft h — bcat is untouched by the trunk pass
        var bc = reinterpret<uint8?>(metal_buffer_contents(r.bcat))
        memcpy(reinterpret<void?>(bc), addr < void? >(s.mtp_cat[0]), int(2l * c.dim * 4l))
        memcpy(reinterpret<void?>(bc + 2l * c.dim * 4l), addr < void? >(s.mtp_xb_save[0]), int(c.dim * 4l))
    }
    encode_verify_step(t, r, pos)
    let vok = finish_verify_step(t, s, r, pos)
    let truth = vok ? parallel_argmax(s.mtp_logits, c.vocab_size) : -1l
    if (vok && truth == d) {
        unsafe {
            var hx = reinterpret<uint8?>(metal_buffer_contents(r.bxf))
            memcpy(addr < void? >(s.mtp_h[0]), reinterpret<void?>(hx + c.dim * 4l), int(c.dim * 4l))
        }
        release_step(r)
        s.mtp_h_pos1 = pos + 2l
        g_mirrors[s.uid].watermark = pos + 2l
        if (c.recr_mask != 0ul) {
            dn_mirror_advance(s.uid, pos + 1l)
        }
        s.mtp_accepted++
        accepted = d
        s.n_past = pos + 2l
        return true
    }
    if (vok) {
        // reject: row0 already IS tok@pos's plain decode — its logits, hidden, and the shadow
        // dn region (post-row0) stand in for the re-forward; the garbage draft row sits above
        // the watermark and the next step rewrites it
        unsafe {
            var hx = reinterpret<uint8?>(metal_buffer_contents(r.bxf))
            memcpy(addr < void? >(s.mtp_h[0]), reinterpret<void?>(hx), int(c.dim * 4l))
            memcpy(addr < void? >(s.logits[0]), addr < void? >(s.mtp_logits[0]), int(c.vocab_size * 4l))
        }
        release_step(r)
        s.mtp_h_pos1 = pos + 1l
        g_mirrors[s.uid].watermark = pos + 1l
        if (c.recr_mask != 0ul) {
            dn_mirror_flip(s.uid, pos + 1l)
        }
        s.n_past = pos + 1l
        return false
    }
    release_step(r)
    if (c.recr_mask != 0ul && key_exists(g_dn_mirrors, s.uid)) {
        // verify dispatch failed mid-scan — the mirror state is unreliable and not re-derivable
        dn_mirror_release(g_dn_mirrors[s.uid])
        g_dn_mirrors |> erase(s.uid)
    }
    forward(t, s, tok, pos)
    s.n_past = pos + 1l
    return false
}

// DASLLAMA_METAL_DECODE_CONCURRENT (default ON): concurrent encoder for the single-stream step,
// barriers only at tracker-detected hazards (hz_gate in every decode-path enc_* helper); g_skip
// knockouts force serial — skipped dispatches break the declared graph
var private g_dconc = -1

def private decode_concurrent : bool {
    if (g_dconc < 0) {
        load_decode_concurrent()
    }
    return g_dconc != 0
}

//! Concurrent-encoder rail for the single-stream decode: 1 forces on, 0 forces serial,
//! negative restores the env default (`DASLLAMA_METAL_DECODE_CONCURRENT`).
def public set_metal_decode_concurrent(v : int) {
    g_dconc = v < 0 ? -1 : (v != 0 ? 1 : 0)
}

// DASLLAMA_METAL_FUSE (default ON): the R1 epilogue-fusion kernel MetalPreAddRms (post_attn_rms +
// add_rms, the pre_post_norm path) — off restores the separate per-op dispatches for A/B. (The g4
// post_ffn1_rms+add fusion was measured and REFUTED, −1.0% on the 26B — see the ledger.)
var private g_dfuse = -1

def private decode_fuse_pre : bool {
    if (g_dfuse < 0) {
        load_decode_fuse()
    }
    return g_dfuse != 0
}

//! Epilogue-fusion rail for the single-stream decode: 1 forces on, 0 forces the separate
//! per-op dispatches, negative restores the env default (`DASLLAMA_METAL_FUSE`).
def public set_metal_decode_fuse(v : int) {
    g_dfuse = v < 0 ? -1 : (v != 0 ? 1 : 0)
}

// encode the whole step (prologue rms, n_layers fused blocks, optional GPU classifier) into a
// fresh command buffer; commit_now submits it, otherwise the caller commits after poking the
// inputs (the encode-ahead rail). A spec step OPENS with argmax + embed gather — no CPU poke.
def private encode_step(t : Model; var r : StepRes; commit_now : bool; var prev_blog : MetalBuffer? = null) {
    let c = t.config
    let dim = c.dim
    // UNRETAINED: skips per-dispatch retain/release of ~2000 resource refs — everything referenced outlives the cb here
    var cb = metal_new_command_buffer_unretained(g_queue)
    let conc = decode_concurrent() && empty(g_skip)
    var enc = conc ? metal_new_compute_encoder_concurrent(cb) : metal_new_compute_encoder(cb)
    gr_arm(conc)   // capture the step as a graph; graph_flush replays it in schedule order
    let hz_d0 = conc ? metal_dispatch_calls() : 0l
    let hz_g0 = g_hz_gated
    if (r.spec) {
        // speculative head: argmax the previous step's logits, gather the winner's embedding row into r.bx (cls_q8 or tied-k6 planes); the escale slot applies embed_scale in the gather
        unsafe {
            enc_argmax(enc, prev_blog, r.btok, r.u_vocab)
            if (t.cls_q8) {
                let bemb = blob_of(g_dev, t, t.emb_q8_off)
                enc_embed(enc, bemb.buf, bemb.boff, r.btok, r.bx, r.u_dim, r.u_embscale, dim)
            } else {
                let bembq = kq_quants_of(g_dev, t, KqFmt.k6, t.wcls_off)
                let bembs = kq_scales_of(g_dev, t, KqFmt.k6, t.wcls_off)
                enc_embed_k6(enc, bembq.buf, bembq.qoff, bembs.buf, bembs.soff, bembs.doff, r.btok, r.bx, r.u_dim, r.u_embscale, dim)
            }
        }
    }
    // layer-0 attention norm — the per-layer trailing add+rms is fused and feeds the NEXT layer
    if (g_skip != "ew") {
        unsafe {
            var bw_att0 = upload_region(addr < void? >(t.fblob[t.rms_att_off]), uint64(dim * 4l))
            enc_rms(enc, r.bx, bw_att0, r.bxb, r.u_dim, r.u_eps)
        }
    }
    var dnli = 0l   // recurrent-layer index — addresses the dn state/conv mirror slices in order
    for (l in range64(c.n_layers)) {
        encode_layer(t, r, enc, l, dnli)
    }
    if (r.gpu_logits) {
        encode_cls_tail(t, r, enc, t.rms_final_off)
    }
    if (conc) {
        graph_flush_sched(enc)
        hz_coverage_check(metal_dispatch_calls() - hz_d0, g_hz_gated - hz_g0)
    }
    metal_end_encoding(enc)
    metal_release(enc)
    if (commit_now) {
        metal_commit(cb)
        r.committed = true
        if (g_step_trace_cap > 0l) {
            r.t_commit = int64(get_time_usec(g_step_trace_t0))
        }
    }
    r.cb = cb
}

// wait for the step's cb, then hand results back: s.x, the mirrored KV row VERBATIM into the CPU
// cache (the CPU never re-encodes), s.logits when the GPU classifier ran, watermark advance
def private finish_step(t : Model; var s : Session; r : StepRes) : bool {
    let ts_wait = ref_time_ticks()
    metal_wait_until_completed(r.cb)
    let err = metal_command_buffer_error(r.cb)
    g_gpu_ms += (metal_command_buffer_gpu_end_time(r.cb) - metal_command_buffer_gpu_start_time(r.cb)) * 1000.0lf
    let wait_us = int64(get_time_usec(ts_wait))
    g_us_wait += wait_us
    let ok = empty(err)
    if (ok) {
        let ts_rb = ref_time_ticks()
        let c = t.config
        let pos = r.pos
        let cap = r.cap
        unsafe {
            memcpy(addr < void? >(s.x[0]), metal_buffer_contents(r.bx), int(r.bytes_x))
            var kmp = reinterpret<uint8?>(metal_buffer_contents(g_arena)) + r.koff
            var vmp = reinterpret<uint8?>(metal_buffer_contents(g_arena)) + r.voff
            let prow = kv_phys_row(s, pos)
            for (l in range64(c.n_layers)) {
                continue if (kv_shared_layer(t, l))   // no rows of its own — the source's copy covers it
                let rowb = kv_row_bytes(s.kv_dtype_k, layer_kv_dim(c, l))
                let lb = mirror_lbase(t, s.kv_dtype_k, cap, l)
                memcpy(reinterpret<void?>(kv_layer_kbase(s, l, c.seq_len) + prow * rowb),
                    reinterpret<void?>(kmp + lb + pos * rowb), int(rowb))
                memcpy(reinterpret<void?>(kv_layer_vbase(s, l, c.seq_len) + prow * rowb),
                    reinterpret<void?>(vmp + lb + pos * rowb), int(rowb))
            }
            if (r.gpu_logits && r.bnc == null) {   // zero-copy steps: the GPU already wrote s.logits
                memcpy(addr < void? >(s.logits[0]), metal_buffer_contents(r.blog), int(r.bytes_log))
            }
            if (r.gpu_logits && c.n_layer_nextn > 0l) {
                // forward()'s mtp_h stash contract — bxf holds the post-final-norm hidden this row
                memcpy(addr < void? >(s.mtp_h[0]), metal_buffer_contents(r.bxf), int(r.bytes_x))
            }
        }
        if (r.gpu_logits && c.n_layer_nextn > 0l) {
            s.mtp_h_pos1 = r.pos + 1l
        }
        if (r.gpu_logits) {
            decode_override_logits_done()
        }
        g_mirrors[s.uid].watermark = r.pos + 1l
        if (r.dn) {   // the GPU state now holds pos's update — advance the mirror's forward-only cursor
            dn_mirror_advance(s.uid, r.pos)
        }
        g_us_readback += int64(get_time_usec(ts_rb))
        g_decodes++
        if (int64(length(g_step_trace)) < g_step_trace_cap) {
            g_step_trace |> push(MetalDecodeStepTrace(pos = r.pos, mode = r.trace_mode,
                commit_us = r.t_commit, finish_us = int64(get_time_usec(g_step_trace_t0)),
                wait_us = wait_us,
                ks = metal_command_buffer_kernel_start_time(r.cb),
                ke = metal_command_buffer_kernel_end_time(r.cb),
                gs = metal_command_buffer_gpu_start_time(r.cb),
                ge = metal_command_buffer_gpu_end_time(r.cb)))
        }
    } else {
        to_log(LOG_ERROR, "dasLLAMA metal decode: dispatch failed ({err}) — falling back to the CPU layer loop\n")   // nolint:PERF028,PERF026 — error path, and the CPU fallback dwarfs the log
        decline(MetalDecodeDecline.gpu_error)
    }
    return ok
}

def private release_step(var r : StepRes) {
    if (r.cb != null) {
        metal_release(r.cb)
        r.cb = null
    }
    if (r.spec) {
        pool_release(g_pool, r.btok, 64ul)
    }
    pool_release(g_pool, r.bx, r.bytes_x)
    pool_release(g_pool, r.bxb, r.bytes_xb)
    pool_release(g_pool, r.bxb2, r.bytes_x)
    pool_release(g_pool, r.bqkv, r.bytes_qkv)
    pool_release(g_pool, r.bh12, r.bytes_h2)
    if (r.bh12s != null) {
        pool_release(g_pool, r.bh12s, r.bytes_h12s)
    }
    pool_release(g_upool, r.bcos, r.bytes_tab)
    pool_release(g_upool, r.bsin, r.bytes_tab)
    if (r.bcos_swa != null) {
        pool_release(g_upool, r.bcos_swa, r.bytes_tab_sw)
        pool_release(g_upool, r.bsin_swa, r.bytes_tab_sw)
    }
    if (r.gpu_logits) {
        pool_release(g_pool, r.bxf, r.bytes_x)
        if (r.blog != null) {   // bnc steps have no staging; bnc itself is registry-owned
            pool_release(g_pool, r.blog, r.bytes_log)
        }
        pool_release(g_upool, r.u_vocab, 4ul)
    }
    if (r.chunked) {
        pool_release(g_pool, r.bpart, r.bytes_part)
        pool_release(g_upool, r.u_nch, 4ul)
    }
    if (r.bcat != null) {
        pool_release(g_pool, r.bcat, 2ul * r.bytes_x)
        pool_release(g_upool, r.u_dim2, 4ul)
    }
    if (r.verify) {
        pool_release(g_upool, r.u_two32, 4ul)
        pool_release(g_upool, r.u_total_dim, 4ul)
        pool_release(g_upool, r.u_total_h, 4ul)
        pool_release(g_upool, r.u_total_qd, 4ul)
    }
    if (r.u_nblk != null) {
        pool_release(g_upool, r.u_nblk, 4ul)
    }
    pool_release(g_upool, r.u_dim, 4ul)
    pool_release(g_upool, r.u_qd, 4ul)
    pool_release(g_upool, r.u_kvd, 4ul)
    pool_release(g_upool, r.u_qkvd, 4ul)
    pool_release(g_upool, r.u_hidden, 4ul)
    pool_release(g_upool, r.u_hs, 4ul)
    pool_release(g_upool, r.u_kvm, 4ul)
    pool_release(g_upool, r.u_nh, 4ul)
    pool_release(g_upool, r.u_cnt, 4ul)
    pool_release(g_upool, r.u_npq, 4ul)
    pool_release(g_upool, r.u_npall, 4ul)
    pool_release(g_upool, r.u_neox, 4ul)
    pool_release(g_upool, r.u_hasb, 4ul)
    pool_release(g_upool, r.u_hasn, 4ul)
    pool_release(g_upool, r.u_scale, 4ul)
    pool_release(g_upool, r.u_eps, 4ul)
    pool_release(g_upool, r.u_softcap, 4ul)
    pool_release(g_upool, r.u_zero32, 4ul)
    pool_release(g_upool, r.u_one32, 4ul)
    if (r.u_winsl != null) {
        pool_release(g_upool, r.u_winsl, 4ul)
    }
    if (r.u_chlo != null) {
        pool_release(g_upool, r.u_chlo, 4ul)
    }
    if (r.u_flcap != null) {
        pool_release(g_upool, r.u_flcap, 4ul)
    }
    if (r.hetero) {
        pool_release(g_upool, r.u_hs_sw, 4ul)
        pool_release(g_upool, r.u_kvd_sw, 4ul)
        pool_release(g_upool, r.u_qd_sw, 4ul)
        pool_release(g_upool, r.u_qkvd_sw, 4ul)
        pool_release(g_upool, r.u_kvm_sw, 4ul)
        pool_release(g_upool, r.u_scale_sw, 4ul)
        pool_release(g_upool, r.u_npq_sw, 4ul)
        pool_release(g_upool, r.u_npall_sw, 4ul)
        if (r.u_nblk_sw != null) {
            pool_release(g_upool, r.u_nblk_sw, 4ul)
        }
        if (r.u_voffe_sw != null) {
            pool_release(g_upool, r.u_voffe_sw, 4ul)
        }
    }
    if (r.u_voffe != null) {
        pool_release(g_upool, r.u_voffe, 4ul)
    }
    if (r.u_embscale != null) {
        pool_release(g_upool, r.u_embscale, 4ul)
    }
    if (r.bsupp != null) {
        pool_release(g_upool, r.bsupp, r.bytes_supp)
        pool_release(g_upool, r.u_nsupp, 4ul)
    }
    if (r.moe) {
        pool_release(g_pool, r.bmoe_lg, r.bytes_moe_lg)
        pool_release(g_pool, r.bmoe_sel, r.bytes_moe_sel)
        pool_release(g_pool, r.bmoe_dn, r.bytes_moe_dn)
        pool_release(g_upool, r.u_moe_ne, 4ul)
        pool_release(g_upool, r.u_moe_k, 4ul)
        pool_release(g_upool, r.u_moe_nfe, 4ul)
        pool_release(g_upool, r.u_moe_knfe, 4ul)
        pool_release(g_upool, r.u_moe_ss, 4ul)
        pool_release(g_upool, r.u_moe_kdim, 4ul)
        pool_release(g_upool, r.u_moe_tot, 4ul)
        pool_release(g_upool, r.u_moe_eblk, 4ul)
        pool_release(g_upool, r.u_moe_esb, 4ul)
        pool_release(g_upool, r.u_moe_xs_dn, 4ul)
        if (r.bmoe_rt != null) {
            pool_release(g_pool, r.bmoe_rt, r.bytes_x)
            pool_release(g_upool, r.u_moe_invd, 4ul)
        }
        if (r.u_moe_nsh != null) {
            pool_release(g_upool, r.u_moe_nsh, 4ul)
        }
        pool_release(g_upool, r.u_moe_gmode, 4ul)
    }
    if (r.u_hass != null) {
        pool_release(g_upool, r.u_hass, 4ul)
    }
    if (r.dn) {
        pool_release(g_pool, r.bdn_qkv, r.bytes_dn_cd)
        pool_release(g_pool, r.bdn_cv, r.bytes_dn_cd)
        pool_release(g_pool, r.bdn_z, r.bytes_dn_di)
        pool_release(g_pool, r.bdn_o, r.bytes_dn_di)
        pool_release(g_pool, r.bdn_b, r.bytes_dn_h)
        pool_release(g_pool, r.bdn_g, r.bytes_dn_h)
        pool_release(g_upool, r.u_cd, 4ul)
        pool_release(g_upool, r.u_dn_kd, 4ul)
        pool_release(g_upool, r.u_ds, 4ul)
        pool_release(g_upool, r.u_nvh, 4ul)
        pool_release(g_upool, r.u_nkh, 4ul)
        pool_release(g_upool, r.u_di, 4ul)
        pool_release(g_upool, r.u_dconv, 4ul)
        pool_release(g_upool, r.u_qscale, 4ul)
        r.bdn_state = null   // mirror-owned
    }
    if (r.bdn_qg != null) {
        pool_release(g_pool, r.bdn_qg, 2ul * r.bytes_q)
        pool_release(g_pool, r.bdn_gate, r.bytes_q)
        pool_release(g_upool, r.u_dn_qd2, 4ul)
    }
    if (r.u_rot != null) {
        pool_release(g_upool, r.u_rot, 4ul)
    }
    if (r.ple) {
        pool_release(g_upool, r.bple, r.bytes_ple)
        pool_release(g_upool, r.u_ple, 4ul)
    }
    r.valid = false
}

// Retire the pre-encoded step. A poked-mode pre was never committed — releasing it drops the work.
// A spec pre is already running: wait it out and SURFACE any execution error — an eaten fault here
// would poison every later submission ("prior/excessive GPU errors") with no trace of the true site.
def private discard_pre {
    if (!g_pre.valid) {
        return
    }
    var r = g_pre
    g_pre.valid = false
    if (r.committed) {
        metal_wait_until_completed(r.cb)
        let err = metal_command_buffer_error(r.cb)
        if (!empty(err)) {
            to_log(LOG_ERROR, "dasLLAMA metal decode: discarded pre-step failed ({err})\n")   // nolint:PERF028,PERF026 — error path on a discarded pre-step
        }
    }
    release_step(r)
}

// Pre-encode the PREDICTED next step (same session, pos + 1) while the current step runs on the
// GPU. When speculation gates pass, the cb commits AND chains itself (argmax + embed gather, no
// CPU poke); otherwise it stays uncommitted. Quietly declines when gates fail — slow path serves those.
def private pre_encode_next(t : Model; s : Session; npos : int64; var prev_blog : MetalBuffer?) {
    // no encode-ahead for recurrent (two cbs race the one dn state buffer); a declined/absent-mirror skip = slow path serves it
    if (t.config.recr_mask != 0ul ||
            decode_decline(t, s, npos) != MetalDecodeDecline.none || !key_exists(g_mirrors, s.uid)) {
        return
    }
    let mr = g_mirrors[s.uid]   // cheap copy, capacity check ONLY — row npos-1 is in flight on the GPU
    if (mr.cap_rows < npos + 1l || mr.kdt != s.kv_dtype_k || mr.vdt != s.kv_dtype_v) {
        return
    }
    let ts_enc = ref_time_ticks()
    var r = acquire_step(t, s, npos, mr.koff, mr.voff, mr.cap_rows)
    if (r.gpu_logits && prev_blog != null && spec_cls_capable(t) &&
            !has_ple(t.config) && spec_armed()) {
        // speculative chain: the cb runs the moment the previous one drains — CPU tail/sampling/next forward() all overlap GPU execution
        r.spec = true
        // a spec step is IN FLIGHT after forward() returns — must not touch session memory (staging only; acceptance lands it via finish_step)
        r.bnc = null
        r.btok = pool_acquire(g_pool, g_dev, 64ul)
        build_decode_rope_row(t, npos, g_rope_cos_next, g_rope_sin_next)
        unsafe {
            memcpy(metal_buffer_contents(r.bcos), addr < void? >(g_rope_cos_next[0]), int(r.bytes_tab))
            memcpy(metal_buffer_contents(r.bsin), addr < void? >(g_rope_sin_next[0]), int(r.bytes_tab))
        }
        if (r.bcos_swa != null) {
            build_decode_rope_row_swa(t, npos, g_rope_cos_next_swa, g_rope_sin_next_swa)
            unsafe {
                memcpy(metal_buffer_contents(r.bcos_swa), addr < void? >(g_rope_cos_next_swa[0]), int(r.bytes_tab_sw))
                memcpy(metal_buffer_contents(r.bsin_swa), addr < void? >(g_rope_sin_next_swa[0]), int(r.bytes_tab_sw))
            }
        }
        encode_step(t, r, true, prev_blog)
    } else {
        encode_step(t, r, false)
    }
    g_pre = r
    g_us_encode += int64(get_time_usec(ts_enc))
}

// the non-pipelined step: mirror catch-up + encode-now + wait; the NEXT step's pre-encode still
// overlaps this step's GPU run. Serves window starts, prediction misses, and speculation reruns.
def private metal_decode_slow(t : Model; var s : Session; pos : int64) : bool {
    let ts_all = ref_time_ticks()
    let mir = mirror_prepare(t, s, pos)
    if (!mir.ok) {
        decline(MetalDecodeDecline.mirror)
        return false
    }
    var r = acquire_step(t, s, pos, mir.koff, mir.voff, mir.cap)
    if (r.dn) {
        var dm = dn_mirror_prepare(s, pos, dn_mirror_regions(t))
        if (!dm.ok) {   // state not resident and no CPU prefix to sync from — decline this step
            release_step(r)
            decline(MetalDecodeDecline.dn_state)
            return false
        }
        r.bdn_state = dm.buf
        r.dn_state_base = dm.state_off
        r.dn_conv_base = dm.conv_off
    }
    poke_step(s, r)
    g_us_setup += int64(get_time_usec(ts_all))
    let ts_enc = ref_time_ticks()
    encode_step(t, r, true)
    g_us_encode += int64(get_time_usec(ts_enc))
    pre_encode_next(t, s, pos + 1l, r.blog)
    let ok = finish_step(t, s, r)
    release_step(r)
    return ok
}

//! The DecodeOverrideFn: runs one token step's whole llama layer stack GPU-resident against the
//! session's KV mirror. true = done (KV row stored CPU-side too, s.x = final residual, s.logits
//! when the GPU classifier gates pass); false = declined, the caller runs the CPU layer loop.
[no_env, no_io]
def metal_decode_forward(t : Model; var s : Session; token, pos : int64) : bool {
    // mixed batch/single schedules: a pipelined batch step may still own this session's KV row
    // and logits — land it before reading anything session-side (no-op when nothing pends)
    finish_pending_step()
    if (weight_caches_stale()) {   // a model (re)load may have recycled cached weight addresses
        discard_pre()
        weight_caches_flush_on_reload()
    }
    g_lp_err = false
    var why = decode_decline(t, s, pos)
    if (why == MetalDecodeDecline.none && !metal_decode_init()) {
        why = MetalDecodeDecline.device
    }
    if (why != MetalDecodeDecline.none) {
        discard_pre()   // the sequence diverged from the prediction
        if (why == MetalDecodeDecline.feature) {
            decline(why, metal_missing_needs(t, DECODE_NEEDS_OK))
        } else {
            decline(why)
        }
        return false
    }
    if (!(g_pre.valid && g_pre.uid == s.uid && g_pre.pos == pos &&
            g_pre.kdt == s.kv_dtype_k && g_pre.s16 == t.wscale_f16 &&
            g_pre.arena_epoch == g_arena_epoch)) {
        discard_pre()
        return metal_decode_slow(t, s, pos)
    }
    // FAST path: this step's cb was encoded during the previous step's GPU run. A spec cb has
    // been running since the previous step drained (GPU-chained argmax + embed); a poked-mode
    // one needs the embed + rope bytes poked, then commits.
    let ts_all = ref_time_ticks()
    var r = g_pre
    g_pre.valid = false
    r.trace_mode = r.spec ? 2l : 1l
    if (!r.spec) {
        poke_step(s, r)
        metal_commit(r.cb)
        r.committed = true
        if (g_step_trace_cap > 0l) {
            r.t_commit = int64(get_time_usec(g_step_trace_t0))
        }
    }
    g_tick++
    g_mirrors[s.uid].tick = g_tick   // keep the live session hot for the LRU
    g_us_setup += int64(get_time_usec(ts_all))
    pre_encode_next(t, s, pos + 1l, r.blog)
    let ok = finish_step(t, s, r)
    if (ok && r.spec) {
        // verify the GPU's greedy pick against the caller's actual token — a miss means this whole
        // step ran on the wrong embedding: drop the chained pre-step and rerun (the garbage KV row
        // sits at/above the rewound watermark, so mirror_prepare's re-upload repairs it).
        var tokp = unsafe(reinterpret<uint?>(metal_buffer_contents(r.btok)))
        let picked = int64(unsafe(tokp[0]))
        if (picked != token) {
            g_spec_misses++
            g_spec_cooldown = g_spec_backoff
            g_spec_backoff = min(g_spec_backoff * 2, 256)
            discard_pre()
            release_step(r)
            g_decodes--   // finish_step counted the mispredicted step; the rerun recounts
            return metal_decode_slow(t, s, pos)
        }
        g_spec_hits++
        g_spec_backoff = 1
    }
    release_step(r)
    return ok
}

// ===== the batched-decode driver (P4) =====

// GEMV-vs-GEMM site policy: the fixed-B batched GEMV forms win to B = 4, the M-pad-32 GEMM from
// B ~ 5 on every decode site (P1a lab — all sites are big shapes: the QKV concat, wo, w1/w3, w2).
// DASLLAMA_METAL_BATCH_GEMM_MIN overrides for A/B.
var private g_bgemm_min = -1l
// the step splits into DASLLAMA_METAL_BATCH_NCB command buffers (llama.cpp's n_cb shape): the
// ~3.2ms/step scheduling span pipelines behind cb k's execution instead of idling as one big cb;
// queue order serializes the pieces, every encode is token-independent. 1 = the single-cb A/B rail.
var private g_bncb = -1l
// per-site split-K / 64-wide policy: at mp=32 the batch GEMM is OCCUPANCY-bound (d/32 threadgroups
// starve small sites) — grid.y k-slices fix it (kv sk8 +135%, wo sk4 +21%, qkv sk2 +9%, w2 sk8 +33%).
// DASLLAMA_METAL_BATCH_SK=0 pins every site back to the planar 32-wide v2 (the A/B rail).
var private g_bsk = true
// DASLLAMA_METAL_BATCH_CONCURRENT: encode on a CONCURRENT compute encoder, barrier only at a real
// hazard (RAW/WAR/WAW) — the only exploitable overlap in our FUSED chain is W1 || W3. Default ON:
// measured +1-2% at every B (pipelines launch overhead even on the RAW chain). =0 = serial (A/B).
var private g_bconcurrent = true
// DASLLAMA_METAL_BATCH_FUSE: fold w1 + w3 + swiglu into one fused dispatch on the fixed-B path
// (B <= 4) — cuts 2 dispatches + their barriers per layer where inter-dispatch bubbles hurt most.
// Default ON; =0 pins the unfused triple (the A/B rail).
var private g_bfuse = true

// the mul_mv-shape GEMV (MetalQ8MvB2/B4 — the de-verbatim'd Part-3 winner, in-graph == the lcpp
// reference) at every fixed-B site instead of the x-staged forms (which measured -10.5% in-graph:
// the walkthrough lab win was SLC-inflated). DASLLAMA_METAL_BATCH_MV=0 = the x-staged A/B rail.
var private g_bmv = true

// B = 5..8 rides two 4-column mv tiles (weights stream twice) instead of the GEMM + split-K
// policy — the reference's own shape below B = 9. Measured +12.7% at B=8 (239.4 vs 212.5 t/s).
// DASLLAMA_METAL_BATCH_MV8=0 = the GEMM-policy A/B rail.
var private g_bmv8 = true

// pin the batched unfused add+rms pair (ew2-add + rms_b) that dim > ADD_RMS_MAX_DIM selects —
// the tests' parity rail for that path on models whose dim fits the fused slab
var private g_b_addrms_unfused = false

def public set_metal_batch_addrms_unfused(v : bool) {
    g_b_addrms_unfused = v
}

// one-deep encode-ahead for the batch driver: a step commits BEFORE the previous is waited on, so
// sessions hold the PREVIOUS step's logits until the next call. WRONG for logit-gated sampling
// (eval_batch's contract) — OPT-IN for benches only: DASLLAMA_METAL_BATCH_PIPE=1, gpu-logits only.
var private g_b_pipe = false
// per-step scratch reused across calls (row table image + per-row mirror slices)
var private g_b_rte : array<uint>
var private g_b_koffs : array<uint64>
var private g_b_voffs : array<uint64>
var private g_b_caps : array<int64>
var private g_b_ulays : array<MetalBuffer?>   // per-layer uniforms, pooled per step

// the model half is decode_shape_decline; per batch: uniform f16/f32 KV, every row within the
// attention depth cap with a real uid, per-row rope rows built, and dim within the add+rms row
// slabs (GEMM path caps at 3072, fixed-B GEMV at ADD_RMS_MAX_DIM — B >= 5 on a wider model declines)
def private batch_decode_decline(t : Model; ws : BatchWorkspace; sessions : array<Session?>; nrows : int64) : MetalDecodeDecline {
    // kquant batch: B <= 8 rides per-stream plane GEMVs; each fused site must be format-uniform per layer (a mixed qkv/w13 site has no batched form)
    if (t.kquant_native) {
        if (nrows > 8l) {
            // B >= 9 rides the kq mul_mm twins — needs %64 GEMM shapes on BOTH attention classes (+ vocab %64 for the batched kq classifier)
            let ck = t.config
            let hs2 = ck.head_size_swa > 0l ? ck.head_size_swa : ck.head_size
            let kvh2 = ck.n_kv_heads_swa > 0l ? ck.n_kv_heads_swa : ck.n_kv_heads
            if ((ck.dim % 64l) != 0l || ((ck.n_heads * ck.head_size) % 64l) != 0l ||
                    ((ck.n_kv_heads * ck.head_size) % 64l) != 0l ||
                    ((ck.n_heads * hs2) % 64l) != 0l || ((kvh2 * hs2) % 64l) != 0l ||
                    (layer_hidden(t, 0l) % 64l) != 0l ||
                    (ck.vocab_size % 64l) != 0l) {
                return MetalDecodeDecline.kquant_native
            }
        }
        for (l in range64(t.config.n_layers)) {
            let kq_qkv = kq_fmt_at(t.wq_fmt, l) != KqFmt.q8
            // no-wv layers (gemma4 global: V = the K projection) have no attn_v format to match
            if ((kq_fmt_at(t.wk_fmt, l) != KqFmt.q8) != kq_qkv ||
                    (t.wv_offs[l] >= 0l && (kq_fmt_at(t.wv_fmt, l) != KqFmt.q8) != kq_qkv)) {
                return MetalDecodeDecline.kquant_native
            }
            let kq_w13 = kq_fmt_at(t.w1_fmt, l) != KqFmt.q8
            if ((kq_fmt_at(t.w3_fmt, l) != KqFmt.q8) != kq_w13) {
                return MetalDecodeDecline.kquant_native
            }
        }
    }
    // shexp MoE has no batch arm — a std-attention shexp model (qwen2moe-class) must decline
    if (!t.blocks.ffn_is_dense && t.config.n_ff_shexp > 0l) {
        return MetalDecodeDecline.graph
    }
    let shape = decode_shape_decline(t, BATCH_NEEDS_OK, true)
    if (shape != MetalDecodeDecline.none) {
        return shape
    }
    // AFTER the capability gates — see decode_decline's planar clause
    if (!t.metal_blob) {
        return MetalDecodeDecline.planar
    }
    let c = t.config
    let kdt = ws.scr.kv_dtype_k
    if (ws.scr.kv_dtype_v != kdt ||
            (kdt != KVDtype.f16 && kdt != KVDtype.f32 &&
             kdt != KVDtype.q8_0 && kdt != KVDtype.tq4)) {
        return MetalDecodeDecline.kv_dtype
    }
    for (i in range64(nrows)) {
        if (sessions[i].uid == 0ul) {
            return MetalDecodeDecline.no_uid
        }
    }
    // dual-rope rows are built per class, each at ITS class's width (gemma4 swa rows = hs_swa/2)
    let swa_hs = c.head_size_swa > 0l ? c.head_size_swa : c.head_size
    if (int64(length(ws.scr.rope_cos)) < nrows * (c.head_size / 2l) ||
            (has_dual_rope(c) && int64(length(ws.scr.rope_cos_swa)) < nrows * (swa_hs / 2l))) {
        return MetalDecodeDecline.rope_rows
    }
    return MetalDecodeDecline.none
}

//! The BatchDecodeOverrideFn (P4): one eval_batch step's whole layer stack GPU-resident — B rows
//! through ONE weight pass. STACK-ONLY: on true every row's KV is stored (mirror + CPU cache) and
//! ws.scr.x_b holds the final residuals; false = declined, CPU loop recomputes from clean state.
[no_env, no_io]
def metal_batch_decode_forward(t : Model; var ws : BatchWorkspace; var sessions : array<Session?>; nrows : int64) : bool {
    if (weight_caches_stale()) {   // a model (re)load may have recycled cached weight addresses
        discard_pre()
        weight_caches_flush_on_reload()
    }
    var why = batch_decode_decline(t, ws, sessions, nrows)
    if (why == MetalDecodeDecline.none && !metal_decode_init()) {
        why = MetalDecodeDecline.device
    }
    if (why != MetalDecodeDecline.none) {
        finish_pending_step()   // the CPU fallback needs the in-flight step's KV/logits landed first
        g_lp_err = false
        if (why == MetalDecodeDecline.feature) {
            batch_decline(why, metal_missing_needs(t, BATCH_NEEDS_OK))
        } else {
            batch_decline(why)
        }
        return false
    }
    // a parked single-stream pre-step may reference a batch row's mirror slice — retire it first
    discard_pre()
    // encode-ahead readiness: with a step in flight, mirror_prepare must stay on its fast path
    // (no upload, no eviction, no rebuild — those touch arena slices the GPU is still writing).
    // Any miss → complete the pending step first and take this one synchronously.
    if (g_lp_active) {
        var ready = true
        for (i in range64(nrows)) {
            var sp = sessions[i]
            if (!key_exists(g_mirrors, sp.uid)) {
                ready = false
                break
            }
            let mr = g_mirrors[sp.uid]   // cheap copy — fields tested off it
            if (mr.cap_rows < ws.positions[i] + 1l || mr.kdt != ws.scr.kv_dtype_k ||
                    mr.vdt != ws.scr.kv_dtype_v || mr.watermark < ws.positions[i]) {
                ready = false
                break
            }
        }
        if (!ready) {
            finish_pending_step()
        }
    }
    let ts_all = ref_time_ticks()
    let c = t.config
    let dim = c.dim
    let head_size = c.head_size
    let n_heads = c.n_heads
    let qd = n_heads * head_size
    let kv_dim = c.n_kv_heads * head_size
    let hidden = layer_hidden(t, 0l)
    let qkvd = qd + 2l * kv_dim
    let half = head_size / 2l
    // the two-class attention geometry (gemma4): config fields = the GLOBAL class, _sw twins =
    // the SLIDING class; buffers size to class maxima, each layer binds its class's uniform set
    let hs_sw = c.head_size_swa > 0l ? c.head_size_swa : head_size
    let kvh_sw = c.n_kv_heads_swa > 0l ? c.n_kv_heads_swa : c.n_kv_heads
    let qd_sw = n_heads * hs_sw
    let kvd_sw = kvh_sw * hs_sw
    let qkvd_sw = qd_sw + 2l * kvd_sw
    let hetero = hs_sw != head_size || kvh_sw != c.n_kv_heads
    let qd_mx = max(qd, qd_sw)
    let qkvd_mx = max(qkvd, qkvd_sw)
    let hs_mx = max(head_size, hs_sw)
    let half_sw = hs_sw / 2l
    let s16 = t.wscale_f16
    let kdt = ws.scr.kv_dtype_k
    let f16 = kdt == KVDtype.f16
    let tq4 = kdt == KVDtype.tq4
    let blockc = kdt == KVDtype.q8_0 || tq4  // block codec: byte-offset row table + codec kernel twins
    let esz = f16 ? 2l : 4l
    // reserve the whole step's arena demand BEFORE pinning — growth is a release-all rebuild
    // and cannot run once the batch's rows are pinned (the readiness check above already
    // finished any in-flight step when a session needs a new slice)
    var demand = 0ul
    for (i in range64(nrows)) {
        demand += mirror_arena_demand(t, *sessions[i], ws.positions[i])
    }
    if (!mirror_reserve(demand)) {
        finish_pending_step()
        g_lp_err = false
        batch_decline(MetalDecodeDecline.mirror)
        return false
    }
    // pin the batch's uids: a later row's mirror alloc must never evict an earlier row's slice
    for (i in range64(nrows)) {
        g_pin |> insert(sessions[i].uid)
    }
    g_b_rte |> resize(int(nrows * 4l))
    g_b_koffs |> resize(int(nrows))
    g_b_voffs |> resize(int(nrows))
    g_b_caps |> resize(int(nrows))
    var maxcnt = 0l
    var rows_ok = false
    for (attempt in range(2)) {
        rows_ok = true
        maxcnt = 0l
        for (i in range64(nrows)) {
            var sp = sessions[i]
            let mir = mirror_prepare(t, *sp, ws.positions[i])
            if (!mir.ok) {
                rows_ok = false
                break
            }
            g_b_koffs[i] = mir.koff
            g_b_voffs[i] = mir.voff
            g_b_caps[i] = mir.cap
            let cnt = ws.positions[i] + 1l
            maxcnt = max(maxcnt, cnt)
            // block codecs address in BYTES (there is no scalar element size); scalar mirrors keep
            // their element-offset entries
            // uint32 holds any arena offset: the arena clamps at 4096MB (= 2^32), so offsets < 2^32
            g_b_rte[i * 4l] = uint(blockc ? mir.koff : mir.koff / uint64(esz))
            g_b_rte[i * 4l + 1l] = uint(blockc ? mir.voff : mir.voff / uint64(esz))
            g_b_rte[i * 4l + 2l] = uint(mir.cap)
            g_b_rte[i * 4l + 3l] = uint(cnt)
        }
        if (rows_ok) {
            break
        }
        // an exact-fit step can strand contiguity around older residents — retry ONCE on an
        // empty arena (re-uploads ride the watermark contract; sequential packing then holds
        // whenever the demand fits capacity-wise)
        g_pin |> clear()
        if (attempt != 0 || !mirror_defrag(demand)) {
            break
        }
        for (i in range64(nrows)) {
            g_pin |> insert(sessions[i].uid)
        }
    }
    g_pin |> clear()
    if (!rows_ok) {
        finish_pending_step()   // fall back synchronously — the CPU loop needs completed KV state
        g_lp_err = false
        batch_decline(MetalDecodeDecline.mirror)
        return false
    }
    if (g_bgemm_min < 0l) {
        load_batch_knobs()
    }
    // the fixed-B GEMV forms cover 4 rows — past that the GEMM is mandatory (the knob can only
    // pull the crossover DOWN) unless the MV8 probe column-tiles (its tiling is probe-tuned to
    // dim <= 3072; wider models ride the GEMM's fixed K-tiles at any dim)
    let mv8 = g_bmv8 && g_bmv && nrows > 4l && nrows <= 8l && dim <= 3072l
    let use_gemm = (nrows >= g_bgemm_min || nrows > 4l) && !mv8
    // the mul_mm rail (opt-in, DASLLAMA_METAL_BATCH_MM=1): MEASURED NEGATIVE at M-pad-32 (3B d512
    // -n 32: B=8 145.2 vs GemmB 153.3) — mm's 32x64 tile grids only d/64 threadgroups vs GemmB's
    // 32x32 = 2x occupancy. Kept as the A/B rail for bigger-M futures (B > 16 pads to 64).
    let use_mm = (use_gemm && g_env_batch_mm &&
        (dim % 64l) == 0l && (qd % 64l) == 0l && (kv_dim % 64l) == 0l && (hidden % 64l) == 0l &&
        (qd_sw % 64l) == 0l && (kvd_sw % 64l) == 0l)
    let mp = use_gemm ? ((nrows + 31l) / 32l) * 32l : (nrows <= 2l ? 2l : nrows <= 4l ? 4l : 8l)
    // per-site GEMM policy (see g_bsk): each split-K site gates on its OWN K's nkb % S == 0 (the
    // partial planes assume mp == 32); ineligible sites stay on the planar v2. Hetero serves via
    // per-class twins; the wo gate must hold for BOTH classes' qd.
    let sk_ok = use_gemm && !use_mm && g_bsk && mp == 32l
    let qkv_sk = (sk_ok && (dim / 32l) % 2l == 0l) ? 2l : 0l
    let wo_sk = (sk_ok && (qd / 32l) % 4l == 0l && (!hetero || (qd_sw / 32l) % 4l == 0l)) ? 4l : 0l
    let w2_sk = (sk_ok && (hidden / 32l) % 8l == 0l) ? 8l : 0l
    let cls64 = use_gemm && !use_mm && g_bsk && (c.vocab_size % 64l) == 0l
    // the mul_mv rail owns the whole fixed-B path (w13 = two mv dispatches + the ew2 swiglu, so
    // the x-staged w13sw fusion stands down with it). Hetero rides it too: the QKV arm carries
    // the no-wv cat3 stream, and the wo site's x stride (the one per-class xs4) picks its twin
    let mv = g_bmv && !use_gemm && !use_mm
    let geglu = c.ffn_act != FfnAct.silu    // gemma: gelu gate — the swiglu-fused w13 form stands down
    // fold w1+w3+swiglu into one fused GEMV on the fixed-B path; a knockout run (g_skip) keeps the
    // stages separable so its per-stage attribution stays valid
    let fuse13 = g_bfuse && !mv && !use_gemm && !use_mm && empty(g_skip) && !t.kquant_native && !geglu
    let chunked = maxcnt > g_attn_single_max
    let nchmax = (maxcnt + ATTN_CHUNK_ROWS - 1l) / ATTN_CHUNK_ROWS
    // partD (walkthrough kernel-2 winner): fused single-pass slice kernel + parallel comb — lane
    // ownership assumes hs == 128 (32 lanes x half4 = one head); no softcap/window uniforms, so those
    // models stand down to the chunked pair. DASLLAMA_METAL_ATTN_D=0 = the A/B rail to the old path.
    let use_attn_d = (head_size == 128l && !hetero && c.attn_logit_softcap <= 0.0 && c.sliding_window == 0l &&
        !c.attn_sinks &&   // partD's online softmax has no sink seed — sink models take the chunked pair
        g_env_attn_d)
    let d_nsgs = min((maxcnt + 31l) / 32l, 16l)
    // logits-on-GPU (default ON): the batched final rms + classifier ride the same cb and the
    // CPU tail's classifier (one full weight-plane stream per step — ~6ms of the small-B wall)
    // is skipped via batch_decode_override_logits_done. Gates mirror the single-stream driver's.
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    let tied_f32 = c.shared_weights && !t.cls_q8 && cls_fmt == KqFmt.q8
    let gpu_logits = ((t.metal_blob || (!g_logits_cpu && g_env_logits && empty(g_skip))) &&
        !tied_f32 && c.n_layer_nextn == 0l &&
        t.wcls_off >= 0l &&
        (cls_fmt != KqFmt.q8 || !use_gemm || (c.vocab_size % (use_mm ? 64l : 32l)) == 0l))
    // MoE (Wave C): plain moe rides bhb/bhb2 for the (row, slot) gate/up rows; g4moe keeps
    // the dense branch there and gives the experts their OWN panels (the concurrent encoder
    // has no write-after-read tracking, so reuse would race)
    let moe = !t.blocks.ffn_is_dense
    let g4moe = moe && c.moe_dense_shexp
    let knfe = moe ? c.n_expert_used * c.n_ff_exp : 0l
    // pooled per-step buffers (mp rows: the fixed-B forms read their full B, the GEMM its M pad —
    // stale pad rows only ever land in pad outputs, which nothing reads back)
    let bytes_x = uint64(mp * dim * 4l)
    let bytes_xb = uint64(mp * max(dim, qd_mx) * 4l)   // dim-wide norm outs AND the qd-wide attention out
    let bytes_qkv = uint64(mp * qkvd_mx * 4l)
    let bytes_h = uint64(max(mp * hidden, nrows * knfe) * 4l)
    let bytes_tab = uint64(nrows * half * 4l)
    let bytes_tab_sw = uint64(nrows * half_sw * 4l)   // the swa pair's rows are ITS class's hs/2
    let bytes_rt = uint64(nrows * 16l)
    let bytes_part = uint64(use_attn_d ? nrows * n_heads * d_nsgs * (head_size + 4l) * 4l :
        nrows * n_heads * nchmax * (hs_mx + 2l) * 4l)
    var bx = pool_acquire(g_pool, g_dev, bytes_x)
    var bxb = pool_acquire(g_pool, g_dev, bytes_xb)
    var bxb2 = pool_acquire(g_pool, g_dev, bytes_x)
    var bqkv = pool_acquire(g_pool, g_dev, bytes_qkv)
    var bhb = pool_acquire(g_pool, g_dev, bytes_h)
    var bhb2 = pool_acquire(g_pool, g_dev, bytes_h)
    var bpart = (use_attn_d || chunked) ? pool_acquire(g_pool, g_dev, bytes_part) : null
    // one pooled partial-plane buffer serves every split-K site (sized to the largest enabled —
    // w2's S8 x 32 x dim is 3MB at 3B); sites run serially, so reuse is free
    var bytes_sk = 0ul
    if (qkv_sk > 0l) {
        bytes_sk = uint64(qkv_sk * 32l * qkvd_mx * 4l)   // hetero: sized for the wider class
    }
    if (wo_sk > 0l) {
        bytes_sk = max(bytes_sk, uint64(wo_sk * 32l * dim * 4l))
    }
    if (w2_sk > 0l) {
        bytes_sk = max(bytes_sk, uint64(w2_sk * 32l * dim * 4l))
    }
    var bpsk = bytes_sk > 0ul ? pool_acquire(g_pool, g_dev, bytes_sk) : null
    let bytes_mlg = uint64(nrows * c.n_expert * 4l)
    let bytes_msel = uint64(nrows * 2l * c.n_expert_used * 4l)
    let bytes_mdn = uint64(nrows * c.n_expert_used * dim * 4l)
    var bmlg = moe ? pool_acquire(g_pool, g_dev, bytes_mlg) : null
    var bmsel = moe ? pool_acquire(g_pool, g_dev, bytes_msel) : null
    var bmdn = moe ? pool_acquire(g_pool, g_dev, bytes_mdn) : null
    let bytes_mg4x = uint64(nrows * dim * 4l)
    let bytes_mgh = uint64(nrows * knfe * 4l)
    var bmg4x = g4moe ? pool_acquire(g_pool, g_dev, bytes_mg4x) : null
    var bmg4rt = g4moe ? pool_acquire(g_pool, g_dev, bytes_mg4x) : null
    var bmgh = g4moe ? pool_acquire(g_pool, g_dev, bytes_mgh) : null
    var bmgh2 = g4moe ? pool_acquire(g_pool, g_dev, bytes_mgh) : null
    let bytes_log = uint64(mp * c.vocab_size * 4l)
    var bxf = gpu_logits ? pool_acquire(g_pool, g_dev, bytes_x) : null
    var blog = gpu_logits ? pool_acquire(g_pool, g_dev, bytes_log) : null
    var bcos = pool_acquire_untracked(g_upool, g_dev, bytes_tab)
    var bsin = pool_acquire_untracked(g_upool, g_dev, bytes_tab)
    let dual_rope = has_dual_rope(c)
    var bcos_swa = dual_rope ? pool_acquire_untracked(g_upool, g_dev, bytes_tab_sw) : null
    var bsin_swa = dual_rope ? pool_acquire_untracked(g_upool, g_dev, bytes_tab_sw) : null
    var brt = pool_acquire_untracked(g_upool, g_dev, bytes_rt)
    var u_dim = uniform_u32(uint(dim))
    var u_qd = uniform_u32(uint(qd))
    var u_kvd = uniform_u32(uint(kv_dim))
    var u_qkvd = uniform_u32(uint(qkvd))
    var u_hidden = uniform_u32(uint(hidden))
    var u_hs = uniform_u32(uint(head_size))
    var u_kvm = uniform_u32(uint(n_heads / c.n_kv_heads))
    var u_nh = uniform_u32(uint(n_heads))
    var u_scale = uniform_f32(c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size)))
    var u_eps = uniform_f32(c.norm_eps)
    var u_npq = uniform_u32(uint(qd / 2l))
    var u_npall = uniform_u32(uint((qd + kv_dim) / 2l))
    var u_neox = uniform_u32(c.rope_neox ? 1u : 0u)
    var u_hasb = uniform_u32(c.attn_qkv_bias ? 1u : 0u)
    var u_softcap = uniform_f32(c.attn_logit_softcap)
    var u_zero32 = uniform_u32(0u)
    // the SLIDING-class uniform twins (hetero models — uniform models leave these null and
    // every layer binds the base set)
    var u_qd_sw = hetero ? uniform_u32(uint(qd_sw)) : null
    var u_kvd_sw = hetero ? uniform_u32(uint(kvd_sw)) : null
    var u_qkvd_sw = hetero ? uniform_u32(uint(qkvd_sw)) : null
    var u_hs_sw = hetero ? uniform_u32(uint(hs_sw)) : null
    var u_kvm_sw = hetero ? uniform_u32(uint(n_heads / kvh_sw)) : null
    var u_scale_sw = hetero ? uniform_f32(c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(hs_sw))) : null
    var u_npq_sw = hetero ? uniform_u32(uint(qd_sw / 2l)) : null
    var u_npall_sw = hetero ? uniform_u32(uint((qd_sw + kvd_sw) / 2l)) : null
    var u_nblk_sw = (hetero && blockc) ? uniform_u32(uint(kvd_sw / 32l)) : null
    // the V-from-K / v_norm sites: v segment element offset within a fused-QKV row, per class
    let vfromk = c.v_from_k_mask != 0ul
    var u_voffe = (c.v_norm || vfromk) ? uniform_u32(uint(qd + kv_dim)) : null
    var u_voffe_sw = ((c.v_norm || vfromk) && hetero) ? uniform_u32(uint(qd_sw + kvd_sw)) : null
    // per-row depths differ, so the batch kernels derive wstart from `window` + their row's cnt
    var u_winsl = c.sliding_window > 0l ? uniform_u32(uint(c.sliding_window)) : null
    var u_flcap = (gpu_logits && c.final_logit_softcap > 0.0) ? uniform_f32(c.final_logit_softcap) : null
    var u_fltot = u_flcap != null ? uniform_u32(uint(nrows * c.vocab_size)) : null
    // suppressed token ids pin to -1e30 after the cap (gemma4) — per live row, the CPU order
    let nsupp = int64(length(t.suppress))
    var bsupp : MetalBuffer?
    var u_nsupp : MetalBuffer?
    if (gpu_logits && nsupp > 0l) {
        bsupp = pool_acquire_untracked(g_upool, g_dev, uint64(nsupp * 4l))
        unsafe {
            var sp = reinterpret<uint?>(metal_buffer_contents(bsupp))
            for (i in range64(nsupp)) {
                sp[i] = uint(t.suppress[i])
            }
        }
        u_nsupp = uniform_u32(uint(nsupp))
    }
    var u_s16 = uniform_u32(s16 ? 1u : 0u)
    var u_total_dim = uniform_u32(uint(nrows * dim))
    var u_total_h = uniform_u32(uint(nrows * hidden))
    // MoE per-step uniforms (the expert twins' stride set; see enc_moe_gemv)
    var u_moe_ne = moe ? uniform_u32(uint(c.n_expert)) : null
    var u_moe_k = moe ? uniform_u32(uint(c.n_expert_used)) : null
    var u_moe_nfe = moe ? uniform_u32(uint(c.n_ff_exp)) : null
    var u_moe_eblk = moe ? uniform_u32(uint(dim * c.n_ff_exp / 32l)) : null
    var u_moe_esb = moe ? uniform_u32(uint(dim * c.n_ff_exp / 256l)) : null
    var u_moe_ss = moe ? uniform_u32(uint(2l * c.n_expert_used)) : null
    var u_moe_xs_dn = moe ? uniform_u32(uint(c.n_ff_exp)) : null
    var u_moe_knfe = moe ? uniform_u32(uint(knfe)) : null
    var u_moe_kdim = moe ? uniform_u32(uint(c.n_expert_used * dim)) : null
    var u_moe_tot = moe ? uniform_u32(uint(nrows * knfe)) : null
    var u_moe_invd = g4moe ? uniform_f32(1.0 / sqrt(float(dim))) : null
    var u_moe_gmode = moe ? uniform_u32(c.moe_gate == MoeGate.softmax_weight ? 1u : (c.norm_topk_prob ? 0u : 2u)) : null
    var u_hass = c.attn_sinks ? uniform_u32(1u) : null
    var u_xs4_dim = mv ? uniform_u32(uint(dim / 4l)) : null
    var u_xs4_qd = mv ? uniform_u32(uint(qd / 4l)) : null
    var u_xs4_qd_sw = (mv && hetero) ? uniform_u32(uint(qd_sw / 4l)) : null   // the wo x stride's sliding-class twin
    var u_xs4_hid = mv ? uniform_u32(uint(hidden / 4l)) : null
    var u_nrows_mv = (mv && nrows > 2l) ? uniform_u32(uint(min(nrows, 4l))) : null
    var u_nrows_mv2 = (mv && nrows > 4l) ? uniform_u32(uint(nrows - 4l)) : null
    // the kq ext twins' live-column uniform (stores gate on it; pad columns compute and discard)
    var u_nrows_kq = (t.kquant_native && nrows >= 2l && nrows <= 8l) ? uniform_u32(uint(nrows)) : null
    var u_nch = (!use_attn_d && chunked) ? uniform_u32(uint(nchmax)) : null
    var u_nsgs = use_attn_d ? uniform_u32(uint(d_nsgs)) : null
    var u_nblk = blockc ? uniform_u32(uint(kv_dim / 32l)) : null
    var u_vocab = gpu_logits ? uniform_u32(uint(c.vocab_size)) : null
    var u_sk_qkv = qkv_sk > 0l ? uniform_u32(uint(qkv_sk)) : null
    var u_sk_wo = wo_sk > 0l ? uniform_u32(uint(wo_sk)) : null
    var u_sk_w2 = w2_sk > 0l ? uniform_u32(uint(w2_sk)) : null
    var u_skt_qkv = qkv_sk > 0l ? uniform_u32(uint(32l * qkvd)) : null
    var u_skt_qkv_sw = (qkv_sk > 0l && hetero) ? uniform_u32(uint(32l * qkvd_sw)) : null   // the sliding class's reduce stride
    var u_skt_dim = (wo_sk > 0l || w2_sk > 0l) ? uniform_u32(uint(32l * dim)) : null
    // the batch kernels' `lay` = the layer's KV-row PREFIX (elements; row BYTES for block
    // codecs) — slab base = rrow.base + lay * cap. Uniform models compute the old
    // l * cap * rowb addresses exactly (prefix[l] = l * kv_dim; codecs are linear bytes/elem)
    g_b_ulays |> resize(int(c.n_layers))
    for (l in range64(c.n_layers)) {
        g_b_ulays[l] = uniform_u32(uint(blockc ? kv_row_bytes(kdt, t.kv_row_prefix[l]) : t.kv_row_prefix[l]))
    }
    unsafe {
        memcpy(metal_buffer_contents(bx), addr < void? >(ws.scr.x_b[0]), int(nrows * dim * 4l))
        memcpy(metal_buffer_contents(bcos), addr < void? >(ws.scr.rope_cos[0]), int(bytes_tab))
        memcpy(metal_buffer_contents(bsin), addr < void? >(ws.scr.rope_sin[0]), int(bytes_tab))
        if (dual_rope) {
            memcpy(metal_buffer_contents(bcos_swa), addr < void? >(ws.scr.rope_cos_swa[0]), int(bytes_tab_sw))
            memcpy(metal_buffer_contents(bsin_swa), addr < void? >(ws.scr.rope_sin_swa[0]), int(bytes_tab_sw))
        }
        memcpy(metal_buffer_contents(brt), addr < void? >(g_b_rte[0]), int(bytes_rt))
    }
    g_us_bsetup += int64(get_time_usec(ts_all))
    let ts_enc = ref_time_ticks()
    let ncb = min(g_bncb, c.n_layers)
    let lay_per_cb = (c.n_layers + ncb - 1l) / ncb
    var cbs : array<MetalCommandBuffer?>
    cbs |> reserve(int(ncb))
    // concurrent-encode rail: a concurrent encoder + explicit barriers ONLY at real buffer hazards
    // (the serial encoder implicit-barriers every dispatch). Gated to a full pipeline — a knockout
    // (g_skip) run stays on the serial encoder so a skipped stage can't leave a dependent unbarriered.
    let conc = g_bconcurrent && empty(g_skip)
    hz_arm(conc)   // the range oracle: the *_b encoders self-declare, hz_gate barriers at real hazards
    var cb = metal_new_command_buffer_unretained(g_queue)
    var enc = conc ? metal_new_compute_encoder_concurrent(cb) : metal_new_compute_encoder(cb)
    // layer-0 attention norm (the per-layer trailing add+rms feeds the NEXT layer's)
    if (g_skip != "ew") {
        unsafe {
            var bw_att0 = upload_region(addr < void? >(t.fblob[t.rms_att_off]), uint64(dim * 4l))
            enc_rms_b(enc, bx, bw_att0, bxb, u_dim, u_eps, nrows)
        }
    }
    for (l in range64(c.n_layers)) {
        unsafe {
            var ulay = g_b_ulays[l]
            // sliding layers on hetero models (gemma4) swap the whole class geometry — sizes,
            // grids, uniform binds; uniform models bind the base set everywhere
            let lsl = layer_is_sliding(c, l)
            let het = hetero && lsl
            let hs_l = het ? hs_sw : head_size
            let kvd_l = het ? kvd_sw : kv_dim
            let qd_l = het ? qd_sw : qd
            let qkvd_l = het ? qkvd_sw : qkvd
            var u_qd_l = het ? u_qd_sw : u_qd
            var u_kvd_l = het ? u_kvd_sw : u_kvd
            var u_qkvd_l = het ? u_qkvd_sw : u_qkvd
            var u_hs_l = het ? u_hs_sw : u_hs
            var u_kvm_l = het ? u_kvm_sw : u_kvm
            var u_scale_l = het ? u_scale_sw : u_scale
            var u_npq_l = het ? u_npq_sw : u_npq
            var u_npall_l = het ? u_npall_sw : u_npall
            var u_nblk_l = het ? u_nblk_sw : u_nblk
            let has_wv = t.wv_offs[l] >= 0l
            // attention: QKV (one concat stream) -> per-row rope + KV store -> per-row attention
            // over [0, cnt_b) -> out-proj -> fused residual add + ffn norm. K-quant sites take
            // nrows per-stream plane GEMVs (class-uniformity per site gated at the decline).
            let fq = kq_fmt_at(t.wq_fmt, l)
            let fo = kq_fmt_at(t.wo_fmt, l)
            let f1 = kq_fmt_at(t.w1_fmt, l)
            let f2 = kq_fmt_at(t.w2_fmt, l)
            if (g_skip != "gemv") {
                if (fq != KqFmt.q8) {
                    enc_kq_site_b(enc, t, fq, t.wq_offs[l], qd_l, dim, bxb, dim, bqkv, qkvd_l, 0l, u_dim, u_qd_l, u_qkvd_l, u_nrows_kq, nrows, mp)
                    enc_kq_site_b(enc, t, kq_fmt_at(t.wk_fmt, l), t.wk_offs[l], kvd_l, dim, bxb, dim, bqkv, qkvd_l, qd_l, u_dim, u_kvd_l, u_qkvd_l, u_nrows_kq, nrows, mp)
                    if (has_wv) {
                        enc_kq_site_b(enc, t, kq_fmt_at(t.wv_fmt, l), t.wv_offs[l], kvd_l, dim, bxb, dim, bqkv, qkvd_l, qd_l + kvd_l, u_dim, u_kvd_l, u_qkvd_l, u_nrows_kq, nrows, mp)
                    }
                } elif (use_mm) {
                    // three per-tensor blob dispatches into the ONE fused QKV buffer — y binds at
                    // the tensor's column byte offset, the qkvd row stride rides the ndim uniform;
                    // disjoint writes, so one barrier (bxb RAW) covers all three — they overlap
                    let bwq_ = blob_of(g_dev, t, t.wq_offs[l])
                    let bwk_ = blob_of(g_dev, t, t.wk_offs[l])
                    enc_gemm_mm_b(enc, bwq_.buf, bwq_.boff, bxb, bqkv, 0ul, u_dim, u_qkvd_l, mp, qd_l)
                    enc_gemm_mm_b(enc, bwk_.buf, bwk_.boff, bxb, bqkv, uint64(qd_l * 4l), u_dim, u_qkvd_l, mp, kvd_l)
                    if (has_wv) {
                        let bwv_ = blob_of(g_dev, t, t.wv_offs[l])
                        enc_gemm_mm_b(enc, bwv_.buf, bwv_.boff, bxb, bqkv, uint64((qd_l + kvd_l) * 4l), u_dim, u_qkvd_l, mp, kvd_l)
                    }
                } elif (mv) {
                    // per-tensor mv dispatches into the fused bqkv at segment y offsets (the
                    // ys uniform carries the fused row stride; the cat streams died with the
                    // blob image — kind-major layout keeps q/k/v apart)
                    let bwq_ = blob_of(g_dev, t, t.wq_offs[l])
                    let bwk_ = blob_of(g_dev, t, t.wk_offs[l])
                    enc_mv_site(enc, bwq_.buf, bwq_.boff, bxb, bqkv, u_dim, u_qd_l, u_xs4_dim, u_qkvd_l,
                        qd_l, nrows, qkvd_l, uint64(dim * 4l), u_nrows_mv, u_nrows_mv2)
                    enc_mv_site(enc, bwk_.buf, bwk_.boff, bxb, bqkv, u_dim, u_kvd_l, u_xs4_dim, u_qkvd_l,
                        kvd_l, nrows, qkvd_l, uint64(dim * 4l), u_nrows_mv, u_nrows_mv2, uint64(qd_l * 4l))
                    if (has_wv) {
                        let bwv_ = blob_of(g_dev, t, t.wv_offs[l])
                        enc_mv_site(enc, bwv_.buf, bwv_.boff, bxb, bqkv, u_dim, u_kvd_l, u_xs4_dim, u_qkvd_l,
                            kvd_l, nrows, qkvd_l, uint64(dim * 4l), u_nrows_mv, u_nrows_mv2, uint64((qd_l + kvd_l) * 4l))
                    }
                } else {
                    // gemm/gemv tiers, per tensor into the fused buffer (split-K stands down —
                    // its reduce writes contiguous y; the qkv site takes plain GemmB instead)
                    let bwq_ = blob_of(g_dev, t, t.wq_offs[l])
                    let bwk_ = blob_of(g_dev, t, t.wk_offs[l])
                    if (use_gemm) {
                        enc_gemm_b(enc, bwq_.buf, bwq_.boff, bxb, bqkv, u_dim, u_qd_l, u_qkvd_l, mp, qd_l)
                        enc_gemm_b(enc, bwk_.buf, bwk_.boff, bxb, bqkv, u_dim, u_kvd_l, u_qkvd_l, mp, kvd_l, uint64(qd_l * 4l))
                        if (has_wv) {
                            let bwv_ = blob_of(g_dev, t, t.wv_offs[l])
                            enc_gemm_b(enc, bwv_.buf, bwv_.boff, bxb, bqkv, u_dim, u_kvd_l, u_qkvd_l, mp, kvd_l, uint64((qd_l + kvd_l) * 4l))
                        }
                    } else {
                        enc_gemv_b(enc, nrows > 2l, bwq_.buf, bwq_.boff, bxb, bqkv, u_dim, u_qd_l, u_qkvd_l, qd_l)
                        enc_gemv_b(enc, nrows > 2l, bwk_.buf, bwk_.boff, bxb, bqkv, u_dim, u_kvd_l, u_qkvd_l, kvd_l, uint64(qd_l * 4l))
                        if (has_wv) {
                            let bwv_ = blob_of(g_dev, t, t.wv_offs[l])
                            enc_gemv_b(enc, nrows > 2l, bwv_.buf, bwv_.boff, bxb, bqkv, u_dim, u_kvd_l, u_qkvd_l, kvd_l, uint64((qd_l + kvd_l) * 4l))
                        }
                    }
                }
            }
            if ((!has_wv || c.v_norm) && g_skip != "ew") {
                // gemma4 V: no-wv layers fill the v segment from the PRE-norm/PRE-rope K slice (the
                // CPU mm_qkv order); with v_norm the copies FUSE into the ones-RMS. The k-slice READ
                // orders against qk_norm's k WRITE via the encoders' own hz ranges (the V-race class).
                if (!has_wv && !c.v_norm) {
                    for (i in range64(nrows)) {
                        enc_copy_row(enc, bqkv, uint64((i * qkvd_l + qd_l) * 4l), bqkv,
                            uint64((i * qkvd_l + qd_l + kvd_l) * 4l), u_kvd_l, kvd_l)
                    }
                }
                if (c.v_norm) {
                    // MAX-width ones upload: the region cache is address-keyed and both classes
                    // read this one plane
                    var bones = upload_region(addr < void? >(t.fblob[t.rms_ones_off]), uint64(max_head_size(c) * 4l))
                    enc_qk_norm(enc, bqkv, bones, u_zero32, het ? u_voffe_sw : u_voffe,
                        u_qkvd_l, u_hs_l, u_zero32, u_eps,
                        !has_wv ? u_qd_l : (het ? u_voffe_sw : u_voffe),
                        kvd_l / hs_l, nrows, hs_l)
                }
            }
            if (c.qk_norm && g_skip != "ew") {
                // per-head QK RMSNorm on every row's fused QKV slice, before rope (qwen3)
                var bqkw = upload_region_cat3(
                    addr < void? >(t.fblob[t.rms_q_offs[l]]), uint64(hs_l * 4l),
                    addr < void? >(t.fblob[t.rms_k_offs[l]]), uint64(hs_l * 4l), null, 0ul)
                enc_qk_norm(enc, bqkv, bqkw, u_nh, u_qd_l, u_qkvd_l, u_hs_l, u_hs_l, u_eps,
                    u_qd_l, n_heads + kvd_l / hs_l, nrows, hs_l)
            }
            if (g_skip != "ew") {
                // rope reads+writes bqkv in place and writes the current KV row into the arena;
                // the [bq | bk | bv] bias plane adds pre-rope (dummy-bound when the model has none)
                var bbias = bqkv
                if (c.attn_qkv_bias) {
                    bbias = upload_region_cat3(
                        addr < void? >(t.bq[l * qd_l]), uint64(qd_l * 4l),
                        addr < void? >(t.bk[l * kvd_l]), uint64(kvd_l * 4l),
                        addr < void? >(t.bv[l * kvd_l]), uint64(kvd_l * 4l))
                }
                // sliding layers rope with the swa row pair (dual-rope configs — gemma3/4;
                // hetero: the swa pair's rows are hs_sw/2 wide — the class's own width)
                var bcos_l = lsl && dual_rope ? bcos_swa : bcos
                var bsin_l = lsl && dual_rope ? bsin_swa : bsin
                if (tq4) {
                    enc_rope_store_b_tq4(enc, bqkv, bcos_l, bsin_l, brt, ulay, u_qd_l, u_hs_l, u_nh, u_kvm_l, u_qkvd_l, u_neox,
                        bbias, u_hasb, n_heads + 2l * (kvd_l / hs_l), nrows, hs_l)
                } elif (blockc) {
                    enc_rope_store_b_q8(enc, bqkv, bcos_l, bsin_l, brt, ulay, u_nblk_l, u_qd_l, u_hs_l, u_npq_l, u_qkvd_l, u_neox,
                        bbias, u_hasb, qd_l / 2l + 2l * (kvd_l / 32l), nrows)
                } else {
                    enc_rope_store_b(enc, f16, bqkv, bcos_l, bsin_l, brt, ulay, u_kvd_l, u_qd_l, u_hs_l, u_npq_l, u_npall_l, u_qkvd_l, u_neox,
                        bbias, u_hasb, (qd_l + kvd_l) / 2l, nrows)
                }
            }
            if (g_skip != "attn") {
                var bsnk : MetalBuffer?   // null = the enc defaults bind the identity
                if (c.attn_sinks) {
                    bsnk = upload_region(addr < void? >(t.fblob[t.sinks_off + l * n_heads]), uint64(n_heads * 4l))
                }
                if (use_attn_d) {
                    if (tq4) {
                        enc_attn_d_tq4(enc, bqkv, bpart, brt, ulay, u_nblk, u_nh, u_kvm, u_hs, u_scale, u_qkvd, u_nsgs,
                            d_nsgs, nrows, n_heads)
                    } elif (blockc) {
                        enc_attn_d_q8(enc, bqkv, bpart, brt, ulay, u_nblk, u_nh, u_kvm, u_hs, u_scale, u_qkvd, u_nsgs,
                            d_nsgs, nrows, n_heads)
                    } else {
                        enc_attn_d(enc, f16, bqkv, bpart, brt, ulay, u_kvd, u_nh, u_kvm, u_hs, u_scale, u_qkvd, u_nsgs,
                            d_nsgs, nrows, n_heads)
                    }
                    enc_attn_comb_d(enc, bpart, bxb, brt, u_nsgs, u_nh, u_hs, nrows, n_heads, head_size)
                } elif (chunked) {
                    // sliding layers bind the span; each row's kernels derive wstart from its own
                    // cnt (per-row depths differ), and the comb skips that row's below-window chunks
                    var bwin = lsl && u_winsl != null ? u_winsl : u_zero32
                    if (tq4) {
                        enc_attn_part_b_tq4(enc, bqkv, bpart, brt, ulay, u_nblk_l, u_nh, u_kvm_l, u_hs_l, u_scale_l, u_qkvd_l, u_nch,
                            u_softcap, bwin, nrows, nchmax, n_heads, hs_l)
                    } elif (blockc) {
                        enc_attn_part_b_q8(enc, bqkv, bpart, brt, ulay, u_nblk_l, u_nh, u_kvm_l, u_hs_l, u_scale_l, u_qkvd_l, u_nch,
                            u_softcap, bwin, nrows, nchmax, n_heads, hs_l)
                    } else {
                        enc_attn_part_b(enc, f16, bqkv, bpart, brt, ulay, u_kvd_l, u_nh, u_kvm_l, u_hs_l, u_scale_l, u_qkvd_l, u_nch,
                            u_softcap, bwin, nrows, nchmax, n_heads, hs_l)
                    }
                    enc_attn_comb_b(enc, bpart, bxb, brt, u_nch, u_nh, u_hs_l, bwin, nrows, n_heads, hs_l, bsnk, u_hass)
                } else {
                    if (tq4) {
                        enc_attn_b_tq4(enc, bqkv, bxb, brt, ulay, u_nblk_l, u_nh, u_kvm_l, u_hs_l, u_scale_l, u_qkvd_l,
                            u_softcap, nrows, n_heads, hs_l, bsnk, u_hass)
                    } elif (blockc) {
                        enc_attn_b_q8(enc, bqkv, bxb, brt, ulay, u_nblk_l, u_nh, u_kvm_l, u_hs_l, u_scale_l, u_qkvd_l,
                            u_softcap, nrows, n_heads, hs_l, bsnk, u_hass)
                    } else {
                        enc_attn_b(enc, f16, bqkv, bxb, brt, ulay, u_kvd_l, u_nh, u_kvm_l, u_hs_l, u_scale_l, u_qkvd_l,
                            u_softcap, nrows, n_heads, hs_l, bsnk, u_hass)
                    }
                }
                if (tq4) {
                    // the attention out is rotated-basis per head — un-rotate before the wo projection
                    enc_fwht_unsign(enc, bxb, u_hs_l, u_qd_l, n_heads, nrows, hs_l)
                }
            }
            if (g_skip != "gemv") {
                if (fo != KqFmt.q8) {
                    enc_kq_site_b(enc, t, fo, t.wo_offs[l], dim, qd_l, bxb, qd_l, bxb2, dim, 0l, u_qd_l, u_dim, u_dim, u_nrows_kq, nrows, mp)
                } elif (use_mm) {
                    let bwo = blob_of(g_dev, t, t.wo_offs[l])
                    enc_gemm_mm_b(enc, bwo.buf, bwo.boff, bxb, bxb2, 0ul, u_qd_l, u_dim, mp, dim)
                } elif (mv) {
                    let bwo = blob_of(g_dev, t, t.wo_offs[l])
                    enc_mv_site(enc, bwo.buf, bwo.boff, bxb, bxb2,
                        u_qd_l, u_dim, het ? u_xs4_qd_sw : u_xs4_qd, u_dim, dim, nrows, dim, uint64(qd_l * 4l), u_nrows_mv, u_nrows_mv2)
                } else {
                    let bwo = blob_of(g_dev, t, t.wo_offs[l])
                    if (wo_sk > 0l) {
                        enc_gemm_sk_b(enc, bwo.buf, bwo.boff, bxb, bpsk, u_qd_l, u_dim, u_sk_wo, dim, wo_sk)
                        enc_sk_reduce_b(enc, bpsk, bxb2, u_skt_dim, u_sk_wo, 32l * dim)
                    } elif (use_gemm) {
                        enc_gemm_b(enc, bwo.buf, bwo.boff, bxb, bxb2, u_qd_l, u_dim, u_dim, mp, dim)
                    } else {
                        enc_gemv_b(enc, nrows > 2l, bwo.buf, bwo.boff, bxb, bxb2, u_qd_l, u_dim, u_dim, dim)
                    }
                }
            }
            if (c.attn_out_bias && g_skip != "gemv") {
                var bwo_b = upload_region(addr < void? >(t.bo[l * dim]), uint64(dim * 4l))
                enc_ew2(enc, g_pso_add, bxb2, 0ul, bwo_b, 0ul, u_total_dim, nrows * dim)
            }
            // ffn: [post-attn norm ->] fused add+norm -> W1 / W3 (the cat region at byte offsets)
            // -> swiglu/geglu -> W2 [-> post-ffn norm] -> fused add + the NEXT layer's attention
            // norm (plain add on the last). pre_post_norm norms the branch output in place first.
            if (g_skip != "ew") {
                if (c.pre_post_norm) {
                    var bw_pa = upload_region(addr < void? >(t.fblob[t.rms_post_att_off + l * dim]), uint64(dim * 4l))
                    enc_rms_b(enc, bxb2, bw_pa, bxb2, u_dim, u_eps, nrows)
                }
                var bw_ffn = upload_region(addr < void? >(t.fblob[t.rms_ffn_off + l * dim]), uint64(dim * 4l))
                // add_rms updates the residual bx in place AND writes the norm-out bxb; past the
                // fused kernel's row slab the unfused pair does the same in two dispatches
                if (dim <= ADD_RMS_MAX_DIM && !g_b_addrms_unfused) {
                    enc_add_rms_b(enc, bx, bxb2, bw_ffn, bxb, u_dim, u_eps, nrows)
                } else {
                    enc_ew2(enc, g_pso_add, bx, 0ul, bxb2, 0ul, u_total_dim, nrows * dim)
                    enc_rms_b(enc, bx, bw_ffn, bxb, u_dim, u_eps, nrows)
                }
            }
            // g4moe: experts ride their OWN panels (bmgh/bmgh2, input bmg4x, acc bmg4rt) so the
            // dense shared branch's bhb/bhb2 tiers run concurrently with the expert chain
            let bex = g4moe ? bmg4x : bxb
            let bgh = g4moe ? bmgh : bhb
            let bgh2 = g4moe ? bmgh2 : bhb2
            if (moe && g_skip != "gemv") {
                // MoE FFN (Wave C): per-row router -> select -> expert gate/up (grid (nfe, k,
                // rows)) -> act over rows*k*nfe -> expert down -> per-row weighted combine
                let fe1 = kq_fmt_at(t.we1_fmt, l)
                let fe3 = kq_fmt_at(t.we3_fmt, l)
                var bw_rt = upload_region(addr < void? >(t.fblob[t.router_off + l * c.n_expert * dim]), uint64(c.n_expert * dim * 4l))
                var brb : MetalBuffer? = g_one
                var bhrb = u_zero32
                if (c.moe_router_bias) {
                    brb = upload_region(addr < void? >(t.fblob[t.router_b_off + l * c.n_expert]), uint64(c.n_expert * 4l))
                    bhrb = u_moe_gmode
                }
                if (g4moe) {
                    var bw_p2 = upload_region(addr < void? >(t.fblob[t.rms_pre_ffn2_off + l * dim]), uint64(dim * 4l))
                    enc_rms_b(enc, bx, bw_p2, bmg4x, u_dim, u_eps, nrows)
                    var bw_rs = upload_region(addr < void? >(t.fblob[t.moe_router_scale_off + l * dim]), uint64(dim * 4l))
                    enc_g4_router_norm(enc, bx, bw_rs, bmg4rt, u_dim, u_eps, u_moe_invd, nrows)
                    enc_moe_router(enc, bw_rt, bmg4rt, bmlg, u_dim, u_moe_ne, u_dim, brb, bhrb, c.n_expert, nrows)
                } else {
                    enc_moe_router(enc, bw_rt, bxb, bmlg, u_dim, u_moe_ne, u_dim, brb, bhrb, c.n_expert, nrows)
                }
                enc_moe_select(enc, bmlg, bmsel, bmsel, u_moe_ne, u_moe_k, u_moe_gmode, nrows)
                if (g4moe) {
                    var bw_ds = upload_region(addr < void? >(t.fblob[t.moe_down_scale_off + l * c.n_expert]), uint64(c.n_expert * 4l))
                    enc_moe_wscale(enc, bmsel, bmsel, bw_ds, u_moe_k, nrows)
                }
                if (t.experts_mx4) {
                    var bwb1 = upload_region(addr < void? >(t.fblob[t.web1_off + l * c.n_expert * c.n_ff_exp]), uint64(c.n_expert * c.n_ff_exp * 4l))
                    var bwb3 = upload_region(addr < void? >(t.fblob[t.web3_off + l * c.n_expert * c.n_ff_exp]), uint64(c.n_expert * c.n_ff_exp * 4l))
                    enc_moe_gemv_mx4(enc, t, t.we1_offs[l], c.n_ff_exp, c.n_expert_used, nrows, bex, bgh,
                        u_dim, u_moe_nfe, bmsel, u_moe_eblk, u_zero32,
                        u_moe_ss, u_dim, u_moe_knfe, bwb1, u_moe_gmode)
                    enc_moe_gemv_mx4(enc, t, t.we3_offs[l], c.n_ff_exp, c.n_expert_used, nrows, bex, bgh2,
                        u_dim, u_moe_nfe, bmsel, u_moe_eblk, u_zero32,
                        u_moe_ss, u_dim, u_moe_knfe, bwb3, u_moe_gmode)
                } else {
                    enc_moe_gemv(enc, t, fe1, t.we1_offs[l], c.n_ff_exp, c.n_expert_used, nrows, bex, bgh,
                        u_dim, u_moe_nfe, bmsel, fe1 == KqFmt.q8 ? u_moe_eblk : u_moe_esb, u_zero32,
                        u_moe_ss, u_dim, u_moe_knfe)
                    enc_moe_gemv(enc, t, fe3, t.we3_offs[l], c.n_ff_exp, c.n_expert_used, nrows, bex, bgh2,
                        u_dim, u_moe_nfe, bmsel, fe3 == KqFmt.q8 ? u_moe_eblk : u_moe_esb, u_zero32,
                        u_moe_ss, u_dim, u_moe_knfe)
                }
            }
            if (moe && g_skip != "ew") {
                let pso_act = c.ffn_act == FfnAct.swiglu_oai ? g_pso_swiglu_oai : (g4moe ? g_pso_geglu : g_pso_swiglu)
                enc_ew2(enc, pso_act, bgh, 0ul, bgh2, 0ul, u_moe_tot, nrows * knfe)
            }
            if (moe && g_skip != "gemv") {
                let fe2 = kq_fmt_at(t.we2_fmt, l)
                if (t.experts_mx4) {
                    var bwb2 = upload_region(addr < void? >(t.fblob[t.web2_off + l * c.n_expert * dim]), uint64(c.n_expert * dim * 4l))
                    enc_moe_gemv_mx4(enc, t, t.we2_offs[l], dim, c.n_expert_used, nrows, bgh, bmdn,
                        u_moe_nfe, u_dim, bmsel, u_moe_eblk, u_moe_xs_dn,
                        u_moe_ss, u_moe_knfe, u_moe_kdim, bwb2, u_moe_gmode)
                } else {
                    enc_moe_gemv(enc, t, fe2, t.we2_offs[l], dim, c.n_expert_used, nrows, bgh, bmdn,
                        u_moe_nfe, u_dim, bmsel, fe2 == KqFmt.q8 ? u_moe_eblk : u_moe_esb, u_moe_xs_dn,
                        u_moe_ss, u_moe_knfe, u_moe_kdim)
                }
                enc_moe_combine(enc, bmdn, bmsel, g4moe ? bmg4rt : bxb, u_dim, u_moe_k, dim, nrows)
                if (g4moe && g_skip != "ew") {
                    var bw_pf2 = upload_region(addr < void? >(t.fblob[t.rms_post_ffn2_off + l * dim]), uint64(dim * 4l))
                    enc_rms_b(enc, bmg4rt, bw_pf2, bmg4rt, u_dim, u_eps, nrows)
                }
            }
            if ((!moe || g4moe) && g_skip != "gemv") {
                if (f1 != KqFmt.q8) {
                    // gate + up into DISJOINT bhb/bhb2 — the ew2 swiglu below fuses them
                    // (fuse13 stands down on kq loads)
                    enc_kq_site_b(enc, t, f1, t.w1_offs[l], hidden, dim, bxb, dim, bhb, hidden, 0l, u_dim, u_hidden, u_hidden, u_nrows_kq, nrows, mp)
                    enc_kq_site_b(enc, t, kq_fmt_at(t.w3_fmt, l), t.w3_offs[l], hidden, dim, bxb, dim, bhb2, hidden, 0l, u_dim, u_hidden, u_hidden, u_nrows_kq, nrows, mp)
                } elif (use_mm) {
                    let bw1 = blob_of(g_dev, t, t.w1_offs[l])
                    let bw3 = blob_of(g_dev, t, t.w3_offs[l])
                    enc_gemm_mm_b(enc, bw1.buf, bw1.boff, bxb, bhb, 0ul, u_dim, u_hidden, mp, hidden)
                    enc_gemm_mm_b(enc, bw3.buf, bw3.boff, bxb, bhb2, 0ul, u_dim, u_hidden, mp, hidden)
                } elif (mv) {
                    // two mv dispatches read the same norm and write DISJOINT gate/up — one
                    // barrier covers both; the ew2 swiglu below fuses them (fuse13 stands down)
                    let bw1 = blob_of(g_dev, t, t.w1_offs[l])
                    let bw3 = blob_of(g_dev, t, t.w3_offs[l])
                    enc_mv_site(enc, bw1.buf, bw1.boff, bxb, bhb,
                        u_dim, u_hidden, u_xs4_dim, u_hidden, hidden, nrows, hidden, uint64(dim * 4l), u_nrows_mv, u_nrows_mv2)
                    enc_mv_site(enc, bw3.buf, bw3.boff, bxb, bhb2,
                        u_dim, u_hidden, u_xs4_dim, u_hidden, hidden, nrows, hidden, uint64(dim * 4l), u_nrows_mv, u_nrows_mv2)
                } else {
                    let bw1 = blob_of(g_dev, t, t.w1_offs[l])
                    let bw3 = blob_of(g_dev, t, t.w3_offs[l])
                    if (fuse13) {
                        // one fused dispatch: gate+up dot + swiglu straight into bhb (w2's input)
                        enc_gemv_w13sw_b(enc, nrows > 2l, bw1.buf, bw1.boff, bw3.boff, bxb, bhb, u_dim, u_hidden, hidden)
                    } else {
                        // W1 and W3 both read the ffn-norm output and write DISJOINT buffers (bhb / bhb2)
                        // — one barrier (bxb RAW) covers both; they then run concurrently
                        if (use_gemm) {
                            enc_gemm_b(enc, bw1.buf, bw1.boff, bxb, bhb, u_dim, u_hidden, u_hidden, mp, hidden)
                            enc_gemm_b(enc, bw3.buf, bw3.boff, bxb, bhb2, u_dim, u_hidden, u_hidden, mp, hidden)
                        } else {
                            enc_gemv_b(enc, nrows > 2l, bw1.buf, bw1.boff, bxb, bhb, u_dim, u_hidden, u_hidden, hidden)
                            enc_gemv_b(enc, nrows > 2l, bw3.buf, bw3.boff, bxb, bhb2, u_dim, u_hidden, u_hidden, hidden)
                        }
                    }
                }
            }
            if ((!moe || g4moe) && g_skip != "ew" && !fuse13) {
                // the gate ew reads gate (bhb) + up (bhb2), writes act(gate)*up back into bhb
                enc_ew2(enc, geglu ? g_pso_geglu : g_pso_swiglu, bhb, 0ul, bhb2, 0ul, u_total_h, nrows * hidden)
            }
            if ((!moe || g4moe) && g_skip != "gemv") {
                if (f2 != KqFmt.q8) {
                    enc_kq_site_b(enc, t, f2, t.w2_offs[l], dim, hidden, bhb, hidden, bxb, dim, 0l, u_hidden, u_dim, u_dim, u_nrows_kq, nrows, mp)
                } elif (use_mm) {
                    let bw2 = blob_of(g_dev, t, t.w2_offs[l])
                    enc_gemm_mm_b(enc, bw2.buf, bw2.boff, bhb, bxb, 0ul, u_hidden, u_dim, mp, dim)
                } elif (mv) {
                    let bw2 = blob_of(g_dev, t, t.w2_offs[l])
                    enc_mv_site(enc, bw2.buf, bw2.boff, bhb, bxb,
                        u_hidden, u_dim, u_xs4_hid, u_dim, dim, nrows, dim, uint64(hidden * 4l), u_nrows_mv, u_nrows_mv2)
                } else {
                    let bw2 = blob_of(g_dev, t, t.w2_offs[l])
                    if (w2_sk > 0l) {
                        enc_gemm_sk_b(enc, bw2.buf, bw2.boff, bhb, bpsk, u_hidden, u_dim, u_sk_w2, dim, w2_sk)
                        enc_sk_reduce_b(enc, bpsk, bxb, u_skt_dim, u_sk_w2, 32l * dim)
                    } elif (use_gemm) {
                        enc_gemm_b(enc, bw2.buf, bw2.boff, bhb, bxb, u_hidden, u_dim, u_dim, mp, dim)
                    } else {
                        enc_gemv_b(enc, nrows > 2l, bw2.buf, bw2.boff, bhb, bxb, u_hidden, u_dim, u_dim, dim)
                    }
                }
            }
            if (g4moe && g_skip != "ew") {
                // dense-branch post-norm, then routed + dense summed into bxb for the epilogue
                var bw_pf1 = upload_region(addr < void? >(t.fblob[t.rms_post_ffn1_off + l * dim]), uint64(dim * 4l))
                enc_rms_b(enc, bxb, bw_pf1, bxb, u_dim, u_eps, nrows)
                enc_ew2(enc, g_pso_add, bxb, 0ul, bmg4rt, 0ul, u_total_dim, nrows * dim)
            }
            if (g_skip != "ew") {
                if (c.pre_post_norm) {
                    var bw_pf = upload_region(addr < void? >(t.fblob[t.rms_post_ffn_off + l * dim]), uint64(dim * 4l))
                    enc_rms_b(enc, bxb, bw_pf, bxb, u_dim, u_eps, nrows)
                }
                // gemma4 layer_out_scale multiplies the WHOLE layer output — (x + branch) * s
                // folds into the residual add's ascale slot (a 4-byte view into the weights blob)
                var bosc : MetalBuffer?
                if (c.layer_out_scale) {
                    bosc = upload_region(addr < void? >(t.fblob[t.out_scale_off + l]), 4ul)
                }
                if (l + 1l < c.n_layers) {
                    var bw_attn = upload_region(addr < void? >(t.fblob[t.rms_att_off + (l + 1l) * dim]), uint64(dim * 4l))
                    if (dim <= ADD_RMS_MAX_DIM && !g_b_addrms_unfused) {
                        enc_add_rms_b(enc, bx, bxb, bw_attn, bxb, u_dim, u_eps, nrows, bosc)
                    } else {
                        enc_ew2(enc, g_pso_add, bx, 0ul, bxb, 0ul, u_total_dim, nrows * dim, bosc)
                        enc_rms_b(enc, bx, bw_attn, bxb, u_dim, u_eps, nrows)
                    }
                } else {
                    // last layer: plain residual add into bx (the final rms reads bx)
                    enc_ew2(enc, g_pso_add, bx, 0ul, bxb, 0ul, u_total_dim, nrows * dim, bosc)
                }
            }
        }
        // group boundary: commit this slice so the driver schedules it under the earlier
        // slices' GPU execution; queue order serializes the pieces
        if ((l + 1l) % lay_per_cb == 0l && l + 1l < c.n_layers) {
            metal_end_encoding(enc)
            metal_release(enc)
            metal_commit(cb)
            cbs |> push(cb)
            cb = metal_new_command_buffer_unretained(g_queue)
            enc = conc ? metal_new_compute_encoder_concurrent(cb) : metal_new_compute_encoder(cb)
            // fresh encoder + in-order queue serialize cross-cb hazards — start the tracker clean
            hz_arm(conc)   // fresh encoder — start the tracker clean (queue order covers cross-cb)
        }
    }
    var nc_all = false
    if (gpu_logits) {
        // batched final rmsnorm + classifier — the caller's CPU classifier step is skipped
        unsafe {
            var bwfin = upload_region(addr < void? >(t.fblob[t.rms_final_off]), uint64(dim * 4l))
            enc_rms_b(enc, bx, bwfin, bxf, u_dim, u_eps, nrows)
            if (cls_fmt != KqFmt.q8) {
                enc_kq_site_b(enc, t, cls_fmt, t.wcls_off, c.vocab_size, dim, bxf, dim, blog, c.vocab_size, 0l, u_dim, u_vocab, u_vocab, u_nrows_kq, nrows, mp)
            } elif (use_mm) {
                let bwc = blob_of(g_dev, t, t.wcls_off)
                enc_gemm_mm_b(enc, bwc.buf, bwc.boff, bxf, blog, 0ul, u_dim, u_vocab, mp, c.vocab_size)
            } elif (mv) {
                let bwc = blob_of(g_dev, t, t.wcls_off)
                enc_mv_site(enc, bwc.buf, bwc.boff, bxf, blog,
                    u_dim, u_vocab, u_xs4_dim, u_vocab, c.vocab_size, nrows, c.vocab_size, uint64(dim * 4l), u_nrows_mv, u_nrows_mv2)
            } else {
                let bwc = blob_of(g_dev, t, t.wcls_off)
                if (cls64) {
                    enc_gemm64_b(enc, bwc.buf, bwc.boff, bxf, blog, u_dim, u_vocab, mp, c.vocab_size)
                } elif (use_gemm) {
                    enc_gemm_b(enc, bwc.buf, bwc.boff, bxf, blog, u_dim, u_vocab, u_vocab, mp, c.vocab_size)
                } else {
                    enc_gemv_b(enc, nrows > 2l, bwc.buf, bwc.boff, bxf, blog, u_dim, u_vocab, u_vocab, c.vocab_size)
                }
            }
        }
        if (u_flcap != null) {
            // final-logit soft cap (gemma2): cap*tanh over the live rows' contiguous logits
            enc_softcap_row(enc, blog, 0ul, u_fltot, u_flcap, nrows * c.vocab_size)
        }
        if (bsupp != null) {
            // gemma4 suppressed ids pin to -1e30 AFTER the cap (the CPU order) — per live row
            for (i in range64(nrows)) {
                enc_suppress_row(enc, blog, uint64(i * c.vocab_size * 4l), bsupp, u_nsupp, nsupp)
            }
        }
        // zero-copy logits: scatter each staging row into its session's wrapped storage ON the cb
        // (the KV writeback stays). All-or-nothing per step: one unwrappable session keeps the whole
        // step on the landing scatter. Pipe mode writes s.logits ASYNCHRONOUSLY — fixed-token-only.
        nc_all = true
        for (i in range64(nrows)) {
            nc_all &&= nc_logits_for(g_dev, *sessions[i]) != null
        }
        if (nc_all) {
            for (i in range64(nrows)) {
                enc_copy_row(enc, blog, uint64(i * c.vocab_size * 4l), nc_logits_for(g_dev, *sessions[i]), 0ul, u_vocab, c.vocab_size)
            }
        }
    }
    metal_end_encoding(enc)
    metal_release(enc)
    metal_commit(cb)
    cbs |> push(cb)
    hz_arm(false)   // disarm the oracle with the encoder — a leaked arm would eat the next CAPTURE's stage
    g_us_bencode += int64(get_time_usec(ts_enc))
    // optimistic watermark at commit: the in-order queue writes row pos before any later-committed
    // work reads the mirror — the pipe's fast-path prepare must not re-upload it from the (not yet
    // written back) CPU cache
    for (i in range64(nrows)) {
        var sp = sessions[i]
        if (key_exists(g_mirrors, sp.uid)) {
            let wm0 = g_mirrors[sp.uid].watermark
            g_mirrors[sp.uid].watermark = max(wm0, ws.positions[i] + 1l)
        }
    }
    // the overlap: the PREVIOUS step completes only now, with this one already committed behind it
    finish_pending_step()
    g_lp_err = false   // a lost pipelined step was logged + counted; this step retries fresh
    // stash this step as the pending one (finish_pending_step just cleared every slot)
    g_lp_active = true
    g_lp_cbs <- cbs
    g_lp_pooled |> push((buf = bx, size = bytes_x))
    g_lp_pooled |> push((buf = bxb, size = bytes_xb))
    g_lp_pooled |> push((buf = bxb2, size = bytes_x))
    g_lp_pooled |> push((buf = bqkv, size = bytes_qkv))
    g_lp_pooled |> push((buf = bhb, size = bytes_h))
    g_lp_pooled |> push((buf = bhb2, size = bytes_h))
    if (use_attn_d || chunked) {
        g_lp_pooled |> push((buf = bpart, size = bytes_part))
    }
    if (bytes_sk > 0ul) {
        g_lp_pooled |> push((buf = bpsk, size = bytes_sk))
    }
    if (moe) {
        g_lp_pooled |> push((buf = bmlg, size = bytes_mlg))
        g_lp_pooled |> push((buf = bmsel, size = bytes_msel))
        g_lp_pooled |> push((buf = bmdn, size = bytes_mdn))
        g_lp_upooled |> push((buf = u_moe_ne, size = 4ul))
        g_lp_upooled |> push((buf = u_moe_k, size = 4ul))
        g_lp_upooled |> push((buf = u_moe_nfe, size = 4ul))
        g_lp_upooled |> push((buf = u_moe_eblk, size = 4ul))
        g_lp_upooled |> push((buf = u_moe_esb, size = 4ul))
        g_lp_upooled |> push((buf = u_moe_ss, size = 4ul))
        g_lp_upooled |> push((buf = u_moe_xs_dn, size = 4ul))
        g_lp_upooled |> push((buf = u_moe_knfe, size = 4ul))
        g_lp_upooled |> push((buf = u_moe_kdim, size = 4ul))
        g_lp_upooled |> push((buf = u_moe_tot, size = 4ul))
        if (g4moe) {
            g_lp_pooled |> push((buf = bmg4x, size = bytes_mg4x))
            g_lp_pooled |> push((buf = bmg4rt, size = bytes_mg4x))
            g_lp_pooled |> push((buf = bmgh, size = bytes_mgh))
            g_lp_pooled |> push((buf = bmgh2, size = bytes_mgh))
            g_lp_upooled |> push((buf = u_moe_invd, size = 4ul))
        }
        g_lp_upooled |> push((buf = u_moe_gmode, size = 4ul))
    }
    if (u_hass != null) {
        g_lp_upooled |> push((buf = u_hass, size = 4ul))
    }
    if (use_attn_d) {
        g_lp_upooled |> push((buf = u_nsgs, size = 4ul))
    } elif (chunked) {
        g_lp_upooled |> push((buf = u_nch, size = 4ul))
    }
    if (u_nblk != null) {
        g_lp_upooled |> push((buf = u_nblk, size = 4ul))
    }
    if (gpu_logits) {
        g_lp_pooled |> push((buf = bxf, size = bytes_x))
        g_lp_pooled |> push((buf = blog, size = bytes_log))
        g_lp_upooled |> push((buf = u_vocab, size = 4ul))
    }
    if (u_flcap != null) {
        g_lp_upooled |> push((buf = u_flcap, size = 4ul))
        g_lp_upooled |> push((buf = u_fltot, size = 4ul))
    }
    if (bsupp != null) {
        g_lp_upooled |> push((buf = bsupp, size = uint64(nsupp * 4l)))
        g_lp_upooled |> push((buf = u_nsupp, size = 4ul))
    }
    if (u_winsl != null) {
        g_lp_upooled |> push((buf = u_winsl, size = 4ul))
    }
    if (hetero) {
        g_lp_upooled |> push((buf = u_qd_sw, size = 4ul))
        g_lp_upooled |> push((buf = u_kvd_sw, size = 4ul))
        g_lp_upooled |> push((buf = u_qkvd_sw, size = 4ul))
        g_lp_upooled |> push((buf = u_hs_sw, size = 4ul))
        g_lp_upooled |> push((buf = u_kvm_sw, size = 4ul))
        g_lp_upooled |> push((buf = u_scale_sw, size = 4ul))
        g_lp_upooled |> push((buf = u_npq_sw, size = 4ul))
        g_lp_upooled |> push((buf = u_npall_sw, size = 4ul))
        if (u_nblk_sw != null) {
            g_lp_upooled |> push((buf = u_nblk_sw, size = 4ul))
        }
    }
    if (u_voffe != null) {
        g_lp_upooled |> push((buf = u_voffe, size = 4ul))
    }
    if (u_voffe_sw != null) {
        g_lp_upooled |> push((buf = u_voffe_sw, size = 4ul))
    }
    g_lp_upooled |> push((buf = u_softcap, size = 4ul))
    g_lp_upooled |> push((buf = u_zero32, size = 4ul))
    if (qkv_sk > 0l) {
        g_lp_upooled |> push((buf = u_sk_qkv, size = 4ul))
        g_lp_upooled |> push((buf = u_skt_qkv, size = 4ul))
        if (u_skt_qkv_sw != null) {
            g_lp_upooled |> push((buf = u_skt_qkv_sw, size = 4ul))
        }
    }
    if (wo_sk > 0l) {
        g_lp_upooled |> push((buf = u_sk_wo, size = 4ul))
    }
    if (w2_sk > 0l) {
        g_lp_upooled |> push((buf = u_sk_w2, size = 4ul))
    }
    if (wo_sk > 0l || w2_sk > 0l) {
        g_lp_upooled |> push((buf = u_skt_dim, size = 4ul))
    }
    if (mv) {
        g_lp_upooled |> push((buf = u_xs4_dim, size = 4ul))
        g_lp_upooled |> push((buf = u_xs4_qd, size = 4ul))
        if (u_xs4_qd_sw != null) {
            g_lp_upooled |> push((buf = u_xs4_qd_sw, size = 4ul))
        }
        g_lp_upooled |> push((buf = u_xs4_hid, size = 4ul))
        if (u_nrows_mv != null) {
            g_lp_upooled |> push((buf = u_nrows_mv, size = 4ul))
        }
        if (u_nrows_mv2 != null) {
            g_lp_upooled |> push((buf = u_nrows_mv2, size = 4ul))
        }
    }
    // u_nrows_kq is allocated per (kquant, 2 <= B <= 8) INDEPENDENT of the mv rail — a kq step
    // with mv down (hetero, or B in the gemm band) must still land it (5 objects/run leaked
    // inside the old `if (mv)` guard once hetero stood mv down)
    if (u_nrows_kq != null) {
        g_lp_upooled |> push((buf = u_nrows_kq, size = 4ul))
    }
    g_lp_upooled |> push((buf = bcos, size = bytes_tab))
    g_lp_upooled |> push((buf = bsin, size = bytes_tab))
    if (dual_rope) {
        g_lp_upooled |> push((buf = bcos_swa, size = bytes_tab_sw))
        g_lp_upooled |> push((buf = bsin_swa, size = bytes_tab_sw))
    }
    g_lp_upooled |> push((buf = brt, size = bytes_rt))
    g_lp_upooled |> push((buf = u_dim, size = 4ul))
    g_lp_upooled |> push((buf = u_qd, size = 4ul))
    g_lp_upooled |> push((buf = u_kvd, size = 4ul))
    g_lp_upooled |> push((buf = u_qkvd, size = 4ul))
    g_lp_upooled |> push((buf = u_hidden, size = 4ul))
    g_lp_upooled |> push((buf = u_hs, size = 4ul))
    g_lp_upooled |> push((buf = u_kvm, size = 4ul))
    g_lp_upooled |> push((buf = u_nh, size = 4ul))
    g_lp_upooled |> push((buf = u_scale, size = 4ul))
    g_lp_upooled |> push((buf = u_eps, size = 4ul))
    g_lp_upooled |> push((buf = u_npq, size = 4ul))
    g_lp_upooled |> push((buf = u_npall, size = 4ul))
    g_lp_upooled |> push((buf = u_neox, size = 4ul))
    g_lp_upooled |> push((buf = u_hasb, size = 4ul))
    g_lp_upooled |> push((buf = u_s16, size = 4ul))
    g_lp_upooled |> push((buf = u_total_dim, size = 4ul))
    g_lp_upooled |> push((buf = u_total_h, size = 4ul))
    g_lp_upooled |> reserve((g_lp_upooled |> length()) + length(g_b_ulays))
    for (ul in g_b_ulays) {
        g_lp_upooled |> push((buf = ul, size = 4ul))
    }
    g_b_ulays |> clear()
    g_lp_blog = gpu_logits && !nc_all ? blog : null   // nc steps: the GPU scatter already fed s.logits
    g_lp_xb = null
    if (!gpu_logits) {
        g_lp_xb = bx   // the CPU-classifier tail reads x_b — sync-only writeback
        unsafe {
            g_lp_xb_dst = addr < void? >(ws.scr.x_b[0])
        }
        g_lp_xb_bytes = nrows * dim * 4l
    }
    g_lp_sessions |> clear()
    g_lp_sessions |> reserve(int(nrows))
    g_lp_positions |> resize(int(nrows))
    g_lp_koffs |> resize(int(nrows))
    g_lp_voffs |> resize(int(nrows))
    g_lp_caps |> resize(int(nrows))
    for (i in range64(nrows)) {
        g_lp_sessions |> push(sessions[i])
        g_lp_positions[i] = ws.positions[i]
        g_lp_koffs[i] = g_b_koffs[i]
        g_lp_voffs[i] = g_b_voffs[i]
        g_lp_caps[i] = g_b_caps[i]
    }
    g_lp_nrows = nrows
    g_lp_layers = c.n_layers
    g_lp_seq = c.seq_len
    g_lp_vocab = c.vocab_size
    // hetero (gemma4): per-layer row bytes + per-(row, layer) slab bases — uniform models
    // capture the same values the old l * cap * rowb arithmetic produced
    g_lp_lrowb |> resize(int(c.n_layers))
    g_lp_lkbase |> resize(int(nrows * c.n_layers))
    g_lp_lvbase |> resize(int(nrows * c.n_layers))
    for (l in range64(c.n_layers)) {
        g_lp_lrowb[l] = kv_row_bytes(kdt, layer_kv_dim(c, l))
        for (i in range64(nrows)) {
            g_lp_lkbase[i * c.n_layers + l] = mirror_lbase(t, kdt, g_lp_caps[i], l)
            g_lp_lvbase[i * c.n_layers + l] = mirror_lbase(t, kdt, g_lp_caps[i], l)
        }
    }
    if (!(g_b_pipe && gpu_logits && empty(g_skip))) {
        finish_pending_step()   // synchronous path: complete + read back immediately
        if (g_lp_err) {
            g_lp_err = false
            return false   // the dispatch failed — the caller reruns this step on the CPU loop
        }
    }
    if (gpu_logits) {
        batch_decode_override_logits_done()
    }
    return true
}



// Knobs resolved once at [init]. They were read per decode step, and get_env_variable allocates
// its result in the context heap on every call — see PERF027.
var private g_env_logits = true
var private g_env_batch_mm = false
var private g_env_attn_d = true
var private g_env_mtp_debug = ""

[cold_path]   // one-time env resolution for the batched decode knobs; the caller tests one global
def private load_batch_knobs() {
    g_bgemm_min = int64(max(env_int("DASLLAMA_METAL_BATCH_GEMM_MIN", 5), 2))
    g_bsk = env_int("DASLLAMA_METAL_BATCH_SK", 1) != 0
    g_bncb = int64(clamp(env_int("DASLLAMA_METAL_BATCH_NCB", 4), 1, 16))
    g_bconcurrent = env_int("DASLLAMA_METAL_BATCH_CONCURRENT", 1) != 0
    g_bfuse = env_int("DASLLAMA_METAL_BATCH_FUSE", 1) != 0
    g_bmv = env_int("DASLLAMA_METAL_BATCH_MV", 1) != 0
    g_bmv8 = env_int("DASLLAMA_METAL_BATCH_MV8", 1) != 0
    g_b_pipe = env_int("DASLLAMA_METAL_BATCH_PIPE", 0) != 0
}

[cold_path]   // one-time env resolution; decode_concurrent is a global test after the first call
def private load_decode_concurrent() {
    g_dconc = env_int("DASLLAMA_METAL_DECODE_CONCURRENT", 1)
}

[cold_path]   // one-time env resolution; decode_fuse_pre is a global test after the first call
def private load_decode_fuse() {
    g_dfuse = env_int("DASLLAMA_METAL_FUSE", 1)
}

[init]
def dasllama_metal_llama_register() {
    // registered but dormant: only select_decode_override("metal") (env rail DASLLAMA_PIN_DECODE
    // in the harnesses) activates it — the batch twin rides the same name — and unsupported
    // shapes decline per call
    register_decode_override("metal", @@metal_decode_forward)
    register_batch_decode_override("metal", @@metal_batch_decode_forward)
    register_mtp_spec_override("metal", @@metal_mtp_spec_eval)
    register_mtp_seam_override("metal", @@metal_mtp_seam_row)
    register_metal_servable(@@metal_model_servable_impl)   // load_model's image-flavor gate
    g_env_logits = env_flag("DASLLAMA_METAL_LOGITS", true)
    g_env_batch_mm = env_flag("DASLLAMA_METAL_BATCH_MM", false)
    g_env_attn_d = env_flag("DASLLAMA_METAL_ATTN_D", true)
    g_env_mtp_debug = env_str("DASLLAMA_MTP_DEBUG", "")
    if (has_env_variable("DASLLAMA_METAL_DECODE_SKIP")) {
        g_skip = get_env_variable("DASLLAMA_METAL_DECODE_SKIP")
        to_log(LOG_WARNING, "dasLLAMA metal decode: SKIP={g_skip} — timing-attribution mode, output is GARBAGE\n")
    }
    if (has_env_variable("DASLLAMA_METAL_SPEC")) {   // the spec chain's A/B rail: 0 off, 1 forced, -1 adaptive
        set_metal_decode_speculate(env_int("DASLLAMA_METAL_SPEC", -1))
    }
}
