// The e2e transducer's and the CTC carriers' device twins of the GigaAM family, as cells of test_gigaam.das's blocks: a
// form's legs on the Metal home and on the discrete home, each keeping its rows, ids and transcripts (`GaKept`), the kq
// lane's legs on a quantized carrier's block, the CTC head's device seat against the CPU head, and the compares that
// follow a block on what its forms kept. No load: every model is a form of its file's block.

options gen2
options _dasllama_internal = true

module _audio_gigaam_cells shared public

require dastest/testing_boost public
require dasllama/dasllama_gigaam public
require dasllama/dasllama_audio
require dasllama/dasllama_audio_io
require dasllama/dasllama_tower
require dasllama/dasllama_math   // with_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio   // stat
require daslib/linq   // sequence_equal: the head twins' ids
require daslib/strings_boost   // split: a kq carrier's members by name
require strings
require math
require _model_tier   // asr_clip, tower_parity_stats
require _compares
require _tower_twin public   // TwinRows and its legs, the knobs, tower_form_of
require _audio_twin   // x_rows_encode, blocks_refused_leg, the kq lane's counters
require _ctc_oracle public   // CtcLeg, ctc_leg / ctc_cpu_leg / ctc_hard_flips
require ?das_metal dasllama/dasllama_metal_tower
require ?das_metal dasllama/dasllama_metal_common   // set_metal_take_max_probe_: the scratch declines' seam
require ?das_metal dasllama/dasllama_metal_prefill   // pf_hmm_ready: the half tensor GEMM, the exact lane's device condition
require ?vulkan dasllama/dasllama_vulkan_tower   // gigaam's standalone mel on the Vulkan home
require ?cuda dasllama/dasllama_cuda_tower_fc   // cuda_gigaam_mel: gigaam's standalone device mel on the CUDA home

//! max |device - CPU| over the CPU rows' largest magnitude: the device GEMMs stage W and X through halves, about 1e-3 of the rows' scale over sixteen blocks, so 1e-2 is that floor with headroom and a poisoned clip (x3, the log-mel moved by ln 9) reads at the rows' own scale.
let private GA_TWIN_BAR = 1e-2

//! the device log-mel against the CPU's, each element within 1e-5 of its magnitude floored at 1: the device frames are the
//! CPU's bit for bit and both sides sum in float, so the log reads within an ulp of the clip's loudest value
let private GA_MEL_BAR = 1e-5

struct GaLeg {
    n : int64
    x : array<float>
    ids : array<int>
    text : string
}

//! one GigaAM log-mel, encode and greedy decode of `samples` through the span encode on the current tower knobs (the device mel
//! where the hook serves); `scale` scales the samples ahead of it
def ga_leg(m : GigaamModel; samples : array<float>; scale : float = 1.0) : GaLeg {
    var st = GigaamState()
    var r = GaLeg()
    var inscope clip := samples
    for (v in clip) {
        v *= scale
    }
    r.n = gigaam_encode_span(clip, 0l, long_length(clip), m, st)
    r.x <- copy_span(st.x, 0l, r.n * m.d_model)
    gigaam_decode(m, st, r.n)
    r.ids <- [for (k in st.toks); k.id]
    r.text = gigaam_detokenize(m, st.toks)
    delete st
    return <- r
}

//! the device rows' bar: within ``GA_TWIN_BAR`` of the CPU rows' largest magnitude, the poisoned expected element that
//! magnitude off
def private ga_bar() : Bar => Bar(kind = BarKind.rel_of_max, rel = GA_TWIN_BAR, poison = 1.0, poison_of_max = true)

//! each element's bar: ``GA_TWIN_BAR`` of its own frame's largest magnitude in `want`, frames `d` wide
def private ga_frame_bars(want : array<float>; d : int64) : array<float> {
    var bars : array<float>
    bars |> reserve(length(want))
    for (f in range64(long_length(want) / d)) {
        var peak = 0.0
        for (i in range64(f * d, (f + 1l) * d)) {
            peak = max(peak, abs(want[i]))
        }
        for (_ in range64(d)) {
            bars |> push(GA_TWIN_BAR * peak)
        }
    }
    return <- bars
}

//! the device rows against the CPU's at the twin bar twice: over the rows' largest magnitude, and frame by frame over each frame's
//! own, so a quiet frame's error cannot hide under a loud frame's scale
def private ga_check_rows(t : T?; tag : string; got, want : array<float>; d : int64) {
    check_rows(t, tag, got, want, ga_bar(), int(d))
    var inscope bars <- ga_frame_bars(want, d)
    check(t, "{tag}, per frame", got, want, Bar(kind = BarKind.per_element, poison = 1.0, poison_of_max = true), Shape(row_width = int(d)), bars)
}

//! the device log-mel alone (`metal_gigaam_mel`, the encode chain's mel dispatches) against `gigaam_log_mel` at ``GA_MEL_BAR``,
//! the frame count and the counter beside it, the bar's poisoned element the control
[unused_argument(t, m, samples, clip)]   // off-Apple the gated body is empty
def private ga_mel_twin_cell(t : T?; m : GigaamModel; samples : array<float>; clip : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var st = GigaamState()
        gigaam_log_mel(samples, m, st)
        var inscope dev : array<float>
        let mel0 = metal_tower_ga_mels()
        let n = metal_gigaam_mel(m, samples, dev)
        t |> equal(n, st.n_len, "{clip}: the device mel's frame count")
        t |> equal(metal_tower_ga_mels() - mel0, 1l, "{clip}: the mel seat ran on the device")
        check_rows(t, "gigaam {clip} metal: the device log-mel tracks the CPU's", dev, st.mel, Bar(rel = GA_MEL_BAR, poison_scaled = true), int(m.n_mel))
        var inscope loud <- [for (v in samples); v * 1.1]
        var inscope dev_loud : array<float>
        metal_gigaam_mel(m, loud, dev_loud)
        check_exceeds(t, "gigaam {clip} metal: the control - the clip at 1.1x on the device side off the CPU's log-mel", dev_loud, st.mel,
            Bar(rel = GA_MEL_BAR))
        delete st
    }
}

//! the head of `samples` whose encoder frame count is the largest multiple of 64 the clip holds - the rotary chain's attention
//! with no pad rows; empty where the clip holds under 64 frames
def private ga_lattice_clip(m : GigaamModel; samples : array<float>) : array<float> {
    let frames = $(n : int64) => gigaam_conv_out(gigaam_conv_out(n >= m.n_fft ? (n - m.n_fft) / m.hop + 1l : 0l, m.subs_kernel), m.subs_kernel)
    var n = long_length(samples)
    let target = invoke(frames, n) / 64l * 64l
    while (target > 0l && invoke(frames, n) > target) {
        n -= m.hop
    }
    return <- copy_span(samples, 0l, target > 0l ? n : 0l)
}

//! the device's declines that keep a lane off it on this box: `device` (no form for the lane's GEMMs), or `flavor` (the lane
//! loaded its CPU image)
def private off_device_declines() : int64
    => gpu_tower_declines_of("device", TowerFamily.gigaam) + gpu_tower_declines_of("flavor", TowerFamily.gigaam)

//! gigaam's device log-mel alone on its home (the encode chain's mel record) into `mel`: its frame count
[unused_argument(m, samples, mel)]
def private gpu_gigaam_mel(m : GigaamModel; samples : array<float>; @exact_size var mel : array<float>) : int64 {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return cuda_gigaam_mel(m, samples, mel) if (tower_family_cuda(TowerFamily.gigaam))
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vulkan_gigaam_mel(m, samples, mel)
    } else {
        return 0l
    }
}

def private ga_twin_encode(m : GigaamModel; var st : GigaamState; clip : array<float>; var out : array<float>) : int64 {
    return x_rows_encode(st, m.d_model, out) $(var s) {
        return gigaam_encode_span(clip, 0l, long_length(clip), m, s)
    }
}

//! gigaam's device log-mel alone on its home against `gigaam_log_mel` at `GA_MEL_BAR`, frame count and counter; a poisoned element the control
[unused_argument(t, m, samples, clip)]   // without a discrete home the gated body is empty
def private ga_discrete_mel_twin_cell(t : T?; m : GigaamModel; samples : array<float>; clip : string) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        var st = GigaamState()
        with_dasllama_jobque_() {
            gigaam_log_mel(samples, m, st)
        }
        var inscope dev : array<float>
        let s0 = gpu_tower_counters(TowerFamily.gigaam)
        let n = gpu_gigaam_mel(m, samples, dev)
        t |> equal(n, st.n_len, "{clip}: the device mel's frame count")
        t |> equal(gpu_tower_counters(TowerFamily.gigaam).mels - s0.mels, 1l, "{clip}: the mel seat ran on the device")
        check_rows(t, "gigaam {clip} {serving_tower_dev(TowerFamily.gigaam)}: the device log-mel tracks the CPU's", dev, st.mel, Bar(rel = GA_MEL_BAR, poison_scaled = true), int(m.n_mel))
        var inscope loud <- [for (v in samples); v * 1.1]
        var inscope dev_loud : array<float>
        gpu_gigaam_mel(m, loud, dev_loud)
        check_exceeds(t, "gigaam {clip} {serving_tower_dev(TowerFamily.gigaam)}: the control - the clip at 1.1x on the device side off the CPU's log-mel", dev_loud, st.mel,
            Bar(rel = GA_MEL_BAR))
        delete st
    }
}

//! GigaAM's lane policy on its discrete home: tower on, the lane knob picks q8 or exact; tower off, q8 whatever the knob; the knob restored
[unused_argument(t)]   // without a discrete home the gated body is empty
def private ga_discrete_lane_policy_cell(t : T?) {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        with_gpu_knobs() {
            reset_gigaam_q8()   // the cell reads the un-pinned policy's lane
            set_serving_gpu_tower(true, TowerFamily.gigaam)
            let was = set_gpu_gigaam_exact_lane(false)
            t |> success(gigaam_gpu_serves() && gigaam_serves_q8(), "the lane knob off: the un-pinned lane is the q8 image")
            set_gpu_gigaam_exact_lane(true)
            t |> success(!gigaam_serves_q8(), "the lane knob on: the un-pinned lane is the exact planes")
            set_serving_gpu_tower(false, TowerFamily.gigaam)
            t |> success(gigaam_serves_q8(), "the tower off: the un-pinned lane is the q8 image whatever the knob (the control)")
            set_gpu_gigaam_exact_lane(was)
        }
    }
}

//! the K-quant members of a carrier's members' names `members`, Q8_0 aside, by the names their tiles take
def private kq_members_of(members : string) : array<string> {
    let names <- split(members, "+")
    return <- [for (name in names); name; where name != "q8"]
}

//! the clips the e2e transducer's twins read, in the order the kept twins index them
let GA_CLIPS = fixed_array("ru_short", "ru_mid")

//! the discrete kq twin's device rows off the CPU kq chain's, in rel_l2: 0.021 - 0.023 measured on the Vulkan and the
//! CUDA home over both clips; the rows-held half of the twin's word compare
let private GA_KQ_DEV_CPU_REL_L2 = 0.05lf

//! a GigaAM carrier's form read off its name (`tower_form_of`): the lanes `f16` (the exact lane off the carrier's
//! halfwords), `q8`, `kq` (a quantized carrier's own planes) and `f32` (a quantized carrier widened under the exact pin)
def gigaam_form_of(form : string) : TowerForm => tower_form_of(form, ["f16", "q8", "kq", "f32"], "gigaam")

//! one leg kept under its name
struct GaNamed {
    key : string
    leg : GaLeg
}

//! One CTC head twin as two forms keep it: the device form's encode rows `rows` and its three head legs (the encode's own
//! rows, the 1.05x control, the uploaded rows), then the CPU form's head leg over `rows`. `legs` counts the forms that kept.
struct GaCtc {
    tag : string
    q8 : bool
    n : int64
    n_classes : int64
    rows : array<float>
    cpu : CtcLeg
    dev : CtcLeg
    moved : CtcLeg
    up : CtcLeg
    legs : int
}

//! What the forms of the e2e transducer's carriers and of the CTC carriers keep for the compares after their blocks:
//! `form` the form whose cells run; on a quantized carrier `carrier` its key in the kept names (`k4`, `mix`) and `fmt` its
//! lane's name; the legs by name (`<clip>/<leg>`, a quantized carrier's `<carrier>/<clip>/<leg>`), counters and names the
//! compares read, the discrete q8 lane's rows three ways a clip of `GA_CLIPS`, and the CTC head twins.
struct GaKept {
    form : string
    carrier : string
    fmt : string
    d_model : int64
    n_layer : int64
    legs : array<GaNamed>
    nums : table<string; int64>
    strs : table<string; string>
    twins : array<TwinRows>
    ctc : array<GaCtc>
}

//! the kept state, a twin a clip
def ga_kept() : GaKept {
    var k = GaKept()
    k.twins |> reserve(length(GA_CLIPS))
    for (_clip in GA_CLIPS) {
        k.twins |> emplace(TwinRows())
    }
    return <- k
}

//! `leg` kept under `key`
def ga_keep(var k : GaKept; key : string; var leg : GaLeg) {
    var n = GaNamed(key = key)
    n.leg <- leg
    k.legs |> emplace(n)
}

//! the kept leg named `key`: its index, -1 where no form kept it
def ga_kept_at(k : GaKept; key : string) : int {
    for (n, i in k.legs, count()) {
        return i if (n.key == key)
    }
    return -1
}

//! every leg of `keys` kept, each miss a failure naming it
def private ga_all_kept(t : T?; k : GaKept; keys : array<string>) : bool {
    var all = true
    for (key in keys) {
        let kept = ga_kept_at(k, key) >= 0
        t |> success(kept, "the leg {key} was kept by its form")
        all = all && kept
    }
    return all
}

//! a quantized carrier's kept name of `clip`'s leg `leg`
def private kq_key(k : GaKept; clip, leg : string) : string => "{k.carrier}/{clip}/{leg}"

//! the carrier's kept name `what` (its members, its device leg's flavor), "" where no form kept it
def private kq_str(k : GaKept; what : string) : string {
    let key = "{k.carrier}/{what}"
    return k.strs?[key] ?? ""
}

// ===== the Metal home =====

//! the exact lane's CPU leg of one clip, on the `f16` form: the CPU chain with the tower off, the driver's counters still
[unused_argument(t, mc, k, samples, clip)]   // off-Apple the gated body is empty
def private ga_metal_exact_cpu(t : T?; mc : GigaamModel; var k : GaKept; samples : array<float>; clip : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(false)
        let c0 = metal_tower_stats()
        var cpu <- ga_leg(mc, samples)
        let c1 = metal_tower_stats()
        t |> equal(c1.encodes - c0.encodes, 0l, "{clip}: the tower-off leg never dispatched")
        t |> success(c1.declines - c0.declines >= 1l, "{clip}: the tower-off leg counted the knob decline")
        ga_keep(k, "{clip}/cpu", cpu)
    }
}

//! the exact lane's device legs of one clip, on the `f16@dev` form - the Metal-flavored image, its halfword planes on the half
//! tensor GEMM: the device mel alone, the encode with every stage counted, and a poisoned clip; the f16 twin route pinned on
//! and put back
[unused_argument(t, m, k, samples, clip)]   // off-Apple the gated body is empty
def private ga_metal_exact_dev(t : T?; m : GigaamModel; var k : GaKept; samples : array<float>; clip : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let f16_found = get_metal_tower_f16()
        set_metal_tower_f16(true)
        set_metal_tower(true)
        ga_mel_twin_cell(t, m, samples, clip)
        let g0 = metal_tower_stats()
        let f0 = metal_tower_f16_encodes()
        let mel0 = metal_tower_ga_mels()
        let kq0 = kq_mm_count()
        var dev <- ga_leg(m, samples)
        let g1 = metal_tower_stats()
        k.nums["{clip}/twin"] = metal_tower_f16_encodes() - f0
        t |> equal(kq_mm_count() - kq0, 0l, "{clip}: the exact lane dispatches no K-quant mul_mm stamp (the kq lane counter's control)")
        t |> equal(g1.encodes - g0.encodes, 1l, "{clip}: the device served the encode")
        t |> equal(metal_tower_ga_mels() - mel0, 1l, "{clip}: the log-mel ran on the device, inside the encode's command buffer")
        t |> equal(g1.convs - g0.convs, 1l, "{clip}: the conv front ran on the device")
        t |> equal(g1.blocks - g0.blocks, m.n_layer, "{clip}: every block ran on the driver")
        var bad <- ga_leg(m, samples, 3.0)
        set_metal_tower_f16(f16_found)
        ga_keep(k, "{clip}/dev", dev)
        ga_keep(k, "{clip}/bad", bad)
    }
}

//! the q8 lane's device legs of one clip: on the `q8@dev` form the encode off the q8 slab with every stage counted and no
//! `quant_mode` decline; on the `q8_z<block>@dev` form the poisoned image's rows
[unused_argument(t, mqd, k, poisoned, samples, clip)]   // off-Apple the gated body is empty
def private ga_metal_q8_dev(t : T?; mqd : GigaamModel; var k : GaKept; poisoned : bool; samples : array<float>; clip : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let tag = "gigaam {clip} q8 lane"
        set_metal_tower(true)
        if (poisoned) {
            var bad <- ga_leg(mqd, samples)
            ga_keep(k, "{clip}/q8_bad", bad)
            return
        }
        let q0 = metal_tower_stats()
        let mel0 = metal_tower_ga_mels()
        let qd0 = metal_tower_declines()?["quant_mode"] ?? 0l
        var dev <- ga_leg(mqd, samples)
        let q1 = metal_tower_stats()
        t |> equal(q1.encodes - q0.encodes, 1l, "{tag}: the device served the encode")
        t |> equal(q1.blocks - q0.blocks, mqd.n_layer, "{tag}: every block ran on the driver")
        t |> equal(metal_tower_ga_mels() - mel0, 1l, "{tag}: the log-mel ran on the device")
        t |> equal((metal_tower_declines()?["quant_mode"] ?? 0l) - qd0, 0l, "{tag}: no quant_mode decline")
        ga_keep(k, "{clip}/q8_dev", dev)
    }
}

//! the GigaAM device seats' side arms on one clip, on the `f16@dev` form: the mel seat's declines (the knob, a clip under one
//! window, a scratch past the largest buffer), the encode off an uploaded mel - its rows kept for the compare with the CPU
//! chain's -, the held rows' key, and a scratch the tower cannot take refusing the CPU chain
[cold_path, unused_argument(t, m, k, samples)]   // off-Apple the gated body is empty
def private ga_seat_arms_dev(t : T?; m : GigaamModel; var k : GaKept; samples : array<float>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(true)
        var inscope dev : array<float>
        let mel0 = metal_tower_ga_mels()
        set_metal_tower(false)
        t |> equal(metal_gigaam_mel(m, samples, dev), -1l, "mel seat: the knob off declines")
        set_metal_tower(true)
        var inscope tiny : array<float>
        tiny |> resize(int(m.n_fft) - 1)
        t |> equal(metal_gigaam_mel(m, tiny, dev), -1l, "mel seat: a clip under one window declines")
        let take_was = set_metal_take_max_probe_(512ul)
        t |> equal(metal_gigaam_mel(m, samples, dev), -1l, "mel seat: a scratch past the device's largest buffer declines")
        set_metal_take_max_probe_(take_was)
        t |> equal(metal_tower_ga_mels() - mel0, 0l, "mel seat: no declined call ran a mel")
        var st = GigaamState()
        gigaam_log_mel(samples, m, st)
        let e0 = metal_tower_stats()
        let n = gigaam_encode(m, st)
        let e1 = metal_tower_stats()
        t |> equal(e1.encodes - e0.encodes, 1l, "uploaded mel: the encode served on the device")
        t |> equal(e1.convs - e0.convs, 1l, "uploaded mel: the conv front ran on the device")
        t |> equal(metal_tower_ga_mels() - mel0, 0l, "uploaded mel: no mel ran on the device")
        var up = GaLeg(n = n)
        up.x <- copy_span(st.x, 0l, n * m.d_model)
        ga_keep(k, "arms/up", up)
        let elems = n * m.d_model
        t |> success(st.enc_gen != 0ul, "the served encode recorded its generation ({st.enc_gen})")
        t |> success(metal_tower_enc_rows(st.enc_gen, elems) != null, "the served encode's rows are held under the generation it recorded")
        t |> equal(metal_tower_held_rows_elems(), elems, "the hold's element count")
        t |> success(metal_tower_enc_rows(0ul, elems) == null, "generation 0 - no device encode - hands off nothing")
        t |> success(metal_tower_enc_rows(st.enc_gen, elems + 1l) == null, "another element count hands off nothing")
        var st2 = GigaamState()
        gigaam_log_mel(samples, m, st2)
        gigaam_encode(m, st2)
        t |> success(st2.enc_gen != st.enc_gen, "a second session's encode records its own generation")
        t |> success(metal_tower_enc_rows(st.enc_gen, elems) == null, "after a second session's encode the first session's generation hands off nothing")
        t |> success(metal_tower_enc_rows(st2.enc_gen, elems) != null, "the second session's generation hands its rows off")
        delete st2
        delete st
        let s0 = metal_tower_stats()
        set_metal_take_max_probe_(512ul)
        var refused = false
        try {
            var inscope mem <- ga_leg(m, samples)
        } recover {
            refused = true
        }
        set_metal_take_max_probe_(take_was)
        t |> equal(metal_tower_stats().encodes - s0.encodes, 0l, "a scratch the tower cannot take: no encode on the device")
        t |> success(refused, "... the Metal-flavored model refuses the CPU chain")
    }
}

//! the asr_prof buckets a GigaAM transcribe fills, the CPU chain's on the `f16` form: its stage and block buckets with the
//! tower off, no device time; the profiler put back as found
[cold_path, unused_argument(t, mc, k, samples)]   // off-Apple the gated body is empty
def private ga_prof_cpu_leg(t : T?; mc : GigaamModel; var k : GaKept; samples : array<float>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prof_found = asr_prof_on()
        set_asr_prof(true)
        asr_prof_reset()
        set_metal_tower(false)
        var cpu <- ga_leg(mc, samples)
        for (b in ["ga.mel", "ga.front", "ga.blocks", GIGAAM_ENCODE_BUCKET, "ga.ffn", "ga.attn_heads", "ga.conv_module"]) {
            t |> success(asr_prof_ms(b) > 0.0lf, "tower off: the CPU chain fills {b} ({asr_prof_ms(b)} ms)")
        }
        t |> equal(asr_prof_ms("ga.encode.gpu"), 0.0lf, "tower off: no device time")
        asr_prof_reset()
        set_asr_prof(prof_found)
        ga_keep(k, "prof/cpu", cpu)
    }
}

//! the asr_prof buckets' device side, on the `f16@dev` form: the device chain's command-buffer time with the tower on, and
//! the rotary chain's replayed restrides and slab trio under the replay alone; the replay pinned off at the head, the replay
//! and the profiler put back as found
[cold_path, unused_argument(t, m, k, samples)]   // off-Apple the gated body is empty
def private ga_prof_dev_leg(t : T?; m : GigaamModel; var k : GaKept; samples : array<float>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let replay_found = set_metal_fc_prof_replay(false)
        let prof_found = asr_prof_on()
        set_asr_prof(true)
        set_metal_tower(true)
        asr_prof_reset()
        var dev <- ga_leg(m, samples)
        for (b in ["ga.encode.gpu", "ga.encode.record", GIGAAM_ENCODE_BUCKET]) {
            t |> success(asr_prof_ms(b) > 0.0lf, "tower on: the device chain fills {b} ({asr_prof_ms(b)} ms)")
        }
        t |> equal(asr_prof_ms("ga.encode.gpu.attn"), 0.0lf, "replay off: no replayed slab trio")
        set_metal_fc_prof_replay(true)
        asr_prof_reset()
        var rep <- ga_leg(m, samples)
        set_metal_fc_prof_replay(replay_found)
        for (b in ["ga.encode.gpu.restride", "ga.encode.gpu.attn"]) {
            t |> success(asr_prof_ms(b) > 0.0lf, "replay on: the replay fills {b} ({asr_prof_ms(b)} ms)")
        }
        asr_prof_reset()
        var inscope mel : array<float>
        metal_gigaam_mel(m, samples, mel)
        t |> success(asr_prof_ms("ga.mel.gpu") > 0.0lf, "the mel seat fills ga.mel.gpu")
        set_asr_prof(prof_found)
        ga_keep(k, "prof/dev", dev)
        ga_keep(k, "prof/rep", rep)
    }
}

//! one clip's legs of the form `f`, the knobs put back
[unused_argument(t, m, k, f, samples, clip)]   // off-Apple the gated body is empty
def private ga_metal_clip_legs(t : T?; m : GigaamModel; var k : GaKept; f : TowerForm; samples : array<float>; clip : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_gpu_knobs() {
            if (f.lane == "f16" && f.dev) {
                ga_metal_exact_dev(t, m, k, samples, clip)
            } elif (f.lane == "f16") {
                ga_metal_exact_cpu(t, m, k, samples, clip)
            } elif (f.dev) {
                ga_metal_q8_dev(t, m, k, f.zero_block >= 0l, samples, clip)
            } else {
                set_metal_tower(false)
                var cpu <- ga_leg(m, samples)
                ga_keep(k, "{clip}/q8_cpu", cpu)
            }
        }
        if (clip == "ru_short" && f.lane == "f16") {
            with_gpu_knobs() {
                if (f.dev) {
                    ga_seat_arms_dev(t, m, k, samples)
                    ga_prof_dev_leg(t, m, k, samples)
                } else {
                    ga_prof_cpu_leg(t, m, k, samples)
                }
            }
        }
    }
}

//! the un-pinned lane's policy under the Metal tower, on the `f16@dev` form: the driver's faster lane with the tower on,
//! the q8 planes with it off
[unused_argument(t)]   // off-Apple the gated body is empty
def private ga_metal_lane_policy(t : T?) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_gpu_knobs() {
            set_metal_tower(true)
            let ga_q8 = gigaam_serves_q8()
            t |> equal(ga_q8, !gigaam_gpu_exact_serves(), "the tower on: the un-pinned lane follows the driver's faster lane")
            t |> equal(!ga_q8, get_metal_tower_f16() && pf_hmm_ready(), "the tower on: the un-pinned lane is the f16 planes where the toolchain compiled the half tensor GEMM, the q8 slab on a box without it")
            set_metal_tower(false)
            t |> success(gigaam_serves_q8(), "the tower off: the un-pinned lane is the q8 planes (the control)")
        }
    }
}

// A form's legs of the Metal twin of the e2e transducer's f16 carrier, over the FLEURS ru clips: `f16` the CPU exact chain
// (the reference of every lane), `f16@dev` the Metal rotary FastConformer chain with its seats' side arms and its
// profiler buckets, `q8` the CPU q8 chain, `q8@dev` the q8 slab's chain, `q8_z<block>@dev` its poison.
[cold_path, unused_argument(t, m, k)]   // off-Apple the gated body is empty
def ga_metal_legs(t : T?; m : GigaamModel; var k : GaKept) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let f = gigaam_form_of(k.form)
        k.d_model = m.d_model
        k.n_layer = m.n_layer
        with_dasllama_jobque_() {
            for (clip in GA_CLIPS) {
                if (!asr_clip_present("{clip}.wav")) {
                    t |> skip("{clip}.wav not present")
                    continue
                }
                var inscope samples <- load_audio_16k_mono(asr_clip("{clip}.wav"))
                ga_metal_clip_legs(t, m, k, f, samples, clip)
                if (clip == "ru_mid" && f.lane == "f16") {
                    var inscope head <- ga_lattice_clip(m, samples)
                    if (empty(head)) {
                        t |> skip("ru_mid holds under 64 encoder frames - no pad-free clip")
                        continue
                    }
                    ga_metal_clip_legs(t, m, k, f, head, "ru_mid lattice")
                }
            }
        }
        if (f.lane == "f16" && f.dev) {
            ga_metal_lane_policy(t)
        }
    }
}

//! one clip's exact-lane verdict on the Metal chain: the frame count, the device rows within the twin bar of the CPU's
//! (whole, and frame by frame), the transcript, and the poisoned clip outside the bar
def private ga_clip_held(t : T?; clip : string; d_model, twin : int64; cpu, dev, bad : GaLeg) {
    t |> equal(dev.n, cpu.n, "{clip}: the frame count")
    ga_check_rows(t, "gigaam {clip} metal ({twin > 0l ? "twin" : "f32 tiles"}): the device rows track the CPU's", dev.x, cpu.x, d_model)
    t |> equal(dev.text, cpu.text, "{clip}: the transcript")
    to_log(LOG_INFO, "test_audio: gigaam {clip} cpu: {cpu.text}\n")
    to_log(LOG_INFO, "test_audio: gigaam {clip} gpu: {dev.text}\n")
    check_exceeds(t, "gigaam {clip} metal: a poisoned clip reds the bar", bad.x, cpu.x, ga_bar())
}

//! one clip's exact-lane verdict off the kept legs (`ga_clip_held`); the CPU chain's frame count, -1 where a leg is missing
def private ga_clip_verdict(t : T?; k : GaKept; clip : string) : int64 {
    return -1l if (!ga_all_kept(t, k, ["{clip}/cpu", "{clip}/dev", "{clip}/bad"]))
    ga_clip_held(t, clip, k.d_model, k.nums?["{clip}/twin"] ?? 0l, k.legs[ga_kept_at(k, "{clip}/cpu")].leg, k.legs[ga_kept_at(k, "{clip}/dev")].leg,
        k.legs[ga_kept_at(k, "{clip}/bad")].leg)
    return k.legs[ga_kept_at(k, "{clip}/cpu")].leg.n
}

//! the q8 lane on the Metal chain, off the q8 slab, against the CPU q8 chain with the exact chain the reference: the device
//! rows' rel_l2 from the exact chain within ``TWIN_SLACK_VK`` x the CPU q8 chain's own plus ``TWIN_REL_L2_FLOOR``, the token ids
//! the CPU q8 chain's; the control a q8 image with blocks from the middle on zeroed, through the device chain, past the bar
def private ga_q8_held(t : T?; clip : string; ex, cpu, dev, bad : GaLeg) {
    let tag = "gigaam {clip} q8 lane"
    t |> equal(dev.n, cpu.n, "{tag}: the frame count")
    t |> equal(dev.ids, cpu.ids, "{tag}: the token ids are the CPU q8 chain's")
    to_log(LOG_INFO, "test_audio: {tag} cpu: {cpu.text}\n")
    to_log(LOG_INFO, "test_audio: {tag} gpu: {dev.text}\n")
    var q8_ex = TowerParityStats()
    var dev_ex = TowerParityStats()
    if (tower_parity_stats(t, "{tag} cpu q8 vs exact", ex.x, cpu.x, q8_ex) && tower_parity_stats(t, "{tag} metal vs exact", ex.x, dev.x, dev_ex)) {
        let bar = double(TWIN_SLACK_VK) * q8_ex.rel_l2 + TWIN_REL_L2_FLOOR
        to_log(LOG_INFO, "test_audio: {tag}: rel_l2 from the exact chain - cpu q8 {q8_ex.rel_l2}, metal {dev_ex.rel_l2}, the bar {bar}\n")
        t |> success(dev_ex.rel_l2 <= bar, "{tag}: the device rows within {TWIN_SLACK_VK}x the CPU q8 chain's rel_l2 from the exact chain ({dev_ex.rel_l2} vs {q8_ex.rel_l2})")
        var bad_ex = TowerParityStats()
        if (tower_parity_stats(t, "{tag} poisoned metal vs exact", ex.x, bad.x, bad_ex)) {
            t |> success(is_finite(bad_ex.rel_l2) && bad_ex.rel_l2 > bar, "{tag}: zeroed blocks through the device chain EXCEED the bar ({bad_ex.rel_l2} vs {bar})")
        }
    }
}

// GigaAM v3 e2e-rnnt on the Metal home, after the f16 carrier's block, on what its forms kept: the rotary FastConformer
// chain against the CPU exact chain on the FLEURS ru clips and on ru_mid's pad-free head, the q8 lane beside it, the
// uploaded mel's rows and the profiled passes' transcripts.
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_FC.md#tower-fc-rotary")]
def test_gigaam_metal_twin(t : T?; k : GaKept) {
    for (clip in GA_CLIPS) {
        if (!asr_clip_present("{clip}.wav")) {
            t |> skip("{clip}.wav not present")
            continue
        }
        ga_clip_verdict(t, k, clip)
        if (ga_all_kept(t, k, ["{clip}/cpu", "{clip}/q8_cpu", "{clip}/q8_dev", "{clip}/q8_bad"])) {
            ga_q8_held(t, clip, k.legs[ga_kept_at(k, "{clip}/cpu")].leg, k.legs[ga_kept_at(k, "{clip}/q8_cpu")].leg,
                k.legs[ga_kept_at(k, "{clip}/q8_dev")].leg, k.legs[ga_kept_at(k, "{clip}/q8_bad")].leg)
        }
        if (clip == "ru_short" && ga_all_kept(t, k, ["{clip}/cpu", "arms/up", "prof/cpu", "prof/dev", "prof/rep"])) {
            let cpu = ga_kept_at(k, "{clip}/cpu")
            let up = ga_kept_at(k, "arms/up")
            t |> equal(k.legs[up].leg.n, k.legs[cpu].leg.n, "uploaded mel: the frame count")
            ga_check_rows(t, "gigaam metal, uploaded mel: the device rows track the CPU's", k.legs[up].leg.x, k.legs[cpu].leg.x, k.d_model)
            let prof = k.legs[ga_kept_at(k, "prof/cpu")].leg.text
            t |> equal(k.legs[ga_kept_at(k, "prof/dev")].leg.text, prof, "the profiled device pass's transcript")
            t |> equal(k.legs[ga_kept_at(k, "prof/rep")].leg.text, prof, "the replayed pass's transcript")
        }
        if (clip == "ru_mid") {
            if (ga_kept_at(k, "ru_mid lattice/cpu") < 0) {
                t |> skip("ru_mid holds under 64 encoder frames - no pad-free clip")
                continue
            }
            let frames = ga_clip_verdict(t, k, "ru_mid lattice")
            t |> success(frames >= 64l && frames % 64l == 0l, "ru_mid lattice: the encoder frames are a whole multiple of 64 ({frames})")
        }
    }
}

// A form's legs of the kq lane's Metal twin, on a quantized carrier's block (`k.carrier`, the lane named `k.fmt`): `kq` the
// CPU kq chain, `kq@dev` the Metal chain with a K-quant mul_mm stamp counted, `kq_z<block>@dev` its poison. The mixed
// carrier reads ru_short alone.
[cold_path, unused_argument(t, m, k), arch(at = "../ARCHITECTURE_GPU_TOWER_FC.md#tower-fc-kq")]   // off-Apple the gated body is empty
def ga_metal_kq_legs(t : T?; m : GigaamModel; var k : GaKept) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let f = gigaam_form_of(k.form)
        if (!f.dev) {
            k.strs["{k.carrier}/members"] = gigaam_kq_members(m)   // the CPU image names the members: a device image carries its device planes alone
        }
        with_dasllama_jobque_() {
            for (clip in GA_CLIPS) {
                continue if (k.carrier == "mix" && clip != "ru_short")
                if (!asr_clip_present("{clip}.wav")) {
                    t |> skip("{clip}.wav not present")
                    continue
                }
                var inscope samples <- load_audio_16k_mono(asr_clip("{clip}.wav"))
                let tag = "gigaam {clip} kq lane {kq_str(k, "members")}"
                with_gpu_knobs() {
                    if (f.zero_block >= 0l) {
                        set_metal_tower(true)
                        t |> success(m.kq_lane && m.dev_backend == TowerDev.metal, "{tag}: the poisoned carrier is a {TowerDev.metal}-flavored kq lane image")
                        var bad <- ga_leg(m, samples)
                        ga_keep(k, kq_key(k, clip, "kq_bad"), bad)
                    } elif (f.dev) {
                        set_metal_tower(true)
                        let q0 = metal_tower_stats()
                        let mel0 = metal_tower_ga_mels()
                        let qd0 = metal_tower_declines()?["quant_mode"] ?? 0l
                        let mm0 = kq_mm_count()
                        var dev <- ga_leg(m, samples)
                        let q1 = metal_tower_stats()
                        t |> equal(q1.encodes - q0.encodes, 1l, "{tag}: the device served the encode")
                        t |> equal(q1.blocks - q0.blocks, m.n_layer, "{tag}: every block ran on the driver")
                        t |> equal(metal_tower_ga_mels() - mel0, 1l, "{tag}: the log-mel ran on the device")
                        t |> equal((metal_tower_declines()?["quant_mode"] ?? 0l) - qd0, 0l, "{tag}: no quant_mode decline")
                        t |> success(kq_mm_count() > mm0, "{tag}: a K-quant mul_mm stamp served the K-quant seats (the census counter rose)")
                        ga_keep(k, kq_key(k, clip, "kq_dev"), dev)
                    } else {
                        t |> success(m.kq_lane, "{tag}: the carrier serves as its own planes")
                        t |> equal(gigaam_exec_fmt(m), k.fmt, "{tag}: the lane's name - its one K-quant member's disk type, or `kq` where the members mix")
                        set_metal_tower(false)
                        var cpu <- ga_leg(m, samples)
                        ga_keep(k, kq_key(k, clip, "kq_cpu"), cpu)
                    }
                }
            }
        }
    }
}

//! one clip's kq-lane verdict on a device chain named `home`, the exact chain `ex` the reference: the frame count and the
//! token ids the CPU kq chain's, the device rows within the three-way bar, the poisoned carrier's rows outside it
def private ga_kq_held(t : T?; tag, home : string; n_layer : int64; ex, cpu, dev, bad : GaLeg) {
    t |> equal(dev.n, cpu.n, "{tag}: the frame count")
    t |> equal(dev.ids, cpu.ids, "{tag}: the token ids are the CPU kq chain's")
    to_log(LOG_INFO, "test_audio: {tag} cpu: {cpu.text}\n")
    to_log(LOG_INFO, "test_audio: {tag} gpu: {dev.text}\n")
    var kq_ex = TowerParityStats()
    var dev_ex = TowerParityStats()
    if (tower_parity_stats(t, "{tag} cpu kq vs exact", ex.x, cpu.x, kq_ex) && tower_parity_stats(t, "{tag} {home} vs exact", ex.x, dev.x, dev_ex)) {
        let bar = double(TWIN_SLACK_VK) * kq_ex.rel_l2 + TWIN_REL_L2_FLOOR
        to_log(LOG_INFO, "test_audio: {tag}: rel_l2 from the exact chain - cpu kq {kq_ex.rel_l2}, {home} {dev_ex.rel_l2}, the bar {bar}\n")
        t |> success(dev_ex.rel_l2 <= bar, "{tag}: the device rows within {TWIN_SLACK_VK}x the CPU kq chain's rel_l2 from the exact chain ({dev_ex.rel_l2} vs {kq_ex.rel_l2})")
        ga_kq_poison_held(t, tag, home, n_layer, ex, bad, bar)
    }
}

//! the kq lane's poisoned control: the carrier minted `home`-flavored with its block GEMMs zeroed from the middle block on,
//! through the device chain, lands outside `bar` of the exact chain's rows
def private ga_kq_poison_held(t : T?; tag, home : string; n_layer : int64; ex, bad : GaLeg; bar : double) {
    var bad_ex = TowerParityStats()
    if (tower_parity_stats(t, "{tag} poisoned {home} vs exact", ex.x, bad.x, bad_ex)) {
        t |> success(is_finite(bad_ex.rel_l2) && bad_ex.rel_l2 > bar, "{tag}: the control - its GEMMs zeroed from block {n_layer / 2l} on, the device rows land outside the bar ({bad_ex.rel_l2} vs {bar})")
    }
}

// The kq lane on the Metal chain against the CPU kq chain, after a quantized carrier's block, the f16 carrier's exact
// chain the reference - "a kernel counter ... that no other plane of that file moves" (REVIEW_KQ_FORMATS.md).
[cold_path]
def ga_metal_kq_twin(t : T?; k : GaKept) {
    for (clip in GA_CLIPS) {
        continue if (k.carrier == "mix" && clip != "ru_short")
        if (!asr_clip_present("{clip}.wav")) {
            t |> skip("{clip}.wav not present")
            continue
        }
        var inscope keys <- ["{clip}/cpu", kq_key(k, clip, "kq_cpu"), kq_key(k, clip, "kq_dev"), kq_key(k, clip, "kq_bad")]
        continue if (!ga_all_kept(t, k, keys))
        ga_kq_held(t, "gigaam {clip} kq lane {kq_str(k, "members")}", "metal", k.n_layer, k.legs[ga_kept_at(k, keys[0])].leg,
            k.legs[ga_kept_at(k, keys[1])].leg, k.legs[ga_kept_at(k, keys[2])].leg, k.legs[ga_kept_at(k, keys[3])].leg)
    }
}

// ===== the discrete home =====

//! the discrete twin's rows of one clip named, from the tower at hand
def private ga_twin_named(var k : TwinRows; clip : string; n_layer : int64) {
    k.spec = TwinSpec(family = "gigaam", tag = "gigaam {clip}", n_layer = n_layer, metric = TwinMetric.rel_l2, seat_family = TowerFamily.gigaam)
}

//! one leg of `m` over `samples` with gigaam's tower knob `on`, the knobs put back
def private ga_knob_leg(m : GigaamModel; samples : array<float>; on : bool) : GaLeg {
    var r = GaLeg()
    with_gpu_knobs() {
        with_dasllama_jobque_() {
            set_serving_gpu_tower(on, TowerFamily.gigaam)
            delete r
            r <- ga_leg(m, samples)
        }
    }
    return <- r
}

//! the q8 lane's device legs of one clip, on the `q8@dev` form: the three-way's device rows, the device mel alone, the
//! transcript's leg with no k4 tile counted, and the storage-range refusal
[cold_path]
def private ga_discrete_q8_dev(t : T?; md : GigaamModel; var k : GaKept; samples : array<float>; clip : string; ci : int) {
    let tag = "gigaam {clip}"
    var std = GigaamState()
    twin_rows_leg(k.twins[ci], TwinLeg.dev, md, std) $(e; var s; var out) => ga_twin_encode(e, s, samples, out)
    delete std
    ga_discrete_mel_twin_cell(t, md, samples, clip)
    with_gpu_knobs() {
        with_dasllama_jobque_() {
            set_serving_gpu_tower(true, TowerFamily.gigaam)
            let k4_0 = kq_tile_count("k4")
            var dev <- ga_leg(md, samples)
            t |> equal(kq_tile_count("k4") - k4_0, 0l, "{tag}: the q8 lane dispatches no k4 tile (the kq lane counter's control)")
            ga_keep(k, "{clip}/q8_dev", dev)
            blocks_refused_leg(t, tag, TowerFamily.gigaam, 64l << 20l) $ {
                var inscope refused <- ga_leg(md, samples)
            }
        }
    }
}

//! the exact lane's device leg of one clip, on the `f16+served` form - the lane's flavor on this box: every stage on the
//! device with no `quant_mode` decline; off the f16 GEMM feed the lane loaded its CPU image and the chain declined it
[cold_path]
def private ga_discrete_exact_dev(t : T?; mxd : GigaamModel; var k : GaKept; samples : array<float>; clip : string) {
    let tag = "gigaam {clip} exact lane"
    k.strs["x_backend"] = "{mxd.dev_backend}"
    with_gpu_knobs() {
        with_dasllama_jobque_() {
            set_serving_gpu_tower(true, TowerFamily.gigaam)
            let s0 = gpu_tower_counters(TowerFamily.gigaam)
            let dq0 = gpu_tower_quant_declines(TowerFamily.gigaam)
            let dd0 = off_device_declines()
            var dev <- ga_leg(mxd, samples)
            let s1 = gpu_tower_counters(TowerFamily.gigaam)
            ga_keep(k, "{clip}/x_dev", dev)
            t |> equal(gpu_tower_quant_declines(TowerFamily.gigaam) - dq0, 0l, "{tag}: no quant_mode decline")
            if (!gpu_tower_front_route(TowerFamily.gigaam)) {
                t |> equal(s1.encodes - s0.encodes, 0l, "{tag}: off the f16 GEMM feed no encode on the device")
                t |> success(off_device_declines() > dd0, "{tag}: off the f16 GEMM feed the chain declined the lane")
                return
            }
            k.nums["{clip}/x_routed"] = 1l
            device_leg_served(t, tag, s0, s1, mxd.n_layer, -1l)
            t |> equal(s1.convs - s0.convs, 1l, "{tag}: the conv front ran on the device")
        }
    }
}

//! one clip's legs of the form `f` on gigaam's discrete home
[cold_path]
def private ga_discrete_clip_legs(t : T?; m : GigaamModel; var k : GaKept; f : TowerForm; samples : array<float>; clip : string; ci : int) {
    ga_twin_named(k.twins[ci], clip, m.n_layer)
    if (f.lane == "f16" && f.zero_block >= 0l) {
        return if ((k.nums?["{clip}/x_routed"] ?? 0l) == 0l)
        let tag = "gigaam {clip} exact lane"
        let p0 = gpu_tower_encodes(TowerFamily.gigaam)
        var bad <- ga_knob_leg(m, samples, true)
        t |> equal(gpu_tower_encodes(TowerFamily.gigaam) - p0, 1l, "{tag} poison: device-served (encodes delta)")
        ga_keep(k, "{clip}/x_bad", bad)
    } elif (f.lane == "f16" && f.edit == "served") {
        ga_discrete_exact_dev(t, m, k, samples, clip)
    } elif (f.lane == "f16") {
        t |> success(!m.q8 && m.gemm_f16, "gigaam {clip} exact lane: the block GEMMs serve off the halfword plane")
        var stx = GigaamState()
        twin_rows_leg(k.twins[ci], TwinLeg.exact, m, stx) $(e; var s; var out) => ga_twin_encode(e, s, samples, out)
        delete stx
        var ex <- ga_knob_leg(m, samples, false)
        ga_keep(k, "{clip}/cpu", ex)
    } elif (f.zero_block >= 0l) {
        var stp = GigaamState()
        twin_rows_poison_leg(t, k.twins[ci], m.n_layer / 2l, m, stp) $(e; var s; var out) => ga_twin_encode(e, s, samples, out)
        delete stp
    } elif (f.dev) {
        ga_discrete_q8_dev(t, m, k, samples, clip, ci)
    } else {
        var st = GigaamState()
        twin_rows_leg(k.twins[ci], TwinLeg.cpu, m, st) $(e; var s; var out) => ga_twin_encode(e, s, samples, out)
        delete st
        var cpu <- ga_knob_leg(m, samples, false)
        ga_keep(k, "{clip}/q8_cpu", cpu)
    }
}

// A form's legs of the discrete twin of the e2e transducer's f16 carrier, over the FLEURS ru clips: `f16` the CPU exact
// chain, `q8` the CPU q8 chain, `q8@dev` the home's q8 image (with the lane policy's cell), `q8_z<block>@dev` its poison,
// `f16+served` the exact lane's flavor on this box, `f16_z<block>@dev` the exact lane's poison.
[cold_path]
def ga_discrete_legs(t : T?; m : GigaamModel; var k : GaKept) {
    let f = gigaam_form_of(k.form)
    k.d_model = m.d_model
    k.n_layer = m.n_layer
    if (!f.dev && f.zero_block < 0l && empty(f.edit)) {
        k.nums[f.lane == "f16" ? "mx_q8" : "m_q8"] = m.q8 ? 1l : 0l
    } elif (f.lane == "q8" && f.zero_block < 0l) {
        k.strs["dev_backend"] = "{m.dev_backend}"
    }
    for (clip, ci in GA_CLIPS, count()) {
        if (!asr_clip_present("{clip}.wav")) {
            t |> skip("{clip}.wav not present")
            continue
        }
        var inscope samples <- load_audio_16k_mono(asr_clip("{clip}.wav"))
        ga_discrete_clip_legs(t, m, k, f, samples, clip, ci)
    }
    if (f.lane == "q8" && f.dev && f.zero_block < 0l) {
        ga_discrete_lane_policy_cell(t)
    }
}

//! GigaAM's exact lane on its discrete home's chain (the f16 GEMM over the halfwords) against the CPU exact chain, one
//! clip: the CPU's frames and ids, the rows nearer the exact chain than the CPU q8 chain's, the zeroed-blocks control past
//! that bar; off the f16 feed the frame count alone
def private ga_discrete_exact_held(t : T?; k : GaKept; clip : string) {
    let tag = "gigaam {clip} exact lane"
    return if (!ga_all_kept(t, k, ["{clip}/cpu", "{clip}/q8_cpu", "{clip}/x_dev"]))
    let ex = ga_kept_at(k, "{clip}/cpu")
    let q8 = ga_kept_at(k, "{clip}/q8_cpu")
    let dev = ga_kept_at(k, "{clip}/x_dev")
    let backend = k.strs?["x_backend"] ?? ""
    t |> equal(k.legs[dev].leg.n, k.legs[ex].leg.n, "{tag}: the frame count")
    return if ((k.nums?["{clip}/x_routed"] ?? 0l) == 0l)
    t |> equal(k.legs[dev].leg.ids, k.legs[ex].leg.ids, "{tag}: the token ids are the CPU exact chain's")
    to_log(LOG_INFO, "test_audio: {tag} cpu exact: {k.legs[ex].leg.text}\n")
    to_log(LOG_INFO, "test_audio: {tag} {backend}   : {k.legs[dev].leg.text}\n")
    var q8_ex = TowerParityStats()
    var dev_ex = TowerParityStats()
    if (tower_parity_stats(t, "{tag} cpu q8 vs exact", k.legs[ex].leg.x, k.legs[q8].leg.x, q8_ex) && tower_parity_stats(t, "{tag} device vs exact", k.legs[ex].leg.x, k.legs[dev].leg.x, dev_ex)) {
        let bar = q8_ex.rel_l2
        to_log(LOG_INFO, "test_audio: {tag}: rel_l2 from the exact chain - {backend} exact lane {dev_ex.rel_l2}, the bar (cpu q8) {bar}\n")
        t |> success(dev_ex.rel_l2 <= bar, "{tag}: the device rows sit nearer the exact chain than the CPU q8 chain's ({dev_ex.rel_l2} vs {bar})")
        return if (!ga_all_kept(t, k, ["{clip}/x_bad"]))
        var bad_ex = TowerParityStats()
        if (tower_parity_stats(t, "{tag} poisoned device vs exact", k.legs[ex].leg.x, k.legs[ga_kept_at(k, "{clip}/x_bad")].leg.x, bad_ex)) {
            t |> success(is_finite(bad_ex.rel_l2) && bad_ex.rel_l2 > bar, "{tag}: zeroed halfword blocks through the device chain EXCEED the bar ({bad_ex.rel_l2} vs {bar})")
        }
    }
}

// GigaAM v3 e2e-rnnt on the discrete home, after the f16 carrier's block, on what its forms kept: the q8 lane's three-way
// with its counters, transcripts and poison, and the exact lane off the halfwords against the CPU exact chain.
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_FC.md#vk-tower-fc-rotary")]
def test_gigaam_discrete_twin(t : T?; k : GaKept) {
    let home = serving_tower_dev(TowerFamily.gigaam)
    let backend = k.strs?["dev_backend"] ?? ""
    t |> success((k.nums?["mx_q8"] ?? 1l) == 0l && (k.nums?["m_q8"] ?? 0l) == 1l && backend == "{home}", "the exact and the q8 models minted, the q8 lane's {home} image beside")
    let n_layer = k.n_layer
    for (clip, ci in GA_CLIPS, count()) {
        if (!asr_clip_present("{clip}.wav")) {
            t |> skip("{clip}.wav not present")
            continue
        }
        let tag = "gigaam {clip}"
        twin_rows_held(t, k.twins[ci]) $(legs) {
            t |> equal(legs.n_cpu, legs.n_exact, "{tag}: the q8 chain's frame count")
            t |> equal(legs.n_dev, legs.n_exact, "{tag}: the device chain's frame count")
            t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "{tag}: the device served the encode")
            t |> equal(legs.dev.blocks - legs.pre.blocks, n_layer, "{tag}: every block ran on the driver")
            front_served(t, tag, legs.dev.convs - legs.pre.convs, 1l, legs.dev.declines - legs.cpu.declines)
            t |> equal(legs.dev.mels - legs.pre.mels, legs.dev.convs - legs.pre.convs, "{tag}: the log-mel ran on the device wherever the front did")
        }
        if (ga_all_kept(t, k, ["{clip}/q8_cpu", "{clip}/q8_dev"])) {
            let cpu = ga_kept_at(k, "{clip}/q8_cpu")
            let dev = ga_kept_at(k, "{clip}/q8_dev")
            to_log(LOG_INFO, "test_audio: gigaam {clip} {backend} vs cpu q8: rel_l2 {rel_l2(k.legs[dev].leg.x, k.legs[cpu].leg.x)}\n")
            to_log(LOG_INFO, "test_audio: gigaam {clip} cpu q8: {k.legs[cpu].leg.text}\n")
            to_log(LOG_INFO, "test_audio: gigaam {clip} {backend}: {k.legs[dev].leg.text}\n")
            t |> equal(k.legs[dev].leg.text, k.legs[cpu].leg.text, "{tag}: the device chain's transcript is the CPU q8 chain's")
        }
        ga_discrete_exact_held(t, k, clip)
    }
}

// A form's legs of the kq lane's discrete twin, on a quantized carrier's block (`k.carrier`, the lane named `k.fmt`): `kq`
// the CPU kq chain, `kq+served` the lane's flavor on this box with every K-quant member's tiles counted, `kq_z<block>@dev`
// its poison. The mixed carrier reads ru_short alone.
[cold_path, arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN_FC.md#vk-tower-fc-kq")]
def ga_discrete_kq_legs(t : T?; m : GigaamModel; var k : GaKept) {
    let f = gigaam_form_of(k.form)
    if (!f.dev && empty(f.edit)) {
        k.strs["{k.carrier}/members"] = gigaam_kq_members(m)   // the CPU image names the members: a device image carries its device planes alone
    }
    if (k.carrier == "mix" && !f.dev && f.zero_block < 0l && empty(f.edit)) {
        t |> equal(gigaam_kq_members(m), "k4+k6+q8", "the mixed carrier's members: Q4_K attention, Q6_K FF, Q8_0 conv pointwise")
    } elif (k.carrier == "k4" && !f.dev && f.zero_block < 0l && empty(f.edit)) {
        t |> success(m.kq_lane, "the Q4_K carrier takes the kq lane")
    }
    for (clip in GA_CLIPS) {
        continue if (k.carrier == "mix" && clip != "ru_short")
        if (!asr_clip_present("{clip}.wav")) {
            t |> skip("{clip}.wav not present")
            continue
        }
        var inscope samples <- load_audio_16k_mono(asr_clip("{clip}.wav"))
        let tag = "gigaam {clip} kq lane {kq_str(k, "members")}"
        if (f.zero_block >= 0l) {
            continue if ((k.nums?[kq_key(k, clip, "routed")] ?? 0l) == 0l)
            let home = serving_tower_dev(TowerFamily.gigaam)
            t |> success(m.kq_lane && m.dev_backend == home, "{tag}: the poisoned carrier is a {home}-flavored kq lane image")
            var bad <- ga_knob_leg(m, samples, true)
            ga_keep(k, kq_key(k, clip, "kq_bad"), bad)
        } elif (f.edit == "served") {
            ga_discrete_kq_dev(t, m, k, samples, clip, tag)
        } else {
            t |> success(m.kq_lane, "{tag}: the carrier serves as its own planes")
            t |> equal(gigaam_exec_fmt(m), k.fmt, "{tag}: the lane's name - its one K-quant member's disk type, or `kq` where the members mix")
            var cpu <- ga_knob_leg(m, samples, false)
            ga_keep(k, kq_key(k, clip, "kq_cpu"), cpu)
        }
    }
}

//! the kq lane's device leg of one clip, on the `kq+served` form: no `kq_lane` or `quant_mode` decline, every K-quant
//! member's tiles counted; off the f16 GEMM feed the chain declines `device`
[cold_path]
def private ga_discrete_kq_dev(t : T?; mkd : GigaamModel; var k : GaKept; samples : array<float>; clip, tag : string) {
    k.strs["{k.carrier}/backend"] = "{mkd.dev_backend}"
    with_gpu_knobs() {
        with_dasllama_jobque_() {
            let members <- kq_members_of(kq_str(k, "members"))
            set_serving_gpu_tower(true, TowerFamily.gigaam)
            let s0 = gpu_tower_counters(TowerFamily.gigaam)
            let dk0 = gpu_tower_declines_of("kq_lane", TowerFamily.gigaam)
            let dq0 = gpu_tower_quant_declines(TowerFamily.gigaam)
            let dd0 = off_device_declines()
            let tiles0 <- [for (n in members); kq_tile_count(n)]
            var dev <- ga_leg(mkd, samples)
            let s1 = gpu_tower_counters(TowerFamily.gigaam)
            ga_keep(k, kq_key(k, clip, "kq_dev"), dev)
            t |> equal(gpu_tower_declines_of("kq_lane", TowerFamily.gigaam) - dk0, 0l, "{tag}: no kq_lane decline")
            t |> equal(gpu_tower_quant_declines(TowerFamily.gigaam) - dq0, 0l, "{tag}: no quant_mode decline")
            if (!gpu_tower_front_route(TowerFamily.gigaam)) {
                t |> equal(s1.encodes - s0.encodes, 0l, "{tag}: off the f16 GEMM feed no encode on the device")
                t |> success(off_device_declines() > dd0, "{tag}: off the f16 GEMM feed the chain declined the lane")
                return
            }
            k.nums[kq_key(k, clip, "routed")] = 1l
            device_leg_served(t, tag, s0, s1, mkd.n_layer, -1l)
            t |> equal(s1.convs - s0.convs, 1l, "{tag}: the conv front ran on the device")
            for (n, c0 in members, tiles0) {
                t |> success(kq_tile_count(n) > c0, "{tag}: the {n} tiles served the {n} seats (the census counter rose)")
            }
        }
    }
}

// GigaAM's kq lane on its discrete home's chain against the CPU kq chain, after a quantized carrier's block, the f16
// carrier's exact chain the reference - "a kernel counter ... that no other plane of that file moves"
// (REVIEW_KQ_FORMATS.md); off the f16 GEMM feed the frame count alone.
[cold_path]
def ga_discrete_kq_twin(t : T?; k : GaKept) {
    let home = serving_tower_dev(TowerFamily.gigaam)
    for (clip in GA_CLIPS) {
        continue if (k.carrier == "mix" && clip != "ru_short")
        if (!asr_clip_present("{clip}.wav")) {
            t |> skip("{clip}.wav not present")
            continue
        }
        let tag = "gigaam {clip} kq lane {kq_str(k, "members")}"
        var inscope keys <- ["{clip}/cpu", kq_key(k, clip, "kq_cpu"), kq_key(k, clip, "kq_dev")]
        continue if (!ga_all_kept(t, k, keys))
        let ex = ga_kept_at(k, keys[0])
        let cpu = ga_kept_at(k, keys[1])
        let dev = ga_kept_at(k, keys[2])
        t |> equal(k.legs[dev].leg.n, k.legs[cpu].leg.n, "{tag}: the frame count")
        continue if ((k.nums?[kq_key(k, clip, "routed")] ?? 0l) == 0l)
        var inscope dev_words <- transcript_words(k.legs[dev].leg.text)
        var inscope cpu_words <- transcript_words(k.legs[cpu].leg.text)
        t |> success(sequence_equal(dev_words, cpu_words), "{tag}: the words are the CPU kq chain's, the casing and punctuation a near-tie token flips aside")
        to_log(LOG_INFO, "test_audio: {tag} cpu kq : {k.legs[cpu].leg.text}\n")
        to_log(LOG_INFO, "test_audio: {tag} {kq_str(k, "backend")} : {k.legs[dev].leg.text}\n")
        var direct = TowerParityStats()
        if (tower_parity_stats(t, "{tag} {home} vs cpu kq", k.legs[cpu].leg.x, k.legs[dev].leg.x, direct)) {
            t |> success(direct.rel_l2 <= GA_KQ_DEV_CPU_REL_L2, "{tag}: the device rows within {GA_KQ_DEV_CPU_REL_L2} rel_l2 of the CPU kq chain's ({direct.rel_l2})")
        }
        let bar = tower_twin_bar(t, tag, TowerFamily.gigaam, k.legs[ex].leg.x, k.legs[cpu].leg.x, k.legs[dev].leg.x, k.n_layer, double(TWIN_SLACK_VK), TWIN_REL_L2_FLOOR)
        if (bar > 0.0lf && ga_all_kept(t, k, [kq_key(k, clip, "kq_bad")])) {
            let bad = ga_kept_at(k, kq_key(k, clip, "kq_bad"))
            ga_kq_poison_held(t, tag, "{home}", k.n_layer, k.legs[ex].leg, k.legs[bad].leg, bar)
            var inscope bad_words <- transcript_words(k.legs[bad].leg.text)
            t |> success(!sequence_equal(bad_words, cpu_words), "{tag}: the control - the poisoned carrier's words are not the CPU kq chain's")
        }
    }
}

// ===== the CTC head's device seat =====

// The device legs of a CTC head twin over ru_short, on the head's device form: the encode on the device, the head off the
// encode's own rows, off the 1.05x clip's (the control) and then off the first clip's uploaded rows - each counted on the
// device; `discrete` adds the discrete seat's held-row handoffs. The encode's rows are kept for the CPU form's leg.
[cold_path]
def ga_ctc_dev_leg(t : T?; m : GigaamModel; var k : GaKept; who, tag : string; discrete : bool) {
    let wav = asr_clip("ru_short.wav")
    if (!stat(wav).is_valid) {
        t |> skip("ru_short.wav not present")
        return
    }
    var inscope samples <- load_audio_16k_mono(wav)
    var c = GaCtc(tag = tag, q8 = m.q8, n_classes = m.n_classes)
    if (discrete) {
        t |> success(m.ctc_f16 && m.dw_f16, "the head and the depthwise taps serve off the carrier's halfwords")
    }
    with_gpu_knobs() {
        with_dasllama_jobque_() {
            set_serving_gpu_tower(true, TowerFamily.gigaam)
            var st = GigaamState()
            c.n = gigaam_encode_span(samples, 0l, min(long_length(samples), GIGAAM_MAX_CLIP_S * AUDIO_SAMPLE_RATE), m, st)
            let held = st.enc_gen != 0ul
            if (discrete) {
                to_log(LOG_INFO, "{who}: {tag}: the encode {held ? "served on the device, its rows held" : "ran on the CPU"}\n")
            } else {
                t |> success(st.enc_gen != 0ul, "{tag}: the encode served on the device")
            }
            c.rows := st.x
            let s0 = gpu_ctc_stats()
            c.dev <- ctc_leg(m, st, c.n)
            let s1 = gpu_ctc_stats()
            t |> equal(c.dev.served, 1l, "{tag}: the head served on the device")
            if (discrete) {
                t |> equal(s1.handoffs - s0.handoffs, held ? 1l : 0l, "{tag}: the head read the encode's own device rows where the chain held them")
            }
            var inscope loud <- [for (v in samples); v * 1.05]
            var sl = GigaamState()
            let nl = gigaam_encode_span(loud, 0l, min(long_length(loud), GIGAAM_MAX_CLIP_S * AUDIO_SAMPLE_RATE), m, sl)
            c.moved <- ctc_leg(m, sl, nl)
            t |> equal(c.moved.served, 1l, "{tag}: the control's head served on the device")
            t |> success(sl.enc_gen != st.enc_gen, "{tag}: the control's encode holds the device rows now")
            delete sl
            let u0 = gpu_ctc_stats()
            c.up <- ctc_leg(m, st, c.n)
            t |> equal(c.up.served, 1l, "{tag}: off rows the device no longer holds the head serves off the uploaded rows")
            if (discrete) {
                t |> equal(gpu_ctc_stats().handoffs - u0.handoffs, 0l, "{tag}: no held rows to read")
            }
            delete st
        }
    }
    c.legs = 1
    k.ctc |> emplace(c)
}

// The CPU leg of every CTC head twin of the form's lane, on the CPU form: the CPU head over the device form's kept rows
// (an f16 head's over half-rounded rows) with the tower knob off, never counted on the device.
[cold_path]
def ga_ctc_cpu_leg(t : T?; mc : GigaamModel; var k : GaKept) {
    for (c in k.ctc) {
        continue if (c.q8 != mc.q8 || c.legs != 1)
        with_gpu_knobs() {
            with_dasllama_jobque_() {
                set_serving_gpu_tower(false, TowerFamily.gigaam)
                var sc = GigaamState()
                sc.x := c.rows
                c.cpu <- ctc_cpu_leg(mc, sc, c.n)
                delete sc
            }
        }
        t |> equal(c.cpu.served, 0l, "{c.tag}: the tower knob off - the head stays on the CPU")
        c.legs = 2
    }
}

// A CTC head on its device seat against the CPU head, after the carrier's block, the twins of the lane `k.form` names
// (`f16`, `q8`): each pick the CPU's but at a near-tie, -log sum exp within `CTC_PLOG_BAR`, the ids and transcript the
// CPU's, the 1.05x control outside the bar, and the same off the uploaded rows. `who` leads the log lines.
[cold_path]
def ga_ctc_held(t : T?; k : GaKept; who : string) {
    let bar = Bar(rel = CTC_PLOG_BAR, abs_floor = CTC_PLOG_BAR, poison_scaled = true)
    for (c in k.ctc) {
        continue if (c.q8 != (k.form == "q8"))
        let tag = c.tag
        t |> equal(c.legs, 2, "{tag}: the head's device legs and its CPU leg were kept")
        continue if (c.legs != 2)
        to_log(LOG_INFO, "{who}: {tag} cpu   : {c.cpu.text}\n")
        to_log(LOG_INFO, "{who}: {tag} device: {c.dev.text}\n")
        t |> equal(ctc_hard_flips(c.cpu, c.dev, c.n_classes), 0, "{tag}: the picks are the CPU's but at a near-tie frame")
        check(t, "{who}: {tag} the device head's per-frame log-sums", c.dev.plogs, c.cpu.plogs, bar, FLAT_SHAPE)
        t |> success(sequence_equal(c.dev.ids, c.cpu.ids), "{tag}: the token ids are the CPU head's")
        t |> equal(c.dev.text, c.cpu.text, "{tag}: the transcript")
        check_exceeds(t, "{who}: {tag} the control - the clip at 1.05x through the device chain", c.moved.plogs, c.cpu.plogs, bar)
        t |> equal(ctc_hard_flips(c.cpu, c.up, c.n_classes), 0, "{tag}: the uploaded rows' picks are the CPU head's but at a near-tie frame")
        check(t, "{who}: {tag} the head off the uploaded rows' log-sums", c.up.plogs, c.cpu.plogs, bar, FLAT_SHAPE)
        t |> equal(c.up.text, c.cpu.text, "{tag}: the uploaded rows' transcript")
    }
}

// GigaAM's CTC head on the family's discrete seat (`vulkan_gigaam_ctc`, `cuda_gigaam_ctc`) against the CPU head, after a
// CTC carrier's block, the lane `k.form` names.
[cold_path, arch(at = "../ARCHITECTURE_MEDIA_ASR.md#gigaam-planes")]
def test_gigaam_ctc_discrete_twin(t : T?; k : GaKept) {
    ga_ctc_held(t, k, "test_audio")
}
