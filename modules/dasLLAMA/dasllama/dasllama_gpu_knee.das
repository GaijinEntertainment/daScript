options gen2
options _dasllama_internal = true
//! The short window's form rules both GPU homes serve and race (`ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k`): the
//! split-K rule a knee sizes, and the pricing that turns a mint's rung timings into the knob. Pure host arithmetic - each
//! home times its own stamps and hands the times here, so the rule a window serves and the rule its race prices are one.
module dasllama_gpu_knee shared public
require dasllama/dasllama_lint public
require dasllama/dasllama_gpu_math public   // SplitKnee, split_k_share: the share arithmetic the cm2 chunk is
require math

//! whether a site whose tile grid is `tiles` workgroups takes the split: the grid is at most half the knee's target
def split_k_on(tiles, kdim : int64; knee : SplitKnee) : bool => knee.tgs > 0l && kdim > 0l && tiles * 2l <= knee.tgs

//! the split count: doubling from one while the grid times the count stays at or under the target, up to eight, each share
//! a whole number of `align` chunks (`split_k_share`) and at least `min_k` deep
def split_k_ways(tiles, kdim : int64; knee : SplitKnee) : int64 {
    var nsk = 1l
    while (nsk < 8l && tiles * nsk * 2l <= knee.tgs && split_k_share(kdim, nsk * 2l, knee) != 0l && kdim / (nsk * 2l) >= knee.min_k) {
        nsk *= 2l
    }
    return nsk
}

//! the count a site of `tiles` workgroups splits its K into: `split_k_ways` where the split engages, one way where it does not
def split_k_count(tiles, kdim : int64; knee : SplitKnee) : int64 => split_k_on(tiles, kdim, knee) ? split_k_ways(tiles, kdim, knee) : 1l

//! The short window's split on the s column (`ARCHITECTURE_GPU_VULKAN_GEMM.md#vk-short-split`): the rule above under the home's
//! raced knee `tgs`, over the role's grid and its co-running roles' `sib_tiles`, each share whole 256-wide chunks with the last
//! one short, the partial planes within `scratch_bytes`; the column is the s column's 32
[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#vk-short-split")]
def short_split_pick(d, cnt, n, sib_tiles, scratch_bytes, tgs : int64) : tuple<tc : int64; nsplit : int64; ksplit : int64> {
    let knee = SplitKnee(tgs = tgs, min_k = 256l, align = 256l, exact = false)
    var sk = split_k_count(cm2_tiles_at(d, cnt, 32l) + sib_tiles, n, knee)
    while (sk > 1l && sk * cnt * d * 4l > scratch_bytes) {
        sk /= 2l
    }
    return (tc = 32l, nsplit = sk, ksplit = sk > 1l ? split_k_share(n, sk, knee) : 0l)
}

//! The rung table both homes race the q8 knee on - a 2048-deep site over five widths - and the K-quant knee's - a 5120-deep site
//! (a 27B's model width) to a 34816-wide one - and the split counts a rung times
let KNEE_Q8_K = 2048l
let KNEE_Q8_WIDTHS = fixed_array(1024l, 2048l, 4096l, 8192l, 16384l)
let KNEE_KQ_K = 5120l
let KNEE_KQ_WIDTHS = fixed_array(2048l, 4096l, 8192l, 17408l, 34816l)
let KNEE_WAYS = fixed_array(2l, 4l, 8l)
let KNEE_MULS = fixed_array(2l, 4l, 8l, 16l)   //! the knobs a rung offers the pricing: its grid times each

//! the `KneeRung.split` slot of a way count: two, four and eight ways
def knee_way_index(nsk : int64) : int => nsk == 2l ? 0 : (nsk == 4l ? 1 : 2)

//! The discrete homes' race shape: a site under the s column's 32 rows, the steps one timed run issues, the best of the rounds,
//! and the weight copies a rung walks - past the card's last-level cache, so a rung reads its weights as a window does (the
//! Metal race keeps its own `KNEE_RING_BYTES` and 32-row site in `dasllama_metal_kernels`)
let KNEE_S_ROWS = 31l
let KNEE_STEPS = 24
let KNEE_ROUNDS = 7
let KNEE_DISCRETE_RING_BYTES = 192l << 20l

//! both arms' best host wall a step - `whole` and `split` each issue `KNEE_STEPS` steps and wait for them - timed in turn, a
//! round of each, over `KNEE_ROUNDS` rounds after two untimed rounds of each, so drift between the arms' rounds prices neither
def knee_arms_us(whole : block<() : void>; split : block<() : void>) : tuple<b : double; t : double> {
    for (_w in range(2)) {
        invoke(whole)
        invoke(split)
    }
    var b = -1.0lf
    var t = -1.0lf
    for (_r in range(KNEE_ROUNDS)) {
        let t0 = ref_time_ticks()
        invoke(whole)
        let us0 = double(get_time_usec(t0)) / double(KNEE_STEPS)
        b = b < 0.0lf ? us0 : min(b, us0)
        let t1 = ref_time_ticks()
        invoke(split)
        let us1 = double(get_time_usec(t1)) / double(KNEE_STEPS)
        t = t < 0.0lf ? us1 : min(t, us1)
    }
    return (b = b, t = t)
}

//! a rung's synthetic q8 planes, word by word: hashed quants a copy `copy` of the ring, one small finite f16 scale in both
//! halves, and f16 activation pairs over a 64-step ramp
def knee_q8_word(i, copy : uint) : uint => tts_hash_u32(i + copy * 0x10001u) & 0x3F3F3F3Fu
let KNEE_Q8_SCALE_WORD = 0x1C001C00u
def knee_xf_word(i : uint) : uint {
    let h = 0x2C00u + i % 64u
    return h | (h << 16u)
}

//! every knob `knee_pick` prices over `widths` - each rung's grid `tiles_of(n)` times each of `KNEE_MULS`
def knee_candidates(widths : int64[5]; tiles_of : block<(n : int64) : int64>) : array<int64> {
    var out : array<int64>
    out |> reserve(length(widths) * length(KNEE_MULS))
    for (n in widths) {
        for (mul in KNEE_MULS) {
            out |> push(invoke(tiles_of, n) * mul)
        }
    }
    return <- out
}

//! One split-K rung a knee race timed: a site of `tiles` workgroups over `kdim`, its whole tile and its split stamp at two,
//! four and eight ways (a negative time: the way the race skipped)
struct KneeRung {
    tiles : int64
    kdim : int64
    base : double
    split : double[3]
}

//! One family's rungs over `widths` at depth `kdim`: a rung's grid is `tiles_of(n)` and its times `time(n, nsk)` - the whole tile
//! and the split at `nsk` ways, each arm's best - at every count of `KNEE_WAYS` whose share `shape` admits (whole chunks, at
//! least `min_k` deep); the others read negative, a way the pricing never takes. `open(n)` builds the width's planes before its
//! first timing and `close()` releases them after its last
def knee_rungs(widths : int64[5]; kdim : int64; shape : SplitKnee; tiles_of : block<(n : int64) : int64>;
               open : block<(n : int64) : void>; close : block<() : void>;
               time : block<(n, nsk : int64) : tuple<b : double; t : double>>) : array<KneeRung> {
    var rungs : array<KneeRung>
    rungs |> reserve(length(widths))
    for (n in widths) {
        invoke(open, n)
        var rung = KneeRung(tiles = invoke(tiles_of, n), kdim = kdim, base = -1.0lf)
        for (wi, nsk in range(3), KNEE_WAYS) {
            rung.split[wi] = -1.0lf
            continue if (split_k_share(kdim, nsk, shape) == 0l || kdim / nsk < shape.min_k)
            let pr = invoke(time, n, nsk)
            rung.base = rung.base < 0.0lf ? pr.b : min(rung.base, pr.b)
            rung.split[wi] = pr.t
        }
        invoke(close)
        rungs |> push(rung)
    }
    return <- rungs
}

//! `knee_rungs` over planes that outlive the widths (the Metal race's)
def knee_rungs(widths : int64[5]; kdim : int64; shape : SplitKnee; tiles_of : block<(n : int64) : int64>;
               time : block<(n, nsk : int64) : tuple<b : double; t : double>>) : array<KneeRung> =>
    <- knee_rungs(widths, kdim, shape, tiles_of, $(_n : int64) {}, $ {}, time)

//! The discrete homes' q8 knee race: the rung table (`KNEE_Q8_WIDTHS` at `KNEE_Q8_K`) under the s column's share rule, a width's
//! synthetic site built by `make_site` before its rungs and freed by `release` after them, each rung's grid `tiles_of` and its two
//! arms' times `times`, priced by `knee_pick` under `tag` in microseconds. Returns the knob; the home gates the race and sets it
def knee_q8_race(tag : string; tiles_of : block<(d : int64) : int64>; make_site : block<(d : int64) : void>; release : block<() : void>;
                 times : block<(d, nsk : int64) : tuple<b : double; t : double>>) : int64 {
    let shape = SplitKnee(min_k = 256l, align = 256l, exact = false)
    var rungs <- knee_rungs(KNEE_Q8_WIDTHS, KNEE_Q8_K, shape, tiles_of, make_site, release, times)
    let tgs = knee_pick(rungs, shape, tag, "us")
    delete rungs
    return tgs
}

//! A raced knob's setter: a value under `floor` is refused with a warning naming the setter, the stored value standing
def set_raced_knob(var g : int64&; v, floor : int64; name : string) {
    if (v < floor) {
        to_log(LOG_WARNING, "dasLLAMA: {name}({v}) out of range - keeping {g}\n")
        return
    }
    g = v
}

//! the knob the rungs' times pick: each candidate prices every rung at the ways `split_k_ways` gives it under `shape` (its
//! share floor and chunk), and the cheapest total wins - a candidate under 1% of the whole-tile total keeps the tiles whole (0).
//! Every arm's time logs on a line of its own, `timing-only` - no arm's output is read back - in the home's `unit`
def knee_pick(rungs : array<KneeRung>; shape : SplitKnee; tag : string; unit : string = "ms") : int64 {
    var whole = 0.0lf
    for (r in rungs) {
        whole += r.base
        to_log(LOG_INFO, "{tag} rung {r.tiles} tiles K {r.kdim}: whole {r.base} {unit} timing-only\n")
        for (wi, nsk in range(3), KNEE_WAYS) {
            continue if (r.split[wi] < 0.0lf)
            to_log(LOG_INFO, "{tag} rung {r.tiles} tiles K {r.kdim}: split x{nsk} {r.split[wi]} {unit} timing-only\n")
        }
    }
    var best = 0l
    var best_cost = whole
    for (r in rungs) {
        for (mul in KNEE_MULS) {
            let knee = SplitKnee(tgs = r.tiles * mul, min_k = shape.min_k, align = shape.align)
            var cost = 0.0lf
            for (q in rungs) {
                let w = split_k_on(q.tiles, q.kdim, knee) ? split_k_ways(q.tiles, q.kdim, knee) : 1l
                let wi = knee_way_index(w)
                cost += (w == 1l || q.split[wi] < 0.0lf) ? q.base : q.split[wi]
            }
            if (cost < best_cost * 0.99lf) {
                best_cost = cost
                best = knee.tgs
            }
        }
    }
    return best
}

//! One cutoff rung: at a site of size `x` (a grid, a row count) the current form took `base` and the other form `alt`
struct CutRung {
    x : int64
    base : double
    alt : double
}

//! The cutoff the rungs' times pick for a form taken on one side of it: `alt_from` true takes the other form at `x >= cut`,
//! false at `x <= cut`. Every rung's `x` is a candidate and the cheapest total wins, where it beats the base-only total by 1%;
//! otherwise `off` (the cutoff that keeps every rung on the current form).
def knee_cut_pick(rungs : array<CutRung>; alt_from : bool; off : int64; tag : string) : int64 {
    var base_total = 0.0lf
    for (r in rungs) {
        base_total += r.base
        to_log(LOG_INFO, "{tag} rung: x {r.x}: current {r.base} ms, other {r.alt} ms\n")
    }
    var best = off
    var best_cost = -1.0lf
    for (c in rungs) {
        var cost = 0.0lf
        for (q in rungs) {
            let alt = alt_from ? q.x >= c.x : q.x <= c.x
            cost += alt ? q.alt : q.base
        }
        if (best_cost < 0.0lf || cost < best_cost) {
            best_cost = cost
            best = c.x
        }
    }
    return best_cost >= 0.0lf && best_cost < base_total * 0.99lf ? best : off
}

//! The Vulkan short window's split-K knee (`ARCHITECTURE_GPU_VULKAN_GEMM.md#vk-short-split`): a window the s column serves
//! whose tile grid, its co-running roles' included, is at most half this many workgroups splits its K; the default is the
//! RTX PRO 4500's race. 0 leaves those windows whole. Raced by the mint (`vk_split_knee_race`); read at every pick.
var private g_vk_split_k_tgs = 128l

def set_vk_split_k_tgs(v : int64) {
    set_raced_knob(g_vk_split_k_tgs, v, 0l, "set_vk_split_k_tgs")
}

def vk_split_k_tgs : int64 => g_vk_split_k_tgs

//! The CUDA short window's split-K knee, the Vulkan knob's twin over the CUDA home's own stamps: a window the s column serves
//! whose tile grid is at most half this many blocks splits its K; 0 leaves those windows whole. Raced by the mint
//! (`cud_split_knee_race`); read at every pick. The default is the race's pick on the RTX PRO 4500 and the RTX 5060 Ti alike.
var private g_cud_split_k_tgs = 64l

def set_cud_split_k_tgs(v : int64) {
    set_raced_knob(g_cud_split_k_tgs, v, 0l, "set_cud_split_k_tgs")
}

def cud_split_k_tgs : int64 => g_cud_split_k_tgs
