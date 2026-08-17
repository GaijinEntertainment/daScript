options gen2
options _comment_hygiene = true

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

def find_mem(phys : VkPhysicalDevice; bits : uint; want : VkMemoryPropertyFlags) : uint {
    return find_memory_type(phys, bits, want)
}

[export]
def main() : int {   // nolint:STYLE038 - flat example scaffold
    if (volkInitialize() != 0) {
        to_log(LOG_ERROR, "no Vulkan loader\n")
        return 1
    }
    var inscope instance <- create_instance("dasVulkan compute", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    // any queue family with compute
    var qcount = 0u
    vkGetPhysicalDeviceQueueFamilyProperties(phys, qcount, null)
    var qprops : array<VkQueueFamilyProperties>
    qprops |> resize(int(qcount))
    unsafe {
        vkGetPhysicalDeviceQueueFamilyProperties(phys, qcount, addr(qprops[0]))
    }
    var fam = -1
    for (i in range(int(qcount))) {
        if (qprops[i].queueFlags.compute) {
            fam = i
            break
        }
    }
    if (fam < 0) {
        to_log(LOG_ERROR, "no compute queue\n")
        return 1
    }
    var inscope device <- create_device(phys, uint(fam))
    volkLoadDevice(boost_value_to_vk(device))
    let dev = boost_value_to_vk(device)
    let queue = get_device_queue(device, uint(fam), 0u)

    // ---- host-visible storage buffer (raw) ----
    let buf_size = uint64(N * 4)
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
    mai.memoryTypeIndex = find_mem(phys, req.memoryTypeBits, want)
    var memory : VkDeviceMemory
    vk_check(vkAllocateMemory(dev, mai, null, memory), null)
    vk_check(vkBindBufferMemory(dev, buffer, memory, 0ul), null)

    // ---- descriptor set layout: one storage buffer at binding 0 ----
    var binding : VkDescriptorSetLayoutBinding
    binding.binding = 0u
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

    // ---- descriptor pool + set ----
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
    // point the descriptor at our buffer
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

    // ---- compute pipeline ----
    var inscope shader <- create_shader_module(device, square_spv)
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

    // ---- dispatch ----
    var inscope pool <- create_command_pool(device, pool_info(uint(fam)))
    run_cmd_sync(device, pool, queue) $(cmd) {
        // pipeline/layout/desc_set are raw here (raw descriptor setup), so unwrap the
        // command buffer; cmd_dispatch is a generated boost wrapper taking CommandBuffer.
        vkCmdBindPipeline(boost_value_to_vk(cmd), VkPipelineBindPoint.COMPUTE, pipeline)
        unsafe {
            vkCmdBindDescriptorSets(boost_value_to_vk(cmd), VkPipelineBindPoint.COMPUTE, pipe_layout, 0u, 1u, addr(desc_set), 0u, null)
        }
        cmd_dispatch(cmd, uint(N / GROUP), 1u, 1u)
    }

    // ---- read back + verify ----
    var mapped : void? = null
    let mf : VkMemoryMapFlags
    unsafe {
        vk_check(vkMapMemory(dev, memory, 0ul, buf_size, mf, addr(mapped)), null)
    }
    var values : array<uint>
    values |> resize(N)
    unsafe {
        memcpy(addr<void?>(values[0]), mapped, N * 4)
    }
    vkUnmapMemory(dev, memory)

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

    // raw cleanup (boost wrappers finalize automatically)
    vkDestroyPipeline(dev, pipeline, null)
    vkDestroyPipelineLayout(dev, pipe_layout, null)
    vkDestroyDescriptorPool(dev, desc_pool, null)
    vkDestroyDescriptorSetLayout(dev, set_layout, null)
    vkDestroyBuffer(dev, buffer, null)
    vkFreeMemory(dev, memory, null)

    if (fails != 0) {
        return 1
    }
    print("compute OK\n")
    return 0
}

def pool_info(queue_family : uint) : CommandPoolCreateInfo {
    var ci : CommandPoolCreateInfo
    ci.queueFamilyIndex = queue_family
    return ci
}
