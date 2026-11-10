options gen2
options _function_length = 0   // one flat gate per kernel arm: fixture build, dispatch, compare (the tier suites' shape)
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require math   // nolint:STYLE030,LINT019 — the vulkan cells' math, unused in the darwin world
require strings
require daslib/enum_trait
require dasllama/dasllama_math
require _compares
require _kernel_oracles   // nolint:STYLE030,LINT019 — the vulkan cells' oracles, unused in the darwin world
require _moe_routing_oracles
require _vk_kq_fixtures
require _kq_fixtures
require ?vulkan _vkd_toy
require ?vulkan _vkd_oracles
require ?vulkan _vk_codec_attn
require ?vulkan vulkan
require ?vulkan vulkan/vulkan_boost
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_env   // g_env_vulkan.coopmat - the resolved-default ladder cell pins and restores it
require ?vulkan dasllama/dasllama_vulkan_classes
require dasllama/dasllama_gpu_math
require ?vulkan dasllama/dasllama_vulkan_prefill
require ?vulkan dasllama/dasllama_vulkan_decode   // da_nsplit - the decode attention's key-split pick the tile-pick cell pins
require ?vulkan dasllama/dasllama_gpu_knee   // vk_split_k_tgs: the tile-pick cell pins the short window's knee
require ?vulkan dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema   // nolint:STYLE029,LINT019 — kq_dev_ssb_i sizes the fixtures' device scale planes inside the vulkan arm; the darwin world reads only its kqformat re-export
require ?vulkan daslib/shader_lingua_franca   // gl_GlobalInvocationID — the CPU replay drives it per lane (only vulkan-guarded arms replay)
require dasllama/dasllama_common
require dasllama/dasllama_arch_qwen35
require daslib/strings_boost
require daslib/math_bits   // nolint:STYLE030,LINT019 — the vulkan cells' NaN sentinel and read-back planes by bit pattern, unused in the darwin world
require daslib/f16_cvt   // nolint:STYLE030,LINT019 — the half conversions of the vulkan cells, unused in the darwin world
require _discrete_cells   // nolint:STYLE030,LINT019 — the f16 s-column case body both homes run, unused in the darwin world

//! The Vulkan tier's GEMM tile families against their CPU oracles, one cell a class family: the f16 GEMMs on the cm2 tile
//! and on KHR fragments, the sdot4 and per-32 batch tiles over every kq leaf format, the coopmat batch variants, the KHR
//! tile and the staged KHR tile over a file's f32 rows, the cm2 l, m, s and e stamps over q8 and every K-quant, IQ, MXFP4
//! and NVFP4 format, the split-k chunks and the reduce, and the resolved coopmat default ladder with the tile and split
//! picks. Skips: every cell without dasVulkan or a Vulkan device; the cm2 arms off a coopmat2 device created in cm2 mode;
//! the KHR arms without KHR cooperative matrices at subgroup 32; the tile-pick cell where DASLLAMA_CM2_TILE or
//! DASLLAMA_CM2_SPLITK forces the pick; a class rail a device declines.

let private TILE_SLACK_ROWS = int(TILE_READ_SLACK)   //! the cm2 fixtures' rows past the last region, the engine's count: a stamp that loads partial columns unclamped (`STILE`) reads the last region's column rounded up to its BN, so the slack holds while every such stamp keeps BN <= slack + 1
let private Q8_CELL_K = 544     //! the q8 cm2 s and e cells' K: 512 + 32, off the 64-deep stamp's k grid, so the tile takes its padded edge path (the l and m cells run K = 576, the fast path; TILE_CELL_K, the per-format cells' K, sits in _vk_kq_fixtures)

let private ARM_CM2_L = 0
let private ARM_CM2_M = 1
let private ARM_CM2_S = 2
let private ARM_KHR = 3
let private ARM_CM2_E = 4
let private ARM_TOY_STRIP = 5   //! the q8 l-tile toy stamp built with the four-wide decode stripped to the scalar callback
let private ARM_TOY_TWIN = 6    //! ... and built as the device builds it

def private cell_arm_name(arm : int) : string {
    if (arm >= ARM_TOY_STRIP) return arm == ARM_TOY_STRIP ? "stripped toy l" : "twin toy l"
    return arm == ARM_CM2_L ? "l" : (arm == ARM_CM2_M ? "m" : (arm == ARM_CM2_S ? "s" : (arm == ARM_KHR ? "khr" : "e")))
}

//! the arm helpers stay generic on purpose - a build without the vulkan module must never infer these bodies
//! 256 is the l stamps' BN default, a column no schedule constant names (the l column is never a schedule piece)
def private cell_arm_rows_per_wg(arm) => arm == ARM_CM2_L || arm == ARM_TOY_STRIP || arm == ARM_TOY_TWIN ? 256 : int(arm == ARM_CM2_S ? SCHED_S_ROWS : SCHED_M_ROWS)

def private cell_arm_tc(arm) => arm == ARM_CM2_E ? CM2_TC_E : int64(cell_arm_rows_per_wg(arm))

def private cell_arm_served(arm) => arm == ARM_KHR ? khr_kq_tile_on() : (g_gpu.coopmat_mode == COOPMAT_CM2 && g_gpu.has_coopmat2)

def private cell_arm_set(arm, fmt : int; bufs; sizes; gbits) {
    if (arm == ARM_KHR) {
        if (!khr_cls_ensure(fmt)) panic("the KHR class rail for format {fmt} must engage on a coopmat device")
        return khr_cls_set(fmt, bufs, sizes, gbits)
    }
    verify(g_gpu.coopmat_mode == COOPMAT_CM2, "a cm2 arm dispatched off cm2 mode - the ladder would hand back the KHR stamp")
    if (arm >= ARM_TOY_STRIP) return arm == ARM_TOY_STRIP ? set_toy_q8_cm2_strip_cls(bufs, sizes, gbits) : set_toy_q8_cm2_twin_cls(bufs, sizes, gbits)
    if (!cm2_cls_ensure(fmt, cell_arm_tc(arm))) panic("the {cell_arm_name(arm)} arm's class rail for format {fmt} must engage in cm2 mode")
    return cm2_cls_set(fmt, cell_arm_tc(arm), bufs, sizes, gbits)
}

def private cell_arm_enc(arm, fmt : int; raw; var h; var s; var pc; groups : int64) {
    if (arm == ARM_KHR) {
        khr_cls_enc(fmt, raw, h, s, pc, groups)
    } elif (arm == ARM_TOY_STRIP) {
        enc_toy_q8_cm2_strip_cls(raw, h, s, pc, groups)
    } elif (arm == ARM_TOY_TWIN) {
        enc_toy_q8_cm2_twin_cls(raw, h, s, pc, groups)
    } else {
        cm2_cls_enc(fmt, cell_arm_tc(arm), raw, h, s, pc, groups)
    }
}

def private tile_arms_all() : array<int> => <- [ARM_CM2_L, ARM_CM2_M, ARM_CM2_S, ARM_KHR, ARM_CM2_E]

//! a tile cell's fixture: the format's packed weight and scale planes moved in, the f16 activation plane built here with the engine's read slack past the two regions
struct TileFixture {
    n, blk_k, rows : int
    wqh, wsh, xfh : array<uint>
}

def private tile_fixture(n, blk_k : int; var wqh, wsh : array<uint>; xf_mul, xf_add : uint) : TileFixture {
    let rows = TILE_CELL_ROWS0 + TILE_CELL_ROWS1
    var f = TileFixture(n = n, blk_k = blk_k, rows = rows)
    f.wqh <- wqh
    f.wsh <- wsh
    f.xfh <- ws_plane((rows + TILE_SLACK_ROWS) * n / 2, xf_mul, xf_add)   // f16 activation pairs
    return <- f
}

//! the tile cells' q8 fmt-0 host planes from two seeds, K = n over both weight planes
def private q8_planes(n : int; wq_seed, ws_seed : uint; var wqh, wsh : array<uint>) => q8_plane(2 * TILE_CELL_D * (n / 32), 1u, wq_seed, 1u, ws_seed, wqh, wsh)

//! a tile cell's arms over one fixture, each the device serves: the schedule at the arm's rows a workgroup with two sentinel map words past it (the dispatch runs over them as the device-written schedules' upper bound does, and a tile that reads one returns before its first store), the y plane under the NaN sentinel, the format's oracle, the bars and their poison control; the cell skips when the device serves none of them; the last arm's oracle plane comes back
def private tile_cell_arms(t : T?; tag : string; fmt : int; var f : TileFixture; arms : array<int>;
                           oracle : function<(wq : array<uint>; wsu : array<uint>; xf : array<uint>; recs : array<uint>; nrec, n, d : int; var y : array<float>) : void>) : array<float> {
    var y_ref : array<float>
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var served = 0
        for (arm in arms) {
            served += cell_arm_served(arm) ? 1 : 0
        }
        if (served == 0) {
            t |> skip("the {tag} tiles need cooperative matrices (cm2 in mode 4 on an NV_coopmat2 device, or KHR coopmat at subgroup 32)")
            return <- y_ref
        }
        let d = TILE_CELL_D
        y_ref |> resize(f.rows * d)
        let tail = 2
        for (arm in arms) {
            continue if (!cell_arm_served(arm))
            var sr <- two_region_sched(TILE_CELL_ROWS0, TILE_CELL_ROWS1, d * (f.n / f.blk_k), cell_arm_rows_per_wg(arm), (d + 127) / 128, tail)
            let wgs = sr.wgs
            var inscope sched <- sr.words
            for (w in range(wgs, wgs + tail)) {
                sched[8 + w] = SCHED_NONE
            }
            invoke(oracle, f.wqh, f.wsh, f.xfh, sched, 2, f.n, d, y_ref)
            with_cell_planes() $(var cp : CellPlanes) {
                let pwq = cp |> in_plain(f.wqh)
                let pws = cp |> in_plain(f.wsh)
                let psc = cp |> in_plain(sched)
                let pxf = cp |> in_plain(f.xfh)
                let py = cp |> out_(f.rows * d)
                let planes = fixed_array(pwq, pws, psc, pxf, py)
                var sc = cell_arm_set(arm, fmt, gather_planes(cp.dev, planes), gather_planes(cp.bytes, planes), gather_planes(cp.hazard_bit, planes))
                var pc = BatchArgs(n = uint(f.n), d = uint(d), map_off = uint(2 * 4))
                cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                    cell_arm_enc(arm, fmt, raw2, h2, sc, pc, int64(wgs + tail))
                }
                check_rows(t, "{tag} {cell_arm_name(arm)}-tile ({g_gpu.decvec_on ? "four-wide twin" : "scalar callback"}) vs the CPU oracle", cp |> plane_run(py), y_ref, cm_bar(y_ref), d)
            }
            delete sched
        }
    }
    return <- y_ref
}

//! one per-format tile cell: the table row's planes through `tile_cell_arms` - the cm2 l/m/s/e tiles and the KHR tile, or s, e and
//! KHR on a per-32 format - against its oracle; a row with a `control` then reads the same planes apart under the sibling codebook
def private fmt_tile_cell(t : T?; fmt : KqFmt) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
            t |> skip("no Vulkan device")
            return
        }
        var rows <- fmt_tile_rows()
        let at = find_index_if(rows) $(r : FmtTile) => r.fmt == fmt
        if (at < 0) panic("format {fmt} has no row in the tile table")
        var wqh, wsuh : array<uint>
        fmt_tile_planes(rows[at], wqh, wsuh)
        var f <- tile_fixture(rows[at].n, rows[at].blk_k, wqh, wsuh, 3u, rows[at].xf_add)
        let arms <- fmt == KqFmt.mx4 ? [ARM_CM2_S, ARM_CM2_E, ARM_KHR] : tile_arms_all()   // the expert-only form has no dense l or m column
        let name = "{fmt}"
        var y_ref <- tile_cell_arms(t, name, int(fmt), f, arms, rows[at].oracle)
        if (!empty(y_ref) && rows[at].control != null) {
            let d = TILE_CELL_D
            let recs <- [0u, 0u, uint(TILE_CELL_ROWS0), 0u, uint(d * (f.n / f.blk_k)), uint(TILE_CELL_ROWS0), uint(TILE_CELL_ROWS1), 0u]
            var y_other : array<float>
            y_other |> resize(f.rows * d)
            invoke(rows[at].control, f.wqh, f.wsh, f.xfh, recs, 2, f.n, d, y_other)
            t |> success(mismatch_rel("", y_other, y_ref, 2e-2, 4e-3 * logits_maxabs(y_ref)) > 0, "the {name} planes read apart under the sibling codebook's oracle (the fixture tells the two forms apart)")
            delete y_other
        }
        delete y_ref
        delete f
        delete rows
    } else {
        t |> skip("dasVulkan not present")
    }
}

//! one q8 fmt-0 tile cell at one arm: K = `k` over the planes `q8_planes` hashes from the two seeds, the f16 activation plane at `xf_mul` / `xf_add`
def private q8_tile_cell(t : T?; arm, k : int; wq_seed, ws_seed, xf_mul, xf_add : uint) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
            t |> skip("no Vulkan device")
            return
        }
        var wqh, wsh : array<uint>
        q8_planes(k, wq_seed, ws_seed, wqh, wsh)
        var f <- tile_fixture(k, 32, wqh, wsh, xf_mul, xf_add)
        var y_ref <- tile_cell_arms(t, "q8", int(KqFmt.q8), f, [arm], @@q8f16_gemm_oracle)
        delete y_ref
        delete f
    } else {
        t |> skip("dasVulkan not present")
    }
}

//! the host planes of an sdot4-feed tile cell: the weight quants and scales, the activation quants and their per-unit scales
struct private Sdot4Planes {
    wqh, wsh, xqh : array<uint>
    xsh : array<float>
    ws_bytes : int64
}

//! the q8-seeded sdot4 planes: `units` weight units of `wq_words` quant words hashed at `i + 3`, `ws_words` scale words at
//! `ws_word(i + 7)` (the device plane `ws_bytes` long), `xunits` activation units of `xq_words` at `hash_word(i * 13 + 1)`,
//! their scales 0.375 + (i % 9) / 16 times `xs_mul`
def private q8_sdot4_planes(units, wq_words, ws_words : int; ws_bytes : int64; xunits, xq_words : int; xs_mul : float) : Sdot4Planes {
    var p = Sdot4Planes(ws_bytes = ws_bytes)
    p.wqh <- hashed_words(units * wq_words, 1u, 3u)
    p.wsh <- ws_plane(ws_words, 1u, 7u)
    p.xqh <- hashed_words(xunits * xq_words, 13u, 1u)
    p.xsh |> resize(xunits)
    for (i in range(xunits)) {
        p.xsh[i] = (0.375 + float(i % 9) * 0.0625) * xs_mul
    }
    return <- p
}

//! an sdot4-feed tile cell's device half over two regions of `cnt0` and `cnt1` rows at `tile` rows a workgroup, `d` weight
//! rows a region at `wblk1` units apart: the planes uploaded (the scale plane bound at `ws_bytes`), the y plane under the
//! sentinel - a row the tile never stores keeps it - `rec` sets the class over [w, ws, schedule, xq, xs, y] and encodes it at the
//! map's workgroups; the rows come back with the schedule
def private sdot4_tile_run(n, d, cnt0, cnt1, tile, wblk1 : int; p : Sdot4Planes; rec) : tuple<y : array<float>; sched : array<uint>> {
    var sr <- two_region_sched(cnt0, cnt1, wblk1, tile, (d + tile - 1) / tile, 0)
    var y : array<float>
    static_if (typeinfo builtin_module_exists(vulkan)) {
        with_cell_planes() $(var cp : CellPlanes) {
            let pwq = cp |> in_plain(p.wqh)
            let pws = cp |> in_plain(p.wsh)
            let psc = cp |> in_plain(sr.words)
            let pxq = cp |> in_plain(p.xqh)
            let pxs = cp |> in_(p.xsh)
            let py = cp |> out_((cnt0 + cnt1) * d)
            let planes = fixed_array(pwq, pws, psc, pxq, pxs, py)
            var sizes = gather_planes(cp.bytes, planes)
            sizes[1] = p.ws_bytes
            cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                invoke(rec, gather_planes(cp.dev, planes), sizes, gather_planes(cp.hazard_bit, planes), raw2, h2, int64(sr.wgs))
            }
            delete y
            y <- cp |> plane_run(py)
        }
    }
    var sched <- sr.words
    return <- (y = y, sched = sched)
}

//! the kernel cells' approx bar with a poison past the expectation's largest magnitude, outside the bar at any scale
let private BAR_APPROX_CTRL = Bar(rel = APPROX_REL, poison_of_max = true, poison_abs = 0.5)

//! the fifteen kq formats with a decode GEMV leaf and a batch tile, the order the family cells walk
let private KQ_LEAF_FMTS = fixed_array(int(KqFmt.k4), int(KqFmt.q40), int(KqFmt.k5), int(KqFmt.k6), int(KqFmt.iq4xs), int(KqFmt.k3), int(KqFmt.iq3s), int(KqFmt.iq3xxs), int(KqFmt.iq4nl), int(KqFmt.k2), int(KqFmt.iq2s), int(KqFmt.iq2xs), int(KqFmt.iq2xxs), int(KqFmt.iq3s4), int(KqFmt.nv4))

//! the family's class rails engage on the device: every leaf's pipeline builds, else the cell skips
def private kq_leaf_rails_engage(ensure : block<(fmt : int) : bool>) : bool {
    for (f in KQ_LEAF_FMTS) {
        if (!invoke(ensure, f)) return false
    }
    return true
}

//! the KHR arm loads and stores whole 16-row fragments, so the cell sizes its x and y planes past `rows` the way the prefill's are
def private f16_gemm_cell(t : T?; tag : string; d, n, rows : int; khr : bool = false) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        {
            let nsplit = 8
            let ybase = 16   // the rows land past a header the kernels must leave alone
            let rows_alloc = khr ? ((rows + 15) / 16) * 16 : rows   //! the KHR tile loads and stores whole 16-row fragments: the x and y planes carry the spare rows, as the prefill's do
            let yn = ybase + rows_alloc * d + (khr ? 16 : 0)   //! a ragged d's last fragment may spill up to 15 elements past the plane on the KHR tile: the spare keeps the probe inside the buffer
            var wh, xh : array<float16>
            wh |> resize(d * n)
            xh |> resize(rows_alloc * n)
            for (i in range(d * n)) {
                wh[i] = float16(float((i * 7) % 61) * 0.02 - 0.6)
            }
            for (i in range(rows_alloc * n)) {
                xh[i] = float16(float((i * 11) % 53) * 0.03 - 0.8)
            }
            var ref : array<float>
            ref |> resize(rows * d)
            for (p in range(rows)) {
                for (e in range(d)) {
                    var s = 0.0
                    for (kk in range(n)) {
                        s += float(xh[p * n + kk]) * float(wh[e * n + kk])
                    }
                    ref[p * d + e] = s
                }
            }
            //! both operands are f16-exact, so every product is exact in f32 and the arms differ by f32 reassociation over K
            //! alone: 2e-5 of the max holds the cell's shapes, the poisoned expectation four bars off
            let bar = Bar(kind = BarKind.rel_of_max, rel = 2e-5, poison = 8e-5, poison_of_max = true)
            with_cell_planes() $(var cp : CellPlanes) {
                let pw = cp |> in_plain(wh)
                let px = cp |> in_plain(xh)
                let yslack = yn - ybase - rows * d
                let y0 = out_(cp, rows * d, ybase, yslack)
                let y1 = khr ? -1 : out_(cp, rows * d, ybase, yslack)
                let ys = fixed_array(y0, y1)
                let pp = khr ? -1 : out_(cp, nsplit * rows * d)   //! the split arm's partial planes: a chunk the GEMM skipped would sum a stale plane into the reduce
                let direct = fixed_array(pw, px, ys[0])
                var sd = f16_gemm_set(khr, gather_planes(cp.dev, direct), gather_planes(cp.bytes, direct), gather_planes(cp.hazard_bit, direct))
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    var pc = F16GemmArgs(n = uint(n), d = uint(d), rows = uint(rows), woff = 0u, ybase = uint(ybase), ystride = uint(d), ksplit = 0u)
                    f16_gemm_enc(khr, raw, h, sd, pc, int64(rows), int64(d))
                    if (!khr) {   // the split arm is the cm2 stamp's alone: the KHR stamp reads no k split
                        var sp = cp |> cls_set(fixed_array(pw, px, pp), @@set_f16_gemm_cm2_cls)
                        var sr = cp |> cls_set(fixed_array(pp, ys[1]), @@set_splitk_reduce_cls)
                        let bk = int(F16G_BK)
                        let ksplit = max(bk, ((n / nsplit + bk - 1) / bk) * bk)   //! the class walks K in F16G_BK steps: a chunk shorter than a step would re-sum the first step, so a short K leaves the tail chunks empty (zero planes) instead
                        var pc2 = F16GemmArgs(n = uint(n), d = uint(d), rows = uint(rows), woff = 0u, ybase = 0u, ystride = uint(d), ksplit = uint(ksplit))
                        enc_f16_gemm_cm2_cls(raw, h, sp, pc2, f16_gemm_wgs(false, int64(rows), int64(d)) * int64(nsplit))
                        splitk_reduce_enc(raw, h, sr, int64(rows * d), int64(nsplit), int64(ybase))
                    }
                }
                let arms = fixed_array(khr ? "khr direct" : "direct", "split-k")
                for (arm in range(khr ? 1 : 2)) {
                    var inscope header <- copy_span(cp |> plane_whole(ys[arm]), 0, ybase)
                    t |> success(outside_run(header, 0, 0, NAN_SENTINEL), "f16 GEMM {tag} {arms[arm]}: the {ybase} words under the base stay the sentinel")
                    check_rows(t, "f16 GEMM {tag} {arms[arm]}: {rows} x {d} over K {n} vs the f32 dot", cp |> plane_run(ys[arm]), ref, bar, d)
                    delete header
                }
            }
            delete wh
            delete xh
            delete ref
        }
    }
}

[test(description = "The small f16 GEMM on the cm2 tile, direct and eight-chunk split-k through the reduce, matches the f32 dot of the same halves within 2e-5 of the max at four shapes, the header under the base left intact")]
def test_vkd_f16_gemm(t0 : T?) {
    t0 |> run("the small f16 GEMM on the cm2 tile (the router logits, the deltanet beta/alpha rows) == the f32 dot, direct and split-k [DASLLAMA_COOPMAT=cm2]") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (g_gpu.coopmat_mode != COOPMAT_CM2 || !ensure_f16_gemm_cm2_cls() || !ensure_splitk_reduce_cls()) {
                t |> skip("no cm2 tile family on this device (the small f16 GEMM rides the cm2 tile alone)")
                return
            }
            f16_gemm_cell(t, "ba-like", 32, 512, 70)
            f16_gemm_cell(t, "router-like", 200, 1024, 130)
            f16_gemm_cell(t, "odd router", 61, 512, 41)   //! rows x d off every 4-multiple: the reduce's element-guarded tail
            f16_gemm_cell(t, "rel plane, ragged d, short K", 2495, 64, 37)   // canary's rel-plane GEMM shape, the cm2 twin of the KHR cell's arm
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the f16 GEMM's s column (`F16GemmCm2S`, the tower GEMM sites `vt_f16_tc` puts on the 32-position column) against the f32 dot
//! of the same halves and bit for bit its m column twin (`F16GemmCm2`), on the case body the CUDA s-column cell runs
[test(description = "The cm2 f16 GEMM s column F16GemmCm2S matches the f32 dot of the same halves and the m column stamp bit for bit, on the case body the CUDA s-column cell shares")]
def test_vkd_f16_gemm_s(t0 : T?) {
    t0 |> run("the f16 GEMM's s column on the cm2 tile == the f32 dot of the same halves, bit for bit the m column [DASLLAMA_COOPMAT=cm2]") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (g_gpu.coopmat_mode != COOPMAT_CM2 || !ensure_f16_gemm_cm2_s_cls() || !ensure_f16_gemm_cm2_cls()) {
                t |> skip("no cm2 tile family on this device (the f16 GEMM's s column rides the cm2 tile alone)")
                return
            }
            with_cell_planes() $(var cp : CellPlanes) {
                f16_gemm_s_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The small f16 GEMM on KHR fragments matches the f32 dot of the same halves within 2e-5 of the max at five shapes, rows and d off the 16-row fragment among them, the header under the base intact")]
def test_vkd_f16_gemm_khr(t0 : T?) {
    t0 |> run("the small f16 GEMM on KHR fragments == the f32 dot, direct (the E-series projection's shapes)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!khr_kq_tile_on() || !ensure_f16_gemm_khr_cls()) {
                t |> skip("device lacks KHR cooperative matrices at subgroup 32")
                return
            }
            f16_gemm_cell(t, "ba-like", 32, 512, 70, true)
            f16_gemm_cell(t, "ple-like", 256, 1536, 77, true)   // E2B's projection geometry over one layer slice, rows off the 16-row fragment
            f16_gemm_cell(t, "one fragment", 16, 64, 16, true)
            f16_gemm_cell(t, "rel plane, ragged d, short K", 2495, 64, 37, true)   // canary's rel-plane GEMM: d = 2 npos - 1 off every fragment, K = the head width
            f16_gemm_cell(t, "rel plane, ragged d, 128 K", 2495, 128, 37, true)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The q8 sdot4 batch tile over two regions with an edge column tile, edge row tiles and a zero-filled K tail matches the q8 GEMM oracle within 2e-3")]
def test_vkd_q8_batch_tile(t0 : T?) {
    t0 |> run("q8 batch tile class kernel == the CPU oracle (edge tiles + K tail)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let n = 96                 // nbb = 3: exercises the zero-filled K tail (nsteps = 1, 8-block pass)
            let nbb = n / 32
            let d = 40                 // wtiles = 2 with a 8-col edge tile
            let cnt0 = 35              // region 0: 2 x-tiles with a 3-row edge
            let cnt1 = 20              // region 1: 1 x-tile
            let totblk = 2 * d * nbb
            var p <- q8_sdot4_planes(totblk, 8, totblk / 2 + 1, int64(totblk) * 2l, (cnt0 + cnt1) * nbb, 8, 1.0)
            var r <- sdot4_tile_run(n, d, cnt0, cnt1, 32, d * nbb, p) $(bufs : uint64[6]; sizes : int64[6]; bits : uint[6]; raw2 : VkCommandBuffer; var h2 : VkHaz; wgs : int64) {
                var sc = set_q8_batch_cls(bufs, sizes, bits)
                var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(2 * 4))
                enc_q8_batch_cls(raw2, h2, sc, pc, wgs)
            }
            var y_ref : array<float>
            y_ref |> resize((cnt0 + cnt1) * d)
            q8_gemm_oracle(p.wqh, p.wsh, p.xqh, p.xsh, r.sched, 2, n, d, y_ref)
            check_rows(t, "q8 batch against the CPU oracle", r.y, y_ref, BAR_APPROX_CTRL, d)
            delete y_ref
            delete r
            delete p
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The sdot4 kq batch tile of all fifteen kq leaf formats over two regions with edge tiles at two superblocks matches the kq GEMM oracle within 2e-3")]
def test_vkd_kq_batch_family(t0 : T?) {
    t0 |> run("kq batch tile class family == the CPU oracle (all 15 formats)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!kq_leaf_rails_engage() <| $(fmt : int) => kq_batch_cls_ensure(fmt, false)) {
                t |> skip("no Vulkan device, or a kq batch class pipeline failed to build")
                return
            }
            let n = 512               // nsb = 2 superblocks
            let nsb = n / 256
            let d = 40                // 2 w-tiles with an 8-col edge
            let cnt0 = 35
            let cnt1 = 20
            let rows = cnt0 + cnt1
            let totsb = 2 * d * nsb   // superblocks across both regions
            for (fmt in KQ_LEAF_FMTS) {
                let sb_words = int(kq_qsb(kq_fmt_of_slot(fmt)) / 4l)
                let wswords = totsb * int(kq_dev_ssb_i(fmt) / 4l)
                var p = Sdot4Planes(ws_bytes = int64(wswords) * 4l)
                p.wqh <- hashed_words(totsb * sb_words, 1u, uint(fmt) * 131u)
                p.wsh <- kq_ws_plane(fmt, wswords, 1u, uint(fmt) * 17u)
                p.xqh <- hashed_words(rows * nsb * 64, 7u, 2u)
                p.xsh <- row_fill(rows * nsb, 1, 0, 0.05, -0.3, 11)
                //! one family set serves every format; region 1's weights follow region 0's stack in superblock units
                var r <- sdot4_tile_run(n, d, cnt0, cnt1, 32, d * nsb, p) $(bufs : uint64[6]; sizes : int64[6]; bits : uint[6]; raw2 : VkCommandBuffer; var h2 : VkHaz; wgs : int64) {
                    var sc = set_kq_batch_cls(bufs, sizes, bits)
                    var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(2 * 4))
                    kq_batch_cls_enc_for(fmt, false, raw2, h2, sc, pc, wgs)
                }
                var inscope y_ref : array<float>
                y_ref |> resize(rows * d)
                kq_gemm_oracle(fmt, p.wqh, p.wsh, p.xqh, p.xsh, r.sched, 2, n, d, y_ref)
                check_rows(t, "kq batch fmt {fmt} against the CPU oracle", r.y, y_ref, BAR_APPROX_CTRL, d)
                delete y_ref
                delete r
                delete p
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The per-32 batch tile for q51, iq4nl32, q4032, q5032 and mx4 over two regions with edge tiles and a K tail matches the engine scalar dots (blk32_gemm_oracle) within 2e-3")]
def test_vkd_blk32_batch_family(t0 : T?) {
    t0 |> run("per-32 batch tiles (q51 iq4nl32 q4032 q5032 mx4) == scalar dots") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let n = 96                // nbb = 3: the zero-filled K tail of the 8-block pass
            let nbb = n / 32
            let d = 40                // 2 w-tiles with an 8-col edge
            let cnt0 = 35
            let cnt1 = 20
            let rows = cnt0 + cnt1
            let totblk = 2 * d * nbb
            for (f in [KqFmt.q51, KqFmt.iq4nl32, KqFmt.q4032, KqFmt.q5032, KqFmt.mx4]) {
                let fmt = int(f)
                verify(kq_batch_cls_ensure(fmt, false), "the per-32 batch class rail must engage")
                var wsh <- blk32_scale_plane(f, totblk)
                let ws_bytes = long_length(wsh) * 4l
                var p <- Sdot4Planes(wqh <- hashed_words(totblk * int(kq_qsb(f) / 4l), 1u, uint(fmt) * 131u), wsh <- wsh,
                    ws_bytes = ws_bytes, xqh <- hashed_words(rows * nbb * 8, 7u, 2u), xsh <- row_fill(rows * nbb, 1, 0, 0.05, -0.3, 11))
                var r <- sdot4_tile_run(n, d, cnt0, cnt1, 32, d * nbb, p) $(bufs : uint64[6]; sizes : int64[6]; bits : uint[6]; raw2 : VkCommandBuffer; var h2 : VkHaz; wgs : int64) {
                    var sc = kq_batch_cls_set_for(fmt, false, bufs, sizes, bits)
                    var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(2 * 4))
                    kq_batch_cls_enc_for(fmt, false, raw2, h2, sc, pc, wgs)
                }
                var inscope y_ref : array<float>
                y_ref |> resize(rows * d)
                blk32_gemm_oracle(f, p.wqh, p.wsh, p.xqh, p.xsh, r.sched, 2, n, d, y_ref)
                check_rows(t, "{f} batch tile against the engine's scalar dot", r.y, y_ref, BAR_APPROX_CTRL, d)
                delete r
                delete p
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the cm batch cell's five arms as the class router's variant ordinals: cmf16, cmi8, mm (the L tile), mm aligned-L, mm M-tile
let private CM_CELL_VARIANTS = fixed_array(1, 2, 5, 4, 3)

[test(description = "Each q8 coopmat batch variant the device coopmat mode routes (cmf16, cmi8, the mm L, aligned-L and M tiles) matches the q8 GEMM oracle at the coopmat bar, 2e-2 relative or 4e-3 of the peak")]
def test_vkd_cm_batch_family(t0 : T?) {
    t0 |> run("coopmat batch class kernels == the CPU oracle (per available mode)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // probe via a NON-coopmat ensure — a cm pipeline build
                t |> skip("no Vulkan device")   // on a no-coopmat device panics, not feints
                return
            }
            var ran = 0
            // per variant: [tile, d, cnt0, cnt1, n] — cmi8 keeps rows/cols aligned (its loads are
            // unguarded); mm/mm_m mix full and edge tiles; mm_a is the all-aligned twin
            for (v in range(5)) {
                var tile = 32
                var d = 40
                var cnt0 = 35
                var cnt1 = 20
                var n = 96
                if (v == 1) {
                    d = 64
                    cnt0 = 32
                    cnt1 = 32
                } elif (v == 2) {
                    tile = 128
                    d = 128   // row-edge tiles only: non-128-multiple d ZEROES the whole mm
                    cnt0 = 140   // dispatch (old kernel identical — latent, ledgered) — real model
                    cnt1 = 116   // dims are 128-multiples, so serving never routes that shape
                    n = 64
                } elif (v == 3) {
                    tile = 128
                    d = 128
                    cnt0 = 128
                    cnt1 = 128
                    n = 64
                } elif (v == 4) {
                    tile = 64
                    d = 96
                    cnt0 = 80
                    cnt1 = 48
                    n = 64
                }
                // the router picks this variant only in its coopmat mode — building a cm class
                // pipeline on a device whose mode never routes it is wasted (and panics no-cm)
                let mode = g_gpu.coopmat_mode
                let on = (v == 0 && mode == 1) || (v == 1 && mode == 2) || (v >= 2 && mode >= 3)
                if (!on) {
                    continue
                }
                let cv = CM_CELL_VARIANTS[v]
                let up = q8_batch_cls_ensure(cv)
                assert(up, "class rail must engage in its coopmat mode")
                ran++
                let nbb = n / 32
                let rows = cnt0 + cnt1
                let totblk = 2 * d * nbb
                //! 16x cooler activation scales than the sdot4 arms: the mm family ACCUMULATES in f16, and the hot form's |y| ~7e4 overflows f16's 65504 to inf
                var p <- q8_sdot4_planes(totblk, 8, totblk / 2 + 1, int64(totblk) * 2l, rows * nbb, 8, 0.0625)
                var r <- sdot4_tile_run(n, d, cnt0, cnt1, tile, d * nbb, p) $(bufs : uint64[6]; sizes : int64[6]; bits : uint[6]; raw2 : VkCommandBuffer; var h2 : VkHaz; wgs : int64) {
                    var sc = q8_batch_cls_set(cv, bufs, sizes, bits)
                    var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(2 * 4))
                    q8_batch_cls_enc(cv, raw2, h2, sc, pc, wgs)
                }
                // the cm kernels stage AND accumulate through f16, so their error scales with
                // the running-sum magnitude (cancelled-small results included) — relative bar
                // plus an absolute floor off the oracle's peak; wiring breaks are peak-sized
                var inscope y_ref : array<float>
                y_ref |> resize(rows * d)
                q8_gemm_oracle(p.wqh, p.wsh, p.xqh, p.xsh, r.sched, 2, n, d, y_ref)
                check_rows(t, "cm batch variant {v}, {g_gpu.decvec_on ? "four-wide twin" : "scalar callback"}", r.y, y_ref, cm_bar(y_ref), d)
                delete y_ref
                delete r
                delete p
            }
            if (ran == 0) {
                t |> skip("no coopmat batch pipes in this mode - set DASLLAMA_COOPMAT to cover")
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "In f16 coopmat mode the q40 cmf16 batch tile over two regions with edge tiles matches the kq GEMM oracle at the coopmat bar, 2e-2 relative or 4e-3 of the peak")]
def test_vkd_kq_q40_cm_batch(t0 : T?) {
    t0 |> run("q40 coopmat f16 batch class kernel == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            if (g_gpu.coopmat_mode != COOPMAT_F16) {
                t |> skip("q40 rides the f16 coopmat twin only in mode 1 (DASLLAMA_COOPMAT=f16)")
                return
            }
            verify(ensure_kq_batch_q40_cmf16_cls(), "class rail must engage in f16 coopmat mode")
            let n = 256                // nsb = 1 superblock per row reduction
            let nsb = n / 256
            let d = 40                 // wtiles = 2 with a 8-col edge tile
            let cnt0 = 35
            let cnt1 = 20
            let rows = cnt0 + cnt1
            let totsb = 2 * d * nsb
            //! the scale words hold the d halves - every u16 a finite half
            var p <- q8_sdot4_planes(totsb, 32, totsb * 5, int64(totsb) * 20l, rows * nsb, 64, 1.0)
            var r <- sdot4_tile_run(n, d, cnt0, cnt1, 32, d * nsb, p) $(bufs : uint64[6]; sizes : int64[6]; bits : uint[6]; raw2 : VkCommandBuffer; var h2 : VkHaz; wgs : int64) {
                var sc = set_kq_batch_q40_cmf16_cls(bufs, sizes, bits)
                var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(2 * 4))
                enc_kq_batch_q40_cmf16_cls(raw2, h2, sc, pc, wgs)
            }
            // f16-staged dequant on the MMA path — relative bar + peak-scaled floor
            var y_ref : array<float>
            y_ref |> resize(rows * d)
            kq_gemm_oracle(int(KqFmt.q40), p.wqh, p.wsh, p.xqh, p.xsh, r.sched, 2, n, d, y_ref)
            check_rows(t, "q40 cm batch, {g_gpu.decvec_on ? "four-wide twin" : "scalar callback"}", r.y, y_ref, cm_bar(y_ref), d)
            delete y_ref
            delete r
            delete p
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! a KHR stamp's split arm over a tile fixture: its first region alone under two 256-aligned k chunks (`want_ck` and the rest) into
//! the partial planes, then the reduce into the y plane, against the oracle's first region - the k0 / k1 / ybase prologue every KHR
//! stamp shares with the cm2 tiles; `nsplit` 1 is the whole GEMM, its one partial plane through the reduce
def private khr_split_arm(t : T?; tag : string; f : TileFixture; y_ref : array<float>; want_ck : int64; set_fn; enc_fn; nsplit : int = 2) {
    verify(ensure_splitk_reduce_cls(), "the split-k reduce's class rail must engage")
    let ck = nsplit > 1 ? cm2_chunk(int64(f.n), int64(nsplit)) : 0l
    if (nsplit > 1) {
        t |> equal(ck, want_ck, "the engine's chunk: K {f.n} in two 256-aligned chunks, {want_ck} and {int64(f.n) - want_ck} deep")
    }
    let cnt0 = TILE_CELL_ROWS0
    let d = TILE_CELL_D
    let swgs = ((cnt0 + 127) / 128) * ((d + 127) / 128) * nsplit
    var y_ref0 <- copy_span(y_ref, 0, cnt0 * d)   // the oracle's first region is the split arm's whole answer
    with_cell_planes() $(var cp : CellPlanes) {
        let pwq = cp |> in_plain(f.wqh)
        let pws = cp |> in_plain(f.wsh)
        let psc = cp |> in_plain(one_region_words(cnt0, swgs))   // one region at row 0 over the first weight plane
        let pxf = cp |> in_plain(f.xfh)
        let pp = cp |> out_(cnt0 * d * nsplit)
        let py = cp |> out_(cnt0 * d)
        var scs = cp |> cls_set(fixed_array(pwq, pws, psc, pxf, pp), set_fn)
        var srd = cp |> cls_set(fixed_array(pp, py), @@set_splitk_reduce_cls)
        var pcs = BatchArgs(n = uint(f.n), d = uint(d), map_off = 4u, ksplit = uint(ck))
        cp |> cell_record() $(raw3 : VkCommandBuffer; var h3 : VkHaz) {
            invoke(enc_fn, raw3, h3, scs, pcs, int64(swgs))
            splitk_reduce_enc(raw3, h3, srd, int64(cnt0 * d), int64(nsplit))
        }
        let form = nsplit > 1 ? "split in two k chunks + the reduce" : "whole, one region"
        check_rows(t, "{tag} {form} vs the CPU oracle", cp |> plane_run(py), y_ref0, cm_bar(y_ref0), d)
    }
    delete y_ref0
}

[test(description = "The KHR q8 tile at K 576 matches the f16 q8 oracle at the coopmat bar over two regions past sentinel schedule words, and whole and split into 512 and 64 deep k chunks through the reduce")]
def test_vkd_q8_khr_batch(t0 : T?) {
    t0 |> run("the KHR kq tile over Q8_0 blocks (q8_batch_khr) == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            if (!khr_kq_tile_on()) {
                t |> skip("the KHR kq tile needs KHR cooperative matrices at subgroup 32")
                return
            }
            var wqh, wsh : array<uint>
            q8_planes(576, 17u, 991u, wqh, wsh)   // K off the 256 lattice: the 32-deep k step runs a tail past the last superblock
            var f <- tile_fixture(576, 32, wqh, wsh, 3u, 5u)
            var y_ref <- tile_cell_arms(t, "q8", int(KqFmt.q8), f, [ARM_KHR], @@q8f16_gemm_oracle)
            // the split arm: the first region alone under two 256-aligned k chunks (512 and 64 deep) into the partial
            // planes, then the reduce into the y plane - the k0 / k1 / ybase prologue the KHR stamp shares with the cm2 tiles
            khr_split_arm(t, "q8 KHR tile", f, y_ref, 512l, @@set_q8_batch_khr_cls,
                @@ < (raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : BatchArgs; wgs : int64) : void > enc_q8_batch_khr_cls)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the f32 stamp's set off the KHR arm's five planes: it binds no scale plane, so the second plane stays unbound
def private set_f32_khr_wo_scales(bufs; sizes; gbits) => set_f32_batch_khr_cls(fixed_array(bufs[0], bufs[2], bufs[3], bufs[4]),
    fixed_array(sizes[0], sizes[2], sizes[3], sizes[4]), fixed_array(gbits[0], gbits[2], gbits[3], gbits[4]))

[test(description = "The staged KHR tile over f32 rows rounded to f16 on the stage matches the f16-rounding oracle at the coopmat bar over one region, whole through the reduce and in two k chunks")]
def test_vkd_f32_khr_batch(t0 : T?) {
    t0 |> run("the staged KHR tile over a file's f32 rows (f32_batch_khr) == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            if (!khr_kq_tile_on()) {
                t |> skip("the staged f32 tile needs KHR cooperative matrices at subgroup 32")
                return
            }
            verify(ensure_f32_batch_khr_cls(), "the staged f32 tile's class rail must engage on a coopmat device")
            let n = 544   //! seventeen 32-deep k steps, off every 64 and 256 lattice
            var wqh <- [for (i in range(2 * TILE_CELL_D * n)); float_bits_to_uint(float(hash_word(uint(i) + 71u) % 4001u) * 0.0000317 - 0.0634)]   //! f32 words whose low mantissa bits the stage rounds away
            var wsh <- [0u, 0u, 0u, 0u]   //! the fixture's scale plane, which the f32 stamp binds no slot for
            var f <- tile_fixture(n, 4, wqh, wsh, 3u, 5u)
            let d = TILE_CELL_D
            var y_ref : array<float>
            y_ref |> resize(f.rows * d)
            var recs <- one_region_words(TILE_CELL_ROWS0, ((TILE_CELL_ROWS0 + 127) / 128) * ((d + 127) / 128))
            f32f16_gemm_oracle(f.wqh, f.wsh, f.xfh, recs, 1, n, d, y_ref)   //! the stamp's one schedule form: a region at row 0, its map at word 4
            khr_split_arm(t, "the staged f32 tile", f, y_ref, 0l, @@ < (bufs : uint64 const[5]; sizes : int64 const[5]; gbits : uint const[5]) : VkDescriptorSet > set_f32_khr_wo_scales,
                @@ < (raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : BatchArgs; wgs : int64) : void > enc_f32_batch_khr_cls, 1)   // the projection's whole form
            khr_split_arm(t, "the staged f32 tile", f, y_ref, 512l, @@ < (bufs : uint64 const[5]; sizes : int64 const[5]; gbits : uint const[5]) : VkDescriptorSet > set_f32_khr_wo_scales,
                @@ < (raw : VkCommandBuffer; var h : VkHaz; var set_ : VkDescriptorSet; var pc : BatchArgs; wgs : int64) : void > enc_f32_batch_khr_cls)   // the window gate's split form
            delete recs
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The cm2 l tile over q8 planes at K 576, past sentinel schedule words, matches the f16 q8 oracle at the coopmat bar, 2e-2 relative or 4e-3 of the peak")]
def test_vkd_cm2l_batch(t0 : T?) {
    t0 |> run("cm2 l-tile fmt-0 batch class kernel == the CPU oracle") <| @(t : T?) {
        q8_tile_cell(t, ARM_CM2_L, 576, 17u, 991u, 3u, 5u)   // K off the 256 lattice: two unrolled superblock passes and a 64-deep tail step
    }
}

[test(description = "The q8 cm2 l-tile toy stamp built with the four-wide decode stripped to the scalar callback and built as the device builds it both match one f16 q8 oracle at the coopmat bar")]
def test_vkd_cm2_decode_vector_pair(t0 : T?) {
    t0 |> run("the four-wide decode twin and its stripped fallback, on the device, against one CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!(g_gpu.coopmat_mode == COOPMAT_CM2 && g_gpu.has_coopmat2)) {
                t |> skip("the cm2 l-tile serves only in mode 4 on an NV_coopmat2 device (DASLLAMA_COOPMAT=cm2)")
                return
            }
            let twin_device = g_gpu.decvec_on
            // the strip stamp: words downgraded to the scalar callback whatever the device has
            g_gpu.decvec_on = false
            verify(ensure_toy_q8_cm2_strip_cls(), "the stripped toy stamp must engage")
            g_gpu.decvec_on = twin_device
            verify(ensure_toy_q8_cm2_twin_cls(), "the toy twin stamp must engage")
            var wqh, wsh : array<uint>
            q8_planes(576, 17u, 991u, wqh, wsh)
            var f <- tile_fixture(576, 32, wqh, wsh, 3u, 5u)
            var y_ref <- tile_cell_arms(t, "cm2 decode pair ({twin_device ? "the four-wide twin" : "the twin stamp on a device without the callback"})", int(KqFmt.q8), f, [ARM_TOY_STRIP, ARM_TOY_TWIN], @@q8f16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The Q4_K cm2 l, m, s and e tiles and the KHR tile, decoding in the load, match the format CPU oracle at the coopmat bar over two regions with sentinel schedule words past the end")]
def test_vkd_k4_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q4_K l/m/s/e tiles and the KHR tile == the CPU oracle (decode-in-load over the repacked planes; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.k4)
    }
}

[test(description = "The Q5_K cm2 l, m, s and e tiles and the KHR tile, composing the fifth bit in the load, match the format CPU oracle at the coopmat bar over two regions with sentinel schedule words past the end")]
def test_vkd_k5_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q5_K l/m/s/e tiles and the KHR tile == the CPU oracle (5th-bit compose decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.k5)
    }
}

[test(description = "The Q5_1 per-32 cm2 l, m, s and e tiles and the KHR tile, reading the plane verbatim as d times q plus m, match the format CPU oracle at the coopmat bar over two regions")]
def test_vkd_q51_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q5_1 l/m/s/e tiles and the KHR tile == the CPU oracle (the per-32 plane verbatim: nibble | qh bit, d * q + m off one word)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.q51)
    }
}

[test(description = "The IQ4_NL per-32 cm2 l, m, s and e tiles and the KHR tile, d times the codebook entry off a half-word d, match the format CPU oracle at the coopmat bar over two regions")]
def test_vkd_iq4nl32_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ4_NL per-32 l/m/s/e tiles and the KHR tile == the CPU oracle (the per-32 plane verbatim: d * LUT[nibble] off a half-word d)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.iq4nl32)
    }
}

[test(description = "The Q4_0 per-32 cm2 l, m, s and e tiles and the KHR tile, d times nibble minus 8 off a half-word d, match the format CPU oracle at the coopmat bar over two regions")]
def test_vkd_q4032_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q4_0 per-32 l/m/s/e tiles and the KHR tile == the CPU oracle (iq4nl32's plane form: d * (nibble - 8) off a half-word d)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.q4032)
    }
}

[test(description = "The Q5_0 per-32 cm2 l, m, s and e tiles and the KHR tile, d times q minus 16 with the qh fifth bit, match the format CPU oracle at the coopmat bar over two regions")]
def test_vkd_q5032_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q5_0 per-32 l/m/s/e tiles and the KHR tile == the CPU oracle (q51's quant form under a half-word d: d * (q - 16), the qh bit as q's fifth)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.q5032)
    }
}

[test(description = "The MXFP4 cm2 s and e tiles and the KHR tile, e2m1 nibbles under the halved E8M0 scale with every code reached, match the format CPU oracle at the coopmat bar over two regions")]
def test_vkd_mx4_cm2_batch(t0 : T?) {
    t0 |> run("cm2 MXFP4 s and e tiles == the CPU oracle (the per-32 plane verbatim: e2m1 nibble, the doubled magnitude times the halved E8M0 scale off one byte)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.mx4)
    }
}

[test(description = "The Q4_0 superblock-form cm2 l, m, s and e tiles and the KHR tile, decoding the per-block f16 d in the load, match the format CPU oracle at the coopmat bar over two regions")]
def test_vkd_q40_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q4_0 l/m/s/e tiles and the KHR tile == the CPU oracle (per-block f16 d decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.q40)
    }
}

[test(description = "The Q2_K cm2 l, m, s and e tiles and the KHR tile, decoding the pair-byte nibble scales in the load, match the format CPU oracle at the coopmat bar over two regions")]
def test_vkd_k2_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q2_K l/m/s/e tiles and the KHR tile == the CPU oracle (pair-byte nibble scales decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.k2)
    }
}

[test(description = "The IQ4_NL cm2 l, m, s and e tiles and the KHR tile, codebook plus per-block f16 d decoded in the load, match the format CPU oracle at the coopmat bar over two regions")]
def test_vkd_iq4nl_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ4_NL l/m/s/e tiles and the KHR tile == the CPU oracle (codebook + per-block f16 d decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.iq4nl)
    }
}

[test(description = "The NVFP4 cm2 l, m, s and e tiles and the KHR tile, doubled e2m1 codes under UE4M3 sub-block scales decoded in the load, match the format CPU oracle at the coopmat bar over two regions")]
def test_vkd_nv4_cm2_batch(t0 : T?) {
    t0 |> run("cm2 NVFP4 l/m/s/e tiles and the KHR tile == the CPU oracle (the doubled e2m1 codes under the UE4M3 sub-block scales at half, decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.nv4)
    }
}

[test(description = "The IQ4_XS cm2 l, m, s and e tiles and the KHR tile, with the codebook staged in workgroup memory, match the format CPU oracle at the coopmat bar over two regions")]
def test_vkd_iq4xs_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ4_XS l/m/s/e tiles and the KHR tile == the CPU oracle (workgroup-staged codebook decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.iq4xs)
    }
}

[test(description = "The iq3s4 cm2 l, m, s and e tiles and the KHR tile match the format CPU oracle at the coopmat bar over two regions, and the iq4xs oracle reads the same planes apart as control")]
def test_vkd_iq3s4_cm2_batch(t0 : T?) {
    t0 |> run("cm2 iq3s4 l/m/s/e tiles and the KHR tile == the CPU oracle (the odd codebook staged, the iq3 row's 1 + 2s sub-scale); the iq4xs oracle is its control") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.iq3s4)
    }
}

[test(description = "The Q3_K cm2 l, m, s and e tiles and the KHR tile, composing the 2-bit quants and hmask in the load, match the format CPU oracle at the coopmat bar over two regions")]
def test_vkd_k3_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q3_K l/m/s/e tiles and the KHR tile == the CPU oracle (2-bit + hmask compose decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.k3)
    }
}

[test(description = "The IQ3_S cm2 l, m, s and e tiles and the KHR tile, the grid staged in workgroup memory, match the format CPU oracle at the coopmat bar, and the iq3xxs oracle reads the planes apart")]
def test_vkd_iq3s_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ3_S l/m/s/e tiles and the KHR tile == the CPU oracle (workgroup-staged grid decode-in-load; the KHR arm stages the same decode); the iq3xxs oracle is its control") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.iq3s)
    }
}

[test(description = "The IQ2_S cm2 l, m, s and e tiles and the KHR tile, the u64 grid staged in workgroup memory, match the format CPU oracle at the coopmat bar over two regions")]
def test_vkd_iq2s_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ2_S l/m/s/e tiles and the KHR tile == the CPU oracle (workgroup-staged u64 grid decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.iq2s)
    }
}

[test(description = "The IQ2_XS cm2 l, m, s and e tiles and the KHR tile, u64 grid and ksigns by parity decoded in the load, match the format CPU oracle at the coopmat bar over two regions")]
def test_vkd_iq2xs_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ2_XS l/m/s/e tiles and the KHR tile == the CPU oracle (u64 grid + ksigns-by-parity decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.iq2xs)
    }
}

[test(description = "The IQ2_XXS cm2 l, m, s and e tiles and the KHR tile, byte grid and aux32 ksigns decoded in the load, match the format CPU oracle at the coopmat bar over two regions")]
def test_vkd_iq2xxs_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ2_XXS l/m/s/e tiles and the KHR tile == the CPU oracle (byte-grid + aux32 ksigns decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.iq2xxs)
    }
}

[test(description = "The IQ3_XXS cm2 l, m, s and e tiles and the KHR tile, halved grid and parity signs decoded in the load, match the format CPU oracle at the coopmat bar, and the iq2xxs oracle reads the planes apart")]
def test_vkd_iq3xxs_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ3_XXS l/m/s/e tiles and the KHR tile == the CPU oracle (halved grid + parity signs decode-in-load; the KHR arm stages the same decode); the iq2xxs oracle is its control") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.iq3xxs)
    }
}

[test(description = "The Q6_K cm2 l, m, s and e tiles and the KHR tile, composing the 6-bit quants in the load, match the format CPU oracle at the coopmat bar over two regions")]
def test_vkd_k6_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q6_K l/m/s/e tiles and the KHR tile == the CPU oracle (6-bit compose decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.k6)
    }
}

[test(description = "The cm2 m tile over q8 planes at K 576, past sentinel schedule words, matches the f16 q8 oracle at the coopmat bar, 2e-2 relative or 4e-3 of the peak")]
def test_vkd_cm2m_batch(t0 : T?) {
    t0 |> run("cm2 m-tile fmt-0 batch class kernel == the CPU oracle") <| @(t : T?) {
        q8_tile_cell(t, ARM_CM2_M, 576, 29u, 733u, 5u, 11u)   // K off the 256 lattice: two unrolled superblock passes and a 64-deep tail step
    }
}

[test(description = "The cm2 expert e column over q8 planes at K 544, off the 64-deep k grid so the padded edge path runs, matches the f16 q8 oracle at the coopmat bar")]
def test_vkd_cm2e_batch(t0 : T?) {
    t0 |> run("cm2 e-column fmt-0 batch class kernel == the CPU oracle") <| @(t : T?) {
        q8_tile_cell(t, ARM_CM2_E, Q8_CELL_K, 31u, 739u, 5u, 13u)
    }
}

[test(description = "The cm2 s tile over q8 planes at K 544, off the 64-deep k grid so the padded edge path runs, matches the f16 q8 oracle at the coopmat bar")]
def test_vkd_cm2s_batch(t0 : T?) {
    t0 |> run("cm2 s-tile fmt-0 batch class kernel == the CPU oracle") <| @(t : T?) {
        q8_tile_cell(t, ARM_CM2_S, Q8_CELL_K, 29u, 733u, 5u, 11u)
    }
}

//! One short window through the s column split along K (`ARCHITECTURE_GPU_VULKAN_GEMM.md#vk-short-split`): `rows` under the
//! column's 32 in one region at row 0, `nsplit` 256-aligned chunks (`cm2_chunk`) into the partial planes and the reduce, against the format's
//! oracle; `control` halves the chunk (the planes cover half of K, every element finite) and must red the bar
[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#vk-short-split")]
def private short_split_arm(t : T?; tag : string; fmt : int; f : TileFixture; rows, nsplit : int; control : bool;
                            oracle : function<(wq : array<uint>; wsu : array<uint>; xf : array<uint>; recs : array<uint>; nrec, n, d : int; var y : array<float>) : void>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let d = TILE_CELL_D
        let ck = int(cm2_chunk(int64(f.n), int64(nsplit)))
        verify(ck > 0, "a short-split cell's K takes its 256-aligned chunks")
        let wgs = ((rows + 31) / 32) * ((d + 127) / 128) * nsplit
        var inscope sched <- one_region_words(rows, wgs)
        var inscope y_ref : array<float>
        y_ref |> resize(rows * d)
        invoke(oracle, f.wqh, f.wsh, f.xfh, sched, 1, f.n, d, y_ref)
        with_cell_planes() $(var cp : CellPlanes) {
            let pwq = cp |> in_plain(f.wqh)
            let pws = cp |> in_plain(f.wsh)
            let psc = cp |> in_plain(sched)
            let pxf = cp |> in_plain(f.xfh)
            let pad = ((rows + 31) / 32) * 32 - rows   //! the column's rows past the window: a store guard that slips writes there
            let ppart = cp |> out_(rows * d * nsplit, 0, pad * d * nsplit)
            let py = cp |> out_(rows * d, 0, pad * d)
            let planes = fixed_array(pwq, pws, psc, pxf, ppart)
            var sc = cell_arm_set(ARM_CM2_S, fmt, gather_planes(cp.dev, planes), gather_planes(cp.bytes, planes), gather_planes(cp.hazard_bit, planes))
            var sr = cp |> cls_set(fixed_array(ppart, py), @@set_splitk_reduce_cls)
            var pc = BatchArgs(n = uint(f.n), d = uint(d), map_off = 4u, ksplit = uint(control ? ck / 2 : ck))
            cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                cell_arm_enc(ARM_CM2_S, fmt, raw2, h2, sc, pc, int64(wgs))
                splitk_reduce_enc(raw2, h2, sr, int64(rows * d), int64(nsplit))
            }
            var inscope got <- cp |> plane_run(py)
            let what = "{tag} s-tile short split, {rows} rows in {nsplit} chunks"
            if (control) {
                check_exceeds(t, "{what}: the whole-K control", got, y_ref, cm_bar(y_ref))
            } else {
                check_rows(t, "{what} vs the CPU oracle", got, y_ref, cm_bar(y_ref), d)
            }
            var inscope py_all <- cp |> plane_whole(py)
            var inscope part_all <- cp |> plane_whole(ppart)
            t |> equal(sentinel_off(copy_span(py_all, rows * d, pad * d)), 0, "{what}: the output's rows past the window keep their sentinel")
            t |> equal(sentinel_off(copy_span(part_all, rows * d * nsplit, pad * d * nsplit)), 0, "{what}: the planes past the window's rows keep their sentinel")
        }
    }
}

[test(description = "The cm2 s column split along K into 256-aligned chunks plus the reduce matches the oracle at the coopmat bar under 32 rows for q8 and every per-format tile row, rows past the window keeping sentinel")]
def test_vkd_cm2s_short_split(t0 : T?) {
    t0 |> run("the s column split along K under 32 rows == the CPU oracle, q8 and every per-format tile row") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!cell_arm_served(ARM_CM2_S) || !ensure_splitk_reduce_cls()) {
                t |> skip("the s column serves only in mode 4 on an NV_coopmat2 device (DASLLAMA_COOPMAT=cm2)")
                return
            }
            let qn = 1024
            var qwq, qws : array<uint>
            q8_planes(qn, 43u, 337u, qwq, qws)
            var qf <- tile_fixture(qn, 32, qwq, qws, 7u, 3u)
            for (rows in [9, 31]) {
                short_split_arm(t, "q8", int(KqFmt.q8), qf, rows, 4, false, @@q8f16_gemm_oracle)
            }
            short_split_arm(t, "q8", int(KqFmt.q8), qf, 31, 4, true, @@q8f16_gemm_oracle)
            delete qf
            var trows <- fmt_tile_rows()
            for (r in trows) {
                var wqh, wsuh : array<uint>
                fmt_tile_planes(r, wqh, wsuh)
                var f <- tile_fixture(r.n, r.blk_k, wqh, wsuh, 3u, r.xf_add)
                short_split_arm(t, "{r.fmt}", int(r.fmt), f, 31, fmt_split_ways(f.n), false, r.oracle)
                delete f
            }
            delete trows
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The cm2 l tile split at K 1536 of 2816 into two partial planes and summed by the reduce matches the f16 q8 oracle at the coopmat bar over one 300 by 160 region with edge tiles")]
def test_vkd_cm2l_split(t0 : T?) {
    t0 |> run("cm2 l-tile split-k + reduce == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            if (!(g_gpu.coopmat_mode == COOPMAT_CM2 && g_gpu.has_coopmat2)) {
                t |> skip("the cm2 l-tile serves only in mode 4 on an NV_coopmat2 device (DASLLAMA_COOPMAT=cm2)")
                return
            }
            verify(ensure_q8_batch_cm2l_cls(), "class rail must engage in cm2 mode")
            verify(ensure_splitk_reduce_cls(), "reduce class rail must engage in cm2 mode")
            // single region (split-k's contract): K = 2816 splits as [0, 1536) + [1536, 2816) —
            // plane 0 runs six whole superblock passes, plane 1 five; d/rows keep edge tiles
            let n = 2816
            let nbb = n / 32
            let d = 160
            let rows = 300
            let nsplit = 2
            let ksplit = 1536
            let wtiles = (d + 127) / 128
            let ttiles = (rows + 255) / 256
            let wgs = wtiles * ttiles * nsplit
            let totblk = d * nbb
            var wqh <- hashed_words(totblk * 8, 1u, 43u)     // arbitrary bytes — s8 quants
            var wsh <- ws_plane((totblk + 1) / 2, 1u, 337u)   // f16 scale pairs, finite halves
            var xfh <- ws_plane(rows * n / 2, 7u, 3u)   // f16 activation pairs
            var sched <- one_region_words(rows, wgs)
            var y_ref : array<float>
            y_ref |> resize(rows * d)
            q8f16_gemm_oracle(wqh, wsh, xfh, sched, 1, n, d, y_ref)
            with_cell_planes() $(var cp : CellPlanes) {
                let pwq = cp |> in_plain(wqh)
                let pws = cp |> in_plain(wsh)
                let psc = cp |> in_plain(sched)
                let pxf = cp |> in_plain(xfh)
                let partial_planes = cp |> out_(rows * d * nsplit)   // the GEMM writes partial planes into the scratch, the reduce sums into y
                let py = cp |> out_(rows * d)
                var sc = cp |> cls_set(fixed_array(pwq, pws, psc, pxf, partial_planes), @@set_q8_batch_cm2l_cls)
                var sr = cp |> cls_set(fixed_array(partial_planes, py), @@set_splitk_reduce_cls)
                var pc = BatchArgs(n = uint(n), d = uint(d), map_off = 4u, ksplit = uint(ksplit))
                cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                    enc_q8_batch_cm2l_cls(raw2, h2, sc, pc, int64(wgs))
                    splitk_reduce_enc(raw2, h2, sr, int64(rows * d), int64(nsplit))
                }
                check_rows(t, "cm2 l-tile split-k ({g_gpu.decvec_on ? "four-wide twin" : "scalar callback"}) vs the CPU oracle", cp |> plane_run(py), y_ref, cm_bar(y_ref), d)
            }
            delete sched
            delete y_ref
            delete wqh
            delete wsh
            delete xfh
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! a GEMM pick as "column x chunks x chunk", so a miss prints all three

[test(description = "The resolved coopmat default ladder (cm2, mm, sdot4, wave64 off) and the cm2 tile, split-k, decode attention split and FFN slice picks return their pinned values per shape and SM count")]
def test_vk_coopmat_default_and_tile_pick(t0 : T?) {
    t0 |> run("the resolved coopmat default ladder and the cm2 tile pick (pure functions)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!empty(g_env_vulkan.coopmat) && g_env_vulkan.coopmat != "auto") {
                t |> skip("DASLLAMA_COOPMAT is set - the resolved default ladder is not what this box runs")
                return
            }
            let saved_force = g_coopmat_mode_force
            g_coopmat_mode_force = -1
            t |> equal(resolve_coopmat_mode(true, true, 32l), 4)    // cm2 where the device has it
            t |> equal(resolve_coopmat_mode(true, false, 32l), 3)   // else mm
            t |> equal(resolve_coopmat_mode(false, false, 32l), 0)  // else sdot4
            t |> equal(resolve_coopmat_mode(true, false, 64l), 0)   // a wave64 device: the coopmat tiles' four fragment owners are 32-lane subgroups
            t |> equal(resolve_coopmat_mode(true, true, 64l), 0)
            g_coopmat_mode_force = 3
            t |> equal(resolve_coopmat_mode(true, false, 64l), 0)   //! ... and a forced mode follows the same rule
            g_coopmat_mode_force = saved_force
            if (!ensure_q8_batch_cls()) {
                t |> skip("no Vulkan device for the tile pick (it reads the device's SM count)")
                return
            }
            if (g_env_vulkan.cm2_tile != 0l || g_env_vulkan.cm2_splitk != 0l) {
                t |> skip("DASLLAMA_CM2_TILE or DASLLAMA_CM2_SPLITK is set - the picks below are forced, not the heuristic")
                return
            }
            let saved_sm = g_gpu.sm_count
            let saved_mode = g_gpu.coopmat_mode
            g_gpu.sm_count = 36
            g_gpu.coopmat_mode = COOPMAT_MM
            t |> success(!cm2_fa_on(), "under mm the cm2 flash pick follows the mode, whatever the card supports")
            if (khr_kq_tile_on()) {
                t |> success(pf_f16_feed(int(KqFmt.q8)), "under mm the f16 feed admits Q8_0: its dense GEMMs take the KHR kq tile")
                t |> success(ple_proj_shape_ok(1536l, 8960l), "under mm the per-layer-embedding projection takes the KHR f16 GEMM at 16-multiples")
                t |> success(!ple_proj_shape_ok(1536l, 8968l), "... and stays on the CPU off them")
            } else {   //! a device without KHR cooperative matrices at subgroup 32 refuses both routes under mm too
                t |> success(!pf_f16_feed(int(KqFmt.q8)), "without the KHR kq tile Q8_0 keeps the requant feed under mm")
                t |> success(!ple_proj_shape_ok(1536l, 8960l), "... and the projection stays on the CPU")
            }
            t |> equal(cm2_tile_cols(8192l, 512l), 128l)                       // the KHR arm has one tile geometry
            t |> equal(cm2_tile_cols(768l, 32l), 128l)                         // ... the bucket rule included
            let khr_kv = cm2_gemm_pick(512l, 512l, 2048l)
            t |> equal(khr_kv.tc, 128l)                                        // ... under the split too
            t |> equal(khr_kv.nsplit, 2l)                                      // ... and the wave model's k chunks: the 1B k/v shape's 16 tiles under-fill 36 SMs
            t |> equal(cm2_gemm_pick(2048l, 512l, 2048l).nsplit, 1l)           // ... where its q shape's 64 tiles run whole
            g_gpu.coopmat_mode = COOPMAT_SDOT4
            t |> success(!pf_f16_feed(int(KqFmt.q8)), "under sdot4 Q8_0 keeps the requant feed - the reference arm's quant path")
            t |> success(fa_khr_serves(256l, 50.0, false) && fa_khr_serves(512l, 0.0, false) && fa_khr_serves(256l, 0.0, true), "the KHR flash pick: a 256 head under the softcap, a 512 head, a gated 256 head")
            t |> success(!fa_khr_serves(128l, 50.0, false) && !fa_khr_serves(128l, 0.0, true) && !fa_khr_serves(96l, 0.0, false) && !fa_khr_serves(256l, 50.0, true), "... not a capped 128 head, a gated 128 head, a 96 head, or a gated head under the softcap")
            g_gpu.coopmat_mode = COOPMAT_CM2
            t |> equal(cm2_tile_cols(3072l, 128l), 128l)    // a short window never takes the half-empty l column
            t |> equal(cm2_tile_cols(3072l, 512l), 128l)    // the 3B q/down shape: m on a strict wave win (96 tiles in 3 waves, 30 units, vs 48 in 2, 32)
            t |> equal(cm2_tile_cols(8192l, 512l), 256l)    // the 3B gate/up shape: l (128 tiles in 4 waves, 64 units, vs 256 in 8, 80)
            t |> equal(cm2_tile_cols(5632l, 512l), 256l)    // the tinyllama gate shape: l (88 tiles in 3 waves, 48 units, vs 176 in 5, 50)
            t |> equal(cm2_tile_cols(768l, 32l), 32l)       // the expert-bucket shape (30B-A3B pp512: ~32 rows per region) takes s
            t |> equal(cm2_tile_cols(768l, 64l), 32l)       // ... up to the half-m edge
            t |> equal(cm2_tile_cols(768l, 65l), 128l)      // past it an m column is more than half full
            t |> equal(batch_tile_edges(int(KqFmt.iq3s4), 672l, 112l, true).tc, 128l, "the batch arm's cm2 column follows the pick on a superblock format")
            t |> equal(batch_tile_edges(int(KqFmt.iq4nl32), 2560l, 112l, true).tc, cm2_tile_cols(2560l, 112l), "... and on a per-32 plane, which carries the l and m columns too")
            t |> equal(batch_tile_edges(int(KqFmt.q51), 2560l, 512l, true).tc, cm2_tile_cols(2560l, 512l), "... q51 too")
            t |> equal(batch_tile_edges(int(KqFmt.mx4), 2880l, 512l, true).tc, 32l, "... and mx4")
            t |> equal(cm2_tiles(512l, 512l), 16l)                             // the 1B k/v shape: four m columns of four
            t |> equal(pick_str(cm2_gemm_pick(512l, 512l, 2048l)), "256x4x512")   // ... alone it fills under half the SMs: four l chunks (32 workgroups of 8 steps, 205 units with the reduce, against 320 whole)
            let saved_knee = vk_split_k_tgs()
            set_vk_split_k_tgs(0l)
            t |> equal(pick_str(cm2_gemm_pick(1024l, 31l, 1024l)), "32x1x0")   //! the short window off its knee: under the column's rows the wave model splits nothing
            set_vk_split_k_tgs(256l)
            t |> equal(pick_str(cm2_gemm_pick(1024l, 31l, 1024l)), "32x4x256")   //! ... on it the shared rule splits the 8-workgroup grid into four 256-deep chunks
            let short_plane = 31l * 1024l * 4l   //! one partial plane of the 31-row, 1024-wide window
            t |> equal(pick_str(cm2_gemm_pick(1024l, 31l, 1024l, 0l, short_plane * 2l)), "32x2x512", "the short split halves to the two planes the scratch holds")
            t |> equal(pick_str(cm2_gemm_pick(1024l, 31l, 1024l, 0l, short_plane * 2l - 1l)), "32x1x0", "... and stays whole where it holds fewer")
            t |> equal(pick_str(cm2_gemm_pick(1024l, 31l, 1024l, 130l)), "32x1x0")   //! ... and keeps it whole where the co-running roles fill the grid past half the knee
            t |> equal(pick_str(cm2_gemm_pick(1024l, 31l, 1536l)), "32x2x768")   //! a 1536-deep K stops doubling where the next share would leave the 256-wide chunks
            t |> equal(pick_str(cm2_gemm_pick(1024l, 32l, 1024l)), "32x1x0")   //! a full column is the wave model's whatever the knee
            set_vk_split_k_tgs(saved_knee)
            t |> equal(cm2_gemm_pick(512l, 512l, 2048l, 80l).nsplit, 1l)       // ... beside q and the other kv projection (96 in the group): never
            t |> equal(cm2_gemm_pick(2048l, 512l, 2048l).nsplit, 1l)           // the 1B q shape: 32 l tiles in one wave, no split pays
            t |> equal(cm2_gemm_pick(512l, 512l, 2048l, 16l).nsplit, 1l)       // the 35B shexp gate beside up (32 in the group): whole, the pair co-runs
            g_gpu.sm_count = 84
            t |> equal(cm2_gemm_pick(512l, 512l, 2048l, 16l).nsplit, 1l, "on 84 SMs the pair takes the s column - 64 workgroups beside up's 16 fill the card in one wave: whole")
            t |> equal(cm2_gemm_pick(512l, 512l, 2048l, 144l).nsplit, 1l, "k beside q's 128 and v's 16 workgroups: never")
            let g3d = cm2_gemm_pick(1152l, 512l, 6912l)
            t |> equal(g3d.tc, 256l, "gemma-3-1b's down (36 m tiles of 84): the l column under the split")
            t |> equal(g3d.nsplit, 4l, "... in four chunks of 1792 (72 l tiles in one wave: 448 units and the reduce's 96 against 1080 whole)")
            t |> equal(g3d.ksplit, 1792l)
            t |> equal(cm2_tile_cols(256l, 512l), 32l, "gemma-3-1b's k/v shape (8 m tiles of 84): the s column")
            t |> equal(cm2_tile_cols(1024l, 512l), 128l, "... its q shape (32 m tiles): m")
            g_gpu.sm_count = 82
            //! the narrow rule at the gemma-4 E2B's K-quant k/v widths on 82 SMs (`narrow:<fmt>`'s one-dispatch rows): every K-quant's s stamp
            //! under its m stamp through 640, the m stamp under s past it
            t |> equal(cm2_tile_cols(256l, 512l), 32l, "E2B's 256-wide k/v (8 m tiles of 82): the s column")
            t |> equal(cm2_tile_cols(512l, 512l), 32l, "... its 512-wide k/v (16 m tiles): s")
            t |> equal(cm2_tile_cols(640l, 512l), 32l, "... the widest plane the rule takes (20 m tiles, 80 of 82)")
            t |> equal(cm2_tile_cols(768l, 512l), 128l, "... the first it leaves to the wave model (24 m tiles): m")
            t |> equal(cm2_tile_cols(256l, 300l), 32l, "... a 300-row window's 256-wide k/v: s")
            t |> equal(cm2_split_fit(224l, 224l / 256l), 1l, "a K under one 256-aligned chunk (224) asks no chunk and runs whole, never zero pieces")
            t |> equal(cm2_split_fit(1536l, 6l, 512l * 256l * 4l, 2l * 512l * 256l * 4l), 2l, "the chunk count fits the scratch: two planes of room, two chunks")
            let SPLIT_K_SCRATCH_PER_SM = 262_144l   //! the split-k scratch the prefill allocates an SM
            let scratch82 = SPLIT_K_SCRATCH_PER_SM * 82l
            t |> equal(pick_str(cm2_gemm_pick(1536l, 512l, 12288l, 0l, scratch82)), "256x3x4096", "E2B's 12288-deep down on 82 SMs: three l chunks")
            t |> equal(pick_str(cm2_gemm_pick(2560l, 512l, 16384l, 0l, scratch82)), "256x2x8192", "E4B's 16384-deep down: two l chunks")
            t |> equal(pick_str(cm2_gemm_pick(2560l, 512l, 8192l, 0l, scratch82)), "256x2x4096", "E4B's 8192-deep down: two l chunks")
            t |> equal(pick_str(cm2_gemm_pick(2304l, 512l, 9216l, 0l, scratch82)), "256x2x4608", "gemma-2's down: two l chunks")
            t |> equal(pick_str(cm2_gemm_pick(1536l, 512l, 2048l, 0l, scratch82)), "256x3x768", "E2B's wo at K 2048: three l chunks")
            t |> equal(pick_str(cm2_gemm_pick(3840l, 512l, 15360l, 0l, scratch82)), "256x1x0", "the 12B's 15360-deep down: 60 l tiles whole - two m chunks would save under a tenth")
            t |> equal(pick_str(cm2_gemm_pick(16384l, 512l, 2560l, 0l, scratch82)), "256x1x0", "E4B's 16384-wide gate: l whole, the split's planes past the scratch anyway")
            t |> equal(pick_str(cm2_gemm_pick(16384l, 512l, 2560l, 256l, scratch82)), "256x1x0", "... beside up's 256 tiles still l: the siblings ride the whole GEMM's waves, never its column")
            t |> equal(pick_str(cm2_gemm_pick(10240l, 512l, 2560l, 160l, scratch82)), "256x1x0", "gemma-3-4b's 10240-wide gate beside up: l whole")
            t |> equal(cm2_gemm_pick(1152l, 512l, 1024l, 0l, scratch82).nsplit, 1l, "gemma-3-1b's wo at K 1024: under 2048 never splits")
            t |> equal(da_nsplit(4l, 82l), 16l, "the decode attention's key split on 82 SMs: four heads take the cap of sixteen")
            t |> equal(da_nsplit(32l, 82l), 6l, "... thirty-two heads take six splits (twice the SMs over the heads, rounded up)")
            t |> equal(da_nsplit(200l, 82l), 1l, "... heads past twice the SM count take one")
            t |> equal(da_nsplit(4l, 0l), 1l, "... an unknown SM count takes one")
            g_gpu.sm_count = 36
            t |> equal(pick_str(cm2_gemm_pick(2048l, 32l, 8192l, 0l, SPLIT_K_SCRATCH_PER_SM * 36l)), "32x2x4096")   // the sliced down shape from row 0: 16 s tiles of 36 take two chunks (383 units against 640 whole)
            t |> equal(pf_split_pick(int(KqFmt.k4), 2048l, 32l, 8192l, 480l, 0l).nsplit, 1l)         // ... from row 480 it never splits (the window chain's pick reads the driver's scratch only from row 0)
            t |> equal(pf_ffn_slice_row0(15l, 16l, 512l, true, true), 480l)   // the last layer on the f16 route starts 32 rows below the window's end
            t |> equal(pf_ffn_slice_row0(15l, 16l, 20l, true, true), 0l)      // a window under 32 rows runs whole
            t |> equal(pf_ffn_slice_row0(14l, 16l, 512l, true, true), 0l)     // an earlier layer runs whole
            t |> equal(pf_ffn_slice_row0(15l, 16l, 512l, false, true), 0l)    // a feed off the f16 route runs whole
            g_gpu.sm_count = 0
            t |> equal(cm2_tile_cols(3072l, 64l), 32l)      // unknown SM count: the bucket rule still fires
            t |> equal(cm2_tile_cols(3072l, 100l), 128l)    // unknown SM count: the short-window rule still fires
            t |> equal(cm2_tile_cols(3072l, 512l), 256l)    // unknown SM count: l, never a split
            g_gpu.sm_count = saved_sm
            g_gpu.coopmat_mode = saved_mode
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}
