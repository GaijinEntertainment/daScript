options gen2
options _comment_hygiene = true

// The device-cooperation seam (dasllama_gpu_tier) tested as a CONTRACT, model-free and
// device-free: uninstalled slots decline (never panic on the probe surface), installed das-side
// doubles receive exactly what the forwarders were handed, and the want/arm state round-trips.
// The op-hook family (ffn/attn/dn/cls) is exercised end-to-end by test_vulkan_tier, which
// installs the real hooks. NOTE: doubles installed here stay for the process (no uninstall
// surface, by design) — they are benign: nothing consults rdec_* unless a resident driver
// armed itself, and the arm double is reset to false at the end.

require dastest/testing_boost public
require dasllama/dasllama_gpu_tier

// ===== rdec doubles: record what arrives, answer distinctively =====

var private g_reserve_fmt = -1
var private g_reserve_blocks = 0l
var private g_token_pos = -1l
var private g_token_cnt = -1l
var private g_prepared = false
var private g_prepared_kv16 = false

def private d_reserve(fmt : int; cap_blocks : int64) : bool {
    g_reserve_fmt = fmt
    g_reserve_blocks = cap_blocks
    return true
}

[unused_argument(wq, ws, n, rows, fmt)]
def private d_place(wq : array<uint8>; ws : array<uint8>; n, rows : int64; fmt : int) : int64 => 7l

[unused_argument(n_layers, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap, neox, eps, scale, qk_norm)]
def private d_prepare(n_layers, dim, qd, kv_dim, head_size, n_heads, hidden, vocab, seq_cap : int64; neox : bool; eps, scale : float; qk_norm : bool; kv16 : bool) : bool {
    g_prepared = true
    g_prepared_kv16 = kv16
    return true
}

[unused_argument(norms)]
def private d_norms(norms : array<float>) {}

[unused_argument(l, bq, bk, bv, bo, b1, b3, b2, fq, fk, fv, fo, f1, f3, f2)]
def private d_set_layer(l, bq, bk, bv, bo, b1, b3, b2 : int64; fq, fk, fv, fo, f1, f3, f2 : int) {}

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
        t |> success(!moe_gpu_tier_arm(), "tier must start unarmed")
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
        t |> success(rdec_prepare(2l, 64l, 64l, 64l, 8l, 8l, 128l, 100l, 32l, true, 1e-5, 1.0, false, true))
        t |> success(g_prepared, "prepare double must have run")
        t |> success(g_prepared_kv16, "kv16 must forward to the double")
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
        // DELTA our set_gpu_tier_want introduces, and restore to the ambient answer
        let ambient_any = gpu_want_arms_tier()
        let want = GpuTierWant(moe_layers = 3l, moe_stream = 2l, vram_mb = 1234l,
                               dn = true, attn = true, heat = 5l, qkv = true)
        set_gpu_tier_want(want)
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
        set_gpu_tier_want(GpuTierWant(qkv = true))
        t |> success(gpu_want_arms_tier(), "a single rail flips arms-tier")
        set_gpu_tier_want(GpuTierWant())
        t |> success(gpu_want_arms_tier() == ambient_any, "arms-tier resets to the ambient env state")
        warn_gpu_backend_absent()   // the no-backend-registered path must not crash whatever the env says
    }
    t |> run("arm gate reflects the registered hook; reset to unarmed") @(t : T?) {
        set_moe_gpu_arm("testtier", @@arm_on)
        t |> success(moe_gpu_tier_arm(), "armed through the double")
        t |> success(moe_gpu_backend_registered(), "registration witness follows the arm hook")
        set_moe_gpu_arm("", @@arm_off)
        t |> success(!moe_gpu_tier_arm(), "reset to unarmed for the rest of the process")
        t |> success(!moe_gpu_backend_registered(), "registration witness resets with the hook")
    }
}
