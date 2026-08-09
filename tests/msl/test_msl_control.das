options gen2
options indenting = 4

// Phase-2 control-flow gate (text layer): mutable + zero-init locals, compound assignment
// (arithmetic + bitwise), pre/post increment (statement-level das ++ arrives prefix; value-level
// postfix stays postfix), every comparison flavor per scalar class, logical &&/||/!, if/elif/else
// chains, ternary — plus the golden snapshot.

require dastest/testing_boost public
require _msl_common
require daslib/strings_boost

let GOLDEN_CONTROL = "#include <metal_stdlib>
using namespace metal;
kernel void control(device const uint* a [[buffer(0)]],
        device const uint* b [[buffer(1)]],
        device const int* p [[buffer(2)]],
        device const int* q [[buffer(3)]],
        device const float* f [[buffer(4)]],
        device const float* g [[buffer(5)]],
        device uint* out1 [[buffer(6)]],
        device int* out2 [[buffer(7)]],
        device float* out3 [[buffer(8)]],
        uint3 gl_GlobalInvocationID [[thread_position_in_grid]]) \{
    uint i = gl_GlobalInvocationID.x;
    uint x = a[i];
    uint y = b[i];
    uint acc = 0u;
    uint zed = \{\};
    bool hot = false;
    if (((x > y) && (x != 0u))) \{
        acc += x;
        hot = true;
    \} else if (((x < y) || (y >= 8u))) \{
        acc -= x;
    \} else \{
        acc *= 3u;
    \}
    if ((x <= y)) \{
        acc /= 2u;
        acc %= 101u;
    \}
    if ((x == y)) \{
        acc |= 1u;
        acc &= 255u;
        acc ^= 129u;
        acc <<= 1u;
        acc >>= 1u;
    \}
    ++acc;
    ++acc;
    --acc;
    --acc;
    uint snap = (acc++);
    uint dnap = (acc--);
    zed += ((acc + snap) + dnap);
    out1[i] = (hot ? zed : (zed + 7u));
    out1[i] += zed;
    out1[i] += (zed + 1u);
    int m = p[i];
    int w = q[i];
    int iacc = 0;
    if ((m > w)) \{
        iacc += 9;
    \}
    if ((m < w)) \{
        iacc += 2;
    \}
    if ((m >= w)) \{
        iacc += 3;
    \}
    if ((m <= w)) \{
        iacc += 4;
    \}
    if ((m == w)) \{
        iacc += 5;
    \}
    if ((m != w)) \{
        iacc += 6;
    \}
    out2[i] = ((!(m > w)) ? iacc : (-iacc));
    float s = f[i];
    float z = g[i];
    float facc = 1.0f;
    if ((s > z)) \{
        facc += 2.0f;
    \}
    if ((s < z)) \{
        facc += 4.0f;
    \}
    if ((s >= z)) \{
        facc *= 2.0f;
    \}
    if ((s <= z)) \{
        facc += 8.0f;
    \}
    if ((s == z)) \{
        facc += 16.0f;
    \}
    if ((s != z)) \{
        facc += 32.0f;
    \}
    out3[i] = facc;
\}
"

[test]
def test_control_shape(t : T?) {
    t |> run("control: locals") <| @(t : T?) {
        t |> success(contains(control_msl, "uint acc = 0u;"), "mutable local with initializer")
        t |> success(contains(control_msl, "uint zed = \{\};"), "uninitialized das local -> MSL zero-init (das zero-init semantics)")
        t |> success(contains(control_msl, "bool hot = false;"), "bool local + const")
    }
    t |> run("control: if/elif/else chain") <| @(t : T?) {
        t |> success(contains(control_msl, "if (((x > y) && (x != 0u))) \{"), "if with && condition")
        t |> success(contains(control_msl, "\} else if (((x < y) || (y >= 8u))) \{"), "elif chains as `else if` (no nesting)")
        t |> success(contains(control_msl, "\} else \{"), "final else arm")
    }
    t |> run("control: compound assignment") <| @(t : T?) {
        t |> success(contains(control_msl, "acc += x;") && contains(control_msl, "acc -= x;"), "+= -=")
        t |> success(contains(control_msl, "acc *= 3u;") && contains(control_msl, "acc /= 2u;"), "*= /=")
        t |> success(contains(control_msl, "acc %= 101u;"), "%= stays infix for uint")
        t |> success(contains(control_msl, "acc |= 1u;") && contains(control_msl, "acc &= 255u;") && contains(control_msl, "acc ^= 129u;"), "|= &= ^=")
        t |> success(contains(control_msl, "acc <<= 1u;") && contains(control_msl, "acc >>= 1u;"), "<<= >>=")
        t |> success(contains(control_msl, "iacc += 9;"), "int +=")
        t |> success(contains(control_msl, "facc += 2.0f;") && contains(control_msl, "facc *= 2.0f;"), "float += *=")
    }
    t |> run("control: increment/decrement") <| @(t : T?) {
        t |> success(contains(control_msl, "++acc;") && contains(control_msl, "--acc;"), "statement-level ++/-- (prefix)")
        t |> success(contains(control_msl, "uint snap = (acc++);"), "value-level postfix ++ (old value)")
        t |> success(contains(control_msl, "uint dnap = (acc--);"), "value-level postfix -- (old value)")
    }
    t |> run("control: comparisons + logical + ternary") <| @(t : T?) {
        t |> success(contains(control_msl, "(x <= y)") && contains(control_msl, "(x == y)"), "uint comparisons")
        t |> success(contains(control_msl, "(m >= w)") && contains(control_msl, "(m != w)"), "int comparisons")
        t |> success(contains(control_msl, "(s > z)") && contains(control_msl, "(s <= z)"), "float comparisons")
        t |> success(contains(control_msl, "(!(m > w))"), "logical not")
        t |> success(contains(control_msl, "(hot ? zed : (zed + 7u))"), "ternary select")
        t |> success(contains(control_msl, "out1[i] += zed;") && contains(control_msl, "out1[i] += (zed + 1u);"),
            "const-cond select folds to the taken side")
        t |> success(!contains(control_msl, "12345u") && !contains(control_msl, "54321u") &&
            !contains(control_msl, "(true ?") && !contains(control_msl, "(false ?"), "folded select leaves zero trace")
    }
}

[test]
def test_control_golden(t : T?) {
    t |> run("control: golden snapshot") <| @(t : T?) {
        t |> equal(control_msl, GOLDEN_CONTROL)
    }
}
