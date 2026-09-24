options gen2
options indenting = 4

module _coopmat_probe_fixture shared public

require vulkan
require vulkan/vulkan_boost

def put_f32(var buf : array<uint8>; i : int; v : float) {
    let w = unsafe(reinterpret<uint>(v))
    buf[i * 4] = uint8(w & 0xFFu)
    buf[i * 4 + 1] = uint8((w >> 8u) & 0xFFu)
    buf[i * 4 + 2] = uint8((w >> 16u) & 0xFFu)
    buf[i * 4 + 3] = uint8((w >> 24u) & 0xFFu)
}

//! device-local buffer + staged upload (matches the graph: A/B/D live in DEVICE_LOCAL memory)
struct DevBuf {
    buf : Buffer
    mem : DeviceMemory
    bytes : int
}

def make_dev_buf(var device : Device; phys : VkPhysicalDevice; bytes : int) : DevBuf {
    var r : DevBuf
    r.bytes = bytes
    var bci : BufferCreateInfo
    bci.size = uint64(bytes)
    bci.usage.storage_buffer = true
    bci.usage.transfer_dst = true
    bci.usage.transfer_src = true
    r.buf <- create_buffer(device, bci)
    var req : VkMemoryRequirements
    vkGetBufferMemoryRequirements(boost_value_to_vk(device), boost_value_to_vk(r.buf), req)
    var want : VkMemoryPropertyFlags
    want.device_local = true
    let mai = MemoryAllocateInfo(allocationSize = req.size, memoryTypeIndex = find_memory_type(phys, req.memoryTypeBits, want))
    r.mem <- allocate_memory(device, mai)
    vk_check(vkBindBufferMemory(boost_value_to_vk(device), boost_value_to_vk(r.buf), boost_value_to_vk(r.mem), 0ul), null)
    return <- r
}

def staging_roundtrip(var device : Device; phys : VkPhysicalDevice; var pool : CommandPool;
                      queue : VkQueue; var db : DevBuf; var host : array<uint8>; upload : bool) {
    var bci : BufferCreateInfo
    bci.size = uint64(db.bytes)
    bci.usage.transfer_src = true
    bci.usage.transfer_dst = true
    var inscope stage <- create_buffer(device, bci)
    var req : VkMemoryRequirements
    vkGetBufferMemoryRequirements(boost_value_to_vk(device), boost_value_to_vk(stage), req)
    var want : VkMemoryPropertyFlags
    want.host_visible = true
    want.host_coherent = true
    let mai = MemoryAllocateInfo(allocationSize = req.size, memoryTypeIndex = find_memory_type(phys, req.memoryTypeBits, want))
    var inscope smem <- allocate_memory(device, mai)
    vk_check(vkBindBufferMemory(boost_value_to_vk(device), boost_value_to_vk(stage), boost_value_to_vk(smem), 0ul), null)
    var mapped : void? = null
    let mf : VkMemoryMapFlags
    if (upload) {
        unsafe {
            vk_check(vkMapMemory(boost_value_to_vk(device), boost_value_to_vk(smem), 0ul, uint64(db.bytes), mf, addr(mapped)), null)
            memcpy(mapped, addr<void?>(host[0]), db.bytes)
        }
        vkUnmapMemory(boost_value_to_vk(device), boost_value_to_vk(smem))
        run_cmd_sync(device, pool, queue) $(cmd) {
            var region : VkBufferCopy
            region.size = uint64(db.bytes)
            vkCmdCopyBuffer(boost_value_to_vk(cmd), boost_value_to_vk(stage), boost_value_to_vk(db.buf), 1u, unsafe(addr(region)))
        }
    } else {
        run_cmd_sync(device, pool, queue) $(cmd) {
            var region : VkBufferCopy
            region.size = uint64(db.bytes)
            vkCmdCopyBuffer(boost_value_to_vk(cmd), boost_value_to_vk(db.buf), boost_value_to_vk(stage), 1u, unsafe(addr(region)))
        }
        unsafe {
            vk_check(vkMapMemory(boost_value_to_vk(device), boost_value_to_vk(smem), 0ul, uint64(db.bytes), mf, addr(mapped)), null)
            memcpy(addr<void?>(host[0]), mapped, db.bytes)
        }
        vkUnmapMemory(boost_value_to_vk(device), boost_value_to_vk(smem))
    }
}
