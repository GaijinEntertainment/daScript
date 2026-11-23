// The audio FastConformer twins' shared pieces: a staging's input poison (a block's GEMM planes, a kq lane's seats), an
// encode's rows off a state, the Vulkan storage-range refusal leg, a q8-class lane's twin and the exact lane as the
// forms of a file keep them (`LaneTwinKept`, `fc_exact_lane_held`), the regrown-residency legs (`TwinRegrowKept`) and a
// family's twin seats as reasons - one text the audio test files and their fixture cell
// modules call with their own family's staging and encode.

options gen2
options _dasllama_internal = true

module _audio_twin shared public

require dastest/testing_boost public
require math   // min, max
require dasllama/dasllama_tower   // TOWER_SEAT_WORDS
require dasllama/dasllama_audio   // AUDIO_SAMPLE_RATE: the regrow legs' short clip
require dasllama/dasllama_math   // with_dasllama_jobque_
require daslib/jobque_boost
require dasllama/dasllama_canary   // CanaryLayerOffs: the FastConformer block's offsets record
require dasllama/dasllama_kqformat   // KqFmt: a kq seat's member
require _compares   // copy_span
require _model_tier   // asr_clip_present: the jfk twins' clip
require _tower_twin public   // TwinRows and its legs, ExactFlavorKept, the knobs
require ?das_metal dasllama/dasllama_metal_tower   // metal_tower_f16_encodes: the q8 lane's slab witness
require ?vulkan dasllama/dasllama_vulkan_tower   // vulkan_tower_stats: the range refusal's counters
require ?vulkan dasllama/dasllama_vulkan_common   // g_gpu: the storage range the refusal stands in; vk_kernel_coverage_of
require ?das_metal dasllama/dasllama_metal_common   // metal_kernel_coverage: the kq lane's mul_mm stamps
require ?cuda dasllama/dasllama_cuda_common   // cud_kernel_census_of: the kq lane's tile census on the CUDA home
require strings   // nolint:STYLE030,LINT019 - ends_with, inside the das_metal half

def zero_plane(var q : array<auto(PT)>; off, n : int64) {
    for (i in range64(n)) {
        q[off + i] = default<PT>
    }
}

//! one zeroed block moves a 32-block tower less than its own q8 chain's noise, so the poison zeroes an eighth of the blocks
def poison_blocks(n_layer : int64) : int64 => max(1l, n_layer / 8l)

//! the GEMM planes of one FastConformer block zeroed in its GEMM plane (the q8 quants, or the halfwords) - eleven, or ten where
//! the family has no pos projection (`pos_w` -1): the poison of the device twins' control
def poison_fc_block(var plane : array<auto(PT)>; lo : CanaryLayerOffs; d, ff : int64) {
    for (w in fixed_array(lo.q_w, lo.k_w, lo.v_w, lo.pos_w, lo.o_w, lo.pw2_w)) {
        if (w >= 0l) {
            zero_plane(plane, w, d * d)
        }
    }
    for (w in fixed_array(lo.ff1a_w, lo.ff1b_w, lo.ff2a_w, lo.ff2b_w)) {
        zero_plane(plane, w, d * ff)
    }
    zero_plane(plane, lo.pw1_w, d * 2l * d)
}

//! `zero_block` >= 0: the GEMM planes of `poison_blocks` blocks from that block on zeroed in `plane`, each block's offsets
//! `layer` maps onto canary's record
def poison_fc_blocks(var plane : array<auto(PT)>; n_layer, zero_block, d, ff : int64; layer : block<(b : int64) : CanaryLayerOffs>) {
    return if (zero_block < 0l)
    for (b in range64(zero_block, min(n_layer, zero_block + poison_blocks(n_layer)))) {
        poison_fc_block(plane, invoke(layer, b), d, ff)
    }
}

//! one encode of `run` over `st`, the encoder's `width`-wide rows of `st.x` it answered for into `out`; its frame count
def x_rows_encode(var st : auto(ST); width : int64; var out : array<float>; run : block<(var s : ST) : int64>) : int64 {
    let n = invoke(run, st)
    delete out
    out <- copy_span(st.x, 0l, n * width)
    return n
}

//! a Vulkan-flavored model's encode under a storage range stood in at `msr_at`: the front stands down and the CPU chain refuses (`#tower-dev-block`)
[unused_argument(t, tag, fam, msr_at, run)]   // without dasVulkan the gated body is empty
def blocks_refused_leg(t : T?; tag : string; fam : TowerFamily; msr_at : int64; run : block<() : void>) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return if (tower_family_cuda(fam))
        let msr = g_gpu.msr
        let s0 = vulkan_tower_stats()
        let k0 <- vulkan_tower_declines()
        g_gpu.msr = msr_at
        var refused = ""
        try {
            invoke(run)
        } recover {
            refused = "refused"
        }
        g_gpu.msr = msr
        let s1 = vulkan_tower_stats()
        let k1 <- vulkan_tower_declines()
        t |> equal(s1.convs - s0.convs, 0l, "{tag}, blocks past the range: no front dispatch counted")
        t |> equal(s1.mels - s0.mels, 0l, "{tag}, blocks past the range: no device mel counted")
        t |> equal(s1.encodes - s0.encodes, 0l, "{tag}, blocks past the range: no encode served")
        t |> equal((k1?["memory"] ?? 0l) - (k0?["memory"] ?? 0l), 0l, "{tag}, blocks past the range: the front's buffers fit (no memory decline)")
        t |> equal(refused, "refused", "{tag}, blocks past the range: the Vulkan-flavored model refuses the CPU chain")
    }
}

//! a held kq-lane staging's seats zeroed from seat `first` up to seat `past` (the seats behind it kept): the quant bytes of every
//! K-quant and Q8_0 seat between them, the planes' ends where `past` is the seat count
def poison_kq_seats(seats : array<int64>; first, past : int64; var kq : array<uint8>; var qblob : array<int8>) {
    let q8 = int64(int(KqFmt.q8))
    var kq_lo = long_length(kq)
    var kq_hi = long_length(kq)
    var q8_lo = long_length(qblob)
    var q8_hi = long_length(qblob)
    for (i in range64(first, long_length(seats) / TOWER_SEAT_WORDS)) {
        let at = seats[i * TOWER_SEAT_WORDS + 1l]
        if (seats[i * TOWER_SEAT_WORDS] == q8) {
            q8_lo = min(q8_lo, at)
            q8_hi = i >= past ? min(q8_hi, at) : q8_hi
        } else {
            kq_lo = min(kq_lo, at)
            kq_hi = i >= past ? min(kq_hi, at) : kq_hi
        }
    }
    for (i in range64(kq_lo, kq_hi)) {
        kq[i] = 0u8
    }
    for (i in range64(q8_lo, q8_hi)) {
        qblob[i] = int8(0)
    }
}

//! the device's dense K-quant mul_mm dispatches, every stamp and form: on the Q4_K carrier the kq lane's K-quant seats alone move them
def kq_mm_count() : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var n = 0l
        var inscope cov <- metal_kernel_coverage()
        for (k, v in keys(cov), values(cov)) {
            if (k |> ends_with("_metal_kq_mulmm_t") || k |> ends_with("_metal_kq_mulmm_db") || k |> ends_with("_metal_kq_mulmm_k6_t") || k |> ends_with("_metal_kq_mulmm_iq4xs_t")
                    || k |> ends_with("_metal_kq_mulmm") || k |> ends_with("_metal_kq_mulmm_sk")) {
                n += v
            }
        }
        return n
    } else {
        return 0l
    }
}

//! the device's k4 cm2 and KHR tile dispatches: the kq lane's K-quant seats run on one of them, by the device's mode and the column pick
//! the dispatches of the K-quant member `fmt`'s batch tiles (`kq_batch_<fmt>_*`) the census counted
[unused_argument(fmt, fam)]
def kq_tile_count(fmt : string; fam : TowerFamily = TowerFamily.gigaam) : int64 {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (tower_family_cuda(fam)) {
            return (max(cud_kernel_census_of("kq_batch_{fmt}_cm2l_cls_cuda"), 0l) + max(cud_kernel_census_of("kq_batch_{fmt}_cm2m_cls_cuda"), 0l)
                + max(cud_kernel_census_of("kq_batch_{fmt}_cm2s_cls_cuda"), 0l))
        }
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return (vk_kernel_coverage_of("kq_batch_{fmt}_cm2l_cls_spv") + vk_kernel_coverage_of("kq_batch_{fmt}_cm2m_cls_spv")
            + vk_kernel_coverage_of("kq_batch_{fmt}_cm2s_cls_spv") + vk_kernel_coverage_of("kq_batch_{fmt}_khr_cls_spv"))
    } else {
        return 0l
    }
}

//! the Metal tower's encodes off the halfword plane since the process started; 0 off a das_metal build
def private metal_f16_encodes() : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_tower_f16_encodes()
    } else {
        return 0l
    }
}

//! A q8-class lane's twin (the q8 lane, a quantized carrier's kq lane) as the forms of its file keep it: the rows three
//! ways (`TwinRows`) and, across the device leg, the driver's `quant_mode` declines and its encodes off the halfword plane.
struct LaneTwinKept {
    rows : TwinRows = TwinRows()
    quant_declines : int64
    f16_encodes : int64
}

//! the lane twin `k` named: every assert's tag, the tower's depth, the family whose home serves the device leg
def lane_twin_named(var k : LaneTwinKept; tag : string; fam : TowerFamily; n_layer : int64) {
    k.rows.spec = TwinSpec(family = "{fam}", tag = tag, n_layer = n_layer, metric = TwinMetric.rel_l2, seat_family = fam)
}

//! the leg `leg` of the lane twin `k` on the form's tower `tw` (`twin_rows_leg`), the device leg's `quant_mode` declines
//! and halfword encodes kept
def lane_twin_leg(var k : LaneTwinKept; leg : TwinLeg; tw : auto(TW); var st : auto(ST);
                  encode : block<(e : TW; var s : ST; var out : array<float>) : int64>) {
    let fam = k.rows.spec.seat_family
    let dq0 = gpu_tower_quant_declines(fam)
    let f0 = metal_f16_encodes()
    twin_rows_leg(k.rows, leg, tw, st, encode)
    if (leg == TwinLeg.dev) {
        k.quant_declines = gpu_tower_quant_declines(fam) - dq0
        k.f16_encodes = metal_f16_encodes() - f0
    }
}

//! the leg of a lane twin's form ladder a form ran
enum LaneFormLeg {
    exact_poison
    exact_dev
    exact
    poison
    dev
    cpu
}

//! the lane flags of a lane twin's three clean forms - the exact form's, the CPU q8 form's, the device form's - and the
//! device image's flavor, each as its leg kept it
struct LaneFlags {
    x_q8 : bool
    c_q8 : bool
    d_q8 : bool
    d_backend : TowerDev
}

//! The leg the form `f` of a lane twin's file runs on its tower `tw`, each on the form's one home: the zeroed and the plain
//! `f32@dev` forms the exact lane's poison and device legs, the exact form the reference chain (and, with `exact_cpu`, the
//! exact lane's CPU leg), the zeroed form the input poison, the `@dev` form the device chain, any other the CPU chain; the
//! clean forms' lane flags kept in `flags`. Answers the leg it ran, for the family's own extras.
def lane_exact_leg(t : T?; var twin : LaneTwinKept; var exact : ExactFlavorKept; var flags : LaneFlags; f : TowerForm; exact_cpu : bool;
                   tw : auto(TW); var st : auto(ST); encode : block<(e : TW; var s : ST; var out : array<float>) : int64>) : LaneFormLeg {
    if (f.lane == "f32" && f.zero_block >= 0l) {
        exact_flavor_poison_leg(t, exact, tw, st, encode)
        return LaneFormLeg.exact_poison
    } elif (f.lane == "f32" && f.dev) {
        exact_flavor_dev_leg(exact, tw, st, encode)
        return LaneFormLeg.exact_dev
    } elif (f.lane == "f32") {
        flags.x_q8 = tw.q8
        lane_twin_leg(twin, TwinLeg.exact, tw, st, encode)
        if (exact_cpu) {
            exact_flavor_cpu_leg(exact, tw, st, encode)
        }
        return LaneFormLeg.exact
    } elif (f.zero_block >= 0l) {
        twin_rows_poison_leg(t, twin.rows, f.zero_block, tw, st, encode)
        return LaneFormLeg.poison
    } elif (f.dev) {
        flags.d_q8 = tw.q8
        flags.d_backend = tw.dev_backend
        lane_twin_leg(twin, TwinLeg.dev, tw, st, encode)
        return LaneFormLeg.dev
    }
    flags.c_q8 = tw.q8
    lane_twin_leg(twin, TwinLeg.cpu, tw, st, encode)
    return LaneFormLeg.cpu
}

//! A lane's twin on the Metal home held, after its file's block: every stage of every encode on the device, the device
//! chain within the three-way bar and the poison past it (`twin_rows_held`), no `quant_mode` decline and no encode off
//! the halfword plane across the device leg.
def metal_q8_twin_held(t : T?; k : LaneTwinKept) {
    let tag = k.rows.spec.tag
    let n_layer = k.rows.spec.n_layer
    twin_rows_held(t, k.rows) $(legs) {
        t |> equal(legs.n_cpu, legs.n_exact, "{tag}: the q8 chain's frame count")
        t |> equal(legs.n_dev, legs.n_exact, "{tag}: the device chain's frame count")
        let n_enc = legs.dev.encodes - legs.pre.encodes
        t |> success(n_enc >= 1l, "{tag}: the device served the encodes ({n_enc})")
        t |> equal(legs.dev.blocks - legs.pre.blocks, n_enc * n_layer, "{tag}: every block of every encode ran on the driver")
        front_served(t, tag, legs.dev.convs - legs.pre.convs, n_enc, legs.dev.declines - legs.cpu.declines)
    }
    t |> equal(k.quant_declines, 0l, "{tag}: the q8 lane declined nothing (`quant_mode`)")
    t |> equal(k.f16_encodes, 0l, "{tag}: the q8 lane read the q8 slab, not the halfword plane")
}

//! a FastConformer's kept exact lane held (`exact_flavor_held`), the front on both device legs
def fc_exact_lane_held(t : T?; k : ExactFlavorKept) {
    front_served(t, "{k.lane.tag} exact lane", exact_flavor_held(t, k), 2l, 0l)
}

//! A FastConformer lane twin on the Metal home held, after its file's block: the exact and the q8 `what` minted with the
//! q8 lane's Metal image beside, the q8 lane three ways with its poison (`metal_q8_twin_held`), then the exact lane.
def fc_metal_twin_held(t : T?; what : string; flags : LaneFlags; twin : LaneTwinKept; exact : ExactFlavorKept) {
    t |> success(!flags.x_q8 && flags.c_q8 && flags.d_q8 && flags.d_backend == TowerDev.metal, "the exact and the q8 {what} minted, the q8 lane's Metal image beside")
    metal_q8_twin_held(t, twin)
    fc_exact_lane_held(t, exact)
}

//! a FastConformer's device leg held inside `twin_rows_held` on one clip: both chains' frame counts, one device encode,
//! every block on the driver, the front served (`front_served`); `tag` names the twin
def fc_discrete_counters(t : T?; tag : string; n_layer : int64; legs : TwinLegs) {
    t |> equal(legs.n_cpu, legs.n_exact, "{tag}: the q8 chain's frame count")
    t |> equal(legs.n_dev, legs.n_exact, "{tag}: the device chain's frame count")
    t |> equal(legs.dev.encodes - legs.pre.encodes, 1l, "{tag}: the device served the encode")
    t |> equal(legs.dev.blocks - legs.pre.blocks, n_layer, "{tag}: every block ran on the driver")
    front_served(t, tag, legs.dev.convs - legs.pre.convs, 1l, legs.dev.declines - legs.cpu.declines)
}

//! what the box lacks for a form an audio family's discrete twin reads, "" where family `fam`'s twin seat stands
//! (`discrete_audio_twin_seat_why`)
[unused_argument(fam)]
def audio_discrete_form_why(fam : TowerFamily) : string {
    static_if (typeinfo builtin_module_exists(vulkan) || typeinfo builtin_module_exists(cuda)) {
        return discrete_audio_twin_seat_why(fam)
    } else {
        return "no discrete GPU home in this build (dasVulkan, dasCUDA)"
    }
}

//! what the box lacks for a form an audio family's discrete twin on jfk reads, "" where it runs: family `fam`'s twin seat
//! (`audio_discrete_form_why`), the JIT, and jfk.wav
def jfk_twin_why(fam : TowerFamily) : string {
    let seat = audio_discrete_form_why(fam)
    return seat if (!empty(seat))
    return "interpreted (dasLLAMA model tests are JIT-only)" if (!jit_enabled())
    return asr_clip_present("jfk.wav") ? "" : "jfk.wav not present"
}

//! the first 3 s of a clip: an encode short enough to size the residency's scratch under the full clip's
def clip_head(samples : array<float>) : array<float> {
    let n = min(long_length(samples), 3l * AUDIO_SAMPLE_RATE)
    return <- [for (i in range64(n)); samples[i]]
}

//! A twin's regrown-residency legs on its family's discrete home as two forms keep them: the device form's rows of the
//! clip on a scratch regrown past a short encode's and on a residency rebuilt after a shutdown, with the device encodes
//! those three took; the poisoned device form's rows of the clip and the encodes it took.
struct TwinRegrowKept {
    again : array<float>
    fresh : array<float>
    poisoned : array<float>
    n_enc : int64
    poison_encodes : int64
    legs : int
}

//! the regrow legs of the device form's tower `enc` on family `fam`'s home: a short encode, the clip, the clip again after
//! the home's shutdown - the two clips' rows and the device encodes kept
def twin_regrow_dev_leg(var k : TwinRegrowKept; fam : TowerFamily; enc : auto(TW); var st : auto(ST); samples : array<float>;
                        encode : block<(e : TW; var s : ST; clip : array<float>; var out : array<float>) : int64>) {
    var inscope head <- clip_head(samples)
    var inscope dev_head : array<float>
    with_gpu_knobs() {
        set_serving_gpu_tower(true, fam)
        let e0 = gpu_tower_encodes(fam)
        with_dasllama_jobque_() {
            invoke(encode, enc, st, head, dev_head)
            invoke(encode, enc, st, samples, k.again)
            gpu_tower_shutdown(fam)
            invoke(encode, enc, st, samples, k.fresh)
        }
        k.n_enc = gpu_tower_encodes(fam) - e0
    }
    k.legs++
}

//! the regrow legs' input poison, on the poisoned device form's tower `encp`: the clip's rows and the device encodes kept
def twin_regrow_poison_leg(var k : TwinRegrowKept; fam : TowerFamily; encp : auto(TW); var stp : auto(ST); samples : array<float>;
                           encode : block<(e : TW; var s : ST; clip : array<float>; var out : array<float>) : int64>) {
    with_gpu_knobs() {
        set_serving_gpu_tower(true, fam)
        let p0 = gpu_tower_encodes(fam)
        with_dasllama_jobque_() {
            invoke(encode, encp, stp, samples, k.poisoned)
        }
        k.poison_encodes = gpu_tower_encodes(fam) - p0
    }
    k.legs++
}

//! The regrow legs held at the twin's bar `bar` from its exact chain's rows `reference`, after the file's block (a bar of 0
//! - its three-way could not run - holds nothing): the short, the regrown and the fresh encodes ran on the device
//! (`exactly_three`: one dispatch an encode), both clips' rows within the bar, the poison device-served and past it.
def twin_regrow_held(t : T?; tag : string; fam : TowerFamily; exactly_three : bool; k : TwinRegrowKept; reference : array<float>; bar : double) {
    return if (bar <= 0.0lf)
    t |> equal(k.legs, 2, "{tag}: the regrow legs kept the device form's rows and the poisoned form's")
    return if (k.legs != 2)
    if (exactly_three) {
        t |> equal(k.n_enc, 3l, "{tag}: the short, the regrown and the fresh encodes ran on the device")
    } else {
        t |> success(k.n_enc >= 3l, "{tag}: the short, the regrown and the fresh encodes ran on the device")
    }
    tower_twin_held(t, "{tag} regrown scratch", fam, reference, k.again, bar)
    tower_twin_held(t, "{tag} after shutdown", fam, reference, k.fresh, bar)
    t |> success(k.poison_encodes > 0l, "{tag} poison: device-served (encodes delta)")
    tower_twin_poisoned(t, tag, fam, reference, k.poisoned, bar)
}
