options gen2

require dastest/testing_boost public
require dasllama/dasllama_math
require math

[test]
def test_silu(t : T?) {
    // Exact + hand-computed shape: silu(0)=0, silu(1)=0.7310586, silu(2)=1.7615942,
    // silu(-1)=-0.2689414 (negative — SiLU is non-monotonic, dips below zero).
    t |> run("known values") @(t : T?) {
        var x <- [0.0, 1.0, 2.0, -1.0]
        silu(x, 4l)
        t |> success(abs(x[0]) < 1e-6)              // 0 * sigmoid(0) = 0 exactly
        t |> success(abs(x[1] - 0.7310586) < 1e-4)
        t |> success(abs(x[2] - 1.7615942) < 1e-4)
        t |> success(abs(x[3] + 0.2689414) < 1e-4)
    }
    // Saturation: sigmoid -> 1 so silu(x) -> x for large +x; sigmoid -> 0 so silu(x) -> 0 for large -x.
    t |> run("saturation") @(t : T?) {
        var x <- [20.0, -20.0]
        silu(x, 2l)
        t |> success(abs(x[0] - 20.0) < 1e-4)
        t |> success(abs(x[1]) < 1e-4)
    }
    // Regression: the array overload forwards via addr(x[0]), which index-faults on an empty
    // array. size <= 0 must be a safe no-op (both silu and gelu).
    t |> run("empty / zero size is a no-op") @(t : T?) {
        var e : array<float>
        silu(e, 0l)
        gelu(e, 0l)
        var x <- [1.0, 2.0, 3.0]
        silu(x, 0l)
        gelu(x, 0l)
        t |> success(abs(x[0] - 1.0) < 1e-6)   // untouched by the zero-count calls
        t |> success(true)                      // reached here ⇒ no index-fault
    }
}

// Realistic gate/up pre-activations for the fused-kernel tests (n=12, mix of signs/magnitudes).
def private gate_inputs() : tuple<array<float>; array<float>> {
    var g <- [0.0, 1.0, 2.0, -1.0, 3.5, -4.0, 0.25, -0.1, 5.0, -2.5, 0.75, -0.5]
    var u <- [1.0, 2.0, -1.0, 0.5, 2.0, -0.5, 3.0, 1.0, -1.5, 0.25, -2.0, 4.0]
    return g => u
}

def private maxdiff(a, b : array<float>) : float {
    var m = 0.0
    for (i in range(length(a))) { m = max(m, abs(a[i] - b[i])) }
    return m
}

[test]
def test_gate(t : T?) {
    let n = 12l
    // The scalar fused gate is the bit-exact reference: swiglu(g,u) == silu(g) then *u, same order.
    t |> run("swiglu scalar == silu*u (bit-exact)") @(t : T?) {
        let inp <- gate_inputs()
        var ref := inp._0
        silu(ref, n)
        for (i in range(int(n))) { ref[i] *= inp._1[i] }
        var got := inp._0
        swiglu(got, inp._1, n)
        t |> success(maxdiff(ref, got) == 0.0, "swiglu bit-exact vs silu then *u")
    }
    t |> run("geglu scalar == gelu*u (bit-exact)") @(t : T?) {
        let inp <- gate_inputs()
        var ref := inp._0
        gelu(ref, n)
        for (i in range(int(n))) { ref[i] *= inp._1[i] }
        var got := inp._0
        geglu(got, inp._1, n)
        t |> success(maxdiff(ref, got) == 0.0, "geglu bit-exact vs gelu then *u")
    }
    // The exp4 fused gate (production default) is ~2 ulp — within tolerance of the scalar reference.
    t |> run("swiglu4 ≈ swiglu (exp4 tolerance)") @(t : T?) {
        let inp <- gate_inputs()
        var ref := inp._0
        swiglu(ref, inp._1, n)
        var got := inp._0
        swiglu4(unsafe(addr(got[0])), unsafe(addr(inp._1[0])), n)
        t |> success(maxdiff(ref, got) < 1e-3, "swiglu4 within tolerance of scalar swiglu")
    }
    t |> run("geglu4 ≈ geglu (exp4 tolerance)") @(t : T?) {
        let inp <- gate_inputs()
        var ref := inp._0
        geglu(ref, inp._1, n)
        var got := inp._0
        geglu4(unsafe(addr(got[0])), unsafe(addr(inp._1[0])), n)
        t |> success(maxdiff(ref, got) < 1e-3, "geglu4 within tolerance of scalar geglu")
    }
}
