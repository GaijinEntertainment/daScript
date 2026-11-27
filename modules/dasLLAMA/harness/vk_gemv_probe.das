options gen2
options stack = 524288
options _dasllama_internal = true

// ATTRIBUTION SWEEP - isolated per-format rates of the decode GEMV classes at one served shape:
// synthetic planes per kq format at [n x d], the shipped kq_gemv class dispatched over enough
// regions of d rows that one dispatch streams STREAM_MB or more (the decode reads a layer's
// planes once per token, so the rate wanted is the DRAM one, not an L2 replay of a small plane),
// timed over repeated submits with a dependency between dispatches. Prints the bytes a dispatch
// reads, its wall per dispatch and the GB/s - the number llama.cpp's per-type mat-vec rates
// (test-backend-ops perf -o MUL_MAT, the m=4096 n=1 k=14336 rows) are read against.
// Args: <n> <d> (default 4096 12288, the Qwen3.5-9B FFN gate; 5120 17408 is the 27B's); `single` dispatches ONE
// plane copy - the served size, L2-warm across the chain - so the row reads the dispatch's own floor past its
// bytes at the streaming rate (the decode's small planes pay it once per GEMV).
// ATTRIBUTION SWEEP - the `cols` mode: the N-column ruler, every format the batched decode step serves (q8 and the
// seventeen kq leaves) at their served lanes, the one-column class beside the N-column class at 1, 2, 4 and 8 activation rows,
// each dispatch's wall attributed to its class and width; the last column is the N-column dispatch's wall over N
// one-column dispatches - under 1 the weight pass is shared, at 1 it is not. The chain alternates two output planes,
// so consecutive dispatches overlap as the batched step's do. The q8 rows engage the row-pair stamps
// (`g_q8_n2_min_n` at 0 for the run); `n2off` holds them to the one-row-a-subgroup form.
// ATTRIBUTION SWEEP - the `cold` mode: ONE plane copy a dispatch, the copies in turn - the served launch size in
// the DRAM regime, the token command's own - each row attributing a format's wall at that launch size; `single`
// replays one copy L2-warm and `cold` yields to it.
// RACE - the `q20` and `q10` modes: a low-bit member's decode GEMV block term and lane split at the served models' shapes.
// `q20`: the shipped `q20_gemv_cls` (signed lanes q - 1, a whole subgroup a row) beside the probe twin `ProbeLowbitGemvT`
// under signed lanes and under unsigned codes with the activation sum folded (`UNS`). `q10`: q20's shipped GEMV beside the
// shipped `q10_gemv_cls` (signed lanes +-1) and the twin under signed lanes, the mask-and-add form (0/1 lanes doubled, the
// activation sum off, `UNS`) and the select form (each activation byte kept or negated by its bit, `SEL`), each twin at the
// whole subgroup, 16 and 8 lanes a row; at the 14336 x 4096 shape both shipped N-column leaves at eight rows. A dispatch is
// one plane copy at the served launch size, the copies in turn (a ring past LB_RING_MB, so every dispatch reads VRAM) - under
// `warm` one copy replayed LB_WARM_CHAIN times, the reference's cache-resident condition - consecutive dispatches
// alternating two output planes; a round submits every arm's chain once, the first LB_WARMUP rounds dropped; every arm's
// first output row is held to its format's shipped one-column arm's at 1e-3 relative.
// RACE - the `iq1` mode: the 1-bit grid members' decode GEMV under the same ring, `warm`, rounds and output alternation - each
// member's shipped leaf (the 4 KB grid staged as it is, `GRID`) at the lanes rule and at 32, 16 and 8 lanes a row; the grid
// lookup's other forms on `Iq1GemvT` - the 4 KB grid read off its buffer (`G1BUF`) or expanded into the nibble form in workgroup
// memory (`GRIDX`, at the rule's lanes and at 8), the reference's 8 KB nibble-form table read off a buffer of its own (`G1NIB` under `G1BUF`); IQ1_S's delta
// as its own term (`G1SEP`); iq2xxs's and q10's shipped leaves beside them, and at the 14336 x 4096 shape both members' N-column
// leaves at eight rows, over the 1B's, the 2560 and 4096 rows', the Qwen1.5-MoE expert's and the reference's shapes.
// RACE - the `tq1lut` mode: tq1's shipped decode GEMV (the word multiply, `tq1_word`) beside `ProbeTq1GemvLut`, each byte's
// digits off a staged 256-entry table, both at the rule's lanes under the same ring, `warm`, rounds and output alternation,
// over Llama-3.2-1B's, TriLM 1.1B's, the Qwen1.5-MoE expert's and the reference's shapes.
require strings
require math
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_vulkan_dispatch   // vk_dispatch: the low-bit race's twin stamps
require daslib/typemacro_boost   // template_struct_instance: the low-bit race's twin template
require dasllama/dasllama_kqformat
require dasllama/dasllama_gemm_schema
require _vk_probe_fixture   // hash_word, cool_f16_pair: the synthetic planes both probes build
require ../tests/_compares.das
require _race_fixture   // time_rounds: the low-bit race's interleaved rounds
require dasllama/dasllama_gpu_kernels_discrete   // q20_byte_lanes, q10_bit_lanes and the recentring words, blk32_scale, tq1_word: the twins' decode
require spirv/spirv_shader

let private DISPATCHES = 20
let private SUBMITS = 5
let private STREAM_MB = 128   //! the least a dispatch streams: four times the 32 MB L2 of the reference card

let private YBIT = 32u    //! the y buffer's hazard bit in the binding order below
let private YBIT2 = 64u   //! the second output plane's, in the `cols` chain

var private g_single_copy = false
var private g_cold = false
var private g_cols = false

//! one probe's synthetic planes and activation rows, uploaded; the dispatch geometry rides beside them
struct private ProbePlanes {
    wq_bytes : int64
    ws_bytes : int64
    xq_bytes : int64
    xs_bytes : int64
    y_bytes : int64
    rg_bytes : int64
    copies : int
    blocks : int     //! weight blocks a plane copy (the region table's stride)
    wgs_all_copies : int64   //! the dispatch grid over every copy's rows (the ring's dispatch)
    wgs_one_copy : int64     //! ... over one copy's rows (a `cold` dispatch's)
    pc : GemvArgs
}

//! the host-side fill of one probe: `wq_words` / `ws_words` of hashed weights, `xq_words` / `xs_words` of activation words
//! and scales over `xrows` rows, `blocks` weight blocks a plane copy (the region table's stride), the copies' regions;
//! `yd2` is the second output plane the `cols` chain alternates with `yd`
def private probe_fill(seed : uint; wq_words, ws_words, xq_words, xs_words, blocks : int; var p : ProbePlanes) : tuple<wqd : uint64; wsd : uint64; xqd : uint64; xsd : uint64; yd : uint64; yd2 : uint64; rgd : uint64> {
    var wqh, wsh, xqh : array<uint>
    var xsh <- row_fill(xs_words, 1, 0, 0.25, -0.5, 7)
    wqh |> reserve(wq_words)
    wqh |> resize(wq_words)
    wsh |> reserve(ws_words)
    wsh |> resize(ws_words)
    xqh |> resize(xq_words)
    for (i in range(wq_words)) {
        wqh[i] = hash_word(uint(i) + seed * 77777u)
    }
    for (i in range(ws_words)) {
        wsh[i] = cool_f16_pair(uint(i) + seed * 13u)
    }
    for (i in range(xq_words)) {
        xqh[i] = hash_word(uint(i) * 31u + 5u)
    }
    p.wq_bytes = int64(wq_words) * 4l
    p.ws_bytes = int64(ws_words) * 4l
    p.xq_bytes = int64(xq_words) * 4l
    p.xs_bytes = int64(xs_words) * 4l
    p.rg_bytes = 8l * int64(p.copies)
    let wqd = make_device_buf(p.wq_bytes)
    let wsd = make_device_buf(p.ws_bytes)
    let xqd = make_device_buf(p.xq_bytes)
    let xsd = make_device_buf(p.xs_bytes)
    let yd = make_device_buf(p.y_bytes)
    let yd2 = g_cols ? make_device_buf(p.y_bytes) : 0ul
    let rgd = make_device_buf(p.rg_bytes)
    var regs <- [for (i in range(p.copies * 2)); (i % 2 == 0) ? uint(i / 2 * blocks) : 0u]   //! region r: its own plane copy, the one x
    unsafe {
        upload_region_at(wqd, 0l, addr<uint8 const?>(wqh[0]), p.wq_bytes)
        upload_region_at(wsd, 0l, addr<uint8 const?>(wsh[0]), p.ws_bytes)
        upload_region_at(xqd, 0l, addr<uint8 const?>(xqh[0]), p.xq_bytes)
        upload_region_at(xsd, 0l, addr<uint8 const?>(xsh[0]), p.xs_bytes)
        upload_region_at(rgd, 0l, addr<uint8 const?>(regs[0]), p.rg_bytes)
    }
    delete wqh
    delete wsh
    delete xqh
    delete xsh
    return (wqd = wqd, wsd = wsd, xqd = xqd, xsd = xsd, yd = yd, yd2 = yd2, rgd = rgd)
}

//! DISPATCHES dependent dispatches of `fmt`'s class (`ncols` 0 = the one-column class) over the planes, timed
//! after a warm-up submit; MB = the weight bytes a dispatch reads. Under `cold` every dispatch is one plane copy
//! at the served launch size, the copies taken in turn so none is L2-warm from the dispatch before. Under `cols`
//! the chain alternates two output planes under their own hazard bits: a dispatch's barrier is the write-after-write
//! against the dispatch two back on its plane (the encoder's own WAW gate), so consecutive dispatches overlap as
//! the batched step's GEMVs do; the one-plane modes chain every dispatch on the one y
def private probe_time(fmt : KqFmt; ncols : int; p : ProbePlanes; b : tuple<wqd : uint64; wsd : uint64; xqd : uint64; xsd : uint64; yd : uint64; yd2 : uint64; rgd : uint64>) : tuple<mb : double; us : double> {
    var sets : array<VkDescriptorSet>
    unsafe {
        let sizes = fixed_array(p.wq_bytes, p.ws_bytes, p.rg_bytes, p.xq_bytes, p.xs_bytes, p.y_bytes)
        let gbits = fixed_array(1u, 2u, 4u, 8u, 16u, YBIT)
        if (g_cold) {   //! a region table a copy, each its own set, so a dispatch names one copy
            sets |> reserve(p.copies)
            for (i in range(p.copies)) {
                var inscope pair <- [uint(i * p.blocks), 0u]
                let rgd = make_device_buf(8l)
                upload_region_at(rgd, 0l, addr<uint8 const?>(pair[0]), 8l)
                let bufs = fixed_array(b.wqd, b.wsd, rgd, b.xqd, b.xsd, b.yd)
                let sz = fixed_array(p.wq_bytes, p.ws_bytes, 8l, p.xq_bytes, p.xs_bytes, p.y_bytes)
                sets |> push(ncols > 0 ? gemv_cls_set_n(int(fmt), bufs, sz, gbits) : gemv_cls_set(int(fmt), bufs, sz, gbits))
                delete pair
            }
        } else {
            let bufs = fixed_array(b.wqd, b.wsd, b.rgd, b.xqd, b.xsd, b.yd)
            sets |> push(ncols > 0 ? gemv_cls_set_n(int(fmt), bufs, sizes, gbits) : gemv_cls_set(int(fmt), bufs, sizes, gbits))
            if (g_cols) {
                let bufs2 = fixed_array(b.wqd, b.wsd, b.rgd, b.xqd, b.xsd, b.yd2)
                let gbits2 = fixed_array(1u, 2u, 4u, 8u, 16u, YBIT2)
                sets |> push(ncols > 0 ? gemv_cls_set_n(int(fmt), bufs2, sizes, gbits2) : gemv_cls_set(int(fmt), bufs2, sizes, gbits2))
            }
        }
    }
    var pc = p.pc
    if (g_cold) {
        pc.nreg = 1u
        pc.ystride = pc.d
    }
    let wgs = g_cold ? p.wgs_one_copy : p.wgs_all_copies
    let us = time_chain(SUBMITS, DISPATCHES, 1) $(raw; var h) {
        for (i in range(DISPATCHES)) {
            var s_cls = sets[i % length(sets)]
            if (ncols > 0) {
                gemv_cls_enc_n(int(fmt), raw, h, s_cls, pc, wgs)
            } else {
                gemv_cls_enc(int(fmt), raw, h, s_cls, pc, wgs)
            }
            if (!g_cols) {
                vhz_dep(raw, h, YBIT, 0u, true)   //! the next dispatch waits on this one's y
            }
        }
    }
    let mb = double(p.wq_bytes + p.ws_bytes) / 1048576.0lf
    return (mb = g_cold ? mb / double(p.copies) : mb, us = us)
}

//! the plane copies a dispatch streams: STREAM_MB or more, or one copy under `single`
def private probe_copies(plane_bytes : int) : int => g_single_copy ? 1 : (STREAM_MB * 1048576 + plane_bytes - 1) / plane_bytes

//! one row of `fmt`'s decode GEMV class at `lanes_per_row` (0 = the whole subgroup) - a kq superblock of 256 or a q8
//! block of 32 int8 quants and an f16 scale (`q8_gemv_cls`, a subgroup a row); `ncols` 0 dispatches the one-column class,
//! else the N-column class over that many activation rows
def private probe_fmt(fmt : KqFmt; n, d : int; lanes_per_row : uint; ncols : int = 0) : tuple<mb : double; us : double> {
    let q8 = fmt == KqFmt.q8
    let bk = q8 ? 32 : 256
    let nb = n / bk
    let nblk = d * nb
    let wq_words = q8 ? 8 : int(kq_dev_qsb(fmt) / 4l)
    let ssb = q8 ? 0 : int(kq_dev_ssb(fmt))
    let plane_bytes = q8 ? nblk * 34 : nblk * (wq_words * 4 + ssb)   // under 2^31 for every served shape (the 27B's largest plane is 89 MB)
    let xrows = max(ncols, 1)
    var p = ProbePlanes(copies = probe_copies(plane_bytes), blocks = nblk)
    p.y_bytes = int64(d) * 4l * int64(p.copies) * int64(xrows)
    p.wgs_all_copies = gemv_wgs(int64(d) * int64(p.copies), lanes_per_row)
    p.wgs_one_copy = gemv_wgs(int64(d), lanes_per_row)
    p.pc = GemvArgs(n = uint(n), d = uint(d), nreg = uint(p.copies), ybase = 0u, lanes_per_row = lanes_per_row,
        ncols = uint(ncols), ystride = uint(d * p.copies))
    let ws_count = q8 ? (nblk * p.copies + 1) / 2 : (nblk * p.copies * ssb + 3) / 4   // whole words at the plane's end (tq2's row is half a word)
    let b = probe_fill(q8 ? 99991u : uint(int(fmt)), nblk * wq_words * p.copies, ws_count, nb * (bk / 4) * xrows, nb * xrows, nblk, p)
    return probe_time(fmt, ncols, p, b)
}

//! one probe row of either class at `ncols` (0 = the one-column class)
def private probe_any(fmt : KqFmt; n, d : int; ncols : int) : tuple<mb : double; us : double> =>
    probe_fmt(fmt, n, d, fmt == KqFmt.q8 ? 0u : gemv_lanes_per_row(int(fmt), int64(n)), ncols)

//! the N-column ruler: the one-column class, then the N-column class at 1, 2, 4 and 8 rows, per format (the q8 rows
//! take the two-rows-a-subgroup stamps past `g_q8_n2_min_n`, which `main` holds at 0 for the run unless `n2off`),
//! every dispatch's output unread - timing only
def private cols_ruler(n, d : int) {
    let fmts <- kq_gemv_fmts()
    for (f in fmts) {
        verify(gemv_cls_ensure(int(f)) && gemv_cls_ensure_n(int(f)), "vk_gemv_probe: the ruler's gemv class rails must engage on a live device")
    }
    let pair_note = g_q8_n2_min_n == 0u ? "the q8 row-pair stamps engaged" : "the q8 rows a row a subgroup (n2off)"
    print("vk_gemv_probe: the N-column ruler at [n {n} x d {d}], {g_gpu.rows_per_wg} rows/wg, {DISPATCHES} dispatches x {SUBMITS} submits after a warm-up over two alternating output planes, {pair_note}; GB/s over the weight bytes; the last column = this dispatch's wall over `cols` one-column dispatches\n")
    print("| fmt    | class | cols |  us/dispatch |    GB/s | vs N single |             |\n")
    print("| :----- | :---- | ---: | -----------: | ------: | ----------: | :---------- |\n")
    let widths = fixed_array(1, 2, 4, 8)
    for (f in fmts) {
        let one = probe_any(f, n, d, 0)
        print("| {f} | one | 1 | {one.us:12.1f} | {one.mb * 1048576.0lf / (one.us * 1000.0lf):7.1f} | {1.0lf:11.2f} | timing-only |\n")
        for (nc in widths) {
            let r = probe_any(f, n, d, nc)
            print("| {f} | ncol | {nc} | {r.us:12.1f} | {r.mb * 1048576.0lf / (r.us * 1000.0lf):7.1f} | {r.us / (one.us * double(nc)):11.2f} | timing-only |\n")
        }
    }
}

let private LB_RING_MB = 96   //! the plane copies an arm walks: three times the 32 MB last-level cache of the reference APU
let private LB_ROUNDS = 8
let private LB_WARMUP = 2
let private LB_WARM_CHAIN = 32   //! under `warm` an arm's chain: this many dispatches over the one plane copy, cache-resident
let private LB_NCOLS = 8         //! the N-column arms' activation rows: the reference's n = 8 row

var private g_lb_warm = false

//! a low-bit decode GEMV's race twin: one row a lane group of `pa.lanes_per_row` lanes (0 = the whole subgroup), a lane a
//! block in turn; the block's codes dotted as signed lanes (q20's q - 1, q10's +-1), under `UNS` as unsigned lanes (q20's
//! codes, q10's 0/1 sign bits doubled) with the activation block's sum folded off the integer dot, and under `SEL` (q10) as the
//! activation bytes kept or negated by the sign bit
[ |> template_struct_instance]
class template ProbeLowbitGemvT {
    @template_constant UNS : bool = false
    @template_constant SEL : bool = false
    @ssbo @binding = 0 wq : array<uint>
    @ssbo @binding = 1 wsu : array<uint>
    @ssbo @binding = 2 regions : array<uint>
    @ssbo @binding = 3 xqw : array<uint>
    @ssbo @binding = 4 axs : array<float>
    @ssbo @binding = 5 y : array<float>
    @push_constant pa : GemvArgs

    [spirv_inline]
    def body {
        let lpr = pa.lanes_per_row == 0u ? gl_SubgroupSize : pa.lanes_per_row
        let lane = gl_SubgroupInvocationID
        let rl = lane % lpr
        let rg = (gl_WorkGroupID.x * gl_NumSubgroups + gl_SubgroupID) * (gl_SubgroupSize / lpr) + lane / lpr
        let live = rg < pa.nreg * pa.d
        let nreg = max(pa.nreg, 1u)
        let r = rg % nreg
        let row = rg / nreg
        var acc = 0.0
        if (live) {
            let nb = pa.n / 32u
            let wb0 = regions[r * 2u] + row * nb
            let xb0 = regions[r * 2u + 1u]
            var b = rl
            while (b < nb) {
                let wb = wb0 + b
                let xbase = (xb0 + b) * 8u
                var idot = 0
                static_if (typeinfo typename(type<FMT>) == typeinfo typename(type<KqTag_q20>)) {
                    let w0 = wq[wb * 2u]
                    let w1 = wq[wb * 2u + 1u]
                    for [unroll_full] (c in range(4)) {
                        let ul = q20_byte_lanes(w0 >> (uint(c) * 8u))
                        let uh = q20_byte_lanes(w1 >> (uint(c) * 8u))
                        let xl = xqw[xbase + uint(c)]
                        let xh = xqw[xbase + 4u + uint(c)]
                        static_if (UNS) {
                            idot += sdot4(ul, xl) + sdot4(uh, xh) - sdot4(xl, 0x01010101u) - sdot4(xh, 0x01010101u)
                        } else {
                            idot += sdot4(q20_signed_word(ul), xl) + sdot4(q20_signed_word(uh), xh)
                        }
                    }
                } else {
                    let w = wq[wb]
                    for [unroll_full] (c in range(8)) {
                        let xw = xqw[xbase + uint(c)]
                        let bits = w >> (uint(c) * 4u)
                        static_if (UNS) {
                            idot += 2 * sdot4(q10_bit_lanes(bits), xw) - sdot4(xw, 0x01010101u)
                        } static_elif (SEL) {
                            let xi = int4(unpack8(int(xw)))
                            idot += (((bits & 1u) != 0u ? xi.x : -xi.x) + ((bits & 2u) != 0u ? xi.y : -xi.y)
                                + ((bits & 4u) != 0u ? xi.z : -xi.z) + ((bits & 8u) != 0u ? xi.w : -xi.w))
                        } else {
                            idot += sdot4(q10_signed_word(q10_bit_lanes(bits)), xw)
                        }
                    }
                }
                let dv = blk32_scale(type<FMT>, wsu[blk32_sword(type<FMT>, wb)], wb).x
                acc = mad(dv * float(idot), axs[xb0 + b], acc)
                b += lpr
            }
        }
        var total = acc
        if (lpr == 8u) {
            total += subgroupShuffleXor(total, 4u)
            total += subgroupShuffleXor(total, 2u)
            total += subgroupShuffleXor(total, 1u)
        } elif (lpr == 16u) {
            total += subgroupShuffleXor(total, 8u)
            total += subgroupShuffleXor(total, 4u)
            total += subgroupShuffleXor(total, 2u)
            total += subgroupShuffleXor(total, 1u)
        } else {
            total = subgroupAdd(acc)
        }
        if (live && rl == 0u) {
            y[pa.ybase + r * pa.d + row] = total
        }
    }
}

[vk_dispatch(name = "probe_q20_gemv_s_cls", grid = "wgs", params = "wgs : int64")]
class ProbeQ20GemvS : ProbeLowbitGemvT {
    typedef FMT = KqTag_q20

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        body()
    }
}

[vk_dispatch(name = "probe_q20_gemv_u_cls", grid = "wgs", params = "wgs : int64")]
class ProbeQ20GemvU : ProbeLowbitGemvT {
    typedef FMT = KqTag_q20
    override UNS = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        body()
    }
}

[vk_dispatch(name = "probe_q10_gemv_s_cls", grid = "wgs", params = "wgs : int64")]
class ProbeQ10GemvS : ProbeLowbitGemvT {
    typedef FMT = KqTag_q10

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        body()
    }
}

[vk_dispatch(name = "probe_q10_gemv_u_cls", grid = "wgs", params = "wgs : int64")]
class ProbeQ10GemvU : ProbeLowbitGemvT {
    typedef FMT = KqTag_q10
    override UNS = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        body()
    }
}

[vk_dispatch(name = "probe_q10_gemv_x_cls", grid = "wgs", params = "wgs : int64")]
class ProbeQ10GemvX : ProbeLowbitGemvT {
    typedef FMT = KqTag_q10
    override SEL = true

    [spirv_kernel(local_size_x = 64, local_size_x_id = 0)]
    def run {
        body()
    }
}

//! `Iq1GemvT`'s IQ1_S leaf with the 4 KB grid read off its buffer, no workgroup copy
[vk_dispatch(name = "probe_iq1s_gemv_buf_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class ProbeIq1sGemvBuf : Iq1GemvT {
    override GRID = false
    override G1BUF = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

//! `Iq1GemvT`'s IQ1_S leaf over the nibble-form table read off its own 8 KB buffer, the reference's table (the probe binds it in the grid's slot)
[vk_dispatch(name = "probe_iq1s_gemv_nibb_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class ProbeIq1sGemvNibB : Iq1GemvT {
    override GRID = false
    override G1NIB = true
    override G1BUF = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

//! `Iq1GemvT`'s IQ1_S leaf with the 4 KB grid expanded into the nibble form in workgroup memory (`GRIDX`, `stage_gridx`)
[vk_dispatch(name = "probe_iq1s_gemv_nibs_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class ProbeIq1sGemvNibS : Iq1GemvT {
    override GRID = false
    override GRIDX = true
    override G1NIB = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

//! `Iq1GemvT`'s IQ1_S leaf with the delta as its own term: the codes as lanes, d x (2s + 1) x (delta - 1) against the half's sum
[vk_dispatch(name = "probe_iq1s_gemv_sep_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class ProbeIq1sGemvSep : Iq1GemvT {
    override G1SEP = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

//! `Iq1GemvT`'s IQ1_M leaf with the 4 KB grid read off its buffer
[vk_dispatch(name = "probe_iq1m_gemv_buf_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class ProbeIq1mGemvBuf : Iq1GemvT {
    override IQ1M = true
    override GRID = false
    override G1BUF = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

//! `Iq1GemvT`'s IQ1_M leaf over the nibble-form table read off its own 8 KB buffer
[vk_dispatch(name = "probe_iq1m_gemv_nibb_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class ProbeIq1mGemvNibB : Iq1GemvT {
    override IQ1M = true
    override GRID = false
    override G1NIB = true
    override G1BUF = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

//! `Iq1GemvT`'s IQ1_M leaf with the 4 KB grid expanded into the nibble form in workgroup memory
[vk_dispatch(name = "probe_iq1m_gemv_nibs_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class ProbeIq1mGemvNibS : Iq1GemvT {
    override IQ1M = true
    override GRID = false
    override GRIDX = true
    override G1NIB = true

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

//! a byte's five TQ1_0 digits, two bits each, digit k at bits 2k (`tq1_word` over the byte alone)
def private tq1_byte_digits(b : uint) : uint =>
    (tq1_word(b, 0u) & 3u) | ((tq1_word(b, 1u) & 3u) << 2u) | ((tq1_word(b, 2u) & 3u) << 4u) | ((tq1_word(b, 3u) & 3u) << 6u) | ((tq1_word(b, 4u) & 3u) << 8u)

//! `K2GemvT`'s TQ1_0 leaf with each byte's digits off a 256-entry table staged into workgroup memory ahead of the shell
//! (`tq1_byte_digits`, through `stage_gridx`) - the digit extraction's other candidate against the shipped word multiply
[vk_dispatch(name = "probe_tq1_gemv_lut_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class ProbeTq1GemvLut : K2GemvT {
    override TQ2 = true
    override TQ1 = true
    override GRIDX = true
    @workgroup tq1lut : uint[256]

    def override stage_gridx {
        let lid = gl_LocalInvocationID.x
        for (k in range(4)) {
            tq1lut[lid + uint(k) * 64u] = tq1_byte_digits(lid + uint(k) * 64u)
        }
        barrier()
    }

    //! digit `n` of each of word `w`'s four bytes off the table, a byte lane each
    def lut_lanes(w, n : uint) : uint =>
        ((tq1lut[w & 255u] >> (2u * n)) & 3u) | (((tq1lut[(w >> 8u) & 255u] >> (2u * n)) & 3u) << 8u) | (((tq1lut[(w >> 16u) & 255u] >> (2u * n)) & 3u) << 16u) | (((tq1lut[w >> 24u] >> (2u * n)) & 3u) << 24u)

    def override tq1_dw(i, n : uint) : uint => lut_lanes(wq[i], n)

    [spirv_kernel(local_size_x = 64)]
    def run {
        leaf_body()
    }
}

let private G1_RULE = 0xFFFFFFFFu   //! an arm's lanes: the family's rule (`gemv_lanes_per_row`)

//! a race arm's kernel: the format's shipped leaf, or a probe twin - `ProbeLowbitGemvT`'s signed lanes, unsigned codes (q10's
//! mask-and-add) and select forms; `Iq1GemvT`'s 4 KB grid off its buffer, the 8 KB nibble-form table off a buffer of its own,
//! the grid expanded into the nibble form in workgroup memory and IQ1_S's delta as its own term; tq1's staged digit table
enum private RaceKernel {
    shipped
    signed
    unsigned
    select
    grid_buf
    nibble_buf
    nibble_expand
    delta_term
    digit_table
}

//! a race arm: its name, format, kernel, lanes a row (`G1_RULE` the family's rule) and activation rows (1, or `LB_NCOLS` on
//! the shipped N-column leaf)
struct private RaceArm {
    name : string
    fmt : KqFmt
    kernel : RaceKernel
    lanes : uint
    ncols : int
}

def private lb_twins(fmt : KqFmt; kernel : RaceKernel; tag : string) : array<RaceArm> {
    return <- [RaceArm(name = "{fmt} twin {tag} x32", fmt = fmt, kernel = kernel, lanes = 0u, ncols = 1),
        RaceArm(name = "{fmt} twin {tag} x16", fmt = fmt, kernel = kernel, lanes = 16u, ncols = 1),
        RaceArm(name = "{fmt} twin {tag} x8 ", fmt = fmt, kernel = kernel, lanes = 8u, ncols = 1)]
}

//! the arms of a mode: under `q20` q20's shipped GEMV and its signed and unsigned twins; under `q10` q20's shipped GEMV beside
//! q10's and q10's signed, mask-and-add and select twins; at the reference shape both shipped N-column leaves at `LB_NCOLS`
def private lb_arms(q10 : bool; with_ncol : bool) : array<RaceArm> {
    var arms <- [RaceArm(name = "q20 shipped      x32", fmt = KqFmt.q20, kernel = RaceKernel.shipped, lanes = 0u, ncols = 1)]
    if (q10) {
        arms |> push(RaceArm(name = "q10 shipped      x32", fmt = KqFmt.q10, kernel = RaceKernel.shipped, lanes = 0u, ncols = 1))
        arms |> push_from(lb_twins(KqFmt.q10, RaceKernel.signed, "signed  "), lb_twins(KqFmt.q10, RaceKernel.unsigned, "maskadd "),
            lb_twins(KqFmt.q10, RaceKernel.select, "select  "))
    } else {
        arms |> push_from(lb_twins(KqFmt.q20, RaceKernel.signed, "signed  "), lb_twins(KqFmt.q20, RaceKernel.unsigned, "unsigned"))
    }
    if (with_ncol) {
        arms |> push(RaceArm(name = "q20 shipped n{LB_NCOLS}   x32", fmt = KqFmt.q20, kernel = RaceKernel.shipped, lanes = 0u, ncols = LB_NCOLS))
        if (q10) {
            arms |> push(RaceArm(name = "q10 shipped n{LB_NCOLS}   x32", fmt = KqFmt.q10, kernel = RaceKernel.shipped, lanes = 0u, ncols = LB_NCOLS))
        }
    }
    return <- arms
}

//! the arms of the `iq1` mode at a shape: each member's shipped leaf at the rule's lanes and at the whole subgroup, 16 and 8,
//! its lookup twins at the rule's lanes (IQ1_S's delta twin too), iq2xxs's and q10's shipped leaves, and at the reference
//! shape each member's N-column leaf at `LB_NCOLS` rows
def private g1_arms(with_ncol : bool) : array<RaceArm> {
    var arms : array<RaceArm>
    arms |> reserve(24)
    for (f in [KqFmt.iq1s, KqFmt.iq1m]) {
        arms |> push(RaceArm(name = "{f} shipped rule  ", fmt = f, kernel = RaceKernel.shipped, lanes = G1_RULE, ncols = 1))
        for (l in fixed_array(0u, 16u, 8u)) {
            arms |> push(RaceArm(name = "{f} shipped x{l == 0u ? 32 : int(l)}{l == 8u ? " " : ""}   ", fmt = f, kernel = RaceKernel.shipped, lanes = l, ncols = 1))
        }
        arms |> push(RaceArm(name = "{f} packed buffer ", fmt = f, kernel = RaceKernel.grid_buf, lanes = G1_RULE, ncols = 1))
        arms |> push(RaceArm(name = "{f} nibble buffer ", fmt = f, kernel = RaceKernel.nibble_buf, lanes = G1_RULE, ncols = 1))
        arms |> push(RaceArm(name = "{f} nibble expand ", fmt = f, kernel = RaceKernel.nibble_expand, lanes = G1_RULE, ncols = 1))
        arms |> push(RaceArm(name = "{f} nibble exp x8 ", fmt = f, kernel = RaceKernel.nibble_expand, lanes = 8u, ncols = 1))
        if (f == KqFmt.iq1s) {
            arms |> push(RaceArm(name = "{f} delta term    ", fmt = f, kernel = RaceKernel.delta_term, lanes = G1_RULE, ncols = 1))
        }
    }
    arms |> push(RaceArm(name = "iq2xxs shipped rule", fmt = KqFmt.iq2xxs, kernel = RaceKernel.shipped, lanes = G1_RULE, ncols = 1))
    arms |> push(RaceArm(name = "q10 shipped x32    ", fmt = KqFmt.q10, kernel = RaceKernel.shipped, lanes = 0u, ncols = 1))
    if (with_ncol) {
        arms |> push(RaceArm(name = "iq1s shipped n{LB_NCOLS}    ", fmt = KqFmt.iq1s, kernel = RaceKernel.shipped, lanes = G1_RULE, ncols = LB_NCOLS))
        arms |> push(RaceArm(name = "iq1m shipped n{LB_NCOLS}    ", fmt = KqFmt.iq1m, kernel = RaceKernel.shipped, lanes = G1_RULE, ncols = LB_NCOLS))
    }
    return <- arms
}

//! the arms of the `tq1lut` mode: tq1's shipped leaf (the word multiply) and its digit-table twin, both at the rule's lanes
def private tq1lut_arms() : array<RaceArm> {
    return <- [RaceArm(name = "tq1 multiply rule", fmt = KqFmt.tq1, kernel = RaceKernel.shipped, lanes = G1_RULE, ncols = 1),
        RaceArm(name = "tq1 table rule   ", fmt = KqFmt.tq1, kernel = RaceKernel.digit_table, lanes = G1_RULE, ncols = 1)]
}

//! a race shape's device planes of one format: the ring's weight and scale words (one copy under `warm`), `LB_NCOLS`
//! activation rows, two outputs of `LB_NCOLS` rows, a region pair a copy
struct private LbPlanes {
    wqd, wsd, xqd, xsd : uint64
    ys : uint64[2]
    rgs : array<uint64>
    sizes : int64[6]
    copies : int
    chain : int   //! the dispatches an arm's chain issues: a copy each, or `LB_WARM_CHAIN` over the one copy
}

//! the activation rows, the outputs, the region pairs and the sizes of planes over `units` weight units a copy
def private lb_finish(var p : LbPlanes; var wqh, wsh, xqh : array<uint>; var xsh : array<float>; d, units : int) {
    p.wqd = dev_upload(wqh)
    p.wsd = dev_upload(wsh)
    p.xqd = dev_upload(xqh)
    p.xsd = dev_upload(xsh)
    p.ys[0] = make_device_buf(int64(LB_NCOLS * d) * 4l)
    p.ys[1] = make_device_buf(int64(LB_NCOLS * d) * 4l)
    p.rgs |> reserve(p.copies)
    for (c in range(p.copies)) {
        var inscope pair <- [uint(c * units), 0u]
        p.rgs |> push(dev_upload(pair))
    }
    p.sizes = fixed_array(long_length(wqh) * 4l, long_length(wsh) * 4l, 8l, long_length(xqh) * 4l, long_length(xsh) * 4l, int64(LB_NCOLS * d) * 4l)
}

//! the planes of a per-32 member
def private lb_planes(fmt : KqFmt; n, d : int) : LbPlanes {
    let nb = n / 32
    let blocks = d * nb
    let copy_bytes = int(int64(blocks) * kq_qsb(fmt) + kq_sbytes(fmt, int64(blocks)))
    let copies0 = max(2, LB_RING_MB * 1048576 / max(1, copy_bytes))
    let copies = g_lb_warm ? 1 : min(copies0 + (copies0 & 1), 512)
    let qwords = int(kq_qsb(fmt) / 4l)
    let wq_words = blocks * qwords * copies
    let ws_words = (blocks / int(kq_sgroup(fmt)) / 2 + 1) * copies
    var p = LbPlanes(copies = copies, chain = g_lb_warm ? LB_WARM_CHAIN : copies)
    var inscope wqh, wsh, xqh : array<uint>
    wqh |> reserve(wq_words)
    wqh |> resize(wq_words)
    for (i in range(wq_words)) {
        wqh[i] = hash_word(uint(i) + 4441u)
    }
    wsh |> reserve(ws_words)
    wsh |> resize(ws_words)
    for (i in range(ws_words)) {
        wsh[i] = cool_f16_pair(uint(i) + 29u)
    }
    xqh |> resize(LB_NCOLS * nb * 8)
    for (i in range(LB_NCOLS * nb * 8)) {
        xqh[i] = hash_word(uint(i) * 31u + 5u)
    }
    var inscope xsh <- row_fill(LB_NCOLS * nb, 1, 0, 0.25, -0.5, 7)
    lb_finish(p, wqh, wsh, xqh, xsh, d, blocks)
    return <- p
}

//! the planes of a superblock member: its device rows, `LB_NCOLS` Q8_K activation rows
def private sb_planes(fmt : KqFmt; n, d : int) : LbPlanes {
    let nsb = n / 256
    let units = d * nsb
    let qwords = int(kq_dev_qsb(fmt) / 4l)
    let swords = int((int64(units) * kq_dev_ssb(fmt) + 3l) / 4l)
    let copies0 = max(2, LB_RING_MB * 1048576 / max(1, (units * qwords + swords) * 4))
    let copies = g_lb_warm ? 1 : min(copies0 + (copies0 & 1), 512)
    var p = LbPlanes(copies = copies, chain = g_lb_warm ? LB_WARM_CHAIN : copies)
    var inscope wqh, wsh, xqh : array<uint>
    wqh |> reserve(units * qwords * copies)
    wqh |> resize(units * qwords * copies)
    for (i in range(length(wqh))) {
        wqh[i] = hash_word(uint(i) + 4441u)
    }
    wsh |> reserve(swords * copies)
    wsh |> resize(swords * copies)
    for (i in range(length(wsh))) {
        wsh[i] = cool_f16_pair(uint(i % swords) + 29u)
    }
    xqh |> resize(LB_NCOLS * nsb * 64)
    for (i in range(length(xqh))) {
        xqh[i] = hash_word(uint(i) * 31u + 5u)
    }
    var inscope xsh <- row_fill(LB_NCOLS * nsb, 1, 0, 0.25, -0.5, 7)
    lb_finish(p, wqh, wsh, xqh, xsh, d, units)
    return <- p
}

//! a format's race planes: the per-32 members' blocks, the superblock members' device rows
def private race_planes(fmt : KqFmt; n, d : int) : LbPlanes {
    return <- lb_planes(fmt, n, d) if (kq_dense_b32(fmt))
    return <- sb_planes(fmt, n, d)
}

//! the reference's 8 KB 1-bit table: every entry in the nibble form (`grid1_nibbles`), a word each
def private g1_nibble_table() : array<uint> => <- [for (e in range(2048)); grid1_nibbles(iq1_grid_entry(IQ1S_GRID, e))]

//! a twin's set of the 7-binding shell (`KqGemvBase`): the planes with the grid buffer, or on the nibble-buffer twins the
//! nibble table `nib`, in the grid's slot
def private twin7_set(a : RaceArm; bufs : uint64 const[6]; sizes : int64 const[6]; bits : uint const[6]; nib : uint64) : VkDescriptorSet {
    let grid = a.kernel == RaceKernel.nibble_buf ? nib : kq_grid_dev()
    let grid_bytes = a.kernel == RaceKernel.nibble_buf ? 2048l * 4l : KQ_GRID_BYTES
    let b7 = fixed_array(bufs[0], bufs[1], bufs[2], bufs[3], bufs[4], bufs[5], grid)
    let s7 = fixed_array(sizes[0], sizes[1], sizes[2], sizes[3], sizes[4], sizes[5], grid_bytes)
    let h7 = fixed_array(bits[0], bits[1], bits[2], bits[3], bits[4], bits[5], 0u)
    let m = a.fmt == KqFmt.iq1m
    if (a.kernel == RaceKernel.grid_buf) {
        return m ? set_probe_iq1m_gemv_buf_cls(b7, s7, h7) : set_probe_iq1s_gemv_buf_cls(b7, s7, h7)
    } elif (a.kernel == RaceKernel.nibble_buf) {
        return m ? set_probe_iq1m_gemv_nibb_cls(b7, s7, h7) : set_probe_iq1s_gemv_nibb_cls(b7, s7, h7)
    } elif (a.kernel == RaceKernel.nibble_expand) {
        return m ? set_probe_iq1m_gemv_nibs_cls(b7, s7, h7) : set_probe_iq1s_gemv_nibs_cls(b7, s7, h7)
    } elif (a.kernel == RaceKernel.digit_table) {
        return set_probe_tq1_gemv_lut_cls(b7, s7, h7)
    }
    return set_probe_iq1s_gemv_sep_cls(b7, s7, h7)
}

//! arm `a`'s set over chain dispatch `c`: copy `c % copies` of its format's ring, its output the dispatch's parity's plane
def private race_copy_set(a : RaceArm; p : LbPlanes; c : int; nib : uint64) : VkDescriptorSet {
    let bufs = fixed_array(p.wqd, p.wsd, p.rgs[c % p.copies], p.xqd, p.xsd, p.ys[c % 2])
    let bits = fixed_array(1u, 2u, 4u, 8u, 16u, c % 2 == 0 ? 32u : 64u)
    if (a.kernel == RaceKernel.shipped) {
        return gemv_cls_set(int(a.fmt), bufs, p.sizes, bits)
    } elif (a.kernel == RaceKernel.signed) {
        return a.fmt == KqFmt.q20 ? set_probe_q20_gemv_s_cls(bufs, p.sizes, bits) : set_probe_q10_gemv_s_cls(bufs, p.sizes, bits)
    } elif (a.kernel == RaceKernel.unsigned) {
        return a.fmt == KqFmt.q20 ? set_probe_q20_gemv_u_cls(bufs, p.sizes, bits) : set_probe_q10_gemv_u_cls(bufs, p.sizes, bits)
    } elif (a.kernel == RaceKernel.select) {
        return set_probe_q10_gemv_x_cls(bufs, p.sizes, bits)
    }
    return twin7_set(a, bufs, p.sizes, bits, nib)
}

//! a twin's encode
def private twin_enc(a : RaceArm; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : GemvArgs; wgs : int64) {
    let m = a.fmt == KqFmt.iq1m
    let q20 = a.fmt == KqFmt.q20
    if (a.kernel == RaceKernel.signed) {
        if (q20) {
            enc_probe_q20_gemv_s_cls(raw, h, s, pc, wgs)
        } else {
            enc_probe_q10_gemv_s_cls(raw, h, s, pc, wgs)
        }
    } elif (a.kernel == RaceKernel.unsigned) {
        if (q20) {
            enc_probe_q20_gemv_u_cls(raw, h, s, pc, wgs)
        } else {
            enc_probe_q10_gemv_u_cls(raw, h, s, pc, wgs)
        }
    } elif (a.kernel == RaceKernel.select) {
        enc_probe_q10_gemv_x_cls(raw, h, s, pc, wgs)
    } elif (a.kernel == RaceKernel.grid_buf) {
        if (m) {
            enc_probe_iq1m_gemv_buf_cls(raw, h, s, pc, wgs)
        } else {
            enc_probe_iq1s_gemv_buf_cls(raw, h, s, pc, wgs)
        }
    } elif (a.kernel == RaceKernel.nibble_buf) {
        if (m) {
            enc_probe_iq1m_gemv_nibb_cls(raw, h, s, pc, wgs)
        } else {
            enc_probe_iq1s_gemv_nibb_cls(raw, h, s, pc, wgs)
        }
    } elif (a.kernel == RaceKernel.nibble_expand) {
        if (m) {
            enc_probe_iq1m_gemv_nibs_cls(raw, h, s, pc, wgs)
        } else {
            enc_probe_iq1s_gemv_nibs_cls(raw, h, s, pc, wgs)
        }
    } elif (a.kernel == RaceKernel.digit_table) {
        enc_probe_tq1_gemv_lut_cls(raw, h, s, pc, wgs)
    } else {
        enc_probe_iq1s_gemv_sep_cls(raw, h, s, pc, wgs)
    }
}

def private race_arm_enc(a : RaceArm; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : GemvArgs; d : int) {
    pc.lanes_per_row = a.lanes == G1_RULE ? gemv_lanes_per_row(int(a.fmt), int64(pc.n)) : a.lanes
    let wgs = gemv_wgs(int64(d), pc.lanes_per_row)
    if (a.kernel != RaceKernel.shipped) {
        twin_enc(a, raw, h, s, pc, wgs)
    } elif (a.ncols > 1) {
        pc.ncols = uint(a.ncols)
        pc.ystride = uint(d)
        gemv_cls_enc_n(int(a.fmt), raw, h, s, pc, wgs)
    } else {
        gemv_cls_enc(int(a.fmt), raw, h, s, pc, wgs)
    }
}

//! one shape of a race: every arm a chain of dispatches over its format's planes (`race_planes`, a format's in the arms'
//! order), a round every arm once; prints each arm's best and median round in microseconds a dispatch, its round-to-round
//! spread, and its first output row's largest relative distance from its format's first arm - its shipped one-column arm -
//! over copy 0
def private race_shape(name : string; n, d : int; arms : array<RaceArm>; nib : uint64) {
    var fmts : array<KqFmt>   // nolint:STYLE027 — the arms' formats first-seen, a dedup no comprehension filter expresses
    for (a in arms) {
        if (find_index(fmts, a.fmt) < 0) {
            fmts |> push(a.fmt)
        }
    }
    var ps : array<LbPlanes>
    ps |> reserve(length(fmts))
    for (f in fmts) {
        ps |> emplace(race_planes(f, n, d))
    }
    var raws : array<VkCommandBuffer>
    raws |> reserve(length(arms))
    for (a in arms) {
        let pi = find_index(fmts, a.fmt)
        var sets <- [for (c in range(ps[pi].chain)); race_copy_set(a, ps[pi], c, nib)]
        var raw = alloc_cmd()
        record_cmd(raw, false) $(r; var h) {
            for (c in range(ps[pi].chain)) {
                var pc = GemvArgs(n = uint(n), d = uint(d), nreg = 1u, ybase = 0u)
                race_arm_enc(a, r, h, sets[c], pc, d)
            }
        }
        raws |> push(raw)
        unsafe {
            delete sets
        }
    }
    var times <- time_rounds(length(arms), LB_ROUNDS, LB_WARMUP) $(i : int) {
        submit_wait(raws[i])
    }
    var refs : array<array<float>>
    refs |> resize(length(fmts))
    for (a, i in arms, count()) {
        let pi = find_index(fmts, a.fmt)
        var one = alloc_cmd()
        var s0 = race_copy_set(a, ps[pi], 0, nib)
        record_cmd(one, false) $(r; var h) {
            var pc = GemvArgs(n = uint(n), d = uint(d), nreg = 1u, ybase = 0u)
            race_arm_enc(a, r, h, s0, pc, d)
        }
        submit_wait(one)
        var inscope got <- readback_floats(ps[pi].ys[0], int64(d) * 4l)
        if (empty(refs[pi])) {
            refs[pi] := got
        }
        var dmax = 0.0
        for (g, w in got, refs[pi]) {
            dmax = max(dmax, abs(g - w) / max(1e-3, abs(w)))
        }
        var inscope sorted := times[i]
        sort(sorted)
        let per = double(ps[pi].chain)
        print("{name} {a.name}: best {double(sorted[0]) / per:8.2f} us  median {double(sorted[length(sorted) / 2]) / per:8.2f} us  spread {double(sorted[length(sorted) - 1] - sorted[0]) / per:6.2f} us  max rel vs shipped {dmax:.2e}  ({ps[pi].copies} copies, {ps[pi].chain} a chain)\n")
    }
    delete refs
    delete times
}

//! the race's shapes: the served models' decode GEMVs (n wide, d rows) - the 1.7B Bonsai files, Llama-3.2-1B, the Qwen1.5-MoE
//! expert stacks (2048 x 1408 a stack), Flash-Next's 2560-wide experts, and the reference's 4096 x 14336 row (the N-column
//! arms there alone)
def private lb_race(q10 : bool) {
    let fmt = q10 ? "q10" : "q20"
    verify(gemv_cls_ensure(int(KqFmt.q20)) && ensure_probe_q20_gemv_s_cls() && ensure_probe_q20_gemv_u_cls() && gemv_cls_ensure_n(int(KqFmt.q20)), "vk_gemv_probe: the low-bit race's q20 class rails must engage on a live device")
    if (q10) {
        verify(gemv_cls_ensure(int(KqFmt.q10)) && gemv_cls_ensure_n(int(KqFmt.q10)) && ensure_probe_q10_gemv_s_cls() && ensure_probe_q10_gemv_u_cls() && ensure_probe_q10_gemv_x_cls(),
            "vk_gemv_probe: the q10 race's class rails must engage on a live device")
    }
    let planes = g_lb_warm ? "one plane copy replayed {LB_WARM_CHAIN} times a chain (cache-resident)" : "a ring of copies past {LB_RING_MB} MB"
    print("vk_gemv_probe: the {fmt} decode GEMV race, subgroup {g_gpu.subgroup_size}, {g_gpu.rows_per_wg} rows/wg, {LB_ROUNDS} rounds after {LB_WARMUP} warm-up, {planes}\n")
    for (sh in fixed_array((name = "bonsai q/o   2048x2048 ", n = 2048, d = 2048), (name = "bonsai kv    2048x1024 ", n = 2048, d = 1024),
            (name = "bonsai up    2048x6144 ", n = 2048, d = 6144), (name = "bonsai down  6144x2048 ", n = 6144, d = 2048),
            (name = "llama up     2048x8192 ", n = 2048, d = 8192), (name = "llama down   8192x2048 ", n = 8192, d = 2048),
            (name = "moe e-up     2048x1408 ", n = 2048, d = 1408), (name = "moe e-down   1408x2048 ", n = 1408, d = 2048),
            (name = "fnext e-up   2560x640  ", n = 2560, d = 640), (name = "fnext e-down 640x2560  ", n = 640, d = 2560),
            (name = "ref          14336x4096", n = 14336, d = 4096))) {
        var inscope arms <- lb_arms(q10, sh.n == 14336)
        race_shape(sh.name, sh.n, sh.d, arms, 0ul)
    }
}

//! the `iq1` race's shapes: the 1B's projections (2048 wide, 2048 and 8192 rows; its down 8192 wide), rows of 2560 and 4096, the
//! Qwen1.5-MoE expert's gate and up (2048 x 1408) and the reference's 14336 x 4096 row
def private g1_race {
    verify(gemv_cls_ensure(int(KqFmt.iq1s)) && gemv_cls_ensure(int(KqFmt.iq1m)) && gemv_cls_ensure(int(KqFmt.iq2xxs)) && gemv_cls_ensure(int(KqFmt.q10))
        && gemv_cls_ensure_n(int(KqFmt.iq1s)) && gemv_cls_ensure_n(int(KqFmt.iq1m)), "vk_gemv_probe: the iq1 race's shipped rails must engage on a live device")
    verify(ensure_probe_iq1s_gemv_buf_cls() && ensure_probe_iq1s_gemv_nibb_cls() && ensure_probe_iq1s_gemv_nibs_cls() && ensure_probe_iq1s_gemv_sep_cls()
        && ensure_probe_iq1m_gemv_buf_cls() && ensure_probe_iq1m_gemv_nibb_cls() && ensure_probe_iq1m_gemv_nibs_cls(), "vk_gemv_probe: the iq1 twins must build")
    var inscope tab <- g1_nibble_table()
    let nib = dev_upload(tab)
    let planes = g_lb_warm ? "one plane copy replayed {LB_WARM_CHAIN} times a chain (cache-resident)" : "a ring of copies past {LB_RING_MB} MB"
    print("vk_gemv_probe: the iq1 decode GEMV race, subgroup {g_gpu.subgroup_size}, {g_gpu.rows_per_wg} rows/wg, {LB_ROUNDS} rounds after {LB_WARMUP} warm-up, {planes}\n")
    for (sh in fixed_array((name = "1b q/o      2048x2048 ", n = 2048, d = 2048), (name = "1b up       2048x8192 ", n = 2048, d = 8192),
            (name = "1b down     8192x2048 ", n = 8192, d = 2048), (name = "rows 2560   2560x2560 ", n = 2560, d = 2560),
            (name = "rows 4096   4096x4096 ", n = 4096, d = 4096), (name = "moe e-up    2048x1408 ", n = 2048, d = 1408),
            (name = "ref         14336x4096", n = 14336, d = 4096))) {
        var inscope arms <- g1_arms(sh.n == 14336)
        race_shape(sh.name, sh.n, sh.d, arms, nib)
    }
}

//! the `tq1lut` race's shapes: Llama-3.2-1B's and TriLM 1.1B's projections, the Qwen1.5-MoE expert's gate and up and the
//! reference's 14336 x 4096 row
def private tq1lut_race {
    verify(gemv_cls_ensure(int(KqFmt.tq1)) && ensure_probe_tq1_gemv_lut_cls(), "vk_gemv_probe: the tq1lut race's rails must engage on a live device")
    let planes = g_lb_warm ? "one plane copy replayed {LB_WARM_CHAIN} times a chain (cache-resident)" : "a ring of copies past {LB_RING_MB} MB"
    print("vk_gemv_probe: the tq1 digit-extraction race, subgroup {g_gpu.subgroup_size}, {g_gpu.rows_per_wg} rows/wg, {LB_ROUNDS} rounds after {LB_WARMUP} warm-up, {planes}\n")
    for (sh in fixed_array((name = "1b q/o      2048x2048 ", n = 2048, d = 2048), (name = "1b up       2048x8192 ", n = 2048, d = 8192),
            (name = "1b down     8192x2048 ", n = 8192, d = 2048), (name = "1b kv       2048x512  ", n = 2048, d = 512),
            (name = "trilm up    2048x5632 ", n = 2048, d = 5632), (name = "trilm down  5632x2048 ", n = 5632, d = 2048),
            (name = "moe e-up    2048x1408 ", n = 2048, d = 1408), (name = "ref         14336x4096", n = 14336, d = 4096))) {
        var inscope arms <- tq1lut_arms()
        race_shape(sh.name, sh.n, sh.d, arms, 0ul)
    }
}

[export]
def main : int {
    let args <- get_command_line_arguments()
    var n = 4096
    var d = 12288
    let shape_args <- [for (a in args); a; where to_int(a) > 0]
    if (length(shape_args) >= 2) {
        n = to_int(shape_args[length(shape_args) - 2])
        d = to_int(shape_args[length(shape_args) - 1])
    }
    g_single_copy = find_index(args, "single") >= 0
    g_cold = find_index(args, "cold") >= 0 && !g_single_copy
    g_cols = find_index(args, "cols") >= 0
    if (find_index(args, "q20") >= 0 || find_index(args, "q10") >= 0 || find_index(args, "iq1") >= 0 || find_index(args, "tq1lut") >= 0) {
        if (!ensure_q8_batch_cls()) {
            print("vk_gemv_probe: no Vulkan device\n")
            return 1
        }
        g_lb_warm = find_index(args, "warm") >= 0
        if (find_index(args, "iq1") >= 0) {
            g1_race()
        } elif (find_index(args, "tq1lut") >= 0) {
            tq1lut_race()
        } else {
            lb_race(find_index(args, "q10") >= 0)
        }
        return 0
    }
    if (g_single_copy && find_index(args, "cold") >= 0) {
        print("vk_gemv_probe: WARNING `single` takes precedence over `cold` - the cold arm is dropped; run them apart\n")
    }
    if (!ensure_q8_batch_cls()) {
        print("vk_gemv_probe: no Vulkan device\n")
        return 1
    }
    if (g_cols) {
        g_q8_n2_min_n = find_index(args, "n2off") >= 0 ? 0xFFFFFFFFu : 0u
        cols_ruler(n, d)
        return 0
    }
    let fmts = fixed_array(KqFmt.k4, KqFmt.q40, KqFmt.k5, KqFmt.k6, KqFmt.iq4xs, KqFmt.iq3s4, KqFmt.k3, KqFmt.iq3s, KqFmt.iq3xxs, KqFmt.iq4nl, KqFmt.k2, KqFmt.tq2, KqFmt.tq1, KqFmt.iq2s, KqFmt.iq2xs, KqFmt.iq2xxs, KqFmt.nv4)
    for (f in fmts) {
        verify(gemv_cls_ensure(int(f)), "vk_gemv_probe: every kq gemv class rail must engage on a live device")
    }
    let ring = g_single_copy ? "one plane copy per dispatch (single)" : "plane copies to {STREAM_MB} MB per dispatch"
    print("vk_gemv_probe: decode GEMV classes at [n {n} x d {d}], {g_gpu.rows_per_wg} rows/wg, {ring}, {DISPATCHES} dispatches x {SUBMITS} submits after a warm-up; lanes = lanes per output row (32 = one row per subgroup)\n")
    print("| fmt    | lanes |   MB/dispatch |  us/dispatch |    GB/s |             |\n")
    print("| :----- | ----: | ------------: | -----------: | ------: | :---------- |\n")
    verify(ensure_q8_gemv_cls(), "vk_gemv_probe: the q8 gemv class rail must engage on a live device")
    let q8 = probe_fmt(KqFmt.q8, n, d, 0u)
    print("| q8 | {g_gpu.subgroup_size:5d} | {q8.mb:13.1f} | {q8.us:12.1f} | {q8.mb * 1048576.0lf / (q8.us * 1000.0lf):7.1f} | timing-only |\n")
    let splits = fixed_array(0u, 16u, 8u)
    for (f in fmts) {
        for (lanes_per_row in splits) {
            let r = probe_fmt(f, n, d, lanes_per_row)
            let gbs = r.mb * 1048576.0lf / (r.us * 1000.0lf)
            print("| {f} | {lanes_per_row == 0u ? g_gpu.subgroup_size : int64(lanes_per_row):5d} | {r.mb:13.1f} | {r.us:12.1f} | {gbs:7.1f} | timing-only |\n")
        }
    }
    return 0
}
