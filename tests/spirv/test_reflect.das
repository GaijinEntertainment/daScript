options gen2
options indenting = 4

// Phase-5 reflection gate: each shader fixture emits a companion {name}_reflect : array<uint> global
// that decode_reflection turns into a typed SpirvReflection. These asserts prove the host sees the
// exact set/binding/kind/stage/local_size/push-range the shader declared — the single source of truth
// that lets dasVulkan auto-build descriptor + pipeline layouts without re-declaring any of it by hand.

require dastest/testing_boost public
require _spirv_common
require spirv/spirv_reflect

[test]
def test_reflect_compute_ssbo(t : T?) {
    t |> run("reflect: compute square -> one storage_buffer @ (0,0), local_size (64,1,1)") <| @(t : T?) {
        var r = decode_reflection(square_spv_reflect)
        t |> success(r.stage.compute, "square: stage = compute")
        t |> success(r.local_size.x == 64 && r.local_size.y == 1 && r.local_size.z == 1, "square: local_size (64,1,1)")
        t |> success(length(r.bindings) == 1, "square: one binding")
        t |> success(empty(r.push_constants), "square: no push constants")
        if (length(r.bindings) == 1) {
            let b = r.bindings[0]
            t |> success(b.set == 0 && b.binding == 0, "square: binding @ (0,0)")
            t |> success(b.kind == SpirvDescriptorKind.storage_buffer, "square: storage_buffer kind")
            t |> success(b.count == 1, "square: count 1")
            t |> success(b.stages.compute, "square: stages = compute")
        }
        finalize(r)
    }
}

[test]
def test_reflect_uniform_fragment(t : T?) {
    t |> run("reflect: ubo_frag -> one uniform_buffer @ (0,0), fragment stage") <| @(t : T?) {
        var r = decode_reflection(ubo_frag_spv_reflect)
        t |> success(r.stage.fragment, "ubo_frag: stage = fragment")
        t |> success(length(r.bindings) == 1 && empty(r.push_constants), "ubo_frag: one binding, no push")
        if (length(r.bindings) == 1) {
            let b = r.bindings[0]
            t |> success(b.set == 0 && b.binding == 0, "ubo_frag: binding @ (0,0)")
            t |> success(b.kind == SpirvDescriptorKind.uniform_buffer, "ubo_frag: uniform_buffer kind")
            t |> success(b.stages.fragment, "ubo_frag: stages = fragment")
        }
        finalize(r)
    }
}

[test]
def test_reflect_uniform_vertex(t : T?) {
    t |> run("reflect: mvp_vert -> one uniform_buffer @ (0,0), vertex stage") <| @(t : T?) {
        var r = decode_reflection(mvp_vert_spv_reflect)
        t |> success(r.stage.vertex, "mvp_vert: stage = vertex")
        t |> success(length(r.bindings) == 1, "mvp_vert: one binding")
        if (length(r.bindings) == 1) {
            let b = r.bindings[0]
            t |> success(b.kind == SpirvDescriptorKind.uniform_buffer && b.stages.vertex, "mvp_vert: uniform_buffer in vertex stage")
        }
        finalize(r)
    }
}

[test]
def test_reflect_push_constant(t : T?) {
    t |> run("reflect: pc_frag -> one push-constant range (offset 0, size 20), no descriptors") <| @(t : T?) {
        var r = decode_reflection(pc_frag_spv_reflect)
        t |> success(empty(r.bindings), "pc_frag: no descriptor bindings")
        t |> success(length(r.push_constants) == 1, "pc_frag: one push-constant range")
        if (length(r.push_constants) == 1) {
            let p = r.push_constants[0]
            t |> success(p.offset == 0, "pc_frag: push range offset 0")
            t |> success(p.size == 20, "pc_frag: push range size 20 (float4@0 + float@16)")
            t |> success(p.stages.fragment, "pc_frag: push range in fragment stage")
        }
        finalize(r)
    }
}

[test]
def test_reflect_sampler(t : T?) {
    t |> run("reflect: tex_frag -> one combined_image_sampler @ (0,0)") <| @(t : T?) {
        var r = decode_reflection(tex_frag_spv_reflect)
        t |> success(length(r.bindings) == 1, "tex_frag: one binding")
        if (length(r.bindings) == 1) {
            let b = r.bindings[0]
            t |> success(b.set == 0 && b.binding == 0, "tex_frag: binding @ (0,0)")
            t |> success(b.kind == SpirvDescriptorKind.combined_image_sampler, "tex_frag: combined_image_sampler kind")
            t |> success(b.stages.fragment, "tex_frag: stages = fragment")
        }
        finalize(r)
    }
}

[test]
def test_reflect_malformed(t : T?) {
    t |> run("reflect: malformed streams decode to empty (no over-read / over-alloc / panic)") <| @(t : T?) {
        // empty stream
        var empty_w : array<uint>
        var r0 = decode_reflection(empty_w)
        t |> success(empty(r0.bindings) && empty(r0.push_constants), "empty stream -> empty reflection")
        finalize(r0)
        delete empty_w
        // valid magic + header, but the declared binding count (5) runs past the words
        var trunc <- [REFLECTION_MAGIC, 0u, 1u, 1u, 1u, 5u]
        var r1 = decode_reflection(trunc)
        t |> success(empty(r1.bindings), "truncated binding count -> no bindings parsed")
        finalize(r1)
        delete trunc
        // corrupt huge binding count (decodes to a negative int) must not allocate or crash
        var huge <- [REFLECTION_MAGIC, 0u, 1u, 1u, 1u, 0xFFFFFFFFu]
        var r2 = decode_reflection(huge)
        t |> success(empty(r2.bindings), "huge/negative binding count -> no bindings parsed")
        finalize(r2)
        delete huge
    }
}

[test]
def test_reflect_roundtrip(t : T?) {
    t |> run("reflect: encode/decode round-trips a hand-built reflection") <| @(t : T?) {
        var src : SpirvReflection
        src.stage = SpirvStageFlags.vertex | SpirvStageFlags.fragment
        src.entry_point := "main"
        src.local_size = int3(1, 1, 1)
        src.bindings |> push(SpirvDescriptorBinding(set = 1, binding = 2,
            kind = SpirvDescriptorKind.combined_image_sampler, count = 3, stages = SpirvStageFlags.fragment))
        src.push_constants |> push(SpirvPushConstantRange(offset = 0, size = 64, stages = SpirvStageFlags.vertex))
        var words <- encode_reflection(src)
        var r = decode_reflection(words)
        t |> success(r.stage.vertex && r.stage.fragment, "roundtrip: stage bits preserved")
        t |> success(length(r.bindings) == 1 && r.bindings[0].set == 1 && r.bindings[0].binding == 2, "roundtrip: binding set/binding")
        t |> success(r.bindings[0].count == 3 && r.bindings[0].kind == SpirvDescriptorKind.combined_image_sampler, "roundtrip: binding count/kind")
        t |> success(length(r.push_constants) == 1 && r.push_constants[0].size == 64, "roundtrip: push range size")
        finalize(r)
        finalize(src)
        delete words
    }
}
