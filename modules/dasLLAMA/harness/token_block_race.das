options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require llvm/daslib/llvm_tune
require ../performance/profile_common.das   // tune_gate - the standing pre-measure stack
require ../tests/_q4k_disk.das              // q4k_disk_superblock - the K-quant superblock the planes transcode
require ../tests/_kq_dot.das                // dot_kq - the disk-order dot, the CPU reference
require dasllama/dasllama_math
require dasllama/dasllama_math_default
require dasllama/dasllama_math_gen
require dasllama/dasllama_convert
require dasllama/dasllama_gemm_schema
require daslib/jobque_boost
require math

// The batch walk's token block, raced: one process, the generated backend's `matmul_kq_batch` on
// k4, every candidate block timed once a round in turn (`ROUNDS` rounds after `WARMUP` dropped),
// the block floor at 1 and the L2 budget lifted so the walk runs the block it was asked for.
// The lever is `q8_token_block`; BASELINE is the block the engine ships. Each arm's output is
// read back on a sampled region - the first and last `SAMPLE_TOKENS` tokens by the first and last
// `SAMPLE_ROWS` rows - and held to the baseline arm's; the baseline arm's to the disk-order dots.
// Lanes: DAS_JOBQUE_THREADS. A run that reports no row, or a candidate the walk does not run as
// asked, exits non-zero.
//
//   bin/daslang -jit modules/dasLLAMA/harness/token_block_race.das

let BASELINE = 512l
let ROUNDS = 11
let WARMUP = 2
let NTOK = 512l
let SAMPLE_TOKENS = 4l
let SAMPLE_ROWS = 32l
let FMT = 4
//! a bf16 tile rounds every weight and activation to bf16 before the dot: a row sits within 1e-2 of the image's largest magnitude
let BF16_ENVELOPE = 1.0e-2
//! an int8 walk sums a row's blocks in another order than the disk-order dot: the same products, f32 rounding apart
let FLOAT_ORDER = 1.0e-5
//! what an arm's output holds before its walk: an element the walk leaves reads as outside every bound
let NOT_WRITTEN = unsafe(reinterpret<float>(0x7FC00000u))

struct Shape {
    n : int64
    d : int64
    blocks : array<int64>
}

struct Planes {
    kq : array<uint8>       //! the backend's layout
    ks : array<uint8>
    disk_kq : array<uint8>  //! disk order, what the reference dots read
    disk_ks : array<uint8>
    xq : array<int8>
    xs : array<float>
    xbs : array<int>
}

def private gen_backend() : string => get_architecture_name() == "arm64" ? "arm64-gen" : "x64-gen"

def private build_planes(n, d : int64) : Planes {
    var p : Planes
    let nsb = n / KQ_SB_ELEMS
    let qsb = kq_qsb(FMT)
    let ssb = kq_ssb(FMT)
    let blk <- q4k_disk_superblock()
    p.disk_kq |> resize(d * nsb * qsb)
    p.disk_ks |> resize(d * nsb * ssb)
    for (r in range64(d)) {
        for (s in range64(nsb)) {
            kq_transcode_superblock(KqFmt.k4, blk, 0l, p.disk_kq, (r * nsb + s) * qsb, p.disk_ks, (r * nsb + s) * ssb)
        }
        var st = 0x51ED270Bu + uint(r) * 0x85EBCA6Bu
        for (i in range64(nsb * qsb)) {
            st = st * 1664525u + 1013904223u
            p.disk_kq[r * nsb * qsb + i] = uint8(uint(p.disk_kq[r * nsb * qsb + i]) ^ (st >> 16u))
        }
    }
    p.kq := p.disk_kq
    p.ks := p.disk_ks
    repack_kq_weight(FMT, p.kq, p.ks, 0l, n, d)
    var src : array<float>
    src |> resize(NTOK * n)
    for (i in range64(NTOK * n)) {
        src[i] = 0.9 * sin(float(i) * 0.27) - 0.5 * cos(float(i) * 0.061)
    }
    p.xq |> resize(NTOK * n)
    p.xs |> resize(NTOK * n / KQ_SB_ELEMS)
    p.xbs |> resize(NTOK * n / 16l)
    unsafe {
        for (t in range64(NTOK)) {
            quantize_q8_k_into_ptr(addr(src[t * n]), n, addr(p.xq[0]), addr(p.xs[0]), addr(p.xbs[0]),
                t * n, t * (n / KQ_SB_ELEMS), t * (n / 16l))
        }
    }
    delete src
    return <- p
}

//! the sampled region's elements, as indices into an `NTOK` x `d` image
def private sampled(d : int64) : array<int64> {
    var at : array<int64>
    at |> reserve(4l * SAMPLE_TOKENS * SAMPLE_ROWS)
    for (tk in range64(NTOK)) {
        if (tk >= SAMPLE_TOKENS && tk < NTOK - SAMPLE_TOKENS) continue
        for (row in range64(d)) {
            if (row >= SAMPLE_ROWS && row < d - SAMPLE_ROWS) continue
            at |> push(tk * d + row)
        }
    }
    return <- at
}

def private disk_dots(p : Planes; n, d : int64; at : array<int64>) : array<float> {
    let nsb = n / KQ_SB_ELEMS
    let qsb = kq_qsb(FMT)
    let ssb = kq_ssb(FMT)
    var want : array<float>
    want |> reserve(length(at))
    unsafe {
        for (i in at) {
            let tk = i / d
            let row = i % d
            want |> push(dot_kq(addr<uint8 const?>(p.disk_kq[row * nsb * qsb]), addr<uint8 const?>(p.disk_ks[row * nsb * ssb]),
                addr<int8 const?>(p.xq[tk * n]), addr<float const?>(p.xs[tk * nsb]), addr<int const?>(p.xbs[tk * (n / 16l)]),
                n, KqFmt.k4))
        }
    }
    return <- want
}

//! the largest difference of `got` from `want` over the sampled region, and the bound it answers to
def private held(got : array<float>; want : array<float>; rel : float) : tuple<diff : float; bound : float; ok : bool> {
    var diff = 0.0
    var mag = 0.0
    var finite = true
    for (g, w in got, want) {
        finite &&= is_finite(g)
        diff = max(diff, abs(g - w))
        mag = max(mag, abs(w))
    }
    return (diff = diff, bound = rel * mag, ok = finite && diff <= rel * mag)
}

def private compare_words(h : tuple<diff : float; bound : float; ok : bool>; against : string; count : int) : string {
    if (h.diff == 0.0 && h.ok) {
        return "bit-exact vs {against} on {count} sampled elements"
    }
    return "{h.ok ? "within" : "OUTSIDE"} {h.bound} of {against} on {count} sampled elements (largest difference {h.diff})"
}

def private stats(var ms : array<float>) : tuple<best : float; median : float; cv : float> {
    sort(ms)
    var mean = 0.0
    for (v in ms) {
        mean += v
    }
    mean /= float(length(ms))
    var sq = 0.0
    for (v in ms) {
        sq += (v - mean) * (v - mean)
    }
    return (best = ms[0], median = ms[length(ms) / 2], cv = sqrt(sq / float(length(ms))) / mean * 100.0)
}

//! one shape's arms; answers the rows it reported, or -1 when an arm failed its compare or did not run the block asked
def private race(sh : Shape) : int {
    var p <- build_planes(sh.n, sh.d)
    let at <- sampled(sh.d)
    let want <- disk_dots(p, sh.n, sh.d, at)
    let bf16 = kq_tileform_of(FMT) > 0
    var y : array<float>
    y |> resize(NTOK * sh.d)
    var times : array<array<float>>
    var images : array<array<float>>
    var walked : array<int64>
    times |> reserve(length(sh.blocks))
    images |> reserve(length(sh.blocks))
    walked |> reserve(length(sh.blocks))
    for (b in sh.blocks) {
        var none : array<float>
        times |> emplace(none)
        set_q8_token_block(b)
        walked |> push(kq_token_block(FMT, sh.n))
        for (v in y) {
            v = NOT_WRITTEN
        }
        matmul_kq_batch(FMT, y, p.kq, p.ks, 0l, p.xq, p.xs, p.xbs, sh.n, sh.d, NTOK)
        var img <- [for (i in at); y[i]]
        images |> emplace(img)
    }
    for (round in range(ROUNDS + WARMUP)) {
        for (ci in range(length(sh.blocks))) {
            set_q8_token_block(sh.blocks[ci])
            let t0 = ref_time_ticks()
            matmul_kq_batch(FMT, y, p.kq, p.ks, 0l, p.xq, p.xs, p.xbs, sh.n, sh.d, NTOK)
            let ms = float(get_time_usec(t0)) / 1000.0
            if (round >= WARMUP) {
                times[ci] |> push(ms)
            }
        }
    }
    let base = find_index(sh.blocks, BASELINE)
    var rows = 0
    var good = base >= 0
    if (base < 0) {
        print("K={sh.n} rows={sh.d}: no arm runs the baseline block {BASELINE} - no row\n")
    }
    for (ci in range(length(sh.blocks))) {
        continue if (base < 0)
        let st = stats(times[ci])
        let isBase = ci == base
        let h = (isBase ? held(images[ci], want, bf16 ? BF16_ENVELOPE : FLOAT_ORDER)
            : held(images[ci], images[base], bf16 ? BF16_ENVELOPE : 0.0))
        let asked = walked[ci] == sh.blocks[ci]
        let words = isBase ? "baseline, {compare_words(h, "the disk-order dots", length(at))}" : compare_words(h, "the {BASELINE} arm", length(at))
        print("K={sh.n} rows={sh.d} ntok={NTOK} backend={active_kernel_backend()} lanes={get_dispatch_lanes()} block={sh.blocks[ci]} walked={walked[ci]} best_ms={st.best} median_ms={st.median} cv%={st.cv} | {words}{asked ? "" : " | INERT: the walk ran another block"}\n")
        good &&= h.ok && asked
        rows++
    }
    delete p
    delete y
    delete times
    delete images
    return good ? rows : -1
}

[export]
def main() : int {
    var rows = 0
    var bad = 0
    with_job_que() {
        setup_dasllama_jobque_()
        let active = active_kernel_backend()
        let pin = kernel_backend_pin()
        let knob = get_q8_token_block()
        let budget = get_q8_l2_budget()
        let floor = bf16_token_block_floor()
        pin_kernel_backend(gen_backend())
        if (active_kernel_backend() != gen_backend()) {
            print("token_block_race: this box serves {active_kernel_backend()}, the pin on {gen_backend()} did not take - no row\n")
            bad++
        } elif (!kernel_backend_has_kq() || !kernel_backend_has_kq_batch()) {
            print("token_block_race: {gen_backend()} carries no kq batch kernel on this box - no row\n")
            bad++
        } elif (!tune_gate()) {
            print("token_block_race: the tune gate refused - no row\n")
            bad++
        } else {
            set_q8_l2_budget(1l << 40l)
            set_bf16_token_block_floor(1l)
            print("token_block_race: backend {active_kernel_backend()}, k4 tile form {kq_tileform_of(FMT)}, {get_dispatch_lanes()} lanes, {ROUNDS} rounds after {WARMUP} dropped\n")
            var shapes <- [
                Shape(n = 2048l, d = 8192l, blocks <- [128l, 256l, 384l, 512l]),
                Shape(n = 2560l, d = 9728l, blocks <- [128l, 256l, 384l, 512l]),
                Shape(n = 4096l, d = 4096l, blocks <- [128l, 256l, 384l, 512l]),
                Shape(n = 8192l, d = 2048l, blocks <- [128l, 256l, 384l, 512l]),
                Shape(n = 9728l, d = 2560l, blocks <- [256l, 384l, 416l, 512l]),
                Shape(n = 16384l, d = 4096l, blocks <- [256l, 384l, 512l])
            ]
            for (sh in shapes) {
                let r = race(sh)
                if (r < 0) {
                    bad++
                } else {
                    rows += r
                }
            }
            delete shapes
        }
        set_bf16_token_block_floor(floor)
        set_q8_l2_budget(budget)
        set_q8_token_block(knob)
        clear_kernel_backend_pin()
        if (!empty(pin)) {
            pin_kernel_backend(pin)
        }
        select_kernel_backend(active)
    }
    if (rows == 0 || bad > 0) {
        print("token_block_race: {rows} rows, {bad} refusals - the run fails\n")
        return 1
    }
    return 0
}
