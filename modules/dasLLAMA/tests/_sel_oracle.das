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
require math

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
