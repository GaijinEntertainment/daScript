options gen2
options indenting = 4

// Shader reflection — a NEUTRAL (graphics-API-agnostic) description of a shader's descriptor bindings
// and push-constant ranges, emitted by dasSpirv alongside the SPIR-V blob and consumed by the host
// (e.g. dasVulkan) to auto-build descriptor-set / pipeline layouts. Single source of truth: the same
// per-global classification that emits the SPIR-V also fills the reflection, so the host never
// re-declares set/binding/type by hand. The producer serializes it to an array<uint> (so it rides the
// exact same module-global-capture rail as the blob); the host calls decode_reflection once at
// layout-build time. The wire form is versioned (REFLECTION_MAGIC) so producer/consumer drift is loud.

module spirv_reflect shared public

//! Descriptor kind, independent of any graphics API (the host maps it to e.g. VkDescriptorType).
enum SpirvDescriptorKind {
    uniform_buffer
    storage_buffer
    combined_image_sampler
}

//! Stages a binding / push range is used in (the host maps to e.g. VkShaderStageFlags). A single
//! shader emits one bit; the host ORs the bits when merging reflections that share a (set, binding).
bitfield SpirvStageFlags {
    vertex
    fragment
    compute
}

//! One descriptor binding (a UBO / SSBO / combined image sampler) at a (set, binding) slot.
struct SpirvDescriptorBinding {
    set     : int
    binding : int
    kind    : SpirvDescriptorKind
    count   : int                   //!< array size; 1 for a non-array binding
    stages  : SpirvStageFlags
}

//! One push-constant range (offset / size in bytes, both 4-aligned).
struct SpirvPushConstantRange {
    offset : int
    size   : int
    stages : SpirvStageFlags
}

//! Everything the host needs to build descriptor-set + pipeline layouts for one shader stage.
struct SpirvReflection {
    stage          : SpirvStageFlags
    entry_point    : string
    local_size     : int3           //!< compute workgroup size; (1,1,1) for graphics stages
    bindings       : array<SpirvDescriptorBinding>
    push_constants : array<SpirvPushConstantRange>
}

// 'RFL1' — wire-format magic + version. Bump on any layout change to encode/decode_reflection.
let REFLECTION_MAGIC = 0x52464C31u

def finalize(var r : SpirvReflection) {
    delete r.bindings
    delete r.push_constants
}

//! Serialize a reflection to a flat uint word stream (the producer-side capture form).
def encode_reflection(refl : SpirvReflection) : array<uint> {
    let nb = length(refl.bindings)
    let np = length(refl.push_constants)
    var w <- [REFLECTION_MAGIC, uint(refl.stage),
        uint(refl.local_size.x), uint(refl.local_size.y), uint(refl.local_size.z),
        uint(nb)]
    w |> reserve(length(w) + 5 * nb + 1 + 3 * np)
    for (b in refl.bindings) {
        w |> push(uint(b.set))
        w |> push(uint(b.binding))
        w |> push(uint(int(b.kind)))
        w |> push(uint(b.count))
        w |> push(uint(b.stages))
    }
    w |> push(uint(np))
    for (p in refl.push_constants) {
        w |> push(uint(p.offset))
        w |> push(uint(p.size))
        w |> push(uint(p.stages))
    }
    return <- w
}

def private kind_from_int(k : int) : SpirvDescriptorKind {
    if (k == int(SpirvDescriptorKind.storage_buffer)) return SpirvDescriptorKind.storage_buffer
    if (k == int(SpirvDescriptorKind.combined_image_sampler)) return SpirvDescriptorKind.combined_image_sampler
    return SpirvDescriptorKind.uniform_buffer
}

// Rebuild the named bitfield from the wire uint. The ctor is a whole-value cast (uint shares the
// bitfield's underlying repr) — needed, not redundant, so the LINT005 "same type" flag is a false
// positive here. (The bit-by-bit `s.vertex = bool` form is lint-clean but the AOT C++ emitter lowers
// it to an ambiguous __bit_set(Bitfield, uint, bool) -> C2666; the cast form AOT-compiles cleanly.)
def private stage_flags_from_uint(raw : uint) : SpirvStageFlags {
    return SpirvStageFlags(raw)     // nolint:LINT005
}

//! Decode the producer's word stream into a typed reflection. An empty / non-magic stream decodes to
//! an empty reflection (entry "main", local_size (1,1,1)) rather than panicking.
def decode_reflection(words : array<uint>) : SpirvReflection {
    var r = SpirvReflection(entry_point = "main", local_size = int3(1, 1, 1))
    if (length(words) < 6 || words[0] != REFLECTION_MAGIC) {
        return <- r
    }
    r.stage = stage_flags_from_uint(words[1])
    r.local_size = int3(int(words[2]), int(words[3]), int(words[4]))
    var idx = 5
    let nb = int(words[idx])
    idx++
    r.bindings |> reserve(nb)
    for (_ in range(nb)) {
        var b : SpirvDescriptorBinding
        b.set = int(words[idx]); idx++
        b.binding = int(words[idx]); idx++
        b.kind = kind_from_int(int(words[idx])); idx++
        b.count = int(words[idx]); idx++
        b.stages = stage_flags_from_uint(words[idx]); idx++
        r.bindings |> push(b)
    }
    let np = int(words[idx])
    idx++
    r.push_constants |> reserve(np)
    for (_ in range(np)) {
        var p : SpirvPushConstantRange
        p.offset = int(words[idx]); idx++
        p.size = int(words[idx]); idx++
        p.stages = stage_flags_from_uint(words[idx]); idx++
        r.push_constants |> push(p)
    }
    return <- r
}
