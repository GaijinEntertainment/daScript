options gen2
options _comment_hygiene = true
options indenting = 4

// Unit coverage for vulkan_reflect's layout builders: the neutral-enum mappers and the per-set
// binding merge (the part build_descriptor_set_layouts feeds the driver). The full end-to-end proof
// — that reflection-built layouts actually render — is test_phase4 (render_textured_quad now builds
// its layouts from quad_vert/quad_frag reflection). These are GPU-free: no device required.

require dastest/testing_boost public
require vulkan/vulkan_reflect

[test]
def test_reflect_mappers(t : T?) {
    t |> success(to_vk_descriptor_type(SpirvDescriptorKind.uniform_buffer) == VkDescriptorType.UNIFORM_BUFFER, "uniform_buffer -> UNIFORM_BUFFER")
    t |> success(to_vk_descriptor_type(SpirvDescriptorKind.storage_buffer) == VkDescriptorType.STORAGE_BUFFER, "storage_buffer -> STORAGE_BUFFER")
    t |> success(to_vk_descriptor_type(SpirvDescriptorKind.combined_image_sampler) == VkDescriptorType.COMBINED_IMAGE_SAMPLER, "combined_image_sampler -> COMBINED_IMAGE_SAMPLER")
    t |> success(to_vk_descriptor_type(SpirvDescriptorKind.storage_image) == VkDescriptorType.STORAGE_IMAGE, "storage_image -> STORAGE_IMAGE")
    t |> success(to_vk_descriptor_type(SpirvDescriptorKind.sampled_image) == VkDescriptorType.SAMPLED_IMAGE, "sampled_image -> SAMPLED_IMAGE")
    t |> success(to_vk_descriptor_type(SpirvDescriptorKind.sampler) == VkDescriptorType.SAMPLER, "sampler -> SAMPLER")
    t |> success(to_vk_descriptor_type(SpirvDescriptorKind.comparison_sampler) == VkDescriptorType.SAMPLER, "comparison_sampler -> SAMPLER")
    t |> success(to_vk_descriptor_type(SpirvDescriptorKind.input_attachment) == VkDescriptorType.INPUT_ATTACHMENT, "input_attachment -> INPUT_ATTACHMENT")
    t |> success(to_vk_descriptor_type(SpirvDescriptorKind.acceleration_structure) == VkDescriptorType.ACCELERATION_STRUCTURE_KHR, "acceleration_structure -> ACCELERATION_STRUCTURE_KHR")
    let vf = to_vk_stage_flags(SpirvStageFlags.vertex | SpirvStageFlags.compute)
    t |> success(vf.vertex && vf.compute && !vf.fragment, "stage flags map vertex|compute, not fragment")
    let rt = to_vk_stage_flags(SpirvStageFlags.raygen | SpirvStageFlags.miss | SpirvStageFlags.closest_hit)
    t |> success(rt.raygen_khr && rt.miss_khr && rt.closest_hit_khr && !rt.compute, "RT stage flags map raygen|miss|closest_hit, not compute")
}

[test]
def test_reflect_merge_shared_binding(t : T?) {
    // two stages both declaring set 0 / binding 0 (a UBO read in vertex AND fragment)
    var rv : SpirvReflection
    rv.bindings |> push(SpirvDescriptorBinding(set = 0, binding = 0, kind = SpirvDescriptorKind.uniform_buffer, count = 1, stages = SpirvStageFlags.vertex))
    var rf : SpirvReflection
    rf.bindings |> push(SpirvDescriptorBinding(set = 0, binding = 0, kind = SpirvDescriptorKind.uniform_buffer, count = 1, stages = SpirvStageFlags.fragment))
    var refls <- [ <- rv, <- rf]
    var merged <- merge_set_bindings(refls, 0)
    t |> success(length(merged) == 1, "shared (set 0, binding 0) collapses to one binding")
    if (length(merged) == 1) {
        t |> success(merged[0].binding == 0u, "merged binding number is 0")
        t |> success(merged[0].descriptorType == VkDescriptorType.UNIFORM_BUFFER, "merged type is UNIFORM_BUFFER")
        t |> success(merged[0].stageFlags.vertex && merged[0].stageFlags.fragment, "merged stage flags are vertex|fragment")
    }
    unsafe { delete merged }    // DescriptorSetLayoutBinding holds a sampler-array field -> unsafe delete
    delete refls
}

[test]
def test_reflect_merge_distinct_bindings(t : T?) {
    // the gate's actual shape: vertex UBO @binding 0, fragment sampler @binding 1, both in set 0
    var rv : SpirvReflection
    rv.bindings |> push(SpirvDescriptorBinding(set = 0, binding = 0, kind = SpirvDescriptorKind.uniform_buffer, count = 1, stages = SpirvStageFlags.vertex))
    var rf : SpirvReflection
    rf.bindings |> push(SpirvDescriptorBinding(set = 0, binding = 1, kind = SpirvDescriptorKind.combined_image_sampler, count = 1, stages = SpirvStageFlags.fragment))
    var refls <- [ <- rv, <- rf]
    var merged <- merge_set_bindings(refls, 0)
    t |> success(length(merged) == 2, "two distinct bindings stay separate")
    if (length(merged) == 2) {
        // merge preserves insertion order: vertex UBO @0 first, then fragment sampler @1
        t |> success(merged[0].binding == 0u && merged[0].descriptorType == VkDescriptorType.UNIFORM_BUFFER &&
            merged[0].stageFlags.vertex && !merged[0].stageFlags.fragment, "binding 0 is a vertex UBO")
        t |> success(merged[1].binding == 1u && merged[1].descriptorType == VkDescriptorType.COMBINED_IMAGE_SAMPLER &&
            merged[1].stageFlags.fragment && !merged[1].stageFlags.vertex, "binding 1 is a fragment sampler")
    }
    unsafe { delete merged }    // DescriptorSetLayoutBinding holds a sampler-array field -> unsafe delete
    delete refls
}
