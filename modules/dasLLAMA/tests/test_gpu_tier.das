options gen2
options stack = 524288   // two Sessions stand on one cell's stack (`test_segments_served_by_override`)
options _comment_hygiene = true
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

// The device-cooperation seam (dasllama_gpu_tier) tested as a CONTRACT, model-free and
// device-free: uninstalled slots decline (never panic on the probe surface), installed das-side
// doubles receive exactly what the forwarders were handed, and the want/arm state round-trips.
// The op-hook family (ffn/attn/dn/cls) is exercised end-to-end by test_vulkan_tier, which
// installs the real hooks. NOTE: doubles installed here stay for the process (no uninstall
// surface, by design) — they are benign: nothing consults rdec_* unless a resident driver
// armed itself, and the arm double is reset to false at the end.

require dastest/testing_boost public
require daslib/fio
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_config
require dasllama/dasllama_kv_dtype   // KVDtype: the mirror codec crosses the seam as its ordinal
require dasllama/dasllama_env   // g_env_gpu: an exported DASLLAMA_GPU answers gpu_want_auto ahead of the want state
require dasllama/dasllama_common   // segs_served_by_override, the prefill override registry
require strings
require math   // max: the prompt window's rounding cell
require daslib/strings_boost
require _child_run   // the override announces print through to_log and DASLLAMA_METAL_FUSE is read at context init

// ===== rdec doubles: record what arrives, answer distinctively =====

var private g_reserve_fmt = -1
var private g_reserve_blocks = 0l
var private g_token_pos = -1l
var private g_token_cnt = -1l
var private g_prepared = false
var private g_prepared_kv_dtype = -1
var private g_prepared_n_attn = -1l
var private g_prepared_gated = false
var private g_prepared_rot = -1l
var private g_prepared_dn_cd = -1l
var private g_prepared_carry = false
var private g_prepared_head = false
var private g_prepared_verify_rows = -1l
var private g_dn_layer_l = -1l
var private g_dn_layer_ba_off = -1l
var private g_dn_ba_rows = -1l
var private g_dn_own_l = -1l
var private g_dn_own_state : uint64

def private d_reserve(fmt : int; cap_blocks : int64) : bool {
    g_reserve_fmt = fmt
    g_reserve_blocks = cap_blocks
    return true
}

[unused_argument(wq, ws, n, rows, fmt)]
def private d_place(wq : array<uint8>; ws : array<uint8>; n, rows : int64; fmt : int) : int64 => 7l

[unused_argument(n_layers, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap, neox, eps, scale, qk_norm, mir_units, n_slabs, cos_elems, regions)]
def private d_prepare(n_layers, n_attn, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap : int64; neox : bool; eps, scale : float; qk_norm : bool; kv_dtype : int; q_gated : bool; rot : int64; dn : RdecDnGeom; mir_units, n_slabs, cos_elems, regions : int64; carry, head : bool; verify_rows : int64) : bool {
    g_prepared = true
    g_prepared_kv_dtype = kv_dtype
    g_prepared_n_attn = n_attn
    g_prepared_gated = q_gated
    g_prepared_rot = rot
    g_prepared_dn_cd = dn.cd
    g_prepared_carry = carry
    g_prepared_head = head
    g_prepared_verify_rows = verify_rows
    return true
}

[unused_argument(norms, obias)]
def private d_norms(norms : array<float>; obias : bool) {}

[unused_argument(l, mir_idx, bq, bk, bv, bo, b1, b3, b2, fq, fk, fv, fo, f1, f3, f2, g)]
def private d_set_layer(l, mir_idx, bq, bk, bv, bo, b1, b3, b2 : int64; fq, fk, fv, fo, f1, f3, f2 : int; g : RdecLayerGeom) {}

[unused_argument(bqkv, bz, bout, bbeta, balpha, b1, b3, b2, fqkv, fz, fout, fbeta, falpha, f1, f3, f2, convw, wnormp, ap, dtp)]
def private d_set_dn_layer(l, bqkv, bz, bout, bbeta, balpha, b1, b3, b2, ba_off : int64; fqkv, fz, fout, fbeta, falpha, f1, f3, f2 : int; convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?) {
    g_dn_layer_l = l
    g_dn_layer_ba_off = ba_off
}

def private d_upload_dn_ba(rows : array<float>) {
    g_dn_ba_rows = long_length(rows)
}

[unused_argument(histp)]
def private d_dn_own(l : int64; stp : float?; histp : float?) {
    g_dn_own_l = l
    g_dn_own_state = intptr(stp)
}

var private g_dn_prefill_own_l = -1l

[unused_argument(stp, histp)]
def private d_dn_prefill_own(l : int64; stp : float?; histp : float?) {
    g_dn_prefill_own_l = l
}

[unused_argument(cls_block, cls_fmt, cap, supp)]
def private d_set_cls(cls_block : int64; cls_fmt : int; cap : float; supp : array<int64>) {}

[unused_argument(x, cossin, ple_row, token, cnt, pick_only)]
def private d_token(x : array<float>; cossin : array<float>; ple_row : array<float>; token, pos, cnt : int64; pick_only : bool; var logits : array<float>) : int64 {
    g_token_pos = pos
    g_token_cnt = cnt
    if (!empty(logits)) {
        logits[0] = 42.0
    }
    return -1l
}

[unused_argument(x_batch, cos_batch, ple_rows, ple_raw, npos, pos0, logits, experts)]
def private d_prefill(x_batch : array<float>; cos_batch : array<float>; ple_rows : array<float>; ple_raw : bool; npos, pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) {}

var private g_sync_l = -1l
var private g_sync_row0 = -1l
var private g_sync_n = -1l
var private g_read_l = -1l
var private g_read_row0 = -1l
var private g_read_n = -1l

[unused_argument(kp, vp)]
def private d_sync(l, row0, nrows : int64; kp : uint8 const?; vp : uint8 const?) {
    g_sync_l = l
    g_sync_row0 = row0
    g_sync_n = nrows
}

def private d_read(l, row0, nrows : int64; var kp : uint8?; var vp : uint8?) {
    g_read_l = l
    g_read_row0 = row0
    g_read_n = nrows
    *kp = uint8(l + 1l)
    *vp = uint8(row0 + 1l)
}

// ===== the ids-form prefill seat's doubles (installed separately from the driver bundle) =====

var private g_ids_npos = -1l
var private g_ids_scale = 0.0
var private g_emb_block = -1l
var private g_emb_fmt = -1
var private g_emb_scale = 0.0

[unused_argument(tokens, cos_batch, pos0, experts)]
def private d_prefill_ids(tokens : array<int64>; emb_scale : float; cos_batch : array<float>; npos, pos0 : int64; var logits : array<float>; experts : RdecHostExpertsRowsFn) {
    g_ids_npos = npos
    g_ids_scale = emb_scale
    if (!empty(logits)) {
        logits[0] = 43.0
    }
}

def private d_set_emb(emb_block : int64; fmt : int; escale : float) : bool {
    g_emb_block = emb_block
    g_emb_fmt = fmt
    g_emb_scale = escale
    return emb_block >= 0l
}

var private g_emb_f32_vocab = -1l

[unused_argument(fblob, off, dim, escale)]
def private d_upload_emb_f32(fblob : array<float>; off, vocab, dim : int64; escale : float) : bool {
    g_emb_f32_vocab = vocab
    return vocab > 0l
}

var private g_carry_row = -1l

def private d_land_carry(row : int64; var dst : array<float>) {
    g_carry_row = row
    dst[0] = float(row)
}

var private g_head_feh = -1
var private g_draft_row = -1l
var private g_draft_token = -2l
var private g_draft_hyd0 = -1l
var private g_draft_pick_only = false

[unused_argument(beh, bq, bk, bv, bo, b1, b3, b2, fq, fk, fv, fo, f1, f3, f2, g)]
def private d_set_head(beh : int64; feh : int; bq, bk, bv, bo, b1, b3, b2 : int64; fq, fk, fv, fo, f1, f3, f2 : int; g : RdecLayerGeom) : bool {
    g_head_feh = feh
    return true
}

[unused_argument(l, xb, logits, acc)]
def private d_no_experts(l : int64; xb : float const?; logits : float const?; var acc : float?) {
    panic("the test's draft double never asks the host for experts")
}

[unused_argument(l, xb, logits, acc, row0, nrows)]
def private d_no_experts_rows(l : int64; xb : float const?; logits : float const?; var acc : float?; row0, nrows : int64) {
    panic("the test's verify double never asks the host for experts")
}

//! the draft double: the token, the row, the hydrate start and the pick ask it was asked for, a sentinel logit where the logits land, a distinctive pick
[unused_argument(x, h, cossin, kp, vp, experts)]
def private d_draft(x : array<float>; token : int64; h : array<float>; cossin : array<float>; row, hyd0 : int64; var kp : uint8?; var vp : uint8?; pick_only : bool; var logits : array<float>; experts : RdecHostExpertsFn) : int64 {
    g_draft_token = token
    g_draft_row = row
    g_draft_hyd0 = hyd0
    g_draft_pick_only = pick_only
    if (!pick_only) {
        logits[0] = 44.0
    }
    return 7l
}

var private g_verify_pos = -1l
var private g_verify_n = -1l
var private g_verify_pick_only = false
var private g_verify_id_last = -1l

//! the verify double: the position, row count, pick ask and last row's id it was asked for, a sentinel logit where the logits land, a sentinel hidden, a distinctive pick a row
[unused_argument(x, cossin, hemb, hcos, h0, side, experts)]
def private d_verify(x, cossin, hemb, hcos, h0, side : array<float>; ids : array<int64>; pos, n : int64; pick_only : bool; var logits : array<float>; var picks : array<int64>; var hid : array<float>; experts : RdecHostExpertsRowsFn) : bool {
    g_verify_id_last = empty(ids) ? -1l : ids[long_length(ids) - 1l]
    g_verify_pos = pos
    g_verify_n = n
    g_verify_pick_only = pick_only
    if (!pick_only) {
        logits[0] = 45.0
    }
    hid[0] = 46.0
    for (i in range64(n)) {
        picks[i] = 10l + i
    }
    return true
}

def private d_verify_rows : int64 => 4l

var private g_rollback_a = -1l

def private d_rollback(a : int64) : bool {
    g_rollback_a = a
    return true
}

var private g_head_warm_ask = false

//! the head warm doubles: the ask kept, a distinctive row count
def private d_head_warm_ask(on : bool) {
    g_head_warm_ask = on
}

def private d_head_warm_rows : int64 => 39l

def private arm_on() : bool => true

def private arm_off() : bool => false

[test]
def test_gpu_tier_contract(t : T?) {
    t |> run("uninstalled slots decline - never serve") @(t : T?) {
        t |> success(!moe_gpu_resident_installed(), "resident must start uninstalled")
        t |> success(!rdec_reserve(4, 100l), "unset reserve must decline")
        var wq, ws : array<uint8>
        t |> equal(rdec_place(wq, ws, 256l, 1l, 4), -1l)
        t |> success(!moe_gpu_tier_arm_(), "tier must start unarmed")
        t |> success(!moe_gpu_backend_registered(), "no backend registered at start")
        delete wq
        delete ws
    }
    t |> run("installed rdec doubles receive exactly what the forwarders were handed") @(t : T?) {
        install_moe_gpu_resident(@@d_reserve, @@d_place, @@d_prepare, @@d_norms, @@d_set_layer,
                                 @@d_set_cls, @@d_token, @@d_prefill, @@d_sync, @@d_read)
        t |> success(moe_gpu_resident_installed(), "install must flip the witness")
        t |> success(rdec_reserve(6, 12345l), "double must answer")
        t |> equal(g_reserve_fmt, 6)
        t |> equal(g_reserve_blocks, 12345l)
        var wq, ws : array<uint8>
        t |> equal(rdec_place(wq, ws, 512l, 2l, 5), 7l)
        t |> success(rdec_prepare(2l, 1l, 64l, 64l, 64l, 8l, 8l, 128l, 100l, 32l, true, 1e-5, 1.0, false, int(KVDtype.q8_0), true, 4l,
            RdecDnGeom(cd = 96l, kd = 32l, di = 32l, nvh = 1l, nkh = 1l, ds = 32l, dconv = 4l, ba_f32 = true), 0l, 1l, 0l, 1l, true, true, 3l))
        t |> success(g_prepared, "prepare double must have run")
        t |> success(g_prepared_carry && g_prepared_head && g_prepared_verify_rows == 3l, "the carry and NextN head flags and the verify's row ask must forward to the double")
        t |> equal(g_prepared_kv_dtype, int(KVDtype.q8_0), "the mirror codec must forward to the double")
        t |> equal(g_prepared_n_attn, 1l)
        t |> success(g_prepared_gated, "the gated-q flag must forward to the double")
        t |> equal(g_prepared_rot, 4l)
        t |> equal(g_prepared_dn_cd, 96l)
        var x, cs, ple, logits : array<float>
        logits |> resize(4)
        rdec_token(x, cs, ple, -1l, 9l, 10l, false, logits)
        t |> equal(g_token_pos, 9l)
        t |> equal(g_token_cnt, 10l)
        t |> equal(logits[0], 42.0)
        delete wq
        delete ws
        delete x
        delete cs
        delete ple
        delete logits
    }
    t |> run("want state round-trips through the accessors") @(t : T?) {
        // env-backed: DASLLAMA_GPU* may be exported (the vulkan census boxes); assert only the
        // DELTA our set_gpu_tier_want_ introduces, and restore to the ambient answer
        let ambient_any = gpu_want_arms_tier()
        let want = GpuTierWant(moe_layers = 3l, moe_stream = 2l, vram_mb = 1234l,
                               dn = true, attn = true, heat = 5l, qkv = true)
        set_gpu_tier_want_(want)
        t |> equal(gpu_want_moe_layers(), 3l)
        t |> equal(gpu_want_moe_stream(), 2l)
        t |> equal(gpu_want_vram_mb(), 1234l)
        t |> success(gpu_want_dn(), "dn wanted")
        t |> success(gpu_want_attn(), "attn wanted")
        t |> equal(gpu_want_heat(), 5l)
        t |> success(gpu_want_qkv(), "qkv wanted")
        t |> equal(gpu_want_auto(), g_env_gpu.gpu ?? false, "auto follows an exported DASLLAMA_GPU, else the set state (not requested)")
        t |> success(gpu_want_arms_tier(), "arms-tier reflects the set state")
        // membership witness: ONE rail alone must flip the predicate (the aggregate case
        // above cannot see a dropped member)
        set_gpu_tier_want_(GpuTierWant(qkv = true))
        t |> success(gpu_want_arms_tier(), "a single rail flips arms-tier")
        set_gpu_tier_want_(GpuTierWant())
        t |> success(gpu_want_arms_tier() == ambient_any, "arms-tier resets to the ambient env state")
        warn_gpu_backend_absent()   // the no-backend-registered path must not crash whatever the env says
    }
    t |> run("arm gate reflects the registered hook; reset to unarmed") @(t : T?) {
        set_moe_gpu_arm("testtier", @@arm_on)
        t |> success(moe_gpu_tier_arm_(), "armed through the double")
        t |> success(moe_gpu_backend_registered(), "registration witness follows the arm hook")
        set_moe_gpu_arm("", @@arm_off)
        t |> success(!moe_gpu_tier_arm_(), "reset to unarmed for the rest of the process")
        t |> success(!moe_gpu_backend_registered(), "registration witness resets with the hook")
    }
}

//! env-backed: an exported DASLLAMA_GPU answers ahead of the want, so the rails-alone arms assert the DELTA against the ambient answer, never false
[test]
def test_gpu_tier_resident_ask(t : T?) {
    let ambient = g_env_gpu.gpu ?? false
    set_gpu_tier_want_(GpuTierWant(dn = true, attn = true, shexp = true))
    t |> equal(gpu_resident_requested(), ambient, "rails spelled out without resident ask for no whole-model driver")
    set_gpu_tier_want_(GpuTierWant(resident = true, dn = true, attn = true, shexp = true))
    t |> success(gpu_resident_requested(), "resident beside explicit rails asks for the whole-model driver")
    t |> equal(gpu_want_auto(), ambient, "resident does not turn the measured-best set on")
    set_gpu_tier_want_(GpuTierWant(auto_tier = true))
    t |> success(gpu_resident_requested(), "the measured-best set asks for the whole-model driver")
    set_gpu_tier_want_(GpuTierWant())
    t |> equal(gpu_resident_requested(), ambient, "a zero want resets to the ambient env answer")
}

[test]
def test_gpu_cpu_passes(t : T?) {
    set_gpu_cpu_pass_reasons(["codec", "paged", "cap"], ["a codec", "pages", "the cap"])
    t |> equal(length(gpu_cpu_passes_()), 0, "nothing fired: the device served every call")
    note_gpu_cpu_pass(1)
    note_gpu_cpu_pass(1)
    note_gpu_cpu_pass(2)
    let fired <- gpu_cpu_passes_()
    t |> equal(length(fired), 2, "one row per reason that fired")
    t |> equal(fired[0].reason, "paged")
    t |> equal(fired[0].words, "pages", "a reason's words ride with its name")
    t |> equal(fired[0].count, 2l)
    t |> equal(fired[1].reason, "cap")
    t |> equal(fired[1].count, 1l)
    set_gpu_cpu_pass_reasons(["codec", "paged", "cap"], ["a codec", "pages", "the cap"])
    t |> equal(length(gpu_cpu_passes_()), 0, "a model arming again starts from zero")
}

[test]
def test_rdec_readback_reaches_the_doubles(t : T?) {
    install_moe_gpu_resident(@@d_reserve, @@d_place, @@d_prepare, @@d_norms, @@d_set_layer,
                             @@d_set_cls, @@d_token, @@d_prefill, @@d_sync, @@d_read)
    var krow, vrow : uint8[2]
    unsafe {
        rdec_kv_read(3l, 5l, 1l, addr(krow[0]), addr(vrow[0]))
        t |> equal(g_read_row0, 5l)
        t |> equal(g_read_n, 1l)
        rdec_kv_read(2l, 0l, 7l, addr(krow[1]), addr(vrow[1]))
        rdec_kv_sync(4l, 0l, 9l, addr(krow[0]), addr(vrow[0]))
    }
    t |> equal(int(krow[0]), 4, "the read double writes back through kp")
    t |> equal(int(vrow[0]), 6, "the read double writes back through vp")
    t |> equal(g_read_l, 2l)
    t |> equal(g_read_n, 7l)
    t |> equal(int(krow[1]), 3, "the whole-history read writes back through kp")
    t |> equal(int(vrow[1]), 1, "the whole-history read writes back through vp")
    t |> success(g_sync_l == 4l && g_sync_row0 == 0l && g_sync_n == 9l, "the sync forwards its layer and row window")
}

[test]
def test_rdec_carry_seat(t : T?) {
    t |> run("the speculative carry's landing installs separately and forwards the row and destination") @(t : T?) {
        t |> success(!rdec_carry_installed(), "no carry landing before its install")
        install_rdec_carry(@@d_land_carry)
        t |> success(rdec_carry_installed(), "the carry install must flip its witness")
        var dst : array<float>
        dst |> resize(4)
        rdec_land_carry(3l, dst)
        t |> equal(g_carry_row, 3l)
        t |> equal(dst[0], 3.0, "the double writes through the destination the forwarder handed it")
        delete dst
    }
}

[test]
def test_rdec_draft_seat(t : T?) {
    t |> run("the NextN draft seat installs apart, forwards the row, the hydrate start and the pick ask, and unsets to a declining form the caller puts back") @(t : T?) {
        let rows : array<float>
        var logits : array<float>
        logits |> resize(4)
        t |> success(!rdec_draft_installed(), "no draft seat before its install")
        t |> equal(rdec_draft(rows, -1l, rows, rows, 3l, 1l, null, null, false, logits, @@d_no_experts), -1l, "an unset draft seat declines - the CPU draft serves")
        install_rdec_draft(@@d_set_head, @@d_draft)
        t |> success(rdec_draft_installed(), "the draft install must flip its witness")
        t |> success(rdec_set_head(1l, 5, 2l, 3l, 4l, 5l, 6l, 7l, 8l, 0, 0, 0, 0, 0, 0, 0, RdecLayerGeom()), "the head double's answer forwards")
        t |> equal(g_head_feh, 5)
        t |> equal(rdec_draft(rows, 31l, rows, rows, 9l, 4l, null, null, false, logits, @@d_no_experts), 7l, "the double's pick forwards")
        t |> equal(g_draft_token, 31l, "the token's id forwards")
        t |> equal(g_draft_row, 9l)
        t |> equal(g_draft_hyd0, 4l)
        t |> success(!g_draft_pick_only, "the logits ask forwards")
        t |> equal(logits[0], 44.0, "the double writes through the logits the forwarder handed it")
        logits[0] = 0.0
        t |> equal(rdec_draft(rows, -1l, rows, rows, 9l, 4l, null, null, true, logits, @@d_no_experts), 7l, "the pick ask forwards and the double still answers the pick")
        t |> success(g_draft_pick_only, "the pick ask reaches the double")
        t |> equal(g_draft_token, -1l, "a host row's -1 forwards")
        t |> equal(logits[0], 0.0, "under the pick ask the double leaves the logits alone")
        let seat = unset_rdec_draft()
        t |> success(!rdec_draft_installed(), "the unset flips the witness back")
        g_draft_row = -1l
        t |> equal(rdec_draft(rows, -1l, rows, rows, 9l, 4l, null, null, false, logits, @@d_no_experts), -1l, "the unset seat declines")
        t |> equal(g_draft_row, -1l, "the unset seat never reaches the installed double")
        reinstall_rdec_draft(seat)
        t |> success(rdec_draft_installed(), "the reinstall flips the witness")
        t |> equal(rdec_draft(rows, -1l, rows, rows, 11l, 0l, null, null, false, logits, @@d_no_experts), 7l, "the seat the unset answered serves again")
        t |> equal(g_draft_row, 11l)
        unset_rdec_draft()   // back to the declining form the cell found: a later cell in this process reads the seat as it was
        t |> success(!rdec_draft_installed(), "the seat is back at its uninstalled state for the rest of the process")
        delete logits
    }
    t |> run("the norms plane: the head's block past the trunk's q/k rows and its own q/k slot, never on the final norm's row") @(t : T?) {
        let trunk = (24l * RDEC_NORM_ROWS + 1l) * 1024l
        t |> equal(rdec_norms_len(24l, 1024l, 256l, false), trunk + 24l * 2l * 256l, "no head: the layers' rows, the final norm, a q/k slot a layer")
        t |> equal(rdec_norms_len(24l, 1024l, 0l, false), trunk, "no head, no q/k norms")
        t |> equal(rdec_head_norm_base(24l, 1024l, 256l), trunk + 25l * 2l * 256l, "the head's q/k rows take the slot at index n_layers")
        t |> equal(rdec_norms_len(24l, 1024l, 256l, true), rdec_head_norm_base(24l, 1024l, 256l) + RDEC_HEAD_NORM_ROWS * 1024l)
        t |> equal(rdec_head_norm_base(24l, 1024l, 0l), trunk, "without q/k norms the block follows the final norm")
        t |> success(rdec_head_norm_base(24l, 1024l, 0l) > 24l * RDEC_NORM_ROWS * 1024l, "the block never lands on the final norm's row, where layer n_layers' own rows would index")
    }
}

[test]
def test_rdec_verify_seat(t : T?) {
    t |> run("the same-slab verify seat installs apart and forwards the rows, the position, the pick ask and every landing") @(t : T?) {
        let rows : array<float>
        let noids : array<int64>
        var ids <- [5l, 6l, 77l]
        var logits, hid : array<float>
        var picks : array<int64>
        logits |> resize(4)
        hid |> resize(4)
        picks |> resize(3)
        t |> success(!rdec_verify_installed(), "no verify seat before its install")
        t |> equal(rdec_verify_rows(), 0l, "an unset verify steps no rows")
        t |> success(!rdec_verify(rows, rows, rows, rows, rows, rows, noids, 5l, 2l, false, logits, picks, hid, @@d_no_experts_rows), "an unset verify declines - the CPU verify serves")
        install_rdec_verify(@@d_verify, @@d_verify_rows)
        t |> success(rdec_verify_installed(), "the verify install must flip its witness")
        t |> equal(rdec_verify_rows(), 4l, "the rows answer forwards")
        t |> success(rdec_verify(rows, rows, rows, rows, rows, rows, ids, 41l, 3l, false, logits, picks, hid, @@d_no_experts_rows), "the double's answer forwards")
        t |> equal(g_verify_id_last, 77l, "the rows' ids forward")
        t |> equal(g_verify_pos, 41l)
        t |> equal(g_verify_n, 3l)
        t |> success(!g_verify_pick_only, "the logits ask forwards")
        t |> equal(logits[0], 45.0, "the double writes through the logits the forwarder handed it")
        t |> equal(hid[0], 46.0, "the double writes through the hidden rows the forwarder handed it")
        t |> equal(picks[2], 12l, "the double writes every row's pick through the picks the forwarder handed it")
        logits[0] = 0.0
        hid[0] = 0.0
        picks[2] = 0l
        t |> success(rdec_verify(rows, rows, rows, rows, rows, rows, noids, 43l, 3l, true, logits, picks, hid, @@d_no_experts_rows), "the double's answer forwards under the pick ask")
        t |> success(g_verify_pick_only, "the pick ask reaches the double")
        t |> equal(g_verify_id_last, -1l, "a call with no ids forwards none")
        t |> equal(logits[0], 0.0, "under the pick ask the double leaves the logits alone")
        t |> equal(hid[0], 46.0, "... and still lands the hidden rows")
        t |> equal(picks[2], 12l, "... and every row's pick")
        delete logits
        delete hid
        delete picks
        delete ids
    }
}

[test]
def test_rdec_rollback_seat(t : T?) {
    t |> run("the verify's rollback seat installs apart: unset it declines and reaches no double, installed it forwards the committed row") @(t : T?) {
        t |> success(!rdec_rollback_installed(), "no rollback seat before its install")
        t |> success(!rdec_rollback(0l), "an unset rollback declines")
        t |> equal(g_rollback_a, -1l, "the unset seat reaches no double")
        install_rdec_rollback(@@d_rollback)
        t |> success(rdec_rollback_installed(), "the rollback install must flip its witness")
        t |> success(rdec_rollback(3l), "the double's answer forwards")
        t |> equal(g_rollback_a, 3l, "the committed row forwards")
    }
}

[test]
def test_rdec_head_warm_seat(t : T?) {
    t |> run("the head's prompt-warm seat installs apart: unset it keeps no ask and answers no rows, installed it forwards the ask and the row count; the head's K/V row ranges ride the row seats at layer n_layers") @(t : T?) {
        var kb, vb : array<uint8>
        kb |> resize(4)
        vb |> resize(4)
        t |> success(!rdec_head_warm_installed(), "no head warm seat before its install")
        rdec_head_warm_ask(true)
        t |> equal(rdec_head_warm_rows(), 0l, "an unset warm wrote no rows")
        t |> success(!g_head_warm_ask, "the unset ask reaches no double")
        install_rdec_head_warm(@@d_head_warm_ask, @@d_head_warm_rows)
        t |> success(rdec_head_warm_installed(), "the install must flip its witness")
        rdec_head_warm_ask(true)
        t |> success(g_head_warm_ask, "the ask forwards")
        t |> equal(rdec_head_warm_rows(), 39l, "the rows answer forwards")
        install_moe_gpu_resident(@@d_reserve, @@d_place, @@d_prepare, @@d_norms, @@d_set_layer,
                                 @@d_set_cls, @@d_token, @@d_prefill, @@d_sync, @@d_read)
        let n_layers = 6l
        unsafe {
            rdec_kv_sync(n_layers, 3l, 5l, addr(kb[0]), addr(vb[0]))
            rdec_kv_read(n_layers, 8l, 2l, addr(kb[0]), addr(vb[0]))
        }
        t |> success(g_sync_l == n_layers && g_sync_row0 == 3l && g_sync_n == 5l, "the head's sync forwards layer n_layers and its row window")
        t |> success(g_read_l == n_layers && g_read_row0 == 8l && g_read_n == 2l, "the head's read forwards layer n_layers and its row window")
        t |> equal(int(kb[0]), 7, "the read double writes through the K rows the forwarder handed it")
        t |> equal(int(vb[0]), 9, "the read double writes through the V rows the forwarder handed it")
        delete kb
        delete vb
    }
}

[test]
def test_rdec_dn_seats(t : T?) {
    t |> run("the hybrid seats install separately and forward what they are handed") @(t : T?) {
        t |> success(!rdec_dn_installed(), "no hybrid seats before their install")
        install_moe_gpu_resident_dn(@@d_set_dn_layer, @@d_upload_dn_ba, @@d_dn_own, @@d_dn_prefill_own)
        t |> success(rdec_dn_installed(), "the hybrid install must flip its witness")
        rdec_set_dn_layer(3l, 1l, 2l, 3l, -1l, -1l, 4l, 5l, 6l, 4096l, 0, 0, 0, 0, 0, 0, 0, 0, null, null, null, null)
        t |> equal(g_dn_layer_l, 3l)
        t |> equal(g_dn_layer_ba_off, 4096l)
        var ba : array<float>
        ba |> resize(48)
        rdec_upload_dn_ba(ba)
        t |> equal(g_dn_ba_rows, 48l)
        var st : array<float>
        st |> resize(8)
        unsafe {
            rdec_dn_own(2l, addr(st[0]), addr(st[4]))
            t |> equal(g_dn_own_l, 2l)
            t |> equal(g_dn_own_state, intptr(addr(st[0])))
            rdec_dn_prefill_own(5l, addr(st[0]), addr(st[4]))   // the prefill's slot handoff seat forwards like the owner bind
            t |> equal(g_dn_prefill_own_l, 5l)
        }
    }
}

[test]
def test_rdec_prefill_ids_seam(t : T?) {
    t |> run("the ids-form prefill seat: unset declines, installed doubles receive the forwarded values") @(t : T?) {
        t |> success(!rdec_prefill_ids_installed(), "the seat starts uninstalled")
        t |> success(!rdec_set_emb(3l, int(KqFmt.q8), 1.0), "an unset set_emb declines - the engine keeps the CPU embed")
        let nofblob : array<float>
        t |> success(!rdec_upload_emb_f32(nofblob, 0l, 16l, 8l, 1.0), "an unset f32 upload declines")
        t |> equal(rdec_emb_f32_bytes(1000l, 256l), 1000l * 256l * 4l)
        t |> equal(rdec_emb_f32_bytes(1l << 20l, 4096l), 0l)   // past RDEC_EMB_F32_CAP the arm declines, so the plan counts nothing
        install_rdec_prefill_ids(@@d_prefill_ids, @@d_set_emb, @@d_upload_emb_f32)
        t |> success(rdec_prefill_ids_installed(), "install must flip the witness")
        t |> success(rdec_set_emb(9l, int(KqFmt.iq4xs), 2.5), "the double's answer forwards")
        t |> equal(g_emb_block, 9l)
        t |> equal(g_emb_fmt, int(KqFmt.iq4xs))
        t |> equal(g_emb_scale, 2.5)
        t |> success(!rdec_set_emb(-1l, int(KqFmt.q8), 1.0), "the double's decline forwards")
        t |> success(rdec_upload_emb_f32(nofblob, 4l, 32000l, 2048l, 1.0), "the f32 double's answer forwards")
        t |> equal(g_emb_f32_vocab, 32000l)
        var ids : array<int64>
        ids |> push(5l)
        let cosb : array<float>
        var logits : array<float>
        logits |> resize(4)
        rdec_prefill_ids(ids, 1.5, cosb, 1l, 0l, logits, @@d_no_experts_rows)
        t |> equal(g_ids_npos, 1l)
        t |> equal(g_ids_scale, 1.5)
        t |> equal(logits[0], 43.0)
    }
}

[test]
def test_dlim_vulkan_tag_dn_native(t : T?) {
    t |> run("the vulkan image identity carries the native-deltanet-planes bit") @(t : T?) {
        var v = DlimVulkanConfig()
        let plain = dlim_vulkan_tag(v)
        v.dn_native = true
        t |> success(dlim_vulkan_tag(v) != plain, "the tag changes with dn_native: '{plain}' vs '{dlim_vulkan_tag(v)}'")
    }
}

[test]
def test_kernel_dump_save(t : T?) {
    t |> run("a kernel dump writes its payload under the knob's directory, writes nothing with the knob unset, and says where it could not write") @(t : T?) {
        let dir = test_temp_dir()
        t |> success(kernel_dump_save(dir, "toy.metal", "TEST_DUMP", "kernel void toy();"), "the text payload is written")
        t |> equal(fread(path_join(dir, "toy.metal")), "kernel void toy();")
        var words <- [0x07230203u, 0x00010300u, 42u]
        t |> success(kernel_dump_save(dir, "toy.spv", "TEST_DUMP", words), "the word payload is written")
        t |> equal(int(stat(path_join(dir, "toy.spv")).size), 12)
        t |> success(kernel_dump_save("", "unset.metal", "TEST_DUMP", "x"), "an unset knob is no failure")
        t |> success(!stat("unset.metal").is_valid, "and writes no file")
        t |> success(!kernel_dump_save(path_join(dir, "no_such_dir"), "toy.metal", "TEST_DUMP", "x"), "a directory that is not there answers false")
        delete words
    }
}

def private seg_probe_prefill(_t : Model; var _s : Session; _npos, _start_pos : int64) : bool => false
def private seg_probe_segments(_t : Model; var _segs : array<PrefillSeg>) : bool => false

//! the segments form declines what the one-session call declines: a session roping from per-row tables or adding deepstack
//! slices takes the window only where the active override registers that arm
[test]
def test_segments_served_by_override(t : T?) {
    t |> run("a per-row-table or deepstack session keeps the segments form off an override with no such arm") @(t : T?) {
        let found = active_prefill_override()
        register_prefill_override("seg_probe", @@seg_probe_prefill)
        register_prefill_segments_override("seg_probe", @@seg_probe_segments)
        t |> success(select_prefill_override("seg_probe"), "the probe override selects")
        var a, b : Session
        var segs <- [PrefillSeg(s = unsafe(addr(a)), start_pos = 0l, npos = 4l, toki = 0l), PrefillSeg(s = unsafe(addr(b)), start_pos = 3l, npos = 2l, toki = 1l)]
        t |> success(segs_served_by_override(segs), "two plain sessions take the segments form")
        b.mrope_pos |> push(int4(1, 2, 3, 0))
        t |> success(!segs_served_by_override(segs), "a session with a per-row rope table declines it on an override without the mrope arm")
        b.mrope_pos |> clear()
        a.ds_active = true
        t |> success(!segs_served_by_override(segs), "a deepstack session declines it on an override without the slice-add arm")
        a.ds_active = false
        t |> success(segs_served_by_override(segs), "... and both take it again with the states cleared")
        select_prefill_override("")
        t |> success(!segs_served_by_override(segs), "no active override, no segments form")
        select_prefill_override(found)
        segs |> clear()
        unsafe {
            delete segs
        }
    }
}

//! the say-once helper every GPU driver file shares: a reason set holds each `why` once, a second call with the same `why` leaves
//! it as it was, another `why` joins it, and a cleared set says it again
[test]
def test_gpu_said_once(t : T?) {
    t |> run("gpu_said_once keeps one entry a reason and says a cleared reason again") @(t : T?) {
        var said : table<string>
        t |> success(gpu_said_once(said, LOG_INFO, "the test's prefix - ", "a reason"), "the first call says the reason")
        t |> equal(length(said), 1, "... and records it")
        t |> success(!gpu_said_once(said, LOG_INFO, "the test's prefix - ", "a reason"), "the same reason again says nothing")
        t |> equal(length(said), 1, "... and records nothing")
        t |> success(gpu_said_once(said, LOG_WARNING, "the test's prefix - ", "another reason"), "another reason is said")
        t |> equal(length(said), 2, "... and joins the set")
        said |> clear()
        t |> success(gpu_said_once(said, LOG_INFO, "the test's prefix - ", "a reason"), "a cleared set (an arm) says the reason again")
        t |> equal(length(said), 1, "... and records it again")
        delete said
    }
}

//! an unset seat with no CPU form panics naming the module that installs it
[test]
def test_unset_seats_name_their_module(t : T?) {
    t |> run("the N-row token command and the per-layer planes, unset, panic naming dasllama/dasllama_math_vulkan") @(t : T?) {
        let token_why = panic_text() $ {
            var picks : array<int64>
            rdec_token_n(default<array<float>>, default<array<float>>, default<array<float>>, false, default<array<int64>>,
                default<array<int64>>, default<array<int64>>, default<array<int64>>, 2l, default<array<float?>>, picks)
        }
        t |> success(find(token_why, "require dasllama/dasllama_math_vulkan") >= 0, "the N-row token command names the module: {token_why}")
        let ple_why = panic_text() $ {
            rdec_set_ple_layer(0l, 0l, 0l, 0, 0)
        }
        t |> success(find(ple_why, "require dasllama/dasllama_math_vulkan") >= 0, "the per-layer planes name the module: {ple_why}")
    }
}

//! the lines `out` holds between the `CASE <name>` record and the next one ("" where the child ran no such case)
def private case_lines(out, name : string) : string {
    let mark = "CASE {name}\n"
    let at = find(out, mark)
    return "" if (at < 0)
    let from = at + length(mark)
    let next = find(out, "CASE ", from)
    return slice(out, from, next < 0 ? length(out) : next)
}

let private RING_ON = "dasLLAMA: probe home - the sliding layers' K/V rows ride rings of the probe's rows (set_gpu_kv_ring, DASLLAMA_GPU_KV_RING=0 keeps every row)"
let private RING_OFF = "dasLLAMA: probe home - the sliding layers' K/V rows keep every position's row, no ring, as asked (set_gpu_kv_ring, DASLLAMA_GPU_KV_RING=0)"
let private METAL_RING_ON = "dasLLAMA: metal decode - the sliding layers' K/V rows ride rings of their window plus the rows the longest window"
let private METAL_RING_OFF = "dasLLAMA: metal decode - the sliding layers' K/V rows keep every position's row, no ring, as asked (set_gpu_kv_ring, DASLLAMA_GPU_KV_RING=0)"
let private EMBED_OFF = "dasLLAMA: resident driver - the host embeds every token row as asked (set_gpu_resident_embed, DASLLAMA_VK_GPU_EMBED=0)"
let private HOT_DMA_OFF = "dasLLAMA: resident driver - the hot expert pool's misses fill on the host (set_gpu_hot_dma, DASLLAMA_GPU_HOT_DMA=0; =1 fills by DMA on the home's copy queue)"
let private DEC_MEET_ON = "dasLLAMA: resident driver - a decode token's picks on a streamed layer go through the prefill's meet (set_gpu_dec_meet, DASLLAMA_GPU_DEC_MEET=1; =0 takes the one-row host step)"
let private ONEPASS_OFF = "dasLLAMA: resident driver - the rare rail runs its three region-list passes (set_rare_onepass, DASLLAMA_RARE_ONEPASS=0; =1 runs the one-pass lane)"
let private BATCHED_OFF = "dasLLAMA: resident driver - the one-pass lane runs the rows core a row (set_rare_batched, DASLLAMA_RARE_BATCHED=0; =1 runs the batched expert cores)"
let private ARENA_OFF = "dasLLAMA: resident driver - the rare rail reads the planar planes (set_rare_arena, DASLLAMA_RARE_ARENA=0; =1 reads the pinned device-layout planes on a trimmed lane and the planar planes where the load kept them, =2 the pinned planes on every lane)"
let private HOST_FIRST_8 = "dasLLAMA: resident driver - the meet hands the host its 8 rarest experts before the device claims (set_moe_host_first, DASLLAMA_MOE_HOST_FIRST=8; 0 lets the sides claim at their own pace)"
let private PF_ROWS_300 = "dasLLAMA: resident driver - the prompt window is 256 rows (set_pf_window_rows_pin, DASLLAMA_GPU_PF_ROWS=300: rounded down to a multiple of 128, never under it; a recurrent model's window is cut at its deltanet chunk; unasked, the plan picks 4096 where it fits with its scratch, else 2048)"
let private GATHER_TAIL = " for the prefill window, the one-row step, the N-row command and the NextN draft and verify (set_gpu_resident_embed, DASLLAMA_VK_GPU_EMBED=0 keeps the host embed)"
let private GATHER_OFF = "dasLLAMA metal batch decode: the batched rows take the rows the host embeds, not the device gather off their ids, as asked (set_metal_batch_gather)"
let private G4_UNFUSED = "dasLLAMA metal decode: the gemma-4 MoE tail runs its unfused chain, not the one-step form, as asked (set_metal_moe_g4_unfused)"
let private NO_SIDECAR_RESOLVER = "dasLLAMA: no assistant drafter sidecar resolver is registered - require dasllama/dasllama_mtp_gemma (dasllama/dasllama_transformer requires it)"
let private FUSE_OFF = "dasLLAMA metal decode: every decode sandwich seam runs the split chain (the branch's post norm, the add, the next norm) on the one-row step and every batched row step, as asked (set_metal_decode_fuse, DASLLAMA_METAL_FUSE=0)"

//! the announce root under `fuse` for DASLLAMA_METAL_FUSE ("" unset), at the info floor the ringed and gather lines print at
def private announce_child(t : T?; fuse : string) : string {
    var out : string
    let rc = run_child(child_cmd("_override_announce_root.das", [(name = "DASLLAMA_METAL_FUSE", value = fuse), (name = "DAS_LOG_LEVEL", value = "info")]), out)
    t |> equal(rc, 0, "the announce root ran (DASLLAMA_METAL_FUSE='{fuse}'):\n{out}")
    t |> success(find(out, "ROOT_DONE") >= 0, "the announce root reached its end:\n{out}")
    return out
}

//! every override the GPU homes announce prints its line where it takes effect, naming the setter and the env spelling, and
//! prints nothing where it does not
[test]
def test_override_announces(t : T?) {
    let out = announce_child(t, "")
    t |> run("the sliding layers' rings announce both states, and a model with no sliding layer says nothing") @(t : T?) {
        t |> success(find(case_lines(out, "ring_on"), RING_ON) >= 0, "the ringed line:\n{case_lines(out, "ring_on")}")
        t |> success(find(case_lines(out, "ring_on"), RING_OFF) < 0, "... and no flat line beside it")
        t |> success(find(case_lines(out, "ring_off"), RING_OFF) >= 0, "the flat line under the pin:\n{case_lines(out, "ring_off")}")
        t |> success(find(case_lines(out, "ring_off"), RING_ON) < 0, "... and no ringed line beside it")
        t |> success(find(case_lines(out, "ring_none"), "sliding layers") < 0, "no sliding layer, no line in either state:\n{case_lines(out, "ring_none")}")
    }
    t |> run("the host embed announces under the pin where the ids seat would gather, and the device gather names every token command") @(t : T?) {
        t |> success(find(case_lines(out, "embed_pinned_off"), EMBED_OFF) >= 0, "the pinned host embed:\n{case_lines(out, "embed_pinned_off")}")
        t |> success(find(case_lines(out, "embed_pinned_off_no_seat"), EMBED_OFF) < 0, "no ids seat, no line")
        t |> success(find(case_lines(out, "embed_pinned_off_hc"), EMBED_OFF) < 0, "a hyper-connection model, no line")
        t |> success(find(case_lines(out, "embed_on"), EMBED_OFF) < 0, "the gather on, no line")
        let lines = case_lines(out, "embed_gather_lines")
        t |> success(find(lines, "the token rows gather on the device off the table's exact f32 rows{GATHER_TAIL}") >= 0, "the f32 rows' line:\n{lines}")
        t |> success(find(lines, "the token rows gather on the device off the table's own Q4_1 plane{GATHER_TAIL}") >= 0, "the plane's line:\n{lines}")
    }
    t |> run("the assistant drafter's sidecar lookup with no resolver registered finds none and names the module to require") @(t : T?) {
        let lines = case_lines(out, "sidecar_no_resolver")
        t |> success(find(lines, NO_SIDECAR_RESOLVER) >= 0, "the warning names the module:\n{lines}")
        t |> success(find(lines, "SIDECAR ''") >= 0, "... and the lookup finds no file:\n{lines}")
    }
    t |> run("the streamed prompt's levers announce where they move off their defaults, by setter and env spelling, and say nothing at the defaults or put back") @(t : T?) {
        let quiet = case_lines(out, "stream_levers_default")
        t |> success(find(quiet, "resident driver - the") < 0, "every lever at its default, no line:\n{quiet}")
        let moved = case_lines(out, "stream_levers_moved")
        for (line in [HOT_DMA_OFF, DEC_MEET_ON, ONEPASS_OFF, BATCHED_OFF, ARENA_OFF, HOST_FIRST_8, PF_ROWS_300]) {
            t |> equal(count(moved, line), 1, "the moved lever's line once:\n{line}\nin\n{moved}")
        }
        let back = case_lines(out, "stream_levers_restored")
        t |> success(find(back, "resident driver - the") < 0, "every lever put back, no line:\n{back}")
    }
    let metal = find(out, "CASE metal_gather_off") >= 0
    static_if (typeinfo builtin_module_exists(das_metal)) {
        t |> success(metal, "a das_metal build runs the Metal cases")
    } else {
        t |> success(!metal, "a build without das_metal runs no Metal case")
    }
    return if (!metal)
    t |> run("the Metal ring announces once a layout, ringed and flat") @(t : T?) {
        t |> equal(count(case_lines(out, "metal_ring_twice"), METAL_RING_ON), 1, "two reads of one layout, one ringed line:\n{case_lines(out, "metal_ring_twice")}")
        t |> equal(count(case_lines(out, "metal_ring_flat"), METAL_RING_OFF), 1, "the flat layout's line:\n{case_lines(out, "metal_ring_flat")}")
    }
    t |> run("the batch gather and the gemma-4 MoE tail pins announce their off state alone") @(t : T?) {
        t |> success(find(case_lines(out, "metal_gather_off"), GATHER_OFF) >= 0, "the gather off:\n{case_lines(out, "metal_gather_off")}")
        t |> success(find(case_lines(out, "metal_gather_on"), GATHER_OFF) < 0, "the gather back on, no line")
        t |> success(find(case_lines(out, "metal_g4_unfused"), G4_UNFUSED) >= 0, "the unfused tail:\n{case_lines(out, "metal_g4_unfused")}")
        t |> success(find(case_lines(out, "metal_g4_fused"), G4_UNFUSED) < 0, "the pin put back, no line")
    }
    t |> run("the fuse knob announces once a state: unset, the setter's off says it once and an off after an on again") @(t : T?) {
        t |> equal(count(out, FUSE_OFF), 2, "two off states, two lines:\n{out}")
        t |> equal(count(case_lines(out, "metal_fuse_restore"), FUSE_OFF), 0, "restoring the default on says nothing")
        t |> equal(count(case_lines(out, "metal_fuse_off"), FUSE_OFF), 1, "two offs, one line")
        t |> equal(count(case_lines(out, "metal_fuse_on_off"), FUSE_OFF), 1, "on then off, one line")
    }
    let off = announce_child(t, "0")
    t |> run("DASLLAMA_METAL_FUSE=0 announces at context init, and the setter's restore and second off say nothing more") @(t : T?) {
        t |> equal(count(off, FUSE_OFF), 2, "the init's line and the off after an on:\n{off}")
        t |> equal(count(case_lines(off, "metal_fuse_restore"), FUSE_OFF), 0, "the restore to the knob's off says nothing")
        t |> equal(count(case_lines(off, "metal_fuse_off"), FUSE_OFF), 0, "the setter's off, already said, says nothing")
        t |> equal(count(case_lines(off, "metal_fuse_on_off"), FUSE_OFF), 1, "on then off says it again")
    }
}

let private FALLBACK_CUDA = "the CPU rails serve (DASLLAMA_CUDA=1: this home has no per-op tier)"
let private FALLBACK_PEROP = "the per-op rails serve"
let private FOREIGN_HOME_VULKAN = "dasLLAMA vulkan tier: dry-bake config names home 'cuda' - this process bakes the vulkan lane alone"
let private FOREIGN_HOME_CUDA = "dasLLAMA cuda tier: dry-bake config names home 'vulkan' - this process bakes the cuda lane alone"

//! the home-words root under the ask `cuda` ("1": DASLLAMA_GPU=1 with DASLLAMA_CUDA=1; "": neither), at the info floor its records print at
def private home_words_child(t : T?; cuda : string) : string {
    var out : string
    let rc = run_child(child_cmd("_home_words_root.das", [(name = "DASLLAMA_GPU", value = cuda), (name = "DASLLAMA_CUDA", value = cuda), (name = "DAS_LOG_LEVEL", value = "info")]), out)
    t |> equal(rc, 0, "the home-words root ran (DASLLAMA_CUDA='{cuda}'):\n{out}")
    t |> success(find(out, "ROOT_DONE") >= 0, "the home-words root reached its end:\n{out}")
    return out
}

//! the texts that follow the asked home, read off a child under each ask: what serves a model the whole-model driver declined
//! (`rdec_fallback_words`: the per-op rails, or the CPU rails on the CUDA home, which has none), and each discrete home's
//! dry-bake arm refusing by name a config the other home wrote (a lane minted off it would lie about its plan)
[test]
def test_home_words(t : T?) {
    let plain = home_words_child(t, "")
    let cuda = home_words_child(t, "1")
    t |> run("a declined model falls to the per-op rails with no CUDA ask, and to the CPU rails under DASLLAMA_CUDA=1 on a dasCUDA build") @(t : T?) {
        t |> success(find(case_lines(plain, "fallback"), "FALLBACK {FALLBACK_PEROP}") >= 0, "no CUDA ask: the per-op rails serve:\n{case_lines(plain, "fallback")}")
        static_if (typeinfo builtin_module_exists(cuda)) {
            t |> success(find(case_lines(cuda, "fallback"), "FALLBACK {FALLBACK_CUDA}") >= 0, "DASLLAMA_CUDA=1: the CPU rails serve, this home having no per-op tier:\n{case_lines(cuda, "fallback")}")
        } else {
            t |> success(find(case_lines(cuda, "fallback"), "FALLBACK {FALLBACK_PEROP}") >= 0, "no dasCUDA module: the ask arms no home, the per-op rails serve:\n{case_lines(cuda, "fallback")}")
        }
    }
    t |> run("the Vulkan home's dry-bake arm refuses a config whose home is cuda, naming the home") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let lines = case_lines(plain, "vulkan_foreign")
            t |> success(find(lines, "ARM false") >= 0, "the arm refused the config:\n{lines}")
            t |> success(find(lines, FOREIGN_HOME_VULKAN) >= 0, "... naming the foreign home:\n{lines}")
        } else {
            t |> skip("no dasVulkan module on this build")
        }
    }
    t |> run("the CUDA home's dry-bake arm refuses a config whose home is vulkan, naming the home") @(t : T?) {
        static_if (typeinfo builtin_module_exists(cuda)) {
            let lines = case_lines(cuda, "cuda_foreign")
            t |> success(find(lines, "ARM false") >= 0, "the arm refused the config:\n{lines}")
            t |> success(find(lines, FOREIGN_HOME_CUDA) >= 0, "... naming the foreign home:\n{lines}")
        } else {
            t |> skip("no dasCUDA module on this build")
        }
    }
}

//! the prompt window's rows: a pin or the knob rounded down to the chain's 128-row unit, never under it
[test]
def test_pf_window_rows_rounding(t : T?) {
    let was = pf_window_rows_pin()
    set_pf_window_rows_pin(300l)
    t |> equal(pf_window_rows(), 256l, "300 pinned rounds down to 256")
    set_pf_window_rows_pin(50l)
    t |> equal(pf_window_rows(), 128l, "50 pinned takes the 128 floor")
    set_pf_window_rows_pin(1024l)
    t |> equal(pf_window_rows(), 1024l, "a multiple stays")
    set_pf_window_rows_pin(0l)
    if (g_env_gpu.gpu_pf_rows > 0l) {
        let knob_units = g_env_gpu.gpu_pf_rows / PF_WINDOW_MIN
        t |> equal(pf_window_rows(), max(knob_units, 1l) * PF_WINDOW_MIN, "no pin reads the knob, rounded the same way")
        t |> success(!pf_window_rows_unasked(), "a set knob is an ask")
    } else {
        t |> success(pf_window_rows_unasked(), "no pin and the knob at 0: the window is the driver's to pick")
        set_pf_window_rows_auto(0l)
        t |> equal(pf_window_rows(), PF_ROWS_DEFAULT, "nothing picked yet reads the narrow default")
        set_pf_window_rows_auto(PF_ROWS_WIDE)
        t |> equal(pf_window_rows(), PF_ROWS_WIDE, "the driver's pick serves under no pin and no knob")
        set_pf_window_rows_pin(512l)
        t |> equal(pf_window_rows(), 512l, "a pin outranks the pick")
        t |> success(!pf_window_rows_unasked(), "a pin is an ask")
        set_pf_window_rows_pin(0l)
        set_pf_window_rows_auto(0l)
    }
    set_pf_window_rows_pin(was)
}

//! the n-gram window seeded from a token list: the ids before position n, oldest first, -1 where the prompt is shorter
[test]
def test_ple_window_seed(t : T?) {
    var win : array<int64>
    win |> resize(4)
    let tokens <- [10l, 11l, 12l, 13l, 14l, 15l]
    ple_window_seed(win, tokens, 6l)
    t |> equal("{win}", "{[12l, 13l, 14l, 15l]}", "the window holds the four ids before the end")
    ple_window_seed(win, tokens, 2l)
    t |> equal("{win}", "{[-1l, -1l, 10l, 11l]}", "a position inside the window's reach fills the older slots with -1")
    ple_window_seed(win, tokens, 0l)
    t |> equal("{win}", "{[-1l, -1l, -1l, -1l]}", "position zero sees no id")
    t |> equal(long_length(win), 4l, "the window keeps its length")
}

//! the vulkan lane mints trimmed unless DASLLAMA_TRIM=0 or set_vulkan_trim says otherwise
[test]
def test_vulkan_trim_default(t : T?) {
    t |> success(VulkanEnv().trim, "the knob's default is the trimmed lane")
    if (!vulkan_trim_lever_set()) {
        t |> equal(vulkan_trim(), g_env_vulkan.trim, "with no lever set the lane follows the knob")
    }
}
