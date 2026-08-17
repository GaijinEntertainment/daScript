options gen2
options _comment_hygiene = true
options indenting = 4

// Build Vulkan descriptor-set + pipeline layouts directly from dasSpirv shader reflection
// (spirv/spirv_reflect). The shader is the single source of truth: instead of hand-declaring a
// VkDescriptorSetLayout / VkPushConstantRange that must mirror the shader's @set/@binding/type/
// @push_constant, decode each shader's {name}_reflect global and let these builders construct the
// layouts. Bindings used by several stages at the same (set, binding) are merged with their stage
// flags OR-ed; push-constant ranges sharing an (offset, size) likewise.

module vulkan_reflect shared

require vulkan public
require vulkan/vulkan_boost public
require spirv/spirv_reflect public

//! Map a neutral dasSpirv descriptor kind to its VkDescriptorType.
def to_vk_descriptor_type(kind : SpirvDescriptorKind) : VkDescriptorType {
    if (kind == SpirvDescriptorKind.storage_buffer) return VkDescriptorType.STORAGE_BUFFER
    if (kind == SpirvDescriptorKind.combined_image_sampler) return VkDescriptorType.COMBINED_IMAGE_SAMPLER
    if (kind == SpirvDescriptorKind.storage_image) return VkDescriptorType.STORAGE_IMAGE
    if (kind == SpirvDescriptorKind.sampled_image) return VkDescriptorType.SAMPLED_IMAGE
    if (kind == SpirvDescriptorKind.sampler || kind == SpirvDescriptorKind.comparison_sampler) return VkDescriptorType.SAMPLER
    if (kind == SpirvDescriptorKind.input_attachment) return VkDescriptorType.INPUT_ATTACHMENT
    if (kind == SpirvDescriptorKind.acceleration_structure) return VkDescriptorType.ACCELERATION_STRUCTURE_KHR
    return VkDescriptorType.UNIFORM_BUFFER
}

//! Map neutral dasSpirv stage bits to VkShaderStageFlags.
def to_vk_stage_flags(stages : SpirvStageFlags) : VkShaderStageFlags {
    var f : VkShaderStageFlags
    if (stages.vertex) {
        f.vertex = true
    }
    if (stages.fragment) {
        f.fragment = true
    }
    if (stages.compute) {
        f.compute = true
    }
    if (stages.task) {
        f.task_ext = true
    }
    if (stages.mesh) {
        f.mesh_ext = true
    }
    if (stages.raygen) {
        f.raygen_khr = true
    }
    if (stages.miss) {
        f.miss_khr = true
    }
    if (stages.closest_hit) {
        f.closest_hit_khr = true
    }
    return f
}

// Accumulator for merging one (set, binding) slot across stages before it becomes a Vk binding.
struct private MergedBinding {
    binding : int
    kind    : SpirvDescriptorKind
    count   : int
    stages  : SpirvStageFlags
}

struct private MergedPush {
    offset : int
    size   : int
    stages : SpirvStageFlags
}

def private max_set(reflections : array<SpirvReflection>) : int {
    var m = -1
    for (refl in reflections) {
        for (b in refl.bindings) {
            if (b.set > m) {
                m = b.set
            }
        }
    }
    return m
}

//! Merge all reflections' bindings for one descriptor set into Vk bindings, OR-ing stage flags where a
//! (set, binding) slot is used by more than one stage. Returns the exact pBindings list
//! build_descriptor_set_layouts feeds the driver — lets a host or test inspect a set without the Vk object.
def merge_set_bindings(reflections : array<SpirvReflection>; set : int) : array<DescriptorSetLayoutBinding> {
    var merged : array<MergedBinding>
    for (refl in reflections) {
        for (b in refl.bindings) {
            continue if (b.set != set)
            var found = -1
            for (i in range(length(merged))) {
                if (merged[i].binding == b.binding) {
                    found = i
                    break
                }
            }
            if (found >= 0) {
                // A slot shared by several stages must agree on kind+count, else the merged layout is
                // invalid Vulkan. Fail fast — a silent first-wins merge produces a layout that mismatches
                // the shader and is very hard to diagnose on-GPU.
                if (merged[found].kind != b.kind || merged[found].count != b.count) {
                    panic("vulkan_reflect: (set {set}, binding {b.binding}) declared with conflicting kind/count across stages")
                }
                merged[found].stages = merged[found].stages | b.stages
            } else {
                merged |> push(MergedBinding(binding = b.binding, kind = b.kind, count = b.count, stages = b.stages))
            }
        }
    }
    var result : array<DescriptorSetLayoutBinding>
    result |> reserve(length(merged))
    for (m in merged) {
        var vb = DescriptorSetLayoutBinding(binding = uint(m.binding),
            descriptorType = to_vk_descriptor_type(m.kind),
            descriptorCount = uint(m.count),
            stageFlags = to_vk_stage_flags(m.stages))
        result |> emplace(vb)
    }
    delete merged
    return <- result
}

//! Build one DescriptorSetLayout per descriptor set across all reflections, bindings merged by
//! (set, binding) with stage flags OR-ed. Indexed by set number 0..maxSet — a set with no bindings gets
//! an empty layout so indices line up with the pipeline layout. Caller owns them (`var inscope sl <- ...`).
def build_descriptor_set_layouts(device : Device; reflections : array<SpirvReflection>) : array<DescriptorSetLayout> {
    var layouts : array<DescriptorSetLayout>
    let topSet = max_set(reflections)
    layouts |> reserve(topSet + 1)
    for (set in range(topSet + 1)) {
        var dslci : DescriptorSetLayoutCreateInfo
        dslci.pBindings <- merge_set_bindings(reflections, set)
        layouts |> emplace(create_descriptor_set_layout(device, dslci))
        // create_descriptor_set_layout's vk_view_destroy frees only the _vk_view_* temporaries, not the
        // idiomatic pBindings array we moved in — delete it so a per-frame caller does not accumulate.
        unsafe { delete dslci.pBindings }
    }
    return <- layouts
}

//! Build a PipelineLayout from the given (reflection-built) descriptor set layouts plus the merged
//! push-constant ranges of all reflections (ranges sharing an (offset, size) have stage flags OR-ed).
//! The set layouts are referenced (weak), not owned — keep them alive for the pipeline's lifetime.
def build_pipeline_layout(device : Device; set_layouts : array<DescriptorSetLayout>; reflections : array<SpirvReflection>) : PipelineLayout {
    var plci : PipelineLayoutCreateInfo
    plci.pSetLayouts |> reserve(length(set_layouts))
    for (sl in set_layouts) {
        plci.pSetLayouts |> push(weak_copy(sl))
    }
    var ranges : array<MergedPush>
    for (refl in reflections) {
        for (p in refl.push_constants) {
            var found = -1
            for (i in range(length(ranges))) {
                if (ranges[i].offset == p.offset && ranges[i].size == p.size) {
                    found = i
                    break
                }
            }
            if (found >= 0) {
                ranges[found].stages = ranges[found].stages | p.stages
            } else {
                ranges |> push(MergedPush(offset = p.offset, size = p.size, stages = p.stages))
            }
        }
    }
    plci.pPushConstantRanges |> reserve(length(ranges))
    for (r in ranges) {
        var pcr = PushConstantRange(stageFlags = to_vk_stage_flags(r.stages),
            offset = uint(r.offset), size = uint(r.size))
        plci.pPushConstantRanges |> push(pcr)
    }
    delete ranges
    var pl <- create_pipeline_layout(device, plci)
    // vk_view_destroy frees only the _vk_view_* temporaries; delete the idiomatic arrays we built so a
    // per-frame caller does not accumulate. pSetLayouts holds weak copies (finalize is a no-op), so this
    // does not destroy the real set layouts.
    unsafe { delete plci.pSetLayouts }
    delete plci.pPushConstantRanges
    return <- pl
}
