options gen2
options indenting = 4

// The rejection gate: a kernel class the emitter must refuse is compiled at run time from source
// text held here (a file on disk would fail the lint lane by design), and the refusal's text is
// the assertion - a rejection that silently stopped firing would otherwise emit a kernel whose
// device reads differ from the Vulkan home's.

require dastest/testing_boost public
require daslib/rtti
require daslib/ast
require strings

//! compiles `text` as the virtual module file `name`; "" when it compiled clean, the issue text otherwise
def private compile_issues(name, text : string) : string {
    var out = ""
    var inscope access <- make_file_access("")
    set_file_source(access, name, text)
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            compile_file(name, access, unsafe(addr(mg)), cop) $(ok; _program; issues) {
                out = ok ? "" : string(issues)
            }
        }
    }
    return out
}

//! one refusal: the fixture module `name`, its source `text`, the text the refusal must carry
struct private RejectArm {
    name : string
    needle : string
    text : string
}

//! a module of `decls` and a kernel class `K` of `members` whose kernel method runs `body`; `ann` extends the annotation
def private class_module(name, decls, members, ann, body : string) : string =>
    ("options gen2\nmodule {name} shared public\nrequire cuda/cuda_kernel public\nrequire cuda/cuda_intrinsics public\nrequire math\n"
        + "{decls}\nclass K \{\n{members}\n    [cuda_kernel(name=\"{name}_cuda\"{ann})]\n    def run \{\n{body}\n    \}\n\}\n")

//! the default class: one float buffer `o`, a block of 64
def private body_arm(name, needle, body : string) : RejectArm =>
    RejectArm(name = name, needle = needle, text = class_module(name, "", "    @ssbo @binding = 0 o : array<float>", ", local_size_x = 64", body))

//! the default class after module-level `decls`
def private decl_arm(name, needle, decls, body : string) : RejectArm =>
    RejectArm(name = name, needle = needle, text = class_module(name, decls, "    @ssbo @binding = 0 o : array<float>", ", local_size_x = 64", body))

//! a class of its own `members`
def private member_arm(name, needle, decls, members, body : string) : RejectArm =>
    RejectArm(name = name, needle = needle, text = class_module(name, decls, members, ", local_size_x = 64", body))

//! the tile fixtures' module scope: a block struct, a combine, a per-element and two decode callbacks
let TILE_DECLS = "struct DecBlk \{
    q : int16[4]
\}
def cm2_max(a, b : float) : float => max(a, b)
def cm2_bias(r, c : uint; v : float) : float => v + float(r) - float(c)
def cm2_pick(r, c : uint; v, x : float) : float => max(v, x) + float(r + c)
def dec_half(b : DecBlk; bc, cib : uint2) : float16 => float16(float(int(b.q[int(cib.y)]) + int(bc.x)))
def dec_int(b : DecBlk; bc, cib : uint2) : int => int(b.q[int(cib.y)]) + int(bc.x)
"

//! the tile fixtures' buffers: f16, f32, int and block planes
let TILE_MEMBERS = "    @ssbo @binding = 0 h : array<float16>
    @ssbo @binding = 1 o : array<float>
    @ssbo @binding = 2 q : array<int>
    @ssbo @binding = 3 blk : array<DecBlk>"

//! the tile fixtures' first statements: a 64x64 layout `tl` and a transposed view `tv`
let TILE_SETUP = "        var tl : tensorLayout2D
        tensorLayoutCreate(tl)
        tensorLayoutSetDimension(tl, 64u, 64u)
        tensorLayoutSetStride(tl, 64u, 1u)
        var tv : tensorView2Dt
        tensorViewCreate(tv)
"

//! a four-warp kernel over the tile buffers, `decls` after the shared ones, `body` after the layout and the view
def private tile_arm(name, needle, decls, body : string) : RejectArm =>
    RejectArm(name = name, needle = needle, text = class_module(name, TILE_DECLS + decls, TILE_MEMBERS, ", local_size_x = 128", TILE_SETUP + body))

def private check_arms(t : T?; arms : array<RejectArm>) {
    for (arm in arms) {
        let issues = compile_issues("{arm.name}.das", arm.text)
        t |> success(!empty(issues), "{arm.name}: the fixture does not compile")
        t |> success(find(issues, arm.needle) >= 0, "{arm.name}: the refusal reads `{arm.needle}`: {issues}")
    }
}

let VEC3_FIELD = "options gen2
module _reject_vec3_field shared public
require cuda/cuda_kernel public
struct Vec3Blk \{
    n : float3
    w : float
\}
class Vec3Blocks \{
    @ssbo @binding = 0 blk : array<Vec3Blk>
    @ssbo @binding = 1 o : array<float>
    [cuda_kernel(name=\"vec3_blocks_cuda\", local_size_x = 64)]
    def run \{
        let i = gl_GlobalInvocationID.x
        o[i] = blk[i].w
    \}
\}
"

let RET_VALUE = "options gen2
module _reject_ret_value shared public
require cuda/cuda_kernel public
class K \{
    @ssbo @binding = 0 o : array<float>
    [cuda_kernel(name=\"ret_value_cuda\", local_size_x = 64)]
    def run : int \{
        o[gl_GlobalInvocationID.x] = 1.0
        return 1
    \}
\}
"

let WITH_PARAMS = "options gen2
module _reject_with_params shared public
require cuda/cuda_kernel public
class K \{
    @ssbo @binding = 0 o : array<float>
    [cuda_kernel(name=\"with_params_cuda\", local_size_x = 64)]
    def run(x : int) \{
        o[gl_GlobalInvocationID.x] = float(x)
    \}
\}
"

let FREE_KERNEL = "options gen2
module _reject_free_kernel shared public
require cuda/cuda_kernel public
[cuda_kernel(name=\"free_kernel_cuda\", local_size_x = 64)]
def free_kernel \{
    let i = gl_GlobalInvocationID.x
\}
"

[test]
def test_cuda_reject_kernel_shape(t : T?) {
    t |> run("a 3-lane struct field, a returned value, a parameter, a free function") <| @(t : T?) {
        check_arms(t, [
            RejectArm(name = "_reject_vec3_field", needle = "field `n` is a 3-lane vector", text = VEC3_FIELD),
            RejectArm(name = "_reject_ret_value", needle = "kernels return void", text = RET_VALUE),
            RejectArm(name = "_reject_with_params", needle = "must take no parameters beyond self", text = WITH_PARAMS),
            RejectArm(name = "_reject_free_kernel", needle = "is not a class method", text = FREE_KERNEL)
        ])
    }
}

[test]
def test_cuda_reject_body(t : T?) {
    t |> run("a statement or an expression outside the CUDA subset names its construct") <| @(t : T?) {
        check_arms(t, [
            body_arm("_reject_two_iters", "only a single `for (i in range(...))` loop",
                "        for (i, j in range(4), range(4)) \{\n            o[i * 4 + j] = 0.0\n        \}"),
            body_arm("_reject_for_source", "must be int or uint (range/urange source)",
                "        for (x in o) \{\n            o[0] = x\n        \}"),
            body_arm("_reject_var_table", "it must be a `let`",
                "        var tbl = fixed_array<float>(1.0, 2.0)\n        tbl[0] = 3.0\n        o[gl_GlobalInvocationID.x] = tbl[1]"),
            body_arm("_reject_call_table", "every element must be an integer or float scalar literal",
                "        let tbl : float[4] = fixed_array<float>(o[0], o[1], o[2], o[3])\n        o[gl_GlobalInvocationID.x] = tbl[1]"),
            body_arm("_reject_inf_literal", "non-finite float literal has no CUDA form",
                "        o[gl_GlobalInvocationID.x] = 1.0 / 0.0"),
            body_arm("_reject_addr_local", "addr() in a kernel must take a kernel-class member element",
                "        var arr : float[4]\n        var p = unsafe(addr(arr[0]))\n        *p = 1.0\n        o[gl_GlobalInvocationID.x] = arr[0]"),
            body_arm("_reject_op_double", "operator `*` result type double const has no CUDA form",
                "        o[gl_GlobalInvocationID.x] = float(double(o[0]) * 2.0lf)"),
            body_arm("_reject_cast_expr", "expression ExprCast is not in the supported CUDA subset",
                "        o[gl_GlobalInvocationID.x] = unsafe(reinterpret<float>(gl_GlobalInvocationID.x))"),
            body_arm("_reject_math_double", "min lowers for numeric classes only",
                "        o[gl_GlobalInvocationID.x] = float(min(double(o[0]), 2.0lf))"),
            body_arm("_reject_wait_var", "cp_async_wait(pending) takes a constant",
                "        cp_async_wait(int(gl_GlobalInvocationID.x))\n        o[0] = 1.0"),
            body_arm("_reject_vec_ctor", "takes 4 scalars, one scalar (splat) or one 4-lane vector",
                "        o[gl_GlobalInvocationID.x] = float4(float2(o[0], o[1]), float2(o[2], o[3])).x"),
            body_arm("_reject_unknown_call", "call `length` is not a supported CUDA builtin",
                "        o[gl_GlobalInvocationID.x] = float(length(o))"),
            body_arm("_reject_table_elem", "local array `tbl` element type double has no CUDA form",
                "        let tbl = fixed_array<double>(1.0lf, 2.0lf)\n        o[gl_GlobalInvocationID.x] = float(tbl[0])"),
            body_arm("_reject_table_copy", "its initializer must be a `fixed_array<T>(...)` of exactly 4 elements",
                "        var src : float[4]\n        src[1] = o[0]\n        let tbl = src\n        o[gl_GlobalInvocationID.x] = tbl[1]"),
            body_arm("_reject_array_elem", "local array `arr` element type double has no CUDA form",
                "        var arr : double[4]\n        o[gl_GlobalInvocationID.x] = float(arr[0])"),
            body_arm("_reject_local_type", "local `d` type double has no CUDA form",
                "        var d = double(o[0])\n        o[gl_GlobalInvocationID.x] = float(d)"),
            body_arm("_reject_call_stmt", "call `clock32` as a statement has no CUDA lowering",
                "        clock32()\n        o[gl_GlobalInvocationID.x] = 1.0"),
            body_arm("_reject_assert_stmt", "statement ExprAssert is not in the supported CUDA subset",
                "        assert(gl_GlobalInvocationID.x < 100u)\n        o[gl_GlobalInvocationID.x] = 1.0"),
            body_arm("_reject_loop_hint", "loop hint `vectorize` has no CUDA lowering",
                "        for [vectorize] (j in range(4)) \{\n            o[j] = 0.0\n        \}"),
            body_arm("_reject_for_iterator", "for-loop source must be range(...) or urange(...)",
                "        for (j in count(0, 1)) \{\n            o[j] = 0.0\n            break\n        \}"),
            body_arm("_reject_subgroup_vec", "subgroupAdd: the operand is a float, int or uint scalar, not float4",
                "        o[gl_GlobalInvocationID.x] = subgroupAdd(float4(o[0])).x"),
            body_arm("_reject_swizzle_range", "swizzle over range const has no CUDA form",
                "        o[gl_GlobalInvocationID.x] = float(range(1, int(gl_GlobalInvocationID.x)).yx.x)"),
            body_arm("_reject_op2_stmt", "operator `+` as a statement is not an assignment",
                "        o[0] + o[1]\n        o[gl_GlobalInvocationID.x] = 1.0"),
            body_arm("_reject_op1_stmt", "unary `-` is not a statement",
                "        -o[0]\n        o[gl_GlobalInvocationID.x] = 1.0"),
            body_arm("_reject_rotate", "operator `<<<` is not in the supported CUDA set",
                "        o[gl_GlobalInvocationID.x] = float(gl_GlobalInvocationID.x <<< 1u)")
        ])
    }
}

[test]
def test_cuda_reject_callee(t : T?) {
    t |> run("a module global, a user operator, an overload of a lowered math, atomic, intrinsic or pack name") <| @(t : T?) {
        check_arms(t, [
            decl_arm("_reject_global_var", "global `g_scale` is not a supported builtin",
                "var g_scale = 2.0",
                "        o[gl_GlobalInvocationID.x] = g_scale"),
            decl_arm("_reject_vec_order", "ordered comparison `<` on vectors",
                "def operator <(a, b : float4) : bool => a.x < b.x",
                "        o[gl_GlobalInvocationID.x] = float4(o[0]) < float4(o[1]) ? 1.0 : 0.0"),
            decl_arm("_reject_math_arity", "exp takes exactly 1 argument",
                "def exp(a, b : float) : float => a + b",
                "        o[gl_GlobalInvocationID.x] = exp(o[0], o[1])"),
            decl_arm("_reject_math_half", "abs on the half family has no CUDA form",
                "def abs(x : float16) : float16 => x",
                "        o[gl_GlobalInvocationID.x] = float(abs(float16(o[0])))"),
            decl_arm("_reject_atomic_arity", "atomicAdd takes exactly 2 arguments",
                "def atomicAdd(var mem : float&) : float => mem",
                "        atomicAdd(o[0])"),
            decl_arm("_reject_atomic_float", "atomicMin lowers on int/uint memory",
                "def atomicMin(var mem : float&; v : float) : float => mem + v",
                "        atomicMin(o[0], 1.0)"),
            decl_arm("_reject_bar_sync_arity", "bar_sync(id, count)",
                "def bar_sync(id : uint) \{\n    let k = id\n\}",
                "        bar_sync(1u)\n        o[0] = 1.0"),
            decl_arm("_reject_byte_perm_arity", "byte_perm(a, b, sel)",
                "def byte_perm(a, b : uint) : uint => a ^ b",
                "        o[gl_GlobalInvocationID.x] = float(byte_perm(1u, 2u))"),
            decl_arm("_reject_asm_operands", "cuda_asm(fmt [, a0 [, a1 [, a2]]]) takes a constant PTX string",
                "def cuda_asm(fmt : string; a0, a1, a2, a3 : uint) \{\n    let k = a0 + a1 + a2 + a3\n\}",
                "        cuda_asm(\"nop;\", 1u, 2u, 3u, 4u)\n        o[0] = 1.0"),
            decl_arm("_reject_bitcast_arity", "float_bits_to_uint takes exactly one argument",
                "def float_bits_to_uint(a, b : float) : uint => uint(a + b)",
                "        o[gl_GlobalInvocationID.x] = float(float_bits_to_uint(o[0], o[1]))"),
            decl_arm("_reject_pack_half_arity", "packHalf2x16 takes exactly one argument",
                "def packHalf2x16(a, b : float) : uint => uint(a + b)",
                "        o[gl_GlobalInvocationID.x] = float(packHalf2x16(o[0], o[1]))"),
            decl_arm("_reject_pack32_arity", "pack32 takes exactly one argument",
                "def pack32(a, b : int) : int => a + b",
                "        o[gl_GlobalInvocationID.x] = float(pack32(1, 2))"),
            decl_arm("_reject_unpack8_type", "unpack8 lowers between a 32/16-bit integer and its byte lanes",
                "def unpack8(x : double) : int2 => int2(int(x))",
                "        o[gl_GlobalInvocationID.x] = float(unpack8(double(o[0])).x)"),
            decl_arm("_reject_sdot4_arity", "sdot4 takes exactly two arguments",
                "def sdot4(a : uint) : int => int(a)",
                "        o[gl_GlobalInvocationID.x] = float(sdot4(1u))")
        ])
    }
}

[test]
def test_cuda_reject_helpers(t : T?) {
    t |> run("an overload of a subgroup, ctor, barrier or range name; a helper the body calls; self as a value") <| @(t : T?) {
        check_arms(t, [
            decl_arm("_reject_subgroup_add_arity", "subgroupAdd takes exactly one argument",
                "def subgroupAdd(a, b : float) : float => a + b",
                "        o[gl_GlobalInvocationID.x] = subgroupAdd(o[0], o[1])"),
            decl_arm("_reject_shuffle_arity", "subgroupShuffle takes exactly two arguments",
                "def subgroupShuffle(a : float) : float => a",
                "        o[gl_GlobalInvocationID.x] = subgroupShuffle(o[0])"),
            decl_arm("_reject_broadcast_first_arity", "subgroupBroadcastFirst takes exactly one argument",
                "def subgroupBroadcastFirst(a, b : float) : float => a + b",
                "        o[gl_GlobalInvocationID.x] = subgroupBroadcastFirst(o[0], o[1])"),
            decl_arm("_reject_subgroup_any_arity", "subgroupAny takes exactly one argument",
                "def subgroupAny(a, b : bool) : bool => a || b",
                "        o[gl_GlobalInvocationID.x] = subgroupAny(o[0] > 0.0, o[1] > 0.0) ? 1.0 : 0.0"),
            decl_arm("_reject_clustered_arity", "subgroupClusteredAdd takes exactly two arguments",
                "def subgroupClusteredAdd(a : float) : float => a",
                "        o[gl_GlobalInvocationID.x] = subgroupClusteredAdd(o[0])"),
            decl_arm("_reject_scalar_ctor_arity", "half takes exactly one argument",
                "def half(a, b : float) : float16 => float16(a + b)",
                "        o[gl_GlobalInvocationID.x] = float(half(o[0], o[1]))"),
            decl_arm("_reject_invoke_fn", "invoke is not a self method call",
                "def twice(x : float) : float => x * 2.0",
                "        o[gl_GlobalInvocationID.x] = invoke(@@twice, o[0])"),
            decl_arm("_reject_barrier_arity", "barrier() takes no arguments",
                "def barrier(x : int) \{\n    let k = x\n\}",
                "        barrier(1)\n        o[0] = 1.0"),
            decl_arm("_reject_range_step", "range with a step is not supported",
                "def range(a, b, c : int) : range => range(a, b + c)",
                "        for (j in range(0, 8, 2)) \{\n            o[j] = 0.0\n        \}"),
            decl_arm("_reject_param_elem", "parameter `a` element type has no CUDA form",
                "def first(var a : double[2]) : float => float(a[0])",
                "        var arr : double[2]\n        o[gl_GlobalInvocationID.x] = first(arr)"),
            decl_arm("_reject_param_type", "parameter `d` type has no CUDA form",
                "def twice_d(d : double) : float => float(d) * 2.0",
                "        o[gl_GlobalInvocationID.x] = twice_d(double(o[0]))"),
            decl_arm("_reject_return_type", "return type of `one_d` has no CUDA form",
                "def one_d : double => double(1.0)",
                "        o[gl_GlobalInvocationID.x] = float(one_d())"),
            decl_arm("_reject_recursive_fn", "function `rec` is recursive",
                "def rec(n : int) : int => n > 0 ? rec(n - 1) : 0",
                "        o[gl_GlobalInvocationID.x] = float(rec(3))"),
            decl_arm("_reject_self_value", "`self` used as a value",
                "def touch(k : K) : float => 1.0",
                "        o[gl_GlobalInvocationID.x] = touch(self)")
        ])
    }
}

[test]
def test_cuda_reject_class(t : T?) {
    t |> run("a member or an element struct the class carries") <| @(t : T?) {
        check_arms(t, [
            member_arm("_reject_empty_struct", "struct `Empty` has no fields - an opaque Vulkan type",
                "struct Empty \{\n\}",
                "    @ssbo @binding = 0 o : array<float>\n    @ssbo @binding = 1 e : array<Empty>",
                "        o[gl_GlobalInvocationID.x] = 1.0"),
            member_arm("_reject_bool_field", "struct `BoolBlk` field `b` type bool has no CUDA form",
                "struct BoolBlk \{\n    b : bool\n    f : float\n\}",
                "    @ssbo @binding = 0 o : array<float>\n    @ssbo @binding = 1 bb : array<BoolBlk>",
                "        o[gl_GlobalInvocationID.x] = bb[0].f"),
            member_arm("_reject_tail_pad", "struct `TailPad`: its CUDA layout (8 bytes, align 4) is not the std430 layout",
                "struct TailPad \{\n    f : float\n    b : int8\n\}",
                "    @ssbo @binding = 0 o : array<float>\n    @ssbo @binding = 1 tp : array<TailPad>",
                "        o[gl_GlobalInvocationID.x] = tp[0].f"),
            member_arm("_reject_plain_member", "member `x` is neither @ssbo, @kargs nor @workgroup",
                "",
                "    @ssbo @binding = 0 o : array<float>\n    x : int",
                "        o[gl_GlobalInvocationID.x] = 1.0"),
            member_arm("_reject_mixed_member", "member `w` mixes @ssbo/@kargs/@workgroup",
                "",
                "    @ssbo @binding = 0 o : array<float>\n    @ssbo @workgroup @binding = 1 w : array<float>",
                "        o[gl_GlobalInvocationID.x] = 1.0"),
            member_arm("_reject_wg_2d", "@workgroup member `w` must be a scalar, vector, or single-dimension fixed array",
                "",
                "    @ssbo @binding = 0 o : array<float>\n    @workgroup w : float[4][4]",
                "        o[gl_GlobalInvocationID.x] = 1.0"),
            member_arm("_reject_kargs_scalar", "@kargs member `ka` must be a struct of 4-byte scalars",
                "",
                "    @ssbo @binding = 0 o : array<float>\n    @kargs ka : int",
                "        o[gl_GlobalInvocationID.x] = 1.0"),
            member_arm("_reject_two_kargs", "are both @kargs - a kernel takes one args struct",
                "struct TwoArgs \{\n    n : uint\n\}",
                "    @ssbo @binding = 0 o : array<float>\n    @kargs ka : TwoArgs\n    @kargs kb : TwoArgs",
                "        o[gl_GlobalInvocationID.x] = 1.0"),
            member_arm("_reject_kargs_field", "@kargs struct `VecArgs` field `v` is float2 - only int/uint/float fields lower",
                "struct VecArgs \{\n    v : float2\n\}",
                "    @ssbo @binding = 0 o : array<float>\n    @kargs ka : VecArgs",
                "        o[gl_GlobalInvocationID.x] = 1.0"),
            member_arm("_reject_kargs_empty", "@kargs struct `NoArgs` has no fields",
                "struct NoArgs \{\n\}",
                "    @ssbo @binding = 0 o : array<float>\n    @kargs ka : NoArgs",
                "        o[gl_GlobalInvocationID.x] = 1.0"),
            member_arm("_reject_no_binding", "@ssbo member `q` has no @binding",
                "",
                "    @ssbo @binding = 0 o : array<float>\n    @ssbo q : array<float>",
                "        o[gl_GlobalInvocationID.x] = 1.0"),
            member_arm("_reject_ssbo_scalar", "@ssbo member `q` must be array<T>",
                "",
                "    @ssbo @binding = 0 o : array<float>\n    @ssbo @binding = 1 q : float",
                "        o[gl_GlobalInvocationID.x] = 1.0"),
            member_arm("_reject_ssbo_elem", "@ssbo member `q` element type double has no CUDA form",
                "",
                "    @ssbo @binding = 0 o : array<float>\n    @ssbo @binding = 1 q : array<double>",
                "        o[gl_GlobalInvocationID.x] = 1.0"),
            member_arm("_reject_cpu_only_width", "is CPU-only (8/16-lane lattice forms do not exist in shaders",
                "",
                "    @ssbo @binding = 0 o : array<float>\n    @ssbo @binding = 1 q : array<byte8>",
                "        o[gl_GlobalInvocationID.x] = 1.0"),
            member_arm("_reject_no_ssbo", "kernel class `K` has no @ssbo members",
                "",
                "    @workgroup w : float[4]",
                "        w[0] = 1.0")
        ])
    }
}

[test]
def test_cuda_reject_methods(t : T?) {
    t |> run("a written args member, a method, a pointer's field, a tile with no block, an intrinsic's address spaces") <| @(t : T?) {
        check_arms(t, [
            member_arm("_reject_kargs_written", "@kargs member `ka` is written",
                "struct WrArgs \{\n    n : uint\n\}",
                "    @ssbo @binding = 0 o : array<float>\n    @kargs ka : WrArgs",
                "        ka.n = 1u\n        o[gl_GlobalInvocationID.x] = 1.0"),
            member_arm("_reject_abstract_method", "method `helper` of `K` has no resolvable body",
                "",
                "    @ssbo @binding = 0 o : array<float>\n    def abstract helper : float",
                "        o[gl_GlobalInvocationID.x] = helper()"),
            member_arm("_reject_recursive_method", "method `rec` is recursive",
                "",
                "    @ssbo @binding = 0 o : array<float>\n    def rec(n : int) : int => n > 0 ? rec(n - 1) : 0",
                "        o[gl_GlobalInvocationID.x] = float(rec(3))"),
            member_arm("_reject_ptr_field", "field access `.w` is not a kernel-class member access",
                "struct PtrBlk \{\n    w : float\n\}",
                "    @ssbo @binding = 0 o : array<float>\n    @ssbo @binding = 1 pb : array<PtrBlk>",
                "        let p = unsafe(addr(pb[0]))\n        o[gl_GlobalInvocationID.x] = p.w"),
            RejectArm(name = "_reject_wg_no_block", needle = "a workgroup-scope coopmat tile needs the kernel's block",
                text = class_module("_reject_wg_no_block", "", "    @ssbo @binding = 0 o : array<float>", ", local_size_x = 0",
                    "        var acc : coopmatWgAcc_f32_64x64\n        o[gl_GlobalInvocationID.x] = 1.0")),
            member_arm("_reject_cp_async_global", "cp_async_16(dst, src) copies 16 bytes from a buffer pointer",
                "",
                "    @ssbo @binding = 0 q4 : array<uint4>",
                "        cp_async_16(unsafe(addr(q4[0])), unsafe(addr(q4[1])))"),
            member_arm("_reject_ldmatrix_global", "ldmatrix(fragment, p) loads a KHR A or B fragment off a @workgroup array pointer",
                "",
                "    @ssbo @binding = 0 q4 : array<uint4>\n    @ssbo @binding = 1 o : array<float>",
                "        var fa : coopmatA_f16_16x16\n        ldmatrix(fa, unsafe(addr(q4[0])))\n        o[0] = 1.0")
        ])
    }
}

[test]
def test_cuda_reject_khr_tiles(t : T?) {
    t |> run("a KHR tile form: an arity, an operand type, a layout") <| @(t : T?) {
        check_arms(t, [
            tile_arm("_reject_khr_convert_arity", "coopmatConvert expects (dst, src)",
                "def coopmatConvert(var d : coopmatAcc_f32_16x16; s : coopmatAcc_f16_16x16; k : int) \{\n    let z = k\n\}",
                "        var x : coopmatAcc_f32_16x16\n        var y : coopmatAcc_f16_16x16\n        coopmatConvert(x, y, 1)"),
            tile_arm("_reject_khr_convert_pair", "coopmatConvert lowers the f16 accumulator to the f32 accumulator",
                "",
                "        var x : coopmatAcc_f16_16x16\n        var y : coopmatAcc_f32_16x16\n        coopmatConvert(x, y)"),
            tile_arm("_reject_khr_load_arity", "coopmatLoad expects (dst, src, idx, stride, layout)",
                "def coopmatLoad(var d : coopmatA_f16_16x16; src : array<float16>; idx : uint) \{\n    let z = idx\n\}",
                "        var a : coopmatA_f16_16x16\n        coopmatLoad(a, h, 0u)"),
            tile_arm("_reject_khr_load_acc", "coopmatLoad: the first argument must be a KHR A or B tile local",
                "def coopmatLoad(var d : coopmatAcc_f32_16x16; src : array<float>; idx : uint; stride, layout : int) \{\n    let z = idx\n\}",
                "        var c : coopmatAcc_f32_16x16\n        coopmatLoad(c, o, 0u, 16, 0)"),
            tile_arm("_reject_khr_load_scalar", "coopmatLoad: the buffer argument must be an @ssbo array",
                "",
                "        var a : coopmatA_f16_16x16\n        var s = 1.0\n        coopmatLoad(a, s, 0u, 16, 0)"),
            tile_arm("_reject_khr_load_layout", "coopmatLoad: layout must be a constant 0 (row-major) or 1 (column-major)",
                "",
                "        var a : coopmatA_f16_16x16\n        coopmatLoad(a, h, 0u, 16, 2)"),
            tile_arm("_reject_khr_mul_add_arity", "coopmatMulAdd expects (a, b, c)",
                "def coopmatMulAdd(a : coopmatA_f16_16x16; b : coopmatB_f16_16x16) : coopmatAcc_f32_16x16 => coopmatAcc_f32_16x16()",
                "        var a : coopmatA_f16_16x16\n        var b : coopmatB_f16_16x16\n        var c : coopmatAcc_f32_16x16\n        c = coopmatMulAdd(a, b)"),
            tile_arm("_reject_khr_mul_add_triple", "is not a KHR (A, B, accumulator) triple the tensor cores serve",
                "def coopmatMulAdd(a : coopmatA_f16_16x16; b : coopmatB_f16_16x16; c : coopmatAcc_s32_16x16) : coopmatAcc_s32_16x16 => c",
                "        var a : coopmatA_f16_16x16\n        var b : coopmatB_f16_16x16\n        var c : coopmatAcc_s32_16x16\n        c = coopmatMulAdd(a, b, c)"),
            tile_arm("_reject_mma_scale_acc", "mma_scale_acc(f32 accumulator, s32 accumulator, row scales, column scales)",
                "def mma_scale_acc(var f : coopmatAcc_f32_16x16; s : coopmatAcc_f32_16x16; rs : float2; cs : float4) \{\n    let z = rs.x + cs.x\n\}",
                "        var f : coopmatAcc_f32_16x16\n        var s : coopmatAcc_f32_16x16\n        mma_scale_acc(f, s, float2(1.0), float4(1.0))"),
            tile_arm("_reject_e2m1_load", "an e2m1 fragment loads through ldmatrix alone",
                "def coopmatLoad(var d : coopmatA_e2m1_16x64; src : array<float16>; idx : uint; stride, layout : int) \{\n    let z = idx\n\}",
                "        var a : coopmatA_e2m1_16x64\n        coopmatLoad(a, h, 0u, 16, 0)"),
            tile_arm("_reject_mma_block_scaled_frags", "mma_block_scaled(f32 accumulator, e2m1 A fragment, e2m1 B fragment",
                "def mma_block_scaled(var f : coopmatAcc_f32_16x16; a : coopmatA_f16_16x16; b : coopmatB_f16_16x16; sa, sb0, sb1 : uint) \{\n    let z = sa + sb0 + sb1\n\}",
                "        var f : coopmatAcc_f32_16x16\n        var a : coopmatA_f16_16x16\n        var b : coopmatB_f16_16x16\n        mma_block_scaled(f, a, b, 1u, 1u, 1u)"),
            tile_arm("_reject_mma_block_scaled_mx_frags", "mma_block_scaled_mx(f32 accumulator, e2m1 A fragment, e2m1 B fragment",
                "def mma_block_scaled_mx(var f : coopmatAcc_f32_16x16; a : coopmatA_f16_16x16; b : coopmatB_f16_16x16; sa, sb0, sb1 : uint) \{\n    let z = sa + sb0 + sb1\n\}",
                "        var f : coopmatAcc_f32_16x16\n        var a : coopmatA_f16_16x16\n        var b : coopmatB_f16_16x16\n        mma_block_scaled_mx(f, a, b, 1u, 1u, 1u)"),
            tile_arm("_reject_mma_scale_cols_acc", "mma_scale_cols(f32 accumulator, column scales)",
                "def mma_scale_cols(var f : coopmatAcc_s32_16x16; cs : float4) \{\n    let z = cs.x\n\}",
                "        var f : coopmatAcc_s32_16x16\n        mma_scale_cols(f, float4(1.0))"),
            tile_arm("_reject_cvt_e2m1x2_arity", "cvt_e2m1x2(lo, hi)",
                "def cvt_e2m1x2(lo : float) : uint => 0u",
                "        o[0] = float(cvt_e2m1x2(1.0))"),
            tile_arm("_reject_cvt_ue4m3_arity", "cvt_ue4m3(x)",
                "def cvt_ue4m3(x, y : float) : uint => 0u",
                "        o[0] = float(cvt_ue4m3(1.0, 2.0))")
        ])
    }
}

[test]
def test_cuda_reject_tensor(t : T?) {
    t |> run("a tensor layout or view, a tensor load or store") <| @(t : T?) {
        check_arms(t, [
            tile_arm("_reject_layout_no_arg", "tensorLayoutCreate takes the layout or view local first",
                "def tensorLayoutCreate \{\n    let z = 1\n\}",
                "        tensorLayoutCreate()"),
            tile_arm("_reject_layout_float", "tensorLayoutCreate takes a tensorLayout2D / tensorLayout2DPad local",
                "",
                "        var f = 0.0\n        tensorLayoutCreate(f)"),
            tile_arm("_reject_view_float", "tensorViewCreate takes a tensorView2D / tensorView2Dt local",
                "",
                "        var f = 0.0\n        tensorViewCreate(f)"),
            tile_arm("_reject_layout_dim_float", "tensorLayoutSetDimension takes (layout, v0, v1)",
                "",
                "        var f = 0.0\n        tensorLayoutSetDimension(f, 1u, 2u)"),
            tile_arm("_reject_load_tensor_arity", "coopmatLoadTensor expects (dst, src, base, layout, r0, nr, c0, nc [, view])",
                "def coopmatLoadTensor(var d : coopmatWgA_f16_64x64; src : array<float16>; base : uint; var l : tensorLayout2D; r0, nr, c0 : uint) \{\n    let z = base\n\}",
                "        var at : coopmatWgA_f16_64x64\n        coopmatLoadTensor(at, h, 0u, tl, 0u, 64u, 0u)"),
            tile_arm("_reject_load_tensor_float", "coopmatLoadTensor: the first argument must be a workgroup-scope coopmatWg* local",
                "",
                "        var f = 0.0\n        coopmatLoadTensor(f, h, 0u, tl, 0u, 64u, 0u, 64u)"),
            tile_arm("_reject_load_tensor_both_views", "is loaded under a plain view here and under the transposed view elsewhere",
                "",
                "        var bt : coopmatWgB_f16_64x64\n        coopmatLoadTensor(bt, h, 0u, tl, 0u, 64u, 0u, 64u, tv)\n        coopmatLoadTensor(bt, h, 0u, tl, 0u, 64u, 0u, 64u)"),
            tile_arm("_reject_store_a_tile", "coopmatStoreTensor stores an accumulator tile; an A or B tile is read-only",
                "",
                "        var at : coopmatWgA_f16_64x64\n        coopmatLoadTensor(at, h, 0u, tl, 0u, 64u, 0u, 64u)\n        coopmatStoreTensor(at, h, 0u, tl, 0u, 64u, 0u, 64u)"),
            tile_arm("_reject_load_tensor_scalar", "coopmatLoadTensor: the buffer must be an @ssbo array, a @workgroup array or a pointer",
                "",
                "        var at : coopmatWgA_f16_64x64\n        var f = 0.0\n        coopmatLoadTensor(at, f, 0u, tl, 0u, 64u, 0u, 64u)"),
            tile_arm("_reject_load_tensor_layout", "coopmatLoadTensor: the fourth argument must be a tensorLayout2D / tensorLayout2DPad local",
                "",
                "        var at : coopmatWgA_f16_64x64\n        var f = 0.0\n        coopmatLoadTensor(at, h, 0u, f, 0u, 64u, 0u, 64u)"),
            tile_arm("_reject_load_tensor_view", "coopmatLoadTensor: the trailing argument must be a tensorView2D / tensorView2Dt local",
                "",
                "        var at : coopmatWgA_f16_64x64\n        var f = 0.0\n        coopmatLoadTensor(at, h, 0u, tl, 0u, 64u, 0u, 64u, f)"),
            tile_arm("_reject_load_tensor_conv", "coopmatLoadTensor: a buffer of int has no conversion to the tile's f16 component",
                "",
                "        var at : coopmatWgA_f16_64x64\n        coopmatLoadTensor(at, q, 0u, tl, 0u, 64u, 0u, 64u)")
        ])
    }
}

[test]
def test_cuda_reject_wg_forms(t : T?) {
    t |> run("a workgroup tile convert or element-wise form") <| @(t : T?) {
        check_arms(t, [
            tile_arm("_reject_wg_convert_shape", "changes the tile shape",
                "",
                "        var a : coopmatWgAcc_f32_64x64\n        var b : coopmatWgAcc_f16_64x32\n        coopmatConvert(a, b)"),
            tile_arm("_reject_wg_convert_s32", "has no CUDA form (the accumulator widths, or an accumulator into an f16 A tile)",
                "",
                "        var a : coopmatWgAcc_s32_64x64\n        var b : coopmatWgAcc_f32_64x64\n        coopmatConvert(a, b)"),
            tile_arm("_reject_fill_float", "coopmatFill: the first argument must be a workgroup-scope coopmatWg* local",
                "",
                "        var f = 0.0\n        coopmatFill(f, 1.0)"),
            tile_arm("_reject_fill_arity", "coopmatFill expects (mat, v)",
                "def coopmatFill(var m : coopmatWgAcc_f32_64x64) \{\n    let z = 1\n\}",
                "        var acc : coopmatWgAcc_f32_64x64\n        coopmatFill(acc)"),
            tile_arm("_reject_scale_a_tile", "coopmatScale walks an accumulator tile; an A or B tile is read-only",
                "",
                "        var at : coopmatWgA_f16_64x64\n        coopmatScale(at, 2.0)"),
            tile_arm("_reject_scale_arity", "coopmatScale expects (mat, s)",
                "def coopmatScale(var m : coopmatWgAcc_f32_64x64) \{\n    let z = 1\n\}",
                "        var acc : coopmatWgAcc_f32_64x64\n        coopmatScale(acc)"),
            tile_arm("_reject_clamp_arity", "coopmatClamp expects (mat, lo, hi)",
                "def coopmatClamp(var m : coopmatWgAcc_f32_64x64; lo : float) \{\n    let z = lo\n\}",
                "        var acc : coopmatWgAcc_f32_64x64\n        coopmatClamp(acc, 1.0)"),
            tile_arm("_reject_copy_arity", "coopmatCopy expects (dst, src)",
                "def coopmatCopy(var m : coopmatWgAcc_f32_64x64) \{\n    let z = 1\n\}",
                "        var acc : coopmatWgAcc_f32_64x64\n        coopmatCopy(acc)"),
            tile_arm("_reject_copy_types", "coopmatCopy: dst and src must be one tile type",
                "",
                "        var a : coopmatWgAcc_f32_64x64\n        var b : coopmatWgAcc_f32_64x32\n        coopmatCopy(a, b)"),
            tile_arm("_reject_add_arity", "coopmatAdd expects (dst, a, b)",
                "def coopmatAdd(var d : coopmatWgAcc_f32_64x64; a : coopmatWgAcc_f32_64x64) \{\n    let z = 1\n\}",
                "        var acc : coopmatWgAcc_f32_64x64\n        coopmatAdd(acc, acc)"),
            tile_arm("_reject_add_types", "coopmatAdd: all three arguments must be one accumulator tile type",
                "",
                "        var a : coopmatWgAcc_f32_64x64\n        var b : coopmatWgAcc_f32_64x32\n        coopmatAdd(a, a, b)")
        ])
    }
}

[test]
def test_cuda_reject_wg_walks(t : T?) {
    t |> run("a workgroup per-element walk, reduce, product or scaled accumulate") <| @(t : T?) {
        check_arms(t, [
            tile_arm("_reject_per_element_arity", "coopmatPerElement expects (dst, src, @@fn, ...up to 4 extras)",
                "def coopmatPerElement(var d : coopmatWgAcc_f32_64x64; s : coopmatWgAcc_f32_64x64) \{\n    let z = 1\n\}",
                "        var acc : coopmatWgAcc_f32_64x64\n        coopmatPerElement(acc, acc)"),
            tile_arm("_reject_per_element_types", "coopmatPerElement: dst and src must be one accumulator tile type",
                "",
                "        var a : coopmatWgAcc_f32_64x64\n        var b : coopmatWgAcc_f32_64x32\n        coopmatPerElement(a, b, @@cm2_bias)"),
            tile_arm("_reject_per_element_fn", "coopmatPerElement: the third argument must be @@<per-element function>",
                "",
                "        var a : coopmatWgAcc_f32_64x64\n        coopmatPerElement(a, a, 1.0)"),
            tile_arm("_reject_per_element_extra", "coopmatPerElement: coopmat extra 1 must match the tile type",
                "",
                "        var a : coopmatWgAcc_f32_64x64\n        var b : coopmatWgAcc_f32_64x32\n        coopmatPerElement(a, a, @@cm2_pick, b)"),
            tile_arm("_reject_reduce_arity", "coopmatReduce expects (dst, src, mask, @@combine)",
                "def coopmatReduce(var d : coopmatWgAcc_f32_64x64; s : coopmatWgAcc_f32_64x64; m : uint) \{\n    let z = m\n\}",
                "        var acc : coopmatWgAcc_f32_64x64\n        coopmatReduce(acc, acc, 1u)"),
            tile_arm("_reject_reduce_comp", "coopmatReduce: dst and src must be accumulator tiles of one component",
                "",
                "        var a : coopmatWgAcc_f32_64x64\n        var b : coopmatWgAcc_f16_64x64\n        coopmatReduce(a, b, COOPMAT_REDUCE_ROW, @@cm2_max)"),
            tile_arm("_reject_reduce_mask", "coopmatReduce: mask must be the constant COOPMAT_REDUCE_ROW or COOPMAT_REDUCE_COLUMN",
                "",
                "        var a : coopmatWgAcc_f32_64x64\n        coopmatReduce(a, a, 3u, @@cm2_max)"),
            tile_arm("_reject_reduce_rows", "coopmatReduce: a row reduce keeps src's row count",
                "",
                "        var a : coopmatWgAcc_f32_128x32\n        var b : coopmatWgAcc_f32_64x32\n        coopmatReduce(a, b, COOPMAT_REDUCE_ROW, @@cm2_max)"),
            tile_arm("_reject_reduce_fn", "coopmatReduce: the last argument must be @@<combine function>",
                "",
                "        var a : coopmatWgAcc_f32_64x64\n        coopmatReduce(a, a, COOPMAT_REDUCE_ROW, 1.0)"),
            tile_arm("_reject_wg_mul_add_triple", "is not a workgroup (A MxK, B KxN, accumulator MxN) triple",
                "def coopmatMulAdd(a : coopmatWgA_f16_64x64; b : coopmatWgB_f16_64x64; c : coopmatWgAcc_f32_64x32) : coopmatWgAcc_f32_64x32 => c",
                "        var at : coopmatWgA_f16_64x64\n        var bt : coopmatWgB_f16_64x64\n        var c : coopmatWgAcc_f32_64x32\n        c = coopmatMulAdd(at, bt, c)"),
            tile_arm("_reject_scale_acc_arity", "coopmatScaleAcc expects (facc, sacc, rs, rbase, cs, cbase)",
                "def coopmatScaleAcc(var f : coopmatWgAcc_f32_64x64; s : coopmatWgAcc_s32_64x64) \{\n    let z = 1\n\}",
                "        var f : coopmatWgAcc_f32_64x64\n        var s : coopmatWgAcc_s32_64x64\n        coopmatScaleAcc(f, s)"),
            tile_arm("_reject_scale_acc_types", "coopmatScaleAcc: an f32 accumulator tile and the s32 accumulator of its shape",
                "",
                "        var f : coopmatWgAcc_f32_64x64\n        var s : coopmatWgAcc_f32_64x64\n        coopmatScaleAcc(f, s, o, 0u, o, 0u)")
        ])
    }
}

[test]
def test_cuda_reject_decode(t : T?) {
    t |> run("a decode-in-load: an arity, the tile, the source, the layout, the callback") <| @(t : T?) {
        check_arms(t, [
            tile_arm("_reject_decode_arity", "coopmatLoadTensorDecode expects (dst, src, base, layout, r0, nr, c0, nc, @@decode [, decode_v])",
                "def coopmatLoadTensorDecode(var d : coopmatWgA_f16_64x64; src : array<DecBlk>; base : uint; var l : tensorLayout2D; r0, nr, c0, nc : uint) \{\n    let z = base\n\}",
                "        var at : coopmatWgA_f16_64x64\n        coopmatLoadTensorDecode(at, blk, 0u, tl, 0u, 64u, 0u, 64u)"),
            tile_arm("_reject_decode_acc", "coopmatLoadTensorDecode: the first argument must be a workgroup-scope f16 A or B tile local",
                "",
                "        var acc : coopmatWgAcc_f32_64x64\n        coopmatLoadTensorDecode(acc, blk, 0u, tl, 0u, 64u, 0u, 64u, @@dec_half)"),
            tile_arm("_reject_decode_source", "coopmatLoadTensorDecode: the source must be an @ssbo array of block structs",
                "",
                "        var at : coopmatWgA_f16_64x64\n        coopmatLoadTensorDecode(at, o, 0u, tl, 0u, 64u, 0u, 64u, @@dec_half)"),
            tile_arm("_reject_decode_layout", "coopmatLoadTensorDecode: the fourth argument must be a tensorLayout2D / tensorLayout2DPad local",
                "",
                "        var at : coopmatWgA_f16_64x64\n        var f = 0.0\n        coopmatLoadTensorDecode(at, blk, 0u, f, 0u, 64u, 0u, 64u, @@dec_half)"),
            tile_arm("_reject_decode_fn", "coopmatLoadTensorDecode: the decode argument must be @@<decode function> or self.<decode method>",
                "",
                "        var at : coopmatWgA_f16_64x64\n        coopmatLoadTensorDecode(at, blk, 0u, tl, 0u, 64u, 0u, 64u, 1.0)"),
            tile_arm("_reject_decode_result", "coopmatLoadTensorDecode: the decode returns int const, which does not convert to the tile's f16 element",
                "",
                "        var at : coopmatWgA_f16_64x64\n        coopmatLoadTensorDecode(at, blk, 0u, tl, 0u, 64u, 0u, 64u, @@dec_int)")
        ])
    }
}
