options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _function_length = 0        // the lab runner is one flat fixture / arm / round ladder
options _cyclomatic_complexity = 0
options _dasllama_internal = true

require metal/msl_shader
require ?das_metal metal/das_metal_boost
require dasllama/dasllama_metal_prefill   // the production MoE tensor scaffolds, their builders and MoeMmArgs
require dasllama/dasllama_metal_kernels   // the scaffolds' decode helpers the lab stamps instantiate here
require dasllama/dasllama_metal_common
require ?das_metal ../../tests/_metal_kernel_common.das   // kq plane fills, the pso_stamp / kernel_of macros, buf_download
require math
require strings

// The routed expert tile lab: what one tile of the prefill's MoE tensor stamps costs, and where. The
// house's short reply is a ~375-token window over 256 experts at top-8, twelve live rows an expert in
// a 32-row tile, and the window's expert GEMMs read flat in live rows. Four arms per format on the
// production builder's binds and grid, every dispatch its own output buffer:
//   prod       the shipped TH stamp - ADAPT on, the tensor op at 8 or 16 rows where the expert holds that few
//              (the baseline; checked against a CPU reference on a sampled region)
//   tile32     the same stamp with ADAPT off: the 32-row op whatever the expert holds (the form it replaced)
//   ko_stage   ATTRIBUTION SWEEP: the stamp with no W staging - the tensor op's share of the tile (timing-only)
//   ko_tensor  ATTRIBUTION SWEEP: the stamp with no tensor op - the staging's share of the tile (timing-only)
//   base       k4 only: the simdgroup-matrix form the K-quant sites run where the dense `kq_mulmm_k4` crown is
//              absent (f32 x, the bucket map) - the tile a box like the M1 Max ships today, against the twin
// Three row patterns an expert: 4 (a 128-token window), 8..16 (the house's 375), 32 (whole tiles), on
// iq4xs (the split scaffold) and k4 (the kq scaffold), 128 experts so the planes sit past the last-level
// cache. Arms alternate one timed round each, best of ROUNDS rounds after a burn round per arm.
//
//   bin/daslang -jit modules/dasLLAMA/benchmarks/matmul/bench_metal_moe_tile_lab.das

class LabMoeIq4xsTile32 : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float16
    override ADAPT = false
}

class LabMoeIq4xsKoStage : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float16
    override KO_STAGE = true
}

class LabMoeIq4xsKoTensor : MetalMoeMulMmIq4xsSplitT {
    typedef XT = float16
    override KO_TENSOR = true
}

class LabMoeK4Tile32 : MetalMoeMulMmK45TensorT {
    typedef XT = float16
    override ADAPT = false
}

class LabMoeK4KoStage : MetalMoeMulMmK45TensorT {
    typedef XT = float16
    override KO_STAGE = true
}

class LabMoeK4KoTensor : MetalMoeMulMmK45TensorT {
    typedef XT = float16
    override KO_TENSOR = true
}

let NE = 128        //! experts: 128 x 1024 x 2048 planes = 142 MB iq4xs, past the SLC
let NK = 8
let KDIM = 2048
let NDIM = 1024     //! the gate plane's width (the down plane is the mirror shape)
let NPOS = 384      //! the window the grid is launched for: 12 tiles an expert, the house's shape
let NDISP = 10      //! dispatches a timed encoder, each into its own y
let ROUNDS = 3
let REF_EXPERTS = 2 //! the CPU reference's sampled region: the first experts' live rows over the first 64 columns
let REF_COLS = 64

struct Pattern {
    name : string
    cnt : array<uint>
    basep : array<uint>
    mpad : int
}

def pattern(name : string; rows : block<(e : int) : int>) : Pattern {
    var p : Pattern
    p.name = name
    p.cnt |> resize(NE)
    p.basep |> resize(NE + 1)
    var acc = 0u
    for (e in range(NE)) {
        p.cnt[e] = uint(invoke(rows, e))
        p.basep[e] = acc
        acc += (p.cnt[e] + 31u) / 32u * 32u
    }
    p.basep[NE] = acc
    p.mpad = int(acc)
    return <- p
}

//! the CPU reference over the sampled region: y[r][n] for the first REF_EXPERTS experts' live rows and the first REF_COLS columns
def ref_region(fmt : int; p : Pattern; xf : array<float16>; kq, ks : array<uint8>) : array<float> {
    let nb = KDIM / 256
    let nsb = NE * NDIM * nb
    var ref, wrow : array<float>
    wrow |> resize(KDIM)
    var live = 0
    for (e in range(REF_EXPERTS)) {
        live += int(p.cnt[e])
    }
    ref |> reserve(REF_COLS * live)
    for (e in range(REF_EXPERTS)) {
        for (n in range(REF_COLS)) {
            kq_row_ref(fmt, (e * NDIM + n) * nb, nb, nsb, kq, ks, wrow)
            for (r in range(int(p.cnt[e]))) {
                let row = int(p.basep[e]) + r
                var acc = 0.0lf
                for (k in range(KDIM)) {
                    acc += double(float(xf[row * KDIM + k])) * double(wrow[k])
                }
                ref |> push(float(acc))
            }
        }
    }
    return <- ref
}

//! the same region out of a y buffer, in the reference's order
def gpu_region(buf; p : Pattern) : array<float> {
    var inscope y <- buf_download(buf, p.mpad * NDIM, type<float>)
    var got : array<float>
    var live = 0
    for (e in range(REF_EXPERTS)) {
        live += int(p.cnt[e])
    }
    got |> reserve(REF_COLS * live)
    for (e in range(REF_EXPERTS)) {
        for (n in range(REF_COLS)) {
            for (r in range(int(p.cnt[e]))) {
                got |> push(y[(int(p.basep[e]) + r) * NDIM + n])
            }
        }
    }
    return <- got
}

def maxrel(a, b : array<float>) : float {
    var m = 0.0
    var scale = 0.0
    for (x, y in a, b) {
        m = max(m, abs(x - y))
        scale = max(scale, abs(y))
    }
    return scale > 0.0 ? m / scale : m
}

//! the live rows of every expert out of a y buffer, for the adapt arm's compare against prod
def live_rows(buf; p : Pattern) : array<float> {
    var inscope y <- buf_download(buf, p.mpad * NDIM, type<float>)
    var got : array<float>
    var live = 0
    for (c in p.cnt) {
        live += int(c)
    }
    got |> reserve(live * NDIM)
    for (e in range(NE)) {
        for (r in range(int(p.cnt[e]))) {
            let row = (int(p.basep[e]) + r) * NDIM
            for (n in range(NDIM)) {
                got |> push(y[row + n])
            }
        }
    }
    return <- got
}

[export]
def main {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var dev = metal_create_system_default_device()
        if (dev == null) {
            panic("no Metal device - no row")
        }
        var queue = metal_new_command_queue(dev)
        var inscope patterns <- [ <- pattern("rows4", $(_e) => 4), <- pattern("rows8-16", $(e) => 8 + (e * 7) % 9), <- pattern("rows32", $(_e) => 32)]
        var mpad_max = 0
        for (p in patterns) {
            mpad_max = max(mpad_max, p.mpad)
        }
        var xf : array<float16>
        xf |> resize(mpad_max * KDIM)
        for (i in range(mpad_max * KDIM)) {
            xf[i] = float16(0.03 * float(i % 61) - 0.9)
        }
        let x32 <- [for (h in xf); float(h)]   //! the base form reads x as f32: the same values the twins read as halves
        let bkt_id <- [for (i in range(mpad_max)); uint(i)]   //! identity bucket map: gather = 0, the rows sit contiguous an expert
        let nb = KDIM / 256
        let nsb = NE * NDIM * nb
        for (fmt in [44, 4]) {
            let fname = fmt == 44 ? "iq4xs" : "k4"
            var kq, ks : array<uint8>
            kq |> reserve(nsb * 128)   //! the planes pass the 64 MB unreserved-growth guard: a superblock's 256 nibbles, and its scales
            ks |> reserve(nsb * 32)
            kq_fill_planes(fmt, nsb, kq, ks)
            let wbytes = double(long_length(kq) + long_length(ks))
            print("\n== {fname}: {NE} experts x {NDIM} x {KDIM} ({wbytes / 1048576.0lf} MB of planes), {NDISP} dispatches an encoder, best of {ROUNDS}\n")
            with_metal_cell(dev, queue) $(var mc : MetalCell) {
                var err : string
                let bkq = mc |> in_plain(kq)
                let bks = mc |> in_plain(ks)
                let bxf = mc |> in_plain(xf)
                let bx32 = mc |> in_plain(x32)
                let bbkt = mc |> in_plain(bkt_id)
                let bkd = mc |> in_plain([uint(KDIM)])
                let bnd = mc |> in_plain([uint(NDIM)])
                let doff = kq_split_doff(fmt, nsb)
                var inscope names <- fmt == 44 ? ["prod", "tile32", "ko_stage", "ko_tensor"] : ["prod", "tile32", "ko_stage", "ko_tensor", "base"]
                let kiq = kernel_of(@@pf_enc_moe_mm_iq4xs_th_c)
                let kk4 = kernel_of(@@pf_enc_moe_mm_k4_th_c)
                let kb4 = kernel_of(@@pf_enc_moe_mm_k4_c)
                var psos <- fmt == 44 ? [
                    invoke(kiq.make, dev, err),
                    pso_stamp(dev, LabMoeIq4xsTile32_metal_moe_mulmm_iq4xs_t_msl, err),
                    pso_stamp(dev, LabMoeIq4xsKoStage_metal_moe_mulmm_iq4xs_t_msl, err),
                    pso_stamp(dev, LabMoeIq4xsKoTensor_metal_moe_mulmm_iq4xs_t_msl, err)
                ] : [
                    invoke(kk4.make, dev, err),
                    pso_stamp(dev, LabMoeK4Tile32_metal_moe_mulmm_k45_t_msl, err),
                    pso_stamp(dev, LabMoeK4KoStage_metal_moe_mulmm_k45_t_msl, err),
                    pso_stamp(dev, LabMoeK4KoTensor_metal_moe_mulmm_k45_t_msl, err),
                    invoke(kb4.make, dev, err)
                ]
                let BASE_ARM = 4
                for (ps, nm in psos, names) {
                    if (ps == null) {
                        panic("{fname} {nm}: pipeline compile failed: {err} - no row")
                    }
                }
                for (p in patterns) {
                    let bcnt = mc |> in_plain(p.cnt)
                    let bbase = mc |> in_plain(p.basep)
                    var inscope ys <- [for (_i in range(NDISP)); mc |> out_words(p.mpad * NDIM, 0.0)]
                    var ka = MoeMmArgs(kdim = uint(KDIM), ndim = uint(NDIM), eplane = uint(KDIM * NDIM / 256), nk = uint(NK), gather = 0u, hasb = 0u)
                    var inscope best : array<double>
                    best |> resize(length(psos))
                    for (i in range(length(best))) {
                        best[i] = 1.0e9lf
                    }
                    for (round in range(ROUNDS + 1)) {   // round 0 is the burn
                        for (ai, ps in count(), psos) {
                            var gpu_ms = 0.0lf
                            let ok = with_compute_encoder_timed(queue, err, gpu_ms) $(enc : MetalComputeEncoder?) {
                                for (d in range(NDISP)) {
                                    let by = mc.buf[ys[d]]
                                    if (fmt == 44) {
                                        invoke(kiq.enc, enc, ps, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bxf], 0ul, by, 0ul,
                                            mc.buf[bkd], mc.buf[bnd], mc.buf[bcnt], mc.buf[bbase], int64(p.mpad), int64(NDIM), int64(NE), int64(NPOS / 32))
                                    } elif (ai == BASE_ARM) {
                                        invoke(kb4.enc, enc, ps, mc.buf[bks], doff, mc.buf[bks], mc.buf[bkq], 0ul, mc.buf[bx32], by, ka,
                                            mc.buf[bcnt], mc.buf[bbase], mc.buf[bbkt], int64(NDIM), int64(NPOS), int64(NE))
                                    } else {
                                        invoke(kk4.enc, enc, ps, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bxf], by, ka,
                                            mc.buf[bcnt], mc.buf[bbase], int64(NDIM), int64(NPOS), int64(NE))
                                    }
                                }
                            }
                            if (!ok) {
                                panic("{fname} {p.name} {names[ai]}: GPU error {err} - no row")
                            }
                            if (round > 0) {
                                best[ai] = min(best[ai], gpu_ms / double(NDISP))
                            }
                        }
                    }
                    // the compares read the last dispatch's y of each arm: prod against the CPU reference on the
                    // sampled region, adapt against prod over every live row
                    var inscope ref <- ref_region(fmt, p, xf, kq, ks)
                    var prod_y : int = -1
                    var adapt_y : int = -1
                    var base_y : int = -1
                    for (ai, ps in count(), psos) {
                        var gpu_ms = 0.0lf
                        let yi = mc |> out_words(p.mpad * NDIM, 0.0)
                        with_compute_encoder_timed(queue, err, gpu_ms) $(enc : MetalComputeEncoder?) {
                            if (fmt == 44) {
                                invoke(kiq.enc, enc, ps, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bxf], 0ul, mc.buf[yi], 0ul,
                                    mc.buf[bkd], mc.buf[bnd], mc.buf[bcnt], mc.buf[bbase], int64(p.mpad), int64(NDIM), int64(NE), int64(NPOS / 32))
                            } elif (ai == BASE_ARM) {
                                invoke(kb4.enc, enc, ps, mc.buf[bks], doff, mc.buf[bks], mc.buf[bkq], 0ul, mc.buf[bx32], mc.buf[yi], ka,
                                    mc.buf[bcnt], mc.buf[bbase], mc.buf[bbkt], int64(NDIM), int64(NPOS), int64(NE))
                            } else {
                                invoke(kk4.enc, enc, ps, mc.buf[bks], doff, mc.buf[bks], 0ul, mc.buf[bkq], 0ul, mc.buf[bxf], mc.buf[yi], ka,
                                    mc.buf[bcnt], mc.buf[bbase], int64(NDIM), int64(NPOS), int64(NE))
                            }
                        }
                        if (ai == 0) {
                            prod_y = yi
                        } elif (ai == 1) {
                            adapt_y = yi
                        } elif (ai == BASE_ARM) {
                            base_y = yi
                        }
                    }
                    var inscope prod_region <- gpu_region(mc.buf[prod_y], p)
                    let prod_rel = maxrel(prod_region, ref)
                    var inscope prod_live <- live_rows(mc.buf[prod_y], p)
                    var inscope adapt_live <- live_rows(mc.buf[adapt_y], p)
                    let adapt_rel = maxrel(adapt_live, prod_live)
                    var base_rel = 0.0
                    if (base_y >= 0) {
                        var inscope base_live <- live_rows(mc.buf[base_y], p)
                        base_rel = maxrel(base_live, prod_live)
                    }
                    var live = 0
                    for (c in p.cnt) {
                        live += int(c)
                    }
                    print("-- {fname} {p.name}: {live} live rows in {p.mpad} padded\n")
                    for (ai, nm in count(), names) {
                        let gbps = wbytes / (best[ai] * 1.0e6lf)
                        var note = "timing-only"
                        if (ai == 0) {
                            note = "vs CPU reference on {length(ref)} sampled elements: max rel {prod_rel} (bound 2e-2 {prod_rel <= 0.02 ? "held" : "MISSED"})"
                        } elif (ai == 1) {
                            note = "vs prod over {length(prod_live)} live elements: max rel {adapt_rel} (bound 1e-3 {adapt_rel <= 1e-3 ? "held" : "MISSED"})"
                        } elif (ai == BASE_ARM) {
                            note = "vs prod over {length(prod_live)} live elements: max rel {base_rel} (f32 x against the twins' halves; bound 2e-2 {base_rel <= 0.02 ? "held" : "MISSED"})"
                        }
                        print("{fname} {p.name} {nm}: {best[ai]} ms a dispatch, {gbps} GB/s of weights, x{best[ai] / best[0]} of prod; {note}\n")
                    }
                }
                for (ps in psos) {
                    metal_release(ps)
                }
                unsafe {
                    delete psos   // handled pipelines, released above: the delete frees the array alone
                }
            }
            delete kq
            delete ks
        }
        metal_release(queue)
        metal_release(dev)
    } else {
        panic("das_metal is not built on this platform - no row")
    }
}
