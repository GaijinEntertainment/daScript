options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

// Shared fixtures + gates for the dasSpirv test suite. The Phase-1 `[compute_shader] square`
// lives here so its emitted blob (`square_spv`) has a single source shared by the opcode-shape,
// census, and spirv-val tests. validate_spirv shells out to SPIRV-Tools' spirv-val (the real
// correctness oracle); it soft-skips when the tool is absent locally and CI enforces presence.

module _spirv_common shared public

require spirv/spirv_shader
require spirv/spirv_grammar
require spirv/spirv_builtins public
require daslib/fio
require math

// ===== Phase-1 fixture: data[i] = i*i, captured into the `square_spv : array<uint>` global =====
var @ssbo @binding = 0 data : array<uint>

// [marker(no_coverage)] keeps daslib/coverage from instrumenting the shader body: it is
// compile-time SPIR-V source, not runtime code, and the injected coverageData/ExprCall nodes
// are not lowerable by the emitter (they'd trip the annotation patch under --cov-path).
[compute_shader(local_size_x=64, name="square_spv"), marker(no_coverage)]
def square {
    let i = gl_GlobalInvocationID.x
    data[i] = i * i
}

// A fresh, caller-owned copy of the captured words (the global itself must not be mutated/moved).
def public square_words : array<uint> {
    var w : array<uint>
    w |> push_from(square_spv)
    return <- w
}

// ===== Phase-2 arithmetic fixtures: exercise every binary/unary scalar op the emitter supports.
// Operands are loaded SSBO values (+ the uint index), so no constants/conversions are needed yet —
// the opcode-shape + spirv-val gates only require each instruction to be emitted and valid. =====
var @ssbo @binding = 0 idata : array<int>
var @ssbo @binding = 0 fdata : array<float>

[compute_shader(local_size_x=64, name="uarith_spv"), marker(no_coverage)]
def uarith {
    let i = gl_GlobalInvocationID.x
    data[i] = data[i] + i       // IAdd
    data[i] = data[i] - i       // ISub
    data[i] = data[i] * i       // IMul
    data[i] = data[i] / i       // UDiv
    data[i] = data[i] % i       // UMod
}

[compute_shader(local_size_x=64, name="iarith_spv"), marker(no_coverage)]
def iarith {
    let i = gl_GlobalInvocationID.x
    idata[i] = idata[i] / 2     // SDiv  (signed int OpConstant 2)
    idata[i] = idata[i] % 3     // SRem  (OpConstant 3)
    idata[i] = -idata[i]        // SNegate
    idata[i] = ~idata[i]        // Not
}

[compute_shader(local_size_x=64, name="farith_spv"), marker(no_coverage)]
def farith {
    let i = gl_GlobalInvocationID.x
    fdata[i] = fdata[i] + 1.0f  // FAdd  (float OpConstant 1.0)
    fdata[i] = fdata[i] - 1.0f  // FSub
    fdata[i] = fdata[i] * 2.0f  // FMul  (OpConstant 2.0)
    fdata[i] = fdata[i] / 2.0f  // FDiv
    fdata[i] = fdata[i] % 2.0f  // FRem  (fmod, dividend's sign)
    fdata[i] = -fdata[i]        // FNegate
}

def public uarith_words : array<uint> {
    var w : array<uint>
    w |> push_from(uarith_spv)
    return <- w
}

def public iarith_words : array<uint> {
    var w : array<uint>
    w |> push_from(iarith_spv)
    return <- w
}

def public farith_words : array<uint> {
    var w : array<uint>
    w |> push_from(farith_spv)
    return <- w
}

// ===== Phase-2 control-flow fixtures: mutable locals (Function OpVariables), compound assignment,
// ++/--, comparisons (signed/unsigned/float), logical &&/||/!, and if/else (OpSelectionMerge +
// OpBranchConditional). One fixture per scalar class so every comparison-opcode flavor is covered. =====
[compute_shader(local_size_x=64, name="uctrl_spv"), marker(no_coverage)]
def uctrl {
    let i = gl_GlobalInvocationID.x
    var acc = data[i]           // mutable local -> Function OpVariable + Store
    var k = 0u
    k++                         // ++ -> Load/IAdd/Store
    acc += i                    // += -> IAdd
    acc -= k                    // -= -> ISub
    acc *= 2u                   // *= -> IMul
    if (acc > 100u) {           // UGreaterThan + if/else (SelectionMerge, BranchConditional)
        acc = acc / 3u
    } else {
        acc = acc + 1u
    }
    if (acc < i && i != 0u) {   // ULessThan, INotEqual, LogicalAnd, single-branch if
        acc = 0u
    }
    if (!(acc == 7u)) {         // IEqual + LogicalNot
        acc -= 2u
    }
    data[i] = acc
}

[compute_shader(local_size_x=64, name="ictrl_spv"), marker(no_coverage)]
def ictrl {
    let i = gl_GlobalInvocationID.x
    var acc = idata[i]
    if (acc >= 0) {             // SGreaterThanEqual
        acc += 2
    }
    if (acc <= 10) {            // SLessThanEqual
        acc -= 3
    }
    if (acc > 0 && acc < 100) { // SGreaterThan, SLessThan, LogicalAnd
        acc = acc / 2
    }
    idata[i] = acc
}

[compute_shader(local_size_x=64, name="fctrl_spv"), marker(no_coverage)]
def fctrl {
    let i = gl_GlobalInvocationID.x
    var acc = fdata[i]
    if (acc > 1.0f) {                       // FOrdGreaterThan
        acc *= 0.5f
    }
    if (acc < 0.0f || acc != 2.0f) {        // FOrdLessThan, FOrdNotEqual, LogicalOr
        acc += 3.0f
    }
    if (acc == 0.0f) {                      // FOrdEqual
        acc = 1.0f
    }
    if (acc >= 5.0f && acc <= 9.0f) {       // FOrdGreaterThanEqual, FOrdLessThanEqual
        acc -= 2.0f
    }
    fdata[i] = acc
}

def public uctrl_words : array<uint> {
    var w : array<uint>
    w |> push_from(uctrl_spv)
    return <- w
}

def public ictrl_words : array<uint> {
    var w : array<uint>
    w |> push_from(ictrl_spv)
    return <- w
}

def public fctrl_words : array<uint> {
    var w : array<uint>
    w |> push_from(fctrl_spv)
    return <- w
}

// ===== Phase-2 loop fixtures: while + range-for (OpLoopMerge 4-block) with break/continue. The
// structured CFG these produce is the real spirv-val workout — a misrouted break/continue or a
// missing block terminator fails validation. =====
[compute_shader(local_size_x=64, name="wloop_spv"), marker(no_coverage)]
def wloop {
    let i = gl_GlobalInvocationID.x
    var acc = 0u
    var k = data[i]
    while (k > 0u) {                // OpLoopMerge + UGreaterThan
        acc += k
        k--                        // -- decrement (ExprOp1) -> Load/ISub/Store
        if (acc > 1000u) {
            break                  // break -> Branch loop merge
        }
    }
    data[i] = acc
}

[compute_shader(local_size_x=64, name="floop_spv"), marker(no_coverage)]
def floop {
    let gi = gl_GlobalInvocationID.x
    var acc = 0
    for (j in range(10)) {          // range(n): init 0, SLessThan, IAdd update
        if (j == 5) {
            continue               // continue -> Branch loop continue block
        }
        acc += j
    }
    for (j in range(2, 8)) {        // range(lo, hi): init lo
        acc -= j
    }
    idata[gi] = acc
}

def public wloop_words : array<uint> {
    var w : array<uint>
    w |> push_from(wloop_spv)
    return <- w
}

def public floop_words : array<uint> {
    var w : array<uint>
    w |> push_from(floop_spv)
    return <- w
}

// ===== Phase-6 control-flow coverage fixtures (added during the AstVisitor port). Each targets a
// construct the Phase-2 fixtures left unexercised; all are byte-identical between the hand-walker and
// the visitor (golden gate) and spirv-val-clean — the structured-CFG validity oracle. =====

// nested loops (for inside while) + the full break/continue cross-product: continue+break in the
// inner for AND continue+break in the outer while (Phase-2 had break only in while, continue only in
// for). The loop_stack push/pop nesting is the #1 port risk; spirv-val rejects a misrouted target.
[compute_shader(local_size_x=64, name="nloop_spv"), marker(no_coverage)]
def nloop {
    let i = gl_GlobalInvocationID.x
    var acc = 0u
    var outer = data[i]
    while (outer > 0u) {
        outer--
        if (outer == 7u) {
            continue                    // continue in the OUTER while
        }
        for (j in range(4)) {
            if (j == 2) {
                continue                // continue in the INNER for
            }
            acc += outer
            if (acc > 5000u) {
                break                   // break out of the INNER for
            }
        }
        if (outer == 1u) {
            break                       // break out of the OUTER while
        }
    }
    data[i] = acc
}

// elif chain: if / elif / elif / else lowers to nested ExprIfThenElse in each if_false — exercises
// the recursive selection-merge nesting (Phase-2 only had single if and if/else).
[compute_shader(local_size_x=64, name="echain_spv"), marker(no_coverage)]
def echain {
    let i = gl_GlobalInvocationID.x
    var acc = idata[i]
    if (acc < 0) {
        acc = 0
    } elif (acc < 10) {
        acc++
    } elif (acc < 20) {
        acc += 2
    } else {
        acc = 100
    }
    idata[i] = acc
}

// early return inside a loop: the then-block terminates with OpReturn while the loop/merge blocks
// continue — the classic structured-CFG trap (a block already terminated + correct merge handling).
[compute_shader(local_size_x=64, name="eret_spv"), marker(no_coverage)]
def eret {
    let i = gl_GlobalInvocationID.x
    var acc = 0
    for (j in range(8)) {
        acc += idata[i] + j
        if (acc > 100) {
            idata[i] = acc
            return                      // early return from inside the for-loop
        }
    }
    idata[i] = acc
}

// runtime-bound for + bool local: `urange(0u, n)` with n a loaded SSBO value exercises the ExprCall
// (non-ExprConstRange) for-source path — endpoints come from emitted Loads, not folded constants
// (all Phase-2 ranges were constant). `var flag : bool` adds the bool Function-OpVariable store/load.
[compute_shader(local_size_x=64, name="rfor_spv"), marker(no_coverage)]
def rfor {
    let i = gl_GlobalInvocationID.x
    let n = data[i]                     // runtime upper bound
    var acc = 0u
    for (j in urange(0u, n)) {
        acc += j
    }
    var flag = acc > 50u               // nolint:LINT003 -- must stay `var`: the point is to exercise the bool Function OpVariable Store/Load path (a `let` would be an SSA value, not memory-backed)
    if (flag) {
        acc = 0u
    }
    data[i] = acc
}

def public nloop_words : array<uint> {
    var w : array<uint>
    w |> push_from(nloop_spv)
    return <- w
}

def public echain_words : array<uint> {
    var w : array<uint>
    w |> push_from(echain_spv)
    return <- w
}

def public eret_words : array<uint> {
    var w : array<uint>
    w |> push_from(eret_spv)
    return <- w
}

def public rfor_words : array<uint> {
    var w : array<uint>
    w |> push_from(rfor_spv)
    return <- w
}

// ===== Phase-3 vertex/fragment fixtures: stage I/O (@in/@out + Location), rasterizer builtins
// (gl_Position/gl_VertexIndex/gl_InstanceIndex/gl_FragCoord), vector constructors
// (OpCompositeConstruct), dot (OpDot), and GLSL.std.450 math (OpExtInst). Each emitted blob is
// spirv-val'd; together they add the Phase-3 opcodes to the census. =====

// vertex: vertex-attribute -> clip position + a passthrough varying
var @in @location = 0 a_pos : float2
var @in @location = 1 a_color : float3
var @out @location = 0 v_color : float3
[vertex_shader(name="tri_vert_spv"), marker(no_coverage)]
def tri_vert {
    gl_Position = float4(a_pos, 0.0f, 1.0f)     // CompositeConstruct: vec2 + 2 scalars -> vec4
    v_color = a_color                            // varying passthrough (vec3 in -> vec3 out)
}

// fragment: varying -> framebuffer color (OriginUpperLeft execution mode)
var @in @location = 0 fi_color : float3
var @out @location = 0 frag_color : float4
[fragment_shader(name="tri_frag_spv"), marker(no_coverage)]
def tri_frag {
    frag_color = float4(fi_color, 1.0f)          // CompositeConstruct: vec3 + scalar -> vec4
}

// vertex: gl_VertexIndex / gl_InstanceIndex builtins driving a procedural position
var @out @location = 0 vi_col : float4
[vertex_shader(name="vindex_spv"), marker(no_coverage)]
def vindex {
    var x = 0.0f
    if (gl_VertexIndex == 0) {                    // VertexIndex (int Input builtin) + IEqual
        x = 1.0f
    }
    if (gl_InstanceIndex > 0) {                   // InstanceIndex (int Input builtin) + SGreaterThan
        x = x + 0.5f
    }
    gl_Position = float4(x, 0.0f, 0.0f, 1.0f)     // CompositeConstruct: 4 scalars
    vi_col = float4(x, x, x, 1.0f)
}

// vertex: gl_PointSize (BuiltIn PointSize Output) — the rasteriser uses this when the pipeline's
// topology is POINT_LIST, drawing each point as a screen-space splat of width gl_PointSize pixels.
// Same vertex-shader storage class as gl_Position (Output); writes route to a BuiltIn PointSize
// decorated OpVariable. Requires the largePoints device feature on the Vulkan side for sizes > 1.
[vertex_shader(name="point_size_spv"), marker(no_coverage)]
def point_size {
    gl_Position = float4(float(gl_VertexIndex), 0.0f, 0.0f, 1.0f)
    gl_PointSize = 4.0f                              // BuiltIn PointSize write -> screen-space splat width
}

// vertex: gl_DrawID (BuiltIn DrawIndex Input) — the multi-draw draw-call ordinal: 0 for the first
// vkCmdDraw / vkCmdDrawIndirect entry, 1 for the second, etc. Lets one vertex shader pick per-draw
// data (e.g. a per-draw transform out of an SSBO) without rebinding pipeline state. Triggers the
// DrawParameters capability emission (in core in SPIR-V 1.3).
[vertex_shader(name="draw_id_spv"), marker(no_coverage)]
def draw_id_vs {
    let x = float(gl_DrawID) * 0.1f                  // BuiltIn DrawIndex read -> per-draw offset
    gl_Position = float4(x, 0.0f, 0.0f, 1.0f)
}

// vertex: nonuniformEXT(idx) — VK_EXT_descriptor_indexing marker. Lowers to OpCopyObject (fresh result
// id) + OpDecorate NonUniform on the new id; triggers the OpCapability ShaderNonUniform + the
// OpExtension "SPV_EXT_descriptor_indexing" once-per-module. The descriptor-array indexing path that
// actually consumes the marked value lands in a separate slice; this fixture just exercises the marker
// emission + cap + extension.
[vertex_shader(name="nuext_spv"), marker(no_coverage)]
def nuext_vs {
    let i = nonuniformEXT(gl_VertexIndex)            // identity passthrough -> OpCopyObject + NonUniform decoration
    gl_Position = float4(float(i), 0.0f, 0.0f, 1.0f)
}

// fragment: gl_PointCoord (BuiltIn PointCoord Input) — the UV inside a rasterised point primitive,
// [0,1]^2 with (0.5, 0.5) at the centre of the sprite. Lets a fragment shader compute a circular
// mask + soft falloff for "pretty point sprites" without uploading a texture; valid only when the
// pipeline's topology is POINT_LIST. Storage class Input (read-only), fragment-stage-only.
var @out @location = 0 pc_color : float4
[fragment_shader(name="point_coord_spv"), marker(no_coverage)]
def point_coord {
    pc_color = float4(gl_PointCoord, 0.0f, 1.0f)     // BuiltIn PointCoord read -> sprite-space UV
}

// fragment: gl_FragCoord builtin + GLSL.std.450 math (dot/sqrt/clamp) -> grayscale
var @out @location = 0 m_out : float4
[fragment_shader(name="raster_math_spv"), marker(no_coverage)]
def raster_math {
    let p = gl_FragCoord                          // FragCoord (float4 Input builtin)
    let d = dot(p, p)                             // OpDot -> float
    let s = sqrt(d)                               // OpExtInst Sqrt (imports GLSL.std.450)
    let c = clamp(s, 0.0f, 1.0f)                  // OpExtInst FClamp
    m_out = float4(c, c, c, 1.0f)                 // CompositeConstruct
}

// vertex / fragment Flat-decoration fixtures: integer varyings auto-Flat (Vulkan spec mandate —
// the driver can't interpolate ints, so the emitter MUST decorate them), and an explicit @flat on a
// float varying forces flat-shading (a stylised-look knob). flat_vert/flat_frag cover both: an int
// passthrough (`mat_id`) which should emit Flat without the user asking, and a float passthrough
// (`flat_val`) carrying @flat which should also emit Flat. The plain float `smooth_val` carries no
// @flat and must NOT pick up the decoration — that's the negative control.
var @out @location = 0 mat_id : int                   // int Output -> auto-Flat
var @flat @out @location = 1 flat_val : float          // float Output + @flat -> Flat
var @out @location = 2 smooth_val : float              // float Output, no @flat -> no Flat
[vertex_shader(name="flat_vert_spv"), marker(no_coverage)]
def flat_vert {
    mat_id = gl_VertexIndex
    flat_val = float(gl_VertexIndex) * 0.5f
    smooth_val = float(gl_VertexIndex) * 0.25f
    gl_Position = float4(0.0f, 0.0f, 0.0f, 1.0f)
}

var @in @location = 0 fi_mat_id : int                  // int Input -> auto-Flat
var @flat @in @location = 1 fi_flat_val : float        // float Input + @flat -> Flat
var @in @location = 2 fi_smooth_val : float            // float Input, no @flat -> no Flat
var @out @location = 0 flat_color : float4
[fragment_shader(name="flat_frag_spv"), marker(no_coverage)]
def flat_frag {
    let m = float(fi_mat_id)
    flat_color = float4(m, fi_flat_val, fi_smooth_val, 1.0f)
}

def public tri_vert_words : array<uint> {
    var w : array<uint>
    w |> push_from(tri_vert_spv)
    return <- w
}

def public point_size_words : array<uint> {
    var w : array<uint>
    w |> push_from(point_size_spv)
    return <- w
}

def public draw_id_words : array<uint> {
    var w : array<uint>
    w |> push_from(draw_id_spv)
    return <- w
}

def public nuext_words : array<uint> {
    var w : array<uint>
    w |> push_from(nuext_spv)
    return <- w
}

def public point_coord_words : array<uint> {
    var w : array<uint>
    w |> push_from(point_coord_spv)
    return <- w
}

def public tri_frag_words : array<uint> {
    var w : array<uint>
    w |> push_from(tri_frag_spv)
    return <- w
}

def public vindex_words : array<uint> {
    var w : array<uint>
    w |> push_from(vindex_spv)
    return <- w
}

def public raster_math_words : array<uint> {
    var w : array<uint>
    w |> push_from(raster_math_spv)
    return <- w
}

def public flat_vert_words : array<uint> {
    var w : array<uint>
    w |> push_from(flat_vert_spv)
    return <- w
}

def public flat_frag_words : array<uint> {
    var w : array<uint>
    w |> push_from(flat_frag_spv)
    return <- w
}

// ===== const arrays + dynamic indexing: the gl_VertexIndex hardcoded-triangle vertex shader.
// Exercises OpTypeArray, OpConstantComposite (vec + array constants), a Function-storage array
// OpVariable initialized from the constant, and OpAccessChain indexing by gl_VertexIndex. =====
var @out @location = 0 cv_color : float3
[vertex_shader(name="catri_spv"), marker(no_coverage)]
def catri {
    let positions = fixed_array(float2(0.0, -0.6), float2(0.6, 0.6), float2(-0.6, 0.6))
    let colors = fixed_array(float3(1.0, 0.0, 0.0), float3(0.0, 1.0, 0.0), float3(0.0, 0.0, 1.0))
    gl_Position = float4(positions[gl_VertexIndex], 0.0, 1.0)   // OpAccessChain + CompositeConstruct
    cv_color = colors[gl_VertexIndex]
}

def public catri_words : array<uint> {
    var w : array<uint>
    w |> push_from(catri_spv)
    return <- w
}

// ===== Phase-4a UBO fixture: a @uniform struct read through OpAccessChain by field index. The
// struct's std140 layout exercises both packing edge cases — a scalar packing after a vec2
// (uv@16,flags@24,scale@28) and after a vec3 (dir@32,bias@44). Every field is read; member loads
// feed CompositeConstruct + scalar arithmetic + an int compare. No NEW opcodes vs const-arrays
// (member access is AccessChain/Load over a Block struct) — it broadens the resource surface. =====
struct Uniforms {
    color : float4      // std140 offset 0
    uv    : float2      // offset 16
    flags : int         // offset 24 (vec2 size 8 -> 24; int aligns to 4)
    scale : float       // offset 28
    dir   : float3      // offset 32 (vec3 aligns to 16)
    bias  : float       // offset 44 (packs into the vec3's trailing slot: vec3 size 12 -> 44)
}
var @uniform @binding = 0 ubo : Uniforms
var @out @location = 0 u_out : float4
[fragment_shader(name="ubo_frag_spv"), marker(no_coverage)]
def ubo_frag {
    var a = ubo.scale + ubo.bias            // FAdd: two scalar members (off 28 + 44)
    if (ubo.flags > 0) {                    // int member load + SGreaterThan
        a = a + 1.0f
    }
    let dir4 = float4(ubo.dir, a)           // CompositeConstruct: vec3 member + scalar
    let uv4 = float4(ubo.uv, a, a)          // CompositeConstruct: vec2 member + 2 scalars
    u_out = ubo.color                       // vec4 member load
    if (a > 0.0f) {
        u_out = dir4
    }
    if (a > 1.0f) {
        u_out = uv4
    }
}

def public ubo_frag_words : array<uint> {
    var w : array<uint>
    w |> push_from(ubo_frag_spv)
    return <- w
}

// ===== Phase-4b matrix fixture: a UBO with two float4x4 -> MVP transform. Matrices are tHandle
// types lowered to OpTypeMatrix (4 float4 columns); daslang stores them column-major (m[i] is
// column i) so they map to SPIR-V's default ColMajor (+ MatrixStride 16 in std140). Exercises
// OpMatrixTimesMatrix, OpMatrixTimesVector, OpVectorTimesScalar, and local-value swizzles
// (OpVectorShuffle for .xy, OpCompositeExtract for .w). =====
struct Camera {
    proj : float4x4     // std140 offset 0  (ColMajor, MatrixStride 16, size 64)
    view : float4x4     // std140 offset 64
}
var @uniform @binding = 0 cam : Camera
var @in @location = 0 in_pos : float3
var @out @location = 0 mv_color : float4
[vertex_shader(name="mvp_vert_spv"), marker(no_coverage)]
def mvp_vert {
    let mvp = cam.proj * cam.view           // OpMatrixTimesMatrix
    let world = float4(in_pos, 1.0f)        // OpCompositeConstruct
    let clip = mvp * world                  // OpMatrixTimesVector
    gl_Position = clip * 0.5f               // OpVectorTimesScalar (float4 * float)
    let xy = clip.xy                        // OpVectorShuffle (float2 from a local float4)
    let w = clip.w                          // OpCompositeExtract (float from a local float4)
    mv_color = float4(xy, w, 1.0f)          // OpCompositeConstruct from the swizzles
}

def public mvp_vert_words : array<uint> {
    var w : array<uint>
    w |> push_from(mvp_vert_spv)
    return <- w
}

// ===== Phase-4c push-constant fixture: a @push_constant struct -> PushConstant storage + Block,
// laid out like a UBO (std140 offsets) but with NO DescriptorSet/Binding (push constants are not
// descriptors) and not in the entry interface. Member access is the same OpAccessChain/OpLoad. =====
struct PushData {
    tint  : float4      // offset 0
    gamma : float       // offset 16
}
var @push_constant pc : PushData
var @out @location = 0 pc_out : float4
[fragment_shader(name="pc_frag_spv"), marker(no_coverage)]
def pc_frag {
    pc_out = pc.tint * pc.gamma     // PushConstant member loads -> VectorTimesScalar
}

def public pc_frag_words : array<uint> {
    var w : array<uint>
    w |> push_from(pc_frag_spv)
    return <- w
}

// ===== Phase-4d combined image sampler fixture: a sampler2D global -> OpTypeImage +
// OpTypeSampledImage in UniformConstant storage + DescriptorSet/Binding; texture(tex, uv) ->
// OpLoad the sampled image + OpImageSampleImplicitLod (fragment-only, uses derivatives). =====
var @binding = 0 tex : sampler2D
var @in @location = 0 ti_uv : float2
var @out @location = 0 tex_out : float4
[fragment_shader(name="tex_frag_spv"), marker(no_coverage)]
def tex_frag {
    tex_out = texture(tex, ti_uv)       // OpLoad sampler + OpImageSampleImplicitLod
}

def public tex_frag_words : array<uint> {
    var w : array<uint>
    w |> push_from(tex_frag_spv)
    return <- w
}

// ===== Phase-7.1a scalar conversions: float(int/uint), int(float/uint), uint(float/int) ->
// OpConvertSToF/UToF/FToS/FToU + OpBitcast (int<->uint same-width reinterpret). Reuses the existing
// int/uint/float SSBOs; every conversion opcode is exercised exactly once. =====
[compute_shader(local_size_x=64, name="conv_spv"), marker(no_coverage)]
def conv {
    let i = gl_GlobalInvocationID.x
    let s = idata[i]                    // int
    let u = data[i]                     // uint
    let f = fdata[i]                    // float
    fdata[i] = float(s) + float(u)      // ConvertSToF + ConvertUToF
    idata[i] = int(f) + int(u)          // ConvertFToS + Bitcast (uint -> int)
    data[i] = uint(f) + uint(s)         // ConvertFToU + Bitcast (int -> uint)
}

def public conv_words : array<uint> {
    var w : array<uint>
    w |> push_from(conv_spv)
    return <- w
}

// ===== Phase-7.1b storage image: an image2D global -> UniformConstant OpTypeImage (Rgba8, sampled=2)
// + DescriptorSet/Binding; imageLoad -> OpImageRead, imageStore -> OpImageWrite. A 2D dispatch writes a
// coordinate-derived gradient (the GPU gate proves the coordinate mapping; here it just emits valid SPIR-V). =====
var @binding = 0 img : image2D
[compute_shader(local_size_x=8, local_size_y=8, name="storimg_spv"), marker(no_coverage)]
def storimg {
    let gid = gl_GlobalInvocationID
    let coord = int2(int(gid.x), int(gid.y))
    let prev = imageLoad(img, coord)                                            // OpImageRead
    imageStore(img, coord, float4(float(gid.x), float(gid.y), prev.z, 1.0f))    // OpImageWrite
}

def public storimg_words : array<uint> {
    var w : array<uint>
    w |> push_from(storimg_spv)
    return <- w
}

// ===== Phase-7.2 explicit-LOD + texel fetch: textureLod -> OpImageSampleExplicitLod (Lod operand),
// texelFetch -> OpImage (extract image from the sampled image) + OpImageFetch. Both are derivative-free,
// so this fixture samples in a COMPUTE shader -- proving the stage lift (texture() is fragment-only). =====
var @binding = 1 cs_tex : sampler2D
[compute_shader(local_size_x=64, name="texlod_spv"), marker(no_coverage)]
def texlod {
    let i = gl_GlobalInvocationID.x
    let uv = float2(float(i) / 64.0f, 0.5f)                 // runtime uv (not a folded constant)
    let a = textureLod(cs_tex, uv, 1.0f)                    // OpImageSampleExplicitLod (in compute)
    let b = texelFetch(cs_tex, int2(int(i), 0), 0)          // OpImage + OpImageFetch
    fdata[i] = a.x + b.x                                    // scalar swizzles + FAdd
}

def public texlod_words : array<uint> {
    var w : array<uint>
    w |> push_from(texlod_spv)
    return <- w
}

// ===== Phase-7.3 additional sampler dimensionalities: samplerCube (Dim Cube), sampler3D (Dim 3D),
// sampler2DArray (Dim 2D + Arrayed). All sample via texture() with a float3 coordinate. These add NO
// new opcodes -- only different OpTypeImage Dim/Arrayed operands -- so they must stay within phase7b. =====
var @binding = 0 cube_tex : samplerCube
var @binding = 1 vol_tex : sampler3D
var @binding = 2 arr_tex : sampler2DArray
var @in @location = 0 dims_dir : float3
var @out @location = 0 dims_out : float4
[fragment_shader(name="dims_spv"), marker(no_coverage)]
def dims_frag {
    let c = texture(cube_tex, dims_dir)     // samplerCube -> OpTypeImage Dim Cube
    let v = texture(vol_tex, dims_dir)      // sampler3D   -> OpTypeImage Dim 3D
    let a = texture(arr_tex, dims_dir)      // sampler2DArray -> OpTypeImage Dim 2D, Arrayed 1
    dims_out = float4(c.x, v.y, a.z, 1.0f)  // scalar swizzles + CompositeConstruct
}

def public dims_words : array<uint> {
    var w : array<uint>
    w |> push_from(dims_spv)
    return <- w
}

// ===== Phase-7.4 separate image + sampler: a texture2D (standalone OpTypeImage) and a sampler
// (standalone OpTypeSampler) are two independent descriptors; sampleTexture combines them at the call
// site (OpSampledImage) before sampling. The Vulkan split model, distinct from the combined sampler2D. =====
var @binding = 0 sep_img : texture2D
var @binding = 1 sep_smp : sampler
var @in @location = 0 sep_uv : float2
var @out @location = 0 sep_out : float4
[fragment_shader(name="septex_spv"), marker(no_coverage)]
def septex_frag {
    sep_out = sampleTexture(sep_img, sep_smp, sep_uv)   // OpSampledImage + OpImageSampleImplicitLod
}

def public septex_words : array<uint> {
    var w : array<uint>
    w |> push_from(septex_spv)
    return <- w
}

// ===== Phase-8.1 ternary cond ? a : b -> OpSelect (branchless). A scalar select uses the scalar bool
// condition directly; a vector select with a scalar condition splats the bool to a bvecN first
// (SPIR-V < 1.4 requires the OpSelect condition width to match the result). =====
[compute_shader(local_size_x=64, name="tern_spv"), marker(no_coverage)]
def tern {
    let i = gl_GlobalInvocationID.x
    let a = fdata[i]
    let b = fdata[i + 1u]
    let s = a > b ? a : b                                        // scalar OpSelect // nolint:PERF015
    let v = a > b ? float4(a, a, a, a) : float4(b, b, b, b)      // vector OpSelect (splatted condition)
    fdata[i] = s + v.x
}

def public tern_words : array<uint> {
    var w : array<uint>
    w |> push_from(tern_spv)
    return <- w
}

// ===== Phase-8.2 vector arithmetic + a folded const-vector operand. Component-wise +/-/ lower to
// OpFAdd/OpFSub/OpFDiv on the vector result type (scalar_class reads the lane kind); unary - is
// OpFNegate. float2(0.5,0.5) as an operand folds to ExprConstFloat2 -> OpConstantComposite (the gap
// Phase 7 deferred: a folded const vector used as an rvalue, not only inside a const-array init). =====
var @in @location = 0 va_in : float2
var @out @location = 0 va_out : float2
[fragment_shader(name="vecarith_spv"), marker(no_coverage)]
def vecarith {
    let a = va_in
    let b = va_in + float2(0.5, 0.5)    // folded const-vector operand + component-wise OpFAdd
    let c = a - b                       // component-wise OpFSub
    let d = c / b                       // component-wise OpFDiv
    va_out = -d                         // component-wise OpFNegate
}

def public vecarith_words : array<uint> {
    var w : array<uint>
    w |> push_from(vecarith_spv)
    return <- w
}

// ===== Phase-8.3 GLSL.std.450 math breadth: exercise every reachable ext-inst the emitter maps beyond
// Phase 3's dot/sqrt/clamp. Keyed on the DASLANG name (lerp -> FMix, rsqrt -> InverseSqrt). Vectors are
// built from scalar swizzles (multi-component swizzle of a global is its own gap, closed in Phase 10.2).
// Each ext-inst is spirv-val'd + asserted by sub-opcode. =====
var @in @location = 0 mx_in : float4
var @out @location = 0 mx_out : float4
[fragment_shader(name="mathx_spv"), marker(no_coverage)]
def mathx {
    let x = mx_in.x
    let y = mx_in.y
    let v = float3(x, y, x)
    let w = float3(y, x, y)
    var r = 0.0f
    r += sin(x) + cos(x) + tan(x)
    r += asin(x) + acos(x) + atan(x) + atan2(y, x)
    r += pow(x, y) + exp(x) + log(x) + exp2(x) + log2(x)
    r += floor(x) + ceil(x) + round(x) + trunc(x) + fract(x)
    r += rsqrt(x)
    r += abs(x) + sign(x) + min(x, y) + max(x, y)
    r += lerp(x, y, 0.5f)                       // GLSL mix -> FMix
    r += length(v) + distance(v, w)
    let n = normalize(v)
    let cr = cross(v, w)
    let rf = reflect(v, w)
    let rr = refract(v, w, 0.5f)
    mx_out = float4(r + n.x + cr.x + rf.x + rr.x, 0.0f, 0.0f, 1.0f)
}

def public mathx_words : array<uint> {
    var w : array<uint>
    w |> push_from(mathx_spv)
    return <- w
}

// ===== Phase-10.1 SSBO std430 vector elements: array<float2/3/4> runtime arrays. The std430 ArrayStride
// is the element's base size rounded up to its base alignment: float2 -> 8, float3 -> 16 (size 12, align
// 16), float4 -> 16. Whole-element load/store + component-wise vector add; no new opcodes vs Phase 8. =====
var @ssbo @set = 0 @binding = 0 sv2 : array<float2>
var @ssbo @set = 0 @binding = 1 sv3 : array<float3>
var @ssbo @set = 0 @binding = 2 sv4 : array<float4>
[compute_shader(local_size_x=64, name="ssbovec_spv"), marker(no_coverage)]
def ssbovec {
    let i = gl_GlobalInvocationID.x
    sv2[i] = sv2[i] + float2(1.0, 0.0)          // float2 element: ArrayStride 8
    sv3[i] = sv3[i] + float3(0.0, 1.0, 0.0)     // float3 element: ArrayStride 16 (vec3 size 12, align 16)
    sv4[i] = sv4[i] + float4(0.0, 0.0, 0.0, 1.0) // float4 element: ArrayStride 16
}

// ===== Phase-10.1 SSBO std430 struct elements: array<Particle> runtime array. The element struct is
// laid out std430 (pos@0, vel@8, life@16; max member align 8 -> struct size 24 -> ArrayStride 24) and
// is NOT Block-decorated (only the outer wrapper struct is). Field access particles[i].field chains a
// second OpAccessChain off the element pointer. The compute-particle tutorial's foundation rail. =====
struct Particle {
    pos  : float2   // std430 offset 0
    vel  : float2   // offset 8
    life : float    // offset 16 (struct rounds to align 8 -> stride 24)
}
var @ssbo @binding = 0 particles : array<Particle>
[compute_shader(local_size_x=64, name="ssbostruct_spv"), marker(no_coverage)]
def ssbostruct {
    let i = gl_GlobalInvocationID.x
    let v = particles[i].vel                        // chained AccessChain (element + field) + Load
    particles[i].pos = particles[i].pos + v         // float2 field read + write
    particles[i].life = particles[i].life - 1.0f    // scalar field read + write
}

def public ssbovec_words : array<uint> {
    var w : array<uint>
    w |> push_from(ssbovec_spv)
    return <- w
}

def public ssbostruct_words : array<uint> {
    var w : array<uint>
    w |> push_from(ssbostruct_spv)
    return <- w
}

// ===== Phase-10.2 composite/global swizzle: multi-component swizzle of a bare global (the Phase-8
// out-of-scope note). A single-component global swizzle stays an lvalue (AccessChain); a multi-component
// one loads the whole global and OpVectorShuffle's it. Covers a builtin global (gl_FragCoord.xy), an @in
// global (contiguous .xyz + reordered .yx), single-component global (.w, still the pointer path), and a
// swizzle of a local value (regression). No new opcodes vs Phase 4b (VectorShuffle/CompositeExtract). =====
var @in @location = 0 sw_in : float4
var @out @location = 0 sw_out : float4
[fragment_shader(name="gswizzle_spv"), marker(no_coverage)]
def gswizzle {
    let fc = gl_FragCoord.xy         // multi-component swizzle of a builtin global -> OpVectorShuffle
    let rgb = sw_in.xyz              // multi-component swizzle of an @in global -> OpVectorShuffle
    let yx = sw_in.yx                // reordered multi-component swizzle -> OpVectorShuffle
    let a = sw_in.w                  // single-component global -> AccessChain (pointer), loaded here
    let lx = rgb.xy                  // swizzle of a LOCAL value (regression) -> OpVectorShuffle
    sw_out = float4(rgb, fc.x + yx.x + a + lx.y)
}

def public gswizzle_words : array<uint> {
    var w : array<uint>
    w |> push_from(gswizzle_spv)
    return <- w
}

// ===== Phase-10.3 fragment realism: discard (OpKill), screen-space derivatives (OpDPdx/OpDPdy/
// OpFwidth), textureSize (OpImageQuerySizeLod + the ImageQuery capability), and a gl_FragDepth write
// (BuiltIn FragDepth Output -> the DepthReplacing execution mode). discard/derivatives/gl_FragDepth are
// fragment-only; textureSize is stage-agnostic (exercised here in a fragment shader for convenience).
// discard() sits in an if-branch (it is a block terminator); dFdx/dFdy take the whole float2 global,
// fwidth a scalar. =====
var @binding = 0 fx_tex : sampler2D
var @in @location = 0 fx_uv : float2
var @out @location = 0 fx_color : float4
[fragment_shader(name="fragx_spv"), marker(no_coverage)]
def fragx {
    if (fx_uv.x < 0.0f) {
        discard()                       // OpKill (fragment-only terminator)
    }
    let dx = dFdx(fx_uv)                 // OpDPdx (float2)
    let dy = dFdy(fx_uv)                 // OpDPdy (float2)
    let w = fwidth(fx_uv.x)              // OpFwidth (scalar)
    let sz = textureSize(fx_tex, 0)      // OpImageQuerySizeLod -> int2 (ImageQuery capability)
    gl_FragDepth = fx_uv.x               // BuiltIn FragDepth write -> DepthReplacing execution mode
    fx_color = float4(dx.x + dy.y + w, float(sz.x), float(sz.y), 1.0f)
}

def public fragx_words : array<uint> {
    var w : array<uint>
    w |> push_from(fragx_spv)
    return <- w
}

// ===== Phase-10.4 depth-compare (shadow) sampling: a sampler2DShadow sampled via textureCompare
// (OpImageSampleDrefImplicitLod). The sampler's OpTypeImage carries Depth=1 and the descriptor reflects
// as a comparison_sampler (not a plain combined_image_sampler). The reference value is the fragment's
// light-space depth (a runtime @in); the scalar result is the comparison (1 = lit, 0 = shadowed).
// Implicit LOD -> fragment-only. =====
var @binding = 0 sh_shadow : sampler2DShadow
var @in @location = 0 sh_uv : float2
var @in @location = 1 sh_ref : float
var @out @location = 0 sh_color : float4
[fragment_shader(name="fragshadow_spv"), marker(no_coverage)]
def fragshadow {
    let lit = textureCompare(sh_shadow, sh_uv, sh_ref)   // OpImageSampleDrefImplicitLod -> scalar in [0,1]
    sh_color = float4(lit, lit, lit, 1.0f)
}

def public fragshadow_words : array<uint> {
    var w : array<uint>
    w |> push_from(fragshadow_spv)
    return <- w
}

// ===== MRT: multiple @location outputs in a fragment shader (G-buffer prep for tutorial 10's
// deferred rendering). Each `var @out @location = N` global is its own Output OpVariable with a
// distinct Location decoration; the entry-point interface lists all of them. This locks in the
// per-global Location model so a future MRT-breaking change in spirv_emit.das gets caught here
// instead of in a downstream tutorial. =====
var @in @location = 0 mrt_uv : float2
var @out @location = 0 mrt_position : float4
var @out @location = 1 mrt_normal   : float4
var @out @location = 2 mrt_albedo   : float4
[fragment_shader(name="mrt_spv"), marker(no_coverage)]
def mrt {
    mrt_position = float4(mrt_uv.x, mrt_uv.y, 0.0f, 1.0f)   // OpStore -> mrt_position (location 0)
    mrt_normal   = float4(0.0f, 1.0f, 0.0f, 0.0f)            // OpStore -> mrt_normal   (location 1)
    mrt_albedo   = float4(0.8f, 0.2f, 0.4f, 1.0f)            // OpStore -> mrt_albedo   (location 2)
}

def public mrt_words : array<uint> {
    var w : array<uint>
    w |> push_from(mrt_spv)
    return <- w
}

// ===== Subpass input attachment: a Vulkan deferred-rendering rail (tutorial 10 lighting subpass).
// Reads a previous subpass's color/depth at the SAME fragment coordinate via subpassLoad. The
// opaque type subpassInput lowers to OpTypeImage Dim=SubpassData (Sampled=2, no format) decorated
// with InputAttachmentIndex (from @input_attachment_index); subpassLoad emits OpImageRead with an
// internal int2(0,0) coord. Requires the InputAttachment capability. =====
var @set = 0 @binding = 0 @input_attachment_index = 0 sp_color : subpassInput
var @out @location = 0 sp_out : float4
[fragment_shader(name="subpass_spv"), marker(no_coverage)]
def subpass_load_frag {
    let sample = subpassLoad(sp_color)         // OpImageRead -> read color from prev subpass
    sp_out = float4(sample.x, sample.y, sample.z, 1.0f)
}

def public subpass_words : array<uint> {
    var w : array<uint>
    w |> push_from(subpass_spv)
    return <- w
}

// ===== Phase-10.5 compute-tier fixtures: shared memory + barriers + atomics =====
// cshared: a workgroup-shared scratch array (`@workgroup`, GLSL `shared` -> Workgroup OpVariable),
// the control + shared-memory sync (barrier() -> OpControlBarrier, memoryBarrierShared() ->
// OpMemoryBarrier), and an atomicAdd onto an SSBO element (OpAtomicIAdd at Device scope, since the
// target lives in buffer memory). The canonical tiled-reduction skeleton.
var @ssbo @binding = 0 rin : array<uint>
var @ssbo @binding = 1 rout : array<uint>
var @workgroup rtile : uint[64]

[compute_shader(local_size_x=64, name="cshared_spv"), marker(no_coverage)]
def cshared {
    let lid = gl_LocalInvocationID.x
    let gid = gl_GlobalInvocationID.x
    rtile[lid] = rin[gid]       // store into shared memory (Workgroup OpAccessChain + OpStore)
    barrier()                   // OpControlBarrier (execution + shared-memory rendezvous)
    memoryBarrierShared()       // OpMemoryBarrier (shared-memory ordering)
    let _o = atomicAdd(rout[0], rtile[lid])   // OpAtomicIAdd onto an SSBO element (Device scope)
}

// catomics: the rest of the atomic family on @workgroup int + uint scalar counters (Workgroup
// scope). Both signednesses are present so atomicMin/Max emit BOTH the signed (SMin/SMax) and
// unsigned (UMin/UMax) opcodes; atomicCompSwap is the OpAtomicCompareExchange CAS. The results feed
// an SSBO write so they stay live.
var @workgroup acnt_i : int
var @workgroup acnt_u : uint
var @ssbo @binding = 0 aout : array<int>

[compute_shader(local_size_x=64, name="catomics_spv"), marker(no_coverage)]
def catomics {
    let gid = gl_GlobalInvocationID.x
    let v = int(gid)
    let u = gid
    let a = atomicMin(acnt_i, v)            // OpAtomicSMin
    let b = atomicMax(acnt_i, v)            // OpAtomicSMax
    let c = atomicMin(acnt_u, u)            // OpAtomicUMin
    let d = atomicMax(acnt_u, u)            // OpAtomicUMax
    let e = atomicAnd(acnt_i, v)            // OpAtomicAnd
    let f = atomicOr(acnt_i, v)             // OpAtomicOr
    let g = atomicXor(acnt_i, v)            // OpAtomicXor
    let h = atomicExchange(acnt_i, v)       // OpAtomicExchange
    let kk = atomicCompSwap(acnt_i, 0, v)   // OpAtomicCompareExchange
    aout[gid] = a + b + e + f + g + h + kk + int(c) + int(d)
}

def public cshared_words : array<uint> {
    var w : array<uint>
    w |> push_from(cshared_spv)
    return <- w
}

def public catomics_words : array<uint> {
    var w : array<uint>
    w |> push_from(catomics_spv)
    return <- w
}

// ===== Phase-10.6 storage-image size query + module-scope shader constants =====
// imgquery: imageSize(img) -> OpImageQuerySize (the storage image's dimensions in texels, NO LOD --
// a storage image is Sampled=2 so the no-LOD query is legal, unlike textureSize which needs a LOD).
// Pulls in the ImageQuery capability. Lets a compute shader size its pixel<->coordinate mapping off
// the bound image instead of a hardcoded extent (the Mandelbrot-tutorial rail).
var @binding = 0 qimg : image2D
[compute_shader(local_size_x=8, local_size_y=8, name="imgquery_spv"), marker(no_coverage)]
def imgquery {
    let gid = gl_GlobalInvocationID
    let dim = imageSize(qimg)                            // OpImageQuerySize -> int2
    let coord = int2(int(gid.x), int(gid.y))
    let u = float(gid.x) / float(dim.x)                  // the queried size feeds the mapping
    let v = float(gid.y) / float(dim.y)
    imageStore(qimg, coord, float4(u, v, 0.0f, 1.0f))
}

// modconst: module-scope shader constants. An immutable `let X = <const-expr>` at module scope folds to
// an OpConstant and references resolve straight to it (no OpVariable / OpLoad) -- so a shader can hoist
// iteration counts, view rectangles, etc. to module scope and share them with the host. Scalar (float +
// int) and a folded const-vector module const are all exercised. Counterpart fail-closed case (a mutable
// `var` global must still be rejected) lives in test_fail_closed.das.
let MC_LO = -2.0f
let MC_HI = 1.0f
let MC_ITER = 16
let MC_BASE = float2(0.5, 0.5)
[compute_shader(local_size_x=64, name="modconst_spv"), marker(no_coverage)]
def modconst {
    let i = gl_GlobalInvocationID.x
    var acc = MC_LO + float(MC_ITER)        // module-scope float + int consts as rvalues
    for (_k in range(MC_ITER)) {            // int const as a loop bound (counter unused)
        acc += MC_HI
    }
    let b = MC_BASE                          // folded const-vector module const
    fdata[i] = acc + b.x
}

def public imgquery_words : array<uint> {
    var w : array<uint>
    w |> push_from(imgquery_spv)
    return <- w
}

def public modconst_words : array<uint> {
    var w : array<uint>
    w |> push_from(modconst_spv)
    return <- w
}

// ===== Phase-11 user-defined shader functions: OpFunction / OpFunctionCall + ref-param copy-in/out =====
// Each user function called (transitively) from a shader is emitted as its own OpFunction. A value
// parameter binds an SSA OpFunctionParameter; a by-reference parameter binds a Function pointer and the
// caller wraps the argument in a copy-in / copy-out temp (Logical addressing forbids passing an
// access-chain pointer straight to a call). The only NEW opcodes are OpFunctionCall / OpFunctionParameter
// / OpReturnValue; everything else reuses already-declared opcodes.

def private uf_add1(x : int) : int {                // scalar value param, scalar return
    return x + 1
}
def private uf_muladd(a, b, c : int) : int {        // multiple value params
    return a * b + c
}
def private uf_double(x : int) : int {              // transitive: calls the shared uf_add1 twice
    return uf_add1(uf_add1(x))
}
def private uf_grade(uv : float2) : float3 {        // vector value param + vector return
    return float3(uv.x, uv.y, uv.x + uv.y)
}

// userfn: value-param surface. uf_add1 is shared (called directly AND inside uf_double) -> ONE OpFunction
// but several OpFunctionCall; uf_grade is the vector-in/vector-out showcase.
[compute_shader(local_size_x=64, name="userfn_spv"), marker(no_coverage)]
def userfn {
    let i = int(gl_GlobalInvocationID.x)
    let a = uf_add1(idata[i])                       // scalar value call
    let b = uf_muladd(a, 2, 3)                      // multi-arg value call
    let c = uf_double(b)                            // transitive (uf_double -> uf_add1 x2)
    let g = uf_grade(float2(float(a), float(b)))    // vector param + vector return
    idata[i] = a + b + c + int(g.x + g.y + g.z)
}

def private uf_accumulate(var acc : float&; x : float) {   // by-reference param (read-modify-write), void
    acc = acc + x * x
}
def private uf_scale_inplace(var v : float&; s : float) {  // a second ref helper (mixed with a value arg)
    v = v * s
}

// userfn_ref: by-reference parameter copy-in/copy-out. `acc` is a local var; each call copies its current
// value into a Function temp, runs the helper through the temp pointer, then copies the temp back into acc.
[compute_shader(local_size_x=64, name="userfn_ref_spv"), marker(no_coverage)]
def userfn_ref {
    let i = int(gl_GlobalInvocationID.x)
    var acc = 0.0f
    uf_accumulate(acc, fdata[i])    // ref arg + value arg
    uf_accumulate(acc, 1.0f)        // shared ref helper, second call site
    uf_scale_inplace(acc, 0.5f)     // a different ref helper
    fdata[i] = acc
}

def private uf_lane_seed() : uint {     // a no-arg helper that READS a builtin global (gl_GlobalInvocationID)
    return gl_GlobalInvocationID.x * 747796405u + 2891336453u
}

// userfn_global: a helper with no parameters that reads a module global directly (the global is collected
// transitively through the helper and classified once for the whole module).
[compute_shader(local_size_x=64, name="userfn_global_spv"), marker(no_coverage)]
def userfn_global {
    let i = gl_GlobalInvocationID.x
    data[i] = uf_lane_seed()
}

def public userfn_words : array<uint> {
    var w : array<uint>
    w |> push_from(userfn_spv)
    return <- w
}

def public userfn_ref_words : array<uint> {
    var w : array<uint>
    w |> push_from(userfn_ref_spv)
    return <- w
}

def public userfn_global_words : array<uint> {
    var w : array<uint>
    w |> push_from(userfn_global_spv)
    return <- w
}

// ===== spirv-val gate =====
struct SpirvValResult {
    ran : bool      // false => tool not found (soft-skip locally; CI provides it)
    ok  : bool
    msg : string
}

def private tool_runs(exe : string) : bool {
    var out : string
    return run_and_capture([exe, "--version"], out, 10.0) == 0
}

def private find_spirv_val : string {
    var cands : array<string>
    if (has_env_variable("VULKAN_SDK")) {
        let sdk = get_env_variable("VULKAN_SDK")
        cands |> push("{sdk}/Bin/spirv-val")           // Windows SDK layout
        cands |> push("{sdk}/Bin/spirv-val.exe")
        cands |> push("{sdk}/bin/spirv-val")           // Linux SDK layout
    }
    cands |> push("spirv-val")                          // PATH (CI)
    cands |> push("spirv-val.exe")
    var found = ""
    for (p in cands) {
        if (tool_runs(p)) {
            found = p
            break
        }
    }
    delete cands
    return found
}

def public validate_spirv(words : array<uint>; target_env : string = "vulkan1.1") : SpirvValResult {
    let exe = find_spirv_val()
    if (empty(exe)) return SpirvValResult(ran = false, ok = false, msg = "spirv-val not found")
    let tf = create_temp_file_result("das_spirv", ".spv")
    if (tf is error) return SpirvValResult(ran = true, ok = false, msg = "temp file: {tf as error}")
    let path = tf as value
    var wrote = false
    fopen(path, "wb") $(f) {
        if (f != null) {
            fwrite(f, words)
            wrote = true
        }
    }
    if (!wrote) {
        remove_result(path)
        return SpirvValResult(ran = true, ok = false, msg = "write failed: {path}")
    }
    var out : string
    let rc = run_and_capture([exe, "--target-env", target_env, path], out, 30.0)
    remove_result(path)
    return SpirvValResult(ran = true, ok = rc == 0, msg = out)
}

// ===== opcode census: the EXACT set generate_spirv emits for the Phase-1 square =====
// Gate B (stronger than LCOV): a meta-test asserts the emitted set equals this declared set in
// BOTH directions — catches an opcode emitted but never asserted, and a declared opcode that
// silently stopped being emitted.
def public phase1_emitter_opcodes : table<uint> {
    var s : table<uint>
    for (op in [
        SpvOp.Capability, SpvOp.MemoryModel, SpvOp.EntryPoint, SpvOp.ExecutionMode,
        SpvOp.Decorate, SpvOp.MemberDecorate,
        SpvOp.TypeInt, SpvOp.TypeVector, SpvOp.TypeVoid, SpvOp.TypeFunction,
        SpvOp.TypeRuntimeArray, SpvOp.TypeStruct, SpvOp.TypePointer,
        SpvOp.Constant, SpvOp.Variable,
        SpvOp.Function, SpvOp.Label, SpvOp.AccessChain, SpvOp.Load, SpvOp.IMul, SpvOp.Store,
        SpvOp.Return, SpvOp.FunctionEnd
    ]) {
        s |> insert(uint(op))
    }
    return <- s
}

// The EXACT set generate_spirv emits across ALL Phase-2 fixtures (square + arith + control + loops).
// The census meta-test unions opcode_set over every fixture and asserts equality with this set in
// BOTH directions — so a newly-emitted-but-unasserted opcode, or a declared opcode that stopped
// firing, fails the gate. Phase-1 set + the Phase-2 additions:
def public phase2_emitter_opcodes : table<uint> {
    var s <- phase1_emitter_opcodes()
    for (op in [
        // types newly needed by Phase 2
        SpvOp.TypeBool, SpvOp.TypeFloat,
        // arithmetic breadth
        SpvOp.IAdd, SpvOp.ISub, SpvOp.UDiv, SpvOp.UMod, SpvOp.SDiv, SpvOp.SRem, SpvOp.SNegate, SpvOp.Not,
        SpvOp.FAdd, SpvOp.FSub, SpvOp.FMul, SpvOp.FDiv, SpvOp.FRem, SpvOp.FNegate,
        // comparisons (only the flavors the fixtures actually exercise)
        SpvOp.IEqual, SpvOp.INotEqual, SpvOp.UGreaterThan, SpvOp.ULessThan,
        SpvOp.SGreaterThan, SpvOp.SLessThan, SpvOp.SGreaterThanEqual, SpvOp.SLessThanEqual,
        SpvOp.FOrdEqual, SpvOp.FOrdNotEqual, SpvOp.FOrdGreaterThan, SpvOp.FOrdLessThan,
        SpvOp.FOrdGreaterThanEqual, SpvOp.FOrdLessThanEqual,
        // logical
        SpvOp.LogicalAnd, SpvOp.LogicalOr, SpvOp.LogicalNot,
        // structured control flow
        SpvOp.SelectionMerge, SpvOp.BranchConditional, SpvOp.Branch, SpvOp.LoopMerge
    ]) {
        s |> insert(uint(op))
    }
    return <- s
}

// Phase-3 adds vertex/fragment: stage I/O + rasterizer builtins reuse Decorate/Variable/EntryPoint/
// ExecutionMode (already declared), so the only NEW opcodes are vector construction, dot, and the
// GLSL.std.450 ext-inst pair. Phase-2 set + these:
def public phase3_emitter_opcodes : table<uint> {
    var s <- phase2_emitter_opcodes()
    for (op in [
        SpvOp.CompositeConstruct,           // vector constructors float4(...)/float3(...)
        SpvOp.Dot,                          // dot(a, b)
        SpvOp.ExtInstImport, SpvOp.ExtInst  // GLSL.std.450 math (sqrt/clamp/...)
    ]) {
        s |> insert(uint(op))
    }
    return <- s
}

// Const arrays + dynamic indexing add OpTypeArray + OpConstantComposite (the array/vector constants).
// Phase-3 set + these:
def public const_array_emitter_opcodes : table<uint> {
    var s <- phase3_emitter_opcodes()
    for (op in [SpvOp.TypeArray, SpvOp.ConstantComposite]) {
        s |> insert(uint(op))
    }
    return <- s
}

// Phase-4a UBOs add NO new opcodes: @uniform member access is OpAccessChain/OpLoad over a Block-
// decorated OpTypeStruct, all already in the set (Uniform storage + DescriptorSet/Binding are
// operands, not opcodes). The declared set is unchanged — the UBO fixture must still stay within it.
def public phase4a_emitter_opcodes : table<uint> {
    return <- const_array_emitter_opcodes()
}

// Phase-4b adds matrices + matrix/vector arithmetic + local-value swizzles. Phase-4a set + these:
def public phase4b_emitter_opcodes : table<uint> {
    var s <- phase4a_emitter_opcodes()
    for (op in [
        SpvOp.TypeMatrix,                                   // float4x4 -> OpTypeMatrix
        SpvOp.MatrixTimesMatrix, SpvOp.MatrixTimesVector,   // proj*view, mvp*world
        SpvOp.VectorTimesScalar,                            // clip * 0.5
        SpvOp.VectorShuffle, SpvOp.CompositeExtract         // clip.xy / clip.w (local swizzle)
    ]) {
        s |> insert(uint(op))
    }
    return <- s
}

// Phase-4c push constants add NO new opcodes: a @push_constant block is the same Block-decorated
// OpTypeStruct read via OpAccessChain/OpLoad — only the storage class (PushConstant) differs, and
// that is an operand, not an opcode. The declared set is unchanged.
def public phase4c_emitter_opcodes : table<uint> {
    return <- phase4b_emitter_opcodes()
}

// Phase-4d combined image samplers add the image type pair + the sample op. Phase-4c set + these:
def public phase4d_emitter_opcodes : table<uint> {
    var s <- phase4c_emitter_opcodes()
    for (op in [
        SpvOp.TypeImage, SpvOp.TypeSampledImage,    // sampler2D -> OpTypeImage + OpTypeSampledImage
        SpvOp.ImageSampleImplicitLod                // texture(tex, uv)
    ]) {
        s |> insert(uint(op))
    }
    return <- s
}

// Phase-7.1 adds scalar conversions + storage images. Phase-4d set + these:
def public phase7a_emitter_opcodes : table<uint> {
    var s <- phase4d_emitter_opcodes()
    for (op in [
        // scalar conversions
        SpvOp.ConvertSToF, SpvOp.ConvertUToF, SpvOp.ConvertFToS, SpvOp.ConvertFToU, SpvOp.Bitcast,
        // storage image read/write (OpTypeImage already declared by Phase-4d samplers)
        SpvOp.ImageRead, SpvOp.ImageWrite
    ]) {
        s |> insert(uint(op))
    }
    return <- s
}

// Phase-7.2 adds explicit-LOD sampling + texel fetch. Phase-7.1 set + these:
def public phase7b_emitter_opcodes : table<uint> {
    var s <- phase7a_emitter_opcodes()
    for (op in [
        SpvOp.ImageSampleExplicitLod,   // textureLod(s, uv, lod)
        SpvOp.Image, SpvOp.ImageFetch   // texelFetch(s, coord, lod)
    ]) {
        s |> insert(uint(op))
    }
    return <- s
}

// Phase-7.3 (sampler3D / samplerCube / sampler2DArray) adds NO new opcodes -- only different
// OpTypeImage Dim/Arrayed operands. The declared set is unchanged; the dims fixture must stay within it.
def public phase7c_emitter_opcodes : table<uint> {
    return <- phase7b_emitter_opcodes()
}

// Phase-7.4 separate image + sampler adds the standalone sampler type + the call-site combine. 7.3 + these:
def public phase7d_emitter_opcodes : table<uint> {
    var s <- phase7c_emitter_opcodes()
    for (op in [
        SpvOp.TypeSampler,      // a standalone sampler global
        SpvOp.SampledImage      // sampleTexture combines image + sampler at the call
    ]) {
        s |> insert(uint(op))
    }
    return <- s
}

// Phase-8.1 adds the ternary -> OpSelect (the bvecN splat reuses OpCompositeConstruct, already declared
// by Phase 3). Phase-8.2 (vector arithmetic + folded const-vector operands) and Phase-8.3 (broader
// GLSL.std.450 math, all via the already-declared OpExtInst) add NO new opcodes. So the Phase-8 declared
// set is 7.4 + OpSelect. 7.4 set + this:
def public phase8_emitter_opcodes : table<uint> {
    var s <- phase7d_emitter_opcodes()
    s |> insert(uint(SpvOp.Select))     // cond ? a : b
    return <- s
}

// Phase-10.1 SSBO std430 vector/struct elements add NO new opcodes: a vector element is the same
// OpTypeRuntimeArray (different ArrayStride operand); a struct element is an extra OpTypeStruct
// (non-Block) read via a chained OpAccessChain/OpLoad — all already declared. The set is unchanged;
// the new fixtures must still stay within it.
def public phase10_1_emitter_opcodes : table<uint> {
    return <- phase8_emitter_opcodes()
}

// Phase-10.2 multi-component global swizzle adds NO new opcodes: it routes a bare global through the
// existing load + OpVectorShuffle / OpCompositeExtract path (both declared by Phase 4b). The set is
// unchanged; the gswizzle fixture must still stay within it.
def public phase10_2_emitter_opcodes : table<uint> {
    return <- phase10_1_emitter_opcodes()
}

// Phase-10.3 fragment realism adds five opcodes: OpKill (discard), OpDPdx/OpDPdy/OpFwidth (derivatives),
// and OpImageQuerySizeLod (textureSize). gl_FragDepth reuses the existing builtin Variable/Decorate path
// (DepthReplacing is an ExecutionMode operand, not an opcode), and OpImage was declared by Phase-7.2
// texelFetch. Phase-10.2 set + these:
def public phase10_3_emitter_opcodes : table<uint> {
    var s <- phase10_2_emitter_opcodes()
    for (op in [
        SpvOp.Kill,                         // discard()
        SpvOp.DPdx, SpvOp.DPdy, SpvOp.Fwidth,   // dFdx/dFdy/fwidth
        SpvOp.ImageQuerySizeLod             // textureSize(s, lod)
    ]) {
        s |> insert(uint(op))
    }
    return <- s
}

// Phase-10.4 depth-compare sampling adds one opcode: OpImageSampleDrefImplicitLod (textureCompare on a
// sampler2DShadow). The Depth=1 image flag is an OpTypeImage operand and the comparison_sampler kind is
// reflection-only — neither is a new opcode. Phase-10.3 set + this:
def public phase10_4_emitter_opcodes : table<uint> {
    var s <- phase10_3_emitter_opcodes()
    s |> insert(uint(SpvOp.ImageSampleDrefImplicitLod))
    return <- s
}

// Phase-10.5 compute tier adds twelve opcodes: the two barriers (OpControlBarrier / OpMemoryBarrier)
// and the atomic family (IAdd, signed+unsigned Min/Max, And/Or/Xor, Exchange, CompareExchange). The
// @workgroup shared global reuses the existing OpVariable / OpTypeArray / OpAccessChain path (a
// Workgroup storage class, not a new opcode). Phase-10.4 set + these:
def public phase10_5_emitter_opcodes : table<uint> {
    var s <- phase10_4_emitter_opcodes()
    for (op in [
        SpvOp.ControlBarrier, SpvOp.MemoryBarrier,                  // barrier() / memoryBarrierShared()
        SpvOp.AtomicIAdd,                                           // atomicAdd
        SpvOp.AtomicSMin, SpvOp.AtomicUMin,                         // atomicMin (int / uint)
        SpvOp.AtomicSMax, SpvOp.AtomicUMax,                         // atomicMax (int / uint)
        SpvOp.AtomicAnd, SpvOp.AtomicOr, SpvOp.AtomicXor,           // atomicAnd / atomicOr / atomicXor
        SpvOp.AtomicExchange, SpvOp.AtomicCompareExchange           // atomicExchange / atomicCompSwap
    ]) {
        s |> insert(uint(op))
    }
    return <- s
}

// Phase-10.6 adds the storage-image size query (imageSize -> OpImageQuerySize, the no-LOD sibling of
// Phase-10.3's OpImageQuerySizeLod). Module-scope shader constants add NO new opcodes: a folded `let`
// global resolves straight to its already-declared OpConstant/OpConstantComposite. Phase-10.5 set + this:
def public phase10_6_emitter_opcodes : table<uint> {
    var s <- phase10_5_emitter_opcodes()
    s |> insert(uint(SpvOp.ImageQuerySize))     // imageSize(img)
    return <- s
}

// Phase-11 user-defined shader functions add three opcodes: OpFunctionCall (the call), OpFunctionParameter
// (each parameter of an emitted helper), and OpReturnValue (a value return; void helpers keep OpReturn,
// already declared). OpFunction / OpLabel / OpFunctionEnd were declared back in Phase 1. Phase-10.6 + these:
def public phase11_fn_emitter_opcodes : table<uint> {
    var s <- phase10_6_emitter_opcodes()
    for (op in [SpvOp.FunctionCall, SpvOp.FunctionParameter, SpvOp.ReturnValue]) {
        s |> insert(uint(op))
    }
    return <- s
}

// PR-B (bindless prereq: nonuniformEXT + NonUniform decoration) adds two opcodes: OpExtension (the
// SPV_EXT_descriptor_indexing declaration; the first non-cap module-prologue extension we emit) and
// OpCopyObject (the fresh-id alias the marker decorates so NonUniform doesn't leak onto every other
// consumer of the same input SSA value). NonUniform itself rides on the existing OpDecorate.
def public bindless_nu_emitter_opcodes : table<uint> {
    var s <- phase11_fn_emitter_opcodes()
    for (op in [SpvOp.Extension, SpvOp.CopyObject]) {
        s |> insert(uint(op))
    }
    return <- s
}
