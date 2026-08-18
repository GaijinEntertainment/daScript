options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_math_default        // portable backend + dot_mx4q8_scalar
require dasllama/dasllama_math_aarch64_neon   // arm64 backends (registered under arm64 + JIT only)
require dasllama/dasllama_convert               // quantize_q8_0 (deterministic Q8 fixtures)
require math

// Region-list (groupN) GEMVs — the MoE decode dispatch fuse: N same-shape [d x n] projections in
// one dispatch, offs packing (weight, activation) element-offset pairs. Expected values come from
// the per-region single GEMV under the SAME backend pin (identical dots, only scheduling differs),
// so equality is exact per backend. Regions 0/1 share activation row 0; region 2 reads row 1.

def private close(t : T?; got, want : float; what : string) {
    t |> success(abs(got - want) <= 1e-6 * max(abs(want), 1.0), "{what}: got {got} want {want}")
}

let N = 64l
let D = 4l   // one interleaved group per region (laneq-compatible), no tail
let NREG = 3l

def private build_offs() : array<int64> {
    return <- [N * D * 0l, 0l, N * D * 1l, 0l, N * D * 2l, N]
}

def private slice_i8(src : array<int8>; off, n : int64) : array<int8> {
    return <- [for (i in range64(off, off + n)); src[i]]
}

def private slice_u8(src : array<uint8>; off, n : int64) : array<uint8> {
    return <- [for (i in range64(off, off + n)); src[i]]
}

def private slice_f(src : array<float>; off, n : int64) : array<float> {
    return <- [for (i in range64(off, off + n)); src[i]]
}

[test]
def test_q8q8_groupn(t : T?) {
    t |> run("q8 groupN == per-region GEMV under every backend (repack where needed)") @(t : T?) {
        // one blob of NREG regions [D x N] + 2 activation rows, all deterministic
        var wf : array<float>
        var xf : array<float>
        wf |> resize(int(NREG * D * N))
        xf |> resize(int(2l * N))
        for (i in range(int(NREG * D * N))) {
            wf[i] = sin(float(i) * 0.11f)
        }
        for (i in range(int(2l * N))) {
            xf[i] = cos(float(i) * 0.07f)
        }
        let wq0 <- quantize_q8_0(wf, NREG * D * N)
        let xq <- quantize_q8_0(xf, 2l * N)
        var offs <- build_offs()
        let saved = active_kernel_backend()
        for (name in kernel_backend_names()) {
            pin_kernel_backend(name)
            // per-backend weight copy: repack backends read their interleaved layout
            var wq : array<int8>
            var ws : array<float>
            wq := wq0.q
            ws := wq0.scales
            if (kernel_backend_needs_repack(name)) {
                for (r in range64(NREG)) {
                    repack_q8q8_weight(wq, ws, r * D * N, N, D)
                }
            }
            var want : array<float>
            want |> resize(int(NREG * D))
            for (r in range64(NREG)) {   // per-region single GEMVs = the reference
                let rwq <- slice_i8(wq, r * D * N, D * N)
                let rws <- slice_f(ws, r * D * N / 32l, D * N / 32l)
                let rxq <- slice_i8(xq.q, offs[r * 2l + 1l], N)
                let rxs <- slice_f(xq.scales, offs[r * 2l + 1l] / 32l, N / 32l)
                var yv : array<float>
                yv |> resize(int(D))
                matmul_q8q8(yv, rwq, rws, rxq, rxs, N, D)
                for (i in range(int(D))) {
                    want[int(r * D) + i] = yv[i]
                }
            }
            var y : array<float>
            y |> resize(int(NREG * D))
            matmul_q8q8_groupn(y, wq, ws, offs, NREG, xq.q, xq.scales, N, D)
            for (i in range(int(NREG * D))) {
                close(t, y[i], want[i], "q8 row {i} (backend {name})")
            }
        }
        clear_kernel_backend_pin()
        if (saved != "") {
            select_kernel_backend(saved)
        }
    }
}

[test]
def test_groupn_bias_fold(t : T?) {   // nolint:STYLE038 — flat fixture build + per-backend q8/mx4 A/B sweep
    t |> run("biased groupN == unbiased + add-after, q8 and mx4, every backend") @(t : T?) {
        var wf : array<float>
        var xf : array<float>
        wf |> resize(int(NREG * D * N))
        xf |> resize(int(2l * N))
        for (i in range(int(NREG * D * N))) {
            wf[i] = sin(float(i) * 0.11f)
        }
        for (i in range(int(2l * N))) {
            xf[i] = cos(float(i) * 0.07f)
        }
        let wq0 <- quantize_q8_0(wf, NREG * D * N)
        let xq <- quantize_q8_0(xf, 2l * N)
        var wn0 : array<uint8>
        var we0 : array<uint8>
        wn0 |> resize(int(NREG * D * N / 2l))
        we0 |> resize(int(NREG * D * N / 32l))
        for (i in range(int(NREG * D * N / 2l))) {
            wn0[i] = uint8((i * 137 + 71) & 0xff)
        }
        for (i in range(int(NREG * D * N / 32l))) {
            we0[i] = uint8(120 + (i * 7) % 16)
        }
        // one bias vector per region, in a shared "fblob"-style buffer at per-region offsets
        var bias : array<float>
        var boffs : array<int64>
        bias |> resize(int(NREG * D))
        boffs |> resize(int(NREG))
        for (i in range(int(NREG * D))) {
            bias[i] = 0.25 * float(i % 7) - 0.5
        }
        for (r in range64(NREG)) {
            boffs[r] = r * D
        }
        var offs <- build_offs()
        let saved = active_kernel_backend()
        for (name in kernel_backend_names()) {
            pin_kernel_backend(name)
            var wq : array<int8>
            var ws : array<float>
            var wn : array<uint8>
            var we : array<uint8>
            wq := wq0.q
            ws := wq0.scales
            wn := wn0
            we := we0
            if (kernel_backend_needs_repack(name)) {
                for (r in range64(NREG)) {
                    repack_q8q8_weight(wq, ws, r * D * N, N, D)
                    repack_mx4_weight(wn, we, r * D * N, N, D)
                }
            }
            // q8: reference = unbiased + add-after
            var want : array<float>
            want |> resize(int(NREG * D))
            matmul_q8q8_groupn(want, wq, ws, offs, NREG, xq.q, xq.scales, N, D)
            for (r in range64(NREG)) {
                for (i in range64(D)) {
                    want[r * D + i] += bias[boffs[r] + i]
                }
            }
            var y : array<float>
            y |> resize(int(NREG * D))
            matmul_q8q8_groupn(y, wq, ws, offs, NREG, xq.q, xq.scales, bias, boffs, N, D)
            for (i in range(int(NREG * D))) {
                t |> equal(y[i], want[i])   // bit-identical: same add, same chain point
            }
            // mx4: same contract
            matmul_mx4q8_groupn(want, wn, we, offs, NREG, xq.q, xq.scales, N, D)
            for (r in range64(NREG)) {
                for (i in range64(D)) {
                    want[r * D + i] += bias[boffs[r] + i]
                }
            }
            matmul_mx4q8_groupn(y, wn, we, offs, NREG, xq.q, xq.scales, bias, boffs, N, D)
            for (i in range(int(NREG * D))) {
                t |> equal(y[i], want[i])
            }
        }
        clear_kernel_backend_pin()
        if (saved != "") {
            select_kernel_backend(saved)
        }
    }
}

[test]
def test_mx4q8_groupn(t : T?) {
    t |> run("mx4 groupN == per-region GEMV under every backend (repack where needed)") @(t : T?) {
        var wn0 : array<uint8>
        var we0 : array<uint8>
        wn0 |> resize(int(NREG * D * N / 2l))
        we0 |> resize(int(NREG * D * N / 32l))
        for (i in range(int(NREG * D * N / 2l))) {
            wn0[i] = uint8((i * 137 + 71) & 0xff)
        }
        for (i in range(int(NREG * D * N / 32l))) {
            we0[i] = uint8(120 + (i * 7) % 16)
        }
        var xf : array<float>
        xf |> resize(int(2l * N))
        for (i in range(int(2l * N))) {
            xf[i] = cos(float(i) * 0.07f)
        }
        let xq <- quantize_q8_0(xf, 2l * N)
        var offs <- build_offs()
        let saved = active_kernel_backend()
        for (name in kernel_backend_names()) {
            pin_kernel_backend(name)
            var wn : array<uint8>
            var we : array<uint8>
            wn := wn0
            we := we0
            if (kernel_backend_needs_repack(name)) {
                for (r in range64(NREG)) {
                    repack_mx4_weight(wn, we, r * D * N, N, D)
                }
            }
            var want : array<float>
            want |> resize(int(NREG * D))
            for (r in range64(NREG)) {
                let rwn <- slice_u8(wn, r * D * N / 2l, D * N / 2l)
                let rwe <- slice_u8(we, r * D * N / 32l, D * N / 32l)
                let rxq <- slice_i8(xq.q, offs[r * 2l + 1l], N)
                let rxs <- slice_f(xq.scales, offs[r * 2l + 1l] / 32l, N / 32l)
                var yv : array<float>
                yv |> resize(int(D))
                matmul_mx4q8(yv, rwn, rwe, rxq, rxs, N, D)
                for (i in range(int(D))) {
                    want[int(r * D) + i] = yv[i]
                }
            }
            var y : array<float>
            y |> resize(int(NREG * D))
            matmul_mx4q8_groupn(y, wn, we, offs, NREG, xq.q, xq.scales, N, D)
            for (i in range(int(NREG * D))) {
                close(t, y[i], want[i], "mx4 row {i} (backend {name})")
            }
        }
        clear_kernel_backend_pin()
        if (saved != "") {
            select_kernel_backend(saved)
        }
    }
}
