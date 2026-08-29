options gen2
options _dasllama_internal = true

// Fused prefill attention lab (M5 NAX arc phase 1): ONE kernel per (32-query block, head)
// walking K/V in 32-position tiles — scores live in threadgroup scratch (never DRAM),
// online softmax, O held in an accumulate-mode cooperative tensor. Raced against the
// production three-pass (QK / softmax / AV) at 1B-like shapes, numerics gated against a
// CPU oracle. Causal-only v1: window/softcap/sink/span join before production wiring.

require math
require daslib/fio   // the lifted reference-flash arm reads its .metal sibling
require metal/msl_shader
require metal/das_metal_boost
require ?das_metal das_metal
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_prefill
require dasllama/dasllama_metal_lens

var g_lab_pso_fused : MetalComputePipeline?

[metal_dispatch(name = "lab_enc_fused_attn", pso = "g_lab_pso_fused", tgmem = "lab_pf_fused_attn_msl_tgmem", tg = 128, grid = "mp/32, heads", params = "mp : int64, heads : int64")]
class LabPfFusedAttn {
    @ssbo @binding = 0 @role = "read" q : array<float>     // [mp x qd], roped, UNSCALED
    @ssbo @binding = 1 @role = "read" k : array<float>     // [np x kv_dim]
    @ssbo @binding = 2 @role = "read" v : array<float>     // [np x kv_dim]
    @ssbo @binding = 3 @role = "write" xb : array<float>    // [mp x qd] attention output
    @uniform @binding = 4 ka : AttnArgs
    @workgroup tq : float16[2048]           // Q tile: 32 q-rows x hs (<=64), scale folded
    @workgroup tk : float16[2048]           // K tile: 32 k-rows x hs, per jb
    @workgroup tv : float16[2048]           // V^T tile: hs v-cols x 32 pos, per jb
    @workgroup ts : float[1024]             // score tile 32x32 f32 (the never-DRAM slab)
    @workgroup tp : float16[1024]           // P tile 32x32 f16 (post-softmax)
    @workgroup trs : float[32]              // per-row online rescale exp(m_old - m_new)
    @workgroup tinv : float[32]             // per-row 1/sum at the end
    @workgroup tsm : float[32]              // running row max
    @workgroup tss : float[32]              // running row sum

    [metal_kernel(name="lab_pf_fused_attn_msl")]
    def lab_pf_fused_attn {   // nolint:STYLE038 — one MSL kernel body, tgmem-barrier coupled
        let qi0 = gl_WorkGroupID.x * 32u
        let h = gl_WorkGroupID.y
        let hs = ka.head_size
        let kvhoff = (h / ka.kv_mul) * hs
        let qhoff = h * hs
        let lid = gl_LocalInvocationID.x
        var acc : float[2048]               // O: 32 q-rows x hs, cooperative on GPU
        // stage Q once, scale folded
        var i = lid
        while (i < 32u * hs) {
            tq[i] = float16(q[(qi0 + i / hs) * ka.qd + qhoff + i % hs] * ka.scale)
            i += gl_WorkGroupSize.x
        }
        var r0 = lid
        while (r0 < 32u) {
            tsm[r0] = -3.0e38
            tss[r0] = 0.0
            r0 += gl_WorkGroupSize.x
        }
        tmm2d_tg_begin(acc, 32u, 64u, 4u, 32u)
        let jlimit = min(ka.qoff + qi0 + 32u, ka.np32)
        var jb = 0u
        while (jb < jlimit) {
            // K tile: 32 key rows x hs, rows past npos zeroed (pads can carry NaN)
            var w = lid
            while (w < 64u) {
                let r = w >> 1u
                let kh = (w & 1u) * 32u
                let pos = jb + r
                if (kh < hs) {
                    for [unroll_full] (t in range(32)) {
                        tk[r * hs + kh + uint(t)] = pos < ka.npos ? float16(k[pos * ka.kv_dim + kvhoff + kh + uint(t)]) : float16(0.0)
                    }
                }
                w += gl_WorkGroupSize.x
            }
            barrier()
            tmm2d_tile_tg(tq, tk, ts, 32u, 32u, 4u, 64u)
            barrier()
            // online softmax: thread r < 32 owns q-row r (width-1 replay walks all rows)
            var rr = lid
            while (rr < 32u) {
                let qglob = ka.qoff + qi0 + rr
                var tmax = -3.0e38
                for (j in range(32)) {
                    let jglob = jb + uint(j)
                    if (jglob <= qglob && jglob < ka.npos) {
                        tmax = max(tmax, ts[int(rr) * 32 + j])
                    }
                }
                let mold = tsm[rr]
                let mnew = max(mold, tmax)
                let resc = mnew > -3.0e38 ? exp(max(mold - mnew, -80.0)) : 1.0
                var psum = 0.0
                for (j in range(32)) {
                    let jglob = jb + uint(j)
                    var p = 0.0
                    if (jglob <= qglob && jglob < ka.npos && mnew > -3.0e38) {
                        p = exp(ts[int(rr) * 32 + j] - mnew)
                    }
                    tp[int(rr) * 32 + j] = float16(p)
                    psum += p
                }
                tsm[rr] = mnew
                tss[rr] = tss[rr] * resc + psum
                trs[rr] = resc
                rr += gl_WorkGroupSize.x
            }
            barrier()
            tmm2d_tg_rowscale(acc, trs, 32u, 64u)
            // V^T tile: hs v-cols x 32 pos (the transpose gather), pad pos zeroed
            w = lid
            while (w < 128u) {
                let c = w >> 1u
                let ph = (w & 1u) * 16u
                if (c < hs) {
                    for [unroll_full] (t in range(16)) {
                        let pos = jb + ph + uint(t)
                        tv[c * 32u + ph + uint(t)] = pos < ka.npos ? float16(v[pos * ka.kv_dim + kvhoff + c]) : float16(0.0)
                    }
                }
                w += gl_WorkGroupSize.x
            }
            barrier()
            tmm2d_tg_step(acc, tp, tv, 32u, 64u, 32u)
            barrier()
            jb += 32u
        }
        var rf = lid
        while (rf < 32u) {
            tinv[rf] = tss[rf] > 0.0 ? 1.0 / tss[rf] : 0.0
            rf += gl_WorkGroupSize.x
        }
        barrier()
        tmm2d_tg_rowscale(acc, tinv, 32u, 64u)
        var op = unsafe(addr(xb[qi0 * ka.qd + qhoff]))
        tmm2d_tg_store(acc, op, 32u, 64u, ka.qd)
    }
}

var g_lab_pso_twr : MetalComputePipeline?

// The TOWER flash form: bidirectional (every key visible - uend the whole canvas), COMPACT
// hs=72 heads (the vision towers' pre-restride q/k/v layout) - no 128 pad, no restride
// passes, no score slab, no rowstat pass. One tg per (32-query block, head); K/V walk in
// 32-position tiles, online softmax in f32, O in an accumulate-mode cooperative tensor.
[metal_dispatch(name = "lab_enc_twr_attn", pso = "g_lab_pso_twr", tgmem = "lab_twr_attn_msl_tgmem", tg = 128, grid = "mp/32, heads", params = "mp : int64, heads : int64")]
class LabTowerFlashAttn {
    @ssbo @binding = 0 @role = "read" q : array<float>     // [np x heads*72], roped, UNSCALED
    @ssbo @binding = 1 @role = "read" k : array<float>     // [np x heads*72]
    @ssbo @binding = 2 @role = "read" v : array<float>     // [np x heads*72]
    @ssbo @binding = 3 @role = "write" xb : array<float>   // [mp x heads*72] attention output
    @uniform @binding = 4 ka : AttnArgs
    @workgroup tq : float16[2560]           // Q tile: 32 q-rows x 80 (72 + 8 zero pad - matmul2d wants K%16), scale folded
    @workgroup tk : float16[2560]           // K tile: 32 k-rows x 80, per jb
    @workgroup tv : float16[2304]           // V^T tile: 72 v-cols x 32 pos, per jb
    @workgroup ts : float[1024]             // score tile 32x32 f32 (the never-DRAM slab)
    @workgroup tp : float16[1024]           // P tile 32x32 f16 (post-softmax)
    @workgroup trs : float[32]              // per-row online rescale exp(m_old - m_new)
    @workgroup tinv : float[32]             // per-row 1/sum at the end
    @workgroup tsm : float[32]              // running row max
    @workgroup tss : float[32]              // running row sum

    [metal_kernel(name="lab_twr_attn_msl")]
    def lab_twr_attn {   // nolint:STYLE038 — one MSL kernel body, tgmem-barrier coupled
        let qi0 = gl_WorkGroupID.x * 32u
        let h = gl_WorkGroupID.y
        let hs = 72u
        let hoff = h * hs
        let lid = gl_LocalInvocationID.x
        var acc : float[2304]               // O: 32 q-rows x 72, cooperative on GPU
        var i = lid
        while (i < 32u * 80u) {
            let qr = qi0 + i / 80u
            let dd = i % 80u
            tq[i] = (qr < ka.npos && dd < hs) ? float16(q[qr * ka.qd + hoff + dd] * ka.scale) : float16(0.0)
            i += gl_WorkGroupSize.x
        }
        var r0 = lid
        while (r0 < 32u) {
            tsm[r0] = -3.0e38
            tss[r0] = 0.0
            r0 += gl_WorkGroupSize.x
        }
        tmm2d_tg_begin(acc, 32u, 72u, 4u, 32u)
        var jb = 0u
        while (jb < ka.np32) {
            // K tile: 32 key rows x 72, rows past npos zeroed (pads can carry NaN)
            var w = lid
            while (w < 64u) {
                let r = w >> 1u
                let kh = (w & 1u) * 40u
                let pos = jb + r
                for [unroll_full] (t in range(40)) {
                    let dd = kh + uint(t)
                    tk[r * 80u + dd] = (pos < ka.npos && dd < hs) ? float16(k[pos * ka.kv_dim + hoff + dd]) : float16(0.0)
                }
                w += gl_WorkGroupSize.x
            }
            barrier()
            tmm2d_tile_tg(tq, tk, ts, 32u, 32u, 4u, 80u)
            barrier()
            // online softmax: thread r < 32 owns q-row r; every staged key column is visible
            var rr = lid
            while (rr < 32u) {
                var tmax = -3.0e38
                for (j in range(32)) {
                    if (jb + uint(j) < ka.npos) {
                        tmax = max(tmax, ts[int(rr) * 32 + j])
                    }
                }
                let mold = tsm[rr]
                let mnew = max(mold, tmax)
                let resc = mnew > -3.0e38 ? exp(max(mold - mnew, -80.0)) : 1.0
                var psum = 0.0
                for (j in range(32)) {
                    var p = 0.0
                    if (jb + uint(j) < ka.npos && mnew > -3.0e38) {
                        p = exp(ts[int(rr) * 32 + j] - mnew)
                    }
                    tp[int(rr) * 32 + j] = float16(p)
                    psum += p
                }
                tsm[rr] = mnew
                tss[rr] = tss[rr] * resc + psum
                trs[rr] = resc
                rr += gl_WorkGroupSize.x
            }
            barrier()
            tmm2d_tg_rowscale(acc, trs, 32u, 72u)
            // V^T tile: 72 v-cols x 32 pos (the transpose gather), pad pos zeroed
            w = lid
            while (w < 144u) {
                let c = w >> 1u
                let ph = (w & 1u) * 16u
                for [unroll_full] (t in range(16)) {
                    let pos = jb + ph + uint(t)
                    tv[c * 32u + ph + uint(t)] = pos < ka.npos ? float16(v[pos * ka.kv_dim + hoff + c]) : float16(0.0)
                }
                w += gl_WorkGroupSize.x
            }
            barrier()
            tmm2d_tg_step(acc, tp, tv, 32u, 72u, 32u)
            barrier()
            jb += 32u
        }
        var rf = lid
        while (rf < 32u) {
            tinv[rf] = tss[rf] > 0.0 ? 1.0 / tss[rf] : 0.0
            rf += gl_WorkGroupSize.x
        }
        barrier()
        tmm2d_tg_rowscale(acc, tinv, 32u, 72u)
        var op = unsafe(addr(xb[qi0 * ka.qd + hoff]))
        tmm2d_tg_store(acc, op, 32u, 72u, ka.qd)
    }
}

def private cpu_oracle(q, k, v : array<float>; var o : array<float>; np, heads, hs, kvmul, qd, kvd : int; scale : float) {
    for (h in range(heads)) {
        let kvh = (h / kvmul) * hs
        for (qi in range(np)) {
            var mx = -3.0e38
            for (j in range(qi + 1)) {
                var s = 0.0lf
                for (d in range(hs)) {
                    s += double(q[qi * qd + h * hs + d]) * double(k[j * kvd + kvh + d])
                }
                mx = max(mx, float(s) * scale)
            }
            var tot = 0.0
            var pw : array<float>
            pw |> resize(qi + 1)
            for (j in range(qi + 1)) {
                var s = 0.0lf
                for (d in range(hs)) {
                    s += double(q[qi * qd + h * hs + d]) * double(k[j * kvd + kvh + d])
                }
                let e = exp(float(s) * scale - mx)
                pw[j] = e
                tot += e
            }
            for (d in range(hs)) {
                var oa = 0.0lf
                for (j in range(qi + 1)) {
                    oa += double(pw[j]) * double(v[j * kvd + kvh + d])
                }
                o[qi * qd + h * hs + d] = float(oa / double(tot))
            }
            delete pw
        }
    }
}

[export]
def main {   // nolint:STYLE038 — one-shot A/B race harness; buffers + PSOs stay live to the release tail
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var dev = metal_create_system_default_device()
        var queue = metal_new_command_queue(dev)
        var err = ""
        g_lab_pso_fused = pipeline_from_source(dev, lab_pf_fused_attn_msl, lab_pf_fused_attn_msl_entry, lab_pf_fused_attn_msl_fastmath, err)
        if (g_lab_pso_fused == null) {
            print("fused pso: {err}\n")
            return
        }
        g_lab_pso_twr = pipeline_from_source(dev, lab_twr_attn_msl, lab_twr_attn_msl_entry, lab_twr_attn_msl_fastmath, err)
        if (g_lab_pso_twr == null) {
            print("tower pso: {err}\n")
            return
        }
        var pso_qk = pipeline_from_source(dev, metal_attn_qk_mm_msl, metal_attn_qk_mm_msl_entry, metal_attn_qk_mm_msl_fastmath, err)
        var pso_sm = pipeline_from_source(dev, metal_attn_rowstat_msl, metal_attn_rowstat_msl_entry, metal_attn_rowstat_msl_fastmath, err)
        var pso_av = pipeline_from_source(dev, MetalAttnAVMm_metal_attn_av_mm_msl, MetalAttnAVMm_metal_attn_av_mm_msl_entry, MetalAttnAVMm_metal_attn_av_mm_msl_fastmath, err)
        var pso_qkt = pipeline_from_source(dev, MetalAttnQKMmT_metal_attn_qk_mm_t_msl, MetalAttnQKMmT_metal_attn_qk_mm_t_msl_entry, MetalAttnQKMmT_metal_attn_qk_mm_t_msl_fastmath, err)
        var pso_avt = pipeline_from_source(dev, MetalAttnAVMmT_metal_attn_av_mm_t_msl, MetalAttnAVMmT_metal_attn_av_mm_t_msl_entry, MetalAttnAVMmT_metal_attn_av_mm_t_msl_fastmath, err)
        if (pso_qk == null || pso_sm == null || pso_av == null || pso_qkt == null || pso_avt == null) {
            print("three-pass pso: {err}\n")
            return
        }
        // rows: decoder-1B-like (the arc's original pair) + the gemma3v tower shape (bidirectional
        // via uend = np - the vision-canvas attention the slab bills 27x per encode)
        for (shape in [(np = 512, heads = 32, hs = 64, kvmul = 4, uend = 0),
                       (np = 2048, heads = 32, hs = 64, kvmul = 4, uend = 0),
                       (np = 4096, heads = 16, hs = 128, kvmul = 1, uend = 4096)]) {
            let np = shape.np
            let heads = shape.heads
            let hs = shape.hs
            let kvmul = shape.kvmul
            let qd = heads * hs
            let kvd = (heads / kvmul) * hs
            var ka = AttnArgs(qd = uint(qd), kv_dim = uint(kvd), head_size = uint(hs), kv_mul = uint(kvmul),
                npos = uint(np), np32 = uint(np), scale = 0.125, qoff = 0u, qrows = uint(np),
                window = 0u, softcap = 0.0, hass = 0u, uend = uint(shape.uend), ulo = 0u)
            var qh <- race_x_f32(np * qd)
            var kh <- race_x_f32(np * kvd)
            var vh <- race_x_f32(np * kvd)
            var bq = race_buf(dev, uint64(np * qd * 4), unsafe(addr<void?>(qh[0])))
            var bk = race_buf(dev, uint64(np * kvd * 4), unsafe(addr<void?>(kh[0])))
            var bv = race_buf(dev, uint64(np * kvd * 4), unsafe(addr<void?>(vh[0])))
            var bo = race_buf(dev, uint64(np * qd * 4), null)
            var bs = race_buf(dev, uint64(np * qd * 4), null)     // three-pass out
            var batt = race_buf(dev, uint64(heads * np * np * 2), null)   // f16 score slab
            var bstat = race_buf(dev, uint64(heads * np * 2 * 4), null)
            var bsink = race_buf(dev, uint64(heads * 4), null)
            var t_fused = -1.0lf
            var t_three = -1.0lf
            race_pair_ms(queue, 5, t_three, t_fused, $(enc) {
                kn_pipeline(enc, pso_qk)
                kn_tgmem(enc, metal_attn_qk_mm_msl_tgmem, 0)
                kn_buffer(enc, bq, 0ul, 0)
                kn_buffer(enc, bk, 0ul, 1)
                kn_buffer(enc, batt, 0ul, 2)
                kn_kargs(enc, ka, 3)
                kn_dispatch(enc, uint3(uint(np / 32), uint(np / 64), uint(heads)), uint3(128u, 1u, 1u))
                kn_pipeline(enc, pso_sm)
                kn_tgmem(enc, metal_attn_rowstat_msl_tgmem, 0)
                kn_buffer(enc, batt, 0ul, 0)
                kn_kargs(enc, ka, 1)
                kn_buffer(enc, bstat, 0ul, 2)
                kn_buffer(enc, bsink, 0ul, 20)
                kn_dispatch(enc, uint3(uint(np), uint(heads), 1u), uint3(128u, 1u, 1u))
                kn_pipeline(enc, pso_av)
                kn_tgmem(enc, MetalAttnAVMm_metal_attn_av_mm_msl_tgmem, 0)
                kn_buffer(enc, batt, 0ul, 0)
                kn_buffer(enc, bv, 0ul, 1)
                kn_buffer(enc, bs, 0ul, 2)
                kn_kargs(enc, ka, 3)
                kn_buffer(enc, bstat, 0ul, 4)
                kn_dispatch(enc, uint3(uint(np / 32), uint(hs / 64), uint(heads)), uint3(128u, 1u, 1u))
            }, $(enc) {
                kn_pipeline(enc, g_lab_pso_fused)
                kn_tgmem(enc, lab_pf_fused_attn_msl_tgmem, 0)
                kn_buffer(enc, bq, 0ul, 0)
                kn_buffer(enc, bk, 0ul, 1)
                kn_buffer(enc, bv, 0ul, 2)
                kn_buffer(enc, bo, 0ul, 3)
                kn_kargs(enc, ka, 4)
                kn_dispatch(enc, uint3(uint(np / 32), uint(heads), 1u), uint3(128u, 1u, 1u))
            })
            // numerics: fused vs the CPU oracle (rel tolerance — exp reorders float sums)
            var oc : array<float>
            oc |> resize(np * qd)
            cpu_oracle(qh, kh, vh, oc, np, heads, hs, kvmul, qd, kvd, 0.125)
            var maxrel = 0.0
            unsafe {
                let pg = reinterpret<float?>(metal_buffer_contents(bo))
                for (ii in range(np * qd)) {
                    let d = abs(pg[ii] - oc[ii])
                    let r = d / max(abs(oc[ii]), 1e-3)
                    maxrel = max(maxrel, r)
                }
            }
            print("np={np}: three-pass={t_three}ms fused={t_fused}ms ratio={t_fused / t_three}  maxrel={maxrel}\n")
            var t_qkb = -1.0lf
            var t_qkt = -1.0lf
            race_pair_ms(queue, 5, t_qkb, t_qkt, $(enc) {
                kn_pipeline(enc, pso_qk)
                kn_tgmem(enc, metal_attn_qk_mm_msl_tgmem, 0)
                kn_buffer(enc, bq, 0ul, 0)
                kn_buffer(enc, bk, 0ul, 1)
                kn_buffer(enc, batt, 0ul, 2)
                kn_kargs(enc, ka, 3)
                kn_dispatch(enc, uint3(uint(np / 32), uint(np / 64), uint(heads)), uint3(128u, 1u, 1u))
            }, $(enc) {
                kn_pipeline(enc, pso_qkt)
                kn_tgmem(enc, MetalAttnQKMmT_metal_attn_qk_mm_t_msl_tgmem, 0)
                kn_buffer(enc, bq, 0ul, 0)
                kn_buffer(enc, bk, 0ul, 1)
                kn_buffer(enc, batt, 0ul, 2)
                kn_kargs(enc, ka, 3)
                kn_dispatch(enc, uint3(uint(np / 32), uint(np / 64), uint(heads)), uint3(128u, 1u, 1u))
            })
            print("  qk_mm np={np}: base={t_qkb}ms tensor={t_qkt}ms ratio={t_qkt / t_qkb}\n")
            var t_avb = -1.0lf
            var t_avt = -1.0lf
            race_pair_ms(queue, 5, t_avb, t_avt, $(enc) {
                kn_pipeline(enc, pso_av)
                kn_tgmem(enc, MetalAttnAVMm_metal_attn_av_mm_msl_tgmem, 0)
                kn_buffer(enc, batt, 0ul, 0)
                kn_buffer(enc, bv, 0ul, 1)
                kn_buffer(enc, bs, 0ul, 2)
                kn_kargs(enc, ka, 3)
                kn_buffer(enc, bstat, 0ul, 4)
                kn_dispatch(enc, uint3(uint(np / 32), uint(hs / 64), uint(heads)), uint3(128u, 1u, 1u))
            }, $(enc) {
                kn_pipeline(enc, pso_avt)
                kn_tgmem(enc, MetalAttnAVMmT_metal_attn_av_mm_t_msl_tgmem, 0)
                kn_buffer(enc, batt, 0ul, 0)
                kn_buffer(enc, bv, 0ul, 1)
                kn_buffer(enc, bs, 0ul, 2)
                kn_kargs(enc, ka, 3)
                kn_buffer(enc, bstat, 0ul, 4)
                kn_dispatch(enc, uint3(uint(np / 32), uint(hs / 64), uint(heads)), uint3(128u, 1u, 1u))
            })
            print("  av_mm np={np}: base={t_avb}ms tensor={t_avt}ms ratio={t_avt / t_avb}\n")
            if (shape.uend > 0) {
                // the tower flash arm: compact hs=72 (first 72 of each padded head-block) vs
                // the production three-pass on its padded layout - the exact A/B production faces
                let hst = 72
                let qdt = heads * hst
                var qt : array<float>
                var kt : array<float>
                var vt : array<float>
                qt |> resize(np * qdt)
                kt |> resize(np * qdt)
                vt |> resize(np * qdt + 256)   // +tail slack: the lifted flash's dead PV tiles read past the last V row
                for (p in range(np)) {
                    for (hh in range(heads)) {
                        for (dd in range(hst)) {
                            qt[(p * heads + hh) * hst + dd] = qh[p * qd + hh * hs + dd]
                            kt[(p * heads + hh) * hst + dd] = kh[p * kvd + hh * hs + dd]
                            vt[(p * heads + hh) * hst + dd] = vh[p * kvd + hh * hs + dd]
                        }
                    }
                }
                var bqt = race_buf(dev, uint64(np * qdt * 4), unsafe(addr<void?>(qt[0])))
                var bkt = race_buf(dev, uint64(np * qdt * 4), unsafe(addr<void?>(kt[0])))
                var bvt = race_buf(dev, uint64((np * qdt + 256) * 4), unsafe(addr<void?>(vt[0])))
                var bot = race_buf(dev, uint64(np * qdt * 4), null)
                var kat = AttnArgs(qd = uint(qdt), kv_dim = uint(qdt), head_size = uint(hst), kv_mul = 1u,
                    npos = uint(np), np32 = uint(np), scale = 0.125, qoff = 0u, qrows = uint(np),
                    window = 0u, softcap = 0.0, hass = 0u, uend = uint(np), ulo = 0u)
                var t_slab = -1.0lf
                var t_flash = -1.0lf
                race_pair_ms(queue, 5, t_slab, t_flash, $(enc) {
                    kn_pipeline(enc, pso_qkt)
                    kn_buffer(enc, bq, 0ul, 0)
                    kn_buffer(enc, bk, 0ul, 1)
                    kn_buffer(enc, batt, 0ul, 2)
                    kn_kargs(enc, ka, 3)
                    kn_dispatch(enc, uint3(uint(np / 32), uint(np / 64), uint(heads)), uint3(128u, 1u, 1u))
                    kn_pipeline(enc, pso_sm)
                    kn_tgmem(enc, metal_attn_rowstat_msl_tgmem, 0)
                    kn_buffer(enc, batt, 0ul, 0)
                    kn_kargs(enc, ka, 1)
                    kn_buffer(enc, bstat, 0ul, 2)
                    kn_buffer(enc, bsink, 0ul, 20)
                    kn_dispatch(enc, uint3(uint(np), uint(heads), 1u), uint3(128u, 1u, 1u))
                    kn_pipeline(enc, pso_av)
                    kn_tgmem(enc, MetalAttnAVMm_metal_attn_av_mm_msl_tgmem, 0)
                    kn_buffer(enc, batt, 0ul, 0)
                    kn_buffer(enc, bv, 0ul, 1)
                    kn_buffer(enc, bs, 0ul, 2)
                    kn_kargs(enc, ka, 3)
                    kn_buffer(enc, bstat, 0ul, 4)
                    kn_dispatch(enc, uint3(uint(np / 32), uint(hs / 64), uint(heads)), uint3(128u, 1u, 1u))
                }, $(enc) {
                    kn_pipeline(enc, g_lab_pso_twr)
                    kn_tgmem(enc, lab_twr_attn_msl_tgmem, 0)
                    kn_buffer(enc, bqt, 0ul, 0)
                    kn_buffer(enc, bkt, 0ul, 1)
                    kn_buffer(enc, bvt, 0ul, 2)
                    kn_buffer(enc, bot, 0ul, 3)
                    kn_kargs(enc, kat, 4)
                    kn_dispatch(enc, uint3(uint(np / 32), uint(heads), 1u), uint3(128u, 1u, 1u))
                })
                // the bidirectional oracle on the first 128 q-rows, computed once for every arm
                let ncheck = 128
                var owant : array<float>
                owant |> resize(ncheck * qdt)
                for (hh in range(heads)) {
                    for (qi in range(ncheck)) {
                        var mx = -3.0e38
                        for (j in range(np)) {
                            var sd = 0.0lf
                            for (dd in range(hst)) {
                                sd += double(qt[qi * qdt + hh * hst + dd]) * double(kt[j * qdt + hh * hst + dd])
                            }
                            mx = max(mx, float(sd) * 0.125)
                        }
                        var tot = 0.0lf
                        var oa : float[72]
                        for (dd in range(hst)) {
                            oa[dd] = 0.0
                        }
                        for (j in range(np)) {
                            var sd = 0.0lf
                            for (dd in range(hst)) {
                                sd += double(qt[qi * qdt + hh * hst + dd]) * double(kt[j * qdt + hh * hst + dd])
                            }
                            let e = exp(float(sd) * 0.125 - mx)
                            tot += double(e)
                            for (dd in range(hst)) {
                                oa[dd] += e * vt[j * qdt + hh * hst + dd]
                            }
                        }
                        for (dd in range(hst)) {
                            owant[qi * qdt + hh * hst + dd] = oa[dd] / float(tot)
                        }
                    }
                }
                var maxrel_t = 0.0
                unsafe {
                    let pg = reinterpret<float?>(metal_buffer_contents(bot))
                    for (ii in range(ncheck * qdt)) {
                        maxrel_t = max(maxrel_t, abs(pg[ii] - owant[ii]) / max(abs(owant[ii]), 1e-3))
                    }
                }
                print("  TOWER np={np}: slab(hs128)={t_slab}ms flash(hs72)={t_flash}ms ratio={t_flash / t_slab}  maxrel={maxrel_t}\n")
                // the lifted reference-flash arm (lcpp_flash_dk72.metal): np%64 and V-tail slack by contract
                var fsrc = ""
                fopen("modules/dasLLAMA/benchmarks/attn/lcpp_flash_dk72.metal", "rb") $(ff) {
                    if (ff != null) {
                        fsrc = fread(ff)
                    }
                }
                var perr = ""
                var pso_port = pipeline_from_source(dev, fsrc, "lab_lcpp_flash72", true, perr)
                if (pso_port == null) {
                    print("  port pso: {perr}\n")
                } else {
                    var bop = race_buf(dev, uint64(np * qdt * 4), null)
                    var t_slab2 = -1.0lf
                    var t_port = -1.0lf
                    race_pair_ms(queue, 5, t_slab2, t_port, $(enc) {
                        kn_pipeline(enc, pso_qkt)
                        kn_buffer(enc, bq, 0ul, 0)
                        kn_buffer(enc, bk, 0ul, 1)
                        kn_buffer(enc, batt, 0ul, 2)
                        kn_kargs(enc, ka, 3)
                        kn_dispatch(enc, uint3(uint(np / 32), uint(np / 64), uint(heads)), uint3(128u, 1u, 1u))
                        kn_pipeline(enc, pso_sm)
                        kn_tgmem(enc, metal_attn_rowstat_msl_tgmem, 0)
                        kn_buffer(enc, batt, 0ul, 0)
                        kn_kargs(enc, ka, 1)
                        kn_buffer(enc, bstat, 0ul, 2)
                        kn_buffer(enc, bsink, 0ul, 20)
                        kn_dispatch(enc, uint3(uint(np), uint(heads), 1u), uint3(128u, 1u, 1u))
                        kn_pipeline(enc, pso_av)
                        kn_tgmem(enc, MetalAttnAVMm_metal_attn_av_mm_msl_tgmem, 0)
                        kn_buffer(enc, batt, 0ul, 0)
                        kn_buffer(enc, bv, 0ul, 1)
                        kn_buffer(enc, bs, 0ul, 2)
                        kn_kargs(enc, ka, 3)
                        kn_buffer(enc, bstat, 0ul, 4)
                        kn_dispatch(enc, uint3(uint(np / 32), uint(hs / 64), uint(heads)), uint3(128u, 1u, 1u))
                    }, $(enc) {
                        kn_pipeline(enc, pso_port)
                        kn_tgmem(enc, 10496ul, 0)
                        kn_buffer(enc, bqt, 0ul, 0)
                        kn_buffer(enc, bkt, 0ul, 1)
                        kn_buffer(enc, bvt, 0ul, 2)
                        kn_buffer(enc, bop, 0ul, 3)
                        kn_kargs(enc, kat, 4)
                        kn_dispatch(enc, uint3(uint(np / 8), uint(heads), 1u), uint3(128u, 1u, 1u))
                    })
                    var maxrel_p = 0.0
                    unsafe {
                        let pg = reinterpret<float?>(metal_buffer_contents(bop))
                        for (ii in range(ncheck * qdt)) {
                            maxrel_p = max(maxrel_p, abs(pg[ii] - owant[ii]) / max(abs(owant[ii]), 1e-3))
                        }
                    }
                    print("  PORT  np={np}: slab(hs128)={t_slab2}ms lcpp-flash72={t_port}ms ratio={t_port / t_slab2}  maxrel={maxrel_p}\n")
                    metal_release(bop)
                    metal_release(pso_port)
                }
                // the f16 data path (the reference's own FA_TYPES): K/V device half, half8x8 operands
                var perrh = ""
                var pso_porth = pipeline_from_source(dev, fsrc, "lab_lcpp_flash72h", true, perrh)
                if (pso_porth == null) {
                    print("  port-h pso: {perrh}\n")
                } else {
                    var kth : array<float16>
                    var vth : array<float16>
                    kth |> resize(np * qdt)
                    vth |> resize(np * qdt + 256)
                    for (ii in range(np * qdt)) {
                        kth[ii] = float16(kt[ii])
                        vth[ii] = float16(vt[ii])
                    }
                    var bkth = race_buf(dev, uint64(np * qdt * 2), unsafe(addr<void?>(kth[0])))
                    var bvth = race_buf(dev, uint64((np * qdt + 256) * 2), unsafe(addr<void?>(vth[0])))
                    var boph = race_buf(dev, uint64(np * qdt * 4), null)
                    var t_slab3 = -1.0lf
                    var t_porth = -1.0lf
                    race_pair_ms(queue, 5, t_slab3, t_porth, $(enc) {
                        kn_pipeline(enc, pso_qkt)
                        kn_buffer(enc, bq, 0ul, 0)
                        kn_buffer(enc, bk, 0ul, 1)
                        kn_buffer(enc, batt, 0ul, 2)
                        kn_kargs(enc, ka, 3)
                        kn_dispatch(enc, uint3(uint(np / 32), uint(np / 64), uint(heads)), uint3(128u, 1u, 1u))
                        kn_pipeline(enc, pso_sm)
                        kn_tgmem(enc, metal_attn_rowstat_msl_tgmem, 0)
                        kn_buffer(enc, batt, 0ul, 0)
                        kn_kargs(enc, ka, 1)
                        kn_buffer(enc, bstat, 0ul, 2)
                        kn_buffer(enc, bsink, 0ul, 20)
                        kn_dispatch(enc, uint3(uint(np), uint(heads), 1u), uint3(128u, 1u, 1u))
                        kn_pipeline(enc, pso_av)
                        kn_tgmem(enc, MetalAttnAVMm_metal_attn_av_mm_msl_tgmem, 0)
                        kn_buffer(enc, batt, 0ul, 0)
                        kn_buffer(enc, bv, 0ul, 1)
                        kn_buffer(enc, bs, 0ul, 2)
                        kn_kargs(enc, ka, 3)
                        kn_buffer(enc, bstat, 0ul, 4)
                        kn_dispatch(enc, uint3(uint(np / 32), uint(hs / 64), uint(heads)), uint3(128u, 1u, 1u))
                    }, $(enc) {
                        kn_pipeline(enc, pso_porth)
                        kn_tgmem(enc, 9344ul, 0)
                        kn_buffer(enc, bqt, 0ul, 0)
                        kn_buffer(enc, bkth, 0ul, 1)
                        kn_buffer(enc, bvth, 0ul, 2)
                        kn_buffer(enc, boph, 0ul, 3)
                        kn_kargs(enc, kat, 4)
                        kn_dispatch(enc, uint3(uint(np / 8), uint(heads), 1u), uint3(128u, 1u, 1u))
                    })
                    var maxrel_h = 0.0
                    unsafe {
                        let pg = reinterpret<float?>(metal_buffer_contents(boph))
                        for (ii in range(ncheck * qdt)) {
                            maxrel_h = max(maxrel_h, abs(pg[ii] - owant[ii]) / max(abs(owant[ii]), 1e-3))
                        }
                    }
                    print("  PORTH np={np}: slab(hs128)={t_slab3}ms lcpp-flash72h={t_porth}ms ratio={t_porth / t_slab3}  maxrel={maxrel_h}\n")
                    // hc: K/V per-head contiguous - the reference's own tensor layout (ld = 72)
                    var perrc = ""
                    var pso_hc = pipeline_from_source(dev, fsrc, "lab_lcpp_flash72hc", true, perrc)
                    if (pso_hc == null) {
                        print("  port-hc pso: {perrc}\n")
                    } else {
                        var khc : array<float16>
                        var vhc : array<float16>
                        khc |> resize(np * qdt)
                        vhc |> resize(np * qdt + 256)
                        for (hh in range(heads)) {
                            for (p_ in range(np)) {
                                for (dd in range(hst)) {
                                    khc[(hh * np + p_) * hst + dd] = float16(kt[p_ * qdt + hh * hst + dd])
                                    vhc[(hh * np + p_) * hst + dd] = float16(vt[p_ * qdt + hh * hst + dd])
                                }
                            }
                        }
                        var bkhc = race_buf(dev, uint64(np * qdt * 2), unsafe(addr<void?>(khc[0])))
                        var bvhc = race_buf(dev, uint64((np * qdt + 256) * 2), unsafe(addr<void?>(vhc[0])))
                        var bohc = race_buf(dev, uint64(np * qdt * 4), null)
                        var t_slab4 = -1.0lf
                        var t_hc = -1.0lf
                        race_pair_ms(queue, 5, t_slab4, t_hc, $(enc) {
                            kn_pipeline(enc, pso_qkt)
                            kn_buffer(enc, bq, 0ul, 0)
                            kn_buffer(enc, bk, 0ul, 1)
                            kn_buffer(enc, batt, 0ul, 2)
                            kn_kargs(enc, ka, 3)
                            kn_dispatch(enc, uint3(uint(np / 32), uint(np / 64), uint(heads)), uint3(128u, 1u, 1u))
                            kn_pipeline(enc, pso_sm)
                            kn_tgmem(enc, metal_attn_rowstat_msl_tgmem, 0)
                            kn_buffer(enc, batt, 0ul, 0)
                            kn_kargs(enc, ka, 1)
                            kn_buffer(enc, bstat, 0ul, 2)
                            kn_buffer(enc, bsink, 0ul, 20)
                            kn_dispatch(enc, uint3(uint(np), uint(heads), 1u), uint3(128u, 1u, 1u))
                            kn_pipeline(enc, pso_av)
                            kn_tgmem(enc, MetalAttnAVMm_metal_attn_av_mm_msl_tgmem, 0)
                            kn_buffer(enc, batt, 0ul, 0)
                            kn_buffer(enc, bv, 0ul, 1)
                            kn_buffer(enc, bs, 0ul, 2)
                            kn_kargs(enc, ka, 3)
                            kn_buffer(enc, bstat, 0ul, 4)
                            kn_dispatch(enc, uint3(uint(np / 32), uint(hs / 64), uint(heads)), uint3(128u, 1u, 1u))
                        }, $(enc) {
                            kn_pipeline(enc, pso_hc)
                            kn_tgmem(enc, 9344ul, 0)
                            kn_buffer(enc, bqt, 0ul, 0)
                            kn_buffer(enc, bkhc, 0ul, 1)
                            kn_buffer(enc, bvhc, 0ul, 2)
                            kn_buffer(enc, bohc, 0ul, 3)
                            kn_kargs(enc, kat, 4)
                            kn_dispatch(enc, uint3(uint(np / 8), uint(heads), 1u), uint3(128u, 1u, 1u))
                        })
                        var maxrel_c = 0.0
                        unsafe {
                            let pg = reinterpret<float?>(metal_buffer_contents(bohc))
                            for (ii in range(ncheck * qdt)) {
                                maxrel_c = max(maxrel_c, abs(pg[ii] - owant[ii]) / max(abs(owant[ii]), 1e-3))
                            }
                        }
                        print("  PORTC np={np}: slab(hs128)={t_slab4}ms lcpp-flash72hc={t_hc}ms ratio={t_hc / t_slab4}  maxrel={maxrel_c}\n")
                        metal_release(bkhc)
                        metal_release(bvhc)
                        metal_release(bohc)
                        metal_release(pso_hc)
                        delete khc
                        delete vhc
                    }
                    metal_release(bkth)
                    metal_release(bvth)
                    metal_release(boph)
                    metal_release(pso_porth)
                    delete kth
                    delete vth
                }
                delete owant
                metal_release(bqt)
                metal_release(bkt)
                metal_release(bvt)
                metal_release(bot)
                delete qt
                delete kt
                delete vt
            }
            delete oc
            metal_release(bq)
            metal_release(bk)
            metal_release(bv)
            metal_release(bo)
            metal_release(bs)
            metal_release(batt)
            metal_release(bstat)
            metal_release(bsink)
            delete qh
            delete kh
            delete vh
        }
        metal_release(queue)
        metal_release(dev)
    }
}
