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
}

def public make_module() : SpirvModule {
    var m : SpirvModule
    m.next_id = 1u
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

def public type_int(var m : SpirvModule; width, signedness : uint) : uint {
    let key = "i{width}_{signedness}"
    let ex = m.type_pool?[key] ?? 0u
    return ex if (ex != 0u)
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
// e.g. Rgba8, so no StorageImageRead/WriteWithoutFormat capability is needed). depth=0, ms=0; the
// arrayed flag distinguishes a 2D/2DArray (etc.) image. Deduped on the full operand tuple.
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

// ===== finalize: header (5 words) ++ sections in layout order =====
def public module_words(m : SpirvModule) : array<uint> {
    // header: magic, version, generator, id-bound (all result-ids < next_id), schema (0)
    var out <- [SPV_MAGIC, SPV_VERSION_1_3, SPV_GENERATOR_MAGIC, m.next_id, 0u]
    for (s in m.sections) {
        out |> push_from(s)
    }
    return <- out
}
