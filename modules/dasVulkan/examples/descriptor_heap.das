options gen2
options _comment_hygiene = true
options indenting = 4

// The compute_boost.das dispatch with NO descriptor set layout, pool,
// set or pipeline layout: VK_EXT_descriptor_heap replaces them with
// one device-addressable buffer the application writes descriptors
// into through an ordinary memory mapping, binds once per command
// buffer, and indexes from the shader. The shader is the one
// compute_boost.das runs, set=0 / binding=0 and all - a mapping
// chained on the stage tells the driver which heap byte that binding
// reads. tests/integration/test_descriptor_heap.das checks it.

require vulkan public
require vulkan/vulkan_boost public
require compute_example_shaders

let public HEAP_N = 256      //! elements; the shader squares each index
//! the heap slot of the storage buffer's descriptor; slot 0 is empty
let public HEAP_SLOT = 1

//! True when a device on this machine has VK_EXT_descriptor_heap; the
//! example and its test skip otherwise.
def public descriptor_heap_available() : bool {
    return probe_device("dasVulkan descriptor heap probe", make_api_version(1u, 3u, 0u)) $(phys) {
        return descriptor_heap_supported(phys)
    }
}

//! Square HEAP_N indices through a storage buffer the shader reaches
//! via the resource heap; returns the results.
def public run_descriptor_heap_square() : array<uint> {
    if (volkInitialize() != 0) {
        panic("no Vulkan loader")
    }
    let api = make_api_version(1u, 3u, 0u)
    var inscope instance <- create_instance("dasVulkan descriptor heap", api)
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let fam = select_graphics_queue_family(phys)
    var inscope device <- create_device_descriptor_heap(phys, fam)
    volkLoadDevice(boost_value_to_vk(device))
    let queue = get_device_queue(device, fam, 0u)

    // the storage buffer is device-addressable: a heap descriptor names
    // a buffer by address, not by handle
    let buf_size = uint64(HEAP_N) * 4ul
    var usage : VkBufferUsageFlags
    usage.storage_buffer = true
    var inscope storage <- create_address_buffer(device, phys, buf_size, usage, true)

    // the heap: two buffer descriptor slots after the driver's reserved
    // range; the descriptor goes into HEAP_SLOT
    var inscope heap <- create_resource_heap(device, phys, HEAP_SLOT + 1)
    var storage_range : VkDeviceAddressRangeEXT
    storage_range.address = storage.address
    storage_range.size = buf_size
    var info : ResourceDescriptorInfoEXT
    info.type_ = VkDescriptorType.STORAGE_BUFFER
    info.data.pAddressRange = unsafe(addr(storage_range))
    with_mapped_memory(device, heap.buffer.memory) $(mapped) {
        let dst = descriptor_host_range(heap, mapped, HEAP_SLOT)
        write_resource_descriptors_e_x_t(device, [info], [dst])
    }

    // the pipeline: no layout; set 0 binding 0 reads heap slot HEAP_SLOT
    var inscope shader <- create_shader_module(device, square_spv)
    let mapping = map_bindings_to_heap(0u, 0u, 1u, descriptor_offset(heap, HEAP_SLOT),
        uint(heap.descriptor_size))
    var inscope pipeline <- create_compute_pipeline_descriptor_heap(device, shader, [mapping])

    var poolci : CommandPoolCreateInfo
    poolci.queueFamilyIndex = fam
    var inscope pool <- create_command_pool(device, poolci)
    run_cmd_sync(device, pool, queue) $(cmd) {
        cmd_bind_resource_heap_e_x_t(cmd, bind_info(heap))
        cmd_bind_pipeline(cmd, pipeline, VkPipelineBindPoint.COMPUTE)
        cmd_dispatch(cmd, uint(HEAP_N / SQUARE_LOCAL_SIZE_X), 1u, 1u)
    }
    var values : array<uint>
    values |> resize(HEAP_N)
    with_mapped_memory(device, storage.memory) $(mapped) {
        unsafe {
            memcpy(addr<void?>(values[0]), mapped, HEAP_N * 4)
        }
    }
    return <- values
}

[export]
def main() : int {
    if (!descriptor_heap_available()) {
        print("VK_EXT_descriptor_heap is not available on this machine\n")
        return 0
    }
    let values <- run_descriptor_heap_square()
    var fails = 0
    for (i in range(HEAP_N)) {
        if (values[i] != uint(i * i)) {
            if (fails < 5) {
                to_log(LOG_ERROR, "  mismatch at {i}: got {values[i]} expected {i * i}\n")
            }
            fails++
        }
    }
    let status = fails == 0 ? "all correct" : "{fails} WRONG"
    print("checked {HEAP_N} elements through the resource heap (out[i] == i*i): {status}\n")
    print("  spot: out[2]={int(values[2])} out[16]={int(values[16])} out[255]={int(values[255])}\n")
    return fails == 0 ? 0 : 1
}
