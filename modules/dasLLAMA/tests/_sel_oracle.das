// The CPU oracle of the selection landing (`SelLanding`, the Vulkan kernel `ClsSelect`): a row's survivors at or above its
// K-th largest logit with the ties kept, its max, its softmax denominator at the row's temperature and the recent window's
// logits - built once here so the kernel cell and the landed sampler's cell read one reading of the plane. Vulkan-free: the
// kernel cell passes the kernel's own K and cap inside its static_if arm.

options gen2
options _dasllama_internal = true

module _sel_oracle shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_common   // Session, SamplingParams, sel_ask_words: the ask words the driver's glue would make
require dasllama/dasllama_gpu_tier   // sel_lane_val, SEL_K, SEL_WORDS, the ask and header word offsets
require dastest/testing_boost   // the gate's compare runs on the cell's T
require _compares   // mismatch_exact, first_mismatch: the gate's exact compares
require math
require daslib/math_bits   // uint_bits_to_float: the landed header's max and denominator words

let SEL_LANE_FLOOR = -3.4e38   //! the lowest finite a lane reads: `sel_lane_val` lands a NaN or a -inf lane here, and an id past the vocab

//! one row's landing off the selection: the survivors sorted by id with their logits, the row's max and softmax denominator,
//! the overflow flag and the recent-window logits
struct SelRow {
    ids : array<int64>
    vals : array<float>
    rmax : float
    den : float
    overflow : bool
    recent : array<float>
}

//! the selection's sampler words for `length(temps)` rows: row r at temperature `temps[r]` listing the recent ids `recent[r]`,
//! each row's words made by the engine's own `sel_ask_words` over a bare session under a penalty (the ids ride the ask only
//! under one), so the layout is the engine's, never restated here
def sel_words(temps : array<float>; recent : array<array<int64>>) : array<uint> {
    var words : array<uint>
    words |> reserve(SEL_WORDS * long_length(temps))
    let none : array<int64>
    for (temp, ids in temps, recent) {
        var s : Session
        var sp = SamplingParams(temp = temp, penalty = 1.5)
        s.spec_params = unsafe(addr(sp))
        s.recent |> push_from(ids)
        var row : array<uint>
        sel_ask_words(s, 1l, none, 0l, row)
        words |> push_from(row)
        s.spec_params = null
        delete row
        delete s
    }
    return <- words
}

//! the host's reading of a [rows x vocab] plane `y` under the selection at `k` survivors a row: every id whose logit reaches the
//! K-th largest (ties kept), the max, the denominator summed in double at the row's temperature, the overflow past the
//! landing's `cap` and the recent ids' logits
def select_rows_oracle(y : array<float>; temps : array<float>; recent : array<array<int64>>; vocab, rows : int64; k : int64 = SEL_K; cap : int64 = 2048l) : array<SelRow> {
    var out : array<SelRow>
    out |> resize(rows)
    let need = min(k, vocab)
    for (r in range64(rows)) {
        var lv <- [for (i in range64(vocab)); sel_lane_val(y[r * vocab + i])]
        var sorted := lv
        sort(sorted) $(a, b : float) : bool { return a > b }
        let kth = sorted[need - 1l]
        out[r].ids <- [for (i in range64(vocab)); i; where lv[i] >= kth]
        out[r].vals <- [for (i in out[r].ids); lv[i]]
        out[r].overflow = long_length(out[r].ids) > cap
        out[r].rmax = sorted[0]
        let inv = temps[r] > 0.0 ? 1.0lf / double(temps[r]) : 1.0lf
        var den = 0.0lf
        for (v in lv) {
            den += exp(max((double(v) - double(sorted[0])) * inv, -80.0lf))
        }
        out[r].den = float(den)
        out[r].recent <- [for (id in recent[r]); id < vocab ? lv[id] : SEL_LANE_FLOOR]
        delete lv
        delete sorted
    }
    return <- out
}

let SEL_FIXTURE_HI = 50.0   //! the fixture's high logit: past the random rows' range, the ties and the leaders sit on it

//! the selection gate's six rows at `vocab` with their ask words: a plain row, 3000 ties at the top (past the landing's cap at the
//! wide vocab, every id at the narrow), ten leaders over 1020 ties AT the K-th (1030 land at the wide vocab), a row pinned whole
//! (every id ties, the landing overflows), NaN and -inf lanes with the max at 123, and an all-NaN row
struct SelFixture {
    y : array<float>
    temps : array<float>
    recent : array<array<int64>>
    words : array<uint>
    rows : int64
}

def sel_fixture(vocab : int64) : SelFixture {
    let nan = unsafe(reinterpret<float>(0x7FC00000u))
    let ninf = unsafe(reinterpret<float>(0xFF800000u))
    var f : SelFixture
    f.rows = 6l
    f.y |> resize(vocab * f.rows)
    var seed = 12345u
    for (i in range64(vocab * f.rows)) {
        seed = seed * 1664525u + 1013904223u
        f.y[i] = float(seed >> 8u) / float(1u << 24u) * 40.0 - 20.0
    }
    for (j in range64(min(3000l, vocab))) {
        f.y[vocab + (j * 37l) % vocab] = SEL_FIXTURE_HI
    }
    for (j in range64(10l)) {
        f.y[2l * vocab + j * 11l] = SEL_FIXTURE_HI
    }
    for (j in range64(min(1020l, vocab - 10l))) {
        f.y[2l * vocab + 200l + j] = 40.0
    }
    for (i in range64(vocab)) {
        f.y[3l * vocab + i] = -1.0e30
    }
    f.y[4l * vocab + 5l] = nan
    f.y[4l * vocab + 123l] = SEL_FIXTURE_HI
    f.y[4l * vocab + 700l] = nan
    f.y[4l * vocab + 701l] = ninf
    for (i in range64(vocab)) {
        f.y[5l * vocab + i] = nan
    }
    f.temps <- [0.7, 1.0, 0.5, 0.0, 1.0, 1.0]
    f.recent <- [[5l, 77l, vocab - 1l, vocab + 5l], [0l], [200l, 1l], [1l], [5l, 123l, 701l], [9l]]
    f.words <- sel_words(f.temps, f.recent)
    return <- f
}

//! the rows' landings as the host reads them off the four planes (the `SEL_*` layout: `SEL_HDR` header words, `SEL_CAP` ids and
//! logits, `SEL_RECENT` recent logits a row), each row's survivors sorted by id
def sel_unpack(hdr, ids : array<uint>; vals, rec : array<float>; words : array<uint>; rows : int64) : array<SelRow> {
    var out : array<SelRow>
    out |> resize(rows)
    for (r in range64(rows)) {
        let n = int64(hdr[r * SEL_HDR + SEL_HDR_COUNT])
        var order <- [for (j in range64(n)); j]
        sort(order) $(a, b : int64) : bool { return ids[r * SEL_CAP + a] < ids[r * SEL_CAP + b] }
        out[r].ids <- [for (j in order); int64(ids[r * SEL_CAP + j])]
        out[r].vals <- [for (j in order); vals[r * SEL_CAP + j]]
        out[r].rmax = uint_bits_to_float(hdr[r * SEL_HDR + SEL_HDR_MAX])
        out[r].den = uint_bits_to_float(hdr[r * SEL_HDR + SEL_HDR_DEN])
        out[r].overflow = hdr[r * SEL_HDR + SEL_HDR_OVERFLOW] != 0u
        let nrec = min(int64(words[r * SEL_WORDS + SEL_ASK_COUNT]), SEL_RECENT)
        out[r].recent <- [for (j in range64(nrec)); rec[r * SEL_RECENT + j]]
        delete order
    }
    return <- out
}

let SEL_DEN_BAR = 2.0e-5   //! the device sums up to 256 exps serially a thread, then a 10-level tree over the workgroup, each exp
                           //! within an ulp: the worst case against the host's double sum is (256 + 10 + 1) x 2^-24, under 1.6e-5

//! the gate's compare of the kernel's six landings against the oracle's, both homes' one reading: the ids and their logits exact,
//! the max, the overflow flag and the recent logits, the denominators within `SEL_DEN_BAR`, the lane floor on an id past the
//! vocab and a -inf lane, a poisoned denominator and a poisoned id its controls
def sel_gate_compare(t : T?; tag : string; got : array<SelRow>; var want : array<SelRow>; vocab : int64) {
    let rows = long_length(want)
    var worst = 0.0
    var worst_row = -1l
    for (r in range64(rows)) {
        let rd = abs(got[r].den - want[r].den) / max(want[r].den, 1.0e-30)
        if (!(rd <= worst)) {   //! a NaN difference is the worst there is: `max` would drop it
            worst = rd
            worst_row = r
        }
        if (want[r].overflow) {   //! an overflowing row lands the cap's worth of its ties, whichever came first: the count is the claim
            t |> equal(long_length(got[r].ids), SEL_CAP, "{tag} row {r}: an overflowing row lands the cap ({length(want[r].ids)} survivors)")
        } else {
            to_log(LOG_INFO, "{tag} row {r}: ids {mismatch_exact(got[r].ids, want[r].ids)} mismatched, first at {first_mismatch(got[r].ids, want[r].ids)}; logits {mismatch_exact(got[r].vals, want[r].vals)} mismatched, first at {first_mismatch(got[r].vals, want[r].vals)}\n")
            t |> equal(mismatch_exact(got[r].ids, want[r].ids), 0, "{tag} row {r}: the landed ids are the host's survivors ({length(got[r].ids)} of {length(want[r].ids)})")
            t |> equal(mismatch_exact(got[r].vals, want[r].vals), 0, "{tag} row {r}: every landed logit is its id's")
        }
        to_log(LOG_INFO, "{tag} row {r}: the max {got[r].rmax} against {want[r].rmax}; recent {mismatch_exact(got[r].recent, want[r].recent)} mismatched, first at {first_mismatch(got[r].recent, want[r].recent)}\n")
        t |> equal(got[r].rmax, want[r].rmax, "{tag} row {r}: the max")
        t |> equal(got[r].overflow, want[r].overflow, "{tag} row {r}: the overflow flag ({length(want[r].ids)} survivors against the cap)")
        t |> equal(mismatch_exact(got[r].recent, want[r].recent), 0, "{tag} row {r}: the recent window's logits ({got[r].recent} against {want[r].recent})")
    }
    to_log(LOG_INFO, "{tag}: the denominator's worst relative difference {worst} at row {worst_row}; row 2 lands {length(got[2].ids)} ids\n")
    t |> success(worst <= SEL_DEN_BAR, "{tag}: the denominators agree within {SEL_DEN_BAR} (worst {worst} at row {worst_row})")
    let poisoned = abs(got[0].den * 1.001 - want[0].den) / want[0].den
    t |> success(is_finite(poisoned) && poisoned > SEL_DEN_BAR, "{tag}: a denominator a tenth of a percent off reds the bar ({poisoned})")
    if (vocab > 3000l) {
        t |> equal(long_length(got[2].ids), 1030l, "{tag}: row 2 lands its ten leaders and the 1020 ties at the K-th")
        t |> success(got[1].overflow && got[3].overflow && got[5].overflow, "{tag}: rows 1, 3 and 5 overflow the landing")
        t |> equal(long_length(got[0].ids), long_length(want[0].ids), "{tag}: row 0 lands K and the ties at the K-th ({length(got[0].ids)})")
    } else {
        t |> equal(long_length(got[0].ids), vocab, "{tag}: a vocab under K lands whole")
    }
    t |> equal(got[0].recent[3], SEL_LANE_FLOOR, "{tag}: an id past the vocab lands the lowest finite")
    t |> equal(got[4].recent[2], SEL_LANE_FLOOR, "{tag}: a -inf lane lands the lowest finite")
    t |> equal(got[4].rmax, SEL_FIXTURE_HI, "{tag}: row 4's max is its widest finite lane")
    want[2].ids[0] = want[2].ids[0] + 1l
    t |> equal(mismatch_exact(got[2].ids, want[2].ids), 1, "{tag}: the id compare reds one poisoned id")
}
