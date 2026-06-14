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

def public tri_vert_words : array<uint> {
    var w : array<uint>
    w |> push_from(tri_vert_spv)
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
