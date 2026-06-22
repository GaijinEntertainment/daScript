// Fail-closed fixture: a nested struct (or fixed-array) member of a LOCAL struct value would need its
// laid-out type to match the loaded composite -- not yet handled, so it must fail closed rather than emit
// a mismatched OpCompositeExtract. (Scalar/vector/matrix members of a local struct value ARE supported;
// see test_local_struct_member.) `_`-prefix + `expect 50501` keep dastest/lint off the failing shader.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

struct Inner {
    x : float4
}
struct Outer {
    inner : Inner
    tail  : float4
}
var @ssbo @binding = 0 dat : array<Outer>
var @ssbo @binding = 1 outd : array<float>

[compute_shader(local_size_x=64, name="fc_lsv_nested_spv")]
def fc_lsv_nested {
    let i = int(gl_GlobalInvocationID.x)
    let e = dat[i]
    let inner = e.inner            // nested-struct member of a local struct value -> fail-closed
    outd[i] = inner.x.x
}
