options gen2

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_math_default        // dot_mx4q8_scalar + the "portable" backend
require dasllama/dasllama_math_aarch64_neon   // dot_mx4q8 (TBL+SDOT under arm64 JIT, scalar fallback elsewhere)
require ?llvm/daslib/llvm_tune dasllama/dasllama_math_gen  // nolint:STYLE030 — registers the "arm64-gen"/"x64-gen" repack tier when dasLLVM is mounted
require math

// MXFP4·Q8 kernels: the E8M0 scale decode, the dot leaves (scalar reference vs the NEON-intrinsic
// form), and the backend-dispatched GEMV. All data is deterministic; the fp64 elementwise-dequant
// reference is independent of the integer-factorized kernels.

def private fill_case(n : int64; var wn : array<uint8>; var we : array<uint8>; var xq : array<int8>; var xs : array<float>; seed : int) {
    let nb = n / 32l
    wn |> resize(int(n / 2l))
    we |> resize(int(nb))
    xq |> resize(int(n))
    xs |> resize(int(nb))
    for (i in range(int(n / 2l))) {
        wn[i] = uint8((i * 137 + 71 + seed * 29) & 0xff)
    }
    for (i in range(int(nb))) {
        we[i] = uint8(120 + ((i * 7 + seed) % 16))      // 2^-8 .. 2^7 — sane block-scale range
        xs[i] = 0.005 + 0.003 * float(i % 5)
    }
    for (i in range(int(n))) {
        xq[i] = int8(((i * 37 + 11 + seed * 13) & 0xff) - 128)
    }
}

// fp64 elementwise dequant-dot: sum over blocks/bytes of (lut[nib]*e8m0_half) * (xq*xs)
def private ref_dot_f64(wn : array<uint8>; we : array<uint8>; xq : array<int8>; xs : array<float>; n : int64) : double {
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    let nb = n / 32l
    var acc = 0.0lf
    for (bi in range(int(nb))) {
        let d = double(e8m0_half(uint(we[bi]))) * double(xs[bi])
        for (j in range(16)) {
            let b = int(wn[bi * 16 + j])
            acc += double(lut[b & 15]) * double(xq[bi * 32 + j]) * d
            acc += double(lut[b >> 4]) * double(xq[bi * 32 + j + 16]) * d
        }
    }
    return acc
}

def private close(t : T?; got, want : float; tol : float; what : string) {
    t |> success(abs(got - want) <= tol * max(abs(want), 1.0), "{what}: got {got} want {want}")
}

[test]
def test_e8m0_half(t : T?) {
    t |> run("normal range - powers of two around 1.0") @(t : T?) {
        t |> equal(e8m0_half(127u), 0.5)
        t |> equal(e8m0_half(128u), 1.0)
        t |> equal(e8m0_half(129u), 2.0)
        t |> equal(e8m0_half(130u), 4.0)
        t |> equal(e8m0_half(120u), 0.00390625)   // 2^-8
    }
    t |> run("edges - denormal bit patterns and the top of the range") @(t : T?) {
        t |> equal(unsafe(reinterpret<uint>(e8m0_half(0u))), 0x00200000)   // 2^-128
        t |> equal(unsafe(reinterpret<uint>(e8m0_half(1u))), 0x00400000)   // 2^-127
        t |> equal(e8m0_half(2u), 1.1754943508222875e-38)                  // 2^-126 = FLT_MIN
        t |> equal(e8m0_half(254u), 8.507059173023462e37)                  // 2^126
    }
}

[test]
def test_dot_mx4q8(t : T?) {
    t |> run("scalar reference vs fp64 elementwise dequant") @(t : T?) {
        let n = 128l
        var wn : array<uint8>
        var we : array<uint8>
        var xq : array<int8>
        var xs : array<float>
        fill_case(n, wn, we, xq, xs, 1)
        var lut : int8[16]
        mxfp4_fill_lut(lut)
        let want = float(ref_dot_f64(wn, we, xq, xs, n))
        unsafe {
            let got = dot_mx4q8_scalar(addr(lut[0]), addr(wn[0]), addr(we[0]), addr(xq[0]), addr(xs[0]), n)
            close(t, got, want, 1e-5, "dot_mx4q8_scalar vs fp64 reference")
        }
    }
    t |> run("TBL+SDOT dot vs scalar reference (block sums exact; same flush order)") @(t : T?) {
        // n = 96 is NOT a multiple of the u2-unrolled superblock, exercising the unroll tail too
        for (n in [96l, 128l, 256l]) {
            var wn : array<uint8>
            var we : array<uint8>
            var xq : array<int8>
            var xs : array<float>
            fill_case(n, wn, we, xq, xs, int(n))
            var lut : int8[16]
            mxfp4_fill_lut(lut)
            unsafe {
                let want = dot_mx4q8_scalar(addr(lut[0]), addr(wn[0]), addr(we[0]), addr(xq[0]), addr(xs[0]), n)
                let got = dot_mx4q8(addr(lut[0]), addr(wn[0]), addr(we[0]), addr(xq[0]), addr(xs[0]), n)
                close(t, got, want, 1e-6, "dot_mx4q8 vs dot_mx4q8_scalar (n = {n})")
            }
        }
    }
}

// GEMV case: d = 5 exercises a non-multiple-of-anything row count; per-row expected from the
// scalar leaf. n = 64, planes sliced per row; the activation is a single token.
def private gemv_case(var wn : array<uint8>; var we : array<uint8>; var xq : array<int8>; var xs : array<float>; var want : array<float>; n, d : int64) {
    fill_case(n * d, wn, we, xq, xs, 3)    // one long fill covers the [d x n] planes
    xq |> resize(int(n))
    xs |> resize(int(n / 32l))
    var lut : int8[16]
    mxfp4_fill_lut(lut)
    want |> resize(int(d))
    unsafe {
        for (i in range(int(d))) {
            let wnp = addr(wn[0]) + int64(i) * (n / 2l)
            let wep = addr(we[0]) + int64(i) * (n / 32l)
            want[i] = dot_mx4q8_scalar(addr(lut[0]), wnp, wep, addr(xq[0]), addr(xs[0]), n)
        }
    }
}

[test]
def test_matmul_mx4q8(t : T?) {
    t |> run("active backend GEMV matches the per-row scalar leaf") @(t : T?) {
        var wn : array<uint8>
        var we : array<uint8>
        var xq : array<int8>
        var xs : array<float>
        var want : array<float>
        gemv_case(wn, we, xq, xs, want, 64l, 5l)
        var y : array<float>
        y |> resize(5)
        matmul_mx4q8(y, wn, we, xq, xs, 64l, 5l)
        for (i in range(5)) {
            close(t, y[i], want[i], 1e-6, "row {i} (backend {active_kernel_backend()})")
        }
    }
    t |> run("every row-major backend agrees (pin A/B)") @(t : T?) {
        var wn : array<uint8>
        var we : array<uint8>
        var xq : array<int8>
        var xs : array<float>
        var want : array<float>
        gemv_case(wn, we, xq, xs, want, 64l, 5l)
        let saved = active_kernel_backend()
        for (name in kernel_backend_names()) {
            if (kernel_backend_needs_repack(name)) {   // interleaved kernels can't read row-major planes
                continue
            }
            pin_kernel_backend(name)
            var y : array<float>
            y |> resize(5)
            matmul_mx4q8(y, wn, we, xq, xs, 64l, 5l)
            for (i in range(5)) {
                close(t, y[i], want[i], 1e-6, "row {i} (backend {name})")
            }
        }
        clear_kernel_backend_pin()
        if (saved != "") {
            select_kernel_backend(saved)
        }
    }
    t |> run("gen tier: repack + interleaved GEMV matches the scalar leaf (incl. tail rows)") @(t : T?) {
        if (!has_backend("arm64-gen")) {   // registered on arm64 + JIT (with dasLLVM) only
            return
        }
        // d = 6 = one 4-row group + 2 row-major tail rows
        var wn : array<uint8>
        var we : array<uint8>
        var xq : array<int8>
        var xs : array<float>
        var want : array<float>
        gemv_case(wn, we, xq, xs, want, 64l, 6l)
        let saved = active_kernel_backend()
        pin_kernel_backend("arm64-gen")
        repack_mx4_weight(wn, we, 0l, 64l, 6l)
        var y : array<float>
        y |> resize(6)
        matmul_mx4q8(y, wn, we, xq, xs, 64l, 6l)
        for (i in range(6)) {
            close(t, y[i], want[i], 1e-6, "row {i} (arm64-gen repacked)")
        }
        clear_kernel_backend_pin()
        if (saved != "") {
            select_kernel_backend(saved)
        }
    }
}

def private has_backend(name : string) : bool {
    for (n in kernel_backend_names()) {
        if (n == name) {
            return true
        }
    }
    return false
}
