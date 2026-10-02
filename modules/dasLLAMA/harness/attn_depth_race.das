options gen2
options _dasllama_internal = true

// ATTENTION AT DEPTH - the prefill attention trio (QK scores, row softmax, AV) raced against a
// device-direct trio at one deep window: 2048 queries landing on 8192 rows of context, the
// Qwen3.6-35B-A3B geometry (16 heads of 256 over 2 KV heads). The production pair stages K, P and
// V through threadgroup memory a tile at a time; the lab pair hands matmul2d device tensors of
// half on both sides - a scaled K twin, the slab holding exp(s - max) in place, a V twin - and
// scales each output row by its reciprocal sum. Hand MSL, nothing here ships.
//
//   bin/daslang -jit modules/dasLLAMA/harness/attn_depth_race.das

require math
require strings
require metal/msl_shader
require metal/das_metal_boost
require ?das_metal das_metal
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_prefill

let HEADS = 16
let HS = 256
let KV_MUL = 8
let QD = HEADS * HS
let KVD = (HEADS / KV_MUL) * HS
let MP = 2048
let QOFF = 8192
let NK = QOFF + MP

let LAB_HEAD = ("#include <metal_stdlib>\n#include <MetalPerformancePrimitives/MetalPerformancePrimitives.h>\n" +
    "using namespace metal;\nusing namespace mpp::tensor_ops;\n" +
    "struct AttnArgs \{ uint qd; uint kv_dim; uint head_size; uint kv_mul; uint npos; uint np32; float scale; uint qoff; uint qrows; uint window; float softcap; uint hass; uint uend; uint ulo; \};\n" +
    "typedef tensor<device half, dextents<int32_t, 2>, tensor_inline> HT;\n")

def private qk_dev_msl(mt : int) : string {
    return build_string() $(var w) {
        w |> write(LAB_HEAD)
        w |> write("kernel void lab_qk_dev{mt}(device const half* q [[buffer(0)]], device const half* k [[buffer(1)]],\n")
        w |> write("        device half* att [[buffer(2)]], constant AttnArgs& ka [[buffer(3)]],\n")
        w |> write("        uint3 wg [[threadgroup_position_in_grid]]) \{\n")
        w |> write("    const uint mBase = wg.x * {mt}u;\n    const uint nBase = wg.y * 64u;\n    const uint h = wg.z;\n")
        w |> write("    if (nBase > ka.qoff + mBase + {mt - 1}u) return;\n")
        w |> write("    auto A = HT((device half *)(q + mBase * ka.qd + h * ka.head_size), dextents<int32_t, 2>(int(ka.head_size), {mt}), metal::array<int32_t, 2>\{1, int(ka.qd)\});\n")
        w |> write("    auto W = HT((device half *)(k + nBase * ka.kv_dim + (h / ka.kv_mul) * ka.head_size), dextents<int32_t, 2>(int(ka.head_size), 64), metal::array<int32_t, 2>\{1, int(ka.kv_dim)\});\n")
        w |> write("    constexpr auto d = matmul2d_descriptor({mt}, 64, static_cast<int>(dynamic_extent), false, true, true);\n")
        w |> write("    matmul2d<d, execution_simdgroups<4>> op;\n")
        w |> write("    auto cT = op.get_destination_cooperative_tensor<HT, HT, float>();\n")
        w |> write("    op.run(A, W, cT);\n")
        w |> write("    device half* c = att + (h * ka.qrows + mBase) * ka.np32 + nBase;\n")
        w |> write("    for (uint16_t i = 0; i < cT.get_capacity(); ++i) \{ if (cT.is_valid_element(i)) \{\n")
        w |> write("        auto ids = cT.get_multidimensional_index(i);\n")
        w |> write("        c[uint(ids[1]) * ka.np32 + uint(ids[0])] = half(cT[i]);\n")
        w |> write("    \} \}\n\}\n")
    }
}

//! one row a threadgroup: exp(s - max) over the scores, the row sum beside; columns past the causal end to the AV tile's walk end take zero
let LAB_SMW = (LAB_HEAD +
    "kernel void lab_smw(device half* att [[buffer(0)]], constant AttnArgs& ka [[buffer(1)]], device float* stat [[buffer(2)]],\n" +
    "        uint3 wg [[threadgroup_position_in_grid]], uint lid [[thread_index_in_threadgroup]], uint sgi [[simdgroup_index_in_threadgroup]]) \{\n" +
    "    threadgroup float red[8];\n" +
    "    const uint qi = wg.x;\n    const uint h = wg.y;\n" +
    "    const uint cnt = ka.qoff + qi + 1u;\n" +
    "    const uint zlim = min(((ka.qoff + (qi / 128u) * 128u + 128u + 63u) / 64u) * 64u, ka.np32);\n" +
    "    device half* row = att + (h * ka.qrows + qi) * ka.np32;\n" +
    "    float m = -1.0e30f;\n" +
    "    for (uint j = lid; j < cnt; j += 128u) m = max(m, float(row[j]));\n" +
    "    m = simd_max(m);\n" +
    "    if (simd_is_first()) red[sgi] = m;\n" +
    "    threadgroup_barrier(mem_flags::mem_threadgroup);\n" +
    "    m = max(max(red[0], red[1]), max(red[2], red[3]));\n" +
    "    float sum = 0.0f;\n" +
    "    for (uint j = lid; j < zlim; j += 128u) \{\n" +
    "        const float e = j < cnt ? exp(float(row[j]) - m) : 0.0f;\n" +
    "        sum += e;\n" +
    "        row[j] = half(e);\n" +
    "    \}\n" +
    "    sum = simd_sum(sum);\n" +
    "    if (simd_is_first()) red[4u + sgi] = sum;\n" +
    "    threadgroup_barrier(mem_flags::mem_threadgroup);\n" +
    "    if (lid == 0u) \{\n" +
    "        stat[(h * ka.qrows + qi) * 2u] = m;\n" +
    "        stat[(h * ka.qrows + qi) * 2u + 1u] = 1.0f / (red[4] + red[5] + red[6] + red[7]);\n" +
    "    \}\n\}\n")

def private av_dev_msl(mt : int) : string {
    return build_string() $(var w) {
        w |> write(LAB_HEAD)
        w |> write("kernel void lab_av_dev{mt}(device const half* p [[buffer(0)]], device const half* v [[buffer(1)]],\n")
        w |> write("        device float* xb [[buffer(2)]], constant AttnArgs& ka [[buffer(3)]], device const float* stat [[buffer(4)]],\n")
        w |> write("        uint3 wg [[threadgroup_position_in_grid]]) \{\n")
        w |> write("    const uint mBase = wg.x * {mt}u;\n    const uint vBase = wg.y * 64u;\n    const uint h = wg.z;\n")
        w |> write("    const uint klim = min(((ka.qoff + mBase + {mt}u + 63u) / 64u) * 64u, ka.np32);\n")
        w |> write("    auto A = HT((device half *)(p + (h * ka.qrows + mBase) * ka.np32), dextents<int32_t, 2>(int(klim), {mt}), metal::array<int32_t, 2>\{1, int(ka.np32)\});\n")
        w |> write("    auto B = HT((device half *)(v + (h / ka.kv_mul) * ka.head_size + vBase), dextents<int32_t, 2>(64, int(klim)), metal::array<int32_t, 2>\{1, int(ka.kv_dim)\});\n")
        w |> write("    constexpr auto d = matmul2d_descriptor({mt}, 64, static_cast<int>(dynamic_extent), false, false, true);\n")
        w |> write("    matmul2d<d, execution_simdgroups<4>> op;\n")
        w |> write("    auto cT = op.get_destination_cooperative_tensor<HT, HT, float>();\n")
        w |> write("    op.run(A, B, cT);\n")
        w |> write("    for (uint16_t i = 0; i < cT.get_capacity(); ++i) \{ if (cT.is_valid_element(i)) \{\n")
        w |> write("        auto ids = cT.get_multidimensional_index(i);\n")
        w |> write("        cT[i] *= stat[(h * ka.qrows + mBase + uint(ids[1])) * 2u + 1u];\n")
        w |> write("    \} \}\n")
        w |> write("    auto C = tensor<device float, dextents<int32_t, 2>, tensor_inline>(xb + mBase * ka.qd + h * ka.head_size + vBase, dextents<int32_t, 2>(64, {mt}), metal::array<int32_t, 2>\{1, int(ka.qd)\});\n")
        w |> write("    cT.store(C);\n\}\n")
    }
}

def private fill_f32(count : int; mul, add : int) : array<float> {
    var xa : array<float>
    xa |> resize(count)
    var st = uint(add) * 2654435761u + 12345u
    for (i in range(count)) {
        st = st * 1664525u + 1013904223u + uint(mul)
        xa[i] = (float(int((st >> 9u) & 0xffffu)) / 65536.0 - 0.5) * 0.5
    }
    return <- xa
}

//! the attention output at a handful of (row, head, column) points from the f32 operands in
//! double, and each buffer's largest distance from it
def private ref_report(qf, kf, vf : array<float>; scale : float; bufs : array<tuple<name : string; buf : MetalBuffer?>>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var worst <- [for (_b in bufs); 0.0lf]
        var mag = 0.0lf
        var sc : array<double>
        sc |> resize(NK)
        for (r in [0, 777, MP - 1]) {
            for (h in [0, 9, HEADS - 1]) {
                let cnt = QOFF + r + 1
                let kvo = (h / KV_MUL) * HS
                var mx = -1.0e300lf
                for (j in range(cnt)) {
                    var dot = 0.0lf
                    for (d in range(HS)) {
                        dot += double(qf[r * QD + h * HS + d]) * double(kf[j * KVD + kvo + d])
                    }
                    sc[j] = dot * double(scale)
                    mx = max(mx, sc[j])
                }
                var tot = 0.0lf
                for (j in range(cnt)) {
                    sc[j] = exp(sc[j] - mx)
                    tot += sc[j]
                }
                for (d in [0, 100, HS - 1]) {
                    var want = 0.0lf
                    for (j in range(cnt)) {
                        want += sc[j] * double(vf[j * KVD + kvo + d])
                    }
                    want /= tot
                    mag = max(mag, abs(want))
                    for (bi, b in count(), bufs) {
                        var got = 0.0
                        unsafe {
                            got = reinterpret<float?>(metal_buffer_contents(b.buf))[r * QD + h * HS + d]
                        }
                        worst[bi] = max(worst[bi], abs(double(got) - want))
                    }
                }
            }
        }
        for (b, w in bufs, worst) {
            print("reference (f32 operands, double): {b.name} off by at most {w} of max {mag}\n")
        }
        delete worst
        delete sc
    }
}

def private half_of(src : array<float>; scale : float) : array<float16> {
    var xa : array<float16>
    xa |> resize(length(src))
    for (d, s in xa, src) {
        d = float16(s * scale)
    }
    return <- xa
}

[export]
def main {   // nolint:STYLE038 — one-shot race harness; buffers + PSOs stay live to the release tail
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var dev = metal_create_system_default_device()
        var queue = metal_new_command_queue(dev)
        var err = ""
        var qk_p = pipeline_from_source(dev, MetalAttnQKMmTH_metal_attn_qk_mm_t_msl, MetalAttnQKMmTH_metal_attn_qk_mm_t_msl_entry, MetalAttnQKMmTH_metal_attn_qk_mm_t_msl_fastmath, err)
        var rs_p = pipeline_from_source(dev, metal_attn_rowstat_msl, metal_attn_rowstat_msl_entry, metal_attn_rowstat_msl_fastmath, err)
        var av_p = pipeline_from_source(dev, MetalAttnAVMmP_metal_attn_av_mm_msl, MetalAttnAVMmP_metal_attn_av_mm_msl_entry, MetalAttnAVMmP_metal_attn_av_mm_msl_fastmath, err)
        var qk_d32 = pipeline_from_source(dev, qk_dev_msl(32), "lab_qk_dev32", true, err)
        var qk_d128 = pipeline_from_source(dev, qk_dev_msl(128), "lab_qk_dev128", true, err)
        var smw = pipeline_from_source(dev, LAB_SMW, "lab_smw", true, err)
        var av_d32 = pipeline_from_source(dev, av_dev_msl(32), "lab_av_dev32", true, err)
        var av_d128 = pipeline_from_source(dev, av_dev_msl(128), "lab_av_dev128", true, err)
        if (qk_p == null || rs_p == null || av_p == null || qk_d32 == null || qk_d128 == null || smw == null || av_d32 == null || av_d128 == null) {
            print("pso: {err}\n")
            return
        }
        let scale = 1.0 / sqrt(float(HS))
        //! `-- <amp>` widens Q so a row's scores spread the way a trained head's do (default 1: a flat row)
        let args <- get_command_line_arguments()
        let amp = !empty(args) && to_float(args[length(args) - 1]) > 0.0 ? to_float(args[length(args) - 1]) : 1.0
        var qf <- fill_f32(MP * QD, 3, 1)
        for (x in qf) {
            x *= amp
        }
        print("Q amplitude x{amp}\n")
        var kf <- fill_f32(NK * KVD, 5, 2)
        var vf <- fill_f32(NK * KVD, 7, 3)
        var qh <- half_of(qf, 1.0)
        var kh <- half_of(kf, scale)
        var vh <- half_of(vf, 1.0)
        var bqh = race_buf(dev, uint64(MP * QD * 2), unsafe(addr<void?>(qh[0])))
        var bk = race_buf(dev, uint64(NK * KVD * 4), unsafe(addr<void?>(kf[0])))
        var bkh = race_buf(dev, uint64(NK * KVD * 2), unsafe(addr<void?>(kh[0])))
        var bv = race_buf(dev, uint64(NK * KVD * 4), unsafe(addr<void?>(vf[0])))
        var bvh = race_buf(dev, uint64(NK * KVD * 2), unsafe(addr<void?>(vh[0])))
        let slab = uint64(HEADS) * uint64(MP) * uint64(NK) * 2ul
        var batt_a = race_buf(dev, slab, null)
        var batt_b = race_buf(dev, slab, null)
        var bstat_a = race_buf(dev, uint64(HEADS * MP * 2 * 4), null)
        var bstat_b = race_buf(dev, uint64(HEADS * MP * 2 * 4), null)
        var bxb_a = race_buf(dev, uint64(MP * QD * 4), null)
        var bxb_b = race_buf(dev, uint64(MP * QD * 4), null)
        var bsink = race_buf(dev, uint64(HEADS * 4), null)
        var ka = AttnArgs(qd = uint(QD), kv_dim = uint(KVD), head_size = uint(HS), kv_mul = uint(KV_MUL),
            npos = uint(NK), np32 = uint(NK), scale = scale, qoff = uint(QOFF), qrows = uint(MP),
            window = 0u, softcap = 0.0, hass = 0u)
        let tg = uint3(128u, 1u, 1u)
        let enc_qk_p = $(enc : MetalComputeEncoder?) {
            kn_pipeline(enc, qk_p)
            kn_tgmem(enc, MetalAttnQKMmTH_metal_attn_qk_mm_t_msl_tgmem, 0)
            kn_buffer(enc, bqh, 0ul, 0)
            kn_buffer(enc, bk, 0ul, 1)
            kn_buffer(enc, batt_a, 0ul, 2)
            kn_kargs(enc, ka, 3)
            kn_dispatch(enc, uint3(uint(MP / 32), uint(NK / 64), uint(HEADS)), tg)
        }
        let enc_rs_p = $(enc : MetalComputeEncoder?) {
            kn_pipeline(enc, rs_p)
            kn_tgmem(enc, metal_attn_rowstat_msl_tgmem, 0)
            kn_buffer(enc, batt_a, 0ul, 0)
            kn_kargs(enc, ka, 1)
            kn_buffer(enc, bstat_a, 0ul, 2)
            kn_buffer(enc, bsink, 0ul, 20)
            kn_dispatch(enc, uint3(uint(MP), uint(HEADS), 1u), tg)
        }
        let enc_av_p = $(enc : MetalComputeEncoder?) {
            kn_pipeline(enc, av_p)
            kn_tgmem(enc, MetalAttnAVMmP_metal_attn_av_mm_msl_tgmem, 0)
            kn_buffer(enc, batt_a, 0ul, 0)
            kn_buffer(enc, bv, 0ul, 1)
            kn_buffer(enc, bxb_a, 0ul, 2)
            kn_kargs(enc, ka, 3)
            kn_buffer(enc, bstat_a, 0ul, 4)
            kn_dispatch(enc, uint3(uint(MP / 32), uint(HS / 64), uint(HEADS)), tg)
        }
        let enc_qk_d = $(enc : MetalComputeEncoder?; mt : int) {
            kn_pipeline(enc, mt == 32 ? qk_d32 : qk_d128)
            kn_buffer(enc, bqh, 0ul, 0)
            kn_buffer(enc, bkh, 0ul, 1)
            kn_buffer(enc, batt_b, 0ul, 2)
            kn_kargs(enc, ka, 3)
            kn_dispatch(enc, uint3(uint(MP / mt), uint(NK / 64), uint(HEADS)), tg)
        }
        let enc_smw = $(enc : MetalComputeEncoder?) {
            kn_pipeline(enc, smw)
            kn_buffer(enc, batt_b, 0ul, 0)
            kn_kargs(enc, ka, 1)
            kn_buffer(enc, bstat_b, 0ul, 2)
            kn_dispatch(enc, uint3(uint(MP), uint(HEADS), 1u), tg)
        }
        let enc_av_d = $(enc : MetalComputeEncoder?; mt : int) {
            kn_pipeline(enc, mt == 32 ? av_d32 : av_d128)
            kn_buffer(enc, batt_b, 0ul, 0)
            kn_buffer(enc, bvh, 0ul, 1)
            kn_buffer(enc, bxb_b, 0ul, 2)
            kn_kargs(enc, ka, 3)
            kn_buffer(enc, bstat_b, 0ul, 4)
            kn_dispatch(enc, uint3(uint(MP / mt), uint(HS / 64), uint(HEADS)), tg)
        }
        let gfma = double(MP) * double(NK) * double(HS) * double(HEADS) / 1.0e9lf
        print("attention at depth: {MP} queries at {QOFF}, {NK} keys, {HEADS} heads of {HS} over {HEADS / KV_MUL} KV heads - {gfma} G multiply-adds a GEMM\n")
        for (mt in [32, 128]) {
            var t_a = -1.0lf
            var t_b = -1.0lf
            race_pair_ms(queue, 3, t_a, t_b, $(enc) {
                invoke(enc_qk_p, enc)
            }, $(enc) {
                invoke(enc_qk_d, enc, mt)
            })
            var note = ""
            let ok = race_envelope_ok_h(batt_a, batt_b, MP * NK, note)   //! head 0's rows
            print("qk     prod={t_a}ms dev{mt}={t_b}ms x{t_a / t_b} {ok ? "scores-agree" : note}\n")
        }
        {
            var t_a = -1.0lf
            var t_b = -1.0lf
            //! each rep refills the lab slab with raw scores before the pass that overwrites them
            race_pair_ms(queue, 3, t_a, t_b, $(enc) {
                invoke(enc_qk_p, enc)
                invoke(enc_rs_p, enc)
            }, $(enc) {
                invoke(enc_qk_d, enc, 128)
                invoke(enc_smw, enc)
            })
            print("qk+sm  prod={t_a}ms dev={t_b}ms x{t_a / t_b}\n")
        }
        for (mt in [32, 128]) {
            var t_a = -1.0lf
            var t_b = -1.0lf
            race_pair_ms(queue, 3, t_a, t_b, $(enc) {
                invoke(enc_qk_p, enc)
                invoke(enc_rs_p, enc)
                invoke(enc_av_p, enc)
            }, $(enc) {
                invoke(enc_qk_d, enc, 128)
                invoke(enc_smw, enc)
                invoke(enc_av_d, enc, mt)
            })
            var maxref = 0.0lf
            var maxdiff = 0.0lf
            unsafe {
                let pa = reinterpret<float?>(metal_buffer_contents(bxb_a))
                let pb = reinterpret<float?>(metal_buffer_contents(bxb_b))
                for (i in range(MP * QD)) {
                    maxref = max(maxref, abs(double(pa[i])))
                    maxdiff = max(maxdiff, abs(double(pa[i]) - double(pb[i])))
                }
            }
            print("trio   prod={t_a}ms dev(qk128, av{mt})={t_b}ms x{t_a / t_b} | output max difference {maxdiff} of max {maxref}\n")
        }
        //! the shipped device pair, its own K/V twin pass in the window
        var e_tw = pipeline_from_source(dev, metal_kv_twin_msl, metal_kv_twin_msl_entry, metal_kv_twin_msl_fastmath, err)
        var e_qk = pipeline_from_source(dev, MetalAttnQKDev128_metal_attn_qk_dev_msl, MetalAttnQKDev128_metal_attn_qk_dev_msl_entry, MetalAttnQKDev128_metal_attn_qk_dev_msl_fastmath, err)
        var e_px = pipeline_from_source(dev, metal_attn_pexp_msl, metal_attn_pexp_msl_entry, metal_attn_pexp_msl_fastmath, err)
        var e_av = pipeline_from_source(dev, MetalAttnAVDev128_metal_attn_av_dev_msl, MetalAttnAVDev128_metal_attn_av_dev_msl_entry, MetalAttnAVDev128_metal_attn_av_dev_msl_fastmath, err)
        if (e_tw == null || e_qk == null || e_px == null || e_av == null) {
            print("shipped pso: {err}\n")
            return
        }
        var tka = KvTwinArgs(total = uint(NK * KVD), scale = scale)
        for (stage in range(4)) {
            var t_a = -1.0lf
            var t_b = -1.0lf
            race_pair_ms(queue, 3, t_a, t_b, $(enc) {
                invoke(enc_qk_p, enc)
                if (stage >= 1) {
                    invoke(enc_rs_p, enc)
                }
                if (stage >= 2) {
                    invoke(enc_av_p, enc)
                }
            }, $(enc) {
                if (stage == 3) {
                    kn_pipeline(enc, e_tw)
                    kn_buffer(enc, bk, 0ul, 0)
                    kn_buffer(enc, bv, 0ul, 1)
                    kn_buffer(enc, bkh, 0ul, 2)
                    kn_buffer(enc, bvh, 0ul, 3)
                    kn_kargs(enc, tka, 4)
                    kn_dispatch(enc, uint3(uint(NK * KVD / 256), 1u, 1u), uint3(256u, 1u, 1u))
                }
                kn_pipeline(enc, e_qk)
                kn_buffer(enc, bqh, 0ul, 0)
                kn_buffer(enc, bkh, 0ul, 1)
                kn_buffer(enc, batt_b, 0ul, 2)
                kn_kargs(enc, ka, 3)
                kn_dispatch(enc, uint3(uint(MP / 128), uint(NK / 64), uint(HEADS)), tg)
                if (stage >= 1) {
                    kn_pipeline(enc, e_px)
                    kn_tgmem(enc, metal_attn_pexp_msl_tgmem, 0)
                    kn_buffer(enc, batt_b, 0ul, 0)
                    kn_kargs(enc, ka, 1)
                    kn_buffer(enc, bstat_b, 0ul, 2)
                    kn_buffer(enc, bsink, 0ul, 20)
                    kn_dispatch(enc, uint3(uint(MP), uint(HEADS), 1u), tg)
                }
                if (stage >= 2) {
                    kn_pipeline(enc, e_av)
                    kn_buffer(enc, batt_b, 0ul, 0)
                    kn_buffer(enc, bvh, 0ul, 1)
                    kn_buffer(enc, bxb_b, 0ul, 2)
                    kn_kargs(enc, ka, 3)
                    kn_buffer(enc, bstat_b, 0ul, 4)
                    kn_dispatch(enc, uint3(uint(MP / 128), uint(HS / 64), uint(HEADS)), tg)
                }
            })
            print("shipped stage {stage} (0 qk, 1 +softmax, 2 +av, 3 +twin pass) prod={t_a}ms dev={t_b}ms x{t_a / t_b}\n")
        }
        ref_report(qf, kf, vf, scale, [(name = "staged trio", buf = bxb_a), (name = "device pair", buf = bxb_b)])
        metal_release(e_tw)
        metal_release(e_qk)
        metal_release(e_px)
        metal_release(e_av)
        delete qf
        delete kf
        delete vf
        delete qh
        delete kh
        delete vh
        for (b in [bqh, bk, bkh, bv, bvh, batt_a, batt_b, bstat_a, bstat_b, bxb_a, bxb_b, bsink]) {
            metal_release(b)
        }
        for (p in [qk_p, rs_p, av_p, qk_d32, qk_d128, smw, av_d32, av_d128]) {
            metal_release(p)
        }
    } else {
        print("attn_depth_race: no das_metal in this build\n")
    }
}
