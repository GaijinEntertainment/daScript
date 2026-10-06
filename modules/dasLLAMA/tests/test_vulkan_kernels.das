options gen2
options _function_length = 0   // one flat gate per kernel arm: fixture build, dispatch, compare (the tier suites' shape)
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require math
require strings   // starts_with: the roster cell finds an entry by its name's head
require dasllama/dasllama_math
require _compares
require _kernel_oracles   // rope_tab_ref: the rope and KV-writer cells' oracle
require _attn_oracle   // attn_row_oracle: the batched decode attention cells' oracle
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
require dasllama/dasllama_gpu_math   // softcap_exp - the softcap cells' oracle
require ?vulkan dasllama/dasllama_vulkan_prefill
require ?vulkan dasllama/dasllama_vulkan_decode   // da_nsplit - the decode attention's key-split pick the tile-pick cell pins
require ?vulkan dasllama/dasllama_vulkan_seams
require ?vulkan dasllama/dasllama_kqformat
require ?vulkan dasllama/dasllama_convert   // quantize_q8_0_into: the hyper-connection low-rank requant's oracle
require ?vulkan spirv/spirv_dis   // foreach_inst / count_op_operand_at: the stamp-shape cells' instruction walks
require ?vulkan spirv/spirv_grammar   // SpvOp / SpvDecoration / SpvStorageClass
require dasllama/dasllama_gemm_schema   // Q8_BLOCK_ELEMS sizes the q8 plane helper outside the vulkan arm; kq_dev_ssb_i - the fixtures size their device scale planes per format
require ?vulkan daslib/shader_lingua_franca   // gl_GlobalInvocationID — the CPU replay drives it per lane (only vulkan-guarded arms replay)
require dasllama/dasllama_common   // MoeGate: the routing cells' select oracle takes the softmax router
require dasllama/dasllama_arch_qwen35
require ?vulkan dasllama/dasllama_moe   // moe_select_core: the host select the window's per-row top-k mirrors
require daslib/strings_boost   // join: the spawned fixture's source
require daslib/math_bits   // float_bits_to_uint / uint_bits_to_float: the NaN sentinel by bit pattern, the read-back planes as floats

let private TILE_SLACK_ROWS = int(TILE_READ_SLACK)   //! the cm2 fixtures' rows past the last region, the engine's count: a stamp that loads partial columns unclamped (`STILE`) reads the last region's column rounded up to its BN, so the slack holds while every such stamp keeps BN <= slack + 1
let private TILE_CELL_K = 768   //! every per-format tile cell's K: three unrolled superblock passes on both depths (a K-quant width is a multiple of 256, so no K-quant stamp ever takes the k loop's tail; the q8 s and e cells' Q8_CELL_K takes the edge path)
let private Q8_CELL_K = 544     //! the q8 cm2 s and e cells' K: 512 + 32, off the 64-deep stamp's k grid, so the tile takes its padded edge path (the l and m cells run K = 576, the fast path)
let private TILE_CELL_D = 160   //! every tile cell's weight rows a plane: one full 128-tile and a 32-row remainder - the KHR arm's edge tile, the cm2 arms' clamped second tile over rows 32..160
let private TILE_CELL_ROWS0 = 300   //! the tile cells' two regions: on every arm's rows a workgroup, whole tiles and an edge
let private TILE_CELL_ROWS1 = 260

let private I8_SENTINEL_BITS = 0x80808080u   //! a Q8 quant plane's fill: -128 in every byte, a value no requant produces (|q| <= 127)

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
def private cell_arm_rows_per_wg(arm) => arm == ARM_CM2_L || arm >= ARM_TOY_STRIP ? 256 : int(arm == ARM_CM2_S ? SCHED_S_ROWS : SCHED_M_ROWS)

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

//! one q8 fmt-0 host plane of `totblk` blocks: the s8 quant words hashed at `i * wq_mul + wq_add`, the f16 scale pairs at `i * ws_mul + ws_add`
def private q8_plane(totblk : int; wq_mul, wq_add, ws_mul, ws_add : uint; var wqh, wsh : array<uint>) {
    let qwords = totblk * int(Q8_BLOCK_ELEMS / 4l)   //! a byte an element: q8 has no `kq_desc` stride row (`kq_qsb` panics on it by design)
    let swords = (totblk * 2 + 3) / 4   //! an f16 scale a block
    delete wqh
    delete wsh
    wqh <- hashed_words(qwords, wq_mul, wq_add)
    wsh <- ws_plane(swords, ws_mul, ws_add)
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
            var sched <- sr.words
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

//! a per-format tile cell's scale-row form: the words one superblock (or one per-32 block) lays in the device's scale plane
enum private ScaleForm {
    d_dmin_hash   //! [d | dmin], then four hashed sub-scale words
    d_pairs       //! four words of two per-block f16 d, then a zero pad word
    hash_d        //! four hashed signed sub-scale words, then [d | 0]
    d_hi_hash     //! [d | the row's hashed high bits], then one hashed word of 4-bit sub-scale fields
    q51_dm        //! one [d | m] word a block, m signed
    iq4nl32_d     //! two f16 d a word
    mx4_e8m0      //! four E8M0 bytes a word
}

//! one row of the per-format tile table: the format, its K and scale unit, the hashed quant words a unit, the scale-row form
//! with its d / dmin multipliers and hash seed, the activation plane's hash offset, the arms (per-32 formats run s, e and KHR),
//! the format's oracle and the sibling codebook's oracle its planes must read apart under (`control`, when set)
struct private FmtTile {
    fmt : KqFmt
    n, blk_k, wq_words : int
    wq_seed : uint
    form : ScaleForm
    dmul, mmul : float
    sseed, xf_add : uint
    per32 : bool
    oracle : function<(wq : array<uint>; wsu : array<uint>; xf : array<uint>; recs : array<uint>; nrec, n, d : int; var y : array<float>) : void>
    control : function<(wq : array<uint>; wsu : array<uint>; xf : array<uint>; recs : array<uint>; nrec, n, d : int; var y : array<float>) : void>
}

//! the per-format tile cells' table: one row a `kq_sb` or per-32 format
def private fmt_tile_rows() : array<FmtTile> {
    var rows : array<FmtTile>
    static_if (typeinfo builtin_module_exists(vulkan)) {
        rows <- [
            FmtTile(fmt = KqFmt.k4, n = TILE_CELL_K, blk_k = 256, wq_words = 32, wq_seed = 29u, form = ScaleForm.d_dmin_hash, dmul = 0.0002, mmul = 0.00005, sseed = 613u, xf_add = 5u, oracle = @@k4f16_gemm_oracle),
            FmtTile(fmt = KqFmt.k5, n = TILE_CELL_K, blk_k = 256, wq_words = 40, wq_seed = 29u, form = ScaleForm.d_dmin_hash, dmul = 0.0002, mmul = 0.00005, sseed = 613u, xf_add = 5u, oracle = @@k5f16_gemm_oracle),
            FmtTile(fmt = KqFmt.k2, n = TILE_CELL_K, blk_k = 256, wq_words = 16, wq_seed = 31u, form = ScaleForm.d_dmin_hash, dmul = 0.0004, mmul = 0.0003, sseed = 91u, xf_add = 7u, oracle = @@k2f16_gemm_oracle),
            FmtTile(fmt = KqFmt.iq2s, n = TILE_CELL_K, blk_k = 256, wq_words = 18, wq_seed = 29u, form = ScaleForm.d_dmin_hash, dmul = 0.0002, mmul = 0.0, sseed = 613u, xf_add = 5u, oracle = @@iq2sf16_gemm_oracle),
            FmtTile(fmt = KqFmt.iq2xs, n = TILE_CELL_K, blk_k = 256, wq_words = 16, wq_seed = 29u, form = ScaleForm.d_dmin_hash, dmul = 0.0002, mmul = 0.0, sseed = 613u, xf_add = 5u, oracle = @@iq2xsf16_gemm_oracle),
            FmtTile(fmt = KqFmt.q40, n = TILE_CELL_K, blk_k = 256, wq_words = 32, wq_seed = 29u, form = ScaleForm.d_pairs, dmul = 0.0002, mmul = 0.00013, xf_add = 5u, oracle = @@q40f16_gemm_oracle),
            FmtTile(fmt = KqFmt.iq4nl, n = TILE_CELL_K, blk_k = 256, wq_words = 32, wq_seed = 29u, form = ScaleForm.d_pairs, dmul = 0.0002, mmul = 0.00013, xf_add = 5u, oracle = @@iq4nlf16_gemm_oracle),
            FmtTile(fmt = KqFmt.k3, n = TILE_CELL_K, blk_k = 256, wq_words = 24, wq_seed = 29u, form = ScaleForm.hash_d, dmul = 0.0002, sseed = 811u, xf_add = 5u, oracle = @@k3f16_gemm_oracle),
            FmtTile(fmt = KqFmt.k6, n = TILE_CELL_K, blk_k = 256, wq_words = 48, wq_seed = 41u, form = ScaleForm.hash_d, dmul = 0.0002, sseed = 811u, xf_add = 5u, oracle = @@k6f16_gemm_oracle),
            FmtTile(fmt = KqFmt.iq3s, n = TILE_CELL_K, blk_k = 256, wq_words = 26, wq_seed = 29u, form = ScaleForm.d_hi_hash, dmul = 0.0002, sseed = 613u, xf_add = 5u, oracle = @@iq3sf16_gemm_oracle, control = @@iq3xxsf16_gemm_oracle),
            FmtTile(fmt = KqFmt.iq2xxs, n = TILE_CELL_K, blk_k = 256, wq_words = 16, wq_seed = 29u, form = ScaleForm.d_hi_hash, dmul = 0.0002, sseed = 613u, xf_add = 5u, oracle = @@iq2xxsf16_gemm_oracle),
            FmtTile(fmt = KqFmt.iq3xxs, n = TILE_CELL_K, blk_k = 256, wq_words = 24, wq_seed = 29u, form = ScaleForm.d_hi_hash, dmul = 0.0002, sseed = 613u, xf_add = 5u, oracle = @@iq3xxsf16_gemm_oracle, control = @@iq2xxsf16_gemm_oracle),
            FmtTile(fmt = KqFmt.iq4xs, n = TILE_CELL_K, blk_k = 256, wq_words = 32, wq_seed = 29u, form = ScaleForm.d_hi_hash, dmul = 0.0002, sseed = 613u, xf_add = 5u, oracle = @@iq4xsf16_gemm_oracle, control = @@iq3s4f16_gemm_oracle),
            FmtTile(fmt = KqFmt.iq3s4, n = TILE_CELL_K, blk_k = 256, wq_words = 32, wq_seed = 29u, form = ScaleForm.d_hi_hash, dmul = 0.0002, sseed = 613u, xf_add = 5u, oracle = @@iq3s4f16_gemm_oracle, control = @@iq4xsf16_gemm_oracle),
            FmtTile(fmt = KqFmt.q51, n = 704, blk_k = 32, wq_words = 5, wq_seed = 37u, form = ScaleForm.q51_dm, xf_add = 5u, per32 = true, oracle = @@q51f16_gemm_oracle),         //! the 26B-A4B's expert width: 22 blocks, two unrolled 256-passes and a 192 tail
            FmtTile(fmt = KqFmt.iq4nl32, n = 640, blk_k = 32, wq_words = 4, wq_seed = 41u, form = ScaleForm.iq4nl32_d, xf_add = 5u, per32 = true, oracle = @@iq4nl32f16_gemm_oracle),   //! Qwen3.8-Flash-Next's expert width: 20 blocks, two unrolled 256-passes and a 128 tail
            FmtTile(fmt = KqFmt.mx4, n = 704, blk_k = 32, wq_words = 4, wq_seed = 53u, form = ScaleForm.mx4_e8m0, xf_add = 5u, per32 = true, oracle = @@mx4f16_gemm_oracle)            //! 22 blocks: eleven 64-deep k steps, two unrolled 256-passes and a 192 tail
        ]
    }
    return <- rows
}

//! a table row's host planes over both weight planes: the quant words hashed at `i + wq_seed` (cool scales throughout: the tile's f16 sums stay far from 65504)
def private fmt_tile_planes(row : FmtTile; var wqh, wsh : array<uint>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let units = 2 * TILE_CELL_D * (row.n / row.blk_k)
        delete wqh
        wqh <- hashed_words(units * row.wq_words, 1u, row.wq_seed)
        if (row.form == ScaleForm.d_dmin_hash || row.form == ScaleForm.d_pairs || row.form == ScaleForm.hash_d) {
            wsh |> resize(units * 5)
        } elif (row.form == ScaleForm.d_hi_hash) {
            wsh |> resize(units * 2)
        } elif (row.form == ScaleForm.q51_dm) {
            wsh |> resize(units)
        }
        for (sb in range(units)) {
            if (row.form == ScaleForm.d_dmin_hash) {
                wsh[sb * 5] = packHalf2x16(float2(row.dmul * float(1 + sb % 7), row.mmul * float(1 + sb % 5)))
                for (wi in range(4)) {
                    wsh[sb * 5 + 1 + wi] = hash_word(uint(sb * 4 + wi) + row.sseed)
                }
            } elif (row.form == ScaleForm.d_pairs) {
                for (wi in range(4)) {
                    let p = sb * 4 + wi
                    wsh[sb * 5 + wi] = packHalf2x16(float2(row.dmul * float(1 + p % 7), row.mmul * float(1 + p % 5)))
                }
            } elif (row.form == ScaleForm.hash_d) {
                for (wi in range(4)) {
                    wsh[sb * 5 + wi] = hash_word(uint(sb * 4 + wi) + row.sseed)
                }
                wsh[sb * 5 + 4] = packHalf2x16(float2(row.dmul * float(1 + sb % 7), 0.0))
            } elif (row.form == ScaleForm.d_hi_hash) {
                wsh[sb * 2] = packHalf2x16(float2(row.dmul * float(1 + sb % 7), 0.0)) | (hash_word(uint(sb * 2) + row.sseed) << 16u)
                wsh[sb * 2 + 1] = hash_word(uint(sb * 2 + 1) + row.sseed)
            } elif (row.form == ScaleForm.q51_dm) {
                wsh[sb] = packHalf2x16(float2(0.0002 * float(1 + sb % 7), 0.0001 * float(sb % 5) - 0.0002))
            }
        }
        if (row.form == ScaleForm.iq4nl32_d) {
            delete wsh
            wsh <- iq4nl32_scale_words(units, 0.0002)
        } elif (row.form == ScaleForm.mx4_e8m0) {
            delete wsh
            wsh <- mx4_scale_words(units)
        }
    }
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
        let arms <- rows[at].per32 ? [ARM_CM2_S, ARM_CM2_E, ARM_KHR] : tile_arms_all()
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

//! a two-region tile schedule: records [wblk0, row0, cnt, wg0] - rows [0, cnt0) from weight block 0, rows [cnt0, cnt0 + cnt1)
//! from `wblk1` - then the workgroup map at `ttile` rows by `wtiles` weight tiles, `extra` zero words past it; `wgs` the map's length
def private two_region_sched(cnt0, cnt1, wblk1, ttile, wtiles, extra : int) : tuple<words : array<uint>; wgs : int> {
    let wgs0 = ((cnt0 + ttile - 1) / ttile) * wtiles
    let wgs = wgs0 + ((cnt1 + ttile - 1) / ttile) * wtiles
    var words : array<uint>
    words |> resize(2 * 4 + wgs + extra)
    words[2] = uint(cnt0)
    words[4] = uint(wblk1)
    words[5] = uint(cnt0)
    words[6] = uint(cnt1)
    words[7] = uint(wgs0)
    for (w in range(wgs)) {
        words[8 + w] = w < wgs0 ? 0u : 1u
    }
    return <- (words = words, wgs = wgs)
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

// The model-less Vulkan kernel-unit suite ([vk_dispatch] bring-up): a class kernel dispatched
// through the GENERATED surface (ensure/set/enc — per-class layout, push constants, hazard rail)
// against the CPU run of the SAME method (the class body is ordinary daslang; the driver loop
// feeds gl_GlobalInvocationID). The facts the cells' gates key on: a Vulkan device (a class's
// ensure engages it); an NV_coopmat2 device in cm2 mode (DASLLAMA_COOPMAT=cm2) for the cm2
// l/m/s/e arms; KHR cooperative matrix at subgroup 32 for the KHR arm; DASLLAMA_CM2_TILE and
// DASLLAMA_CM2_SPLITK for the tile-pick cell; a device over the deltanet chain's shared-memory
// cap for the dn step cells (`g_gpu.dn_shared_ok`, else they skip); a device over the batched
// decode attention classes' workgroup footprint (`ensure_da_attn_b*_cls`, else the da_attn_b, the
// wide-head and the gated cells skip) and a coopmat device for their h128 arms. Without dasVulkan
// (or with no device) every check skips or feints cleanly.

[test]
def test_vkd_class_dispatch(t0 : T?) {
    t0 |> run("class kernel through the generated [vk_dispatch] surface == CPU") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_vkd_toy()) {
                t |> skip("no Vulkan device")
                return
            }
            let n = 250   // deliberately not a multiple of 64 — the in-kernel guard covers the pad
            var xs : array<float>
            xs |> resize(n)
            for (i in range(n)) {
                xs[i] = float(i) * 0.25 - 7.0
            }
            // CPU oracle: the same method on an instance with plain arrays; the loop overruns the
            // logical range exactly like the padded GPU grid does, so the guard is under test too
            var k = new VkdToy()
            k.xs := xs
            k.ys |> resize(n)
            k.pa = VtArgs(n = uint(n), gain = 2.5)
            for (i in range(n + 6)) {
                gl_GlobalInvocationID = uint3(uint(i), 0u, 0u)
                k->run()
            }
            // device: SSBOs + the generated set/encoder, one submit, readback
            var gpu : array<float>
            with_cell_planes() $(var cp : CellPlanes) {
                let px = cp |> in_(xs)
                let py = cp |> out_(n)
                var s = cp |> cls_set(fixed_array(px, py), @@set_vkd_toy)
                var pc = VtArgs(n = uint(n), gain = 2.5)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_vkd_toy(raw, h, s, pc, int64(n))
                }
                delete gpu
                gpu <- cp |> plane_run(py)
            }
            check_exact(t, "GPU vs CPU on all {n} lanes", gpu, k.ys)
            delete xs
            delete gpu
            unsafe {
                delete k
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private SPV_CAP_COOPMAT_KHR = 6022u
let private SPV_CAP_COOPMAT_NV = 5357u
let private SPV_CAP_NV_COOPMAT2_FIRST = 5430u   //! CooperativeMatrixReductionsNV through TensorAddressingNV: the NV_cooperative_matrix2 capabilities
let private SPV_CAP_NV_COOPMAT2_LAST = 5439u

def private has_nv_coopmat_cap(caps : table<uint>) : bool {
    for (c in keys(caps)) {
        if (c == SPV_CAP_COOPMAT_NV || (c >= SPV_CAP_NV_COOPMAT2_FIRST && c <= SPV_CAP_NV_COOPMAT2_LAST)) return true
    }
    return false
}

//! the kept words of one stamp, read back through the pipeline seat's keeping seam
def private kept_stamp_shape(kernel : string) : tuple<wg_vars : int; caps : table<uint>> {
    var res : tuple<wg_vars : int; caps : table<uint>>
    res.wg_vars = -1
    static_if (typeinfo builtin_module_exists(vulkan)) {
        g_vkd_spv_kept |> get(kernel) $(words) {
            res.wg_vars = count_op_operand_at(words, SpvOp.Variable, 2, uint(SpvStorageClass.Workgroup))
            foreach_inst(words) $(op : uint; w : array<uint>; s, n : int) {
                if (op == uint(SpvOp.Capability) && n == 1) {
                    res.caps |> insert(w[s])
                }
            }
        }
    }
    return <- res
}

//! the kernel cells' approx bar with a poison past the expectation's largest magnitude, outside the bar at any scale
let private BAR_APPROX_CTRL = Bar(rel = APPROX_REL, poison_of_max = true, poison_abs = 0.5)

//! a kept stamp's GLSL.std.450 Tanh instructions (x) and those of them whose operand is an FClamp's result (y)
def private kept_tanh_reads(kernel : string) : int2 {
    var res = int2(0, 0)
    static_if (typeinfo builtin_module_exists(vulkan)) {
        g_vkd_spv_kept |> get(kernel) $(words) {
            var clamps : table<uint>
            foreach_inst(words) $(op : uint; w : array<uint>; s, n : int) {
                if (op == uint(SpvOp.ExtInst) && n >= 5 && w[s + 3] == uint(GLSLstd450.FClamp)) {
                    clamps |> insert(w[s + 1])
                }
            }
            foreach_inst(words) $(op : uint; w : array<uint>; s, n : int) {
                if (op == uint(SpvOp.ExtInst) && n >= 5 && w[s + 3] == uint(GLSLstd450.Tanh)) {
                    res.x++
                    if (key_exists(clamps, w[s + 4])) {
                        res.y++
                    }
                }
            }
            delete clamps
        }
    }
    return res
}

//! whether the flash stamp ladder refuses the stamp: the ensure ladder, or the encode ladder when `enc`
def private fa_ladder_panics(enc, khr : bool; hs : int64; gated, cap : bool; sink : bool = false) : bool {
    var tripped = false
    static_if (typeinfo builtin_module_exists(vulkan)) {
        try {
            if (enc) {
                var raw : VkCommandBuffer
                var h : VkHaz
                var s : VkDescriptorSet
                var pc : FaArgs
                fa_stamp_enc(khr, hs, gated, cap, sink, false, raw, h, s, pc, 1l)
            } else {
                fa_stamp_ensure(khr, hs, gated, cap, sink, false)
            }
        } recover {
            tripped = true
        }
    }
    return tripped
}

def private nonwritable_bindings(words : array<uint>) : table<int> {
    var res : table<int>
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var binding_of : table<uint; int>
        var nonwritable : table<uint>
        foreach_inst(words) $(op : uint; w : array<uint>; s, n : int) {
            if (op == uint(SpvOp.Decorate) && n == 3 && w[s + 1] == uint(SpvDecoration.Binding)) {
                binding_of[w[s]] = int(w[s + 2])
            } elif (op == uint(SpvOp.Decorate) && n == 2 && w[s + 1] == uint(SpvDecoration.NonWritable)) {
                nonwritable |> insert(w[s])
            }
        }
        for (id, b in keys(binding_of), values(binding_of)) {
            if (key_exists(nonwritable, id)) {
                res |> insert(b)
            }
        }
    }
    return <- res
}

[test]
def test_vkd_readonly_stamp(t0 : T?) {
    t0 |> run("the lens stamps NonWritable on every binding the kernel never writes, and on those alone") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            var ro <- nonwritable_bindings(vkd_toy_spv)
            t |> success(key_exists(ro, 0), "binding 0, read only, is NonWritable")
            t |> success(!key_exists(ro, 1), "binding 1, written, is not")
            t |> equal(1, length(ro), "one binding of the two is NonWritable")
            delete ro
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_fa_stamp_words(t0 : T?) {
    t0 |> run("the folded flash template's stamps: the KHR arm's shared-memory tiles exist on its stamps alone, and each arm asks for its own cooperative-matrix capability") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_tower_clamp_cvt_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            let cm2_on = g_gpu.has_coopmat2_fa
            let khr_on = khr_kq_tile_on()
            if (!cm2_on && !khr_on) {
                t |> skip("the flash stamps need cooperative matrices (NV_coopmat2 with the flash trio, or KHR coopmat at subgroup 32)")
                return
            }
            let head_sizes = fixed_array<int64>(64l, 128l, 256l, 512l)
            g_vkd_spv_keep = true
            vkd_pipes_rebuild()   // a stamp an earlier cell built rebuilds through the keeping seat; every dispatch here waits its fence, so the device is idle
            var cm2_ok = true
            var khr_ok = true
            for (hs in head_sizes) {
                cm2_ok = cm2_ok && (!cm2_on || fa_stamp_ensure(false, hs, false, false, false, false))
                khr_ok = khr_ok && (!khr_on || fa_stamp_ensure(true, hs, false, false, false, false))
            }
            g_vkd_spv_keep = false   // off before any assert, so a red cell keeps nothing past this one
            t |> success(cm2_ok, "every cm2 flash stamp engages on an fa-capable device")
            t |> success(khr_ok, "every KHR flash stamp engages on a coopmat device")
            for (hs in head_sizes) {
                if (cm2_on) {
                    var cm2 <- kept_stamp_shape("fa_cm2_h{hs}_cls_spv")
                    t |> equal(0, cm2.wg_vars, "the cm2 h{hs} stamp declares no Workgroup variable: the KHR arm's four tiles gate off it")
                    t |> success(has_nv_coopmat_cap(cm2.caps), "the cm2 h{hs} stamp asks for the NV cooperative-matrix capabilities")
                    delete cm2
                }
                if (khr_on) {
                    var khr <- kept_stamp_shape("fa_khr_h{hs}_cls_spv")
                    t |> equal(4, khr.wg_vars, "the KHR h{hs} stamp declares its four shared-memory tiles")
                    t |> success(key_exists(khr.caps, SPV_CAP_COOPMAT_KHR), "the KHR h{hs} stamp asks for the KHR cooperative-matrix capability")
                    t |> success(!has_nv_coopmat_cap(khr.caps), "the KHR h{hs} stamp asks for no NV cooperative-matrix capability: the cm2 typedefs it inherits emit nothing")
                    delete khr
                }
            }
            g_vkd_spv_kept |> clear()
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_fa_stamp_refusals(t0 : T?) {
    t0 |> run("the flash stamp ladders refuse a gated KHR stamp, a softcap or gated stamp off head size 256 and a head size no stamp serves, before any device work") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            for (enc in fixed_array(false, true)) {
                let ladder = enc ? "encode" : "ensure"
                t |> success(fa_ladder_panics(enc, true, 256l, true, false), "the KHR arm has no gated stamp: the {ladder} ladder refuses")
                t |> success(fa_ladder_panics(enc, false, 128l, false, true), "no softcap stamp off head size 256: the {ladder} ladder refuses")
                t |> success(fa_ladder_panics(enc, false, 128l, true, false), "no gated stamp off head size 256: the {ladder} ladder refuses")
                t |> success(fa_ladder_panics(enc, false, 96l, false, false), "a head size no stamp serves: the {ladder} ladder refuses")
            }
            t |> success(fa_ladder_panics(false, false, 128l, false, false, true), "no sink stamp off head size 64: the ensure ladder refuses")
            t |> success(fa_ladder_panics(false, false, 256l, false, true, true), "no sink stamp at the softcap's head size: the ensure ladder refuses")
            t |> success(fa_ladder_panics(true, true, 128l, false, false, true), "no sink stamp off head size 64: the encode ladder refuses")
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! spawns a compile of a one-class [vk_dispatch] program whose kernel writes its second binding; `ro` declares that binding @readonly - the shape the lens refuses
def private spawn_readonly_fixture(ro : bool; tag : string) : tuple<rc : int; out : string> {
    let roarg = ro ? "@readonly " : ""
    let src = join([
        "options gen2",
        "options _dasllama_internal = true",
        "",
        "require dasllama/dasllama_lint public",
        "require spirv/spirv_shader",
        "require vulkan",
        "require vulkan/vulkan_boost",
        "require dasllama/dasllama_vulkan_common",
        "require dasllama/dasllama_vulkan_dispatch",
        "",
        "struct RoArgs \{",
        "    n : uint",
        "\}",
        "",
        "[vk_dispatch(name = \"ro_fixture\", grid = \"n/64\", params = \"n : int64\")]",
        "class RoFixture \{",
        "    @ssbo @binding = 0 xs : array<float>",
        "    @ssbo @binding = 1 {roarg}ys : array<float>",
        "    @push_constant pa : RoArgs",
        "",
        "    [spirv_kernel(local_size_x = 64, name = \"ro_fixture_spv\")]",
        "    def run \{",
        "        let i = gl_GlobalInvocationID.x",
        "        if (i < pa.n) \{",
        "            ys[i] = xs[i] * 2.0",
        "        \}",
        "    \}",
        "\}",
        "",
        "[export]",
        "def main() \{",
        "    print(\"ok\\n\")",
        "\}"], "\n")
    return spawn_compile_only(src, "vk_lens_{tag}", 180.0)
}

[test]
def test_vkd_lens_readonly_gate(t0 : T?) {
    t0 |> run("[vk_dispatch] refuses @readonly on a binding a kernel writes and compiles the plain twin") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let bad = spawn_readonly_fixture(true, "bad")
            t |> success(bad.rc != 0, "a written @readonly binding must not compile")
            t |> success(find(bad.out, "is @readonly but a kernel of RoFixture writes binding 1") >= 0, "the refusal names the member and the binding: {bad.out}")
            let good = spawn_readonly_fixture(false, "ok")
            t |> equal(0, good.rc, "the twin without the qualifier compiles: {good.out}")
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! a two-kernel class whose `[vk_dispatch]` names its kernel by `kern`: a `*` prefix picks exactly one method or the lens refuses
def private spawn_kernel_prefix_fixture(kern, tag : string) : tuple<rc : int; out : string> {
    let src = join([
        "options gen2",
        "options _dasllama_internal = true",
        "",
        "require dasllama/dasllama_lint public",
        "require spirv/spirv_shader",
        "require vulkan",
        "require vulkan/vulkan_boost",
        "require dasllama/dasllama_vulkan_common",
        "require dasllama/dasllama_vulkan_dispatch",
        "",
        "struct PfArgs \{",
        "    n : uint",
        "\}",
        "",
        "[vk_dispatch(name = \"pf_fixture\", kernel = \"{kern}\", grid = \"n/64\", params = \"n : int64\")]",
        "class PfFixture \{",
        "    @ssbo @binding = 0 xs : array<float>",
        "    @ssbo @binding = 1 ys : array<float>",
        "    @push_constant pa : PfArgs",
        "",
        "    [spirv_kernel(local_size_x = 64, name = \"pf_fixture_a_spv\")]",
        "    def run_a \{",
        "        let i = gl_GlobalInvocationID.x",
        "        if (i < pa.n) \{",
        "            ys[i] = xs[i] * 2.0",
        "        \}",
        "    \}",
        "",
        "    [spirv_kernel(local_size_x = 64, name = \"pf_fixture_b_spv\")]",
        "    def run_b \{",
        "        let i = gl_GlobalInvocationID.x",
        "        if (i < pa.n) \{",
        "            ys[i] = xs[i] * 3.0",
        "        \}",
        "    \}",
        "\}",
        "",
        "[export]",
        "def main() \{",
        "    print(\"ok\\n\")",
        "\}"], "\n")
    return spawn_compile_only(src, "vk_prefix_{tag}", 180.0)
}

[test]
def test_vkd_lens_kernel_prefix_gate(t0 : T?) {
    t0 |> run("[vk_dispatch] refuses a kernel= prefix that names two methods and compiles the exact twin") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let bad = spawn_kernel_prefix_fixture("run_*", "bad")
            t |> success(bad.rc != 0, "a prefix naming two [spirv_kernel] methods must not compile")
            t |> success(find(bad.out, "names 2 [spirv_kernel] methods") >= 0, "the refusal counts the matches: {bad.out}")
            let good = spawn_kernel_prefix_fixture("run_a", "ok")
            t |> equal(0, good.rc, "the exact name compiles: {good.out}")
        } else {
            t |> skip("dasVulkan not present")
        }
    }
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
                    var header <- copy_span(cp |> plane_whole(ys[arm]), 0, ybase)
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

[test]
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

[test]
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

//! the roster entry whose name starts with `prefix`, as arming reads it; generic so a build without the vulkan module never infers it
def private roster_on(roster; prefix : string) : bool {
    for (e in roster) {
        if (e.name |> starts_with(prefix)) {
            return e.present && !e.off
        }
    }
    panic("no device-init roster entry starts with '{prefix}'")
    return false
}

[test, arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-extension-roster")]
def test_vkd_ext_roster(t0 : T?) {
    t0 |> run("the device-init roster: one entry per capability, each saying what rides on it, agreeing with the arming") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            var inscope roster <- vk_ext_roster(g_gpu.phys)
            t |> success(length(roster) >= 12, "the roster names the tier's capabilities ({length(roster)} entries)")
            var seen : table<string>
            for (e in roster) {
                t |> success(!key_exists(seen, e.name), "a roster entry is named once: {e.name}")
                seen |> insert(e.name)
                t |> success(!empty(e.rides), "a roster entry says what rides on it: {e.name}")
                t |> success(empty(e.knob) || (e.knob |> starts_with("DASLLAMA_VK_")) || e.knob == "DASLLAMA_COOPMAT", "a roster knob is a Vulkan-tier env knob: {e.name} -> '{e.knob}'")
                t |> success(!e.off || !empty(e.knob), "a route is held off only by a named knob: {e.name}")
            }
            t |> equal(g_gpu.has_coopmat, roster_on(roster, "VK_KHR_cooperative_matrix"), "has_coopmat follows the VK_KHR_cooperative_matrix entry")
            t |> equal(g_gpu.has_coopmat2, roster_on(roster, "VK_NV_cooperative_matrix2 (workgroup"), "has_coopmat2 follows the VK_NV_cooperative_matrix2 entry")
            t |> equal(g_gpu.has_coopmat2_fa, roster_on(roster, "VK_NV_cooperative_matrix2 reductions"), "has_coopmat2_fa follows the cm2 flash-attention entry")
            t |> equal(g_gpu.decvec_on, roster_on(roster, "VK_NV_cooperative_matrix_decode_vector"), "decvec_on follows the decode-vector entry and its knob")
            t |> equal(g_gpu.sm_count > 0, roster_on(roster, "VK_NV_shader_sm_builtins"), "the SM count follows the VK_NV_shader_sm_builtins entry")
            t |> equal(g_gpu.full_sg_on, roster_on(roster, "computeFullSubgroups"), "full_sg_on follows the computeFullSubgroups entry and its knob")
            t |> equal(g_gpu.has_memprio, g_gpu.has_coopmat && roster_on(roster, "VK_EXT_memory_priority"), "has_memprio follows the VK_EXT_memory_priority entry and its knob on a coopmat device")
            t |> equal(g_gpu.rebar_type >= 0, roster_on(roster, "a ReBAR memory type"), "the ReBAR type follows the ReBAR entry and its knob")
            t |> equal(g_gpu.dn_shared_ok, roster_on(roster, "maxComputeSharedMemorySize"), "dn_shared_ok follows the shared-memory limit entry")
            t |> success(g_gpu.xfer_fam < 0 || roster_on(roster, "timelineSemaphore"), "an armed transfer queue rides the timelineSemaphore entry")
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test, arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-decode-16bit-lanes")]
def test_vkd_direct_decode(t0 : T?) {
    t0 |> run("a [spirv_decode] method called directly from the kernel body == the same method on the CPU") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_vkd_direct_decode()) {
                t |> skip("no Vulkan device")
                return
            }
            let n = 64   //! two q8-shaped blocks of 32 quants
            var k = new VkdDirectDecode()
            k.wq |> resize(2)
            k.ws <- [float16(0.5), float16(-0.25)]
            k.y |> resize(n)
            k.pa = VdArgs(n = uint(n))
            for (b in range(2)) {
                for (i in range(16)) {
                    let q0 = (b * 32 + i * 2) % 23 - 11
                    let q1 = (b * 32 + i * 2 + 1) % 23 - 11
                    k.wq[b].qs[i] = int16((q0 & 0xFF) | ((q1 & 0xFF) << 8))
                }
            }
            for (i in range(n)) {
                gl_GlobalInvocationID = uint3(uint(i), 0u, 0u)
                k->run()
            }
            var gpu : array<float>
            with_cell_planes() $(var cp : CellPlanes) {
                let pwq = cp |> in_plain(k.wq)
                let pws = cp |> in_plain(k.ws)
                let py = cp |> out_(n)
                var s = cp |> cls_set(fixed_array(pwq, pws, py), @@set_vkd_direct_decode)
                var pc = VdArgs(n = uint(n))
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_vkd_direct_decode(raw, h, s, pc, 1l)
                }
                delete gpu
                gpu <- cp |> plane_run(py)
            }
            var nonzero = 0
            for (v in k.y) {
                nonzero += v != 0.0 ? 1 : 0
            }
            t |> success(nonzero > 48, "the oracle is non-trivial ({nonzero} of {n} lanes non-zero)")
            check_exact(t, "GPU vs CPU on all {n} lanes through the direct decode call", gpu, k.y)
            delete gpu
            unsafe {
                delete k
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_ar_class(t0 : T?) {
    t0 |> run("ar_add_rms class twin == the vk_add_rms seam (bit-exact, same reduce order)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_cls_ar()) {
                t |> skip("no Vulkan device")
                return
            }
            let dim = 1024
            let eps = 1e-5
            let ascale = 1.44
            var fx <- ar_fixture(dim, dim)
            var x <- fx.x
            var a <- fx.a
            var w <- fx.w
            // the seam is the reference: the SHIPPED kernel on the same device, same inputs
            var x2 := x
            var y2 : array<float>
            y2 |> resize(dim)
            vk_add_rms(x2, a, w, y2, int64(dim), eps, ascale, true)
            // the residual plane is IN-PLACE, so it cannot hold a sentinel — this liveness check
            // is its non-vacuity witness (add_on with a[0] = -3.0 must change x2)
            t |> success(x2[0] != x[0], "the seam wrote the residual stream")
            // the class twin through the generated surface
            var r <- cls_ar_run(x, a, w, ArArgs(dim = uint(dim), add_on = 1u, eps = eps, ascale = ascale), 1)
            var yc <- r.y
            var xc <- r.x
            var bad_y = 0
            var bad_x = 0
            for (i in range(dim)) {
                if (yc[i] != y2[i]) {
                    bad_y++
                }
                if (xc[i] != x2[i]) {
                    bad_x++
                }
            }
            t |> success(bad_y == 0, "normed output bit-exact vs the seam ({bad_y} of {dim} diverge)")
            t |> success(bad_x == 0, "residual stream bit-exact vs the seam ({bad_x} of {dim} diverge)")
            //! the biased add partner (gpt-oss's attention output bias): the bias row rides the norms plane past the norm row, the
            //! kernel adds it to the partner before the add; the seam fed the pre-added partner is the bit-exact reference
            {
                var b <- row_fill(dim, 23, 0, 0.09, 3.1, 71)
                var ab <- [for (i in range(dim)); a[i] + b[i]]
                var wb <- biased_norms(w, b)
                var x3 := x
                var y3 : array<float>
                y3 |> resize(dim)
                vk_add_rms(x3, ab, w, y3, int64(dim), eps, ascale, true)
                var r3 <- cls_ar_run(x, a, wb, ArArgs(dim = uint(dim), add_on = 1u, eps = eps, ascale = ascale, abias_on = 1u, aboff = uint(dim)), 1)
                var ycb <- r3.y
                var xcb <- r3.x
                var bad_yb = 0
                var bad_xb = 0
                var moved = 0
                for (i in range(dim)) {
                    if (ycb[i] != y3[i]) {
                        bad_yb++
                    }
                    if (xcb[i] != x3[i]) {
                        bad_xb++
                    }
                    if (xcb[i] != x2[i]) {
                        moved++
                    }
                }
                t |> success(bad_yb == 0, "biased partner: the normed output is bit-exact vs the seam fed the pre-added partner ({bad_yb} of {dim} diverge)")
                t |> success(bad_xb == 0, "biased partner: the residual stream is bit-exact vs that seam ({bad_xb} of {dim} diverge)")
                t |> success(moved > 0, "biased partner control: the bias moves the residual off the unbiased step ({moved} of {dim})")
                //! the CPU oracle beside the seam: x' = (x + a + b) * ascale, y = w * x' * rsqrt(mean(x'^2) + eps)
                var x_cpu, y_cpu : array<float>
                x_cpu |> resize(dim)
                y_cpu |> resize(dim)
                for (i in range(dim)) {
                    x_cpu[i] = (x[i] + ab[i]) * ascale
                }
                rmsnorm(y_cpu, x_cpu, w, int64(dim), eps)
                t |> success(mismatch_approx(ycb, y_cpu) == 0 && mismatch_approx(xcb, x_cpu) == 0, "biased partner: the normed output and the residual match the CPU oracle ({mismatch_approx(ycb, y_cpu)} / {mismatch_approx(xcb, x_cpu)} of {dim} off)")
                y_cpu[2] += 0.5
                t |> success(mismatch_approx(ycb, y_cpu) == 1, "biased partner control: the CPU bar reds one poisoned element")
                delete ycb
                delete xcb
                delete x_cpu
                delete y_cpu
                delete b
                delete ab
                delete wb
                delete x3
                delete y3
            }
            delete x
            delete a
            delete w
            delete x2
            delete y2
            delete yc
            delete xc
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the residual-step fixture's rows: `n` residual and partner elements (whole rows of `dim`) on two modular ramps scaled by `xs`
//! and `as_`, offset by `xo` and `ao`, and the `dim`-wide norm row 0.5 up on a ramp of step `ws`
def private ar_fixture(n, dim : int; xs : float = 0.11; xo : float = 5.0; as_ : float = 0.07; ao : float = 3.0; ws : float = 0.05) : tuple<x : array<float>; a : array<float>; w : array<float>> {
    var x <- row_fill(n, 37, 0, xs, xo, 101)
    var a <- row_fill(n, 13, 0, as_, ao, 89)
    var w <- row_fill(dim, 1, 0, ws, -0.5, 17)
    return <- (x = x, a = a, w = w)
}

//! the norms plane of a step with an output bias: the norm row `w`, then the bias row `b` past it
def private biased_norms(w, b : array<float>) : array<float> {
    var wb := w
    wb |> push_from(b)
    return <- wb
}

//! a fused residual + requant stamp's planes back: the quants as words, the scales, the residual row and the normed row
struct private ArRqOut {
    q : array<uint>
    s : array<float>
    x : array<float>
    y : array<float>
}

//! one fused residual + requant stamp (`name`: cls_ar_rq_b, or the row-storing / Q8_K forms cls_ar_rqx, cls_ar_rqk,
//! cls_ar_rqkx) over the residual `x` (written in place), the partner `a` and the norms plane `w`: the quants under -128, the
//! scales - a superblock's under `kform` - and the stored row under the NaN sentinel
def private ar_rq_run(name : string; x, a, w : array<float>; pc; kform, storex : bool) : ArRqOut {
    var r : ArRqOut
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let dim = length(x)
        with_cell_planes() $(var cp : CellPlanes) {
            let px = cp |> io_(x)
            let pa = cp |> in_(a)
            let pw = cp |> in_(w)
            let pq = cp |> out_words(int64(dim), I8_SENTINEL_BITS)
            let ps = cp |> out_(kform ? dim / int(KQ_SB_ELEMS) : dim / int(KQ_B32_ELEMS))
            let py = cp |> out_(storex ? dim : 1)
            var args = pc
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                if (name == "cls_ar_rq_b") {
                    var s = cp |> cls_set(fixed_array(px, pa, pw, pq, ps), @@set_cls_ar_rq_b)
                    enc_cls_ar_rq_b(raw, h, s, args, 1l)
                } elif (name == "cls_ar_rqx") {
                    var s = cp |> cls_set(fixed_array(px, pa, pw, pq, ps, py), @@set_cls_ar_rqx)
                    enc_cls_ar_rqx(raw, h, s, args, 1l)
                } elif (name == "cls_ar_rqk") {
                    var s = cp |> cls_set(fixed_array(px, pa, pw, pq, ps), @@set_cls_ar_rqk)
                    enc_cls_ar_rqk(raw, h, s, args, 1l)
                } else {
                    var s = cp |> cls_set(fixed_array(px, pa, pw, pq, ps, py), @@set_cls_ar_rqkx)
                    enc_cls_ar_rqkx(raw, h, s, args, 1l)
                }
            }
            r.q <- cp |> plane_as(pq, type<uint>, dim / 4)
            r.s <- cp |> plane_run(ps)
            r.x <- cp |> plane_run(px)
            r.y <- cp |> plane_run(py)
        }
    }
    return <- r
}

//! one cls_ar dispatch over `nrows` rows (`pc` the class's ArArgs): the norm plane `w` whole, the residual `x` read and written in
//! place, y under the -999.25 sentinel - a GPU-vs-GPU compare is vacuous-prone, two dead kernels leave two matching zeroed
//! planes, and no real computation lands the sentinel; y and x come back
def private cls_ar_run(x, a, w : array<float>; pc; nrows : int) : tuple<y : array<float>; x : array<float>> {
    var y_out, x_out : array<float>
    static_if (typeinfo builtin_module_exists(vulkan)) {
        with_cell_planes() $(var cp : CellPlanes) {
            let px = cp |> io_(x)
            let pa = cp |> in_(a)
            let pw = cp |> in_(w)
            let py = cp |> out_words(long_length(x) * 4l, float_bits_to_uint(-999.25))
            var s = cp |> cls_set(fixed_array(px, pa, pw, py), @@set_cls_ar)
            var args = pc
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_cls_ar(raw, h, s, args, int64(nrows))
            }
            delete y_out
            delete x_out
            y_out <- cp |> plane_as(py, type<float>, length(x))
            x_out <- cp |> plane_run(px)
        }
    }
    return <- (y = y_out, x = x_out)
}

//! the classifier epilogue's rows form: `rows` logits rows through one dispatch of `rows` (the whole plane), or a
//! dispatch of one row `rows` times - the row-at-a-time form is the rows form at one row
def private cls_epi_rows_run(y : array<float>; supp : array<uint>; vocab, rows : int64; cap : float; a_row_at_a_time : bool) : array<float> {
    var yc : array<float>
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let per = a_row_at_a_time ? 1l : rows   //! rows a dispatch
        var r0 = 0l
        while (r0 < rows) {
            with_cell_planes() $(var cp : CellPlanes) {
                let py = cp |> io_(copy_span(y, r0 * vocab, per * vocab))
                let ps = cp |> in_plain(supp)
                var s = cp |> cls_set(fixed_array(py, ps), @@set_cls_epi)
                var pc = ClsEpiArgs(vocab = uint(vocab), cap = cap, nsupp = uint(length(supp)), rows = uint(per))
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_cls_epi(raw, h, s, pc, (vocab * per + 255l) / 256l)
                }
                yc |> push_from(cp |> plane_run(py))
            }
            r0 += per
        }
    }
    return <- yc
}

[test]
def test_vkd_cls_epi_rows(t0 : T?) {
    t0 |> run("cls_epi over four logits rows == the one-row dispatch a row at a time, and the CPU softcap with the pins") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_cls_epi()) {
                t |> skip("no Vulkan device")
                return
            }
            let vocab = 1000l   //! off the 256-lane grid, so the last workgroup's guard is exercised on every row
            let rows = 4l
            let cap = 30.0
            var supp <- [3u, 777u, 999u]
            var y <- row_fill(int(vocab * rows), 37, 0, 0.7, 60.0, 211)   //! reaches past the cap on both sides
            var want : array<float>   // the CPU form: the cap in the exp form the kernel takes, then the pins
            want |> resize(vocab * rows)
            for (i in range64(vocab * rows)) {
                var v = softcap_exp(y[i], cap)
                let id = uint(i % vocab)
                for (s in supp) {
                    if (s == id) {
                        v = -1.0e30
                    }
                }
                want[i] = v
            }
            var four <- cls_epi_rows_run(y, supp, vocab, rows, cap, false)
            var one <- cls_epi_rows_run(y, supp, vocab, rows, cap, true)
            var bad = 0
            var over = 0
            var over_poison = 0
            var pinned = 0
            var maxdiff = 0.0
            var worst = 0l
            let bar = 1e-4
            for (i in range64(vocab * rows)) {
                if (four[i] != one[i]) {
                    bad++
                }
                if (want[i] <= -1.0e29) {
                    if (four[i] <= -1.0e29) {
                        pinned++
                    }
                } else {
                    let d = abs(four[i] - want[i])
                    if (d > maxdiff) {
                        maxdiff = d
                        worst = i
                    }
                    if (!(d <= bar)) {
                        over++
                    }
                    if (abs(four[i] - (want[i] + 0.01)) > bar) {
                        over_poison++
                    }
                }
            }
            t |> success(bad == 0, "the four-row dispatch is the one-row dispatch a row at a time, bit for bit ({bad} of {vocab * rows} diverge)")
            t |> success(over == 0, "every capped logit within {bar} of the CPU softcap ({over} over; max difference {maxdiff} at row {worst / vocab} id {worst % vocab})")
            t |> success(over_poison > 0, "the bar is live: an added 0.01 lands outside it ({over_poison} over)")
            t |> equal(pinned, int(rows) * length(supp), "every suppressed id pinned on every row")
            delete four
            delete one
            delete want
            delete y
            delete supp
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the device pick of every row of a [rows x vocab] plane: the first maximum's id a row, as the host reads it
def private cls_argmax_run(y : array<float>; vocab, rows : int64) : array<uint> {
    var ids : array<uint>
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let nchunk = int64(CLS_ARGMAX_CHUNKS)
        with_cell_planes() $(var cp : CellPlanes) {
            let py = cp |> in_(y)
            let pm = cp |> out_(int(rows * nchunk))   //! the partials the second pass reads: an unwritten slot must be a sentinel, not a stale max
            let pi = cp |> out_(int(rows * nchunk))
            let pd = cp |> out_(int(rows))
            var sp = cp |> cls_set(fixed_array(py, pm, pi), @@set_cls_argmax_part)
            var sf = cp |> cls_set(fixed_array(pm, pi, pd), @@set_cls_argmax_fin)
            var pc = ArgmaxArgs(vocab = uint(vocab))
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_cls_argmax_part(raw, h, sp, pc, rows * nchunk)
                enc_cls_argmax_fin(raw, h, sf, pc, rows)
            }
            ids <- cp |> plane_as(pd, type<uint>, int(rows))
        }
    }
    return <- ids
}

//! the host's reading of the same plane: the first maximum a row, the walk `parallel_argmax` runs - which splits a row this
//! wide across a job queue, and this model-free file opens none
def private first_max_rows(y : array<float>; vocab, rows : int64) : array<uint> {
    var ids : array<uint>
    ids |> resize(rows)
    for (r in range64(rows)) {
        var mi = 0l
        for (i in range64(1l, vocab)) {
            if (y[r * vocab + i] > y[r * vocab + mi]) {
                mi = i
            }
        }
        ids[r] = uint(mi)
    }
    return <- ids
}

[test]
def test_vkd_cls_argmax(t0 : T?) {
    t0 |> run("cls_argmax: every row's pick is the first maximum's id through the two passes, at a 262144 and a 1000 vocab, ties, an all-equal row, a row with NaN and -inf lanes and an all-NaN row included") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_cls_argmax_part() || !ensure_cls_argmax_fin()) {
                t |> skip("no Vulkan device")
                return
            }
            let nan = unsafe(reinterpret<float>(0x7FC00000u))
            let ninf = unsafe(reinterpret<float>(0xFF800000u))
            for (vocab in fixed_array(262144l, 1000l)) {
                let rows = 6l
                var y : array<float>
                y |> resize(vocab * rows)
                for (i in range64(vocab * rows)) {
                    y[i] = hash_unit(uint(i) * 7u + 3u) * 20.0
                }
                //! row 1's maximum sits twice: the lower id must win, as the host's first-maximum walk reads
                let hi = 50.0   //! past the plane's range (hash_unit spans [-2, 2), scaled by 20)
                y[vocab + 900l] = hi
                y[vocab + 37l] = hi
                y[2l * vocab + vocab - 1l] = hi   //! row 2's at the last id
                for (i in range64(vocab)) {   //! row 0 pinned whole (every id suppressed): the first id wins, as the host's walk reads
                    y[i] = -1.0e30
                }
                //! row 4: NaN lanes below and above the maximum and a -inf lane, the widest finite lane at 123 must win; row 5: every lane NaN
                y[4l * vocab + 5l] = nan
                y[4l * vocab + 123l] = hi
                y[4l * vocab + 700l] = nan
                y[4l * vocab + 701l] = ninf
                for (i in range64(vocab)) {
                    y[5l * vocab + i] = nan
                }
                var got <- cls_argmax_run(y, vocab, rows)
                var want <- first_max_rows(y, vocab, rows)
                let bad = mismatch_exact(got, want)
                to_log(LOG_INFO, "cls_argmax at vocab {vocab}: device picks {got}, the host's first maxima {want} ({bad} of {rows} rows off)\n")
                t |> success(bad == 0, "vocab {vocab}: every row's device pick is the host's first maximum ({bad} of {rows} rows off; row 1 {got[1]} of {want[1]}, row 2 {got[2]} of {want[2]}, row 4 {got[4]} of {want[4]}, row 5 {got[5]} of {want[5]})")
                t |> equal(int64(want[1]), 37l, "vocab {vocab}: the tie lands on the lower id")
                t |> equal(int64(got[0]), 0l, "vocab {vocab}: the all-equal row lands id 0")
                t |> equal(int64(got[4]), 123l, "vocab {vocab}: the row with NaN and -inf lanes lands its widest finite lane")
                t |> equal(int64(got[5]), 0l, "vocab {vocab}: the all-NaN row lands id 0, never the empty-slice sentinel 0xFFFFFFFF ({got[5]})")
                want[3] = want[3] == 0u ? 1u : 0u   //! the poison: a wrong id on one row must red the compare
                t |> equal(mismatch_exact(got, want), 1, "vocab {vocab}: the pick compare reds one poisoned row")
                delete got
                delete want
                delete y
            }
        } else {
            t |> skip("no vulkan module")
        }
    }
}

[test]
def test_vkd_cls_ar_row0(t0 : T?) {
    t0 |> run("cls_ar with a row base lands on the rows from row0 and leaves the rows below untouched") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_cls_ar()) {
                t |> skip("no Vulkan device, or the cls_ar pipeline failed to build")
                return
            }
            let dim = 1024
            let rows = 3
            let row0 = 1
            let total = dim * rows
            let eps = 1e-5
            var fx <- ar_fixture(total, dim)
            var x <- fx.x
            var a <- fx.a
            var w <- fx.w
            // the reference: the seam row by row from row0; rows below keep x and a sentinel y
            var x2 := x
            var y2 : array<float>
            y2 |> resize(total)
            for (i in range(total)) {
                y2[i] = -999.25
            }
            for (r in range(row0, rows)) {
                var xr, ar, yr : array<float>
                xr |> resize(dim)
                ar |> resize(dim)
                yr |> resize(dim)
                for (i in range(dim)) {
                    xr[i] = x[r * dim + i]
                    ar[i] = a[r * dim + i]
                }
                vk_add_rms(xr, ar, w, yr, int64(dim), eps, 1.0, true)
                for (i in range(dim)) {
                    x2[r * dim + i] = xr[i]
                    y2[r * dim + i] = yr[i]
                }
                delete xr
                delete ar
                delete yr
            }
            var r <- cls_ar_run(x, a, w, ArArgs(dim = uint(dim), add_on = 1u, eps = eps, ascale = 1.0, row0 = uint(row0)), rows - row0)
            let bad_y = mismatch_exact(r.y, y2)
            let bad_x = mismatch_exact(r.x, x2)
            t |> success(bad_y == 0, "normed rows from row0 bit-exact, the row below still the sentinel ({bad_y} of {total} diverge)")
            t |> success(bad_x == 0, "residual rows from row0 bit-exact, the row below untouched ({bad_x} of {total} diverge)")
            delete x
            delete a
            delete w
            delete x2
            delete y2
            delete r
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_actf16_base(t0 : T?) {
    t0 |> run("actf16 with an element base == the same row dispatched from zero; the halves below the base keep their sentinel") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_actf16_cls()) {
                t |> skip("no Vulkan device, or the actf16 pipeline failed to build")
                return
            }
            let nelem = 2048
            let total = nelem * 2
            var gh <- row_fill(total, 29, 0, 0.06, 1.9, 61)
            var uh <- row_fill(total, 43, 0, 0.05, 2.1, 83)
            // arm 0: the second row's data at the front, base 0; arm 1: the plane as is, base nelem
            var g0, u0 : array<float>
            g0 |> resize(total)
            u0 |> resize(total)
            for (i in range(nelem)) {
                g0[i] = gh[nelem + i]
                u0[i] = uh[nelem + i]
            }
            var h0 <- actf16_halves(g0, u0, nelem, 0)
            var h1 <- actf16_halves(gh, uh, nelem, nelem)
            var bad_row = 0
            var bad_sent = 0
            for (i in range(nelem / 2)) {
                if (h1[nelem / 2 + i] != h0[i]) {
                    bad_row++
                }
                if (h1[i] != 0xFC00FC00u) {
                    bad_sent++
                }
            }
            t |> success(bad_row == 0, "the based row equals the same row dispatched from zero ({bad_row} of {nelem / 2} words diverge)")
            t |> success(bad_sent == 0, "the halves below the base keep the sentinel ({bad_sent} of {nelem / 2} words written)")
            delete gh
            delete uh
            delete g0
            delete u0
            delete h0
            delete h1
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_act_gelu_past_clamp(t0 : T?) {
    t0 |> run("the tanh GELU act on gate values past the tanh clamp (|v| to 300) == the CPU oracle, every half finite") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_actf16_cls()) {
                t |> skip("no Vulkan device, or the actf16 pipeline failed to build")
                return
            }
            let nelem = 1024
            let big <- [12.0, -12.0, 20.0, -20.0, 60.0, -60.0, 300.0, -300.0]
            var gh <- row_fill(nelem, 29, 0, 0.06, 1.9, 61)
            for (i in range(nelem / 8)) {
                gh[i * 8] = big[i % 8]
            }
            var uh <- row_fill(nelem, 43, 0, 0.05, 2.1, 83)
            let past = length([for (v in gh); v; where abs(v) > 10.0])
            t |> success(past == nelem / 8, "the fixture puts {nelem / 8} gate values past the clamp's reach")
            var h_cls <- actf16_halves(gh, uh, nelem, 0)
            var got <- [for (i in range(nelem)); half_at(h_cls, i)]
            var want <- [for (i in range(nelem)); act_mul_ref(gh[i], uh[i], true)]
            t |> success(all_finite(got), "every half the act landed is finite")
            check_rows(t, "actf16 gelu past the clamp", got, want, BAR_APPROX, FLAT_ROWS)
            g_vkd_spv_keep = true
            vkd_pipes_rebuild()
            let built = ensure_actf16_cls()
            g_vkd_spv_keep = false
            t |> success(built, "the actf16 stamp rebuilds through the keeping seat")
            let tanh = kept_tanh_reads("actf16_cls_spv")
            t |> success(tanh.x > 0 && tanh.x == tanh.y, "every tanh in the actf16 stamp reads a clamp's result ({tanh.y} of {tanh.x}): a device tanh built from exp answers NaN past +-15")
            g_vkd_spv_kept |> clear()
            delete gh
            delete uh
            delete h_cls
            delete got
            delete want
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! one actf16 dispatch of `nelem` elements from element `elem0` of `g` and `u`: the whole half plane back, every half it was
//! not written left at -inf
def private actf16_halves(g, u : array<float>; nelem, elem0 : int) : array<uint> {
    var halves : array<uint>
    static_if (typeinfo builtin_module_exists(vulkan)) {
        with_cell_planes() $(var cp : CellPlanes) {
            let pg = cp |> in_(g)
            let pu = cp |> in_(u)
            let ph = cp |> out_words(long_length(g) * 2l, 0xFC00FC00u)
            var sc = cp |> cls_set(fixed_array(pg, pu, ph), @@set_actf16_cls)
            var pc = ActArgs(nelem = uint(nelem), act = ACT_GELU, nblk = 0u, elem0 = uint(elem0))
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_actf16_cls(raw, h, sc, pc, int64((nelem / 4 + 255) / 256))
            }
            halves <- cp |> plane_as(ph, type<uint>, length(g) / 2)
        }
    }
    return <- halves
}

// ===== the kq GEMV family: class kernels vs the CPU oracle, per format =====
// Synthetic planes — any bit pattern is valid K-quant data, EXCEPT the scale words, which are
// composed from finite halves so a NaN can't poison the compare.

def private safe_half(i : uint) : uint {
    let tab = fixed_array(0x3C00u, 0xB800u, 0x3400u, 0x4200u, 0x2E00u, 0xBC00u, 0x3E00u, 0xB000u)
    return tab[i % 8u]
}

def private ws_word(i : uint) : uint => safe_half(i * 7u + 3u) | (safe_half(i * 3u + 1u) << 16u)

//! `n` scale words `ws_word(i * mul + add)`: two finite halves a word
def private ws_plane(n : int; mul, add : uint) : array<uint> =>
    <- [for (i in range(n)); ws_word(uint(i) * mul + add)]

// ===== CPU oracles =====
// The reference side of every arm: plain sequential math (the Metal kernel-unit pattern), or the
// class's own per-block methods driven lane-free. Device subgroup/tile folds reassociate float
// sums, so float compares take the relative bar; integer packs and max folds are order-free and
// stay exact, except quants downstream of exp/tanh (GPU transcendentals differ by ulps — those
// compare as signed bytes within +-1).

// the dot-product cells' bar - 1e-3 of the larger of 1 and the reference - over n outputs from base
def private rel_mismatches(got, want : array<float>; base, n : int) : int {
    var bad = 0
    for (i in range(n)) {
        if (!(abs(got[base + i] - want[base + i]) <= 1e-3 * max(1.0, abs(want[base + i])))) {
            bad++
        }
    }
    return bad
}

// the beta/alpha planes' written positions alone: the words between the two planes stay untouched
def private ba_plane_mismatches(got, want : array<float>; npos, ne, nvh, ob_b, ob_g, ostride : int) : int {
    var bad = 0
    for (p in range(npos)) {
        for (e in range(ne)) {
            let at = (e < nvh ? ob_b : ob_g) + p * ostride + e % nvh
            if (!(abs(got[at] - want[at]) <= 1e-3 * max(1.0, abs(want[at])))) {
                bad++
            }
        }
    }
    return bad
}


//! the signed bytes of the packed quant word `g` outside +-tol of `w`'s (act chains carry transcendental ulp noise)
def private qword_off(g, w : uint; tol : int) : int {
    var bad = 0
    for (sh in range(4)) {
        let a = int(g >> uint(sh * 8)) & 0xFF
        let b = int(w >> uint(sh * 8)) & 0xFF
        let sa = a >= 128 ? a - 256 : a
        let sb = b >= 128 ? b - 256 : b
        if (!(abs(sa - sb) <= tol)) {
            bad++
        }
    }
    return bad
}

def private mismatch_qbytes(got, want : array<uint>; tol : int) : int {
    var bad = 0
    for (g, w in got, want) {
        bad += qword_off(g, w, tol)
    }
    return bad
}

//! the first word of `got` with a byte outside +-tol of `want`'s, -1 when none
def private first_qbytes_off(got, want : array<uint>; tol : int) : int {
    for (i in range(min(length(got), length(want)))) {
        return i if (qword_off(got[i], want[i], tol) > 0)
    }
    return -1
}

//! the quant words `got` within one step of `want`, byte for byte and of one length, and the control: `want`'s first byte three steps
//! off must red the same compare
def private check_qbytes(t : T?; tag : string; got, want : array<uint>) {
    let bad = mismatch_qbytes(got, want, 1)
    let verdict = "the quant bytes within one step ({bad} off, the first in word {first_qbytes_off(got, want, 1)}, {length(got)} words against {length(want)})"
    to_log(LOG_INFO, "{tag}: {verdict}\n")
    t |> success(bad == 0 && length(got) == length(want), "{tag}: {verdict}")
    var poisoned := want
    poisoned[0] = (poisoned[0] & ~0xFFu) | ((poisoned[0] + 3u) & 0xFFu)
    t |> success(mismatch_qbytes(got, poisoned, 1) > 0, "{tag}: the bar reds a quant byte three steps off")
    delete poisoned
}

//! the fourteen kq formats with a decode GEMV leaf and a batch tile, the order the family cells walk
let private KQ_LEAF_FMTS = fixed_array(int(KqFmt.k4), int(KqFmt.q40), int(KqFmt.k5), int(KqFmt.k6), int(KqFmt.iq4xs), int(KqFmt.k3), int(KqFmt.iq3s), int(KqFmt.iq3xxs), int(KqFmt.iq4nl), int(KqFmt.k2), int(KqFmt.iq2s), int(KqFmt.iq2xs), int(KqFmt.iq2xxs), int(KqFmt.iq3s4))

//! the family's class rails engage on the device: every leaf's pipeline builds, else the cell skips
def private kq_leaf_rails_engage(ensure : block<(fmt : int) : bool>) : bool {
    for (f in KQ_LEAF_FMTS) {
        if (!invoke(ensure, f)) return false
    }
    return true
}

//! a decode GEMV cell's host planes: the weight quants and scales, the region records, one activation row's quants and scales
struct private GemvFix {
    wqh, wsh, xqh : array<uint>
    xsh : array<float>
    regs : array<uint>
}

//! `nreg` regions of `d` rows, region r's weights at r x d x `units` units, the scale plane `wsh` moved in; the weight quants
//! `units` a row of `wq_words` words hashed at `i + wq_seed`, the activation row's `xunits` of `xq_words` at
//! `hash_word(i * xq_mul + xq_add)`, its scales `xs0 + (i % xs_mod) * xs_step`
def private gemv_fix(nreg, d, units, wq_words : int; wq_seed : uint; var wsh : array<uint>; xunits, xq_words : int; xq_mul, xq_add : uint;
                     xs0, xs_step : float; xs_mod : int) : GemvFix {
    var f <- GemvFix(wqh <- hashed_words(nreg * d * units * wq_words, 1u, wq_seed), wsh <- wsh,
        xqh <- hashed_words(xunits * xq_words, xq_mul, xq_add), xsh <- row_fill(xunits, 1, 0, xs_step, -xs0, xs_mod))
    f.regs |> resize(nreg * 2)
    for (r in range(nreg)) {
        f.regs[r * 2] = uint(r * d * units)
    }
    return <- f
}

let private GEMV_PLANES = fixed_array(0, 1, 2, 3, 4, 5)   //! a GEMV cell's plane ids in the classes' binding order

//! the fixture on a cell's planes in the GEMV classes' binding order - [w, ws, regions, xq, xs, y] at hazard bits 1 .. 32 - the `rows` outputs under the sentinel
def private gemv_cell_planes(var cp; f : GemvFix; rows : int) {
    cp |> in_plain(f.wqh, 1u)
    cp |> in_plain(f.wsh, 2u)
    cp |> in_plain(f.regs, 4u)
    cp |> in_plain(f.xqh, 8u)
    cp |> in_plain(f.xsh, 16u)
    cp |> out_(rows, 0, 0, 32u)
}

//! a fused gate + up cell's region records over its two stacks: `nreg` slots of `d` rows `units` units wide, the gate stack's
//! regions in `g`, the up stack's - past every gate row - in `u`
def private gu_regions(nreg, d, units : int) : tuple<g : array<uint>; u : array<uint>> {
    var g <- [for (i in range(nreg * 2)); i % 2 == 0 ? uint((i / 2) * d * units) : 0u]
    var u <- [for (i in range(nreg * 2)); i % 2 == 0 ? uint((nreg + i / 2) * d * units) : 0u]
    return <- (g = g, u = u)
}

//! a fused gate + up cell's hidden rows - the quants `oq` and the scales `os`, `nblk` Q8_0 blocks - against the activated
//! reference `act`: every scale, every dequantized value within a quant step, and the poisoned oracle element of every block
//! missing (`q8_blocks_off`'s control)
def private gu_blocks_hold(t : T?; tag : string; oq : array<int8>; os, act : array<float>; nblk : int) {
    let off = q8_blocks_off(oq, os, act, nblk)
    t |> success(off.scales == 0, "{tag}: the block scales match ({off.scales} of {nblk} off)")
    let verdict = "the dequantized hidden rows sit within a quant step ({off.values} of {nblk * 32} off; the largest {off.worst} steps at element {off.worst_at})"
    to_log(LOG_INFO, "{tag}: {verdict}\n")
    t |> success(off.values == 0, "{tag}: {verdict}")
    t |> success(off.poison_inside == 0, "{tag}: the bar reds a poisoned oracle element in every block ({off.poison_inside} passed)")
}

//! a per-32 format's decode GEMV - one lane a block over Q8_0 activations, `d` rows in each of two regions - at each width
//! in `ns`, against the scalar dot's float order (`row_oracle` a row) with its poisoned element; the weight quants `wq_words`
//! a block hashed at `i + wq_seed`, `scales` the format's scale plane over a block count; q51 also folds the two regions
//! under their weights inside the summed twin
def private per32_gemv_cell(t : T?; fmt : KqFmt; ns : array<int>; wq_words : int; wq_seed, xq_mul, xq_add : uint; row_oracle;
                            scales : block<(totblk : int) : array<uint>>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        for (n in ns) {
            let nb = n / 32
            let d = 3
            let nreg = 2
            let rows = nreg * d
            var wsh <- invoke(scales, rows * nb)
            var f <- gemv_fix(nreg, d, nb, wq_words, wq_seed, wsh, nb, 8, xq_mul, xq_add, 0.25, 0.125, 5)
            var y_ref : array<float>
            y_ref |> resize(rows)
            for (r in range(nreg)) {
                for (row in range(d)) {
                    y_ref[r * d + row] = invoke(row_oracle, f.wqh, f.wsh, f.xqh, f.xsh, r * d * nb + row * nb, nb)
                }
            }
            with_cell_planes() $(var cp : CellPlanes) {
                gemv_cell_planes(cp, f, rows)
                var s_cls = gemv_cls_set(int(fmt), gather_planes(cp.dev, GEMV_PLANES), gather_planes(cp.bytes, GEMV_PLANES), gather_planes(cp.hazard_bit, GEMV_PLANES))
                var pc = GemvArgs(n = uint(n), d = uint(d), nreg = uint(nreg), ybase = 0u)
                cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                    gemv_enc(int(fmt), raw2, h2, s_cls, pc, int64(rows))
                }
                var y_cls <- plane_run(cp, 5)
                check_rows(t, "{fmt} GEMV at n {n} against the scalar dot", y_cls, y_ref, BAR_APPROX_CTRL, FLAT_ROWS)
                //! the summed form: the two regions' rows folded under their weights inside the GEMV, one row of d out
                if (fmt == KqFmt.q51 && ensure_q51_gemv_sum_cls()) {
                    var wgt <- [0.625, -0.375]
                    let pys = cp |> out_(d, 0, 0, 64u)
                    let psw = cp |> in_(wgt)
                    var s_sum = cp |> cls_set(fixed_array(0, 1, 2, 3, 4, pys, psw), @@set_q51_gemv_sum_cls)
                    cp |> cell_record() $(raw3 : VkCommandBuffer; var h3 : VkHaz) {
                        enc_q51_gemv_sum_cls(raw3, h3, s_sum, pc, gemv_wgs(int64(d), 0u))
                    }
                    var ys_cls <- cp |> plane_run(pys)
                    var ys_ref <- [for (row in range(d)); wgt[0] * y_ref[row] + wgt[1] * y_ref[d + row]]
                    let bads = mismatch_approx(ys_cls, ys_ref)
                    t |> success(bads == 0, "q51 GEMV summed under the weights at n {n} matches the weighted rows ({bads} of {d} off)")
                    var ys_poison := ys_cls
                    ys_poison[0] += 1.0 + 2.0 * logits_maxabs(ys_ref)
                    t |> success(mismatch_approx(ys_poison, ys_ref) > 0, "q51 GEMV summed: the bar reds a poisoned row")
                    delete wgt
                    delete ys_cls
                    delete ys_ref
                    delete ys_poison
                }
                var y_poison := y_cls   // the bar's control
                y_poison[0] += 1.0 + 2.0 * logits_maxabs(y_ref)
                t |> success(mismatch_approx(y_poison, y_ref) > 0, "the {fmt} GEMV bar reds a poisoned element")
                delete y_poison
                delete y_cls
            }
            delete y_ref
            delete f
        }
    }
}

[test]
def test_vkd_kq_gemv_family(t0 : T?) {
    t0 |> run("kq GEMV class family == the CPU oracle (all 14 formats)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!kq_leaf_rails_engage() <| $(fmt : int) => gemv_cls_ensure(fmt)) {
                t |> skip("no Vulkan device, or a kq GEMV class pipeline failed to build")
                return
            }
            for (nsb in fixed_array(2, 20)) {   //! superblocks a row: 512 wide, the guarded tail alone; 5120 wide, four unrolled steps and a tail at every lane split
            let d = 3        // rows per region
            let nreg = 2
            let rows = nreg * d
            for (fmt in KQ_LEAF_FMTS) {
                let sb_words = int(kq_qsb(kq_fmt_of_slot(fmt)) / 4l)
                let ws_words_n = rows * nsb * int(kq_dev_ssb_i(fmt) / 4l)
                var wsh <- ws_plane(ws_words_n, 1u, uint(fmt) * 13u)
                var f <- gemv_fix(nreg, d, nsb, sb_words, uint(fmt) * 77777u, wsh, nsb, 64, 31u, 5u, 0.5, 0.25, 7)
                var y_ref <- kq_gemv_oracle(fmt, f.wqh, f.wsh, f.xqh, f.xsh, f.regs, nsb * 256, d, nreg)
                with_cell_planes() $(var cp : CellPlanes) {
                    gemv_cell_planes(cp, f, rows)
                    var s_cls = gemv_cls_set(fmt, gather_planes(cp.dev, GEMV_PLANES), gather_planes(cp.bytes, GEMV_PLANES), gather_planes(cp.hazard_bit, GEMV_PLANES))
                    //! the three lane splits of a subgroup: one row, two rows, four rows (the 16-block row keeps every lane busy at eight)
                    for (lanes_per_row in fixed_array(0u, 16u, 8u)) {
                        let wgs = gemv_wgs(int64(rows), lanes_per_row)
                        plane_refill(cp, 5)   //! a lane split that stores no row leaves the sentinel, not the last split's rows
                        var pc = GemvArgs(n = uint(nsb * 256), d = uint(d), nreg = uint(nreg), ybase = 0u, lanes_per_row = lanes_per_row)
                        cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                            gemv_cls_enc(fmt, raw2, h2, s_cls, pc, wgs)
                        }
                        var y_cls <- plane_run(cp, 5)
                        let bad = mismatch_approx(y_cls, y_ref)
                        let lanes = lanes_per_row == 0u ? g_gpu.subgroup_size : int64(lanes_per_row)
                        t |> success(bad == 0, "fmt {fmt} at {lanes} lanes per row: class kernel matches the CPU oracle ({bad} of {rows} rows off)")
                        var y_poison := y_cls   // the bar's control
                        y_poison[0] += 1.0 + 2.0 * logits_maxabs(y_ref)
                        t |> success(mismatch_approx(y_poison, y_ref) > 0, "fmt {fmt} at {lanes} lanes per row: the GEMV bar reds a poisoned element")
                        delete y_poison
                        delete y_cls
                    }
                }
                if (kq_has_float_oracle(fmt)) {
                    var y_flt <- kq_gemv_float_oracle(fmt, f.wqh, f.wsh, f.xqh, f.xsh, f.regs, nsb * 256, d, nreg)
                    let badf = mismatch_approx(y_ref, y_flt)
                    t |> success(badf == 0, "fmt {fmt}: the class bit-math matches the float dequant oracle ({badf} of {rows} rows off)")
                    delete y_flt
                }
                delete y_ref
                delete f
            }
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the Q4_K experts' gate+up+act twin against the family cell's oracle run twice: both stacks' rows in one plane
//! (the up stack's regions after the gate's), the activated rows' Q8_0 blocks within a quant step at every lane
//! split, the scales within 2e-3; a poisoned oracle element must red
[test]
def test_vkd_kq_gemv_k4_gu(t0 : T?) {
    t0 |> run("Q4_K gate + up GEMVs, the act and its Q8_0 requant in one dispatch == the CPU chain") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_kq_gemv_k4_gu_cls() && ensure_kq_gemv_k4_cls() && ensure_q8_actrq_cls())) {   //! the three-kernel path's rails for the byte compare
                t |> skip("no Vulkan device")
                return
            }
            for (shape in fixed_array((d = 64, nreg = 3), (d = 704, nreg = 8))) {   //! two blocks a slot; the 26B's eight slots of 704
            let fmt = int(KqFmt.k4)
            let nsb = 11        // 2816 wide: the 26B's routed feed
            let d = shape.d
            let nreg = shape.nreg
            let rows = nreg * d
            let nblk = rows / 32
            let nwsb = 2 * rows * nsb   // the gate stack's rows, then the up stack's
            let wq_words = nwsb * 32
            let ws_words_n = nwsb * int(kq_dev_ssb_i(fmt) / 4l)
            var wqh <- hashed_words(wq_words, 7u, 4441u)
            var wsh <- ws_plane(ws_words_n, 3u, 91u)
            var xqh <- hashed_words(nsb * 64, 23u, 17u)
            var xsh <- row_fill(nsb, 1, 0, 0.25, -0.5, 5)
            var rg <- gu_regions(nreg, d, nsb)
            var y_g <- kq_gemv_oracle(fmt, wqh, wsh, xqh, xsh, rg.g, nsb * 256, d, nreg)
            var y_u <- kq_gemv_oracle(fmt, wqh, wsh, xqh, xsh, rg.u, nsb * 256, d, nreg)
            var act <- [for (i in range(nblk * 32)); act_mul_ref(y_g[i], y_u[i], true)]
            with_cell_planes() $(var cp : CellPlanes) {
                let pwq = cp |> in_plain(wqh)
                let pws = cp |> in_plain(wsh)
                let prg = cp |> in_plain(rg.g)
                let pxq = cp |> in_plain(xqh)
                let pxs = cp |> in_(xsh)
                let py = cp |> out_(rows)
                let prg3 = cp |> in_plain(rg.u)
                let poq = cp |> out_words(int64(rows), NAN_SENTINEL_BITS)
                let pos = cp |> out_(nblk)
                let fused = fixed_array(pwq, pws, prg, pxq, pxs, py, pxs, prg3, poq, pos)   //! slot 6 the engine's grid table, set below
                var bufs = gather_planes(cp.dev, fused)
                var sizes = gather_planes(cp.bytes, fused)
                var bits = gather_planes(cp.hazard_bit, fused)
                bufs[6] = kq_grid_dev()
                sizes[6] = KQ_GRID_BYTES
                bits[6] = 0u
                var s_cls = set_kq_gemv_k4_gu_cls(bufs, sizes, bits)
                for (lanes_per_row in fixed_array(0u, 16u, 8u)) {
                    for (p in fixed_array(py, poq, pos)) {
                        cp |> plane_refill(p)
                    }
                    var pc = GemvArgs(n = uint(nsb * 256), d = uint(d), nreg = uint(nreg), ybase = 0u, lanes_per_row = lanes_per_row, act = ACT_GELU)
                    cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                        enc_kq_gemv_k4_gu_cls(raw2, h2, s_cls, pc, int64(nblk))
                    }
                    var oq <- cp |> plane_as(poq, type<int8>, rows)
                    var os <- cp |> plane_run(pos)
                    var y_cls <- cp |> plane_run(py)
                    let badg = mismatch_approx(y_cls, y_g)
                    let lanes = lanes_per_row == 0u ? g_gpu.subgroup_size : int64(lanes_per_row)
                    let tag = "k4 gu {nreg} x {d} at {lanes} lanes per row"
                    t |> success(badg == 0, "{tag}: the gate rows match the family's oracle ({badg} of {rows} off)")
                    gu_blocks_hold(t, tag, oq, os, act, nblk)
                    if (lanes_per_row == 16u) {   //! the three-kernel path on the same planes at the served lane split: its hidden rows byte for byte
                        let pyu = cp |> out_(rows)
                        let poq2 = cp |> out_words(int64(rows), NAN_SENTINEL_BITS)
                        let pos2 = cp |> out_(nblk)
                        var s_g = gemv_cls_set(fmt, gather_planes(cp.dev, GEMV_PLANES), gather_planes(cp.bytes, GEMV_PLANES), gather_planes(cp.hazard_bit, GEMV_PLANES))
                        let up = fixed_array(pwq, pws, prg3, pxq, pxs, pyu)
                        var s_u = gemv_cls_set(fmt, gather_planes(cp.dev, up), gather_planes(cp.bytes, up), gather_planes(cp.hazard_bit, up))
                        var s_a = cp |> cls_set(fixed_array(py, pyu, poq2, pos2), @@set_actrq_cls)
                        var pc2 = GemvArgs(n = uint(nsb * 256), d = uint(d), nreg = uint(nreg), ybase = 0u, lanes_per_row = 16u)
                        var pca = ActArgs(nelem = uint(rows), act = ACT_GELU, nblk = uint(nblk))
                        cp |> plane_refill(py)   //! the gate GEMV writes the fused run's gate plane: the sentinel back first, or a skipped dispatch reads as a match
                        cp |> cell_record() $(raw3 : VkCommandBuffer; var h3 : VkHaz) {
                            enc_kq_gemv_k4_cls(raw3, h3, s_g, pc2, gemv_wgs(int64(rows), 16u))
                            enc_kq_gemv_k4_cls(raw3, h3, s_u, pc2, gemv_wgs(int64(rows), 16u))
                            enc_q8_actrq_cls(raw3, h3, s_a, pca, (int64(rows) / 4l + 255l) / 256l)
                        }
                        var oq2 <- cp |> plane_as(poq2, type<int8>, rows)
                        var os2 <- cp |> plane_run(pos2)
                        let bad = mismatch_q8(oq, os, oq2, os2, 0.0, 0)
                        t |> success(bad == 0, "{tag}: the hidden rows match the three-kernel path byte for byte ({bad} of {rows} quants and {nblk} scales off)")
                        delete oq2
                        delete os2
                    }
                    delete oq
                    delete os
                    delete y_cls
                }
            }
            delete wqh
            delete wsh
            delete xqh
            delete xsh
            delete rg
            delete y_g
            delete y_u
            delete act
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_act_family(t0 : T?) {
    t0 |> run("act/requant + combine class kernels == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_q8_actrq_cls() && ensure_q8k_actrq_cls() && ensure_combine_cls())) {
                t |> skip("no Vulkan device")
                return
            }
            let nelem = 1024
            var gh <- row_fill(nelem, 29, 0, 0.21, 6.0, 61)
            var uh <- row_fill(nelem, 11, 0, 0.17, 4.0, 47)
            // arm 0 = q8 (32-blocks), arm 1 = q8k (256-superblocks); arms 2 and 3 the same under the clamped swiglu (gpt-oss's act code on the unbiased kernels)
            for (arm in range(4)) {
                let q8k = (arm & 1) == 1
                let act = arm >= 2 ? ACT_OAI : ACT_SILU
                let nblk = q8k ? nelem / 256 : nelem / 32
                var q_cls : array<uint>
                var s_cls : array<float>
                with_cell_planes() $(var cp : CellPlanes) {
                    let pg = cp |> in_(gh)
                    let pu = cp |> in_(uh)
                    let pq = cp |> out_words(int64(nelem), I8_SENTINEL_BITS)   // 1 byte per quant either way
                    let ps = cp |> out_(nblk)
                    var sc = cp |> cls_set(fixed_array(pg, pu, pq, ps), @@set_actrq_cls)
                    var pc = ActArgs(nelem = uint(nelem), act = act, nblk = uint(nblk))
                    cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                        if (q8k) {
                            enc_q8k_actrq_cls(raw2, h2, sc, pc, 1l)   // 256 threads cover both shapes at nelem = 1024
                        } else {
                            enc_q8_actrq_cls(raw2, h2, sc, pc, 1l)
                        }
                    }
                    delete q_cls
                    delete s_cls
                    q_cls <- cp |> plane_as(pq, type<uint>, nelem / 4)
                    s_cls <- cp |> plane_run(ps)
                }
                var q_ref : array<uint>
                var s_ref : array<float>
                actrq_oracle(gh, uh, nelem, q8k, act, q_ref, s_ref)
                // exp/tanh differ by ulps between backends: quants compare as bytes +-1, scales approx
                let tag = "{q8k ? "q8k" : "q8"} actrq{act == ACT_OAI ? " under the clamped swiglu" : ""}"
                check_qbytes(t, tag, q_cls, q_ref)
                check_rows(t, "{tag} scales", s_cls, s_ref, BAR_APPROX_CTRL, FLAT_ROWS)
                delete q_ref
                delete s_ref
                delete q_cls
                delete s_cls
            }
            //! the biased twins under the clamped swiglu (gpt-oss): three slot rows of 128, each row's expert naming its gate and up
            //! bias rows out of a six-expert plane; the Q8_0 requant form and the f16 form against the oracle over the biased inputs
            t |> success(ensure_q8_actrq_b_cls() && ensure_actf16_b_cls(), "the biased act twins compile on this device")
            {
                let brow = 128
                let ne = 6
                let nrow = 3
                let nb = nrow * brow
                var rex <- [uint(2), uint(0), uint(5)]
                var gb <- row_fill(ne * brow, 13, 0, 0.3, 4.5, 31)   //! biases past the 7 clamp on some elements: the clamps must engage
                var ub <- row_fill(ne * brow, 17, 0, 0.4, 5.6, 29)
                var gh2, uh2 : array<float>
                gh2 |> resize(nb)
                uh2 |> resize(nb)
                for (e in range(nb)) {
                    let ex = int(rex[e / brow])
                    gh2[e] = gh[e] + gb[ex * brow + e % brow]
                    uh2[e] = uh[e] + ub[ex * brow + e % brow]
                }
                var q_cls : array<uint>
                var s_cls : array<float>
                var h_cls : array<uint>
                var pc = ActArgs(nelem = uint(nb), act = ACT_OAI, nblk = uint(nb / 32), bias_on = 1u, gboff = 0u, uboff = 0u, brow = uint(brow))
                with_cell_planes() $(var cp : CellPlanes) {
                    let pg = cp |> in_(gh)
                    let pu = cp |> in_(uh)
                    let pq = cp |> out_words(int64(nb), NAN_SENTINEL_BITS)
                    let ps = cp |> out_(nb / 32)
                    let ph = cp |> out_words(int64(nb) * 2l, NAN_SENTINEL_BITS)
                    let pgb = cp |> in_(gb)
                    let pub = cp |> in_(ub)
                    let prex = cp |> in_plain(rex)
                    var sc = cp |> cls_set(fixed_array(pg, pu, pq, ps, pgb, pub, prex), @@set_q8_actrq_b_cls)
                    var sc16 = cp |> cls_set(fixed_array(pg, pu, ph, pgb, pub, prex), @@set_actf16_b_cls)
                    cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                        enc_q8_actrq_b_cls(raw2, h2, sc, pc, (int64(nb) / 4l + 255l) / 256l)
                        enc_actf16_b_cls(raw2, h2, sc16, pc, (int64(nb) / 4l + 255l) / 256l)
                    }
                    delete q_cls
                    delete s_cls
                    delete h_cls
                    q_cls <- cp |> plane_as(pq, type<uint>, nb / 4)
                    s_cls <- cp |> plane_run(ps)
                    h_cls <- cp |> plane_as(ph, type<uint>, nb / 2)
                }
                var q_ref : array<uint>
                var s_ref : array<float>
                actrq_oracle(gh2, uh2, nb, false, ACT_OAI, q_ref, s_ref)
                check_qbytes(t, "the biased q8 actrq under the clamped swiglu", q_cls, q_ref)
                check_rows(t, "the biased q8 actrq under the clamped swiglu, scales", s_cls, s_ref, BAR_APPROX_CTRL, FLAT_ROWS)
                var h_ref, h_got : array<float>
                h_ref |> resize(nb)
                h_got |> resize(nb)
                for (e in range(nb)) {
                    h_ref[e] = float(float16(act_mul_ref(gh2[e], uh2[e], ACT_OAI)))
                }
                for (i in range(nb)) {
                    h_got[i] = half_at(h_cls, i)
                }
                let badh = mismatch_rel("", h_got, h_ref, 2.0e-3, 2.0e-3 * logits_maxabs(h_ref))
                t |> success(badh == 0, "the biased f16 act under the clamped swiglu matches the oracle within the half's rounding ({badh} of {nb} off)")
                //! the controls: an unbiased oracle must red both forms (the bias rows carry the signal)
                var q_nb : array<uint>
                var s_nb : array<float>
                actrq_oracle(gh, uh, nb, false, ACT_OAI, q_nb, s_nb)
                t |> success(mismatch_qbytes(q_cls, q_nb, 1) > 0, "biased actrq control: the unbiased oracle reds the compare")
                h_ref[7] += 0.5
                t |> success(mismatch_rel("", h_got, h_ref, 2.0e-3, 2.0e-3 * logits_maxabs(h_ref)) == 1, "biased f16 act control: the bar reds one poisoned element")
                delete rex
                delete gb
                delete ub
                delete gh2
                delete uh2
                delete q_cls
                delete s_cls
                delete h_cls
                delete q_ref
                delete s_ref
                delete h_ref
                delete h_got
                delete q_nb
                delete s_nb
            }
            // the combine
            {
                let n = 64
                let k = 4
                let npos = 3
                let nrows = 8
                var sidx : array<uint>
                var sw : array<float>
                sidx |> resize(npos * k)
                sw |> resize(npos * k)
                for (i in range(npos * k)) {
                    sidx[i] = hash_word(uint(i)) % uint(nrows)
                    sw[i] = 0.125 + float(i % 5) * 0.25
                }
                var rows <- row_fill(nrows * n, 7, 0, 0.13, 5.5, 93)
                var a_cls : array<float>
                with_cell_planes() $(var cp : CellPlanes) {
                    let pa = cp |> out_(npos * n)   //! r0 == 0 starts the sum: the accumulator rows are written, never read
                    let psi = cp |> in_plain(sidx)
                    let psw = cp |> in_(sw)
                    let pr = cp |> in_(rows)
                    var sc = cp |> cls_set(fixed_array(pa, psi, psw, pr), @@set_combine_cls)
                    var pc = CombineArgs(n = uint(n), k = uint(k), r0 = 0u, r1 = uint(nrows))
                    cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                        enc_combine_cls(raw2, h2, sc, pc, int64(npos))
                    }
                    delete a_cls
                    a_cls <- cp |> plane_run(pa)
                }
                // oracle: same slot order, but the device contracts mul+add to fma — approx bar
                var a_ref : array<float>
                a_ref |> resize(npos * n)
                for (p in range(npos)) {
                    for (i in range(n)) {
                        var acc = 0.0
                        for (j in range(k)) {
                            acc += sw[p * k + j] * rows[int(sidx[p * k + j]) * n + i]
                        }
                        a_ref[p * n + i] = acc
                    }
                }
                check_rows(t, "combine against the CPU oracle", a_cls, a_ref, BAR_APPROX_CTRL, n)
                delete sidx
                delete sw
                delete rows
                delete a_ref
                delete a_cls
            }
            delete gh
            delete uh
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! `n` elements of `src` from `soff`, plus the bias row `bh` from `boff` when the cell has one (an empty `bh`: no bias)
def private biased_row(src : array<float>; soff : int; bh : array<float>; boff, n : int) : array<float> {
    if (empty(bh)) {
        return <- [for (i in range(n)); src[soff + i]]
    }
    return <- [for (i in range(n)); src[soff + i] + bh[boff + i]]
}

//! the rope oracle's cos and sin rows: the two halves of the `hs`-wide cos | sin row at `at`
def private cossin_rows(csh : array<float>; at, hs : int) : tuple<c : array<float>; s : array<float>> {
    var c <- [for (j in range(hs / 2)); csh[at + j]]
    var s <- [for (j in range(hs / 2)); csh[at + hs / 2 + j]]
    return <- (c = c, s = s)
}

//! the KV writers' rows at one position: q from `q0h` at `qoff`, k and v from `kvh` at `koff` / `voff`, each biased by its
//! `bh` row past `bpad` (q, then k, then v) when the cell has one; q and k rotated by the engine's table rope on the cos | sin
//! row of `csh` at `csoff`; k and v through `round`, the mirror's grid
def private rope_kv_rows(q0h, kvh, bh, csh : array<float>; qoff, koff, voff, csoff, bpad, nh, nkvh, hs : int;
                         round : block<(x : float) : float>) : tuple<q : array<float>; k : array<float>; v : array<float>> {
    let qd = nh * hs
    let kvd = nkvh * hs
    var cs <- cossin_rows(csh, csoff, hs)
    var q <- biased_row(q0h, qoff, bh, bpad, qd)
    var k <- biased_row(kvh, koff, bh, bpad + qd, kvd)
    var v <- biased_row(kvh, voff, bh, bpad + qd + kvd, kvd)
    rope_tab_ref(q, 0, hs, hs, qd, true, cs.c, cs.s)
    rope_tab_ref(k, 0, hs, hs, kvd, true, cs.c, cs.s)
    for (i in range(kvd)) {
        k[i] = invoke(round, k[i])
        v[i] = invoke(round, v[i])
    }
    delete cs
    return <- (q = q, k = k, v = v)
}

//! the fused qk-norm + rope + KV-store oracle at one position: each head row biased (when `bh` is set), normed by the engine's
//! rmsnorm on its weight row (q heads on `wnh`'s first `hs`, k heads on the next), rotated by the engine's table rope on the
//! cos | sin row `csh`; k lands at mirror row `krow` and v copies its raw row plus its bias, both through `round`
def private qkn_rope_rows(q0h, kvh, bh, wnh, csh : array<float>; bpad, nh, nkvh, hs, krow, mirn : int; eps : float;
                          round : block<(x : float) : float>) : tuple<q : array<float>; mk : array<float>; mv : array<float>> {
    let qd = nh * hs
    let kvd = nkvh * hs
    var cs <- cossin_rows(csh, 0, hs)
    var q, mk, mv : array<float>
    q |> resize(qd)
    mk |> resize(mirn)
    mv |> resize(mirn)
    for (wid in range(nh + nkvh)) {
        let is_k = wid >= nh
        let hh = is_k ? wid - nh : wid
        let sbase = is_k ? qd + hh * hs : hh * hs
        var row <- is_k ? biased_row(kvh, sbase, bh, bpad + sbase, hs) : biased_row(q0h, sbase, bh, bpad + sbase, hs)
        var nrow : array<float>
        nrow |> resize(hs)
        unsafe {
            rmsnorm(addr<float?>(nrow[0]), addr<float const?>(row[0]), addr<float const?>(wnh[is_k ? hs : 0]), int64(hs), eps)
        }
        rope_tab_ref(nrow, 0, hs, hs, hs, true, cs.c, cs.s)
        for (i in range(hs)) {
            if (is_k) {
                mk[krow + hh * hs + i] = invoke(round, nrow[i])
            } else {
                q[hh * hs + i] = nrow[i]
            }
        }
        if (is_k) {
            var vrow <- biased_row(kvh, qd + kvd + hh * hs, bh, bpad + qd + kvd + hh * hs, hs)
            for (i in range(hs)) {
                mv[krow + hh * hs + i] = invoke(round, vrow[i])
            }
            delete vrow
        }
        delete row
        delete nrow
    }
    delete cs
    return <- (q = q, mk = mk, mv = mv)
}

//! the rope + K/V store (`rope_kv_cls`, or under `f16` its half-mirror twin) on one row against the CPU oracle: the q rows roped
//! in place, k roped and v raw into the mirrors at row 0, with the q/k/v bias rows past a pad under `biased` - the oracle adds
//! the bias first, then the engine's NEOX table rotation, the mirrors on the half grid under `f16`; the approx bars ride out
//! the device's fma contraction
def private rope_kv_arm(t : T?; f16, biased : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let hs = 64
        let half = hs / 2
        let nh = 4
        let nkvh = 2
        let qd = nh * hs
        let kvd = nkvh * hs
        let npairs = (nh + nkvh) * half
        var qh <- row_fill(qd, 23, 0, 0.09, 3.0, 71)
        var kvh <- row_fill(qd + 2 * kvd, 19, 0, 0.07, 2.9, 83)
        var csh : array<float>
        csh |> resize(hs)
        for (i in range(half)) {
            csh[i] = 0.3 + float(i % 9) * 0.07          // cos row
            csh[half + i] = 0.1 + float(i % 7) * 0.11   // sin row
        }
        let bpad = biased ? 64 : 0   // the bias rows sit past a pad, so boff is exercised off its default
        var bh <- biased ? bias_past_pad(bpad, qd + 2 * kvd, 13, 0.05, 0.7, 29, 55.0) : array<float>()
        var ref <- rope_kv_rows(qh, kvh, bh, csh, 0, qd, qd + kvd, 0, bpad, nh, nkvh, hs) $(x : float) => f16 ? f16r(x) : x
        with_cell_planes() $(var cp : CellPlanes) {
            //! the class kernel: q@0, kvsrc@1, kmir@2, vmir@3, cossin@4, tok@5 (pos 0 -> row 0), bias@6
            let pq = cp |> io_(qh)
            let pkv = cp |> in_(kvh)
            let pkm = mirror_plane(cp, kvd, f16)
            let pvm = mirror_plane(cp, kvd, f16)
            let pcs = cp |> in_(csh)
            let ptok = cp |> in_plain([TokMeta()])
            let pb = biased ? in_(cp, bh) : pcs
            let planes = fixed_array(pq, pkv, pkm, pvm, pcs, ptok, pb)
            var sc = f16 ? cls_set(cp, planes, @@set_rope_kv_f16_cls) : cls_set(cp, planes, @@set_rope_kv_cls)
            var pc = RopeKvArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(half),
                neox = 1u, kpair0 = uint(nh * half), npairs = uint(npairs), layerbase = 0u, boff = uint(bpad), hasb = biased ? 1u : 0u)
            cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                if (f16) {
                    enc_rope_kv_f16_cls(raw2, h2, sc, pc, int64((npairs + 255) / 256))
                } else {
                    enc_rope_kv_cls(raw2, h2, sc, pc, int64((npairs + 255) / 256))
                }
            }
            var cls_q <- cp |> plane_run(pq)
            var m <- mirror_pair(cp, pkm, pvm, kvd, f16)
            let tag = "{f16 ? "rope_kv_f16" : "rope_kv_store"}{biased ? " with the q/k/v bias" : ""}"
            check_rows(t, "{tag}, q", cls_q, ref.q, BAR_APPROX_CTRL, qd, hs)
            mirror_rows_check(t, tag, m, ref.k, ref.v, 0, kvd, kvd)
            delete cls_q
            delete m
        }
        delete qh
        delete kvh
        delete csh
        delete bh
        delete ref
    }
}

[test]
def test_vkd_rope_family(t0 : T?) {
    t0 |> run("rope_kv_store + qk_rms class kernels == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_rope_kv_cls() && ensure_qk_rms_cls())) {
                t |> skip("no Vulkan device")
                return
            }
            rope_kv_arm(t, false, true)
            let hs = 64
            let nh = 4
            let nkvh = 2
            let qd = nh * hs
            let kvd = nkvh * hs
            // ===== qk_rms =====
            {
                let rows = 2
                let qstride = qd
                let kstride = kvd
                let eps = 1e-5
                var qh <- row_fill(rows * qd, 31, 0, 0.08, 2.5, 67)
                var kh <- row_fill(rows * kvd, 43, 0, 0.06, 1.7, 59)
                var wh <- [for (i in range(2 * hs)); 0.4 + float(i % 13) * 0.06]
                var cls_q, cls_k : array<float>
                with_cell_planes() $(var cp : CellPlanes) {
                    // the class kernel: q@0, k@1, weights@2
                    let pq = cp |> io_(qh)
                    let pk = cp |> io_(kh)
                    let pw = cp |> in_(wh)
                    var sc = cp |> cls_set(fixed_array(pq, pk, pw), @@set_qk_rms_cls)
                    var pc = QkRmsArgs(hs = uint(hs), nh = uint(nh), nkvh = uint(nkvh),
                        qstride = uint(qstride), kstride = uint(kstride), kbase = 0u,
                        qwoff = 0u, kwoff = uint(hs), eps = eps)
                    cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                        enc_qk_rms_cls(raw2, h2, sc, pc, int64(rows * (nh + nkvh)))
                    }
                    delete cls_q
                    delete cls_k
                    cls_q <- cp |> plane_run(pq)
                    cls_k <- cp |> plane_run(pk)
                }
                // oracle: per-head rms (1/sqrt(mean(v^2)+eps)) times the shared weight row —
                // the device reduces sum-of-squares in tree order, so approx bars
                var ref_q, ref_k : array<float>
                ref_q |> resize(rows * qd)
                ref_k |> resize(rows * kvd)
                for (r in range(rows)) {
                    for (hh in range(nh + nkvh)) {
                        let is_q = hh < nh
                        let base = is_q ? r * qstride + hh * hs : r * kstride + (hh - nh) * hs
                        let wbase = is_q ? 0 : hs
                        unsafe {
                            if (is_q) {
                                rmsnorm(addr<float?>(ref_q[base]), addr<float const?>(qh[base]), addr<float const?>(wh[wbase]), int64(hs), eps)
                            } else {
                                rmsnorm(addr<float?>(ref_k[base]), addr<float const?>(kh[base]), addr<float const?>(wh[wbase]), int64(hs), eps)
                            }
                        }
                    }
                }
                check_rows(t, "qk_rms, q", cls_q, ref_q, BAR_APPROX_CTRL, qd, hs)
                check_rows(t, "qk_rms, k", cls_k, ref_k, BAR_APPROX_CTRL, kvd, hs)
                delete qh
                delete kh
                delete wh
                delete ref_q
                delete ref_k
                delete cls_q
                delete cls_k
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
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

[test]
def test_vkd_kq_batch_family(t0 : T?) {
    t0 |> run("kq batch tile class family == the CPU oracle (all 14 formats)") <| @(t : T?) {
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
                p.wsh <- ws_plane(wswords, 1u, uint(fmt) * 17u)
                p.xqh <- hashed_words(rows * nsb * 64, 7u, 2u)
                p.xsh <- row_fill(rows * nsb, 1, 0, 0.05, -0.3, 11)
                //! one family set serves every format; region 1's weights follow region 0's stack in superblock units
                var r <- sdot4_tile_run(n, d, cnt0, cnt1, 32, d * nsb, p) $(bufs : uint64[6]; sizes : int64[6]; bits : uint[6]; raw2 : VkCommandBuffer; var h2 : VkHaz; wgs : int64) {
                    var sc = set_kq_batch_cls(bufs, sizes, bits)
                    var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(2 * 4))
                    kq_batch_cls_enc_for(fmt, false, raw2, h2, sc, pc, wgs)
                }
                var y_ref : array<float>
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

//! the cm batch cell's five arms as the class router's variant ordinals: cmf16, cmi8, mm (the L tile), mm aligned-L, mm M-tile
let private CM_CELL_VARIANTS = fixed_array(1, 2, 5, 4, 3)

[test]
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
                var y_ref : array<float>
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

[test]
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

[test]
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
            verify(ensure_splitk_reduce_cls(), "the split-k reduce's class rail must engage")
            let nsplit = 2
            let ck = cm2_chunk(int64(f.n), int64(nsplit))
            t |> equal(ck, 512l, "the engine's chunk: K 576 in two 256-aligned chunks, 512 and 64 deep")
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
                var scs = cp |> cls_set(fixed_array(pwq, pws, psc, pxf, pp), @@set_q8_batch_khr_cls)
                var srd = cp |> cls_set(fixed_array(pp, py), @@set_splitk_reduce_cls)
                var pcs = BatchArgs(n = uint(f.n), d = uint(d), map_off = 4u, ksplit = uint(ck))
                cp |> cell_record() $(raw3 : VkCommandBuffer; var h3 : VkHaz) {
                    enc_q8_batch_khr_cls(raw3, h3, scs, pcs, int64(swgs))
                    splitk_reduce_enc(raw3, h3, srd, int64(cnt0 * d), int64(nsplit))
                }
                check_rows(t, "q8 KHR tile split in two k chunks + the reduce vs the CPU oracle", cp |> plane_run(py), y_ref0, cm_bar(y_ref0), d)
            }
            delete y_ref0
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_cm2l_batch(t0 : T?) {
    t0 |> run("cm2 l-tile fmt-0 batch class kernel == the CPU oracle") <| @(t : T?) {
        q8_tile_cell(t, ARM_CM2_L, 576, 17u, 991u, 3u, 5u)   // K off the 256 lattice: two unrolled superblock passes and a 64-deep tail step
    }
}

[test]
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

[test]
def test_vkd_k4_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q4_K l/m/s/e tiles and the KHR tile == the CPU oracle (decode-in-load over the repacked planes; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.k4)
    }
}

[test]
def test_vkd_k5_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q5_K l/m/s/e tiles and the KHR tile == the CPU oracle (5th-bit compose decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.k5)
    }
}

[test]
def test_vkd_q51_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q5_1 s and e tiles == the CPU oracle (the per-32 plane verbatim: nibble | qh bit, d * q + m off one word)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.q51)
    }
}

//! the iq4nl32 cells' scale words: cool f16 d, two blocks a word, so the tile's f16 sums stay far from 65504
def private iq4nl32_scale_words(totblk : int; mul : float) : array<uint> {
    var w : array<uint>
    static_if (typeinfo builtin_module_exists(vulkan)) {
        w |> resize((totblk + 1) / 2)
        for (i in range(length(w))) {
            w[i] = packHalf2x16(float2(mul * float(1 + (2 * i) % 7), mul * float(1 + (2 * i + 1) % 7)))
        }
    }
    return <- w
}

[test]
def test_vkd_iq4nl32_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ4_NL per-32 s and e tiles == the CPU oracle (the per-32 plane verbatim: d * LUT[nibble] off a half-word d)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.iq4nl32)
    }
}

//! the mx4 cells' E8M0 bytes: exponents 112..124 (2^-16 .. 2^-4 halved), four a word, so the tile's f16 sums stay far from 65504
def private mx4_scale_words(totblk : int) : array<uint> {
    var w : array<uint>
    w |> resize((totblk + 3) / 4)
    for (b in range(totblk)) {
        w[b / 4] |= uint(112 + b % 13) << uint((b % 4) * 8)
    }
    return <- w
}

[test]
def test_vkd_mx4_cm2_batch(t0 : T?) {
    t0 |> run("cm2 MXFP4 s and e tiles == the CPU oracle (the per-32 plane verbatim: e2m1 nibble, the doubled magnitude times the halved E8M0 scale off one byte)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.mx4)
    }
}

[test]
def test_vkd_q40_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q4_0 l/m/s/e tiles and the KHR tile == the CPU oracle (per-block f16 d decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.q40)
    }
}

[test]
def test_vkd_k2_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q2_K l/m/s/e tiles and the KHR tile == the CPU oracle (pair-byte nibble scales decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.k2)
    }
}

[test]
def test_vkd_iq4nl_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ4_NL l/m/s/e tiles and the KHR tile == the CPU oracle (codebook + per-block f16 d decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.iq4nl)
    }
}

[test]
def test_vkd_iq4xs_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ4_XS l/m/s/e tiles and the KHR tile == the CPU oracle (workgroup-staged codebook decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.iq4xs)
    }
}

[test]
def test_vkd_iq3s4_cm2_batch(t0 : T?) {
    t0 |> run("cm2 iq3s4 l/m/s/e tiles and the KHR tile == the CPU oracle (the odd codebook staged, the iq3 row's 1 + 2s sub-scale); the iq4xs oracle is its control") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.iq3s4)
    }
}

[test]
def test_vkd_k3_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q3_K l/m/s/e tiles and the KHR tile == the CPU oracle (2-bit + hmask compose decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.k3)
    }
}

[test]
def test_vkd_iq3s_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ3_S l/m/s/e tiles and the KHR tile == the CPU oracle (workgroup-staged grid decode-in-load; the KHR arm stages the same decode); the iq3xxs oracle is its control") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.iq3s)
    }
}

[test]
def test_vkd_iq2s_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ2_S l/m/s/e tiles and the KHR tile == the CPU oracle (workgroup-staged u64 grid decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.iq2s)
    }
}

[test]
def test_vkd_iq2xs_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ2_XS l/m/s/e tiles and the KHR tile == the CPU oracle (u64 grid + ksigns-by-parity decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.iq2xs)
    }
}

[test]
def test_vkd_iq2xxs_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ2_XXS l/m/s/e tiles and the KHR tile == the CPU oracle (byte-grid + aux32 ksigns decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.iq2xxs)
    }
}

[test]
def test_vkd_iq3xxs_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ3_XXS l/m/s/e tiles and the KHR tile == the CPU oracle (halved grid + parity signs decode-in-load; the KHR arm stages the same decode); the iq2xxs oracle is its control") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.iq3xxs)
    }
}

[test]
def test_vkd_k6_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q6_K l/m/s/e tiles and the KHR tile == the CPU oracle (6-bit compose decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        fmt_tile_cell(t, KqFmt.k6)
    }
}

[test]
def test_vkd_cm2m_batch(t0 : T?) {
    t0 |> run("cm2 m-tile fmt-0 batch class kernel == the CPU oracle") <| @(t : T?) {
        q8_tile_cell(t, ARM_CM2_M, 576, 29u, 733u, 5u, 11u)   // K off the 256 lattice: two unrolled superblock passes and a 64-deep tail step
    }
}

[test]
def test_vkd_cm2e_batch(t0 : T?) {
    t0 |> run("cm2 e-column fmt-0 batch class kernel == the CPU oracle") <| @(t : T?) {
        q8_tile_cell(t, ARM_CM2_E, Q8_CELL_K, 31u, 739u, 5u, 13u)
    }
}

[test]
def test_vkd_cm2s_batch(t0 : T?) {
    t0 |> run("cm2 s-tile fmt-0 batch class kernel == the CPU oracle") <| @(t : T?) {
        q8_tile_cell(t, ARM_CM2_S, Q8_CELL_K, 29u, 733u, 5u, 11u)
    }
}

[test]
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

[test]
def test_vkd_da_attn_rqk(t0 : T?) {
    t0 |> run("decode attention with the requant folded into its store == the CPU oracle's row, quantized: Q8_0 and Q8_K, the pass and the combine, the 256 and 512 heads") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!da_attn_stamps_ensure(KVDtype.f32)) {
                t |> skip("no Vulkan device")
                return
            }
            //! the two-head slab on the groups it serves - two heads a group (gemma-4's shape at 256 and 512), one head a group
            //! (the dead lane through the pass's store and through the combine) - each held bit for bit to the four-head stamp
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 4, kv_mul = 2, fold = Fold.q8_0, g4_twin = true))   //! the pass's own Q8_0 store
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 4, kv_mul = 2, fold = Fold.q8_0, nsplit = 8, g4_twin = true))   //! the last piece's combine (three empty pieces)
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 256, nheads = 2, kv_mul = 2, keys = 70, fold = Fold.q8_k, g4_twin = true))   //! one superblock a head, the pass
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 256, nheads = 2, kv_mul = 1, keys = 70, fold = Fold.q8_k, g4_twin = true))
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 256, nheads = 2, kv_mul = 1, keys = 70, fold = Fold.q8_k, nsplit = 4, g4_twin = true))   //! ... the last piece's combine
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 512, nheads = 2, kv_mul = 2, keys = 70, fold = Fold.q8_k, nsplit = 4, g4_twin = true))   //! the wide head: two superblocks
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 512, nheads = 2, kv_mul = 2, keys = 70, fold = Fold.q8_0))   //! the wide head's Q8_0 blocks, the pass
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 4, kv_mul = 4, fold = Fold.q8_0))   //! the four-head slab's own store
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 4, kv_mul = 4, fold = Fold.q8_0, nsplit = 8))   //! ... and its combine's
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_da_attn(t0 : T?) {
    t0 |> run("decode attention class kernel == the CPU oracle (chunk boundary)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(da_attn_stamps_ensure(KVDtype.f32) && da_attn_stamps_ensure(KVDtype.f16))) {
                t |> skip("no Vulkan device")
                return
            }
            codec_attn_arms(t, KVDtype.f32)
            //! a two-head group's softcap (gemma-2's 4.0 cap the scaled scores overrun), whole and in eight key pieces
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 4, kv_mul = 2, cap = 4.0))
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 4, kv_mul = 2, cap = 4.0, nsplit = 8))
            //! the sink stamp over a two-head group (gpt-oss): the pass folding the sink at its store, and the last piece's
            //! combine seeding it - over the f32 mirror and over the f16 one (the resident driver's kv16 form)
            for (kv in fixed_array(KVDtype.f32, KVDtype.f16)) {
                codec_attn_arm(t, CodecAttn(kv = kv, hs = 64, nheads = 4, kv_mul = 2, sink = true))
                codec_attn_arm(t, CodecAttn(kv = kv, hs = 64, nheads = 4, kv_mul = 2, sink = true, nsplit = 8))
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_da_attn_shapes(t0 : T?) {
    t0 |> run("decode attention at the shapes the fixed cells leave out == the CPU oracle: ragged key groups, a full four-head slab, a short last slab, a 24-word head, two rows a dispatch") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!da_attn_stamps_ensure(KVDtype.f32)) {
                t |> skip("no Vulkan device")
                return
            }
            //! every split arm dispatches twice over the counters the first left: the arrival counters the pass resets rearm
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 96, nheads = 4, kv_mul = 1))   //! 21 key groups of 12 words: the ragged group count, no fold
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 96, nheads = 4, kv_mul = 1, nsplit = 4, resubmit = true))
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 128, nheads = 8, kv_mul = 4))   //! a full four-head slab (`live` 4): two fold rounds at subgroup 32
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 128, nheads = 8, kv_mul = 4, nsplit = 8, resubmit = true))
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 10, kv_mul = 5))   //! a short last slab: `live` 1 on each kv head's second slab
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 192, nheads = 2, kv_mul = 2, keys = 70))   //! 24 words a V row, not a power of two: no fold
            //! two rows a dispatch, each over its own mirror region: the row index and the slab arithmetic together, the
            //! four-head stamp over the same rows held to the two-head slab bit for bit
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 4, kv_mul = 2, nrows = 2, g4_twin = true))
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 4, kv_mul = 2, nrows = 2, nsplit = 4, resubmit = true, g4_twin = true))
            codec_attn_arm(t, CodecAttn(kv = KVDtype.f32, hs = 64, nheads = 4, kv_mul = 2, nrows = 4, nsplit = 4, resubmit = true))
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_at_attn(t0 : T?) {
    t0 |> run("prefill attention class kernel == the CPU oracle (partial tiles, gated)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!ensure_at_attn_cls()) {
                t |> skip("at_attn class declined on this device (workgroup cap)")
                return
            }
            let nheads = 4
            let kv_mul = 2
            let rows = 13   // partial 8-tile: q0=8 has 5 valid rows — the row masks run
            let w0 = 32     // nonzero window base: kend crosses the 32-position K tile → alpha rescale runs
            let scale = 0.125
            for (hs in [64, 256]) {   // dpl 2 (o0/o1 only) and dpl 8 (the o4..o7 arm); gate the wide one
                let gated = hs == 256
                let qd = nheads * hs
                let kvd = (nheads / kv_mul) * hs
                var f <- da_attn_b_fix(rows, nheads, kv_mul, hs, w0, 0, 0, 0.05, 1.8, 0.03, 1.4, false)   // K/V panels hold absolute positions [0, w0+rows)
                var gh <- row_fill(rows * qd, 29, 0, 0.06, 1.9, 61)
                // oracle: causal rows (row r attends [0, w0 + r]); gated = out * sigmoid(gate)
                var y_ref <- da_attn_b_ref(f, scale, 0, 0.0)
                if (gated) {
                    for (i in range(rows * qd)) {
                        y_ref[i] = sig_gate(y_ref[i], gh[i])
                    }
                }
                with_cell_planes() $(var cp : CellPlanes) {
                    // the class kernel: q@0, K@1, V@2, out@3 (pre-loaded with the gates)
                    let pq = cp |> in_(f.qh)
                    let pk = cp |> in_(f.kh)
                    let pv = cp |> in_(f.vh)
                    let py = cp |> io_(gh)
                    var sc = cp |> cls_set(fixed_array(pq, pk, pv, py), @@set_at_attn_cls)
                    var pc = AtAttnArgs(rows = uint(rows), w0 = uint(w0), qd = uint(qd),
                        kvd = uint(kvd), hs = uint(hs), kv_mul = uint(kv_mul),
                        flags = gated ? 1u : 0u, dpl = uint(hs / 32), scale = scale)
                    cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                        enc_at_attn_cls(raw2, h2, sc, pc, int64(nheads * ((rows + 7) / 8)))
                    }
                    check_rows(t, "at_attn hs {hs}{gated ? " gated" : ""} against the CPU oracle", cp |> plane_run(py), y_ref, BAR_APPROX_CTRL, qd, hs)
                }
                delete f
                delete gh
                delete y_ref
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the cells' dispatch of one flash stamp through the prefill's ladders, the grid from the arm's rows a workgroup; generic so a build without the vulkan module never infers it
def private enc_fa_tile(khr : bool; hs : int; gated, cap, sink, f16 : bool; raw; var h; var s; var pc; nheads, rows : int) {
    fa_stamp_enc(khr, int64(hs), gated, cap, sink, f16, raw, h, s, pc, fa_stamp_wgs(khr, int64(nheads), int64(rows)))
}

def private fa_arm_ensure(khr : bool) : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        for (hs in fixed_array(64l, 128l, 256l, 512l)) {
            if (!fa_stamp_pair_ensure(khr, hs, false, false, false, true)) return false
        }
        if (!fa_stamp_pair_ensure(khr, 64l, false, false, true, true)) return false   //! the sink stamps (gpt-oss)
    }
    return true
}

//! a flash stamp's set over the cell planes `p` (q, the K and V shadows, the out plane), and the sink plane `psink` past them on a
//! sink stamp; generic so a build without the vulkan module never infers it
def private fa_cell_set(cp; khr, sink : bool; p; psink : int) {
    if (sink) {
        let p5 = fixed_array(p[0], p[1], p[2], p[3], psink)
        return fa_stamp_sink_set(khr, gather_planes(cp.dev, p5), gather_planes(cp.bytes, p5), gather_planes(cp.hazard_bit, p5))
    }
    return fa_stamp_set(khr, gather_planes(cp.dev, p), gather_planes(cp.bytes, p), gather_planes(cp.hazard_bit, p))
}

//! the f16-out twin is held to the f32 stamp's output through the clamp-convert, not to the oracle: the two stamps must agree bit for bit
def private fa_tile_run(t : T?; khr : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let tag = khr ? "fa_khr" : "fa_cm2"
        let nheads = 4
        let kv_mul = 2
        let rows = 77    // partial 64-tile: q0=64 has 13 valid rows — clamp loads + store discard run (the KHR tile: five 16-row tiles, the last 13 deep)
        let w0 = 64      // nonzero window base: the causal bound crosses K tiles mid-window
        let scale = 0.125
        //! the sink arms (gpt-oss): the h64 stamps seeded with a per-head sink logit, whole and under a sliding window
        for (arm in fixed_array((hs = 64, window = 0, sink = false), (hs = 128, window = 0, sink = false), (hs = 256, window = 0, sink = false),
                (hs = 512, window = 0, sink = false), (hs = 128, window = 40, sink = false), (hs = 64, window = 0, sink = true), (hs = 64, window = 40, sink = true))) {
            let hs = arm.hs
            let window = arm.window   //! a sliding layer's arm: the queries attend the `window` positions up to their own
            let sink = arm.sink
            let stag = sink ? " sink" : ""
            var sinkh : array<float>
            sinkh |> resize(nheads)
            for (hh in range(nheads)) {
                sinkh[hh] = 0.45 * float(hh) - 0.3   //! sink logits inside the scaled scores' range, one under and the rest over the row's typical max
            }
            let qd = nheads * hs
            let kvd = (nheads / kv_mul) * hs
            let kpos = w0 + rows
            let kpad = kpos + FA_SHADOW_SLACK_ROWS
            var qh <- row_fill(rows * qd, 41, 0, 0.05, 1.8, 73)
            var kh <- row_fill(kpos * kvd, 17, 0, 0.03, 1.4, 97)
            var vh <- row_fill(kpos * kvd, 53, 0, 0.04, 1.7, 89)
            kh |> resize(kpad * kvd)
            vh |> resize(kpad * kvd)
            for (i in range(kpos * kvd, kpad * kvd)) {   //! the slack rows carry a large finite value, not zero: a masked probability that is not exactly zero would drag it into the output
                kh[i] = 10000.0
                vh[i] = 10000.0
            }
            var y_cls : array<float>
            var y16_fused, y16_split : array<float16>
            with_cell_planes() $(var cp : CellPlanes) {
                let pq = cp |> in_(qh)
                let pk = cp |> in_(kh)
                let pv = cp |> in_(vh)
                let pk_shadow16 = cp |> out_words(int64(kpad * kvd) * 2l, NAN_SENTINEL_BITS)
                let pv_shadow16 = cp |> out_words(int64(kpad * kvd) * 2l, NAN_SENTINEL_BITS)
                let py = cp |> out_(rows * qd)
                //! the compared f16 planes under a NaN in both halves: a dead leg on either side reads as a mismatch
                let py16f = cp |> out_words(int64(rows * qd) * 2l, NAN_SENTINEL_BITS)   // the f16-out twin's plane
                let py16s = cp |> out_words(int64(rows * qd) * 2l, NAN_SENTINEL_BITS)   // the split pair's: f32 out -> the clamp-convert
                let psink = cp |> in_(sinkh)
                var sck = cp |> cls_set(fixed_array(pk, pk_shadow16), @@set_tower_clamp_cvt_cls)
                var scv = cp |> cls_set(fixed_array(pv, pv_shadow16), @@set_tower_clamp_cvt_cls)
                var sfa = fa_cell_set(cp, khr, sink, fixed_array(pq, pk_shadow16, pv_shadow16, py), psink)
                var sfa16 = fa_cell_set(cp, khr, sink, fixed_array(pq, pk_shadow16, pv_shadow16, py16f), psink)
                var scy = cp |> cls_set(fixed_array(py, py16s), @@set_tower_clamp_cvt_cls)
                var pcv = f16_rows_args(uint(kpad * kvd))
                let cvtwgs = int64((kpad * kvd / 4 + 255) / 256)
                var pcf = FaArgs(rows = uint(rows), w0 = uint(w0), qd = uint(qd),
                    kvd = uint(kvd), kv_mul = uint(kv_mul), kvbase = 0u, scale = scale, window = uint(window), sinkoff = 0u)
                var pcy = f16_rows_args(uint(rows * qd))
                cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                    enc_tower_clamp_cvt_cls(raw2, h2, sck, pcv, cvtwgs)
                    enc_tower_clamp_cvt_cls(raw2, h2, scv, pcv, cvtwgs)
                    enc_fa_tile(khr, hs, false, false, sink, false, raw2, h2, sfa, pcf, nheads, rows)
                    enc_fa_tile(khr, hs, false, false, sink, true, raw2, h2, sfa16, pcf, nheads, rows)
                    enc_tower_clamp_cvt_cls(raw2, h2, scy, pcy, int64((rows * qd / 4 + 255) / 256))   // the split pair's own convert over the f32 out: the fused twin's true peer
                }
                delete y_cls
                delete y16_fused
                delete y16_split
                y_cls <- cp |> plane_run(py)
                y16_fused <- cp |> plane_as(py16f, type<float16>, rows * qd)
                y16_split <- cp |> plane_as(py16s, type<float16>, rows * qd)
            }
            // oracle mirrors the kernel's f16 grid: q pre-scales then rounds (the kernel's
            // MatrixTimesScalar-then-convert), K/V round through the shadow; softmax stays f32. The cm2 kernel's
            // O accumulates in f16 under the row-max bias (every P at an eighth or under, the normalized row a
            // convex combination of V rows with |v| <= 1 on this fixture), so the f16 running sum's rounding
            // stays under the 2e-3 relative bar mismatch_approx holds against the f32 oracle, and the f16 O
            // twin against the f32 stamp pins the same bar - the bound is this fixture's magnitudes, not a proof.
            // The KHR kernel rounds P to f16 and sums each 64-key step in f16 before its f32 o row: the same bar.
            var qh2, kh2, vh2 : array<float>
            qh2 |> resize(rows * qd)
            kh2 |> resize(kpos * kvd)
            vh2 |> resize(kpos * kvd)
            for (i in range(rows * qd)) {
                qh2[i] = float(float16(qh[i] * scale))
            }
            for (i in range(kpos * kvd)) {
                kh2[i] = float(float16(kh[i]))
                vh2[i] = float(float16(vh[i]))
            }
            var y_ref : array<float>
            y_ref |> resize(rows * qd)
            for (r in range(rows)) {
                for (hh in range(nheads)) {
                    let cnt = w0 + r + 1
                    attn_row_oracle(qh2, r * qd + hh * hs, kh2, 0, vh2, 0, kvd, hh, hs, kv_mul,
                        cnt, 1.0, y_ref, r * qd + hh * hs, window > 0 && cnt > window ? cnt - window : 0, 0.0, sink, sinkh[hh])
                }
            }
            let bad = mismatch_approx(y_cls, y_ref)
            t |> success(bad == 0, "{tag} hs {hs} window {window}{stag} matches the CPU oracle ({bad} of {rows * qd} off)")
            y_ref[0] += 0.5
            t |> success(mismatch_approx(y_cls, y_ref) > 0, "{tag} hs {hs} window {window}{stag}: the bar reds a poisoned oracle element")
            if (sink) {   //! the sink control: the sink-free oracle over the same rows must miss - the phantom key carries mass
                var y_nosink : array<float>
                y_nosink |> resize(rows * qd)
                for (r in range(rows)) {
                    let cnt = w0 + r + 1
                    attn_heads_oracle(qh2, r * qd, kh2, 0, vh2, 0, kvd, nheads, hs, kv_mul, cnt, 1.0, y_nosink, r * qd, window > 0 && cnt > window ? cnt - window : 0)
                }
                let moved = mismatch_approx(y_cls, y_nosink)
                t |> success(moved > 0, "{tag} hs {hs} window {window} sink control: the sink moves the output off the sink-free oracle ({moved} of {rows * qd})")
                delete y_nosink
            }
            var bad16 = 0
            for (i in range(rows * qd)) {
                if (float(y16_fused[i]) != float(y16_split[i])) {
                    bad16++
                }
            }
            t |> success(bad16 == 0, "{tag} hs {hs}{stag} f16-out twin == the f32 stamp's clamp-convert ({bad16} of {rows * qd} off)")
            delete sinkh
            delete qh
            delete kh
            delete vh
            delete qh2
            delete kh2
            delete vh2
            delete y_ref
            delete y_cls
            delete y16_fused
            delete y16_split
        }
    }
}

[test]
def test_vkd_fa_cm2(t0 : T?) {
    t0 |> run("cm2 flash-attention family == the CPU oracle (h64 + h128 + h256, clamp-convert-staged planes)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_tower_clamp_cvt_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            if (!g_gpu.has_coopmat2_fa) {
                t |> skip("device lacks the cm2 flash-attention trio")
                return
            }
            verify(fa_arm_ensure(false), "the fa class rails and their f16-out twins must engage on an fa-capable device")
            fa_tile_run(t, false)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_fa_khr(t0 : T?) {
    t0 |> run("KHR flash-attention family == the CPU oracle (h64 + h128 + h256 + h512, the window arm, clamp-convert-staged planes)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_tower_clamp_cvt_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!khr_kq_tile_on()) {
                t |> skip("device lacks KHR cooperative matrices at subgroup 32")
                return
            }
            if (!fa_arm_ensure(true)) {
                t |> skip("the KHR fa stamps decline on this device: their 38912 B of workgroup memory is past its floor")
                return
            }
            fa_tile_run(t, true)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the h256 flash stamps at one geometry, the f32 and f16-out twins against the CPU oracle over the kernel's f16 grid. `gated`:
//! a [q | gate] q panel (per-head stride 2 x hs), the oracle scaled by the sigmoid of the gate half - the Qwen3.5 attention shape;
//! else the softcap stamps at gemma-2's shape, wide q and k values whose scaled scores run past `cap`, the oracle under the same
//! cap. The control is the plain oracle - ungated, or uncapped - which must miss the kernel. Returns (f32 mismatches, f16-vs-f32
//! mismatches, outputs compared - 0 when the control does not miss, f32 mismatches with one oracle element poisoned).
def private fa_h256_run(nheads, kv_mul, rows, w0 : int; gated : bool; cap : float; khr : bool) : tuple<bad : int; bad16 : int; n : int; poisoned : int> {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let hs = 256
        let qhs = gated ? 2 * hs : hs
        let scale = 0.0625
        let qd = nheads * hs
        let qrow = nheads * qhs
        let kvd = (nheads / kv_mul) * hs
        let kpos = w0 + rows
        let kpad = kpos + FA_SHADOW_SLACK_ROWS
        //! the softcap fixture's wide q and k values: the scaled scores run past the cap, so the cap is load-bearing
        var qp <- row_fill(rows * qrow, 41, 0, gated ? 0.05 : 0.25, gated ? 1.8 : 9.0, 73)
        var kh <- row_fill(kpos * kvd, 17, 0, gated ? 0.03 : 0.15, gated ? 1.4 : 7.0, 97)
        var vh <- row_fill(kpos * kvd, 53, 0, 0.04, 1.7, 89)
        kh |> resize(kpad * kvd)
        vh |> resize(kpad * kvd)
        var y_cls : array<float>
        var y16 : array<float16>
        with_cell_planes() $(var cp : CellPlanes) {
            let pq = cp |> in_(qp)
            let pk = cp |> in_(kh)
            let pv = cp |> in_(vh)
            let pk_shadow16 = cp |> out_words(int64(kpad * kvd) * 2l, NAN_SENTINEL_BITS)
            let pv_shadow16 = cp |> out_words(int64(kpad * kvd) * 2l, NAN_SENTINEL_BITS)
            let py = cp |> out_(rows * qd)
            let py16 = cp |> out_words(int64(rows * qd) * 2l, NAN_SENTINEL_BITS)
            var sck = cp |> cls_set(fixed_array(pk, pk_shadow16), @@set_tower_clamp_cvt_cls)
            var scv = cp |> cls_set(fixed_array(pv, pv_shadow16), @@set_tower_clamp_cvt_cls)
            var sfa = fa_cell_set(cp, khr, false, fixed_array(pq, pk_shadow16, pv_shadow16, py), -1)
            var sfa16 = fa_cell_set(cp, khr, false, fixed_array(pq, pk_shadow16, pv_shadow16, py16), -1)
            var pcv = f16_rows_args(uint(kpad * kvd))
            let cvtwgs = int64((kpad * kvd / 4 + 255) / 256)
            var pcf = FaArgs(rows = uint(rows), w0 = uint(w0), qd = uint(qd), kvd = uint(kvd), kv_mul = uint(kv_mul), kvbase = 0u,
                scale = scale, cap = cap, qstride = gated ? uint(qrow) : 0u, qhs = gated ? uint(qhs) : 0u)
            let capped = !gated
            cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                enc_tower_clamp_cvt_cls(raw2, h2, sck, pcv, cvtwgs)
                enc_tower_clamp_cvt_cls(raw2, h2, scv, pcv, cvtwgs)
                enc_fa_tile(khr, hs, gated, capped, false, false, raw2, h2, sfa, pcf, nheads, rows)
                enc_fa_tile(khr, hs, gated, capped, false, true, raw2, h2, sfa16, pcf, nheads, rows)
            }
            delete y_cls
            delete y16
            y_cls <- cp |> plane_run(py)
            y16 <- cp |> plane_as(py16, type<float16>, rows * qd)
        }
        // oracle: q (the gate halves too) pre-scales and rounds to f16 like the kernel, K / V round through the shadow; the gate reads the f32 panel
        var qp2 <- [for (x in qp); float(float16(x * scale))]
        var kh2 <- [for (i in range(kpos * kvd)); float(float16(kh[i]))]
        var vh2 <- [for (i in range(kpos * kvd)); float(float16(vh[i]))]
        var y_ref, y_plain : array<float>
        y_ref |> resize(rows * qd)
        y_plain |> resize(rows * qd)
        for (r in range(rows)) {
            for (hh in range(nheads)) {
                attn_row_oracle(qp2, r * qrow + hh * qhs, kh2, 0, vh2, 0, kvd, hh, hs, kv_mul,
                    w0 + r + 1, 1.0, y_ref, r * qd + hh * hs, 0, cap)
                attn_row_oracle(qp2, r * qrow + hh * qhs, kh2, 0, vh2, 0, kvd, hh, hs, kv_mul,
                    w0 + r + 1, 1.0, y_plain, r * qd + hh * hs)
                if (gated) {
                    for (c in range(hs)) {
                        let g = qp[r * qrow + hh * qhs + hs + c]
                        y_ref[r * qd + hh * hs + c] *= sigmoid_f32(g)
                    }
                }
            }
        }
        let bad = mismatch_approx(y_cls, y_ref)
        let bad_plain = mismatch_approx(y_cls, y_plain)   //! the control: the same rows without the gate or the cap must miss, or the fixture never reached them
        y_ref[hs + 3] += 0.5
        let poisoned = mismatch_approx(y_cls, y_ref)
        y_ref[hs + 3] -= 0.5
        var bad16 = 0
        // the f16 twin's 2e-3 bar: the O accumulator's f16 step is bounded as test_vkd_fa_cm2's oracle note states, on the same fixture magnitudes
        for (i in range(rows * qd)) {
            if (!(abs(float(y16[i]) - y_cls[i]) <= 2e-3 * max(1.0, abs(y_cls[i])))) {
                bad16++
            }
        }
        delete qp
        delete kh
        delete vh
        delete qp2
        delete kh2
        delete vh2
        delete y_ref
        delete y_plain
        delete y_cls
        delete y16
        return (bad = bad, bad16 = bad16, n = bad_plain > 0 ? rows * qd : 0, poisoned = poisoned)
    } else {
        return (bad = 0, bad16 = 0, n = 0, poisoned = 0)
    }
}

//! a gated h256 cell at one geometry: the gate moves the output, the kernel matches the gated oracle, the bar reds one poisoned
//! oracle element, the f16-out twin matches the f32 stamp rounded
def private fa_h256_gated_cell(t : T?; tag : string; nheads, kv_mul, rows, w0 : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (!ensure_tower_clamp_cvt_cls()) {
            t |> skip("no Vulkan device")
            return
        }
        if (!g_gpu.has_coopmat2_fa) {
            t |> skip("device lacks the cm2 flash-attention trio")
            return
        }
        verify(fa_stamp_pair_ensure(false, 256l, true, false, false, true), "the gated h256 fa rails must engage on an fa-capable device")
        let r = fa_h256_run(nheads, kv_mul, rows, w0, true, 0.0, false)
        t |> success(r.n > 0, "{tag}: the gate moves the output (the ungated oracle misses the kernel)")
        t |> success(r.bad == 0, "{tag}: matches the CPU oracle ({r.bad} of {r.n} off)")
        t |> equal(r.poisoned, 1, "{tag}: the bar reds one poisoned oracle element")
        t |> success(r.bad16 == 0, "{tag}: f16-out twin == the f32 stamp rounded ({r.bad16} of {r.n} off)")
    } else {
        t |> skip("dasVulkan not present")
    }
}

[test]
def test_vkd_fa_cm2_h256_gated(t0 : T?) {
    t0 |> run("cm2 flash-attention h256 gated == the CPU oracle (q at the head's stride, sigmoid gate on the output)") <| @(t : T?) {
        fa_h256_gated_cell(t, "fa_cm2 h256 gated", 4, 2, 77, 64)   // partial 64-tile, a nonzero window base
    }
}

//! the h256 softcap stamps under cap 50 on the cm2 tile or the KHR one (`khr`), 4 heads over 2 kv heads at 77 rows from
//! position 64 (a partial 64-tile, a nonzero window base): the capped oracle matches, the uncapped one misses, the f16-out twin
//! matches the f32 stamp rounded
def private fa_h256_softcap_cell(t : T?; khr : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (!ensure_tower_clamp_cvt_cls()) {
            t |> skip("no Vulkan device")
            return
        }
        if (khr ? !(g_gpu.has_coopmat && khr_kq_tile_on()) : !g_gpu.has_coopmat2_fa) {
            t |> skip(khr ? "device lacks KHR cooperative matrices at subgroup 32" : "device lacks the cm2 flash-attention trio")
            return
        }
        let tag = khr ? "fa_khr" : "fa_cm2"
        verify(fa_stamp_pair_ensure(khr, 256l, false, true, false, true), "the softcap h256 fa rails must engage on a device that serves the tile")
        let r = fa_h256_run(4, 2, 77, 64, false, 50.0, khr)
        t |> success(r.n > 0, "the softcap fixture's scores reach the cap (the uncapped oracle misses the kernel)")
        t |> success(r.bad == 0, "{tag} h256 softcap matches the capped CPU oracle ({r.bad} of {r.n} off)")
        t |> success(r.bad16 == 0, "{tag} h256 softcap f16-out twin == the f32 stamp rounded ({r.bad16} of {r.n} off)")
    } else {
        t |> skip("dasVulkan not present")
    }
}

[test]
def test_vkd_fa_cm2_h256_softcap(t0 : T?) {
    t0 |> run("cm2 flash-attention h256 softcap stamps == the CPU oracle under cap 50 (gemma-2's shape; the uncapped oracle as the control)") <| @(t : T?) {
        fa_h256_softcap_cell(t, false)
    }
}

[test]
def test_vkd_fa_khr_h256_softcap(t0 : T?) {
    t0 |> run("KHR flash-attention h256 softcap stamps == the CPU oracle under cap 50 (gemma-2's shape; the uncapped oracle as the control)") <| @(t : T?) {
        fa_h256_softcap_cell(t, true)
    }
}

// the 9B's own shape: 16 heads over 4 kv heads, a full 512-row window from position 0
[test]
def test_vkd_fa_cm2_h256_9b(t0 : T?) {
    t0 |> run("cm2 flash-attention h256 gated at the 9B geometry (16 heads / 4 kv, 512 rows at w0 0) == the CPU oracle") <| @(t : T?) {
        fa_h256_gated_cell(t, "fa_cm2 h256 gated 9B", 16, 4, 512, 0)
    }
}

let private DAB_B = 0          //! the batched decode attention stamps: the 8-row tile, the wide-head 4-row tile, the h128 coopmat tile, and the f16-mirror twins of all three
let private DAB_BW = 1
let private DAB_H128 = 2
let private DAB_B_F16 = 3
let private DAB_H128_F16 = 4
let private DAB_BW_F16 = 5

//! whether a DAB_* stamp reads f16 mirrors
def private dab_f16(stamp : int) : bool => stamp == DAB_B_F16 || stamp == DAB_H128_F16 || stamp == DAB_BW_F16

//! a batched decode attention fixture: `npos` query rows over mirrors holding positions [0, w0 + npos) past `kbase` / `vbase` words
struct private DaAttnBFix {
    qh, kh, vh : array<float>
    gate : array<float>   //! a gated model's gate half beside each head's q (`npos` x `nheads` x `hs`); empty = ungated
    npos, nheads, kv_mul, hs, w0, kbase, vbase : int
}

//! q at (i * 41) % 73 x `qm` - `qa`, K at (i * 17) % 97 x `km` - `ka`, V at (i * 53) % 89 x 0.04 - 1.7; `f16` puts K and V on the f16 grid the f16 mirrors hold
def private da_attn_b_fix(npos, nheads, kv_mul, hs, w0, kbase, vbase : int; qm, qa, km, ka : float; f16 : bool) : DaAttnBFix {
    let qd = nheads * hs
    let kvd = (nheads / kv_mul) * hs
    let kpos = w0 + npos
    var f = DaAttnBFix(npos = npos, nheads = nheads, kv_mul = kv_mul, hs = hs, w0 = w0, kbase = kbase, vbase = vbase)
    f.qh <- [for (i in range(npos * qd)); float((i * 41) % 73) * qm - qa]
    f.kh <- [for (i in range(kbase + kpos * kvd)); f16 ? f16r(float((i * 17) % 97) * km - ka) : float((i * 17) % 97) * km - ka]
    f.vh <- [for (i in range(vbase + kpos * kvd)); f16 ? f16r(float((i * 53) % 89) * 0.04 - 1.7) : float((i * 53) % 89) * 0.04 - 1.7]
    return <- f
}

//! the batched cells' oracle over the offset mirrors: row r attends [kstart, w0 + r], kstart the `window`'s start, every score under `cap`
def private da_attn_b_ref(f : DaAttnBFix; scale : float; window : int; cap : float) : array<float> {
    let qd = f.nheads * f.hs
    let kvd = (f.nheads / f.kv_mul) * f.hs
    var y : array<float>
    y |> resize(f.npos * qd)
    for (r in range(f.npos)) {
        let cnt = f.w0 + r + 1
        let kstart = window > 0 && cnt > window ? cnt - window : 0
        attn_heads_oracle(f.qh, r * qd, f.kh, f.kbase, f.vh, f.vbase, kvd, f.nheads, f.hs, f.kv_mul, cnt, scale, y, r * qd, kstart, cap)
    }
    if (!empty(f.gate)) {
        for (i in range(length(y))) {
            y[i] = sig_gate(y[i], f.gate[i])
        }
    }
    return <- y
}

//! the [q | gate] panel a gated model's q rows take: each head's `hs` q values, then its `hs` gate values
def private gated_q_panel(f : DaAttnBFix) : array<float> {
    let hs = f.hs
    return <- [for (i in range(2 * length(f.qh))); (i / hs) % 2 == 0 ? f.qh[(i / (2 * hs)) * hs + i % hs] : f.gate[(i / (2 * hs)) * hs + i % hs]]
}

//! one batched decode attention dispatch through `stamp` (a DAB_*) over the fixture: q, the K and V mirrors (f16 halves on an f16
//! stamp), the out plane under the sentinel, a workgroup a head per stamp tile of rows; the rows come back
def private da_attn_b_run(f : DaAttnBFix; stamp : int; scale, cap : float; window : int) : array<float> {
    var y : array<float>
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let f16 = dab_f16(stamp)
        let tile = stamp == DAB_BW || stamp == DAB_BW_F16 ? 4 : (stamp == DAB_H128 || stamp == DAB_H128_F16 ? 32 : 8)
        let qd = f.nheads * f.hs
        let kvd = (f.nheads / f.kv_mul) * f.hs
        let gated = !empty(f.gate)
        with_cell_planes() $(var cp : CellPlanes) {
            let pq = gated ? in_(cp, gated_q_panel(f)) : in_(cp, f.qh)
            let pk = f16 ? in_plain(cp, pack_f16(f.kh)) : in_(cp, f.kh)
            let pv = f16 ? in_plain(cp, pack_f16(f.vh)) : in_(cp, f.vh)
            let py = cp |> out_(f.npos * qd)
            let planes = fixed_array(pq, pk, pv, py)
            let bufs = gather_planes(cp.dev, planes)
            let sizes = gather_planes(cp.bytes, planes)
            let bits = gather_planes(cp.hazard_bit, planes)
            var pc = DaAttnBArgs(npos = uint(f.npos), nh = uint(f.nheads), kvd = uint(kvd), hs = uint(f.hs), kv_mul = uint(f.kv_mul),
                kbase = uint(f.kbase), vbase = uint(f.vbase), w0 = uint(f.w0), scale = scale, window = uint(window), cap = cap,
                qhs = gated ? uint(2 * f.hs) : 0u, gated = gated ? 1u : 0u)
            let wgs = int64(f.nheads * ((f.npos + tile - 1) / tile))
            cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                if (stamp == DAB_B) {
                    var sc = set_da_attn_b_cls(bufs, sizes, bits)
                    enc_da_attn_b_cls(raw2, h2, sc, pc, wgs)
                } elif (stamp == DAB_BW) {
                    var sc = set_da_attn_bw_cls(bufs, sizes, bits)
                    enc_da_attn_bw_cls(raw2, h2, sc, pc, wgs)
                } elif (stamp == DAB_H128) {
                    var sc = set_da_attn_b_h128_cls(bufs, sizes, bits)
                    enc_da_attn_b_h128_cls(raw2, h2, sc, pc, wgs)
                } elif (stamp == DAB_B_F16) {
                    var sc = set_da_attn_b_f16_cls(bufs, sizes, bits)
                    enc_da_attn_b_f16_cls(raw2, h2, sc, pc, wgs)
                } elif (stamp == DAB_BW_F16) {
                    var sc = set_da_attn_bw_f16_cls(bufs, sizes, bits)
                    enc_da_attn_bw_f16_cls(raw2, h2, sc, pc, wgs)
                } else {
                    var sc = set_da_attn_b_h128_f16_cls(bufs, sizes, bits)
                    enc_da_attn_b_h128_f16_cls(raw2, h2, sc, pc, wgs)
                }
            }
            y <- cp |> plane_run(py)
        }
    }
    return <- y
}

[test]
def test_vkd_da_attn_b(t0 : T?) {
    t0 |> run("batched decode attention class kernel == the CPU oracle (mirror offsets)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!ensure_da_attn_b_cls()) {
                t |> skip("da_attn_b class declined on this device (workgroup cap)")
                return
            }
            let scale = 0.125
            for (hs in [32, 128, 256]) {   // dpl 1 / 4 / 8 — every guarded accumulator arm
                //! 13 rows: a partial 8-tile, q0 = 8 has 5 valid rows - the row masks run; w0 32: kend crosses the 32-position K
                //! tile, the alpha rescale runs; the mirrors' word offsets 64 / 160 - the offset plumbing must survive the port
                var f <- da_attn_b_fix(13, 4, 2, hs, 32, 64, 160, 0.05, 1.8, 0.03, 1.4, false)
                let n = length(f.qh)
                var y_cls <- da_attn_b_run(f, DAB_B, scale, 0.0, 0)
                var y_ref <- da_attn_b_ref(f, scale, 0, 0.0)
                let bad = mismatch_approx(y_cls, y_ref)
                t |> success(bad == 0, "da_attn_b hs {hs} matches the CPU oracle ({bad} of {n} off)")
                // the softcap arm (gemma-2): the same tile under a cap the scaled scores overrun
                let cap = 4.0
                var y_cap <- da_attn_b_run(f, DAB_B, scale, cap, 0)
                let moved = mismatch_approx(y_cap, y_ref)
                t |> success(moved > 0, "control: the {cap} cap moves the uncapped rows ({moved} of {n})")
                var y_ref_cap <- da_attn_b_ref(f, scale, 0, cap)
                let badc = mismatch_approx(y_cap, y_ref_cap)
                t |> success(badc == 0, "da_attn_b hs {hs} under a {cap} softcap matches the CPU oracle ({badc} of {n} off)")
                delete f
                delete y_ref
                delete y_cls
                delete y_cap
                delete y_ref_cap
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// the wide-head twin (heads past 256): the 4-row q tile over 16-position K/V stages, at 384 (the
// guarded eighth dim lane) and 512, with the window mask on the long arm
[test]
def test_vkd_da_attn_bw(t0 : T?) {
    t0 |> run("wide-head batched decode attention class kernel == the CPU oracle (mirror offsets, the window)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!ensure_da_attn_bw_cls()) {
                t |> skip("da_attn_bw class declined on this device (workgroup cap)")
                return
            }
            if (!ensure_da_attn_bw_f16_cls()) {
                t |> skip("da_attn_bw_f16 class declined on this device (workgroup cap) - the f16-mirror arms")
            }
            let scale = 1.0     // the gemma-4 score scale
            let f16_on = ensure_da_attn_bw_f16_cls()   //! the f16-mirror twin (the gemma-4 global heads on the f16 mirror) runs beside every arm
            for (arm in range(4)) {
                let hs = arm == 0 ? 384 : 512
                let npos = arm >= 2 ? 120 : 13    //! 13: a partial 4-tile and a stage crossing; 120: thirty 4-tiles over a 620-position mirror (the second prefill window's shape)
                let w0 = arm >= 2 ? 500 : 32
                let window = arm == 3 ? 24 : 0    //! the last arm slides: every row sees the 24 positions up to its own
                //! four heads over one kv head, the gemma-4 global layers' shape; the mirrors' word offsets 64 / 160
                var f <- da_attn_b_fix(npos, 4, 4, hs, w0, 64, 160, 0.005, 0.18, 0.003, 0.14, false)
                var y_cls <- da_attn_b_run(f, DAB_BW, scale, 0.0, window)
                var y_ref <- da_attn_b_ref(f, scale, window, 0.0)
                let bad = mismatch_approx(y_cls, y_ref)
                t |> success(bad == 0, "da_attn_bw hs {hs} npos {npos} window {window} matches the CPU oracle ({bad} of {length(y_ref)} off)")
                y_ref[0] += 0.5
                t |> success(mismatch_approx(y_cls, y_ref) > 0, "da_attn_bw hs {hs} npos {npos} window {window}: the bar reds a poisoned oracle element")
                if (f16_on) {
                    var f16 <- da_attn_b_fix(npos, 4, 4, hs, w0, 64, 160, 0.005, 0.18, 0.003, 0.14, true)
                    var y16 <- da_attn_b_run(f16, DAB_BW_F16, scale, 0.0, window)
                    var y16_ref <- da_attn_b_ref(f16, scale, window, 0.0)
                    check_rows(t, "da_attn_bw_f16 hs {hs} npos {npos} window {window} against the CPU oracle", y16, y16_ref, BAR_APPROX_CTRL, 4 * hs, hs)
                    delete f16
                    delete y16
                    delete y16_ref
                }
                delete f
                delete y_ref
                delete y_cls
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_da_attn_b_h128(t0 : T?) {
    t0 |> run("h128 coopmat attention class kernel == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!ensure_da_attn_b_cls()) {   // probe via the NON-coopmat twin — a cm pipeline build
                t |> skip("da_attn_b class declined on this device (workgroup cap)")   // on a no-coopmat device panics, not feints
                return
            }
            if (!g_gpu.has_coopmat) {
                t |> skip("no coopmat on this device")
                return
            }
            verify(ensure_da_attn_b_h128_cls(), "the h128 class rail must engage on a coopmat device")
            let scale = 0.125
            var f <- da_attn_b_fix(45, 4, 2, 128, 32, 64, 160, 0.05, 1.8, 0.03, 1.4, false)   // 45 rows, two q tiles: the second has 13 valid rows - the row masks run
            var y_cls <- da_attn_b_run(f, DAB_H128, scale, 0.0, 0)
            // the h128 twin stages q/K/V through f16 pairs for the MMAs — floor-barred like the
            // cm GEMMs (softmax weights bound the V fold, so the peak floor is generous enough)
            var y_ref <- da_attn_b_ref(f, scale, 0, 0.0)
            check_rows(t, "da_attn_b_h128 against the CPU oracle", y_cls, y_ref, cm_bar(y_ref), 4 * 128, 128)
            delete f
            delete y_ref
            delete y_cls
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! one gated chunked-attention arm: q read at the head's [q | gate] stride, the output under the sigmoid of the gate half -
//! the gated oracle matches, the ungated one misses, the bar reds a poisoned element
//! `bar_of` reads the bar off the oracle rows: a stamp staging q/K/V through halves takes `cm_bar`, the f32 stamps a fixed bar
def private da_attn_b_gated_arm(t : T?; tag : string; stamp, npos, kv_mul, hs, w0 : int; qm, qa, km, ka : float; bar_of : block<(want : array<float>) : Bar>) {
    var f <- da_attn_b_fix(npos, 4, kv_mul, hs, w0, 64, 160, qm, qa, km, ka, dab_f16(stamp))
    f.gate <- [for (i in range(length(f.qh))); float((i * 29) % 61) * 0.08 - 2.4]
    let scale = hs > 256 ? 1.0 : 0.125
    var y_cls <- da_attn_b_run(f, stamp, scale, 0.0, 0)
    var y_ref <- da_attn_b_ref(f, scale, 0, 0.0)
    let bar = invoke(bar_of, y_ref)
    check_rows(t, tag, y_cls, y_ref, bar, 4 * hs, hs)
    delete f.gate
    var y_plain <- da_attn_b_ref(f, scale, 0, 0.0)
    t |> success(check_count(y_cls, y_plain, bar) > 0, "{tag}: the gate moves the output (the ungated oracle misses the kernel)")
    delete f
    delete y_cls
    delete y_ref
    delete y_plain
}

[test]
def test_vkd_da_attn_b_gated(t0 : T?) {
    t0 |> run("gated chunked attention == the gated CPU oracle on the 8-row, wide and h128 stamps") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!(ensure_da_attn_b_cls() && ensure_da_attn_bw_cls())) {
                t |> skip("da_attn_b classes declined on this device (workgroup cap)")
                return
            }
            da_attn_b_gated_arm(t, "da_attn_b gated hs 128", DAB_B, 13, 2, 128, 32, 0.05, 1.8, 0.03, 1.4) $(want : array<float>) : Bar {
                return BAR_APPROX
            }
            da_attn_b_gated_arm(t, "da_attn_bw gated hs 512", DAB_BW, 13, 4, 512, 32, 0.005, 0.18, 0.003, 0.14) $(want : array<float>) : Bar {
                return BAR_APPROX
            }
            if (ensure_da_attn_bw_f16_cls()) {
                da_attn_b_gated_arm(t, "da_attn_bw_f16 gated hs 512", DAB_BW_F16, 13, 4, 512, 32, 0.005, 0.18, 0.003, 0.14) $(want : array<float>) : Bar {
                    return BAR_APPROX_CTRL
                }
            } else {
                t |> skip("da_attn_bw_f16 class declined on this device (workgroup cap) - the f16 gated arm")
            }
            if (!g_gpu.has_coopmat) {
                t |> skip("no coopmat on this device - the h128 gated arm")
                return
            }
            if (!ensure_da_attn_b_h128_cls()) {
                t |> failure("the h128 class rail must engage on a coopmat device")
                return
            }
            da_attn_b_gated_arm(t, "da_attn_b_h128 gated", DAB_H128, 45, 2, 128, 32, 0.05, 1.8, 0.03, 1.4) $(want : array<float>) : Bar {
                return cm_bar(want)   //! the h128 stamp stages q/K/V through f16 pairs for its MMAs
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_requant_family(t0 : T?) {
    t0 |> run("fused ar+requant + q8k requant class kernels == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_cls_ar_rq_b() && ensure_cls_q8k_rq() && ensure_cls_ar_rqx() && ensure_cls_ar_rqk() && ensure_cls_ar_rqkx())) {
                t |> skip("no Vulkan device")
                return
            }
            // ===== the fused ar + Q8_0 requant =====
            {
                let dim = 1024
                let eps = 1e-5
                let ascale = 1.25
                var fx <- ar_fixture(dim, dim, 0.09, 4.2, 0.06, 2.6, 0.04)
                var x <- fx.x
                var a <- fx.a
                var w <- fx.w
                // the class kernel: x@0, a@1, w@2, outq@3, outs@4
                let pc = ArArgs(dim = uint(dim), add_on = 1u, eps = eps, ascale = ascale)
                var r <- ar_rq_run("cls_ar_rq_b", x, a, w, pc, false, false)
                var q_cls <- r.q
                var s_cls <- r.s
                var x_cls <- r.x
                delete r
                // oracle: x' = (x+a)*ascale in place, rms over the row, y = w*(x'*inv), Q8_0 pack —
                // the wg reduce reassociates the sum of squares, so quants ride the byte bar
                var x_ref, y_ref : array<float>
                x_ref |> resize(dim)
                y_ref |> resize(dim)
                for (i in range(dim)) {
                    x_ref[i] = (x[i] + a[i]) * ascale
                }
                rmsnorm(y_ref, x_ref, w, int64(dim), eps)
                var q_ref : array<uint>
                var s_ref : array<float>
                requant_oracle(y_ref, 0, dim, false, q_ref, s_ref)
                check_qbytes(t, "ar+requant fused", q_cls, q_ref)
                check_rows(t, "ar+requant fused, scales", s_cls, s_ref, BAR_APPROX_CTRL, FLAT_ROWS)
                check_rows(t, "ar+requant fused, the residual row", x_cls, x_ref, BAR_APPROX_CTRL, FLAT_ROWS)
                //! the biased add partner (gpt-oss's output bias) on the fused stamp: the bias row rides the norms plane past the norm row
                {
                    var b <- row_fill(dim, 23, 0, 0.09, 3.1, 71)
                    var wb <- biased_norms(w, b)
                    let pcb = ArArgs(dim = uint(dim), add_on = 1u, eps = eps, ascale = ascale, abias_on = 1u, aboff = uint(dim))
                    var rb <- ar_rq_run("cls_ar_rq_b", x, a, wb, pcb, false, false)
                    var qb_cls <- rb.q
                    var sb_cls <- rb.s
                    var xb_cls <- rb.x
                    delete rb
                    var xb_ref, yb_ref : array<float>
                    xb_ref |> resize(dim)
                    yb_ref |> resize(dim)
                    for (i in range(dim)) {
                        xb_ref[i] = (x[i] + a[i] + b[i]) * ascale
                    }
                    rmsnorm(yb_ref, xb_ref, w, int64(dim), eps)
                    var qb_ref : array<uint>
                    var sb_ref : array<float>
                    requant_oracle(yb_ref, 0, dim, false, qb_ref, sb_ref)
                    let badb = (mismatch_qbytes(qb_cls, qb_ref, 1) + mismatch_approx(sb_cls, sb_ref)
                        + mismatch_approx(xb_cls, xb_ref))
                    t |> success(badb == 0, "ar+requant fused with the biased partner matches the CPU oracle ({badb} elements off)")
                    t |> success(mismatch_approx(xb_cls, x_ref) > 0, "biased partner control: the bias moves the residual off the unbiased step ({mismatch_approx(xb_cls, x_ref)} of {dim})")
                    delete b
                    delete wb
                    delete qb_cls
                    delete sb_cls
                    delete xb_cls
                    delete xb_ref
                    delete yb_ref
                    delete qb_ref
                    delete sb_ref
                }
                //! the three stamps beside it over the same x, a, w (x re-uploaded: the add writes it in place)
                var q_ref_k : array<uint>
                var s_ref_k : array<float>
                requant_oracle(y_ref, 0, dim, true, q_ref_k, s_ref_k)
                let stamps <- fixed_array(
                    (name = "cls_ar_rqx", kform = false, storex = true),
                    (name = "cls_ar_rqk", kform = true, storex = false),
                    (name = "cls_ar_rqkx", kform = true, storex = true))
                for (stamp in stamps) {
                    let kform = stamp.kform
                    let storex = stamp.storex
                    var r3 <- ar_rq_run(stamp.name, x, a, w, pc, kform, storex)
                    var q_got <- r3.q
                    var s_got <- r3.s
                    var y_got <- r3.y
                    delete r3
                    var badk = 0
                    if (kform) {
                        badk = mismatch_qbytes(q_got, q_ref_k, 1) + mismatch_approx(s_got, s_ref_k)
                        //! the Q8_K bars' control: one scale pushed past the relative bar, one quant byte two steps off
                        var s_poison := s_ref_k
                        s_poison[1] += 0.01 + 0.01 * abs(s_ref_k[1])
                        var q_poison := q_ref_k
                        q_poison[3] = q_poison[3] ^ 0x02020202u
                        t |> success(mismatch_approx(s_got, s_poison) > 0 && mismatch_qbytes(q_got, q_poison, 1) > 0,
                            "{stamp.name}: the Q8_K scale and quant bars red a poisoned expectation")
                        delete s_poison
                        delete q_poison
                    } else {
                        badk = mismatch_qbytes(q_got, q_cls, 0) + mismatch_approx(s_got, s_cls)
                    }
                    if (storex) {
                        badk += mismatch_approx(y_got, y_ref)
                        var poison := y_got   // the row bar's control: one element pushed past it must red
                        poison[7] += 1.0 + 2.0 * abs(y_ref[7])
                        t |> success(mismatch_approx(poison, y_ref) > 0, "{stamp.name}: the row bar reds a poisoned element")
                        delete poison
                    }
                    t |> success(badk == 0, "{stamp.name} (Q8_K {kform}, the row stored {storex}) matches ({badk} elements off)")
                    delete q_got
                    delete s_got
                    delete y_got
                }
                delete q_ref_k
                delete s_ref_k
                delete x
                delete a
                delete w
                delete x_ref
                delete y_ref
                delete q_ref
                delete s_ref
                delete q_cls
                delete s_cls
                delete x_cls
            }
            // ===== the Q8_K requant =====
            {
                let nsb = 4
                let nelem = nsb * 256
                var src <- row_fill(nelem, 61, 0, 0.05, 2.8, 113)
                var q_cls : array<uint>
                var s_cls : array<float>
                with_cell_planes() $(var cp : CellPlanes) {
                    // the class kernel: src@0, outq@1, outs@2
                    let psrc = cp |> in_(src)
                    let pq = cp |> out_words(int64(nelem), I8_SENTINEL_BITS)
                    let ps = cp |> out_(nsb)
                    var sc = cp |> cls_set(fixed_array(psrc, pq, ps), @@set_rq_cls)
                    var pc = RqArgs(inbase = 0u, nblk = uint(nsb))
                    cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                        enc_cls_q8k_rq(raw2, h2, sc, pc, int64((nsb * 32 + 255) / 256))
                    }
                    delete q_cls
                    delete s_cls
                    q_cls <- cp |> plane_as(pq, type<uint>, nelem / 4)
                    s_cls <- cp |> plane_run(ps)
                }
                // plain requant of f32 rows — amax is order-free, the pack deterministic: exact
                var q_ref : array<uint>
                var s_ref : array<float>
                requant_oracle(src, 0, nelem, true, q_ref, s_ref)
                let bad = mismatch_qbytes(q_cls, q_ref, 0) + mismatch_exact(s_cls, s_ref)
                t |> success(bad == 0, "q8k requant matches the CPU oracle bit-exact ({bad} elements off)")
                delete src
                delete q_ref
                delete s_ref
                delete q_cls
                delete s_cls
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_q51_gemv(t0 : T?) {
    t0 |> run("q51 GEMV class kernel == the scalar dot's float order (one lane a block over Q8_0 activations; 704 and 1408 wide)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q51_gemv_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            //! 704: 22 blocks, one subgroup pass with idle lanes; 1408: 44, a second, partial pass
            per32_gemv_cell(t, KqFmt.q51, [704, 1408], 5, 61u, 19u, 7u, @@q51_gemv_row_oracle) $(totblk : int) {
                return <- [for (b in range(totblk)); packHalf2x16(float2(0.002 * float(1 + b % 7), 0.001 * float(b % 5) - 0.002))]
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_iq4nl32_gemv(t0 : T?) {
    t0 |> run("iq4nl32 GEMV class kernel == the scalar dot's float order (one lane a block over Q8_0 activations; 640 and 1280 wide)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_iq4nl32_gemv_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            //! 640: 20 blocks, one subgroup pass with idle lanes; 1280: 40, a second, partial pass
            per32_gemv_cell(t, KqFmt.iq4nl32, [640, 1280], 4, 67u, 23u, 5u, @@iq4nl32_gemv_row_oracle) $(totblk : int) {
                return <- iq4nl32_scale_words(totblk, 0.002)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_mx4_gemv(t0 : T?) {
    t0 |> run("mx4 GEMV class kernel == the scalar dot's float order (one lane a block over Q8_0 activations; 704 and 2880 wide)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_mx4_gemv_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            //! 704: 22 blocks, one subgroup pass with idle lanes; 2880: 90, gpt-oss's width, three passes, the last partial
            per32_gemv_cell(t, KqFmt.mx4, [704, 2880], 4, 67u, 19u, 7u, @@mx4_gemv_row_oracle) $(totblk : int) {
                return <- mx4_scale_words(totblk)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the fused mx4 gate + up + act + requant on one fixture: the gate stack's rows then the up stack's in one plane, the slots'
//! regions on both, a six-expert bias plane and the slots' experts; the fused dispatch against the oracle (the bias added before
//! the clamped swiglu), then the split path - two GEMVs and the biased requant - byte for byte
def private mx4_gemv_gu_run(t : T?; d, nreg : int; bias : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let n = 2880   //! gpt-oss's routed width: 90 blocks a row
        let nb = n / 32
        let rows = nreg * d
        let nblk = rows / 32
        let ne = 6
        let totblk = 2 * rows * nb   // the gate stack's blocks, then the up stack's
        var wqh <- hashed_words(totblk * 4, 5u, 977u)
        var xqh <- hashed_words(nb * 8, 29u, 11u)
        var xsh <- row_fill(nb, 1, 0, 0.125, -0.25, 5)
        var gb, ub : array<float>
        var rex : array<uint>
        gb |> resize(ne * d)
        ub |> resize(ne * d)
        rex |> resize(nreg)
        var wsh <- mx4_scale_words(totblk)
        for (i in range(ne * d)) {   //! biases of the dots' own magnitude, some past the 7 clamp
            gb[i] = bias ? float((i * 13) % 31) * 0.5 - 7.5 : 0.0
            ub[i] = bias ? float((i * 17) % 29) * 0.6 - 8.4 : 0.0
        }
        for (r in range(nreg)) {
            rex[r] = uint((r * 5 + 2) % ne)
        }
        var gu <- gu_regions(nreg, d, nb)
        let act = bias ? ACT_OAI : ACT_SILU
        var oq : array<int8>
        var os, y_cls : array<float>
        var oq2 : array<int8>
        var os2 : array<float>
        with_cell_planes() $(var cp : CellPlanes) {
            let pwq = cp |> in_plain(wqh)
            let pws = cp |> in_plain(wsh)
            let prg = cp |> in_plain(gu.g)
            let pxq = cp |> in_plain(xqh)
            let pxs = cp |> in_(xsh)
            let py = cp |> out_(rows)
            let prg3 = cp |> in_plain(gu.u)
            let pgb = cp |> in_(gb)
            let pub = cp |> in_(ub)
            let prex = cp |> in_plain(rex)
            let poq = cp |> out_words(int64(rows), NAN_SENTINEL_BITS)
            let pos = cp |> out_(nblk)
            let pyg = cp |> out_(rows)   //! the split path on the same planes: the two GEMVs, then the (biased) act + requant
            let pyu = cp |> out_(rows)
            let poq2 = cp |> out_words(int64(rows), NAN_SENTINEL_BITS)
            let pos2 = cp |> out_(nblk)
            var s_cls = cp |> cls_set(fixed_array(pwq, pws, prg, pxq, pxs, py, prg3, pgb, pub, prex, poq, pos), @@set_mx4_gemv_gu_cls)
            let gate = fixed_array(pwq, pws, prg, pxq, pxs, pyg)
            let up = fixed_array(pwq, pws, prg3, pxq, pxs, pyu)
            var s_g = gemv_cls_set(int(KqFmt.mx4), gather_planes(cp.dev, gate), gather_planes(cp.bytes, gate), gather_planes(cp.hazard_bit, gate))
            var s_u = gemv_cls_set(int(KqFmt.mx4), gather_planes(cp.dev, up), gather_planes(cp.bytes, up), gather_planes(cp.hazard_bit, up))
            var pc = GemvArgs(n = uint(n), d = uint(d), nreg = uint(nreg), ybase = 0u, act = act, bias_on = bias ? 1u : 0u, gboff = 0u, uboff = 0u)
            var pc2 = GemvArgs(n = uint(n), d = uint(d), nreg = uint(nreg), ybase = 0u)
            var pca = ActArgs(nelem = uint(rows), act = act, nblk = uint(nblk), bias_on = bias ? 1u : 0u, gboff = 0u, uboff = 0u, brow = uint(d))
            cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                enc_mx4_gemv_gu_cls(raw2, h2, s_cls, pc, int64(nblk))
                gemv_enc(int(KqFmt.mx4), raw2, h2, s_g, pc2, int64(rows))
                gemv_enc(int(KqFmt.mx4), raw2, h2, s_u, pc2, int64(rows))
                if (bias) {
                    var s_a = cp |> cls_set(fixed_array(pyg, pyu, poq2, pos2, pgb, pub, prex), @@set_q8_actrq_b_cls)
                    enc_q8_actrq_b_cls(raw2, h2, s_a, pca, (int64(rows) / 4l + 255l) / 256l)
                } else {
                    var s_a = cp |> cls_set(fixed_array(pyg, pyu, poq2, pos2), @@set_actrq_cls)
                    enc_q8_actrq_cls(raw2, h2, s_a, pca, (int64(rows) / 4l + 255l) / 256l)
                }
            }
            delete oq
            delete os
            delete y_cls
            delete oq2
            delete os2
            oq <- cp |> plane_as(poq, type<int8>, rows)
            os <- cp |> plane_run(pos)
            y_cls <- cp |> plane_run(py)
            oq2 <- cp |> plane_as(poq2, type<int8>, rows)
            os2 <- cp |> plane_run(pos2)
        }
        var y_g, y_u : array<float>
        y_g |> resize(rows)
        y_u |> resize(rows)
        for (r in range(nreg)) {
            for (row in range(d)) {
                y_g[r * d + row] = mx4_gemv_row_oracle(wqh, wsh, xqh, xsh, r * d * nb + row * nb, nb)
                y_u[r * d + row] = mx4_gemv_row_oracle(wqh, wsh, xqh, xsh, (nreg + r) * d * nb + row * nb, nb)
            }
        }
        let badg = mismatch_approx(y_cls, y_g)
        var y_gp := y_g   //! the gate rows' control: one element pushed past the bar must red
        y_gp[0] += 1.0 + 2.0 * logits_maxabs(y_g)
        let badgp = mismatch_approx(y_cls, y_gp)
        //! the activated reference row: each slot row's expert biases added before the act
        var a_ref <- [for (rg in range(nblk * 32)); act_mul_ref(y_g[rg] + gb[int(rex[rg / d]) * d + rg % d], y_u[rg] + ub[int(rex[rg / d]) * d + rg % d], act)]
        let tag = "mx4 gu {nreg} x {d}{bias ? " biased" : ""}"
        t |> success(badg == 0, "{tag}: the gate rows match the scalar dot ({badg} of {rows} off)")
        t |> success(badgp == 1, "{tag}: the gate rows' bar reds one poisoned element ({badgp})")
        delete y_gp
        gu_blocks_hold(t, tag, oq, os, a_ref, nblk)
        delete a_ref
        let bad = mismatch_q8(oq, os, oq2, os2, 0.0, 0)
        t |> success(bad == 0, "{tag}: the hidden rows match the split path byte for byte ({bad} of {rows} quants and {nblk} scales off)")
        delete wqh
        delete wsh
        delete xqh
        delete xsh
        delete gb
        delete ub
        delete rex
        delete gu
        delete oq
        delete os
        delete oq2
        delete os2
        delete y_cls
        delete y_g
        delete y_u
    }
}

[test]
def test_vkd_mx4_gemv_gu(t0 : T?) {
    t0 |> run("mx4 gate + up GEMVs, the biased clamped-swiglu act and its Q8_0 requant in one dispatch == the scalar dot and the split path (gpt-oss's routed step)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_mx4_gemv_gu_cls() && ensure_mx4_gemv_cls() && ensure_q8_actrq_b_cls() && ensure_q8_actrq_cls())) {   //! the split path's rails for the byte compare
                t |> skip("no Vulkan device")
                return
            }
            mx4_gemv_gu_run(t, 64, 3, true)     //! two blocks a slot, the biases past the clamp
            mx4_gemv_gu_run(t, 2880, 4, true)   //! gpt-oss's four slots of 2880
            mx4_gemv_gu_run(t, 64, 3, false)    //! the plain silu form, no bias plane
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the fused gate + up + act + Q8_0 requant dispatch against the CPU oracle: the two q8 GEMVs in the scalar order,
//! the activation, and the block quant of the hidden row - the dequantized row within a quant step of the oracle's,
//! the scales within the GEMV bar, under both activations and at a width whose block count is off the 16-block lane pass
def private q8_gemv_gu_run(t; n, hid : int; act : uint) {
    let nb = n / 32
    let nblk_out = hid / 32
    let totblk = hid * nb
    var gqh, gsh, uqh, ush : array<uint>
    q8_plane(totblk, 1u, 71u, 1u, 3u, gqh, gsh)
    q8_plane(totblk, 3u, 5u, 7u, 11u, uqh, ush)
    var xqh <- hashed_words(nb * 8, 17u, 9u)
    var xsh <- row_fill(nb, 1, 0, 0.125, -0.25, 5)
    var oq : array<int8>
    var os : array<float>
    with_cell_planes() $(var cp : CellPlanes) {
        let pg = cp |> in_plain(gqh)
        let pgs = cp |> in_plain(gsh)
        let pu = cp |> in_plain(uqh)
        let pus = cp |> in_plain(ush)
        let pxq = cp |> in_plain(xqh)
        let pxs = cp |> in_(xsh)
        let poq = cp |> out_words(int64(hid), NAN_SENTINEL_BITS)
        let pos = cp |> out_(nblk_out)
        var s_cls = cp |> cls_set(fixed_array(pg, pgs, pu, pus, pxq, pxs, poq, pos), @@set_q8_gemv_gu_n1_cls)
        var pc = GuNArgs(n = uint(n), d = uint(hid), gblk = 0u, ublk = 0u, act = act, ncols = 1u)
        cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
            enc_q8_gemv_gu_n1_cls(raw, h, s_cls, pc, int64(nblk_out))
        }
        delete oq
        oq <- cp |> plane_as(poq, type<int8>, hid)
        delete os
        os <- cp |> plane_run(pos)
    }
    // the oracle: the q8 GEMM oracle over one record with the x row as its one column, per plane, then the activation
    let one_row <- [0u, 0u, 1u, 0u]
    var g_ref, u_ref : array<float>
    g_ref |> resize(hid)
    u_ref |> resize(hid)
    q8_gemm_oracle(gqh, gsh, xqh, xsh, one_row, 1, n, hid, g_ref)
    q8_gemm_oracle(uqh, ush, xqh, xsh, one_row, 1, n, hid, u_ref)
    var hidden : array<float>
    hidden |> resize(hid)
    for (row in range(hid)) {
        hidden[row] = act_mul_ref(g_ref[row], u_ref[row], act)
    }
    gu_blocks_hold(t, "q8 gu act {act} n {n} hid {hid}", oq, os, hidden, nblk_out)
    delete hidden
    delete gqh
    delete gsh
    delete uqh
    delete ush
    delete xqh
    delete xsh
    delete oq
    delete os
}

[test]
def test_vkd_q8_gemv_gu(t0 : T?) {
    t0 |> run("fused q8 gate + up GEMV + act + Q8_0 requant == the CPU oracle (gelu, silu and the clamped swiglu; 36 and 16 blocks deep)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_gemv_gu_n1_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            q8_gemv_gu_run(t, 1152, 256, ACT_GELU)   // gemma-3-1b's dim: 36 blocks, a partial third lane pass; eight requant blocks
            q8_gemv_gu_run(t, 512, 96, ACT_SILU)    // 16 blocks: exactly one lane pass; three blocks
            q8_gemv_gu_run(t, 512, 96, ACT_OAI)   //! the third branch of the kernel's act select, gpt-oss's clamped swiglu
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the fused per-layer-embedding tail: gelu(gate) x the side row, its Q8_0 blocks and the proj GEMV in one dispatch,
//! against the same chain in the scalar order; the bar is the dot envelope (a one-step quant flip at a rounding tie
//! sits far under it), with a poisoned expectation that must red
def private q8_gemv_pleact_run(t; n, d : int) {
    let nb = n / 32
    let totblk = d * nb
    var wqh, wsh : array<uint>
    q8_plane(totblk, 5u, 19u, 3u, 7u, wqh, wsh)
    var gh <- row_fill(n, 37, 0, 0.06, 3.0, 101)
    var sh <- row_fill(3 * n, 29, 0, 0.05, 2.0, 83)   // three layers' side rows: the cell reads the middle one
    var y_cls : array<float>
    with_cell_planes() $(var cp : CellPlanes) {
        let py = cp |> out_(d)
        var s_cls = cp |> cls_set(fixed_array(cp |> in_plain(wqh), cp |> in_plain(wsh), cp |> in_(gh), cp |> in_(sh), py), @@set_q8_gemv_pleact_cls)
        var pc = PleActArgs(n = uint(n), d = uint(d), wblk = 0u, ubase = uint(n), act = ACT_GELU)
        let rpw = 4l * g_gpu.rows_per_wg
        cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
            enc_q8_gemv_pleact_cls(raw, h, s_cls, pc, (int64(d) + rpw - 1l) / rpw)
        }
        delete y_cls
        y_cls <- cp |> plane_run(py)
    }
    // the oracle: the activated row through the engine's Q8_0 quantizer, then each row's dot in the scalar block order
    let act <- [for (e in range(n)); act_mul_ref(gh[e], sh[n + e], true)]
    var qx <- q8_roundtrip(act, int64(n), false)
    var xq <- [for (q in qx.xq); int(q)]
    var xs <- qx.xs
    delete qx
    var bad = 0
    var pbad = 0
    for (row in range(d)) {
        var acc = 0.0
        var env = 0.0
        for (b in range(nb)) {
            let wb = row * nb + b
            let ws16 = wb % 2 == 0 ? unpackHalf2x16(wsh[wb / 2]).x : unpackHalf2x16(wsh[wb / 2]).y
            var idot = 0
            var ienv = 0
            for (j in range(32)) {
                let w = int(int8(uint8((wqh[wb * 8 + j / 4] >> uint((j % 4) * 8)) & 0xFFu)))
                idot += w * xq[b * 32 + j]
                ienv += abs(w * xq[b * 32 + j])
            }
            acc += float(idot) * ws16 * xs[b]
            env += float(ienv) * abs(ws16) * xs[b]
        }
        let bar = 2e-3 * env + 1e-6
        if (!(abs(y_cls[row] - acc) <= bar)) {
            bad++
        }
        if (abs(y_cls[row] - (acc + 3.0 * bar)) <= bar) {   // the control: the expectation pushed three bars must miss
            pbad++
        }
    }
    t |> success(bad == 0, "q8 pleact n {n} d {d}: the proj row matches the CPU chain ({bad} of {d} rows off)")
    t |> success(pbad == 0, "q8 pleact n {n} d {d}: the bar reds a poisoned expectation on every row ({pbad} passed)")
    delete wqh
    delete wsh
    delete gh
    delete sh
    delete xq
    delete xs
    delete y_cls
}

//! the q8 GEMV with the residual step as its last workgroup's epilogue, against the GEMV in the scalar block order and
//! `ArRq`'s math on the CPU: the residual row updated in place, the normed (or raw) row's Q8_0 blocks; the counter
//! reads zero after the dispatch; a poisoned expectation must red
//! `abias`: the GEMV's row takes a bias row out of the norms plane (its third row here) before the add - gpt-oss's attention output bias
//! the GEMV-epilogue cells' host fixture: the q8 weight plane over `d` rows of `n`, the Q8_0 x row, the residual row,
//! and three rows in the norms plane - the post norm at 0, the next norm at d, the bias row at 2d
struct GemvArFixture {
    wqh : array<uint>
    wsh : array<uint>
    xqh : array<uint>
    xsh : array<float>
    xres : array<float>
    wnh : array<float>
}

def private q8_gemv_ar_fixture(n, d : int) : GemvArFixture {
    let nb = n / 32
    let totblk = d * nb
    var fx <- GemvArFixture(wqh <- hashed_words(totblk * 8, 3u, 29u), wsh <- ws_plane(totblk / 2 + 1, 5u, 1u),
        xqh <- hashed_words(nb * 8, 13u, 7u), xsh <- row_fill(nb, 1, 0, 0.1, -0.2, 7), xres <- row_fill(d, 43, 0, 0.4, 19.0, 97))
    fx.wnh |> resize(3 * d)
    for (i in range(2 * d)) {
        fx.wnh[i] = 0.8 + float(i % 9) * 0.05
    }
    for (i in range(d)) {
        fx.wnh[2 * d + i] = float((i * 31) % 67) * 0.6 - 19.8   //! a bias of the residual row's own magnitude, so a dropped add is loud
    }
    return <- fx
}

//! the q8 GEMV + residual step's ten planes over the fixture, in `set_q8_gemv_ar_cls`'s binding order: the GEMV's y row at 4,
//! the residual row in place at 5, the norm rows at 6, the quants at 7, the scales at 8
def private gemv_ar_planes(var cp; fx : GemvArFixture; d : int) : int[10] {   //! `cp` untyped: the CellPlanes type lives behind the `?vulkan` require, and a typed signature breaks the compile where the module is absent
    let pwq = cp |> in_plain(fx.wqh)
    let pws = cp |> in_plain(fx.wsh)
    let pxq = cp |> in_plain(fx.xqh)
    let pxs = cp |> in_plain(fx.xsh)
    let py = cp |> out_(d)
    let pxr = cp |> io_(fx.xres)
    let pwn = cp |> in_(fx.wnh)
    let poq = cp |> out_words(int64(d), NAN_SENTINEL_BITS)
    let pos = cp |> out_(d / 32)
    let pcnt = cp |> out_words(16l, 0u)
    return fixed_array(pwq, pws, pxq, pxs, py, pxr, pwn, poq, pos, pcnt)
}

def private q8_gemv_ar_run(t; n, d : int; pre_on, raw : bool; ascale : float; abias : bool = false) {
    let nblk_out = d / 32
    var fx <- q8_gemv_ar_fixture(n, d)
    let eps = 1.0e-6
    var xr_cls : array<float>
    var oq : array<int8>
    var os : array<float>
    var cnt_after : array<uint>
    with_cell_planes() $(var cp : CellPlanes) {
        let pl = gemv_ar_planes(cp, fx, d)
        var s_cls = cp |> cls_set(pl, @@set_q8_gemv_ar_cls)
        var pc = GemvArArgs(n = uint(n), d = uint(d), wblk = 0u, xblk = 0u, add_on = 1u, woff = uint(d), pwoff = 0u,
            pre_on = pre_on ? 1u : 0u, no_norm = raw ? 1u : 0u, eps = eps, ascale = ascale, abias_on = abias ? 1u : 0u, aboff = uint(2 * d))
        let rpw = 4l * g_gpu.rows_per_wg
        cp |> cell_record() $(rw : VkCommandBuffer; var h : VkHaz) {
            enc_q8_gemv_ar_cls(rw, h, s_cls, pc, (int64(d) + rpw - 1l) / rpw)
        }
        delete xr_cls
        xr_cls <- cp |> plane_run(pl[5])
        delete oq
        oq <- cp |> plane_as(pl[7], type<int8>, d)
        delete os
        os <- cp |> plane_run(pl[8])
        delete cnt_after
        cnt_after <- cp |> plane_as(pl[9], type<uint>, 4)
    }
    // the oracle: the q8 GEMM oracle over one record with the x row as its one column, then the residual step's math
    let one_row <- [0u, 0u, 1u, 0u]
    var y : array<float>
    y |> resize(d)
    q8_gemm_oracle(fx.wqh, fx.wsh, fx.xqh, fx.xsh, one_row, 1, n, d, y)
    if (abias) {
        for (k in range(d)) {
            y[k] += fx.wnh[2 * d + k]
        }
    }
    var aref := y   //! the add partner the step sees: the normed GEMV row under pre_on, the raw row otherwise
    if (pre_on) {
        rmsnorm(unsafe(addr<float?>(aref[0])), unsafe(addr<float const?>(y[0])), unsafe(addr<float const?>(fx.wnh[0])), int64(d), eps)
    }
    var xr_ref <- [for (k in range(d)); (fx.xres[k] + aref[k]) * ascale]
    var normed := xr_ref
    if (!raw) {
        rmsnorm(unsafe(addr<float?>(normed[0])), unsafe(addr<float const?>(xr_ref[0])), unsafe(addr<float const?>(fx.wnh[d])), int64(d), eps)
    }
    //! the residual bar: the step's own rounding on the row, plus the GEMV's on the partner it adds - a 512-deep dot whose
    //! terms cancel lands a few 1e-4 of its magnitude off the oracle's order (two rows of 2816 read 3e-4 at gemma-4-26B's width)
    var badx = 0
    var pbadx = 0
    for (k in range(d)) {
        let bar = 1e-4 * abs(xr_ref[k]) + 1e-4 + 4e-4 * abs(aref[k]) * ascale
        if (!(abs(xr_cls[k] - xr_ref[k]) <= bar)) {
            badx++
        }
        if (abs(xr_cls[k] - (xr_ref[k] + 0.1 * abs(xr_ref[k]) + 0.1)) > bar) {   //! the control: an expectation pushed a tenth off must red
            pbadx++
        }
    }
    let off = q8_blocks_off(oq, os, normed, nblk_out)
    let tag = "n {n} d {d} pre {pre_on ? 1 : 0} raw {raw ? 1 : 0} bias {abias ? 1 : 0}"
    t |> success(badx == 0, "q8 gemv+ar {tag}: the residual row matches the oracle ({badx} of {d} off)")
    t |> success(pbadx == d, "q8 gemv+ar {tag}: the residual bar reds an oracle row pushed a tenth off ({pbadx} of {d} red)")
    t |> success(off.scales == 0, "q8 gemv+ar {tag}: the block scales match ({off.scales} of {nblk_out} off)")
    t |> success(off.values == 0, "q8 gemv+ar {tag}: the dequantized row sits within a quant step ({off.values} of {d} off)")
    t |> success(cnt_after[0] == 0u, "q8 gemv+ar {tag}: the arrival counter reads zero after the dispatch ({cnt_after[0]})")
    t |> success(off.poison_inside == 0, "q8 gemv+ar {tag}: the bar reds a poisoned oracle element in every block ({off.poison_inside} passed)")
    delete fx
    delete xr_cls
    delete oq
    delete os
    delete cnt_after
    delete y
    delete xr_ref
    delete normed
}

[test]
def test_vkd_q8_gemv_ar(t0 : T?) {
    t0 |> run("q8 GEMV with the residual step as its last workgroup's epilogue == the CPU chain (the sandwich step, the raw-quant step, a plain step)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_gemv_ar_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            q8_gemv_ar_run(t, 512, 1152, true, false, 1.0)    // gemma-3-1b's wo step: the add partner normed first, then the FFN norm's quant
            q8_gemv_ar_run(t, 256, 1536, true, true, 1.0)     // an E-series down step: the row quantized as it is for the PLE gate
            q8_gemv_ar_run(t, 512, 1024, false, false, 0.5)   // a plain step under an output scale
            q8_gemv_ar_run(t, 512, 1024, false, false, 1.0, true)   // gpt-oss's wo step: the output bias row added before the residual
            q8_gemv_ar_run(t, 512, 1152, true, false, 1.0, true)    // a sandwich-normed wo step with an output bias: the bias rides the partner before its norm
            q8_gemv_ar_run(t, 512, 2816, true, false, 1.0)    // gemma-4-26B's wo step: two whole four-column rounds and a three-column tail
            q8_gemv_ar_run(t, 512, 2816, false, false, 1.0)   // the same width on the plain step (the four-column arm's tail)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the epilogue's residual step against the row kernel fed the GEMV's own y row - the same residual row and norms, the
//! updated row, the quants and the scales bit for bit: the N-row command's sites take the row kernel where the one-row
//! command's take the epilogue, and the regions cells hold the two commands bit for bit
def private q8_gemv_ar_twin_run(t; n, d : int; pre_on : bool; ascale : float) {
    let nblk_out = d / 32
    var fx <- q8_gemv_ar_fixture(n, d)
    let eps = 1.0e-6
    var xr_epi_h : array<float>
    var oq_epi_h : array<uint>
    var os_epi_h, xr_row_h : array<float>
    var oq_row_h : array<uint>
    var os_row_h : array<float>
    with_cell_planes() $(var cp : CellPlanes) {
        let pl = gemv_ar_planes(cp, fx, d)
        let pxr_row = cp |> io_(fx.xres)
        let poq_row = cp |> out_words(int64(d), NAN_SENTINEL_BITS)
        let pos_row = cp |> out_(nblk_out)
        var s_epi = cp |> cls_set(pl, @@set_q8_gemv_ar_cls)
        var s_row = cp |> cls_set(fixed_array(pxr_row, pl[4], pl[6], poq_row, pos_row), @@set_cls_ar_rq_b)
        var pc = GemvArArgs(n = uint(n), d = uint(d), wblk = 0u, xblk = 0u, add_on = 1u, woff = uint(d), pwoff = 0u,
            pre_on = pre_on ? 1u : 0u, no_norm = 0u, eps = eps, ascale = ascale, abias_on = 0u, aboff = uint(2 * d))
        let rpw = 4l * g_gpu.rows_per_wg
        var pca = ArArgs(dim = uint(d), add_on = 1u, woff = uint(d), eps = eps, ascale = ascale, pre_on = pre_on ? 1u : 0u, pwoff = 0u)
        cp |> cell_record() $(rw : VkCommandBuffer; var h : VkHaz) {
            enc_q8_gemv_ar_cls(rw, h, s_epi, pc, (int64(d) + rpw - 1l) / rpw)
            enc_cls_ar_rq_b(rw, h, s_row, pca, 1l)
        }
        delete xr_epi_h
        xr_epi_h <- cp |> plane_run(pl[5])
        delete oq_epi_h
        oq_epi_h <- cp |> plane_as(pl[7], type<uint>, d / 4)
        delete os_epi_h
        os_epi_h <- cp |> plane_run(pl[8])
        delete xr_row_h
        xr_row_h <- cp |> plane_run(pxr_row)
        delete oq_row_h
        oq_row_h <- cp |> plane_as(poq_row, type<uint>, d / 4)
        delete os_row_h
        os_row_h <- cp |> plane_run(pos_row)
    }
    let tag = "n {n} d {d} pre {pre_on ? 1 : 0}"
    t |> success(xr_epi_h[0] != fx.xres[0], "q8 gemv+ar twin {tag}: the epilogue wrote the residual row")
    t |> success(xr_row_h[0] != fx.xres[0], "q8 gemv+ar twin {tag}: the row kernel wrote the residual row")
    let badx = mismatch_exact(xr_epi_h, xr_row_h)
    let first = first_mismatch(xr_epi_h, xr_row_h, 0, 0, d)
    if (first >= 0) {
        to_log(LOG_INFO, "q8 gemv+ar twin {tag}: first residual mismatch at {first}: epilogue {xr_epi_h[first]} row kernel {xr_row_h[first]}\n")
    }
    t |> success(badx == 0, "q8 gemv+ar twin {tag}: the epilogue's residual row == the row kernel's ({badx} of {d} off)")
    let bads = mismatch_exact(os_epi_h, os_row_h)
    t |> success(bads == 0, "q8 gemv+ar twin {tag}: the epilogue's block scales == the row kernel's ({bads} of {nblk_out} off)")
    let badq = mismatch_qbytes(oq_epi_h, oq_row_h, 0)
    t |> success(badq == 0, "q8 gemv+ar twin {tag}: the epilogue's quants == the row kernel's ({badq} of {d} off)")
    delete fx
    delete xr_epi_h
    delete xr_row_h
    delete oq_epi_h
    delete oq_row_h
    delete os_epi_h
    delete os_row_h
}

[test]
def test_vkd_q8_gemv_ar_row_twin(t0 : T?) {
    t0 |> run("q8 GEMV epilogue's residual step == the row kernel fed the same y row, bit for bit (the sandwich step and the plain step)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_gemv_ar_cls() || !ensure_cls_ar_rq_b()) {
                t |> skip("no Vulkan device")
                return
            }
            q8_gemv_ar_twin_run(t, 512, 1152, true, 1.0)    // gemma-3-1b's wo step
            q8_gemv_ar_twin_run(t, 2048, 2304, true, 1.0)   // gemma-2-2b's wo step: eight heads of 256 into the 2304 residual
            q8_gemv_ar_twin_run(t, 9216, 2304, true, 1.0)   // gemma-2-2b's down step
            q8_gemv_ar_twin_run(t, 512, 1024, false, 0.5)   // a plain step under an output scale
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! one output plane of an N-form cell: its name in the asserts, the 32-bit words one column holds, whether the two forms'
//! words are the same by claim - a plane whose words differ by design (a feed word carrying its row) is the caller's compare -
//! and the words under its first column's base
struct NformPlane {
    name : string
    words : int
    cmp : bool = true
    head : int
}

//! the output planes of one form as the blocks bind them: plane p's device buffer and its bytes
struct NformOuts {
    bufs : array<uint64>
    bytes : array<int64>
}

//! the two forms' output planes read back: `one[p]` over every column of the extent, column c at c * (head + words) + head;
//! `nf[p]` the last count's, column c at head + c * words (`head` and `words` plane p's)
struct NformPlanes {
    one : array<array<uint>>
    nf : array<array<uint>>
}

//! a read-back plane's words as the floats the kernel stored
def private plane_floats(w : array<uint>) : array<float> => <- [for (x in w); uint_bits_to_float(x)]

//! the words of `ncols` live columns of a plane off between the forms; a live column the N form never wrote - every word still the sentinel - counts whole
def private nform_plane_mismatch(one, nf : array<uint>; ncols, head, words : int) : int {
    var bad = 0
    for (c in range(ncols)) {
        let nbase = head + c * words
        let obase = c * (head + words) + head
        var sentinels = 0
        for (i in range(words)) {
            if (nf[nbase + i] == NAN_SENTINEL_BITS) {
                sentinels++
            }
        }
        if (sentinels == words) {
            bad += words
            continue
        }
        for (i in range(words)) {
            if (nf[nbase + i] != one[obase + i]) {
                bad++
            }
        }
    }
    return bad
}

//! the asserts behind `nform_cols_cell` at one count: every compared plane's live columns word for word, the words under each
//! plane's head and the slots past the last live column still the sentinel, and one poisoned word of the first compared plane reddening its compare
def private nform_cols_compare(t : T?; tag : string; ncols, ncol_max : int; planes : array<NformPlane>; var r : NformPlanes) {
    var stale = 0
    var poisoned = false
    for (p in range(length(planes))) {
        let pl = planes[p]
        if (pl.cmp) {
            let bad = nform_plane_mismatch(r.one[p], r.nf[p], ncols, pl.head, pl.words)
            t |> success(bad == 0, "{tag}: {pl.name} over the live columns are the one form's word for word ({bad} of {ncols * pl.words} words off)")
            if (!poisoned) {
                poisoned = true
                let at = pl.head + (ncols - 1) * pl.words + pl.words / 2
                let kept = r.nf[p][at]
                r.nf[p][at] = kept ^ 0x55u
                t |> equal(nform_plane_mismatch(r.one[p], r.nf[p], ncols, pl.head, pl.words), 1, "{tag}: the word-exact bar on {pl.name} reds one poisoned word")
                r.nf[p][at] = kept
            }
        }
        for (i in range(pl.head)) {
            if (r.nf[p][i] != NAN_SENTINEL_BITS) {
                stale++
            }
        }
        for (i in range(pl.head + ncols * pl.words, pl.head + (ncol_max + 1) * pl.words)) {
            if (r.nf[p][i] != NAN_SENTINEL_BITS) {
                stale++
            }
        }
    }
    t |> success(stale == 0, "{tag}: the words under the base and the slots past the last live column keep the sentinel ({stale} written)")
}

//! one N-form-vs-one-form cell: `one_col(c, raw, h, outs)` records the one form for column `c` of the extent `ncol_max`,
//! `n_form(ncols, raw, h, outs)` the N form at each count of `counts`; the output planes are made here - one slot past the
//! extent, plane p's one form the cell's plane 2p and its N form 2p + 1 - NaN-filled before every run, copied back under the
//! hazard mask `bits`, compared by `nform_cols_compare` and returned
def private nform_cols_cell(t : T?; tag : string; counts : array<int>; ncol_max : int; planes : array<NformPlane>; bits : uint; one_col; n_form) : NformPlanes {
    var res : NformPlanes
    static_if (typeinfo builtin_module_exists(vulkan)) {
        with_cell_planes() $(var cp : CellPlanes) {
            let np = length(planes)
            var one_outs, n_outs : NformOuts
            for (pl in planes) {
                let po = cp |> out_words(int64(pl.head + pl.words) * 4l, NAN_SENTINEL_BITS, bits)
                let pn = cp |> out_words(int64(pl.head + pl.words * (ncol_max + 1)) * 4l, NAN_SENTINEL_BITS, bits)
                one_outs.bufs |> push(cp.dev[po])
                one_outs.bytes |> push(cp.bytes[po])
                n_outs.bufs |> push(cp.dev[pn])
                n_outs.bytes |> push(cp.bytes[pn])
            }
            res.one |> resize(np)
            res.nf |> resize(np)
            for (c in range(ncol_max)) {
                for (p in range(np)) {
                    cp |> plane_refill(2 * p)
                }
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    invoke(one_col, c, raw, h, one_outs)
                }
                for (p in range(np)) {
                    res.one[p] |> push_from(plane_as(cp, 2 * p, type<uint>, int(cp.bytes[2 * p] / 4l)))
                }
            }
            for (ncols in counts) {
                for (p in range(np)) {
                    cp |> plane_refill(2 * p + 1)
                }
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    invoke(n_form, ncols, raw, h, n_outs)
                }
                for (p in range(np)) {
                    delete res.nf[p]
                    res.nf[p] <- plane_as(cp, 2 * p + 1, type<uint>, int(cp.bytes[2 * p + 1] / 4l))
                }
                nform_cols_compare(t, "{tag} at {ncols} column(s)", ncols, ncol_max, planes, res)
            }
            delete one_outs
            delete n_outs
        }
    }
    return <- res
}

//! the one-row columns against the CPU chain: per column the q8 GEMM oracle over one record with the x row as its one column,
//! per plane, then the activation - its block scales within 2e-3, the dequantized hidden row within a quant step
def private q8_gemv_gu_oracle_cols(t; tag : string; oq_w, os_w : array<uint>; gqh, gsh, uqh, ush, xqh : array<uint>; xsh : array<float>; n, hid, ncols : int; act : uint) {
    let nb = n / 32
    let nblk_out = hid / 32
    let one_rec <- [0u, 0u, 1u, 0u]
    var obad = 0
    var obads = 0
    var opoison = 0
    for (c in range(ncols)) {
        var xq_c : array<uint>
        var xs_c : array<float>
        xq_c |> resize(nb * 8)
        xs_c |> resize(nb)
        for (i in range(nb * 8)) {
            xq_c[i] = xqh[c * nb * 8 + i]
        }
        for (i in range(nb)) {
            xs_c[i] = xsh[c * nb + i]
        }
        var g_ref, u_ref : array<float>
        g_ref |> resize(hid)
        u_ref |> resize(hid)
        q8_gemm_oracle(gqh, gsh, xq_c, xs_c, one_rec, 1, n, hid, g_ref)
        q8_gemm_oracle(uqh, ush, xq_c, xs_c, one_rec, 1, n, hid, u_ref)
        var act_c <- [for (e in range(hid)); act_mul_ref(g_ref[e], u_ref[e], act)]
        var oq_c <- [for (e in range(hid)); int8(uint8((oq_w[c * (hid / 4) + e / 4] >> uint((e % 4) * 8)) & 0xFFu))]
        var os_c <- [for (b in range(nblk_out)); uint_bits_to_float(os_w[c * nblk_out + b])]
        let off = q8_blocks_off(oq_c, os_c, act_c, nblk_out)
        obads += off.scales
        obad += off.values
        opoison += off.poison_inside
        delete act_c
        delete oq_c
        delete os_c
        delete g_ref
        delete u_ref
        delete xq_c
        delete xs_c
    }
    t |> success(obads == 0, "one-row fused gate-up {tag}: every column's block scales match the CPU oracle ({obads} of {nblk_out * ncols} off)")
    t |> success(obad == 0, "one-row fused gate-up {tag}: every column's dequantized hidden row sits within a quant step of the CPU oracle ({obad} of {hid * ncols} off)")
    t |> success(opoison == 0, "one-row fused gate-up {tag}: the bar reds a poisoned oracle element in every block ({opoison} passed)")
}

//! the fused gate-up GEMV's N-column form over `ncols` columns against the one-row kernel run a column at a time, word for
//! word through `nform_cols_cell`; the one-row columns over the extent against the CPU chain
def private q8_gemv_gu_n_run(t; n, hid, ncols : int; act : uint) {
    let nb = n / 32
    let nblk_out = hid / 32
    let totblk = hid * nb
    let wq_bytes = int64(totblk) * 32l
    let ws_bytes = int64(totblk) * 2l
    let xq_bytes = int64(nb) * 32l
    let xs_bytes = int64(nb) * 4l
    let ncol_max = 8
    var gqh, gsh, uqh, ush : array<uint>
    q8_plane(totblk, 1u, 71u, 1u, 3u, gqh, gsh)
    q8_plane(totblk, 3u, 5u, 7u, 11u, uqh, ush)
    var xqh <- hashed_words(nb * 8 * ncol_max, 17u, 9u)
    var xsh <- row_fill(nb * ncol_max, 1, 0, 0.125, -0.25, 5)
    let gqd = make_device_buf(wq_bytes)
    let gsd = make_device_buf(ws_bytes)
    let uqd = make_device_buf(wq_bytes)
    let usd = make_device_buf(ws_bytes)
    let xqd = make_device_buf(xq_bytes)
    let xsd = make_device_buf(xs_bytes)
    let xqd_n = make_device_buf(xq_bytes * int64(ncols))
    let xsd_n = make_device_buf(xs_bytes * int64(ncols))
    let gbits = fixed_array(1u, 2u, 4u, 8u, 16u, 32u, 64u, 64u)
    let shape = "n {n} hid {hid} act {act}"
    unsafe {
        upload_region_at(gqd, 0l, addr<uint8 const?>(gqh[0]), wq_bytes)
        upload_region_at(gsd, 0l, addr<uint8 const?>(gsh[0]), ws_bytes)
        upload_region_at(uqd, 0l, addr<uint8 const?>(uqh[0]), wq_bytes)
        upload_region_at(usd, 0l, addr<uint8 const?>(ush[0]), ws_bytes)
        upload_region_at(xqd_n, 0l, addr<uint8 const?>(xqh[0]), xq_bytes * int64(ncols))
        upload_region_at(xsd_n, 0l, addr<uint8 const?>(xsh[0]), xs_bytes * int64(ncols))
    }
    var planes <- nform_cols_cell(t, "N-row fused gate-up {shape}", [ncols], ncol_max,
        [NformPlane(name = "the quant bytes", words = hid / 4), NformPlane(name = "the block scales", words = nblk_out)], 64u,
        $(c : int; raw : VkCommandBuffer; var h : VkHaz; o : NformOuts) {
            unsafe {
                upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[c * nb * 8]), xq_bytes)
                upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[c * nb]), xs_bytes)
            }
            var s_one = set_q8_gemv_gu_n1_cls(fixed_array(gqd, gsd, uqd, usd, xqd, xsd, o.bufs[0], o.bufs[1]),
                fixed_array(wq_bytes, ws_bytes, wq_bytes, ws_bytes, xq_bytes, xs_bytes, o.bytes[0], o.bytes[1]), gbits)
            var pc1 = GuNArgs(n = uint(n), d = uint(hid), gblk = 0u, ublk = 0u, act = act, ncols = 1u)
            enc_q8_gemv_gu_n1_cls(raw, h, s_one, pc1, int64(nblk_out))
        },
        $(nc : int; raw : VkCommandBuffer; var h : VkHaz; o : NformOuts) {
            var s_n = q8_gemv_gu_n_set(fixed_array(gqd, gsd, uqd, usd, xqd_n, xsd_n, o.bufs[0], o.bufs[1]),
                fixed_array(wq_bytes, ws_bytes, wq_bytes, ws_bytes, xq_bytes * int64(nc), xs_bytes * int64(nc), o.bytes[0], o.bytes[1]), gbits)
            var pc = GuNArgs(n = uint(n), d = uint(hid), gblk = 0u, ublk = 0u, act = act, ncols = uint(nc))
            q8_gemv_gu_n_enc(raw, h, s_n, pc, int64(nblk_out))
        })
    q8_gemv_gu_oracle_cols(t, shape, planes.one[0], planes.one[1], gqh, gsh, uqh, ush, xqh, xsh, n, hid, ncol_max, act)
    delete planes
    delete gqh
    delete gsh
    delete uqh
    delete ush
    delete xqh
    delete xsh
}

[test]
def test_vkd_q8_gemv_gu_n(t0 : T?) {
    t0 |> run("fused q8 gate-up GEMV's N-column stamps == the one-row kernel a column at a time, bit for bit; the one-row rows == the CPU chain") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_q8_gemv_gu_n1_cls() && q8_gemv_gu_n_ensure(8l))) {
                t |> skip("no Vulkan device")
                return
            }
            q8_gemv_gu_n_run(t, 512, 1152, 4, ACT_SILU)   //! the four-column stamp full
            q8_gemv_gu_n_run(t, 256, 512, 3, ACT_GELU)   //! an odd count on the four-column stamp: its last lane re-dots column 2, a subgroup idles in the quantize
            q8_gemv_gu_n_run(t, 512, 1024, 8, ACT_SILU)   //! the eight-column stamp full: every subgroup a column at 32 lanes, two rounds at 64
            q8_gemv_gu_n_run(t, 256, 512, 5, ACT_SILU)   //! the eight-column stamp at five: three lanes re-dot column 4
            q8_gemv_gu_n_run(t, 256, 512, 2, ACT_GELU)   //! the two-column stamp full
            q8_gemv_gu_n_run(t, 256, 512, 1, ACT_SILU)   //! one column on the two-column stamp: the one-row command's shape through the N form
            q8_gemv_gu_n_run(t, 256, 512, 2, ACT_OAI)   //! the clamped swiglu through the N form
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the N-column form against the one-row dispatches, one a column, word for word through `nform_cols_cell`: the same kernel
//! math in the same order; the one-row form's CPU chain is `q8_gemv_pleact_run`'s at the same widths
def private q8_gemv_pleact_cols_run(t; n, d, ncols : int) {
    let nb = n / 32
    let totblk = d * nb
    let wq_bytes = int64(totblk) * 32l
    let ws_bytes = int64(totblk) * 2l
    let all = 3 * n   // three layers' side rows a column: the cell reads the middle one
    let ncol_max = 8
    var wqh, wsh : array<uint>
    q8_plane(totblk, 5u, 19u, 3u, 7u, wqh, wsh)
    var gh <- row_fill(ncol_max * n, 37, 0, 0.06, 3.0, 101)
    var sh <- row_fill(ncol_max * all, 29, 0, 0.05, 2.0, 83)
    let g_bytes = int64(ncols * n) * 4l
    let s_bytes = int64(ncols * all) * 4l
    let g1_bytes = int64(n) * 4l
    let s1_bytes = int64(all) * 4l
    let wqd = make_device_buf(wq_bytes)
    let wsd = make_device_buf(ws_bytes)
    let gd = make_device_buf(g_bytes)
    let sd = make_device_buf(s_bytes)
    let gd1 = make_device_buf(g1_bytes)
    let sd1 = make_device_buf(s1_bytes)
    let rpw = 4l * g_gpu.rows_per_wg
    let groups = (int64(d) + rpw - 1l) / rpw
    let gbits = fixed_array(1u, 2u, 4u, 8u, 16u)
    unsafe {
        upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
        upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
        upload_region_at(gd, 0l, addr<uint8 const?>(gh[0]), g_bytes)
        upload_region_at(sd, 0l, addr<uint8 const?>(sh[0]), s_bytes)
    }
    var planes <- nform_cols_cell(t, "q8 pleact n {n} d {d}", [ncols], ncol_max, [NformPlane(name = "the proj rows", words = d)], 16u,
        $(c : int; raw : VkCommandBuffer; var h : VkHaz; o : NformOuts) {
            unsafe {
                upload_region_at(gd1, 0l, addr<uint8 const?>(gh[c * n]), g1_bytes)
                upload_region_at(sd1, 0l, addr<uint8 const?>(sh[c * all]), s1_bytes)
            }
            var s1 = set_q8_gemv_pleact_cls(fixed_array(wqd, wsd, gd1, sd1, o.bufs[0]), fixed_array(wq_bytes, ws_bytes, g1_bytes, s1_bytes, o.bytes[0]), gbits)
            var pc1 = PleActArgs(n = uint(n), d = uint(d), wblk = 0u, ubase = uint(n), act = ACT_GELU)
            enc_q8_gemv_pleact_cls(raw, h, s1, pc1, groups)
        },
        $(nc : int; raw : VkCommandBuffer; var h : VkHaz; o : NformOuts) {
            var s_cls = set_q8_gemv_pleact_cls(fixed_array(wqd, wsd, gd, sd, o.bufs[0]), fixed_array(wq_bytes, ws_bytes, g_bytes, s_bytes, o.bytes[0]), gbits)
            var pc = PleActArgs(n = uint(n), d = uint(d), wblk = 0u, ubase = uint(n), act = ACT_GELU, ncols = uint(nc), sstride = uint(all))
            enc_q8_gemv_pleact_cls(raw, h, s_cls, pc, groups)
        })
    delete planes
    delete wqh
    delete wsh
    delete gh
    delete sh
}

[test]
def test_vkd_q8_gemv_pleact(t0 : T?) {
    t0 |> run("fused per-layer-embedding act + Q8_0 requant + q8 proj GEMV == the CPU chain (256 and 128 wide)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_gemv_pleact_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            q8_gemv_pleact_run(t, 256, 200)   // the E-series side row: eight blocks, a partial last workgroup
            q8_gemv_pleact_run(t, 128, 64)    // a narrower row: the idle upper lanes of the build
            q8_gemv_pleact_cols_run(t, 256, 200, 4)   // the N-row command's columns: four rows, the E-series width
            q8_gemv_pleact_cols_run(t, 128, 64, 3)    // an odd column count on the narrower row
            q8_gemv_pleact_cols_run(t, 256, 200, 8)   // the per-row planes' extent: every column's words live
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_q8_gemv_pair(t0 : T?) {
    t0 |> run("q8 GEMV class kernel == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_gemv_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let nb = 16      // 32-blocks per row reduction (n = 512)
            let d = 3
            let nreg = 2
            let rows = nreg * d
            let totblk = rows * nb
            {
                var wsh <- [for (i in range(totblk / 2)); ws_word(uint(i))]   //! two f16 scales a word: the device plane's totblk * 2 bytes
                var f <- gemv_fix(nreg, d, nb, 8, 55u, wsh, nb, 8, 17u, 9u, 0.25, 0.125, 5)
                let wgs = (int64(rows) + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg
                // the q8 GEMM oracle over one record of one activation row: the regions' weight rows lie end to end
                var y_ref : array<float>
                y_ref |> resize(rows)
                var rec <- [0u, 0u, 1u, 0u]
                q8_gemm_oracle(f.wqh, f.wsh, f.xqh, f.xsh, rec, 1, nb * 32, rows, y_ref)
                delete rec
                with_cell_planes() $(var cp : CellPlanes) {
                    gemv_cell_planes(cp, f, rows)   //! a row the GEMV never stores keeps the sentinel
                    var s_cls = cp |> cls_set(GEMV_PLANES, @@set_q8_gemv_cls)
                    var pc = GemvArgs(n = 512u, d = uint(d), nreg = uint(nreg), ybase = 0u)
                    cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                        enc_q8_gemv_cls(raw2, h2, s_cls, pc, wgs)
                    }
                    var y_cls <- plane_run(cp, 5)
                    let bad = mismatch_approx(y_cls, y_ref)
                    t |> success(bad == 0, "q8 GEMV matches the CPU oracle ({bad} of {rows} rows off)")
                    var y_poison := y_cls   // the bar's control
                    y_poison[0] += 1.0 + 2.0 * logits_maxabs(y_ref)
                    t |> success(mismatch_approx(y_poison, y_ref) > 0, "the q8 GEMV bar reds a poisoned element")
                    delete y_poison
                    delete y_cls
                }
                delete y_ref
                delete f
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! one format's N-column decode GEMV against its one-column class over a synthetic [n x d] plane and eight activation
//! rows of distinct content: the one-column class once per row (its region naming that row's activation blocks, its
//! `ybase` that row's y), held to the CPU oracle; then the N class at 1, 2, 4 and 8 columns (`ystride` = d), every
//! column bit for bit against the one-column row - the N form walks a lane's blocks in the one-column's order and folds
//! the lanes the same way (`gemv_shell_n` against `gemv_shell`, `Q8GemvNT` against `Q8Gemv`), so the sums land in one
//! order - the sentinel past the columns it wrote untouched; the q8 plane runs again with the row-pair stamps engaged
//! (`g_q8_n2_min_n` at 0 over the even d, restored after); a poisoned element per bar
def private gemv_ncol_arm(t : T?; f : KqFmt; n, d : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let fmt = int(f)
        let name = "{f}"
        let q8 = f == KqFmt.q8
        let xblk = q8 ? n / 32 : n / 256
        let xq_col = q8 ? xblk * 8 : xblk * 64
        let ncol_max = 8
        var wqh, wsh : array<uint>
        if (q8) {
            q8_plane(d * xblk, 1u, uint(fmt) * 77777u, 1u, uint(fmt) * 13u, wqh, wsh)
        } else {
            wqh |> push_from(hashed_words(d * xblk * int(kq_qsb(f) / 4l), 1u, uint(fmt) * 77777u))
            wsh |> push_from(ws_plane(d * xblk * int(kq_dev_ssb(f) / 4l), 1u, uint(fmt) * 13u))
        }
        var xqh <- hashed_words(xq_col * ncol_max, 31u, 5u)
        var xsh <- row_fill(xblk * ncol_max, 1, 0, 0.25, -0.5, 7)
        let wq_bytes = long_length(wqh) * 4l
        let ws_bytes = long_length(wsh) * 4l
        let xq_bytes = int64(xq_col * ncol_max) * 4l
        let xs_bytes = int64(xblk * ncol_max) * 4l
        let wqd = make_device_buf(wq_bytes)
        let wsd = make_device_buf(ws_bytes)
        let xqd = make_device_buf(xq_bytes)
        let xsd = make_device_buf(xs_bytes)
        let rgd = make_device_buf(8l)   //! the region pair, uploaded before each record: the one form's x column, the N form's row 0
        let gbits = fixed_array(1u, 2u, 4u, 8u, 16u, 32u)
        unsafe {
            upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
            upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
            upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
            upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
        }
        var y_one : array<uint>
        let n2_min_prior = g_q8_n2_min_n
        for (paired in fixed_array(false, true)) {
            continue if (paired && !q8)
            g_q8_n2_min_n = paired ? 0u : 0xFFFFFFFFu
            let form = paired ? "{name} row-pair" : name
            t |> success(gemv_cls_ensure_n(fmt), "{form}: the N-column class rail engages")
            var planes <- nform_cols_cell(t, form, [1, 2, 3, 4, 5, 6, 7, 8], ncol_max, [NformPlane(name = "the y rows", words = d)], 32u,   //! every count: the odd ones leave dead columns on the wider stamps
                $(c : int; raw : VkCommandBuffer; var h : VkHaz; o : NformOuts) {
                    var pair <- [0u, uint(c * xblk)]
                    upload_region_at(rgd, 0l, unsafe(addr<uint8 const?>(pair[0])), 8l)
                    var s1 = gemv_cls_set(fmt, fixed_array(wqd, wsd, rgd, xqd, xsd, o.bufs[0]), fixed_array(wq_bytes, ws_bytes, 8l, xq_bytes, xs_bytes, o.bytes[0]), gbits)
                    var pc1 = GemvArgs(n = uint(n), d = uint(d), nreg = 1u, ybase = 0u)
                    gemv_enc(fmt, raw, h, s1, pc1, int64(d))
                    delete pair
                },
                $(ncols : int; raw : VkCommandBuffer; var h : VkHaz; o : NformOuts) {
                    var pair <- [0u, 0u]
                    upload_region_at(rgd, 0l, unsafe(addr<uint8 const?>(pair[0])), 8l)
                    var s_n = gemv_cls_set_n(fmt, fixed_array(wqd, wsd, rgd, xqd, xsd, o.bufs[0]), fixed_array(wq_bytes, ws_bytes, 8l, xq_bytes, xs_bytes, o.bytes[0]), gbits)
                    var pc = GemvArgs(n = uint(n), d = uint(d), nreg = 1u, ybase = 0u, ncols = uint(ncols), ystride = uint(d),
                        lanes_per_row = gemv_lanes_per_row(fmt, int64(n)))
                    gemv_cls_enc_n(fmt, raw, h, s_n, pc, gemv_wgs(int64(d), pc.lanes_per_row))   //! the class entry: the one-column count still lands on the narrowest N stamp
                    delete pair
                })
            delete y_one
            y_one <- planes.one[0]
            delete planes
        }
        g_q8_n2_min_n = n2_min_prior
        var y_ref <- gemv_ncol_oracle(f, wqh, wsh, xqh, xsh, n, d, ncol_max)
        var y_onef <- plane_floats(y_one)
        let bad_o = mismatch_approx(y_onef, y_ref)
        t |> success(bad_o == 0, "{name}: the one-column class over every activation row matches the CPU oracle ({bad_o} of {d * ncol_max} off)")
        y_ref[d + 2] += 1.0 + abs(y_ref[d + 2])   //! the poison: one past the element's own magnitude (an int8 dot lands in the hundreds of thousands)
        t |> success(mismatch_approx(y_onef, y_ref) == 1, "{name}: the oracle bar reds one poisoned element")
        delete y_ref
        delete y_onef
        delete y_one
        delete wqh
        delete wsh
        delete xqh
        delete xsh
    }
}

[test]
def test_vkd_gemv_ncol(t0 : T?) {
    t0 |> run("the N-column decode GEMV leaves (q8 and its row-pair stamps, every kq format) == the one-column class, column for column") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_gemv_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let fmts <- kq_gemv_fmts()
            t |> equal(length(fmts), 1 + length(KQ_LEAF_FMTS), "the family's roster is q8 and the fourteen kq leaves")
            for (f in fmts) {
                t |> success(gemv_cls_ensure(int(f)), "{f}: the one-column class rail engages")
                gemv_ncol_arm(t, f, 1024, 96)   //! n a multiple of 256 for the superblock formats, d even for the row-pair stamps
                gemv_ncol_arm(t, f, 4096, 90)   //! a wider reduction at a row count off the lane splits' multiples
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_cvt_family(t0 : T?) {
    t0 |> run("dn requant + cvt/act-f16 class kernels == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_cls_dn_rq()) {
                t |> skip("no Vulkan device")
                return
            }
            // dn_requant: src floats at an offset -> Q8_0 words + scales
            {
                let nblk = 24
                let inbase = 64
                var srch <- row_fill(inbase + nblk * 32, 37, 0, 0.04, 2.2, 113)
                let wgs = (nblk * 8 + 255) / 256
                var q_cls : array<uint>
                var s_cls : array<float>
                with_cell_planes() $(var cp : CellPlanes) {
                    let psrc = cp |> in_(srch)
                    let pq = cp |> out_words(int64(nblk) * 32l, I8_SENTINEL_BITS)
                    let ps = cp |> out_(nblk)
                    var sc = cp |> cls_set(fixed_array(psrc, pq, ps), @@set_rq_cls)
                    var pc = RqArgs(inbase = uint(inbase), nblk = uint(nblk))
                    cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                        enc_cls_dn_rq(raw2, h2, sc, pc, int64(wgs))
                    }
                    delete q_cls
                    delete s_cls
                    q_cls <- cp |> plane_as(pq, type<uint>, nblk * 8)
                    s_cls <- cp |> plane_run(ps)
                }
                // plain Q8_0 requant with an input offset — exact
                var q_ref : array<uint>
                var s_ref : array<float>
                requant_oracle(srch, inbase, nblk * 32, false, q_ref, s_ref)
                let bad = mismatch_qbytes(q_cls, q_ref, 0) + mismatch_exact(s_cls, s_ref)
                t |> success(bad == 0, "dn_requant matches the CPU oracle bit-exact ({bad} off)")
                delete srch
                delete q_ref
                delete s_ref
                delete q_cls
                delete s_cls
            }
            // the f16 converters: act-f16, f16 convert (mode-free classes)
            {
                verify(ensure_actf16_cls() && ensure_tower_clamp_cvt_cls(),
                    "the cvt class rails must engage on a live device")
                let nelem = 2048
                var gh <- row_fill(nelem, 29, 0, 0.06, 1.9, 61)
                var uh <- row_fill(nelem, 43, 0, 0.05, 2.1, 83)
                for (arm in range(1, 4)) {   // 1 = act-f16 (gelu), 2 = f16 convert, 3 = act-f16 under the clamped swiglu
                    var h_cls : array<uint>
                    with_cell_planes() $(var cp : CellPlanes) {
                        let pg = cp |> in_(gh)
                        let pu = cp |> in_(uh)
                        let ph = cp |> out_words(int64(nelem) * 2l, NAN_SENTINEL_BITS)
                        let wgs = (nelem / 4 + 255) / 256
                        cp |> cell_record() $(raw2 : VkCommandBuffer; var h2 : VkHaz) {
                            if (arm != 2) {
                                var sc = cp |> cls_set(fixed_array(pg, pu, ph), @@set_actf16_cls)
                                var pc = ActArgs(nelem = uint(nelem), act = arm == 3 ? ACT_OAI : ACT_GELU, nblk = 0u)
                                enc_actf16_cls(raw2, h2, sc, pc, int64(wgs))
                            } else {
                                var sc = cp |> cls_set(fixed_array(pu, ph), @@set_tower_clamp_cvt_cls)
                                var pc = f16_rows_args(uint(nelem))
                                enc_tower_clamp_cvt_cls(raw2, h2, sc, pc, int64(wgs))
                            }
                        }
                        delete h_cls
                        h_cls <- cp |> plane_as(ph, type<uint>, nelem / 2)
                    }
                    // oracle in f32, compared through the f16 halves the kernels emit (the gelu
                    // arm carries transcendental ulps — approx absorbs both)
                    var got, want : array<float>
                    got |> resize(nelem)
                    want |> resize(nelem)
                    for (i in range(nelem)) {
                        got[i] = half_at(h_cls, i)
                        want[i] = arm == 2 ? uh[i] : act_mul_ref(gh[i], uh[i], arm == 3 ? ACT_OAI : ACT_GELU)
                    }
                    check_rows(t, "cvt arm {arm} against the CPU oracle", got, want, BAR_APPROX_CTRL, FLAT_ROWS)
                    delete got
                    delete want
                    delete h_cls
                }
                delete gh
                delete uh
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! `bpad` slots of `pad` then bias rows to `bpad + n` as `row_fill` lays them over the whole index: a KV writer's bias plane
def private bias_past_pad(bpad, n, mul : int; scale, off : float; modulus : int; pad : float) : array<float> {
    var bh <- row_fill(bpad + n, mul, 0, scale, off, modulus)
    for (i in range(bpad)) {
        bh[i] = pad
    }
    return <- bh
}

//! the elements of the K and V mirror rows [w0, w0 + n) of `m` (the V mirror `vbase` past the K one) off the oracle's at `approx`
//! a mirror compare's count off the approx bar and its largest difference, at that mirror element
struct MirrorOff {
    bad : int
    worst : float
    worst_at : int = -1
}

def private mirror_elem(var off : MirrorOff; got, want : float; at : int) {
    if (!approx(got, want)) {
        off.bad++
    }
    let d = abs(got - want)
    if (d > off.worst) {
        off.worst = d
        off.worst_at = at
    }
}

//! the K rows `mk` and V rows `mv` of `n` elements from `w0` against the mirror plane `m`, V at `vbase`
def private mirror_rows_off(m, mk, mv : array<float>; w0, n, vbase : int) : MirrorOff {
    var off = MirrorOff()
    for (i in range(n)) {
        off |> mirror_elem(m[w0 + i], mk[w0 + i], w0 + i)
        off |> mirror_elem(m[vbase + w0 + i], mv[w0 + i], vbase + w0 + i)
    }
    return off
}

//! `mirror_rows_off` asserted 0 under `tag`, and its control: the oracle's first K element moved past its own magnitude must red it
def private mirror_rows_check(t : T?; tag : string; m, mk, mv : array<float>; w0, n, vbase : int) {
    let off = mirror_rows_off(m, mk, mv, w0, n, vbase)
    let verdict = "the K and V mirror rows match the CPU oracle ({off.bad} off; the largest difference {off.worst} at mirror element {off.worst_at})"
    to_log(LOG_INFO, "{tag}: {verdict}\n")
    t |> success(off.bad == 0, "{tag}: {verdict}")
    var poisoned := mk
    poisoned[w0] += 1.0 + abs(poisoned[w0])
    t |> success(mirror_rows_off(m, poisoned, mv, w0, n, vbase).bad > 0, "{tag}: the bar reds a poisoned K mirror element")
    delete poisoned
}

//! a KV writer cell's mirror plane of `n` elements - halves under `f16` - under the NaN sentinel
def private mirror_plane(var cp; n : int; f16 : bool) : int => f16 ? out_words(cp, int64(n) * 2l, NAN_SENTINEL_BITS) : out_(cp, n)

//! the K mirror plane `pk` then the V one `pv`, `n` elements each, as floats: `mirror_rows_off`'s operand
def private mirror_pair(cp; pk, pv, n : int; f16 : bool) : array<float> {
    var m <- f16 ? unpack_f16(plane_as(cp, pk, type<float16>, n)) : plane_as(cp, pk, type<float>, n)
    m |> push_from(f16 ? unpack_f16(plane_as(cp, pv, type<float16>, n)) : plane_as(cp, pv, type<float>, n))
    return <- m
}

//! the batched KV writer's oracle over `npos` positions of the four-head, two-kv-head, 64-wide fixture: the bias row at
//! `bpad` first, then each position's NEOX pairs off its cos row; the roped q rows, and k roped + v raw (plus bias) at the
//! absolute mirror rows from `pos0` past `layerbase`, on the half grid under `f16`
def private rope_kv_b_ref(q0h, kvh, bh, csh : array<float>; npos, pos0, layerbase, bpad : int; f16 : bool) : tuple<q : array<float>; mk : array<float>; mv : array<float>> {
    let nh = 4
    let nkvh = 2
    let hs = 64
    let qd = nh * hs
    let kvd = nkvh * hs
    let mirn = layerbase + (pos0 + npos) * kvd
    var q_ref, mk_ref, mv_ref : array<float>
    q_ref |> resize(npos * qd)
    mk_ref |> resize(mirn)
    mv_ref |> resize(mirn)
    for (p in range(npos)) {
        var ref <- rope_kv_rows(q0h, kvh, bh, csh, p * qd, p * kvd, npos * kvd + p * kvd, p * hs, bpad, nh, nkvh, hs) $(x : float) => f16 ? f16r(x) : x
        let mo = layerbase + (pos0 + p) * kvd
        for (i in range(qd)) {
            q_ref[p * qd + i] = ref.q[i]
        }
        for (i in range(kvd)) {
            mk_ref[mo + i] = ref.k[i]
            mv_ref[mo + i] = ref.v[i]
        }
        delete ref
    }
    return <- (q = q_ref, mk = mk_ref, mv = mv_ref)
}

//! the batched rope + K/V store (`rope_kv_b_cls`, or under `f16` its half-mirror twin) over five positions into the mirrors
//! at absolute rows against the CPU oracle - the split k / v projection rows, then the merged [k | v] rows over a mirror
//! sentinel - with the q/k/v bias rows past a pad under `biased`; the f32 stamp's in-place q rows compare too
def private rope_kv_b_arm(t : T?; f16, biased : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let nh = 4
        let nkvh = 2
        let hs = 64
        let half = hs / 2
        let qd = nh * hs
        let kvd = nkvh * hs
        let npos = 5
        let pos0 = 7
        let layerbase = 256
        let voff = npos * kvd   // v rows base inside the kv projection buffer
        let ppp = (nh + nkvh) * half
        let kpair0 = nh * half
        let mirn = layerbase + (pos0 + npos) * kvd
        let name = f16 ? "rope_kv_b_f16{biased ? " with the q/k/v bias" : ""}" : "rope_kv_store_b"
        var q0h <- row_fill(npos * qd, 31, 0, 0.05, 1.6, 67)
        var kvh <- row_fill(voff + npos * kvd, 23, 0, 0.04, 1.5, 71)
        var csh <- row_fill(npos * hs, 13, 0, 0.045, 0.9, 41)
        let bpad = biased ? 96 : 0   // the bias rows sit past a pad, so boff is exercised off its default
        var bh <- biased ? bias_past_pad(bpad, qd + 2 * kvd, 19, 0.02, 0.3, 31, 77.0) : array<float>()
        let wgs = (npos * ppp + 255) / 256
        var oref <- rope_kv_b_ref(q0h, kvh, bh, csh, npos, pos0, layerbase, bpad, f16)
        let q_ref & = oref.q
        let mk_ref & = oref.mk
        let mv_ref & = oref.mv
        // the merged k|v projection layout (the DASLLAMA_VK_KV_MERGE default): [k | v] per position at stride 2*kvd, v at
        // +kvd - the same oracle rows must land
        var kvm_h : array<float>
        kvm_h |> resize(npos * 2 * kvd)
        for (p in range(npos)) {
            for (c in range(kvd)) {
                kvm_h[p * 2 * kvd + c] = kvh[p * kvd + c]
                kvm_h[p * 2 * kvd + kvd + c] = kvh[voff + p * kvd + c]
            }
        }
        let w0 = layerbase + pos0 * kvd
        for (merged in fixed_array(false, true)) {
            with_cell_planes() $(var cp : CellPlanes) {
                let pq = cp |> io_(q0h)
                let pkv = merged ? in_(cp, kvm_h) : in_(cp, kvh)
                let pkm = mirror_plane(cp, mirn, f16)
                let pvm = mirror_plane(cp, mirn, f16)
                let pcs = cp |> in_(csh)
                let pb = biased ? in_(cp, bh) : pcs
                let planes = fixed_array(pq, pkv, pkm, pvm, pcs, pb)
                var sc = f16 ? cls_set(cp, planes, @@set_rope_kv_b_f16_cls) : cls_set(cp, planes, @@set_rope_kv_b_cls)
                var pc = RopeKvBArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs),
                    half = uint(half), neox = 1u, kpair0 = uint(kpair0),
                    ppp = uint(ppp), layerbase = uint(layerbase), npos = uint(npos),
                    pos0 = uint(pos0), voff = uint(merged ? kvd : voff), kstride = uint(merged ? 2 * kvd : kvd), boff = uint(bpad), hasb = biased ? 1u : 0u)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    if (f16) {
                        enc_rope_kv_b_f16_cls(raw, h, sc, pc, int64(wgs))
                    } else {
                        enc_rope_kv_b_cls(raw, h, sc, pc, int64(wgs))
                    }
                }
                var q_cls <- cp |> plane_run(pq)
                var m_cls <- mirror_pair(cp, pkm, pvm, mirn, f16)
                let form = merged ? " over merged k|v rows (kstride 2*kvd, voff kvd)" : ""
                if (!f16) {
                    check_rows(t, "{name}{form}, q", q_cls, q_ref, BAR_APPROX_CTRL, qd, hs)
                }
                mirror_rows_check(t, "{name}{form}", m_cls, mk_ref, mv_ref, w0, npos * kvd, mirn)
                delete q_cls
                delete m_cls
            }
        }
        delete kvm_h
        delete q0h
        delete kvh
        delete csh
        delete bh
        delete oref
    }
}

//! the fused qk-norm + rope + K/V store (`qkn_rope_cls`, or under `f16` its half-mirror twin) on one row, one workgroup a
//! head, against the CPU oracle, with the q/k/v bias rows past a pad under `biased`; the f32 stamp then runs its N-row form
//! over two stacked rows, each at its own token meta, against the one-row kernel bit for bit
def private qkn_rope_arm(t : T?; f16, biased : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let nh = 4
        let nkvh = 2
        let hs = 64
        let half = hs / 2
        let qd = nh * hs
        let kvd = nkvh * hs
        let neox = 1
        // ONE mirror row both planes: the class twin composes it as layerbase + pos*kvd (the serving ladder always wrote
        // equal K/V rows; the independent-rows freedom was the old kernel's meta accident, not a contract)
        let krow = 192
        let eps = 0.00001
        let mirn2 = krow + nkvh * hs
        let name = f16 ? "qkn_rope_f16{biased ? " with the q/k/v bias" : ""}" : "qkn_rope_kv"
        var q0h <- row_fill(qd, 31, 0, 0.05, 1.6, 67)
        var kvh <- row_fill(qd + 2 * kvd, 23, 0, 0.04, 1.5, 71)
        var csh <- row_fill(hs, 13, 0, 0.045, 0.9, 41)
        var wnh <- [for (i in range(2 * hs)); 0.75 + float(i % 11) * 0.05]
        let bpad = biased ? 96 : 0   // the bias rows sit past a pad, so boff is exercised off its default
        var bh <- biased ? bias_past_pad(bpad, qd + 2 * kvd, 17, 0.02, 0.28, 29, 77.0) : array<float>()
        let wgs = nh + nkvh
        var pc = QknRopeArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs),
            half = uint(half), neox = uint(neox), nh = uint(nh),
            layerbase = uint(krow), qwoff = 0u,
            kwoff = uint(hs), eps = eps, boff = uint(bpad), hasb = biased ? 1u : 0u)
        var q_cls, m_cls : array<float>
        with_cell_planes() $(var cp : CellPlanes) {
            let pq = cp |> io_(q0h)
            let pkv = cp |> in_(kvh)
            let pkm = mirror_plane(cp, mirn2, f16)
            let pvm = mirror_plane(cp, mirn2, f16)
            let pcs = cp |> in_(csh)
            let pwn = cp |> in_(wnh)
            let ptok = cp |> in_plain([TokMeta()])   // mirror row = layerbase alone
            let pb = biased ? in_(cp, bh) : pcs
            let planes = fixed_array(pq, pkv, pkm, pvm, pcs, pwn, ptok, pb)
            var sc = f16 ? cls_set(cp, planes, @@set_qkn_rope_f16_cls) : cls_set(cp, planes, @@set_qkn_rope_cls)
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                if (f16) {
                    enc_qkn_rope_f16_cls(raw, h, sc, pc, int64(wgs))
                } else {
                    enc_qkn_rope_cls(raw, h, sc, pc, int64(wgs))
                }
            }
            delete q_cls
            delete m_cls
            q_cls <- cp |> plane_run(pq)
            m_cls <- mirror_pair(cp, pkm, pvm, mirn2, f16)
        }
        var ref <- qkn_rope_rows(q0h, kvh, bh, wnh, csh, bpad, nh, nkvh, hs, krow, mirn2, eps) $(x : float) => f16 ? f16r(x) : x
        check_rows(t, "{name}, q", q_cls, ref.q, BAR_APPROX_CTRL, qd, hs)
        mirror_rows_check(t, name, m_cls, ref.mk, ref.mv, krow, nkvh * hs, mirn2)
        if (!f16) {
            qkn_rope_n_rows(t, q0h, kvh, csh, wnh, bh, pc)
        }
        delete q0h
        delete kvh
        delete csh
        delete wnh
        delete bh
        delete ref
        delete q_cls
        delete m_cls
    }
}

//! one `qkn_rope_cls` dispatch of `wgs` workgroups under `pc` into an N-form cell's outputs `o` (the q plane, `q` uploaded over
//! it, then the K and V mirrors) over `cp`'s input planes `ins` (the kv rows, the cos row, the norm rows, the token meta, the bias)
def private qkn_cell_enc(cp; o : NformOuts; ins : int[5]; q : array<float>; pc; wgs : int; raw; var h) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        upload_region_at(o.bufs[0], 0l, unsafe(addr<uint8 const?>(q[0])), long_length(q) * 4l)
        var sc = set_qkn_rope_cls(fixed_array(o.bufs[0], cp.dev[ins[0]], o.bufs[1], o.bufs[2], cp.dev[ins[1]], cp.dev[ins[2]], cp.dev[ins[3]], cp.dev[ins[4]]),
            fixed_array(o.bytes[0], cp.bytes[ins[0]], o.bytes[1], o.bytes[2], cp.bytes[ins[1]], cp.bytes[ins[2]], cp.bytes[ins[3]], cp.bytes[ins[4]]),
            fixed_array(1u, 0u, 2u, 4u, 0u, 0u, 0u, 0u))
        var pcv = pc
        enc_qkn_rope_cls(raw, h, sc, pcv, int64(wgs))
    }
}

//! `qkn_rope_cls`'s N-row form over two stacked rows of `qkn_rope_arm`'s fixture (`pc` its one-row arguments, `wnh` / `bh` its
//! norm and bias rows - no bias plane where `bh` is empty), each row at its own token meta, against the one-row kernel on that row
//! alone at position 0 word for word through `nform_cols_cell`: the q rows in place, the mirror rows past layerbase
def private qkn_rope_n_rows(t : T?; q0h, kvh, csh, wnh, bh : array<float>; pc) {
    let nh = 4
    let nkvh = 2
    let hs = 64
    let qd = nh * hs
    let kvd = nkvh * hs
    let krow = 192
    let wgs = nh + nkvh
    let kvrow = qd + 2 * kvd
    var q2h, kv2h, cs2h : array<float>
    q2h |> resize(2 * qd)
    kv2h |> resize(2 * kvrow)
    cs2h |> resize(2 * hs)
    for (i in range(qd)) {
        q2h[i] = q0h[i]
        q2h[qd + i] = q0h[qd - 1 - i] * 0.7 - 0.2
    }
    for (i in range(kvrow)) {
        kv2h[i] = kvh[i]
        kv2h[kvrow + i] = kvh[kvrow - 1 - i] * 0.6 + 0.1
    }
    for (i in range(hs)) {
        cs2h[i] = csh[i]
        cs2h[hs + i] = csh[hs - 1 - i] * 0.9
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        with_cell_planes() $(var cp : CellPlanes) {
            let pwn = cp |> in_(wnh)
            let pkv0 = cp |> in_(copy_span(kv2h, 0, kvrow))
            let pkv1 = cp |> in_(copy_span(kv2h, kvrow, kvrow))
            let pcs0 = cp |> in_(copy_span(cs2h, 0, hs))
            let pcs1 = cp |> in_(copy_span(cs2h, hs, hs))
            let pkv = fixed_array(pkv0, pkv1)
            let pcs = fixed_array(pcs0, pcs1)
            let ptok1 = cp |> in_plain([TokMeta()])
            let pkv2 = cp |> in_(kv2h)
            let pcs2 = cp |> in_(cs2h)
            let ptok2 = cp |> in_plain([TokMeta(pos = 0u), TokMeta(pos = 1u)])
            let pb = empty(bh) ? -1 : in_(cp, bh)
            var pc2 = pc
            pc2.rowwg = uint(wgs)
            pc2.kvrow = uint(kvrow)
            pc2.qrow = uint(qd)
            pc2.cosrow = uint(hs)
            var planes <- nform_cols_cell(t, "qkn_rope_kv over two rows", [2], 2,
                [NformPlane(name = "the q rows", words = qd), NformPlane(name = "the K mirror rows", words = kvd, head = krow),
                 NformPlane(name = "the V mirror rows", words = kvd, head = krow)], 7u,
                $(c : int; raw : VkCommandBuffer; var h : VkHaz; o : NformOuts) {
                    qkn_cell_enc(cp, o, fixed_array(pkv[c], pcs[c], pwn, ptok1, pb < 0 ? pcs[c] : pb), copy_span(q2h, c * qd, qd), pc, wgs, raw, h)
                },
                $(nrows : int; raw : VkCommandBuffer; var h : VkHaz; o : NformOuts) {
                    qkn_cell_enc(cp, o, fixed_array(pkv2, pcs2, pwn, ptok2, pb < 0 ? pcs2 : pb), copy_span(q2h, 0, nrows * qd), pc2, wgs * nrows, raw, h)
                })
            delete planes
        }
    }
    delete q2h
    delete kv2h
    delete cs2h
}

[test]
def test_vkd_rope_b_pair(t0 : T?) {
    t0 |> run("batch rope + fused qkn-rope class kernels == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_rope_kv_b_cls() && ensure_qkn_rope_cls())) {
                t |> skip("no Vulkan device")
                return
            }
            rope_kv_b_arm(t, false, true)
            qkn_rope_arm(t, false, true)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_at_prep(t0 : T?) {
    t0 |> run("attention prep class (q+k collapse) == the CPU oracle (gated+qknorm+partial rope+projection bias)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!ensure_at_prep_cls()) {
                t |> skip("at_prep class declined on this device (workgroup cap)")
                return
            }
            let nh = 4
            let nkvh = 2
            let hs = 64
            let half = 16   // PARTIAL rope: dims [32, 64) pass through unroped
            let qd = nh * hs
            let kvd = nkvh * hs
            let rows = 3
            let w0 = 5      // the k panel lands at absolute positions
            let eps = 0.00001
            let flags = 3   // gated q + qk-norm
            // smalls layout (offsets ride meta words / push args): eps@0, rmsq@16, rmsk@16+hs,
            // cos rows, sin rows
            let rmsq_off = 16
            let rmsk_off = 16 + hs
            let cos_off = 16 + 2 * hs
            let sin_off = cos_off + rows * half
            let sm_n = sin_off + rows * half
            var srcqh <- row_fill(rows * 2 * qd, 19, 0, 0.045, 1.7, 79)
            var srckh <- row_fill(rows * kvd, 27, 0, 0.055, 1.4, 59)
            var smh <- row_fill(sm_n, 11, 0, 0.05, 0.8, 37)
            var bh <- row_fill(qd + kvd, 7, 0, 0.03, 0.3, 23)   //! the projection bias: [q (qd) | k (kvd)], the k pass reads at boff = qd
            smh[0] = eps
            var o_cls : array<float>
            with_cell_planes() $(var cp : CellPlanes) {
                let psq = cp |> in_(srcqh)
                let psk = cp |> in_(srckh)
                let psm = cp |> in_(smh)
                let pb = cp |> in_(bh)
                let pqp = cp |> out_(rows * qd)
                let pg = cp |> out_(rows * qd)
                let pkp = cp |> out_((w0 + rows) * kvd)
                // the class collapse: ONE kernel, two dispatches with different push values
                var scq = cp |> cls_set(fixed_array(psq, pqp, pg, psm, pb), @@set_at_prep_cls)
                var sck = cp |> cls_set(fixed_array(psk, pkp, pg, psm, pb), @@set_at_prep_cls)
                var pcq = AtPrepArgs(d = uint(qd), nh = uint(nh), hs = uint(hs),
                    half = uint(half), flags = uint(flags), rms_off = uint(rmsq_off),
                    cos_off = uint(cos_off), sin_off = uint(sin_off), obase = 0u,
                    qsrc = 1u, boff = 0u, hasb = 1u, eps = eps)
                var pck = AtPrepArgs(d = uint(kvd), nh = uint(nkvh), hs = uint(hs),
                    half = uint(half), flags = uint(flags), rms_off = uint(rmsk_off),
                    cos_off = uint(cos_off), sin_off = uint(sin_off), obase = uint(w0),
                    qsrc = 0u, boff = uint(qd), hasb = 1u, eps = eps)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_at_prep_cls(raw, h, scq, pcq, int64(rows))
                    enc_at_prep_cls(raw, h, sck, pck, int64(rows))
                }
                for (p in fixed_array(pqp, pg, pkp)) {
                    o_cls |> push_from(cp |> plane_run(p))
                }
            }
            //! oracle: deinterleave (gated q), bias, per-head rms with the pass's weight row, partial NEOX rope over dims [0, 2*half), panel rows at obase + p
            var o_ref : array<float>
            o_ref |> resize(2 * rows * qd + (w0 + rows) * kvd)
            let kb0 = 2 * rows * qd + w0 * kvd
            for (p in range(rows)) {
                for (leg in range(2)) {
                    let isq = leg == 0
                    let d = isq ? qd : kvd
                    let nhh = isq ? nh : nkvh
                    let rms_off = isq ? rmsq_off : rmsk_off
                    let boff = isq ? 0 : qd
                    var rrow : array<float>
                    rrow |> resize(d)
                    for (i in range(d)) {
                        let hh = i / hs
                        let j = i % hs
                        if (isq) {
                            rrow[i] = srcqh[p * 2 * qd + hh * 2 * hs + j] + bh[i]
                            o_ref[rows * qd + p * qd + i] = srcqh[p * 2 * qd + hh * 2 * hs + hs + j]
                        } else {
                            rrow[i] = srckh[p * kvd + i] + bh[boff + i]
                        }
                    }
                    for (hh in range(nhh)) {
                        unsafe {
                            rmsnorm(addr<float?>(rrow[hh * hs]), addr<float const?>(rrow[hh * hs]), addr<float const?>(smh[rms_off]), int64(hs), eps)
                        }
                    }
                    rope_tab_ref(rrow, 0, hs, 2 * half, d, true, smh, smh, cos_off + p * half, sin_off + p * half)
                    for (i in range(d)) {
                        o_ref[(isq ? p * qd : kb0 + p * kvd) + i] = rrow[i]
                    }
                    delete rrow
                }
            }
            var bad = 0
            for (i in range(2 * rows * qd)) {   // q panel + gate stash
                if (!approx(o_cls[i], o_ref[i])) {
                    bad++
                }
            }
            for (i in range(rows * kvd)) {   // k panel: only the written absolute rows
                if (!approx(o_cls[kb0 + i], o_ref[kb0 + i])) {
                    bad++
                }
            }
            t |> success(bad == 0, "at_prep class collapse matches the CPU oracle ({bad} off)")
            delete srcqh
            delete srckh
            delete smh
            delete bh
            delete o_ref
            delete o_cls
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private HCT_DIM = 96        //! off the 256-lane workgroup: the row loops take a tail, the elementwise grids a partial group
let private HCT_HC = 4
let private HCT_LR = 32         //! one Q8_0 block of low-rank row
let private HCT_HCD = HCT_HC * HCT_DIM
let private HCT_EPS = 1e-6

//! the hc_mix oracle over `rows` wide rows: the engine's mixer tail with no scatter logits, the mean of each row's sigmoid-gated streams
def private hc_mix_ref(xn, gate : array<float>; rows : int) : array<float> {
    var want : array<float>
    want |> resize(rows * HCT_DIM)
    var gated := xn
    for (r in range(rows)) {
        unsafe {
            hc_mix_tail(addr(gated[r * HCT_HCD]), addr(gate[r * HCT_HCD]), null, null, addr(want[r * HCT_DIM]), int64(HCT_DIM), int64(HCT_HC))
        }
    }
    delete gated
    return <- want
}

//! the CPU rmsnorm of `n` elements from `xo` under the weight row at `wo`, into `out` from `oo`
def private hct_rms(x : array<float>; xo : int; w : array<float>; wo : int; n : int; var out : array<float>; oo : int) {
    unsafe {
        rmsnorm(addr<float?>(out[oo]), addr<float const?>(x[xo]), addr<float const?>(w[wo]), int64(n), HCT_EPS)
    }
}

//! the scatter's CPU form: res_c += out x 2 sigmoid(inj_c / hc), `out` the block row, or under `moe` the routed sum plus the shared row at `g`
def private hct_comb_ref(res : array<float>; outr, inj, acc : array<float>; moe, sh : bool; g : float) : array<float> {
    var r := res
    let v <- [for (i in range(HCT_DIM)); moe ? acc[i] + (sh ? outr[i] * g : 0.0) : outr[i]]
    unsafe {
        hc_combine(addr(r[0]), addr(v[0]), addr(inj[0]), int64(HCT_DIM), int64(HCT_HC))
    }
    return <- r
}

[test]
def test_vkd_hc_seams(t0 : T?) {
    t0 |> run("hc_norm: stream c of the wide row under gamma row c == the CPU rmsnorm, the site's rows at an offset") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_hc_norm_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let res <- row_fill(HCT_HCD, 31, 7, 0.05, 1.7)
            let wn <- row_fill(2 * HCT_HCD, 13, 5, 0.03, 1.1)   //! two sites' gamma rows: the dispatch reads the second site's
            var want : array<float>
            want |> resize(HCT_HCD)
            unsafe {
                hc_grouped_norm(addr(want[0]), addr(res[0]), addr(wn[HCT_HCD]), int64(HCT_DIM), int64(HCT_HC), HCT_EPS)
            }
            with_cell_planes() $(var cp : CellPlanes) {
                let rp = cp |> in_(res)
                let wp = cp |> in_(wn)
                let xp = cp |> out_(HCT_HCD)
                var s = cp |> cls_set(fixed_array(rp, xp, wp), @@set_hc_norm_cls)
                var pa = HcNormArgs(dim = uint(HCT_DIM), hc = uint(HCT_HC), woff = uint(HCT_HCD), row0 = 0u, eps = HCT_EPS)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_hc_norm_cls(raw, h, s, pa, int64(HCT_HC))
                }
                check_rows(t, "hc_norm vs the grouped CPU rmsnorm", cp |> plane_run(xp), want, BAR_APPROX, HCT_DIM)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
    t0 |> run("hc_lo_rq: silu(lo / hc) into Q8_0 blocks == the CPU quantizer over the activated row") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_hc_lo_rq_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let lo <- row_fill(HCT_LR, 19, 3, 0.4, 6.0)
            var act : array<float>
            act |> resize(HCT_LR)
            for (i in range(HCT_LR)) {
                let v = lo[i] / float(HCT_HC)
                act[i] = silu_f32(v)
            }
            var q_ref : array<int8>
            var s_ref : array<float>
            q_ref |> resize(HCT_LR)
            s_ref |> resize(HCT_LR / 32)
            quantize_q8_0_into(act, int64(HCT_LR), q_ref, s_ref, 0l, 0l)
            with_cell_planes() $(var cp : CellPlanes) {
                let lp = cp |> in_(lo)
                let qp = cp |> out_(HCT_LR / 4)   //! the quant bytes: a float slot holds four
                let sp = cp |> out_(HCT_LR / 32)
                var s = cp |> cls_set(fixed_array(lp, qp, sp), @@set_hc_lo_rq_cls)
                var pa = HcLoArgs(nblk = uint(HCT_LR / 32), inv_hc = 1.0 / float(HCT_HC))
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_hc_lo_rq_cls(raw, h, s, pa, rq_wgs(false, int64(HCT_LR)))
                }
                let scales <- cp |> plane_run(sp)
                check_rows(t, "hc_lo_rq block scales vs the CPU quantizer", scales, s_ref, BAR_APPROX, FLAT_ROWS)
                let qbytes <- staged(cp.host.mapped, cp.host_off[qp], HCT_LR, type<int8>)
                var deq, deq_ref : array<float>
                deq |> resize(HCT_LR)
                deq_ref |> resize(HCT_LR)
                for (i in range(HCT_LR)) {
                    deq[i] = float(int(qbytes[i])) * scales[i / 32]
                    deq_ref[i] = float(int(q_ref[i])) * s_ref[i / 32]
                }
                //! a quant may round a step apart where the device's exp lands the activation on the other side of a boundary: the bar is one step
                check_rows(t, "hc_lo_rq dequantized row vs the CPU quantizer's", deq, deq_ref, Bar(rel = 0.0, abs_floor = 1.01 * s_ref[0], poison = 1.0), FLAT_ROWS)
                delete deq
                delete deq_ref
            }
            delete act
            delete q_ref
            delete s_ref
        } else {
            t |> skip("dasVulkan not present")
        }
    }
    t0 |> run("hc_mix: the sigmoid-gated stream mean into the block input row == the CPU form") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_hc_mix_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let xn <- row_fill(HCT_HCD, 23, 11, 0.05, 1.3)
            let gate <- row_fill(HCT_HCD, 29, 5, 0.2, 3.0)
            let want <- hc_mix_ref(xn, gate, 1)
            with_cell_planes() $(var cp : CellPlanes) {
                let xp = cp |> in_(xn)
                let gp = cp |> in_(gate)
                let bp = cp |> out_(HCT_DIM)
                var s = cp |> cls_set(fixed_array(xp, gp, bp), @@set_hc_mix_cls)
                var pa = HcMixArgs(dim = uint(HCT_DIM), hc = uint(HCT_HC), rows = 1u)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_hc_mix_cls(raw, h, s, pa, int64((HCT_DIM + 255) / 256))
                }
                check_rows(t, "hc_mix vs the CPU gated mean", cp |> plane_run(bp), want, BAR_APPROX, FLAT_ROWS)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
    t0 |> run("hc_comb: the scatter under the site logits - the plain row, the routed sum with the shared row gated, ungated, and with no shared row") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_hc_comb_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let res <- row_fill(HCT_HCD, 37, 3, 0.05, 1.2)
            let outr <- row_fill(HCT_DIM, 41, 7, 0.06, 1.5)
            let inj <- row_fill(HCT_HC, 43, 5, 0.9, 1.8)
            let acc <- row_fill(HCT_DIM, 47, 9, 0.04, 1.1)
            let lstride = 9
            let lidx = lstride - 1   // the gate logit sits in the row's last column
            var logits <- row_fill(lstride, 53, 1, 0.5, 2.0)
            logits[lidx] = 0.7   // a sigmoid off one half, so the ungated arm reads apart
            let g = sigmoid_f32(logits[lidx])
            with_cell_planes() $(var cp : CellPlanes) {
                let rp0 = cp |> io_(res)
                let rp1 = cp |> io_(res)
                let rp2 = cp |> io_(res)
                let rp3 = cp |> io_(res)
                let op = cp |> in_(outr)
                let ip = cp |> in_(inj)
                let ap = cp |> in_(acc)
                let lp = cp |> in_(logits)
                var s0 = cp |> cls_set(fixed_array(rp0, op, ip, ap, lp), @@set_hc_comb_cls)
                var s1 = cp |> cls_set(fixed_array(rp1, op, ip, ap, lp), @@set_hc_comb_cls)
                var s2 = cp |> cls_set(fixed_array(rp2, op, ip, ap, lp), @@set_hc_comb_cls)
                var s3 = cp |> cls_set(fixed_array(rp3, op, ip, ap, lp), @@set_hc_comb_cls)
                let wgs = int64((HCT_HCD + 255) / 256)
                var pa0 = HcCombArgs(dim = uint(HCT_DIM), hc = uint(HCT_HC), moe_on = 0u, sh_on = 0u, gated = 0u, rows = 1u, row0 = 0u, lstride = uint(lstride))
                var pa1 = HcCombArgs(dim = uint(HCT_DIM), hc = uint(HCT_HC), moe_on = 1u, sh_on = 1u, gated = 1u, rows = 1u, row0 = 0u, lstride = uint(lstride))
                var pa2 = HcCombArgs(dim = uint(HCT_DIM), hc = uint(HCT_HC), moe_on = 1u, sh_on = 1u, gated = 0u, rows = 1u, row0 = 0u, lstride = uint(lstride))
                var pa3 = HcCombArgs(dim = uint(HCT_DIM), hc = uint(HCT_HC), moe_on = 1u, sh_on = 0u, gated = 1u, rows = 1u, row0 = 0u, lstride = uint(lstride))
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_hc_comb_cls(raw, h, s0, pa0, wgs)
                    enc_hc_comb_cls(raw, h, s1, pa1, wgs)
                    enc_hc_comb_cls(raw, h, s2, pa2, wgs)
                    enc_hc_comb_cls(raw, h, s3, pa3, wgs)
                }
                check_rows(t, "hc_comb plain (the block row)", cp |> plane_run(rp0), hct_comb_ref(res, outr, inj, acc, false, false, 1.0), BAR_APPROX, HCT_DIM)
                check_rows(t, "hc_comb routed + gated shared row", cp |> plane_run(rp1), hct_comb_ref(res, outr, inj, acc, true, true, g), BAR_APPROX, HCT_DIM)
                check_rows(t, "hc_comb routed + ungated shared row", cp |> plane_run(rp2), hct_comb_ref(res, outr, inj, acc, true, true, 1.0), BAR_APPROX, HCT_DIM)
                check_rows(t, "hc_comb routed, no shared row", cp |> plane_run(rp3), hct_comb_ref(res, outr, inj, acc, true, false, g), BAR_APPROX, HCT_DIM)
                let r1 <- cp |> plane_run(rp1)
                let r2 <- cp |> plane_run(rp2)
                t |> success(mismatch_approx(r1, r2) > 0, "the gated and ungated arms land apart (the gate is read)")
            }
            delete logits
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the window forms of the mixer seams: the norm at a row offset, the gated mean and the scatter over rows - each against the
//! one-row CPU form applied a row at a time, the rows outside the dispatch's slice untouched
[test]
def test_vkd_hc_rows(t0 : T?) {
    t0 |> run("hc_init: hc copies of every embedded row open the wide rows") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_hc_init_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let rows = 3
            let x <- row_fill(rows * HCT_DIM, 31, 7, 0.05, 1.7)
            var want : array<float>
            want |> resize(rows * HCT_HCD)
            for (r in range(rows)) {
                for (c in range(HCT_HC)) {
                    for (i in range(HCT_DIM)) {
                        want[r * HCT_HCD + c * HCT_DIM + i] = x[r * HCT_DIM + i]
                    }
                }
            }
            with_cell_planes() $(var cp : CellPlanes) {
                let xp = cp |> in_(x)
                let rp = cp |> out_(rows * HCT_HCD)
                var s = cp |> cls_set(fixed_array(xp, rp), @@set_hc_init_cls)
                var pa = HcInitArgs(dim = uint(HCT_DIM), hc = uint(HCT_HC), rows = uint(rows))
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_hc_init_cls(raw, h, s, pa, int64((rows * HCT_HCD + 255) / 256))
                }
                check_rows(t, "hc_init wide rows", cp |> plane_run(rp), want, BAR_APPROX, HCT_HCD)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
    t0 |> run("hc_norm over rows: two wide rows normed at once, and the last row alone from row0 into row 0 (the head mixer's read)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_hc_norm_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let res <- row_fill(2 * HCT_HCD, 31, 7, 0.05, 1.7)
            let wn <- row_fill(HCT_HCD, 13, 5, 0.03, 1.1)
            var want : array<float>
            want |> resize(2 * HCT_HCD)
            for (g in range(2 * HCT_HC)) {
                hct_rms(res, g * HCT_DIM, wn, (g % HCT_HC) * HCT_DIM, HCT_DIM, want, g * HCT_DIM)
            }
            let want1 <- copy_span(want, HCT_HCD, HCT_HCD)
            with_cell_planes() $(var cp : CellPlanes) {
                let rp = cp |> in_(res)
                let wp = cp |> in_(wn)
                let xp = cp |> out_(2 * HCT_HCD)
                let xp1 = cp |> out_(HCT_HCD)
                var s = cp |> cls_set(fixed_array(rp, xp, wp), @@set_hc_norm_cls)
                var s1 = cp |> cls_set(fixed_array(rp, xp1, wp), @@set_hc_norm_cls)
                var pa = HcNormArgs(dim = uint(HCT_DIM), hc = uint(HCT_HC), woff = 0u, row0 = 0u, eps = HCT_EPS)
                var pa1 = HcNormArgs(dim = uint(HCT_DIM), hc = uint(HCT_HC), woff = 0u, row0 = 1u, eps = HCT_EPS)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_hc_norm_cls(raw, h, s, pa, int64(2 * HCT_HC))
                    enc_hc_norm_cls(raw, h, s1, pa1, int64(HCT_HC))
                }
                check_rows(t, "hc_norm two rows", cp |> plane_run(xp), want, BAR_APPROX, HCT_DIM)
                check_rows(t, "hc_norm the last row from row0 into row 0", cp |> plane_run(xp1), want1, BAR_APPROX, HCT_DIM)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
    t0 |> run("hc_mix over rows: two rows' gated stream means") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_hc_mix_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let xn <- row_fill(2 * HCT_HCD, 23, 11, 0.05, 1.3)
            let gate <- row_fill(2 * HCT_HCD, 29, 5, 0.2, 3.0)
            let want <- hc_mix_ref(xn, gate, 2)
            with_cell_planes() $(var cp : CellPlanes) {
                let xp = cp |> in_(xn)
                let gp = cp |> in_(gate)
                let bp = cp |> out_(2 * HCT_DIM)
                var s = cp |> cls_set(fixed_array(xp, gp, bp), @@set_hc_mix_cls)
                var pa = HcMixArgs(dim = uint(HCT_DIM), hc = uint(HCT_HC), rows = 2u)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_hc_mix_cls(raw, h, s, pa, int64((2 * HCT_DIM + 255) / 256))
                }
                check_rows(t, "hc_mix two rows", cp |> plane_run(bp), want, BAR_APPROX, HCT_DIM)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
    t0 |> run("hc_comb over rows: the routed + gated scatter of rows 1..2 of three, each row under its own logits, row 0 untouched") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_hc_comb_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let rows = 3
            let lstride = 9
            let lidx = lstride - 1
            let res <- row_fill(rows * HCT_HCD, 37, 3, 0.05, 1.2)
            let outr <- row_fill(rows * HCT_DIM, 41, 7, 0.06, 1.5)
            let inj <- row_fill(rows * HCT_HC, 43, 5, 0.9, 1.8)
            let acc <- row_fill(rows * HCT_DIM, 47, 9, 0.04, 1.1)
            let logits <- row_fill(rows * lstride, 53, 1, 0.5, 2.0)
            var want : array<float>
            want |> reserve(rows * HCT_HCD)
            want |> push_from(copy_span(res, 0, HCT_HCD))
            for (r in range(1, rows)) {
                let g = sigmoid_f32(logits[r * lstride + lidx])
                want |> push_from(hct_comb_ref(copy_span(res, r * HCT_HCD, HCT_HCD), copy_span(outr, r * HCT_DIM, HCT_DIM),
                    copy_span(inj, r * HCT_HC, HCT_HC), copy_span(acc, r * HCT_DIM, HCT_DIM), true, true, g))
            }
            with_cell_planes() $(var cp : CellPlanes) {
                let rp = cp |> io_(res)
                let op = cp |> in_(outr)
                let ip = cp |> in_(inj)
                let ap = cp |> in_(acc)
                let lp = cp |> in_(logits)
                var s = cp |> cls_set(fixed_array(rp, op, ip, ap, lp), @@set_hc_comb_cls)
                var pa = HcCombArgs(dim = uint(HCT_DIM), hc = uint(HCT_HC), moe_on = 1u, sh_on = 1u, gated = 1u,
                    rows = uint(rows - 1), row0 = 1u, lstride = uint(lstride))
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_hc_comb_cls(raw, h, s, pa, int64(((rows - 1) * HCT_HCD + 255) / 256))
                }
                check_rows(t, "hc_comb rows 1..2 scattered, row 0 kept", cp |> plane_run(rp), want, BAR_APPROX, HCT_DIM)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

let private PLT_NROWS = 5   //! the conv ring: (kern - 1) x dil + 1 rows at kern 3, dil 2
let private PLT_KERN = 3
let private PLT_DIL = 2

//! the PLE gate's CPU form for the row at `row_at`: each stream's normed key against its normed residual, the signed-sqrt sigmoid
//! gate on the value row at `val_at` into `gated`, the gated row normed into `normed` at `normed_at`; returns the stream dots
def private ple_gate_row_ref(key, res, val, wn : array<float>; row_at, val_at : int; var gated : array<float>; var normed : array<float>;
                             normed_at : int) : array<float> {
    let dim = HCT_DIM
    let hcd = HCT_HCD
    var kn, qn : array<float>
    kn |> resize(hcd)
    qn |> resize(hcd)
    var svs : array<float>
    svs |> reserve(HCT_HC)
    for (c in range(HCT_HC)) {
        hct_rms(key, row_at + c * dim, wn, c * dim, dim, kn, c * dim)
        hct_rms(res, row_at + c * dim, wn, hcd + c * dim, dim, qn, c * dim)
        var sv = 0.0
        for (i in range(dim)) {
            sv += kn[c * dim + i] * qn[c * dim + i]
        }
        sv *= 1.0 / sqrt(float(dim))
        svs |> push(sv)
        let mag = sqrt(max(abs(sv), 1.0e-6))
        let g = sigmoid_f32(sv > 0.0 ? mag : (sv < 0.0 ? -mag : 0.0))
        for (i in range(dim)) {
            gated[row_at + c * dim + i] = val[val_at + i] * g
        }
        hct_rms(gated, row_at + c * dim, wn, 2 * hcd + c * dim, dim, normed, normed_at + c * dim)
    }
    delete kn
    delete qn
    return <- svs
}

//! the side input's two kernels at position `pos` against the CPU form: the gate's `gated` rows and the ring slot it writes, the
//! conv's landing in the residual; a position under the taps' reach (`pos` < (kern - 1) x dil) reads zero before the sequence
def private ple_side_case(t : T?; pos : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let dim = HCT_DIM
        let hc = HCT_HC
        let hcd = HCT_HCD
        var key <- row_fill(hcd, 31, 7, 0.05, 1.7)
        for (i in range(dim, 2 * dim)) {   //! streams 1 and 3 read a negated key: their dots take the other sign
            key[i] = -key[i]
            key[2 * dim + i] = -key[2 * dim + i]
        }
        let res <- row_fill(hcd, 13, 5, 0.06, 1.9)
        let val <- row_fill(dim, 17, 3, 0.05, 1.4)
        let hist <- row_fill(PLT_NROWS * hcd, 19, 11, 0.04, 1.2)
        let wn <- row_fill(3 * hcd + hcd * PLT_KERN, 23, 9, 0.03, 1.1)
        var gated_ref : array<float>
        var hist_ref := hist
        var res_ref := res
        gated_ref |> resize(hcd)
        let slot = (pos % PLT_NROWS) * hcd
        var svs <- ple_gate_row_ref(key, res, val, wn, 0, 0, gated_ref, hist_ref, slot)
        var npos_sv = 0
        var nneg_sv = 0
        for (sv in svs) {
            if (sv > 0.0) {
                npos_sv++
            } elif (sv < 0.0) {
                nneg_sv++
            }
        }
        delete svs
        t |> success(npos_sv > 0 && nneg_sv > 0, "the fixture's stream dots take both signs ({npos_sv} positive, {nneg_sv} negative)")
        for (e in range(hcd)) {
            var co = 0.0
            for (k in range(PLT_KERN)) {
                let src_pos = pos - (PLT_KERN - 1 - k) * PLT_DIL
                if (src_pos >= 0) {
                    co += wn[3 * hcd + e * PLT_KERN + k] * hist_ref[(src_pos % PLT_NROWS) * hcd + e]
                }
            }
            res_ref[e] = (res[e] + gated_ref[e]) + silu_f32(co)
        }
        var toks <- [TokMeta(pos = uint(pos), cnt = uint(pos + 1), dnslot = 0u)]
        with_cell_planes() $(var cp : CellPlanes) {
            let kp = cp |> in_(key)
            let rp = cp |> io_(res)
            let vp = cp |> in_(val)
            let gp = cp |> out_(hcd)
            let hp = cp |> io_(hist)
            let wp = cp |> in_(wn)
            let tp = cp |> in_plain(toks)
            var sg = cp |> cls_set(fixed_array(kp, rp, vp, gp, hp, wp, tp, hp), @@set_ple_gate_cls)
            var sc = cp |> cls_set(fixed_array(rp, gp, hp, wp, tp, hp), @@set_ple_conv_cls)
            var pg = PleGateArgs(dim = uint(dim), hc = uint(hc), nrows = uint(PLT_NROWS),
                rows = 0u, pos0 = 0u, eps = HCT_EPS, inv_sq = 1.0 / sqrt(float(dim)))
            var pc = PleConvArgs(dim = uint(dim), hc = uint(hc), kern = uint(PLT_KERN), dil = uint(PLT_DIL),
                rows = 0u, pos0 = 0u)
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_ple_gate_cls(raw, h, sg, pg, int64(hc))
                enc_ple_conv_cls(raw, h, sc, pc, int64((hcd + 255) / 256))
            }
            check_rows(t, "ple_gate gated rows at pos {pos}", cp |> plane_run(gp), gated_ref, BAR_APPROX, dim)
            check_rows(t, "ple_gate conv ring at pos {pos} (the slot written, the others kept)", cp |> plane_run(hp), hist_ref, BAR_APPROX, hcd)
            check_rows(t, "ple_conv residual landing at pos {pos}", cp |> plane_run(rp), res_ref, BAR_APPROX, dim)
        }
        delete toks
        delete gated_ref
        delete hist_ref
        delete res_ref
    }
}

//! the side input's window form over `rows` rows from `pos0` against the CPU form a row at a time: the gate's rows and its normed
//! panel rows, the conv reading a tap inside the window off the panel and before it off the ring;
//! `tok_pos`: `pos0` off the token record (`PLE_POS_TOK`, the verify's form) and the rows committed to the ring after the conv (`PleCommit`); without it the ring stays untouched
def private ple_panel_case(t : T?; rows, pos0 : int; tok_pos : bool = false) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let dim = HCT_DIM
        let hc = HCT_HC
        let hcd = HCT_HCD
        var key <- row_fill(rows * hcd, 31, 7, 0.05, 1.7)
        for (r in range(rows)) {
            for (i in range(dim, 2 * dim)) {   //! streams 1 and 3 read a negated key, as the one-row case
                key[r * hcd + i] = -key[r * hcd + i]
                key[r * hcd + 2 * dim + i] = -key[r * hcd + 2 * dim + i]
            }
        }
        let res <- row_fill(rows * hcd, 13, 5, 0.06, 1.9)
        let val <- row_fill(rows * dim, 17, 3, 0.05, 1.4)
        let hist <- row_fill(PLT_NROWS * hcd, 19, 11, 0.04, 1.2)
        let wn <- row_fill(3 * hcd + hcd * PLT_KERN, 23, 9, 0.03, 1.1)
        var gated_ref, panel_ref : array<float>
        var res_ref := res
        gated_ref |> resize(rows * hcd)
        panel_ref |> resize(rows * hcd)
        for (r in range(rows)) {
            var svs <- ple_gate_row_ref(key, res, val, wn, r * hcd, r * dim, gated_ref, panel_ref, r * hcd)
            delete svs
        }
        var from_ring = 0
        var from_panel = 0
        for (r in range(rows)) {
            let pos = pos0 + r
            for (e in range(hcd)) {
                var co = 0.0
                for (k in range(PLT_KERN)) {
                    let src_pos = pos - (PLT_KERN - 1 - k) * PLT_DIL
                    if (src_pos >= pos0) {
                        co += wn[3 * hcd + e * PLT_KERN + k] * panel_ref[(src_pos - pos0) * hcd + e]
                        from_panel += e == 0 ? 1 : 0
                    } elif (src_pos >= 0) {
                        co += wn[3 * hcd + e * PLT_KERN + k] * hist[(src_pos % PLT_NROWS) * hcd + e]
                        from_ring += e == 0 ? 1 : 0
                    }
                }
                res_ref[r * hcd + e] = (res[r * hcd + e] + gated_ref[r * hcd + e]) + silu_f32(co)
            }
        }
        t |> success(from_ring > 0 && from_panel > 0, "the fixture's taps read both the ring and the panel ({from_ring} ring, {from_panel} panel)")
        var hist_ref := hist
        if (tok_pos) {
            for (r in range(rows)) {
                let slot = (pos0 + r) % PLT_NROWS
                for (e in range(hcd)) {
                    hist_ref[slot * hcd + e] = panel_ref[r * hcd + e]
                }
            }
        }
        var toks <- [TokMeta(pos = uint(tok_pos ? pos0 : 0), cnt = 1u, dnslot = 0u)]
        with_cell_planes() $(var cp : CellPlanes) {
            let kp = cp |> in_(key)
            let rp = cp |> io_(res)
            let vp = cp |> in_(val)
            let gp = cp |> out_(rows * hcd)
            let hp = cp |> io_(hist)
            let wp = cp |> in_(wn)
            let tp = cp |> in_plain(toks)
            let pp = cp |> out_(rows * hcd)
            var sg = cp |> cls_set(fixed_array(kp, rp, vp, gp, hp, wp, tp, pp), @@set_ple_gate_cls)
            var sc = cp |> cls_set(fixed_array(rp, gp, hp, wp, tp, pp), @@set_ple_conv_cls)
            var sm = cp |> cls_set(fixed_array(pp, hp, tp), @@set_ple_commit_cls)
            let push_pos = tok_pos ? PLE_POS_TOK : uint(pos0)
            var pg = PleGateArgs(dim = uint(dim), hc = uint(hc), nrows = uint(PLT_NROWS),
                rows = uint(rows), pos0 = push_pos, eps = HCT_EPS, inv_sq = 1.0 / sqrt(float(dim)))
            var pc = PleConvArgs(dim = uint(dim), hc = uint(hc), kern = uint(PLT_KERN), dil = uint(PLT_DIL),
                rows = uint(rows), pos0 = push_pos)
            var pm = PleCommitArgs(dim = uint(dim), hc = uint(hc), nrows = uint(PLT_NROWS), rows = uint(rows))
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_ple_gate_cls(raw, h, sg, pg, int64(rows * hc))
                enc_ple_conv_cls(raw, h, sc, pc, int64((rows * hcd + 255) / 256))
                if (tok_pos) {
                    enc_ple_commit_cls(raw, h, sm, pm, int64((rows * hcd + 255) / 256))
                }
            }
            let form = tok_pos ? "pos off the token record" : "pos off the push"
            check_rows(t, "ple_gate window gated rows from pos {pos0} ({form})", cp |> plane_run(gp), gated_ref, BAR_APPROX, dim)
            check_rows(t, "ple_gate window panel rows (the normed gated rows, {form})", cp |> plane_run(pp), panel_ref, BAR_APPROX, dim)
            if (tok_pos) {
                check_rows(t, "ple_commit lands the rows in the ring's slots of their positions", cp |> plane_run(hp), hist_ref, BAR_APPROX, hcd)
            } else {
                check_rows(t, "ple_gate window leaves the ring untouched", cp |> plane_run(hp), hist, BAR_APPROX, hcd)
            }
            check_rows(t, "ple_conv window residual landing (taps off the panel and the ring, {form})", cp |> plane_run(rp), res_ref, BAR_APPROX, dim)
        }
        delete hist_ref
        delete toks
        delete gated_ref
        delete panel_ref
        delete res_ref
    }
}

[test]
def test_vkd_ple_side(t0 : T?) {
    t0 |> run("ple_gate + ple_conv: the n-gram side input's gate, its ring slot and the conv's landing == the CPU form, inside and under the taps' reach") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init() || !ensure_ple_gate_cls() || !ensure_ple_conv_cls() || !ensure_ple_commit_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            ple_side_case(t, 7)   //! every tap reads a written slot
            ple_side_case(t, 1)   //! the two earlier taps sit before the sequence and read zero
            ple_panel_case(t, 3, 6)   //! the window form: three rows whose taps read the ring before the window and the panel inside it
            ple_panel_case(t, 3, 6, true)   //! the verify's form: the position off the token record, three rows committed to the ring after the conv (1152 elements: the last workgroup partial, so the guard's false arm runs)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the deltanet cells' gate: a live device and a deltanet chain the device's shared memory holds; false with the skip registered
//! where either is missing
def private dn_cells_gate(t : T?) : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (!vk_moe_init()) {
            t |> skip("no Vulkan device")
            return false
        }
        if (!g_gpu.dn_shared_ok) {
            t |> skip("dn chain declined on this device (shared-memory cap)")
            return false
        }
        return true
    } else {
        return false
    }
}

//! The sequential delta rule the dn scan cells hold the chunked scan to: every value head's `npos` steps over the conv rows
//! `convh` (q | k | v rows `cd` wide) from the state `sth`, the step's beta, gate, decay and dt at their offsets into the smalls
//! `smh` - the raw o rows [npos x nvh x ds], then the final states [nvh x ds x ds].
def private dn_delta_ref(convh, sth, smh : array<float>; npos, nkh, nvh, ds, beta_off, g_off, a_off, dt_off : int) : array<float> {
    let kd = nkh * ds
    let di = nvh * ds
    let cd = 2 * kd + di
    var ref : array<float>
    ref |> resize(npos * di + nvh * ds * ds)
    var st_h, dvn : array<float>
    st_h |> resize(ds * ds)
    dvn |> resize(ds)
    for (hh in range(nvh)) {
        let kh = hh % nkh
        for (e in range(ds * ds)) {
            st_h[e] = sth[hh * ds * ds + e]
        }
        for (tt in range(npos)) {
            let crow = tt * cd
            let gt = smh[a_off + hh] * softplus(smh[g_off + tt * nvh + hh] + smh[dt_off + hh])
            unsafe {
                dn_delta_head(addr(st_h[0]), addr(convh[crow + kh * ds]), addr(convh[crow + kd + kh * ds]), addr(convh[crow + 2 * kd + hh * ds]),
                    addr(ref[tt * di + hh * ds]), addr(dvn[0]), gt, sigmoid_f32(smh[beta_off + tt * nvh + hh]), int64(ds))
            }
        }
        for (e in range(ds * ds)) {
            ref[npos * di + hh * ds * ds + e] = st_h[e]
        }
    }
    delete st_h
    delete dvn
    return <- ref
}

[test]
def test_vkd_dn_family(t0 : T?) {
    t0 |> run("deltanet class kernels == the CPU oracle (conv, fused step, 2-phase scan)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!dn_cells_gate(t)) return
            verify(ensure_dn_conv_cls() && ensure_dn_step_cls() && ensure_dn_scan_cls() && ensure_dn_scan_p3_cls(),
                "the dn class rails must engage on a device over the shared cap")
            for (ds in fixed_array(64, 128)) {   //! ds 64 keeps a state column in one row part of the fused step; 128, the served hybrids' head size, splits it in two and takes the scan's eight-lane clusters
            let nkh = 2
            let nvh = 4
            let kd = nkh * ds
            let cd = 2 * kd + nvh * ds
            let dconv = 4
            let taps = dconv - 1
            let eps = 0.00001
            let qscale = 1.25
            // --- dn_conv: depthwise conv + SiLU + q/k L2-norm, channel-major over position blocks ---
            {
                let npos = 37   //! a second position block primed from window rows
                let hist_mode = 1
                let taps_off = 16
                let hist_off = taps_off + cd * dconv
                let sm_n = hist_off + cd * taps
                var srch <- row_fill(npos * cd, 19, 0, 0.03, 1.1, 79)
                var smh <- row_fill(sm_n, 11, 0, 0.02, 0.35, 37)
                smh[0] = eps
                smh[1] = qscale
                var o_cls : array<float>
                with_cell_planes() $(var cp : CellPlanes) {
                    let psrc = cp |> in_(srch)
                    let psm = cp |> in_(smh)
                    let po = cp |> out_(npos * cd)
                    var sc = cp |> cls_set(fixed_array(psrc, po, psm), @@set_dn_conv_cls)
                    var pc = DnConvArgs(npos = uint(npos), cd = uint(cd), kd = uint(kd), ds = uint(ds),
                        dconv = uint(dconv), hist_mode = uint(hist_mode), taps_off = uint(taps_off),
                        hist_off = uint(hist_off), eps = eps, qscale = qscale)
                    cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                        enc_dn_conv_cls(raw, h, sc, pc, dn_conv_wgs(int64(cd), int64(npos)))
                    }
                    delete o_cls
                    o_cls <- cp |> plane_run(po)
                }
                // oracle: depthwise conv (history rows below position 0) + SiLU, then per-ds-block
                // L2 norm over the q/k regions with qscale on the q blocks
                var o_ref <- dn_conv_ref(srch, smh, hist_off, smh, taps_off, npos, cd, dconv, kd, ds, eps, qscale)
                check_rows(t, "dn_conv at ds {ds} against the CPU oracle", o_cls, o_ref, BAR_APPROX_CTRL, cd)
                delete srch
                delete smh
                delete o_ref
                delete o_cls
            }
            // --- dn_step_fused: the whole per-v-head decode chain, state + ring in place ---
            {
                let par = 1
                let beta_off = 16
                let g_off = beta_off + nvh
                let a_off = g_off + nvh
                let dt_off = a_off + nvh
                let taps_off = dt_off + nvh
                let wnorm_off = taps_off + cd * dconv
                let hist_off = wnorm_off + ds
                let sm_n = hist_off + 2 * cd * taps
                let nblk = nvh * ds / 32
                var projh <- row_fill(cd + nvh * ds, 23, 0, 0.03, 1.0, 71)
                var sth <- row_fill(nvh * ds * ds, 13, 0, 0.02, 0.5, 53)
                var smh <- row_fill(sm_n, 17, 0, 0.02, 0.4, 43)
                smh[0] = eps
                smh[1] = qscale
                smh[2] = float(par)
                for (i in range(nvh)) {
                    smh[a_off + i] = -0.4 - float(i) * 0.1   // a stays negative (decay)
                }
                var pars <- [par]
                var slots <- [0]
                let offs = fixed_array(beta_off, g_off, a_off, dt_off, taps_off, wnorm_off, hist_off, 0)
                var got <- dn_step_rows_run(projh, sth, smh, pars, slots, 1, nblk, ds, nvh, nkh, cd, dconv, offs, eps, qscale, false)
                // oracle: the whole fused chain — ring conv + silu, q/k L2, chunk-length-1 delta
                // rule in place, gated out-norm, the head's Q8_0 blocks
                var sm_ref : array<float>
                sm_ref := smh
                let rrb = hist_off + par * cd * taps
                let wrb = hist_off + (1 - par) * cd * taps
                for (ch in range(cd)) {
                    for (k in range(taps - 1)) {
                        sm_ref[wrb + ch * taps + k] = smh[rrb + ch * taps + k + 1]
                    }
                    sm_ref[wrb + ch * taps + taps - 1] = projh[ch]
                }
                var st_ref : array<float>
                st_ref := sth
                var out_ref : array<float>
                out_ref |> resize(nvh * ds)
                var out_ref_sig : array<float>   //! the same row under the sigmoid out-gate (the `dn_step_sig_cls` stamp's)
                out_ref_sig |> resize(nvh * ds)
                for (hh in range(nvh)) {
                    let kh = hh % nkh
                    var dq, dk, dv : array<float>
                    dq |> resize(ds)
                    dk |> resize(ds)
                    dv |> resize(ds)
                    for (sl in range(3)) {
                        for (j in range(ds)) {
                            let ch = (sl == 0 ? kh * ds : (sl == 1 ? kd + kh * ds : 2 * kd + hh * ds)) + j
                            var acc = projh[ch] * smh[taps_off + ch * dconv + taps]
                            for (k in range(taps)) {
                                acc += smh[rrb + ch * taps + k] * smh[taps_off + ch * dconv + k]
                            }
                            let sv = silu_f32(acc)
                            if (sl == 0) {
                                dq[j] = sv
                            } elif (sl == 1) {
                                dk[j] = sv
                            } else {
                                dv[j] = sv
                            }
                        }
                    }
                    l2_norm_rows(unsafe(addr(dq[0])), 1l, int64(ds), eps)
                    l2_norm_rows(unsafe(addr(dk[0])), 1l, int64(ds), eps)
                    let graw = smh[g_off + hh] + smh[dt_off + hh]
                    let eg = exp(smh[a_off + hh] * softplus(graw))
                    let bt = sigmoid_f32(smh[beta_off + hh])
                    var qk = 0.0
                    for (j in range(ds)) {
                        dq[j] *= qscale
                        qk += dq[j] * dk[j]
                    }
                    let sbase = hh * ds * ds
                    var dvn, dout : array<float>
                    dvn |> resize(ds)
                    dout |> resize(ds)
                    for (j in range(ds)) {
                        var ks = 0.0
                        var qs = 0.0
                        for (i in range(ds)) {
                            ks += dk[i] * sth[sbase + i * ds + j]
                            qs += dq[i] * sth[sbase + i * ds + j]
                        }
                        dvn[j] = bt * (dv[j] - eg * ks)
                        dout[j] = eg * qs + qk * dvn[j]
                    }
                    for (e in range(ds * ds)) {
                        st_ref[sbase + e] = sth[sbase + e] * eg + dk[e / ds] * dvn[e % ds]
                    }
                    var sum = 0.0
                    for (j in range(ds)) {
                        sum += dout[j] * dout[j]
                    }
                    let inv = 1.0 / sqrt(sum / float(ds) + eps)
                    for (j in range(ds)) {
                        let zv = projh[cd + hh * ds + j]
                        out_ref[hh * ds + j] = dout[j] * inv * smh[wnorm_off + j] * silu_f32(zv)
                        out_ref_sig[hh * ds + j] = dout[j] * inv * smh[wnorm_off + j] * sigmoid_f32(zv)
                    }
                    delete dq
                    delete dk
                    delete dv
                    delete dvn
                    delete dout
                }
                var q_ref : array<uint>
                var s_ref : array<float>
                requant_oracle(out_ref, 0, nvh * ds, false, q_ref, s_ref)
                let bad = (mismatch_approx(got.state, st_ref) + mismatch_approx(got.smalls, sm_ref)
                    + mismatch_approx(got.oscales, s_ref) + mismatch_qbytes(got.oquants, q_ref, 1))
                t |> success(bad == 0, "dn_step_fused matches the CPU oracle at ds {ds} ({bad} elements off)")
                //! the f32 arm (oform 1): the same step leaves the o row as floats - the pre-quant oracle row - and advances state and ring alike
                var gotf <- dn_step_rows_run(projh, sth, smh, pars, slots, 1, nblk, ds, nvh, nkh, cd, dconv, offs, eps, qscale, true)
                let badf = mismatch_approx(gotf.of32, out_ref) + mismatch_approx(gotf.state, st_ref) + mismatch_approx(gotf.smalls, sm_ref)
                t |> success(badf == 0, "dn_step_fused's f32 o row (oform 1) at ds {ds} matches the pre-quant oracle with state and ring advanced alike ({badf} elements off)")
                delete gotf
                //! the sigmoid out-gate stamp (`dn_step_sig_cls`, qwen4exp): the same step, the o row gated by sigmoid(z); the silu oracle is its control
                var gots <- dn_step_rows_run(projh, sth, smh, pars, slots, 1, nblk, ds, nvh, nkh, cd, dconv, offs, eps, qscale, true, false, true)
                let bads = mismatch_approx(gots.of32, out_ref_sig) + mismatch_approx(gots.state, st_ref) + mismatch_approx(gots.smalls, sm_ref)
                let wids = span_maxdiff(gots.of32, out_ref_sig, 0, 0, length(out_ref_sig))
                t |> success(bads == 0, "dn_step_sig's f32 o row at ds {ds} matches the sigmoid-gated oracle with state and ring advanced alike ({bads} elements off, the o row's widest {wids.diff} at {wids.at})")
                t |> success(mismatch_approx(gots.of32, out_ref) > 0, "dn_step_sig at ds {ds} lands off the silu oracle (the two stamps differ)")
                delete gots
                delete out_ref_sig
                delete projh
                delete sth
                delete smh
                delete sm_ref
                delete st_ref
                delete out_ref
                delete q_ref
                delete s_ref
                delete got
                delete pars
                delete slots
            }
            // --- dn_scan + the out-norm over 70 rows: the state advanced through every token ---
            {
                let npos = 70
                let CS = 64
                let nch = (npos + CS - 1) / CS
                let di = nvh * ds
                let slab_stride = 7 * CS * ds + 2 * CS * CS + 4 * CS
                let st_off = nvh * nch * slab_stride
                let o_off = st_off + nvh * (ds * ds + CS * ds)
                let beta_off = 16
                let g_off = beta_off + npos * nvh
                let a_off = g_off + npos * nvh
                let dt_off = a_off + nvh
                let wnorm_off = dt_off + nvh
                let sm_n = wnorm_off + ds
                var convh : array<float>
                convh |> resize(npos * cd)
                for (i in range(npos * cd)) {   //! unit-scale q/k rows, the conv stage's L2-normed shape: on unnormed rows the delta rule's recurrence overflows float range within the window at ds 128
                    convh[i] = (float((i * 19) % 79) * 0.025 - 0.9) * 0.15
                }
                var zh <- row_fill(npos * di, 29, 0, 0.05, 1.5, 61)
                var sth <- row_fill(nvh * ds * ds, 13, 0, 0.02, 0.5, 53)
                var smh <- row_fill(sm_n, 17, 0, 0.03, 0.6, 43)
                smh[0] = eps
                for (i in range(nvh)) {
                    smh[a_off + i] = -0.4 - float(i) * 0.1
                }
                for (i in range(ds)) {
                    smh[wnorm_off + i] = 0.8 + float(i % 9) * 0.04
                }
                let cmp_n = npos * di + nvh * ds * ds
                var pcs = DnScanArgs(npos = uint(npos), cd = uint(cd), kd = uint(kd),
                    nvh = uint(nvh), nkh = uint(nkh), ds = uint(ds),
                    beta_off = uint(beta_off), g_off = uint(g_off), a_off = uint(a_off),
                    dt_off = uint(dt_off), o_off = uint(o_off), di = uint(di))
                var pc3 = DnScanP3Args(npos = uint(npos), nvh = uint(nvh), di = uint(di), ds = uint(ds),
                    o_off = uint(o_off), wnorm_off = uint(wnorm_off), eps = eps)
                //! the scan from the uploaded state, then the out-norm gated by silu(z) and, the second time, by the sigmoid out-gate
                //! stamp (`dn_scan_p3_sig_cls`, qwen4exp)
                var outs : array<array<float>>
                outs |> reserve(2)
                with_cell_planes() $(var cp : CellPlanes) {
                    let pconv = cp |> in_(convh)
                    let pz = cp |> in_(zh)
                    let pst = cp |> io_(sth)
                    let psm = cp |> in_(smh)
                    let pslab = cp |> out_(npos * di, o_off)   //! the o rows past the scan's workspace under the sentinel: a row the scan or the out-norm skips reds
                    var sc1 = cp |> cls_set(fixed_array(pconv, pst, psm, pslab), @@set_dn_scan_cls)
                    for (sig in fixed_array(false, true)) {
                        if (sig) {
                            verify(ensure_dn_scan_p3_sig_cls(), "the sigmoid out-norm stamp must compile on a device that compiled the silu one")
                            cp |> plane_upload(pst, sth)
                            cp |> plane_refill(pslab)
                        }
                        let p3 = fixed_array(pz, pslab, psm)
                        var sc3 = sig ? cp |> cls_set(p3, @@set_dn_scan_p3_sig_cls) : cp |> cls_set(p3, @@set_dn_scan_p3_cls)
                        cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                            enc_dn_scan_cls(raw, h, sc1, pcs, dn_scan_wgs(int64(nvh), int64(ds)))
                            if (sig) {
                                enc_dn_scan_p3_sig_cls(raw, h, sc3, pc3, int64(npos))
                            } else {
                                enc_dn_scan_p3_cls(raw, h, sc3, pc3, int64(npos))
                            }
                        }
                        var o <- cp |> plane_run(pslab)
                        var st_got <- cp |> plane_run(pst)
                        o |> push_from(st_got)
                        delete st_got
                        outs |> emplace(o)
                    }
                }
                var o_cls <- outs[0]
                var o_cls_sig <- outs[1]
                // the chunked WY form equals the sequential rule in exact arithmetic: the drift is reassociation, which the peak-floored bars absorb
                var o_ref <- dn_delta_ref(convh, sth, smh, npos, nkh, nvh, ds, beta_off, g_off, a_off, dt_off)
                var o_ref_sig := o_ref   //! the same rows under the sigmoid out-gate (the `dn_scan_p3_sig_cls` stamp's), the state part alike
                for (tt in range(npos)) {
                    for (hh in range(nvh)) {
                        let at = tt * di + hh * ds
                        var sum = 0.0
                        for (j in range(ds)) {
                            sum += o_ref[at + j] * o_ref[at + j]
                        }
                        let inv = 1.0 / sqrt(sum / float(ds) + eps)
                        for (j in range(ds)) {
                            let zv = zh[at + j]
                            let normed = o_ref[at + j] * inv * smh[wnorm_off + j]
                            o_ref[at + j] = normed * silu_f32(zv)
                            o_ref_sig[at + j] = normed * sigmoid_f32(zv)
                        }
                    }
                }
                let floor = 1e-2 * logits_maxabs(o_ref)
                let bad = mismatch_rel("", o_cls, o_ref, 1e-2, floor)
                t |> success(bad == 0, "dn_scan + out-norm matches the CPU oracle at ds {ds} ({bad} of {cmp_n} off)")
                var poison := o_cls   //! the bar's control: one normed o value pushed past both bars must red
                poison[0] += 1.0 + 2.0 * logits_maxabs(o_ref)
                t |> success(mismatch_rel("", poison, o_ref, 1e-2, floor) > 0, "dn_scan + out-norm at ds {ds}: the bar reds a poisoned value")
                delete poison
                //! the sigmoid out-gate stamp's rows gated by sigmoid(z); the silu oracle is its control
                let bads = mismatch_rel("", o_cls_sig, o_ref_sig, 1e-2, floor)
                let wids = span_maxdiff(o_cls_sig, o_ref_sig, 0, 0, cmp_n)
                t |> success(bads == 0, "dn_scan + the sigmoid out-norm stamp matches the CPU oracle at ds {ds} ({bads} of {cmp_n} off, widest {wids.diff} at {wids.at})")
                t |> success(mismatch_rel("", o_cls_sig, o_ref, 1e-2, floor) > 0, "dn_scan_p3_sig at ds {ds} lands off the silu oracle (the two stamps differ)")
                delete o_cls_sig
                delete o_ref_sig
                delete convh
                delete zh
                delete sth
                delete smh
                delete o_ref
                delete o_cls
                delete outs
            }
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

struct DnRowsOut {
    state : array<float>
    smalls : array<float>
    oscales : array<float>
    oquants : array<uint>
    of32 : array<float>
}

def private dn_step_enc(raw; var h; var sc; var pc; wgs : int64; zsig : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        if (zsig) {
            enc_dn_step_sig_cls(raw, h, sc, pc, wgs)
        } else {
            enc_dn_step_cls(raw, h, sc, pc, wgs)
        }
    }
}

//! `dn_step_cls` over `nrows` rows on buffers of its own - one dispatch, or under `seq` a dispatch a row in order (the rows' first
//! row in the push, the same-slab verify's form) - two calls share no device plane; `zsig` takes the sigmoid out-gate stamp
def private dn_step_rows_run(proj, st, sm : array<float>; pars, slots : array<int>; nrows, nblk, ds, nvh, nkh, cd, dconv : int;
                             offs : int[8]; eps, qscale : float; out_f32 : bool; seq : bool = false; zsig : bool = false) : DnRowsOut {
    var r : DnRowsOut
    static_if (typeinfo builtin_module_exists(vulkan)) {   //! the device half: a fully typed function is inferred in a build without the module too
        let ring = cd * (dconv - 1)
        with_cell_planes() $(var cp : CellPlanes) {
            let pproj = cp |> in_(proj)
            let pst = cp |> io_(st)
            let psm = cp |> io_(sm)
            let psc = cp |> out_(nrows * nblk)
            let pq = cp |> out_(nrows * nblk * 8)
            let ptok = cp |> in_plain([for (i in range(nrows)); TokMeta(parity = uint(pars[i]), dnslot = uint(slots[i]))])
            let pof = cp |> out_(nrows * nvh * ds)
            for (i in range(seq ? 1 : nrows)) {   // the ring image each row writes starts as a sentinel: a skipped ring advance cannot compare equal (sequential rows write the image the row before read, so the first row's alone is filled here - the rest between the dispatches)
                nan_sentinel_upload(cp.dev[psm], int64(ring) * 4l, int64(offs[6] + slots[i] * 2 * ring + (1 - pars[i]) * ring) * 4l)
            }
            if (zsig) {
                verify(ensure_dn_step_sig_cls(), "the sigmoid out-gate stamp must compile on a device that compiled the silu one")
            }
            let idx = fixed_array(pproj, pst, psm, psc, pq, ptok, pof)
            var sc = zsig ? cp |> cls_set(idx, @@set_dn_step_sig_cls) : cp |> cls_set(idx, @@set_dn_step_cls)
            var pc = DnStepArgs(cd = uint(cd), kd = uint(nkh * ds), nkh = uint(nkh), ds = uint(ds), dconv = uint(dconv),
                beta_off = uint(offs[0]), g_off = uint(offs[1]), a_off = uint(offs[2]), dt_off = uint(offs[3]),
                taps_off = uint(offs[4]), wnorm_off = uint(offs[5]), hist_off = uint(offs[6]),
                eps = eps, qscale = qscale, out_f32 = out_f32 ? 1u : 0u, beta_row_stride = uint(offs[7]))
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                if (seq) {
                    for (i in range(nrows)) {
                        if (i > 0) {   // the image this row writes held the row before's input: a sentinel now, so a skipped ring advance cannot compare equal
                            cp |> plane_fill_cmd(psm, raw, h, offs[6] + slots[i] * 2 * ring + (1 - pars[i]) * ring, ring)
                        }
                        pc.row0 = uint(i)
                        dn_step_enc(raw, h, sc, pc, int64(nvh), zsig)
                    }
                } else {
                    dn_step_enc(raw, h, sc, pc, int64(nvh * nrows), zsig)
                }
            }
            r.state <- cp |> plane_run(pst)
            r.smalls <- cp |> plane_run(psm)
            r.oscales <- cp |> plane_run(psc)
            r.oquants <- cp |> plane_as(pq, type<uint>, nrows * nblk * 8)
            r.of32 <- cp |> plane_run(pof)
        }
    }
    return <- r
}

//! the dn step cells' fixture: `nrows` rows over `nslots` slots at head size `ds` - the rows' projections, every slot's state,
//! the smalls plane (eps, qscale, the rows' beta and alpha rows, a, dt, the taps, the norm row, a ring pair a slot) and the
//! offsets the push carries
struct private DnFixture {
    nrows : int
    nslots : int
    nkh : int
    nvh : int
    cd : int
    dconv : int
    eps : float
    qscale : float
    bstride : int
    beta_off : int
    g_off : int
    hist_off : int
    ring : int
    sm_n : int
    prow : int
    st_n : int
    nblk : int
    orow : int
    offs : int[8]
    projh : array<float>
    sth : array<float>
    smh : array<float>
}

def private dn_fixture(nrows, nslots, ds : int; out_f32 : bool) : DnFixture {
    var f <- DnFixture(nrows = nrows, nslots = nslots, nkh = 2, nvh = 4, dconv = 4, eps = 0.00001, qscale = 1.25, beta_off = 16)
    let kd = f.nkh * ds
    f.cd = 2 * kd + f.nvh * ds
    let taps = f.dconv - 1
    f.bstride = 2 * f.nvh
    f.g_off = f.beta_off + nrows * f.bstride
    let a_off = f.g_off + nrows * f.nvh
    let dt_off = a_off + f.nvh
    let taps_off = dt_off + f.nvh
    let wnorm_off = taps_off + f.cd * f.dconv
    f.hist_off = wnorm_off + ds
    f.ring = f.cd * taps
    f.sm_n = f.hist_off + nslots * 2 * f.ring
    f.prow = f.cd + f.nvh * ds
    f.st_n = f.nvh * ds * ds
    f.nblk = f.nvh * ds / 32
    f.orow = out_f32 ? f.nvh * ds : f.nblk * 8
    f.offs = fixed_array(f.beta_off, f.g_off, a_off, dt_off, taps_off, wnorm_off, f.hist_off, f.bstride)
    f.projh <- row_fill(nrows * f.prow, 29, 0, 0.03, 1.0, 67)
    f.sth <- row_fill(nslots * f.st_n, 13, 0, 0.02, 0.5, 53)
    f.smh <- row_fill(f.sm_n, 17, 0, 0.02, 0.4, 43)
    f.smh[0] = f.eps
    f.smh[1] = f.qscale
    for (i in range(f.nvh)) {
        f.smh[a_off + i] = -0.4 - float(i) * 0.1
    }
    return <- f
}

//! `dn_step_rows_run` over the fixture's `nrows` rows (`proj` the rows' projections, `st` the slots' states, `sm` the smalls) in `pars` / `slots`
def private dn_fixture_run(f : DnFixture; proj, st, sm : array<float>; pars, slots : array<int>; nrows : int; ds : int; out_f32 : bool; seq : bool = false) : DnRowsOut =>
    <- dn_step_rows_run(proj, st, sm, pars, slots, nrows, f.nblk, ds, f.nvh, f.nkh, f.cd, f.dconv, f.offs, f.eps, f.qscale, out_f32, seq)

//! a row's beta and alpha rows moved to row 0's place in a copy of `sm`: the one-row dispatch reads row 0
def private dn_row_smalls(sm : array<float>; r, beta_off, g_off, bstride, nvh : int) : array<float> {
    var sm1 := sm
    for (i in range(nvh)) {
        sm1[beta_off + i] = sm[beta_off + r * bstride + i]
        sm1[g_off + i] = sm[g_off + r * nvh + i]
    }
    return <- sm1
}

//! row `r`'s o row of `got` against `want`'s row 0: the elements off, the first off (the scales' first too on the q8 form),
//! and the largest f32 difference with its head and column, logged under `tag` and answered as the count off
def private dn_o_row_off(tag : string; got, want : DnRowsOut; r, nvh, ds, nblk : int; out_f32 : bool) : int {
    if (out_f32) {
        let n = nvh * ds
        let off = mismatch_exact(got.of32, want.of32, r * n, 0, n)
        let md = span_maxdiff(got.of32, want.of32, r * n, 0, n)
        to_log(LOG_INFO, "{tag}: row {r}'s o row {off} of {n} f32 elements off, the largest difference {md.diff} at head {md.at / ds} col {md.at % ds}\n")
        return off
    }
    let sc_off = mismatch_exact(got.oscales, want.oscales, r * nblk, 0, nblk)
    let q_off = mismatch_exact(got.oquants, want.oquants, r * nblk * 8, 0, nblk * 8)
    let sc_first = first_mismatch(got.oscales, want.oscales, r * nblk, 0, nblk)
    let q_first = first_mismatch(got.oquants, want.oquants, r * nblk * 8, 0, nblk * 8)
    let md = span_maxdiff(got.oscales, want.oscales, r * nblk, 0, nblk)
    to_log(LOG_INFO, "{tag}: row {r}'s o row {sc_off} of {nblk} scales off (the first at {sc_first}, the largest difference {md.diff} at block {md.at}), {q_off} of {nblk * 8} quant words off (the first at {q_first})\n")
    return sc_off + q_off
}

//! the slot state and ring pair of `got` against `want` at the same offsets: the elements off and the largest difference with its
//! row and column (a state row is `ds` wide, a ring row is a tap of `cd` channels), logged under `tag`
def private dn_state_ring_off(tag : string; got, want : DnRowsOut; st_off, st_n, hist_off, ring, cd, ds : int) : int {
    let st_diffs = mismatch_exact(got.state, want.state, st_off, st_off, st_n)
    let ring_diffs = mismatch_exact(got.smalls, want.smalls, hist_off, hist_off, 2 * ring)
    let sm = span_maxdiff(got.state, want.state, st_off, st_off, st_n)
    let rm = span_maxdiff(got.smalls, want.smalls, hist_off, hist_off, 2 * ring)
    to_log(LOG_INFO, "{tag}: the state {st_diffs} elements off (the largest difference {sm.diff} at row {sm.at / ds} col {sm.at % ds}), the two ring images {ring_diffs} elements off (the largest difference {rm.diff} at tap {rm.at / cd} channel {rm.at % cd})\n")
    return st_diffs + ring_diffs
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command")]
def private dn_step_rows_arm(t : T?; ds : int; out_f32 : bool) {
    let nrows = 2
    var f <- dn_fixture(nrows, nrows, ds, out_f32)
    var pars <- [1, 0]   //! the slots' parities differ: each row reads its own image
    var slots <- [0, 1]
    var got <- dn_fixture_run(f, f.projh, f.sth, f.smh, pars, slots, nrows, ds, out_f32)
    let tag = "dn_step_fused, {nrows} rows in two slots, ds {ds}, o as {out_f32 ? "f32" : "q8"}"
    for (r in range(nrows)) {
        var proj1 <- copy_span(f.projh, r * f.prow, f.prow)
        var sm1 <- dn_row_smalls(f.smh, r, f.beta_off, f.g_off, f.bstride, f.nvh)
        var pars1 <- [pars[r]]
        var slots1 <- [slots[r]]
        var want <- dn_fixture_run(f, proj1, f.sth, sm1, pars1, slots1, 1, ds, out_f32)
        let st_ring_off = dn_state_ring_off("{tag} row {r} in slot {slots[r]}", got, want, r * f.st_n, f.st_n, f.hist_off + r * 2 * f.ring, f.ring, f.cd, ds)
        let o_off = dn_o_row_off(tag, got, want, r, f.nvh, ds, f.nblk, out_f32)
        t |> success(st_ring_off == 0 && o_off == 0,
            "{tag}: row {r} in slot {slots[r]} - state and ring {st_ring_off}, o row {o_off} elements off the one-row dispatch")
        let other = 1 - r   //! the control: row r's reference must miss the other row's o row, so the compare can fail
        let ctrl = out_f32 ? mismatch_exact(got.of32, want.of32, other * f.nvh * ds, 0, f.nvh * ds) : mismatch_exact(got.oquants, want.oquants, other * f.nblk * 8, 0, f.nblk * 8)
        t |> success(ctrl > 0, "control: row {r}'s one-row reference misses row {other}'s o row ({ctrl} elements apart)")
        delete proj1
        delete sm1
        delete pars1
        delete slots1
        delete want
    }
    delete got
    delete f
    delete pars
    delete slots
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-verify-command")]
def private dn_step_sameslot_arm(t : T?; ds : int; out_f32 : bool) {
    let nrows = 3
    var f <- dn_fixture(nrows, 1, ds, out_f32)
    var pars <- [1, 0, 1]   //! the ring parity flips a row: each row reads the image the row before it wrote
    var slots <- [0, 0, 0]
    var got <- dn_fixture_run(f, f.projh, f.sth, f.smh, pars, slots, nrows, ds, out_f32, true)
    let tag = "dn_step_fused, {nrows} rows of one slot a dispatch a row, ds {ds}, o as {out_f32 ? "f32" : "q8"}"
    let ring1 = f.hist_off + f.ring   //! the second row's ring image: sentinel-filled between the dispatches, so its input is what the run wrote
    to_log(LOG_INFO, "{tag}: state[0] {got.state[0]} from {f.sth[0]}, the second row's ring image [0] {got.smalls[ring1]} from {f.smh[ring1]}\n")
    t |> success(got.state[0] != f.sth[0], "{tag}: the slot's state moved off its input at element 0 ({got.state[0]} from {f.sth[0]})")
    t |> success(got.smalls[ring1] != f.smh[ring1], "{tag}: the second row's ring image moved off its input at element 0 ({got.smalls[ring1]} from {f.smh[ring1]})")
    var st_r := f.sth
    var sm_r := f.smh
    for (r in range(nrows)) {
        var proj1 <- copy_span(f.projh, r * f.prow, f.prow)
        var sm1 <- dn_row_smalls(sm_r, r, f.beta_off, f.g_off, f.bstride, f.nvh)
        var pars1 <- [pars[r]]
        var slots1 <- [0]
        var want <- dn_fixture_run(f, proj1, st_r, sm1, pars1, slots1, 1, ds, out_f32)
        let o_off = dn_o_row_off(tag, got, want, r, f.nvh, ds, f.nblk, out_f32)
        t |> success(o_off == 0, "{tag}: row {r}'s o row {o_off} of {f.orow} elements off the one-row dispatch on the state row {r - 1} left")
        if (r == 1) {
            var sm0 <- dn_row_smalls(f.smh, r, f.beta_off, f.g_off, f.bstride, f.nvh)
            var cold <- dn_fixture_run(f, proj1, f.sth, sm0, pars1, slots1, 1, ds, out_f32)
            let ctrl = out_f32 ? mismatch_exact(got.of32, cold.of32, r * f.nvh * ds, 0, f.nvh * ds) : mismatch_exact(got.oquants, cold.oquants, r * f.nblk * 8, 0, f.nblk * 8)
            t |> success(ctrl > 0, "{tag} control: row 1 from the untouched state misses the sequential row 1 ({ctrl} elements apart)")
            delete sm0
            delete cold
        }
        if (r == nrows - 1) {
            let st_ring_off = dn_state_ring_off("{tag} after the last row", got, want, 0, f.st_n, f.hist_off, f.ring, f.cd, ds)
            t |> success(st_ring_off == 0, "{tag}: after the last row the state and the two ring images are the chained one-row dispatches' ({st_ring_off} elements off)")
        }
        st_r := want.state
        sm_r := want.smalls
        delete proj1
        delete sm1
        delete pars1
        delete slots1
        delete want
    }
    delete st_r
    delete sm_r
    delete got
    delete f
    delete pars
    delete slots
}

[test]
def test_vkd_dn_step_rows_sameslot(t0 : T?) {
    t0 |> run("dn_step_fused over three rows of one slot a dispatch a row == the one-row dispatch a row at a time, chained") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!dn_cells_gate(t)) return
            verify(ensure_dn_step_cls(), "the dn step class rail must engage on a device over the shared cap")
            for (ds in fixed_array(64, 128)) {
                dn_step_sameslot_arm(t, ds, false)
                dn_step_sameslot_arm(t, ds, true)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_dn_step_rows(t0 : T?) {
    t0 |> run("dn_step_fused over two rows in two region slots == the one-row dispatch a row at a time") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!dn_cells_gate(t)) return
            verify(ensure_dn_step_cls(), "the dn step class rail must engage on a device over the shared cap")
            for (ds in fixed_array(64, 128)) {
                dn_step_rows_arm(t, ds, false)
                dn_step_rows_arm(t, ds, true)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_dn_ba(t0 : T?) {
    t0 |> run("dn_ba (the prefill's f32 beta/alpha GEMM) == the CPU dot at a partial tile and past one workgroup") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            verify(ensure_dn_ba_cls(), "the dn_ba class rail must engage on a live device")
            for (npos in [21, 40]) {   // a partial 16-row tile alone; two full tiles + a partial one
                dn_ba_cell(t, "dn_ba npos {npos}", 256, 8, npos, 64, false, false)   // the layer's row base 64 in the f32 plane
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! dn_ba at one geometry against the CPU dot over the f16-rounded weight rows (the layer's `ne` rows from row base `woff`): the
//! beta outputs at 16, the alpha outputs past an 8-word gap, the plane under the sentinel and only the written positions
//! compared; the bar's control pushes the first alpha output (`poison_beta`: the first beta output); `timed` logs the dispatch
//! before its submit and times the submit, so a device hang names its kernel
def private dn_ba_cell(t : T?; tag : string; dim, nvh, npos, woff : int; poison_beta, timed : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let ne = 2 * nvh
        let ostride = nvh
        let ob_b = 16
        let ob_g = ob_b + npos * ostride + 8
        let out_n = ob_g + npos * ostride
        var wh <- hash_fill(woff + ne * dim, 0u, 1000u, 500.0, 1.0)
        var xh <- hash_fill(npos * dim, 77777u, 1000u, 500.0, 1.0)
        var want : array<float>
        want |> resize(out_n)
        var wh16 <- pack_f16(wh)   // the device keeps the rows f16: the oracle dots the rounded weights
        for (p in range(npos)) {
            for (e in range(ne)) {
                var acc = 0.0
                for (k in range(dim)) {
                    acc += float(wh16[woff + e * dim + k]) * xh[p * dim + k]
                }
                want[(e < nvh ? ob_b : ob_g) + p * ostride + e % nvh] = acc
            }
        }
        let wgs = dn_ba_wgs(int64(npos), int64(nvh))
        var got : array<float>
        with_cell_planes() $(var cp : CellPlanes) {
            let pw = cp |> in_plain(wh16)
            let px = cp |> in_(xh)
            let po = cp |> out_(out_n)
            var sc = cp |> cls_set(fixed_array(pw, px, po), @@set_dn_ba_cls)
            var pc = DnBaArgs(npos = uint(npos), dim = uint(dim), nvh = uint(nvh), woff = uint(woff), xstride = uint(dim),
                obase_b = uint(ob_b), obase_g = uint(ob_g), ostride = uint(ostride))
            if (timed) {
                cp |> cell_record_timed(tag, "{wgs} workgroups over {npos} rows x {ne} outputs x {dim}") $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_dn_ba_cls(raw, h, sc, pc, wgs)
                }
            } else {
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_dn_ba_cls(raw, h, sc, pc, wgs)
                }
            }
            delete got
            got <- cp |> plane_run(po)
        }
        let bad = ba_plane_mismatches(got, want, npos, ne, nvh, ob_b, ob_g, ostride)
        t |> success(bad == 0, "{tag}: {bad} of {npos * ne} outputs off the CPU dot")
        let at = poison_beta ? ob_b : ob_g
        var poison := got   // the bar's control: one output pushed past it must red
        poison[at] += 1.0 + 2.0 * abs(want[at])
        t |> success(ba_plane_mismatches(poison, want, npos, ne, nvh, ob_b, ob_g, ostride) > 0, "{tag}: the bar reds a poisoned output")
        delete poison
        delete wh
        delete wh16
        delete xh
        delete want
        delete got
    }
}

// the scan against the sequential CPU delta rule at one geometry: one dispatch, logged before the
// submit and timed after it, so a device hang names its kernel
def private dn_scan_cell(t : T?; tag : string; ds, nkh, nvh, npos : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        {
            let kd = nkh * ds
            let di = nvh * ds
            let cd = 2 * kd + di
            let beta_off = 16
            let g_off = beta_off + npos * nvh
            let a_off = g_off + npos * nvh
            let dt_off = a_off + nvh
            let sm_n = dt_off + nvh
            let o_off = 64   // the o rows sit past a header the kernel must leave alone
            var convh : array<float>
            convh |> resize(npos * cd)
            for (i in range(npos * cd)) {   // unit-scale q/k rows keep the 512-step recurrence bounded
                convh[i] = (float((i * 19) % 79) * 0.025 - 0.9) * 0.15
            }
            var sth <- row_fill(nvh * ds * ds, 13, 0, 0.02, 0.5, 53)
            var smh <- row_fill(sm_n, 17, 0, 0.03, 0.6, 43)
            for (i in range(nvh)) {
                smh[a_off + i] = -0.4 - float(i) * 0.1
            }
            let wgs = dn_scan_wgs(int64(nvh), int64(ds))
            var got : array<float>   //! the o rows, then the state
            var head_kept = 0
            with_cell_planes() $(var cp : CellPlanes) {
                let pconv = cp |> in_(convh)
                let pst = cp |> io_(sth)
                let psm = cp |> in_(smh)
                let po = cp |> out_(npos * di, o_off)
                var sc = cp |> cls_set(fixed_array(pconv, pst, psm, po), @@set_dn_scan_cls)
                var pcs = DnScanArgs(npos = uint(npos), cd = uint(cd), kd = uint(kd),
                    nvh = uint(nvh), nkh = uint(nkh), ds = uint(ds),
                    beta_off = uint(beta_off), g_off = uint(g_off), a_off = uint(a_off),
                    dt_off = uint(dt_off), o_off = uint(o_off), di = uint(di))
                cp |> cell_record_timed("dn_scan {tag}", "{wgs} workgroups over {npos} rows") $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_dn_scan_cls(raw, h, sc, pcs, wgs)
                }
                var whole <- cp |> plane_whole(po)
                for (i in range(o_off)) {
                    head_kept += float_bits_to_uint(whole[i]) == NAN_SENTINEL_BITS ? 1 : 0
                }
                delete got
                got <- cp |> plane_run(po)
                var st_got <- cp |> plane_run(pst)
                got |> push_from(st_got)
                delete st_got
                delete whole
            }
            t |> equal(head_kept, o_off, "dn_scan {tag}: the {o_off}-word header ahead of the o rows keeps its sentinel")
            var ref <- dn_delta_ref(convh, sth, smh, npos, nkh, nvh, ds, beta_off, g_off, a_off, dt_off)   // raw o rows: the out-norm is phase 3's
            let floor = 1e-2 * logits_maxabs(ref)
            let bad = mismatch_rel("", got, ref, 1e-2, floor)
            t |> success(bad == 0, "dn_scan {tag}: {bad} of {length(got)} o-row and state values off the sequential oracle")
            var poison := got   // the bar's control: one o value pushed past both bars must red
            poison[0] += 1.0 + 2.0 * logits_maxabs(ref)
            t |> success(mismatch_rel("", poison, ref, 1e-2, floor) > 0, "dn_scan {tag}: the bar reds a poisoned value")
            delete poison
            delete convh
            delete sth
            delete smh
            delete got
            delete ref
        }
    }
}

// the 9B-class geometry the small-model zoo never reaches: 32 value heads over ds 128 (the scan's
// 8-lane column clusters, all three shuffle stages) and a full 64-row beta/alpha tile
[test]
def test_vkd_dn_9b_scan(t0 : T?) {
    t0 |> run("dn_scan at the 9B geometry (32 heads, ds 128, 512 rows) == the sequential CPU delta rule") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!dn_cells_gate(t)) return
            verify(ensure_dn_scan_cls(), "the dn_scan class rail must engage on a live device")
            dn_scan_cell(t, "9B", 128, 16, 32, 512)
            dn_scan_cell(t, "one token", 128, 16, 32, 1)   //! the prefetch's clamp fires on the first iteration
            dn_scan_cell(t, "the whole window", 128, 16, 32, int(DN_WINDOW))   //! the gate arrays' exact bound
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// a narrow state: at ds 32 a column group spans 128 columns, so three of every four lanes are
// idle - their reads clamp to column 0 and their stores stay off; the last head's idle columns
// would otherwise read past the conv plane
[test]
def test_vkd_dn_scan_narrow(t0 : T?) {
    t0 |> run("dn_scan at a narrow state (4 heads, ds 32, 64 rows) == the sequential CPU delta rule") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!dn_cells_gate(t)) return
            verify(ensure_dn_scan_cls(), "the dn_scan class rail must engage on a live device")
            dn_scan_cell(t, "narrow", 32, 2, 4, 64)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_dn_9b_ba(t0 : T?) {
    t0 |> run("dn_ba at the 9B geometry (dim 4096, 32 heads, 512 rows) == the CPU dot") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            verify(ensure_dn_ba_cls(), "the dn_ba class rail must engage on a live device")
            dn_ba_cell(t, "dn_ba 9B geometry", 4096, 32, 512, 64 * 4096, true, true)   // the second layer's rows: the base offset is under test too
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// the decode's beta/alpha GEMV over the f16 rows: ne rows at a row base, the ne outputs at an output base
[test]
def test_vkd_router_gemv_f16(t0 : T?) {
    t0 |> run("router_gemv_f16 (the decode's f16 beta/alpha rows) == the CPU dot at a row base and an output base") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            verify(ensure_router_gemv_f16_cls(), "the router_gemv_f16 class rail must engage on a live device")
            let dim = 4096
            let ne = 32
            let woff = ne * dim   // the second layer's alpha rows
            let obase = 16
            let out_n = obase + ne + 8
            var wh <- hash_fill(woff + ne * dim, 4242u, 1000u, 500.0, 1.0)
            var xh <- hash_fill(dim, 99u, 1000u, 500.0, 1.0)
            var want : array<float>
            want |> resize(out_n)
            var wh16 <- pack_f16(wh)
            for (e in range(ne)) {
                var acc = 0.0
                for (k in range(dim)) {
                    acc += float(wh16[woff + e * dim + k]) * xh[k]
                }
                want[obase + e] = acc
            }
            //! the two-base form: the rows from ne1 on land at obase2, the four sentinel words between the halves untouched
            let ne1 = ne / 2
            let obase2 = obase + ne1 + 4
            var want2 : array<float>
            want2 |> resize(out_n)
            for (e in range(ne)) {
                want2[(e < ne1 ? obase : obase2 - ne1) + e] = want[obase + e]
            }
            //! the norm-folded form: every expert row's dot over x under the norm row at nwoff, scaled by the row's weightless rms
            let nwoff = 64
            var wnh <- hash_fill(nwoff + dim, 777u, 1000u, 1000.0, -0.5)
            let eps = 1.0e-6
            var ssn = 0.0
            for (k in range(dim)) {
                ssn += xh[k] * xh[k]
            }
            let invn = 1.0 / sqrt(ssn / float(dim) + eps)
            var want3 : array<float>
            want3 |> resize(out_n)
            for (e in range(ne)) {
                var acc = 0.0
                for (k in range(dim)) {
                    acc += float(wh16[woff + e * dim + k]) * (xh[k] * wnh[nwoff + k])
                }
                want3[obase + e] = acc * invn
            }
            with_cell_planes() $(var cp : CellPlanes) {
                let pw = cp |> in_plain(wh16)
                let px = cp |> in_(xh)
                let po = cp |> out_(out_n)
                let pn = cp |> in_(wnh)
                var sc = cp |> cls_set(fixed_array(pw, px, po, px), @@set_router_gemv_f16_cls)
                var pc = RouterArgs(ne = uint(ne), dim = uint(dim), woff = uint(woff), obase = uint(obase))
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_router_gemv_f16_cls(raw, h, sc, pc, int64(ne))
                }
                var got <- cp |> plane_run(po)
                let bad = rel_mismatches(got, want, obase, ne)
                t |> success(bad == 0, "router_gemv_f16: {bad} of {ne} outputs off the CPU dot")
                var poison := got   // the bar's control: one output pushed past it must red
                poison[obase] += 1.0 + 2.0 * abs(want[obase])
                t |> success(rel_mismatches(poison, want, obase, ne) > 0, "router_gemv_f16: the bar reds a poisoned output")
                delete poison
                cp |> plane_refill(po)
                var pc2 = RouterArgs(ne = uint(ne), dim = uint(dim), woff = uint(woff), obase = uint(obase), ne_split = uint(ne1), obase_split = uint(obase2))
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_router_gemv_f16_cls(raw, h, sc, pc2, int64(ne))
                }
                var got2 <- cp |> plane_run(po)
                let bad2 = rel_mismatches(got2, want2, obase, ne1) + rel_mismatches(got2, want2, obase2, ne1)
                var gap = 0
                for (i in range(obase + ne1, obase2)) {
                    if (float_bits_to_uint(got2[i]) != NAN_SENTINEL_BITS) {
                        gap++
                    }
                }
                t |> success(bad2 == 0 && gap == 0, "router_gemv_f16 two bases: {bad2} of {ne} outputs off the CPU dot, {gap} of 4 gap words written")
                cp |> plane_refill(po)
                var sc3 = cp |> cls_set(fixed_array(pw, px, po, pn), @@set_router_gemv_f16_cls)
                var pc3 = RouterArgs(ne = uint(ne), dim = uint(dim), woff = uint(woff), obase = uint(obase), norm_on = 1u, nwoff = uint(nwoff), eps = eps)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_router_gemv_f16_cls(raw, h, sc3, pc3, int64(ne))
                }
                var got3 <- cp |> plane_run(po)
                let bad3 = rel_mismatches(got3, want3, obase, ne)
                t |> success(bad3 == 0, "router_gemv_f16 norm-folded: {bad3} of {ne} outputs off the CPU chain")
                t |> success(rel_mismatches(got3, want, obase, ne) > 0, "router_gemv_f16 norm-folded: the control - the unnormed dot must miss")
                delete got
                delete got2
                delete got3
            }
            delete wnh
            delete want3
            delete want2
            delete wh
            delete wh16
            delete xh
            delete want
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// ===== the f16-KV-mirror instance family (the KT = float16 stamps of the codec templates) =====

// f32 -> f16 rows for upload (the session/mirror codec form)
def private pack_f16(src : array<float>) : array<float16> {
    var out : array<float16>
    out |> resize(length(src))
    for (s, o in src, out) {
        o = float16(s)
    }
    return <- out
}

// f16 readback rows -> f32 for the compares
def private unpack_f16(src : array<float16>) : array<float> {
    var out : array<float>
    out |> resize(length(src))
    for (s, o in src, out) {
        o = float(s)
    }
    return <- out
}

[test]
def test_vkd_kv16_writers(t0 : T?) {
    t0 |> run("f16-mirror writer instances == the CPU oracle (rope_kv / rope_kv_b / qkn_rope)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_rope_kv_f16_cls() && ensure_rope_kv_b_f16_cls() && ensure_qkn_rope_f16_cls())) {
                t |> skip("no Vulkan device")
                return
            }
            // --- rope_kv_f16: k/v rows land f16, the clamp a no-op at these magnitudes ---
            for (biased in fixed_array(false, true)) {
                rope_kv_arm(t, true, biased)
            }
            for (biased in fixed_array(false, true)) {
                rope_kv_b_arm(t, true, biased)
            }
            for (biased in fixed_array(false, true)) {
                qkn_rope_arm(t, true, biased)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_kv16_readers(t0 : T?) {
    t0 |> run("f16-mirror reader instances == the CPU oracle (da_attn / da_attn_b / h128)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!(da_attn_stamps_ensure(KVDtype.f16) && ensure_da_attn_b_f16_cls())) {
                t |> skip("f16 attention classes declined on this device")
                return
            }
            // --- da_attn_f16: hs 64 the scalar head, 256 the one-word lane, 512 the two-word lane ---
            codec_attn_arms(t, KVDtype.f16)
            for (hs in [64, 256, 512]) {
                codec_attn_arm(t, CodecAttn(kv = KVDtype.f16, hs = hs, nheads = 4, kv_mul = 2, g4_twin = true))
            }
            // --- da_attn_b_f16: batched window over f16 mirrors (dpl 1 and 8 arms) ---
            for (hs in [32, 256]) {
                var f <- da_attn_b_fix(13, 4, 2, hs, 32, 64, 160, 0.05, 1.8, 0.03, 1.4, true)
                var y_cls <- da_attn_b_run(f, DAB_B_F16, 0.125, 0.0, 0)
                var y_ref <- da_attn_b_ref(f, 0.125, 0, 0.0)
                check_rows(t, "da_attn_b_f16 hs {hs} against the CPU oracle", y_cls, y_ref, BAR_APPROX_CTRL, 4 * hs, hs)
                delete f
                delete y_ref
                delete y_cls
            }
            // --- da_attn_b_h128_f16: the coopmat twin over f16 mirrors ---
            if (g_gpu.has_coopmat) {
                verify(ensure_da_attn_b_h128_f16_cls(), "the h128 f16 class rail must engage on a coopmat device")
                var f <- da_attn_b_fix(45, 4, 2, 128, 32, 64, 160, 0.05, 1.8, 0.03, 1.4, true)
                var y_cls <- da_attn_b_run(f, DAB_H128_F16, 0.125, 0.0, 0)
                var y_ref <- da_attn_b_ref(f, 0.125, 0, 0.0)
                check_rows(t, "da_attn_b_h128_f16 against the CPU oracle", y_cls, y_ref, cm_bar(y_ref), 4 * 128, 128)
                delete f
                delete y_ref
                delete y_cls
            } else {
                t |> skip("no coopmat on this device - the h128 f16 arm")
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_emb_gather_pair(t0 : T?) {
    t0 |> run("embed-gather class kernels (q8 + f32) == the CPU dequant/copy") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_emb_gather_cls() || !ensure_emb_gather_f32_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let dim = 64
            let nbb = dim / 32
            let vocab = 16
            let npos = 8
            let scale = 1.25
            let wblk0 = 3              // the plane sits 3 blocks into its slab - the serving shape, never block 0
            let totblk = wblk0 + vocab * nbb
            var wqh <- hashed_words(totblk * 8, 1u, 5u)
            var wsh16 : array<float16>
            var idsh : array<uint>
            wsh16 |> resize(totblk)
            idsh |> resize(npos)
            for (i in range(totblk)) {
                wsh16[i] = float16(0.0625 + float(i % 7) * 0.03125)
            }
            for (i in range(npos)) {
                idsh[i] = uint((i * 5 + 3) % 6)   // a 6-cycle over 8 rows: tokens 3 and 2 gather twice
            }
            // f32 twin's table: a distinct pattern so a crossed binding cannot pass
            var wfh : array<float>
            wfh |> resize(vocab * dim)
            for (i in range(vocab * dim)) {
                wfh[i] = float(i % 97) * 0.125 - 6.0
            }
            var y_cls, yf_cls : array<float>
            with_cell_planes() $(var cp : CellPlanes) {
                let pwq = cp |> in_plain(wqh)
                let pws = cp |> in_plain(wsh16)
                let pid = cp |> in_plain(idsh)
                let pwf = cp |> in_(wfh)
                let px = cp |> out_(npos * dim)
                var scq = cp |> cls_set(fixed_array(pwq, pws, pid, px), @@set_emb_gather_cls)
                var scf = cp |> cls_set(fixed_array(pwf, pid, px), @@set_emb_gather_f32_cls)
                var pc = EmbArgs(npos = uint(npos), dim = uint(dim), wblk0 = uint(wblk0), embed_scale = scale)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_emb_gather_cls(raw, h, scq, pc, int64((npos * dim / 4 + 255) / 256))
                }
                delete y_cls
                y_cls <- cp |> plane_run(px)
                cp |> plane_refill(px)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_emb_gather_f32_cls(raw, h, scf, pc, int64((npos * dim / 4 + 255) / 256))
                }
                delete yf_cls
                yf_cls <- cp |> plane_run(px)
            }
            var y_ref, yf_ref : array<float>
            y_ref |> resize(npos * dim)
            yf_ref |> resize(npos * dim)
            for (r in range(npos)) {
                let tok = int(idsh[r])
                for (col in range(dim)) {
                    let ib = wblk0 + tok * nbb + col / 32
                    let by = col % 32
                    let w = wqh[ib * 8 + by / 4]
                    let q = int8(w >> uint(8 * (by % 4)))
                    y_ref[r * dim + col] = float(int(q)) * (float(wsh16[ib]) * scale)
                    yf_ref[r * dim + col] = wfh[tok * dim + col] * scale
                }
            }
            let bad = mismatch_exact(y_cls, y_ref)
            t |> success(bad == 0, "q8 embed gather matches the CPU dequant ({bad} of {npos * dim} off)")
            let badf = mismatch_exact(yf_cls, yf_ref)
            t |> success(badf == 0, "f32 embed gather matches the CPU copy ({badf} of {npos * dim} off)")
            delete wqh
            delete wsh16
            delete idsh
            delete wfh
            delete y_cls
            delete yf_cls
            delete y_ref
            delete yf_ref
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_hot_combine(t0 : T?) {
    t0 |> run("the hot expert pool's combine (the slot window past 0 over slot rows 1 .. k): the hit rows add to the host's sum, a zero weight skips its stale row") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_combine_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let n = 300   //! past one workgroup's 256 threads: the strided loop's second trip
            let k = 5
            let nhit = 3
            var miss <- hash_steps(n, 71u)
            var w : array<float>
            var down <- hash_steps(k * n, 83u)
            var ident1 : array<uint>
            w |> resize(k)
            ident1 |> resize(k)
            for (j in range(k)) {
                w[j] = j < nhit ? 0.05 + float(j) * 0.07 : 0.0
                ident1[j] = uint(j + 1)
            }
            let nan = unsafe(reinterpret<float>(0x7FC00000u))
            for (i in range(nhit * n, k * n)) {
                down[i] = nan   // a miss's row is stale: no hit chain wrote it
            }
            var y, y0 : array<float>
            with_cell_planes() $(var cp : CellPlanes) {
                let pa = cp |> io_(miss)
                let pa0 = cp |> io_(miss)
                let pid = cp |> in_plain(ident1)
                let pw = cp |> in_(w)
                let pd = cp |> in_(down)
                var sp = cp |> cls_set(fixed_array(pa, pid, pw, pd), @@set_combine_cls)
                var s0 = cp |> cls_set(fixed_array(pa0, pid, pw, pd), @@set_combine_cls)
                var pc = CombineArgs(n = uint(n), k = uint(k), r0 = 1u, r1 = uint(k) + 1u)
                var pc0 = CombineArgs(n = uint(n), k = uint(k), r0 = 0u, r1 = uint(k))   // the window at 0 starts the sum: the host's row is lost
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_combine_cls(raw, h, sp, pc, 1l)
                    enc_combine_cls(raw, h, s0, pc0, 1l)
                }
                delete y
                y <- cp |> plane_run(pa)
                delete y0
                y0 <- cp |> plane_run(pa0)
            }
            var want : array<float>
            want |> resize(n)
            for (i in range(n)) {
                var acc = miss[i]
                for (j in range(nhit)) {
                    acc += w[j] * down[j * n + i]
                }
                want[i] = acc
            }
            var finite = 0
            var lost = 0
            for (i in range(n)) {
                finite++ if (y[i] == y[i] && abs(y[i]) < 1.0e6)
                lost++ if (!(abs(y0[i] - want[i]) <= 1.0e-4))
            }
            t |> equal(finite, n, "no stale row reached the sum: a zero weight skips its row, whose NaN a product would carry")
            let bad = rel_mismatches(y, want, 0, n)
            let widest = span_maxdiff(y, want, 0, 0, n)
            t |> success(bad == 0, "the hit rows add to the host's sum under their weights ({bad} of {n} off, widest {widest.diff} at {widest.at})")
            var poison := want
            poison[n - 1] += 0.01
            t |> success(rel_mismatches(y, poison, 0, n) == 1, "control: the bar reds one poisoned element")
            t |> success(lost > n / 2, "control: a slot window at 0 starts the sum over, loses the host's row and reads a row past the hits ({lost} of {n} off)")
            delete miss
            delete w
            delete down
            delete ident1
            delete y
            delete y0
            delete want
            delete poison
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_dec_combine_pair(t0 : T?) {
    t0 |> run("decode combine class kernels (routed, routed + shared gated and ungated) == the CPU sum") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_dec_combine_cls() || !ensure_dec_combine_sh_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let n = 300
            let k = 5
            let ngate = 8
            let gidx = 6
            var xin <- hash_steps(n, 11u)
            var w : array<float>
            var down <- hash_steps(k * n, 37u)
            var sh <- hash_steps(n, 23u)
            var gate : array<float>
            w |> resize(k)
            gate |> resize(ngate)
            for (j in range(k)) {
                w[j] = 0.05 + float(j) * 0.07
            }
            for (i in range(ngate)) {
                gate[i] = float(i) * 0.5 - 2.0   // gate[gidx] = 1.0: a sigmoid off 0.5, so an ungated arm reads apart
            }
            let wgs = int64((n + 255) / 256)
            var y_r, y_s, y_u : array<float>
            with_cell_planes() $(var cp : CellPlanes) {
                let px = cp |> in_(xin)
                let pw = cp |> in_(w)
                let pd = cp |> in_(down)
                let psh = cp |> in_(sh)
                let pg = cp |> in_(gate)
                let por = cp |> out_(n)
                let pos = cp |> out_(n)
                let pou = cp |> out_(n)
                var sr = cp |> cls_set(fixed_array(px, pw, pd, por), @@set_dec_combine_cls)
                var ss = cp |> cls_set(fixed_array(px, pw, pd, psh, pg, pos), @@set_dec_combine_sh_cls)
                var su = cp |> cls_set(fixed_array(px, pw, pd, psh, pg, pou), @@set_dec_combine_sh_cls)
                var pcr = DecCombineArgs(n = uint(n), k = uint(k))
                var pcs = DecCombineArgs(n = uint(n), k = uint(k), gidx = uint(gidx), gated = 1u)
                var pcu = DecCombineArgs(n = uint(n), k = uint(k), gidx = uint(gidx), gated = 0u)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_dec_combine_cls(raw, h, sr, pcr, wgs)
                    enc_dec_combine_sh_cls(raw, h, ss, pcs, wgs)
                    enc_dec_combine_sh_cls(raw, h, su, pcu, wgs)
                }
                delete y_r
                y_r <- cp |> plane_run(por)
                delete y_s
                y_s <- cp |> plane_run(pos)
                delete y_u
                y_u <- cp |> plane_run(pou)
            }
            var r_ref, s_ref, u_ref : array<float>
            r_ref |> resize(n)
            s_ref |> resize(n)
            u_ref |> resize(n)
            let g = sigmoid_f32(gate[gidx])
            for (i in range(n)) {
                var acc = 0.0
                for (j in range(k)) {
                    acc += w[j] * down[j * n + i]
                }
                r_ref[i] = xin[i] + acc
                s_ref[i] = xin[i] + acc + g * sh[i]
                u_ref[i] = xin[i] + acc + sh[i]
            }
            // the bar: both sides sum k f32 products in the same order over operands under 1, so only the driver's fma contraction and the device exp separate them - a few ulp of 1
            let bad_r = rel_mismatches(y_r, r_ref, 0, n)
            t |> success(bad_r == 0, "routed combine matches the CPU sum ({bad_r} of {n} off)")
            let bad_s = rel_mismatches(y_s, s_ref, 0, n)
            t |> success(bad_s == 0, "routed + gated shared combine matches the CPU sum ({bad_s} of {n} off)")
            let bad_u = rel_mismatches(y_u, u_ref, 0, n)
            t |> success(bad_u == 0, "routed + ungated shared combine matches the CPU sum ({bad_u} of {n} off)")
            var moved = 0
            for (i in range(n)) {
                moved++ if (y_s[i] != y_u[i])
            }
            t |> success(moved > 0, "gate control: the gate moves the shared row's weight off 1 ({moved} of {n})")
            var r_poison := r_ref
            r_poison[n - 1] += 0.01
            t |> success(rel_mismatches(y_r, r_poison, 0, n) == 1, "routed combine control: the bar reds one poisoned element")
            var s_poison := s_ref
            s_poison[n - 1] += 0.01
            t |> success(rel_mismatches(y_s, s_poison, 0, n) == 1, "shared combine control: the bar reds one poisoned element")
            delete xin
            delete w
            delete down
            delete sh
            delete gate
            delete y_r
            delete y_s
            delete y_u
            delete r_ref
            delete s_ref
            delete u_ref
            delete r_poison
            delete s_poison
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the split-vs-fused residual cells' fixture: `nrows` rows of `dim` residual and partner, the norm row past `woff` pad words
def private ar_fused_fixture(dim, nrows, woff : int) : tuple<x : array<float>; aa : array<float>; wn : array<float>> {
    var x <- [for (i in range(nrows * dim)); 0.375 + float(i % 13) * 0.0625 - float(i % 5) * 0.125]
    var aa <- [for (i in range(nrows * dim)); float(i % 7) * 0.25 - 0.5]
    var wn <- [for (i in range(woff + dim)); 0.75 + float(i % 9) * 0.03125]
    return <- (x = x, aa = aa, wn = wn)
}

//! the split and the fused legs' residual planes: IN-PLACE, so neither can hold a sentinel - the liveness pair is their witness - then bit for bit
def private ar_fused_residual_check(t : T?; xh, x_split, x_fused : array<float>) {
    t |> success(x_split[0] != xh[0], "the split ar wrote the residual stream")
    t |> success(x_fused[0] != xh[0], "the fused ar wrote the residual stream")
    let badx = mismatch_exact(x_split, x_fused)
    t |> success(badx == 0, "fused residual update == split ({badx} of {length(xh)} off)")
}

//! the fused residual step against the split pair over six 128-wide rows, bit for bit: under `rq` the Q8_0 requant fold
//! (`cls_ar_rq_b` against `cls_ar` + `cls_dn_rq`, the quant words and the scales), else the half store (`cls_ar_f16_b`
//! against `cls_ar` + `tower_clamp_cvt_cls`, the half words); the compared planes of each leg under its own sentinel
def private ar_fused_arm(t : T?; rq : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let dim = 128
        let nrows = 6
        let woff = 64
        let nblk = nrows * (dim / 32)
        var fx <- ar_fused_fixture(dim, nrows, woff)
        let out_bytes = rq ? fixed_array(int64(nblk) * 32l, int64(nblk) * 4l) : fixed_array(int64(nrows * dim) * 2l, 0l)
        //! two DIFFERENT sentinels on the compared planes: a dead leg on either side reads as a mismatch - a float word on the quant
        //! and scale planes, a half twice over on the f16 rows
        let sent_split = rq ? float_bits_to_uint(-999.25) : packHalf2x16(float2(-999.25, -999.25))
        let sent_fused = rq ? float_bits_to_uint(-777.5) : packHalf2x16(float2(-777.5, -777.5))
        with_cell_planes() $(var cp : CellPlanes) {
            let pxs = cp |> io_(fx.x)
            let pxf = cp |> io_(fx.x)
            let paa = cp |> in_(fx.aa)
            let pwn = cp |> in_(fx.wn)
            let pyo = cp |> out_(nrows * dim)
            let pas = cp |> out_words(out_bytes[0], sent_split)
            let paf = cp |> out_words(out_bytes[0], sent_fused)
            let pbs = rq ? cp |> out_words(out_bytes[1], sent_split) : -1
            let pbf = rq ? cp |> out_words(out_bytes[1], sent_fused) : -1
            var sca = cp |> cls_set(fixed_array(pxs, paa, pwn, pyo), @@set_cls_ar)
            var scs = rq ? cp |> cls_set(fixed_array(pyo, pas, pbs), @@set_rq_cls) : cp |> cls_set(fixed_array(pyo, pas), @@set_tower_clamp_cvt_cls)
            var scf = rq ? cp |> cls_set(fixed_array(pxf, paa, pwn, paf, pbf), @@set_cls_ar_rq_b) : cp |> cls_set(fixed_array(pxf, paa, pwn, paf), @@set_cls_ar_f16_b)
            var pca = ArArgs(dim = uint(dim), add_on = 1u, woff = uint(woff), eps = 1e-5, ascale = 1.0)
            var pcr = RqArgs(inbase = 0u, nblk = uint(nblk))
            var pcc = f16_rows_args(uint(nrows * dim))
            let swgs = int64((nrows * dim / 4 + 255) / 256)
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_cls_ar(raw, h, sca, pca, int64(nrows))
                if (rq) {
                    enc_cls_dn_rq(raw, h, scs, pcr, swgs)
                    enc_cls_ar_rq_b(raw, h, scf, pca, int64(nrows))
                } else {
                    enc_tower_clamp_cvt_cls(raw, h, scs, pcc, swgs)
                    enc_cls_ar_f16_b(raw, h, scf, pca, int64(nrows))
                }
            }
            let nw = int(out_bytes[0] / 4l)
            var a_split_h <- cp |> plane_as(pas, type<uint>, nw)
            var a_fused_h <- cp |> plane_as(paf, type<uint>, nw)
            if (rq) {
                check_exact(t, "fused quant words == split", a_fused_h, a_split_h)
                var s_split_h <- cp |> plane_as(pbs, type<float>, nblk)
                var s_fused_h <- cp |> plane_as(pbf, type<float>, nblk)
                check_exact(t, "fused scales == split", s_fused_h, s_split_h)
                delete s_split_h
                delete s_fused_h
            } else {
                check_exact(t, "fused f16 rows == split", a_fused_h, a_split_h)
            }
            var x_split_h <- cp |> plane_run(pxs)
            var x_fused_h <- cp |> plane_run(pxf)
            ar_fused_residual_check(t, fx.x, x_split_h, x_fused_h)
            delete a_split_h
            delete a_fused_h
            delete x_split_h
            delete x_fused_h
        }
        delete fx
    }
}

[test]
def test_vkd_ar_rq_fused(t0 : T?) {
    t0 |> run("fused batch ar+rq == the split cls_ar + cls_dn_rq pair, bit-exact") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_cls_ar() || !ensure_cls_dn_rq() || !ensure_cls_ar_rq_b()) {
                t |> skip("no Vulkan device")
                return
            }
            ar_fused_arm(t, true)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! a GEMM pick as "column x chunks x chunk", so a miss prints all three
def private pick_str(p : tuple<tc : int64; nsplit : int64; ksplit : int64>) : string => "{p.tc}x{p.nsplit}x{p.ksplit}"

[test]
def test_vk_coopmat_default_and_tile_pick(t0 : T?) {
    t0 |> run("the resolved coopmat default ladder and the cm2 tile pick (pure functions)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!empty(g_env_vulkan.coopmat) && g_env_vulkan.coopmat != "auto") {
                t |> skip("DASLLAMA_COOPMAT is set - the resolved default ladder is not what this box runs")
                return
            }
            let saved_force = g_coopmat_mode_force
            g_coopmat_mode_force = -1
            t |> equal(resolve_coopmat_mode(true, true), 4)    // cm2 where the device has it
            t |> equal(resolve_coopmat_mode(true, false), 3)   // else mm
            t |> equal(resolve_coopmat_mode(false, false), 0)  // else sdot4
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
            t |> success(fa_khr_serves(256l, 50.0, false) && fa_khr_serves(512l, 0.0, false), "the KHR flash pick: a 256 head under the softcap, a 512 head")
            t |> success(!fa_khr_serves(128l, 50.0, false) && !fa_khr_serves(128l, 0.0, true) && !fa_khr_serves(96l, 0.0, false), "... not a capped 128 head, a gated model, or a 96 head")
            g_gpu.coopmat_mode = COOPMAT_CM2
            t |> equal(cm2_tile_cols(3072l, 128l), 128l)    // a short window never takes the half-empty l column
            t |> equal(cm2_tile_cols(3072l, 512l), 128l)    // the 3B q/down shape: m on a strict wave win (96 tiles in 3 waves, 30 units, vs 48 in 2, 32)
            t |> equal(cm2_tile_cols(8192l, 512l), 256l)    // the 3B gate/up shape: l (128 tiles in 4 waves, 64 units, vs 256 in 8, 80)
            t |> equal(cm2_tile_cols(5632l, 512l), 256l)    // the tinyllama gate shape: l (88 tiles in 3 waves, 48 units, vs 176 in 5, 50)
            t |> equal(cm2_tile_cols(768l, 32l), 32l)       // the expert-bucket shape (30B-A3B pp512: ~32 rows per region) takes s
            t |> equal(cm2_tile_cols(768l, 64l), 32l)       // ... up to the half-m edge
            t |> equal(cm2_tile_cols(768l, 65l), 128l)      // past it an m column is more than half full
            t |> equal(batch_tile_edges(int(KqFmt.iq3s4), 672l, 112l, true).tc, 128l, "the batch arm's cm2 column follows the pick on a superblock format")
            t |> equal(batch_tile_edges(int(KqFmt.iq4nl32), 2560l, 112l, true).tc, 32l, "... and is the s column on a per-32 expert plane, its one cm2 stamp")
            t |> equal(batch_tile_edges(int(KqFmt.q51), 2560l, 512l, true).tc, 32l, "... q51 too")
            t |> equal(batch_tile_edges(int(KqFmt.mx4), 2880l, 512l, true).tc, 32l, "... and mx4")
            t |> equal(cm2_tiles(512l, 512l), 16l)                             // the 1B k/v shape: four m columns of four
            t |> equal(pick_str(cm2_gemm_pick(512l, 512l, 2048l)), "256x4x512")   // ... alone it fills under half the SMs: four l chunks (32 workgroups of 8 steps, 205 units with the reduce, against 320 whole)
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

[test]
def test_vkd_ar_f16_fused(t0 : T?) {
    t0 |> run("fused batch ar+f16 == the split cls_ar + tower_clamp_cvt_cls pair, bit-exact") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_cls_ar() || !ensure_tower_clamp_cvt_cls() || !ensure_cls_ar_f16_b()) {
                t |> skip("no Vulkan device")
                return
            }
            ar_fused_arm(t, false)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// ===== the resident MoE block's routing kernels (router GEMM, per-row top-k, the bucket schedule, the gated combine) =====

let private MR_NPOS = 37
let private MR_NE = 20
let private MR_K = 3
let private MR_DIM = 2 * 64   //! two of the router tile's 64-wide K steps
let private MR_NER = MR_NE + 1   //! the shared expert's gate row rides past the experts

//! one 32-bit word to a float in [-2, 2)
def private hash_unit(i : uint) : float => float(hash_word(i) % 4096u) / 1024.0 - 2.0

//! the host select's picks and weights over the rows, `k` a row
struct TopkSelect {
    idx : array<int64>
    w : array<float>
}

//! the host select (`moe_select_core`) a row at a time over `npos` rows of `logits` at `lstride`, each row under `rbias`'s row at `rboff` when `bias_on`
def private topk_rows_select(gate : MoeGate; logits, rbias : array<float>; npos, ne, k, lstride, rboff : int; bias_on, norm : bool; wscale : float) : TopkSelect {
    var sel : TopkSelect
    static_if (typeinfo builtin_module_exists(vulkan)) {
        sel.idx |> resize(npos * k)
        sel.w |> resize(npos * k)
        var row : array<float>
        row |> resize(ne)
        for (p in range(npos)) {
            for (e in range(ne)) {
                row[e] = logits[p * lstride + e] + (bias_on ? rbias[rboff + e] : 0.0)
            }
            unsafe {
                moe_select_core(gate, int64(ne), int64(k), norm, wscale, addr(row[0]), addr(sel.idx[0]), addr(sel.w[0]), int64(p * k))
            }
        }
        delete row
    }
    return <- sel
}

//! the device's picks and weights against the host select's: the picks exact, the weights within 1e-4
def private topk_rows_compare(t : T?; tag : string; gidx : array<uint>; gw : array<float>; sel : TopkSelect) {
    var badi = 0
    for (g, w in gidx, sel.idx) {
        if (int64(g) != w) {
            badi++
        }
    }
    t |> success(badi == 0 && length(gidx) == length(sel.idx), "{tag}: the picks are the host select's ({badi} of {length(gidx)} off, {length(sel.idx)} picks selected)")
    check_rows(t, "{tag}: the weights against the host select", gw, sel.w, Bar(rel = 1e-4, poison_of_max = true, poison_abs = 0.5), FLAT_ROWS)
}

//! the router GEMM: two layers' [ner x dim] rows, the second layer's dispatched through woff; logits at the ner
//! stride; `ner` past a half-tile exercises both rows of an invocation, under it the second row's guard alone
def private moe_router_arm(t : T?; ner : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let nw = 2 * ner * MR_DIM
        var wh, xh : array<float>
        wh |> resize(nw)
        xh |> resize(MR_NPOS * MR_DIM)
        for (i in range(nw)) {
            wh[i] = hash_unit(uint(i) + 11u) * 0.25
        }
        for (i in range(MR_NPOS * MR_DIM)) {
            xh[i] = hash_unit(uint(i) + 7_777u)
        }
        var got : array<float>
        with_cell_planes() $(var cp : CellPlanes) {
            let pw = cp |> in_(wh)
            let px = cp |> in_(xh)
            let pl = cp |> out_(MR_NPOS * ner)
            var sc = cp |> cls_set(fixed_array(pw, px, pl), @@set_router_gemm_cls)
            var pc = RouterGemmArgs(npos = uint(MR_NPOS), ne = uint(ner), dim = uint(MR_DIM), woff = uint(ner * MR_DIM), ostride = uint(ner))
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_router_gemm_cls(raw, h, sc, pc, int64(((MR_NPOS + int(ROUTER_TP) - 1) / int(ROUTER_TP)) * ((ner + int(ROUTER_TE) - 1) / int(ROUTER_TE))))
            }
            delete got
            got <- cp |> plane_run(pl)
        }
        var want : array<float>
        want |> resize(MR_NPOS * ner)
        for (p in range(MR_NPOS)) {
            for (e in range(ner)) {
                var acc = 0.0
                for (c in range(MR_DIM)) {
                    acc += wh[ner * MR_DIM + e * MR_DIM + c] * xh[p * MR_DIM + c]
                }
                want[p * ner + e] = acc
            }
        }
        let bad = mismatch_rel("", got, want, 1e-4)
        t |> success(bad == 0, "router GEMM over {ner} rows matches the CPU oracle at the second layer's offset ({bad} of {MR_NPOS * ner} off)")
        want[5 * ner + 3] += 0.5
        t |> success(mismatch_rel("", got, want, 1e-4) == 1, "router GEMM control ({ner} rows): the bar reds one poisoned element")
        delete wh
        delete xh
        delete got
        delete want
    }
}

//! the per-row select against moe_select_core over the same rows: norm on with no scale, norm off with a scale
//! `gate` and `rbias` are the CPU select's own form (gpt-oss: softmax_weight, a router bias before the pick), which the kernel serves on its renormalized-softmax arm
def private moe_topk_rows_arm(t : T?; norm : bool; wscale : float; gate : MoeGate; rbias : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var lh, rb : array<float>
        lh |> resize(MR_NPOS * MR_NER)
        rb |> resize(2 * MR_NE)   //! two layers' bias rows: the dispatch reads the second (rboff = MR_NE), the first is a decoy
        for (i in range(MR_NPOS * MR_NER)) {
            lh[i] = hash_unit(uint(i) * 3u + 101u) * 2.0 + float(i % 7) * 0.0625   //! spread past any near-tie
        }
        for (e in range(MR_NE)) {
            rb[e] = 7.0
            rb[MR_NE + e] = rbias ? hash_unit(uint(e) * 5u + 7u) * 3.0 - 1.5 : 0.0   //! a bias wide enough to move picks
        }
        var gidx : array<uint>
        var gw : array<float>
        with_cell_planes() $(var cp : CellPlanes) {
            let pl = cp |> in_(lh)
            let prb = cp |> in_(rb)
            let pw = cp |> out_(MR_NPOS * MR_K)
            let pidx = cp |> out_(MR_NPOS * MR_K)
            var sc = cp |> cls_set(fixed_array(pl, pw, pl, prb, pidx), @@set_topk_rows_cls)   //! no down scale: the logits fill the ds binding, never read at NO_DSCALE
            var pc = TopkArgs(ne = uint(MR_NE), k = uint(MR_K), norm = norm || gate == MoeGate.softmax_weight ? 1u : 0u, lstride = uint(MR_NER), wscale = wscale, dsoff = NO_DSCALE,
                rbias_on = rbias ? 1u : 0u, rboff = uint(MR_NE))
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_topk_rows_cls(raw, h, sc, pc, int64(MR_NPOS))
            }
            delete gidx
            gidx <- cp |> plane_as(pidx, type<uint>, MR_NPOS * MR_K)
            delete gw
            gw <- cp |> plane_run(pw)
        }
        var sel <- topk_rows_select(gate, lh, rb, MR_NPOS, MR_NE, MR_K, MR_NER, MR_NE, true, norm, wscale)
        let arm = (gate == MoeGate.softmax_weight ? "softmax over the picks" : (norm ? "renormalized" : "scaled by {wscale}")) + (rbias ? ", router bias" : "")
        topk_rows_compare(t, "per-row top-k ({arm})", gidx, gw, sel)
        sel.w[4] += 0.5
        t |> success(mismatch_rel("", gw, sel.w, 1e-4) == 1, "per-row top-k control: the bar reds one poisoned weight")
        if (rbias) {   //! the bias control: the select over the unbiased logits must pick differently somewhere
            var sel0 <- topk_rows_select(gate, lh, rb, MR_NPOS, MR_NE, MR_K, MR_NER, MR_NE, false, norm, wscale)
            var moved = 0
            for (i0, i in sel0.idx, sel.idx) {
                if (i0 != i) {
                    moved++
                }
            }
            t |> success(moved > 0, "router bias control: the bias moves {moved} of {MR_NPOS * MR_K} picks off the unbiased select")
            delete sel0
        }
        delete sel
        delete rb
        delete lh
        delete gidx
        delete gw
    }
}

//! the bucket schedule against its CPU twin, word for word over the whole planes (three empty experts, a sentinel fill)
def private moe_sched_arm(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let nk = 4 * MR_NPOS * MR_K
        let wt_gu = 3
        let wt_dn = 2
        let sh = sched_shape(nk, MR_NE, wt_gu, wt_dn)
        var idx : array<uint>
        idx |> resize(nk)
        //! the ladder's four shapes: expert 0 a whole m column plus an s remainder, 1 and 2 one partial
        //! m column each, 3 within the s column; the rest spread thin, the last three route nothing
        let c0 = int(SCHED_M_ROWS) + int(SCHED_S_ROWS) - 10
        let c1 = int(SCHED_M_ROWS) - 28
        let c2 = int(SCHED_S_ROWS) + 8
        let c3 = int(SCHED_S_ROWS) - 12
        for (r in range(nk)) {
            idx[r] = r < c0 ? 0u : (r < c0 + c1 ? 1u : (r < c0 + c1 + c2 ? 2u : (r < c0 + c1 + c2 + c3 ? 3u : 4u + hash_word(uint(r) + 55u) % uint(MR_NE - 7))))
        }
        let bases = fixed_array(1_000u, 96u, 2_000u, 96u, 3_000u, 48u)
        var winv, w1, w3, w2 : array<uint>
        sched_oracle(idx, MR_NE, wt_gu, wt_dn, sh, bases, winv, w1, w3, w2)
        let s_bytes = long_length(w1) * 4l
        var ginv, g1, g3, g2, gbex : array<uint>
        with_cell_planes() $(var cp : CellPlanes) {
            let pidx = cp |> in_plain(idx)
            let pinv = cp |> out_(nk)
            let p1 = cp |> out_words(s_bytes, MR_SENT)
            let p3 = cp |> out_words(s_bytes, MR_SENT)
            let p2 = cp |> out_words(s_bytes, MR_SENT)
            let pbex = cp |> out_(nk)
            var sc = cp |> cls_set(fixed_array(pidx, pinv, p1, p3, p2, pbex), @@set_moe_sched_cls)
            var pc = SchedArgs(nk = uint(nk), ne = uint(MR_NE), wt_gu = uint(wt_gu), wt_dn = uint(wt_dn),
                map_off = uint(sh.map_off), map_off_mgu = uint(sh.map_off_mgu), map_off_mdn = uint(sh.map_off_mdn),
                bound_gu = uint(sh.bound_gu), bound_dn = uint(sh.bound_dn), bound_mgu = uint(sh.bound_mgu), bound_mdn = uint(sh.bound_mdn),
                base1 = bases[0], stride1 = bases[1], base3 = bases[2], stride3 = bases[3], base2 = bases[4], stride2 = bases[5])
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_moe_sched_cls(raw, h, sc, pc, 1l)
            }
            delete ginv
            ginv <- cp |> plane_as(pinv, type<uint>, nk)
            delete g1
            g1 <- cp |> plane_as(p1, type<uint>, length(w1))
            delete g3
            g3 <- cp |> plane_as(p3, type<uint>, length(w1))
            delete g2
            g2 <- cp |> plane_as(p2, type<uint>, length(w1))
            delete gbex
            gbex <- cp |> plane_as(pbex, type<uint>, nk)
        }
        //! the atomic cursor fixes no order within a bucket, so the check is per bucket: a permutation of its rows
        var inv_bad = 0
        var bex_bad = 0
        var seen : array<bool>
        seen |> resize(nk)
        for (r in range(nk)) {
            let e = int(idx[r])
            let base = uint(bucket_start(idx, e))
            let rows = uint(bucket_rows(idx, e))
            let row = ginv[r]
            if (row < base || row >= base + rows || seen[int(row)]) {
                inv_bad++
            } else {
                seen[int(row)] = true
                if (gbex[int(row)] != idx[r]) {
                    bex_bad++
                }
            }
        }
        t |> success(inv_bad == 0, "the schedule's slot -> bucket row map is a permutation of each bucket's rows ({nk} slots, {inv_bad} off)")
        t |> success(bex_bad == 0, "the schedule's bucket row -> expert map names each row's slot's expert ({bex_bad} of {nk} off)")
        delete seen
        delete gbex
        t |> success(mismatch_qbytes(g1, w1, 0) == 0, "the gate plane's s and m records, maps and sentinel tails match the CPU twin word for word ({length(w1)} words)")
        t |> success(mismatch_qbytes(g3, w3, 0) == 0, "the up plane's schedule matches")
        t |> success(mismatch_qbytes(g2, w2, 0) == 0, "the down plane's schedule matches (its own tile counts and tails)")
        w2[sh.map_off_mdn + sh.bound_mdn - 1] = 0u
        t |> success(mismatch_qbytes(g2, w2, 0) > 0, "schedule control: the m map's last sentinel word flipped reds the compare")
        delete idx
        delete winv
        delete w1
        delete w3
        delete w2
        delete ginv
        delete g1
        delete g3
        delete g2
    }
}

//! the residual step with the combine folded in: x += [g *] shared row + the k weighted expert rows
//! through the slot map, then the next norm - the f32 and the f16 normed-row forms against one CPU
//! oracle; `with_sh` = the layer has a shared expert (its rows ride the add partner)
//! `dbias` = each slot's row takes its expert's down bias before its weight (gpt-oss), the slot -> expert map from a six-expert plane
def private moe_ar_comb_arm(t : T?; gated, with_sh, dbias : bool) {   // nolint:STYLE037 — one arm: the fixture, both forms' dispatches, the oracle, the compares
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let n = 64
        let k = dbias ? 12 : 3   //! twelve slots on the biased arm: the row pass's eight-slot register form and its tail past them
        let npos = 5
        let nrows = npos * k
        let ne = 6
        let eps = 1.0e-6
        var fx <- ar_comb_fixture(n, k, npos)
        var wn, lg : array<float>
        var sex : array<uint>
        var db : array<float>
        wn |> resize(n)
        lg |> resize(npos * MR_NER)
        sex |> resize(nrows)
        let dbn = dbias ? 2 * ne * n : n   //! two layers' rows when biased (the dispatch reads the second), the biasless zero row of the model dim otherwise - the driver's own shapes
        db |> resize(dbn)
        for (i in range(nrows)) {
            sex[i] = hash_word(uint(i) + 3u) % uint(ne)
        }
        for (i in range(dbn)) {
            db[i] = dbias ? (i < ne * n ? 9.0 : float((i * 19) % 23) * 0.25 - 2.75) : 0.0   //! the first layer's rows are a decoy
        }
        for (i in range(n)) {
            wn[i] = 0.5 + float(i % 7) * 0.125
        }
        for (i in range(npos * MR_NER)) {
            lg[i] = hash_unit(uint(i) + 9u)
        }
        var got_x, got_y : array<float>
        var got_h : array<uint>
        var got_q : array<int8>
        var got_s : array<float>
        var pc = ArArgs(dim = uint(n), add_on = with_sh ? 1u : 0u, woff = 0u, eps = eps, ascale = 1.0, row0 = 0u,
            slots = uint(k), lstride = uint(MR_NER), gated = gated ? 1u : 0u, dbias_on = dbias ? 1u : 0u, dboff = dbias ? uint(ne * n) : 0u)
        with_cell_planes() $(var cp : CellPlanes) {
            let px = cp |> io_(fx.xs)
            let psh = cp |> in_(fx.sh)
            let pwn = cp |> in_(wn)
            let pyo = cp |> out_(npos * n)
            let psi = cp |> in_plain(fx.sidx)
            let psw = cp |> in_(fx.sw)
            let pro = cp |> in_(fx.rows)
            let plg = cp |> in_(lg)
            let pdb = cp |> in_(db)
            let psex = cp |> in_plain(sex)
            let pyh = cp |> out_words(int64(npos * n) * 2l, NAN_SENTINEL_BITS)
            let poq = cp |> out_words(int64(npos * n), NAN_SENTINEL_BITS)
            let pos = cp |> out_(npos * n / 32)
            //! the f32 form: x updated in place, the normed row to yo
            var sc = cp |> cls_set(fixed_array(px, psh, pwn, pyo, psi, psw, pro, plg, pdb, psex), @@set_cls_ar_comb)
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_cls_ar_comb(raw, h, sc, pc, int64(npos))
            }
            delete got_x
            got_x <- cp |> plane_run(px)
            delete got_y
            got_y <- cp |> plane_run(pyo)
            //! the f16 form over a fresh x
            cp |> plane_upload(px, fx.xs)
            var sc16 = cp |> cls_set(fixed_array(px, psh, pwn, pyh, psi, psw, pro, plg, pdb, psex), @@set_cls_ar_comb_f16_b)
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_cls_ar_comb_f16_b(raw, h, sc16, pc, int64(npos))
            }
            delete got_h
            got_h <- cp |> plane_as(pyh, type<uint>, npos * n / 2)
            //! the quantizing twin over a fresh x: the normed rows as Q8_0 blocks at the row binding, the scales past the slot map
            cp |> plane_upload(px, fx.xs)
            var scq = cp |> cls_set(fixed_array(px, psh, pwn, poq, psi, psw, pro, plg, pdb, psex, pos), @@set_cls_ar_comb_rq)
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_cls_ar_comb_rq(raw, h, scq, pc, int64(npos))
            }
            delete got_q
            got_q <- cp |> plane_as(poq, type<int8>, npos * n)
            delete got_s
            got_s <- cp |> plane_run(pos)
        }
        var want_x, want_y : array<float>
        want_x |> resize(npos * n)
        want_y |> resize(npos * n)
        for (p in range(npos)) {
            let g = with_sh && gated ? sigmoid_f32(lg[p * MR_NER + MR_NE]) : 1.0
            for (i in range(n)) {
                var v = fx.xs[p * n + i]
                if (with_sh) {
                    v += g * fx.sh[p * n + i]
                }
                for (j in range(k)) {
                    v += fx.sw[p * k + j] * (fx.rows[int(fx.sidx[p * k + j]) * n + i] + (dbias ? db[ne * n + int(sex[p * k + j]) * n + i] : 0.0))
                }
                want_x[p * n + i] = v
            }
            unsafe {
                rmsnorm(addr<float?>(want_y[p * n]), addr<float const?>(want_x[p * n]), addr<float const?>(wn[0]), int64(n), eps)
            }
        }
        let arm = (with_sh ? (gated ? "gated shared" : "ungated shared") : "no shared") + " expert" + (dbias ? ", down biases" : "")
        //! the quantizing twin: every block's scale within 2e-3 of the oracle's amax over 127, every dequantized element within a quant step
        let off = q8_blocks_off(got_q, got_s, want_y, npos * n / 32)
        t |> success(off.scales == 0, "the combine-folded residual step ({arm}): the quantizing twin's block scales match the oracle ({off.scales} of {npos * n / 32} off)")
        t |> success(off.values == 0, "the combine-folded residual step ({arm}): the quantizing twin's rows sit within a quant step of the oracle ({off.values} of {npos * n} off)")
        t |> success(off.poison_inside == 0, "combine-folded residual control: the quant bar reds a poisoned oracle element in every block ({off.poison_inside} passed)")
        ar_comb_check(t, "the combine-folded residual step ({arm})", "combine-folded residual control", fx.xs, got_x, got_y, got_h, want_x, want_y, n)
        delete fx
        delete wn
        delete lg
        delete got_x
        delete got_y
        delete got_h
        delete got_q
        delete got_s
        delete want_x
        delete want_y
        delete sex
        delete db
    }
}

//! the combine-folded residual cells' fixture rows: a permutation of the `npos` x `k` bucket rows as the slot map, the slot
//! weights, the bucket rows `n` wide, the residual rows and the shared-expert rows
struct private ArCombFix {
    sidx : array<uint>
    sw : array<float>
    rows : array<float>
    xs : array<float>
    sh : array<float>
}

def private ar_comb_fixture(n, k, npos : int) : ArCombFix {
    let nrows = npos * k
    return <- ArCombFix(sidx <- [for (i in range(nrows)); uint((i * 7) % nrows)],
        sw <- [for (i in range(nrows)); 0.125 + float(i % 5) * 0.25],
        rows <- row_fill(nrows * n, 7, 0, 0.13, 5.5, 93),
        xs <- row_fill(npos * n, 11, 0, 0.1, 1.7, 37),
        sh <- row_fill(npos * n, 5, 0, 0.2, 3.0, 41))
}

//! a combine-folded residual step's three outputs under `tag`: the in-place residual rows moved off their input `xs` and within
//! the approx bar of `want_x`, the f32 normed rows of `want_y`, and the f16 normed rows `got_h` within the half's rounding - a
//! half rounds to 2^-11 of the value, the bar four halves of that with the same fraction of the row's largest value as the floor
//! the near-zero elements read; then, under `ctrl`, one poisoned element of each expectation reds its bar
def private ar_comb_check(t : T?; tag, ctrl : string; xs, got_x, got_y : array<float>; got_h; var want_x, want_y : array<float>; n : int) {
    t |> success(mismatch_approx(got_x, xs) > 0, "{tag}: the in-place residual rows moved off their input")
    check(t, "{tag}: the residual rows match the CPU oracle", got_x, want_x, BAR_APPROX, Shape(row_width = n))
    check(t, "{tag}: the f32 normed rows match the CPU oracle", got_y, want_y, BAR_APPROX, Shape(row_width = n))
    var got_yf <- [for (i in range(length(want_y))); half_at(got_h, i)]
    let f16_bar = 2.0e-3
    check(t, "{tag}: the f16 normed rows within the half's rounding of the oracle", got_yf, want_y, Bar(rel = f16_bar, abs_floor = f16_bar * logits_maxabs(want_y)), Shape(row_width = n))
    want_x[2 * n + 9] += 0.5
    t |> success(mismatch_approx(got_x, want_x) == 1, "{ctrl}: the residual bar reds one poisoned element")
    want_y[3 * n + 5] += 0.5
    t |> success(mismatch_approx(got_y, want_y) == 1, "{ctrl}: the f32 normed-row bar reds one poisoned element")
    t |> success(mismatch_rel("", got_yf, want_y, f16_bar, f16_bar * logits_maxabs(want_y)) == 1, "{ctrl}: the f16 normed-row bar reds one poisoned element")
    delete got_yf
}

//! the gemma-4 combine: x = (x + post(post2(sum_j w_j d_j) + post1(dense))) * ascale, then the next norm -
//! the routed sum and the dense row each under their own post norm, their sum under the layer's, the step
//! scaled by the output scale; the f32 and f16 normed-row forms against one CPU oracle
def private moe_ar_comb_g4_arm(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let n = 64
        let k = 8   //! gemma-4's eight routed slots: the row pass's register form full
        let npos = 5
        let eps = 1.0e-6
        let ascale = 0.75
        var fx <- ar_comb_fixture(n, k, npos)
        var wn : array<float>   //! four norm rows: the next norm, the dense post norm, the routed post norm, the layer's post norm
        wn |> resize(4 * n)
        for (i in range(4 * n)) {
            wn[i] = 0.5 + float((i * 3) % 7) * 0.125
        }
        let nblk = npos * n / 32
        let rq = ensure_cls_ar_comb_g4_rq()
        var got_x, got_y : array<float>
        var got_h : array<uint>
        var oq : array<int8>
        var os : array<float>
        var pc = ArArgs(dim = uint(n), add_on = 1u, woff = 0u, eps = eps, ascale = ascale, row0 = 0u,
            slots = uint(k), pwoff = uint(n), routed_post_off = uint(2 * n), sum_post_off = uint(3 * n))
        with_cell_planes() $(var cp : CellPlanes) {
            let px = cp |> io_(fx.xs)
            let psh = cp |> in_(fx.sh)
            let pwn = cp |> in_(wn)
            let pyo = cp |> out_(npos * n)
            let psi = cp |> in_plain(fx.sidx)
            let psw = cp |> in_(fx.sw)
            let pro = cp |> in_(fx.rows)
            let pyh = cp |> out_words(int64(npos * n) * 2l, NAN_SENTINEL_BITS)
            let poq = cp |> out_words(int64(npos * n), NAN_SENTINEL_BITS)
            let pos = cp |> out_(nblk)
            var sc = cp |> cls_set(fixed_array(px, psh, pwn, pyo, psi, psw, pro), @@set_cls_ar_comb_g4)
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_cls_ar_comb_g4(raw, h, sc, pc, int64(npos))
            }
            delete got_x
            got_x <- cp |> plane_run(px)
            delete got_y
            got_y <- cp |> plane_run(pyo)
            cp |> plane_upload(px, fx.xs)
            var sc16 = cp |> cls_set(fixed_array(px, psh, pwn, pyh, psi, psw, pro), @@set_cls_ar_comb_g4_f16_b)
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_cls_ar_comb_g4_f16_b(raw, h, sc16, pc, int64(npos))
            }
            delete got_h
            got_h <- cp |> plane_as(pyh, type<uint>, npos * n / 2)
            //! the quantizing twin: the same step, the normed rows as Q8_0 blocks (two a row)
            if (rq) {
                cp |> plane_upload(px, fx.xs)
                var scq = cp |> cls_set(fixed_array(px, psh, pwn, poq, psi, psw, pro, pos), @@set_cls_ar_comb_g4_rq)
                cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                    enc_cls_ar_comb_g4_rq(raw, h, scq, pc, int64(npos))
                }
                delete oq
                oq <- cp |> plane_as(poq, type<int8>, npos * n)
                delete os
                os <- cp |> plane_run(pos)
            }
        }
        var want_x, want_y, r, c : array<float>
        want_x |> resize(npos * n)
        want_y |> resize(npos * n)
        r |> resize(n)
        c |> resize(n)
        for (p in range(npos)) {
            var sr = 0.0
            var sd = 0.0
            for (i in range(n)) {
                var v = 0.0
                for (j in range(k)) {
                    v += fx.sw[p * k + j] * fx.rows[int(fx.sidx[p * k + j]) * n + i]
                }
                r[i] = v
                sr += v * v
                sd += fx.sh[p * n + i] * fx.sh[p * n + i]
            }
            let inv_r = 1.0 / sqrt(sr / float(n) + eps)
            let inv_d = 1.0 / sqrt(sd / float(n) + eps)
            var sc = 0.0
            for (i in range(n)) {
                c[i] = wn[2 * n + i] * (r[i] * inv_r) + wn[n + i] * (fx.sh[p * n + i] * inv_d)
                sc += c[i] * c[i]
            }
            let inv_c = 1.0 / sqrt(sc / float(n) + eps)
            for (i in range(n)) {
                want_x[p * n + i] = (fx.xs[p * n + i] + wn[3 * n + i] * (c[i] * inv_c)) * ascale
            }
            unsafe {
                rmsnorm(addr<float?>(want_y[p * n]), addr<float const?>(want_x[p * n]), addr<float const?>(wn[0]), int64(n), eps)
            }
        }
        //! the quantizing twin held to the oracle's blocks
        if (rq) {
            let off = q8_blocks_off(oq, os, want_y, nblk)
            t |> success(off.scales == 0, "the gemma-4 quantizing combine: the block scales match the oracle's normed rows ({off.scales} of {nblk} off)")
            t |> success(off.values == 0, "the gemma-4 quantizing combine: the dequantized rows sit within a quant step of the oracle ({off.values} of {npos * n} off)")
            t |> success(off.poison_inside == 0, "gemma-4 quantizing combine control: the quant bar reds a poisoned oracle element in every block ({off.poison_inside} passed)")
        }
        ar_comb_check(t, "the gemma-4 combine step", "gemma-4 combine control", fx.xs, got_x, got_y, got_h, want_x, want_y, n)
        delete fx
        delete wn
        delete got_x
        delete got_y
        delete got_h
        delete oq
        delete os
        delete want_x
        delete want_y
        delete r
        delete c
    }
}

//! the N-row command's top-k against the one-row top-k run a row at a time: the picks, the weights and the region
//! records bit for bit, the rows' feed blocks where the one-row form writes row zero's - and the picks against the host
//! select, the same oracle the window's per-row form holds to; `gptoss_router_bias` takes the biased pick
def private topk_n_arm(t : T?; gptoss_router_bias : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let npos = 4
        let ne = 40
        let k = 6
        let lstride = 48
        let xnb1 = 7u
        let xnb2 = 5u
        var lh, rb : array<float>
        lh |> resize(npos * lstride)
        rb |> resize(2 * ne)
        for (i in range(npos * lstride)) {
            lh[i] = hash_unit(uint(i) * 3u + 211u) * 2.0 + float(i % 7) * 0.0625   //! spread past any near-tie
        }
        for (e in range(ne)) {
            rb[e] = 7.0
            rb[ne + e] = gptoss_router_bias ? hash_unit(uint(e) * 5u + 7u) * 3.0 - 1.5 : 0.0
        }
        let l_bytes = int64(npos * lstride) * 4l
        let l1_bytes = int64(lstride) * 4l
        let rb_bytes = int64(2 * ne) * 4l
        let ds_bytes = 16l
        let ld = make_device_buf(l_bytes)
        let ld1 = make_device_buf(l1_bytes)
        let dsd = make_device_buf(ds_bytes)
        let rbd = make_device_buf(rb_bytes)
        let gbits = fixed_array(1u, 2u, 4u, 8u, 16u, 0u, 0u, 32u)
        var pc = TopkArgs(ne = uint(ne), k = uint(k), norm = 1u, wscale = 0.0, dsoff = NO_DSCALE, rbias_on = gptoss_router_bias ? 1u : 0u, rboff = uint(ne),
            base1 = 100u, stride1 = 1000u, base3 = 200u, stride3 = 1000u, base2 = 300u, stride2 = 2000u, xnb2 = xnb2,
            lstride = uint(lstride), xnb1 = xnb1)
        var pc1 = pc
        pc1.lstride = 0u
        pc1.xnb1 = 0u
        unsafe {
            upload_region_at(ld, 0l, addr<uint8 const?>(lh[0]), l_bytes)
            upload_region_at(rbd, 0l, addr<uint8 const?>(rb[0]), rb_bytes)
            nan_sentinel_upload(dsd, ds_bytes)
        }
        let arm = gptoss_router_bias ? "router bias" : "plain"
        let tag = "topk_n ({arm})"
        var planes <- nform_cols_cell(t, tag, [npos], npos,
            [NformPlane(name = "the w1 slot blocks", words = 2 * k, cmp = false), NformPlane(name = "the w3 slot blocks", words = 2 * k, cmp = false),
             NformPlane(name = "the w2 slot blocks", words = 2 * k, cmp = false), NformPlane(name = "the weights", words = k), NformPlane(name = "the picks", words = k)],
            2u | 4u | 8u | 16u | 32u,
            $(p : int; raw : VkCommandBuffer; var h : VkHaz; o : NformOuts) {
                upload_region_at(ld1, 0l, unsafe(addr<uint8 const?>(lh[p * lstride])), l1_bytes)
                var sc1 = set_topk_n_cls(fixed_array(ld1, o.bufs[0], o.bufs[1], o.bufs[2], o.bufs[3], dsd, rbd, o.bufs[4]),
                    fixed_array(l1_bytes, o.bytes[0], o.bytes[1], o.bytes[2], o.bytes[3], ds_bytes, rb_bytes, o.bytes[4]), gbits)
                enc_topk_n_cls(raw, h, sc1, pc1, 1l)
            },
            $(nrows : int; raw : VkCommandBuffer; var h : VkHaz; o : NformOuts) {
                var scN = set_topk_n_cls(fixed_array(ld, o.bufs[0], o.bufs[1], o.bufs[2], o.bufs[3], dsd, rbd, o.bufs[4]),
                    fixed_array(l_bytes, o.bytes[0], o.bytes[1], o.bytes[2], o.bytes[3], ds_bytes, rb_bytes, o.bytes[4]), gbits)
                enc_topk_n_cls(raw, h, scN, pc, int64(nrows))
            })
        var bad = 0
        var badx = 0
        for (p in range(npos)) {
            for (j in range(k)) {
                let s = p * k + j
                if (planes.nf[0][s * 2] != planes.one[0][s * 2] || planes.nf[1][s * 2] != planes.one[1][s * 2] || planes.nf[2][s * 2] != planes.one[2][s * 2]) {
                    bad++
                }
                if (planes.nf[0][s * 2 + 1] != uint(p) * xnb1 || planes.nf[1][s * 2 + 1] != uint(p) * xnb1 || planes.nf[2][s * 2 + 1] != uint(s) * xnb2
                        || planes.one[0][s * 2 + 1] != 0u || planes.one[1][s * 2 + 1] != 0u || planes.one[2][s * 2 + 1] != uint(j) * xnb2) {
                    badx++
                }
            }
        }
        t |> success(bad == 0, "{tag}: the rows' weight blocks == the one-row top-k a row at a time ({bad} of {npos * k} slots off)")
        t |> success(badx == 0, "{tag}: every slot's feed blocks are its row's, the one-row form's row zero's ({badx} of {npos * k} slots off)")
        let live = npos * k   //! the N plane holds one row past the extent (the sentinel slot): the host select covers the live rows
        var gw <- [for (i in range(live)); uint_bits_to_float(planes.nf[3][i])]
        var gidx <- [for (i in range(live)); planes.nf[4][i]]
        var sel <- topk_rows_select(MoeGate.softmax, lh, rb, npos, ne, k, lstride, ne, true, true, 0.0)
        topk_rows_compare(t, tag, gidx, gw, sel)
        delete sel
        delete gidx
        delete gw
        delete planes
        delete lh
        delete rb
    }
}

[test]
def test_vkd_topk_n(t0 : T?) {
    t0 |> run("the N-row command's top-k == the one-row top-k a row at a time, bit for bit, and the host select") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_topk_n_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            topk_n_arm(t, gptoss_router_bias = false)
            topk_n_arm(t, gptoss_router_bias = true)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the router GEMV's descriptor set on the stamp `f16` names
def private router_set(f16 : bool; bufs; sizes) => f16 ? set_router_gemv_f16_cls(bufs, sizes, fixed_array(0u, 1u, 2u, 0u)) : set_router_gemv_cls(bufs, sizes, fixed_array(0u, 1u, 2u, 0u))

//! the router GEMV recorded on the stamp `f16` names
def private router_enc(f16 : bool; raw; var h; var sc; var pc; groups : int64) {
    if (f16) {
        enc_router_gemv_f16_cls(raw, h, sc, pc, groups)
    } else {
        enc_router_gemv_cls(raw, h, sc, pc, groups)
    }
}

//! the CPU oracle: each row's dot with every expert row, the row under its rms and the norm row where the fused norm is on;
//! the bar is the sum's rounding over dim terms of the same order, the envelope scaling it with the row
def private router_cols_oracle(t : T?; arm : string; got, xh, nh, wdev : array<float>; ncols, ne, dim, obase : int; norm_on : bool; eps : float) {
    let xn <- [for (i in range(ncols * dim)); norm_on ? xh[i] * nh[i % dim] : xh[i]]
    var ref <- dense_ref_rows(wdev, xn, ncols, dim, ne)
    for (c in range(ncols)) {
        var ss = 0.0
        for (i in range(dim)) {
            ss += xh[c * dim + i] * xh[c * dim + i]
        }
        let inv = norm_on ? 1.0 / sqrt(ss / float(dim) + eps) : 1.0
        for (e in range(c * ne, (c + 1) * ne)) {
            ref.want[e] *= inv
            ref.env[e] *= inv
        }
    }
    check(t, "{arm}: the logits vs the CPU oracle", copy_span(got, obase, ncols * ne), ref.want, Bar(kind = BarKind.envelope, rel = 1e-4, abs_floor = 1e-6),
        Shape(row_width = ne), ref.env)
    delete ref
}

//! the router GEMV over `ncols` x rows against the one-row dispatch a row at a time (word for word through `nform_cols_cell`:
//! one kernel body, the same order) and against a CPU oracle of the same dot - the f16 stamp's weights rounded as the device
//! reads them, and under `norm_on` each row's rms under a norm row, as the gemma-4 MoE router reads its input
def private router_cols_arm(t : T?; ncols : int; norm_on, f16 : bool) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let stamp = f16 ? "f16" : "f32"
        let form = "router_gemv {stamp}{norm_on ? " under the fused rms" : ""}"
        let arm = "{form} over {ncols} columns"
        if (f16 ? !ensure_router_gemv_f16_cls() : !ensure_router_gemv_cls()) {
            t |> skip("{arm}: the class rail declined on this device")
            return
        }
        let dim = 2048
        let ne = 24
        let obase = 8
        let ncol_max = 8
        var wh <- hash_fill(ne * dim, 777u, 1000u, 500.0, 1.0)
        var xh <- hash_fill(ncol_max * dim, 31u, 1000u, 500.0, 1.0)
        var nh <- hash_fill(dim, 9u, 1000u, 1000.0, -0.5)
        var wh16 <- pack_f16(wh)
        var wdev : array<float>   //! the weights as the device reads them: the f16 stamp's rounded, the f32 stamp's as given
        wdev |> resize(ne * dim)
        for (i in range(ne * dim)) {
            wdev[i] = f16 ? float(wh16[i]) : wh[i]
        }
        let w_bytes = f16 ? long_length(wh16) * 2l : int64(ne * dim) * 4l
        let x_bytes = int64(ncols * dim) * 4l
        let x1_bytes = int64(dim) * 4l
        let n_bytes = int64(dim) * 4l
        let wd = make_device_buf(w_bytes)
        let xd = make_device_buf(x_bytes)
        let xd1 = make_device_buf(x1_bytes)
        let nd = make_device_buf(n_bytes)
        let eps = 1e-6
        let non = norm_on ? 1u : 0u
        unsafe {
            if (f16) {
                upload_region_at(wd, 0l, addr<uint8 const?>(wh16[0]), w_bytes)
            } else {
                upload_region_at(wd, 0l, addr<uint8 const?>(wh[0]), w_bytes)
            }
            upload_region_at(xd, 0l, addr<uint8 const?>(xh[0]), x_bytes)
            upload_region_at(nd, 0l, addr<uint8 const?>(nh[0]), n_bytes)
        }
        var planes <- nform_cols_cell(t, form, [ncols], ncol_max, [NformPlane(name = "the logits", words = ne, head = obase)], 2u,
            $(c : int; raw : VkCommandBuffer; var h : VkHaz; o : NformOuts) {
                upload_region_at(xd1, 0l, unsafe(addr<uint8 const?>(xh[c * dim])), x1_bytes)
                var sc1 = router_set(f16, fixed_array(wd, xd1, o.bufs[0], nd), fixed_array(w_bytes, x1_bytes, o.bytes[0], n_bytes))
                var pc1 = RouterArgs(ne = uint(ne), dim = uint(dim), woff = 0u, obase = uint(obase), norm_on = non, nwoff = 0u, eps = eps)
                router_enc(f16, raw, h, sc1, pc1, int64(ne))
            },
            $(nc : int; raw : VkCommandBuffer; var h : VkHaz; o : NformOuts) {
                var sc = router_set(f16, fixed_array(wd, xd, o.bufs[0], nd), fixed_array(w_bytes, x_bytes, o.bytes[0], n_bytes))
                var pc = RouterArgs(ne = uint(ne), dim = uint(dim), woff = 0u, obase = uint(obase), norm_on = non, nwoff = 0u, eps = eps, ncols = uint(nc))
                router_enc(f16, raw, h, sc, pc, int64(ne))
            })
        var got <- plane_floats(planes.nf[0])
        router_cols_oracle(t, arm, got, xh, nh, wdev, ncols, ne, dim, obase, norm_on, eps)
        delete got
        delete planes
        delete wh
        delete xh
        delete nh
        delete wh16
        delete wdev
    } else {
        t |> skip("dasVulkan not present")
    }
}

[test]
def test_vkd_router_gemv_cols(t0 : T?) {
    t0 |> run("the router GEMV over x rows as columns == the one-row dispatch a row at a time, bit for bit, and the CPU dot") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            router_cols_arm(t, 3, false, true)    // the E-series pre-step projection's stamp
            router_cols_arm(t, 8, false, true)    // ... at the per-row planes' extent
            router_cols_arm(t, 3, false, false)   // the MoE router's stamp
            router_cols_arm(t, 3, true, false)    // ... under the gemma-4 router's fused rms
            router_cols_arm(t, 8, true, false)    // ... at the extent, every partial slot live
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_moe_routing(t0 : T?) {
    t0 |> run("the resident MoE block's routing kernels == the CPU oracles: router GEMM, per-row top-k, the bucket schedule, the combine-folded residual step") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_router_gemm_cls() && ensure_topk_rows_cls() && ensure_moe_sched_cls() && ensure_cls_ar_comb() && ensure_cls_ar_comb_f16_b()
                    && ensure_cls_ar_comb_rq() && ensure_cls_ar_comb_g4() && ensure_cls_ar_comb_g4_f16_b())) {
                t |> skip("no Vulkan device")
                return
            }
            moe_router_arm(t, MR_NER)
            moe_router_arm(t, 9)   //! fewer rows than the tile's half: the second row of every invocation is guarded off
            moe_topk_rows_arm(t, true, 0.0, MoeGate.softmax, false)
            moe_topk_rows_arm(t, false, 2.5, MoeGate.softmax, false)
            moe_topk_rows_arm(t, false, 0.0, MoeGate.softmax_weight, true)   //! gpt-oss: the biased pick, softmax over the picks
            moe_sched_arm(t)
            moe_ar_comb_arm(t, true, true, false)
            moe_ar_comb_arm(t, false, true, false)
            moe_ar_comb_arm(t, false, false, false)
            moe_ar_comb_arm(t, false, false, true)   //! gpt-oss: the down biases under the routing weights
            moe_ar_comb_g4_arm(t)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the GEMV family's lanes-per-row rule, pinned per (format, row length): the codebook and grid formats
//! take fewer lanes than the k-lattice formats at the same length, the q8 GEMV always the whole subgroup
[test]
def test_vkd_gemv_lane_rule(t0 : T?) {
    t0 |> run("gemv_lanes_per_row: the measured lane split per format class and row length") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let grid = fixed_array(KqFmt.iq2s, KqFmt.iq2xs, KqFmt.iq2xxs, KqFmt.iq3xxs, KqFmt.iq4xs, KqFmt.iq4nl, KqFmt.iq3s4)
            let lattice = fixed_array(KqFmt.k2, KqFmt.k3, KqFmt.k4, KqFmt.k5, KqFmt.k6, KqFmt.q40, KqFmt.iq3s)
            //! row length -> (the grid formats' lanes, the k-lattice formats' lanes); 0 = the whole subgroup
            let rows = fixed_array((n = 768l, g = 8u, l = 8u), (n = 1024l, g = 8u, l = 16u), (n = 1536l, g = 8u, l = 16u),
                (n = 2048l, g = 16u, l = 16u), (n = 3072l, g = 16u, l = 16u), (n = 4096l, g = 16u, l = 0u), (n = 8192l, g = 16u, l = 0u))
            for (r in rows) {
                for (f in grid) {
                    t |> equal(gemv_lanes_per_row(int(f), r.n), r.g, "{f} at K {r.n}: a grid format takes {r.g} lanes per row")
                }
                for (f in lattice) {
                    t |> equal(gemv_lanes_per_row(int(f), r.n), r.l, "{f} at K {r.n}: a k-lattice format takes {r.l} lanes per row")
                }
                t |> equal(gemv_lanes_per_row(int(KqFmt.q8), r.n), 0u, "q8 at K {r.n}: the whole subgroup")
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}
