options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_arch_qwen35 shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_common
require dasllama/dasllama_blocks   // the gated-attention path shares the std attention + FFN block helpers
require dasllama/dasllama_attn_prefill   // prefill_attention: the gated-attention prefill drives the shared head kernels
require dasllama/dasllama_math   // the moved DeltaNet kernels call silu/softplus/l2_norm_rows/rope_*/get_dispatch_lanes
require dasllama/dasllama_par    // maybe_parallel_for: the prefill kernels thread over rows
require math
require dasllama/dasllama_convert   // quantize_q8_0_into: the Q8_0 activation image the deltanet GEMVs consume
require dasllama/dasllama_kv_codec  // fwht_signs_row/fwht_unsign_row: the gated-attention Hadamard rotation

// Qwen 3.5/3.6 — hybrid: 3 of every 4 layers are Gated-DeltaNet (recurrent linear attention, no
// KV), the rest gated full attention (2x-wide Q with per-head sigmoid out-gate, partial NEOX rope
// 64/256, QK-norm). ChatML template like qwen2/qwen3; the pre-FFN norm ships as
// post_attention_norm (gpt-oss naming).
def private configure_qwen35(var c : Config) {
    c.rope_neox = true
    c.qk_norm = true
    c.q_gated = true
    c.partial_rope = true
    c.hybrid_deltanet = true
    c.post_attn_is_ffn_norm = true
}

// qwen35moe (Qwen3.6-35B-A3B, Qwen3.5-397B): hybrid attention + routed-experts FFN, softmax top-k
// with renorm + qwen2moe-style sigmoid-gated shared expert (loader-detected via ffn_up_shexp).
// qwen3next (Qwen3-Next-80B/Coder-Next) is the parent hybrid — same config, geometry via {arch}. prefix.
def private configure_qwen35moe(var c : Config) {
    configure_qwen35(c)
    c.moe = true
    c.norm_topk_prob = true
}

[init]
def register_arch_qwen35() {
    var d = ArchDesc(name = "qwen35", configure = @@configure_qwen35)
    d.blocks = qwen35_blocks()
    chatml_chat(d.chat)
    hermes_tools(d.chat)   // same Hermes tool block as qwen2/qwen3 (the Qwen3.5 template keeps it)
    // (clone the template BEFORE register_arch moves `d`)
    var dm = ArchDesc(name = "qwen35moe", configure = @@configure_qwen35moe)
    dm.blocks = qwen35moe_blocks()
    dm.chat := d.chat
    var dn = ArchDesc(name = "qwen3next", configure = @@configure_qwen35moe)
    dn.blocks = qwen35moe_blocks()
    dn.chat := d.chat
    register_arch("qwen35", d)
    register_arch("qwen35moe", dm)
    register_arch("qwen3next", dn)
}

// ===== qwen35 hybrid blocks: Gated DeltaNet (recurrent) + gated attention =====
// Alternates Gated-DeltaNet linear-attention layers (no KV, O(1) f32 state) with full-attention
// layers whose 2x-wide Q carries a sigmoid output gate. Oracle: llama.cpp qwen35.cpp + delta-net-base.cpp.

// Causal depthwise conv1d, one token step: per channel, dot the (taps = d_conv-1)-deep history
// plus the current input against that channel's taps (oldest first, ggml_ssm_conv order), then
// shift the history. Float accumulation matches ggml's sumf.
def private dn_conv_step(var outp : float?; var st : float?; inp : float const?; w : float const?; channels, taps : int64) {
    unsafe {
        for (ch in range64(channels)) {
            let sb = ch * taps
            let wb = ch * (taps + 1l)
            var acc = 0.0
            for (k in range64(taps)) {
                acc += st[sb + k] * w[wb + k]
            }
            let cur = inp[ch]
            acc += cur * w[wb + taps]
            for (k in range64(taps - 1l)) {
                st[sb + k] = st[sb + k + 1l]
            }
            st[sb + taps - 1l] = cur
            outp[ch] = acc
        }
    }
}

// One delta-rule step for one v-head (llama.cpp build_delta_net_autoregressive): S decays by
// exp(g); sk = kᵀS; d = (v - sk)·β; S += k⊗d; o = qᵀS (q pre-scaled by 1/sqrt(d_state)).
// Row-by-row accumulation over the k index reproduces ggml's sum order.
def private dn_delta_head(var st : float?; q, k, v : float const?; var o : float?; var skd : float?; g, beta : float; ds : int64) {
    unsafe {
        let decay = exp(g)
        for (j in range64(ds)) {
            skd[j] = 0.0
        }
        for (i in range64(ds)) {
            var row = st + i * ds
            let ki = k[i]
            for (j in range64(ds)) {
                let sv = row[j] * decay
                row[j] = sv
                skd[j] += ki * sv
            }
        }
        for (j in range64(ds)) {
            skd[j] = (v[j] - skd[j]) * beta
            o[j] = 0.0
        }
        for (i in range64(ds)) {
            var row = st + i * ds
            let ki = k[i]
            let qi = q[i]
            for (j in range64(ds)) {
                let sv = row[j] + ki * skd[j]
                row[j] = sv
                o[j] += qi * sv
            }
        }
    }
}

// The per-token deltanet core, shared by decode and (per position) prefill: β/g transforms, causal
// conv + SiLU, q/k L2-norm + q pre-scale, the per-v-head delta rule, and z-gated out-norm.
// `qkv` is this token's fused [q;k;v] row; z/beta/g its other projections; output lands in `o`.
def private dn_token_core(t : Model; var s : Session; l : int64; qkv : float const?; var z : float?; var beta : float?; var g : float?; var o : float?) {
    let c = t.config
    let cd = dn_conv_dim(c)
    let kd = dn_key_dim(c)
    let nvh = c.ssm_dt_rank
    let nkh = c.ssm_n_group
    let ds = c.ssm_d_state
    let taps = c.ssm_d_conv - 1l
    unsafe {
        // β = σ(raw); g = ssm_a ⊙ softplus(raw + dt_bias) — ssm_a stores −exp(A_log) precomputed
        for (h in range64(nvh)) {
            beta[h] = 1.0 / (1.0 + exp(-beta[h]))
            g[h] = t.fblob[t.dn_a_off + l * nvh + h] * softplus(g[h] + t.fblob[t.dn_dt_off + l * nvh + h])
        }
        // causal conv over the fused [q;k;v] row (persistent per-layer history), then SiLU
        var cop = addr(s.dn_conv_out[0])
        dn_conv_step(cop, addr(s.dn_conv_state[s.dn_conv_prefix[l]]), qkv,
            addr < float const? >(t.fblob[t.dn_conv_off + l * c.ssm_d_conv * cd]), cd, taps)
        silu(cop, cd)
        // L2-normalize the q and k heads (v passes through), then pre-scale q by 1/sqrt(ds)
        l2_norm_rows(cop, nkh, ds, c.norm_eps)
        l2_norm_rows(cop + kd, nkh, ds, c.norm_eps)
        scale_inplace(cop, 1.0 / sqrt(float(ds)), kd)
        // per-v-head delta rule; k-heads repeat cyclically onto v-heads (h % nkh); heads are independent, so lanes over them
        let qp = reinterpret<float const?>(cop)
        let kp = reinterpret<float const?>(cop + kd)
        let vp = reinterpret<float const?>(cop + 2l * kd)
        var stp = addr(s.dn_state[s.dn_state_prefix[l]])
        var skp = addr(s.dn_sk[0])
        let dn_lanes = min(int(nvh), lanes_for_work(2l * nvh * ds * ds, g_gemv_lane_cap))
        maybe_parallel_for(0, int(nvh), dn_lanes) $(rb, re) {
            unsafe {
                for (hh in range(rb, re)) {
                    let h = int64(hh)
                    let kh = h % nkh
                    dn_delta_head(stp + h * ds * ds, qp + kh * ds, kp + kh * ds, vp + h * ds,
                        o + h * ds, skp + h * ds, g[h], beta[h], ds)
                }
            }
        }
        // gated out-norm: per head RMSNorm(o; ssm_norm, shared weights) ⊙ SiLU(z)
        let wnorm = addr < float const? >(t.fblob[t.dn_norm_off + l * ds])
        for (h in range64(nvh)) {
            rmsnorm(o + h * ds, reinterpret<float const?>(o + h * ds), wnorm, ds, c.norm_eps)
        }
        silu(z, c.ssm_d_inner)
        mul_inplace(o, reinterpret<float const?>(z), c.ssm_d_inner)
    }
}

// The deltanet decode step as ONE device chain (npos=1 through MoeGpuDnFn): CPU keeps norm +
// raw β/α + the q8 image. State/conv history live DEVICE-SIDE across steps — dn_flush_layer /
// moe_gpu_dn_invalidate are the sync seams for every CPU consumer.
def private deltanet_decode_gpu(t : Model; var s : Session; l : int64) {
    let c = t.config
    let dim = c.dim
    let cd = dn_conv_dim(c)
    let kd = dn_key_dim(c)
    let di = c.ssm_d_inner
    let nvh = c.ssm_dt_rank
    let nkh = c.ssm_n_group
    let ds = c.ssm_d_state
    let dconv = c.ssm_d_conv
    let taps = dconv - 1l
    let ts_norm0 = prof_ticks()
    rms_at(s.xb, t, t.rms_att_off + l * dim, s.x, dim)
    prof_add("norm", ts_norm0)
    let ts_qkv = prof_ticks()
    if (t.dn_ba_f32) {   // F32-on-disk beta/alpha matmul fp32, like llama.cpp
        mm_fblob(s.dn_beta, t, t.dn_betaf_off + l * dim * nvh, s.xb, dim, nvh)
        mm_fblob(s.dn_g, t, t.dn_alphaf_off + l * dim * nvh, s.xb, dim, nvh)
    } else {
        mm_f(s.dn_beta, t, KqFmt.q8, t.dnbeta_offs[l], s.xb, s.xq, s.xs, s.xbs, dim, nvh)
        mm_f(s.dn_g, t, KqFmt.q8, t.dnalpha_offs[l], s.xb, s.xq, s.xs, s.xbs, dim, nvh)
    }
    quantize_q8_0_into(s.xb, dim, s.xq, s.xs, 0l, 0l)   // one Q8_0 image serves the qkv AND z GEMVs
    prof_add("mm_qkv", ts_qkv)
    let ts_gpu = prof_ticks()
    s.dn_tail |> resize(taps * cd)   // contract slot only — the step arm never writes it
    unsafe {
        matmul_moe_gpu_dn(s.xb2, t.dnqkv_offs[l], t.dngate_offs[l], t.dnout_offs[l],
            s.xq, s.xs, s.dn_beta, s.dn_g,
            addr < float const? >(t.fblob[t.dn_conv_off + l * dconv * cd]),
            addr < float const? >(t.fblob[t.dn_norm_off + l * ds]),
            addr < float const? >(t.fblob[t.dn_a_off + l * nvh]),
            addr < float const? >(t.fblob[t.dn_dt_off + l * nvh]),
            addr(s.dn_state[s.dn_state_prefix[l]]), s.dn_tail,
            addr < float const? >(s.dn_conv_state[s.dn_conv_prefix[l]]),
            1l, dim, cd, kd, di, nvh, nkh, ds, dconv, c.norm_eps)
    }
    prof_add("dn_gpu", ts_gpu)
    let ts_resid = prof_ticks()
    add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb2[0])), dim)
    prof_add("act_resid", ts_resid)
}

// attn-norm → fused qkv/z/β/α projections → per-token deltanet core → ssm_out projection → residual.
def private deltanet_decode(t : Model; var s : Session; l : int64) {
    if (moe_gpu_dnd_on_gpu(l)) {
        deltanet_decode_gpu(t, s, l)
        return
    }
    dn_flush_layer(s, l)   // device state may be dirty from earlier GPU steps (route flip)
    let c = t.config
    let dim = c.dim
    let cd = dn_conv_dim(c)
    let di = c.ssm_d_inner
    let nvh = c.ssm_dt_rank
    let ts_norm0 = prof_ticks()
    rms_at(s.xb, t, t.rms_att_off + l * dim, s.x, dim)
    prof_add("norm", ts_norm0)
    let ts_qkv = prof_ticks()
    mm_f(s.dn_qkv, t, fmt_at(t.dnqkv_fmt, l), t.dnqkv_offs[l], s.xb, s.xq, s.xs, s.xbs, dim, cd)
    mm_f(s.dn_z, t, fmt_at(t.dngate_fmt, l), t.dngate_offs[l], s.xb, s.xq, s.xs, s.xbs, dim, di)
    if (t.dn_ba_f32) {   // F32-on-disk beta/alpha matmul fp32, like llama.cpp
        mm_fblob(s.dn_beta, t, t.dn_betaf_off + l * dim * nvh, s.xb, dim, nvh)
        mm_fblob(s.dn_g, t, t.dn_alphaf_off + l * dim * nvh, s.xb, dim, nvh)
    } else {
        mm_f(s.dn_beta, t, KqFmt.q8, t.dnbeta_offs[l], s.xb, s.xq, s.xs, s.xbs, dim, nvh)
        mm_f(s.dn_g, t, KqFmt.q8, t.dnalpha_offs[l], s.xb, s.xq, s.xs, s.xbs, dim, nvh)
    }
    prof_add("mm_qkv", ts_qkv)
    let ts_attn = prof_ticks()
    unsafe {
        dn_token_core(t, s, l, addr < float const? >(s.dn_qkv[0]), addr(s.dn_z[0]),
            addr(s.dn_beta[0]), addr(s.dn_g[0]), addr(s.dn_o[0]))
    }
    prof_add("attn", ts_attn)
    let ts_wo = prof_ticks()
    mm_f(s.xb2, t, fmt_at(t.dnout_fmt, l), t.dnout_offs[l], s.dn_o, s.xq, s.xs, s.xbs, di, dim)
    prof_add("mm_wo", ts_wo)
    let ts_resid = prof_ticks()
    add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb2[0])), dim)
    prof_add("act_resid", ts_resid)
}

// attn-norm → 2x-wide [q|gate] + k/v → QK-norm → partial rope → KV-store → GQA attention →
// out ⊙ σ(gate) → out-proj → residual. The gated-attention twin of attention_std_decode.
def private attention_qwen35_gated_decode(t : Model; var s : Session; l : int64; pos : int64) { // nolint:STYLE038 — one decode block: qkv GEMV, QK-norm, Hadamard, rope, attention, out-gate; the phases share live registers/scratch
    trace_tag(TRACE_TAG_ATTN_STD)
    let c = t.config
    let dim = c.dim
    let kv_dim = layer_kv_dim(c, l)
    let head_size = layer_head_size(c, l)
    let n_heads = c.n_heads
    let n_kv_heads = layer_n_kv_heads(c, l)
    let qd = n_heads * head_size
    let kv_mul = n_heads / n_kv_heads
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    let rot = c.rope_dim > 0l ? c.rope_dim : head_size
    var kcl = kv_layer_kbase(s, l, c.seq_len)
    var vcl = kv_layer_vbase(s, l, c.seq_len)
    let kstart = 0l
    let cnt = pos + 1l
    let ts_norm0 = prof_ticks()
    rms_at(s.xb, t, t.rms_att_off + l * dim, s.x, dim)
    prof_add("norm", ts_norm0)
    let ts_qkv = prof_ticks()
    mm_f(s.dn_qg, t, fmt_at(t.wq_fmt, l), t.wq_offs[l], s.xb, s.xq, s.xs, s.xbs, dim, 2l * qd)
    mm_f(s.k, t, fmt_at(t.wk_fmt, l), t.wk_offs[l], s.xb, s.xq, s.xs, s.xbs, dim, kv_dim)
    mm_f(s.v, t, fmt_at(t.wv_fmt, l), t.wv_offs[l], s.xb, s.xq, s.xs, s.xbs, dim, kv_dim)
    // deinterleave: head h's q lives at 2h·hs, its gate at (2h+1)·hs
    unsafe {
        let qgp = addr < float const? >(s.dn_qg[0])
        var qp = addr(s.q[0])
        var gp = addr(s.dn_qgate[0])
        for (h in range64(n_heads)) {
            let sb = h * 2l * head_size
            let db = h * head_size
            for (i in range64(head_size)) {
                qp[db + i] = qgp[sb + i]
                gp[db + i] = qgp[sb + head_size + i]
            }
        }
    }
    prof_add("mm_qkv", ts_qkv)
    let ts_qknorm = prof_ticks()
    rms_batch(s.q, s.q, t, t.rms_q_offs[l], head_size, n_heads)
    rms_batch(s.k, s.k, t, t.rms_k_offs[l], head_size, n_kv_heads)
    prof_add("norm", ts_qknorm)
    let ts_rope = prof_ticks()
    if (g_use_rope_table) {
        unsafe {
            let crow = addr < float const? >(s.rope_cos[0])
            let srow = addr < float const? >(s.rope_sin[0])
            rope_scaled_neox_tab_part(addr(s.q[0]), head_size, rot, qd, crow, srow)
            rope_scaled_neox_tab_part(addr(s.k[0]), head_size, rot, kv_dim, crow, srow)
        }
    } else {
        unsafe {
            rope_scaled_neox_part(addr(s.q[0]), pos, head_size, rot, qd, c.rope_freq_base, c.rope_freq_scale, false, null, c.rope_mscale)
            rope_scaled_neox_part(addr(s.k[0]), pos, head_size, rot, kv_dim, c.rope_freq_base, c.rope_freq_scale, false, null, c.rope_mscale)
        }
    }
    prof_add("rope", ts_rope)
    let ktq4 = s.kv_dtype_k == KVDtype.tq4
    let vtq4 = s.kv_dtype_v == KVDtype.tq4
    if (ktq4 || vtq4) {
        // tq4 basis change at the RoPE seam: attention runs rotated; xb un-rotates per head after
        let ts_rot = prof_ticks()
        unsafe {
            let sgp = addr < float const? >(s.kv_signs[0])
            if (ktq4) {
                var qrp = addr(s.q[0])
                for (hh in range64(qd / head_size)) {
                    fwht_signs_row(qrp + hh * head_size, sgp, head_size)
                }
                var krp = addr(s.k[0])
                for (hh in range64(kv_dim / head_size)) {
                    fwht_signs_row(krp + hh * head_size, sgp, head_size)
                }
            }
            if (vtq4) {
                var vrp = addr(s.v[0])
                for (hh in range64(kv_dim / head_size)) {
                    fwht_signs_row(vrp + hh * head_size, sgp, head_size)
                }
            }
        }
        prof_add("tq4_rot", ts_rot)
    }
    if (s.kv_dtype_k == KVDtype.q8_0 || ktq4) {
        quantize_q8_0_into(s.q, qd, s.attq, s.atts, 0l, 0l)
    }
    let ts_kv = prof_ticks()
    unsafe {
        kv_store_row(kcl, s.kv_dtype_k, kv_dim, kv_phys_row(s, pos), 0l, kv_dim,
            addr < float const? >(s.k[0]))
        kv_store_row(vcl, s.kv_dtype_v, kv_dim, kv_phys_row(s, pos), 0l, kv_dim,
            addr < float const? >(s.v[0]))
    }
    prof_add("kv_store", ts_kv)
    let ts_attn = prof_ticks()
    let seq_len = c.seq_len
    let attn_softcap = c.attn_logit_softcap
    let attn_lanes = min(int(n_heads), lanes_for_work(2l * cnt * head_size * n_heads, g_gemv_lane_cap))
    let kdt = s.kv_dtype_k
    let vdt = s.kv_dtype_v
    let nrun = kv_runs(s, kstart, pos + 1l, s.kv_runs)
    unsafe {
        let kplanes = kdt == KVDtype.q8_0 || kdt == KVDtype.tq4
        let qp = addr < float const? >(s.q[0])
        let qqp = kplanes ? addr < int8 const? >(s.attq[0]) : null
        let qsp = kplanes ? addr < float const? >(s.atts[0]) : null
        let runsp = addr < KVRun const? >(s.kv_runs[0])
        var xbp = addr(s.xb[0])
        var attp = addr(s.att[0])
        maybe_parallel_for(0, int(n_heads), attn_lanes) $(rb, re) {
            unsafe {
                for (h in range(rb, re)) {
                    let hh = int64(h)
                    let qoff = hh * head_size
                    let kvhoff = (hh / kv_mul) * head_size
                    attn_head_decode_d(attp + hh * seq_len, xbp, qp, qqp, qsp, kcl, vcl, kdt, vdt, runsp,
                        nrun, qoff, kvhoff, kstart, cnt, kv_dim, head_size, hh, scale, attn_softcap, null)
                }
            }
        }
    }
    prof_add("attn", ts_attn)
    if (vtq4) {   // tq4 V: xb accumulated in the rotated basis — un-rotate before the out-gate
        unsafe {
            let sgp = addr < float const? >(s.kv_signs[0])
            var xrp = addr(s.xb[0])
            for (hh in range64(n_heads)) {
                fwht_unsign_row(xrp + hh * head_size, sgp, head_size)
            }
        }
    }
    // per-head sigmoid output gate on the attention output, before the out projection (exp4)
    let ts_wo = prof_ticks()
    unsafe {
        var xbp2 = addr(s.xb[0])
        let gp2 = addr < float const? >(s.dn_qgate[0])
        if (g_act_vec) {
            sigmoid_gate4(xbp2, gp2, qd)
        } else {
            sigmoid_gate(xbp2, gp2, qd)
        }
    }
    mm_f(s.xb2, t, fmt_at(t.wo_fmt, l), t.wo_offs[l], s.xb, s.xq, s.xs, s.xbs, qd, dim)
    prof_add("mm_wo", ts_wo)
    let ts_resid = prof_ticks()
    add_inplace(unsafe(addr(s.x[0])), unsafe(addr(s.xb2[0])), dim)
    prof_add("act_resid", ts_resid)
}

// Zero the recurrent state for a fresh conversation (position 0).

// The qwen35 decode attention block: forward-only state guard, then deltanet or gated attention
// per the layer mask.
[no_alloc, no_env, no_io]
def private attention_qwen35_decode(t : Model; var s : Session; l : int64; pos : int64) {
    if (l == 0l) {   // recurrent state is forward-only: full reset at 0, no rewind
        if (pos == 0l) {
            dn_reset(s)
        } elif (pos != s.dn_pos) {
            panic("dasLLAMA: deltanet state is forward-only - cannot eval position {pos} (session is at {s.dn_pos}); rewinding needs a fresh session")
        }
        s.dn_pos = pos + 1l
    }
    if (layer_is_recurrent(t.config, l)) {
        trace_tag(TRACE_TAG_ATTN_STD)
        deltanet_decode(t, s, l)
    } else {
        attention_qwen35_gated_decode(t, s, l, pos)
    }
}

// ffn-norm -> gated FFN (act(W1·x) * (W3·x)) -> down-proj(+post-norm) -> residual.
// Dense gated FFN kernel, in place: reads pre-normed activation in s.xb, writes act(W1·xb)·(W3·xb)
// projected through W2 back into s.xb. Shared by ffn_dense_decode and gemma4's dense shared expert.

// ===== qwen35 hybrid blocks: Gated DeltaNet (chunked) + gated attention — PREFILL =====
def private dn_conv_batch(t : Model; var s : Session; l : int64; npos : int64) {
    let c = t.config
    let cd = dn_conv_dim(c)
    let kd = dn_key_dim(c)
    let nkh = c.ssm_n_group
    let ds = c.ssm_d_state
    let dconv = c.ssm_d_conv
    let taps = dconv - 1l
    let qscale = 1.0 / sqrt(float(ds))
    unsafe {
        let wsrc = addr < float const? >(t.fblob[t.dn_conv_off + l * dconv * cd])
        var wtp = addr(s.dn_wtap[0])
        for (ch in range64(cd)) {
            for (k in range64(dconv)) {
                wtp[k * cd + ch] = wsrc[ch * dconv + k]
            }
        }
    }
    var inp0 : float const?
    var wtap0 : float const?
    var hist0 : float const?
    var outp0 : float?
    unsafe {
        inp0 = addr < float const? >(s.dn_qkv_b[0])
        wtap0 = addr < float const? >(s.dn_wtap[0])
        hist0 = addr < float const? >(s.dn_conv_state[s.dn_conv_prefix[l]])
        outp0 = addr(s.dn_conv_b[0])
    }
    maybe_parallel_for(npos * cd >= g_act_par_threshold, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
        unsafe {
            for (pp in range(rb, re)) {
                let p = int64(pp)
                var orow = outp0 + p * cd
                if (p >= taps) {   // all taps in-batch: dconv row fmas over float4 channel blocks
                    let irow = inp0 + (p - taps) * cd
                    var ch = 0l
                    while (ch < cd) {
                        var acc = float4(0.0, 0.0, 0.0, 0.0)
                        for (k in range64(dconv)) {
                            acc += *addr < float4 const? >(irow[k * cd + ch]) * *addr < float4 const? >(wtap0[k * cd + ch])
                        }
                        *addr < float4? >(orow[ch]) = acc
                        ch += 4l
                    }
                } else {           // window straddles the batch start: missing taps from history
                    for (ch in range64(cd)) {
                        var acc = 0.0
                        for (k in range64(dconv)) {
                            let sp = p - taps + k
                            acc += (sp >= 0l ? inp0[sp * cd + ch] : hist0[ch * taps + p + k]) * wtap0[k * cd + ch]
                        }
                        orow[ch] = acc
                    }
                }
                silu(orow, cd)
                l2_norm_rows(orow, nkh, ds, c.norm_eps)
                l2_norm_rows(orow + kd, nkh, ds, c.norm_eps)
                scale_inplace(orow, qscale, kd)
            }
        }
    }
    unsafe {   // advance the conv history to the last taps input rows (channel-major)
        var st = addr(s.dn_conv_state[s.dn_conv_prefix[l]])
        for (k in range64(taps)) {
            let sp = npos - taps + k
            if (sp >= 0l) {
                for (ch in range64(cd)) {
                    st[ch * taps + k] = inp0[sp * cd + ch]
                }
            } else {   // npos < taps: shift surviving old taps left (ascending k reads ahead of writes)
                for (ch in range64(cd)) {
                    st[ch * taps + k] = st[ch * taps + k + npos]
                }
            }
        }
    }
}

// One v-head's chunked delta rule over the whole prefill window (mirrors llama.cpp
// build_delta_net_chunking; NOT bit-identical to the recurrent form — chunk FP reassociation).
// Solves (I + A) d = V_beta - (K_beta ⊙ e^{gcum}) S per chunk; state S advances once per chunk.
def private dn_chunked_head(conv : float const?; qoff, koff, voff, cd : int64; // nolint:STYLE037,STYLE038 — the chunked DeltaNet head is one barrier-coupled recurrence; splitting it would spill the chunk state
                            bp, gp : float const?; bg_stride : int64;
                            zp : float const?; var op : float?; o_stride : int64;
                            var stp : float?; var ws : float?; wnorm : float const?;
                            aa, dtb, eps : float; vec : bool; npos, ds : int64) {
    let CS = DN_CHUNK
    unsafe {
        var qc = ws                         // CS x ds: q rows, scaled to q ⊙ e^{gcum} after the KQ GEMM
        var kcT = ws + CS * ds              // ds x cl: raw k, transposed (the B side of A/KQ GEMMs)
        var kb = ws + 2l * CS * ds          // CS x ds: k ⊙ beta, scaled to -(k_beta ⊙ e^{gcum}) after A
        var vb = ws + 3l * CS * ds          // CS x ds: v ⊙ beta
        var w = ws + 4l * CS * ds           // CS x ds: -T @ (k_beta ⊙ e^{gcum})
        var vnew = ws + 5l * CS * ds        // CS x ds: the in-chunk delta vectors d
        var och = ws + 6l * CS * ds         // CS x ds: this chunk's outputs
        var kgT = ws + 7l * CS * ds         // ds x cl: (k ⊙ e^{g_last - gcum})^T for the state update
        var tm = ws + 8l * CS * ds          // cl x cl: A, solved in place into T (unit diag)
        var kq = ws + 8l * CS * ds + CS * CS   // cl x cl: Q K^T ⊙ decay (diagonal kept)
        var gcum = ws + 8l * CS * ds + 2l * CS * CS   // CS: in-chunk cumsum of log-decay
        var egc = gcum + CS                 // CS: e^{gcum}
        var kgc = gcum + 2l * CS            // CS: e^{g_last - gcum}
        var c0 = 0l
        while (c0 < npos) {
            let cl = min(CS, npos - c0)
            // gather + per-token transforms (beta/g raw -> beta = sigmoid, g = a·softplus(raw+dt))
            var gacc = 0.0
            for (i in range64(cl)) {
                let crow = conv + (c0 + i) * cd
                let bv = 1.0 / (1.0 + exp(-bp[(c0 + i) * bg_stride]))
                gacc += aa * softplus(gp[(c0 + i) * bg_stride] + dtb)
                gcum[i] = gacc
                egc[i] = exp(gacc)
                for (j in range64(ds)) {
                    let kv = crow[koff + j]
                    qc[i * ds + j] = crow[qoff + j]
                    kcT[j * cl + i] = kv
                    kb[i * ds + j] = kv * bv
                    vb[i * ds + j] = crow[voff + j] * bv
                }
            }
            // A = (k_beta k^T) ⊙ decay, strictly lower, negated for the in-place solve; KQ keeps its diagonal
            for (i in range64(cl * cl)) {
                tm[i] = 0.0
                kq[i] = 0.0
            }
            gemm_f32(tm, kb, kcT, cl, ds, cl)
            gemm_f32(kq, qc, kcT, cl, ds, cl)
            for (i in range64(cl)) {
                let gi = gcum[i]
                var j = 0l
                if (vec) {   // exp4 over the strictly-lower row (args <= 0; clamp = flash-fold convention)
                    let gi4 = float4(gi, gi, gi, gi)
                    while (j + 4l <= i) {
                        let d = exp4(max(gi4 - float4(gcum[j], gcum[j + 1l], gcum[j + 2l], gcum[j + 3l]), EXP4_LO))
                        tm[i * cl + j] = -tm[i * cl + j] * d.x
                        tm[i * cl + j + 1l] = -tm[i * cl + j + 1l] * d.y
                        tm[i * cl + j + 2l] = -tm[i * cl + j + 2l] * d.z
                        tm[i * cl + j + 3l] = -tm[i * cl + j + 3l] * d.w
                        kq[i * cl + j] *= d.x
                        kq[i * cl + j + 1l] *= d.y
                        kq[i * cl + j + 2l] *= d.z
                        kq[i * cl + j + 3l] *= d.w
                        j += 4l
                    }
                }
                while (j < i) {
                    let d = exp(gi - gcum[j])
                    tm[i * cl + j] = -tm[i * cl + j] * d
                    kq[i * cl + j] *= d
                    j++
                }
                tm[i * cl + i] = 0.0
                for (ju in range64(i + 1l, cl)) {   // upper: tm zeroed, kq zeroed (diagonal kept)
                    tm[i * cl + ju] = 0.0
                    kq[i * cl + ju] = 0.0
                }
            }
            // forward substitution in place: T[i][j] = -A[i][j] - sum_t A[i][t] T[t][j]
            for (i in range64(cl)) {
                for (tt in range64(i)) {
                    let coef = tm[i * cl + tt]
                    for (j in range64(tt)) {
                        tm[i * cl + j] += coef * tm[tt * cl + j]
                    }
                }
                tm[i * cl + i] = 1.0
            }
            // d = T v_beta - (T (k_beta ⊙ e^{gcum})) S — kb scaled in place, minus folded into w
            for (i in range64(cl)) {
                scale_inplace(kb + i * ds, -egc[i], ds)
            }
            for (i in range64(cl * ds)) {
                w[i] = 0.0
                vnew[i] = 0.0
            }
            gemm_f32(w, tm, kb, cl, cl, ds)
            gemm_f32(vnew, tm, vb, cl, cl, ds)
            gemm_f32(vnew, w, stp, cl, ds, ds)
            // o = (q ⊙ e^{gcum}) S + KQ d (qc scaled in place after its raw-q GEMM above)
            for (i in range64(cl)) {
                scale_inplace(qc + i * ds, egc[i], ds)
            }
            for (i in range64(cl * ds)) {
                och[i] = 0.0
            }
            gemm_f32(och, qc, stp, cl, ds, ds)
            gemm_f32(och, kq, vnew, cl, cl, ds)
            // gated out-norm into batched output rows; z-gate exp4 (largest exp population of the whole prefill)
            for (i in range64(cl)) {
                rmsnorm(och + i * ds, reinterpret<float const?>(och + i * ds), wnorm, ds, eps)
                var orow = op + (c0 + i) * o_stride
                let zrow = zp + (c0 + i) * o_stride
                if (vec) {
                    silu_mul4(orow, och + i * ds, zrow, ds)
                } else {
                    for (j in range64(ds)) {
                        let zv = zrow[j]
                        orow[j] = och[i * ds + j] * (zv / (1.0 + exp(-zv)))
                    }
                }
            }
            // state advance, once per chunk: S = e^{g_last} S + (k ⊙ e^{g_last - gcum})^T d
            let glast = gcum[cl - 1l]
            scale_inplace(stp, exp(glast), ds * ds)
            for (i in range64(cl)) {
                kgc[i] = exp(glast - gcum[i])
            }
            for (j in range64(ds)) {
                var kr = kgT + j * cl
                let ks = kcT + j * cl
                for (i in range64(cl)) {
                    kr[i] = ks[i] * kgc[i]
                }
            }
            gemm_f32(stp, kgT, vnew, ds, cl, ds)
            c0 += cl
        }
    }
}

// The chunked deltanet prefill core: batched conv rows, then the per-v-head chunk pass threaded
// over heads (each head's state chain is independent; per-head slabs in dn_cws).
def private deltanet_prefill_chunked(t : Model; var s : Session; l : int64; npos : int64) {
    let c = t.config
    let cd = dn_conv_dim(c)
    let kd = dn_key_dim(c)
    let di = c.ssm_d_inner
    let nvh = c.ssm_dt_rank
    let nkh = c.ssm_n_group
    let ds = c.ssm_d_state
    let ts_conv = prof_ticks()
    dn_conv_batch(t, s, l, npos)
    prof_add("dn_conv", ts_conv)
    var conv0 : float const?
    var bp0 : float const?
    var gp0 : float const?
    var zp0 : float const?
    var op0 : float?
    var st0 : float?
    var ws0 : float?
    var wnorm0 : float const?
    var ab0 : float const?
    var dtb0 : float const?
    unsafe {
        conv0 = addr < float const? >(s.dn_conv_b[0])
        bp0 = addr < float const? >(s.dn_beta_b[0])
        gp0 = addr < float const? >(s.dn_g_b[0])
        zp0 = addr < float const? >(s.dn_z_b[0])
        op0 = addr(s.dn_o_b[0])
        st0 = addr(s.dn_state[s.dn_state_prefix[l]])
        ws0 = addr(s.dn_cws[0])
        wnorm0 = addr < float const? >(t.fblob[t.dn_norm_off + l * ds])
        ab0 = addr < float const? >(t.fblob[t.dn_a_off + l * nvh])
        dtb0 = addr < float const? >(t.fblob[t.dn_dt_off + l * nvh])
    }
    let wssz = dn_cws_head_size(c)
    let vec = g_act_vec   // read on the caller, captured — fork workers must not read globals
    let ts_scan = prof_ticks()
    maybe_parallel_for(0, int(nvh), min(int(nvh), get_dispatch_lanes())) $(rb, re) {
        unsafe {
            for (hh in range(rb, re)) {
                let h = int64(hh)
                let kh = h % nkh   // k-heads repeat cyclically onto v-heads (ggml_repeat: h % nkh)
                dn_chunked_head(conv0, kh * ds, kd + kh * ds, 2l * kd + h * ds, cd,
                    bp0 + h, gp0 + h, nvh, zp0 + h * ds, op0 + h * ds, di,
                    st0 + h * ds * ds, ws0 + h * wssz, wnorm0,
                    ab0[h], dtb0[h], c.norm_eps, vec, npos, ds)
            }
        }
    }
    prof_add("dn_scan", ts_scan)
}

// The GPU arm: the whole block (qkv/z GEMMs, conv, chunked scan, gated out-norm, out-proj) as one
// device chain per window; norm, raw beta/alpha GEMMs, residual, conv-history store stay CPU.
// Numerics drift-class vs CPU (GPU exp/f32 accumulation) — the DN parity bar (qwen35 knife-edge).
def private deltanet_prefill_gpu(t : Model; var s : Session; l : int64; npos : int64) {
    let c = t.config
    let dim = c.dim
    let cd = dn_conv_dim(c)
    let kd = dn_key_dim(c)
    let di = c.ssm_d_inner
    let nvh = c.ssm_dt_rank
    let nkh = c.ssm_n_group
    let ds = c.ssm_d_state
    let dconv = c.ssm_d_conv
    let taps = dconv - 1l
    let ts_norm0 = prof_ticks()
    rms_batch(s.xb_b, s.x_b, t, t.rms_att_off + l * dim, dim, npos)
    prof_add("dn_norm", ts_norm0)
    let ts_in = prof_ticks()
    if (t.dn_ba_f32) {   // raw beta/alpha rows — the transforms run in-kernel
        mm_fblob_b(s.dn_beta_b, t, t.dn_betaf_off + l * dim * nvh, s.xb_b, dim, nvh, npos)
        mm_fblob_b(s.dn_g_b, t, t.dn_alphaf_off + l * dim * nvh, s.xb_b, dim, nvh, npos)
    } else {
        mm_b_f(s, s.dn_beta_b, t, KqFmt.q8, t.dnbeta_offs[l], s.xb_b, dim, nvh, npos)
        mm_b_f(s, s.dn_g_b, t, KqFmt.q8, t.dnalpha_offs[l], s.xb_b, dim, nvh, npos)
    }
    requant_rows_q8(s.xb_b, dim, npos, s.xqb, s.xsb)   // one Q8_0 image serves the qkv AND z GEMMs
    prof_add("dn_mm_in", ts_in)
    let ts_gpu = prof_ticks()
    s.dn_tail |> resize(taps * cd)
    unsafe {
        matmul_moe_gpu_dn(s.xb2_b, t.dnqkv_offs[l], t.dngate_offs[l], t.dnout_offs[l],
            s.xqb, s.xsb, s.dn_beta_b, s.dn_g_b,
            addr<float const?>(t.fblob[t.dn_conv_off + l * dconv * cd]),
            addr<float const?>(t.fblob[t.dn_norm_off + l * ds]),
            addr<float const?>(t.fblob[t.dn_a_off + l * nvh]),
            addr<float const?>(t.fblob[t.dn_dt_off + l * nvh]),
            addr(s.dn_state[s.dn_state_prefix[l]]), s.dn_tail,
            addr<float const?>(s.dn_conv_state[s.dn_conv_prefix[l]]),
            npos, dim, cd, kd, di, nvh, nkh, ds, dconv, c.norm_eps)
    }
    prof_add("dn_gpu", ts_gpu)
    let ts_resid = prof_ticks()
    unsafe {   // conv-history advance from the tail rows (channel-major — the decode layout)
        var st = addr(s.dn_conv_state[s.dn_conv_prefix[l]])
        let tp = addr < float const? >(s.dn_tail[0])
        for (k in range64(taps)) {
            for (ch in range64(cd)) {
                st[ch * taps + k] = tp[k * cd + ch]
            }
        }
    }
    add_inplace(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb2_b[0])), npos * dim)
    prof_add("dn_resid", ts_resid)
}

// qwen35 deltanet prefill: batched projections, then the inherently sequential per-token core
// (state carried across positions), then one batched ssm_out projection + residual.
def private deltanet_prefill(t : Model; var s : Session; l : int64; npos : int64) {
    trace_tag(TRACE_TAG_DN)   // override the loop's attn_std stamp: dn layers color separately
    dn_flush_layer(s, l)   // every prefill branch reads/round-trips CPU state buffers
    if (g_dn_prefill_mode == DnPrefillMode.chunked && npos >= 32l && moe_gpu_dn_on_gpu(l)) {
        deltanet_prefill_gpu(t, s, l, npos)   // buckets dn_norm / dn_mm_in / dn_gpu / dn_resid
        return
    }
    let c = t.config
    let dim = c.dim
    let cd = dn_conv_dim(c)
    let di = c.ssm_d_inner
    let nvh = c.ssm_dt_rank
    let ts_norm0 = prof_ticks()
    rms_batch(s.xb_b, s.x_b, t, t.rms_att_off + l * dim, dim, npos)
    prof_add("dn_norm", ts_norm0)
    let ts_qkv = prof_ticks()
    mm_b_f(s, s.dn_qkv_b, t, fmt_at(t.dnqkv_fmt, l), t.dnqkv_offs[l], s.xb_b, dim, cd, npos)
    mm_b_f(s, s.dn_z_b, t, fmt_at(t.dngate_fmt, l), t.dngate_offs[l], s.xb_b, dim, di, npos)
    if (t.dn_ba_f32) {   // F32-on-disk beta/alpha matmul fp32, like llama.cpp
        mm_fblob_b(s.dn_beta_b, t, t.dn_betaf_off + l * dim * nvh, s.xb_b, dim, nvh, npos)
        mm_fblob_b(s.dn_g_b, t, t.dn_alphaf_off + l * dim * nvh, s.xb_b, dim, nvh, npos)
    } else {
        mm_b_f(s, s.dn_beta_b, t, KqFmt.q8, t.dnbeta_offs[l], s.xb_b, dim, nvh, npos)
        mm_b_f(s, s.dn_g_b, t, KqFmt.q8, t.dnalpha_offs[l], s.xb_b, dim, nvh, npos)
    }
    prof_add("dn_mm_in", ts_qkv)
    if (g_dn_prefill_mode == DnPrefillMode.chunked && npos > 1l) {   // single rows: recurrent is cheaper
        deltanet_prefill_chunked(t, s, l, npos)   // buckets dn_conv + dn_scan inside
    } else {
        let ts_core = prof_ticks()
        unsafe {
            let qkvp = addr < float const? >(s.dn_qkv_b[0])
            var zp = addr(s.dn_z_b[0])
            var bp = addr(s.dn_beta_b[0])
            var gp = addr(s.dn_g_b[0])
            var op = addr(s.dn_o_b[0])
            for (p in range64(npos)) {
                dn_token_core(t, s, l, qkvp + p * cd, zp + p * di, bp + p * nvh, gp + p * nvh, op + p * di)
            }
        }
        prof_add("dn_core", ts_core)
    }
    let ts_wo = prof_ticks()
    mm_b_f(s, s.xb2_b, t, fmt_at(t.dnout_fmt, l), t.dnout_offs[l], s.dn_o_b, di, dim, npos)
    prof_add("dn_mm_out", ts_wo)
    let ts_resid = prof_ticks()
    add_inplace(unsafe(addr(s.x_b[0])), unsafe(addr(s.xb2_b[0])), npos * dim)
    prof_add("dn_resid", ts_resid)
}

// The batched gated-attention twin of attention_std_prefill: 2x-wide [q|gate], partial rope,
// sigmoid out-gate before the shared out-proj suffix.
def private attention_qwen35_gated_prefill(t : Model; var s : Session; l : int64; npos : int64; start_pos : int64) { // nolint:STYLE038 — prefill twin of the decode block above; same register-coupled phase chain over the token batch
    if (attn_gpu_prefill_ready(s, l, npos, start_pos)) {
        attention_gpu_prefill(t, s, l, npos, true)   // buckets at_norm / at_gpu / kv_store / at_resid
        return
    }
    let c = t.config
    let dim = c.dim
    let kv_dim = layer_kv_dim(c, l)
    let head_size = layer_head_size(c, l)
    let n_heads = c.n_heads
    let n_kv_heads = layer_n_kv_heads(c, l)
    let qd = n_heads * head_size
    let kv_mul = n_heads / n_kv_heads
    let scale = c.attn_scale > 0.0 ? c.attn_scale : 1.0 / sqrt(float(head_size))
    let rot = c.rope_dim > 0l ? c.rope_dim : head_size
    var kcl = kv_layer_kbase(s, l, c.seq_len)
    var vcl = kv_layer_vbase(s, l, c.seq_len)
    let ts_norm0 = prof_ticks()
    rms_batch(s.xb_b, s.x_b, t, t.rms_att_off + l * dim, dim, npos)
    prof_add("norm", ts_norm0)
    let ts_qkv = prof_ticks()
    mm_b_f(s, s.dn_qg_b, t, fmt_at(t.wq_fmt, l), t.wq_offs[l], s.xb_b, dim, 2l * qd, npos)
    mm_b_f(s, s.k_b, t, fmt_at(t.wk_fmt, l), t.wk_offs[l], s.xb_b, dim, kv_dim, npos)
    mm_b_f(s, s.v_b, t, fmt_at(t.wv_fmt, l), t.wv_offs[l], s.xb_b, dim, kv_dim, npos)
    // deinterleave q/gate rows (q at 2h·hs, gate at (2h+1)·hs), threaded: serial copy was a ~3ms all-lane hole per layer
    unsafe {
        let qgp = addr < float const? >(s.dn_qg_b[0])
        var qp = addr(s.q_b[0])
        var gp = addr(s.dn_qgate_b[0])
        maybe_parallel_for(npos * qd >= g_act_par_threshold, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                for (pp in range(rb, re)) {
                    let p = int64(pp)
                    for (h in range64(n_heads)) {
                        let sb = p * 2l * qd + h * 2l * head_size
                        let db = p * qd + h * head_size
                        copy_floats(qp + db, qgp + sb, head_size)
                        copy_floats(gp + db, qgp + sb + head_size, head_size)
                    }
                }
            }
        }
    }
    prof_add("mm_qkv", ts_qkv)
    let ts_qknorm = prof_ticks()
    rms_batch(s.q_b, s.q_b, t, t.rms_q_offs[l], head_size, npos * n_heads)
    rms_batch(s.k_b, s.k_b, t, t.rms_k_offs[l], head_size, npos * n_kv_heads)
    prof_add("norm", ts_qknorm)
    let ts_rope = prof_ticks()
    unsafe {
        let half = rot / 2l
        var qbp = addr(s.q_b[0])
        var kbp = addr(s.k_b[0])
        if (g_use_rope_table) {
            let cp = addr < float const? >(s.rope_cos[0])
            let sp2 = addr < float const? >(s.rope_sin[0])
            for (p in range64(npos)) {
                rope_scaled_neox_tab_part(qbp + p * qd, head_size, rot, qd, cp + p * half, sp2 + p * half)
                rope_scaled_neox_tab_part(kbp + p * kv_dim, head_size, rot, kv_dim, cp + p * half, sp2 + p * half)
            }
        } else {
            for (p in range64(npos)) {
                rope_scaled_neox_part(qbp + p * qd, start_pos + p, head_size, rot, qd, c.rope_freq_base, c.rope_freq_scale, false, null, c.rope_mscale)
                rope_scaled_neox_part(kbp + p * kv_dim, start_pos + p, head_size, rot, kv_dim, c.rope_freq_base, c.rope_freq_scale, false, null, c.rope_mscale)
            }
        }
    }
    prof_add("rope", ts_rope)
    let ktq4 = s.kv_dtype_k == KVDtype.tq4
    let vtq4 = s.kv_dtype_v == KVDtype.tq4
    if (ktq4 || vtq4) {   // tq4 basis change (attention_std_prefill's twin); xb_b un-rotates below
        let ts_rot = prof_ticks()
        unsafe {
            let sgp = addr < float const? >(s.kv_signs[0])
            if (ktq4) {
                tq4_rotate_batch(s.q_b, npos, qd, head_size, sgp)
                tq4_rotate_batch(s.k_b, npos, kv_dim, head_size, sgp)
            }
            if (vtq4) {
                tq4_rotate_batch(s.v_b, npos, kv_dim, head_size, sgp)
            }
        }
        prof_add("tq4_rot", ts_rot)
    }
    let ts_kv = prof_ticks()
    let store_nrun = kv_runs(s, start_pos, start_pos + npos, s.kv_runs)
    unsafe {
        kv_store_batch(kcl, vcl, s.kv_dtype_k, s.kv_dtype_v, s.k_b, s.v_b,
            addr < KVRun const? >(s.kv_runs[0]), store_nrun, start_pos, npos, kv_dim)
    }
    prof_add("kv_store", ts_kv)
    let ts_attn = prof_ticks()
    prefill_attention(s, kcl, vcl, npos, start_pos, head_size, qd, kv_dim, n_heads, kv_mul, scale,
                      false, 0l, c.attn_logit_softcap, null)
    prof_add("attn", ts_attn)
    if (vtq4) {   // un-rotate every position's xb before the out-gate (gate is in the original basis)
        unsafe {
            tq4_unrotate_batch(s.xb_b, npos, qd, head_size, addr < float const? >(s.kv_signs[0]))
        }
    }
    // threaded + exp4: serial scalar loop was npos·qd (2.1M @pp512) libm exps, ~10ms all-lane hole/layer
    let ts_gate = prof_ticks()
    unsafe {
        var xbp = addr(s.xb_b[0])
        let gbp = addr < float const? >(s.dn_qgate_b[0])
        let vec = g_act_vec
        maybe_parallel_for(npos * qd >= g_act_par_threshold, 0, int(npos), get_dispatch_lanes()) $(rb, re) {
            unsafe {
                let off = int64(rb) * qd
                let cnt = int64(re - rb) * qd
                if (vec) {
                    sigmoid_gate4(xbp + off, gbp + off, cnt)
                } else {
                    sigmoid_gate(xbp + off, gbp + off, cnt)
                }
            }
        }
    }
    prof_add("act", ts_gate)
    attn_out_suffix(t, s, l, npos)
}

// The qwen35 prefill attention block: forward-only state guard, then deltanet or gated attention.
[no_alloc, no_env, no_io]
def private attention_qwen35_prefill(t : Model; var s : Session; l : int64; npos : int64; start_pos : int64) {
    if (l == 0l) {   // recurrent state is forward-only: full reset at 0, no rewind
        if (start_pos == 0l) {
            dn_reset(s)
        } elif (start_pos != s.dn_pos) {
            panic("dasLLAMA: deltanet state is forward-only - cannot prefill from position {start_pos} (session is at {s.dn_pos}); rewinding needs a fresh session")
        }
        s.dn_pos = start_pos + npos
    }
    if (layer_is_recurrent(t.config, l)) {
        deltanet_prefill(t, s, l, npos)
    } else {
        attention_qwen35_gated_prefill(t, s, l, npos, start_pos)
    }
}

// ffn-norm → batched gated FFN → down-proj(+post-norm) → residual.
[no_alloc, no_env, no_io]

//! qwen35's block set: the hybrid Gated-DeltaNet / gated-attention dispatch + the dense FFN.
//! attn_is_std = false — batched decode falls back to per-row forward() (recurrent state is
//! per-session and strictly sequential).
def qwen35_blocks() : ArchBlocks {
    return ArchBlocks(attn_decode = @@attention_qwen35_decode, ffn_decode = @@ffn_dense_decode,
                      attn_prefill = @@attention_qwen35_prefill, ffn_prefill = @@ffn_dense_prefill,
                      attn_is_std = false)
}

//! qwen35moe's block set: the same hybrid attention dispatch + the routed-experts FFN
//! (softmax top-k with renorm + the qwen2moe-style sigmoid-gated shared expert).
def qwen35moe_blocks() : ArchBlocks {
    return ArchBlocks(attn_decode = @@attention_qwen35_decode, ffn_decode = @@ffn_moe_decode,
                      attn_prefill = @@attention_qwen35_prefill, ffn_prefill = @@ffn_moe_prefill,
                      attn_is_std = false, ffn_is_dense = false)
}
