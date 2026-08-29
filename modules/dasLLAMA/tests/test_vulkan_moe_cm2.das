options gen2
options _comment_hygiene = true
options _function_length = 0   // one flat gate per arm: fixture build, hook install, dispatch, CPU-reference compare
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_math
require ?vulkan dasllama/dasllama_math_vulkan  // the MoE GPU tier under test; the test installs its hooks explicitly (no env gate)
require llvm/daslib/f16_cvt
require math
require _vk_kq_fixtures
require dasllama/dasllama_vulkan_common   // find_stack_idx: the fresh-base guard
require dasllama/dasllama_env   // the split knob the streamed cells name in their tags

// The cm2 expert chain (dasllama_vulkan_prefill's f16-fed batch arm) against a CPU reference
// that rounds exactly where the device does: the gathered activation rows to f16, the hidden
// plane to f16 after the act. Unlike test_vulkan_tier this file leaves the coopmat mode at the
// router's default - the chain serves only in mode 4 on an NV_coopmat2 device, and skips
// loudly anywhere else. Without dasVulkan (or with no Vulkan device) every check feints cleanly.

let NE = 3l   // experts per synthetic stack

def private f16r(v : float) : float => f16_to_f32(f32_to_f16(v))

def private silu(v : float) : float => v / (1.0 + exp(-v))

// the window's activation rows: modest values so |gate| stays in the sane silu regime
def private fill_x(var x : array<float>; npos, n : int64; sign : float) {
    x |> resize(npos * n)
    for (p in range64(npos)) {
        for (i in range64(n)) {
            x[p * n + i] = sign * 0.25 * float((p * 31l + i * 13l + 5l) % 255l - 127l) / 127.0
        }
    }
}

// the CPU reference of the chain's contract: x -> f16, dequantized gate/up dots, silu*up,
// h -> f16, dequantized down dot, routed weighted sum into the position's row
def private ref_chain(w1, w3, w2 : array<float>; x : array<float>; inv : array<uint>; w : array<float>;
                      expert_of_row : array<int>; n, nfe, npos, k : int64; var out : array<float>) {
    let kn = int(n)
    let knfe = int(nfe)
    out |> resize(npos * n)
    for (o in out) {
        o = 0.0
    }
    var x16 : array<float>
    x16 |> resize(npos * n)
    for (i in range64(npos * n)) {
        x16[i] = f16r(x[i])
    }
    var h16 : array<float>
    h16 |> resize(nfe)
    for (j in range64(npos * k)) {
        let b = int(inv[j])
        let p = j / k
        let e = expert_of_row[b]
        for (o in range(knfe)) {
            var g = 0.0lf
            var u = 0.0lf
            for (i in range(kn)) {
                let xv = double(x16[p * n + int64(i)])
                g += double(w1[(e * knfe + o) * kn + i]) * xv
                u += double(w3[(e * knfe + o) * kn + i]) * xv
            }
            h16[o] = f16r(silu(float(g)) * float(u))
        }
        for (o2 in range(kn)) {
            var d = 0.0lf
            for (i in range(knfe)) {
                d += double(w2[(e * kn + o2) * knfe + i]) * double(h16[i])
            }
            out[p * n + int64(o2)] += w[j] * float(d)
        }
    }
    delete x16
    delete h16
}

def private chain_cell(t : T?; tag : string; n, nfe, npos, k, cnt_a : int64; woff, stride : int64; stream : bool; head_only : bool = false) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var wq1 : array<uint8>
        var ws1 : array<uint8>
        var wq3 : array<uint8>
        var ws3 : array<uint8>
        var wq2 : array<uint8>
        var ws2 : array<uint8>
        fill_kq_stack(1, NE * nfe, n, 21l + woff % 7l, true, wq1, ws1)
        fill_kq_stack(1, NE * nfe, n, 37l + woff % 7l, true, wq3, ws3)
        fill_kq_stack(3, NE * n, nfe, 56l + woff % 7l, false, wq2, ws2)
        let woff1 = woff
        let woff3 = woff + stride
        let woff2 = woff + 2l * stride
        if (!moe_gpu_upload_stack(wq1, ws1, woff1, n, NE * nfe, 1, [stream = stream])) {
            t |> skip("{tag}: no Vulkan device (or probe/budget refused the upload)")
            return
        }
        if (!stream) {   // a base inside an earlier cell's stack resolves to THAT stack (first hit wins) and computes on its weights
            t |> success(find_stack_idx(woff1, 1) == length(g_gpu.stacks) - 1, "{tag}: the gate base resolves to the stack just uploaded")
            t |> success(find_stack_idx(woff3, 1) < 0 && find_stack_idx(woff2, 3) < 0, "{tag}: the up/down bases are fresh")
        }
        t |> success(moe_gpu_upload_stack(wq3, ws3, woff3, n, NE * nfe, 1, [stream = stream]), "{tag}: k4 up stack uploaded")
        t |> success(moe_gpu_upload_stack(wq2, ws2, woff2, nfe, NE * n, 3, [stream = stream]), "{tag}: k6 down stack uploaded")
        if (!moe_gpu_ffn_xf_ok(npos, n, 1, 1, 3)) {
            t |> skip("{tag}: the cm2 expert chain serves only in mode 4 on an NV_coopmat2 device")
            return
        }
        var w1 : array<float>
        var w3 : array<float>
        var w2 : array<float>
        dequant_kq_stack(1, wq1, ws1, NE * nfe, n, w1)
        dequant_kq_stack(1, wq3, ws3, NE * nfe, n, w3)
        dequant_kq_stack(3, wq2, ws2, NE * n, nfe, w2)
        // routing: grid entry j -> bucket row (j * 7) % nk (7 coprime to nk); rows below cnt_a are expert 1's
        let nk = npos * k
        var inv : array<uint>
        var w : array<float>
        var expert_of_row : array<int>
        inv |> resize(nk)
        w |> resize(nk)
        expert_of_row |> resize(nk)
        for (j in range64(nk)) {
            let b = (j * 7l) % nk
            inv[j] = uint(b)
            w[j] = 0.25 + 0.5 * float((j * 7l) % 5l) / 4.0
            expert_of_row[b] = b < cnt_a ? 1 : 0
        }
        let ege1 = nfe * n
        let ege2 = n * nfe
        var offs1 <- [woff1 + ege1, 0l, cnt_a, woff1, cnt_a, nk - cnt_a]
        var offs3 <- [woff3 + ege1, 0l, cnt_a, woff3, cnt_a, nk - cnt_a]
        var offs2 <- [woff2 + ege2, 0l, cnt_a, woff2, cnt_a, nk - cnt_a]
        var x : array<float>
        var gout : array<float>
        var refv : array<float>
        gout |> resize(npos * n)
        // head_only = the async arm over region A alone (the split's shape); region B's slots weigh 0
        var wg : array<float>
        wg |> resize(nk)
        for (j in range64(nk)) {
            wg[j] = int64(inv[j]) < cnt_a ? w[j] : 0.0
        }
        for (win in range(2)) {
            fill_x(x, npos, n, win == 0 ? 1.0 : -1.0)
            if (head_only) {
                matmul_moe_gpu_ffn_combined_xf_begin(offs1, offs3, offs2, 1l, x, wg, inv, n, nfe, nk, npos, 1, 1, 3, false)
                matmul_moe_gpu_ffn_combined_xf_join(gout)
                ref_chain(w1, w3, w2, x, inv, wg, expert_of_row, n, nfe, npos, k, refv)
            } else {
                matmul_moe_gpu_ffn_combined_xf(gout, offs1, offs3, offs2, 2l, x, w, inv, n, nfe, nk, npos, 1, 1, 3, false)
                ref_chain(w1, w3, w2, x, inv, w, expert_of_row, n, nfe, npos, k, refv)
            }
            let mr = max_abs(refv)
            let rel_bar = 4e-3 * mr
            let bad = count_bad(gout, refv, 2e-2, rel_bar)
            let md = max_diff(gout, refv)
            to_log(LOG_INFO, "{tag} win {win}: {bad} of {npos * n} off the reference (maxdiff {md}, maxref {mr}, rel bar {rel_bar})\n")
            t |> success(mr > 0.0, "{tag} win {win}: the reference is non-trivial")
            t |> success(bad == 0, "{tag} win {win}: cm2 expert chain matches the CPU reference ({bad} of {npos * n} off, maxdiff {md} vs abs 2e-2 / rel {rel_bar})")
            if (win == 0) {
                var poison := gout   // the bar's control: one element pushed past both bars must red
                poison[0] += 1.0 + 2.0 * mr
                t |> success(count_bad(poison, refv, 2e-2, rel_bar) > 0, "{tag}: the chain bar reds a poisoned element")
                delete poison
            }
        }
        delete wg
        delete refv
        delete gout
        delete x
        delete offs1
        delete offs3
        delete offs2
        delete inv
        delete w
        delete expert_of_row
        delete w1
        delete w3
        delete w2
        delete wq1
        delete ws1
        delete wq3
        delete ws3
        delete wq2
        delete ws2
    }
}

[test]
def test_vulkan_moe_cm2_chain(t : T?) {
    t |> run("cm2 expert chain (k4 gate/up + k6 down, f16 feed) == the CPU reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
            set_moe_gpu_ffn_xf_hooks(@@vk_moe_ffn_xf, @@vk_moe_ffn_xf_ok)
            // small regions (20 + 20 rows): one s-tile row column per region, d = 512 on every GEMM
            chain_cell(t, "small", 512l, 512l, 20l, 2l, 20l, 21000000l, 1000000l, false)
            // a 20 + 10 pair: region B starts at row 20 and ends inside the tile that started it - the
            // s tile's B layout must cover the whole column it loads, not just the region's last row.
            // Bases step past the previous cell's three stacks (NE * nfe * n elements each)
            chain_cell(t, "small-b", 512l, 512l, 15l, 2l, 20l, 25000000l, 1000000l, false)
            // big regions (400 + 200 rows) over a 2048-wide model dim: multi-row-tile regions, the
            // 8-superblock k4 decode, and the down GEMM's l-tile pick at d = 2048
            chain_cell(t, "big", 2048l, 512l, 300l, 2l, 400l, 31000000l, 4000000l, false)
        } else {
            t |> skip("dasVulkan not installed; the cm2 expert chain is untestable here")
        }
    }
}

[test]
def test_vulkan_moe_cm2_streamed(t : T?) {
    t |> run("cm2 expert chain over a streamed group (slot rebase + the timeline hand-off) == the CPU reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
            set_moe_gpu_ffn_xf_hooks(@@vk_moe_ffn_xf, @@vk_moe_ffn_xf_ok)
            chain_cell(t, "streamed (DASLLAMA_GPU_MOE_SPLIT={g_env_gpu.gpu_moe_split ? 1 : 0})", 512l, 512l, 40l, 2l, 48l, 41000000l, 1000000l, true)
        } else {
            t |> skip("dasVulkan not installed; the cm2 expert chain is untestable here")
        }
    }
}

[test]
def test_vulkan_moe_cm2_split(t : T?) {
    t |> run("cm2 expert chain async arm: a region head alone (the streamed-layer split's GPU share, partial slot copy) == the CPU reference") @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            install_moe_gpu_tier(@@vk_moe_ffn, @@vk_moe_upload_stack, @@vk_moe_cls, @@vk_moe_dense, @@vk_moe_drop_stacks, @@vk_moe_dn, @@vk_moe_attn)
            set_moe_gpu_ffn_xf_hooks(@@vk_moe_ffn_xf, @@vk_moe_ffn_xf_ok)
            set_moe_gpu_ffn_xf_async_hooks(@@vk_moe_ffn_xf_begin, @@vk_moe_ffn_xf_join, @@vk_moe_ffn_xf_async_rows)
            t |> success(moe_gpu_ffn_xf_async_rows(512l, 512l) >= 128l, "the async arm serves a window of bucket rows")
            // resident: the async begin/join over region A only
            chain_cell(t, "async", 512l, 512l, 20l, 2l, 20l, 51000000l, 1000000l, false, true)
            // streamed: the slot takes expert 1's bytes alone (the partial copy) before the chain
            chain_cell(t, "split (DASLLAMA_GPU_MOE_SPLIT={g_env_gpu.gpu_moe_split ? 1 : 0})", 512l, 512l, 40l, 2l, 48l, 61000000l, 1000000l, true, true)
        } else {
            t |> skip("dasVulkan not installed; the cm2 expert chain is untestable here")
        }
    }
}
