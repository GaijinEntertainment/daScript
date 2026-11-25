options gen2
options _dasllama_internal = true

// The decode attention's kernel-cell harness over every K/V mirror codec - f32, f16, q8_0 and tq4: one `CodecAttn` row names
// an arm's shape and the paths it walks (the slab its `kv_mul` picks, the key split, the rows a dispatch, the folded requant,
// the gate, the window, the softcap, the sink), `codec_attn_arm` dispatches it as the driver does against the CPU attention
// over the rows the CPU cache's own codec reads back. test_vulkan_kv_codec_kernels runs the block codecs' rows,
// test_vulkan_kernels the scalar mirrors'. Requires the dasVulkan module whole: its users require it behind `?vulkan`.

module _vk_codec_attn shared public

require dastest/testing_boost public
require vulkan
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_classes
require dasllama/dasllama_common   // kv_store_row / kv_load_row and the tq4 batch rotations: the CPU cache's own codec
require dasllama/dasllama_kv_dtype
require _vkd_oracles
require _kv_codec_oracles public
require _compares
require _attn_oracle
require math

let private I8_SENTINEL_BITS = 0x80808080u   //! a Q8 quant plane's fill: -128 in every byte, a value no requant writes

let private QA_BASE_BLOCKS = 3      //! the layer's base in the mirror: odd, so a block codec's blocks start two bytes off a word
let private QA_REGION_BLOCKS = 4    //! row 0's region past the layer's base; row r's region starts r x `keys` rows of blocks further
let private QA_KEYS = 300           //! past the 256-key chunk, so the running max rescales
let private QA_BAR = Bar(rel = 1e-4)   //! the device reads 1e-7 off the oracle; a block codec's rows before their quantization read 1e-3 off it

//! the requant a decode-attention arm folds into its store: `DaAttnArgs.rqk`'s three values
enum Fold {
    none
    q8_0
    q8_k
}

//! the rows as codec `kv` stores them: tq4's rotated first
def private stored_basis(kv : KVDtype; rows_f : array<float>; n, kvd, hs : int; signs : array<float>) : array<float> {
    if (kv == KVDtype.tq4) return <- tq4_rotated(rows_f, n, kvd, hs, signs)
    return <- clone_to_move(rows_f)
}

//! a mirror offset of `blocks` 32-element blocks in codec `kv`'s units: blocks under a block codec, elements under a scalar one
def private qa_units(kv : KVDtype; blocks : int) : int => int(kv_row_units(kv, int64(blocks * 32)))

//! the mirror of codec `kv` from rows in the model's basis
def private codec_mirror(kv : KVDtype; rows_f : array<float>; n, kvd, hs, base_units : int; signs : array<float>) : tuple<bytes : array<uint8>; deq : array<float>> {
    var stored <- stored_basis(kv, rows_f, n, kvd, hs, signs)
    var mirror <- codec_mirror_of_stored(kv, stored, n, kvd, base_units)
    delete stored
    return <- mirror
}

//! one decode-attention arm's shape: the codec's stamp for the slab `kv_mul` picks, or the sink stamp; `g4_twin` also runs the
//! four-head stamp on a two-head slab's arm and holds it to the slab bit for bit, `resubmit` dispatches the arm a second time
//! over the counters the first left (the arrival counters the pass resets must have rearmed)
struct CodecAttn {
    kv : KVDtype
    hs : int
    nheads : int
    kv_mul : int
    nsplit : int = 1
    nrows : int = 1
    keys : int = QA_KEYS
    fold : Fold
    gated : bool
    window : int
    cap : float
    sink : bool
    g4_twin : bool
    resubmit : bool
    fixcnt : bool   //! a tile arm: every row attends the arm's `keys` under the kernel's fixed count, the token block unread
    ring : int      //! a sliding layer's ring rows: the device mirror holds each region's last `ring` positions at their `kv_ring_row` slots, zero rows past them
}

//! the arm's inputs and what the CPU attention reads of them
struct private CodecAttnFixture {
    q_model : array<float>     //! the queries in the model's basis, [rows x heads x hs]
    q : array<float>           //! ... as the device attends them: rotated under tq4
    gate : array<float>        //! a gated arm's gate halves, [rows x heads x hs]
    q_plane : array<float>     //! the device's q plane: `q`, or [q | gate] a head under the gate
    k_model : array<float>     //! the K rows before the codec stores them, row r's `keys` rows in region r
    v_model : array<float>
    k : tuple<bytes : array<uint8>; deq : array<float>>
    v : tuple<bytes : array<uint8>; deq : array<float>>
    kring : array<uint8>       //! a ring arm's device mirrors: each region's last `ring` positions at their slots, the region's rows past the ring zero
    vring : array<uint8>
    sinks : array<float>
    signs : array<float>
}

def private codec_attn_fixture(a : CodecAttn) : CodecAttnFixture {
    let kvd = a.nheads / a.kv_mul * a.hs
    let qdim = a.nheads * a.hs
    var f = CodecAttnFixture(signs <- tq4_sign_row(),
        q_model <- row_fill(a.nrows * qdim, 41, 3, 0.05, 1.8, 73), gate <- row_fill(a.nrows * qdim, 29, 11, 0.07, 2.1, 61),
        k_model <- row_fill(a.nrows * a.keys * kvd, 17, 5, 0.03, 1.4), v_model <- row_fill(a.nrows * a.keys * kvd, 53, 7, 0.04, 1.7, 89))
    for (i in range(a.nrows * qdim)) {   //! an irregular factor an element: the rotated query of a regular ramp holds elements at exactly half its
        f.q_model[i] *= 1.0 + 0.011 * float((i * 7919) % 97) / 97.0   //! block's max, where the query's q8_0 quant is a coin flip between two f32 orders
    }
    for (r in range(1, a.nrows)) {   // each row its own content: a crossed region cannot pass
        let rs = 1.0 + 0.5 * float(r)
        for (i in range(qdim)) {
            f.q_model[r * qdim + i] *= rs
        }
        for (i in range(a.keys * kvd)) {
            f.k_model[r * a.keys * kvd + i] *= rs
            f.v_model[r * a.keys * kvd + i] *= rs
        }
    }
    f.q <- stored_basis(a.kv, f.q_model, a.nrows, qdim, a.hs, f.signs)
    if (a.gated) {
        f.q_plane <- q_gate_plane(f.q, f.gate, a.hs)
    } else {
        f.q_plane := f.q
    }
    let base = qa_units(a.kv, QA_BASE_BLOCKS + QA_REGION_BLOCKS)
    f.k <- codec_mirror(a.kv, f.k_model, a.nrows * a.keys, kvd, a.hs, base, f.signs)
    f.v <- codec_mirror(a.kv, f.v_model, a.nrows * a.keys, kvd, a.hs, base, f.signs)
    if (a.ring > 0) {
        var kr <- codec_mirror(a.kv, ring_laid_rows(f.k_model, a.nrows, a.keys, a.keys, kvd, a.ring, 0, 0.0), a.nrows * a.keys, kvd, a.hs, base, f.signs)
        var vr <- codec_mirror(a.kv, ring_laid_rows(f.v_model, a.nrows, a.keys, a.keys, kvd, a.ring, 0, 0.0), a.nrows * a.keys, kvd, a.hs, base, f.signs)
        f.kring <- kr.bytes
        f.vring <- vr.bytes
        delete kr
        delete vr
    }
    f.sinks <- sink_logits(f.q, f.k.deq, a.nheads, a.hs, a.kv_mul, a.keys, kvd, 1.0 / sqrt(float(a.hs)))
    return <- f
}

//! The CPU attention's rows over the fixture, [rows x heads x hs], through the arm's window, softcap, sink and gate: the
//! queries `q` against the K and V rows `k` and `v`. Each flag off gives the control that drops its mechanism.
def private codec_attn_oracle(a : CodecAttn; f : CodecAttnFixture; q, k, v : array<float>; window, softcap, sink, gate : bool = true) : array<float> {
    let kvd = a.nheads / a.kv_mul * a.hs
    let qdim = a.nheads * a.hs
    let scale = 1.0 / sqrt(float(a.hs))
    let kstart = window && a.window != 0 && a.keys > a.window ? a.keys - a.window : 0
    var out : array<float>
    out |> resize(a.nrows * qdim)
    for (r in range(a.nrows)) {
        for (hh in range(a.nheads)) {
            attn_row_oracle(q, r * qdim + hh * a.hs, k, r * a.keys * kvd, v, r * a.keys * kvd, kvd, hh, a.hs, a.kv_mul, a.keys, scale,
                out, r * qdim + hh * a.hs, kstart, softcap ? a.cap : 0.0, sink && a.sink, f.sinks[hh])
        }
    }
    if (gate && a.gated) {
        gate_rows(out, f.gate)
    }
    return <- out
}

//! the row's requant a store fold wrote (`quants`, `scales`) against the oracle row quantized the same way: blocks whose
//! scale is past a hair of the oracle's, and quants past one step of it
def private requant_misses(quants : array<int8>; scales, row : array<float>; blk : int) : tuple<scales : int; quants : int> {
    var bad_s = 0
    var bad_q = 0
    for (b in range(length(row) / blk)) {
        var m = 0.0
        for (i in range(blk)) {
            m = max(m, abs(row[b * blk + i]))
        }
        let d = m / 127.0
        bad_s += abs(scales[b] - d) <= 1.0e-6 + 1.0e-3 * d ? 0 : 1
        let id = d != 0.0 ? 1.0 / d : 0.0
        for (i in range(blk)) {
            bad_q += abs(int(quants[b * blk + i]) - int(round(row[b * blk + i] * id))) > 1 ? 1 : 0
        }
    }
    return (scales = bad_s, quants = bad_q)
}

//! a fold's `quants` and `scales` against the oracle row `want` quantized the same way, and against the control's row - or,
//! with no codec control, `want` scaled by 1.5 - which must miss
def private fold_checks(t : T?; tag : string; quants : array<int8>; scales, want, control : array<float>; blk : int) {
    let misses = requant_misses(quants, scales, want, blk)
    t |> success(misses.scales == 0 && misses.quants == 0, "{tag}: the fold's scales and quants are the oracle row's quantized ({misses.scales} scales, {misses.quants} quants off)")
    var ctrl_row <- empty(control) ? scaled_copy(want, 1.5) : clone_to_move(control)
    let ctrl = requant_misses(quants, scales, ctrl_row, blk)
    t |> success(ctrl.scales + ctrl.quants > 0, "{tag} control: the control's row quantized misses the fold's ({ctrl.scales} scales, {ctrl.quants} quants off)")
    delete ctrl_row
}

def private codec_attn_tag(a : CodecAttn; slab : DaSlab) : string {
    var tag = "{da_attn_stamp_name(a.kv, slab)} hs {a.hs}, {a.nheads} heads over {a.nheads / a.kv_mul}, {a.keys} keys in {a.nsplit} piece(s), {a.nrows} row(s)"
    if (a.fold != Fold.none) {
        tag += a.fold == Fold.q8_k ? ", the Q8_K fold" : ", the Q8_0 fold"
    }
    if (a.gated) {
        tag += ", gated"
    }
    if (a.window != 0) {
        tag += ", a {a.window}-key window"
    }
    if (a.cap != 0.0) {
        tag += ", softcap {a.cap}"
    }
    if (a.ring != 0) {
        tag += ", a {a.ring}-row ring"
    }
    return tag
}

//! The rows that tell the arm's compare from its codec's: under q8_0 the attention over the rows before their quantization,
//! under tq4 the query before its rotation; none under a scalar mirror - f32 stores the rows as they are, and the fixture's
//! rounding to half moves the attention inside the bar
def private codec_control(a : CodecAttn; f : CodecAttnFixture) : array<float> {
    if (a.kv == KVDtype.tq4) return <- codec_attn_oracle(a, f, f.q_model, f.k.deq, f.v.deq)
    if (a.kv == KVDtype.q8_0) return <- codec_attn_oracle(a, f, f.q, f.k_model, f.v_model)
    return <- array<float>()
}

//! the query the oracle scores: the block-codec stamps read its q8_0 blocks, as the CPU hot path does; the scalar mirrors read it f32
def private oracle_query(a : CodecAttn; f : CodecAttnFixture) : array<float> {
    if (a.kv == KVDtype.q8_0 || a.kv == KVDtype.tq4) {
        return <- q8_query_roundtrip(f.q)
    }
    return <- clone_to_move(f.q)
}

//! the four-head stamp's rows against the two-head slab's over the same rows (its dead lanes add zero): bit for bit, or at
//! the approx bar with its control on a device whose two stamps part by an ulp (`vk_rounds_alike`)
def private g4_twin_check(t : T?; tag : string; g4, got : array<float>; qdim, hs : int) {
    if (vk_rounds_alike()) {
        check_exact(t, "{tag}: the four-head stamp over the same rows is the two-head slab bit for bit (its dead lanes add zero)", g4, got)
    } else {
        check_rows(t, "{tag}: the four-head stamp over the same rows is the two-head slab at the approx bar (its dead lanes add zero)", g4, got, BAR_APPROX, qdim, hs)
    }
}

//! the controls: the codec's own (`codec_control`), each mechanism dropped, and under a fold the control's row quantized
def codec_attn_arm(t : T?; a : CodecAttn) {
    let kvd = a.nheads / a.kv_mul * a.hs
    let qdim = a.nheads * a.hs
    let qstride = a.gated ? 2 * a.hs : a.hs
    let slab = da_slab_of(a.sink, int64(a.kv_mul))
    let tag = codec_attn_tag(a, slab)
    let blk = a.fold == Fold.q8_k ? 256 : 32
    let rqk = a.fold == Fold.none ? DA_RQ_NONE : (a.fold == Fold.q8_k ? DA_RQ_Q8K : DA_RQ_Q8)
    var f <- codec_attn_fixture(a)
    var toks <- [for (r in range(a.nrows)); TokMeta(cnt = uint(a.keys), mirbase = uint(qa_units(a.kv, QA_REGION_BLOCKS) + r * a.keys * int(kv_row_units(a.kv, int64(kvd)))))]
    let rowwgs = da_attn_row_wgs(int64(a.nheads), int64(a.kv_mul), int64(a.nsplit))
    let rowgroups = int(da_attn_row_wgs(int64(a.nheads), int64(a.kv_mul), 1l))
    let cnt_off = a.nrows * a.nheads * a.nsplit * (a.hs + 2)
    var want <- codec_attn_oracle(a, f, oracle_query(a, f), f.k.deq, f.v.deq)
    var control <- codec_control(a, f)
    with_cell_planes() $(var cp : CellPlanes) {
        let qp = cp |> in_(f.q_plane)
        let kp = cp |> in_plain(a.ring > 0 ? f.kring : f.k.bytes)
        let vp = cp |> in_plain(a.ring > 0 ? f.vring : f.v.bytes)
        let yp = cp |> out_(a.nrows * qdim)
        let tp = cp |> in_plain(toks)
        let pp = cp |> out_words(int64(cnt_off + a.nrows * rowgroups) * 4l, NAN_SENTINEL_BITS)
        upload_zeros(cp.dev[pp], int64(cnt_off) * 4l, int64(a.nrows * rowgroups) * 4l)   // the arrival counters start at rest
        let aqp = cp |> out_words(int64((a.nrows * qdim + 3) / 4 * 4), I8_SENTINEL_BITS)
        let asp = cp |> out_(a.nrows * qdim / blk)
        let sp = cp |> in_(f.sinks)
        let planes = fixed_array(qp, kp, vp, yp, tp, pp, aqp, asp, sp)
        var s = da_attn_stamp_set(a.kv, slab, gather_planes(cp.dev, planes), gather_planes(cp.bytes, planes), gather_planes(cp.hazard_bit, planes))
        let base = uint(qa_units(a.kv, QA_BASE_BLOCKS))
        var pa = DaAttnArgs(kvd = uint(kvd), hs = uint(a.hs), kv_mul = uint(a.kv_mul), kbase = base, vbase = base,
            scale = 1.0 / sqrt(float(a.hs)), qstride = uint(qstride), gated = a.gated ? 1u : 0u, window = uint(a.window), cap = a.cap,
            nsplit = uint(a.nsplit), rqk = rqk, sinkoff = 0u, rowwg = uint(rowwgs), qrow = uint(a.nheads * qstride), cntoff = uint(cnt_off),
            ring = uint(a.ring))
        cp |> cell_record() $(rw : VkCommandBuffer; var h : VkHaz) {
            da_attn_stamp_enc(a.kv, slab, rw, h, s, pa, rowwgs * int64(a.nrows))
        }
        var got <- cp |> plane_run(yp)
        check_rows(t, "{tag} vs the CPU attention over the rows the codec reads back", got, want, QA_BAR, qdim, a.hs)
        if (!empty(control)) {
            t |> success(check_count(got, control, QA_BAR) > 0, "{tag} control: {a.kv == KVDtype.tq4 ? "the query before its rotation" : "the attention over the rows before the codec stored them"} lands outside the bar")
        }
        codec_attn_mechanism_controls(t, tag, a, f, got)
        codec_attn_ring_control(t, tag, a, cp, s, pa, slab, rowwgs, yp, pp, cnt_off, rowgroups, want)
        if (a.fold != Fold.none) {
            var quants <- cp |> plane_as(aqp, type<int8>, a.nrows * qdim)
            var scales <- cp |> plane_run(asp)
            fold_checks(t, tag, quants, scales, want, control, blk)
            delete quants
            delete scales
        }
        if (a.resubmit) {
            cp |> plane_refill(yp)
            cp |> cell_record() $(rw : VkCommandBuffer; var h : VkHaz) {
                da_attn_stamp_enc(a.kv, slab, rw, h, s, pa, rowwgs * int64(a.nrows))
            }
            var again <- cp |> plane_run(yp)
            check_rows(t, "{tag}: the dispatch again over the counters the first left (they rearmed)", again, want, QA_BAR, qdim, a.hs)
            delete again
        }
        if (a.g4_twin) {
            t |> equal(slab, DaSlab.two, "{tag}: the four-head twin rides a two-head slab's arm")
            cp |> plane_refill(yp)
            var s4 = da_attn_stamp_set(a.kv, DaSlab.four, gather_planes(cp.dev, planes), gather_planes(cp.bytes, planes), gather_planes(cp.hazard_bit, planes))
            cp |> cell_record() $(rw : VkCommandBuffer; var h : VkHaz) {
                da_attn_stamp_enc(a.kv, DaSlab.four, rw, h, s4, pa, rowwgs * int64(a.nrows))
            }
            var g4 <- cp |> plane_run(yp)
            g4_twin_check(t, tag, g4, got, qdim, a.hs)
            delete g4
        }
        delete got
    }
    delete want
    delete control
    delete toks
    delete f
}

//! a ring arm's control: the same rows read a position a row (`ring` 0) - the zero rows past the ring - land outside the bar
def private codec_attn_ring_control(t : T?; tag : string; a : CodecAttn; var cp : CellPlanes; var s : VkDescriptorSet; pa : DaAttnArgs;
                                    slab : DaSlab; rowwgs : int64; yp, pp, cnt_off, rowgroups : int; want : array<float>) {
    return if (a.ring == 0)
    cp |> plane_refill(yp)
    cp |> plane_refill(pp)
    upload_zeros(cp.dev[pp], int64(cnt_off) * 4l, int64(a.nrows * rowgroups) * 4l)   //! the partials and the arrival counters start as the first run found them
    var flat = pa
    flat.ring = 0u
    cp |> cell_record() $(rw : VkCommandBuffer; var h : VkHaz) {
        da_attn_stamp_enc(a.kv, slab, rw, h, s, flat, rowwgs * int64(a.nrows))
    }
    var flat_got <- cp |> plane_run(yp)
    t |> success(check_count(flat_got, want, QA_BAR) > 0, "{tag} control: the ring's rows read a position a row land outside the bar")
    delete flat_got
}

//! the oracle with each mechanism the arm turns on dropped lands outside the bar, and row 1 misses row 0's oracle
def private codec_attn_mechanism_controls(t : T?; tag : string; a : CodecAttn; f : CodecAttnFixture; got : array<float>) {
    let qdim = a.nheads * a.hs
    if (a.sink) {
        var ctl <- codec_attn_oracle(a, f, f.q, f.k.deq, f.v.deq, true, true, false)
        t |> success(check_count(got, ctl, QA_BAR) > 0, "{tag} control: the sink-free oracle lands outside the bar")
        delete ctl
    }
    if (a.window != 0) {
        var ctl <- codec_attn_oracle(a, f, f.q, f.k.deq, f.v.deq, false)
        t |> success(check_count(got, ctl, QA_BAR) > 0, "{tag} control: the oracle over every key lands outside the bar")
        delete ctl
    }
    if (a.cap != 0.0) {
        var ctl <- codec_attn_oracle(a, f, f.q, f.k.deq, f.v.deq, true, false)
        t |> success(check_count(got, ctl, QA_BAR) > 0, "{tag} control: the uncapped oracle lands outside the bar")
        delete ctl
    }
    if (a.gated) {
        var ctl <- codec_attn_oracle(a, f, f.q, f.k.deq, f.v.deq, true, true, true, false)
        t |> success(check_count(got, ctl, QA_BAR) > 0, "{tag} control: the ungated oracle lands outside the bar")
        delete ctl
    }
    if (a.nrows > 1) {
        var row1 <- copy_span(got, qdim, qdim)
        var want <- codec_attn_oracle(a, f, f.q, f.k.deq, f.v.deq)
        var row0 <- copy_span(want, 0, qdim)
        t |> success(check_count(row1, row0, QA_BAR) > 0, "{tag} control: row 1 read its own region - row 0's oracle misses it")
        delete row1
        delete want
        delete row0
    }
}

//! every path the pass has, on codec `kv`'s stamps
def codec_attn_arms(t : T?; kv : KVDtype) {
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 64, nheads = 4, kv_mul = 4))     // the four-head slab, a 64-wide head: the lanes fold inside a subgroup
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 32, nheads = 4, kv_mul = 4))     // one block a head: the K block loop's single trip
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 128, nheads = 8, kv_mul = 4))    // two K/V heads: the second head's blocks sit past the first's
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 64, nheads = 14, kv_mul = 7))    // a short last slab: four heads, then three
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 64, nheads = 4, kv_mul = 4, nsplit = 8))   // the keys in eight pieces, the last-arriving piece combining
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 256, nheads = 2, kv_mul = 2, keys = 70))   // a 256-wide head: the lanes own one dim slice each
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 512, nheads = 2, kv_mul = 2, keys = 70))   // the wide head: a thread's second dim
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 64, nheads = 4, kv_mul = 2))               // the two-head slab
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 64, nheads = 4, kv_mul = 2, nsplit = 8))   // ... split
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 64, nheads = 4, kv_mul = 2, nrows = 2))    // two rows a dispatch, each over its own mirror region
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 64, nheads = 4, kv_mul = 4, nrows = 2, nsplit = 4))
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 64, nheads = 4, kv_mul = 2, fold = Fold.q8_0))               // the pass's own Q8_0 store
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 64, nheads = 4, kv_mul = 2, fold = Fold.q8_0, nsplit = 8))   // ... the combine's
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 256, nheads = 2, kv_mul = 2, fold = Fold.q8_k, keys = 70))   // one superblock a head
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 512, nheads = 2, kv_mul = 2, fold = Fold.q8_k, keys = 70, nsplit = 4))
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 64, nheads = 4, kv_mul = 4, gated = true))
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 64, nheads = 4, kv_mul = 4, gated = true, nsplit = 8))
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 64, nheads = 4, kv_mul = 2, window = 40))
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 256, nheads = 2, kv_mul = 2, window = 40, cap = 1.5))   // gemma-2's sliding layer, the softcap inside the fixture's scores so it moves the row
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 64, nheads = 4, kv_mul = 2, window = 40, ring = 64))    // the sliding layer's ring: the window's keys across the slots' wrap
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 256, nheads = 2, kv_mul = 2, window = 100, ring = 128, nrows = 2, nsplit = 4))   // ... two regions, the keys in pieces
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 64, nheads = 8, kv_mul = 8, sink = true))               // the sink stamp: the pass folds the sink at its store
    codec_attn_arm(t, CodecAttn(kv = kv, hs = 64, nheads = 8, kv_mul = 8, sink = true, nsplit = 8))   // ... and the last piece's combine seeds it
}

let private TILE_BAR = Bar(rel = 2.0e-3)   //! the flash tile's bar: P and a step's P x V sum round to half
let private TILE_ROW_STEP = 9              //! the keys a tile row attends past the row before it: far enough apart that a dropped limit lands outside the bar

//! the keys row `r` of a tile arm attends: the last row every key of the region, each row before it TILE_ROW_STEP fewer
def private tile_cnt(a : CodecAttn; r : int) : int => a.keys - (a.nrows - 1 - r) * TILE_ROW_STEP

//! the CPU attention's rows of a tile arm, [rows x heads x hs]: every row over region 0, row r its own count of keys - or,
//! with `one_count`, the last row's (the control that drops the per-row limit); `gate` off leaves the rows ungated
def private tile_oracle(a : CodecAttn; f : CodecAttnFixture; one_count : bool = false; gate : bool = true) : array<float> {
    let kvd = a.nheads / a.kv_mul * a.hs
    let qdim = a.nheads * a.hs
    var out : array<float>
    out |> resize(a.nrows * qdim)
    for (r in range(a.nrows)) {
        let cnt = one_count ? a.keys : tile_cnt(a, r)
        let kstart = a.window != 0 && cnt > a.window ? cnt - a.window : 0
        for (hh in range(a.nheads)) {
            attn_row_oracle(f.q, r * qdim + hh * a.hs, f.k.deq, 0, f.v.deq, 0, kvd, hh, a.hs, a.kv_mul, cnt, 1.0 / sqrt(float(a.hs)),
                out, r * qdim + hh * a.hs, kstart)
        }
    }
    if (gate && a.gated) {
        gate_rows(out, f.gate)
    }
    return <- out
}

def private tile_attn_tag(a : CodecAttn) : string {
    var tag = "the flash tile's decode form, hs {a.hs}, {a.nheads} heads over {a.nheads / a.kv_mul}, {a.keys} keys in {a.nsplit} piece(s), {a.nrows} row(s) of one region"
    if (a.gated) {
        tag += ", gated"
    }
    if (a.fixcnt) {
        tag += ", one fixed count"
    }
    if (a.window != 0) {
        tag += ", a {a.window}-key window"
    }
    return tag
}

//! The decode form of the KHR flash tile, then the decode attention's combine-only dispatch, against the CPU attention a row:
//! `a.nrows` rows of one session over ONE mirror region (row r attends `tile_cnt` keys), the rows and a K/V head's query heads
//! as the tile's rows, the keys in `a.nsplit` pieces a (kv head, q tile). Heads of 256 and 512, an f16 mirror. Controls: every
//! row at the last row's count, and with a gate the ungated rows. `wgs` is the arm's dispatch counted by hand - a workgroup a
//! (kv head, tile of sixteen (row, query head) pairs, key piece) - which the drivers' grid (`fa_dec_groups`) is held to.
def tile_attn_arm(t : T?; a : CodecAttn; wgs : int64) {
    let kvd = a.nheads / a.kv_mul * a.hs
    let qdim = a.nheads * a.hs
    let qstride = a.gated ? 2 * a.hs : a.hs
    let slab = da_slab_of(false, int64(a.kv_mul))
    let tag = tile_attn_tag(a)
    var f <- codec_attn_fixture(a)
    let region = uint(qa_units(a.kv, QA_REGION_BLOCKS))
    var toks <- [for (r in range(a.nrows)); TokMeta(cnt = uint(tile_cnt(a, r)), mirbase = region)]
    let rowgroups = int(da_attn_comb_wgs(int64(a.nheads)))
    let part_floats = a.nrows * a.nheads * a.nsplit * (a.hs + 2)
    var want <- tile_oracle(a, f, a.fixcnt)
    with_cell_planes() $(var cp : CellPlanes) {
        let qp = cp |> in_(f.q_plane)
        let kp = cp |> in_plain(f.k.bytes)
        let vp = cp |> in_plain(f.v.bytes)
        let yp = cp |> out_(a.nrows * qdim)
        let tp = cp |> in_plain(toks)
        let pp = cp |> out_words(int64(part_floats + a.nrows * rowgroups) * 4l, NAN_SENTINEL_BITS)
        upload_zeros(cp.dev[pp], int64(part_floats) * 4l, int64(a.nrows * rowgroups) * 4l)
        let aqp = cp |> out_words(int64((a.nrows * qdim + 3) / 4 * 4), I8_SENTINEL_BITS)
        let asp = cp |> out_(a.nrows * qdim / 32)
        let sp = cp |> in_(f.sinks)
        let tile_planes = fixed_array(qp, kp, vp, pp, tp)
        var st = set_fa_khr_dec_cls(gather_planes(cp.dev, tile_planes), gather_planes(cp.bytes, tile_planes), gather_planes(cp.hazard_bit, tile_planes))
        let planes = fixed_array(qp, kp, vp, yp, tp, pp, aqp, asp, sp)
        var s = da_attn_stamp_set(a.kv, slab, gather_planes(cp.dev, planes), gather_planes(cp.bytes, planes), gather_planes(cp.hazard_bit, planes))
        let base = uint(qa_units(a.kv, QA_BASE_BLOCKS))
        let scale = 1.0 / sqrt(float(a.hs))
        var pt = FaArgs(rows = uint(a.nrows), qd = uint(qdim), kvd = uint(kvd), kv_mul = uint(a.kv_mul), kvbase = base, scale = scale,
            qstride = uint(a.nheads * qstride), qhs = uint(qstride), window = uint(a.window), ring = uint(a.ring), vbase = base,
            nsplit = uint(a.nsplit))
        if (a.fixcnt) {
            pt.kvbase = base + region
            pt.vbase = base + region
            pt.fixcnt = uint(a.keys)
        }
        var pa = DaAttnArgs(kvd = uint(kvd), hs = uint(a.hs), kv_mul = uint(a.kv_mul), kbase = base, vbase = base, scale = scale,
            qstride = uint(qstride), gated = a.gated ? 1u : 0u, window = uint(a.window), nsplit = uint(a.nsplit), rqk = DA_RQ_NONE,
            rowwg = uint(rowgroups), qrow = uint(a.nheads * qstride), cntoff = uint(part_floats), ring = uint(a.ring), comb = 1u)
        let tile_wgs = fa_dec_groups(int64(a.nheads / a.kv_mul), int64(a.nrows), int64(a.kv_mul)) * int64(a.nsplit)
        t |> equal(tile_wgs, wgs, "{tag}: the drivers' grid is the {wgs} workgroups counted by hand")
        cp |> cell_record() $(rw : VkCommandBuffer; var h : VkHaz) {
            if (a.hs == 512) {
                enc_fa_khr_dec_h512_cls(rw, h, st, pt, tile_wgs)
            } elif (a.hs == 256) {
                enc_fa_khr_dec_h256_cls(rw, h, st, pt, tile_wgs)
            } else {
                enc_fa_khr_dec_h128_cls(rw, h, st, pt, tile_wgs)
            }
            da_attn_stamp_enc(a.kv, slab, rw, h, s, pa, int64(rowgroups * a.nrows))
        }
        var got <- cp |> plane_run(yp)
        check_rows(t, "{tag} vs the CPU attention a row", got, want, TILE_BAR, qdim, a.hs)
        if (a.nrows > 1) {
            var ctl <- tile_oracle(a, f, !a.fixcnt)
            check_exceeds(t, a.fixcnt ? "{tag} control: each row at its own count" : "{tag} control: every row at the last row's count", got, ctl, TILE_BAR)
            delete ctl
        }
        if (a.gated) {
            var ctl <- tile_oracle(a, f, false, false)
            check_exceeds(t, "{tag} control: the ungated rows", got, ctl, TILE_BAR)
            delete ctl
        }
        delete got
    }
    delete want
    delete toks
    delete f
}
