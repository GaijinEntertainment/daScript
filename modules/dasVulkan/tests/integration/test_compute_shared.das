options gen2
options _comment_hygiene = true
options indenting = 4

// PR 8 (dasSpirv Phase 10.5 GPU regression gate): the compute tier -- @workgroup shared memory
// (Workgroup storage), barrier() (OpControlBarrier), atomicAdd on an SSBO element (OpAtomicIAdd at
// Device scope), and the implicit Workgroup-scope memory ordering. One workgroup of 64
// invocations: each thread writes its local id into shared tile[lid], a barrier rendezvous, then
// each thread atomicAdds tile[lid] onto out[0] and one thread writes the final mark to out[1].
//
// Expected SSBO values after dispatch:
//   out[0] = sum(0..63) = 2016                 -- 64 OpAtomicIAdd's land cumulatively
//   out[1] = 64                                -- the "alive" mark, written once
// A correct dispatch on lavapipe proves shared-memory + control-barrier + atomic semantics, the
// part spirv-val cannot verify dynamically.

require dastest/testing_boost public
require vulkan
require vulkan/vulkan_boost public
require vulkan/vulkan_reflect
require vulkan/vulkan_commands public
require spirv/spirv_shader public
require spirv/spirv_builtins public
require spirv/spirv_reflect public
require scene_helpers

// ===== Shader fixture =====
var @ssbo @binding = 0 csout : array<uint>
var @workgroup cstile : uint[64]

[compute_shader(local_size_x=64, name="compute_shared_spv"), marker(no_coverage)]
def compute_shared {
    let lid = gl_LocalInvocationID.x
    cstile[lid] = lid
    barrier()
    let _o = atomicAdd(csout[0], cstile[lid])
    if (lid == 0u) {
        csout[1] = 64u
    }
}

// ===== Host dispatch =====
def private run_compute_shared() : array<uint> {   // nolint:STYLE038 - flat command-record run
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    var values : array<uint>
    values |> resize(2)
    var inscope instance <- create_instance("dasVulkan test compute_shared", make_api_version(1u, 1u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let dev = boost_value_to_vk(device)
    let queue = get_device_queue(device, gfx, 0u)

    let buf_size = uint64(2 * 4)
    var bci = VkBufferCreateInfo()
    bci.size = buf_size
    bci.usage.storage_buffer = true
    var buffer : VkBuffer
    vk_check(vkCreateBuffer(dev, bci, null, buffer), null)
    var req : VkMemoryRequirements
    vkGetBufferMemoryRequirements(dev, buffer, req)
    var want : VkMemoryPropertyFlags
    want.host_visible = true
    want.host_coherent = true
    var mai = VkMemoryAllocateInfo()
    mai.allocationSize = req.size
    mai.memoryTypeIndex = find_memory_type(phys, req.memoryTypeBits, want)
    var memory : VkDeviceMemory
    vk_check(vkAllocateMemory(dev, mai, null, memory), null)
    vk_check(vkBindBufferMemory(dev, buffer, memory, 0ul), null)
    // zero out the SSBO so the atomic accumulator starts at 0 (host-visible coherent memory).
    var zero : void? = null
    let mf0 : VkMemoryMapFlags
    var zeros : array<uint> = [for (_k in range(2)); 0u]
    unsafe {
        vk_check(vkMapMemory(dev, memory, 0ul, buf_size, mf0, addr(zero)), null)
        memcpy(zero, addr<void?>(zeros[0]), buf_size)
    }
    vkUnmapMemory(dev, memory)
    delete zeros

    var binding : VkDescriptorSetLayoutBinding
    binding.descriptorType = VkDescriptorType.STORAGE_BUFFER
    binding.descriptorCount = 1u
    binding.stageFlags.compute = true
    var dslci = VkDescriptorSetLayoutCreateInfo()
    dslci.bindingCount = 1u
    var set_layout : VkDescriptorSetLayout
    unsafe {
        dslci.pBindings = addr(binding)
        vk_check(vkCreateDescriptorSetLayout(dev, dslci, null, set_layout), null)
    }
    var pool_size : VkDescriptorPoolSize
    pool_size.type_ = VkDescriptorType.STORAGE_BUFFER
    pool_size.descriptorCount = 1u
    var dpci = VkDescriptorPoolCreateInfo()
    dpci.maxSets = 1u
    dpci.poolSizeCount = 1u
    var desc_pool : VkDescriptorPool
    unsafe {
        dpci.pPoolSizes = addr(pool_size)
        vk_check(vkCreateDescriptorPool(dev, dpci, null, desc_pool), null)
    }
    var dsai = VkDescriptorSetAllocateInfo()
    dsai.descriptorPool = desc_pool
    dsai.descriptorSetCount = 1u
    var desc_set : VkDescriptorSet
    unsafe {
        dsai.pSetLayouts = addr(set_layout)
        vk_check(vkAllocateDescriptorSets(dev, dsai, addr(desc_set)), null)
    }
    var dbinfo : VkDescriptorBufferInfo
    dbinfo.buffer = buffer
    dbinfo.range_ = buf_size
    var write = VkWriteDescriptorSet()
    write.dstSet = desc_set
    write.descriptorCount = 1u
    write.descriptorType = VkDescriptorType.STORAGE_BUFFER
    unsafe {
        write.pBufferInfo = addr(dbinfo)
        vkUpdateDescriptorSets(dev, 1u, addr(write), 0u, null)
    }

    var inscope shader <- create_shader_module(device, compute_shared_spv)
    var plci = VkPipelineLayoutCreateInfo()
    plci.setLayoutCount = 1u
    var pipe_layout : VkPipelineLayout
    unsafe {
        plci.pSetLayouts = addr(set_layout)
        vk_check(vkCreatePipelineLayout(dev, plci, null, pipe_layout), null)
    }
    var stage = VkPipelineShaderStageCreateInfo()
    stage.stage.compute = true
    stage.module_ = boost_value_to_vk(shader)
    stage.pName = "main"
    var cpci = VkComputePipelineCreateInfo()
    cpci.stage = stage
    cpci.layout = pipe_layout
    var pipeline : VkPipeline
    unsafe {
        vk_check(vkCreateComputePipelines(dev, null, 1u, addr(cpci), null, addr(pipeline)), null)
    }

    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)
    run_cmd_sync(device, pool, queue) $(cmd) {
        vkCmdBindPipeline(boost_value_to_vk(cmd), VkPipelineBindPoint.COMPUTE, pipeline)
        unsafe {
            vkCmdBindDescriptorSets(boost_value_to_vk(cmd), VkPipelineBindPoint.COMPUTE, pipe_layout,
                0u, 1u, addr(desc_set), 0u, null)
        }
        vkCmdDispatch(boost_value_to_vk(cmd), 1u, 1u, 1u)
    }

    var mapped : void? = null
    let mf : VkMemoryMapFlags
    unsafe {
        vk_check(vkMapMemory(dev, memory, 0ul, buf_size, mf, addr(mapped)), null)
        memcpy(addr<void?>(values[0]), mapped, buf_size)
    }
    vkUnmapMemory(dev, memory)

    vkDestroyPipeline(dev, pipeline, null)
    vkDestroyPipelineLayout(dev, pipe_layout, null)
    vkDestroyDescriptorPool(dev, desc_pool, null)
    vkDestroyDescriptorSetLayout(dev, set_layout, null)
    vkDestroyBuffer(dev, buffer, null)
    vkFreeMemory(dev, memory, null)
    return <- values
}

[test]
def test_compute_shared(t : T?) {
    var values <- run_compute_shared()
    t |> equal(values[0], 2016u)
    t |> success(values[0] == 2016u, "atomicAdd sums shared-memory writes to 0+1+...+63 = 2016 (got {values[0]})")
    t |> equal(values[1], 64u)
    t |> success(values[1] == 64u, "single thread (lid==0) writes the alive mark 64 (got {values[1]})")
    delete values
}
