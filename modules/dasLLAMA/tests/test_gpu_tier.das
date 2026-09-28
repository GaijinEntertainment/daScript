options gen2
options _comment_hygiene = true
options _dasllama_internal = true

// The device-cooperation seam (dasllama_gpu_tier) tested as a CONTRACT, model-free and
// device-free: uninstalled slots decline (never panic on the probe surface), installed das-side
// doubles receive exactly what the forwarders were handed, and the want/arm state round-trips.
// The op-hook family (ffn/attn/dn/cls) is exercised end-to-end by test_vulkan_tier, which
// installs the real hooks. NOTE: doubles installed here stay for the process (no uninstall
// surface, by design) — they are benign: nothing consults rdec_* unless a resident driver
// armed itself, and the arm double is reset to false at the end.

require dastest/testing_boost public
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_config
require dasllama/dasllama_env   // g_env_gpu: an exported DASLLAMA_GPU answers gpu_want_auto ahead of the want state

// ===== rdec doubles: record what arrives, answer distinctively =====

var private g_reserve_fmt = -1
var private g_reserve_blocks = 0l
var private g_token_pos = -1l
var private g_token_cnt = -1l
var private g_prepared = false
var private g_prepared_kv16 = false
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

[unused_argument(n_layers, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap, neox, eps, scale, qk_norm, mir_elems, cos_elems, regions)]
def private d_prepare(n_layers, n_attn, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap : int64; neox : bool; eps, scale : float; qk_norm : bool; kv16 : bool; q_gated : bool; rot : int64; dn : RdecDnGeom; mir_elems, cos_elems, regions : int64; carry, head : bool; verify_rows : int64) : bool {
    g_prepared = true
    g_prepared_kv16 = kv16
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

[unused_argument(bqkv, bz, bout, bbeta, balpha, b1, b3, b2, fqkv, fz, fout, f1, f3, f2, convw, wnormp, ap, dtp)]
def private d_set_dn_layer(l, bqkv, bz, bout, bbeta, balpha, b1, b3, b2, ba_off : int64; fqkv, fz, fout, f1, f3, f2 : int; convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?) {
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

[unused_argument(x, cossin, ple_row, cnt, pick_only)]
def private d_token(x : array<float>; cossin : array<float>; ple_row : array<float>; pos, cnt : int64; pick_only : bool; var logits : array<float>) : int64 {
    g_token_pos = pos
    g_token_cnt = cnt
    if (!empty(logits)) {
        logits[0] = 42.0
    }
    return -1l
}

[unused_argument(x_batch, cos_batch, ple_rows, ple_raw, npos, pos0, logits)]
def private d_prefill(x_batch : array<float>; cos_batch : array<float>; ple_rows : array<float>; ple_raw : bool; npos, pos0 : int64; var logits : array<float>) {}

[unused_argument(l, kp, vp, npos)]
def private d_sync(l : int64; kp : uint8 const?; vp : uint8 const?; npos : int64) {}

var private g_read_pos = -1l
var private g_read_bulk_npos = -1l

// the readback doubles write a sentinel through the pointers the forwarder handed them
def private d_read(l, pos : int64; var kp : uint8?; var vp : uint8?) {
    g_read_pos = pos
    *kp = uint8(l + 1l)
    *vp = uint8(pos + 1l)
}

def private d_read_bulk(l : int64; var kp : uint8?; var vp : uint8?; npos : int64) {
    g_read_bulk_npos = npos
    *kp = uint8(npos)
    *vp = uint8(l)
}

// ===== the ids-form prefill seat's doubles (installed separately from the driver bundle) =====

var private g_ids_npos = -1l
var private g_ids_scale = 0.0
var private g_emb_block = -1l
var private g_emb_f32_vocab = -1l

[unused_argument(tokens, cos_batch, pos0)]
def private d_prefill_ids(tokens : array<int64>; emb_scale : float; cos_batch : array<float>; npos, pos0 : int64; var logits : array<float>) {
    g_ids_npos = npos
    g_ids_scale = emb_scale
    if (!empty(logits)) {
        logits[0] = 43.0
    }
}

def private d_set_emb(emb_block : int64) : bool {
    g_emb_block = emb_block
    return emb_block >= 0l
}

[unused_argument(fblob, off, dim)]
def private d_upload_emb_f32(fblob : array<float>; off, vocab, dim : int64) : bool {
    g_emb_f32_vocab = vocab
    return vocab > 0l
}

var private g_carry_row = -1l

//! the carry landing's double: the row it was asked for, written into the destination's first element
def private d_land_carry(row : int64; var dst : array<float>) {
    g_carry_row = row
    dst[0] = float(row)
}

var private g_head_feh = -1
var private g_draft_row = -1l
var private g_draft_hyd0 = -1l

[unused_argument(beh, bq, bk, bv, bo, b1, b3, b2, fq, fk, fv, fo, f1, f3, f2, g)]
def private d_set_head(beh : int64; feh : int; bq, bk, bv, bo, b1, b3, b2 : int64; fq, fk, fv, fo, f1, f3, f2 : int; g : RdecLayerGeom) : bool {
    g_head_feh = feh
    return true
}

//! the draft double: the row and the hydrate start it was asked for, a sentinel logit, a distinctive pick
[unused_argument(x, h, cossin, kp, vp)]
def private d_draft(x : array<float>; h : array<float>; cossin : array<float>; row, hyd0 : int64; var kp : uint8?; var vp : uint8?; var logits : array<float>) : int64 {
    g_draft_row = row
    g_draft_hyd0 = hyd0
    logits[0] = 44.0
    return 7l
}

var private g_verify_pos = -1l
var private g_verify_n = -1l

//! the verify double: the position and row count it was asked for, a sentinel logit and hidden, a distinctive pick a row
[unused_argument(x, cossin, hemb, hcos, h0, kp, vp)]
def private d_verify(x, cossin, hemb, hcos, h0 : array<float>; pos, n : int64; var kp : uint8?; var vp : uint8?; var logits : array<float>; var picks : array<int64>; var hid : array<float>) : bool {
    g_verify_pos = pos
    g_verify_n = n
    logits[0] = 45.0
    hid[0] = 46.0
    for (i in range64(n)) {
        picks[i] = 10l + i
    }
    return true
}

def private d_verify_rows : int64 => 4l

def private arm_on() : bool => true

def private arm_off() : bool => false

[test]
def test_gpu_tier_contract(t : T?) {
    t |> run("uninstalled slots decline - never serve") @(t : T?) {
        t |> success(!moe_gpu_resident_installed(), "resident must start uninstalled")
        t |> success(!rdec_reserve(4, 100l), "unset reserve must decline")
        var wq : array<uint8>
        var ws : array<uint8>
        t |> equal(rdec_place(wq, ws, 256l, 1l, 4), -1l)
        t |> success(!moe_gpu_tier_arm_(), "tier must start unarmed")
        t |> success(!moe_gpu_backend_registered(), "no backend registered at start")
        delete wq
        delete ws
    }
    t |> run("installed rdec doubles receive exactly what the forwarders were handed") @(t : T?) {
        install_moe_gpu_resident(@@d_reserve, @@d_place, @@d_prepare, @@d_norms, @@d_set_layer,
                                 @@d_set_cls, @@d_token, @@d_prefill, @@d_sync, @@d_read, @@d_read_bulk)
        t |> success(moe_gpu_resident_installed(), "install must flip the witness")
        t |> success(rdec_reserve(6, 12345l), "double must answer")
        t |> equal(g_reserve_fmt, 6)
        t |> equal(g_reserve_blocks, 12345l)
        var wq : array<uint8>
        var ws : array<uint8>
        t |> equal(rdec_place(wq, ws, 512l, 2l, 5), 7l)
        t |> success(rdec_prepare(2l, 1l, 64l, 64l, 64l, 8l, 8l, 128l, 100l, 32l, true, 1e-5, 1.0, false, true, true, 4l,
            RdecDnGeom(cd = 96l, kd = 32l, di = 32l, nvh = 1l, nkh = 1l, ds = 32l, dconv = 4l, ba_f32 = true), 0l, 0l, 1l, true, true, 3l))
        t |> success(g_prepared, "prepare double must have run")
        t |> success(g_prepared_carry && g_prepared_head && g_prepared_verify_rows == 3l, "the carry and NextN head flags and the verify's row ask must forward to the double")
        t |> success(g_prepared_kv16, "kv16 must forward to the double")
        t |> equal(g_prepared_n_attn, 1l)
        t |> success(g_prepared_gated, "the gated-q flag must forward to the double")
        t |> equal(g_prepared_rot, 4l)
        t |> equal(g_prepared_dn_cd, 96l)
        var x : array<float>
        var cs : array<float>
        var ple : array<float>
        var logits : array<float>
        logits |> resize(4)
        rdec_token(x, cs, ple, 9l, 10l, false, logits)
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
                             @@d_set_cls, @@d_token, @@d_prefill, @@d_sync, @@d_read, @@d_read_bulk)
    var krow : uint8[2]
    var vrow : uint8[2]
    unsafe {
        rdec_read_kv(3l, 5l, addr(krow[0]), addr(vrow[0]))
        rdec_read_kv_bulk(2l, addr(krow[1]), addr(vrow[1]), 7l)
    }
    t |> equal(g_read_pos, 5l)
    t |> equal(int(krow[0]), 4, "the read double writes back through kp")
    t |> equal(int(vrow[0]), 6, "the read double writes back through vp")
    t |> equal(g_read_bulk_npos, 7l)
    t |> equal(int(krow[1]), 7, "the bulk double writes back through kp")
    t |> equal(int(vrow[1]), 2, "the bulk double writes back through vp")
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
    t |> run("the NextN draft seat installs apart, forwards, and unsets to a declining form the caller puts back") @(t : T?) {
        let rows : array<float>
        var logits : array<float>
        logits |> resize(4)
        t |> success(!rdec_draft_installed(), "no draft seat before its install")
        t |> equal(rdec_draft(rows, rows, rows, 3l, 1l, null, null, logits), -1l, "an unset draft seat declines - the CPU draft serves")
        install_rdec_draft(@@d_set_head, @@d_draft)
        t |> success(rdec_draft_installed(), "the draft install must flip its witness")
        t |> success(rdec_set_head(1l, 5, 2l, 3l, 4l, 5l, 6l, 7l, 8l, 0, 0, 0, 0, 0, 0, 0, RdecLayerGeom()), "the head double's answer forwards")
        t |> equal(g_head_feh, 5)
        t |> equal(rdec_draft(rows, rows, rows, 9l, 4l, null, null, logits), 7l, "the double's pick forwards")
        t |> equal(g_draft_row, 9l)
        t |> equal(g_draft_hyd0, 4l)
        t |> equal(logits[0], 44.0, "the double writes through the logits the forwarder handed it")
        let seat = unset_rdec_draft()
        t |> success(!rdec_draft_installed(), "the unset flips the witness back")
        g_draft_row = -1l
        t |> equal(rdec_draft(rows, rows, rows, 9l, 4l, null, null, logits), -1l, "the unset seat declines")
        t |> equal(g_draft_row, -1l, "the unset seat never reaches the installed double")
        reinstall_rdec_draft(seat)
        t |> success(rdec_draft_installed(), "the reinstall flips the witness")
        t |> equal(rdec_draft(rows, rows, rows, 11l, 0l, null, null, logits), 7l, "the seat the unset answered serves again")
        t |> equal(g_draft_row, 11l)
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
    t |> run("the same-slab verify seat installs apart and forwards the rows, the position and every landing") @(t : T?) {
        let rows : array<float>
        var logits : array<float>
        var hid : array<float>
        var picks : array<int64>
        logits |> resize(4)
        hid |> resize(4)
        picks |> resize(3)
        t |> success(!rdec_verify_installed(), "no verify seat before its install")
        t |> equal(rdec_verify_rows(), 0l, "an unset verify steps no rows")
        t |> success(!rdec_verify(rows, rows, rows, rows, rows, 5l, 2l, null, null, logits, picks, hid), "an unset verify declines - the CPU verify serves")
        install_rdec_verify(@@d_verify, @@d_verify_rows)
        t |> success(rdec_verify_installed(), "the verify install must flip its witness")
        t |> equal(rdec_verify_rows(), 4l, "the rows answer forwards")
        t |> success(rdec_verify(rows, rows, rows, rows, rows, 41l, 3l, null, null, logits, picks, hid), "the double's answer forwards")
        t |> equal(g_verify_pos, 41l)
        t |> equal(g_verify_n, 3l)
        t |> equal(logits[0], 45.0, "the double writes through the logits the forwarder handed it")
        t |> equal(hid[0], 46.0, "the double writes through the hidden rows the forwarder handed it")
        t |> equal(picks[2], 12l, "the double writes every row's pick through the picks the forwarder handed it")
        delete logits
        delete hid
        delete picks
    }
}

[test]
def test_rdec_dn_seats(t : T?) {
    t |> run("the hybrid seats install separately and forward what they are handed") @(t : T?) {
        t |> success(!rdec_dn_installed(), "no hybrid seats before their install")
        install_moe_gpu_resident_dn(@@d_set_dn_layer, @@d_upload_dn_ba, @@d_dn_own, @@d_dn_prefill_own)
        t |> success(rdec_dn_installed(), "the hybrid install must flip its witness")
        rdec_set_dn_layer(3l, 1l, 2l, 3l, -1l, -1l, 4l, 5l, 6l, 4096l, 0, 0, 0, 0, 0, 0, null, null, null, null)
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
        t |> success(!rdec_set_emb(3l), "an unset set_emb declines - the engine keeps the CPU embed")
        let nofblob : array<float>
        t |> success(!rdec_upload_emb_f32(nofblob, 0l, 16l, 8l), "an unset f32 upload declines")
        t |> equal(rdec_emb_f32_bytes(1000l, 256l), 1000l * 256l * 4l)
        t |> equal(rdec_emb_f32_bytes(1l << 20l, 4096l), 0l)   // past RDEC_EMB_F32_CAP the arm declines, so the plan counts nothing
        install_rdec_prefill_ids(@@d_prefill_ids, @@d_set_emb, @@d_upload_emb_f32)
        t |> success(rdec_prefill_ids_installed(), "install must flip the witness")
        t |> success(rdec_set_emb(9l), "the double's answer forwards")
        t |> equal(g_emb_block, 9l)
        t |> success(!rdec_set_emb(-1l), "the double's decline forwards")
        t |> success(rdec_upload_emb_f32(nofblob, 4l, 32000l, 2048l), "the f32 double's answer forwards")
        t |> equal(g_emb_f32_vocab, 32000l)
        var ids : array<int64>
        ids |> push(5l)
        let cosb : array<float>
        var logits : array<float>
        logits |> resize(4)
        rdec_prefill_ids(ids, 1.5, cosb, 1l, 0l, logits)
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
