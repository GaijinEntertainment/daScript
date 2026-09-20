options gen2
options _function_length = 0   // one flat gate per kernel arm: fixture build, dispatch, compare (the tier suites' shape)
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require math
require strings   // starts_with: the roster cell finds an entry by its name's head
require dasllama/dasllama_math
require ?vulkan _vkd_toy
require ?vulkan _vkd_oracles
require ?vulkan vulkan
require ?vulkan vulkan/vulkan_boost
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_env   // g_env_vulkan.coopmat - the resolved-default ladder cell pins and restores it
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan dasllama/dasllama_vulkan_prefill
require ?vulkan dasllama/dasllama_vulkan_decode   // da_nsplit - the decode attention's key-split pick the tile-pick cell pins
require ?vulkan dasllama/dasllama_vulkan_seams
require ?vulkan dasllama/dasllama_kqformat
require ?vulkan dasllama/dasllama_gemm_schema   // kq_dev_ssb_i - the fixtures size their device scale planes per format
require ?vulkan daslib/shader_lingua_franca   // gl_GlobalInvocationID — the CPU replay drives it per lane (only vulkan-guarded arms replay)
require dasllama/dasllama_common   // MoeGate: the routing cells' select oracle takes the softmax router
require ?vulkan dasllama/dasllama_moe   // moe_select_core: the host select the window's per-row top-k mirrors
require daslib/fio   // the lens readonly gate's spawned compiles
require daslib/strings_boost   // join: the spawned fixture's source
require ?vulkan daslib/math_bits   // float_bits_to_uint: the f16 GEMM cell reads its NaN sentinel by bit pattern

let private TILE_SLACK_ROWS = int(TILE_READ_SLACK)   //! the cm2 fixtures' rows past the last region, the engine's count: a stamp that loads partial columns unclamped (`STILE`) reads the last region's column rounded up to its BN, so the slack holds while every such stamp keeps BN <= slack + 1
let private TILE_CELL_K = 768   //! every per-format tile cell's K: three unrolled superblock passes on both depths (a K-quant width is a multiple of 256, so no K-quant stamp ever takes the k loop's tail; the q8 s and e cells' Q8_CELL_K takes the edge path)
let private Q8_CELL_K = 544     //! the q8 cm2 s and e cells' K: 512 + 32, off the 64-deep stamp's k grid, so the tile takes its padded edge path (the l and m cells run K = 576, the fast path)
let private TILE_CELL_D = 160   //! every tile cell's weight rows a plane: one full 128-tile and a 32-row remainder - the KHR arm's edge tile, the cm2 arms' clamped second tile over rows 32..160
let private TILE_CELL_ROWS0 = 300   //! the tile cells' two regions: on every arm's rows a workgroup, whole tiles and an edge
let private TILE_CELL_ROWS1 = 260
let private TILE_CELL_SB = 2 * TILE_CELL_D * (TILE_CELL_K / 256)   //! the per-format cells' superblocks over both weight planes

let private ARM_CM2_L = 0
let private ARM_CM2_M = 1
let private ARM_CM2_S = 2
let private ARM_KHR = 3
let private ARM_CM2_E = 4

def private cell_arm_name(arm : int) : string {
    return arm == ARM_CM2_L ? "l" : (arm == ARM_CM2_M ? "m" : (arm == ARM_CM2_S ? "s" : (arm == ARM_KHR ? "khr" : "e")))
}

//! the arm helpers stay generic on purpose - a build without the vulkan module must never infer these bodies
//! 256 is the l stamps' BN default, a column no schedule constant names (the l column is never a schedule piece)
def private cell_arm_rows_per_wg(arm) => arm == ARM_CM2_L ? 256 : int(arm == ARM_CM2_S ? SCHED_S_ROWS : SCHED_M_ROWS)

def private cell_arm_tc(arm) => arm == ARM_CM2_E ? CM2_TC_E : int64(cell_arm_rows_per_wg(arm))

def private cell_arm_served(arm) => arm == ARM_KHR ? khr_kq_tile_on() : (g_gpu.coopmat_mode == COOPMAT_CM2 && g_gpu.has_coopmat2)

def private cell_arm_set(arm, fmt : int; bufs; sizes; gbits) {
    if (arm == ARM_KHR) {
        if (!khr_cls_ensure(fmt)) panic("the KHR class rail for format {fmt} must engage on a coopmat device")
        return khr_cls_set(fmt, bufs, sizes, gbits)
    }
    verify(g_gpu.coopmat_mode == COOPMAT_CM2, "a cm2 arm dispatched off cm2 mode - the ladder would hand back the KHR stamp")
    if (!cm2_cls_ensure(fmt, cell_arm_tc(arm))) panic("the {cell_arm_name(arm)} arm's class rail for format {fmt} must engage in cm2 mode")
    return cm2_cls_set(fmt, cell_arm_tc(arm), bufs, sizes, gbits)
}

def private cell_arm_enc(arm, fmt : int; raw; var h; var s; var pc; groups : int64) {
    if (arm == ARM_KHR) {
        khr_cls_enc(fmt, raw, h, s, pc, groups)
    } else {
        cm2_cls_enc(fmt, cell_arm_tc(arm), raw, h, s, pc, groups)
    }
}

def private tile_arms_all() : array<int> {
    return <- [ARM_CM2_L, ARM_CM2_M, ARM_CM2_S, ARM_KHR, ARM_CM2_E]
}

//! a tile cell's fixture: the format's packed weight and scale planes moved in, the f16 activation plane built here with the engine's read slack past the two regions; the device planes are made and uploaded by the arm loop once it knows an arm is served, the output plane left for its sentinel fill
struct TileFixture {
    n, blk_k, rows : int
    wqh, wsh, xfh : array<uint>
    wq_bytes, ws_bytes, xf_bytes, y_bytes : int64
    wqd, wsd, xfd, yd : uint64
}

def private tile_fixture(n, blk_k : int; var wqh, wsh : array<uint>; xf_mul, xf_add : uint) : TileFixture {
    let rows = TILE_CELL_ROWS0 + TILE_CELL_ROWS1
    let xf_words = (rows + TILE_SLACK_ROWS) * n / 2
    var xfh : array<uint>
    xfh |> resize(xf_words)
    for (i in range(xf_words)) {
        xfh[i] = ws_word(uint(i) * xf_mul + xf_add)   // f16 activation pairs
    }
    var f = TileFixture(n = n, blk_k = blk_k, rows = rows, wq_bytes = long_length(wqh) * 4l, ws_bytes = long_length(wsh) * 4l,
        xf_bytes = int64(xf_words) * 4l, y_bytes = int64(rows * TILE_CELL_D) * 4l)
    f.wqh <- wqh
    f.wsh <- wsh
    f.xfh <- xfh
    return <- f
}

//! the fixture's device planes, made and uploaded once the caller knows an arm is served; the output plane is left for the sentinel fill
def private tile_fixture_upload(var f : TileFixture) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        f.wqd = make_device_buf(f.wq_bytes)
        f.wsd = make_device_buf(f.ws_bytes)
        f.xfd = make_device_buf(f.xf_bytes)
        f.yd = make_device_buf(f.y_bytes)
        unsafe {
            upload_region_at(f.wqd, 0l, addr<uint8 const?>(f.wqh[0]), f.wq_bytes)
            upload_region_at(f.wsd, 0l, addr<uint8 const?>(f.wsh[0]), f.ws_bytes)
            upload_region_at(f.xfd, 0l, addr<uint8 const?>(f.xfh[0]), f.xf_bytes)
        }
    }
}

//! the q8 fmt-0 host planes: s8 quant bytes and f16 scale pairs from two seeds, K = n over both weight planes
def private q8_planes(n : int; wq_seed, ws_seed : uint; var wqh, wsh : array<uint>) {
    let totblk = 2 * TILE_CELL_D * (n / 32)
    wqh |> resize(totblk * 8)
    wsh |> resize((totblk + 1) / 2)
    for (i in range(totblk * 8)) {
        wqh[i] = hash_word(uint(i) + wq_seed)
    }
    for (i in range((totblk + 1) / 2)) {
        wsh[i] = ws_word(uint(i) + ws_seed)
    }
}

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
        tile_fixture_upload(f)
        let d = TILE_CELL_D
        var host = make_host_buf(f.y_bytes, true, [cached = true])
        var y_cls : array<float>
        y_cls |> resize(f.rows * d)
        y_ref |> resize(f.rows * d)
        let tail = 2
        for (arm in arms) {
            continue if (!cell_arm_served(arm))
            let tname = cell_arm_name(arm)
            let ttile = cell_arm_rows_per_wg(arm)
            let wtiles = (d + 127) / 128
            let wgs0 = ((TILE_CELL_ROWS0 + ttile - 1) / ttile) * wtiles
            let wgs = wgs0 + ((TILE_CELL_ROWS1 + ttile - 1) / ttile) * wtiles
            var sched : array<uint>
            sched |> resize(2 * 4 + wgs + tail)
            sched[2] = uint(TILE_CELL_ROWS0)
            sched[4] = uint(d * (f.n / f.blk_k))
            sched[5] = uint(TILE_CELL_ROWS0)
            sched[6] = uint(TILE_CELL_ROWS1)
            sched[7] = uint(wgs0)
            for (w in range(wgs)) {
                sched[8 + w] = w < wgs0 ? 0u : 1u
            }
            for (w in range(wgs, wgs + tail)) {
                sched[8 + w] = SCHED_NONE
            }
            unsafe {
                nan_sentinel_upload(f.yd, f.y_bytes)
                let sc_bytes = int64(2 * 4 + wgs + tail) * 4l
                let scd = make_device_buf(sc_bytes)
                upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
                var sc = cell_arm_set(arm, fmt, fixed_array(f.wqd, f.wsd, scd, f.xfd, f.yd),
                    fixed_array(f.wq_bytes, f.ws_bytes, sc_bytes, f.xf_bytes, f.y_bytes), fixed_array(1u, 2u, 0u, 4u, 16u))
                var raw2 = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                var h2 : VkHaz
                var pc = BatchArgs(n = uint(f.n), d = uint(d), map_off = uint(2 * 4))
                cell_arm_enc(arm, fmt, raw2, h2, sc, pc, int64(wgs + tail))
                vhz_dep(raw2, h2, 16u, 0u, true)
                cmd_copy_whole(raw2, f.yd, host.buf, f.y_bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(y_cls[0]), host.mapped, f.y_bytes)
            }
            invoke(oracle, f.wqh, f.wsh, f.xfh, sched, 2, f.n, d, y_ref)
            let bad = mismatch_bars(y_cls, y_ref, 2e-2, 4e-3 * max_abs(y_ref))
            to_log(LOG_INFO, "{tag} {tname}-tile batch: {bad} of {f.rows * d} off the oracle, {g_gpu.decvec_on ? "four-wide twin" : "scalar callback"}\n")
            t |> success(bad == 0, "{tag} {tname}-tile matches the CPU oracle ({bad} of {f.rows * d} off)")
            var y_poison := y_cls   // the bar's control: one element pushed past both bars must red
            y_poison[0] += 1.0 + 2.0 * max_abs(y_ref)
            t |> success(mismatch_bars(y_poison, y_ref, 2e-2, 4e-3 * max_abs(y_ref)) > 0, "the {tag} {tname}-tile bar reds a poisoned element")
            delete y_poison
            delete sched
        }
        delete y_cls
    }
    return <- y_ref
}

// The model-less Vulkan kernel-unit suite ([vk_dispatch] bring-up): a class kernel dispatched
// through the GENERATED surface (ensure/set/enc — per-class layout, push constants, hazard rail)
// against the CPU run of the SAME method (the class body is ordinary daslang; the driver loop
// feeds gl_GlobalInvocationID). The facts the cells' gates key on: a Vulkan device (a class's
// ensure engages it); an NV_coopmat2 device in cm2 mode (DASLLAMA_COOPMAT=cm2) for the cm2
// l/m/s/e arms; KHR cooperative matrix at subgroup 32 for the KHR arm; DASLLAMA_CM2_TILE and
// DASLLAMA_CM2_SPLITK for the tile-pick cell. Without dasVulkan (or with no device) every check
// skips or feints cleanly.

[test]
def test_vkd_class_dispatch(t0 : T?) {
    t0 |> run("class kernel through the generated [vk_dispatch] surface == CPU") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_vkd_toy()) {
                t |> skip("no Vulkan device")
                return
            }
            let n = 250   // deliberately not a multiple of 64 — the in-kernel guard covers the pad
            let bytes = int64(n) * 4l
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
            let xdev = make_device_buf(bytes)
            let ydev = make_device_buf(bytes)
            var host = make_host_buf(bytes, true, [cached = true])
            var gpu : array<float>
            gpu |> resize(n)
            unsafe {
                upload_region_at(xdev, 0l, addr<uint8 const?>(xs[0]), bytes)
                var s = set_vkd_toy(fixed_array(xdev, ydev), fixed_array(bytes, bytes), fixed_array(1u, 2u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                var pc = VtArgs(n = uint(n), gain = 2.5)
                enc_vkd_toy(raw, h, s, pc, int64(n))
                vhz_dep(raw, h, 2u, 0u, true)   // ys (bit 2) reads back over the copy engine
                cmd_copy_whole(raw, ydev, host.buf, bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                memcpy(addr<void?>(gpu[0]), host.mapped, bytes)
            }
            var bad = 0
            for (i in range(n)) {
                if (gpu[i] != k.ys[i]) {
                    bad++
                }
            }
            t |> success(bad == 0, "GPU == CPU on all {n} lanes ({bad} diverge)")
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

//! SPIR-V's OpDecorate is opcode 71, three words (target, decoration) or four with the decoration's literal; Binding is 33, NonWritable 24
let private NAN_SENTINEL_BITS = 0x7fc00000u   //! the output fill before a dispatch: a NaN, so a stale or unwritten element cannot sit inside any bar
let private SPV_HEADER_WORDS = 5   //! the module header the instruction walk skips
let private SPV_OP_DECORATE = 71u
let private SPV_DECOR_NONWRITABLE = 24u
let private SPV_DECOR_BINDING = 33u
let private SPV_OP_CAPABILITY = 17u
let private SPV_OP_VARIABLE = 59u
let private SPV_STORAGE_WORKGROUP = 4u
let private SPV_CAP_COOPMAT_KHR = 6022u
let private SPV_CAP_COOPMAT_NV = 5357u
let private SPV_CAP_NV_COOPMAT2_FIRST = 5430u   //! CooperativeMatrixReductionsNV through TensorAddressingNV: the NV_cooperative_matrix2 capabilities
let private SPV_CAP_NV_COOPMAT2_LAST = 5439u

def private spv_workgroup_vars(words : array<uint>) : int {
    var n = 0
    var i = SPV_HEADER_WORDS
    while (i < length(words)) {
        let wc = int(words[i] >> 16u)
        let op = words[i] & 0xFFFFu
        if (op == SPV_OP_VARIABLE && wc >= 4 && words[i + 3] == SPV_STORAGE_WORKGROUP) {
            n++
        }
        i += max(wc, 1)
    }
    return n
}

def private spv_capabilities(words : array<uint>) : table<uint> {
    var caps : table<uint>
    var i = SPV_HEADER_WORDS
    while (i < length(words)) {
        let wc = int(words[i] >> 16u)
        let op = words[i] & 0xFFFFu
        if (op == SPV_OP_CAPABILITY && wc == 2) {
            caps |> insert(words[i + 1])
        }
        i += max(wc, 1)
    }
    return <- caps
}

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
            res.wg_vars = spv_workgroup_vars(words)
            res.caps <- spv_capabilities(words)
        }
    }
    return <- res
}

def private fa_ensure_panics(khr : bool; hs : int64; gated, cap : bool; sink : bool = false) : bool {
    var tripped = false
    static_if (typeinfo builtin_module_exists(vulkan)) {
        try {
            fa_stamp_ensure(khr, hs, gated, cap, sink, false)
        } recover {
            tripped = true
        }
    }
    return tripped
}

def private fa_enc_panics(khr : bool; hs : int64; gated, cap : bool; sink : bool = false) : bool {
    var tripped = false
    static_if (typeinfo builtin_module_exists(vulkan)) {
        try {
            var raw : VkCommandBuffer
            var h : VkHaz
            var s : VkDescriptorSet
            var pc : FaArgs
            fa_stamp_enc(khr, hs, gated, cap, sink, false, raw, h, s, pc, 1l)
        } recover {
            tripped = true
        }
    }
    return tripped
}

def private nonwritable_bindings(words : array<uint>) : table<int> {
    var binding_of : table<uint; int>
    var nonwritable : table<uint>
    var i = SPV_HEADER_WORDS
    while (i < length(words)) {
        let wc = int(words[i] >> 16u)
        let op = words[i] & 0xFFFFu
        if (op == SPV_OP_DECORATE && wc == 4 && words[i + 2] == SPV_DECOR_BINDING) {
            binding_of[words[i + 1]] = int(words[i + 3])
        } elif (op == SPV_OP_DECORATE && wc == 3 && words[i + 2] == SPV_DECOR_NONWRITABLE) {
            nonwritable |> insert(words[i + 1])
        }
        i += max(wc, 1)
    }
    var res : table<int>
    for (id, b in keys(binding_of), values(binding_of)) {
        if (key_exists(nonwritable, id)) {
            res |> insert(b)
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
            if (!ensure_f16cvt_cls()) {   // engage the device without touching coopmat rails
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
            t |> success(fa_ensure_panics(true, 256l, true, false), "the KHR arm has no gated stamp: the ensure ladder refuses")
            t |> success(fa_ensure_panics(false, 128l, false, true), "no softcap stamp off head size 256: the ensure ladder refuses")
            t |> success(fa_ensure_panics(false, 128l, true, false), "no gated stamp off head size 256: the ensure ladder refuses")
            t |> success(fa_ensure_panics(false, 96l, false, false), "a head size no stamp serves: the ensure ladder refuses")
            t |> success(fa_enc_panics(true, 256l, true, false), "the KHR arm has no gated stamp: the encode ladder refuses")
            t |> success(fa_enc_panics(false, 128l, false, true), "no softcap stamp off head size 256: the encode ladder refuses")
            t |> success(fa_enc_panics(false, 128l, true, false), "no gated stamp off head size 256: the encode ladder refuses")
            t |> success(fa_enc_panics(false, 96l, false, false), "a head size no stamp serves: the encode ladder refuses")
            t |> success(fa_ensure_panics(false, 128l, false, false, true), "no sink stamp off head size 64: the ensure ladder refuses")
            t |> success(fa_ensure_panics(false, 256l, false, true, true), "no sink stamp at the softcap's head size: the ensure ladder refuses")
            t |> success(fa_enc_panics(true, 128l, false, false, true), "no sink stamp off head size 64: the encode ladder refuses")
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
    let dir_r = create_temp_directory_result("dasllama_vk_lens_{tag}")
    if (!(dir_r is value)) {
        return (rc = -1, out = "temp dir failed")
    }
    let dir = unsafe(dir_r.value)
    let path = path_join(dir, "prog.das")
    if (!fwrite(path, src)) {
        rmdir_rec_result(dir)
        return (rc = -1, out = "fwrite failed")
    }
    let argv <- get_command_line_arguments()
    let self = find(to_generic_path(argv[0]), "/") < 0 ? argv[0] : get_full_file_name(normalize(argv[0]))
    var out : string
    let rc = run_and_capture([self, "-compile-only", path], out, 180.0)
    rmdir_rec_result(dir)
    return (rc = rc, out = out)
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

//! the KHR arm loads and stores whole 16-row fragments, so the cell sizes its x and y planes past `rows` the way the prefill's are
def private f16_gemm_cell(t : T?; tag : string; d, n, rows : int; khr : bool = false) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        {
            let nsplit = 8
            let ybase = 16   // the rows land past a header the kernels must leave alone
            let rows_alloc = khr ? ((rows + 15) / 16) * 16 : rows   //! the KHR tile loads and stores whole 16-row fragments: the x and y planes carry the spare rows, as the prefill's do
            let yn = ybase + rows_alloc * d
            var wh : array<float16>
            var xh : array<float16>
            wh |> resize(d * n)
            xh |> resize(rows_alloc * n)
            for (i in range(d * n)) {
                wh[i] = float16(float((i * 7) % 61) * 0.02 - 0.6)
            }
            for (i in range(rows_alloc * n)) {
                xh[i] = float16(float((i * 11) % 53) * 0.03 - 0.8)
            }
            let w_bytes = int64(d * n) * 2l
            let x_bytes = int64(rows_alloc * n) * 2l
            let y_bytes = int64(yn) * 4l
            let part_bytes = int64(nsplit * rows * d) * 4l
            let wd = make_device_buf(w_bytes)
            let xd = make_device_buf(x_bytes)
            let yd = make_device_buf(y_bytes)
            let yd2 = make_device_buf(y_bytes)
            let pd = make_device_buf(part_bytes)
            var host = make_host_buf(2l * y_bytes, true, [cached = true])
            var got : array<float>
            got |> resize(2 * yn)
            nan_sentinel_upload(yd, y_bytes, 0l)
            nan_sentinel_upload(yd2, y_bytes, 0l)
            unsafe {
                upload_region_at(wd, 0l, addr<uint8 const?>(wh[0]), w_bytes)
                upload_region_at(xd, 0l, addr<uint8 const?>(xh[0]), x_bytes)
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                var pc = F16GemmArgs(n = uint(n), d = uint(d), rows = uint(rows), woff = 0u, ybase = uint(ybase), ystride = uint(d), ksplit = 0u)
                var sd = f16_gemm_set(khr, fixed_array(wd, xd, yd), fixed_array(w_bytes, x_bytes, y_bytes), fixed_array(1u, 2u, 4u))
                f16_gemm_enc(khr, raw, h, sd, pc, int64(rows), int64(d))
                if (!khr) {   // the split arm is the cm2 stamp's alone: the KHR stamp reads no k split
                    nan_sentinel_upload(pd, part_bytes, 0l)   // the split arm's partial planes: a chunk the GEMM skipped would sum a stale plane into the reduce
                    var sp = set_f16_gemm_cm2_cls(fixed_array(wd, xd, pd), fixed_array(w_bytes, x_bytes, part_bytes), fixed_array(1u, 2u, 8u))
                    var sr = set_splitk_reduce_cls(fixed_array(pd, yd2), fixed_array(part_bytes, y_bytes), fixed_array(8u, 16u))
                    var pc2 = F16GemmArgs(n = uint(n), d = uint(d), rows = uint(rows), woff = 0u, ybase = 0u, ystride = uint(d), ksplit = uint(n / nsplit))
                    enc_f16_gemm_cm2_cls(raw, h, sp, pc2, f16_gemm_wgs(false, int64(rows), int64(d)) * int64(nsplit))
                    var pcr = SkRedArgs(nelem = uint(rows * d), k = uint(nsplit), ybase = uint(ybase))
                    enc_splitk_reduce_cls(raw, h, sr, pcr, int64(rows * d + 1023) / 1_024l)
                }
                vhz_dep(raw, h, 4u | 16u, 0u, true)
                cmd_copy_range(raw, yd, 0l, host.buf, 0l, y_bytes)
                cmd_copy_range(raw, yd2, 0l, host.buf, y_bytes, y_bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                memcpy(addr<void?>(got[0]), host.mapped, 2l * y_bytes)
            }
            var ref : array<float>
            ref |> resize(rows * d)
            var amax = 0.0
            for (p in range(rows)) {
                for (e in range(d)) {
                    var s = 0.0
                    for (kk in range(n)) {
                        s += float(xh[p * n + kk]) * float(wh[e * n + kk])
                    }
                    ref[p * d + e] = s
                    amax = max(amax, abs(s))
                }
            }
            let tol = amax * 2e-5   //! both operands are f16-exact, so every product is exact in f32 and the arms differ by f32 reassociation over K alone; 2e-5 of the max holds the cell's shapes with the poison leg red
            let poison = tol * 4.0
            let arms = fixed_array(khr ? "khr direct" : "direct", "split-k")
            for (arm in range(khr ? 1 : 2)) {
                let base = arm * yn
                var header_ok = true
                for (i in range(ybase)) {
                    header_ok = header_ok && float_bits_to_uint(got[base + i]) == NAN_SENTINEL_BITS
                }
                var worst = 0.0
                var worst_poisoned = 1.0e30
                for (i in range(rows * d)) {
                    let dv = abs(got[base + ybase + i] - ref[i])
                    worst = max(worst, dv)
                    worst_poisoned = min(worst_poisoned, abs(got[base + ybase + i] - (ref[i] + poison)))
                }
                t |> success(header_ok, "f16 GEMM {tag} {arms[arm]}: the {ybase} words under the base stay the sentinel")
                t |> success(worst <= tol, "f16 GEMM {tag} {arms[arm]}: {rows} x {d} over K {n} within {tol} of the f32 dot (worst {worst}, max |y| {amax})")
                t |> success(worst_poisoned > tol, "f16 GEMM {tag} {arms[arm]}: the poisoned expectation reds the bar (nearest {worst_poisoned})")
            }
            delete wh
            delete xh
            delete got
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
            let wq_bytes = 2l * 32l
            let ws_bytes = 2l * 2l
            let y_bytes = int64(n) * 4l
            let wqd = make_device_buf(wq_bytes)
            let wsd = make_device_buf(ws_bytes)
            let yd = make_device_buf(y_bytes)
            var host = make_host_buf(y_bytes, true, [cached = true])
            var gpu : array<float>
            gpu |> resize(n)
            unsafe {
                upload_region_at(wqd, 0l, addr<uint8 const?>(k.wq[0].qs[0]), wq_bytes)
                upload_region_at(wsd, 0l, addr<uint8 const?>(k.ws[0]), ws_bytes)
                var s = set_vkd_direct_decode(fixed_array(wqd, wsd, yd), fixed_array(wq_bytes, ws_bytes, y_bytes), fixed_array(1u, 2u, 4u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                var pc = VdArgs(n = uint(n))
                enc_vkd_direct_decode(raw, h, s, pc, 1l)
                vhz_dep(raw, h, 4u, 0u, true)
                cmd_copy_whole(raw, yd, host.buf, y_bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                memcpy(addr<void?>(gpu[0]), host.mapped, y_bytes)
            }
            var bad = 0
            var nonzero = 0
            for (i in range(n)) {
                if (gpu[i] != k.y[i]) {
                    bad++
                }
                if (k.y[i] != 0.0) {
                    nonzero++
                }
            }
            t |> success(nonzero > 48, "the oracle is non-trivial ({nonzero} of {n} lanes non-zero)")
            t |> success(bad == 0, "GPU == CPU on all {n} lanes through the direct decode call ({bad} diverge)")
            var poisoned = 0   // the control: one oracle lane pushed off must red the same compare
            gpu[3]++
            for (i in range(n)) {
                if (gpu[i] != k.y[i]) {
                    poisoned++
                }
            }
            t |> success(poisoned == 1, "the exact compare reds a poisoned lane ({poisoned} diverge)")
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
            let bytes = int64(dim) * 4l
            let eps = 1e-5
            let ascale = 1.44
            var x : array<float>
            var a : array<float>
            var w : array<float>
            x |> resize(dim)
            a |> resize(dim)
            w |> resize(dim)
            for (i in range(dim)) {
                x[i] = float((i * 37) % 101) * 0.11 - 5.0
                a[i] = float((i * 13) % 89) * 0.07 - 3.0
                w[i] = 0.5 + float(i % 17) * 0.05
            }
            // the seam is the reference: the SHIPPED kernel on the same device, same inputs
            var x2 := x
            var y2 : array<float>
            y2 |> resize(dim)
            vk_add_rms(x2, a, w, y2, int64(dim), eps, ascale, true)
            // the residual plane is IN-PLACE, so it cannot hold a sentinel — this liveness check
            // is its non-vacuity witness (add_on with a[0] = -3.0 must change x2)
            t |> success(x2[0] != x[0], "the seam wrote the residual stream")
            // the class twin through the generated surface
            let xd = make_device_buf(bytes)
            let ad = make_device_buf(bytes)
            let wd = make_device_buf(bytes)
            let yd = make_device_buf(bytes)
            var host = make_host_buf(bytes, true, [cached = true])
            var yc : array<float>
            var xc : array<float>
            yc |> resize(dim)
            xc |> resize(dim)
            // Sentinel-prefill the twin's output plane: a GPU-vs-GPU compare is vacuous-prone —
            // when BOTH kernels are dead (the MoltenVK 1.4.1 silent-discard cliff) two zeroed
            // buffers match trivially. A sentinel no real computation can produce turns every
            // dead combination into a loud mismatch.
            var ysent : array<float>
            ysent |> resize(dim)
            for (i in range(dim)) {
                ysent[i] = -999.25
            }
            unsafe {
                upload_region_at(yd, 0l, addr<uint8 const?>(ysent[0]), bytes)
                upload_region_at(xd, 0l, addr<uint8 const?>(x[0]), bytes)
                upload_region_at(ad, 0l, addr<uint8 const?>(a[0]), bytes)
                upload_region_at(wd, 0l, addr<uint8 const?>(w[0]), bytes)
                var s = set_cls_ar(fixed_array(xd, ad, wd, yd), fixed_array(bytes, bytes, bytes, bytes), fixed_array(1u, 2u, 4u, 8u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                var pc = ArArgs(dim = uint(dim), add_on = 1u, eps = eps, ascale = ascale)
                enc_cls_ar(raw, h, s, pc, 1l)
                vhz_dep(raw, h, 8u | 1u, 0u, true)   // y + the updated x read back over the copy engine
                cmd_copy_whole(raw, yd, host.buf, bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                memcpy(addr<void?>(yc[0]), host.mapped, bytes)
                var raw2 = alloc_cmd()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                cmd_copy_whole(raw2, xd, host.buf, bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(xc[0]), host.mapped, bytes)
            }
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
                var b : array<float>
                var ab : array<float>
                var wb : array<float>
                b |> resize(dim)
                ab |> resize(dim)
                wb |> resize(2 * dim)
                for (i in range(dim)) {
                    b[i] = float((i * 23) % 71) * 0.09 - 3.1
                    ab[i] = a[i] + b[i]
                    wb[i] = w[i]
                    wb[dim + i] = b[i]
                }
                var x3 := x
                var y3 : array<float>
                y3 |> resize(dim)
                vk_add_rms(x3, ab, w, y3, int64(dim), eps, ascale, true)
                let wbd = make_device_buf(2l * bytes)
                unsafe {
                    upload_region_at(yd, 0l, addr<uint8 const?>(ysent[0]), bytes)
                    upload_region_at(xd, 0l, addr<uint8 const?>(x[0]), bytes)
                    upload_region_at(wbd, 0l, addr<uint8 const?>(wb[0]), 2l * bytes)
                    var s3 = set_cls_ar(fixed_array(xd, ad, wbd, yd), fixed_array(bytes, bytes, 2l * bytes, bytes), fixed_array(1u, 2u, 4u, 8u))
                    var raw3 = alloc_cmd()
                    let begin3 = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw3, begin3), null)
                    var h3 : VkHaz
                    var pc3 = ArArgs(dim = uint(dim), add_on = 1u, eps = eps, ascale = ascale, abias_on = 1u, aboff = uint(dim))
                    enc_cls_ar(raw3, h3, s3, pc3, 1l)
                    vhz_dep(raw3, h3, 8u | 1u, 0u, true)
                    cmd_copy_whole(raw3, yd, host.buf, bytes)
                    vk_check(vkEndCommandBuffer(raw3), null)
                    submit_wait(raw3)
                    memcpy(addr<void?>(yc[0]), host.mapped, bytes)
                    var raw4 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw4, begin3), null)
                    cmd_copy_whole(raw4, xd, host.buf, bytes)
                    vk_check(vkEndCommandBuffer(raw4), null)
                    submit_wait(raw4)
                    memcpy(addr<void?>(xc[0]), host.mapped, bytes)
                }
                var bad_yb = 0
                var bad_xb = 0
                var moved = 0
                for (i in range(dim)) {
                    if (yc[i] != y3[i]) {
                        bad_yb++
                    }
                    if (xc[i] != x3[i]) {
                        bad_xb++
                    }
                    if (xc[i] != x2[i]) {
                        moved++
                    }
                }
                t |> success(bad_yb == 0, "biased partner: the normed output is bit-exact vs the seam fed the pre-added partner ({bad_yb} of {dim} diverge)")
                t |> success(bad_xb == 0, "biased partner: the residual stream is bit-exact vs that seam ({bad_xb} of {dim} diverge)")
                t |> success(moved > 0, "biased partner control: the bias moves the residual off the unbiased step ({moved} of {dim})")
                //! the CPU oracle beside the seam: x' = (x + a + b) * ascale, y = w * x' * rsqrt(mean(x'^2) + eps)
                var x_cpu : array<float>
                var y_cpu : array<float>
                x_cpu |> resize(dim)
                y_cpu |> resize(dim)
                var ssb = 0.0
                for (i in range(dim)) {
                    x_cpu[i] = (x[i] + ab[i]) * ascale
                    ssb += x_cpu[i] * x_cpu[i]
                }
                let invb = 1.0 / sqrt(ssb / float(dim) + eps)
                for (i in range(dim)) {
                    y_cpu[i] = w[i] * (x_cpu[i] * invb)
                }
                t |> success(mismatch_approx(yc, y_cpu) == 0 && mismatch_approx(xc, x_cpu) == 0, "biased partner: the normed output and the residual match the CPU oracle ({mismatch_approx(yc, y_cpu)} / {mismatch_approx(xc, x_cpu)} of {dim} off)")
                y_cpu[2] += 0.5
                t |> success(mismatch_approx(yc, y_cpu) == 1, "biased partner control: the CPU bar reds one poisoned element")
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
            delete ysent
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the classifier epilogue's rows form: `rows` logits rows through one dispatch of `rows` (the whole plane), or a
//! dispatch of one row `rows` times - the row-at-a-time form is the rows form at one row
def private cls_epi_rows_run(y : array<float>; supp : array<uint>; vocab, rows : int64; cap : float; a_row_at_a_time : bool) : array<float> {
    var yc : array<float>
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let row_bytes = vocab * 4l
        let sbytes = max(long_length(supp), 1l) * 4l
        let sd = make_device_buf(sbytes)
        var host = make_host_buf(row_bytes * rows, true, [cached = true])
        yc |> resize(vocab * rows)
        let begin = VkCommandBufferBeginInfo()
        unsafe {
            if (!empty(supp)) {
                upload_region_at(sd, 0l, addr<uint8 const?>(supp[0]), sbytes)
            }
            let per = a_row_at_a_time ? 1l : rows   //! rows a dispatch
            var r0 = 0l
            while (r0 < rows) {
                let bytes = row_bytes * per
                let yd = make_device_buf(bytes)
                upload_region_at(yd, 0l, addr<uint8 const?>(y[r0 * vocab]), bytes)
                var s = set_cls_epi(fixed_array(yd, sd), fixed_array(bytes, sbytes), fixed_array(1u, 0u))
                var raw = alloc_cmd()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                var pc = ClsEpiArgs(vocab = uint(vocab), cap = cap, nsupp = uint(length(supp)), rows = uint(per))
                enc_cls_epi(raw, h, s, pc, (vocab * per + 255l) / 256l)
                vhz_dep(raw, h, 1u, 0u, true)
                cmd_copy_whole(raw, yd, host.buf, bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                memcpy(addr<void?>(yc[r0 * vocab]), host.mapped, bytes)
                r0 += per
            }
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
            var y : array<float>
            y |> resize(vocab * rows)
            for (i in range64(vocab * rows)) {
                y[i] = float((i * 37l) % 211l) * 0.7 - 60.0   //! reaches past the cap on both sides
            }
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
                    if (d > bar) {
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
            let bytes = int64(total) * 4l
            let wbytes = int64(dim) * 4l
            let eps = 1e-5
            var x : array<float>
            var a : array<float>
            var w : array<float>
            x |> resize(total)
            a |> resize(total)
            w |> resize(dim)
            for (i in range(total)) {
                x[i] = float((i * 37) % 101) * 0.11 - 5.0
                a[i] = float((i * 13) % 89) * 0.07 - 3.0
            }
            for (i in range(dim)) {
                w[i] = 0.5 + float(i % 17) * 0.05
            }
            // the reference: the seam row by row from row0; rows below keep x and a sentinel y
            var x2 := x
            var y2 : array<float>
            y2 |> resize(total)
            for (i in range(total)) {
                y2[i] = -999.25
            }
            for (r in range(row0, rows)) {
                var xr : array<float>
                var ar : array<float>
                var yr : array<float>
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
            let xd = make_device_buf(bytes)
            let ad = make_device_buf(bytes)
            let wd = make_device_buf(wbytes)
            let yd = make_device_buf(bytes)
            var host = make_host_buf(bytes, true, [cached = true])
            var yc : array<float>
            var xc : array<float>
            yc |> resize(total)
            xc |> resize(total)
            var ysent : array<float>
            ysent |> resize(total)
            for (i in range(total)) {
                ysent[i] = -999.25
            }
            unsafe {
                upload_region_at(yd, 0l, addr<uint8 const?>(ysent[0]), bytes)
                upload_region_at(xd, 0l, addr<uint8 const?>(x[0]), bytes)
                upload_region_at(ad, 0l, addr<uint8 const?>(a[0]), bytes)
                upload_region_at(wd, 0l, addr<uint8 const?>(w[0]), wbytes)
                var s = set_cls_ar(fixed_array(xd, ad, wd, yd), fixed_array(bytes, bytes, wbytes, bytes), fixed_array(1u, 2u, 4u, 8u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                var pc = ArArgs(dim = uint(dim), add_on = 1u, eps = eps, ascale = 1.0, row0 = uint(row0))
                enc_cls_ar(raw, h, s, pc, int64(rows - row0))
                vhz_dep(raw, h, 8u | 1u, 0u, true)
                cmd_copy_whole(raw, yd, host.buf, bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                memcpy(addr<void?>(yc[0]), host.mapped, bytes)
                var raw2 = alloc_cmd()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                cmd_copy_whole(raw2, xd, host.buf, bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(xc[0]), host.mapped, bytes)
            }
            let bad_y = mismatch_exact(yc, y2)
            let bad_x = mismatch_exact(xc, x2)
            t |> success(bad_y == 0, "normed rows from row0 bit-exact, the row below still the sentinel ({bad_y} of {total} diverge)")
            t |> success(bad_x == 0, "residual rows from row0 bit-exact, the row below untouched ({bad_x} of {total} diverge)")
            delete x
            delete a
            delete w
            delete x2
            delete y2
            delete yc
            delete xc
            delete ysent
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
            let g_bytes = int64(total) * 4l
            let h_bytes = int64(total) * 2l
            var gh : array<float>
            var uh : array<float>
            gh |> resize(total)
            uh |> resize(total)
            for (i in range(total)) {
                gh[i] = float((i * 29) % 61) * 0.06 - 1.9
                uh[i] = float((i * 43) % 83) * 0.05 - 2.1
            }
            // arm 0: the second row's data at the front, base 0; arm 1: the plane as is, base nelem
            var g0 : array<float>
            var u0 : array<float>
            g0 |> resize(total)
            u0 |> resize(total)
            for (i in range(nelem)) {
                g0[i] = gh[nelem + i]
                u0[i] = uh[nelem + i]
            }
            var hsent : array<uint>
            hsent |> resize(total / 2)
            for (i in range(total / 2)) {
                hsent[i] = 0xFC00FC00u
            }
            let gd = make_device_buf(g_bytes)
            let ud = make_device_buf(g_bytes)
            let hd = make_device_buf(h_bytes)
            var host = make_host_buf(h_bytes, true, [cached = true])
            var h0 : array<uint>
            var h1 : array<uint>
            h0 |> resize(total / 2)
            h1 |> resize(total / 2)
            for (arm in range(2)) {
                unsafe {
                    upload_region_at(gd, 0l, addr<uint8 const?>(arm == 0 ? g0[0] : gh[0]), g_bytes)
                    upload_region_at(ud, 0l, addr<uint8 const?>(arm == 0 ? u0[0] : uh[0]), g_bytes)
                    upload_region_at(hd, 0l, addr<uint8 const?>(hsent[0]), h_bytes)
                    var sc = set_actf16_cls(fixed_array(gd, ud, hd), fixed_array(g_bytes, g_bytes, h_bytes), fixed_array(1u, 2u, 4u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    var pc = ActArgs(nelem = uint(nelem), act = ACT_GELU, nblk = 0u, elem0 = arm == 0 ? 0u : uint(nelem))
                    enc_actf16_cls(raw, h, sc, pc, int64((nelem / 4 + 255) / 256))
                    vhz_dep(raw, h, 4u, 0u, true)
                    cmd_copy_whole(raw, hd, host.buf, h_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(arm == 0 ? h0[0] : h1[0]), host.mapped, h_bytes)
                }
            }
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
            delete hsent
            delete h0
            delete h1
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// ===== the kq GEMV family: class kernels vs the CPU oracle, per format =====
// Synthetic planes — any bit pattern is valid K-quant data, EXCEPT the scale words, which are
// composed from finite halves so a NaN can't poison the compare.

def private safe_half(i : uint) : uint {
    let tab = fixed_array(0x3C00u, 0xB800u, 0x3400u, 0x4200u, 0x2E00u, 0xBC00u, 0x3E00u, 0xB000u)
    return tab[i % 8u]
}

def private ws_word(i : uint) : uint {
    return safe_half(i * 7u + 3u) | (safe_half(i * 3u + 1u) << 16u)
}

def private hash_word(i : uint) : uint {
    var h = i * 2654435761u
    h ^= h >> 16u
    h *= 2246822519u
    return h ^ (h >> 13u)
}

// ===== CPU oracles =====
// The reference side of every arm: plain sequential math (the Metal kernel-unit pattern), or the
// class's own per-block methods driven lane-free. Device subgroup/tile folds reassociate float
// sums, so float compares take the relative bar; integer packs and max folds are order-free and
// stay exact, except quants downstream of exp/tanh (GPU transcendentals differ by ulps — those
// compare as signed bytes within +-1).

def private approx(x, y : float) : bool => abs(x - y) <= 2e-3 * max(1.0, max(abs(x), abs(y)))

def private mismatch_rel(got, want : array<float>; bar : float) : int {
    var bad = 0
    for (i in range(length(got))) {
        let g = got[i]
        let w = want[i]
        if (!(abs(g - w) <= bar * max(1.0, max(abs(g), abs(w))))) {
            if (bad < 6) {   // the eyeball dump: enough to tell tolerance drift from garbage
                to_log(LOG_ERROR, "  mismatch [{i}] gpu {g} cpu {w}\n")
            }
            bad++
        }
    }
    return bad
}

def private mismatch_approx(got, want : array<float>) : int => mismatch_rel(got, want, 2e-3)

//! `mismatch_approx` over the `n` elements from `base`: one row's slice of a multi-row output
def private mismatch_approx(got, want : array<float>; base, n : int) : int {
    var bad = 0
    for (i in range(n)) {
        if (!approx(got[base + i], want[base + i])) {
            bad++
        }
    }
    return bad
}

// relative bar + an absolute floor — for f16-accumulating kernels, whose error scales with the
// running-sum magnitude, not the (possibly cancelled-small) result
def private mismatch_bars(got, want : array<float>; rel, absf : float) : int {
    var bad = 0
    for (i in range(length(got))) {
        let g = got[i]
        let w = want[i]
        if (!(abs(g - w) <= absf) && !(abs(g - w) <= rel * max(1.0, max(abs(g), abs(w))))) {
            if (bad < 6) {
                to_log(LOG_ERROR, "  mismatch [{i}] gpu {g} cpu {w}\n")
            }
            bad++
        }
    }
    return bad
}

//! `bytes` of `word` into the device buffer `dev` at `off`; the three take `dev` untyped so a build without the vulkan module never infers them
def private fill_upload(dev; off, bytes : int64; word : uint) {
    var s : array<uint>
    s |> resize(bytes / 4l)
    for (v in s) {
        v = word
    }
    upload_region_at(dev, off, unsafe(addr<uint8 const?>(s[0])), bytes)
    delete s
}

//! `bytes` of zero words into `dev` at `off`: the split attention's arrival counters start at rest
def private zero_upload(dev; off, bytes : int64) => fill_upload(dev, off, bytes, 0u)

//! NaN-fills a device output's [off, off + bytes) before its dispatch: a stale or unwritten output cannot sit inside any bar
def private nan_sentinel_upload(dev; bytes : int64; off : int64 = 0l) => fill_upload(dev, off, bytes, NAN_SENTINEL_BITS)

// the dot-product cells' bar - 1e-3 of the larger of 1 and the reference - over n outputs from base
def private rel_mismatches(got, want : array<float>; base, n : int) : int {
    var bad = 0
    for (i in range(n)) {
        if (abs(got[base + i] - want[base + i]) > 1e-3 * max(1.0, abs(want[base + i]))) {
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
            if (abs(got[at] - want[at]) > 1e-3 * max(1.0, abs(want[at]))) {
                bad++
            }
        }
    }
    return bad
}

def private max_abs(a : array<float>) : float {
    var m = 0.0
    for (v in a) {
        m = max(m, abs(v))
    }
    return m
}

def private mismatch_exact(got, want : array<float>) : int {
    var bad = 0
    for (g, w in got, want) {
        if (g != w) {
            bad++
        }
    }
    return bad
}

//! `mismatch_exact` over the first `n` elements: the rows a dispatch wrote, the sentinel past them left to its own assert
def private mismatch_exact(got, want : array<float>; n : int) : int {
    var bad = 0
    for (i in range(n)) {
        if (got[i] != want[i]) {
            bad++
        }
    }
    return bad
}

// packed quant words as signed bytes within +-tol (act chains carry transcendental ulp noise)
def private mismatch_qbytes(got, want : array<uint>; tol : int) : int {
    var bad = 0
    for (g, w in got, want) {
        for (sh in range(4)) {
            let a = int(g >> uint(sh * 8)) & 0xFF
            let b = int(w >> uint(sh * 8)) & 0xFF
            let sa = a >= 128 ? a - 256 : a
            let sb = b >= 128 ? b - 256 : b
            if (!(abs(sa - sb) <= tol)) {
                bad++
            }
        }
    }
    return bad
}

// single-query causal attention oracle: one output row per (query row, head) — plain two-pass
// softmax over [kstart, cnt) (a sliding layer's window starts past 0); the device's online form
// reassociates, so approx bars
//! `has_sink` / `sink`: the head's attention sink logit (gpt-oss) joins the softmax's max and denominator as a phantom key with no value row
def private attn_row_oracle(q : array<float>; qbase : int; kk : array<float>; kbase0 : int;
                            vv : array<float>; vbase0 : int; kvd, hh, hs, kv_mul, cnt : int;
                            scale : float; var out : array<float>; obase : int; kstart : int = 0; cap : float = 0.0;
                            has_sink : bool = false; sink : float = 0.0) {
    let khoff = (hh / kv_mul) * hs
    var sc : array<float>
    sc |> resize(cnt)
    var mm = has_sink ? sink : -3.0e38
    for (j in range(kstart, cnt)) {
        var s = 0.0
        for (e in range(hs)) {
            s += q[qbase + e] * kk[kbase0 + j * kvd + khoff + e]
        }
        sc[j] = s * scale
        if (cap != 0.0) {   // gemma-2's attention logit softcap: cap * tanh(s / cap), the exp form the kernels take
            sc[j] = cap * (1.0 - 2.0 / (1.0 + exp(2.0 * sc[j] / cap)))
        }
        mm = max(mm, sc[j])
    }
    var tot = has_sink ? exp(sink - mm) : 0.0
    for (j in range(kstart, cnt)) {
        sc[j] = exp(sc[j] - mm)
        tot += sc[j]
    }
    for (e in range(hs)) {
        var acc = 0.0
        for (j in range(kstart, cnt)) {
            acc += sc[j] * vv[vbase0 + j * kvd + khoff + e]
        }
        out[obase + e] = acc / tot
    }
    delete sc
}

[test]
def test_vkd_kq_gemv_family(t0 : T?) {
    t0 |> run("kq GEMV class family == the CPU oracle (all 13 formats)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_kq_gemv_k4_cls() && ensure_kq_gemv_q40_cls() && ensure_kq_gemv_k5_cls() && ensure_kq_gemv_k6_cls() && ensure_kq_gemv_iq4xs_cls() && ensure_kq_gemv_k3_cls() && ensure_kq_gemv_iq3s_cls() && ensure_kq_gemv_iq3xxs_cls() && ensure_kq_gemv_iq4nl_cls() && ensure_kq_gemv_k2_cls() && ensure_kq_gemv_iq2s_cls() && ensure_kq_gemv_iq2xs_cls() && ensure_kq_gemv_iq2xxs_cls())) {
                t |> skip("no Vulkan device, or a kq GEMV class pipeline failed to build")
                return
            }
            for (nsb in fixed_array(2, 20)) {   //! superblocks a row: 512 wide, the guarded tail alone; 5120 wide, four unrolled steps and a tail at every lane split
            let d = 3        // rows per region
            let nreg = 2
            let rows = nreg * d
            let nfmt = 13
            let fmts = fixed_array(int(KqFmt.k4), int(KqFmt.q40), int(KqFmt.k5), int(KqFmt.k6), int(KqFmt.iq4xs), int(KqFmt.k3), int(KqFmt.iq3s), int(KqFmt.iq3xxs), int(KqFmt.iq4nl), int(KqFmt.k2), int(KqFmt.iq2s), int(KqFmt.iq2xs), int(KqFmt.iq2xxs))
            let stride4s = fixed_array(32, 32, 40, 48, 32, 24, 26, 24, 32, 16, 18, 16, 16)   // uint WORDS per superblock (iq3s/iq3xxs/iq2s are not uint4-aligned)
            for (fi in range(nfmt)) {
                let fmt = fmts[fi]
                let stride4 = stride4s[fi]
                let nwsb = rows * nsb                       // region r's weights at r*d*nsb
                let wq_words = nwsb * stride4
                let ws_words_n = nwsb * int(kq_dev_ssb_i(fmt) / 4l)
                var wqh : array<uint>
                var wsh : array<uint>
                var xqh : array<uint>
                var xsh : array<float>
                wqh |> resize(wq_words)
                wsh |> resize(ws_words_n)
                xqh |> resize(nsb * 64)                     // nsb superblocks x 64 uints
                xsh |> resize(nsb)
                for (i in range(wq_words)) {
                    wqh[i] = hash_word(uint(i) + uint(fmt) * 77777u)
                }
                for (i in range(ws_words_n)) {
                    wsh[i] = ws_word(uint(i) + uint(fmt) * 13u)
                }
                for (i in range(nsb * 64)) {
                    xqh[i] = hash_word(uint(i) * 31u + 5u)
                }
                for (i in range(nsb)) {
                    xsh[i] = 0.5 + float(i % 7) * 0.25
                }
                let wq_bytes = int64(wq_words) * 4l
                let ws_bytes = int64(ws_words_n) * 4l
                let xq_bytes = int64(nsb) * 256l
                let xs_bytes = int64(nsb) * 4l
                let y_bytes = int64(rows) * 4l
                let wqd = make_device_buf(wq_bytes)
                let wsd = make_device_buf(ws_bytes)
                let xqd = make_device_buf(xq_bytes)
                let xsd = make_device_buf(xs_bytes)
                let yd2 = make_device_buf(y_bytes)
                let rgd = make_device_buf(int64(nreg) * 8l)
                var host = make_host_buf(y_bytes, true, [cached = true])
                var y_cls : array<float>
                y_cls |> resize(rows)
                var regs : array<uint>
                regs |> resize(nreg * 2)
                for (r in range(nreg)) {
                    regs[r * 2] = uint(r * d * nsb)
                    regs[r * 2 + 1] = 0u
                }
                var y_ref <- kq_gemv_oracle(fmt, wqh, wsh, xqh, xsh, regs, nsb * 256, d, nreg)
                unsafe {
                    upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                    upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
                    upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                    upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                    upload_region_at(rgd, 0l, addr<uint8 const?>(regs[0]), int64(nreg) * 8l)
                    let cb = fixed_array(wqd, wsd, rgd, xqd, xsd, yd2)
                    let cs = fixed_array(wq_bytes, ws_bytes, int64(nreg) * 8l, xq_bytes, xs_bytes, y_bytes)
                    let cg = fixed_array(1u, 2u, 4u, 8u, 16u, 32u)
                    var s_cls = gemv_cls_set(fmt, cb, cs, cg)
                    //! the three lane splits of a subgroup: one row, two rows, four rows (the 16-block row keeps every lane busy at eight)
                    for (lanes_per_row in fixed_array(0u, 16u, 8u)) {
                        let wgs = gemv_wgs(int64(rows), lanes_per_row)
                        nan_sentinel_upload(yd2, y_bytes)   //! a lane split that stores no row leaves the sentinel, not the last split's rows
                        var raw2 = alloc_cmd()
                        let begin = VkCommandBufferBeginInfo()
                        vk_check(vkBeginCommandBuffer(raw2, begin), null)
                        var h2 : VkHaz
                        var pc = GemvArgs(n = uint(nsb * 256), d = uint(d), nreg = uint(nreg), ybase = 0u, lanes_per_row = lanes_per_row)
                        if (fmt == int(KqFmt.k4)) {
                            enc_kq_gemv_k4_cls(raw2, h2, s_cls, pc, wgs)
                        } elif (fmt == int(KqFmt.q40)) {
                            enc_kq_gemv_q40_cls(raw2, h2, s_cls, pc, wgs)
                        } elif (fmt == int(KqFmt.k5)) {
                            enc_kq_gemv_k5_cls(raw2, h2, s_cls, pc, wgs)
                        } elif (fmt == int(KqFmt.iq4xs)) {
                            enc_kq_gemv_iq4xs_cls(raw2, h2, s_cls, pc, wgs)
                        } elif (fmt == int(KqFmt.k3)) {
                            enc_kq_gemv_k3_cls(raw2, h2, s_cls, pc, wgs)
                        } elif (fmt == int(KqFmt.iq3s)) {
                            enc_kq_gemv_iq3s_cls(raw2, h2, s_cls, pc, wgs)
                        } elif (fmt == int(KqFmt.iq3xxs)) {
                            enc_kq_gemv_iq3xxs_cls(raw2, h2, s_cls, pc, wgs)
                        } elif (fmt == int(KqFmt.iq4nl)) {
                            enc_kq_gemv_iq4nl_cls(raw2, h2, s_cls, pc, wgs)
                        } elif (fmt == int(KqFmt.k2)) {
                            enc_kq_gemv_k2_cls(raw2, h2, s_cls, pc, wgs)
                        } elif (fmt == int(KqFmt.iq2s)) {
                            enc_kq_gemv_iq2s_cls(raw2, h2, s_cls, pc, wgs)
                        } elif (fmt == int(KqFmt.iq2xs)) {
                            enc_kq_gemv_iq2xs_cls(raw2, h2, s_cls, pc, wgs)
                        } elif (fmt == int(KqFmt.iq2xxs)) {
                            enc_kq_gemv_iq2xxs_cls(raw2, h2, s_cls, pc, wgs)
                        } else {
                            enc_kq_gemv_k6_cls(raw2, h2, s_cls, pc, wgs)
                        }
                        vhz_dep(raw2, h2, 32u, 0u, true)
                        cmd_copy_whole(raw2, yd2, host.buf, y_bytes)
                        vk_check(vkEndCommandBuffer(raw2), null)
                        submit_wait(raw2)
                        memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
                        let bad = mismatch_approx(y_cls, y_ref)
                        let lanes = lanes_per_row == 0u ? g_gpu.subgroup_size : int64(lanes_per_row)
                        t |> success(bad == 0, "fmt {fmt} at {lanes} lanes per row: class kernel matches the CPU oracle ({bad} of {rows} rows off)")
                    }
                }
                if (fmt == int(KqFmt.iq4xs)) {   // the codebook pack has an independent float witness
                    var y_flt <- iq4xs_gemv_float_oracle(wqh, wsh, xqh, xsh, regs, nsb * 256, d, nreg)
                    let badf = mismatch_approx(y_ref, y_flt)
                    t |> success(badf == 0, "iq4xs: the class bit-math matches the float dequant oracle ({badf} of {rows} rows off)")
                    delete y_flt
                }
                if (fmt == int(KqFmt.k3)) {   // the lane/mask compose has an independent float witness
                    var y_flt <- k3_gemv_float_oracle(wqh, wsh, xqh, xsh, regs, nsb * 256, d, nreg)
                    let badf = mismatch_approx(y_ref, y_flt)
                    t |> success(badf == 0, "k3: the class bit-math matches the float dequant oracle ({badf} of {rows} rows off)")
                    delete y_flt
                }
                if (fmt == int(KqFmt.iq3s)) {   // the grid/sign gather has an independent float witness
                    var y_flt <- iq3s_gemv_float_oracle(wqh, wsh, xqh, xsh, regs, nsb * 256, d, nreg)
                    let badf = mismatch_approx(y_ref, y_flt)
                    t |> success(badf == 0, "iq3s: the class bit-math matches the float dequant oracle ({badf} of {rows} rows off)")
                    delete y_flt
                }
                if (fmt == int(KqFmt.iq3xxs)) {   // the halved-grid/parity-sign gather has its own float witness
                    var y_flt <- iq3xxs_gemv_float_oracle(wqh, wsh, xqh, xsh, regs, nsb * 256, d, nreg)
                    let badf = mismatch_approx(y_ref, y_flt)
                    t |> success(badf == 0, "iq3xxs: the class bit-math matches the float dequant oracle ({badf} of {rows} rows off)")
                    delete y_flt
                }
                if (fmt == int(KqFmt.iq2s)) {   // the u64-grid/sign-byte gather has its own float witness
                    var y_flt <- iq2s_gemv_float_oracle(wqh, wsh, xqh, xsh, regs, nsb * 256, d, nreg)
                    let badf = mismatch_approx(y_ref, y_flt)
                    t |> success(badf == 0, "iq2s: the class bit-math matches the float dequant oracle ({badf} of {rows} rows off)")
                    delete y_flt
                }
                if (fmt == int(KqFmt.iq2xs)) {   // the u64-grid/ksigns-parity gather has its own float witness
                    var y_flt <- iq2xs_gemv_float_oracle(wqh, wsh, xqh, xsh, regs, nsb * 256, d, nreg)
                    let badf = mismatch_approx(y_ref, y_flt)
                    t |> success(badf == 0, "iq2xs: the class bit-math matches the float dequant oracle ({badf} of {rows} rows off)")
                    delete y_flt
                }
                if (fmt == int(KqFmt.iq2xxs)) {   // the byte-grid/aux32 gather has its own float witness
                    var y_flt <- iq2xxs_gemv_float_oracle(wqh, wsh, xqh, xsh, regs, nsb * 256, d, nreg)
                    let badf = mismatch_approx(y_ref, y_flt)
                    t |> success(badf == 0, "iq2xxs: the class bit-math matches the float dequant oracle ({badf} of {rows} rows off)")
                    delete y_flt
                }
                delete regs
                delete y_ref
                delete wqh
                delete wsh
                delete xqh
                delete xsh
                delete y_cls
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
            var wqh : array<uint>
            var wsh : array<uint>
            var xqh : array<uint>
            var xsh : array<float>
            wqh |> resize(wq_words)
            wsh |> resize(ws_words_n)
            xqh |> resize(nsb * 64)
            xsh |> resize(nsb)
            for (i in range(wq_words)) {
                wqh[i] = hash_word(uint(i) * 7u + 4441u)
            }
            for (i in range(ws_words_n)) {
                wsh[i] = ws_word(uint(i) * 3u + 91u)
            }
            for (i in range(nsb * 64)) {
                xqh[i] = hash_word(uint(i) * 23u + 17u)
            }
            for (i in range(nsb)) {
                xsh[i] = 0.5 + float(i % 5) * 0.25
            }
            var regs : array<uint>
            var regs3 : array<uint>
            regs |> resize(nreg * 2)
            regs3 |> resize(nreg * 2)
            for (r in range(nreg)) {
                regs[r * 2] = uint(r * d * nsb)
                regs[r * 2 + 1] = 0u
                regs3[r * 2] = uint((nreg + r) * d * nsb)
                regs3[r * 2 + 1] = 0u
            }
            var y_g <- kq_gemv_oracle(fmt, wqh, wsh, xqh, xsh, regs, nsb * 256, d, nreg)
            var y_u <- kq_gemv_oracle(fmt, wqh, wsh, xqh, xsh, regs3, nsb * 256, d, nreg)
            let wq_bytes = int64(wq_words) * 4l
            let ws_bytes = int64(ws_words_n) * 4l
            let xq_bytes = int64(nsb) * 256l
            let xs_bytes = int64(nsb) * 4l
            let rg_bytes = int64(nreg) * 8l
            let y_bytes = int64(rows) * 4l
            let oq_bytes = int64(rows)
            let os_bytes = int64(nblk) * 4l
            let wqd = make_device_buf(wq_bytes)
            let wsd = make_device_buf(ws_bytes)
            let xqd = make_device_buf(xq_bytes)
            let xsd = make_device_buf(xs_bytes)
            let rgd = make_device_buf(rg_bytes)
            let rg3d = make_device_buf(rg_bytes)
            let yd = make_device_buf(y_bytes)
            let oqd = make_device_buf(oq_bytes)
            let osd = make_device_buf(os_bytes)
            var host = make_host_buf(oq_bytes + os_bytes + y_bytes, true, [cached = true])
            var oq : array<int8>
            var os : array<float>
            var y_cls : array<float>
            oq |> resize(rows)
            os |> resize(nblk)
            y_cls |> resize(rows)
            unsafe {
                upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
                upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                upload_region_at(rgd, 0l, addr<uint8 const?>(regs[0]), rg_bytes)
                upload_region_at(rg3d, 0l, addr<uint8 const?>(regs3[0]), rg_bytes)
                var s_cls = set_kq_gemv_k4_gu_cls(fixed_array(wqd, wsd, rgd, xqd, xsd, yd, kq_grid_dev(), rg3d, oqd, osd),
                    fixed_array(wq_bytes, ws_bytes, rg_bytes, xq_bytes, xs_bytes, y_bytes, KQ_GRID_BYTES, rg_bytes, oq_bytes, os_bytes),
                    fixed_array(1u, 2u, 4u, 8u, 16u, 32u, 0u, 64u, 128u, 128u))
                for (lanes_per_row in fixed_array(0u, 16u, 8u)) {
                    nan_sentinel_upload(oqd, oq_bytes)
                    nan_sentinel_upload(osd, os_bytes)
                    nan_sentinel_upload(yd, y_bytes)
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = GemvArgs(n = uint(nsb * 256), d = uint(d), nreg = uint(nreg), ybase = 0u, lanes_per_row = lanes_per_row, act = ACT_GELU)
                    enc_kq_gemv_k4_gu_cls(raw2, h2, s_cls, pc, int64(nblk))
                    vhz_dep(raw2, h2, 32u | 128u, 0u, true)
                    cmd_copy_whole(raw2, oqd, host.buf, oq_bytes)
                    cmd_copy_range(raw2, osd, 0l, host.buf, oq_bytes, os_bytes)
                    cmd_copy_range(raw2, yd, 0l, host.buf, oq_bytes + os_bytes, y_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(oq[0]), host.mapped, oq_bytes)
                    memcpy(addr<void?>(os[0]), reinterpret<void?>(intptr(host.mapped) + uint64(oq_bytes)), os_bytes)
                    memcpy(addr<void?>(y_cls[0]), reinterpret<void?>(intptr(host.mapped) + uint64(oq_bytes + os_bytes)), y_bytes)
                    let badg = mismatch_approx(y_cls, y_g)
                    var bads = 0
                    var badq = 0
                    var pbad = 0
                    for (blk in range(nblk)) {
                        var m = 0.0
                        for (e in range(32)) {
                            m = max(m, abs(act_mul_ref(y_g[blk * 32 + e], y_u[blk * 32 + e], true)))
                        }
                        let d_ref = m / 127.0
                        if (abs(os[blk] - d_ref) > 2e-3 * max(d_ref, 1e-6)) {
                            bads++
                        }
                        for (e in range(32)) {
                            let a = act_mul_ref(y_g[blk * 32 + e], y_u[blk * 32 + e], true)
                            let deq = float(oq[blk * 32 + e]) * os[blk]
                            if (abs(deq - a) > d_ref + 2e-3 * abs(a)) {
                                badq++
                            }
                            if (e == 0 && abs(deq - (a + 3.0 * d_ref + 1.0)) <= d_ref + 2e-3 * abs(a)) {   // the control: the expectation pushed three quant steps must miss
                                pbad++
                            }
                        }
                    }
                    let lanes = lanes_per_row == 0u ? g_gpu.subgroup_size : int64(lanes_per_row)
                    let tag = "k4 gu {nreg} x {d} at {lanes} lanes per row"
                    t |> success(badg == 0, "{tag}: the gate rows match the family's oracle ({badg} of {rows} off)")
                    t |> success(bads == 0, "{tag}: the block scales match ({bads} of {nblk} off)")
                    t |> success(badq == 0, "{tag}: the dequantized hidden rows sit within a quant step ({badq} of {rows} off)")
                    t |> success(pbad == 0, "{tag}: the bar reds a poisoned oracle element in every block ({pbad} passed)")
                    if (lanes_per_row == 16u) {   //! the three-kernel path on the same buffers at the served lane split: its hidden rows byte for byte
                        let yud = make_device_buf(y_bytes)
                        let oq2d = make_device_buf(oq_bytes)
                        let os2d = make_device_buf(os_bytes)
                        nan_sentinel_upload(yud, y_bytes)
                        nan_sentinel_upload(oq2d, oq_bytes)
                        nan_sentinel_upload(os2d, os_bytes)
                        var s_g = gemv_cls_set(fmt, fixed_array(wqd, wsd, rgd, xqd, xsd, yd), fixed_array(wq_bytes, ws_bytes, rg_bytes, xq_bytes, xs_bytes, y_bytes), fixed_array(1u, 2u, 4u, 8u, 16u, 32u))
                        var s_u = gemv_cls_set(fmt, fixed_array(wqd, wsd, rg3d, xqd, xsd, yud), fixed_array(wq_bytes, ws_bytes, rg_bytes, xq_bytes, xs_bytes, y_bytes), fixed_array(1u, 2u, 64u, 8u, 16u, 256u))
                        var s_a = set_actrq_cls(fixed_array(yd, yud, oq2d, os2d), fixed_array(y_bytes, y_bytes, oq_bytes, os_bytes), fixed_array(32u, 256u, 512u, 512u))
                        var raw3 = alloc_cmd()
                        vk_check(vkBeginCommandBuffer(raw3, begin), null)
                        var h3 : VkHaz
                        var pc2 = GemvArgs(n = uint(nsb * 256), d = uint(d), nreg = uint(nreg), ybase = 0u, lanes_per_row = 16u)
                        enc_kq_gemv_k4_cls(raw3, h3, s_g, pc2, gemv_wgs(int64(rows), 16u))
                        enc_kq_gemv_k4_cls(raw3, h3, s_u, pc2, gemv_wgs(int64(rows), 16u))
                        var pca = ActArgs(nelem = uint(rows), act = ACT_GELU, nblk = uint(nblk))
                        enc_q8_actrq_cls(raw3, h3, s_a, pca, (int64(rows) / 4l + 255l) / 256l)
                        vhz_dep(raw3, h3, 512u, 0u, true)
                        cmd_copy_whole(raw3, oq2d, host.buf, oq_bytes)
                        cmd_copy_range(raw3, os2d, 0l, host.buf, oq_bytes, os_bytes)
                        vk_check(vkEndCommandBuffer(raw3), null)
                        submit_wait(raw3)
                        var oq2 : array<int8>
                        var os2 : array<float>
                        oq2 |> resize(rows)
                        os2 |> resize(nblk)
                        memcpy(addr<void?>(oq2[0]), host.mapped, oq_bytes)
                        memcpy(addr<void?>(os2[0]), reinterpret<void?>(intptr(host.mapped) + uint64(oq_bytes)), os_bytes)
                        var dq = 0
                        var dqmax = 0
                        for (e in range(rows)) {
                            let dd = abs(int(oq[e]) - int(oq2[e]))
                            dq += dd != 0 ? 1 : 0
                            dqmax = max(dqmax, dd)
                        }
                        var ds = 0
                        for (b in range(nblk)) {
                            ds += os[b] != os2[b] ? 1 : 0
                        }
                        t |> success(dq == 0 && ds == 0, "{tag}: the hidden rows match the three-kernel path byte for byte ({dq} of {rows} quants off, at most {dqmax}; {ds} of {nblk} scales off)")
                        delete oq2
                        delete os2
                    }
                }
            }
            delete wqh
            delete wsh
            delete xqh
            delete xsh
            delete regs
            delete regs3
            delete y_g
            delete y_u
            delete oq
            delete os
            delete y_cls
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
            var gh : array<float>
            var uh : array<float>
            gh |> resize(nelem)
            uh |> resize(nelem)
            for (i in range(nelem)) {
                gh[i] = float((i * 29) % 61) * 0.21 - 6.0
                uh[i] = float((i * 11) % 47) * 0.17 - 4.0
            }
            let e_bytes = int64(nelem) * 4l
            let gd = make_device_buf(e_bytes)
            let ud = make_device_buf(e_bytes)
            unsafe {
                upload_region_at(gd, 0l, addr<uint8 const?>(gh[0]), e_bytes)
                upload_region_at(ud, 0l, addr<uint8 const?>(uh[0]), e_bytes)
            }
            // arm 0 = q8 (32-blocks), arm 1 = q8k (256-superblocks); arms 2 and 3 the same under the clamped swiglu (gpt-oss's act code on the unbiased kernels)
            for (arm in range(4)) {
                let q8k = (arm & 1) == 1
                let act = arm >= 2 ? ACT_OAI : ACT_SILU
                let nblk = q8k ? nelem / 256 : nelem / 32
                let q_bytes = int64(nelem)          // 1 byte per quant either way
                let s_bytes = int64(nblk) * 4l
                let hq2 = make_device_buf(q_bytes)
                let hs2 = make_device_buf(s_bytes)
                var hostq = make_host_buf(q_bytes, true, [cached = true])
                var hosts = make_host_buf(s_bytes, true, [cached = true])
                var q_cls : array<uint>
                var s_cls : array<float>
                q_cls |> resize(nelem / 4)
                s_cls |> resize(nblk)
                let wgs = 1l   // 256 threads cover both shapes at nelem = 1024
                unsafe {
                    // the class kernel: gate@0, up@1, outq@2, outs@3
                    var sc = set_actrq_cls(fixed_array(gd, ud, hq2, hs2),
                        fixed_array(e_bytes, e_bytes, q_bytes, s_bytes),
                        fixed_array(16u, 1u, 4u, 8u))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = ActArgs(nelem = uint(nelem), act = act, nblk = uint(nblk))
                    if (q8k) {
                        enc_q8k_actrq_cls(raw2, h2, sc, pc, wgs)
                    } else {
                        enc_q8_actrq_cls(raw2, h2, sc, pc, wgs)
                    }
                    vhz_dep(raw2, h2, 4u | 8u, 0u, true)
                    cmd_copy_whole(raw2, hq2, hostq.buf, q_bytes)
                    cmd_copy_whole(raw2, hs2, hosts.buf, s_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(q_cls[0]), hostq.mapped, q_bytes)
                    memcpy(addr<void?>(s_cls[0]), hosts.mapped, s_bytes)
                }
                var q_ref : array<uint>
                var s_ref : array<float>
                actrq_oracle(gh, uh, nelem, q8k, act, q_ref, s_ref)
                // exp/tanh differ by ulps between backends: quants compare as bytes +-1, scales approx
                let badq = mismatch_qbytes(q_cls, q_ref, 1)
                let bads = mismatch_approx(s_cls, s_ref)
                t |> success(badq == 0 && bads == 0, "{q8k ? "q8k" : "q8"} actrq{act == ACT_OAI ? " under the clamped swiglu" : ""} matches the CPU oracle ({badq} bytes / {bads} scales off)")
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
                var gb : array<float>
                var ub : array<float>
                var gh2 : array<float>
                var uh2 : array<float>
                gb |> resize(ne * brow)
                ub |> resize(ne * brow)
                gh2 |> resize(nb)
                uh2 |> resize(nb)
                for (i in range(ne * brow)) {
                    gb[i] = float((i * 13) % 31) * 0.3 - 4.5   //! biases past the 7 clamp on some elements: the clamps must engage
                    ub[i] = float((i * 17) % 29) * 0.4 - 5.6
                }
                for (e in range(nb)) {
                    let ex = int(rex[e / brow])
                    gh2[e] = gh[e] + gb[ex * brow + e % brow]
                    uh2[e] = uh[e] + ub[ex * brow + e % brow]
                }
                let b_bytes = int64(ne * brow) * 4l
                let gbd = make_device_buf(b_bytes)
                let ubd = make_device_buf(b_bytes)
                let rexd = make_device_buf(int64(nrow) * 4l)
                let hq2 = make_device_buf(int64(nb))
                let hs2 = make_device_buf(int64(nb / 32) * 4l)
                let hd2 = make_device_buf(int64(nb) * 2l)
                var hostq = make_host_buf(int64(nb), true, [cached = true])
                var hosts = make_host_buf(int64(nb / 32) * 4l, true, [cached = true])
                var hosth = make_host_buf(int64(nb) * 2l, true, [cached = true])
                var q_cls : array<uint>
                var s_cls : array<float>
                var h_cls : array<uint>
                q_cls |> resize(nb / 4)
                s_cls |> resize(nb / 32)
                h_cls |> resize(nb / 2)
                var pc = ActArgs(nelem = uint(nb), act = ACT_OAI, nblk = uint(nb / 32), bias_on = 1u, gboff = 0u, uboff = 0u, brow = uint(brow))
                unsafe {
                    upload_region_at(gbd, 0l, addr<uint8 const?>(gb[0]), b_bytes)
                    upload_region_at(ubd, 0l, addr<uint8 const?>(ub[0]), b_bytes)
                    upload_region_at(rexd, 0l, addr<uint8 const?>(rex[0]), int64(nrow) * 4l)
                    nan_sentinel_upload(hq2, int64(nb))
                    nan_sentinel_upload(hs2, int64(nb / 32) * 4l)
                    nan_sentinel_upload(hd2, int64(nb) * 2l)
                    var sc = set_q8_actrq_b_cls(fixed_array(gd, ud, hq2, hs2, gbd, ubd, rexd),
                        fixed_array(e_bytes, e_bytes, int64(nb), int64(nb / 32) * 4l, b_bytes, b_bytes, int64(nrow) * 4l),
                        fixed_array(16u, 1u, 4u, 8u, 0u, 0u, 0u))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    enc_q8_actrq_b_cls(raw2, h2, sc, pc, (int64(nb) / 4l + 255l) / 256l)
                    var sc16 = set_actf16_b_cls(fixed_array(gd, ud, hd2, gbd, ubd, rexd),
                        fixed_array(e_bytes, e_bytes, int64(nb) * 2l, b_bytes, b_bytes, int64(nrow) * 4l),
                        fixed_array(16u, 1u, 32u, 0u, 0u, 0u))
                    enc_actf16_b_cls(raw2, h2, sc16, pc, (int64(nb) / 4l + 255l) / 256l)
                    vhz_dep(raw2, h2, 4u | 8u | 32u, 0u, true)
                    cmd_copy_whole(raw2, hq2, hostq.buf, int64(nb))
                    cmd_copy_whole(raw2, hs2, hosts.buf, int64(nb / 32) * 4l)
                    cmd_copy_whole(raw2, hd2, hosth.buf, int64(nb) * 2l)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(q_cls[0]), hostq.mapped, int64(nb))
                    memcpy(addr<void?>(s_cls[0]), hosts.mapped, int64(nb / 32) * 4l)
                    memcpy(addr<void?>(h_cls[0]), hosth.mapped, int64(nb) * 2l)
                }
                var q_ref : array<uint>
                var s_ref : array<float>
                actrq_oracle(gh2, uh2, nb, false, ACT_OAI, q_ref, s_ref)
                let badq = mismatch_qbytes(q_cls, q_ref, 1)
                let bads = mismatch_approx(s_cls, s_ref)
                t |> success(badq == 0 && bads == 0, "the biased q8 actrq under the clamped swiglu matches the oracle over the biased rows ({badq} bytes / {bads} scales off)")
                var h_ref : array<float>
                var h_got : array<float>
                h_ref |> resize(nb)
                h_got |> resize(nb)
                for (e in range(nb)) {
                    h_ref[e] = float(float16(act_mul_ref(gh2[e], uh2[e], ACT_OAI)))
                }
                for (i in range(nb / 2)) {
                    let pr = unpackHalf2x16(h_cls[i])
                    h_got[2 * i] = pr.x
                    h_got[2 * i + 1] = pr.y
                }
                let badh = mismatch_bars(h_got, h_ref, 2.0e-3, 2.0e-3 * max_abs(h_ref))
                t |> success(badh == 0, "the biased f16 act under the clamped swiglu matches the oracle within the half's rounding ({badh} of {nb} off)")
                //! the controls: an unbiased oracle must red both forms (the bias rows carry the signal)
                var q_nb : array<uint>
                var s_nb : array<float>
                actrq_oracle(gh, uh, nb, false, ACT_OAI, q_nb, s_nb)
                t |> success(mismatch_qbytes(q_cls, q_nb, 1) > 0, "biased actrq control: the unbiased oracle reds the compare")
                h_ref[7] += 0.5
                t |> success(mismatch_bars(h_got, h_ref, 2.0e-3, 2.0e-3 * max_abs(h_ref)) == 1, "biased f16 act control: the bar reds one poisoned element")
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
                var rows : array<float>
                var accs : array<float>
                sidx |> resize(npos * k)
                sw |> resize(npos * k)
                rows |> resize(nrows * n)
                accs |> resize(npos * n)
                for (i in range(npos * k)) {
                    sidx[i] = hash_word(uint(i)) % uint(nrows)
                    sw[i] = 0.125 + float(i % 5) * 0.25
                }
                for (i in range(nrows * n)) {
                    rows[i] = float((i * 7) % 93) * 0.13 - 5.5
                }
                for (i in range(npos * n)) {
                    accs[i] = 0.0
                }
                let si_bytes = int64(npos * k) * 4l
                let ro_bytes = int64(nrows * n) * 4l
                let ac_bytes = int64(npos * n) * 4l
                let sid = make_device_buf(si_bytes)
                let swd = make_device_buf(si_bytes)
                let rod = make_device_buf(ro_bytes)
                let ac2 = make_device_buf(ac_bytes)
                var hosta = make_host_buf(ac_bytes, true, [cached = true])
                var a_cls : array<float>
                a_cls |> resize(npos * n)
                unsafe {
                    upload_region_at(sid, 0l, addr<uint8 const?>(sidx[0]), si_bytes)
                    upload_region_at(swd, 0l, addr<uint8 const?>(sw[0]), si_bytes)
                    upload_region_at(rod, 0l, addr<uint8 const?>(rows[0]), ro_bytes)
                    upload_region_at(ac2, 0l, addr<uint8 const?>(accs[0]), ac_bytes)
                    var sc = set_combine_cls(fixed_array(ac2, sid, swd, rod),
                        fixed_array(ac_bytes, si_bytes, si_bytes, ro_bytes),
                        fixed_array(1u, 2u, 4u, 8u))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = CombineArgs(n = uint(n), k = uint(k), r0 = 0u, r1 = uint(nrows))
                    enc_combine_cls(raw2, h2, sc, pc, int64(npos))
                    vhz_dep(raw2, h2, 1u, 0u, true)
                    cmd_copy_whole(raw2, ac2, hosta.buf, ac_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(a_cls[0]), hosta.mapped, ac_bytes)
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
                let bad = mismatch_approx(a_cls, a_ref)
                t |> success(bad == 0, "combine matches the CPU oracle ({bad} of {npos * n} off)")
                delete sidx
                delete sw
                delete rows
                delete accs
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

[test]
def test_vkd_rope_family(t0 : T?) {
    t0 |> run("rope_kv_store + qk_rms class kernels == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_rope_kv_cls() && ensure_qk_rms_cls())) {
                t |> skip("no Vulkan device")
                return
            }
            let hs = 64
            let half = hs / 2
            let nh = 4
            let nkvh = 2
            let qd = nh * hs
            let kvd = nkvh * hs
            let begin = VkCommandBufferBeginInfo()
            // ===== rope + KV store =====
            {
                let npairs = (nh + nkvh) * half
                let kpair0 = nh * half
                var qh : array<float>
                var kvh : array<float>
                var csh : array<float>
                qh |> resize(qd)
                kvh |> resize(qd + 2 * kvd)
                csh |> resize(hs)
                for (i in range(qd)) {
                    qh[i] = float((i * 23) % 71) * 0.09 - 3.0
                }
                for (i in range(qd + 2 * kvd)) {
                    kvh[i] = float((i * 19) % 83) * 0.07 - 2.9
                }
                for (i in range(half)) {
                    csh[i] = 0.3 + float(i % 9) * 0.07          // cos row
                    csh[half + i] = 0.1 + float(i % 7) * 0.11   // sin row
                }
                let bpad = 64   // the bias rows sit past a pad, so boff is exercised off its default
                var bh : array<float>
                bh |> resize(bpad + qd + 2 * kvd)
                for (i in range(bpad + qd + 2 * kvd)) {
                    bh[i] = i < bpad ? 55.0 : float((i * 13) % 29) * 0.05 - 0.7
                }
                let b_bytes = long_length(bh) * 4l
                let q_bytes = int64(qd) * 4l
                let kv_bytes = int64(qd + 2 * kvd) * 4l
                let m_bytes = int64(kvd) * 4l
                let cs_bytes = int64(hs) * 4l
                let kvd_dev = make_device_buf(kv_bytes)
                let csd = make_device_buf(cs_bytes)
                let bd = make_device_buf(b_bytes)
                let q2 = make_device_buf(q_bytes)
                let km2 = make_device_buf(m_bytes)
                let vm2 = make_device_buf(m_bytes)
                var host = make_host_buf(kv_bytes, true, [cached = true])
                var cls_q : array<float>
                var cls_k : array<float>
                var cls_v : array<float>
                cls_q |> resize(qd)
                cls_k |> resize(kvd)
                cls_v |> resize(kvd)
                let wgs = int64((npairs + 255) / 256)
                unsafe {
                    upload_region_at(kvd_dev, 0l, addr<uint8 const?>(kvh[0]), kv_bytes)
                    upload_region_at(csd, 0l, addr<uint8 const?>(csh[0]), cs_bytes)
                    upload_region_at(q2, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                    upload_region_at(bd, 0l, addr<uint8 const?>(bh[0]), b_bytes)
                    //! the class kernel: q@0, kvsrc@1, kmir@2, vmir@3, cossin@4, tok@5 (pos 0 -> row 0), bias@6
                    var tokb = make_host_buf(16l, true)
                    var tm = reinterpret<TokMeta?>(tokb.mapped)
                    tm.pos = 0u
                    tm.cnt = 0u
                    tm.mirbase = 0u
                    var sc = set_rope_kv_cls(fixed_array(q2, kvd_dev, km2, vm2, csd, tokb.buf, bd),
                        fixed_array(q_bytes, kv_bytes, m_bytes, m_bytes, cs_bytes, 16l, b_bytes),
                        fixed_array(16u, 1u, 2u, 4u, 8u, 0u, 0u))
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = RopeKvArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(half),
                        neox = 1u, kpair0 = uint(kpair0), npairs = uint(npairs), layerbase = 0u, boff = uint(bpad), hasb = 1u)
                    enc_rope_kv_cls(raw2, h2, sc, pc, wgs)
                    vhz_dep(raw2, h2, 2u | 4u | 16u, 0u, true)
                    cmd_copy_whole(raw2, q2, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(cls_q[0]), host.mapped, q_bytes)
                    var raw2b = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2b, begin), null)
                    cmd_copy_whole(raw2b, km2, host.buf, m_bytes)
                    vk_check(vkEndCommandBuffer(raw2b), null)
                    submit_wait(raw2b)
                    memcpy(addr<void?>(cls_k[0]), host.mapped, m_bytes)
                    var raw2c = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2c, begin), null)
                    cmd_copy_whole(raw2c, vm2, host.buf, m_bytes)
                    vk_check(vkEndCommandBuffer(raw2c), null)
                    submit_wait(raw2c)
                    memcpy(addr<void?>(cls_v[0]), host.mapped, m_bytes)
                }
                //! oracle: bias first, then the sequential NEOX (j, j+half) rotation; v is raw plus bias — the approx bars ride out the device's fma contraction
                var ref_q : array<float>
                var ref_k : array<float>
                var ref_v : array<float>
                ref_q |> resize(qd)
                ref_k |> resize(kvd)
                ref_v |> resize(kvd)
                for (i in range(qd)) {
                    ref_q[i] = qh[i] + bh[bpad + i]
                }
                for (gid in range(npairs)) {
                    let is_k = gid >= kpair0
                    let lp = is_k ? gid - kpair0 : gid
                    let hh = lp / half
                    let j = lp % half
                    let fcr = csh[j]
                    let fci = csh[half + j]
                    if (is_k) {
                        let a0 = kvh[qd + hh * hs + j] + bh[bpad + qd + hh * hs + j]
                        let a1 = kvh[qd + hh * hs + j + half] + bh[bpad + qd + hh * hs + j + half]
                        ref_k[hh * hs + j] = a0 * fcr - a1 * fci
                        ref_k[hh * hs + j + half] = a0 * fci + a1 * fcr
                        ref_v[hh * hs + j] = kvh[qd + kvd + hh * hs + j] + bh[bpad + qd + kvd + hh * hs + j]
                        ref_v[hh * hs + j + half] = kvh[qd + kvd + hh * hs + j + half] + bh[bpad + qd + kvd + hh * hs + j + half]
                    } else {
                        let a0 = qh[hh * hs + j] + bh[bpad + hh * hs + j]
                        let a1 = qh[hh * hs + j + half] + bh[bpad + hh * hs + j + half]
                        ref_q[hh * hs + j] = a0 * fcr - a1 * fci
                        ref_q[hh * hs + j + half] = a0 * fci + a1 * fcr
                    }
                }
                let bad = mismatch_approx(cls_q, ref_q) + mismatch_approx(cls_k, ref_k) + mismatch_approx(cls_v, ref_v)
                t |> success(bad == 0, "rope_kv_store with the q/k/v bias matches the CPU oracle ({bad} elements off)")
                delete qh
                delete kvh
                delete csh
                delete bh
                delete ref_q
                delete ref_k
                delete ref_v
                delete cls_q
                delete cls_k
                delete cls_v
            }
            // ===== qk_rms =====
            {
                let rows = 2
                let qstride = qd
                let kstride = kvd
                let eps = 1e-5
                var qh : array<float>
                var kh : array<float>
                var wh : array<float>
                var eh : array<float>
                qh |> resize(rows * qd)
                kh |> resize(rows * kvd)
                wh |> resize(2 * hs)
                eh |> resize(1)
                for (i in range(rows * qd)) {
                    qh[i] = float((i * 31) % 67) * 0.08 - 2.5
                }
                for (i in range(rows * kvd)) {
                    kh[i] = float((i * 43) % 59) * 0.06 - 1.7
                }
                for (i in range(2 * hs)) {
                    wh[i] = 0.4 + float(i % 13) * 0.06
                }
                eh[0] = eps
                let q_bytes = int64(rows * qd) * 4l
                let k_bytes = int64(rows * kvd) * 4l
                let w_bytes = int64(2 * hs) * 4l
                let q2 = make_device_buf(q_bytes)
                let k2 = make_device_buf(k_bytes)
                let wd = make_device_buf(w_bytes)
                var host = make_host_buf(q_bytes, true, [cached = true])
                var cls_q : array<float>
                var cls_k : array<float>
                cls_q |> resize(rows * qd)
                cls_k |> resize(rows * kvd)
                let wgs = int64(rows * (nh + nkvh))
                unsafe {
                    upload_region_at(q2, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                    upload_region_at(k2, 0l, addr<uint8 const?>(kh[0]), k_bytes)
                    upload_region_at(wd, 0l, addr<uint8 const?>(wh[0]), w_bytes)
                    // the class kernel: q@0, k@1, weights@2
                    var sc = set_qk_rms_cls(fixed_array(q2, k2, wd),
                        fixed_array(q_bytes, k_bytes, w_bytes),
                        fixed_array(1u, 16u, 4u))
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = QkRmsArgs(hs = uint(hs), nh = uint(nh), nkvh = uint(nkvh),
                        qstride = uint(qstride), kstride = uint(kstride), kbase = 0u,
                        qwoff = 0u, kwoff = uint(hs), eps = eps)
                    enc_qk_rms_cls(raw2, h2, sc, pc, wgs)
                    vhz_dep(raw2, h2, 1u | 16u, 0u, true)
                    cmd_copy_whole(raw2, q2, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(cls_q[0]), host.mapped, q_bytes)
                    var raw2b = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2b, begin), null)
                    cmd_copy_whole(raw2b, k2, host.buf, k_bytes)
                    vk_check(vkEndCommandBuffer(raw2b), null)
                    submit_wait(raw2b)
                    memcpy(addr<void?>(cls_k[0]), host.mapped, k_bytes)
                }
                // oracle: per-head rms (1/sqrt(mean(v^2)+eps)) times the shared weight row —
                // the device reduces sum-of-squares in tree order, so approx bars
                var ref_q : array<float>
                var ref_k : array<float>
                ref_q |> resize(rows * qd)
                ref_k |> resize(rows * kvd)
                for (r in range(rows)) {
                    for (hh in range(nh + nkvh)) {
                        let is_q = hh < nh
                        let base = is_q ? r * qstride + hh * hs : r * kstride + (hh - nh) * hs
                        let wbase = is_q ? 0 : hs
                        var ss = 0.0
                        for (i in range(hs)) {
                            let v = is_q ? qh[base + i] : kh[base + i]
                            ss += v * v
                        }
                        let inv = 1.0 / sqrt(ss / float(hs) + eps)
                        for (i in range(hs)) {
                            if (is_q) {
                                ref_q[base + i] = wh[wbase + i] * (qh[base + i] * inv)
                            } else {
                                ref_k[base + i] = wh[wbase + i] * (kh[base + i] * inv)
                            }
                        }
                    }
                }
                let bad = mismatch_approx(cls_q, ref_q) + mismatch_approx(cls_k, ref_k)
                t |> success(bad == 0, "qk_rms matches the CPU oracle ({bad} elements off)")
                delete qh
                delete kh
                delete wh
                delete eh
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
            let rows = cnt0 + cnt1
            let wtiles = (d + 31) / 32
            let wgs0 = ((cnt0 + 31) / 32) * wtiles
            let wgs1 = ((cnt1 + 31) / 32) * wtiles
            let wgs = wgs0 + wgs1
            let totblk = 2 * d * nbb
            var wqh : array<uint>
            var wsh : array<uint>
            var xqh : array<uint>
            var xsh : array<float>
            wqh |> resize(totblk * 8)
            wsh |> resize(totblk / 2 + 1)
            xqh |> resize(rows * nbb * 8)
            xsh |> resize(rows * nbb)
            for (i in range(totblk * 8)) {
                wqh[i] = hash_word(uint(i) + 3u)
            }
            for (i in range(totblk / 2 + 1)) {
                wsh[i] = ws_word(uint(i) + 7u)
            }
            for (i in range(rows * nbb * 8)) {
                xqh[i] = hash_word(uint(i) * 13u + 1u)
            }
            for (i in range(rows * nbb)) {
                xsh[i] = 0.375 + float(i % 9) * 0.0625
            }
            let wq_bytes = int64(totblk) * 32l
            let ws_bytes = int64(totblk) * 2l
            let xq_bytes = int64(rows * nbb) * 32l
            let xs_bytes = int64(rows * nbb) * 4l
            let y_bytes = int64(rows * d) * 4l
            let wqd = make_device_buf(wq_bytes)
            let wsd = make_device_buf(ws_bytes)
            let xqd = make_device_buf(xq_bytes)
            let xsd = make_device_buf(xs_bytes)
            let yd2 = make_device_buf(y_bytes)
            var host = make_host_buf(y_bytes, true, [cached = true])
            var y_cls : array<float>
            y_cls |> resize(rows * d)
            // records: [wblk0, row0, cnt, wg0]; region 1's weights follow region 0's stack
            var sched : array<uint>
            sched |> resize(2 * 4 + wgs)
            sched[0] = 0u
            sched[1] = 0u
            sched[2] = uint(cnt0)
            sched[3] = 0u
            sched[4] = uint(d * nbb)
            sched[5] = uint(cnt0)
            sched[6] = uint(cnt1)
            sched[7] = uint(wgs0)
            for (w in range(wgs)) {
                sched[8 + w] = w < wgs0 ? 0u : 1u
            }
            unsafe {
                upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
                upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                let sc_bytes = int64(2 * 4 + wgs) * 4l
                let scd = make_device_buf(sc_bytes)
                upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
                // the class kernel: records at 0, map at map_off
                var sc = set_q8_batch_cls(fixed_array(wqd, wsd, scd, xqd, xsd, yd2),
                    fixed_array(wq_bytes, ws_bytes, sc_bytes, xq_bytes, xs_bytes, y_bytes),
                    fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                var raw2 = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                var h2 : VkHaz
                var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(2 * 4))
                enc_q8_batch_cls(raw2, h2, sc, pc, int64(wgs))
                vhz_dep(raw2, h2, 32u, 0u, true)
                cmd_copy_whole(raw2, yd2, host.buf, y_bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
            }
            var y_ref : array<float>
            y_ref |> resize(rows * d)
            q8_gemm_oracle(wqh, wsh, xqh, xsh, sched, 2, n, d, y_ref)
            let bad = mismatch_approx(y_cls, y_ref)
            t |> success(bad == 0, "q8 batch matches the CPU oracle ({bad} of {rows * d} off)")
            delete sched
            delete y_ref
            delete wqh
            delete wsh
            delete xqh
            delete xsh
            delete y_cls
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_kq_batch_family(t0 : T?) {
    t0 |> run("kq batch tile class family == the CPU oracle (all 13 formats)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_kq_batch_k4_cls() && ensure_kq_batch_q40_cls() && ensure_kq_batch_k5_cls() && ensure_kq_batch_k6_cls() && ensure_kq_batch_iq4xs_cls() && ensure_kq_batch_k3_cls() && ensure_kq_batch_iq3s_cls() && ensure_kq_batch_iq3xxs_cls() && ensure_kq_batch_iq4nl_cls() && ensure_kq_batch_k2_cls() && ensure_kq_batch_iq2s_cls() && ensure_kq_batch_iq2xs_cls() && ensure_kq_batch_iq2xxs_cls())) {
                t |> skip("no Vulkan device, or a kq batch class pipeline failed to build")
                return
            }
            let n = 512               // nsb = 2 superblocks
            let nsb = n / 256
            let d = 40                // 2 w-tiles with an 8-col edge
            let cnt0 = 35
            let cnt1 = 20
            let rows = cnt0 + cnt1
            let wtiles = (d + 31) / 32
            let wgs0 = ((cnt0 + 31) / 32) * wtiles
            let wgs1 = ((cnt1 + 31) / 32) * wtiles
            let wgs = wgs0 + wgs1
            let totsb = 2 * d * nsb   // superblocks across both regions
            let nfmt = 13
            let wstrides = fixed_array(32, 32, 40, 48, 32, 24, 26, 24, 32, 16, 18, 16, 16)   // uint words per superblock: k4 q40 k5 k6 iq4xs k3 iq3s iq3xxs iq4nl k2 iq2s iq2xs iq2xxs
            let pipes = fixed_array(int(KqFmt.k4), int(KqFmt.q40), int(KqFmt.k5), int(KqFmt.k6), int(KqFmt.iq4xs), int(KqFmt.k3), int(KqFmt.iq3s), int(KqFmt.iq3xxs), int(KqFmt.iq4nl), int(KqFmt.k2), int(KqFmt.iq2s), int(KqFmt.iq2xs), int(KqFmt.iq2xxs))
            for (fi in range(nfmt)) {
                let fmt = pipes[fi]
                let wstride = wstrides[fi]
                var wqh : array<uint>
                var wsh : array<uint>
                var xqh : array<uint>
                var xsh : array<float>
                let wswords = totsb * int(kq_dev_ssb_i(fmt) / 4l)
                wqh |> resize(totsb * wstride)
                wsh |> resize(wswords)
                xqh |> resize(rows * nsb * 64)
                xsh |> resize(rows * nsb)
                for (i in range(totsb * wstride)) {
                    wqh[i] = hash_word(uint(i) + uint(fmt) * 131u)
                }
                for (i in range(wswords)) {
                    wsh[i] = ws_word(uint(i) + uint(fmt) * 17u)
                }
                for (i in range(rows * nsb * 64)) {
                    xqh[i] = hash_word(uint(i) * 7u + 2u)
                }
                for (i in range(rows * nsb)) {
                    xsh[i] = 0.3 + float(i % 11) * 0.05
                }
                let wq_bytes = int64(totsb * wstride) * 4l
                let ws_bytes = int64(wswords) * 4l
                let xq_bytes = int64(rows * nsb) * 256l
                let xs_bytes = int64(rows * nsb) * 4l
                let y_bytes = int64(rows * d) * 4l
                let wqd = make_device_buf(wq_bytes)
                let wsd = make_device_buf(ws_bytes)
                let xqd = make_device_buf(xq_bytes)
                let xsd = make_device_buf(xs_bytes)
                let yd2 = make_device_buf(y_bytes)
                var host = make_host_buf(y_bytes, true, [cached = true])
                var y_cls : array<float>
                y_cls |> resize(rows * d)
                var sched : array<uint>
                sched |> resize(2 * 4 + wgs)
                sched[0] = 0u
                sched[1] = 0u
                sched[2] = uint(cnt0)
                sched[3] = 0u
                sched[4] = uint(d * nsb)   // region 1's weights follow region 0's stack (superblock units)
                sched[5] = uint(cnt0)
                sched[6] = uint(cnt1)
                sched[7] = uint(wgs0)
                for (w in range(wgs)) {
                    sched[8 + w] = w < wgs0 ? 0u : 1u
                }
                unsafe {
                    upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                    upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
                    upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                    upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                    let sc_bytes = int64(2 * 4 + wgs) * 4l
                    let scd = make_device_buf(sc_bytes)
                    upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
                    // one family set serves all four formats
                    var sc = set_kq_batch_cls(fixed_array(wqd, wsd, scd, xqd, xsd, yd2),
                        fixed_array(wq_bytes, ws_bytes, sc_bytes, xq_bytes, xs_bytes, y_bytes),
                        fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(2 * 4))
                    if (fmt == int(KqFmt.k4)) {
                        enc_kq_batch_k4_cls(raw2, h2, sc, pc, int64(wgs))
                    } elif (fmt == int(KqFmt.q40)) {
                        enc_kq_batch_q40_cls(raw2, h2, sc, pc, int64(wgs))
                    } elif (fmt == int(KqFmt.k5)) {
                        enc_kq_batch_k5_cls(raw2, h2, sc, pc, int64(wgs))
                    } elif (fmt == int(KqFmt.iq4xs)) {
                        enc_kq_batch_iq4xs_cls(raw2, h2, sc, pc, int64(wgs))
                    } elif (fmt == int(KqFmt.k3)) {
                        enc_kq_batch_k3_cls(raw2, h2, sc, pc, int64(wgs))
                    } elif (fmt == int(KqFmt.iq3s)) {
                        enc_kq_batch_iq3s_cls(raw2, h2, sc, pc, int64(wgs))
                    } elif (fmt == int(KqFmt.iq3xxs)) {
                        enc_kq_batch_iq3xxs_cls(raw2, h2, sc, pc, int64(wgs))
                    } elif (fmt == int(KqFmt.iq4nl)) {
                        enc_kq_batch_iq4nl_cls(raw2, h2, sc, pc, int64(wgs))
                    } elif (fmt == int(KqFmt.k2)) {
                        enc_kq_batch_k2_cls(raw2, h2, sc, pc, int64(wgs))
                    } elif (fmt == int(KqFmt.iq2s)) {
                        enc_kq_batch_iq2s_cls(raw2, h2, sc, pc, int64(wgs))
                    } elif (fmt == int(KqFmt.iq2xs)) {
                        enc_kq_batch_iq2xs_cls(raw2, h2, sc, pc, int64(wgs))
                    } elif (fmt == int(KqFmt.iq2xxs)) {
                        enc_kq_batch_iq2xxs_cls(raw2, h2, sc, pc, int64(wgs))
                    } else {
                        enc_kq_batch_k6_cls(raw2, h2, sc, pc, int64(wgs))
                    }
                    vhz_dep(raw2, h2, 32u, 0u, true)
                    cmd_copy_whole(raw2, yd2, host.buf, y_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
                }
                var y_ref : array<float>
                y_ref |> resize(rows * d)
                kq_gemm_oracle(fmt, wqh, wsh, xqh, xsh, sched, 2, n, d, y_ref)
                let bad = mismatch_approx(y_cls, y_ref)
                t |> success(bad == 0, "kq batch fmt {fmt} matches the CPU oracle ({bad} of {rows * d} off)")
                delete sched
                delete y_ref
                delete wqh
                delete wsh
                delete xqh
                delete xsh
                delete y_cls
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

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
                let up = (v == 0 ? ensure_q8_batch_cmf16_cls()
                    : (v == 1 ? ensure_q8_batch_cmi8_cls()
                    : (v == 2 ? ensure_q8_batch_mm_cls()
                    : (v == 3 ? ensure_q8_batch_mm_a_cls() : ensure_q8_batch_mm_m_cls()))))
                assert(up, "class rail must engage in its coopmat mode")
                ran++
                let nbb = n / 32
                let rows = cnt0 + cnt1
                let wtiles = (d + tile - 1) / tile
                let wgs0 = ((cnt0 + tile - 1) / tile) * wtiles
                let wgs1 = ((cnt1 + tile - 1) / tile) * wtiles
                let wgs = wgs0 + wgs1
                let totblk = 2 * d * nbb
                var wqh : array<uint>
                var wsh : array<uint>
                var xqh : array<uint>
                var xsh : array<float>
                wqh |> resize(totblk * 8)
                wsh |> resize(totblk / 2 + 1)
                xqh |> resize(rows * nbb * 8)
                xsh |> resize(rows * nbb)
                for (i in range(totblk * 8)) {
                    wqh[i] = hash_word(uint(i) + 3u)
                }
                for (i in range(totblk / 2 + 1)) {
                    wsh[i] = ws_word(uint(i) + 7u)
                }
                for (i in range(rows * nbb * 8)) {
                    xqh[i] = hash_word(uint(i) * 13u + 1u)
                }
                for (i in range(rows * nbb)) {
                    // 16x cooler than the sdot4 arms: the mm family ACCUMULATES in f16, and the
                    // hot form's |y| ~7e4 overflows f16's 65504 to inf
                    xsh[i] = (0.375 + float(i % 9) * 0.0625) * 0.0625
                }
                let wq_bytes = int64(totblk) * 32l
                let ws_bytes = int64(totblk) * 2l
                let xq_bytes = int64(rows * nbb) * 32l
                let xs_bytes = int64(rows * nbb) * 4l
                let y_bytes = int64(rows * d) * 4l
                let wqd = make_device_buf(wq_bytes)
                let wsd = make_device_buf(ws_bytes)
                let xqd = make_device_buf(xq_bytes)
                let xsd = make_device_buf(xs_bytes)
                let yd2 = make_device_buf(y_bytes)
                var host = make_host_buf(y_bytes, true, [cached = true])
                var y_cls : array<float>
                y_cls |> resize(rows * d)
                var sched : array<uint>
                sched |> resize(2 * 4 + wgs)
                sched[0] = 0u
                sched[1] = 0u
                sched[2] = uint(cnt0)
                sched[3] = 0u
                sched[4] = uint(d * nbb)
                sched[5] = uint(cnt0)
                sched[6] = uint(cnt1)
                sched[7] = uint(wgs0)
                for (w in range(wgs)) {
                    sched[8 + w] = w < wgs0 ? 0u : 1u
                }
                unsafe {
                    upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                    upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
                    upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                    upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                    let sc_bytes = int64(2 * 4 + wgs) * 4l
                    let scd = make_device_buf(sc_bytes)
                    upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
                    let bufs = fixed_array(wqd, wsd, scd, xqd, xsd, yd2)
                    let sizes = fixed_array(wq_bytes, ws_bytes, sc_bytes, xq_bytes, xs_bytes, y_bytes)
                    let gbits = fixed_array(1u, 2u, 0u, 4u, 8u, 16u)
                    var sc = (v == 0 ? set_q8_batch_cmf16_cls(bufs, sizes, gbits)
                        : (v == 1 ? set_q8_batch_cmi8_cls(bufs, sizes, gbits)
                        : (v == 2 ? set_q8_batch_mm_cls(bufs, sizes, gbits)
                        : (v == 3 ? set_q8_batch_mm_a_cls(bufs, sizes, gbits)
                        : set_q8_batch_mm_m_cls(bufs, sizes, gbits)))))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(2 * 4))
                    if (v == 0) {
                        enc_q8_batch_cmf16_cls(raw2, h2, sc, pc, int64(wgs))
                    } elif (v == 1) {
                        enc_q8_batch_cmi8_cls(raw2, h2, sc, pc, int64(wgs))
                    } elif (v == 2) {
                        enc_q8_batch_mm_cls(raw2, h2, sc, pc, int64(wgs))
                    } elif (v == 3) {
                        enc_q8_batch_mm_a_cls(raw2, h2, sc, pc, int64(wgs))
                    } else {
                        enc_q8_batch_mm_m_cls(raw2, h2, sc, pc, int64(wgs))
                    }
                    vhz_dep(raw2, h2, 32u, 0u, true)
                    cmd_copy_whole(raw2, yd2, host.buf, y_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
                }
                // the cm kernels stage AND accumulate through f16, so their error scales with
                // the running-sum magnitude (cancelled-small results included) — relative bar
                // plus an absolute floor off the oracle's peak; wiring breaks are peak-sized
                var y_ref : array<float>
                y_ref |> resize(rows * d)
                q8_gemm_oracle(wqh, wsh, xqh, xsh, sched, 2, n, d, y_ref)
                let bad = mismatch_bars(y_cls, y_ref, 2e-2, 4e-3 * max_abs(y_ref))
                to_log(LOG_INFO, "cm batch variant {v}: {bad} of {rows * d} off the oracle, {g_gpu.decvec_on ? "four-wide twin" : "scalar callback"}\n")
                t |> success(bad == 0, "cm batch variant {v} matches the CPU oracle ({bad} of {rows * d} off)")
                delete sched
                delete y_ref
                delete wqh
                delete wsh
                delete xqh
                delete xsh
                delete y_cls
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
            let wtiles = (d + 31) / 32
            let wgs0 = ((cnt0 + 31) / 32) * wtiles
            let wgs1 = ((cnt1 + 31) / 32) * wtiles
            let wgs = wgs0 + wgs1
            let totsb = 2 * d * nsb
            var wqh : array<uint>
            var wsuh : array<uint>
            var xqh : array<uint>
            var xsh : array<float>
            wqh |> resize(totsb * 32)
            wsuh |> resize(totsb * 5)
            xqh |> resize(rows * nsb * 64)
            xsh |> resize(rows * nsb)
            for (i in range(totsb * 32)) {
                wqh[i] = hash_word(uint(i) + 3u)
            }
            for (i in range(totsb * 5)) {
                wsuh[i] = ws_word(uint(i) + 7u)   // d halves live here — every u16 a finite half
            }
            for (i in range(rows * nsb * 64)) {
                xqh[i] = hash_word(uint(i) * 13u + 1u)
            }
            for (i in range(rows * nsb)) {
                xsh[i] = 0.375 + float(i % 9) * 0.0625
            }
            let wq_bytes = int64(totsb) * 128l
            let wsu_bytes = int64(totsb) * 20l
            let xq_bytes = int64(rows * nsb) * 256l
            let xs_bytes = int64(rows * nsb) * 4l
            let y_bytes = int64(rows * d) * 4l
            let wqd = make_device_buf(wq_bytes)
            let wsud = make_device_buf(wsu_bytes)
            let xqd = make_device_buf(xq_bytes)
            let xsd = make_device_buf(xs_bytes)
            let yd1 = make_device_buf(y_bytes)
            let yd2 = make_device_buf(y_bytes)
            var host = make_host_buf(y_bytes, true, [cached = true])
            var y_cls : array<float>
            y_cls |> resize(rows * d)
            var sched : array<uint>
            sched |> resize(2 * 4 + wgs)
            sched[0] = 0u
            sched[1] = 0u
            sched[2] = uint(cnt0)
            sched[3] = 0u
            sched[4] = uint(d * nsb)
            sched[5] = uint(cnt0)
            sched[6] = uint(cnt1)
            sched[7] = uint(wgs0)
            for (w in range(wgs)) {
                sched[8 + w] = w < wgs0 ? 0u : 1u
            }
            unsafe {
                upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                upload_region_at(wsud, 0l, addr<uint8 const?>(wsuh[0]), wsu_bytes)
                upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                let sc_bytes = int64(2 * 4 + wgs) * 4l
                let scd = make_device_buf(sc_bytes)
                upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
                var sc = set_kq_batch_q40_cmf16_cls(fixed_array(wqd, wsud, scd, xqd, xsd, yd2),
                    fixed_array(wq_bytes, wsu_bytes, sc_bytes, xq_bytes, xs_bytes, y_bytes),
                    fixed_array(1u, 2u, 0u, 4u, 8u, 16u))
                var raw2 = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                var h2 : VkHaz
                var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(2 * 4))
                enc_kq_batch_q40_cmf16_cls(raw2, h2, sc, pc, int64(wgs))
                vhz_dep(raw2, h2, 32u, 0u, true)
                cmd_copy_whole(raw2, yd2, host.buf, y_bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
            }
            // f16-staged dequant on the MMA path — relative bar + peak-scaled floor
            var y_ref : array<float>
            y_ref |> resize(rows * d)
            kq_gemm_oracle(int(KqFmt.q40), wqh, wsuh, xqh, xsh, sched, 2, n, d, y_ref)
            let bad = mismatch_bars(y_cls, y_ref, 2e-2, 4e-3 * max_abs(y_ref))
            to_log(LOG_INFO, "q40 cm batch: {bad} of {rows * d} off the oracle, {g_gpu.decvec_on ? "four-wide twin" : "scalar callback"}\n")
            t |> success(bad == 0, "q40 cm batch matches the CPU oracle ({bad} of {rows * d} off)")
            delete sched
            delete y_ref
            delete wqh
            delete wsuh
            delete xqh
            delete xsh
            delete y_cls
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
            var wqh : array<uint>
            var wsh : array<uint>
            q8_planes(576, 17u, 991u, wqh, wsh)   // K off the 256 lattice: the 32-deep k step runs a tail past the last superblock
            var f <- tile_fixture(576, 32, wqh, wsh, 3u, 5u)
            var y_ref <- tile_cell_arms(t, "q8", int(KqFmt.q8), f, [ARM_KHR], @@q8f16_gemm_oracle)
            // the split arm: the first region alone under two 256-aligned k chunks (512 and 64 deep) into the partial
            // planes, then the reduce into the y plane - the k0 / k1 / ybase prologue the KHR stamp shares with the cm2 tiles
            verify(ensure_splitk_reduce_cls(), "the split-k reduce's class rail must engage")
            let nsplit = 2
            let ck = ((f.n + nsplit - 1) / nsplit + 255) & ~255
            let cnt0 = TILE_CELL_ROWS0
            let d = TILE_CELL_D
            let swgs = ((cnt0 + 127) / 128) * ((d + 127) / 128) * nsplit
            var ssched : array<uint>
            ssched |> resize(4 + swgs)
            ssched[2] = uint(cnt0)   // one region at row 0 over the first weight plane; the map stays zero
            let y0_bytes = int64(cnt0 * d) * 4l
            let part_bytes = y0_bytes * int64(nsplit)
            let pd = make_device_buf(part_bytes)
            let yd_s = make_device_buf(y0_bytes)
            var host_s = make_host_buf(y0_bytes, true, [cached = true])
            var y_split : array<float>
            y_split |> resize(cnt0 * d)
            unsafe {
                nan_sentinel_upload(pd, part_bytes)
                nan_sentinel_upload(yd_s, y0_bytes)
                let ssc_bytes = int64(4 + swgs) * 4l
                let sscd = make_device_buf(ssc_bytes)
                upload_region_at(sscd, 0l, addr<uint8 const?>(ssched[0]), ssc_bytes)
                var scs = set_q8_batch_khr_cls(fixed_array(f.wqd, f.wsd, sscd, f.xfd, pd),
                    fixed_array(f.wq_bytes, f.ws_bytes, ssc_bytes, f.xf_bytes, part_bytes), fixed_array(1u, 2u, 0u, 4u, 8u))
                var srd = set_splitk_reduce_cls(fixed_array(pd, yd_s), fixed_array(part_bytes, y0_bytes), fixed_array(8u, 16u))
                var raw3 = alloc_cmd()
                let begin3 = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw3, begin3), null)
                var h3 : VkHaz
                var pcs = BatchArgs(n = uint(f.n), d = uint(d), map_off = 4u, ksplit = uint(ck))
                enc_q8_batch_khr_cls(raw3, h3, scs, pcs, int64(swgs))
                var pcr = SkRedArgs(nelem = uint(cnt0 * d), k = uint(nsplit), ybase = 0u)
                enc_splitk_reduce_cls(raw3, h3, srd, pcr, int64(cnt0 * d + 1023) / 1_024l)
                vhz_dep(raw3, h3, 16u, 0u, true)
                cmd_copy_whole(raw3, yd_s, host_s.buf, y0_bytes)
                vk_check(vkEndCommandBuffer(raw3), null)
                submit_wait(raw3)
                memcpy(addr<void?>(y_split[0]), host_s.mapped, y0_bytes)
            }
            var y_ref0 : array<float>
            y_ref0 |> resize(cnt0 * d)
            for (i in range(cnt0 * d)) {
                y_ref0[i] = y_ref[i]   // the oracle's first region is the split arm's whole answer
            }
            let bad_s = mismatch_bars(y_split, y_ref0, 2e-2, 4e-3 * max_abs(y_ref0))
            t |> success(bad_s == 0, "q8 KHR tile split in two k chunks + the reduce matches the CPU oracle ({bad_s} of {cnt0 * d} off)")
            var y_poison_s := y_split
            y_poison_s[cnt0 * d - 1] += 1.0 + 2.0 * max_abs(y_ref0)
            t |> success(mismatch_bars(y_poison_s, y_ref0, 2e-2, 4e-3 * max_abs(y_ref0)) > 0, "the split arm's bar reds a poisoned element")
            delete y_poison_s
            delete y_ref0
            delete y_split
            delete ssched
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
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            if (!(g_gpu.coopmat_mode == COOPMAT_CM2 && g_gpu.has_coopmat2)) {
                t |> skip("the cm2 l-tile serves only in mode 4 on an NV_coopmat2 device (DASLLAMA_COOPMAT=cm2)")
                return
            }
            var wqh : array<uint>
            var wsh : array<uint>
            q8_planes(576, 17u, 991u, wqh, wsh)   // K off the 256 lattice: two unrolled superblock passes and a 64-deep tail step
            var f <- tile_fixture(576, 32, wqh, wsh, 3u, 5u)
            var y_ref <- tile_cell_arms(t, "q8", int(KqFmt.q8), f, [ARM_CM2_L], @@q8f16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
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
            let n = 576
            let d = TILE_CELL_D
            var wqh : array<uint>
            var wsh : array<uint>
            q8_planes(n, 17u, 991u, wqh, wsh)
            var f <- tile_fixture(n, 32, wqh, wsh, 3u, 5u)
            tile_fixture_upload(f)   // no arm loop here: the pair dispatches the toy stamps itself
            let wtiles = (d + 127) / 128
            let wgs0 = ((TILE_CELL_ROWS0 + 255) / 256) * wtiles
            let wgs = wgs0 + ((TILE_CELL_ROWS1 + 255) / 256) * wtiles
            var host = make_host_buf(f.y_bytes, true, [cached = true])
            var sched : array<uint>
            sched |> resize(2 * 4 + wgs)
            sched[2] = uint(TILE_CELL_ROWS0)
            sched[4] = uint(d * (n / 32))
            sched[5] = uint(TILE_CELL_ROWS0)
            sched[6] = uint(TILE_CELL_ROWS1)
            sched[7] = uint(wgs0)
            for (w in range(wgs)) {
                sched[8 + w] = w < wgs0 ? 0u : 1u
            }
            var y_ref : array<float>
            y_ref |> resize(f.rows * d)
            q8f16_gemm_oracle(f.wqh, f.wsh, f.xfh, sched, 2, n, d, y_ref)
            let floor = 4e-3 * max_abs(y_ref)
            unsafe {
                let sc_bytes = int64(2 * 4 + wgs) * 4l
                let scd = make_device_buf(sc_bytes)
                upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
                for (arm in range(2)) {
                    var y_cls : array<float>
                    y_cls |> resize(f.rows * d)
                    nan_sentinel_upload(f.yd, f.y_bytes)
                    var sc = (arm == 0
                        ? set_toy_q8_cm2_strip_cls(fixed_array(f.wqd, f.wsd, scd, f.xfd, f.yd),
                            fixed_array(f.wq_bytes, f.ws_bytes, sc_bytes, f.xf_bytes, f.y_bytes), fixed_array(1u, 2u, 0u, 4u, 16u))
                        : set_toy_q8_cm2_twin_cls(fixed_array(f.wqd, f.wsd, scd, f.xfd, f.yd),
                            fixed_array(f.wq_bytes, f.ws_bytes, sc_bytes, f.xf_bytes, f.y_bytes), fixed_array(1u, 2u, 0u, 4u, 16u)))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(2 * 4))
                    if (arm == 0) {
                        enc_toy_q8_cm2_strip_cls(raw2, h2, sc, pc, int64(wgs))
                    } else {
                        enc_toy_q8_cm2_twin_cls(raw2, h2, sc, pc, int64(wgs))
                    }
                    vhz_dep(raw2, h2, 16u, 0u, true)
                    cmd_copy_whole(raw2, f.yd, host.buf, f.y_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, f.y_bytes)
                    let bad = mismatch_bars(y_cls, y_ref, 2e-2, floor)
                    let what = arm == 0 ? "stripped (scalar callback)" : (twin_device ? "the four-wide twin" : "the twin stamp on a device without the callback")
                    to_log(LOG_INFO, "cm2 decode pair, {what}: {bad} of {f.rows * d} off the oracle, {g_gpu.decvec_on ? "four-wide twin" : "scalar callback"}\n")
                    t |> success(bad == 0, "{what} matches the CPU oracle ({bad} of {f.rows * d} off)")
                    var y_poison := y_cls   // the bar's control: one element pushed past both bars must red
                    y_poison[0] += 1.0 + 2.0 * max_abs(y_ref)
                    t |> success(mismatch_bars(y_poison, y_ref, 2e-2, floor) > 0, "the pair's bar reds a poisoned element")
                    delete y_poison
                    delete y_cls
                }
            }
            delete sched
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
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            let totsb = TILE_CELL_SB
            var wqh : array<uint>
            var wsuh : array<uint>
            wqh |> resize(totsb * 32)
            wsuh |> resize(totsb * 5)
            for (i in range(totsb * 32)) {
                wqh[i] = hash_word(uint(i) + 29u)     // arbitrary nibble bytes
            }
            for (sb in range(totsb)) {
                let dv = 0.0002 * float(1 + sb % 7)   // cool scales: the f16 acc must stay far from 65504
                let dmv = 0.00005 * float(1 + sb % 5)
                wsuh[sb * 5] = packHalf2x16(float2(dv, dmv))
                for (wi in range(4)) {
                    wsuh[sb * 5 + 1 + wi] = hash_word(uint(sb * 4 + wi) + 613u)   // sc / mn bytes
                }
            }
            var f <- tile_fixture(TILE_CELL_K, 256, wqh, wsuh, 3u, 5u)
            var y_ref <- tile_cell_arms(t, "k4", int(KqFmt.k4), f, tile_arms_all(), @@k4f16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_k5_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q5_K l/m/s/e tiles and the KHR tile == the CPU oracle (5th-bit compose decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            let totsb = TILE_CELL_SB
            var wqh : array<uint>
            var wsuh : array<uint>
            wqh |> resize(totsb * 40)
            wsuh |> resize(totsb * 5)
            for (i in range(totsb * 40)) {
                wqh[i] = hash_word(uint(i) + 29u)     // arbitrary quant bytes
            }
            for (sb in range(totsb)) {
                let dv = 0.0002 * float(1 + sb % 7)   // cool scales: the f16 acc must stay far from 65504
                let dmv = 0.00005 * float(1 + sb % 5)
                wsuh[sb * 5] = packHalf2x16(float2(dv, dmv))
                for (wi in range(4)) {
                    wsuh[sb * 5 + 1 + wi] = hash_word(uint(sb * 4 + wi) + 613u)   // sc / mn bytes
                }
            }
            var f <- tile_fixture(TILE_CELL_K, 256, wqh, wsuh, 3u, 5u)
            var y_ref <- tile_cell_arms(t, "k5", int(KqFmt.k5), f, tile_arms_all(), @@k5f16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_q51_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q5_1 s and e tiles == the CPU oracle (the per-32 plane verbatim: nibble | qh bit, d * q + m off one word)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            let n = 704                //! the 26B-A4B's expert width: 22 blocks, two unrolled 256-passes and a 192 tail
            let totblk = 2 * TILE_CELL_D * (n / 32)
            var wqh : array<uint>
            var wsh : array<uint>
            wqh |> resize(totblk * 5)
            wsh |> resize(totblk)
            for (i in range(totblk * 5)) {
                wqh[i] = hash_word(uint(i) + 37u)     // arbitrary quant bytes and qh bits
            }
            for (b in range(totblk)) {   // cool f16 d, a signed m: the f16 acc must stay far from 65504
                wsh[b] = packHalf2x16(float2(0.0002 * float(1 + b % 7), 0.0001 * float(b % 5) - 0.0002))
            }
            var f <- tile_fixture(n, 32, wqh, wsh, 3u, 5u)
            var y_ref <- tile_cell_arms(t, "q51", int(KqFmt.q51), f, [ARM_CM2_S, ARM_CM2_E, ARM_KHR], @@q51f16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
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
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            let n = 704                //! 22 blocks: eleven 64-deep k steps, two unrolled 256-passes and a 192 tail
            let totblk = 2 * TILE_CELL_D * (n / 32)
            var wqh : array<uint>
            wqh |> resize(totblk * 4)
            for (i in range(totblk * 4)) {
                wqh[i] = hash_word(uint(i) + 53u)     // arbitrary e2m1 codes, every nibble value reached
            }
            var wsh <- mx4_scale_words(totblk)
            var f <- tile_fixture(n, 32, wqh, wsh, 3u, 5u)
            var y_ref <- tile_cell_arms(t, "mx4", int(KqFmt.mx4), f, [ARM_CM2_S, ARM_CM2_E, ARM_KHR], @@mx4f16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_q40_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q4_0 l/m/s/e tiles and the KHR tile == the CPU oracle (per-block f16 d decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            let totsb = TILE_CELL_SB
            var wqh : array<uint>
            var wsuh : array<uint>
            wqh |> resize(totsb * 32)
            wsuh |> resize(totsb * 5)
            for (i in range(totsb * 32)) {
                wqh[i] = hash_word(uint(i) + 29u)     // arbitrary quant bytes
            }
            for (sb in range(totsb)) {
                for (wi in range(4)) {   // 8 per-block f16 d - cool, the f16 acc must stay far from 65504
                    let p = sb * 4 + wi
                    wsuh[sb * 5 + wi] = packHalf2x16(float2(0.0002 * float(1 + p % 7), 0.00013 * float(1 + p % 5)))
                }
                wsuh[sb * 5 + 4] = 0u
            }
            var f <- tile_fixture(TILE_CELL_K, 256, wqh, wsuh, 3u, 5u)
            var y_ref <- tile_cell_arms(t, "q40", int(KqFmt.q40), f, tile_arms_all(), @@q40f16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_k2_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q2_K l/m/s/e tiles and the KHR tile == the CPU oracle (pair-byte nibble scales decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            let totsb = TILE_CELL_SB
            var wqh : array<uint>
            var wsuh : array<uint>
            wqh |> resize(totsb * 16)
            wsuh |> resize(totsb * 5)
            for (i in range(totsb * 16)) {
                wqh[i] = hash_word(uint(i) + 31u)     // arbitrary quant bytes
            }
            for (sb in range(totsb)) {
                wsuh[sb * 5] = packHalf2x16(float2(0.0004 * float(1 + sb % 7), 0.0003 * float(1 + sb % 5)))
                for (wi in range(4)) {   // 16 sc/min nibble-pair bytes
                    wsuh[sb * 5 + 1 + wi] = hash_word(uint(sb * 4 + wi) + 91u)
                }
            }
            var f <- tile_fixture(TILE_CELL_K, 256, wqh, wsuh, 3u, 7u)
            var y_ref <- tile_cell_arms(t, "k2", int(KqFmt.k2), f, tile_arms_all(), @@k2f16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_iq4nl_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ4_NL l/m/s/e tiles and the KHR tile == the CPU oracle (codebook + per-block f16 d decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            let totsb = TILE_CELL_SB
            var wqh : array<uint>
            var wsuh : array<uint>
            wqh |> resize(totsb * 32)
            wsuh |> resize(totsb * 5)
            for (i in range(totsb * 32)) {
                wqh[i] = hash_word(uint(i) + 29u)     // arbitrary quant bytes
            }
            for (sb in range(totsb)) {
                for (wi in range(4)) {   // 8 per-block f16 d - cool, the f16 acc must stay far from 65504
                    let p = sb * 4 + wi
                    wsuh[sb * 5 + wi] = packHalf2x16(float2(0.0002 * float(1 + p % 7), 0.00013 * float(1 + p % 5)))
                }
                wsuh[sb * 5 + 4] = 0u
            }
            var f <- tile_fixture(TILE_CELL_K, 256, wqh, wsuh, 3u, 5u)
            var y_ref <- tile_cell_arms(t, "iq4nl", int(KqFmt.iq4nl), f, tile_arms_all(), @@iq4nlf16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_iq4xs_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ4_XS l/m/s/e tiles and the KHR tile == the CPU oracle (workgroup-staged codebook decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            let totsb = TILE_CELL_SB
            var wqh : array<uint>
            var wsuh : array<uint>
            wqh |> resize(totsb * 32)
            wsuh |> resize(totsb * 2)
            for (i in range(totsb * 32)) {
                wqh[i] = hash_word(uint(i) + 29u)     // arbitrary quant bytes
            }
            for (sb in range(totsb)) {
                wsuh[sb * 2] = packHalf2x16(float2(0.0002 * float(1 + sb % 7), 0.0)) | (hash_word(uint(sb * 2) + 613u) << 16u)   //! cool d | the row's high bits
                wsuh[sb * 2 + 1] = hash_word(uint(sb * 2 + 1) + 613u)   //! the eight 4-bit sub-scale fields
            }
            var f <- tile_fixture(TILE_CELL_K, 256, wqh, wsuh, 3u, 5u)
            var y_ref <- tile_cell_arms(t, "iq4xs", int(KqFmt.iq4xs), f, tile_arms_all(), @@iq4xsf16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_k3_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q3_K l/m/s/e tiles and the KHR tile == the CPU oracle (2-bit + hmask compose decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            let totsb = TILE_CELL_SB
            var wqh : array<uint>
            var wsuh : array<uint>
            wqh |> resize(totsb * 24)
            wsuh |> resize(totsb * 5)
            for (i in range(totsb * 24)) {
                wqh[i] = hash_word(uint(i) + 29u)     // arbitrary quant bytes
            }
            for (sb in range(totsb)) {
                for (wi in range(4)) {
                    wsuh[sb * 5 + wi] = hash_word(uint(sb * 4 + wi) + 811u)   // signed sub-scale bytes
                }
                wsuh[sb * 5 + 4] = packHalf2x16(float2(0.0002 * float(1 + sb % 7), 0.0))   // cool d
            }
            var f <- tile_fixture(TILE_CELL_K, 256, wqh, wsuh, 3u, 5u)
            var y_ref <- tile_cell_arms(t, "k3", int(KqFmt.k3), f, tile_arms_all(), @@k3f16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_iq3s_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ3_S l/m/s/e tiles and the KHR tile == the CPU oracle (workgroup-staged grid decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            let totsb = TILE_CELL_SB
            var wqh : array<uint>
            var wsuh : array<uint>
            wqh |> resize(totsb * 26)
            wsuh |> resize(totsb * 2)
            for (i in range(totsb * 26)) {
                wqh[i] = hash_word(uint(i) + 29u)     // arbitrary quant bytes
            }
            for (sb in range(totsb)) {
                wsuh[sb * 2] = packHalf2x16(float2(0.0002 * float(1 + sb % 7), 0.0)) | (hash_word(uint(sb * 2) + 613u) << 16u)   //! cool d | the row's high bits
                wsuh[sb * 2 + 1] = hash_word(uint(sb * 2 + 1) + 613u)   //! the eight 4-bit sub-scale fields
            }
            var f <- tile_fixture(TILE_CELL_K, 256, wqh, wsuh, 3u, 5u)
            var y_ref <- tile_cell_arms(t, "iq3s", int(KqFmt.iq3s), f, tile_arms_all(), @@iq3sf16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_iq2s_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ2_S l/m/s/e tiles and the KHR tile == the CPU oracle (workgroup-staged u64 grid decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            let totsb = TILE_CELL_SB
            var wqh : array<uint>
            var wsuh : array<uint>
            wqh |> resize(totsb * 18)
            wsuh |> resize(totsb * 5)
            for (i in range(totsb * 18)) {
                wqh[i] = hash_word(uint(i) + 29u)     // arbitrary quant bytes
            }
            for (sb in range(totsb)) {
                wsuh[sb * 5] = packHalf2x16(float2(0.0002 * float(1 + sb % 7), 0.0))   // cool d8
                wsuh[sb * 5 + 1] = hash_word(uint(sb * 4) + 613u)   // 16 unsigned strip bytes
                wsuh[sb * 5 + 2] = hash_word(uint(sb * 4 + 1) + 613u)
                wsuh[sb * 5 + 3] = hash_word(uint(sb * 4 + 2) + 613u)
                wsuh[sb * 5 + 4] = hash_word(uint(sb * 4 + 3) + 613u)
            }
            var f <- tile_fixture(TILE_CELL_K, 256, wqh, wsuh, 3u, 5u)
            var y_ref <- tile_cell_arms(t, "iq2s", int(KqFmt.iq2s), f, tile_arms_all(), @@iq2sf16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_iq2xs_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ2_XS l/m/s/e tiles and the KHR tile == the CPU oracle (u64 grid + ksigns-by-parity decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            let totsb = TILE_CELL_SB
            var wqh : array<uint>
            var wsuh : array<uint>
            wqh |> resize(totsb * 16)
            wsuh |> resize(totsb * 5)
            for (i in range(totsb * 16)) {
                wqh[i] = hash_word(uint(i) + 29u)     // arbitrary quant bytes
            }
            for (sb in range(totsb)) {
                wsuh[sb * 5] = packHalf2x16(float2(0.0002 * float(1 + sb % 7), 0.0))   // cool d8
                wsuh[sb * 5 + 1] = hash_word(uint(sb * 4) + 613u)   // 16 unsigned strip bytes
                wsuh[sb * 5 + 2] = hash_word(uint(sb * 4 + 1) + 613u)
                wsuh[sb * 5 + 3] = hash_word(uint(sb * 4 + 2) + 613u)
                wsuh[sb * 5 + 4] = hash_word(uint(sb * 4 + 3) + 613u)
            }
            var f <- tile_fixture(TILE_CELL_K, 256, wqh, wsuh, 3u, 5u)
            var y_ref <- tile_cell_arms(t, "iq2xs", int(KqFmt.iq2xs), f, tile_arms_all(), @@iq2xsf16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_iq2xxs_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ2_XXS l/m/s/e tiles and the KHR tile == the CPU oracle (byte-grid + aux32 ksigns decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            let totsb = TILE_CELL_SB
            var wqh : array<uint>
            var wsuh : array<uint>
            wqh |> resize(totsb * 16)
            wsuh |> resize(totsb * 2)
            for (i in range(totsb * 16)) {
                wqh[i] = hash_word(uint(i) + 29u)     // arbitrary quant bytes
            }
            for (sb in range(totsb)) {
                wsuh[sb * 2] = packHalf2x16(float2(0.0002 * float(1 + sb % 7), 0.0)) | (hash_word(uint(sb * 2) + 613u) << 16u)   //! cool d8 | the row's high bits
                wsuh[sb * 2 + 1] = hash_word(uint(sb * 2 + 1) + 613u)   //! the eight 4-bit (2ls + 1) fields
            }
            var f <- tile_fixture(TILE_CELL_K, 256, wqh, wsuh, 3u, 5u)
            var y_ref <- tile_cell_arms(t, "iq2xxs", int(KqFmt.iq2xxs), f, tile_arms_all(), @@iq2xxsf16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_iq3xxs_cm2_batch(t0 : T?) {
    t0 |> run("cm2 IQ3_XXS l/m/s/e tiles and the KHR tile == the CPU oracle (halved grid + parity signs decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            let totsb = TILE_CELL_SB
            var wqh : array<uint>
            var wsuh : array<uint>
            wqh |> resize(totsb * 24)
            wsuh |> resize(totsb * 2)
            for (i in range(totsb * 24)) {
                wqh[i] = hash_word(uint(i) + 29u)     // arbitrary quant bytes
            }
            for (sb in range(totsb)) {
                wsuh[sb * 2] = packHalf2x16(float2(0.0002 * float(1 + sb % 7), 0.0)) | (hash_word(uint(sb * 2) + 613u) << 16u)   //! cool d | the row's high bits
                wsuh[sb * 2 + 1] = hash_word(uint(sb * 2 + 1) + 613u)   //! the eight 4-bit sub-scale fields
            }
            var f <- tile_fixture(TILE_CELL_K, 256, wqh, wsuh, 3u, 5u)
            var y_ref <- tile_cell_arms(t, "iq3xxs", int(KqFmt.iq3xxs), f, tile_arms_all(), @@iq3xxsf16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_k6_cm2_batch(t0 : T?) {
    t0 |> run("cm2 Q6_K l/m/s/e tiles and the KHR tile == the CPU oracle (6-bit compose decode-in-load; the KHR arm stages the same decode)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            let totsb = TILE_CELL_SB
            var wqh : array<uint>
            var wsuh : array<uint>
            wqh |> resize(totsb * 48)
            wsuh |> resize(totsb * 5)
            for (i in range(totsb * 48)) {
                wqh[i] = hash_word(uint(i) + 41u)     // arbitrary ql/qh bytes
            }
            for (sb in range(totsb)) {
                for (wi in range(4)) {
                    wsuh[sb * 5 + wi] = hash_word(uint(sb * 4 + wi) + 811u)   // signed sub-scale bytes
                }
                wsuh[sb * 5 + 4] = packHalf2x16(float2(0.0002 * float(1 + sb % 7), 0.0))   // cool d: the f16 acc must stay far from 65504
            }
            var f <- tile_fixture(TILE_CELL_K, 256, wqh, wsuh, 3u, 5u)
            var y_ref <- tile_cell_arms(t, "k6", int(KqFmt.k6), f, tile_arms_all(), @@k6f16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_cm2m_batch(t0 : T?) {
    t0 |> run("cm2 m-tile fmt-0 batch class kernel == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            if (!(g_gpu.coopmat_mode == COOPMAT_CM2 && g_gpu.has_coopmat2)) {
                t |> skip("the cm2 m-tile serves only in cm2 mode on an NV_coopmat2 device (DASLLAMA_COOPMAT=cm2)")
                return
            }
            var wqh : array<uint>
            var wsh : array<uint>
            q8_planes(576, 29u, 733u, wqh, wsh)   // K off the 256 lattice: two unrolled superblock passes and a 64-deep tail step
            var f <- tile_fixture(576, 32, wqh, wsh, 5u, 11u)
            var y_ref <- tile_cell_arms(t, "q8", int(KqFmt.q8), f, [ARM_CM2_M], @@q8f16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_cm2e_batch(t0 : T?) {
    t0 |> run("cm2 e-column fmt-0 batch class kernel == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device")
                return
            }
            if (!(g_gpu.coopmat_mode == COOPMAT_CM2 && g_gpu.has_coopmat2)) {
                t |> skip("the cm2 e-column serves only in cm2 mode on an NV_coopmat2 device (DASLLAMA_COOPMAT=cm2)")
                return
            }
            var wqh : array<uint>
            var wsh : array<uint>
            q8_planes(Q8_CELL_K, 31u, 739u, wqh, wsh)
            var f <- tile_fixture(Q8_CELL_K, 32, wqh, wsh, 5u, 13u)
            var y_ref <- tile_cell_arms(t, "q8", int(KqFmt.q8), f, [ARM_CM2_E], @@q8f16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_cm2s_batch(t0 : T?) {
    t0 |> run("cm2 s-tile fmt-0 batch class kernel == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_batch_cls()) {   // engage the device without touching coopmat rails
                t |> skip("no Vulkan device - skipping")
                return
            }
            if (!(g_gpu.coopmat_mode == COOPMAT_CM2 && g_gpu.has_coopmat2)) {
                t |> skip("the cm2 s-tile serves only in mode 4 on an NV_coopmat2 device (DASLLAMA_COOPMAT=cm2) - skipping")
                return
            }
            var wqh : array<uint>
            var wsh : array<uint>
            q8_planes(Q8_CELL_K, 29u, 733u, wqh, wsh)
            var f <- tile_fixture(Q8_CELL_K, 32, wqh, wsh, 5u, 11u)
            var y_ref <- tile_cell_arms(t, "q8", int(KqFmt.q8), f, [ARM_CM2_S], @@q8f16_gemm_oracle)
            delete y_ref
            delete f
        } else {
            t |> skip("dasVulkan not present - skipping")
        }
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
            var wqh : array<uint>
            var wsh : array<uint>
            var xfh : array<uint>
            wqh |> resize(totblk * 8)
            wsh |> resize((totblk + 1) / 2)
            xfh |> resize(rows * n / 2)
            for (i in range(totblk * 8)) {
                wqh[i] = hash_word(uint(i) + 43u)     // arbitrary bytes — s8 quants
            }
            for (i in range((totblk + 1) / 2)) {
                wsh[i] = ws_word(uint(i) + 337u)      // f16 scale pairs, finite halves
            }
            for (i in range(rows * n / 2)) {
                xfh[i] = ws_word(uint(i) * 7u + 3u)   // f16 activation pairs
            }
            let wq_bytes = int64(totblk) * 32l
            let ws_bytes = int64(length(wsh)) * 4l
            let xf_bytes = int64(rows * n) * 2l
            let y_bytes = int64(rows * d) * 4l
            let sk_bytes = y_bytes * int64(nsplit)
            let wqd = make_device_buf(wq_bytes)
            let wsd = make_device_buf(ws_bytes)
            let xfd = make_device_buf(xf_bytes)
            let skd = make_device_buf(sk_bytes)
            let yd2 = make_device_buf(y_bytes)
            var host = make_host_buf(y_bytes, true, [cached = true])
            var y_cls : array<float>
            y_cls |> resize(rows * d)
            var sched : array<uint>
            sched |> resize(4 + wgs)
            sched[0] = 0u
            sched[1] = 0u
            sched[2] = uint(rows)
            sched[3] = 0u
            for (w in range(wgs)) {
                sched[4 + w] = 0u
            }
            unsafe {
                upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
                upload_region_at(xfd, 0l, addr<uint8 const?>(xfh[0]), xf_bytes)
                let sc_bytes = int64(4 + wgs) * 4l
                let scd = make_device_buf(sc_bytes)
                upload_region_at(scd, 0l, addr<uint8 const?>(sched[0]), sc_bytes)
                // the GEMM writes partial planes into the scratch (group 32), the reduce sums into y
                var sc = set_q8_batch_cm2l_cls(fixed_array(wqd, wsd, scd, xfd, skd),
                    fixed_array(wq_bytes, ws_bytes, sc_bytes, xf_bytes, sk_bytes),
                    fixed_array(1u, 2u, 0u, 4u, 32u))
                var sr = set_splitk_reduce_cls(fixed_array(skd, yd2),
                    fixed_array(sk_bytes, y_bytes),
                    fixed_array(32u, 16u))
                var raw2 = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                var h2 : VkHaz
                var pc = BatchArgs(n = uint(n), d = uint(d), map_off = 4u, ksplit = uint(ksplit))
                enc_q8_batch_cm2l_cls(raw2, h2, sc, pc, int64(wgs))
                var pcr = SkRedArgs(nelem = uint(rows * d), k = uint(nsplit))
                enc_splitk_reduce_cls(raw2, h2, sr, pcr, int64(rows * d + 1023) / 1_024l)
                vhz_dep(raw2, h2, 16u, 0u, true)
                cmd_copy_whole(raw2, yd2, host.buf, y_bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
            }
            var y_ref : array<float>
            y_ref |> resize(rows * d)
            q8f16_gemm_oracle(wqh, wsh, xfh, sched, 1, n, d, y_ref)
            let bad = mismatch_bars(y_cls, y_ref, 2e-2, 4e-3 * max_abs(y_ref))
            to_log(LOG_INFO, "cm2 l-tile split-k: {bad} of {rows * d} off the oracle, {g_gpu.decvec_on ? "four-wide twin" : "scalar callback"}\n")
            t |> success(bad == 0, "cm2 l-tile split-k matches the CPU oracle ({bad} of {rows * d} off)")
            delete sched
            delete y_ref
            delete wqh
            delete wsh
            delete xfh
            delete y_cls
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the decode attention's folded requant: the pass (nsplit 1) or the combine (nsplit > 1) quantizes the head rows it
//! stores - Q8_0 blocks of 32 (`DA_RQ_Q8`) or Q8_K superblocks of 256 (`DA_RQ_Q8K`, a head of 256 or 512) - against the
//! CPU oracle's row quantized the same way: every scale within a hair, every quant within one step (the row itself
//! carries the f32 noise the plain arms allow), on a sentinel fill; the row store itself is held to the oracle too
def private da_attn_rqk_arm(t : T?; hs, nheads, kv_mul, cnt, nsplit : int; rqk : uint; slab2 : bool = false) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let kvd = (nheads / kv_mul) * hs
        let qdim = nheads * hs
        let scale = 0.125
        let blk = rqk == DA_RQ_Q8K ? 256 : 32
        var qh : array<float>
        var kh : array<float>
        var vh : array<float>
        qh |> resize(qdim)
        kh |> resize(cnt * kvd)
        vh |> resize(cnt * kvd)
        for (i in range(qdim)) {
            qh[i] = float((i * 41) % 73) * 0.05 - 1.8
        }
        for (i in range(cnt * kvd)) {
            kh[i] = float((i * 17) % 97) * 0.03 - 1.4
            vh[i] = float((i * 53) % 89) * 0.04 - 1.7
        }
        let q_bytes = int64(qdim) * 4l
        let m_bytes = int64(cnt * kvd) * 4l
        let cnt_off = nheads * nsplit * (hs + 2)   //! the arrival counters past the pieces
        let p_bytes = int64(cnt_off + int(da_attn_row_wgs(int64(nheads), int64(kv_mul), 1l))) * 4l
        let s_bytes = int64(qdim / blk) * 4l
        let qd = make_device_buf(q_bytes)
        let kd = make_device_buf(m_bytes)
        let vd = make_device_buf(m_bytes)
        let yd = make_device_buf(q_bytes)
        let partd = make_device_buf(p_bytes)
        let aqd = make_device_buf(int64(qdim))
        let asd = make_device_buf(s_bytes)
        var host = make_host_buf(q_bytes * 2l, true, [cached = true])
        var y_cls : array<float>
        var q_cls : array<int8>
        var s_cls : array<float>
        y_cls |> resize(qdim)
        q_cls |> resize(qdim)
        s_cls |> resize(qdim / blk)
        verify(ensure_da_attn_cls() && (!slab2 || ensure_da_attn_g2_cls()), "the decode attention class rail must engage on a live device")
        unsafe {
            upload_region_at(qd, 0l, addr<uint8 const?>(qh[0]), q_bytes)
            upload_region_at(kd, 0l, addr<uint8 const?>(kh[0]), m_bytes)
            upload_region_at(vd, 0l, addr<uint8 const?>(vh[0]), m_bytes)
            nan_sentinel_upload(yd, q_bytes)
            nan_sentinel_upload(partd, int64(cnt_off) * 4l)
            zero_upload(partd, int64(cnt_off) * 4l, p_bytes - int64(cnt_off) * 4l)
            nan_sentinel_upload(aqd, int64(qdim))
            nan_sentinel_upload(asd, s_bytes)
            var tokb = make_host_buf(16l, true)
            var tmc = reinterpret<TokMeta?>(tokb.mapped)
            tmc.cnt = uint(cnt)
            tmc.mirbase = 0u
            var sc = set_da_attn_cls(fixed_array(qd, kd, vd, yd, tokb.buf, partd, aqd, asd),
                fixed_array(q_bytes, m_bytes, m_bytes, q_bytes, 16l, p_bytes, int64(qdim), s_bytes),
                fixed_array(1u, 2u, 4u, 16u, 0u, 32u, 64u, 64u))
            var raw = alloc_cmd()
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
            var h : VkHaz
            var pc = DaAttnArgs(kvd = uint(kvd), hs = uint(hs), kv_mul = uint(kv_mul), kbase = 0u, vbase = 0u,
                scale = scale, nsplit = uint(nsplit), rqk = rqk, cntoff = uint(cnt_off))
            if (slab2) {
                enc_da_attn_g2_cls(raw, h, sc, pc, da_attn_row_wgs(int64(nheads), int64(kv_mul), int64(nsplit)))
            } else {
                enc_da_attn_cls(raw, h, sc, pc, da_attn_row_wgs(int64(nheads), int64(kv_mul), int64(nsplit)))
            }
            vhz_dep(raw, h, 16u | 64u, 0u, true)
            cmd_copy_range(raw, yd, 0l, host.buf, 0l, q_bytes)
            cmd_copy_range(raw, aqd, 0l, host.buf, q_bytes, int64(qdim))
            cmd_copy_range(raw, asd, 0l, host.buf, q_bytes + int64(qdim), s_bytes)
            vk_check(vkEndCommandBuffer(raw), null)
            submit_wait(raw)
            memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
            memcpy(addr<void?>(q_cls[0]), reinterpret<void?>(intptr(host.mapped) + uint64(q_bytes)), int64(qdim))
            memcpy(addr<void?>(s_cls[0]), reinterpret<void?>(intptr(host.mapped) + uint64(q_bytes + int64(qdim))), s_bytes)
        }
        var y_ref : array<float>
        y_ref |> resize(qdim)
        for (hh in range(nheads)) {
            attn_row_oracle(qh, hh * hs, kh, 0, vh, 0, kvd, hh, hs, kv_mul, cnt, scale, y_ref, hh * hs)
        }
        let arm = "hs {hs} x {nheads} heads, {nsplit} split(s), {rqk == DA_RQ_Q8K ? "Q8_K" : "Q8_0"}{slab2 ? ", the two-head slab" : ""}"
        let bad = mismatch_approx(y_cls, y_ref)
        t |> success(bad == 0, "folded-requant attention ({arm}): the row matches the CPU oracle ({bad} of {qdim} off)")
        y_ref[hs / 2] += 0.5
        t |> equal(mismatch_approx(y_cls, y_ref), 1, "folded-requant attention ({arm}): the row's bar reds one poisoned element")
        y_ref[hs / 2] -= 0.5
        if (slab2) {   //! the second lane: the four-head slab at the same shape, whose dead lanes contribute zero - the two round alike
            var y_g4 : array<float>
            y_g4 |> resize(qdim)
            unsafe {
                nan_sentinel_upload(yd, q_bytes)
                var tokb4 = make_host_buf(16l, true)
                var tmc4 = reinterpret<TokMeta?>(tokb4.mapped)
                tmc4.cnt = uint(cnt)
                tmc4.mirbase = 0u
                var sc4 = set_da_attn_cls(fixed_array(qd, kd, vd, yd, tokb4.buf, partd, aqd, asd),
                    fixed_array(q_bytes, m_bytes, m_bytes, q_bytes, 16l, p_bytes, int64(qdim), s_bytes),
                    fixed_array(1u, 2u, 4u, 16u, 0u, 32u, 64u, 64u))
                var raw4 = alloc_cmd()
                let begin4 = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw4, begin4), null)
                var h4 : VkHaz
                var pc4 = DaAttnArgs(kvd = uint(kvd), hs = uint(hs), kv_mul = uint(kv_mul), kbase = 0u, vbase = 0u,
                    scale = scale, nsplit = uint(nsplit), rqk = rqk, cntoff = uint(cnt_off))
                enc_da_attn_cls(raw4, h4, sc4, pc4, da_attn_row_wgs(int64(nheads), int64(kv_mul), int64(nsplit)))
                vhz_dep(raw4, h4, 16u, 0u, true)
                cmd_copy_range(raw4, yd, 0l, host.buf, 0l, q_bytes)
                vk_check(vkEndCommandBuffer(raw4), null)
                submit_wait(raw4)
                memcpy(addr<void?>(y_g4[0]), host.mapped, q_bytes)
            }
            var apart = 0
            for (i in range(qdim)) {
                if (y_g4[i] != y_cls[i]) {
                    apart++
                }
            }
            t |> success(apart == 0, "folded-requant attention ({arm}): the two-head slab is the four-head slab bit for bit ({apart} of {qdim} apart)")
            delete y_g4
        }
        // the oracle's own quantization of its row: the block's amax over 127, the quants rounded against it
        var bad_s = 0
        var bad_q = 0
        for (b in range(qdim / blk)) {
            var m = 0.0
            for (i in range(blk)) {
                m = max(m, abs(y_ref[b * blk + i]))
            }
            let d = m / 127.0
            if (abs(s_cls[b] - d) > 1.0e-6 + 1.0e-3 * d) {
                bad_s++
            }
            let id = d != 0.0 ? 1.0 / d : 0.0
            for (i in range(blk)) {
                let want = int(round(y_ref[b * blk + i] * id))
                if (abs(int(q_cls[b * blk + i]) - want) > 1) {
                    bad_q++
                }
            }
        }
        t |> success(bad_s == 0, "folded-requant attention ({arm}): every block scale matches the oracle row's ({bad_s} of {qdim / blk} off)")
        t |> success(bad_q == 0, "folded-requant attention ({arm}): every quant within one step of the oracle row's ({bad_q} of {qdim} off)")
        delete qh
        delete kh
        delete vh
        delete y_ref
        delete y_cls
        delete q_cls
        delete s_cls
    }
}

[test]
def test_vkd_da_attn_rqk(t0 : T?) {
    t0 |> run("decode attention with the requant folded into its store == the CPU oracle's row, quantized: Q8_0 and Q8_K, the pass and the combine, the 256 and 512 heads") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_da_attn_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            da_attn_rqk_arm(t, 64, 4, 2, 300, 1, DA_RQ_Q8)     //! the pass's own Q8_0 store
            da_attn_rqk_arm(t, 64, 4, 2, 300, 8, DA_RQ_Q8)     //! the last piece's combine storing Q8_0 (three empty pieces)
            da_attn_rqk_arm(t, 256, 2, 2, 70, 1, DA_RQ_Q8K)    //! one superblock a head, the pass
            da_attn_rqk_arm(t, 256, 2, 1, 70, 4, DA_RQ_Q8K)    //! ... the last piece's combine
            da_attn_rqk_arm(t, 512, 2, 2, 70, 4, DA_RQ_Q8K)    //! the wide head: two superblocks, the last piece's combine
            da_attn_rqk_arm(t, 512, 2, 2, 70, 1, DA_RQ_Q8)     //! the wide head's Q8_0 blocks, the pass
            //! the two-head slab stamp on the groups it serves: two heads a group (gemma-4's shape at 256 and 512), one head a
            //! group (the dead lane through the pass's store and through the combine)
            da_attn_rqk_arm(t, 64, 4, 2, 300, 1, DA_RQ_Q8, true)
            da_attn_rqk_arm(t, 64, 4, 2, 300, 8, DA_RQ_Q8, true)
            da_attn_rqk_arm(t, 256, 2, 2, 70, 1, DA_RQ_Q8K, true)
            da_attn_rqk_arm(t, 256, 2, 1, 70, 1, DA_RQ_Q8K, true)
            da_attn_rqk_arm(t, 256, 2, 1, 70, 4, DA_RQ_Q8K, true)
            da_attn_rqk_arm(t, 512, 2, 2, 70, 4, DA_RQ_Q8K, true)
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_da_attn(t0 : T?) {
    t0 |> run("decode attention class kernel == the CPU oracle (chunk boundary)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_da_attn_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            let hs = 64
            let kv_mul = 2
            let nheads = 4
            let kvd = (nheads / kv_mul) * hs
            let cnt = 300   // crosses the 256-position chunk boundary: the alpha-rescale path runs
            let scale = 0.125
            var qh : array<float>
            var kh : array<float>
            var vh : array<float>
            var sh : array<float>
            qh |> resize(nheads * hs)
            kh |> resize(cnt * kvd)
            vh |> resize(cnt * kvd)
            sh |> resize(1)
            for (i in range(nheads * hs)) {
                qh[i] = float((i * 41) % 73) * 0.05 - 1.8
            }
            for (i in range(cnt * kvd)) {
                kh[i] = float((i * 17) % 97) * 0.03 - 1.4
                vh[i] = float((i * 53) % 89) * 0.04 - 1.7
            }
            sh[0] = scale
            let q_bytes = int64(nheads * hs) * 4l
            let m_bytes = int64(cnt * kvd) * 4l
            let qd = make_device_buf(q_bytes)
            let kd = make_device_buf(m_bytes)
            let vd = make_device_buf(m_bytes)
            let yd2 = make_device_buf(q_bytes)
            var host = make_host_buf(q_bytes, true, [cached = true])
            var y_cls : array<float>
            y_cls |> resize(nheads * hs)
            unsafe {
                upload_region_at(qd, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                upload_region_at(kd, 0l, addr<uint8 const?>(kh[0]), m_bytes)
                upload_region_at(vd, 0l, addr<uint8 const?>(vh[0]), m_bytes)
                // the class kernel: q@0, K@1, V@2, y@3, tok@4 (cnt rides TokMeta)
                var tokb = make_host_buf(16l, true)
                var tmc = reinterpret<TokMeta?>(tokb.mapped)
                tmc.cnt = uint(cnt)
                tmc.mirbase = 0u
                var sc = set_da_attn_cls(fixed_array(qd, kd, vd, yd2, tokb.buf, yd2, yd2, yd2),   //! unsplit, unfolded: the out rows fill the partials and quant bindings
                    fixed_array(q_bytes, m_bytes, m_bytes, q_bytes, 16l, q_bytes, q_bytes, q_bytes),
                    fixed_array(1u, 2u, 4u, 16u, 0u, 16u, 16u, 16u))
                var raw2 = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                var h2 : VkHaz
                var pc = DaAttnArgs(kvd = uint(kvd), hs = uint(hs),
                    kv_mul = uint(kv_mul), kbase = 0u, vbase = 0u, scale = scale, rqk = DA_RQ_NONE)
                enc_da_attn_cls(raw2, h2, sc, pc, da_attn_row_wgs(int64(nheads), int64(kv_mul), 1l))
                vhz_dep(raw2, h2, 16u, 0u, true)
                cmd_copy_whole(raw2, yd2, host.buf, q_bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
            }
            var y_ref : array<float>
            y_ref |> resize(nheads * hs)
            for (hh in range(nheads)) {
                attn_row_oracle(qh, hh * hs, kh, 0, vh, 0, kvd, hh, hs, kv_mul, cnt, scale, y_ref, hh * hs)
            }
            let bad = mismatch_approx(y_cls, y_ref)
            t |> success(bad == 0, "da_attn matches the CPU oracle ({bad} of {nheads * hs} off)")
            // the softcap arm (gemma-2): a cap the scaled scores overrun, the kernel's exp form against the oracle's
            let cap = 4.0
            unsafe {
                var tokb2 = make_host_buf(16l, true)
                var tmc2 = reinterpret<TokMeta?>(tokb2.mapped)
                tmc2.cnt = uint(cnt)
                tmc2.mirbase = 0u
                var sc2 = set_da_attn_cls(fixed_array(qd, kd, vd, yd2, tokb2.buf, yd2, yd2, yd2),
                    fixed_array(q_bytes, m_bytes, m_bytes, q_bytes, 16l, q_bytes, q_bytes, q_bytes),
                    fixed_array(1u, 2u, 4u, 16u, 0u, 16u, 16u, 16u))
                var raw3 = alloc_cmd()
                let begin3 = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw3, begin3), null)
                var h3 : VkHaz
                var pc3 = DaAttnArgs(kvd = uint(kvd), hs = uint(hs),
                    kv_mul = uint(kv_mul), kbase = 0u, vbase = 0u, scale = scale, cap = cap, rqk = DA_RQ_NONE)
                enc_da_attn_cls(raw3, h3, sc2, pc3, da_attn_row_wgs(int64(nheads), int64(kv_mul), 1l))
                vhz_dep(raw3, h3, 16u, 0u, true)
                cmd_copy_whole(raw3, yd2, host.buf, q_bytes)
                vk_check(vkEndCommandBuffer(raw3), null)
                submit_wait(raw3)
                memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
            }
            let moved = mismatch_approx(y_cls, y_ref)
            t |> success(moved > 0, "control: the {cap} cap moves the uncapped output ({moved} of {nheads * hs})")
            for (hh in range(nheads)) {
                attn_row_oracle(qh, hh * hs, kh, 0, vh, 0, kvd, hh, hs, kv_mul, cnt, scale, y_ref, hh * hs, 0, cap)
            }
            let badc = mismatch_approx(y_cls, y_ref)
            t |> success(badc == 0, "da_attn under a {cap} softcap matches the CPU oracle ({badc} of {nheads * hs} off)")
            //! the split arm: eight key splits of the 300 (64-key pieces, the last 44 keys wide, three pieces empty),
            //! the partials combined by the last-arriving piece inside the pass, under the same cap against the same oracle
            let nsplit = 8
            let cnt_off = nheads * nsplit * (hs + 2)   //! the arrival counters past the pieces
            let p_bytes = int64(cnt_off + int(da_attn_row_wgs(int64(nheads), int64(kv_mul), 1l))) * 4l
            let partd = make_device_buf(p_bytes)
            unsafe {
                nan_sentinel_upload(partd, int64(cnt_off) * 4l)
                zero_upload(partd, int64(cnt_off) * 4l, p_bytes - int64(cnt_off) * 4l)
                nan_sentinel_upload(yd2, q_bytes)
                var tokb4 = make_host_buf(16l, true)
                var tmc4 = reinterpret<TokMeta?>(tokb4.mapped)
                tmc4.cnt = uint(cnt)
                tmc4.mirbase = 0u
                var sc4 = set_da_attn_cls(fixed_array(qd, kd, vd, yd2, tokb4.buf, partd, yd2, yd2),
                    fixed_array(q_bytes, m_bytes, m_bytes, q_bytes, 16l, p_bytes, q_bytes, q_bytes),
                    fixed_array(1u, 2u, 4u, 16u, 0u, 32u, 16u, 16u))
                var raw4 = alloc_cmd()
                let begin4 = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw4, begin4), null)
                var h4 : VkHaz
                var pc4 = DaAttnArgs(kvd = uint(kvd), hs = uint(hs),
                    kv_mul = uint(kv_mul), kbase = 0u, vbase = 0u, scale = scale, cap = cap, nsplit = uint(nsplit), rqk = DA_RQ_NONE, cntoff = uint(cnt_off))
                enc_da_attn_cls(raw4, h4, sc4, pc4, da_attn_row_wgs(int64(nheads), int64(kv_mul), int64(nsplit)))
                vhz_dep(raw4, h4, 16u, 0u, true)
                cmd_copy_whole(raw4, yd2, host.buf, q_bytes)
                vk_check(vkEndCommandBuffer(raw4), null)
                submit_wait(raw4)
                memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
            }
            let bads = mismatch_approx(y_cls, y_ref)
            t |> success(bads == 0, "da_attn split {nsplit} ways, its last piece combining, matches the CPU oracle under the cap ({bads} of {nheads * hs} off)")
            y_ref[hs + 3] += 0.5
            t |> success(mismatch_approx(y_cls, y_ref) == 1, "split attention control: the bar reds one poisoned element")
            //! the sink arms (gpt-oss): a per-head sink logit, the unsplit pass folding it at its store and the split pass leaving it
            //! to its last piece's combine, both against the oracle with the sink; the sink-free oracle is the control
            t |> success(ensure_da_attn_sink_cls(), "the sink twin compiles on this device")
            var sinkh : array<float>
            sinkh |> resize(nheads)
            for (hh in range(nheads)) {   //! a sink past each head's top scaled score: the fixture's 300-key row is flat, so only a dominant phantom key carries measurable mass
                let khoff = (hh / kv_mul) * hs
                var smax = -3.0e38
                for (j in range(cnt)) {
                    var s = 0.0
                    for (e in range(hs)) {
                        s += qh[hh * hs + e] * kh[j * kvd + khoff + e]
                    }
                    smax = max(smax, s * scale)
                }
                sinkh[hh] = smax + 2.5 + 0.5 * float(hh)
            }
            let sink_bytes = int64(nheads) * 4l
            let sinkd = make_device_buf(sink_bytes)
            var y_sink : array<float>
            y_sink |> resize(nheads * hs)
            for (hh in range(nheads)) {
                attn_row_oracle(qh, hh * hs, kh, 0, vh, 0, kvd, hh, hs, kv_mul, cnt, scale, y_sink, hh * hs, 0, 0.0, true, sinkh[hh])
            }
            var y_free : array<float>
            y_free |> resize(nheads * hs)
            for (hh in range(nheads)) {
                attn_row_oracle(qh, hh * hs, kh, 0, vh, 0, kvd, hh, hs, kv_mul, cnt, scale, y_free, hh * hs)
            }
            t |> success(mismatch_approx(y_sink, y_free) > 0, "sink control: the sink moves the oracle's own output ({mismatch_approx(y_sink, y_free)} of {nheads * hs})")
            for (split in fixed_array(1, nsplit)) {
                unsafe {
                    upload_region_at(sinkd, 0l, addr<uint8 const?>(sinkh[0]), sink_bytes)
                    nan_sentinel_upload(yd2, q_bytes)
                    nan_sentinel_upload(partd, int64(cnt_off) * 4l)
                    zero_upload(partd, int64(cnt_off) * 4l, p_bytes - int64(cnt_off) * 4l)
                    var tokb5 = make_host_buf(16l, true)
                    var tmc5 = reinterpret<TokMeta?>(tokb5.mapped)
                    tmc5.cnt = uint(cnt)
                    tmc5.mirbase = 0u
                    var sc5 = set_da_attn_sink_cls(fixed_array(qd, kd, vd, yd2, tokb5.buf, partd, yd2, yd2, sinkd),
                        fixed_array(q_bytes, m_bytes, m_bytes, q_bytes, 16l, p_bytes, q_bytes, q_bytes, sink_bytes),
                        fixed_array(1u, 2u, 4u, 16u, 0u, 32u, 16u, 16u, 0u))
                    var raw5 = alloc_cmd()
                    let begin5 = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw5, begin5), null)
                    var h5 : VkHaz
                    var pc5 = DaAttnArgs(kvd = uint(kvd), hs = uint(hs),
                        kv_mul = uint(kv_mul), kbase = 0u, vbase = 0u, scale = scale, nsplit = uint(split), rqk = DA_RQ_NONE, sinkoff = 0u, cntoff = uint(cnt_off))
                    enc_da_attn_sink_cls(raw5, h5, sc5, pc5, da_attn_row_wgs(int64(nheads), int64(kv_mul), int64(split)))
                    vhz_dep(raw5, h5, 16u, 0u, true)
                    cmd_copy_whole(raw5, yd2, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw5), null)
                    submit_wait(raw5)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
                }
                let badk = mismatch_approx(y_cls, y_sink)
                t |> success(badk == 0, "da_attn sink twin split {split} ways matches the CPU oracle with the sink ({badk} of {nheads * hs} off)")
                t |> success(mismatch_approx(y_cls, y_free) > 0, "da_attn sink twin split {split} ways: the sink-free oracle reds the compare")
            }
            //! the f16-mirror sink twin (the resident driver's kv16 form): the same rows over f16 K/V, the oracle over the f16-rounded values
            t |> success(ensure_da_attn_f16_sink_cls(), "the f16 sink twin compiles on this device")
            {
                var kh16 : array<float>
                var vh16 : array<float>
                kh16 |> resize(cnt * kvd)
                vh16 |> resize(cnt * kvd)
                for (i in range(cnt * kvd)) {
                    kh16[i] = q16(kh[i])
                    vh16[i] = q16(vh[i])
                }
                var k16 <- pack_f16(kh16)
                var v16 <- pack_f16(vh16)
                let m16_bytes = int64(cnt * kvd) * 2l
                let kd16 = make_device_buf(m16_bytes)
                let vd16 = make_device_buf(m16_bytes)
                var y_sink16 : array<float>
                var y_free16 : array<float>
                y_sink16 |> resize(nheads * hs)
                y_free16 |> resize(nheads * hs)
                for (hh in range(nheads)) {
                    attn_row_oracle(qh, hh * hs, kh16, 0, vh16, 0, kvd, hh, hs, kv_mul, cnt, scale, y_sink16, hh * hs, 0, 0.0, true, sinkh[hh])
                    attn_row_oracle(qh, hh * hs, kh16, 0, vh16, 0, kvd, hh, hs, kv_mul, cnt, scale, y_free16, hh * hs)
                }
                for (split in fixed_array(1, nsplit)) {
                    unsafe {
                        upload_region_at(kd16, 0l, addr<uint8 const?>(k16[0]), m16_bytes)
                        upload_region_at(vd16, 0l, addr<uint8 const?>(v16[0]), m16_bytes)
                        nan_sentinel_upload(yd2, q_bytes)
                        nan_sentinel_upload(partd, int64(cnt_off) * 4l)
                        zero_upload(partd, int64(cnt_off) * 4l, p_bytes - int64(cnt_off) * 4l)
                        var tokb6 = make_host_buf(16l, true)
                        var tmc6 = reinterpret<TokMeta?>(tokb6.mapped)
                        tmc6.cnt = uint(cnt)
                        tmc6.mirbase = 0u
                        var sc6 = set_da_attn_f16_sink_cls(fixed_array(qd, kd16, vd16, yd2, tokb6.buf, partd, yd2, yd2, sinkd),
                            fixed_array(q_bytes, m16_bytes, m16_bytes, q_bytes, 16l, p_bytes, q_bytes, q_bytes, sink_bytes),
                            fixed_array(1u, 2u, 4u, 16u, 0u, 32u, 16u, 16u, 0u))
                        var raw6 = alloc_cmd()
                        let begin6 = VkCommandBufferBeginInfo()
                        vk_check(vkBeginCommandBuffer(raw6, begin6), null)
                        var h6 : VkHaz
                        var pc6 = DaAttnArgs(kvd = uint(kvd), hs = uint(hs),
                            kv_mul = uint(kv_mul), kbase = 0u, vbase = 0u, scale = scale, nsplit = uint(split), rqk = DA_RQ_NONE, sinkoff = 0u, cntoff = uint(cnt_off))
                        enc_da_attn_f16_sink_cls(raw6, h6, sc6, pc6, da_attn_row_wgs(int64(nheads), int64(kv_mul), int64(split)))
                        vhz_dep(raw6, h6, 16u, 0u, true)
                        cmd_copy_whole(raw6, yd2, host.buf, q_bytes)
                        vk_check(vkEndCommandBuffer(raw6), null)
                        submit_wait(raw6)
                        memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
                    }
                    let badk16 = mismatch_approx(y_cls, y_sink16)
                    t |> success(badk16 == 0, "da_attn f16 sink twin split {split} ways matches the CPU oracle with the sink ({badk16} of {nheads * hs} off)")
                    t |> success(mismatch_approx(y_cls, y_free16) > 0, "da_attn f16 sink twin split {split} ways: the sink-free oracle reds the compare")
                }
                delete kh16
                delete vh16
                delete k16
                delete v16
                delete y_sink16
                delete y_free16
            }
            delete sinkh
            delete y_sink
            delete y_free
            delete qh
            delete kh
            delete vh
            delete sh
            delete y_ref
            delete y_cls
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the decode attention pass at one (head width, slab, key split, row count) shape against `attn_row_oracle` per (row, head):
//! `nrows` rows through one dispatch, row r's q past r x qdim and its K/V in its own mirror region (`tok[r].mirbase`, the
//! second region's content scaled by 1.5), each row against the oracle over its own region; the split arm submits the
//! same command a second time, so the arrival counters the pass resets must have rearmed; every bar with its poison
def private da_attn_shape_arm(t : T?; hs, nheads, kv_mul, cnt, nsplit, nrows : int; slab2 : bool = false) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let kvd = (nheads / kv_mul) * hs
        let qdim = nheads * hs
        let scale = 0.125
        let mir_elems = cnt * kvd
        var qh : array<float>
        var kh : array<float>
        var vh : array<float>
        qh |> resize(qdim * nrows)
        kh |> resize(mir_elems * nrows)
        vh |> resize(mir_elems * nrows)
        for (r in range(nrows)) {
            let rs = 1.0 + 0.5 * float(r)
            for (i in range(qdim)) {
                qh[r * qdim + i] = (float(((i + r * 17) * 41) % 73) * 0.05 - 1.8) * rs
            }
            for (i in range(mir_elems)) {
                kh[r * mir_elems + i] = (float((i * 17) % 97) * 0.03 - 1.4) * rs
                vh[r * mir_elems + i] = (float((i * 53) % 89) * 0.04 - 1.7) * rs
            }
        }
        let q_bytes = int64(qdim * nrows) * 4l
        let m_bytes = int64(mir_elems) * int64(nrows) * 4l
        let tok_bytes = 16l * int64(nrows)
        let rowwg = da_attn_row_wgs(int64(nheads), int64(kv_mul), int64(nsplit))
        let rowgroups = int(da_attn_row_wgs(int64(nheads), int64(kv_mul), 1l))   //! the (kv head, slab) groups a row: a counter each
        let cnt_off = nrows * nheads * nsplit * (hs + 2)   //! the arrival counters past every row's pieces
        let p_bytes = int64(cnt_off + nrows * rowgroups) * 4l
        let qd = make_device_buf(q_bytes)
        let kd = make_device_buf(m_bytes)
        let vd = make_device_buf(m_bytes)
        let yd = make_device_buf(q_bytes)
        let partd = make_device_buf(p_bytes)
        var host = make_host_buf(q_bytes, true, [cached = true])
        var y_cls : array<float>
        y_cls |> resize(qdim * nrows)
        var raw = alloc_cmd()
        unsafe {
            upload_region_at(qd, 0l, addr<uint8 const?>(qh[0]), q_bytes)
            upload_region_at(kd, 0l, addr<uint8 const?>(kh[0]), m_bytes)
            upload_region_at(vd, 0l, addr<uint8 const?>(vh[0]), m_bytes)
            nan_sentinel_upload(yd, q_bytes)
            nan_sentinel_upload(partd, int64(cnt_off) * 4l)
            zero_upload(partd, int64(cnt_off) * 4l, p_bytes - int64(cnt_off) * 4l)
            var tokb = make_host_buf(tok_bytes, true)
            var tm = reinterpret<TokMeta?>(tokb.mapped)
            for (r in range(nrows)) {
                tm[r].pos = uint(cnt - 1)
                tm[r].cnt = uint(cnt)
                tm[r].parity = 0u
                tm[r].mirbase = uint(r * mir_elems)
            }
            var sc = set_da_attn_cls(fixed_array(qd, kd, vd, yd, tokb.buf, partd, yd, yd),   //! unfolded: the quant bindings take the out rows (unwritten)
                fixed_array(q_bytes, m_bytes, m_bytes, q_bytes, tok_bytes, p_bytes, q_bytes, q_bytes),
                fixed_array(1u, 2u, 4u, 16u, 0u, 32u, 16u, 16u))
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
            var h : VkHaz
            var pc = DaAttnArgs(kvd = uint(kvd), hs = uint(hs), kv_mul = uint(kv_mul), kbase = 0u, vbase = 0u, scale = scale,
                nsplit = uint(nsplit), rqk = DA_RQ_NONE, cntoff = uint(cnt_off),
                rowwg = nrows > 1 ? uint(rowwg) : 0u, qrow = nrows > 1 ? uint(qdim) : 0u)
            if (slab2) {
                enc_da_attn_g2_cls(raw, h, sc, pc, rowwg * int64(nrows))
            } else {
                enc_da_attn_cls(raw, h, sc, pc, rowwg * int64(nrows))
            }
            vhz_dep(raw, h, 16u, 0u, true)
            cmd_copy_whole(raw, yd, host.buf, q_bytes)
            vk_check(vkEndCommandBuffer(raw), null)
            submit_wait(raw)
            memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
        }
        var y_ref : array<float>
        y_ref |> resize(qdim * nrows)
        for (r in range(nrows)) {
            for (hh in range(nheads)) {
                attn_row_oracle(qh, r * qdim + hh * hs, kh, r * mir_elems, vh, r * mir_elems, kvd, hh, hs, kv_mul, cnt, scale, y_ref, r * qdim + hh * hs)
            }
        }
        let arm = "hs {hs} x {nheads} heads, {kv_mul} a kv head, {cnt} keys, {nsplit} split(s), {nrows} row(s){slab2 ? ", the two-head slab" : ""}"
        for (r in range(nrows)) {
            let bad = mismatch_approx(y_cls, y_ref, r * qdim, qdim)
            t |> success(bad == 0, "decode attention ({arm}): row {r} matches its own oracle ({bad} of {qdim} off)")
        }
        if (nsplit > 1) {
            unsafe {
                nan_sentinel_upload(yd, q_bytes)
                submit_wait(raw)
                memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
            }
            let bad2 = mismatch_approx(y_cls, y_ref)
            t |> success(bad2 == 0, "decode attention ({arm}): the same command submitted again matches the oracle ({bad2} of {qdim * nrows} off) - the arrival counters rearmed")
        }
        if (nrows > 1) {
            var y_x : array<float>
            y_x |> resize(qdim * nrows)
            for (hh in range(nheads)) {
                attn_row_oracle(qh, qdim + hh * hs, kh, 0, vh, 0, kvd, hh, hs, kv_mul, cnt, scale, y_x, qdim + hh * hs)
            }
            let crossed = mismatch_approx(y_cls, y_x, qdim, qdim)
            t |> success(crossed > 0, "decode attention ({arm}): row 1 read its own region - the oracle over row 0's misses ({crossed} of {qdim})")
            delete y_x
        }
        for (r in range(nrows)) {
            y_ref[r * qdim + hs + 3] += 0.5
            t |> success(mismatch_approx(y_cls, y_ref, r * qdim, qdim) == 1, "decode attention ({arm}): row {r}'s bar reds one poisoned element")
        }
        delete qh
        delete kh
        delete vh
        delete y_ref
        delete y_cls
    }
}

[test]
def test_vkd_da_attn_shapes(t0 : T?) {
    t0 |> run("decode attention at the shapes the fixed cells leave out == the CPU oracle: ragged key groups, a full four-head slab, a short last slab, a 24-word head, two rows a dispatch") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_da_attn_cls() && ensure_da_attn_g2_cls())) {
                t |> skip("no Vulkan device")
                return
            }
            da_attn_shape_arm(t, 96, 4, 1, 300, 1, 1)    //! 21 key groups of 12 words: the ragged group count, no fold
            da_attn_shape_arm(t, 96, 4, 1, 300, 4, 1)
            da_attn_shape_arm(t, 128, 8, 4, 300, 1, 1)   //! a full four-head slab (`live` 4): two fold rounds at subgroup 32
            da_attn_shape_arm(t, 128, 8, 4, 300, 8, 1)
            da_attn_shape_arm(t, 64, 10, 5, 300, 1, 1)   //! a short last slab: `live` 1 on each kv head's second slab
            da_attn_shape_arm(t, 192, 2, 2, 70, 1, 1)    //! 24 words a V row, not a power of two: no fold
            da_attn_shape_arm(t, 64, 4, 2, 300, 1, 2)    //! two rows a dispatch, each over its own mirror region
            da_attn_shape_arm(t, 64, 4, 2, 300, 4, 2)
            da_attn_shape_arm(t, 64, 4, 2, 300, 1, 2, true)   //! the two-head slab over the rows: the row index and the slab arithmetic together
            da_attn_shape_arm(t, 64, 4, 2, 300, 4, 4, true)
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
                let kpos = w0 + rows   // K/V panels hold absolute positions [0, w0+rows)
                var qh : array<float>
                var kh : array<float>
                var vh : array<float>
                var gh : array<float>
                qh |> resize(rows * qd)
                kh |> resize(kpos * kvd)
                vh |> resize(kpos * kvd)
                gh |> resize(rows * qd)
                for (i in range(rows * qd)) {
                    qh[i] = float((i * 41) % 73) * 0.05 - 1.8
                    gh[i] = float((i * 29) % 61) * 0.06 - 1.9
                }
                for (i in range(kpos * kvd)) {
                    kh[i] = float((i * 17) % 97) * 0.03 - 1.4
                    vh[i] = float((i * 53) % 89) * 0.04 - 1.7
                }
                let q_bytes = int64(rows * qd) * 4l
                let m_bytes = int64(kpos * kvd) * 4l
                let qdev = make_device_buf(q_bytes)
                let kdev = make_device_buf(m_bytes)
                let vdev = make_device_buf(m_bytes)
                let yd2 = make_device_buf(q_bytes)
                var host = make_host_buf(q_bytes, true, [cached = true])
                var y_cls : array<float>
                y_cls |> resize(rows * qd)
                let wgs = nheads * ((rows + 7) / 8)
                unsafe {
                    upload_region_at(qdev, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                    upload_region_at(kdev, 0l, addr<uint8 const?>(kh[0]), m_bytes)
                    upload_region_at(vdev, 0l, addr<uint8 const?>(vh[0]), m_bytes)
                    // the class kernel: q@0, K@1, V@2, out@3 (pre-loaded with the gates)
                    upload_region_at(yd2, 0l, addr<uint8 const?>(gh[0]), q_bytes)
                    var sc = set_at_attn_cls(fixed_array(qdev, kdev, vdev, yd2),
                        fixed_array(q_bytes, m_bytes, m_bytes, q_bytes),
                        fixed_array(1u, 2u, 8u, 4u))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = AtAttnArgs(rows = uint(rows), w0 = uint(w0), qd = uint(qd),
                        kvd = uint(kvd), hs = uint(hs), kv_mul = uint(kv_mul),
                        flags = gated ? 1u : 0u, dpl = uint(hs / 32), scale = scale)
                    enc_at_attn_cls(raw2, h2, sc, pc, int64(wgs))
                    vhz_dep(raw2, h2, 4u, 0u, true)
                    cmd_copy_whole(raw2, yd2, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
                }
                // oracle: causal rows (row r attends [0, w0 + r]); gated = out * sigmoid(gate)
                var y_ref : array<float>
                y_ref |> resize(rows * qd)
                for (r in range(rows)) {
                    for (hh in range(nheads)) {
                        attn_row_oracle(qh, r * qd + hh * hs, kh, 0, vh, 0, kvd, hh, hs, kv_mul,
                            w0 + r + 1, scale, y_ref, r * qd + hh * hs)
                    }
                }
                if (gated) {
                    for (i in range(rows * qd)) {
                        y_ref[i] = y_ref[i] / (1.0 + exp(-gh[i]))
                    }
                }
                let bad = mismatch_approx(y_cls, y_ref)
                t |> success(bad == 0, "at_attn hs {hs} matches the CPU oracle ({bad} of {rows * qd} off)")
                delete qh
                delete kh
                delete vh
                delete gh
                delete y_ref
                delete y_cls
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

//! the f16-out twin is held to the f32 stamp's output through f16cvt, not to the oracle: the two stamps must agree bit for bit
def private fa_tile_run(t : T?; khr : bool) {   // nolint:STYLE037 — one arm loop: the fixture, both stamps, the oracle and its controls
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
            let sink_bytes = int64(nheads) * 4l
            let sinkd = make_device_buf(sink_bytes)
            let qd = nheads * hs
            let kvd = (nheads / kv_mul) * hs
            let kpos = w0 + rows
            let kpad = kpos + FA_SHADOW_SLACK_ROWS
            var qh : array<float>
            var kh : array<float>
            var vh : array<float>
            qh |> resize(rows * qd)
            kh |> resize(kpad * kvd)
            vh |> resize(kpad * kvd)
            for (i in range(rows * qd)) {
                qh[i] = float((i * 41) % 73) * 0.05 - 1.8
            }
            for (i in range(kpos * kvd)) {
                kh[i] = float((i * 17) % 97) * 0.03 - 1.4
                vh[i] = float((i * 53) % 89) * 0.04 - 1.7
            }
            for (i in range(kpos * kvd, kpad * kvd)) {   //! the slack rows carry a large finite value, not zero: a masked probability that is not exactly zero would drag it into the output
                kh[i] = 10000.0
                vh[i] = 10000.0
            }
            let q_bytes = int64(rows * qd) * 4l
            let m_bytes = int64(kpad * kvd) * 4l
            let sh_bytes = int64(kpad * kvd) * 2l
            let h16_bytes = int64(rows * qd) * 2l
            let qdev = make_device_buf(q_bytes)
            let kdev = make_device_buf(m_bytes)
            let vdev = make_device_buf(m_bytes)
            let khdev = make_device_buf(sh_bytes)
            let vhdev = make_device_buf(sh_bytes)
            let yd = make_device_buf(q_bytes)
            let yh16_fused = make_device_buf(h16_bytes)   // the f16-out twin's plane
            let yh16_split = make_device_buf(h16_bytes)   // the split pair's: f32 out -> f16cvt
            var host = make_host_buf(q_bytes, true, [cached = true])
            var host16 = make_host_buf(h16_bytes * 2l, true, [cached = true])
            var y_cls : array<float>
            var y16_fused : array<float16>
            var y16_split : array<float16>
            y_cls |> resize(rows * qd)
            y16_fused |> resize(rows * qd)
            y16_split |> resize(rows * qd)
            // two DIFFERENT sentinels on the compared f16 planes: a dead leg on either side reads as a mismatch
            for (i in range(rows * qd)) {
                y16_fused[i] = float16(-777.5)
                y16_split[i] = float16(-999.25)
            }
            unsafe {
                upload_region_at(qdev, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                upload_region_at(kdev, 0l, addr<uint8 const?>(kh[0]), m_bytes)
                upload_region_at(vdev, 0l, addr<uint8 const?>(vh[0]), m_bytes)
                upload_region_at(yh16_fused, 0l, addr<uint8 const?>(y16_fused[0]), h16_bytes)
                upload_region_at(yh16_split, 0l, addr<uint8 const?>(y16_split[0]), h16_bytes)
                upload_region_at(sinkd, 0l, addr<uint8 const?>(sinkh[0]), sink_bytes)
                nan_sentinel_upload(yd, q_bytes)
                var sck = set_f16cvt_cls(fixed_array(kdev, khdev),
                    fixed_array(m_bytes, sh_bytes), fixed_array(2u, 16u))
                var scv = set_f16cvt_cls(fixed_array(vdev, vhdev),
                    fixed_array(m_bytes, sh_bytes), fixed_array(8u, 32u))
                var sfa = (sink
                    ? fa_stamp_sink_set(khr, fixed_array(qdev, khdev, vhdev, yd, sinkd),
                        fixed_array(q_bytes, sh_bytes, sh_bytes, q_bytes, sink_bytes),
                        fixed_array(1u, 16u, 32u, 4u, 0u))
                    : fa_stamp_set(khr, fixed_array(qdev, khdev, vhdev, yd),
                        fixed_array(q_bytes, sh_bytes, sh_bytes, q_bytes),
                        fixed_array(1u, 16u, 32u, 4u)))
                var sfa16 = (sink
                    ? fa_stamp_sink_set(khr, fixed_array(qdev, khdev, vhdev, yh16_fused, sinkd),
                        fixed_array(q_bytes, sh_bytes, sh_bytes, h16_bytes, sink_bytes),
                        fixed_array(1u, 16u, 32u, 64u, 0u))
                    : fa_stamp_set(khr, fixed_array(qdev, khdev, vhdev, yh16_fused),
                        fixed_array(q_bytes, sh_bytes, sh_bytes, h16_bytes),
                        fixed_array(1u, 16u, 32u, 64u)))
                var scy = set_f16cvt_cls(fixed_array(yd, yh16_split),
                    fixed_array(q_bytes, h16_bytes), fixed_array(4u, 128u))
                var raw2 = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                var h2 : VkHaz
                var pcv = ActArgs(nelem = uint(kpad * kvd), nblk = 0u)
                let cvtwgs = int64((kpad * kvd / 4 + 255) / 256)
                enc_f16cvt_cls(raw2, h2, sck, pcv, cvtwgs)
                enc_f16cvt_cls(raw2, h2, scv, pcv, cvtwgs)
                var pcf = FaArgs(rows = uint(rows), w0 = uint(w0), qd = uint(qd),
                    kvd = uint(kvd), kv_mul = uint(kv_mul), kvbase = 0u, scale = scale, window = uint(window), sinkoff = 0u)
                enc_fa_tile(khr, hs, false, false, sink, false, raw2, h2, sfa, pcf, nheads, rows)
                enc_fa_tile(khr, hs, false, false, sink, true, raw2, h2, sfa16, pcf, nheads, rows)
                // the split pair's own convert: f16cvt over the f32 out - the fused twin's true peer
                var pcy = ActArgs(nelem = uint(rows * qd), nblk = 0u)
                enc_f16cvt_cls(raw2, h2, scy, pcy, int64((rows * qd / 4 + 255) / 256))
                vhz_dep(raw2, h2, 4u | 64u | 128u, 0u, true)
                cmd_copy_whole(raw2, yd, host.buf, q_bytes)
                cmd_copy_range(raw2, yh16_fused, 0l, host16.buf, 0l, h16_bytes)
                cmd_copy_range(raw2, yh16_split, 0l, host16.buf, h16_bytes, h16_bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
                memcpy(addr<void?>(y16_fused[0]), host16.mapped, h16_bytes)
                memcpy(addr<void?>(y16_split[0]), reinterpret<void?>(intptr(host16.mapped) + uint64(h16_bytes)), h16_bytes)
            }
            // oracle mirrors the kernel's f16 grid: q pre-scales then rounds (the kernel's
            // MatrixTimesScalar-then-convert), K/V round through the shadow; softmax stays f32. The cm2 kernel's
            // O accumulates in f16 under the row-max bias (every P at an eighth or under, the normalized row a
            // convex combination of V rows with |v| <= 1 on this fixture), so the f16 running sum's rounding
            // stays under the 2e-3 relative bar mismatch_approx holds against the f32 oracle, and the f16 O
            // twin against the f32 stamp pins the same bar - the bound is this fixture's magnitudes, not a proof.
            // The KHR kernel rounds P to f16 and sums each 64-key step in f16 before its f32 o row: the same bar.
            var qh2 : array<float>
            var kh2 : array<float>
            var vh2 : array<float>
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
                    for (hh in range(nheads)) {
                        let cnt = w0 + r + 1
                        attn_row_oracle(qh2, r * qd + hh * hs, kh2, 0, vh2, 0, kvd, hh, hs, kv_mul,
                            cnt, 1.0, y_nosink, r * qd + hh * hs, window > 0 && cnt > window ? cnt - window : 0)
                    }
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
            t |> success(bad16 == 0, "{tag} hs {hs}{stag} f16-out twin == the f32 stamp's f16cvt ({bad16} of {rows * qd} off)")
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
    t0 |> run("cm2 flash-attention family == the CPU oracle (h64 + h128 + h256, f16cvt-staged planes)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_f16cvt_cls()) {   // engage the device without touching coopmat rails
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
    t0 |> run("KHR flash-attention family == the CPU oracle (h64 + h128 + h256 + h512, the window arm, f16cvt-staged planes)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_f16cvt_cls()) {
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

// the gated h256 stamps at one geometry: a [q | gate] q panel (per-head stride 2 x hs), the f32
// and f16-out twins against the CPU oracle scaled by the sigmoid of the gate half - the Qwen3.5
// attention shape. Returns (f32 mismatches, f16-vs-f32 mismatches, outputs compared).
def private fa_h256_gated_run(nheads, kv_mul, rows, w0 : int) : tuple<bad : int; bad16 : int; n : int> {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let hs = 256
        let qhs = 2 * hs
        let scale = 0.0625
        let qd = nheads * hs
        let qrow = nheads * qhs
        let kvd = (nheads / kv_mul) * hs
        let kpos = w0 + rows
        var qp : array<float>
        var kh : array<float>
        var vh : array<float>
        qp |> resize(rows * qrow)
        kh |> resize(kpos * kvd)
        vh |> resize(kpos * kvd)
        for (i in range(rows * qrow)) {
            qp[i] = float((i * 41) % 73) * 0.05 - 1.8
        }
        for (i in range(kpos * kvd)) {
            kh[i] = float((i * 17) % 97) * 0.03 - 1.4
            vh[i] = float((i * 53) % 89) * 0.04 - 1.7
        }
        let qp_bytes = int64(rows * qrow) * 4l
        let m_bytes = int64(kpos * kvd) * 4l
        let sh_bytes = int64(kpos * kvd) * 2l
        let o_bytes = int64(rows * qd) * 4l
        let o16_bytes = int64(rows * qd) * 2l
        let qdev = make_device_buf(qp_bytes)
        let kdev = make_device_buf(m_bytes)
        let vdev = make_device_buf(m_bytes)
        let khdev = make_device_buf(sh_bytes)
        let vhdev = make_device_buf(sh_bytes)
        let yd = make_device_buf(o_bytes)
        let yh16 = make_device_buf(o16_bytes)
        var host = make_host_buf(o_bytes + o16_bytes, true, [cached = true])
        var y_cls : array<float>
        var y16 : array<float16>
        y_cls |> resize(rows * qd)
        y16 |> resize(rows * qd)
        nan_sentinel_upload(yd, o_bytes)
        nan_sentinel_upload(yh16, o16_bytes)
        unsafe {
            upload_region_at(qdev, 0l, addr<uint8 const?>(qp[0]), qp_bytes)
            upload_region_at(kdev, 0l, addr<uint8 const?>(kh[0]), m_bytes)
            upload_region_at(vdev, 0l, addr<uint8 const?>(vh[0]), m_bytes)
            var sck = set_f16cvt_cls(fixed_array(kdev, khdev), fixed_array(m_bytes, sh_bytes), fixed_array(2u, 16u))
            var scv = set_f16cvt_cls(fixed_array(vdev, vhdev), fixed_array(m_bytes, sh_bytes), fixed_array(8u, 32u))
            var sfa = fa_stamp_set(false, fixed_array(qdev, khdev, vhdev, yd),
                fixed_array(qp_bytes, sh_bytes, sh_bytes, o_bytes), fixed_array(1u, 16u, 32u, 4u))
            var sfa16 = fa_stamp_set(false, fixed_array(qdev, khdev, vhdev, yh16),
                fixed_array(qp_bytes, sh_bytes, sh_bytes, o16_bytes), fixed_array(1u, 16u, 32u, 64u))
            var raw2 = alloc_cmd()
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw2, begin), null)
            var h2 : VkHaz
            var pcv = ActArgs(nelem = uint(kpos * kvd), nblk = 0u)
            let cvtwgs = int64((kpos * kvd / 4 + 255) / 256)
            enc_f16cvt_cls(raw2, h2, sck, pcv, cvtwgs)
            enc_f16cvt_cls(raw2, h2, scv, pcv, cvtwgs)
            var pcf = FaArgs(rows = uint(rows), w0 = uint(w0), qd = uint(qd), kvd = uint(kvd),
                kv_mul = uint(kv_mul), kvbase = 0u, scale = scale, qstride = uint(qrow), qhs = uint(qhs))
            enc_fa_tile(false, 256, true, false, false, false, raw2, h2, sfa, pcf, nheads, rows)
            enc_fa_tile(false, 256, true, false, false, true, raw2, h2, sfa16, pcf, nheads, rows)
            vhz_dep(raw2, h2, 4u | 64u, 0u, true)
            cmd_copy_whole(raw2, yd, host.buf, o_bytes)
            cmd_copy_range(raw2, yh16, 0l, host.buf, o_bytes, o16_bytes)
            vk_check(vkEndCommandBuffer(raw2), null)
            submit_wait(raw2)
            memcpy(addr<void?>(y_cls[0]), host.mapped, o_bytes)
            memcpy(addr<void?>(y16[0]), reinterpret<void?>(intptr(host.mapped) + uint64(o_bytes)), o16_bytes)
        }
        // oracle: the q halves pre-scale and round to f16 like the kernel; the gate halves stay f32
        var qp2 : array<float>
        var kh2 : array<float>
        var vh2 : array<float>
        qp2 |> resize(rows * qrow)
        kh2 |> resize(kpos * kvd)
        vh2 |> resize(kpos * kvd)
        for (i in range(rows * qrow)) {
            qp2[i] = float(float16(qp[i] * scale))
        }
        for (i in range(kpos * kvd)) {
            kh2[i] = float(float16(kh[i]))
            vh2[i] = float(float16(vh[i]))
        }
        var y_ref : array<float>
        y_ref |> resize(rows * qd)
        for (r in range(rows)) {
            for (hh in range(nheads)) {
                attn_row_oracle(qp2, r * qrow + hh * qhs, kh2, 0, vh2, 0, kvd, hh, hs, kv_mul,
                    w0 + r + 1, 1.0, y_ref, r * qd + hh * hs)
                for (c in range(hs)) {
                    let g = qp[r * qrow + hh * qhs + hs + c]
                    y_ref[r * qd + hh * hs + c] *= 1.0 / (1.0 + exp(-g))
                }
            }
        }
        let bad = mismatch_approx(y_cls, y_ref)
        var bad16 = 0
        for (i in range(rows * qd)) {
            if (abs(float(y16[i]) - y_cls[i]) > 2e-3 * max(1.0, abs(y_cls[i]))) {
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
        delete y_cls
        delete y16
        return (bad = bad, bad16 = bad16, n = rows * qd)
    } else {
        return (bad = 0, bad16 = 0, n = 0)
    }
}

[test]
def test_vkd_fa_cm2_h256_gated(t0 : T?) {
    t0 |> run("cm2 flash-attention h256 gated == the CPU oracle (q at the head's stride, sigmoid gate on the output)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_f16cvt_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!g_gpu.has_coopmat2_fa) {
                t |> skip("device lacks the cm2 flash-attention trio")
                return
            }
            verify(fa_stamp_pair_ensure(false, 256l, true, false, false, true), "the gated h256 fa rails must engage on an fa-capable device")
            let r = fa_h256_gated_run(4, 2, 77, 64)   // partial 64-tile, a nonzero window base
            t |> success(r.bad == 0, "fa_cm2 h256 gated matches the CPU oracle ({r.bad} of {r.n} off)")
            t |> success(r.bad16 == 0, "fa_cm2 h256 gated f16-out twin == the f32 stamp rounded ({r.bad16} of {r.n} off)")
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the softcap stamps at gemma-2's shape (256-wide heads, cap 50): the f32 and f16-out leaves against the oracle with the
//! same cap on every scaled score. Returns (f32 mismatches, f16-vs-f32 mismatches, outputs compared).
def private fa_h256_softcap_run(nheads, kv_mul, rows, w0 : int; cap : float; khr : bool) : tuple<bad : int; bad16 : int; n : int> {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let hs = 256
        let scale = 0.0625
        let qd = nheads * hs
        let kvd = (nheads / kv_mul) * hs
        let kpos = w0 + rows
        let kpad = kpos + FA_SHADOW_SLACK_ROWS
        var qh : array<float>
        var kh : array<float>
        var vh : array<float>
        qh |> resize(rows * qd)
        kh |> resize(kpad * kvd)
        vh |> resize(kpad * kvd)
        for (i in range(rows * qd)) {   //! wide q and k values: the scaled scores run past the cap, so the cap is load-bearing
            qh[i] = float((i * 41) % 73) * 0.25 - 9.0
        }
        for (i in range(kpos * kvd)) {
            kh[i] = float((i * 17) % 97) * 0.15 - 7.0
            vh[i] = float((i * 53) % 89) * 0.04 - 1.7
        }
        let q_bytes = int64(rows * qd) * 4l
        let m_bytes = int64(kpad * kvd) * 4l
        let sh_bytes = int64(kpad * kvd) * 2l
        let o16_bytes = int64(rows * qd) * 2l
        let qdev = make_device_buf(q_bytes)
        let kdev = make_device_buf(m_bytes)
        let vdev = make_device_buf(m_bytes)
        let khdev = make_device_buf(sh_bytes)
        let vhdev = make_device_buf(sh_bytes)
        let yd = make_device_buf(q_bytes)
        let yh16 = make_device_buf(o16_bytes)
        var host = make_host_buf(q_bytes + o16_bytes, true, [cached = true])
        var y_cls : array<float>
        var y16 : array<float16>
        y_cls |> resize(rows * qd)
        y16 |> resize(rows * qd)
        nan_sentinel_upload(yd, q_bytes)
        nan_sentinel_upload(yh16, o16_bytes)
        unsafe {
            upload_region_at(qdev, 0l, addr<uint8 const?>(qh[0]), q_bytes)
            upload_region_at(kdev, 0l, addr<uint8 const?>(kh[0]), m_bytes)
            upload_region_at(vdev, 0l, addr<uint8 const?>(vh[0]), m_bytes)
            var sck = set_f16cvt_cls(fixed_array(kdev, khdev), fixed_array(m_bytes, sh_bytes), fixed_array(2u, 16u))
            var scv = set_f16cvt_cls(fixed_array(vdev, vhdev), fixed_array(m_bytes, sh_bytes), fixed_array(8u, 32u))
            var sfa = fa_stamp_set(khr, fixed_array(qdev, khdev, vhdev, yd),
                fixed_array(q_bytes, sh_bytes, sh_bytes, q_bytes), fixed_array(1u, 16u, 32u, 4u))
            var sfa16 = fa_stamp_set(khr, fixed_array(qdev, khdev, vhdev, yh16),
                fixed_array(q_bytes, sh_bytes, sh_bytes, o16_bytes), fixed_array(1u, 16u, 32u, 64u))
            var raw2 = alloc_cmd()
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw2, begin), null)
            var h2 : VkHaz
            var pcv = ActArgs(nelem = uint(kpad * kvd), nblk = 0u)
            let cvtwgs = int64((kpad * kvd / 4 + 255) / 256)
            enc_f16cvt_cls(raw2, h2, sck, pcv, cvtwgs)
            enc_f16cvt_cls(raw2, h2, scv, pcv, cvtwgs)
            var pcf = FaArgs(rows = uint(rows), w0 = uint(w0), qd = uint(qd), kvd = uint(kvd),
                kv_mul = uint(kv_mul), kvbase = 0u, scale = scale, cap = cap)
            enc_fa_tile(khr, hs, false, true, false, false, raw2, h2, sfa, pcf, nheads, rows)
            enc_fa_tile(khr, hs, false, true, false, true, raw2, h2, sfa16, pcf, nheads, rows)
            vhz_dep(raw2, h2, 4u | 64u, 0u, true)
            cmd_copy_whole(raw2, yd, host.buf, q_bytes)
            cmd_copy_range(raw2, yh16, 0l, host.buf, q_bytes, o16_bytes)
            vk_check(vkEndCommandBuffer(raw2), null)
            submit_wait(raw2)
            memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
            memcpy(addr<void?>(y16[0]), reinterpret<void?>(intptr(host.mapped) + uint64(q_bytes)), o16_bytes)
        }
        var qh2 : array<float>
        var kh2 : array<float>
        var vh2 : array<float>
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
        var y_nocap : array<float>
        y_ref |> resize(rows * qd)
        y_nocap |> resize(rows * qd)
        for (r in range(rows)) {
            for (hh in range(nheads)) {
                attn_row_oracle(qh2, r * qd + hh * hs, kh2, 0, vh2, 0, kvd, hh, hs, kv_mul,
                    w0 + r + 1, 1.0, y_ref, r * qd + hh * hs, 0, cap)
                attn_row_oracle(qh2, r * qd + hh * hs, kh2, 0, vh2, 0, kvd, hh, hs, kv_mul,
                    w0 + r + 1, 1.0, y_nocap, r * qd + hh * hs)
            }
        }
        let bad = mismatch_approx(y_cls, y_ref)
        let bad_nocap = mismatch_approx(y_cls, y_nocap)   //! the control: the same rows without the cap must miss, or the fixture never reached the cap
        var bad16 = 0
        // the f16 twin's 2e-3 bar: the O accumulator's f16 step is bounded as test_vkd_fa_cm2's oracle note states, on the same fixture magnitudes
        for (i in range(rows * qd)) {
            if (abs(float(y16[i]) - y_cls[i]) > 2e-3 * max(1.0, abs(y_cls[i]))) {
                bad16++
            }
        }
        delete qh
        delete kh
        delete vh
        delete qh2
        delete kh2
        delete vh2
        delete y_ref
        delete y_nocap
        delete y_cls
        delete y16
        return (bad = bad, bad16 = bad16, n = bad_nocap > 0 ? rows * qd : 0)
    } else {
        return (bad = 0, bad16 = 0, n = 0)
    }
}

[test]
def test_vkd_fa_cm2_h256_softcap(t0 : T?) {
    t0 |> run("cm2 flash-attention h256 softcap stamps == the CPU oracle under cap 50 (gemma-2's shape; the uncapped oracle as the control)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_f16cvt_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!g_gpu.has_coopmat2_fa) {
                t |> skip("device lacks the cm2 flash-attention trio")
                return
            }
            verify(fa_stamp_pair_ensure(false, 256l, false, true, false, true), "the softcap h256 fa rails must engage on an fa-capable device")
            let r = fa_h256_softcap_run(4, 2, 77, 64, 50.0, false)   // partial 64-tile, a nonzero window base
            t |> success(r.n > 0, "the softcap fixture's scores reach the cap (the uncapped oracle misses the kernel)")
            t |> success(r.bad == 0, "fa_cm2 h256 softcap matches the capped CPU oracle ({r.bad} of {r.n} off)")
            t |> success(r.bad16 == 0, "fa_cm2 h256 softcap f16-out twin == the f32 stamp rounded ({r.bad16} of {r.n} off)")
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_fa_khr_h256_softcap(t0 : T?) {
    t0 |> run("KHR flash-attention h256 softcap stamps == the CPU oracle under cap 50 (gemma-2's shape; the uncapped oracle as the control)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_f16cvt_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!(g_gpu.has_coopmat && khr_kq_tile_on())) {
                t |> skip("device lacks KHR cooperative matrices at subgroup 32")
                return
            }
            verify(fa_stamp_pair_ensure(true, 256l, false, true, false, true), "the softcap h256 KHR fa rails must engage on a coopmat device")
            let r = fa_h256_softcap_run(4, 2, 77, 64, 50.0, true)
            t |> success(r.n > 0, "the softcap fixture's scores reach the cap (the uncapped oracle misses the kernel)")
            t |> success(r.bad == 0, "fa_khr h256 softcap matches the capped CPU oracle ({r.bad} of {r.n} off)")
            t |> success(r.bad16 == 0, "fa_khr h256 softcap f16-out twin == the f32 stamp rounded ({r.bad16} of {r.n} off)")
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

// the 9B's own shape: 16 heads over 4 kv heads, a full 512-row window from position 0
[test]
def test_vkd_fa_cm2_h256_9b(t0 : T?) {
    t0 |> run("cm2 flash-attention h256 gated at the 9B geometry (16 heads / 4 kv, 512 rows at w0 0) == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_f16cvt_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!g_gpu.has_coopmat2_fa) {
                t |> skip("device lacks the cm2 flash-attention trio")
                return
            }
            verify(fa_stamp_pair_ensure(false, 256l, true, false, false, true), "the gated h256 fa rails must engage on an fa-capable device")
            let r = fa_h256_gated_run(16, 4, 512, 0)
            t |> success(r.bad == 0, "fa_cm2 h256 gated 9B: matches the CPU oracle ({r.bad} of {r.n} off)")
            t |> success(r.bad16 == 0, "fa_cm2 h256 gated 9B: f16-out twin == the f32 stamp rounded ({r.bad16} of {r.n} off)")
        } else {
            t |> skip("dasVulkan not present")
        }
    }
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
            let nheads = 4
            let kv_mul = 2
            let npos = 13   // partial 8-tile: q0=8 has 5 valid rows — the row masks run
            let w0 = 32     // nonzero window base: kend crosses the 32-position K tile → alpha rescale runs
            let kbase = 64  // word offsets into the mirrors — the offset plumbing must survive the port
            let vbase = 160
            let scale = 0.125
            for (hs in [32, 128, 256]) {   // dpl 1 / 4 / 8 — every guarded accumulator arm
                let qd = nheads * hs
                let kvd = (nheads / kv_mul) * hs
                let kpos = w0 + npos   // mirrors hold absolute positions [0, w0+npos)
                var qh : array<float>
                var kh : array<float>
                var vh : array<float>
                qh |> resize(npos * qd)
                kh |> resize(kbase + kpos * kvd)
                vh |> resize(vbase + kpos * kvd)
                for (i in range(npos * qd)) {
                    qh[i] = float((i * 41) % 73) * 0.05 - 1.8
                }
                for (i in range(kbase + kpos * kvd)) {
                    kh[i] = float((i * 17) % 97) * 0.03 - 1.4
                }
                for (i in range(vbase + kpos * kvd)) {
                    vh[i] = float((i * 53) % 89) * 0.04 - 1.7
                }
                let q_bytes = int64(npos * qd) * 4l
                let k_bytes = int64(kbase + kpos * kvd) * 4l
                let v_bytes = int64(vbase + kpos * kvd) * 4l
                let qdev = make_device_buf(q_bytes)
                let kdev = make_device_buf(k_bytes)
                let vdev = make_device_buf(v_bytes)
                let yd2 = make_device_buf(q_bytes)
                var host = make_host_buf(q_bytes, true, [cached = true])
                var y_cls : array<float>
                y_cls |> resize(npos * qd)
                let wgs = nheads * ((npos + 7) / 8)
                unsafe {
                    upload_region_at(qdev, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                    upload_region_at(kdev, 0l, addr<uint8 const?>(kh[0]), k_bytes)
                    upload_region_at(vdev, 0l, addr<uint8 const?>(vh[0]), v_bytes)
                    // the class kernel: q@0, K@1, V@2, out@3
                    var sc = set_da_attn_b_cls(fixed_array(qdev, kdev, vdev, yd2),
                        fixed_array(q_bytes, k_bytes, v_bytes, q_bytes),
                        fixed_array(1u, 2u, 8u, 16u))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = DaAttnBArgs(npos = uint(npos), nh = uint(nheads), kvd = uint(kvd),
                        hs = uint(hs), kv_mul = uint(kv_mul), kbase = uint(kbase),
                        vbase = uint(vbase), w0 = uint(w0), scale = scale)
                    enc_da_attn_b_cls(raw2, h2, sc, pc, int64(wgs))
                    vhz_dep(raw2, h2, 16u, 0u, true)
                    cmd_copy_whole(raw2, yd2, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
                }
                // oracle: causal rows over the offset mirrors (row r attends [0, w0 + r])
                var y_ref : array<float>
                y_ref |> resize(npos * qd)
                for (r in range(npos)) {
                    for (hh in range(nheads)) {
                        attn_row_oracle(qh, r * qd + hh * hs, kh, kbase, vh, vbase, kvd, hh, hs,
                            kv_mul, w0 + r + 1, scale, y_ref, r * qd + hh * hs)
                    }
                }
                let bad = mismatch_approx(y_cls, y_ref)
                t |> success(bad == 0, "da_attn_b hs {hs} matches the CPU oracle ({bad} of {npos * qd} off)")
                // the softcap arm (gemma-2): the same tile under a cap the scaled scores overrun
                let cap = 4.0
                unsafe {
                    var sc2 = set_da_attn_b_cls(fixed_array(qdev, kdev, vdev, yd2),
                        fixed_array(q_bytes, k_bytes, v_bytes, q_bytes),
                        fixed_array(1u, 2u, 8u, 16u))
                    var raw3 = alloc_cmd()
                    let begin3 = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw3, begin3), null)
                    var h3 : VkHaz
                    var pc3 = DaAttnBArgs(npos = uint(npos), nh = uint(nheads), kvd = uint(kvd),
                        hs = uint(hs), kv_mul = uint(kv_mul), kbase = uint(kbase),
                        vbase = uint(vbase), w0 = uint(w0), scale = scale, cap = cap)
                    enc_da_attn_b_cls(raw3, h3, sc2, pc3, int64(wgs))
                    vhz_dep(raw3, h3, 16u, 0u, true)
                    cmd_copy_whole(raw3, yd2, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw3), null)
                    submit_wait(raw3)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
                }
                let moved = mismatch_approx(y_cls, y_ref)
                t |> success(moved > 0, "control: the {cap} cap moves the uncapped rows ({moved} of {npos * qd})")
                for (r in range(npos)) {
                    for (hh in range(nheads)) {
                        attn_row_oracle(qh, r * qd + hh * hs, kh, kbase, vh, vbase, kvd, hh, hs,
                            kv_mul, w0 + r + 1, scale, y_ref, r * qd + hh * hs, 0, cap)
                    }
                }
                let badc = mismatch_approx(y_cls, y_ref)
                t |> success(badc == 0, "da_attn_b hs {hs} under a {cap} softcap matches the CPU oracle ({badc} of {npos * qd} off)")
                delete qh
                delete kh
                delete vh
                delete y_ref
                delete y_cls
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
            let nheads = 4
            let kv_mul = 4      // one kv head, the gemma-4 global layers' shape
            let kbase = 64      // word offsets into the mirrors — the offset plumbing must survive the port
            let vbase = 160
            let scale = 1.0     // the gemma-4 score scale
            for (arm in range(4)) {
                let hs = arm == 0 ? 384 : 512
                let npos = arm >= 2 ? 120 : 13    //! 13: a partial 4-tile and a stage crossing; 120: thirty 4-tiles over a 620-position mirror (the second prefill window's shape)
                let w0 = arm >= 2 ? 500 : 32
                let window = arm == 3 ? 24 : 0    //! the last arm slides: every row sees the 24 positions up to its own
                let qd = nheads * hs
                let kvd = (nheads / kv_mul) * hs
                let kpos = w0 + npos
                var qh : array<float>
                var kh : array<float>
                var vh : array<float>
                qh |> resize(npos * qd)
                kh |> resize(kbase + kpos * kvd)
                vh |> resize(vbase + kpos * kvd)
                for (i in range(npos * qd)) {
                    qh[i] = float((i * 41) % 73) * 0.005 - 0.18
                }
                for (i in range(kbase + kpos * kvd)) {
                    kh[i] = float((i * 17) % 97) * 0.003 - 0.14
                }
                for (i in range(vbase + kpos * kvd)) {
                    vh[i] = float((i * 53) % 89) * 0.04 - 1.7
                }
                let q_bytes = int64(npos * qd) * 4l
                let k_bytes = int64(kbase + kpos * kvd) * 4l
                let v_bytes = int64(vbase + kpos * kvd) * 4l
                let qdev = make_device_buf(q_bytes)
                let kdev = make_device_buf(k_bytes)
                let vdev = make_device_buf(v_bytes)
                let yd2 = make_device_buf(q_bytes)
                var host = make_host_buf(q_bytes, true, [cached = true])
                var y_cls : array<float>
                y_cls |> resize(npos * qd)
                let wgs = nheads * ((npos + 3) / 4)
                unsafe {
                    upload_region_at(qdev, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                    upload_region_at(kdev, 0l, addr<uint8 const?>(kh[0]), k_bytes)
                    upload_region_at(vdev, 0l, addr<uint8 const?>(vh[0]), v_bytes)
                    nan_sentinel_upload(yd2, q_bytes)
                    var sc = set_da_attn_bw_cls(fixed_array(qdev, kdev, vdev, yd2),
                        fixed_array(q_bytes, k_bytes, v_bytes, q_bytes),
                        fixed_array(1u, 2u, 8u, 16u))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = DaAttnBArgs(npos = uint(npos), nh = uint(nheads), kvd = uint(kvd),
                        hs = uint(hs), kv_mul = uint(kv_mul), kbase = uint(kbase),
                        vbase = uint(vbase), w0 = uint(w0), scale = scale, window = uint(window))
                    enc_da_attn_bw_cls(raw2, h2, sc, pc, int64(wgs))
                    vhz_dep(raw2, h2, 16u, 0u, true)
                    cmd_copy_whole(raw2, yd2, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
                }
                // oracle: causal rows over the offset mirrors (row r attends [kstart, w0 + r]), kstart the window's start
                var y_ref : array<float>
                y_ref |> resize(npos * qd)
                for (r in range(npos)) {
                    let cnt = w0 + r + 1
                    let kstart = window > 0 && cnt > window ? cnt - window : 0
                    for (hh in range(nheads)) {
                        attn_row_oracle(qh, r * qd + hh * hs, kh, kbase, vh, vbase, kvd, hh, hs,
                            kv_mul, cnt, scale, y_ref, r * qd + hh * hs, kstart)
                    }
                }
                let bad = mismatch_approx(y_cls, y_ref)
                t |> success(bad == 0, "da_attn_bw hs {hs} npos {npos} window {window} matches the CPU oracle ({bad} of {npos * qd} off)")
                y_ref[0] += 0.5
                t |> success(mismatch_approx(y_cls, y_ref) > 0, "da_attn_bw hs {hs} npos {npos} window {window}: the bar reds a poisoned oracle element")
                delete qh
                delete kh
                delete vh
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
            let hs = 128
            let nheads = 4
            let kv_mul = 2
            let npos = 45   // qtiles=2: the second tile has 13 valid rows — the row masks run
            let w0 = 32
            let kbase = 64
            let vbase = 160
            let scale = 0.125
            let qd = nheads * hs
            let kvd = (nheads / kv_mul) * hs
            let kpos = w0 + npos
            var qh : array<float>
            var kh : array<float>
            var vh : array<float>
            qh |> resize(npos * qd)
            kh |> resize(kbase + kpos * kvd)
            vh |> resize(vbase + kpos * kvd)
            for (i in range(npos * qd)) {
                qh[i] = float((i * 41) % 73) * 0.05 - 1.8
            }
            for (i in range(kbase + kpos * kvd)) {
                kh[i] = float((i * 17) % 97) * 0.03 - 1.4
            }
            for (i in range(vbase + kpos * kvd)) {
                vh[i] = float((i * 53) % 89) * 0.04 - 1.7
            }
            let q_bytes = int64(npos * qd) * 4l
            let k_bytes = int64(kbase + kpos * kvd) * 4l
            let v_bytes = int64(vbase + kpos * kvd) * 4l
            let qdev = make_device_buf(q_bytes)
            let kdev = make_device_buf(k_bytes)
            let vdev = make_device_buf(v_bytes)
            let yd2 = make_device_buf(q_bytes)
            var host = make_host_buf(q_bytes, true, [cached = true])
            var y_cls : array<float>
            y_cls |> resize(npos * qd)
            let wgs = nheads * ((npos + 31) / 32)
            unsafe {
                upload_region_at(qdev, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                upload_region_at(kdev, 0l, addr<uint8 const?>(kh[0]), k_bytes)
                upload_region_at(vdev, 0l, addr<uint8 const?>(vh[0]), v_bytes)
                // the class kernel: q@0, K@1, V@2, out@3
                var sc = set_da_attn_b_h128_cls(fixed_array(qdev, kdev, vdev, yd2),
                    fixed_array(q_bytes, k_bytes, v_bytes, q_bytes),
                    fixed_array(1u, 2u, 8u, 16u))
                var raw2 = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw2, begin), null)
                var h2 : VkHaz
                var pc = DaAttnBArgs(npos = uint(npos), nh = uint(nheads), kvd = uint(kvd),
                    hs = uint(hs), kv_mul = uint(kv_mul), kbase = uint(kbase),
                    vbase = uint(vbase), w0 = uint(w0), scale = scale)
                enc_da_attn_b_h128_cls(raw2, h2, sc, pc, int64(wgs))
                vhz_dep(raw2, h2, 16u, 0u, true)
                cmd_copy_whole(raw2, yd2, host.buf, q_bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
            }
            // the h128 twin stages q/K/V through f16 pairs for the MMAs — floor-barred like the
            // cm GEMMs (softmax weights bound the V fold, so the peak floor is generous enough)
            var y_ref : array<float>
            y_ref |> resize(npos * qd)
            for (r in range(npos)) {
                for (hh in range(nheads)) {
                    attn_row_oracle(qh, r * qd + hh * hs, kh, kbase, vh, vbase, kvd, hh, hs,
                        kv_mul, w0 + r + 1, scale, y_ref, r * qd + hh * hs)
                }
            }
            let bad = mismatch_bars(y_cls, y_ref, 2e-2, 4e-3 * max_abs(y_ref))
            t |> success(bad == 0, "da_attn_b_h128 matches the CPU oracle ({bad} of {npos * qd} off)")
            delete qh
            delete kh
            delete vh
            delete y_ref
            delete y_cls
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_requant_family(t0 : T?) {
    t0 |> run("fused ar+requant + q8k requant class kernels == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_cls_ar_rq() && ensure_cls_q8k_rq() && ensure_cls_ar_rqx() && ensure_cls_ar_rqk() && ensure_cls_ar_rqkx())) {
                t |> skip("no Vulkan device")
                return
            }
            let begin = VkCommandBufferBeginInfo()
            // ===== the fused ar + Q8_0 requant =====
            {
                let dim = 1024
                let eps = 1e-5
                let ascale = 1.25
                let nblk = dim / 32
                var x : array<float>
                var a : array<float>
                var w : array<float>
                x |> resize(dim)
                a |> resize(dim)
                w |> resize(dim)
                for (i in range(dim)) {
                    x[i] = float((i * 37) % 101) * 0.09 - 4.2
                    a[i] = float((i * 13) % 89) * 0.06 - 2.6
                    w[i] = 0.5 + float(i % 17) * 0.04
                }
                let e_bytes = int64(dim) * 4l
                let q_bytes = int64(dim)
                let s_bytes = int64(nblk) * 4l
                let x2 = make_device_buf(e_bytes)
                let ad = make_device_buf(e_bytes)
                let wd = make_device_buf(e_bytes)
                let q2 = make_device_buf(q_bytes)
                let s2 = make_device_buf(s_bytes)
                var host = make_host_buf(e_bytes, true, [cached = true])
                var q_cls : array<uint>
                var s_cls : array<float>
                var x_cls : array<float>
                q_cls |> resize(dim / 4)
                s_cls |> resize(nblk)
                x_cls |> resize(dim)
                unsafe {
                    upload_region_at(x2, 0l, addr<uint8 const?>(x[0]), e_bytes)
                    upload_region_at(ad, 0l, addr<uint8 const?>(a[0]), e_bytes)
                    upload_region_at(wd, 0l, addr<uint8 const?>(w[0]), e_bytes)
                    // the class kernel: x@0, a@1, w@2, outq@3, outs@4
                    var sc = set_cls_ar_rq(fixed_array(x2, ad, wd, q2, s2),
                        fixed_array(e_bytes, e_bytes, e_bytes, q_bytes, s_bytes),
                        fixed_array(1u, 2u, 4u, 16u, 8u))
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = ArArgs(dim = uint(dim), add_on = 1u, eps = eps, ascale = ascale)
                    enc_cls_ar_rq(raw2, h2, sc, pc, 1l)
                    vhz_dep(raw2, h2, 1u | 8u | 16u, 0u, true)
                    cmd_copy_whole(raw2, q2, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(q_cls[0]), host.mapped, q_bytes)
                    var raw2b = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2b, begin), null)
                    cmd_copy_whole(raw2b, s2, host.buf, s_bytes)
                    vk_check(vkEndCommandBuffer(raw2b), null)
                    submit_wait(raw2b)
                    memcpy(addr<void?>(s_cls[0]), host.mapped, s_bytes)
                    var raw2c = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2c, begin), null)
                    cmd_copy_whole(raw2c, x2, host.buf, e_bytes)
                    vk_check(vkEndCommandBuffer(raw2c), null)
                    submit_wait(raw2c)
                    memcpy(addr<void?>(x_cls[0]), host.mapped, e_bytes)
                }
                // oracle: x' = (x+a)*ascale in place, rms over the row, y = w*(x'*inv), Q8_0 pack —
                // the wg reduce reassociates the sum of squares, so quants ride the byte bar
                var x_ref : array<float>
                var y_ref : array<float>
                x_ref |> resize(dim)
                y_ref |> resize(dim)
                var ss = 0.0
                for (i in range(dim)) {
                    x_ref[i] = (x[i] + a[i]) * ascale
                    ss += x_ref[i] * x_ref[i]
                }
                let inv = 1.0 / sqrt(ss / float(dim) + eps)
                for (i in range(dim)) {
                    y_ref[i] = w[i] * (x_ref[i] * inv)
                }
                var q_ref : array<uint>
                var s_ref : array<float>
                requant_oracle(y_ref, 0, dim, false, q_ref, s_ref)
                let bad = (mismatch_qbytes(q_cls, q_ref, 1) + mismatch_approx(s_cls, s_ref)
                    + mismatch_approx(x_cls, x_ref))
                t |> success(bad == 0, "ar+requant fused matches the CPU oracle ({bad} elements off)")
                //! the biased add partner (gpt-oss's output bias) on the fused stamp: the bias row rides the norms plane past the norm row
                {
                    var b : array<float>
                    var wb : array<float>
                    b |> resize(dim)
                    wb |> resize(2 * dim)
                    for (i in range(dim)) {
                        b[i] = float((i * 23) % 71) * 0.09 - 3.1
                        wb[i] = w[i]
                        wb[dim + i] = b[i]
                    }
                    let wbd = make_device_buf(2l * e_bytes)
                    var qb_cls : array<uint>
                    var sb_cls : array<float>
                    var xb_cls : array<float>
                    qb_cls |> resize(dim / 4)
                    sb_cls |> resize(nblk)
                    xb_cls |> resize(dim)
                    unsafe {
                        upload_region_at(x2, 0l, addr<uint8 const?>(x[0]), e_bytes)
                        upload_region_at(wbd, 0l, addr<uint8 const?>(wb[0]), 2l * e_bytes)
                        var scb = set_cls_ar_rq(fixed_array(x2, ad, wbd, q2, s2),
                            fixed_array(e_bytes, e_bytes, 2l * e_bytes, q_bytes, s_bytes),
                            fixed_array(1u, 2u, 4u, 16u, 8u))
                        var rawb = alloc_cmd()
                        vk_check(vkBeginCommandBuffer(rawb, begin), null)
                        var hb : VkHaz
                        var pcb = ArArgs(dim = uint(dim), add_on = 1u, eps = eps, ascale = ascale, abias_on = 1u, aboff = uint(dim))
                        enc_cls_ar_rq(rawb, hb, scb, pcb, 1l)
                        vhz_dep(rawb, hb, 1u | 8u | 16u, 0u, true)
                        cmd_copy_whole(rawb, q2, host.buf, q_bytes)
                        vk_check(vkEndCommandBuffer(rawb), null)
                        submit_wait(rawb)
                        memcpy(addr<void?>(qb_cls[0]), host.mapped, q_bytes)
                        var rawb2 = alloc_cmd()
                        vk_check(vkBeginCommandBuffer(rawb2, begin), null)
                        cmd_copy_whole(rawb2, s2, host.buf, s_bytes)
                        vk_check(vkEndCommandBuffer(rawb2), null)
                        submit_wait(rawb2)
                        memcpy(addr<void?>(sb_cls[0]), host.mapped, s_bytes)
                        var rawb3 = alloc_cmd()
                        vk_check(vkBeginCommandBuffer(rawb3, begin), null)
                        cmd_copy_whole(rawb3, x2, host.buf, e_bytes)
                        vk_check(vkEndCommandBuffer(rawb3), null)
                        submit_wait(rawb3)
                        memcpy(addr<void?>(xb_cls[0]), host.mapped, e_bytes)
                    }
                    var xb_ref : array<float>
                    var yb_ref : array<float>
                    xb_ref |> resize(dim)
                    yb_ref |> resize(dim)
                    var ssb = 0.0
                    for (i in range(dim)) {
                        xb_ref[i] = (x[i] + a[i] + b[i]) * ascale
                        ssb += xb_ref[i] * xb_ref[i]
                    }
                    let invb = 1.0 / sqrt(ssb / float(dim) + eps)
                    for (i in range(dim)) {
                        yb_ref[i] = w[i] * (xb_ref[i] * invb)
                    }
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
                let sk_bytes = int64(dim / 256) * 4l
                var host3 = make_host_buf(q_bytes + s_bytes + e_bytes, true, [cached = true])
                let stamps <- fixed_array(
                    (name = "cls_ar_rqx", kform = false, storex = true),
                    (name = "cls_ar_rqk", kform = true, storex = false),
                    (name = "cls_ar_rqkx", kform = true, storex = true))
                for (stamp in stamps) {
                    let kform = stamp.kform
                    let storex = stamp.storex
                    let q3 = make_device_buf(q_bytes)
                    let s3 = make_device_buf(s_bytes)
                    let y3 = make_device_buf(e_bytes)
                    let sb = kform ? sk_bytes : s_bytes
                    var q_got : array<uint>
                    var s_got : array<float>
                    var y_got : array<float>
                    q_got |> resize(dim / 4)
                    s_got |> resize(kform ? dim / 256 : nblk)
                    y_got |> resize(dim)
                    unsafe {
                        upload_region_at(x2, 0l, addr<uint8 const?>(x[0]), e_bytes)
                        nan_sentinel_upload(q3, q_bytes)
                        nan_sentinel_upload(s3, s_bytes)
                        nan_sentinel_upload(y3, e_bytes)
                        var raw3 = alloc_cmd()
                        vk_check(vkBeginCommandBuffer(raw3, begin), null)
                        var h3 : VkHaz
                        var pc3 = ArArgs(dim = uint(dim), add_on = 1u, eps = eps, ascale = ascale)
                        if (stamp.name == "cls_ar_rqx") {
                            var sc3 = set_cls_ar_rqx(fixed_array(x2, ad, wd, q3, s3, y3),
                                fixed_array(e_bytes, e_bytes, e_bytes, q_bytes, sb, e_bytes), fixed_array(1u, 2u, 4u, 16u, 8u, 32u))
                            enc_cls_ar_rqx(raw3, h3, sc3, pc3, 1l)
                        } elif (stamp.name == "cls_ar_rqk") {
                            var sc3 = set_cls_ar_rqk(fixed_array(x2, ad, wd, q3, s3),
                                fixed_array(e_bytes, e_bytes, e_bytes, q_bytes, sb), fixed_array(1u, 2u, 4u, 16u, 8u))
                            enc_cls_ar_rqk(raw3, h3, sc3, pc3, 1l)
                        } else {
                            var sc3 = set_cls_ar_rqkx(fixed_array(x2, ad, wd, q3, s3, y3),
                                fixed_array(e_bytes, e_bytes, e_bytes, q_bytes, sb, e_bytes), fixed_array(1u, 2u, 4u, 16u, 8u, 32u))
                            enc_cls_ar_rqkx(raw3, h3, sc3, pc3, 1l)
                        }
                        vhz_dep(raw3, h3, 1u | 8u | 16u | 32u, 0u, true)
                        cmd_copy_range(raw3, q3, 0l, host3.buf, 0l, q_bytes)
                        cmd_copy_range(raw3, s3, 0l, host3.buf, q_bytes, sb)
                        cmd_copy_range(raw3, y3, 0l, host3.buf, q_bytes + s_bytes, e_bytes)
                        vk_check(vkEndCommandBuffer(raw3), null)
                        submit_wait(raw3)
                        let hp3 = intptr(host3.mapped)
                        memcpy(addr<void?>(q_got[0]), host3.mapped, q_bytes)
                        memcpy(addr<void?>(s_got[0]), reinterpret<void?>(hp3 + uint64(q_bytes)), sb)
                        memcpy(addr<void?>(y_got[0]), reinterpret<void?>(hp3 + uint64(q_bytes + s_bytes)), e_bytes)
                    }
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
                var src : array<float>
                src |> resize(nelem)
                for (i in range(nelem)) {
                    src[i] = float((i * 61) % 113) * 0.05 - 2.8
                }
                let e_bytes = int64(nelem) * 4l
                let q_bytes = int64(nelem)
                let s_bytes = int64(nsb) * 4l
                let sd = make_device_buf(e_bytes)
                let q2 = make_device_buf(q_bytes)
                let s2 = make_device_buf(s_bytes)
                var host = make_host_buf(q_bytes, true, [cached = true])
                var q_cls : array<uint>
                var s_cls : array<float>
                q_cls |> resize(nelem / 4)
                s_cls |> resize(nsb)
                let wgs = (nsb * 32 + 255) / 256
                unsafe {
                    upload_region_at(sd, 0l, addr<uint8 const?>(src[0]), e_bytes)
                    // the class kernel: src@0, outq@1, outs@2
                    var sc = set_rq_cls(fixed_array(sd, q2, s2),
                        fixed_array(e_bytes, q_bytes, s_bytes),
                        fixed_array(1u, 4u, 8u))
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = RqArgs(inbase = 0u, nblk = uint(nsb))
                    enc_cls_q8k_rq(raw2, h2, sc, pc, int64(wgs))
                    vhz_dep(raw2, h2, 4u | 8u, 0u, true)
                    cmd_copy_whole(raw2, q2, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(q_cls[0]), host.mapped, q_bytes)
                    var raw2b = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2b, begin), null)
                    cmd_copy_whole(raw2b, s2, host.buf, s_bytes)
                    vk_check(vkEndCommandBuffer(raw2b), null)
                    submit_wait(raw2b)
                    memcpy(addr<void?>(s_cls[0]), host.mapped, s_bytes)
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
            for (n in [704, 1408]) {   // 22 blocks: one subgroup pass with idle lanes; 44: a second, partial pass
                let nb = n / 32
                let d = 3
                let nreg = 2
                let rows = nreg * d
                let totblk = rows * nb
                let wq_bytes = int64(totblk) * 20l
                let ws_bytes = int64(totblk) * 4l
                var wqh : array<uint>
                var wsh : array<uint>
                var xqh : array<uint>
                var xsh : array<float>
                wqh |> resize(totblk * 5)
                wsh |> resize(totblk)
                xqh |> resize(nb * 8)
                xsh |> resize(nb)
                for (i in range(totblk * 5)) {
                    wqh[i] = hash_word(uint(i) + 61u)
                }
                for (b in range(totblk)) {
                    wsh[b] = packHalf2x16(float2(0.002 * float(1 + b % 7), 0.001 * float(b % 5) - 0.002))
                }
                for (i in range(nb * 8)) {
                    xqh[i] = hash_word(uint(i) * 19u + 7u)
                }
                for (i in range(nb)) {
                    xsh[i] = 0.25 + float(i % 5) * 0.125
                }
                let xq_bytes = int64(nb) * 32l
                let xs_bytes = int64(nb) * 4l
                let y_bytes = int64(rows) * 4l
                let wqd = make_device_buf(wq_bytes)
                let wsd = make_device_buf(ws_bytes)
                let xqd = make_device_buf(xq_bytes)
                let xsd = make_device_buf(xs_bytes)
                let yd2 = make_device_buf(y_bytes)
                let rgd = make_device_buf(int64(nreg) * 8l)
                var host = make_host_buf(y_bytes, true, [cached = true])
                var y_cls : array<float>
                y_cls |> resize(rows)
                var regs : array<uint>
                regs |> resize(nreg * 2)
                for (r in range(nreg)) {
                    regs[r * 2] = uint(r * d * nb)
                    regs[r * 2 + 1] = 0u
                }
                unsafe {
                    upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                    upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
                    upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                    upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                    upload_region_at(rgd, 0l, addr<uint8 const?>(regs[0]), int64(nreg) * 8l)
                    nan_sentinel_upload(yd2, y_bytes)
                    var s_cls = gemv_cls_set(int(KqFmt.q51), fixed_array(wqd, wsd, rgd, xqd, xsd, yd2),
                        fixed_array(wq_bytes, ws_bytes, int64(nreg) * 8l, xq_bytes, xs_bytes, y_bytes),
                        fixed_array(1u, 2u, 4u, 8u, 16u, 32u))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = GemvArgs(n = uint(n), d = uint(d), nreg = uint(nreg), ybase = 0u)
                    gemv_enc(int(KqFmt.q51), raw2, h2, s_cls, pc, int64(rows))
                    vhz_dep(raw2, h2, 32u, 0u, true)
                    cmd_copy_whole(raw2, yd2, host.buf, y_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
                }
                var y_ref : array<float>
                y_ref |> resize(rows)
                for (r in range(nreg)) {
                    for (row in range(d)) {
                        y_ref[r * d + row] = q51_gemv_row_oracle(wqh, wsh, xqh, xsh, r * d * nb + row * nb, nb)
                    }
                }
                let bad = mismatch_approx(y_cls, y_ref)
                t |> success(bad == 0, "q51 GEMV at n {n} matches the scalar dot ({bad} of {rows} rows off)")
                //! the summed form: the two regions' rows folded under their weights inside the GEMV, one row of d out
                if (ensure_q51_gemv_sum_cls()) {
                    var wgt <- [0.625, -0.375]
                    let ys_bytes = int64(d) * 4l
                    let ysd = make_device_buf(ys_bytes)
                    let swd = make_device_buf(8l)
                    var ys_cls : array<float>
                    ys_cls |> resize(d)
                    unsafe {
                        upload_region_at(swd, 0l, addr<uint8 const?>(wgt[0]), 8l)
                        nan_sentinel_upload(ysd, ys_bytes)
                        var s_sum = set_q51_gemv_sum_cls(fixed_array(wqd, wsd, rgd, xqd, xsd, ysd, swd),
                            fixed_array(wq_bytes, ws_bytes, int64(nreg) * 8l, xq_bytes, xs_bytes, ys_bytes, 8l),
                            fixed_array(1u, 2u, 4u, 8u, 16u, 32u, 64u))
                        var raw3 = alloc_cmd()
                        let begin3 = VkCommandBufferBeginInfo()
                        vk_check(vkBeginCommandBuffer(raw3, begin3), null)
                        var h3 : VkHaz
                        var pcs = GemvArgs(n = uint(n), d = uint(d), nreg = uint(nreg), ybase = 0u)
                        enc_q51_gemv_sum_cls(raw3, h3, s_sum, pcs, gemv_wgs(int64(d), 0u))
                        vhz_dep(raw3, h3, 32u, 0u, true)
                        cmd_copy_whole(raw3, ysd, host.buf, ys_bytes)
                        vk_check(vkEndCommandBuffer(raw3), null)
                        submit_wait(raw3)
                        memcpy(addr<void?>(ys_cls[0]), host.mapped, ys_bytes)
                    }
                    var ys_ref : array<float>
                    ys_ref |> resize(d)
                    for (row in range(d)) {
                        ys_ref[row] = wgt[0] * y_ref[row] + wgt[1] * y_ref[d + row]
                    }
                    let bads = mismatch_approx(ys_cls, ys_ref)
                    t |> success(bads == 0, "q51 GEMV summed under the weights at n {n} matches the weighted rows ({bads} of {d} off)")
                    var ys_poison := ys_cls
                    ys_poison[0] += 1.0 + 2.0 * max_abs(ys_ref)
                    t |> success(mismatch_approx(ys_poison, ys_ref) > 0, "q51 GEMV summed: the bar reds a poisoned row")
                    delete wgt
                    delete ys_cls
                    delete ys_ref
                    delete ys_poison
                }
                var y_poison := y_cls   // the bar's control
                y_poison[0] += 1.0 + 2.0 * max_abs(y_ref)
                t |> success(mismatch_approx(y_poison, y_ref) > 0, "the q51 GEMV bar reds a poisoned element")
                delete y_poison
                delete regs
                delete y_ref
                delete wqh
                delete wsh
                delete xqh
                delete xsh
                delete y_cls
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
            for (n in [704, 2880]) {   // 22 blocks: one subgroup pass with idle lanes; 90: gpt-oss's width, three passes, the last partial
                let nb = n / 32
                let d = 3
                let nreg = 2
                let rows = nreg * d
                let totblk = rows * nb
                let wq_bytes = int64(totblk) * 16l
                var wqh : array<uint>
                var xqh : array<uint>
                var xsh : array<float>
                wqh |> resize(totblk * 4)
                xqh |> resize(nb * 8)
                xsh |> resize(nb)
                for (i in range(totblk * 4)) {
                    wqh[i] = hash_word(uint(i) + 67u)
                }
                var wsh <- mx4_scale_words(totblk)
                let ws_bytes = long_length(wsh) * 4l
                for (i in range(nb * 8)) {
                    xqh[i] = hash_word(uint(i) * 19u + 7u)
                }
                for (i in range(nb)) {
                    xsh[i] = 0.25 + float(i % 5) * 0.125
                }
                let xq_bytes = int64(nb) * 32l
                let xs_bytes = int64(nb) * 4l
                let y_bytes = int64(rows) * 4l
                let wqd = make_device_buf(wq_bytes)
                let wsd = make_device_buf(ws_bytes)
                let xqd = make_device_buf(xq_bytes)
                let xsd = make_device_buf(xs_bytes)
                let yd2 = make_device_buf(y_bytes)
                let rgd = make_device_buf(int64(nreg) * 8l)
                var host = make_host_buf(y_bytes, true, [cached = true])
                var y_cls : array<float>
                y_cls |> resize(rows)
                var regs : array<uint>
                regs |> resize(nreg * 2)
                for (r in range(nreg)) {
                    regs[r * 2] = uint(r * d * nb)
                    regs[r * 2 + 1] = 0u
                }
                unsafe {
                    upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                    upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
                    upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                    upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                    upload_region_at(rgd, 0l, addr<uint8 const?>(regs[0]), int64(nreg) * 8l)
                    nan_sentinel_upload(yd2, y_bytes)
                    var s_cls = gemv_cls_set(int(KqFmt.mx4), fixed_array(wqd, wsd, rgd, xqd, xsd, yd2),
                        fixed_array(wq_bytes, ws_bytes, int64(nreg) * 8l, xq_bytes, xs_bytes, y_bytes),
                        fixed_array(1u, 2u, 4u, 8u, 16u, 32u))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = GemvArgs(n = uint(n), d = uint(d), nreg = uint(nreg), ybase = 0u)
                    gemv_enc(int(KqFmt.mx4), raw2, h2, s_cls, pc, int64(rows))
                    vhz_dep(raw2, h2, 32u, 0u, true)
                    cmd_copy_whole(raw2, yd2, host.buf, y_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
                }
                var y_ref : array<float>
                y_ref |> resize(rows)
                for (r in range(nreg)) {
                    for (row in range(d)) {
                        y_ref[r * d + row] = mx4_gemv_row_oracle(wqh, wsh, xqh, xsh, r * d * nb + row * nb, nb)
                    }
                }
                let bad = mismatch_approx(y_cls, y_ref)
                t |> success(bad == 0, "mx4 GEMV at n {n} matches the scalar dot ({bad} of {rows} rows off)")
                var y_poison := y_cls   // the bar's control
                y_poison[0] += 1.0 + 2.0 * max_abs(y_ref)
                t |> success(mismatch_approx(y_poison, y_ref) > 0, "the mx4 GEMV bar reds a poisoned element")
                delete y_poison
                delete regs
                delete y_ref
                delete wqh
                delete wsh
                delete xqh
                delete xsh
                delete y_cls
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the fused mx4 gate + up + act + requant on one fixture: the gate stack's rows then the up stack's in one plane, the slots'
//! regions on both, a six-expert bias plane and the slots' experts; the fused dispatch against the oracle (the bias added before
//! the clamped swiglu), then the split path - two GEMVs and the biased requant - byte for byte
def private mx4_gemv_gu_run(t : T?; d, nreg : int; bias : bool) {   // nolint:STYLE037 — one linear fixture: the planes, the fused dispatch, the oracle bars, the split path's byte compare
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let n = 2880   //! gpt-oss's routed width: 90 blocks a row
        let nb = n / 32
        let rows = nreg * d
        let nblk = rows / 32
        let ne = 6
        let totblk = 2 * rows * nb   // the gate stack's blocks, then the up stack's
        var wqh : array<uint>
        var xqh : array<uint>
        var xsh : array<float>
        var gb : array<float>
        var ub : array<float>
        var rex : array<uint>
        wqh |> resize(totblk * 4)
        xqh |> resize(nb * 8)
        xsh |> resize(nb)
        gb |> resize(ne * d)
        ub |> resize(ne * d)
        rex |> resize(nreg)
        for (i in range(totblk * 4)) {
            wqh[i] = hash_word(uint(i) * 5u + 977u)
        }
        var wsh <- mx4_scale_words(totblk)
        for (i in range(nb * 8)) {
            xqh[i] = hash_word(uint(i) * 29u + 11u)
        }
        for (i in range(nb)) {
            xsh[i] = 0.25 + float(i % 5) * 0.125
        }
        for (i in range(ne * d)) {   //! biases of the dots' own magnitude, some past the 7 clamp
            gb[i] = bias ? float((i * 13) % 31) * 0.5 - 7.5 : 0.0
            ub[i] = bias ? float((i * 17) % 29) * 0.6 - 8.4 : 0.0
        }
        for (r in range(nreg)) {
            rex[r] = uint((r * 5 + 2) % ne)
        }
        var regs : array<uint>
        var regs3 : array<uint>
        regs |> resize(nreg * 2)
        regs3 |> resize(nreg * 2)
        for (r in range(nreg)) {
            regs[r * 2] = uint(r * d * nb)
            regs[r * 2 + 1] = 0u
            regs3[r * 2] = uint((nreg + r) * d * nb)
            regs3[r * 2 + 1] = 0u
        }
        let act = bias ? ACT_OAI : ACT_SILU
        let wq_bytes = int64(totblk) * 16l
        let ws_bytes = long_length(wsh) * 4l
        let xq_bytes = int64(nb) * 32l
        let xs_bytes = int64(nb) * 4l
        let rg_bytes = int64(nreg) * 8l
        let b_bytes = int64(ne * d) * 4l
        let y_bytes = int64(rows) * 4l
        let oq_bytes = int64(rows)
        let os_bytes = int64(nblk) * 4l
        let wqd = make_device_buf(wq_bytes)
        let wsd = make_device_buf(ws_bytes)
        let xqd = make_device_buf(xq_bytes)
        let xsd = make_device_buf(xs_bytes)
        let rgd = make_device_buf(rg_bytes)
        let rg3d = make_device_buf(rg_bytes)
        let gbd = make_device_buf(b_bytes)
        let ubd = make_device_buf(b_bytes)
        let rexd = make_device_buf(rg_bytes)
        let yd = make_device_buf(y_bytes)
        let yud = make_device_buf(y_bytes)
        let oqd = make_device_buf(oq_bytes)
        let osd = make_device_buf(os_bytes)
        let oq2d = make_device_buf(oq_bytes)
        let os2d = make_device_buf(os_bytes)
        var host = make_host_buf(2l * (oq_bytes + os_bytes) + y_bytes, true, [cached = true])
        var oq : array<int8>
        var os : array<float>
        var oq2 : array<int8>
        var os2 : array<float>
        var y_cls : array<float>
        oq |> resize(rows)
        os |> resize(nblk)
        oq2 |> resize(rows)
        os2 |> resize(nblk)
        y_cls |> resize(rows)
        unsafe {
            upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
            upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
            upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
            upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
            upload_region_at(rgd, 0l, addr<uint8 const?>(regs[0]), rg_bytes)
            upload_region_at(rg3d, 0l, addr<uint8 const?>(regs3[0]), rg_bytes)
            upload_region_at(gbd, 0l, addr<uint8 const?>(gb[0]), b_bytes)
            upload_region_at(ubd, 0l, addr<uint8 const?>(ub[0]), b_bytes)
            upload_region_at(rexd, 0l, addr<uint8 const?>(rex[0]), int64(nreg) * 4l)
            nan_sentinel_upload(oqd, oq_bytes)
            nan_sentinel_upload(osd, os_bytes)
            nan_sentinel_upload(yd, y_bytes)
            nan_sentinel_upload(yud, y_bytes)
            nan_sentinel_upload(oq2d, oq_bytes)
            nan_sentinel_upload(os2d, os_bytes)
            var s_cls = set_mx4_gemv_gu_cls(fixed_array(wqd, wsd, rgd, xqd, xsd, yd, rg3d, gbd, ubd, rexd, oqd, osd),
                fixed_array(wq_bytes, ws_bytes, rg_bytes, xq_bytes, xs_bytes, y_bytes, rg_bytes, b_bytes, b_bytes, rg_bytes, oq_bytes, os_bytes),
                fixed_array(1u, 2u, 4u, 8u, 16u, 32u, 64u, 0u, 0u, 64u, 128u, 128u))
            var raw2 = alloc_cmd()
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw2, begin), null)
            var h2 : VkHaz
            var pc = GemvArgs(n = uint(n), d = uint(d), nreg = uint(nreg), ybase = 0u, act = act, bias_on = bias ? 1u : 0u, gboff = 0u, uboff = 0u)
            enc_mx4_gemv_gu_cls(raw2, h2, s_cls, pc, int64(nblk))
            //! the split path on the same planes: the two GEMVs, then the (biased) act + requant
            var s_g = gemv_cls_set(int(KqFmt.mx4), fixed_array(wqd, wsd, rgd, xqd, xsd, yud), fixed_array(wq_bytes, ws_bytes, rg_bytes, xq_bytes, xs_bytes, y_bytes), fixed_array(1u, 2u, 4u, 8u, 16u, 256u))
            let yu2d = make_device_buf(y_bytes)
            nan_sentinel_upload(yu2d, y_bytes)
            var s_u = gemv_cls_set(int(KqFmt.mx4), fixed_array(wqd, wsd, rg3d, xqd, xsd, yu2d), fixed_array(wq_bytes, ws_bytes, rg_bytes, xq_bytes, xs_bytes, y_bytes), fixed_array(1u, 2u, 64u, 8u, 16u, 1024u))
            var pc2 = GemvArgs(n = uint(n), d = uint(d), nreg = uint(nreg), ybase = 0u)
            gemv_enc(int(KqFmt.mx4), raw2, h2, s_g, pc2, int64(rows))
            gemv_enc(int(KqFmt.mx4), raw2, h2, s_u, pc2, int64(rows))
            var pca = ActArgs(nelem = uint(rows), act = act, nblk = uint(nblk), bias_on = bias ? 1u : 0u, gboff = 0u, uboff = 0u, brow = uint(d))
            if (bias) {
                var s_a = set_q8_actrq_b_cls(fixed_array(yud, yu2d, oq2d, os2d, gbd, ubd, rexd),
                    fixed_array(y_bytes, y_bytes, oq_bytes, os_bytes, b_bytes, b_bytes, rg_bytes), fixed_array(256u, 1024u, 512u, 512u, 0u, 0u, 64u))
                enc_q8_actrq_b_cls(raw2, h2, s_a, pca, (int64(rows) / 4l + 255l) / 256l)
            } else {
                var s_a = set_actrq_cls(fixed_array(yud, yu2d, oq2d, os2d), fixed_array(y_bytes, y_bytes, oq_bytes, os_bytes), fixed_array(256u, 1024u, 512u, 512u))
                enc_q8_actrq_cls(raw2, h2, s_a, pca, (int64(rows) / 4l + 255l) / 256l)
            }
            vhz_dep(raw2, h2, 32u | 128u | 512u, 0u, true)
            cmd_copy_whole(raw2, oqd, host.buf, oq_bytes)
            cmd_copy_range(raw2, osd, 0l, host.buf, oq_bytes, os_bytes)
            cmd_copy_range(raw2, yd, 0l, host.buf, oq_bytes + os_bytes, y_bytes)
            cmd_copy_range(raw2, oq2d, 0l, host.buf, oq_bytes + os_bytes + y_bytes, oq_bytes)
            cmd_copy_range(raw2, os2d, 0l, host.buf, 2l * oq_bytes + os_bytes + y_bytes, os_bytes)
            vk_check(vkEndCommandBuffer(raw2), null)
            submit_wait(raw2)
            let hp = intptr(host.mapped)
            memcpy(addr<void?>(oq[0]), host.mapped, oq_bytes)
            memcpy(addr<void?>(os[0]), reinterpret<void?>(hp + uint64(oq_bytes)), os_bytes)
            memcpy(addr<void?>(y_cls[0]), reinterpret<void?>(hp + uint64(oq_bytes + os_bytes)), y_bytes)
            memcpy(addr<void?>(oq2[0]), reinterpret<void?>(hp + uint64(oq_bytes + os_bytes + y_bytes)), oq_bytes)
            memcpy(addr<void?>(os2[0]), reinterpret<void?>(hp + uint64(2l * oq_bytes + os_bytes + y_bytes)), os_bytes)
        }
        var y_g : array<float>
        var y_u : array<float>
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
        y_gp[0] += 1.0 + 2.0 * max_abs(y_g)
        let badgp = mismatch_approx(y_cls, y_gp)
        var bads = 0
        var badq = 0
        var pbad = 0
        for (blk in range(nblk)) {
            var m = 0.0
            for (e in range(32)) {
                let rg = blk * 32 + e
                let ex = int(rex[rg / d])
                let c = rg % d
                m = max(m, abs(act_mul_ref(y_g[rg] + gb[ex * d + c], y_u[rg] + ub[ex * d + c], act)))
            }
            let d_ref = m / 127.0
            if (abs(os[blk] - d_ref) > 2e-3 * max(d_ref, 1e-6)) {
                bads++
            }
            for (e in range(32)) {
                let rg = blk * 32 + e
                let ex = int(rex[rg / d])
                let c = rg % d
                let a = act_mul_ref(y_g[rg] + gb[ex * d + c], y_u[rg] + ub[ex * d + c], act)
                let deq = float(oq[rg]) * os[blk]
                if (abs(deq - a) > d_ref + 2e-3 * abs(a)) {
                    badq++
                }
                if (e == 0 && abs(deq - (a + 3.0 * d_ref + 1.0)) <= d_ref + 2e-3 * abs(a)) {   // the control: the expectation pushed three quant steps must miss
                    pbad++
                }
            }
        }
        let tag = "mx4 gu {nreg} x {d}{bias ? " biased" : ""}"
        t |> success(badg == 0, "{tag}: the gate rows match the scalar dot ({badg} of {rows} off)")
        t |> success(badgp == 1, "{tag}: the gate rows' bar reds one poisoned element ({badgp})")
        delete y_gp
        t |> success(bads == 0, "{tag}: the block scales match ({bads} of {nblk} off)")
        t |> success(badq == 0, "{tag}: the dequantized hidden rows sit within a quant step ({badq} of {rows} off)")
        t |> success(pbad == 0, "{tag}: the bar reds a poisoned oracle element in every block ({pbad} passed)")
        var dq = 0
        var dqmax = 0
        for (e in range(rows)) {
            let dd = abs(int(oq[e]) - int(oq2[e]))
            dq += dd != 0 ? 1 : 0
            dqmax = max(dqmax, dd)
        }
        var ds = 0
        for (b in range(nblk)) {
            ds += os[b] != os2[b] ? 1 : 0
        }
        t |> success(dq == 0 && ds == 0, "{tag}: the hidden rows match the split path byte for byte ({dq} of {rows} quants off, at most {dqmax}; {ds} of {nblk} scales off)")
        delete wqh
        delete wsh
        delete xqh
        delete xsh
        delete gb
        delete ub
        delete rex
        delete regs
        delete regs3
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
def private q8_gemv_gu_run(t; n, hid : int; gelu : bool) {
    let nb = n / 32
    let nblk_out = hid / 32
    let totblk = hid * nb
    let wq_bytes = int64(totblk) * 32l
    let ws_bytes = int64(totblk) * 2l
    var gqh : array<uint>
    var gsh : array<uint>
    var uqh : array<uint>
    var ush : array<uint>
    var xqh : array<uint>
    var xsh : array<float>
    gqh |> resize(totblk * 8)
    uqh |> resize(totblk * 8)
    gsh |> resize(totblk / 2 + 1)
    ush |> resize(totblk / 2 + 1)
    xqh |> resize(nb * 8)
    xsh |> resize(nb)
    for (i in range(totblk * 8)) {
        gqh[i] = hash_word(uint(i) + 71u)
        uqh[i] = hash_word(uint(i) * 3u + 5u)
    }
    for (i in range(totblk / 2 + 1)) {
        gsh[i] = ws_word(uint(i) + 3u)
        ush[i] = ws_word(uint(i) * 7u + 11u)
    }
    for (i in range(nb * 8)) {
        xqh[i] = hash_word(uint(i) * 17u + 9u)
    }
    for (i in range(nb)) {
        xsh[i] = 0.25 + float(i % 5) * 0.125
    }
    let xq_bytes = int64(nb) * 32l
    let xs_bytes = int64(nb) * 4l
    let oq_bytes = int64(hid)
    let os_bytes = int64(nblk_out) * 4l
    let gqd = make_device_buf(wq_bytes)
    let gsd = make_device_buf(ws_bytes)
    let uqd = make_device_buf(wq_bytes)
    let usd = make_device_buf(ws_bytes)
    let xqd = make_device_buf(xq_bytes)
    let xsd = make_device_buf(xs_bytes)
    let oqd = make_device_buf(oq_bytes)
    let osd = make_device_buf(os_bytes)
    var host = make_host_buf(oq_bytes + os_bytes, true, [cached = true])
    var oq : array<int8>
    var os : array<float>
    oq |> resize(hid)
    os |> resize(nblk_out)
    unsafe {
        upload_region_at(gqd, 0l, addr<uint8 const?>(gqh[0]), wq_bytes)
        upload_region_at(gsd, 0l, addr<uint8 const?>(gsh[0]), ws_bytes)
        upload_region_at(uqd, 0l, addr<uint8 const?>(uqh[0]), wq_bytes)
        upload_region_at(usd, 0l, addr<uint8 const?>(ush[0]), ws_bytes)
        upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
        upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
        nan_sentinel_upload(oqd, oq_bytes)
        nan_sentinel_upload(osd, os_bytes)
        var s_cls = set_q8_gemv_gu_cls(fixed_array(gqd, gsd, uqd, usd, xqd, xsd, oqd, osd),
            fixed_array(wq_bytes, ws_bytes, wq_bytes, ws_bytes, xq_bytes, xs_bytes, oq_bytes, os_bytes),
            fixed_array(1u, 2u, 4u, 8u, 16u, 32u, 64u, 64u))
        var raw2 = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw2, begin), null)
        var h2 : VkHaz
        var pc = GuArgs(n = uint(n), d = uint(hid), gblk = 0u, ublk = 0u, act = gelu ? ACT_GELU : ACT_SILU)
        enc_q8_gemv_gu_cls(raw2, h2, s_cls, pc, int64(nblk_out))
        vhz_dep(raw2, h2, 64u, 0u, true)
        cmd_copy_whole(raw2, oqd, host.buf, oq_bytes)
        cmd_copy_range(raw2, osd, 0l, host.buf, oq_bytes, os_bytes)
        vk_check(vkEndCommandBuffer(raw2), null)
        submit_wait(raw2)
        memcpy(addr<void?>(oq[0]), host.mapped, oq_bytes)
        memcpy(addr<void?>(os[0]), reinterpret<void?>(intptr(host.mapped) + uint64(oq_bytes)), os_bytes)
    }
    // the oracle: the q8 GEMM oracle over one record with the x row as its one column, per plane, then the activation
    let one_row <- [0u, 0u, 1u, 0u]
    var g_ref : array<float>
    var u_ref : array<float>
    g_ref |> resize(hid)
    u_ref |> resize(hid)
    q8_gemm_oracle(gqh, gsh, xqh, xsh, one_row, 1, n, hid, g_ref)
    q8_gemm_oracle(uqh, ush, xqh, xsh, one_row, 1, n, hid, u_ref)
    var act : array<float>
    act |> resize(hid)
    for (row in range(hid)) {
        act[row] = act_mul_ref(g_ref[row], u_ref[row], gelu)
    }
    var bad = 0
    var bads = 0
    for (blk in range(nblk_out)) {
        var m = 0.0
        for (e in range(32)) {
            m = max(m, abs(act[blk * 32 + e]))
        }
        let d_ref = m / 127.0
        if (abs(os[blk] - d_ref) > 2e-3 * max(d_ref, 1e-6)) {
            bads++
        }
        for (e in range(32)) {
            let deq = float(oq[blk * 32 + e]) * os[blk]
            if (abs(deq - act[blk * 32 + e]) > d_ref + 2e-3 * abs(act[blk * 32 + e])) {
                bad++
            }
        }
    }
    let tag = "{gelu ? "gelu" : "silu"} n {n} hid {hid}"
    t |> success(bads == 0, "q8 gu {tag}: the block scales match the oracle ({bads} of {nblk_out} off)")
    t |> success(bad == 0, "q8 gu {tag}: the dequantized hidden row sits within a quant step of the oracle ({bad} of {hid} off)")
    var poison := act   // the control: one oracle value pushed three quant steps past its block's must red
    poison[0] += 3.0 * os[0] + 1.0
    var pbad = 0
    for (e in range(32)) {
        let deq = float(oq[e]) * os[0]
        if (abs(deq - poison[e]) > os[0] + 2e-3 * abs(poison[e])) {
            pbad++
        }
    }
    t |> success(pbad > 0, "q8 gu {tag}: the bar reds a poisoned oracle element")
    delete poison
    delete act
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
    t0 |> run("fused q8 gate + up GEMV + act + Q8_0 requant == the CPU oracle (gelu and silu; 36 and 16 blocks deep)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_gemv_gu_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            q8_gemv_gu_run(t, 1152, 256, true)   // gemma-3-1b's dim: 36 blocks, a partial third lane pass; eight requant blocks
            q8_gemv_gu_run(t, 512, 96, false)    // 16 blocks: exactly one lane pass; three blocks
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
    let wq_bytes = int64(totblk) * 32l
    let ws_bytes = int64(totblk) * 2l
    var wqh : array<uint>
    var wsh : array<uint>
    var gh : array<float>
    var sh : array<float>
    wqh |> resize(totblk * 8)
    wsh |> resize(totblk / 2 + 1)
    gh |> resize(n)
    sh |> resize(3 * n)   // three layers' side rows: the cell reads the middle one
    for (i in range(totblk * 8)) {
        wqh[i] = hash_word(uint(i) * 5u + 19u)
    }
    for (i in range(totblk / 2 + 1)) {
        wsh[i] = ws_word(uint(i) * 3u + 7u)
    }
    for (i in range(n)) {
        gh[i] = float((i * 37) % 101) * 0.06 - 3.0
    }
    for (i in range(3 * n)) {
        sh[i] = float((i * 29) % 83) * 0.05 - 2.0
    }
    let g_bytes = int64(n) * 4l
    let s_bytes = int64(3 * n) * 4l
    let y_bytes = int64(d) * 4l
    let wqd = make_device_buf(wq_bytes)
    let wsd = make_device_buf(ws_bytes)
    let gd = make_device_buf(g_bytes)
    let sd = make_device_buf(s_bytes)
    let yd = make_device_buf(y_bytes)
    var host = make_host_buf(y_bytes, true, [cached = true])
    var y_cls : array<float>
    y_cls |> resize(d)
    unsafe {
        upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
        upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
        upload_region_at(gd, 0l, addr<uint8 const?>(gh[0]), g_bytes)
        upload_region_at(sd, 0l, addr<uint8 const?>(sh[0]), s_bytes)
        nan_sentinel_upload(yd, y_bytes)
        var s_cls = set_q8_gemv_pleact_cls(fixed_array(wqd, wsd, gd, sd, yd),
            fixed_array(wq_bytes, ws_bytes, g_bytes, s_bytes, y_bytes),
            fixed_array(1u, 2u, 4u, 8u, 16u))
        var raw2 = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw2, begin), null)
        var h2 : VkHaz
        var pc = PleActArgs(n = uint(n), d = uint(d), wblk = 0u, ubase = uint(n), act = ACT_GELU)
        let rpw = 4l * g_gpu.rows_per_wg
        enc_q8_gemv_pleact_cls(raw2, h2, s_cls, pc, (int64(d) + rpw - 1l) / rpw)
        vhz_dep(raw2, h2, 16u, 0u, true)
        cmd_copy_whole(raw2, yd, host.buf, y_bytes)
        vk_check(vkEndCommandBuffer(raw2), null)
        submit_wait(raw2)
        memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
    }
    // the oracle: the activated row, its Q8_0 blocks, then each row's dot in the scalar block order
    var xq : array<int>
    var xs : array<float>
    xq |> resize(n)
    xs |> resize(nb)
    for (b in range(nb)) {
        var m = 0.0
        for (e in range(32)) {
            m = max(m, abs(act_mul_ref(gh[b * 32 + e], sh[n + b * 32 + e], true)))
        }
        let dq = m / 127.0
        xs[b] = dq
        let id = dq != 0.0 ? 1.0 / dq : 0.0
        for (e in range(32)) {
            xq[b * 32 + e] = int(round(act_mul_ref(gh[b * 32 + e], sh[n + b * 32 + e], true) * id))
        }
    }
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
        if (abs(y_cls[row] - acc) > bar) {
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
def private q8_gemv_ar_run(t; n, d : int; pre_on, raw : bool; ascale : float; abias : bool = false) {   // nolint:STYLE037 — one linear fixture: the GEMV, the residual step and its quant, each with its bar
    let nb = n / 32
    let totblk = d * nb
    let nblk_out = d / 32
    let wq_bytes = int64(totblk) * 32l
    let ws_bytes = int64(totblk) * 2l
    var wqh : array<uint>
    var wsh : array<uint>
    var xqh : array<uint>
    var xsh : array<float>
    var xres : array<float>
    var wnh : array<float>
    wqh |> resize(totblk * 8)
    wsh |> resize(totblk / 2 + 1)
    xqh |> resize(nb * 8)
    xsh |> resize(nb)
    xres |> resize(d)
    wnh |> resize(3 * d)   // two norm rows - the post norm at 0, the next norm at d - then the bias row at 2d
    for (i in range(totblk * 8)) {
        wqh[i] = hash_word(uint(i) * 3u + 29u)
    }
    for (i in range(totblk / 2 + 1)) {
        wsh[i] = ws_word(uint(i) * 5u + 1u)
    }
    for (i in range(nb * 8)) {
        xqh[i] = hash_word(uint(i) * 13u + 7u)
    }
    for (i in range(nb)) {
        xsh[i] = 0.2 + float(i % 7) * 0.1
    }
    for (i in range(d)) {
        xres[i] = float((i * 43) % 97) * 0.4 - 19.0
    }
    for (i in range(2 * d)) {
        wnh[i] = 0.8 + float(i % 9) * 0.05
    }
    for (i in range(d)) {
        wnh[2 * d + i] = float((i * 31) % 67) * 0.6 - 19.8   //! a bias of the residual row's own magnitude, so a dropped add is loud
    }
    let xq_bytes = int64(nb) * 32l
    let xs_bytes = int64(nb) * 4l
    let r_bytes = int64(d) * 4l
    let wn_bytes = int64(3 * d) * 4l
    let oq_bytes = int64(d)
    let os_bytes = int64(nblk_out) * 4l
    let wqd = make_device_buf(wq_bytes)
    let wsd = make_device_buf(ws_bytes)
    let xqd = make_device_buf(xq_bytes)
    let xsd = make_device_buf(xs_bytes)
    let yd = make_device_buf(r_bytes)
    let xrd = make_device_buf(r_bytes)
    let wnd = make_device_buf(wn_bytes)
    let oqd = make_device_buf(oq_bytes)
    let osd = make_device_buf(os_bytes)
    let cntd = make_device_buf(16l)
    var zeros : array<uint>
    zeros |> resize(4)
    var host = make_host_buf(r_bytes + oq_bytes + os_bytes + 16l, true, [cached = true])
    var xr_cls : array<float>
    var oq : array<int8>
    var os : array<float>
    var cnt_after : array<uint>
    xr_cls |> resize(d)
    oq |> resize(d)
    os |> resize(nblk_out)
    cnt_after |> resize(4)
    let eps = 1.0e-6
    unsafe {
        upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
        upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
        upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
        upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
        upload_region_at(xrd, 0l, addr<uint8 const?>(xres[0]), r_bytes)
        upload_region_at(wnd, 0l, addr<uint8 const?>(wnh[0]), wn_bytes)
        upload_region_at(cntd, 0l, addr<uint8 const?>(zeros[0]), 16l)
        nan_sentinel_upload(yd, r_bytes)
        nan_sentinel_upload(oqd, oq_bytes)
        nan_sentinel_upload(osd, os_bytes)
        var s_cls = set_q8_gemv_ar_cls(fixed_array(wqd, wsd, xqd, xsd, yd, xrd, wnd, oqd, osd, cntd),
            fixed_array(wq_bytes, ws_bytes, xq_bytes, xs_bytes, r_bytes, r_bytes, wn_bytes, oq_bytes, os_bytes, 16l),
            fixed_array(1u, 2u, 4u, 8u, 16u, 32u, 0u, 64u, 64u, 16u))
        var raw2 = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw2, begin), null)
        var h2 : VkHaz
        var pc = GemvArArgs(n = uint(n), d = uint(d), wblk = 0u, xblk = 0u, add_on = 1u, woff = uint(d), pwoff = 0u,
            pre_on = pre_on ? 1u : 0u, no_norm = raw ? 1u : 0u, eps = eps, ascale = ascale, abias_on = abias ? 1u : 0u, aboff = uint(2 * d))
        let rpw = 4l * g_gpu.rows_per_wg
        enc_q8_gemv_ar_cls(raw2, h2, s_cls, pc, (int64(d) + rpw - 1l) / rpw)
        vhz_dep(raw2, h2, 16u | 32u | 64u, 0u, true)
        cmd_copy_whole(raw2, xrd, host.buf, r_bytes)
        cmd_copy_range(raw2, oqd, 0l, host.buf, r_bytes, oq_bytes)
        cmd_copy_range(raw2, osd, 0l, host.buf, r_bytes + oq_bytes, os_bytes)
        cmd_copy_range(raw2, cntd, 0l, host.buf, r_bytes + oq_bytes + os_bytes, 16l)
        vk_check(vkEndCommandBuffer(raw2), null)
        submit_wait(raw2)
        memcpy(addr<void?>(xr_cls[0]), host.mapped, r_bytes)
        memcpy(addr<void?>(oq[0]), reinterpret<void?>(intptr(host.mapped) + uint64(r_bytes)), oq_bytes)
        memcpy(addr<void?>(os[0]), reinterpret<void?>(intptr(host.mapped) + uint64(r_bytes + oq_bytes)), os_bytes)
        memcpy(addr<void?>(cnt_after[0]), reinterpret<void?>(intptr(host.mapped) + uint64(r_bytes + oq_bytes + os_bytes)), 16l)
    }
    // the oracle: the q8 GEMM oracle over one record with the x row as its one column, then the residual step's math
    let one_row <- [0u, 0u, 1u, 0u]
    var y : array<float>
    y |> resize(d)
    q8_gemm_oracle(wqh, wsh, xqh, xsh, one_row, 1, n, d, y)
    if (abias) {
        for (k in range(d)) {
            y[k] += wnh[2 * d + k]
        }
    }
    var ainv = 0.0
    if (pre_on) {
        var sa = 0.0
        for (k in range(d)) {
            sa += y[k] * y[k]
        }
        ainv = 1.0 / sqrt(sa / float(d) + eps)
    }
    var xr_ref : array<float>
    xr_ref |> resize(d)
    var aref : array<float>   //! the add partner the step sees: the normed GEMV row under pre_on, the raw row otherwise
    aref |> resize(d)
    var ss = 0.0
    for (k in range(d)) {
        let a = pre_on ? wnh[k] * (y[k] * ainv) : y[k]
        aref[k] = a
        xr_ref[k] = (xres[k] + a) * ascale
        ss += xr_ref[k] * xr_ref[k]
    }
    let inv = raw ? 1.0 : 1.0 / sqrt(ss / float(d) + eps)
    var normed : array<float>
    normed |> resize(d)
    for (k in range(d)) {
        normed[k] = (raw ? 1.0 : wnh[d + k]) * (xr_ref[k] * inv)
    }
    //! the residual bar: the step's own rounding on the row, plus the GEMV's on the partner it adds - a 512-deep dot whose
    //! terms cancel lands a few 1e-4 of its magnitude off the oracle's order (two rows of 2816 read 3e-4 at gemma-4-26B's width)
    var badx = 0
    var pbadx = 0
    for (k in range(d)) {
        let bar = 1e-4 * abs(xr_ref[k]) + 1e-4 + 4e-4 * abs(aref[k]) * ascale
        if (abs(xr_cls[k] - xr_ref[k]) > bar) {
            badx++
        }
        if (abs(xr_cls[k] - (xr_ref[k] + 0.1 * abs(xr_ref[k]) + 0.1)) > bar) {   //! the control: an expectation pushed a tenth off must red
            pbadx++
        }
    }
    var bads = 0
    var badq = 0
    for (blk in range(nblk_out)) {
        var m = 0.0
        for (e in range(32)) {
            m = max(m, abs(normed[blk * 32 + e]))
        }
        let d_ref = m / 127.0
        if (abs(os[blk] - d_ref) > 2e-3 * max(d_ref, 1e-6)) {
            bads++
        }
        for (e in range(32)) {
            let deq = float(oq[blk * 32 + e]) * os[blk]
            if (abs(deq - normed[blk * 32 + e]) > d_ref + 2e-3 * abs(normed[blk * 32 + e])) {
                badq++
            }
        }
    }
    let tag = "n {n} d {d} pre {pre_on ? 1 : 0} raw {raw ? 1 : 0} bias {abias ? 1 : 0}"
    t |> success(badx == 0, "q8 gemv+ar {tag}: the residual row matches the oracle ({badx} of {d} off)")
    t |> success(pbadx == d, "q8 gemv+ar {tag}: the residual bar reds an oracle row pushed a tenth off ({pbadx} of {d} red)")
    t |> success(bads == 0, "q8 gemv+ar {tag}: the block scales match ({bads} of {nblk_out} off)")
    t |> success(badq == 0, "q8 gemv+ar {tag}: the dequantized row sits within a quant step ({badq} of {d} off)")
    t |> success(cnt_after[0] == 0u, "q8 gemv+ar {tag}: the arrival counter reads zero after the dispatch ({cnt_after[0]})")
    var poison := normed   // the control: one expected value pushed three quant steps past its block must red
    poison[0] += 3.0 * os[0] + 1.0
    var pbad = 0
    for (e in range(32)) {
        let deq = float(oq[e]) * os[0]
        if (abs(deq - poison[e]) > os[0] + 2e-3 * abs(poison[e])) {
            pbad++
        }
    }
    t |> success(pbad > 0, "q8 gemv+ar {tag}: the bar reds a poisoned oracle element")
    delete wqh
    delete wsh
    delete xqh
    delete xsh
    delete xres
    delete wnh
    delete zeros
    delete xr_cls
    delete oq
    delete os
    delete cnt_after
    delete y
    delete xr_ref
    delete normed
    delete poison
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

//! the one-row fused gate-up rows against the CPU chain, a column at a time: the q8 GEMM oracle with that column's row,
//! the activation, then the Q8_0 block-scale and dequant bars of the sibling one-row cell; `bytes` holds the rows at
//! `col_bytes` apart, the quants first and the scales after them
def private q8_gemv_gu_oracle_cols(t; tag : string; bytes : uint8?; col_bytes : int64; gqh, gsh, uqh, ush, xqh : array<uint>; xsh : array<float>; n, hid, ncols : int; act : uint) {
    let nb = n / 32
    let nblk_out = hid / 32
    let oq_bytes = int64(hid)
    let one_rec <- [0u, 0u, 1u, 0u]
    var obad = 0
    var obads = 0
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
        var g_ref : array<float>
        var u_ref : array<float>
        g_ref |> resize(hid)
        u_ref |> resize(hid)
        q8_gemm_oracle(gqh, gsh, xq_c, xs_c, one_rec, 1, n, hid, g_ref)
        q8_gemm_oracle(uqh, ush, xq_c, xs_c, one_rec, 1, n, hid, u_ref)
        let one = col_bytes * int64(c)
        unsafe {
            var os_c = reinterpret<float?>(bytes + one + oq_bytes)
            var oq_c = reinterpret<int8?>(bytes + one)
            for (blk in range(nblk_out)) {
                var m = 0.0
                for (e in range(32)) {
                    m = max(m, abs(act_mul_ref(g_ref[blk * 32 + e], u_ref[blk * 32 + e], act)))
                }
                let d_ref = m / 127.0
                if (abs(os_c[blk] - d_ref) > 2e-3 * max(d_ref, 1e-6)) {
                    obads++
                }
                for (e in range(32)) {
                    let a_ref = act_mul_ref(g_ref[blk * 32 + e], u_ref[blk * 32 + e], act)
                    let deq = float(oq_c[blk * 32 + e]) * os_c[blk]
                    if (abs(deq - a_ref) > d_ref + 2e-3 * abs(a_ref)) {
                        obad++
                    }
                }
            }
        }
        delete g_ref
        delete u_ref
        delete xq_c
        delete xs_c
    }
    t |> success(obads == 0, "one-row fused gate-up {tag}: every column's block scales match the CPU oracle ({obads} of {nblk_out * ncols} off)")
    t |> success(obad == 0, "one-row fused gate-up {tag}: every column's dequantized hidden row sits within a quant step of the CPU oracle ({obad} of {hid * ncols} off)")
}

//! the fused gate-up GEMV's N-column form over `ncols` rows against the one-row kernel run a column at a time, bit for
//! bit, the one-row rows against the CPU chain, the slots past the live columns untouched
def private q8_gemv_gu_n_run(t; n, hid, ncols : int; act : uint) {
    let nb = n / 32
    let nblk_out = hid / 32
    let totblk = hid * nb
    let wq_bytes = int64(totblk) * 32l
    let ws_bytes = int64(totblk) * 2l
    let xq_bytes = int64(nb) * 32l
    let xs_bytes = int64(nb) * 4l
    let oq_bytes = int64(hid)
    let os_bytes = int64(nblk_out) * 4l
    let ncol_max = 8
    var gqh : array<uint>
    var gsh : array<uint>
    var uqh : array<uint>
    var ush : array<uint>
    var xqh : array<uint>
    var xsh : array<float>
    gqh |> resize(totblk * 8)
    uqh |> resize(totblk * 8)
    gsh |> resize(totblk / 2 + 1)
    ush |> resize(totblk / 2 + 1)
    xqh |> resize(nb * 8 * ncols)
    xsh |> resize(nb * ncols)
    for (i in range(totblk * 8)) {
        gqh[i] = hash_word(uint(i) + 71u)
        uqh[i] = hash_word(uint(i) * 3u + 5u)
    }
    for (i in range(totblk / 2 + 1)) {
        gsh[i] = ws_word(uint(i) + 3u)
        ush[i] = ws_word(uint(i) * 7u + 11u)
    }
    for (i in range(nb * 8 * ncols)) {
        xqh[i] = hash_word(uint(i) * 17u + 9u)
    }
    for (i in range(nb * ncols)) {
        xsh[i] = 0.25 + float(i % 5) * 0.125
    }
    let gqd = make_device_buf(wq_bytes)
    let gsd = make_device_buf(ws_bytes)
    let uqd = make_device_buf(wq_bytes)
    let usd = make_device_buf(ws_bytes)
    let col_bytes = oq_bytes + os_bytes
    let nbase = col_bytes * int64(ncols)   // the one-row runs' rows, then the N-row run's eight column slots
    var host = make_host_buf(nbase + col_bytes * int64(ncol_max), true, [cached = true])
    let tag = "n {n} hid {hid} act {act} x {ncols}"
    unsafe {
        upload_region_at(gqd, 0l, addr<uint8 const?>(gqh[0]), wq_bytes)
        upload_region_at(gsd, 0l, addr<uint8 const?>(gsh[0]), ws_bytes)
        upload_region_at(uqd, 0l, addr<uint8 const?>(uqh[0]), wq_bytes)
        upload_region_at(usd, 0l, addr<uint8 const?>(ush[0]), ws_bytes)
        for (c in range(ncols)) {
            let xqd = make_device_buf(xq_bytes)
            let xsd = make_device_buf(xs_bytes)
            let oqd = make_device_buf(oq_bytes)
            let osd = make_device_buf(os_bytes)
            upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[c * nb * 8]), xq_bytes)
            upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[c * nb]), xs_bytes)
            nan_sentinel_upload(oqd, oq_bytes)
            nan_sentinel_upload(osd, os_bytes)
            var s_one = set_q8_gemv_gu_cls(fixed_array(gqd, gsd, uqd, usd, xqd, xsd, oqd, osd),
                fixed_array(wq_bytes, ws_bytes, wq_bytes, ws_bytes, xq_bytes, xs_bytes, oq_bytes, os_bytes),
                fixed_array(1u, 2u, 4u, 8u, 16u, 32u, 64u, 64u))
            var raw1 = alloc_cmd()
            let begin1 = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw1, begin1), null)
            var h1 : VkHaz
            var pc1 = GuArgs(n = uint(n), d = uint(hid), gblk = 0u, ublk = 0u, act = act)
            enc_q8_gemv_gu_cls(raw1, h1, s_one, pc1, int64(nblk_out))
            vhz_dep(raw1, h1, 64u, 0u, true)
            let cb = col_bytes * int64(c)
            cmd_copy_range(raw1, oqd, 0l, host.buf, cb, oq_bytes)
            cmd_copy_range(raw1, osd, 0l, host.buf, cb + oq_bytes, os_bytes)
            vk_check(vkEndCommandBuffer(raw1), null)
            submit_wait(raw1)
        }
        let xqd_n = make_device_buf(xq_bytes * int64(ncols))
        let xsd_n = make_device_buf(xs_bytes * int64(ncols))
        let oqd_n = make_device_buf(oq_bytes * int64(ncol_max))
        let osd_n = make_device_buf(os_bytes * int64(ncol_max))
        upload_region_at(xqd_n, 0l, addr<uint8 const?>(xqh[0]), xq_bytes * int64(ncols))
        upload_region_at(xsd_n, 0l, addr<uint8 const?>(xsh[0]), xs_bytes * int64(ncols))
        nan_sentinel_upload(oqd_n, oq_bytes * int64(ncol_max))
        nan_sentinel_upload(osd_n, os_bytes * int64(ncol_max))
        var s_n = q8_gemv_gu_n_set(fixed_array(gqd, gsd, uqd, usd, xqd_n, xsd_n, oqd_n, osd_n),
            fixed_array(wq_bytes, ws_bytes, wq_bytes, ws_bytes, xq_bytes * int64(ncols), xs_bytes * int64(ncols), oq_bytes * int64(ncol_max), os_bytes * int64(ncol_max)),
            fixed_array(1u, 2u, 4u, 8u, 16u, 32u, 64u, 64u))
        var raw2 = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw2, begin), null)
        var h2 : VkHaz
        var pc = GuNArgs(n = uint(n), d = uint(hid), gblk = 0u, ublk = 0u, act = act, ncols = uint(ncols))
        q8_gemv_gu_n_enc(raw2, h2, s_n, pc, int64(nblk_out))
        vhz_dep(raw2, h2, 64u, 0u, true)
        let nsbase = nbase + oq_bytes * int64(ncol_max)   // the N-row run's scale slots past its quant slots
        cmd_copy_range(raw2, oqd_n, 0l, host.buf, nbase, oq_bytes * int64(ncol_max))
        cmd_copy_range(raw2, osd_n, 0l, host.buf, nsbase, os_bytes * int64(ncol_max))
        vk_check(vkEndCommandBuffer(raw2), null)
        submit_wait(raw2)
        var bytes = reinterpret<uint8?>(host.mapped)
        var bad_q = 0
        var bad_s = 0
        for (c in range(ncols)) {
            let one = col_bytes * int64(c)
            for (i in range(hid)) {
                if (bytes[one + int64(i)] != bytes[nbase + int64(c * hid + i)]) {
                    bad_q++
                }
            }
            for (i in range(nblk_out)) {
                for (k in range(4)) {
                    if (bytes[one + oq_bytes + int64(i) * 4l + int64(k)] != bytes[nsbase + int64(c * nblk_out + i) * 4l + int64(k)]) {
                        bad_s++
                        break
                    }
                }
            }
        }
        t |> success(bad_q == 0, "N-row fused gate-up {tag}: the quant bytes over {ncols} columns are the one-row kernel's bit for bit ({bad_q} off)")
        t |> success(bad_s == 0, "N-row fused gate-up {tag}: the block scales over {ncols} columns are the one-row kernel's ({bad_s} off)")
        var stale = 0
        for (j in range64(int64(ncols) * oq_bytes, int64(ncol_max) * oq_bytes)) {   //! the slots past the live columns keep the sentinel, byte by byte
            if (uint(bytes[nbase + j]) != ((NAN_SENTINEL_BITS >> (8u * uint(j % 4l))) & 0xffu)) {
                stale++
            }
        }
        for (j in range64(int64(ncols) * os_bytes, int64(ncol_max) * os_bytes)) {
            if (uint(bytes[nsbase + j]) != ((NAN_SENTINEL_BITS >> (8u * uint(j % 4l))) & 0xffu)) {
                stale++
            }
        }
        t |> success(stale == 0, "N-row fused gate-up {tag}: the slots past the live columns keep the sentinel ({stale} bytes written)")
        let poison_at = nbase + int64((ncols - 1) * hid + hid / 2)   //! the control: one quant byte of the last live column flipped reds exactly one
        let kept = bytes[poison_at]
        bytes[poison_at] = uint8(uint(kept) ^ 0x55u)
        var pbad = 0
        for (c in range(ncols)) {
            let one = col_bytes * int64(c)
            for (i in range(hid)) {
                if (bytes[one + int64(i)] != bytes[nbase + int64(c * hid + i)]) {
                    pbad++
                }
            }
        }
        t |> equal(pbad, 1, "N-row fused gate-up {tag}: the bit-exact bar reds one poisoned quant byte")
        bytes[poison_at] = kept
        q8_gemv_gu_oracle_cols(t, tag, bytes, col_bytes, gqh, gsh, uqh, ush, xqh, xsh, n, hid, ncols, act)
    }
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
            if (!(ensure_q8_gemv_gu_cls() && q8_gemv_gu_n_ensure(8l))) {
                t |> skip("no Vulkan device")
                return
            }
            q8_gemv_gu_n_run(t, 512, 1152, 4, ACT_SILU)   //! the four-column stamp full
            q8_gemv_gu_n_run(t, 256, 512, 3, ACT_GELU)   //! an odd count on the four-column stamp: its last lane re-dots column 2, a subgroup idles in the quantize
            q8_gemv_gu_n_run(t, 512, 1024, 8, ACT_SILU)   //! the eight-column stamp full: every subgroup a column at 32 lanes, two rounds at 64
            q8_gemv_gu_n_run(t, 256, 512, 5, ACT_SILU)   //! the eight-column stamp at five: three lanes re-dot column 4
            q8_gemv_gu_n_run(t, 256, 512, 2, ACT_GELU)   //! the two-column stamp full
            q8_gemv_gu_n_run(t, 256, 512, 1, ACT_SILU)   //! one column on the two-column stamp: the one-row command's shape through the N form
        } else {
            t |> skip("dasVulkan not present")
        }
    }
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
                let wq_bytes = int64(totblk) * 32l
                let ws_bytes = int64(totblk) * 2l
                let wq_words = int((wq_bytes + 3l) / 4l)
                var wqh : array<uint>
                var wsh : array<uint>
                var xqh : array<uint>
                var xsh : array<float>
                wqh |> resize(wq_words)
                wsh |> resize(totblk / 2 + 1)
                xqh |> resize(nb * 8)
                xsh |> resize(nb)
                for (i in range(wq_words)) {
                    wqh[i] = hash_word(uint(i) + 55u)
                }
                for (i in range(totblk / 2 + 1)) {
                    wsh[i] = ws_word(uint(i))
                }
                for (i in range(nb * 8)) {
                    xqh[i] = hash_word(uint(i) * 17u + 9u)
                }
                for (i in range(nb)) {
                    xsh[i] = 0.25 + float(i % 5) * 0.125
                }
                let xq_bytes = int64(nb) * 32l
                let xs_bytes = int64(nb) * 4l
                let y_bytes = int64(rows) * 4l
                let wqd = make_device_buf(wq_bytes)
                let wsd = make_device_buf(ws_bytes)
                let xqd = make_device_buf(xq_bytes)
                let xsd = make_device_buf(xs_bytes)
                let yd2 = make_device_buf(y_bytes)
                let rgd = make_device_buf(int64(nreg) * 8l)
                var host = make_host_buf(y_bytes, true, [cached = true])
                var y_cls : array<float>
                y_cls |> resize(rows)
                let wgs = (int64(rows) + g_gpu.rows_per_wg - 1l) / g_gpu.rows_per_wg
                var regs : array<uint>
                regs |> resize(nreg * 2)
                for (r in range(nreg)) {
                    regs[r * 2] = uint(r * d * nb)
                    regs[r * 2 + 1] = 0u
                }
                unsafe {
                    upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                    upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
                    upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
                    upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
                    upload_region_at(rgd, 0l, addr<uint8 const?>(regs[0]), int64(nreg) * 8l)
                    var s_cls = set_q8_gemv_cls(fixed_array(wqd, wsd, rgd, xqd, xsd, yd2),
                        fixed_array(wq_bytes, ws_bytes, int64(nreg) * 8l, xq_bytes, xs_bytes, y_bytes),
                        fixed_array(1u, 2u, 4u, 8u, 16u, 32u))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = GemvArgs(n = 512u, d = uint(d), nreg = uint(nreg), ybase = 0u)
                    enc_q8_gemv_cls(raw2, h2, s_cls, pc, wgs)
                    vhz_dep(raw2, h2, 32u, 0u, true)
                    cmd_copy_whole(raw2, yd2, host.buf, y_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, y_bytes)
                }
                // oracle: per-block int dot x per-block scales, sequential block order
                var y_ref : array<float>
                y_ref |> resize(rows)
                for (r in range(nreg)) {
                    for (row in range(d)) {
                        let w0 = r * d * nb + row * nb
                        var acc = 0.0
                        for (b in range(nb)) {
                            var idot = 0
                            for (j in range(8)) {
                                idot += sdot4(wqh[(w0 + b) * 8 + j], xqh[b * 8 + j])
                            }
                            let ws16 = ((w0 + b) % 2 == 0 ? unpackHalf2x16(wsh[(w0 + b) / 2]).x
                                : unpackHalf2x16(wsh[(w0 + b) / 2]).y)
                            acc += float(idot) * ws16 * xsh[b]
                        }
                        y_ref[r * d + row] = acc
                    }
                }
                let bad = mismatch_approx(y_cls, y_ref)
                t |> success(bad == 0, "q8 GEMV matches the CPU oracle ({bad} of {rows} rows off)")
                delete regs
                delete y_ref
                delete wqh
                delete wsh
                delete xqh
                delete xsh
                delete y_cls
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
        let wq_words = q8 ? d * xblk * 8 : d * xblk * int(kq_qsb(f) / 4l)
        let ws_words = q8 ? (d * xblk + 1) / 2 : d * xblk * int(kq_dev_ssb(f) / 4l)
        let xq_col = q8 ? xblk * 8 : xblk * 64
        let ncol_max = 8
        var wqh : array<uint>
        var wsh : array<uint>
        var xqh : array<uint>
        var xsh : array<float>
        wqh |> resize(wq_words)
        wsh |> resize(ws_words)
        xqh |> resize(xq_col * ncol_max)
        xsh |> resize(xblk * ncol_max)
        for (i in range(wq_words)) {
            wqh[i] = hash_word(uint(i) + uint(fmt) * 77777u)
        }
        for (i in range(ws_words)) {
            wsh[i] = ws_word(uint(i) + uint(fmt) * 13u)
        }
        for (i in range(xq_col * ncol_max)) {
            xqh[i] = hash_word(uint(i) * 31u + 5u)
        }
        for (i in range(xblk * ncol_max)) {
            xsh[i] = 0.5 + float(i % 7) * 0.25
        }
        let wq_bytes = int64(wq_words) * 4l
        let ws_bytes = int64(ws_words) * 4l
        let xq_bytes = int64(xq_col * ncol_max) * 4l
        let xs_bytes = int64(xblk * ncol_max) * 4l
        let y_bytes = int64(d * ncol_max) * 4l
        let wqd = make_device_buf(wq_bytes)
        let wsd = make_device_buf(ws_bytes)
        let xqd = make_device_buf(xq_bytes)
        let xsd = make_device_buf(xs_bytes)
        let yd = make_device_buf(y_bytes)
        var host = make_host_buf(y_bytes, true, [cached = true])
        var y_one : array<float>
        var y_n : array<float>
        y_one |> resize(d * ncol_max)
        y_n |> resize(d * ncol_max)
        let gbits = fixed_array(1u, 2u, 4u, 8u, 16u, 32u)
        unsafe {
            upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
            upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), ws_bytes)
            upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), xq_bytes)
            upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), xs_bytes)
            nan_sentinel_upload(yd, y_bytes)
            var raw = alloc_cmd()
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
            var h : VkHaz
            for (c in range(ncol_max)) {
                var pair <- [0u, uint(c * xblk)]
                let rgd = make_device_buf(8l)
                upload_region_at(rgd, 0l, addr<uint8 const?>(pair[0]), 8l)
                var s1 = gemv_cls_set(fmt, fixed_array(wqd, wsd, rgd, xqd, xsd, yd), fixed_array(wq_bytes, ws_bytes, 8l, xq_bytes, xs_bytes, y_bytes), gbits)
                var pc1 = GemvArgs(n = uint(n), d = uint(d), nreg = 1u, ybase = uint(c * d))
                gemv_enc(fmt, raw, h, s1, pc1, int64(d))
                delete pair
            }
            vhz_dep(raw, h, 32u, 0u, true)
            cmd_copy_whole(raw, yd, host.buf, y_bytes)
            vk_check(vkEndCommandBuffer(raw), null)
            submit_wait(raw)
            memcpy(addr<void?>(y_one[0]), host.mapped, y_bytes)
        }
        var y_ref <- gemv_ncol_oracle(f, wqh, wsh, xqh, xsh, n, d, ncol_max)
        let bad_o = mismatch_approx(y_one, y_ref)
        t |> success(bad_o == 0, "{name}: the one-column class over every activation row matches the CPU oracle ({bad_o} of {d * ncol_max} off)")
        y_ref[d + 2] += 1.0 + abs(y_ref[d + 2])   //! the poison: one past the element's own magnitude (an int8 dot lands in the hundreds of thousands)
        t |> success(mismatch_approx(y_one, y_ref) == 1, "{name}: the oracle bar reds one poisoned element")
        let n2_min_prior = g_q8_n2_min_n
        for (paired in fixed_array(false, true)) {
            continue if (paired && !q8)
            g_q8_n2_min_n = paired ? 0u : 0xFFFFFFFFu
            let form = paired ? "{name} row-pair" : name
            t |> success(gemv_cls_ensure_n(fmt), "{form}: the N-column class rail engages")
            var pair <- [0u, 0u]
            let rgd = make_device_buf(8l)
            upload_region_at(rgd, 0l, unsafe(addr<uint8 const?>(pair[0])), 8l)
            var s_n = gemv_cls_set_n(fmt, fixed_array(wqd, wsd, rgd, xqd, xsd, yd), fixed_array(wq_bytes, ws_bytes, 8l, xq_bytes, xs_bytes, y_bytes), gbits)
            delete pair
            for (ncols in fixed_array(1, 2, 3, 4, 5, 6, 7, 8)) {   //! every count: the odd ones leave dead columns on the wider stamps
                unsafe {
                    nan_sentinel_upload(yd, y_bytes)
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    var pc = GemvArgs(n = uint(n), d = uint(d), nreg = 1u, ybase = 0u, ncols = uint(ncols), ystride = uint(d))
                    gemv_enc_n(fmt, raw, h, s_n, pc, int64(d))
                    vhz_dep(raw, h, 32u, 0u, true)
                    cmd_copy_whole(raw, yd, host.buf, y_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(y_n[0]), host.mapped, y_bytes)
                }
                let live = ncols * d
                let bad = mismatch_exact(y_n, y_one, live)
                t |> success(bad == 0, "{form} at {ncols} column(s): every column bit-matches the one-column class's row ({bad} of {live} off)")
                var stale = 0
                for (i in range(live, d * ncol_max)) {
                    if (float_bits_to_uint(y_n[i]) != NAN_SENTINEL_BITS) {
                        stale++
                    }
                }
                t |> success(stale == 0, "{form} at {ncols} column(s): the rows past its columns keep the sentinel ({stale} of {d * ncol_max - live} written)")
                y_n[live - 1] += 1.0 + abs(y_n[live - 1])
                t |> success(mismatch_exact(y_n, y_one, live) == 1, "{form} at {ncols} column(s): the bit-exact bar reds one poisoned element")
            }
        }
        g_q8_n2_min_n = n2_min_prior
        delete y_ref
        delete y_one
        delete y_n
        delete wqh
        delete wsh
        delete xqh
        delete xsh
    }
}

[test]
def test_vkd_gemv_ncol(t0 : T?) {
    t0 |> run("the N-column decode GEMV leaves (q8 and its row-pair stamps, k4, k6, k5, q40, iq4xs) == the one-column class, column for column") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_q8_gemv_cls()) {
                t |> skip("no Vulkan device")
                return
            }
            for (f in fixed_array(KqFmt.q8, KqFmt.k4, KqFmt.k6, KqFmt.k5, KqFmt.q40, KqFmt.iq4xs)) {
                t |> success(gemv_cls_ensure(int(f)), "{f}: the one-column class rail engages")
                gemv_ncol_arm(t, f, 1024, 96)   //! n a multiple of 256 for the superblock formats, d even for the row-pair stamps
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
                var srch : array<float>
                srch |> resize(inbase + nblk * 32)
                for (i in range(inbase + nblk * 32)) {
                    srch[i] = float((i * 37) % 113) * 0.04 - 2.2
                }
                let src_bytes = int64(inbase + nblk * 32) * 4l
                let q_bytes = int64(nblk) * 32l
                let s_bytes = int64(nblk) * 4l
                let srcd = make_device_buf(src_bytes)
                let qd2 = make_device_buf(q_bytes)
                let sd2 = make_device_buf(s_bytes)
                var host = make_host_buf(q_bytes + s_bytes, true, [cached = true])
                var q_cls : array<uint>
                var s_cls : array<float>
                q_cls |> resize(nblk * 8)
                s_cls |> resize(nblk)
                let wgs = (nblk * 8 + 255) / 256
                unsafe {
                    upload_region_at(srcd, 0l, addr<uint8 const?>(srch[0]), src_bytes)
                    var sc = set_rq_cls(fixed_array(srcd, qd2, sd2),
                        fixed_array(src_bytes, q_bytes, s_bytes),
                        fixed_array(1u, 4u, 8u))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = RqArgs(inbase = uint(inbase), nblk = uint(nblk))
                    enc_cls_dn_rq(raw2, h2, sc, pc, int64(wgs))
                    vhz_dep(raw2, h2, 4u, 0u, true)
                    cmd_copy_whole(raw2, qd2, host.buf, q_bytes)
                    cmd_copy_range(raw2, sd2, 0l, host.buf, q_bytes, s_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(q_cls[0]), host.mapped, q_bytes)
                    memcpy(addr<void?>(s_cls[0]), reinterpret<void?>(intptr(host.mapped) + uint64(q_bytes)), s_bytes)
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
                verify(ensure_actf16_cls() && ensure_f16cvt_cls(),
                    "the cvt class rails must engage on a live device")
                let nelem = 2048
                var gh : array<float>
                var uh : array<float>
                gh |> resize(nelem)
                uh |> resize(nelem)
                for (i in range(nelem)) {
                    gh[i] = float((i * 29) % 61) * 0.06 - 1.9
                    uh[i] = float((i * 43) % 83) * 0.05 - 2.1
                }
                let g_bytes = int64(nelem) * 4l
                let h_bytes = int64(nelem) * 2l
                let gd = make_device_buf(g_bytes)
                let ud = make_device_buf(g_bytes)
                let hd2 = make_device_buf(h_bytes)
                var host = make_host_buf(h_bytes, true, [cached = true])
                var h_cls : array<uint>
                h_cls |> resize(nelem / 2)
                for (arm in range(1, 4)) {   // 1 = act-f16 (gelu), 2 = f16 convert, 3 = act-f16 under the clamped swiglu
                    unsafe {
                        upload_region_at(gd, 0l, addr<uint8 const?>(gh[0]), g_bytes)
                        upload_region_at(ud, 0l, addr<uint8 const?>(uh[0]), g_bytes)
                        let wgs = (nelem / 4 + 255) / 256
                        var sc : VkDescriptorSet
                        var raw2 = alloc_cmd()
                        let begin = VkCommandBufferBeginInfo()
                        vk_check(vkBeginCommandBuffer(raw2, begin), null)
                        var h2 : VkHaz
                        if (arm != 2) {
                            sc = set_actf16_cls(fixed_array(gd, ud, hd2),
                                fixed_array(g_bytes, g_bytes, h_bytes), fixed_array(1u, 2u, 4u))
                            var pc = ActArgs(nelem = uint(nelem), act = arm == 3 ? ACT_OAI : ACT_GELU, nblk = 0u)
                            enc_actf16_cls(raw2, h2, sc, pc, int64(wgs))
                        } else {
                            sc = set_f16cvt_cls(fixed_array(ud, hd2),
                                fixed_array(g_bytes, h_bytes), fixed_array(1u, 4u))
                            var pc = ActArgs(nelem = uint(nelem), nblk = 0u)
                            enc_f16cvt_cls(raw2, h2, sc, pc, int64(wgs))
                        }
                        vhz_dep(raw2, h2, 4u, 0u, true)
                        cmd_copy_whole(raw2, hd2, host.buf, h_bytes)
                        vk_check(vkEndCommandBuffer(raw2), null)
                        submit_wait(raw2)
                        memcpy(addr<void?>(h_cls[0]), host.mapped, h_bytes)
                    }
                    // oracle in f32, compared through the f16 halves the kernels emit (the gelu
                    // arm carries transcendental ulps — approx absorbs both)
                    var got : array<float>
                    var want : array<float>
                    got |> resize(nelem)
                    want |> resize(nelem)
                    for (i in range(nelem)) {
                        got[i] = half_at(h_cls, i)
                        want[i] = arm == 2 ? uh[i] : act_mul_ref(gh[i], uh[i], arm == 3 ? ACT_OAI : ACT_GELU)
                    }
                    let bad = mismatch_approx(got, want)
                    t |> success(bad == 0, "cvt arm {arm} matches the CPU oracle ({bad} of {nelem} off)")
                    delete got
                    delete want
                }
                delete gh
                delete uh
                delete h_cls
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test]
def test_vkd_rope_b_pair(t0 : T?) {
    t0 |> run("batch rope + fused qkn-rope class kernels == the CPU oracle") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_rope_kv_b_cls() && ensure_qkn_rope_cls())) {
                t |> skip("no Vulkan device")
                return
            }
            let nh = 4
            let nkvh = 2
            let hs = 64
            let half = hs / 2
            let qd = nh * hs
            let kvd = nkvh * hs
            // --- rope_kv_store_b: npos positions, q in place, k roped + v raw into the mirrors ---
            {
                let neox = 1
                let npos = 5
                let pos0 = 7
                let layerbase = 256
                let voff = npos * kvd   // v rows base inside the kv projection buffer
                let ppp = (nh + nkvh) * half
                let kpair0 = nh * half
                let mirn = layerbase + (pos0 + npos) * kvd
                var q0h : array<float>
                var kvh : array<float>
                var csh : array<float>
                q0h |> resize(npos * qd)
                kvh |> resize(voff + npos * kvd)
                csh |> resize(npos * hs)
                for (i in range(npos * qd)) {
                    q0h[i] = float((i * 31) % 67) * 0.05 - 1.6
                }
                for (i in range(voff + npos * kvd)) {
                    kvh[i] = float((i * 23) % 71) * 0.04 - 1.5
                }
                for (i in range(npos * hs)) {
                    csh[i] = float((i * 13) % 41) * 0.045 - 0.9
                }
                let q_bytes = int64(npos * qd) * 4l
                let kv_bytes = int64(voff + npos * kvd) * 4l
                let cs_bytes = int64(npos * hs) * 4l
                let m_bytes = int64(mirn) * 4l
                let qd2 = make_device_buf(q_bytes)
                let kvdv = make_device_buf(kv_bytes)
                let csd = make_device_buf(cs_bytes)
                let km2 = make_device_buf(m_bytes)
                let vm2 = make_device_buf(m_bytes)
                var host = make_host_buf(q_bytes + 2l * m_bytes, true, [cached = true])
                var q_cls : array<float>
                var m_cls : array<float>
                q_cls |> resize(npos * qd)
                m_cls |> resize(2 * mirn)
                let wgs = (npos * ppp + 255) / 256
                let bpad = 96   // the bias rows sit past a pad, so boff is exercised off its default
                var bh : array<float>
                bh |> resize(bpad + qd + 2 * kvd)
                for (i in range(bpad + qd + 2 * kvd)) {
                    bh[i] = i < bpad ? 77.0 : float((i * 19) % 31) * 0.02 - 0.3
                }
                let b_bytes = long_length(bh) * 4l
                let bd = make_device_buf(b_bytes)
                unsafe {
                    upload_region_at(kvdv, 0l, addr<uint8 const?>(kvh[0]), kv_bytes)
                    upload_region_at(csd, 0l, addr<uint8 const?>(csh[0]), cs_bytes)
                    upload_region_at(qd2, 0l, addr<uint8 const?>(q0h[0]), q_bytes)
                    upload_region_at(bd, 0l, addr<uint8 const?>(bh[0]), b_bytes)
                    var sc = set_rope_kv_b_cls(fixed_array(qd2, kvdv, km2, vm2, csd, bd),
                        fixed_array(q_bytes, kv_bytes, m_bytes, m_bytes, cs_bytes, b_bytes),
                        fixed_array(1u, 16u, 2u, 4u, 8u, 0u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    var pc = RopeKvBArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs),
                        half = uint(half), neox = uint(neox), kpair0 = uint(kpair0),
                        ppp = uint(ppp), layerbase = uint(layerbase), npos = uint(npos),
                        pos0 = uint(pos0), voff = uint(voff), kstride = uint(kvd), boff = uint(bpad), hasb = 1u)
                    enc_rope_kv_b_cls(raw, h, sc, pc, int64(wgs))
                    vhz_dep(raw, h, 8u, 0u, true)
                    cmd_copy_whole(raw, qd2, host.buf, q_bytes)
                    cmd_copy_range(raw, km2, 0l, host.buf, q_bytes, m_bytes)
                    cmd_copy_range(raw, vm2, 0l, host.buf, q_bytes + m_bytes, m_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(q_cls[0]), host.mapped, q_bytes)
                    memcpy(addr<void?>(m_cls[0]), reinterpret<void?>(intptr(host.mapped) + uint64(q_bytes)), 2l * m_bytes)
                }
                // oracle: the bias row at boff first, then per-position NEOX pairs off per-position cos
                // rows; k roped + v raw (plus bias) at absolute mirror rows — only the written region compares
                var q_ref : array<float>
                var mk_ref : array<float>
                var mv_ref : array<float>
                q_ref |> resize(npos * qd)
                mk_ref |> resize(mirn)
                mv_ref |> resize(mirn)
                for (i in range(npos * qd)) {
                    q_ref[i] = q0h[i]
                }
                for (p in range(npos)) {
                    for (lp in range(ppp)) {
                        let is_k = lp >= kpair0
                        let pr = is_k ? lp - kpair0 : lp
                        let hh = pr / half
                        let j = pr % half
                        let fcr = csh[p * hs + j]
                        let fci = csh[p * hs + half + j]
                        if (is_k) {
                            let kb = p * kvd + hh * hs
                            let bk = bpad + qd + hh * hs
                            let bv = bpad + qd + kvd + hh * hs
                            let a0 = kvh[kb + j] + bh[bk + j]
                            let a1 = kvh[kb + j + half] + bh[bk + j + half]
                            let mo = layerbase + (pos0 + p) * kvd + hh * hs
                            mk_ref[mo + j] = a0 * fcr - a1 * fci
                            mk_ref[mo + j + half] = a0 * fci + a1 * fcr
                            mv_ref[mo + j] = kvh[voff + kb + j] + bh[bv + j]
                            mv_ref[mo + j + half] = kvh[voff + kb + j + half] + bh[bv + j + half]
                        } else {
                            let qb = p * qd + hh * hs
                            let bq = bpad + hh * hs
                            let a0 = q0h[qb + j] + bh[bq + j]
                            let a1 = q0h[qb + j + half] + bh[bq + j + half]
                            q_ref[qb + j] = a0 * fcr - a1 * fci
                            q_ref[qb + j + half] = a0 * fci + a1 * fcr
                        }
                    }
                }
                var bad = mismatch_approx(q_cls, q_ref)
                let w0 = layerbase + pos0 * kvd
                for (i in range(npos * kvd)) {
                    if (!approx(m_cls[w0 + i], mk_ref[w0 + i])) {
                        bad++
                    }
                    if (!approx(m_cls[mirn + w0 + i], mv_ref[w0 + i])) {
                        bad++
                    }
                }
                t |> success(bad == 0, "rope_kv_store_b matches the CPU oracle ({bad} off)")
                // the merged k|v projection layout (the DASLLAMA_VK_KV_MERGE default): [k | v] per position at
                // stride 2*kvd, v at +kvd - the same oracle rows must land; the mirrors take a sentinel first
                var kvm_h : array<float>
                kvm_h |> resize(npos * 2 * kvd)
                for (p in range(npos)) {
                    for (c in range(kvd)) {
                        kvm_h[p * 2 * kvd + c] = kvh[p * kvd + c]
                        kvm_h[p * 2 * kvd + kvd + c] = kvh[voff + p * kvd + c]
                    }
                }
                var msent : array<float>
                msent |> resize(mirn)
                for (i in range(mirn)) {
                    msent[i] = -999.25
                }
                unsafe {
                    upload_region_at(qd2, 0l, addr<uint8 const?>(q0h[0]), q_bytes)
                    upload_region_at(kvdv, 0l, addr<uint8 const?>(kvm_h[0]), kv_bytes)
                    upload_region_at(km2, 0l, addr<uint8 const?>(msent[0]), m_bytes)
                    upload_region_at(vm2, 0l, addr<uint8 const?>(msent[0]), m_bytes)
                    var scm = set_rope_kv_b_cls(fixed_array(qd2, kvdv, km2, vm2, csd, bd),
                        fixed_array(q_bytes, kv_bytes, m_bytes, m_bytes, cs_bytes, b_bytes),
                        fixed_array(1u, 16u, 2u, 4u, 8u, 0u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    var pc = RopeKvBArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs),
                        half = uint(half), neox = uint(neox), kpair0 = uint(kpair0),
                        ppp = uint(ppp), layerbase = uint(layerbase), npos = uint(npos),
                        pos0 = uint(pos0), voff = uint(kvd), kstride = uint(2 * kvd), boff = uint(bpad), hasb = 1u)
                    enc_rope_kv_b_cls(raw, h, scm, pc, int64(wgs))
                    vhz_dep(raw, h, 8u, 0u, true)
                    cmd_copy_whole(raw, qd2, host.buf, q_bytes)
                    cmd_copy_range(raw, km2, 0l, host.buf, q_bytes, m_bytes)
                    cmd_copy_range(raw, vm2, 0l, host.buf, q_bytes + m_bytes, m_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(q_cls[0]), host.mapped, q_bytes)
                    memcpy(addr<void?>(m_cls[0]), reinterpret<void?>(intptr(host.mapped) + uint64(q_bytes)), 2l * m_bytes)
                }
                var badm = mismatch_approx(q_cls, q_ref)
                for (i in range(npos * kvd)) {
                    if (!approx(m_cls[w0 + i], mk_ref[w0 + i])) {
                        badm++
                    }
                    if (!approx(m_cls[mirn + w0 + i], mv_ref[w0 + i])) {
                        badm++
                    }
                }
                t |> success(badm == 0, "rope_kv_store_b over merged k|v rows (kstride 2*kvd, voff kvd) matches the CPU oracle ({badm} off)")
                delete kvm_h
                delete msent
                delete q0h
                delete kvh
                delete csh
                delete bh
                delete q_ref
                delete mk_ref
                delete mv_ref
                delete q_cls
                delete m_cls
            }
            // --- qkn_rope_kv: qk-rms + rope + kv-store in one dispatch, one wg per head-row ---
            {
                let neox = 1
                // ONE mirror row both planes: the class twin composes it as layerbase + pos*kvd
                // (the serving ladder always wrote equal K/V rows; the independent-rows freedom
                // was the old kernel's meta accident, not a contract)
                let krow = 192
                let vrow = 192
                let eps = 0.00001
                let mirn2 = vrow + nkvh * hs
                var q0h : array<float>
                var kvh : array<float>
                var csh : array<float>
                var wnh : array<float>
                q0h |> resize(qd)
                kvh |> resize(qd + 2 * kvd)
                csh |> resize(hs)
                wnh |> resize(2 * hs)
                for (i in range(qd)) {
                    q0h[i] = float((i * 31) % 67) * 0.05 - 1.6
                }
                for (i in range(qd + 2 * kvd)) {
                    kvh[i] = float((i * 23) % 71) * 0.04 - 1.5
                }
                for (i in range(hs)) {
                    csh[i] = float((i * 13) % 41) * 0.045 - 0.9
                }
                for (i in range(2 * hs)) {
                    wnh[i] = 0.75 + float(i % 11) * 0.05
                }
                let q_bytes = int64(qd) * 4l
                let kv_bytes = int64(qd + 2 * kvd) * 4l
                let cs_bytes = int64(hs) * 4l
                let wn_bytes = int64(2 * hs) * 4l
                let m_bytes = int64(mirn2) * 4l
                let qd2 = make_device_buf(q_bytes)
                let kvdv = make_device_buf(kv_bytes)
                let csd = make_device_buf(cs_bytes)
                let wnd = make_device_buf(wn_bytes)
                let km2 = make_device_buf(m_bytes)
                let vm2 = make_device_buf(m_bytes)
                var host = make_host_buf(q_bytes + 2l * m_bytes, true, [cached = true])
                var q_cls : array<float>
                var m_cls : array<float>
                q_cls |> resize(qd)
                m_cls |> resize(2 * mirn2)
                let wgs = nh + nkvh
                let bpad = 96   // the bias rows sit past a pad, so boff is exercised off its default
                var bh : array<float>
                bh |> resize(bpad + qd + 2 * kvd)
                for (i in range(bpad + qd + 2 * kvd)) {
                    bh[i] = i < bpad ? 77.0 : float((i * 17) % 29) * 0.02 - 0.28
                }
                let b_bytes = long_length(bh) * 4l
                let bd = make_device_buf(b_bytes)
                unsafe {
                    upload_region_at(kvdv, 0l, addr<uint8 const?>(kvh[0]), kv_bytes)
                    upload_region_at(csd, 0l, addr<uint8 const?>(csh[0]), cs_bytes)
                    upload_region_at(wnd, 0l, addr<uint8 const?>(wnh[0]), wn_bytes)
                    upload_region_at(qd2, 0l, addr<uint8 const?>(q0h[0]), q_bytes)
                    upload_region_at(bd, 0l, addr<uint8 const?>(bh[0]), b_bytes)
                    var tokb = make_host_buf(16l, true)
                    var tmq = reinterpret<TokMeta?>(tokb.mapped)
                    tmq.pos = 0u   // mirror row = layerbase alone
                    tmq.cnt = 0u
                    tmq.mirbase = 0u
                    var sc = set_qkn_rope_cls(fixed_array(qd2, kvdv, km2, vm2, csd, wnd, tokb.buf, bd),
                        fixed_array(q_bytes, kv_bytes, m_bytes, m_bytes, cs_bytes, wn_bytes, 16l, b_bytes),
                        fixed_array(16u, 1u, 2u, 4u, 8u, 8u, 0u, 0u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    var pc = QknRopeArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs),
                        half = uint(half), neox = uint(neox), nh = uint(nh),
                        layerbase = uint(krow), qwoff = 0u,
                        kwoff = uint(hs), eps = eps, boff = uint(bpad), hasb = 1u)
                    enc_qkn_rope_cls(raw, h, sc, pc, int64(wgs))
                    vhz_dep(raw, h, 8u, 0u, true)
                    cmd_copy_whole(raw, qd2, host.buf, q_bytes)
                    cmd_copy_range(raw, km2, 0l, host.buf, q_bytes, m_bytes)
                    cmd_copy_range(raw, vm2, 0l, host.buf, q_bytes + m_bytes, m_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(q_cls[0]), host.mapped, q_bytes)
                    memcpy(addr<void?>(m_cls[0]), reinterpret<void?>(intptr(host.mapped) + uint64(q_bytes)), 2l * m_bytes)
                }
                // oracle: the bias row at boff, then per-head rms-norm with its weight row, then NEOX
                // rope off the normed row; k lands at the mirror row, v copies raw plus its bias
                var q_ref : array<float>
                var mk_ref : array<float>
                var mv_ref : array<float>
                q_ref |> resize(qd)
                mk_ref |> resize(mirn2)
                mv_ref |> resize(mirn2)
                for (wid in range(nh + nkvh)) {
                    let is_k = wid >= nh
                    let hh = is_k ? wid - nh : wid
                    let sbase = is_k ? qd + hh * hs : hh * hs
                    var ss = 0.0
                    for (i in range(hs)) {
                        let v = (is_k ? kvh[sbase + i] : q0h[sbase + i]) + bh[bpad + sbase + i]
                        ss += v * v
                    }
                    let inv = 1.0 / sqrt(ss / float(hs) + eps)
                    var nrow : array<float>
                    nrow |> resize(hs)
                    for (i in range(hs)) {
                        let v = (is_k ? kvh[sbase + i] : q0h[sbase + i]) + bh[bpad + sbase + i]
                        nrow[i] = wnh[(is_k ? hs : 0) + i] * (v * inv)
                    }
                    for (j in range(half)) {
                        let fcr = csh[j]
                        let fci = csh[half + j]
                        let r0 = nrow[j] * fcr - nrow[j + half] * fci
                        let r1 = nrow[j] * fci + nrow[j + half] * fcr
                        if (is_k) {
                            mk_ref[krow + hh * hs + j] = r0
                            mk_ref[krow + hh * hs + j + half] = r1
                        } else {
                            q_ref[hh * hs + j] = r0
                            q_ref[hh * hs + j + half] = r1
                        }
                    }
                    if (is_k) {
                        for (i in range(hs)) {
                            mv_ref[krow + hh * hs + i] = kvh[qd + kvd + hh * hs + i] + bh[bpad + qd + kvd + hh * hs + i]
                        }
                    }
                    delete nrow
                }
                var bad = mismatch_approx(q_cls, q_ref)
                for (i in range(nkvh * hs)) {
                    if (!approx(m_cls[krow + i], mk_ref[krow + i])) {
                        bad++
                    }
                    if (!approx(m_cls[mirn2 + vrow + i], mv_ref[krow + i])) {
                        bad++
                    }
                }
                t |> success(bad == 0, "qkn_rope_kv matches the CPU oracle ({bad} off)")
                //! the N-row form: two rows stacked, each at its own token meta, against the one-row kernel run on that row alone
                {
                    let kvrow = qd + 2 * kvd
                    let mn = krow + 2 * kvd   // row 0 at position 0 and row 1 at position 1: two mirror rows past layerbase
                    var q2h : array<float>
                    var kv2h : array<float>
                    var cs2h : array<float>
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
                    let mn_bytes = int64(mn) * 4l
                    let q1d = make_device_buf(q_bytes)
                    let kv1d = make_device_buf(kv_bytes)
                    let cs1d = make_device_buf(cs_bytes)
                    let km1 = make_device_buf(mn_bytes)
                    let vm1 = make_device_buf(mn_bytes)
                    let q2d = make_device_buf(2l * q_bytes)
                    let kv2d = make_device_buf(2l * kv_bytes)
                    let cs2d = make_device_buf(2l * cs_bytes)
                    let kmn = make_device_buf(mn_bytes)
                    let vmn = make_device_buf(mn_bytes)
                    var host2 = make_host_buf(3l * q_bytes + 4l * mn_bytes, true, [cached = true])
                    unsafe {
                        upload_region_at(q1d, 0l, addr<uint8 const?>(q2h[qd]), q_bytes)
                        upload_region_at(kv1d, 0l, addr<uint8 const?>(kv2h[kvrow]), kv_bytes)
                        upload_region_at(cs1d, 0l, addr<uint8 const?>(cs2h[hs]), cs_bytes)
                        upload_region_at(q2d, 0l, addr<uint8 const?>(q2h[0]), 2l * q_bytes)
                        upload_region_at(kv2d, 0l, addr<uint8 const?>(kv2h[0]), 2l * kv_bytes)
                        upload_region_at(cs2d, 0l, addr<uint8 const?>(cs2h[0]), 2l * cs_bytes)
                        nan_sentinel_upload(km1, mn_bytes)
                        nan_sentinel_upload(vm1, mn_bytes)
                        nan_sentinel_upload(kmn, mn_bytes)
                        nan_sentinel_upload(vmn, mn_bytes)
                        var tok1 = make_host_buf(16l, true)
                        var tm1 = reinterpret<TokMeta?>(tok1.mapped)
                        tm1.pos = 1u
                        tm1.cnt = 0u
                        tm1.parity = 0u
                        tm1.mirbase = 0u
                        var tok2 = make_host_buf(32l, true)
                        var tm2 = reinterpret<TokMeta?>(tok2.mapped)
                        for (r in range(2)) {
                            tm2[r].pos = uint(r)
                            tm2[r].cnt = 0u
                            tm2[r].parity = 0u
                            tm2[r].mirbase = 0u
                        }
                        var s1 = set_qkn_rope_cls(fixed_array(q1d, kv1d, km1, vm1, cs1d, wnd, tok1.buf, bd),
                            fixed_array(q_bytes, kv_bytes, mn_bytes, mn_bytes, cs_bytes, wn_bytes, 16l, b_bytes),
                            fixed_array(16u, 1u, 2u, 4u, 8u, 8u, 0u, 0u))
                        var s2 = set_qkn_rope_cls(fixed_array(q2d, kv2d, kmn, vmn, cs2d, wnd, tok2.buf, bd),
                            fixed_array(2l * q_bytes, 2l * kv_bytes, mn_bytes, mn_bytes, 2l * cs_bytes, wn_bytes, 32l, b_bytes),
                            fixed_array(16u, 1u, 2u, 4u, 8u, 8u, 0u, 0u))
                        var rawn = alloc_cmd()
                        let beginn = VkCommandBufferBeginInfo()
                        vk_check(vkBeginCommandBuffer(rawn, beginn), null)
                        var hn : VkHaz
                        var pc1 = QknRopeArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs),
                            half = uint(half), neox = uint(neox), nh = uint(nh),
                            layerbase = uint(krow), qwoff = 0u,
                            kwoff = uint(hs), eps = eps, boff = uint(bpad), hasb = 1u)
                        enc_qkn_rope_cls(rawn, hn, s1, pc1, int64(wgs))
                        var pc2 = QknRopeArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs),
                            half = uint(half), neox = uint(neox), nh = uint(nh),
                            layerbase = uint(krow), qwoff = 0u,
                            kwoff = uint(hs), eps = eps, boff = uint(bpad), hasb = 1u,
                            rowwg = uint(wgs), kvrow = uint(kvrow), qrow = uint(qd), cosrow = uint(hs))
                        enc_qkn_rope_cls(rawn, hn, s2, pc2, int64(wgs) * 2l)
                        vhz_dep(rawn, hn, 8u, 0u, true)
                        cmd_copy_whole(rawn, q1d, host2.buf, q_bytes)
                        cmd_copy_range(rawn, km1, 0l, host2.buf, q_bytes, mn_bytes)
                        cmd_copy_range(rawn, vm1, 0l, host2.buf, q_bytes + mn_bytes, mn_bytes)
                        cmd_copy_range(rawn, q2d, 0l, host2.buf, q_bytes + 2l * mn_bytes, 2l * q_bytes)
                        cmd_copy_range(rawn, kmn, 0l, host2.buf, 3l * q_bytes + 2l * mn_bytes, mn_bytes)
                        cmd_copy_range(rawn, vmn, 0l, host2.buf, 3l * q_bytes + 3l * mn_bytes, mn_bytes)
                        vk_check(vkEndCommandBuffer(rawn), null)
                        submit_wait(rawn)
                        let f = reinterpret<float?>(host2.mapped)
                        let km1o = qd
                        let vm1o = qd + mn
                        let q2o = qd + 2 * mn
                        let kmno = 3 * qd + 2 * mn
                        let vmno = 3 * qd + 3 * mn
                        var badn = 0
                        for (i in range(qd)) {   // row 0's q against the one-row run above, row 1's against its own
                            if (float_bits_to_uint(f[q2o + i]) != float_bits_to_uint(q_cls[i])) {
                                badn++
                            }
                            if (float_bits_to_uint(f[q2o + qd + i]) != float_bits_to_uint(f[i])) {
                                badn++
                            }
                        }
                        for (i in range(kvd)) {   // row 0's mirror rows against the one-row run's, row 1's (one row past) against its own
                            if (float_bits_to_uint(f[kmno + krow + i]) != float_bits_to_uint(m_cls[krow + i])) {
                                badn++
                            }
                            if (float_bits_to_uint(f[vmno + vrow + i]) != float_bits_to_uint(m_cls[mirn2 + vrow + i])) {
                                badn++
                            }
                            if (float_bits_to_uint(f[kmno + krow + kvd + i]) != float_bits_to_uint(f[km1o + krow + kvd + i])) {
                                badn++
                            }
                            if (float_bits_to_uint(f[vmno + vrow + kvd + i]) != float_bits_to_uint(f[vm1o + vrow + kvd + i])) {
                                badn++
                            }
                        }
                        t |> success(badn == 0, "qkn_rope_kv over two rows: each row's q and mirror rows are the one-row kernel's bit for bit ({badn} off)")
                        var stalen = 0
                        for (i in range(krow)) {   // nothing lands under layerbase on either mirror
                            if (float_bits_to_uint(f[kmno + i]) != NAN_SENTINEL_BITS || float_bits_to_uint(f[vmno + i]) != NAN_SENTINEL_BITS) {
                                stalen++
                            }
                        }
                        t |> success(stalen == 0, "qkn_rope_kv over two rows: the mirror rows under layerbase keep the sentinel ({stalen} written)")
                    }
                    delete q2h
                    delete kv2h
                    delete cs2h
                }
                delete q0h
                delete kvh
                delete csh
                delete wnh
                delete q_ref
                delete mk_ref
                delete mv_ref
                delete q_cls
                delete m_cls
            }
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
            var srcqh : array<float>
            var srckh : array<float>
            var smh : array<float>
            var bh : array<float>   //! the projection bias: [q (qd) | k (kvd)], the k pass reads at boff = qd
            srcqh |> resize(rows * 2 * qd)
            srckh |> resize(rows * kvd)
            smh |> resize(sm_n)
            bh |> resize(qd + kvd)
            for (i in range(rows * 2 * qd)) {
                srcqh[i] = float((i * 19) % 79) * 0.045 - 1.7
            }
            for (i in range(rows * kvd)) {
                srckh[i] = float((i * 27) % 59) * 0.055 - 1.4
            }
            for (i in range(sm_n)) {
                smh[i] = float((i * 11) % 37) * 0.05 - 0.8
            }
            for (i in range(qd + kvd)) {
                bh[i] = float((i * 7) % 23) * 0.03 - 0.3
            }
            smh[0] = eps
            let srcq_bytes = int64(rows * 2 * qd) * 4l
            let srck_bytes = int64(rows * kvd) * 4l
            let sm_bytes = int64(sm_n) * 4l
            let b_bytes = int64(qd + kvd) * 4l
            let qp_bytes = int64(rows * qd) * 4l
            let kp_bytes = int64((w0 + rows) * kvd) * 4l
            let srcqd = make_device_buf(srcq_bytes)
            let srckd = make_device_buf(srck_bytes)
            let smd = make_device_buf(sm_bytes)
            let bd = make_device_buf(b_bytes)
            let qp2 = make_device_buf(qp_bytes)
            let g2 = make_device_buf(qp_bytes)
            let kp2 = make_device_buf(kp_bytes)
            var host = make_host_buf(2l * qp_bytes + kp_bytes, true, [cached = true])
            var o_cls : array<float>
            o_cls |> resize(2 * rows * qd + (w0 + rows) * kvd)
            unsafe {
                upload_region_at(srcqd, 0l, addr<uint8 const?>(srcqh[0]), srcq_bytes)
                upload_region_at(srckd, 0l, addr<uint8 const?>(srckh[0]), srck_bytes)
                upload_region_at(smd, 0l, addr<uint8 const?>(smh[0]), sm_bytes)
                upload_region_at(bd, 0l, addr<uint8 const?>(bh[0]), b_bytes)
                // the class collapse: ONE kernel, two dispatches with different push values
                var scq = set_at_prep_cls(fixed_array(srcqd, qp2, g2, smd, bd),
                    fixed_array(srcq_bytes, qp_bytes, qp_bytes, sm_bytes, b_bytes),
                    fixed_array(8u, 1u, 2u, 4u, 32u))
                var sck = set_at_prep_cls(fixed_array(srckd, kp2, g2, smd, bd),
                    fixed_array(srck_bytes, kp_bytes, qp_bytes, sm_bytes, b_bytes),
                    fixed_array(8u, 16u, 0u, 4u, 32u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                var pcq = AtPrepArgs(d = uint(qd), nh = uint(nh), hs = uint(hs),
                    half = uint(half), flags = uint(flags), rms_off = uint(rmsq_off),
                    cos_off = uint(cos_off), sin_off = uint(sin_off), obase = 0u,
                    qsrc = 1u, boff = 0u, hasb = 1u, eps = eps)
                enc_at_prep_cls(raw, h, scq, pcq, int64(rows))
                var pck = AtPrepArgs(d = uint(kvd), nh = uint(nkvh), hs = uint(hs),
                    half = uint(half), flags = uint(flags), rms_off = uint(rmsk_off),
                    cos_off = uint(cos_off), sin_off = uint(sin_off), obase = uint(w0),
                    qsrc = 0u, boff = uint(qd), hasb = 1u, eps = eps)
                enc_at_prep_cls(raw, h, sck, pck, int64(rows))
                vhz_dep(raw, h, 1u | 2u | 16u, 0u, true)
                cmd_copy_whole(raw, qp2, host.buf, qp_bytes)
                cmd_copy_range(raw, g2, 0l, host.buf, qp_bytes, qp_bytes)
                cmd_copy_range(raw, kp2, 0l, host.buf, 2l * qp_bytes, kp_bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                memcpy(addr<void?>(o_cls[0]), host.mapped, 2l * qp_bytes + kp_bytes)
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
                        var ss = 0.0
                        for (j in range(hs)) {
                            ss += rrow[hh * hs + j] * rrow[hh * hs + j]
                        }
                        let inv = 1.0 / sqrt(ss / float(hs) + eps)
                        for (j in range(hs)) {
                            rrow[hh * hs + j] = smh[rms_off + j] * (rrow[hh * hs + j] * inv)
                        }
                    }
                    for (i in range(d)) {
                        let hh = i / hs
                        let j = i % hs
                        var outv = rrow[i]
                        if (j < half) {
                            outv = rrow[i] * smh[cos_off + p * half + j] - rrow[hh * hs + j + half] * smh[sin_off + p * half + j]
                        } elif (j < 2 * half) {
                            let jj = j - half
                            outv = rrow[hh * hs + jj] * smh[sin_off + p * half + jj] + rrow[i] * smh[cos_off + p * half + jj]
                        }
                        if (isq) {
                            o_ref[p * qd + i] = outv
                        } else {
                            o_ref[kb0 + p * kvd + i] = outv
                        }
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

[test]
def test_vkd_dn_family(t0 : T?) {
    t0 |> run("deltanet class kernels == the CPU oracle (conv, fused step, 2-phase scan)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!g_gpu.dn_shared_ok) {
                t |> skip("dn chain declined on this device (shared-memory cap)")
                return
            }
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
                var srch : array<float>
                var smh : array<float>
                srch |> resize(npos * cd)
                smh |> resize(sm_n)
                for (i in range(npos * cd)) {
                    srch[i] = float((i * 19) % 79) * 0.03 - 1.1
                }
                for (i in range(sm_n)) {
                    smh[i] = float((i * 11) % 37) * 0.02 - 0.35
                }
                smh[0] = eps
                smh[1] = qscale
                let src_bytes = int64(npos * cd) * 4l
                let sm_bytes = int64(sm_n) * 4l
                let out_bytes = int64(npos * cd) * 4l
                let srcd = make_device_buf(src_bytes)
                let smd = make_device_buf(sm_bytes)
                let od2 = make_device_buf(out_bytes)
                var host = make_host_buf(out_bytes, true, [cached = true])
                var o_cls : array<float>
                o_cls |> resize(npos * cd)
                unsafe {
                    upload_region_at(srcd, 0l, addr<uint8 const?>(srch[0]), src_bytes)
                    upload_region_at(smd, 0l, addr<uint8 const?>(smh[0]), sm_bytes)
                    var sc = set_dn_conv_cls(fixed_array(srcd, od2, smd),
                        fixed_array(src_bytes, out_bytes, sm_bytes),
                        fixed_array(8u, 1u, 4u))
                    var raw2 = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = DnConvArgs(npos = uint(npos), cd = uint(cd), kd = uint(kd), ds = uint(ds),
                        dconv = uint(dconv), hist_mode = uint(hist_mode), taps_off = uint(taps_off),
                        hist_off = uint(hist_off), eps = eps, qscale = qscale)
                    enc_dn_conv_cls(raw2, h2, sc, pc, dn_conv_wgs(int64(cd), int64(npos)))
                    vhz_dep(raw2, h2, 1u, 0u, true)
                    cmd_copy_whole(raw2, od2, host.buf, out_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(o_cls[0]), host.mapped, out_bytes)
                }
                // oracle: depthwise conv (history rows below position 0) + SiLU, then per-ds-block
                // L2 norm over the q/k regions with qscale on the q blocks
                var o_ref : array<float>
                o_ref |> resize(npos * cd)
                for (p in range(npos)) {
                    for (ch in range(cd)) {
                        var acc = 0.0
                        for (k in range(dconv)) {
                            let sp = p + k - taps
                            var xv = 0.0
                            if (sp >= 0) {
                                xv = srch[sp * cd + ch]
                            } else {
                                xv = smh[hist_off + ch * taps + p + k]   // hist_mode 1
                            }
                            acc += xv * smh[taps_off + ch * dconv + k]
                        }
                        o_ref[p * cd + ch] = acc / (1.0 + exp(-acc))
                    }
                    for (b in range(2 * kd / ds)) {
                        var sum = 0.0
                        for (j in range(ds)) {
                            sum += o_ref[p * cd + b * ds + j] * o_ref[p * cd + b * ds + j]
                        }
                        let s = (1.0 / max(sqrt(sum), eps)) * (b < kd / ds ? qscale : 1.0)
                        for (j in range(ds)) {
                            o_ref[p * cd + b * ds + j] *= s
                        }
                    }
                }
                let bad = mismatch_approx(o_cls, o_ref)
                t |> success(bad == 0, "dn_conv matches the CPU oracle at ds {ds} ({bad} of {npos * cd} off)")
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
                var projh : array<float>
                var sth : array<float>
                var smh : array<float>
                projh |> resize(cd + nvh * ds)
                sth |> resize(nvh * ds * ds)
                smh |> resize(sm_n)
                for (i in range(cd + nvh * ds)) {
                    projh[i] = float((i * 23) % 71) * 0.03 - 1.0
                }
                for (i in range(nvh * ds * ds)) {
                    sth[i] = float((i * 13) % 53) * 0.02 - 0.5
                }
                for (i in range(sm_n)) {
                    smh[i] = float((i * 17) % 43) * 0.02 - 0.4
                }
                smh[0] = eps
                smh[1] = qscale
                smh[2] = float(par)
                for (i in range(nvh)) {
                    smh[a_off + i] = -0.4 - float(i) * 0.1   // a stays negative (decay)
                }
                let proj_bytes = int64(cd + nvh * ds) * 4l
                let st_bytes = int64(nvh * ds * ds) * 4l
                let sm_bytes = int64(sm_n) * 4l
                let sc_bytes = int64(nblk) * 4l
                let q_bytes = int64(nblk) * 32l
                let projd = make_device_buf(proj_bytes)
                let st2 = make_device_buf(st_bytes)
                let sm2 = make_device_buf(sm_bytes)
                let scd2 = make_device_buf(sc_bytes)
                let qd2 = make_device_buf(q_bytes)
                let of_bytes = int64(nvh * ds) * 4l
                let ofd2 = make_device_buf(of_bytes)
                var host = make_host_buf(st_bytes + sm_bytes + sc_bytes + q_bytes, true, [cached = true])
                var st_cls : array<float>
                var sm_cls : array<float>
                var sc_cls : array<float>
                var q_cls : array<uint>
                st_cls |> resize(nvh * ds * ds)
                sm_cls |> resize(sm_n)
                sc_cls |> resize(nblk)
                q_cls |> resize(nblk * 8)
                unsafe {
                    upload_region_at(projd, 0l, addr<uint8 const?>(projh[0]), proj_bytes)
                    upload_region_at(st2, 0l, addr<uint8 const?>(sth[0]), st_bytes)
                    upload_region_at(sm2, 0l, addr<uint8 const?>(smh[0]), sm_bytes)
                    var tokb = make_host_buf(16l, true)
                    var tmd = reinterpret<TokMeta?>(tokb.mapped)
                    tmd.parity = uint(par)
                    var sc = set_dn_step_cls(fixed_array(projd, st2, sm2, scd2, qd2, tokb.buf, ofd2),
                        fixed_array(proj_bytes, st_bytes, sm_bytes, sc_bytes, q_bytes, 16l, of_bytes),
                        fixed_array(16u, 2u, 8u, 1u, 4u, 0u, 32u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    var pc = DnStepArgs(cd = uint(cd), kd = uint(kd), nkh = uint(nkh),
                        ds = uint(ds), dconv = uint(dconv), beta_off = uint(beta_off),
                        g_off = uint(g_off), taps_off = uint(taps_off),
                        wnorm_off = uint(wnorm_off), a_off = uint(a_off),
                        dt_off = uint(dt_off), hist_off = uint(hist_off),
                        eps = eps, qscale = qscale)
                    enc_dn_step_cls(raw, h, sc, pc, int64(nvh))
                    vhz_dep(raw, h, 1u | 2u | 4u | 8u, 0u, true)
                    cmd_copy_whole(raw, st2, host.buf, st_bytes)
                    cmd_copy_range(raw, sm2, 0l, host.buf, st_bytes, sm_bytes)
                    cmd_copy_range(raw, scd2, 0l, host.buf, st_bytes + sm_bytes, sc_bytes)
                    cmd_copy_range(raw, qd2, 0l, host.buf, st_bytes + sm_bytes + sc_bytes, q_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(st_cls[0]), host.mapped, st_bytes)
                    memcpy(addr<void?>(sm_cls[0]), reinterpret<void?>(intptr(host.mapped) + uint64(st_bytes)), sm_bytes)
                    memcpy(addr<void?>(sc_cls[0]), reinterpret<void?>(intptr(host.mapped) + uint64(st_bytes + sm_bytes)), sc_bytes)
                    memcpy(addr<void?>(q_cls[0]), reinterpret<void?>(intptr(host.mapped) + uint64(st_bytes + sm_bytes + sc_bytes)), q_bytes)
                }
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
                for (hh in range(nvh)) {
                    let kh = hh % nkh
                    var dq : array<float>
                    var dk : array<float>
                    var dv : array<float>
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
                            let sv = acc / (1.0 + exp(-acc))
                            if (sl == 0) {
                                dq[j] = sv
                            } elif (sl == 1) {
                                dk[j] = sv
                            } else {
                                dv[j] = sv
                            }
                        }
                    }
                    var sumq = 0.0
                    var sumk = 0.0
                    for (j in range(ds)) {
                        sumq += dq[j] * dq[j]
                        sumk += dk[j] * dk[j]
                    }
                    let sq = (1.0 / max(sqrt(sumq), eps)) * qscale
                    let sk = 1.0 / max(sqrt(sumk), eps)
                    let graw = smh[g_off + hh] + smh[dt_off + hh]
                    let eg = exp(smh[a_off + hh] * (graw > 20.0 ? graw : log(1.0 + exp(graw))))
                    let bt = 1.0 / (1.0 + exp(-smh[beta_off + hh]))
                    var qk = 0.0
                    for (j in range(ds)) {
                        dq[j] *= sq
                        dk[j] *= sk
                        qk += dq[j] * dk[j]
                    }
                    let sbase = hh * ds * ds
                    var dvn : array<float>
                    var dout : array<float>
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
                        out_ref[hh * ds + j] = dout[j] * inv * smh[wnorm_off + j] * (zv / (1.0 + exp(-zv)))
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
                let bad = (mismatch_approx(st_cls, st_ref) + mismatch_approx(sm_cls, sm_ref)
                    + mismatch_approx(sc_cls, s_ref) + mismatch_qbytes(q_cls, q_ref, 1))
                t |> success(bad == 0, "dn_step_fused matches the CPU oracle at ds {ds} ({bad} elements off)")
                //! the f32 arm (oform 1): the same step leaves the o row as floats - the pre-quant oracle row - and advances state and ring alike
                var of_f32 : array<float>
                var st_f32 : array<float>
                var sm_f32 : array<float>
                of_f32 |> resize(nvh * ds)
                st_f32 |> resize(nvh * ds * ds)
                sm_f32 |> resize(sm_n)
                unsafe {
                    upload_region_at(st2, 0l, addr<uint8 const?>(sth[0]), st_bytes)
                    upload_region_at(sm2, 0l, addr<uint8 const?>(smh[0]), sm_bytes)
                    nan_sentinel_upload(ofd2, of_bytes)
                    var tokf = make_host_buf(16l, true)
                    var tmf = reinterpret<TokMeta?>(tokf.mapped)
                    tmf.parity = uint(par)
                    var scf = set_dn_step_cls(fixed_array(projd, st2, sm2, scd2, qd2, tokf.buf, ofd2),
                        fixed_array(proj_bytes, st_bytes, sm_bytes, sc_bytes, q_bytes, 16l, of_bytes),
                        fixed_array(16u, 2u, 8u, 1u, 4u, 0u, 32u))
                    var hostf = make_host_buf(of_bytes + st_bytes + sm_bytes, true, [cached = true])
                    var rawf = alloc_cmd()
                    let beginf = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(rawf, beginf), null)
                    var hf : VkHaz
                    var pcf = DnStepArgs(cd = uint(cd), kd = uint(kd), nkh = uint(nkh),
                        ds = uint(ds), dconv = uint(dconv), beta_off = uint(beta_off),
                        g_off = uint(g_off), taps_off = uint(taps_off),
                        wnorm_off = uint(wnorm_off), a_off = uint(a_off),
                        dt_off = uint(dt_off), hist_off = uint(hist_off),
                        eps = eps, qscale = qscale, out_f32 = 1u)
                    enc_dn_step_cls(rawf, hf, scf, pcf, int64(nvh))
                    vhz_dep(rawf, hf, 2u | 8u | 32u, 0u, true)
                    cmd_copy_whole(rawf, ofd2, hostf.buf, of_bytes)
                    cmd_copy_range(rawf, st2, 0l, hostf.buf, of_bytes, st_bytes)
                    cmd_copy_range(rawf, sm2, 0l, hostf.buf, of_bytes + st_bytes, sm_bytes)
                    vk_check(vkEndCommandBuffer(rawf), null)
                    submit_wait(rawf)
                    memcpy(addr<void?>(of_f32[0]), hostf.mapped, of_bytes)
                    memcpy(addr<void?>(st_f32[0]), reinterpret<void?>(intptr(hostf.mapped) + uint64(of_bytes)), st_bytes)
                    memcpy(addr<void?>(sm_f32[0]), reinterpret<void?>(intptr(hostf.mapped) + uint64(of_bytes + st_bytes)), sm_bytes)
                }
                let badf = mismatch_approx(of_f32, out_ref) + mismatch_approx(st_f32, st_ref) + mismatch_approx(sm_f32, sm_ref)
                t |> success(badf == 0, "dn_step_fused's f32 o row (oform 1) at ds {ds} matches the pre-quant oracle with state and ring advanced alike ({badf} elements off)")
                delete of_f32
                delete st_f32
                delete sm_f32
                delete projh
                delete sth
                delete smh
                delete sm_ref
                delete st_ref
                delete out_ref
                delete q_ref
                delete s_ref
                delete st_cls
                delete sm_cls
                delete sc_cls
                delete q_cls
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
                let slab_n = o_off + npos * di
                let beta_off = 16
                let g_off = beta_off + npos * nvh
                let a_off = g_off + npos * nvh
                let dt_off = a_off + nvh
                let wnorm_off = dt_off + nvh
                let sm_n = wnorm_off + ds
                var convh : array<float>
                var zh : array<float>
                var sth : array<float>
                var smh : array<float>
                convh |> resize(npos * cd)
                zh |> resize(npos * di)
                sth |> resize(nvh * ds * ds)
                smh |> resize(sm_n)
                for (i in range(npos * cd)) {   //! unit-scale q/k rows, the conv stage's L2-normed shape: on unnormed rows the delta rule's recurrence overflows float range within the window at ds 128
                    convh[i] = (float((i * 19) % 79) * 0.025 - 0.9) * 0.15
                }
                for (i in range(npos * di)) {
                    zh[i] = float((i * 29) % 61) * 0.05 - 1.5
                }
                for (i in range(nvh * ds * ds)) {
                    sth[i] = float((i * 13) % 53) * 0.02 - 0.5
                }
                for (i in range(sm_n)) {
                    smh[i] = float((i * 17) % 43) * 0.03 - 0.6
                }
                smh[0] = eps
                for (i in range(nvh)) {
                    smh[a_off + i] = -0.4 - float(i) * 0.1
                }
                for (i in range(ds)) {
                    smh[wnorm_off + i] = 0.8 + float(i % 9) * 0.04
                }
                let conv_bytes = int64(npos * cd) * 4l
                let z_bytes = int64(npos * di) * 4l
                let st_bytes = int64(nvh * ds * ds) * 4l
                let sm_bytes = int64(sm_n) * 4l
                let slab_bytes = int64(slab_n) * 4l
                let convd = make_device_buf(conv_bytes)
                let zd = make_device_buf(z_bytes)
                let st2 = make_device_buf(st_bytes)
                let smd = make_device_buf(sm_bytes)
                let sl2 = make_device_buf(slab_bytes)
                let o_bytes = int64(npos * di) * 4l
                var host = make_host_buf(o_bytes + st_bytes, true, [cached = true])
                var o_cls : array<float>
                let cmp_n = npos * di + nvh * ds * ds
                o_cls |> resize(cmp_n)
                nan_sentinel_upload(sl2, o_bytes, int64(o_off) * 4l)   //! the o rows under the sentinel: a row the scan or the out-norm skips reds
                unsafe {
                    upload_region_at(convd, 0l, addr<uint8 const?>(convh[0]), conv_bytes)
                    upload_region_at(zd, 0l, addr<uint8 const?>(zh[0]), z_bytes)
                    upload_region_at(smd, 0l, addr<uint8 const?>(smh[0]), sm_bytes)
                    upload_region_at(st2, 0l, addr<uint8 const?>(sth[0]), st_bytes)
                    var sc1 = set_dn_scan_cls(fixed_array(convd, st2, smd, sl2),
                        fixed_array(conv_bytes, st_bytes, sm_bytes, slab_bytes),
                        fixed_array(1u, 2u, 8u, 4u))
                    var sc3 = set_dn_scan_p3_cls(fixed_array(zd, sl2, smd),
                        fixed_array(z_bytes, slab_bytes, sm_bytes),
                        fixed_array(16u, 4u, 8u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    var pcs = DnScanArgs(npos = uint(npos), cd = uint(cd), kd = uint(kd),
                        nvh = uint(nvh), nkh = uint(nkh), ds = uint(ds),
                        beta_off = uint(beta_off), g_off = uint(g_off), a_off = uint(a_off),
                        dt_off = uint(dt_off), o_off = uint(o_off), di = uint(di))
                    enc_dn_scan_cls(raw, h, sc1, pcs, dn_scan_wgs(int64(nvh), int64(ds)))
                    var pc3 = DnScanP3Args(npos = uint(npos), nvh = uint(nvh), di = uint(di), ds = uint(ds),
                        o_off = uint(o_off), wnorm_off = uint(wnorm_off), eps = eps)
                    enc_dn_scan_p3_cls(raw, h, sc3, pc3, int64(npos))
                    vhz_dep(raw, h, 2u | 4u, 0u, true)
                    cmd_copy_range(raw, sl2, int64(o_off) * 4l, host.buf, 0l, o_bytes)
                    cmd_copy_range(raw, st2, 0l, host.buf, o_bytes, st_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(o_cls[0]), host.mapped, o_bytes + st_bytes)
                }
                // oracle: the SEQUENTIAL delta rule — the chunked WY form equals it in exact
                // arithmetic, so the drift is pure reassociation; peak-floored bars absorb it
                var o_ref : array<float>
                o_ref |> resize(cmp_n)
                for (hh in range(nvh)) {
                    let kh = hh % nkh
                    var st_h : array<float>
                    st_h |> resize(ds * ds)
                    for (e in range(ds * ds)) {
                        st_h[e] = sth[hh * ds * ds + e]
                    }
                    var gcum = 0.0
                    for (tt in range(npos)) {
                        let crow = tt * cd
                        let graw = smh[g_off + tt * nvh + hh] + smh[dt_off + hh]
                        let gt = smh[a_off + hh] * (graw > 20.0 ? graw : log(1.0 + exp(graw)))
                        gcum += gt
                        let eg = exp(gt)
                        let bt = 1.0 / (1.0 + exp(-smh[beta_off + tt * nvh + hh]))
                        var dvn : array<float>
                        dvn |> resize(ds)
                        for (j in range(ds)) {
                            var ks = 0.0
                            for (i in range(ds)) {
                                ks += convh[crow + kd + kh * ds + i] * st_h[i * ds + j]
                            }
                            dvn[j] = bt * (convh[crow + 2 * kd + hh * ds + j] - eg * ks)
                        }
                        for (e in range(ds * ds)) {
                            st_h[e] = st_h[e] * eg + convh[crow + kd + kh * ds + e / ds] * dvn[e % ds]
                        }
                        var orow : array<float>
                        orow |> resize(ds)
                        var sum = 0.0
                        for (j in range(ds)) {
                            var qs = 0.0
                            for (i in range(ds)) {
                                qs += convh[crow + kh * ds + i] * st_h[i * ds + j]
                            }
                            orow[j] = qs
                            sum += qs * qs
                        }
                        let inv = 1.0 / sqrt(sum / float(ds) + eps)
                        for (j in range(ds)) {
                            let zv = zh[tt * di + hh * ds + j]
                            o_ref[tt * di + hh * ds + j] = orow[j] * inv * smh[wnorm_off + j] * (zv / (1.0 + exp(-zv)))
                        }
                        delete dvn
                        delete orow
                    }
                    for (e in range(ds * ds)) {
                        o_ref[npos * di + hh * ds * ds + e] = st_h[e]
                    }
                    delete st_h
                }
                let floor = 1e-2 * max_abs(o_ref)
                let bad = mismatch_bars(o_cls, o_ref, 1e-2, floor)
                t |> success(bad == 0, "dn_scan + out-norm matches the CPU oracle at ds {ds} ({bad} of {cmp_n} off)")
                var poison := o_cls   //! the bar's control: one normed o value pushed past both bars must red
                poison[0] += 1.0 + 2.0 * max_abs(o_ref)
                t |> success(mismatch_bars(poison, o_ref, 1e-2, floor) > 0, "dn_scan + out-norm at ds {ds}: the bar reds a poisoned value")
                delete poison
                delete convh
                delete zh
                delete sth
                delete smh
                delete o_ref
                delete o_cls
            }
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
            let dim = 256
            let nvh = 8
            let ne = 2 * nvh
            let woff = 64          // the layer's row base in the f32 plane
            let ostride = nvh
            for (npos in [21, 40]) {   // a partial 16-row tile alone; two full tiles + a partial one
                let ob_b = 16
                let ob_g = ob_b + npos * ostride + 8
                let out_n = ob_g + npos * ostride
                var wh : array<float>
                var xh : array<float>
                var want : array<float>
                wh |> resize(woff + ne * dim)
                xh |> resize(npos * dim)
                want |> resize(out_n)
                for (i in range(length(wh))) {
                    wh[i] = float(hash_word(uint(i)) % 1000u) / 500.0 - 1.0
                }
                for (i in range(length(xh))) {
                    xh[i] = float(hash_word(uint(i) + 77777u) % 1000u) / 500.0 - 1.0
                }
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
                let w_bytes = int64(length(wh16)) * 2l
                let x_bytes = int64(length(xh)) * 4l
                let o_bytes = int64(out_n) * 4l
                let wd = make_device_buf(w_bytes)
                let xd = make_device_buf(x_bytes)
                let od = make_device_buf(o_bytes)
                var host = make_host_buf(o_bytes, true, [cached = true])
                var got : array<float>
                got |> resize(out_n)
                nan_sentinel_upload(od, o_bytes)
                unsafe {
                    upload_region_at(wd, 0l, addr<uint8 const?>(wh16[0]), w_bytes)
                    upload_region_at(xd, 0l, addr<uint8 const?>(xh[0]), x_bytes)
                    var sc = set_dn_ba_cls(fixed_array(wd, xd, od), fixed_array(w_bytes, x_bytes, o_bytes), fixed_array(0u, 1u, 2u))
                    var raw = alloc_cmd()
                    let begin = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    var pc = DnBaArgs(npos = uint(npos), dim = uint(dim), nvh = uint(nvh), woff = uint(woff), xstride = uint(dim),
                        obase_b = uint(ob_b), obase_g = uint(ob_g), ostride = uint(ostride))
                    enc_dn_ba_cls(raw, h, sc, pc, dn_ba_wgs(int64(npos), int64(nvh)))
                    vhz_dep(raw, h, 2u, 0u, true)
                    cmd_copy_range(raw, od, 0l, host.buf, 0l, o_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(got[0]), host.mapped, o_bytes)
                }
                let bad = ba_plane_mismatches(got, want, npos, ne, nvh, ob_b, ob_g, ostride)
                t |> success(bad == 0, "dn_ba npos {npos}: {bad} of {npos * ne} outputs off the CPU dot")
                var poison := got   // the bar's control: one output pushed past it must red
                poison[ob_g] += 1.0 + 2.0 * abs(want[ob_g])
                t |> success(ba_plane_mismatches(poison, want, npos, ne, nvh, ob_b, ob_g, ostride) > 0, "dn_ba npos {npos}: the bar reds a poisoned output")
                delete poison
                delete wh
                delete wh16
                delete xh
                delete want
                delete got
            }
        } else {
            t |> skip("dasVulkan not present")
        }
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
            let o_n = o_off + npos * di
            var convh : array<float>
            var sth : array<float>
            var smh : array<float>
            convh |> resize(npos * cd)
            sth |> resize(nvh * ds * ds)
            smh |> resize(sm_n)
            for (i in range(npos * cd)) {   // unit-scale q/k rows keep the 512-step recurrence bounded
                convh[i] = (float((i * 19) % 79) * 0.025 - 0.9) * 0.15
            }
            for (i in range(nvh * ds * ds)) {
                sth[i] = float((i * 13) % 53) * 0.02 - 0.5
            }
            for (i in range(sm_n)) {
                smh[i] = float((i * 17) % 43) * 0.03 - 0.6
            }
            for (i in range(nvh)) {
                smh[a_off + i] = -0.4 - float(i) * 0.1
            }
            let conv_bytes = int64(npos * cd) * 4l
            let st_bytes = int64(nvh * ds * ds) * 4l
            let sm_bytes = int64(sm_n) * 4l
            let o_bytes = int64(o_n) * 4l
            let convd = make_device_buf(conv_bytes)
            let st2 = make_device_buf(st_bytes)
            let smd = make_device_buf(sm_bytes)
            let od = make_device_buf(o_bytes)
            var host = make_host_buf(o_bytes + st_bytes, true, [cached = true])
            var got : array<float>
            got |> resize(o_n + nvh * ds * ds)
            let wgs = dn_scan_wgs(int64(nvh), int64(ds))
            nan_sentinel_upload(od, o_bytes - int64(o_off) * 4l, int64(o_off) * 4l)   // the header stays as allocated: the oracle holds it at zero
            unsafe {
                upload_region_at(convd, 0l, addr<uint8 const?>(convh[0]), conv_bytes)
                upload_region_at(smd, 0l, addr<uint8 const?>(smh[0]), sm_bytes)
                upload_region_at(st2, 0l, addr<uint8 const?>(sth[0]), st_bytes)
                var sc = set_dn_scan_cls(fixed_array(convd, st2, smd, od),
                    fixed_array(conv_bytes, st_bytes, sm_bytes, o_bytes),
                    fixed_array(1u, 2u, 8u, 4u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                var pcs = DnScanArgs(npos = uint(npos), cd = uint(cd), kd = uint(kd),
                    nvh = uint(nvh), nkh = uint(nkh), ds = uint(ds),
                    beta_off = uint(beta_off), g_off = uint(g_off), a_off = uint(a_off),
                    dt_off = uint(dt_off), o_off = uint(o_off), di = uint(di))
                enc_dn_scan_cls(raw, h, sc, pcs, wgs)
                vhz_dep(raw, h, 2u | 4u, 0u, true)
                cmd_copy_range(raw, od, 0l, host.buf, 0l, o_bytes)
                cmd_copy_range(raw, st2, 0l, host.buf, o_bytes, st_bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                to_log(LOG_INFO, "dn_scan {tag}: dispatching {wgs} workgroups over {npos} rows\n")
                let tick = ref_time_ticks()
                submit_wait(raw)
                to_log(LOG_INFO, "dn_scan {tag}: submit + wait {get_time_usec(tick)} us\n")
                memcpy(addr<void?>(got[0]), host.mapped, o_bytes + st_bytes)
            }
            // oracle: the sequential delta rule per head, raw o rows (the out-norm is phase 3's)
            var ref : array<float>
            ref |> resize(length(got))
            for (i in range(o_off)) {
                ref[i] = 0.0
            }
            var st_h : array<float>
            var dvn : array<float>
            st_h |> resize(ds * ds)
            dvn |> resize(ds)
            for (hh in range(nvh)) {
                let kh = hh % nkh
                for (e in range(ds * ds)) {
                    st_h[e] = sth[hh * ds * ds + e]
                }
                for (tt in range(npos)) {
                    let crow = tt * cd
                    let graw = smh[g_off + tt * nvh + hh] + smh[dt_off + hh]
                    let eg = exp(smh[a_off + hh] * (graw > 20.0 ? graw : log(1.0 + exp(graw))))
                    let bt = 1.0 / (1.0 + exp(-smh[beta_off + tt * nvh + hh]))
                    for (j in range(ds)) {
                        var ks = 0.0
                        for (i in range(ds)) {
                            ks += convh[crow + kd + kh * ds + i] * st_h[i * ds + j]
                        }
                        dvn[j] = bt * (convh[crow + 2 * kd + hh * ds + j] - eg * ks)
                    }
                    for (e in range(ds * ds)) {
                        st_h[e] = st_h[e] * eg + convh[crow + kd + kh * ds + e / ds] * dvn[e % ds]
                    }
                    for (j in range(ds)) {
                        var qs = 0.0
                        for (i in range(ds)) {
                            qs += convh[crow + kh * ds + i] * st_h[i * ds + j]
                        }
                        ref[o_off + tt * di + hh * ds + j] = qs
                    }
                }
                for (e in range(ds * ds)) {
                    ref[o_n + hh * ds * ds + e] = st_h[e]
                }
            }
            let floor = 1e-2 * max_abs(ref)
            let bad = mismatch_bars(got, ref, 1e-2, floor)
            t |> success(bad == 0, "dn_scan {tag}: {bad} of {length(got)} o-row and state values off the sequential oracle")
            var poison := got   // the bar's control: one o value pushed past both bars must red
            poison[o_off] += 1.0 + 2.0 * max_abs(ref)
            t |> success(mismatch_bars(poison, ref, 1e-2, floor) > 0, "dn_scan {tag}: the bar reds a poisoned value")
            delete poison
            delete convh
            delete sth
            delete smh
            delete got
            delete ref
            delete st_h
            delete dvn
        }
    }
}

// the 9B-class geometry the small-model zoo never reaches: 32 value heads over ds 128 (the scan's
// 8-lane column clusters, all three shuffle stages) and a full 64-row beta/alpha tile
[test]
def test_vkd_dn_9b_scan(t0 : T?) {
    t0 |> run("dn_scan at the 9B geometry (32 heads, ds 128, 512 rows) == the sequential CPU delta rule") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!g_gpu.dn_shared_ok) {
                t |> skip("dn chain declined on this device (shared-memory cap)")
                return
            }
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
            if (!vk_moe_init()) {
                t |> skip("no Vulkan device")
                return
            }
            if (!g_gpu.dn_shared_ok) {
                t |> skip("dn chain declined on this device (shared-memory cap)")
                return
            }
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
            let dim = 4096
            let nvh = 32
            let ne = 2 * nvh
            let npos = 512
            let woff = ne * dim   // the second layer's rows: the base offset is under test too
            let ostride = nvh
            let ob_b = 16
            let ob_g = ob_b + npos * ostride + 8
            let out_n = ob_g + npos * ostride
            var wh : array<float>
            var xh : array<float>
            var want : array<float>
            wh |> resize(woff + ne * dim)
            xh |> resize(npos * dim)
            want |> resize(out_n)
            for (i in range(length(wh))) {
                wh[i] = float(hash_word(uint(i)) % 1000u) / 500.0 - 1.0
            }
            for (i in range(length(xh))) {
                xh[i] = float(hash_word(uint(i) + 77777u) % 1000u) / 500.0 - 1.0
            }
            var wh16 <- pack_f16(wh)
            for (p in range(npos)) {
                for (e in range(ne)) {
                    var acc = 0.0
                    for (k in range(dim)) {
                        acc += float(wh16[woff + e * dim + k]) * xh[p * dim + k]
                    }
                    want[(e < nvh ? ob_b : ob_g) + p * ostride + e % nvh] = acc
                }
            }
            let w_bytes = int64(length(wh16)) * 2l
            let x_bytes = int64(length(xh)) * 4l
            let o_bytes = int64(out_n) * 4l
            let wd = make_device_buf(w_bytes)
            let xd = make_device_buf(x_bytes)
            let od = make_device_buf(o_bytes)
            var host = make_host_buf(o_bytes, true, [cached = true])
            var got : array<float>
            got |> resize(out_n)
            let wgs = dn_ba_wgs(int64(npos), int64(nvh))
            nan_sentinel_upload(od, o_bytes)
            unsafe {
                upload_region_at(wd, 0l, addr<uint8 const?>(wh16[0]), w_bytes)
                upload_region_at(xd, 0l, addr<uint8 const?>(xh[0]), x_bytes)
                var sc = set_dn_ba_cls(fixed_array(wd, xd, od), fixed_array(w_bytes, x_bytes, o_bytes), fixed_array(0u, 1u, 2u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                var pc = DnBaArgs(npos = uint(npos), dim = uint(dim), nvh = uint(nvh), woff = uint(woff), xstride = uint(dim),
                    obase_b = uint(ob_b), obase_g = uint(ob_g), ostride = uint(ostride))
                enc_dn_ba_cls(raw, h, sc, pc, wgs)
                vhz_dep(raw, h, 2u, 0u, true)
                cmd_copy_range(raw, od, 0l, host.buf, 0l, o_bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                to_log(LOG_INFO, "dn_ba 9B: dispatching {wgs} workgroups over {npos} rows x {ne} outputs x {dim}\n")
                let tick = ref_time_ticks()
                submit_wait(raw)
                to_log(LOG_INFO, "dn_ba 9B: submit + wait {get_time_usec(tick)} us\n")
                memcpy(addr<void?>(got[0]), host.mapped, o_bytes)
            }
            let bad = ba_plane_mismatches(got, want, npos, ne, nvh, ob_b, ob_g, ostride)
            t |> success(bad == 0, "dn_ba 9B geometry: {bad} of {npos * ne} outputs off the CPU dot")
            var poison := got   // the bar's control: one output pushed past it must red
            poison[ob_b] += 1.0 + 2.0 * abs(want[ob_b])
            t |> success(ba_plane_mismatches(poison, want, npos, ne, nvh, ob_b, ob_g, ostride) > 0, "dn_ba 9B geometry: the bar reds a poisoned output")
            delete poison
            delete wh
            delete wh16
            delete xh
            delete want
            delete got
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
            var wh : array<float>
            var xh : array<float>
            var want : array<float>
            wh |> resize(woff + ne * dim)
            xh |> resize(dim)
            want |> resize(out_n)
            for (i in range(length(wh))) {
                wh[i] = float(hash_word(uint(i) + 4242u) % 1000u) / 500.0 - 1.0
            }
            for (i in range(dim)) {
                xh[i] = float(hash_word(uint(i) + 99u) % 1000u) / 500.0 - 1.0
            }
            var wh16 <- pack_f16(wh)
            for (e in range(ne)) {
                var acc = 0.0
                for (k in range(dim)) {
                    acc += float(wh16[woff + e * dim + k]) * xh[k]
                }
                want[obase + e] = acc
            }
            let w_bytes = int64(length(wh16)) * 2l
            let x_bytes = int64(dim) * 4l
            let o_bytes = int64(out_n) * 4l
            let wd = make_device_buf(w_bytes)
            let xd = make_device_buf(x_bytes)
            let od = make_device_buf(o_bytes)
            var host = make_host_buf(o_bytes, true, [cached = true])
            var got : array<float>
            got |> resize(out_n)
            nan_sentinel_upload(od, o_bytes)
            unsafe {
                upload_region_at(wd, 0l, addr<uint8 const?>(wh16[0]), w_bytes)
                upload_region_at(xd, 0l, addr<uint8 const?>(xh[0]), x_bytes)
                var sc = set_router_gemv_f16_cls(fixed_array(wd, xd, od, xd), fixed_array(w_bytes, x_bytes, o_bytes, x_bytes), fixed_array(0u, 1u, 2u, 0u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                var pc = RouterArgs(ne = uint(ne), dim = uint(dim), woff = uint(woff), obase = uint(obase))
                enc_router_gemv_f16_cls(raw, h, sc, pc, int64(ne))
                vhz_dep(raw, h, 2u, 0u, true)
                cmd_copy_range(raw, od, 0l, host.buf, 0l, o_bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                memcpy(addr<void?>(got[0]), host.mapped, o_bytes)
            }
            let bad = rel_mismatches(got, want, obase, ne)
            t |> success(bad == 0, "router_gemv_f16: {bad} of {ne} outputs off the CPU dot")
            var poison := got   // the bar's control: one output pushed past it must red
            poison[obase] += 1.0 + 2.0 * abs(want[obase])
            t |> success(rel_mismatches(poison, want, obase, ne) > 0, "router_gemv_f16: the bar reds a poisoned output")
            delete poison
            //! the two-base form: the rows from ne1 on land at obase2, the four sentinel words between the halves untouched
            let ne1 = ne / 2
            let obase2 = obase + ne1 + 4
            var want2 : array<float>
            want2 |> resize(out_n)
            for (e in range(ne)) {
                want2[(e < ne1 ? obase : obase2 - ne1) + e] = want[obase + e]
            }
            var got2 : array<float>
            got2 |> resize(out_n)
            nan_sentinel_upload(od, o_bytes)
            unsafe {
                var sc2 = set_router_gemv_f16_cls(fixed_array(wd, xd, od, xd), fixed_array(w_bytes, x_bytes, o_bytes, x_bytes), fixed_array(0u, 1u, 2u, 0u))
                var raw2 = alloc_cmd()
                let begin2 = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw2, begin2), null)
                var h2 : VkHaz
                var pc2 = RouterArgs(ne = uint(ne), dim = uint(dim), woff = uint(woff), obase = uint(obase), ne_split = uint(ne1), obase_split = uint(obase2))
                enc_router_gemv_f16_cls(raw2, h2, sc2, pc2, int64(ne))
                vhz_dep(raw2, h2, 2u, 0u, true)
                cmd_copy_range(raw2, od, 0l, host.buf, 0l, o_bytes)
                vk_check(vkEndCommandBuffer(raw2), null)
                submit_wait(raw2)
                memcpy(addr<void?>(got2[0]), host.mapped, o_bytes)
            }
            let bad2 = rel_mismatches(got2, want2, obase, ne1) + rel_mismatches(got2, want2, obase2, ne1)
            var gap = 0
            for (i in range(obase + ne1, obase2)) {
                if (float_bits_to_uint(got2[i]) != NAN_SENTINEL_BITS) {
                    gap++
                }
            }
            t |> success(bad2 == 0 && gap == 0, "router_gemv_f16 two bases: {bad2} of {ne} outputs off the CPU dot, {gap} of 4 gap words written")
            //! the norm-folded form: every expert row's dot over x under the norm row at nwoff, scaled by the row's weightless rms
            let nwoff = 64
            var wnh : array<float>
            wnh |> resize(nwoff + dim)
            for (i in range(length(wnh))) {
                wnh[i] = float(hash_word(uint(i) + 777u) % 1000u) / 1000.0 + 0.5
            }
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
            let wn_bytes = int64(length(wnh)) * 4l
            let wnd = make_device_buf(wn_bytes)
            var got3 : array<float>
            got3 |> resize(out_n)
            nan_sentinel_upload(od, o_bytes)
            unsafe {
                upload_region_at(wnd, 0l, addr<uint8 const?>(wnh[0]), wn_bytes)
                var sc3 = set_router_gemv_f16_cls(fixed_array(wd, xd, od, wnd), fixed_array(w_bytes, x_bytes, o_bytes, wn_bytes), fixed_array(0u, 1u, 2u, 4u))
                var raw3 = alloc_cmd()
                let begin3 = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw3, begin3), null)
                var h3 : VkHaz
                var pc3 = RouterArgs(ne = uint(ne), dim = uint(dim), woff = uint(woff), obase = uint(obase), norm_on = 1u, nwoff = uint(nwoff), eps = eps)
                enc_router_gemv_f16_cls(raw3, h3, sc3, pc3, int64(ne))
                vhz_dep(raw3, h3, 2u, 0u, true)
                cmd_copy_range(raw3, od, 0l, host.buf, 0l, o_bytes)
                vk_check(vkEndCommandBuffer(raw3), null)
                submit_wait(raw3)
                memcpy(addr<void?>(got3[0]), host.mapped, o_bytes)
            }
            let bad3 = rel_mismatches(got3, want3, obase, ne)
            t |> success(bad3 == 0, "router_gemv_f16 norm-folded: {bad3} of {ne} outputs off the CPU chain")
            t |> success(rel_mismatches(got3, want, obase, ne) > 0, "router_gemv_f16 norm-folded: the control - the unnormed dot must miss")
            delete wnh
            delete want3
            delete got3
            delete want2
            delete got2
            delete wh
            delete wh16
            delete xh
            delete want
            delete got
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

// the f16 grid a stored value lands on (the writer arms' oracle rounding)
def private q16(x : float) : float => float(float16(x))

[test]
def test_vkd_kv16_writers(t0 : T?) {
    t0 |> run("f16-mirror writer instances == the CPU oracle (rope_kv / rope_kv_b / qkn_rope)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!(ensure_rope_kv_f16_cls() && ensure_rope_kv_b_f16_cls() && ensure_qkn_rope_f16_cls())) {
                t |> skip("no Vulkan device")
                return
            }
            let nh = 4
            let nkvh = 2
            let hs = 64
            let half = hs / 2
            let qd = nh * hs
            let kvd = nkvh * hs
            let begin = VkCommandBufferBeginInfo()
            // --- rope_kv_f16: q roped f32 in place, k/v rows land f16 ---
            {
                let npairs = (nh + nkvh) * half
                let kpair0 = nh * half
                var qh : array<float>
                var kvh : array<float>
                var csh : array<float>
                qh |> resize(qd)
                kvh |> resize(qd + 2 * kvd)
                csh |> resize(hs)
                for (i in range(qd)) {
                    qh[i] = float((i * 23) % 71) * 0.09 - 3.0
                }
                for (i in range(qd + 2 * kvd)) {
                    kvh[i] = float((i * 19) % 83) * 0.07 - 2.9
                }
                for (i in range(half)) {
                    csh[i] = 0.3 + float(i % 9) * 0.07
                    csh[half + i] = 0.1 + float(i % 7) * 0.11
                }
                let q_bytes = int64(qd) * 4l
                let kv_bytes = int64(qd + 2 * kvd) * 4l
                let m_bytes = int64(kvd) * 2l   // f16 rows
                let cs_bytes = int64(hs) * 4l
                let kvd_dev = make_device_buf(kv_bytes)
                let csd = make_device_buf(cs_bytes)
                let q2 = make_device_buf(q_bytes)
                let km2 = make_device_buf(m_bytes)
                let vm2 = make_device_buf(m_bytes)
                var host = make_host_buf(q_bytes + 2l * m_bytes, true, [cached = true])
                var cls_q : array<float>
                var k16 : array<float16>
                var v16 : array<float16>
                cls_q |> resize(qd)
                k16 |> resize(kvd)
                v16 |> resize(kvd)
                let wgs = int64((npairs + 255) / 256)
                unsafe {
                    upload_region_at(kvd_dev, 0l, addr<uint8 const?>(kvh[0]), kv_bytes)
                    upload_region_at(csd, 0l, addr<uint8 const?>(csh[0]), cs_bytes)
                    upload_region_at(q2, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                    var tokb = make_host_buf(16l, true)
                    var tm = reinterpret<TokMeta?>(tokb.mapped)
                    tm.pos = 0u
                    tm.cnt = 0u
                    tm.mirbase = 0u
                    var sc = set_rope_kv_f16_cls(fixed_array(q2, kvd_dev, km2, vm2, csd, tokb.buf, csd),
                        fixed_array(q_bytes, kv_bytes, m_bytes, m_bytes, cs_bytes, 16l, cs_bytes),
                        fixed_array(16u, 1u, 2u, 4u, 8u, 0u, 0u))
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = RopeKvArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(half),
                        neox = 1u, kpair0 = uint(kpair0), npairs = uint(npairs), layerbase = 0u)
                    enc_rope_kv_f16_cls(raw2, h2, sc, pc, wgs)
                    vhz_dep(raw2, h2, 2u | 4u | 16u, 0u, true)
                    cmd_copy_whole(raw2, q2, host.buf, q_bytes)
                    cmd_copy_range(raw2, km2, 0l, host.buf, q_bytes, m_bytes)
                    cmd_copy_range(raw2, vm2, 0l, host.buf, q_bytes + m_bytes, m_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(cls_q[0]), host.mapped, q_bytes)
                    memcpy(addr<void?>(k16[0]), reinterpret<void?>(intptr(host.mapped) + uint64(q_bytes)), m_bytes)
                    memcpy(addr<void?>(v16[0]), reinterpret<void?>(intptr(host.mapped) + uint64(q_bytes + m_bytes)), m_bytes)
                }
                var cls_k <- unpack_f16(k16)
                var cls_v <- unpack_f16(v16)
                // oracle: the f32 rotation lands on the f16 grid (clamp is a no-op at these magnitudes)
                var ref_q : array<float>
                var ref_k : array<float>
                var ref_v : array<float>
                ref_q |> resize(qd)
                ref_k |> resize(kvd)
                ref_v |> resize(kvd)
                for (i in range(qd)) {
                    ref_q[i] = qh[i]
                }
                for (gid in range(npairs)) {
                    let is_k = gid >= kpair0
                    let lp = is_k ? gid - kpair0 : gid
                    let hh = lp / half
                    let j = lp % half
                    let fcr = csh[j]
                    let fci = csh[half + j]
                    if (is_k) {
                        let a0 = kvh[qd + hh * hs + j]
                        let a1 = kvh[qd + hh * hs + j + half]
                        ref_k[hh * hs + j] = q16(a0 * fcr - a1 * fci)
                        ref_k[hh * hs + j + half] = q16(a0 * fci + a1 * fcr)
                        ref_v[hh * hs + j] = q16(kvh[qd + kvd + hh * hs + j])
                        ref_v[hh * hs + j + half] = q16(kvh[qd + kvd + hh * hs + j + half])
                    } else {
                        let a0 = qh[hh * hs + j]
                        let a1 = qh[hh * hs + j + half]
                        ref_q[hh * hs + j] = a0 * fcr - a1 * fci
                        ref_q[hh * hs + j + half] = a0 * fci + a1 * fcr
                    }
                }
                let bad = mismatch_approx(cls_q, ref_q) + mismatch_approx(cls_k, ref_k) + mismatch_approx(cls_v, ref_v)
                t |> success(bad == 0, "rope_kv_f16 matches the CPU oracle ({bad} elements off)")
                delete qh
                delete kvh
                delete csh
                delete ref_q
                delete ref_k
                delete ref_v
                delete cls_q
                delete cls_k
                delete cls_v
                delete k16
                delete v16
            }
            // --- rope_kv_b_f16: batched positions into f16 mirrors at absolute rows ---
            {
                let neox = 1
                let npos = 5
                let pos0 = 7
                let layerbase = 256
                let voff = npos * kvd
                let ppp = (nh + nkvh) * half
                let kpair0 = nh * half
                let mirn = layerbase + (pos0 + npos) * kvd
                var q0h : array<float>
                var kvh : array<float>
                var csh : array<float>
                q0h |> resize(npos * qd)
                kvh |> resize(voff + npos * kvd)
                csh |> resize(npos * hs)
                for (i in range(npos * qd)) {
                    q0h[i] = float((i * 31) % 67) * 0.05 - 1.6
                }
                for (i in range(voff + npos * kvd)) {
                    kvh[i] = float((i * 23) % 71) * 0.04 - 1.5
                }
                for (i in range(npos * hs)) {
                    csh[i] = float((i * 13) % 41) * 0.045 - 0.9
                }
                let q_bytes = int64(npos * qd) * 4l
                let kv_bytes = int64(voff + npos * kvd) * 4l
                let cs_bytes = int64(npos * hs) * 4l
                let m_bytes = int64(mirn) * 2l   // f16 rows
                let qd2 = make_device_buf(q_bytes)
                let kvdv = make_device_buf(kv_bytes)
                let csd = make_device_buf(cs_bytes)
                let km2 = make_device_buf(m_bytes)
                let vm2 = make_device_buf(m_bytes)
                var host = make_host_buf(2l * m_bytes, true, [cached = true])
                var m16 : array<float16>
                m16 |> resize(2 * mirn)
                let wgs = (npos * ppp + 255) / 256
                unsafe {
                    upload_region_at(kvdv, 0l, addr<uint8 const?>(kvh[0]), kv_bytes)
                    upload_region_at(csd, 0l, addr<uint8 const?>(csh[0]), cs_bytes)
                    upload_region_at(qd2, 0l, addr<uint8 const?>(q0h[0]), q_bytes)
                    var sc = set_rope_kv_b_f16_cls(fixed_array(qd2, kvdv, km2, vm2, csd, csd),
                        fixed_array(q_bytes, kv_bytes, m_bytes, m_bytes, cs_bytes, cs_bytes),
                        fixed_array(1u, 16u, 2u, 4u, 8u, 0u))
                    var raw = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    var pc = RopeKvBArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs),
                        half = uint(half), neox = uint(neox), kpair0 = uint(kpair0),
                        ppp = uint(ppp), layerbase = uint(layerbase), npos = uint(npos),
                        pos0 = uint(pos0), voff = uint(voff), kstride = uint(kvd))
                    enc_rope_kv_b_f16_cls(raw, h, sc, pc, int64(wgs))
                    vhz_dep(raw, h, 8u, 0u, true)
                    cmd_copy_range(raw, km2, 0l, host.buf, 0l, m_bytes)
                    cmd_copy_range(raw, vm2, 0l, host.buf, m_bytes, m_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(m16[0]), host.mapped, 2l * m_bytes)
                }
                var m_cls <- unpack_f16(m16)
                var mk_ref : array<float>
                var mv_ref : array<float>
                mk_ref |> resize(mirn)
                mv_ref |> resize(mirn)
                for (p in range(npos)) {
                    for (lp in range(ppp)) {
                        let is_k = lp >= kpair0
                        if (!is_k) {
                            continue
                        }
                        let pr = lp - kpair0
                        let hh = pr / half
                        let j = pr % half
                        let fcr = csh[p * hs + j]
                        let fci = csh[p * hs + half + j]
                        let kb = p * kvd + hh * hs
                        let a0 = kvh[kb + j]
                        let a1 = kvh[kb + j + half]
                        let mo = layerbase + (pos0 + p) * kvd + hh * hs
                        mk_ref[mo + j] = q16(a0 * fcr - a1 * fci)
                        mk_ref[mo + j + half] = q16(a0 * fci + a1 * fcr)
                        mv_ref[mo + j] = q16(kvh[voff + kb + j])
                        mv_ref[mo + j + half] = q16(kvh[voff + kb + j + half])
                    }
                }
                var bad = 0
                let wb0 = layerbase + pos0 * kvd
                for (i in range(npos * kvd)) {
                    if (!approx(m_cls[wb0 + i], mk_ref[wb0 + i])) {
                        bad++
                    }
                    if (!approx(m_cls[mirn + wb0 + i], mv_ref[wb0 + i])) {
                        bad++
                    }
                }
                t |> success(bad == 0, "rope_kv_b_f16 matches the CPU oracle ({bad} off)")
                // the merged k|v projection layout (the DASLLAMA_VK_KV_MERGE default): [k | v] per position at
                // stride 2*kvd, v at +kvd - the same oracle rows must land; the mirrors take a sentinel first
                var kvm_h : array<float>
                kvm_h |> resize(npos * 2 * kvd)
                for (p in range(npos)) {
                    for (c in range(kvd)) {
                        kvm_h[p * 2 * kvd + c] = kvh[p * kvd + c]
                        kvm_h[p * 2 * kvd + kvd + c] = kvh[voff + p * kvd + c]
                    }
                }
                var msent : array<float16>
                msent |> resize(mirn)
                for (i in range(mirn)) {
                    msent[i] = float16(-999.25)
                }
                unsafe {
                    upload_region_at(qd2, 0l, addr<uint8 const?>(q0h[0]), q_bytes)
                    upload_region_at(kvdv, 0l, addr<uint8 const?>(kvm_h[0]), kv_bytes)
                    upload_region_at(km2, 0l, addr<uint8 const?>(msent[0]), m_bytes)
                    upload_region_at(vm2, 0l, addr<uint8 const?>(msent[0]), m_bytes)
                    var scm = set_rope_kv_b_f16_cls(fixed_array(qd2, kvdv, km2, vm2, csd, csd),
                        fixed_array(q_bytes, kv_bytes, m_bytes, m_bytes, cs_bytes, cs_bytes),
                        fixed_array(1u, 16u, 2u, 4u, 8u, 0u))
                    var raw = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    var pc = RopeKvBArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs),
                        half = uint(half), neox = uint(neox), kpair0 = uint(kpair0),
                        ppp = uint(ppp), layerbase = uint(layerbase), npos = uint(npos),
                        pos0 = uint(pos0), voff = uint(kvd), kstride = uint(2 * kvd))
                    enc_rope_kv_b_f16_cls(raw, h, scm, pc, int64(wgs))
                    vhz_dep(raw, h, 8u, 0u, true)
                    cmd_copy_range(raw, km2, 0l, host.buf, 0l, m_bytes)
                    cmd_copy_range(raw, vm2, 0l, host.buf, m_bytes, m_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(m16[0]), host.mapped, 2l * m_bytes)
                }
                var m_clsm <- unpack_f16(m16)
                var badm = 0
                for (i in range(npos * kvd)) {
                    if (!approx(m_clsm[wb0 + i], mk_ref[wb0 + i])) {
                        badm++
                    }
                    if (!approx(m_clsm[mirn + wb0 + i], mv_ref[wb0 + i])) {
                        badm++
                    }
                }
                t |> success(badm == 0, "rope_kv_b_f16 over merged k|v rows (kstride 2*kvd, voff kvd) matches the CPU oracle ({badm} off)")
                delete kvm_h
                delete msent
                delete m_clsm
                delete q0h
                delete kvh
                delete csh
                delete mk_ref
                delete mv_ref
                delete m_cls
                delete m16
            }
            // --- qkn_rope_f16: rms + rope fused, k/v rows land f16 ---
            {
                let neox = 1
                let krow = 192
                let eps = 0.00001
                let mirn2 = krow + nkvh * hs
                var q0h : array<float>
                var kvh : array<float>
                var csh : array<float>
                var wnh : array<float>
                q0h |> resize(qd)
                kvh |> resize(qd + 2 * kvd)
                csh |> resize(hs)
                wnh |> resize(2 * hs)
                for (i in range(qd)) {
                    q0h[i] = float((i * 31) % 67) * 0.05 - 1.6
                }
                for (i in range(qd + 2 * kvd)) {
                    kvh[i] = float((i * 23) % 71) * 0.04 - 1.5
                }
                for (i in range(hs)) {
                    csh[i] = float((i * 13) % 41) * 0.045 - 0.9
                }
                for (i in range(2 * hs)) {
                    wnh[i] = 0.75 + float(i % 11) * 0.05
                }
                let q_bytes = int64(qd) * 4l
                let kv_bytes = int64(qd + 2 * kvd) * 4l
                let cs_bytes = int64(hs) * 4l
                let wn_bytes = int64(2 * hs) * 4l
                let m_bytes = int64(mirn2) * 2l   // f16 rows
                let qd2 = make_device_buf(q_bytes)
                let kvdv = make_device_buf(kv_bytes)
                let csd = make_device_buf(cs_bytes)
                let wnd = make_device_buf(wn_bytes)
                let km2 = make_device_buf(m_bytes)
                let vm2 = make_device_buf(m_bytes)
                var host = make_host_buf(q_bytes + 2l * m_bytes, true, [cached = true])
                var q_cls : array<float>
                var m16 : array<float16>
                q_cls |> resize(qd)
                m16 |> resize(2 * mirn2)
                let wgs = nh + nkvh
                unsafe {
                    upload_region_at(kvdv, 0l, addr<uint8 const?>(kvh[0]), kv_bytes)
                    upload_region_at(csd, 0l, addr<uint8 const?>(csh[0]), cs_bytes)
                    upload_region_at(wnd, 0l, addr<uint8 const?>(wnh[0]), wn_bytes)
                    upload_region_at(qd2, 0l, addr<uint8 const?>(q0h[0]), q_bytes)
                    var tokb = make_host_buf(16l, true)
                    var tmq = reinterpret<TokMeta?>(tokb.mapped)
                    tmq.pos = 0u
                    tmq.cnt = 0u
                    tmq.mirbase = 0u
                    var sc = set_qkn_rope_f16_cls(fixed_array(qd2, kvdv, km2, vm2, csd, wnd, tokb.buf, csd),
                        fixed_array(q_bytes, kv_bytes, m_bytes, m_bytes, cs_bytes, wn_bytes, 16l, cs_bytes),
                        fixed_array(16u, 1u, 2u, 4u, 8u, 8u, 0u, 0u))
                    var raw = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw, begin), null)
                    var h : VkHaz
                    var pc = QknRopeArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs),
                        half = uint(half), neox = uint(neox), nh = uint(nh),
                        layerbase = uint(krow), qwoff = 0u,
                        kwoff = uint(hs), eps = eps)
                    enc_qkn_rope_f16_cls(raw, h, sc, pc, int64(wgs))
                    vhz_dep(raw, h, 8u, 0u, true)
                    cmd_copy_whole(raw, qd2, host.buf, q_bytes)
                    cmd_copy_range(raw, km2, 0l, host.buf, q_bytes, m_bytes)
                    cmd_copy_range(raw, vm2, 0l, host.buf, q_bytes + m_bytes, m_bytes)
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_wait(raw)
                    memcpy(addr<void?>(q_cls[0]), host.mapped, q_bytes)
                    memcpy(addr<void?>(m16[0]), reinterpret<void?>(intptr(host.mapped) + uint64(q_bytes)), 2l * m_bytes)
                }
                var m_cls <- unpack_f16(m16)
                var q_ref : array<float>
                var mk_ref : array<float>
                var mv_ref : array<float>
                q_ref |> resize(qd)
                mk_ref |> resize(mirn2)
                mv_ref |> resize(mirn2)
                for (wid in range(nh + nkvh)) {
                    let is_k = wid >= nh
                    let hh = is_k ? wid - nh : wid
                    let sbase = is_k ? qd + hh * hs : hh * hs
                    var ss = 0.0
                    for (i in range(hs)) {
                        let v = is_k ? kvh[sbase + i] : q0h[sbase + i]
                        ss += v * v
                    }
                    let inv = 1.0 / sqrt(ss / float(hs) + eps)
                    var nrow : array<float>
                    nrow |> resize(hs)
                    for (i in range(hs)) {
                        let v = is_k ? kvh[sbase + i] : q0h[sbase + i]
                        nrow[i] = wnh[(is_k ? hs : 0) + i] * (v * inv)
                    }
                    for (j in range(half)) {
                        let fcr = csh[j]
                        let fci = csh[half + j]
                        let r0 = nrow[j] * fcr - nrow[j + half] * fci
                        let r1 = nrow[j] * fci + nrow[j + half] * fcr
                        if (is_k) {
                            mk_ref[krow + hh * hs + j] = q16(r0)
                            mk_ref[krow + hh * hs + j + half] = q16(r1)
                        } else {
                            q_ref[hh * hs + j] = r0
                            q_ref[hh * hs + j + half] = r1
                        }
                    }
                    if (is_k) {
                        for (i in range(hs)) {
                            mv_ref[krow + hh * hs + i] = q16(kvh[qd + kvd + hh * hs + i])
                        }
                    }
                    delete nrow
                }
                var bad = mismatch_approx(q_cls, q_ref)
                for (i in range(nkvh * hs)) {
                    if (!approx(m_cls[krow + i], mk_ref[krow + i])) {
                        bad++
                    }
                    if (!approx(m_cls[mirn2 + krow + i], mv_ref[krow + i])) {
                        bad++
                    }
                }
                t |> success(bad == 0, "qkn_rope_f16 matches the CPU oracle ({bad} off)")
                delete q0h
                delete kvh
                delete csh
                delete wnh
                delete q_ref
                delete mk_ref
                delete mv_ref
                delete q_cls
                delete m_cls
                delete m16
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
            if (!(ensure_da_attn_f16_cls() && ensure_da_attn_b_f16_cls() && ensure_da_attn_f16_g2_cls())) {
                t |> skip("f16 attention classes declined on this device")
                return
            }
            let begin = VkCommandBufferBeginInfo()
            // --- da_attn_f16: decode row over f16 mirrors (chunk boundary) - the scalar head (64), the one-word lane (256) and the two-word lane (512) ---
            for (hs in [64, 256, 512]) {
                let kv_mul = 2
                let nheads = 4
                let kvd = (nheads / kv_mul) * hs
                let cnt = 300
                let scale = 0.125
                var qh : array<float>
                var kh : array<float>
                var vh : array<float>
                qh |> resize(nheads * hs)
                kh |> resize(cnt * kvd)
                vh |> resize(cnt * kvd)
                for (i in range(nheads * hs)) {
                    qh[i] = float((i * 41) % 73) * 0.05 - 1.8
                }
                for (i in range(cnt * kvd)) {
                    kh[i] = q16(float((i * 17) % 97) * 0.03 - 1.4)
                    vh[i] = q16(float((i * 53) % 89) * 0.04 - 1.7)
                }
                var k16 <- pack_f16(kh)
                var v16 <- pack_f16(vh)
                let q_bytes = int64(nheads * hs) * 4l
                let m_bytes = int64(cnt * kvd) * 2l
                let qdv = make_device_buf(q_bytes)
                let kd = make_device_buf(m_bytes)
                let vd = make_device_buf(m_bytes)
                let yd2 = make_device_buf(q_bytes)
                var host = make_host_buf(q_bytes, true, [cached = true])
                var y_cls : array<float>
                y_cls |> resize(nheads * hs)
                unsafe {
                    upload_region_at(qdv, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                    upload_region_at(kd, 0l, addr<uint8 const?>(k16[0]), m_bytes)
                    upload_region_at(vd, 0l, addr<uint8 const?>(v16[0]), m_bytes)
                    var tokb = make_host_buf(16l, true)
                    var tmc = reinterpret<TokMeta?>(tokb.mapped)
                    tmc.cnt = uint(cnt)
                    tmc.mirbase = 0u
                    var sc = set_da_attn_f16_cls(fixed_array(qdv, kd, vd, yd2, tokb.buf, yd2, yd2, yd2),
                        fixed_array(q_bytes, m_bytes, m_bytes, q_bytes, 16l, q_bytes, q_bytes, q_bytes),
                        fixed_array(1u, 2u, 4u, 16u, 0u, 16u, 16u, 16u))
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = DaAttnArgs(kvd = uint(kvd), hs = uint(hs),
                        kv_mul = uint(kv_mul), kbase = 0u, vbase = 0u, scale = scale, rqk = DA_RQ_NONE)
                    enc_da_attn_f16_cls(raw2, h2, sc, pc, da_attn_row_wgs(int64(nheads), int64(kv_mul), 1l))
                    vhz_dep(raw2, h2, 16u, 0u, true)
                    cmd_copy_whole(raw2, yd2, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
                }
                // oracle over the SAME f16-grid values the device loads — plain f32 attention
                var y_ref : array<float>
                y_ref |> resize(nheads * hs)
                for (hh in range(nheads)) {
                    attn_row_oracle(qh, hh * hs, kh, 0, vh, 0, kvd, hh, hs, kv_mul, cnt, scale, y_ref, hh * hs)
                }
                let bad = mismatch_approx(y_cls, y_ref)
                t |> success(bad == 0, "da_attn_f16 at head {hs} matches the CPU oracle ({bad} of {nheads * hs} off)")
                //! the two-head slab's f16 stamp (the one a two-head group serves on the f16 mirror) at the same shape: the oracle, and the four-head stamp bit for bit
                var y_g2 : array<float>
                y_g2 |> resize(nheads * hs)
                unsafe {
                    var tokb2 = make_host_buf(16l, true)
                    var tmc2 = reinterpret<TokMeta?>(tokb2.mapped)
                    tmc2.cnt = uint(cnt)
                    tmc2.mirbase = 0u
                    var sc2 = set_da_attn_f16_cls(fixed_array(qdv, kd, vd, yd2, tokb2.buf, yd2, yd2, yd2),
                        fixed_array(q_bytes, m_bytes, m_bytes, q_bytes, 16l, q_bytes, q_bytes, q_bytes),
                        fixed_array(1u, 2u, 4u, 16u, 0u, 16u, 16u, 16u))
                    var raw3 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw3, begin), null)
                    var h3 : VkHaz
                    var pc3 = DaAttnArgs(kvd = uint(kvd), hs = uint(hs),
                        kv_mul = uint(kv_mul), kbase = 0u, vbase = 0u, scale = scale, rqk = DA_RQ_NONE)
                    enc_da_attn_f16_g2_cls(raw3, h3, sc2, pc3, da_attn_row_wgs(int64(nheads), int64(kv_mul), 1l))
                    vhz_dep(raw3, h3, 16u, 0u, true)
                    cmd_copy_whole(raw3, yd2, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw3), null)
                    submit_wait(raw3)
                    memcpy(addr<void?>(y_g2[0]), host.mapped, q_bytes)
                }
                let bad_g2 = mismatch_approx(y_g2, y_ref)
                t |> success(bad_g2 == 0, "da_attn_f16_g2 at head {hs} matches the CPU oracle ({bad_g2} of {nheads * hs} off)")
                var apart = 0
                for (i in range(nheads * hs)) {
                    if (y_g2[i] != y_cls[i]) {
                        apart++
                    }
                }
                t |> success(apart == 0, "da_attn_f16_g2 at head {hs} is da_attn_f16 bit for bit ({apart} of {nheads * hs} apart)")
                y_ref[hs / 2] += 0.5
                t |> equal(mismatch_approx(y_g2, y_ref), 1, "da_attn_f16_g2 at head {hs}: the bar reds one poisoned element")
                y_ref[hs / 2] -= 0.5
                delete y_g2
                delete qh
                delete kh
                delete vh
                delete y_ref
                delete y_cls
                delete k16
                delete v16
            }
            // --- da_attn_b_f16: batched window over f16 mirrors (dpl 1 and 8 arms) ---
            for (hs in [32, 256]) {
                let nheads = 4
                let kv_mul = 2
                let npos = 13
                let w0 = 32
                let kbase = 64
                let vbase = 160
                let scale = 0.125
                let qd = nheads * hs
                let kvd = (nheads / kv_mul) * hs
                let kpos = w0 + npos
                var qh : array<float>
                var kh : array<float>
                var vh : array<float>
                qh |> resize(npos * qd)
                kh |> resize(kbase + kpos * kvd)
                vh |> resize(vbase + kpos * kvd)
                for (i in range(npos * qd)) {
                    qh[i] = float((i * 41) % 73) * 0.05 - 1.8
                }
                for (i in range(kbase + kpos * kvd)) {
                    kh[i] = q16(float((i * 17) % 97) * 0.03 - 1.4)
                }
                for (i in range(vbase + kpos * kvd)) {
                    vh[i] = q16(float((i * 53) % 89) * 0.04 - 1.7)
                }
                var k16 <- pack_f16(kh)
                var v16 <- pack_f16(vh)
                let q_bytes = int64(npos * qd) * 4l
                let k_bytes = int64(kbase + kpos * kvd) * 2l
                let v_bytes = int64(vbase + kpos * kvd) * 2l
                let qdev = make_device_buf(q_bytes)
                let kdev = make_device_buf(k_bytes)
                let vdev = make_device_buf(v_bytes)
                let yd2 = make_device_buf(q_bytes)
                var host = make_host_buf(q_bytes, true, [cached = true])
                var y_cls : array<float>
                y_cls |> resize(npos * qd)
                let wgs = nheads * ((npos + 7) / 8)
                unsafe {
                    upload_region_at(qdev, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                    upload_region_at(kdev, 0l, addr<uint8 const?>(k16[0]), k_bytes)
                    upload_region_at(vdev, 0l, addr<uint8 const?>(v16[0]), v_bytes)
                    var sc = set_da_attn_b_f16_cls(fixed_array(qdev, kdev, vdev, yd2),
                        fixed_array(q_bytes, k_bytes, v_bytes, q_bytes),
                        fixed_array(1u, 2u, 8u, 16u))
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = DaAttnBArgs(npos = uint(npos), nh = uint(nheads), kvd = uint(kvd),
                        hs = uint(hs), kv_mul = uint(kv_mul), kbase = uint(kbase),
                        vbase = uint(vbase), w0 = uint(w0), scale = scale)
                    enc_da_attn_b_f16_cls(raw2, h2, sc, pc, int64(wgs))
                    vhz_dep(raw2, h2, 16u, 0u, true)
                    cmd_copy_whole(raw2, yd2, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
                }
                var y_ref : array<float>
                y_ref |> resize(npos * qd)
                for (r in range(npos)) {
                    for (hh in range(nheads)) {
                        attn_row_oracle(qh, r * qd + hh * hs, kh, kbase, vh, vbase, kvd, hh, hs,
                            kv_mul, w0 + r + 1, scale, y_ref, r * qd + hh * hs)
                    }
                }
                let bad = mismatch_approx(y_cls, y_ref)
                t |> success(bad == 0, "da_attn_b_f16 hs {hs} matches the CPU oracle ({bad} of {npos * qd} off)")
                delete qh
                delete kh
                delete vh
                delete y_ref
                delete y_cls
                delete k16
                delete v16
            }
            // --- da_attn_b_h128_f16: the coopmat twin over f16 mirrors ---
            if (g_gpu.has_coopmat) {
                verify(ensure_da_attn_b_h128_f16_cls(), "the h128 f16 class rail must engage on a coopmat device")
                let hs = 128
                let nheads = 4
                let kv_mul = 2
                let npos = 45
                let w0 = 32
                let kbase = 64
                let vbase = 160
                let scale = 0.125
                let qd = nheads * hs
                let kvd = (nheads / kv_mul) * hs
                let kpos = w0 + npos
                var qh : array<float>
                var kh : array<float>
                var vh : array<float>
                qh |> resize(npos * qd)
                kh |> resize(kbase + kpos * kvd)
                vh |> resize(vbase + kpos * kvd)
                for (i in range(npos * qd)) {
                    qh[i] = float((i * 41) % 73) * 0.05 - 1.8
                }
                for (i in range(kbase + kpos * kvd)) {
                    kh[i] = q16(float((i * 17) % 97) * 0.03 - 1.4)
                }
                for (i in range(vbase + kpos * kvd)) {
                    vh[i] = q16(float((i * 53) % 89) * 0.04 - 1.7)
                }
                var k16 <- pack_f16(kh)
                var v16 <- pack_f16(vh)
                let q_bytes = int64(npos * qd) * 4l
                let k_bytes = int64(kbase + kpos * kvd) * 2l
                let v_bytes = int64(vbase + kpos * kvd) * 2l
                let qdev = make_device_buf(q_bytes)
                let kdev = make_device_buf(k_bytes)
                let vdev = make_device_buf(v_bytes)
                let yd2 = make_device_buf(q_bytes)
                var host = make_host_buf(q_bytes, true, [cached = true])
                var y_cls : array<float>
                y_cls |> resize(npos * qd)
                let wgs = nheads * ((npos + 31) / 32)
                unsafe {
                    upload_region_at(qdev, 0l, addr<uint8 const?>(qh[0]), q_bytes)
                    upload_region_at(kdev, 0l, addr<uint8 const?>(k16[0]), k_bytes)
                    upload_region_at(vdev, 0l, addr<uint8 const?>(v16[0]), v_bytes)
                    var sc = set_da_attn_b_h128_f16_cls(fixed_array(qdev, kdev, vdev, yd2),
                        fixed_array(q_bytes, k_bytes, v_bytes, q_bytes),
                        fixed_array(1u, 2u, 8u, 16u))
                    var raw2 = alloc_cmd()
                    vk_check(vkBeginCommandBuffer(raw2, begin), null)
                    var h2 : VkHaz
                    var pc = DaAttnBArgs(npos = uint(npos), nh = uint(nheads), kvd = uint(kvd),
                        hs = uint(hs), kv_mul = uint(kv_mul), kbase = uint(kbase),
                        vbase = uint(vbase), w0 = uint(w0), scale = scale)
                    enc_da_attn_b_h128_f16_cls(raw2, h2, sc, pc, int64(wgs))
                    vhz_dep(raw2, h2, 16u, 0u, true)
                    cmd_copy_whole(raw2, yd2, host.buf, q_bytes)
                    vk_check(vkEndCommandBuffer(raw2), null)
                    submit_wait(raw2)
                    memcpy(addr<void?>(y_cls[0]), host.mapped, q_bytes)
                }
                var y_ref : array<float>
                y_ref |> resize(npos * qd)
                for (r in range(npos)) {
                    for (hh in range(nheads)) {
                        attn_row_oracle(qh, r * qd + hh * hs, kh, kbase, vh, vbase, kvd, hh, hs,
                            kv_mul, w0 + r + 1, scale, y_ref, r * qd + hh * hs)
                    }
                }
                let bad = mismatch_bars(y_cls, y_ref, 2e-2, 4e-3 * max_abs(y_ref))
                t |> success(bad == 0, "da_attn_b_h128_f16 matches the CPU oracle ({bad} of {npos * qd} off)")
                delete qh
                delete kh
                delete vh
                delete y_ref
                delete y_cls
                delete k16
                delete v16
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
            var wqh : array<uint>
            var wsh16 : array<float16>
            var idsh : array<uint>
            wqh |> resize(totblk * 8)
            wsh16 |> resize(totblk)
            idsh |> resize(npos)
            for (i in range(totblk * 8)) {
                wqh[i] = hash_word(uint(i) + 5u)
            }
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
            let wq_bytes = int64(totblk) * 32l
            let ws_bytes = int64(totblk) * 2l
            let ids_bytes = int64(npos) * 4l
            let x_bytes = int64(npos * dim) * 4l
            let wf_bytes = int64(vocab * dim) * 4l
            let wqd = make_device_buf(wq_bytes)
            let wsd = make_device_buf(ws_bytes)
            let idd = make_device_buf(ids_bytes)
            let xd = make_device_buf(x_bytes)
            let wfd = make_device_buf(wf_bytes)
            var host = make_host_buf(x_bytes * 2l, true, [cached = true])
            var y_cls : array<float>
            var yf_cls : array<float>
            y_cls |> resize(npos * dim)
            yf_cls |> resize(npos * dim)
            unsafe {
                upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), wq_bytes)
                upload_region_at(wsd, 0l, addr<uint8 const?>(wsh16[0]), ws_bytes)
                upload_region_at(idd, 0l, addr<uint8 const?>(idsh[0]), ids_bytes)
                upload_region_at(wfd, 0l, addr<uint8 const?>(wfh[0]), wf_bytes)
                var scq = set_emb_gather_cls(fixed_array(wqd, wsd, idd, xd),
                    fixed_array(wq_bytes, ws_bytes, ids_bytes, x_bytes),
                    fixed_array(1u, 2u, 4u, 8u))
                var scf = set_emb_gather_f32_cls(fixed_array(wfd, idd, xd),
                    fixed_array(wf_bytes, ids_bytes, x_bytes),
                    fixed_array(1u, 4u, 8u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                var pc = EmbArgs(npos = uint(npos), dim = uint(dim), wblk0 = uint(wblk0), embed_scale = scale)
                enc_emb_gather_cls(raw, h, scq, pc, int64((npos * dim / 4 + 255) / 256))
                vhz_dep(raw, h, 8u, 0u, true)
                cmd_copy_range(raw, xd, 0l, host.buf, 0l, x_bytes)
                enc_emb_gather_f32_cls(raw, h, scf, pc, int64((npos * dim / 4 + 255) / 256))
                vhz_dep(raw, h, 8u, 0u, true)
                cmd_copy_range(raw, xd, 0l, host.buf, x_bytes, x_bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                memcpy(addr<void?>(y_cls[0]), host.mapped, x_bytes)
                memcpy(addr<void?>(yf_cls[0]), reinterpret<void?>(intptr(host.mapped) + uint64(x_bytes)), x_bytes)
            }
            var y_ref : array<float>
            var yf_ref : array<float>
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
def test_vkd_ar_rq_fused(t0 : T?) {
    t0 |> run("fused batch ar+rq == the split cls_ar + cls_dn_rq pair, bit-exact") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_cls_ar() || !ensure_cls_dn_rq() || !ensure_cls_ar_rq_b()) {
                t |> skip("no Vulkan device")
                return
            }
            let dim = 128
            let nrows = 6
            let woff = 64
            let nblk = nrows * (dim / 32)
            var xh : array<float>
            var aah : array<float>
            var wnh : array<float>
            xh |> resize(nrows * dim)
            aah |> resize(nrows * dim)
            wnh |> resize(woff + dim)
            for (i in range(nrows * dim)) {
                xh[i] = 0.375 + float(i % 13) * 0.0625 - float(i % 5) * 0.125
                aah[i] = float(i % 7) * 0.25 - 0.5
            }
            for (i in range(woff + dim)) {
                wnh[i] = 0.75 + float(i % 9) * 0.03125
            }
            let row_bytes = int64(nrows * dim) * 4l
            let wn_bytes = int64(woff + dim) * 4l
            let q_bytes = int64(nblk) * 32l
            let s_bytes = int64(nblk) * 4l
            let x_split = make_device_buf(row_bytes)
            let x_fused = make_device_buf(row_bytes)
            let aad = make_device_buf(row_bytes)
            let wnd = make_device_buf(wn_bytes)
            let yod = make_device_buf(row_bytes)
            let q_split = make_device_buf(q_bytes)
            let s_split = make_device_buf(s_bytes)
            let q_fused = make_device_buf(q_bytes)
            let s_fused = make_device_buf(s_bytes)
            // two DIFFERENT sentinels on the compared planes: a dead leg on either side reads as a mismatch
            var sent_split : array<float>
            var sent_fused : array<float>
            sent_split |> resize(nrows * dim)
            sent_fused |> resize(nrows * dim)
            for (i in range(nrows * dim)) {
                sent_split[i] = -999.25
                sent_fused[i] = -777.5
            }
            var host = make_host_buf((q_bytes + s_bytes) * 2l + row_bytes * 2l, true, [cached = true])
            unsafe {
                upload_region_at(x_split, 0l, addr<uint8 const?>(xh[0]), row_bytes)
                upload_region_at(x_fused, 0l, addr<uint8 const?>(xh[0]), row_bytes)
                upload_region_at(aad, 0l, addr<uint8 const?>(aah[0]), row_bytes)
                upload_region_at(wnd, 0l, addr<uint8 const?>(wnh[0]), wn_bytes)
                upload_region_at(q_split, 0l, addr<uint8 const?>(sent_split[0]), q_bytes)
                upload_region_at(s_split, 0l, addr<uint8 const?>(sent_split[0]), s_bytes)
                upload_region_at(q_fused, 0l, addr<uint8 const?>(sent_fused[0]), q_bytes)
                upload_region_at(s_fused, 0l, addr<uint8 const?>(sent_fused[0]), s_bytes)
                var sca = set_cls_ar(fixed_array(x_split, aad, wnd, yod),
                    fixed_array(row_bytes, row_bytes, wn_bytes, row_bytes),
                    fixed_array(1u, 2u, 0u, 4u))
                var scr = set_rq_cls(fixed_array(yod, q_split, s_split),
                    fixed_array(row_bytes, q_bytes, s_bytes),
                    fixed_array(4u, 8u, 8u))
                var scf = set_cls_ar_rq_b(fixed_array(x_fused, aad, wnd, q_fused, s_fused),
                    fixed_array(row_bytes, row_bytes, wn_bytes, q_bytes, s_bytes),
                    fixed_array(16u, 2u, 0u, 32u, 32u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                var pca = ArArgs(dim = uint(dim), add_on = 1u, woff = uint(woff), eps = 1e-5, ascale = 1.0)
                enc_cls_ar(raw, h, sca, pca, int64(nrows))
                var pcr = RqArgs(inbase = 0u, nblk = uint(nblk))
                enc_cls_dn_rq(raw, h, scr, pcr, int64((nrows * dim / 4 + 255) / 256))
                enc_cls_ar_rq_b(raw, h, scf, pca, int64(nrows))
                vhz_dep(raw, h, 8u | 32u | 16u | 1u, 0u, true)
                cmd_copy_range(raw, q_split, 0l, host.buf, 0l, q_bytes)
                cmd_copy_range(raw, s_split, 0l, host.buf, q_bytes, s_bytes)
                cmd_copy_range(raw, q_fused, 0l, host.buf, q_bytes + s_bytes, q_bytes)
                cmd_copy_range(raw, s_fused, 0l, host.buf, q_bytes * 2l + s_bytes, s_bytes)
                cmd_copy_range(raw, x_split, 0l, host.buf, (q_bytes + s_bytes) * 2l, row_bytes)
                cmd_copy_range(raw, x_fused, 0l, host.buf, (q_bytes + s_bytes) * 2l + row_bytes, row_bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                var q_split_h : array<uint>
                var q_fused_h : array<uint>
                var s_split_h : array<float>
                var s_fused_h : array<float>
                var x_split_h : array<float>
                var x_fused_h : array<float>
                q_split_h |> resize(nblk * 8)
                q_fused_h |> resize(nblk * 8)
                s_split_h |> resize(nblk)
                s_fused_h |> resize(nblk)
                x_split_h |> resize(nrows * dim)
                x_fused_h |> resize(nrows * dim)
                let hp = intptr(host.mapped)
                memcpy(addr<void?>(q_split_h[0]), host.mapped, q_bytes)
                memcpy(addr<void?>(s_split_h[0]), reinterpret<void?>(hp + uint64(q_bytes)), s_bytes)
                memcpy(addr<void?>(q_fused_h[0]), reinterpret<void?>(hp + uint64(q_bytes + s_bytes)), q_bytes)
                memcpy(addr<void?>(s_fused_h[0]), reinterpret<void?>(hp + uint64(q_bytes * 2l + s_bytes)), s_bytes)
                memcpy(addr<void?>(x_split_h[0]), reinterpret<void?>(hp + uint64((q_bytes + s_bytes) * 2l)), row_bytes)
                memcpy(addr<void?>(x_fused_h[0]), reinterpret<void?>(hp + uint64((q_bytes + s_bytes) * 2l + row_bytes)), row_bytes)
                let badq = mismatch_qbytes(q_split_h, q_fused_h, 0)
                t |> success(badq == 0, "fused quant words == split ({badq} of {nblk * 8} off)")
                let bads = mismatch_exact(s_split_h, s_fused_h)
                t |> success(bads == 0, "fused scales == split ({bads} of {nblk} off)")
                // the residual plane is IN-PLACE, so it cannot hold a sentinel - the liveness pair is its witness
                t |> success(x_split_h[0] != xh[0], "the split ar wrote the residual stream")
                t |> success(x_fused_h[0] != xh[0], "the fused ar wrote the residual stream")
                let badx = mismatch_exact(x_split_h, x_fused_h)
                t |> success(badx == 0, "fused residual update == split ({badx} of {nrows * dim} off)")
                delete q_split_h
                delete q_fused_h
                delete s_split_h
                delete s_fused_h
                delete x_split_h
                delete x_fused_h
            }
            delete sent_split
            delete sent_fused
            delete xh
            delete aah
            delete wnh
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
    t0 |> run("fused batch ar+f16 == the split cls_ar + f16cvt_cls pair, bit-exact") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            if (!ensure_cls_ar() || !ensure_f16cvt_cls() || !ensure_cls_ar_f16_b()) {
                t |> skip("no Vulkan device")
                return
            }
            let dim = 128
            let nrows = 6
            let woff = 64
            var xh : array<float>
            var aah : array<float>
            var wnh : array<float>
            xh |> resize(nrows * dim)
            aah |> resize(nrows * dim)
            wnh |> resize(woff + dim)
            for (i in range(nrows * dim)) {
                xh[i] = 0.375 + float(i % 13) * 0.0625 - float(i % 5) * 0.125
                aah[i] = float(i % 7) * 0.25 - 0.5
            }
            for (i in range(woff + dim)) {
                wnh[i] = 0.75 + float(i % 9) * 0.03125
            }
            let row_bytes = int64(nrows * dim) * 4l
            let wn_bytes = int64(woff + dim) * 4l
            let h_bytes = int64(nrows * dim) * 2l
            let x_split = make_device_buf(row_bytes)
            let x_fused = make_device_buf(row_bytes)
            let aad = make_device_buf(row_bytes)
            let wnd = make_device_buf(wn_bytes)
            let yod = make_device_buf(row_bytes)
            let f16_split = make_device_buf(h_bytes)
            let f16_fused = make_device_buf(h_bytes)
            // two DIFFERENT sentinels on the compared planes: a dead leg on either side reads as a mismatch
            var sent_split : array<float16>
            var sent_fused : array<float16>
            sent_split |> resize(nrows * dim)
            sent_fused |> resize(nrows * dim)
            for (i in range(nrows * dim)) {
                sent_split[i] = float16(-999.25)
                sent_fused[i] = float16(-777.5)
            }
            var host = make_host_buf(h_bytes * 2l + row_bytes * 2l, true, [cached = true])
            unsafe {
                upload_region_at(x_split, 0l, addr<uint8 const?>(xh[0]), row_bytes)
                upload_region_at(x_fused, 0l, addr<uint8 const?>(xh[0]), row_bytes)
                upload_region_at(aad, 0l, addr<uint8 const?>(aah[0]), row_bytes)
                upload_region_at(wnd, 0l, addr<uint8 const?>(wnh[0]), wn_bytes)
                upload_region_at(f16_split, 0l, addr<uint8 const?>(sent_split[0]), h_bytes)
                upload_region_at(f16_fused, 0l, addr<uint8 const?>(sent_fused[0]), h_bytes)
                var sca = set_cls_ar(fixed_array(x_split, aad, wnd, yod),
                    fixed_array(row_bytes, row_bytes, wn_bytes, row_bytes),
                    fixed_array(1u, 2u, 0u, 4u))
                var scc = set_f16cvt_cls(fixed_array(yod, f16_split),
                    fixed_array(row_bytes, h_bytes), fixed_array(4u, 8u))
                var scf = set_cls_ar_f16_b(fixed_array(x_fused, aad, wnd, f16_fused),
                    fixed_array(row_bytes, row_bytes, wn_bytes, h_bytes),
                    fixed_array(16u, 2u, 0u, 32u))
                var raw = alloc_cmd()
                let begin = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(raw, begin), null)
                var h : VkHaz
                var pca = ArArgs(dim = uint(dim), add_on = 1u, woff = uint(woff), eps = 1e-5, ascale = 1.0)
                enc_cls_ar(raw, h, sca, pca, int64(nrows))
                var pcc = ActArgs(nelem = uint(nrows * dim), nblk = 0u)
                enc_f16cvt_cls(raw, h, scc, pcc, int64((nrows * dim / 4 + 255) / 256))
                enc_cls_ar_f16_b(raw, h, scf, pca, int64(nrows))
                vhz_dep(raw, h, 8u | 32u | 16u | 1u, 0u, true)
                cmd_copy_range(raw, f16_split, 0l, host.buf, 0l, h_bytes)
                cmd_copy_range(raw, f16_fused, 0l, host.buf, h_bytes, h_bytes)
                cmd_copy_range(raw, x_split, 0l, host.buf, h_bytes * 2l, row_bytes)
                cmd_copy_range(raw, x_fused, 0l, host.buf, h_bytes * 2l + row_bytes, row_bytes)
                vk_check(vkEndCommandBuffer(raw), null)
                submit_wait(raw)
                let nw = nrows * dim / 2
                var f16_split_h : array<uint>
                var f16_fused_h : array<uint>
                var x_split_h : array<float>
                var x_fused_h : array<float>
                f16_split_h |> resize(nw)
                f16_fused_h |> resize(nw)
                x_split_h |> resize(nrows * dim)
                x_fused_h |> resize(nrows * dim)
                let hp = intptr(host.mapped)
                memcpy(addr<void?>(f16_split_h[0]), host.mapped, h_bytes)
                memcpy(addr<void?>(f16_fused_h[0]), reinterpret<void?>(hp + uint64(h_bytes)), h_bytes)
                memcpy(addr<void?>(x_split_h[0]), reinterpret<void?>(hp + uint64(h_bytes * 2l)), row_bytes)
                memcpy(addr<void?>(x_fused_h[0]), reinterpret<void?>(hp + uint64(h_bytes * 2l + row_bytes)), row_bytes)
                let badh = mismatch_qbytes(f16_split_h, f16_fused_h, 0)
                t |> success(badh == 0, "fused f16 rows == split ({badh} of {nw} words off)")
                // the residual plane is IN-PLACE, so it cannot hold a sentinel - the liveness pair is its witness
                t |> success(x_split_h[0] != xh[0], "the split ar wrote the residual stream")
                t |> success(x_fused_h[0] != xh[0], "the fused ar wrote the residual stream")
                let badx = mismatch_exact(x_split_h, x_fused_h)
                t |> success(badx == 0, "fused residual update == split ({badx} of {nrows * dim} off)")
                delete f16_split_h
                delete f16_fused_h
                delete x_split_h
                delete x_fused_h
            }
            delete sent_split
            delete sent_fused
            delete xh
            delete aah
            delete wnh
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

//! the device: the recorded dispatches over `raw`, then `bytes` of `dev` back into `host`
def private vkd_run_copy(var raw; var h; dev : uint64; host; bytes : int64; bits : uint) {
    vhz_dep(raw, h, bits, 0u, true)
    cmd_copy_whole(raw, dev, host.buf, bytes)
    vk_check(vkEndCommandBuffer(raw), null)
    submit_wait(raw)
}

//! the router GEMM: two layers' [ner x dim] rows, the second layer's dispatched through woff; logits at the ner
//! stride; `ner` past a half-tile exercises both rows of an invocation, under it the second row's guard alone
def private moe_router_arm(t : T?; ner : int) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let nw = 2 * ner * MR_DIM
        var wh : array<float>
        var xh : array<float>
        wh |> resize(nw)
        xh |> resize(MR_NPOS * MR_DIM)
        for (i in range(nw)) {
            wh[i] = hash_unit(uint(i) + 11u) * 0.25
        }
        for (i in range(MR_NPOS * MR_DIM)) {
            xh[i] = hash_unit(uint(i) + 7_777u)
        }
        let w_bytes = int64(nw) * 4l
        let x_bytes = int64(MR_NPOS * MR_DIM) * 4l
        let l_bytes = int64(MR_NPOS * ner) * 4l
        let wd = make_device_buf(w_bytes)
        let xd = make_device_buf(x_bytes)
        let ld = make_device_buf(l_bytes)
        var host = make_host_buf(l_bytes, true, [cached = true])
        var got : array<float>
        got |> resize(MR_NPOS * ner)
        unsafe {
            upload_region_at(wd, 0l, addr<uint8 const?>(wh[0]), w_bytes)
            upload_region_at(xd, 0l, addr<uint8 const?>(xh[0]), x_bytes)
            nan_sentinel_upload(ld, l_bytes)
            var sc = set_router_gemm_cls(fixed_array(wd, xd, ld), fixed_array(w_bytes, x_bytes, l_bytes), fixed_array(0u, 1u, 2u))
            var raw = alloc_cmd()
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
            var h : VkHaz
            var pc = RouterGemmArgs(npos = uint(MR_NPOS), ne = uint(ner), dim = uint(MR_DIM), woff = uint(ner * MR_DIM), ostride = uint(ner))
            enc_router_gemm_cls(raw, h, sc, pc, int64(((MR_NPOS + int(ROUTER_TP) - 1) / int(ROUTER_TP)) * ((ner + int(ROUTER_TE) - 1) / int(ROUTER_TE))))
            vkd_run_copy(raw, h, ld, host, l_bytes, 2u)
            memcpy(addr<void?>(got[0]), host.mapped, l_bytes)
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
        let bad = mismatch_rel(got, want, 1e-4)
        t |> success(bad == 0, "router GEMM over {ner} rows matches the CPU oracle at the second layer's offset ({bad} of {MR_NPOS * ner} off)")
        want[5 * ner + 3] += 0.5
        t |> success(mismatch_rel(got, want, 1e-4) == 1, "router GEMM control ({ner} rows): the bar reds one poisoned element")
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
        var lh : array<float>
        var rb : array<float>
        lh |> resize(MR_NPOS * MR_NER)
        rb |> resize(2 * MR_NE)   //! two layers' bias rows: the dispatch reads the second (rboff = MR_NE), the first is a decoy
        for (i in range(MR_NPOS * MR_NER)) {
            lh[i] = hash_unit(uint(i) * 3u + 101u) * 2.0 + float(i % 7) * 0.0625   //! spread past any near-tie
        }
        for (e in range(MR_NE)) {
            rb[e] = 7.0
            rb[MR_NE + e] = rbias ? hash_unit(uint(e) * 5u + 7u) * 3.0 - 1.5 : 0.0   //! a bias wide enough to move picks
        }
        let l_bytes = int64(MR_NPOS * MR_NER) * 4l
        let s_bytes = int64(MR_NPOS * MR_K) * 4l
        let rb_bytes = int64(2 * MR_NE) * 4l
        let ld = make_device_buf(l_bytes)
        let idxd = make_device_buf(s_bytes)
        let wd = make_device_buf(s_bytes)
        let rbd = make_device_buf(rb_bytes)
        var hosti = make_host_buf(s_bytes, true, [cached = true])
        var hostw = make_host_buf(s_bytes, true, [cached = true])
        var gidx : array<uint>
        var gw : array<float>
        gidx |> resize(MR_NPOS * MR_K)
        gw |> resize(MR_NPOS * MR_K)
        unsafe {
            upload_region_at(ld, 0l, addr<uint8 const?>(lh[0]), l_bytes)
            upload_region_at(rbd, 0l, addr<uint8 const?>(rb[0]), rb_bytes)
            nan_sentinel_upload(idxd, s_bytes)
            nan_sentinel_upload(wd, s_bytes)
            var sc = set_topk_rows_cls(fixed_array(ld, idxd, wd, ld, rbd), fixed_array(l_bytes, s_bytes, s_bytes, l_bytes, rb_bytes), fixed_array(1u, 2u, 4u, 1u, 0u))   //! no down scale: the logits fill the ds binding, never read at NO_DSCALE
            var raw = alloc_cmd()
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
            var h : VkHaz
            var pc = TopkRowsArgs(npos = uint(MR_NPOS), ne = uint(MR_NE), k = uint(MR_K), norm = norm || gate == MoeGate.softmax_weight ? 1u : 0u, lstride = uint(MR_NER), wscale = wscale, dsoff = NO_DSCALE,
                rbias_on = rbias ? 1u : 0u, rboff = uint(MR_NE))
            enc_topk_rows_cls(raw, h, sc, pc, int64(MR_NPOS))
            vhz_dep(raw, h, 2u | 4u, 0u, true)
            cmd_copy_whole(raw, idxd, hosti.buf, s_bytes)
            cmd_copy_whole(raw, wd, hostw.buf, s_bytes)
            vk_check(vkEndCommandBuffer(raw), null)
            submit_wait(raw)
            memcpy(addr<void?>(gidx[0]), hosti.mapped, s_bytes)
            memcpy(addr<void?>(gw[0]), hostw.mapped, s_bytes)
        }
        var widx : array<int64>
        var ww : array<float>
        widx |> resize(MR_NPOS * MR_K)
        ww |> resize(MR_NPOS * MR_K)
        var row : array<float>
        row |> resize(MR_NE)
        for (p in range(MR_NPOS)) {
            for (e in range(MR_NE)) {
                row[e] = lh[p * MR_NER + e] + rb[MR_NE + e]
            }
            unsafe {
                moe_select_core(gate, int64(MR_NE), int64(MR_K), norm, wscale, addr(row[0]), addr(widx[0]), addr(ww[0]), int64(p * MR_K))
            }
        }
        var badi = 0
        for (i in range(MR_NPOS * MR_K)) {
            if (int64(gidx[i]) != widx[i]) {
                badi++
            }
        }
        let badw = mismatch_rel(gw, ww, 1e-4)
        let arm = (gate == MoeGate.softmax_weight ? "softmax over the picks" : (norm ? "renormalized" : "scaled by {wscale}")) + (rbias ? ", router bias" : "")
        t |> success(badi == 0, "per-row top-k picks the host select's experts ({arm}; {badi} of {MR_NPOS * MR_K} off)")
        t |> success(badw == 0, "per-row top-k weights match the host select ({arm}; {badw} of {MR_NPOS * MR_K} off)")
        ww[4] += 0.5
        t |> success(mismatch_rel(gw, ww, 1e-4) == 1, "per-row top-k control: the bar reds one poisoned weight")
        if (rbias) {   //! the bias control: the select over the unbiased logits must pick differently somewhere
            var wid0 : array<int64>
            var ww0 : array<float>
            wid0 |> resize(MR_NPOS * MR_K)
            ww0 |> resize(MR_NPOS * MR_K)
            for (p in range(MR_NPOS)) {
                for (e in range(MR_NE)) {
                    row[e] = lh[p * MR_NER + e]
                }
                unsafe {
                    moe_select_core(gate, int64(MR_NE), int64(MR_K), norm, wscale, addr(row[0]), addr(wid0[0]), addr(ww0[0]), int64(p * MR_K))
                }
            }
            var moved = 0
            for (i in range(MR_NPOS * MR_K)) {
                if (wid0[i] != widx[i]) {
                    moved++
                }
            }
            t |> success(moved > 0, "router bias control: the bias moves {moved} of {MR_NPOS * MR_K} picks off the unbiased select")
            delete wid0
            delete ww0
        }
        delete rb
        delete lh
        delete gidx
        delete gw
        delete widx
        delete ww
        delete row
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
        var winv : array<uint>
        var w1 : array<uint>
        var w3 : array<uint>
        var w2 : array<uint>
        sched_oracle(idx, MR_NE, wt_gu, wt_dn, sh, bases, winv, w1, w3, w2)
        let i_bytes = int64(nk) * 4l
        let s_bytes = long_length(w1) * 4l
        let idxd = make_device_buf(i_bytes)
        let invd = make_device_buf(i_bytes)
        let bexd = make_device_buf(i_bytes)
        let s1d = make_device_buf(s_bytes)
        let s3d = make_device_buf(s_bytes)
        let s2d = make_device_buf(s_bytes)
        var fill : array<uint>
        fill |> resize(length(w1))
        for (f in fill) {
            f = MR_SENT
        }
        var host = make_host_buf(2l * i_bytes + 3l * s_bytes, true, [cached = true])
        var ginv : array<uint>
        var gbex : array<uint>
        var g1 : array<uint>
        var g3 : array<uint>
        var g2 : array<uint>
        ginv |> resize(nk)
        gbex |> resize(nk)
        g1 |> resize(length(w1))
        g3 |> resize(length(w1))
        g2 |> resize(length(w1))
        unsafe {
            upload_region_at(idxd, 0l, addr<uint8 const?>(idx[0]), i_bytes)
            nan_sentinel_upload(invd, i_bytes)
            nan_sentinel_upload(bexd, i_bytes)
            upload_region_at(s1d, 0l, addr<uint8 const?>(fill[0]), s_bytes)
            upload_region_at(s3d, 0l, addr<uint8 const?>(fill[0]), s_bytes)
            upload_region_at(s2d, 0l, addr<uint8 const?>(fill[0]), s_bytes)
            var sc = set_moe_sched_cls(fixed_array(idxd, invd, s1d, s3d, s2d, bexd), fixed_array(i_bytes, i_bytes, s_bytes, s_bytes, s_bytes, i_bytes), fixed_array(1u, 2u, 2u, 2u, 2u, 2u))
            var raw = alloc_cmd()
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
            var h : VkHaz
            var pc = SchedArgs(nk = uint(nk), ne = uint(MR_NE), wt_gu = uint(wt_gu), wt_dn = uint(wt_dn),
                map_off = uint(sh.map_off), map_off_mgu = uint(sh.map_off_mgu), map_off_mdn = uint(sh.map_off_mdn),
                bound_gu = uint(sh.bound_gu), bound_dn = uint(sh.bound_dn), bound_mgu = uint(sh.bound_mgu), bound_mdn = uint(sh.bound_mdn),
                base1 = bases[0], stride1 = bases[1], base3 = bases[2], stride3 = bases[3], base2 = bases[4], stride2 = bases[5])
            enc_moe_sched_cls(raw, h, sc, pc, 1l)
            vhz_dep(raw, h, 2u, 0u, true)
            cmd_copy_range(raw, invd, 0l, host.buf, 0l, i_bytes)
            cmd_copy_range(raw, s1d, 0l, host.buf, i_bytes, s_bytes)
            cmd_copy_range(raw, s3d, 0l, host.buf, i_bytes + s_bytes, s_bytes)
            cmd_copy_range(raw, s2d, 0l, host.buf, i_bytes + 2l * s_bytes, s_bytes)
            cmd_copy_range(raw, bexd, 0l, host.buf, i_bytes + 3l * s_bytes, i_bytes)
            vk_check(vkEndCommandBuffer(raw), null)
            submit_wait(raw)
            let hp = intptr(host.mapped)
            memcpy(addr<void?>(ginv[0]), host.mapped, i_bytes)
            memcpy(addr<void?>(g1[0]), reinterpret<void?>(hp + uint64(i_bytes)), s_bytes)
            memcpy(addr<void?>(g3[0]), reinterpret<void?>(hp + uint64(i_bytes + s_bytes)), s_bytes)
            memcpy(addr<void?>(g2[0]), reinterpret<void?>(hp + uint64(i_bytes + 2l * s_bytes)), s_bytes)
            memcpy(addr<void?>(gbex[0]), reinterpret<void?>(hp + uint64(i_bytes + 3l * s_bytes)), i_bytes)
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
        delete fill
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
        var sidx : array<uint>
        var sw : array<float>
        var rows : array<float>
        var xs : array<float>
        var sh : array<float>
        var wn : array<float>
        var lg : array<float>
        var sex : array<uint>
        var db : array<float>
        sidx |> resize(nrows)
        sw |> resize(nrows)
        rows |> resize(nrows * n)
        xs |> resize(npos * n)
        sh |> resize(npos * n)
        wn |> resize(n)
        lg |> resize(npos * MR_NER)
        sex |> resize(nrows)
        let dbn = dbias ? 2 * ne * n : n   //! two layers' rows when biased (the dispatch reads the second), the biasless zero row of the model dim otherwise - the driver's own shapes
        db |> resize(dbn)
        for (i in range(nrows)) {
            sidx[i] = uint((i * 7) % nrows)   //! a permutation of the bucket rows
            sw[i] = 0.125 + float(i % 5) * 0.25
            sex[i] = hash_word(uint(i) + 3u) % uint(ne)
        }
        for (i in range(dbn)) {
            db[i] = dbias ? (i < ne * n ? 9.0 : float((i * 19) % 23) * 0.25 - 2.75) : 0.0   //! the first layer's rows are a decoy
        }
        for (i in range(nrows * n)) {
            rows[i] = float((i * 7) % 93) * 0.13 - 5.5
        }
        for (i in range(npos * n)) {
            xs[i] = float((i * 11) % 37) * 0.1 - 1.7
            sh[i] = float((i * 5) % 41) * 0.2 - 3.0
        }
        for (i in range(n)) {
            wn[i] = 0.5 + float(i % 7) * 0.125
        }
        for (i in range(npos * MR_NER)) {
            lg[i] = hash_unit(uint(i) + 9u)
        }
        let si_bytes = int64(nrows) * 4l
        let x_bytes = int64(npos * n) * 4l
        let ro_bytes = int64(nrows * n) * 4l
        let wn_bytes = int64(n) * 4l
        let lg_bytes = int64(npos * MR_NER) * 4l
        let db_bytes = int64(dbn) * 4l
        let sid = make_device_buf(si_bytes)
        let swd = make_device_buf(si_bytes)
        let rod = make_device_buf(ro_bytes)
        let xd = make_device_buf(x_bytes)
        let shd = make_device_buf(x_bytes)
        let wnd = make_device_buf(wn_bytes)
        let lgd = make_device_buf(lg_bytes)
        let dbd = make_device_buf(db_bytes)
        let sexd = make_device_buf(si_bytes)
        let yod = make_device_buf(x_bytes)
        let yhd = make_device_buf(x_bytes / 2l)
        let oq_bytes = int64(npos * n)
        let os_bytes = int64(npos * n / 32) * 4l
        let oqd = make_device_buf(oq_bytes)
        let osd = make_device_buf(os_bytes)
        var host = make_host_buf(3l * x_bytes, true, [cached = true])
        var got_x : array<float>
        var got_y : array<float>
        var got_h : array<uint>
        var got_q : array<int8>
        var got_s : array<float>
        got_x |> resize(npos * n)
        got_y |> resize(npos * n)
        got_h |> resize(npos * n / 2)
        got_q |> resize(npos * n)
        got_s |> resize(npos * n / 32)
        var pc = ArArgs(dim = uint(n), add_on = with_sh ? 1u : 0u, woff = 0u, eps = eps, ascale = 1.0, row0 = 0u,
            slots = uint(k), lstride = uint(MR_NER), gated = gated ? 1u : 0u, dbias_on = dbias ? 1u : 0u, dboff = dbias ? uint(ne * n) : 0u)
        unsafe {
            upload_region_at(sid, 0l, addr<uint8 const?>(sidx[0]), si_bytes)
            upload_region_at(swd, 0l, addr<uint8 const?>(sw[0]), si_bytes)
            upload_region_at(rod, 0l, addr<uint8 const?>(rows[0]), ro_bytes)
            upload_region_at(shd, 0l, addr<uint8 const?>(sh[0]), x_bytes)
            upload_region_at(wnd, 0l, addr<uint8 const?>(wn[0]), wn_bytes)
            upload_region_at(lgd, 0l, addr<uint8 const?>(lg[0]), lg_bytes)
            upload_region_at(dbd, 0l, addr<uint8 const?>(db[0]), db_bytes)
            upload_region_at(sexd, 0l, addr<uint8 const?>(sex[0]), si_bytes)
            nan_sentinel_upload(yod, x_bytes)
            nan_sentinel_upload(yhd, x_bytes / 2l)
            //! the f32 form: x updated in place, the normed row to yo
            upload_region_at(xd, 0l, addr<uint8 const?>(xs[0]), x_bytes)
            var sc = set_cls_ar_comb(fixed_array(xd, shd, wnd, yod, sid, swd, rod, lgd, dbd, sexd),
                fixed_array(x_bytes, x_bytes, wn_bytes, x_bytes, si_bytes, si_bytes, ro_bytes, lg_bytes, db_bytes, si_bytes),
                fixed_array(1u, 2u, 0u, 4u, 8u, 8u, 16u, 32u, 0u, 8u))
            var raw = alloc_cmd()
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
            var h : VkHaz
            enc_cls_ar_comb(raw, h, sc, pc, int64(npos))
            vhz_dep(raw, h, 1u | 4u, 0u, true)
            cmd_copy_range(raw, xd, 0l, host.buf, 0l, x_bytes)
            cmd_copy_range(raw, yod, 0l, host.buf, x_bytes, x_bytes)
            vk_check(vkEndCommandBuffer(raw), null)
            submit_wait(raw)
            memcpy(addr<void?>(got_x[0]), host.mapped, x_bytes)
            memcpy(addr<void?>(got_y[0]), reinterpret<void?>(intptr(host.mapped) + uint64(x_bytes)), x_bytes)
            //! the f16 form over a fresh x
            upload_region_at(xd, 0l, addr<uint8 const?>(xs[0]), x_bytes)
            var sc16 = set_cls_ar_comb_f16_b(fixed_array(xd, shd, wnd, yhd, sid, swd, rod, lgd, dbd, sexd),
                fixed_array(x_bytes, x_bytes, wn_bytes, x_bytes / 2l, si_bytes, si_bytes, ro_bytes, lg_bytes, db_bytes, si_bytes),
                fixed_array(1u, 2u, 0u, 4u, 8u, 8u, 16u, 32u, 0u, 8u))
            var raw2 = alloc_cmd()
            vk_check(vkBeginCommandBuffer(raw2, begin), null)
            var h2 : VkHaz
            enc_cls_ar_comb_f16_b(raw2, h2, sc16, pc, int64(npos))
            vkd_run_copy(raw2, h2, yhd, host, x_bytes / 2l, 4u)
            memcpy(addr<void?>(got_h[0]), host.mapped, x_bytes / 2l)
            //! the quantizing twin over a fresh x: the normed rows as Q8_0 blocks at the row binding, the scales past the slot map
            upload_region_at(xd, 0l, addr<uint8 const?>(xs[0]), x_bytes)
            nan_sentinel_upload(oqd, oq_bytes)
            nan_sentinel_upload(osd, os_bytes)
            var scq = set_cls_ar_comb_rq(fixed_array(xd, shd, wnd, oqd, sid, swd, rod, lgd, dbd, sexd, osd),
                fixed_array(x_bytes, x_bytes, wn_bytes, oq_bytes, si_bytes, si_bytes, ro_bytes, lg_bytes, db_bytes, si_bytes, os_bytes),
                fixed_array(1u, 2u, 0u, 4u, 8u, 8u, 16u, 32u, 0u, 8u, 4u))
            var raw3 = alloc_cmd()
            vk_check(vkBeginCommandBuffer(raw3, begin), null)
            var h3 : VkHaz
            enc_cls_ar_comb_rq(raw3, h3, scq, pc, int64(npos))
            vhz_dep(raw3, h3, 4u, 0u, true)
            cmd_copy_range(raw3, oqd, 0l, host.buf, 0l, oq_bytes)
            cmd_copy_range(raw3, osd, 0l, host.buf, oq_bytes, os_bytes)
            vk_check(vkEndCommandBuffer(raw3), null)
            submit_wait(raw3)
            memcpy(addr<void?>(got_q[0]), host.mapped, oq_bytes)
            memcpy(addr<void?>(got_s[0]), reinterpret<void?>(intptr(host.mapped) + uint64(oq_bytes)), os_bytes)
        }
        var want_x : array<float>
        var want_y : array<float>
        want_x |> resize(npos * n)
        want_y |> resize(npos * n)
        for (p in range(npos)) {
            let g = with_sh && gated ? 1.0 / (1.0 + exp(-lg[p * MR_NER + MR_NE])) : 1.0
            var ss = 0.0
            for (i in range(n)) {
                var v = xs[p * n + i]
                if (with_sh) {
                    v += g * sh[p * n + i]
                }
                for (j in range(k)) {
                    v += sw[p * k + j] * (rows[int(sidx[p * k + j]) * n + i] + (dbias ? db[ne * n + int(sex[p * k + j]) * n + i] : 0.0))
                }
                want_x[p * n + i] = v
                ss += v * v
            }
            let inv = 1.0 / sqrt(ss / float(n) + eps)
            for (i in range(n)) {
                want_y[p * n + i] = wn[i] * (want_x[p * n + i] * inv)
            }
        }
        let arm = (with_sh ? (gated ? "gated shared" : "ungated shared") : "no shared") + " expert" + (dbias ? ", down biases" : "")
        t |> success(mismatch_approx(got_x, xs) > 0, "the combine-folded residual step ({arm}): the in-place residual rows moved off their input")
        let bad_x = mismatch_approx(got_x, want_x)
        t |> success(bad_x == 0, "the combine-folded residual step ({arm}): the residual rows match the CPU oracle ({bad_x} of {npos * n} off)")
        let bad_y = mismatch_approx(got_y, want_y)
        t |> success(bad_y == 0, "the combine-folded residual step ({arm}): the f32 normed rows match the CPU oracle ({bad_y} of {npos * n} off)")
        var got_yf : array<float>
        got_yf |> resize(npos * n)
        for (i in range(npos * n / 2)) {
            let pr = unpackHalf2x16(got_h[i])
            got_yf[2 * i] = pr.x
            got_yf[2 * i + 1] = pr.y
        }
        //! the f16 bar: a half rounds to 2^-11 of the value (4.9e-4 relative); the bar is four halves of
        //! that, and the same fraction of the row's largest value as the floor the near-zero elements read
        let f16_bar = 2.0e-3
        let bad_h = mismatch_bars(got_yf, want_y, f16_bar, f16_bar * max_abs(want_y))
        t |> success(bad_h == 0, "the combine-folded residual step ({arm}): the f16 normed rows within the half's rounding of the oracle ({bad_h} off)")
        //! the quantizing twin: every block's scale within 2e-3 of the oracle's amax over 127, every dequantized element within a quant step
        var bad_s = 0
        var bad_q = 0
        var pbad = 0
        for (blk in range(npos * n / 32)) {
            var m = 0.0
            for (e in range(32)) {
                m = max(m, abs(want_y[blk * 32 + e]))
            }
            let d_ref = m / 127.0
            if (abs(got_s[blk] - d_ref) > 2e-3 * max(d_ref, 1e-6)) {
                bad_s++
            }
            for (e in range(32)) {
                let deq = float(got_q[blk * 32 + e]) * got_s[blk]
                let a = want_y[blk * 32 + e]
                if (abs(deq - a) > d_ref + 2e-3 * abs(a)) {
                    bad_q++
                }
                if (e == 1 && abs(deq - (a + 3.0 * d_ref + 1.0)) <= d_ref + 2e-3 * abs(a)) {   // the control: the expectation pushed three quant steps must miss
                    pbad++
                }
            }
        }
        t |> success(bad_s == 0, "the combine-folded residual step ({arm}): the quantizing twin's block scales match the oracle ({bad_s} of {npos * n / 32} off)")
        t |> success(bad_q == 0, "the combine-folded residual step ({arm}): the quantizing twin's rows sit within a quant step of the oracle ({bad_q} of {npos * n} off)")
        t |> success(pbad == 0, "combine-folded residual control: the quant bar reds a poisoned oracle element in every block ({pbad} passed)")
        want_x[2 * n + 9] += 0.5
        t |> success(mismatch_approx(got_x, want_x) == 1, "combine-folded residual control: the residual bar reds one poisoned element")
        want_y[3 * n + 5] += 0.5
        t |> success(mismatch_approx(got_y, want_y) == 1, "combine-folded residual control: the f32 normed-row bar reds one poisoned element")
        t |> success(mismatch_bars(got_yf, want_y, f16_bar, f16_bar * max_abs(want_y)) == 1, "combine-folded residual control: the f16 normed-row bar reds one poisoned element")
        delete sidx
        delete sw
        delete rows
        delete xs
        delete sh
        delete wn
        delete lg
        delete got_x
        delete got_y
        delete got_h
        delete got_yf
        delete got_q
        delete got_s
        delete want_x
        delete want_y
        delete sex
        delete db
    }
}

//! the gemma-4 combine: x = (x + post(post2(sum_j w_j d_j) + post1(dense))) * ascale, then the next norm -
//! the routed sum and the dense row each under their own post norm, their sum under the layer's, the step
//! scaled by the output scale; the f32 and f16 normed-row forms against one CPU oracle
def private moe_ar_comb_g4_arm(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let n = 64
        let k = 8   //! gemma-4's eight routed slots: the row pass's register form full
        let npos = 5
        let nrows = npos * k
        let eps = 1.0e-6
        let ascale = 0.75
        var sidx : array<uint>
        var sw : array<float>
        var rows : array<float>
        var xs : array<float>
        var sh : array<float>
        var wn : array<float>   //! four norm rows: the next norm, the dense post norm, the routed post norm, the layer's post norm
        sidx |> resize(nrows)
        sw |> resize(nrows)
        rows |> resize(nrows * n)
        xs |> resize(npos * n)
        sh |> resize(npos * n)
        wn |> resize(4 * n)
        for (i in range(nrows)) {
            sidx[i] = uint((i * 7) % nrows)
            sw[i] = 0.125 + float(i % 5) * 0.25
        }
        for (i in range(nrows * n)) {
            rows[i] = float((i * 7) % 93) * 0.13 - 5.5
        }
        for (i in range(npos * n)) {
            xs[i] = float((i * 11) % 37) * 0.1 - 1.7
            sh[i] = float((i * 5) % 41) * 0.2 - 3.0
        }
        for (i in range(4 * n)) {
            wn[i] = 0.5 + float((i * 3) % 7) * 0.125
        }
        let si_bytes = int64(nrows) * 4l
        let x_bytes = int64(npos * n) * 4l
        let ro_bytes = int64(nrows * n) * 4l
        let wn_bytes = int64(4 * n) * 4l
        let sid = make_device_buf(si_bytes)
        let swd = make_device_buf(si_bytes)
        let rod = make_device_buf(ro_bytes)
        let xd = make_device_buf(x_bytes)
        let shd = make_device_buf(x_bytes)
        let wnd = make_device_buf(wn_bytes)
        let yod = make_device_buf(x_bytes)
        let yhd = make_device_buf(x_bytes / 2l)
        var host = make_host_buf(3l * x_bytes, true, [cached = true])
        var got_x : array<float>
        var got_y : array<float>
        var got_h : array<uint>
        got_x |> resize(npos * n)
        got_y |> resize(npos * n)
        got_h |> resize(npos * n / 2)
        var pc = ArArgs(dim = uint(n), add_on = 1u, woff = 0u, eps = eps, ascale = ascale, row0 = 0u,
            slots = uint(k), pwoff = uint(n), routed_post_off = uint(2 * n), sum_post_off = uint(3 * n))
        unsafe {
            upload_region_at(sid, 0l, addr<uint8 const?>(sidx[0]), si_bytes)
            upload_region_at(swd, 0l, addr<uint8 const?>(sw[0]), si_bytes)
            upload_region_at(rod, 0l, addr<uint8 const?>(rows[0]), ro_bytes)
            upload_region_at(shd, 0l, addr<uint8 const?>(sh[0]), x_bytes)
            upload_region_at(wnd, 0l, addr<uint8 const?>(wn[0]), wn_bytes)
            nan_sentinel_upload(yod, x_bytes)
            nan_sentinel_upload(yhd, x_bytes / 2l)
            upload_region_at(xd, 0l, addr<uint8 const?>(xs[0]), x_bytes)
            var sc = set_cls_ar_comb_g4(fixed_array(xd, shd, wnd, yod, sid, swd, rod),
                fixed_array(x_bytes, x_bytes, wn_bytes, x_bytes, si_bytes, si_bytes, ro_bytes),
                fixed_array(1u, 2u, 0u, 4u, 8u, 8u, 16u))
            var raw = alloc_cmd()
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
            var h : VkHaz
            enc_cls_ar_comb_g4(raw, h, sc, pc, int64(npos))
            vhz_dep(raw, h, 1u | 4u, 0u, true)
            cmd_copy_range(raw, xd, 0l, host.buf, 0l, x_bytes)
            cmd_copy_range(raw, yod, 0l, host.buf, x_bytes, x_bytes)
            vk_check(vkEndCommandBuffer(raw), null)
            submit_wait(raw)
            memcpy(addr<void?>(got_x[0]), host.mapped, x_bytes)
            memcpy(addr<void?>(got_y[0]), reinterpret<void?>(intptr(host.mapped) + uint64(x_bytes)), x_bytes)
            upload_region_at(xd, 0l, addr<uint8 const?>(xs[0]), x_bytes)
            var sc16 = set_cls_ar_comb_g4_f16_b(fixed_array(xd, shd, wnd, yhd, sid, swd, rod),
                fixed_array(x_bytes, x_bytes, wn_bytes, x_bytes / 2l, si_bytes, si_bytes, ro_bytes),
                fixed_array(1u, 2u, 0u, 4u, 8u, 8u, 16u))
            var raw2 = alloc_cmd()
            vk_check(vkBeginCommandBuffer(raw2, begin), null)
            var h2 : VkHaz
            enc_cls_ar_comb_g4_f16_b(raw2, h2, sc16, pc, int64(npos))
            vkd_run_copy(raw2, h2, yhd, host, x_bytes / 2l, 4u)
            memcpy(addr<void?>(got_h[0]), host.mapped, x_bytes / 2l)
        }
        var want_x : array<float>
        var want_y : array<float>
        var r : array<float>
        var c : array<float>
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
                    v += sw[p * k + j] * rows[int(sidx[p * k + j]) * n + i]
                }
                r[i] = v
                sr += v * v
                sd += sh[p * n + i] * sh[p * n + i]
            }
            let inv_r = 1.0 / sqrt(sr / float(n) + eps)
            let inv_d = 1.0 / sqrt(sd / float(n) + eps)
            var sc = 0.0
            for (i in range(n)) {
                c[i] = wn[2 * n + i] * (r[i] * inv_r) + wn[n + i] * (sh[p * n + i] * inv_d)
                sc += c[i] * c[i]
            }
            let inv_c = 1.0 / sqrt(sc / float(n) + eps)
            var ss = 0.0
            for (i in range(n)) {
                let v = (xs[p * n + i] + wn[3 * n + i] * (c[i] * inv_c)) * ascale
                want_x[p * n + i] = v
                ss += v * v
            }
            let inv = 1.0 / sqrt(ss / float(n) + eps)
            for (i in range(n)) {
                want_y[p * n + i] = wn[i] * (want_x[p * n + i] * inv)
            }
        }
        t |> success(mismatch_approx(got_x, xs) > 0, "the gemma-4 combine step: the in-place residual rows moved off their input")
        let bad_x = mismatch_approx(got_x, want_x)
        t |> success(bad_x == 0, "the gemma-4 combine step: the residual rows match the CPU oracle ({bad_x} of {npos * n} off)")
        let bad_y = mismatch_approx(got_y, want_y)
        t |> success(bad_y == 0, "the gemma-4 combine step: the f32 normed rows match the CPU oracle ({bad_y} of {npos * n} off)")
        var got_yf : array<float>
        got_yf |> resize(npos * n)
        for (i in range(npos * n / 2)) {
            let pr = unpackHalf2x16(got_h[i])
            got_yf[2 * i] = pr.x
            got_yf[2 * i + 1] = pr.y
        }
        let f16_bar = 2.0e-3
        let bad_h = mismatch_bars(got_yf, want_y, f16_bar, f16_bar * max_abs(want_y))
        t |> success(bad_h == 0, "the gemma-4 combine step: the f16 normed rows within the half's rounding of the oracle ({bad_h} off)")
        //! the quantizing twin: the same step, the normed rows as Q8_0 blocks (two a row) held to the oracle's blocks
        if (ensure_cls_ar_comb_g4_rq()) {
            let nblk = npos * n / 32
            let oq_bytes = int64(npos * n)
            let os_bytes = int64(nblk) * 4l
            let oqd = make_device_buf(oq_bytes)
            let osd = make_device_buf(os_bytes)
            var oq : array<int8>
            var os : array<float>
            oq |> resize(npos * n)
            os |> resize(nblk)
            unsafe {
                upload_region_at(xd, 0l, addr<uint8 const?>(xs[0]), x_bytes)
                nan_sentinel_upload(oqd, oq_bytes)
                nan_sentinel_upload(osd, os_bytes)
                var scq = set_cls_ar_comb_g4_rq(fixed_array(xd, shd, wnd, oqd, sid, swd, rod, osd),
                    fixed_array(x_bytes, x_bytes, wn_bytes, oq_bytes, si_bytes, si_bytes, ro_bytes, os_bytes),
                    fixed_array(1u, 2u, 0u, 4u, 8u, 8u, 16u, 4u))
                var rawq = alloc_cmd()
                let beginq = VkCommandBufferBeginInfo()
                vk_check(vkBeginCommandBuffer(rawq, beginq), null)
                var hq : VkHaz
                enc_cls_ar_comb_g4_rq(rawq, hq, scq, pc, int64(npos))
                vhz_dep(rawq, hq, 4u, 0u, true)
                cmd_copy_range(rawq, oqd, 0l, host.buf, 0l, oq_bytes)
                cmd_copy_range(rawq, osd, 0l, host.buf, oq_bytes, os_bytes)
                vk_check(vkEndCommandBuffer(rawq), null)
                submit_wait(rawq)
                memcpy(addr<void?>(oq[0]), host.mapped, oq_bytes)
                memcpy(addr<void?>(os[0]), reinterpret<void?>(intptr(host.mapped) + uint64(oq_bytes)), os_bytes)
            }
            var badq = 0
            var badqs = 0
            for (blk in range(nblk)) {
                var m = 0.0
                for (e in range(32)) {
                    m = max(m, abs(want_y[blk * 32 + e]))
                }
                let d_ref = m / 127.0
                if (abs(os[blk] - d_ref) > 2e-3 * max(d_ref, 1e-6)) {
                    badqs++
                }
                for (e in range(32)) {
                    let deq = float(oq[blk * 32 + e]) * os[blk]
                    if (abs(deq - want_y[blk * 32 + e]) > d_ref + 2e-3 * abs(want_y[blk * 32 + e])) {
                        badq++
                    }
                }
            }
            t |> success(badqs == 0, "the gemma-4 quantizing combine: the block scales match the oracle's normed rows ({badqs} of {nblk} off)")
            t |> success(badq == 0, "the gemma-4 quantizing combine: the dequantized rows sit within a quant step of the oracle ({badq} of {npos * n} off)")
            var poisonq := want_y
            poisonq[0] += 3.0 * os[0] + 1.0
            var pbad = 0
            for (e in range(32)) {
                let deq = float(oq[e]) * os[0]
                if (abs(deq - poisonq[e]) > os[0] + 2e-3 * abs(poisonq[e])) {
                    pbad++
                }
            }
            t |> success(pbad > 0, "gemma-4 quantizing combine control: the quant bar reds a poisoned oracle element")
            delete oq
            delete os
            delete poisonq
        }
        want_x[2 * n + 9] += 0.5
        t |> success(mismatch_approx(got_x, want_x) == 1, "gemma-4 combine control: the residual bar reds one poisoned element")
        want_y[3 * n + 5] += 0.5
        t |> success(mismatch_approx(got_y, want_y) == 1, "gemma-4 combine control: the f32 normed-row bar reds one poisoned element")
        t |> success(mismatch_bars(got_yf, want_y, f16_bar, f16_bar * max_abs(want_y)) == 1, "gemma-4 combine control: the f16 normed-row bar reds one poisoned element")
        delete sidx
        delete sw
        delete rows
        delete xs
        delete sh
        delete wn
        delete got_x
        delete got_y
        delete got_h
        delete got_yf
        delete want_x
        delete want_y
        delete r
        delete c
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
            let grid = fixed_array(KqFmt.iq2s, KqFmt.iq2xs, KqFmt.iq2xxs, KqFmt.iq3xxs, KqFmt.iq4xs, KqFmt.iq4nl)
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
