// The CTC head's pick cells' synthetic fixture and CPU reference, shared by the Metal and the Vulkan kernel cells (`GkCtcPick`):
// the cases - three widths, a tie, an all-NaN row, pad slots past the classes poisoned - and the CPU `gg_decode_ctc` row loop;
// and the GigaAM CTC head's model legs - a decode's picks, plogs and ids, its device seat's served count - the device cells hold
// against the CPU head.

options gen2
options _dasllama_internal = true

module _ctc_oracle shared public

require dasllama/dasllama_lint public
require math
require daslib/math_bits   // uint_bits_to_float: the NaN row
require _compares   // NAN_SENTINEL: the pad slots; copy_span
require dasllama/dasllama_gigaam   // the head's model legs
require ?das_metal dasllama/dasllama_metal_tower   // metal_tower_ga_ctcs: the Metal CTC seat's count
require ?vulkan dasllama/dasllama_vulkan_asr_dec   // vulkan_ctc_stats: the Vulkan seat's, on a build without das_metal

//! the GigaAM CTC heads the build's device seat served: Metal's, else Vulkan's, 0 with neither
def ctc_seat_heads : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_tower_ga_ctcs()
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return vulkan_ctc_stats().heads
        } else {
            return 0l
        }
    }
}

//! the cases (nc, rows, ldl, nan_row): 34 classes over 64-wide rows (one stride of the 256 threads, the pad unread), 1025 over 1088
//! (a tail past four strides), 257 with row 0 all NaN
def ctc_pick_cases : int4[3] => fixed_array(int4(34, 13, 64, 0), int4(1025, 40, 1088, 0), int4(257, 5, 257, 1))

//! One CTC pick case's rows: unbiased logits [rows][ldl] whose pad slots past `nc` hold NaN (a pick that read them reds), every
//! third row a tie of two classes at 9 (the lower wins), the bias behind a lead of 1e30, row 0 all NaN where `nan_row`.
struct CtcPickFix {
    lg : array<float>
    wb : array<float>
    nc : int
    rows : int
    ldl : int
    boff : int
}

def ctc_pick_fix(nc, rows, ldl : int; nan_row : bool) : CtcPickFix {
    var fx = CtcPickFix(nc = nc, rows = rows, ldl = ldl, boff = 7)
    fx.lg |> resize(rows * ldl)
    for (r in range(rows)) {
        for (i in range(ldl)) {
            let e = r * ldl + i
            fx.lg[e] = i < nc ? 3.0 * sin(float(e) * 0.37) + 0.5 * cos(float(e) * 1.3) : NAN_SENTINEL
        }
        if (r % 3 == 1) {
            fx.lg[r * ldl + (r * 7) % nc] = 9.0
            fx.lg[r * ldl + (r * 7 + 5) % nc] = 9.0
        }
    }
    if (nan_row) {
        for (i in range(nc)) {
            fx.lg[i] = uint_bits_to_float(0x7fc00000u)
        }
    }
    fx.wb |> resize(fx.boff + nc)
    for (i in range(fx.boff)) {
        fx.wb[i] = 1.0e30
    }
    for (i in range(nc)) {
        fx.wb[fx.boff + i] = 0.01 * float(i % 5)
    }
    return <- fx
}

//! the CPU `gg_decode_ctc` row loop over the biased rows: each row's argmax (the lowest of a tie, class 0 where no logit beats
//! the first: a NaN row) and -log of the double sum of exp(logit - max)
def ctc_pick_ref(fx : CtcPickFix) : tuple<tok : array<uint>; plog : array<float>> {
    var tok : array<uint>
    var plog : array<float>
    tok |> reserve(fx.rows)
    plog |> reserve(fx.rows)
    for (r in range(fx.rows)) {
        var best = 0
        var mx = fx.lg[r * fx.ldl] + fx.wb[fx.boff]
        for (i in range(1, fx.nc)) {
            let v = fx.lg[r * fx.ldl + i] + fx.wb[fx.boff + i]
            if (v > mx) {
                mx = v
                best = i
            }
        }
        var lse = 0.0lf
        for (i in range(fx.nc)) {
            lse += exp(double(fx.lg[r * fx.ldl + i] + fx.wb[fx.boff + i] - mx))
        }
        tok |> push(uint(best))
        plog |> push(-float(log(lse)))
    }
    return (tok <- tok, plog <- plog)
}

let CTC_PLOG_BAR = 1e-4   //!< a frame's -log sum exp(logit - max), device against CPU: the GEMM summed in another order and a float log-sum against the double one
let CTC_TIE_MARGIN = 1e-3   //!< a frame whose CPU top two logits sit closer than this (of the largest magnitude) may pick either

//! One CTC head decode's outputs: the CPU leg's logits, each frame's pick and its -log sum exp(logit - max) - read back off the
//! device, or summed here in double off the CPU's logits - the tokens' ids and the transcript, and the device seat's served
//! count it moved (`ctc_seat_heads`).
struct CtcLeg {
    logits : array<float>
    picks : array<int>
    plogs : array<float>
    ids : array<int>
    text : string
    served : int64
}

def ctc_leg(m : GigaamModel; var st : GigaamState; n : int64) : CtcLeg {
    var r : CtcLeg
    let nc = m.n_classes
    let c0 = ctc_seat_heads()
    gigaam_decode(m, st, n)
    r.served = ctc_seat_heads() - c0
    r.picks <- copy_span(st.picks, 0l, n)
    if (r.served > 0l) {
        r.plogs <- copy_span(st.plogs, 0l, n)
    } else {
        r.logits <- copy_span(st.logits, 0l, n * nc)
        r.plogs |> reserve(n)
        for (f in range64(n)) {
            var mx = r.logits[f * nc]
            for (c in range64(nc)) {
                mx = max(mx, r.logits[f * nc + c])
            }
            var lse = 0.0lf
            for (c in range64(nc)) {
                lse += exp(double(r.logits[f * nc + c] - mx))
            }
            r.plogs |> push(-float(log(lse)))
        }
    }
    r.ids <- [for (x in st.toks); x.id]
    r.text = gigaam_detokenize(m, st.toks)
    return <- r
}

//! the CPU head's leg the device head is held to: on an f16 head the rows rounded to half first - the device's half GEMM
//! stages its input rows to half, its products then exact - so the two differ by summation order alone
def ctc_cpu_leg(m : GigaamModel; var st : GigaamState; n : int64) : CtcLeg {
    if (!m.ctc_f16) {
        return <- ctc_leg(m, st, n)
    }
    var inscope keep := st.x
    for (v in st.x) {
        v = float(float16(v))
    }
    var r <- ctc_leg(m, st, n)
    st.x := keep
    return <- r
}

//! the frames whose picks differ between the CPU leg and another where the CPU's top two logits sit apart by `CTC_TIE_MARGIN` or
//! more of the largest magnitude: a near-tie frame may pick either
def ctc_hard_flips(cpu, dev : CtcLeg; nc : int64) : int {
    var scale = 0.0
    for (v in cpu.logits) {
        scale = max(scale, abs(v))
    }
    var hard = 0
    for (f in range64(long_length(cpu.picks))) {
        continue if (cpu.picks[f] == dev.picks[f])
        var a = -3.4e38
        var b = -3.4e38
        for (c in range64(nc)) {
            let v = cpu.logits[f * nc + c]
            if (v > a) {
                b = a
                a = v
            } elif (v > b) {
                b = v
            }
        }
        if (a - b >= CTC_TIE_MARGIN * scale) {
            to_log(LOG_WARNING, "test_gigaam: frame {f} picks {cpu.picks[f]} on the CPU, {dev.picks[f]} on the device, margin {a - b}\n")
            hard++
        }
    }
    return hard
}
