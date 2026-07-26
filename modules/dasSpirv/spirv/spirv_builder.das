options gen2

// Low-level SPIR-V module builder: result-id allocator, the ten logical sections (emitted
// in SPIR-V's mandated order at finalize), arity-overloaded instruction emit, string-literal
// word packing, and type/constant dedup pools. AST-agnostic — the emitter (spirv_emit.das)
// and tests drive it; it knows nothing about daslang AST.

module spirv_builder shared public

require spirv_grammar
require strings

// ===== Logical sections (index order == SPIR-V module layout order) =====
let public SEC_CAPS       = 0   // OpCapability
let public SEC_EXT        = 1   // OpExtension
let public SEC_EXTIMPORT  = 2   // OpExtInstImport
let public SEC_MEMMODEL   = 3   // OpMemoryModel (single)
let public SEC_ENTRY      = 4   // OpEntryPoint
let public SEC_EXECMODE   = 5   // OpExecutionMode
let public SEC_DEBUG      = 6   // OpSource / OpName / OpMemberName
let public SEC_DECOR      = 7   // OpDecorate / OpMemberDecorate
let public SEC_TYPES      = 8   // OpType* / OpConstant* / global OpVariable (interleaved)
let public SEC_FUNCS      = 9   // function bodies
let private SECTION_COUNT = 10

struct public SpirvModule {
    next_id    : uint                    // result-id allocator; ids are 1-based, 0 is invalid
    sections   : array<array<uint>>      // one word-buffer per SEC_*
    type_pool  : table<string; uint>     // dedup key -> type result-id (Phase 1+)
    const_pool : table<string; uint>     // dedup key -> constant result-id (Phase 1+)
    version    : uint                    // SPIR-V version word for the header (default 1.3; bumped to 1.4 for mesh/task)
}

def public make_module() : SpirvModule {
    var m = SpirvModule(next_id = 1u, version = SPV_VERSION_1_3)
    resize(m.sections, SECTION_COUNT)
    return <- m
}

def finalize(var m : SpirvModule) {
    delete m.sections       // array<array<uint>>: outer delete finalizes the inner buffers too
    delete m.type_pool
    delete m.const_pool
}

// ===== id allocation =====
def public alloc_id(var m : SpirvModule) : uint {
    let id = m.next_id
    m.next_id++
    return id
}

// ===== raw word + arity-overloaded instruction emit =====
// word0 of every instruction is (wordCount << 16) | opcode. The opcode is the typed SpvOp;
// operands (result-type-id, result-id, ids, literals, and uint()-cast operand enums) follow
// in the exact order the grammar dictates.
def private pw(var m : SpirvModule; sec : int; w : uint) {
    m.sections[sec] |> push(w)
}

def public emit(var m : SpirvModule; sec : int; op : SpvOp) {
    pw(m, sec, (1u << 16u) | uint(op))
}

def public emit(var m : SpirvModule; sec : int; op : SpvOp; a : uint) {
    pw(m, sec, (2u << 16u) | uint(op))
    pw(m, sec, a)
}

def public emit(var m : SpirvModule; sec : int; op : SpvOp; a, b : uint) {
    pw(m, sec, (3u << 16u) | uint(op))
    pw(m, sec, a)
    pw(m, sec, b)
}

def public emit(var m : SpirvModule; sec : int; op : SpvOp; a, b, c : uint) {
    pw(m, sec, (4u << 16u) | uint(op))
    pw(m, sec, a)
    pw(m, sec, b)
    pw(m, sec, c)
}

def public emit(var m : SpirvModule; sec : int; op : SpvOp; a, b, c, d : uint) {
    pw(m, sec, (5u << 16u) | uint(op))
    pw(m, sec, a)
    pw(m, sec, b)
    pw(m, sec, c)
    pw(m, sec, d)
}

// Variable-length operand list (OpEntryPoint interface, OpTypeStruct members,
// OpAccessChain indices, OpConstantComposite, …).
def public emit_n(var m : SpirvModule; sec : int; op : SpvOp; operands : array<uint>) {
    pw(m, sec, (uint(1 + length(operands)) << 16u) | uint(op))
    for (o in operands) {
        pw(m, sec, o)
    }
}

// ===== SPIR-V literal string -> words (LSB-first bytes, NUL-terminated, zero-padded) =====
def public string_words(s : string) : array<uint> {
    var words : array<uint>
    var cur = 0u
    var shift = 0u
    peek_data(s) $(bytes) {
        for (b in bytes) {
            cur = cur | (uint(b) << shift)
            shift += 8u
            if (shift == 32u) {
                words |> push(cur)
                cur = 0u
                shift = 0u
            }
        }
    }
    // Always push the trailing word: it carries the remaining bytes plus the implicit NUL
    // (cur is zero above the written bytes). When the byte count is a multiple of 4 this is
    // an all-zero word — the required separate NUL-terminator word.
    words |> push(cur)
    return <- words
}

// ===== cached, deduplicated type + constant emitters =====
// SPIR-V requires types and constants to be unique. Each helper returns the existing id if the
// canonical key is already in the pool, else allocates, emits into SEC_TYPES, and caches. ids
// are 1-based so 0 is a safe "absent" sentinel. Decorations (ArrayStride/Offset/Block) go to
// SEC_DECOR alongside the type they qualify.

def public type_void(var m : SpirvModule) : uint {
    let ex = m.type_pool?["void"] ?? 0u
    return ex if (ex != 0u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.TypeVoid, id)
    m.type_pool |> insert("void", id)
    return id
}

def public type_bool(var m : SpirvModule) : uint {
    let ex = m.type_pool?["bool"] ?? 0u
    return ex if (ex != 0u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.TypeBool, id)
    m.type_pool |> insert("bool", id)
    return id
}

// Sub-32-bit numeric types require their OpCapability (Float16/Int16/Int8 — all core in
// SPIR-V 1.3, no OpExtension; spirv-val verified). Emitting it HERE, in the type factory,
// makes the cap impossible to forget: any path that creates the type pulls it. Deduped
// through the type pool alongside the type itself.
def private ensure_width_cap(var m : SpirvModule; is_float : bool; width : uint) {
    if (width == 32u) {
        return
    }
    let key = is_float ? "cap_f{width}" : "cap_i{width}"
    return if ((m.type_pool?[key] ?? 0u) != 0u)
    if (is_float) {
        emit(m, SEC_CAPS, SpvOp.Capability, uint(width == 64u ? SpvCapability.Float64 : SpvCapability.Float16))
    } elif (width == 64u) {
        emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.Int64))
    } elif (width == 16u) {
        emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.Int16))
    } elif (width == 8u) {
        emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.Int8))
    }
    m.type_pool |> insert(key, 1u)
}

// 8/16-bit SSBO members additionally require the STORAGE capability (the width cap above only
// licenses the type/arithmetic). StorageBuffer16BitAccess is core in SPIR-V 1.3 (cap only);
// StorageBuffer8BitAccess needs OpExtension "SPV_KHR_8bit_storage" at our 1.3 header (core at
// 1.5). Deduped through the type pool like ensure_width_cap so the layout code can pull them
// without an EmitCtx.
def public ensure_storage_buffer_16(var m : SpirvModule) {
    return if ((m.type_pool?["cap_sb16"] ?? 0u) != 0u)
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.StorageBuffer16BitAccess))
    m.type_pool |> insert("cap_sb16", 1u)
}

def public ensure_storage_buffer_8(var m : SpirvModule) {
    return if ((m.type_pool?["cap_sb8"] ?? 0u) != 0u)
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.StorageBuffer8BitAccess))
    var nm <- string_words("SPV_KHR_8bit_storage")
    emit_n(m, SEC_EXT, SpvOp.Extension, nm)
    delete nm
    m.type_pool |> insert("cap_sb8", 1u)
}

// Cooperative matrices (Tensor-Core MMA) need OpCapability CooperativeMatrixKHR + OpExtension
// "SPV_KHR_cooperative_matrix", and — because coopmat ops carry implicit memory scope — the
// VulkanMemoryModel capability (spirv-val rejects Shader + CooperativeMatrixKHR without it), which also
// forces OpMemoryModel to Vulkan (generate_spirv picks that when this cap is present). The coopmat
// opcodes are core only at SPIR-V 1.6, so the module version floors at 1.6 (glslang's KHR_coopmat blobs
// are 1.6, where VulkanMemoryModel is core too — no OpExtension for it). Deduped through the type pool
// like ensure_storage_buffer_8 so type_cooperative_matrix can pull it without an EmitCtx.
def public ensure_cooperative_matrix(var m : SpirvModule) {
    if (m.version < SPV_VERSION_1_6) {
        m.version = SPV_VERSION_1_6
    }
    return if ((m.type_pool?["cap_coopmat"] ?? 0u) != 0u)
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.VulkanMemoryModel))
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixKHR))
    var nm <- string_words("SPV_KHR_cooperative_matrix")
    emit_n(m, SEC_EXT, SpvOp.Extension, nm)
    delete nm
    m.type_pool |> insert("cap_coopmat", 1u)
}

// cm2 (SPV_NV_cooperative_matrix2 + SPV_NV_tensor_addressing): tensor-addressed cooperative-matrix
// loads/stores — workgroup-scope tiles, tensor layout/view objects, block loads with DECODE functions.
// Pulls the KHR coopmat base (VulkanMemoryModel + 1.6 floor) plus the NV capability set glslang
// declares for a cm2 shader, including PhysicalStorageBufferAddresses: decode functions take
// PhysicalStorageBuffer pointers, and generate_spirv switches the ADDRESSING model to
// PhysicalStorageBuffer64 when this pool key is present (the "cap_coopmat" pattern, one level up).
def public ensure_cooperative_matrix2(var m : SpirvModule) {
    ensure_cooperative_matrix(m)
    return if ((m.type_pool?["cap_cm2"] ?? 0u) != 0u)
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.PhysicalStorageBufferAddresses))
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixTensorAddressingNV))
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.CooperativeMatrixBlockLoadsNV))
    emit(m, SEC_CAPS, SpvOp.Capability, uint(SpvCapability.TensorAddressingNV))
    var nm <- string_words("SPV_NV_cooperative_matrix2")
    emit_n(m, SEC_EXT, SpvOp.Extension, nm)
    delete nm
    var nm2 <- string_words("SPV_NV_tensor_addressing")
    emit_n(m, SEC_EXT, SpvOp.Extension, nm2)
    delete nm2
    m.type_pool |> insert("cap_cm2", 1u)
}

// OpTypeTensorLayoutNV — the cm2 address-mapping object type: `dim` dimensions, `clamp` out-of-bounds
// mode (SpvTensorClampMode: 0 = Undefined for the aligned fast path, 1 = Constant zero-fill for
// guarded edges). Both operands are <id>s of uint constants, like the coopmat type's. Deduped.
def public type_tensor_layout(var m : SpirvModule; dim : uint; clamp : uint) : uint {
    ensure_cooperative_matrix2(m)
    let key = "tlayout{dim}_{clamp}"
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let dim_id = const_uint(m, dim)
    let clamp_id = const_uint(m, clamp)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.TypeTensorLayoutNV, id, dim_id, clamp_id)
    m.type_pool |> insert(key, id)
    return id
}

// OpTypeTensorViewNV — the cm2 element-permutation object type (`perm` = one uint per dimension;
// (1,0) is the transpose view a KxN B-tile loads from an NxK-major panel through). `has_dims` is a
// bool-constant <id> operand; the permutation entries are uint-constant <id>s. Deduped on the tuple.
def public type_tensor_view(var m : SpirvModule; dim : uint; has_dims : bool; perm : array<uint>) : uint {
    ensure_cooperative_matrix2(m)
    let key = build_string() $(var w) {
        w |> write("tview{dim}_{has_dims}")
        for (p in perm) {
            w |> write("_{p}")
        }
    }
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let dim_id = const_uint(m, dim)
    let hd_id = const_bool(m, has_dims)
    let id = alloc_id(m)
    var ops <- [id, dim_id, hd_id]
    ops |> reserve(3 + length(perm))
    for (p in perm) {
        ops |> push(const_uint(m, p))
    }
    emit_n(m, SEC_TYPES, SpvOp.TypeTensorViewNV, ops)
    delete ops
    m.type_pool |> insert(key, id)
    return id
}

def public type_int(var m : SpirvModule; width, signedness : uint) : uint {
    let key = "i{width}_{signedness}"
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    ensure_width_cap(m, false, width)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.TypeInt, id, width, signedness)
    m.type_pool |> insert(key, id)
    return id
}

def public type_uint(var m : SpirvModule) : uint {
    return type_int(m, 32u, 0u)
}

def public type_float(var m : SpirvModule; width : uint) : uint {
    let key = "f{width}"
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    ensure_width_cap(m, true, width)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.TypeFloat, id, width)
    m.type_pool |> insert(key, id)
    return id
}

def public type_vector(var m : SpirvModule; comp_id : uint; n : int) : uint {
    let key = "v{n}_{comp_id}"
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.TypeVector, id, comp_id, uint(n))
    m.type_pool |> insert(key, id)
    return id
}

// A matrix type: `cols` columns, each of type `col_type` (a vector type-id). daslang stores matrices
// column-major (m[i] is column i), matching SPIR-V's default — so float4x4 -> OpTypeMatrix(v4float, 4).
def public type_matrix(var m : SpirvModule; col_type : uint; cols : int) : uint {
    if (cols <= 0) {
        panic("type_matrix: column count must be positive, got {cols}")
    }
    let key = "mat{col_type}_c{cols}"
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.TypeMatrix, id, col_type, uint(cols))
    m.type_pool |> insert(key, id)
    return id
}

// OpTypeImage. `sampled_type` is the texel component scalar. `sampled` = 1 for a sampled texture
// (used with a sampler, format usually Unknown), 2 for a read-write storage image (a KNOWN format
// e.g. Rgba8, so no StorageImageRead/WriteWithoutFormat capability is needed). `depth` = 1 marks a
// depth-comparison image (sampler2DShadow, sampled via Dref), else 0; ms is always 0; the arrayed flag
// distinguishes a 2D/2DArray (etc.) image. Deduped on the full operand tuple (depth included).
def public type_image(var m : SpirvModule; sampled_type : uint; dim : SpvDim;
                      arrayed : uint = 0u; sampled : uint = 1u; format : SpvImageFormat = SpvImageFormat.Unknown;
                      depth : uint = 0u) : uint {
    let key = "img{sampled_type}_d{uint(dim)}_a{arrayed}_s{sampled}_f{uint(format)}_z{depth}"
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let id = alloc_id(m)
    // operands: Sampled Type, Dim, Depth, Arrayed, MS, Sampled, Image Format
    var ops <- [id, sampled_type, uint(dim), depth, arrayed, 0u, sampled, uint(format)]
    emit_n(m, SEC_TYPES, SpvOp.TypeImage, ops)
    delete ops
    m.type_pool |> insert(key, id)
    return id
}

// OpTypeSampler — a standalone sampler type (separate image+sampler model). Deduped (no operands).
def public type_sampler(var m : SpirvModule) : uint {
    let key = "sampler"
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.TypeSampler, id)
    m.type_pool |> insert(key, id)
    return id
}

// OpTypeAccelerationStructureKHR — the opaque top-level acceleration structure descriptor type
// (SPV_KHR_ray_tracing). Deduped (no operands).
def public type_acceleration_structure(var m : SpirvModule) : uint {
    let key = "accel"
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.TypeAccelerationStructureKHR, id)
    m.type_pool |> insert(key, id)
    return id
}

// OpTypeRayQueryKHR — the opaque ray-query object type (SPV_KHR_ray_query). A Function-storage
// LOCAL (not a descriptor), passed by pointer to the rayQuery*EXT intrinsics. Deduped (no operands).
def public type_ray_query(var m : SpirvModule) : uint {
    let key = "rayquery"
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.TypeRayQueryKHR, id)
    m.type_pool |> insert(key, id)
    return id
}

// OpTypeCooperativeMatrixKHR — a subgroup-scoped MxN (or MxK / KxN) tile of `comp_type`. The scope,
// rows, cols and use are all <id> operands (OpConstant uints), NOT inline literals, so they are
// materialized as constants first — define-before-use inside SEC_TYPES, exactly as type_array emits its
// length const. `use` is SpvCooperativeMatrixUse (A/B/Accumulator); scope is SpvScope (Subgroup here).
// Pulls the capability/extension + 1.6 floor via ensure_cooperative_matrix. Deduped on the full tuple.
def public type_cooperative_matrix(var m : SpirvModule; comp_type, scope, rows, cols, use : uint) : uint {
    ensure_cooperative_matrix(m)
    let key = "coopmat{comp_type}_s{scope}_r{rows}_c{cols}_u{use}"
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let scope_id = const_uint(m, scope)
    let rows_id = const_uint(m, rows)
    let cols_id = const_uint(m, cols)
    let use_id = const_uint(m, use)
    let id = alloc_id(m)
    var ops <- [id, comp_type, scope_id, rows_id, cols_id, use_id]
    emit_n(m, SEC_TYPES, SpvOp.TypeCooperativeMatrixKHR, ops)
    delete ops
    m.type_pool |> insert(key, id)
    return id
}

// OpTypeSampledImage wrapping an image type — the type of a combined image+sampler descriptor.
def public type_sampled_image(var m : SpirvModule; image_type : uint) : uint {
    let key = "simg{image_type}"
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.TypeSampledImage, id, image_type)
    m.type_pool |> insert(key, id)
    return id
}

def public type_pointer(var m : SpirvModule; sc : SpvStorageClass; pointee : uint) : uint {
    let key = "p{uint(sc)}_{pointee}"
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.TypePointer, id, uint(sc), pointee)
    m.type_pool |> insert(key, id)
    return id
}

// No-parameter OpTypeFunction returning `ret` (deduped). Named *_void because the only caller
// so far is the compute entry point (ret = void); it generalizes for free to any return type.
def public type_function_void(var m : SpirvModule; ret : uint) : uint {
    let key = "fn_{ret}"
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.TypeFunction, id, ret)
    m.type_pool |> insert(key, id)
    return id
}

// OpTypeFunction with `ret` result and `params` operand types (deduped). Generalizes
// type_function_void; used for user-defined shader function signatures (one param-type per
// argument: a value type for by-value params, an OpTypePointer Function for by-reference params).
def public type_function(var m : SpirvModule; ret : uint; params : array<uint>) : uint {
    let key = build_string() $(var w) {
        w |> write("fn_{ret}")
        for (p in params) {
            w |> write("_{p}")
        }
    }
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let id = alloc_id(m)
    var ops <- [id, ret]
    ops |> push_from(params)
    emit_n(m, SEC_TYPES, SpvOp.TypeFunction, ops)
    delete ops
    m.type_pool |> insert(key, id)
    return id
}

def public type_runtime_array(var m : SpirvModule; elem : uint; stride : int) : uint {
    if (stride <= 0) {
        panic("type_runtime_array: ArrayStride must be positive, got {stride}")
    }
    let key = "rta{elem}_s{stride}"
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.TypeRuntimeArray, id, elem)
    emit(m, SEC_DECOR, SpvOp.Decorate, id, uint(SpvDecoration.ArrayStride), uint(stride))
    m.type_pool |> insert(key, id)
    return id
}

// A runtime-sized array of an OPAQUE element (an image / sampled image / sampler) -- the descriptor
// array a variable-descriptor-count binding exposes. Opaque types have no byte layout, so unlike the
// SSBO form above this one carries NO ArrayStride; decorating it would be invalid SPIR-V. Deduped on
// the element alone, in its own key space so it never collides with a strided array of the same id.
def public type_runtime_array_opaque(var m : SpirvModule; elem : uint) : uint {
    let key = "rtao{elem}"
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.TypeRuntimeArray, id, elem)
    m.type_pool |> insert(key, id)
    return id
}

// A fixed-size array type. OpTypeArray's length operand is the id of a uint OpConstant (not a
// literal), so the length constant is emitted first (define-before-use within SEC_TYPES). Deduped
// by element + length.
def public type_array(var m : SpirvModule; elem : uint; len : int) : uint {
    if (len <= 0) {
        panic("type_array: length must be positive, got {len}")
    }
    let key = "arr{elem}_n{len}"
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let len_id = const_uint(m, uint(len))
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.TypeArray, id, elem, len_id)
    m.type_pool |> insert(key, id)
    return id
}

// A fixed-size array type with an ArrayStride decoration — required when the array appears as a
// member of a Block-decorated struct (std140/std430 layout). The stride is part of the dedup key so
// the same (elem,len) pair used at two different strides stays as two distinct types.
def public type_array_strided(var m : SpirvModule; elem : uint; len : int; stride : int) : uint {
    if (len <= 0) {
        panic("type_array_strided: length must be positive, got {len}")
    }
    if (stride <= 0) {
        panic("type_array_strided: ArrayStride must be positive, got {stride}")
    }
    let key = "arr{elem}_n{len}_s{stride}"
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let len_id = const_uint(m, uint(len))
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.TypeArray, id, elem, len_id)
    emit(m, SEC_DECOR, SpvOp.Decorate, id, uint(SpvDecoration.ArrayStride), uint(stride))
    m.type_pool |> insert(key, id)
    return id
}

// A composite (vector / array / struct) constant from already-emitted constituent constant ids.
// Deduped by type + constituents — two identical composites collapse to one OpConstantComposite.
def public const_composite(var m : SpirvModule; type_id : uint; parts : array<uint>) : uint {
    let key = build_string() $(var wr) {
        wr |> write("cc{type_id}")
        for (p in parts) {
            wr |> write("_{p}")
        }
    }
    let ex = m.const_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let id = alloc_id(m)
    var ops <- [type_id, id]
    ops |> push_from(parts)
    emit_n(m, SEC_TYPES, SpvOp.ConstantComposite, ops)
    delete ops
    m.const_pool |> insert(key, id)
    return id
}

// A block-decorated struct (UBO/SSBO interface). Members + their byte offsets are supplied by
// the caller (it owns layout). Two structs with identical member ids + offsets dedup to one.
def public type_struct_block(var m : SpirvModule; members : array<uint>; offsets : array<int>) : uint {
    var no_mats : array<int>   // no matrix members
    no_mats |> resize(length(members))
    let id = type_struct_block(m, members, offsets, no_mats)
    delete no_mats
    return id
}

// As above, plus per-member matrix strides: mat_strides[k] > 0 marks member k as a matrix needing
// ColMajor + MatrixStride decorations (daslang matrices are column-major, so ColMajor matches). The
// stride is part of the dedup key so the decorations stay tied to a single deduped type emission.
def public type_struct_block(var m : SpirvModule; members : array<uint>; offsets : array<int>; mat_strides : array<int>) : uint {
    return type_struct_decorated(m, members, offsets, mat_strides, true)
}

// A laid-out struct WITHOUT the Block decoration — used as the element type of an SSBO runtime array
// (the outer wrapping struct carries Block; the element struct only needs member Offset/matrix
// decorations). Same offset/matrix-stride contract as type_struct_block.
def public type_struct_layout(var m : SpirvModule; members : array<uint>; offsets : array<int>; mat_strides : array<int>) : uint {
    return type_struct_decorated(m, members, offsets, mat_strides, false)
}

// Shared core: emit a laid-out OpTypeStruct with member Offset (+ ColMajor/MatrixStride for matrix
// members), optionally Block-decorated. `as_block` is part of the dedup key so a Block and a non-Block
// struct with identical members/offsets stay distinct types.
def private type_struct_decorated(var m : SpirvModule; members : array<uint>; offsets : array<int>; mat_strides : array<int>; as_block : bool) : uint {
    if (empty(members) || length(members) != length(offsets) || length(members) != length(mat_strides)) {
        panic("type_struct_decorated: members/offsets/mat_strides must be non-empty and equal length (members={length(members)}, offsets={length(offsets)}, mat_strides={length(mat_strides)})")
    }
    let key = build_string() $(var wr) {
        wr |> write(as_block ? "struct_b" : "struct_l")
        for (mid in members) {
            wr |> write("_{mid}")
        }
        for (off in offsets) {
            wr |> write("@{off}")
        }
        for (ms in mat_strides) {
            wr |> write("m{ms}")
        }
    }
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let id = alloc_id(m)
    var ops : array<uint>
    ops |> push(id)
    ops |> push_from(members)
    emit_n(m, SEC_TYPES, SpvOp.TypeStruct, ops)
    delete ops
    if (as_block) {
        emit(m, SEC_DECOR, SpvOp.Decorate, id, uint(SpvDecoration.Block))
    }
    for (k in range(length(offsets))) {
        emit(m, SEC_DECOR, SpvOp.MemberDecorate, id, uint(k), uint(SpvDecoration.Offset), uint(offsets[k]))
        if (mat_strides[k] > 0) {
            // a matrix member: column-major (daslang's layout) + the per-column byte stride
            emit(m, SEC_DECOR, SpvOp.MemberDecorate, id, uint(k), uint(SpvDecoration.ColMajor))
            emit(m, SEC_DECOR, SpvOp.MemberDecorate, id, uint(k), uint(SpvDecoration.MatrixStride), uint(mat_strides[k]))
        }
    }
    m.type_pool |> insert(key, id)
    return id
}

def public const_uint(var m : SpirvModule; v : uint) : uint {
    let key = "cu_{v}"
    let ex = m.const_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let ut = type_uint(m)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.Constant, ut, id, v)
    m.const_pool |> insert(key, id)
    return id
}

// The zero of ANY type, whatever its shape -- OpConstantNull takes the type id and needs no
// per-component constituents, so one entry serves scalars, vectors, matrices and arrays alike.
// Keyed by that type id, which the type pool has already made unique.
def public const_null(var m : SpirvModule; t : uint) : uint {
    let key = "cn_{t}"
    let ex = m.const_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.ConstantNull, t, id)
    m.const_pool |> insert(key, id)
    return id
}

// Signed-int and float OpConstants. The literal word is the raw 32-bit pattern: uint(v) carries an
// int's two's-complement bits; reinterpret carries a float's IEEE-754 bits. Keyed separately from
// const_uint so int 2 and uint 2 (distinct types) don't collide in the pool.
def public const_int(var m : SpirvModule; v : int) : uint {
    let key = "ci_{v}"
    let ex = m.const_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let it = type_int(m, 32u, 1u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.Constant, it, id, uint(v))
    m.const_pool |> insert(key, id)
    return id
}


def public const_float(var m : SpirvModule; v : float) : uint {
    // Key by the raw IEEE-754 bits, NOT the stringified float: stringifying would collapse distinct
    // bit patterns (-0.0 vs 0.0, NaN payloads, rounding-equal values) into one pool entry and reuse
    // the wrong constant id. The bits are also the literal word, so compute once.
    let bits = unsafe(reinterpret<uint>(v))
    let key = "cf_{bits}"
    let ex = m.const_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let ft = type_float(m, 32u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.Constant, ft, id, bits)
    m.const_pool |> insert(key, id)
    return id
}

// The 64-bit float OpConstant. Unlike every other constant here the literal spans TWO words, low-order
// first (SPIR-V 2.2.1), so a 64-bit value is the one case the 4-operand emit is spent in full. Keyed on
// the bits, for the reason const_float spells out above.
def public const_double(var m : SpirvModule; v : double) : uint {
    let bits = unsafe(reinterpret<uint64>(v))
    let key = "cd_{bits}"
    let ex = m.const_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let ft = type_float(m, 64u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.Constant, ft, id, uint(bits & 0xfffffffful), uint(bits >> 32ul))
    m.const_pool |> insert(key, id)
    return id
}

// Boolean OpConstant: OpConstantTrue / OpConstantFalse carry NO literal word (the opcode IS the
// value), so this is type + result-id only. Keyed separately so it never collides with const_int 0/1.
// f32 -> f16 bits, round-to-nearest-even — the das CPU reference semantics exactly (its
// float16 converts are RNE too, so a folded CPU constant and this routine agree bit-for-bit).
// Pure das because the f16_cvt daslib lives in dasLLVM (not requireable from here).
def public float_to_half_bits(v : float) : uint {
    let bits = unsafe(reinterpret<uint>(v))
    let s = (bits >> 16u) & 0x8000
    let ebits = int((bits >> 23u) & 0xff)
    var mant = bits & 0x7fffff
    if (ebits == 0xff) {    // inf / nan (keep a nan payload bit so nan stays nan)
        return s | 0x7c00 | (mant != 0u ? (0x200 | (mant >> 13u)) : 0u)
    }
    let e = ebits - 127 + 15
    if (e >= 0x1f) {
        return s | 0x7c00  // overflow -> signed infinity
    }
    if (e <= 0) {           // subnormal half (or underflow to signed zero)
        if (e < -10) {
            return s
        }
        mant |= 0x800000
        let shift = uint(14 - e)
        var r = mant >> shift
        let rem = mant & ((1u << shift) - 1u)
        let halfway = 1u << (shift - 1u)
        if (rem > halfway || (rem == halfway && (r & 1u) != 0u)) {
            r ++
        }
        return s | r
    }
    var r = (uint(e) << 10u) | (mant >> 13u)
    let rem = mant & 0x1fff
    if (rem > 0x1000 || (rem == 0x1000 && (r & 1u) != 0u)) {
        r ++                // RNE; a mantissa carry rolls into the exponent correctly (may hit inf)
    }
    return s | r
}

// Sub-32-bit OpConstant literals pack into ONE word: floats zero-extend, signed ints
// sign-extend, unsigned ints zero-extend (SPIR-V 2.2.1 literal rules).
def public const_float16(var m : SpirvModule; bits16 : uint) : uint {
    let key = "ch_{bits16}"
    let ex = m.const_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let ft = type_float(m, 16u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.Constant, ft, id, bits16 & 0xffff)
    m.const_pool |> insert(key, id)
    return id
}

def public const_int16(var m : SpirvModule; v : int) : uint {
    let key = "ci16_{v}"
    let ex = m.const_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let it = type_int(m, 16u, 1u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.Constant, it, id, uint(v))     // sign-extended word
    m.const_pool |> insert(key, id)
    return id
}

def public const_uint16(var m : SpirvModule; v : uint) : uint {
    let key = "cu16_{v}"
    let ex = m.const_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let it = type_int(m, 16u, 0u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.Constant, it, id, v & 0xffff)
    m.const_pool |> insert(key, id)
    return id
}

def public const_int8(var m : SpirvModule; v : int) : uint {
    let key = "ci8_{v}"
    let ex = m.const_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let it = type_int(m, 8u, 1u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.Constant, it, id, uint(v))     // sign-extended word
    m.const_pool |> insert(key, id)
    return id
}

def public const_uint8(var m : SpirvModule; v : uint) : uint {
    let key = "cu8_{v}"
    let ex = m.const_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let it = type_int(m, 8u, 0u)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, SpvOp.Constant, it, id, v & 0xff)
    m.const_pool |> insert(key, id)
    return id
}

def public const_bool(var m : SpirvModule; v : bool) : uint {
    let key = "cb_{v}"
    let ex = m.const_pool?[key] ?? 0u
    return ex if (ex != 0u)
    let bt = type_bool(m)
    let id = alloc_id(m)
    emit(m, SEC_TYPES, v ? SpvOp.ConstantTrue : SpvOp.ConstantFalse, bt, id)
    m.const_pool |> insert(key, id)
    return id
}

// ===== finalize: header (5 words) ++ sections in layout order =====
def public module_words(m : SpirvModule) : array<uint> {
    // header: magic, version, generator, id-bound (all result-ids < next_id), schema (0)
    var out <- [SPV_MAGIC, m.version, SPV_GENERATOR_MAGIC, m.next_id, 0u]
    for (s in m.sections) {
        out |> push_from(s)
    }
    return <- out
}
