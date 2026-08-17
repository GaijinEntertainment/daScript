options gen2
options _comment_hygiene = true

// Boost twin of compute.das: the same "square each index" compute dispatch, but
// driven entirely through the vulkan_boost layer — buffer, memory (+ requirements
// and bind), descriptor set layout + pool, pipeline layout, shader, compute
// pipeline, command pool, the dispatch bracket, and the typed readback are all
// boost (RAII; they finalize in reverse order, no manual destroy). The lone raw
// escape hatch is the pool-allocated descriptor set: descriptor sets are
// reclaimed by their pool, so there is no single-object boost creator. compute.das
// does the same work in 208 lines with 21 raw vk calls; this has exactly one.

require vulkan
require vulkan/vulkan_boost
require spirv/spirv_shader
require spirv/spirv_builtins public

let N = 256                 // elements; the shader squares each index
let GROUP = 64              // local_size_x in the [compute_shader] below

// The compute shader, authored in daslang and lowered to SPIR-V at compile time by dasSpirv's
// [compute_shader] macro. The emitted blob is the `square_spv : array<uint>` global below,
// fed straight to create_shader_module -- no GLSL, no glslang, no committed .spv.
var @ssbo @binding = 0 data : array<uint>

[compute_shader(local_size_x=64, name="square_spv")]
def square {
    let i = gl_GlobalInvocationID.x
    data[i] = i * i
}

[export]
def main() : int {   // nolint:STYLE038 - flat example scaffold
    if (volkInitialize() != 0) {
        to_log(LOG_ERROR, "no Vulkan loader\n")
        return 1
    }
    var inscope instance <- create_instance("dasVulkan compute (boost)", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    // A graphics-capable queue family also supports compute on every practical
    // implementation (and on lavapipe), so reuse the graphics selector rather
    // than enumerating queue families by hand.
    let fam = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, fam)
    volkLoadDevice(boost_value_to_vk(device))
    let dev = boost_value_to_vk(device)
    let queue = get_device_queue(device, fam, 0u)

    // ---- host-visible storage buffer (boost) ----
    let buf_size = uint64(N * 4)
    var bci : BufferCreateInfo
    bci.size = buf_size
    bci.usage.storage_buffer = true
    var inscope buffer <- create_buffer(device, bci)

    let req = get_buffer_memory_requirements(device, buffer)
    var want : VkMemoryPropertyFlags
    want.host_visible = true
    want.host_coherent = true
    let mai = MemoryAllocateInfo(allocationSize = req.size,
                                 memoryTypeIndex = find_memory_type(phys, req.memoryTypeBits, want))
    var inscope memory <- allocate_memory(device, mai)
    bind_buffer_memory(device, buffer, memory, 0ul)

    // ---- descriptor set layout + pool (boost) ----
    var dslci : DescriptorSetLayoutCreateInfo
    var b0 = DescriptorSetLayoutBinding(binding = 0u,
                                        descriptorType = VkDescriptorType.STORAGE_BUFFER,
                                        descriptorCount = 1u)
    b0.stageFlags.compute = true
    dslci.pBindings |> emplace(b0)
    var inscope set_layout <- create_descriptor_set_layout(device, dslci)

    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = 1u
    var psize = DescriptorPoolSize(type_ = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
    dpci.pPoolSizes |> push(psize)
    var inscope desc_pool <- create_descriptor_pool(device, dpci)

    // descriptor sets are reclaimed by their pool, so there is no single-object
    // boost creator — allocate it raw from the (boost-owned) pool + layout.
    var dsai = VkDescriptorSetAllocateInfo()
    dsai.descriptorPool = boost_value_to_vk(desc_pool)
    dsai.descriptorSetCount = 1u
    var raw_set_layout = boost_value_to_vk(set_layout)
    var raw_set : VkDescriptorSet
    unsafe {
        dsai.pSetLayouts = addr(raw_set_layout)
        vk_check(vkAllocateDescriptorSets(dev, dsai, addr(raw_set)), null)
    }

    // point the descriptor at our buffer (boost update_descriptor_sets)
    var write = WriteDescriptorSet(dstSet = vk_value_to_boost(raw_set),
                                   dstBinding = 0u,
                                   descriptorType = VkDescriptorType.STORAGE_BUFFER,
                                   descriptorCount = 1u)
    var binfo = DescriptorBufferInfo(buffer = weak_copy(buffer), range_ = buf_size)
    write.pBufferInfo |> push(binfo)
    var writes : array<WriteDescriptorSet>
    writes |> emplace(write)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    // ---- compute pipeline (boost: owning, single-object) ----
    var inscope shader <- create_shader_module(device, square_spv)
    var plci : PipelineLayoutCreateInfo
    plci.pSetLayouts <- [weak_copy(set_layout)]
    var inscope pipe_layout <- create_pipeline_layout(device, plci)
    var inscope pipeline <- create_compute_pipeline(device, pipe_layout, shader)

    // ---- dispatch (all boost) ----
    var inscope pool <- create_command_pool(device, pool_info(fam))
    run_cmd_sync(device, pool, queue) $(cmd) {
        cmd_bind_pipeline(cmd, pipeline, VkPipelineBindPoint.COMPUTE)
        var no_offsets : array<uint>
        cmd_bind_descriptor_sets(cmd, VkPipelineBindPoint.COMPUTE, pipe_layout, 0u, [vk_value_to_boost(raw_set)], no_offsets)
        cmd_dispatch(cmd, uint(N / GROUP), 1u, 1u)
    }

    // ---- read back (typed copying readback — no block, no memcpy) + verify ----
    let values = read_memory(device, memory, type<uint>, N)

    var fails = 0
    for (i in range(N)) {
        let expected = uint(i * i)
        if (values[i] != expected) {
            if (fails < 5) {
                to_log(LOG_ERROR, "  mismatch at {i}: got {values[i]} expected {expected}\n")
            }
            fails++
        }
    }
    let status = fails == 0 ? "all correct" : "{fails} WRONG"
    print("checked {N} elements (out[i] == i*i): {status}\n")
    print("  spot: out[2]={values[2]} out[16]={values[16]} out[255]={values[255]}\n")

    if (fails != 0) {
        return 1
    }
    print("compute OK (boost)\n")
    return 0
}

def pool_info(queue_family : uint) : CommandPoolCreateInfo {
    var ci : CommandPoolCreateInfo
    ci.queueFamilyIndex = queue_family
    return ci
}
