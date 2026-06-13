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
    for (s in m.sections) {
        delete s
    }
    delete m.sections
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
    peek_data(s) <| $(bytes) {
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

// ===== finalize: header (5 words) ++ sections in layout order =====
def public module_words(m : SpirvModule) : array<uint> {
    var out : array<uint>
    out |> push(SPV_MAGIC)
    out |> push(SPV_VERSION_1_3)
    out |> push(SPV_GENERATOR_MAGIC)
    out |> push(m.next_id)              // id bound: all result-ids are < next_id
    out |> push(0u)                     // schema (reserved, 0)
    for (s in m.sections) {
        out |> push_from(s)
    }
    return <- out
}
