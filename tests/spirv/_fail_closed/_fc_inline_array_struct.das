// Fail-closed fixture: a [spirv_inline] helper whose parameter is a
// struct with array members must be rejected - only a called
// OpFunction spills such a struct into a Function local.
expect 50501

options gen2

require spirv/spirv_shader
require spirv/spirv_builtins

struct FcArr {
    arr : float[4]
    k : float
}

var @ssbo @binding = 0 data : array<float>

[spirv_inline]
def pick(c : FcArr; i : int) : float {
    return c.arr[i] * c.k
}

[compute_shader(local_size_x=64, name="fc_inline_array_struct_spv")]
def fc_inline_array_struct {
    let i = int(gl_GlobalInvocationID.x)
    var s : FcArr
    s.arr[0] = data[i]
    s.k = 0.5f
    data[i] = pick(s, i & 3)
}
