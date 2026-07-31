options gen2
options indenting = 4

// The kargs form: a @uniform member whose type is a STRUCT lowers to ONE `constant Foo&`
// parameter carrying every scalar, instead of one bound buffer per scalar. Fields are 4-byte
// scalars only, which is what makes the das struct and the MSL struct the same bytes — the
// host writes its own struct straight through setBytes with no padding rules to keep in sync.
// The behavioral twin is tests/metal/test_metal_uniform_struct.das.

require dastest/testing_boost public
require _msl_common
require strings

def private has(t : T?; needle : string) {
    t |> success(find(ukargs_msl, needle) >= 0, "emitted MSL carries `{needle}`")
    if (find(ukargs_msl, needle) < 0) {
        to_log(LOG_ERROR, "MISSING `{needle}` in:\n{ukargs_msl}\n")
    }
}

[test]
def test_msl_uniform_struct(t : T?) {
    t |> run("a @uniform struct lowers to one constant& parameter plus a preamble struct") <| @(t : T?) {
        // the definition precedes the kernel, field order and types preserved verbatim
        has(t, "struct UkArgs \{")
        has(t, "    uint n;")
        has(t, "    float gain;")
        has(t, "    int bias;")
        let def_at = find(ukargs_msl, "struct UkArgs \{")
        let kern_at = find(ukargs_msl, "kernel void ukargs(")
        t |> success(def_at >= 0 && kern_at > def_at, "the struct definition precedes the kernel")
        // ONE bind for all three scalars, in the constant address space
        has(t, "constant UkArgs& ka [[buffer(2)]]")
        // field access is one hop: no scalar ever becomes its own parameter
        has(t, "if ((i >= ka.n))")
        has(t, "ys[i] = ((xs[i] * ka.gain) + float(ka.bias));")
        t |> success(find(ukargs_msl, "constant uint& n") < 0, "no per-scalar uniform parameter survives")
        t |> success(find(ukargs_msl, "buffer(3)") < 0, "the three scalars cost exactly one binding")
    }
}

[test]
def test_msl_shape_stays_literal(t : T?) {
    t |> run("a per-type overload keeps the shape constant LITERAL in the emitted text") <| @(t : T?) {
        // dasLLAMA CODEREVIEW #21: shape is compile-time. Each instance must carry its own
        // constant, not take a stride as a parameter and hope the shader compiler folds it.
        t |> success(find(ukstride_msl, "xs[(blk * 34u)]") >= 0, "the int8 instance multiplies by a literal 34u")
        t |> success(find(ukstride_msl, "xs[(blk * 18u)]") >= 0, "the uint8 instance multiplies by a literal 18u")
        t |> success(find(ukstride_msl, "* bstr") < 0 && find(ukstride_msl, "uint stride") < 0,
            "no stride ever travels as a value")
        if (find(ukstride_msl, "xs[(blk * 34u)]") < 0 || find(ukstride_msl, "xs[(blk * 18u)]") < 0) {
            to_log(LOG_ERROR, "SHAPE-LITERAL: emitted MSL is\n{ukstride_msl}\n")
        }
    }
}
