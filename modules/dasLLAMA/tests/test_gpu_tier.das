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

[unused_argument(n_layers, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap, neox, eps, scale, qk_norm)]
def private d_prepare(n_layers, n_attn, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap : int64; neox : bool; eps, scale : float; qk_norm : bool; kv16 : bool; q_gated : bool; rot : int64; dn : RdecDnGeom) : bool {
    g_prepared = true
    g_prepared_kv16 = kv16
    g_prepared_n_attn = n_attn
    g_prepared_gated = q_gated
    g_prepared_rot = rot
    g_prepared_dn_cd = dn.cd
    return true
}

[unused_argument(norms)]
def private d_norms(norms : array<float>) {}

[unused_argument(l, mir_idx, bq, bk, bv, bo, b1, b3, b2, fq, fk, fv, fo, f1, f3, f2)]
def private d_set_layer(l, mir_idx, bq, bk, bv, bo, b1, b3, b2 : int64; fq, fk, fv, fo, f1, f3, f2 : int) {}

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

[unused_argument(cls_block, cls_fmt)]
def private d_set_cls(cls_block : int64; cls_fmt : int) {}

[unused_argument(x, cossin, cnt)]
def private d_token(x : array<float>; cossin : array<float>; pos, cnt : int64; var logits : array<float>) {
    g_token_pos = pos
    g_token_cnt = cnt
    if (!empty(logits)) {
        logits[0] = 42.0
    }
}

[unused_argument(x_batch, cos_batch, npos, logits)]
def private d_prefill(x_batch : array<float>; cos_batch : array<float>; npos : int64; var logits : array<float>) {}

[unused_argument(l, kp, vp, npos)]
def private d_sync(l : int64; kp : uint8 const?; vp : uint8 const?; npos : int64) {}

[unused_argument(l, pos, kp, vp)]
def private d_read(l, pos : int64; kp : uint8?; vp : uint8?) {}

[unused_argument(l, kp, vp, npos)]
def private d_read_bulk(l : int64; kp : uint8?; vp : uint8?; npos : int64) {}

// ===== the ids-form prefill seat's doubles (installed separately from the driver bundle) =====

var private g_ids_npos = -1l
var private g_ids_scale = 0.0
var private g_emb_block = -1l
var private g_emb_f32_vocab = -1l

[unused_argument(tokens, cos_batch)]
def private d_prefill_ids(tokens : array<int64>; emb_scale : float; cos_batch : array<float>; npos : int64; var logits : array<float>) {
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
            RdecDnGeom(cd = 96l, kd = 32l, di = 32l, nvh = 1l, nkh = 1l, ds = 32l, dconv = 4l, ba_f32 = true)))
        t |> success(g_prepared, "prepare double must have run")
        t |> success(g_prepared_kv16, "kv16 must forward to the double")
        t |> equal(g_prepared_n_attn, 1l)
        t |> success(g_prepared_gated, "the gated-q flag must forward to the double")
        t |> equal(g_prepared_rot, 4l)
        t |> equal(g_prepared_dn_cd, 96l)
        var x : array<float>
        var cs : array<float>
        var logits : array<float>
        logits |> resize(4)
        rdec_token(x, cs, 9l, 10l, logits)
        t |> equal(g_token_pos, 9l)
        t |> equal(g_token_cnt, 10l)
        t |> equal(logits[0], 42.0)
        delete wq
        delete ws
        delete x
        delete cs
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
        t |> success(!gpu_want_auto(), "auto not requested")
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
        rdec_prefill_ids(ids, 1.5, cosb, 1l, logits)
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
