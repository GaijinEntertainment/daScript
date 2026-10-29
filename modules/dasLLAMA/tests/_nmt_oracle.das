options gen2
options _dasllama_internal = true

module _nmt_oracle shared public

require dasllama/dasllama_lint public
require dastest/testing_boost
require math
require daslib/math_bits   // float_bits_to_uint: the kept check's bit compare
require strings
require dasllama/dasllama_gpu_kernels_common   // the greedy state's words, `NMT_NO_TOKEN`
require _compares   // check_exact, sentinel_off: the cells' shared verdicts
require dasllama/dasllama_nmt   // NmtModel: the seat's poison seam reads its classifier
require ?das_metal dasllama/dasllama_metal_nmt      // the Metal translation seat's knob, counters, poison seam and shutdown
require ?das_metal dasllama/dasllama_metal_common   // metal_common_init: whether the box has a Metal device
require ?vulkan dasllama/dasllama_vulkan_nmt        // the Vulkan seat's, on a build without das_metal
require ?vulkan dasllama/dasllama_vulkan_tower      // vulkan_tower_arms: whether the tier serves on this box

// The Bergamot step's device kernels' fixtures and CPU references (`GkNmtEmbed`, `GkNmtSsru`, `GkNmtPick`), shared by the
// Metal and the Vulkan cells. Every value is a formula of its index, so a cell knows each element. Then the build's device
// translation seat (`nmt_seat_*`: Metal, else Vulkan, else none) that the device cell and `../harness/nmt_probe.das` drive.

//! The build's device translation seat: Metal, else Vulkan, else none.
def nmt_seat_name : string {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return "metal"
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return "vulkan"
        } else {
            return ""
        }
    }
}

//! whether the build's seat can serve on this box: a Metal build's where the box has a Metal device, a Vulkan build's where the
//! tier arms a device
def nmt_seat_ready : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_common_init()
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return vulkan_tower_arms()
        } else {
            return false
        }
    }
}

//! the build's seat pinned on or off; answers the setting it replaced (false with no seat)
[unused_argument(on)]
def nmt_seat_pin(on : bool) : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let was = metal_nmt_knob()
        set_metal_nmt(on)
        return was
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let was = vulkan_nmt_knob()
            set_vulkan_nmt(on)
            return was
        } else {
            return false
        }
    }
}

//! the build's seat's counters: the sentences it served whole and its declines (zeros with no seat)
def nmt_seat_stats : tuple<sentences : int64; declines : int64> {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let s = metal_nmt_stats()
        return (sentences = s.sentences, declines = s.declines)
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let s = vulkan_nmt_stats()
            return (sentences = s.sentences, declines = s.declines)
        } else {
            return (sentences = 0l, declines = 0l)
        }
    }
}

//! the build's seat's declines of reason `why` (0 with no seat)
[unused_argument(why)]
def nmt_seat_declines(why : string) : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_nmt_declines()?[why] ?? 0l
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return vulkan_nmt_declines()?[why] ?? 0l
        } else {
            return 0l
        }
    }
}

//! the attached model's classifier zeroed on the device, so every step's logits are the output bias alone (no seat: nothing)
[unused_argument(m)]
def nmt_seat_poison(m : NmtModel) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        metal_nmt_zero_cls_probe_(m)
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            vulkan_nmt_zero_cls_probe_(m)
        }
    }
}

//! the device's largest buffer under the seat's scratch takes stood in by `bytes` (0 reads the device's again), with the
//! stand-in it replaced; `seam` false where the seat reads no such limit (Vulkan: `ARCHITECTURE_GPU_ASYMMETRIES.md`)
[unused_argument(bytes)]
def nmt_seat_take_max_probe(bytes : uint64) : tuple<seam : bool; was : uint64> {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return (seam = true, was = set_metal_take_max_probe_(bytes))
    } else {
        return (seam = false, was = 0ul)
    }
}

//! every device object the seat owns released; the next served sentence attaches again
def nmt_seat_shutdown {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        metal_nmt_shutdown()
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            vulkan_nmt_shutdown()
        }
    }
}

//! true, with the cell skipped, where the build has no device translation seat or the box no device for it
def nmt_seat_skip(t : T?; tag : string) : bool {
    return false if (nmt_seat_name() != "" && nmt_seat_ready())
    t |> skip("{tag}: no device translation seat on this build or box (Metal, or Vulkan under DASLLAMA_GPU=1 on a build without das_metal)")
    return true
}

//! one side of a translation compare decoded, newlines escaped: one log record a side
def nmt_log_side(m : NmtModel; tag, side : string; ids : array<int64>) {
    to_log(LOG_INFO, "{tag} {side}: {replace(spm_decode_ids(m.spm.tk, ids), "\n", "\\n")}\n")
}

let NMT_FIX_D = 100   //! the row width: off the 64-thread grid and the 256-thread stride
let NMT_FIX_V = 5     //! the embedding table's rows
let NMT_FIX_LEAD = 7  //! the floats ahead of the table in the plane - a kernel reading the table from 0 misses
let NMT_PICK_SLACK = 4   //! the record words past the cap a pick cell sizes its record plane with, each under the sentinel

//! the elements past `n` a dispatch of `n` one-element invocations covers on a 256-lane grid - every home's widest
def nmt_slack(n : int) : int => (n + 255) / 256 * 256 - n

//! the slots of a written plane past its run of `n` still under the NaN sentinel: the count off, its first index, logged both
//! sides; a poisoned slot the control
def nmt_check_slack(t : T?; tag : string; whole : array<float>; n : int) {
    var inscope tail <- copy_span(whole, n, length(whole) - n)
    let off = sentinel_off(tail)
    to_log(LOG_INFO, "{tag}: {off} of {length(tail)} slots past the run off the sentinel, the first at {first_sentinel_off(tail)}\n")
    t |> equal(off, 0, "{tag}: the {length(tail)} slots past the run keep the sentinel")
    if (!empty(tail)) {
        tail[0] = 0.0
        t |> success(sentinel_off(tail) > 0, "{tag}: the slack check sees a poisoned slot")
    }
}

//! every element of `whole` outside [lo, lo + n) equal to the same element of `kept`: the count off and its first index logged
def nmt_check_kept(t : T?; tag : string; whole, kept : array<float>; lo, n : int) {
    var off = 0
    var first = -1
    for (i in range(length(whole))) {
        continue if (i >= lo && i < lo + n)
        if (float_bits_to_uint(whole[i]) != float_bits_to_uint(kept[i])) {
            off++
            first = first < 0 ? i : first
        }
    }
    to_log(LOG_INFO, "{tag}: {off} of {length(whole) - n} elements outside the run moved, the first at {first}\n")
    t |> equal(off, 0, "{tag}: the elements outside the run keep their values")
}

//! The embed cell's plane: `NMT_FIX_LEAD` floats, then the [V][d] table; and the [positions][d] sinusoid stand-in.
struct NmtEmbedFix {
    wn : array<float>
    pos : array<float>
}

def nmt_embed_fix(npos : int) : NmtEmbedFix {
    return <- NmtEmbedFix(wn <- [for (i in range(NMT_FIX_LEAD + NMT_FIX_V * NMT_FIX_D)); float((i * 37) % 101) * 0.01 - 0.5],
                          pos <- [for (i in range(npos * NMT_FIX_D)); float((i * 13) % 17) * 0.1 - 0.8])
}

//! The CPU `nmt_embed_row` over `ids` at positions `p0..`: the table row times `scale` (a zero row for `NMT_NO_TOKEN`) plus
//! the position row.
def nmt_embed_ref(f : NmtEmbedFix; ids : array<uint>; p0 : int; scale : float) : array<float> {
    var out : array<float>
    out |> reserve(length(ids) * NMT_FIX_D)
    for (r, id in count(), ids) {
        for (i in range(NMT_FIX_D)) {
            let e = id == NMT_NO_TOKEN ? 0.0 : f.wn[NMT_FIX_LEAD + int(id) * NMT_FIX_D + i] * scale
            out |> push(e + f.pos[(p0 + r) * NMT_FIX_D + i])
        }
    }
    return <- out
}

//! The SSRU cell's inputs: the stacked row [x W | x Wf], the plane holding the forget bias at `NMT_FIX_LEAD`, two layers'
//! cells (the dispatch reads layer 1's).
struct NmtSsruFix {
    h : array<float>
    wn : array<float>
    cell : array<float>
}

def nmt_ssru_fix : NmtSsruFix {
    return <- NmtSsruFix(h <- [for (i in range(2 * NMT_FIX_D)); float((i * 29) % 41) * 0.15 - 3.0],
                         wn <- [for (i in range(NMT_FIX_LEAD + NMT_FIX_D)); float((i * 11) % 23) * 0.1 - 1.1],
                         cell <- [for (i in range(2 * NMT_FIX_D)); float((i * 7) % 19) * 0.2 - 1.8])
}

//! The CPU loop of `nmt_decode_step`'s SSRU on layer 1's cell: the cells after it and the output row.
def nmt_ssru_ref(f : NmtSsruFix) : tuple<cell : array<float>; o : array<float>> {
    var cell := f.cell
    var o : array<float>
    o |> reserve(NMT_FIX_D)
    for (i in range(NMT_FIX_D)) {
        let g = 1.0 / (1.0 + exp(-(f.h[NMT_FIX_D + i] + f.wn[NMT_FIX_LEAD + i])))
        let c = g * cell[NMT_FIX_D + i] + (1.0 - g) * f.h[i]
        cell[NMT_FIX_D + i] = c
        o |> push(max(c, 0.0))
    }
    return (cell = cell, o = o)
}

//! One pick case: the logits row, the bias plane (the bias at `NMT_FIX_LEAD`), the state words before, the cap.
struct NmtPickCase {
    tag : string
    lg : array<float>
    wb : array<float>
    st : array<uint>
    eos : uint
    cap : uint
}

//! The pick's cases at a vocabulary of `nv`: a plain step, a tie (the lower index wins), the end token, the cap's last step,
//! and a sentence already done (nothing written).
def nmt_pick_cases(nv : int) : array<NmtPickCase> {
    var cases : array<NmtPickCase>
    cases |> reserve(5)
    for (k in range(5)) {
        var c = NmtPickCase(eos = 0u, cap = 9u)
        c.lg <- [for (i in range(nv)); float((i * 7919) % 997) * 0.001]
        c.wb <- [for (i in range(NMT_FIX_LEAD + nv)); i < NMT_FIX_LEAD ? 99.0 : float(i % 3) * 0.0001]
        c.st <- [4u, 17u, 0u, 4u]
        let hot = nv - 2
        c.lg[hot] = 50.0
        if (k == 0) {
            c.tag = "step"
        } elif (k == 1) {
            c.tag = "tie"
            c.lg[nv / 3] = 50.0
            c.wb[NMT_FIX_LEAD + nv / 3] = c.wb[NMT_FIX_LEAD + hot]
        } elif (k == 2) {
            c.tag = "eos"
            c.lg[0] = 60.0
        } elif (k == 3) {
            c.tag = "cap"
            c.st[0] = 8u
        } else {
            c.tag = "done"
            c.st[2] = 1u
        }
        cases |> emplace(c)
    }
    return <- cases
}

//! The pick case's control leg: the bias at index 7 pushed past every logit, a done case also reopened - every case's state or
//! record then moves off its clean expectation
def nmt_pick_poisoned(c : NmtPickCase) : NmtPickCase {
    var cp := c
    cp.wb[NMT_FIX_LEAD + 7] += 100.0
    cp.st[NMT_ST_DONE] = 0u
    return <- cp
}

//! the record plane `n` words long a case's step leaves: the sentinel everywhere but its record at the step `t`
def nmt_pick_toks(c : NmtPickCase; rec : uint; n : int) : array<uint> {
    var toks <- [for (_i in range(n)); NMT_NO_TOKEN]
    toks[int(c.st[NMT_ST_T])] = rec
    return <- toks
}

//! a pick dispatch's state words and record plane against the reference's step on `c`, every word exact
def nmt_pick_check(t : T?; tag : string; st, toks : array<uint>; c : NmtPickCase) {
    var inscope want <- nmt_pick_ref(c)
    var inscope want_toks <- nmt_pick_toks(c, want.rec, length(toks))
    check_exact(t, "{tag}: the state words", st, want.st)
    check_exact(t, "{tag}: the token records, the slack past the cap among them", toks, want_toks)
}

//! the control: the poisoned leg's state words and records off the clean case's expectation
def nmt_pick_moved(t : T?; tag : string; st, toks : array<uint>; clean : NmtPickCase) {
    var inscope want <- nmt_pick_ref(clean)
    var inscope want_toks <- nmt_pick_toks(clean, want.rec, length(toks))
    let moved = mismatch_exact(st, want.st) + mismatch_exact(toks, want_toks)
    to_log(LOG_INFO, "{tag}: the poisoned leg moves {moved} words off the clean expectation, the first record at {first_mismatch(toks, want_toks)}\n")
    t |> success(moved > 0, "{tag}: the control - the poisoned leg moves the state or the records off the clean expectation")
}

//! The CPU loop's step on a case: the state words after it and the token record it writes at `t` (`NMT_NO_TOKEN` for none).
def nmt_pick_ref(c : NmtPickCase) : tuple<st : array<uint>; rec : uint> {
    var st := c.st
    var best = 0
    for (i in range(1, length(c.lg))) {
        if (c.lg[i] + c.wb[NMT_FIX_LEAD + i] > c.lg[best] + c.wb[NMT_FIX_LEAD + best]) {
            best = i
        }
    }
    var rec = NMT_NO_TOKEN
    if (st[NMT_ST_DONE] == 0u) {
        let t = st[NMT_ST_T]
        if (uint(best) == c.eos) {
            st[NMT_ST_DONE] = 1u
            st[NMT_ST_NTOK] = t
        } else {
            rec = uint(best)
            st[NMT_ST_NTOK] = t + 1u
            st[NMT_ST_PREV] = uint(best)
            st[NMT_ST_T] = t + 1u
            if (t + 1u >= c.cap) {
                st[NMT_ST_DONE] = 1u
            }
        }
    }
    return (st = st, rec = rec)
}
