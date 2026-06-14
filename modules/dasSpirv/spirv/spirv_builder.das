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

// A block-decorated struct (UBO/SSBO interface). Members + their byte offsets are supplied by
// the caller (it owns layout). Two structs with identical member ids + offsets dedup to one.
def public type_struct_block(var m : SpirvModule; members : array<uint>; offsets : array<int>) : uint {
    if (empty(members) || length(members) != length(offsets)) {
        panic("type_struct_block: members and offsets must be non-empty and equal length (members={length(members)}, offsets={length(offsets)})")
    }
    let key = build_string() $(var wr) {
        wr |> write("struct_b")
        for (mid in members) {
            wr |> write("_{mid}")
        }
        for (off in offsets) {
            wr |> write("@{off}")
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
    emit(m, SEC_DECOR, SpvOp.Decorate, id, uint(SpvDecoration.Block))
    for (k in range(length(offsets))) {
        emit(m, SEC_DECOR, SpvOp.MemberDecorate, id, uint(k), uint(SpvDecoration.Offset), uint(offsets[k]))
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

// ===== finalize: header (5 words) ++ sections in layout order =====
def public module_words(m : SpirvModule) : array<uint> {
    // header: magic, version, generator, id-bound (all result-ids < next_id), schema (0)
    var out <- [SPV_MAGIC, SPV_VERSION_1_3, SPV_GENERATOR_MAGIC, m.next_id, 0u]
    for (s in m.sections) {
        out |> push_from(s)
    }
    return <- out
}
