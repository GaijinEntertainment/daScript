options gen2
options _comment_hygiene = true

module dasllama_layout shared public
options _cyclomatic_complexity = 0   // format/schema dispatch ladders are the domain shape
options _function_length = 0   // same

require dasllama/dasllama_common
require dasllama/dasllama_math          // g_repack_* backend pointers + active_* layout knobs
require dasllama/dasllama_math_default  // k4_sc_mn — the k4 6-bit sc/mn decoder
require dasllama/dasllama_repack        // k45_nib/k5_hbit/k6_nib — the disk-order K-quant accessors
require dasllama/dasllama_gguf          // K6_SSB — the k6 disk scale stride
require dasllama/dasllama_gemm_schema   // kq_qsb/kq_ssb — the fmt-id plane strides
require dasllama/dasllama_par           // maybe_parallel_for + jobque probes
require dasllama/dasllama_load   // StreamReg + the streaming-fill hook contract this file registers into
require math

// Disk-format -> compute-layout transforms: the metal-flavor blob transform, the CPU repack
// walkers, and the GPU tier gathers. Requires dasllama_common back for Model/KqFmt; the loader
// reaches the entry points through the hooks registered at [init] (register_model_layout).

// ===== metal-flavor blob transform (the blob-only .dlim redesign) =====

// A q8 region's blob bind offset (off/32)*34 lands 16B-aligned iff off % 256 == 0; a k6
// region's d-plane bind (NSB*16 + (off/256)*2) needs 4B alignment iff off % 512 == 0.
// k4/k5/q40 quant + scale binds are 16B-multiples by construction.
def private metal_blob_off_ok(off : int64; fmt : KqFmt) : bool {
    if (off < 0l) return true
    if (fmt == KqFmt.q8) return off % 256l == 0l
    if (fmt == KqFmt.k6) return off % 512l == 0l
    if (fmt == KqFmt.q51) return off % 128l == 0l   // (off/32)*20 and (off/32)*4 both 16B-aligned
    return true
}

def private metal_blob_layout_ok(t : Model) : bool {
    let c = t.config
    if (long_length(t.wq_offs) < c.n_layers) {
        return false
    }
    // a pure-MoE file (qwen3moe) carries no dense FFN arrays — its expert stacks bind instead
    let has_ffn = long_length(t.w1_offs) >= c.n_layers
    let has_exps = (c.moe && long_length(t.we1_offs) >= c.n_layers &&
        long_length(t.we2_offs) >= c.n_layers && long_length(t.we3_offs) >= c.n_layers)
    let has_dn = c.recr_mask != 0ul && long_length(t.dnqkv_offs) >= c.n_layers
    if (!has_ffn && !has_exps) {
        return false
    }
    for (l in range64(c.n_layers)) {
        // recurrent layers (Wave D): no attention tensors (offs -1, off_ok true); dn planes must be q8
        let dn_bad = (has_dn && layer_is_recurrent(c, l) &&
            (!dn_layer_q8(t, l) ||
             !metal_blob_off_ok(t.dnqkv_offs[l], KqFmt.q8) ||
             !metal_blob_off_ok(t.dngate_offs[l], KqFmt.q8) ||
             !metal_blob_off_ok(t.dnbeta_offs[l], KqFmt.q8) ||
             !metal_blob_off_ok(t.dnalpha_offs[l], KqFmt.q8) ||
             !metal_blob_off_ok(t.dnout_offs[l], KqFmt.q8)))
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
    // shexp planes: always q8, contiguous at base + l*she — base + stride covers every layer
    if (c.n_ff_shexp > 0l) {
        let she = c.dim * c.n_ff_shexp
        if ((she % 256l) != 0l ||
                !metal_blob_off_ok(t.wsh1_off, KqFmt.q8) ||
                !metal_blob_off_ok(t.wsh2_off, KqFmt.q8) ||
                !metal_blob_off_ok(t.wsh3_off, KqFmt.q8)) {
            return false
        }
    }
    // E-series PLE gate/proj q8 planes bind per layer; the token table / model-proj stay CPU-side
    if (has_ple(c)) {
        let ples = c.dim * c.n_embd_per_layer
        if ((ples % 256l) != 0l ||
                !metal_blob_off_ok(t.ple_gate_off, KqFmt.q8) ||
                !metal_blob_off_ok(t.ple_proj_off, KqFmt.q8)) {
            return false
        }
    }
    let cls_fmt = c.shared_weights ? t.emb_fmt : t.wcls_fmt
    return (metal_blob_off_ok(t.wcls_off, cls_fmt) &&
        (!t.cls_q8 || metal_blob_off_ok(t.emb_q8_off, KqFmt.q8)))
}

//! Can `t` take the blob form? The refusal half of the transform, split out so the image writer
//! can commit to the metal flavor without first building the blob plane in RAM.
def metal_blob_eligible(t : Model) : bool {
    if (t.quant != QuantMode.q8 || t.kq_repacked || t.mx4_repacked || kernel_backend_needs_repack(active_kernel_backend())) {
        return false
    }
    if (t.image_map != null) {
        // image-backed planes are borrowed READ-ONLY views — the in-place k4s/k5s/k6s
        // rewrites would fault. Transform an owned load (load_model_ / DASLLAMA_IMAGE=0).
        to_log(LOG_WARNING, "dasLLAMA: metal-blob transform declined - the model is image-mapped (borrowed planes); transform an owned gguf load\n")
        return false
    }
    if (!metal_blob_layout_ok(t)) {
        to_log(LOG_WARNING, "dasLLAMA: metal-blob transform declined - a weight offset violates the GPU bind alignment\n")
        return false
    }
    return true
}

//! Block count of the blob plane and its stride — the 34B gguf-native block_q8_0, which is the
//! layout the planar load SPLIT into qblob+qscales when it read the file.
def metal_blob_shape(t : Model) : tuple<blocks : int64; stride : int64> {
    return (blocks = long_length(t.qblob) / Q8_BLOCK_ELEMS, stride = Q8_QPB + Q8_SPB)
}

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
def metal_blob_scale_plane(var t : Model; name : string) : bool {
    // k4/k5: the 16B verbatim disk scale block per superblock — the 4B pad is CPU-repack
    // scratch (see the plane-layout comment in dasllama_gguf) and a blob model never repacks
    if (name == "k4s") {
        if (!empty(t.k4s)) {
            compact_kq_scale_strip(t.k4s)
        }
        return true
    }
    if (name == "k5s") {
        if (!empty(t.k5s)) {
            compact_kq_scale_strip(t.k5s)
        }
        return true
    }
    if (name != "k6s") {
        return false
    }
    // k6: the GPU split form — [nsb x 16B sub-scale strips][nsb x f16 d halfwords]
    if (!empty(t.k6s)) {
        var k6c : array<uint8>
        let nsb = long_length(t.k6s) / K6_SSB
        k6c |> reserve_resize(nsb * K6_SSB)   // a 27B-class q6 region is a ~120 MB strip — one grow
        unsafe {
            var dp = addr(k6c[0])
            let sp = addr < uint8 const? >(t.k6s[0])
            for (sb in range64(nsb)) {
                memcpy(reinterpret<void?>(dp + sb * 16l), reinterpret<void const?>(sp + sb * 18l), 16)
                dp[nsb * 16l + sb * 2l] = sp[sb * 18l + 16l]
                dp[nsb * 16l + sb * 2l + 1l] = sp[sb * 18l + 17l]
            }
        }
        t.k6s <- k6c
    }
    return true
}

//! Commit `t` to the metal blob flavor WITHOUT building the big plane: the kq scale strips take
//! their device form and the flag flips, but mblob stays empty and qblob/qscales16 stay put. A
//! committed model serves NOTHING — write its image and drop it, or finish the transform.
def metal_blob_commit(var t : Model) : bool {
    if (!metal_blob_eligible(t)) {
        return false
    }
    for (nm in ["k4s", "k5s", "k6s"]) {
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

// A repack work item is common's StreamReg (fmt: 0 = q8, 1 = mx4, 2 = q51, 4/5/6/40 = kq).
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
    let rq51 = active_repack_q51()
    unsafe {
        var qbp : int8? = null
        var qsp : float? = null
        var mxqp : uint8? = null
        var mxsp : uint8? = null
        var k4qp : uint8? = null
        var k4sp : uint8? = null
        var k5qp : uint8? = null
        var k5sp : uint8? = null
        var k6qp : uint8? = null
        var k6sp : uint8? = null
        var q40qp : uint8? = null
        var q40sp : uint8? = null
        var q51qp : uint8? = null
        var q51sp : uint8? = null
        if (!empty(t.qblob)) { qbp = addr(t.qblob[0]) }
        if (!empty(t.qscales)) { qsp = addr(t.qscales[0]) }
        if (!empty(t.mxq)) { mxqp = addr(t.mxq[0]) }
        if (!empty(t.mxs)) { mxsp = addr(t.mxs[0]) }
        if (!empty(t.k4q)) { k4qp = addr(t.k4q[0]) }
        if (!empty(t.k4s)) { k4sp = addr(t.k4s[0]) }
        if (!empty(t.k5q)) { k5qp = addr(t.k5q[0]) }
        if (!empty(t.k5s)) { k5sp = addr(t.k5s[0]) }
        if (!empty(t.k6q)) { k6qp = addr(t.k6q[0]) }
        if (!empty(t.k6s)) { k6sp = addr(t.k6s[0]) }
        if (!empty(t.q40q)) { q40qp = addr(t.q40q[0]) }
        if (!empty(t.q40s)) { q40sp = addr(t.q40s[0]) }
        if (!empty(t.q51q)) { q51qp = addr(t.q51q[0]) }
        if (!empty(t.q51s)) { q51sp = addr(t.q51s[0]) }
        let rp = addr<StreamReg const?>(regs[0])
        let nr = length(regs)
        let njobs = is_job_que_available() ? min(nr, 4 * (get_total_hw_jobs() + 1)) : 1
        maybe_parallel_for(0, nr, njobs) $(rb, re) {
            unsafe {
                for (i in range(rb, re)) {
                    let f = rp[i].fmt
                    if (f == 0) {
                        invoke(rq8, qbp + rp[i].off, qsp + rp[i].off / Q8_BLOCK_ELEMS, rp[i].n, rp[i].d)
                    } elif (f == 1) {
                        invoke(rmx, mxqp + rp[i].off / 2l, mxsp + rp[i].off / 32l, rp[i].n, rp[i].d)
                    } elif (f == 2) {
                        invoke(rq51, q51qp + (rp[i].off / 32l) * Q51_QPB, q51sp + (rp[i].off / 32l) * Q51_SPB, rp[i].n, rp[i].d)
                    } else {
                        let sb = rp[i].off / 256l
                        let qsb = kq_qsb(f)
                        let ssb = kq_ssb(f)
                        var kqp = f == 4 ? k4qp : (f == 5 ? k5qp : (f == 6 ? k6qp : q40qp))
                        var ksp = f == 4 ? k4sp : (f == 5 ? k5sp : (f == 6 ? k6sp : q40sp))
                        invoke(rkq, f, kqp + sb * qsb, ksp + sb * ssb, rp[i].n, rp[i].d)
                    }
                }
            }
        }
    }
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
            if (t.dnbeta_offs[l] >= 0l) {   // -1 when beta/alpha ship F32 (fblob, no repack)
                push_repack(regs, 0, t.dnbeta_offs[l], dim, c.ssm_dt_rank)
                push_repack(regs, 0, t.dnalpha_offs[l], dim, c.ssm_dt_rank)
            }
            if (fmt_at(t.dnout_fmt, l) == KqFmt.q8) {
                push_repack(regs, 0, t.dnout_offs[l], c.ssm_d_inner, dim)
            }
        }
        if (has_ple(c)) {  // gemma-4 E-series per-layer PLE gate/proj (the emb table stays linear, unpacked)
            let ple = c.n_embd_per_layer
            push_repack(regs, 0, t.ple_gate_off + l * dim * ple, dim, ple)
            push_repack(regs, 0, t.ple_proj_off + l * ple * dim, ple, dim)
        }
        if (c.n_expert > 0l && l >= c.n_layer_dense_lead) {   // MoE: each expert slice is its own 2D tensor, repacked per its shape
            let ege = dim * c.n_ff_exp
            if (!t.experts_mx4) {   // native-MXFP4 stacks live in mxq/mxs, row-major (no Q8 repack)
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
                push_repack(regs, 0, t.wsh1_off + l * she, dim, c.n_ff_shexp)
                push_repack(regs, 0, t.wsh2_off + l * she, c.n_ff_shexp, dim)
                push_repack(regs, 0, t.wsh3_off + l * she, dim, c.n_ff_shexp)
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
    if ((!c.shared_weights && t.wcls_fmt == KqFmt.q8) || t.cls_q8) {
        // the linear copy at emb_q8_off is deliberately NOT repacked — embedding-row dequant indexes it directly
        push_repack(regs, 0, t.wcls_off, dim, c.vocab_size)
    }
    if (c.n_layer_nextn > 0l && t.mtp_ehproj_fmt == KqFmt.q8) {   // NextN eh_proj: 2*dim -> dim
        push_repack(regs, 0, t.mtp_ehproj_off, 2l * dim, dim)
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
    if (fmt == KqFmt.k4) {
        push_repack(regs, 4, woff, n, d)
    } elif (fmt == KqFmt.k5) {
        push_repack(regs, 5, woff, n, d)
    } elif (fmt == KqFmt.k6) {
        push_repack(regs, 6, woff, n, d)
    } elif (fmt == KqFmt.q40) {
        push_repack(regs, 40, woff, n, d)
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
        // recurrent (deltanet) layers carry no attention projections — their kq planes are the dn triple
        if (layer_is_recurrent(c, l)) {
            let cd = dn_conv_dim(c)
            push_repack_kq(regs, fmt_at(t.dnqkv_fmt, l), t.dnqkv_offs[l], dim, cd)
            push_repack_kq(regs, fmt_at(t.dngate_fmt, l), t.dngate_offs[l], dim, c.ssm_d_inner)
            push_repack_kq(regs, fmt_at(t.dnout_fmt, l), t.dnout_offs[l], c.ssm_d_inner, dim)
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

// Repack the native-MXFP4 expert stacks into the active backend's interleaved mx layout, IN PLACE
// (each expert slice is its own 2D tensor). Mirrors repack_q8_weights' contract: called only after
// select_matmul_backend_for_load picks a repack backend; the pointer is otherwise identity.
def private repack_mx4_stacks(var t : Model) {
    let c = t.config
    let dim = c.dim
    let ege = dim * c.n_ff_exp
    var regs : array<StreamReg>
    for (l in range64(c.n_layers)) {
        for (e in range64(c.n_expert)) {
            let base = e * ege
            push_repack(regs, 1, t.we1_offs[l] + base, dim, c.n_ff_exp)
            push_repack(regs, 1, t.we2_offs[l] + base, c.n_ff_exp, dim)
            push_repack(regs, 1, t.we3_offs[l] + base, dim, c.n_ff_exp)
        }
    }
    repack_regions(t, regs)
    delete regs
}

// q51-native expert stacks -> the active backend's grp<mr> q51 layout (per-stack: a model can
// mix q51 downs with k4 gates, e.g. gemma-4-26B-A4B). No-op unless a stack is tagged q51.
def private collect_q51_regions(t : Model; var regs : array<StreamReg>) {
    let c = t.config
    let dim = c.dim
    let ege = dim * c.n_ff_exp
    for (l in range64(c.n_layers)) {
        let f1q = fmt_at(t.we1_fmt, l) == KqFmt.q51
        let f2q = fmt_at(t.we2_fmt, l) == KqFmt.q51
        let f3q = fmt_at(t.we3_fmt, l) == KqFmt.q51
        if (!(f1q || f2q || f3q)) {
            continue
        }
        for (e in range64(c.n_expert)) {
            let base = e * ege
            if (f1q) {
                push_repack(regs, 2, t.we1_offs[l] + base, dim, c.n_ff_exp)
            }
            if (f2q) {
                push_repack(regs, 2, t.we2_offs[l] + base, c.n_ff_exp, dim)
            }
            if (f3q) {
                push_repack(regs, 2, t.we3_offs[l] + base, dim, c.n_ff_exp)
            }
        }
    }
}

def private repack_q51_stacks(var t : Model) {
    var regs : array<StreamReg>
    collect_q51_regions(t, regs)
    if (!empty(regs)) {
        repack_regions(t, regs)
        to_log(LOG_INFO, "dasLLAMA: q51 expert planes repacked to grp{active_q51_layout_mr()}\n")
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
        } elif (fmt == 2) {
            invoke(active_repack_q51(), reinterpret<uint8?>(qp), reinterpret<uint8?>(sp), n, d)
        } else {
            invoke(active_repack_kq(), fmt, reinterpret<uint8?>(qp), reinterpret<uint8?>(sp), n, d)
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
    assert(wbias == 0l || wbias == 128l, "GPU MoE gather: unknown q8 plane bias")
    let nslices = int(rows / slice_rows)
    unsafe {
        let wp = addr<int8 const?>(t.qblob[woff])
        var s16p : uint16 const? = null
        var s32p : float const? = null
        if (t.wscale_f16) {
            s16p = addr<uint16 const?>(t.qscales16[woff / Q8_BLOCK_ELEMS])
        } else {
            s32p = addr<float const?>(t.qscales[woff / Q8_BLOCK_ELEMS])
        }
        var wqp = addr(wq[0])
        var wsp = addr<uint16?>(ws[0])
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

// Gather one PREPARED K-quant weight stack into the GPU tier's device planes — quant payloads
// row-major per superblock in the k/k+16 nibble pairing, plus DECODED 20B scale rows. Reads
// whatever the load prepared (grp<mr> interleave or disk-order); device bytes are identical either way.
def moe_gpu_gather_stack_kq(t : Model; fmt : KqFmt; woff : int64; n, rows, slice_rows : int64;
                            repacked : bool; mr : int64; var wq : array<uint8>; var ws : array<uint8>) {
    let nsb = n / KQ_SUPERBLOCK_ELEMS
    let qsb = kq_qsb(fmt)    // quant plane stride per superblock (disk == device for quants)
    let dssb = kq_ssb(fmt)   // disk/plane scale stride; device rows decode to KQ_DEV_SSB
    assert(n % KQ_SUPERBLOCK_ELEMS == 0l, "GPU MoE kq gather: row length must be a superblock multiple")
    assert(rows * nsb * qsb <= 2147483647l, "GPU MoE kq stack exceeds the int32 gather-buffer rail")
    wq |> grow_resize(rows * nsb * qsb)
    ws |> grow_resize(rows * nsb * KQ_DEV_SSB)
    let nslices = int(rows / slice_rows)
    let qrow = nsb * qsb
    let srow = nsb * dssb
    let ng = repacked ? slice_rows / mr : 0l
    unsafe {
        let sb0 = woff / KQ_SUPERBLOCK_ELEMS
        let qp = (fmt == KqFmt.k4 ? addr<uint8 const?>(t.k4q[sb0 * kq_qsb(4)])
            : (fmt == KqFmt.k5 ? addr<uint8 const?>(t.k5q[sb0 * kq_qsb(5)])
            : (fmt == KqFmt.k6 ? addr<uint8 const?>(t.k6q[sb0 * kq_qsb(6)]) : addr<uint8 const?>(t.q40q[sb0 * kq_qsb(40)]))))
        let sp = (fmt == KqFmt.k4 ? addr<uint8 const?>(t.k4s[sb0 * kq_ssb(4)])
            : (fmt == KqFmt.k5 ? addr<uint8 const?>(t.k5s[sb0 * kq_ssb(5)])
            : (fmt == KqFmt.k6 ? addr<uint8 const?>(t.k6s[sb0 * kq_ssb(6)]) : addr<uint8 const?>(t.q40s[sb0 * kq_ssb(40)]))))
        var wqp = addr(wq[0])
        var wsp = addr(ws[0])
        let njobs = is_job_que_available() ? min(nslices, 4 * (get_total_hw_jobs() + 1)) : 1
        maybe_parallel_for(0, nslices, njobs) $(ssb, sse) {
            unsafe {
                for (s in range(ssb, sse)) {
                    let sliceQ = int64(s) * slice_rows * qrow
                    let sliceS = int64(s) * slice_rows * srow
                    for (ri in range64(slice_rows)) {
                        let rr = int64(s) * slice_rows + ri
                        let grouped = repacked && ri < ng * mr
                        let g = ri / mr
                        let r = ri % mr
                        for (sbi in range64(nsb)) {
                            var dq = wqp + rr * qrow + sbi * qsb
                            var dsc = wsp + (rr * nsb + sbi) * KQ_DEV_SSB
                            if (grouped) {
                                let gq = qp + sliceQ + g * mr * qrow + sbi * qsb * mr
                                let gs = sp + sliceS + g * mr * srow + sbi * dssb * mr
                                for (m in range64(128l)) {   // nibble region: verbatim per (blk, j, t)
                                    dq[m] = gq[((m / 4l) * mr + r) * 4l + m % 4l]
                                }
                                if (fmt == KqFmt.k5) {
                                    for (bj in range64(32l)) {
                                        dq[128l + bj] = gq[128l * mr + bj * mr + r]
                                    }
                                } elif (fmt == KqFmt.k6) {
                                    for (p in range64(64l)) {
                                        dq[128l + p] = gq[128l * mr + ((p / 4l) * mr + r) * 4l + p % 4l]
                                    }
                                }
                                if (fmt == KqFmt.k6) {
                                    for (idx in range64(16l)) {
                                        dsc[idx] = gs[idx * mr + r]
                                    }
                                    dsc[16] = gs[16l * mr + 2l * r]
                                    dsc[17] = gs[16l * mr + 2l * r + 1l]
                                    dsc[18] = uint8(0)
                                    dsc[19] = uint8(0)
                                } elif (fmt == KqFmt.q40) {
                                    for (blk in range64(8l)) {   // 8 per-block f16 d, mr-interleaved
                                        dsc[blk * 2l] = gs[blk * 2l * mr + 2l * r]
                                        dsc[blk * 2l + 1l] = gs[blk * 2l * mr + 2l * r + 1l]
                                    }
                                    for (idx in range64(16l, 20l)) {
                                        dsc[idx] = uint8(0)
                                    }
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
                                if (fmt == KqFmt.q40) {
                                    for (m in range64(128l)) {   // Q4_0 bytes already pair k / k+16
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
                                if (fmt == KqFmt.k6 || fmt == KqFmt.q40) {
                                    for (idx in range64(dssb)) {
                                        dsc[idx] = rs[idx]
                                    }
                                    for (idx in range64(dssb, KQ_DEV_SSB)) {
                                        dsc[idx] = uint8(0)
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
}

// ===== hook registration (the loader in dasllama_common reaches back through these) =====

[init]
def private register_layout_hooks {
    register_model_layout(@@repack_q8_weights, @@repack_kq_weights, @@repack_mx4_stacks, @@repack_q51_stacks,
        @@moe_gpu_gather_stack, @@moe_gpu_gather_stack_kq)
    register_stream_layout(@@collect_q8_regions, @@collect_kq_regions, @@collect_q51_regions, @@stream_repack_one,
        @@metal_blob_eligible, @@metal_blob_scale_plane)
}
