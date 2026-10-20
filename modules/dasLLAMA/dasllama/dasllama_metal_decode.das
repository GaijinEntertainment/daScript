options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_metal_decode shared public

require dasllama/dasllama_lint public
require math
require strings
require dasllama/dasllama_env  // the g_env_* knob globals ([EnvConfig] declarations; ENVIRONMENT.md is generated from them)
require metal/das_metal_boost
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_gpu_math   // kv_ring_row: a mirror position's row
require dasllama/dasllama_ple   // ple_plane: the per-layer-embedding planes
require dasllama/dasllama_metal_common public
require dasllama/dasllama_metal_kernels public
require daslib/typemacro_boost   // [template_struct_instance] — the layer encoder's row-shape stamps

// The dasMetal GPU-resident DECODE path (the emission arc's P2 driver + P3 chase shape) — the
// mirror image of dasllama_metal_prefill: one command buffer per token step runs the whole llama
// layer stack on the GPU. Decode is GEMV-bandwidth-bound (one token streams every weight once)
// AND launch-gap-bound (~4.3us per serial dispatch), so the layer runs 9 fused dispatches:
//   fused QKV / W1|W3 GEMVs   the P1-lab-proven single-simdgroup-per-row Q8 GEMV
//                             (MetalQ8Gemv_metal_q8_gemv_msl, f32 activations) over CONCATENATED weight
//                             regions (upload_region_cat) into one output buffer each —
//                             downstream dispatches bind at byte offsets
//   metal_sq_attn16/32        single-query attention over the KV mirror, one threadgroup per
//                             (stream, head), simdgroup-cooperative coalesced K scoring; depth
//                             > 128 rows takes the fused single-pass metal_sq_attn_d where it
//                             serves the model, else metal_sq_attn_part16/32 + metal_sq_attn_comb
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
    discard_batch_pre()
    finish_pending_step()   // land any in-flight pipelined step before tearing down
    g_land_err = false
    metal_sel_release()
    metal_kernels_release()
    metal_common_release()
}


// ... plus the decode-specific gates (uniform f16/f32 KV, attention depth, a real session uid)
[unused_argument(pos)]   // depth retired (M2.4) — pos stays in the gate signature for future clauses
def private decode_decline(t : Model; s : Session; pos : int64) : MetalDecodeDecline {
    // mirror + writeback move raw row bytes, so the session codec must equal the mirror dtype; depth is capacity-bounded by the mirror arena, not gated here
    if (s.kv_dtype_k != s.kv_dtype_v ||
            (s.kv_dtype_k != KVDtype.f16 && s.kv_dtype_k != KVDtype.f32 &&
             s.kv_dtype_k != KVDtype.q8_0 && s.kv_dtype_k != KVDtype.tq4)) {
        return MetalDecodeDecline.kv_dtype
    }
    if (s.uid == 0ul) {
        return MetalDecodeDecline.no_uid
    }
    let shape = decode_shape_decline(t, DECODE_NEEDS_OK, true, true, true, allow_hc = true)
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
    if (long_length(s.rope_cos) < rot / 2l ||
            (has_dual_rope(t.config) && long_length(s.rope_cos_swa) < swa_hs / 2l)) {
        return MetalDecodeDecline.rope_rows
    }
    return MetalDecodeDecline.none
}


//! the Metal drivers gather the token rows on the device for a blob model whose table the gather serves: the decode step off its
//! poked id, the prefill window off the segments' ids; any call they then decline backfills on the host
[unused_argument(s, npos, pos)]
def private metal_embed_dev_gate(t : Model; s : Session; npos, pos : int64) : bool => emb_dev_gather_ok(t)

//! the speculative chain feeds itself on this model: the device gathers the argmax winner's row (`emb_dev_gather_ok`: the q8
//! linear copy, the fp32 table, or the table's K-quant plane by its tag)
def private spec_cls_capable(t : Model) : bool => emb_dev_gather_ok(t)

// ===== the per-step resource bundle + encode-ahead pipelining =====

// The batch step's weight-site tier, decided once per step: the tensor GEMM (use_mm), the tile
// GEMM (use_gemm), the split-K scratch, and the row count with its 32-padded form.
struct private BatchTier {
    use_mm : bool
    use_gemm : bool
    mp : int64
    nrows : int64
    bpsk : MetalBuffer?
}

let private ROUTE_ROW_BYTES = 16ul   //! one KV route row: k base, v base, cap, key count

// Everything one step's command buffer owns. The encode-ahead rail (g_pre) keeps ONE pre-encoded
// for the predicted next position; forward() pokes the token-dependent bytes (embed row + rope
// row) and COMMITS — encode cost overlaps the PREVIOUS step's GPU run instead of the per-token wall.
struct private StepRes {
    valid : bool
    pos : int64
    chain_pos : int64   // the position the layer chain's row 0 encodes at - the single row's own, the verify's step position (its StepRes sits one ahead), the batch's first row
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
    btok : MetalBuffer?   // the argmax winners: a spec step's one (read back to verify the prediction), a chained draft step's one a row
    btok_in : MetalBuffer?   //! the single row's token id, poked; the step gathers its row on the device (null = the host's row is poked)
    pick_ask : bool          //! the caller's sampler is a bare argmax: the step takes its own pick (`bpick`) and lands it in the logits' place
    bpick : MetalBuffer?
    bvarg : MetalBuffer?     //! the verify's rows' picks, one uint a row (`enc_argmax_rows`)
    sel_rows : int64         //! the rows whose selection the step's classifier tail landed (0 = none asked)
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
    blog : MetalBuffer?   // pooled staging — only when blog_zero_copy is unavailable
    blog_zero_copy : MetalBuffer?    // zero-copy logits: the session's wrapped storage (registry-owned, never released here)
    draft : bool          // MTP draft-head step (metal_mtp_draft_forward) — bcat/u_dim2 live
    bcat : MetalBuffer?   // draft eh_proj input: [enorm(embed) ; hnorm(mtp_h)] (2*dim)
    u_dim2 : MetalBuffer?
    nrows : int64         // activation-row count (1 normal, 2 = MTP verify); byte sizes pre-scaled
    verify : bool         // B=2 same-slab MTP verify step — the total-element uniforms live
    brt : MetalBuffer?    // the verify's and the batch's KV route table (one row per row per layer)
    bh12b : MetalBuffer?  // the batch's up panel - it keeps gate and up in two panels, bh12 the gate
    bmg4x : MetalBuffer?  // the batch's gemma4-MoE expert input, off the dense branch's panels
    bmgh : MetalBuffer?   // ... and its routed gate panel
    bmghb : MetalBuffer?  // ... and up panel
    bt : BatchTier        // the batch's weight-site tier
    mv_kdim : bool        // the batch's per-site mv form (the site's k aligned)
    mv_wo : bool
    mv_w2 : bool
    wo_sk : int64         // the batch's split-K at wo / w2 (0 = none)
    w2_sk : int64
    cls64 : bool
    fuse13 : bool         // the batch's fused gate+up form serves this step
    attn_d : bool         // the batch's decode-attention form (nsgs partials per row)
    d_nsgs : int64
    nchmax : int64        // the batch's chunk count over the deepest row
    u_total_dim : MetalBuffer?
    u_total_h : MetalBuffer?
    u_total_h2 : MetalBuffer?  // the second FFN width's row-total (the batch on a MatFormer E-series; null = uniform model)
    u_total_qd : MetalBuffer?
    u_dim : MetalBuffer?
    u_qd : MetalBuffer?
    u_kvd : MetalBuffer?
    u_qkvd : MetalBuffer?
    u_hidden : MetalBuffer?
    u_hidden2 : MetalBuffer?   // the second FFN width's uniform (MatFormer E-series; null = uniform model)
    hid0 : int64               // layer 0's FFN width — the per-layer uniform pick keys on it
    u_hs : MetalBuffer?
    u_cnt : MetalBuffer?
    u_scale : MetalBuffer?
    u_eps : MetalBuffer?
    u_vocab : MetalBuffer?
    u_nch : MetalBuffer?
    u_neox : MetalBuffer?  // rope pairing mode (0 = NORM adjacent, 1 = NEOX j/j+half)
    u_hasb : MetalBuffer?  // QKV bias present (the rope-store/qkv_rs epilogue reads the bias plane)
    u_softcap : MetalBuffer?  // attention-logit soft cap (0 = off) — every attention dispatch binds it
    u_zero32 : MetalBuffer?   // 0u — the window/chlo bind for global (non-sliding) layers
    u_winsl : MetalBuffer?    // sliding span (sliding models only; sliding layers bind it as `window`)
    u_chlo : MetalBuffer?     // first live chunk = wstart/64 (sliding models only; the comb skip)
    cls_epi : bool            //! the classifier epilogue runs: a final softcap or suppressed ids (gpu_logits only)
    // the SLIDING-class uniform twins (hetero models — gemma4's 16q/8kv/256 vs 16q/1kv/512;
    // uniform models leave these null and every layer binds the base set)
    hetero : bool
    u_hs_sw : MetalBuffer?
    u_kvd_sw : MetalBuffer?
    u_qd_sw : MetalBuffer?
    u_qkvd_sw : MetalBuffer?
    u_scale_sw : MetalBuffer?
    // scalar twins of the attention uniforms, for the kargs binds. Set beside the buffers with
    // the SAME expressions, so `het ? k_x_sw : k_x` mirrors `het ? u_x_sw : u_x` value for value.
    k_hs : uint
    k_hs_sw : uint
    k_kstride : uint
    k_kstride_sw : uint
    k_kvm : uint
    k_kvm_sw : uint
    k_scale : float
    k_scale_sw : float
    k_chlo : uint     // first live chunk (sliding) — the comb's skip start
    // ... and of the rope-store uniforms, same discipline
    k_qd : uint
    k_qd_sw : uint
    k_kvd : uint
    k_kvd_sw : uint
    k_npq : uint
    k_npq_sw : uint
    k_npall : uint
    k_npall_sw : uint
    k_nblk : uint
    k_nblk_sw : uint
    k_nh : uint
    k_neox : uint
    k_hasb : uint
    k_hasn : uint
    k_eps : float
    k_rot : uint      // partial rotary width; 0 = full, i.e. the layer's own head_size
    k_qkvd : uint
    k_qkvd_sw : uint
    k_voffe : uint    // v segment element offset (qd + kvd)
    k_voffe_sw : uint
    u_embscale : MetalBuffer?  // sqrt(dim) embed scale on the spec chain's GPU gather (gemma)
    bsupp : MetalBuffer?       // suppressed token ids (u32) - the gemma4 logits pin
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
    bxh : MetalBuffer?         //! the f16 twin of bxb's rows, eight rows deep - the k4 half-x forms' x; null = the step writes none
    bfgh : MetalBuffer?        //! the f16 twin of the gate panel after its activation - the w2 site's x
    bytes_xh : uint64
    bytes_fgh : uint64
    xh_ok : bool               //! bxh holds the twin of what bxb holds now: set by the norm that wrote both, cleared where another pass overwrites bxb
    fgh_ok : bool              //! the same for bfgh and the gate panel
    bmoe_rt : MetalBuffer?     // gemma4: router-input row, reused as the routed-branch acc
    u_moe_invd : MetalBuffer?  // gemma4: 1/sqrt(dim) router scale
    u_moe_gmode : MetalBuffer? // select gating mode (1 = gpt-oss softmax_weight)
    // scalar twins of the MoE GEMV uniforms (see the k_hs family above — same discipline)
    k_dim : uint
    k_moe_nfe : uint
    k_moe_eblk : uint
    k_moe_esb : uint
    k_moe_ss : uint
    k_moe_knfe : uint
    k_moe_kdim : uint
    k_moe_xs_dn : uint
    k_moe_gmode : uint
    u_moe_nsh : MetalBuffer?   // shexp width (qwen35moe; null = no shared expert)
    u_moe_shtot : MetalBuffer?   //! nrows * shexp width - the shared expert's activation span over the rows
    u_hass : MetalBuffer?      // attention sinks present (binds hass on the attn dispatches)
    // gemma-4 E-series per-layer embeddings — ple steps only: the side rows position-major [rows x layers*ple], built on the
    // device (`ple_dev`) or poked from the CPU pre-step's s.ple_inp
    ple : bool
    bple : MetalBuffer?        // the rows' per-layer side input
    u_ple : MetalBuffer?       // n_embd_per_layer
    bytes_ple : uint64
    ple_dev : bool             //! the step builds the side rows itself (`enc_ple_pre_dev`) off its token ids
    bple_proj : MetalBuffer?   //! [rows x layers*ple] the embedded rows' projection
    u_ple_all : MetalBuffer?   //! layers x ple: the side row's width and its rows' stride
    u_ple_sq : MetalBuffer?    //! sqrt(ple): the q8 table stamp's scale
    // qwen4exp hyper-connections (hc steps only, the single row): the wide residual made on device
    // from the poked embed row, and the mixer's scratch; the two sites' scatter logits keep apart
    hc : bool
    bhc : MetalBuffer?         // [nrows x hc x dim] the wide residual
    bhc_xn : MetalBuffer?      // [nrows x hc x dim] the normed streams
    bhc_lo : MetalBuffer?      // [nrows x low_rank]
    bhc_gate : MetalBuffer?    // [nrows x hc x dim] the up projection
    bhc_inj_a : MetalBuffer?   // [nrows x hc] the attention site's scatter logits
    bhc_inj_f : MetalBuffer?   // [nrows x hc] the ffn site's
    bhc_save : MetalBuffer?    // MTP steps: [nrows x hc x dim] the trunk's wide rows kept past the draft head's prelude (the carry the commit lands)
    bhc_carry : MetalBuffer?   // MTP steps: [nrows x hc x dim] the rows' carry inputs (the poked wide carries, the verify's committed history)
    bhc_cat : MetalBuffer?     // MTP steps: [nrows x hc x 2 dim] the per-stream eh_proj rows
    u_hcd : MetalBuffer?       // hc x dim
    u_hc : MetalBuffer?        // hc
    u_lr : MetalBuffer?        // low_rank
    u_lr_tot : MetalBuffer?    // nrows x low_rank
    u_hcd_tot : MetalBuffer?   // nrows x hc x dim
    bytes_hcd : uint64         // one wide row
    bytes_lr : uint64
    bytes_hc : uint64
    // the n-gram PLE side input (config.ple_layer >= 0): the CPU-gathered heads poke in; the ring rides the DnMirror
    png : bool
    bple_emb : MetalBuffer?    // [in] the gathered heads (poked, GPU-read-only)
    bple_key : MetalBuffer?    // [hc x dim] the key projection
    bple_val : MetalBuffer?    // [dim] the value projection
    bple_gated : MetalBuffer?  // [hc x dim] value x gate per stream
    bple_cin : MetalBuffer?    // [nrows x hc x dim] the verify rows' conv-input panel: read by every row's conv before the accepted rows land in the ring
    batch_rows : bool          // the batch driver's step: every row is its own session, so its side input writes its own ring in place
    ple_ring_base : uint64     // the ring's byte offset inside bdn_state (the live region)
    u_ple_in : MetalBuffer?
    bytes_ple_in : uint64
    // deltanet (Wave D) — recurrent-layer scratch + the gated-attention twins; dn steps only
    dn : bool
    bdn_state : MetalBuffer?   //! the DnMirror pool (mirror-owned, never released here)
    bdn_qkv : MetalBuffer?     // [cd] fused qkv projection row
    bdn_cv : MetalBuffer?      // [cd] conv output (l2-normed in place)
    bdn_z : MetalBuffer?       // [di] z gate row
    bdn_b : MetalBuffer?       //! [nvh] raw beta projections; [2 nvh] beta then alpha a row under `dn_ba_rows`
    bdn_g : MetalBuffer?       // [nvh] raw alpha projections
    dn_ba_rows : bool          //! the F32 beta/alpha planes ran as one GEMV into `bdn_b`
    u_nvh2 : MetalBuffer?      //! 2 nvh - the [beta; alpha] GEMV's row count
    bdn_grp : MetalBuffer?     //! the recurrent stages' group table (`dn_grp_push`)
    bytes_dn_grp : uint64
    dn_ng : int64
    dn_npmax : int64
    bdn_o : MetalBuffer?       // [di] delta-rule output (gated in place)
    dn_split : bool
    dn_rd : int64              //! the single row's mirror region its recurrent stages read
    dn_wr : int64              //! the single row's mirror region its recurrent stores land in, live once the step lands
    dn_epoch : int64           //! the pool generation the single row's group table was built over
    btape : MetalBuffer?
    tape_slot : uint64
    n_dn_taped : int64               // recurrent layers taped
    u_tcd : MetalBuffer?
    u_tnvh : MetalBuffer?
    bdn_qg : MetalBuffer?      // [2*qd] packed [q|gate] projection (gated-attention layers)
    bdn_gate : MetalBuffer?    // [qd] deinterleaved sigmoid-gate rows
    u_cd : MetalBuffer?
    u_nvh : MetalBuffer?
    u_di : MetalBuffer?
    u_one32 : MetalBuffer?     // constant 1 (dn npos, the shexp gate's 1-row GEMV) — unconditional
    u_dn_qd2 : MetalBuffer?    // 2*qd — the packed [q|gate] GEMV width
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
[arch(at="../ARCHITECTURE_GPU_MTP_DECODE.md#verify-row-pad")]
//! `deepest` = -1 means the step's rows reach no further than `pos`.
//! the single row builds its side row on the device: the pre-step serves the model and the token's id reaches the step
//! (`btok_in`, or a chained step's argmax)
def private metal_ple_decode_dev(t : Model) : bool => metal_ple_dev_said("decode", t) && emb_dev_gather_ok(t)

//! The E-series pre-step on the device over `nrows` decode rows (`enc_ple_pre`): the embedded rows `bx` projected into `bproj` on
//! the GEMV forms - the bf16 rows, else the f32 router GEMV
def enc_ple_pre_dev(enc : MetalComputeEncoder?; t : Model; btok : MetalBuffer?; toff : uint64; bx, bple, bproj, ball, bsq : MetalBuffer?; nrows : int64) {
    enc_ple_pre(enc, t, btok, toff, bple, bproj, ball, bsq, nrows) $ {
        let c = t.config
        let all = c.n_layers * c.n_embd_per_layer
        var ka = moe_router_args(all, c.dim, nrows)
        if (t.ple_model_bf16) {
            let bw = plane_of(g_dev, t, t.bf16blob, t.ple_model_off)
            ka.woff = uint(bw.boff / 2ul)
            enc_ple_proj_bf16_rows(enc, bw.buf, bx, bproj, ka, all, nrows, nrows > 1l)
        } else {
            var bw = upload_region(unsafe(addr < void? >(t.fblob[t.ple_model_off])), uint64(all * c.dim * 4l))
            enc_f32_rows_gemv(enc, bw, bx, bproj, g_one, ka, all, nrows, false)
        }
    }
}

//! a step's side rows (`bytes_ple` of them): on the device (`dev`) the gather and projection buffers, read and written by the
//! step itself; else the poked rows
def private ple_step_dev(t : Model; var r : StepRes; dev : bool) {
    r.ple_dev = dev
    r.u_ple_all = uniform_u32(uint(t.config.n_layers * t.config.n_embd_per_layer))
    if (dev) {
        r.bple = pool_acquire(g_pool, g_dev, r.bytes_ple)
        r.bple_proj = pool_acquire(g_pool, g_dev, r.bytes_ple)
        r.u_ple_sq = uniform_f32(sqrt(float(t.config.n_embd_per_layer)))
    } else {
        r.bple = pool_acquire_untracked(g_upool, g_dev, r.bytes_ple)
    }
}

def private ple_step_release(var r : StepRes) {
    if (r.ple_dev) {
        pool_release(g_pool, r.bple, r.bytes_ple)
        pool_release(g_pool, r.bple_proj, r.bytes_ple)
        pool_release(g_upool, r.u_ple_sq, 4ul)
    } else {
        pool_release(g_upool, r.bple, r.bytes_ple)
    }
    pool_release(g_upool, r.u_ple_all, 4ul)
}

//! The decode seat of the E-series pre-step (`register_ple_gpu_decode_gate`): the Metal driver takes the step and builds the
//! side rows on the device - the single row off its token id, the batched rows off theirs. A step the driver then declines
//! runs the CPU pre-step late (`metal_decode_served`, the engine's batch fallback)
[unused_argument(s, pos)]
def private metal_ple_decode_gate(t : Model; s : Session; npos, pos : int64) : bool {
    return false if (active_decode_override() != "metal")
    return npos == 1l ? metal_ple_decode_dev(t) : metal_ple_dev_said("batch decode", t)
}

def private acquire_step(t : Model; s : Session; pos : int64; koff, voff : uint64; cap : int64; draft : bool = false; nrows : int64 = 1l; deepest : int64 = -1l) : StepRes {   // nolint:STYLE037,STYLE038 — flat per-shape buffer/uniform acquire list
    let c = t.config
    let dim = c.dim
    let head_size = c.head_size
    let n_heads = c.n_heads
    let qd = n_heads * head_size
    let kv_dim = c.n_kv_heads * head_size
    let hidden = layer_hidden(t, 0l)
    let hid2 = ffn_second_hidden(t)   // decline guarantees 0 (uniform) or the one other width
    let hid_mx = max(hidden, hid2)
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
    let h2 = max(2l * hid_mx, max(2l * knfe, 2l * nsh))   // moe: bh12 holds k gate rows | k up rows (+ the shexp pair)
    let rot = c.rope_dim > 0l ? c.rope_dim : head_size   // partial-rope tables are rot/2 wide
    let mp = nrows <= 1l ? 1l : ((nrows + 3l) / 4l) * 4l
    var r = StepRes(valid = true, pos = pos, chain_pos = pos, uid = s.uid, nrows = nrows,
        kdt = s.kv_dtype_k, s16 = t.wscale_f16, hetero = hetero, moe = moe,
        dn = c.recr_mask != 0ul,
        koff = koff, voff = voff, cap = cap, arena_epoch = g_arena_epoch,
        bytes_x = uint64(mp * dim * 4l), bytes_q = uint64(mp * qd_mx * 4l), bytes_xb = uint64(mp * max(dim, qd_mx) * 4l),
        bytes_qkv = uint64(mp * qkvd_mx * 4l),
        bytes_h2 = uint64(mp * h2 * 4l), bytes_tab = uint64(nrows * (rot / 2l) * 4l),
        bytes_tab_sw = uint64(nrows * (hs_sw / 2l) * 4l),
        bytes_log = uint64(mp * c.vocab_size * 4l))
    r.bx = pool_acquire(g_pool, g_dev, r.bytes_x)
    r.bxb = pool_acquire(g_pool, g_dev, r.bytes_xb)   // rms out (dim) AND attention out (qd)
    r.bxb2 = pool_acquire(g_pool, g_dev, r.bytes_x)
    r.bqkv = pool_acquire(g_pool, g_dev, r.bytes_qkv)   // q | roped-in-place q, k, raw v
    r.bh12 = pool_acquire(g_pool, g_dev, r.bytes_h2)    // w1 gate | swiglu in place, w3 up
    if (kq_half_rows(t, nrows)) {
        r.bytes_xh = uint64(max(mp, KQ_HALF_TWIN_ROWS) * max(dim, qd_mx) * 2l)
        r.bytes_fgh = uint64(max(mp, KQ_HALF_TWIN_ROWS) * hid_mx * 2l)
        r.bxh = pool_acquire(g_pool, g_dev, r.bytes_xh)
        r.bfgh = pool_acquire(g_pool, g_dev, r.bytes_fgh)
    }
    r.bcos = pool_acquire_untracked(g_upool, g_dev, r.bytes_tab)
    r.bsin = pool_acquire_untracked(g_upool, g_dev, r.bytes_tab)
    if (has_dual_rope(c)) {
        r.bcos_swa = pool_acquire_untracked(g_upool, g_dev, r.bytes_tab_sw)
        r.bsin_swa = pool_acquire_untracked(g_upool, g_dev, r.bytes_tab_sw)
    }
    r.u_dim = uniform_u32(uint(dim))
    r.k_dim = uint(dim)
    r.u_qd = uniform_u32(uint(qd))
    r.k_qd = uint(qd)
    r.u_kvd = uniform_u32(uint(kv_dim))
    r.k_kvd = uint(kv_dim)
    r.u_qkvd = uniform_u32(uint(qkvd))
    r.k_qkvd = uint(qkvd)
    r.u_hidden = uniform_u32(uint(hidden))
    r.hid0 = hidden
    if (hid2 > 0l) {
        r.u_hidden2 = uniform_u32(uint(hid2))
    }
    r.u_hs = uniform_u32(uint(head_size))
    r.k_nh = uint(n_heads)
    r.u_cnt = uniform_u32(uint(pos + 1l))
    r.k_npq = uint(qd / 2l)
    r.k_npall = uint((qd + kv_dim) / 2l)
    // attn_scale overrides the 1/sqrt(hs) score scale (gemma4: 1.0 — QK-norm replaces it)
    r.u_scale = uniform_f32(c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size)))
    let blockc0 = r.kdt == KVDtype.q8_0 || r.kdt == KVDtype.tq4
    r.k_hs = uint(head_size)
    r.k_kstride = uint(blockc0 ? kv_dim / 32l : kv_dim)
    r.k_kvm = uint(n_heads / c.n_kv_heads)
    r.k_scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    r.u_eps = uniform_f32(c.norm_eps)
    r.k_eps = c.norm_eps
    if (c.v_norm || c.v_from_k_mask != 0ul) {
        r.k_voffe = uint(qd + kv_dim)
        if (hetero) {
            r.k_voffe_sw = uint(qd_sw + kvd_sw)
        }
    }
    if (hetero) {
        r.u_hs_sw = uniform_u32(uint(hs_sw))
        r.u_kvd_sw = uniform_u32(uint(kvd_sw))
        r.k_kvd_sw = uint(kvd_sw)
        r.u_qd_sw = uniform_u32(uint(qd_sw))
        r.k_qd_sw = uint(qd_sw)
        r.u_qkvd_sw = uniform_u32(uint(qkvd_sw))
        r.k_qkvd_sw = uint(qkvd_sw)
        r.u_scale_sw = uniform_f32(c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(hs_sw)))
        r.k_hs_sw = uint(hs_sw)
        r.k_kstride_sw = uint(blockc0 ? kvd_sw / 32l : kvd_sw)
        r.k_kvm_sw = uint(n_heads / kvh_sw)
        r.k_scale_sw = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(hs_sw))
        r.k_npq_sw = uint(qd_sw / 2l)
        r.k_npall_sw = uint((qd_sw + kvd_sw) / 2l)
    }
    r.u_neox = uniform_u32(c.rope_neox ? 1u : 0u)
    r.k_neox = c.rope_neox ? 1u : 0u
    r.u_hasb = uniform_u32(c.attn_qkv_bias ? 1u : 0u)
    r.k_hasb = c.attn_qkv_bias ? 1u : 0u
    r.k_hasn = c.qk_norm ? 1u : 0u
    r.u_softcap = uniform_f32(c.attn_logit_softcap)
    r.u_zero32 = uniform_u32(0u)
    r.u_one32 = uniform_u32(1u)
    if (c.sliding_window > 0l) {
        let chlo = uint((pos + 1l > c.sliding_window ? pos + 1l - c.sliding_window : 0l) / ATTN_CHUNK_ROWS)
        r.u_winsl = uniform_u32(uint(c.sliding_window))
        r.u_chlo = uniform_u32(chlo)
        r.k_chlo = chlo
    }
    if (r.kdt == KVDtype.q8_0 || r.kdt == KVDtype.tq4) {
        r.k_nblk = uint(kv_dim / 32l)
        if (hetero) {
            r.k_nblk_sw = uint(kvd_sw / 32l)
        }
    }
    if (moe) {
        let ege = uint(dim * c.n_ff_exp)
        r.bytes_moe_lg = uint64(mp * (c.n_expert + 1l) * 4l)   //! a row's logits and the shared expert's gate logit past them
        r.bytes_moe_sel = uint64(mp) * 128ul
        r.bytes_moe_dn = uint64(mp * c.n_expert_used * dim * 4l)
        r.bmoe_lg = pool_acquire(g_pool, g_dev, r.bytes_moe_lg)
        r.bmoe_sel = pool_acquire(g_pool, g_dev, r.bytes_moe_sel)   // k <= 16: [k idx][k w] per stream
        r.bmoe_dn = pool_acquire(g_pool, g_dev, r.bytes_moe_dn)
        r.u_moe_ne = uniform_u32(uint(c.n_expert))
        r.u_moe_k = uniform_u32(uint(c.n_expert_used))
        r.u_moe_nfe = uniform_u32(uint(c.n_ff_exp))
        r.k_moe_nfe = uint(c.n_ff_exp)
        r.u_moe_knfe = uniform_u32(uint(knfe))
        r.k_moe_knfe = uint(knfe)
        r.u_moe_ss = uniform_u32(uint(2l * c.n_expert_used))
        r.k_moe_ss = uint(2l * c.n_expert_used)
        r.u_moe_kdim = uniform_u32(uint(c.n_expert_used * dim))
        r.k_moe_kdim = uint(c.n_expert_used * dim)
        r.u_moe_tot = uniform_u32(uint(nrows * knfe))
        r.u_moe_eblk = uniform_u32(ege / 32u)
        r.k_moe_eblk = ege / 32u
        r.u_moe_esb = uniform_u32(ege / 256u)
        r.k_moe_esb = ege / 256u
        r.u_moe_xs_dn = uniform_u32(uint(c.n_ff_exp))
        r.k_moe_xs_dn = uint(c.n_ff_exp)
        if (c.moe_dense_shexp) {   // gemma4: router-input scratch + the 1/sqrt(dim) scale
            r.bmoe_rt = pool_acquire(g_pool, g_dev, r.bytes_x)
            r.u_moe_invd = uniform_f32(1.0 / sqrt(float(dim)))
        }
        if (nsh > 0l) {
            r.u_moe_nsh = uniform_u32(uint(nsh))
            r.u_moe_shtot = uniform_u32(uint(nrows * nsh))
        }
        if (c.moe_dense_shexp || nsh > 0l) {
            r.bytes_h12s = uint64(nrows * max(2l * hid_mx, 2l * nsh) * 4l)
            r.bh12s = pool_acquire(g_pool, g_dev, r.bytes_h12s)
        }
        r.u_moe_gmode = uniform_u32(c.moe_gate == MoeGate.softmax_weight ? 1u : (c.norm_topk_prob ? 0u : 2u))
        r.k_moe_gmode = c.moe_gate == MoeGate.softmax_weight ? 1u : (c.norm_topk_prob ? 0u : 2u)
    }
    if (c.attn_sinks) {
        r.u_hass = uniform_u32(1u)
    }
    if (r.dn) {
        let cd = dn_conv_dim(c)
        let di = c.ssm_d_inner
        r.bytes_dn_cd = uint64(mp * cd * 4l)   // bdn_qkv/bdn_cv
        r.bytes_dn_di = uint64(mp * di * 4l)   // bdn_z/bdn_o
        r.bytes_dn_h = uint64(mp * c.ssm_dt_rank * 4l)   // bdn_b/bdn_g
        r.bdn_qkv = pool_acquire(g_pool, g_dev, r.bytes_dn_cd)
        r.bdn_cv = pool_acquire(g_pool, g_dev, r.bytes_dn_cd)
        r.bdn_z = pool_acquire(g_pool, g_dev, r.bytes_dn_di)
        r.bdn_o = pool_acquire(g_pool, g_dev, r.bytes_dn_di)
        r.bdn_b = pool_acquire(g_pool, g_dev, 2ul * r.bytes_dn_h)
        r.bdn_g = pool_acquire(g_pool, g_dev, r.bytes_dn_h)
        r.u_cd = uniform_u32(uint(cd))
        r.u_nvh = uniform_u32(uint(c.ssm_dt_rank))
        r.u_di = uniform_u32(uint(di))
        r.dn_ba_rows = t.dn_ba_f32
        r.u_nvh2 = uniform_u32(uint(2l * c.ssm_dt_rank))
    }
    if (c.q_gated) {
        r.bdn_qg = pool_acquire(g_pool, g_dev, 2ul * r.bytes_q)
        r.bdn_gate = pool_acquire(g_pool, g_dev, r.bytes_q)
        r.u_dn_qd2 = uniform_u32(uint(2l * qd))
    }
    if (has_ple(c)) {   // E-series: the side-input row, built on the device off the step's token id or poked from the CPU pre-step
        r.ple = true
        r.bytes_ple = uint64(c.n_layers * c.n_embd_per_layer * 4l)
        r.u_ple = uniform_u32(uint(c.n_embd_per_layer))
        ple_step_dev(t, r, !draft && nrows == 1l && metal_ple_decode_dev(t))
    }
    if (c.hyper_conn) {   // qwen4exp: the wide residual + the mixer scratch, nrows rows (the single step, the MTP draft and verify rows)
        r.hc = true
        let hcd = hc_dim(c)
        r.bytes_hcd = uint64(hcd * 4l)
        r.bytes_lr = uint64(c.hc_low_rank * 4l)
        r.bytes_hc = uint64(c.hc_count * 4l)
        let rows = uint64(mp)
        r.bhc = pool_acquire(g_pool, g_dev, rows * r.bytes_hcd)
        r.bhc_xn = pool_acquire(g_pool, g_dev, rows * r.bytes_hcd)
        r.bhc_gate = pool_acquire(g_pool, g_dev, rows * r.bytes_hcd)
        r.bhc_lo = pool_acquire(g_pool, g_dev, rows * r.bytes_lr)
        r.bhc_inj_a = pool_acquire(g_pool, g_dev, rows * r.bytes_hc)
        r.bhc_inj_f = pool_acquire(g_pool, g_dev, rows * r.bytes_hc)
        r.u_hcd = uniform_u32(uint(hcd))
        r.u_hc = uniform_u32(uint(c.hc_count))
        r.u_lr = uniform_u32(uint(c.hc_low_rank))
        r.u_lr_tot = uniform_u32(uint(nrows * c.hc_low_rank))
        r.u_hcd_tot = uniform_u32(uint(nrows * hcd))
        if (draft || nrows > 1l) {   // the draft head's wide carry rows, the trunk rows the commit lands, the per-stream cat rows
            r.bhc_save = pool_acquire(g_pool, g_dev, rows * r.bytes_hcd)
            r.bhc_carry = pool_acquire(g_pool, g_dev, rows * r.bytes_hcd)
            r.bhc_cat = pool_acquire(g_pool, g_dev, rows * 2ul * r.bytes_hcd)
        }
    }
    if (has_ple_ngram(c)) {   // qwen4exp: the poked heads + the two projections' rows (nrows of each)
        r.png = true
        r.bytes_ple_in = uint64(ple_in_dim(c) * 4l)
        r.bple_emb = pool_acquire_untracked(g_upool, g_dev, uint64(mp) * r.bytes_ple_in)
        r.bple_key = pool_acquire(g_pool, g_dev, uint64(mp) * r.bytes_hcd)
        r.bple_gated = pool_acquire(g_pool, g_dev, uint64(mp) * r.bytes_hcd)
        if (nrows > 1l) {
            r.bple_cin = pool_acquire(g_pool, g_dev, uint64(mp) * r.bytes_hcd)
        }
        r.bple_val = pool_acquire(g_pool, g_dev, r.bytes_x)
        r.u_ple_in = uniform_u32(uint(ple_in_dim(c)))
    }
    if (rot != head_size) {
        r.k_rot = uint(rot)
    }
    // logits-on-GPU (default ON, DASLLAMA_METAL_LOGITS=0 pins the CPU classifier) mirrors the prefill gates
    let tied_f32 = cls_tied_f32(t)
    // tied-fp32 stays CPU; blob models FORCE GPU logits (no CPU planes); MTP serves — finish_step stashes mtp_h from bxf
    r.gpu_logits = ((t.metal_blob || (!g_logits_cpu && g_env_logits && empty(g_skip))) &&
        !tied_f32 &&
        t.wcls_off >= 0l)
    if (r.gpu_logits) {
        r.cls_epi = c.final_logit_softcap > 0.0 || !empty(t.suppress)   //! cap*tanh, then the pins, after the classifier GEMV
        if (!empty(t.suppress)) {   // gemma4: pin the suppressed ids to -1e30 after the cap
            r.bytes_supp = uint64(long_length(t.suppress) * 4l)
            r.bsupp = pool_acquire_untracked(g_upool, g_dev, r.bytes_supp)
            unsafe {
                var sp = reinterpret<uint?>(metal_buffer_contents(r.bsupp))
                for (i in range(length(t.suppress))) {
                    sp[i] = uint(t.suppress[i])
                }
            }
        }
        if (c.embed_scale != 1.0) {   // the spec gather's escale bind (gemma; identity rides g_one)
            r.u_embscale = uniform_f32(c.embed_scale)
        }
        r.bxf = pool_acquire(g_pool, g_dev, r.bytes_x)
        // zero-copy logits write straight into s.logits; the pooled staging is fallback (spec-capable models keep staging too — the speculative chain must only ever touch pooled memory). Multi-row steps (verify) always stage.
        r.blog_zero_copy = nrows == 1l ? nc_logits_for(g_dev, s) : null
        if (r.blog_zero_copy == null || spec_cls_capable(t)) {
            r.blog = pool_acquire(g_pool, g_dev, r.bytes_log)
        }
        r.u_vocab = uniform_u32(uint(c.vocab_size))
    }
    // chunked split-K attention above the single-tg depth: the form and the partials follow the DEEPEST row the step reaches, never row 0's depth
    let top = deepest >= 0l ? deepest : pos
    r.chunked = top + nrows > g_attn_single_max
    let nchunks = (top + nrows + ATTN_CHUNK_ROWS - 1l) / ATTN_CHUNK_ROWS
    r.bytes_part = uint64(nrows * n_heads * nchunks * (hs_mx + 2l) * 4l)
    //! the single row's route addresses its slice from the arena's start: a slice past that reach takes the chunked pair
    r.attn_d = (r.chunked && attn_d_serves(head_size, hs_sw) && (nrows == 1l || (c.sliding_window == 0l && !c.attn_sinks))
        && (nrows > 1l || mirror_in_reach(r.kdt, voff + uint64(cap * kv_row_bytes(r.kdt, kv_row_total(t))))))
    if (r.attn_d) {
        r.d_nsgs = attn_d_groups(top + nrows)
        r.bytes_part = uint64(attn_d_part_floats(nrows, n_heads, hs_mx, r.d_nsgs) * 4l)
        r.bpart = pool_acquire(g_pool, g_dev, r.bytes_part)
    }
    if (r.attn_d && nrows == 1l) {
        r.brt = pool_acquire_untracked(g_upool, g_dev, ROUTE_ROW_BYTES)
        let esz = uint64(blockc0 ? 1l : kv_row_bytes(r.kdt, 1l))   // nolint:LINT021 — divides uint64 arena offsets; only the quotient narrows
        var route = fixed_array(uint(koff / esz), uint(voff / esz), uint(cap), uint(pos + 1l))
        unsafe {
            memcpy(metal_buffer_contents(r.brt), addr<void?>(route[0]), int64(ROUTE_ROW_BYTES))
        }
    } elif (r.chunked && !r.attn_d) {
        r.bpart = pool_acquire(g_pool, g_dev, r.bytes_part)
        r.u_nch = uniform_u32(uint(nchunks))
    }
    if (!draft && nrows == 1l && emb_dev_gather_ok(t)) {
        r.btok_in = pool_acquire_untracked(g_upool, g_dev, 4ul)
    }
    if (draft || nrows > 1l) {   // the verify's warm sub-pass shares the draft's cat/eh_proj plumbing
        r.draft = draft
        r.bcat = pool_acquire(g_pool, g_dev, 2ul * r.bytes_x)
        r.u_dim2 = uniform_u32(uint(2l * dim))
    }
    if (nrows > 1l) {
        r.verify = true
        r.u_total_dim = uniform_u32(uint(nrows * dim))
        r.u_total_h = uniform_u32(uint(nrows * hidden))
        r.u_total_qd = uniform_u32(uint(nrows * qd))
    }
    return r
}

// the token-dependent input bytes — the ONLY step state not known at encode time
def private poke_step(s : Session; r : StepRes; token : int64) {
    unsafe {
        if (r.btok_in != null) {
            *reinterpret<uint?>(metal_buffer_contents(r.btok_in)) = uint(token)
        } else {
            memcpy(metal_buffer_contents(r.bx), addr < void? >(s.x[0]), r.bytes_x)
        }
        memcpy(metal_buffer_contents(r.bcos), addr < void? >(s.rope_cos[0]), r.bytes_tab)
        memcpy(metal_buffer_contents(r.bsin), addr < void? >(s.rope_sin[0]), r.bytes_tab)
        if (r.bcos_swa != null) {
            memcpy(metal_buffer_contents(r.bcos_swa), addr < void? >(s.rope_cos_swa[0]), r.bytes_tab_sw)
            memcpy(metal_buffer_contents(r.bsin_swa), addr < void? >(s.rope_sin_swa[0]), r.bytes_tab_sw)
        }
        if (r.ple && !r.ple_dev) {   // forward()'s ple_pre_decode filled s.ple_inp before the hook
            memcpy(metal_buffer_contents(r.bple), addr < void? >(s.ple_inp[0]), r.bytes_ple)
        }
        if (r.png) {   // the arch's pre-stack step gathered this token's heads before the hook
            memcpy(metal_buffer_contents(r.bple_emb), addr < void? >(s.ple_emb[0]), r.bytes_ple_in)
        }
    }
}

// The layer encoder: a layer is the same chain at every row shape (QKV, norms, rope+store,
// attention, wo, FFN, the residual adds); the shapes differ in how a weight site dispatches and
// which kernel family serves a phase. LayerEncT carries the chain once, a stamp binds its family.

// which weight site a dispatch serves - the batch tier keys its mv/split-K choice on it
enum private SiteKind {
    qkv
    wo
    w13
    w2
    cls
    other       // deltanet projections, PLE, the shared expert - the plain row form
}

//! the single row's site: one GEMV, y contiguous at column ycol
[unused_argument(e, kind, ys, bys)]
def private site_single(var e : SingleLayerEnc; t : Model; enc : MetalComputeEncoder?; kind : SiteKind; fmt : KqFmt;
                        woff, rows, n, ys : int64; bx, by, bn, bd, bys : MetalBuffer?; ycol : int64) {
    enc_site_gemv(enc, t, fmt, woff, rows, n, bx, by, bn, bd, uint64(ycol * 4l))
}

// the FFN panels a family keeps: the expert input, the routed gate / up destinations and the
// dense gate / up destinations - a column is in elements of the panel's row
struct private FfnPanels {
    moe_in : MetalBuffer?
    mg : MetalBuffer?
    mu : MetalBuffer?
    mu_col : int64
    fg : MetalBuffer?
    fu : MetalBuffer?
    fu_col : int64
}

//! the single row's panels: gate and up share one panel at the row's width, the gemma4 dense
//! branch its own, the expert input the pre-norm's scratch row
[unused_argument(moe_with_dense)]
def private panels_single(var e : SingleLayerEnc; moe_with_dense : bool; hid_l, knfe : int64) : FfnPanels {
    var r & = unsafe(*e.r)
    return FfnPanels(moe_in = r.bxb2, mg = r.bh12, mu = r.bh12, mu_col = knfe,
        fg = moe_with_dense ? r.bh12s : r.bh12, fu = moe_with_dense ? r.bh12s : r.bh12, fu_col = hid_l)
}

//! the single row's totals are the per-row uniforms
[unused_argument(e)]
def private total_single(var e : SingleLayerEnc; u : MetalBuffer?; n : int64) : tuple<b : MetalBuffer?; n : int64> => (b = u, n = n)

//! the single row's residual step at seam `sm` (`enc_residual`); the fuse knob off splits a sandwich seam
def private residual_single(var e : SingleLayerEnc; t : Model; enc : MetalComputeEncoder?; bx, ba, by : MetalBuffer?; sm : ResidualSeam) {
    var r & = unsafe(*e.r)
    enc_residual(enc, t, bx, ba, by, null, sm, 1l, ResidualForm.one_row, sm.pre_off >= 0l && !decode_fuse_pre(), ResidualUniforms(bdim = r.u_dim, beps = r.u_eps, btot = r.u_dim))
}

//! whether layer l's gemma-4 MoE tail runs as the one step (`MetalMoeCombG4`): a pre/post-norm layer with a next layer to norm for,
//! no per-layer-embedding or hyper-connection seam between, a row the step's stash holds, no knockout active
[arch(at="../ARCHITECTURE_GPU_PREFILL_MOE.md#metal-g4-moe-step")]
def private moe_g4_step_serves(t : Model; l : int64; seam : bool) : bool {
    let c = t.config
    return (!g_moe_g4_unfused && c.pre_post_norm && !seam && l + 1l < c.n_layers && c.dim <= ADD_RMS_MAX_DIM && g_skip == "")
}

//! `nrows` rows' gemma-4 step: x updated in place, its next norm into bxb, its f16 twin into bxh where `twin`
def private moe_g4_step_enc(var r : StepRes; t : Model; enc : MetalComputeEncoder?; l, nrows : int64; twin : bool) {
    var ka = moe_g4_layer_args(t, l, false)
    if (twin) {
        enc_moe_comb_g4_hx(enc, r.bx, r.bmoe_sel, r.bmoe_dn, r.bxb, moe_g4_norms(t, l, true), r.bxb, r.bxh, ka, nrows)
    } else {
        enc_moe_comb_g4(enc, r.bx, r.bmoe_sel, r.bmoe_dn, r.bxb, moe_g4_norms(t, l, true), r.bxb, ka, nrows)
    }
}

//! the single row's gemma-4 step, no f16 twin
def private moe_g4_step_single(var e : SingleLayerEnc; t : Model; enc : MetalComputeEncoder?; l : int64) {
    moe_g4_step_enc(*e.r, t, enc, l, 1l, false)
}

def private rms_single(var e : SingleLayerEnc; enc : MetalComputeEncoder?; bs : MetalBuffer?; soff : uint64; bw, bd : MetalBuffer?) {
    enc_rms(enc, bs, soff, bw, bd, e.r.u_dim, e.r.u_eps)
}

def private add_single(var e : SingleLayerEnc; enc : MetalComputeEncoder?; bg : MetalBuffer?; goff : uint64; bu : MetalBuffer?; uoff : uint64; dim : int64; bosc : MetalBuffer?) {
    enc_add(enc, bg, goff, bu, uoff, e.r.u_dim, dim, bosc)
}

//! the single row's expert args: one shared activation row, no stream strides
[unused_argument(e, down)]
def private moe_args_single(var e : SingleLayerEnc; ndim, ddim, eplane, xs, hasb : uint; down : bool) : MoeGemvArgs => MoeGemvArgs(ndim = ndim, ddim = ddim, eplane = eplane, xs = xs, ss = 0u, bxs = 0u, bys = 0u, hasb = hasb)

//! whether a layer's q/k norm, rope and K/V store run as the one fused pass (`GkQknRopeKv`): a q/k-norm model on a scalar mirror
def private qkn_fused(c : Config; kdt : KVDtype) : bool => c.qk_norm && (kdt == KVDtype.f16 || kdt == KVDtype.f32)

//! whether a gated layer's q stays in its [q | gate] rows (`bdn_qg`, a head 2 hs apart): the rope and the attention read it there
//! and the attention's store (tq4: the un-rotation) gates; a codec past the fused and the block-codec ropes keeps the deinterleave
def private qg_direct(c : Config; kdt : KVDtype) : bool => c.q_gated && (kdt == KVDtype.q8_0 || kdt == KVDtype.tq4 || qkn_fused(c, kdt))

//! the fused pass's args for layer l: its rows `qkvd` apart in the [q | k | v] buffer, the cos and sin rows `cosrow` apart, `lay`
//! the per-layer stride the route table's layer index steps; the V row the k row's under v-norm with no v plane, else its own
def private qkn_args(t : Model; l, qd, kv_dim, head_size, rot : int64; neox, hasb : bool; eps : float; qkvd, cosrow, lay : int64) : GkQknRopeArgs {
    let c = t.config
    let nowv = t.wv_offs[l] < 0l
    return GkQknRopeArgs(qd = uint(qd), kvd = uint(kv_dim), hs = uint(head_size), half = uint(rot / 2l), neox = neox ? 1u : 0u,
        nh = uint(qd / head_size), layerbase = uint(lay), kwoff = uint(head_size), eps = eps, hasb = hasb ? 1u : 0u,
        vfk = c.v_norm && nowv ? 1u : 0u, vnorm = c.v_norm && !nowv ? 1u : 0u, kvrow = uint(qkvd), qrow = uint(qkvd), cosrow = uint(cosrow))
}

//! layer l's [q norm | k norm] weight rows
def private qkn_weights(t : Model; l, head_size : int64) : MetalBuffer? {
    unsafe {
        return upload_region_cat(addr < void? >(t.fblob[t.rms_q_offs[l]]), uint64(head_size * 4l),
            addr < void? >(t.fblob[t.rms_k_offs[l]]), uint64(head_size * 4l))
    }
}

//! the single row's rope + KV store: the codec's form over the mirror's byte offsets
[unused_argument(rowb)]
def private rope_store_single(var e : SingleLayerEnc; t : Model; enc : MetalComputeEncoder?; l : int64; het, kvsh : bool;
                              bbias, bcos_l, bsin_l : MetalBuffer?; lkoff, lvoff : uint64; rowb, qd, kv_dim, head_size, n_heads : int64) {
    let c = t.config
    var r & = unsafe(*e.r)
    let f16v = r.kdt == KVDtype.f16
    let k_hs_l = het ? r.k_hs_sw : r.k_hs
    let k_qd_l = het ? r.k_qd_sw : r.k_qd
    let k_kvm_l = het ? r.k_kvm_sw : r.k_kvm
    let k_npq_l = het ? r.k_npq_sw : r.k_npq
    let k_npall_l = het ? r.k_npall_sw : r.k_npall
    let k_nblk_l = het ? r.k_nblk_sw : r.k_nblk
    let pos = r.pos
    // shared-KV layers dispatch the Q rows only — no K rope, no mirror row store
    let rs_rot = r.k_rot != 0u ? r.k_rot : k_hs_l   // 0 = full rope, i.e. this layer's head size
    let gd = qg_direct(c, r.kdt)
    let bq = gd ? r.bdn_qg : null
    let qhs = gd ? 2u * k_hs_l : 0u
    unsafe {
        if (r.kdt == KVDtype.q8_0) {
            var qka = RopeStoreQ8Args(qd = k_qd_l, head_size = k_hs_l, npairs_q = k_npq_l,
                nblk = k_nblk_l, neox = r.k_neox, has_bias = r.k_hasb, rot = rs_rot, qhs = qhs)
            enc_rope_store_q8(enc, r.bqkv, uint64((qd + kv_dim) * 4l), g_arena, g_arena,
                lkoff + uint64(mirror_row_at(t, r.kdt, l, pos)), lvoff + uint64(mirror_row_at(t, r.kdt, l, pos)),
                bcos_l, bsin_l, bbias, qka,
                kvsh ? qd / 2l : q8_store_threads(qd, kv_dim), bq)
        } elif (r.kdt == KVDtype.tq4) {
            var tka = RopeStoreTq4Args(qd = k_qd_l, head_size = k_hs_l, nheads = r.k_nh,
                kv_mul = k_kvm_l, neox = r.k_neox, has_bias = r.k_hasb, rot = rs_rot, qhs = qhs)
            enc_rope_store_tq4(enc, r.bqkv, uint64((qd + kv_dim) * 4l), g_arena, g_arena,
                lkoff + uint64(mirror_row_at(t, r.kdt, l, pos)), lvoff + uint64(mirror_row_at(t, r.kdt, l, pos)),
                bcos_l, bsin_l, bbias, tka,
                kvsh ? n_heads : n_heads + 2l * (kv_dim / head_size), head_size, bq)
        } elif (qkn_fused(c, r.kdt)) {
            var qka = qkn_args(t, l, qd, kv_dim, head_size, int64(rs_rot), r.k_neox != 0u, r.k_hasb != 0u, r.k_eps, 0l, 0l, 0l)
            qka.qstride = qhs
            enc_qkn_rope(enc, f16v, r.bqkv, g_arena, g_arena, lkoff + uint64(mirror_row_at(t, r.kdt, l, pos)), lvoff + uint64(mirror_row_at(t, r.kdt, l, pos)),
                bcos_l, bsin_l, 0ul, qkn_weights(t, l, head_size), bbias, qka, kvsh ? n_heads : n_heads + kv_dim / head_size, bq)
        } else {
            var rka = gk_rope_kv_args(int64(k_qd_l), kv_dim, int64(k_hs_l), int64(k_npq_l), int64(k_npall_l), r.k_neox != 0u, r.k_hasb != 0u, int64(rs_rot))
            enc_rope_store(enc, f16v, r.bqkv, uint64((qd + kv_dim) * 4l), g_arena, g_arena,
                lkoff + uint64(mirror_row_at(t, r.kdt, l, pos)), lvoff + uint64(mirror_row_at(t, r.kdt, l, pos)),
                bcos_l, bsin_l, 0ul, bbias, rka,
                kvsh ? qd / 2l : (qd + kv_dim) / 2l)
        }
    }
}

//! Whether the fused single-pass attention serves a model's rows: a lane owns one quad of the head (a head of 64, 96 or 128),
//! a simdgroup 128 dims of a wide one (a head of 256 or 512), on every mirror codec. A model's two head classes each
//! need their stamp: `hs_sw` is the sliding class's head, the base head where there is one class.
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def private attn_d_serves(head_size, hs_sw : int64) : bool
    => attn_d_head_ok(head_size) && attn_d_head_ok(hs_sw) && g_env_attn_d

//! Layer `l`'s fused single-pass attention over the rows of `r.brt` at `rtoff`, merged into `r.bxb`. Under `route_per_layer` each
//! route base is already the layer's; else the kernel steps a stream's slab to the layer. `window` is a sliding layer's span in
//! keys (0 = the whole context), `bsnk` the layer's sink logits (null = none).
def private encode_attn_d(t : Model; r : StepRes; enc : MetalComputeEncoder?; l, head_size, n_heads : int64;
                          window : uint; bsnk : MetalBuffer?; rtoff : uint64 = 0ul; route_per_layer : bool = false) {
    let blockc = r.kdt == KVDtype.q8_0 || r.kdt == KVDtype.tq4
    let c = t.config
    let kvd = uint(layer_kv_dim(c, l))
    let lays = route_per_layer ? uint2(0u, 0u) : mirror_route_lay(t, r.kdt, l)
    //! the layer's own head class: `head_size` and the K/V row are its, so its heads a K/V head and its score scale follow
    let gd = qg_direct(c, r.kdt)
    var dka = SqAttnDArgs(lay = lays.x, layr = lays.y, ring = uint(mirror_ring(t, l)), kstride = blockc ? kvd / 32u : kvd, nheads = uint(n_heads),
        kv_mul = uint(n_heads * head_size) / kvd, hs = uint(head_size),
        scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size)),
        qstride = gd ? uint(2l * n_heads * head_size) : uint(n_heads * head_size) + 2u * kvd, nsgs = uint(r.d_nsgs),
        window = window, softcap = c.attn_logit_softcap, qhs = gd ? uint(2l * head_size) : 0u,
        gated = gd && r.kdt != KVDtype.tq4 ? 1u : 0u)
    enc_attn_d(enc, r.kdt, gd ? r.bdn_qg : r.bqkv, r.bpart, r.brt, dka, r.d_nsgs, r.nrows, n_heads, rtoff)
    let halves = attn_d_halves(head_size)   // the wide head's partials are two or four heads of 128 to the combine
    var cka = SqAttnCombDArgs(nchmax = uint(r.d_nsgs), nheads = uint(n_heads * halves), hs = uint(head_size / halves),
        hass = bsnk != null ? 1u : 0u, hdiv = uint(halves))
    enc_attncombd_c(enc, r.bpart, r.bxb, r.brt, rtoff, cka, bsnk, n_heads * halves, r.nrows)
}

//! the single row's attention: past the single-form ceiling the fused single-pass form where it serves the model and the chunked pair elsewhere, else one dispatch
[unused_argument(l)]
def private attention_single(var e : SingleLayerEnc; t : Model; enc : MetalComputeEncoder?; l : int64; lsl, het : bool;
                             bsnk : MetalBuffer?; lkoff, lvoff : uint64; nchunks, head_size, n_heads : int64) {
    let c = t.config
    var r & = unsafe(*e.r)
    let cnt = uint(r.pos + 1l)
    // one kargs per shape; `kstride` is elements (f16/f32) or 32-blocks (q8_0/tq4)
    let kstride_l = het ? r.k_kstride_sw : r.k_kstride
    let kvm_u = het ? r.k_kvm_sw : r.k_kvm
    let scale_u = het ? r.k_scale_sw : r.k_scale
    let hs_u = het ? r.k_hs_sw : r.k_hs
    let hass_u = c.attn_sinks ? 1u : 0u
    let gd = qg_direct(c, r.kdt)
    let bq = gd ? r.bdn_qg : r.bqkv
    let qhs_u = gd ? 2u * hs_u : 0u
    let gated_u = gd && r.kdt != KVDtype.tq4 ? 1u : 0u   //! tq4 gates after its un-rotation
    if (r.attn_d) {
        encode_attn_d(t, r, enc, l, head_size, n_heads, lsl ? uint(c.sliding_window) : 0u, bsnk)
    } elif (r.chunked) {
        var pka = SqAttnPartArgs(cnt = cnt, kstride = kstride_l, nheads = uint(n_heads),
            kv_mul = kvm_u, hs = hs_u, scale = scale_u, nchunks = uint(nchunks),
            softcap = c.attn_logit_softcap, window = lsl ? uint(c.sliding_window) : 0u, qhs = qhs_u, gated = gated_u, ring = uint(mirror_ring(t, l)))
        enc_attn_part(enc, r.kdt, g_arena, g_arena, lkoff, lvoff, bq, r.bpart, pka, n_heads * nchunks, head_size)
        var cka = SqAttnCombArgs(nch = uint(nchunks), nheads = uint(n_heads), hs = hs_u,
            chlo = lsl ? r.k_chlo : 0u, hass = hass_u)
        enc_attn_comb(enc, r.bpart, r.bxb, cka, n_heads, head_size, bsnk)
    } else {
        var ska = SqAttnArgs(cnt = cnt, kstride = kstride_l, nheads = uint(n_heads),
            kv_mul = kvm_u, hs = hs_u, scale = scale_u,
            softcap = c.attn_logit_softcap, hass = hass_u, qhs = qhs_u, gated = gated_u)
        enc_attn(enc, r.kdt, g_arena, g_arena, lkoff, lvoff, bq, r.bxb, ska, n_heads, head_size, bsnk)
    }
}

//! the single row's fused gate+up dot + silu/gelu epilogue — gemma geglu rides the act uniform
[unused_argument(l)]
def private w13_fused_single(var e : SingleLayerEnc; t : Model; enc : MetalComputeEncoder?; l : int64; hid_l : int64; u_hid_l : MetalBuffer?; geglu : bool; fg : MetalBuffer?) : bool {
    let bw1 = blob_of(g_dev, t, t.w1_offs[l])
    enc_gemv_w13sw(enc, bw1.buf, bw1.boff, uint64((t.w3_offs[l] / 32l) * 34l),
        e.r.bxb, fg, e.r.u_dim, u_hid_l, t.config.dim, hid_l, geglu ? e.r.u_one32 : e.r.u_zero32)
    return true
}

//! the step's recurrent planes over its groups (`r.bdn_grp`): the single row's one, the batch's row a session, the verify's
//! group a stream
def private dn_stages_of(var r : StepRes; split : bool) : DnStages {
    return DnStages(qkv = r.bdn_qkv, cv = r.bdn_cv, b = r.bdn_b, g = r.dn_ba_rows ? r.bdn_b : r.bdn_g, o = r.bdn_o, z = r.bdn_z,
        grp = r.bdn_grp, ng = r.dn_ng, npmax = r.dn_npmax, rows = r.nrows, split = split, ba_rows = r.dn_ba_rows)
}

[arch(at="../ARCHITECTURE_GPU_MTP.md#dn-group-stages")]
def private enc_dn_ba_f32(enc : MetalComputeEncoder?; t : Model; l : int64; bx, by : MetalBuffer?; nrows : int64) {
    let dim = t.config.dim
    let nvh = t.config.ssm_dt_rank
    let pb = uint64(dim * nvh * 4l)
    unsafe {
        var bw = upload_region_cat(addr < void? >(t.fblob[t.dn_betaf_off + l * dim * nvh]), pb,
            addr < void? >(t.fblob[t.dn_alphaf_off + l * dim * nvh]), pb)
        var ka = moe_router_args(2l * nvh, dim, nrows)
        enc_moe_router(enc, bw, bx, by, g_one, ka, 2l * nvh, nrows)
    }
}

//! the step's group table from the words in `w`, released with the step
def private dn_grp_take(var r : StepRes; ng, npmax : int64) {
    var tb = dn_grp_upload()
    r.bdn_grp = tb.buf
    r.bytes_dn_grp = tb.bytes
    r.dn_ng = ng
    r.dn_npmax = npmax
}

//! the single row's deltanet layer: projections -> conv+SiLU -> q/k L2 -> delta rule -> gated
//! out-norm -> out-proj; the state and history in place, or a chained step's into the region its group names to write
def private recurrent_single(var e : SingleLayerEnc; t : Model; enc : MetalComputeEncoder?; l : int64; var dnli : int64&) {
    var r & = unsafe(*e.r)
    recurrent_layer(r, t, enc, l, dnli, true, r.dn_split)
}

//! the step's deltanet layer over its rows: the projections (the single row's GEMVs where `single`, else the rows sites), conv + SiLU,
//! q/k L2, the delta rule and the gated out-norm, the out projection; the state and history in place, or where a split step's group
//! names its region
def private recurrent_layer(var r : StepRes; t : Model; enc : MetalComputeEncoder?; l : int64; var dnli : int64&; single, split : bool) {
    if (g_skip != "gemv") {
        enc_dn_proj_rows(r, t, enc, l, single)
    }
    if (g_skip != "attn") {
        let d = dn_stages_of(r, split)
        enc_dn_front(enc, t, l, dnli, d)
        enc_dn_back(enc, t, l, dnli, d)
    }
    if (g_skip != "gemv") {
        enc_dn_out_rows(r, t, enc, l, single)
    }
    dnli++
}

// ===== the hyper-connection seams of the single row (qwen4exp) =====

//! One mixer over the wide residual: normed streams -> down -> silu(lo / hc) -> up -> the sigmoid-gated
//! stream mean into `by`; the scatter logits of the site into `binj` (a null binj = the head mixer)
def private hc_mix_step(r : StepRes; t : Model; enc : MetalComputeEncoder?; norm_off, down_off, up_off, inject_off : int64; by, binj : MetalBuffer?) {
    hc_mix_rows(r, t, enc, r.bhc, norm_off, down_off, up_off, inject_off, by, binj)
}

//! a site's `n` x `d` plane over `nr` rows of `bx` into `by`: the single row's GEMV where `single`, else the rows GEMM site
def private site_rows(enc : MetalComputeEncoder?; t : Model; single : bool; fmt : KqFmt; off, n, d : int64; bx, by, u_d, u_n : MetalBuffer?; nr : int64) {
    if (single) {
        enc_site_gemv(enc, t, fmt, off, n, d, bx, by, u_d, u_n)
    } else {
        enc_batch_site(enc, t, rows_tier(nr), fmt, off, n, d, n, bx, by, u_d, u_n, u_n, 0l, false)
    }
}

//! the rms norm of `nr` rows of `bx` under the weight row `bw` into `by`: the one-row form where `single`
def private norm_rows(enc : MetalComputeEncoder?; single : bool; bx, bw, by, u_dim, u_eps : MetalBuffer?; nr : int64) {
    if (single) {
        enc_rms(enc, bx, 0ul, bw, by, u_dim, u_eps)
    } else {
        enc_rms_b(enc, bx, 0ul, bw, by, u_dim, u_eps, nr)
    }
}

//! The mixer over `bres`'s nrows wide rows (the step's row count): the single row takes the GEMVs,
//! the MTP rows the rows GEMM site
def private hc_mix_rows(r : StepRes; t : Model; enc : MetalComputeEncoder?; bres : MetalBuffer?; norm_off, down_off, up_off, inject_off : int64; by, binj : MetalBuffer?) {
    let c = t.config
    let hcd = hc_dim(c)
    let lr = c.hc_low_rank
    let nr = r.nrows
    unsafe {
        if (g_skip != "ew") {
            var bw_n = upload_region(addr < void? >(t.fblob[norm_off]), uint64(hcd * 4l))
            enc_hc_norm(enc, bres, r.bhc_xn, bw_n, hc_norm_args(c), nr * c.hc_count)
        }
        if (g_skip != "gemv") {
            if (binj != null) {
                var bw_i = upload_region(addr < void? >(t.fblob[inject_off]), uint64(c.hc_count * hcd * 4l))
                var ika = moe_router_args(c.hc_count, hcd, nr)
                enc_moe_router(enc, bw_i, r.bhc_xn, binj, g_one, ika, c.hc_count, nr)
            }
            site_rows(enc, t, nr == 1l, KqFmt.q8, down_off, lr, hcd, r.bhc_xn, r.bhc_lo, r.u_hcd, r.u_lr, nr)
        }
        if (g_skip != "ew") {
            enc_hc_lo(enc, r.bhc_lo, 0ul, r.u_lr_tot, r.u_hc, nr * lr)
        }
        if (g_skip != "gemv") {
            site_rows(enc, t, nr == 1l, KqFmt.q8, up_off, hcd, lr, r.bhc_lo, r.bhc_gate, r.u_lr, r.u_hcd, nr)
        }
        if (g_skip != "ew") {
            enc_hc_mix(enc, r.bhc_xn, r.bhc_gate, by, hc_mix_args(c, nr), nr * c.dim)
        }
    }
}

//! Layer l's mixer at its attention (ffn = false) or ffn site - the CPU path's mix_decode
def private hc_mix_site(r : StepRes; t : Model; enc : MetalComputeEncoder?; l : int64; ffn : bool; by, binj : MetalBuffer?) {
    let site = l * 2l + (ffn ? 1l : 0l)
    let hcd = hc_dim(t.config)
    hc_mix_step(r, t, enc, t.hc_norm_off + site * hcd, t.hc_down_offs[site], t.hc_up_offs[site],
        t.hc_inject_off + site * hcd * t.config.hc_count, by, binj)
}

//! A block's output scattered into the streams under the site's logits
def private hc_combine_step(r : StepRes; t : Model; enc : MetalComputeEncoder?; bout, binj : MetalBuffer?) {
    if (g_skip != "ew") {
        enc_hc_combine(enc, r.bhc, bout, binj, hc_comb_args(t.config, r.nrows), r.nrows * hc_dim(t.config))
    }
}

//! The draft head's prelude over nrows wide carries in `bhc_carry`: per stream, [enorm(embed) ; hnorm_ch(h_ch)]
//! projects to the draft's wide residual stream - hc rows of one eh_proj GEMM per position land in bhc.
//! `embed` holds the rows' raw embed rows; the normed embed rows pass through bh12
def private hc_draft_prelude(t : Model; var r : StepRes; enc : MetalComputeEncoder?; embed : MetalBuffer?; nr : int64) {
    let c = t.config
    let dim = c.dim
    let nh = c.hc_count
    let rowb = uint64(dim * 4l)
    unsafe {
        var bw_en = upload_region(addr < void? >(t.fblob[t.mtp_enorm_off]), rowb)
        enc_rms_b(enc, embed, 0ul, bw_en, r.bh12, r.u_dim, r.u_eps, nr)
        var bw_hn = upload_region(addr < void? >(t.fblob[t.mtp_hnorm_off]), r.bytes_hcd)
        enc_hc_norm(enc, r.bhc_carry, r.bhc_xn, bw_hn, hc_norm_args(c), nr * nh)
        enc_cat2(enc, r.bhc_cat, r.bh12, r.bhc_xn, cat2_args(dim, nr * nh, nh), nr * nh * dim)
        enc_batch_site(enc, t, rows_tier(nr * nh), t.mtp_ehproj_fmt, t.mtp_ehproj_off, dim, 2l * dim, dim,
            r.bhc_cat, r.bhc, r.u_dim2, r.u_dim, r.u_dim, 0l, false)
    }
}

//! The n-gram side input before layer ple_layer's attention mixer: the key/value projections of the poked heads, then the
//! gate, ring write, dilated conv and residual add - the prefill window's (`enc_ple_side_rows`); the projections stay the
//! step's GEMVs where the window has its GEMM, a shape split. A single row writes its own ring, a verify's rows each stream's
def private ple_side_step(r : StepRes; t : Model; enc : MetalComputeEncoder?) {
    let c = t.config
    let hcd = hc_dim(c)
    let ind = ple_in_dim(c)
    let nr = r.nrows
    if (g_skip != "gemv") {
        if (nr == 1l) {
            enc_site_gemv(enc, t, KqFmt.q8, t.ple_key_off, hcd, ind, r.bple_emb, r.bple_key, r.u_ple_in, r.u_hcd)
            enc_site_gemv(enc, t, KqFmt.q8, t.ple_val_off, c.dim, ind, r.bple_emb, r.bple_val, r.u_ple_in, r.u_dim)
        } else {
            enc_batch_site(enc, t, rows_tier(nr), KqFmt.q8, t.ple_key_off, hcd, ind, hcd, r.bple_emb, r.bple_key, r.u_ple_in, r.u_hcd, r.u_hcd, 0l, false)
            enc_batch_site(enc, t, rows_tier(nr), KqFmt.q8, t.ple_val_off, c.dim, ind, c.dim, r.bple_emb, r.bple_val, r.u_ple_in, r.u_dim, r.u_dim, 0l, false)
        }
    }
    if (g_skip != "ew") {
        if (nr == 1l) {
            enc_ple_side_rows(enc, t, r.bhc, 0ul, r.bple_key, 0ul, r.bple_val, 0ul, r.bple_gated, 0ul, r.bdn_state, r.ple_ring_base,
                r.bdn_state, r.ple_ring_base, r.chain_pos, 1l, false)
        } elif (r.batch_rows) {   // one row a session: each writes its own mirror's ring in place, as the single row does
            for (i in range64(nr)) {
                let rb = uint64(i) * r.bytes_hcd
                let pos_i = int64(g_b_rte[i * 4l + 3l]) - 1l   // the route table's cnt is the row's position + 1
                enc_ple_side_rows(enc, t, r.bhc, rb, r.bple_key, rb, r.bple_val, uint64(i) * uint64(c.dim * 4l), r.bple_gated, rb,
                    dn_pool_buffer(), g_b_ple_bases[i], dn_pool_buffer(), g_b_ple_bases[i], pos_i, 1l, false)
            }
        } else {
            for (g in g_v_groups) {   // the panel form: slot p aliases row p-1's oldest tap, so no row lands in the ring before every conv read (commit_ple_rows)
                let rb = uint64(g.row0) * r.bytes_hcd
                enc_ple_side_rows(enc, t, r.bhc, rb, r.bple_key, rb, r.bple_val, uint64(g.row0) * uint64(c.dim * 4l), r.bple_gated, rb,
                    r.bple_cin, rb, dn_pool_buffer(), g.ple_base, g.pos, g.nr, true)
            }
        }
    }
}

def private mtp_ple_rows_gather(t : Model; var s : Session; nr, pos : int64) {
    mtp_ple_window_snapshot(s)
    forward_prefill_alloc(t, s, nr, pos)
    invoke(t.blocks.pre_stack_prefill, t, s, s.mtp_vbatch, nr, pos)
    mtp_ple_window_restore(s)
}

//! The round committed `n` rows of `mtp_vbatch` at `pos`: the window advances over them the way the
//! CPU prefill's pre-stack hook advances it (the gather it repeats is a handful of table rows)
def private mtp_ple_window_commit(t : Model; var s : Session; n, pos : int64) {
    forward_prefill_alloc(t, s, n, pos)
    invoke(t.blocks.pre_stack_prefill, t, s, s.mtp_vbatch, n, pos)
}

//! A group's accepted rows' conv inputs land in its ring after the walk - row p at slot (pos + p) % rows -
//! so a rejected row never clobbers a slot a later conv reads
def private commit_ple_rows(t : Model; var r : StepRes; gi, n : int64) {
    let c = t.config
    let hcd = hc_dim(c)
    let rows = ple_hist_rows(c)
    var g & = unsafe(g_v_groups[gi])
    var cb = metal_new_command_buffer_unretained(g_queue)
    var enc = metal_new_compute_encoder(cb)
    for (p in range64(n)) {
        enc_copy_row(enc, r.bple_cin, uint64(g.row0 + p) * r.bytes_hcd, dn_pool_buffer(), g.ple_base + uint64(((g.pos + p) % rows) * hcd * 4l), r.u_hcd, hcd)
    }
    metal_end_encoding(enc)
    metal_release(enc)
    metal_commit(cb)
    metal_wait_until_completed(cb)
    metal_release(cb)
}

[ |> template_struct_instance]
struct template LayerEncT {
    r : StepRes?                          // the step's state - the stamp's driver owns it
    @template_constant SINGLE : bool = false   // the single row alone serves the fused QKV form
    @template_call site = @@site_single
    @template_call ple = @@ple_single
    @template_call total = @@total_single
    @template_call residual = @@residual_single
    @template_call rms = @@rms_single
    @template_call add = @@add_single
    @template_call moe_args = @@moe_args_single
    @template_call rope_store = @@rope_store_single
    @template_call attention = @@attention_single
    @template_call recurrent = @@recurrent_single
    @template_call w13_fused = @@w13_fused_single
    @template_call panels = @@panels_single
    @template_call moe_g4_step = @@moe_g4_step_single

    //! one layer's dispatch chain; the phases are hz-barrier coupled
    def encode(t : Model; enc : MetalComputeEncoder?; l : int64; var dnli : int64&) {   // nolint:STYLE037,STYLE038 — one layer's dispatch chain
        let c = t.config
        let dim = c.dim
        let n_heads = c.n_heads
        let hid_l = layer_hidden(t, l)   // per-layer (MatFormer E-series)
        let u_hid_l = (r.u_hidden2 == null || hid_l == r.hid0) ? r.u_hidden : r.u_hidden2
        let pos = r.chain_pos
        let cap = r.cap
        let f16v = r.kdt == KVDtype.f16
        let scalar_kv = r.kdt == KVDtype.f16 || r.kdt == KVDtype.f32   // block codecs take their own kernels
        let geglu = c.ffn_act != FfnAct.silu     // gemma: gelu gate — the swiglu-fused forms stand down
        let nchunks = (pos + r.nrows + ATTN_CHUNK_ROWS - 1l) / ATTN_CHUNK_ROWS
        unsafe {
            let recr = layer_is_recurrent(c, l)
            if (r.hc) {   // the hyper-connection chain: the side input (at its layer) and the attention site's mixer open the layer into bxb
                if (r.png && l == c.ple_layer) {
                    ple_side_step(*r, t, enc)
                }
                hc_mix_site(*r, t, enc, l, false, r.bxb, r.bhc_inj_a)
            }
            // attention: fused QKV GEMV -> single-query attention -> out-proj -> fused add+norm; K-quant sites (mixed PER TENSOR formats) split into per-tensor plane GEMVs
            let fq = fmt_at(t.wq_fmt, l)
            let fk = fmt_at(t.wk_fmt, l)
            let fv = fmt_at(t.wv_fmt, l)
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
            // the same per-layer selection on the kargs scalars, chosen ONCE beside the buffer twins
            let k_hs_l = het ? r.k_hs_sw : r.k_hs
            let k_qd_l = het ? r.k_qd_sw : r.k_qd
            let k_qkvd_l = het ? r.k_qkvd_sw : r.k_qkvd
            let k_voffe_l = het ? r.k_voffe_sw : r.k_voffe
            // E-series shared-KV layer: Q-only — lkoff/lvoff alias the source slab via kv_row_prefix
            let kvsh = kv_shared_layer(t, l)
            // qk_norm needs the head-wide prepass between GEMV and rope — the fused form stands down (hetero too: its row map bakes one class geometry); q_gated packs [q|gate] (deint first)
            let fused_qkv = SINGLE && scalar_kv && qkv_q8 && !c.qk_norm && !r.hetero && !c.q_gated && !kvsh
            if (recr) {
                recurrent(self, t, enc, l, dnli)
            }
            // the [bq | bk | bv] bias plane matches the fused QKV index space, added pre-rope; no bias -> the qkv buffer dummy-binds the slot
            var bbias = r.bqkv
            if (c.attn_qkv_bias) {
                bbias = upload_region_cat(
                    addr < void? >(t.bq[l * qd]), uint64(qd * 4l),
                    addr < void? >(t.bk[l * kv_dim]), uint64(kv_dim * 4l),
                    addr < void? >(t.bv[l * kv_dim]), uint64(kv_dim * 4l))
            }
            if (!recr && g_skip != "gemv" && g_skip != "qkv") {   // qkv knockout: the attention input GEMVs only
                if (fused_qkv && t.wv_offs[l] >= 0l) {
                    // one dispatch over all three segments — the kernel binds each tensor's blob slice separately (kind-major layout keeps q/k/v apart)
                    let bwq_ = blob_of(g_dev, t, t.wq_offs[l])
                    var fka = Q8QkvRsArgs(ndim = r.k_dim, qd = r.k_qd, kvd = r.k_kvd,
                        hs = r.k_hs, neox = r.k_neox, has_bias = r.k_hasb)
                    enc_gemv_qkv_rs(enc, f16v, bwq_.buf, bwq_.boff,
                        uint64((t.wk_offs[l] / 32l) * 34l), uint64((t.wv_offs[l] / 32l) * 34l),
                        r.bxb, r.bqkv, g_arena, g_arena,
                        lkoff + uint64(mirror_row_at(t, r.kdt, l, pos)), lvoff + uint64(mirror_row_at(t, r.kdt, l, pos)),
                        bcos_l, bsin_l, bbias, fka, qkvd)
                } else {
                    if (c.q_gated) {
                        // 2x-wide packed [q|gate] projection; deinterleaved into the qkv q segment + gate rows only off the direct rail
                        site(self, t, enc, SiteKind.qkv, fq, t.wq_offs[l], 2l * qd, dim, 2l * qd, r.bxb, r.bdn_qg, r.u_dim, r.u_dn_qd2, r.u_dn_qd2, 0l)
                        if (!qg_direct(c, r.kdt)) {
                            enc_dn_deint(enc, r.bdn_qg, r.bqkv, r.bdn_gate,
                                DnDeintArgs(hs = k_hs_l, qd = k_qd_l, qs = uint(qkvd)), qd, r.nrows)
                        }
                    } else {
                        site(self, t, enc, SiteKind.qkv, fq, t.wq_offs[l], qd, dim, qkvd, r.bxb, r.bqkv, r.u_dim, u_qd_l, u_qkvd_l, 0l)
                    }
                    if (!kvsh) {
                        site(self, t, enc, SiteKind.qkv, fk, t.wk_offs[l], kv_dim, dim, qkvd, r.bxb, r.bqkv, r.u_dim, u_kvd_l, u_qkvd_l, qd)
                        if (t.wv_offs[l] >= 0l) {
                            site(self, t, enc, SiteKind.qkv, fv, t.wv_offs[l], kv_dim, dim, qkvd, r.bxb, r.bqkv, r.u_dim, u_kvd_l, u_qkvd_l, qd + kv_dim)
                        }
                    }
                }
            }
            if (!recr && !kvsh && g_skip != "ew" && (t.wv_offs[l] < 0l || c.v_norm)) {
                // gemma4 V: no-wv layers fill the v segment from the PRE-norm/PRE-rope K projection (the CPU mm_qkv order, BEFORE qk_norm touches k); with v_norm the copy FUSES into the ones-RMS
                if (t.wv_offs[l] < 0l && !c.v_norm) {
                    for (i in range64(r.nrows)) {
                        enc_copy_row(enc, r.bqkv, uint64((i * qkvd + qd) * 4l), r.bqkv, uint64((i * qkvd + qd + kv_dim) * 4l), u_kvd_l, kv_dim)
                    }
                }
                if (c.v_norm && !qkn_fused(c, r.kdt) && !c.qk_norm) {   //! with q/k norm the V arms ride the q/k norm's dispatch below
                    // MAX-width upload: the region cache is address-keyed and both classes read this one plane — a narrower first upload starved the hs-512 class (Metal OOB reads are ZEROS)
                    var bones = upload_region(addr < void? >(t.fblob[t.rms_ones_off]), uint64(max_head_size(c) * 4l))
                    var vka = qk_norm_args(0u, uint(kv_dim / head_size), k_hs_l, k_qkvd_l, t.wv_offs[l] < 0l ? k_qd_l : k_voffe_l, k_voffe_l, 0u, r.k_eps)
                    enc_qk_norm(enc, r.bqkv, r.bqkv, bones, vka, kv_dim / head_size, r.nrows)
                }
            }
            if (!recr && c.qk_norm && g_skip != "ew" && !qkn_fused(c, r.kdt)) {
                var bqkw = upload_region_cat(
                    addr < void? >(t.fblob[t.rms_q_offs[l]]), uint64(head_size * 4l),
                    addr < void? >(t.fblob[t.rms_k_offs[l]]), uint64(head_size * 4l))
                let nkn = kvsh ? 0l : kv_dim / head_size
                var qka = qk_norm_args(r.k_nh, uint(nkn), k_hs_l, k_qkvd_l, k_qd_l, k_qd_l, k_hs_l, r.k_eps)
                var bqn = r.bqkv
                if (qg_direct(c, r.kdt)) {   //! the q heads norm in their [q | gate] rows
                    bqn = r.bdn_qg
                    qka.qstride = uint(2l * qd)
                    qka.qbase = 0u
                    qka.qhs = 2u * k_hs_l
                }
                var nvn = 0l
                if (c.v_norm && !kvsh) {   //! V off the pre-norm K (no v plane) or normed in place, weightless: the same dispatch
                    let nowv = t.wv_offs[l] < 0l
                    qka.vfk = nowv ? 1u : 0u
                    qka.vnorm = nowv ? 0u : 1u
                    qka.vbase = k_voffe_l
                    nvn = nowv ? 0l : nkn
                }
                enc_qk_norm(enc, bqn, r.bqkv, bqkw, qka, n_heads + nkn + nvn, r.nrows)
            }
            if (!recr && g_skip != "ew" && !fused_qkv) {
                rope_store(self, t, enc, l, het, kvsh, bbias, bcos_l, bsin_l, lkoff, lvoff, rowb, qd, kv_dim, head_size, n_heads)
            }
            if (!recr && g_skip != "attn") {
                // sliding only surfaces chunked (the gate pins window >= 128 >= the single-form ceiling)
                var bsnk : MetalBuffer?   // null = the enc defaults bind the identity
                if (c.attn_sinks) {
                    bsnk = upload_region(addr < void? >(t.fblob[t.sinks_off + l * n_heads]), uint64(n_heads * 4l))
                }
                r.xh_ok = false   //! the attention output lands in bxb with no twin
                attention(self, t, enc, l, lsl, het, bsnk, lkoff, lvoff, nchunks, head_size, n_heads)
                if (r.kdt == KVDtype.tq4) {
                    // the attention out is rotated-basis per head — un-rotate before the wo projection, a gated head's gate after it
                    let gd = qg_direct(c, r.kdt)
                    enc_fwht_unsign(enc, r.bxb, u_hs_l, u_qd_l, n_heads, r.nrows, head_size, gd ? r.bdn_qg : null, gd ? r.u_dn_qd2 : null)
                }
            }
            if (!recr && c.q_gated && !qg_direct(c, r.kdt) && g_skip != "ew") {
                // gated-attention epilogue: out ⊙ σ(gate) between the attention output and wo (the CPU order)
                let tq = total(self, u_qd_l, qd)
                enc_sigmul(enc, r.bxb, 0ul, r.bdn_gate, 0ul, tq.b, tq.n)
            }
            if (!recr && g_skip != "gemv" && g_skip != "wo") {   // recurrent layers have no wo — ssm_out (already in bxb2) is their out projection
                site(self, t, enc, SiteKind.wo, fmt_at(t.wo_fmt, l), t.wo_offs[l], dim, qd, dim, r.bxb, r.bxb2, u_qd_l, r.u_dim, r.u_dim, 0l)
                if (c.attn_out_bias) {
                    var bwo_b = upload_region(addr < void? >(t.bo[l * dim]), uint64(dim * 4l))
                    add(self, enc, r.bxb2, 0ul, bwo_b, 0ul, dim, null)
                }
            }
            // ffn: [post-attn norm ->] fused add+norm -> W1|W3 -> swiglu/geglu -> W2 [-> post-ffn norm] -> fused add + next attn norm (pre_post_norm norms IN PLACE — the CPU order)
            if (r.hc) {   // the attention output scatters into the streams; the ffn site's mixer makes the next block input
                hc_combine_step(*r, t, enc, r.bxb2, r.bhc_inj_a)
                hc_mix_site(*r, t, enc, l, true, r.bxb, r.bhc_inj_f)
            } elif (g_skip != "ew") {   //! the attention branch under its post norm (the sandwich), the add, the FFN norm: one step
                residual(self, t, enc, r.bx, r.bxb2, r.bxb, seam(c.pre_post_norm ? t.rms_post_att_off + l * dim : -1l, t.rms_ffn_off + l * dim))
            }
            let moe_with_dense = r.moe && c.moe_dense_shexp
            let g4_step = moe_with_dense && moe_g4_step_serves(t, l, r.ple || r.hc)
            let knfe = r.moe ? c.n_expert_used * c.n_ff_exp : 0l
            var pan = panels(self, moe_with_dense, hid_l, knfe)
            if (r.moe) {
                encode_moe(t, enc, l, pan, moe_with_dense, g4_step, knfe)
            }
            if (!r.moe || moe_with_dense) {
                dense_ffn(t, enc, l, pan, moe_with_dense, geglu, hid_l, u_hid_l)
            }
            if (g4_step) {   //! the routed sum, both branches' norms, the residual and the next norm: one step
                moe_g4_step(self, t, enc, l)
                return
            }
            if (moe_with_dense && g_skip != "ew") {
                                var bw_post_dense = upload_region(addr < void? >(t.fblob[t.rms_post_ffn1_off + l * dim]), uint64(dim * 4l))
                rms(self, enc, r.bxb, 0ul, bw_post_dense, r.bxb)
                add(self, enc, r.bxb, 0ul, r.bmoe_rt, 0ul, dim, null)
            }
            let post_ffn = c.pre_post_norm ? t.rms_post_ffn_off + l * dim : -1l   //! the FFN branch's post norm (the sandwich)
            if (r.hc) {
                if (post_ffn >= 0l && g_skip != "ew") {
                    rms(self, enc, r.bxb, 0ul, upload_region(addr < void? >(t.fblob[post_ffn]), uint64(dim * 4l)), r.bxb)
                }
                hc_combine_step(*r, t, enc, r.bxb, r.bhc_inj_f)
                return
            }
            //! the layer's last seam: the branch under its post norm, the add under the layer's output scale (gemma-4), the next
            //! layer's attention norm - the per-layer embedding stands between the FFN's add and that seam, its own branch's post norm
            //! in the seam's place
            let next_norm = l + 1l < c.n_layers ? t.rms_att_off + (l + 1l) * dim : -1l
            let osc = c.layer_out_scale ? t.out_scale_off + l : -1l
            if (r.ple) {
                if (g_skip != "ew") {
                    residual(self, t, enc, r.bx, r.bxb, null, seam(post_ffn, -1l))
                }
                ple(self, t, enc, l)
                if (g_skip != "ew") {
                    residual(self, t, enc, r.bx, r.bxb, r.bxb, seam(t.ple_post_off + l * dim, next_norm, osc))
                }
            } elif (g_skip != "ew") {
                residual(self, t, enc, r.bx, r.bxb, r.bxb, seam(post_ffn, next_norm, osc))
            }
        }
    }

    //! the dense FFN: W1|W3 into the family's gate / up panels (fused where the family has the
    //! form) -> swiglu/geglu -> W2 into bxb; gemma4's shared branch runs it beside the experts
    def dense_ffn(t : Model; enc : MetalComputeEncoder?; l : int64; var pan : FfnPanels; moe_with_dense, geglu : bool; hid_l : int64; u_hid_l : MetalBuffer?) {
        let c = t.config
        let dim = c.dim
        let f1 = fmt_at(t.w1_fmt, l)
        let f3 = fmt_at(t.w3_fmt, l)
        let w13_q8 = f1 == KqFmt.q8 && f3 == KqFmt.q8
        let th = total(self, u_hid_l, hid_l)
        let sites_on = g_skip != "gemv" && !(moe_with_dense && g_skip == "moe_sh")
        let fused13 = w13_q8 && sites_on && w13_fused(self, t, enc, l, hid_l, u_hid_l, geglu, pan.fg)
        if (sites_on && !fused13) {
            site(self, t, enc, SiteKind.w13, f1, t.w1_offs[l], hid_l, dim, hid_l, r.bxb, pan.fg, r.u_dim, u_hid_l, u_hid_l, 0l)
            site(self, t, enc, SiteKind.w13, f3, t.w3_offs[l], hid_l, dim, hid_l, r.bxb, pan.fu, r.u_dim, u_hid_l, u_hid_l, pan.fu_col)
        }
        if (g_skip != "ew" && !fused13) {
            let twin = r.bfgh != null && pan.fg == r.bh12   //! the activation dual-stores the gate panel's f16 twin for the w2 site
            if (geglu) {
                if (twin) {
                    enc_geglu_hx(enc, pan.fg, 0ul, pan.fu, uint64(pan.fu_col * 4l), th.b, r.bfgh, th.n)
                } else {
                    enc_geglu(enc, pan.fg, 0ul, pan.fu, uint64(pan.fu_col * 4l), th.b, th.n)
                }
            } elif (twin) {
                enc_swiglu_hx(enc, pan.fg, 0ul, pan.fu, uint64(pan.fu_col * 4l), th.b, r.bfgh, th.n)
            } else {
                enc_swiglu(enc, pan.fg, 0ul, pan.fu, uint64(pan.fu_col * 4l), th.b, th.n)
            }
            r.fgh_ok = twin
        }
        if (sites_on) {
            site(self, t, enc, SiteKind.w2, fmt_at(t.w2_fmt, l), t.w2_offs[l], dim, hid_l, dim, pan.fg, r.bxb, u_hid_l, r.u_dim, r.u_dim, 0l)
        }
        r.fgh_ok = false   //! the gate panel is spent; w2 wrote bxb with no twin
        r.xh_ok = false
    }

    //! the routed FFN: router -> select -> experts -> combine; moe_with_dense = gemma4's parallel-dense form
    def encode_moe(t : Model; enc : MetalComputeEncoder?; l : int64; var pan : FfnPanels; moe_with_dense, g4_step : bool; knfe : int64) {
        let c = t.config
        let dim = c.dim
        let nr = r.nrows
        let tk = total(self, r.u_moe_knfe, knfe)
        let mx4_hasb = c.moe_exps_bias ? 1u : 0u
        let shg = r.u_moe_nsh != null   //! the shared expert's gate logit rides the router as its row ne
        let ner = c.n_expert + (shg ? 1l : 0l)
        unsafe {
            var bmx = r.bxb   // expert input: plain = the rms_ffn norm-out; moe_with_dense = pre_ffn2
            if (moe_with_dense && g_skip != "ew") {
                var bw_p2 = upload_region(addr < void? >(t.fblob[t.rms_pre_ffn2_off + l * dim]), uint64(dim * 4l))
                rms(self, enc, r.bx, 0ul, bw_p2, pan.moe_in)
                bmx = pan.moe_in
            }
            if (g_skip != "gemv") {
                let rows_bytes = uint64(c.n_expert * dim * 4l)
                var bw_rt = (shg ? upload_region_cat(addr < void? >(t.fblob[t.router_off + l * c.n_expert * dim]), rows_bytes,
                        addr < void? >(t.fblob[t.shexp_gate_off + l * dim]), uint64(dim * 4l))
                    : upload_region(addr < void? >(t.fblob[t.router_off + l * c.n_expert * dim]), rows_bytes))
                var brb : MetalBuffer? = g_one
                if (c.moe_router_bias) {
                    brb = upload_region(addr < void? >(t.fblob[t.router_b_off + l * c.n_expert]), uint64(c.n_expert * 4l))
                }
                var rka = moe_router_args(ner, dim, nr, 0l, c.moe_router_bias)
                if (moe_with_dense) {
                    var bw_rs = upload_region(addr < void? >(t.fblob[t.moe_router_scale_off + l * dim]), uint64(dim * 4l))
                    enc_g4_router_norm(enc, r.bx, bw_rs, r.bmoe_rt, r.u_dim, r.u_eps, r.u_moe_invd, nr)
                    enc_moe_router(enc, bw_rt, r.bmoe_rt, r.bmoe_lg, brb, rka, ner, nr)
                } else {
                    enc_moe_router(enc, bw_rt, r.bxb, r.bmoe_lg, brb, rka, ner, nr)
                }
                var bw_ds : MetalBuffer?
                if (moe_with_dense) {
                    bw_ds = upload_region(addr < void? >(t.fblob[t.moe_down_scale_off + l * c.n_expert]), uint64(c.n_expert * 4l))
                }
                enc_moe_select_rows(enc, c, r.bmoe_lg, r.bmoe_sel, nr, ner, bw_ds)
                if (g_skip != "moe_rt") {   // moe_rt knockout: the routed expert GEMVs only (router/select stay live)
                    enc_moe_gate_up(enc, t, l, nr, bmx, pan.mg, pan.mu, r.bmoe_sel, uint64(pan.mu_col * 4l),
                        moe_args(self, r.k_dim, r.k_moe_nfe, 0u, 0u, mx4_hasb, false))
                }
            }
            if (g_skip != "ew") {
                enc_moe_act(enc, c, pan.mg, pan.mu, uint64(pan.mu_col * 4l), tk.b, tk.n)
            }
            if (g_skip != "gemv") {
                if (g_skip != "moe_rt") {
                    enc_moe_down(enc, t, l, nr, pan.mg, r.bmoe_dn, r.bmoe_sel, moe_args(self, r.k_moe_nfe, r.k_dim, 0u, r.k_moe_xs_dn, mx4_hasb, true))
                }
                if (r.u_moe_nsh != null && g_skip != "moe_sh") {
                    // shexp reads bxb BEFORE combine overwrites it; own bh12s panel = schedulable under the routed chain
                    enc_shexp(*r, t, enc, l, SINGLE)   //! the single-row driver's form owns bh12s; a rows driver at one row keeps its rows form
                }
                if (shg && g_skip != "ew") {   //! the routed sum and the shared expert's rows under their gate's sigmoid, one pass
                    enc_moe_combine_sh(enc, r.bxb, r.bmoe_sel, r.bmoe_dn, r.bxb2, r.bmoe_lg, moe_comb_args(dim, c.n_expert_used, ner, c.n_expert), dim, nr)
                } elif (!g4_step) {   //! the gemma-4 step sums the routed rows itself
                    enc_moe_combine(enc, moe_with_dense ? r.bmoe_rt : r.bxb, r.bmoe_sel, r.bmoe_dn, moe_comb_args(dim, c.n_expert_used), dim, nr)
                }
            }
            if (!moe_with_dense) {
                r.xh_ok = false   //! the routed sum (and the shared expert's rows in bh12) landed with no twin
                r.fgh_ok = false
            }
            if (moe_with_dense && !g4_step && g_skip != "ew") {
                                var bw_post_routed = upload_region(addr < void? >(t.fblob[t.rms_post_ffn2_off + l * dim]), uint64(dim * 4l))
                rms(self, enc, r.bmoe_rt, 0ul, bw_post_routed, r.bmoe_rt)
            }
        }
    }

}

//! `nrows` rows through a per-layer gate (dim -> ple) or projection (ple -> dim) plane off the file's f32 rows: the router GEMV
//! (`GkRouterGemv`, `enc_f32_rows_gemv`'s word), a column a row
def private enc_ple_f32(enc : MetalComputeEncoder?; t : Model; pp : PlePlane; bx, by : MetalBuffer?; nrows : int64) {
    var bw = upload_region(unsafe(addr < void? >(t.fblob[pp.off])), uint64(pp.n * pp.d * 4l))
    var ka = moe_router_args(pp.d, pp.n, nrows)
    enc_f32_rows_gemv(enc, bw, bx, by, g_one, ka, pp.d, nrows, false)
}

//! layer l's PLE branch over `nrows` rows between the FFN residual and the layer's last seam: proj·(gelu(gate·x) ⊙ side[l]) into
//! bxb, a plane in its own format through the stamp's weight site (`site`: the plane, x, y and their two widths' uniforms), the
//! file's f32 rows on the router GEMV; the side multiply over the layer's block of the layer-major side plane
def private ple_branch(r : StepRes; t : Model; enc : MetalComputeEncoder?; l, nrows : int64;
                       site : block<(pp : PlePlane; bx, by, bn, bd : MetalBuffer?) : void>) {
    let ple = t.config.n_embd_per_layer
    let pg = ple_plane(t, l, false)
    let pp = ple_plane(t, l, true)
    if (g_skip != "gemv") {
        if (pg.f32) {
            enc_ple_f32(enc, t, pg, r.bx, r.bh12, nrows)
        } else {
            invoke(site, pg, r.bx, r.bh12, r.u_dim, r.u_ple)
        }
    }
    if (g_skip != "ew") {
        var ka = GkPleActArgs(nelem = uint(nrows * ple), sbase = uint(l * ple), sstride = uint(t.config.n_layers * ple), slen = uint(ple))
        enc_geglu_side(enc, r.bh12, r.bple, ka, nrows * ple)
    }
    if (g_skip != "gemv") {
        if (pp.f32) {
            enc_ple_f32(enc, t, pp, r.bh12, r.bxb, nrows)
        } else {
            invoke(site, pp, r.bh12, r.bxb, r.u_ple, r.u_dim)
        }
    }
}

//! the single row's PLE branch: the planes on the one-row GEMV
def private ple_single(var e : auto(ET); t : Model; enc : MetalComputeEncoder?; l : int64) {
    ple_branch(*e.r, t, enc, l, 1l) $(pp; bx, by, bn, bd) {
        enc_site_gemv(enc, t, pp.f, pp.off, pp.d, pp.n, bx, by, bn, bd)
    }
}

//! the rows' PLE branch: the planes as row sites
def private ple_rows(var e : BatchLayerEnc; t : Model; enc : MetalComputeEncoder?; l : int64) {
    ple_branch(*e.r, t, enc, l, e.r.nrows) $(pp; bx, by, bn, bd) {
        site_batch(e, t, enc, SiteKind.other, pp.f, pp.off, pp.d, pp.n, pp.d, bx, by, bn, bd, bd, 0l)
    }
}

//! the single-row step's layer encoder
struct private SingleLayerEnc : LayerEncT {
    override SINGLE = true
}

// one trunk-shaped fused layer (attention/deltanet + ffn/moe) — the decode step's loop body,
// shared with the MTP draft arm (which runs it at l == n_layers); advances the recurrent cursor
[arch(at="../ARCHITECTURE_GPU_MTP_DECODE.md#metal-layer-enc")]
def private encode_layer(t : Model; var r : StepRes; enc : MetalComputeEncoder?; l : int64; var dnli : int64&) {
    var e = SingleLayerEnc(r = unsafe(addr(r)))
    e->encode(t, enc, l, dnli)
}

def private encode_cls_tail(t : Model; var r : StepRes; enc : MetalComputeEncoder?; norm_off : int64;
                            hc_norm_off : int64 = -1l; hc_down_off : int64 = -1l; hc_up_off : int64 = -1l) {
    let c = t.config
    var bcls = r.blog_zero_copy != null ? r.blog_zero_copy : r.blog
    enc_cls_tail_rows(t, r, enc, norm_off, bcls, 1l, hc_norm_off, hc_down_off, hc_up_off, true)
    enc_sel_tail(t, r, enc, bcls, 1l)
    if (r.blog_zero_copy != null && r.blog != null) {
        // spec-capable step: mirror zero-copy logits into pooled staging — the next step's speculative argmax reads that, never session memory
        enc_copy_row(enc, r.blog_zero_copy, 0ul, r.blog, 0ul, r.u_vocab, c.vocab_size)
    }
}

//! the caller pokes bx = the raw embed row, bxb2 = mtp_h, bcos/bsin = the row's rope; `with_cls` off (the seam row) skips the head norm and classifier
def private encode_draft_body(t : Model; var r : StepRes; enc : MetalComputeEncoder?; with_cls : bool) {
    let c = t.config
    enc_draft_head_rows(t, r, enc, r.bx, 1l, true)   //! a hyper-connection carry sits poked in bhc_carry, the prelude landing the wide residual in bhc
    var dn0 = 0l
    encode_layer(t, r, enc, c.n_layers, dn0)
    if (with_cls) {
        // the head's own h_nextn is its wide residual BEFORE the head mixer: kept in bhc_save for the landing
        if (r.hc) {
            enc_copy_row(enc, r.bhc, 0ul, r.bhc_save, 0ul, r.u_hcd, hc_dim(c))
        }
        encode_cls_tail(t, r, enc, t.mtp_headnorm_off >= 0l ? t.mtp_headnorm_off : t.rms_final_off,
            t.mtp_hc_head_norm_off, t.mtp_hc_head_down_off, t.mtp_hc_head_up_off)
    }
}

def private encode_draft_step(t : Model; var r : StepRes; with_cls : bool = true) {
    var cb = metal_new_command_buffer_unretained(g_queue)
    let conc = decode_concurrent() && empty(g_skip)
    var enc = conc ? metal_new_compute_encoder_concurrent(cb) : metal_new_compute_encoder(cb)
    gr_arm(conc)
    let hz_d0 = conc ? metal_dispatch_calls() : 0l
    let hz_g0 = g_hz_gated
    encode_draft_body(t, r, enc, with_cls)
    if (conc) {
        graph_flush_sched(enc)
        hz_coverage_check(metal_dispatch_calls() - hz_d0, g_hz_gated - hz_g0)
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
    let ok = !metal_command_buffer_failed(r.cb)
    g_msel_landed = ok ? r.sel_rows : 0l
    if (ok) {
        unsafe {
            land_draft_row(t, s, r.pos, r.cap, r.koff, r.voff, reinterpret<uint8?>(metal_buffer_contents(g_arena)),
                seam ? null : metal_buffer_contents(r.hc ? r.bhc_save : r.bxf),
                (seam || r.blog_zero_copy != null) ? null : metal_buffer_contents(r.blog))
        }
    } else {
        to_log(LOG_ERROR, "dasLLAMA metal MTP draft: dispatch failed ({cb_error_text(r.cb)})\n")   // nolint:PERF028,PERF026 — error path on a failed draft dispatch
        decline(MetalDecodeDecline.gpu_error)
    }
    return ok
}

//! the draft step at head row `pos - 1` opened and poked, the mirror held through `wm_pos` (the trunk position where none is
//! given); a step whose `gpu_logits` is off declined, the caller releasing it either way
def private metal_draft_step_open(t : Model; var s : Session; token, pos, wm_pos : int64) : StepRes {
    let c = t.config
    var none : StepRes
    return none if (g_dev == null || !t.metal_blob || c.n_layer_nextn <= 0l || pos < 1l)
    let ts_prep = prof_ticks()
    finish_pending_step()
    discard_batch_pre()   // the round advances the rows a pre-encoded batch step was built for
    let dpos = pos - 1l
    let need_wm = wm_pos >= 0l ? wm_pos : pos
    if (!key_exists(g_mirrors, s.uid) || g_mirrors[s.uid].watermark < need_wm) {
        return none if (!mirror_prepare(t, s, need_wm).ok)
    }
    let mr = g_mirrors[s.uid]
    return none if (mr.cap_rows < pos || mr.kdt != s.kv_dtype_k || mr.vdt != s.kv_dtype_v)
    var r = acquire_step(t, s, dpos, mr.koff, mr.voff, mr.cap_rows, [draft = true])
    return r if (!r.gpu_logits)
    mtp_draft_prep(t, s, token, dpos)
    unsafe {
        memcpy(metal_buffer_contents(r.bx), addr < void? >(s.x[0]), r.bytes_x)
        if (r.hc) {   // the carry is the wide residual row
            memcpy(metal_buffer_contents(r.bhc_carry), addr < void? >(s.mtp_h[0]), r.bytes_hcd)
        } else {
            memcpy(metal_buffer_contents(r.bxb2), addr < void? >(s.mtp_h[0]), r.bytes_x)
        }
        memcpy(metal_buffer_contents(r.bcos), addr < void? >(s.rope_cos[0]), r.bytes_tab)
        memcpy(metal_buffer_contents(r.bsin), addr < void? >(s.rope_sin[0]), r.bytes_tab)
    }
    prof_add("mtp.draft.prep", ts_prep)
    return r
}

//! forward_mtp's GPU twin: the draft head against a resident blob model. `pos` is the trunk position (n_past); the draft row
//! lands at pos-1 (rotary is delta-invariant). Prepares the session's mirror to rows [0, pos) itself (adopted, built or
//! uploaded from the cache); needs GPU logits. false = declined.
[hot_path, arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-nextn-chain"), arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-round-one-join")]
def metal_mtp_draft_forward(t : Model; var s : Session; token, pos : int64; seam : bool = false; wm_pos : int64 = -1l) : bool {
    var r = metal_draft_step_open(t, s, token, pos, wm_pos)
    if (!r.gpu_logits) {
        release_step(r)
        return false
    }
    let ts_enc = prof_ticks()
    encode_draft_step(t, r, !seam)
    prof_add("mtp.draft.enc", ts_enc)
    let ts_fin = prof_ticks()
    let ok = finish_draft_step(t, s, r, seam)
    prof_add("mtp.draft.wait", ts_fin)
    release_step(r)
    return ok
}

//! forward_mtp(..., false)'s GPU twin — the continuation-window SEAM: write the draft head's
//! KV row pos-1 from (embed(token), the pre-window mtp_h) without touching logits or mtp_h.
[hot_path]
def metal_mtp_seam_row(t : Model; var s : Session; token, pos : int64) : bool => metal_mtp_draft_forward(t, s, token, pos, true)

// ===== MTP B=2 same-slab verify (the spec step's trunk forward) =====

var private @scratch g_vrt : array<uint>   // per-layer 2-row route entries: uint4(koff_el+lbase, voff_el+lbase, cap, cnt_i)

//! the verify seat's rows: the session, its k+1 rows at `row0`, its slab and its recurrent mirror
//! (mirror-owned), and its tape slots
struct private MtpVerifyGroup {
    s : Session?
    tok : int64
    pos : int64
    trunk : int64   // the stream's trunk position through the draft rows steps (a draft row sits at trunk + i - 1)
    nr : int64
    row0 : int64
    koff : uint64
    voff : uint64
    cap : int64
    dn_sbase : uint64
    dn_cbase : uint64
    dn_salt : uint64
    dn_calt : uint64
    ple_base : uint64   // the n-gram conv ring inside the mirror (qwen4exp)
    tape_base : uint64
}
var private @scratch g_v_groups : array<MtpVerifyGroup>   //! the verify being encoded: the layer encoders read the rows' groups here
var private g_chain_rounds = 0l   //! rounds whose draft and verify ran as one command buffer since load: the depth-1 NextN round's and the assistant drafter's

[arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-nextn-chain")]
def public metal_mtp_chained_rounds() : int64 => g_chain_rounds

//! the verify's shape gate: one K/V codec on both sides (every mirror codec the decode serves), a silu FFN (dense or routed MoE with
//! or without the shared expert), no epilogue exotics; a missing attn_v and the moe_with_dense / mx4 MoE forms have no verify arm
def private metal_mtp_verify_ok(t : Model; s : Session) : bool {
    let c = t.config
    if (s.kv_dtype_k != s.kv_dtype_v || (s.kv_dtype_k != KVDtype.f16 && s.kv_dtype_k != KVDtype.f32 &&
            s.kv_dtype_k != KVDtype.q8_0 && s.kv_dtype_k != KVDtype.tq4) ||
            has_dual_rope(c) || c.attn_sinks || c.v_norm || c.pre_post_norm || c.layer_out_scale ||
            c.final_logit_softcap > 0.0 || !empty(t.suppress) ||
            (!t.blocks.ffn_is_dense && (c.moe_dense_shexp || t.experts_mx4)) ||
            c.ffn_act != FfnAct.silu ||
            t.wcls_off < 0l || cls_tied_f32(t)) {
        return false
    }
    for (l in range64(c.n_layers)) {
        return false if (layer_is_sliding(c, l))
        continue if (layer_is_recurrent(c, l))
        return false if (t.wv_offs[l] < 0l)
    }
    return true
}


def private rms_rows(var e : auto(ET); enc : MetalComputeEncoder?; bs : MetalBuffer?; soff : uint64; bw, bd : MetalBuffer?) {
    enc_rms_b(enc, bs, soff, bw, bd, e.r.u_dim, e.r.u_eps, e.r.nrows)
}

def private add_rows(var e : auto(ET); enc : MetalComputeEncoder?; bg : MetalBuffer?; goff : uint64; bu : MetalBuffer?; uoff : uint64; dim : int64; bosc : MetalBuffer?) {
    enc_add(enc, bg, goff, bu, uoff, e.r.u_total_dim, e.r.nrows * dim, bosc)
}

def private moe_args_rows(var e : auto(ET); ndim, ddim, eplane, xs, hasb : uint; down : bool) : MoeGemvArgs {
    let r & = unsafe(*e.r)
    return MoeGemvArgs(ndim = ndim, ddim = ddim, eplane = eplane, xs = xs, ss = r.k_moe_ss,
        bxs = down ? r.k_moe_knfe : r.k_dim, bys = down ? r.k_moe_kdim : r.k_moe_knfe, hasb = hasb)
}

//! the rows' residual step at seam `sm` (`enc_residual`): the norm into bxb dual-stores its f16 twin where the step carries one
//! (the k4 half-x forms' x); the unfused pin, and the fuse knob off on a sandwich seam, split it
def private residual_rows(var e : auto(ET); t : Model; enc : MetalComputeEncoder?; bx, ba, by : MetalBuffer?; sm : ResidualSeam) {
    var r & = unsafe(*e.r)
    let stores = by == r.bxb && sm.norm_off >= 0l
    let twin = r.bxh != null && stores
    let split = g_b_addrms_unfused || (sm.pre_off >= 0l && !decode_fuse_pre())
    enc_residual(enc, t, bx, ba, by, twin ? r.bxh : null, sm, r.nrows, ResidualForm.rows, split, ResidualUniforms(bdim = r.u_dim, beps = r.u_eps, btot = r.u_total_dim))
    if (stores) {
        r.xh_ok = twin
    }
}

//! the rows' gemma-4 step: every row's x updated in place, its next norm into bxb with the f16 twin where the step carries one
def private moe_g4_step_rows(var e : auto(ET); t : Model; enc : MetalComputeEncoder?; l : int64) {
    var r & = unsafe(*e.r)
    let twin = r.bxh != null
    moe_g4_step_enc(r, t, enc, l, r.nrows, twin)
    r.xh_ok = twin
}

def private total_rows(e : auto(ET); u : MetalBuffer?; n : int64) : tuple<b : MetalBuffer?; n : int64> {
    let r & = unsafe(*e.r)
    if (u != r.u_dim && u != r.u_hidden && u != r.u_qd && u != r.u_moe_knfe && !(u != null && u == r.u_hidden2)) {
        panic("row chain: a row-total is asked for a uniform with no nrows-scaled twin")
    }
    let bt = u == r.u_dim ? r.u_total_dim : (u == r.u_hidden ? r.u_total_h : (u == r.u_qd ? r.u_total_qd : (u == r.u_hidden2 ? r.u_total_h2 : r.u_moe_tot)))
    if (bt == null) {
        let slot = u == r.u_dim ? "u_dim" : (u == r.u_hidden ? "u_hidden" : (u == r.u_qd ? "u_qd" : (u == r.u_hidden2 ? "u_hidden2" : "u_moe_knfe")))
        panic("row chain: the row-total twin of {slot} was not built for this step (the step gate declines the models that would ask)")
    }
    return (b = bt, n = n * r.nrows)
}

[arch(at="../ARCHITECTURE_GPU_MTP_DECODE.md#metal-layer-enc")]
def private rows_tier(nrows : int64) : BatchTier => BatchTier(use_mm = false, use_gemm = false, mp = nrows, nrows = nrows, bpsk = null)

let private KQ_HALF_TWIN_ROWS = 8l   //! a half-x form reads every one of its columns, so a twin holds eight rows whatever the step's count

//! the rows step writes the f16 twins: the row count is the half-x forms', and a plane of a format whose
//! half crown is on sits at a site the rows forms serve (q, k, v, the FFN's three; wo's x has no producer twin)
[arch(at="../ARCHITECTURE_GPU_MTP_DECODE.md#kq-rows-crown")]
def private kq_half_rows(t : Model; nrows : int64) : bool {
    return false if (!metal_kq_half_any() || nrows < 2l || nrows > KQ_HALF_TWIN_ROWS)
    for (l in range64(t.config.n_layers)) {
        if (metal_kq_half_on(fmt_at(t.wq_fmt, l)) || metal_kq_half_on(fmt_at(t.wk_fmt, l)) || metal_kq_half_on(fmt_at(t.wv_fmt, l))
                || metal_kq_half_on(fmt_at(t.w1_fmt, l)) || metal_kq_half_on(fmt_at(t.w3_fmt, l)) || metal_kq_half_on(fmt_at(t.w2_fmt, l))) {
            return true
        }
    }
    return false
}

//! the twin a site's x carries: bxb's where the norm that wrote bxb stored it, the gate panel's where the activation did
def private site_x_twin(e : auto(ET); bx : MetalBuffer?; blk : block<(bxh : MetalBuffer?) : void>) {
    let r & = unsafe(*e.r)
    if (bx == r.bxb && r.xh_ok) {
        invoke(blk, r.bxh)
    } elif (bx == r.bh12 && r.fgh_ok) {
        invoke(blk, r.bfgh)
    } else {
        invoke(blk, null)
    }
}

[unused_argument(kind)]
def private site_verify(e : VerifyLayerEnc; t : Model; enc : MetalComputeEncoder?; kind : SiteKind; fmt : KqFmt;
                        woff, rows, n, ys : int64; bx, by, bn, bd, bys : MetalBuffer?; ycol : int64) {
    site_x_twin(e, bx) $(bxh) {
        enc_batch_site(enc, t, rows_tier(e.r.nrows), fmt, woff, rows, n, ys, bx, by, bn, bd, bys, ycol, false, bxh = bxh)
    }
}

//! the verify rows' panels: the single row's, at nrows rows and with no gemma4 branch
[unused_argument(moe_with_dense)]
def private panels_verify(var e : VerifyLayerEnc; moe_with_dense : bool; hid_l, knfe : int64) : FfnPanels {
    var r & = unsafe(*e.r)
    return FfnPanels(moe_in = r.bxb2, mg = r.bh12, mu = r.bh12, mu_col = r.nrows * knfe,
        fg = r.bh12, fu = r.bh12, fu_col = r.nrows * hid_l)
}


//! the verify rows' fused gate+up dot and the act over the rows form (w2 follows as a rows site)
[unused_argument(u_hid_l)]
def private w13_fused_verify(var e : VerifyLayerEnc; t : Model; enc : MetalComputeEncoder?; l : int64; hid_l : int64; u_hid_l : MetalBuffer?; geglu : bool; fg : MetalBuffer?) : bool {
    let r & = unsafe(*e.r)
    let bw1 = blob_of(g_dev, t, t.w1_offs[l])
    let bw3 = blob_of(g_dev, t, t.w3_offs[l])
    enc_gemv_w13sw_rows(enc, r.nrows, bw1.buf, bw1.boff, bw3.boff, r.bxb, fg, r.u_dim, r.u_hidden, geglu ? r.u_one32 : r.u_zero32, t.config.dim, hid_l)
    return true
}

//! the verify rows' rope + KV store: the codec's batch form over the route table (rtoff picks the layer's rows, whose bases are
//! the layer's own, so `lay` is 0), the base geometry
[unused_argument(het, kvsh, lkoff, lvoff, rowb)]
def private rope_store_verify(var e : VerifyLayerEnc; t : Model; enc : MetalComputeEncoder?; l : int64; het, kvsh : bool;
                              bbias, bcos_l, bsin_l : MetalBuffer?; lkoff, lvoff : uint64; rowb, qd, kv_dim, head_size, n_heads : int64) {
    let r & = unsafe(*e.r)
    let qkvd = uint(qd + 2l * kv_dim)
    let rtoff = uint64(l * 16l * r.nrows)
    let vrot = int64(r.k_rot != 0u ? r.k_rot : r.k_hs)
    let gd = qg_direct(t.config, r.kdt)
    let bq = gd ? r.bdn_qg : null
    let qhs = gd ? uint(2l * head_size) : 0u
    let qrow = gd ? uint(2l * qd) : 0u
    if (r.kdt == KVDtype.tq4) {
        var tka = RopeStoreBTq4Args(lay = 0u, qd = uint(qd), head_size = uint(head_size), nheads = uint(n_heads), kv_mul = r.k_kvm,
            qstride = qkvd, neox = r.k_neox, has_bias = r.k_hasb, rot = uint(vrot), qhs = qhs, qrow = qrow, ring = uint(mirror_ring(t, l)))
        enc_rope_store_b_tq4(enc, r.bqkv, bcos_l, bsin_l, r.brt, bbias, tka, n_heads + 2l * (kv_dim / head_size), r.nrows, head_size, rtoff, bq)
    } elif (r.kdt == KVDtype.q8_0) {
        var qka = RopeStoreBQ8Args(lay = 0u, nblk = uint(kv_dim / 32l), qd = uint(qd), head_size = uint(head_size), npairs_q = uint(qd / 2l),
            qstride = qkvd, neox = r.k_neox, has_bias = r.k_hasb, rot = uint(vrot), qhs = qhs, qrow = qrow, ring = uint(mirror_ring(t, l)))
        enc_rope_store_b_q8(enc, r.bqkv, bcos_l, bsin_l, r.brt, bbias, qka, q8_store_threads(qd, kv_dim), r.nrows, rtoff, bq)
    } elif (qkn_fused(t.config, r.kdt)) {
        var nka = qkn_args(t, l, qd, kv_dim, head_size, vrot, r.k_neox != 0u, r.k_hasb != 0u, r.k_eps, int64(qkvd), vrot / 2l, 0l)
        nka.ring = uint(mirror_ring(t, l))
        if (gd) {
            nka.qstride = qhs
            nka.qrow = qrow
        }
        enc_qkn_rope_b(enc, r.kdt == KVDtype.f16, r.bqkv, bcos_l, bsin_l, r.brt, qkn_weights(t, l, head_size), bbias, nka,
            n_heads + 2l * (kv_dim / head_size), r.nrows, rtoff, bq)
    } else {
        var vrka = gk_rope_kv_args(int64(r.k_qd), int64(r.k_kvd), int64(r.k_hs), int64(r.k_npq), int64(r.k_npall), r.k_neox != 0u, r.k_hasb != 0u, vrot,
            0l, 0l, r.nrows, int64(qkvd), int64(qkvd), vrot / 2l, 0l, mirror_ring(t, l))
        enc_rope_store_b(enc, r.kdt == KVDtype.f16, r.bqkv, bcos_l, bsin_l, r.brt, bbias, vrka, (qd + kv_dim) / 2l, r.nrows, rtoff)
    }
}

//! the verify rows' attention: the codec's batch forms over each layer's slab through the route table (the base, non-sliding
//! geometry; `kstride` counts a block codec's rows in 32-blocks)
[unused_argument(lsl, het, bsnk, lkoff, lvoff)]
def private attention_verify(var e : VerifyLayerEnc; t : Model; enc : MetalComputeEncoder?; l : int64; lsl, het : bool;
                             bsnk : MetalBuffer?; lkoff, lvoff : uint64; nchunks, head_size, n_heads : int64) {
    let c = t.config
    let r & = unsafe(*e.r)
    let kv_dim = uint(c.n_kv_heads * head_size)
    let kstride = r.kdt == KVDtype.q8_0 || r.kdt == KVDtype.tq4 ? kv_dim / 32u : kv_dim
    let qkvd = uint(n_heads * head_size) + 2u * kv_dim
    let rtoff = uint64(l * 16l * r.nrows)
    let vscale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    let gd = qg_direct(c, r.kdt)
    let bq = gd ? r.bdn_qg : r.bqkv
    let qstride = gd ? uint(2l * n_heads * head_size) : qkvd
    let qhs = gd ? uint(2l * head_size) : 0u
    let gated = gd && r.kdt != KVDtype.tq4 ? 1u : 0u
    if (r.attn_d) {
        encode_attn_d(t, r, enc, l, head_size, n_heads, 0u, null, rtoff, true)
    } elif (r.chunked) {
        var pka = SqAttnPartBArgs(lay = 0u, kstride = kstride, nheads = uint(n_heads),
            kv_mul = uint(n_heads / c.n_kv_heads), hs = uint(head_size), scale = vscale,
            qstride = qstride, nchmax = uint(nchunks), softcap = c.attn_logit_softcap, window = 0u, qhs = qhs, gated = gated, ring = uint(mirror_ring(t, l)))
        enc_attn_part_b(enc, r.kdt, bq, r.bpart, r.brt, pka, r.nrows, nchunks, n_heads, head_size, [rtoff = rtoff])
        var cka = SqAttnCombArgs(nch = uint(nchunks), nheads = uint(n_heads),
            hs = uint(head_size), window = 0u, hass = 0u)
        enc_attn_comb_b(enc, r.bpart, r.bxb, cka, r.brt, rtoff, null, n_heads * r.nrows, head_size)
    } else {
        var bka = SqAttnBArgs(lay = 0u, kstride = kstride, nheads = uint(n_heads),
            kv_mul = uint(n_heads / c.n_kv_heads), hs = uint(head_size), scale = vscale,
            qstride = qstride, softcap = c.attn_logit_softcap, hass = 0u, qhs = qhs, gated = gated)
        enc_attn_b(enc, r.kdt, bq, r.bxb, r.brt, bka, r.nrows, n_heads, head_size, [rtoff = rtoff])
    }
}

//! the rows forms' deltanet input projections over `r.nrows` rows: qkv, z and beta/alpha, each plane through its
//! tag's rows site (f32-on-disk beta/alpha through the router-slab f32 GEMV, one stream a row)
def private enc_dn_proj_rows(r : StepRes; t : Model; enc : MetalComputeEncoder?; l : int64; single : bool = false) {
    let c = t.config
    let dim = c.dim
    let cd = dn_conv_dim(c)
    let di = c.ssm_d_inner
    let nvh = c.ssm_dt_rank
    let nr = r.nrows
    site_rows(enc, t, single, fmt_at(t.dnqkv_fmt, l), t.dnqkv_offs[l], cd, dim, r.bxb, r.bdn_qkv, r.u_dim, r.u_cd, nr)
    site_rows(enc, t, single, fmt_at(t.dngate_fmt, l), t.dngate_offs[l], di, dim, r.bxb, r.bdn_z, r.u_dim, r.u_di, nr)
    if (t.dn_ba_f32) {
        enc_dn_ba_f32(enc, t, l, r.bxb, r.bdn_b, nr)
    } else {
        site_rows(enc, t, single, fmt_at(t.dnbeta_fmt, l), t.dnbeta_offs[l], nvh, dim, r.bxb, r.bdn_b, r.u_dim, r.u_nvh, nr)
        site_rows(enc, t, single, fmt_at(t.dnalpha_fmt, l), t.dnalpha_offs[l], nvh, dim, r.bxb, r.bdn_g, r.u_dim, r.u_nvh, nr)
    }
}

//! the shared expert over bxb into bxb2, each plane through its tag's site: two q8 gate/up planes keep the fused gate + up + act
//! form, any other pair takes a site each and the activation; the single row's panel is bh12s, the rows forms' bh12 (free after the
//! routed W2), up at column nrows * nsh
def private enc_shexp(r : StepRes; t : Model; enc : MetalComputeEncoder?; l : int64; single : bool) {
    let dim = t.config.dim
    let nsh = t.config.n_ff_shexp
    let nr = r.nrows
    let p1 = shexp_plane(t, l, 1)
    let p3 = shexp_plane(t, l, 3)
    let p2 = shexp_plane(t, l, 2)
    let bp = single ? r.bh12s : r.bh12
    if (p1.f == KqFmt.q8 && p3.f == KqFmt.q8) {
        let bwsh1 = blob_of(g_dev, t, p1.off)
        let w3off = uint64((p3.off / 32l) * 34l)
        if (single) {
            enc_gemv_w13sw(enc, bwsh1.buf, bwsh1.boff, w3off, r.bxb, bp, r.u_dim, r.u_moe_nsh, dim, nsh, r.u_zero32)
        } else {
            enc_gemv_w13sw_rows(enc, nr, bwsh1.buf, bwsh1.boff, w3off, r.bxb, bp, r.u_dim, r.u_moe_nsh, r.u_zero32, dim, nsh)
        }
    } else {
        enc_shexp_site(r, t, enc, single, p1.f, p1.off, nsh, dim, r.bxb, bp, r.u_dim, r.u_moe_nsh, 0l)
        enc_shexp_site(r, t, enc, single, p3.f, p3.off, nsh, dim, r.bxb, bp, r.u_dim, r.u_moe_nsh, nr * nsh)
        if (g_skip != "ew") {
            enc_swiglu(enc, bp, 0ul, bp, uint64(nr * nsh * 4l), single ? r.u_moe_nsh : r.u_moe_shtot, nr * nsh)
        }
    }
    enc_shexp_site(r, t, enc, single, p2.f, p2.off, dim, nsh, bp, r.bxb2, r.u_moe_nsh, r.u_dim, 0l)
}

//! one shared-expert site of `rows` outputs over K `n`: the single row's GEMV, or the rows form over `r.nrows` rows at row stride `rows`
def private enc_shexp_site(r : StepRes; t : Model; enc : MetalComputeEncoder?; single : bool; fmt : KqFmt; woff, rows, n : int64;
                           bx, by, bn, bd : MetalBuffer?; ycol : int64) {
    if (single) {
        enc_site_gemv(enc, t, fmt, woff, rows, n, bx, by, bn, bd, uint64(ycol * 4l))
    } else {
        enc_batch_site(enc, t, rows_tier(r.nrows), fmt, woff, rows, n, rows, bx, by, bn, bd, bd, ycol, false)
    }
}

//! the rows forms' deltanet out projection over `r.nrows` rows, through the out plane's tag
def private enc_dn_out_rows(r : StepRes; t : Model; enc : MetalComputeEncoder?; l : int64; single : bool = false) {
    let dim = t.config.dim
    let di = t.config.ssm_d_inner
    site_rows(enc, t, single, fmt_at(t.dnout_fmt, l), t.dnout_offs[l], dim, di, r.bdn_o, r.bxb2, r.u_di, r.u_dim, r.nrows)
}

//! the verify rows' deltanet layer: the rows forms, the post-row0 state and history landing in
//! the shadow region (reject flips to it), the split tape recording the pre-scan operands
def private recurrent_verify(var e : VerifyLayerEnc; t : Model; enc : MetalComputeEncoder?; l : int64; var dnli : int64&) {
    let c = t.config
    var r & = unsafe(*e.r)
    let cd = dn_conv_dim(c)
    let nvh = c.ssm_dt_rank
    var d = dn_stages_of(r, r.dn_split)
    d.mid = !r.dn_split
    enc_dn_proj_rows(r, t, enc, l)
    enc_dn_front(enc, t, l, dnli, d)
    if (r.dn_split) {
        for (g in g_v_groups) {
            let tb = g.tape_base + r.tape_slot * uint64(dnli)
            let cvb = uint64(g.nr * cd * 4l)
            enc_copy_row(enc, r.bdn_cv, uint64(g.row0 * cd * 4l), r.btape, tb, r.u_tcd, g.nr * cd)
            enc_copy_row(enc, r.bdn_qkv, uint64(g.row0 * cd * 4l), r.btape, tb + cvb, r.u_tcd, g.nr * cd)
            enc_dn_tape_ba(r, enc, nvh, g.row0, g.nr, tb + 2ul * cvb, false)
        }
    }
    enc_dn_back(enc, t, l, dnli, d)
    enc_dn_out_rows(r, t, enc, l)
    dnli++
}

//! a group's `nr` beta and alpha rows from row `row0` against its tape slot at `tb`, as two copies of `nr * nvh` floats: the
//! [beta; alpha] rows' span in halves under `dn_ba_rows`, else a plane each; `back` copies the tape into the planes' first rows
def private enc_dn_tape_ba(var r : StepRes; enc : MetalComputeEncoder?; nvh, row0, nr : int64; tb : uint64; back : bool) {
    let n = nr * nvh
    let nvb = uint64(n * 4l)
    let rows_b = r.dn_ba_rows ? 2l : 1l
    let o0 = back ? 0ul : uint64(row0 * rows_b * nvh * 4l)
    let o1 = r.dn_ba_rows ? o0 + nvb : o0
    var b1 = r.dn_ba_rows ? r.bdn_b : r.bdn_g
    if (back) {
        enc_copy_row(enc, r.btape, tb, r.bdn_b, o0, r.u_tnvh, n)
        enc_copy_row(enc, r.btape, tb + nvb, b1, o1, r.u_tnvh, n)
    } else {
        enc_copy_row(enc, r.bdn_b, o0, r.btape, tb, r.u_tnvh, n)
        enc_copy_row(enc, b1, o1, r.btape, tb + nvb, r.u_tnvh, n)
    }
}

//! the MTP verify's layer encoder: k+1 rows over the same slab through the route table
struct private VerifyLayerEnc : LayerEncT {
    override site = @@site_verify
    override total = @@total_rows
    override residual = @@residual_rows
    override rms = @@rms_rows
    override add = @@add_rows
    override moe_args = @@moe_args_rows
    override rope_store = @@rope_store_verify
    override attention = @@attention_verify
    override recurrent = @@recurrent_verify
    override w13_fused = @@w13_fused_verify
    override panels = @@panels_verify
    override moe_g4_step = @@moe_g4_step_rows
}

// one verify-shaped B=2 layer (batch-form sites, same-slab brt row at l*32) — the trunk
// loop body, shared with the draft-slab warm (which runs it at l == n_layers)
[unused_argument(pos)]
def private encode_verify_layer(t : Model; var r : StepRes; enc : MetalComputeEncoder?; var brt : MetalBuffer?; l, pos : int64; var dnli : int64&) {
    r.brt = brt
    r.chain_pos = pos
    var e = VerifyLayerEnc(r = unsafe(addr(r)))
    e->encode(t, enc, l, dnli)
}

//! One speculative round's drafts beside the verify's truths, for the trace arms.
[cold_path]
def mtp_round_trace(tag : string; s : Session; pos, k, nr, a, vocab : int64) {
    let line : string = build_string() $(var w) {
        w |> write("{tag} round pos={pos} k={k} drafts=[")
        for (i in range64(k)) {
            w |> write("{s.mtp_vbatch[i + 1l]} ")
        }
        w |> write("] truths=[")
        for (i in range64(nr)) {
            w |> write("{argmax_row(s.mtp_logits_b, i, vocab)} ")
        }
        w |> write("] a={a}")
    }
    to_log(LOG_INFO, "{line}\n")
}

[arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-verify-draft-warm"), arch(at="../ARCHITECTURE_GPU_MTP.md#verify-serial-encoder"), arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-nextn-chain")]
def private encode_verify_step(t : Model; var r : StepRes) {
    let c = t.config
    let dim = c.dim
    let nr = r.nrows   //! every group's rows together
    let ng = long_length(g_v_groups)
    var brt = group_routes(t, r, nr)
    var cb = metal_new_command_buffer_unretained(g_queue)
    var enc = metal_new_compute_encoder(cb)
    if (g_v_chain_on) {
        encode_draft_body(t, g_v_chain, enc, true)
        enc_argmax(enc, g_v_chain.blog, g_v_chain.btok, g_v_chain.u_vocab)
        let xoff = uint64(dim * 4l)
        enc_embed_dev(enc, t, g_v_chain.btok, 0ul, r.bx, xoff, 1l, r.u_dim, r.u_embscale)
        enc_embed_dev(enc, t, g_v_chain.btok, 0ul, r.bcat, xoff, 1l, r.u_dim, r.u_embscale)
    }
    if (r.hc) {   // the rows' embeds open the wide residual rows; the mixers stand at the seams
        enc_hc_init(enc, r.bx, r.bhc, hc_init_args(c, nr), nr * hc_dim(c))
    } else {
        var bw_att0 = upload_region(unsafe(addr < void? >(t.fblob[t.rms_att_off])), uint64(dim * 4l))
        if (r.bxh != null) {
            enc_rms_bhx(enc, r.bx, 0ul, bw_att0, r.bxb, r.u_dim, r.u_eps, r.bxh, nr)
            r.xh_ok = true
        } else {
            enc_rms_b(enc, r.bx, 0ul, bw_att0, r.bxb, r.u_dim, r.u_eps, nr)
        }
    }
    var dnli = 0l
    var pdeep = g_v_groups[0].pos   //! the chain's chunk count follows the DEEPEST group: a shallower count leaves the deep rows' partials unwritten
    for (g in g_v_groups) {
        pdeep = max(pdeep, g.pos)
    }
    for (l in range64(c.n_layers)) {
        encode_verify_layer(t, r, enc, brt.buf, l, pdeep, dnli)
    }
    enc_cls_tail_rows(t, r, enc, t.rms_final_off, r.blog, nr)
    if (r.bvarg != null) {
        enc_argmax_rows(enc, r.blog, r.bvarg, r.u_vocab, nr)
    }
    enc_sel_tail(t, r, enc, r.blog, nr)
    // the head's carry hiddens from committed history: a group's row 0 takes its saved pre-draft hidden (cat row nr + its index), row i the verify's post-norm row i-1
    let rowb = uint64(dim * 4l)
    if (r.hc) {   // the carries are wide rows: the trunk's wide residual rows kept for the commit, row i-1 feeding row i
        enc_copy_row(enc, r.bhc, 0ul, r.bhc_save, 0ul, r.u_hcd_tot, nr * hc_dim(c))
        for (g in g_v_groups) {
            for (i in range64(1l, g.nr)) {
                enc_copy_row(enc, r.bhc_save, uint64(g.row0 + i - 1l) * r.bytes_hcd, r.bhc_carry, uint64(g.row0 + i) * r.bytes_hcd, r.u_hcd, hc_dim(c))
            }
        }
    } else {
        for (gi in range64(ng)) {
            let g = g_v_groups[gi]
            enc_copy_row(enc, r.bcat, uint64(nr + gi) * rowb, r.bxb2, uint64(g.row0) * rowb, r.u_dim, dim)
            for (i in range64(1l, g.nr)) {
                enc_copy_row(enc, r.bxf, uint64(g.row0 + i - 1l) * rowb, r.bxb2, uint64(g.row0 + i) * rowb, r.u_dim, dim)
            }
        }
    }
    enc_draft_head_rows(t, r, enc, r.bcat, nr)
    var dnw = 0l
    encode_verify_layer(t, r, enc, brt.buf, c.n_layers, pdeep, dnw)
    metal_end_encoding(enc)
    metal_release(enc)
    metal_commit(cb)
    r.committed = true
    r.cb = cb
    pool_release(g_upool, brt.buf, brt.bytes)
}

//! the draft head's rows prelude: [enorm(embed) ; hnorm(carry)] -> eh_proj -> the trunk-shaped block's
//! attention norm. `embed` holds the rows' raw embeds, bxb2 their carry hiddens; bx and bxb leave with the
//! projected rows and their norm
def private enc_draft_head_rows(t : Model; var r : StepRes; enc : MetalComputeEncoder?; embed : MetalBuffer?; nr : int64; single : bool = false) {
    let c = t.config
    let dim = c.dim
    if (r.hc) {   // the carries sit in bhc_carry as wide rows; the prelude lands the draft's wide rows in bhc
        hc_draft_prelude(t, r, enc, embed, nr)
        return
    }
    unsafe {
        var bw_en = upload_region(addr < void? >(t.fblob[t.mtp_enorm_off]), uint64(dim * 4l))
        norm_rows(enc, single, embed, bw_en, r.bh12, r.u_dim, r.u_eps, nr)
        var bw_hn = upload_region(addr < void? >(t.fblob[t.mtp_hnorm_off]), uint64(dim * 4l))
        norm_rows(enc, single, r.bxb2, bw_hn, r.bxb, r.u_dim, r.u_eps, nr)
        enc_cat2(enc, r.bcat, r.bh12, r.bxb, cat2_args(dim, nr), nr * dim)
        site_rows(enc, t, single, t.mtp_ehproj_fmt, t.mtp_ehproj_off, dim, 2l * dim, r.bcat, r.bx, r.u_dim2, r.u_dim, nr)
        var bw_att = upload_region(addr < void? >(t.fblob[t.rms_att_off + c.n_layers * dim]), uint64(dim * 4l))
        norm_rows(enc, single, r.bx, bw_att, r.bxb, r.u_dim, r.u_eps, nr)
    }
    r.xh_ok = false   //! the head's norm wrote bxb with no twin: the MTP layer's sites read the f32 rows
}

//! the rows classifier tail: the final norm at `norm_off` over bx, the classifier GEMM into `bcls`, then
//! per row the final-logit soft cap and the suppressed ids in the CPU order
def private enc_cls_tail_rows(t : Model; var r : StepRes; enc : MetalComputeEncoder?; norm_off : int64; bcls : MetalBuffer?; nr : int64;
                              hc_norm_off : int64 = -1l; hc_down_off : int64 = -1l; hc_up_off : int64 = -1l; single : bool = false) {
    let c = t.config
    let dim = c.dim
    if (r.hc) {   // the head mixer over every wide row stands where the final norm would (the trunk's, or the draft head's own)
        hc_mix_rows(r, t, enc, r.bhc, hc_norm_off >= 0l ? hc_norm_off : t.hc_head_norm_off,
            hc_down_off >= 0l ? hc_down_off : t.hc_head_down_off, hc_up_off >= 0l ? hc_up_off : t.hc_head_up_off, -1l, r.bxf, null)
    } else {
        var bwfin = upload_region(unsafe(addr < void? >(t.fblob[norm_off])), uint64(dim * 4l))
        norm_rows(enc, single, r.bx, bwfin, r.bxf, r.u_dim, r.u_eps, nr)
    }
    site_rows(enc, t, single, cls_fmt_of(t), t.wcls_off, c.vocab_size, dim, r.bxf, bcls, r.u_dim, r.u_vocab, nr)
    if (r.cls_epi) {
        enc_cls_epi_rows(enc, bcls, r.bsupp, c.vocab_size, nr, c.final_logit_softcap, int64(r.bytes_supp / 4ul))
    }
}

//! lands every group's KV rows into its stream and its rows' logits into its stream's verify rows;
//! accept/reject bookkeeping is the caller's
def private finish_verify_step(t : Model; r : StepRes; land_logits : bool = true) : bool {
    let ts_wait = prof_ticks()
    metal_wait_until_completed(r.cb)
    prof_add("mtp.verify.wait", ts_wait)
    let ts_land = prof_ticks()
    let ok = !metal_command_buffer_failed(r.cb)
    g_msel_landed = ok ? r.sel_rows : 0l
    if (ok) {
        let c = t.config
        unsafe {
            let ab = reinterpret<uint8?>(metal_buffer_contents(g_arena))
            var lg = reinterpret<uint8?>(metal_buffer_contents(r.blog))
            let vb = c.vocab_size * 4l
            for (g in g_v_groups) {
                var s & = unsafe(*g.s)
                for (l in range64(c.n_layers + c.n_layer_nextn)) {   // + the warmed draft-slab rows
                    let rowb = kv_row_bytes(s.kv_dtype_k, layer_kv_dim(c, l))
                    let lb = mirror_lbase(t, s.kv_dtype_k, g.cap, l)
                    for (i in range64(g.nr)) {
                        mirror_row_writeback(s, c.seq_len, l, kv_ring_row(g.pos + i, mirror_ring(t, l)), kv_phys_row(s, g.pos + i), rowb, lb, ab, g.koff, g.voff)
                    }
                }
                s.mtp_logits_b |> ensure_length(g.nr * c.vocab_size)
                if (land_logits) {
                    memcpy(addr < void? >(s.mtp_logits_b[0]), reinterpret<void?>(lg + g.row0 * vb), uint64(g.nr * vb))
                    memcpy(addr < void? >(s.mtp_logits[0]), reinterpret<void?>(lg + g.row0 * vb), uint64(vb))
                }
            }
        }
    } else {
        to_log(LOG_ERROR, "dasLLAMA metal MTP verify: dispatch failed ({cb_error_text(r.cb)})\n")   // nolint:PERF028,PERF026 — error path, and the CPU fallback dwarfs the log
        decline(MetalDecodeDecline.gpu_error)
    }
    prof_add("mtp.verify.land", ts_land)
    return ok
}

[arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-dn-shadow-replay")]
def private encode_dn_replay(t : Model; var r : StepRes; gi : int64; nrun : int64) {
    let c = t.config
    let cd = dn_conv_dim(c)
    let nvh = c.ssm_dt_rank
    var g & = unsafe(g_v_groups[gi])
    var cb = metal_new_command_buffer_unretained(g_queue)
    var enc = metal_new_compute_encoder(cb)
    //! the group's taped rows come back at row 0 of the planes: one group replays at a time
    dn_grp_clear()
    dn_grp_push(0l, nrun, g.dn_sbase, g.dn_salt, g.dn_cbase, g.dn_calt)
    var tab = dn_grp_upload()
    var d = dn_stages_of(r, true)
    d.z = null
    d.grp = tab.buf
    d.ng = 1l
    d.npmax = nrun
    d.rows = nrun
    var dnli = 0l
    let cvb = uint64(g.nr * cd * 4l)
    for (l in range64(c.n_layers)) {
        if (!layer_is_recurrent(c, l)) {
            continue
        }
        let tb = g.tape_base + r.tape_slot * uint64(dnli)
        enc_copy_row(enc, r.btape, tb, r.bdn_cv, 0ul, r.u_tcd, g.nr * cd)
        enc_copy_row(enc, r.btape, tb + cvb, r.bdn_qkv, 0ul, r.u_tcd, g.nr * cd)
        enc_dn_tape_ba(r, enc, nvh, 0l, g.nr, tb + 2ul * cvb, true)
        enc_dn_hist(enc, c, dnli, d)
        enc_dn_back(enc, t, l, dnli, d)
        dnli++
    }
    metal_end_encoding(enc)
    metal_release(enc)
    metal_commit(cb)
    metal_wait_until_completed(cb)
    metal_release(cb)
    pool_release(g_upool, tab.buf, tab.bytes)
}

//! The round's commit at `pos` after `a` of `k` drafts: the host half (`mtp_round_commit_host`) and the mirror's watermark
def mtp_round_commit(var s : Session; pos, k, a : int64) {
    g_mirrors[s.uid].watermark = pos + a + 1l
    mtp_round_commit_host(s, pos, k, a)
}

//! one route entry per (layer, row): (k base, v base, cap, cnt) - the row at g.pos + i lands as row cnt - 1 of its stream's slab
//! every group's rows' route entries (k base, v base, cap, position) per layer, the rows past the groups
//! filled with row 0's; returns the pooled buffer and its byte size for the release
def private group_routes(t : Model; r : StepRes; nr : int64) : tuple<buf : MetalBuffer?; bytes : uint64> {
    let c = t.config
    let esz = uint64(kv_block_codec(r.kdt) ? 1l : kv_row_bytes(r.kdt, 1l))   // nolint:LINT021 — divides uint64 arena offsets; only the quotient narrows
    g_vrt |> grow_resize((c.n_layers + c.n_layer_nextn) * 4l * nr)
    for (l in range64(c.n_layers + c.n_layer_nextn)) {
        for (g in g_v_groups) {
            //! each group's layer bases follow ITS mirror's cap - a deeper stream's slice is taller than group 0's
            let lb = uint((g.koff + uint64(mirror_lbase(t, r.kdt, g.cap, l))) / esz)
            let vb = uint((g.voff + uint64(mirror_lbase(t, r.kdt, g.cap, l))) / esz)
            for (i in range64(g.nr)) {
                let e = (l * nr + g.row0 + i) * 4l
                g_vrt[e] = lb
                g_vrt[e + 1l] = vb
                g_vrt[e + 2l] = uint(g.cap)
                g_vrt[e + 3l] = uint(g.pos + 1l + i)
            }
        }
    }
    var covered = 0l
    for (g in g_v_groups) {
        covered += g.nr
    }
    for (l in range64(c.n_layers + c.n_layer_nextn)) {
        for (i in range64(covered, nr)) {
            let e = (l * nr + i) * 4l
            let e0 = (l * nr) * 4l
            for (k in range64(4l)) {
                g_vrt[e + k] = g_vrt[e0 + k]
            }
        }
    }
    let bytes_rt = uint64((c.n_layers + c.n_layer_nextn) * 16l * nr)
    var brt = pool_acquire_untracked(g_upool, g_dev, bytes_rt)
    unsafe {
        memcpy(metal_buffer_contents(brt), addr < void? >(g_vrt[0]), bytes_rt)
    }
    return (buf = brt, bytes = bytes_rt)
}

//! lands a draft row: its draft-slab KV row back to the stream's cache, then - when `hx` is given - the
//! head's own h_nextn as the stream's carry hidden (NOT a trunk hidden: the warm's seam row must not
//! consume it) and, when `lg` is given, the row's logits
def private land_draft_row(t : Model; var s : Session; pos : int64; cap : int64; koff, voff : uint64;
                           ab : uint8?; hx, lg : void?) {
    let c = t.config
    let l = c.n_layers
    unsafe {
        let rowb = kv_row_bytes(s.kv_dtype_k, layer_kv_dim(c, l))
        let lb = mirror_lbase(t, s.kv_dtype_k, cap, l)
        mirror_row_writeback(s, c.seq_len, l, kv_ring_row(pos, mirror_ring(t, l)), kv_phys_row(s, pos), rowb, lb, ab, koff, voff)
        if (hx != null) {
            memcpy(addr < void? >(s.mtp_h[0]), hx, mtp_h_dim(c) * 4l)
            s.mtp_h_pos1 = 0l
        }
        if (lg != null) {
            memcpy(addr < void? >(s.logits[0]), lg, c.vocab_size * 4l)
        }
    }
}

var private g_msel_words : MetalBuffer?   //! the rows' ask words (SEL_WORDS a row, MTP_MAX_ROWS rows), host-poked a step
var private g_msel_hdr : MetalBuffer?     //! the landing's four planes, a row a slot up to MTP_MAX_ROWS: pointers into them are the landing
var private g_msel_ids : MetalBuffer?
var private g_msel_vals : MetalBuffer?
var private g_msel_rec : MetalBuffer?
var private g_msel_ask = false            //! the next classifier tail of `g_msel_ask_rows` rows lands its selection
var private g_msel_ask_rows = 0l
var private g_msel_landed = 0l            //! the rows the last landed step's selection covered (0 = its rows alone)
var private g_msel_installed = false


def private msel_alloc() {
    return if (g_msel_words != null)
    let nb = MTP_MAX_ROWS
    g_msel_words = metal_new_buffer(g_dev, uint64(SEL_WORDS * 4l * nb))
    g_msel_hdr = metal_new_buffer(g_dev, uint64(SEL_HDR * 4l * nb))
    g_msel_ids = metal_new_buffer(g_dev, uint64(SEL_CAP * 4l * nb))
    g_msel_vals = metal_new_buffer(g_dev, uint64(SEL_CAP * 4l * nb))
    g_msel_rec = metal_new_buffer(g_dev, uint64(SEL_RECENT * 4l * nb))
}

//! the landing's buffers released with the driver's kernels
def metal_sel_release() {
    release_handles(g_msel_words, g_msel_hdr, g_msel_ids, g_msel_vals, g_msel_rec)
    g_msel_words = null
    g_msel_hdr = null
    g_msel_ids = null
    g_msel_vals = null
    g_msel_rec = null
    g_msel_ask = false
    g_msel_landed = 0l
}

//! RdecSelAskFn: the next command of `nrows` rows lands the selection; false where the device has no selection pipeline
[arch(at="../ARCHITECTURE_GPU_MTP_DECODE_PICK.md#metal-selection-landing")]
def private metal_sel_ask(nrows : int64; words : array<uint>) : bool {
    return false if (g_dev == null || g_pso_cls_select == null)
    verify(nrows >= 1l && nrows <= MTP_MAX_ROWS && long_length(words) >= SEL_WORDS * nrows, "metal_sel_ask: rows past the landing's planes, or words short of them")
    msel_alloc()
    unsafe {
        memcpy(metal_buffer_contents(g_msel_words), addr < void? >(words[0]), SEL_WORDS * 4l * nrows)
    }
    g_msel_ask = true
    g_msel_ask_rows = nrows
    return true
}

[arch(at="../ARCHITECTURE_GPU_MTP_DECODE_PICK.md#metal-selection-landing")]
def private metal_sel_landing() : SelLanding {
    var l : SelLanding
    g_msel_ask = false   //! an ask no command took dies here, never on a later command
    return l if (g_msel_landed == 0l || g_msel_hdr == null)
    unsafe {
        l.rows = g_msel_landed
        l.cap = SEL_CAP
        l.recent_cap = SEL_RECENT
        l.hdr = reinterpret<uint const?>(metal_buffer_contents(g_msel_hdr))
        l.ids = reinterpret<uint const?>(metal_buffer_contents(g_msel_ids))
        l.vals = reinterpret<float const?>(metal_buffer_contents(g_msel_vals))
        l.rec = reinterpret<float const?>(metal_buffer_contents(g_msel_rec))
    }
    return l
}

[unused_argument(row0, nrows, dst), arch(at="../ARCHITECTURE_GPU_MTP_DECODE_PICK.md#metal-selection-landing")]
def private metal_sel_land_rows(row0, nrows : int64; var dst : array<float>) {}

[arch(at="../ARCHITECTURE_GPU_MTP_DECODE_PICK.md#metal-selection-landing")]
def private msel_install() {
    return if (g_msel_installed)
    install_rdec_select(@@metal_sel_ask, @@metal_sel_landing, @@metal_sel_land_rows)
    g_msel_installed = true
}

[arch(at="../ARCHITECTURE_GPU_MTP_DECODE_PICK.md#metal-selection-landing")]
def private enc_sel_tail(t : Model; var r : StepRes; enc : MetalComputeEncoder?; bcls : MetalBuffer?; nrows : int64) {
    r.sel_rows = 0l
    return if (!g_msel_ask || g_msel_ask_rows != nrows)
    g_msel_ask = false
    var ka = GkClsSelArgs(vocab = uint(t.config.vocab_size))
    enc_cls_select(enc, bcls, g_msel_words, g_msel_hdr, g_msel_ids, g_msel_vals, g_msel_rec, ka, nrows)
    r.sel_rows = nrows
}

var private g_v_rd : StepRes           //! the verify step the seat parked for the round's reject: the recurrent replay reads its tape, the ring's restore its rows
var private g_v_parked = false
var private g_v_park_uid = 0ul
var private g_v_park_pos = 0l
var private g_v_park_n = 0l
var private g_v_ring_save : MetalBuffer?   //! an n-gram model's conv ring as it stood before the parked verify committed its rows
var private g_v_ring_u : MetalBuffer?
var private g_v_ring_bytes = 0ul

def private mtp_verify_unpark() {
    return if (!g_v_parked)
    release_step(g_v_rd)
    if (g_v_ring_save != null) {
        pool_release(g_pool, g_v_ring_save, g_v_ring_bytes)
        pool_release(g_pool, g_v_ring_u, 4ul)
        g_v_ring_save = null
        g_v_ring_u = null
    }
    g_v_parked = false
    g_v_park_uid = 0ul
    g_v_groups |> clear()
}

[arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-round-one-join")]
def private metal_mtp_cold_why(t : Model; s : Session; pos, k : int64) : string {
    let c = t.config
    var why = ""
    if (g_dev == null) {
        why = "no device"
    } elif (!metal_mtp_verify_ok(t, s)) {
        why = "the verify declines the model's shape or the session's codec"
    } elif (c.recr_mask != 0ul && !dn_mirror_at(s.uid, pos)) {
        why = "the recurrent mirror does not stand at the round's position"   //! the verify scans the decode mirror's live region
    } elif (!key_exists(g_mirrors, s.uid)) {
        why = "the session has no K/V mirror"
    } else {
        let kmr = g_mirrors[s.uid]
        if (kmr.watermark < pos) {
            why = "the mirror's watermark sits below the round"
        } elif (kmr.cap_rows < pos + k + 1l) {
            why = "the slab holds fewer rows than the round's"
        } elif (!mirror_slice_in_reach(s.uid)) {
            why = "the route table does not reach the slab"
        }
    }
    if (!empty(why) && g_env_mtp_debug == "cold") {
        let dnp = c.recr_mask != 0ul && key_exists(g_dn_mirrors, s.uid) ? g_dn_mirrors[s.uid].pos : -1l
        let wm = key_exists(g_mirrors, s.uid) ? g_mirrors[s.uid].watermark : -1l
        to_log(LOG_INFO, "mtp cold: {why} (pos={pos} k={k} h_pos1={s.mtp_h_pos1} dn_pos={dnp} watermark={wm})\n")
    }
    return why
}

var private g_v_chain : StepRes   //! the depth-1 round's parked draft step: encoded ahead of the verify on its encoder, its winner in `btok`
var private g_v_chain_on = false
var private g_v_chain_tok = 0l   //! ... and the draft's inputs, for the slow draft where the verify cannot take the parked step
var private g_v_chain_row = 0l

def private chain_draft_drop() {
    return if (!g_v_chain_on)
    release_step(g_v_chain)
    g_v_chain_on = false
}

//! whether a depth-1 greedy round's draft can ride the verify's command buffer: the round asks the pick alone (greedy, no chain
//! cut) at depth 1, the table gathers the winner on the device, and no n-gram side input reads the host's tokens before the join
def private can_park_chain_draft(t : Model; s : Session; pick_only : bool) : bool
    => pick_only && mtp_depth_for(s, get_mtp_depth()) == 1l && emb_dev_gather_ok(t) && !has_ple_ngram(t.config)

[hot_path, arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-nextn-chain")]   //! per draft row
def private metal_mtp_draft_seat(t : Model; var s : Session; token, row : int64; pick_only : bool) : int64 {
    mtp_verify_unpark()
    chain_draft_drop()
    msel_install()
    if (can_park_chain_draft(t, s, pick_only)) {
        var rd = metal_draft_step_open(t, s, token, row + 1l, s.n_past)
        if (!rd.gpu_logits) {
            release_step(rd)
            return -1l
        }
        rd.btok = pool_acquire(g_pool, g_dev, 64ul)
        g_v_chain = rd
        g_v_chain_on = true
        g_v_chain_tok = token
        g_v_chain_row = row
        return token
    }
    return -1l if (!metal_mtp_draft_forward(t, s, token, row + 1l, false, s.n_past))
    return parallel_argmax(s.logits, t.config.vocab_size)
}

def private chain_draft_slow(t : Model; var s : Session) {
    chain_draft_drop()
    if (metal_mtp_draft_forward(t, s, g_v_chain_tok, g_v_chain_row + 1l, false, s.n_past)) {
        s.mtp_vbatch[1] = parallel_argmax(s.logits, t.config.vocab_size)
    }
}

[arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-nextn-chain")]
def private chain_draft_land(t : Model; var s : Session) {
    return if (!g_v_chain_on)
    unsafe {
        let tokp = reinterpret<uint?>(metal_buffer_contents(g_v_chain.btok))
        s.mtp_vbatch[1] = int64(tokp[0])
        land_draft_row(t, s, g_v_chain.pos, g_v_chain.cap, g_v_chain.koff, g_v_chain.voff, reinterpret<uint8?>(metal_buffer_contents(g_arena)), null, null)
    }
    g_chain_rounds++
    chain_draft_drop()
}

[unused_argument(t)]
def private metal_mtp_verify_rows_cap(t : Model) : int64 => MTP_MAX_ROWS

[hot_path, arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-round-one-join"), arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-verify-draft-warm")]   //! per round
def private metal_mtp_verify_seat(t : Model; var s : Session; toks : array<int64>; n, pos : int64; pick_only : bool) : bool {   // nolint:STYLE038 — the verify's prep, tape, poke, encode and join share the step's locals
    let c = t.config
    mtp_verify_unpark()
    msel_install()
    finish_pending_step()
    discard_batch_pre()   // the round advances the rows a pre-encoded batch step was built for
    if (n < 2l || n > MTP_MAX_ROWS || !key_exists(g_mirrors, s.uid)) {
        chain_draft_drop()
        return false
    }
    if (g_v_chain_on && (n != 2l || g_v_chain.arena_epoch != g_arena_epoch)) {   //! a parked draft whose arena moved under the verify's acquire holds stale offsets
        chain_draft_slow(t, s)
    }
    let ts_prep = prof_ticks()
    kv_ensure(s, pos + n)
    mtp_verify_prep_n(t, s, toks, n, pos)
    let mr = g_mirrors[s.uid]
    g_v_groups |> clear()
    g_v_groups |> push(MtpVerifyGroup(s = unsafe(addr(s)), tok = toks[0], pos = pos, trunk = pos, nr = n, row0 = 0l,
        koff = mr.koff, voff = mr.voff, cap = mr.cap_rows))
    var r = acquire_step(t, s, pos + 1l, mr.koff, mr.voff, mr.cap_rows, [nrows = n, deepest = pos + 1l])
    var refused = !r.gpu_logits   //! the verify has no CPU classifier fallback: blog and bxf live under gpu_logits
    if (!refused) {
        r.bvarg = pool_acquire(g_pool, g_dev, uint64(max(n, 16l) * 4l))
    }
    if (!refused && r.dn) {
        let cd = dn_conv_dim(c)
        let nvh = c.ssm_dt_rank
        r.n_dn_taped = 0l
        for (l in range64(c.n_layers)) {
            if (layer_is_recurrent(c, l)) {
                r.n_dn_taped++
            }
        }
        r.dn_split = true
        r.tape_slot = uint64(2l * n * cd * 4l + 2l * n * nvh * 4l)
        r.btape = pool_acquire(g_pool, g_dev, r.tape_slot * uint64(r.n_dn_taped))
        r.u_tcd = uniform_u32(uint(n * cd))
        r.u_tnvh = uniform_u32(uint(n * nvh))
        dn_mirror_room(1l)
        let dm = dn_mirror_prepare(s, pos)
        refused = !dm.ok
        if (!refused) {
            var g & = unsafe(g_v_groups[0])
            g.dn_sbase = dm.state_off
            g.dn_cbase = dm.conv_off
            g.dn_salt = dm.state_alt
            g.dn_calt = dm.conv_alt
            g.ple_base = dm.ple_off
            g.tape_base = 0ul
            dn_grp_clear()
            dn_grp_push(0l, n, g.dn_sbase, g.dn_salt, g.dn_cbase, g.dn_calt)
            dn_grp_take(r, 1l, n)
        }
    }
    if (refused) {
        release_step(r)
        chain_draft_drop()
        g_v_groups |> clear()
        return false
    }
    if (r.png) {   //! the rows [tok, drafts] gathered against the n-gram window; the window advances at the commit, not here
        mtp_ple_rows_gather(t, s, n, pos)
        unsafe {
            memcpy(metal_buffer_contents(r.bple_emb), addr < void? >(s.ple_emb_b[0]), uint64(n) * r.bytes_ple_in)
        }
    }
    unsafe {
        let rowb = uint64(c.dim * 4l)
        memcpy(metal_buffer_contents(r.bx), addr < void? >(s.mtp_cat[0]), uint64(n) * rowb)
        memcpy(metal_buffer_contents(r.bcos), addr < void? >(s.rope_cos[0]), r.bytes_tab)
        memcpy(metal_buffer_contents(r.bsin), addr < void? >(s.rope_sin[0]), r.bytes_tab)
        memcpy(metal_buffer_contents(r.bcat), addr < void? >(s.mtp_cat[0]), uint64(n) * rowb)
        if (r.hc) {
            memcpy(metal_buffer_contents(r.bhc_carry), addr < void? >(s.mtp_xb_save[0]), r.bytes_hcd)
        } else {
            memcpy(reinterpret<void?>(reinterpret<uint8?>(metal_buffer_contents(r.bcat)) + uint64(n) * rowb), addr < void? >(s.mtp_xb_save[0]), rowb)
        }
    }
    prof_add("mtp.verify.poke", ts_prep)
    let ts_enc = prof_ticks()
    encode_verify_step(t, r)
    prof_add("mtp.verify.enc", ts_enc)
    if (!finish_verify_step(t, r, !pick_only)) {
        release_step(r)
        chain_draft_drop()
        if (c.recr_mask != 0ul) {
            dn_mirror_drop(s.uid)
        }
        g_v_groups |> clear()
        return false
    }
    chain_draft_land(t, s)
    metal_verify_land(t, s, r, n, pos, pick_only)
    return true
}

[arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-round-one-join")]
def private metal_verify_land(t : Model; var s : Session; var r : StepRes; n, pos : int64; pick_only : bool) {
    let c = t.config
    let vocab = c.vocab_size
    let hd = mtp_h_dim(c)
    let ts_land = prof_ticks()
    s.mtp_picks |> resize(n)
    unsafe {
        let ap = reinterpret<uint?>(metal_buffer_contents(r.bvarg))
        for (i in range64(n)) {
            s.mtp_picks[i] = int64(ap[i])
        }
    }
    s.mtp_hrows |> overwrite_resize(n * hd)
    unsafe {
        memcpy(addr < void? >(s.mtp_hrows[0]), metal_buffer_contents(r.hc ? r.bhc_save : r.bxf), uint64(n * hd * 4l))
    }
    copy_floats(s.mtp_hrows, (n - 1l) * hd, s.mtp_h, 0l, hd)
    s.mtp_h_pos1 = pos + n
    if (!pick_only) {
        copy_floats(s.mtp_logits_b, (n - 1l) * vocab, s.logits, 0l, vocab)
    }
    if (r.dn) {
        dn_mirror_flip(s.uid, pos + n)
    }
    if (r.png) {
        let rb = uint64(long_length(s.ple_conv_hist)) * 4ul
        g_v_ring_bytes = rb
        g_v_ring_save = pool_acquire(g_pool, g_dev, rb)
        g_v_ring_u = uniform_u32(uint(long_length(s.ple_conv_hist)))
        ple_ring_copy(true)
        commit_ple_rows(t, r, 0l, n)
        mtp_ple_window_commit(t, s, n, pos)
    }
    g_mirrors[s.uid].watermark = pos + n
    g_v_rd = r
    g_v_parked = true
    g_v_park_uid = s.uid
    g_v_park_pos = pos
    g_v_park_n = n
    prof_add("mtp.verify.land", ts_land)
}

def private ple_ring_copy(save : bool) {
    let g = g_v_groups[0]
    let n = long_length(g.s.ple_conv_hist)
    var cb = metal_new_command_buffer_unretained(g_queue)
    var enc = metal_new_compute_encoder(cb)
    if (save) {
        enc_copy_row(enc, dn_pool_buffer(), g.ple_base, g_v_ring_save, 0ul, g_v_ring_u, n)
    } else {
        enc_copy_row(enc, g_v_ring_save, 0ul, dn_pool_buffer(), g.ple_base, g_v_ring_u, n)
    }
    metal_end_encoding(enc)
    metal_release(enc)
    metal_commit(cb)
    metal_wait_until_completed(cb)
    metal_release(cb)
}

[hot_path, arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-dn-shadow-replay")]   //! per rejected round
def private metal_mtp_rollback_seat(t : Model; var s : Session; pos, a : int64) : bool {
    return false if (!g_v_parked || g_v_park_uid != s.uid || g_v_park_pos != pos)
    if (g_v_rd.dn) {
        encode_dn_replay(t, g_v_rd, 0l, a + 1l)
        dn_mirror_set_pos(s.uid, pos + a + 1l)
    }
    if (g_v_rd.png) {
        ple_ring_copy(false)
        commit_ple_rows(t, g_v_rd, 0l, a + 1l)
        mtp_ple_window_restore(s)
        mtp_ple_window_commit(t, s, a + 1l, pos)
    }
    g_mirrors[s.uid].watermark = pos + a + 1l
    mtp_verify_unpark()
    return true
}

// DASLLAMA_METAL_DECODE_CONCURRENT (default ON): concurrent encoder for the single-stream step,
// barriers only at tracker-detected hazards (hz_gate in every decode-path enc_* helper); g_skip
// knockouts force serial — skipped dispatches break the declared graph
var private g_dconc = -1

def private decode_concurrent : bool => pin_over_env(g_dconc, g_env_metal.decode_concurrent)

//! Concurrent-encoder rail for the single-stream decode: 1 forces on, 0 forces serial,
//! negative restores the env default (`DASLLAMA_METAL_DECODE_CONCURRENT`).
def public set_metal_decode_concurrent(v : int) {
    g_dconc = v < 0 ? -1 : (v != 0 ? 1 : 0)
}

// DASLLAMA_METAL_FUSE (default ON): every sandwich seam as one residual step (`MetalPreAddRms` / `MetalPreAdd`) - off
// restores the split chain (the branch's post norm, the add, the next norm) for A/B
var private g_dfuse = -1

def private decode_fuse_pre : bool => pin_over_env(g_dfuse, g_env_metal.fuse)

var private g_dfuse_said = -1   //! the fuse state the announce last saw: -1 none, 0 off, 1 on (on prints nothing)

//! the fuse knob's off announce, once per state: the split chain reaches the one-row step and every batched row step
def private decode_fuse_say {
    let on = decode_fuse_pre()
    let said = g_dfuse_said
    g_dfuse_said = on ? 1 : 0
    return if (on || said == 0)
    to_log(LOG_WARNING, "dasLLAMA metal decode: every decode sandwich seam runs the split chain (the branch's post norm, the add, the next norm) on the one-row step and every batched row step, as asked (set_metal_decode_fuse, DASLLAMA_METAL_FUSE=0)\n")
}

//! Epilogue-fusion rail for the one-row step and the batched rows: 1 forces on, 0 forces the separate
//! per-op dispatches, negative restores the env default (`DASLLAMA_METAL_FUSE`).
def public set_metal_decode_fuse(v : int) {
    g_dfuse = v < 0 ? -1 : (v != 0 ? 1 : 0)
    decode_fuse_say()
}

//! The rail `set_metal_decode_fuse` set: 1 on, 0 off, -1 the env default - what a caller that sets it puts back
def public get_metal_decode_fuse : int => g_dfuse

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
        enc_argmax(enc, prev_blog, r.btok, r.u_vocab)
        enc_embed_dev(enc, t, r.btok, 0ul, r.bx, 0ul, 1l, r.u_dim, r.u_embscale)
        if (r.ple_dev) {
            enc_ple_pre_dev(enc, t, r.btok, 0ul, r.bx, r.bple, r.bple_proj, r.u_ple_all, r.u_ple_sq, 1l)
        }
    } elif (r.btok_in != null) {
        enc_embed_dev(enc, t, r.btok_in, 0ul, r.bx, 0ul, 1l, r.u_dim, r.u_embscale)   //! the poked token's row, gathered on the device
        if (r.ple_dev) {
            enc_ple_pre_dev(enc, t, r.btok_in, 0ul, r.bx, r.bple, r.bple_proj, r.u_ple_all, r.u_ple_sq, 1l)
        }
    }
    // layer-0 attention norm — the per-layer trailing add+rms is fused and feeds the NEXT layer; a hyper-connection model makes the wide residual instead (each layer opens with its own mixer)
    if (g_skip != "ew") {
        if (r.hc) {
            for (ch in range64(c.hc_count)) {
                enc_copy_row(enc, r.bx, 0ul, r.bhc, uint64(ch * dim * 4l), r.u_dim, dim)
            }
        } else {
            var bw_att0 = upload_region(unsafe(addr < void? >(t.fblob[t.rms_att_off])), uint64(dim * 4l))
            enc_rms(enc, r.bx, 0ul, bw_att0, r.bxb, r.u_dim, r.u_eps)
        }
    }
    var dnli = 0l   // recurrent-layer index — addresses the dn state/conv mirror slices in order
    for (l in range64(c.n_layers)) {
        encode_layer(t, r, enc, l, dnli)
    }
    if (r.gpu_logits) {
        encode_cls_tail(t, r, enc, t.rms_final_off)
        if (r.pick_ask) {
            enc_argmax(enc, r.blog_zero_copy != null ? r.blog_zero_copy : r.blog, r.bpick, r.u_vocab)
        }
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
        if (g_step_trace_cap > 0l) {   // an unarmed trace leaves g_step_trace_t0 at 0 — the stamp would read the epoch
            r.t_commit = int64(get_time_usec(g_step_trace_t0))
        }
    }
    r.cb = cb
}

// wait for the step's cb, then hand results back: s.x, the mirrored KV row VERBATIM into the CPU
// cache (the CPU never re-encodes), s.logits when the GPU classifier ran, watermark advance
def private finish_step(t : Model; var s : Session; r : StepRes) : bool {
    let ts_wait = ref_time_ticks()
    if (g_wait_spin) {
        while (metal_command_buffer_gpu_end_time(r.cb) == 0.0lf && get_time_usec(ts_wait) < WAIT_SPIN_CAP_US) {   // clock: control — the spin's 200 ms bound, not a reported time
        }
    }
    metal_wait_until_completed(r.cb)
    let ok = !metal_command_buffer_failed(r.cb)
    g_msel_landed = ok ? r.sel_rows : 0l
    g_gpu_ms += (metal_command_buffer_gpu_end_time(r.cb) - metal_command_buffer_gpu_start_time(r.cb)) * 1000.0lf
    let wait_us = int64(get_time_usec(ts_wait))
    g_us_wait += wait_us
    if (ok) {
        let ts_rb = ref_time_ticks()
        let c = t.config
        let pos = r.pos
        let cap = r.cap
        unsafe {
            memcpy(addr < void? >(s.x[0]), metal_buffer_contents(r.bx), r.bytes_x)
            if (r.hc) {   // the wide residual is the session's state the CPU path would continue from
                memcpy(addr < void? >(s.hc_res[0]), metal_buffer_contents(r.bhc), r.bytes_hcd)
            }
            let ab = reinterpret<uint8?>(metal_buffer_contents(g_arena))
            let prow = kv_phys_row(s, pos)
            for (l in range64(c.n_layers)) {
                continue if (kv_shared_layer(t, l))   // no rows of its own — the source's copy covers it
                let rowb = kv_row_bytes(s.kv_dtype_k, layer_kv_dim(c, l))
                let lb = mirror_lbase(t, s.kv_dtype_k, cap, l)
                mirror_row_writeback(s, c.seq_len, l, kv_ring_row(pos, mirror_ring(t, l)), prow, rowb, lb, ab, r.koff, r.voff)
            }
            let pick = r.pick_ask && s.pick_asked
            if (r.gpu_logits && r.blog_zero_copy == null && !pick) {   // zero-copy steps: the GPU already wrote s.logits
                memcpy(addr < void? >(s.logits[0]), metal_buffer_contents(r.blog), r.bytes_log)
            }
            if (pick) {
                land_pick(s, int64(reinterpret<uint?>(metal_buffer_contents(r.bpick))[0]))
                g_picks_landed++
            }
        }
        if (r.gpu_logits && (c.n_layer_nextn > 0l || g_mtp_carry_hidden)) {
            if (r.hc) {   // the draft head's carry is the wide residual before the head mixer
                stash_mtp_carry(s, metal_buffer_contents(r.bhc), r.bytes_hcd, r.pos + 1l)
            } else {
                stash_mtp_carry(s, metal_buffer_contents(g_mtp_carry_pre && c.n_layer_nextn <= 0l ? r.bx : r.bxf), r.bytes_x, r.pos + 1l)
            }
        }
        if (r.gpu_logits) {
            decode_override_logits_done()
        }
        g_mirrors[s.uid].watermark = r.pos + 1l
        if (r.dn) {   //! the region the step wrote holds pos's update: it is live, the mirror at pos + 1
            dn_mirror_land(s.uid, r.dn_wr, r.pos + 1l)
        }
        g_us_readback += int64(get_time_usec(ts_rb))
        g_decodes++
        if (long_length(g_step_trace) < g_step_trace_cap) {
            g_step_trace |> push(MetalDecodeStepTrace(pos = r.pos, mode = r.trace_mode,
                commit_us = r.t_commit, finish_us = int64(get_time_usec(g_step_trace_t0)),
                wait_us = wait_us,
                ks = metal_command_buffer_kernel_start_time(r.cb),
                ke = metal_command_buffer_kernel_end_time(r.cb),
                gs = metal_command_buffer_gpu_start_time(r.cb),
                ge = metal_command_buffer_gpu_end_time(r.cb)))
        }
    } else {
        to_log(LOG_ERROR, "dasLLAMA metal decode: dispatch failed ({cb_error_text(r.cb)}) - falling back to the CPU layer loop\n")   // nolint:PERF028,PERF026 — error path, and the CPU fallback dwarfs the log
        decline(MetalDecodeDecline.gpu_error)
    }
    return ok
}

def private release_step(var r : StepRes) {   // nolint:STYLE037,STYLE038 — flat one-release-per-buffer list
    if (r.cb != null) {
        metal_release(r.cb)
        r.cb = null
    }
    if (r.btape != null) {
        pool_release(g_pool, r.btape, r.tape_slot * uint64(r.n_dn_taped))
        pool_release(g_pool, r.u_tcd, 4ul)
        pool_release(g_pool, r.u_tnvh, 4ul)
        r.btape = null
    }
    if (r.spec || (r.draft && r.btok != null)) {
        pool_release(g_pool, r.btok, 64ul)
    }
    if (r.bpick != null) {
        pool_release(g_pool, r.bpick, 64ul)
    }
    if (r.bvarg != null) {
        pool_release(g_pool, r.bvarg, uint64(max(r.nrows, 16l) * 4l))
    }
    if (r.btok_in != null) {
        pool_release(g_upool, r.btok_in, 4ul)
    }
    pool_release(g_pool, r.bx, r.bytes_x)
    pool_release(g_pool, r.bxb, r.bytes_xb)
    pool_release(g_pool, r.bxb2, r.bytes_x)
    pool_release(g_pool, r.bqkv, r.bytes_qkv)
    pool_release(g_pool, r.bh12, r.bytes_h2)
    if (r.bh12s != null) {
        pool_release(g_pool, r.bh12s, r.bytes_h12s)
    }
    if (r.bxh != null) {
        pool_release(g_pool, r.bxh, r.bytes_xh)
        pool_release(g_pool, r.bfgh, r.bytes_fgh)
    }
    pool_release(g_upool, r.bcos, r.bytes_tab)
    pool_release(g_upool, r.bsin, r.bytes_tab)
    if (r.bcos_swa != null) {
        pool_release(g_upool, r.bcos_swa, r.bytes_tab_sw)
        pool_release(g_upool, r.bsin_swa, r.bytes_tab_sw)
    }
    if (r.gpu_logits) {
        pool_release(g_pool, r.bxf, r.bytes_x)
        if (r.blog != null) {   // blog_zero_copy steps have no staging; blog_zero_copy itself is registry-owned
            pool_release(g_pool, r.blog, r.bytes_log)
        }
        pool_release(g_upool, r.u_vocab, 4ul)
    }
    if (r.attn_d) {
        pool_release(g_pool, r.bpart, r.bytes_part)
        if (!r.verify) {   //! the verify's route table is its group's, released with it
            pool_release(g_upool, r.brt, ROUTE_ROW_BYTES)
        }
    } elif (r.chunked) {
        pool_release(g_pool, r.bpart, r.bytes_part)
        pool_release(g_upool, r.u_nch, 4ul)
    }
    if (r.bcat != null) {
        pool_release(g_pool, r.bcat, 2ul * r.bytes_x)
        pool_release(g_upool, r.u_dim2, 4ul)
    }
    if (r.verify) {
        pool_release(g_upool, r.u_total_dim, 4ul)
        pool_release(g_upool, r.u_total_h, 4ul)
        pool_release(g_upool, r.u_total_qd, 4ul)
    }
    pool_release(g_upool, r.u_dim, 4ul)
    pool_release(g_upool, r.u_qd, 4ul)
    pool_release(g_upool, r.u_kvd, 4ul)
    pool_release(g_upool, r.u_qkvd, 4ul)
    pool_release(g_upool, r.u_hidden, 4ul)
    if (r.u_hidden2 != null) {
        pool_release(g_upool, r.u_hidden2, 4ul)
    }
    pool_release(g_upool, r.u_hs, 4ul)
    pool_release(g_upool, r.u_cnt, 4ul)
    pool_release(g_upool, r.u_neox, 4ul)
    pool_release(g_upool, r.u_hasb, 4ul)
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
    if (r.hetero) {
        pool_release(g_upool, r.u_hs_sw, 4ul)
        pool_release(g_upool, r.u_kvd_sw, 4ul)
        pool_release(g_upool, r.u_qd_sw, 4ul)
        pool_release(g_upool, r.u_qkvd_sw, 4ul)
        pool_release(g_upool, r.u_scale_sw, 4ul)
    }
    if (r.u_embscale != null) {
        pool_release(g_upool, r.u_embscale, 4ul)
    }
    if (r.bsupp != null) {
        pool_release(g_upool, r.bsupp, r.bytes_supp)
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
            pool_release(g_upool, r.u_moe_shtot, 4ul)
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
        pool_release(g_pool, r.bdn_b, 2ul * r.bytes_dn_h)
        pool_release(g_pool, r.bdn_g, r.bytes_dn_h)
        pool_release(g_upool, r.u_cd, 4ul)
        pool_release(g_upool, r.u_nvh, 4ul)
        pool_release(g_upool, r.u_nvh2, 4ul)
        pool_release(g_upool, r.u_di, 4ul)
        if (r.bdn_grp != null) {
            pool_release(g_upool, r.bdn_grp, r.bytes_dn_grp)
            r.bdn_grp = null
        }
        r.bdn_state = null   // mirror-owned
    }
    if (r.bdn_qg != null) {
        pool_release(g_pool, r.bdn_qg, 2ul * r.bytes_q)
        pool_release(g_pool, r.bdn_gate, r.bytes_q)
        pool_release(g_upool, r.u_dn_qd2, 4ul)
    }
    if (r.ple) {
        ple_step_release(r)
        pool_release(g_upool, r.u_ple, 4ul)
    }
    if (r.hc) {
        let rows = uint64(r.nrows <= 1l ? 1l : ((r.nrows + 3l) / 4l) * 4l)
        pool_release(g_pool, r.bhc, rows * r.bytes_hcd)
        pool_release(g_pool, r.bhc_xn, rows * r.bytes_hcd)
        pool_release(g_pool, r.bhc_gate, rows * r.bytes_hcd)
        pool_release(g_pool, r.bhc_lo, rows * r.bytes_lr)
        pool_release(g_pool, r.bhc_inj_a, rows * r.bytes_hc)
        pool_release(g_pool, r.bhc_inj_f, rows * r.bytes_hc)
        if (r.bhc_save != null) {
            pool_release(g_pool, r.bhc_save, rows * r.bytes_hcd)
            pool_release(g_pool, r.bhc_carry, rows * r.bytes_hcd)
            pool_release(g_pool, r.bhc_cat, rows * 2ul * r.bytes_hcd)
        }
        pool_release(g_upool, r.u_hcd, 4ul)
        pool_release(g_upool, r.u_hc, 4ul)
        pool_release(g_upool, r.u_lr, 4ul)
        pool_release(g_upool, r.u_lr_tot, 4ul)
        pool_release(g_upool, r.u_hcd_tot, 4ul)
    }
    if (r.png) {
        let rows = uint64(r.nrows <= 1l ? 1l : ((r.nrows + 3l) / 4l) * 4l)
        pool_release(g_upool, r.bple_emb, rows * r.bytes_ple_in)
        pool_release(g_pool, r.bple_key, rows * r.bytes_hcd)
        pool_release(g_pool, r.bple_gated, rows * r.bytes_hcd)
        if (r.bple_cin != null) {
            pool_release(g_pool, r.bple_cin, rows * r.bytes_hcd)
        }
        pool_release(g_pool, r.bple_val, r.bytes_x)
        pool_release(g_upool, r.u_ple_in, 4ul)
    }
    r.valid = false
}

//! the wait's spin bound: a step past it is a stall, and the blocking wait behind the spin reports it
let private WAIT_SPIN_CAP_US = 200000
var private g_wait_spin = true         //!< the step wait spins on the GPU end time before the blocking wait (DASLLAMA_METAL_WAIT_SPIN)

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
        if (metal_command_buffer_failed(r.cb)) {
            to_log(LOG_ERROR, "dasLLAMA metal decode: discarded pre-step failed ({cb_error_text(r.cb)})\n")   // nolint:PERF028,PERF026 — error path on a discarded pre-step
        }
    }
    release_step(r)
}

//! Both drivers' pre-encoded steps retired: the single-row chain's and the batch driver's.
[cold_path]   // a mirror move or upload, reached through the prefill's quiesce hook
def private discard_pre_encoded_steps {
    discard_pre()
    discard_batch_pre()
    mtp_verify_unpark()
    chain_draft_drop()
}

// Pre-encode the PREDICTED next step (same session, pos + 1) while the current step runs on the
// GPU. When speculation gates pass, the cb commits AND chains itself (argmax + embed gather, no
// CPU poke); otherwise it stays uncommitted. Quietly declines when gates fail — slow path serves those.
[arch(at="../ARCHITECTURE_GPU_MTP_DECODE_PICK.md#greedy-chain")]
def private pre_encode_next(t : Model; s : Session; npos : int64; var prev_blog : MetalBuffer?; prev_dn_wr : int64) {
    if (decode_decline(t, s, npos) != MetalDecodeDecline.none || !key_exists(g_mirrors, s.uid)) {
        return
    }
    //! a recurrent step reads the region the step in flight writes and writes the next one, so a miss leaves the state it read
    //! whole; the n-gram ring has one region, so its models never chain
    let dn = t.config.recr_mask != 0ul
    if (dn && (prev_dn_wr < 0l || has_ple_ngram(t.config) || dn_mirror_cur_at(s.uid, npos - 1l) < 0l)) {
        return
    }
    let mr = g_mirrors[s.uid]   // cheap copy, capacity check ONLY — row npos-1 is in flight on the GPU
    if (mr.cap_rows < npos + 1l || mr.kdt != s.kv_dtype_k || mr.vdt != s.kv_dtype_v) {
        return
    }
    let ts_enc = ref_time_ticks()
    var r = acquire_step(t, s, npos, mr.koff, mr.voff, mr.cap_rows)
    if (dn) {
        let rd = dn_mirror_region_offs(s.uid, prev_dn_wr)
        let wr = dn_mirror_region_offs(s.uid, dn_region_next(prev_dn_wr))
        r.bdn_state = dn_pool_buffer()
        r.dn_split = true
        r.dn_rd = prev_dn_wr
        r.dn_wr = dn_region_next(prev_dn_wr)
        r.dn_epoch = g_dn_pool_epoch
        dn_grp_clear()
        dn_grp_push(0l, 1l, rd.state_off, wr.state_off, rd.conv_off, wr.conv_off)
        dn_grp_take(r, 1l, 1l)
    }
    if (r.gpu_logits && prev_blog != null && spec_cls_capable(t) &&
            (!r.ple || r.ple_dev) && !s.sampled && spec_armed()) {
        // speculative chain: the cb runs the moment the previous one drains — CPU tail/sampling/next forward() all overlap GPU execution
        r.spec = true
        // a spec step is IN FLIGHT after forward() returns — must not touch session memory (staging only; acceptance lands it via finish_step)
        r.blog_zero_copy = null
        r.btok = pool_acquire(g_pool, g_dev, 64ul)
        build_decode_rope_row(t, npos + s.rope_pos_delta, g_rope_cos_next, g_rope_sin_next)   // the delta keeps this bit-identical to forward()'s row for post-image mrope sessions
        unsafe {
            memcpy(metal_buffer_contents(r.bcos), addr < void? >(g_rope_cos_next[0]), r.bytes_tab)
            memcpy(metal_buffer_contents(r.bsin), addr < void? >(g_rope_sin_next[0]), r.bytes_tab)
        }
        if (r.bcos_swa != null) {
            build_decode_rope_row_swa(t, npos + s.rope_pos_delta, g_rope_cos_next_swa, g_rope_sin_next_swa)
            unsafe {
                memcpy(metal_buffer_contents(r.bcos_swa), addr < void? >(g_rope_cos_next_swa[0]), r.bytes_tab_sw)
                memcpy(metal_buffer_contents(r.bsin_swa), addr < void? >(g_rope_sin_next_swa[0]), r.bytes_tab_sw)
            }
        }
        pick_arm(s, r)
        encode_step(t, r, true, prev_blog)
    } else {
        pick_arm(s, r)
        encode_step(t, r, false)
    }
    g_pre = r
    g_us_encode += int64(get_time_usec(ts_enc))
}

var private g_picks_landed = 0l

//! How many single-row steps landed their pick in the logits' place under the caller's pick ask since load (a cell's witness)
def public metal_picks_landed() : int64 => g_picks_landed

[arch(at="../ARCHITECTURE_GPU_MTP_DECODE_PICK.md#greedy-chain")]
def private pick_arm(s : Session; var r : StepRes) {
    return if (!s.pick_asked || !r.gpu_logits)
    r.pick_ask = true
    r.bpick = pool_acquire(g_pool, g_dev, 64ul)
}

// the non-pipelined step: mirror catch-up + encode-now + wait; the NEXT step's pre-encode still
// overlaps this step's GPU run. Serves window starts, prediction misses, and speculation reruns.
def private metal_decode_slow(t : Model; var s : Session; token, pos : int64) : bool {
    let ts_all = ref_time_ticks()
    let mir = mirror_prepare(t, s, pos)
    if (!mir.ok) {
        decline(MetalDecodeDecline.mirror)
        return false
    }
    var r = acquire_step(t, s, pos, mir.koff, mir.voff, mir.cap)
    if (r.dn) {
        var dm = dn_mirror_prepare(s, pos)
        if (!dm.ok) {   // state not resident and no CPU prefix to sync from — decline this step
            release_step(r)
            decline(MetalDecodeDecline.dn_state)
            return false
        }
        r.bdn_state = dm.buf
        r.ple_ring_base = dm.ple_off
        r.dn_wr = dn_mirror_cur_at(s.uid, pos)
        r.dn_rd = r.dn_wr
        r.dn_epoch = g_dn_pool_epoch
        dn_grp_clear()
        dn_grp_push(0l, 1l, dm.state_off, dm.state_off, dm.conv_off, dm.conv_off)
        dn_grp_take(r, 1l, 1l)
    }
    poke_step(s, r, token)
    pick_arm(s, r)
    g_us_setup += int64(get_time_usec(ts_all))
    let ts_enc = ref_time_ticks()
    encode_step(t, r, true)
    g_us_encode += int64(get_time_usec(ts_enc))
    pre_encode_next(t, s, pos + 1l, r.blog, r.dn ? r.dn_wr : -1l)
    let ok = finish_step(t, s, r)
    release_step(r)
    return ok
}

//! The DecodeOverrideFn: runs one token step's whole llama layer stack GPU-resident against the
//! session's KV mirror. true = done (KV row stored CPU-side too, s.x = final residual, s.logits
//! when the GPU classifier gates pass); false = declined, the caller runs the CPU layer loop.
def metal_decode_forward(t : Model; var s : Session; token, pos : int64) : bool {
    // mixed batch/single schedules: a pipelined batch step may still own this session's KV row
    // and logits — land it before reading anything session-side (no-op when nothing pends)
    finish_pending_step()
    mtp_verify_unpark()
    chain_draft_drop()
    msel_install()
    g_land_err = false
    discard_batch_pre()
    var why = decode_decline(t, s, pos)
    if (why == MetalDecodeDecline.none && !metal_decode_init()) {
        why = MetalDecodeDecline.device
    }
    if (why != MetalDecodeDecline.none) {
        discard_pre()   // the sequence diverged from the prediction
        if (why == MetalDecodeDecline.feature) {
            decline(why, metal_missing_needs(t, DECODE_NEEDS_OK))
        } else {
            decline(why, metal_fmt_decline_say("decode", why, t))
        }
        return false
    }
    residency_flush()   // pin/commit before the step's submission (the post-CPU-window slack)
    if (!(g_pre.valid && g_pre.uid == s.uid && g_pre.pos == pos &&
            g_pre.kdt == s.kv_dtype_k && g_pre.s16 == t.wscale_f16 &&
            g_pre.arena_epoch == g_arena_epoch &&
            (!g_pre.dn || (g_pre.dn_epoch == g_dn_pool_epoch && dn_mirror_cur_at(s.uid, pos) == g_pre.dn_rd)))) {
        discard_pre()
        return metal_decode_slow(t, s, token, pos)
    }
    // FAST path: this step's cb was encoded during the previous step's GPU run. A spec cb has
    // been running since the previous step drained (GPU-chained argmax + embed); a poked-mode
    // one needs the embed + rope bytes poked, then commits.
    let ts_all = ref_time_ticks()
    var r = g_pre
    g_pre.valid = false
    r.trace_mode = r.spec ? 2l : 1l
    if (!r.spec) {
        poke_step(s, r, token)
        metal_commit(r.cb)
        r.committed = true
        if (g_step_trace_cap > 0l) {   // same unarmed-trace guard as encode_step's commit
            r.t_commit = int64(get_time_usec(g_step_trace_t0))
        }
    }
    g_tick++
    g_mirrors[s.uid].tick = g_tick   // keep the live session hot for the LRU
    g_us_setup += int64(get_time_usec(ts_all))
    pre_encode_next(t, s, pos + 1l, r.blog, r.dn ? r.dn_wr : -1l)
    let ok = finish_step(t, s, r)
    if (ok && r.spec) {
        // verify the GPU's greedy pick against the caller's actual token — a miss means this whole
        // step ran on the wrong embedding: drop the chained pre-step and rerun (the garbage KV row
        // sits at/above the rewound watermark, so mirror_prepare's re-upload repairs it).
        let tokp = unsafe(reinterpret<uint?>(metal_buffer_contents(r.btok)))
        let picked = int64(unsafe(tokp[0]))
        if (picked != token) {
            g_spec_misses++
            g_spec_cooldown = g_spec_backoff
            g_spec_backoff = min(g_spec_backoff * 2, 256)
            if (r.dn) {   //! the region the step read still holds the state at pos: it is live again
                dn_mirror_land(s.uid, r.dn_rd, pos)
            }
            discard_pre()
            release_step(r)
            g_decodes--   // finish_step counted the mispredicted step; the rerun recounts
            s.pick_ready = false
            return metal_decode_slow(t, s, token, pos)
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
// the step splits into DASLLAMA_METAL_BATCH_NCB command buffers: the
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
let private MV_K_ALIGN = 256l
var private g_mtp_carry_hidden = false
var private g_mtp_carry_pre = false

//! The assistant drafter (dasllama_metal_mtp_gemma) attaches by asking the decode to carry the
//! final hidden of every step in s.mtp_h - the same stash the NextN carriers get for free. `pre`
//! picks the residual before output_norm over the normed hidden.
def set_metal_mtp_carry_hidden(on : bool; pre : bool = false) {
    g_mtp_carry_hidden = on
    g_mtp_carry_pre = pre
    discard_batch_pre()   // a step encoded under the other carry would stash the wrong hidden
}

def metal_mtp_carry_pre() : bool => g_mtp_carry_pre

var private g_b_same_toks : MetalBuffer?
var private g_b_emb_gathered = 0l
var private g_b_gather = true

//! The batched steps' rows whose embed row the device gathered off the row's id, in this process (a cell's served witness)
def metal_batch_rows_gathered() : int64 => g_b_emb_gathered

//! the batched step gathers its rows off their ids (on, the default), or takes the rows the host embeds and pokes up (the A/B control)
def set_metal_batch_gather(on : bool) {
    g_b_gather = on
    if (!on) {
        to_log(LOG_WARNING, "dasLLAMA metal batch decode: the batched rows take the rows the host embeds, not the device gather off their ids, as asked (set_metal_batch_gather)\n")
    }
    discard_batch_pre()   // a step encoded under the other form would read the other rows
}

def metal_batch_gather() : bool => g_b_gather

[hot_path, unused_argument(s, npos, pos)]
def private metal_batch_embed_gate(t : Model; s : Session; npos, pos : int64) : bool => g_b_gather && emb_dev_gather_ok(t)

//! encodes the drafts that write the same-slab rows' ids onto the verify's own command buffer
typedef BatchSameSlabChain = function<(t : Model; enc : MetalComputeEncoder?) : void>

var private g_b_same_chain : BatchSameSlabChain
var private g_b_same_chain_on = false

//! The same-slab verify's rows come as a GPU token buffer (the draft chain's argmax outputs, never
//! read back): the driver dequantizes their embeddings on the encoder instead of taking the
//! workspace's CPU-embedded rows, and lands every row's argmax for the accept walk. null = CPU rows.
def set_batch_same_slab_tokens(var b : MetalBuffer?) {
    g_b_same_toks = b
    g_b_same_chain_on = false
}

//! The same-slab rows' ids in `b` are written by drafts the verify itself encodes: `chain` runs at the head of the verify's
//! first command buffer, so the draft and the verify are one commit and one join (`metal_mtp_chained_rounds` counts the steps)
[arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-round-one-join")]
def set_batch_same_slab_chain(var b : MetalBuffer?; chain : BatchSameSlabChain) {
    g_b_same_toks = b
    g_b_same_chain = chain
    g_b_same_chain_on = true
}

var private g_bconcurrent = true
// DASLLAMA_METAL_BATCH_FUSE: fold w1 + w3 + swiglu into one fused dispatch on the fixed-B path
// (B <= 4) — cuts 2 dispatches + their barriers per layer where inter-dispatch bubbles hurt most.
// Default ON; =0 pins the unfused triple (the A/B rail).
var private g_bfuse = true

// the mul_mv-shape GEMV (MetalQ8MvB2/B4 — the Part-3 winner, in-graph == the upstream
// reference) at every fixed-B site instead of the x-staged forms (which measured -10.5% in-graph:
// the walkthrough lab win was SLC-inflated). DASLLAMA_METAL_BATCH_MV=0 = the x-staged A/B rail.
var private g_bmv = true

// B = 5..8 rides two 4-column mv tiles (weights stream twice) instead of the GEMM + split-K
// policy — the reference's own shape below B = 9. Measured +12.7% at B=8 (239.4 vs 212.5 t/s).
// DASLLAMA_METAL_BATCH_MV8=0 = the GEMM-policy A/B rail.
var private g_bmv8 = true

// pin the batched residual step's split chain (the rms, ew2-add, rms_b) - the tests' parity rail for the split form
var private g_b_addrms_unfused = false

def public set_metal_batch_addrms_unfused(v : bool) {
    g_b_addrms_unfused = v
    discard_batch_pre()   // a step encoded under the other form would serve it one step late
}

//! pin the gemma-4 MoE tail to its unfused chain (the routed combine, the three post norms, the add and the next add-norm), the
//! tests' control for the one-step form (`MetalMoeCombG4`); returns the pin it replaced
def public set_metal_moe_g4_unfused(v : bool) : bool {
    let prior = g_moe_g4_unfused
    g_moe_g4_unfused = v
    if (v) {
        to_log(LOG_WARNING, "dasLLAMA metal decode: the gemma-4 MoE tail runs its unfused chain, not the one-step form, as asked (set_metal_moe_g4_unfused)\n")
    }
    discard_batch_pre()
    return prior
}

// one-deep encode-ahead for the batch driver: a step commits BEFORE the previous is waited on, so
// sessions hold the PREVIOUS step's logits until the next call. WRONG for logit-gated sampling
// (eval_batch's contract) — OPT-IN for benches only: DASLLAMA_METAL_BATCH_PIPE=1, gpu-logits only.
var private g_b_pipe = false
// per-step scratch reused across calls (row table image + per-row mirror slices)
var private @scratch g_b_rte : array<uint>
var private @scratch g_b_koffs : array<uint64>
var private @scratch g_b_voffs : array<uint64>
var private @scratch g_b_caps : array<int64>
//! the step being built: each recurrent group's live state base, and the group count (a row a session, or the same-slab rows as one)
var private @scratch g_b_dn_soffs : array<uint64>
var private g_b_dn_ng = 0l
var private @scratch g_b_ple_bases : array<uint64>   //! each row's n-gram conv ring inside its session's mirror (qwen4exp)

// the model half is decode_shape_decline; per batch: uniform f16/f32 KV, every row within the
// attention depth cap with a real uid, per-row rope rows built, and dim within the add+rms row
// slabs (GEMM path caps at 3072, fixed-B GEMV at ADD_RMS_MAX_DIM — B >= 5 on a wider model declines)
def private batch_decode_decline(t : Model; ws : BatchWorkspace; sessions : array<Session?>; nrows : int64) : MetalDecodeDecline {
    //! the deltanet hybrids ride the recurrent rows form where the single row does (decode_shape_decline's graph clause); a MoE's shared expert rides the rows form
    let shape = decode_shape_decline(t, BATCH_NEEDS_OK, true, true, true, allow_hc = !batch_same_slab())   //! MoE, the E-series' shared-KV layers (Q-only rows), its two FFN widths and the hyper-connection rows (one session a row) all have batch forms
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
    // rows are built at the ROTATED width (partial rope: rope_dim/2), dual-rope rows per class at ITS width
    let rot = c.rope_dim > 0l ? c.rope_dim : c.head_size
    let swa_hs = c.head_size_swa > 0l ? c.head_size_swa : c.head_size
    if (long_length(ws.scr.rope_cos) < nrows * (rot / 2l) ||
            (has_dual_rope(c) && long_length(ws.scr.rope_cos_swa) < nrows * (swa_hs / 2l))) {
        return MetalDecodeDecline.rope_rows
    }
    return MetalDecodeDecline.none
}

//! One batch-shaped weight site: a K-quant plane rides its per-stream GEMV, a q8 site the tier's
//! form - tensor GEMM, the mv form where this site's k is aligned (mv_ok), split-K (sk > 0), the
//! 64-wide or plain tile GEMM, or the row GEMVs. y lands at column ycol of a ys-wide row.
def private enc_batch_site(enc : MetalComputeEncoder?; t : Model; bt : BatchTier; fmt : KqFmt;
                           woff, rows, n, ys : int64; bx, by, bn, bd, bys : MetalBuffer?;
                           ycol : int64; mv_ok : bool; sk : int64 = 0l; gemm64 : bool = false; bxh : MetalBuffer? = null) {
    if (fmt != KqFmt.q8) {
        enc_kq_site_b(enc, t, fmt, woff, rows, n, bx, n, by, ys, ycol, bn, bd, bys, bt.nrows, bt.mp, bxh)
        return
    }
    let bw = blob_of(g_dev, t, woff)
    if (bt.use_mm) {
        enc_gemm_mm_b(enc, bw.buf, bw.boff, bx, by, uint64(ycol * 4l), bn, bys, bt.mp, rows)
    } elif (mv_ok) {
        enc_mv_site(enc, bw.buf, bw.boff, bx, by, n, n / 4l, rows, bt.nrows, ys, uint64(ycol * 4l))
    } elif (sk > 0l) {
        enc_gemm_sk_b(enc, bw.buf, bw.boff, bx, bt.bpsk, gemm_sk_ka(n, rows, sk), rows, sk)
        enc_sk_reduce_b(enc, bt.bpsk, by, 32l * rows, sk)
    } elif (gemm64) {
        enc_gemm64_b(enc, bw.buf, bw.boff, bx, by, gemm_ka(n, rows, 0l), bt.mp, rows)
    } elif (bt.use_gemm) {
        enc_gemm_b(enc, bw.buf, bw.boff, bx, by, gemm_ka(n, rows, ys), bt.mp, rows, uint64(ycol * 4l))
    } else {
        enc_gemv_rows(enc, bt.nrows, bw.buf, bw.boff, bx, by, bn, bd, bys, n, rows, ys, uint64(ycol * 4l))
    }
}


//! the batch rows' site: the step's tier, the site kind picking its mv form and split
def private site_batch(e : BatchLayerEnc; t : Model; enc : MetalComputeEncoder?; kind : SiteKind; fmt : KqFmt;
                       woff, rows, n, ys : int64; bx, by, bn, bd, bys : MetalBuffer?; ycol : int64) {
    let r & = unsafe(*e.r)
    let mv_ok = kind == SiteKind.wo ? r.mv_wo : (kind == SiteKind.w2 ? r.mv_w2 : (kind == SiteKind.other ? false : r.mv_kdim))
    let sk = kind == SiteKind.wo ? r.wo_sk : (kind == SiteKind.w2 ? r.w2_sk : 0l)
    site_x_twin(e, bx) $(bxh) {
        enc_batch_site(enc, t, r.bt, fmt, woff, rows, n, ys, bx, by, bn, bd, bys, ycol, mv_ok, sk, kind == SiteKind.cls && r.cls64, bxh)
    }
}

//! the batch rows' panels: gate and up in two panels; gemma4's experts take their own pair so the
//! dense branch's tiers run beside the expert chain, a plain MoE's ride the row panels
[unused_argument(hid_l, knfe)]
def private panels_batch(var e : BatchLayerEnc; moe_with_dense : bool; hid_l, knfe : int64) : FfnPanels {
    var r & = unsafe(*e.r)
    return FfnPanels(moe_in = r.bmg4x, mg = moe_with_dense ? r.bmgh : r.bh12, mu = moe_with_dense ? r.bmghb : r.bh12b, mu_col = 0l,
        fg = r.bh12, fu = r.bh12b, fu_col = 0l)
}


//! the batch rows' fused gate+up dot + its act (swiglu or geglu) straight into the gate panel (w2's input), where
//! the step's tier chose it (fuse13: the row GEMV tier, no knockout)
def private w13_fused_batch(var e : BatchLayerEnc; t : Model; enc : MetalComputeEncoder?; l : int64; hid_l : int64; u_hid_l : MetalBuffer?; geglu : bool; fg : MetalBuffer?) : bool {
    let r & = unsafe(*e.r)
    if (!r.fuse13) {
        return false
    }
    let bw1 = blob_of(g_dev, t, t.w1_offs[l])
    let bw3 = blob_of(g_dev, t, t.w3_offs[l])
    enc_gemv_w13sw_b(enc, r.nrows > 2l, bw1.buf, bw1.boff, bw3.boff, r.bxb, fg, r.u_dim, u_hid_l, geglu ? r.u_one32 : r.u_zero32, hid_l)
    return true
}

//! the batch rows' rope + KV store: the codec's batch form over the route table, `lay` the
//! layer's KV-row prefix (elements; row bytes for a block codec)
[unused_argument(kvsh, lkoff, lvoff, rowb), arch(at="../ARCHITECTURE_GPU_MTP_DECODE.md#metal-rope-store-rot")]
def private rope_store_batch(var e : BatchLayerEnc; t : Model; enc : MetalComputeEncoder?; l : int64; het, kvsh : bool;
                             bbias, bcos_l, bsin_l : MetalBuffer?; lkoff, lvoff : uint64; rowb, qd, kv_dim, head_size, n_heads : int64) {
    var r & = unsafe(*e.r)
    let blockc = r.kdt == KVDtype.q8_0 || r.kdt == KVDtype.tq4
    let lays = mirror_route_lay(t, r.kdt, l)
    let lay_l = lays.x
    let ring_l = uint(mirror_ring(t, l))
    let qkvd = uint(qd + 2l * kv_dim)
    let kvm_l = het ? r.k_kvm_sw : r.k_kvm
    // 0 = full rope; a partial carrier (qwen35: rope 64 of head 256) rotates rot/2 pairs, the rest identity
    let rot_l = r.k_rot != 0u ? r.k_rot : uint(head_size)
    // a shared-KV layer (E-series) dispatches its Q rows only: no K rope, no row store into the source slab it reads
    let gd = qg_direct(t.config, r.kdt)
    let bq = gd ? r.bdn_qg : null
    let qhs = gd ? uint(2l * head_size) : 0u
    let qrow = gd ? uint(2l * qd) : 0u
    if (r.kdt == KVDtype.tq4) {
        var tka = RopeStoreBTq4Args(lay = lay_l, qd = uint(qd), head_size = uint(head_size),
            nheads = uint(n_heads), kv_mul = kvm_l, qstride = qkvd,
            neox = r.k_neox, has_bias = r.k_hasb, rot = rot_l, qhs = qhs, qrow = qrow, layr = lays.y, ring = ring_l)
        enc_rope_store_b_tq4(enc, r.bqkv, bcos_l, bsin_l, r.brt, bbias, tka,
            kvsh ? n_heads : n_heads + 2l * (kv_dim / head_size), r.nrows, head_size, 0ul, bq)
    } elif (blockc) {
        var qka = RopeStoreBQ8Args(lay = lay_l, nblk = uint(kv_dim / 32l), qd = uint(qd),
            head_size = uint(head_size), npairs_q = uint(qd / 2l), qstride = qkvd,
            neox = r.k_neox, has_bias = r.k_hasb, rot = rot_l, qhs = qhs, qrow = qrow, layr = lays.y, ring = ring_l)
        enc_rope_store_b_q8(enc, r.bqkv, bcos_l, bsin_l, r.brt, bbias, qka,
            kvsh ? qd / 2l : q8_store_threads(qd, kv_dim), r.nrows, 0ul, bq)
    } elif (qkn_fused(t.config, r.kdt)) {
        var nka = qkn_args(t, l, qd, kv_dim, head_size, int64(rot_l), r.k_neox != 0u, r.k_hasb != 0u, r.k_eps, int64(qkvd), int64(rot_l) / 2l, int64(lay_l))
        nka.layr = lays.y
        nka.ring = ring_l
        if (gd) {
            nka.qstride = qhs
            nka.qrow = qrow
        }
        enc_qkn_rope_b(enc, r.kdt == KVDtype.f16, r.bqkv, bcos_l, bsin_l, r.brt, qkn_weights(t, l, head_size), bbias, nka,
            kvsh ? n_heads : n_heads + 2l * (kv_dim / head_size), r.nrows, 0ul, bq)
    } else {
        let npairs = kvsh ? qd / 2l : (qd + kv_dim) / 2l
        var rka = gk_rope_kv_args(qd, kv_dim, head_size, qd / 2l, npairs, r.k_neox != 0u, r.k_hasb != 0u, int64(rot_l),
            0l, 0l, r.nrows, int64(qkvd), int64(qkvd), int64(rot_l) / 2l, int64(lay_l), int64(ring_l))
        rka.layr = lays.y
        enc_rope_store_b(enc, r.kdt == KVDtype.f16, r.bqkv, bcos_l, bsin_l, r.brt, bbias, rka,
            npairs, r.nrows)
    }
}

//! the batch rows' attention: the decode form's per-row partials where the step armed it, the
//! chunked pair over the deepest row's chunks (sliding layers carry the span), else one dispatch
[unused_argument(lkoff, lvoff, nchunks)]
def private attention_batch(var e : BatchLayerEnc; t : Model; enc : MetalComputeEncoder?; l : int64; lsl, het : bool;
                            bsnk : MetalBuffer?; lkoff, lvoff : uint64; nchunks, head_size, n_heads : int64) {
    let c = t.config
    var r & = unsafe(*e.r)
    let blockc = r.kdt == KVDtype.q8_0 || r.kdt == KVDtype.tq4
    let kvd = uint(layer_kv_dim(c, l))
    let qkvd = uint(n_heads * head_size) + 2u * kvd
    let lays = mirror_route_lay(t, r.kdt, l)
    let lay_l = lays.x
    let kstride_l = blockc ? kvd / 32u : kvd
    let kvm_l = het ? r.k_kvm_sw : r.k_kvm
    let scale_l = het ? r.k_scale_sw : r.k_scale
    let window_l = lsl && c.sliding_window > 0l ? uint(c.sliding_window) : 0u
    let hass_u = c.attn_sinks ? 1u : 0u
    let gd = qg_direct(c, r.kdt)
    let bq = gd ? r.bdn_qg : r.bqkv
    let qstride = gd ? uint(2l * n_heads * head_size) : qkvd
    let qhs = gd ? uint(2l * head_size) : 0u
    let gated = gd && r.kdt != KVDtype.tq4 ? 1u : 0u
    if (r.attn_d) {
        encode_attn_d(t, r, enc, l, head_size, n_heads, window_l, bsnk)
    } elif (r.chunked) {
        // each row derives wstart from its own cnt; the comb skips that row's below-window chunks
        var pka = SqAttnPartBArgs(lay = lay_l, kstride = kstride_l, nheads = uint(n_heads),
            kv_mul = kvm_l, hs = uint(head_size), scale = scale_l, qstride = qstride,
            nchmax = uint(r.nchmax), softcap = c.attn_logit_softcap, window = window_l, qhs = qhs, gated = gated, layr = lays.y,
            ring = uint(mirror_ring(t, l)))
        enc_attn_part_b(enc, r.kdt, bq, r.bpart, r.brt, pka, r.nrows, r.nchmax, n_heads, head_size)
        var cka = SqAttnCombArgs(nch = uint(r.nchmax), nheads = uint(n_heads),
            hs = uint(head_size), window = window_l, hass = hass_u)
        enc_attn_comb_b(enc, r.bpart, r.bxb, cka, r.brt, 0ul, bsnk, n_heads * r.nrows, head_size)
    } else {
        var bka = SqAttnBArgs(lay = lay_l, kstride = kstride_l, nheads = uint(n_heads),
            kv_mul = kvm_l, hs = uint(head_size), scale = scale_l, qstride = qstride,
            softcap = c.attn_logit_softcap, hass = hass_u, qhs = qhs, gated = gated, layr = lays.y)
        enc_attn_b(enc, r.kdt, bq, r.bxb, r.brt, bka, r.nrows, n_heads, head_size, bsnk)
    }
}

//! the batch rows' recurrent layer: the projections as rows GEMVs (one weight pass), then each stage
//! once over every group - a row a session against that session's own state, or the same-slab
//! rows as one group walking its session's state row after row
def private recurrent_batch(var e : BatchLayerEnc; t : Model; enc : MetalComputeEncoder?; l : int64; var dnli : int64&) {
    var r & = unsafe(*e.r)
    recurrent_layer(r, t, enc, l, dnli, false, false)
}

//! the batch step's layer encoder: B rows through the route table on the batch kernel family
struct private BatchLayerEnc : LayerEncT {
    override site = @@site_batch
    override total = @@total_rows
    override residual = @@residual_rows
    override rms = @@rms_rows
    override add = @@add_rows
    override moe_args = @@moe_args_rows
    override rope_store = @@rope_store_batch
    override attention = @@attention_batch
    override recurrent = @@recurrent_batch
    override w13_fused = @@w13_fused_batch
    override panels = @@panels_batch
    override ple = @@ple_rows
    override moe_g4_step = @@moe_g4_step_rows
}

//! The BatchDecodeOverrideFn (P4): one eval_batch step's whole layer stack GPU-resident — B rows
//! through ONE weight pass. STACK-ONLY: on true every row's KV is stored (mirror + CPU cache) and
//! ws.scr.x_b holds the final residuals; false = declined, CPU loop recomputes from clean state.
[arch(at="../ARCHITECTURE_GPU_MTP.md#batch-same-slab"), arch(at="../ARCHITECTURE_GPU_MTP_DECODE.md#metal-dispatch-requires")]
def metal_batch_decode_forward(t : Model; var ws : BatchWorkspace; var sessions : array<Session?>; nrows : int64) : bool {
    var why = batch_decode_decline(t, ws, sessions, nrows)
    if (why == MetalDecodeDecline.none && !metal_decode_init()) {
        why = MetalDecodeDecline.device
    }
    if (why != MetalDecodeDecline.none) {
        finish_pending_step()   // the CPU fallback needs the in-flight step's KV/logits landed first
        g_land_err = false
        discard_batch_pre()   // the CPU step advances the rows past the pre-encoded positions
        if (why == MetalDecodeDecline.feature) {
            batch_decline(why, metal_missing_needs(t, BATCH_NEEDS_OK))
        } else {
            batch_decline(why, metal_fmt_decline_say("batch decode", why, t))
        }
        return false
    }
    residency_flush()   // pin/commit before the step's submission (the post-CPU-window slack)
    // a parked single-stream pre-step may reference a batch row's mirror slice — retire it first
    discard_pre()
    if (batch_pre_matches(t, ws, sessions, nrows)) {
        return batch_take_pre(t, ws, sessions, nrows)
    }
    discard_batch_pre()
    // encode-ahead readiness: with a step in flight, mirror_prepare must stay on its fast path - any
    // miss completes the pending step first and takes this one synchronously
    if (pending_active() && !rows_mirror_ready(ws, sessions, nrows, 0l)) {
        finish_pending_step()
    }
    // the recurrent rows' state: each session's DnMirror at its own position - a cold mirror syncs
    // from the CPU prefix under the previous step's completion, a session with neither declines the step;
    // the same-slab rows are one session's consecutive positions, one group over its one mirror
    if (t.config.recr_mask != 0ul) {
        finish_pending_step()
        if (!batch_dn_rows(ws, sessions, nrows, 0l)) {
            g_land_err = false
            batch_decline(MetalDecodeDecline.dn_state)
            return false
        }
    }
    let ts_all = ref_time_ticks()
    let rp = batch_rows_prepare(t, ws, sessions, nrows, 0l)
    if (!rp.ok) {
        finish_pending_step()   // fall back synchronously — the CPU loop needs completed KV state
        g_land_err = false
        batch_decline(MetalDecodeDecline.mirror)
        return false
    }
    var L & = g_land[1 - g_lp_i]
    batch_step_build(t, ws, sessions, nrows, rp.maxcnt, 0l, false, ts_all, L)
    if (!L.gpu_logits) {
        L.xb = L.bx   // the CPU-classifier tail reads x_b — sync-only writeback
        L.xb_dst = unsafe(addr < void? >(ws.scr.x_b[0]))
        L.xb_bytes = nrows * L.dim * 4l
    }
    return batch_step_land(t, ws, sessions, nrows, L)
}

//! The recurrent rows' group table words at `positions + poff`: a row a session against its own mirror, or the same-slab
//! rows as one group over their one session's (`ARCHITECTURE_GPU_MTP.md#dn-group-stages`); false = a row with no state
def private batch_dn_rows(ws : BatchWorkspace; var sessions : array<Session?>; nrows, poff : int64) : bool {
    let ng = batch_row_groups(nrows)
    dn_grp_clear()
    g_b_ple_bases |> clear()
    g_b_dn_soffs |> clear()
    dn_mirror_room(ng)
    g_b_ple_bases |> reserve(ng)
    g_b_dn_soffs |> reserve(ng)
    for (i in range64(nrows)) {
        continue if (g_b_grp_nr[i] == 0l)
        var sp = sessions[i]
        var dm = dn_mirror_prepare(*sp, ws.positions[i] + poff)
        return false if (!dm.ok)
        dn_grp_push(i, g_b_grp_nr[i], dm.state_off, dm.state_off, dm.conv_off, dm.conv_off)
        g_b_ple_bases |> push(dm.ple_off)
        g_b_dn_soffs |> push(dm.state_off)
    }
    g_b_dn_ng = ng
    return true
}

var private @scratch g_b_grp_nr : array<int64>   //! a row's group size where the row opens its group, 0 past the group's first row

//! The step's row groups: under the same-slab step one session's consecutive rows a group (`batch_same_group_rows`), else a row a
//! group; fills `g_b_grp_nr` and returns the group count
[arch(at="../ARCHITECTURE_GPU_MTP.md#batch-same-slab")]
def private batch_row_groups(nrows : int64) : int64 {
    g_b_grp_nr |> resize(nrows)
    let same = batch_same_slab()
    var row = 0l
    var g = 0l
    while (row < nrows) {
        let n = same ? (g < batch_same_group_count() ? clamp(batch_same_group_rows(g), 1l, nrows - row) : nrows - row) : 1l
        for (i in range64(n)) {
            g_b_grp_nr[row + i] = i == 0l ? n : 0l
        }
        row += n
        g++
    }
    return g
}

//! the deepest group's rows: the recurrent stages' longest walk
def private batch_group_rows_max(nrows : int64) : int64 {
    var m = 0l
    for (i in range64(nrows)) {
        m = max(m, g_b_grp_nr[i])
    }
    return m
}

def private batch_step_land(t : Model; ws : BatchWorkspace; var sessions : array<Session?>; nrows : int64; var L : BatchLanding&) : bool {
    // the overlap: the PREVIOUS step completes only now, with this one already committed behind it
    finish_pending_step()
    g_land_err = false   // a lost pipelined step was logged + counted; this step retries fresh
    g_lp_i = 1 - g_lp_i
    L.active = true
    let gpu_logits = L.gpu_logits
    batch_pre_encode_next(t, ws, sessions, nrows, L)
    if (!(g_b_pipe && gpu_logits && empty(g_skip))) {
        finish_pending_step()   // synchronous path: complete + read back immediately
        if (g_land_err) {
            g_land_err = false
            discard_batch_pre()   // the CPU rerun advances the rows past the step just pre-encoded
            return false   // the dispatch failed — the caller reruns this step on the CPU loop
        }
    }
    if (gpu_logits) {
        batch_decode_override_logits_done()
    }
    return true
}

//! `maxcnt` is the deepest row's count; `ok = false` is a failed reserve or prepare - the caller chooses between declining and standing down.
[arch(at="../ARCHITECTURE_GPU_MTP.md#mtp-round-one-join")]
def batch_rows_prepare(t : Model; ws : BatchWorkspace; var sessions : array<Session?>; nrows, poff : int64) : tuple<ok : bool; maxcnt : int64> {
    let kdt = ws.scr.kv_dtype_k
    let blockc = kdt == KVDtype.q8_0 || kdt == KVDtype.tq4   // block codec: the row table carries byte offsets
    let esz = kdt == KVDtype.f16 ? 2l : 4l
    // reserve the whole step's arena demand BEFORE pinning — growth is a release-all rebuild
    // and cannot run once the batch's rows are pinned (the readiness check above already
    // finished any in-flight step when a session needs a new slice)
    batch_row_groups(nrows)
    var demand = 0ul
    var largest = 0ul
    for (i in range64(nrows)) {
        continue if (g_b_grp_nr[i] == 0l)
        let d = mirror_arena_demand(t, *sessions[i], ws.positions[i + g_b_grp_nr[i] - 1l] + poff)
        demand += d
        largest = max(largest, d)
    }
    if (!mirror_reserve(demand, largest)) {
        return (ok = false, maxcnt = 0l)
    }
    // pin the batch's uids: a later row's mirror alloc must never evict an earlier row's slice
    for (i in range64(nrows)) {
        g_pin |> insert(sessions[i].uid)
    }
    g_b_rte |> resize(nrows * 4l)
    g_b_koffs |> resize(nrows)
    g_b_voffs |> resize(nrows)
    g_b_caps |> resize(nrows)
    var maxcnt = 0l
    var rows_ok = false
    var mir_same = (ok = false, koff = 0ul, voff = 0ul, cap = 0l)
    for (attempt in range(2)) {
        rows_ok = true
        maxcnt = 0l
        for (i in range64(nrows)) {
            var sp = sessions[i]
            if (g_b_grp_nr[i] > 0l) {
                mir_same = mirror_prepare(t, *sp, ws.positions[i] + poff)
            }
            let mir = mir_same
            if (!mir.ok || !mirror_slice_in_reach(sp.uid)) {   //! a slice past the row table's reach takes the empty-arena retry too
                rows_ok = false
                break
            }
            g_b_koffs[i] = mir.koff
            g_b_voffs[i] = mir.voff
            g_b_caps[i] = mir.cap
            let cnt = ws.positions[i] + poff + 1l
            maxcnt = max(maxcnt, cnt)
            //! block codecs address in bytes, scalar mirrors in elements; every row's slice lies in the uint32 reach (checked above)
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
        if (attempt != 0 || !mirror_defrag(demand, largest)) {
            break
        }
        for (i in range64(nrows)) {
            g_pin |> insert(sessions[i].uid)
        }
    }
    g_pin |> clear()
    return (ok = rows_ok, maxcnt = maxcnt)
}

def private batch_step_poke(L : BatchLanding; ws : BatchWorkspace) {
    unsafe {
        if (L.poke_x) {
            memcpy(metal_buffer_contents(L.bx), addr < void? >(ws.scr.x_b[0]), L.nrows * L.dim * 4l)
        }
        memcpy(metal_buffer_contents(L.bcos), addr < void? >(ws.scr.rope_cos[0]), L.bytes_tab)
        memcpy(metal_buffer_contents(L.bsin), addr < void? >(ws.scr.rope_sin[0]), L.bytes_tab)
        if (L.bcos_swa != null) {
            memcpy(metal_buffer_contents(L.bcos_swa), addr < void? >(ws.scr.rope_cos_swa[0]), L.bytes_tab_sw)
            memcpy(metal_buffer_contents(L.bsin_swa), addr < void? >(ws.scr.rope_sin_swa[0]), L.bytes_tab_sw)
        }
        if (L.bple != null) {   // the CPU pre-step's rows, position-major as the step reads them
            memcpy(metal_buffer_contents(L.bple), addr < void? >(ws.scr.ple_inp[0]), L.nrows * L.layers * L.ple_n * 4l)
        }
        if (L.row_tok != null) {   // the rows' token ids the device gathers their rows and side rows by
            var tp = reinterpret<uint?>(metal_buffer_contents(L.row_tok))
            for (i in range64(L.nrows)) {
                tp[i] = uint(ws.row_tokens[i])
            }
        }
    }
}

//! Every row's mirror slice sits on `mirror_prepare`'s fast path at `positions + poff`: the slice
//! holds the position, the codec is the session's, and the watermark sits EXACTLY there - below it
//! a prepare uploads, above it a rewind re-uploads from row 0, both under an in-flight step's writes.
def private rows_mirror_ready(ws : BatchWorkspace; sessions : array<Session?>; nrows, poff : int64) : bool {
    for (i in range64(nrows)) {
        let sp = sessions[i]
        let pos = ws.positions[i] + poff
        return false if (!key_exists(g_mirrors, sp.uid))
        let mr = g_mirrors[sp.uid]   // cheap copy — fields tested off it
        return false if (mr.cap_rows < pos + 1l || mr.kdt != sp.kv_dtype_k || mr.vdt != sp.kv_dtype_v || mr.watermark != pos)
    }
    return true
}

var private g_b_pre = false   //!< DASLLAMA_METAL_BATCH_PRE: the next batched step pre-encodes under this one's GPU run

//! Test and A/B lever over `DASLLAMA_METAL_BATCH_PRE`.
def public set_metal_batch_pre(on : bool) {
    if (g_bgemm_min < 0l) {
        load_batch_knobs()   // the lazy env load would overwrite the lever on the first step
    }
    g_b_pre = on
    if (!on) {
        discard_batch_pre()
    }
}

def public metal_batch_pre_enabled() : bool {
    if (g_bgemm_min < 0l) {
        load_batch_knobs()   // the shipped default is the env's, not the initializer's
    }
    return g_b_pre
}

[arch(at="../ARCHITECTURE_GPU_MTP_DECODE.md#batch-pre-encode")]
def private batch_pre_encode_next(t : Model; ws : BatchWorkspace; var sessions : array<Session?>; nrows : int64; L : BatchLanding) {
    return if (!g_b_pre || !L.gpu_logits || L.sameslab || t.config.hyper_conn || !empty(g_skip))
    for (i in range64(nrows)) {
        return if (ws.positions[i] + 2l > t.config.seq_len)
    }
    return if (!rows_mirror_ready(ws, sessions, nrows, 1l))
    //! a recurrent row's mirror stands at its next position from the commit-time mark: the prepare touches no state
    for (i in range64(nrows)) {
        return if (t.config.recr_mask != 0ul && dn_mirror_live_off(sessions[i].uid, ws.positions[i] + 1l) == ~0ul)
    }
    return if (t.config.recr_mask != 0ul && !batch_dn_rows(ws, sessions, nrows, 1l))
    let ts_all = ref_time_ticks()
    let rp = batch_rows_prepare(t, ws, sessions, nrows, 1l)
    return if (!rp.ok)
    var P & = g_land[1 - g_lp_i]
    batch_step_build(t, ws, sessions, nrows, rp.maxcnt, 1l, true, ts_all, P)
    P.pre_valid = true
}

//! The pre-encoded step is this call's: the same rows (by uid - a session pointer is not identity)
//! at the same positions, the same model shape and KV codec, the arena epoch, every slice where
//! the row table names it, and every row's zero-copy logits wrap where the scatter was encoded.
def private batch_pre_matches(t : Model; ws : BatchWorkspace; var sessions : array<Session?>; nrows : int64) : bool {
    let P & = g_land[1 - g_lp_i]
    let c = t.config
    return false if (!P.pre_valid || batch_same_slab() || P.nrows != nrows || P.kdt != ws.scr.kv_dtype_k || P.epoch != g_arena_epoch || !empty(g_skip) ||
        P.dim != c.dim || P.layers != c.n_layers || P.seq != c.seq_len || P.vocab != c.vocab_size)
    for (i in range64(nrows)) {
        return false if (P.uids[i] != sessions[i].uid || P.positions[i] != ws.positions[i])
    }
    if (c.recr_mask != 0ul) {   //! the pool has not moved and every row's state still sits where the step was encoded
        return false if (P.dn_epoch != g_dn_pool_epoch)
        for (i in range64(nrows)) {
            return false if (dn_mirror_live_off(sessions[i].uid, ws.positions[i]) != P.dn_soffs[i])
        }
    }
    return false if (!rows_mirror_ready(ws, sessions, nrows, 0l))   // a prepare off the fast path would touch the arena under the pending step
    for (i in range64(nrows)) {
        let mir = mirror_prepare(t, *sessions[i], ws.positions[i])
        return false if (!mir.ok || mir.koff != P.koffs[i] || mir.voff != P.voffs[i] || mir.cap != P.caps[i])
    }
    if (P.nc_all) {
        for (i in range64(nrows)) {
            return false if (nc_logits_for(g_dev, *sessions[i]) != P.ncbufs[i])
        }
    }
    return true
}

def private batch_take_pre(t : Model; ws : BatchWorkspace; var sessions : array<Session?>; nrows : int64) : bool {
    let ts_all = ref_time_ticks()
    var P & = g_land[1 - g_lp_i]
    batch_step_poke(P, ws)
    for (c_i in P.cbs) {
        metal_commit(c_i)
    }
    P.pre_valid = false
    mark_rows_committed(P)
    g_batch_pre_taken++
    g_us_bsetup += int64(get_time_usec(ts_all))
    return batch_step_land(t, ws, sessions, nrows, P)
}

//! `pre` leaves every command buffer ended, uncommitted and unpoked; `poff` is the rows' position offset from `ws.positions`.
def batch_step_build(t : Model; ws : BatchWorkspace; var sessions : array<Session?>; nrows, maxcnt, poff : int64; pre : bool; ts_all : int64; var L : BatchLanding&) {   // nolint:STYLE037,STYLE038 — the batch step encoder; acquisition, layer chain, classifier tail and stash share the whole step's state
    let c = t.config
    let dim = c.dim
    let head_size = c.head_size
    let n_heads = c.n_heads
    let qd = n_heads * head_size
    let kv_dim = c.n_kv_heads * head_size
    let hidden = layer_hidden(t, 0l)
    let hid2 = ffn_second_hidden(t)   //! the MatFormer E-series' second FFN width (0 = uniform); the step gate declines a third
    let hid_mx = max(hidden, hid2)
    let qkvd = qd + 2l * kv_dim
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
    let tq4 = kdt == KVDtype.tq4
    let blockc = kdt == KVDtype.q8_0 || tq4  // block codec: byte-offset row table + codec kernel twins
    let dn = c.recr_mask != 0ul
    let same = batch_same_slab()
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
        (dim % 64l) == 0l && (qd % 64l) == 0l && (kv_dim % 64l) == 0l && (hidden % 64l) == 0l && (hid2 % 64l) == 0l &&
        (qd_sw % 64l) == 0l && (kvd_sw % 64l) == 0l)
    let mp = use_gemm ? ((nrows + 31l) / 32l) * 32l : (nrows <= 2l ? 2l : nrows <= 4l ? 4l : 8l)
    // per-site GEMM policy (see g_bsk): each split-K site gates on its OWN K's nkb % S == 0 (the
    // partial planes assume mp == 32); ineligible sites stay on the planar v2. Hetero serves via
    // per-class twins; the wo gate must hold for BOTH classes' qd.
    let sk_ok = use_gemm && !use_mm && g_bsk && mp == 32l
    let qkv_sk = (sk_ok && (dim / 32l) % 2l == 0l) ? 2l : 0l
    let wo_sk = (sk_ok && (qd / 32l) % 4l == 0l && (!hetero || (qd_sw / 32l) % 4l == 0l)) ? 4l : 0l
    let w2_sk = (sk_ok && (hidden / 32l) % 8l == 0l && (hid2 / 32l) % 8l == 0l) ? 8l : 0l
    let cls64 = use_gemm && !use_mm && g_bsk && (c.vocab_size % 64l) == 0l
    // the mul_mv rail owns the whole fixed-B path (w13 = two mv dispatches + the ew2 swiglu, so
    // the x-staged w13sw fusion stands down with it). Hetero rides it too: the QKV arm carries
    // the no-wv cat3 stream, and the wo site's x stride (the one per-class xs4) picks its twin
    let mv = g_bmv && !use_gemm && !use_mm
    let mv_kdim = mv && (dim % MV_K_ALIGN) == 0l
    let mv_wo = mv && (qd % MV_K_ALIGN) == 0l && (!hetero || (qd_sw % MV_K_ALIGN) == 0l)
    let mv_w2 = mv && (hidden % MV_K_ALIGN) == 0l && (hid2 % MV_K_ALIGN) == 0l
    // fold w1+w3+the act into one fused GEMV on the fixed-B path; a knockout run (g_skip) keeps the
    // stages separable so its per-stage attribution stays valid
    let fuse13 = g_bfuse && !mv && !use_gemm && !use_mm && empty(g_skip) && !t.kquant_native
    let chunked = maxcnt > g_attn_single_max
    let nchmax = (maxcnt + ATTN_CHUNK_ROWS - 1l) / ATTN_CHUNK_ROWS
    let use_attn_d = attn_d_serves(head_size, hs_sw)
    let d_nsgs = attn_d_groups(maxcnt)
    // logits-on-GPU (default ON): the batched final rms + classifier ride the same cb and the
    // CPU tail's classifier (one full weight-plane stream per step — ~6ms of the small-B wall)
    // is skipped via batch_decode_override_logits_done. Gates mirror the single-stream driver's.
    let cls_fmt = cls_fmt_of(t)
    let tied_f32 = cls_tied_f32(t)
    let gpu_logits = ((t.metal_blob || (!g_logits_cpu && g_env_logits && empty(g_skip))) &&
        !tied_f32 &&
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
    let mr = ((nrows + 3l) / 4l) * 4l
    let bytes_x = uint64(mp * dim * 4l)
    let bytes_xb = uint64(mp * max(dim, qd_mx) * 4l)   // dim-wide norm outs AND the qd-wide attention out
    let bytes_qkv = uint64(mp * qkvd_mx * 4l)
    let nsh = moe ? c.n_ff_shexp : 0l
    let bytes_h = uint64(max(max(mp * hid_mx, mr * knfe), max(mr, mp) * 2l * nsh) * 4l)   //! the wider FFN width, the PLE gate rows (ple <= hidden) and the shared expert's gate|up pair all fit
    let bytes_tab = uint64(nrows * ((c.rope_dim > 0l ? c.rope_dim : head_size) / 2l) * 4l)   //! the rows are built at the ROTATED width (a partial carrier: rope_dim/2) - the copy below reads exactly them
    let bytes_tab_sw = uint64(nrows * half_sw * 4l)   // the swa pair's rows are ITS class's hs/2
    let bytes_rt = uint64(nrows * 16l)
    let bytes_part = uint64(use_attn_d ? attn_d_part_floats(nrows, n_heads, hs_mx, d_nsgs) * 4l :
        nrows * n_heads * nchmax * (hs_mx + 2l) * 4l)
    var bx = pool_acquire(g_pool, g_dev, bytes_x)
    var bxb = pool_acquire(g_pool, g_dev, bytes_xb)
    let twins = kq_half_rows(t, nrows)
    let bytes_xh = twins ? uint64(max(mp, KQ_HALF_TWIN_ROWS) * max(dim, qd_mx) * 2l) : 0ul
    let bytes_fgh = twins ? uint64(max(mp, KQ_HALF_TWIN_ROWS) * hid_mx * 2l) : 0ul
    var bxh = twins ? pool_acquire(g_pool, g_dev, bytes_xh) : null
    var bfgh = twins ? pool_acquire(g_pool, g_dev, bytes_fgh) : null
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
    var bt = BatchTier(use_mm = use_mm, use_gemm = use_gemm, mp = mp, nrows = nrows, bpsk = bpsk)
    let bytes_mlg = uint64(mr * (c.n_expert + 1l) * 4l)
    let bytes_msel = uint64(mr * 2l * c.n_expert_used * 4l)
    let bytes_mdn = uint64(mr * c.n_expert_used * dim * 4l)
    var bmlg = moe ? pool_acquire(g_pool, g_dev, bytes_mlg) : null
    var bmsel = moe ? pool_acquire(g_pool, g_dev, bytes_msel) : null
    var bmdn = moe ? pool_acquire(g_pool, g_dev, bytes_mdn) : null
    let bytes_mg4x = uint64(mr * dim * 4l)
    let bytes_mgh = uint64(mr * knfe * 4l)
    var bmg4x = g4moe ? pool_acquire(g_pool, g_dev, bytes_mg4x) : null
    var bmg4rt = g4moe ? pool_acquire(g_pool, g_dev, bytes_mg4x) : null
    var bmgh = g4moe ? pool_acquire(g_pool, g_dev, bytes_mgh) : null
    var bmgh2 = g4moe ? pool_acquire(g_pool, g_dev, bytes_mgh) : null
    // the recurrent rows' planes (nrows deep) and the gated-attention pair; the mirrors are the sessions'
    let cd = dn ? dn_conv_dim(c) : 0l
    let di = dn ? c.ssm_d_inner : 0l
    let nvh = dn ? c.ssm_dt_rank : 0l
    let mdn = max(mr, mp)   //! the recurrent projections are weight sites: the GEMM tier writes its whole M pad into them
    let bytes_dn_cd = uint64(mdn * cd * 4l)
    let bytes_dn_di = uint64(mdn * di * 4l)
    let bytes_dn_h = uint64(mdn * nvh * 4l)
    var bdn_qkv = dn ? pool_acquire(g_pool, g_dev, bytes_dn_cd) : null
    var bdn_cv = dn ? pool_acquire(g_pool, g_dev, bytes_dn_cd) : null
    var bdn_z = dn ? pool_acquire(g_pool, g_dev, bytes_dn_di) : null
    var bdn_o = dn ? pool_acquire(g_pool, g_dev, bytes_dn_di) : null
    var bdn_b = dn ? pool_acquire(g_pool, g_dev, 2ul * bytes_dn_h) : null
    var bdn_g = dn ? pool_acquire(g_pool, g_dev, bytes_dn_h) : null
    var u_cd = dn ? uniform_u32(uint(cd)) : null
    var u_nvh = dn ? uniform_u32(uint(nvh)) : null
    var u_nvh2 = dn ? uniform_u32(uint(2l * nvh)) : null
    var u_di = dn ? uniform_u32(uint(di)) : null
    var dn_tab : tuple<buf : MetalBuffer?; bytes : uint64>
    if (dn) {
        dn_tab = dn_grp_upload()
    }
    let bytes_qg = uint64(mr * qd * 4l)
    var bdn_qg = c.q_gated ? pool_acquire(g_pool, g_dev, 2ul * bytes_qg) : null
    var bdn_gate = c.q_gated ? pool_acquire(g_pool, g_dev, bytes_qg) : null
    var u_dn_qd2 = c.q_gated ? uniform_u32(uint(2l * qd)) : null
    var u_total_qd = c.q_gated ? uniform_u32(uint(nrows * qd)) : null
    // the hyper-connection rows (qwen4exp): the wide residual and the mixer scratch at mp rows, the n-gram side input's rows
    let hc = c.hyper_conn
    let png = has_ple_ngram(c)
    let hcd = hc ? hc_dim(c) : 0l
    let bytes_hcd = uint64(hcd * 4l)
    let bytes_lr = uint64(c.hc_low_rank * 4l)
    let bytes_hc = uint64(c.hc_count * 4l)
    let rows_hc = uint64(mp)
    var bhc = hc ? pool_acquire(g_pool, g_dev, rows_hc * bytes_hcd) : null
    var bhc_xn = hc ? pool_acquire(g_pool, g_dev, rows_hc * bytes_hcd) : null
    var bhc_gate = hc ? pool_acquire(g_pool, g_dev, rows_hc * bytes_hcd) : null
    var bhc_lo = hc ? pool_acquire(g_pool, g_dev, rows_hc * bytes_lr) : null
    var bhc_inj_a = hc ? pool_acquire(g_pool, g_dev, rows_hc * bytes_hc) : null
    var bhc_inj_f = hc ? pool_acquire(g_pool, g_dev, rows_hc * bytes_hc) : null
    var u_hcd = hc ? uniform_u32(uint(hcd)) : null
    var u_hc = hc ? uniform_u32(uint(c.hc_count)) : null
    var u_lr = hc ? uniform_u32(uint(c.hc_low_rank)) : null
    var u_lr_tot = hc ? uniform_u32(uint(nrows * c.hc_low_rank)) : null
    var u_hcd_tot = hc ? uniform_u32(uint(nrows * hcd)) : null
    let ind = png ? ple_in_dim(c) : 0l
    let bytes_ple_in = uint64(ind * 4l)
    var bple_emb = png ? pool_acquire_untracked(g_upool, g_dev, uint64(nrows) * bytes_ple_in) : null
    var bple_key = png ? pool_acquire(g_pool, g_dev, rows_hc * bytes_hcd) : null
    var bple_gated = png ? pool_acquire(g_pool, g_dev, rows_hc * bytes_hcd) : null
    var bple_val = png ? pool_acquire(g_pool, g_dev, bytes_x) : null
    var u_ple_in = png ? uniform_u32(uint(ind)) : null
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
    let k_dim = uint(dim)
    var u_qd = uniform_u32(uint(qd))
    var u_kvd = uniform_u32(uint(kv_dim))
    var u_qkvd = uniform_u32(uint(qkvd))
    var u_hidden = uniform_u32(uint(hidden))
    var u_hidden2 = hid2 > 0l ? uniform_u32(uint(hid2)) : null
    var u_hs = uniform_u32(uint(head_size))
    var u_eps = uniform_f32(c.norm_eps)
    var u_softcap = uniform_f32(c.attn_logit_softcap)
    var u_zero32 = uniform_u32(0u)
    var u_one32 = uniform_u32(1u)   //! the shared expert's gate dot is a 1-row router GEMV
    // the SLIDING-class uniform twins (hetero models — uniform models leave these null and
    // every layer binds the base set)
    var u_qd_sw = hetero ? uniform_u32(uint(qd_sw)) : null
    var u_kvd_sw = hetero ? uniform_u32(uint(kvd_sw)) : null
    var u_qkvd_sw = hetero ? uniform_u32(uint(qkvd_sw)) : null
    var u_hs_sw = hetero ? uniform_u32(uint(hs_sw)) : null
    // the V-from-K / v_norm sites: v segment element offset within a fused-QKV row, per class
    // per-row depths differ, so the batch kernels derive wstart from `window` + their row's cnt
    var u_winsl = c.sliding_window > 0l ? uniform_u32(uint(c.sliding_window)) : null
    // suppressed token ids pin to -1e30 after the cap (gemma4), the CPU order
    let nsupp = long_length(t.suppress)
    var bsupp : MetalBuffer?
    if (gpu_logits && nsupp > 0l) {
        bsupp = pool_acquire_untracked(g_upool, g_dev, uint64(nsupp * 4l))
        unsafe {
            var sp = reinterpret<uint?>(metal_buffer_contents(bsupp))
            for (i in range64(nsupp)) {
                sp[i] = uint(t.suppress[i])
            }
        }
    }
    var u_s16 = uniform_u32(s16 ? 1u : 0u)
    var u_total_dim = uniform_u32(uint(nrows * dim))
    var u_total_h = uniform_u32(uint(nrows * hidden))
    var u_total_h2 = hid2 > 0l ? uniform_u32(uint(nrows * hid2)) : null
    // the E-series' per-layer side rows, position-major: built on the device off the rows' token ids, or the CPU pre-step's poked
    let plef = has_ple(c)
    let ple_n = plef ? c.n_embd_per_layer : 0l
    let bytes_ple = uint64(c.n_layers * nrows * ple_n * 4l)
    var ple_r = StepRes(bytes_ple = bytes_ple)   //! the side rows' buffers, `ple_step_dev`'s
    if (plef) {
        ple_step_dev(t, ple_r, metal_ple_dev_said("decode", t))
    }
    var u_ple = plef ? uniform_u32(uint(ple_n)) : null
    let same_toks = same && g_b_same_toks != null   //! the same-slab rows' ids ride the device already
    let gpu_toks = (g_b_gather || same_toks) && emb_dev_gather_ok(t)   //! every row's embed off its id: the same-slab rows' ids on the device, else the rows' ids poked up
    var row_tok = ((gpu_toks || ple_r.ple_dev) && !same_toks) ? pool_acquire_untracked(g_upool, g_dev, uint64(nrows * 4l)) : null
    var btoks = row_tok != null ? row_tok : g_b_same_toks
    // MoE per-step uniforms (the expert twins' stride set; see enc_moe_gemv)
    var u_moe_ne = moe ? uniform_u32(uint(c.n_expert)) : null
    var u_moe_nsh = nsh > 0l ? uniform_u32(uint(nsh)) : null
    var u_moe_shtot = nsh > 0l ? uniform_u32(uint(nrows * nsh)) : null
    var u_moe_k = moe ? uniform_u32(uint(c.n_expert_used)) : null
    var u_moe_nfe = moe ? uniform_u32(uint(c.n_ff_exp)) : null
    let k_moe_nfe = uint(c.n_ff_exp)
    var u_moe_eblk = moe ? uniform_u32(uint(dim * c.n_ff_exp / 32l)) : null
    let k_moe_eblk = uint(dim * c.n_ff_exp / 32l)
    var u_moe_esb = moe ? uniform_u32(uint(dim * c.n_ff_exp / 256l)) : null
    let k_moe_esb = uint(dim * c.n_ff_exp / 256l)
    var u_moe_ss = moe ? uniform_u32(uint(2l * c.n_expert_used)) : null
    let k_moe_ss = uint(2l * c.n_expert_used)
    var u_moe_xs_dn = moe ? uniform_u32(uint(c.n_ff_exp)) : null
    let k_moe_xs_dn = uint(c.n_ff_exp)
    var u_moe_knfe = moe ? uniform_u32(uint(knfe)) : null
    let k_moe_knfe = uint(knfe)
    var u_moe_kdim = moe ? uniform_u32(uint(c.n_expert_used * dim)) : null
    let k_moe_kdim = uint(c.n_expert_used * dim)
    var u_moe_tot = moe ? uniform_u32(uint(nrows * knfe)) : null
    var u_moe_invd = g4moe ? uniform_f32(1.0 / sqrt(float(dim))) : null
    var u_moe_gmode = moe ? uniform_u32(c.moe_gate == MoeGate.softmax_weight ? 1u : (c.norm_topk_prob ? 0u : 2u)) : null
    let k_moe_gmode = c.moe_gate == MoeGate.softmax_weight ? 1u : (c.norm_topk_prob ? 0u : 2u)
    var u_hass = c.attn_sinks ? uniform_u32(1u) : null
    var u_nch = (!use_attn_d && chunked) ? uniform_u32(uint(nchmax)) : null
    var u_nsgs = use_attn_d ? uniform_u32(uint(d_nsgs)) : null
    var u_vocab = gpu_logits ? uniform_u32(uint(c.vocab_size)) : null
    L.bx = bx
    L.poke_x = !gpu_toks
    L.bcos = bcos
    L.bsin = bsin
    L.bcos_swa = bcos_swa
    L.bsin_swa = bsin_swa
    L.bytes_tab = bytes_tab
    L.bytes_tab_sw = bytes_tab_sw
    L.bple = ple_r.ple_dev ? default<MetalBuffer?> : ple_r.bple
    L.row_tok = row_tok
    L.ple_n = ple_n
    L.dim = dim
    L.nrows = nrows
    L.layers = c.n_layers
    unsafe {
        memcpy(metal_buffer_contents(brt), addr < void? >(g_b_rte[0]), bytes_rt)
    }
    if (!pre) {
        if (png) {   // each row's n-gram heads: the arch's pre-stack hook gathers them against that session's window and advances it
            for (i in range64(nrows)) {
                var sp = sessions[i]
                invoke(t.blocks.pre_stack_decode, t, *sp, ws.row_tokens[i], ws.positions[i])
                unsafe {
                    memcpy(reinterpret<void?>(reinterpret<uint8?>(metal_buffer_contents(bple_emb)) + uint64(i) * bytes_ple_in), addr < void? >(sp.ple_emb[0]), bytes_ple_in)
                }
            }
        }
        batch_step_poke(L, ws)
    }
    g_us_bsetup += int64(get_time_usec(ts_all))
    let ts_enc = ref_time_ticks()
    let ncb = min(g_bncb, c.n_layers)
    let lay_per_cb = (c.n_layers + ncb - 1l) / ncb
    var cbs : array<MetalCommandBuffer?>   // two steps' cb lists are alive at once (this one + the pending L.cbs), so a reused global cannot serve
    cbs |> reserve(ncb)   // nolint:PERF026 — see the declaration above; bounded by ncb
    // concurrent-encode rail: a concurrent encoder + explicit barriers ONLY at real buffer hazards
    // (the serial encoder implicit-barriers every dispatch). Gated to a full pipeline — a knockout
    // (g_skip) run stays on the serial encoder so a skipped stage can't leave a dependent unbarriered.
    let conc = g_bconcurrent && empty(g_skip)
    var cb = metal_new_command_buffer_unretained(g_queue)
    if (same_toks && g_b_same_chain_on) {   //! the drafts writing the rows' ids run first, on a serial pass of their own; tracked buffers order the passes
        var cenc = metal_new_compute_encoder(cb)
        invoke(g_b_same_chain, t, cenc)
        metal_end_encoding(cenc)
        metal_release(cenc)
        g_chain_rounds++
    }
    hz_arm(conc)   // the range oracle: the *_b encoders self-declare, hz_gate barriers at real hazards
    var enc = conc ? metal_new_compute_encoder_concurrent(cb) : metal_new_compute_encoder(cb)
    if (gpu_toks) {
        var u_esc = c.embed_scale != 1.0 ? uniform_f32(c.embed_scale) : g_one
        enc_embed_dev(enc, t, btoks, 0ul, bx, 0ul, nrows, u_dim, u_esc)
        g_b_emb_gathered += nrows
        if (u_esc != g_one) {
            L.upooled |> push((buf = u_esc, size = 4ul))
        }
    }
    if (ple_r.ple_dev) {   //! the rows' side input off their own token ids, after their embed rows landed in bx
        enc_ple_pre_dev(enc, t, btoks, 0ul, bx, ple_r.bple, ple_r.bple_proj, ple_r.u_ple_all, ple_r.u_ple_sq, nrows)
    }
    // layer-0 attention norm (the per-layer trailing add+rms feeds the NEXT layer's); a hyper-connection model opens the rows' wide residual instead
    var xh0 = false
    if (g_skip != "ew") {
        if (hc) {
            enc_hc_init(enc, bx, bhc, hc_init_args(c, nrows), nrows * hcd)
        } else {
            var bw_att0 = upload_region(unsafe(addr < void? >(t.fblob[t.rms_att_off])), uint64(dim * 4l))
            if (bxh != null) {
                enc_rms_bhx(enc, bx, 0ul, bw_att0, bxb, u_dim, u_eps, bxh, nrows)
                xh0 = true
            } else {
                enc_rms_b(enc, bx, 0ul, bw_att0, bxb, u_dim, u_eps, nrows)
            }
        }
    }
    var r = StepRes(valid = true, pos = maxcnt - 1l, chain_pos = maxcnt - nrows, nrows = nrows, kdt = kdt, hetero = hetero, moe = moe,
        chunked = chunked, attn_d = use_attn_d, d_nsgs = d_nsgs, nchmax = nchmax, hid0 = hidden,
        u_hidden2 = u_hidden2, u_total_h2 = u_total_h2,
        ple = plef, bple = ple_r.bple, u_ple = u_ple, bytes_ple = bytes_ple, ple_dev = ple_r.ple_dev,
        bple_proj = ple_r.bple_proj, u_ple_all = ple_r.u_ple_all, u_ple_sq = ple_r.u_ple_sq,
        hc = hc, png = png, batch_rows = true, bytes_hcd = bytes_hcd, bytes_lr = bytes_lr, bytes_hc = bytes_hc, bytes_ple_in = bytes_ple_in,
        bhc = bhc, bhc_xn = bhc_xn, bhc_gate = bhc_gate, bhc_lo = bhc_lo, bhc_inj_a = bhc_inj_a, bhc_inj_f = bhc_inj_f,
        u_hcd = u_hcd, u_hc = u_hc, u_lr = u_lr, u_lr_tot = u_lr_tot, u_hcd_tot = u_hcd_tot,
        bple_emb = bple_emb, bple_key = bple_key, bple_gated = bple_gated, bple_val = bple_val, u_ple_in = u_ple_in,
        dn = dn, bdn_qkv = bdn_qkv, bdn_cv = bdn_cv, bdn_z = bdn_z, bdn_o = bdn_o, bdn_b = bdn_b, bdn_g = bdn_g,
        u_cd = u_cd, u_nvh = u_nvh, u_nvh2 = u_nvh2, u_di = u_di, dn_ba_rows = dn && t.dn_ba_f32,
        bdn_grp = dn_tab.buf, dn_ng = g_b_dn_ng, dn_npmax = batch_group_rows_max(nrows),
        bdn_qg = bdn_qg, bdn_gate = bdn_gate, u_dn_qd2 = u_dn_qd2, u_total_qd = u_total_qd,
        bx = bx, bxb = bxb, bxb2 = bxb2, bqkv = bqkv, bh12 = bhb, bh12b = bhb2, bpart = bpart,
        bxh = bxh, bfgh = bfgh, bytes_xh = bytes_xh, bytes_fgh = bytes_fgh, xh_ok = xh0,
        bcos = bcos, bsin = bsin, bcos_swa = bcos_swa, bsin_swa = bsin_swa, brt = brt,
        bmoe_lg = bmlg, bmoe_sel = bmsel, bmoe_dn = bmdn, bmoe_rt = bmg4rt, bmg4x = bmg4x, bmgh = bmgh, bmghb = bmgh2,
        bt = bt, mv_kdim = mv_kdim, mv_wo = mv_wo, mv_w2 = mv_w2, wo_sk = wo_sk, w2_sk = w2_sk, cls64 = cls64, fuse13 = fuse13,
        u_dim = u_dim, u_qd = u_qd, u_kvd = u_kvd, u_qkvd = u_qkvd, u_hidden = u_hidden, u_hs = u_hs, u_eps = u_eps,
        u_zero32 = u_zero32, u_one32 = u_one32, u_total_dim = u_total_dim, u_total_h = u_total_h,
        u_hs_sw = u_hs_sw, u_kvd_sw = u_kvd_sw, u_qd_sw = u_qd_sw, u_qkvd_sw = u_qkvd_sw,
        u_moe_ne = u_moe_ne, u_moe_k = u_moe_k, u_moe_nfe = u_moe_nfe, u_moe_knfe = u_moe_knfe, u_moe_ss = u_moe_ss, u_moe_nsh = u_moe_nsh, u_moe_shtot = u_moe_shtot,
        u_moe_kdim = u_moe_kdim, u_moe_tot = u_moe_tot, u_moe_eblk = u_moe_eblk, u_moe_esb = u_moe_esb,
        u_moe_xs_dn = u_moe_xs_dn, u_moe_invd = u_moe_invd, u_moe_gmode = u_moe_gmode,
        k_dim = k_dim, k_qd = uint(qd), k_kvd = uint(kv_dim), k_qkvd = uint(qkvd), k_hs = uint(head_size), k_nh = uint(n_heads),
        k_npq = uint(qd / 2l), k_npall = uint((qd + kv_dim) / 2l), k_nblk = uint(kv_dim / 32l), k_voffe = uint(qd + kv_dim),
        k_kstride = uint(blockc ? kv_dim / 32l : kv_dim), k_kvm = uint(n_heads / c.n_kv_heads),
        k_scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size)),
        k_qd_sw = uint(qd_sw), k_kvd_sw = uint(kvd_sw), k_qkvd_sw = uint(qkvd_sw), k_hs_sw = uint(hs_sw),
        k_npq_sw = uint(qd_sw / 2l), k_npall_sw = uint((qd_sw + kvd_sw) / 2l), k_nblk_sw = uint(kvd_sw / 32l), k_voffe_sw = uint(qd_sw + kvd_sw),
        k_kstride_sw = uint(blockc ? kvd_sw / 32l : kvd_sw), k_kvm_sw = uint(n_heads / kvh_sw),
        k_scale_sw = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(hs_sw)),
        k_eps = c.norm_eps, k_neox = c.rope_neox ? 1u : 0u, k_hasb = c.attn_qkv_bias ? 1u : 0u, k_hasn = c.qk_norm ? 1u : 0u,
        k_rot = uint(max(c.rope_dim, 0l)),
        k_moe_nfe = k_moe_nfe, k_moe_eblk = k_moe_eblk, k_moe_esb = k_moe_esb, k_moe_ss = k_moe_ss, k_moe_xs_dn = k_moe_xs_dn,
        k_moe_knfe = k_moe_knfe, k_moe_kdim = k_moe_kdim, k_moe_gmode = k_moe_gmode)
    var le = BatchLayerEnc(r = unsafe(addr(r)))
    var dnli = 0l
    for (l in range64(c.n_layers)) {
        le->encode(t, enc, l, dnli)
        // group boundary: commit this slice so the driver schedules it under the earlier
        // slices' GPU execution; queue order serializes the pieces
        if ((l + 1l) % lay_per_cb == 0l && l + 1l < c.n_layers) {
            metal_end_encoding(enc)
            metal_release(enc)
            if (!pre) {
                metal_commit(cb)
            }
            cbs |> push(cb)   // nolint:PERF026 — see the cbs declaration
            cb = metal_new_command_buffer_unretained(g_queue)
            enc = conc ? metal_new_compute_encoder_concurrent(cb) : metal_new_compute_encoder(cb)
            // fresh encoder + in-order queue serialize cross-cb hazards — start the tracker clean
            hz_arm(conc)   // fresh encoder — start the tracker clean (queue order covers cross-cb)
        }
    }
    var nc_all = false
    if (gpu_logits) {
        // batched final rmsnorm + classifier — the caller's CPU classifier step is skipped; the hyper-connection head mixer stands where the norm would
        if (hc) {
            hc_mix_rows(r, t, enc, r.bhc, t.hc_head_norm_off, t.hc_head_down_off, t.hc_head_up_off, -1l, bxf, null)
        } else {
            var bwfin = upload_region(unsafe(addr < void? >(t.fblob[t.rms_final_off])), uint64(dim * 4l))
            enc_rms_b(enc, bx, 0ul, bwfin, bxf, u_dim, u_eps, nrows)
        }
        enc_batch_site(enc, t, bt, cls_fmt, t.wcls_off, c.vocab_size, dim, c.vocab_size, bxf, blog, u_dim, u_vocab, u_vocab, 0l, mv_kdim, 0l, cls64)
        if (c.final_logit_softcap > 0.0 || bsupp != null) {   //! the final softcap, then the suppressed ids, over the live rows
            enc_cls_epi_rows(enc, blog, bsupp, c.vocab_size, nrows, c.final_logit_softcap, nsupp)
        }
        // zero-copy logits: scatter each staging row into its session's wrapped storage ON the cb
        // (the KV writeback stays). All-or-nothing per step: one unwrappable session keeps the whole
        // step on the landing scatter. Pipe mode writes s.logits ASYNCHRONOUSLY — fixed-token-only.
        if (same) {
            let bytes_arg = uint64(max(nrows, 16l) * 4l)
            var bvarg = pool_acquire(g_pool, g_dev, bytes_arg)
            enc_argmax_rows(enc, blog, bvarg, u_vocab, nrows)
            L.bvarg = bvarg
            L.pooled |> push((buf = bvarg, size = bytes_arg))
        }
        nc_all = !same
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
    if (!pre) {
        metal_commit(cb)
    }
    cbs |> push(cb)   // nolint:PERF026 — see the cbs declaration
    hz_arm(false)   // disarm the oracle with the encoder — a leaked arm would eat the next CAPTURE's stage
    g_us_bencode += int64(get_time_usec(ts_enc))
    // the landing stash (the slot was emptied by its last finish or discard)
    L.cbs <- cbs   // emptied by release_landing's clear(); capacity deliberately retained
    // every run below APPENDS: the same-slab argmax and the embed-scale uniform were pushed during the encode
    L.pooled |> push((buf = bx, size = bytes_x))   // nolint:STYLE012
    L.pooled |> push((buf = bxb, size = bytes_xb))
    L.pooled |> push((buf = bxb2, size = bytes_x))
    L.pooled |> push((buf = bqkv, size = bytes_qkv))
    L.pooled |> push((buf = bhb, size = bytes_h))
    L.pooled |> push((buf = bhb2, size = bytes_h))
    if (twins) {
        L.pooled |> push((buf = bxh, size = bytes_xh))   // nolint:STYLE012
        L.pooled |> push((buf = bfgh, size = bytes_fgh))
    }
    if (use_attn_d || chunked) {
        L.pooled |> push((buf = bpart, size = bytes_part))
    }
    if (bytes_sk > 0ul) {
        L.pooled |> push((buf = bpsk, size = bytes_sk))
    }
    if (moe) {
        L.pooled |> push((buf = bmlg, size = bytes_mlg))   // nolint:STYLE012
        L.pooled |> push((buf = bmsel, size = bytes_msel))
        L.pooled |> push((buf = bmdn, size = bytes_mdn))
        L.upooled |> push((buf = u_moe_ne, size = 4ul))   // nolint:STYLE012
        L.upooled |> push((buf = u_moe_k, size = 4ul))
        L.upooled |> push((buf = u_moe_nfe, size = 4ul))
        L.upooled |> push((buf = u_moe_eblk, size = 4ul))
        L.upooled |> push((buf = u_moe_esb, size = 4ul))
        L.upooled |> push((buf = u_moe_ss, size = 4ul))
        L.upooled |> push((buf = u_moe_xs_dn, size = 4ul))
        L.upooled |> push((buf = u_moe_knfe, size = 4ul))
        L.upooled |> push((buf = u_moe_kdim, size = 4ul))
        L.upooled |> push((buf = u_moe_tot, size = 4ul))
        if (g4moe) {
            L.pooled |> push((buf = bmg4x, size = bytes_mg4x))   // nolint:STYLE012
            L.pooled |> push((buf = bmg4rt, size = bytes_mg4x))
            L.pooled |> push((buf = bmgh, size = bytes_mgh))
            L.pooled |> push((buf = bmgh2, size = bytes_mgh))
            L.upooled |> push((buf = u_moe_invd, size = 4ul))
        }
        L.upooled |> push((buf = u_moe_gmode, size = 4ul))
    }
    if (u_hass != null) {
        L.upooled |> push((buf = u_hass, size = 4ul))
    }
    if (use_attn_d) {
        L.upooled |> push((buf = u_nsgs, size = 4ul))
    } elif (chunked) {
        L.upooled |> push((buf = u_nch, size = 4ul))
    }
    if (gpu_logits) {
        L.pooled |> push((buf = bxf, size = bytes_x))   // nolint:STYLE012
        L.pooled |> push((buf = blog, size = bytes_log))
        L.upooled |> push((buf = u_vocab, size = 4ul))
    }
    if (bsupp != null) {
        L.upooled |> push((buf = bsupp, size = uint64(nsupp * 4l)))
    }
    if (u_winsl != null) {
        L.upooled |> push((buf = u_winsl, size = 4ul))
    }
    if (hetero) {
        L.upooled |> push((buf = u_qd_sw, size = 4ul))   // nolint:STYLE012
        L.upooled |> push((buf = u_kvd_sw, size = 4ul))
        L.upooled |> push((buf = u_qkvd_sw, size = 4ul))
        L.upooled |> push((buf = u_hs_sw, size = 4ul))
    }
    L.upooled |> push((buf = u_softcap, size = 4ul))   // nolint:STYLE012
    L.upooled |> push((buf = u_zero32, size = 4ul))
    L.upooled |> push((buf = bcos, size = bytes_tab))
    L.upooled |> push((buf = bsin, size = bytes_tab))
    if (dual_rope) {
        L.upooled |> push((buf = bcos_swa, size = bytes_tab_sw))   // nolint:STYLE012
        L.upooled |> push((buf = bsin_swa, size = bytes_tab_sw))
    }
    L.upooled |> push((buf = brt, size = bytes_rt))   // nolint:STYLE012
    L.upooled |> push((buf = u_dim, size = 4ul))
    L.upooled |> push((buf = u_qd, size = 4ul))
    L.upooled |> push((buf = u_kvd, size = 4ul))
    L.upooled |> push((buf = u_qkvd, size = 4ul))
    L.upooled |> push((buf = u_hidden, size = 4ul))
    L.upooled |> push((buf = u_hs, size = 4ul))
    L.upooled |> push((buf = u_eps, size = 4ul))
    L.upooled |> push((buf = u_s16, size = 4ul))
    L.upooled |> push((buf = u_total_dim, size = 4ul))
    L.upooled |> push((buf = u_total_h, size = 4ul))
    L.upooled |> push((buf = u_one32, size = 4ul))
    if (u_moe_nsh != null) {
        L.upooled |> push((buf = u_moe_nsh, size = 4ul))   // nolint:STYLE012 - appends to the step's existing list
        L.upooled |> push((buf = u_moe_shtot, size = 4ul))
    }
    if (u_hidden2 != null) {
        L.upooled |> push((buf = u_hidden2, size = 4ul))   // nolint:STYLE012
        L.upooled |> push((buf = u_total_h2, size = 4ul))
    }
    if (plef) {
        if (ple_r.ple_dev) {
            L.pooled |> push((buf = ple_r.bple, size = bytes_ple))   // nolint:STYLE012
            L.pooled |> push((buf = ple_r.bple_proj, size = bytes_ple))
            L.upooled |> push((buf = ple_r.u_ple_sq, size = 4ul))
        } else {
            L.upooled |> push((buf = ple_r.bple, size = bytes_ple))
        }
        L.upooled |> push((buf = ple_r.u_ple_all, size = 4ul))   // nolint:STYLE012
        L.upooled |> push((buf = u_ple, size = 4ul))
    }
    if (row_tok != null) {
        L.upooled |> push((buf = row_tok, size = uint64(nrows * 4l)))
    }
    if (dn) {
        L.pooled |> push((buf = bdn_qkv, size = bytes_dn_cd))   // nolint:STYLE012
        L.pooled |> push((buf = bdn_cv, size = bytes_dn_cd))
        L.pooled |> push((buf = bdn_z, size = bytes_dn_di))
        L.pooled |> push((buf = bdn_o, size = bytes_dn_di))
        L.pooled |> push((buf = bdn_b, size = 2ul * bytes_dn_h))
        L.pooled |> push((buf = bdn_g, size = bytes_dn_h))
        L.upooled |> push((buf = u_cd, size = 4ul))   // nolint:STYLE012
        L.upooled |> push((buf = u_nvh, size = 4ul))
        L.upooled |> push((buf = u_nvh2, size = 4ul))
        L.upooled |> push((buf = u_di, size = 4ul))
        L.upooled |> push((buf = dn_tab.buf, size = dn_tab.bytes))
        L.dn_uids |> reserve(nrows)
        L.dn_pos |> reserve(nrows)
        for (i in range64(nrows)) {
            L.dn_uids |> push(sessions[i].uid)
            L.dn_pos |> push(ws.positions[i] + poff)
        }
        L.dn_soffs |> clear()
        L.dn_soffs |> push_from(g_b_dn_soffs)
        L.dn_epoch = g_dn_pool_epoch
    }
    if (hc) {
        L.pooled |> push((buf = bhc, size = rows_hc * bytes_hcd))   // nolint:STYLE012
        L.pooled |> push((buf = bhc_xn, size = rows_hc * bytes_hcd))
        L.pooled |> push((buf = bhc_gate, size = rows_hc * bytes_hcd))
        L.pooled |> push((buf = bhc_lo, size = rows_hc * bytes_lr))
        L.pooled |> push((buf = bhc_inj_a, size = rows_hc * bytes_hc))
        L.pooled |> push((buf = bhc_inj_f, size = rows_hc * bytes_hc))
        L.upooled |> push((buf = u_hcd, size = 4ul))   // nolint:STYLE012
        L.upooled |> push((buf = u_hc, size = 4ul))
        L.upooled |> push((buf = u_lr, size = 4ul))
        L.upooled |> push((buf = u_lr_tot, size = 4ul))
        L.upooled |> push((buf = u_hcd_tot, size = 4ul))
    }
    if (png) {
        L.upooled |> push((buf = bple_emb, size = uint64(nrows) * bytes_ple_in))
        L.pooled |> push((buf = bple_key, size = rows_hc * bytes_hcd))   // nolint:STYLE012
        L.pooled |> push((buf = bple_gated, size = rows_hc * bytes_hcd))
        L.pooled |> push((buf = bple_val, size = bytes_x))
        L.upooled |> push((buf = u_ple_in, size = 4ul))
    }
    if (c.q_gated) {
        L.pooled |> push((buf = bdn_qg, size = 2ul * bytes_qg))   // nolint:STYLE012
        L.pooled |> push((buf = bdn_gate, size = bytes_qg))
        L.upooled |> push((buf = u_dn_qd2, size = 4ul))   // nolint:STYLE012
        L.upooled |> push((buf = u_total_qd, size = 4ul))
    }
    L.blog = gpu_logits && !nc_all ? blog : null   // nc steps: the GPU scatter already fed s.logits
    L.sameslab = same
    L.grp_nr |> clear()
    if (same) {
        for (n in g_b_grp_nr) {
            if (n > 0l) {
                L.grp_nr |> push(n)
            }
        }
    }
    //! a plain step on a NextN-headed (or carry-armed) model lands each row's post-norm hidden into its session's `mtp_h`, so the stream's next speculative round starts warm - the same-slab verify lands the rows into `mtp_hrows` instead
    let carry = same || c.n_layer_nextn > 0l || g_mtp_carry_hidden
    L.bxf = carry && gpu_logits ? (hc ? bhc : (g_mtp_carry_pre && c.n_layer_nextn <= 0l ? bx : bxf)) : null   //! a hyper-connection model's carry is the wide residual row before the head mixer
    L.bxf_bytes = L.bxf != null ? nrows * (hc ? hcd : dim) * 4l : 0l
    L.xb = null   // the CPU-classifier tail's x_b writeback is the forward's to arm (sync-only; the pre rail needs GPU logits)
    L.sessions |> clear()
    L.sessions |> reserve(nrows)
    L.uids |> clear()
    L.uids |> reserve(nrows)
    L.ncbufs |> clear()
    L.nc_all = nc_all
    L.positions |> resize(nrows)
    L.koffs |> resize(nrows)
    L.voffs |> resize(nrows)
    L.caps |> resize(nrows)
    for (i in range64(nrows)) {
        L.sessions |> push(sessions[i])
        L.uids |> push(sessions[i].uid)
        if (nc_all) {
            L.ncbufs |> push(nc_logits_for(g_dev, *sessions[i]))
        }
        L.positions[i] = ws.positions[i] + poff
        L.koffs[i] = g_b_koffs[i]
        L.voffs[i] = g_b_voffs[i]
        L.caps[i] = g_b_caps[i]
    }
    L.seq = c.seq_len
    L.vocab = c.vocab_size
    L.kdt = kdt
    L.epoch = g_arena_epoch
    L.gpu_logits = gpu_logits
    // hetero (gemma4): per-layer row bytes + per-(row, layer) slab bases — uniform models
    // capture the same values the old l * cap * rowb arithmetic produced
    L.lrowb |> grow_resize(c.n_layers)
    L.lring |> grow_resize(c.n_layers)
    L.lkbase |> grow_resize(nrows * c.n_layers)
    L.lvbase |> grow_resize(nrows * c.n_layers)
    for (l in range64(c.n_layers)) {
        L.lrowb[l] = kv_row_bytes(kdt, layer_kv_dim(c, l))
        L.lring[l] = mirror_ring(t, l)
        for (i in range64(nrows)) {
            L.lkbase[i * c.n_layers + l] = mirror_lbase(t, kdt, L.caps[i], l)
            L.lvbase[i * c.n_layers + l] = mirror_lbase(t, kdt, L.caps[i], l)
        }
    }
    if (!pre) {
        mark_rows_committed(L)
    }
}


// Knobs resolved once at [init]. They were read per decode step, and get_env_variable allocates
// its result in the context heap on every call — see PERF027.
var private g_env_logits = true
var private g_env_batch_mm = false
var private g_env_attn_d = true
var private g_env_mtp_debug = ""

//! Test lever over the DASLLAMA_MTP_DEBUG bisect modes ("cold" logs the failing engagement gate;
//! "" restores; the reject seam is the common round's `set_mtp_force_reject_every`).
def public set_metal_mtp_debug(mode : string) {
    g_env_mtp_debug = mode
}

//! Test and race lever over `DASLLAMA_METAL_ATTN_D`: the single row's and the batched step's split single-pass
//! attention against the chunked per-(row, head) pair, flipped inside one process.
def public set_metal_attn_d(on : bool) {
    g_env_attn_d = on
    discard_pre()         // a step encoded under the other form would serve it one step late
    discard_batch_pre()
}

def public get_metal_attn_d() : bool => g_env_attn_d

[cold_path]   // one-time env resolution for the batched decode knobs; the caller tests one global
def private load_batch_knobs() {
    g_bgemm_min = int64(max(g_env_metal.batch_gemm_min, 2))
    g_bsk = g_env_metal.batch_sk
    g_bncb = int64(clamp(g_env_metal.batch_ncb, 1, 16))
    g_bconcurrent = g_env_metal.batch_concurrent
    g_bfuse = g_env_metal.batch_fuse
    g_bmv = g_env_metal.batch_mv
    g_bmv8 = g_env_metal.batch_mv8
    g_b_pipe = g_env_metal.batch_pipe
    g_b_pre = g_env_metal.batch_pre
}

[hot_path]   // per speculative round, reached through the engine's registered seat owner
def private metal_mtp_owns(t : Model) : bool => t.metal_blob

//! the registered overrides: the drivers, and the pool's spin window follows a step they served
[hot_path]
def private metal_decode_served(t : Model; var s : Session; token, pos : int64) : bool {
    let ok = metal_served(metal_decode_forward(t, s, token, pos))
    if (s.ple_gpu_pending) {   //! the gate took the pre-step: the step built it, or the CPU pre-step runs late for the CPU layers
        if (!ok) {
            ple_pre_decode(t, s, token)
        }
        s.ple_gpu_pending = false
    }
    return ok
}

[hot_path]
def private metal_batch_decode_served(t : Model; var ws : BatchWorkspace; var sessions : array<Session?>; nrows : int64) : bool
    => metal_served(metal_batch_decode_forward(t, ws, sessions, nrows))

[init]
def dasllama_metal_decode_register() {
    // registered but dormant: only select_decode_override("metal") (env rail DASLLAMA_PIN_DECODE
    // in the harnesses) activates it — the batch twin rides the same name — and unsupported
    // shapes decline per call
    register_decode_override("metal", @@metal_decode_served)
    register_batch_decode_override("metal", @@metal_batch_decode_served)
    register_ple_gpu_decode_gate(@@metal_ple_decode_gate)
    register_reload_prep(@@discard_pre_encoded_steps)   // a reload orphans the pre-encoded steps — retire them before the regions drop
    register_mirror_quiesce(@@discard_pre_encoded_steps)
    register_mtp_draft_override("metal", @@metal_mtp_draft_seat)
    register_mtp_verify_override("metal", @@metal_mtp_verify_seat)
    register_mtp_verify_rows_override("metal", @@metal_mtp_verify_rows_cap)
    register_mtp_rollback_override("metal", @@metal_mtp_rollback_seat)
    register_mtp_cold_override("metal", @@metal_mtp_cold_why)
    register_mtp_seam_override("metal", @@metal_mtp_seam_row)
    register_mtp_seat_owner("metal", @@metal_mtp_owns)
    register_embed_dev_gates("metal", @@metal_embed_dev_gate, @@metal_embed_dev_gate)
    register_embed_batch_gate("metal", @@metal_batch_embed_gate)
    g_env_logits = g_env_metal.logits
    g_env_batch_mm = g_env_metal.batch_mm
    g_env_attn_d = g_env_metal.attn_d
    decode_fuse_say()
    if (!g_env_attn_d) {
        to_log(LOG_WARNING, "dasLLAMA metal decode: DASLLAMA_METAL_ATTN_D=0 - the single row, the verify's rows and the batch step take the chunked attention pair past the single-dispatch ceiling (the fused form's control)\n")
    }
    g_env_mtp_debug = g_env_metal.mtp_debug
    g_wait_spin = g_env_metal.wait_spin   // the single-row finish_step reads it too, so it resolves here, not in the batch driver's knob loader
    if (!empty(g_env_metal.decode_skip)) {
        set_metal_decode_skip(g_env_metal.decode_skip)   // routes through the unknown-token guard
        to_log(LOG_WARNING, "dasLLAMA metal decode: SKIP={g_skip} - timing-attribution mode, output is GARBAGE\n")
    }
    if (is_some(g_env_metal.spec)) {   // the spec chain's A/B rail: 0 off, 1 forced, -1 adaptive
        set_metal_decode_speculate(unwrap(g_env_metal.spec))
    }
}
