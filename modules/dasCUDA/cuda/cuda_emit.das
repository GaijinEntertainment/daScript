options gen2
options indenting = 4
options _cyclomatic_complexity = 0   // CUDA emitter: exact-type is-ladders over the AST node classes, one arm per construct, and the census gate wants them all in one place
options _function_length = 0         // same ladders, measured by line

// CUDA C++ text emitter. generate_cuda lowers one [cuda_kernel] class method to a __global__
// kernel over the Vulkan kernel vocabulary (gl_* IDs, @ssbo buffers, @kargs, @workgroup arrays,
// barrier(), the subgroup set, atomics): the @ssbo members become one by-value struct of
// pointers in @binding order (a pointer is const __restrict__ when no statement writes through
// it), the @kargs member a by-value struct parameter, every @workgroup member a carve of the one
// dynamic `__das_smem` block, referenced builtins prologue locals over blockIdx/threadIdx, and every class method or free
// function a __device__ function that takes what it reads - members, builtins - as hidden
// parameters, the SPIR-V call model in text. The kernel text is self-contained: NVRTC ships no
// header, so vector operators, the half family and the warp reductions are a prelude the text
// carries only where it names them. Every construct records a census kind at its emit site.

module cuda_emit shared public

require cuda_types
require daslib/ast
require daslib/ast_boost
require daslib/cmp
require daslib/rtti
require daslib/shader_emit_common
require daslib/strings_boost
require spirv/spirv_builtins
require strings
require math

struct public CudaEmitConfig {
    kernel_name : string    //! "" -> the method name (class-prefix stripped)
    block_size : uint3      //! the launch's block from the annotation; a zero lane leaves __launch_bounds__ off
    min_blocks : int        //! the annotation's min_blocks: __launch_bounds__'s second argument, the blocks an SM the register allocation must allow; 0 leaves it off
    precise : bool          //! the annotation's precise: every scalar f32 add, subtract, multiply and divide emits as its correctly rounded intrinsic (`__fadd_rn` ...), which the fast-math compile neither contracts into an fma nor approximates
}

struct private CudaMember {
    name : string           //! emitted identifier
    elem : string           //! element (buffer, shared) or struct (kargs) spelling
    binding : int
    written : bool
    kargs : bool
    workgroup : bool
    is_array : bool         //! a @workgroup fixed array, not a scalar or vector
    wg_len : int            //! a @workgroup member's element count
    wg_bytes : uint64
    alias_of : string       //! the member whose pointer-struct slot this one views through its own element type; "" = owns the slot
    group_written : bool    //! some member of this binding is written - no view of it is __restrict__
}

//! a struct a buffer element or a local carries: its CUDA declaration and the layout the Vulkan home reads it through
struct private ElemStruct {
    decl : string
    size : int
    align : int
}

//! one discovered user function or class method: both lower 1:1 as __device__ functions, a method
//! taking the members it touches as hidden parameters beside the builtins
struct private UserFn {
    @do_not_delete fn : FunctionPtr
    name : string
    is_method : bool
    builtins : table<string>    //! gl_* this body reads itself
    members : table<string>     //! kernel-class members this body touches itself
    calls : array<int>
    proto : string
    text : string
}

//! a shared carve of slots: the raw blocks of a decoded A tile's step as the source lays them, or the tile's two f16 slots
struct private RawRing {
    off : uint64
    slot_bytes : uint64
}

struct private CudaCtx {
    errors : array<string>
    census : table<string>
    members : table<string; CudaMember>
    builtins_used : table<string>
    renames : table<string; string>
    ptrs : table<string; string>                //! pointer local -> its member
    scan_ptrs : table<string; string>
    arg_structs : table<string; string>         //! kargs struct name -> its definition
    elem_structs : table<string; ElemStruct>        //! element struct name -> its definition and layout
    elem_struct_order : array<string>               //! element structs in declaration order, an inner one before its outer
    helpers : table<string; string>             //! prelude helper key -> text, emitted key-sorted
    const_tables : table<string; string>
    self_name : string
    user_fn_index : table<string; int>
    user_fns : array<UserFn>
    scan_stack : array<string>
    cur_user_fn : int = -1
    fn_depth : int
    warps : int                                 //! the launch block's warps, from the annotation; the cm2 tiles split over them
    row_own : bool                              //! the accumulators' fragment map: a warp owns a fragment row (the scan found a row reduce whose rows the warps divide), else a column
    k_major_tiles : table<string>               //! B tile locals every tensor load reads under a plain view: stored [k][n] (n contiguous)
    n_major_tiles : table<string>               //! B tile locals a tensor load reads under the transposed view: stored [n][k] (k contiguous)
    global_view_tiles : table<string>           //! f16 B tile locals that tensor loads from 16-bit sources write under the transposed view and products alone read: a per-warp global view, never a shared tile
    kperm : bool                                //! the kernel has a global view: every f16 A tile and shared B tile stores its k permuted (WG_KPERM_PRELUDE)
    precise : bool                              //! the kernel's scalar f32 arithmetic emits as the correctly rounded intrinsics (`CudaEmitConfig.precise`)
    tiles : table<string; TileLife>             //! the shared tile locals by name: where each is declared and last read, how it is written - the slot reuse and the ones peephole read them
    loops : array<LoopSpan>                     //! every loop's span the scan walked: a tile declared ahead of a loop and read inside it lives to the loop's end
    scan_at : SrcPos                            //! the furthest position the scan reached
    ones_sum : table<string>                    //! B tiles filled with 1.0 and read by products alone: the product is its A rows' sum in the warp, the tile never carved
    slots : array<SmemSlot>                     //! the tile slots carved so far, each with the tile that holds it
    reduce_sites : int                          //! coopmatReduce sites emitted, each with its own shared scratch
    smem_off : uint64                           //! the dynamic shared block carved so far: every shared declaration's offset comes from here
    layout_block_size : table<string; int2>            //! the tensor layout locals whose block size is a compile-time pair (b0, b1), by their emitted name
    scan_layout_block_size : table<string; int2>       //! the same by the local's own name, as the scan walks the sets ahead of the decode calls
    raw_rings : table<string; RawRing>          //! the decoded A tiles' raw block slot, by tile name
    decode_slot_pairs : table<string; RawRing>          //! the decoded A tiles' two f16 slots (off = the first, slot_bytes = one), by tile name; the tile's local is a pointer the decode rebinds
    deferred : table<string; string>            //! a decoded A tile's next-step decode pass, emitted after the next product over the tile
    deferred_post : table<string; string>       //! an s8 ring tile's copies, emitted past the ring barrier that follows its deferred text
    flush_after : array<string>                 //! statement text to emit right after the statement being emitted
    hoisted : array<string>                     //! declarations at kernel scope: a pipelined tile's slot pointer and prediction registers outlive the loop its local sits in
    raw_s8_tiles : table<string>                //! the s8 tiles on a cp.async ring: their deferred copies share one block barrier past the product
    @do_not_delete kcls : StructurePtr
}

//! C++ keywords plus the names NVRTC's device runtime owns and the prelude's own types
let private CUDA_RESERVED : table<string> <- {
    "auto", "template", "typename", "using", "namespace", "constexpr", "new", "delete", "this",
    "class", "struct", "union", "signed", "unsigned", "void", "const", "static", "inline",
    "extern", "register", "volatile", "short", "long", "char", "bool", "int", "float", "double",
    "half", "half2", "half4", "uint", "ushort", "uchar", "threadIdx", "blockIdx", "blockDim",
    "gridDim", "warpSize", "default", "case", "switch", "operator", "typedef", "sizeof",
    "throw", "try", "catch", "goto", "do", "asm", "export", "virtual", "friend", "public",
    "private", "protected", "mutable", "explicit", "enum", "wchar_t", "and", "or", "not", "xor"
}

def private safe_name(name : string) : string => shader_safe_name(CUDA_RESERVED, name)

def private census_kind(var ctx : CudaCtx; kind : string) {
    ctx.census |> insert(kind)
}

struct private BuiltinInfo {
    ok : bool
    ctype : string
    init : string           //! the prologue initializer
    dep : string            //! a builtin this one reads first, "" for none
}

def private builtin_info(name : string) : BuiltinInfo {
    if (name == "gl_GlobalInvocationID") {
        return BuiltinInfo(ok = true, ctype = "uint3",
            init = "make_uint3(blockIdx.x * blockDim.x + threadIdx.x, blockIdx.y * blockDim.y + threadIdx.y, blockIdx.z * blockDim.z + threadIdx.z)")
    } elif (name == "gl_LocalInvocationID") {
        return BuiltinInfo(ok = true, ctype = "uint3", init = "make_uint3(threadIdx.x, threadIdx.y, threadIdx.z)")
    } elif (name == "gl_WorkGroupID") {
        return BuiltinInfo(ok = true, ctype = "uint3", init = "make_uint3(blockIdx.x, blockIdx.y, blockIdx.z)")
    } elif (name == "gl_NumWorkGroups") {
        return BuiltinInfo(ok = true, ctype = "uint3", init = "make_uint3(gridDim.x, gridDim.y, gridDim.z)")
    } elif (name == "gl_WorkGroupSize") {
        return BuiltinInfo(ok = true, ctype = "uint3", init = "make_uint3(blockDim.x, blockDim.y, blockDim.z)")
    } elif (name == "gl_LocalInvocationIndex") {
        return BuiltinInfo(ok = true, ctype = "uint", init = "threadIdx.x + threadIdx.y * blockDim.x + threadIdx.z * blockDim.x * blockDim.y")
    } elif (name == "gl_SubgroupInvocationID") {
        return BuiltinInfo(ok = true, ctype = "uint", init = "gl_LocalInvocationIndex & 31u", dep = "gl_LocalInvocationIndex")
    } elif (name == "gl_SubgroupSize") {
        return BuiltinInfo(ok = true, ctype = "uint", init = "32u")
    } elif (name == "gl_SubgroupID") {
        return BuiltinInfo(ok = true, ctype = "uint", init = "gl_LocalInvocationIndex >> 5", dep = "gl_LocalInvocationIndex")
    } elif (name == "gl_NumSubgroups") {
        return BuiltinInfo(ok = true, ctype = "uint", init = "(blockDim.x * blockDim.y * blockDim.z + 31u) >> 5")
    }
    return BuiltinInfo(ok = false)
}

//! the prologue order: a builtin after the one it reads
let private BUILTIN_ORDER <- [
    "gl_WorkGroupID", "gl_LocalInvocationID", "gl_WorkGroupSize", "gl_NumWorkGroups", "gl_GlobalInvocationID",
    "gl_LocalInvocationIndex", "gl_SubgroupInvocationID", "gl_SubgroupSize", "gl_SubgroupID", "gl_NumSubgroups"
]

def private use_builtin(var ctx : CudaCtx; name : string) {
    ctx.builtins_used |> insert(name)
    if (ctx.cur_user_fn >= 0) {
        ctx.user_fns[ctx.cur_user_fn].builtins |> insert(name)
    }
    let dep = builtin_info(name).dep
    if (!empty(dep)) {
        use_builtin(ctx, dep)
    }
}

def private align_up(off, a : int) : int => a <= 1 ? off : ((off + a - 1) / a) * a

//! the prelude emits its helpers sorted by key: a rank prefix sorts a helper after the ranks whose declarations it reads
let private HELPER_RANK_COOPMAT = "zcm.0"
let private HELPER_RANK_CM2 = "zcm.1"
let private HELPER_RANK_COOPMAT_OPS = "zcm.2"
let private HELPER_RANK_WG_TILE = "zcm.3"
let private HELPER_RANK_WG_ACCESS = "zcm.4"
let private HELPER_RANK_WG_MMA = "zcm.5"
let private HELPER_RANK_WG_ROWSUM = "zcm.6"
let private HELPER_RANK_INTRINSIC = "zin."
let private HELPER_RANK_SMEM = "zz."

def private register_helper(var ctx : CudaCtx; key, text : string) {
    if (!key_exists(ctx.helpers, key)) {
        ctx.helpers[key] = text
    }
}

//! the one dynamic shared block every shared declaration is carved from, 16-aligned; the kernel's
//! `_smem` companion is the carve's end and the launch passes it
let private SMEM_PRELUDE = "extern __shared__ __align__(16) char __das_smem[];"

//! `bytes` of shared memory at a 16-aligned offset of the block; the declaration text binds `name` to it
def private smem_carve(var ctx : CudaCtx; bytes : uint64) : uint64 {
    register_helper(ctx, "{HELPER_RANK_SMEM}smem", SMEM_PRELUDE)
    let off = (ctx.smem_off + 15ul) & ~15ul
    ctx.smem_off = off + bytes
    return off
}

//! a shared array of `n` elements, bound to its carve as a pointer
def private smem_decl(var ctx : CudaCtx; ctype, name : string; elem_bytes : uint64; n : int) : string {
    let off = int(smem_carve(ctx, elem_bytes * uint64(n)))
    return "{ctype}* {name} = ({ctype}*)(__das_smem + {off});"
}

//! one shared value, bound to its carve as a reference
def private smem_decl(var ctx : CudaCtx; ctype, name : string; bytes : uint64) : string {
    let off = int(smem_carve(ctx, bytes))
    return "{ctype}& {name} = *({ctype}*)(__das_smem + {off});"
}

//! a struct a buffer element or a local carries, declared once: scalar, vector (1, 2 or 4 lanes), fixed-array
//! and nested-struct fields at CUDA's natural layout, which the std430 layout the Vulkan home reads the
//! same bytes through must equal field by field; "" after err when a field has no CUDA form or the layouts part
def private elem_struct(var ctx : CudaCtx; st : StructurePtr; at : LineInfo) : string {
    if (st == null) {
        shader_error(ctx, at, "struct type has no structure")
        return ""
    }
    let sname = safe_name(string(st.name))
    if (key_exists(ctx.elem_structs, sname)) {
        return empty(ctx.elem_structs[sname].decl) ? "" : sname
    }
    ctx.elem_structs[sname] = ElemStruct()
    if (empty(st.fields)) {
        shader_error(ctx, at, "struct `{st.name}` has no fields - an opaque Vulkan type (the coopmat, tensorLayout and tensorView family) has no CUDA form yet")
        return ""
    }
    var lines : array<string>
    var offsets : array<int>
    var off = 0
    var maxAlign = 1
    var bad = false
    for (f in st.fields) {
        let isArr = f._type != null && f._type.baseType == Type.tFixedArray
        let et = isArr ? f._type.firstType : f._type
        let count = isArr ? f._type.fixedDim : -1
        if (et == null || count == 0) {
            shader_error(ctx, at, "struct `{st.name}` field `{f.name}` has no CUDA form")
            bad = true
            continue
        }
        if (reject_cpu_only_width(ctx, at, et)) {
            bad = true
            continue
        }
        var tn = ""
        var falign = 0
        var fsize = 0
        if (et.baseType == Type.tStructure) {
            tn = elem_struct(ctx, et.structType, at)
            if (empty(tn)) {
                bad = true
                continue
            }
            falign = ctx.elem_structs[tn].align
            fsize = ctx.elem_structs[tn].size
        } else {
            tn = et.baseType == Type.tBool ? "" : shader_type_name(et)
            if (empty(tn)) {
                shader_error(ctx, at, "struct `{st.name}` field `{f.name}` type {describe(et)} has no CUDA form (int/uint/float scalars, the 16/8-bit lattice, their 2/4 vectors, fixed arrays and structs of those)")
                bad = true
                continue
            }
            let lanes = component_count(et.baseType)
            if (lanes == 3) {
                shader_error(ctx, at, "struct `{st.name}` field `{f.name}` is a 3-lane vector - std430 aligns it to 4 lanes and CUDA to 1, so the two homes read different bytes; spell it as 4 lanes or as scalars")
                bad = true
                continue
            }
            fsize = (scalar_width(et.baseType) / 8) * lanes
            falign = fsize
            if (cuda_is_half_family(et)) {
                need_half(ctx, et)
            }
        }
        off = align_up(off, falign)
        offsets |> push(off)
        off += count > 0 ? fsize * count : fsize
        maxAlign = max(maxAlign, falign)
        lines |> push(count > 0 ? "    {tn} {safe_name(string(f.name))}[{count}];" : "    {tn} {safe_name(string(f.name))};")
    }
    let size = align_up(off, maxAlign)
    if (!bad) {
        var inscope fields : array<BlockField>
        var inscope lerr : array<string>
        let lay = compute_block_layout(sname, st, fields, lerr, BlockLayoutRules.std430)
        var same = lay.ok && lay.size == size && lay.align == maxAlign && length(fields) == length(offsets)
        if (same) {
            for (bf, o in fields, offsets) {
                same = same && bf.offset == o
            }
        }
        if (!same) {
            let theirs = lay.ok ? "{lay.size} bytes, align {lay.align}" : join(lerr, "; ")
            shader_error(ctx, at, "struct `{st.name}`: its CUDA layout ({size} bytes, align {maxAlign}) is not the std430 layout the Vulkan home reads the same buffer through ({theirs})")
            bad = true
        }
    }
    if (bad || empty(lines)) {
        if (empty(lines) && !bad) {
            shader_error(ctx, at, "struct `{st.name}` has no fields")
        }
        delete lines
        return ""
    }
    ctx.elem_structs[sname] = ElemStruct(decl = "struct {sname} \{\n{join(lines, "\n")}\n\};", size = size, align = maxAlign)
    ctx.elem_struct_order |> push(sname)
    delete lines
    return sname
}

//! the CUDA spelling of a type a buffer element, a local or a parameter carries: a KHR tile is its fragment
//! struct, any other struct declares itself on first sight
def private type_spell(var ctx : CudaCtx; t : TypeDecl?; at : LineInfo) : string {
    if (t != null && t.baseType == Type.tStructure) {
        let ci = coop_type_of(t)
        if (ci.ok) {
            need_coopmat(ctx)
            return ci.ctype
        }
        let wi = wg_type_of(ctx, t)
        if (wi.ok) {
            need_wg(ctx, wi)
            return wi.ctype
        }
        if (is_tensor_layout(t) || is_tensor_view(t)) {
            need_cm2(ctx)
            return is_tensor_layout(t) ? "__das_tlayout" : "__das_tview"
        }
        if (t.structType != null && (string(t.structType.name) |> starts_with("coopmatWg")) && ctx.warps <= 0) {
            shader_error(ctx, at, "a workgroup-scope coopmat tile needs the kernel's block: give [cuda_kernel] its local_size_x")
            return ""
        }
        return elem_struct(ctx, t.structType, at)
    }
    return shader_type_name(t)
}

//! a binding two members view through their own element types: a write through any view marks the whole group
def private settle_alias_groups(var ctx : CudaCtx) {
    var written : table<int>
    for (m in values(ctx.members)) {
        if (!m.workgroup && !m.kargs && m.written) {
            written |> insert(m.binding)
        }
    }
    var marks <- [for (name, m in keys(ctx.members), values(ctx.members)); (name = name, w = key_exists(written, m.binding)); where !m.workgroup && !m.kargs]
    for (mk in marks) {
        ctx.members[mk.name].group_written = mk.w
    }
}

//! a buffer's CUDA pointer spelling: `T*` when written, `const T*` otherwise, `__restrict__` only while no view of its binding is written
def private buffer_ptr_spelling(m : CudaMember) : string =>
    m.written ? "{m.elem}* {m.name}" : (m.group_written ? "const {m.elem}* {m.name}" : "const {m.elem}* __restrict__ {m.name}")

def private scan_members(var ctx : CudaCtx; fn : FunctionPtr) {
    for (fld in fn.classParent.fields) {
        if ((fld.name == "__rtti" || fld.name == "__finalize") ||
                (fld._type != null && (fld._type.baseType == Type.tFunction || fld._type.baseType == Type.tLambda))) {
            continue
        }
        var isSsbo = false
        var isKargs = false
        var isWorkgroup = false
        var binding = -1
        for (annDecl in fld.annotation) {
            if (annDecl.name == "ssbo") {
                isSsbo = true
            } elif (annDecl.name == "kargs" || annDecl.name == "uniform" || annDecl.name == "push_constant") {
                isKargs = true
            } elif (annDecl.name == "workgroup") {
                isWorkgroup = true
            } elif (annDecl.name == "binding" && annDecl.basicType == Type.tInt) {
                binding = annDecl.iValue
            }
        }
        let fname = string(fld.name)
        if (!isSsbo && !isKargs && !isWorkgroup) {
            shader_error(ctx, fld.at, "member `{fld.name}` is neither @ssbo, @kargs nor @workgroup - a CUDA kernel class carries no plain members (a method is a function here, not a splice)")
            continue
        }
        if ((isSsbo ? 1 : 0) + (isKargs ? 1 : 0) + (isWorkgroup ? 1 : 0) > 1) {
            shader_error(ctx, fld.at, "member `{fld.name}` mixes @ssbo/@kargs/@workgroup - pick one")
            continue
        }
        if (isWorkgroup) {
            var wgElem = ""
            var wgIsArray = false
            var wgLen = 1
            var wgElemBytes = 0ul
            if (fld._type != null && fld._type.baseType == Type.tFixedArray && fld._type.firstType != null &&
                    fld._type.firstType.baseType != Type.tFixedArray) {
                if (reject_cpu_only_width(ctx, fld.at, fld._type.firstType)) {
                    continue
                }
                wgElem = shader_type_name(fld._type.firstType)
                wgElemBytes = cuda_elem_bytes(fld._type.firstType)
                wgIsArray = true
                wgLen = fld._type.fixedDim
            } else {
                if (reject_cpu_only_width(ctx, fld.at, fld._type)) {
                    continue
                }
                wgElem = shader_type_name(fld._type)
                wgElemBytes = cuda_elem_bytes(fld._type)
            }
            if (empty(wgElem) || wgLen == 0) {
                shader_error(ctx, fld.at, "@workgroup member `{fld.name}` must be a scalar, vector, or single-dimension fixed array of those (e.g. uint[64]); got {fld._type == null ? "?" : describe(fld._type)}")
                continue
            }
            ctx.renames[fname] = safe_name(fname)
            ctx.members[fname] = CudaMember(name = safe_name(fname), elem = wgElem, binding = -1, workgroup = true,
                is_array = wgIsArray, wg_len = wgLen, wg_bytes = wgElemBytes * uint64(wgLen))
            continue
        }
        if (isKargs) {
            if (fld._type == null || fld._type.baseType != Type.tStructure) {
                shader_error(ctx, fld.at, "@kargs member `{fld.name}` must be a struct of 4-byte scalars; got {fld._type == null ? "?" : describe(fld._type)}")
                continue
            }
            for (existing, m in keys(ctx.members), values(ctx.members)) {
                if (m.kargs) {
                    shader_error(ctx, fld.at, "members `{existing}` and `{fname}` are both @kargs - a kernel takes one args struct")
                }
            }
            let sname = scalar_args_struct(ctx, fld._type.structType, fld.at, CUDA_RESERVED, "@kargs", "CUDA", "struct __align__(8)")
            if (empty(sname)) {
                continue
            }
            ctx.renames[fname] = safe_name(fname)
            ctx.members[fname] = CudaMember(name = safe_name(fname), elem = sname, binding = -1, kargs = true)
            continue
        }
        if (binding < 0) {
            shader_error(ctx, fld.at, "@ssbo member `{fld.name}` has no @binding - every buffer needs an explicit index, its slot in the pointer struct")
            continue
        }
        if (fld._type == null || fld._type.baseType != Type.tArray) {
            shader_error(ctx, fld.at, "@ssbo member `{fld.name}` must be array<T>; got {fld._type == null ? "?" : describe(fld._type)}")
            continue
        }
        if (reject_cpu_only_width(ctx, fld.at, fld._type.firstType)) {
            continue
        }
        let elem = type_spell(ctx, fld._type.firstType, fld.at)
        if (empty(elem)) {
            if (fld._type.firstType == null || fld._type.firstType.baseType != Type.tStructure) {
                shader_error(ctx, fld.at, "@ssbo member `{fld.name}` element type {describe(fld._type.firstType)} has no CUDA form (int/uint/float scalars, the 16/8-bit lattice, their 2/3/4 vectors, structs of those)")
            }
            continue
        }
        var owner = ""
        for (existing, m in keys(ctx.members), values(ctx.members)) {
            if (m.binding == binding && empty(m.alias_of)) {
                owner = existing
            }
        }
        ctx.renames[fname] = safe_name(fname)
        ctx.members[fname] = CudaMember(name = safe_name(fname), elem = elem, binding = binding, alias_of = owner)
    }
    var ssboCount = 0
    for (m in values(ctx.members)) {
        if (!m.kargs && !m.workgroup) {
            ssboCount++
        }
    }
    if (empty(ctx.errors) && ssboCount == 0) {
        shader_error(ctx, fn.at, "kernel class `{fn.classParent.name}` has no @ssbo members - a kernel with no buffers cannot observe or produce anything")
    }
}

//! the kernel-class member an expression names: `member`, `self.member`, or a pointer local over one; "" otherwise
def private member_lookup(ctx : CudaCtx; e : Expression?) : string {
    if (e == null) {
        return ""
    }
    if (e is ExprRef2Value) {
        return member_lookup(ctx, (e as ExprRef2Value).subexpr)
    }
    if (e is ExprField) {
        let ef = e as ExprField
        let n = string(ef.name)
        if (is_self_ref(ctx, ef.value) && key_exists(ctx.members, n)) {
            return n
        }
        return ""
    }
    if (e is ExprVar) {
        let n = string((e as ExprVar).name)
        if (key_exists(ctx.ptrs, n)) {
            return ctx.ptrs?[n] ?? ""
        }
        if (key_exists(ctx.scan_ptrs, n)) {
            return ctx.scan_ptrs?[n] ?? ""
        }
        return ""
    }
    return ""
}

def private use_member(var ctx : CudaCtx; name : string) {
    if (ctx.cur_user_fn >= 0) {
        ctx.user_fns[ctx.cur_user_fn].members |> insert(name)
    }
}

def private mark_write_target(var ctx : CudaCtx; e : Expression?) {
    if (e == null) {
        return
    }
    if (e is ExprRef2Value) {
        mark_write_target(ctx, (e as ExprRef2Value).subexpr)
    } elif (e is ExprPtr2Ref) {
        mark_write_target(ctx, (e as ExprPtr2Ref).subexpr)
    } elif (e is ExprUnsafe) {
        mark_write_target(ctx, (e as ExprUnsafe).body)
    } elif (e is ExprRef2Ptr) {
        mark_write_target(ctx, (e as ExprRef2Ptr).subexpr)
    } elif (e is ExprCall && call_base_name("{(e as ExprCall).name}") == "i_das_ptr_add") {
        mark_write_target(ctx, (e as ExprCall).arguments[0])
    } elif (e is ExprAt) {
        mark_write_target(ctx, (e as ExprAt).subexpr)
    } elif (e is ExprSwizzle) {
        mark_write_target(ctx, (e as ExprSwizzle).value)
    } elif (e is ExprField) {
        let name = string((e as ExprField).name)
        if (key_exists(ctx.members, name)) {
            ctx.members[name].written = true
        } else {
            mark_write_target(ctx, (e as ExprField).value)
        }
    } elif (e is ExprVar) {
        let pn = string((e as ExprVar).name)
        if (key_exists(ctx.scan_ptrs, pn)) {
            let mem = ctx.scan_ptrs[pn]
            if (key_exists(ctx.members, mem)) {
                ctx.members[mem].written = true
            }
        }
    }
}

//! the das-coded names the emit ladders own - discovery must never lower these as user functions
let private LADDER_OWNED : table<string> <- {
    "barrier", "memoryBarrierShared", "memoryBarrierBuffer", "i_das_ptr_add",
    "subgroupAdd", "subgroupMin", "subgroupMax", "subgroupShuffle", "subgroupShuffleXor", "subgroupBroadcast",
    "subgroupBroadcastFirst", "subgroupElect", "subgroupAll", "subgroupAny", "subgroupClusteredAdd",
    "atomicAdd", "atomicMin", "atomicMax", "atomicAnd", "atomicOr", "atomicXor", "atomicExchange", "atomicCompSwap",
    "sdot4", "usdot4", "uint_bits_to_float", "int_bits_to_float", "float_bits_to_uint", "float_bits_to_int",
    "packHalf2x16", "unpackHalf2x16", "unpack8", "pack32",
    "coopmatLoad", "coopmatStore", "coopmatMulAdd", "coopmatConvert",
    "tensorLayoutCreate", "tensorLayoutSetDimension", "tensorLayoutSetStride", "tensorLayoutSetBlockSize", "tensorViewCreate",
    "coopmatLoadTensor", "coopmatLoadTensorDecode", "coopmatStoreTensor", "coopmatPerElement", "coopmatReduce", "coopmatScaleAcc",
    "coopmatFill", "coopmatScale", "coopmatCopy", "coopmatAdd", "coopmatSub", "coopmatMulElem", "coopmatClamp"
}

//! the cm2 statement forms emit_cm2_stmt owns
let private CM2_STATEMENTS : table<string> <- {
    "tensorLayoutCreate", "tensorLayoutSetDimension", "tensorLayoutSetStride", "tensorLayoutSetBlockSize", "tensorViewCreate",
    "coopmatLoadTensor", "coopmatLoadTensorDecode", "coopmatStoreTensor", "coopmatPerElement", "coopmatReduce", "coopmatScaleAcc",
    "coopmatFill", "coopmatScale", "coopmatCopy", "coopmatAdd", "coopmatSub", "coopmatMulElem", "coopmatClamp"
}

//! a KHR marker type's CUDA fragment: the struct, its use (A, B, C) and its component spelling
struct private CoopInfo {
    ok : bool
    ctype : string
    use : string
    comp : string
}

def private coop_info(name : string) : CoopInfo {
    if (name == "coopmatA_f16_16x16") {
        return CoopInfo(ok = true, ctype = "__das_cmA_f16", use = "A", comp = "f16")
    } elif (name == "coopmatB_f16_16x16") {
        return CoopInfo(ok = true, ctype = "__das_cmB_f16", use = "B", comp = "f16")
    } elif (name == "coopmatAcc_f32_16x16") {
        return CoopInfo(ok = true, ctype = "__das_cmC_f32", use = "C", comp = "f32")
    } elif (name == "coopmatAcc_f16_16x16") {
        return CoopInfo(ok = true, ctype = "__das_cmC_f16", use = "C", comp = "f16")
    } elif (name == "coopmatA_s8_16x32") {
        return CoopInfo(ok = true, ctype = "__das_cmA_s8", use = "A", comp = "s8")
    } elif (name == "coopmatB_s8_32x16") {
        return CoopInfo(ok = true, ctype = "__das_cmB_s8", use = "B", comp = "s8")
    } elif (name == "coopmatAcc_s32_16x16") {
        return CoopInfo(ok = true, ctype = "__das_cmC_s32", use = "C", comp = "s32")
    }
    return CoopInfo(ok = false)
}

def private coop_type_of(t : TypeDecl?) : CoopInfo {
    if (t == null || t.baseType != Type.tStructure || t.structType == null) {
        return CoopInfo(ok = false)
    }
    return coop_info(string(t.structType.name))
}

//! the fragment structs over the m16n8k16 (f16) and m16n8k32 (s8) lane layouts - a 16-wide N is two
//! n-halves, so B carries two fragments and an accumulator two - the lane split and the raw loads
let private COOPMAT_PRELUDE = "struct __das_cmA_f16 \{ uint r[4]; \};
struct __das_cmB_f16 \{ uint r[4]; \};
struct __das_cmC_f32 \{ float r[8]; \};
struct __das_cmC_f16 \{ uint r[4]; \};
struct __das_cmA_s8 \{ uint r[4]; \};
struct __das_cmB_s8 \{ uint r[4]; \};
struct __das_cmC_s32 \{ int r[8]; \};
__device__ __forceinline__ int __das_lane() \{ return int((threadIdx.x + threadIdx.y * blockDim.x + threadIdx.z * blockDim.x * blockDim.y) & 31u); \}
__device__ __forceinline__ uint __das_ld32(const char* p) \{ return *(const uint*)p; \}
__device__ __forceinline__ uint __das_ldh2(const char* p0, const char* p1) \{ return uint(*(const ushort*)p0) | (uint(*(const ushort*)p1) << 16); \}
__device__ __forceinline__ uint __das_ld4b(const char* p0, const char* p1, const char* p2, const char* p3) \{ return uint(*(const uchar*)p0) | (uint(*(const uchar*)p1) << 8) | (uint(*(const uchar*)p2) << 16) | (uint(*(const uchar*)p3) << 24); \}
__device__ __forceinline__ float __das_h2f(uint h) \{ float f; unsigned short b = (unsigned short)(h & 0xffffu); asm(\"cvt.f32.f16 %0, %1;\" : \"=f\"(f) : \"h\"(b)); return f; \}"

def private need_coopmat(var ctx : CudaCtx) {
    register_helper(ctx, "{HELPER_RANK_COOPMAT}base", COOPMAT_PRELUDE)
    census_kind(ctx, "prelude.coopmat")
}

//! the gather of one fragment from a byte base `p` with a row stride of `sb` bytes: a 32-bit load where
//! the fragment's pair (f16) or quad (s8) is contiguous, a lane-wise gather where the layout runs the
//! other way; `g` = lane / 4 is the fragment row (or n), `t` = lane % 4 its pair column
def private coop_load_helper(ci : CoopInfo; row : bool) : string {
    let name = "__das_cm_ld_{ci.use}_{ci.comp}_{row ? "row" : "col"}"
    var body = ""
    if (ci.comp == "f16" && ci.use == "A") {
        body = (row ?
            "m.r[0] = __das_ld32(p + g * sb + t * 4); m.r[1] = __das_ld32(p + (g + 8) * sb + t * 4); m.r[2] = __das_ld32(p + g * sb + t * 4 + 16); m.r[3] = __das_ld32(p + (g + 8) * sb + t * 4 + 16);" :
            "m.r[0] = __das_ldh2(p + (2 * t) * sb + g * 2, p + (2 * t + 1) * sb + g * 2); m.r[1] = __das_ldh2(p + (2 * t) * sb + (g + 8) * 2, p + (2 * t + 1) * sb + (g + 8) * 2); m.r[2] = __das_ldh2(p + (2 * t + 8) * sb + g * 2, p + (2 * t + 9) * sb + g * 2); m.r[3] = __das_ldh2(p + (2 * t + 8) * sb + (g + 8) * 2, p + (2 * t + 9) * sb + (g + 8) * 2);")
    } elif (ci.comp == "f16" && ci.use == "B") {
        body = (row ?
            "m.r[0] = __das_ldh2(p + (2 * t) * sb + g * 2, p + (2 * t + 1) * sb + g * 2); m.r[1] = __das_ldh2(p + (2 * t + 8) * sb + g * 2, p + (2 * t + 9) * sb + g * 2); m.r[2] = __das_ldh2(p + (2 * t) * sb + (g + 8) * 2, p + (2 * t + 1) * sb + (g + 8) * 2); m.r[3] = __das_ldh2(p + (2 * t + 8) * sb + (g + 8) * 2, p + (2 * t + 9) * sb + (g + 8) * 2);" :
            "m.r[0] = __das_ld32(p + g * sb + t * 4); m.r[1] = __das_ld32(p + g * sb + t * 4 + 16); m.r[2] = __das_ld32(p + (g + 8) * sb + t * 4); m.r[3] = __das_ld32(p + (g + 8) * sb + t * 4 + 16);")
    } elif (ci.comp == "s8" && ci.use == "A") {
        body = (row ?
            "m.r[0] = __das_ld32(p + g * sb + t * 4); m.r[1] = __das_ld32(p + (g + 8) * sb + t * 4); m.r[2] = __das_ld32(p + g * sb + t * 4 + 16); m.r[3] = __das_ld32(p + (g + 8) * sb + t * 4 + 16);" :
            "m.r[0] = __das_ld4b(p + (4 * t) * sb + g, p + (4 * t + 1) * sb + g, p + (4 * t + 2) * sb + g, p + (4 * t + 3) * sb + g); m.r[1] = __das_ld4b(p + (4 * t) * sb + g + 8, p + (4 * t + 1) * sb + g + 8, p + (4 * t + 2) * sb + g + 8, p + (4 * t + 3) * sb + g + 8); m.r[2] = __das_ld4b(p + (4 * t + 16) * sb + g, p + (4 * t + 17) * sb + g, p + (4 * t + 18) * sb + g, p + (4 * t + 19) * sb + g); m.r[3] = __das_ld4b(p + (4 * t + 16) * sb + g + 8, p + (4 * t + 17) * sb + g + 8, p + (4 * t + 18) * sb + g + 8, p + (4 * t + 19) * sb + g + 8);")
    } elif (ci.comp == "s8" && ci.use == "B") {
        body = (row ?
            "m.r[0] = __das_ld4b(p + (4 * t) * sb + g, p + (4 * t + 1) * sb + g, p + (4 * t + 2) * sb + g, p + (4 * t + 3) * sb + g); m.r[1] = __das_ld4b(p + (4 * t + 16) * sb + g, p + (4 * t + 17) * sb + g, p + (4 * t + 18) * sb + g, p + (4 * t + 19) * sb + g); m.r[2] = __das_ld4b(p + (4 * t) * sb + g + 8, p + (4 * t + 1) * sb + g + 8, p + (4 * t + 2) * sb + g + 8, p + (4 * t + 3) * sb + g + 8); m.r[3] = __das_ld4b(p + (4 * t + 16) * sb + g + 8, p + (4 * t + 17) * sb + g + 8, p + (4 * t + 18) * sb + g + 8, p + (4 * t + 19) * sb + g + 8);" :
            "m.r[0] = __das_ld32(p + g * sb + t * 4); m.r[1] = __das_ld32(p + g * sb + t * 4 + 16); m.r[2] = __das_ld32(p + (g + 8) * sb + t * 4); m.r[3] = __das_ld32(p + (g + 8) * sb + t * 4 + 16);")
    }
    return "__device__ __forceinline__ void {name}({ci.ctype}& m, const char* p, int sb) \{ int l = __das_lane(), g = l >> 2, t = l & 3; {body} \}"
}

//! the scatter of an accumulator: f32 / s32 lanes as scalars, an f16 accumulator's pairs as two halves (a
//! half-aligned base must stay legal); `h` walks the two n-halves
def private coop_store_helper(ci : CoopInfo; row : bool) : string {
    let name = "__das_cm_st_C_{ci.comp}_{row ? "row" : "col"}"
    var body = ""
    if (ci.comp == "f16") {
        let at0 = row ? "p + g * sb + (2 * t + 8 * h) * 2" : "p + (2 * t + 8 * h) * sb + g * 2"
        let at1 = row ? "p + g * sb + (2 * t + 1 + 8 * h) * 2" : "p + (2 * t + 1 + 8 * h) * sb + g * 2"
        let at2 = row ? "p + (g + 8) * sb + (2 * t + 8 * h) * 2" : "p + (2 * t + 8 * h) * sb + (g + 8) * 2"
        let at3 = row ? "p + (g + 8) * sb + (2 * t + 1 + 8 * h) * 2" : "p + (2 * t + 1 + 8 * h) * sb + (g + 8) * 2"
        body = "for (int h = 0; h < 2; ++h) \{ *(ushort*)({at0}) = (ushort)(m.r[2 * h] & 0xffffu); *(ushort*)({at1}) = (ushort)(m.r[2 * h] >> 16); *(ushort*)({at2}) = (ushort)(m.r[2 * h + 1] & 0xffffu); *(ushort*)({at3}) = (ushort)(m.r[2 * h + 1] >> 16); \}"
    } else {
        let sc = ci.comp == "f32" ? "float" : "int"
        let at0 = row ? "p + g * sb + (2 * t + 8 * h) * 4" : "p + (2 * t + 8 * h) * sb + g * 4"
        let at1 = row ? "p + g * sb + (2 * t + 1 + 8 * h) * 4" : "p + (2 * t + 1 + 8 * h) * sb + g * 4"
        let at2 = row ? "p + (g + 8) * sb + (2 * t + 8 * h) * 4" : "p + (2 * t + 8 * h) * sb + (g + 8) * 4"
        let at3 = row ? "p + (g + 8) * sb + (2 * t + 1 + 8 * h) * 4" : "p + (2 * t + 1 + 8 * h) * sb + (g + 8) * 4"
        body = "for (int h = 0; h < 2; ++h) \{ *({sc}*)({at0}) = m.r[4 * h]; *({sc}*)({at1}) = m.r[4 * h + 1]; *({sc}*)({at2}) = m.r[4 * h + 2]; *({sc}*)({at3}) = m.r[4 * h + 3]; \}"
    }
    return "__device__ __forceinline__ void {name}(const {ci.ctype}& m, char* p, int sb) \{ int l = __das_lane(), g = l >> 2, t = l & 3; {body} \}"
}

//! one 16x16x16 (f16) or 16x16x32 (s8) tile product: two m16n8 mma.sync, one per n-half
def private coop_mma_helper(a : CoopInfo; c : CoopInfo) : string {
    let name = "__das_mma_{a.comp}_{c.comp}"
    let bt = "__das_cmB_{a.comp}"
    if (c.comp == "f16") {
        return "__device__ __forceinline__ {c.ctype} {name}(const {a.ctype}& a, const {bt}& b, {c.ctype} c) \{ asm(\"mma.sync.aligned.m16n8k16.row.col.f16.f16.f16.f16 \{%0,%1\}, \{%2,%3,%4,%5\}, \{%6,%7\}, \{%0,%1\};\" : \"+r\"(c.r[0]), \"+r\"(c.r[1]) : \"r\"(a.r[0]), \"r\"(a.r[1]), \"r\"(a.r[2]), \"r\"(a.r[3]), \"r\"(b.r[0]), \"r\"(b.r[1])); asm(\"mma.sync.aligned.m16n8k16.row.col.f16.f16.f16.f16 \{%0,%1\}, \{%2,%3,%4,%5\}, \{%6,%7\}, \{%0,%1\};\" : \"+r\"(c.r[2]), \"+r\"(c.r[3]) : \"r\"(a.r[0]), \"r\"(a.r[1]), \"r\"(a.r[2]), \"r\"(a.r[3]), \"r\"(b.r[2]), \"r\"(b.r[3])); return c; \}"
    }
    let op = a.comp == "s8" ? "mma.sync.aligned.m16n8k32.row.col.s32.s8.s8.s32" : "mma.sync.aligned.m16n8k16.row.col.f32.f16.f16.f32"
    let cc = c.comp == "s32" ? "r" : "f"
    return "__device__ __forceinline__ {c.ctype} {name}(const {a.ctype}& a, const {bt}& b, {c.ctype} c) \{ asm(\"{op} \{%0,%1,%2,%3\}, \{%4,%5,%6,%7\}, \{%8,%9\}, \{%0,%1,%2,%3\};\" : \"+{cc}\"(c.r[0]), \"+{cc}\"(c.r[1]), \"+{cc}\"(c.r[2]), \"+{cc}\"(c.r[3]) : \"r\"(a.r[0]), \"r\"(a.r[1]), \"r\"(a.r[2]), \"r\"(a.r[3]), \"r\"(b.r[0]), \"r\"(b.r[1])); asm(\"{op} \{%0,%1,%2,%3\}, \{%4,%5,%6,%7\}, \{%8,%9\}, \{%0,%1,%2,%3\};\" : \"+{cc}\"(c.r[4]), \"+{cc}\"(c.r[5]), \"+{cc}\"(c.r[6]), \"+{cc}\"(c.r[7]) : \"r\"(a.r[0]), \"r\"(a.r[1]), \"r\"(a.r[2]), \"r\"(a.r[3]), \"r\"(b.r[2]), \"r\"(b.r[3])); return c; \}"
}

let private COOPMAT_CVT_F32_F16 = "__device__ __forceinline__ __das_cmC_f32 __das_cm_cvt_f32_f16(const __das_cmC_f16& s) \{ __das_cmC_f32 d; for (int h = 0; h < 2; ++h) \{ d.r[4 * h] = __das_h2f(s.r[2 * h]); d.r[4 * h + 1] = __das_h2f(s.r[2 * h] >> 16); d.r[4 * h + 2] = __das_h2f(s.r[2 * h + 1]); d.r[4 * h + 3] = __das_h2f(s.r[2 * h + 1] >> 16); \} return d; \}"

//! the element size of the buffer a tile loads from or stores to: a member buffer, a shared array or a pointer local
def private coop_buf_elem_bytes(t : TypeDecl?) : uint64 {
    if (t == null) {
        return 0ul
    }
    if (t.baseType == Type.tArray || t.baseType == Type.tFixedArray || t.baseType == Type.tPointer) {
        return cuda_elem_bytes(t.firstType)
    }
    return 0ul
}

//! a compile-time integer: a literal, or a global constant's initializer (`COOPMAT_REDUCE_ROW` arrives unfolded)
def private const_int_of(e : Expression?) : int {
    if (e == null) {
        return -1
    }
    if (e is ExprConstInt) {
        return (e as ExprConstInt).value
    } elif (e is ExprConstUInt) {
        return int((e as ExprConstUInt).value)
    } elif (e is ExprRef2Value) {
        return const_int_of((e as ExprRef2Value).subexpr)
    } elif (e is ExprVar) {
        let ev = e as ExprVar
        if (!(ev.varFlags.local || ev.varFlags.argument || ev.varFlags._block) && ev.variable != null) {
            return const_int_of(ev.variable.init)
        }
    }
    return -1
}

//! coopmatLoad(dst, src, idx, stride, layout) / coopmatStore(mat, dst, idx, stride, layout) / coopmatConvert(dst, src)
//! as statements; "" after err. idx and stride are in the buffer's elements, as the SPIR-V load reads them
def private emit_coop_stmt(var ctx : CudaCtx; ec : ExprCall?) : string {
    let cname = call_base_name("{ec.name}")
    let na = length(ec.arguments)
    need_coopmat(ctx)
    if (cname == "coopmatConvert") {
        if (na != 2) {
            shader_error(ctx, ec.at, "coopmatConvert expects (dst, src)")
            return ""
        }
        let wd = wg_type_of(ctx, ec.arguments[0]._type)
        let ws = wg_type_of(ctx, ec.arguments[1]._type)
        if (wd.ok && ws.ok) {
            return emit_wg_convert_stmt(ctx, ec, wd, ws)
        }
        let di = coop_type_of(ec.arguments[0]._type)
        let si = coop_type_of(ec.arguments[1]._type)
        if (!(di.ok && si.ok && di.use == "C" && si.use == "C" && di.comp == "f32" && si.comp == "f16")) {
            shader_error(ctx, ec.at, "coopmatConvert lowers the f16 accumulator to the f32 accumulator; {describe(ec.arguments[0]._type)} <- {describe(ec.arguments[1]._type)} has no CUDA form yet")
            return ""
        }
        register_helper(ctx, "{HELPER_RANK_COOPMAT_OPS}cvt_f32_f16", COOPMAT_CVT_F32_F16)
        census_kind(ctx, "call.coopmatConvert.f16.f32")
        return "{emit_value(ctx, ec.arguments[0])} = __das_cm_cvt_f32_f16({emit_value(ctx, ec.arguments[1])});"
    }
    let is_load = cname == "coopmatLoad"
    if (na != 5) {
        shader_error(ctx, ec.at, "{cname} expects ({is_load ? "dst, src" : "mat, dst"}, idx, stride, layout)")
        return ""
    }
    let ci = coop_type_of(ec.arguments[0]._type)
    if (!ci.ok || (is_load ? ci.use == "C" : ci.use != "C")) {
        shader_error(ctx, ec.at, "{cname}: the first argument must be a {is_load ? "KHR A or B tile" : "KHR accumulator tile"} local, not {describe(ec.arguments[0]._type)}")
        return ""
    }
    let eb = coop_buf_elem_bytes(ec.arguments[1]._type)
    if (eb == 0ul) {
        shader_error(ctx, ec.at, "{cname}: the buffer argument must be an @ssbo array, a @workgroup array or a pointer of a scalar/vector element type, not {describe(ec.arguments[1]._type)}")
        return ""
    }
    let lay = const_int_of(ec.arguments[4])
    if (lay != 0 && lay != 1) {
        shader_error(ctx, ec.at, "{cname}: layout must be a constant 0 (row-major) or 1 (column-major)")
        return ""
    }
    let row = lay == 0
    let key = "{HELPER_RANK_COOPMAT_OPS}{is_load ? "ld" : "st"}_{ci.use}_{ci.comp}_{row ? "row" : "col"}"
    register_helper(ctx, key, is_load ? coop_load_helper(ci, row) : coop_store_helper(ci, row))
    let tile = emit_value(ctx, ec.arguments[0])
    let buf = emit_value(ctx, ec.arguments[1])
    let idx = emit_value(ctx, ec.arguments[2])
    let stride = emit_value(ctx, ec.arguments[3])
    census_kind(ctx, "call.{cname}.{ci.use}.{ci.comp}.{row ? "row" : "col"}")
    let fn = is_load ? "__das_cm_ld_{ci.use}_{ci.comp}" : "__das_cm_st_C_{ci.comp}"
    return "{fn}_{row ? "row" : "col"}({tile}, ({is_load ? "const " : ""}char*)({buf} + ({idx})), int({stride}) * {int(eb)});"
}

//! coopmatMulAdd(a, b, c) as a value: the tile product helper for the operand pair
def private emit_coop_mul_add(var ctx : CudaCtx; ec : ExprCall?) : string {
    if (length(ec.arguments) != 3) {
        shader_error(ctx, ec.at, "coopmatMulAdd expects (a, b, c)")
        return "/*coopmatMulAdd*/"
    }
    let ai = coop_type_of(ec.arguments[0]._type)
    let bi = coop_type_of(ec.arguments[1]._type)
    let ci = coop_type_of(ec.arguments[2]._type)
    if (!(ai.ok && bi.ok && ci.ok && ai.use == "A" && bi.use == "B" && ci.use == "C" && ai.comp == bi.comp
            && ((ai.comp == "f16" && (ci.comp == "f32" || ci.comp == "f16")) || (ai.comp == "s8" && ci.comp == "s32")))) {
        shader_error(ctx, ec.at, "coopmatMulAdd: ({describe(ec.arguments[0]._type)}, {describe(ec.arguments[1]._type)}, {describe(ec.arguments[2]._type)}) is not a KHR (A, B, accumulator) triple the tensor cores serve")
        return "/*coopmatMulAdd*/"
    }
    need_coopmat(ctx)
    register_helper(ctx, "{HELPER_RANK_COOPMAT_OPS}mma_{ai.comp}_{ci.comp}", coop_mma_helper(ai, ci))
    census_kind(ctx, "call.coopmatMulAdd.{ai.comp}.{ci.comp}")
    return "__das_mma_{ai.comp}_{ci.comp}({emit_value(ctx, ec.arguments[0])}, {emit_value(ctx, ec.arguments[1])}, {emit_value(ctx, ec.arguments[2])})"
}

//! a source position as plain numbers (the line info itself is a handle no table or array holds)
struct private SrcPos {
    line : uint
    column : uint
}

def private pos_of(at : LineInfo) : SrcPos => SrcPos(line = at.line, column = at.column)

struct private LoopSpan {
    start : SrcPos
    end : SrcPos
}

//! a shared tile local's life: declared at `decl`, last read at `last` (a loop a read sits in extends it to the loop's end);
//! a convert target is written under a warp sync and so never takes another tile's slot; `ones` = filled with 1.0 and never
//! loaded, `other_use` = read anywhere but as a product's B; a name two declarations share is `ambiguous` and gets neither form
struct private TileLife {
    decl : SrcPos
    last : SrcPos
    convert_target : bool
    ones : bool
    other_use : bool
    ones_feeds_row_owned : bool     //! every product over it lands in an accumulator whose fragment rows the warps divide
    ambiguous : bool
    global_view_written : bool      //! a tensor load from a 16-bit source writes it
    global_view_blocked : bool      //! anything but a 16-bit tensor load or a product's B operand touches it, or it is not an f16 B tile
    read_whole_by_row_owner : bool  //! a product over it lands in an accumulator whose fragment rows equal the warps
    decoded : bool                  //! a decode-in-load of the pipelined form writes it: two f16 slots behind a pointer, its blocks through the raw slot
    raw_s8 : bool                   //! an s8 tile a tensor load fills off an int8 plane: three slots behind a pointer, the windows ahead copied by cp.async
}

//! a carved shared slot and the tile holding it
struct private SmemSlot {
    off : uint64
    bytes : uint64
    owner : string
}

def private before(a, b : SrcPos) : bool => a.line < b.line || (a.line == b.line && a.column < b.column)

//! whether a decode-in-load call takes the pipelined form (`raw_decode_text`): an f16 A tile, the layout's block size a
//! compile-time (1, b1) with b1 a multiple of sixteen dividing the tile's k, a block of one byte an element
[arch(at="../ARCHITECTURE_EMIT.md#cuda-decoded-tile-pipeline")]
def private can_pipeline_decode(var ctx : CudaCtx; ecs : ExprCall?) : bool {
    return false if (length(ecs.arguments) < 9)
    let wi = wg_type_of(ctx, ecs.arguments[0]._type)
    return false if (!wi.ok || wi.use != "A" || !wi.in_smem || wi.comp != "f16")
    let ln = tile_var_name(ecs.arguments[3])
    return false if (empty(ln) || !key_exists(ctx.scan_layout_block_size, ln))
    let bs = ctx.scan_layout_block_size[ln]
    return false if (bs.x != 1 || bs.y % 16 != 0 || bs.y > wi.cols || wi.cols % bs.y != 0)
    let bt = ecs.arguments[1]._type
    return false if (bt == null || bt.baseType != Type.tArray || bt.firstType == null || bt.firstType.baseType != Type.tStructure)
    let blkT = elem_struct(ctx, bt.firstType.structType, ecs.at)
    return !empty(blkT) && ctx.elem_structs[blkT].size == bs.y
}

//! the shared tile local an expression names, "" for any other expression
def private tile_var_name(e : Expression?) : string {
    var x = e
    if (x != null && x is ExprRef2Value) {
        x = (x as ExprRef2Value).subexpr
    }
    let ev = x ?as ExprVar
    return ev != null && ev.varFlags.local ? string(ev.name) : ""
}

struct private WgInfo {
    ok : bool
    use : string        //! "A", "B" or "C"
    comp : string       //! f16, s8, f32, s32
    rows : int
    cols : int
    ctype : string
    elem : string       //! the shared element spelling (A, B)
    in_smem : bool      //! the tile lives in shared memory (A, B), not in the warps' registers (C)
    row_stride : int    //! a shared tile's row stride in elements: the contiguous extent plus a 16-byte pad against bank conflicts
    padded_elems : int  //! a shared tile's elements with the pad: A [rows][row_stride], B [cols][row_stride], k contiguous in both
    frag_rows : int     //! fragment rows (C)
    frag_cols : int     //! fragment columns (C)
    frags : int
    slots : int         //! fragments a warp holds
    row_own : bool      //! the slot map (C) a warp owns a fragment row under, else a fragment column
    k_major : bool      //! a B tile stored [k][n] (n contiguous), read through the transposing ldmatrix
    global_view : bool  //! a B tile the product gathers per warp from its global source, with no shared copy
}

//! the accumulator's fragment rows equal the warps: one fragment row a warp
def private row_per_warp(wi : WgInfo; warps : int) : bool => wi.frag_rows == warps

//! a warp owns a whole fragment row of the accumulator
def private warp_owns_row(wi : WgInfo; warps : int) : bool => wi.row_own && row_per_warp(wi, warps)

//! the global-view form of a B tile's info: the view record replaces the shared array
def private as_global_view(wi : WgInfo; global_view : bool) : WgInfo {
    var w = wi
    if (global_view && wi.use == "B" && !wi.k_major) {
        w.global_view = true
        w.in_smem = false
        w.padded_elems = 0
        w.ctype = "{w.ctype}_gv"
    }
    return w
}

//! the [k][n] form of a B tile's info
def private as_k_major(wi : WgInfo; k_major : bool) : WgInfo {
    var w = wi
    if (k_major && wi.use == "B") {
        w.k_major = true
        w.row_stride = w.cols + 8
        w.padded_elems = w.rows * w.row_stride
        w.ctype = "{w.ctype}_kn"
    }
    return w
}

//! the tile info of an expression: a B tile local the scan found loaded under plain views alone takes the [k][n] form
def private wg_var_info(ctx : CudaCtx; e : Expression?) : WgInfo {
    let wi = wg_type_of(ctx, e != null ? e._type : null)
    return wi if (!wi.ok || wi.use != "B" || e == null)
    var x = e
    if (x is ExprRef2Value) {
        x = (x as ExprRef2Value).subexpr
    }
    let ev = x ?as ExprVar
    return wi if (ev == null)
    return wg_var_info_named(ctx, string(ev.name), e._type)
}

//! the same by a local's name, for its declaration
def private wg_var_info_named(ctx : CudaCtx; name : string; t : TypeDecl?) : WgInfo {
    let wi = wg_type_of(ctx, t)
    return wi if (!wi.ok || wi.use != "B")
    return as_global_view(as_k_major(wi, key_exists(ctx.k_major_tiles, name)), key_exists(ctx.global_view_tiles, name))
}

//! the fragment coordinates of `__f` under the tile's slot map
def private wg_coords(wi : WgInfo) : string =>
    wi.row_own ? "int __i = __f % {wi.frag_rows}, __j = __f / {wi.frag_rows};" : "int __i = __f / {wi.frag_cols}, __j = __f % {wi.frag_cols};"

//! `coopmatWg{A|B|Acc}_{comp}_{R}x{C}` -> its shape; `warps` sizes an accumulator's per-warp slots, `row_own` its slot map
def private wg_info_of(name : string; warps : int; row_own : bool) : WgInfo {
    var wi = WgInfo(ok = false)
    return wi if (!(name |> starts_with("coopmatWg")))
    var rest = name |> slice(9)
    if (rest |> starts_with("Acc_")) {
        wi.use = "C"
        rest = rest |> slice(4)
    } elif (rest |> starts_with("A_")) {
        wi.use = "A"
        rest = rest |> slice(2)
    } elif (rest |> starts_with("B_")) {
        wi.use = "B"
        rest = rest |> slice(2)
    } else {
        return wi
    }
    let parts <- split(rest, "_")
    return wi if (length(parts) != 2)
    wi.comp = parts[0]
    let dims <- split(parts[1], "x")
    return wi if (length(dims) != 2)
    wi.rows = to_int(dims[0])
    wi.cols = to_int(dims[1])
    return wi if (wi.rows <= 0 || wi.cols <= 0 || (wi.rows % 16) != 0 || (wi.cols % 16) != 0)
    if (wi.use == "C") {
        return wi if (!(wi.comp == "f32" || wi.comp == "f16" || wi.comp == "s32") || warps <= 0)
        wi.frag_rows = wi.rows / 16
        wi.frag_cols = wi.cols / 16
        wi.frags = wi.frag_rows * wi.frag_cols
        wi.slots = (wi.frags + warps - 1) / warps
        wi.row_own = row_own
        wi.ctype = "__das_wgC_{wi.comp}_{wi.rows}x{wi.cols}_w{warps}{row_own ? "r" : ""}"
    } else {
        return wi if (!(wi.comp == "f16" || wi.comp == "s8"))
        wi.elem = wi.comp == "f16" ? "ushort" : "char"
        wi.in_smem = true
        let pad = wi.comp == "f16" ? 8 : 16
        wi.row_stride = (wi.use == "A" ? wi.cols : wi.rows) + pad
        wi.padded_elems = (wi.use == "A" ? wi.rows : wi.cols) * wi.row_stride
        wi.ctype = "__das_wg{wi.use}_{wi.comp}_{wi.rows}x{wi.cols}"
    }
    wi.ok = true
    return wi
}

//! the k extent of a B tile type name, 0 for any other
def private wg_b_rows(name : string; warps : int) : int {
    let wi = wg_info_of(name, warps, false)
    return wi.ok && wi.use == "B" ? wi.rows : 0
}

def private wg_type_of(ctx : CudaCtx; t : TypeDecl?) : WgInfo {
    if (t == null || t.baseType != Type.tStructure || t.structType == null) {
        return WgInfo(ok = false)
    }
    return wg_info_of(string(t.structType.name), ctx.warps, ctx.row_own)
}

def private is_tensor_layout(t : TypeDecl?) : bool =>
    t != null && t.baseType == Type.tStructure && t.structType != null && (string(t.structType.name) |> starts_with("tensorLayout2D"))

def private is_tensor_view(t : TypeDecl?) : bool =>
    t != null && t.baseType == Type.tStructure && t.structType != null && (string(t.structType.name) |> starts_with("tensorView2D"))

//! the warp and thread indices, the f32 -> f16 convert, an accumulator fragment's element access by
//! index k (0..7: n-half k / 4, the row half k & 2, the column pair bit k & 1), the tensor objects
let private CM2_PRELUDE = "__device__ __forceinline__ int __das_tid() \{ return int(threadIdx.x + threadIdx.y * blockDim.x + threadIdx.z * blockDim.x * blockDim.y); \}
__device__ __forceinline__ int __das_nthreads() \{ return int(blockDim.x * blockDim.y * blockDim.z); \}
__device__ __forceinline__ int __das_warp() \{ return __das_tid() >> 5; \}
__device__ __forceinline__ uint __das_f2h(float f) \{ unsigned short h; asm(\"cvt.rn.f16.f32 %0, %1;\" : \"=h\"(h) : \"f\"(f)); return uint(h); \}
__device__ __forceinline__ int __das_cm_row(int i, int g, int k) \{ return i * 16 + g + ((k & 2) ? 8 : 0); \}
__device__ __forceinline__ int __das_cm_col(int j, int t, int k) \{ return j * 16 + (k >> 2) * 8 + 2 * t + (k & 1); \}
__device__ __forceinline__ float __das_cmC_get_f32(const __das_cmC_f32& m, int k) \{ return m.r[k]; \}
__device__ __forceinline__ void __das_cmC_set_f32(__das_cmC_f32& m, int k, float v) \{ m.r[k] = v; \}
__device__ __forceinline__ int __das_cmC_get_s32(const __das_cmC_s32& m, int k) \{ return m.r[k]; \}
__device__ __forceinline__ void __das_cmC_set_s32(__das_cmC_s32& m, int k, int v) \{ m.r[k] = v; \}
__device__ __forceinline__ float __das_cmC_get_f16(const __das_cmC_f16& m, int k) \{ uint w = m.r[(k >> 2) * 2 + ((k >> 1) & 1)]; return __das_h2f((k & 1) ? (w >> 16) : w); \}
__device__ __forceinline__ void __das_cmC_set_f16(__das_cmC_f16& m, int k, float v) \{ uint& w = m.r[(k >> 2) * 2 + ((k >> 1) & 1)]; uint h = __das_f2h(v); w = (k & 1) ? ((w & 0xffffu) | (h << 16)) : ((w & 0xffff0000u) | h); \}
__device__ __forceinline__ __das_cmC_f16 __das_cm_cvt_f16_f32(const __das_cmC_f32& s) \{ __das_cmC_f16 d; for (int h = 0; h < 2; ++h) \{ d.r[2 * h] = __das_f2h(s.r[4 * h]) | (__das_f2h(s.r[4 * h + 1]) << 16); d.r[2 * h + 1] = __das_f2h(s.r[4 * h + 2]) | (__das_f2h(s.r[4 * h + 3]) << 16); \} return d; \}
struct __das_tlayout \{ uint d0, d1, s0, s1, b0, b1; int pad; \};
struct __das_tview \{ int t; \};"

def private need_cm2(var ctx : CudaCtx) {
    need_coopmat(ctx)
    register_helper(ctx, "{HELPER_RANK_CM2}cm2", CM2_PRELUDE)
    register_helper(ctx, "{HELPER_RANK_COOPMAT_OPS}cvt_f32_f16", COOPMAT_CVT_F32_F16)
    census_kind(ctx, "prelude.cm2")
}

//! the tile struct, declared once: a shared element array (A, B) or the per-warp fragment slots (C)
def private need_wg(var ctx : CudaCtx; wi : WgInfo) {
    need_cm2(ctx)
    let key = "{HELPER_RANK_WG_TILE}wg.{wi.ctype}"
    return if (key_exists(ctx.helpers, key))
    if (wi.global_view) {
        ctx.helpers[key] = "struct {wi.ctype} \{ const ushort* p; uint s0, s1, d0, d1, r0, c0; int fast; \};"
        return
    }
    if (ctx.kperm && wi.in_smem && wi.comp == "f16") {
        register_helper(ctx, "{HELPER_RANK_COOPMAT_OPS}kperm", WG_KPERM_PRELUDE)
    }
    ctx.helpers[key] = (wi.in_smem ?
        "struct {wi.ctype} \{ {wi.elem} e[{wi.padded_elems}]; \};" :
        "struct {wi.ctype} \{ __das_cmC_{wi.comp} fr[{wi.slots}]; \};")
}

//! the k permutation of a kernel whose B tiles are global views
let private WG_KPERM_PRELUDE = "__device__ __forceinline__ int __das_kp(int k) \{ return (k & ~31) | ((k & 4) << 2) | ((k & 2) << 2) | ((k & 24) >> 2) | (k & 1); \}"

//! the k coordinate as the tile stores it: permuted under kp
[arch(at="../ARCHITECTURE_EMIT.md#cuda-k-permutation")]
def private wg_k_at(kp : bool; k : string) : string => kp ? "__das_kp({k})" : k

//! the element index of (k, o) in a shared tile, o the other coordinate (an A tile's row, a B tile's column)
def private wg_tile_at(wi : WgInfo; k, o : string) : string =>
    wi.use == "B" && wi.k_major ? "({k}) * {wi.row_stride} + ({o})" : "({o}) * {wi.row_stride} + ({k})"

//! a B tile as a view of its source: the window's base pointer, the layout's strides, dimensions and origin, and whether the
//! fast gather applies - a unit column stride, the whole window inside the bounds (or no pad clamp), a row stride, origin
//! and base at 16-byte multiples, so a lane's eight k are one aligned 16-byte load
def private wg_view_helper(wi : WgInfo; srcT : string) : string {
    let name = "__das_wg_view_{wi.ctype}_{srcT}"
    return "__device__ __forceinline__ void {name}({wi.ctype}& m, const {srcT}* src, const __das_tlayout& tl, uint r0, uint c0) \{ m.p = (const ushort*)src; m.s0 = tl.s0; m.s1 = tl.s1; m.d0 = tl.d0; m.d1 = tl.d1; m.r0 = r0; m.c0 = c0; m.fast = (tl.s1 == 1u && (tl.s0 & 7u) == 0u && (c0 & 7u) == 0u && ((((size_t)src) & 15) == 0) && (!tl.pad || (r0 + {wi.cols}u <= tl.d0 && c0 + {wi.rows}u <= tl.d1))) ? 1 : 0; \}"
}

//! the B fragments of a view over the 32 k at k0 and the 16 n at n0 - the two k steps' fragments at once: under the
//! permutation lane (g, t) holds the elements k0 + 8t .. k0 + 8t + 7 at n = g and at n = g + 8 (tile (k, n) is the source's
//! (r0 + n, c0 + k)), one 16-byte load each; the slow path clamps every element to the bounds and honors the column stride
def private wg_gld_helper(wi : WgInfo) : string {
    let name = "__das_wg_gld_{wi.ctype}"
    let el = "__device__ __forceinline__ uint __das_wg_gel_{wi.ctype}(const {wi.ctype}& b, int k, int n) \{ uint sr = b.r0 + uint(n), sc = b.c0 + uint(k); return (sr < b.d0 && sc < b.d1) ? uint(b.p[sr * b.s0 + sc * b.s1]) : 0u; \}"
    let slow = "_Pragma(\"unroll\") for (int kk = 0; kk < 2; ++kk) \{ _Pragma(\"unroll\") for (int q = 0; q < 2; ++q) \{ int k = k0 + 8 * t + 4 * kk + 2 * q; m[kk].r[q] = __das_wg_gel_{wi.ctype}(b, k, n0 + g) | (__das_wg_gel_{wi.ctype}(b, k + 1, n0 + g) << 16); m[kk].r[2 + q] = __das_wg_gel_{wi.ctype}(b, k, n0 + g + 8) | (__das_wg_gel_{wi.ctype}(b, k + 1, n0 + g + 8) << 16); \} \}"
    let fast = "const ushort* p = b.p + (b.r0 + uint(n0 + g)) * b.s0 + b.c0 + uint(k0 + 8 * t); uint4 w0 = __ldg((const uint4*)p); uint4 w1 = __ldg((const uint4*)(p + 8 * b.s0)); m[0].r[0] = w0.x; m[0].r[1] = w0.y; m[1].r[0] = w0.z; m[1].r[1] = w0.w; m[0].r[2] = w1.x; m[0].r[3] = w1.y; m[1].r[2] = w1.z; m[1].r[3] = w1.w;"
    return "{el}\n__device__ __forceinline__ void {name}(__das_cmB_f16* m, const {wi.ctype}& b, int k0, int n0) \{ int l = __das_lane(), g = l >> 2, t = l & 3; if (b.fast) \{ {fast} \} else \{ {slow} \} \}"
}

//! the scalar spelling of an accumulator's component
def private wg_scalar(comp : string) : string => comp == "s32" ? "int" : "float"

//! an element read from a buffer of `srcT` as the tile component `comp`: "" when the pair has no form
def private convert_in(srcT, comp : string; x : string) : string {
    if (comp == "f32" || comp == "f16") {
        if (srcT == "half") {
            return comp == "f32" ? "__das_h2f(uint(({x}).bits))" : "uint(({x}).bits)"
        } elif (srcT == "ushort") {
            return comp == "f32" ? "__das_h2f(uint({x}))" : "uint({x})"
        } elif (srcT == "float") {
            return comp == "f32" ? "({x})" : "__das_f2h({x})"
        }
        return ""
    } elif (comp == "s8") {
        return srcT == "char" || srcT == "uchar" ? "({x})" : ""
    } elif (comp == "s32") {
        return srcT == "int" ? "({x})" : ""
    }
    return ""
}

//! a store of an accumulator element `v` (the scalar of `comp`) into a buffer of `dstT`: "" when no form
def private convert_out(dstT, comp : string; at, v : string) : string {
    if (comp == "f32" || comp == "f16") {
        if (dstT == "half") {
            return "((ushort*)dst)[{at}] = (ushort)__das_f2h({v});"
        } elif (dstT == "ushort") {
            return "dst[{at}] = (ushort)__das_f2h({v});"
        } elif (dstT == "float") {
            return "dst[{at}] = {v};"
        }
        return ""
    } elif (comp == "s32") {
        return dstT == "int" ? "dst[{at}] = {v};" : ""
    }
    return ""
}

//! the per-slot fragment loop head every accumulator walk opens: `s`, `f`, `i`, `j` in scope, plus the lane split
def private wg_slot_loop_head(wi : WgInfo; warps : int) : string =>
    "int __l = __das_lane(), __g = __l >> 2, __t = __l & 3, __w = __das_warp(); for (int __s = 0; __s < {wi.slots}; ++__s) \{ int __f = __s * {warps} + __w; if (__f >= {wi.frags}) break; {wg_coords(wi)}"

//! a block-wide walk of `total` elements, `per` a thread a trip, the trip's first element in `e`: where the block's threads
//! divide the walk, the trips run at a constant count unrolled two at a time - unrolling the whole walk hits the register
//! cap; otherwise the strided runtime loop
def private wg_grid_loop(total, per, warps : int; body : string) : string {
    let step = warps * 32 * per
    if (total % step == 0) {
        return "_Pragma(\"unroll 2\") for (int __i = 0; __i < {total / step}; ++__i) \{ int e = __das_tid() * {per} + __i * {step}; {body} \}"
    }
    return "for (int e = __das_tid() * {per}; e < {total}; e += {step}) \{ {body} \}"
}

//! a tensor load: a shared tile cooperatively (two barriers), an accumulator by its lanes; `trans`
//! reads the window transposed (the tensorView2Dt form); the pad clamp zero-fills out of bounds; `kp` stores the k permuted
def private wg_ldt_helper(wi : WgInfo; srcT : string; trans : bool; warps : int; kp : bool) : string {
    let name = "__das_wg_ldt_{wi.ctype}_{srcT}_{trans ? "t" : "n"}{kp ? "_kp" : ""}"
    let sr = trans ? "r0 + uint(c)" : "r0 + uint(r)"
    let sc = trans ? "c0 + uint(r)" : "c0 + uint(c)"
    let cv = convert_in(srcT, wi.comp, "x")
    let vt = wi.in_smem ? wi.elem : wg_scalar(wi.comp)
    let fetch = "{vt} v = ({vt})0; if (!tl.pad || (sr < tl.d0 && sc < tl.d1)) \{ {srcT} x = src[sr * tl.s0 + sc * tl.s1]; v = ({vt}){cv}; \}"
    if (wi.in_smem) {
        // the walk runs along the source's contiguous direction (its columns under a plain view, its rows under a transposed one)
        // so a warp's loads coalesce
        let walk = trans ? "int c = e / {wi.rows}, r = e % {wi.rows};" : "int r = e / {wi.cols}, c = e % {wi.cols};"
        let kv = wi.use == "A" ? "c" : "r"
        let ov = wi.use == "A" ? "r" : "c"
        let along_k = !(wi.use == "B" && wi.k_major)
        let scalar = wg_grid_loop(wi.rows * wi.cols, 1, warps, "{walk} uint sr = {sr}, sc = {sc}; {fetch} m.e[{wg_tile_at(wi, wg_k_at(kp, kv), ov)}] = v;")
        // where the source's contiguous direction is the tile's own, a flat, 16-byte-aligned, 8-multiple-stride window
        // copies 16 bytes a thread; under a pad layout a run at the edge goes element by element
        let e16 = wi.comp == "f16" ? 8 : 16   // the elements a 16-byte run holds
        let vec16 = (wi.comp == "f16" && (srcT == "half" || srcT == "ushort")) || (wi.comp == "s8" && (srcT == "char" || srcT == "uchar"))
        let vec = vec16 && (trans == (wi.use == "B" && !wi.k_major))
        let at_q = along_k ? wg_tile_at(wi, wg_k_at(kp, "{kv} + q"), ov) : wg_tile_at(wi, wg_k_at(kp, kv), "{ov} + q")
        let edge = "for (int q = 0; q < {e16}; ++q) \{ uint sq = sc + uint(q); {vt} v = ({vt})0; if (sr < tl.d0 && sq < tl.d1) \{ {srcT} x = src[sr * tl.s0 + sq]; v = ({vt}){cv}; \} m.e[{at_q}] = v; \}"
        let run_at = wg_tile_at(wi, "(({kv}) & ~31) + ((({kv}) >> 2) & 6)", ov)
        let copy = (along_k && kp ?
            "uint4 __w = *(const uint4*)(src + sr * tl.s0 + sc); uint* __d = (uint*)&m.e[{run_at}]; __d[0] = __w.x; __d[4] = __w.y; __d[8] = __w.z; __d[12] = __w.w;" :
            "*(uint4*)&m.e[{wg_tile_at(wi, wg_k_at(kp, kv), ov)}] = *(const uint4*)(src + sr * tl.s0 + sc);")
        let vec_loop = wg_grid_loop(wi.rows * wi.cols, e16, warps, "{walk} uint sr = {sr}, sc = {sc}; if (!tl.pad || (sr < tl.d0 && sc + {e16}u <= tl.d1)) \{ {copy} \} else \{ {edge} \}")
        let fast = vec ? "if (tl.s1 == 1u && (tl.s0 & {e16 - 1}u) == 0u && ((((size_t)src) & 15) == 0)) \{ {vec_loop} \} else \{ {scalar} \}" : scalar
        return "__device__ __forceinline__ void {name}({wi.ctype}& m, const {srcT}* src, const __das_tlayout& tl, uint r0, uint c0) \{ __syncthreads(); {fast} __syncthreads(); \}"
    }
    return "__device__ __forceinline__ void {name}({wi.ctype}& m, const {srcT}* src, const __das_tlayout& tl, uint r0, uint c0) \{ {wg_slot_loop_head(wi, warps)} for (int __k = 0; __k < 8; ++__k) \{ int r = __das_cm_row(__i, __g, __k), c = __das_cm_col(__j, __t, __k); uint sr = {sr}, sc = {sc}; {fetch} __das_cmC_set_{wi.comp}(m.fr[__s], __k, v); \} \} \}"
}

//! a tensor store of an accumulator by its lanes, the pad clamp skipping out of bounds
def private wg_stt_helper(wi : WgInfo; dstT : string; trans : bool; warps : int) : string {
    let name = "__das_wg_stt_{wi.ctype}_{dstT}_{trans ? "t" : "n"}"
    let sr = trans ? "r0 + uint(c)" : "r0 + uint(r)"
    let sc = trans ? "c0 + uint(r)" : "c0 + uint(c)"
    let st = convert_out(dstT, wi.comp, "sr * tl.s0 + sc * tl.s1", "__das_cmC_get_{wi.comp}(m.fr[__s], __k)")
    return "__device__ __forceinline__ void {name}(const {wi.ctype}& m, {dstT}* dst, const __das_tlayout& tl, uint r0, uint c0) \{ {wg_slot_loop_head(wi, warps)} for (int __k = 0; __k < 8; ++__k) \{ int r = __das_cm_row(__i, __g, __k), c = __das_cm_col(__j, __t, __k); uint sr = {sr}, sc = {sc}; if (!tl.pad || (sr < tl.d0 && sc < tl.d1)) \{ {st} \} \} \} \}"
}

//! the f16 fragment loads off a shared tile through ldmatrix: A's four 8x8 matrices by (row, k-half) in the a0..a3 order the
//! mma reads, B's by (n-half, k-half) off the k-contiguous [n][k] tile - the mma's own B layout, so no transpose - a lane's
//! address is its row's 16-byte segment, which the tile's padded stride spreads over the bank groups
let private WG_LDMATRIX_PRELUDE = "__device__ __forceinline__ void __das_wg_ldm_A(__das_cmA_f16& m, const ushort* p, int ld) \{ int l = __das_lane(); uint a = (uint)__cvta_generic_to_shared(p + (l & 15) * ld + (l >> 4) * 8); asm volatile(\"ldmatrix.sync.aligned.m8n8.x4.shared.b16 \{%0,%1,%2,%3\}, [%4];\" : \"=r\"(m.r[0]), \"=r\"(m.r[1]), \"=r\"(m.r[2]), \"=r\"(m.r[3]) : \"r\"(a)); \}
__device__ __forceinline__ void __das_wg_ldm_B(__das_cmB_f16& m, const ushort* p, int ld) \{ int l = __das_lane(); int mt = l >> 3, r = l & 7; uint a = (uint)__cvta_generic_to_shared(p + ((mt >> 1) * 8 + r) * ld + (mt & 1) * 8); asm volatile(\"ldmatrix.sync.aligned.m8n8.x4.shared.b16 \{%0,%1,%2,%3\}, [%4];\" : \"=r\"(m.r[0]), \"=r\"(m.r[1]), \"=r\"(m.r[2]), \"=r\"(m.r[3]) : \"r\"(a)); \}
__device__ __forceinline__ void __das_wg_ldm_Bt(__das_cmB_f16& m, const ushort* p, int ld) \{ int l = __das_lane(); int mt = l >> 3, r = l & 7; uint a = (uint)__cvta_generic_to_shared(p + ((mt & 1) * 8 + r) * ld + (mt >> 1) * 8); asm volatile(\"ldmatrix.sync.aligned.m8n8.x4.trans.shared.b16 \{%0,%1,%2,%3\}, [%4];\" : \"=r\"(m.r[0]), \"=r\"(m.r[1]), \"=r\"(m.r[2]), \"=r\"(m.r[3]) : \"r\"(a)); \}"

//! the s8 fragment loads off a shared tile through ldmatrix, the rows as bytes: A's four 8x8 matrices by (row half, 16-byte k
//! half) in the a0..a3 order the m16n8k32 mma reads, B's by (n half, k half) off the k-contiguous [n][k] tile
let private WG_LDMATRIX_S8_PRELUDE = "__device__ __forceinline__ void __das_wg_ldm_A_s8(__das_cmA_s8& m, const char* p, int ld) \{ int l = __das_lane(); int mt = l >> 3, r = l & 7; uint a = (uint)__cvta_generic_to_shared(p + (r + 8 * (mt & 1)) * ld + (mt >> 1) * 16); asm volatile(\"ldmatrix.sync.aligned.m8n8.x4.shared.b16 \{%0,%1,%2,%3\}, [%4];\" : \"=r\"(m.r[0]), \"=r\"(m.r[1]), \"=r\"(m.r[2]), \"=r\"(m.r[3]) : \"r\"(a)); \}
__device__ __forceinline__ void __das_wg_ldm_B_s8(__das_cmB_s8& m, const char* p, int ld) \{ int l = __das_lane(); int mt = l >> 3, r = l & 7; uint a = (uint)__cvta_generic_to_shared(p + (r + 8 * (mt >> 1)) * ld + (mt & 1) * 16); asm volatile(\"ldmatrix.sync.aligned.m8n8.x4.shared.b16 \{%0,%1,%2,%3\}, [%4];\" : \"=r\"(m.r[0]), \"=r\"(m.r[1]), \"=r\"(m.r[2]), \"=r\"(m.r[3]) : \"r\"(a)); \}"

//! the tile product over the warp's own fragments: a k step loads each of the warp's A and B fragments once - its slots
//! `s` own the fragments f = s x warps + w, so with the fragment columns a multiple of the warp count a warp holds every
//! row at G = frag_cols / warps columns (G B fragments live across the row loop), and with the warp count a multiple of the
//! columns it holds one column at rows s x H + w / frag_cols (one B fragment a k step); any other grid walks a slot at a
//! time. f16 and s8 fragments both come through ldmatrix
def private wg_mma_helper(a, b, c : WgInfo; warps : int) : string {
    let ks = a.comp == "s8" ? 32 : 16
    let f16 = a.comp == "f16"
    let ksteps = a.cols / ks
    let mma = "__das_mma_{a.comp}_{c.comp}"
    // the A fragment of row `i` and the B fragment of column `j` at k step `kk`: a [k][n] B tile through the transposing ldmatrix
    let ldA = f16 ? "__das_wg_ldm_A(fa, &a.e[(__i * 16) * {a.row_stride} + kk * {ks}], {a.row_stride});" : "__das_wg_ldm_A_s8(fa, (const char*)&a.e[(__i * 16) * {a.row_stride} + kk * {ks}], {a.row_stride});"
    let ldB1 = b.k_major ? "__das_wg_ldm_Bt(fb, &b.e[(kk * {ks}) * {b.row_stride} + __j * 16], {b.row_stride});" : (f16 ? "__das_wg_ldm_B(fb, &b.e[(__j * 16) * {b.row_stride} + kk * {ks}], {b.row_stride});" : "__das_wg_ldm_B_s8(fb, (const char*)&b.e[(__j * 16) * {b.row_stride} + kk * {ks}], {b.row_stride});")
    let ldBs = b.k_major ? "__das_wg_ldm_Bt(fbs[jj], &b.e[(kk * {ks}) * {b.row_stride} + __j * 16], {b.row_stride});" : (f16 ? "__das_wg_ldm_B(fbs[jj], &b.e[(__j * 16) * {b.row_stride} + kk * {ks}], {b.row_stride});" : "__das_wg_ldm_B_s8(fbs[jj], (const char*)&b.e[(__j * 16) * {b.row_stride} + kk * {ks}], {b.row_stride});")
    // a view's B fragments come two k steps at a time (the 32 k a lane's 16-byte load covers): the k loop walks the
    // 32-blocks, gathers the warp's B fragments of the block, then runs both steps' A loads and products off them
    let gB1 = "__das_wg_gld_{b.ctype}(fb2, b, kk2 * 32, __j * 16);"
    let gBs = "__das_wg_gld_{b.ctype}(fbs2[jj], b, kk2 * 32, __j * 16);"
    let kh_loop = "_Pragma(\"unroll\") for (int kh = 0; kh < 2; ++kh) \{ int kk = kk2 * 2 + kh;"
    var body = ""
    if (warp_owns_row(c, warps)) {
        // a warp's row: its one A fragment a k step against every B fragment, slot s the column s
        body = (b.global_view ?
            "int __i = w; _Pragma(\"unroll\") for (int kk2 = 0; kk2 < {ksteps / 2}; ++kk2) \{ __das_cmA_{a.comp} fa2[2]; {kh_loop} __das_cmA_{a.comp}& fa = fa2[kh]; {ldA} \} _Pragma(\"unroll\") for (int __j = 0; __j < {c.frag_cols}; ++__j) \{ __das_cmB_{a.comp} fb2[2]; {gB1} c.fr[__j] = {mma}(fa2[0], fb2[0], c.fr[__j]); c.fr[__j] = {mma}(fa2[1], fb2[1], c.fr[__j]); \} \}" :
            "int __i = w; _Pragma(\"unroll\") for (int kk = 0; kk < {ksteps}; ++kk) \{ __das_cmA_{a.comp} fa; {ldA} _Pragma(\"unroll\") for (int __j = 0; __j < {c.frag_cols}; ++__j) \{ __das_cmB_{a.comp} fb; {ldB1} c.fr[__j] = {mma}(fa, fb, c.fr[__j]); \} \}")
    } elif (c.frags % warps == 0 && c.frag_cols % warps == 0 && !c.row_own) {
        let g = c.frag_cols / warps
        // a view's block is gathered one block ahead: the set the view carries serves the block it names (the step's first
        // block, gathered at the end of the product before), and the next block's gather - the step's second, or the next
        // step's first where the window reaches it - goes out before this block's products
        let products = "{kh_loop} _Pragma(\"unroll\") for (int __i = 0; __i < {c.frag_rows}; ++__i) \{ __das_cmA_{a.comp} fa; {ldA} _Pragma(\"unroll\") for (int jj = 0; jj < {g}; ++jj) \{ c.fr[__i * {g} + jj] = {mma}(fa, fbs2[jj][kh], c.fr[__i * {g} + jj]); \} \} \}"
        body = (b.global_view ?
            "_Pragma(\"unroll\") for (int kk2 = 0; kk2 < {ksteps / 2}; ++kk2) \{ __das_cmB_{a.comp} fbs2[{g}][2]; _Pragma(\"unroll\") for (int jj = 0; jj < {g}; ++jj) \{ int __j = w + {warps} * jj; {gBs} \} {products} \}" :
            "_Pragma(\"unroll\") for (int kk = 0; kk < {ksteps}; ++kk) \{ __das_cmB_{a.comp} fbs[{g}]; _Pragma(\"unroll\") for (int jj = 0; jj < {g}; ++jj) \{ int __j = w + {warps} * jj; {ldBs} \} _Pragma(\"unroll\") for (int __i = 0; __i < {c.frag_rows}; ++__i) \{ __das_cmA_{a.comp} fa; {ldA} _Pragma(\"unroll\") for (int jj = 0; jj < {g}; ++jj) \{ c.fr[__i * {g} + jj] = {mma}(fa, fbs[jj], c.fr[__i * {g} + jj]); \} \} \}")
    } elif (c.frags % warps == 0 && warps % c.frag_cols == 0 && !c.row_own) {
        let h = warps / c.frag_cols
        body = (b.global_view ?
            "int __j = w % {c.frag_cols}; _Pragma(\"unroll\") for (int kk2 = 0; kk2 < {ksteps / 2}; ++kk2) \{ __das_cmB_{a.comp} fb2[2]; {gB1} {kh_loop} _Pragma(\"unroll\") for (int s = 0; s < {c.slots}; ++s) \{ int __i = s * {h} + w / {c.frag_cols}; __das_cmA_{a.comp} fa; {ldA} c.fr[s] = {mma}(fa, fb2[kh], c.fr[s]); \} \} \}" :
            "int __j = w % {c.frag_cols}; _Pragma(\"unroll\") for (int kk = 0; kk < {ksteps}; ++kk) \{ __das_cmB_{a.comp} fb; {ldB1} _Pragma(\"unroll\") for (int s = 0; s < {c.slots}; ++s) \{ int __i = s * {h} + w / {c.frag_cols}; __das_cmA_{a.comp} fa; {ldA} c.fr[s] = {mma}(fa, fb, c.fr[s]); \} \}")
    } else {
        body = (b.global_view ?
            "for (int s = 0; s < {c.slots}; ++s) \{ int __f = s * {warps} + w; if (__f >= {c.frags}) break; {wg_coords(c)} for (int kk2 = 0; kk2 < {ksteps / 2}; ++kk2) \{ __das_cmB_{a.comp} fb2[2]; {gB1} {kh_loop} __das_cmA_{a.comp} fa; {ldA} c.fr[s] = {mma}(fa, fb2[kh], c.fr[s]); \} \} \}" :
            "for (int s = 0; s < {c.slots}; ++s) \{ int __f = s * {warps} + w; if (__f >= {c.frags}) break; {wg_coords(c)} for (int kk = 0; kk < {ksteps}; ++kk) \{ __das_cmA_{a.comp} fa; __das_cmB_{a.comp} fb; {ldA} {ldB1} c.fr[s] = {mma}(fa, fb, c.fr[s]); \} \}")
    }
    return "__device__ __forceinline__ {c.ctype} __das_wg_mma_{a.ctype}_{b.ctype}_{c.ctype}(const {a.ctype}& a, const {b.ctype}& b, {c.ctype} c) \{ int w = __das_warp(); {body} return c; \}"
}

//! the A tile's row sums into a row-owned accumulator (the product over a ones tile)
[arch(at="../ARCHITECTURE_EMIT.md#cuda-ones-row-sum")]
def private wg_rowsum_helper(a, c : WgInfo) : string {
    let quarter = a.cols / 4
    let rd = a.comp == "f16" ? "__das_h2f((uint)a.e[__row * {a.row_stride} + __t * {quarter} + __q])" : "(float)a.e[__row * {a.row_stride} + __t * {quarter} + __q]"
    let sc = wg_scalar(c.comp)
    let getv = "__das_cmC_get_{c.comp}"
    let setv = "__das_cmC_set_{c.comp}"
    return "__device__ __forceinline__ {c.ctype} __das_wg_rowsum_{a.ctype}_{c.ctype}(const {a.ctype}& a, {c.ctype} c) \{ int __l = __das_lane(), __g = __l >> 2, __t = __l & 3, __w = __das_warp(); float __r[2]; _Pragma(\"unroll\") for (int __h = 0; __h < 2; ++__h) \{ int __row = __w * 16 + __g + __h * 8; float __a = 0.0f; _Pragma(\"unroll\") for (int __q = 0; __q < {quarter}; ++__q) \{ __a += {rd}; \} __a += __shfl_xor_sync(0xffffffffu, __a, 1); __a += __shfl_xor_sync(0xffffffffu, __a, 2); __r[__h] = __a; \} _Pragma(\"unroll\") for (int __s = 0; __s < {c.slots}; ++__s) \{ _Pragma(\"unroll\") for (int __h = 0; __h < 2; ++__h) \{ _Pragma(\"unroll\") for (int __q = 0; __q < 4; ++__q) \{ int __k = __h * 2 + (__q & 1) + (__q >> 1) * 4; {setv}(c.fr[__s], __k, ({sc})(({sc}){getv}(c.fr[__s], __k) + ({sc})__r[__h])); \} \} \} return c; \}"
}

//! an element-wise walk over an accumulator's lanes: `body` sees the slot `__s`, the element `__k` and
//! the fragment coordinates (`__i`, `__j`, `__g`, `__t`); the double-underscore names cannot shadow a das local
def private wg_walk(wi : WgInfo; warps : int; body : string) : string =>
    "\{ {wg_slot_loop_head(wi, warps)} for (int __k = 0; __k < 8; ++__k) \{ {body} \} \} \}"

//! the (row, col) of the walked element
let private WG_ROW = "__das_cm_row(__i, __g, __k)"
let private WG_COL = "__das_cm_col(__j, __t, __k)"


//! the callback a `@@fn` argument names, through the reference wrap the call site adds
def private callback_fn(e : Expression?) : FunctionPtr {
    var x = e
    if (x != null && x is ExprRef2Value) {
        x = (x as ExprRef2Value).subexpr
    }
    let fa = x ?as ExprAddr
    return fa != null ? unsafe(reinterpret<FunctionPtr>(fa.func)) : null
}

//! a callback's call text over `args`, the hidden parameters appended as at every user-function call
def private ufn_call_text(var ctx : CudaCtx; fn : FunctionPtr; args : array<string>; at : LineInfo) : string {
    let ui = ctx.user_fn_index?[user_fn_key(fn)] ?? -1
    if (ui < 0) {
        let bn = call_base_name("{fn.name}")
        shader_error(ctx, at, "internal: callback `{bn}` was not discovered at scan time")
        return "/*callback*/"
    }
    var all := args
    let hidden <- hidden_args(ctx, ui)
    all |> push_from(hidden)
    return "{ctx.user_fns[ui].name}({join(all, ", ")})"
}

//! the tensor layout and view setters, as plain assignments on the local
def private emit_tensor_object_stmt(var ctx : CudaCtx; ec : ExprCall?; cname : string) : string {
    need_cm2(ctx)
    let na = length(ec.arguments)
    if (na < 1) {
        shader_error(ctx, ec.at, "{cname} takes the layout or view local first")
        return ""
    }
    let obj = emit_value(ctx, ec.arguments[0])
    let t0 = ec.arguments[0]._type
    if (cname == "tensorLayoutCreate") {
        if (!is_tensor_layout(t0)) {
            shader_error(ctx, ec.at, "tensorLayoutCreate takes a tensorLayout2D / tensorLayout2DPad local")
            return ""
        }
        let pad = t0.structType.name == "tensorLayout2DPad"
        census_kind(ctx, pad ? "call.tensorLayoutCreate.pad" : "call.tensorLayoutCreate")
        ctx.layout_block_size[obj] = int2(1, 1)
        return "{obj}.b0 = 1u; {obj}.b1 = 1u; {obj}.pad = {pad ? 1 : 0};"
    } elif (cname == "tensorViewCreate") {
        if (!is_tensor_view(t0)) {
            shader_error(ctx, ec.at, "tensorViewCreate takes a tensorView2D / tensorView2Dt local")
            return ""
        }
        let tr = t0.structType.name == "tensorView2Dt"
        census_kind(ctx, tr ? "call.tensorViewCreate.t" : "call.tensorViewCreate")
        return "{obj}.t = {tr ? 1 : 0};"
    }
    if (na != 3 || !is_tensor_layout(t0)) {
        shader_error(ctx, ec.at, "{cname} takes (layout, v0, v1)")
        return ""
    }
    let v0 = emit_value(ctx, ec.arguments[1])
    let v1 = emit_value(ctx, ec.arguments[2])
    if (cname == "tensorLayoutSetDimension") {
        census_kind(ctx, "call.tensorLayoutSetDimension")
        return "{obj}.d0 = {v0}; {obj}.d1 = {v1};"
    } elif (cname == "tensorLayoutSetStride") {
        census_kind(ctx, "call.tensorLayoutSetStride")
        return "{obj}.s0 = {v0}; {obj}.s1 = {v1};"
    }
    census_kind(ctx, "call.tensorLayoutSetBlockSize")
    let b0c = const_int_of(ec.arguments[1])
    let b1c = const_int_of(ec.arguments[2])
    if (b0c > 0 && b1c > 0) {
        ctx.layout_block_size[obj] = int2(b0c, b1c)
    } else {
        ctx.layout_block_size |> erase(obj)
    }
    return "{obj}.b0 = {v0}; {obj}.b1 = {v1};"
}

//! the element spelling of a tensor load's or store's buffer: an array, a fixed array or a pointer of a scalar; "" otherwise
def private tensor_buf_elem(bt : TypeDecl?) : string =>
    bt != null && (bt.baseType == Type.tArray || bt.baseType == Type.tPointer || bt.baseType == Type.tFixedArray) ? shader_type_name(bt.firstType) : ""

//! coopmatLoadTensor / coopmatStoreTensor (dst-or-mat, buf, base, tl, r0, nr, c0, nc [, view]): the window
//! is the whole tile; the view's TYPE decides the transpose
def private emit_wg_tensor_stmt(var ctx : CudaCtx; ec : ExprCall?; is_store : bool) : string {
    let cname = is_store ? "coopmatStoreTensor" : "coopmatLoadTensor"
    let na = length(ec.arguments)
    if (na != 8 && na != 9) {
        shader_error(ctx, ec.at, "{cname} expects ({is_store ? "mat, dst" : "dst, src"}, base, layout, r0, nr, c0, nc [, view])")
        return ""
    }
    let wi = wg_var_info(ctx, ec.arguments[0])
    if (!wi.ok) {
        shader_error(ctx, ec.at, "{cname}: the first argument must be a workgroup-scope coopmatWg* local, not {describe(ec.arguments[0]._type)}")
        return ""
    }
    if (wi.k_major && !is_store) {
        var dx = ec.arguments[0]
        if (dx is ExprRef2Value) {
            dx = (dx as ExprRef2Value).subexpr
        }
        let dv = dx ?as ExprVar
        if (dv != null && key_exists(ctx.n_major_tiles, string(dv.name))) {
            shader_error(ctx, ec.at, "coopmatLoadTensor: the B tile {dv.name} is loaded under a plain view here and under the transposed view elsewhere - one tile keeps one store")
            return ""
        }
    }
    if (is_store && wi.in_smem) {
        shader_error(ctx, ec.at, "coopmatStoreTensor stores an accumulator tile; an A or B tile is read-only")
        return ""
    }
    let bt = ec.arguments[1]._type
    let elemT = tensor_buf_elem(bt)
    if (empty(elemT)) {
        shader_error(ctx, ec.at, "{cname}: the buffer must be an @ssbo array, a @workgroup array or a pointer of a scalar element, not {describe(bt)}")
        return ""
    }
    if (!is_tensor_layout(ec.arguments[3]._type)) {
        shader_error(ctx, ec.at, "{cname}: the fourth argument must be a tensorLayout2D / tensorLayout2DPad local")
        return ""
    }
    var trans = false
    if (na == 9) {
        let vt = ec.arguments[8]._type
        if (!is_tensor_view(vt)) {
            shader_error(ctx, ec.at, "{cname}: the trailing argument must be a tensorView2D / tensorView2Dt local")
            return ""
        }
        trans = vt.structType.name == "tensorView2Dt"
    }
    if (empty(is_store ? convert_out(elemT, wi.comp, "0", "0") : convert_in(elemT, wi.comp, "x"))) {
        shader_error(ctx, ec.at, "{cname}: a buffer of {elemT} has no conversion to the tile's {wi.comp} component")
        return ""
    }
    need_wg(ctx, wi)
    let tile = emit_value(ctx, ec.arguments[0])
    let buf = emit_value(ctx, ec.arguments[1])
    let base = emit_value(ctx, ec.arguments[2])
    let tl = emit_value(ctx, ec.arguments[3])
    let r0 = emit_value(ctx, ec.arguments[4])
    let c0 = emit_value(ctx, ec.arguments[6])
    if (!is_store && wi.comp == "s8") {
        let raw = raw_s8_text(ctx, ec, wi, elemT, trans, buf, base, tl, r0, c0)
        if (!empty(raw)) {
            return raw
        }
    }
    if (wi.global_view) {
        register_helper(ctx, "{HELPER_RANK_WG_ACCESS}view.{wi.ctype}.{elemT}", wg_view_helper(wi, elemT))
        census_kind(ctx, "call.{cname}.{wi.use}.{wi.comp}.{elemT}.gv{wi.k_major ? ".kn" : ".t"}")
        return "__das_wg_view_{wi.ctype}_{elemT}({tile}, {buf} + ({base}), {tl}, uint({r0}), uint({c0}));"
    }
    let kp = ctx.kperm && wi.in_smem && wi.comp == "f16"
    let key = "{HELPER_RANK_WG_ACCESS}{is_store ? "stt" : "ldt"}.{wi.ctype}.{elemT}.{trans ? "t" : "n"}{kp ? ".kp" : ""}"
    register_helper(ctx, key, is_store ? wg_stt_helper(wi, elemT, trans, ctx.warps) : wg_ldt_helper(wi, elemT, trans, ctx.warps, kp))
    census_kind(ctx, "call.{cname}.{wi.use}.{wi.comp}.{elemT}{trans ? ".t" : (wi.k_major ? ".kn" : "")}{kp ? ".kp" : ""}")
    let fn = "__das_wg_{is_store ? "stt" : "ldt"}_{wi.ctype}_{elemT}_{trans ? "t" : "n"}{kp ? "_kp" : ""}"
    return "{fn}({tile}, {buf} + ({base}), {tl}, uint({r0}), uint({c0}));"
}

//! coopmatConvert on workgroup tiles: accumulator widths both ways by fragment, an accumulator into a shared A tile by its lanes
def private emit_wg_convert_stmt(var ctx : CudaCtx; ec : ExprCall?; di, si : WgInfo) : string {
    need_wg(ctx, di)
    need_wg(ctx, si)
    let dst = emit_value(ctx, ec.arguments[0])
    let src = emit_value(ctx, ec.arguments[1])
    if (di.rows != si.rows || di.cols != si.cols) {
        shader_error(ctx, ec.at, "coopmatConvert: {describe(ec.arguments[0]._type)} <- {describe(ec.arguments[1]._type)} changes the tile shape")
        return ""
    }
    if (di.use == "C" && si.use == "C" && di.comp != si.comp && di.comp != "s32" && si.comp != "s32") {
        let fn = di.comp == "f32" ? "__das_cm_cvt_f32_f16" : "__das_cm_cvt_f16_f32"
        census_kind(ctx, "call.coopmatConvert.wg.{si.comp}.{di.comp}")
        return "for (int __s = 0; __s < {di.slots}; ++__s) \{ {dst}.fr[__s] = {fn}({src}.fr[__s]); \}"
    }
    if (di.use == "A" && di.comp == "f16" && si.use == "C" && si.comp != "s32") {
        let walk = wg_walk(si, ctx.warps, "{dst}.e[{WG_ROW} * {di.row_stride} + {wg_k_at(ctx.kperm, WG_COL)}] = (ushort)__das_f2h(__das_cmC_get_{si.comp}({src}.fr[__s], __k));")
        if (warp_owns_row(si, ctx.warps)) {   // a warp writes its own fragment row of the tile and the product reads that row alone: a warp sync
            census_kind(ctx, "call.coopmatConvert.wg.{si.comp}.A.warp")
            return "__syncwarp(); " + walk + " __syncwarp();"
        }
        census_kind(ctx, "call.coopmatConvert.wg.{si.comp}.A")
        return "__syncthreads(); " + walk + " __syncthreads();"
    }
    shader_error(ctx, ec.at, "coopmatConvert: {describe(ec.arguments[0]._type)} <- {describe(ec.arguments[1]._type)} has no CUDA form (the accumulator widths, or an accumulator into an f16 A tile)")
    return ""
}

//! coopmatFill / Scale / Copy / Add / Sub / MulElem / Clamp on workgroup tiles, as lane walks
def private emit_wg_elementwise_stmt(var ctx : CudaCtx; ec : ExprCall?; cname : string) : string {
    let na = length(ec.arguments)
    let wi = wg_var_info(ctx, ec.arguments[0])
    if (!wi.ok) {
        shader_error(ctx, ec.at, "{cname}: the first argument must be a workgroup-scope coopmatWg* local")
        return ""
    }
    need_wg(ctx, wi)
    let m = emit_value(ctx, ec.arguments[0])
    let sc = wg_scalar(wi.comp)
    let getv = "__das_cmC_get_{wi.comp}"
    let setv = "__das_cmC_set_{wi.comp}"
    if (cname == "coopmatFill") {
        if (na != 2) {
            shader_error(ctx, ec.at, "coopmatFill expects (mat, v)")
            return ""
        }
        if (key_exists(ctx.ones_sum, tile_var_name(ec.arguments[0]))) {
            census_kind(ctx, "call.coopmatFill.wg.ones")
            return ""
        }
        let v = emit_value(ctx, ec.arguments[1])
        if (wi.in_smem) {
            census_kind(ctx, "call.coopmatFill.wg.shared")
            let cv = wi.comp == "f16" ? "(ushort)__das_f2h({v})" : "(char)({v})"
            return "__syncthreads(); \{ {wi.elem} __v = {cv}; for (int __e = __das_tid(); __e < {wi.padded_elems}; __e += __das_nthreads()) \{ {m}.e[__e] = __v; \} \} __syncthreads();"
        }
        census_kind(ctx, "call.coopmatFill.wg.{wi.comp}")
        return "\{ {sc} __v = ({sc})({v}); " + wg_walk(wi, ctx.warps, "{setv}({m}.fr[__s], __k, __v);") + " \}"
    }
    if (wi.in_smem) {
        shader_error(ctx, ec.at, "{cname} walks an accumulator tile; an A or B tile is read-only")
        return ""
    }
    if (cname == "coopmatScale") {
        if (na != 2) {
            shader_error(ctx, ec.at, "coopmatScale expects (mat, s)")
            return ""
        }
        census_kind(ctx, "call.coopmatScale.wg.{wi.comp}")
        return "\{ {sc} __v = ({sc})({emit_value(ctx, ec.arguments[1])}); " + wg_walk(wi, ctx.warps, "{setv}({m}.fr[__s], __k, {getv}({m}.fr[__s], __k) * __v);") + " \}"
    }
    if (cname == "coopmatClamp") {
        if (na != 3) {
            shader_error(ctx, ec.at, "coopmatClamp expects (mat, lo, hi)")
            return ""
        }
        census_kind(ctx, "call.coopmatClamp.wg.{wi.comp}")
        let lo = emit_value(ctx, ec.arguments[1])
        let hi = emit_value(ctx, ec.arguments[2])
        return "\{ {sc} __lo = ({sc})({lo}), __hi = ({sc})({hi}); " + wg_walk(wi, ctx.warps, "\{ {sc} __x = {getv}({m}.fr[__s], __k); {setv}({m}.fr[__s], __k, __x < __lo ? __lo : (__x > __hi ? __hi : __x)); \}") + " \}"
    }
    if (cname == "coopmatCopy") {
        if (na != 2) {
            shader_error(ctx, ec.at, "coopmatCopy expects (dst, src)")
            return ""
        }
        let si = wg_type_of(ctx, ec.arguments[1]._type)
        if (!si.ok || si.ctype != wi.ctype) {
            shader_error(ctx, ec.at, "coopmatCopy: dst and src must be one tile type")
            return ""
        }
        census_kind(ctx, "call.coopmatCopy.wg")
        return "{m} = {emit_value(ctx, ec.arguments[1])};"
    }
    if (na != 3) {
        shader_error(ctx, ec.at, "{cname} expects (dst, a, b)")
        return ""
    }
    let ai = wg_type_of(ctx, ec.arguments[1]._type)
    let bi = wg_var_info(ctx, ec.arguments[2])
    if (!ai.ok || !bi.ok || ai.ctype != wi.ctype || bi.ctype != wi.ctype) {
        shader_error(ctx, ec.at, "{cname}: all three arguments must be one accumulator tile type")
        return ""
    }
    let a = emit_value(ctx, ec.arguments[1])
    let b = emit_value(ctx, ec.arguments[2])
    let op = cname == "coopmatAdd" ? "+" : (cname == "coopmatSub" ? "-" : "*")
    census_kind(ctx, "call.{cname}.wg.{wi.comp}")
    return wg_walk(wi, ctx.warps, "{setv}({m}.fr[__s], __k, {getv}({a}.fr[__s], __k) {op} {getv}({b}.fr[__s], __k));")
}

//! coopmatPerElement(dst, src, @@fn, extras...): dst[r, c] = fn(r, c, src[r, c], extras) over the lanes' elements;
//! a coopmat extra arrives as its own (r, c) element, a scalar as is; an f16 tile's callback speaks half
def private emit_wg_per_element_stmt(var ctx : CudaCtx; ec : ExprCall?) : string {
    let na = length(ec.arguments)
    if (na < 3 || na > 7) {
        shader_error(ctx, ec.at, "coopmatPerElement expects (dst, src, @@fn, ...up to 4 extras)")
        return ""
    }
    let di = wg_type_of(ctx, ec.arguments[0]._type)
    let si = wg_type_of(ctx, ec.arguments[1]._type)
    if (!di.ok || !si.ok || di.ctype != si.ctype || di.in_smem) {
        shader_error(ctx, ec.at, "coopmatPerElement: dst and src must be one accumulator tile type")
        return ""
    }
    let fn = callback_fn(ec.arguments[2])
    if (fn == null) {
        shader_error(ctx, ec.at, "coopmatPerElement: the third argument must be @@<per-element function>")
        return ""
    }
    need_wg(ctx, di)
    let half = di.comp == "f16"
    if (half) {
        need_half(ctx, ec.arguments[0]._type)
    }
    let getv = "__das_cmC_get_{di.comp}"
    let setv = "__das_cmC_set_{di.comp}"
    let dst = emit_value(ctx, ec.arguments[0])
    let src = emit_value(ctx, ec.arguments[1])
    let srcv = "{getv}({src}.fr[__s], __k)"
    var args <- [ "uint({WG_ROW})", "uint({WG_COL})", half ? "half({srcv})" : srcv ]
    for (x in range(3, na)) {
        let ei = wg_type_of(ctx, ec.arguments[x]._type)
        if (ei.ok) {
            if (ei.ctype != di.ctype) {
                shader_error(ctx, ec.at, "coopmatPerElement: coopmat extra {x - 2} must match the tile type")
                return ""
            }
            let ev = "{getv}({emit_value(ctx, ec.arguments[x])}.fr[__s], __k)"
            args |> push(half ? "half({ev})" : ev)
        } else {
            args |> push(emit_value(ctx, ec.arguments[x]))
        }
    }
    let call = ufn_call_text(ctx, fn, args, ec.at)
    let rv = half ? "float({call})" : call
    census_kind(ctx, "call.coopmatPerElement.wg.{di.comp}")
    return wg_walk(di, ctx.warps, "{setv}({dst}.fr[__s], __k, {rv});")
}

//! coopmatReduce(dst, src, mask, @@fn): a row (1) or column (2) reduce; within a lane over its
//! elements, across the fragment through shuffles, across fragments and warps through a shared
//! scratch of partials this site owns; the reduced axis may resize (dst walks its own slots)
def private emit_wg_reduce_stmt(var ctx : CudaCtx; ec : ExprCall?) : string {
    if (length(ec.arguments) != 4) {
        shader_error(ctx, ec.at, "coopmatReduce expects (dst, src, mask, @@combine)")
        return ""
    }
    let di = wg_type_of(ctx, ec.arguments[0]._type)
    let si = wg_type_of(ctx, ec.arguments[1]._type)
    if (!di.ok || !si.ok || di.in_smem || si.in_smem || di.comp != si.comp) {
        shader_error(ctx, ec.at, "coopmatReduce: dst and src must be accumulator tiles of one component")
        return ""
    }
    let mask = const_int_of(ec.arguments[2])
    if (mask != int(COOPMAT_REDUCE_ROW) && mask != int(COOPMAT_REDUCE_COLUMN)) {
        shader_error(ctx, ec.at, "coopmatReduce: mask must be the constant COOPMAT_REDUCE_ROW or COOPMAT_REDUCE_COLUMN")
        return ""
    }
    let row = mask == int(COOPMAT_REDUCE_ROW)
    if (row ? di.rows != si.rows : di.cols != si.cols) {
        shader_error(ctx, ec.at, "coopmatReduce: a {row ? "row" : "column"} reduce keeps src's {row ? "row" : "column"} count")
        return ""
    }
    let fn = callback_fn(ec.arguments[3])
    if (fn == null) {
        shader_error(ctx, ec.at, "coopmatReduce: the last argument must be @@<combine function>")
        return ""
    }
    need_wg(ctx, di)
    need_wg(ctx, si)
    let half = di.comp == "f16"
    if (half) {
        need_half(ctx, ec.arguments[0]._type)
    }
    let sc = wg_scalar(di.comp)
    let getv = "__das_cmC_get_{di.comp}"
    let setv = "__das_cmC_set_{di.comp}"
    let dst = emit_value(ctx, ec.arguments[0])
    let src = emit_value(ctx, ec.arguments[1])
    let cmb = ufn_call_text(ctx, fn, [half ? "half(__a)" : "__a", half ? "half(__b)" : "__b"], ec.at)
    let combine = half ? "float({cmb})" : cmb
    let w = ctx.warps
    if (row && warp_owns_row(si, w) && warp_owns_row(di, w)) {
        // the warp owns every column of its row: the row's value folds inside a lane, across the quad, then across the warp's
        // slots, and lands in every slot of dst's row - no scratch, no block barrier
        census_kind(ctx, "call.coopmatReduce.wg.row.{di.comp}.warp")
        return build_string() $(var o) {
            o |> write("\{ {sc} __r[2]; _Pragma(\"unroll\") for (int __h = 0; __h < 2; ++__h) \{ int __k0 = __h * 2; {sc} __a = {getv}({src}.fr[0], __k0); {sc} __b; ")
            o |> write("_Pragma(\"unroll\") for (int __s = 0; __s < {si.slots}; ++__s) \{ _Pragma(\"unroll\") for (int __q = 0; __q < 4; ++__q) \{ if (__s > 0 || __q > 0) \{ __b = {getv}({src}.fr[__s], __k0 + (__q & 1) + (__q >> 1) * 4); __a = {combine}; \} \} \} ")
            o |> write("__b = __shfl_xor_sync(0xffffffffu, __a, 1); __a = {combine}; __b = __shfl_xor_sync(0xffffffffu, __a, 2); __a = {combine}; __r[__h] = __a; \} ")
            o |> write("_Pragma(\"unroll\") for (int __s = 0; __s < {di.slots}; ++__s) \{ _Pragma(\"unroll\") for (int __h = 0; __h < 2; ++__h) \{ {setv}({dst}.fr[__s], __h * 2, __r[__h]); {setv}({dst}.fr[__s], __h * 2 + 1, __r[__h]); {setv}({dst}.fr[__s], __h * 2 + 4, __r[__h]); {setv}({dst}.fr[__s], __h * 2 + 5, __r[__h]); \} \} \}")
        }
    }
    ctx.reduce_sites++
    let scratch = "__das_red_{ctx.reduce_sites}"
    census_kind(ctx, "call.coopmatReduce.wg.{row ? "row" : "col"}.{di.comp}")
    return build_string() $(var o) {
        o |> write("\{ {smem_decl(ctx, sc, scratch, 4ul, row ? si.rows * si.frag_cols : si.cols * si.frag_rows)} __syncthreads(); ")
        o |> write("int __l = __das_lane(), __g = __l >> 2, __t = __l & 3, __w = __das_warp(); ")
        o |> write("for (int __s = 0; __s < {si.slots}; ++__s) \{ int __f = __s * {w} + __w; if (__f >= {si.frags}) break; {wg_coords(si)} ")
        if (row) {
            o |> write("for (int __h = 0; __h < 2; ++__h) \{ int __k0 = __h * 2; {sc} __a = {getv}({src}.fr[__s], __k0), __b = {getv}({src}.fr[__s], __k0 + 1); __a = {combine}; __b = {getv}({src}.fr[__s], __k0 + 4); __a = {combine}; __b = {getv}({src}.fr[__s], __k0 + 5); __a = {combine}; ")
            o |> write("__b = __shfl_xor_sync(0xffffffffu, __a, 1); __a = {combine}; __b = __shfl_xor_sync(0xffffffffu, __a, 2); __a = {combine}; ")
            o |> write("if (__t == 0) {scratch}[(__i * 16 + __g + __h * 8) * {si.frag_cols} + __j] = __a; \} \} __syncthreads(); ")
            o |> write("for (int __s = 0; __s < {di.slots}; ++__s) \{ int __f = __s * {w} + __w; if (__f >= {di.frags}) break; {wg_coords(di)} ")
            o |> write("for (int __h = 0; __h < 2; ++__h) \{ int __row = __i * 16 + __g + __h * 8; {sc} __a = {scratch}[__row * {si.frag_cols}]; for (int __jj = 1; __jj < {si.frag_cols}; ++__jj) \{ {sc} __b = {scratch}[__row * {si.frag_cols} + __jj]; __a = {combine}; \} ")
            o |> write("{setv}({dst}.fr[__s], __h * 2, __a); {setv}({dst}.fr[__s], __h * 2 + 1, __a); {setv}({dst}.fr[__s], __h * 2 + 4, __a); {setv}({dst}.fr[__s], __h * 2 + 5, __a); \} \} \}")
        } else {
            o |> write("for (int __q = 0; __q < 4; ++__q) \{ int __k0 = (__q >> 1) * 4 + (__q & 1); {sc} __a = {getv}({src}.fr[__s], __k0), __b = {getv}({src}.fr[__s], __k0 + 2); __a = {combine}; ")
            o |> write("__b = __shfl_xor_sync(0xffffffffu, __a, 4); __a = {combine}; __b = __shfl_xor_sync(0xffffffffu, __a, 8); __a = {combine}; __b = __shfl_xor_sync(0xffffffffu, __a, 16); __a = {combine}; ")
            o |> write("if (__g == 0) {scratch}[__das_cm_col(__j, __t, __k0) * {si.frag_rows} + __i] = __a; \} \} __syncthreads(); ")
            o |> write("for (int __s = 0; __s < {di.slots}; ++__s) \{ int __f = __s * {w} + __w; if (__f >= {di.frags}) break; {wg_coords(di)} ")
            o |> write("for (int __q = 0; __q < 4; ++__q) \{ int __k0 = (__q >> 1) * 4 + (__q & 1); int __col = __das_cm_col(__j, __t, __k0); {sc} __a = {scratch}[__col * {si.frag_rows}]; for (int __ii = 1; __ii < {si.frag_rows}; ++__ii) \{ {sc} __b = {scratch}[__col * {si.frag_rows} + __ii]; __a = {combine}; \} ")
            o |> write("{setv}({dst}.fr[__s], __k0, __a); {setv}({dst}.fr[__s], __k0 + 2, __a); \} \} \}")
        }
    }
}

//! coopmatMulAdd over workgroup tiles as a value
def private emit_wg_mul_add(var ctx : CudaCtx; ec : ExprCall?; ai, bi, ci : WgInfo) : string {
    if (!(ai.use == "A" && bi.use == "B" && ci.use == "C" && ai.comp == bi.comp && ai.cols == bi.rows && ai.rows == ci.rows && bi.cols == ci.cols
            && ((ai.comp == "f16" && ci.comp != "s32") || (ai.comp == "s8" && ci.comp == "s32")))) {
        shader_error(ctx, ec.at, "coopmatMulAdd: ({describe(ec.arguments[0]._type)}, {describe(ec.arguments[1]._type)}, {describe(ec.arguments[2]._type)}) is not a workgroup (A MxK, B KxN, accumulator MxN) triple")
        return "/*coopmatMulAdd*/"
    }
    need_wg(ctx, ai)
    need_wg(ctx, bi)
    need_wg(ctx, ci)
    if (key_exists(ctx.ones_sum, tile_var_name(ec.arguments[1]))) {
        register_helper(ctx, "{HELPER_RANK_WG_ROWSUM}rowsum.{ai.ctype}.{ci.ctype}", wg_rowsum_helper(ai, ci))
        census_kind(ctx, "call.coopmatMulAdd.wg.rowsum")
        return "__das_wg_rowsum_{ai.ctype}_{ci.ctype}({emit_value(ctx, ec.arguments[0])}, {emit_value(ctx, ec.arguments[2])})"
    }
    let fa = CoopInfo(ok = true, ctype = "__das_cmA_{ai.comp}", use = "A", comp = ai.comp)
    let fc = CoopInfo(ok = true, ctype = "__das_cmC_{ci.comp}", use = "C", comp = ci.comp)
    if (ai.comp == "f16") {
        register_helper(ctx, "{HELPER_RANK_COOPMAT_OPS}ldm", WG_LDMATRIX_PRELUDE)
        if (bi.global_view) {
            register_helper(ctx, "{HELPER_RANK_WG_ACCESS}gld.{bi.ctype}", wg_gld_helper(bi))
        }
    } else {
        register_helper(ctx, "{HELPER_RANK_COOPMAT_OPS}ldm_s8", WG_LDMATRIX_S8_PRELUDE)
    }
    register_helper(ctx, "{HELPER_RANK_COOPMAT_OPS}mma_{ai.comp}_{ci.comp}", coop_mma_helper(fa, fc))
    register_helper(ctx, "{HELPER_RANK_WG_MMA}mma.{ai.ctype}.{bi.ctype}.{ci.ctype}", wg_mma_helper(ai, bi, ci, ctx.warps))
    census_kind(ctx, "call.coopmatMulAdd.wg.{ai.comp}.{ci.comp}")
    var bars, posts : array<string>
    for (operand in range(2)) {   // a pipelined operand's next-step decode or copy runs past this product, under its mma stream
        let tn = tile_var_name(ec.arguments[operand])
        if (!empty(tn) && key_exists(ctx.deferred, tn)) {
            ctx.flush_after |> push(ctx.deferred[tn])
            ctx.deferred |> erase(tn)
            if (key_exists(ctx.raw_s8_tiles, tn)) {
                bars |> push("__p_{tile_base_name(ctx, tn)}_bar")
                posts |> push(ctx.deferred_post[tn])
                ctx.deferred_post |> erase(tn)
            }
        }
    }
    if (!empty(bars)) {   // the ring tiles' block barrier, every second window: every thread's waited chunks of the next two windows visible, every thread past this product before a copy lands in a slot it read
        ctx.flush_after |> push("if ({join(bars, " || ")}) __syncthreads();")
        ctx.flush_after |> push_from(posts)
    }
    return "__das_wg_mma_{ai.ctype}_{bi.ctype}_{ci.ctype}({emit_value(ctx, ec.arguments[0])}, {emit_value(ctx, ec.arguments[1])}, {emit_value(ctx, ec.arguments[2])})"
}

//! the `self.<method>` form of a callback argument: the method's name, "" for any other shape
def private callback_method_name(ctx : CudaCtx; e : Expression?) : string {
    var x = e
    if (x != null && x is ExprRef2Value) {
        x = (x as ExprRef2Value).subexpr
    }
    let mf = x ?as ExprField
    return mf != null && is_self_ref(ctx, mf.value) ? string(mf.name) : ""
}

//! the decode callback of coopmatLoadTensorDecode: `@@fn` or `self.<method>`, discovered at scan time
def private decode_callback(var ctx : CudaCtx; e : Expression?; at : LineInfo) : FunctionPtr {
    var fa = callback_fn(e)
    if (fa != null) {
        return fa
    }
    let mname = callback_method_name(ctx, e)
    if (empty(mname)) {
        return null
    }
    return resolve_method(ctx, mname, at)
}

//! coopmatLoadTensorDecode(dst, src, base, tl, r0, nr, c0, nc, decode [, decode_v]): the source elements are
//! blocks of (b0, b1) tile elements; every thread decodes a share of the shared tile's elements through the
//! callback at (block coordinate, coordinate in block); a tenth argument naming a method returning a half4 decodes a run
//! four elements a call, the scalar form serving only a run the layout's bound cuts
def private emit_wg_decode_stmt(var ctx : CudaCtx; ec : ExprCall?) : string {
    let na = length(ec.arguments)
    if (na != 9 && na != 10) {
        shader_error(ctx, ec.at, "coopmatLoadTensorDecode expects (dst, src, base, layout, r0, nr, c0, nc, @@decode [, decode_v])")
        return ""
    }
    let wi = wg_type_of(ctx, ec.arguments[0]._type)
    if (!wi.ok || !wi.in_smem || wi.comp != "f16") {
        shader_error(ctx, ec.at, "coopmatLoadTensorDecode: the first argument must be a workgroup-scope f16 A or B tile local, not {describe(ec.arguments[0]._type)}")
        return ""
    }
    let bt = ec.arguments[1]._type
    if (bt == null || bt.baseType != Type.tArray || bt.firstType == null || bt.firstType.baseType != Type.tStructure) {
        shader_error(ctx, ec.at, "coopmatLoadTensorDecode: the source must be an @ssbo array of block structs, not {describe(bt)}")
        return ""
    }
    if (!is_tensor_layout(ec.arguments[3]._type)) {
        shader_error(ctx, ec.at, "coopmatLoadTensorDecode: the fourth argument must be a tensorLayout2D / tensorLayout2DPad local")
        return ""
    }
    let fn = decode_callback(ctx, ec.arguments[8], ec.at)
    if (fn == null) {
        shader_error(ctx, ec.at, "coopmatLoadTensorDecode: the decode argument must be @@<decode function> or self.<decode method>")
        return ""
    }
    let retT = shader_type_name(fn.result)
    let cv = convert_in(retT, "f16", "__d")
    if (empty(cv)) {
        shader_error(ctx, ec.at, "coopmatLoadTensorDecode: the decode returns {describe(fn.result)}, which does not convert to the tile's f16 element")
        return ""
    }
    need_wg(ctx, wi)
    let blkT = elem_struct(ctx, bt.firstType.structType, ec.at)
    return "" if (empty(blkT))
    let dst = emit_value(ctx, ec.arguments[0])
    let src = emit_value(ctx, ec.arguments[1])
    let base = emit_value(ctx, ec.arguments[2])
    let tl = emit_value(ctx, ec.arguments[3])
    let r0 = emit_value(ctx, ec.arguments[4])
    let c0 = emit_value(ctx, ec.arguments[6])
    // a thread decodes a run of eight consecutive columns of one row - one block struct read for the run where the block
    // spans it (b1 a multiple of eight), the per-element form otherwise
    let run = 8
    let call_run = ufn_call_text(ctx, fn, ["__blk", "__bc", "make_uint2(__sr % {tl}.b0, (__sc + uint(__q)) % {tl}.b1)"], ec.at)
    let call_one = ufn_call_text(ctx, fn, ["{src}[({base}) + (__sr / {tl}.b0) * {tl}.s0 + (__sc / {tl}.b1) * {tl}.s1]", "make_uint2(__sr / {tl}.b0, __sc / {tl}.b1)", "make_uint2(__sr % {tl}.b0, __sc % {tl}.b1)"], ec.at)
    let at = wi.use == "B" ? "__c * {wi.row_stride} + {wg_k_at(ctx.kperm, "__r")}" : "__r * {wi.row_stride} + {wg_k_at(ctx.kperm, "__c")}"
    let at_q = wi.use == "B" ? "(__c + __q) * {wi.row_stride} + {wg_k_at(ctx.kperm, "__r")}" : "__r * {wi.row_stride} + {wg_k_at(ctx.kperm, "__c + __q")}"
    census_kind(ctx, "call.coopmatLoadTensorDecode.{wi.use}")
    let coords = "int __r = e / {wi.cols}, __c = e % {wi.cols}; uint __sr = uint({r0}) + uint(__r), __sc = uint({c0}) + uint(__c);"
    // the four-wide twin, where the call names one returning a half4: a run decodes four elements a call, the scalar form
    // only on a run the layout's bound cuts
    var call_v4 = ""
    if (na == 10) {
        let fn4 = decode_callback(ctx, ec.arguments[9], ec.at)
        if (fn4 != null && cuda_is_half_family(fn4.result) && component_count(fn4.result.baseType) == 4) {
            need_half(ctx, fn4.result)
            call_v4 = ufn_call_text(ctx, fn4, ["__blk", "__bc", "make_uint2(__sr % {tl}.b0, (__sc + uint(__q)) % {tl}.b1)"], ec.at)
            census_kind(ctx, "call.coopmatLoadTensorDecode.v4")
        }
    }
    let raw = raw_decode_text(ctx, ec, wi, blkT, src, base, tl, r0, c0, call_run, call_v4, retT, cv, run)
    if (!empty(raw)) {
        return raw
    }
    var run_dec = "_Pragma(\"unroll\") for (int __q = 0; __q < {run}; ++__q) \{ ushort __v = 0; if (!{tl}.pad || __sc + uint(__q) < {tl}.d1) \{ {retT} __d = {call_run}; __v = (ushort){cv}; \} {dst}.e[{at_q}] = __v; \}"
    if (!empty(call_v4)) {
        let lanes = build_string() $(var o) {
            for (j in range(4)) {
                let cj = "__c + __q + {j}"
                let at_j = wi.use == "B" ? "({cj}) * {wi.row_stride} + {wg_k_at(ctx.kperm, "__r")}" : "__r * {wi.row_stride} + {wg_k_at(ctx.kperm, cj)}"
                o |> write("{dst}.e[{at_j}] = __d4.{VEC_LANES[j]}.bits; ")
            }
        }
        let call_j = call_run |> replace("uint(__q)", "uint(__j)")
        let at_j = at_q |> replace("__q", "__j")
        run_dec = "_Pragma(\"unroll\") for (int __q = 0; __q < {run}; __q += 4) \{ if (!{tl}.pad || __sc + uint(__q) + 4u <= {tl}.d1) \{ half4 __d4 = {call_v4}; {lanes}\} else \{ _Pragma(\"unroll\") for (int __j = __q; __j < __q + 4; ++__j) \{ ushort __v = 0; if (__sc + uint(__j) < {tl}.d1) \{ {retT} __d = {call_j}; __v = (ushort){cv}; \} {dst}.e[{at_j}] = __v; \} \} \}"
    }
    let run_body = "{coords} if (!{tl}.pad || (__sr < {tl}.d0 && __sc < {tl}.d1)) \{ const {blkT}& __blk = {src}[({base}) + (__sr / {tl}.b0) * {tl}.s0 + (__sc / {tl}.b1) * {tl}.s1]; uint2 __bc = make_uint2(__sr / {tl}.b0, __sc / {tl}.b1); {run_dec} \} else \{ for (int __q = 0; __q < {run}; ++__q) \{ {dst}.e[{at_q}] = 0; \} \}"
    let one_body = "{coords} ushort __v = 0; if (!{tl}.pad || (__sr < {tl}.d0 && __sc < {tl}.d1)) \{ {retT} __d = {call_one}; __v = (ushort){cv}; \} {dst}.e[{at}] = __v;"
    return "\{ __syncthreads(); if ({tl}.b1 % {run}u == 0u) \{ {wg_grid_loop(wi.rows * wi.cols, run, ctx.warps, run_body)} \} else \{ {wg_grid_loop(wi.rows * wi.cols, 1, ctx.warps, one_body)} \} __syncthreads(); \}"
}

//! the cp.async helpers a text names: the 16-byte copy and the commit
def private need_cp_async(var ctx : CudaCtx) {
    register_helper(ctx, "{HELPER_RANK_INTRINSIC}cp_async_16", "__device__ __forceinline__ void __das_cp_async_16(const void* dst, const void* src) \{ asm volatile(\"cp.async.cg.shared.global [%0], [%1], 16;\" :: \"r\"((unsigned)__cvta_generic_to_shared(dst)), \"l\"(src) : \"memory\"); \}")
    register_helper(ctx, "{HELPER_RANK_INTRINSIC}cp_async_commit", "__device__ __forceinline__ void __das_cp_async_commit() \{ asm volatile(\"cp.async.commit_group;\" ::: \"memory\"); \}")
}

//! the cp.async helpers plus the wait that leaves `pending` groups in flight
def private need_cp_async_wait(var ctx : CudaCtx; pending : int) {
    need_cp_async(ctx)
    register_helper(ctx, "{HELPER_RANK_INTRINSIC}cp_async_wait.{pending}", "__device__ __forceinline__ void __das_cp_async_wait_{pending}() \{ asm volatile(\"cp.async.wait_group {pending};\" ::: \"memory\"); \}")
}

let private RAW_S8_RING = 6   //! an s8 tile's ring slots: RAW_S8_RING - 2 windows ahead in flight at the ring's barrier, the wait leaves RAW_S8_RING - 4 pending

//! an s8 tile's pipelined tensor load on its cp.async ring, "" where the scan did not ring the tile; the source's contiguous
//! direction is the tile's k, a row stride and the origin at 16-byte multiples
[arch(at="../ARCHITECTURE_EMIT.md#cuda-decoded-tile-pipeline")]
def private raw_s8_text(var ctx : CudaCtx; ec : ExprCall?; wi : WgInfo; srcT : string; trans : bool; src, base, tl, r0, c0 : string) : string {
    let tname = tile_var_name(ec.arguments[0])
    let kx = wi.use == "A" ? wi.cols : wi.rows   // the k extent: a window's width along the source's contiguous direction
    return "" if (empty(tname) || !key_exists(ctx.decode_slot_pairs, tname) || wi.k_major || trans != (wi.use == "B") || !(srcT == "char" || srcT == "uchar") || kx % 16 != 0)
    let dbl = ctx.decode_slot_pairs[tname]
    ctx.raw_rings[tname] = dbl   // the kernel's last speculative copies wait before the block leaves
    ctx.raw_s8_tiles |> insert(tname)
    let pname = "__p_{tile_base_name(ctx, tname)}"
    need_cp_async_wait(ctx, 0)
    let ring = RAW_S8_RING
    let pending = ring - 4
    need_cp_async_wait(ctx, pending)
    census_kind(ctx, "call.coopmatLoadTensor.{wi.use}.s8.{srcT}{trans ? ".t" : ""}.raw")
    let walk = trans ? "int c = e / {wi.rows}, r = e % {wi.rows};" : "int r = e / {wi.cols}, c = e % {wi.cols};"
    let kv = wi.use == "A" ? "c" : "r"
    let ov = wi.use == "A" ? "r" : "c"
    let sr = trans ? "__rr0 + uint(c)" : "__rr0 + uint(r)"
    let warps = ctx.warps
    let off = int(dbl.off)
    let slot_bytes = int(dbl.slot_bytes)
    let at = wg_tile_at(wi, kv, ov)
    // the copies of the window at column `col` into slot `slot`: a chunk inside the bounds by cp.async, one past them as zeros
    let copy_of = @(col, slot : string) : string {
        let sc = trans ? "({col}) + uint(r)" : "({col}) + uint(c)"
        let body = "{walk} uint sr = {sr}, sc = {sc}; char* __d = __das_smem + {off} + ({slot}) * {slot_bytes} + ({at}); if (!{tl}.pad || (sr < {tl}.d0 && sc + 16u <= {tl}.d1)) \{ __das_cp_async_16(__d, (const char*)&{src}[__rb + sr * {tl}.s0 + sc]); \} else \{ *(uint4*)__d = make_uint4(0u, 0u, 0u, 0u); \}"
        return wg_grid_loop(wi.rows * wi.cols, 16, warps, body)
    }
    // a group commits whether it copied or not, so the wait's pending count stays the windows ahead
    let a1 = ring - 2
    let a2 = ring - 1
    ctx.deferred[tname] = "\{ uint __nc = {pname}_c + {kx}u; int __ns = ({pname}_s + 1) % {ring}; {pname}_bar = ({pname}_s & 1) == 1; if ({pname}_bar) \{ __das_cp_async_wait_{pending}(); \} if (__nc < {tl}.d1) \{ {pname}_c = __nc; {pname}_s = __ns; \} else \{ {pname}_s = -1; \} \}"
    let copy_a1 = copy_of("__nc + {a1 * kx}u", "(__ns + {a1}) % {ring}")
    let copy_a2 = copy_of("__nc + {a2 * kx}u", "(__ns + {a2}) % {ring}")
    ctx.deferred_post[tname] = "if ({pname}_bar && {pname}_s >= 0) \{ uint __rb = {pname}_b; uint __rr0 = {pname}_r; uint __nc = {pname}_c; int __ns = {pname}_s; if (__nc + {a1 * kx}u < {tl}.d1) \{ {copy_a1} \} __das_cp_async_commit(); if (__nc + {a2 * kx}u < {tl}.d1) \{ {copy_a2} \} __das_cp_async_commit(); \}"
    let copy_now = copy_of("__rc0", "0")
    let copy_next = copy_of("__rc0 + {kx}u", "1")
    let copies_ahead = build_string() $(var o) {
        for (j in range(2, ring)) {
            let col_j = "__rc0 + {j * kx}u"
            let slot_j = "{j}"
            let copy_j = copy_of(col_j, slot_j)
            o |> write("if ({col_j} < {tl}.d1) \{ {copy_j} \} __das_cp_async_commit(); ")
        }
    }
    return build_string() $(var w) {
        w |> write("\{ uint __rb = uint({base}); uint __rr0 = uint({r0}); uint __rc0 = uint({c0}); int __slot; ")
        w |> write("if ({pname}_s >= 0 && {pname}_b == __rb && {pname}_r == __rr0 && {pname}_c == __rc0) \{ __slot = {pname}_s; \} else \{ ")
        // not predicted: this window's and the next one's chunks copied into slots 0 and 1 and waited, the windows after on their way into the slots after, a group each
        w |> write("__slot = 0; __syncthreads(); {copy_now} __das_cp_async_commit(); if (__rc0 + {kx}u < {tl}.d1) \{ {copy_next} \} __das_cp_async_commit(); __das_cp_async_wait_0(); __syncthreads(); ")
        w |> write("{copies_ahead}{pname}_b = __rb; {pname}_r = __rr0; {pname}_c = __rc0; {pname}_s = 0; \} ")
        w |> write("{pname} = ({wi.ctype}*)(__das_smem + {off} + __slot * {slot_bytes}); \}")
    }
}

//! the decode-in-load's pipelined form, "" where it does not apply (the scan's `can_pipeline_decode` admits the tile)
[arch(at="../ARCHITECTURE_EMIT.md#cuda-decoded-tile-pipeline")]
def private raw_decode_text(var ctx : CudaCtx; ec : ExprCall?; wi : WgInfo; blkT, src, base, tl, r0, c0, call_run, call_v4, retT, cv : string; run0 : int) : string {
    return "" if (wi.use != "A")
    let tname = tile_var_name(ec.arguments[0])
    return "" if (empty(tname) || !key_exists(ctx.decode_slot_pairs, tname) || !key_exists(ctx.layout_block_size, tl))
    let bs = ctx.layout_block_size[tl]
    let b1 = bs.y
    let run = 16   // a thread's run is one 16-byte chunk of a block
    return "" if (bs.x != 1 || b1 % run != 0 || run % run0 != 0 || b1 > wi.cols || wi.cols % b1 != 0)
    let size = ctx.elem_structs[blkT].size
    return "" if (size != b1 || size % 16 != 0)   // a byte an element: a chunk's bytes are its run's elements
    let bpr = wi.cols / b1
    let slot_bytes = uint64(wi.rows * wi.cols)
    if (!key_exists(ctx.raw_rings, tname)) {
        ctx.raw_rings[tname] = RawRing(off = smem_carve(ctx, slot_bytes), slot_bytes = slot_bytes)
    }
    let ring = ctx.raw_rings[tname]
    let dbl = ctx.decode_slot_pairs[tname]
    let pname = "__p_{tile_base_name(ctx, tname)}"
    need_cp_async_wait(ctx, 0)
    census_kind(ctx, "call.coopmatLoadTensorDecode.A.raw")
    // the copies of one step's blocks into a raw slot, by the decode's own walk: a thread's chunk at e holds the bytes of its
    // run at e - the slot holds the blocks row-major, as the decode indexes them
    let kperm = ctx.kperm
    let warps = ctx.warps
    let copy_of = @(col : string) : string {
        let body = "int __r = e / {wi.cols}, __c = e % {wi.cols}; uint __sr = __rr0 + uint(__r); uint __sb = ({col}) / {b1}u + uint(__c / {b1}); if (!{tl}.pad || (__sr < {tl}.d0 && __sb * {b1}u < {tl}.d1)) \{ __das_cp_async_16(__das_smem + {int(ring.off)} + e, (const char*)&{src}[__rb + __sr * {tl}.s0 + __sb * {tl}.s1] + (__c % {b1})); \}"
        return wg_grid_loop(wi.rows * wi.cols, run, warps, body)
    }
    // the decode of the raw slot's blocks at column `col` into the f16 tile `dst`: a thread's run of sixteen, stored as
    // eight packed pairs (the k permutation keeps a pair adjacent)
    let decode_pass = @(dst, col : string) : string {
        let st = build_string() $(var o) {
            for (m in range(run / 2)) {
                let kp = wg_k_at(kperm, "__c + {2 * m}")
                o |> write("*(uint*)&{dst}.e[__r * {wi.row_stride} + {kp}] = __v[{2 * m}] | (__v[{2 * m + 1}] << 16); ")
            }
        }
        let scalar_dec = "_Pragma(\"unroll\") for (int __q = 0; __q < {run}; ++__q) \{ __v[__q] = 0u; if (!{tl}.pad || __sc + uint(__q) < {tl}.d1) \{ {retT} __d = {call_run}; __v[__q] = {cv}; \} \}"
        var dec = scalar_dec
        if (!empty(call_v4)) {
            let lanes = build_string() $(var o) {
                for (j in range(4)) {
                    o |> write("__v[__q + {j}] = uint(__d4.{VEC_LANES[j]}.bits); ")
                }
            }
            let call_j = call_run |> replace("uint(__q)", "uint(__j)")
            dec = "_Pragma(\"unroll\") for (int __q = 0; __q < {run}; __q += 4) \{ if (!{tl}.pad || __sc + uint(__q) + 4u <= {tl}.d1) \{ half4 __d4 = {call_v4}; {lanes}\} else \{ _Pragma(\"unroll\") for (int __j = __q; __j < __q + 4; ++__j) \{ __v[__j] = 0u; if (__sc + uint(__j) < {tl}.d1) \{ {retT} __d = {call_j}; __v[__j] = {cv}; \} \} \} \}"
        }
        let body = "int __r = e / {wi.cols}, __c = e % {wi.cols}; uint __sr = __rr0 + uint(__r), __sc = ({col}) + uint(__c); uint __v[{run}]; if (!{tl}.pad || (__sr < {tl}.d0 && __sc < {tl}.d1)) \{ const {blkT}& __blk = __raw[__r * {bpr} + __c / {b1}]; uint2 __bc = make_uint2(__sr, __sc / {b1}u); {dec} \} else \{ _Pragma(\"unroll\") for (int __q = 0; __q < {run}; ++__q) \{ __v[__q] = 0u; \} \} {st}"
        return "const {blkT}* __raw = (const {blkT}*)(__das_smem + {int(ring.off)}); {wg_grid_loop(wi.rows * wi.cols, run, warps, body)}"
    }
    // past the next product over the tile: the thread waits for its own chunks of the step after this one (copied past the
    // decode that freed the slot, a product ago), decodes them into the f16 slot the product does not read, and copies the
    // step after that; the registers then name the decoded step
    let dec_next = decode_pass("(*__pn)", "__nc")
    let copy_after = copy_of("__nc + {wi.cols}u")
    ctx.deferred[tname] = "\{ uint __rb = {pname}_b; uint __rr0 = {pname}_r; uint __nc = {pname}_c + {wi.cols}u; int __ns = {pname}_s ^ 1; __das_cp_async_wait_0(); if (__nc < {tl}.d1) \{ {wi.ctype}* __pn = ({wi.ctype}*)(__das_smem + {int(dbl.off)} + __ns * {int(dbl.slot_bytes)}); {dec_next} {pname}_c = __nc; {pname}_s = __ns; if (__nc + {wi.cols}u < {tl}.d1) \{ {copy_after} \} __das_cp_async_commit(); \} else \{ {pname}_s = -1; \} \}"
    let copy_now = copy_of("__rc0")
    let copy_next = copy_of("__rc0 + {wi.cols}u")
    let dec_now = decode_pass("(*{pname})", "__rc0")
    return build_string() $(var w) {
        w |> write("\{ uint __rb = uint({base}); uint __rr0 = uint({r0}); uint __rc0 = uint({c0}); __syncthreads(); int __slot; ")
        w |> write("if ({pname}_s >= 0 && {pname}_b == __rb && {pname}_r == __rr0 && {pname}_c == __rc0) \{ __slot = {pname}_s; \} else \{ ")
        // not predicted: the step's own blocks copied and decoded into slot 0 and published, then the next step's blocks on
        // their way, as a predicted step finds them
        w |> write("__slot = 0; {copy_now} __das_cp_async_commit(); __das_cp_async_wait_0(); {pname} = ({wi.ctype}*)(__das_smem + {int(dbl.off)}); {dec_now} __syncthreads(); ")
        w |> write("if (__rc0 + {wi.cols}u < {tl}.d1) \{ {copy_next} \} __das_cp_async_commit(); {pname}_b = __rb; {pname}_r = __rr0; {pname}_c = __rc0; {pname}_s = 0; \} ")
        w |> write("{pname} = ({wi.ctype}*)(__das_smem + {int(dbl.off)} + __slot * {int(dbl.slot_bytes)}); \}")
    }
}

//! a tile local's base emitted name, under the pointer rename a decoded tile carries
def private tile_base_name(ctx : CudaCtx; vname : string) : string {
    let r = ctx.renames?[vname] ?? vname
    return (r |> starts_with("(*__p_")) ? slice(r, 6, length(r) - 1) : r
}

//! the cm2 statement forms; "" after err, or when `cname` is not one
def private emit_cm2_stmt(var ctx : CudaCtx; ec : ExprCall?; cname : string) : string {
    if ((cname |> starts_with("tensorLayout")) || (cname |> starts_with("tensorView"))) {
        return emit_tensor_object_stmt(ctx, ec, cname)
    }
    if (cname == "coopmatLoadTensor" || cname == "coopmatStoreTensor") {
        return emit_wg_tensor_stmt(ctx, ec, cname == "coopmatStoreTensor")
    }
    if (cname == "coopmatLoadTensorDecode") {
        return emit_wg_decode_stmt(ctx, ec)
    }
    if (cname == "coopmatPerElement") {
        return emit_wg_per_element_stmt(ctx, ec)
    }
    if (cname == "coopmatReduce") {
        return emit_wg_reduce_stmt(ctx, ec)
    }
    if (cname == "coopmatScaleAcc") {
        return emit_wg_scale_acc_stmt(ctx, ec)
    }
    return emit_wg_elementwise_stmt(ctx, ec, cname)
}

//! coopmatScaleAcc(facc, sacc, rs, rbase, cs, cbase): facc[r, c] += float(sacc[r, c]) * rs[rbase + r] * cs[cbase + c] over the
//! lanes' elements - the block-scaled int8 product's fold, the shared scale arrays indexed by the walk's coordinates
def private emit_wg_scale_acc_stmt(var ctx : CudaCtx; ec : ExprCall?) : string {
    if (length(ec.arguments) != 6) {
        shader_error(ctx, ec.at, "coopmatScaleAcc expects (facc, sacc, rs, rbase, cs, cbase)")
        return ""
    }
    let di = wg_var_info(ctx, ec.arguments[0])
    let si = wg_var_info(ctx, ec.arguments[1])
    if (!di.ok || !si.ok || di.use != "C" || si.use != "C" || di.comp != "f32" || si.comp != "s32" || di.rows != si.rows || di.cols != si.cols || di.row_own != si.row_own) {
        shader_error(ctx, ec.at, "coopmatScaleAcc: an f32 accumulator tile and the s32 accumulator of its shape")
        return ""
    }
    need_wg(ctx, di)
    need_wg(ctx, si)
    let dst = emit_value(ctx, ec.arguments[0])
    let src = emit_value(ctx, ec.arguments[1])
    let rs = emit_value(ctx, ec.arguments[2])
    let rb = emit_value(ctx, ec.arguments[3])
    let cs = emit_value(ctx, ec.arguments[4])
    let cb = emit_value(ctx, ec.arguments[5])
    census_kind(ctx, "call.coopmatScaleAcc.wg")
    // a lane's eight elements of a fragment sit on two rows and four columns: their six scales read once, then the folds
    let fold = build_string() $(var o) {
        o |> write("float __r0 = {rs}[({rb}) + __i * 16 + __g], __r1 = {rs}[({rb}) + __i * 16 + __g + 8]; ")
        o |> write("float __c0 = {cs}[({cb}) + __j * 16 + 2 * __t], __c1 = {cs}[({cb}) + __j * 16 + 2 * __t + 1], __c2 = {cs}[({cb}) + __j * 16 + 2 * __t + 8], __c3 = {cs}[({cb}) + __j * 16 + 2 * __t + 9]; ")
        for (k in range(8)) {
            let r = (k & 2) != 0 ? "__r1" : "__r0"
            let c = "__c{(k >> 2) * 2 + (k & 1)}"
            o |> write("__das_cmC_set_f32({dst}.fr[__s], {k}, __das_cmC_get_f32({dst}.fr[__s], {k}) + float(__das_cmC_get_s32({src}.fr[__s], {k})) * {r} * {c}); ")
        }
    }
    return "\{ {wg_slot_loop_head(di, ctx.warps)} {fold} \} \}"
}

//! GLSL's packHalf2x16 / unpackHalf2x16: lane x in the low half, through the PTX converts
let private PACK_HALF2_PRELUDE = "__device__ __forceinline__ uint __das_pack_half2(float2 v) \{ uint r; asm(\"cvt.rn.f16x2.f32 %0, %1, %2;\" : \"=r\"(r) : \"f\"(v.y), \"f\"(v.x)); return r; \}"
let private UNPACK_HALF2_PRELUDE = "__device__ __forceinline__ float2 __das_unpack_half2(uint u) \{ unsigned short lo = (unsigned short)(u & 0xffffu), hi = (unsigned short)(u >> 16); float a, b; asm(\"cvt.f32.f16 %0, %1;\" : \"=f\"(a) : \"h\"(lo)); asm(\"cvt.f32.f16 %0, %1;\" : \"=f\"(b) : \"h\"(hi)); return make_float2(a, b); \}"

def private is_user_fn(fn : FunctionPtr) : bool {
    if (fn == null || fn.flags.builtIn || key_exists(LADDER_OWNED, call_base_name("{fn.name}"))) {
        return false
    }
    let origin = fn.fromGeneric != null ? fn.fromGeneric : fn
    let mname = origin._module != null ? string(origin._module.name) : ""
    return mname != "cuda_builtins" && mname != "cuda_intrinsics" && mname != "spirv_builtins" && mname != "shader_lingua_franca"
}

def private discover_user_fn(var ctx : CudaCtx; fn : FunctionPtr; at : LineInfo) : int {
    let key = user_fn_key(fn)
    let bn = call_base_name("{fn.name}")
    for (s in ctx.scan_stack) {
        if (s == key) {
            shader_error(ctx, at, "function `{bn}` is recursive - a kernel helper does not recurse")
            return -1
        }
    }
    if (key_exists(ctx.user_fn_index, key)) {
        return ctx.user_fn_index[key]
    }
    if (fn.body == null) {
        shader_error(ctx, at, "function `{bn}` has no body to lower")
        return -1
    }
    let idx = length(ctx.user_fns)
    ctx.user_fn_index[key] = idx
    ctx.user_fns |> emplace <| UserFn(fn = unsafe(reinterpret<FunctionPtr>(fn)), name = unique_fn_name(ctx, CUDA_RESERVED, bn))
    ctx.scan_stack |> push(key)
    let prev = ctx.cur_user_fn
    let prev_self = ctx.self_name
    ctx.cur_user_fn = idx
    ctx.self_name = ""
    scan_body(ctx, fn.body)
    ctx.self_name = prev_self
    ctx.cur_user_fn = prev
    ctx.scan_stack |> pop()
    return idx
}

//! the concrete kernel class's method-field init is the dispatch table: the field init (inherited or overriding) resolves a method call statically
def private resolve_method(var ctx : CudaCtx; mname : string; at : LineInfo) : FunctionPtr {
    for (fld in ctx.kcls.fields) {
        if (fld.name == mname) {
            var ie = fld.init
            while (ie != null) {
                if (ie is ExprRef2Value) {
                    ie = (ie as ExprRef2Value).subexpr
                } elif (ie is ExprCast) {
                    ie = (ie as ExprCast).subexpr
                } else {
                    break
                }
            }
            let ea = ie ?as ExprAddr
            if (ea != null && ea.func != null) {
                return unsafe(reinterpret<FunctionPtr>(ea.func))
            }
            let shape = ie == null ? "no init" : "{describe(ie)} [{ie.__rtti}]"
            shader_error(ctx, at, "method `{mname}` of `{ctx.kcls.name}` has no resolvable body (abstract, or its field is not a plain method: {shape})")
            return null
        }
    }
    shader_error(ctx, at, "method `{mname}` is not a member of `{ctx.kcls.name}`")
    return null
}

def private discover_method(var ctx : CudaCtx; mfn : FunctionPtr; mname : string; at : LineInfo) : int {
    let key = user_fn_key(mfn)
    for (s in ctx.scan_stack) {
        if (s == key) {
            shader_error(ctx, at, "method `{mname}` is recursive - a kernel helper does not recurse")
            return -1
        }
    }
    if (key_exists(ctx.user_fn_index, key)) {
        return ctx.user_fn_index[key]
    }
    if (mfn.body == null) {
        shader_error(ctx, at, "method `{mname}` has no body to lower")
        return -1
    }
    let idx = length(ctx.user_fns)
    ctx.user_fn_index[key] = idx
    ctx.user_fns |> emplace <| UserFn(fn = unsafe(reinterpret<FunctionPtr>(mfn)), name = unique_fn_name(ctx, CUDA_RESERVED, mname), is_method = true)
    ctx.scan_stack |> push(key)
    let prev = ctx.cur_user_fn
    let prev_self = ctx.self_name
    ctx.cur_user_fn = idx
    ctx.self_name = !empty(mfn.arguments) ? string(mfn.arguments[0].name) : ""
    scan_body(ctx, mfn.body)
    ctx.self_name = prev_self
    ctx.cur_user_fn = prev
    ctx.scan_stack |> pop()
    return idx
}

def private scan_body(var ctx : CudaCtx; e : Expression?) {
    if (e == null) {
        return
    }
    let here = pos_of(e.at)
    if (before(ctx.scan_at, here)) {
        ctx.scan_at = here
    }
    if (e is ExprVar) {
        let vn = string((e as ExprVar).name)
        if ((e as ExprVar).varFlags.local && key_exists(ctx.tiles, vn) && before(ctx.tiles[vn].last, here)) {
            ctx.tiles[vn].last = here
        }
    }
    if (e is ExprRef2Value) {
        scan_body(ctx, (e as ExprRef2Value).subexpr)
    } elif (e is ExprUnsafe) {
        scan_body(ctx, (e as ExprUnsafe).body)
    } elif (e is ExprPtr2Ref) {
        scan_body(ctx, (e as ExprPtr2Ref).subexpr)
    } elif (e is ExprBlock) {
        for (s in (e as ExprBlock).list) {
            scan_body(ctx, s)
        }
    } elif (e is ExprWith) {
        scan_body(ctx, (e as ExprWith).body)
    } elif (e is ExprLet) {
        for (v in (e as ExprLet).variables) {
            let twi = wg_info_of(v._type != null && v._type.structType != null ? string(v._type.structType.name) : "", ctx.warps, false)
            if (twi.ok && twi.in_smem) {
                let tn = string(v.name)
                if (key_exists(ctx.tiles, tn)) {
                    ctx.tiles[tn].ambiguous = true
                } else {
                    ctx.tiles[tn] = TileLife(decl = pos_of(v.at), last = pos_of(v.at), ones_feeds_row_owned = true, global_view_blocked = !(twi.use == "B" && twi.comp == "f16"))
                }
            }
            if (v.init != null && v.init is ExprRef2Ptr && (v.init as ExprRef2Ptr).subexpr is ExprAt) {
                let pat = (v.init as ExprRef2Ptr).subexpr as ExprAt
                let pmem = member_lookup(ctx, pat.subexpr)
                if (!empty(pmem)) {
                    ctx.scan_ptrs[string(v.name)] = pmem
                }
            }
            scan_body(ctx, v.init)
        }
    } elif (e is ExprCopy) {
        // rebinding a pointer local (p = p + 1) writes no member
        var lv = (e as ExprCopy).left
        while (lv != null && lv is ExprRef2Value) {
            lv = (lv as ExprRef2Value).subexpr
        }
        if (!(lv is ExprVar && lv._type != null && lv._type.baseType == Type.tPointer)) {
            mark_write_target(ctx, (e as ExprCopy).left)
        }
        scan_body(ctx, (e as ExprCopy).left)
        scan_body(ctx, (e as ExprCopy).right)
    } elif (e is ExprOp2) {
        let o2 = e as ExprOp2
        if (o2._type != null && o2._type.baseType == Type.tVoid) {
            mark_write_target(ctx, o2.left)
        }
        scan_body(ctx, o2.left)
        scan_body(ctx, o2.right)
    } elif (e is ExprOp1) {
        let o1 = e as ExprOp1
        let op = string(o1.op)
        if (op == "++" || op == "--" || op == "+++" || op == "---") {
            mark_write_target(ctx, o1.subexpr)
        }
        scan_body(ctx, o1.subexpr)
    } elif (e is ExprOp3) {
        let o3 = e as ExprOp3
        if (o3.subexpr is ExprConstBool) {
            scan_body(ctx, (o3.subexpr as ExprConstBool).value ? o3.left : o3.right)
        } else {
            scan_body(ctx, o3.subexpr)
            scan_body(ctx, o3.left)
            scan_body(ctx, o3.right)
        }
    } elif (e is ExprIfThenElse) {
        scan_body(ctx, (e as ExprIfThenElse).cond)
        scan_body(ctx, (e as ExprIfThenElse).if_true)
        scan_body(ctx, (e as ExprIfThenElse).if_false)
    } elif (e is ExprWhile) {
        scan_body(ctx, (e as ExprWhile).cond)
        scan_body(ctx, (e as ExprWhile).body)
        ctx.loops |> push(LoopSpan(start = pos_of(e.at), end = ctx.scan_at))
    } elif (e is ExprFor) {
        for (src in (e as ExprFor).sources) {
            scan_body(ctx, src)
        }
        scan_body(ctx, (e as ExprFor).body)
        ctx.loops |> push(LoopSpan(start = pos_of(e.at), end = ctx.scan_at))
    } elif (e is ExprCall) {
        let ecs = e as ExprCall
        let scn = call_base_name("{ecs.name}")
        if ((scn |> starts_with("atomic")) && !empty(ecs.arguments)) {
            mark_write_target(ctx, ecs.arguments[0])
        }
        // the tile locals' lives: a convert's target, a ones fill, the product's B operand, a tensor load from a 16-bit source
        // (the global view's write), any other read
        for (ai, arg in range(length(ecs.arguments)), ecs.arguments) {
            let tn = tile_var_name(arg)
            continue if (empty(tn) || !key_exists(ctx.tiles, tn))
            if (scn == "coopmatConvert" && ai == 0) {
                ctx.tiles[tn].convert_target = true
                ctx.tiles[tn].global_view_blocked = true
            } elif (scn == "coopmatFill" && ai == 0 && length(ecs.arguments) == 2) {
                let cf = ecs.arguments[1] ?as ExprConstFloat
                if (cf != null && cf.value == 1.0) {
                    ctx.tiles[tn].ones = true
                } else {
                    ctx.tiles[tn].other_use = true
                }
                ctx.tiles[tn].global_view_blocked = true
            } elif (scn == "coopmatMulAdd" && ai == 1 && length(ecs.arguments) == 3) {
                let ci = wg_info_of(ecs.arguments[2]._type != null && ecs.arguments[2]._type.structType != null ? string(ecs.arguments[2]._type.structType.name) : "", ctx.warps, false)
                if (!(ci.ok && ci.use == "C" && row_per_warp(ci, ctx.warps) && ci.comp != "s32")) {
                    ctx.tiles[tn].ones_feeds_row_owned = false
                }
                if (ci.ok && ci.use == "C" && row_per_warp(ci, ctx.warps)) {
                    ctx.tiles[tn].read_whole_by_row_owner = true
                }
            } elif (scn == "coopmatLoadTensor" && ai == 0 && length(ecs.arguments) >= 8) {
                ctx.tiles[tn].other_use = true
                let st = tensor_buf_elem(ecs.arguments[1]._type)
                if (st == "half" || st == "ushort") {
                    ctx.tiles[tn].global_view_written = true
                } else {
                    ctx.tiles[tn].global_view_blocked = true
                    let swi = wg_type_of(ctx, ecs.arguments[0]._type)
                    if (swi.ok && swi.in_smem && swi.comp == "s8" && (st == "char" || st == "uchar")) {
                        ctx.tiles[tn].raw_s8 = true
                    }
                }
            } else {
                ctx.tiles[tn].other_use = true
                ctx.tiles[tn].global_view_blocked = true
                if (scn == "coopmatLoadTensorDecode" && ai == 0 && can_pipeline_decode(ctx, ecs)) {
                    ctx.tiles[tn].decoded = true
                }
            }
        }
        if ((scn == "tensorLayoutCreate" || scn == "tensorLayoutSetBlockSize") && !empty(ecs.arguments)) {
            let ln = tile_var_name(ecs.arguments[0])
            if (!empty(ln)) {
                let b0c = scn == "tensorLayoutCreate" ? 1 : (length(ecs.arguments) == 3 ? const_int_of(ecs.arguments[1]) : -1)
                let b1c = scn == "tensorLayoutCreate" ? 1 : (length(ecs.arguments) == 3 ? const_int_of(ecs.arguments[2]) : -1)
                if (b0c > 0 && b1c > 0) {
                    ctx.scan_layout_block_size[ln] = int2(b0c, b1c)
                } else {
                    ctx.scan_layout_block_size |> erase(ln)
                }
            }
        }
        // a B tile's loads decide its store: every load under a plain view keeps the source's [k][n] rows, a load under the
        // transposed view the [n][k] tile; a tile loaded both ways is refused at its load
        if (scn == "coopmatLoadTensor" && length(ecs.arguments) >= 8) {
            var dx = ecs.arguments[0]
            if (dx is ExprRef2Value) {
                dx = (dx as ExprRef2Value).subexpr
            }
            let dv = dx ?as ExprVar
            let dt = dx != null && dx._type != null && dx._type.structType != null ? string(dx._type.structType.name) : ""
            let brows = wg_b_rows(dt, ctx.warps)
            if (dv != null && brows > 0) {
                let tv = length(ecs.arguments) == 9 ? ecs.arguments[8]._type : null
                let trans = tv != null && tv.structType != null && tv.structType.name == "tensorView2Dt"
                if (trans) {
                    ctx.n_major_tiles |> insert(string(dv.name))
                } else {
                    ctx.k_major_tiles |> insert(string(dv.name))
                }
                // a global view is the [n][k] form over 32-multiples of k: a lane's eight k are one load
                if ((!trans || brows % 32 != 0) && key_exists(ctx.tiles, string(dv.name))) {
                    ctx.tiles[string(dv.name)].global_view_blocked = true
                }
            }
        }
        // a row reduce over an accumulator whose fragment rows the warps divide turns the kernel's accumulators row-owned: the
        // reduce then stays inside a warp, and an accumulator converted into an A tile reaches the next product warp-locally
        if (scn == "coopmatReduce" && length(ecs.arguments) == 4 && const_int_of(ecs.arguments[2]) == int(COOPMAT_REDUCE_ROW)) {
            let si = wg_info_of(ecs.arguments[1]._type != null && ecs.arguments[1]._type.structType != null ? string(ecs.arguments[1]._type.structType.name) : "", ctx.warps, false)
            if (si.ok && si.use == "C" && row_per_warp(si, ctx.warps)) {
                ctx.row_own = true
            }
        }
        if ((scn == "coopmatStore" || scn == "coopmatStoreTensor") && length(ecs.arguments) >= 2) {
            mark_write_target(ctx, ecs.arguments[1])
        }
        for (ai, arg in range(length(ecs.arguments)), ecs.arguments) {
            let cb = callback_fn(arg)
            if (cb != null && is_user_fn(cb)) {
                let cui = discover_user_fn(ctx, cb, ecs.at)
                if (cui >= 0 && ctx.cur_user_fn >= 0) {
                    ctx.user_fns[ctx.cur_user_fn].calls |> push(cui)
                }
            } elif (scn == "coopmatLoadTensorDecode" && ai >= 8) {
                let mname = callback_method_name(ctx, arg)
                if (!empty(mname)) {
                    let mfn = resolve_method(ctx, mname, ecs.at)
                    if (mfn != null) {
                        let mui = discover_method(ctx, mfn, mname, ecs.at)
                        if (mui >= 0 && ctx.cur_user_fn >= 0) {
                            ctx.user_fns[ctx.cur_user_fn].calls |> push(mui)
                        }
                    }
                }
            }
        }
        if (is_user_fn(ecs.func)) {
            let ui = discover_user_fn(ctx, ecs.func, ecs.at)
            if (ui >= 0) {
                if (ctx.cur_user_fn >= 0) {
                    ctx.user_fns[ctx.cur_user_fn].calls |> push(ui)
                }
                let na = length(ecs.arguments)
                let np = length(ecs.func.arguments)
                for (i in range(min(na, np))) {
                    if (param_is_written(ecs.func.arguments[i]._type)) {
                        mark_write_target(ctx, ecs.arguments[i])
                    }
                }
            }
        }
        for (arg in ecs.arguments) {
            scan_body(ctx, arg)
        }
    } elif (e is ExprInvoke) {
        let ei = e as ExprInvoke
        let mf = length(ei.arguments) >= 2 ? invoke_method_field(ei.arguments[0]) : null
        if (ei.isInvokeMethod && mf != null && is_self_ref(ctx, ei.arguments[1])) {
            let mname = string(mf.name)
            let mfn = resolve_method(ctx, mname, ei.at)
            if (mfn != null) {
                let ui = discover_method(ctx, mfn, mname, ei.at)
                if (ui >= 0) {
                    if (ctx.cur_user_fn >= 0) {
                        ctx.user_fns[ctx.cur_user_fn].calls |> push(ui)
                    }
                    let na = length(ei.arguments) - 2
                    let np = length(mfn.arguments) - 1
                    for (i in range(min(na, np))) {
                        if (param_is_written(mfn.arguments[i + 1]._type)) {
                            mark_write_target(ctx, ei.arguments[i + 2])
                        }
                    }
                }
            }
        }
        for (i in range(2, length(ei.arguments))) {
            scan_body(ctx, ei.arguments[i])
        }
    } elif (e is ExprReturn) {
        scan_body(ctx, (e as ExprReturn).subexpr)
    } elif (e is ExprRef2Ptr) {
        scan_body(ctx, (e as ExprRef2Ptr).subexpr)
    } elif (e is ExprAt) {
        scan_body(ctx, (e as ExprAt).subexpr)
        scan_body(ctx, (e as ExprAt).index)
    } elif (e is ExprField) {
        let ef = e as ExprField
        let n = string(ef.name)
        if (is_self_ref(ctx, ef.value) && key_exists(ctx.members, n)) {
            use_member(ctx, n)
        } else {
            scan_body(ctx, ef.value)
        }
    } elif (e is ExprSwizzle) {
        scan_body(ctx, (e as ExprSwizzle).value)
    } elif (e is ExprVar) {
        let ev = e as ExprVar
        if (!(ev.varFlags.local || ev.varFlags.argument || ev.varFlags._block)) {
            if (builtin_info(string(ev.name)).ok) {
                use_builtin(ctx, string(ev.name))
            }
        } elif (ctx.cur_user_fn >= 0 && key_exists(ctx.scan_ptrs, string(ev.name))) {
            use_member(ctx, ctx.scan_ptrs[string(ev.name)])
        }
    }
}

//! what a function needs from its caller - its own reads and every callee's, transitively, sorted for a pinned signature
def private ufn_all_needs(ctx : CudaCtx; idx : int; var seen : table<int>; var bi : table<string>; var mem : table<string>) {
    if (key_exists(seen, idx)) {
        return
    }
    seen |> insert(idx)
    for (b in keys(ctx.user_fns[idx].builtins)) {
        bi |> insert(b)
        let dep = builtin_info(b).dep
        if (!empty(dep)) {
            bi |> insert(dep)
        }
    }
    for (m in keys(ctx.user_fns[idx].members)) {
        mem |> insert(m)
    }
    for (c in ctx.user_fns[idx].calls) {
        ufn_all_needs(ctx, c, seen, bi, mem)
    }
}

def private ufn_hidden(ctx : CudaCtx; idx : int) : tuple<builtins : array<string>; members : array<string>> {
    var seen : table<int>
    var bi, mem : table<string>
    ufn_all_needs(ctx, idx, seen, bi, mem)
    var bnames <- [for (b in keys(bi)); b]
    bnames |> sort()
    var mnames <- [for (m in keys(mem)); m]
    mnames |> sort()
    delete seen
    delete bi
    delete mem
    return <- (builtins = bnames, members = mnames)
}

//! the hidden trailing arguments at a call: the builtins by name, then the members' emitted names - the order `hidden_params` declares
def private hidden_args(var ctx : CudaCtx; idx : int) : array<string> {
    var hidden <- ufn_hidden(ctx, idx)
    var args <- hidden.builtins
    args |> reserve(length(args) + length(hidden.members))
    for (m in hidden.members) {
        args |> push(ctx.members[m].name)
    }
    return <- args
}

//! the correctly rounded intrinsic a precise kernel emits for a scalar f32 operator; "" for an operator that has none
def private precise_f32_fn(op : string) : string {
    if (op == "+") {
        return "__fadd_rn"
    } elif (op == "-") {
        return "__fsub_rn"
    } elif (op == "*") {
        return "__fmul_rn"
    } elif (op == "/") {
        return "__fdiv_rn"
    }
    return ""
}

def private float_literal(var ctx : CudaCtx; at : LineInfo; v : float) : string {
    if (!is_finite(v)) {
        shader_error(ctx, at, "non-finite float literal has no CUDA form")
        return "0.0f"
    }
    let s = "{v}"
    if ((s |> find(".")) < 0 && (s |> find("e")) < 0) {
        return "{s}.0f"
    }
    return "{s}f"
}

let private VEC_LANES = fixed_array<string>("x", "y", "z", "w")

def private lane_list(expr : string; n : int; fmt : string) : string {
    let parts <- [for (i in range(n)); fmt |> replace("@", "{expr}.{VEC_LANES[i]}")]
    return join(parts, ", ")
}

//! the half family: a 16-bit float in a ushort with the two PTX converts, arithmetic through float
let private HALF_PRELUDE = "struct half \{
    unsigned short bits;
    __device__ __forceinline__ half() : bits(0) \{\}
    __device__ __forceinline__ explicit half(float f) \{ asm(\"cvt.rn.f16.f32 %0, %1;\" : \"=h\"(bits) : \"f\"(f)); \}
    __device__ __forceinline__ operator float() const \{ float f; asm(\"cvt.f32.f16 %0, %1;\" : \"=f\"(f) : \"h\"(bits)); return f; \}
\};
__device__ __forceinline__ half operator+(half a, half b) \{ return half(float(a) + float(b)); \}
__device__ __forceinline__ half operator-(half a, half b) \{ return half(float(a) - float(b)); \}
__device__ __forceinline__ half operator*(half a, half b) \{ return half(float(a) * float(b)); \}
__device__ __forceinline__ half operator/(half a, half b) \{ return half(float(a) / float(b)); \}
__device__ __forceinline__ half operator-(half a) \{ return half(-float(a)); \}
__device__ __forceinline__ bool operator<(half a, half b) \{ return float(a) < float(b); \}
__device__ __forceinline__ bool operator>(half a, half b) \{ return float(a) > float(b); \}
__device__ __forceinline__ bool operator<=(half a, half b) \{ return float(a) <= float(b); \}
__device__ __forceinline__ bool operator>=(half a, half b) \{ return float(a) >= float(b); \}
__device__ __forceinline__ bool operator==(half a, half b) \{ return float(a) == float(b); \}
__device__ __forceinline__ bool operator!=(half a, half b) \{ return float(a) != float(b); \}
__device__ __forceinline__ half& operator+=(half& a, half b) \{ a = a + b; return a; \}
__device__ __forceinline__ half& operator-=(half& a, half b) \{ a = a - b; return a; \}
__device__ __forceinline__ half& operator*=(half& a, half b) \{ a = a * b; return a; \}
__device__ __forceinline__ half& operator/=(half& a, half b) \{ a = a / b; return a; \}"

//! a half vector: lanes of half, built from and widened to its float twin lane by lane
def private half_vec_prelude(n : int) : string {
    let fl = "float{n}"
    let hv = "half{n}"
    return build_string() $(var w) {
        w |> write("struct {hv} \{ half ")
        for (i in range(n)) {
            w |> write("{i > 0 ? ", " : ""}{VEC_LANES[i]}")
        }
        w |> write("; \};\n")
        w |> write("__device__ __forceinline__ {hv} make_{hv}(")
        for (i in range(n)) {
            w |> write("{i > 0 ? ", " : ""}half {VEC_LANES[i]}")
        }
        w |> write(") \{ {hv} r; ")
        for (i in range(n)) {
            w |> write("r.{VEC_LANES[i]} = {VEC_LANES[i]}; ")
        }
        w |> write("return r; \}\n")
        w |> write("__device__ __forceinline__ {hv} __das_cvt_{hv}_{fl}({fl} v) \{ return make_{hv}({lane_list("v", n, "half(@)")}); \}\n")
        w |> write("__device__ __forceinline__ {fl} __das_cvt_{fl}_{hv}({hv} v) \{ return make_{fl}({lane_list("v", n, "float(@)")}); \}")
    }
}

//! the lane-wise operators CUDA's own vector types lack: vector-vector, vector-scalar and scalar-vector forms of the arithmetic set, the compound forms, unary minus
def private vec_ops_prelude(t : TypeDecl?; integral : bool) : string {
    let ctype = shader_type_name(t)
    let n = component_count(t.baseType)
    let sc = shader_scalar_name(t)
    var ops <- [ "+", "-", "*", "/" ]
    if (integral) {
        ops |> push_from([ "%", "&", "|", "^", "<<", ">>" ])
    }
    var vv, vs, sv : array<string>
    vv |> reserve(4)
    vs |> reserve(4)
    sv |> reserve(4)
    return build_string() $(var w) {
        for (op in ops) {
            vv |> clear()
            vs |> clear()
            sv |> clear()
            for (i in range(n)) {
                let l = VEC_LANES[i]
                vv |> push("a.{l} {op} b.{l}")
                vs |> push("a.{l} {op} b")
                sv |> push("a {op} b.{l}")
            }
            w |> write("__device__ __forceinline__ {ctype} operator{op}({ctype} a, {ctype} b) \{ return make_{ctype}({join(vv, ", ")}); \}\n")
            w |> write("__device__ __forceinline__ {ctype} operator{op}({ctype} a, {sc} b) \{ return make_{ctype}({join(vs, ", ")}); \}\n")
            w |> write("__device__ __forceinline__ {ctype} operator{op}({sc} a, {ctype} b) \{ return make_{ctype}({join(sv, ", ")}); \}\n")
            w |> write("__device__ __forceinline__ {ctype}& operator{op}=({ctype}& a, {ctype} b) \{ a = a {op} b; return a; \}\n")
            w |> write("__device__ __forceinline__ {ctype}& operator{op}=({ctype}& a, {sc} b) \{ a = a {op} b; return a; \}\n")
        }
        if (integral) {
            w |> write("__device__ __forceinline__ {ctype} operator~({ctype} a) \{ return make_{ctype}({lane_list("a", n, "~@")}); \}\n")
        }
        if (sc != "uint" && sc != "ushort" && sc != "uchar") {
            w |> write("__device__ __forceinline__ {ctype} operator-({ctype} a) \{ return make_{ctype}({lane_list("a", n, "-@")}); \}\n")
        }
        let eqs <- [for (i in range(n)); "a.{VEC_LANES[i]} == b.{VEC_LANES[i]}"]
        w |> write("__device__ __forceinline__ bool __das_all_eq_{ctype}({ctype} a, {ctype} b) \{ return {join(eqs, " && ")}; \}\n")
        w |> write("__device__ __forceinline__ bool __das_any_ne_{ctype}({ctype} a, {ctype} b) \{ return !__das_all_eq_{ctype}(a, b); \}")
    }
}

def private need_vec_ops(var ctx : CudaCtx; t : TypeDecl?) {
    let ct = shader_type_name(t)
    return if (empty(ct) || component_count(t.baseType) <= 1)
    if (cuda_is_half_family(t)) {
        need_half(ctx, t)
        return
    }
    register_helper(ctx, "vec.{ct}", vec_ops_prelude(t, !is_float_class(t.baseType)))
}

def private need_half(var ctx : CudaCtx; t : TypeDecl?) {
    register_helper(ctx, "half", HALF_PRELUDE)
    census_kind(ctx, "prelude.half")
    let n = component_count(t.baseType)
    if (n > 1) {
        register_helper(ctx, "half.vec{n}", half_vec_prelude(n))
    }
}

//! a vector built from one scalar, evaluated once
def private need_splat(var ctx : CudaCtx; t : TypeDecl?) : string {
    let ctype = shader_type_name(t)
    let n = component_count(t.baseType)
    let sc = shader_scalar_name(t)
    let key = "splat.{ctype}"
    if (!key_exists(ctx.helpers, key)) {
        let lanes <- [for (_i in range(n)); "v"]
        ctx.helpers[key] = "__device__ __forceinline__ {ctype} __das_splat_{ctype}({sc} v) \{ return make_{ctype}({join(lanes, ", ")}); \}"
    }
    return "__das_splat_{ctype}"
}

//! a lane-wise conversion between two vector types of one lane count
def private need_cvt(var ctx : CudaCtx; dt, st : TypeDecl?) : string {
    let dst = shader_type_name(dt)
    let src = shader_type_name(st)
    let key = "cvt.{dst}.{src}"
    if (!key_exists(ctx.helpers, key) && !cuda_is_half_family(dt) && !cuda_is_half_family(st)) {
        let n = component_count(dt.baseType)
        let lane_fmt = "{shader_scalar_name(dt)}(@)"
        ctx.helpers[key] = "__device__ __forceinline__ {dst} __das_cvt_{dst}_{src}({src} v) \{ return make_{dst}({lane_list("v", n, lane_fmt)}); \}"
    }
    return "__das_cvt_{dst}_{src}"
}

//! a scalar function applied lane by lane; `arity` scalar operands of the same vector type
def private need_lanewise(var ctx : CudaCtx; fn : string; t : TypeDecl?; arity : int) : string {
    let ctype = shader_type_name(t)
    let key = "lane.{fn}.{ctype}"
    let hname = "__das_lw_{fn}_{ctype}"
    if (!key_exists(ctx.helpers, key)) {
        let n = component_count(t.baseType)
        let argNames = fixed_array<string>("a", "b", "c")
        let params <- [for (k in range(arity)); "{ctype} {argNames[k]}"]
        let lanes <- [for (i in range(n)); build_string() $(var lw) {
            lw |> write(fn)
            lw |> write("(")
            for (k in range(arity)) {
                lw |> write("{k > 0 ? ", " : ""}{argNames[k]}.{VEC_LANES[i]}")
            }
            lw |> write(")")
        }]
        ctx.helpers[key] = "__device__ __forceinline__ {ctype} {hname}({join(params, ", ")}) \{ return make_{ctype}({join(lanes, ", ")}); \}"
    }
    return hname
}

//! a swizzle of two or more lanes, the source evaluated once
def private need_swizzle(var ctx : CudaCtx; ctype : string; dst : string; comps : array<int>) : string {
    let names <- [for (f in comps); VEC_LANES[f]]
    let tag = join(names, "")
    let key = "swz.{ctype}.{tag}"
    let hname = "__das_swz_{ctype}_{tag}"
    if (!key_exists(ctx.helpers, key)) {
        let lanes <- [for (f in comps); "v.{VEC_LANES[f]}"]
        ctx.helpers[key] = "__device__ __forceinline__ {dst} {hname}({ctype} v) \{ return make_{dst}({join(lanes, ", ")}); \}"
    }
    return hname
}

//! the warp reductions over all 32 lanes - a butterfly of xor shuffles; the block is a whole number of warps
def private need_warp_reduce(var ctx : CudaCtx; kind : string; ctype : string) : string {
    let key = "warp.{kind}.{ctype}"
    let hname = "__das_warp_{kind}_{ctype}"
    if (!key_exists(ctx.helpers, key)) {
        let fold = kind == "add" ? "v = v + o;" : (kind == "max" ? "v = o > v ? o : v;" : "v = o < v ? o : v;")
        ctx.helpers[key] = "__device__ __forceinline__ {ctype} {hname}({ctype} v) \{ for (int d = 16; d > 0; d >>= 1) \{ {ctype} o = __shfl_xor_sync(0xffffffffu, v, d); {fold} \} return v; \}"
    }
    return hname
}

//! a clustered add: the butterfly over the cluster's lanes alone
def private need_warp_cluster_add(var ctx : CudaCtx; ctype : string) : string {
    let key = "warp.cadd.{ctype}"
    let hname = "__das_warp_cadd_{ctype}"
    if (!key_exists(ctx.helpers, key)) {
        ctx.helpers[key] = "__device__ __forceinline__ {ctype} {hname}({ctype} v, unsigned n) \{ for (unsigned d = n >> 1; d > 0; d >>= 1) \{ v = v + __shfl_xor_sync(0xffffffffu, v, d); \} return v; \}"
    }
    return hname
}

def private need_clamp(var ctx : CudaCtx; ctype : string) : string {
    let key = "clamp.{ctype}"
    let hname = "__das_clamp_{ctype}"
    if (!key_exists(ctx.helpers, key)) {
        ctx.helpers[key] = "__device__ __forceinline__ {ctype} {hname}({ctype} v, {ctype} lo, {ctype} hi) \{ return v < lo ? lo : (v > hi ? hi : v); \}"
    }
    return hname
}

let private USDOT4_PRELUDE = "__device__ __forceinline__ int __das_usdot4(unsigned a, unsigned b) \{ int r = 0; for (int k = 0; k < 4; ++k) \{ r += int((a >> (8 * k)) & 0xffu) * int((signed char)((b >> (8 * k)) & 0xffu)); \} return r; \}"

//! the kernel-class member an `addr(member[expr])` reaches; "" after err for a pointer over anything else or over the @kargs member
def private addr_member(var ctx : CudaCtx; pat : ExprAt?; at : LineInfo) : string {
    let pmem = pat != null ? member_lookup(ctx, pat.subexpr) : ""
    if (empty(pmem)) {
        shader_error(ctx, at, "addr() in a kernel must take a kernel-class member element (addr(member[expr]))")
        return ""
    }
    if (ctx.members[pmem].kargs) {
        shader_error(ctx, at, "addr() over the @kargs member has no CUDA pointer form")
        return ""
    }
    return pmem
}

def private emit_value(var ctx : CudaCtx; e : Expression?) : string {
    if (e == null) {
        shader_error(ctx, LineInfo(), "internal: null expression")
        return "/*null*/"
    }
    if (e is ExprRef2Value) {
        return emit_value(ctx, (e as ExprRef2Value).subexpr)
    } elif (e is ExprUnsafe) {
        return emit_value(ctx, (e as ExprUnsafe).body)
    } elif (e is ExprPtr2Ref) {
        census_kind(ctx, "expr.deref")
        return "(*{emit_value(ctx, (e as ExprPtr2Ref).subexpr)})"
    } elif (e is ExprConstInt) {
        census_kind(ctx, "const.int")
        return "{(e as ExprConstInt).value}"
    } elif (e is ExprConstUInt) {
        census_kind(ctx, "const.uint")
        return uint_literal((e as ExprConstUInt).value)
    } elif (e is ExprConstFloat) {
        census_kind(ctx, "const.float")
        return float_literal(ctx, e.at, (e as ExprConstFloat).value)
    } elif (e is ExprConstFloat16) {
        census_kind(ctx, "const.float16")
        need_half(ctx, e._type)
        return "half({float_literal(ctx, e.at, float((e as ExprConstFloat16).value))})"
    } elif (e is ExprConstBool) {
        census_kind(ctx, "const.bool")
        return (e as ExprConstBool).value ? "true" : "false"
    } elif (e is ExprConstInt2) {
        census_kind(ctx, "const.i32v")
        let v = (e as ExprConstInt2).value
        return "make_int2({v.x}, {v.y})"
    } elif (e is ExprConstInt3) {
        census_kind(ctx, "const.i32v")
        let v = (e as ExprConstInt3).value
        return "make_int3({v.x}, {v.y}, {v.z})"
    } elif (e is ExprConstInt4) {
        census_kind(ctx, "const.i32v")
        let v = (e as ExprConstInt4).value
        return "make_int4({v.x}, {v.y}, {v.z}, {v.w})"
    } elif (e is ExprConstUInt2) {
        census_kind(ctx, "const.u32v")
        let v = (e as ExprConstUInt2).value
        return "make_uint2({uint_literal(v.x)}, {uint_literal(v.y)})"
    } elif (e is ExprConstUInt3) {
        census_kind(ctx, "const.u32v")
        let v = (e as ExprConstUInt3).value
        return "make_uint3({uint_literal(v.x)}, {uint_literal(v.y)}, {uint_literal(v.z)})"
    } elif (e is ExprConstUInt4) {
        census_kind(ctx, "const.u32v")
        let v = (e as ExprConstUInt4).value
        return "make_uint4({uint_literal(v.x)}, {uint_literal(v.y)}, {uint_literal(v.z)}, {uint_literal(v.w)})"
    } elif (e is ExprConstFloat2) {
        census_kind(ctx, "const.f32v")
        let v = (e as ExprConstFloat2).value
        return "make_float2({float_literal(ctx, e.at, v.x)}, {float_literal(ctx, e.at, v.y)})"
    } elif (e is ExprConstFloat3) {
        census_kind(ctx, "const.f32v")
        let v = (e as ExprConstFloat3).value
        return "make_float3({float_literal(ctx, e.at, v.x)}, {float_literal(ctx, e.at, v.y)}, {float_literal(ctx, e.at, v.z)})"
    } elif (e is ExprConstFloat4) {
        census_kind(ctx, "const.f32v")
        let v = (e as ExprConstFloat4).value
        return "make_float4({float_literal(ctx, e.at, v.x)}, {float_literal(ctx, e.at, v.y)}, {float_literal(ctx, e.at, v.z)}, {float_literal(ctx, e.at, v.w)})"
    } elif (e is ExprVar) {
        let ev = e as ExprVar
        let name = string(ev.name)
        if (ev.varFlags.local || ev.varFlags.argument || ev.varFlags._block || key_exists(ctx.renames, name)) {
            if (name == ctx.self_name) {
                shader_error(ctx, ev.at, "`self` used as a value - only member access (self.field) lowers to CUDA")
                return "/*self*/"
            }
            census_kind(ctx, "expr.var")
            return ctx.renames?[name] ?? name
        }
        if (builtin_info(name).ok) {
            census_kind(ctx, "expr.builtin.{name}")
            return name
        }
        let gv = ev.variable
        if (gv != null && gv._type != null && gv._type.flags.constant && gv.init != null) {
            let ce = get_const_expr(compiling_program(), gv.init)
            if (ce != null) {
                census_kind(ctx, "expr.global_const")
                return emit_value(ctx, ce)
            }
        }
        shader_error(ctx, ev.at, "global `{name}` is not a supported builtin - kernels reach data only through @ssbo members, builtin IDs and module `let` constants")
        return "/*{name}*/"
    } elif (e is ExprField) {
        let ef = e as ExprField
        let fieldName = string(ef.name)
        if (is_self_ref(ctx, ef.value) && key_exists(ctx.members, fieldName)) {
            census_kind(ctx, "expr.member")
            return ctx.members[fieldName].name
        }
        let um = member_lookup(ctx, ef.value)
        if (!empty(um) && ctx.members[um].kargs) {
            census_kind(ctx, "expr.member.kargs")
            return "{ctx.members[um].name}.{safe_name(fieldName)}"
        }
        let vt = ef.value._type
        if (vt != null && vt.baseType == Type.tStructure && !empty(elem_struct(ctx, vt.structType, ef.at))) {
            census_kind(ctx, "expr.field.struct")
            return "{emit_value(ctx, ef.value)}.{safe_name(fieldName)}"
        }
        shader_error(ctx, ef.at, "field access `.{fieldName}` is not a kernel-class member access - only self.<@ssbo/@kargs member>, one hop into the @kargs struct and a field of a struct value lower to CUDA")
        return "/*.{fieldName}*/"
    } elif (e is ExprCall && call_base_name("{(e as ExprCall).name}") == "i_das_ptr_add") {
        let pc = e as ExprCall
        census_kind(ctx, "op.ptr_add")
        return "({emit_value(ctx, pc.arguments[0])} + ({emit_value(ctx, pc.arguments[1])}))"
    } elif (e is ExprRef2Ptr) {
        let pat = (e as ExprRef2Ptr).subexpr ?as ExprAt
        let pmem = addr_member(ctx, pat, e.at)
        if (empty(pmem)) {
            return "/*addr*/"
        }
        let pm = ctx.members[pmem]
        census_kind(ctx, pm.workgroup ? "expr.addr.shared" : "expr.addr.global")
        return "({pm.name} + ({emit_value(ctx, pat.index)}))"
    } elif (e is ExprAt) {
        let ea = e as ExprAt
        let sub = emit_value(ctx, ea.subexpr)
        let idx = emit_value(ctx, ea.index)
        let st = ea.subexpr._type
        if (st != null && scalar_class(st.baseType) >= 0 && component_count(st.baseType) > 1) {
            let vt = shader_type_name(st)
            let key = "at.{vt}"
            if (!key_exists(ctx.helpers, key)) {
                let n = component_count(st.baseType)
                let sc0 = shader_scalar_name(st)
                // CUDA's char vectors hold signed char lanes
                let sc = sc0 == "char" ? "signed char" : sc0
                let pick = build_string() $(var w) {
                    for (i in range(n - 1)) {
                        w |> write("(i == {i} ? v.{VEC_LANES[i]} : ")
                    }
                    w |> write("v.{VEC_LANES[n - 1]}")
                    w |> write(repeat(")", n - 1))
                }
                ctx.helpers[key] = "__device__ __forceinline__ {sc}& __das_at_{vt}({vt}& v, int i) \{ return {pick}; \}\n__device__ __forceinline__ const {sc}& __das_at_{vt}(const {vt}& v, int i) \{ return {pick}; \}"
            }
            census_kind(ctx, "expr.index.vector")
            return "__das_at_{vt}({sub}, {idx})"
        }
        census_kind(ctx, "expr.index")
        return "{sub}[{idx}]"
    } elif (e is ExprSwizzle) {
        let sw = e as ExprSwizzle
        let sub = emit_value(ctx, sw.value)
        var comps : array<int>
        comps |> reserve(4)
        for (fch in sw.fields) {
            let f = int(fch)
            if (f < 0 || f > 3) {
                shader_error(ctx, sw.at, "swizzle component {f} out of xyzw range")
                return "/*swizzle*/"
            }
            comps |> push(f)
        }
        if (length(comps) == 1) {
            census_kind(ctx, "expr.swizzle")
            return "{sub}.{VEC_LANES[comps[0]]}"
        }
        let srcType = shader_type_name(sw.value._type)
        let dstType = shader_type_name(sw._type)
        if (empty(srcType) || empty(dstType)) {
            shader_error(ctx, sw.at, "swizzle over {describe(sw.value._type)} has no CUDA form")
            return "/*swizzle*/"
        }
        need_vec_ops(ctx, sw._type)
        census_kind(ctx, "expr.swizzle.multi")
        return "{need_swizzle(ctx, srcType, dstType, comps)}({sub})"
    } elif (e is ExprOp2) {
        let op2 = e as ExprOp2
        let op = string(op2.op)
        if (op2._type != null && op2._type.baseType == Type.tVoid) {
            shader_error(ctx, op2.at, "compound assignment `{op}` used as a value")
            return "/*op {op}*/"
        }
        let cmp = cmp_census_name(op)
        if (!empty(cmp)) {
            let lcls = census_class(op2.left._type)
            let l = emit_value(ctx, op2.left)
            let r = emit_value(ctx, op2.right)
            if (is_vec_class(lcls)) {
                need_vec_ops(ctx, op2.left._type)
                let ct = shader_type_name(op2.left._type)
                if (op == "==") {
                    census_kind(ctx, "op.eq.{lcls}")
                    return "__das_all_eq_{ct}({l}, {r})"
                } elif (op == "!=") {
                    census_kind(ctx, "op.ne.{lcls}")
                    return "__das_any_ne_{ct}({l}, {r})"
                }
                shader_error(ctx, op2.at, "ordered comparison `{op}` on vectors has no scalar das form")
                return "/*op {op}*/"
            }
            if (lcls == "f16") {
                need_half(ctx, op2.left._type)
            }
            census_kind(ctx, "op.{cmp}.{lcls}")
            return "({l} {op} {r})"
        }
        if (op == "&&" || op == "||") {
            let l = emit_value(ctx, op2.left)
            let r = emit_value(ctx, op2.right)
            census_kind(ctx, op == "&&" ? "op.land" : "op.lor")
            return "({l} {op} {r})"
        }
        let opName = op2_census_name(op)
        if (empty(opName)) {
            shader_error(ctx, op2.at, "operator `{op}` is not in the supported CUDA set")
            return "/*op {op}*/"
        }
        let cls = census_class(op2._type)
        if (cls == "?") {
            shader_error(ctx, op2.at, "operator `{op}` result type {describe(op2._type)} has no CUDA form")
            return "/*op {op}*/"
        }
        let l = emit_value(ctx, op2.left)
        let r = emit_value(ctx, op2.right)
        census_kind(ctx, "op.{opName}.{cls}")
        if (is_vec_class(cls)) {
            need_vec_ops(ctx, op2._type)
            if (op == "%" && cls == "f32v") {
                return "{need_lanewise(ctx, "fmodf", op2._type, 2)}({l}, {r})"
            }
        } elif (cls == "f16") {
            need_half(ctx, op2._type)
        } elif (op == "%" && cls == "f32") {
            return "fmodf({l}, {r})"
        } elif (ctx.precise && cls == "f32" && !empty(precise_f32_fn(op))) {
            census_kind(ctx, "op.{opName}.f32.rn")
            return "{precise_f32_fn(op)}({l}, {r})"
        }
        return "({l} {op} {r})"
    } elif (e is ExprOp1) {
        let op1 = e as ExprOp1
        let op = string(op1.op)
        if (op == "+") {
            return emit_value(ctx, op1.subexpr)
        }
        let sub = emit_value(ctx, op1.subexpr)
        if (op == "-") {
            let cls = census_class(op1._type)
            if (is_vec_class(cls)) {
                need_vec_ops(ctx, op1._type)
            } elif (cls == "f16") {
                need_half(ctx, op1._type)
            }
            census_kind(ctx, "op.neg.{cls}")
            return "(-{sub})"
        } elif (op == "!") {
            census_kind(ctx, "op.lnot")
            return "(!{sub})"
        } elif (op == "~") {
            let cls = census_class(op1._type)
            if (is_vec_class(cls)) {
                need_vec_ops(ctx, op1._type)
            }
            census_kind(ctx, "op.bnot.{cls}")
            return "(~{sub})"
        } elif (op == "++") {
            census_kind(ctx, "op.preinc")
            return "(++{sub})"
        } elif (op == "--") {
            census_kind(ctx, "op.predec")
            return "(--{sub})"
        } elif (op == "+++") {
            census_kind(ctx, "op.postinc")
            return "({sub}++)"
        } elif (op == "---") {
            census_kind(ctx, "op.postdec")
            return "({sub}--)"
        }
        shader_error(ctx, op1.at, "unary operator `{op}` is not in the supported CUDA set")
        return "/*op {op}*/"
    } elif (e is ExprOp3) {
        let op3 = e as ExprOp3
        if (op3.op != "?") {
            shader_error(ctx, op3.at, "ternary operator `{op3.op}` is not supported")
            return "/*op {op3.op}*/"
        }
        if (op3.subexpr is ExprConstBool) {
            return emit_value(ctx, (op3.subexpr as ExprConstBool).value ? op3.left : op3.right)
        }
        let c = emit_value(ctx, op3.subexpr)
        let tv = emit_value(ctx, op3.left)
        let fv = emit_value(ctx, op3.right)
        census_kind(ctx, "op.select")
        return "({c} ? {tv} : {fv})"
    } elif (e is ExprInvoke) {
        return emit_invoke_value(ctx, e as ExprInvoke)
    } elif (e is ExprCall) {
        return emit_call_value(ctx, e as ExprCall)
    }
    shader_error(ctx, e.at, "expression {e.__rtti} is not in the supported CUDA subset")
    return "/*{e.__rtti}*/"
}

//! das math names -> the CUDA float function; the integer twins are MATH_INT_*
let private MATH_UNARY : table<string; string> <- {
    "exp" => "expf", "log" => "logf", "exp2" => "exp2f", "log2" => "log2f", "sqrt" => "sqrtf", "rsqrt" => "rsqrtf",
    "sin" => "sinf", "cos" => "cosf", "tanh" => "tanhf", "abs" => "fabsf", "saturate" => "__saturatef",
    "round" => "roundf", "floor" => "floorf", "ceil" => "ceilf", "atan" => "atanf"
}

let private MATH_BINARY : table<string; string> <- {
    "min" => "fminf", "max" => "fmaxf", "atan2" => "atan2f", "pow" => "powf", "fmod" => "fmodf"
}

let private MATH_INT_UNARY : table<string; string> <- { "abs" => "abs" }

let private MATH_INT_BINARY : table<string; string> <- { "min" => "min", "max" => "max" }

let private BITCASTS : table<string; string> <- {
    "uint_bits_to_float" => "__uint_as_float", "int_bits_to_float" => "__int_as_float",
    "float_bits_to_uint" => "__float_as_uint", "float_bits_to_int" => "__float_as_int"
}

//! the subgroup op's operand type spelling; "" after the rejection
def private subgroup_operand(var ctx : CudaCtx; ec : ExprCall?; cname : string) : string =>
    scalar_operand_ok(ctx, ec, cname) ? shader_type_name(ec.arguments[0]._type) : ""

def private math_call(var ctx : CudaCtx; ec : ExprCall?; cname : string; arity : int) : string {
    let na = length(ec.arguments)
    if (na != arity) {
        shader_error(ctx, ec.at, "{cname} takes exactly {arity} argument{arity == 1 ? "" : "s"}")
        return "/*call {cname}*/"
    }
    if (reject_cpu_only_width(ctx, ec.at, ec.arguments[0]._type)) {
        return "/*call {cname}*/"
    }
    let at = ec.arguments[0]._type
    let acls = census_class(at)
    let isF = acls |> starts_with("f")
    let isI = acls |> starts_with("i")
    let isU = acls |> starts_with("u")
    var fn = ""
    if (arity == 1) {
        fn = isF ? (MATH_UNARY?[cname] ?? "") : (isI ? (MATH_INT_UNARY?[cname] ?? "") : "")
    } elif (arity == 2) {
        fn = isF ? (MATH_BINARY?[cname] ?? "") : ((isI || isU) ? (MATH_INT_BINARY?[cname] ?? "") : "")
    } elif (cname == "mad") {
        fn = isF ? "fmaf" : ""
    } elif (cname == "clamp") {
        fn = (isF || isI || isU) ? need_clamp(ctx, shader_scalar_name(at)) : ""
    }
    if (empty(fn)) {
        shader_error(ctx, ec.at, "{cname} lowers for {arity == 1 && cname != "abs" ? "float" : "numeric"} classes only - argument is {acls}")
        return "/*call {cname}*/"
    }
    if (acls == "f16" || acls == "f16v") {
        shader_error(ctx, ec.at, "{cname} on the half family has no CUDA form - widen to float first")
        return "/*call {cname}*/"
    }
    let args <- [for (arg in ec.arguments); emit_value(ctx, arg)]
    census_kind(ctx, "call.math.{cname}.{acls}")
    if (is_vec_class(acls)) {
        need_vec_ops(ctx, at)
        return "{need_lanewise(ctx, fn, at, arity)}({join(args, ", ")})"
    }
    return "{fn}({join(args, ", ")})"
}

def private atomic_call(var ctx : CudaCtx; ec : ExprCall?; cname : string) : string {
    let na = length(ec.arguments)
    let want = cname == "atomicCompSwap" ? 3 : 2
    if (na != want) {
        shader_error(ctx, ec.at, "{cname} takes exactly {want} arguments")
        return "/*call {cname}*/"
    }
    let acls = census_class(ec.arguments[0]._type)
    if (!(acls == "i32" || acls == "u32" || (acls == "f32" && (cname == "atomicAdd" || cname == "atomicExchange")))) {
        shader_error(ctx, ec.at, "{cname} lowers on int/uint memory (atomicAdd and atomicExchange on float too) - the operand is {acls}")
        return "/*call {cname}*/"
    }
    let cfn = cname == "atomicExchange" ? "atomicExch" : (cname == "atomicCompSwap" ? "atomicCAS" : cname)
    var args : array<string>
    args |> reserve(na)
    args |> push("&({emit_value(ctx, ec.arguments[0])})")
    for (i in range(1, na)) {
        args |> push(emit_value(ctx, ec.arguments[i]))
    }
    census_kind(ctx, "call.{cname}.{acls}")
    return "{cfn}({join(args, ", ")})"
}

//! the CUDA-only intrinsics a body calls as statements
let private INTRINSIC_STATEMENTS : table<string> <- { "cp_async_16", "cp_async_commit", "cp_async_wait", "ldmatrix", "mma_scale_acc", "bar_sync", "cuda_asm" }

//! the address space a pointer expression reaches: "shared" for a @workgroup member, "global" for a buffer, "" when unknown
def private ptr_space(ctx : CudaCtx; e : Expression?) : string {
    var x = e
    while (x != null && (x is ExprRef2Value || x is ExprUnsafe)) {
        x = x is ExprUnsafe ? (x as ExprUnsafe).body : (x as ExprRef2Value).subexpr
    }
    return "" if (x == null)
    var mem = ""
    if (x is ExprRef2Ptr) {
        let pat = (x as ExprRef2Ptr).subexpr ?as ExprAt
        mem = pat != null ? member_lookup(ctx, pat.subexpr) : ""
    } elif (x is ExprVar) {
        mem = ctx.ptrs?[string((x as ExprVar).name)] ?? ""
    } elif (x is ExprCall && call_base_name("{(x as ExprCall).name}") == "i_das_ptr_add") {
        return ptr_space(ctx, (x as ExprCall).arguments[0])
    }
    var space = ""
    ctx.members |> get(mem) $(m) {
        space = m.workgroup ? "shared" : "global"
    }
    return space
}

//! a das string as a C string literal's body
def private c_escape(s : string) : string =>
    s |> replace("\\", "\\\\") |> replace("\"", "\\\"") |> replace("\n", "\\n")

//! the CUDA-only tier (`cuda_intrinsics`): cp.async, ldmatrix, the block-scaled accumulate, named barriers, byte_perm,
//! clock32 and the inline-PTX floor - "" when `cname` is none of them
def private cuda_intrinsic_call(var ctx : CudaCtx; ec : ExprCall?; cname : string) : string {
    let na = length(ec.arguments)
    if (cname == "cp_async_16") {
        if (na != 2 || ptr_space(ctx, ec.arguments[0]) != "shared" || ptr_space(ctx, ec.arguments[1]) != "global") {
            shader_error(ctx, ec.at, "cp_async_16(dst, src) copies 16 bytes from a buffer pointer (addr(member[i]) or a pointer local to one) into a @workgroup array pointer")
            return "/*cp_async_16*/"
        }
        need_cp_async(ctx)
        census_kind(ctx, "call.cp_async_16")
        return "__das_cp_async_16({emit_value(ctx, ec.arguments[0])}, {emit_value(ctx, ec.arguments[1])})"
    }
    if (cname == "cp_async_commit") {
        need_cp_async(ctx)
        census_kind(ctx, "call.cp_async_commit")
        return "__das_cp_async_commit()"
    }
    if (cname == "cp_async_wait") {
        let n = na == 1 && ec.arguments[0] is ExprConstInt ? (ec.arguments[0] as ExprConstInt).value : -1
        if (n < 0) {
            shader_error(ctx, ec.at, "cp_async_wait(pending) takes a constant: the groups still allowed in flight")
            return "/*cp_async_wait*/"
        }
        need_cp_async_wait(ctx, n)
        census_kind(ctx, "call.cp_async_wait")
        return "__das_cp_async_wait_{n}()"
    }
    if (cname == "ldmatrix") {
        let ci = na == 2 ? coop_type_of(ec.arguments[0]._type) : CoopInfo(ok = false)
        if (!ci.ok || ci.use == "C" || ptr_space(ctx, ec.arguments[1]) != "shared") {
            shader_error(ctx, ec.at, "ldmatrix(fragment, p) loads a KHR A or B fragment off a @workgroup array pointer (the lane's 16-byte row)")
            return "/*ldmatrix*/"
        }
        need_coopmat(ctx)
        let hname = "__das_ldm_{ci.ctype}"
        register_helper(ctx, "{HELPER_RANK_INTRINSIC}ldm.{hname}", "__device__ __forceinline__ void {hname}({ci.ctype}& m, const void* p) \{ unsigned a = (unsigned)__cvta_generic_to_shared(p); asm volatile(\"ldmatrix.sync.aligned.m8n8.x4.shared.b16 \{%0,%1,%2,%3\}, [%4];\" : \"=r\"(m.r[0]), \"=r\"(m.r[1]), \"=r\"(m.r[2]), \"=r\"(m.r[3]) : \"r\"(a)); \}")
        census_kind(ctx, "call.ldmatrix.{ci.use}.{ci.comp}")
        return "{hname}({emit_value(ctx, ec.arguments[0])}, {emit_value(ctx, ec.arguments[1])})"
    }
    if (cname == "mma_scale_acc") {
        let fi = na == 4 ? coop_type_of(ec.arguments[0]._type) : CoopInfo(ok = false)
        let si = na == 4 ? coop_type_of(ec.arguments[1]._type) : CoopInfo(ok = false)
        if (!(fi.ok && si.ok && fi.comp == "f32" && si.comp == "s32")) {
            shader_error(ctx, ec.at, "mma_scale_acc(f32 accumulator, s32 accumulator, row scales, column scales)")
            return "/*mma_scale_acc*/"
        }
        need_coopmat(ctx)
        register_helper(ctx, "{HELPER_RANK_INTRINSIC}mma_scale_acc", "__device__ __forceinline__ void __das_mma_scale_acc(__das_cmC_f32& f, const __das_cmC_s32& s, float2 rs, float4 cs) \{ float r[2] = \{ rs.x, rs.y \}; float c[4] = \{ cs.x, cs.y, cs.z, cs.w \}; _Pragma(\"unroll\") for (int k = 0; k < 8; ++k) \{ f.r[k] += float(s.r[k]) * r[(k >> 1) & 1] * c[((k >> 2) << 1) | (k & 1)]; \} \}")
        census_kind(ctx, "call.mma_scale_acc")
        return "__das_mma_scale_acc({emit_value(ctx, ec.arguments[0])}, {emit_value(ctx, ec.arguments[1])}, {emit_value(ctx, ec.arguments[2])}, {emit_value(ctx, ec.arguments[3])})"
    }
    if (cname == "bar_sync") {
        if (na != 2) {
            shader_error(ctx, ec.at, "bar_sync(id, count)")
            return "/*bar_sync*/"
        }
        register_helper(ctx, "{HELPER_RANK_INTRINSIC}bar_sync", "__device__ __forceinline__ void __das_bar_sync(unsigned id, unsigned count) \{ asm volatile(\"bar.sync %0, %1;\" :: \"r\"(id), \"r\"(count) : \"memory\"); \}")
        census_kind(ctx, "call.bar_sync")
        return "__das_bar_sync({emit_value(ctx, ec.arguments[0])}, {emit_value(ctx, ec.arguments[1])})"
    }
    if (cname == "byte_perm") {
        if (na != 3) {
            shader_error(ctx, ec.at, "byte_perm(a, b, sel)")
            return "/*byte_perm*/"
        }
        census_kind(ctx, "call.byte_perm")
        return "__byte_perm({emit_value(ctx, ec.arguments[0])}, {emit_value(ctx, ec.arguments[1])}, {emit_value(ctx, ec.arguments[2])})"
    }
    if (cname == "clock32") {
        census_kind(ctx, "call.clock32")
        return "(unsigned)clock()"
    }
    if (cname == "cuda_asm" || cname == "cuda_asm_value") {
        let ret = cname == "cuda_asm_value"
        let fs = na >= 1 ? ec.arguments[0] ?as ExprConstString : null
        if (fs == null || na > 4) {
            shader_error(ctx, ec.at, "{cname}(fmt [, a0 [, a1 [, a2]]]) takes a constant PTX string and up to three 32-bit operands")
            return "/*{cname}*/"
        }
        let fmt = c_escape(string(fs.value))
        let n = na - 1
        let site = length(ctx.helpers)
        let hname = "__das_asm{ret ? "_r" : ""}_{site}"
        let params <- [for (i in range(n)); "unsigned a{i}"]
        let ins <- [for (i in range(n)); "\"r\"(a{i})"]
        let body = (ret ?
            "unsigned r; asm volatile(\"{fmt}\" : \"=r\"(r){n > 0 ? " : " : ""}{join(ins, ", ")} : \"memory\"); return r;" :
            "asm volatile(\"{fmt}\" :: {join(ins, ", ")} : \"memory\");")
        register_helper(ctx, "{HELPER_RANK_INTRINSIC}asm.{site}", "__device__ __forceinline__ {ret ? "unsigned" : "void"} {hname}({join(params, ", ")}) \{ {body} \}")
        census_kind(ctx, "call.{cname}.{n}")
        let args <- [for (i in range(1, na)); emit_value(ctx, ec.arguments[i])]
        return "{hname}({join(args, ", ")})"
    }
    return ""
}

def private emit_call_value(var ctx : CudaCtx; ec : ExprCall?) : string {
    let cname = call_base_name("{ec.name}")
    let na = length(ec.arguments)
    let intrinsic = cuda_intrinsic_call(ctx, ec, cname)
    if (!empty(intrinsic)) {
        return intrinsic
    }
    if (key_exists(MATH_UNARY, cname)) {
        return math_call(ctx, ec, cname, 1)
    }
    if (key_exists(MATH_BINARY, cname)) {
        return math_call(ctx, ec, cname, 2)
    }
    if (cname == "clamp" || cname == "mad") {
        return math_call(ctx, ec, cname, 3)
    }
    if (key_exists(BITCASTS, cname)) {
        if (na != 1) {
            shader_error(ctx, ec.at, "{cname} takes exactly one argument")
            return "/*call {cname}*/"
        }
        if (reject_cpu_only_width(ctx, ec.at, ec._type)) {
            return "/*call {cname}*/"
        }
        let v = emit_value(ctx, ec.arguments[0])
        let scls = census_class(ec.arguments[0]._type)
        census_kind(ctx, "call.bitcast.{scls}.{census_class(ec._type)}")
        if (is_vec_class(scls)) {
            need_vec_ops(ctx, ec._type)
            return "{need_lanewise(ctx, BITCASTS?[cname] ?? "", ec._type, 1)}({v})"
        }
        return "{BITCASTS?[cname] ?? ""}({v})"
    }
    if (cname == "coopmatMulAdd") {
        if (na == 3) {
            let wa = wg_type_of(ctx, ec.arguments[0]._type)
            let wb = wg_var_info(ctx, ec.arguments[1])
            let wc = wg_type_of(ctx, ec.arguments[2]._type)
            if (wa.ok || wb.ok || wc.ok) {
                return emit_wg_mul_add(ctx, ec, wa, wb, wc)
            }
        }
        return emit_coop_mul_add(ctx, ec)
    }
    if (cname == "packHalf2x16" || cname == "unpackHalf2x16") {
        if (na != 1) {
            shader_error(ctx, ec.at, "{cname} takes exactly one argument")
            return "/*call {cname}*/"
        }
        let v = emit_value(ctx, ec.arguments[0])
        census_kind(ctx, "call.{cname}")
        if (cname == "packHalf2x16") {
            register_helper(ctx, "packHalf2x16", PACK_HALF2_PRELUDE)
            return "__das_pack_half2({v})"
        }
        register_helper(ctx, "unpackHalf2x16", UNPACK_HALF2_PRELUDE)
        return "__das_unpack_half2({v})"
    }
    if (cname == "unpack8" || cname == "pack32") {
        if (na != 1) {
            shader_error(ctx, ec.at, "{cname} takes exactly one argument")
            return "/*call {cname}*/"
        }
        let st = ec.arguments[0]._type
        let dt = ec._type
        let sT = shader_type_name(st)
        let dT = shader_type_name(dt)
        if (empty(sT) || empty(dT) || reject_cpu_only_width(ctx, ec.at, dt)) {
            shader_error(ctx, ec.at, "{cname} lowers between a 32/16-bit integer and its byte lanes - {describe(st)} -> {describe(dt)} has no CUDA form")
            return "/*call {cname}*/"
        }
        let v = emit_value(ctx, ec.arguments[0])
        census_kind(ctx, "call.{cname}.{sT}.{dT}")
        if (cname == "unpack8") {
            let key = "unpack8.{dT}"
            if (!key_exists(ctx.helpers, key)) {
                let lane = shader_scalar_name(dt)
                let parts <- [for (i in range(component_count(dt.baseType))); i == 0 ? "{lane}(x)" : "{lane}(x >> {i * 8})"]
                ctx.helpers[key] = "__device__ __forceinline__ {dT} __das_unpack8_{dT}({sT} x) \{ return make_{dT}({join(parts, ", ")}); \}"
            }
            return "__das_unpack8_{dT}({v})"
        }
        let key = "pack32.{sT}"
        if (!key_exists(ctx.helpers, key)) {
            let parts <- [for (i in range(component_count(st.baseType))); i == 0 ? "{dT}(uchar(v.x))" : "({dT}(uchar(v.{VEC_LANES[i]})) << {i * 8})"]
            ctx.helpers[key] = "__device__ __forceinline__ {dT} __das_pack32_{sT}({sT} v) \{ return {join(parts, " | ")}; \}"
        }
        return "__das_pack32_{sT}({v})"
    }
    if (cname == "sdot4" || cname == "usdot4") {
        if (na != 2) {
            shader_error(ctx, ec.at, "{cname} takes exactly two arguments")
            return "/*call {cname}*/"
        }
        let a = emit_value(ctx, ec.arguments[0])
        let b = emit_value(ctx, ec.arguments[1])
        census_kind(ctx, "call.{cname}")
        if (cname == "sdot4") {
            return "__dp4a(int({a}), int({b}), 0)"
        }
        register_helper(ctx, "usdot4", USDOT4_PRELUDE)
        return "__das_usdot4({a}, {b})"
    }
    if (cname == "subgroupAdd" || cname == "subgroupMax" || cname == "subgroupMin") {
        if (na != 1) {
            shader_error(ctx, ec.at, "{cname} takes exactly one argument")
            return "/*call {cname}*/"
        }
        let ct = subgroup_operand(ctx, ec, cname)
        return "/*call {cname}*/" if (empty(ct))
        let v = emit_value(ctx, ec.arguments[0])
        census_kind(ctx, "call.{cname}.{census_class(ec._type)}")
        let kind = cname == "subgroupAdd" ? "add" : (cname == "subgroupMax" ? "max" : "min")
        return "{need_warp_reduce(ctx, kind, ct)}({v})"
    } elif (cname == "subgroupShuffle" || cname == "subgroupShuffleXor" || cname == "subgroupBroadcast") {
        if (na != 2) {
            shader_error(ctx, ec.at, "{cname} takes exactly two arguments")
            return "/*call {cname}*/"
        }
        let ct = subgroup_operand(ctx, ec, cname)
        return "/*call {cname}*/" if (empty(ct))
        let v = emit_value(ctx, ec.arguments[0])
        let l = emit_value(ctx, ec.arguments[1])
        census_kind(ctx, "call.{cname}.{census_class(ec._type)}")
        return "{cname == "subgroupShuffleXor" ? "__shfl_xor_sync" : "__shfl_sync"}(__activemask(), {v}, {l})"
    } elif (cname == "subgroupBroadcastFirst") {
        if (na != 1) {
            shader_error(ctx, ec.at, "{cname} takes exactly one argument")
            return "/*call {cname}*/"
        }
        let ct = subgroup_operand(ctx, ec, cname)
        return "/*call {cname}*/" if (empty(ct))
        census_kind(ctx, "call.{cname}.{census_class(ec._type)}")
        return "__shfl_sync(__activemask(), {emit_value(ctx, ec.arguments[0])}, __ffs(__activemask()) - 1)"
    } elif (cname == "subgroupElect") {
        use_builtin(ctx, "gl_SubgroupInvocationID")
        census_kind(ctx, "call.subgroupElect")
        return "(gl_SubgroupInvocationID == (unsigned)(__ffs(__activemask()) - 1))"
    } elif (cname == "subgroupAll" || cname == "subgroupAny") {
        if (na != 1) {
            shader_error(ctx, ec.at, "{cname} takes exactly one argument")
            return "/*call {cname}*/"
        }
        census_kind(ctx, "call.{cname}")
        return "{cname == "subgroupAll" ? "__all_sync" : "__any_sync"}(__activemask(), {emit_value(ctx, ec.arguments[0])})"
    } elif (cname == "subgroupClusteredAdd") {
        if (na != 2) {
            shader_error(ctx, ec.at, "{cname} takes exactly two arguments")
            return "/*call {cname}*/"
        }
        let ct = subgroup_operand(ctx, ec, cname)
        return "/*call {cname}*/" if (empty(ct))
        census_kind(ctx, "call.{cname}.{census_class(ec._type)}")
        return "{need_warp_cluster_add(ctx, ct)}({emit_value(ctx, ec.arguments[0])}, {emit_value(ctx, ec.arguments[1])})"
    } elif (cname |> starts_with("atomic")) {
        return atomic_call(ctx, ec, cname)
    }
    if (ec._type != null && reject_cpu_only_width(ctx, ec.at, ec._type)) {
        return "/*call {cname}*/"
    }
    let result_type_spelling = ec._type != null ? das_type_spelling(ec._type.baseType) : ""
    if (!empty(result_type_spelling) && na >= 1
            && (cname == result_type_spelling || (cname == "half" && result_type_spelling == "float16"))) {
        let dstT = shader_type_name(ec._type)
        let lanes = component_count(ec._type.baseType)
        let dst = census_class(ec._type)
        if (cuda_is_half_family(ec._type)) {
            need_half(ctx, ec._type)
        }
        if (lanes == 1) {
            if (na != 1) {
                shader_error(ctx, ec.at, "{cname} takes exactly one argument")
                return "/*call {cname}*/"
            }
            let src = census_class(ec.arguments[0]._type)
            if (ec.arguments[0]._type != null && cuda_is_half_family(ec.arguments[0]._type)) {
                need_half(ctx, ec.arguments[0]._type)
            }
            census_kind(ctx, src == dst ? "ctor.{dst}" : "cvt.{src}.{dst}")
            return "{dstT}({emit_value(ctx, ec.arguments[0])})"
        }
        need_vec_ops(ctx, ec._type)
        if (na == lanes) {
            let args <- [for (arg in ec.arguments); emit_value(ctx, arg)]
            census_kind(ctx, "ctor.{dst}")
            return "make_{dstT}({join(args, ", ")})"
        }
        if (na == 1) {
            let st = ec.arguments[0]._type
            let src = census_class(st)
            let v = emit_value(ctx, ec.arguments[0])
            if (!is_vec_class(src)) {
                census_kind(ctx, "ctor.splat.{dst}")
                return "{need_splat(ctx, ec._type)}({v})"
            }
            if (component_count(st.baseType) == lanes) {
                let srcT = shader_type_name(st)
                if (srcT == dstT) {
                    census_kind(ctx, "ctor.{dst}")
                    return v
                }
                if (cuda_is_half_family(st)) {
                    need_half(ctx, st)
                }
                census_kind(ctx, "cvt.{src}.{dst}")
                return "{need_cvt(ctx, ec._type, st)}({v})"
            }
        }
        shader_error(ctx, ec.at, "{cname}(...) takes {lanes} scalars, one scalar (splat) or one {lanes}-lane vector (convert)")
        return "/*call {cname}*/"
    }
    if (!empty(result_type_spelling) && na == 1 && cname == "{result_type_spelling}_sat") {
        let rbt = ec._type.baseType
        let w = scalar_width(rbt)
        let st = ec.arguments[0]._type
        if (scalar_class(rbt) >= 0 && !is_float_class(rbt) && w < 32 && st != null && component_count(st.baseType) == component_count(rbt)) {
            let v = emit_value(ctx, ec.arguments[0])
            var lo = "0"
            var hi = w == 16 ? "65535" : "255"
            if (is_signed_class(rbt)) {
                lo = w == 16 ? "-32768" : "-128"
                hi = w == 16 ? "32767" : "127"
            }
            let sc = shader_scalar_name(st)
            let clampf = need_clamp(ctx, sc)
            census_kind(ctx, "cvt_sat.{census_class(st)}.{census_class(ec._type)}")
            let dstT = shader_type_name(ec._type)
            let lanes = component_count(ec._type.baseType)
            if (lanes == 1) {
                return "{dstT}({clampf}({v}, {lo}, {hi}))"
            }
            let key = "sat.{dstT}.{shader_type_name(st)}"
            let hname = "__das_sat_{dstT}_{shader_type_name(st)}"
            if (!key_exists(ctx.helpers, key)) {
                let lane_fmt = "{shader_scalar_name(ec._type)}({clampf}(@, {lo}, {hi}))"
                ctx.helpers[key] = "__device__ __forceinline__ {dstT} {hname}({shader_type_name(st)} v) \{ return make_{dstT}({lane_list("v", lanes, lane_fmt)}); \}"
            }
            return "{hname}({v})"
        }
    }
    if (is_user_fn(ec.func)) {
        let ui = ctx.user_fn_index?[user_fn_key(ec.func)] ?? -1
        if (ui < 0) {
            shader_error(ctx, ec.at, "internal: user function `{cname}` was not discovered at scan time")
            return "/*call {cname}*/"
        }
        var args <- [for (i in range(na)); emit_value(ctx, ec.arguments[i])]
        let hidden <- hidden_args(ctx, ui)
        args |> push_from(hidden)
        census_kind(ctx, "call.user")
        return "{ctx.user_fns[ui].name}({join(args, ", ")})"
    }
    shader_error(ctx, ec.at, "call `{cname}` is not a supported CUDA builtin and not a lowerable user function - kernels reach only the CUDA subset")
    return "/*call {cname}*/"
}

//! a method call: the devirtualized function with the caller's arguments, then what the body needs
def private emit_invoke_value(var ctx : CudaCtx; ei : ExprInvoke?) : string {
    let mf = length(ei.arguments) >= 2 ? invoke_method_field(ei.arguments[0]) : null
    if (!ei.isInvokeMethod || mf == null || !is_self_ref(ctx, ei.arguments[1])) {
        shader_error(ctx, ei.at, "invoke is not a self method call - only self.method(...) lowers")
        return "/*invoke*/"
    }
    let mname = string(mf.name)
    let mfn = resolve_method(ctx, mname, ei.at)
    if (mfn == null) {
        return "/*invoke {mname}*/"
    }
    let ui = ctx.user_fn_index?[user_fn_key(mfn)] ?? -1
    if (ui < 0) {
        shader_error(ctx, ei.at, "internal: method `{mname}` was not discovered at scan time")
        return "/*invoke {mname}*/"
    }
    var args <- [for (i in range(2, length(ei.arguments))); emit_value(ctx, ei.arguments[i])]
    let hidden <- hidden_args(ctx, ui)
    args |> push_from(hidden)
    census_kind(ctx, "call.method")
    return "{ctx.user_fns[ui].name}({join(args, ", ")})"
}

def private emit_store_target(var ctx : CudaCtx; e : Expression?) : string => emit_value(ctx, e)

def private local_name(ctx : CudaCtx; name : string) : string {
    let base = safe_name(name)
    return key_exists(ctx.members, name) ? "{base}_l" : base
}

//! a `let` fixed-array local with an all-literal initializer lowers to a __constant__ table
def private hoist_const_table(var ctx : CudaCtx; v : VariablePtr) {
    let et = v._type.firstType
    let n = v._type.fixedDim
    if (!v._type.flags.constant) {
        shader_error(ctx, v.at, "fixed-array local `{v.name}` initializer hoists to a __constant__ table - it must be a `let` (a written table has no constant form)")
        return
    }
    if (reject_cpu_only_width(ctx, v.at, et)) {
        return
    }
    let ename = shader_type_name(et)
    if (empty(ename)) {
        shader_error(ctx, v.at, "local array `{v.name}` element type {describe(et)} has no CUDA form")
        return
    }
    var mk : ExprMakeArray?
    if (v.init is ExprMakeArray) {
        mk = v.init as ExprMakeArray
    } elif (v.init is ExprCall) {
        let ec = v.init as ExprCall
        if (length(ec.arguments) == 1 && ec.arguments[0] is ExprMakeArray) {
            mk = ec.arguments[0] as ExprMakeArray
        }
    }
    if (mk == null || length(mk.values) != n) {
        shader_error(ctx, v.at, "fixed-array local `{v.name}` hoists to a __constant__ table - its initializer must be a `fixed_array<T>(...)` of exactly {n} elements")
        return
    }
    var vals : array<string>
    vals |> reserve(n)
    for (el in mk.values) {
        if (!(el is ExprConstInt || el is ExprConstUInt || el is ExprConstFloat || el is ExprConstInt8 || el is ExprConstUInt8 ||
                el is ExprConstInt16 || el is ExprConstUInt16)) {
            shader_error(ctx, v.at, "fixed-array local `{v.name}` hoists to a __constant__ table - every element must be an integer or float scalar literal")
            return
        }
        vals |> push(emit_value(ctx, el))
    }
    let vtxt = build_string() $(var w) {
        for (i in range(n)) {
            if (i % 8 == 0) {
                w |> write("    ")
            }
            w |> write(vals[i])
            if (i != n - 1) {
                w |> write(",")
            }
            if (i % 8 == 7 || i == n - 1) {
                w |> write("\n")
            } else {
                w |> write(" ")
            }
        }
    }
    let aname = string(v.name)
    var an = local_name(ctx, aname)
    var txt = "__constant__ {ename} {an}[{n}] = \{\n{vtxt}\};"
    if (key_exists(ctx.const_tables, an) && ctx.const_tables[an] != txt) {
        var k = 1
        while (key_exists(ctx.const_tables, "{an}_c{k}")) {
            k++
        }
        an = "{an}_c{k}"
        txt = "__constant__ {ename} {an}[{n}] = \{\n{vtxt}\};"
        census_kind(ctx, "stmt.let.const_table.renamed")
    }
    ctx.renames[aname] = an
    ctx.const_tables[an] = txt
    census_kind(ctx, "stmt.let.const_table")
}

//! a statement, then whatever the statement's own emission asked to follow it (a decoded tile's deferred decode pass after
//! the product over it) - in the statement's own scope, since a compound statement's inner statements flush inside it
def private emit_stmt(var ctx : CudaCtx; e : Expression?; indent : string; var lines : array<string>) {
    emit_stmt_inner(ctx, e, indent, lines)
    if (!empty(ctx.flush_after)) {
        lines |> reserve(length(lines) + length(ctx.flush_after))
        for (t in ctx.flush_after) {
            lines |> push("{indent}{t}")
        }
        ctx.flush_after |> clear()
    }
}

[arch(at="../ARCHITECTURE_EMIT.md#cuda-tile-slot-reuse"), arch(at="../ARCHITECTURE_EMIT.md#cuda-decoded-tile-pipeline")]
def private emit_stmt_inner(var ctx : CudaCtx; e : Expression?; indent : string; var lines : array<string>) {
    if (e == null) {
        return
    }
    if (e is ExprBlock) {
        for (s in (e as ExprBlock).list) {
            emit_stmt(ctx, s, indent, lines)
        }
    } elif (e is ExprUnsafe) {
        emit_stmt(ctx, (e as ExprUnsafe).body, indent, lines)
    } elif (e is ExprWith) {
        emit_stmt(ctx, (e as ExprWith).body, indent, lines)
    } elif (e is ExprLet) {
        for (v in (e as ExprLet).variables) {
            if (v.init != null && v.init is ExprRef2Ptr && (v.init as ExprRef2Ptr).subexpr is ExprAt) {
                let pat = (v.init as ExprRef2Ptr).subexpr as ExprAt
                let pmem = addr_member(ctx, pat, v.at)
                if (empty(pmem)) {
                    continue
                }
                let pm = ctx.members[pmem]
                let pname0 = string(v.name)
                ctx.renames[pname0] = local_name(ctx, pname0)
                ctx.ptrs[pname0] = pmem
                let base = emit_value(ctx, pat.index)
                let isc = !pm.workgroup && !pm.written
                census_kind(ctx, pm.workgroup ? "decl.ptr.shared" : "decl.ptr.global")
                lines |> push("{indent}{isc ? "const " : ""}{pm.elem}* {ctx.renames[pname0]} = {pm.name} + ({base});")
                continue
            }
            if (v._type != null && v._type.baseType == Type.tFixedArray && v._type.firstType != null &&
                    v._type.firstType.baseType != Type.tFixedArray) {
                if (v.init != null) {
                    hoist_const_table(ctx, v)
                    continue
                }
                let et = v._type.firstType
                let n = v._type.fixedDim
                let aname = string(v.name)
                ctx.renames[aname] = local_name(ctx, aname)
                if (reject_cpu_only_width(ctx, v.at, et)) {
                    continue
                }
                let ename = type_spell(ctx, et, v.at)
                if (empty(ename)) {
                    if (et.baseType != Type.tStructure) {
                        shader_error(ctx, v.at, "local array `{v.name}` element type {describe(et)} has no CUDA form")
                    }
                    continue
                }
                if (cuda_is_half_family(et)) {
                    need_half(ctx, et)
                }
                census_kind(ctx, "stmt.var.array")
                lines |> push("{indent}{ename} {ctx.renames[aname]}[{n}] = \{\};")
                continue
            }
            if (reject_cpu_only_width(ctx, v.at, v._type)) {
                continue
            }
            let wgi = wg_var_info_named(ctx, string(v.name), v._type)
            if (wgi.ok && wgi.global_view) {   // a view of its source, held in every thread's registers: nothing carved, no barrier
                need_wg(ctx, wgi)
                let gname = string(v.name)
                ctx.renames[gname] = local_name(ctx, gname)
                census_kind(ctx, "stmt.var.wg_view")
                lines |> push("{indent}{wgi.ctype} {ctx.renames[gname]} = \{\};")
                continue
            }
            if (wgi.ok && wgi.in_smem) {
                need_wg(ctx, wgi)
                let sname0 = string(v.name)
                ctx.renames[sname0] = local_name(ctx, sname0)
                if (key_exists(ctx.ones_sum, sname0)) {   // the ones tile has no shared form
                    census_kind(ctx, "stmt.var.wg_ones")
                    continue
                }
                let need = uint64(wgi.padded_elems) * (wgi.comp == "f16" ? 2ul : 1ul)
                let life = ctx.tiles?[sname0] ?? TileLife(ambiguous = true)
                let pipelined_dec = wgi.use == "A" && wgi.comp == "f16" && life.decoded
                let pipelined_raw = wgi.comp == "s8" && life.raw_s8
                if (pipelined_dec || pipelined_raw) {
                    let nslots = pipelined_raw ? uint64(RAW_S8_RING) : 2ul
                    let doff = smem_carve(ctx, need * nslots)
                    ctx.decode_slot_pairs[sname0] = RawRing(off = doff, slot_bytes = need)
                    let pname = "__p_{ctx.renames[sname0]}"
                    ctx.renames[sname0] = "(*{pname})"
                    census_kind(ctx, pipelined_raw ? "stmt.var.wg_shared.ring" : "stmt.var.wg_shared.dbl")
                    // at kernel scope: a tile local declared inside the k loop would otherwise reset the ring's state a step, and no step predicts
                    ctx.hoisted |> push("    {wgi.ctype}* {pname} = ({wgi.ctype}*)(__das_smem + {int(doff)});")   // nolint:STYLE012 - the function's hoisted lines accumulate over its tiles
                    // the step decoded and ready, in every thread's registers (the values are uniform): base, row, column, slot (-1: none)
                    ctx.hoisted |> push("    uint {pname}_b = 0u, {pname}_r = 0u, {pname}_c = 0u; int {pname}_s = -1; bool {pname}_bar = false;")
                    continue
                }
                var best = -1
                if (!life.ambiguous && !life.convert_target) {
                    for (i in range(length(ctx.slots))) {
                        let sl = ctx.slots[i]
                        continue if (sl.bytes < need || !key_exists(ctx.tiles, sl.owner))
                        let holder = ctx.tiles[sl.owner]
                        continue if (holder.ambiguous || !before(holder.last, pos_of(v.at)))
                        if (best < 0 || sl.bytes < ctx.slots[best].bytes) {
                            best = i
                        }
                    }
                }
                var off = 0ul
                if (best >= 0) {
                    ctx.slots[best].owner = sname0
                    off = ctx.slots[best].off
                    census_kind(ctx, "stmt.var.wg_shared.reuse")
                } else {
                    off = smem_carve(ctx, need)
                    ctx.slots |> push(SmemSlot(off = off, bytes = need, owner = sname0))
                    census_kind(ctx, "stmt.var.wg_shared")
                }
                lines |> push("{indent}{wgi.ctype}& {ctx.renames[sname0]} = *({wgi.ctype}*)(__das_smem + {int(off)});")
                continue
            }
            let tname = type_spell(ctx, v._type, v.at)
            if (empty(tname)) {
                if (v._type == null || v._type.baseType != Type.tStructure) {
                    shader_error(ctx, v.at, "local `{v.name}` type {describe(v._type)} has no CUDA form")
                }
                continue
            }
            if (cuda_is_half_family(v._type)) {
                need_half(ctx, v._type)
            }
            let vname = string(v.name)
            ctx.renames[vname] = local_name(ctx, vname)
            if (v.init == null) {
                census_kind(ctx, "stmt.var.zero")
                lines |> push("{indent}{tname} {ctx.renames[vname]} = \{\};")
            } else {
                let init = emit_value(ctx, v.init)
                census_kind(ctx, v._type.flags.constant ? "stmt.let" : "stmt.var")
                lines |> push("{indent}{tname} {ctx.renames[vname]} = {init};")
            }
        }
    } elif (e is ExprCopy) {
        let ec = e as ExprCopy
        let l = emit_store_target(ctx, ec.left)
        let r = emit_value(ctx, ec.right)
        census_kind(ctx, "stmt.assign")
        lines |> push("{indent}{l} = {r};")
    } elif (e is ExprOp2) {
        let op2 = e as ExprOp2
        let op = string(op2.op)
        let base = compound_base(op)
        if (op2._type == null || op2._type.baseType != Type.tVoid || empty(base)) {
            shader_error(ctx, op2.at, "operator `{op}` as a statement is not an assignment")
            return
        }
        let l = emit_value(ctx, op2.left)
        let r = emit_value(ctx, op2.right)
        let cls = census_class(op2.left._type)
        census_kind(ctx, "op.{op2_census_name(base)}_assign.{cls}")
        if (is_vec_class(cls)) {
            need_vec_ops(ctx, op2.left._type)
            if (base == "%" && cls == "f32v") {
                lines |> push("{indent}{l} = {need_lanewise(ctx, "fmodf", op2.left._type, 2)}({l}, {r});")
                return
            }
        } elif (cls == "f16") {
            need_half(ctx, op2.left._type)
        } elif (base == "%" && cls == "f32") {
            lines |> push("{indent}{l} = fmodf({l}, {r});")
            return
        } elif (ctx.precise && cls == "f32" && !empty(precise_f32_fn(base))) {
            census_kind(ctx, "op.{op2_census_name(base)}_assign.f32.rn")
            lines |> push("{indent}{l} = {precise_f32_fn(base)}({l}, {r});")
            return
        }
        lines |> push("{indent}{l} {op} {r};")
    } elif (e is ExprOp1) {
        let op1 = e as ExprOp1
        let op = string(op1.op)
        let sub = emit_value(ctx, op1.subexpr)
        if (op == "++") {
            census_kind(ctx, "op.preinc")
            lines |> push("{indent}++{sub};")
        } elif (op == "--") {
            census_kind(ctx, "op.predec")
            lines |> push("{indent}--{sub};")
        } elif (op == "+++") {
            census_kind(ctx, "op.postinc")
            lines |> push("{indent}{sub}++;")
        } elif (op == "---") {
            census_kind(ctx, "op.postdec")
            lines |> push("{indent}{sub}--;")
        } else {
            shader_error(ctx, op1.at, "unary `{op}` is not a statement")
        }
    } elif (e is ExprIfThenElse) {
        emit_if(ctx, e as ExprIfThenElse, indent, lines, "")
    } elif (e is ExprWhile) {
        let wh = e as ExprWhile
        let c = emit_value(ctx, wh.cond)
        emit_loop_hints(ctx, wh.annotations, wh.at, indent, lines)
        census_kind(ctx, "stmt.while")
        lines |> push("{indent}while ({c}) \{")
        emit_stmt(ctx, wh.body, indent + "    ", lines)
        lines |> push("{indent}\}")
    } elif (e is ExprFor) {
        emit_for(ctx, e as ExprFor, indent, lines)
    } elif (e is ExprInvoke) {
        census_kind(ctx, "stmt.call_method")
        lines |> push("{indent}{emit_invoke_value(ctx, e as ExprInvoke)};")
    } elif (e is ExprCall) {
        let ec = e as ExprCall
        let cname = call_base_name("{ec.name}")
        if (cname == "barrier" || cname == "memoryBarrierShared") {
            if (!empty(ec.arguments)) {
                shader_error(ctx, ec.at, "{cname}() takes no arguments")
                return
            }
            census_kind(ctx, cname == "barrier" ? "call.barrier" : "call.memory_barrier_shared")
            lines |> push("{indent}__syncthreads();")
        } elif (cname == "memoryBarrierBuffer") {
            census_kind(ctx, "call.memory_barrier_buffer")
            lines |> push("{indent}__threadfence();")
        } elif (cname == "coopmatLoad" || cname == "coopmatStore" || cname == "coopmatConvert") {
            let text = emit_coop_stmt(ctx, ec)
            if (!empty(text)) {
                lines |> push("{indent}{text}")
            }
        } elif (key_exists(CM2_STATEMENTS, cname)) {
            let text = emit_cm2_stmt(ctx, ec, cname)
            if (!empty(text)) {
                lines |> push("{indent}{text}")
            }
        } elif ((cname |> starts_with("atomic")) || is_user_fn(ec.func)) {
            census_kind(ctx, is_user_fn(ec.func) ? "stmt.call_user" : "stmt.call_atomic")
            lines |> push("{indent}{emit_call_value(ctx, ec)};")
        } elif (key_exists(INTRINSIC_STATEMENTS, cname)) {
            census_kind(ctx, "stmt.call_intrinsic")
            lines |> push("{indent}{emit_call_value(ctx, ec)};")
        } else {
            shader_error(ctx, ec.at, "call `{cname}` as a statement has no CUDA lowering - barrier()/memoryBarrier*() sync, the atomics and user functions are the statement calls")
        }
    } elif (e is ExprBreak) {
        census_kind(ctx, "stmt.break")
        lines |> push("{indent}break;")
    } elif (e is ExprContinue) {
        census_kind(ctx, "stmt.continue")
        lines |> push("{indent}continue;")
    } elif (e is ExprReturn) {
        let er = e as ExprReturn
        if (er.subexpr != null) {
            if (ctx.fn_depth == 0) {
                shader_error(ctx, er.at, "kernels return void - `return <value>` has no CUDA form")
                return
            }
            census_kind(ctx, "stmt.return_value")
            lines |> push("{indent}return {emit_value(ctx, er.subexpr)};")
            return
        }
        census_kind(ctx, "stmt.return")
        lines |> push("{indent}return;")
    } else {
        shader_error(ctx, e.at, "statement {e.__rtti} is not in the supported CUDA subset")
    }
}

def private emit_if(var ctx : CudaCtx; ite : ExprIfThenElse?; indent : string; var lines : array<string>; opener : string) {
    let c = emit_value(ctx, ite.cond)
    census_kind(ctx, empty(opener) ? "stmt.if" : "stmt.elif")
    lines |> push("{indent}{opener}if ({c}) \{")
    emit_stmt(ctx, ite.if_true, indent + "    ", lines)
    if (ite.if_false == null) {
        lines |> push("{indent}\}")
    } elif (ite.if_false is ExprIfThenElse) {
        emit_if(ctx, ite.if_false as ExprIfThenElse, indent, lines, "\} else ")
    } else {
        census_kind(ctx, "stmt.else")
        lines |> push("{indent}\} else \{")
        emit_stmt(ctx, ite.if_false, indent + "    ", lines)
        lines |> push("{indent}\}")
    }
}

//! the per-loop hints the JIT and the MSL emitter read, each as its `#pragma unroll` form
def private emit_loop_hints(var ctx : CudaCtx; anns : AnnotationArgumentList; at : LineInfo; indent : string; var lines : array<string>) {
    for (a in anns) {
        if (a.name == "unroll" || a.name == "unroll_full") {
            census_kind(ctx, "stmt.for.unroll")
            lines |> push("{indent}#pragma unroll")
        } elif (a.name == "unroll_disable" || a.name == "dont_unroll") {
            census_kind(ctx, "stmt.for.unroll")
            lines |> push("{indent}#pragma unroll 1")
        } elif (a.name == "unroll_count" && a.basicType == Type.tInt) {
            census_kind(ctx, "stmt.for.unroll")
            lines |> push("{indent}#pragma unroll {a.iValue}")
        } else {
            shader_error(ctx, at, "loop hint `{a.name}` has no CUDA lowering - supported: unroll, unroll_full, unroll_disable, dont_unroll, unroll_count=N")
        }
    }
}

def private emit_for(var ctx : CudaCtx; fr : ExprFor?; indent : string; var lines : array<string>) {
    if (length(fr.iteratorVariables) != 1 || length(fr.sources) != 1) {
        shader_error(ctx, fr.at, "only a single `for (i in range(...))` loop lowers to CUDA")
        return
    }
    let iv = fr.iteratorVariables[0]
    if (iv._type == null || (iv._type.baseType != Type.tInt && iv._type.baseType != Type.tUInt)) {
        shader_error(ctx, iv.at, "for-loop induction variable `{iv.name}` must be int or uint (range/urange source)")
        return
    }
    let isU = iv._type.baseType == Type.tUInt
    let tname = isU ? "uint" : "int"
    var lo = ""
    var hi = ""
    var hiIsConst = false
    if (fr.sources[0] is ExprConstRange) {
        let fromto = unsafe(reinterpret<int2>((fr.sources[0] as ExprConstRange).value))
        lo = "{fromto.x}"
        hi = "{fromto.y}"
        hiIsConst = true
    } elif (fr.sources[0] is ExprConstURange) {
        let fromto = unsafe(reinterpret<uint2>((fr.sources[0] as ExprConstURange).value))
        lo = "{fromto.x}u"
        hi = "{fromto.y}u"
        hiIsConst = true
    } else {
        let src = fr.sources[0] ?as ExprCall
        if (src == null || (src.name != "range" && src.name != "urange")) {
            shader_error(ctx, fr.at, "for-loop source must be range(...) or urange(...)")
            return
        }
        let na = length(src.arguments)
        if (na != 1 && na != 2) {
            shader_error(ctx, fr.at, "range with a step is not supported")
            return
        }
        lo = na == 2 ? emit_value(ctx, src.arguments[0]) : (isU ? "0u" : "0")
        hi = emit_value(ctx, src.arguments[na - 1])
        hiIsConst = src.arguments[na - 1] is ExprConstInt || src.arguments[na - 1] is ExprConstUInt
    }
    let ivName = string(iv.name)
    ctx.renames[ivName] = local_name(ctx, ivName)
    let mn = ctx.renames[ivName]
    emit_loop_hints(ctx, fr.annotations, fr.at, indent, lines)
    census_kind(ctx, "stmt.for")
    if (hiIsConst) {
        lines |> push("{indent}for ({tname} {mn} = {lo}; {mn} < {hi}; ++{mn}) \{")
    } else {
        let en = local_name(ctx, "{mn}_end")
        lines |> push("{indent}for ({tname} {mn} = {lo}, {en} = {hi}; {mn} < {en}; ++{mn}) \{")
    }
    emit_stmt(ctx, fr.body, indent + "    ", lines)
    lines |> push("{indent}\}")
}

//! the CUDA parameter for one das argument; "" = error already reported
def private ufn_param(var ctx : CudaCtx; v : VariablePtr; at : LineInfo) : string {
    let dname = string(v.name)
    let name = safe_name(dname)
    ctx.renames[dname] = name
    let t = v._type
    if (t == null) {
        shader_error(ctx, at, "parameter `{dname}` has no type")
        return ""
    }
    if (t.baseType == Type.tArray || t.baseType == Type.tFixedArray || t.baseType == Type.tPointer) {
        let pt = t.firstType
        if (pt == null) {
            shader_error(ctx, at, "parameter `{dname}` has no element type")
            return ""
        }
        if (reject_cpu_only_width(ctx, at, pt)) {
            return ""
        }
        let elem = type_spell(ctx, pt, at)
        if (empty(elem)) {
            if (pt.baseType != Type.tStructure) {
                shader_error(ctx, at, "parameter `{dname}` element type has no CUDA form")
            }
            return ""
        }
        if (cuda_is_half_family(pt)) {
            need_half(ctx, pt)
        }
        let isc = t.baseType == Type.tPointer ? pt.flags.constant : t.flags.constant
        let kind = t.baseType == Type.tArray ? "buffer" : (t.baseType == Type.tFixedArray ? "shared" : "ptr")
        census_kind(ctx, isc ? "ufn.param.{kind}_const" : "ufn.param.{kind}")
        return isc ? "const {elem}* {name}" : "{elem}* {name}"
    }
    if (reject_cpu_only_width(ctx, at, t)) {
        return ""
    }
    let tn = type_spell(ctx, t, at)
    if (empty(tn)) {
        if (t.baseType != Type.tStructure) {
            shader_error(ctx, at, "parameter `{dname}` type has no CUDA form (scalars, their 2/3/4 vectors, structs, array<T>, T[N], T?)")
        }
        return ""
    }
    if (cuda_is_half_family(t)) {
        need_half(ctx, t)
    }
    if (t.flags.ref && !t.flags.constant) {
        census_kind(ctx, "ufn.param.ref")
        return "{tn}& {name}"
    }
    if (t.baseType == Type.tStructure) {   // a struct parameter is das's const reference: a block decode reads its element in place, no stack copy
        census_kind(ctx, "ufn.param.struct_cref")
        return "const {tn}& {name}"
    }
    census_kind(ctx, "ufn.param.value")
    return "{tn} {name}"
}

//! the hidden trailing parameters: the builtins by value, then the members - a buffer as its pointer, the args struct by const reference, a shared array as its pointer
def private hidden_params(var ctx : CudaCtx; idx : int) : array<string> {
    var params : array<string>
    var hidden <- ufn_hidden(ctx, idx)
    params |> reserve(length(hidden.builtins) + length(hidden.members))
    for (b in hidden.builtins) {
        census_kind(ctx, "ufn.param.hidden_builtin")
        params |> push("{builtin_info(b).ctype} {b}")
    }
    for (mname in hidden.members) {
        let m = ctx.members[mname]
        if (m.kargs) {
            census_kind(ctx, "ufn.param.hidden_kargs")
            params |> push("const {m.elem}& {m.name}")
        } elif (m.workgroup && !m.is_array) {
            census_kind(ctx, "ufn.param.hidden_shared_scalar")
            params |> push("{m.elem}& {m.name}")
        } elif (m.workgroup) {
            census_kind(ctx, "ufn.param.hidden_shared")
            params |> push("{m.elem}* {m.name}")
        } else {
            census_kind(ctx, m.written ? "ufn.param.hidden_buffer" : (m.group_written ? "ufn.param.hidden_buffer_const_aliased" : "ufn.param.hidden_buffer_const"))
            params |> push(buffer_ptr_spelling(m))
        }
    }
    return <- params
}

def private emit_one_ufn(var ctx : CudaCtx; idx : int) {
    var save_renames <- ctx.renames
    var save_ptrs <- ctx.ptrs
    let save_self = ctx.self_name
    let fn = ctx.user_fns[idx].fn
    let is_method = ctx.user_fns[idx].is_method
    ctx.self_name = is_method && !empty(fn.arguments) ? string(fn.arguments[0].name) : ""
    var params : array<string>
    params |> reserve(length(fn.arguments) + 4)
    for (i in range(is_method ? 1 : 0, length(fn.arguments))) {
        let p = ufn_param(ctx, fn.arguments[i], fn.at)
        if (!empty(p)) {
            params |> push(p)
        }
    }
    var hp <- hidden_params(ctx, idx)
    params |> push_from(hp)
    var ret = "void"
    if (fn.result != null && fn.result.baseType != Type.tVoid) {
        if (!reject_cpu_only_width(ctx, fn.at, fn.result)) {
            ret = shader_type_name(fn.result)
            if (empty(ret)) {
                shader_error(ctx, fn.at, "return type of `{ctx.user_fns[idx].name}` has no CUDA form")
                ret = "void"
            } elif (cuda_is_half_family(fn.result)) {
                need_half(ctx, fn.result)
            }
        }
    }
    var body_lines : array<string>
    ctx.fn_depth++
    emit_stmt(ctx, fn.body, "    ", body_lines)
    ctx.fn_depth--
    census_kind(ctx, is_method ? "decl.method" : "decl.user_fn")
    let sig = "__device__ __forceinline__ {ret} {ctx.user_fns[idx].name}({join(params, ", ")})"
    ctx.user_fns[idx].proto = "{sig};"
    var hoisted <- ctx.hoisted   // a pipelined tile declared in this function: its ring state at the function's top
    ctx.user_fns[idx].text = build_string() $(var w) {
        w |> write(sig)
        w |> write(" \{\n")
        if (!empty(hoisted)) {
            w |> write(join(hoisted, "\n"))
            w |> write("\n")
        }
        w |> write(join(body_lines, "\n"))
        w |> write("\n\}")
    }
    delete hoisted
    delete params
    delete body_lines
    ctx.self_name = save_self
    ctx.renames <- save_renames
    ctx.ptrs <- save_ptrs
}

//! prototypes first, then bodies, in name order - call order cannot bite
def private emit_user_fns(var ctx : CudaCtx) : string {
    if (empty(ctx.user_fns)) {
        return ""
    }
    var order <- [for (i in range(length(ctx.user_fns))); i]
    sort(order) <| cmp_by(ctx.user_fns[_].name)
    for (i in order) {
        emit_one_ufn(ctx, i)
    }
    return build_string() $(var w) {
        for (i in order) {
            w |> write(ctx.user_fns[i].proto)
            w |> write("\n")
        }
        for (i in order) {
            w |> write(ctx.user_fns[i].text)
            w |> write("\n")
        }
    }
}

[macro_function]
def public cuda_kernel_entry_name(fn : FunctionPtr; cfg : CudaEmitConfig) : string =>
    shader_entry_name(fn, cfg.kernel_name, CUDA_RESERVED)

//! every kernel's entry on sm_90 and up: `griddepcontrol.wait` then `griddepcontrol.launch_dependents`
let private PDL_WAIT = "#if __CUDA_ARCH__ >= 900\n    asm volatile(\"griddepcontrol.wait;\" ::: \"memory\");\n    asm volatile(\"griddepcontrol.launch_dependents;\");\n#endif\n"

let private PRELUDE_HEAD = "typedef unsigned int uint;\ntypedef unsigned short ushort;\ntypedef unsigned char uchar;"

//! Lower a [cuda_kernel] class method to CUDA C++ text. On any error the return is "" and `errors`
//! explains; `census` collects the construct kinds emitted; `smem` is the end of the dynamic shared carve in
//! bytes - the launch's dynamic shared size.
[macro_function, arch(at="../ARCHITECTURE.md#cuda-programmatic-dependent-launch")]
def public generate_cuda(fn : FunctionPtr; var errors : array<string>; cfg : CudaEmitConfig; var census : table<string>; var smem : uint64&) : string {
    var ctx = CudaCtx(warps = int((cfg.block_size.x * cfg.block_size.y * cfg.block_size.z + 31u) / 32u), precise = cfg.precise)
    if (fn == null) {
        errors |> push("generate_cuda: null function")
        return ""
    }
    if (fn.classParent == null) {
        errors |> push("[cuda_kernel] `{fn.name}` is not a class method - kernels are methods of a class whose @ssbo members declare the buffers")
        return ""
    }
    if ((fn.arguments |> length) != 1) {
        errors |> push("[cuda_kernel] `{fn.name}` must take no parameters beyond self - kernels read/write only @ssbo/@kargs members")
        return ""
    }
    ctx.self_name = string(fn.arguments[0].name)
    ctx.kcls = unsafe(reinterpret<StructurePtr>(fn.classParent))
    scan_members(ctx, fn)
    scan_body(ctx, fn.body)
    // the tiles' lives settle once the whole kernel is scanned (ARCHITECTURE_EMIT.md, tile slot reuse and the ones tile's row sum)
    for (name, tl in keys(ctx.tiles), values(ctx.tiles)) {
        for (lp in ctx.loops) {
            if (before(tl.decl, lp.start) && !before(tl.last, lp.start) && !before(lp.end, tl.last)) {
                tl.last = lp.end
            }
        }
        if (tl.ones && !tl.other_use && !tl.convert_target && !tl.ambiguous && tl.ones_feeds_row_owned && ctx.row_own) {
            ctx.ones_sum |> insert(name)
        }
        // a global view unless a row-owning warp reads the whole tile (the flash tile's K), where staging it once for every warp pays
        if (tl.global_view_written && !tl.global_view_blocked && !tl.ambiguous && !(ctx.row_own && tl.read_whole_by_row_owner)) {
            ctx.global_view_tiles |> insert(name)
        }
    }
    ctx.kperm = !empty(ctx.global_view_tiles)
    if (!empty(ctx.errors)) {
        errors |> push_from(ctx.errors)
        return ""
    }
    for (fld in fn.classParent.fields) {
        if (fld._type != null) {
            let et = fld._type.baseType == Type.tArray || fld._type.baseType == Type.tFixedArray ? fld._type.firstType : fld._type
            if (et != null && cuda_is_half_family(et) && key_exists(ctx.members, string(fld.name))) {
                need_half(ctx, et)
            }
        }
    }

    let entry = cuda_kernel_entry_name(fn, cfg)
    let bufs_name = "{entry}_bufs"

    var wg_names <- [for (name, m in keys(ctx.members), values(ctx.members)); name; where m.workgroup]
    wg_names |> sort()
    var shared_lines : array<string>
    shared_lines |> reserve(length(wg_names))
    for (name in wg_names) {
        let m = ctx.members[name]
        census_kind(ctx, m.is_array ? "decl.shared_array" : "decl.shared")
        let decl = (m.is_array ? smem_decl(ctx, m.elem, m.name, m.wg_bytes / uint64(m.wg_len), m.wg_len)
            : smem_decl(ctx, m.elem, m.name, m.wg_bytes))
        shared_lines |> push("    {decl}")
    }

    settle_alias_groups(ctx)
    let user_fn_text = emit_user_fns(ctx)

    var body_lines : array<string>
    emit_stmt(ctx, fn.body, "    ", body_lines)

    var ordered <- [for (name, m in keys(ctx.members), values(ctx.members)); name; where !m.workgroup && !m.kargs]
    sort(ordered) <| cmp_by(ctx.members?[_] ?. binding ?? 0, _)
    var struct_lines, unpack_lines : array<string>
    struct_lines |> reserve(length(ordered))
    unpack_lines |> reserve(length(ordered))
    for (name in ordered) {
        let m = ctx.members[name]
        let spelling = buffer_ptr_spelling(m)
        if (empty(m.alias_of)) {
            struct_lines |> push("    {m.group_written ? "" : "const "}{m.elem}* {m.name};")
            census_kind(ctx, m.written ? "param.buffer" : (m.group_written ? "param.buffer_const_aliased" : "param.buffer_const"))
            unpack_lines |> push("    {spelling} = __das_b.{m.name};")
        } else {
            census_kind(ctx, m.written ? "param.buffer_alias" : (m.group_written ? "param.buffer_alias_const_aliased" : "param.buffer_alias_const"))
            unpack_lines |> push("    {spelling} = ({m.written ? "" : "const "}{m.elem}*)__das_b.{ctx.members[m.alias_of].name};")
        }
    }
    var kargs_param = ""
    for (name, m in keys(ctx.members), values(ctx.members)) {
        if (m.kargs) {
            if (m.written) {
                shader_error(ctx, fn.at, "@kargs member `{name}` is written - the args struct is read-only; outputs are @ssbo buffers")
                continue
            }
            census_kind(ctx, "param.kargs")
            kargs_param = ", const {m.elem} {m.name}"
        }
    }
    var builtin_lines : array<string>
    for (name in BUILTIN_ORDER) {
        if (key_exists(ctx.builtins_used, name)) {
            let bi = builtin_info(name)
            census_kind(ctx, "param.builtin.{name}")
            builtin_lines |> push("    const {bi.ctype} {name} = {bi.init};")
        }
    }

    if (!empty(ctx.errors)) {
        errors |> push_from(ctx.errors)
        return ""
    }
    smem = ctx.smem_off

    census_kind(ctx, "preamble")
    census_kind(ctx, "kernel_sig")
    census_kind(ctx, "stmt.pdl_wait")
    var helper_text = ""
    if (!empty(ctx.helpers)) {
        var hk <- [for (k in keys(ctx.helpers)); k]
        hk |> sort()
        helper_text = build_string() $(var w) {
            for (k in hk) {
                w |> write(ctx.helpers[k])
                w |> write("\n")
            }
        }
    }
    if (!empty(ctx.const_tables)) {
        var cts <- [for (v in values(ctx.const_tables)); v]
        cts |> sort()
        helper_text += join(cts, "\n") + "\n"
    }
    var kstruct_text = ""
    if (!empty(ctx.elem_struct_order)) {
        census_kind(ctx, "decl.elem_struct")
        kstruct_text = join([for (s in ctx.elem_struct_order); ctx.elem_structs[s].decl], "\n") + "\n"
    }
    if (!empty(ctx.arg_structs)) {
        var ks <- [for (v in values(ctx.arg_structs)); v]
        ks |> sort()
        census_kind(ctx, "decl.kargs_struct")
        kstruct_text += join(ks, "\n") + "\n"
    }
    let min_blocks = cfg.min_blocks > 0 ? ", {cfg.min_blocks}" : ""
    let bounds = (cfg.block_size.x > 0u && cfg.block_size.y > 0u && cfg.block_size.z > 0u) ? " __launch_bounds__({int64(uint64(cfg.block_size.x * cfg.block_size.y * cfg.block_size.z))}{min_blocks})" : ""
    if (!empty(bounds)) {
        census_kind(ctx, "kernel_sig.launch_bounds{cfg.min_blocks > 0 ? ".min_blocks" : ""}")
    }
    let chunks = [
        "{PRELUDE_HEAD}\n{helper_text}{kstruct_text}struct {bufs_name} \{\n{join(struct_lines, "\n")}\n\};\n{user_fn_text}",
        "extern \"C\" __global__ void{bounds} {entry}(const {bufs_name} __das_b{kargs_param}) \{\n",
        empty(unpack_lines) ? "" : join(unpack_lines, "\n") + "\n",
        PDL_WAIT,
        empty(shared_lines) ? "" : join(shared_lines, "\n") + "\n",
        empty(builtin_lines) ? "" : join(builtin_lines, "\n") + "\n",
        empty(ctx.hoisted) ? "" : join(ctx.hoisted, "\n") + "\n",
        join(body_lines, "\n"),
        empty(ctx.raw_rings) ? "" : "\n    __das_cp_async_wait_0();",   // a ring's last speculative copies land before the block leaves
        "\n\}\n"
    ]
    for (kind in keys(ctx.census)) {
        census |> insert(kind)
    }
    return join(chunks, "")
}
