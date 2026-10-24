options gen2
options _dasllama_internal = true
options stack = 524288

// The decode GEMV on both discrete homes of one card, the same planes: the Vulkan class kernel through the hazard rail and
// the CUDA class kernel through its launch rail, at a llama-1B projection's shapes, over Q8_0, the Q4_K_M file's K-quant
// formats and the Q5_1 file's per-32 format. Each arm rotates its dispatches through a ring of weight copies past the card's
// L2, so the race reads the planes from VRAM.
// ATTRIBUTION SWEEP: the one mode times both homes' decode GEMVs to attribute their token-rate gap; no output is compared.
// The arms run in turn, one round of each (a round: one submit of DISPATCHES dispatches); each arm drops 2 warmup rounds,
// and every number is its best round after them, in microseconds a dispatch.
//   DASLLAMA_GPU=1 daslang -jit modules/dasLLAMA/harness/gemv_cuda_vs_vulkan.das

require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_cuda_classes
require ../tests/_vk_kq_fixtures.das
require _cuda_probe_fixture
require daslib/strings_boost
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema

let ROUNDS = 20
let WARMUPS = 2
let DISPATCHES = 50
let RING_BYTES = 64l << 20l   //! a ring of weight copies past this outgrows a 32 MB L2 twice over

//! the Q8_0 planes of `d` rows of `nb` blocks over the ramp scale plane, one region, the activation scales `0.25 + 0.125 x (i % 5)`
def planes(d, nb : int) : GemvFix {
    var wsh <- ramp_ws_plane(d * nb)
    return <- gemv_fix(1, d, nb, 8, 55u, wsh, nb, 8, 17u, 9u, 0.25, 0.125, 5)
}

//! the weight and activation bytes one dispatch reads
def gemv_bytes(n, d : int) : double =>
    double(d) * double(n) * (1.0lf + 2.0lf / 32.0lf) + double(n) * (1.0lf + 4.0lf / 32.0lf)

def gbs(bytes : double; us : double) : double => bytes / us / 1000.0lf

//! the weight plane pairs a ring of `p`'s planes holds, together past `RING_BYTES`
def ring_copies(p : GemvFix) : int => int(RING_BYTES / ((long_length(p.wqh) + long_length(p.wsh)) * 4l)) + 1

//! a Vulkan arm's device planes: the weight copies, the one activation row and output they share, a descriptor set a copy
struct VkRing {
    wq : array<uint64>
    ws : array<uint64>
    regs : uint64
    xq : uint64
    xs : uint64
    y : uint64
    sets : array<VkDescriptorSet>
}

def vk_ring(p : GemvFix; d : int; mk_set : block<(bufs : uint64 const[6]; sizes : int64 const[6]; bits : uint const[6]) : VkDescriptorSet>) : VkRing {
    let copies = ring_copies(p)
    var r <- VkRing(wq <- [for (_c in range(copies)); dev_upload(p.wqh)], ws <- [for (_c in range(copies)); dev_upload(p.wsh)],
        regs = dev_upload(p.regs), xq = dev_upload(p.xqh), xs = dev_upload(p.xsh), y = make_device_buf(int64(d) * 4l))
    let sizes = fixed_array(long_length(p.wqh) * 4l, long_length(p.wsh) * 4l, long_length(p.regs) * 4l, long_length(p.xqh) * 4l, long_length(p.xsh) * 4l, int64(d) * 4l)
    let bits = fixed_array(0u, 0u, 0u, 0u, 0u, 1u)
    r.sets <- [for (c in range(copies)); invoke(mk_set, fixed_array(r.wq[c], r.ws[c], r.regs, r.xq, r.xs, r.y), sizes, bits)]
    return <- r
}

def vk_ring_free(var r : VkRing) {
    for (b in r.wq) {
        destroy_device_buf(b)
    }
    for (b in r.ws) {
        destroy_device_buf(b)
    }
    for (b in [r.regs, r.xq, r.xs, r.y]) {
        destroy_device_buf(b)
    }
    delete r.wq
    delete r.ws
}

//! a CUDA arm's launch sets: a weight copy each, over the one activation row and output
def cuda_ring(p : GemvFix; d : int) : array<uint64[6]> {
    let s = g_cud.stream
    let regs = buf_upload(p.regs, s)
    let xq = buf_upload(p.xqh, s)
    let xs = buf_upload(p.xsh, s)
    let y = buf_fill(d, 0.0, s)
    return <- [for (_c in range(ring_copies(p))); fixed_array(buf_upload(p.wqh, s), buf_upload(p.wsh, s), regs, xq, xs, y)]
}

def cuda_ring_free(var ring : array<uint64[6]>) {
    for (bufs in ring) {
        cuda_mem_free(bufs[0])
        cuda_mem_free(bufs[1])
    }
    for (b in range(2, 6)) {
        cuda_mem_free(ring[0][b])
    }
    delete ring
}

//! one race over `p`'s planes: the Vulkan arm when `vk_armed`, its dispatches over a ring of descriptor sets `mk_set` binds
//! and `vk_enc` encodes from, and the CUDA arm when `cu_armed`, its launches over a ring of launch sets `cu_enc` launches from
def race_gemv(p : GemvFix; d : int; vk_armed, cu_armed : bool;
              mk_set : block<(bufs : uint64 const[6]; sizes : int64 const[6]; bits : uint const[6]) : VkDescriptorSet>;
              vk_enc : block<(cmd : VkCommandBuffer; var hz : VkHaz; var set_ : VkDescriptorSet) : void>;
              cu_enc : block<(bufs : uint64 const[6]) : void>) : RaceRow {
    var r <- vk_armed ? vk_ring(p, d, mk_set) : default<VkRing>
    var raw : VkCommandBuffer
    if (vk_armed) {
        raw = record_arm() $(cmd : VkCommandBuffer; var hz : VkHaz) {
            for (i in range(DISPATCHES)) {
                invoke(vk_enc, cmd, hz, r.sets[i % length(r.sets)])
            }
        }
    }
    var ring <- cu_armed ? cuda_ring(p, d) : default<array<uint64[6]>>
    let row = race(ROUNDS, WARMUPS, DISPATCHES, vk_armed ? "" : VK_CLASS_REFUSED, raw, cu_armed ? "" : CU_CLASS_REFUSED) $(i) {
        invoke(cu_enc, ring[i % length(ring)])
    }
    if (vk_armed) {
        vk_ring_free(r)
    }
    if (cu_armed) {
        cuda_ring_free(ring)
    }
    return row
}

def race_q8(p : GemvFix; n, d : int) : RaceRow {
    var pc = GemvArgs(n = uint(n), d = uint(d), nreg = 1u, ybase = 0u)
    let vk_armed = dasllama_vulkan_classes::ensure_q8_gemv_cls()
    let cu_armed = dasllama_cuda_classes::ensure_q8_gemv_cls()
    let vk_wgs = vk_armed ? gemv_wgs(int64(d), 0u) : 0l   //! the grid reads the device the ensure arms
    let cu_wgs = cu_armed ? cud_gemv_wgs(int64(d), 0u) : 0l
    return race_gemv(p, d, vk_armed, cu_armed,
        $(bufs; sizes; bits) { return set_q8_gemv_cls(bufs, sizes, bits); },
        $(cmd : VkCommandBuffer; var hz : VkHaz; var set_ : VkDescriptorSet) { dasllama_vulkan_classes::enc_q8_gemv_cls(cmd, hz, set_, pc, vk_wgs); },
        $(bufs) { dasllama_cuda_classes::enc_q8_gemv_cls(g_cud.stream, bufs, pc, cu_wgs); })
}

//! a K-quant plane pair of `d` rows of `n` elements in the device layout, hashed, with its Q8_K activation row
def kq_planes(fmt, d, n : int) : GemvFix {
    let nsb = n / 256
    var wsh <- kq_ws_plane(fmt, d * nsb * int(kq_dev_ssb_i(fmt) / 4l), 1u, uint(fmt) * 13u)
    return <- gemv_fix(1, d, nsb, int(kq_qsb(kq_fmt_of_slot(fmt)) / 4l), uint(fmt) * 77777u, wsh, nsb, 64, 31u, 5u, 0.5, 0.25, 7)
}

//! the weight and activation bytes a K-quant dispatch reads
def kq_bytes(fmt, n, d : int) : double =>
    double(d) * double(n / 256) * double(kq_qsb(kq_fmt_of_slot(fmt)) + kq_dev_ssb_i(fmt)) + double(n) * (1.0lf + 4.0lf / 256.0lf)

//! a per-32 plane pair of `d` rows of `n` elements (the format's quant words a block, a scale word a block or a half of one),
//! hashed, with its Q8_0 activation row
def b32_planes(fmt, d, n : int) : GemvFix {
    let nb = n / 32
    let f = kq_fmt_of_slot(fmt)
    var wsh <- ws_plane((d * nb * int(kq_ssb(f)) + 3) / 4, 1u, uint(fmt) * 13u)
    return <- gemv_fix(1, d, nb, int(kq_qsb(f) / 4l), uint(fmt) * 77777u, wsh, nb, 8, 31u, 5u, 0.5, 0.25, 7)
}

//! the weight and activation bytes a per-32 dispatch reads
def b32_bytes(fmt, n, d : int) : double =>
    double(d) * double(n / 32) * double(kq_qsb(kq_fmt_of_slot(fmt)) + kq_ssb(kq_fmt_of_slot(fmt))) + double(n) * (1.0lf + 4.0lf / 32.0lf)

def race_kq(fmt : int; p : GemvFix; n, d : int) : RaceRow {
    var pc = GemvArgs(n = uint(n), d = uint(d), nreg = 1u, ybase = 0u)
    return race_gemv(p, d, gemv_cls_ensure(fmt), cud_gemv_ensure(fmt),
        $(bufs; sizes; bits) { return gemv_cls_set(fmt, bufs, sizes, bits); },
        $(cmd : VkCommandBuffer; var hz : VkHaz; var set_ : VkDescriptorSet) { gemv_enc(fmt, cmd, hz, set_, pc, int64(d)); },
        $(bufs) { cud_gemv_enc(fmt, g_cud.stream, bufs, pc, int64(d)); })
}

//! the same race at `lanes` lanes a row (0 = the whole subgroup) in place of the rule's, both homes
def race_kq_lanes(fmt : int; p : GemvFix; n, d : int; lanes : uint) : RaceRow {
    var pc = GemvArgs(n = uint(n), d = uint(d), nreg = 1u, ybase = 0u, lanes_per_row = lanes)
    let vk_armed = gemv_cls_ensure(fmt)
    let cu_armed = cud_gemv_ensure(fmt)
    let vk_wgs = vk_armed ? gemv_wgs(int64(d), lanes) : 0l
    let cu_wgs = cu_armed ? cud_gemv_wgs(int64(d), lanes) : 0l
    return race_gemv(p, d, vk_armed, cu_armed,
        $(bufs; sizes; bits) { return gemv_cls_set(fmt, bufs, sizes, bits); },
        $(cmd : VkCommandBuffer; var hz : VkHaz; var set_ : VkDescriptorSet) { gemv_cls_enc(fmt, cmd, hz, set_, pc, vk_wgs); },
        $(bufs) { cud_gemv_cls_enc(fmt, g_cud.stream, bufs, pc, cu_wgs); })
}

//! `--only <fmt,...>`: the named superblock formats at the llama-8B token step's shapes, at the rule's lanes a row and at 8, 16
//! and the whole subgroup (a lane-rule A/B); the rows measured
def race_only_rows(only : table<string; void>) : int {
    var measured = 0
    let shapes <- [(n = 4096, d = 4096), (n = 4096, d = 14336), (n = 14336, d = 4096)]
    print("superblock decode GEMV at the llama-8B shapes, the rule's lanes a row then 8 / 16 / 32, both homes\n")
    for (fmt in fixed_array(int(KqFmt.k4), int(KqFmt.k5), int(KqFmt.k6), int(KqFmt.iq4xs), int(KqFmt.q40), int(KqFmt.iq4nl), int(KqFmt.nv4))) {
        continue if (!key_exists(only, "{kq_fmt_of_slot(fmt)}"))
        for (s in shapes) {
            var p <- kq_planes(fmt, s.d, s.n)
            let bytes = kq_bytes(fmt, s.n, s.d)
            let row = race_kq(fmt, p, s.n, s.d)
            let rule = gemv_lanes_per_row(fmt, int64(s.n))
            measured += report_race(pad_right("{kq_fmt_of_slot(fmt)} n {s.n} d {s.d} rule {rule == 0u ? 32 : int(rule)}", 30, ' '), row, "GB/s") $(us) => gbs(bytes, us)
            for (lanes in fixed_array(8u, 16u, 0u)) {
                let lrow = race_kq_lanes(fmt, p, s.n, s.d, lanes)
                measured += report_race(pad_right("{kq_fmt_of_slot(fmt)} n {s.n} d {s.d} lanes {lanes == 0u ? 32 : int(lanes)}", 30, ' '), lrow, "GB/s") $(us) => gbs(bytes, us)
            }
            delete p
        }
    }
    return measured
}

[export]
def main {
    let only <- race_only()
    if (!empty(only)) {
        let measured = race_only_rows(only)
        cud_shutdown()
        require_measured(measured)
        return
    }
    let shapes <- [(n = 2048, d = 2048), (n = 8192, d = 2048), (n = 2048, d = 8192), (n = 2048, d = 128256)]
    var measured = 0
    print("q8 decode GEMV, one card, both discrete homes: the best of {ROUNDS} rounds of {DISPATCHES} dispatches after {WARMUPS} warmup rounds, us a dispatch, the weights a ring past {RING_BYTES >> 20l} MB\n")
    for (s in shapes) {
        var p <- planes(s.d, s.n / 32)
        let bytes = gemv_bytes(s.n, s.d)
        let row = race_q8(p, s.n, s.d)
        measured += report_race(pad_right("n {s.n} d {s.d}", 18, ' '), row, "GB/s") $(us) => gbs(bytes, us)
        delete p
    }
    print("K-quant decode GEMV (the Q4_K_M file's formats), the same homes\n")
    for (fmt in fixed_array(int(KqFmt.k4), int(KqFmt.k6), int(KqFmt.k5), int(KqFmt.iq4xs))) {
        for (s in shapes) {
            continue if (s.d > 8192)   //! the classifier shape sits on the Q8_0 table in the 1B files
            var p <- kq_planes(fmt, s.d, s.n)
            let bytes = kq_bytes(fmt, s.n, s.d)
            let row = race_kq(fmt, p, s.n, s.d)
            measured += report_race(pad_right("{kq_fmt_of_slot(fmt)} n {s.n} d {s.d}", 22, ' '), row, "GB/s") $(us) => gbs(bytes, us)
            delete p
        }
    }
    print("per-32 decode GEMV (the Q5_1 file's format), the same homes\n")
    for (fmt in fixed_array(int(KqFmt.q51))) {
        for (s in shapes) {
            continue if (s.d > 8192)
            var p <- b32_planes(fmt, s.d, s.n)
            let bytes = b32_bytes(fmt, s.n, s.d)
            let row = race_kq(fmt, p, s.n, s.d)
            measured += report_race(pad_right("{kq_fmt_of_slot(fmt)} n {s.n} d {s.d}", 22, ' '), row, "GB/s") $(us) => gbs(bytes, us)
            delete p
        }
    }
    cud_shutdown()
    require_measured(measured)
}
