options gen2
options _dasllama_internal = true

// Fused prefill attention lab (M5 NAX arc phase 1): ONE kernel per (32-query block, head)
// walking K/V in 32-position tiles — scores live in threadgroup scratch (never DRAM),
// online softmax, O held in an accumulate-mode cooperative tensor. Raced against the
// production three-pass (QK / softmax / AV) at 1B-like shapes, numerics gated against a
// CPU oracle. Causal-only v1: window/softcap/sink/span join before production wiring.

require math
require daslib/math_bits
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
    def lab_pf_fused_attn {
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
def main {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var dev = metal_create_system_default_device()
        var queue = metal_new_command_queue(dev)
        var err = ""
        g_lab_pso_fused = pipeline_from_source(dev, lab_pf_fused_attn_msl, lab_pf_fused_attn_msl_entry, lab_pf_fused_attn_msl_fastmath, err)
        if (g_lab_pso_fused == null) {
            print("fused pso: {err}\n")
            return
        }
        var pso_qk = pipeline_from_source(dev, metal_attn_qk_mm_msl, metal_attn_qk_mm_msl_entry, metal_attn_qk_mm_msl_fastmath, err)
        var pso_sm = pipeline_from_source(dev, metal_attn_softmax_msl, metal_attn_softmax_msl_entry, metal_attn_softmax_msl_fastmath, err)
        var pso_av = pipeline_from_source(dev, metal_attn_av_mm_msl, metal_attn_av_mm_msl_entry, metal_attn_av_mm_msl_fastmath, err)
        var pso_qkt = pipeline_from_source(dev, metal_attn_qk_mm_t_msl, metal_attn_qk_mm_t_msl_entry, metal_attn_qk_mm_t_msl_fastmath, err)
        var pso_avt = pipeline_from_source(dev, metal_attn_av_mm_t_msl, metal_attn_av_mm_t_msl_entry, metal_attn_av_mm_t_msl_fastmath, err)
        if (pso_qk == null || pso_sm == null || pso_av == null || pso_qkt == null || pso_avt == null) {
            print("three-pass pso: {err}\n")
            return
        }
        let heads = 32
        let hs = 64
        let kvmul = 4
        let qd = heads * hs
        let kvd = (heads / kvmul) * hs
        for (np in [512, 2048]) {
            var ka = AttnArgs(qd = uint(qd), kv_dim = uint(kvd), head_size = uint(hs), kv_mul = uint(kvmul),
                npos = uint(np), np32 = uint(np), scale = 0.125, qoff = 0u, qrows = uint(np),
                window = 0u, softcap = 0.0, hass = 0u, uend = 0u, ulo = 0u)
            var qh <- race_x_f32(np * qd)
            var kh <- race_x_f32(np * kvd)
            var vh <- race_x_f32(np * kvd)
            var bq = race_buf(dev, uint64(np * qd * 4), unsafe(addr<void?>(qh[0])))
            var bk = race_buf(dev, uint64(np * kvd * 4), unsafe(addr<void?>(kh[0])))
            var bv = race_buf(dev, uint64(np * kvd * 4), unsafe(addr<void?>(vh[0])))
            var bo = race_buf(dev, uint64(np * qd * 4), null)
            var bs = race_buf(dev, uint64(np * qd * 4), null)     // three-pass out
            var batt = race_buf(dev, uint64(heads * np * np * 4), null)
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
                kn_tgmem(enc, metal_attn_softmax_msl_tgmem, 0)
                kn_buffer(enc, batt, 0ul, 0)
                kn_kargs(enc, ka, 1)
                kn_buffer(enc, bsink, 0ul, 20)
                kn_dispatch(enc, uint3(uint(np), uint(heads), 1u), uint3(128u, 1u, 1u))
                kn_pipeline(enc, pso_av)
                kn_tgmem(enc, metal_attn_av_mm_msl_tgmem, 0)
                kn_buffer(enc, batt, 0ul, 0)
                kn_buffer(enc, bv, 0ul, 1)
                kn_buffer(enc, bs, 0ul, 2)
                kn_kargs(enc, ka, 3)
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
                kn_tgmem(enc, metal_attn_qk_mm_t_msl_tgmem, 0)
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
                kn_tgmem(enc, metal_attn_av_mm_msl_tgmem, 0)
                kn_buffer(enc, batt, 0ul, 0)
                kn_buffer(enc, bv, 0ul, 1)
                kn_buffer(enc, bs, 0ul, 2)
                kn_kargs(enc, ka, 3)
                kn_dispatch(enc, uint3(uint(np / 32), uint(hs / 64), uint(heads)), uint3(128u, 1u, 1u))
            }, $(enc) {
                kn_pipeline(enc, pso_avt)
                kn_tgmem(enc, metal_attn_av_mm_t_msl_tgmem, 0)
                kn_buffer(enc, batt, 0ul, 0)
                kn_buffer(enc, bv, 0ul, 1)
                kn_buffer(enc, bs, 0ul, 2)
                kn_kargs(enc, ka, 3)
                kn_dispatch(enc, uint3(uint(np / 32), uint(hs / 64), uint(heads)), uint3(128u, 1u, 1u))
            })
            print("  av_mm np={np}: base={t_avb}ms tensor={t_avt}ms ratio={t_avt / t_avb}\n")
            delete oc
            metal_release(bq)
            metal_release(bk)
            metal_release(bv)
            metal_release(bo)
            metal_release(bs)
            metal_release(batt)
            metal_release(bsink)
            delete qh
            delete kh
            delete vh
        }
        metal_release(queue)
        metal_release(dev)
    }
}
