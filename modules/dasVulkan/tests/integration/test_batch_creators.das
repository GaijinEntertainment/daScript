options gen2
options _comment_hygiene = true
options indenting = 4

// The #53 batch creators, end to end on a real driver: allocate_descriptor_sets and
// allocate_command_buffers (shape B: N owning wrappers from one allocate-info) feed a
// compute dispatch whose pipeline comes from create_compute_pipelines (shape A), so
// every batch-created handle is USED, not just non-null. The graphics batch creator
// is covered by test_pipeline_boost_structs.das.

require dastest/testing_boost public
require vulkan
require vulkan/vulkan_boost
require compute_shaders

[test]
def test_batch_creators(t : T?) {   // nolint:STYLE038 - flat test cell
    if (volkInitialize() != 0) {
        t |> failure("no Vulkan loader")
        return
    }
    let n = 256
    var inscope instance <- create_instance("dasVulkan test", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let dev = boost_value_to_vk(device)
    let queue = get_device_queue(device, gfx, 0u)

    // storage buffer for out[i] = i*i
    let buf_size = uint64(n * 4)
    var bci : BufferCreateInfo
    bci.size = buf_size
    bci.usage.storage_buffer = true
    var inscope buffer <- create_buffer(device, bci)
    var req : VkMemoryRequirements
    vkGetBufferMemoryRequirements(dev, boost_value_to_vk(buffer), req)
    var want : VkMemoryPropertyFlags
    want.host_visible = true
    want.host_coherent = true
    let mai = MemoryAllocateInfo(allocationSize = req.size,
        memoryTypeIndex = find_memory_type(phys, req.memoryTypeBits, want))
    var inscope memory <- allocate_memory(device, mai)
    vk_check(vkBindBufferMemory(dev, boost_value_to_vk(buffer), boost_value_to_vk(memory), 0ul), null)

    var dslci : DescriptorSetLayoutCreateInfo
    var b0 = DescriptorSetLayoutBinding(binding = 0u,
        descriptorType = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
    b0.stageFlags.compute = true
    dslci.pBindings |> emplace(b0)
    var inscope set_layout <- create_descriptor_set_layout(device, dslci)

    var dpci : DescriptorPoolCreateInfo
    dpci.maxSets = 2u
    var psize = DescriptorPoolSize(type_ = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 2u)
    dpci.pPoolSizes |> push(psize)
    var inscope desc_pool <- create_descriptor_pool(device, dpci)

    // shape B: batch descriptor-set allocation -- descriptorSetCount comes from the
    // pSetLayouts length, one owning wrapper per set
    var dsai = DescriptorSetAllocateInfo(descriptorPool = weak_copy(desc_pool),
        pSetLayouts <- [weak_copy(set_layout), weak_copy(set_layout)])
    var inscope sets <- allocate_descriptor_sets(device, dsai)
    t |> equal(length(sets), 2)
    t |> success(sets[0]._vk != 0ul && sets[1]._vk != 0ul, "both descriptor sets are non-null")
    t |> success(sets[0]._vk != sets[1]._vk, "batch-allocated descriptor sets are distinct")

    var writes : array<WriteDescriptorSet>
    var w0 = WriteDescriptorSet(dstSet = sets[1], dstBinding = 0u,
        descriptorType = VkDescriptorType.STORAGE_BUFFER, descriptorCount = 1u)
    var binfo = DescriptorBufferInfo(buffer = weak_copy(buffer), range_ = buf_size)
    w0.pBufferInfo |> push(binfo)
    writes |> emplace(w0)
    let no_copies : array<CopyDescriptorSet>
    update_descriptor_sets(device, writes, no_copies)

    // shape A: batch compute-pipeline creation -- two entries from one call; the
    // dispatch below runs the SECOND one, proving per-entry marshal order
    var inscope shader <- create_shader_module(device, square_spv)
    var plci : PipelineLayoutCreateInfo
    plci.pSetLayouts <- [weak_copy(set_layout)]
    var inscope pipe_layout <- create_pipeline_layout(device, plci)
    var stage = VkPipelineShaderStageCreateInfo()
    stage.stage.compute = true
    stage.module_ = boost_value_to_vk(shader)
    stage.pName = "main"
    var cpcis <- [
        ComputePipelineCreateInfo(stage = stage, layout = weak_copy(pipe_layout)),
        ComputePipelineCreateInfo(stage = stage, layout = weak_copy(pipe_layout))]
    let no_cache : PipelineCache
    var inscope pipelines <- create_compute_pipelines(device, no_cache, cpcis)
    t |> equal(length(pipelines), 2)
    t |> success(pipelines[0]._vk != 0ul && pipelines[1]._vk != 0ul, "both compute pipelines are non-null")
    t |> success(pipelines[0]._vk != pipelines[1]._vk, "batch-created compute pipelines are distinct")

    // shape B: batch command-buffer allocation -- record + submit the THIRD one
    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = gfx
    var inscope pool <- create_command_pool(device, poolci)
    var cbai = CommandBufferAllocateInfo(commandPool = weak_copy(pool),
        level = VkCommandBufferLevel.PRIMARY, commandBufferCount = 3u)
    var inscope cbufs <- allocate_command_buffers(device, cbai)
    t |> equal(length(cbufs), 3)
    t |> success(cbufs[0]._vk != 0ul && cbufs[1]._vk != 0ul && cbufs[2]._vk != 0ul,
        "all command buffers are non-null")
    t |> success(cbufs[0]._vk != cbufs[1]._vk && cbufs[1]._vk != cbufs[2]._vk,
        "batch-allocated command buffers are distinct")

    var begin_info : CommandBufferBeginInfo
    begin_command_buffer(cbufs[2], begin_info)
    cmd_bind_pipeline(cbufs[2], pipelines[1], VkPipelineBindPoint.COMPUTE)
    var bind_sets : array<DescriptorSet>
    bind_sets |> push(sets[1])
    var no_dyn : array<uint>
    cmd_bind_descriptor_sets(cbufs[2], VkPipelineBindPoint.COMPUTE, pipe_layout, 0u, bind_sets, no_dyn)
    cmd_dispatch(cbufs[2], uint(n / 64), 1u, 1u)
    end_command_buffer(cbufs[2])

    var si : SubmitInfo
    si.pCommandBuffers |> push(cbufs[2])
    var submits : array<SubmitInfo>
    submits |> emplace(si)
    let no_fence : Fence
    queue_submit(vk_value_to_boost(queue), submits, no_fence)
    device_wait_idle(device)

    var values : array<uint>
    values |> resize(n)
    var mapped : void? = null
    let mf : VkMemoryMapFlags
    unsafe {
        vk_check(vkMapMemory(dev, boost_value_to_vk(memory), 0ul, buf_size, mf, addr(mapped)), null)
        memcpy(addr<void?>(values[0]), mapped, n * 4)
    }
    vkUnmapMemory(dev, boost_value_to_vk(memory))
    var wrong = 0
    for (i in range(n)) {
        if (values[i] != uint(i * i)) {
            wrong++
        }
    }
    t |> success(wrong == 0, "the batch-created pipeline squared all {n} elements through batch-allocated set + command buffer")
    delete values
}
