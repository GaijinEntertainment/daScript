options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_layout shared public
options _cyclomatic_complexity = 0   // format/schema dispatch ladders are the domain shape
options _function_length = 0   // same

require dasllama/dasllama_lint public
require dasllama/dasllama_common
require dasllama/dasllama_math          // g_repack_* backend pointers + active_* layout knobs
require dasllama/dasllama_math_default  // k4_sc_mn — the k4 6-bit sc/mn decoder
require dasllama/dasllama_repack        // k45_nib/k5_hbit/k6_nib — the disk-order K-quant accessors
require dasllama/dasllama_gguf          // K6_SSB — the k6 disk scale stride
require dasllama/dasllama_gemm_schema   // kq_qsb/kq_ssb — the fmt-id plane strides
require dasllama/dasllama_plane   // PlaneI8/PlaneF - q8_region_append reads the model planes
require dasllama/dasllama_par           // maybe_parallel_for + jobque probes
require dasllama/dasllama_load   // StreamReg + the streaming-fill hook contract this file registers into
require dasllama/dasllama_ple   // ple_planes: the per-layer-embedding planes the walkers pack
require daslib/enum_trait   // the plane table walks the KqFmt enum
require daslib/math_bits    // float_bits_to_uint: the nv4 device row's per-tensor scale word
require strings   // the scale-plane name's format prefix
require math

// Disk-format -> compute-layout transforms: the metal-flavor blob transform, the CPU repack
// walkers, and the GPU tier gathers. Requires dasllama_common back for Model/KqFmt; the loader
// reaches the entry points through the hooks registered at [init] (register_model_layout).

//! The pack-code version folded into every .dlim identity: bump it with ANY edit to a pack or
//! repack walker in this file, so a cached image baked by the old code re-bakes instead of
//! being mapped and served (an image's filename hash carries model + schema + this).
let PACK_VERSION = 3

// ===== metal-flavor blob transform (the blob-only .dlim redesign) =====

// A q8 region's blob bind offset (off/32)*34 lands 16B-aligned iff off % 256 == 0; a k6
// region's d-plane bind (NSB*16 + (off/256)*2) needs 4B alignment iff off % 512 == 0.
// k4/k5/q40 quant + scale binds are 16B-multiples by construction.
[arch(at = "../ARCHITECTURE_GPU_QUANT_PLANES.md#metal-kq-split-scale-plane")]
def private metal_blob_off_ok(off : int64; fmt : KqFmt) : bool {
    if (off < 0l) return true
    if (fmt == KqFmt.q8) return off % 256l == 0l
    if (fmt == KqFmt.k6 || fmt == KqFmt.iq4xs || fmt == KqFmt.iq3s4 || fmt == KqFmt.k3 || fmt == KqFmt.iq3s || fmt == KqFmt.iq3xxs || fmt == KqFmt.iq2s || fmt == KqFmt.iq2xs || fmt == KqFmt.iq2xxs) return off % 512l == 0l
    if (fmt == KqFmt.q51) return off % 128l == 0l   // (off/32)*20 and (off/32)*4 both 16B-aligned
    if (fmt == KqFmt.iq4nl32 || fmt == KqFmt.q4032 || fmt == KqFmt.q5032) return off % 256l == 0l   // (off/32)*16 or *20, and (off/32)*2, all 16B-aligned
    return true
}

def private metal_blob_layout_ok(t : Model) : bool {
    let c = t.config
    let nl = c.n_layers + c.n_layer_nextn   //! the NextN head's planes bind like a trunk layer's
    if (long_length(t.wq_offs) < nl) {
        return false
    }
    // a pure-MoE file (qwen3moe) carries no dense FFN arrays — its expert stacks bind instead
    let has_ffn = long_length(t.w1_offs) >= nl
    let has_exps = (c.moe && long_length(t.we1_offs) >= nl &&
        long_length(t.we2_offs) >= nl && long_length(t.we3_offs) >= nl)
    let has_dn = c.recr_mask != 0ul && long_length(t.dnqkv_offs) >= nl
    if (!has_ffn && !has_exps) {
        return false
    }
    for (l in range64(nl)) {
        // recurrent layers carry no attention tensors (offs -1, off_ok true); each dn plane binds at its own format
        let dn_bad = (has_dn && layer_is_recurrent(c, l) &&
            (!metal_blob_off_ok(t.dnqkv_offs[l], fmt_at(t.dnqkv_fmt, l)) ||
             !metal_blob_off_ok(t.dngate_offs[l], fmt_at(t.dngate_fmt, l)) ||
             !metal_blob_off_ok(t.dnbeta_offs[l], fmt_at(t.dnbeta_fmt, l)) ||
             !metal_blob_off_ok(t.dnalpha_offs[l], fmt_at(t.dnalpha_fmt, l)) ||
             !metal_blob_off_ok(t.dnout_offs[l], fmt_at(t.dnout_fmt, l))))
        if (dn_bad ||
                !metal_blob_off_ok(t.wq_offs[l], fmt_at(t.wq_fmt, l)) ||
                !metal_blob_off_ok(t.wk_offs[l], fmt_at(t.wk_fmt, l)) ||
                !metal_blob_off_ok(t.wv_offs[l], fmt_at(t.wv_fmt, l)) ||
                !metal_blob_off_ok(t.wo_offs[l], fmt_at(t.wo_fmt, l)) ||
                (has_ffn && (!metal_blob_off_ok(t.w1_offs[l], fmt_at(t.w1_fmt, l)) ||
                    !metal_blob_off_ok(t.w2_offs[l], fmt_at(t.w2_fmt, l)) ||
                    !metal_blob_off_ok(t.w3_offs[l], fmt_at(t.w3_fmt, l)))) ||
                (has_exps && (!metal_blob_off_ok(t.we1_offs[l], fmt_at(t.we1_fmt, l)) ||
                    !metal_blob_off_ok(t.we2_offs[l], fmt_at(t.we2_fmt, l)) ||
                    !metal_blob_off_ok(t.we3_offs[l], fmt_at(t.we3_fmt, l))))) {
            return false
        }
    }
    // shexp planes: the q8 transcode contiguous at base + l*she (base + stride covers every layer), and each plane the loader kept in its file format
    if (c.n_ff_shexp > 0l) {
        let she = c.dim * c.n_ff_shexp
        if ((she % 256l) != 0l ||
                !metal_blob_off_ok(t.wsh1_off, KqFmt.q8) ||
                !metal_blob_off_ok(t.wsh2_off, KqFmt.q8) ||
                !metal_blob_off_ok(t.wsh3_off, KqFmt.q8)) {
            return false
        }
        for (l in range64(nl)) {
            for (kind in fixed_array(1, 2, 3)) {
                let p = shexp_plane(t, l, kind)
                return false if (!metal_blob_off_ok(p.off, p.f))
            }
        }
    }
    if (!c.shared_weights && emb_kq(t) && !metal_blob_off_ok(t.emb_kq_off, t.emb_fmt)) {
        return false
    }
    // E-series PLE gate/proj planes bind per layer at their tags (the f32 rows ride fblob); the token table / model-proj stay CPU-side
    if (has_ple(c)) {
        let ples = c.dim * c.n_embd_per_layer
        return false if ((ples % KQ_SB_ELEMS) != 0l)
        for (l in range64(c.n_layers)) {
            for (pp in ple_planes(t, l)) {
                return false if (!pp.f32 && !metal_blob_off_ok(pp.off, pp.f))
            }
        }
    }
    if (c.hyper_conn) {   // qwen4exp: the mixers' q8 down/up planes bind per site, the head pair once
        for (si in range64(long_length(t.hc_down_offs))) {
            if (!metal_blob_off_ok(t.hc_down_offs[si], KqFmt.q8) || !metal_blob_off_ok(t.hc_up_offs[si], KqFmt.q8)) {
                return false
            }
        }
        if (!metal_blob_off_ok(t.hc_head_down_off, KqFmt.q8) || !metal_blob_off_ok(t.hc_head_up_off, KqFmt.q8)
                || (c.n_layer_nextn > 0l && (!metal_blob_off_ok(t.mtp_hc_head_down_off, KqFmt.q8) || !metal_blob_off_ok(t.mtp_hc_head_up_off, KqFmt.q8)))) {
            return false
        }
    }
    if (has_ple_ngram(c) && (!metal_blob_off_ok(t.ple_key_off, KqFmt.q8) || !metal_blob_off_ok(t.ple_val_off, KqFmt.q8))) {
        return false
    }
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    return (metal_blob_off_ok(t.wcls_off, cls_fmt) &&
        (!t.emb_q8 || metal_blob_off_ok(t.emb_q8_off, KqFmt.q8)))
}

//! Can `t`'s LAYOUT take the blob form - the packing flags and the bind-alignment gate, ownership
//! aside? The question a mapped planar image answers before the rail rebakes the metal flavor.
[arch(at = "../ARCHITECTURE_IMAGE.md#image-flavor-rebake")]
def metal_blob_form_ok(t : Model) : bool {
    if (t.quant != QuantMode.q8 || t.kq_repacked || t.mx4_repacked || kernel_backend_needs_repack(active_kernel_backend())) {
        return false
    }
    if (!metal_blob_layout_ok(t)) {
        to_log(LOG_WARNING, "dasLLAMA: metal-blob transform declined - a weight offset violates the GPU bind alignment\n")
        return false
    }
    return true
}

//! Can THIS `t` take the blob form in place? The refusal half of the transform, split out so the
//! image writer can commit to the metal flavor without first building the blob plane in RAM.
def metal_blob_eligible(t : Model) : bool {
    if (t.image_map != null) {
        // image-backed planes are borrowed READ-ONLY views — the in-place k4s/k5s/k6s
        // rewrites would fault. Transform an owned load (load_model_ / DASLLAMA_IMAGE=0).
        to_log(LOG_WARNING, "dasLLAMA: metal-blob transform declined - the model is image-mapped (borrowed planes); transform an owned gguf load\n")
        return false
    }
    return metal_blob_form_ok(t)
}

//! Block count of the blob plane and its stride — the 34B gguf-native block_q8_0, which is the
//! layout the planar load SPLIT into qblob+qscales when it read the file.
def metal_blob_shape(t : Model) : tuple<blocks : int64; stride : int64> =>
    (blocks = long_length(t.qblob) / Q8_BLOCK_ELEMS, stride = Q8_QPB + Q8_SPB)

//! Interleave blob blocks [b0, b0+nblk) from the planar planes into `dst` (>= nblk*34 bytes).
//! The whole-plane call materializes mblob; the image writer calls it band by band instead, which
//! is what keeps a cold metal mint from holding the weights twice.
def metal_blob_fill_blocks(t : Model; b0, nblk : int64; var dst : array<uint8>) {
    if (nblk <= 0l) {
        return
    }
    let mbb = Q8_QPB + Q8_SPB
    let s16 = t.wscale_f16
    unsafe {
        var dp = addr(dst[0])
        let qp = addr < int8 const? >(t.qblob[b0 * Q8_BLOCK_ELEMS])
        var sp : float const?
        var sp16 : uint16 const?
        if (s16) {
            sp16 = addr(t.qscales16[b0])
        } else {
            sp = addr(t.qscales[b0])
        }
        let nch = int(clamp(nblk / 262144l, 1l, 64l))
        maybe_parallel_for(0, int(nblk / 32l) + (nblk % 32l != 0l ? 1 : 0), is_job_que_available() ? nch : 1) $(cb, ce) {
            unsafe {
                for (b in range64(int64(cb) * 32l, min(int64(ce) * 32l, nblk))) {
                    var ph = reinterpret<uint16?>(dp + b * mbb)
                    ph[0] = s16 ? sp16[b] : uint16(f32_to_f16(clamp(sp[b], -65504.0, 65504.0)))
                    memcpy(reinterpret<void?>(dp + b * mbb + Q8_SPB), reinterpret<void const?>(qp + b * Q8_QPB), 32)
                }
            }
        }
    }
}

//! Put ONE kq scale plane into its metal device form, named by its Model field; false = not a kq
//! scale plane. Per-plane because the streamed image walk applies each as its own turn comes,
//! which is the only point where that plane's fills are done and its bytes are not yet written.
[arch(at = "../ARCHITECTURE_GPU_QUANT_PLANES.md#metal-kq-split-scale-plane")]
def metal_blob_scale_plane(var t : Model; name : string) : bool {
    var f = KqFmt.q8
    if (!(name |> ends_with("s")) || !kq_fmt_of_name(slice(name, 0, length(name) - 1), f) || !kq_sb(f) || f == KqFmt.q40 || f == KqFmt.iq4nl || f == KqFmt.nv4) {
        return false
    }
    if (empty(t.kq[int(f)].s)) {
        return true
    }
    if (f == KqFmt.k4 || f == KqFmt.k5) {
        // the 16B verbatim disk scale block per superblock; the 4B pad is repack scratch a blob model never uses
        compact_kq_scale_strip(t.kq[int(f)].s)
        return true
    }
    var @exact_size xc : array<uint8>
    kq_split_scale_plane(f, unsafe(addr<uint8 const?>(t.kq[int(f)].s[0])), long_length(t.kq[int(f)].s) / kq_ssb(f), xc)
    t.kq[int(f)].s <- xc
    return true
}

//! Commit `t` to the metal blob flavor WITHOUT building the big plane: the kq scale strips take
//! their device form and the flag flips, but mblob stays empty and qblob/qscales16 stay put. A
//! committed model serves NOTHING — write its image and drop it, or finish the transform.
def metal_blob_commit(var t : Model) : bool {
    if (!metal_blob_eligible(t)) {
        return false
    }
    for (nm in BLOB_SCALE_PLANES) {
        metal_blob_scale_plane(t, nm)
    }
    t.metal_blob = true
    return true
}

//! Convert a planar q8-mode Model to the metal-blob form IN PLACE — the RAM-serving rail (a model
//! bound for an image commits instead and lets the writer derive). False = cannot take the blob
//! form (not q8, repacked, bind-misaligned); the caller keeps the planar model.
def convert_model_to_metal_blob(var t : Model) : bool {
    if (t.metal_blob) {
        metal_blob_materialize(t)   // a COMMITTED model still owes its plane; a converted one is a no-op
        return true
    }
    if (!metal_blob_commit(t)) {
        return false
    }
    metal_blob_materialize(t)
    return true
}

// Build the blob plane a committed model is still missing and retire the planar sources. The only
// way a model reaches here already whole is the early return above, where qblob is long gone.
def private metal_blob_materialize(var t : Model) {
    let sh = metal_blob_shape(t)
    if (sh.blocks <= 0l) {
        return
    }
    let ts = ref_time_ticks()
    t.mblob |> reserve(sh.blocks * sh.stride)
    t.mblob |> resize_no_init(sh.blocks * sh.stride)
    metal_blob_fill_blocks(t, 0l, sh.blocks, t.mblob)
    delete t.qblob
    delete t.qscales
    delete t.qscales16
    to_log(LOG_INFO, "dasLLAMA: metal-blob transform ({sh.blocks} blocks) in {get_time_usec(ts) / 1000} ms\n")
}

// 20B -> 16B in place: keep bytes [0..16) of every superblock strip, then shrink. Forward
// byte moves are safe — every destination sits below its source (regions can overlap).
def private compact_kq_scale_strip(var ks : array<uint8>) {
    let nsb = long_length(ks) / 20l
    unsafe {
        var p = addr(ks[0])
        for (sb in range64(1l, nsb)) {   // sb 0 already sits at 0
            for (i in range64(16l)) {
                p[sb * 16l + i] = p[sb * 20l + i]
            }
        }
    }
    ks |> resize(nsb * 16l)
}

// ===== CPU repack walkers (backend grp<mr> interleave, in place at load) =====

// A repack work item is common's StreamReg (fmt: the format's stream code, 0 = q8).
// Chunking splits huge regions (the classifier) for load balance; 1024 is a multiple of every
// backend mr, so row groups never straddle a boundary.
def private push_repack(var regs : array<StreamReg>; fmt : int; off, n, d : int64) {
    var r0 = 0l
    while (r0 < d) {
        let rc = min(1024l, d - r0)
        regs |> push(StreamReg(off = off + r0 * n, n = n, d = rc, fmt = fmt))
        r0 += rc
    }
}

// Run every collected repack across the jobque — regions are disjoint, so any lane count is
// bit-exact. Workers touch hoisted raw pointers + the hoisted backend fn values only.
def private repack_regions(var t : Model; regs : array<StreamReg>) {
    if (empty(regs)) {
        return
    }
    // hoisted as VALUES on the arming thread — lanes can't read math's globals
    let rq8 = active_repack_q8q8()
    let rkq = active_repack_kq()
    let rmx = active_repack_mx4()
    // plane bases and per-32 repacks by int(KqFmt): a region's format
    var kqp, ksp : array<uint8?>
    var rb32 : array<RepackQ51Fn>
    kqp |> resize(KQ_FMT_COUNT)
    ksp |> resize(KQ_FMT_COUNT)
    rb32 |> resize(KQ_FMT_COUNT)
    for (f in type<KqFmt>) {
        kq_plane_pair(t, f) $(var q : array<uint8>; var s : array<uint8>) {
            if (!empty(q)) {
                kqp[int(f)] = unsafe(addr(q[0]))
            }
            if (!empty(s)) {
                ksp[int(f)] = unsafe(addr(s[0]))
            }
        }
        if (kq_dense_b32(f)) {
            rb32[int(f)] = active_repack_b32(f)
        }
    }
    unsafe {
        var qbp : int8? = null
        var qsp : float? = null
        if (!empty(t.qblob)) { qbp = addr(t.qblob[0]) }
        if (!empty(t.qscales)) { qsp = addr(t.qscales[0]) }
        var mxqp = kqp[int(KqFmt.mx4)]
        var mxsp = ksp[int(KqFmt.mx4)]
        let rp = addr<StreamReg const?>(regs[0])
        let nr = length(regs)
        let njobs = is_job_que_available() ? min(nr, 4 * (get_total_hw_jobs() + 1)) : 1
        var kqpp = addr(kqp[0])
        var kspp = addr(ksp[0])
        var rb32p = addr(rb32[0])
        maybe_parallel_for(0, nr, njobs) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    let f = rp[i].fmt
                    if (f == 0) {
                        invoke(rq8, qbp + rp[i].off, qsp + rp[i].off / Q8_BLOCK_ELEMS, rp[i].n, rp[i].d)
                    } elif (f == MX4_STREAM_TAG) {
                        let mb = rp[i].off / KQ_B32_ELEMS
                        invoke(rmx, mxqp + rp[i].off / 2l, mxsp + mb, rp[i].n, rp[i].d)
                    } else {
                        var rf = KqFmt.q8
                        if (!kq_fmt_of_stream_code(f, rf)) {
                            panic("repack_regions: region code {f} names no kq format")
                        }
                        let u = rp[i].off / kq_elems(rf)
                        var qp = kqpp[int(rf)] + u * kq_qsb(rf)
                        var sp = kspp[int(rf)] + u * kq_ssb(rf)
                        if (kq_sb(rf)) {
                            invoke(rkq, kq_schema_id(rf), qp, sp, rp[i].n, rp[i].d)
                        } else {
                            invoke(rb32p[int(rf)], qp, sp, rp[i].n, rp[i].d)
                        }
                    }
                }
            }
        }
        kqp |> clear()
        ksp |> clear()
        delete kqp
        delete ksp
    }
    delete rb32
}

// Repack every Q8 2D weight tensor in qblob/qscales into the active backend's interleaved layout,
// IN PLACE, once at load. Mirrors forward()'s per-tensor offsets/shapes exactly; the embedding +
// norms live in fblob and are never touched.
def private collect_q8_regions(t : Model; var regs : array<StreamReg>) {
    let c = t.config
    let dim = c.dim
    let hidden = c.hidden_dim
    for (l in range64(c.n_layers + c.n_layer_nextn)) {   // + the MTP/NextN block at index n_layers
        let kv = layer_kv_dim(c, l)
        let qd = layer_qd(c, l)
        // K-quant-tagged weights live in the kq planes (disk order, never repacked here) — their offsets are NOT qblob offsets
        if (t.wq_offs[l] >= 0l && fmt_at(t.wq_fmt, l) == KqFmt.q8) {
            push_repack(regs, 0, t.wq_offs[l], dim, qd * (c.q_gated ? 2l : 1l))
        }
        if (t.wk_offs[l] >= 0l && fmt_at(t.wk_fmt, l) == KqFmt.q8) {   // absent on E-series shared-KV layers
            push_repack(regs, 0, t.wk_offs[l], dim, kv)
        }
        if (t.wv_offs[l] >= 0l && fmt_at(t.wv_fmt, l) == KqFmt.q8) {
            push_repack(regs, 0, t.wv_offs[l], dim, kv)
        }
        if (t.wo_offs[l] >= 0l && fmt_at(t.wo_fmt, l) == KqFmt.q8) {
            push_repack(regs, 0, t.wo_offs[l], qd, dim)
        }
        if (layer_is_recurrent(c, l)) {  // deltanet projections (kq-tagged planes repack in the kq walk)
            let cd = dn_conv_dim(c)
            if (fmt_at(t.dnqkv_fmt, l) == KqFmt.q8) {
                push_repack(regs, 0, t.dnqkv_offs[l], dim, cd)
            }
            if (fmt_at(t.dngate_fmt, l) == KqFmt.q8) {
                push_repack(regs, 0, t.dngate_offs[l], dim, c.ssm_d_inner)
            }
            if (t.dnbeta_offs[l] >= 0l && fmt_at(t.dnbeta_fmt, l) == KqFmt.q8) {   // -1 when beta/alpha ship F32 (fblob, no repack)
                push_repack(regs, 0, t.dnbeta_offs[l], dim, c.ssm_dt_rank)
            }
            if (t.dnalpha_offs[l] >= 0l && fmt_at(t.dnalpha_fmt, l) == KqFmt.q8) {
                push_repack(regs, 0, t.dnalpha_offs[l], dim, c.ssm_dt_rank)
            }
            if (fmt_at(t.dnout_fmt, l) == KqFmt.q8) {
                push_repack(regs, 0, t.dnout_offs[l], c.ssm_d_inner, dim)
            }
        }
        if (has_ple(c) && l < c.n_layers) {  // gemma-4 E-series per-layer PLE gate/proj (the emb table stays linear, unpacked)
            for (pp in ple_planes(t, l)) {
                if (!pp.f32 && pp.f == KqFmt.q8) {
                    push_repack(regs, 0, pp.off, pp.n, pp.d)
                }
            }
        }
        if (c.n_expert > 0l && l >= c.n_layer_dense_lead) {   // MoE: each expert slice is its own 2D tensor, repacked per its shape
            let ege = dim * c.n_ff_exp
            if (t.experts_mx4) {   //! native-MXFP4 stacks live in mxq/mxs and take the mx layout of the q8 walk's backend
                for (e in range64(c.n_expert)) {
                    push_repack(regs, MX4_STREAM_TAG, t.we1_offs[l] + e * ege, dim, c.n_ff_exp)
                    push_repack(regs, MX4_STREAM_TAG, t.we2_offs[l] + e * ege, c.n_ff_exp, dim)
                    push_repack(regs, MX4_STREAM_TAG, t.we3_offs[l] + e * ege, dim, c.n_ff_exp)
                }
            } else {
                for (e in range64(c.n_expert)) {
                    if (fmt_at(t.we1_fmt, l) == KqFmt.q8) {
                        push_repack(regs, 0, t.we1_offs[l] + e * ege, dim, c.n_ff_exp)
                    }
                    if (fmt_at(t.we2_fmt, l) == KqFmt.q8) {
                        push_repack(regs, 0, t.we2_offs[l] + e * ege, c.n_ff_exp, dim)
                    }
                    if (fmt_at(t.we3_fmt, l) == KqFmt.q8) {
                        push_repack(regs, 0, t.we3_offs[l] + e * ege, dim, c.n_ff_exp)
                    }
                }
            }
            if (c.n_ff_shexp > 0l) {
                let she = dim * c.n_ff_shexp
                if (shexp_plane_q8(t, l, 1)) {   //! the transcode regions the load filled: a q8-tagged plane's alone
                    push_repack(regs, 0, t.wsh1_off + l * she, dim, c.n_ff_shexp)
                }
                if (shexp_plane_q8(t, l, 2)) {
                    push_repack(regs, 0, t.wsh2_off + l * she, c.n_ff_shexp, dim)
                }
                if (shexp_plane_q8(t, l, 3)) {
                    push_repack(regs, 0, t.wsh3_off + l * she, dim, c.n_ff_shexp)
                }
            }
            if (c.moe_dense_shexp) {  // gemma4: the dense parallel shared expert repacks like a plain FFN
                if (fmt_at(t.w1_fmt, l) == KqFmt.q8) {
                    push_repack(regs, 0, t.w1_offs[l], dim, hidden)
                }
                if (fmt_at(t.w2_fmt, l) == KqFmt.q8) {
                    push_repack(regs, 0, t.w2_offs[l], hidden, dim)
                }
                if (fmt_at(t.w3_fmt, l) == KqFmt.q8) {
                    push_repack(regs, 0, t.w3_offs[l], dim, hidden)
                }
            }
        } else {
            // dense arch layer OR a glm4moe dense-lead layer (a plain FFN, no expert stacks)
            let h = layer_hidden(t, l)
            if (fmt_at(t.w1_fmt, l) == KqFmt.q8) {
                push_repack(regs, 0, t.w1_offs[l], dim, h)
            }
            if (fmt_at(t.w2_fmt, l) == KqFmt.q8) {
                push_repack(regs, 0, t.w2_offs[l], h, dim)
            }
            if (fmt_at(t.w3_fmt, l) == KqFmt.q8) {
                push_repack(regs, 0, t.w3_offs[l], dim, h)
            }
        }
    }
    if ((!c.shared_weights && t.wcls_fmt == KqFmt.q8) || t.cls_q8 || t.cls_q8_copy) {
        // the linear copy at emb_q8_off is deliberately NOT repacked — embedding-row dequant indexes it directly
        push_repack(regs, 0, t.wcls_off, dim, c.vocab_size)
    }
    if (c.n_layer_nextn > 0l && t.mtp_ehproj_fmt == KqFmt.q8) {   // NextN eh_proj: 2*dim -> dim
        push_repack(regs, 0, t.mtp_ehproj_off, 2l * dim, dim)
    }
    if (c.hyper_conn) {   // qwen4exp mixers: per (layer, site) down [low_rank x hc_dim] + up [hc_dim x low_rank], and the head pair
        let hcd = hc_dim(c)
        for (si in range64(long_length(t.hc_down_offs))) {
            push_repack(regs, 0, t.hc_down_offs[si], hcd, c.hc_low_rank)
            push_repack(regs, 0, t.hc_up_offs[si], c.hc_low_rank, hcd)
        }
        push_repack(regs, 0, t.hc_head_down_off, hcd, c.hc_low_rank)
        push_repack(regs, 0, t.hc_head_up_off, c.hc_low_rank, hcd)
        if (c.n_layer_nextn > 0l) {   // the draft head's own head mixer pair
            push_repack(regs, 0, t.mtp_hc_head_down_off, hcd, c.hc_low_rank)
            push_repack(regs, 0, t.mtp_hc_head_up_off, c.hc_low_rank, hcd)
        }
    }
    if (has_ple_ngram(c)) {   // qwen4exp n-gram side input: key [hc_dim x in] + value [dim x in] (the hash table is raw bytes, never repacked)
        push_repack(regs, 0, t.ple_key_off, ple_in_dim(c), hc_dim(c))
        push_repack(regs, 0, t.ple_val_off, ple_in_dim(c), dim)
    }
}

def private repack_q8_weights(var t : Model) {
    var regs : array<StreamReg>
    collect_q8_regions(t, regs)
    repack_regions(t, regs)
    t.mx4_repacked = true
    delete regs
}

// route one kq-tagged tensor to its format's plane pair (q8 tags no-op — those live in qblob)
def private push_repack_kq(var regs : array<StreamReg>; fmt : KqFmt; woff, n, d : int64) {
    if (kq_sb(fmt)) {
        push_repack(regs, kq_stream_code(fmt), woff, n, d)
    }
}

// Repack every K-quant-tagged weight tensor into the active backend's grp<mr> kq layout, IN
// PLACE — the kq sibling of repack_q8_weights, called only when the backend carries kq slots.
// The tied/untied classifier repacks too: embed_row gathers its rows through the grp layout after.
def private collect_kq_regions(t : Model; var regs : array<StreamReg>) {
    let c = t.config
    let dim = c.dim
    for (l in range64(c.n_layers + c.n_layer_nextn)) {   // + the MTP/NextN block at index n_layers
        let kv = layer_kv_dim(c, l)
        let qd = layer_qd(c, l)
        // recurrent (deltanet) layers carry no attention projections — their kq planes are the dn triple and beta/alpha
        if (layer_is_recurrent(c, l)) {
            let cd = dn_conv_dim(c)
            push_repack_kq(regs, fmt_at(t.dnqkv_fmt, l), t.dnqkv_offs[l], dim, cd)
            push_repack_kq(regs, fmt_at(t.dngate_fmt, l), t.dngate_offs[l], dim, c.ssm_d_inner)
            push_repack_kq(regs, fmt_at(t.dnout_fmt, l), t.dnout_offs[l], c.ssm_d_inner, dim)
            if (!t.dn_ba_f32) {
                push_repack_kq(regs, fmt_at(t.dnbeta_fmt, l), t.dnbeta_offs[l], dim, c.ssm_dt_rank)
                push_repack_kq(regs, fmt_at(t.dnalpha_fmt, l), t.dnalpha_offs[l], dim, c.ssm_dt_rank)
            }
        }
        if (t.wq_offs[l] >= 0l) {
            push_repack_kq(regs, fmt_at(t.wq_fmt, l), t.wq_offs[l], dim, qd * (c.q_gated ? 2l : 1l))
        }
        if (t.wk_offs[l] >= 0l) {
            push_repack_kq(regs, fmt_at(t.wk_fmt, l), t.wk_offs[l], dim, kv)
        }
        if (t.wv_offs[l] >= 0l) {
            push_repack_kq(regs, fmt_at(t.wv_fmt, l), t.wv_offs[l], dim, kv)
        }
        if (t.wo_offs[l] >= 0l) {
            push_repack_kq(regs, fmt_at(t.wo_fmt, l), t.wo_offs[l], qd, dim)
        }
        if (has_ple(c) && l < c.n_layers) {
            for (pp in ple_planes(t, l)) {
                if (!pp.f32) {
                    push_repack_kq(regs, pp.f, pp.off, pp.n, pp.d)
                }
            }
        }
        if (c.n_expert > 0l && l >= c.n_layer_dense_lead) {
            if (!t.experts_mx4) {   // each expert slice grp-repacks independently, like the Q8 loop
                let ege = dim * c.n_ff_exp
                for (e in range64(c.n_expert)) {
                    push_repack_kq(regs, fmt_at(t.we1_fmt, l), t.we1_offs[l] + e * ege, dim, c.n_ff_exp)
                    push_repack_kq(regs, fmt_at(t.we2_fmt, l), t.we2_offs[l] + e * ege, c.n_ff_exp, dim)
                    push_repack_kq(regs, fmt_at(t.we3_fmt, l), t.we3_offs[l] + e * ege, dim, c.n_ff_exp)
                }
            }
            if (c.moe_dense_shexp) {
                push_repack_kq(regs, fmt_at(t.w1_fmt, l), t.w1_offs[l], dim, c.hidden_dim)
                push_repack_kq(regs, fmt_at(t.w2_fmt, l), t.w2_offs[l], c.hidden_dim, dim)
                push_repack_kq(regs, fmt_at(t.w3_fmt, l), t.w3_offs[l], dim, c.hidden_dim)
            }
            if (c.n_ff_shexp > 0l && l < long_length(t.wshk1_offs)) {
                if (t.wshk1_offs[l] >= 0l) {
                    push_repack_kq(regs, fmt_at(t.wsh1_fmt, l), t.wshk1_offs[l], dim, c.n_ff_shexp)
                }
                if (t.wshk2_offs[l] >= 0l) {
                    push_repack_kq(regs, fmt_at(t.wsh2_fmt, l), t.wshk2_offs[l], c.n_ff_shexp, dim)
                }
                if (t.wshk3_offs[l] >= 0l) {
                    push_repack_kq(regs, fmt_at(t.wsh3_fmt, l), t.wshk3_offs[l], dim, c.n_ff_shexp)
                }
            }
        } else {
            // dense arch layer OR a glm4moe dense-lead layer; pure-MoE arches never reach here
            let h = layer_hidden(t, l)
            push_repack_kq(regs, fmt_at(t.w1_fmt, l), t.w1_offs[l], dim, h)
            push_repack_kq(regs, fmt_at(t.w2_fmt, l), t.w2_offs[l], h, dim)
            push_repack_kq(regs, fmt_at(t.w3_fmt, l), t.w3_offs[l], dim, h)
        }
    }
    if ((!c.shared_weights && t.wcls_fmt != KqFmt.q8) || cls_kq(t)) {
        push_repack_kq(regs, c.shared_weights ? t.emb_fmt : t.wcls_fmt, t.wcls_off, dim, c.vocab_size)
    }
    if (!c.shared_weights && emb_kq(t)) {
        push_repack_kq(regs, t.emb_fmt, t.emb_kq_off, dim, c.vocab_size)
    }
    if (c.n_layer_nextn > 0l) {   // NextN eh_proj (no-op for q8 tags)
        push_repack_kq(regs, t.mtp_ehproj_fmt, t.mtp_ehproj_off, 2l * dim, dim)
    }
}

def private repack_kq_weights(var t : Model) {
    var regs : array<StreamReg>
    collect_kq_regions(t, regs)
    repack_regions(t, regs)
    delete regs
}

// A per-32 format's expert stacks -> the active backend's grp<mr> layout of that format (per-stack:
// a model can mix q51 downs with k4 gates, e.g. gemma-4-26B-A4B, or iq4nl32 downs with iq3s
// gates, Qwen3.8-Flash-Next). No-op unless a stack carries the tag.
def private collect_b32_regions(t : Model; var regs : array<StreamReg>; fmt : KqFmt) {
    let c = t.config
    let dim = c.dim
    let ege = dim * c.n_ff_exp
    let code = kq_stream_code(fmt)
    for (l in range64(c.n_layers)) {
        let f1q = fmt_at(t.we1_fmt, l) == fmt
        let f2q = fmt_at(t.we2_fmt, l) == fmt
        let f3q = fmt_at(t.we3_fmt, l) == fmt
        if (!(f1q || f2q || f3q)) {
            continue
        }
        for (e in range64(c.n_expert)) {
            let base = e * ege
            if (f1q) {
                push_repack(regs, code, t.we1_offs[l] + base, dim, c.n_ff_exp)
            }
            if (f2q) {
                push_repack(regs, code, t.we2_offs[l] + base, c.n_ff_exp, dim)
            }
            if (f3q) {
                push_repack(regs, code, t.we3_offs[l] + base, dim, c.n_ff_exp)
            }
        }
    }
    collect_b32_dense_regions(t, regs, fmt)
}

//! the dense attention and FFN planes tagged `fmt` (`kq_fmt_row_ok` keeps their per-32 tags), each its own 2D region
def private collect_b32_dense_regions(t : Model; var regs : array<StreamReg>; fmt : KqFmt) {
    let c = t.config
    let dim = c.dim
    let code = kq_stream_code(fmt)
    for (l in range64(c.n_layers + c.n_layer_nextn)) {
        let qd = layer_qd(c, l)
        let kv = layer_kv_dim(c, l)
        let h = layer_hidden(t, l)
        for (r in [(f = fmt_at(t.wq_fmt, l), off = l < long_length(t.wq_offs) ? t.wq_offs[l] : -1l, n = dim, d = qd * (c.q_gated ? 2l : 1l)),
                   (f = fmt_at(t.wk_fmt, l), off = l < long_length(t.wk_offs) ? t.wk_offs[l] : -1l, n = dim, d = kv),
                   (f = fmt_at(t.wv_fmt, l), off = l < long_length(t.wv_offs) ? t.wv_offs[l] : -1l, n = dim, d = kv),
                   (f = fmt_at(t.wo_fmt, l), off = l < long_length(t.wo_offs) ? t.wo_offs[l] : -1l, n = qd, d = dim),
                   (f = fmt_at(t.w1_fmt, l), off = l < long_length(t.w1_offs) ? t.w1_offs[l] : -1l, n = dim, d = h),
                   (f = fmt_at(t.w2_fmt, l), off = l < long_length(t.w2_offs) ? t.w2_offs[l] : -1l, n = h, d = dim),
                   (f = fmt_at(t.w3_fmt, l), off = l < long_length(t.w3_offs) ? t.w3_offs[l] : -1l, n = dim, d = h)]) {
            if (r.f == fmt && r.off >= 0l) {
                push_repack(regs, code, r.off, r.n, r.d)
            }
        }
    }
    collect_b32_table_regions(t, regs, fmt)
}

//! the table planes tagged `fmt` (`kq_tag` keeps their per-32 tags): the deltanet five, the shared expert's triple,
//! the classifier (the tied token table's plane), an untied token table and the NextN eh_proj, each its own 2D region
def private collect_b32_table_regions(t : Model; var regs : array<StreamReg>; fmt : KqFmt) {
    let c = t.config
    let dim = c.dim
    let code = kq_stream_code(fmt)
    for (l in range64(c.n_layers + c.n_layer_nextn)) {
        if (layer_is_recurrent(c, l)) {
            let cd = dn_conv_dim(c)
            for (r in [(f = fmt_at(t.dnqkv_fmt, l), off = t.dnqkv_offs[l], n = dim, d = cd),
                       (f = fmt_at(t.dngate_fmt, l), off = t.dngate_offs[l], n = dim, d = c.ssm_d_inner),
                       (f = fmt_at(t.dnout_fmt, l), off = t.dnout_offs[l], n = c.ssm_d_inner, d = dim),
                       (f = t.dn_ba_f32 ? KqFmt.q8 : fmt_at(t.dnbeta_fmt, l), off = t.dn_ba_f32 ? -1l : t.dnbeta_offs[l], n = dim, d = c.ssm_dt_rank),
                       (f = t.dn_ba_f32 ? KqFmt.q8 : fmt_at(t.dnalpha_fmt, l), off = t.dn_ba_f32 ? -1l : t.dnalpha_offs[l], n = dim, d = c.ssm_dt_rank)]) {
                if (r.f == fmt && r.off >= 0l) {
                    push_repack(regs, code, r.off, r.n, r.d)
                }
            }
        }
        if (has_ple(c) && l < c.n_layers) {
            for (pp in ple_planes(t, l)) {
                if (!pp.f32 && pp.f == fmt) {
                    push_repack(regs, code, pp.off, pp.n, pp.d)
                }
            }
        }
        if (c.n_ff_shexp > 0l && l < long_length(t.wshk1_offs)) {
            for (r in [(f = fmt_at(t.wsh1_fmt, l), off = t.wshk1_offs[l], n = dim, d = c.n_ff_shexp),
                       (f = fmt_at(t.wsh2_fmt, l), off = t.wshk2_offs[l], n = c.n_ff_shexp, d = dim),
                       (f = fmt_at(t.wsh3_fmt, l), off = t.wshk3_offs[l], n = dim, d = c.n_ff_shexp)]) {
                if (r.f == fmt && r.off >= 0l) {
                    push_repack(regs, code, r.off, r.n, r.d)
                }
            }
        }
    }
    if (cls_fmt_of(t) == fmt && (!c.shared_weights || emb_kq(t))) {
        push_repack(regs, code, t.wcls_off, dim, c.vocab_size)
    }
    if (!c.shared_weights && emb_kq(t) && t.emb_fmt == fmt) {
        push_repack(regs, code, t.emb_kq_off, dim, c.vocab_size)
    }
    if (c.n_layer_nextn > 0l && t.mtp_ehproj_fmt == fmt) {
        push_repack(regs, code, t.mtp_ehproj_off, 2l * dim, dim)
    }
}

//! every plane of per-32 format `fmt` repacked to the active grp<mr>
def private repack_b32_stacks(var t : Model; fmt : KqFmt) {
    var regs : array<StreamReg>
    collect_b32_regions(t, regs, fmt)
    if (!empty(regs)) {
        repack_regions(t, regs)
        to_log(LOG_INFO, "dasLLAMA: {fmt} planes repacked to grp{active_b32_layout_mr(fmt)}\n")
    }
    delete regs
}

// StreamRepackFn: one region through the active backend's repack, on caller-supplied pointers
// (the streamed fill biases the q pointer into its temp; scales stay in the RAM plane)
def private stream_repack_one(fmt : int; qp, sp : void?; n, d : int64) {
    // inline on the loader/converter thread — the override-aware selectors are safe here
    unsafe {
        if (fmt == 0) {
            invoke(active_repack_q8q8(), reinterpret<int8?>(qp), reinterpret<float?>(sp), n, d)
            return
        }
        var rf = KqFmt.q8
        if (!kq_fmt_of_stream_code(fmt, rf)) {
            panic("stream_repack_one: region code {fmt} names no plane format")
        }
        var q = reinterpret<uint8?>(qp)
        var s = reinterpret<uint8?>(sp)
        if (rf == KqFmt.mx4) {
            invoke(active_repack_mx4(), q, s, n, d)
        } elif (kq_dense_b32(rf)) {
            invoke(active_repack_b32(rf), q, s, n, d)
        } else {
            invoke(active_repack_kq(), kq_schema_id(rf), q, s, n, d)
        }
    }
}

// ===== GPU tier gathers (prepared planes -> device-layout bytes) =====

// Gather one PREPARED Q8 weight stack into the GPU tier's two row-major planes — int8 quants +
// f16 halfword scales. slice_rows is the repack unit (each expert slice interleaves independently).
// Reads whatever the active backend prepared (grp<mr> interleave or plain row-major).
def moe_gpu_gather_stack(t : Model; woff : int64; n, rows, slice_rows : int64; mr, kgroup, wbias : int64;
                         var wq : array<uint8>; var ws : array<uint8>) {
    let nb = n / Q8_BLOCK_ELEMS
    assert(rows * n <= 2147483647l, "GPU MoE stack exceeds the int32 gather-buffer rail")
    wq |> grow_resize(rows * n)
    ws |> grow_resize(rows * nb * Q8_SPB)
    unsafe {
        if (t.wscale_f16) {
            q8_gather_rows(addr<int8 const?>(t.qblob[woff]), addr<uint16 const?>(t.qscales16[woff / Q8_BLOCK_ELEMS]), null,
                n, rows, slice_rows, mr, kgroup, wbias, wq, ws)
        } else {
            q8_gather_rows(addr<int8 const?>(t.qblob[woff]), null, addr<float const?>(t.qscales[woff / Q8_BLOCK_ELEMS]),
                n, rows, slice_rows, mr, kgroup, wbias, wq, ws)
        }
    }
}

let Q8_GATHER_SLICE_ROWS = 64l   //! a cold gather's parallel slice, where a region's rows divide by it

//! a whole region's gather slice: the fixed slice where it divides the rows and the group, else the whole region
def q8_gather_slice(rows, mr : int64) : int64
    => rows % Q8_GATHER_SLICE_ROWS == 0l && Q8_GATHER_SLICE_ROWS % mr == 0l ? Q8_GATHER_SLICE_ROWS : rows

//! The gather's core over raw planes: `rows` x `n` prepared Q8 rows at `wp` (grp<mr> interleaved per `slice_rows`, or plain
//! row-major at mr 1) with their scales (f16 halfwords at `s16p`, else f32 at `s32p`) into row-major int8 quants at
//! `wq[dst_q..]` and f16 halfword scales at halfword `dst_s` of `ws` - the tower drivers gather region by region through it
def q8_gather_rows(wp : int8 const?; s16p : uint16 const?; s32p : float const?; n, rows, slice_rows : int64; mr, kgroup, wbias : int64;
                   var wq : array<uint8>; var ws : array<uint8>; dst_q : int64 = 0l; dst_s : int64 = 0l) {
    let nb = n / Q8_BLOCK_ELEMS
    if (wbias != 0l && wbias != 128l) {
        panic("q8_gather_rows: unknown q8 plane bias {wbias}")
    }
    let nslices = int(rows / slice_rows)
    unsafe {
        var wqp = addr(wq[dst_q])
        var wsp = addr<uint16?>(ws[dst_s * Q8_SPB])
        let njobs = is_job_que_available() ? min(nslices, 4 * (get_total_hw_jobs() + 1)) : 1
        maybe_parallel_for(0, nslices, njobs) $(sb, se) {
            unsafe {
                let ng = slice_rows / mr
                for (s in range(sb, se)) {
                    let swoff = int64(s) * slice_rows * n
                    let ssoff = int64(s) * slice_rows * nb
                    for (rr in range64(slice_rows)) {
                        let grouped = rr < ng * mr
                        let g = rr / mr
                        let r = rr % mr
                        for (bi in range64(nb)) {
                            // scale: group-interleaved [g][bi][r] or the row-major tail; the f32
                            // fallback takes the same clamped rounding wscale_convert_f16 applies
                            let si = grouped ? ssoff + (g * nb + bi) * mr + r : ssoff + rr * nb + bi
                            wsp[ssoff + rr * nb + bi] = (s16p != null ? s16p[si]
                                : uint16(f32_to_f16(clamp(s32p[si], -65504.0, 65504.0))))
                            var dst = wqp + swoff + rr * n + bi * 32l
                            if (grouped) {
                                let gbase = swoff + g * mr * n + r * kgroup
                                var kg = (bi * 32l) / kgroup
                                var o = 0l
                                for (_kk in range64(32l / kgroup)) {
                                    let src = gbase + kg * kgroup * mr
                                    for (j in range64(kgroup)) {
                                        dst[o + j] = uint8(int(wp[src + j]) ^ int(wbias))
                                    }
                                    kg++
                                    o += kgroup
                                }
                            } else {
                                let src = swoff + rr * n + bi * 32l
                                for (j in range64(32l)) {
                                    dst[j] = uint8(wp[src + j])
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

//! the GGUF's 34-byte Q8_0 blocks of `tensors` (byte offset in `blob`, elements) split into the device plane pair the q8 GEMV
//! reads: 32 quant bytes a block, an f16 scale a block; each tensor's first block index back, in the list's order (the Vulkan
//! assistant drafter's upload form)
def q8_repack(blob : array<uint8>; tensors : array<tuple<boff : int64; n : int64>>; @exact_size var quants : array<uint8>;
              @exact_size var scales : array<uint16>) : array<int64> {
    var nblk = 0l
    for (te in tensors) {
        nblk += te.n / Q8_BLOCK_ELEMS
    }
    quants |> reserve(nblk * Q8_QPB)
    quants |> resize(nblk * Q8_QPB)
    scales |> reserve(nblk)
    scales |> resize(nblk)
    var bases : array<int64>
    bases |> reserve(long_length(tensors))
    var cur = 0l
    for (te in tensors) {
        bases |> push(cur)
        unsafe {
            let src = addr<uint8 const?>(blob[te.boff])
            for (b in range64(te.n / Q8_BLOCK_ELEMS)) {
                let bp = src + b * (Q8_SPB + Q8_QPB)
                scales[cur] = uint16(uint(bp[0]) | (uint(bp[1]) << 8u))
                memcpy(addr<void?>(quants[cur * Q8_QPB]), reinterpret<void?>(bp + Q8_SPB), Q8_QPB)
                cur++
            }
        }
    }
    return <- bases
}

//! One GEMM region of the q8 planes (`rows` x `n` at element `off`, in the CPU backend's layout `lay`) appended to `staging` as the
//! Metal 34-byte block stream at the 16-byte-aligned cursor `cur`; answers the region's byte offset and advances the cursor.
def q8_region_append(qblob : PlaneI8; qscales : PlaneF; off, n, rows : int64; lay : Q8RepackLayout; var staging : array<uint8>;
                     var cur : uint64&) : uint64 {
    cur = (cur + 15ul) & ~15ul
    let at = cur
    q8_region_to_metal_blob(plane_at(qblob, off), plane_at(qscales, off / Q8_BLOCK_ELEMS), n, rows, lay.repacked, lay.mr, lay.kgroup,
        lay.wbias, unsafe(addr(staging[int64(cur)])))
    cur += uint64((rows * n / Q8_BLOCK_ELEMS) * (Q8_QPB + Q8_SPB))
    return at
}

// Interleave one PREPARED Q8 region (split planes, f32 scales, possibly grp<mr>-repacked) into
// the Metal 34B-block stream (f16 scale + 32 quants per block, row-major) — the whisper-decoder
// upload's per-region step; the region is ONE repack unit (rows/mr groups + a row-major tail).
def q8_region_to_metal_blob(qp : int8 const?; sp : float const?; n, rows : int64;
                            repacked : bool; mr, kgroup, wbias : int64; var dst : uint8?) {
    let nb = n / Q8_BLOCK_ELEMS
    assert(wbias == 0l || wbias == 128l, "q8_region_to_metal_blob: unknown q8 plane bias")
    let ng = repacked ? rows / mr : 0l
    let njobs = is_job_que_available() ? min(int(rows), 4 * (get_total_hw_jobs() + 1)) : 1
    maybe_parallel_for(0, int(rows), njobs) $(rb, re) {
        unsafe {
            for (rr in range64(int64(rb), int64(re))) {
                let grouped = repacked && rr < ng * mr
                let g = rr / mr
                let r = rr % mr
                for (bi in range64(nb)) {
                    var blk = dst + (rr * nb + bi) * 34l
                    let si = grouped ? (g * nb + bi) * mr + r : rr * nb + bi
                    *reinterpret<uint16?>(blk) = uint16(f32_to_f16(clamp(sp[si], -65504.0, 65504.0)))
                    if (grouped) {
                        let gbase = g * mr * n + r * kgroup
                        var kg = (bi * 32l) / kgroup
                        var o = 2l
                        for (_kk in range64(32l / kgroup)) {
                            let src = gbase + kg * kgroup * mr
                            for (j in range64(kgroup)) {
                                blk[o + j] = uint8(int(qp[src + j]) ^ int(wbias))
                            }
                            kg++
                            o += kgroup
                        }
                    } else {
                        let src = rr * n + bi * 32l
                        for (j in range64(32l)) {
                            blk[2l + j] = uint8(qp[src + j])
                        }
                    }
                }
            }
        }
    }
}

//! The codebook formats' two-word device scale row: word 0 = the plane row's f16 d with iq4xs's
//! scales_h above it, word 1 = the eight 4-bit fields (scales_l, or s) packed from the row's eight
//! decoded sub-scale bytes (iq4xs's ls - 32 or the iq3 pair's 1 + 2s).
def private pack_iq_dev_row(fmt : KqFmt; var out : uint8?; d : uint8 const?; sc : uint8 const?; sstride : int64) {
    unsafe {
        out[0] = d[0]
        out[1] = d[1]
        var hi = 0u
        var lo = 0u
        for (ib in range64(8l)) {
            let v = int(int8(sc[ib * sstride]))
            let f = uint(fmt == KqFmt.iq4xs ? v + 32 : (v - 1) >> 1)
            lo |= (f & 15u) << uint(4l * ib)
            hi |= ((f >> 4u) & 3u) << uint(2l * ib)
        }
        out[2] = uint8(hi & 0xFFu)
        out[3] = uint8(hi >> 8u)
        for (b in range64(4l)) {
            out[4l + b] = uint8((lo >> uint(8l * b)) & 0xFFu)
        }
    }
}

//! Gather one PREPARED per-32 expert stack (q51, iq4nl32) into the GPU tier's device planes: the
//! disk layout (`qb` quant bytes and `sb` scale bytes a 32-block, row-major), out of whatever
//! grp<mr> interleave the repack left - the quant bytes as four-byte columns, the scale bytes verbatim per row.
def private moe_gpu_gather_stack_b32(qplane, splane : array<uint8>; qb, sb : int64; woff : int64; n, rows, slice_rows : int64;
                                     repacked : bool; mr : int64; var wq : array<uint8>; var ws : array<uint8>) {
    let nb = n / Q8_BLOCK_ELEMS
    assert(n % Q8_BLOCK_ELEMS == 0l, "GPU MoE per-32 gather: row length must be a 32-block multiple")
    assert(rows * nb * qb <= 2147483647l, "GPU MoE per-32 stack exceeds the int32 gather-buffer rail")
    wq |> grow_resize(rows * nb * qb)
    ws |> grow_resize(rows * nb * sb)
    let nslices = int(rows / slice_rows)
    let qrow = nb * qb
    let srow = nb * sb
    let ng = repacked ? slice_rows / mr : 0l
    unsafe {
        let b0 = woff / Q8_BLOCK_ELEMS
        let qp = addr<uint8 const?>(qplane[b0 * qb])
        let sp = addr<uint8 const?>(splane[b0 * sb])
        var wqp = addr(wq[0])
        var wsp = addr(ws[0])
        let nrows = nslices * int(slice_rows)
        let njobs = is_job_que_available() ? min(nrows, 4 * (get_total_hw_jobs() + 1)) : 1
        maybe_parallel_for(0, nrows, njobs) $(rowb, rowe) {   //! a row a unit: one slice alone still spreads over the lanes
            unsafe {
                for (row in range(rowb, rowe)) {
                    let rr = int64(row)
                    let ri = rr % slice_rows
                    let sliceQ = (rr - ri) * qrow
                    let sliceS = (rr - ri) * srow
                    let grouped = repacked && ri < ng * mr
                    let g = ri / mr
                    let r = ri % mr
                    for (bi in range64(nb)) {
                        var dq = wqp + rr * qrow + bi * qb
                        var dsc = wsp + (rr * nb + bi) * sb
                        if (grouped) {
                            let gq = qp + sliceQ + g * mr * qrow + bi * qb * mr
                            let gs = sp + sliceS + g * mr * srow + bi * sb * mr
                            for (j in range64(qb)) {
                                dq[j] = gq[(j / 4l) * 4l * mr + r * 4l + j % 4l]
                            }
                            for (h in range64(sb)) {
                                dsc[h] = gs[r * sb + h]
                            }
                        } else {
                            let rq = qp + sliceQ + ri * qrow + bi * qb
                            let rs = sp + sliceS + ri * srow + bi * sb
                            for (m in range64(qb)) {
                                dq[m] = rq[m]
                            }
                            for (m in range64(sb)) {
                                dsc[m] = rs[m]
                            }
                        }
                    }
                }
            }
        }
    }
}

//! one stack of per-32 format `fmt` gathered off its own plane pair
def moe_gpu_gather_b32_stack(t : Model; fmt : KqFmt; woff : int64; n, rows, slice_rows : int64;
                             repacked : bool; mr : int64; var wq : array<uint8>;
                             var ws : array<uint8>) {
    kq_plane_pair(t, fmt) $(q, s) {
        moe_gpu_gather_stack_b32(q, s, kq_qsb(fmt), kq_ssb(fmt), woff, n, rows, slice_rows,
            repacked, mr, wq, ws)
    }
}

//! Gather one native-MXFP4 expert stack into the GPU tier's device planes: the disk layout (16 nibble bytes and one E8M0
//! byte a 32-block, row-major), out of whatever grp<mr> interleave the repack left (`repack_mx4_grp`'s order, per expert slice).
def moe_gpu_gather_stack_mx4(t : Model; woff : int64; n, rows, slice_rows : int64;
                             repacked : bool; mr : int64; var wq : array<uint8>; var ws : array<uint8>) {
    let nb = n / Q8_BLOCK_ELEMS
    assert(n % Q8_BLOCK_ELEMS == 0l, "GPU MoE mx4 gather: row length must be a 32-block multiple")
    assert(rows * nb * MX4_QB <= 2147483647l, "GPU MoE mx4 stack exceeds the int32 gather-buffer rail")
    wq |> grow_resize(rows * nb * MX4_QB)
    ws |> grow_resize(rows * nb * MX4_SB)
    let nslices = int(rows / slice_rows)
    let qrow = nb * MX4_QB
    let srow = nb * MX4_SB
    let ng = repacked ? slice_rows / mr : 0l
    unsafe {
        let b0 = woff / Q8_BLOCK_ELEMS
        let qp = addr<uint8 const?>(t.mxq[b0 * MX4_QB])
        let sp = addr<uint8 const?>(t.mxs[b0 * MX4_SB])
        var wqp = addr(wq[0])
        var wsp = addr(ws[0])
        let nrows = nslices * int(slice_rows)
        let njobs = is_job_que_available() ? min(nrows, 4 * (get_total_hw_jobs() + 1)) : 1
        maybe_parallel_for(0, nrows, njobs) $(rowb, rowe) {   //! a row a unit: one slice alone still spreads over the lanes
            unsafe {
                for (row in range(rowb, rowe)) {
                    let rr = int64(row)
                    let ri = rr % slice_rows
                    let sliceQ = (rr - ri) * qrow
                    let sliceS = (rr - ri) * srow
                    let grouped = repacked && ri < ng * mr
                    let g = ri / mr
                    let r = ri % mr
                    for (bi in range64(nb)) {
                        var dq = wqp + rr * qrow + bi * MX4_QB
                        if (grouped) {
                            let gq = qp + sliceQ + g * mr * qrow + bi * MX4_QB * mr
                            for (j in range64(MX4_QB)) {
                                dq[j] = gq[(j / 4l) * 4l * mr + r * 4l + j % 4l]
                            }
                            wsp[rr * nb + bi] = sp[sliceS + g * mr * srow + bi * mr + r]
                        } else {
                            let rq = qp + sliceQ + ri * qrow + bi * MX4_QB
                            for (m in range64(MX4_QB)) {
                                dq[m] = rq[m]
                            }
                            wsp[rr * nb + bi] = sp[sliceS + ri * srow + bi]
                        }
                    }
                }
            }
        }
    }
}

// Gather one PREPARED K-quant weight stack into the GPU tier's device planes — quant payloads
// row-major per superblock in the k/k+16 nibble pairing, plus the device scale rows (decoded 20 B, or
// the codebook formats' two words); grp<mr> interleave or disk-order in, identical device bytes out.
[arch(at = "../ARCHITECTURE_CPU_KERNELS.md#kq-subblock-planes"), arch(at = "../ARCHITECTURE_ENGINE_FORMATS_NV4.md#nv4-site-scale")]
//! The MoE stack's kq gather: `rows` rows of format `fmt` from the Model's planes at `woff` into the device layout, `wq` and
//! `ws` grown to hold them; the nv4 rows' site-scale word read a slice off the model's table.
def moe_gpu_gather_stack_kq(t : Model; fmt : KqFmt; woff : int64; n, rows, slice_rows : int64;
                            repacked : bool; mr : int64; var wq : array<uint8>; var ws : array<uint8>) {
    let nsb = n / KQ_SUPERBLOCK_ELEMS
    let qsb = kq_qsb(fmt)
    let dssb = kq_ssb(fmt)
    assert(rows * nsb * qsb <= 2147483647l, "GPU MoE kq stack exceeds the int32 gather-buffer rail")
    wq |> grow_resize(rows * nsb * qsb)
    ws |> grow_resize(rows * nsb * kq_dev_ssb(fmt))
    //! the nv4 rows' site-scale word a slice; no other member carries one
    var slice_sbits <- [for (sl in range64(fmt == KqFmt.nv4 ? rows / slice_rows : 0l)); float_bits_to_uint(site_scale(t.site_scale, fmt, woff + sl * slice_rows * n))]
    unsafe {
        let sb0 = woff / KQ_SUPERBLOCK_ELEMS
        kq_gather_rows_dev(fmt, addr<uint8 const?>(t.kq[int(fmt)].q[sb0 * qsb]), addr<uint8 const?>(t.kq[int(fmt)].s[sb0 * dssb]),
            n, rows, slice_rows, repacked, mr, wq, ws, 0l, 0l, slice_sbits)
    }
    delete slice_sbits
}

//! The kq gather's core over raw planes: `rows` x `n` prepared K-quant rows at `qp` with their scale rows at `sp` (the CPU backend's
//! grp<mr> interleave a `slice_rows` slice where `repacked`, else disk order) into the device layout at byte `dst_q` of `wq` and
//! `dst_s` of `ws`; `slice_sbits` a slice's site-scale word (nv4's), empty elsewhere. The MoE stack gather and the towers gather through it.
def kq_gather_rows_dev(fmt : KqFmt; qp, sp : uint8 const?; n, rows, slice_rows : int64; repacked : bool; mr : int64;
                       var wq : array<uint8>; var ws : array<uint8>; dst_q, dst_s : int64; slice_sbits : array<uint>) {
    let nsb = n / KQ_SUPERBLOCK_ELEMS
    let qsb = kq_qsb(fmt)    // quant plane stride per superblock (disk == device for quants)
    let dssb = kq_ssb(fmt)   // disk/plane scale stride; device rows decode to kq_dev_ssb(fmt)
    let dev_ssb = kq_dev_ssb(fmt)
    let two_word_row = kq_two_word_row(fmt)
    assert(n % KQ_SUPERBLOCK_ELEMS == 0l, "GPU kq gather: row length must be a superblock multiple")
    let nslices = int(rows / slice_rows)
    let qrow = nsb * qsb
    let srow = nsb * dssb
    let ng = repacked ? slice_rows / mr : 0l
    unsafe {
        var wqp = addr(wq[dst_q])
        var wsp = addr(ws[dst_s])
        let sbp = long_length(slice_sbits) > 0l ? addr<uint const?>(slice_sbits[0]) : null
        let nrows = nslices * int(slice_rows)
        let njobs = is_job_que_available() ? min(nrows, 4 * (get_total_hw_jobs() + 1)) : 1
        maybe_parallel_for(0, nrows, njobs) $(rowb, rowe) {   //! a row a unit: one slice alone still spreads over the lanes
            unsafe {
                for (row in range(rowb, rowe)) {
                    let rr = int64(row)
                    let ri = rr % slice_rows
                    //! the nv4 row's fifth word: its slice's site scale (a slice is one expert's plane)
                    let sbits = sbp != null ? sbp[(rr - ri) / slice_rows] : 0u
                    let sliceQ = (rr - ri) * qrow
                    let sliceS = (rr - ri) * srow
                    let grouped = repacked && ri < ng * mr
                    let g = ri / mr
                    let r = ri % mr
                    for (sbi in range64(nsb)) {
                        var dq = wqp + rr * qrow + sbi * qsb
                        var dsc = wsp + (rr * nsb + sbi) * dev_ssb
                        if (grouped) {
                            let gq = qp + sliceQ + g * mr * qrow + sbi * qsb * mr
                            let gs = sp + sliceS + g * mr * srow + sbi * dssb * mr
                            if (fmt == KqFmt.iq3s) {   // 26 uniform 4-byte columns over [qs][qh][signs]
                                for (m in range64(104l)) {
                                    dq[m] = gq[((m / 4l) * mr + r) * 4l + m % 4l]
                                }
                            } elif (fmt == KqFmt.iq3xxs) {   // 24 uniform 4-byte columns over [qs][aux]
                                for (m in range64(96l)) {
                                    dq[m] = gq[((m / 4l) * mr + r) * 4l + m % 4l]
                                }
                            } elif (fmt == KqFmt.k3) {   // the sub-block-packed qs + hmask columns
                                for (m in range64(64l)) {
                                    let half = (m % 32l) / 16l
                                    let j = (m % 16l) / 4l
                                    var qbyte = 0u
                                    for (bb in range64(4l)) {
                                        let col = ((m / 32l) * 4l + bb) * 2l + half
                                        qbyte |= ((uint(gq[(col * mr + r) * 4l + m % 4l]) >> uint(2l * j)) & 3u) << uint(2l * bb)
                                    }
                                    dq[m] = uint8(qbyte)
                                }
                                for (m in range64(32l)) {
                                    var hbyte = 0u
                                    for (blk in range64(8l)) {
                                        hbyte |= ((uint(gq[64l * mr + (blk * mr + r) * 4l + m % 4l]) >> uint(m / 4l)) & 1u) << uint(blk)
                                    }
                                    dq[64l + m] = uint8(hbyte)
                                }
                            } elif (fmt == KqFmt.k2) {   // 16 qs columns, nothing else
                                for (m in range64(64l)) {
                                    dq[m] = gq[((m / 4l) * mr + r) * 4l + m % 4l]
                                }
                            } elif (fmt == KqFmt.iq2s) {   // 18 uniform 4-byte columns over [idx][signs][qh]
                                for (m in range64(72l)) {
                                    dq[m] = gq[((m / 4l) * mr + r) * 4l + m % 4l]
                                }
                            } elif (fmt == KqFmt.iq2xs) {   // 16 uniform 4-byte columns over the u16 qs words
                                for (m in range64(64l)) {
                                    dq[m] = gq[((m / 4l) * mr + r) * 4l + m % 4l]
                                }
                            } elif (fmt == KqFmt.iq2xxs) {   // 16 uniform 4-byte columns over the u16 qs words
                                for (m in range64(64l)) {
                                    dq[m] = gq[((m / 4l) * mr + r) * 4l + m % 4l]
                                }
                            } else {
                                for (m in range64(128l)) {   // nibble region: verbatim per (blk, j, t)
                                    dq[m] = gq[((m / 4l) * mr + r) * 4l + m % 4l]
                                }
                            }
                            if (fmt == KqFmt.k5) {
                                for (bj in range64(32l)) {
                                    dq[128l + bj] = gq[128l * mr + bj * mr + r]
                                }
                            } elif (fmt == KqFmt.k6) {   // the sub-block-packed qh columns
                                for (p in range64(64l)) {
                                    let half = (p % 32l) / 16l
                                    let j = (p % 16l) / 4l
                                    var qhbyte = 0u
                                    for (bb in range64(4l)) {
                                        let col = ((p / 32l) * 4l + bb) * 2l + half
                                        qhbyte |= ((uint(gq[128l * mr + (col * mr + r) * 4l + p % 4l]) >> uint(2l * j)) & 3u) << uint(2l * bb)
                                    }
                                    dq[128l + p] = uint8(qhbyte)
                                }
                            }
                            if (fmt == KqFmt.k6 || fmt == KqFmt.k3) {
                                for (idx in range64(16l)) {
                                    dsc[idx] = gs[idx * mr + r]
                                }
                                dsc[16] = gs[16l * mr + 2l * r]
                                dsc[17] = gs[16l * mr + 2l * r + 1l]
                                dsc[18] = uint8(0)
                                dsc[19] = uint8(0)
                            } elif (fmt == KqFmt.q40 || fmt == KqFmt.iq4nl || fmt == KqFmt.nv4) {
                                for (blk in range64(8l)) {   // 8 per-block f16 d (nv4: UE4M3 byte pairs), mr-interleaved (iq4nl repacks with q40's interleave)
                                    dsc[blk * 2l] = gs[blk * 2l * mr + 2l * r]
                                    dsc[blk * 2l + 1l] = gs[blk * 2l * mr + 2l * r + 1l]
                                }
                                for (idx in range64(16l, 20l)) {
                                    dsc[idx] = uint8((sbits >> uint((idx - 16l) * 8l)) & 0xFFu)
                                }
                            } elif (fmt == KqFmt.k2) {   // the field-major grp header re-packs to [d][dmin][16 pair bytes]
                                dsc[0] = gs[16l * mr + 2l * r]
                                dsc[1] = gs[16l * mr + 2l * r + 1l]
                                dsc[2] = gs[18l * mr + 2l * r]
                                dsc[3] = gs[18l * mr + 2l * r + 1l]
                                for (idx in range64(16l)) {
                                    dsc[4l + idx] = gs[idx * mr + r]
                                }
                            } elif (two_word_row) {   // the k4-shaped grp row (d at 2r, the eight sub-scale bytes mr-strided from 4mr) packs to two words
                                pack_iq_dev_row(fmt, dsc, gs + 2l * r, gs + 4l * mr + r, mr)
                            } else {
                                dsc[0] = gs[2l * r]
                                dsc[1] = gs[2l * r + 1l]
                                dsc[2] = gs[2l * mr + 2l * r]
                                dsc[3] = gs[2l * mr + 2l * r + 1l]
                                for (blk in range64(8l)) {
                                    dsc[4l + blk] = gs[4l * mr + blk * mr + r]
                                    dsc[12l + blk] = gs[12l * mr + blk * mr + r]
                                }
                            }
                        } else {
                            let rq = qp + sliceQ + ri * qrow + sbi * qsb
                            let rs = sp + sliceS + ri * srow + sbi * dssb
                            if (fmt == KqFmt.q40 || fmt == KqFmt.iq4xs || fmt == KqFmt.iq3s4 || fmt == KqFmt.k3 || fmt == KqFmt.iq3s || fmt == KqFmt.iq3xxs || fmt == KqFmt.iq4nl || fmt == KqFmt.nv4 || fmt == KqFmt.k2 || fmt == KqFmt.iq2s || fmt == KqFmt.iq2xs || fmt == KqFmt.iq2xxs) {
                                for (m in range64(qsb)) {   // disk bytes already in the device form (k/k+16 pairs; k3's lanes)
                                    dq[m] = rq[m]
                                }
                            } else {
                                for (m in range64(128l)) {   // re-pair disk k/k+32 nibbles to k/k+16
                                    let k = (m / 16l) * 32l + m % 16l
                                    let lo = fmt == KqFmt.k6 ? k6_nib(rq, k) : k45_nib(rq, k)
                                    let hi = fmt == KqFmt.k6 ? k6_nib(rq, k + 16l) : k45_nib(rq, k + 16l)
                                    dq[m] = uint8(lo | (hi << 4u))
                                }
                            }
                            if (fmt == KqFmt.k5) {
                                for (bj in range64(32l)) {
                                    let k = (bj / 4l) * 32l + (bj % 4l) * 4l
                                    var hb = 0u
                                    for (tt in range64(4l)) {
                                        hb |= k5_hbit(rq + 128l, k + tt) << uint(tt)
                                        hb |= k5_hbit(rq + 128l, k + 16l + tt) << uint(4l + tt)
                                    }
                                    dq[128l + bj] = uint8(hb)
                                }
                            } elif (fmt == KqFmt.k6) {
                                for (p in range64(64l)) {
                                    dq[128l + p] = rq[128l + p]
                                }
                            }
                            if (two_word_row) {   // the plane row (d, then the eight decoded sub-scale bytes at 4) packs to two words
                                pack_iq_dev_row(fmt, dsc, rs, rs + 4l, 1l)
                            } elif (fmt == KqFmt.k6 || fmt == KqFmt.q40 || fmt == KqFmt.k3 || fmt == KqFmt.iq4nl || fmt == KqFmt.nv4 || fmt == KqFmt.k2 || fmt == KqFmt.iq2s || fmt == KqFmt.iq2xs) {   // rows already in the decoded device form
                                for (idx in range64(dssb)) {
                                    dsc[idx] = rs[idx]
                                }
                                for (idx in range64(dssb, KQ_DEV_SSB)) {
                                    dsc[idx] = uint8((sbits >> uint((idx - 16l) * 8l)) & 0xFFu)
                                }
                            } else {
                                for (idx in range64(4l)) {   // f16 d + f16 dmin verbatim
                                    dsc[idx] = rs[idx]
                                }
                                for (blk in range64(8l)) {   // decode the 6-bit sc/mn packing
                                    let sm = k4_sc_mn(rs + 4l, blk)
                                    dsc[4l + blk] = uint8(sm.x)
                                    dsc[12l + blk] = uint8(sm.y)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// ===== the VAD's device weight slab =====

//! The element offsets of the VAD's tensors in its one weight slab, the layout both GPU drivers upload.
struct VadSlabOffs {
    stft : int64
    c1w : int64
    c1b : int64
    c2w : int64
    c2b : int64
    c3w : int64
    c3b : int64
    c4w : int64
    c4b : int64
    ih : int64
    hh : int64
    lb : int64
    fw : int64
    fb : int64
}

//! `src` appended to the slab, answering its element offset
def private sv_slab_put(var slab : array<float>; src : array<float>) : int64 {
    let off = long_length(slab)
    slab |> push_from(src)
    return off
}

//! `src` [rows][cols] appended to the slab transposed, [cols][rows], answering its element offset
def private sv_slab_put_t(var slab : array<float>; src : array<float>; rows, cols : int64) : int64 {
    var t : array<float>
    transpose_ct(src, rows, cols, t)
    return sv_slab_put(slab, t)
}

//! a VAD model's tensors end to end into `m.slab` and the slab's FNV-1a hash: the STFT basis [stft_ch][nfft] and the
//! LSTM's two weight planes [4 hidden][hidden] transposed (k-major, so a kernel's lanes read consecutive output rows),
//! each conv in its load-time [cin][3][cout] layout. `m` is `dasllama_vad`'s `VadModel`, which requires this file.
def vad_build_slab(var m : auto(VadT); stft_ch, nfft, hidden : int64) {
    m.slab |> clear()
    var o : VadSlabOffs   // nolint:STYLE013 — each offset is the slab's length after the put before it: a constructor's argument order is undefined
    o.stft = sv_slab_put_t(m.slab, m.stft, stft_ch, nfft)
    o.c1w = sv_slab_put(m.slab, m.conv1_w)
    o.c1b = sv_slab_put(m.slab, m.conv1_b)
    o.c2w = sv_slab_put(m.slab, m.conv2_w)
    o.c2b = sv_slab_put(m.slab, m.conv2_b)
    o.c3w = sv_slab_put(m.slab, m.conv3_w)
    o.c3b = sv_slab_put(m.slab, m.conv3_b)
    o.c4w = sv_slab_put(m.slab, m.conv4_w)
    o.c4b = sv_slab_put(m.slab, m.conv4_b)
    o.ih = sv_slab_put_t(m.slab, m.lstm_ih, 4l * hidden, hidden)
    o.hh = sv_slab_put_t(m.slab, m.lstm_hh, 4l * hidden, hidden)
    o.lb = sv_slab_put(m.slab, m.lstm_b)
    o.fw = sv_slab_put(m.slab, m.final_w)
    o.fb = long_length(m.slab)
    m.slab |> push(m.final_b)
    m.slab_offs = o
    var h = 0xcbf29ce484222325ul
    for (v in m.slab) {
        h = (h ^ uint64(unsafe(reinterpret<uint>(v)))) * 0x100000001b3ul
    }
    m.slab_key = h
}

// ===== hook registration (the loader in dasllama_common reaches back through these) =====

[init]
def private register_layout_hooks {
    register_model_layout(@@repack_q8_weights, @@repack_kq_weights, @@repack_b32_stacks, @@moe_gpu_gather_stack, @@moe_gpu_gather_stack_kq,
        @@moe_gpu_gather_b32_stack, @@moe_gpu_gather_stack_mx4)
    register_stream_layout(@@collect_q8_regions, @@collect_kq_regions, @@collect_b32_regions,
        @@stream_repack_one, @@metal_blob_eligible, @@metal_blob_scale_plane)
}
